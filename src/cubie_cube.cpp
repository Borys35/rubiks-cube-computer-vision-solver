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
        {5, 10},  // UR slot: U, R facelets
        {7, 19},  // UF slot: U, F facelets
        {3, 37},  // UL slot: U, L facelets
        {1, 46},  // UB slot: U, B facelets
        {28, 14}, // DR slot: D, R facelets
        {30, 23}, // DF slot: D, F facelets
        {34, 41}, // DL slot: D, L facelets
        {32, 52}, // DB slot: D, B facelets
        {21, 12}, // FR slot: F, R facelets
        {25, 43}, // FL slot: F, L facelets
        {48, 39}, // BL slot: B, L facelets
        {50, 16}  // BR slot: B, R facelets
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