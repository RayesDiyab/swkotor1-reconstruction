#pragma once
// Producer declarations for the genuine game_prepare / game_release composition
// (lane-prepare, M41 epoch, client_services fastcall-thiscall header form).
// Definitions owned by this lane are marked [lane-prepare]; everything else is
// a still-missing producer and is a link-time UNDEF until its owner supplies it.
// Addresses are swkotor.exe (GOG K1, base 0x400000); see FIELD-MAP.json.
#include "6a19be3575-functional_bootstrap.h"
#include "99ee3f1677-game_lifetime.h"
#include "35e3e29e81-live_publication.h"
#include "fd73ec7604-window_live_views.h"
#include "6acfe4dba0-services.h"
#include <windows.h>
#include <cstdint>

extern "C" {
// ---- [lane-prepare] prepare_image_data.cpp (generated from load-image bytes) --
extern const kotor_menu_live_views::WinMainInitial kotor_image_winmain_initial;
extern const kotor_menu_live_views::ShellInitial kotor_image_shell_initial;
extern const kotor_menu_live_views::ReInitInitial kotor_image_reinit_initial;
extern const kotor_menu_live_views::MessageInitial kotor_image_message_initial;
extern kotor_window_shell::Constants kotor_shell_constants;
// Single canonical cell owners (StartServices words + client_services cells).
extern void* kotor_cell_interface_007a39f0;
extern void* kotor_cell_tlk_007a3a08;                         // [lane-tlk] sole owner (D44)
extern void* kotor_cell_tlk_client_007a3a0c;
extern std::uint32_t kotor_cell_allow_windowed_007a3a30;
extern unsigned char kotor_cell_logging_008328e1;
extern void* kotor_cell_rules_007a3a28;
extern void* kotor_cell_sound_007a39ec;                       // [lane-sound] sole owner (D25)
extern const client_services::Word kotor_rdata_sound_mode_0074c5f8; // [lane-sound] sole owner (D25)
extern volatile std::uint32_t kotor_cell_disable_sound_00832894; // [lane-sound] sole owner (D25)
void __cdecl kotor_sound_publish_audio_providers() noexcept;    // [lane-sound] audio::providers publisher

}

// Function producers use C++ linkage (provider exceptions may propagate under
// /EHsc, as string_priority.h does); machine ABI is unchanged.
namespace kotor_game_prepare {
// ---- [lane-prepare] manager_chain.cpp (Manager ctor 00401160 callees) --------
client_services::Word __stdcall Tick0073d0f8();                          // IAT GetTickCount
void* __fastcall ConstructReentrantServerStats004010c0(void* self, void* edx);
void* __fastcall ConstructObjectTableManager004ae5d0(void* self, void* edx);
void* __fastcall ConstructClientInternal005fbfe0(void* self, void* edx);
void* __fastcall ConstructMiniGameObjectArray0066bf10(void* self, void* edx);

// ---- [lane-prepare] manager_chain.cpp (005FBFE0 callees, integrator assignment)
void* __fastcall ConstructPlayerList005f4f40(void* self, void* edx);     // CExoLinkedList<CSWCPlayer>
void* __fastcall ConstructParty00636170(void* self, void* edx);          // CSWParty::CSWParty (0x1290)
void __fastcall ResetPartyMemberData004eae80(void* self, void* edx);     // CSWPartyMemberData::Reset
// ---- [lane-prepare] client_callees.cpp: typed String transports over the sole
// CExoString owner kotor_reconstruction::StartupString (005E5A90 / 005E5C20).
client_services::String* __fastcall ConstructStringText005e5a90(client_services::String*, void*, const char*);
void __fastcall DestroyString005e5c20(client_services::String*, void*);

// ---- [lane-prepare] client_callees.cpp: Shell CExoString lifetime transports
// 005B3190 / 005E5C20 over kotor_window_shell::String (static init 0073ACE0/
// 0073AD40/0073AD60, atexit 0073B720/0073B750/0073B760).
kotor_window_shell::String* __fastcall kotor_shell_string_construct_005b3190(kotor_window_shell::String*, void*) noexcept;
void __fastcall kotor_shell_string_destroy_005e5c20(kotor_window_shell::String*, void*) noexcept;

// ---- [lane-prepare] window_dispatch.cpp / window_sources.cpp -------------------
kotor_winmain::Tick __cdecl kotor_winmain_invoke(void*, const kotor_winmain::Call&);
kotor_winmain::Word __cdecl kotor_winmain_read_word(void*, kotor_winmain::Word);
kotor_window_shell::Result __cdecl kotor_shell_invoke(void*, const kotor_window_shell::Call&);
// Must write providers->owner before any throw; drain(nullptr) returns true only
// when nothing was acquired.
bool __cdecl kotor_window_sources_acquire(void* aggregate, kotor_menu_live_views::Views*,
    kotor_winmain::Context*, kotor_window_shell::Context*, kotor_live_publication::Providers*);
bool __cdecl kotor_window_sources_drain(void* owner) noexcept;
kotor_reinit::Result __cdecl kotor_reinit_invoke(void*, const kotor_reinit::Call&);
kotor_message::Result __cdecl kotor_message_invoke(void*, const kotor_message::Call&);
void __cdecl kotor_window_write_word(void*, kotor_message::Word, kotor_message::Word);
}

