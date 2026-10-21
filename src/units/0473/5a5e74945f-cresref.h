#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

#if !defined(_MSC_VER) || !defined(_M_IX86)
#error This bounded reconstruction requires MSVC Windows x86.
#endif

namespace kotor_reconstruction {

// Minimal fixture for the two verified leaf methods, not a recovered class.
// Bytes after an early NUL remain part of the object and are never repaired.
struct CResRef {
    std::uint8_t bytes[16];

    __declspec(noinline) int __thiscall IsValid() const noexcept;
    __declspec(noinline) std::uint8_t __thiscall GetLength() const noexcept;
    __declspec(noinline) void __thiscall CopyToString(char* out) const noexcept;
    __declspec(noinline) int __thiscall operator==(const CResRef* rhs) const noexcept;
    __declspec(noinline) int __thiscall operator!=(const CResRef* rhs) const noexcept;
};

static_assert(std::is_standard_layout<CResRef>::value, "fixture layout");
static_assert(sizeof(CResRef) == 16, "inline sixteen-byte object");
static_assert(offsetof(CResRef, bytes) == 0, "first resource byte at this+0");
static_assert(sizeof(void*) == 4, "x86 ABI");

} // namespace kotor_reconstruction
