#pragma once
// Party-selection lane private declarations.
// Includes the accepted camera-A declaration/type header read-only and adds only the
// exact external declarations this lane's five owned functions need. No bodies here.
#include <cstdint>
#include <cstring>
#include "44cb3ee1fa-client_input_event.h"          // accepted exact five-export types/declarations
#include "8daa5e033b-window_shared_cells.h"         // accepted manager_007a39fc cell

namespace party_selection {
using U8 = std::uint8_t;
using U32 = std::uint32_t;

template <class T>
inline T at(const void* p, U32 off) {
    T value;
    std::memcpy(&value, static_cast<const unsigned char*>(p) + off, sizeof(T));
    return value;
}

template <class T>
inline void put(void* p, U32 off, T value) {
    std::memcpy(static_cast<unsigned char*>(p) + off, &value, sizeof(T));
}

inline void* manager() { return kotor_menu_window_cells::manager_007a39fc; }
inline void* client_app() { return at<void*>(manager(), 4); }
inline void* server_app() { return at<void*>(manager(), 8); }

// Runtime vtable dispatch. The slot is a byte offset; on x86 it is 4-byte aligned.
template <class T>
inline T vcall(void* object, U32 slot) {
    void** vtable = *reinterpret_cast<void***>(object);
    void* entry = vtable[slot / 4];
    using Fn = T(__fastcall*)(void*, void*);
    static_assert(sizeof(Fn) == sizeof(void*), "x86 function pointer width");
    Fn fn;
    std::memcpy(&fn, &entry, sizeof(fn));
    return fn(object, nullptr);
}
}  // namespace party_selection

// Existing composed-link providers (exact symbols from the frozen v5 MAP).
namespace lane_config_required {
void* __fastcall GetPartyTable_004aee70(void* server, void* unused_edx);
}

// Named selected providers that are used by more than one owned body. These are the
// exact C++ namespace symbols in the frozen v5 MAP, not C aliases:
//   005ED690 ?get_in_game_gui_005ed690@kotor_input_required@@YIPAXPAX0@Z  (0457.obj)
//   005ED540 ?get_player_creature_005ed540@kotor_input_required@@YIPAXPAX0@Z
//            (0580-startup46-client-exo-app.obj)
//   0062E550 ?show_solo_mode_query_0062e550@kotor_input_required@@YIXPAX0I@Z
//            (defined once by this lane in source/show_solo_mode_query_0062e550.cpp)
namespace kotor_input_required {
void* __fastcall get_in_game_gui_005ed690(void* app, void* unused_edx);      // RET0/@8
void* __fastcall get_player_creature_005ed540(void* app, void* unused_edx);  // RET0/@8
void __fastcall show_solo_mode_query_0062e550(void* gui, void* unused_edx,
                                               U32 text_id);                  // RET4/@12
}  // namespace kotor_input_required

extern "C" {
// Original literal table 0074666C (selected linked VA 00869870,
// t9-object_projectile_literals-observed.obj). Byte 0 is the inverted
// GetGameObject success flag compared at 005FB0EC/005FB251.
extern const unsigned char object_projectile_literal_0074666c[8];

std::uint32_t __fastcall kotor_fastcall_004aea40(void*, void*);
void* __fastcall kotor_fastcall_004ae910(void*, void*, std::uint32_t);
void* __fastcall kotor_fastcall_004ae770(void*, void*, std::uint32_t);
std::uint32_t __fastcall recovered_creature_incapacitated_004ef890(void*, void*);
void __fastcall kotor_fastcall_005f6b60(void*, void*, std::uint32_t, std::uint32_t);
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void*, void*);
void* __fastcall kotor_fastcall_00635460(void*, void*);
void* __fastcall kotor_fastcall_005ed8b0(void*, void*);
int __fastcall recovered_party_index_006349c0(void*, void*, std::uint32_t);

int __fastcall kotor_fastcall_005f4fb0(
    void* list, void* unused_edx, std::uint32_t id,
    int(__cdecl* compare)(std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t));
unsigned char __fastcall kotor_fastcall_004d8230(void*, void*, std::uint32_t, void**);
void __fastcall kotor_fastcall_0048dde0(void*, void*, std::uint32_t);
unsigned char __fastcall kotor_fastcall_00617ad0(void*, void*, void*);
void __fastcall kotor_fastcall_005f9c60(void*, void*, void*, std::uint32_t,
                                        std::uint32_t, std::uint32_t);
int __fastcall kotor_m15_ingame_tutorial_reason_will_show_0062f420(void*, void*,
                                                                   std::uint32_t);
int __fastcall kotor_m15_ingame_set_tutorial_reason_0062b0d0(void*, void*,
                                                             std::uint32_t);
int __cdecl kotor_cdecl_0061fa90(std::uint32_t, std::uint32_t, std::uint32_t,
                                 std::uint32_t);

void* __fastcall kotor_fastcall_005ed530(void*, void*);
void* __fastcall frame_GetServerArea_00605640(void*, void*);
int __fastcall kotor_m15_client_get_area_not_ready_005edad0(void*, void*);
void* __fastcall kotor_fastcall_005ed6f0(void*, void*);
int __fastcall frame_TogglePauseRequest_00677800(void*, void*);
int __fastcall frame_ContainsPanel_0040bd70(void*, void*, void*);
void __fastcall kotor_fastcall_0040bc70(void*, void*, void*, std::uint32_t,
                                        std::uint32_t);
void __fastcall gui_m48_client_set_input_class_005eda60(void*, void*, std::uint32_t,
                                                        std::uint32_t);
void __fastcall kotor_fastcall_0040a140(void*, void*, std::uint32_t);
void __fastcall kotor_fastcall_0060ee00(void*, void*, std::uint32_t, std::uint32_t,
                                        void*);

// Nested-providers lane exact contracts. One reviewed /alternatename per symbol lives in
// source/nested_bindings.cpp; the expected nested names are the first choice and the
// @kotor_fastcall_ADDR@N names are the reviewed fallback targets.
void* __fastcall kotor_thunk_00687fd0(void*, void*);          // 00687FD0, RET0
void __fastcall kotor_nested_005f4d60(void*, void*, void*);   // 005F4D60, RET4
void __fastcall kotor_nested_006c24a0(void*, void*, std::uint32_t);  // 006C24A0, RET4
}
