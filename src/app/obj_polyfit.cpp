#include <Eigen/QR>
#include <vector>
#include <filesystem>
#include "timer.h"
#include "tinyobj.h"
#include "vector_read_write_binary.h"

namespace fs = std::filesystem;

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

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout
			<< "Usage: app <path> <file_prefix> \n"
			<< "Usage: ./obj_polifit.exe C:/models/ Fig02\n";
		return EXIT_FAILURE;
	}

	const std::string prefix{ argv[2] };
	const fs::path dir{ argv[1] };

	std::vector<std::string> files;

	//
	// Get the files to be loaded
	//
	for (const auto& entry : fs::directory_iterator(dir)) 
	{
		auto pos_prefix = (entry.path().has_stem()) ? entry.path().stem().string().find(prefix) : std::string::npos;
		if (pos_prefix != std::string::npos && entry.path().extension() == ".obj")
		{
			files.push_back(entry.path().string());
			//std::cout << "--> " << entry.path().stem().string() << '\n';
		}
	}

	std::vector<tinyobj::scene_t> obj_scenes(files.size());


	//
	// Load and process the first file
	//
	std::cout << "[Info] Loading obj file " << files[0] << std::endl;
	if (!tinyobj::load(obj_scenes[0], files[0]))
	{
		std::cerr 
			<< "[Error] Loading obj file : " << files[0] << std::endl
			<< "Abort" << std::endl;
		return EXIT_FAILURE;
	}

	const auto vertex_array_size = obj_scenes[0].attrib.vertices.size();

	//
	// Load and process the others files
	//
	for (auto i = 1; i < files.size(); ++i)
	{
		std::cout << "[Info] Loading obj file " << files[i] << std::endl;
		if (!tinyobj::load(obj_scenes[i], files[i]))
		{
			std::cerr 
				<< "[Error] Loading obj file : " << files[i] << std::endl
				<< "Abort" << std::endl;
			return EXIT_FAILURE;
		}

		if (obj_scenes[i].attrib.vertices.size() != vertex_array_size)
		{
			std::cerr << "[Error] Vertex count does not match. " 
				<< obj_scenes[i].attrib.vertices.size() << " != " 
				<< vertex_array_size << " Abort " << std::endl;
			return EXIT_FAILURE;
		}

	}



	std::vector<float> ref{ 0, 50, 100, 250, 500, 1000, 2500, 5000, 7500, 10000, 12500, 15000, 17500, 20000 };
	
	std::vector<float> poly_x, poly_y, poly_z;

	for (auto i = 0; i < vertex_array_size; i+=3)
	{
		std::vector<float> x, y, z, coeff_x, coeff_y, coeff_z;

		for (tinyobj::scene_t& scene : obj_scenes)
		{
			x.push_back(scene.attrib.vertices[i + 0]);
			y.push_back(scene.attrib.vertices[i + 1]);
			z.push_back(scene.attrib.vertices[i + 2]);
		}

		if (x.size() != ref.size())
		{
			std::cerr << "[Error] Array size does not match. Abort " << std::endl;
			return EXIT_FAILURE;
		}

		polyfit(x, ref, coeff_x, 3);
		polyfit(y, ref, coeff_y, 3);
		polyfit(z, ref, coeff_z, 3);

		for (auto j = 0; j < coeff_x.size(); ++j)
		{
			poly_x.push_back(coeff_x[j]);
			poly_y.push_back(coeff_y[j]);
			poly_z.push_back(coeff_z[j]);
		}
	}
	const std::string out_filename = (dir / (prefix + ".poly")).string();
	std::cout << "[Info] Saving output file: " << out_filename << std::endl;

	std::vector<float> poly_xyz;
	std::copy(poly_x.begin(), poly_x.end(), std::back_inserter(poly_xyz));
	std::copy(poly_y.begin(), poly_y.end(), std::back_inserter(poly_xyz));
	std::copy(poly_z.begin(), poly_z.end(), std::back_inserter(poly_xyz));

	//vector_write((dir / (prefix + "x.poly")).string(), poly_x);
	//vector_write((dir / (prefix + "y.poly")).string(), poly_y);
	//vector_write((dir / (prefix + "z.poly")).string(), poly_z);

	vector_write(out_filename, poly_xyz);

    return 0;
}