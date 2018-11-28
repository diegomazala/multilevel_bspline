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

    const std::string filename_Sd = 
    //"C:/Users/diego/Projects/DSc/data/Figs/sd/sd_3.obj";
    argv[1];
    const std::string filename_cdm = 
    //"C:/Users/diego/Projects/DSc/data/Figs/sd/cdm_3.obj";
    argv[2];
    const std::string filename_out = 
    //"C:/Users/diego/Projects/DSc/data/Figs/sd/out_3.obj";
    argv[3];

    TriMesh mesh_sd, mesh_cdm;
    OpenMesh::IO::Options mesh_cdm_io_opt(OpenMesh::IO::Options::VertexNormal),
        mesh_sd_io_opt(OpenMesh::IO::Options::VertexNormal);

    timer tm_load_mesh;
    std::vector<std::thread> thread;
    try
    {
        std::cout << "Loading: " << filename_Sd << '\n';
        mesh_sd.request_vertex_normals();
        thread.push_back(
            std::thread([&] { OpenMesh::IO::read_mesh(mesh_sd, filename_Sd, mesh_sd_io_opt); }));

        std::cout << "Loading: " << filename_cdm << '\n';
        mesh_cdm.request_vertex_normals();
        thread.push_back(
            std::thread([&] { OpenMesh::IO::read_mesh(mesh_cdm, filename_cdm, mesh_cdm_io_opt); }));
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    for (auto &t : thread)
        t.join();

    tm_load_mesh.stop();

    std::cout << "Meshes loaded! " << std::endl;

    if (!mesh_sd_io_opt.check(OpenMesh::IO::Options::VertexNormal))
    {
        std::cout << "Mesh_sd does not have normals. Computing..." << std::endl;
        mesh_sd.request_vertex_normals();
        mesh_sd.request_face_normals();
        compute_vertex_normal(mesh_sd);
    }

    auto vit_cdm = mesh_cdm.vertices_begin();
    for (auto vi = mesh_sd.vertices_begin(); vi != mesh_sd.vertices_end(); ++vi, ++vit_cdm)
    {
        auto &point = mesh_sd.point(*vi);
        //auto &cdm = mesh_cdm.point(*vit_cdm);
        const auto &normal = mesh_sd.normal(*vi).normalized();
        // const auto &normal = mesh_cdm.normal(*vi).normalized();
        const decimal_t cdm = mesh_cdm.point(*vit_cdm)[2];
        point = point + normal * cdm;

        mesh_sd.set_point(*vi, point);
    }

    //
    // Save output mesh
    //
    std::cout << "Saving mesh..." << std::endl;
    timer tm_save_mesh;
    if (!OpenMesh::IO::write_mesh(mesh_sd, filename_out, mesh_sd_io_opt))
    {
        std::cout << "Could not save " << filename_out << std::endl;
        return EXIT_FAILURE;
    }
    tm_save_mesh.stop();

    return EXIT_SUCCESS;
}
