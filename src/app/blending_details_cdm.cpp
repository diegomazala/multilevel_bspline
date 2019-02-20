#include <mesh_utils.h>
#include <timer.h>

int main(int argc, char *argv[])
{
    using decimal_t = double;
    timer tm_total;

    if (argc < 5)
    {
       std::cout << "Usage: app <Sb_mesh_file> <Sd_0_mesh_file> <Sd_1_mesh_file> <cdm_scalar_mesh_file> <output_mesh_file>\n"
                 << "Usage: app \
                 ~/data/Figs/sd_cdm_fig03/sd_3.obj \
				 ~/data/Figs/sd_cdm_fig03/cdm_3.obj \
                 ~/data/Figs/sd_cdm_fig02/sd_3.obj \
                 ~/data/Figs/sd_cdm_fig02/sd_4.obj \
                 ~/data/Figs/sd_cdm_fig02/cdm_3.obj \
                 ~/data/Figs/sd_cdm_fig03/blending_3_2_cdm_3.obj \n";
       return EXIT_FAILURE;
    }

    const std::string filename_Sb = argv[1];
    const std::string filename_cdm_0 = argv[2];
    const std::string filename_Sd_0 = argv[3];
    const std::string filename_Sd_1 = argv[4];
    const std::string filename_cdm_1 = argv[5];
    const decimal_t weight_cdm_0 = atof(argv[6]);
    const decimal_t weight_cdm_1 = atof(argv[7]);
    const std::string filename_out = argv[8];

    Mesh_t mesh_sb, mesh_sd_0, mesh_sd_1, mesh_cdm_0, mesh_cdm_1;
    OpenMesh::IO::Options 
        mesh_cdm_io_opt,
        mesh_sb_io_opt,
        mesh_sd_io_opt_0(OpenMesh::IO::Options::VertexNormal),
        mesh_sd_io_opt_1;

    timer tm_load_mesh;
    std::vector<std::thread> thread;
    try
    {
        std::cout << "Loading: " << filename_Sb << '\n';
        thread.push_back(
            std::thread([&] 
            { 
                OpenMesh::IO::read_mesh(mesh_sb, filename_Sb, mesh_sb_io_opt); 
            }));


        std::cout << "Loading: " << filename_Sd_0 << '\n';
        mesh_sd_0.request_vertex_normals();
        thread.push_back(
            std::thread([&] 
            { 
                OpenMesh::IO::read_mesh(mesh_sd_0, filename_Sd_0, mesh_sd_io_opt_0); 
            }));

        std::cout << "Loading: " << filename_Sd_1 << '\n';
        thread.push_back(
            std::thread([&] 
            { 
                OpenMesh::IO::read_mesh(mesh_sd_1, filename_Sd_1, mesh_sd_io_opt_1); 
            }));

        std::cout << "Loading: " << filename_cdm_0 << '\n';
        mesh_cdm_0.request_vertex_normals();
        thread.push_back(
            std::thread([&] 
            { 
                OpenMesh::IO::read_mesh(mesh_cdm_0, filename_cdm_0, mesh_cdm_io_opt); 
            }));

		std::cout << "Loading: " << filename_cdm_1 << '\n';
        mesh_cdm_1.request_vertex_normals();
        thread.push_back(
            std::thread([&] { OpenMesh::IO::read_mesh(mesh_cdm_1, filename_cdm_1, mesh_cdm_io_opt); }));
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    for (auto &t : thread)
        t.join();

    tm_load_mesh.stop();

    std::cout << "Meshes loaded and normals computed! " << std::endl;

    auto vit_sd_0 = mesh_sd_0.vertices_begin();
    auto vit_sd_1 = mesh_sd_1.vertices_begin();
    for (; vit_sd_0 != mesh_sd_0.vertices_end(); ++vit_sd_0, ++vit_sd_1)
    {
        const auto& sd0 = mesh_sd_0.point(*vit_sd_0);
        const auto& sd1 = mesh_sd_1.point(*vit_sd_1);

        const auto D0 = sd1 - sd0;
        
        mesh_sd_0.set_point(*vit_sd_1, D0);
    }

	Mesh_t& mesh_D0 = mesh_sd_0;
	Mesh_t& mesh_SA = mesh_sb;

    auto vit_sb = mesh_sb.vertices_begin();
    auto vit_D0 = mesh_D0.vertices_begin();
    for (; vit_sb != mesh_sb.vertices_end(); ++vit_sb, ++vit_D0)
    {
        const auto& SbA = mesh_sb.point(*vit_sb);
        const auto& D0 = mesh_D0.point(*vit_D0);
        const auto SA = SbA + D0;
        mesh_SA.set_point(*vit_sb, SA);
    }

    //
    // Computing normals
    //
    mesh_SA.request_vertex_normals();
    mesh_SA.request_face_normals();
    compute_vertex_normal(mesh_SA);
    


    auto vit_SA = mesh_SA.vertices_begin();
    auto vit_cdm_0 = mesh_cdm_0.vertices_begin();
    auto vit_cdm_1 = mesh_cdm_1.vertices_begin();
    for (; vit_SA != mesh_SA.vertices_end(); ++vit_SA, ++vit_cdm_0, ++vit_cdm_1)
    {
        const auto &SA = mesh_SA.point(*vit_SA);
        const auto &N = mesh_SA.normal(*vit_SA).normalized();
        const decimal_t cdm_0 = mesh_cdm_0.point(*vit_cdm_0)[2];
        const decimal_t cdm_1 = mesh_cdm_1.point(*vit_cdm_1)[2];
        const auto M = SA + N * (weight_cdm_0 * cdm_0 + weight_cdm_1 * cdm_1);
        mesh_SA.set_point(*vit_SA, M);
    }

    compute_vertex_normal(mesh_SA);



    //
    // Save output mesh
    //
    std::cout << "Saving mesh '" << filename_out << "'..." << std::endl;
    timer tm_save_mesh;
    if (!OpenMesh::IO::write_mesh(mesh_SA, filename_out, OpenMesh::IO::Options::VertexNormal))
    {
        std::cout << "Could not save " << filename_out << std::endl;
        return EXIT_FAILURE;
    }
    tm_save_mesh.stop();


    return EXIT_SUCCESS;
}
