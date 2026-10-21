// aurora_gui::cells() for the Aurora GUI layer writers (004591B0, 004592B0,
// 004592F0, 00459580; M30 r5 owner-payloads/aurora_menu/layers.cpp, unchanged).
// Every field addresses the sole owner of its original cell:
//  - static cells owned by lane-gui-draw (gui_cells.cpp, gl_enum_cells.cpp);
//  - screenHeight 0078D1D8: existing M40 kotor_menu_window_cells::height_0078d1d8;
//  - screenWidth 0078D1D4: W-5 (lane-m50-vtables) single owner
//    kotor_renderer_gl_cells::width_0078d1d4 (gl-renderer-gl-cells, D45); the
//    Views member is no longer published (functional_bootstrap.cpp checks
//    Global::Width == &kotor_renderer_gl_cells::width_0078d1d4).
// Fails closed (std::terminate, as in the r5 accessor) when no Views is
// published Live; no default width, no partial descriptor, no fallback.
#include "4fe11d5c02-layers.h"
#include "490217376c-live_publication.h"
#include "f96fb2c5c4-window_shared_cells.h"
#include "f84493c265-gui_cells.h"
#include "76a582238c-gl_enum_cells.h"
namespace kotor_renderer_gl_cells { extern std::uint32_t width_0078d1d4; }   // W-5 single owner (gl-renderer-gl-cells, D45; cold 800 = original .data 0078D1D4)
#include <exception>

namespace {
// Address metadata only (not original-address storage). Rewritten on every
// access from the current Live publication; GUI runs on the sole UI thread.
aurora_gui::Cells current{};
}

namespace aurora_gui {
const Cells& cells() noexcept {
    kotor_live_publication::Attachment* const a = kotor_menu_window_attachment;
    if (!a || a->phase != kotor_live_publication::Phase::Live || !a->views)
        std::terminate();
    current.gate_007bb4d0 = &kotor_renderer_cells::busy_007bb4d0;
    current.width_0078d1d4 = &kotor_renderer_gl_cells::width_0078d1d4;
    current.height_0078d1d8 = &kotor_menu_window_cells::height_0078d1d8;
    current.index_007b9460 = &kotor_renderer_cells::viewport_index_007b9460;
    current.frames5_007b9468 = kotor_renderer_cells::viewport_frame_words_007b9468;
    current.alpha_007b9248 = kotor_renderer_cells::viewport_alphas_007b9248;
    current.lighting_0078d45c = &kotor_renderer_cells::disable_0078d45c;
    current.depth_0078d474 = &kotor_renderer_cells::enable_0078d474;
    current.blend_0078d448 = &kotor_renderer_cells::enable_0078d448;
    current.texture_0078d458 = &kotor_renderer_cells::enable_0078d458;
    return current;
}
}
