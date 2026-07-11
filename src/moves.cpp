#include "moves.hpp"
#include <sstream>
#include <unordered_map>

CubieCube create_base_move(MoveIndex move_idx)
{
    CubieCube c = CubieCube();
    // Correct order of corners and edges permutations are defined in corner.hpp and edge.hpp
    switch (move_idx)
    {
    case U:
        c.cp = {Corner::UBR, Corner::URF, Corner::UFL, Corner::ULB, Corner::DFR, Corner::DFL, Corner::DLB, Corner::DBR};
        c.co = {0, 0, 0, 0, 0, 0, 0, 0};
        c.ep = {Edge::UB, Edge::UR, Edge::UF, Edge::UL, Edge::DR, Edge::DF, Edge::DL, Edge::DB, Edge::FR, Edge::FL, Edge::BL, Edge::BR};
        c.eo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        break;
    case R:
        c.cp = {Corner::DFR, Corner::UFL, Corner::ULB, Corner::URF, Corner::DBR, Corner::DFL, Corner::DLB, Corner::UBR};
        c.co = {2, 0, 0, 1, 1, 0, 0, 2};
        c.ep = {Edge::FR, Edge::UF, Edge::UL, Edge::UB, Edge::BR, Edge::DF, Edge::DL, Edge::DB, Edge::DR, Edge::FL, Edge::BL, Edge::UR};
        c.eo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        break;
    case F:
        c.cp = {Corner::UFL, Corner::DFL, Corner::ULB, Corner::UBR, Corner::URF, Corner::DFR, Corner::DLB, Corner::DBR};
        c.co = {1, 2, 0, 0, 2, 1, 0, 0};
        c.ep = {Edge::UR, Edge::FL, Edge::UL, Edge::UB, Edge::DR, Edge::FR, Edge::DL, Edge::DB, Edge::UF, Edge::DF, Edge::BL, Edge::BR};
        c.eo = {0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0};
        break;
    case D:
        c.cp = {Corner::URF, Corner::UFL, Corner::ULB, Corner::UBR, Corner::DFL, Corner::DLB, Corner::DBR, Corner::DFR};
        c.co = {0, 0, 0, 0, 0, 0, 0, 0};
        c.ep = {Edge::UR, Edge::UF, Edge::UL, Edge::UB, Edge::DF, Edge::DL, Edge::DB, Edge::DR, Edge::FR, Edge::FL, Edge::BL, Edge::BR};
        c.eo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        break;
    case L:
        c.cp = {Corner::URF, Corner::ULB, Corner::DLB, Corner::UBR, Corner::DFR, Corner::UFL, Corner::DFL, Corner::DBR};
        c.co = {0, 1, 2, 0, 0, 2, 1, 0};
        c.ep = {Edge::UR, Edge::UF, Edge::BL, Edge::UB, Edge::DR, Edge::DF, Edge::FL, Edge::DB, Edge::FR, Edge::UL, Edge::DL, Edge::BR};
        c.eo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        break;
    case B:
        c.cp = {Corner::URF, Corner::UFL, Corner::UBR, Corner::DBR, Corner::DFR, Corner::DFL, Corner::ULB, Corner::DLB};
        c.co = {0, 0, 1, 2, 0, 0, 2, 1};
        c.ep = {Edge::UR, Edge::UF, Edge::UL, Edge::BR, Edge::DR, Edge::DF, Edge::DL, Edge::BL, Edge::FR, Edge::FL, Edge::UB, Edge::DB};
        c.eo = {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1};
        break;
    default:
        break;
    }
    return c;
}
const std::array<CubieCube, MOVE_COUNT> ALL_MOVES = []()
{
    std::array<CubieCube, MOVE_COUNT> moves;
    moves[U] = create_base_move(U);
    moves[D] = create_base_move(D);
    moves[R] = create_base_move(R);
    moves[L] = create_base_move(L);
    moves[F] = create_base_move(F);
    moves[B] = create_base_move(B);
    std::array<MoveIndex, 6> base_indices = {U, D, R, L, F, B};
    for (auto base : base_indices)
    {
        // X2 = X * X
        CubieCube x2 = moves[base];
        x2.multiply(moves[base]);
        moves[base + 1] = x2;
        // X' = X2 * X
        CubieCube x_prime = x2;
        x_prime.multiply(moves[base]);
        moves[base + 2] = x_prime;
    }
    return moves;
}();

static const std::string MOVE_NAMES[18] = {
    "U", "U2", "U'",
    "D", "D2", "D'",
    "R", "R2", "R'",
    "L", "L2", "L'",
    "F", "F2", "F'",
    "B", "B2", "B'"};

std::string moves_to_string(const std::vector<int> &moves)
{
    std::string result;
    for (size_t i = 0; i < moves.size(); ++i)
    {
        if (moves[i] >= 0 && moves[i] < 18)
        {
            result += MOVE_NAMES[moves[i]];
            if (i != moves.size() - 1)
            {
                result += " ";
            }
        }
    }
    return result;
}

std::vector<int> string_to_moves(const std::string move_string)
{
    static const std::unordered_map<std::string, int> STRING_TO_MOVE = {
        {"U", U}, {"U2", U2}, {"U'", U_PRIME}, {"D", D}, {"D2", D2}, {"D'", D_PRIME}, {"R", R}, {"R2", R2}, {"R'", R_PRIME}, {"L", L}, {"L2", L2}, {"L'", L_PRIME}, {"F", F}, {"F2", F2}, {"F'", F_PRIME}, {"B", B}, {"B2", B2}, {"B'", B_PRIME}};

    std::vector<int> result;
    std::istringstream iss(move_string);
    std::string token;

    // Automatically splits by spaces
    while (iss >> token)
    {
        auto it = STRING_TO_MOVE.find(token);
        if (it != STRING_TO_MOVE.end())
        {
            result.push_back(it->second);
        }
    }

    return result;
}

bool is_redundant_move(int m1, int m2)
{
    if (m1 / 3 == m2 / 3)
        return true;
    return false;
}