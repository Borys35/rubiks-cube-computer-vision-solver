#pragma once

#include "i_solver.hpp"

class BFSSolver : public ISolver
{
private:
    int max_depth;

public:
    BFSSolver(int max_depth) : max_depth(max_depth) {}

    std::vector<int> solve(const CubieCube &cc, std::stop_token stoken) override;
};