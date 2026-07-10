#pragma once

#include "i_solver.hpp"

class BFSSolver : public ISolver
{
public:
    BFSSolver() = default;

    std::string solve(const CubieCube &cc) override;
};