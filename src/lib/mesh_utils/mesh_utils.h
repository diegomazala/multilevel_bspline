#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>
#include <filesystem>
namespace fs = std::filesystem;

#if _MSC_VER
#pragma warning(push, 0) // supressing warnings for OpenMesh
#endif
#define _USE_MATH_DEFINES
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#if _MSC_VER
#pragma warning(pop)
#endif

// Define my personal traits
template<typename T>
struct MeshTraits : OpenMesh::DefaultTraits
{
	using Scalar = T;
    /// The default coordinate type is OpenMesh::Vec3f.
    using Point = OpenMesh::VectorT<T,3>;

    /// The default normal type is OpenMesh::Vec3f.
    using Normal = OpenMesh::VectorT<T,3>;

    /// The default 1D texture coordinate type is float.
    using TexCoord2D = OpenMesh::VectorT<T,2>;

    /// The default texture index type
    using TextureIndex = uint32_t;

    /// The default color type is OpenMesh::Vec3uc.
    using Color = OpenMesh::Vec3uc;

    #ifndef DOXY_IGNORE_THIS
    VertexTraits    {};
    HalfedgeTraits  {};
    EdgeTraits      {};
    FaceTraits      {};
    #endif

    VertexAttributes(0);
    HalfedgeAttributes(OpenMesh::Attributes::PrevHalfedge);
    EdgeAttributes(0);
    FaceAttributes(0);
};

using TriMesh = OpenMesh::TriMesh_ArrayKernelT<MeshTraits<double>>;
using Mesh_t = OpenMesh::PolyMesh_ArrayKernelT<MeshTraits<double>>;

template <typename T>
constexpr T ndc(T v, T min, T max) // normalize to [0, 1]
{
    return (v - min) / (max - min);
}

template <typename T>
static std::string
filename_append_before_extension(const std::string &filename_in,
                                 const T &append, const char separator = '_')
{
    auto ext_pos = filename_in.rfind('.', filename_in.length());
    if (ext_pos == std::string::npos)
        ext_pos = filename_in.length();
    std::stringstream filename_out;
    filename_out << filename_in.substr(0, ext_pos) << separator << append
                 << filename_in.substr(ext_pos, filename_in.length() - ext_pos);
    return filename_out.str();
}

template <typename mesh_t>
static bool load_mesh(mesh_t &mesh, const std::string &filename)
{
    mesh.request_vertex_texcoords2D();
    mesh.request_vertex_normals();
    try
    {
        std::cout << "-- Loading mesh  " << filename << std::endl;
        OpenMesh::IO::Options OptionRead(OpenMesh::IO::Options::VertexTexCoord | OpenMesh::IO::Options::VertexNormal);
        return OpenMesh::IO::read_mesh(mesh, filename, OptionRead);
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        return false;
    }
}

template <typename mesh_t>
static bool save_mesh(const mesh_t &mesh, const std::string &filename)
{
    try
    {
        std::cout << "-- Saving mesh   " << filename << std::endl;
        OpenMesh::IO::Options OptionWrite(OpenMesh::IO::Options::VertexTexCoord | OpenMesh::IO::Options::VertexNormal);
        return OpenMesh::IO::write_mesh(mesh, filename, OptionWrite);
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        return false;
    }
}


static std::tuple<Mesh_t::Point, Mesh_t::Point>
mesh_bounding_box(const Mesh_t &mesh)
{
	Mesh_t::Point min_vert = {std::numeric_limits<TriMesh::Scalar>::max(),
                                std::numeric_limits<TriMesh::Scalar>::max(),
                                std::numeric_limits<TriMesh::Scalar>::max()};
	Mesh_t::Point max_vert = {std::numeric_limits<TriMesh::Scalar>::min(),
                                std::numeric_limits<TriMesh::Scalar>::min(),
                                std::numeric_limits<TriMesh::Scalar>::min()};

    for (auto vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi)
    {
        auto pt = mesh.point(*vi);
        for (auto i = 0; i < 3; ++i)
        {
            if (pt[i] < min_vert[i])
                min_vert[i] = pt[i];
            if (pt[i] > max_vert[i])
                max_vert[i] = pt[i];
        }
    }
    return std::make_tuple(min_vert, max_vert);
}

