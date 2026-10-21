#pragma once

// ABI mirror of the selected-image providers called by 00647C10.
//
// Every declaration below must produce exactly the decorated symbol shown in
// the selected map (root-normal-winmain-20261019T1250Z native-v1 kotor.map):
//
//   @kotor_m15b_portrait_construct_00678ec0@8   004873D0  0820-m15b-rules-leaves.obj
//   @ref_assign_copy_00406120@16                00614BA0  0278-addkey-assign-00406120.obj
//   @kotor_m15e_portrait_get_portrait_00678ef0@16 00544B00 0831.obj
//   ?exists_00408bc0@Manager@menu_resources@@QAEIPAURef@2@IPAI@Z 005FE370 0031-service-resource-14.obj
//   ?BorrowCanonicalManager007a39e8@menu_resources@@YAPAUManager@1@XZ 0060D820 0150-manager-cell-borrow.obj
//   @unresolved_ref_assign_00406290@16          00605470  0081-resource-ref-assignment.obj
//   @kotor_fastcall_004015a0@8                  006358D0  0399-routes-shutdown.obj
//
// This header declares the minimal menu_resources types needed for the exact
// C++ name mangling.  Do not co-include it with the canonical menu_resources
// public header in one translation unit: that header defines the same class
// names with the full member set.  The reconstruction TU is self-contained.
//
// Ordinary typed forwarding is intentional: ECX/EDX carry the receiver and an
// unused dummy, stack arguments stay in original order, and each provider's
// own RET count (RET8/RET0/RET12) removes its outgoing arguments.  The cdecl
// target removes nothing (RET0); its callers remove the two incoming dwords.
//
// Exception contract: the constructor seam and GetPortrait are declared
// noexcept(false).  The host fixture deliberately models a failed construction,
// and GetPortrait's selected lower-construct path is a potentially throwing C++
// path with an EH frame.  Without noexcept(false), an extern "C" declaration
// inherits the compiler's C-function nonthrow assumption and would forbid or
// drop that propagation.  ref_assign_copy and the literal assignment are
// proven nonthrowing selected bodies, and 004015A0 is the deliberate no-op
// destructor, so they keep the C-linkage nonthrow domain.  The C++ Manager
// method and borrow accessor are unaffected by the C-linkage assumption.

#include <cstdint>

namespace menu_resources {

using Word = std::uint32_t;

struct Ref {
    std::uint8_t bytes[16];
};

struct Manager {
    Word __thiscall exists_00408bc0(Ref* resource, Word type, Word* output);
};

Manager* __cdecl BorrowCanonicalManager007a39e8();

static_assert(sizeof(Ref) == 16, "original CResRef storage is 16 bytes");
static_assert(sizeof(Word) == 4, "original 32-bit word");
static_assert(sizeof(void*) == 4, "32-bit x86 ABI");

}  // namespace menu_resources

extern "C" {

// 00678EC0 CSWPortrait::CSWPortrait(), ECX=this, RET0, returns this.
// noexcept(false): construction can fail and the failure must propagate.
void* __fastcall kotor_m15b_portrait_construct_00678ec0(
    void* self, void* unused_edx) noexcept(false);

// 00406120 CResRef copy: ECX=dest, [esp+4]=out, [esp+8]=source, RET8.
// Writes a lowercased 16-byte copy of source into dest (all 16 bytes are
// converted through the selected 006FB766 lower helper, without stopping at
// NUL), copies dest to out, and returns out.
void* __fastcall ref_assign_copy_00406120(
    void* self, void* unused_edx, void* out, const void* source);

// 00678EF0 CSWPortrait::GetPortrait(out, BYTE size), ECX=this, RET8.
// Appends the selected size suffix (1..5 -> T,S,M,L,H), lowercases the
// concatenation through the selected lower_construct path, and writes the
// result to out; writes the empty resref when this is empty.
// noexcept(false): the selected lower-construct path has an EH frame and must
// not lose cleanup solely because this seam has C linkage.
void* __fastcall kotor_m15e_portrait_get_portrait_00678ef0(
    void* self, void* unused_edx, void* out, unsigned int size) noexcept(false);

// 00406290 CResRef literal assignment: ECX=dest, [esp+4]=out, [esp+8]=text,
// RET8.  Zeroes dest, lowercases text (up to 16 bytes / first NUL), copies the
// 16-byte dest to out, returns out.
menu_resources::Ref* __fastcall unresolved_ref_assign_00406290(
    menu_resources::Ref* self, void* unused_edx, menu_resources::Ref* out,
    const char* text);

// 004015A0 CSWPortrait/CResRef destructor: ECX=this, RET0.  The original body
// is a single ret; the selected alias is the same deliberate no-op.
void __fastcall kotor_fastcall_004015a0(void* self, void* unused_edx);

}  // extern "C"
