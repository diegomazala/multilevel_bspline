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


	Eigen::VectorXd _x(14), _xn(14);
	_x << 0, 50, 100, 250, 500, 1000, 2500, 5000, 7500, 10000, 12500, 15000, 17500, 20000;
	
	for (auto j = 0; j < _x.size(); ++j)
	{
		_xn[j] = lerp(_x[0], _x[_x.rows() - 1], _x[j]);
	}

	if (_x.size() != vert_files.size())
	{
		std::cerr << "Input x array size does not match to number of vert files. ("
			<< _x.size() << " != " << vert_files.size() << "). Abort" << std::endl;
		return EXIT_FAILURE;
	}



	const int order = 3;
	std::vector<Eigen::Matrix<double, 14, 1>> _xx(vertex_array_size);
	std::vector<Eigen::Matrix<double, 14, 1>> _yx(vertex_array_size);
	std::vector<Eigen::Matrix<double, 14, 1>> _yy(vertex_array_size);
	std::vector<Eigen::Matrix<double, 14, 1>> _yz(vertex_array_size);
	std::vector<Eigen::Matrix<double, Eigen::Dynamic, 1>> _coeff_x(vertex_array_size);	// size = order + 1
	std::vector<Eigen::Matrix<double, Eigen::Dynamic, 1>> _coeff_y(vertex_array_size);
	std::vector<Eigen::Matrix<double, Eigen::Dynamic, 1>> _coeff_z(vertex_array_size);


	std::cout << "[Info] Building matrices ..." << std::endl;

	for (auto i = 0; i < vertex_array_size; i += 3)
	{
		for (auto j = 0; j < vert_files.size(); ++j)
		{
			_xx[i][j] = lerp(_x[0], _x[_x.rows() - 1], _x[j]);
			_yx[i][j] = vert_files[j][i + 0];
			_yy[i][j] = vert_files[j][i + 1];
			_yz[i][j] = vert_files[j][i + 2];
		}
	}



	std::cout << "Computing polyfit ..." << std::endl;
	timer t;



	for (auto i = 0; i < vertex_array_size; i += 3)
	{
		_coeff_x[i] = poly::fit<double>(_xn, _yx[i], order);
		_coeff_y[i] = poly::fit<double>(_xn, _yy[i], order);
		_coeff_z[i] = poly::fit<double>(_xn, _yz[i], order);
	}
	
	
	t.stop();
	std::cout << "Polyfit time elapsed: " << t.diff_sec() << " seconds " << std::endl;

	


	//
	// Combining the coeff vector into a single vector
	//
	std::vector<float> poly;
	for (const Eigen::Matrix<double, Eigen::Dynamic, 1>& v : _coeff_x)
		for (const auto& vv : v)
			poly.push_back(vv);
	for (const Eigen::Matrix<double, Eigen::Dynamic, 1>&v : _coeff_y)
		for (const auto& vv : v)
			poly.push_back(vv);
	for (const Eigen::Matrix<double, Eigen::Dynamic, 1>&v : _coeff_z)
		for (const auto& vv : v)
			poly.push_back(vv);
	
	

	const std::string out_filename = (dir / (prefix + ".poly")).string();
	std::cout << "[Info] Saving output file: " << out_filename << std::endl;
	vector_write(out_filename, poly);


    return EXIT_SUCCESS;
}