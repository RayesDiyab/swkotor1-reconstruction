#include "014ad7e09d-menu_active_property.h"
#include "518164b261-w2_g07_appearance.hpp"
#include "11fdb9ffa5-w2_native_ports.hpp"

// Native boundary only. The selected g07 owner retains the original algorithm.
extern "C" void* __fastcall kotor_m15_menu_active_property_00553960(
    void* item, void*, std::uint32_t index) {
    w2::native::Mem memory;
    return w2::native::ptr(w2::g07::active_property_00553960(
        memory, w2::native::word(item), index));
}
