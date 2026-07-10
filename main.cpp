#include <iostream>
#include "cubie_cube.hpp"
#include "moves.hpp"
#include "terminal_visualizer.hpp"

void print_if_solved(const CubieCube &c)
{
    std::cout << (c.is_solved() ? "> It's solved!" : "> Not solved yet...") << std::endl;
}

int main()
{
    CubieCube cc = CubieCube();

    cc.multiply(ALL_MOVES[L]);

    Visualizer *visualizer = new TerminalVisualizer();

    visualizer->show_facelet_indices(cc.to_facelet_cube());
    visualizer->display_cube(cc.to_facelet_cube());

    delete visualizer;

    return 0;
}