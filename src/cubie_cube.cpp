#include "cubie_cube.hpp"

void CubieCube::multiply(const CubieCube &b)
{
    CubieCube result;

    for (int i = 0; i < CORNER_COUNT; ++i)
    {
        result.cp[i] = cp[static_cast<int>(b.cp[i])];
        result.co[i] = (co[static_cast<int>(b.cp[i])] + b.co[i]) % 3;
    }

    for (int i = 0; i < EDGE_COUNT; ++i)
    {
        result.ep[i] = ep[static_cast<int>(b.ep[i])];
        result.eo[i] = (eo[static_cast<int>(b.ep[i])] + b.eo[i]) % 2;
    }

    *this = result;
}