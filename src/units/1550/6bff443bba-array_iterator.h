#pragma once
#include <cstdint>

namespace kotor_array_iterator {
using Destroy = void (__fastcall*)(void*, void*);
}

// 006FA852 consumes raw element storage and a 32-bit count/stride pair.
extern "C" void __stdcall c2da_destroy_array_006fa852(
    void*, std::uint32_t, std::uint32_t, kotor_array_iterator::Destroy);
