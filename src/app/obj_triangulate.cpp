#include <iostream>
#include "tinyobj.h"
#include <unordered_map>

#include <filesystem>
namespace fs = std::filesystem;

#define _USE_MATH_DEFINES
#include <cmath>


template <typename T>
static std::string
filename_append_before_extension(const std::string& filename_in,
	const T& append, const char separator = '_')
{
	auto ext_pos = filename_in.rfind('.', filename_in.length());
	if (ext_pos == std::string::npos)
		ext_pos = filename_in.length();
	std::stringstream filename_out;
	filename_out << filename_in.substr(0, ext_pos) << separator << append
		<< filename_in.substr(ext_pos, filename_in.length() - ext_pos);
	return filename_out.str();
}


void quad_to_tri(tinyobj::scene_t& scene)
{
	auto max_size = max(max(scene.attrib.vertices.size() / 3, scene.attrib.normals.size() / 3), scene.attrib.texcoords.size() / 2);

	for (auto& shape : scene.shapes)
	{
		// is not quad?
		if (!shape.mesh.indices.size() / shape.mesh.num_face_vertices.size() == 4)
			continue;

		tinyobj::mesh_t tri_mesh;

		for (auto i = 0; i < shape.mesh.indices.size(); i += 4)
		{
			const auto& idx_0 = shape.mesh.indices.at(i + 0);
			const auto& idx_1 = shape.mesh.indices.at(i + 1);
			const auto& idx_2 = shape.mesh.indices.at(i + 2);
			const auto& idx_3 = shape.mesh.indices.at(i + 3);

			//std::cout
			//	<< idx_0.vertex_index << ' ' << idx_0.texcoord_index << ' ' << idx_0.normal_index << std::endl
			//	<< idx_1.vertex_index << ' ' << idx_1.texcoord_index << ' ' << idx_1.normal_index << std::endl
			//	<< idx_2.vertex_index << ' ' << idx_2.texcoord_index << ' ' << idx_2.normal_index << std::endl
			//	<< idx_3.vertex_index << ' ' << idx_3.texcoord_index << ' ' << idx_3.normal_index << std::endl;

			tri_mesh.indices.push_back(idx_0);
			tri_mesh.indices.push_back(idx_1);
			tri_mesh.indices.push_back(idx_3);
			tri_mesh.num_face_vertices.push_back(3);

			tri_mesh.indices.push_back(idx_1);
			tri_mesh.indices.push_back(idx_2);
			tri_mesh.indices.push_back(idx_3);
			tri_mesh.num_face_vertices.push_back(3);
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
		for (auto it = fs::directory_iterator(input_path); it != fs::directory_iterator(); ++it)
		{
			tinyobj::scene_t scene;
			auto filename_in = it->path().string();
			if (!tinyobj::load(scene, filename_in))
				return EXIT_FAILURE;

			quad_to_tri(scene);

			const std::string filename_out = filename_append_before_extension(filename_in, "tri");
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
		
		quad_to_tri(scene);

		const std::string filename_out = filename_append_before_extension(filename_in, "tri");
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
