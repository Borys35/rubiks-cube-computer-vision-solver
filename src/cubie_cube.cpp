#include "cubie_cube.hpp"

bool CubieCube::is_solved()
{
    for (int i = 0; i < EDGE_COUNT; i++)
    {
        if (ep[i] != static_cast<Edge>(i) || eo[i] != 0)
            return false;
    }
    for (int i = 0; i < CORNER_COUNT; i++)
    {
        if (cp[i] != static_cast<Corner>(i) || co[i] != 0)
            return false;
    }
    return true;
}

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