#pragma once
// Private typed declarations for the action-input five original functions.
// This header defines no storage, bodies, traps, placeholders, or aliases.
// The five owner definitions live in action_input_providers.cpp and use the
// accepted client_input_event.h declarations verbatim.
#include <cstdint>
#include <cstring>
#include "7eeea93bc2-client_input_event.h"
#include "3884bf8e1d-window_shared_cells.h"

extern "C" {
// Reused selected providers (v5 map pins; see PLAN.md and EVIDENCE/provider-map.txt).
void  __fastcall kotor_fastcall_006887d0(void*, void*);                                   // RET0, void
void  __fastcall kotor_fastcall_00689610(void*, void*, std::uint32_t, std::uint32_t);     // RET8, void
void* __fastcall kotor_fastcall_005ed8b0(void*, void*);                                   // RET0, EAX party
void* __fastcall kotor_fastcall_00635460(void*, void*);                                   // RET0, conditional dispatcher
void* __fastcall kotor_fastcall_005eda40(void*, void*);                                   // RET0, EAX Manager*
void  __fastcall kotor_fastcall_0040a140(void*, void*, std::uint32_t);                    // RET4, PlayGuiSound
int   __fastcall show_tutorial_window_005edf40(void*, void*, std::uint32_t, std::uint32_t,
                                                std::uint32_t, std::uint32_t);             // RET0x10, EAX int

// Nested-providers lane bodies (owned by ff3f01b0-5967-48e8-84ad-fc2485924b73).
// This TU imports only 00674090 and 006880C0. 004D37B0 is the exact nested
// contract consumed by the 006880C0 provider; it is not called by this TU.
void  __fastcall kotor_fastcall_00674090(void*, void*);                                   // RET0, void
void  __fastcall kotor_fastcall_006880c0(void*, void*);                                   // RET0, void
int   __fastcall kotor_fastcall_004d37b0(void*, void*);                                   // RET0, EAX 0/1
}

namespace action_input_detail {
using U8 = std::uint8_t;
using U32 = std::uint32_t;
using I32 = std::int32_t;

// Explicit address/load/store primitives matching the original instruction forms:
//   addr = lea, ld* = mov reg,[mem], st32 = mov [mem],reg.
inline U8* addr(void* p, U32 off) noexcept { return static_cast<U8*>(p) + off; }
inline U32 ld32(const void* p) noexcept { U32 v; std::memcpy(&v, p, sizeof v); return v; }
inline I32 ld32s(const void* p) noexcept { I32 v; std::memcpy(&v, p, sizeof v); return v; }
inline std::int8_t ld8s(const void* p) noexcept { std::int8_t v; std::memcpy(&v, p, sizeof v); return v; }
inline std::uint8_t ld8(const void* p) noexcept { std::uint8_t v; std::memcpy(&v, p, sizeof v); return v; }
inline void st32(void* p, U32 v) noexcept { std::memcpy(p, &v, sizeof v); }

// [007A39FC] is the selected void* volatile AppManager cell. The accepted
// source line 70 uses the same static_cast; no null guard is added.
inline void* app() noexcept {
    kotor_input_hover::AppManager* mgr =
        static_cast<kotor_input_hover::AppManager*>(kotor_menu_window_cells::manager_007a39fc);
    return mgr->client;
}
} // namespace action_input_detail
