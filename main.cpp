#include <iostream>
#include "cubie_cube.hpp"
#include "coordinate_cube.hpp"
#include "moves.hpp"
#include "terminal_visualizer.hpp"
#include "bfs_solver.hpp"

void print_if_solved(const CubieCube &c)
{
    std::cout << (c.is_solved() ? "> It's solved!" : "> Not solved yet...") << std::endl;
}

int main()
{
    CoordinateCube::generate_move_tables(); // must be called first before using CoordinateCube

    CubieCube cc = CubieCube();

    std::string scramble_string = "L' R U2 R";
    std::vector<int> scramble_moves = string_to_moves(scramble_string);
    for (size_t i = 0; i < scramble_moves.size(); i++)
    {
        cc.multiply(ALL_MOVES[scramble_moves[i]]);
    }

    Visualizer *visualizer = new TerminalVisualizer();
    visualizer->display_cube(cc.to_facelet_cube());

    auto max_depth{8u};
    ISolver *solver = new BFSSolver(max_depth);
    std::vector<int> solve_moves = solver->solve(cc);

    // should be "Solve: U2 L R'" or "Solve: U2 R' L"
    std::cout << "Solve: " << moves_to_string(solve_moves) << std::endl;

    delete visualizer;
    delete solver;

    return 0;
}