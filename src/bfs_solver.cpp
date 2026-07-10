#include <queue>
#include <vector>

#include "bfs_solver.hpp"

std::vector<int> BFSSolver::solve(const CubieCube &cc)
{
    // {Current Cube State, Moves History}
    std::queue<std::pair<CubieCube, std::vector<int>>> q;
    q.push({cc, {}});

    while (!q.empty())
    {
        auto [current_cube, move_history] = q.front();
        q.pop();

        if (current_cube.is_solved())
        {
            return move_history;
        }

        // depth = 6 for now
        if (move_history.size() >= 6)
        {
            continue;
        }

        for (int m = 0; m < 18; ++m)
        {
            // pruning. Not allowing same moves (e.g. U -> U' -> U2)
            if (!move_history.empty() && (move_history.back() / 3 == m / 3))
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