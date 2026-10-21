// m46_emitter_cells_view.cpp - lane-m46-3dmodel: the single kotor_emitter::emitter_cells instance (emitter.h:111-116).
// Pointers to the owners' storage (never copies); statically initialised (addresses of external objects; the object-
// pointer conversion below is folded by cl into the .rdata relocation - verified: no .CRT$XCU / dynamic initializer).
//   0078E614 render gate (.data 1)   -> kotor_vp_cells::gate_0078e614 (m46_orphan_cells.cpp; D54 assigns mesh gates to the
//                                       lane-3d-vp cell area; no owner existed in M45 / lane objects / lane sources)
//   007FBF78 emitter bucket {data,size,capacity} (BSS) -> m46_orphan_cells.cpp emitter_bucket_007fbf78 (writers 0046D5F4 /
//                                       0046D6F1 ProcessParts, 0046BF12 ClearBuckets; atexit free 0073BAB0)
//   00827FB0 triangle statistic (BSS) -> r4_lowers_cells::render_counts_00827fa8[2] (M44/M45 r4-lowers-r4-cells owns
//                                       00827FA8..00827FBC as one 6-word array; 0046EE0C clears 00827FB0 through it).
//                                       NOTE: M45 also stages lane-3d-fb _aurora_triangle_count_00827fb0 = a SECOND storage
//                                       object for 00827FB0 (C3-class conflict; earliest owner r4 wins per D68 rule).
//                                       Element type is unsigned (r4) vs the view's I: object-pointer conversion (D24).
// Read sites: render_func.cpp 00450A26..00450A2D / 00450A40 (bucket), render_linked.cpp 00496DA1..00496DA7,
// render_standard.cpp (triangles).
#include "4ac9e30747-emitter.h"

extern "C" {
extern kotor_scene3d::PointerList emitter_bucket_007fbf78;         // _emitter_bucket_007fbf78 (m46_orphan_cells.cpp)
}
namespace kotor_vp_cells {
extern volatile std::uint32_t gate_0078e614;                       // ?gate_0078e614@kotor_vp_cells@@3IC (m46_orphan_cells.cpp)
}
namespace r4_lowers_cells {
extern std::uint32_t volatile render_counts_00827fa8[6];           // ?render_counts_00827fa8@r4_lowers_cells@@3RCIC (r4_lowers.h:61)
}

namespace kotor_emitter {
namespace {
const EmitterCells emitter_cells_storage = {
    &kotor_vp_cells::gate_0078e614,
    &emitter_bucket_007fbf78,
    reinterpret_cast<volatile I*>(&r4_lowers_cells::render_counts_00827fa8[2]),   // 00827FB0
};
}
extern const EmitterCells& emitter_cells = emitter_cells_storage;   // ?emitter_cells@kotor_emitter@@3ABUEmitterCells@1@B
} // namespace kotor_emitter
