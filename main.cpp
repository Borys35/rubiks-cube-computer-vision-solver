#include <iostream>
#include "cubie_cube.hpp"
#include "move_tables.hpp"
#include "moves.hpp"
#include "terminal_visualizer.hpp"
#include "bfs_solver.hpp"
#include "kociemba_solver.hpp"

void print_if_solved(const CubieCube &c)
{
    std::cout << (c.is_solved() ? "> It's solved!" : "> Not solved yet...") << std::endl;
}

int main()
{
    MoveTables::init_move_tables(); // must be called first before using MoveTables

    CubieCube cc = CubieCube();

    std::string scramble_string = "L' R U2 R";
    std::cout << "Scramble: " << scramble_string << std::endl;

    // for (size_t i = 0; i < 18; i++)
    // {
    //     cc.multiply(ALL_MOVES[i]);
    // }

    std::vector<int> scramble_moves = string_to_moves(scramble_string);
    for (size_t i = 0; i < scramble_moves.size(); i++)
    {
        cc.multiply(ALL_MOVES[scramble_moves[i]]);
    }

    Visualizer *visualizer = new TerminalVisualizer();
    visualizer->display_cube(cc.to_facelet_cube());

    auto max_depth{8u};
    // ISolver *solver = new BFSSolver(max_depth);
    ISolver *solver = new Kociemba::KociembaSolver(8);
    std::vector<int> solve_moves = solver->solve(cc);

    // Solve: R' U2 R' L
    std::cout << "Solve: " << moves_to_string(solve_moves) << " (" << solve_moves.size() << " moves)" << std::endl;

    delete visualizer;
    delete solver;

    return 0;
}