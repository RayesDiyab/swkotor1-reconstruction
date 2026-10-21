#pragma once
#include <cstdint>

// Physical x86 entry contracts. ECX is the borrowed original receiver; EDX is
// ignored. The sole stack DWORD is the original float's unchanged bit pattern.
// Each entry returns with RET 4. These declarations create no receiver lifetime,
// shared type, storage, vtable or original-address dispatch table.
extern "C" void __fastcall kotor_panel_extent_0040AA00(
    void* receiver, void* ignored_edx, void* out_extent);
extern "C" void __fastcall kotor_panel_draw_0040B760(
    void* receiver, void* ignored_edx, std::uint32_t delta_bits);
extern "C" void __fastcall kotor_main_menu_draw_0067AC10(
    void* receiver, void* ignored_edx, std::uint32_t delta_bits);

// Genuine provider exports to be joined by their existing Aurora owner. Their
// absence deliberately leaves the production object unresolved. No definitions
// or fallback bindings are supplied here. Unobserved returns are discarded.
extern "C" void __stdcall kotor_AurGUIStartLayer_004591B0();
extern "C" void __stdcall kotor_AurGUIStopLayer_004592B0();
extern "C" std::uint32_t __cdecl kotor_AurGUISetupViewport_004592F0(
    std::uint32_t x, std::uint32_t y, std::uint32_t width,
    std::uint32_t height, void* color, std::uint32_t depth_test_zero,
    std::uint32_t alpha_bits);
extern "C" void __stdcall kotor_AurGUICloseViewport_00459580();
