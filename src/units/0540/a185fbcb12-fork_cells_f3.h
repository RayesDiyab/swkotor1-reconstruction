#pragma once
// Cells requested by fork F3 (storage defined by the lane owner, not here).
// Types/cold values: original PE (tools/peread.py); all BSS unless noted.
#include <cstdint>
namespace kotor_renderer_gl_cells {
// GLRender ATI fragment shader IDs: nine separately addressed scalars written
// in this order by 004794E0 (004794F5, 004795AF, 0047969E, 0047975A, 00479AC3,
// 00479E25, 00479F46, 0047A15F, 0047A1CC). No array/adjacency is assumed.
extern std::uint32_t ati_fragment_shader_00830244, ati_fragment_shader_0083024c,
    ati_fragment_shader_00830248, ati_fragment_shader_00830250, ati_fragment_shader_00830254,
    ati_fragment_shader_00830258, ati_fragment_shader_0083025c, ati_fragment_shader_00830260,
    ati_fragment_shader_00830264;
extern std::uint32_t env_map_on_00827fa0;      // "envMapOn" (Ghidra symbol), BSS
extern std::uint32_t tex1_on_00827fa4;         // "tex1On" (Ghidra symbol), BSS
}
