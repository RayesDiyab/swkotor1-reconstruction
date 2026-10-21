#pragma once
#include <cstdint>

// Native ABI facade only: reuses selected w2::g07::active_property_00553960
// with the existing live w2::native::Mem implementation.
extern "C" void* __fastcall kotor_m15_menu_active_property_00553960(
    void* item, void* unused_edx, std::uint32_t index);
