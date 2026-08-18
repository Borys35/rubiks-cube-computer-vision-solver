#pragma once

#include <array>
#include "corner.hpp"
#include "edge.hpp"
#include "color.hpp"

using FaceletCube = std::array<Color, 54>;
using FaceletFace = std::array<Color, 9>;

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

    // facelet cube representation
    FaceletCube to_facelet_cube() const;

    // for coordinate cube representation
    void set_eo_from_coordinate(int eo_coord);
    void set_co_from_coordinate(int co_coord);
    void set_ud_slice_from_coordinate(int ud_slice_coord);
    void set_cp_from_coordinate(int cp_coord);
    void set_ep8_from_coordinate(int ep8_coord);
    void set_ud_slice_perm_from_coordinate(int ud_slice_perm_coord);
    int get_eo_coordinate() const;
    int get_co_coordinate() const;
    int get_ud_slice_coordinate() const;
    int get_cp_coordinate() const;
    int get_ep8_coordinate() const;
    int get_ud_slice_perm_coordinate() const;

private:
    int cnk(int n, int k) const; // combinatorial number system n choose k
};
