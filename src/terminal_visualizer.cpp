#include <iostream>
#include "terminal_visualizer.hpp"

#include "terminal_visualizer.hpp"
#include <iostream>

void TerminalVisualizer::display_cube(const FaceletCube &fc) const
{
    auto print_sticker = [](Color c)
    {
        switch (c)
        {
        case U_COLOR:
            std::cout << "\033[48;5;15m  \033[0m";
            break;
        case D_COLOR:
            std::cout << "\033[48;5;11m  \033[0m";
            break;
        case F_COLOR:
            std::cout << "\033[48;5;10m  \033[0m";
            break;
        case B_COLOR:
            std::cout << "\033[48;5;12m  \033[0m";
            break;
        case L_COLOR:
            std::cout << "\033[48;5;208m  \033[0m";
            break;
        case R_COLOR:
            std::cout << "\033[48;5;9m  \033[0m";
            break;
        }
    };

    std::cout << "\n\n";

    for (int r = 0; r < 3; ++r)
    {
        std::cout << "      ";
        for (int c = 0; c < 3; ++c)
        {
            print_sticker(fc[0 * 9 + r * 3 + c]);
        }
        std::cout << "\n";
    }

    int middle_faces[4] = {4, 2, 1, 5};

    for (int r = 0; r < 3; ++r)
    {
        for (int face_idx : middle_faces)
        {
            for (int c = 0; c < 3; ++c)
            {
                print_sticker(fc[face_idx * 9 + r * 3 + c]);
            }
        }
        std::cout << "\n";
    }

    for (int r = 0; r < 3; ++r)
    {
        std::cout << "      ";
        for (int c = 0; c < 3; ++c)
        {
            print_sticker(fc[3 * 9 + r * 3 + c]);
        }
        std::cout << "\n";
    }

    std::cout << "\n";
}