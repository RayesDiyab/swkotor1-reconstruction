#pragma once
// 006056B0 CSWCArea::AddCorpse.
//
// Original: __thiscall, ECX = CSWCArea*, one stack argument U32, RET 4, void.
// Body 006056B0..006057AF; next function 006057B0. Fields:
//   +0x268 ring[4] of U32
//   +0x278 read index byte
//   +0x279 write index byte
//   sentinel 0x7F000000
//
// The selected map row at 006056B0 is CResRef::operator!=, a different identity.
// Do not bind that row; this object owns the new body.
#include "133115ec0d-r4_sound_corpse_abi.h"

namespace kotor_menu_window_cells {

// Selected 008EE344 (original 007A39FC): pointer to AppManager. +4 is the
// client app. The original loads the cell, then +4, with no null guard.
extern void* volatile manager_007a39fc;

} // namespace kotor_menu_window_cells

extern "C" {

// Selected 00715B20 (original 005ED5B0), @12 RET4. It reads [client+4] and
// tail-calls the selected creature lookup 005F1180.
void* __fastcall frame_R4_005ed5b0(void* client, void* unused_edx, std::uint32_t id) noexcept(false);

// Selected 0053A7E0 (original 005ED5F0), @12 RET4. It reads [app+4] and
// tail-calls the selected placeable lookup 005F12C0.
void* __fastcall recovered_client_get_placeable_by_id_005ed5f0(void* app, void* unused_edx,
                                                               std::uint32_t id) noexcept(false);

// Selected 00523E70 (original 00682E00), @8 RET0. It calls GetServerObject,
// then the +0x48 virtual slot, and returns that EAX. For the finite placeable
// class the selected +0x48 target is 005996A0 constant_self.
void* __fastcall kotor_fastcall_00682e00(void* placeable, void* unused_edx) noexcept(false);

// Selected 00506F80 (original 0063CE60), @24 RET0x10.
void __fastcall recovered_client_object_set_desired_fade_state_0063ce60(
    void* self, void* unused_edx, std::uint32_t a, std::uint32_t b, std::uint32_t c,
    std::uint32_t d) noexcept(false);

// Selected 00506D50 (original 0063E220), @12 RET4.
void __fastcall recovered_client_object_set_fade_state_0063e220(void* self, void* unused_edx,
                                                                std::uint32_t a) noexcept(false);

// 006056B0. ECX = self, one stack U32, RET4, void.
void __fastcall kotor_area_add_corpse_006056b0(void* self, void* unused_edx,
                                               std::uint32_t id) noexcept(false);

} // extern "C"
