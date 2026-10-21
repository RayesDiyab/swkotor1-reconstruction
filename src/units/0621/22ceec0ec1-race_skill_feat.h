#pragma once
// lane-rules fork D: CSWRules::LoadRaceInfo / LoadSkillInfo / LoadFeatInfo and the
// CSWRace / CSWSkill / CSWFeat element methods they reach (swkotor.exe 9c10e045...).
// D1 form (__thiscall -> __fastcall(self, unused EDX, stack args)); C++ linkage so
// RAII unwinds of the original EH states are emitted (D30-FINAL).
#include "45d2335566-rules_common.h"

extern "C" {
// M41 window-native-strings.obj (modern-ini-support/native_strings.h, identical): 005E5670 CExoString::CStr.
char* __fastcall WindowCStr005e5670(kotor_reconstruction::StartupString*, void*);
}

namespace kotor_rules {
// ---- CSWRules loaders (consumer: lane-prepare 00552C50 at 00552F02/00552FEB/00552FF9) ----
void __fastcall kotor_fastcall_0054fec0(void* rules, void* unused_edx);   // CSWRules::LoadRaceInfo, RET
void __fastcall kotor_fastcall_00550590(void* rules, void* unused_edx);   // CSWRules::LoadSkillInfo, RET
void __fastcall kotor_fastcall_005515e0(void* rules, void* unused_edx);   // CSWRules::LoadFeatInfo, RET
// ---- element methods (eh-vector callbacks: exact crt_eh_vector_constructor_006fb55f types) ----
void* __fastcall kotor_fastcall_005c7890(void* self, void* unused_edx);   // CSWRace::CSWRace (0x34), EAX=this
void  __fastcall kotor_fastcall_005c78d0(void* self, void* unused_edx);   // CSWRace::~CSWRace
void  __fastcall kotor_fastcall_005c7ad0(void* self, void* unused_edx,
                                         kotor_reconstruction::StartupString* table_name); // CSWRace::LoadFeatsTable, RET4
void* __fastcall kotor_fastcall_005cd700(void* self, void* unused_edx);   // CSWFeat::CSWFeat (0x48), EAX=this
void* __fastcall kotor_fastcall_005cd8a0(void* self, void* unused_edx);   // CSWSkill::CSWSkill (0x20), EAX=this
void  __fastcall kotor_fastcall_0059d7c0(void* self, void* unused_edx);   // 0059D7C0 (CSWSkill element dtor: ~CExoString at +10h)
// 004015A0 is a bare RET (CSWFeat element dtor at 0055168A). Its M41 owner is
// `extern "C" void __cdecl renderer_void_004015a0()` (aurora-peer-void.obj), whose type
// differs from the iterator's destroy type; per D24 this is the typed equivalent
// (no new address credit, no function-pointer cast).
void  __fastcall kotor_fastcall_004015a0(void* self, void* unused_edx);
}
