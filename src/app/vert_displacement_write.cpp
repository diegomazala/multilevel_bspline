#include <iostream>
#include "tinyobj.h"
#include "vector_read_write_binary.h"
#include <thread>

#include <filesystem>
namespace fs = std::filesystem;

#define _USE_MATH_DEFINES
#include <cmath>

static uint8_t get_dist_to_color_range(float value, float min_dist = 0.f, float max_dist = 3.0f)
{
	return static_cast<uint8_t>( ( (value - min_dist) / (max_dist - min_dist) ) * max_dist);
}

int obj_displacement_write(const std::string& filename_inA, const std::string& filename_inB, const std::string& filename_out)
{
	tinyobj::scene_t sceneA, sceneB;

	//
	// Load the meshes
	//
	{
		bool success_loadingA, success_loadingB;

		std::cout << "[Info] Loading obj file " << filename_inA << std::endl;
		std::cout << "[Info] Loading obj file " << filename_inB << std::endl;

		std::vector<std::thread> thread;
		{
			std::cout << "Loading: " << filename_inA << std::endl;
			thread.push_back(std::thread([&] {
				success_loadingA = tinyobj::load(sceneA, filename_inA);
			}));

			std::cout << "Loading: " << filename_inB << std::endl;
			thread.push_back(std::thread([&] {
				success_loadingB = tinyobj::load(sceneB, filename_inB);
			}));
		}
		for (auto& t : thread)
			t.join();

		if (!success_loadingA)
		{
			std::cerr << "[Error] Loading obj file : " << filename_inA << std::endl;
			return EXIT_FAILURE;
		}

		if (!success_loadingB)
		{
			std::cerr << "[Error] Loading obj file : " << filename_inB << std::endl;
			return EXIT_FAILURE;
		}

		tinyobj::print_info(sceneA);
		tinyobj::print_info(sceneB);

		if (sceneA.attrib.vertices.size() != sceneB.attrib.vertices.size())
		{
			std::cerr << "[Error] The number of vertices must match. "
				<< sceneA.attrib.vertices.size() << " != " << sceneB.attrib.vertices.size() << std::endl;
			return EXIT_FAILURE;
		}
	}

	auto max_size = max(sceneA.attrib.vertices.size() / 3, sceneA.attrib.texcoords.size() / 2);
	std::vector<tinyobj::real_t> displacement;
	displacement.reserve(max_size * 3);


	for (auto& shape : sceneA.shapes)
	{
		for (const auto& idx : shape.mesh.indices)
		{
			const auto vi = idx.vertex_index * 3;

			for (auto i = 0; i < 3; ++i)
			{
				displacement.push_back(sceneB.attrib.vertices[vi + i] - sceneA.attrib.vertices[vi + i]);
			}
		}
	}

	//for (int i = 0; i < displacement.size(); i += 3)
	//{
	//	std::cout << displacement[i + 0] << ' ' << displacement[i + 1] << ' ' << displacement[i + 2] << std::endl;
	//}

	vector_write(filename_out, displacement);

	std::cout << "Saved displacement file : " << filename_out << std::endl;

	return EXIT_SUCCESS;
}


int vert_displacement_write(const std::string& filename_inA, const std::string& filename_inB, const std::string& filename_out)
{
	std::vector<float> vertsA;
	vector_read(filename_inA, vertsA);

	std::vector<float> vertsB;
	vector_read(filename_inB, vertsB);

	if (vertsA.size() != vertsB.size())
	{
		std::cerr << "[Error] The number of vertices must match. "
			<< vertsA.size() << " != " << vertsB.size() << std::endl;
		return EXIT_FAILURE;
	}

	std::vector<float> displacement(vertsA.size());

	for (auto i = 0; i < vertsA.size(); ++i)
	{
		displacement[i] = vertsB[i] - vertsA[i];
	}

	for (int i = 0; i < displacement.size(); i += 3)
	{
		std::cout << displacement[i + 0] << ' ' << displacement[i + 1] << ' ' << displacement[i + 2] << std::endl;
	}


	vector_write(filename_out, displacement);
	std::cout << "Saved displacement file : " << filename_out << std::endl;

	return EXIT_SUCCESS;
}


int main(int argc, char* argv[]) 
{ 
	if (argc < 3)
	{
        std::cerr << "Usage: app <mesh_filename_inA> <mesh_filename_inB> <displacement_filename>\n";
        return EXIT_FAILURE;
	}

	const std::string filename_inA = argv[1];
	const std::string filename_inB = argv[2];
	const std::string filename_out = argv[3];

	auto extension = filename_inA.substr(filename_inA.size() - 4, 4);
	if (extension == "vert")
	{
		return vert_displacement_write(filename_inA, filename_inB, filename_out);
	}
	else if (extension == ".obj")
	{
		return obj_displacement_write(filename_inA, filename_inB, filename_out);
	}
	else
	{
		std::cerr << "Error: Unrecognized file format. Support only to .vert and .obj\n";
		return EXIT_FAILURE;
	}
}

