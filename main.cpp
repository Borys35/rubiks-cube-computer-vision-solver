#include <iostream>
#include "corner.h"
#include "edge.h"

int main()
{
    Edge e = Edge::UR;
    std::cout << "Hello world; " << std::endl;
    if (e == Edge::UR)
    {
        std::cout << "Edge is up-right" << std::endl;
    }
    return 0;
}