#include <iostream>
#include "cubie_cube.hpp"
#include "moves.hpp"
#include "terminal_visualizer.hpp"
#include "bfs_solver.hpp"

void print_if_solved(const CubieCube &c)
{
    std::cout << (c.is_solved() ? "> It's solved!" : "> Not solved yet...") << std::endl;
}

int main()
{
    CubieCube cc = CubieCube();

    // 4 move scramble
    cc.multiply(ALL_MOVES[R]);
    cc.multiply(ALL_MOVES[U_PRIME]);
    cc.multiply(ALL_MOVES[F2]);
    cc.multiply(ALL_MOVES[L]);

    Visualizer *visualizer = new TerminalVisualizer();

    visualizer->display_cube(cc.to_facelet_cube());

    ISolver *solver = new BFSSolver();
    std::string solve = solver->solve(cc);

    std::cout << "Solve: " << solve << std::endl;

    delete visualizer;
    delete solver;

    return 0;
}