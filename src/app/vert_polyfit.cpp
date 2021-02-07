#include <Eigen/QR>
#include <vector>
#include <filesystem>
#include "timer.h"
#include "vector_read_write_binary.h"

namespace fs = std::filesystem;


template<typename T> T lerp(T begin, T end, T value)
{
	return (value - begin) / (end - begin);
}

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

// app.exe D:/Projects/MDP/Meshes/AllFaces/Laplacian_All_Levels/ Fig02_lap_

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout
			<< "Usage: app <path> <file_prefix> \n"
			<< "Usage: ./vert_polyfit.exe C:/models/ Fig02\n"
			<< "Usage: ./vert_polyfit.exe E:/Projects/MDP/faceblending/samples/models fig02_lap_\n";
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
		std::cout << "[Info] Loading obj file " << files[i] << std::endl;
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

	std::vector<float> x{ 0, 50, 100, 250, 500, 1000, 2500, 5000, 7500, 10000, 12500, 15000, 17500, 20000 };
	//std::vector<float> x{ 0, 10000, 20000 };

	if (x.size() != vert_files.size())
	{
		std::cerr << "Input x array size does not match to number of vert files. ("
				<< x.size() << " != " << vert_files.size() << "). Abort" << std::endl;
		return EXIT_FAILURE;
	}

	
	
	std::vector<std::vector<double>> xx(vertex_array_size);
	std::vector<std::vector<double>> yx(vertex_array_size);
	std::vector<std::vector<double>> yy(vertex_array_size);
	std::vector<std::vector<double>> yz(vertex_array_size);
	std::vector<std::vector<double>> coeff_x(vertex_array_size);
	std::vector<std::vector<double>> coeff_y(vertex_array_size);
	std::vector<std::vector<double>> coeff_z(vertex_array_size);
	
	for (auto i = 0; i < vertex_array_size; i+=3)
	{
		for (auto j = 0; j < vert_files.size(); ++j)
		{
			xx[i].push_back( lerp(x.front(), x.back(), x[j]) );
			yx[i].push_back(vert_files[j][i + 0]);
			yy[i].push_back(vert_files[j][i + 1]);
			yz[i].push_back(vert_files[j][i + 2]);
		}
	}

	std::cout << "Computing polyfit ..." << std::endl;
	timer t;
	for (auto i = 0; i < vertex_array_size; i += 3)
	{
		polyfit(xx[i], yx[i], coeff_x[i], 3);
		polyfit(xx[i], yy[i], coeff_y[i], 3);
		polyfit(xx[i], yz[i], coeff_z[i], 3);
	}
	t.stop();
	std::cout << "Polyfit time elapsed: " << t.diff_sec() << " seconds " << std::endl;

#if 0
	for (auto j = 0; j < vert_files.size(); ++j)
	{
		std::cout << xx[0][j] << ", [" << yx[0][j] << ", " << yy[0][j] << ", " << yz[0][j] << std::endl;
	}

	float xv = 0.25;
	std::cout << "---------------------------------" << std::endl;
	std::cout << " 0.25 -> " << 
		coeff_x[0][3] * std::pow(xv, 3) +
		coeff_x[0][2] * std::pow(xv, 2) +
		coeff_x[0][1] * xv + 
		coeff_x[0][0] << std::endl;
	std::cout << " 0.25 -> " << 
		coeff_y[0][3] * std::pow(xv, 3) +
		coeff_y[0][2] * std::pow(xv, 2) +
		coeff_y[0][1] * xv + 
		coeff_y[0][0] << std::endl;
	std::cout << " 0.25 -> " << 
		coeff_z[0][3] * std::pow(xv, 3) +
		coeff_z[0][2] * std::pow(xv, 2) +
		coeff_z[0][1] * xv + 
		coeff_z[0][0] << std::endl;
#endif

	//
	// Combining the coeff vector into a single vector
	//
	std::vector<float> poly;
	for (const auto& v : coeff_x)
		std::copy(v.begin(), v.end(), std::back_inserter(poly));
	for (const auto& v : coeff_y)
		std::copy(v.begin(), v.end(), std::back_inserter(poly));
	for (const auto& v : coeff_z)
		std::copy(v.begin(), v.end(), std::back_inserter(poly));


	const std::string out_filename = (dir / (prefix + ".poly")).string();
	std::cout << "[Info] Saving output file: " << out_filename << std::endl;
	vector_write(out_filename, poly);


    return EXIT_SUCCESS;
}