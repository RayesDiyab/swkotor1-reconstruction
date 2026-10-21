#pragma once
// Nested provider successors for camera-A/action-input.
// Exact selected provider symbols and the ten owned original-address exports.
// This header declares; it defines no storage, provider body or cell.

#include <cstdint>

using U32 = std::uint32_t;

extern "C" {
// Accepted selected providers (v5 MAP; exact C symbols).
void* __fastcall frame_R4_005ed5b0(void*, void*, U32);              // 005ED5B0  @frame_R4_005ed5b0@12
void  __fastcall kotor_fastcall_00611700(void*, void*, U32, U32);  // 00611700  @kotor_fastcall_00611700@16
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void*, void*); // 0060FB20
int   __fastcall recovered_creature_incapacitated_004ef890(void*, void*);        // 004EF890
void* __fastcall recovered_creature_client_creature_004efb20(void*, void*);      // 004EFB20
void* __fastcall gob_get_property_0x68_0063c070(void*, void*);                    // 0063C070
void* __fastcall kotor_fastcall_005f1420(void*, void*);                            // 005F1420
U32   __fastcall kotor_m15a_creature_get_equipped_item_id_0060ca80(void*, void*, U32); // 0060CA80
void* __fastcall kotor_fastcall_005ed580(void*, void*, U32);                       // 005ED580
void  __fastcall kotor_m15a_item_power_item_00647130(void*, void*, U32, U32, U32, U32); // 00647130
void  __fastcall recovered_creature_set_combat_state_004f2610(void*, void*, U32, U32);  // 004F2610
void  __fastcall kotor_fastcall_0063c760(void*, void*, U32, U32, float, U32, U32);      // 0063C760
void* __fastcall kotor_fastcall_005ed8b0(void*, void*);                             // 005ED8B0
void* __fastcall client_get_player_creature_005ed540(void*, void*);                 // 005ED540
void  __fastcall recovered_client_creature_play_sound_set_sound_00611470(void*, void*, U32, U32); // 00611470
int   __cdecl    crt_rand_006fc447(void);                                          // 006FC447
void* __fastcall kotor_fastcall_00635460(void*, void*);                             // 00635460
void  __fastcall w5_object_clear_all_actions_004ccd80(void*, void*, U32);           // 004CCD80

// 006774E0 message chain providers.
void __fastcall kotor_fastcall_004d68a0(void*, void*, U32, U32, U32);  // @kotor_fastcall_004d68a0@20
void __fastcall kotor_fastcall_004d6340(void*, void*, U32, U32);       // @kotor_fastcall_004d6340@16
void __fastcall kotor_fastcall_004d6960(void*, void*, U32, U32);       // @kotor_fastcall_004d6960@16
U32  __fastcall kotor_fastcall_004d6440(void*, void*, void**, U32*);   // @kotor_fastcall_004d6440@16
U32  __fastcall recovered_client_send_00677410(void*, void*, U32, U32, void*, U32); // @recovered_client_send_00677410@24

// Allocator/free: game CRT heap, not host malloc/free.
void __cdecl live_free(void*);                                         // _live_free

// Owned exports (ten original addresses).
void __fastcall kotor_nested_005f4460(void*, void*);                   // @8
void __fastcall kotor_nested_00674090(void*, void*);                   // @8
U32  __fastcall kotor_nested_005ed670(void*, void*, U32, void*, void*); // @20, RET 0xC
void __fastcall kotor_nested_006c24a0(void*, void*, U32);              // @12, RET 4
void* __fastcall kotor_thunk_00687fd0(void*, void*);                   // @8
void __fastcall kotor_nested_005f4d60(void*, void*, void*);            // @12, RET 4
void __fastcall kotor_nested_006880c0(void*, void*);                   // @8
U32  __fastcall kotor_nested_004d37b0(void*, void*);                   // @8, EAX 0/1
U32  __fastcall kotor_nested_005eea90(void*, void*, U32, void*, void*); // @20, RET 0xC
U32  __fastcall kotor_nested_006774e0(void*, void*, U32, void*, void*); // @20, RET 0xC
}

// Accepted storage owner (window_shared_cells.h); declaration only.
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }

// Exact C++ provider symbols.
namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void*, void*); }
namespace kotor_list { struct Internal; }
namespace kotor_input_hover { void* __fastcall ListRemoveTail005e9ce0(kotor_list::Internal*, void*); }
