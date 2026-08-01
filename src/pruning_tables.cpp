#include "pruning_tables.hpp"
#include "moves.hpp"

namespace Kociemba
{
    namespace PruningTables
    {
        std::vector<int> PRUNING_CO_UD(CO_COUNT *UD_SLICE_COUNT, -1);
        std::vector<int> PRUNING_EO_UD(EO_COUNT *UD_SLICE_COUNT, -1);
        std::vector<int> PRUNING_CP_SLICE(CP_COUNT *UD_SLICE_PERM_COUNT, -1);
        std::vector<int> PRUNING_EP8_SLICE(EP8_COUNT *UD_SLICE_PERM_COUNT, -1);

        void generate_co_ud_pruning_table()
        {
            PRUNING_CO_UD[0 * UD_SLICE_COUNT + 0] = 0;
            int depth = 0;
            int populated_count = 1;
            const int total_states = CO_COUNT * UD_SLICE_COUNT;

            while (populated_count < total_states)
            {
                for (int co = 0; co < CO_COUNT; co++)
                {
                    for (int ud = 0; ud < UD_SLICE_COUNT; ud++)
                    {
                        int current_idx = co * UD_SLICE_COUNT + ud;
                        if (PRUNING_CO_UD[current_idx] == depth)
                        {
                            for (int move = 0; move < 18; move++)
                            {
                                int next_co = MoveTables::MOVE_TABLE_CO[co][move];
                                int next_ud = MoveTables::MOVE_TABLE_UD_SLICE[ud][move];
                                int next_idx = next_co * UD_SLICE_COUNT + next_ud;

                                if (PRUNING_CO_UD[next_idx] == -1)
                                {
                                    PRUNING_CO_UD[next_idx] = depth + 1;
                                    populated_count++;
                                }
                            }
                        }
                    }
                }
                depth++;
            }
        }

        void generate_eo_ud_pruning_table()
        {
            PRUNING_EO_UD[0 * UD_SLICE_COUNT + 0] = 0;
            int depth = 0;
            int populated_count = 1;
            const int total_states = EO_COUNT * UD_SLICE_COUNT;

            while (populated_count < total_states)
            {
                for (int eo = 0; eo < EO_COUNT; eo++)
                {
                    for (int ud = 0; ud < UD_SLICE_COUNT; ud++)
                    {
                        int current_idx = eo * UD_SLICE_COUNT + ud;
                        if (PRUNING_EO_UD[current_idx] == depth)
                        {
                            for (int move = 0; move < 18; move++)
                            {
                                int next_eo = MoveTables::MOVE_TABLE_EO[eo][move];
                                int next_ud = MoveTables::MOVE_TABLE_UD_SLICE[ud][move];
                                int next_idx = next_eo * UD_SLICE_COUNT + next_ud;

                                if (PRUNING_EO_UD[next_idx] == -1)
                                {
                                    PRUNING_EO_UD[next_idx] = depth + 1;
                                    populated_count++;
                                }
                            }
                        }
                    }
                }
                depth++;
            }
        }

        void generate_cp_slice_pruning_table()
        {
            PRUNING_CP_SLICE[0 * UD_SLICE_PERM_COUNT + 0] = 0;
            int depth = 0;
            int populated_count = 1;
            const int total_states = CP_COUNT * UD_SLICE_PERM_COUNT;

            while (populated_count < total_states)
            {
                for (int cp = 0; cp < CP_COUNT; cp++)
                {
                    for (int slice = 0; slice < UD_SLICE_PERM_COUNT; slice++)
                    {
                        int current_idx = cp * UD_SLICE_PERM_COUNT + slice;
                        if (PRUNING_CP_SLICE[current_idx] == depth)
                        {
                            for (int i = 0; i < 10; i++)
                            {
                                int move = Kociemba::PHASE_2_MOVES[i];
                                int next_cp = MoveTables::MOVE_TABLE_CP[cp][move];
                                int next_slice = MoveTables::MOVE_TABLE_UD_SLICE_PERM[slice][move];
                                int next_idx = next_cp * UD_SLICE_PERM_COUNT + next_slice;

                                if (PRUNING_CP_SLICE[next_idx] == -1)
                                {
                                    PRUNING_CP_SLICE[next_idx] = depth + 1;
                                    populated_count++;
                                }
                            }
                        }
                    }
                }
                depth++;
            }
        }

        void generate_ep8_slice_pruning_table()
        {
            PRUNING_EP8_SLICE[0 * UD_SLICE_PERM_COUNT + 0] = 0;
            int depth = 0;
            int populated_count = 1;
            const int total_states = EP8_COUNT * UD_SLICE_PERM_COUNT;

            while (populated_count < total_states)
            {
                for (int ep8 = 0; ep8 < EP8_COUNT; ep8++)
                {
                    for (int slice = 0; slice < UD_SLICE_PERM_COUNT; slice++)
                    {
                        int current_idx = ep8 * UD_SLICE_PERM_COUNT + slice;
                        if (PRUNING_EP8_SLICE[current_idx] == depth)
                        {
                            for (int i = 0; i < 10; i++)
                            {
                                int move = Kociemba::PHASE_2_MOVES[i];
                                int next_ep8 = MoveTables::MOVE_TABLE_EP8[ep8][move];
                                int next_slice = MoveTables::MOVE_TABLE_UD_SLICE_PERM[slice][move];
                                int next_idx = next_ep8 * UD_SLICE_PERM_COUNT + next_slice;

                                if (PRUNING_EP8_SLICE[next_idx] == -1)
                                {
                                    PRUNING_EP8_SLICE[next_idx] = depth + 1;
                                    populated_count++;
                                }
                            }
                        }
                    }
                }
                depth++;
            }
        }

        void init_phase1_pruning_tables()
        {
            generate_co_ud_pruning_table();
            generate_eo_ud_pruning_table();
        }

        void init_phase2_pruning_tables()
        {
            generate_cp_slice_pruning_table();
            generate_ep8_slice_pruning_table();
        }

        void init_all_pruning_tables()
        {
            init_phase1_pruning_tables();
            init_phase2_pruning_tables();
        }

        int get_phase1_pruning_value(int current_co, int current_eo, int current_ud)
        {
            int co_ud_dist = PRUNING_CO_UD[current_co * UD_SLICE_COUNT + current_ud];
            int eo_ud_dist = PRUNING_EO_UD[current_eo * UD_SLICE_COUNT + current_ud];
            return std::max(co_ud_dist, eo_ud_dist);
        }

        int get_phase2_pruning_value(int current_cp, int current_ep8, int current_slice)
        {
            int cp_dist = PRUNING_CP_SLICE[current_cp * UD_SLICE_PERM_COUNT + current_slice];
            int ep8_dist = PRUNING_EP8_SLICE[current_ep8 * UD_SLICE_PERM_COUNT + current_slice];
            return std::max(cp_dist, ep8_dist);
        }
    }
}