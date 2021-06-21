#include <Eigen/QR>
#include <vector>
#include <filesystem>
#include "face_blending_project.h"
#include "timer.h"
#include "vector_read_write_binary.h"
#include "tinyobj.h"


namespace fs = std::filesystem;

const float epsilon = 0.001f;
const float pow_distance = 2.0f;

const int coeff_order = 3 + 1;

const int base_mesh = 1;
const float displacement_multiplier = 1.0f;

std::vector<std::vector<float>> vertOriginal;
std::vector<std::vector<float>> vertSmoothed;
std::vector<std::vector<float>> polyfit;
std::size_t n_verts = 0;


Eigen::Vector3f get_vertex_from_mesh_original(std::size_t mesh_index, std::size_t vert_index)
{
	return Eigen::Vector3f(
		vertOriginal[mesh_index][vert_index * 3 + 0], 
		vertOriginal[mesh_index][vert_index * 3 + 1], 
		vertOriginal[mesh_index][vert_index * 3 + 2]);
}

Eigen::Vector3f get_vertex_from_mesh_smoothed(std::size_t mesh_index, std::size_t vert_index)
{
	return Eigen::Vector3f(
		vertSmoothed[mesh_index][vert_index * 3 + 0], 
		vertSmoothed[mesh_index][vert_index * 3 + 1],
		vertSmoothed[mesh_index][vert_index * 3 + 2]);
}

