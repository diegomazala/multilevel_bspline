#include <iostream>
#include <iomanip> 
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include "tinyply.h"
#include "tinyobj.h"
#include "vector_read_write_binary.h"
#include <filesystem>
namespace fs = std::filesystem;


void join_files(const std::string& vert_file, const std::string& face_file)
{
	std::vector<float> verts;
	vector_read(vert_file, verts);

	std::vector<uint32_t> faces;
	vector_read(face_file, faces);
	int poly = (faces.size() % 4 == 0) ? 4 : 3;

	std::filebuf fb;
	fb.open(fs::path(vert_file).replace_extension(".ply").string(), std::ios::out | std::ios::binary);
	std::ostream outputStream(&fb);

	tinyply::PlyFile ply_out_file;

	ply_out_file.add_properties_to_element("vertex", { "x", "y", "z" }, verts);
	if (!faces.empty())
		ply_out_file.add_properties_to_element("face", { "vertex_indices" }, faces, poly, tinyply::PlyProperty::Type::UINT8);
	
	ply_out_file.write(outputStream, true);
	fb.close();
}


void split_obj(tinyobj::scene_t& scene)
{
	auto max_size = max(scene.attrib.vertices.size() / 3, scene.attrib.texcoords.size() / 2);

	std::vector<tinyobj::real_t> vertices, texcoords, normals;
	vertices.reserve(max_size * 3);
	normals.reserve(max_size * 3);
	texcoords.reserve(max_size * 2);
	
	std::vector<uint32_t> indices;

	for (auto& shape : scene.shapes)
	{
		for (const auto& idx : shape.mesh.indices)
		{
			vertices.push_back(scene.attrib.vertices[idx.vertex_index * 3 + 0]);
			vertices.push_back(scene.attrib.vertices[idx.vertex_index * 3 + 1]);
			vertices.push_back(scene.attrib.vertices[idx.vertex_index * 3 + 2]);

			normals.push_back(scene.attrib.normals[idx.normal_index * 3 + 0]);
			normals.push_back(scene.attrib.normals[idx.normal_index * 3 + 1]);
			normals.push_back(scene.attrib.normals[idx.normal_index * 3 + 2]);

			texcoords.push_back(scene.attrib.texcoords[idx.texcoord_index * 2 + 0]);
			texcoords.push_back(scene.attrib.texcoords[idx.texcoord_index * 2 + 1]);

			indices.push_back(indices.size());
		}
	}

	std::cout
		<< "File Info: " << std::endl
		<< '\t' << vertices.size() / 3 << " vertices\n"
		<< '\t' << normals.size() / 3 << " normals\n"
		<< '\t' << texcoords.size() / 2 << " texcoords\n"
		<< '\t' << indices.size() << " indices\n";

}



