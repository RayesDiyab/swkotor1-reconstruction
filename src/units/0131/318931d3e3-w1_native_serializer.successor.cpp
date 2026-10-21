extern "C" unsigned __fastcall recovered_module_update_time_0056a5f0(void*,void*,void*,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned); extern "C" unsigned __fastcall recovered_area_weather_0056d1e0(void*,void*,void*,unsigned,unsigned);
extern "C" unsigned __fastcall recovered_start_start_new_module_0056cbf0(void*,void*);
// Lane W1 native binding: maps original callee addresses used by the W1 cores to linked providers,
// to W1 bodies, or to kotor_unreachable_trap(address). x86 only. Exports are fastcall-with-dummy-EDX
// (== thiscall with callee stack cleanup), named by original address.
#include "93d73ae698-w1_ports.hpp"
#include "7dd862643f-creature_ai.hpp"
#include "4a8ca74ca0-leaves.hpp"
#include "5ecaebb99c-perception.hpp"
#include "521c87c368-action_queue.hpp"
#include "ed405eef1f-w1_action_queue_bindings.hpp"
#include "e556d6f3cd-combat_timer.hpp"
#include "fde3237102-effects.hpp"
#include "65dd6aca7d-run_actions.hpp"
#include "8fe93ed262-mine_check.hpp"
#include "18e1c9243a-module_update.hpp"
#include "1568f48cf6-object_state.hpp"
#include "f5b66431ee-creature_event.hpp"
#include "844cc6557a-item_event.hpp"
#include "386b2d6488-object_ai.hpp"
#include "12e99d71d0-trigger_event.hpp"
#include "14b14dc5e6-placeable_event.hpp"
#include "0d8d5e7c97-door_event.hpp"
#include "c51588067b-object_actions.hpp"
#include "112803e9eb-vm_actions_w1.hpp"
#include "2e1ef3b500-server_queries.hpp"
#include "9b6e07d27f-faction.hpp"
#include "214484301a-nearest.hpp"
#include "4085ff001c-unequip.hpp"
#include "0e76439f1a-safe_location.hpp"
#include "e5205c2ce5-effect_handlers.hpp"
#include "8512140f7c-combat_state.hpp"
#include "398f3b8720-combat_relation.hpp"
#include "9f5a6fa2ed-start_module.hpp"
#include <cstring>
#include <cstdint>
#if !defined(_M_IX86)
#error "w1_native.cpp is the x86 native binding"
#endif
// ---- linked providers (1115/1118 composition) --------------------------------------------------------------
extern "C" __declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long original_address, const char* symbol);
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }        // 0053
namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } } // 0068
namespace kotor_window_shell { struct String; }
namespace kotor_list { struct Internal; }
namespace base_ini_reconstruction { void* __fastcall remove_head_005e9c80(kotor_list::Internal*, void*); } // RemoveHead
namespace sound_volume29_required { unsigned __fastcall predicate_006054e0(void*, void*, void*); } // return_true_4
namespace kotor_reconstruction { struct ArrayMutations; struct StartupString { int BorrowedEquals005e5290(const char*); int BorrowedLength005e5790(); }; }
namespace kotor_reconstruction { struct StartupString; }
namespace kotor::server_mainloop {                     // identical to server_mainloop.hpp (1127): portable providers' interface
using Word = unsigned;
struct Memory { virtual ~Memory() = default; virtual Word read(Word) = 0; virtual void write(Word, Word) = 0; virtual Word read16(Word) = 0;
                virtual void write16(Word, Word) = 0; virtual Word read8(Word) = 0; };
Word creature_faction_00513fc0(Memory&, Word);                                       // CSWSCreature::GetFaction (faction_member_helpers)
Word creature_get_class_005a4e90(Memory&, Word, Word);                               // 0982
}
namespace recovered_creature_area_access { using U = unsigned; struct Ports { virtual ~Ports() = default; virtual U read(U) = 0;
  virtual void write(U, U) = 0; virtual U invalid() = 0; }; void set(U, U, Ports&); }      // 1059 CSWSObject::SetArea
namespace kotor_original { void* __fastcall kotor_fastcall_00552c50(void*, void*); }      // 0423 CSWRules::CSWRules
namespace kotor_rules { void __fastcall kotor_fastcall_00551440(void*, void*); }          // 0430 CSWRules::~CSWRules
namespace directory_list { kotor_reconstruction::StartupString* __fastcall LowerCase005e6080(kotor_reconstruction::StartupString*, void*, kotor_reconstruction::StartupString*); }   // 0288
namespace ini_comparison { int __fastcall CompareNC005e58c0(kotor_reconstruction::StartupString*, void*, const char*); }   // 0154 CExoString::CompareNoCase // 0077 CExoString::operator==(const char*)
namespace menu_list_growth { void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*, void*, unsigned); } // CExoArrayList::Allocate
namespace kotor_game_prepare { kotor_window_shell::String* __fastcall kotor_shell_string_construct_005b3190(kotor_window_shell::String*, void*); } // 0417 CExoString()
namespace frame_control { extern unsigned message_manager_007a3a00; extern unsigned resource_manager_007a39e8; extern unsigned sound_manager_007a39ec; }   // 0456 / 0151 / 0673               // 0456
namespace menu_continuation { struct ExoTimers { long long GetHighResolutionTimer_005e9260(); }; } // 0457
extern "C" {
extern void* kotor_cell_rules_007a3a28;                                              // 0421
extern unsigned kotor_rules_cell32_007a21a8;                                         // 0431
extern unsigned kotor_rules_cell32_007a21ac;                                         // 0431
unsigned __fastcall kotor_fastcall_004aea40(void*, void*);                            // GetPlayerCreatureId
void* __fastcall kotor_fastcall_004ae770(void*, void*, unsigned);                     // GetCreatureByGameObjectID
void* __fastcall kotor_fastcall_004ae750(void*, void*, unsigned);                     // GetGameObject
void* __fastcall kotor_fastcall_004ae780(void*, void*, unsigned);                     // GetAreaByGameObjectID (0530)
void __fastcall kotor_m15e_slot090_void0_004015a0(void*, void*);                      // 004015A0 void_return (0832)
unsigned __fastcall recovered_vm_run_script_situation_005d0fd0(void*, void*, void*, unsigned, unsigned);
void __fastcall recovered_vm_delete_script_situation_005d1110(void*, void*, void*);     // CVirtualMachine::DeleteScriptSituation (W6 t2) // CVirtualMachine::RunScriptSituation (W6, lane-w6 tiers/t2)
unsigned __fastcall recovered_cc_message_00568570(void*, void*, unsigned, unsigned, void*, unsigned); // 00568570 SendServerToPlayerCCMessage
void* __fastcall recovered_item_lookup_004ae760(void*, void*, unsigned);
unsigned __fastcall kotor_fastcall_004b1f70(void*, void*, unsigned);                  // CServerExoAppInternal::ClientToServerObjectId (0595)
void __fastcall w1x_object_set_orientation_004cd170(void*, void*, void*);
extern unsigned recovered_health_threshold_007a1b38;                                  // .data 0.2f (0963)
unsigned __fastcall recovered_vm_outer_pop_integer_005d1000(void*, void*, void*);      // W7 t1
unsigned __fastcall recovered_vm_outer_pop_float_005d1020(void*, void*, void*);        // W7 t2
unsigned __fastcall recovered_vm_outer_pop_object_005d10c0(void*, void*, void*);       // 1196
unsigned __fastcall recovered_vm_outer_push_object_005d10d0(void*, void*, unsigned);   // 1196
unsigned __fastcall recovered_vm_outer_push_engine_structure_005d10b0(void*, void*, int, unsigned); // W6 t3
unsigned __fastcall recovered_vm_outer_pop_engine_structure_005d10a0(void*, void*, int, void*);     // W6 t3
void __fastcall w5_object_add_open_door_action_004cfac0(void*, void*, unsigned, unsigned);   // W5 t2
void __fastcall w5_object_add_close_door_action_004cfb20(void*, void*, unsigned, unsigned);  // W5 t2
void __fastcall w5_object_clear_all_actions_004ccd80(void*, void*, unsigned);
void __fastcall w5_combat_clear_all_attacks_004d37e0(void*, void*);                    // W5 CSWSCombatRound::ClearAllAttacks
void __fastcall recovered_object_set_last_hostile_actor_004cc0c0(void*, void*, unsigned, unsigned);   // 1194w CSWSObject::SetLastHostileActor
struct W1Vector { unsigned x, y, z; };                                                 // Vector passed by value (12 bytes)
unsigned __fastcall w2x_area_room_004bb600(void*, void*, void*, unsigned);              // W2 CSWSArea::GetRoom (0886 glue)
unsigned __fastcall w1x_area_test_safe_point_004be5e0(void*, void*, void*, void*);      // 0886 wrapper over 1067
int __stdcall area_no_nonwalk_polys_in_static_object_004bbe20(unsigned, void*, void*, unsigned, unsigned, unsigned, unsigned, int, void*);   // 0526 (ECX ignored)
void* __fastcall collision_mesh_world_to_local_00596a00(void*, void*, void*, const void*);   // 0526..0529
unsigned __fastcall w5_adapt_check_aabb_walkable_00581530(void*, void*, W1Vector, W1Vector, void*);   // W5
double __fastcall w5_room_compute_height_00579430(void*, void*, W1Vector, int);          // W5 CSWSRoom::ComputeHeight (ST0)
void* __fastcall recovered_creature_inventory_004ef770(void*, void*, unsigned);                       // 1086 GetItemRepository
unsigned __fastcall recovered_inventory_insert_0055d330(void*, void*, void**, unsigned, unsigned, unsigned);   // 1088 CItemRepository::AddItem
void __fastcall recovered_creature_armor_004ece80(void*, void*, void*, unsigned, unsigned);            // 1037 ComputeArmourClass
unsigned __fastcall recovered_item_weight_005562a0(void*, void*);                                     // 0886 CSWSItem::GetWeight
unsigned __fastcall recovered_area_insert_0050dfd0(void*, void*, unsigned, unsigned);                 // 1065 CSWSArea::AddObjectToArea
unsigned __fastcall recovered_creature_equip_004feb90(void*, void*, unsigned, void*, unsigned, unsigned, unsigned);   // 1035 EquipItem
unsigned __fastcall recovered_inventory_unequip_message_0056fd10(void*, void*, unsigned, unsigned, unsigned);     // W2 t6b w2_inv_send_native, RET 0Ch
unsigned __fastcall recovered_inventory_unequip_cancel_message_0056fd80(void*, void*, unsigned, unsigned);       // W2 t6b w2_inv_send_native, RET 8
unsigned __fastcall kotor_fastcall_004f2a50(void*, void*, unsigned);                                             // producer_helpers ToggleMode, RET 4
void __cdecl frame_LogFormat_005e5680(void*, const char*, ...);
void __fastcall kotor_fastcall_004cb9e0(void*, void*, unsigned);                                              // CalculateLastSpellProjectileTime, RET 4                                                 // 0125 CExoString::Format
unsigned __fastcall recovered_vm_outer_pop_string_005d1080(void*, void*, void*);       // W7 t5 CVirtualMachine::StackPopString, RET 4
int __fastcall async_string_equal_005e5200(void*, void*, void*);                       // 0188 CExoString::operator==
void* __fastcall kotor_fastcall_004b2820(void*, void*);                                // 0595 CServerExoApp::GetPlayerCreature
unsigned __fastcall recovered_vm_outer_pop_vector_005d1040(void*, void*, void*);                         // W6 t5 CVirtualMachine::StackPopVector, RET 4
unsigned __fastcall recovered_vm_outer_push_vector_005d1050(void*, void*, unsigned, unsigned, unsigned);  // W6 t5 StackPushVector (by value), RET C
int __fastcall recovered_server_faction_of_object_004b40c0(void*, void*, unsigned, unsigned*);   // W7 CServerExoAppInternal::GetFactionOfObject (ruling: W7 owns 004B40C0)              // W5 t2b CSWSObject::ClearAllActions, RET 4
void* __fastcall kotor_fastcall_00635460(void*, void*);                               // CSWParty::GetPlayerCharacter (0767)
void* __fastcall kotor_fastcall_005ed580(void*, void*, unsigned);                     // CClientExoApp::GetGameObject (0701)
void __fastcall assign_string_005e5c50(void*, void*, void*) noexcept(false);         // CExoString::operator= (0310)
void* __fastcall recovered_server_set_move_to_module_pending_004aecc0(void*, void*, unsigned);      // 004AECC0 E1
void* __fastcall recovered_server_set_move_to_module_string_004aecd0(void*, void*, void*) noexcept(false);          // 004AECD0 E1
void* __fastcall recovered_server_set_move_to_module_start_waypoint_004aed30(void*, void*, void*) noexcept(false);  // 004AED30 E1
unsigned __fastcall recovered_item_in_repository_00555f40(void*, void*, void*, unsigned); // CItemRepository::GetItemInRepository (W2 t3)
unsigned __fastcall recovered_remove_item_00555fd0(void*, void*, void*);              // CItemRepository::RemoveItem (W2 t3)
void __fastcall recovered_door_state_00589600(void*, void*, unsigned, unsigned);      // CSWSDoor::SetOpenState (0886)
unsigned __fastcall w5_door_open_door_00589c70(void*, void*, unsigned);              // CSWSDoor::OpenDoor (W5 t2)
void* __fastcall w5_door_get_linked_object_00589580(void*, void*);                     // CSWSDoor::GetLinkedObject (W5 t2)
unsigned __fastcall recovered_stats_effect_query_005a6960(void*, void*, unsigned, unsigned); // CSWSCreatureStats::GetEffectImmunity (1094)
void __fastcall kotor_m15e_slot008_void8_005b5e90(void*, void*, unsigned, unsigned);  // 005B5E90 void_return (8) (0832)
void __fastcall kotor_fastcall_004ae9a0(void*, void*, unsigned, unsigned);            // CServerExoApp::SetPauseState (0453)
void __fastcall kotor_fastcall_0062ac40(void*, void*);                                // CGuiInGame::StopGlobalFade (0457)
void __fastcall kotor_m15_ingame_start_global_fade_0062abf0(void*, void*, unsigned, unsigned, unsigned, void*); // CGuiInGame::StartGlobalFade (0814)              // GetItemByGameObjectID (1025)
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void*, void*, unsigned); // CSWBaseItemArray::GetBaseItem (0820)
void __fastcall recovered_item_owner_00553210(void*, void*, unsigned, unsigned, unsigned, unsigned); // CSWSItem::SetPossessor (1013)
void __fastcall recovered_item_remove_area_005538a0(void*, void*);                    // CSWSItem::RemoveFromArea (1134)
int __fastcall c2da_current_get_int_00413660(void*, void*, int, void*, int*);         // C2DA::GetINTEntry (0376)
unsigned __fastcall exo_array_list_count_004af240(void*, void*, unsigned);            // CExoArrayList::Count(value) (0567)
void* __fastcall recovered_event_string_004d7360(void*, void*, void*, unsigned);      // CSWCCMessageData::GetString (0886)
void __fastcall recovered_event_destroy_004d7590(void*, void*);                       // CScriptEvent::~CScriptEvent (0886)
void __fastcall gui_scene_void_return_0c_005c5630(void*, void*, unsigned, unsigned, unsigned); // void_return_0xc (0464)
int __fastcall frame_LogIsEmpty_005e58a0(void*, void*);                               // CExoString::IsEmpty (0457)
void* __fastcall frame_R4_005ed5b0(void*, void*, unsigned);                           // CClientExoApp::GetCreatureByGameObjectID (0457)
void __fastcall kotor_fastcall_004ae960(void*, void*, unsigned);                      // TogglePauseState (0595)
void* __fastcall kotor_fastcall_004ae9d0(void*, void*);                               // GetActiveExclusionList (0453)
void* __fastcall kotor_fastcall_00561080(void*, void*);                               // CSWSPlayer::GetGameObject (0595)
void __fastcall recovered_event_string_set_004d6ae0(void*, void*, unsigned, void*);   // CSWCCMessageData::SetString (0886)
const char* __fastcall WindowCStr005e5670(void*, void*);                              // CExoString::CStr
double __cdecl vector_magnitude_004ab100(const void*);
float __cdecl kotor_m15a_yaw_004aa0f0(const void*);                                   // Yaw (0816); result read from ST0
void __fastcall vector_normalize_004ab130(void*, void*);                              // Vector::Normalize (0483)
void __fastcall w1x_pointer_list_insert_00616940(void*, void*, unsigned, unsigned);  // CExoArrayList<pointer>::Insert (0886 glue over 1032)
void* __fastcall kotor_fastcall_004aede0(void*, void*);                               // GetWorldTimer
unsigned __fastcall recovered_ai_level_004b08a0(void*, void*, void*, unsigned);       // SetAILevel
unsigned __fastcall kotor_fastcall_004b48e0(void*, void*, unsigned);                  // IsOnActiveExclusionList
void __fastcall frame_GetWorldTime_004ade40(void*, void*, unsigned*, unsigned*);
unsigned __fastcall recovered_world_time_difference_004ae460(void*, void*, unsigned, unsigned, unsigned, unsigned, unsigned*, unsigned*); // G01 donor
void* __fastcall kotor_fastcall_004ae910(void*, void*, unsigned);                     // GetClientObjectByObjectId
void* __fastcall recovered_object_area_004cb120(void*, void*);                        // GetArea
unsigned __fastcall frame_R4_005ed690(void*, void*);                                  // GetInGameGui
unsigned __fastcall kotor_fastcall_005eda50(void*, void*, unsigned);                  // ServerToClientObjectId
void* __fastcall kotor_fastcall_005ed8b0(void*, void*);                               // GetSWParty
void __fastcall recovered_object_position_004cd1a0(void*, void*, void*, unsigned, unsigned, unsigned); // SetPosition
unsigned __fastcall unresolved_list_at_pos_005e9b60(void*, void*, void*);
unsigned __fastcall unresolved_list_next_005e9b90(void*, void*, void**);
unsigned __fastcall recovered_creature_incapacitated_004ef890(void*, void*);          // GetDeadTemp
int __fastcall kotor_m15b_c2da_get_float_entry_00413350(void*, void*, int, int, float*);
void* __fastcall frame_GetCharacter_006346c0(void*, void*, unsigned);
unsigned __fastcall kotor_fastcall_004aea30(void*, void*, unsigned);                  // ClientToServerObjectId
void* __fastcall kotor_fastcall_005ed530(void*, void*);                               // CClientExoApp::GetModule
void* __fastcall kotor_fastcall_004aee00(void*, void*);                               // GetPauseTimer
void __fastcall recovered_stats_refresh_005addc0(void*, void*);                       // UpdateCombatInformation
void __fastcall kotor_fastcall_0048dde0(void*, void*, int);                          // CExoArrayList<Vector>::deleteAt
void __fastcall recovered_event_integer_004d76b0(void*, void*, unsigned, unsigned);   // CSWCCMessageData::SetInteger
void __fastcall recovered_event_object_004d7780(void*, void*, unsigned, unsigned);    // CSWCCMessageData::SetObjectID
void __stdcall c2da_destroy_array_006fa852(void*, unsigned, int, void(__fastcall*)(void*, void*)); // eh vector dtor iterator
void* __cdecl live_operator_new(unsigned);                                           // 006FA7E6 game CRT
void __cdecl live_free(void*);                                                       // 006FA390 game CRT
void __fastcall frame_R4_005d0fc0(void*, void*, unsigned*, unsigned, unsigned);       // CVirtualMachine::RunScript
unsigned __stdcall c2da_resource_constant_word_0063e7f0();                            // return_zero (no stack args)
void __fastcall frame_LogStringCtor_005e5a90(void*, void*, const char*);              // CExoString(const char*)
void __fastcall frame_LogStringDtor_005e5c20(void*, void*);                           // ~CExoString
int __fastcall c2da_current_get_string_00413270(void*, void*, int, void*, void*);     // C2DA::GetCExoStringEntry
int __cdecl crt_rand_006fc447();
unsigned char __fastcall kotor_fastcall_004d8230(void*, void*, unsigned, void**);
void __fastcall c2da_005e9c10(void*, void*, void*);
void* __fastcall resource_append_005e9dc0(void*, void*, void*);                       // CExoLinkedListInternal::AddTail
void* __fastcall c2da_005e9d70(void*, void*, void*);                                  // CExoLinkedListInternal::AddHead
void* __fastcall kotor_fastcall_005ed700(void*, void*);                               // CClientExoApp::GetClientOptions
void __fastcall kotor_fastcall_005eda60(void*, void*, unsigned, unsigned);            // CClientExoApp::SetInputClass
unsigned __fastcall kotor_m15_client_get_player_creature_id_005ed550(void*, void*);   // CClientExoApp::GetPlayerCreatureId
void* __fastcall GuiAssign005e5140(void*, void*, const char*);                        // CExoString::operator=(const char*)                                    // CExoLinkedListInternal::Remove(position)
void __fastcall recovered_effect_integer_005036a0(void*, void*, unsigned, unsigned);   // CGameEffect::SetInteger
void* __fastcall recovered_effect_construct_00503e40(void*, void*, unsigned);          // CGameEffect::CGameEffect
void __fastcall recovered_effect_creator_00503a00(void*, void*, unsigned);             // CGameEffect::SetCreator
void __fastcall recovered_effect_apply_004d14f0(void*, void*, void*, unsigned, unsigned); // CSWSObject::ApplyEffect
unsigned __fastcall recovered_event_time_compare_004adfa0(void*, void*, unsigned, unsigned, unsigned, unsigned); // CompareWorldTimes
unsigned __fastcall frame_GetPauseState_004ae980(void*, void*, unsigned);              // CServerExoApp::GetPauseState(mask)
unsigned __fastcall w1x_creature_skill_rank_005aa570(void*, void*, unsigned, void*, unsigned); // 0886 wrapper (GetSkillRank)
void* __fastcall recovered_object_render_004cc2b0(void*, void*);                      // CSWSObject::GetClientObject
void* __fastcall recovered_equipment_item_005a4c20(void*, void*, unsigned);           // CSWInventory::GetItemInSlot
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void*, void*);                // CSWItem::GetBaseItem
float* __cdecl kotor_cdecl_00423bc0(float*, const float*, const float*);             // Vector::operator- (out = a - b)
void* __cdecl vector_normalized_004ab180(void*, const void*);                          // normalize(out, in)
unsigned __fastcall kotor_m15e_slot128_false4_0063e7c0(void*, void*, unsigned);       // return_false_4
extern unsigned kotor_rules_cell32_007a210c;          // CGameObjectArray::GetGameObject
int __fastcall c2da_current_get_float_00413430(void*, void*, unsigned, void*, float*); // C2DA::GetFLOATEntry(row, column name)
unsigned __fastcall recovered_area_index_00506f20(void*, void*, unsigned*, unsigned);  // CSWSArea::GetFirstObjectIndiceByX
int __fastcall area_clear_line_of_sight_0050c330(void*, void*, void*, void*, void*, unsigned*, unsigned, unsigned, int, int); // CSWSArea::ClearLineOfSight
void* __fastcall recovered_event_construct_004d7540(void*, void*);                    // CScriptEvent::CScriptEvent
void* __fastcall server_get_module_004ae6b0(void*, void*);                            // CServerExoApp::GetModule
unsigned __fastcall recovered_event_enqueue_004b08d0(void*, void*, unsigned, unsigned, unsigned, unsigned, unsigned, void*); // AddEventDeltaTime                                                     // _rand
// Requested from the 0886 owner (w1_0886_exports.inc): extern "C" wrappers over the portable providers whose
// only native glue lives inside server_mainloop_native_vm_canonical_cell.cpp.
void __fastcall w1x_object_set_hp_004cc280(void*, void*, unsigned);
unsigned __fastcall w1x_area_height_bits_004bc380(void*, void*, void*, unsigned, unsigned*);
unsigned __fastcall w1x_area_safe_location_004be860(void*, void*, unsigned, unsigned, unsigned, unsigned, unsigned);
unsigned __fastcall w1x_creature_max_fp_004fd490(void*, void*);
void __fastcall w1x_unique_append_006449d0(void*, void*, unsigned);                  // CExoArrayList<unsigned long>::AddUnique (0886 glue)
const char* __cdecl kotor_cdecl_00401430(unsigned);                                    // 00401430 ClearLogCounters: static-pointer return, argument ignored
void __fastcall kotor_fastcall_005df4e0(void*, void*);                                  // 005DF4E0 CExoInput::ClearEvents
void* __fastcall frame_GetWorldTimer_005ed4f0(void*, void*);                            // 005ED4F0 CClientExoApp::GetWorldTimer (exact selected symbol case)
void __fastcall kotor_fastcall_004adff0(void*, void*);                                  // 004ADFF0 CWorldTimer::PauseWorldTimer
void __fastcall disable_input_005edbe0(void*, void*);                                   // 005EDBE0 -> 0061F7C0: [inner+0x2BC]=1
unsigned __fastcall w1x_server_load_module_004b95b0(void*, void*, void*, unsigned);     // 004B95B0 0886 adapter over 0920 load_module
}
// ---- W1-owned storage cells -------------------------------------------------------------------------------
// BSS debug cells of the original (no .text writer stores a nonzero value; see creature_ai.cpp).
extern "C" unsigned recovered_debug_cell_008327f8 = 0, recovered_debug_cell_00832838 = 0,
    recovered_debug_cell_008327e4 = 0, recovered_debug_cell_008327e8 = 0, recovered_debug_cell_008327ec = 0, recovered_debug_cell_008327f0 = 0,
    recovered_debug_cell_00832800 = 0;
