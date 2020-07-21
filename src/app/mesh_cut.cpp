#include <iostream>

#include <algorithm>
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;

#define _USE_MATH_DEFINES
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

struct MyTraits : public OpenMesh::DefaultTraits
{
};

using PolyMesh = OpenMesh::PolyMesh_ArrayKernelT<MyTraits>;
OpenMesh::VPropHandleT<PolyMesh::Normal> Normals;
OpenMesh::VPropHandleT<PolyMesh::TexCoord2D> TexCoord;

std::string g_filename_in;
std::string g_filename_out;

template <typename T>
std::string filename_append_before_extension(const std::string &filename_in, const T &append,
                                             const char separator = '_')
{
    auto ext_pos = filename_in.rfind('.', filename_in.length());
    if (ext_pos == std::string::npos)
        ext_pos = filename_in.length();
    std::stringstream filename_out;
    filename_out << filename_in.substr(0, ext_pos) << separator << append
                 << filename_in.substr(ext_pos, filename_in.length() - ext_pos);
    return filename_out.str();
}

void mesh_cut(PolyMesh &mesh, float min_z)
{
    mesh.request_face_status();
    mesh.request_edge_status();
    mesh.request_vertex_status();

    std::cout << "deleting faces ;" << std::endl;
    std::vector<PolyMesh::VertexHandle> verts;
    for (auto vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi)
    {
        if (mesh.point(*vi)[2] < min_z)
        {
            verts.push_back(*vi);
        }
    }

    std::cout << "deleting verts : " << verts.size() << std::endl;

    for (auto vh : verts)
    {
        for (auto fit = mesh.vf_cwbegin(vh); fit != mesh.vf_cwbegin(vh); ++fit)
            mesh.delete_face(*fit, false);

        mesh.delete_vertex(vh, false);
    }

    std::cout << "mesh.garbage_collection();" << std::endl;
    mesh.garbage_collection();
}

template <typename T> void vector_write(std::ofstream &out_file, const std::vector<T> &data)
{
    const std::size_t count = data.size();
    out_file.write(reinterpret_cast<const char *>(&count), sizeof(std::size_t));
    out_file.write(reinterpret_cast<const char *>(&data[0]), count * sizeof(T));
}

template <typename T> void vector_read(std::ifstream &in_file, std::vector<T> &data)
{
    std::size_t count;
    in_file.read(reinterpret_cast<char *>(&count), sizeof(std::size_t));
    data.resize(count);
    in_file.read(reinterpret_cast<char *>(&data[0]), count * sizeof(T));
}

void save_verts_index(const PolyMesh &mesh, float min_z)
{
    std::vector<int> verts;
    for (auto vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi)
    {
        if (mesh.point(*vi)[2] < min_z)
        {
            verts.push_back(vi->idx());
        }
    }

    std::cout << "Vertex list size: " << verts.size() << std::endl;

    std::ofstream out_file;
    out_file.open("vertex_list.bin", std::ios::out | std::ios::binary);
    vector_write(out_file, verts);
    out_file.close();
}

void delete_vertex_index(PolyMesh &mesh)
{
    std::vector<int> verts;
    std::ifstream in_file;
    in_file.open("vertex_list.bin", std::ios::in | std::ios::binary);
    vector_read(in_file, verts);
    in_file.close();

    mesh.request_face_status();
    mesh.request_edge_status();
    mesh.request_vertex_status();

    std::cout << "Vertex count to delete: " << verts.size() << std::endl;

    for (auto vidx : verts)
    {
        auto vh = mesh.vertex_handle(vidx);
        for (auto fit = mesh.vf_cwbegin(vh); fit != mesh.vf_cwbegin(vh); ++fit)
            mesh.delete_face(*fit, false);

        mesh.delete_vertex(vh, false);
    }

    std::cout << "mesh.garbage_collection();" << std::endl;
    mesh.garbage_collection();
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "Usage: app <mesh_file_in> < min_z > <mesh_file_out>\n"
                  << "Usage: app ../data/face.obj 0 \n";
        return EXIT_FAILURE;
    }

    g_filename_in = argv[1];
    const float min_z = atof(argv[2]);
    if (argc > 2)
        g_filename_out = argv[3];
    else
        g_filename_out = filename_append_before_extension(g_filename_in, "cut");

    // g_filename_out = g_filename_in;
    // g_filename_out = g_filename_out.replace(g_filename_out.size() - 8, 5, ".");

    OpenMesh::IO::Options OptionRead(OpenMesh::IO::Options::VertexTexCoord);
    PolyMesh mesh_in;
    try
    {
        std::cout << "-- Loading mesh " << g_filename_in << std::endl;
        mesh_in.request_vertex_texcoords2D();
        if (!OpenMesh::IO::read_mesh(mesh_in, g_filename_in, OptionRead))
        {
            std::cout << "Could not open " << g_filename_in << std::endl;
            return EXIT_FAILURE;
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        return false;
    }

    if (!OptionRead.check(OpenMesh::IO::Options::VertexTexCoord))
        std::cout << "[Info] Mesh does not have tex coords" << std::endl;

    //
    // these three functions are independent
    // for simplicity, choose only one of then
    //
    //save_verts_index(mesh_in, min_z);
    // mesh_cut(mesh_in, min_z);
    delete_vertex_index(mesh_in);

    std::cout << "-- Saving mesh " << g_filename_out << std::endl;
    if (!OpenMesh::IO::write_mesh(mesh_in, g_filename_out, OpenMesh::IO::Options::VertexTexCoord))
    {
        std::cout << "Could not save " << g_filename_out << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