Eigen::Vector3f get_vertex_curve_fit(std::size_t mesh_index, std::size_t vert_index, float mix)
{
	const auto& mesh = polyfit[mesh_index];

	auto ivx = (vert_index * coeff_order) + (n_verts * 0 * coeff_order);
	auto ivy = (vert_index * coeff_order) + (n_verts * 1 * coeff_order);
	auto ivz = (vert_index * coeff_order) + (n_verts * 2 * coeff_order);

	Eigen::Vector4f coeff_x(mesh[ivx + 0], mesh[ivx + 1], mesh[ivx + 2], mesh[ivx + 3]);
	Eigen::Vector4f coeff_y(mesh[ivy + 0], mesh[ivy + 1], mesh[ivy + 2], mesh[ivy + 3]);
	Eigen::Vector4f coeff_z(mesh[ivz + 0], mesh[ivz + 1], mesh[ivz + 2], mesh[ivz + 3]);

	return Eigen::Vector3f(
		coeff_x.w() * pow(mix, 3) + coeff_x.z() * pow(mix, 2) + coeff_x.y() * mix + coeff_x.x(),
		coeff_y.w() * pow(mix, 3) + coeff_y.z() * pow(mix, 2) + coeff_y.y() * mix + coeff_y.x(),
		coeff_z.w() * pow(mix, 3) + coeff_z.z() * pow(mix, 2) + coeff_z.y() * mix + coeff_z.x()
	);
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

	timer t;
	t.start();
	  
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
	std::vector<float>		 thumbnailPos2d	  = project.getThumbnailsPos();

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
	auto vert_count = vert_count_o;
	n_verts = vert_count / 3;

	std::vector<float> vertex_output(vert_count);

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

	std::cout << "[Info] Mesh loading " << t.diff_sec_now() << "s" << std::endl;

	//
	// Load triangle indices
	//
	std::vector<uint32_t> indices;
	auto tris_count = vector_read(project.getTriMeshFilename(), indices);
	auto n_triangles = tris_count / 3;

	//
	// Compute smooth_mix
	//
	Eigen::Vector3f mask_tex(1, 0, 0); // single region
	float smooth_mix = (mask_tex[0] * mask_weight[0]) + (mask_tex[1] * mask_weight[1]) + (mask_tex[2] * mask_weight[2]);

	Eigen::Vector2f mouse_screen[3] = 
	{
		Eigen::Vector2f(mouse_pos[0] * 2 - 1, mouse_pos[1] * 2 - 1),
		Eigen::Vector2f(mouse_pos[2] * 2 - 1, mouse_pos[3] * 2 - 1),
		Eigen::Vector2f(mouse_pos[4] * 2 - 1, mouse_pos[5] * 2 - 1)
	};

	//
	// for each vertex
	//
	for (auto iv = 0; iv < n_verts; ++iv)
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

			d_curve[im] = get_vertex_curve_fit(im, iv, 0) - get_vertex_curve_fit(im, iv, smooth_mix);

			Eigen::Vector2f pos2d(thumbnailPos2d[im + 0], thumbnailPos2d[im + 1]);

			Eigen::Vector3f dist = Eigen::Vector3f(
				(mouse_screen[0] - pos2d).norm(),
				(mouse_screen[1] - pos2d).norm(),
				(mouse_screen[2] - pos2d).norm());

			w[im][0] = pow(1.0f / (dist[0] + epsilon), pow_distance);
			w[im][1] = pow(1.0f / (dist[1] + epsilon), pow_distance);
			w[im][2] = pow(1.0f / (dist[2] + epsilon), pow_distance);
		}

		Eigen::Vector3f sum_w(0, 0, 0);
		for (int i = 0; i < mesh_count; ++i)
			sum_w += w[i];


		//
		// computing vertex base
		//
		Eigen::Vector3f vbase_lerp = Eigen::Vector3f(0);
		{
			Eigen::Vector3f v_smoothed = get_vertex_from_mesh_smoothed(base_mesh, iv);
			vbase_lerp = v_smoothed + d_lerp[base_mesh] * (1 - smooth_mix);
		}

		Eigen::Vector3f vbase_curve = get_vertex_curve_fit(base_mesh, iv, smooth_mix);


		Eigen::Vector3f disp_masked_lerp[3] = { Eigen::Vector3f::Zero(), Eigen::Vector3f::Zero(), Eigen::Vector3f::Zero() };
		Eigen::Vector3f disp_masked_curve[3] = { Eigen::Vector3f::Zero(), Eigen::Vector3f::Zero(), Eigen::Vector3f::Zero() };

		for (auto im = 0; im < mesh_count; ++im)
		{
			disp_masked_lerp[0] = disp_masked_lerp[0] + w[im][0] * mask_tex.x() * smooth_mix * d_lerp[im];
			disp_masked_lerp[1] = disp_masked_lerp[1] + w[im][1] * mask_tex.y() * smooth_mix * d_lerp[im];
			disp_masked_lerp[2] = disp_masked_lerp[2] + w[im][2] * mask_tex.z() * smooth_mix * d_lerp[im];

			disp_masked_curve[0] = disp_masked_curve[0] + w[im][0] * mask_tex.x() * d_curve[im];
			disp_masked_curve[1] = disp_masked_curve[1] + w[im][1] * mask_tex.y() * d_curve[im];
			disp_masked_curve[2] = disp_masked_curve[2] + w[im][2] * mask_tex.z() * d_curve[im];
		}

		disp_masked_lerp[0] = disp_masked_lerp[0] / sum_w[0];
		disp_masked_lerp[1] = disp_masked_lerp[1] / sum_w[1];
		disp_masked_lerp[2] = disp_masked_lerp[2] / sum_w[2];

		disp_masked_curve[0] = disp_masked_curve[0] / sum_w[0];
		disp_masked_curve[1] = disp_masked_curve[1] / sum_w[1];
		disp_masked_curve[2] = disp_masked_curve[2] / sum_w[2];

		Eigen::Vector3f disp_lerp = Eigen::Vector3f(disp_masked_lerp[0] + disp_masked_lerp[1] + disp_masked_lerp[2]);
		Eigen::Vector3f disp_curve = Eigen::Vector3f(disp_masked_curve[0] + disp_masked_curve[1] + disp_masked_curve[2]);

		Eigen::Vector3f in_Vertex_lerp = Eigen::Vector3f(vbase_lerp + disp_lerp * displacement_multiplier);
		Eigen::Vector3f in_Vertex_curve = Eigen::Vector3f(vbase_curve + disp_curve * displacement_multiplier);

#if 0  // lerp
		vertex_output[iv * 3 + 0] = in_Vertex_lerp.x();
		vertex_output[iv * 3 + 1] = in_Vertex_lerp.y();
		vertex_output[iv * 3 + 2] = in_Vertex_lerp.z();
#else  // curve
		vertex_output[iv * 3 + 0] = in_Vertex_curve.x();
		vertex_output[iv * 3 + 1] = in_Vertex_curve.y();
		vertex_output[iv * 3 + 2] = in_Vertex_curve.z();
#endif
	}

	std::cout << "[Info] Vertex processing " << t.diff_sec_now() << "s" << std::endl;

	//
	// Output obj file
	//
	{
		tinyobj::scene_t output_obj;
		output_obj.attrib.vertices = vertex_output;
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

	std::cout << "[Info] Finished " << t.diff_sec_now() << "s" << std::endl;

	system("meshlab.exe c:/tmp/out.obj");

    return EXIT_SUCCESS;
}