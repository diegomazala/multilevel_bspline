#pragma once

#define USE_GRID_CLOSEST_Z_ONLY 0

#include <mesh_utils.h>
#include <nanoflann_pointcloud.hpp>
#include <timer.h>


template <typename decimal_t>
void create_3d_control_lattice(TriMesh &grid, int m, int n, int kdtree_count, int knn_search_checks,
                               TriMesh &mesh)
{
    // if set to 3, it will present wrong z coords for the neighbors
    // I don't know the reason. I have to check
    constexpr int dimension = 2;

    create_grid_mesh(grid, m, n);

    //
    // Collect data
    //
    nanoflann::PointCloud<decimal_t> samples;
    samples.pts.resize(mesh.n_vertices());
    size_t pt_index = 0;
    for (auto vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi, ++pt_index)
    {
        const auto &uv = mesh.texcoord2D(*vi);
        samples.pts[pt_index].x = uv[0];
        samples.pts[pt_index].y = uv[1];
        samples.pts[pt_index].z = 0;
    }

    //
    // Construct a kd-tree index
    const nanoflann::pointcloud_adaptor_t<decimal_t> pc2kd(samples); // The adaptor
    //
    // construct a kd-tree index:
    //
    nanoflann::kdtree_t<decimal_t, dimension> kdtree(
        dimension, pc2kd, nanoflann::KDTreeSingleIndexAdaptorParams(kdtree_count));
    kdtree.buildIndex();


    //
    // build uv query array
    //
    timer tm_kdtree_search;
    for (auto vi = grid.vertices_begin(); vi != grid.vertices_end(); ++vi)
    {
        auto uv = grid.texcoord2D(*vi);
        //auto uv = grid.point(*vi);
        const uint32_t num_results = 1;
        uint32_t ret_index;
        decimal_t out_dist_sqr;
        nanoflann::KNNResultSet<decimal_t, uint32_t> resultSet(num_results);
        resultSet.init(&ret_index, &out_dist_sqr);
        kdtree.findNeighbors(resultSet, &uv[0], nanoflann::SearchParams(knn_search_checks));
        const auto &pt_mesh = mesh.point(mesh.vertex_handle(ret_index));

#if USE_GRID_CLOSEST_Z_ONLY // apply only z coord
        pt[2] = pt_mesh[2];
        grid.set_point(*vi, pt);
#else
        grid.set_point(*vi, pt_mesh);
#endif
    }
    tm_kdtree_search.stop();
}




template <typename decimal_t>
std::vector<std::array<decimal_t, 3>>
compute_control_points(const TriMesh &mesh, size_t rows, size_t cols, int kdtree_count = 10, int knn_search_checks = 16)
{
    constexpr int dimension = 2;

    //
    // Collect data
    //
    nanoflann::PointCloud<decimal_t> samples;
    samples.pts.resize(mesh.n_vertices());
    size_t pt_index = 0;
    for (auto vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi, ++pt_index)
    {
        const auto &uv = mesh.texcoord2D(*vi);
        samples.pts[pt_index].x = uv[0];
        samples.pts[pt_index].y = uv[1];
        samples.pts[pt_index].z = 0;
    }

    //
    // Construct a kd-tree index
    const nanoflann::pointcloud_adaptor_t<decimal_t> pc2kd(samples); // The adaptor
    //
    // construct a kd-tree index:
    //
    nanoflann::kdtree_t<decimal_t, dimension> kdtree(
        dimension, pc2kd, nanoflann::KDTreeSingleIndexAdaptorParams(kdtree_count));
    kdtree.buildIndex();

    std::vector<std::array<decimal_t, 3>> control_points(rows * cols);

    size_t index = 0;
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < cols; ++j)
        {
            decimal_t uv[2] = {static_cast<decimal_t>(j) / (cols - 1),
                               static_cast<decimal_t>(i) / (rows - 1)};

            const uint32_t num_results = 1;
            uint32_t ret_index;
            decimal_t out_dist_sqr;
            nanoflann::KNNResultSet<decimal_t, uint32_t> resultSet(num_results);
            resultSet.init(&ret_index, &out_dist_sqr);
            kdtree.findNeighbors(resultSet, &uv[0], nanoflann::SearchParams(knn_search_checks));
            const auto &pt_mesh = mesh.point(mesh.vertex_handle(ret_index));
            //const auto index = i * cols + j;
            control_points[index][0] = pt_mesh[0];
            control_points[index][1] = pt_mesh[1];
            control_points[index][2] = pt_mesh[2];
            index++;
        }
    }
    
    return control_points;
}