#define _USE_MATH_DEFINES
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
// -------------------- OpenMesh
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
typedef OpenMesh::PolyMesh_ArrayKernelT<> PolyMesh;
OpenMesh::EPropHandleT<PolyMesh::Scalar> eWeights, atheta, btheta;
OpenMesh::VPropHandleT<PolyMesh::Normal> vNormals;
OpenMesh::VPropHandleT<PolyMesh::Point> cogs;
OpenMesh::FPropHandleT<PolyMesh::Scalar> fRank;

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

void Scale(PolyMesh &mesh, PolyMesh::Scalar s)
{
    PolyMesh::VertexIter v_it, v_end(mesh.vertices_end());
    for (v_it = mesh.vertices_begin(); v_it != v_end; ++v_it)
        mesh.set_point(*v_it, mesh.point(*v_it) * s);
}

void VertexNormal(PolyMesh &mesh)
{
    PolyMesh::VertexIter v_it, v_end(mesh.vertices_end());
    PolyMesh::VertexFaceIter vf_it;
    PolyMesh::Normal tmp;
    PolyMesh::Scalar count;
    for (v_it = mesh.vertices_begin(); v_it != v_end; ++v_it)
    {
        tmp[0] = tmp[1] = tmp[2] = 0.0;
        count = 0.0;
        for (vf_it = mesh.vf_iter(*v_it); vf_it.is_valid(); ++vf_it)
        {
            tmp += mesh.calc_face_normal(*vf_it);
            ++count;
        }
        mesh.property(vNormals, *v_it) = tmp / count;
    }
}

void OriginalRank(PolyMesh &mesh)
{
    PolyMesh::FaceIter f_it, f_end(mesh.faces_end());
    PolyMesh::FaceFaceIter ff_it;
    PolyMesh::Scalar original, lb(-1.0), ub(1.0), count;
    PolyMesh::Normal aon, d0, d1;
    for (f_it = mesh.faces_begin(); f_it != f_end; ++f_it)
    {
        aon[0] = aon[1] = aon[2] = count = 0.0;
        for (ff_it = mesh.ff_iter(*f_it); ff_it.is_valid(); ++ff_it)
        {
            aon += mesh.calc_face_normal(*ff_it).normalize();
            ++count;
        }
        d0 = mesh.calc_face_normal(*f_it);
        d0.normalize();
        d1 = aon / count;
        d1.normalize();
        original = std::abs(std::max(lb, std::min(ub, dot(d0, d1))));
        mesh.property(fRank, *f_it) = original;
    }
}

void SimpleRank(PolyMesh &mesh, int mode)
{
    PolyMesh::FaceIter f_it, f_end(mesh.faces_end());
    PolyMesh::FaceFaceIter ff_it;
    PolyMesh::Scalar lb(-1.0), ub(1.0), count;
    PolyMesh::Normal aon, d0, d1;
    PolyMesh::Scalar weight, weight0;
    PolyMesh::Scalar valence;
    PolyMesh::Normal tmp;
    int r = 0, o = 0, y = 0, g = 0;
    int r0 = 0, o0 = 0, y0 = 0, g0 = 0;

    for (f_it = mesh.faces_begin(); f_it != f_end; ++f_it)
    {
        aon[0] = aon[1] = aon[2] = valence = 0.0;
        for (ff_it = mesh.ff_iter(*f_it); ff_it.is_valid(); ++ff_it)
        {
            tmp = mesh.calc_face_normal(*ff_it);
            tmp.normalize();
            aon += tmp;
            ++valence;
        }
        d0 = mesh.calc_face_normal(*f_it);
        d0.normalize();
        d1 = aon / valence;
        d1.normalize();
        weight = std::abs(std::max(lb, std::min(ub, dot(d0, d1))));
        weight0 = mesh.property(fRank, *f_it);

        // 1          0.92387953251 0.70710678118 0.38268343236 0
        // cos(0)     cos(22.5)     cos(45)       cos(67.5)     cos(90)
        if (weight < 0.4)
            r++;
        if ((weight >= 0.4) && (weight < 0.7))
            o++;
        if ((weight >= 0.7) && (weight < 0.9))
            y++;
        if ((weight >= 0.9) && (weight <= 1))
            g++;

        if (weight0 < 0.4)
            r0++;
        if ((weight0 >= 0.4) && (weight0 < 0.7))
            o0++;
        if ((weight0 >= 0.7) && (weight0 < 0.9))
            y0++;
        if ((weight0 >= 0.9) && (weight0 <= 1))
            g0++;
    }

    // print result
    std::cout << "Before smoothing:\n";
    std::cout << "  -*Rank*-    "
              << "-*number of faces*-\n";
    std::cout << "    Bad:        " << r0 << "\n";
    std::cout << "    Good:       " << o0 << "\n";
    std::cout << "    Fine:       " << y0 << "\n";
    std::cout << "    Perfect:    " << g0 << "\n";
    std::cout << "\n";
    if (mode == 1)
        std::cout << "After Laplacian smoothing:\n";
    if (mode == 2)
        std::cout << "After Taubin smoothing:\n";
    if (mode == 3)
        std::cout << "After Laplacian smoothing with custom weight:\n";
    std::cout << "  -*Rank*-    "
              << "-*number of faces*-\n";
    std::cout << "    Bad:        " << r << "\n";
    std::cout << "    Good:       " << o << "\n";
    std::cout << "    Fine:       " << y << "\n";
    std::cout << "    Perfect:    " << g << "\n";
}

