#include <filesystem>
#include <fstream>
#include <thread>
#include <timer.h>
namespace fs = std::filesystem;

#if _MSC_VER
#pragma warning(push, 0) // suppressing warnings for OpenMesh
#endif
#define _USE_MATH_DEFINES
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#if _MSC_VER
#pragma warning(pop)
#endif

using decimal_t = float;
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
typedef OpenMesh::TriMesh_ArrayKernelT<> TriMesh;
typedef OpenMesh::PolyMesh_ArrayKernelT<> Mesh_t;

#include <embree3/rtcore.h>
#include <embree3/rtcore_ray.h>
struct Vertex
{
    float x, y, z, r;
};
struct Triangle
{
    int v0, v1, v2;
};
struct Quad
{
	int v0, v1, v2, v3;
};

void error_handler(const RTCError code, const char *str)
{
    if (code == RTC_ERROR_NONE)
        return;

    printf("Embree: ");
    switch (code)
    {
    case RTC_ERROR_UNKNOWN:
        printf("RTC_ERROR_UNKNOWN");
        break;
    case RTC_ERROR_INVALID_ARGUMENT:
        printf("RTC_ERROR_INVALID_ARGUMENT");
        break;
    case RTC_ERROR_INVALID_OPERATION:
        printf("RTC_ERROR_INVALID_OPERATION");
        break;
    case RTC_ERROR_OUT_OF_MEMORY:
        printf("RTC_ERROR_OUT_OF_MEMORY");
        break;
    case RTC_ERROR_UNSUPPORTED_CPU:
        printf("RTC_ERROR_UNSUPPORTED_CPU");
        break;
    case RTC_ERROR_CANCELLED:
        printf("RTC_ERROR_CANCELLED");
        break;
    default:
        printf("invalid error code");
        break;
    }
    if (str)
    {
        printf(" (");
        while (*str)
            putchar(*str++);
        printf(")\n");
    }
    exit(1);
}

