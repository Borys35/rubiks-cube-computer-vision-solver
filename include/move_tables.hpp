#pragma once

#include "cubie_cube.hpp"
#include "moves.hpp"

namespace MoveTables
{
    constexpr int EO_COORDINATE_COUNT = 2048;
    constexpr int CO_COORDINATE_COUNT = 2187;
    constexpr int UD_SLICE_COORDINATE_COUNT = 495;

    constexpr int CP_COORDINATE_COUNT = 40320;
    constexpr int EP8_COORDINATE_COUNT = 40320;
    constexpr int UD_SLICE_PERM_COORDINATE_COUNT = 24;

    extern int MOVE_TABLE_EO[EO_COORDINATE_COUNT][MOVE_COUNT];
    extern int MOVE_TABLE_CO[CO_COORDINATE_COUNT][MOVE_COUNT];
    extern int MOVE_TABLE_UD_SLICE[UD_SLICE_COORDINATE_COUNT][MOVE_COUNT];

    extern int MOVE_TABLE_CP[CP_COORDINATE_COUNT][MOVE_COUNT];
    extern int MOVE_TABLE_EP8[EP8_COORDINATE_COUNT][MOVE_COUNT];
    extern int MOVE_TABLE_UD_SLICE_PERM[UD_SLICE_PERM_COORDINATE_COUNT][MOVE_COUNT];

    void init_move_tables();
    void init_eo_move_tables();
    void init_co_move_tables();
    void init_ud_slice_move_tables();
    void init_cp_move_tables();
    void init_ep8_move_tables();
    void init_ud_slice_perm_move_tables();
}