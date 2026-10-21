#pragma once
// lane-w3 private declarations shared by the w3_client_*.cpp units: other existing providers, the lane-w3 entry
// points, the composition bridge exports (bridge/w3_bridge_*.{inc,cpp}) and the explicit traps (w3_traps.cpp).
#include "dd06b37d22-w3_client.h"

// ------------------------------------------------------------------ existing free-function owners
namespace menu_tables_m41 { void __fastcall SetCameraMode_0061b6f0(void* options, void* unused_edx, std::uint32_t mode); }
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }        // [007A39FC] AppManager*
namespace frame_control { extern unsigned resource_manager_007a39e8; }               // [007A39E8] CExoResMan*
namespace kotor_binding_required { void* __fastcall game_object_004D7D60(void* self, void*, unsigned type, unsigned id); }

extern "C" {
// ------------------------------------------------------------------ client app (existing owners)
void* __fastcall kotor_fastcall_005ed530(void* app, void*);                               // 005ED530 GetModule
void* __fastcall client_get_player_creature_005ed540(void* app, void*);                   // 005ED540 GetPlayerCreature
void* __fastcall frame_R4_005ed5b0(void* app, void*, std::uint32_t id);                    // 005ED5B0 GetCreatureByGameObjectID RET 4
void* __fastcall frame_R4_005ed690(void* app, void*);                                      // 005ED690 GetInGameGui
void* __fastcall kotor_fastcall_005ed700(void* app, void*);                                // 005ED700 GetClientOptions
void* __fastcall frame_GetWorldTimer_005ed4f0(void* app, void*);                           // 005ED4F0 GetWorldTimer
void* __fastcall kotor_m15e_client_get_client_ai_master_005ed6b0(void* app, void*);       // 005ED6B0 GetClientAIMaster
void __fastcall kotor_m15_client_set_load_screen_by_module_005edcf0(void* app, void*, const void* name);   // 005EDCF0 RET 4
void __fastcall kotor_m15_client_show_load_screen_005eda80(void* app, void*, std::uint32_t, std::uint32_t,
                                                           std::uint32_t);                  // 005EDA80 RET 0Ch
void __fastcall kotor_m15_client_set_load_bar_progress_005edd40(void* app, void*, std::int32_t progress,
                                                                std::uint32_t update_hint);  // 005EDD40 RET 8
void __fastcall kotor_fastcall_004015a0(void* self, void*);                                // 004015A0 void_return
unsigned __fastcall net_layer_send_message_to_player_005d4cb0(void* net, void*, unsigned player, const void* data,
                                                              unsigned size, unsigned flags);   // 005D4CB0 RET 10h
void __cdecl kotor_cdecl_00401c10(float seconds, unsigned, unsigned);                     // 00401C10 UpdateScreen
void* __cdecl new_aur_camera_0045d930(const char* name);                                   // 0045D930 NewCAurCamera

// ------------------------------------------------------------------ composition bridge (bridge/)
void __fastcall recovered_bridge_world_timer_set_minutes_per_hour_004adba0(void* timer, void*, std::uint32_t minutes);
void* __fastcall recovered_bridge_module_construct_00643f40(void* memory, void*);
std::uint32_t __fastcall recovered_bridge_resman_add_encapsulated_004087a0(void* resman, void*, void* name,
                                                                          std::uint32_t type);

// ------------------------------------------------------------------ lane-w3 entry points
void* __fastcall kotor_fastcall_005ed570(void* app, void*) noexcept;   // 005ED570 GetGameObjectArray RET; lane-w4 creature/creature_ai.cpp (noexcept: cannot throw)
void* __fastcall recovered_client_create_module_005ed440(void* app, void*) noexcept(false);
void* __fastcall recovered_client_internal_create_module_005eea20(void* internal, void*) noexcept(false);
std::uint32_t __fastcall kotor_fastcall_005ed730(void* app, void*);   // 005ED730 CClientExoApp::InitializeInGameGUI RET; lane-w4 gui/ingame_init.cpp
std::uint32_t __fastcall kotor_fastcall_005f1410(void* internal, void*);   // 005F1410 CClientExoAppInternal::InitializeInGameGUI RET; lane-w4 gui/ingame_init.cpp
std::uint32_t __fastcall recovered_client_send_module_loaded_00677490(void* self, void*) noexcept(false);
std::uint32_t __fastcall recovered_client_module_load_module_0063f660(void* self, void*) noexcept(false);
std::uint32_t __fastcall recovered_client_handle_module_006684f0(void* self, void*, std::uint32_t minor) noexcept(false);
std::uint32_t __fastcall recovered_client_handle_login_0064ec00(void* self, void*, std::uint32_t minor) noexcept(false);
void* __fastcall recovered_client_area_construct_00606090(void* self, void*) noexcept(false);
extern volatile std::uint32_t recovered_client_login_confirmed_008328fc;

// ------------------------------------------------------------------ explicit traps (w3_traps.cpp; owner noted)
// 00632860 CGuiInGame::Initialize (lane-w4 kotor_fastcall_00632860) is reached only through W4's 005ED730 -> 005F1410 now.
std::uint32_t __fastcall recovered_client_handle_module_update_time_006528b0(void* self, void*);
std::uint32_t __fastcall recovered_client_handle_export_reply_006634a0(void* self, void*);
std::uint32_t __fastcall recovered_client_handle_start_new_module_start_00652840(void* self, void*);
std::uint32_t __fastcall recovered_client_handle_start_new_module_end_00652860(void* self, void*);
std::uint32_t __fastcall recovered_client_handle_save_game_status_006529f0(void* self, void*);
std::uint32_t __fastcall recovered_client_handle_end_game_00652ad0(void* self, void*);
void __fastcall recovered_client_unload_module_005ed430(void* app, void*);
}

namespace kotor_w3 {
inline void* client_app() noexcept { return rp(kotor_menu_window_cells::manager_007a39fc, 4); }   // [[007A39FC]+4]
// 005ED6F0 CClientExoApp::GetSWCMessage = [[this+4]+14Ch] (1115 provides it only inside private port adapters;
// field read reproduced inline exactly as those adapters do).
inline void* client_message() noexcept { return rp(rp(client_app(), 4), 0x14C); }
// 005EDFF0 CClientExoApp::GetLoadStep(n) = BYTE [[this+4]+3E4h+n] (inline, same precedent: module_resources_native.inc).
inline U client_load_step(void* app, U n) noexcept { return ld<U8>(rp(app, 4), 0x3E4 + n); }
// [vtbl+0](1): scalar deleting destructor of any original polymorphic object (thiscall RET 4).
inline void delete_object(void* object) noexcept(false) {
    vslot<void* (__fastcall*)(void*, void*, U)>(object, 0)(object, nullptr, 1u);
}
// [vtbl+0](flags): same slot with explicit flags (3 = vector delete of a counted array).
inline void delete_object_flags(void* object, U flags) noexcept(false) {
    vslot<void* (__fastcall*)(void*, void*, U)>(object, 0)(object, nullptr, flags);
}
inline U resource_manager_add_encapsulated(void* name, U type) noexcept(false) {
    return recovered_bridge_resman_add_encapsulated_004087a0(word_ptr(frame_control::resource_manager_007a39e8),
                                                             nullptr, name, type);
}
}  // namespace kotor_w3