void LenWeight(PolyMesh &mesh)
{
    PolyMesh::EdgeIter e_it, e_end(mesh.edges_end());
    for (e_it = mesh.edges_begin(); e_it != e_end; ++e_it)
        mesh.property(eWeights, *e_it) = 1.0 / mesh.calc_edge_length(*e_it);
}

void CotanWeight(PolyMesh &mesh)
{
    PolyMesh::EdgeIter e_it, e_end(mesh.edges_end());
    PolyMesh::Scalar weight, lb(-1.0), ub(1.0), e0, e1, e2, a, b;
    PolyMesh::HalfedgeHandle h0, h1, h2;
    PolyMesh::Point p0, p1, p2;
    PolyMesh::Normal d0, d1;
    const float pi = 3.14159265359;

    for (e_it = mesh.edges_begin(); e_it != e_end; ++e_it)
    {
        weight = 0.0;

        h0 = mesh.halfedge_handle(*e_it, 0);
        p0 = mesh.point(mesh.to_vertex_handle(h0));
        h1 = mesh.halfedge_handle(*e_it, 1);
        p1 = mesh.point(mesh.to_vertex_handle(h1));

        h2 = mesh.next_halfedge_handle(h0);
        p2 = mesh.point(mesh.to_vertex_handle(h2));
        d0 = (p0 - p2);
        d0.normalize();
        d1 = (p1 - p2);
        d1.normalize();
        e1 = mesh.calc_edge_length(h2);
        e2 = mesh.calc_edge_length(mesh.next_halfedge_handle(h2));
        a = acos(dot(d0, d1));
        weight += PolyMesh::Scalar(1.0) / tan(a);

        h2 = mesh.next_halfedge_handle(h1);
        p2 = mesh.point(mesh.to_vertex_handle(h2));
        d0 = (p0 - p2);
        d0.normalize();
        d1 = (p1 - p2);
        d1.normalize();
        e1 = mesh.calc_edge_length(h2);
        e2 = mesh.calc_edge_length(mesh.next_halfedge_handle(h2));
        b = acos(dot(d0, d1));
        weight += PolyMesh::Scalar(1.0) / tan(b);

        mesh.property(eWeights, *e_it) = weight;
        mesh.property(atheta, *e_it) = a * 180.0 / pi;
        mesh.property(btheta, *e_it) = b * 180.0 / pi;
    }
}