// .data 007A1B28, initial value 1 (written only by CSWSMessage::HandlePlayerToServerCheatMessage).
extern "C" unsigned recovered_stealth_timer_cell_007a1b28 = 1;
extern "C" unsigned recovered_reputation_override_007a2068 = 0xffffffffu;
extern "C" void* __fastcall recovered_rules_server_scalar_destroy_0057a220(void*, void*, unsigned);
extern "C" void* const recovered_vtable_007493e0[1] = {reinterpret_cast<void*>(&recovered_rules_server_scalar_destroy_0057a220)};   // CSWSRules vftable   // .data -1: GetReputation debug override (no TU owns it in 1123)
extern "C" unsigned recovered_debug_cell_008327fc = 0;            // RunActions queue-dump debug flag (no writer)
// .data 007A2288, initial value 1: set by RunActions (party-leader camera restore), read by the client camera code
// (004CF691 StartDialog, 0060B958 ResetCameraToFollow, 00621xxx HandleInputEvent): those owners bind to this cell.
extern "C" unsigned recovered_camera_reset_cell_007a2288 = 1;
#include "10fd141533-w1_action_bodies_decl.inc"
// unready47 (case 47) provider batch: missing combat-round bodies and the
// selected real providers they call.  All are real selected objects; the
// replacements below only route the original addresses to them.
namespace w1 { namespace combat_round {
unsigned end_combat_round_004d4620(unsigned self, unsigned flag, Ports& p);
void clear_attacker_list_005b8010(unsigned self, Ports& p);
void create_new_attack_actions_005b6980(unsigned self, unsigned arg1, unsigned arg2, unsigned arg3, Ports& p);
void cancel_all_combat_modes_005b48b0(unsigned self, unsigned arg, Ports& p);
void update_attack_target_for_all_action_004d3e50(unsigned self, unsigned target, Ports& p);
} }
extern "C" unsigned __fastcall w5_combat_attack_clear_004d3010(void*, void*);
extern "C" void __fastcall w5_creature_set_combat_mode_0050ee80(void*, void*, unsigned, unsigned);
extern "C" unsigned __fastcall kotor_fastcall_005edc10(void*, void*);
extern "C" void __fastcall kotor_fastcall_005edee0(void*, void*, unsigned, unsigned);
extern "C" unsigned __fastcall kotor_fastcall_004f2de0(void*, void*, float, unsigned, unsigned);
extern "C" void __fastcall kotor_fastcall_005edd70(void*, void*, unsigned, unsigned);
extern "C" void __fastcall w5_creature_set_lock_orientation_to_object_004f34a0(void*, void*, unsigned, unsigned);
extern "C" void __fastcall w5_combat_clear_all_special_attacks_004d4f60(void*, void*);
extern "C" unsigned __fastcall kotor_fastcall_004d38b0(void*, void*, unsigned, unsigned, unsigned, unsigned, unsigned);
extern "C" unsigned __fastcall w5_creature_get_use_range_004ee440(void*, void*, unsigned, void*, float*, unsigned);
extern "C" unsigned int __fastcall load_screen_get_next_load_screen_hint_strref_005ee090(void*, void*, void*, unsigned int) noexcept(false);
extern "C" void __fastcall load_screen_set_load_screen_hint_update_timer_005ee040(void*, void*, unsigned int) noexcept(false);
extern "C" void __fastcall kotor_fastcall_0062ec60(void*, void*, unsigned int) noexcept(false);
extern "C" void* __fastcall async_ref_construct_string_00406d60(void*, void*, void*) noexcept(false);
extern "C" void __fastcall kotor_fastcall_004088d0(void*, void*, void*) noexcept(false);
extern "C" void __fastcall kotor_fastcall_005edba0(void*, void*) noexcept(false);
extern "C" void __fastcall frame_R4_004adbd0(void*, void*) noexcept(false);
// ---- route closure (w1-routes): providers for the former "not provided" callees (c13c map, non-trap bodies) ----------
namespace menu_resources {                             // resource-r1 resources.h: CResRef and CExoResMan members (0027/0030/0031)
struct Ref { unsigned char bytes[16]; Ref* __thiscall construct_00406d80(const char*); };
struct Manager { unsigned __thiscall directory_00408800(kotor_reconstruction::StartupString*); unsigned __thiscall exists_00408bc0(Ref*, unsigned, unsigned*); };
}
kotor_reconstruction::StartupString* __fastcall GuiConcat005e5d10(kotor_reconstruction::StartupString*, void*, kotor_reconstruction::StartupString*,
                                                                  const kotor_reconstruction::StartupString*);   // 0093 CExoString::operator+
