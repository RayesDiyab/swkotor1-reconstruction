#pragma once
// lane-r4-lowers: shared declarations for the R4 first-frame lower bodies.
// Naming epoch (same as lane-routes kotor_original_calls.h / D1):
//   original __thiscall -> extern "C" __fastcall kotor_fastcall_<addr>(self, unused EDX, stack words...)
//   original __cdecl    -> extern "C" __cdecl   kotor_cdecl_<addr>(...)
// Every name below is either DEFINED in this lane (marked [lane]) or is a genuine
// original callee that is still missing (marked [MISSING], listed in PACKET.json with
// its size); none is a stub, fixture or default.
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error Approved MSVC x86 compiler required
#endif
#include <cstdint>
#include <cstring>
#include "cd1a77591e-mainloop.h"          // R4 consumer declarations (menu_continuation / frame_control / frame_log)
#include "25d9685aed-entry_providers.h"   // R4 frame_R4_* consumer declarations

namespace r4_lowers {
using U32 = std::uint32_t;
using I32 = std::int32_t;
using U64 = std::uint64_t;
using I64 = std::int64_t;
inline U32 w(const void* p, U32 off) { U32 v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
inline void sw(void* p, U32 off, U32 v) { std::memcpy(static_cast<unsigned char*>(p) + off, &v, 4); }
inline float f(const void* p, U32 off) { float v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
inline void sf(void* p, U32 off, float v) { std::memcpy(static_cast<unsigned char*>(p) + off, &v, 4); }
inline unsigned char b8(const void* p, U32 off) { return static_cast<const unsigned char*>(p)[off]; }
inline void* pw(const void* p, U32 off) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(w(p, off))); }
inline void* at(U32 word) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(word)); }
inline unsigned char* bytes(void* p) { return static_cast<unsigned char*>(p); }
// Native virtual calls through the receiver's own table word (R4 draw.cpp precedent:
// the loaded table word is converted to the slot's exact __thiscall type; no
// function-pointer-to-function-pointer cast).
inline U32 vcall0(void* r, U32 slot) { return reinterpret_cast<U32(__thiscall*)(void*)>(w(pw(r, 0), slot))(r); }
inline U32 vcall1(void* r, U32 slot, U32 a) { return reinterpret_cast<U32(__thiscall*)(void*, U32)>(w(pw(r, 0), slot))(r, a); }
inline U32 vcall4(void* r, U32 slot, U32 a, U32 b, U32 c, U32 d) {
    return reinterpret_cast<U32(__thiscall*)(void*, U32, U32, U32, U32)>(w(pw(r, 0), slot))(r, a, b, c, d);
}
}

// Borrowed canonical cells (owners outside this lane; declarations only).
namespace kotor_menu_window_cells { extern void* volatile input_007a39e4; }       // M41 window-shared-cells.obj
namespace kotor_input_cells { extern void* volatile gui_manager_007a39f4; }       // lane-input input_cells.cpp (D22)
namespace kotor_input_cells_required { extern volatile std::int32_t word_008338f4; } // DEFINED here (r4_cells.cpp)
namespace texture_upload_cells { extern std::uint32_t texture_binds_count_007a4710; } // lane-upload upload_cells.cpp
namespace aurora_model { extern volatile std::uint8_t aurora_model_gate_007b8fc0; }    // lane-3d-model model_instance.cpp
namespace kotor_menu_10_19 { struct TimerDependency; }

// Cells owned by this lane (r4_cells.cpp), no prior owner in M41 or any lane packet.
namespace r4_lowers_cells {
// 007B90FC CExoArrayList<PostedString*> (D50/D59): ONE 12-byte object, original layout
// {data 007B90FC, count 007B9100, alloc 007B9104}; sole owner lane-r4-lowers.
struct PostedList { std::uint32_t volatile data, count, alloc; };
static_assert(sizeof(PostedList) == 12, "original CExoArrayList extent");
extern PostedList posted_list_007b90fc;
extern volatile float arrow_pulse_timer_0078d3e8;          // .data 3F400000
extern volatile std::uint8_t arrow_pulse_up_007a457c;      // .bss 0
extern volatile float arrow_pulse_alpha_0078d3e4;          // .data 3F800000
extern std::uint32_t volatile trace_table_007b9108;        // .bss
extern std::int32_t volatile trace_count_007b910c;         // .bss
extern std::uint32_t volatile string_color_00798bb4[4];    // .data 3F800000 x4
extern std::uint32_t volatile render_counts_00827fa8[6];   // .bss 00827FA8..00827FBC
extern std::uint32_t volatile render_count_007b92a8;       // .bss
}

