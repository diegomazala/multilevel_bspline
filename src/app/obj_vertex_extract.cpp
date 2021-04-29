#include <iostream>
#include <iomanip>
#include "tinyobj.h"

#include <filesystem>
namespace fs = std::filesystem;

#define _USE_MATH_DEFINES
#include <cmath>


bool vert_extract(tinyobj::scene_t& scene, int index)
{
	if (index < scene.attrib.vertices.size() && index > -1)
	{
		std::string number_str = std::to_string(index);
		std::string append_str = std::string(6 - number_str.length(), '0') + number_str;

		std::cout
			<< std::fixed
			<< std::setw(6)
			//<< append_str << " : "
			<< scene.attrib.vertices[index * 3 + 0] << ' '
			<< scene.attrib.vertices[index * 3 + 1] << ' '
			<< scene.attrib.vertices[index * 3 + 2] << std::endl;

		return true;
	}
	std::cout << "Index out of range: " << index << std::endl;
	return false;
}


int main(int argc, char* argv[]) 
{ 
	if (argc < 3)
	{
        std::cerr << "Usage: app <filename_in> <index, index, index, ...>\n"
		<< "Usage: app <directory> <prefix> <index, index, index, ...>\n";
        return EXIT_FAILURE;
	}


	std::vector<std::string> files;
	const fs::path dir{ argv[1] };
	int argc_start_index = 0;

	if (!fs::is_directory(dir))
	{
		files.push_back(argv[1]);
		argc_start_index = 2;
	}
	else
	{
		argc_start_index = 3;
		const std::string prefix{ argv[2] };
		//
		// Get the files to be loaded
		//
		for (const auto& entry : fs::directory_iterator(dir))
		{
			auto pos_prefix = (entry.path().has_stem()) ? entry.path().stem().string().find(prefix) : std::string::npos;
			if (pos_prefix != std::string::npos && entry.path().extension() == ".obj")
			{
				files.push_back(entry.path().string());
			}
		}
	}

	std::sort(files.begin(), files.end());

	std::cout << "Number of files: " << files.size() << std::endl;

	for (const auto& f : files)
	{
		std::vector<int> indices;
		for (auto i = argc_start_index; i < argc; ++i)
			indices.push_back(atoi(argv[i]));

		//std::cout << "[Info] Loading obj file " << f << std::endl;

		tinyobj::scene_t scene;
		auto success_loading = tinyobj::load(scene, f);

		if (!success_loading)
		{
			std::cerr << "[Error] Loading obj file : " << f << std::endl;
			return EXIT_FAILURE;
		}

		for (auto i : indices)
		{
			vert_extract(scene, i);
		}
	}


	return EXIT_SUCCESS;
}
	
