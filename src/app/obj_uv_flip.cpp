#include <iostream>
#include "tinyobj.h"

int main(int argc, char* argv[]) 
{ 
	if (argc < 4)
	{
        std::cerr << "Usage: app <filename_in> <filename_out> <flip_u> <flip_v>\n app.exe file_in.obj file_out.obj 0 1";
        return EXIT_FAILURE;
	}

	const std::string filename_in = argv[1];
	const std::string filename_out = argv[2];
	const bool flip_u = (bool)atoi(argv[3]);
	const bool flip_v = (bool)atoi(argv[4]);

	std::cout << "[Info] Loading obj file " << filename_in << std::endl;
	tinyobj::scene_t scene;

	std::cout << "Loading: " << filename_in << std::endl;
	if (!tinyobj::load(scene, filename_in))
	{
		std::cerr << "[Error] Loading obj file : " << filename_in << std::endl;
		return EXIT_FAILURE;
	}

	for (auto i = 0; i < scene.attrib.texcoords.size(); i += 2)
	{
		if (flip_u)
			scene.attrib.texcoords[i] = 1.0f - scene.attrib.texcoords[i];
		if (flip_v)
			scene.attrib.texcoords[i+1] = 1.0f - scene.attrib.texcoords[i+1];
	}

	std::cout << "[Info] Saving obj file : " << filename_out << std::endl;

	if (!tinyobj::save(scene, filename_out))
	{
		std::cerr << "[Error] Saving obj file : " << filename_out << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "---- x ----" << std::endl;

	return EXIT_SUCCESS;
}

