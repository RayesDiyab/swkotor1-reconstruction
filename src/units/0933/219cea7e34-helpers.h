#pragma once
#include <stdint.h>
#include <Windows.h>
#include <type_traits>
// Exact installed Win32 declaration, authenticated original kernel32 IAT export.
using Task8RaiseException = void (WINAPI*)(DWORD,DWORD,DWORD,const ULONG_PTR*);
static_assert(sizeof(void*)==4 && sizeof(DWORD)==4 && sizeof(ULONG_PTR)==4,"x86 only");
static_assert(std::is_same<decltype(&::RaiseException),Task8RaiseException>::value,"RaiseException ABI");
extern "C" Task8RaiseException task8_raiseexception_iat; // alias only; no storage definition
extern "C" int32_t __cdecl task8_math_port_00707e0a();
extern "C" int32_t __cdecl task8_math_port_00707e17();
// Assembly-call contract:6 caller-cleaned words; record storage at least0x64 bytes.
// The original caller ignores EAX; after resumed exception, EAX holds current record pointer.
#include "6b9d4e525c-pow_raise_port.h"
