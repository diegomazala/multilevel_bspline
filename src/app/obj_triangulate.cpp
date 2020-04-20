#include <iostream>
#include "tinyobj.h"
#include <unordered_map>

#include <filesystem>
namespace fs = std::filesystem;

#define _USE_MATH_DEFINES
#include <cmath>



void quad_to_tri(tinyobj::scene_t& scene, bool invert_face = false)
{
	auto max_size = max(max(scene.attrib.vertices.size() / 3, scene.attrib.normals.size() / 3), scene.attrib.texcoords.size() / 2);

	for (auto& shape : scene.shapes)
	{
		// is not quad?
		if (!shape.mesh.indices.size() / shape.mesh.num_face_vertices.size() == 4)
			continue;

		tinyobj::mesh_t tri_mesh;
		if (!invert_face)
		{
			for (auto i = 0; i < shape.mesh.indices.size(); i += 4)
			{
				const auto& idx_0 = shape.mesh.indices.at(i + 0);
				const auto& idx_1 = shape.mesh.indices.at(i + 1);
				const auto& idx_2 = shape.mesh.indices.at(i + 2);
				const auto& idx_3 = shape.mesh.indices.at(i + 3);

				tri_mesh.indices.push_back(idx_0);
				tri_mesh.indices.push_back(idx_1);
				tri_mesh.indices.push_back(idx_3);
				tri_mesh.num_face_vertices.push_back(3);

				tri_mesh.indices.push_back(idx_1);
				tri_mesh.indices.push_back(idx_2);
				tri_mesh.indices.push_back(idx_3);
				tri_mesh.num_face_vertices.push_back(3);
			}
		}
		else
		{
			for (auto i = 0; i < shape.mesh.indices.size(); i += 4)
			{
				const auto& idx_0 = shape.mesh.indices.at(i + 0);
				const auto& idx_1 = shape.mesh.indices.at(i + 1);
				const auto& idx_2 = shape.mesh.indices.at(i + 2);
				const auto& idx_3 = shape.mesh.indices.at(i + 3);

				tri_mesh.indices.push_back(idx_0);
				tri_mesh.indices.push_back(idx_3);
				tri_mesh.indices.push_back(idx_1);
				tri_mesh.num_face_vertices.push_back(3);

				tri_mesh.indices.push_back(idx_1);
				tri_mesh.indices.push_back(idx_3);
				tri_mesh.indices.push_back(idx_2);
				tri_mesh.num_face_vertices.push_back(3);
			}
		}

		shape.mesh = tri_mesh;
	}
}


int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout
			<< std::fixed << std::endl
			<< "Usage            : ./<app.exe> <input_filename> <output_filename> <invert_face>" << std::endl
			<< "Default          : ./quad_to_tri.exe cube.quad cube.tri 0" << std::endl
			<< "Default          : ./quad_to_tri.exe cube.quad cube.tri 1" << std::endl
			<< std::endl;
		return EXIT_FAILURE;
	}

	bool invert_face = (argc > 3) && (atoi(argv[3]) == 1);
	const std::string info = invert_face ? "(inverted face) " : "(not inverted face) ";

	//
	// Initial parameters
	//
	const fs::path input_path = argv[1];
	fs::path output_path = (argc > 2) ? argv[2] : input_path;


	if (fs::is_directory(input_path))
	{
		const fs::path output_dir = ((argc > 2) ? argv[2] : (fs::is_directory(output_path) ? input_path : input_path.parent_path()));

		for (auto it = fs::directory_iterator(input_path); it != fs::directory_iterator(); ++it)
		{
			if (it->path().extension() != ".obj")
				continue;

			tinyobj::scene_t scene;
			auto filename_in = it->path().string();
			if (!tinyobj::load(scene, filename_in))
				return EXIT_FAILURE;

			quad_to_tri(scene, invert_face);

			const std::string filename_out = (output_dir / it->path().stem()).string() + "_tri.obj";
			std::cout << "[Info] Saving obj file : " << filename_out << std::endl;
			auto success_saving = tinyobj::save(scene, filename_out);
			if (!success_saving)
			{
				std::cerr << "[Error] Saving obj file : " << filename_out << std::endl;
				return EXIT_FAILURE;
			}
		}
	}
	else
	{
		tinyobj::scene_t scene;
		auto filename_in = input_path.string();
		if (!tinyobj::load(scene, filename_in))
			return EXIT_FAILURE;
		
		quad_to_tri(scene, invert_face);

		std::string filename_out;

		if (fs::is_directory(output_path))
		{
			output_path /= input_path.stem();
			filename_out = output_path.string() + "_tri.obj";
		}
		else
		{
			filename_out = output_path.string();
		}
		

		std::cout << "[Info] Saving obj file : " << filename_out << std::endl;
		auto success_saving = tinyobj::save(scene, filename_out);
		if (!success_saving)
		{
			std::cerr << "[Error] Saving obj file : " << filename_out << std::endl;
			return EXIT_FAILURE;
		}
	}


	return EXIT_SUCCESS;
}
