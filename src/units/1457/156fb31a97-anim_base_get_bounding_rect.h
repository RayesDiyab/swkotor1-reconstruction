// Original swkotor.exe animbase GetBoundingRect (+0D8) slot bodies:
//   0069F630 CSWCAnimBase::GetBoundingRect          (push arg; push [ecx+B8]; call 0069EDC0; ret 4)
//   00699C10 CSWCAnimBaseHeadWield::GetBoundingRect (RET 4, EAX 0/1)
//   00699D20 vtordisp thunk (sub ecx,[ecx-4]; jmp 00699C10)
// Typed exactly as KotorM15eAnimBaseVtbl::s0d8 of the PATCHED m15e_animbase.h (tier t4 patches):
// kotor_m15e::U (__fastcall*)(void*, void*, void*).
#pragma once
#include "bd9f6ce7e8-m15e_animbase.h"
extern "C" {
kotor_m15e::U __fastcall anim_base_get_bounding_rect_0069f630(void* self, void* edx, void* rect);
kotor_m15e::U __fastcall anim_base_head_wield_get_bounding_rect_00699c10(void* self, void* edx, void* rect);
kotor_m15e::U __fastcall anim_base_head_wield_get_bounding_rect_vtordisp_00699d20(void* self, void* edx, void* rect);
// GAP (no provider in the composition): 0040A520 CSWGuiExtent::Union, cdecl (out, a, b) -> out.
// Bound by exact symbol only; not implemented here.
void* __cdecl gui_extent_union_0040a520(void* out, const void* a, const void* b);
}