static Mesh_t::Point
center_of_bounding_box(const Mesh_t::Point &min_corner,
                       const Mesh_t::Point &max_corner)
{
    return {min_corner[0] + ((max_corner[0] - min_corner[0]) * 0.5f),
            min_corner[1] + ((max_corner[1] - min_corner[1]) * 0.5f),
            min_corner[2] + ((max_corner[2] - min_corner[2]) * 0.5f)};
}

static Mesh_t::Scalar max_range_of_bounding_box(const Mesh_t::Point &min_corner,
                                       const Mesh_t::Point &max_corner)
{
    std::vector<Mesh_t::Scalar> range = {max_corner[0] - min_corner[0],
                                max_corner[1] - min_corner[1],
                                max_corner[2] - min_corner[2]};
    return *std::max_element(range.begin(), range.end());
}

static void normalize_mesh(Mesh_t &mesh, const Mesh_t::Point &center, Mesh_t::Scalar scale)
{
    for (auto vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi)
    {
        auto point = mesh.point(*vi);
        point -= center;
        point /= scale;
        point += Mesh_t::Point(0.5f, 0.5f, 0.5f);
        mesh.set_point(*vi, point);
    }
}

template <typename mesh_t>
void normalize_mesh(mesh_t &mesh)
{
    auto [min_corner, max_corner] = mesh_bounding_box(mesh);
    auto scale = max_range_of_bounding_box(min_corner, max_corner);
    auto center = center_of_bounding_box(min_corner, max_corner);
    normalize_mesh(mesh, center, scale);
}

void create_grid_mesh(TriMesh &mesh, uint32_t rows, uint32_t cols,
	TriMesh::Scalar width = 1.0f, TriMesh::Scalar height = 1.0f)
{
    std::vector<TriMesh::VertexHandle> vhandle((rows + 1) * (cols + 1));
    std::vector<TriMesh::VertexHandle> fhandle(3);

    for (uint32_t i = 0; i <= rows; ++i)
    {
        for (uint32_t j = 0; j <= cols; ++j)
        {
			TriMesh::Scalar x = static_cast<TriMesh::Scalar>(j) / cols * width;
			TriMesh::Scalar y = static_cast<TriMesh::Scalar>(i) / rows * height;
            vhandle[j * (rows + 1) + i] = mesh.add_vertex(TriMesh::Point(x, y, 0));
        }
    }

    rows += 1;
    cols += 1;

    for (uint32_t i = 0; i < rows - 1; ++i)
    {
        for (uint32_t j = 1; j < cols; ++j)
        {
            // std::cout << i * rows + j << ' ' << (i + 1) * rows + j - 1 << ' ' << i
            // * rows + j - 1 << std::endl;
            fhandle[0] = vhandle[i * rows + j];
            fhandle[2] = vhandle[(i + 1) * rows + j - 1];
            fhandle[1] = vhandle[i * rows + j - 1];
            mesh.add_face(fhandle);
            // std::cout << i * rows + j << ' ' << (i + 1) * rows + j << ' ' << (i +
            // 1) * rows + j - 1 << std::endl;
            fhandle[0] = vhandle[i * rows + j];
            fhandle[2] = vhandle[(i + 1) * rows + j];
            fhandle[1] = vhandle[(i + 1) * rows + j - 1];
            mesh.add_face(fhandle);
            // std::cout << std::endl;
        }
    }

    mesh.request_vertex_texcoords2D();
    for (auto vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi)
    {
        const auto& point = mesh.point(*vi);
        mesh.set_texcoord2D(*vi, {point[0], point[1]});
    }
    //mesh.release_vertex_texcoords2D();    
}

template <typename decimal>
static void mesh_to_vecs(TriMesh &mesh, std::shared_ptr<std::vector<decimal>> U,
                         std::shared_ptr<std::vector<decimal>> V,
                         std::shared_ptr<std::vector<decimal>> Z)
{
    for (auto vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi)
    {
        auto uv = mesh.texcoord2D(*vi);
        auto point = mesh.point(*vi);

        // if (point[2] > 0.5)
        {
            U->push_back(uv[0]);
            V->push_back(uv[1]);
            Z->push_back(point[2]);
        }
    }
}


