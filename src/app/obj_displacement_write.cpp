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


static auto get_rgb_color_ramp(float value)
{
	int aR = 0;   int aG = 0; int aB = 255;  // RGB for our 1st color (blue in this case).
	int bR = 255; int bG = 0; int bB = 0;    // RGB for our 2nd color (red in this case).

	float rgb[3] =
	{
		(float)(bR - aR) * value + aR,    // Evaluated as -255*value + 255.
		(float)(bG - aG) * value + aG,    // Evaluates as 0.
		(float)(bB - aB) * value + aB     // Evaluates as 255*value + 0.
	};

	return rgb;
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


	vector_write(filename_out, displacement);

	std::cout << "Saved displacement file : " << filename_out << std::endl;

	return EXIT_SUCCESS;
}

