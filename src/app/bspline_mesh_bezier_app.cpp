#include <bspline_surface.h>
#include <mesh_utils.h>
#include <bezier.h>
#include <timer.h>
#include <nanoflann_pointcloud.hpp>
#include <fstream>
#include <iostream>
#include <memory>



template <typename decimal_t>
void create_3d_control_lattice(
							TriMesh &grid, int m, int n, 
							int kdtree_count, int knn_search_checks,
							TriMesh &mesh)
{
	constexpr int dimension = 2;

	create_grid_mesh(grid, m, n);

	//
	// Collect uv data
	//
	timer tm_kdtree_build_query;
	nanoflann::PointCloud<decimal_t> samples;
	samples.pts.resize(mesh.n_vertices());
	size_t uv_index = 0;
	for (auto vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi, ++uv_index)
	{
		const auto& uv = mesh.point(*vi);
		samples.pts[uv_index].x = uv[0];
		samples.pts[uv_index].y = uv[1];
		samples.pts[uv_index].z = uv[2];
		//std::cout << std::fixed << "sample pts: " << samples.pts[uv_index].x << ' ' << samples.pts[uv_index].y << ' ' << samples.pts[uv_index].z << std::endl;
	}
	std::cout << std::endl;

	//
	// Construct a kd-tree index
	const nanoflann::pointcloud_adaptor_t<decimal_t>  pc2kd(samples); // The adaptor
	//
	// construct a kd-tree index:
	//
	nanoflann::kdtree_t<decimal_t, dimension> kdtree_index(dimension, pc2kd, nanoflann::KDTreeSingleIndexAdaptorParams(kdtree_count));
	kdtree_index.buildIndex();
	tm_kdtree_build_query.stop();
	//
	// build uv query array
	//
	timer tm_kdtree_search;
	for (auto vi = grid.vertices_begin(); vi != grid.vertices_end(); ++vi)
	{
		const auto& uv = grid.point(*vi);

		const uint32_t num_results = 1;
		uint32_t ret_index;
		decimal_t out_dist_sqr;
		nanoflann::KNNResultSet<decimal_t, uint32_t> resultSet(num_results);
		resultSet.init(&ret_index, &out_dist_sqr);

		kdtree_index.findNeighbors(resultSet, &uv[0], nanoflann::SearchParams(knn_search_checks));

		TriMesh::VertexHandle vi_mesh = mesh.vertex_handle(ret_index);
		const auto& pt_mesh = mesh.point(vi_mesh);
        
		grid.set_point(*vi, pt_mesh);
	}
	tm_kdtree_search.stop();

	

	std::cout
		<< std::fixed << "[Times in seconds]  \n"
		<< "KdTree Query     : " << tm_kdtree_build_query.diff_sec() << '\n'
		<< "KdTree Search    : " << tm_kdtree_search.diff_sec() << '\n';

}

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
        filename_append_before_extension(filename_in, argv[2]), "bspbezier");

    TriMesh mesh;
    timer tm_load_mesh;
    if (!load_mesh(mesh, filename_in))
    {
        std::cout << "Could not read " << filename_in << std::endl;
        return EXIT_FAILURE;
    }
    tm_load_mesh.stop();

    timer tm_copy_data_arrays;
    std::vector<decimal_t> x(mesh.n_vertices(), 0);
    std::vector<decimal_t> y(mesh.n_vertices(), 0);
    std::vector<decimal_t> z(mesh.n_vertices(), 0);
    std::vector<decimal_t> u_array(mesh.n_vertices(), 0);
    std::vector<decimal_t> v_array(mesh.n_vertices(), 0);
    // mesh_to_vecs(mesh, x, y, z);
    mesh_uv_to_vecs(mesh, x, y, z, u_array, v_array);
    tm_copy_data_arrays.stop();


    //
    // build knn
    //
    //constexpr int dimension = 2;
    //const int neighbours_count = (argc > 3) ? atoi(argv[3]) : 16;
    const int kdtree_count = (argc > 4) ? atoi(argv[3]) : 10;
    const int knn_search_checks = (argc > 5) ? atoi(argv[4]) : 16;

	//
	// build control lattice grid
	//
	timer tm_build_control_lattice;
	TriMesh grid;
	create_3d_control_lattice<decimal_t>(grid, m + 3 - 1, n + 3 - 1, 
		kdtree_count, knn_search_checks, mesh);
	tm_build_control_lattice.stop();


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

    

    timer tm_surf_compute;
    // surface::bspline_t<decimal_t> surf_x = {u_array.data(), v_array.data(), x.data(),
    //                                       mesh.n_vertices(), m, n};
    // surface::bspline_t<decimal_t> surf_y = {u_array.data(), v_array.data(), y.data(),
    //                                       mesh.n_vertices(), m, n};                                       
    surface::bspline_t<decimal_t> surf_z = {u_array.data(), v_array.data(), z.data(),
                                          mesh.n_vertices(), m, n};
    //
    // initialize phi matrix
    // it's needed to change the surf.compute function to accumulate the phi values
    //