// ---- Original-callee naming epoch shared with lane-routes (kotor_original_calls.h):
// thiscall -> extern "C" __fastcall kotor_fastcall_<addr>(self, unused EDX, ...);
// bodies whose original has an unwinding EH frame use C++ linkage in namespace
// kotor_original under the same name (so /EHsc callers keep their unwind tables).
extern "C" {
// [lane-prepare] backdrop.cpp
DWORD __stdcall kotor_stdcall_00401a30(void* event);                     // initCinematicBackdrop, RET 4
LRESULT __stdcall kotor_stdcall_00401590(HWND, UINT, WPARAM, LPARAM);    // backdrop WndProc: xor eax,eax; ret 10h
extern std::uint32_t kotor_cell_backdrop_class_007a3cec;                 // .data image 0
// [lane-prepare] client_callees.cpp
void __fastcall kotor_fastcall_005df4e0(void* input, void*);                    // CExoInput::ClearEvents
void __fastcall kotor_fastcall_005df540(void* input, void*, int);               // CExoInput::SetActive
void __fastcall kotor_fastcall_005dffb0(void* internal, void*, int);            // CExoInputInternal::SetActive
void __fastcall kotor_fastcall_005f2980(void* internal, void*);                 // TurnOffKeyboardModal
void __fastcall kotor_fastcall_005df5b0(void* input, void*, std::uint32_t);     // [lane-routes] CExoInput::RefreshMouseCursor (D41)
void __fastcall kotor_fastcall_005e0930(void* internal, void*, std::uint32_t);  // [lane-routes] CExoInputInternal::RefreshMouseCursor
void* __fastcall kotor_fastcall_005eda40(void* client, void*);                  // CClientExoApp::GetSWGuiManager
void* __fastcall kotor_fastcall_00635a90(void* self, void*);                    // CSWPartyFollowDataArray ctor
void __fastcall kotor_fastcall_00634b60(void* self, void*, const void* vector, std::uint32_t value); // ::Reset RET 8
void __fastcall kotor_fastcall_00634760(void* self, void*, int formation);      // CSWParty::SetFormation RET 4
// [lane-routes] defined (kotor_original_calls.h)
int __fastcall kotor_fastcall_0070c1f0(void* player, void*);                    // CExoMoviePlayer::IsMoviePlaying
// MISSING (unowned) callees exposed by the bodies above:
void __fastcall kotor_fastcall_005e0710(void* internal, void*);                 // [lane-input area] CExoInputInternal::ClearEvents
void __fastcall kotor_fastcall_005e3c20(void* object, void*, int);              // [lane-input area] CExoRawInputInternal::SetActive (RET 4)
void __fastcall kotor_fastcall_00677e50(void* message, void*);                  // CSWCMessage::SendPlayerToServerInput_AbortDriveControl
void __fastcall kotor_fastcall_00605c40(void* area, void*, int);                // CSWCArea::HiliteAreaObjects RET 4
}
// [lane-sound] owner (donor task-29 sound-mode-closure-r2 sound_mode.cpp):
namespace table29_required { void __fastcall gui_sound_mode_005d5e80(void* outer, void*, std::uint32_t mode); }
namespace kotor_original {
// [lane-prepare] slot producers defined in rules_c2da.cpp,
// load_screen.cpp (identical redeclarations of those units' headers).
void* __fastcall kotor_fastcall_00552c50(void* self, void* edx);                // CSWRules::CSWRules
void* __fastcall kotor_fastcall_0067a710(void* self, void* edx, void* gui_manager); // CSWGuiLoadScreen ctor
// [lane-prepare] client_callees.cpp (EH frame 0072963C: allocation funclets)
void __fastcall kotor_fastcall_00632720(void* in_game, void*);                  // CGuiInGame::InitializeGlobal
// [lane-gui-load area, D29 control ctors/vtables] GUI box constructors + vtable
void* __fastcall kotor_fastcall_00626df0(void* self, void*, void* gui_manager); // CSWGuiMessageBox ctor
void* __fastcall kotor_fastcall_006ce7c0(void* self, void*, void* gui_manager); // CSWGuiSkillInfoBox ctor
void* __fastcall kotor_fastcall_006aa100(void* self, void*, void* gui_manager); // CSWGuiTutorialBox ctor
}
extern "C" const void* const kotor_vtable_007513f8[];   // [lane-gui-load area, D29] vtable stored at 00632820

// ---- lane-config owns CClientOptions (claude-takeover/lane-config/src/
// client_options_load.h, D1 form). Redeclared here with the identical signature.
namespace client_options {
void* __fastcall CClientOptions_0061db60(void* self, void* unused_edx);
}

// ---- lane-r4-lowers default owner (names as consumed by lane-routes kotor_original_calls.h)
namespace kotor_game_prepare {
void __fastcall SetEventDescriptions005eeb10(void* self, void* edx);
void __fastcall SetGlobalUnpauseableState0062b410(void* self, void* edx, client_services::Word);
void __fastcall ShowMouse0061fa00(void* self, void* edx, int);
// [lane-prepare] route_cells.cpp
void publish_make_current_bias(float volatile* bias_7b9274) noexcept;
}
namespace kotor_rules_c2da {
int __fastcall ClientGetIntEntry00413660(void* table, void* unused_edx, int row, client_services::String* column, int* value); // rules_c2da.cpp
}
// lane-menu-tables (d1_table_options_video.cpp) runtime attach of the two Views words.
namespace options_video {
bool attach_saved_video_mode(std::uint32_t* width_78d1d4, unsigned char* depth_78d1e0) noexcept;
void detach_saved_video_mode() noexcept;
// lane-config (video_mode_settings.h, D46): its video_mode_bindings runtime cells.
bool attach_video_mode_cells(std::uint32_t* width_78d1d4, std::uint32_t* mode_78d1dc, unsigned char* depth_78d1e0) noexcept;
void detach_video_mode_cells() noexcept;
}
