#pragma once
// lane-gui-render: 2D GUI draw spine for the first K1 main-menu frame.
// Original: GOG swkotor.exe sha256 9c10e045...91435, image base 0x400000.
//
// Conventions
// - Every original __thiscall entry is emitted as an ABI-identical
//   __fastcall(Self*, void* unused_edx, ...) with identical callee stack
//   cleanup (decision D1). Callers pass nullptr for EDX.
// - Virtual slots are read from the receiver's vptr as typed __fastcall
//   function-pointer values (memcpy of the stored pointer word; no
//   function-pointer casts). The slot types below are views of the original
//   vtables cited at each slot; table storage/installation is owned by the
//   GUI object/vtable owner (lane-gui-load G5), not by this lane.
// - Floats that the original only moves (delta, alpha, rotation) are carried as
//   their raw binary32 DWORD (Word). Where the original routes a value through
//   the x87 stack (FLD m32 / FSTP m32), the round trip is reproduced in asm.
// - Object fields are accessed as raw offsets of the original layouts (offsets
//   cited per use; they agree with lane-gui-load gui_control_load.h views).
#include <cstdint>

namespace gui_render {
using Word = std::uint32_t;
using I32 = std::int32_t;
static_assert(sizeof(void*) == 4 && sizeof(Word) == 4 && sizeof(float) == 4, "x86");

// ---- slot views (D1 fastcall forms of the original thiscall slots) ----------
// GUI object draw/update slots, one stack DWORD (float delta), RET 4:
//   panel +34 Draw / +38 Update (vtable 752F70 CSWGuiMainMenu: 0067AC10 / 0060E760)
//   control +38 Draw (73E5B8 label 00417750, 73E658 button 00417AB0)
//   border/text +0C Draw (73E338 004168C0, 73E3A4 00416240)
using DeltaSlot = void (__fastcall*)(void* self, void* unused_edx, Word delta_bits);
// CAurGUIImageInternal (vtable 74183C) slots used by the border draws:
//   +0C GetImageHeight 0045A090, +10 GetImageWidth 0045A0A0: no args, EAX int.
using ImageMetricSlot = I32 (__fastcall*)(void* self, void* unused_edx);
//   +18 Draw 00459920 and +24 DrawBuffered 00459800: 8 stack DWORDs, RET 20h.
//   (x, y, w, h, flags, angle binary32, const Vector* color, alpha binary32)
using ImageDrawSlot = void (__fastcall*)(void* self, void* unused_edx, I32 x, I32 y, I32 w,
                                         I32 h, Word flags, Word angle_bits,
                                         const void* color, Word alpha_bits);
// CSWCArea-family object at [module+40] slot +80 (called with 0x106E at 005EE25B).
using QuerySlot = Word (__fastcall*)(void* self, void* unused_edx, Word query);
}

extern "C" {
// ---- authored in this lane: GUI controls (gui_control_draw.cpp) -------------
void __fastcall kotor_fastcall_00417750(void* label, void*, gui_render::Word delta_bits);
void __fastcall kotor_fastcall_00417ab0(void* button, void*, gui_render::Word delta_bits);
void __fastcall kotor_fastcall_004168c0(void* border, void*, gui_render::Word delta_bits);
gui_render::I32 __fastcall kotor_fastcall_00414cd0(void* params, void*);
void __fastcall kotor_fastcall_004155b0(void* border, void*, gui_render::I32 h,
    gui_render::I32 w, gui_render::I32 x, gui_render::I32 y, gui_render::Word alpha_bits,
    const void* color);
void __fastcall kotor_fastcall_004157f0(void* border, void*, gui_render::I32 h,
    gui_render::I32 w, gui_render::I32 x, gui_render::I32 y, gui_render::Word alpha_bits,
    const void* color);

// ---- authored in this lane: CSWGuiManager::DrawPCBG (gui_pcbg.cpp) ---------
// R4 consumer name (lane-routes draw.obj composition.h), thiscall RET 4.
void __fastcall frame_DrawCursor_0040ae80(void* manager, void*, gui_render::Word delta_bits) noexcept(false);
// The other R4 manager lowers (0040AB70, 006857A0, 0040C830, 005EE230, 00418300)
// are owned by lane-r4-lowers; lane-gui-render's equivalent bodies are kept only
// as an independent cross-check in crosscheck/.

// ---- borrowed providers (other owners) --------------------------------------
// lane-gui-draw writers (M30 r5 layers.cpp, Task22 producer):
void __stdcall kotor_AurGUIStartLayer_004591B0();
void __stdcall kotor_AurGUIStopLayer_004592B0();
std::uint32_t __cdecl kotor_AurGUISetupViewport_004592F0(std::uint32_t x, std::uint32_t y,
    std::uint32_t width, std::uint32_t height, void* color, std::uint32_t depth_test_zero,
    std::uint32_t alpha_bits);
void __stdcall kotor_AurGUICloseViewport_00459580();
// Aurora GUI image flush 0045A170 (cdecl, one binary32 stack word).
void __cdecl kotor_cdecl_0045a170(gui_render::Word alpha_bits);
}

// ---- cells owned by this lane (gui_render_cells.cpp) ------------------------
namespace kotor_gui_render_cells {
// 0078E43C .data file-backed 0x3F800000 (1.0f): GUI image buffer alpha.
// Readers 00416D23/00416DC1 (Border::Draw), 0045B23C/0045B2ED/0045B31B,
// 006883A3, 006945E3; sole writer 0045B351 (CAurGUIImageInternal::DrawBuffered).
extern volatile std::uint32_t gui_image_alpha_0078e43c;
}
