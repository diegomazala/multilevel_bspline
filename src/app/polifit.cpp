#include <Eigen/QR>
#include <vector>
#include "timer.h"

template<typename T>
void polyfit(const std::vector<T> &xv, const std::vector<T> &yv, std::vector<T> &coeff, int order)
{
    Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> A(xv.size(), order + 1);
    Eigen::Matrix<T, Eigen::Dynamic, 1> yv_mapped = Eigen::Matrix<T, Eigen::Dynamic, 1>::Map(&yv.front(), yv.size());
    Eigen::Matrix<T, Eigen::Dynamic, 1> result;

    assert(xv.size() == yv.size());
    assert(xv.size() >= order+1);

    // create matrix
    for (size_t i = 0; i < xv.size(); i++)
        for (size_t j = 0; j < order+1; j++)
            A(i, j) = pow(xv.at(i), j);

    // solve for linear least squares fit
    result = A.householderQr().solve(yv_mapped);

    coeff.resize(order+1);
    for (size_t i = 0; i < order+1; i++)
        coeff[i] = result[i];
}

int main()
{
    std::vector<double> x_values, y_values, coeff;
    
    x_values.push_back(0);  y_values.push_back(1);
	x_values.push_back(1);	y_values.push_back(2);
	x_values.push_back(2);	y_values.push_back(3);
	x_values.push_back(3);	y_values.push_back(4);
	x_values.push_back(4);	y_values.push_back(5);
    

    timer t;
    polyfit(x_values, y_values, coeff, 3);
    t.stop();
    std::cout << std::fixed << coeff[3] << "*x^3 + " << coeff[2] << "*x^2 + " << coeff[1] << "*x + " << coeff[0] << std::endl;
    printf("%f*x^3 + %f*x^2 + %f*x + %f\n", coeff[3], coeff[2], coeff[1], coeff[0]);
    //std::cout << msg << diff_sec() << " seconds " << std::endl;

    return 0;
}