#pragma once
#include <cstdint>

// Physical transport only: the receiver provides an initialized, readable
// four-byte internal pointer at offset zero. No larger nominal File is claimed.
// EDX is unused; buffer, size and count occupy the three callee-popped words.
extern "C" __declspec(noinline) void __fastcall c2da_file_005e6980(
    void* outer, void* unused_edx, void* buffer,
    std::uint32_t size, std::uint32_t count);

// Genuine unresolved lower owner. This declaration is not a callback fixture
// or a synchronous-read substitution. Main/File must select its implementation.
extern "C" void __fastcall c2da_file_internal_005ebbf0(
    void* internal, void* unused_edx, void* buffer,
    std::uint32_t size, std::uint32_t count);
