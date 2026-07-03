#pragma once

#include <array>
#include "corner.hpp"
#include "edge.hpp"
#include "color.hpp"

using FaceletCube = std::array<Color, 54>;

class CubieCube
{
public:
    // Corner/Edge Permutations and Orientations
    std::array<Edge, EDGE_COUNT> ep;
    std::array<short, EDGE_COUNT> eo;
    std::array<Corner, CORNER_COUNT> cp;
    std::array<short, CORNER_COUNT> co;

    CubieCube()
    {
        set_solved();
    }

    void set_solved()
    {
        for (int i = 0; i < EDGE_COUNT; i++)
        {
            ep[i] = static_cast<Edge>(i);
            eo[i] = 0;
        }
        for (int i = 0; i < CORNER_COUNT; i++)
        {
            cp[i] = static_cast<Corner>(i);
            co[i] = 0;
        }
    }

    bool is_solved() const;
    void multiply(const CubieCube &b);
    FaceletCube to_facelet_cube() const;
};
