#pragma once

#include "bspline_surface.h"
#include <array>
#include <memory>

// surface namespace starts here
namespace surface
{

template <typename decimal_t, typename vec3_t = std::array<decimal_t, 3>>
class multilevel_bspline_t
{
  public:
    using matrix_t = std::vector<std::vector<decimal_t>>;
    using matrix4_t = std::array<std::array<decimal_t, 4>, 4>;
    using vec2_t = std::array<decimal_t, 2>;
    using bspline_surface_t = surface::bspline_t<decimal_t>;

    multilevel_bspline_t(const decimal_t *_x_vec_ptr, const decimal_t *_y_vec_ptr,
                const decimal_t *_z_vec_ptr, size_t _point_count, uint32_t _m,
                uint32_t _n, uint32_t _h)
        : x_ptr(_x_vec_ptr), y_ptr(_y_vec_ptr), z_ptr(_z_vec_ptr),
          point_count(_point_count), m(_m), n(_n), h(_h)
    {
    }

    
    void compute()
    {
        surfaces.clear();

        const decimal_t *delta_z_ptr = this->z_ptr;
        auto lm = this->m;
        auto ln = this->n;

        for (uint32_t k = 0; k <= this->h; ++k)
        {
            surfaces.push_back(
                bspline_surface_t(
                    x_ptr, y_ptr, delta_z_ptr, 
                    point_count, lm, ln));

            auto &surf = surfaces[k];

            //
            // Compute the surface approximation the its error
            //
            surf.compute();
            auto error = surf.compute_error();
            std::cout << std::fixed << "[k, error]: " << k << ' ' << error
                      << std::endl;

            //
            // Get difference between the function and the input z
            //
            delta_z_ptr = surf.get_delta_z();

            //
            // Refine
            //
            lm *= 2;
            ln *= 2;
        }
    }

    //
    // Overload operator () to evaluate the function at (u, v)
    //
    decimal_t operator()(decimal_t u, decimal_t v) const { return eval(u, v); }
    decimal_t operator()(decimal_t u, decimal_t v, uint32_t k) const { return eval(u, v, k); }


    //
    // Evaluate the function at (u, v)
    //
    decimal_t eval(decimal_t u, decimal_t v) const
    {
        //
        // Evaluate the function in all levels
        //
        decimal_t val = 0;
        for (const auto &surf : surfaces)
        {
            val += surf(u, v);
        }
        return val;
    }

    decimal_t eval(decimal_t u, decimal_t v, uint32_t k) const
    {
        //
        // Evaluate the function in all levels
        //
        decimal_t val = 0;
        for (uint32_t i = 0; i <= k; ++i)
        {
            val += surfaces[i](u, v);
        }
        return val;
    }

    

  //protected:

    //
    // Attributes
    //
    const decimal_t *x_ptr;
    const decimal_t *y_ptr;
    const decimal_t *z_ptr;
    const size_t point_count;
    uint32_t m, n, h;
    std::vector<bspline_surface_t> surfaces;
};

} // namespace surface