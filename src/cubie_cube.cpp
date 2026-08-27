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

    for (int i = 0; i < CORNER_COUNT; ++i)
    {
        int piece = static_cast<int>(cp[i]);
        int orient = co[i];
        for (int n = 0; n < 3; ++n)
        {
            fc[CORNER_FACELETS[i][(n + orient) % 3]] = CORNER_COLORS[piece][n];
        }
    }

    for (int i = 0; i < EDGE_COUNT; ++i)
    {
        int piece = static_cast<int>(ep[i]);
        int orient = eo[i];
        fc[EDGE_FACELETS[i][orient]] = EDGE_COLORS[piece][0];
        fc[EDGE_FACELETS[i][1 - orient]] = EDGE_COLORS[piece][1];
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

bool CubieCube::is_solvable() const {
	// check if every color appears exactly 9 times
    FaceletCube fc = to_facelet_cube();
    std::array<int, COLOR_COUNT> color_counts = { 0 };

    for (int i = 0; i < 54; ++i) {
        if (fc[i] == UNKNOWN_COLOR || fc[i] >= COLOR_COUNT) return false;
        color_counts[fc[i]]++;
    }

    for (int count : color_counts) {
        if (count != 9) return false;
    }

    // sum check of co
    int sum = 0;
    for (int i = 0; i < CORNER_COUNT; i++) {
        sum += co[i];
    }
    if (sum % 3 != 0)
        return false;

    // sum check of eo
    sum = 0;
    for (int i = 0; i < EDGE_COUNT; i++) {
        sum += eo[i];
    }
    if (sum % 2 != 0)
        return false;
    
    int ep_inv = 0;
    for (int i = 0; i < EDGE_COUNT - 1; i++) {
        for (int j = i + 1; j < EDGE_COUNT; j++) {
            if (ep[i] > ep[j]) ep_inv++;
        }
    }

    int cp_inv = 0;
    for (int i = 0; i < CORNER_COUNT - 1; i++) {
        for (int j = i + 1; j < CORNER_COUNT; j++) {
            if (cp[i] > cp[j]) cp_inv++;
        }
    }

    // edge and corner parity must match
    return (ep_inv % 2) == (cp_inv % 2);
}

void CubieCube::from_facelet_cube(const FaceletCube& fc) {
    for (int i = 0; i < CORNER_COUNT; ++i) {
        int ori = -1;
        for (int x = 0; x < 3; ++x) {
            Color c = fc[CORNER_FACELETS[i][x]];
            if (c == U_COLOR || c == D_COLOR) {
                ori = x;
                break;
            }
        }
        co[i] = ori;

        Color c0 = fc[CORNER_FACELETS[i][ori]];
        Color c1 = fc[CORNER_FACELETS[i][(1 + ori) % 3]];
        Color c2 = fc[CORNER_FACELETS[i][(2 + ori) % 3]];

        for (int p = 0; p < CORNER_COUNT; ++p) {
            if (CORNER_COLORS[p][0] == c0 &&
                CORNER_COLORS[p][1] == c1 &&
                CORNER_COLORS[p][2] == c2) {
                cp[i] = static_cast<Corner>(p);
                break;
            }
        }
    }

    for (int i = 0; i < EDGE_COUNT; ++i) {
        Color c0 = fc[EDGE_FACELETS[i][0]];
        Color c1 = fc[EDGE_FACELETS[i][1]];

        for (int p = 0; p < EDGE_COUNT; ++p) {
            if (EDGE_COLORS[p][0] == c0 && EDGE_COLORS[p][1] == c1) {
                ep[i] = static_cast<Edge>(p);
                eo[i] = 0;
                break;
            }
            if (EDGE_COLORS[p][0] == c1 && EDGE_COLORS[p][1] == c0) {
                ep[i] = static_cast<Edge>(p);
                eo[i] = 1;
                break;
            }
        }
    }
}