extern "C" {
struct W1Ref16 { unsigned char bytes[16]; };                                                       // CResRef by value
void* __fastcall c2da_current_construct_00413cc0(void*, void*, W1Ref16, unsigned);                   // 0377 C2DA::C2DA, RET 14h
int __fastcall c2da_current_load_004143b0(void*, void*);                                             // 0380 C2DA::Load2DArray
unsigned __fastcall kotor_fastcall_005b92b0(void*, void*, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned); // SavingThrowRoll
unsigned __fastcall kotor_fastcall_00641dc0(void*, void*);                                           // CSWCModule::RestoreCamera (W4)
unsigned __fastcall kotor_fastcall_004d35c0(void*, void*);                                           // CSWSCombatRound::GetSpellActionPending
void* __fastcall kotor_fastcall_004ae7f0(void*, void*, unsigned);                                    // CServerExoApp::GetEncounterByGameObjectID
void __fastcall kotor_fastcall_004af010(void*, void*, unsigned);                                     // CServerExoApp::SetActionConversation
void __fastcall kotor_fastcall_004af030(void*, void*, unsigned);                                     // CServerExoApp::SetIgnoreConversationStartRange
void __fastcall recovered_attack_data_destruct_004d4480(void*, void*);                               // ~CSWSCombatAttackData
void __fastcall kotor_fastcall_004d6ee0(void*, void*, void*);                                        // CSWCCMessageData::CopyTo
void __fastcall kotor_fastcall_004ec760(void*, void*, void*);                                        // BroadcastSavingThrowData
void __fastcall kotor_fastcall_004edba0(void*, void*, void*);                                        // ForceMoveToPoint
unsigned __fastcall kotor_fastcall_004f1ef0(void*, void*, unsigned, unsigned);                       // CancelAction
void __fastcall kotor_fastcall_004f5f70(void*, void*);                                               // RemoveCombatInvisibilityEffects
void __fastcall kotor_fastcall_00594310(void*, void*, float, unsigned);                              // CSWSEncounter::RemoveFromActiveCreatureCount
unsigned __fastcall area_vfx_queue_announce_005fb490(void*, void*, int);                             // CClientExoAppInternal::PlayRumblePattern
unsigned __fastcall recovered_creature_remove_item_00510ef0(void*, void*, void*, unsigned, unsigned, void*); // CSWSCreature::RemoveItem (W2)
void __fastcall kotor_fastcall_0062eeb0(void*, void*, unsigned char, unsigned);                      // CGuiInGame::UpdateStatus (W4)
void __fastcall kotor_fastcall_0058ee30(void*, void*);                                               // CSWSTrigger::RemoveFromArea
void __fastcall recovered_area_transition_bmp_00560cd0(void*, void*, unsigned, void*);               // CSWSPlayer::SetAreaTransitionBMP (W2)
unsigned __fastcall recovered_placeable_acquire_item_00584b10(void*, void*, void*, unsigned, unsigned); // CSWSPlaceable::AcquireItem (W6)
void __fastcall w5_placeable_open_inventory_00587420(void*, void*, unsigned, unsigned);              // CSWSPlaceable::OpenInventory
void __fastcall kotor_fastcall_005edc30(void*, void*, unsigned, unsigned);                           // CClientExoApp::ChangeCharacter
void __fastcall recovered_client_set_player_creature_005ed780(void*, void*, unsigned) noexcept(false); // CClientExoApp::SetPlayerCreature (W3)
void __fastcall kotor_fastcall_005ed720(void*, void*) noexcept(false);                               // AttachPlayerGameObjectToArea (W4)
unsigned __fastcall kotor_fastcall_006349f0(void*, void*, unsigned);                                 // CSWParty::RemoveCharacter (W4)
void __fastcall kotor_fastcall_00610a10(void*, void*, unsigned);                                     // CSWCCreature::SetCombatMode (W4)
void __fastcall kotor_m15_client_set_load_screen_hint_005edf00(void*, void*, unsigned) noexcept(false); // SetLoadScreenHint (0813)
void __fastcall kotor_fastcall_005f3030(void*, void*, unsigned);                                     // CClientExoAppInternal::SetCanSendDialog (W4)
unsigned __fastcall kotor_fastcall_006332b0(void*, void*);                                           // CGuiInGame::CloseDialog
void __fastcall recovered_client_creature_set_combat_state_006182a0(void*, void*, unsigned) noexcept(false); // CSWCCreature::SetCombatState (W3)
// claude_w1_route_bodies.cpp (srcw5/actions): originals implemented for these routes
void* __fastcall kotor_fastcall_004aefd0(void*, void*, void*);                                       // CServerExoApp::SetLastConversation
void __fastcall kotor_fastcall_004ce9a0(void*, void*, unsigned);                                     // CSWSObject::SetKeepCorpse
void __fastcall kotor_fastcall_004cf9b0(void*, void*, void*);                                        // CSWSObject::AddMatchedExpressionString
void __fastcall kotor_fastcall_004cfa50(void*, void*);                                               // CSWSObject::ClearMatchedExpressionStrings
void __fastcall kotor_fastcall_0064b4f0(void*, void*, void*);                                        // CSWSObject::SetSpellTargetPosition
void __fastcall kotor_fastcall_0055dc10(void*, void*);                                               // CSWSItem::CloseItemForAllPlayers
void __fastcall kotor_fastcall_00587770(void*, void*);                                               // CSWSPlaceable::ClosePlaceableForAllPlayers
void __fastcall kotor_fastcall_00506b30(void*, void*, unsigned, unsigned);                           // CSWSArea::SetTransitionPending
unsigned char __fastcall kotor_fastcall_00506ac0(void*, void*);                                      // CSWSArea::GetNextTransitionPendingID
void __fastcall kotor_fastcall_004d0870(void*, void*, unsigned);                                     // CSWSObject::RemoveSomeEffectsOfDurationType
void __fastcall kotor_fastcall_004ec7a0(void*, void*, void*);                                        // CSWSCreature::BroadcastStealthDetectionData
void __fastcall kotor_fastcall_004ec840(void*, void*, void*);                                        // CSWSCreature::BroadcastMineDetectionData
void __fastcall kotor_fastcall_004d42e0(void*, void*, unsigned, void*);                              // CSWSCombatRound::DEBUGDumpCombatRoundToScreen
unsigned __fastcall kotor_fastcall_0058fec0(void*, void*, unsigned);                                 // CSWArea::RemoveStaticBoundingBox
void __fastcall kotor_fastcall_00584950(void*, void*);                                               // CSWSPlaceable::RemoveFromArea
void __fastcall kotor_fastcall_0058b490(void*, void*);                                               // CSWSDoor::RemoveFromArea
void __fastcall kotor_fastcall_004d4e80(void*, void*, unsigned);                                     // CSWSCombatRound::DecrementPauseTimer
unsigned __fastcall kotor_fastcall_004d45e0(void*, void*, unsigned);                                 // CSWSCombatAttackData::IsSubAttackReady
void __fastcall kotor_fastcall_004d45b0(void*, void*);                                               // ResolveTopSubAttackToAttack
void __fastcall kotor_fastcall_004d32d0(void*, void*);                                               // RemoveTopSubAttack
unsigned __fastcall kotor_fastcall_004f1250(void*, void*, unsigned);                                 // CSWSCreature::ResetCombatAnimations
unsigned __fastcall kotor_fastcall_004ee770(void*, void*, int, void*, unsigned, float);              // CSWSCreature::ApplyPoisonDamage
unsigned __fastcall kotor_fastcall_004f6790(void*, void*, void*, unsigned, unsigned);                // CSWSCreature::ApplyDiseasePayload
void __fastcall kotor_fastcall_004cdb00(void*, void*, unsigned, unsigned, void*, void*, unsigned, unsigned, unsigned, unsigned,
                                        unsigned, unsigned);                                         // CSWSObject::BroadcastSafeProjectile
unsigned __fastcall kotor_fastcall_004ce220(void*, void*);                                           // CSWSObject::SpawnBodyBag
void __fastcall kotor_fastcall_004f0fd0(void*, void*, unsigned);                                     // CSWSCreature::RemoveFromArea
unsigned __fastcall kotor_fastcall_00584ec0(void*, void*, void*);                                    // CSWSPlaceable::AIActionCastSpell
void __fastcall kotor_fastcall_0058d570(void*, void*, unsigned);                                     // CSWSTrigger::OnEnterMine
unsigned __fastcall kotor_fastcall_00635350(void*, void*);                                           // CSWParty::IsPartyTogether
void __fastcall kotor_fastcall_005d5e70(void*, void*) noexcept(false);                               // CExoSound::StopAllOneShots
int __fastcall kotor_fastcall_005edb20(void*, void*) noexcept(false);                                // CClientExoApp::PlayLoadModuleMovies
void __fastcall kotor_fastcall_004b4f50(void*, void*) noexcept(false);                               // ForceRunAreaOnExitScripts
int __fastcall kotor_fastcall_004b2ba0(void*, void*) noexcept(false);                                // StorePlayerCharacters
void __fastcall kotor_fastcall_004b2380(void*, void*, void*, void*) noexcept(false);                 // DoModuleEnterSaveCleanup
int __fastcall kotor_fastcall_004b5050(void*, void*, void*, void*) noexcept(false);                  // AutoSaveOnEnteringModule
unsigned __fastcall kotor_fastcall_004ba820(void*, void*, void*) noexcept(false);                    // ExecuteLoadModule
void __fastcall kotor_fastcall_005b8050(void*, void*, unsigned, unsigned) noexcept(false);           // CSWSCreature::SignalDamage (claude_w1_chains_damage)
void __fastcall kotor_fastcall_004b8300(void*, void*, void*, void*) noexcept(false);               // DoPCAutosave(name, table) (claude_w1_chains_autosave)
unsigned __fastcall kotor_fastcall_004b2e70(void*, void*) noexcept(false);                          // StoreCurrentModule (claude_save_module)
}
namespace {
using w1::U;
inline void* ptr(U a) { return reinterpret_cast<void*>(a); }
inline U word(const void* p) { return reinterpret_cast<U>(p); }
__declspec(noreturn) void trap(U address, const char* what) { kotor_unreachable_trap(address, what); }
U* cell(U a) {
  switch (a) {
  case 0x8327f8: return &recovered_debug_cell_008327f8;
  case 0x832838: return &recovered_debug_cell_00832838;
  case 0x8327e4: return &recovered_debug_cell_008327e4;
  case 0x7a1b38: return &recovered_health_threshold_007a1b38;
  case 0x7a2068: return &recovered_reputation_override_007a2068;
  case 0x8327e8: return &recovered_debug_cell_008327e8;
  case 0x8327ec: return &recovered_debug_cell_008327ec;
  case 0x8327f0: return &recovered_debug_cell_008327f0;
  case 0x8327fc: return &recovered_debug_cell_008327fc;
  case 0x7a2288: return &recovered_camera_reset_cell_007a2288;
  case 0x832800: return &recovered_debug_cell_00832800;
  case 0x7a1b28: return &recovered_stealth_timer_cell_007a1b28;
  default: return nullptr;
  }
}
// .rdata string literals passed by address.
const char* literal(U a) {
  switch (a) {
  case 0x746bb8: return "SCRIPTNAME";
  case 0x746f28: return "PrimaryRange";
  case 0x746f38: return "SecondaryRange";
  case 0x7471ec: return "FadeDelayOnDeath";
  case 0x73df6c: return "default";
  case 0x7471d8: return "k_hen_dialogue01";
  case 0x7499c4: return "k_trg_transfail";
  case 0x7499b0: return "NW_G0_Transition";
  case 0x747c2c: return "MODELTYPE";
  case 0x747670: return "Walk";
  case 0x73ee04: return "NULL";
  case 0x747c28: return "B";
  case 0x73ece0: return "F";
  case 0x73d71c: return "";
  case 0x73d5d8: return " ";
  case 0x73d90c: return "MODULES:";             // StartNewModule resource alias (read from the exe)
  case 0x745128: return "modulesave";           // StartNewModule C2DA resref (read from the exe)
  default: trap(a, "W1 unmapped .rdata literal");
  }
}
// Original-image data cells the cores (and the portable providers W1 calls directly) read by constant address; each maps
// to the reconstruction's symbol, as 0886's NativeMemory does. Anything else is a data-derived pointer.
bool cell_value(U a, U& v) {
  switch (a) {
  case 0x7a39fc: v = word(kotor_menu_window_cells::manager_007a39fc); return true;
  case 0x7a39e0: v = word(kotor_menu_10_19::dependencies::base_007a39e0); return true;
  case 0x7a3a00: v = frame_control::message_manager_007a3a00; return true;
  case 0x7a39e8: v = frame_control::resource_manager_007a39e8; return true;   // CExoResMan* (0151)
  case 0x7a39ec: v = frame_control::sound_manager_007a39ec; return true;      // CExoSound* (0673)
  case 0x7a3a28: v = word(kotor_cell_rules_007a3a28); return true;
  case 0x7a21a8: v = kotor_rules_cell32_007a21a8; return true;
  case 0x7a21ac: v = kotor_rules_cell32_007a21ac; return true;
  case 0x7a210c: v = kotor_rules_cell32_007a210c; return true;
  default: if (U* c = cell(a)) { v = *c; return true; } return false;
  }
}
struct W1Memory final : kotor::server_mainloop::Memory {
  U read(U a) override { U v; if (cell_value(a, v)) return v; std::memcpy(&v, ptr(a), 4); return v; }
  void write(U a, U v) override { std::memcpy(ptr(a), &v, 4); }
  U read16(U a) override { std::uint16_t v; std::memcpy(&v, ptr(a), 2); return v; }
  void write16(U a, U v) override { std::uint16_t x = static_cast<std::uint16_t>(v); std::memcpy(ptr(a), &x, 2); }
  U read8(U a) override { std::uint8_t v; std::memcpy(&v, ptr(a), 1); return v; }
};
struct W1AreaAccess final : recovered_creature_area_access::Ports {
  W1Memory m;
  unsigned read(unsigned a) override { return m.read(a); }
  void write(unsigned a, unsigned v) override { m.write(a, v); }
  unsigned invalid() override { return 0x7f000000u; }             // [00746644]
};
struct NativePorts final : w1::Ports {
  // Data-derived pointers: plain dereference (heap objects, the executable's own vtables and statics).
  U read(U a) override { U v; std::memcpy(&v, ptr(a), 4); return v; }
  U read16(U a) override { std::uint16_t v; std::memcpy(&v, ptr(a), 2); return v; }
  U read8(U a) override { std::uint8_t v; std::memcpy(&v, ptr(a), 1); return v; }
  void write(U a, U v) override { std::memcpy(ptr(a), &v, 4); }
  void write16(U a, U v) override { std::uint16_t x = static_cast<std::uint16_t>(v); std::memcpy(ptr(a), &x, 2); }
  void write8(U a, U v) override { std::uint8_t x = static_cast<std::uint8_t>(v); std::memcpy(ptr(a), &x, 1); }
  // Constant original-global addresses used by the cores; anything else is an unmapped constant -> trap.
  U global(U a) override {
    if (a == 0x7493e0) return word(recovered_vtable_007493e0);   // CSWSRules vftable ADDRESS (W1-reconstructed table)
    U v;
    if (cell_value(a, v)) return v;
    trap(a, "W1 read of unmapped original global");
  }
  void set_global(U a, U v) override {
    if (U* c = cell(a)) { *c = v; return; }
    trap(a, "W1 write of unmapped/read-only original global");
  }
  U vcall(U self, U slot, std::initializer_list<U> args) override {
    U fn = read(read(self) + slot);
    const U* a = args.begin();
    switch (args.size()) {
    case 0: { using F = U(__thiscall*)(void*); return reinterpret_cast<F>(fn)(ptr(self)); }
    case 1: { using F = U(__thiscall*)(void*, U); return reinterpret_cast<F>(fn)(ptr(self), a[0]); }
    case 2: { using F = U(__thiscall*)(void*, U, U); return reinterpret_cast<F>(fn)(ptr(self), a[0], a[1]); }
    case 3: { using F = U(__thiscall*)(void*, U, U, U); return reinterpret_cast<F>(fn)(ptr(self), a[0], a[1], a[2]); }
    case 4: { using F = U(__thiscall*)(void*, U, U, U, U); return reinterpret_cast<F>(fn)(ptr(self), a[0], a[1], a[2], a[3]); }
    case 5: { using F = U(__thiscall*)(void*, U, U, U, U, U); return reinterpret_cast<F>(fn)(ptr(self), a[0], a[1], a[2], a[3], a[4]); }
    case 10: { using F = U(__thiscall*)(void*, U, U, U, U, U, U, U, U, U, U);   // CSWSArea vtable +0x18 (0050C1E0)
      return reinterpret_cast<F>(fn)(ptr(self), a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]); }
    default: trap(slot, "W1 vcall arity");
    }
  }
  std::uint64_t call64(U target, U self, std::initializer_list<U>) override {
    if (target == 0x5e9260)
      return static_cast<std::uint64_t>(static_cast<menu_continuation::ExoTimers*>(ptr(self))->GetHighResolutionTimer_005e9260());
    trap(target, "W1 call64 callee not provided");
  }
  U call(U t, U s, std::initializer_list<U> args) override;
  void call_st0(U t, U s0_self, std::initializer_list<U> args, U out) override {
    if (t == 0x579430) {                                                 // thiscall(room, Vector by value, int), RET 10h, ST0
      const U* a = args.begin();
      st0_room_height_00579430(ptr(s0_self), a[0], a[1], a[2], a[3], static_cast<unsigned char*>(ptr(out)));
      return;
    }
    void* fn = t == 0x4ab100 ? reinterpret_cast<void*>(&vector_magnitude_004ab100)
             : t == 0x4aa0f0 ? reinterpret_cast<void*>(&kotor_m15a_yaw_004aa0f0) : nullptr;
    if (!fn) trap(t, "W1 call_st0 callee not provided");
    st0_cdecl1(fn, args.begin()[0], static_cast<unsigned char*>(ptr(out)));
  }
  // closeC: the inline-asm CALLs live in __declspec(noinline) helpers (clang MS asm does not mark caller-saved
  // registers clobbered by a CALL; a helper boundary makes EAX/ECX/EDX dead across it).
  __declspec(noinline) static void st0_room_height_00579430(void* room, U x, U y, U z, U w, unsigned char* q5) {
    void* f5 = reinterpret_cast<void*>(&w5_room_compute_height_00579430);
    __asm {
      push w
      push z
      push y
      push x
      mov ecx, room
      xor edx, edx
      call f5
      mov eax, q5
      fstp tbyte ptr [eax]
    }
  }
  __declspec(noinline) static void st0_cdecl1(void* fn, U v, unsigned char* q) {
    __asm {
      push v
      call fn
      add esp, 4
      mov eax, q
      fstp tbyte ptr [eax]
    }
  }
};
U NativePorts::call(U t, U s, std::initializer_list<U> args) {
  const U* a = args.begin();
  switch (t) {
  // ---- existing linked providers
  case 0x4aea40: return kotor_fastcall_004aea40(ptr(s), nullptr);
  case 0x4ae750: return word(kotor_fastcall_004ae750(ptr(s), nullptr, a[0]));          // CServerExoApp::GetGameObject
  case 0x4d69f0: return w1::leaves::message_data_construct_004d69f0(s, *this);
  case 0x4d6a40: w1::leaves::message_data_destroy_004d6a40(s, *this); return 0;
  case 0x4ae770: return word(kotor_fastcall_004ae770(ptr(s), nullptr, a[0]));
  case 0x5e5670: return word(WindowCStr005e5670(ptr(s), nullptr));
  case 0x4ab100: (void)vector_magnitude_004ab100(ptr(a[0])); return 0;
  case 0x4aede0: return word(kotor_fastcall_004aede0(ptr(s), nullptr));
  case 0x4b08a0: return recovered_ai_level_004b08a0(ptr(s), nullptr, ptr(a[0]), a[1]);
  case 0x4b48e0: return kotor_fastcall_004b48e0(ptr(s), nullptr, a[0]);
  case 0x4ade40: frame_GetWorldTime_004ade40(ptr(s), nullptr, static_cast<unsigned*>(ptr(a[0])), static_cast<unsigned*>(ptr(a[1]))); return 0;
  case 0x4ae460: return recovered_world_time_difference_004ae460(ptr(s), nullptr, a[0], a[1], a[2], a[3], static_cast<unsigned*>(ptr(a[4])), static_cast<unsigned*>(ptr(a[5])));
  case 0x4ae910: return word(kotor_fastcall_004ae910(ptr(s), nullptr, a[0]));
  case 0x4cb120: return word(recovered_object_area_004cb120(ptr(s), nullptr));
  case 0x5ed690: return frame_R4_005ed690(ptr(s), nullptr);
  case 0x5eda50: return kotor_fastcall_005eda50(ptr(s), nullptr, a[0]);
  case 0x5ed8b0: return word(kotor_fastcall_005ed8b0(ptr(s), nullptr));
  case 0x4cd1a0: recovered_object_position_004cd1a0(ptr(s), nullptr, ptr(a[0]), a[1], a[2], a[3]); return 0;
  case 0x5e9b60: return unresolved_list_at_pos_005e9b60(ptr(s), nullptr, ptr(a[0]));
  case 0x5e9b90: return unresolved_list_next_005e9b90(ptr(s), nullptr, static_cast<void**>(ptr(a[0])));
  case 0x4ef890: return recovered_creature_incapacitated_004ef890(ptr(s), nullptr);
  case 0x413350: return static_cast<U>(kotor_m15b_c2da_get_float_entry_00413350(ptr(s), nullptr, static_cast<int>(a[0]), static_cast<int>(a[1]), static_cast<float*>(ptr(a[2]))));
  case 0x6346c0: return word(frame_GetCharacter_006346c0(ptr(s), nullptr, a[0]));
  case 0x4aea30: return kotor_fastcall_004aea30(ptr(s), nullptr, a[0]);
  case 0x5ed530: return word(kotor_fastcall_005ed530(ptr(s), nullptr));
  case 0x4aee00: return word(kotor_fastcall_004aee00(ptr(s), nullptr));
  case 0x5addc0: recovered_stats_refresh_005addc0(ptr(s), nullptr); return 0;
  case 0x5d0fc0: frame_R4_005d0fc0(ptr(s), nullptr, static_cast<unsigned*>(ptr(a[0])), a[1], a[2]); return 0; // result unused by W1 callers
  case 0x63e7f0: return c2da_resource_constant_word_0063e7f0();
  case 0x5b3190: return word(kotor_game_prepare::kotor_shell_string_construct_005b3190(static_cast<kotor_window_shell::String*>(ptr(s)), nullptr));
  case 0x5e5a90: frame_LogStringCtor_005e5a90(ptr(s), nullptr, literal(a[0])); return s;
  case 0x5e5c20: frame_LogStringDtor_005e5c20(ptr(s), nullptr); return 0;
  case 0x413270: return static_cast<U>(c2da_current_get_string_00413270(ptr(s), nullptr, static_cast<int>(a[0]), ptr(a[1]), ptr(a[2])));
  case 0x6fc447: return static_cast<U>(crt_rand_006fc447());
  case 0x413430: return static_cast<U>(c2da_current_get_float_00413430(ptr(s), nullptr, a[0], ptr(a[1]), static_cast<float*>(ptr(a[2]))));
  case 0x506f20: return recovered_area_index_00506f20(ptr(s), nullptr, static_cast<unsigned*>(ptr(a[0])), a[1]);
  case 0x50c330: return static_cast<U>(area_clear_line_of_sight_0050c330(ptr(s), nullptr, ptr(a[0]), ptr(a[1]), ptr(a[2]), static_cast<unsigned*>(ptr(a[3])), a[4], a[5], static_cast<int>(a[6]), static_cast<int>(a[7])));
  case 0x4d7540: return word(recovered_event_construct_004d7540(ptr(s), nullptr));
  case 0x4ae6b0: return word(server_get_module_004ae6b0(ptr(s), nullptr));
  case 0x4b08d0: return recovered_event_enqueue_004b08d0(ptr(s), nullptr, a[0], a[1], a[2], a[3], a[4], ptr(a[5]));
  case 0x5e03c0: menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(ptr(s)), nullptr, a[0]); return 0;
  case 0x4ae830: return w1::creature_ai::active_timer_004ae830(s, a[0], *this);
  case 0x4fb4b0: { alignas(8) unsigned char frame[0xb8]; return w1::perception::listen_detection_004fb4b0(s, a[0], a[1], word(frame), *this); }
  case 0x4f1fd0: { alignas(8) unsigned char frame[0x8c]; return w1::perception::spot_detection_004f1fd0(s, a[0], a[1], word(frame), *this); }
  case 0x5aa570: return w1x_creature_skill_rank_005aa570(ptr(s), nullptr, a[0], ptr(a[1]), a[2]);
  case 0x4cc2b0: return word(recovered_object_render_004cc2b0(ptr(s), nullptr));
  case 0x5a4c20: return word(recovered_equipment_item_005a4c20(ptr(s), nullptr, a[0]));
  case 0x5b4790: return word(kotor_m15b_item_get_base_item_005b4790(ptr(s), nullptr));
  case 0x423bc0: return word(kotor_cdecl_00423bc0(static_cast<float*>(ptr(a[0])), static_cast<const float*>(ptr(a[1])), static_cast<const float*>(ptr(a[2]))));
  case 0x4ab180: return word(vector_normalized_004ab180(ptr(a[0]), ptr(a[1])));
  case 0x6054e0: return sound_volume29_required::predicate_006054e0(ptr(s), nullptr, ptr(a[0]));
  case 0x63e7c0: return kotor_m15e_slot128_false4_0063e7c0(ptr(s), nullptr, a[0]);
  case 0x4ee440: return w5_creature_get_use_range_004ee440(ptr(s), nullptr, a[0], ptr(a[1]), static_cast<float*>(ptr(a[2])), a[3]);
  // ---- unready47 selected combat-round providers
  case 0x4d3010: w5_combat_attack_clear_004d3010(ptr(s), nullptr); return 0;
  case 0x50ee80: w5_creature_set_combat_mode_0050ee80(ptr(s), nullptr, a[0], a[1]); return 0;
  case 0x5edc10: return kotor_fastcall_005edc10(ptr(s), nullptr);
  case 0x5edee0: kotor_fastcall_005edee0(ptr(s), nullptr, a[0], a[1]); return 0;
  case 0x5edd70: kotor_fastcall_005edd70(ptr(s), nullptr, a[0], a[1]); return 0;
  case 0x4f2de0: { float f; std::memcpy(&f, &a[0], 4); return kotor_fastcall_004f2de0(ptr(s), nullptr, f, a[1], a[2]); }
  case 0x4f34a0: w5_creature_set_lock_orientation_to_object_004f34a0(ptr(s), nullptr, a[0], a[1]); return 0;
  case 0x4d4f60: w5_combat_clear_all_special_attacks_004d4f60(ptr(s), nullptr); return 0;
  case 0x4d38b0: return kotor_fastcall_004d38b0(ptr(s), nullptr, a[0], a[1], a[2], a[3], a[4]);
  case 0x4ec7a0: kotor_fastcall_004ec7a0(ptr(s), nullptr, ptr(a[0])); return 0;   // claude_w1_route_bodies BroadcastStealthDetectionData, RET 4
  case 0x501950: { alignas(4) unsigned char scratch[4]; return w1::perception::invisible_00501950(s, a[0], word(scratch), *this); }
  case 0x503690: return w1::perception::effect_integer_00503690(s, a[0], *this);
  case 0x4d8230: return kotor_fastcall_004d8230(ptr(s), nullptr, a[0], static_cast<void**>(ptr(a[1])));
  case 0x5b92b0: return kotor_fastcall_005b92b0(ptr(s), nullptr, a[0], a[1], a[2], a[3], a[4], a[5], a[6]);   // claude_combat_attack_rolls SavingThrowRoll, RET 1Ch (AL)
  case 0x4cc0c0: recovered_object_set_last_hostile_actor_004cc0c0(ptr(s), nullptr, a[0], a[1]); return 0;   // 1194w
  case 0x502ac0: { alignas(8) unsigned char frame[0x80]; w1::perception::do_perception_00502ac0(s, a[0], word(frame), *this); return 0; }
  case 0x6fa7e6: return word(live_operator_new(a[0]));
  case 0x6fa390: live_free(ptr(a[0])); return 0;
  case 0x48dde0: kotor_fastcall_0048dde0(ptr(s), nullptr, static_cast<int>(a[0])); return 0;
  case 0x4d76b0: recovered_event_integer_004d76b0(ptr(s), nullptr, a[0], a[1]); return 0;
  case 0x4d7780: recovered_event_object_004d7780(ptr(s), nullptr, a[0], a[1]); return 0;
  case 0x6fa852:
    if (a[3] != 0x5e5c20) trap(a[3], "W1 vector destructor element dtor");
    c2da_destroy_array_006fa852(ptr(a[0]), a[1], static_cast<int>(a[2]), &frame_LogStringDtor_005e5c20); return 0;
  case 0x568570: return recovered_cc_message_00568570(ptr(s), nullptr, a[0], a[1], ptr(a[2]), a[3]); // 00568570
  // ---- 0886-owned portable providers through requested extern "C" wrappers
  case 0x4cc280: w1x_object_set_hp_004cc280(ptr(s), nullptr, a[0]); return 0;
  case 0x4bc380: return w1x_area_height_bits_004bc380(ptr(s), nullptr, ptr(a[0]), a[1], static_cast<unsigned*>(ptr(a[2])));
  case 0x4be860: return w1x_area_safe_location_004be860(ptr(s), nullptr, a[0], a[1], a[2], a[3], a[4]);
  case 0x4fd490: return w1x_creature_max_fp_004fd490(ptr(s), nullptr);
  // ---- W1 bodies
  case 0x6349c0: return w1::creature_ai::party_index_006349c0(s, a[0], *this);
  // ---- not yet provided: explicit trap with the original callee address
  case 0x4f61d0: { alignas(4) unsigned char scratch[12]; w1::action_queue::clear_hostile_004f61d0(s, a[0], word(scratch), *this); return 0; }
  case 0x5e9c10: c2da_005e9c10(ptr(s), nullptr, ptr(a[0])); return 0;
  case 0x5d1110: recovered_vm_delete_script_situation_005d1110(ptr(s), nullptr, ptr(a[0])); return 0;   // W6-owned
  case 0x4faf20: { alignas(4) unsigned char s2[0x10]; w1::combat_timer::update_combat_round_timer_004faf20(s, word(s2), *this); return 0; }
  case 0x4ae980: return frame_GetPauseState_004ae980(ptr(s), nullptr, a[0]);
  case 0x4d4620: return w1::combat_round::end_combat_round_004d4620(s, a[0], *this);
  case 0x5b8010: w1::combat_round::clear_attacker_list_005b8010(s, *this); return 0;
  case 0x5b6980: w1::combat_round::create_new_attack_actions_005b6980(s, a[0], a[1], a[2], *this); return 0;
  case 0x5b48b0: w1::combat_round::cancel_all_combat_modes_005b48b0(s, a[0], *this); return 0;
  case 0x4d3e50: w1::combat_round::update_attack_target_for_all_action_004d3e50(s, a[0], *this); return 0;
  case 0x4d4e80: kotor_fastcall_004d4e80(ptr(s), nullptr, a[0]); return 0;   // claude_w1_route_bodies DecrementPauseTimer, RET 4
  case 0x4d45e0: return kotor_fastcall_004d45e0(ptr(s), nullptr, a[0]);   // claude_w1_route_bodies IsSubAttackReady, RET 4
  case 0x4d45b0: kotor_fastcall_004d45b0(ptr(s), nullptr); return 0;   // claude_w1_route_bodies ResolveTopSubAttackToAttack, RET 0
  case 0x5b8050: kotor_fastcall_005b8050(ptr(s), nullptr, a[0], a[1]); return 0;   // claude_w1_chains_damage SignalDamage(ranged, target), RET 8
  case 0x4d32d0: kotor_fastcall_004d32d0(ptr(s), nullptr); return 0;   // claude_w1_route_bodies RemoveTopSubAttack, RET 0
  case 0x4f1250: return kotor_fastcall_004f1250(ptr(s), nullptr, a[0]);   // claude_w1_route_bodies ResetCombatAnimations, RET 4
  case 0x5e5680:
    if (s == 0 && a[1] == 0x747000) {   // SetCombatState 004F282C: Format(&text, [00747000], 9 values); text unused
      frame_LogFormat_005e5680(ptr(a[0]), "WARNING:  %x %s %d %d %d %d %x %s %x", a[2], static_cast<const char*>(ptr(a[3])),
                               a[4], a[5], a[6], a[7], a[8], static_cast<const char*>(ptr(a[9])), a[10]);
      return 0;
    }
    if (s != 0 && args.size() == 3 && a[0] == 0x749430) {   // RunActions queue dump 0057F5xx: Format(&text, [00749430] " %d(%d)", id, group)
      frame_LogFormat_005e5680(ptr(s), " %d(%d)", a[1], a[2]);
      return 0;
    }
    if (s != 0 && args.size() == 2 && a[0] == 0x7464e0) {   // IncrementTimer 004D4D2x: Format(&text, [007464E0], CStr(name)); text unused
      frame_LogFormat_005e5680(ptr(s), "CSWSCombatRound::IncrementTimer - %s Master cannot be found and round has expired; Resetting",
                               static_cast<const char*>(ptr(a[1])));
      return 0;
    }
    if (s != 0 && args.size() == 5 && a[0] == 0x746478) {   // IncrementTimer 004D4D5x: Format(&text, [00746478], CStr(name), id, timer, end)
      frame_LogFormat_005e5680(ptr(s), "CSWSCombatRound::IncrementTimer - %s Master IS found (%x) and round has expired (%d %d); Resetting",
                               static_cast<const char*>(ptr(a[1])), a[2], a[3], a[4]);
      return 0;
    }
    if (s != 0 && args.size() == 3 && a[0] == 0x746418) {   // IncrementTimer 004D4DEx: Format(&text, [00746418], CStr(name), timer)
      frame_LogFormat_005e5680(ptr(s), "CSWSCombatRound::IncrementTimer - %s Timer is negative at %d; Ending combat round and resetting",
                               static_cast<const char*>(ptr(a[1])), a[2]);
      return 0;
    }
    trap(t, "CExoString::Format with an unmapped format literal");
  case 0x4d1730: { alignas(8) unsigned char f2[0x54]; w1::effects::update_effect_list_004d1730(s, a[0], a[1], word(f2), *this); return 0; }
  case 0x4d06c0: return w1::effects::remove_effect_by_id_004d06c0(s, a[0], a[1], *this);
  case 0x5036a0: recovered_effect_integer_005036a0(ptr(s), nullptr, a[0], a[1]); return 0;
  case 0x503e40: return word(recovered_effect_construct_00503e40(ptr(s), nullptr, a[0]));
  case 0x503a00: recovered_effect_creator_00503a00(ptr(s), nullptr, a[0]); return 0;
  case 0x4d14f0: recovered_effect_apply_004d14f0(ptr(s), nullptr, ptr(a[0]), a[1], a[2]); return 0;
  case 0x4adfa0: return recovered_event_time_compare_004adfa0(ptr(s), nullptr, a[0], a[1], a[2], a[3]);
  case 0x4ee770: { float f; std::memcpy(&f, &a[3], 4); return kotor_fastcall_004ee770(ptr(s), nullptr, static_cast<int>(a[0]), ptr(a[1]), a[2], f); }   // claude_w1_route_bodies ApplyPoisonDamage(row, effect, a7, float), RET 10h
  case 0x4f6790: return kotor_fastcall_004f6790(ptr(s), nullptr, ptr(a[0]), a[1], a[2]);   // claude_w1_route_bodies ApplyDiseasePayload(effect, day, ms), RET 0Ch
  case 0x57f4a0: { alignas(8) unsigned char f2[0x40]; return w1::run_actions::run_actions_0057f4a0(s, a[0], a[1], a[2], a[3], word(f2), *this); }
  case 0x5e9c80: return word(base_ini_reconstruction::remove_head_005e9c80(static_cast<kotor_list::Internal*>(ptr(s)), nullptr));
  case 0x5e9dc0: (void)resource_append_005e9dc0(ptr(s), nullptr, ptr(a[0])); return 0;
  case 0x5e9d70: (void)c2da_005e9d70(ptr(s), nullptr, ptr(a[0])); return 0;
  case 0x5ed700: return word(kotor_fastcall_005ed700(ptr(s), nullptr));
  case 0x5eda60: kotor_fastcall_005eda60(ptr(s), nullptr, a[0], a[1]); return 0;
  case 0x5ed550: return kotor_m15_client_get_player_creature_id_005ed550(ptr(s), nullptr);
  case 0x5e5140: (void)GuiAssign005e5140(ptr(s), nullptr, literal(a[0])); return 0;
  case 0x641dc0: return kotor_fastcall_00641dc0(ptr(s), nullptr);   // W4 combat_restore CSWCModule::RestoreCamera, RET 0
  case 0x4d35c0: return kotor_fastcall_004d35c0(ptr(s), nullptr);   // claude_w5_creature_actions GetSpellActionPending, RET 0
  case 0x584ec0: return kotor_fastcall_00584ec0(ptr(s), nullptr, ptr(a[0]));   // claude_w1_route_bodies CSWSPlaceable::AIActionCastSpell(node), RET 4
  case 0x4d42e0: kotor_fastcall_004d42e0(ptr(s), nullptr, a[0], ptr(a[1])); return 0;   // claude_w1_route_bodies DEBUGDumpCombatRoundToScreen, RET 8
  case 0x4ce6e0: { alignas(8) unsigned char f2[4]; w1::module_update::report_overflow_004ce6e0(s, a[0], a[1], a[2], word(f2), *this); return 0; }
  case 0x4ae7f0: return word(kotor_fastcall_004ae7f0(ptr(s), nullptr, a[0]));   // claude_w5_creature_actions (-> 004B1A10), RET 4
  case 0x4aefd0: (void)kotor_fastcall_004aefd0(ptr(s), nullptr, ptr(a[0])); return 0;   // claude_w1_route_bodies SetLastConversation, RET 4
  case 0x4af010: kotor_fastcall_004af010(ptr(s), nullptr, a[0]); return 0;   // claude_dialogue_start SetActionConversation, RET 4
  case 0x4af030: kotor_fastcall_004af030(ptr(s), nullptr, a[0]); return 0;   // dialogue_state SetIgnoreConversationStartRange, RET 4
  case 0x4cb9e0: kotor_fastcall_004cb9e0(ptr(s), nullptr, a[0]); return 0;   // claude_combat_spell (RET 4, void)
  case 0x4cdb00: kotor_fastcall_004cdb00(ptr(s), nullptr, a[0], a[1], ptr(a[2]), ptr(a[3]), a[4], a[5], a[6], a[7], a[8], a[9]); return 0;   // claude_w1_route_bodies BroadcastSafeProjectile, RET 28h
  case 0x4ce220: return kotor_fastcall_004ce220(ptr(s), nullptr);   // claude_w1_route_bodies SpawnBodyBag (body-bag id), RET 0
  case 0x4ce8a0: { alignas(4) unsigned char f2[4]; w1::object_actions::set_fade_out_time_004ce8a0(s, a[0], word(f2), *this); return 0; }
  case 0x4ce9a0: kotor_fastcall_004ce9a0(ptr(s), nullptr, a[0]); return 0;   // claude_w1_route_bodies SetKeepCorpse, RET 4
  case 0x4cf9b0: kotor_fastcall_004cf9b0(ptr(s), nullptr, ptr(a[0])); return 0;   // claude_w1_route_bodies AddMatchedExpressionString, RET 4
  case 0x4cfa50: kotor_fastcall_004cfa50(ptr(s), nullptr); return 0;   // claude_w1_route_bodies ClearMatchedExpressionStrings, RET 0
  case 0x4d4480: recovered_attack_data_destruct_004d4480(ptr(s), nullptr); return 0;   // creature_destructor_r2 ~CSWSCombatAttackData, RET 0
  case 0x4d6ee0: kotor_fastcall_004d6ee0(ptr(s), nullptr, ptr(a[0])); return 0;   // claude_combat_attack_round CopyTo, RET 4
  case 0x4ec760: kotor_fastcall_004ec760(ptr(s), nullptr, ptr(a[0])); return 0;   // claude_combat_attack_rolls BroadcastSavingThrowData, RET 4
  case 0x4edba0: kotor_fastcall_004edba0(ptr(s), nullptr, ptr(a[0])); return 0;   // claude_w5_pathfind_walk ForceMoveToPoint, RET 4
  case 0x4f0fd0: kotor_fastcall_004f0fd0(ptr(s), nullptr, a[0]); return 0;   // claude_w1_route_bodies CSWSCreature::RemoveFromArea, RET 4
  case 0x4f1ef0: return kotor_fastcall_004f1ef0(ptr(s), nullptr, a[0], a[1]);   // claude_w5_movement_actions CancelAction, RET 8
  case 0x4f3580: kotor_fastcall_004f3580(ptr(s), nullptr, a[0]); return 0;
  case 0x4f5f70: kotor_fastcall_004f5f70(ptr(s), nullptr); return 0;   // claude_combat_spell_mine RemoveCombatInvisibilityEffects, RET 0
  case 0x4fbbe0: kotor_fastcall_004fbbe0(ptr(s), nullptr, a[0]); return 0;
  case 0x594310: { float f; std::memcpy(&f, &a[0], 4); kotor_fastcall_00594310(ptr(s), nullptr, f, a[1]); return 0; }   // claude_w5_creature_actions RemoveFromActiveCreatureCount(float, int), RET 8
  case 0x5ede40: return area_vfx_queue_announce_005fb490(ptr(read(s + 4)), nullptr, static_cast<int>(a[0]));   // 005EDE40 jmp 005FB490 with ECX=[ECX+4] (area_vfx_announce PlayRumblePattern), RET 4
  case 0x64b4f0: kotor_fastcall_0064b4f0(ptr(s), nullptr, ptr(a[0])); return 0;   // claude_w1_route_bodies SetSpellTargetPosition, RET 4
  case 0x4015a0: kotor_m15e_slot090_void0_004015a0(ptr(s), nullptr); return 0;
  case 0x413660: return static_cast<U>(c2da_current_get_int_00413660(ptr(s), nullptr, static_cast<int>(a[0]), ptr(a[1]), static_cast<int*>(ptr(a[2]))));
  case 0x4af240: return exo_array_list_count_004af240(ptr(s), nullptr, a[0]);
  case 0x4d7360: return word(recovered_event_string_004d7360(ptr(s), nullptr, ptr(a[0]), a[1]));
  case 0x4d7590: recovered_event_destroy_004d7590(ptr(s), nullptr); return 0;
  case 0x5c5630: gui_scene_void_return_0c_005c5630(ptr(s), nullptr, a[0], a[1], a[2]); return 0;
  case 0x5e5290: return static_cast<U>(static_cast<kotor_reconstruction::StartupString*>(ptr(s))->BorrowedEquals005e5290(literal(a[0])));
  case 0x5e58a0: return static_cast<U>(frame_LogIsEmpty_005e58a0(ptr(s), nullptr));
  case 0x5ed5b0: return word(frame_R4_005ed5b0(ptr(s), nullptr, a[0]));
  case 0x634620: return w1::perception::party_member_00634620(s, a[0], *this);
  case 0x66dcc0: w1::effects::int_list_remove_0066dcc0(s, a[0], *this); return 0;
  case 0x4d6480: return w1::leaves::message_get_integer_004d6480(s, a[0], *this);
  case 0x4d64c0: return w1::leaves::message_get_object_004d64c0(s, a[0], *this);
  case 0x5d0fd0: return recovered_vm_run_script_situation_005d0fd0(ptr(s), nullptr, ptr(a[0]), a[1], a[2]);   // W6-owned
  case 0x4ae760: return word(recovered_item_lookup_004ae760(ptr(s), nullptr, a[0]));
  case 0x5b31a0: return word(kotor_m15b_base_item_array_get_base_item_005b31a0(ptr(s), nullptr, a[0]));
  case 0x553210: recovered_item_owner_00553210(ptr(s), nullptr, a[0], a[1], a[2], a[3]); return 0;
  case 0x5538a0: recovered_item_remove_area_005538a0(ptr(s), nullptr); return 0;
  case 0x55dc10: kotor_fastcall_0055dc10(ptr(s), nullptr); return 0;   // claude_w1_route_bodies CloseItemForAllPlayers, RET 0
  case 0x510ef0: return recovered_creature_remove_item_00510ef0(ptr(s), nullptr, ptr(a[0]), a[1], a[2], ptr(a[3]));   // W2 w2_party_native RemoveItem, RET 10h
  case 0x62eeb0: kotor_fastcall_0062eeb0(ptr(s), nullptr, static_cast<unsigned char>(a[0]), a[1]); return 0;   // W4 main_interface CGuiInGame::UpdateStatus, RET 8
  case 0x58d570: kotor_fastcall_0058d570(ptr(s), nullptr, a[0]); return 0;   // claude_w1_route_bodies CSWSTrigger::OnEnterMine, RET 4
  case 0x506b30: kotor_fastcall_00506b30(ptr(s), nullptr, a[0], a[1]); return 0;   // claude_w1_route_bodies SetTransitionPending, RET 8
  case 0x5e5790: return static_cast<U>(static_cast<kotor_reconstruction::StartupString*>(ptr(s))->BorrowedLength005e5790());
  case 0x4aecc0: return word(recovered_server_set_move_to_module_pending_004aecc0(ptr(s), nullptr, a[0])); // 004AECC0, RET4
  case 0x4aecd0: return word(recovered_server_set_move_to_module_string_004aecd0(ptr(s), nullptr, ptr(a[0]))); // 004AECD0, RET4
  case 0x4aed30: return word(recovered_server_set_move_to_module_start_waypoint_004aed30(ptr(s), nullptr, ptr(a[0]))); // 004AED30, RET4
  case 0x62ac40: kotor_fastcall_0062ac40(ptr(s), nullptr); return 0;
  case 0x635350: return kotor_fastcall_00635350(ptr(s), nullptr);   // claude_w1_route_bodies CSWParty::IsPartyTogether, RET 0
  case 0x506ac0: return kotor_fastcall_00506ac0(ptr(s), nullptr);   // claude_w1_route_bodies GetNextTransitionPendingID, RET 0 (AL; callers store the byte)
  case 0x62abf0: kotor_m15_ingame_start_global_fade_0062abf0(ptr(s), nullptr, a[0], a[1], a[2], ptr(a[3])); return 0;
  case 0x4ae9a0: kotor_fastcall_004ae9a0(ptr(s), nullptr, a[0], a[1]); return 0;
  case 0x58ee30: kotor_fastcall_0058ee30(ptr(s), nullptr); return 0;   // claude_combat_spell_mine CSWSTrigger::RemoveFromArea, RET 0
  case 0x560cd0: recovered_area_transition_bmp_00560cd0(ptr(s), nullptr, a[0], ptr(a[1])); return 0;   // W2 w2_entry_native SetAreaTransitionBMP, RET 8
  case 0x584b10: return recovered_placeable_acquire_item_00584b10(ptr(s), nullptr, ptr(a[0]), a[1], a[2]);   // W6 t4 native CSWSPlaceable::AcquireItem, RET 0Ch
  case 0x587770: kotor_fastcall_00587770(ptr(s), nullptr); return 0;   // claude_w1_route_bodies ClosePlaceableForAllPlayers, RET 0
  case 0x584950: kotor_fastcall_00584950(ptr(s), nullptr); return 0;   // claude_w1_route_bodies CSWSPlaceable::RemoveFromArea, RET 0
  case 0x587420: w5_placeable_open_inventory_00587420(ptr(s), nullptr, a[0], a[1]); return 0;   // claude_w5_object_actions OpenInventory, RET 8
  case 0x5a6960: return recovered_stats_effect_query_005a6960(ptr(s), nullptr, a[0], a[1]);
  case 0x5b5e90: kotor_m15e_slot008_void8_005b5e90(ptr(s), nullptr, a[0], a[1]); return 0;
  case 0x589c70: return w5_door_open_door_00589c70(ptr(s), nullptr, a[0]);   // W5 t2
  case 0x589580: return word(w5_door_get_linked_object_00589580(ptr(s), nullptr));   // W5 t2
  case 0x58b490: kotor_fastcall_0058b490(ptr(s), nullptr); return 0;   // claude_w1_route_bodies CSWSDoor::RemoveFromArea, RET 0
  case 0x58fec0: return kotor_fastcall_0058fec0(ptr(s), nullptr, a[0]);   // claude_w1_route_bodies RemoveStaticBoundingBox, RET 4
  case 0x589600: recovered_door_state_00589600(ptr(s), nullptr, a[0], a[1]); return 0;
  case 0x5890d0: return w1::door_event::door_is_linked_005890d0(s, *this);
  case 0x555f40: return recovered_item_in_repository_00555f40(ptr(s), nullptr, ptr(a[0]), a[1]);
  case 0x555fd0: return recovered_remove_item_00555fd0(ptr(s), nullptr, ptr(a[0]));
  case 0x4cac30: w1::object_actions::add_action_node_parameter_004cac30(s, a[0], a[1], a[2], a[3], *this); return 0;
  case 0x57cb10: { alignas(4) unsigned char f2[4]; return w1::object_actions::add_do_command_action_0057cb10(s, a[0], word(f2), *this); }
  case 0x5edc30: kotor_fastcall_005edc30(ptr(s), nullptr, a[0], a[1]); return 0;   // change_character ChangeCharacter (-> 005F6B60, RET 8)
  case 0x5ed780: recovered_client_set_player_creature_005ed780(ptr(s), nullptr, a[0]); return 0;   // W3 client_objects SetPlayerCreature (-> 005F2BB0, RET 4)
  case 0x5ed720: kotor_fastcall_005ed720(ptr(s), nullptr); return 0;   // W4 client_update AttachPlayerGameObjectToArea (-> 005F5CF0, RET 0)
  case 0x6349f0: return kotor_fastcall_006349f0(ptr(s), nullptr, a[0]);   // W4 membership CSWParty::RemoveCharacter, RET 4
  case 0x4ccd80: w5_object_clear_all_actions_004ccd80(ptr(s), nullptr, a[0]); return 0;   // W5 t2b (ACTION-ABI.md: W5 owns 004CCD80)
  case 0x610a10: kotor_fastcall_00610a10(ptr(s), nullptr, a[0]); return 0;   // W4 selection CSWCCreature::SetCombatMode, RET 4
  case 0x5d1000: return recovered_vm_outer_pop_integer_005d1000(ptr(s), nullptr, ptr(a[0]));
  case 0x5d1020: return recovered_vm_outer_pop_float_005d1020(ptr(s), nullptr, ptr(a[0]));
  case 0x5d10c0: return recovered_vm_outer_pop_object_005d10c0(ptr(s), nullptr, ptr(a[0]));
  case 0x5d10d0: return recovered_vm_outer_push_object_005d10d0(ptr(s), nullptr, a[0]);
  case 0x5d10b0: return recovered_vm_outer_push_engine_structure_005d10b0(ptr(s), nullptr, static_cast<int>(a[0]), a[1]);
  case 0x5d10a0: return recovered_vm_outer_pop_engine_structure_005d10a0(ptr(s), nullptr, static_cast<int>(a[0]), ptr(a[1]));
  case 0x4cfac0: w5_object_add_open_door_action_004cfac0(ptr(s), nullptr, a[0], a[1]); return 0;
  case 0x4cfb20: w5_object_add_close_door_action_004cfb20(ptr(s), nullptr, a[0], a[1]); return 0;
  case 0x635460: return word(kotor_fastcall_00635460(ptr(s), nullptr));
  case 0x5ed580: return word(kotor_fastcall_005ed580(ptr(s), nullptr, a[0]));
  case 0x5e58c0: return static_cast<U>(ini_comparison::CompareNC005e58c0(static_cast<kotor_reconstruction::StartupString*>(ptr(s)), nullptr, literal(a[0])));
  case 0x4b1f70: return kotor_fastcall_004b1f70(ptr(s), nullptr, a[0]);
  case 0x4cd170: w1x_object_set_orientation_004cd170(ptr(s), nullptr, ptr(a[0])); return 0;
  case 0x4aed90: return w1::server_queries::server_player_list_004aed90(s, *this);
  case 0x4aec90: return w1::server_queries::server_info_004aec90(s, *this);
  case 0x4eff30: return w1::server_queries::creature_is_injured_004eff30(s, *this);
  case 0x4cc330: w1::server_queries::object_delete_dialog_004cc330(s, *this); return 0;
  case 0x4f7750: return w1::server_queries::creature_following_leader_004f7750(s, *this);
  case 0x57cb80: { alignas(4) unsigned char f[0x28 * 8]; return w1::faction::get_reputation_0057cb80(s, a[0], a[1], word(f), *this); }
  case 0x4ae950: return static_cast<U>(recovered_server_faction_of_object_004b40c0(ptr(read(s + 4)), nullptr, a[0], static_cast<unsigned*>(ptr(a[1]))));   // 004AE950 jmp 004B40C0 with ECX = [ECX+4]; W7 owns 004B40C0
  case 0x52b280: return w1::faction::is_npc_faction_0052b280(s, a[0], *this);
  case 0x52b420: return w1::faction::npc_faction_reputation_0052b420(s, a[0], a[1], *this);
  case 0x513ff0: return w1::faction::creature_reputation_00513ff0(s, a[0], a[1], *this);
  case 0x4ab130: vector_normalize_004ab130(ptr(s), nullptr); return 0;
  case 0x4b14f0: { alignas(4) unsigned char f[4]; return w1::start_module::get_module_004b14f0(s, word(f), *this); }
  case 0x4ba920: { alignas(4) unsigned char f[0x2c] = {}; return w1::start_module::start_new_module_004ba920(s, a[0], word(f), *this); }
  case 0x401430: return word(kotor_cdecl_00401430(a[0]));                     // 004BA856 ClearLogCounters; callee ignores the argument
  case 0x5df4e0: kotor_fastcall_005df4e0(ptr(s), nullptr); return 0;          // 004BA878 CExoInput::ClearEvents
  case 0x5ed4f0: return word(frame_GetWorldTimer_005ed4f0(ptr(s), nullptr));  // 004BA8B2/004BA8DB GetWorldTimer
  case 0x4adff0: kotor_fastcall_004adff0(ptr(s), nullptr); return 0;          // 004BA8B9/004BA8CD PauseWorldTimer
  case 0x5edbe0: disable_input_005edbe0(ptr(s), nullptr); return 0;           // 004BA904 -> 0061F7C0
  case 0x4b95b0: return w1x_server_load_module_004b95b0(ptr(s), nullptr, ptr(a[0]), a[1]); // 004BA912 LoadModule, RET8
  case 0x5edba0: kotor_fastcall_005edba0(ptr(s), nullptr); return 0;
  case 0x4adbd0: frame_R4_004adbd0(ptr(s), nullptr); return 0;
  case 0x5ee090: return load_screen_get_next_load_screen_hint_strref_005ee090(ptr(s), nullptr, ptr(a[0]), a[1]);
  case 0x5edf00: kotor_m15_client_set_load_screen_hint_005edf00(ptr(s), nullptr, a[0]); return 0;   // 0813 SetLoadScreenHint (-> 005F37F0, RET 4)
  case 0x5ee040: load_screen_set_load_screen_hint_update_timer_005ee040(ptr(s), nullptr, a[0]); return 0;
  case 0x5edc80: kotor_fastcall_005f3030(ptr(read(s + 4)), nullptr, a[0]); return 0;   // 005EDC80 jmp 005F3030 with ECX=[ECX+4] (W4 client_update SetCanSendDialog), RET 4
  case 0x62ec60: kotor_fastcall_0062ec60(ptr(s), nullptr, a[0]); return 0;
  case 0x6332b0: (void)kotor_fastcall_006332b0(ptr(s), nullptr); return 0;   // claude_dialogue_client CGuiInGame::CloseDialog, RET 0
  case 0x5d5e70: kotor_fastcall_005d5e70(ptr(s), nullptr); return 0;   // claude_w1_route_bodies CExoSound::StopAllOneShots (-> 005D8220), RET 0
  case 0x5edb20: return static_cast<U>(kotor_fastcall_005edb20(ptr(s), nullptr));   // claude_w1_route_bodies PlayLoadModuleMovies (-> 00602650), RET 0
  case 0x408800: return static_cast<menu_resources::Manager*>(ptr(s))->directory_00408800(static_cast<kotor_reconstruction::StartupString*>(ptr(a[0])));   // 0027 AddResourceDirectory, RET 4
  case 0x406d60: return word(async_ref_construct_string_00406d60(ptr(s), nullptr, ptr(a[0])));
  case 0x408bc0: return static_cast<menu_resources::Manager*>(ptr(s))->exists_00408bc0(static_cast<menu_resources::Ref*>(ptr(a[0])), a[1], static_cast<unsigned*>(ptr(a[2])));   // 0031 Exists, RET 0Ch
  case 0x4088d0: kotor_fastcall_004088d0(ptr(s), nullptr, ptr(a[0])); return 0;
  case 0x4b4f50: kotor_fastcall_004b4f50(ptr(s), nullptr); return 0;   // claude_w1_route_bodies ForceRunAreaOnExitScripts, RET 0
  case 0x4b2ba0: return static_cast<U>(kotor_fastcall_004b2ba0(ptr(s), nullptr));   // claude_w1_route_bodies StorePlayerCharacters, RET 0
  case 0x4b2e70: return kotor_fastcall_004b2e70(ptr(s), nullptr);   // claude_save_module StoreCurrentModule(), RET 0, EAX 1
  case 0x406d80: return word(static_cast<menu_resources::Ref*>(ptr(s))->construct_00406d80(literal(a[0])));   // 0030 CResRef(const char*), RET 4
  case 0x413cc0: { W1Ref16 r; std::memcpy(r.bytes, ptr(a[0]), 16); return word(c2da_current_construct_00413cc0(ptr(s), nullptr, r, a[1])); }   // 0377 C2DA(CResRef by value, int), RET 14h
  case 0x4143b0: return static_cast<U>(c2da_current_load_004143b0(ptr(s), nullptr));   // 0380 Load2DArray, RET 0
  case 0x4b2380: kotor_fastcall_004b2380(ptr(s), nullptr, ptr(a[0]), ptr(a[1])); return 0;   // claude_w1_route_bodies DoModuleEnterSaveCleanup(name, table), RET 8
  case 0x4b5050: return static_cast<U>(kotor_fastcall_004b5050(ptr(s), nullptr, ptr(a[0]), ptr(a[1])));   // claude_w1_route_bodies AutoSaveOnEnteringModule(name, table), RET 8
  case 0x4b8300: kotor_fastcall_004b8300(ptr(s), nullptr, ptr(a[0]), ptr(a[1])); return 0;   // claude_w1_chains_autosave DoPCAutosave(name, table), RET 8
  case 0x4ba820: return kotor_fastcall_004ba820(ptr(s), nullptr, ptr(a[0]));   // claude_w1_route_bodies ExecuteLoadModule(name), RET 4
  case 0x56cbf0: return recovered_start_start_new_module_0056cbf0(ptr(s), nullptr); // 0056CBF0 SendServerToPlayerModule_StartStartNewModule
  case 0x4f2610: { alignas(4) unsigned char f[0x30] = {}; w1::combat_state::set_combat_state_004f2610(s, a[0], a[1], word(f), *this); return 0; }
  case 0x4d3770: w1::combat_state::combat_round_remove_all_actions_004d3770(s, *this); return 0;
  case 0x57a1f0: return w1::combat_state::rules_server_construct_0057a1f0(s, *this);
  case 0x57a210: w1::combat_state::rules_server_destroy_0057a210(s, *this); return 0;
  case 0x57a220: return w1::combat_state::rules_server_scalar_destroy_0057a220(s, a[0], *this);
  case 0x513fc0: { W1Memory m; return kotor::server_mainloop::creature_faction_00513fc0(m, s); }   // faction_member_helpers (portable)
  case 0x552c50: return word(kotor_original::kotor_fastcall_00552c50(ptr(s), nullptr));
  case 0x551440: kotor_rules::kotor_fastcall_00551440(ptr(s), nullptr); return 0;
  case 0x4d37e0: w5_combat_clear_all_attacks_004d37e0(ptr(s), nullptr); return 0;
  case 0x4ec8e0: kotor_fastcall_004ec8e0(ptr(s), nullptr); return 0;
  case 0x6182a0: recovered_client_creature_set_combat_state_006182a0(ptr(s), nullptr, a[0]); return 0;   // claude_w3_update CSWCCreature::SetCombatState, RET 4
  case 0x4d83a0: return w1::effect_handlers::on_apply_assured_hit_004d83a0(s, a[0], a[1], a[2], *this);
  case 0x4d83f0: return w1::effect_handlers::on_remove_assured_hit_004d83f0(s, a[0], a[1], *this);
  case 0x54bde1: return 0;   // ORIGINAL BUG marker (closeC): GetNearestObject nth <= 0: the core next reads the distance list at index nth-1 (before the array) -- performed as the original does, no trap
  case 0x54bed7: return 0;   // ORIGINAL BUG marker (closeC): GetNearestObject nth <= 0: the core next reads the id list at index nth-1 (before the array) -- performed as the original does, no trap
  case 0x502664: return 0;   // ORIGINAL BUG marker (closeC): RunUnequip off-hand path: the core next reads [off-hand item + 14h] unchecked (faults on null) -- performed as the original does, no trap
  case 0x4d062f: return 0;   // ORIGINAL BUG marker (closeC): RemoveEffect on an absent effect: the core next shifts the array into data[-1] -- performed as the original does, no trap
  case 0x54bb27: return 0;   // ORIGINAL BUG marker (closeC): GetNearestCreatureToLocation reputation criterion: the core next reads the never-written centre slot [esp+1C] -- performed as the original does, no trap
  case 0x54bbeb: return 0;   // ORIGINAL BUG marker (closeC): GetNearestCreatureToLocation perception criterion: the core next reads the never-written centre slot [esp+1C] -- performed as the original does, no trap
  case 0x50ca10: return w1::safe_location::no_nonwalk_polys_in_doors_0050ca10(s, a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], *this);
  case 0x4c0fa0: { alignas(8) unsigned char f[0xa4]; return w1::safe_location::compute_safe_location_in_direction_004c0fa0(s, a[0], a[1], a[2], a[3], a[4], a[5], word(f), *this); }
  case 0x506400: { alignas(8) unsigned char f[0x2c] = {}; return w1::safe_location::position_walkable_00506400(s, a[0], a[1], a[2], word(f), *this); }
  case 0x581a10: { alignas(8) unsigned char f[0x58]; return w1::safe_location::surface_material_walk_check_00581a10(s, a[0], a[1], a[2], word(f), *this); }
  case 0x4bb600: return w2x_area_room_004bb600(ptr(s), nullptr, ptr(a[0]), a[1]);
  case 0x4be5e0: return w1x_area_test_safe_point_004be5e0(ptr(s), nullptr, ptr(a[0]), ptr(a[1]));
  case 0x4bbe20: return static_cast<U>(area_no_nonwalk_polys_in_static_object_004bbe20(a[0], ptr(a[1]), ptr(a[2]), a[3], a[4], a[5], a[6], static_cast<int>(a[7]), ptr(a[8])));
  case 0x596a00: return word(collision_mesh_world_to_local_00596a00(ptr(s), nullptr, ptr(a[0]), ptr(a[1])));
  case 0x581530: return w5_adapt_check_aabb_walkable_00581530(ptr(s), nullptr, W1Vector{a[0], a[1], a[2]}, W1Vector{a[3], a[4], a[5]}, ptr(a[6]));
  case 0x5023a0: { alignas(4) unsigned char f[0x14]; return w1::unequip::run_unequip_005023a0(s, a[0], a[1], a[2], a[3], a[4], word(f), *this); }
  case 0x513e30: return w1::unequip::can_unequip_weapon_00513e30(s, a[0], *this);
  case 0x5a4980: return w1::unequip::inventory_slot_from_item_005a4980(s, a[0], *this);
  case 0x5a4ba0: return w1::unequip::inventory_remove_item_005a4ba0(s, a[0], *this);
  case 0x4efe00: w1::unequip::set_last_instant_equip_004efe00(s, a[0], a[1], *this); return 0;
  case 0x4faa70: return w1::unequip::unequip_item_004faa70(s, a[0], *this);
  case 0x553c30: w1::unequip::item_remove_properties_00553c30(s, a[0], a[1], *this); return 0;
  case 0x555720: { alignas(4) unsigned char v[0xc]; w1::unequip::item_add_to_area_00555720(s, a[0], a[1], a[2], a[3], a[4], word(v), *this); return 0; }
  case 0x4cb160: { W1AreaAccess q; recovered_creature_area_access::set(s, a[0], q); return 0; }   // 1059 SetArea
  case 0x4d05c0: w1::effects::remove_effect_004d05c0(s, a[0], *this); return 0;
  case 0x4d0820: w1::effects::remove_effect_by_creator_004d0820(s, a[0], *this); return 0;
  case 0x5a4950: return w1::item_event::inventory_has_005a4950(s, a[0], *this);
  case 0x4ef770: return word(recovered_creature_inventory_004ef770(ptr(s), nullptr, a[0]));
  case 0x55d330: return recovered_inventory_insert_0055d330(ptr(s), nullptr, static_cast<void**>(ptr(a[0])), a[1], a[2], a[3]);
  case 0x4ece80: recovered_creature_armor_004ece80(ptr(s), nullptr, ptr(a[0]), a[1], a[2]); return 0;
  case 0x5562a0: return recovered_item_weight_005562a0(ptr(s), nullptr);
  case 0x50dfd0: return recovered_area_insert_0050dfd0(ptr(s), nullptr, a[0], a[1]);
  case 0x4feb90: return recovered_creature_equip_004feb90(ptr(s), nullptr, a[0], ptr(a[1]), a[2], a[3], a[4]);
  case 0x4aedb0: return read(read(s + 4) + 0x10010);                 // CServerExoApp::GetSWSMessage (0x10-byte accessor, not claimed)
  case 0x56fd10: return recovered_inventory_unequip_message_0056fd10(ptr(s), nullptr, a[0], a[1], a[2]);   // SendServerToPlayerInventory_Unequip
  case 0x56fd80: return recovered_inventory_unequip_cancel_message_0056fd80(ptr(s), nullptr, a[0], a[1]); // ..._UnequipCancel
  case 0x4f2a50: return kotor_fastcall_004f2a50(ptr(s), nullptr, a[0]);                                   // CSWSCreature::ToggleMode
  case 0x5d1080: return recovered_vm_outer_pop_string_005d1080(ptr(s), nullptr, ptr(a[0]));
  case 0x5e6080: return word(directory_list::LowerCase005e6080(static_cast<kotor_reconstruction::StartupString*>(ptr(s)), nullptr, static_cast<kotor_reconstruction::StartupString*>(ptr(a[0]))));
  case 0x5e5200: return static_cast<U>(async_string_equal_005e5200(ptr(s), nullptr, ptr(a[0])));
  case 0x4b2820: return word(kotor_fastcall_004b2820(ptr(s), nullptr));
  case 0x5a4e90: { W1Memory m; return kotor::server_mainloop::creature_get_class_005a4e90(m, s, a[0]) & 0xff; }   // 0982 GetClass (AL)
  case 0x4cdac0: return w1::vm_actions::has_spell_effect_applied_004cdac0(s, a[0], *this);
  case 0x5d1040: return recovered_vm_outer_pop_vector_005d1040(ptr(s), nullptr, ptr(a[0]));
  case 0x5d1050: return recovered_vm_outer_push_vector_005d1050(ptr(s), nullptr, a[0], a[1], a[2]);
  case 0x610950: return w1::vm_actions::set_lock_orientation_00610950(s, a[0], a[1], *this);
  case 0x517a20: return w1::perception::visible_element_00517a20(s, a[0], *this);
  case 0x57cd90: { alignas(4) unsigned char f[4] = {}; return w1::perception::ai_state_reputation_0057cd90(s, a[0], *this, word(f)); }
  case 0x616940: w1x_pointer_list_insert_00616940(ptr(s), nullptr, a[0], a[1]); return 0;
  case 0x4f78c0: w1::nearest::float_list_insert_004f78c0(s, a[0], a[1], *this); return 0;
  case 0x4f2c30: return w1::nearest::valid_nearest_object_type_004f2c30(s, a[0], a[1], *this);
  case 0x4fc4c0: { alignas(8) unsigned char f[0xc0]; w1::nearest::get_nearest_objects_004fc4c0(s, a[0], a[1], a[2], a[3], word(f), *this); return 0; }
  case 0x5140d0: return 3;                                          // inline_return_3: [00745DC4] (RunActions handles it inline)
  case 0x4ae780: return word(kotor_fastcall_004ae780(ptr(s), nullptr, a[0]));
  case 0x4ae960: kotor_fastcall_004ae960(ptr(s), nullptr, a[0]); return 0;
  case 0x4ae9d0: return word(kotor_fastcall_004ae9d0(ptr(s), nullptr));
  case 0x561080: return word(kotor_fastcall_00561080(ptr(s), nullptr));
  case 0x4d6ae0: recovered_event_string_set_004d6ae0(ptr(s), nullptr, a[0], ptr(a[1])); return 0;
  case 0x56a5f0: return recovered_module_update_time_0056a5f0(ptr(s), nullptr, ptr(a[0]), a[1], a[2], a[3], a[4], a[5], a[6], a[7]);
  case 0x56d1e0: return recovered_area_weather_0056d1e0(ptr(s), nullptr, ptr(a[0]), a[1], a[2]);
  case 0x4d0870: kotor_fastcall_004d0870(ptr(s), nullptr, a[0]); return 0;   // claude_w1_route_bodies RemoveSomeEffectsOfDurationType, RET 4
  case 0x5e5d10: return word(GuiConcat005e5d10(static_cast<kotor_reconstruction::StartupString*>(ptr(s)), nullptr, static_cast<kotor_reconstruction::StartupString*>(ptr(a[0])), static_cast<const kotor_reconstruction::StartupString*>(ptr(a[1]))));   // 0093 CExoString::operator+ (this, hidden out, rhs), RET 8
  case 0x5e5c50: assign_string_005e5c50(ptr(s), nullptr, ptr(a[0])); return 0;   // 0310 (owner ruling)
