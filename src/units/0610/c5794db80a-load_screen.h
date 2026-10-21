#pragma once
// lane-prepare: CSWGuiLoadScreen::CSWGuiLoadScreen 0067A710 (swkotor.exe GOG K1).
// Original: thiscall, one stack word (CSWGuiManager*), RET 4, EAX=this; 0x6B8
// allocation at 005F51F2, call at 005F5213 with Internal+0x274. The body has a
// C++ EH frame (FuncInfo 0077E7DC, 14 unwind states) so the D1 veneer and every
// throwing callee use C++ linkage (/EHsc unwinding stays live).
#include "27c7999c0c-gui_control_load.h"   // lane-gui-load: gui_load::{Panel,Label,Object,String,ResRef} + layout veneers
#include "b601162484-gui_objects.h"        // lane-gui-load (D29 owner): panel/label ctors+dtors, 0040A3E0, 00415E00
#include "c534281817-startup_string.h"     // M41 kotor_reconstruction::StartupString (005E5A90/005E5C20/005E5C50)
#include "c30aaa70eb-resources.h"          // M41 menu_resources::Ref::construct_00406d80
#include "7d1d224ac9-async_load.h"         // M41 async_ref_construct_string_00406d60
#include "689940810d-string_priority.h"    // M41 GuiConcat005e5d10
#include <cstdint>

namespace kotor_original {
// CSWGuiLoadScreen layout (0x6B8; offsets from 0067A740..0067A993):
//   +000 CSWGuiPanel (0x64)   +064 CSWGuiProgressBar PB_PROGRESS (0x150)
//   +1B4 LBL_HINT  +2F4 LBL_LOGO  +434 LBL_LOADING  +578 background label (0x140 each)
//   +29C = LBL_HINT text params (label +D0 text, +18)   +5E8 = background border params
constexpr std::uint32_t load_screen_size = 0x6B8;

// D1 veneer for 0067A710 (ECX self, EDX unused, one stack word).
void* __fastcall kotor_fastcall_0067a710(void* self, void* unused_edx, void* gui_manager);

// ---- lane-gui-load area (D29 control ctors/vtables): not yet declared by lane-gui-load;
// these remain explicit dependencies (C++ linkage so unwinding is preserved) ----
void* __fastcall kotor_fastcall_0041aed0(void* bar, void* unused_edx);                      // CSWGuiProgressBar::CSWGuiProgressBar
void __fastcall kotor_fastcall_00417fb0(void* bar, void* unused_edx, std::int32_t value);   // CSWGuiProgressBar::SetCurValue RET 4
// Unwind-map destructors (FuncInfo 0077E7DC -> funclets 0072E100..0072E146).
void __fastcall kotor_fastcall_0041af70(void* bar, void* unused_edx);     // ~CSWGuiProgressBar (state 1)
}
extern "C" {
// CSWGuiLoadScreen vtable (.rdata 00752DC0; slot0 0067AB60, 0040A5A0, 0040A630,
// 0060E760, 00409EB0, 004015A0...). A modern vtable object is required; the raw
// original VA is never stored. lane-gui-load area (D29 vtables).
extern const void* const kotor_vtable_00752dc0[];
// D49: canonical 0041ACD0 CSWGuiLabel::CSWGuiLabel (owner lane-gui-render; identical declaration).
void* __fastcall kotor_fastcall_0041acd0(void* self, void*) noexcept(false);
}
