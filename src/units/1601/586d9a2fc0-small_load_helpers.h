#pragma once

// Lane A small load helpers — exact x86 (MSVC) ABI carriers.
//
// The original bodies at 005EEAB0 and 00677560 take ECX (this/message manager)
// plus three dword stack arguments and return with `ret 0xC`.  The three
// caller dwords are represented here as a standard-layout, trivially copyable
// 12-byte aggregate.  Under MSVC x86 __fastcall the first 4-byte parameter
// (self) uses ECX; the 12-byte aggregate is not a DWORD-or-smaller register
// candidate, so it is passed by value on the stack at [esp+4], [esp+8],
// [esp+0xC].  No parameter remains for EDX.  The callee's cleanup is 12 bytes
// and the extern "C" decoration is @name@16.  This matches the original
// thiscall-shaped contract exactly (see SOURCE-AUDIT.md).

#include <cstddef>

struct LoadGameArgs {
    unsigned arg1;       // [esp+4]   original dword argument
    const void* arg2;    // [esp+8]   original CExoString*
    const void* arg3;    // [esp+0xC] original CExoString*
};

#if defined(_M_IX86)
static_assert(sizeof(unsigned) == 4, "x86 dword");
static_assert(sizeof(void*) == 4, "x86 pointer");
static_assert(sizeof(LoadGameArgs) == 12, "three stack dwords");
static_assert(alignof(LoadGameArgs) == 4, "x86 aggregate alignment");
static_assert(offsetof(LoadGameArgs, arg1) == 0, "arg1 stack slot");
static_assert(offsetof(LoadGameArgs, arg2) == 4, "arg2 stack slot");
static_assert(offsetof(LoadGameArgs, arg3) == 8, "arg3 stack slot");
#else
#error Lane A small load helpers are x86-only
#endif

// Expected decorations:
//   @kotor_fastcall_005eeab0@16  (ECX + 12 stack bytes, RET 0xC)
//   @kotor_fastcall_00677560@16  (ECX + 12 stack bytes, RET 0xC)
//   @kotor_fastcall_005f4ae0@4   (ECX only, RET 0)
extern "C" unsigned __fastcall kotor_fastcall_005eeab0(void* self,
                                                       LoadGameArgs args);
extern "C" unsigned __fastcall kotor_fastcall_00677560(void* self,
                                                       LoadGameArgs args);
extern "C" void __fastcall kotor_fastcall_005f4ae0(void*);
