#pragma once
#include <windows.h>
#include <cstdint>

// Sole task6 storage definitions. Consumers borrow these actual typed cells;
// a Word pointer must not be cast to and dereferenced as a pointer-object slot.
// Cell32 performs the separately reviewed native DWORD view for word consumers.
namespace kotor_menu_window_cells {
extern void* volatile manager_007a39fc;
extern std::uint32_t volatile reinit_007a3a2c;
extern std::uint32_t volatile height_0078d1d8;
extern HWND volatile render_007a39d8;
extern HWND volatile top_007a39d4;
extern HINSTANCE volatile instance_007a39c0;
extern void* volatile aurora_007a39f8;
extern void* volatile input_007a39e4;
}
