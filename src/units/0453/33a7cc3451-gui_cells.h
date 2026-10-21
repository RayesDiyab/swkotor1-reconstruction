#pragma once
// Sole owner declarations for the runtime/readonly cells consumed by the GUI
// draw descriptors (gui_wrap_draw::bindings, gui_viewport_list::viewport).
// Names and exact types are reused from the existing Main6 M30 canonical cell
// lineage (task-6/m30-canonical-renderer-cells-r1: canonical_cells.h R1 and
// r4b viewport_array_cells.h); this header adds no new nominal type.
// Original-image evidence for every cell: lane-gui-draw/NOTES.md.
#include <cstdint>
// Existing Main6 r4b declarations: viewport_capacity=10,
// float viewport_alphas_007b9248[10], uint16 viewport_frame_words_007b9468[50],
// short viewport_index_007b9460.
#include "7472d6f270-viewport_array_cells.h"

namespace kotor_renderer_cells {
// Runtime cells (exact M30 R1 canonical_cells.h types).
extern std::uint32_t pulse_0078d3d4;          // 0078D3D4 pulsingAlpha
extern std::uint32_t busy_007bb4d0;           // 007BB4D0 noGUIRender
extern std::uint32_t disable_0078d45c;        // 0078D45C GL_LIGHTING
extern std::uint32_t volatile reset_007a689c; // 007A689C enablePausePlane
// Readonly .rdata literals (exact M30 R1 / Task17 projection types).
extern const volatile float hundred_00741838;
extern const volatile float quarter_0073edf0;
extern const volatile float negative_hundred_0074186c;
extern const volatile float epsilon_00741870;
extern const volatile float zero_0073d700;
extern const volatile float one_0073d764;
extern const volatile float channel_0073ede4;
extern const volatile std::uint32_t quads_0073f34c;
}
