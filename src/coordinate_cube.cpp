#include "coordinate_cube.hpp"

int CoordinateCube::MOVE_TABLE_EO[EO_COORDINATE_COUNT][MOVE_COUNT];
int CoordinateCube::MOVE_TABLE_CO[CO_COORDINATE_COUNT][MOVE_COUNT];
int CoordinateCube::MOVE_TABLE_UD_SLICE[UD_SLICE_COORDINATE_COUNT][MOVE_COUNT];

void CoordinateCube::generate_move_tables()
{
    generate_eo_move_tables();
    generate_co_move_tables();
    generate_ud_slice_move_tables();
}

void CoordinateCube::generate_eo_move_tables()
{
    CubieCube cc;
    for (int i = 0; i < EO_COORDINATE_COUNT; i++)
    {
        cc.set_eo_from_coordinate(i);

        for (int m = 0; m < MOVE_COUNT; m++)
        {
            CubieCube moved_cc = cc;
            moved_cc.multiply(ALL_MOVES[m]);
            MOVE_TABLE_EO[i][m] = moved_cc.get_eo_coordinate();
        }
    }
}

void CoordinateCube::generate_co_move_tables()
{
    CubieCube cc;
    for (int i = 0; i < CO_COORDINATE_COUNT; i++)
    {
        cc.set_co_from_coordinate(i);

        for (int m = 0; m < MOVE_COUNT; m++)
        {
            CubieCube moved_cc = cc;
            moved_cc.multiply(ALL_MOVES[m]);
            MOVE_TABLE_CO[i][m] = moved_cc.get_co_coordinate();
        }
    }
}

void CoordinateCube::generate_ud_slice_move_tables()
{
    CubieCube cc;
    for (int i = 0; i < UD_SLICE_COORDINATE_COUNT; i++)
    {
        cc.set_ud_slice_from_coordinate(i);

        for (int m = 0; m < MOVE_COUNT; m++)
        {
            CubieCube moved_cc = cc;
            moved_cc.multiply(ALL_MOVES[m]);
            MOVE_TABLE_UD_SLICE[i][m] = moved_cc.get_ud_slice_coordinate();
        }
    }
}