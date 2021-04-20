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

	const int laplacian_levels = files.size();

	using vertvec_t = std::vector<float>;
	std::vector<vertvec_t> vert_files(laplacian_levels);


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
	x << 0, 50, 100, 250, 500, 1000, 2500, 5000, 7500, 10000, 12500, 15000, 17500, 20000;
	
	for (auto j = 0; j < x.size(); ++j)
	{
		xn[j] = normalize(x[0], x[x.rows() - 1], x[j]);
	}

	if (x.size() != vert_files.size())
	{
		std::cerr << "Input x array size does not match to number of vert files. ("
			<< x.size() << " != " << vert_files.size() << "). Abort" << std::endl;
		return EXIT_FAILURE;
	}

	const int order = 3;

	std::vector<Eigen::VectorXd> yx(vertex_array_size, { Eigen::VectorXd(laplacian_levels) });
	std::vector<Eigen::VectorXd> yy(vertex_array_size, { Eigen::VectorXd(laplacian_levels) });
	std::vector<Eigen::VectorXd> yz(vertex_array_size, { Eigen::VectorXd(laplacian_levels) });
	std::vector<Eigen::VectorXd> coeff_x(vertex_array_size), coeff_y(vertex_array_size), coeff_z(vertex_array_size);
	std::vector<Eigen::VectorXd> err(n_verts);
	std::vector<Eigen::VectorXf> err_norm(n_verts);

	std::cout << "[Info] Building matrices (copying vertices from files to data structures) ... ";
	timer t;
	t.start();

	for (auto i = 0; i < vertex_array_size; i += 3)
	{
		for (auto j = 0; j < vert_files.size(); ++j)
		{
			yx[i][j] = vert_files[j][i + 0];
			yy[i][j] = vert_files[j][i + 1];
			yz[i][j] = vert_files[j][i + 2];
		}
	}
	std::cout << t.diff_sec_now() << "s" << std::endl;


	std::cout << "[Info] Computing polyfit ... ";
	t.start();

	for (auto i = 0; i < vertex_array_size; i += 3)
	{
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



	std::cout << "[Info] Computing error ... ";
	float minx = FLT_MAX, miny = FLT_MAX, minz = FLT_MAX, maxx = FLT_MIN, maxy = FLT_MIN, maxz = FLT_MIN;
	t.start();
	for (auto i = 0; i < n_verts; ++i)
	{
#if USE_LERP_APPROXIMATION
		err[i] = poly::compute_error_xyz_lerp(x, yx[i], yy[i], yz[i]);
#else
		err[i] = poly::compute_error_xyz(x, yx[i], yy[i], yz[i], coeff_x[i], coeff_y[i], coeff_z[i]);
#endif

		if (minx > err[i].x()) minx = err[i].x();
		if (miny > err[i].y()) miny = err[i].y();
		if (minz > err[i].z()) minz = err[i].z();
		if (maxx < err[i].x()) maxx = err[i].x();
		if (maxy < err[i].y()) maxy = err[i].y();
		if (maxz < err[i].z()) maxz = err[i].z();
	}
	std::cout << t.diff_sec_now() << "s" << std::endl;

	std::cout << std::fixed << "[Info] Min/Max Errors: " << minx << ' ' << miny << ' ' << minz << " : " << maxx << ' ' << maxy << ' ' << maxz << std::endl;

	for (auto i = 0; i < n_verts; ++i)
	{
		err_norm[i] = err[i].cast<float>();
		err_norm[i][0] = normalize<float>(minx, maxx, err[i].x());
		err_norm[i][1] = normalize<float>(miny, maxy, err[i].y());
		err_norm[i][2] = normalize<float>(minz, maxz, err[i].z());
	}

	std::cout << "[Info] Building error color map ... ";
	t.start();
	std::vector<float> err_color(vertex_array_size);
	for (auto i = 0; i < n_verts; ++i)
	{
		auto rgb = get_heat_map_rgb(err_norm[i].norm());
		//auto rgb = err_norm[i];

		err_color[i * 3 + 0] = rgb[0];
		err_color[i * 3 + 1] = rgb[1];
		err_color[i * 3 + 2] = rgb[2];
	}
	std::cout << t.diff_sec_now() << "s" << std::endl;


#if USE_LERP_APPROXIMATION
	const std::string out_error_rgb_filename = (dir / (prefix + ".lerp_rgb")).string();
#else
	const std::string out_error_rgb_filename = (dir / (prefix + ".poly_rgb")).string();
#endif
	//const std::string out_error_rgb_filename = "E:/Projects/MDP/faceblending/samples/models/Fig05.lerp_rgb";
	std::cout << "[Info] Saving error color file: " << out_error_rgb_filename << " ... ";
	t.start();
	vector_write(out_error_rgb_filename, err_color);
	std::cout << t.diff_sec_now() << "s" << std::endl;

	////const std::string out_error_filename = (dir / (prefix + ".poly_err")).string();
	//const std::string out_error_filename = "E:/Projects/MDP/faceblending/samples/models/Fig02.poly_err";
	//std::cout << "[Info] Saving error file: " << out_error_filename << std::endl;
	//vector_write(out_error_filename, err_norm.data()->data(), vertex_array_size);
	//std::cout << t.diff_sec_now() << "s" << std::endl;


    return EXIT_SUCCESS;
}