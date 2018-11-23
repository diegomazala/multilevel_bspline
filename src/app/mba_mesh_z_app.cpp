#include "bspline_mesh.h"
#include <bspline_surface.h>
#include <fstream>
#include <iostream>
#include <mba_surface.h>
#include <memory>

int main(int argc, char *argv[])
{

    using decimal_t = double;
    timer tm_total;

    if (argc < 4)
    {
        std::cout << "Usage: app <mesh_file> < m_n > < h >\n"
                  << "Usage: app ../data/face.obj 3 3 \n";
        return EXIT_FAILURE;
    }

    const std::string filename_in = argv[1];
    const uint32_t m = atoi(argv[2]);
    const uint32_t n = m;
    const uint32_t h = atoi(argv[3]);
    const bool scattered = static_cast<bool>((argc > 4) ? atoi(argv[4]) : false);
    const std::string filename_out = filename_append_before_extension(
        filename_append_before_extension(filename_append_before_extension(filename_in, argv[2]),
                                         argv[3]),
        "mba");

    TriMesh mesh;
    timer tm_load_mesh;
    if (!load_mesh(mesh, filename_in))
    {
        std::cout << "Could not read " << filename_in << std::endl;
        return EXIT_FAILURE;
    }
    tm_load_mesh.stop();

    //
    // build knn
    //
    const int kdtree_count = (argc > 4) ? atoi(argv[3]) : 10;
    const int knn_search_checks = (argc > 5) ? atoi(argv[4]) : 16;

    //
    // build control lattice grid
    //
    timer tm_build_control_lattice;
    const auto &control_points =
        compute_control_points<decimal_t>(mesh, n + 3, m + 3, kdtree_count, knn_search_checks);
    tm_build_control_lattice.stop();

    //
    // build data arrays
    //
    timer tm_copy_data_arrays;
    std::vector<decimal_t> x(mesh.n_vertices(), 0);
    std::vector<decimal_t> y(mesh.n_vertices(), 0);
    std::vector<decimal_t> z(mesh.n_vertices(), 0);
    mesh_to_vecs(mesh, x, y, z);
    tm_copy_data_arrays.stop();

    //
    // construct the surface function
    //
    timer tm_surf_compute;
    surface::multilevel_bspline_t<decimal_t> surf(x.data(), y.data(), z.data(), mesh.n_vertices(), m, n, h);

    //
    // saving control points for visualization
    //
    // {
    //     const std::string filename_ctr_pts = filename_append_before_extension(
    //         filename_append_before_extension(filename_in, argv[2]), "ctrl_pts");
    //     save_points_obj(control_points, filename_ctr_pts);
    // }

    //
    // compute the surface function
    //
    if (scattered)
    {
        //
        // using scattered data as described by Lee-1997
        //
        surf.compute();
    }
    else
    {
        //
        // using closest points in original mesh
        //
        std::vector<decimal_t> z(control_points.size());
        for (size_t i = 0; i < control_points.size(); ++i)
        {
            z[i] = control_points[i][2];
        }
        surf.compute_with_control_points(z);
    }
    tm_surf_compute.stop();

#if 1
    timer tm_updating_save_mesh;
    for (size_t k = 0; k <= h; ++k)
    {
        //
        // For each vertex, compute the surface value at uv
        // and interpolate (x,y)
        //
        timer tm_update_vertices;
        for (size_t index = 0; index < mesh.n_vertices(); ++index)
        {
            TriMesh::VertexHandle vi = mesh.vertex_handle(index);
            //const auto uv = mesh.texcoord2D(vi);
            auto point_mesh = mesh.point(vi);
            auto point_out = point_mesh;

            // for (auto p = 0; p < 3; ++p)
            // {
            //     point_out[p] = surf[p](uv[0], uv[1], k);
            // }

            point_out[2] = surf(point_mesh[0], point_mesh[1], k);

            mesh.set_point(vi, point_out);
        }
        tm_update_vertices.stop();
        //
        // Save output mesh
        //
        timer tm_save_mesh;
        const std::string filename =
            filename_append_before_extension(filename_out, std::to_string(k));
        if (!save_mesh(mesh, filename))
        {
            std::cout << "Could not save " << filename << std::endl;
            return EXIT_FAILURE;
        }
        tm_save_mesh.stop();
    }
    tm_updating_save_mesh.stop();
#else
    //
    // For each vertex, compute the surface value at uv
    // and interpolate (x,y)
    //
    timer tm_update_vertices;
    for (size_t index = 0; index < mesh.n_vertices(); ++index)
    {
        TriMesh::VertexHandle vi = mesh.vertex_handle(index);
        const auto uv = mesh.texcoord2D(vi);
        auto point_mesh = mesh.point(vi);
        auto point_out = point_mesh;

        for (auto p = 0; p < 3; ++p)
        {
            point_out[p] = surf[p](uv[0], uv[1]);
        }

        mesh.set_point(vi, point_out);
    }
    tm_update_vertices.stop();

    //
    // Save output mesh
    //
    timer tm_save_mesh;
    if (!save_mesh(mesh, filename_out))
    {
        std::cout << "Could not save " << filename_out << std::endl;
        return EXIT_FAILURE;
    }
    tm_save_mesh.stop();
#endif

    //
    // Print time info
    //
    std::cout << std::fixed << "[Times in seconds]  \n"
              << "Loading Mesh     : " << tm_load_mesh.diff_sec() << '\n'
              << "Copying Arrays   : " << tm_copy_data_arrays.diff_sec() << '\n'
              << "Surface Computing: " << tm_surf_compute.diff_sec() << '\n'
              << "Building Ctrl Pts: " << tm_build_control_lattice.diff_sec() << '\n'
              //<< "Saving Mesh      : " << tm_save_mesh.diff_sec() << '\n'
              << "Total time       : " << tm_total.diff_sec_now() << '\n'
              << std::endl;

    return EXIT_SUCCESS;
}
