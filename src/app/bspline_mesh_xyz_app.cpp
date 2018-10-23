#include "bspline_mesh.h"
#include <bspline_surface.h>
#include <fstream>
#include <iostream>
#include <memory>

#define COMPUTE_SURF_GRID 0

int main(int argc, char *argv[])
{

    using decimal_t = double;
    timer tm_total;

    if (argc < 3)
    {
        std::cout << "Usage: app <mesh_file> < m_n >\n"
                  << "Usage: app ../data/face.obj 3 \n";
        return EXIT_FAILURE;
    }

    const std::string filename_in = argv[1];
    const uint32_t m = atoi(argv[2]);
    const uint32_t n = m;
    const std::string filename_out = filename_append_before_extension(
        filename_append_before_extension(filename_in, argv[2]), "bspxyz_[mesh_xyz]");

    TriMesh mesh_source;
    timer tm_load_mesh;
    if (!load_mesh(mesh_source, filename_in))
    {
        std::cout << "Could not read " << filename_in << std::endl;
        return EXIT_FAILURE;
    }
    tm_load_mesh.stop();

    //
    // build knn
    //
    //constexpr int dimension = 3;
    // const int neighbours_count = (argc > 3) ? atoi(argv[3]) : 16;
    const int kdtree_count = (argc > 4) ? atoi(argv[3]) : 10;
    const int knn_search_checks = (argc > 5) ? atoi(argv[4]) : 16;

    //
    // build control lattice grid
    //
    timer tm_build_control_lattice;
    TriMesh grid;
    create_3d_control_lattice<decimal_t>(grid, m + 3 - 1, n + 3 - 1, kdtree_count,
                                         knn_search_checks, mesh_source);
    tm_build_control_lattice.stop();

    //
    // save the grid (control points)
    //
    timer tm_save_control_lattice;
    {
        const std::string filename_pts = filename_append_before_extension(
            filename_append_before_extension(filename_in, argv[2]), "pts");
        if (!save_points_obj(grid, filename_pts))
        {
            std::cout << "Could not save control_lattice mesh " << filename_pts << std::endl;
            return EXIT_FAILURE;
        }
    }
    tm_save_control_lattice.stop();
    //exit(0);

#if 0
    //
    // Compute kdtree of the grid
    //
    nanoflann::PointCloud<decimal_t> samples;
    samples.pts.resize(grid.n_vertices());
    size_t pt_index = 0;
    for (auto vi = grid.vertices_begin(); vi != grid.vertices_end(); ++vi, ++pt_index)
    {
        const auto &pt = grid.point(*vi);
        samples.pts[pt_index].x = pt[0];
        samples.pts[pt_index].y = pt[1];
        samples.pts[pt_index].z = pt[2];
    }
    const nanoflann::pointcloud_adaptor_t<decimal_t> pc2kd(samples); // The adaptor
    nanoflann::kdtree_t<decimal_t, dimension> kdtree_grid(
        dimension, pc2kd, nanoflann::KDTreeSingleIndexAdaptorParams(kdtree_count));
    kdtree_grid.buildIndex();
#endif

#if COMPUTE_SURF_GRID
    TriMesh &mesh = grid;
#else
    TriMesh &mesh = mesh_source;
#endif


    //
    // build data arrays
    //
    timer tm_copy_data_arrays;
    std::vector<decimal_t> x(mesh.n_vertices(), 0);
    std::vector<decimal_t> y(mesh.n_vertices(), 0);
    std::vector<decimal_t> z(mesh.n_vertices(), 0);
    std::vector<decimal_t> u_array(mesh.n_vertices(), 0);
    std::vector<decimal_t> v_array(mesh.n_vertices(), 0);
    mesh_uv_to_vecs(mesh, x, y, z, u_array, v_array);
    tm_copy_data_arrays.stop();

    //
    // construct the surface function
    //
    timer tm_surf_compute;
    std::array<surface::bspline_t<decimal_t>, 3> surf{
        {{u_array.data(), v_array.data(), x.data(), mesh.n_vertices(), m, n},
         {u_array.data(), v_array.data(), y.data(), mesh.n_vertices(), m, n},
         {u_array.data(), v_array.data(), z.data(), mesh.n_vertices(), m, n}}};

    //
    // compute the surface function
    //
#if 0
    for (auto &s : surf)
    {
        s.average_z = 0;
        s.compute();
        // compute_error() is just for debug purposes
        std::cout << std::fixed << "Error            : " << s.compute_error() << std::endl;
    }
    tm_surf_compute.stop();
#else



    uint32_t ind = 0;
    for (uint32_t i = 0; i < (m + 3); ++i)
    {
        for (uint32_t j = 0; j < (n + 3); ++j)
        {
            TriMesh::VertexHandle vi = grid.vertex_handle(ind);
            auto pt_grid = grid.point(vi);
            surf[0].phi[j][i] = pt_grid[0];
            surf[1].phi[j][i] = pt_grid[1];
            surf[2].phi[j][i] = pt_grid[2];
            ind++;
        }
    }

    //
    // Saving phi matrix
    //
    const auto file_out = filename_append_before_extension(filename_out, "phi");
    std::cout << "-- Saving points " << file_out << std::endl;
    std::ofstream out(file_out, std::ios::out);
    for (uint32_t i = 0; i < (m + 3); ++i)
    {
        for (uint32_t j = 0; j < (n + 3); ++j)
        {
            out << std::fixed << "v " << surf[0].phi[i][j] << ' ' << surf[1].phi[i][j] << ' ' << surf[2].phi[i][j] << '\n';
        }
    }
    out.close();
    
#endif

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




    //
    // Print time info
    //
    std::cout << std::fixed << "[Times in seconds]  \n"
              << "Loading Mesh     : " << tm_load_mesh.diff_sec() << '\n'
              << "Copying Arrays   : " << tm_copy_data_arrays.diff_sec() << '\n'
              << "Surface Computing: " << tm_surf_compute.diff_sec() << '\n'
              << "Building Ctrl Pts: " << tm_build_control_lattice.diff_sec() << '\n'
              << "Update Vertices  : " << tm_update_vertices.diff_sec() << '\n'
              << "Saving Ctrl Pts  : " << tm_save_control_lattice.diff_sec() << '\n'
              << "Saving Mesh      : " << tm_save_mesh.diff_sec() << '\n'
              << "Total time       : " << tm_total.diff_sec_now() << '\n'
              << std::endl;

    return EXIT_SUCCESS;
}
