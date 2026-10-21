#pragma once
// lane-m15-a (L15-A) unit m15a-appearance: CSWCCreatureAppearance (0x6C; allocated by CSWCCreature::CSWCCreature
// 00616BF7, back pointer to the creature at +3Ch written by the ctor 00696494).
// The first 0x3C bytes are the appearance DATA block that SetAppearance copies (rep movs 0Fh dwords at 006DC9E6)
// and that the Create* builders take as their `data` argument (SetAppearanceAlignment passes the appearance object
// itself as data at 0069849F).
// Observed data-block fields (byte offsets): +00 BYTE body variation (clamped 1..0Ah, 00697652), +01 BYTE texture
// variation (00697689), +14 DWORD (00697 6A1: +40h on the body letter), +18 WORD appearance.2da row, +1A BYTE head,
// +21 BYTE tail, +22 BYTE wing, +24/+28 floats (ctor 0.6f/0.5f), +2C WORD alignment (0x32), +30 DWORD, +34 BYTE (FFh).
// Object fields: +38 int (appearance.2da INT column at 00698D21), +3C CSWCCreature*, +40..+54 DWORDs,
// +58/+5C/+60/+64 floats (00698B53..C2A), +68 DWORD (ForceAnimate gate 006978A3).
// Conventions: m15a_common.h (D1 fastcall veneers; RET N per entry).
#include "02417a06c1bef2bc_m15a_common.h"
#include <cstdint>

extern "C" {
// 00696430 CSWCCreatureAppearance(CSWCCreature* creature) RET 4, EAX = this.
void* __fastcall kotor_m15a_appearance_construct_00696430(void* self, void*, void* creature);
// 006964C0 CreateHead(anim_type (byte in a word), data) RET 8 -> int.
std::uint32_t __fastcall kotor_m15a_appearance_create_head_006964c0(void* self, void*, std::uint32_t anim_type,
                                                                    void* data) noexcept(false);
// 00696D40 GetAnimType(CExoString* model_type) RET 4 -> BYTE (AL only).
std::uint8_t __fastcall kotor_m15a_appearance_get_anim_type_00696d40(void* self, void*, void* model_type) noexcept(false);
// 00697610 GetBTypeModelAndTextureResRef(data, CResRef* model_out (may be 0), CResRef* texture_out (may be 0)) RET 0Ch.
void __fastcall kotor_m15a_appearance_get_btype_model_and_texture_00697610(void* self, void*, void* data,
                                                                            void* model_out, void* texture_out) noexcept(false);
// 006978A0 ForceAnimate() RET 0.
void __fastcall kotor_m15a_appearance_force_animate_006978a0(void* self, void*);
// 00697A20 / 00697B00 / 00697BC0 CreateHead/Tail/WingAppearance(data) RET 4 -> int.
std::uint32_t __fastcall kotor_m15a_appearance_create_head_appearance_00697a20(void* self, void*, void* data) noexcept(false);
std::uint32_t __fastcall kotor_m15a_appearance_create_tail_appearance_00697b00(void* self, void*, void* data) noexcept(false);
std::uint32_t __fastcall kotor_m15a_appearance_create_wing_appearance_00697bc0(void* self, void*, void* data) noexcept(false);
// 00697CE0 CreateBTypeBody(anim_type (byte in a word), data) RET 8 -> int.
std::uint32_t __fastcall kotor_m15a_appearance_create_btype_body_00697ce0(void* self, void*, std::uint32_t anim_type,
                                                                          void* data) noexcept(false);
// 00698150 SetAppearanceAlignment(alignment (WORD in a word)) RET 4.
void __fastcall kotor_m15a_appearance_set_appearance_alignment_00698150(void* self, void*, std::uint32_t alignment) noexcept(false);
// 00698550 CreateSimple(anim_type (byte in a word), data) RET 8 -> int.
std::uint32_t __fastcall kotor_m15a_appearance_create_simple_00698550(void* self, void*, std::uint32_t anim_type,
                                                                      void* data) noexcept(false);
// 00698B10 CreateBodyAppearance(data) RET 4 -> int (0 when no body was built).
std::uint32_t __fastcall kotor_m15a_appearance_create_body_appearance_00698b10(void* self, void*, void* data) noexcept(false);
}
