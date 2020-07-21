#include <iostream>
#include "tinyobj.h"

#define _USE_MATH_DEFINES
#include <cmath>

int main(int argc, char* argv[]) 
{ 
	if (argc < 4)
	{
        std::cerr << "Usage: app <filename_in> <filename_out> <group_names [...]>\n";
        return EXIT_FAILURE;
	}

	const std::string filename_in = argv[1];
	const std::string filename_out = argv[2];

	std::vector<std::string> group_names;
	for (auto i = 2; i < argc; ++i)
	{
		group_names.push_back(argv[i]);
		std::cout << "[Info] Group to be removed: " << argv[i] << std::endl;
	}
	

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
	tinyobj::remove_groups(scene, group_names);
	tinyobj::garbage_collect(scene_out, scene);

	tinyobj::print_info(scene_out);

	std::cout << "[Info] Saving obj file : " << filename_out << std::endl;

	auto success_saving = tinyobj::save(scene_out, filename_out, "default");

	if (!success_saving)
	{
		std::cerr << "[Error] Saving obj file : " << filename_out << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

