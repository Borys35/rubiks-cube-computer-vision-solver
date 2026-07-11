#pragma once

#include "cubie_cube.hpp"
#include "moves.hpp"

constexpr int EO_COORDINATE_COUNT = 2048;
constexpr int CO_COORDINATE_COUNT = 2187;
constexpr int UD_SLICE_COORDINATE_COUNT = 495;

class CoordinateCube
{
public:
    static int MOVE_TABLE_EO[EO_COORDINATE_COUNT][MOVE_COUNT];
    static int MOVE_TABLE_CO[CO_COORDINATE_COUNT][MOVE_COUNT];
    static int MOVE_TABLE_UD_SLICE[UD_SLICE_COORDINATE_COUNT][MOVE_COUNT];
    static void generate_move_tables();

private:
    static void generate_eo_move_tables();
    static void generate_co_move_tables();
    static void generate_ud_slice_move_tables();
};