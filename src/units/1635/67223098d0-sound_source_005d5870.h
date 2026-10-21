#pragma once
// 005D5870 CExoSoundSource::CExoSoundSource() wrapper, 8-byte outer object.
//
// Original: __thiscall, ECX = outer, no stack arguments, RET 0, EAX = outer.
// The outer object is { +0 vtable, +4 CExoSoundSourceInternal* }.
#include "133115ec0d-r4_sound_corpse_abi.h"

namespace kotor_sound_rev3 {

// Selected 007DEAF8 (original 0074C608): one-slot const array. Slot 0 is
// SourceDelete005d60a0 in 0680-rev3-sound-source-object.obj. The wrapper only
// stores the table address; it never calls through it.
using Deleting = void* (__fastcall*)(void*, void*, std::uint32_t);
extern const Deleting vtable_0074c608[1];

// Selected 006DED60 (original 005DAF50). The selected body builds 0xA0 bytes,
// inlines the 16-byte zero at +0xC, fills the fields through +0x9C, and returns
// self. It may throw; the wrapper's guard must free the block exactly once and
// let the exception continue. EDX is the original unused dummy; the stack
// argument is the system pointer.
void* __fastcall InternalConstruct005daf50(void* self, void* unused_edx, void* system);

} // namespace kotor_sound_rev3

extern "C" {

// Selected _live_operator_new @ 005FBBC0 (cdecl(size) RET0) and _live_free @
// 005FBA60 (cdecl(ptr) RET0). The free is the original EH cleanup call and is
// the non-throwing deallocation seam; the allocator may throw and is declared
// noexcept(false).
void* __cdecl live_operator_new(std::uint32_t size) noexcept(false);
void __cdecl live_free(void* block) noexcept;

// Selected 0095025C (original 007A39EC): the cell value is the CExoSound*; its
// first dword is the system argument. The original double-dereferences with no
// null guard.
extern void* kotor_cell_sound_007a39ec;

// Selected 00950260 (original 00832894). The original loads a dword. The
// selected owner is a volatile Word in .bss; its upper bytes are zero, so the
// nonzero test is equivalent. Do not create a second cell.
extern volatile std::uint32_t kotor_cell_disable_sound_00832894;

// 005D5870. The selected facade is fastcall(self, unused_edx) @8 RET0. The
// original lower-ctor exception must propagate after the raw block is freed.
void* __fastcall kotor_sound_source_construct_005d5870(void* self, void* unused_edx) noexcept(false);

} // extern "C"
