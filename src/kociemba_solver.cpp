#include "kociemba_solver.hpp"

std::vector<int> KociembaSolver::solve(const CubieCube &cc)
{
    std::vector<int> p1_moves{};
    // Phase 1 IDA*
    for (int depth1 = 0; depth1 < max_depth; depth1++)
    {
        search_phase1(cc, depth1, p1_moves);
    }
    return best_solution;
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
    // TODO: implement pruning

    // goal reached
    if (depth_left == 0 && is_cube_in_g1_substate(state))
    {
        int max_depth2 = max_depth - p1_moves.size() - 1;

        std::vector<int> p2_moves{};
        // Phase 2 IDA*
        for (int depth2 = 0; depth2 <= max_depth2; depth2++)
        {
            search_phase2(state, depth2, p1_moves, p2_moves);
        }

        return;
    }

    // DFS
    for (int move = 0; move < MOVE_COUNT; move++)
    {
        if (!p1_moves.empty() && is_redundant_move(p1_moves.back(), static_cast<int>(move)))
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
    // TODO: implement pruning

    // goal reached
    if (depth_left == 0 && state.is_solved())
    {
        best_solution.reserve(p1_moves.size() + p2_moves.size());
        best_solution.insert(best_solution.end(), p1_moves.begin(), p1_moves.end());
        best_solution.insert(best_solution.end(), p2_moves.begin(), p2_moves.end());

        return;
    }

    // DFS for restricted moves only (R2, L2, F2, B2, all U's, all D's)
    for (auto move : p2_restricted_moves)
    {
        if (!p2_moves.empty() && is_redundant_move(p2_moves.back(), static_cast<int>(move)))
            continue;

        CubieCube next_state = state;
        next_state.multiply(ALL_MOVES[move]);
        p2_moves.push_back(move);

        search_phase2(next_state, depth_left - 1, p1_moves, p2_moves);

        p2_moves.pop_back(); // remove last move to backtrack
    }
}