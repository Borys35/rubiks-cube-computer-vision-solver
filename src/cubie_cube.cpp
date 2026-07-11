#include "cubie_cube.hpp"

bool CubieCube::is_solved() const
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

FaceletCube CubieCube::to_facelet_cube() const
{
    FaceletCube fc{};

    fc[4] = U_COLOR;
    fc[4 + 9] = R_COLOR;
    fc[4 + 9 * 2] = F_COLOR;
    fc[4 + 9 * 3] = D_COLOR;
    fc[4 + 9 * 4] = L_COLOR;
    fc[4 + 9 * 5] = B_COLOR;

    static const int corner_facelets[8][3] = {
        {8, 9, 20},   // URF slot: U, R, F facelets
        {6, 18, 38},  // UFL slot: U, F, L facelets
        {0, 36, 47},  // ULB slot: U, L, B facelets
        {2, 45, 11},  // UBR slot: U, B, R facelets
        {29, 26, 15}, // DFR slot: D, F, R facelets
        {27, 44, 24}, // DFL slot: D, L, F facelets
        {33, 53, 42}, // DLB slot: D, B, L facelets
        {35, 17, 51}  // DBR slot: D, R, B facelets
    };

    static const Color corner_colors[8][3] = {
        {U_COLOR, R_COLOR, F_COLOR}, // URF piece
        {U_COLOR, F_COLOR, L_COLOR}, // UFL piece
        {U_COLOR, L_COLOR, B_COLOR}, // ULB piece
        {U_COLOR, B_COLOR, R_COLOR}, // UBR piece
        {D_COLOR, F_COLOR, R_COLOR}, // DFR piece
        {D_COLOR, L_COLOR, F_COLOR}, // DFL piece
        {D_COLOR, B_COLOR, L_COLOR}, // DLB piece
        {D_COLOR, R_COLOR, B_COLOR}  // DBR piece
    };

    for (int i = 0; i < CORNER_COUNT; ++i)
    {
        int piece = static_cast<int>(cp[i]);
        int orient = co[i];
        for (int n = 0; n < 3; ++n)
        {
            fc[corner_facelets[i][(n + orient) % 3]] = corner_colors[piece][n];
        }
    }

    static const int edge_facelets[12][2] = {
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

    static const Color edge_colors[12][2] = {
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

    for (int i = 0; i < EDGE_COUNT; ++i)
    {
        int piece = static_cast<int>(ep[i]);
        int orient = eo[i];
        fc[edge_facelets[i][orient]] = edge_colors[piece][0];
        fc[edge_facelets[i][1 - orient]] = edge_colors[piece][1];
    }

    return fc;
}

void CubieCube::set_eo_from_coordinate(int eo_coord)
{
    for (int i = EDGE_COUNT - 1; i >= 0; --i)
    {
        eo[i] = eo_coord % 2;
        eo_coord /= 2;
    }
}

void CubieCube::set_co_from_coordinate(int co_coord)
{
    for (int i = CORNER_COUNT - 1; i >= 0; --i)
    {
        co[i] = co_coord % 3;
        co_coord /= 3;
    }
}

void CubieCube::set_ud_slice_from_coordinate(int ud_slice_coord)
{
    for (int i = 0; i < EDGE_COUNT; ++i)
    {
        if (ud_slice_coord & (1 << i))
        {
            ep[i] = static_cast<Edge>(8 + i); // Set to UD slice edges
        }
        else
        {
            ep[i] = static_cast<Edge>(i); // Set to non-UD slice edges
        }
    }
}

int CubieCube::get_eo_coordinate() const
{
    int eo_coord = 0;
    for (int i = 0; i < EDGE_COUNT; ++i)
    {
        eo_coord = (eo_coord << 1) | eo[i];
    }
    return eo_coord;
}

int CubieCube::get_co_coordinate() const
{
    int co_coord = 0;
    for (int i = 0; i < CORNER_COUNT; ++i)
    {
        co_coord = co_coord * 3 + co[i];
    }
    return co_coord;
}

int CubieCube::get_ud_slice_coordinate() const
{
    int ud_slice_coord = 0;
    for (int i = 0; i < EDGE_COUNT; ++i)
    {
        if (static_cast<int>(ep[i]) >= 8) // check if the edge is in the UD slice
        {
            ud_slice_coord |= (1 << i);
        }
    }
    return ud_slice_coord;
}