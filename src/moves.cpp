#include "moves.hpp"

CubieCube create_base_move(MoveIndex move_idx)
{
    CubieCube c = CubieCube();

    // Correct order of corners and edges permutations are defined in corner.hpp and edge.hpp
    switch (move_idx)
    {
    case U:
        c.cp = {Corner::UFL, Corner::ULB, Corner::UBR, Corner::URF, Corner::DRB, Corner::DFL, Corner::DLB, Corner::DBR};
        c.co = {0, 0, 0, 0, 0, 0, 0, 0};
        c.ep = {Edge::UF, Edge::UL, Edge::UB, Edge::UR, Edge::DR, Edge::DF, Edge::DL, Edge::DB, Edge::FR, Edge::FL, Edge::BL, Edge::BR};
        c.eo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        break;

    case D:
        c.cp = {Corner::URF, Corner::UFL, Corner::ULB, Corner::UBR, Corner::DLB, Corner::DRB, Corner::DBR, Corner::DFL};
        c.co = {0, 0, 0, 0, 0, 0, 0, 0};
        c.ep = {Edge::UR, Edge::UF, Edge::UL, Edge::UB, Edge::DF, Edge::DL, Edge::DB, Edge::DR, Edge::FR, Edge::FL, Edge::BL, Edge::BR};
        c.eo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        break;

    case R:
        c.cp = {Corner::UBR, Corner::UFL, Corner::ULB, Corner::DBR, Corner::URF, Corner::DFL, Corner::DLB, Corner::DRB};
        c.co = {1, 0, 0, 2, 1, 0, 0, 2};
        c.ep = {Edge::FR, Edge::UF, Edge::UL, Edge::UB, Edge::BR, Edge::DF, Edge::DL, Edge::DB, Edge::DR, Edge::FL, Edge::BL, Edge::UR};
        c.eo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        break;

    case L:
        c.cp = {Corner::URF, Corner::ULB, Corner::DLB, Corner::UBR, Corner::DRB, Corner::UFL, Corner::DFL, Corner::DBR};
        c.co = {0, 1, 2, 0, 0, 2, 1, 0};
        c.ep = {Edge::UR, Edge::UF, Edge::BL, Edge::UB, Edge::DR, Edge::DF, Edge::FL, Edge::DB, Edge::FR, Edge::UL, Edge::DL, Edge::BR};
        c.eo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        break;

    case F:
        c.cp = {Corner::UFL, Corner::DFL, Corner::ULB, Corner::UBR, Corner::DRB, Corner::URF, Corner::DLB, Corner::DBR};
        c.co = {1, 2, 0, 0, 0, 1, 0, 2};
        c.ep = {Edge::UR, Edge::FL, Edge::UL, Edge::UB, Edge::DR, Edge::FR, Edge::DL, Edge::DB, Edge::UF, Edge::DF, Edge::BL, Edge::BR};
        c.eo = {0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0};
        break;

    case B:
        c.cp = {Corner::URF, Corner::UFL, Corner::UBR, Corner::DBR, Corner::DLB, Corner::DFL, Corner::ULB, Corner::DRB};
        c.co = {0, 0, 1, 2, 2, 0, 1, 0};
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