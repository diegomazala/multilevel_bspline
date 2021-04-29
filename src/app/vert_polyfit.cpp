#include <Eigen/QR>
#include <vector>
#include <filesystem>
#include "poly_fit.h"
#include "timer.h"
#include "vector_read_write_binary.h"

namespace fs = std::filesystem;

#define USE_LERP_APPROXIMATION 0

template<typename T> T normalize(T begin, T end, T value)
{
	return (value - begin) / (end - begin + FLT_MIN);
}



static auto get_heat_map_rgb(float value)
{
	const int NUM_COLORS = 4;
	static float color[NUM_COLORS][3] = { {0,0,1}, {0,1,0}, {1,1,0}, {1,0,0} };
	// A static array of 4 colors:  (blue,   green,  yellow,  red) using {r,g,b} for each.

	int idx1;        // |-- Our desired color will be between these two indexes in "color".
	int idx2;        // |
	float fractBetween = 0;  // Fraction between "idx1" and "idx2" where our value is.

	if (value <= 0) { idx1 = idx2 = 0; }    // accounts for an input <=0
	else if (value >= 1) { idx1 = idx2 = NUM_COLORS - 1; }    // accounts for an input >=0
	else
	{
		value = value * (NUM_COLORS - 1);        // Will multiply value by 3.
		idx1 = floor(value);                  // Our desired color will be after this index.
		idx2 = idx1 + 1;                        // ... and before this index (inclusive).
		fractBetween = value - float(idx1);    // Distance between the two indexes (0-1).
	}

	float rgb[3] =
	{
		(color[idx2][0] - color[idx1][0]) * fractBetween + color[idx1][0],
		(color[idx2][1] - color[idx1][1]) * fractBetween + color[idx1][1],
		(color[idx2][2] - color[idx1][2]) * fractBetween + color[idx1][2]
	};
	return rgb;
}



