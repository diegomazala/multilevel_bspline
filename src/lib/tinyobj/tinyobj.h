#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


#ifndef NOMINMAX
#define min(x,y) ((x) < (y) ? (x) : (y))
#define max(x,y) ((x) > (y) ? (x) : (y))
#endif

namespace tinyobj
{

	struct scene_t
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn;
		std::string err;
	};

	

	static bool load(scene_t& scene, const std::string& filename)
	{
#if _WIN32
		const auto last_slash = min(filename.rfind('/'), filename.rfind('\\'));
#else
		const auto last_slash = min(filename.rfind('/'), filename.rfind('\\'));
#endif

		const auto dir = filename.substr(0, last_slash);
		const bool triangulate = false;


		bool success = tinyobj::LoadObj(
			&scene.attrib,
			&scene.shapes,
			&scene.materials,
			&scene.warn,
			&scene.err,
			filename.c_str(),
			dir.c_str(),
			triangulate);

		if (!scene.warn.empty())
		{
			std::cout << "WARN: " << scene.warn << std::endl;
		}

		if (!scene.err.empty())
		{
			std::cerr << "ERR: " << scene.err << std::endl;
		}

		if (!success)
		{
			std::cerr << "Failed to load/parse.\n";
			return false;
		}

		return success;
	}


	static bool save_material(const scene_t& scene, const std::string& filename)
	{
		try
		{
			std::ofstream out(filename, std::ios::out);

			for (const auto& mat : scene.materials)
			{
				out << std::fixed
					<< "\nnewmtl " << mat.name
					<< "\nmap_Kd " << mat.diffuse_texname
					<< "\nKa " << mat.ambient[0] << ' ' << mat.ambient[1] << ' ' << mat.ambient[2]
					<< "\nKd " << mat.diffuse[0] << ' ' << mat.diffuse[1] << ' ' << mat.diffuse[2]
					<< "\nKs " << mat.specular[0] << ' ' << mat.specular[1] << ' ' << mat.specular[2]
					<< std::endl;
			}
			out.close();
			return true;
		}
		catch (const std::exception &ex)
		{
			std::cerr << ex.what() << std::endl;
			return false;
		}
	}


	static bool save(const scene_t& scene, const std::string& filename)
	{
		try
		{
			std::ofstream out(filename, std::ios::out);

#if _WIN32
			const auto last_slash = min(filename.rfind('/'), filename.rfind('\\'));
			const auto filename_without_dir = filename.substr(last_slash + 1, filename.size() - last_slash - 5);
#else
			const auto last_slash = min(filename.rfind('/'), filename.rfind('\\'));
			const auto filename_without_dir = filename.substr(last_slash, filename.size() - last_slash);
#endif
			const auto dir = filename.substr(0, last_slash);
			const std::string material_filename = dir + '/' + filename_without_dir + ".mtl";

			save_material(scene, material_filename);

			out << std::fixed
				<< "###############################"
				<< "\n## Vertex Count    : " << scene.attrib.vertices.size()
				<< "\n## Normal Count    : " << scene.attrib.normals.size()
				<< "\n## TexCoord Count  : " << scene.attrib.texcoords.size()
				<< "\n## Color Count     : " << scene.attrib.colors.size()
				<< "\n## Material Count  : " << scene.materials.size()
				<< "\n###############################\n"
				<< "\nmtllib " << filename_without_dir << ".mtl\n"
				<< std::endl;

			if (scene.attrib.colors.size() > 0)
			{
				for (auto i = 0; i < scene.attrib.vertices.size(); i += 3)
				{
					out << "v " << scene.attrib.vertices[i] << ' ' << scene.attrib.vertices[i + 1] << ' ' << scene.attrib.vertices[i + 2]
						<< ' ' << scene.attrib.colors[i] << ' ' << scene.attrib.colors[i + 1] << ' ' << scene.attrib.colors[i + 2] << '\n';
				}
			}
			else
			{
				for (auto i = 0; i < scene.attrib.vertices.size(); i += 3)
				{
					out << "v " << scene.attrib.vertices[i] << ' ' << scene.attrib.vertices[i + 1] << ' ' << scene.attrib.vertices[i + 2] << '\n';
				}
			}


			for (auto i = 0; i < scene.attrib.normals.size(); i += 3)
			{
				out << "vn " << scene.attrib.normals[i] << ' ' << scene.attrib.normals[i + 1] << ' ' << scene.attrib.normals[i + 2] << '\n';
			}

			for (auto i = 0; i < scene.attrib.texcoords.size(); i += 2)
			{
				out << "vt " << scene.attrib.texcoords[i] << ' ' << scene.attrib.texcoords[i + 1] << '\n';
			}

			out << std::endl;


			// for each shape (group)
			std::size_t matidx = 0;
			for (const auto& shape : scene.shapes)
			{
				const auto& mesh = shape.mesh;
				
				const auto mat_name = (scene.materials.size() > 0 ? scene.materials[matidx++ % scene.materials.size()].name : "default");

				out << "\n###############################"
					<< "\nusemtl " << mat_name
					<< "\ng " << (!shape.name.empty() ? shape.name : "no_name") << std::endl;

				size_t index_offset = 0;


				// for each face
				for (auto f = 0; f < shape.mesh.num_face_vertices.size(); ++f)
				{
					size_t verts_per_face = shape.mesh.num_face_vertices[f];

					out << 'f';
					// For each vertex in the face
					for (size_t v = 0; v < verts_per_face; v++)
					{
						tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
						out << ' ' << idx.vertex_index + 1;

						if (idx.texcoord_index > -1)
						{
							out << '/' << idx.texcoord_index + 1;
							if (idx.normal_index > -1)
								out << '/' << idx.normal_index + 1;
						}
						else
						{
							if (idx.normal_index > -1)
								out << "//" << idx.normal_index + 1;
						}
					}
					out << '\n';
					index_offset += verts_per_face;
				}

			}

			out.close();
			return true;
		}
		catch (const std::exception &ex)
		{
			std::cerr << ex.what() << std::endl;
			return false;
		}
	}


	static void print_info(const scene_t& scene)
	{
		std::cout << std::fixed
			<< "== Attributes: "
			<< "\nVertex Count    : " << scene.attrib.vertices.size() / 3
			<< "\nNormal Count    : " << scene.attrib.normals.size() / 3
			<< "\nTexCoord Count  : " << scene.attrib.texcoords.size() / 2
			<< "\nColors Count    : " << scene.attrib.colors.size()
			<< "\nMaterial Count  : " << scene.materials.size()
			<< "\nShapes          : " << scene.materials.size();
		    
		for (const auto& shape : scene.shapes)
		{
			std::cout
				<< "\n                  - [" << shape.name
				<< "] " << shape.mesh.indices.size() / shape.mesh.num_face_vertices[0];
		}
	
		std::cout << "\n================" << std::endl;
	}

	static void garbage_collect(scene_t& out, const scene_t& in)
	{
		out.materials = in.materials;

		std::vector<tinyobj::real_t>& vertices = out.attrib.vertices;
		std::vector<tinyobj::real_t>& normals = out.attrib.normals;
		std::vector<tinyobj::real_t>& texcoords = out.attrib.texcoords;
		vertices.clear();
		normals.clear();
		texcoords.clear();

		std::vector<int> vert_freq(in.attrib.vertices.size() / 3, 0);
		std::vector<int> texcoord_freq(in.attrib.texcoords.size() / 2, 0);

		// count the amount of vertices which are not used
		for (const auto& shape : in.shapes)
		{
			const auto& mesh = shape.mesh;
			for (const auto& ind : mesh.indices)
			{
				vert_freq[ind.vertex_index]++;
				texcoord_freq[ind.texcoord_index]++;
			}
		}

		// copy the used vertices to a new vector
		for (int i = 0; i < vert_freq.size(); i++)
		{
			if (vert_freq[i] == 0)
				continue;
			
			const auto i3 = i * 3;

			vertices.push_back(in.attrib.vertices[i3]);
			vertices.push_back(in.attrib.vertices[i3 + 1]);
			vertices.push_back(in.attrib.vertices[i3 + 2]);
		}

		// copy the used texcoords to a new vector
		for (int i = 0; i < texcoord_freq.size(); i++)
		{
			if (texcoord_freq[i] == 0)
				continue;

			const auto i2 = i * 2;

			texcoords.push_back(in.attrib.texcoords[i2]);
			texcoords.push_back(in.attrib.texcoords[i2 + 1]);
		}


		int zero_count_vert = 0;
		for (int i = 0; i < vert_freq.size(); i++)
		{
			if (vert_freq[i] == 0)
				zero_count_vert++;

			vert_freq[i] = zero_count_vert;
		}

		int zero_count_texcoord = 0;
		for (int i = 0; i < texcoord_freq.size(); i++)
		{
			if (texcoord_freq[i] == 0)
				zero_count_texcoord++;

			texcoord_freq[i] = zero_count_texcoord;
		}

		std::cout << "[Info] The number of unused vertices is " << zero_count_vert << std::endl;
		std::cout << "[Info] The number of unused texcoord is " << zero_count_texcoord << std::endl;

		// for each shape
		out.shapes = in.shapes;
		for (auto& shape : out.shapes)
		{
			for (auto& idx : shape.mesh.indices)
			{
				auto replace_vert = vert_freq[idx.vertex_index];
				auto replace_texcoord = texcoord_freq[idx.texcoord_index];
				idx.vertex_index -= replace_vert;
				idx.normal_index = -1;
				idx.texcoord_index -= replace_texcoord;
			}
		}
	}

	static bool extract_group(scene_t& out, const std::string& group_name, const scene_t& in)
	{
		out.materials = in.materials;

		std::vector<tinyobj::real_t>& vertices = out.attrib.vertices;
		std::vector<tinyobj::real_t>& normals = out.attrib.normals;
		std::vector<tinyobj::real_t>& texcoords = out.attrib.texcoords;

		vertices.clear();
		normals.clear();
		texcoords.clear();
				
		auto shape = in.shapes.begin();

		while (shape != in.shapes.end() && shape->name != group_name)
		{
			++shape;
		}

		if (shape == in.shapes.end())
		{
			std::cerr << "Error: Could not find group name: " << group_name << std::endl;
			return false;
		}

		std::vector<int> vert_freq(in.attrib.vertices.size() / 3, 0);
		std::vector<int> texcoord_freq(in.attrib.texcoords.size() / 2, 0);

		const auto& mesh = shape->mesh;
		for (const auto& ind : mesh.indices)
		{
			vert_freq[ind.vertex_index]++;
			texcoord_freq[ind.texcoord_index]++;
		}

		// copy the used vertices to a new vector
		for (int i = 0; i < vert_freq.size(); i++)
		{
			if (vert_freq[i] == 0)
				continue;

			const auto i3 = i * 3;

			vertices.push_back(in.attrib.vertices[i3]);
			vertices.push_back(in.attrib.vertices[i3 + 1]);
			vertices.push_back(in.attrib.vertices[i3 + 2]);
		}

		// copy the used texcoords to a new vector
		for (int i = 0; i < texcoord_freq.size(); i++)
		{
			if (texcoord_freq[i] == 0)
				continue;

			const auto i2 = i * 2;

			texcoords.push_back(in.attrib.texcoords[i2]);
			texcoords.push_back(in.attrib.texcoords[i2 + 1]);
		}


		int zero_count_vert = 0;
		for (int i = 0; i < vert_freq.size(); i++)
		{
			if (vert_freq[i] == 0)
				zero_count_vert++;

			vert_freq[i] = zero_count_vert;
		}

		int zero_count_texcoord = 0;
		for (int i = 0; i < texcoord_freq.size(); i++)
		{
			if (texcoord_freq[i] == 0)
				zero_count_texcoord++;

			texcoord_freq[i] = zero_count_texcoord;
		}

		out.shapes = { *shape };
		for (auto& idx : out.shapes[0].mesh.indices)
		{
			auto replace_vert = vert_freq[idx.vertex_index];
			auto replace_texcoord = texcoord_freq[idx.texcoord_index];
			idx.vertex_index -= replace_vert;
			idx.normal_index = -1;
			idx.texcoord_index -= replace_texcoord;
		}

		return true;
	}
} // end namespace tinyobj
