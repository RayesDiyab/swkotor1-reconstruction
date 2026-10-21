#pragma once
// 006925B0 CSWCMapPin::~CSWCMapPin and 00692610 scalar deleting destructor.
//
// Original identities (read-only oracle swkotor.exe SHA-256
// 9C10E0450A6EECA417E036E3CDE7474FED1F0A92AAB018446D156944DEA91435):
//   006925B0..00692605, 0x55 body bytes, sole RET 0 at 00692604.
//     __thiscall ECX=this, no stack arguments, RET0, EH frame handler 007301F8,
//     FuncInfo 00780634, state-0 unwind action 007301F0 -> 0063E800 (~CSWCObject).
//     Sequence: store 00754598 at +0, set EH state 0, destroy CExoString at +0x100
//     via 005E5C20, set EH state -1, call 0063E800.
//   00692610..0069262E, 0x1E body bytes, sole RET 4 at 0069262B.
//     __thiscall ECX=this, one stack delete flag, RET4, EAX=this.
//     Sequence: call 006925B0(this); testb $1, [esp+8]; if set, push this and call
//     006FA390 (free); add esp,4; mov eax,esi; pop esi; ret 4.
//
// Project ABI: the zero-stack thiscall destructor is
//   __fastcall(self, unused_edx) -> @8, RET0.
// The scalar deleting destructor has one stack dword:
//   __fastcall(self, unused_edx, delete_flag) -> @12, RET4.
//
// The two C providers below are declared noexcept(false) because the original
// unwind action must be able to run when the CExoString destructor throws; this
// is an explicit exception declaration, not a blanket /EHsc assumption.
#include <cstdint>
#include "848078071a-map_pin_ctor.h"

extern "C" {

// 005E5C20 CExoString::~CExoString, selected real provider
// @frame_LogStringDtor_005e5c20@8 at 0064AF00 [0459-r4-lowers-r4-log-joins.obj].
// ECX = CExoString*, no stack arguments, RET0.
void __fastcall frame_LogStringDtor_005e5c20(void* self, void* unused_edx) noexcept(false);

// 006FA390 free, selected canonical live-free owner
// __live_free / _live_free at 005FBA60 [0004-live-policy.obj].
// cdecl, one pointer argument, RET0 (caller cleans); null-tolerant.
void live_free(void* pointer) noexcept;

// 0063E800 CSWCObject::~CSWCObject is the selected real provider
// @kotor_m15e_object_destruct_0063e800@8 at 00545DE0 [0831.obj]; its
// declaration is in map_pin_ctor.h and is reused unchanged.

// 006925B0 CSWCMapPin::~CSWCMapPin, projected to the project __fastcall ABI.
void __fastcall recovered_client_map_pin_destruct_006925b0(
    void* self, void* unused_edx) noexcept(false);

// 00692610 CSWCMapPin scalar deleting destructor, projected to the project
// __fastcall ABI with one stack dword.  Returns the original `this`.
void* __fastcall recovered_client_map_pin_scalar_delete_00692610(
    void* self, void* unused_edx, std::uint32_t delete_flag) noexcept(false);

}  // extern "C"
