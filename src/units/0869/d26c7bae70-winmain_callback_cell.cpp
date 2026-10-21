// lane-m48-a: original .data callback cell 0078E39C and the two original functions it holds.
// Evidence (swkotor.exe GOG K1, dumpbin /disasm, lane-m48-a/d/):
//   .data 0078E39C file-backed cold bytes A0 15 40 00 (= 004015A0).
//   Image-wide references: 0040441A (WinMain: mov dword ptr [0078E39C],00401510h at 00404418),
//   004504E7 (call [0078E39C] at 004504E5, BSP tile scene) and 00457C17 (call [0078E39C] at 00457C15, scene load).
//   004015A0: a lone `ret` (cdecl, no arguments) - provided by the existing aurora-peer-void unit.
//   00401510: Win32 message pump (cdecl, no arguments, RET 0); user32 IAT 0073D3F4 PeekMessageA,
//             0073D3B4 GetMessageA, 0073D3A0 TranslateMessage, 0073D3B8 DispatchMessageA (IAT base 0073D398).
// Consumer spelling (3d/core46/m46_bsp_cells.cpp:37, m46_scene3d_cells.cpp:40):
//   namespace kotor_winmain { extern kotor_scene3d::Callback volatile callback_0078e39c; }  (Callback = void(__cdecl*)())
// Storage of 007A39B8 (WinMain quit flag) is WinMain's own Globals::quit_7a39b8, reached through
// kotor_winmain::bound_context->globals exactly like winmain.cpp (no duplicate cell).
// D6 precedent: the original's own user32 imports are called directly (user32.lib is already linked).
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include "cda5e2e155-winmain.h"

namespace kotor_winmain {
using Callback0078e39c = void (__cdecl*)();

extern "C" {
// 004015A0 (C3 ret), the cell's cold value: single existing provider aurora-reviewed/peer-bodies/void/source.cpp
// (naked `ret`, extern "C" void __cdecl()), exactly the cell's type - no second body.
void __cdecl renderer_void_004015a0();

// 00401510 (installed by WinMain at 00404418): drain the thread's message queue.
void __cdecl winmain_message_pump_00401510() {
    MSG message;                                                              // 00401510 sub esp,1Ch (MSG at [esp+4])
    // ebp = [0073D3F4] PeekMessageA (00401514)
    if (::PeekMessageA(&message, nullptr, 0u, 0u, 0u) == 0) return;           // 0040151A..0040152B (PM_NOREMOVE) je 00401583
    do {
        // esi = [0073D3B4] GetMessageA, edi = [0073D3A0] TranslateMessage, ebx = [0073D3B8] DispatchMessageA
        if (::GetMessageA(&message, nullptr, 0u, 0u) != 0) {                 // 00401542..00401551
            ::TranslateMessage(&message);                                     // 00401553..00401558
            ::DispatchMessageA(&message);                                     // 0040155A..0040155F
        } else {
            bound_context->globals->quit_7a39b8 = 1u;                         // 00401563 mov dword ptr [007A39B8],1
        }
    } while (::PeekMessageA(&message, nullptr, 0u, 0u, 0u) != 0);             // 0040156D..0040157E jne 00401542
}                                                                             // 00401580..00401587 ret
}

// 0078E39C: typed callback cell, constant-initialized to its original cold value 004015A0 (no dynamic initializer).
// The WinMain store at 00404418 (value 00401510) must target THIS cell (open gate, WinMain owner): see PACKET.json.
Callback0078e39c volatile callback_0078e39c = &renderer_void_004015a0;
}
