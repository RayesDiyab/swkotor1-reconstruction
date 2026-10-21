#include "06b9b092fa-async_read.h"
#include <cstring>
#include <type_traits>

extern "C" void __fastcall c2da_file_005e6980(
    void* outer, void*, void* buffer, std::uint32_t size, std::uint32_t count) {
    void* internal;
    std::memcpy(&internal, outer, sizeof(internal));
    c2da_file_internal_005ebbf0(internal, nullptr, buffer, size, count);
}

using AsyncReadTransport=void(__fastcall*)(void*,void*,void*,std::uint32_t,std::uint32_t);
static_assert(std::is_same<decltype(&c2da_file_005e6980),AsyncReadTransport>::value,
              "ECX receiver; three stack words; no return-value contract");
static_assert(sizeof(void*)==4 && sizeof(std::uint32_t)==4,
              "original x86 widths; exactly four outer bytes are read");
