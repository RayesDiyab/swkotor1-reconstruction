#pragma once
// lane-rules fork C: CSWRules::LoadClassInfo 0054F2C0 and the CSWClass subtree
// (swkotor.exe GOG K1 sha256 9c10e045..., base 0x400000). D1 form, C++ linkage
// (original bodies carry EH frames; D30-FINAL prefers emitted cleanup).
#include "45d2335566-rules_common.h"

// M41 modern-ini-support/native_strings.h (identical declaration): 005E5670 CExoString::CStr.
extern "C" char* __fastcall WindowCStr005e5670(kotor_reconstruction::StartupString*, void*);

namespace kotor_rules {
// 0054F2C0 CSWRules::LoadClassInfo, RET (EAX not meaningful: last value is the
// restored SEH link, 0054FEA0). ECX = CSWRules (0xDC allocation).
void __fastcall kotor_fastcall_0054f2c0(void* rules, void* unused_edx);
// 00550C00 CSWRules::GetFeat(WORD id), RET 4: EAX = &feats[id] (0x48 stride, +90h) when
// id < WORD[+A4h] and (BYTE[entry+28h] & 10h), else 0.
void* __fastcall kotor_fastcall_00550c00(void* rules, void* unused_edx, std::uint16_t feat);
// 005BCAC0 CSWClass::CSWClass, RET, EAX = this (0x1A0 element; 006FB55F construct callback type).
void* __fastcall kotor_fastcall_005bcac0(void* self, void* unused_edx);
// 005BCBB0 CSWClass::~CSWClass, RET (006FB55F destroy callback type; fork-main adds the
// D24 006FA852-typed adapter).
void __fastcall kotor_fastcall_005bcbb0(void* self, void* unused_edx);
// CSWClass table loaders (ECX = CSWClass element). Arguments are CExoString* (and CSWRules*).
void __fastcall kotor_fastcall_005bcda0(void* self, void* unused_edx, String* table);           // LoadAttackBonusTable, RET 4
void __fastcall kotor_fastcall_005bcf70(void* self, void* unused_edx, String* prefix);          // LoadFeatGain, RET 4
void __fastcall kotor_fastcall_005bd0f0(void* self, void* unused_edx, String* prefix, void* rules); // LoadFeatsTable, RET 8
void __fastcall kotor_fastcall_005bd480(void* self, void* unused_edx, String* table);           // LoadSavingThrowTable, RET 4
void __fastcall kotor_fastcall_005bd6c0(void* self, void* unused_edx, String* prefix);          // LoadSkillsTable, RET 4
void __fastcall kotor_fastcall_005bd900(void* self, void* unused_edx, String* column);          // LoadSpellGainTable, RET 4
void __fastcall kotor_fastcall_005bdbe0(void* self, void* unused_edx, String* table);           // LoadSpellKnownTable, RET 4
void __fastcall kotor_fastcall_005bde80(void* self, void* unused_edx, String* column);          // LoadArmorClassTable, RET 4
void __fastcall kotor_fastcall_005be4c0(void* self, void* unused_edx);                          // LoadSpellsTable, RET
}
