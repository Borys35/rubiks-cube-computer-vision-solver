#pragma once

#include <string>
#include <vector>

#include "cubie_cube.hpp"
#include "moves.hpp"

class ISolver
{
public:
    virtual ~ISolver() = default;

    virtual std::vector<int> solve(const CubieCube &cc) = 0;
};