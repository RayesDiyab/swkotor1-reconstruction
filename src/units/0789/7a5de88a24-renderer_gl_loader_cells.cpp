// LoadExtensions_00436490 storage + descriptor (lane-renderer-bodies, D13 round).
// Every field names the original object the loader touches; addresses cited in
// extension_loader.h / NOTES. Program bases/suffixes come from the LOCAL D13
// extract (renderer_original_data.h); procedure rows/masks from renderer_gl_procs.
#include "5e2fd05edc-renderer_gl_cells.h"
#include "fee770f519-renderer_gl_imports.h"
#include "61e3cacb4e-renderer_gl_procs.h"
#include "19ccdafce3-renderer_original_data.h"
#include "97677fa6bb-extension_loader.h"
#include "16ea3f255d-render_pool.h"
#include "0a27d352e1-nv_combiners.h"
#include "fadbb6dacf-ati_fragment_shaders.h"
#include <cstring>
namespace kotor_renderer_gl_cells {
// 0073F52C .rdata "GeForce3" (strstr needle at 00437252); short identifier literal.
const char special_renderer_0073f52c[] = "GeForce3";
// Program output words written at 004372F3/00437332 (ATI) and 00437416/0043745C (NV).
char* first_program_0083028c = nullptr;
char* second_program_00830290 = nullptr;
// Pool cells requested by fork F1 (BSS zero in the original).
kotor_renderer_gl::RequestPoolManager* pool_007a68a4 = nullptr;
std::uint32_t pool_total_bytes_007a68a8 = 0;
std::uint32_t pool_page_bytes_007a68ac = 0;
std::uint32_t pool_vbo_bytes_007a6984 = 0;
std::uint32_t pool_sys_bytes_007a6988 = 0;
// Dispatch cells (BSS), typed per the unified NV/ATI signatures.
kotor_renderer_init::EnableDispatch enable_dispatch_007a47e0 = nullptr;
kotor_renderer_init::ConstantDispatch constant0_dispatch_007a6820 = nullptr;
kotor_renderer_init::ConstantDispatch constant1_dispatch_007a47dc = nullptr;
namespace {
using namespace kotor_renderer_init;
ExtensionLoaderBindings loader_table = {
    &iat_wglGetProcAddress_0073d310, &iat_glGetString_0073d290,
    extension_proc_name_0073f45c, &extension_proc_007bb7ec,
    capability_rows_28, procedure_rows_201,
    &capabilities_007bb85c, &mask_0078e49c,
    special_renderer_0073f52c, &renderer_flag_0078e398,
    &max_anisotropy_007a6858, &mask_0078e4d0,
    &enable_dispatch_007a47e0, &constant0_dispatch_007a6820, &constant1_dispatch_007a47dc,
    &kotor_renderer_gl::EnableFragmentShaders_0047a2f0, &kotor_renderer_gl::EnableCombiners_0042e600,
    &kotor_renderer_gl::SetFragmentShaderConstant0_0047a280, &kotor_renderer_gl::SetFragmentShaderConstant0_00424940,
    &kotor_renderer_gl::SetFragmentShaderConstant1_0047a2a0, &kotor_renderer_gl::SetCombinerConstant1_00424960,
    arb_vertex_base_0078f600, arb_vertex_base_00791240,
    ati_program_suffix_0078fe18, nv_program_suffix_0078fd78,
    &first_program_0083028c, &second_program_00830290,
    &programs_0078e5ec, &vertex_enum_0078d46c,
    &pool_007a68a4};
}
}
namespace kotor_renderer_init {
ExtensionLoaderBindings& extension_loader = kotor_renderer_gl_cells::loader_table;
// 00437258 call 006FBD00: CRT _strstr (statically linked in the original).
const char* __cdecl StringSearch_006fbd00(const char* text, const char* needle) {
    return std::strstr(text, needle);
}
}
