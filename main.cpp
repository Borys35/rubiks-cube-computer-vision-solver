#include <iostream>
#include "cubie_cube.hpp"
#include "moves.hpp"

void print_if_solved(CubieCube *c)
{
    std::cout << ((*c).is_solved() ? "> It's solved!" : "> Not solved yet...") << std::endl;
}

int main()
{
    CubieCube cc = CubieCube();
    std::cout << "Is my cube solved?" << std::endl;
    print_if_solved(&cc);
    std::cout << "Doing L2 move" << std::endl;
    cc.multiply(ALL_MOVES[L2]);
    print_if_solved(&cc);
    std::cout << "Doing L2 move again" << std::endl;
    cc.multiply(ALL_MOVES[L2]);
    print_if_solved(&cc);

    return 0;
}