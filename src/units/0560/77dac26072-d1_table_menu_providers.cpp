// Sole definition of menu_providers_40_49::providers (D1 header epoch, d1/ diff).
// Constant-initialised typed data: every initializer is a function or object
// address of exactly the slot type; no function-pointer cast.
#include "8ba4a4693e-providers.h"
#include "9ea344dff0-d1_bodies.h"
#include "369787b919-window_shared_cells.h"
namespace kotor_renderer_init { int __cdecl SetAntiAliasingMode_0044f2f0(int); } // lane-renderer-bodies owner (aa_probe.cpp)
namespace kotor_menu_window_cells { extern void* volatile gui_manager_007a39f4; } // REQUIRED canonical cell (Main; lane-input proposes storage)
namespace menu_providers_40_49 {
static_assert(sizeof(Providers)==40,"10 DWORD slots");
Providers providers={
    &menu_tables_m41::ClientListConstruct_004b25d0,         // +00 004B25D0
    &kotor_renderer_init::SetAntiAliasingMode_0044f2f0,     // +04 0044F2F0 (INI-conditional)
    &menu_tables_required::SetAnisotropy_0044f2c0,          // +08 0044F2C0 REQUIRED (INI-conditional)
    &menu_tables_required::SetVSyncState_0044ee90,          // +0C 0044EE90 REQUIRED (INI-conditional)
    &menu_tables_required::UpdateAllFonts_0040b420,         // +10 0040B420 REQUIRED (not executed before first frame)
    &menu_tables_m41::SetDefaultKeyboardCamera_0061d9e0,    // +14 0061D9E0
    &menu_tables_m41::InputSetMouseCursor_005df5c0,         // +18 005DF5C0
    &menu_tables_m41::GuiSetSoftwareMouseCursor_0040a270,   // +1C 0040A270
    &kotor_menu_window_cells::gui_manager_007a39f4,         // +20 007A39F4
    &kotor_menu_window_cells::input_007a39e4,               // +24 007A39E4 (M41 window-shared-cells)
};
}
