#pragma once
#include "35393c3ac6-exorand.h"
#include "158a6c4f90-startup_string.h"
#include <Windows.h>
namespace startup_rand {
using Rand=kotor_reconstruction::CExoRand;
using Internal=kotor_reconstruction::CExoRandInternal;
struct RandImports {
    BOOL (WINAPI *query_counter)(LARGE_INTEGER*);
    void (WINAPI *system_info)(SYSTEM_INFO*);
    DWORD (WINAPI *tick_count)();
    void (__fastcall *gen_seed)(Internal*,void*);
};
extern const RandImports rand_imports; // one selected real or fixture context
static_assert(sizeof(SYSTEM_INFO)==36 && sizeof(RandImports)==16 &&
    offsetof(RandImports,system_info)==4 && offsetof(RandImports,tick_count)==8,
    "observed x86 API sizes and source-only import transport");
__declspec(noinline) Rand* __fastcall ConstructRand005ea420(Rand*,void*);
__declspec(noinline) Internal* __fastcall ConstructRandInternal005ed0e0(Internal*,void*);
__declspec(noinline) void __fastcall GenSeed005ed080(Internal*,void*);
// Support-only caller alignment; original constructor aligns its own frame8.
__declspec(noinline) void __fastcall AlignedGenSeed(Internal*,void*);
}
