#include <iostream>
#include "cubie_cube.hpp"

int main()
{
    Edge e = Edge::BL;
    CubieCube cc = CubieCube();
    std::cout << "Hello world; " << static_cast<int>(cc.cp[2]) << std::endl;
    if (e == Edge::UR)
    {
        std::cout << "Edge is up-right" << std::endl;
    }
    return 0;
}