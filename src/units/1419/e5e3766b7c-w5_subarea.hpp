#pragma once
#include "58f963adf7-w5_types.hpp"
#include "1a370ea669-w5_foreign.hpp"
extern "C" {
using w5::U; using w5::I; using w5::Vector;
U __fastcall w5_area_intersect_line_segments_0058f840(void* area, void*, float p1x, float p1y, float p2x, float p2y,
    float q1x, float q1y, float q2x, float q2y, float* ox, float* oy);                    // RET 28h
U __fastcall w5_trigger_line_segment_intersect_0058cd80(void* trigger, void*, const Vector* from, const Vector* to); // RET 8
U __fastcall w5_trigger_in_trigger_0058ce40(void* trigger, void*, const Vector* p);       // RET 4
U __fastcall w5_door_in_door_005897d0(void* door, void*, Vector p);                     // RET 0Ch
U __fastcall recovered_aoe_in_area_of_effect_00595450(void* aoe, void*, Vector p);             // W2 export (t3b), trap fallback
U __fastcall recovered_aoe_segment_intersect_005950c0(void* aoe, void*, Vector a, Vector b); // W2 export (t3b), trap fallback
U __fastcall recovered_encounter_in_activate_area_00590090(void* e, void*, const Vector* p);   // W2 export (t3b), trap fallback
U __fastcall recovered_encounter_segment_intersect_0058ffd0(void* e, void*, const Vector* a, const Vector* b); // W2 export (t3b), trap fallback
U __fastcall w5_area_in_sub_areas_004bf470(void* area, void*, const Vector* p, void* out_list);   // RET 8
U __fastcall w5_area_intersecting_line_segment_004bf2c0(void* area, void*, const Vector* a, const Vector* b, void* out_list); // RET 0Ch
U __fastcall w5_creature_handle_sub_area_state_changes_00516020(void* creature, void*, void* now_list, void* crossed_list,
    U trigger_check, U fire);                                                              // RET 10h
U __fastcall w5_creature_update_subareas_on_move_to_0051b7b0(void* creature, void*, const Vector* from, const Vector* to,
    U use_crossed, void* crossed, U trigger_check);                                        // RET 14h
}
