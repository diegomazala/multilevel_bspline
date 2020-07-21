#include <iostream>
#include "tinyobj.h"

#define _USE_MATH_DEFINES
#include <cmath>

int main(int argc, char* argv[]) 
{ 
	if (argc < 3)
	{
        std::cerr << "Usage: app <filename_in> <filename_out> <group_name>\n";
        return EXIT_FAILURE;
	}

	const std::string filename_in = argv[1];
	const std::string filename_out = argv[2];
	

	std::cout << "[Info] Loading obj file " << filename_in << std::endl;

	tinyobj::scene_t scene;
	auto success_loading = tinyobj::load(scene, filename_in);

	if (!success_loading)
	{
		std::cerr << "[Error] Loading obj file : " << filename_in << std::endl;
		return EXIT_FAILURE;
	}

		
	tinyobj::print_info(scene);


	tinyobj::scene_t scene_out;

	if (argc > 3)
	{
		const std::string group_name = argv[3];
		std::cout << "[Info] Extracting group : " << group_name << std::endl;
		tinyobj::extract_group(scene_out, group_name, scene);
	}
	else
	{
		std::string group_name = scene.shapes[0].name;
		size_t max_num_polys = scene.shapes[0].mesh.indices.size() / scene.shapes[0].mesh.num_face_vertices[0];
		for (auto i = 1; i < scene.shapes.size(); ++i)
		{
			auto num_polys = scene.shapes[i].mesh.indices.size() / scene.shapes[i].mesh.num_face_vertices[0];
			if (num_polys > max_num_polys)
			{
				max_num_polys = num_polys;
				group_name = scene.shapes[i].name;
			}
		}
		std::cout << "[Info] Extracting group : " << group_name << std::endl;
		tinyobj::extract_group(scene_out, group_name, scene);
	}

	std::cout << "[Info] Saving obj file : " << filename_out << std::endl;

	auto success_saving = tinyobj::save(scene_out, filename_out);

	if (!success_saving)
	{
		std::cerr << "[Error] Saving obj file : " << filename_out << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

