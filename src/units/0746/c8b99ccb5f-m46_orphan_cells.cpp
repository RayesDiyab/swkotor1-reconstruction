// m46_orphan_cells.cpp - lane-m46-3dmodel: storage for original globals read by the mesh / emitter / bsp areas for which
// NO storage owner exists in the M45 stage objects, in any claude-takeover lane object, or in any lane source
// (lane-m46-3dmodel/symidx + source scan, 2026-10-07). Single-owner PROPOSAL: if another lane later publishes one of
// these addresses, the integrator keeps exactly one (drop here, join the name).
// Values: pinned GOG swkotor.exe sha256 9c10e045... (.data words read with tools/pe.py; BSS = 0).
// Writers: an exe .text scan finds NO store instruction to 0078E608/0078E614/0078E844/0078E8C0 (reads only, listed
// below), and no .data/.rdata word holds any of these addresses (no table-driven writer, e.g. console variables).
// Constant-initialised: no dynamic initializer / .CRT$XCU.
#include <cstdint>
#include "6c81d8874d-scene3d.h"   // kotor_scene3d::PointerList (= kotor_reconstruction::ArrayMutations, 12 bytes)

// Gate words: D54 places the mesh render-path gates in the lane-3d-vp cell area (kotor_vp_cells naming, like the M45
// owners gate_0078e5fc / gate_0078e650 / gate_0078e66c / scale_0078e630).
namespace kotor_vp_cells {
// .data 00000001. Readers: 00450A26 00450AC0 00456364 0046F900 0046FF80 0047040D 00470750 004707A0 00475BBE 004760DE
// 004766E7 004770BD (emitters / bsp tile / trimesh / skin).
extern volatile std::uint32_t gate_0078e614;   // [M46 compose] storage: lane-m46-3dcore scene3d_gate_0078e614 (single storage)
// .data 00000001. Readers: 004708CC 00470E75 00470F07 00470F59 004A7BCE.
volatile std::uint32_t gate_0078e608 = 1u;
// .data 00000001. Reader: 00484290 (PartDanglyMesh::Animate).
volatile std::uint32_t gate_0078e844 = 1u;
// .data 00000001 (D31 vertex-program gate, fixed in .data). Readers: 00425719 0042576A 004257BB 00425852 004258A8
// 00473C58 0047433B 00477DE8 00478E4E 00478E9F 00478EF0 00478F99 00478FE9 0048D6D0.
volatile std::uint32_t skin_programs_0078e8c0 = 1u;
}

extern "C" {
// BSS {data, size, capacity} = 0. Writers: 0046D5F4 / 0046D6F1 (ProcessParts, data after growth), 0046BF12 ClearBuckets
// (size). Original static initializer 0073B010 registers atexit 0073BAB0 (free data if non-null at exit): that
// shutdown-only cleanup is NOT reproduced (recorded deviation, same class as D60 until an allowlisted initializer exists).
extern kotor_scene3d::PointerList emitter_bucket_007fbf78;   // [M46 compose] storage: lane-m46-3dcore scene3d_emitter_bucket_007fbf78 (single storage)
}