#include "16750756cb-w1_action_bodies_cases.inc"
  case 0x4ebb70: w1::creature_ai::compute_ai_state_on_action_004ebb70(s, a[0], a[1], *this); return 0;
  case 0x4fa390: { alignas(8) unsigned char f2[0xc4]; w1::mine_check::update_mine_check_004fa390(s, word(f2), *this); return 0; }
  case 0x550b60: return w1::mine_check::roll_dice_00550b60(a[0], a[1], *this);
  case 0x58c8a0: { alignas(8) unsigned char f2[0x64]; w1::mine_check::nearest_point_0058c8a0(s, a[0], a[1], word(f2), *this); return 1; }
  case 0x6449d0: w1x_unique_append_006449d0(ptr(s), nullptr, a[0]); return 0;
  case 0x4ec840: kotor_fastcall_004ec840(ptr(s), nullptr, ptr(a[0])); return 0;   // claude_w1_route_bodies BroadcastMineDetectionData, RET 4
  case 0x50ee50: recovered_set_stealth_mode_0050ee50(ptr(s), nullptr, a[0], a[1]); return 0; // 005D7E30 selected body, RET8
  case 0x4f6f30: { alignas(4) unsigned char s2[8]; w1::action_queue::update_action_queue_004f6f30(s, word(s2), *this); return 0; } // 004F6F30 portable body
  case 0x553960: return word(kotor_m15_menu_active_property_00553960(ptr(s), nullptr, a[0])); // 005A6C50 selected facade, RET4
  case 0x57a2b0: return static_cast<U>(recovered_object_aq_action_id_by_id_0057a2b0(ptr(s), nullptr, a[0], static_cast<int>(a[1]))); // 004FD790 selected export, RET8
  default: trap(t, "W1 callee not provided");
  }
}
}
// ---- exports ----------------------------------------------------------------------------------------------
extern "C" void __fastcall recovered_creature_ai_update_004fe210(void* self, void*) {
  NativePorts p;
  alignas(8) unsigned char frame[0x27c];              // AIUpdate locals + SpawnInHeartbeatPerception frame (uninitialized)
  w1::creature_ai::ai_update_004fe210(word(self), word(frame), p);
}
extern "C" void __fastcall recovered_creature_spawn_heartbeat_perception_004eb6e0(void* self, void*, unsigned mode) {
  NativePorts p; alignas(8) unsigned char frame[0xd0]; std::memcpy(frame + 0x48, &mode, 4);
  w1::creature_ai::spawn_heartbeat_perception_004eb6e0(word(self), word(frame), p);
}
extern "C" void* __fastcall recovered_message_data_construct_004d69f0(void* self, void*) {
  NativePorts p; return ptr(w1::leaves::message_data_construct_004d69f0(word(self), p));
}
extern "C" void __fastcall recovered_message_data_destroy_004d6a40(void* self, void*) {
  NativePorts p; w1::leaves::message_data_destroy_004d6a40(word(self), p);
}
extern "C" void __fastcall recovered_creature_feedback_004ede10(void* self, void*, unsigned id, void* data) {
  NativePorts p; w1::leaves::send_feedback_004ede10(word(self), id, word(data), p);
}
extern "C" void __fastcall recovered_creature_set_invited_004ef520(void* self, void*, unsigned value) {
  NativePorts p; w1::leaves::set_invited_004ef520(word(self), value, p);
}
extern "C" void __fastcall recovered_creature_clean_invitations_00518660(void* self, void*) {
  NativePorts p; alignas(8) unsigned char frame[0x34]; w1::leaves::clean_invitations_00518660(word(self), word(frame), p);
}
extern "C" void __fastcall recovered_creature_update_visible_0051b050(void* self, void*, unsigned mode) {
  NativePorts p; alignas(8) unsigned char frame[0x50]; std::memcpy(frame + 0x2c, &mode, 4);
  w1::perception::update_visible_0051b050(word(self), word(frame), p);
}
extern "C" void* __fastcall recovered_creature_visible_element_00517a20(void* self, void*, unsigned id) {
  NativePorts p; return ptr(w1::perception::visible_element_00517a20(word(self), id, p));
}
extern "C" void __fastcall recovered_creature_remove_visible_00517960(void* self, void*, unsigned id) {
  NativePorts p; w1::perception::remove_visible_00517960(word(self), id, p);
}
extern "C" void __fastcall recovered_creature_do_perception_00502ac0(void* self, void*, void* target) {
  NativePorts p; alignas(8) unsigned char frame[0x80]; w1::perception::do_perception_00502ac0(word(self), word(target), word(frame), p);
}
extern "C" unsigned __fastcall recovered_creature_stealth_detection_00500c00(void* self, void*, void* target, unsigned los, unsigned* spot, unsigned* listen, unsigned flag) {
  NativePorts p; return w1::perception::stealth_detection_00500c00(word(self), word(target), los, word(spot), word(listen), flag, p);
}
extern "C" void __fastcall recovered_creature_add_visible_0051b370(void* self, void*, unsigned id, unsigned a2, unsigned a3, unsigned a4, unsigned a5) {
  NativePorts p; w1::perception::add_visible_0051b370(word(self), id, a2, a3, a4, a5, p);
}
extern "C" unsigned __fastcall recovered_creature_check_visible_005179e0(void* self, void*, unsigned id) {
  NativePorts p; return w1::perception::check_visible_005179e0(word(self), id, p);
}
extern "C" unsigned __fastcall recovered_party_member_00634620(void* party, void*, unsigned id) {
  NativePorts p; return w1::perception::party_member_00634620(word(party), id, p);
}
extern "C" unsigned __fastcall recovered_party_table_leader_00563a00(void* table, void*, unsigned id) {
  NativePorts p; return w1::perception::party_leader_00563a00(word(table), id, p);
}
extern "C" unsigned __fastcall recovered_object_action_groups_004cc690(void* self, void*) {
  NativePorts p; alignas(4) unsigned char s[4]; return w1::action_queue::num_groups_004cc690(word(self), word(s), p);
}
extern "C" void* __fastcall recovered_object_group_position_004cc5c0(void* self, void*, unsigned index) {
  NativePorts p; alignas(4) unsigned char s[4]; return ptr(w1::action_queue::position_by_group_index_004cc5c0(word(self), index, word(s), p));
}
extern "C" unsigned __fastcall recovered_object_action_by_group_004cc530(void* self, void*, unsigned group, void** out) {
  NativePorts p; alignas(4) unsigned char s[4]; return w1::action_queue::action_by_group_id_004cc530(word(self), group, word(out), word(s), p);
}
extern "C" void __fastcall recovered_action_node_destroy_004cc4d0(void* self, void*) {
  NativePorts p; w1::action_queue::action_node_destroy_004cc4d0(word(self), p);
}
extern "C" void __fastcall recovered_object_remove_group_004cc6f0(void* self, void*, unsigned group) {
  NativePorts p; alignas(4) unsigned char s[4]; w1::action_queue::remove_group_004cc6f0(word(self), group, word(s), p);
}
extern "C" unsigned __fastcall recovered_object_acceptable_action_0057a240(void*, void*, unsigned type) {
  return w1::action_queue::acceptable_action_0057a240(type);
}
extern "C" void __fastcall recovered_creature_update_action_queue_004f6f30(void* self, void*) {
  NativePorts p; alignas(4) unsigned char s[8];
  w1::action_queue::update_action_queue_004f6f30(word(self), word(s), p);
}
extern "C" void __fastcall recovered_creature_clear_hostile_004f61d0(void* self, void*, void* target) {
  NativePorts p; alignas(4) unsigned char s[12]; w1::action_queue::clear_hostile_004f61d0(word(self), word(target), word(s), p);
}
extern "C" unsigned __fastcall recovered_creature_invisible_00501950(void* self, void*, void* viewer) {
  NativePorts p; alignas(4) unsigned char s[4]; return w1::perception::invisible_00501950(word(self), word(viewer), word(s), p);
}
extern "C" unsigned __fastcall recovered_creature_listen_detection_004fb4b0(void* self, void*, void* target, unsigned flag) {
  NativePorts p; alignas(8) unsigned char f[0xb8]; return w1::perception::listen_detection_004fb4b0(word(self), word(target), flag, word(f), p);
}
extern "C" unsigned __fastcall recovered_creature_spot_detection_004f1fd0(void* self, void*, void* target, unsigned flag) {
  NativePorts p; alignas(8) unsigned char f[0x8c]; return w1::perception::spot_detection_004f1fd0(word(self), word(target), flag, word(f), p);
}
extern "C" unsigned __fastcall recovered_creature_detect_mode_004edae0(void* self, void*) {
  NativePorts p; return w1::perception::detect_mode_004edae0(word(self), p);
}
extern "C" unsigned __fastcall recovered_object_animation_stationary_004cae60(void*, void*, unsigned animation) {
  NativePorts p; return w1::perception::animation_stationary_004cae60(animation, p);
}
extern "C" void __fastcall recovered_creature_pacify_004fda20(void* self, void*, unsigned id) {
  NativePorts p; w1::perception::pacify_004fda20(word(self), id, p);
}
extern "C" unsigned __fastcall recovered_effect_integer_00503690(void* self, void*, unsigned index) {
  NativePorts p; return w1::perception::effect_integer_00503690(word(self), index, p);
}
extern "C" void __fastcall recovered_creature_combat_round_timer_004faf20(void* self, void*) {
  NativePorts p; alignas(4) unsigned char s[0x10]; w1::combat_timer::update_combat_round_timer_004faf20(word(self), word(s), p);
}
extern "C" void __fastcall recovered_combat_round_increment_timer_004d4c10(void* self, void*, unsigned dt) {
  NativePorts p; alignas(4) unsigned char s[0x10]; w1::combat_timer::increment_timer_004d4c10(word(self), dt, word(s), p);
}
extern "C" void* __fastcall recovered_combat_round_attack_004d2b50(void* self, void*, unsigned index) {
  return ptr(w1::combat_timer::round_attack_004d2b50(word(self), index));
}
extern "C" void __fastcall recovered_object_update_effect_list_004d1730(void* self, void*, unsigned day, unsigned ms) {
  NativePorts p; alignas(8) unsigned char f[0x54]; w1::effects::update_effect_list_004d1730(word(self), day, ms, word(f), p);
}
extern "C" unsigned __fastcall recovered_object_remove_effect_004d06c0(void* self, void*, unsigned lo, unsigned hi) {
  NativePorts p; return w1::effects::remove_effect_by_id_004d06c0(word(self), lo, hi, p);
}
extern "C" void __fastcall recovered_effect_destroy_00503590(void* self, void*) {
  NativePorts p; w1::effects::effect_destroy_00503590(word(self), p);
}
extern "C" void __fastcall recovered_object_remove_effect_target_004cf3f0(void* self, void*, unsigned owner) {
  NativePorts p; w1::effects::remove_effect_target_004cf3f0(word(self), owner, p);
}
extern "C" void __fastcall recovered_int_list_remove_0066dcc0(void* self, void*, unsigned value) {
  NativePorts p; w1::effects::int_list_remove_0066dcc0(word(self), value, p);
}
extern "C" void __fastcall recovered_effect_expiry_00503770(void* self, void*, unsigned* day, unsigned* ms) {
  NativePorts p; w1::effects::effect_expiry_00503770(word(self), word(day), word(ms), p);
}
extern "C" void* __fastcall recovered_creature_client_creature_004efb20(void* self, void*) {
  NativePorts p; return ptr(w1::perception::client_creature_004efb20(word(self), p));
}
// 004EFC70 / 004EFD20 with the original ABI: thiscall, no stack arguments, float result in ST0.
extern "C" float __fastcall recovered_creature_spot_distance_004efc70(void* self, void*) {
  NativePorts p; alignas(4) unsigned char f[0x10]; U bits = w1::perception::spot_distance_004efc70(word(self), word(f), p);
  float r; std::memcpy(&r, &bits, 4); return r;
}
extern "C" float __fastcall recovered_creature_listen_distance_004efd20(void* self, void*) {
  NativePorts p; alignas(4) unsigned char f[0x10]; U bits = w1::perception::listen_distance_004efd20(word(self), word(f), p);
  float r; std::memcpy(&r, &bits, 4); return r;
}
extern "C" unsigned __fastcall recovered_object_run_actions_0057f4a0(void* self, void*, unsigned day, unsigned ms, unsigned lo, unsigned hi) {
  NativePorts p; alignas(8) unsigned char f[0x40]; return w1::run_actions::run_actions_0057f4a0(word(self), day, ms, lo, hi, word(f), p);
}
extern "C" void __fastcall recovered_creature_ai_state_on_action_004ebb70(void* self, void*, unsigned type, unsigned target) {
  NativePorts p; w1::creature_ai::compute_ai_state_on_action_004ebb70(word(self), type, target, p);
}
extern "C" void __fastcall recovered_creature_mine_check_004fa390(void* self, void*) {
  NativePorts p; alignas(8) unsigned char f[0xc4]; w1::mine_check::update_mine_check_004fa390(word(self), word(f), p);
}
extern "C" unsigned __fastcall recovered_trigger_nearest_point_0058c8a0(void* self, void*, void* in, void* out) {
  NativePorts p; alignas(8) unsigned char f[0x64]; w1::mine_check::nearest_point_0058c8a0(word(self), word(in), word(out), word(f), p); return 1;
}
// 005140D0 (action types 0x29/0x2B/0x36): mov eax, [00745DC4] (= 3, failed); ret 4.
extern "C" unsigned __fastcall recovered_action_return_failed_005140d0(void*, void*, void*) { return 3; }
// ---- shared message / VM leaves
extern "C" unsigned __fastcall recovered_message_get_integer_004d6480(void* self, void*, unsigned index) {
  NativePorts p; return w1::leaves::message_get_integer_004d6480(word(self), index, p);
}
extern "C" unsigned __fastcall recovered_message_get_object_004d64c0(void* self, void*, unsigned index) {
  NativePorts p; return w1::leaves::message_get_object_004d64c0(word(self), index, p);
}
// ---- G02 item event handler (item CGameObject subobject vtable +0x78) and leaves
extern "C" void __fastcall recovered_item_event_handler_0055ee10(void* self, void*, unsigned code, unsigned caller, void* data, unsigned, unsigned) {
  NativePorts p; w1::item_event::handler_0055ee10(word(self), code, caller, word(data), p);
}
extern "C" unsigned __fastcall recovered_inventory_has_item_005a4950(void* self, void*, void* item) {
  NativePorts p; return w1::item_event::inventory_has_005a4950(word(self), word(item), p);
}
extern "C" unsigned __fastcall recovered_store_has_item_005c6bb0(void* self, void*, unsigned id) {
  NativePorts p; return w1::item_event::store_has_005c6bb0(word(self), id, p);
}
extern "C" unsigned __fastcall recovered_store_remove_item_005c6ba0(void* self, void*, void* item) {
  NativePorts p; return w1::item_event::store_remove_005c6ba0(word(self), word(item), p);
}
// ---- G05 placeable / door / trigger AIUpdate (vtable +0x70)
extern "C" void __fastcall recovered_placeable_ai_update_005849d0(void* self, void*) {
  NativePorts p; alignas(8) unsigned char f[0xb0]; w1::object_ai::placeable_ai_update_005849d0(word(self), word(f), p);
}
extern "C" void __fastcall recovered_door_ai_update_005889c0(void* self, void*) {
  NativePorts p; alignas(8) unsigned char f[0xb0]; w1::object_ai::door_ai_update_005889c0(word(self), word(f), p);
}
extern "C" void __fastcall recovered_trigger_ai_update_0058d760(void* self, void*) {
  NativePorts p; alignas(8) unsigned char f[0xb0]; w1::object_ai::trigger_ai_update_0058d760(word(self), word(f), p);
}
// ---- G05 trigger event handler (vtable +0x78)
extern "C" void __fastcall recovered_trigger_event_handler_0058f140(void* self, void*, unsigned code, unsigned caller, void* data, unsigned, unsigned) {
  NativePorts p; alignas(8) unsigned char f[0x40]; w1::trigger_event::event_handler_0058f140(word(self), code, caller, word(data), word(f), p);
}
// ---- G05 placeable event handler (vtable +0x78)
extern "C" void __fastcall recovered_placeable_event_handler_00587ba0(void* self, void*, unsigned code, unsigned caller, void* data, unsigned day, unsigned ms) {
  NativePorts p; alignas(8) unsigned char f[0x4c]; w1::placeable_event::event_handler_00587ba0(word(self), code, caller, word(data), day, ms, word(f), p);
}
// ---- G05 door event handler (vtable +0x78)
extern "C" void __fastcall recovered_door_event_handler_0058b850(void* self, void*, unsigned code, unsigned caller, void* data, unsigned day, unsigned ms) {
  NativePorts p; alignas(8) unsigned char f[0x68]; w1::door_event::event_handler_0058b850(word(self), code, caller, word(data), day, ms, word(f), p);
}
extern "C" unsigned __fastcall recovered_door_is_linked_005890d0(void* self, void*) {
  NativePorts p; return w1::door_event::door_is_linked_005890d0(word(self), p);
}
// ---- server action queue (for W7) and SetFadeOutTime
extern "C" void __fastcall recovered_object_add_action_node_parameter_004cac30(void* self, void*, void* node, unsigned index, unsigned type, void* value) {
  NativePorts p; w1::object_actions::add_action_node_parameter_004cac30(word(self), word(node), index, type, word(value), p);
}
extern "C" unsigned __fastcall recovered_object_add_action_004cea20(void* self, void*, unsigned type, unsigned group,
    unsigned t0, void* v0, unsigned t1, void* v1, unsigned t2, void* v2, unsigned t3, void* v3, unsigned t4, void* v4, unsigned t5, void* v5,
    unsigned t6, void* v6, unsigned t7, void* v7, unsigned t8, void* v8, unsigned t9, void* v9, unsigned t10, void* v10, unsigned t11, void* v11,
    unsigned t12, void* v12) {
  NativePorts p; alignas(4) unsigned char f[4];
  const U params[26] = {t0, word(v0), t1, word(v1), t2, word(v2), t3, word(v3), t4, word(v4), t5, word(v5), t6, word(v6), t7, word(v7),
                        t8, word(v8), t9, word(v9), t10, word(v10), t11, word(v11), t12, word(v12)};
  return w1::object_actions::add_action_004cea20(word(self), type, group, params, word(f), p);
}
extern "C" unsigned __fastcall recovered_object_add_do_command_action_0057cb10(void* self, void*, void* situation) {
  NativePorts p; alignas(4) unsigned char f[4]; return w1::object_actions::add_do_command_action_0057cb10(word(self), word(situation), word(f), p);
}
extern "C" void __fastcall recovered_object_set_fade_out_time_004ce8a0(void* self, void*, unsigned value) {
  NativePorts p; alignas(4) unsigned char f[4]; w1::object_actions::set_fade_out_time_004ce8a0(word(self), value, word(f), p);
}
// ---- W1 tier 6 ACTION bodies (command table ABI: thiscall(this, routine, argc), RET 8)
#define W1_ACTION_EXPORT(name, size, call) \
  extern "C" int __fastcall name(void* self, void*, int routine, unsigned argc) { \
    (void)routine; (void)argc; NativePorts p; alignas(8) unsigned char f[size] = {}; return static_cast<int>(call); }
