#include "mesh_utils.h"



void convert_quad_to_tri(Mesh_t &mesh, TriMesh &tri_mesh, bool invert_face = false)
{
	mesh.request_face_status();
	mesh.request_vertex_status();
	mesh.request_vertex_normals();
	mesh.request_vertex_texcoords2D();
    tri_mesh.request_face_status();
    tri_mesh.request_vertex_status();
    tri_mesh.request_vertex_normals();
    tri_mesh.request_vertex_texcoords2D();


    for (auto vit = mesh.vertices_begin(); vit != mesh.vertices_end(); ++vit)
    {
        auto vh = tri_mesh.add_vertex(mesh.point(vit));
        tri_mesh.set_texcoord2D(vh, mesh.texcoord2D(vit));
        tri_mesh.set_normal(vh, mesh.normal(vit));
    }

    std::cout << "Poly Mesh: " << mesh.n_vertices() << std::endl
              << "Tri  Mesh: " << tri_mesh.n_vertices() << std::endl;


    for (auto fit = mesh.faces_begin(); fit != mesh.faces_end(); ++fit)
    {
        std::cout << fit->is_valid() << ' ' << fit->valence() << std::endl;
        
        if (fit->valence() != 4)
            continue;

        std::vector<uint32_t> inds;
        inds.reserve(4);

        for (auto v : fit->vertices())
        {
            std::cout << v.idx() << std::endl;
            inds.push_back(v.idx());
        }

        std::vector<TriMesh::VertexHandle> face_vhandle_0, face_vhandle_1;

        auto vh_0 = tri_mesh.vertex_handle(inds[0]);
        auto vh_1 = tri_mesh.vertex_handle(inds[1]);
        auto vh_2 = tri_mesh.vertex_handle(inds[2]);
        auto vh_3 = tri_mesh.vertex_handle(inds[3]);

		std::cout
			<< vh_0.idx() << ' '
			<< vh_1.idx() << ' '
			<< vh_2.idx() << ' '
			<< vh_3.idx() << '\n';

		std::cout
			<< vh_0.is_valid() << ' '
			<< vh_1.is_valid() << ' '
			<< vh_2.is_valid() << ' '
			<< vh_3.is_valid() << '\n';

        std::cout
            << tri_mesh.point(vh_0) << '\n'
            << tri_mesh.point(vh_1) << '\n'
            << tri_mesh.point(vh_2) << '\n'
            << tri_mesh.point(vh_3) << '\n';

        face_vhandle_0.push_back(vh_0);
        face_vhandle_0.push_back(vh_1);
        face_vhandle_0.push_back(vh_3);
        tri_mesh.add_face(face_vhandle_0);

        face_vhandle_1.push_back(vh_1);
        face_vhandle_1.push_back(vh_2);
        face_vhandle_1.push_back(vh_3);
        tri_mesh.add_face(face_vhandle_1);
    }

    std::cout << "Poly Mesh: " << mesh.n_vertices() << ' ' << mesh.n_faces() << std::endl
              << "Tri  Mesh: " << tri_mesh.n_vertices() << ' ' << tri_mesh.n_faces() << std::endl;
}



int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout
            << std::fixed << std::endl
            << "Usage            : ./<app.exe> <input_filename> <output_filename> <invert_face>" << std::endl
            << "Default          : ./quad_to_tri.exe cube.quad cube.tri 0" << std::endl
            << "Default          : ./quad_to_tri.exe cube.quad cube.tri 1" << std::endl
            << std::endl;
        return EXIT_FAILURE;
    }

    bool invert_face = (argc > 3) && (atoi(argv[3]) == 1);
    const std::string info = invert_face ? "(inverted face) " : "(not inverted face) ";

    //
    // Initial parameters
    //
    const fs::path input_path = argv[1];
    fs::path output_path = (argc > 2) ? argv[2] : input_path;


    if (fs::is_directory(input_path))
    {
        for (auto it = fs::directory_iterator(input_path); it != fs::directory_iterator(); ++it)
        {
            TriMesh mesh;
            auto filename_in = it->path().string();
            if (!load_mesh(mesh, filename_in))
                continue;
            
            const std::string filename_out = filename_append_before_extension(filename_in, "tri");
            save_mesh(mesh, filename_out);
        }
    }
    else
    {
        Mesh_t mesh;
        auto filename_in = input_path.string();
        if (!load_mesh(mesh, filename_in))
            return EXIT_FAILURE;

        TriMesh tri_mesh;
        //convert_quad_to_tri(mesh, tri_mesh, invert_face);
        mesh.triangulate();

        const std::string filename_out = filename_append_before_extension(filename_in, "tri");
        save_mesh(mesh, filename_out);
    }


    return EXIT_SUCCESS;
}