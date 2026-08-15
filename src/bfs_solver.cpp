#include <queue>

#include "include/bfs_solver.hpp"

std::vector<int> BFSSolver::solve(const CubieCube &cc)
{
    // {Current Cube State, Moves History}
    std::queue<std::pair<CubieCube, std::vector<int>>> q;
    q.push({cc, {}});

    while (!q.empty())
    {
        std::pair<CubieCube, std::vector<int>> current_state = q.front();
        CubieCube current_cube = current_state.first;
        std::vector<int> move_history = current_state.second;
        q.pop();

        if (current_cube.is_solved())
        {
            return move_history;
        }

        if (move_history.size() >= max_depth)
        {
            continue;
        }

        for (int m = 0; m < 18; ++m)
        {
            // pruning. Not allowing same moves (e.g. U -> U' -> U2)
            if (!move_history.empty() && (is_redundant_move(move_history.back(), m)))
            {
                continue;
            }

            CubieCube next_cube = current_cube;
            next_cube.multiply(ALL_MOVES[m]);

            std::vector<int> next_history = move_history;
            next_history.push_back(m);

            q.push({next_cube, next_history});
        }
    }

    return {};
}