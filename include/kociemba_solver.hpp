#pragma once

#include "i_solver.hpp"

class KociembaSolver : public ISolver
{
private:
    int max_depth;

public:
    KociembaSolver(int max_depth) : max_depth(max_depth) {}

    std::vector<int> solve(const CubieCube &cc) override;
};