extern "C" {
// ---------------- defined in this lane ----------------
std::uint32_t __fastcall kotor_fastcall_0040aba0(void* gui, void*, void* panel);                  // GetPosInPanelList [lane]
void* __fastcall kotor_fastcall_0040be00(void* gui, void*);                                       // PopModalPanel [lane]
unsigned char __fastcall kotor_fastcall_004d8230(void* array, void*, std::uint32_t id, void** out); // CGameObjectArray::GetGameObject [lane]
unsigned __fastcall kotor_fastcall_005f1180(void* internal, void*, unsigned id);                  // GetCreatureByGameObjectID internal [lane]
void __fastcall kotor_fastcall_004ae190(void* timer, void*, float scale);                         // CWorldTimer::SetSnapshotScale [lane]
void* __fastcall kotor_fastcall_005ed500(void* client, void*);                                    // GetPauseTimer [lane]
void* __fastcall kotor_fastcall_005ed510(void* client, void*);                                    // GetAnimationTimer [lane]
void* __fastcall kotor_fastcall_004aede0(void* server, void*);                                    // CServerExoApp::GetWorldTimer [lane]
void* __fastcall kotor_fastcall_004aee00(void* server, void*);                                    // CServerExoApp::GetPauseTimer [lane]
void __fastcall kotor_fastcall_0062ac40(void* in_game, void*);                                    // StopGlobalFade [lane]
int __fastcall kotor_fastcall_006244b0(void* fade, void*);                                        // CSWGuiFade::IsFadeDone [lane]
unsigned __fastcall kotor_fastcall_005f2ae0(void* internal, void*);                               // LoadScreenExists [lane]
std::uint32_t __fastcall kotor_fastcall_005df550(void* input, void*, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t); // CExoInput::SetRumble [lane]
std::uint32_t __fastcall kotor_fastcall_00401590(void* object, void*, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t); // return_zero_0x10 [lane]
int __fastcall kotor_fastcall_005df560(void* input, void*);                                       // CExoInput::GetMainDeviceLost [lane]
int __fastcall kotor_fastcall_005dffd0(void* internal, void*);                                    // CExoInputInternal::GetMainDeviceLost [lane]
unsigned __fastcall kotor_fastcall_005d5140(void* net, void*);                                    // CNetLayer status-loop reset [lane]
void* __fastcall kotor_fastcall_005ed530(void* client, void*);                                    // CClientExoApp::GetModule [lane]
float* __cdecl kotor_cdecl_00423bc0(float* out, const float* a, const float* b);                  // Vector::operator-= (out=a-b) [lane]
float __cdecl kotor_cdecl_004ab100(const float* v);                                               // vector_magnitude, ST0 result [lane]
std::uint32_t __fastcall kotor_fastcall_005df510(void* input, void*);                              // CExoInput::GetActive [lane-routes window_leaves.cpp]
// ---------------- MISSING genuine callees (not reached on frame 1 unless noted) ----------------
void __fastcall kotor_fastcall_0040acc0(void* gui, void*) noexcept(false);                                        // CSWGuiManager::DetermineNeedToDraw (304)
void __fastcall kotor_fastcall_0040c1e0(void* gui, void*, std::uint32_t x, std::uint32_t y);      // CSWGuiManager::HandleMouseMove (900)
void __fastcall kotor_fastcall_00689d80(void* main_interface, void*);                             // CSWGuiMainInterface::PopulateMenus (652)
void __fastcall kotor_fastcall_006247a0(void* fade, void*);                                       // CSWGuiFade::ResetFade (104)
std::uint32_t __fastcall kotor_fastcall_0068fdd0(void* pattern, void*, std::uint32_t time_bits, std::uint32_t* left, std::uint32_t* right); // CSWRumblePattern::GetMagnitudes (155)
void __fastcall kotor_fastcall_0048dde0(void* list, void*, std::int32_t index);                   // CExoArrayList<Vector>::deleteAt (72)
void __fastcall kotor_fastcall_005df540(void* input, void*, std::uint32_t active);                // CExoInput::SetActive (8 -> internal)
void __fastcall kotor_fastcall_0062f590(void* in_game, void*);                                    // CGuiInGame::EnterControllerPauseState (94)
void __fastcall kotor_fastcall_005d5690(void* net, void*, std::uint32_t, const char* message, std::uint32_t length); // CNetLayer::NonWindowMessages (134)
void* __fastcall kotor_fastcall_005ed580(void* client, void*, std::uint32_t id);                  // CClientExoApp::GetGameObject (8 -> internal)
void __fastcall kotor_fastcall_00630560(void* in_game, void*, void* entry, std::uint32_t speaker); // CGuiInGame::AppendToDialogBuffer
std::uint32_t __fastcall kotor_fastcall_006a6d80(void* dialog, void*, std::uint32_t index);       // CSWGuiDialog::GetReplyIndex
std::uint32_t __fastcall kotor_fastcall_004aea40(void* server, void*);                            // CServerExoApp::GetPlayerCreatureId (8)
void* __fastcall kotor_fastcall_004ae910(void* server, void*, std::uint32_t id);                  // CServerExoApp::GetClientObjectByObjectId (8)
std::uint32_t __fastcall kotor_fastcall_004aea30(void* server, void*, std::uint32_t id);          // CServerExoApp::ClientToServerObjectId (8)
void* __fastcall kotor_fastcall_004ae750(void* server, void*, std::uint32_t id);                  // CServerExoApp::GetGameObject (8)
void* __fastcall kotor_fastcall_004ae780(void* server, void*, std::uint32_t id);                  // CServerExoApp::GetAreaByGameObjectID (8)
void __fastcall kotor_fastcall_00506a60(void* server_area, void*, void* client_area);             // CSWSArea::AttachmentFromClientArea (13)
void __fastcall kotor_fastcall_00630620(void* in_game, void*, std::uint32_t count);              // CGuiInGame::InitializeDialogAnimationData
void __fastcall kotor_fastcall_00628710(void* in_game, void*, std::uint32_t index, std::uint32_t id, std::uint32_t animation); // CGuiInGame::FUN_00628710
void __fastcall kotor_fastcall_006313a0(void* in_game, void*);                                    // CGuiInGame::SetDialogAnimations
void __fastcall kotor_fastcall_004cb480(void* object, void*, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t); // CSWSObject::ReplyDialog
void __fastcall kotor_fastcall_00633660(void* in_game, void*, std::uint32_t, std::uint32_t);      // CGuiInGame::HandleDialogSelection
void __fastcall kotor_fastcall_0040bd20(void* gui, void*, void* panel);                           // CSWGuiManager::SendPanelToBack (76)
void __fastcall kotor_fastcall_006c7ec0(void* transition, void*, std::uint32_t id, void* position); // CSWGuiInGameAreaTransition::SetTransitionObject (389)
void __fastcall kotor_fastcall_0040bc70(gui_ctor::Gui* gui, void*, void* panel, std::uint32_t, std::uint32_t) noexcept(false); // CSWGuiManager::AddPanel (173; lane-routes name)
void __fastcall kotor_fastcall_0062eb10(void* in_game, void*);                                    // CGuiInGame::HideAreaTransition (207)
void __fastcall kotor_fastcall_004ae960(void* server, void*, std::uint32_t);                      // CServerExoApp::TogglePauseState (8)
void __fastcall kotor_fastcall_004b9240(void* server_internal, void*);                            // CServerExoAppInternal::UnloadModule (850)
void __fastcall kotor_fastcall_005d45d0(void* vm_internal, void*, std::uint32_t*, std::uint32_t, std::uint32_t); // CVirtualMachineInternal::RunScript (459)
void __cdecl kotor_cdecl_0044ce50(void* posted, std::uint32_t, std::uint32_t);                   // AurPrintString (446)
void __fastcall kotor_fastcall_00445080(void* list, void*, void* element);                        // CExoArrayList::Remove (68)
void __cdecl kotor_cdecl_0044d490(const char* text, std::uint32_t, std::int32_t, std::uint32_t); // AurPostString (93)
char* __cdecl kotor_cdecl_0044c1f0(char* line);                                                  // FuncInterp (868; lane-routes name)
void __fastcall kotor_fastcall_005ed310(void* server, void*);                                     // void_return_this+4 (DestroyServer callee)
void __fastcall kotor_fastcall_004ae900(void* server, void*);                                     // CServerExoApp::StopServices (8)
void __fastcall kotor_fastcall_004aec70(void* server, void*);                                     // CServerExoApp::~CServerExoApp
void __cdecl kotor_cdecl_00401440();                                                             // SetWindowTitle
void __cdecl live_free(void*);                                                                   // M41 live-policy.obj (006FA390 family)
// M41 x87 conversion provider (custom ABI; assembly only)
}
namespace kotor_game_prepare {
void __fastcall ShowMouse0061fa00(void* self, void* edx, int);
}
