#pragma once

#include <mesh_utils.h>
#include <nanoflann_pointcloud.hpp>
#include <timer.h>


template <typename decimal_t, int dimension>
std::shared_ptr<nanoflann::kdtree_t<decimal_t, dimension>> create_kdtree_from_mesh(TriMesh &mesh,
                             int kdtree_count = 10)
{
    //
    // Collect data
    //
    nanoflann::PointCloud<decimal_t> samples;
    samples.pts.resize(mesh.n_vertices());
    size_t pt_index = 0;
    for (auto vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi, ++pt_index)
    {
        const auto &pt = mesh.point(*vi);
        samples.pts[pt_index].x = pt[0];
        samples.pts[pt_index].y = pt[1];
        samples.pts[pt_index].z = pt[2];
    }

    //
    // Construct a kd-tree index
    const nanoflann::pointcloud_adaptor_t<decimal_t> pc2kd(samples); // The adaptor
    //
    // construct a kd-tree index:
    //
    //nanoflann::kdtree_t<decimal_t, dimension> kdtree(dimension, pc2kd, nanoflann::KDTreeSingleIndexAdaptorParams(kdtree_count));
    //kdtree.buildIndex();
    auto kdtree = std::make_shared<nanoflann::kdtree_t<decimal_t, dimension>>(dimension, pc2kd, nanoflann::KDTreeSingleIndexAdaptorParams(kdtree_count));
    kdtree->buildIndex();
    
    return kdtree;
}


template <typename decimal_t, int dimension>
uint32_t find_closest_neighbor(const nanoflann::kdtree_t<decimal_t, dimension> &kdtree,
                               const TriMesh::Point &pt, int knn_search_checks = 16)
{
    const uint32_t num_results = 1;
    uint32_t ret_index;
    decimal_t out_dist_sqr;
    nanoflann::KNNResultSet<decimal_t, uint32_t> resultSet(num_results);
    resultSet.init(&ret_index, &out_dist_sqr);
    kdtree.findNeighbors(resultSet, &pt[0], nanoflann::SearchParams(knn_search_checks));
    return ret_index;
}

template <typename decimal_t, int dimension>
TriMesh::Point find_closest_neighbor(const nanoflann::kdtree_t<decimal_t, dimension> &kdtree,
                                     const TriMesh::Point &pt, const TriMesh &mesh,
                                     int knn_search_checks = 16)
{
    uint32_t ret_index = find_closest_neighbor(kdtree, pt, knn_search_checks);
    return mesh.point(mesh.vertex_handle(ret_index));
}


template <typename decimal_t>
void create_3d_control_lattice(TriMesh &grid, int m, int n, int kdtree_count, int knn_search_checks,
                               TriMesh &mesh)
{
    constexpr int dimension = 3;

    create_grid_mesh(grid, m, n);

    auto kdtree_ptr = create_kdtree_from_mesh<decimal_t, dimension>(mesh, kdtree_count);

    //
    // build uv query array
    //
    timer tm_kdtree_search;
    for (auto vi = grid.vertices_begin(); vi != grid.vertices_end(); ++vi)
    {
        const auto &pt = grid.point(*vi);
        const auto &pt_mesh = find_closest_neighbor(*kdtree_ptr, pt, mesh, knn_search_checks);
        grid.set_point(*vi, pt_mesh);
    }
    tm_kdtree_search.stop();
}