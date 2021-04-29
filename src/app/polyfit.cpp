#include "poly_fit.h"
#include <vector>
#include <iomanip>
#include <fstream>
#include "timer.h"

#define LEVELS 14


template<typename T> T normalize(T begin, T end, T value)
{
	return (value - begin) / (end - begin + FLT_MIN);
}


void compute_error_lerp()
{
	Eigen::VectorXd x(LEVELS), yx(LEVELS), yy(LEVELS), yz(LEVELS);

	x << 0, 50, 100, 250, 500, 1000, 2500, 5000, 7500, 10000, 12500, 15000, 17500, 20000;
	yx << 1.664268, 1.664402, 1.664637, 1.665649, 1.667658, 1.671632, 1.680969, 1.691985, 1.700001, 1.706105, 1.710957, 1.714973, 1.718405, 1.721413;
	yy << 169.585770, 169.584137, 169.583008, 169.578857, 169.571259, 169.556000, 169.510559, 169.440292, 169.382278, 169.336670, 169.297943, 169.259796, 169.221649, 169.186157;
	yz << 10.966385, 10.964172, 10.962615, 10.957320, 10.946722, 10.923341, 10.855303, 10.763299, 10.692936, 10.636847, 10.590565, 10.551096, 10.516779, 10.486420;

	for (auto i = 0; i < x.size(); ++i)
	{
		auto vx = (x[i] - x[0]) / (x[x.rows() - 1] - x[0]) * (yx[yx.rows() - 1] - yx[0]) + yx[0];
		auto vy = (x[i] - x[0]) / (x[x.rows() - 1] - x[0]) * (yy[yy.rows() - 1] - yy[0]) + yy[0];
		auto vz = (x[i] - x[0]) / (x[x.rows() - 1] - x[0]) * (yz[yz.rows() - 1] - yz[0]) + yz[0];

		std::cout << vx << ' ' << vy << ' ' << vz << std::endl;
	}
}


void test_from_file(const std::string& filename)
{
	std::ifstream file(filename);

	if (!file.is_open())
	{
		std::cout << "Error: Could not open file: " << filename << std::endl;
		return;
	}

	int count = 0;
	int interval = 0;
	file >> count >> interval;

	Eigen::VectorXd x(count), yx(count), yy(count), yz(count);

	for (auto i = 0; i < count; ++i)
	{
		x[i] = i * interval;
		file >> yx[i] >> yy[i] >> yz[i];
	}


	int order = 3;

	std::cout << "Computing polyfit x..." << std::endl;
	auto coeff_x = poly::fit(x, yx, order);
	std::cout << "Computing polyfit y..." << std::endl;
	auto coeff_y = poly::fit(x, yy, order);
	std::cout << "Computing polyfit z..." << std::endl;
	auto coeff_z = poly::fit(x, yz, order);

	std::cout << "Computing polyfit error..." << std::endl;
	auto error = poly::compute_error_xyz(x, yx, yy, yz, coeff_x, coeff_y, coeff_z);
	std::cout << "\nError xyz : " << error.transpose() << std::endl;
}


void test_verts_red(int order)
{
	Eigen::VectorXd x(LEVELS), yx(LEVELS), yy(LEVELS), yz(LEVELS);

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

	std::cout << "error xyz : " << poly::compute_error(x, yx, yy, yz, coeff_x, coeff_y, coeff_z) << std::endl;
}


void test_verts_yellow(int order)
{
	Eigen::VectorXd x(LEVELS), yx(LEVELS), yy(LEVELS), yz(LEVELS);

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

	std::cout << "error xyz : " << poly::compute_error(x, yx, yy, yz, coeff_x, coeff_y, coeff_z) << std::endl;
}


void test_verts_mult(int order)
{
	const int n_verts = 2;
	Eigen::VectorXd x(LEVELS);
	x << 0, 50, 100, 250, 500, 1000, 2500, 5000, 7500, 10000, 12500, 15000, 17500, 20000;

	std::vector<Eigen::VectorXd> yx(n_verts, { Eigen::VectorXd(LEVELS) });
	std::vector<Eigen::VectorXd> yy(n_verts, { Eigen::VectorXd(LEVELS) });
	std::vector<Eigen::VectorXd> yz(n_verts, { Eigen::VectorXd(LEVELS) });
	
	yx[0] << 1.664268, 1.664402, 1.664637, 1.665649, 1.667658, 1.671632, 1.680969, 1.691985, 1.700001, 1.706105, 1.710957, 1.714973, 1.718405, 1.721413, 3.418428;
	yx[1] << 3.418428, 3.418810, 3.419191, 3.420278, 3.421903, 3.424710, 3.431972, 3.442448, 3.451100, 3.458237, 3.464099, 3.468845, 3.472584, 3.475405;

	yy[0] << 169.585770, 169.584137, 169.583008, 169.578857, 169.571259, 169.556000, 169.510559, 169.440292, 169.382278, 169.336670, 169.297943, 169.259796, 169.221649, 169.186157;
	yy[1] << 162.945496, 162.945435, 162.945435, 162.945435, 162.945435, 162.945435, 162.945435, 162.945435, 162.945435, 162.945435, 162.945435, 162.945435, 162.945435, 162.945435;

	yz[0] << 10.966385, 10.964172, 10.962615, 10.957320, 10.946722, 10.923341, 10.855303, 10.763299, 10.692936, 10.636847, 10.590565, 10.551096, 10.516779, 10.486420;
	yz[1] << 9.488377, 9.488394, 9.488400, 9.488398, 9.488281, 9.487712, 9.486279, 9.487717, 9.491785, 9.496988, 9.502705, 9.508610, 9.514561, 9.520452;

	std::vector<Eigen::VectorXd> coeff_x(n_verts), coeff_y(n_verts), coeff_z(n_verts);
	for (auto i = 0; i < n_verts; ++i)
	{
		coeff_x[i] = poly::fit(x, yx[i], order);
		coeff_y[i] = poly::fit(x, yy[i], order);
		coeff_z[i] = poly::fit(x, yz[i], order);
	}

	for (auto i = 0; i < n_verts; ++i)
	{
		for (auto v : x)
		{
			std::cout << poly::eval(coeff_x[i], v) << ' ' << poly::eval(coeff_y[i], v) << ' ' << poly::eval(coeff_z[i], v) << std::endl;
		}
	}

	for (auto i = 0; i < n_verts; ++i)
		std::cout << "error : " << poly::compute_error(x, yx[i], yy[i], yz[i], coeff_x[i], coeff_y[i], coeff_z[i]) << std::endl;

	for (auto i = 0; i < n_verts; ++i)
		std::cout << "error : " << poly::compute_error_lerp(x, yx[i], yy[i], yz[i]) << std::endl;
}

int main()
{
	std::cout << std::fixed << std::setprecision(6);

	//compute_error_lerp();
	//test_verts_red(1);
	//test_verts_yellow(3);
	//test_verts_mult(3);

	test_from_file("E:/Projects/MDP/multilevel_bspline/build_msvc_2019_x64/bin/Release/obj_vertex_extrac.log");

    return 0;
}