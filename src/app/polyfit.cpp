#include "poly_fit.h"
#include <vector>
#include <iomanip>
#include "timer.h"

int32_t nearestPowerOfTwo(int32_t n)
{
	int v = n;

	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++; // next power of 2

    int32_t x = v >> 1; // previous power of 2

	return (v - n) > (n - x) ? x : v;
}


void test_verts_red(int order)
{
	Eigen::VectorXd x(14), yx(14), yy(14), yz(14);

	x << 0, 50, 100, 250, 500, 1000, 2500, 5000, 7500, 10000, 12500, 15000, 17500, 20000;
	yx << 1.664268, 1.664402, 1.664637, 1.665649, 1.667658, 1.671632, 1.680969, 1.691985, 1.700001, 1.706105, 1.710957, 1.714973, 1.718405, 1.721413;
	yy << 169.585770, 169.584137, 169.583008, 169.578857, 169.571259, 169.556000, 169.510559, 169.440292, 169.382278, 169.336670, 169.297943, 169.259796, 169.221649, 169.186157;
	yz << 10.966385, 10.964172, 10.962615, 10.957320, 10.946722, 10.923341, 10.855303, 10.763299, 10.692936, 10.636847, 10.590565, 10.551096, 10.516779, 10.486420;

	auto coeff_x = poly::fit(x, yx, order);
	auto coeff_y = poly::fit(x, yy, order);
	auto coeff_z = poly::fit(x, yz, order);

	for (auto v : x)
	{
		std::cout 
			<< poly::eval(coeff_x, v) << ' ' 
			<< poly::eval(coeff_y, v) << ' '
			<< poly::eval(coeff_z, v) << ' '
			<< std::endl;
	}
}


void test_verts_yellow(int order)
{
	Eigen::VectorXd x(14), yx(14), yy(14), yz(14);

	x << 0, 50, 100, 250, 500, 1000, 2500, 5000, 7500, 10000, 12500, 15000, 17500, 20000;
	yx << 3.418428, 3.418810, 3.419191, 3.420278, 3.421903, 3.424710, 3.431972, 3.442448, 3.451100, 3.458237, 3.464099, 3.468845, 3.472584, 3.475405;
	yy << 162.945496, 162.945435, 162.945435, 162.945435, 162.945435, 162.945435, 162.945435, 162.945435, 162.945435, 162.945435, 162.945435, 162.945435, 162.945435, 162.945435;
	yz << 9.488377, 9.488394, 9.488400, 9.488398, 9.488281, 9.487712, 9.486279, 9.487717, 9.491785, 9.496988, 9.502705, 9.508610, 9.514561, 9.520452;

	auto coeff_x = poly::fit(x, yx, order);
	auto coeff_y = poly::fit(x, yy, order);
	auto coeff_z = poly::fit(x, yz, order);

	for (auto v : x)
	{
		std::cout
			<< poly::eval(coeff_x, v) << ' '
			<< poly::eval(coeff_y, v) << ' '
			<< poly::eval(coeff_z, v) << ' '
			<< std::endl;
	}
}


int main()
{
	std::cout << std::fixed << std::setprecision(6);

	test_verts_red(3);
	//test_verts_yellow(3);

    return 0;
}