#if 0      
    for (uint32_t i = 0; i < m + 3; ++i)
    {
        for (uint32_t j = 0; j < n + 3; ++j)
        {
			uint32_t grid_ind = i * (m + 3) + j;
            //surf_x.phi[i][j] = grid.point(grid.vertex_handle(grid_ind))[0];
            //surf_y.phi[i][j] = grid.point(grid.vertex_handle(grid_ind))[1];
            surf_z.phi[i][j] = grid.point(grid.vertex_handle(grid_ind))[2];
        }
    }
#endif    
    // surf_x.average_z = 0;
    // surf_y.average_z = 0;
    // surf_z.average_z = 0;
    //surf_x.compute();
    //surf_y.compute();
    surf_z.compute();
    tm_surf_compute.stop();
    //std::cout << "Error: " << surf_x.compute_error() << std::endl;
    //std::cout << "Error: " << surf_y.compute_error() << std::endl;
    std::cout << "Error: " << surf_z.compute_error() << std::endl;

    //
    // For each vertex, compute the surface value at uv
    // and interpolate (x,y)
    //
    timer tm_update_vertices;
    const decimal_t interval_normalization_factor_u = m * surf_z.urange_inv;
    const decimal_t interval_normalization_factor_v = n * surf_z.vrange_inv;
    for (size_t index = 0; index < mesh.n_vertices(); ++index)
    {
        TriMesh::VertexHandle vi = mesh.vertex_handle(index);
        const auto uv = mesh.texcoord2D(vi);
        auto point = mesh.point(vi);

        // BEZIER INTERPOLATION

        // Map to the half open domain Omega = [0,m) x [0,n)
        // The mapped u and v must be (strictly) less than m and n respectively
        decimal_t u = (uv[0] - surf_z.umin) * interval_normalization_factor_u;
        decimal_t v = (uv[1] - surf_z.vmin) * interval_normalization_factor_v;
        //
        // compute 4x4 neighborhood position
        //auto [i, j, s, t] = surf.compute_ijst(uv[0], uv[1]);
        auto [i, j, s, t] = surf_z.compute_ijst(u, v);
        //
        // interpolate (x,y)
        //
        //std::cout << std::fixed << '[' << i << ',' << j << "] [" << index << "]  v: [" << point << "] uv [" << uv << ']' << std::endl;
        for (auto pi = 0; pi < 3; ++pi)
        {
            decimal_t p[4][4];
            for (auto k = 0; k < 4; ++k)
            {
                for (auto l = 0; l < 4; ++l)
                {
                    //const auto grid_ind = (i + k) * (m + 3) + (j + l);
                    const auto grid_ind = (j + l) * (m + 3) + (i + k);
                    p[k][l] = grid.point(grid.vertex_handle(grid_ind))[pi];
                    //std::cout << p[k][l] << ' ';
                }
                //std::cout << std::endl;
            }
            //std::cout << "uv " << uv << "  bezier: " << surface::bezier::cubic<decimal_t>(uv[0], uv[1], p) << "\n\n";
            point[pi] = surface::bezier::cubic<decimal_t>(uv[0], uv[1], p);
        }
        mesh.set_point(vi, point);
    }
    tm_update_vertices.stop();

    timer tm_save_mesh;
    if (!save_mesh(mesh, filename_out))
    {
        std::cout << "Could not save " << filename_out << std::endl;
        return EXIT_FAILURE;
    }
    tm_save_mesh.stop();

    std::cout
        << std::fixed << "[Times in seconds]  \n"
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
