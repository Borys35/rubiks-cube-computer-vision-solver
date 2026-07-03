#pragma once

#include "cubie_cube.hpp"

class Visualizer
{
public:
    virtual ~Visualizer() = default;

    virtual void display_cube(const FaceletCube &fc) const = 0;
};