W1_ACTION_EXPORT(recovered_vm_get_last_perceived_0053b230, 4, w1::vm_actions::get_last_perceived_0053b230(word(self), word(f), p))
W1_ACTION_EXPORT(recovered_vm_get_party_member_by_index_0053c870, 4, w1::vm_actions::get_party_member_by_index_0053c870(word(self), word(f), p))
W1_ACTION_EXPORT(recovered_vm_signal_event_005439d0, 0x1c, w1::vm_actions::signal_event_005439d0(word(self), word(f), p))
W1_ACTION_EXPORT(recovered_vm_event_user_defined_00535a90, 4, w1::vm_actions::event_user_defined_00535a90(word(self), word(f), p))
W1_ACTION_EXPORT(recovered_vm_action_open_door_00540260, 0xc, w1::vm_actions::action_open_door_00540260(word(self), word(f), p))
W1_ACTION_EXPORT(recovered_vm_action_close_door_0052f4f0, 0xc, w1::vm_actions::action_close_door_0052f4f0(word(self), word(f), p))
W1_ACTION_EXPORT(recovered_vm_cancel_combat_00546510, 0x18, w1::vm_actions::cancel_combat_00546510(word(self), argc, word(f), p))
W1_ACTION_EXPORT(recovered_vm_destroy_object_0052ff20, 0x18, w1::vm_actions::destroy_object_0052ff20(word(self), argc, word(f), p))
W1_ACTION_EXPORT(recovered_vm_get_position_0053cae0, 0x14, w1::vm_actions::get_position_0053cae0(word(self), word(f), p))
W1_ACTION_EXPORT(recovered_vm_set_facing_00542830, 0x4c, w1::vm_actions::set_facing_00542830(word(self), static_cast<U>(routine), word(f), p))
W1_ACTION_EXPORT(recovered_vm_get_nearest_object_0054b550, 0xc0, w1::vm_actions::get_nearest_object_0054b550(word(self), static_cast<U>(routine), argc, word(f), p))
W1_ACTION_EXPORT(recovered_vm_play_animation_00540550, 0x28, w1::vm_actions::play_animation_00540550(word(self), static_cast<U>(routine), word(f), p))
extern "C" unsigned __fastcall recovered_object_add_action_to_front_004ccb00(void* self, void*, unsigned type, unsigned group,
    unsigned t0, void* v0, unsigned t1, void* v1, unsigned t2, void* v2, unsigned t3, void* v3, unsigned t4, void* v4, unsigned t5, void* v5,
    unsigned t6, void* v6, unsigned t7, void* v7, unsigned t8, void* v8, unsigned t9, void* v9, unsigned t10, void* v10, unsigned t11, void* v11,
    unsigned t12, void* v12) {
  NativePorts p; alignas(4) unsigned char f[4];
  const U params[26] = {t0, word(v0), t1, word(v1), t2, word(v2), t3, word(v3), t4, word(v4), t5, word(v5), t6, word(v6), t7, word(v7),
                        t8, word(v8), t9, word(v9), t10, word(v10), t11, word(v11), t12, word(v12)};
  return w1::object_actions::add_action_to_front_004ccb00(word(self), type, group, params, word(f), p);
}
extern "C" void __fastcall recovered_client_creature_set_lock_orientation_00610950(void* self, void*, unsigned target, unsigned force) {
  NativePorts p; (void)w1::vm_actions::set_lock_orientation_00610950(word(self), target, force, p);
}
extern "C" unsigned __fastcall recovered_object_has_spell_effect_applied_004cdac0(void* self, void*, unsigned spell) {
  NativePorts p; return w1::vm_actions::has_spell_effect_applied_004cdac0(word(self), spell, p);
}
extern "C" unsigned __fastcall recovered_creature_run_unequip_005023a0(void* self, void*, unsigned item, unsigned repository, unsigned combine, void** out, unsigned value) {
  NativePorts p; alignas(4) unsigned char f[0x14]; return w1::unequip::run_unequip_005023a0(word(self), item, repository, combine, word(out), value, word(f), p);
}
extern "C" unsigned __fastcall recovered_creature_can_unequip_weapon_00513e30(void* self, void*, void* item) {
  NativePorts p; return w1::unequip::can_unequip_weapon_00513e30(word(self), word(item), p);
}
extern "C" unsigned __fastcall recovered_inventory_slot_from_item_005a4980(void* self, void*, void* item) {
  NativePorts p; return w1::unequip::inventory_slot_from_item_005a4980(word(self), word(item), p);
}
extern "C" unsigned __fastcall recovered_inventory_remove_item_005a4ba0(void* self, void*, void* item) {
  NativePorts p; return w1::unequip::inventory_remove_item_005a4ba0(word(self), word(item), p);
}
extern "C" void __fastcall recovered_creature_set_last_instant_equip_004efe00(void* self, void*, unsigned slot, unsigned value) {
  NativePorts p; w1::unequip::set_last_instant_equip_004efe00(word(self), slot, value, p);
}
extern "C" unsigned __fastcall recovered_creature_unequip_item_004faa70(void* self, void*, void* item) {
  NativePorts p; return w1::unequip::unequip_item_004faa70(word(self), word(item), p);
}
extern "C" void __fastcall recovered_item_remove_properties_00553c30(void* self, void*, void* creature, unsigned slot) {
  NativePorts p; w1::unequip::item_remove_properties_00553c30(word(self), word(creature), slot, p);
}
extern "C" void __fastcall recovered_item_add_to_area_00555720(void* self, void*, void* area, float x, float y, float z, unsigned flag) {
  NativePorts p; alignas(4) unsigned char v[0xc]; U fx, fy, fz; std::memcpy(&fx, &x, 4); std::memcpy(&fy, &y, 4); std::memcpy(&fz, &z, 4);
  w1::unequip::item_add_to_area_00555720(word(self), word(area), fx, fy, fz, flag, word(v), p);
}
extern "C" void __fastcall recovered_object_remove_effect_004d05c0(void* self, void*, void* effect) {
  NativePorts p; w1::effects::remove_effect_004d05c0(word(self), word(effect), p);
}
extern "C" void __fastcall recovered_object_remove_effect_by_creator_004d0820(void* self, void*, unsigned creator) {
  NativePorts p; w1::effects::remove_effect_by_creator_004d0820(word(self), creator, p);
}
extern "C" unsigned __fastcall recovered_area_no_nonwalk_polys_in_doors_0050ca10(void* self, void*, void* a, void* b, float zlo, float zhi,
    float radius, unsigned object_id, unsigned* out_id, void* list, unsigned flag9, unsigned flag10) {
  NativePorts p; U lo, hi, r; std::memcpy(&lo, &zlo, 4); std::memcpy(&hi, &zhi, 4); std::memcpy(&r, &radius, 4);
  return w1::safe_location::no_nonwalk_polys_in_doors_0050ca10(word(self), word(a), word(b), lo, hi, r, object_id, word(out_id), word(list), flag9, flag10, p);
}
extern "C" unsigned __fastcall recovered_area_compute_safe_location_in_direction_004c0fa0(void* self, void*, void* start, void* dir, float max_dist,
    void* creature, unsigned check_doors, void* out) {
  NativePorts p; alignas(8) unsigned char f[0xa4]; U m; std::memcpy(&m, &max_dist, 4);
  return w1::safe_location::compute_safe_location_in_direction_004c0fa0(word(self), word(start), word(dir), m, word(creature), check_doors, word(out), word(f), p);
}
extern "C" unsigned __fastcall recovered_area_position_walkable_00506400(void* self, void*, W1Vector v) {
  NativePorts p; alignas(8) unsigned char f[0x2c] = {}; return w1::safe_location::position_walkable_00506400(word(self), v.x, v.y, v.z, word(f), p);
}
extern "C" unsigned __fastcall recovered_surface_mesh_walk_material_00581a10(void* self, void*, W1Vector v) {
  NativePorts p; alignas(8) unsigned char f[0x58]; return w1::safe_location::surface_material_walk_check_00581a10(word(self), v.x, v.y, v.z, word(f), p);
}
// CSWSEffectListHandler callbacks (0854 ai_resolve rows; same shape as @recovered_effect_handler_004d9200@20).
extern "C" unsigned __fastcall recovered_effect_apply_assured_hit_004d83a0(void* self, void*, void* object, void* effect, unsigned flag) {
  NativePorts p; return w1::effect_handlers::on_apply_assured_hit_004d83a0(word(self), word(object), word(effect), flag, p);
}
extern "C" unsigned __fastcall recovered_effect_remove_assured_hit_004d83f0(void* self, void*, void* object, void* effect) {
  NativePorts p; return w1::effect_handlers::on_remove_assured_hit_004d83f0(word(self), word(object), word(effect), p);
}
extern "C" void __fastcall recovered_creature_set_combat_state_004f2610(void* self, void*, unsigned state, unsigned flag) {
  NativePorts p; alignas(4) unsigned char f[0x30] = {}; w1::combat_state::set_combat_state_004f2610(word(self), state, flag, word(f), p);
}
extern "C" void __fastcall recovered_combat_round_remove_all_actions_004d3770(void* self, void*) {
  NativePorts p; w1::combat_state::combat_round_remove_all_actions_004d3770(word(self), p);
}
extern "C" void* __fastcall recovered_rules_server_construct_0057a1f0(void* self, void*) {
  NativePorts p; return ptr(w1::combat_state::rules_server_construct_0057a1f0(word(self), p));
}
extern "C" void __fastcall recovered_rules_server_destroy_0057a210(void* self, void*) {
  NativePorts p; w1::combat_state::rules_server_destroy_0057a210(word(self), p);
}
extern "C" void* __fastcall recovered_rules_server_scalar_destroy_0057a220(void* self, void*, unsigned flags) {
  NativePorts p; return ptr(w1::combat_state::rules_server_scalar_destroy_0057a220(word(self), flags, p));
}
extern "C" void* __fastcall recovered_server_get_module_004b14f0(void* self, void*) {
  NativePorts p; alignas(4) unsigned char f[4]; return ptr(w1::start_module::get_module_004b14f0(word(self), word(f), p));
}
extern "C" unsigned __fastcall recovered_server_start_new_module_004ba920(void* self, void*, void* name) {
  NativePorts p; alignas(4) unsigned char f[0x2c] = {}; return w1::start_module::start_new_module_004ba920(word(self), word(name), word(f), p);
}
extern "C" void __fastcall recovered_object_set_delete_no_fade_004ce920(void* self, void*, unsigned value) {
  NativePorts p; alignas(4) unsigned char f[4]; w1::vm_actions::set_delete_no_fade_004ce920(word(self), value, word(f), p);
}
extern "C" unsigned __fastcall recovered_party_table_npc_id_005638a0(void* self, void*, unsigned id) {
  NativePorts p; return w1::vm_actions::party_table_npc_id_005638a0(word(self), id, p);
}
extern "C" void __fastcall recovered_party_table_clear_member_005639e0(void* self, void*, unsigned index) {
  NativePorts p; w1::vm_actions::party_table_clear_member_005639e0(word(self), index, p);
}
extern "C" void __fastcall recovered_creature_cancel_combat_004fdaa0(void* self, void*, unsigned flag) {
  NativePorts p; alignas(4) unsigned char f[0xc]; w1::vm_actions::creature_cancel_combat_004fdaa0(word(self), flag, word(f), p);
}
// ---- server queries (W4 review list)
extern "C" void* __fastcall recovered_server_player_list_004aed90(void* self, void*) { NativePorts p; return ptr(w1::server_queries::server_player_list_004aed90(word(self), p)); }
// CSWSObject::GetAIStateReputation(other id), thiscall ret 4. The original returns in AL only (0057CDE3/0057CDEF/0057CDF6:
// mov al,1 / mov al,2 / xor al,al; EAX bits 8..31 are left over); this export returns 0/1/2 zero-extended, which every AL
// consumer reads identically.
extern "C" unsigned __fastcall recovered_object_ai_state_reputation_0057cd90(void* self, void*, unsigned other_id) {
  NativePorts p; alignas(4) unsigned char f[4] = {}; return w1::perception::ai_state_reputation_0057cd90(word(self), other_id, p, word(f));
}
extern "C" void* __fastcall recovered_server_info_004aec90(void* self, void*) { NativePorts p; return ptr(w1::server_queries::server_info_004aec90(word(self), p)); }
extern "C" unsigned __fastcall recovered_server_is_injured_004aee40(void* self, void*, unsigned id) {
  NativePorts p; alignas(4) unsigned char f[4]; return w1::server_queries::server_is_injured_004aee40(word(self), id, word(f), p);
}
extern "C" unsigned __fastcall recovered_internal_is_injured_004b1d20(void* self, void*, unsigned id) {
  NativePorts p; alignas(4) unsigned char f[4]; return w1::server_queries::internal_is_injured_004b1d20(word(self), id, word(f), p);
}
extern "C" unsigned __fastcall recovered_creature_is_injured_004eff30(void* self, void*) { NativePorts p; return w1::server_queries::creature_is_injured_004eff30(word(self), p); }
extern "C" void __fastcall recovered_object_delete_dialog_004cc330(void* self, void*) { NativePorts p; w1::server_queries::object_delete_dialog_004cc330(word(self), p); }
extern "C" unsigned __fastcall recovered_creature_following_leader_004f7750(void* self, void*) { NativePorts p; return w1::server_queries::creature_following_leader_004f7750(word(self), p); }
extern "C" unsigned __fastcall recovered_object_get_reputation_0057cb80(void* self, void*, unsigned target, unsigned arg2) {
  NativePorts p; alignas(4) unsigned char f[0x28 * 8]; return w1::faction::get_reputation_0057cb80(word(self), target, arg2, word(f), p);
}
extern "C" unsigned __fastcall recovered_faction_is_npc_0052b280(void* self, void*, unsigned f) { NativePorts p; return w1::faction::is_npc_faction_0052b280(word(self), f, p); }
extern "C" unsigned __fastcall recovered_faction_npc_reputation_0052b420(void* self, void*, unsigned a, unsigned b) { NativePorts p; return w1::faction::npc_faction_reputation_0052b420(word(self), a, b, p); }
extern "C" unsigned __fastcall recovered_creature_reputation_00513ff0(void* self, void*, unsigned other, unsigned faction) { NativePorts p; return w1::faction::creature_reputation_00513ff0(word(self), other, faction, p); }
extern "C" void __fastcall recovered_creature_get_nearest_objects_004fc4c0(void* self, void*, float range_hostile, float range_other, void* cone, void* all) {
  NativePorts p; alignas(8) unsigned char f[0xc0]; U rh, ro; std::memcpy(&rh, &range_hostile, 4); std::memcpy(&ro, &range_other, 4);
  w1::nearest::get_nearest_objects_004fc4c0(word(self), rh, ro, word(cone), word(all), word(f), p);
}
extern "C" unsigned __fastcall recovered_creature_valid_nearest_object_type_004f2c30(void* self, void*, void* object, int* hostile) {
  NativePorts p; return w1::nearest::valid_nearest_object_type_004f2c30(word(self), word(object), word(hostile), p);
}
extern "C" void __fastcall recovered_float_list_insert_004f78c0(void* self, void*, float value, unsigned index) {
  NativePorts p; U v; std::memcpy(&v, &value, 4); w1::nearest::float_list_insert_004f78c0(word(self), v, index, p);
}
// ---- G04 creature event handler (creature vtable +0x78)
extern "C" void __fastcall recovered_creature_event_handler_004fece0(void* self, void*, unsigned code, unsigned caller, void* data, unsigned, unsigned) {
  NativePorts p; alignas(8) unsigned char f[0x124]; w1::creature_event::event_handler_004fece0(word(self), code, caller, word(data), word(f), p);
}
// ---- object state (creature vtable +0x94 / +0x9C, area CGameObject subobject +0x28)














