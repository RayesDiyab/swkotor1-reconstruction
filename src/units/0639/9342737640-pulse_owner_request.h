#pragma once
#include "82332a19d6-canonical_views.h"
#include <type_traits>

// Proposed declaration-only Root-owned aliases. This packet defines no state,
// readonly data, reference backing objects, initialization or GUI provider table.
// producer_bits and the existing Task17 readonly pulse reference MUST borrow
// one and the same live unsigned-int object, with no float/Word lvalue alias.
namespace kotor_pulse_canonical_required {
extern volatile float& timer_0078d3d0;
extern volatile unsigned int& producer_bits_0078d3d4;
extern volatile unsigned int& direction_007a4578;
extern const volatile float& factor_0073e318;
extern const volatile float& period_0073e31c;
extern const volatile float& low_0073e320;
}

// Physical observed boundary: no receiver, one binary32 stack word, caller
// cleanup (RET0). Only consumed void is proposed; EAX return credit is absent.
extern "C" __declspec(noinline) void __cdecl
GuiPulseUpdate00414a50(float elapsed) noexcept;
using GuiPulseNative = void (__cdecl*)(float) noexcept;
static_assert(std::is_same<decltype(&GuiPulseUpdate00414a50),
                           GuiPulseNative>::value,"native cdecl consumed-void");
static_assert(sizeof(float)==4 && sizeof(unsigned int)==4 && sizeof(void*)==4,
              "reviewed MSVC x86 binary32/WORD pointer widths");
static_assert(std::is_same<decltype(kotor_gui_canonical_required::pulse_0078d3d4),
                           const volatile unsigned int&>::value,
              "actual existing raw DWORD consumer identity");
