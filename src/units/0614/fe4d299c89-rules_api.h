#pragma once
// lane-rules public interface for consumers (lane-prepare CSWRules::CSWRules 00552C50):
// C++-linkage D1 entry points of the CSWRules/2DA subtree + the typed vtable 00748BB4.
#include "f7bace6431-rules_vtable.h"
#include "55aae85039-two_dim_arrays.h"
namespace kotor_rules {
void __fastcall kotor_fastcall_0059ba20(void* self, void* unused_edx);   // CSWSpellArray::Load, RET
void __fastcall kotor_fastcall_005515e0(void* rules, void* unused_edx);  // CSWRules::LoadFeatInfo, RET
void __fastcall kotor_fastcall_005b31d0(void* self, void* unused_edx);   // CSWBaseItemArray::Load, RET
void __fastcall kotor_fastcall_00550590(void* rules, void* unused_edx);  // CSWRules::LoadSkillInfo, RET
void __fastcall kotor_fastcall_0054f2c0(void* rules, void* unused_edx);  // CSWRules::LoadClassInfo, RET
void __fastcall kotor_fastcall_0054fec0(void* rules, void* unused_edx);  // CSWRules::LoadRaceInfo, RET
}
