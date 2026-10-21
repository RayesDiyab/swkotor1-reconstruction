#pragma once
// lane-w3 tier t2 private declarations for w3_client_t2.cpp (coordinator ruling t2-7): the client Player-list /
// Custom-token message handlers, the GameObjectUpdate 'W' (WorkRemaining) record, the AreaLoaded sender and
// CSWCCreature::SetWayPoints, plus the CSWCPlayer leaves they need. Original: swkotor.exe GOG K1 (read-only).
#include "625a7a9ed5-w3_client_objects.h"

namespace menu_resources { struct Ref; }

extern "C" {
// ------------------------------------------------------------------ existing owners (runtime1115 / 1122 maps)
void __fastcall kotor_fastcall_0041db50(void* tlk, void*, unsigned number, void* value);           // 0041DB50 CTlkTable::SetCustomToken RET 8 (0583)
void* __fastcall kotor_fastcall_005ed6d0(void* app, void*, unsigned player_id) noexcept(false);    // 005ED6D0 GetPlayerByPlayerID RET 4 (0779)
void __fastcall c2da_005e9c10(void* list, void*, void* position);                                   // 005E9C10 CExoLinkedListInternal::Remove RET 4 (0191)
void* __fastcall kotor_m15b_portrait_construct_00678ec0(void* self, void*) noexcept(false);         // 00678EC0 CSWPortrait::CSWPortrait (0820)
void __stdcall vector_constructor_iterator_00401130(void* base, std::uint32_t element_size, std::uint32_t count,
                                                    void* (__fastcall* construct)(void*, void*));   // 00401130 RET 10h (0526)
void* __fastcall vector_default_construct_00409e70(void* vector, void*);                           // 00409E70 Vector::Vector (0526)

// ------------------------------------------------------------------ lane-w3 tier t2 (w3_client_t2.cpp)
unsigned __fastcall recovered_client_custom_token_0064bf30(void* self, void*, unsigned minor) noexcept(false);   // RET 4
unsigned __fastcall recovered_client_player_list_00668640(void* self, void*, unsigned minor) noexcept(false);    // RET 4
std::uint32_t __fastcall recovered_client_handle_update_work_remaining_0064dcf0(void* self, void*);   // RET
std::uint32_t __fastcall recovered_client_send_area_loaded_006778f0(void* self, void*) noexcept(false);   // RET
void __fastcall recovered_client_creature_set_way_points_0060c790(void* creature, void*, std::uint32_t count,
                                                                  std::uint32_t points);   // RET 8
void* __fastcall recovered_client_player_construct_0064b460(void* self, void*, std::uint32_t player_id) noexcept(false);   // RET 4
void __fastcall recovered_client_player_destruct_005f1090(void* self, void*) noexcept(false);   // RET
void __fastcall recovered_client_player_set_primary_pc_portrait_id_005ee5c0(void* self, void*, std::uint32_t id) noexcept(false);   // RET 4
void __fastcall recovered_client_player_set_portrait_005ee480(void* self, void*, kotor_w3::ResRef portrait) noexcept(false);   // RET 10h

// ------------------------------------------------------------------ explicit traps (w3_traps_t2.cpp)
std::int32_t __cdecl recovered_client_creature_stats_check_custom_portrait_00647c10(void* resref, std::int32_t female);   // cdecl static
}
