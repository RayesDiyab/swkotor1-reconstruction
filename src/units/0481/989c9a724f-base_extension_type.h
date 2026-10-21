#pragma once
// lane-resource-lowers: typed declarations for the two Base-extension lowers that
// 00406650 (CExoResMan::GetResTypeFromFile) needs and that no M40 object defines.
#include "9f8b86cc9d-startup_string.h"
#include <cstdint>
// Original 005E6450 (Ghidra CExoString::CompareNoCase): ECX this String, one stack
// String* (RET 4), EAX 1 equal / 0 different. fastcall keeps ECX receiver, unused EDX.
extern "C" __declspec(noinline) std::int32_t __fastcall exostring_compare_no_case_005e6450(
    kotor_reconstruction::StartupString* self, void* unused_edx,
    kotor_reconstruction::StartupString* other);
// Original 005E6670 (CExoBase::GetResTypeFromExtension) = mov ecx,[ecx+14h];
// jmp 005E7A40 (CExoBaseInternal::GetResTypeFromExtension). ECX receiver is the
// actual base_ini_reconstruction::Base (same receiver contract as M40
// c2da_base_005e6660), one stack String* (RET 4). Only AX is produced.
extern "C" __declspec(noinline) std::uint16_t __fastcall c2da_base_005e6670(
    void* receiver, void* unused_edx, kotor_reconstruction::StartupString* extension);
