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
					<< "\nmapKd " << mat.diffuse_texname
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

			//save_material(scene, material_filename);

			out << std::fixed
				<< "###############################"
				<< "\n## Vertex Count    : " << scene.attrib.vertices.size()
				<< "\n## Normal Count    : " << scene.attrib.normals.size()
				<< "\n## TexCoord Count  : " << scene.attrib.texcoords.size()
				<< "\n## Color Count     : " << scene.attrib.colors.size()
				<< "\n## Material Count  : " << scene.materials.size()
				<< "\n###############################\n"
				<< "\nmatlib " << filename_without_dir << ".mtl\n"
				<< std::endl;

			
			for (auto i = 0; i < scene.attrib.vertices.size(); i+=3)
			{
				out << "v " << scene.attrib.vertices[i] << ' ' << scene.attrib.vertices[i + 1] << ' ' << scene.attrib.vertices[i + 2] << '\n';
			}

			for (auto i = 0; i < scene.attrib.normals.size(); i += 3)
			{
				out << "vn " << scene.attrib.normals[i] << ' ' << scene.attrib.normals[i + 1] << ' ' << scene.attrib.normals[i + 2] << '\n';
			}

			for (auto i = 0; i < scene.attrib.texcoords.size(); i += 2)
			{
				out << "vt " << scene.attrib.texcoords[i] << ' ' << scene.attrib.texcoords[i + 1] << '\n';
			}


			// for each shape (group)
			for (const auto& shape : scene.shapes)
			{
				const auto& mesh = shape.mesh;
				
				out << "\n###############################";
				if (shape.mesh.material_ids.size() > 0 && scene.materials.size() > shape.mesh.material_ids[0])
				{
					out << "\nusemtl " << scene.materials[shape.mesh.material_ids[0]].name;
				}
				out << "\ng " << (!shape.name.empty() ? shape.name : "no_name") << std::endl;

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
							out << '/' << idx.vertex_index + 1;
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
		std::vector<tinyobj::real_t>& vertices = out.attrib.vertices;
		std::vector<tinyobj::real_t>& normals = out.attrib.normals;
		std::vector<tinyobj::real_t>& texcoords = out.attrib.texcoords;
		vertices.clear();
		normals.clear();
		texcoords.clear();

		std::vector<int> vert_freq(in.attrib.vertices.size() / 3, 0);

		// count the amount of vertices which are not used
		for (const auto& shape : in.shapes)
		{
			const auto& mesh = shape.mesh;
			for (const auto& ind : mesh.indices)
			{
				vert_freq[ind.vertex_index]++;
			}
		}

		// copy the used vertices to a new vector
		for (int i = 0; i < vert_freq.size(); i++)
		{
			if (vert_freq[i] == 0)
				continue;
			
			const auto i2 = i * 2;
			const auto i3 = i * 3;

			vertices.push_back(in.attrib.vertices[i3]);
			vertices.push_back(in.attrib.vertices[i3 + 1]);
			vertices.push_back(in.attrib.vertices[i3 + 2]);
			
			if (in.attrib.normals.size() > 0 && in.attrib.normals.size() == in.attrib.vertices.size())
			{
				normals.push_back(in.attrib.normals[i3]);
				normals.push_back(in.attrib.normals[i3 + 1]);
				normals.push_back(in.attrib.normals[i3 + 2]);
			}
			
			if (in.attrib.texcoords.size() > 0 && in.attrib.texcoords.size() == in.attrib.vertices.size())
			{
				texcoords.push_back(in.attrib.texcoords[max(i2 + 0, in.attrib.texcoords.size() - 1)]);
				texcoords.push_back(in.attrib.texcoords[max(i2 + 1, in.attrib.texcoords.size() - 1)]);
			}
		}

		if (in.attrib.normals.size() != normals.size())
		{
			normals.clear();
			std::copy(in.attrib.normals.begin(), in.attrib.normals.end(), normals.begin());
		}

		if (in.attrib.texcoords.size() != in.attrib.vertices.size())
		{
			texcoords.clear();
			texcoords.insert(texcoords.begin(), in.attrib.texcoords.begin(), in.attrib.texcoords.end());
		}

		int zero_count = 0;
		for (int i = 0; i < vert_freq.size(); i++)
		{
			if (vert_freq[i] == 0)
				zero_count++;

			vert_freq[i] = zero_count;
		}

		std::cout << "[Info] The number of unused vertices is " << zero_count << std::endl;

		// for each shape
		out.shapes = in.shapes;
		for (auto& shape : out.shapes)
		{
			for (auto& idx : shape.mesh.indices)
			{
				auto replace = vert_freq[idx.vertex_index];
				idx.vertex_index -= replace;
				idx.normal_index -= replace;
				idx.texcoord_index -= replace;
			}
		}
	}

	static bool extract_group(scene_t& out, const std::string& group_name, const scene_t& in)
	{
		std::vector<tinyobj::real_t>& vertices = out.attrib.vertices;
		std::vector<tinyobj::real_t>& normals = out.attrib.normals;
		std::vector<tinyobj::real_t>& texcoords = out.attrib.texcoords;

		vertices.clear();
		normals.clear();
		texcoords.clear();

		std::vector<int> vert_freq(in.attrib.vertices.size() / 3, 0);

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

		const auto& mesh = shape->mesh;
		for (const auto& ind : mesh.indices)
		{
			vert_freq[ind.vertex_index]++;
		}

		// copy the used vertices to a new vector
		for (int i = 0; i < vert_freq.size(); i++)
		{
			if (vert_freq[i] == 0)
				continue;
			vertices.push_back(in.attrib.vertices[i]);

			if (in.attrib.normals.size() > 0)
				normals.push_back(in.attrib.normals[i]);

			if (in.attrib.texcoords.size() > 0)
				texcoords.push_back(in.attrib.texcoords[i]);
		}

		out.shapes = { *shape };
		for (auto& idx : out.shapes[0].mesh.indices)
		{
			auto replace = vert_freq[idx.vertex_index];
			idx.vertex_index -= replace;
			idx.normal_index -= replace;
			idx.texcoord_index -= replace;
		}

		return true;
	}
} // end namespace tinyobj
