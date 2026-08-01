#include <iostream>
#include "cubie_cube.hpp"
#include "move_tables.hpp"
#include "moves.hpp"
#include "terminal_visualizer.hpp"
#include "bfs_solver.hpp"
#include "kociemba_solver.hpp"
#include "pruning_tables.hpp"

void print_if_solved(const CubieCube &c)
{
    std::cout << (c.is_solved() ? "> It's solved!" : "> Not solved yet...") << std::endl;
}

int main()
{
    MoveTables::init_move_tables();
    Kociemba::PruningTables::init_all_pruning_tables();

    CubieCube cc = CubieCube();

    std::string scramble_string = "R L F' D' F R' U2 B' R D' R2 B2 U R2 F2 B2 R2 U D2 L2";
    std::cout << "Scramble: " << scramble_string << std::endl;

    std::vector<int> scramble_moves = string_to_moves(scramble_string);
    for (size_t i = 0; i < scramble_moves.size(); i++)
    {
        cc.multiply(ALL_MOVES[scramble_moves[i]]);
    }

    Visualizer *visualizer = new TerminalVisualizer();
    visualizer->display_cube(cc.to_facelet_cube());

    // ISolver *solver = new BFSSolver(max_depth_total);
    auto max_depth_total{22u};
    auto max_depth1{12u};
    ISolver *solver = new Kociemba::KociembaSolver(max_depth_total, max_depth1);
    std::vector<int> solve_moves = solver->solve(cc);

    // Solve: R' U2 R' L
    std::cout << "Solve: " << moves_to_string(solve_moves) << " (" << solve_moves.size() << " moves)" << std::endl;

    delete visualizer;
    delete solver;

    return 0;
}