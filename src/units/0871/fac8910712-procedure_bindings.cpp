// lane-m48-a: the single definition of font_txi::procedure_bindings (consumer lane-font txi_parse.cpp 00422390,
// 'proceduretype' life/perlin/random/ringtexdistort paths 00422453/004224A0/004225BC/00422611), bound to the
// genuine controller tables owned by lane-texture-epoch (controller_life_random_ring.cpp / controller_perlin_arturo.cpp):
//   0073ED54 Life   (txi_parse stores it at 0042248B mov [esi],73ED54h)
//   0073ED70 Perlin (004224D8)       0073ED90 Random (004225F6)       0073EDAC RingTexDistort (0042264B)
// D76: no member is left null. D24: the descriptor's void** members receive the typed tables by object-pointer
// conversion only (no function-pointer cast). Perlin's extended table begins with the shared base table
// (offset 0, pointer-interconvertible), so &perlin_vtable_0073ed70 is the original address 0073ED70.
// Constant-initialized storage (verified: no .CRT$XCU in the object; see PACKET.json).
#include "7cea577bba-font_txi.hpp"
#include "1265433832-texture_controller.hpp"

namespace font_txi {
namespace {
// Object-pointer conversions written inline (address constants): MSVC emits them as static relocations
// (no dynamic initializer), checked with dumpbin /headers (no .CRT$XCU).
const ProcedureBindings procedure_bindings_storage = {
    const_cast<void**>(reinterpret_cast<void* const*>(&texture_controller::life_vtable_0073ed54)),
    const_cast<void**>(reinterpret_cast<void* const*>(&texture_controller::perlin_vtable_0073ed70.base)),
    const_cast<void**>(reinterpret_cast<void* const*>(&texture_controller::random_vtable_0073ed90)),
    const_cast<void**>(reinterpret_cast<void* const*>(&texture_controller::ring_distort_vtable_0073edac)),
};
} // namespace
const ProcedureBindings& procedure_bindings = procedure_bindings_storage;
} // namespace font_txi
