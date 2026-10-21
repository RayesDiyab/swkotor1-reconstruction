#pragma once
#include <cstdint>
#include <cstddef>

// Layout and labels from Lane's research, checked against the selected bodies.
// A local behavioral reconstruction; not BioWare's recovered original source.
struct CExoString {
    char* c_string;
    std::uint32_t length;
    __declspec(noinline) int GetLength();
    __declspec(noinline) int IsEmpty();
    __declspec(noinline) char __thiscall operator[](int index);
    __declspec(noinline) int __thiscall Find(char character, int start);
    __declspec(noinline) int operator==(CExoString* compare);
    __declspec(noinline) int operator==(char* compare);
    __declspec(noinline) int operator!=(CExoString* compare);
    __declspec(noinline) int operator!=(char* compare);
    __declspec(noinline) int CompareNoCase(CExoString* compare);
};
static_assert(sizeof(void*) == 4, "Pilot must be built for Windows x86");
static_assert(sizeof(CExoString) == 8, "Win32 research layout");
static_assert(offsetof(CExoString, c_string) == 0, "Observed pointer load");
static_assert(offsetof(CExoString, length) == 4, "Research length layout");
static_assert(static_cast<char>(0x80) < 0, "Reviewed caller promotions require signed plain char; /J rejected");
