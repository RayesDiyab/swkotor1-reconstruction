#pragma once
// R5 client trigger lookup: original 005F1220 and its thin outer 005ED5D0.
//
// Original: GOG KOTOR 1 Windows x86 swkotor.exe (image base 0x00400000),
// SHA-256 9C10E0450A6EECA417E036E3CDE7474FED1F0A92AAB018446D156944DEA91435.
//
// 005F1220 CClientExoAppInternal::GetTriggerByGameObjectID
//   __thiscall, ECX = CClientExoAppInternal*, one unsigned object id on the
//   stack, `ret 4`, returns CSWCTrigger* or null.
//
// 005ED5D0 CClientExoApp::GetTriggerByGameObjectID
//   __thiscall, ECX = CClientExoApp*, one unsigned object id on the stack,
//   loads the internal at [ECX+4] and tail-jumps to 005F1220, `ret 4`.
//
// Both are projected as __fastcall(self, unused_edx, id): ECX = receiver,
// EDX = the original unused home, one 4-byte stack argument, callee RET4.
// The selected provider `@kotor_fastcall_004d8230@16` is a real body at
// 007162A0 (0457-successor.obj); the slot at object vtable +0x3C is resolved
// dynamically from the receiver's own vtable (selected trigger table 007F9200
// slot +0x3C is the real self-return 00539110).
//
// The callee declarations are explicitly noexcept(false): the lookup and the
// virtual slot are may-throw seams and the original has no catch. `/EHs` must
// be used, not `/EHsc`, so exceptions are not cut off at the extern "C" seam.
#include "43d8ceea0b-r5_client_trigger_abi.h"

extern "C" {

// Selected 007162A0 @kotor_fastcall_004d8230@16 (CGameObjectArray::GetGameObject).
// ECX = object table, EDX = unused, stack = (unsigned id, void** out), RET 8.
// Returns the found/not-found byte: 0 = found, nonzero = not found. The pointer
// result is written through `out` only on the found path.
unsigned char __fastcall kotor_fastcall_004d8230(void* array, void* unused_edx,
                                                 unsigned int id, void** out) noexcept(false);

// Original 005F1220. ECX = CClientExoAppInternal*, EDX = unused, stack = id, RET 4.
// Reads the object table at [internal+0x14], then the object type byte +8, then
// calls the receiver's own vtable slot +0x3C. No null-self guard is added.
void* __fastcall kotor_fastcall_005f1220(void* internal, void* unused_edx,
                                         unsigned int id) noexcept(false);

// Original 005ED5D0. ECX = CClientExoApp*, EDX = unused, stack = id, RET 4.
// Reads [client+4] (the internal) and forwards the same id. No null-self guard.
void* __fastcall kotor_fastcall_005ed5d0(void* client, void* unused_edx,
                                         unsigned int id) noexcept(false);

} // extern "C"
