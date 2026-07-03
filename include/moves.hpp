#pragma once

#include <array>
#include "cubie_cube.hpp"

enum MoveIndex
{
    U,
    U2,
    U_PRIME,
    D,
    D2,
    D_PRIME,
    R,
    R2,
    R_PRIME,
    L,
    L2,
    L_PRIME,
    F,
    F2,
    F_PRIME,
    B,
    B2,
    B_PRIME
};

constexpr int MOVE_COUNT = 18;

extern const std::array<CubieCube, MOVE_COUNT> ALL_MOVES;