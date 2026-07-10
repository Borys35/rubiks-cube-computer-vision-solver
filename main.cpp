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

    std::vector<int> scramble_moves = {R, U_PRIME, F2, L};
    std::cout << "Scramble: " << moves_to_string(scramble_moves) << std::endl;

    std::string scramble_string = "L' R U2";
    scramble_moves = string_to_moves(scramble_string);
    for (int i = 0; i < 3; i++)
    {
        cc.multiply(ALL_MOVES[scramble_moves[i]]);
    }

    // 4 move scramble
    // cc.multiply(ALL_MOVES[R]);
    // cc.multiply(ALL_MOVES[U_PRIME]);
    // cc.multiply(ALL_MOVES[F2]);
    // cc.multiply(ALL_MOVES[L]);

    Visualizer *visualizer = new TerminalVisualizer();

    visualizer->display_cube(cc.to_facelet_cube());

    ISolver *solver = new BFSSolver();
    std::string solve = solver->solve(cc);

    std::cout << "Solve: " << solve << std::endl;

    delete visualizer;
    delete solver;

    return 0;
}