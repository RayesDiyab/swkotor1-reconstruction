#pragma once
#include <cstdint>

namespace kotor_renderer_init {
// Existing framebuffer ABI. The shipped body never reads current_program.
void __cdecl ReportARBError_0047a2c0(const char* current_program);
}
namespace kotor_vertex_program {
// Existing Task15 caller spelling; forwards to the sole reconstructed body.
void __cdecl VertexProgramARBError_0047a2c0(const char* current_program);
}

// Revision (lane-renderer-bodies): the original calls the opengl32 IAT cells
// 0073D290 glGetString and 0073D244 glGetIntegerv; both go through the single
// kotor_renderer_init adapter owner instead of a second dllimport declaration.
#include "eb2df13cd1-renderer_gl_imports_gen.h"
namespace kotor_renderer_init {
extern const char* __stdcall glGetString_0073d290(U32);
}
