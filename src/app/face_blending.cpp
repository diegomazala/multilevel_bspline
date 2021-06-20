#include <Eigen/QR>
#include <vector>
#include <filesystem>
#include "face_blending_project.h"
#include "timer.h"
#include "vector_read_write_binary.h"
#include "tinyobj.h"


namespace fs = std::filesystem;

std::vector<std::vector<float>> vertOriginal;
std::vector<std::vector<float>> vertSmoothed;
std::vector<std::vector<float>> polyfit;


Eigen::Vector3f get_vertex_from_mesh_original(std::size_t mesh_index, std::size_t vert_index)
{
	return Eigen::Vector3f(
		vertOriginal[mesh_index][vert_index + 0], 
		vertOriginal[mesh_index][vert_index + 1], 
		vertOriginal[mesh_index][vert_index + 2]);
}

Eigen::Vector3f get_vertex_from_mesh_smoothed(std::size_t mesh_index, std::size_t vert_index)
{
	return Eigen::Vector3f(
		vertSmoothed[mesh_index][vert_index + 0], 
		vertSmoothed[mesh_index][vert_index + 1], 
		vertSmoothed[mesh_index][vert_index + 2]);
}






int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout
			<< "Usage: app <project_file> \n"
			<< "Usage: ./face_blending.exe faces2.prj\n"
			<< "Usage: ./face_blending.exe E:/Projects/MDP/faceblending/samples/models/faces2.prj \n";
		return EXIT_FAILURE;
	}

	  
	//
	// Load and process the project file
	//
	const std::string project_filename = argv[1];
	std::cout << "[Info]  Loading project file " << project_filename << std::endl;
	FaceBlendingProject project;
	if (!project.load(project_filename.c_str()))
	{
		std::cout << "[Error] Could not load project file " << project_filename << std::endl;
		return EXIT_FAILURE;
	}

	//
	// Get the parameters from project file
	//
	std::vector<std::string> originalMeshes   = project.getOriginalMeshesFilename();
	std::vector<std::string> smoothedMeshes	  = project.getSmoothedMeshesFilename();
	std::vector<std::string> texturesFilename = project.getTexturesFilename();
	std::vector<std::string> polyfitFilename  = project.getPolyfitFilename();
	std::vector<float>		 mask_weight	  = project.getMaskWeights();
	std::vector<float>		 mouse_pos		  = project.getMousePositions();

	//
	// Initialize vectors to storage the vertices
	//
	const auto mesh_count = originalMeshes.size();
	vertOriginal = std::vector<std::vector<float>>(mesh_count);
	vertSmoothed = std::vector<std::vector<float>>(mesh_count);
	polyfit		 = std::vector<std::vector<float>>(mesh_count);
	
	//
	// Load first mesh in the vectors
	//
	auto vert_count_o = vector_read(originalMeshes[0], vertOriginal[0]);
	auto vert_count_s = vector_read(smoothedMeshes[0], vertSmoothed[0]);
	auto polyfit_count = vector_read(polyfitFilename[0], polyfit[0]);

	assert(vert_count_o == vert_count_s && "Number of vertices does not match");
	const auto vert_count = vert_count_o;

	//
	// Load all remaining meshes
	//
	for (auto i = 1; i < mesh_count; ++i)
	{
		vert_count_o = vector_read(originalMeshes[i], vertOriginal[i]);
		vert_count_s = vector_read(smoothedMeshes[i], vertSmoothed[i]);
		polyfit_count = vector_read(polyfitFilename[i], polyfit[i]);

		assert(vert_count_o == vert_count_s 
			&& vert_count_s == vert_count
			&& "Number of vertices does not match");
	}

	//
	// Load triangle indices
	//
	std::vector<uint32_t> indices;
	auto tris_count = vector_read(project.getTriMeshFilename(), indices);

	//
	// Compute smooth_mix
	//
	Eigen::Vector3f mask_tex = Eigen::Vector3f::Ones();
	float smooth_mix = (mask_tex[0] * mask_weight[0]) + (mask_tex[1] * mask_weight[1]) + (mask_tex[2] * mask_weight[2]);

	//
	// for each vertex
	//
	for (auto iv = 0; iv < vert_count; iv += 3)
	{
		std::vector<Eigen::Vector3f> d_lerp(mesh_count, Eigen::Vector3f::Zero());
		std::vector<Eigen::Vector3f> d_curve(mesh_count, Eigen::Vector3f::Zero());
		std::vector<Eigen::Vector3f> w(mesh_count, Eigen::Vector3f::Zero());

		//
		// for each mesh
		//
		for (auto im = 0; im < mesh_count; ++im)
		{
			d_lerp[im] = get_vertex_from_mesh_original(im, iv) - get_vertex_from_mesh_smoothed(im, iv);
		}
	}



	//
	// Output obj file
	//
	{
		tinyobj::scene_t output_obj;
		output_obj.attrib.vertices = vertOriginal[0];
		tinyobj::shape_t shape;

		for (const auto& ind : indices)
			shape.mesh.indices.push_back({ (int)ind, 0, 0 });

		shape.mesh.num_face_vertices = std::vector<uint8_t>(shape.mesh.indices.size() / 3, 3);
		shape.name = "main_shape";
		output_obj.shapes.push_back(shape);

		const std::string filename = "c:/tmp/out.obj";

		std::cout << "[Info]  Saving obj file : " << filename << std::endl;
		auto success_saving = tinyobj::save(output_obj, filename);
		if (!success_saving)
		{
			std::cerr << "[Error] Saving obj file : " << filename << std::endl;
			return false;
		}

		std::cout << "[Info]  Saved obj file  : " << filename << std::endl;
	}

    return EXIT_SUCCESS;
}