int split_file(const fs::path& input_filename, const std::string& input_poly_filename, const fs::path output_dir)
{
	const std::string file_extension = fs::path(input_filename).extension().string();

	if (input_filename.extension() == ".ply")
	{
		std::cout << "PLY File Format: " << input_filename.filename() << std::endl;

		std::vector<float> verts, norms, uv;
		std::vector<uint32_t> faces;

		std::ifstream ss(input_filename.string(), std::ios::binary);
		tinyply::PlyFile file(ss);

		uint32_t vertex_count = file.request_properties_from_element("vertex", { "x", "y", "z" }, verts);
		uint32_t normal_count = file.request_properties_from_element("vertex", { "nx", "ny", "nz" }, norms);
		uint32_t uv_count = file.request_properties_from_element("vertex", { "u", "v" }, uv);
		uint32_t face_count = file.request_properties_from_element("face", { "vertex_indices" }, faces);
		file.read(ss);

		std::cout
			<< "File Info: " << std::endl
			<< '\t' << vertex_count/3 << " vertices\n"
			<< '\t' << normal_count/3 << " normals\n"
			<< '\t' << uv_count/2 << " texcoords\n"
			<< '\t' << face_count << " faces\n\n";

		if (vertex_count > 0 && face_count > 0)
		{
			auto out_filename = output_dir / input_filename.stem();
			vector_write(out_filename.replace_extension(".vert").string(), verts);
			std::string extension = ((faces.size() % 3 == 0) ? (".tri") : (".quad"));
			vector_write(out_filename.replace_extension(extension).string(), faces);
			if (norms.size() > 0)
				vector_write(out_filename.replace_extension(".nor").string(), norms);
			if (uv.size() > 0)
				vector_write(out_filename.replace_extension(".uv").string(), uv);
		}
		else
		{
			std::cerr << "[Error] Could not load ply file : " << input_filename.string() << std::endl;
			return EXIT_FAILURE;
		}
	}
	else if (input_filename.extension() == ".obj")
	{
		std::cout << "OBJ File Format: " << input_filename.filename() << std::endl;

		tinyobj::scene_t obj_load;
		if (tinyobj::load(obj_load, input_filename.string()))
		{
#if 1
			tinyobj::scene_t& scene = obj_load;
#else
			tinyobj::scene_t scene = obj_load;
			tinyobj::garbage_collect(scene, obj_load);
#endif
			std::vector<float> polyfit;
			vector_read(input_poly_filename, polyfit);

			auto max_size = max(scene.attrib.vertices.size() / 3, scene.attrib.texcoords.size() / 2);

			std::vector<tinyobj::real_t> vertices, texcoords, normals, poly;
			vertices.reserve(max_size * 3);
			normals.reserve(max_size * 3);
			texcoords.reserve(max_size * 2);
			poly.reserve(max_size * 4);

			std::vector<uint32_t> indices;

			for (auto& shape : scene.shapes)
			{
				for (const auto& idx : shape.mesh.indices)
				{
					vertices.push_back(scene.attrib.vertices[idx.vertex_index * 3 + 0]);
					vertices.push_back(scene.attrib.vertices[idx.vertex_index * 3 + 1]);
					vertices.push_back(scene.attrib.vertices[idx.vertex_index * 3 + 2]);

					for (auto k = 0; k < 12; ++k)
						poly.push_back(polyfit[idx.vertex_index * 12 + k]);

					if (scene.attrib.normals.size() > 0)
					{
						normals.push_back(scene.attrib.normals[idx.normal_index * 3 + 0]);
						normals.push_back(scene.attrib.normals[idx.normal_index * 3 + 1]);
						normals.push_back(scene.attrib.normals[idx.normal_index * 3 + 2]);
					}

					if (scene.attrib.texcoords.size() > 0)
					{
						texcoords.push_back(scene.attrib.texcoords[idx.texcoord_index * 2 + 0]);
						texcoords.push_back(scene.attrib.texcoords[idx.texcoord_index * 2 + 1]);
					}

					indices.push_back(indices.size());
				}
			}

			std::cout
				<< "File Info: " << std::endl
				<< '\t' << vertices.size() / 3 << " vertices\n"
				<< '\t' << normals.size() / 3 << " normals\n"
				<< '\t' << texcoords.size() / 2 << " texcoords\n"
				<< '\t' << poly.size() / 4 << " polyfit\n"
				<< '\t' << indices.size() << " indices\n";

			auto out_filename = output_dir / input_filename.stem();
			vector_write(out_filename.replace_extension(".vert").string(), vertices);
			vector_write(out_filename.replace_extension(".poly").string(), poly);
			if (normals.size() > 0)
				vector_write(out_filename.replace_extension(".nor").string(), normals);
			if (texcoords.size() > 0)
				vector_write(out_filename.replace_extension(".uv").string(), texcoords);

			auto vert_per_face = (uint8_t)scene.shapes[0].mesh.num_face_vertices[0];
			std::string extension = ((vert_per_face == 3) ? (".tri") : (".quad"));
			vector_write(out_filename.replace_extension(extension).string(), indices);
		}
		else
		{
			std::cerr << "[Error] Could not load obj file : " << input_filename.string() << std::endl;
			return EXIT_FAILURE;
		}
	}
	else
	{
		std::cerr << "Error: File format not supported" << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}


int main(int argc, char* argv[])
{
	//join_files(argv[1], argv[2]);
	//return 0;

	if (argc < 3)
	{
		std::cout
			<< std::fixed << std::endl
			<< "Usage            : ./<app.exe> <input_filename> <input_poly_filename> <output_dir>" << std::endl
			<< "Default          : ./split_3d_file.exe Fig02.obj Fig02.poly C:/tmp/" << std::endl
			<< std::endl;
		return EXIT_FAILURE;
	}

	//
	// Initial parameters
	//
	const std::string input_path = argv[1];
	const std::string input_poly_path = argv[2];
	const fs::path output_dir = ((argc > 3) ? argv[3] : (fs::is_directory(input_path) ? input_path : fs::path(input_path).parent_path()));

	fs::create_directory(output_dir);

	return split_file(input_path, input_poly_path, output_dir);
}