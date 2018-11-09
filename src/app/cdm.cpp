#include <experimental/filesystem>
#include <fstream>
#include <thread>
#include <timer.h>
namespace fs = std::experimental::filesystem;

#include "raytri.h"

#if _MSC_VER
#pragma warning(push, 0) // supressing warnings for OpenMesh
#endif
#define _USE_MATH_DEFINES
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#if _MSC_VER
#pragma warning(pop)
#endif

using decimal_t = float;
typedef OpenMesh::PolyMesh_ArrayKernelT<OpenMesh::DefaultTraits> MyMesh;
// using Mesh_t = OpenMesh::PolyMesh_ArrayKernelT<>;
// typedef OpenMesh::PolyMesh_ArrayKernelT<> PolyMesh;

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

static void compute_vertex_normal(MyMesh &mesh)
{
    if (!mesh.has_vertex_normals())
        mesh.request_vertex_normals();

    if (!mesh.has_face_normals())
        mesh.request_face_normals();

    mesh.update_vertex_normals();
    mesh.update_face_normals();

    MyMesh::VertexIter v_it, v_end(mesh.vertices_end());
    MyMesh::VertexFaceIter vf_it;
    MyMesh::Normal tmp;
    MyMesh::Scalar count;
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

int main(int argc, char *argv[])
{
    timer tm_total;

    if (argc != 3)
    {
        std::cout << "Usage: app <surface_mesh_file> <base_mesh_file>\n"
                  << "Usage: app ../data/face_bsp.obj face.obj \n";
        return EXIT_FAILURE;
    }

    const std::string filename_base_mesh = argv[1];
    const std::string filename_surface_mesh = argv[2];
    const std::string filename_out =
        fs::path(filename_surface_mesh).replace_extension("pts.obj").string();

    timer tm_load_mesh;
    MyMesh surface_mesh, base_mesh;
    OpenMesh::IO::Options surface_mesh_io_opt;
    OpenMesh::IO::Options base_mesh_io_opt(OpenMesh::IO::Options::VertexTexCoord);

    std::vector<std::thread> thread;
    try
    {
        std::cout << "Loading: " << filename_base_mesh << '\n';
        base_mesh.request_vertex_texcoords2D();
        thread.push_back(std::thread([&] { 
            OpenMesh::IO::read_mesh(base_mesh, filename_base_mesh, base_mesh_io_opt); 
        }));

        std::cout << "Loading: " << filename_surface_mesh << '\n';
        thread.push_back(std::thread([&] {
            OpenMesh::IO::read_mesh(surface_mesh, filename_surface_mesh, surface_mesh_io_opt);
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

    if (!base_mesh_io_opt.check(OpenMesh::IO::Options::VertexTexCoord))
    {
        std::cerr << "Error: Base mesh must have uv coords" << std::endl;
        return EXIT_FAILURE;
    }
    
    

    if (!surface_mesh_io_opt.check(OpenMesh::IO::Options::VertexNormal))
    {
        std::cout << "surface_mesh does not have normals" << std::endl;
        surface_mesh.request_vertex_normals();
        surface_mesh.request_face_normals();
        compute_vertex_normal(surface_mesh);
    }

    size_t n_intersections = 0;
    auto vi_base = surface_mesh.vertices_begin();
    for (auto vi_surf = surface_mesh.vertices_begin(); 
        vi_surf != surface_mesh.vertices_end();
        ++vi_surf, ++vi_base)
    {
        //std::cout << count++ << '/' << surface_mesh.n_vertices() << '\n';
        
        //for (auto fh = base_mesh.faces_begin(); fh != base_mesh.faces_end(); ++fh)
        
        bool has_intersection = false;
        auto fh = base_mesh.faces_begin();
        while(fh != base_mesh.faces_end() && !has_intersection)
        {
            const auto uv = base_mesh.texcoord2D(*vi_base);
            const auto surf_point = surface_mesh.point(*vi_surf);
            const auto surf_normal = surface_mesh.normal(*vi_surf);
            const Vec3f orig(surf_point[0], surf_point[1], surf_point[2]);
            const Vec3f dir(surf_normal[0], surf_normal[1], surf_normal[2]);

            surface_mesh.set_point(*vi_surf, {uv[0], uv[1], surf_point[2]});
            MyMesh::FaceVertexIter fv_it = base_mesh.fv_iter(*fh);
            if (base_mesh.valence(*fh) == 3) // triangle
            {
                auto v0 = base_mesh.point(*fv_it);
                auto v1 = base_mesh.point(*(++fv_it));
                auto v2 = base_mesh.point(*(++fv_it));
                decimal_t t, u, v;
                if (rayTriangleIntersect(orig, dir, 
                    {v0[0], v0[1], v0[2]}, 
                    {v1[0], v1[1], v1[2]},
                    {v2[0], v2[1], v2[2]}, 
                    t, u, v))
                {
                    has_intersection = true;
                    n_intersections++;
                    surface_mesh.set_point(*vi_surf, {surf_point[0], surf_point[1], t});
                    std::cout << "Vertex intersect " << vi_surf->idx() << '\t' << fh->idx() << std::endl;
                }
            }
            else // quad
            {
                std::cout << "Malha quad????" << std::endl;
                auto v0 = base_mesh.point(*fv_it);
                auto v1 = base_mesh.point(*(++fv_it));
                auto v2 = base_mesh.point(*(++fv_it));
                auto v3 = base_mesh.point(*(++fv_it));
                decimal_t t, u, v;
                
                if (rayTriangleIntersect(orig, dir, 
                    {v0[0], v0[1], v0[2]}, 
                    {v1[0], v1[1], v1[2]},
                    {v2[0], v2[1], v2[2]}, 
                    t, u, v))
                {
                    has_intersection = true;
                    surface_mesh.set_point(*vi_surf, {surf_point[0], surf_point[1], t});
                }
                else if (rayTriangleIntersect(orig, dir, 
                    {v1[0], v1[1], v1[2]},
                    {v2[0], v2[1], v2[2]}, 
                    {v3[0], v3[1], v3[2]}, 
                    t, u, v))
                {
                    has_intersection = true;
                    surface_mesh.set_point(*vi_surf, {surf_point[0], surf_point[1], t});
                }
            }

            ++fh;
        }

        if (!has_intersection)
        {
            std::cout << "Vertex NOT intersect " << vi_surf->idx() << '\t' << surface_mesh.point(*vi_surf) << std::endl;
        }
    }

    std::cout << "N intersections: " << n_intersections << std::endl;

    save_points_obj(surface_mesh, filename_out);
    // if (!OpenMesh::IO::write_mesh(surface_mesh, filename_out))
	// {
	// 	std::cerr << "Error: cannot write mesh to " << filename_out << std::endl;
	// 	return false;
	// }

    tm_total.stop();
    tm_load_mesh.print_interval("Loading meshes  : ");
    tm_total.print_interval("Total time      : ");
}
