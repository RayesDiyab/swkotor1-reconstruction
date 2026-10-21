#pragma once
#include <cstdint>

namespace kotor_pbuffer {
using U32 = std::uint32_t;
using I32 = std::int32_t;
using ChoosePixelFormat = I32 (__stdcall*)(void*, const I32*, const float*, U32, I32*, U32*);
using CreatePbuffer = void* (__stdcall*)(void*, I32, I32, I32, const I32*);

// Borrowed storage only. Main owns the descriptor and all three cell lifetimes.
// Extension slots must be the canonical slots populated by the existing loader.
struct Bindings {
    const volatile U32* main_dc_007a47e4;
    ChoosePixelFormat volatile* choose_007bb730;
    CreatePbuffer volatile* create_007bb530;
};
extern const Bindings& bindings;
void* __cdecl CreatePBuffer_00426560(U32 width, U32 height);
}
