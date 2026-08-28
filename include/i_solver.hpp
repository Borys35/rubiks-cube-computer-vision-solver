#pragma once

#include <string>
#include <vector>
#include <thread>

#include "cubie_cube.hpp"
#include "moves.hpp"

class ISolver
{
public:
    virtual ~ISolver() = default;

    virtual std::vector<int> solve(const CubieCube &cc, std::stop_token stoken) = 0;
};