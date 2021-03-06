#include <Eigen/QR>


namespace poly
{
	template<typename T>
	Eigen::Matrix<T, Eigen::Dynamic, 1> fit(Eigen::Matrix<T, Eigen::Dynamic, 1> x, Eigen::Matrix<T, Eigen::Dynamic, 1> y, int order)
	{
		assert(x.size() == y.size());
		assert(order >= 1 && order <= x.size() - 1);
		Eigen::MatrixXd A(x.size(), order + 1);

		for (auto i = 0; i < x.size(); i++)
		{
			A(i, 0) = 1.0;
		}

		for (auto j = 0; j < x.size(); j++)
		{
			for (auto i = 0; i < order; i++)
			{
				A(j, i + 1) = A(j, i) * x(j);
			}
		}

		return A.householderQr().solve(y);
	}


	template<typename T>
	T eval(Eigen::Matrix<T, Eigen::Dynamic, 1> coeffs, T x)
	{
		T result = 0.0;
		for (auto i = 0; i < coeffs.size(); i++)
			result += coeffs[i] * std::pow(x, i);
		return result;
	}


	template<typename T>
	T compute_error(
		const Eigen::Matrix<T, Eigen::Dynamic, 1>& x,
		const Eigen::Matrix<T, Eigen::Dynamic, 1>& y,
		const Eigen::Matrix<T, Eigen::Dynamic, 1>& coeff)
	{
		T sum_error = 0;
		for (auto i = 0; i < x.size(); ++i)
		{
			sum_error += std::abs(y[i] - poly::eval(coeff_x, x[i]));
		}
		return sum_error;
	}



	template<typename T>
	T compute_error(
		const Eigen::Matrix<T, Eigen::Dynamic, 1>& x,
		const Eigen::Matrix<T, Eigen::Dynamic, 1>& yx,
		const Eigen::Matrix<T, Eigen::Dynamic, 1>& yy,
		const Eigen::Matrix<T, Eigen::Dynamic, 1>& yz,
		const Eigen::Matrix<T, Eigen::Dynamic, 1>& coeff_x,
		const Eigen::Matrix<T, Eigen::Dynamic, 1>& coeff_y,
		const Eigen::Matrix<T, Eigen::Dynamic, 1>& coeff_z)
	{
		T sum_error = 0;
		for (auto i = 0; i < x.size(); ++i)
		{
			auto ex = std::pow(yx[i] - poly::eval(coeff_x, x[i]), 2);
			auto ey = std::pow(yy[i] - poly::eval(coeff_y, x[i]), 2);
			auto ez = std::pow(yz[i] - poly::eval(coeff_z, x[i]), 2);

			sum_error += std::sqrt(ex + ey + ez);
			//std::cout << error_xyz[i] << std::endl;
		}
		return sum_error;
	}


	template<typename T>
	Eigen::Matrix<T, 3, 1> compute_error_xyz(
		const Eigen::Matrix<T, Eigen::Dynamic, 1>& x,
		const Eigen::Matrix<T, Eigen::Dynamic, 1>& yx,
		const Eigen::Matrix<T, Eigen::Dynamic, 1>& yy,
		const Eigen::Matrix<T, Eigen::Dynamic, 1>& yz,
		const Eigen::Matrix<T, Eigen::Dynamic, 1>& coeff_x,
		const Eigen::Matrix<T, Eigen::Dynamic, 1>& coeff_y,
		const Eigen::Matrix<T, Eigen::Dynamic, 1>& coeff_z)
	{
		Eigen::Matrix<T, 3, 1> sum_error(0, 0, 0);
		for (auto i = 0; i < x.size(); ++i)
		{
			sum_error[0] += std::abs(yx[i] - poly::eval(coeff_x, x[i]));
			sum_error[1] += std::abs(yy[i] - poly::eval(coeff_y, x[i]));
			sum_error[2] += std::abs(yz[i] - poly::eval(coeff_z, x[i]));
		}
		return sum_error;
	}


	template<typename T>
	T compute_error_lerp(
		const Eigen::Matrix<T, Eigen::Dynamic, 1>& x,
		const Eigen::Matrix<T, Eigen::Dynamic, 1>& yx,
		const Eigen::Matrix<T, Eigen::Dynamic, 1>& yy,
		const Eigen::Matrix<T, Eigen::Dynamic, 1>& yz)
	{
		T sum_error = 0;
		for (auto i = 0; i < x.size(); ++i)
		{
			auto ex = std::pow(yx[i] - ((x[i] - x[0]) / (x[x.rows() - 1] - x[0]) * (yx[yx.rows() - 1] - yx[0]) + yx[0]), 2);
			auto ey = std::pow(yy[i] - ((x[i] - x[0]) / (x[x.rows() - 1] - x[0]) * (yy[yy.rows() - 1] - yy[0]) + yy[0]), 2);
			auto ez = std::pow(yz[i] - ((x[i] - x[0]) / (x[x.rows() - 1] - x[0]) * (yz[yz.rows() - 1] - yz[0]) + yz[0]), 2);

			sum_error += std::sqrt(ex + ey + ez);
		}
		return sum_error;
	}

	template<typename T>
	Eigen::Matrix<T, 3, 1> compute_error_xyz_lerp(
		const Eigen::Matrix<T, Eigen::Dynamic, 1>& x,
		const Eigen::Matrix<T, Eigen::Dynamic, 1>& yx,
		const Eigen::Matrix<T, Eigen::Dynamic, 1>& yy,
		const Eigen::Matrix<T, Eigen::Dynamic, 1>& yz)
	{
		Eigen::Matrix<T, 3, 1> sum_error(0, 0, 0);
		for (auto i = 0; i < x.size(); ++i)
		{
			sum_error[0] += std::abs(yx[i] - (x[i] - x[0]) / (x[x.rows() - 1] - x[0]) * (yx[yx.rows() - 1] - yx[0]) + yx[0]);
			sum_error[1] += std::abs(yy[i] - (x[i] - x[0]) / (x[x.rows() - 1] - x[0]) * (yy[yy.rows() - 1] - yy[0]) + yy[0]);
			sum_error[2] += std::abs(yz[i] - (x[i] - x[0]) / (x[x.rows() - 1] - x[0]) * (yz[yz.rows() - 1] - yz[0]) + yz[0]);
		}
		return sum_error;
	}
}
