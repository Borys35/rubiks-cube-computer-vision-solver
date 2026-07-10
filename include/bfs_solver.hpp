#pragma once

#include "i_solver.hpp"

class BFSSolver : public ISolver
{
public:
    BFSSolver() = default;

    std::vector<int> solve(const CubieCube &cc) override;
};