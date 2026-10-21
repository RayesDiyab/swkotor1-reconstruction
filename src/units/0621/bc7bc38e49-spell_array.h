#pragma once
// lane-rules (fork A): CSWSpellArray::Load 0059BA20 and its subtree (swkotor.exe GOG K1,
// sha256 9c10e045..., base 0x400000). D1 form, C++ linkage (original EH frames:
// 0059BA20 -> 007215D0, 0059B310 -> 007211D5, 0059B5B0 -> 00721225).
// CSWSpellArray receiver = 8-byte block allocated at 00552ECA: +0 count, +4 CSWSpell[count].
// CSWSpell element = 0x198 bytes (imul 198h at 0059BA98 / 0059B9F7).
#include "45d2335566-rules_common.h"

namespace kotor_rules {
// 0059BA20 CSWSpellArray::Load(), RET; EAX at exit is not a defined result (last
// callee 005E5C20 at 0059D79D) -> void.
void __fastcall kotor_fastcall_0059ba20(void* self, void* unused_edx);
// 0059B310 CSWSpell::CSWSpell(), RET, EAX=this. Typed as the 006FB55F element ctor.
void* __fastcall kotor_fastcall_0059b310(void* self, void* unused_edx);
// 0059B5B0 CSWSpell::~CSWSpell(), RET. Typed as the 006FB55F element dtor.
void __fastcall kotor_fastcall_0059b5b0(void* self, void* unused_edx);
// D24 typed view of 0059B5B0 for 006FA852's callback slot (c2da_current::StringDestroy);
// same body (forwards to kotor_fastcall_0059b5b0), no function-pointer cast.
void __fastcall kotor_fastcall_0059b5b0_array_destroy(kotor_reconstruction::StartupString* self, void* unused_edx);
// 0059B700 CSWSpell::SetPrerequisites(CExoString*), RET 4.
void __fastcall kotor_fastcall_0059b700(void* self, void* unused_edx, kotor_reconstruction::StartupString* text);
// 0059B7D0 CSWSpell::SetMasterSpells(CExoString*), RET 4.
void __fastcall kotor_fastcall_0059b7d0(void* self, void* unused_edx, kotor_reconstruction::StartupString* text);
// 0059B9E0 CSWSpellArray::~CSWSpellArray(), RET (shutdown only: ~CSWRules 005514A2).
void __fastcall kotor_fastcall_0059b9e0(void* self, void* unused_edx);
}
