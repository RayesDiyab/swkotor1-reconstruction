#pragma once
// lane-m47-core rev2: shared declarations of the direct-call R4 frame replacement (frame-r4-driver / frame-r4-menu_r2 /
// frame-r4-mainloop). Replaces the runtime-leased `frame_menu::bindings` / `menu_bindings` address-dispatch seam
// (D76): the frame now calls the genuine providers directly and keeps its bounded-slice diagnostic in one unit-owned
// object instead of a pointer table.
#include "0ed1bf6788-entry_providers.h"   // frame_R4_* genuine provider joins (r4-lowers), mainloop.h, menu_r2.h, driver.h
#include "6a3a57a963-composition.h"       // frame_RenderBoundary_0044ed90, frame_GetCharacter_006346c0, ... , frame_ContainsPanel_0040bd70
#include "12ed6df888-suffix.h"            // frame_control::sound_manager_007a39ec
namespace frame_menu {
// Bounded-slice diagnostic (formerly reached through bindings.diagnostic); owner: rev2 driver.cpp.
extern Diagnostic diagnostic;
// D21 naked veneer for the game-object virtual slot +98h (2 stack words, callee RET 8): 006035A0 call [eax+98h].
U32 __fastcall object_slot_98(void* object, void* unused_edx, U32 a, U32 b);
}
