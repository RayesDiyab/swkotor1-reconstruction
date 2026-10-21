#pragma once
// Lane C (`C-panel-ctor-abi`) source-phase declarations for the two load-control
// entry adapters.
//
// This header defines no provider body. `kotor_reconstruction::StartupString`
// mirrors the accepted `ui-save-state/source/ui_save_state_types.h` declaration
// (8-byte packed CExoString view; same members and same member declarations) so
// `StartupString::Assign005e5c50` joins the accepted 0125 object's exact symbol:
//   ?Assign005e5c50@StartupString@kotor_reconstruction@@QAEPAU12@PAU12@@Z
//
// The two owned definitions are in `load_screen_adapters.cpp`; the provider
// below is bind-only (accepted 0841 object).

#include <cstddef>
#include <cstdint>

namespace kotor_reconstruction {

#pragma pack(push, 1)
struct StartupString {
    char* data;              // +0x00 allocated NUL-terminated copy
    std::uint32_t capacity;  // +0x04 byte count including the NUL
    StartupString();
    StartupString(char const* text);
    ~StartupString();
    StartupString* Assign005e5c50(StartupString* rhs);
};
#pragma pack(pop)

static_assert(sizeof(StartupString) == 8, "accepted CExoString layout");
static_assert(offsetof(StartupString, data) == 0, "accepted CExoString data offset");
static_assert(offsetof(StartupString, capacity) == 4, "accepted CExoString length offset");

}  // namespace kotor_reconstruction

extern "C" {

// Accepted selected provider, v5 00402C40 in
// 0841-begin_play_native_hint_reuse.obj. Exact decorated contract:
//   @recovered_loading_hint_005f4760@16
// ECX = inner pointer, EDX unused, two stack arguments, RET 8, EAX = hint.
// Bound only; not re-authored here.
unsigned int __fastcall recovered_loading_hint_005f4760(
    void* inner, void* unused, kotor_reconstruction::StartupString* name,
    unsigned int arg);

// Owned entry adapters. External symbols:
//   @load_screen_set_load_music_005edd30@12
//   @load_screen_get_next_load_screen_hint_strref_005ee090@16
void __fastcall load_screen_set_load_music_005edd30(
    void* internal, void* unused, kotor_reconstruction::StartupString* name);

unsigned int __fastcall load_screen_get_next_load_screen_hint_strref_005ee090(
    void* internal, void* unused, kotor_reconstruction::StartupString* name,
    unsigned int arg);

}  // extern "C"
