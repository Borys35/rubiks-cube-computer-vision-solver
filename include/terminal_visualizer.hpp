#pragma once

#include "visualizer.hpp"

class TerminalVisualizer : public Visualizer
{
public:
    TerminalVisualizer() {}
    void display_cube(const FaceletCube &fc) const override;
};