// app.exe D:/Projects/MDP/Meshes/AllFaces/Quad2Triangles_Polyfit Fig05_lap_
int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout
			<< "Usage: app <path> <file_prefix> \n"
			<< "Usage: ./vert_polyfit.exe C:/models/ Fig02\n"
			<< "Usage: ./vert_polyfit.exe D:/Projects/MDP/Meshes/AllFaces/Quad2Triangles_Polyfit Fig05_lap_\n";
		return EXIT_FAILURE;
	}

	const std::string prefix{ argv[2] };
	const fs::path dir{ argv[1] };

	if (!fs::is_directory(dir))
	{
		std::cout << dir.string() << "is not a valid directory" << std::endl;
		return EXIT_FAILURE;
	}


	std::vector<std::string> files;

	//
	// Get the files to be loaded
	//
	for (const auto& entry : fs::directory_iterator(dir))
	{
		auto pos_prefix = (entry.path().has_stem()) ? entry.path().stem().string().find(prefix) : std::string::npos;
		if (pos_prefix != std::string::npos && entry.path().extension() == ".vert")
		{
			files.push_back(entry.path().string());
			std::cout << "--> " << entry.path().stem().string() << '\n';
		}
	}

	const int laplacian_levels = files.size();

	std::vector<std::vector<float>> vert_files(laplacian_levels);


	//
	// Load and process the first file
	//
	std::cout << "[Info] Loading vert file " << files[0] << std::endl;
	if (!vector_read(files[0], vert_files[0]))
	{
		std::cerr
			<< "[Error] Loading vert file : " << files[0] << std::endl
			<< "Abort" << std::endl;
		return EXIT_FAILURE;
	}
	

	const auto vertex_array_size = vert_files[0].size();
	const auto n_verts = vertex_array_size / 3;

	//
	// Load and process the others files
	//
	for (auto i = 1; i < laplacian_levels; ++i)
	{
		std::cout << "[Info] Loading vert file " << files[i] << std::endl;
		if (!vector_read(files[i], vert_files[i]))
		{
			std::cerr
				<< "[Error] Loading vert file : " << files[i] << std::endl
				<< "Abort" << std::endl;
			return EXIT_FAILURE;
		}

		if (vert_files[i].size() != vertex_array_size)
		{
			std::cerr << "[Error] Vertex count does not match. "
				<< vert_files[i].size() << " != "
				<< vertex_array_size << " Abort " << std::endl;
			return EXIT_FAILURE;
		}
	}
	

	Eigen::VectorXd x(laplacian_levels), xn(laplacian_levels);
	//x << 0, 50, 100, 250, 500, 1000, 2500, 5000, 7500, 10000, 12500, 15000, 17500, 20000;
	// x << 0, 50, 500, 5000, 10000, 20000;

	x << 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		20, 30, 40, 50, 100,
		250, 500, 1000, 2500, 5000, 7500, 
		10000, 12500, 15000, 17500, 
		19990, 19991, 19992, 19993, 19994,
		19995, 19996, 19997, 19998, 19999,
		20000;

	if (x.size() != vert_files.size())
	{
		std::cerr << "Input x array size does not match to number of vert files. ("
			<< x.size() << " != " << vert_files.size() << "). Abort" << std::endl;
		return EXIT_FAILURE;
	}

	
	std::cout << "[Info] Normalizing x array ... " << std::endl;

	for (auto j = 0; j < laplacian_levels; ++j)
	{
		xn[j] = normalize(x[0], x[x.rows() - 1], x[j]);
	}
	
	

	const int order = 3;

	std::vector<Eigen::VectorXd> yx(n_verts, { Eigen::VectorXd(laplacian_levels) });
	std::vector<Eigen::VectorXd> yy(n_verts, { Eigen::VectorXd(laplacian_levels) });
	std::vector<Eigen::VectorXd> yz(n_verts, { Eigen::VectorXd(laplacian_levels) });
	std::vector<Eigen::VectorXd> coeff_x(n_verts), coeff_y(n_verts), coeff_z(n_verts);

	std::cout << "[Info] Building matrices (copying vertices from files to data structures) ... ";
	timer t;
	t.start();

	for (auto i = 0; i < n_verts; i++)
	{
		for (auto j = 0; j < laplacian_levels; ++j)
		{
			yx[i][j] = vert_files[j][i * 3 + 0];
			yy[i][j] = vert_files[j][i * 3 + 1];
			yz[i][j] = vert_files[j][i * 3 + 2];
		}
	}
	std::cout << t.diff_sec_now() << "s" << std::endl;


	std::cout << "[Info] Computing polyfit ... \n";
	t.start();

	for (auto i = 0; i < n_verts; i++)
	{
		std::cout << '\r' << (float(i) / n_verts) * 100 << " % " << std::flush;
		coeff_x[i] = poly::fit<double>(xn, yx[i], order);
		coeff_y[i] = poly::fit<double>(xn, yy[i], order);
		coeff_z[i] = poly::fit<double>(xn, yz[i], order);
	}
	std::cout << t.diff_sec_now() << "s" << std::endl;


	//
	// Combining the coeff vector into a single vector
	//
	std::vector<float> poly;
	for (const Eigen::Matrix<double, Eigen::Dynamic, 1>& v : coeff_x)
		for (const auto& vv : v)
			poly.push_back(vv);
	for (const Eigen::Matrix<double, Eigen::Dynamic, 1>&v : coeff_y)
		for (const auto& vv : v)
			poly.push_back(vv);
	for (const Eigen::Matrix<double, Eigen::Dynamic, 1>&v : coeff_z)
		for (const auto& vv : v)
			poly.push_back(vv);
	
	const std::string out_filename = (dir / (prefix + ".poly")).string();
	std::cout << "[Info] Saving output file: " << out_filename << std::endl;
	vector_write(out_filename, poly);

	std::vector<Eigen::Matrix<double, Eigen::Dynamic, 3>> diff_l(n_verts, { Eigen::Matrix<double, Eigen::Dynamic, 3>(laplacian_levels, 3) });
	std::vector<Eigen::Matrix<double, Eigen::Dynamic, 3>> diff_c(n_verts, { Eigen::Matrix<double, Eigen::Dynamic, 3>(laplacian_levels, 3) });

	std::vector<Eigen::Vector3d> diff_l_avg(laplacian_levels, Eigen::Vector3d::Zero());
	std::vector<Eigen::Vector3d> diff_c_avg(laplacian_levels, Eigen::Vector3d::Zero());

	std::cout << "[Info] Computing difference between methods " << std::endl;

	for (auto i = 0; i < n_verts; ++i)
	{
		std::cout << '\r' << (float(i) / n_verts) * 100  << " % " << std::flush;
		auto l = laplacian_levels - 1;
		auto a = Eigen::Vector3d(yx[i][0], yy[i][0], yz[i][0]);
		auto b = Eigen::Vector3d(yx[i][l], yy[i][l], yz[i][l]);

		for (auto j = 0; j < laplacian_levels; ++j)
		{
			auto e = poly::eval_lerp<double>(a, b, xn[j]);
			diff_l[i](j, 0) = std::abs(e[0] - yx[i][j]);
			diff_l[i](j, 1) = std::abs(e[1] - yy[i][j]);
			diff_l[i](j, 2) = std::abs(e[2] - yz[i][j]);

			diff_c[i](j, 0) = std::abs(poly::eval(coeff_x[i], xn[j]) - yx[i][j]);
			diff_c[i](j, 1) = std::abs(poly::eval(coeff_y[i], xn[j]) - yy[i][j]);
			diff_c[i](j, 2) = std::abs(poly::eval(coeff_z[i], xn[j]) - yz[i][j]);

			diff_l_avg[j] += diff_l[i].row(j);
			diff_c_avg[j] += diff_c[i].row(j);
		}
	}
	std::cout << std::endl;

	std::cout << "[Info] Average Error: " << std::endl;
	for (auto j = 0; j < laplacian_levels; ++j)
	{
		diff_l_avg[j] = diff_l_avg[j] / laplacian_levels;
		diff_c_avg[j] = diff_c_avg[j] / laplacian_levels;

		std::cout << std::fixed << std::setw(6) << j << " : " << xn[j]  << " : "<< x[j] << " :  (l) " << diff_l_avg[j].transpose() << "  :  (c)" << diff_c_avg[j].transpose() << std::endl;
	}

    return EXIT_SUCCESS;
}