extern "C" void* __fastcall recovered_module_as_sws_module_004c6bb0(void* self, void*) {
  return ptr(w1::object_state::module_as_sws_module_004c6bb0(word(self)));
}
extern "C" void* __fastcall recovered_area_as_sws_area_0050d340(void* self, void*) {
  return ptr(w1::object_state::area_as_sws_area_0050d340(word(self)));
}
// ---- G06 module / area update pump
extern "C" unsigned __fastcall recovered_module_do_update_004c6dc0(void* self, void*) {
  NativePorts p; alignas(8) unsigned char f[0x5c]; return w1::module_update::module_do_update_004c6dc0(word(self), word(f), p);
}
extern "C" unsigned __fastcall recovered_module_ai_update_004c6c90(void* self, void*) {
  NativePorts p; alignas(8) unsigned char f[0x5c]; return w1::module_update::module_ai_update_004c6c90(word(self), word(f), p);
}
extern "C" void __fastcall recovered_module_update_time_004c6210(void* self, void*, unsigned day, unsigned ms, unsigned elapsed) {
  NativePorts p; alignas(8) unsigned char f[0x38]; w1::module_update::update_time_004c6210(word(self), day, ms, elapsed, word(f), p);
}
extern "C" void __fastcall recovered_module_time_stop_check_004c6480(void* self, void*) {
  NativePorts p; alignas(8) unsigned char f[0x10]; w1::module_update::time_stop_check_004c6480(word(self), word(f), p);
}
extern "C" unsigned __fastcall recovered_area_ai_update_00508a30(void* self, void*) {
  NativePorts p; alignas(8) unsigned char f[0x1c]; return w1::module_update::area_ai_update_00508a30(word(self), word(f), p);
}
extern "C" void __fastcall recovered_area_set_weather_00508050(void* self, void*, unsigned weather, unsigned keep) {
  NativePorts p; alignas(8) unsigned char f[8]; w1::module_update::set_weather_00508050(word(self), weather, keep, word(f), p);
}
extern "C" unsigned __fastcall recovered_server_active_pause_state_004ae990(void* self, void*) {
  NativePorts p; return w1::module_update::active_pause_state_004ae990(word(self), p);   // AL is the result
}
extern "C" void __fastcall recovered_timer_calendar_day_004add50(void*, void*, unsigned day, void* year, void* month, void* dom) {
  NativePorts p; w1::module_update::calendar_day_004add50(day, word(year), word(month), word(dom), p);
}
extern "C" void __fastcall recovered_timer_time_of_day_004add90(void* self, void*, unsigned ms, void* h, void* m, void* s, void* msec) {
  NativePorts p; w1::module_update::time_of_day_004add90(word(self), ms, word(h), word(m), word(s), word(msec), p);
}
extern "C" void __fastcall recovered_object_report_overflow_004ce6e0(void* self, void*, unsigned code, unsigned value, unsigned limit, unsigned) {
  NativePorts p; alignas(8) unsigned char f[4]; w1::module_update::report_overflow_004ce6e0(word(self), code, value, limit, word(f), p);
}
extern "C" unsigned __fastcall recovered_rules_roll_dice_00550b60(void*, void*, unsigned count, unsigned sides) {
  NativePorts p; return w1::mine_check::roll_dice_00550b60(count, sides, p);
}
extern "C" unsigned __fastcall recovered_party_index_006349c0(void* party, void*, unsigned id) {
  NativePorts p; return w1::creature_ai::party_index_006349c0(word(party), id, p);
}
extern "C" unsigned __fastcall recovered_active_timer_004ae830(void* server, void*, unsigned id) {
  NativePorts p; return w1::creature_ai::active_timer_004ae830(word(server), id, p);
}
extern "C" void __fastcall recovered_creature_excited_timer_004ed110(void* self, void*) {
  NativePorts p; w1::creature_ai::excited_timer_004ed110(word(self), p);
}
extern "C" void __fastcall recovered_creature_compute_ai_state_004f1460(void* self, void*) {
  NativePorts p; alignas(4) unsigned char scratch[4]; w1::creature_ai::compute_ai_state_004f1460(word(self), word(scratch), p);
}
extern "C" unsigned __fastcall recovered_stats_adjust_hp_accumulator_005a5c70(void* stats, void*, unsigned amount) {
  NativePorts p; return w1::creature_ai::adjust_hp_accumulator_005a5c70(word(stats), amount, p);
}
extern "C" unsigned __fastcall recovered_stats_adjust_fp_accumulator_005a5cc0(void* stats, void*, unsigned amount) {
  NativePorts p; return w1::creature_ai::adjust_fp_accumulator_005a5cc0(word(stats), amount, p);
}
extern "C" void __fastcall recovered_area_set_stealth_xp_00506aa0(void* area, void*, unsigned value) {
  NativePorts p; w1::creature_ai::set_stealth_xp_00506aa0(word(area), value, p);
}
#include "0ca1a01498-combat_reuse.inc"   // stale-stubs: private copy without the null-faction trap
