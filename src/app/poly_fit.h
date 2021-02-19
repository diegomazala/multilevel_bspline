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
}
