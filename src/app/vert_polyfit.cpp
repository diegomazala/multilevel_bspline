#include <Eigen/QR>
#include <vector>
#include <filesystem>
#include "poly_fit.h"
#include "timer.h"
#include "vector_read_write_binary.h"

namespace fs = std::filesystem;

#define new_poly 1

template<typename T> T lerp(T begin, T end, T value)
{
	return (value - begin) / (end - begin);
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


	using vertvec_t = std::vector<float>;
	std::vector<vertvec_t> vert_files(files.size());


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



	//
	// Load and process the others files
	//
	for (auto i = 1; i < files.size(); ++i)
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


	Eigen::VectorXd x(14), xn(14);
	x << 0, 50, 100, 250, 500, 1000, 2500, 5000, 7500, 10000, 12500, 15000, 17500, 20000;
	
	for (auto j = 0; j < x.size(); ++j)
	{
		xn[j] = lerp(x[0], x[x.rows() - 1], x[j]);
	}

	if (x.size() != vert_files.size())
	{
		std::cerr << "Input x array size does not match to number of vert files. ("
			<< x.size() << " != " << vert_files.size() << "). Abort" << std::endl;
		return EXIT_FAILURE;
	}


	const int order = 3;
	std::vector<Eigen::Matrix<double, 14, 1>> yx(vertex_array_size);
	std::vector<Eigen::Matrix<double, 14, 1>> yy(vertex_array_size);
	std::vector<Eigen::Matrix<double, 14, 1>> yz(vertex_array_size);
	std::vector<Eigen::Matrix<double, Eigen::Dynamic, 1>> coeff_x(vertex_array_size);	// rows = order + 1
	std::vector<Eigen::Matrix<double, Eigen::Dynamic, 1>> coeff_y(vertex_array_size);
	std::vector<Eigen::Matrix<double, Eigen::Dynamic, 1>> coeff_z(vertex_array_size);
	std::vector<double> error_x(vertex_array_size);	
	std::vector<double> error_y(vertex_array_size);
	std::vector<double> error_z(vertex_array_size);


	std::cout << "[Info] Building matrices ..." << std::endl;

	for (auto i = 0; i < vertex_array_size; i += 3)
	{
		for (auto j = 0; j < vert_files.size(); ++j)
		{
			yx[i][j] = vert_files[j][i + 0];
			yy[i][j] = vert_files[j][i + 1];
			yz[i][j] = vert_files[j][i + 2];
		}
	}


	std::cout << "Computing polyfit ..." << std::endl;
	timer t;

	for (auto i = 0; i < vertex_array_size; i += 3)
	{
		coeff_x[i] = poly::fit<double>(xn, yx[i], order);
		coeff_y[i] = poly::fit<double>(xn, yy[i], order);
		coeff_z[i] = poly::fit<double>(xn, yz[i], order);
	}
	t.stop();
	std::cout << "Polyfit Computing time elapsed: " << t.diff_sec() << " seconds " << std::endl;

	std::cout << "Computing error ..." << std::endl;
	t.start();
	for (auto i = 0; i < vertex_array_size; i += 3)
	{
		for (auto j = 0; j < vert_files.size(); ++j)
		{
			error_x[i] = std::sqrt(std::abs(std::pow(poly::eval(coeff_x[i], xn[j]), 2) - std::pow(yx[i][j], 2)));
			error_y[i] = std::sqrt(std::abs(std::pow(poly::eval(coeff_y[i], xn[j]), 2) - std::pow(yy[i][j], 2)));
			error_z[i] = std::sqrt(std::abs(std::pow(poly::eval(coeff_z[i], xn[j]), 2) - std::pow(yz[i][j], 2)));
		}
	}
	t.stop();
	std::cout << "Polyfit Error time elapsed: " << t.diff_sec() << " seconds " << std::endl;
	
	
	

	
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

	std::vector<float> error(error_x.size() * 3, 0);
	std::vector<float> err_color(error_x.size() * 3, 0);

	float minx = FLT_MAX, miny = FLT_MAX, minz = FLT_MAX, maxx = FLT_MIN, maxy = FLT_MIN, maxz = FLT_MIN;

	for (auto i = 0; i < error_x.size(); ++i)
	{
		error[i * 3 + 0] = error_x[i];
		error[i * 3 + 1] = error_y[i];
		error[i * 3 + 2] = error_z[i];

		if (minx > error_x[i]) minx = error_x[i];
		if (miny > error_y[i]) miny = error_y[i];
		if (minz > error_z[i]) minz = error_z[i];
		if (maxx < error_x[i]) maxx = error_x[i];
		if (maxy < error_y[i]) maxy = error_y[i];
		if (maxz < error_z[i]) maxz = error_z[i];
	}

	std::cout
		<< minx << ' ' << miny << ' ' << minz << std::endl
		<< maxx << ' ' << maxy << ' ' << maxz << std::endl;

	float min_err = FLT_MAX, max_err = FLT_MIN;

	for (auto i = 0; i < error_x.size(); ++i)
	{
		float x_norm = (error[i * 3 + 0] - minx) / ((maxx - minx) + FLT_MIN);
		float y_norm = (error[i * 3 + 1] - miny) / ((maxy - miny) + FLT_MIN);
		float z_norm = (error[i * 3 + 2] - minz) / ((maxz - minz) + FLT_MIN);

		float err = std::sqrt(std::pow(x_norm, 2) + std::pow(y_norm, 2) + std::pow(y_norm, 2));

		if (min_err > err) min_err = err;
		if (max_err < err) max_err = err;

		auto rgb = get_heat_map_rgb(err);

		err_color[i * 3 + 0] = rgb[0];
		err_color[i * 3 + 1] = rgb[1];
		err_color[i * 3 + 2] = rgb[2];
	}


	std::cout
		<< min_err << ' ' << max_err << std::endl;

	const std::string out_error_filename = (dir / (prefix + ".poly_err")).string();
	std::cout << "[Info] Saving output file: " << out_error_filename << std::endl;
	vector_write(out_error_filename, error);

	const std::string out_error_rgb_filename = (dir / (prefix + ".poly_rgb")).string();
	std::cout << "[Info] Saving output file: " << out_error_rgb_filename << std::endl;
	vector_write(out_error_rgb_filename, err_color);

    return EXIT_SUCCESS;
}