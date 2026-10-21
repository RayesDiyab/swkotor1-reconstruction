#pragma once
// R4 sound/corpse batch: shared byte-access helpers and ABI conventions.
//
// Original: GOG KOTOR 1 Windows x86 swkotor.exe (image base 0x00400000),
// SHA-256 9C10E0450A6EECA417E036E3CDE7474FED1F0A92AAB018446D156944DEA91435.
//
// Every original __thiscall entry is reconstructed as an extern "C" __fastcall
// veneer: ECX = receiver, EDX = unused dummy, remaining arguments on the stack.
// The selected providers use the same convention and the same callee cleanup.
//
// The helpers use uintptr_t arithmetic plus memcpy so a null or malformed
// receiver still reaches the original faulting access; they do not turn the
// access into a C++ pointer-arithmetic assumption and they never replace an
// original dereference with a guard.
#if !defined(_M_IX86)
#error "R4 sound/corpse reconstruction is x86-only"
#endif

#include <cstdint>
#include <cstring>

namespace r4_sound_corpse {

using U8 = std::uint8_t;
using U32 = std::uint32_t;
using U = std::uintptr_t;

inline const U8* at(const void* base, U offset) noexcept {
    return reinterpret_cast<const U8*>(reinterpret_cast<U>(base) + offset);
}

inline U8* at(void* base, U offset) noexcept {
    return reinterpret_cast<U8*>(reinterpret_cast<U>(base) + offset);
}

inline U8 load_u8(const void* base, U offset) noexcept {
    U8 value;
    std::memcpy(&value, at(base, offset), sizeof value);
    return value;
}

inline U32 load_u32(const void* base, U offset) noexcept {
    U32 value;
    std::memcpy(&value, at(base, offset), sizeof value);
    return value;
}

inline void* load_ptr(const void* base, U offset) noexcept {
    void* value;
    std::memcpy(&value, at(base, offset), sizeof value);
    return value;
}

inline void store_u8(void* base, U offset, U8 value) noexcept {
    std::memcpy(at(base, offset), &value, sizeof value);
}

inline void store_u32(void* base, U offset, U32 value) noexcept {
    std::memcpy(at(base, offset), &value, sizeof value);
}

inline void store_ptr(void* base, U offset, const void* value) noexcept {
    std::memcpy(at(base, offset), &value, sizeof value);
}

} // namespace r4_sound_corpse
