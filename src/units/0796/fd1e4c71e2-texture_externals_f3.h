#pragma once
// Texture-family dependencies of 004233E0 (owned by the texture lanes; NOT defined here).
// Opaque receivers: only the vtable word at +0 is touched by F3 code.
#include <cstdint>
#include "ccf5cb551c-render_shared_lowers.h"
extern "C" {
// 00420AC0 AurTextureFind: cdecl (const char*,const char*,const U*,I) -> CAurTexture* (lane-texture-epoch texture.hpp spelling).
void* __cdecl find_00420ac0(const char*, const char*, const std::uint32_t*, std::int32_t);
// 00423150 CAurTextureBasic ctor: thiscall(ECX this, two stack words, RET 8) as D1 fastcall(this, unused EDX, ...).
void* __fastcall basic_ctor_00423150(void*, void*, const char*, const char*);
// 00420C20 CAurTextureBasic::AsNormalizeVectorCubeMap: thiscall, zero stack words (004233E0 caller pushes none).
void __fastcall basic_as_normalize_vector_cube_map_00420c20(void*, void*);
}
namespace texture {
extern "C" {
// No [vtable+04] carrier exists in lane-texture-epoch texture_x86.asm; fork F3 owns this one
// (fork-f3/src/texture_carrier_slot_04.cpp, D21 naked inline asm). Receiver is the AurTextureFind result (any CAurTexture
// table); EAX pointer result is the receiver later bound via [vtable+B4] (0047AA9E..0047AAA2).
Basic* __fastcall texture_call_slot_04(void*);
}
}
namespace kotor_menu_10_19 { namespace dependencies {
// 006FA7E6 allocator (M41 export-joins.obj alias); paired free on unwind is 006FA390.
void* __cdecl allocate_006fa7e6(unsigned int);
}}
