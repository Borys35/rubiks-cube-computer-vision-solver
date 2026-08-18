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

    bool is_solvable() const;
    void from_facelet_cube(const FaceletCube& fc);

private:
    int cnk(int n, int k) const; // combinatorial number system n choose k

    static constexpr int CORNER_FACELETS[8][3] = {
        {8, 9, 20},   // URF slot: U, R, F facelets
        {6, 18, 38},  // UFL slot: U, F, L facelets
        {0, 36, 47},  // ULB slot: U, L, B facelets
        {2, 45, 11},  // UBR slot: U, B, R facelets
        {29, 26, 15}, // DFR slot: D, F, R facelets
        {27, 44, 24}, // DFL slot: D, L, F facelets
        {33, 53, 42}, // DLB slot: D, B, L facelets
        {35, 17, 51}  // DBR slot: D, R, B facelets
    };

    static constexpr Color CORNER_COLORS[8][3] = {
        {U_COLOR, R_COLOR, F_COLOR}, // URF piece
        {U_COLOR, F_COLOR, L_COLOR}, // UFL piece
        {U_COLOR, L_COLOR, B_COLOR}, // ULB piece
        {U_COLOR, B_COLOR, R_COLOR}, // UBR piece
        {D_COLOR, F_COLOR, R_COLOR}, // DFR piece
        {D_COLOR, L_COLOR, F_COLOR}, // DFL piece
        {D_COLOR, B_COLOR, L_COLOR}, // DLB piece
        {D_COLOR, R_COLOR, B_COLOR}  // DBR piece
    };

    static constexpr int EDGE_FACELETS[12][2] = {
        {5, 10},  // UR slot: U6, R2
        {7, 19},  // UF slot: U8, F2
        {3, 37},  // UL slot: U4, L2
        {1, 46},  // UB slot: U2, B2
        {32, 16}, // DR slot: D6, R8
        {28, 25}, // DF slot: D2, F8
        {30, 43}, // DL slot: D4, L8
        {34, 52}, // DB slot: D8, B8
        {23, 12}, // FR slot: F6, R4
        {21, 41}, // FL slot: F4, L6
        {50, 39}, // BL slot: B6, L4
        {48, 14}  // BR slot: B4, R6
    };

    static constexpr Color EDGE_COLORS[12][2] = {
        {U_COLOR, R_COLOR}, // UR piece
        {U_COLOR, F_COLOR}, // UF piece
        {U_COLOR, L_COLOR}, // UL piece
        {U_COLOR, B_COLOR}, // UB piece
        {D_COLOR, R_COLOR}, // DR piece
        {D_COLOR, F_COLOR}, // DF piece
        {D_COLOR, L_COLOR}, // DL piece
        {D_COLOR, B_COLOR}, // DB piece
        {F_COLOR, R_COLOR}, // FR piece
        {F_COLOR, L_COLOR}, // FL piece
        {B_COLOR, L_COLOR}, // BL piece
        {B_COLOR, R_COLOR}  // BR piece
    };
};
