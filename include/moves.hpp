#pragma once

#include <array>
#include <string>
#include <vector>
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

std::string moves_to_string(const std::vector<int> &moves);
std::vector<int> string_to_moves(const std::string move_string);
bool is_redundant_move(int m1, int m2);