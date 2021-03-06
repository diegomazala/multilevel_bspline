﻿
#include <iostream>
#include <iomanip> 
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <thread>
#include <exception>
#include "cmdline.h"
#include "timer.h"

#include <filesystem>
namespace fs = std::filesystem;

#define _USE_MATH_DEFINES
#undef min
#undef max
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
using PolyMesh = OpenMesh::PolyMesh_ArrayKernelT<>;


cmdline::parser cmd_parser;
void cmd_parser_build()
{
	cmd_parser.add<std::string>("d",  0, "Detail mesh file", true);
	cmd_parser.add<std::string>("b",  0, "Base mesh file", false);
	cmd_parser.add<std::string>("ds", 0, "Smooth detail mesh file", true);
	cmd_parser.add<std::string>("bs", 0, "Smooth target mesh file", true);
	cmd_parser.add<std::string>("o",  0, "Output mesh result filename", true, "output.obj");
}


int main(int argc, char* argv[])
{
	//
	// Parse command line
	//
	cmd_parser_build();
	cmd_parser.parse_check(argc, argv);


	//
	// Set variables
	//
	const std::string& detail_mesh_filename 		= cmd_parser.get<std::string>("d");
	const std::string& detail_smooth_mesh_filename	= cmd_parser.get<std::string>("ds");
	const std::string& base_mesh_filename           = cmd_parser.get<std::string>("b");
	const std::string& base_smooth_mesh_filename 	    = cmd_parser.get<std::string>("bs");
	const std::string& output_mesh_filename 		= cmd_parser.get<std::string>("o");


	PolyMesh detail_mesh;	
	PolyMesh detail_smooth_mesh;
	PolyMesh base_smooth_mesh;

	timer t;
	t.start();

	std::vector<std::thread> thread;
	try
	{
		std::cout << "Loading: " << detail_mesh_filename << '\n';
        OpenMesh::IO::Options OptionRead(OpenMesh::IO::Options::VertexTexCoord);
        detail_mesh.request_vertex_texcoords2D();
		thread.push_back(std::thread([&] { OpenMesh::IO::read_mesh(detail_mesh, detail_mesh_filename, OptionRead); }));

		//std::cout << "Loading: " << base_mesh_filename << '\n';
		//thread.push_back(std::thread([&]() { OpenMesh::IO::read_mesh(target_mesh, base_mesh_filename, OptionRead); }));

		std::cout << "Loading: " << detail_smooth_mesh_filename << '\n';
        detail_smooth_mesh.request_vertex_texcoords2D();
		thread.push_back(std::thread([&] { OpenMesh::IO::read_mesh(detail_smooth_mesh, detail_smooth_mesh_filename, OptionRead); }));

		std::cout << "Loading: " << base_smooth_mesh_filename << '\n';
        base_smooth_mesh.request_vertex_texcoords2D();
		thread.push_back(std::thread([&] { OpenMesh::IO::read_mesh(base_smooth_mesh, base_smooth_mesh_filename, OptionRead); }));
	}
	catch(const std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}

	for (auto& t : thread) 
	 	t.join();


	t.stop();
	t.print_interval("Loading meshes  : ");

	if (detail_mesh.n_vertices() != base_smooth_mesh.n_vertices() || detail_smooth_mesh.n_vertices() != base_smooth_mesh.n_vertices())
	{
		std::cerr 
			<< "[Error] Vertex count does not match. The meshes must have the same topology.\n" 
			<< "Detail Mesh     : " << detail_mesh.n_vertices() << ' ' << detail_mesh_filename  << '\n'
                << "Detail Smooth Mesh: " << detail_smooth_mesh.n_vertices() << ' '
                << detail_smooth_mesh_filename << '\n'
                << "Base Smooth Mesh: " << base_smooth_mesh.n_vertices() << ' '
                << base_smooth_mesh_filename << '\n'
			<< std::endl;
		return EXIT_FAILURE;
	}

	PolyMesh::VertexIter v_it_d(detail_mesh.vertices_begin()), v_end_d(detail_mesh.vertices_end());
	PolyMesh::VertexIter v_it_db(detail_smooth_mesh.vertices_begin()), v_end_db(detail_smooth_mesh.vertices_end());
	PolyMesh::VertexIter v_it_tb(base_smooth_mesh.vertices_begin()), v_end_tb(base_smooth_mesh.vertices_end());

	t.start();
	for (; v_it_tb != v_end_tb; ++v_it_d, ++v_it_db, ++v_it_tb) 
	{
		const auto vec3 = detail_mesh.point(*v_it_d) - detail_smooth_mesh.point(*v_it_db) + base_smooth_mesh.point(*v_it_tb);
		detail_mesh.set_point(*v_it_d, vec3);
	}
	t.stop();
	t.print_interval("Computing detail: ");
	


	
	std::cout << "Saving : " << output_mesh_filename << '\n';
	t.start();
    OpenMesh::IO::Options OptionWrite(OpenMesh::IO::Options::VertexTexCoord);
	if (!OpenMesh::IO::write_mesh(detail_mesh, output_mesh_filename, OptionWrite))
	{
		std::cerr << "Error: cannot write mesh to " << output_mesh_filename << std::endl;
		return false;
	}
	t.stop();
	t.print_interval("Saving mesh file: ");
	

	return EXIT_SUCCESS;
}
