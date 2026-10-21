#pragma once
#include <cstddef>
#include <cstdint>
#include <type_traits>
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error This bounded reconstruction requires MSVC Windows x86.
#endif

namespace kotor_reconstruction {
// Consumed prefixes only. No complete input class or live device is represented.
struct InputInternalView {
    unsigned char opaque00[0x378];
    unsigned char mouse_mode;
    unsigned char opaque379[0x27];
    unsigned long delta_x;
    unsigned long delta_y;
    unsigned long capture_keyboard_keys;
    unsigned long last_mouse_down;
    __declspec(noinline) void __thiscall GetMouseDelta(unsigned long* x, unsigned long* y);
};
struct InputView {
    unsigned char opaque00[4];
    InputInternalView* internal;
    __declspec(noinline) void __thiscall GetMouseDelta(unsigned long* x, unsigned long* y);
    __declspec(noinline) unsigned char __thiscall GetMouseMode();
    __declspec(noinline) void __thiscall SetCaptureKeyboardKeys(unsigned long value);
    __declspec(noinline) unsigned long __thiscall GetLastSetMouseDownState();
};
static_assert(sizeof(void*) == 4 && sizeof(unsigned long) == 4, "x86 widths");
static_assert(std::is_standard_layout<InputView>::value && std::is_standard_layout<InputInternalView>::value, "consumed views");
static_assert(offsetof(InputView, internal) == 4 && sizeof(InputView) == 8, "wrapper consumed offset");
static_assert(offsetof(InputInternalView, mouse_mode) == 0x378, "mode byte");
static_assert(offsetof(InputInternalView, delta_x) == 0x3a0, "delta x raw32");
static_assert(offsetof(InputInternalView, delta_y) == 0x3a4, "delta y raw32");
static_assert(offsetof(InputInternalView, capture_keyboard_keys) == 0x3a8, "capture raw32");
static_assert(offsetof(InputInternalView, last_mouse_down) == 0x3ac && sizeof(InputInternalView) == 0x3b0, "last-down raw32");
}
