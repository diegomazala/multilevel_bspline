#include <iostream>
#include <iomanip>
#include "tinyobj.h"

#include <filesystem>
namespace fs = std::filesystem;

#define _USE_MATH_DEFINES
#include <cmath>

template<typename T>
std::string to_string_left_padded(T v, int left_count, char left_char = '0')
{
	const std::string number_str = std::to_string(v);
	return std::string(left_count - number_str.length(), left_char) + number_str;
}

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



void vert_extract_to_file(const std::vector<std::string>& files, const std::string& output_dir, std::size_t vert_begin, std::size_t vert_end)
{
	const std::size_t batch_size = 256;
	auto n_verts = vert_end - vert_begin;

	std::vector<std::ofstream> out_files(n_verts);
	{
		for (auto i = 0; i < n_verts; ++i)
		{
			std::string fname = output_dir + "/v" + to_string_left_padded(i + vert_begin, 6, '0') + ".txt";
			try
			{
				out_files[i] = std::ofstream(fname, std::ios::out);
			}
			catch (const std::exception& ex)
			{
				std::cout << ex.what() << std::endl;
			}

			if (!out_files[i].is_open())
			{
				std::cout << "\n[Error] Could not open file to write: " << fname << std::endl;
			}
			else
			{
				std::cout << "\r[Info] File open: " << float(i * (n_verts - 1)) / ((n_verts - 1) * (n_verts - 1)) * 100 << " %";
			}
		}
	}

	std::cout << std::endl;

	std::size_t f = 0;
	while (f < files.size())
	{
		std::cout << "--------------------------------------- Loading " << f << std::endl;
		std::vector<tinyobj::scene_t> scenes(batch_size);
		std::vector<bool> success_load(batch_size);


		auto cur_batch_size = 0;
		for (auto b = 0; b < batch_size; ++b)
		{
			std::cout << '[' << vert_begin << " - " << vert_end << "]  " << f << " : " << b << " :  Loading: " << files[f] << std::endl;

			success_load[b] = tinyobj::load(scenes[b], files[f]);
			if (!success_load[b])
			{
				std::cout << "[Error] Could not load " << files[f] << std::endl;
				break;
			}

			cur_batch_size = b + 1;

			if (++f == files.size())
				break;
		}

		std::cout << "[Info] Writing batch to files" << std::endl;

		for (auto i = 0; i < n_verts; ++i)
		{
			for (auto b = 0; b < cur_batch_size; ++b)
			{
				out_files[i] << std::fixed
					<< scenes[b].attrib.vertices[(i + vert_begin) * 3 + 0] << ' '
					<< scenes[b].attrib.vertices[(i + vert_begin) * 3 + 1] << ' '
					<< scenes[b].attrib.vertices[(i + vert_begin) * 3 + 2] << std::endl;
			}
		}


		scenes.clear();
		success_load.clear();
	}

	for (auto i = 0; i < n_verts; ++i)
		out_files[i].close();
}





void vert_extract_to_file(const std::vector<std::string>& files, const std::string& output_dir)
{
	std::size_t n_verts = 0;
	{
		tinyobj::scene_t scene;
		if (tinyobj::load(scene, files[0]))
			n_verts = scene.attrib.vertices.size() / 3;
	}

	const std::size_t vertex_batch_size = 256;
	std::size_t last_vertex_batch = 0;
	while (last_vertex_batch < n_verts)
	{
		std::cout << "\n========================================== Vertex_batch : " << last_vertex_batch << std::endl;
		vert_extract_to_file(files, output_dir, last_vertex_batch, min(last_vertex_batch + vertex_batch_size, n_verts));

		last_vertex_batch += vertex_batch_size;
	}

}



int main(int argc, char* argv[]) 
{ 
	if (argc < 3)
	{
        std::cerr << "Usage: app <filename_in> <index, index, index, ...>\n"
		<< "Usage: app <directory> <prefix> <index, index, index, ...>\n"
		<< "Usage: app <directory> <prefix> <output_dir>\n";
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

#if 1

	vert_extract_to_file(files, fs::path(argv[3]).string());
	//vert_extract_to_file({ files.begin(), files.begin() + 40 }, fs::path(argv[3]).string());

#else

	std::vector<int> indices;
	for (auto i = argc_start_index; i < argc; ++i)
		indices.push_back(atoi(argv[i]));

	for (const auto& f : files)
	{
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

#endif


	return EXIT_SUCCESS;
}
	
