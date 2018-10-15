#pragma once

#include <cmath>
#include <functional>
#include <vector>

// surface namespace starts here
namespace surface
{

	//
	// B-splines functions
	//
	template <typename decimal_t>
	static std::function<decimal_t(decimal_t)> B[4] = {
		[](decimal_t t) { return std::pow((1 - t), 3) / 6; },
		[](decimal_t t) {
			return (3 * std::pow(t, 3) - 6 * std::pow(t, 2) + 4) / 6;
		},
		[](decimal_t t) {
			return (-3 * std::pow(t, 3) + 3 * std::pow(t, 2) + 3 * t + 1) / 6;
		},
		[](decimal_t t) { return std::pow(t, 3) / 6; } };

	//
	// B-splines first derivative functions
	//
	template <typename decimal_t>
	static std::function<decimal_t(decimal_t)> dB[4] = {
		[](decimal_t t) { return 0.5 * (1 - t) * (t - 1); },
		[](decimal_t t) { return 1.5 * t * t - 2 * t;   },
		[](decimal_t t) { return -(1.5 * t * t) + t + 0.5; },
		[](decimal_t t) { return t * t / 2.0; } };

	//
	// B-splines second derivative functions
	//
	template <typename decimal_t>
	static std::function<decimal_t(decimal_t)> ddB[4] = {
		[](decimal_t t) { return -t + 1; },
		[](decimal_t t) { return 3 * t - 2;   },
		[](decimal_t t) { return -3 * t + 1; },
		[](decimal_t t) { return t; } };

	// Pre-evaluated tensors equivalent to B(k,s)*B(l,t)
	template <typename decimal_t>
	static decimal_t tensor_BB[3][3] =
	{ {1. / 36., 1. / 9., 1. / 36.},
	{1. / 9., 4. / 9., 1. / 9.},
	{1. / 36., 1. / 9., 1. / 36.} };

	// Pre-evaluated tensor products of B-splines, in a grid index
	// Mixed derivative an positional tensor for the x-component.
	// equivalent to dB(k,s)*B(l,t).
	// (The second row with zeros is not used in evaluations.)
	template <typename decimal_t>
	static decimal_t tensor_dBB[3][3] = { {-1. / 12., -1. / 3., -1. / 12.},
									  {0., 0., 0.},
									  {1. / 12., 1. / 3., 1. / 12.} };

	// The transpose of the matrix above, for the y-components,
	// equivalent to B(k,s)*dB(l,t)
	template <typename decimal_t>
	static decimal_t tensor_BdB[3][3] = { {-1. / 12., 0., 1. / 12.},
									  {-1. / 3., 0., 1. / 3.},
									  {-1. / 12., 0., 1. / 12.} };

	//
	// Function to test doubles and floats considering decimals
	//
	template <typename T>
	constexpr bool logically_equal(const T &a, const T &b,
		const T error_factor = 1)
	{
		return a == b ||
			std::abs(a - b) < std::abs(std::min(a, b)) *
			std::numeric_limits<T>::epsilon() * error_factor;
	}

	template <typename decimal_t>
	class bspline_t
	{
	public:
		using matrix_t = std::vector<std::vector<decimal_t>>;
		using matrix4_t = std::array<std::array<decimal_t, 4>, 4>;

		bspline_t() = default;
		virtual ~bspline_t() = default;
		bspline_t(const bspline_t &) = default;
		bspline_t(bspline_t &&) = default;
		bspline_t &operator =(const bspline_t &) = default;
		bspline_t &operator =(bspline_t &&) = default;



		bspline_t(const decimal_t *_x_vec_ptr, const decimal_t *_y_vec_ptr,
			const decimal_t *_z_vec_ptr, size_t _point_count, uint32_t _m,
			uint32_t _n)
			: x_ptr(_x_vec_ptr), y_ptr(_y_vec_ptr), z_ptr(_z_vec_ptr),
			point_count(_point_count), m(_m), n(_n),
			phi((m + 3), std::vector<decimal_t>(n + 3, 0)),
			delta_z(point_count)
		{
			this->init();
		}

