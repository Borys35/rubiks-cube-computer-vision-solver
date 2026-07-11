#pragma once

#include "i_solver.hpp"

class KociembaSolver : public ISolver
{
private:
    int max_depth{25}; // max moves allowed
    std::vector<int> best_solution;
    int p2_restricted_moves[10];

    bool is_cube_in_g1_substate(const CubieCube &cc);
    void search_phase1(const CubieCube &state, int depth_left, std::vector<int> &p1_moves);
    void search_phase2(const CubieCube &state, int depth_left, std::vector<int> &p1_moves, std::vector<int> &p2_moves);

public:
    KociembaSolver(int max_depth) : max_depth(max_depth)
    {
        p2_restricted_moves[0] = U;
        p2_restricted_moves[1] = U2;
        p2_restricted_moves[2] = U_PRIME;
        p2_restricted_moves[3] = D;
        p2_restricted_moves[4] = D2;
        p2_restricted_moves[5] = D_PRIME;
        p2_restricted_moves[6] = R;
        p2_restricted_moves[7] = L;
        p2_restricted_moves[8] = F;
        p2_restricted_moves[9] = B;
    }

    std::vector<int> solve(const CubieCube &cc) override;
};