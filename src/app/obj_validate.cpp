#include <iostream>
#include "mesh_utils.h"
#include "tinyobj.h"


#if 0

template <typename mesh_t>
void remove_unused_vertices(mesh_t& mesh)
{
	std::cout << "before: " << mesh.n_vertices() << std::endl;
	mesh_t::VertexIter v_it, v_end(mesh.vertices_end());
	mesh_t::VertexFaceIter vf_it;
	mesh_t::FaceIter f_it, f_end(mesh.faces_end());
	std::vector<mesh_t::VertexIter> vhandles;
	std::vector<mesh_t::FaceIter> fhandles;

	for (f_it = mesh.faces_begin(); f_it != f_end; ++f_it)
	{
		if (mesh.valence(f_it) < 1)
			fhandles.push_back(f_it);
	}

	for (v_it = mesh.vertices_begin(); v_it != v_end; ++v_it)
	{
		if (mesh.valence(v_it) < 1)
			vhandles.push_back(v_it);
	}
#if 0
	for (v_it = mesh.vertices_begin(); v_it != v_end; ++v_it)
	{
		if (!mesh.vf_iter(*v_it).is_valid())
		{
			handles.push_back(v_it);
			std::cout << "\n** " << mesh.point(v_it);
		}
		else
		{
			std::cout << "\n   " << mesh.point(v_it);
		}
	}
#endif
	std::cout << "!valid: " << vhandles.size() << ' ' << fhandles.size() << std::endl;

	for (const auto& v : vhandles)
		mesh.delete_vertex(v, false);

	mesh.garbage_collection();
	std::cout << "after : " << mesh.n_vertices() << std::endl;
}

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cerr << "Usage: app <filename_in> <filename_out>\n";
		return EXIT_FAILURE;
	}

	const std::string filename_in = argv[1];
	const std::string filename_out = argv[2];


	OpenMesh::IO::Options mesh_io_opt(OpenMesh::IO::Options::VertexTexCoord);
	Mesh_t mesh;

	try
	{
		std::cout << "Loading: " << filename_in << '\n';
		mesh.request_vertex_texcoords2D();
		OpenMesh::IO::read_mesh(mesh, filename_in, mesh_io_opt);
	}
	catch (const std::exception &ex)
	{
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}

	remove_unused_vertices(mesh);

	return EXIT_SUCCESS;
}
#else

int main(int argc, char* argv[]) 
{ 
	if (argc < 3)
	{
        std::cerr << "Usage: app <filename_in> <filename_out>\n";
        return EXIT_FAILURE;
	}

	const std::string filename_in = argv[1];
	const std::string filename_out = argv[2];

	tinyobj::scene_t scene;
	auto success_loading = tinyobj::load(scene, filename_in);

	if (!success_loading)
		return EXIT_FAILURE;

	tinyobj::print_info(scene);
	
	tinyobj::scene_t scene_out;
	tinyobj::garbage_collect(scene_out, scene);

	auto success_saving = tinyobj::save(scene, filename_out);

	if (!success_saving)
		return EXIT_FAILURE;



	return EXIT_SUCCESS;
}
#endif