		//
		// Run the algorith to build bspline surface
		//
		void compute()
		{
			matrix_t delta = { (m + 3), std::vector<decimal_t>(n + 3, 0) };
			matrix_t omega = { (m + 3), std::vector<decimal_t>(n + 3, 0) };
			//this->phi = {(m + 3), std::vector<decimal_t>(n + 3, 0)};

			const decimal_t interval_normalization_factor_u = m * urange_inv;
			const decimal_t interval_normalization_factor_v = n * vrange_inv;

			const decimal_t cell_x = static_cast<decimal_t>(1) / (m + 3 - 1);
			const decimal_t cell_y = static_cast<decimal_t>(1) / (n + 3 - 1);


			for (size_t index = 0; index < point_count; ++index)
			{
				const decimal_t x = x_ptr[index];
				const decimal_t y = y_ptr[index];

				// here we apply a offset to make the algorithm stable
				// this offset is the average of all z values
				const decimal_t z = z_ptr[index] - average_z;

				// Map to the half open domain Omega = [0,m) x [0,n)
				// The mapped u and v must be (strictly) less than m and n respectively
				decimal_t u = (x - umin) * interval_normalization_factor_u;
				decimal_t v = (y - vmin) * interval_normalization_factor_v;

				//
				// Compute [i,j] indices and [s,t] params
				//
				auto[i, j, s, t] = compute_ijst(u, v);

				//std::cout << std::fixed << "[" << u << ' ' << v << "] : " << s << ' ' << t << '\n';
				//std::cout << std::fixed << "[" << i << ' ' << j << "] : " << s << ' ' << t << '\n';
				//std::cout << std::abs(u - s) + std::abs(v - t)  << std::endl;

				//
				// Compute w_kl's and sum_sum w²_ab
				//
				const auto[w, sum_w_ab2] = compute_wkl_and_sum_w_ab2(s, t);
				const auto sum_w_ab2_inv = 1 / sum_w_ab2;

				//
				// Update delta and gamma matrices
				//
				for (auto k = 0; k < 4; ++k)
				{
					for (auto l = 0; l < 4; ++l)
					{
						// eq(3)
						auto phi_kl = w[k][l] * z * sum_w_ab2_inv;

						auto w2 = w[k][l] * w[k][l];

						delta[i + k][j + l] += w2 * phi_kl;
						omega[i + k][j + l] += w2;
					}
				}
			}



			//
			// Compute phi matrix [m+3,n+3]
			//
			for (uint32_t i = 0; i < m + 3; ++i)
			{
				for (uint32_t j = 0; j < n + 3; ++j)
				{
					// avoiding division by zero
					if (!logically_equal(omega[i][j], static_cast<decimal_t>(0)))
					{
						// eq(5)

						// this is the original from paper
						// this->phi[i][j] = delta[i][j] / omega[i][j];

						// here we remove the offset inserted in the begining of the algorithm
						this->phi[i][j] += delta[i][j] / omega[i][j] + this->average_z;
					}
					else
					{
						// this is the original from paper
						// this->phi[i][j] = 0;

						// here we remove the offset inserted in the begining of the algorithm
						this->phi[i][j] += average_z;
					}
				}
			}
		}


		//
		// Overload operator () to evaluate the function at (u, v)
		// (u, v) must be inside of the domain
		//
		decimal_t operator()(decimal_t u, decimal_t v) const { return eval(u, v); }

		//
		// Overload operator () to evaluate the function at (i, j) position.
		// (i, j) is a index in the spline coefficient matrix
		// (i, j) must be inside of the domain
		// 0 <= i <= m and  0 <= j <= n, where m_ and n_ are retrieved by
		//
		//decimal_t operator()(uint32_t i, uint32_t j) const { return eval(i, j); }

		//
		// Compute the difference between the original z and
		// the returned value from function
		//
		decimal_t compute_error()
		{
			decimal_t sum_error = 0;
			for (size_t index = 0; index < point_count; ++index)
			{
				const decimal_t x = x_ptr[index];
				const decimal_t y = y_ptr[index];
				const decimal_t z = z_ptr[index];

				const auto f = eval(x, y);

				delta_z[index] = z - f;
				sum_error += std::abs(z - f);
			}
			return sum_error;
		}

		decimal_t error(size_t point_index) const { return delta_z[point_index]; }

		const decimal_t *get_delta_z() const { return delta_z.data(); }

		const decimal_t get_phi(size_t i, size_t j) const { return this->phi[i][j]; }

		const auto& get_phi_matrix() const { return this->phi; }

		//protected:

		  //
		  // Evaluate the function at (u, v)
		  //
		decimal_t eval(decimal_t u, decimal_t v) const
		{
			// Map to the half open domain Omega = [0,m) x [0,n)
			u = (u - umin) * urange_inv * m;
			v = (v - vmin) * vrange_inv * n;

			//
			// Compute [i,j] indices and [s,t] params
			//
			auto[i, j, s, t] = compute_ijst(u, v);


			//std::cout << std::fixed << "[" << u << ' ' << v << "] : " << s << ' ' << t << '\n';
			//
			// Evaluate the function
			//
			decimal_t val = 0;
			for (auto k = 0; k < 4; ++k)
			{
				for (auto l = 0; l < 4; ++l)
				{
					val +=
						this->phi[i + k][j + l] * B<decimal_t>[k](s) * B<decimal_t>[l](t);
					//(*this->phi)[i + k][j + l] * B<decimal_t>[k](s) * B<decimal_t>[l](t);
				}
			}
			return val;
		}

