#pragma once
// W5 G16 keyboard drive: CSWSCreature::AIActionDrive (0051E6A0) and its movement closure.
#include "58f963adf7-w5_types.hpp"
#include "1a370ea669-w5_foreign.hpp"
#include "4f622be2f2-w5_area_direct_line.hpp"   // TestDirectLine + NoCreaturesOnLine adapter
extern "C" {
using w5::U; using w5::I; using w5::Vector;
double __fastcall w5_creature_get_run_rate_004f1be0(void* creature, void*);                 // RET 0, ST0
double __fastcall w5_creature_get_walk_rate_004f1b20(void* creature, void*);                // RET 0, ST0
// 00561820 GetLastUpdateObject: W2-owned (recovered_last_update_object_00561820, w5_foreign.hpp)
void __fastcall w5_creature_terminate_client_side_path_005110b0(void* creature, void*, U arg); // RET 4 (byte)
void* __fastcall w5_server_get_door_by_id_004ae7c0(void* server, void*, U id);             // RET 4 (-> 004B1920)
void* __fastcall w5_server_internal_get_door_by_id_004b1920(void* internal, void*, U id);  // RET 4
U __fastcall w5_door_get_is_linked_005890d0(void* door, void*);                            // RET 0
U __fastcall w5_area_get_door_crossed_005064d0(void* area, void*, const Vector* from, const Vector* to); // RET 8
void __fastcall w5_creature_auto_close_gui_panels_004eded0(void* creature, void*, U arg);   // RET 4
U __fastcall w5_creature_drive_update_location_0051cd20(void* creature, void*, U run);      // RET 4, action result
U __fastcall w5_action_drive_0051e6a0(void* creature, void*, void* node);                  // RET 4, action result
I __fastcall w5_creature_bump_friends_004f6390(void* creature, void*, void* other, const Vector* from, const Vector* to); // RET 0Ch (t2 body; trap fallback)
U __fastcall w5_item_close_inventory_0055d800(void* item, void*, U id, U flag);             // RET 8 (trap)
U __fastcall w5_placeable_close_inventory_00587560(void* placeable, void*, U id, U flag);   // RET 8 (trap)

// support adapters over existing host bodies
void __fastcall w1x_object_set_orientation_004cd170(void* object, void*, void* direction);   // W1 0886 wrapper (ruling)
// reused existing natives
void* __fastcall unresolved_list_at_pos_005e9b60(void* list, void*, void* position);        // GetAtPos [0168]
unsigned __fastcall unresolved_list_next_005e9b90(void* list, void*, void** position);      // GetNext [0168]
unsigned __fastcall kotor_m15e_slot010_zero_0063e7f0(void* self, void*);                     // return_zero 0063E7F0 [0832]
}