template <typename mesh_t> bool save_points_obj(const mesh_t &mesh, const std::string &filename)
{
    try
    {
        std::cout << "-- Saving points " << filename << std::endl;
        std::ofstream out(filename, std::ios::out);
        for (auto vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi)
        {
            out << std::fixed << "v " << mesh.point(*vi) << '\n';
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

template <typename mesh_t>
static void compute_vertex_normal(mesh_t &mesh)
{
    if (!mesh.has_vertex_normals())
        mesh.request_vertex_normals();

    if (!mesh.has_face_normals())
        mesh.request_face_normals();

    mesh.update_vertex_normals();
    mesh.update_face_normals();

	mesh_t::VertexIter v_it, v_end(mesh.vertices_end());
	mesh_t::VertexFaceIter vf_it;
	mesh_t::Normal tmp;
    mesh_t::Scalar count;
    for (v_it = mesh.vertices_begin(); v_it != v_end; ++v_it)
    {
        tmp[0] = tmp[1] = tmp[2] = 0.0;
        count = 0.0;
        for (vf_it = mesh.vf_iter(*v_it); vf_it.is_valid(); ++vf_it)
        {
            tmp += mesh.calc_face_normal(*vf_it);
            ++count;
        }
        mesh.set_normal(*v_it, tmp / count);
    }
}

template <typename mesh_t>
void line_mesh_intersection(mesh_t &mesh_base, mesh_t &mesh_target)
{
	bool triangle_mesh = mesh_base.valence(*mesh_base.faces_begin()) == 3;

    RTCDevice device = nullptr;
    RTCScene scene = nullptr;

    device = rtcNewDevice(nullptr);
    error_handler(rtcGetDeviceError(device), "Fail creating device");

    scene = rtcNewScene(device);
    rtcSetSceneFlags(scene, RTC_SCENE_FLAG_ROBUST);
    error_handler(rtcGetDeviceError(device), "Fail creating scene");

    timer tm_build_mesh;
    {
        RTCGeometry mesh_embree = rtcNewGeometry(device, (triangle_mesh ? RTC_GEOMETRY_TYPE_TRIANGLE : RTC_GEOMETRY_TYPE_QUAD));

        // add vertices
        Vertex *vertices = reinterpret_cast<Vertex *>(
            rtcSetNewGeometryBuffer(mesh_embree, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3,
                                    sizeof(Vertex), mesh_target.n_vertices()));
        std::size_t vInd = 0;
        for (auto vi = mesh_target.vertices_begin(); vi != mesh_target.vertices_end(); ++vi, ++vInd)
        {
            std::memcpy(&vertices[vInd], mesh_target.point(*vi).data(), sizeof(float) * 3);
        }

		// add faces
		if (triangle_mesh)
		{
			Triangle *triangles = reinterpret_cast<Triangle *>(
				rtcSetNewGeometryBuffer(mesh_embree, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3,
					sizeof(Triangle), mesh_target.n_faces()));
			std::size_t fInd = 0;
			for (auto fi = mesh_target.faces_begin(); fi != mesh_target.faces_end(); ++fi, ++fInd)
			{
				auto fv_it = mesh_target.fv_iter(*fi);
				triangles[fInd].v0 = fv_it->idx();
				fv_it++;
				triangles[fInd].v1 = fv_it->idx();
				fv_it++;
				triangles[fInd].v2 = fv_it->idx();
			}
		}
		else
		{
			auto n_faces = mesh_target.n_faces();
			Quad *quads = reinterpret_cast<Quad *>(
				rtcSetNewGeometryBuffer(mesh_embree, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT4,
					sizeof(Quad), mesh_target.n_faces()));
			std::size_t fInd = 0;
			for (auto fi = mesh_target.faces_begin(); fi != mesh_target.faces_end(); ++fi, ++fInd)
			{
				auto fv_it = mesh_target.fv_iter(*fi);
				quads[fInd].v0 = fv_it->idx();
				fv_it++;
				quads[fInd].v1 = fv_it->idx();
				fv_it++;
				quads[fInd].v2 = fv_it->idx();
				fv_it++;
				quads[fInd].v3 = fv_it->idx();
			}
		}

        rtcCommitGeometry(mesh_embree);
        rtcAttachGeometry(scene, mesh_embree);
        rtcReleaseGeometry(mesh_embree);
    }

    rtcCommitScene(scene);
    error_handler(rtcGetDeviceError(device), "Fail committing scene");

    tm_build_mesh.stop();

    RTCIntersectContext context;
    rtcInitIntersectContext(&context);

    timer ray_intersect;
    for (auto vi = mesh_base.vertices_begin(); vi != mesh_base.vertices_end(); ++vi)
    {
        std::cout << vi->idx() << '/' << mesh_base.n_vertices() << '\r';
        auto &point = mesh_base.point(*vi);
        const auto &normal = mesh_base.normal(*vi);

        RTCRayHit rayhit[2];
        for (auto &rh : rayhit)
        {
            auto &ray = rh.ray;
            auto &hit = rh.hit;
            ray.org_x = point[0];
            ray.org_y = point[1];
            ray.org_z = point[2];
            hit.geomID = RTC_INVALID_GEOMETRY_ID;
            ray.dir_x = normal[0];
            ray.dir_y = normal[1];
            ray.dir_z = normal[2];
            ray.tnear = 0.0f;
            ray.tfar = 1.0f;
            ray.time = 0.0f;
            ray.id = RTC_INVALID_GEOMETRY_ID;
        }
        // invert ray
        rayhit[1].ray.dir_x = -rayhit[1].ray.dir_x;
        rayhit[1].ray.dir_y = -rayhit[1].ray.dir_y;
        rayhit[1].ray.dir_z = -rayhit[1].ray.dir_z;

        rtcIntersect1(scene, &context, &rayhit[0]);
        rtcIntersect1(scene, &context, &rayhit[1]);

        // Booth rays hit
        if (rayhit[0].hit.geomID != RTC_INVALID_GEOMETRY_ID &&
            rayhit[1].hit.geomID != RTC_INVALID_GEOMETRY_ID)
        {
            if (rayhit[0].ray.tfar < rayhit[1].ray.tfar)
            {
                point[2] = rayhit[0].ray.tfar;
            }
            else
            {
                point[2] = -rayhit[1].ray.tfar;
            }
        }
        // Only first ray hit
        else if (rayhit[0].hit.geomID != RTC_INVALID_GEOMETRY_ID)
        {
            point[2] = rayhit[0].ray.tfar;
        }
        // Only second ray hit
        else if (rayhit[1].hit.geomID != RTC_INVALID_GEOMETRY_ID)
        {
            point[2] = -rayhit[1].ray.tfar;
        }
        // No hit
        else
        {
            point[2] = 0.0f;
        }


        mesh_base.set_point(*vi, point);

        // std::cout << ray.tnear << ' ' << ray.tfar << ' ' << ((hit.geomID ==
        // RTC_INVALID_GEOMETRY_ID) ? "RTC_INVALID_GEOMETRY_ID" : "HIT OK") << std::endl;
    }
    ray_intersect.stop();

    std::cout << std::fixed << std::endl
              << "Time in seconds intersection code:" << std::endl
              << "Building Meshes: " << tm_build_mesh.diff_sec() << std::endl
              << "Ray Intersect  : " << ray_intersect.diff_sec() << std::endl
              << std::endl;
}

int main(int argc, char *argv[])
{
    timer tm_total;

    if (argc < 3)
    {
        std::cout << "Usage: app <target_mesh_file> <surface_mesh_file> <output_file>\n"
                  << "Usage: app ../data/face_bsp.obj face.obj \n";
        return EXIT_FAILURE;
    }

    const std::string filename_mesh_target = argv[1];
    const std::string filename_mesh_surface = argv[2];
    const std::string filename_out = ((argc > 3) ? argv[3] :
        fs::path(filename_mesh_surface).replace_extension("cdm.obj").string());

    timer tm_load_mesh;
    Mesh_t mesh_surface, mesh_target;
    OpenMesh::IO::Options mesh_surface_io_opt;
    OpenMesh::IO::Options mesh_target_io_opt(OpenMesh::IO::Options::VertexTexCoord);

    std::vector<std::thread> thread;
    try
    {
        std::cout << "Loading: " << filename_mesh_target << '\n';
        mesh_target.request_vertex_texcoords2D();
        thread.push_back(std::thread([&] {
            OpenMesh::IO::read_mesh(mesh_target, filename_mesh_target, mesh_target_io_opt);
        }));

        std::cout << "Loading: " << filename_mesh_surface << '\n';
        thread.push_back(std::thread([&] {
            OpenMesh::IO::read_mesh(mesh_surface, filename_mesh_surface, mesh_surface_io_opt);
        }));
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    for (auto &t : thread)
        t.join();

    tm_load_mesh.stop();

    // if (!mesh_target_io_opt.check(OpenMesh::IO::Options::VertexTexCoord))
    // {
    //     std::cerr << "Error: Base mesh must have uv coords" << std::endl;
    //     return EXIT_FAILURE;
    // }

    if (!mesh_surface_io_opt.check(OpenMesh::IO::Options::VertexNormal))
    {
        std::cout << "mesh_surface does not have normals" << std::endl;
        mesh_surface.request_vertex_normals();
        mesh_surface.request_face_normals();
        compute_vertex_normal(mesh_surface);
    }

	timer tm_intersection;
    line_mesh_intersection(mesh_surface, mesh_target);
	tm_intersection.stop();

	
	std::cout << "Saving: " << filename_out << '\n';
	timer tm_saving;
    save_points_obj(mesh_surface, filename_out);
	tm_saving.stop();
    // if (!OpenMesh::IO::write_mesh(mesh_surface, filename_out))
    // {
    // 	std::cerr << "Error: cannot write mesh to " << filename_out << std::endl;
    // 	return false;
    // }

    tm_total.stop();
    tm_load_mesh.print_interval("Loading meshes  : ");
	tm_intersection.print_interval("Intersection    : ");
	tm_saving.print_interval("Saving meshes   : ");
    tm_total.print_interval("Total time      : ");
}
