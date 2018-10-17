#pragma once
#include <nanoflann.hpp>

// begin of namespace nanoflann
namespace nanoflann
{

// This is an exampleof a custom data set class
template <typename T>
struct PointCloud
{
	typedef T coord_t; //!< The type of each coordinate

	struct Point
	{
		T  x, y, z;
	};

	std::vector<Point>  pts;
}; // end of PointCloud

// And this is the "dataset to kd-tree" adaptor class:
template <typename Derived>
struct PointCloudAdaptor
{
	typedef typename Derived::coord_t coord_t;

	const Derived &obj; //!< A const ref to the data set origin

	/// The constructor that sets the data set source
	PointCloudAdaptor(const Derived &obj_) : obj(obj_) { }

	/// CRTP helper method
	inline const Derived& derived() const { return obj; }

	// Must return the number of data points
	inline size_t kdtree_get_point_count() const { return derived().pts.size(); }

	// Returns the dim'th component of the idx'th point in the class:
	// Since this is inlined and the "dim" argument is typically an immediate value, the
	//  "if/else's" are actually solved at compile time.
	inline coord_t kdtree_get_pt(const size_t idx, const size_t dim) const
	{
		if (dim == 0) return derived().pts[idx].x;
		else if (dim == 1) return derived().pts[idx].y;
		else return derived().pts[idx].z;
	}

	// Optional bounding-box computation: return false to default to a standard bbox computation loop.
	//   Return true if the BBOX was already computed by the class and returned in "bb" so it can be avoided to redo it again.
	//   Look at bb.size() to find out the expected dimensionality (e.g. 2 or 3 for point clouds)
	template <class BBOX>
	bool kdtree_get_bbox(BBOX& /*bb*/) const { return false; }

}; // end of PointCloudAdaptor

template <typename decimal_t>
using pointcloud_adaptor_t = PointCloudAdaptor<PointCloud<decimal_t>>;

template <typename decimal_t, const int dimension>
using kdtree_t = KDTreeSingleIndexAdaptor<
		nanoflann::L2_Simple_Adaptor<decimal_t, pointcloud_adaptor_t<decimal_t> >,
		pointcloud_adaptor_t<decimal_t>,
		dimension>;
        
} // end of namespace nanoflann