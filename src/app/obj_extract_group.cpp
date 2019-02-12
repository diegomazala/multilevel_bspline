#include <iostream>
#include "tinyobj.h"

#define _USE_MATH_DEFINES
#include <cmath>

int main(int argc, char* argv[]) 
{ 
	if (argc < 4)
	{
        std::cerr << "Usage: app <filename_in> <group_name> <filename_out>\n";
        return EXIT_FAILURE;
	}

	const std::string filename_in = argv[1];
	const std::string group_name = argv[2];
	const std::string filename_out = argv[3];

	std::cout << "[Info] Loading obj file " << filename_in << std::endl;

	tinyobj::scene_t scene;
	auto success_loading = tinyobj::load(scene, filename_in);

	if (!success_loading)
		return EXIT_FAILURE;
		
	tinyobj::print_info(scene);


	tinyobj::scene_t scene_out;
	tinyobj::extract_group(scene_out, group_name, scene);

	std::cout << "[Info] Saving obj file : " << filename_out << std::endl;

	auto success_saving = tinyobj::save(scene_out, filename_out);

	if (!success_saving)
		return EXIT_FAILURE;


	return EXIT_SUCCESS;
}

