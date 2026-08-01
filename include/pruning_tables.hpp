#pragma once

#include "cubie_cube.hpp"
#include "move_tables.hpp"
#include "kociemba_solver.hpp"

namespace Kociemba
{
    namespace PruningTables
    {
        // phase 1
        constexpr int CO_COUNT = 2187;
        constexpr int EO_COUNT = 2048;
        constexpr int UD_SLICE_COUNT = 495;

        // phase 2
        constexpr int CP_COUNT = 40320;
        constexpr int EP8_COUNT = 40320;
        constexpr int UD_SLICE_PERM_COUNT = 24;

        void init_phase1_pruning_tables();
        void init_phase2_pruning_tables();
        void init_all_pruning_tables();

        // lookup pruning values
        int get_phase1_pruning_value(int current_co, int current_eo, int current_ud);
        int get_phase2_pruning_value(int current_cp, int current_ep8, int current_slice);
    }
}