void VertexSmooth(PolyMesh &mesh, unsigned int mode, std::vector<int> nIterations, bool fix_border = false)
{
    /////////////////////////////////////////////////////////
    // mode 1 = Laplacian smoothing                        //
    // mode 2 = Taubin smoothing                           //
    // mode 3 = Laplacian smoothing with custom weight     //
    // 0 < lamda < 1, -1 < mu < 0                          //
    /////////////////////////////////////////////////////////

    // smoothing mesh N times
    PolyMesh::VertexIter v_it, v_end(mesh.vertices_end());
    PolyMesh::VertexEdgeIter ve_it;
    PolyMesh::VertexVertexIter vv_it;
    PolyMesh::EdgeIter e_it, e_end(mesh.edges_end());
    PolyMesh::Point cog, tmp, p0, p1;
    PolyMesh::HalfedgeHandle h0, h1;
    PolyMesh::Scalar valence, weight;
    unsigned int i;
    int N = nIterations[nIterations.size() - 1];
    float lamda, mu;

    if (mode == 1 || mode == 3)
        lamda = 0.25;
    if (mode == 2)
    {
        lamda = 0.5;
        mu = -0.67;
    }
    for (i = 0; i < N; ++i)
    {
        std::cout << "-- smoothing " << i << std::endl;
        if (mode == 3) CotanWeight(mesh);

        for (v_it = mesh.vertices_begin(); v_it != v_end; ++v_it)
        {
			if (fix_border && mesh.is_boundary(v_it))
				continue;

            cog[0] = cog[1] = cog[2] = valence = 0.0;

            // iterate over all neighboring vertices
            if (mode == 1 || mode == 2)
            {
                for (vv_it = mesh.vv_iter(*v_it); vv_it.is_valid(); ++vv_it)
                {
                    cog += mesh.point(*vv_it);
                    ++valence;
                }
                tmp = (cog / valence - mesh.point(*v_it)) * lamda + mesh.point(*v_it);
                if (mode == 2)
                    tmp = (cog / valence - tmp) * mu + tmp;
            }
            else if (mode == 3)
            {
                for (ve_it = mesh.ve_iter(*v_it); ve_it.is_valid(); ++ve_it)
                {
                    h0 = mesh.halfedge_handle(*ve_it, 0);
                    p0 = mesh.point(mesh.to_vertex_handle(h0));
                    h1 = mesh.halfedge_handle(*ve_it, 1);
                    p1 = mesh.point(mesh.to_vertex_handle(h1));
                    weight = mesh.property(eWeights, *ve_it);
                    if (p0 == mesh.point(*v_it))
                        cog += weight * (p1 - p0);
                    else
                        cog += weight * (p0 - p1);
                    valence += weight;
                }
                tmp = (cog / valence) * lamda + mesh.point(*v_it);
            }
            mesh.property(cogs, *v_it) = tmp;
        }
        for (v_it = mesh.vertices_begin(); v_it != v_end; ++v_it)
            if (!(fix_border && mesh.is_boundary(*v_it)))
                mesh.set_point(*v_it, mesh.property(cogs, *v_it));

#if 1
        for (auto l : nIterations)
        {
            if (i == l)
            {
                // add vertex normals
                mesh.update_vertex_normals();
                mesh.update_face_normals();
                VertexNormal(mesh);

                OpenMesh::IO::Options mesh_out_opt(OpenMesh::IO::Options::VertexNormal);
                mesh_out_opt += OpenMesh::IO::Options::VertexTexCoord;

                if (OpenMesh::IO::write_mesh(mesh, filename_append_before_extension(g_filename_out, l), mesh_out_opt))
                {
                    std::cout << "Saved smoothed mesh: "
                              << filename_append_before_extension(g_filename_out, l) << std::endl;
                }
                else
                {
                    std::cout << "Error: Could not save "
                              << filename_append_before_extension(g_filename_out, l) << std::endl;
                }
            }
        }
#endif
    }
}

int main(int argc, char **argv)
{
    PolyMesh mesh;
    
    if (argc < 5)
    {
        std::cerr << "Usage: \n\
                    <filename_in> \n\
                    <filename_out> \n\
                    smooth <mode={1 = laplacian, 2 = taubin, 3 = laplacian with custom weights> \n\
                    <number of iterations, number of iterations, number of iterations, ...> \n";
        return EXIT_FAILURE;
    }

	g_filename_in = argv[1];
	g_filename_out = argv[2];
    int mode = atoi(argv[3]);
    std::vector<int> nIterations;
    for (auto i = 4; i < argc; ++i)
        nIterations.push_back(atoi(argv[i]));

    OpenMesh::IO::Options mesh_in_opt(OpenMesh::IO::Options::VertexTexCoord);

    mesh.request_vertex_normals();
    mesh.request_face_normals();
    mesh.request_vertex_texcoords2D();

    if (!OpenMesh::IO::read_mesh(mesh, g_filename_in, mesh_in_opt))
    {
        std::cerr << "Error: Cannot read mesh from " << argv[3] << std::endl;
        return 1;
    }

    mesh.update_vertex_normals();
    mesh.update_face_normals();

    // add custom properties
    mesh.add_property(vNormals);
    mesh.add_property(cogs);
    mesh.add_property(fRank);
    if (mode == 3)
    {
        mesh.add_property(atheta);
        mesh.add_property(btheta);
        mesh.add_property(eWeights);
    }

    // main function
    OriginalRank(mesh);
    VertexSmooth(mesh, mode, nIterations);

    // add vertex normals
    mesh.update_vertex_normals();
    mesh.update_face_normals();
    VertexNormal(mesh);

#if 0 // The mesh is being save inside VertexNormal function
    OpenMesh::IO::Options mesh_out_opt(OpenMesh::IO::Options::VertexNormal);
    if (mesh_in_opt.check(OpenMesh::IO::Options::VertexTexCoord))
        mesh_out_opt += OpenMesh::IO::Options::VertexTexCoord;


    // write mesh to stdout
    if (!OpenMesh::IO::write_mesh(mesh, g_filename_out, mesh_out_opt))
    {
        std::cerr << "Error: cannot write mesh to " << argv[4] << std::endl;
        return 1;
    }
#endif

    // print simple rank
    SimpleRank(mesh, mode);


    return 0;
}