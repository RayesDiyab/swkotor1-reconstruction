// Authored small leaves reached through the Shell / ReInit / Message route rows
// (swkotor.exe 9c10e045...1435; lane-routes/evidence/full.asm.txt). Each cites its range.
#include "81dc466724-winmain_routes.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdio>
#include <cstring>

namespace {
using Word = std::uint32_t;
Word word_at(const void* object, Word offset) { Word v; std::memcpy(&v, static_cast<const unsigned char*>(object) + offset, 4); return v; }
void* pointer_at(const void* object, Word offset) { return reinterpret_cast<void*>(word_at(object, offset)); }
const char version_0074154c[] = "SWPC 0027";
}
// Lane-prepare declared this client slot producer; one body, linker alias to it.
#pragma comment(linker, "/alternatename:?IsMoviePlaying0070c1f0@kotor_game_prepare@@YIHPAX0@Z=@kotor_fastcall_0070c1f0@8")

// 0044D6D0 FullScreen: body owned by lane-window (aurora_window_slots.cpp, D45); this name is
// redirected to it (identical fastcall(Renderer*, EDX) ABI, @8 decoration).
#pragma comment(linker, "/alternatename:@kotor_fastcall_0044d6d0@8=@aurora_fullscreen_0044d6d0@8")
// 0044EF60 AurDisableVertexBufferObjects: byte [007B927D]=1.
extern "C" void __cdecl kotor_cdecl_0044ef60() { *winmain_routes::cells.vbo_disabled_007b927d = 1; }
// 0044EF70 AurDisableVertexBufferObjectsWriteOnly: byte [0078E398]=0.
extern "C" void __cdecl kotor_cdecl_0044ef70() { *winmain_routes::cells.vbo_write_only_0078e398 = 0; }
// 0044EF80 AurEnableVertexBufferObjectsWriteOnly: byte [0078E398]=1.
extern "C" void __cdecl kotor_cdecl_0044ef80() { *winmain_routes::cells.vbo_write_only_0078e398 = 1; }
// 0044EEC0..0044EED7 AurGetVersionString: sprintf(007B9228, "SWPC 0027"); return 007B9228.
extern "C" char* __cdecl kotor_cdecl_0044eec0() {
    char* const buffer = winmain_routes::cells.version_007b9228;
    std::sprintf(buffer, version_0074154c);
    return buffer;
}
// 005DF6E0 CExoInput::GetMouseMode: single body = existing M40 input-snapshot raw-AL getter
// ?GetMouseMode@InputView@kotor_reconstruction@@QAEEXZ (mov eax,[ecx+4]; mov al,[eax+378h]; ret == original bytes);
// kotor_fastcall_005df6e0 joined to it in m45-joins/integrator_joins_m45.cpp (thiscall, no stack args == fastcall(ECX,EDX)).
// 005ED7C0 IsClientInitialized: [[ecx+4]+204h].
extern "C" std::uint32_t __fastcall kotor_fastcall_005ed7c0(void* outer, void*) { return word_at(pointer_at(outer, 4), 0x204); }
// 005ED4D0 IsClientImeUI: [[ecx+4]+0DCh].   005ED4E0 GetImeUI: [[ecx+4]+0E0h].
extern "C" std::uint32_t __fastcall kotor_fastcall_005ed4d0(void* outer, void*) { return word_at(pointer_at(outer, 4), 0xdc); }
extern "C" std::uint32_t __fastcall kotor_fastcall_005ed4e0(void* outer, void*) { return word_at(pointer_at(outer, 4), 0xe0); }
// 8-byte wrappers `mov ecx,[ecx+4]; jmp <inner>` (stack words forwarded, inner cleans).
extern "C" void __fastcall kotor_fastcall_005ee170(void* outer, void*, std::uint32_t flags) { kotor_fastcall_0061f9c0(pointer_at(outer, 4), nullptr, flags); }
extern "C" void __fastcall kotor_fastcall_005ee180(void* outer, void*, std::uint32_t flags) { kotor_game_prepare::ShowMouse0061fa00(pointer_at(outer, 4), nullptr, static_cast<int>(flags)); }
extern "C" void __fastcall kotor_fastcall_005ed870(void* outer, void*) { kotor_game_prepare::SetEventDescriptions005eeb10(pointer_at(outer, 4), nullptr); }
extern "C" void __fastcall kotor_fastcall_005ed980(void* outer, void*) { kotor_fastcall_005f9780(pointer_at(outer, 4), nullptr); }
extern "C" void __fastcall kotor_fastcall_005ee160(void* outer, void*) { kotor_fastcall_005f4a00(pointer_at(outer, 4), nullptr); }
extern "C" std::uint32_t __fastcall kotor_fastcall_005df510(void* input, void*) { return kotor_fastcall_005dff40(pointer_at(input, 4), nullptr); }
extern "C" void __fastcall kotor_fastcall_005df590(void* input, void*, void* event) { kotor_fastcall_005e12a0(pointer_at(input, 4), nullptr, event); }
extern "C" void __fastcall kotor_fastcall_005df5b0(void* input, void*, std::uint32_t value) { kotor_fastcall_005e0930(pointer_at(input, 4), nullptr, value); }
extern "C" void __fastcall kotor_fastcall_005df5a0(void* input, void*, std::uint32_t mode) { kotor_fastcall_005e1740(pointer_at(input, 4), nullptr, mode); }
// 0061F9C0..0061F9EE CClientExoAppInternal::HideMouse(flags), RET4: [+3D8]|=flags; if the
// result is nonzero: if [007A39E4] Input: 005DF5E0(input); if [+274] GUI: 0040A260(gui).
extern "C" void __fastcall kotor_fastcall_0061f9c0(void* internal, void*, std::uint32_t flags) {
    unsigned char* const self = static_cast<unsigned char*>(internal);
    Word mask; std::memcpy(&mask, self + 0x3d8, 4);
    mask |= flags; std::memcpy(self + 0x3d8, &mask, 4);
    if (!mask) return;
    if (void* input = kotor_menu_window_cells::input_007a39e4) kotor_fastcall_005df5e0(input, nullptr);
    if (void* gui = pointer_at(internal, 0x274)) kotor_fastcall_0040a260(gui, nullptr);
}
// 004015F0..00401607 handleMouseTrails: if [007A3CA8]>=1 SPI(5Dh, value, 0, 0).
extern "C" void __cdecl kotor_cdecl_004015f0() {
    const int value = kotor_winmain::bound_context->globals->mouse_trails_7a3ca8;
    if (value >= 1) SystemParametersInfoA(0x5d, static_cast<UINT>(value), nullptr, 0);
}
// 00401650..00401667 handleScreenSaver: if [007A3CAC]>=1 SPI(11h, value, 0, 0).
extern "C" void __cdecl kotor_cdecl_00401650() {
    const int value = kotor_winmain::bound_context->globals->screen_saver_7a3cac;
    if (value >= 1) SystemParametersInfoA(0x11, static_cast<UINT>(value), nullptr, 0);
}
