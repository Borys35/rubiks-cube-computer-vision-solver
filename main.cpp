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
    FaceletCube fc = cc.to_facelet_cube();

    for (int i = 0; i < fc.size(); i++)
    {
        std::cout << fc[i];
    }

    // std::cout << "Is my cube solved?" << std::endl;
    // print_if_solved(cc);
    // std::cout << "Doing L2 move" << std::endl;
    cc.multiply(ALL_MOVES[U]);
    // print_if_solved(cc);
    // std::cout << "Doing L2 move again" << std::endl;
    // cc.multiply(ALL_MOVES[L2]);
    // print_if_solved(cc);

    Visualizer *visualizer = new TerminalVisualizer();

    visualizer->display_cube(cc.to_facelet_cube());

    delete visualizer;

    return 0;
}