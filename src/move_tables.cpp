#include "move_tables.hpp"
#include "kociemba_solver.hpp"

int MoveTables::MOVE_TABLE_EO[EO_COORDINATE_COUNT][MOVE_COUNT];
int MoveTables::MOVE_TABLE_CO[CO_COORDINATE_COUNT][MOVE_COUNT];
int MoveTables::MOVE_TABLE_UD_SLICE[UD_SLICE_COORDINATE_COUNT][MOVE_COUNT];
int MoveTables::MOVE_TABLE_CP[CP_COORDINATE_COUNT][MOVE_COUNT];
int MoveTables::MOVE_TABLE_EP8[EP8_COORDINATE_COUNT][MOVE_COUNT];
int MoveTables::MOVE_TABLE_UD_SLICE_PERM[UD_SLICE_PERM_COORDINATE_COUNT][MOVE_COUNT];

void MoveTables::init_move_tables()
{
    init_eo_move_tables();
    init_co_move_tables();
    init_ud_slice_move_tables();
    init_cp_move_tables();
    init_ep8_move_tables();
    init_ud_slice_perm_move_tables();
}

void MoveTables::init_eo_move_tables()
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

void MoveTables::init_co_move_tables()
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

void MoveTables::init_ud_slice_move_tables()
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

void MoveTables::init_cp_move_tables()
{
    CubieCube cc;
    for (int i = 0; i < CP_COORDINATE_COUNT; i++)
    {
        cc.set_cp_from_coordinate(i);

        for (auto m : Kociemba::PHASE_2_MOVES)
        {
            CubieCube moved_cc = cc;
            moved_cc.multiply(ALL_MOVES[m]);
            MOVE_TABLE_CP[i][m] = moved_cc.get_cp_coordinate();
        }
    }
}

void MoveTables::init_ep8_move_tables()
{
    CubieCube cc;
    for (int i = 0; i < EP8_COORDINATE_COUNT; i++)
    {
        cc.set_ep8_from_coordinate(i);

        for (auto m : Kociemba::PHASE_2_MOVES)
        {
            CubieCube moved_cc = cc;
            moved_cc.multiply(ALL_MOVES[m]);
            MOVE_TABLE_EP8[i][m] = moved_cc.get_ep8_coordinate();
        }
    }
}

void MoveTables::init_ud_slice_perm_move_tables()
{
    CubieCube cc;
    for (int i = 0; i < UD_SLICE_PERM_COORDINATE_COUNT; i++)
    {
        cc.set_ud_slice_perm_from_coordinate(i);

        for (auto m : Kociemba::PHASE_2_MOVES)
        {
            CubieCube moved_cc = cc;
            moved_cc.multiply(ALL_MOVES[m]);
            MOVE_TABLE_UD_SLICE_PERM[i][m] = moved_cc.get_ud_slice_perm_coordinate();
        }
    }
}