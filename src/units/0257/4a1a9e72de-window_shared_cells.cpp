#include "bbc22d46e2-window_shared_cells.h"
#include "6708d64a32-global_aliases.h"

namespace kotor_menu_window_cells {
// Original pinned PE cold data: seven zero DWORDs and height 600.
// Static module lifetime; no object/heap creation or dynamic initializer.
void* volatile manager_007a39fc = nullptr;
std::uint32_t volatile reinit_007a3a2c = 0;
std::uint32_t volatile height_0078d1d8 = 600;
HWND volatile render_007a39d8 = nullptr;
HWND volatile top_007a39d4 = nullptr;
HINSTANCE volatile instance_007a39c0 = nullptr;
void* volatile aurora_007a39f8 = nullptr;
void* volatile input_007a39e4 = nullptr;

static const kotor_menu_aliases::Cells8 owned_descriptor = {
    &manager_007a39fc, &reinit_007a3a2c, &height_0078d1d8,
    &render_007a39d8, &top_007a39d4, &instance_007a39c0,
    &aurora_007a39f8, &input_007a39e4
};
}

// This publishes storage addresses only. The live window Context/frame/provider
// Publication remains undefined until its separate identity/lifetime gates pass.
kotor_menu_aliases::Cells8 const* const kotor_menu_window_shared_cells =
    &kotor_menu_window_cells::owned_descriptor;

static_assert(sizeof(HWND) == 4 && sizeof(HINSTANCE) == 4, "native x86 cells");
