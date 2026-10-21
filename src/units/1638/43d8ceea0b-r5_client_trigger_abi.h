#pragma once
// R5 client trigger batch: shared byte-access helpers and ABI conventions.
//
// Original: GOG KOTOR 1 Windows x86 swkotor.exe (image base 0x00400000),
// SHA-256 9C10E0450A6EECA417E036E3CDE7474FED1F0A92AAB018446D156944DEA91435.
//
// Every original __thiscall entry is reconstructed as an extern "C" __fastcall
// veneer: ECX = receiver, EDX = unused dummy, remaining arguments on the stack.
// 005ED5D0 and 005F1220 each take one 4-byte stack argument and therefore return
// with the original `ret 4` (callee cleanup), not caller cleanup.
//
// The load helpers use uintptr_t arithmetic plus memcpy so a null or malformed
// receiver still reaches the original faulting access. They never turn an
// original dereference into a guard and never write through the receiver.
//
// Overloads keep reads const-correct: a writable base yields void*, a const base
// yields const void*. The production body only needs the writable overload; the
// tests exercise both.
#if !defined(_M_IX86)
#error "R5 client trigger reconstruction is x86-only"
#endif

#include <cstdint>
#include <cstring>

namespace r5_client_trigger {

using U8 = std::uint8_t;
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

inline const void* load_ptr(const void* base, U offset) noexcept {
    const void* value;
    std::memcpy(&value, at(base, offset), sizeof value);
    return value;
}

inline void* load_ptr(void* base, U offset) noexcept {
    void* value;
    std::memcpy(&value, at(base, offset), sizeof value);
    return value;
}

} // namespace r5_client_trigger
