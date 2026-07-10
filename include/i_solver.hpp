#pragma once

#include <string>

#include "cubie_cube.hpp"

class ISolver
{
public:
    virtual ~ISolver() = default;

    virtual std::string solve(const CubieCube &cc) = 0;
};