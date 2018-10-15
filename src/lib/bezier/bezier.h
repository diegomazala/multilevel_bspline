#pragma once
#include <cmath>

#pragma warning(push, 0) // supressing warnings
#include <Eigen/Core>
#pragma warning(pop)

// namespace curve starts here
namespace curve
{
// namespace bezier starts here
namespace bezier
{
template <typename decimal_t>
static decimal_t linear(decimal_t t, decimal_t p[2])
{
    return p[0] + (p[1] - p[0]) * t;
}

template <typename decimal_t>
static decimal_t quadratic(decimal_t t, decimal_t p[3])
{
    return std::pow(1 - t, 2) * p[0] + (1 - t) * 2 * t * p[1] + t * t * p[2];
}

template <typename decimal_t>
static decimal_t cubic(decimal_t t, decimal_t p[4])
{
    return std::pow((1 - t), 3) * p[0] + 3 * t * std::pow(1 - t, 2) * p[1] +
           3 * std::pow(t, 2) * (1 - t) * p[2] + std::pow(t, 3) * p[3];
}

template <typename decimal_t>
static decimal_t cubic(decimal_t t, decimal_t p0, decimal_t p1, decimal_t p2, decimal_t p3)
{
    return std::pow((1 - t), 3) * p0 + 3 * t * std::pow(1 - t, 2) * p1 +
           3 * std::pow(t, 2) * (1 - t) * p2 + std::pow(t, 3) * p3;
}
} // namespace bezier
} // namespace curve


// namespace surface starts here
namespace surface
{
// namespace bezier starts here
namespace bezier
{
template <typename decimal_t>
decimal_t cubic(decimal_t u, decimal_t v, decimal_t p[4][4])
{
    Eigen::Matrix<decimal_t, 1, 4> U;
    Eigen::Matrix<decimal_t, 4, 1> V;
    Eigen::Matrix<decimal_t, 4, 4> P;

    U << std::pow((1 - u), 3),
        3 * u * std::pow(1 - u, 2),
        3 * std::pow(u, 2) * (1 - u),
        std::pow(u, 3);

    V << std::pow((1 - v), 3),
        3 * v * std::pow(1 - v, 2),
        3 * std::pow(v, 2) * (1 - v),
        std::pow(v, 3);

    for (auto i = 0; i < 4; ++i)
    {
        for (auto j = 0; j < 4; ++j)
        {
            P(i, j) = p[i][j];
        }
    }

    return U * P * V;
}
} // namespace bezier
} // namespace surface

