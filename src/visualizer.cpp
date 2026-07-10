#include <iostream>

#include "visualizer.hpp"

void Visualizer::show_facelet_indices(const FaceletCube &fc) const
{
    std::cout << "Facelets:" << std::endl;
    for (int i = 0; i < fc.size(); i++)
    {
        std::cout << fc[i];
        if (i % 9 == 8)
            std::cout << "\n";
    }
    std::cout << std::endl;
}