template <typename scalar_t, typename mesh_t>
void mesh_to_vecs(const mesh_t& mesh, std::vector<scalar_t>& x, std::vector<scalar_t>& y, std::vector<scalar_t>& z)
{
    if (x.size() != mesh.n_vertices())
        x.resize(mesh.n_vertices());

    if (y.size() != mesh.n_vertices())
        y.resize(mesh.n_vertices());

    if (z.size() != mesh.n_vertices())
        z.resize(mesh.n_vertices());

    size_t i = 0;
	for (auto vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi, ++i)
	{
		const auto& point = mesh.point(*vi);
        x[i] = point[0];
        y[i] = point[1];
        z[i] = point[2];
	}
}



template <typename scalar_t, typename mesh_t>
void mesh_uv_to_vecs(const mesh_t& mesh, std::vector<scalar_t>& u, std::vector<scalar_t>& v, std::vector<scalar_t>& z)
{
    if (u.size() != mesh.n_vertices())
        u.resize(mesh.n_vertices());

    if (v.size() != mesh.n_vertices())
        v.resize(mesh.n_vertices());

    if (z.size() != mesh.n_vertices())
        z.resize(mesh.n_vertices());

    size_t i = 0;
	for (auto vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi, ++i)
	{
        const auto& uv = mesh.texcoord2D(*vi);
		const auto& point = mesh.point(*vi);
        u[i] = uv[0];
        v[i] = uv[1];
        z[i] = point[2];
	}
}

template <typename scalar_t, typename mesh_t>
void mesh_uv_to_vecs(const mesh_t &mesh, std::vector<scalar_t> &x, std::vector<scalar_t> &y,
                     std::vector<scalar_t> &z, 
                    std::vector<scalar_t>& u, std::vector<scalar_t>& v)
{
    if (x.size() != mesh.n_vertices())
        x.resize(mesh.n_vertices());

    if (y.size() != mesh.n_vertices())
        y.resize(mesh.n_vertices());

    if (z.size() != mesh.n_vertices())
        z.resize(mesh.n_vertices());

    if (u.size() != mesh.n_vertices())
        u.resize(mesh.n_vertices());
    
    if (v.size() != mesh.n_vertices())
        v.resize(mesh.n_vertices());

    size_t i = 0;
	for (auto vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi, ++i)
	{
        const auto& _uv = mesh.texcoord2D(*vi);
		const auto& _point = mesh.point(*vi);
        x[i] = _point[0];
        y[i] = _point[1];
        z[i] = _point[2];
        u[i] = _uv[0];
        v[i] = _uv[1];
	}
}

template <typename mesh_t>
void compute_vertex_normal(mesh_t &mesh)
{
    if (!mesh.has_vertex_normals())
        mesh.request_vertex_normals();

    if (!mesh.has_face_normals())
        mesh.request_face_normals();

    mesh.update_vertex_normals();
    mesh.update_face_normals();

	mesh_t::VertexIter v_it, v_end(mesh.vertices_end());
	mesh_t::VertexFaceIter vf_it;
	mesh_t::Normal tmp;
	mesh_t::Scalar count;
    for (v_it = mesh.vertices_begin(); v_it != v_end; ++v_it)
    {
        tmp[0] = tmp[1] = tmp[2] = 0.0;
        count = 0.0;
        for (vf_it = mesh.vf_iter(*v_it); vf_it.is_valid(); ++vf_it)
        {
            tmp += mesh.calc_face_normal(*vf_it);
            ++count;
        }
        mesh.set_normal(*v_it, tmp / count);
    }
}

template <typename mesh_t>
bool save_points_obj(const mesh_t& mesh, const std::string& filename)
{
    try
    {
        std::cout << "-- Saving points " << filename << std::endl;
        std::ofstream out(filename, std::ios::out);
        for (auto vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi)
        {
            out << std::fixed << "v " << mesh.point(*vi) << '\n';
        }
        out.close();
        return true;
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        return false;
    }   
}

template <typename decimal_t>
bool save_points_obj(const std::vector<std::array<decimal_t, 3>>& points, const std::string& filename)
{
    try
    {
        std::cout << "-- Saving points " << filename << std::endl;
        std::ofstream out(filename, std::ios::out);
        for (const auto pt : points)
        {
            out << std::fixed << "v " << pt[0] << ' ' << pt[1] << ' ' << pt[2] << '\n';
        }
        out.close();
        return true;
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        return false;
    }   
}

