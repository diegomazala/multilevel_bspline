#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <unordered_map>


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
		auto last_slash = std::min(filename.rfind('/'), filename.rfind('\\'));
		auto dir = filename.substr(0, last_slash);
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

	static bool save(const scene_t& scene, const std::string& filename)
	{
		try
		{
			std::ofstream out(filename, std::ios::out);
			
			out << std::fixed;

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
			for (auto& shape : scene.shapes)
			{
				const auto& mesh = shape.mesh;
				
				out << "\n###############################\n"
					<< "g " << (!shape.name.empty() ? shape.name : "no_name") << std::endl;

				size_t index_offset = 0;

				// for each face
				for (auto f = 0; f < shape.mesh.num_face_vertices.size(); ++f)
				{
					size_t verts_per_face = shape.mesh.num_face_vertices[f];

					out << "f ";
					// For each vertex in the face
					for (size_t v = 0; v < verts_per_face; v++)
					{
						tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
						out << idx.vertex_index + 1 << ' ';
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
			<< "\nVertex Count    : " << scene.attrib.vertices.size()
			<< "\nNormal Count    : " << scene.attrib.normals.size()
			<< "\nTexCoord Count  : " << scene.attrib.texcoords.size()
			<< "\nColors Count    : " << scene.attrib.colors.size()
			<< "\nMaterial Count  : " << scene.materials.size()
			<< std::endl;
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
		for (auto& shape : in.shapes)
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
			vertices.push_back(in.attrib.vertices[i]);
			
			if (in.attrib.normals.size() > 0)
				normals.push_back(in.attrib.normals[i]);
			
			if (in.attrib.texcoords.size() > 0)
				texcoords.push_back(in.attrib.texcoords[i]);
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

		std::cout << vertices.size() << ' ' << normals.size() << ' ' << texcoords.size() << std::endl;
	}
} // end namespace tinyobj
