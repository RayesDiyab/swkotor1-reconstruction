#pragma once
// lane-window: genuine CAurInternalGL slot bodies for the native table 00741518
// (swkotor.exe GOG, image base 0x400000) and the typed table constant that the
// renderer-epoch producer stores (renderer_adoption::Inputs::table).
// Convention (D1): original __thiscall -> __fastcall(Renderer*, void* unused_edx, stack words);
// original ECX-free __stdcall entries keep __stdcall. No function-pointer casts (D24).
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error Pinned MSVC x86 only
#endif
#include "4de2cab66b-renderer_adoption.h"
#include <cstdint>

extern "C" {
// +00 0044ED40 scalar deleting destructor (RET 4); EAX = receiver.
std::uint32_t __fastcall genuine_delete_0044ed40(kotor_renderer_init::Renderer*, void*, std::uint32_t flags);
// +04 0044D6D0 FullScreen: mov eax,[ecx+0ACh]; ret.
std::uint32_t __fastcall aurora_fullscreen_0044d6d0(kotor_renderer_init::Renderer*, void*);
// +0C 0044D6E0 GetValidMode: ECX unused, five stack words, RET 14h.
std::uint32_t __stdcall aurora_valid_mode_0044d6e0(std::uint32_t*, std::uint32_t*, std::uint8_t*, std::uint32_t*, std::uint32_t);
// +14 0044E9E0 Reshape (RET 8).
void __fastcall genuine_reshape_0044e9e0(kotor_renderer_init::Renderer*, void*, std::uint32_t width, std::uint32_t height);
// +18 0044D8D0 RestoreDesktopDisplayMode / +1C 0044D900 RestoreAuroraDisplayMode (RET 0).
std::uint32_t __fastcall aurora_restore_desktop_0044d8d0(kotor_renderer_init::Renderer*, void*);
std::uint32_t __fastcall aurora_restore_display_0044d900(kotor_renderer_init::Renderer*, void*);
// +20 0044EA30 ShutDown(release, restore) (RET 8). EAX incidental (epoch type: void).
void __fastcall genuine_shutdown_0044ea30(kotor_renderer_init::Renderer*, void*, std::uint32_t release, std::uint32_t restore);
// +24 0044D990 ResetGammaRamp(gamma float bits, keep) (RET 8); returns 0 (GetDC failed) or 1.
std::uint32_t __fastcall genuine_gamma_0044d990(kotor_renderer_init::Renderer*, void*, std::uint32_t gamma_bits, std::uint32_t keep);
// +28 0044D970 GetGLContext: mov eax,[ecx+0B4h]; ret.
void* __fastcall aurora_get_context_0044d970(kotor_renderer_init::Renderer*, void*);
// +2C 0044D980 MakeCurrentGLContext: jmp [0073D2B0] (wglMakeCurrent), RET 8.
std::int32_t __stdcall aurora_make_current_0044d980(void*, void*);
// 0044EC60 CAurInternalGL::~CAurInternalGL (non-virtual body, RET 0).
void __fastcall aurora_destroy_0044ec60(kotor_renderer_init::Renderer*, void*);
// 0044DA40 CAurInternal (base) scalar deleting destructor, table 007414F0 slot 0 (RET 4).
std::uint32_t __fastcall aurora_base_delete_0044da40(kotor_renderer_init::Renderer*, void*, std::uint32_t flags);
}

namespace kotor_aurora_window {
// Original .rdata 00741518: twelve native entries (0044ED40 .. 0044D980).
// +08 is the existing M41 aurora_context_adoption::initialize_slot (0044DAB0 transport).
extern const renderer_adoption::VTable native_vtable_00741518;
// Original .rdata 007414F0: CAurInternal base table: 0044DA40 then nine _purecall (006FB8DE).
struct BaseVTable {
    renderer_adoption::Delete destroy;
    int (__cdecl* pure[9])();
};
extern const BaseVTable base_vtable_007414f0;
// 00830384: vertex program written only by InitEditor 0044E7CC, released by ShutDown 0044EA7C.
// .bss image zero; no other owner exists (task-22 program_cells.cpp lists ten cells, not this one).
extern void* output_00830384;
}

namespace kotor_aurora_window {
// R-5 / D41: typed read of a live receiver's table word ([ecx] at the original call sites,
// e.g. ReInit 00403850 +04, 00403A57 +0C, 00403923 +20; Message 004029E9 +18, 00402A63 +1C).
// Object-pointer conversion only (D24); consumers then call the typed member
// (table->fullscreen / valid_mode via ->mode / ->desktop / ->display / ->shutdown / ->gamma ...).
inline const renderer_adoption::VTable* table_of(const kotor_renderer_init::Renderer* receiver) noexcept {
    return reinterpret_cast<const renderer_adoption::VTable*>(static_cast<std::uintptr_t>(receiver->vtable_word));
}
}
