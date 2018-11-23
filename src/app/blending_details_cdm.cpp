#include <mesh_utils.h>
#include <timer.h>


int main(int argc, char *argv[])
{
    using decimal_t = double;
    timer tm_total;

    if (argc < 3)
    {
        std::cout << "Usage: app <Sd_mesh_file> <scalar_normal_mesh_file>\n"
                  << "Usage: app \
                  ../Figs/mba_2_10/fig02_2_10_mba_3.obj \
                  ../Figs/mba_2_10/raycast_normal/fig02_2_10_mba_3.cdm.obj \
                  ../Figs/fig_2_3_nor_cdm.obj \n";
        return EXIT_FAILURE;
    }

    const std::string filename_Sd = argv[1];
    const std::string filename_cdm = argv[2];
    const std::string filename_out = argv[3];

    TriMesh mesh_sd, mesh_cdm;
    OpenMesh::IO::Options mesh_sd_io_opt(OpenMesh::IO::Options::VertexNormal);
    OpenMesh::IO::Options mesh_cdm_io_opt;
    

    timer tm_load_mesh;
    std::vector<std::thread> thread;
    try
    {
        std::cout << "Loading: " << filename_cdm << '\n';
        thread.push_back(std::thread([&] 
        {
            OpenMesh::IO::read_mesh(mesh_sd, filename_Sd, mesh_sd_io_opt);

            // if (!mesh_sd_io_opt.check(OpenMesh::IO::Options::VertexNormal))
            // {
            //     std::cout << "mesh_sd does not have normals" << std::endl;
            //     mesh_sd.request_vertex_normals();
            //     mesh_sd.request_face_normals();
            //     compute_vertex_normal(mesh_sd);
            // }
        }));

        std::cout << "Loading: " << filename_Sd << '\n';
        thread.push_back(std::thread([&] 
        {
            OpenMesh::IO::read_mesh(mesh_cdm, filename_cdm, mesh_cdm_io_opt);
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


    auto vit_cdm = mesh_cdm.vertices_begin();
    for (auto vi = mesh_sd.vertices_begin(); vi != mesh_sd.vertices_end(); ++vi, ++vit_cdm)
    {
        auto& point = mesh_sd.point(*vi);
        const auto& normal = mesh_sd.normal(*vi);
        const decimal_t cdm = mesh_cdm.point(*vit_cdm)[2];
        point = point + normal * cdm;
        
        mesh_sd.set_point(*vi, point);
    }

    //
    // Save output mesh
    //
    timer tm_save_mesh;
    if (!save_mesh(mesh_sd, filename_out))
    {
        std::cout << "Could not save " << filename_out << std::endl;
        return EXIT_FAILURE;
    }
    tm_save_mesh.stop();


    return EXIT_SUCCESS;
}
