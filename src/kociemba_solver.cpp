#include "include/kociemba_solver.hpp"
#include "include/pruning_tables.hpp"
#include <iostream>

namespace Kociemba
{
    int max_depth = 0;

    std::vector<int> KociembaSolver::solve(const CubieCube &cc)
    {
        best_solution.clear();
        max_depth = max_depth_total;
        std::vector<int> p1_moves{};
        // Phase 1 IDA*
        for (int depth1 = 0; depth1 <= max_depth; depth1++)
        {
            if (!best_solution.empty() && best_solution.size() <= acceptable_length)
            {
                break; // already found a solution within the max length
            }
            std::cout << "Searching phase 1 with depth " << depth1 << std::endl;
            search_phase1(cc, depth1, p1_moves);
        }
	    optimize_solution(best_solution);
        return best_solution;
    }

	void KociembaSolver::optimize_solution(std::vector<int>& solution)
	{
        size_t write_idx = 0;

        for (int m : solution) {
            if (write_idx == 0 || solution[write_idx - 1] / 3 != m / 3) {
                solution[write_idx++] = m;
            }
            else {
                int last = solution[--write_idx];
                int turns = ((last % 3) + 1 + (m % 3) + 1) % 4;

                if (turns != 0) {
                    solution[write_idx++] = (last / 3) * 3 + (turns - 1);
                }
            }
        }

		std::cout << "Optimized solution length: " << write_idx << std::endl;
        solution.resize(write_idx);
	}

    bool KociembaSolver::is_cube_in_g1_substate(const CubieCube &cc)
    {
        for (size_t i = 0; i < CORNER_COUNT; i++)
        {
            if (cc.co[i] != 0)
                return false;
        }

        for (size_t i = 0; i < EDGE_COUNT; i++)
        {
            if (cc.eo[i] != 0)
                return false;
        }

        for (size_t i = 8; i < 12; i++)
        {
            if (static_cast<int>(cc.ep[i]) < 8 || static_cast<int>(cc.ep[i]) > 11)
            {
                return false;
            }
        }

        return true;
    }

    void KociembaSolver::search_phase1(const CubieCube &state, int depth_left, std::vector<int> &p1_moves)
    {
		if (!best_solution.empty() && best_solution.size() <= acceptable_length)
		{
			return; // already found a solution within the max length
		}
        // pruning
        int co = state.get_co_coordinate();
        int eo = state.get_eo_coordinate();
        int ud = state.get_ud_slice_coordinate();

        int heuristic = PruningTables::get_phase1_pruning_value(co, eo, ud);

        if (heuristic > depth_left)
        {
            return; // not enough depth left to reach goal
        }

        // goal reached
        if (heuristic == 0)
        {
            int max_depth2 = max_depth - p1_moves.size();

            std::vector<int> p2_moves{};
            // Phase 2 IDA*
            for (int depth2 = 0; depth2 <= max_depth2; depth2++)
            {
                search_phase2(state, depth2, p1_moves, p2_moves);
            }

            return;
        }

        if (depth_left <= 0)
        {
            return; // not in g state
        }

        // DFS
        for (int move = 0; move < MOVE_COUNT; move++)
        {
            if (!p1_moves.empty() && is_redundant_move(p1_moves.back(), move))
                continue;

            CubieCube next_state = state;
            next_state.multiply(ALL_MOVES[move]);
            p1_moves.push_back(move);

            search_phase1(next_state, depth_left - 1, p1_moves);

            p1_moves.pop_back(); // remove last move to backtrack
        }
    }

    void KociembaSolver::search_phase2(const CubieCube &state, int depth_left, std::vector<int> &p1_moves, std::vector<int> &p2_moves)
    {
        if (!best_solution.empty() && best_solution.size() <= acceptable_length)
        {
            return; // already found a solution within the max length
        }
        // pruning
        int cp = state.get_cp_coordinate();
        int ep8 = state.get_ep8_coordinate();
        int slice = state.get_ud_slice_perm_coordinate();

        int heuristic = PruningTables::get_phase2_pruning_value(cp, ep8, slice);

        if (heuristic > depth_left)
        {
            return; // not enough depth left to reach goal
        }

        // goal reached
        if (heuristic == 0)
        {
            if (best_solution.empty() || p1_moves.size() + p2_moves.size() < best_solution.size())
            {
                std::cout << "Found new solution with " << p1_moves.size() + p2_moves.size() << " moves." << std::endl;
                best_solution.clear();
                best_solution.reserve(p1_moves.size() + p2_moves.size());
                best_solution.insert(best_solution.end(), p1_moves.begin(), p1_moves.end());
                best_solution.insert(best_solution.end(), p2_moves.begin(), p2_moves.end());

                // shrink down total depth
                max_depth = best_solution.size() - 1;
            }
            return;
        }

        if (depth_left <= 0)
        {
            return;
        }

        // DFS for restricted moves only (R2, L2, F2, B2, all U's, all D's)
        for (auto move : PHASE_2_MOVES)
        {
            if (!p2_moves.empty() && is_redundant_move(p2_moves.back(), static_cast<int>(move)))
                continue;

            CubieCube next_state = state;
            next_state.multiply(ALL_MOVES[static_cast<int>(move)]);
            p2_moves.push_back(static_cast<int>(move));

            search_phase2(next_state, depth_left - 1, p1_moves, p2_moves);

            p2_moves.pop_back(); // remove last move to backtrack
        }
    }

}