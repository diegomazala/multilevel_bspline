#include <iostream>
#include "bezier.h"




int main(int, char* argv[])
{
    using decimal_t = float;
    decimal_t u = static_cast<decimal_t>(atof(argv[1]));
    decimal_t v = static_cast<decimal_t>(atof(argv[2]));

    // float coords[4] = { 1.0f, 2.0f, 3.0f, 4.0f};
    // std::cout << "linear   : "<< curve::bezier::linear<float>(atof(argv[1]), coords) << std::endl;
    // std::cout << "quadratic: "<< curve::bezier::quadratic<float>(atof(argv[1]), coords) << std::endl;
    // std::cout << "cubic    : "<< curve::bezier::cubic<float>(atof(argv[1]), coords) << std::endl;
#if 1
    decimal_t p[4][4] = 
    {
        0.0f, 0.33f, 0.66f, 1.0f,
        0.0f, 0.33f, 0.66f, 1.0f,
        0.0f, 0.33f, 0.66f, 1.0f,
        0.0f, 0.33f, 0.66f, 1.0f
    };
#else
    decimal_t p[4][4];
    for (auto i = 0; i < 4; ++i)
    {
        for (auto j = 0; j < 4; ++j)
        {
            p[i][j] = static_cast<decimal_t>(i * 4 + j) / (4 * 4 - 1);
        }
    }
#endif    

    std::cout << std::fixed << "\nBezier Surface Interpol: " << surface::bezier::cubic<decimal_t>(u, v, p) << std::endl << std::endl;

    return 0;
}
