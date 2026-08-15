#include "include/cubie_cube.hpp"

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

int CubieCube::cnk(int n, int k) const
{
    if (n < k || k < 0)
        return 0;
    if (k == 0)
        return 1;
    int val = 1;
    for (int i = 0; i < k; ++i)
    {
        val = val * (n - i) / (i + 1);
    }
    return val;
}

void CubieCube::set_eo_from_coordinate(int eo_coord)
{
    int eo_sum = 0;
    for (int i = EDGE_COUNT - 2; i >= 0; --i)
    {
        eo[i] = eo_coord % 2;
        eo_sum += eo[i];
        eo_coord /= 2;
    }

    eo[EDGE_COUNT - 1] = (2 - (eo_sum % 2)) % 2;
}

void CubieCube::set_co_from_coordinate(int co_coord)
{
    int co_sum = 0;
    for (int i = CORNER_COUNT - 2; i >= 0; --i)
    {
        co[i] = co_coord % 3;
        co_sum += co[i];
        co_coord /= 3;
    }

    co[CORNER_COUNT - 1] = (3 - (co_sum % 3)) % 3;
}

void CubieCube::set_ud_slice_from_coordinate(int coord)
{
    int k = 3;
    for (int i = 11; i >= 0; --i)
    {
        if (coord >= cnk(i, k + 1))
        {
            coord -= cnk(i, k + 1);
            ep[i] = static_cast<Edge>(8 + k);
            k--;
        }
        else
        {
            ep[i] = static_cast<Edge>(-1);
        }
    }

    int non_ud = 0;
    for (int i = 0; i < 12; ++i)
    {
        if (static_cast<int>(ep[i]) == -1)
        {
            ep[i] = static_cast<Edge>(non_ud++);
        }
    }
}

void CubieCube::set_cp_from_coordinate(int cp_coord)
{
    Corner elements[8];
    for (int i = 0; i < 8; ++i)
        elements[i] = static_cast<Corner>(i);

    int factorial = 5040; // 7!
    for (int i = 0; i < 8; ++i)
    {
        int pos = cp_coord / factorial;
        cp[i] = elements[pos];
        cp_coord %= factorial;

        for (int j = pos; j < 7 - i; ++j)
        {
            elements[j] = elements[j + 1];
        }
        if (i < 7)
            factorial /= (7 - i);
    }
}

void CubieCube::set_ep8_from_coordinate(int ep8_coord)
{
    Edge elements[8];
    for (int i = 0; i < 8; ++i)
        elements[i] = static_cast<Edge>(i);

    int factorial = 5040; // 7!
    for (int i = 0; i < 8; ++i)
    {
        int pos = ep8_coord / factorial;
        ep[i] = elements[pos];
        ep8_coord %= factorial;

        for (int j = pos; j < 7 - i; ++j)
        {
            elements[j] = elements[j + 1];
        }
        if (i < 7)
            factorial /= (7 - i);
    }
}

void CubieCube::set_ud_slice_perm_from_coordinate(int ud_slice_perm_coord)
{
    Edge elements[4];
    for (int i = 0; i < 4; ++i)
        elements[i] = static_cast<Edge>(8 + i);

    int factorial = 6; // 3!
    for (int i = 0; i < 4; ++i)
    {
        int pos = ud_slice_perm_coord / factorial;
        ep[8 + i] = elements[pos];
        ud_slice_perm_coord %= factorial;

        for (int j = pos; j < 3 - i; ++j)
        {
            elements[j] = elements[j + 1];
        }
        if (i < 3)
            factorial /= (3 - i);
    }
}

int CubieCube::get_eo_coordinate() const
{
    int eo_coord = 0;
    for (int i = 0; i < EDGE_COUNT - 1; ++i) // EDGE_COUNT - 1 because the last edge's orientation is determined by the others
    {
        eo_coord = (eo_coord << 1) | eo[i];
    }
    return eo_coord;
}

int CubieCube::get_co_coordinate() const
{
    int co_coord = 0;
    for (int i = 0; i < CORNER_COUNT - 1; ++i)
    {
        co_coord = co_coord * 3 + co[i];
    }
    return co_coord;
}

int CubieCube::get_ud_slice_coordinate() const
{
    int coord = 0;
    int k = 3;
    for (int i = 11; i >= 0; --i)
    {
        if (static_cast<int>(ep[i]) >= 8)
        {
            coord += cnk(i, k + 1);
            k--;
            if (k < 0)
                break;
        }
    }
    return coord;
}

int CubieCube::get_cp_coordinate() const
{
    int index = 0;
    int factorial = 5040; // 7!
    for (int i = 0; i < 7; ++i)
    {
        int smaller = 0;
        for (int j = i + 1; j < 8; ++j)
        {
            if (cp[j] < cp[i])
                smaller++;
        }
        index += smaller * factorial;
        factorial /= (7 - i);
    }
    return index;
}

int CubieCube::get_ep8_coordinate() const
{
    int index = 0;
    int factorial = 5040; // 7!
    for (int i = 0; i < 7; ++i)
    {
        int smaller = 0;
        for (int j = i + 1; j < 8; ++j)
        {
            if (ep[j] < ep[i])
                smaller++;
        }
        index += smaller * factorial;
        factorial /= (7 - i);
    }
    return index;
}

int CubieCube::get_ud_slice_perm_coordinate() const
{
    int index = 0;
    int factorial = 6; // 3!
    for (int i = 0; i < 3; ++i)
    {
        int smaller = 0;
        for (int j = i + 1; j < 4; ++j)
        {
            if (ep[8 + j] < ep[8 + i])
                smaller++;
        }
        index += smaller * factorial;
        factorial /= (3 - i);
    }
    return index;
}
