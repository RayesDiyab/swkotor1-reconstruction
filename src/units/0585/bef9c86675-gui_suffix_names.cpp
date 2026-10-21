// lane-gui-load rev2: CExoString[5] at 007A3D50 (full-screen BG panel suffixes, read by
// CSWGuiPanel::GetFullScreenBG at 0040A941 and the GUI-manager ctor bindings) and its
// original CRT static initializer staticInitGUINames1 0073AD80 / exit callback
// staticDisposeGUINames1 0073B770. Pattern of lane-resman texture_pack_storage.cpp.
#include "c534281817-startup_string.h"
#include <cstdlib>
#include <new>

extern "C" {
// 007A3D50..007A3D77: 5 x 8-byte CExoString, .bss loader zero-fill in the original image.
alignas(4) unsigned char kotor_cells_gui_suffix_007a3d50[40] = {};
__declspec(noinline) int __cdecl gui_load_init_gui_names_0073ad80();
__declspec(noinline) void __cdecl gui_load_dispose_gui_names_0073b770();
}

namespace {
kotor_reconstruction::StartupString* slot(unsigned index) {
    return reinterpret_cast<kotor_reconstruction::StartupString*>(kotor_cells_gui_suffix_007a3d50 + index * 8u);
}
}

// 0073B770: eh_vector_destructor_iterator(007A3D50, 8, 5, 005E5C20) -> elements 4..0.
extern "C" __declspec(noinline) void __cdecl gui_load_dispose_gui_names_0073b770() {
    for (unsigned index = 5; index-- > 0;) slot(index)->~StartupString();
}

// 0073AD80: five CExoString(char const*) 005E5A90 constructions in address order from
// 0073E0D0 "back", 0073E0C8 "store", 0073E0C0 "pazaak", 0073E0BC "map", 0073E0B4 "comp"
// (0073AD80..0073ADC6), then _atexit(0073B770) (006FB332 at 0073ADD0); EAX = atexit result.
extern "C" __declspec(noinline) int __cdecl gui_load_init_gui_names_0073ad80() {
    ::new (slot(0)) kotor_reconstruction::StartupString("back");
    ::new (slot(1)) kotor_reconstruction::StartupString("store");
    ::new (slot(2)) kotor_reconstruction::StartupString("pazaak");
    ::new (slot(3)) kotor_reconstruction::StartupString("map");
    ::new (slot(4)) kotor_reconstruction::StartupString("comp");
    return std::atexit(&gui_load_dispose_gui_names_0073b770);
}

// The original runs 0073AD80 from the CRT _initterm table; a C++ dynamic initializer is
// the modern CRT's _initterm (.CRT$XCU) entry for the same role. Named namespace so the
// initializer symbol is path-independent (lane-audit-r7 allowlist).
namespace kotor_gui_names {
extern const int gui_names_root_0073ad80;
const int gui_names_root_0073ad80 = gui_load_init_gui_names_0073ad80();
}