		// //
		// // Evaluate the function at (u, v)
		// //
		// decimal_t eval(uint32_t i, uint32_t j) const
		// {
		//     //i = i - 1;
		//     //j = j - 1;
		//     int m_size = 3;

		//     decimal_t val = 0.0;
		//     // Only over 3x3 coefficients in the C2 case and 2x2 in the C1 case
		//     for (auto k = 0; k < m_size; k++) 
		//     {
		//         for (auto l = 0; l < m_size; l++) 
		//         {
		//             //val += this->phi[i + k][j + l] * tensor_BB<decimal_t>[k][l];
		//             val += this->phi[i + k][j + l];
		//         }
		//     }

		//     return val;
		// }

		std::tuple<decimal_t, decimal_t> derivative(decimal_t u, decimal_t v)
		{
			// Map to the half open domain Omega = [0,m) x [0,n)
			u = (u - umin) * urange_inv * m;
			v = (v - vmin) * vrange_inv * n;

			//
			// Compute [i,j] indices and [s,t] params
			//
			auto[i, j, s, t] = compute_ijst(u, v);

			//
			// Evaluate the function
			//
			decimal_t valx = 0;
			decimal_t valy = 0;
			for (auto k = 0; k < 4; ++k)
			{
				for (auto l = 0; l < 4; ++l)
				{
					valx += this->phi[i + k][j + l] * dB<decimal_t>[k](s) * B<decimal_t>[l](t);
					valy += this->phi[i + k][j + l] * B<decimal_t>[k](s) * dB<decimal_t>[l](t);
				}
			}

			// find cross product and normalize
			// (-df/du, -df/dv, 1)
			valx *= ((decimal_t)m / (umax - umin));
			valy *= ((decimal_t)n / (vmax - vmin));

			return { valx, valy };
		}

		//
		// Compute [i,j] indices and [s,t] params
		//
		std::tuple<int64_t, int64_t, decimal_t, decimal_t>
			compute_ijst(decimal_t x, decimal_t y) const
		{
			decimal_t floor[2] = { std::floor(x), std::floor(y) };
			auto i = static_cast<int64_t>(floor[0] - 1);
			auto j = static_cast<int64_t>(floor[1] - 1);
			decimal_t s = x - floor[0];
			decimal_t t = y - floor[1];

			if (i == m - 1)
			{
				i--;
				s = 1;
			}
			if (j == n - 1)
			{
				j--;
				t = 1;
			}

			// in the paper, the range goes from [-1, m + 1]
			// here, we use [0, m + 2]
			i += 1; // +1 to go for positive indices
			j += 1;

			return { i, j, s, t };
		}

		//
		// Compute w_kl's and sum_sum w²_ab
		//
		static std::tuple<matrix4_t, decimal_t>
			compute_wkl_and_sum_w_ab2(decimal_t s, decimal_t t)
		{
			matrix4_t w;
			decimal_t sum_w_ab2 = 0;
			for (auto k = 0; k < 4; ++k)
			{
				for (auto l = 0; l < 4; ++l)
				{
					w[k][l] = B<decimal_t>[k](s) * B<decimal_t>[l](t);
					sum_w_ab2 += w[k][l] * w[k][l];
				}
			}
			return { w, sum_w_ab2 };
			// return std::make_tuple(w, sum_w_ab2);
		}

		void init()
		{
			umin = std::numeric_limits<decimal_t>::max();
			vmin = std::numeric_limits<decimal_t>::max();
			umax = std::numeric_limits<decimal_t>::min();
			vmax = std::numeric_limits<decimal_t>::min();

			decimal_t sum_z = 0;

			for (size_t index = 0; index < point_count; ++index)
			{
				const decimal_t x = x_ptr[index];
				const decimal_t y = y_ptr[index];
				const decimal_t z = z_ptr[index];

				if (x < umin)
				{
					umin = x;
				}
				if (y < vmin)
				{
					vmin = y;
				}
				if (x > umax)
				{
					umax = x;
				}
				if (y > vmax)
				{
					vmax = y;
				}

				sum_z += z;
			}

			urange_inv = 1 / (umax - umin);
			vrange_inv = 1 / (vmax - vmin);

			average_z = sum_z / point_count;
		}

		//
		// Attributes
		//
		const decimal_t *x_ptr;
		const decimal_t *y_ptr;
		const decimal_t *z_ptr;
		const size_t point_count = 0;
		uint32_t m, n;
		matrix_t phi;
		decimal_t umin, vmin, umax, vmax, urange_inv, vrange_inv, average_z;
		std::vector<decimal_t> delta_z;

	};

} // namespace surface
