#include <iostream>

#include <fstream>
#include <algorithm>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

#define _USE_MATH_DEFINES
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

struct MyTraits : public OpenMesh::DefaultTraits
{
};

using TriMesh = OpenMesh::TriMesh_ArrayKernelT<MyTraits>;
OpenMesh::VPropHandleT<TriMesh::Normal> Normals;
OpenMesh::VPropHandleT<TriMesh::TexCoord2D> TexCoord;

std::string g_filename_in;
std::string g_filename_out;



template <typename T>
std::string filename_append_before_extension(const std::string& filename_in, const T& append, const char separator = '_')
{
	auto ext_pos = filename_in.rfind('.', filename_in.length());
	if (ext_pos == std::string::npos)
		ext_pos = filename_in.length();
	std::stringstream filename_out;
	filename_out << filename_in.substr(0, ext_pos) << separator << append << filename_in.substr(ext_pos, filename_in.length() - ext_pos);
	return filename_out.str();
}


bool load_mesh_in(TriMesh& mesh)
{
	//mesh.request_vertex_texcoords2D();
	//mesh.request_vertex_normals();
	try
	{
		std::cout << "-- Loading mesh " << g_filename_in << std::endl;
		//OpenMesh::IO::Options OptionRead(OpenMesh::IO::Options::VertexTexCoord);
		return OpenMesh::IO::read_mesh(mesh, g_filename_in);
	}
	catch (const std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
		return false;
	}
}

bool save_mesh_out(TriMesh& mesh)
{
	try
	{
		std::cout << "-- Saving mesh " << g_filename_out << std::endl;
		//mesh.request_vertex_texcoords2D();
		//mesh.request_vertex_normals();
		//OpenMesh::IO::Options OptionWrite(OpenMesh::IO::Options::VertexTexCoord); // | OpenMesh::IO::Options::VertexNormal);
		return OpenMesh::IO::write_mesh(mesh, g_filename_out);
	}
	catch (const std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
		return false;
	}
}


void mesh_cut(TriMesh& mesh, float min_z)
{
	mesh.request_face_status();
  	mesh.request_edge_status();
  	mesh.request_vertex_status();

	
	std::cout << "deleting faces ;" << std::endl;
	std::vector<TriMesh::VertexHandle> verts;
	for (auto vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi)
	{
		if (mesh.point(*vi)[2] < min_z)
		{
			verts.push_back(*vi);
		}
	}

	std::cout << "deleting verts : " << verts.size() << std::endl;
	
	for (auto vh : verts)
	{
		for (auto fit = mesh.vf_cwbegin(vh); fit != mesh.vf_cwbegin(vh); ++fit)
			mesh.delete_face(*fit, false);

		mesh.delete_vertex(vh, false);
	}
	
	std::cout << "mesh.garbage_collection();" << std::endl;
	mesh.garbage_collection();

}



template<typename T>
void vector_write(std::ofstream& out_file, const std::vector<T>& data)
{
	const std::size_t count = data.size();
	out_file.write(reinterpret_cast<const char*>(&count), sizeof(std::size_t));
	out_file.write(reinterpret_cast<const char*>(&data[0]), count * sizeof(T));
}


template<typename T>
void vector_read(std::ifstream& in_file, std::vector<T>& data)
{
	std::size_t count;
	in_file.read(reinterpret_cast<char*>(&count), sizeof(std::size_t));
	data.resize(count);
	in_file.read(reinterpret_cast<char*>(&data[0]), count * sizeof(T));
}

void save_verts_index(const TriMesh& mesh, float min_z)
{
	std::vector<int> verts;
	for (auto vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi)
	{
		if (mesh.point(*vi)[2] < min_z)
		{
			verts.push_back(vi->idx());
		}
	}

    std::cout << "Vertex list size: " << verts.size() << std::endl;

	std::ofstream out_file;
	out_file.open("vertex_list.bin", std::ios::out | std::ios::binary);
	vector_write(out_file, verts);
	out_file.close();
}

void delete_vertex_index(TriMesh& mesh)
{
	std::vector<int> verts;
	std::ifstream in_file;
	in_file.open("vertex_list.bin", std::ios::in | std::ios::binary);
	vector_read(in_file, verts);
	in_file.close();

	mesh.request_face_status();
  	mesh.request_edge_status();
  	mesh.request_vertex_status();

    std::cout << "Vertex count to delete: " << verts.size() << std::endl;

	for (auto vidx : verts)
	{
		auto vh = mesh.vertex_handle(vidx);
		for (auto fit = mesh.vf_cwbegin(vh); fit != mesh.vf_cwbegin(vh); ++fit)
			mesh.delete_face(*fit, false);

		mesh.delete_vertex(vh, false);
	}
	
	std::cout << "mesh.garbage_collection();" << std::endl;
	mesh.garbage_collection();
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout 
			<< "Usage: app <mesh_file> < min_z > \n"
			<< "Usage: app ../data/face.obj 0 \n";
		return EXIT_FAILURE;
	}
	
	g_filename_in = argv[1];
	const float min_z = atof(argv[2]);
	g_filename_out = filename_append_before_extension(g_filename_in, "cut");
    
    //g_filename_out = g_filename_in;
    //g_filename_out = g_filename_out.replace(g_filename_out.size() - 8, 5, ".");



	TriMesh mesh_in;
	if (!load_mesh_in(mesh_in))
	{
		std::cout << "Could not read " << g_filename_in << std::endl;
		return EXIT_FAILURE;
	}

    //
    // these three functions are independent
    // for simplicity, choose only one of then
    //
	//mesh_cut(mesh_in, min_z);
	//save_verts_index(mesh_in, min_z);
	delete_vertex_index(mesh_in);

	if (!save_mesh_out(mesh_in))
	{
		std::cout << "Could not save " << g_filename_out << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}