#include <iostream>
#include "tinyobj.h"

#define _USE_MATH_DEFINES
#include <cmath>

int main(int argc, char* argv[]) 
{ 
	if (argc < 3)
	{
        std::cerr << "Usage: app <filename_in> <filename_out>\n";
        return EXIT_FAILURE;
	}

	const std::string filename_in = argv[1];
	const std::string filename_out = argv[2];

	std::cout << "[Info] Loading obj file " << filename_in << std::endl;
	tinyobj::scene_t scene;
	auto success_loading = tinyobj::load(scene, filename_in);

	if (!success_loading)
		return EXIT_FAILURE;

	tinyobj::print_info(scene);

	tinyobj::scene_t scene_out;
	tinyobj::garbage_collect(scene_out, scene);

	tinyobj::print_info(scene_out);
	
	std::cout << "[Info] Saving obj file : " << filename_out << std::endl;
	auto success_saving = tinyobj::save(scene_out, filename_out);

	if (!success_saving)
	{
		std::cerr << "[Error] Saving obj file : " << filename_out << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "---- x ----" << std::endl;

	return EXIT_SUCCESS;
}

