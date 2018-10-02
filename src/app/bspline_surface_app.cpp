#include <iostream>
#include <array>
#include "bspline_surface.h"


int main(int argc, char* argv[])
{
    if (argc < 2)
	{
		std::cout 
			<< "Usage: app <mesh_file> < m_n > \n"
			<< "Usage: app 64 \n";
		return EXIT_FAILURE;
	}


	using decimal_t = double;

	const size_t width = 16;
	const size_t height = width;
    const size_t point_count = width * height;

	const uint32_t m = (argc > 1 ? atoi(argv[1]) : 64);
	const uint32_t n = m;

    std::vector<decimal_t> x(point_count);
    std::vector<decimal_t> y(point_count);
    std::vector<decimal_t> z(point_count);
	for (size_t i = 0; i < height; ++i)
	{
		for (size_t j = 0; j < width; ++j)
		{
			auto index = i * width + j;
			x[index] = static_cast<decimal_t>(i) / static_cast<decimal_t>(width);
			y[index] = static_cast<decimal_t>(j) / static_cast<decimal_t>(height);
			z[index] = std::sin(x[index] * 2);
		}
	}

	surface::bspline_t<decimal_t> surf = { x.data(), y.data(), z.data(), point_count, m, n };
    surf.compute();
    auto error = surf.compute_error();
	std::cout << std::fixed  << "Error: " << error << std::endl;


    return EXIT_SUCCESS;
}

