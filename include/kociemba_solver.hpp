#pragma once

#include "i_solver.hpp"
#include "move_tables.hpp"

namespace Kociemba
{
    const int PHASE_2_MOVES[10] = {U, U2, U_PRIME, D, D2, D_PRIME, R2, L2, F2, B2};

    class KociembaSolver : public ISolver
    {
    private:
        int max_depth_total{25}; // max moves allowed
        int max_depth1{15};      // max moves allowed for a solution to be considered acceptable
        std::vector<int> best_solution;

        bool is_cube_in_g1_substate(const CubieCube &cc);
        void search_phase1(const CubieCube &state, int depth_left, std::vector<int> &p1_moves);
        void search_phase2(const CubieCube &state, int depth_left, std::vector<int> &p1_moves, std::vector<int> &p2_moves);

    public:
        KociembaSolver(int max_depth_total, int max_depth1) : max_depth_total(max_depth_total), max_depth1(max_depth1)
        {
        }

        std::vector<int> solve(const CubieCube &cc) override;
    };
}