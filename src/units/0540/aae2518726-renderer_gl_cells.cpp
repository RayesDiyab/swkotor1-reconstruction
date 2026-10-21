// Storage + descriptor owner (lane-renderer-bodies). Cold values were read from
// the pinned GOG swkotor.exe sections (.data raw bytes, or loader-zero tail);
// see NOTES.md "cold values". All initializers are constant (no CRT dynamic init).
#include "f90e611d0e-renderer_gl_cells.h"
#include "a185fbcb12-fork_cells_f3.h"
#include "a8af050a1e-renderer_gl_imports.h"
#include "6cb781896f-aa_mode.h"
#include "112afbaf4f-vertex_availability.h"
#include "958b3f24f7-capabilities.h"
#include "dfcfb7fa81-validator_cells.h"
#include "88eb0e688f-window_shared_cells.h"
namespace kotor_renderer_gl_cells {
U32 capabilities_007bb85c = 0;
kotor_renderer_init::ExtensionString extension_proc_007bb7ec = nullptr;
const char* extension_string_007a688c = nullptr;
U32 ati_soft_gate_007bb860 = 0;  // original runtime writer 0044EF90 not in this cohort
U32 mask_multisample_0078e4ec = 0x08000000u, control_0078e4fc = 1u;
U32 cache_0078e508 = 0xffffffffu, cache_vertex_0078e51c = 0xffffffffu, cache_soft_0078e520 = 0xffffffffu;
U32 cache_ati_soft_0078e524 = 0xffffffffu, cache_frame_0078e528 = 0xffffffffu, cache_ati_frame_0078e52c = 0xffffffffu;
U32 cache_antialias_0078e5d8 = 0xffffffffu;
U32 cache_anisotropic_0078e534 = 0xffffffffu, cache_bumpy_shiny_0078e510 = 0xffffffffu, cache_vertex_buffer_0078e548 = 0xffffffffu;
I32 vertex_buffer_level_0078e500 = 2;
U8 vertex_buffer_disabled_007b927d = 0;
float max_anisotropy_007a6858 = 0.0f;
U32 attenuation_texture_007a68a0 = 0;
U32 current_tid_007a683c[6] = {};
U32 current_stage_007a6898 = 0;
U8 renderer_flag_0078e398 = 1;
U32 vertex_enum_0078d46c = 0x8620u;
I32 initialized_0078e38c = 1;
U16 format_0078e388 = 0xffffu;
U8 vsync_0078d438 = 1;
U32 env_map_on_00827fa0 = 0, tex1_on_00827fa4 = 0;
U32 combiner_flag_0078d96c = 1;
U32 look_up_texture_007bb868 = 0;
U32 multitexture_cache_0078e514 = 0xffffffffu;
float homogenize_a_00831f18[4] = {};
// Requested by fork F3: ATI fragment shader IDs (BSS, separately addressed).
std::uint32_t ati_fragment_shader_00830244 = 0, ati_fragment_shader_0083024c = 0, ati_fragment_shader_00830248 = 0;
std::uint32_t ati_fragment_shader_00830250 = 0, ati_fragment_shader_00830254 = 0, ati_fragment_shader_00830258 = 0;
std::uint32_t ati_fragment_shader_0083025c = 0, ati_fragment_shader_00830260 = 0, ati_fragment_shader_00830264 = 0;
I32 antialias_007a6888 = 0, previous_antialias_0078d440 = -1, programs_0078e5ec = 1;
U16 color_bits_007b9220 = 0, depth_bits_007b9224 = 0;
U32 main_context_007a6854 = 0, main_dc_007a47e4 = 0;
U8 soft_enabled_0078dae9 = 1;
U32 soft_requested_0078e420 = 1, width_0078d1d4 = 800;
U32 scene_texture_007a687c = 0, scratch_texture_007a68f8 = 0;
U32 power_width_007a6890 = 0, power_height_007a6894 = 0;
void* pbuffer1_007a68e8 = nullptr; void* dc1_007a68ec = nullptr; void* context1_007a68f0 = nullptr;
U32 texture1_007a68f4 = 0;
void* pbuffer2_007a68fc = nullptr; void* dc2_007a6900 = nullptr; void* context2_007a6904 = nullptr;
U32 texture2_007a6908 = 0;
U32 soft_shader_007a6928 = 0;
const char class_name_0078e310[] = "Test GL Window";
const char window_title_0073f478[] = "Exo Base Window";
const char extension_proc_name_0073f45c[] = "wglGetExtensionsStringARB";
const char choose_proc_name_0073f444[] = "wglChoosePixelFormatARB";
const char ati_fragment_shader_name_0073f42c[] = "GL_ATI_fragment_shader";
const char multisample_name_0073f418[] = "WGL_ARB_multisample";
namespace {
using namespace kotor_renderer_init;
AntiAliasingProbeBindings probe_table = {
    &kotor_menu_window_cells::instance_007a39c0,
    &iat_DefWindowProcA_0073d3b0,
    class_name_0078e310, window_title_0073f478,
    extension_proc_name_0073f45c, choose_proc_name_0073f444,
    ati_fragment_shader_name_0073f42c, multisample_name_0073f418,
    &iat_wglGetProcAddress_0073d310,
    &extension_proc_007bb7ec, &wglChoosePixelFormatARB_007bb730, &extension_string_007a688c,
    &capabilities_007bb85c, &mask_multisample_0078e4ec};
AntiAliasingModeBindings mode_table = {&antialias_007a6888, &previous_antialias_0078d440};
AntiAliasingQueryBindings query_table = {
    &kotor_menu_window_cells::render_007a39d8, &color_bits_007b9220, &depth_bits_007b9224,
    &wglChoosePixelFormatARB_007bb730, &wglGetPixelFormatAttribivARB_007bb63c};
VertexAvailabilityBindings availability_table = {
    &cache_vertex_0078e51c, &mask_0078e4d4, &capabilities_007bb85c, &programs_0078e5ec};
const kotor_renderer_capabilities::Bindings capability_table = {
    &cache_soft_0078e520, &cache_ati_soft_0078e524, &cache_frame_0078e528, &cache_ati_frame_0078e52c,
    &mask_0078e4c8, &mask_0078e4d0, &mask_0078e4d4, &mask_0078e4dc, &mask_0078e4e4,
    &capabilities_007bb85c, &ati_soft_gate_007bb860};
const kotor_pbuffer::Bindings pbuffer_table = {&main_dc_007a47e4, &wglChoosePixelFormatARB_007bb730, &wglCreatePbufferARB_007bb530};
const kotor_soft_shadow::Bindings soft_table = {
    &soft_enabled_0078dae9, &soft_requested_0078e420,
    &width_0078d1d4, &kotor_menu_window_cells::height_0078d1d8,
    &scene_texture_007a687c, &scratch_texture_007a68f8,
    &power_width_007a6890, &power_height_007a6894,
    &main_dc_007a47e4, &main_context_007a6854,
    &pbuffer1_007a68e8, &dc1_007a68ec, &context1_007a68f0, &texture1_007a68f4,
    &pbuffer2_007a68fc, &dc2_007a6900, &context2_007a6904, &texture2_007a6908,
    &soft_shader_007a6928,
    &wglGetPbufferDCARB_007bb6b8, &wglMakeContextCurrentARB_007bb818,
    &glGenFragmentShadersATI_007bb754, &glBindFragmentShaderATI_007bb6cc,
    &glBeginFragmentShaderATI_007bb7f4, &glSampleMapATI_007bb620,
    &glColorFragmentOp2ATI_007bb840, &glAlphaFragmentOp2ATI_007bb5e8, &glColorFragmentOp3ATI_007bb7fc, &glAlphaFragmentOp3ATI_007bb680,
    &glEndFragmentShaderATI_007bb554};
}
}
// Descriptor exports with the exact names the reviewed bodies borrow.
namespace kotor_renderer_init {
AntiAliasingProbeBindings& aa_probe = kotor_renderer_gl_cells::probe_table;
AntiAliasingModeBindings& aa_mode = kotor_renderer_gl_cells::mode_table;
AntiAliasingQueryBindings& aa_query = kotor_renderer_gl_cells::query_table;
VertexAvailabilityBindings& availability = kotor_renderer_gl_cells::availability_table;
}
namespace kotor_renderer_capabilities {
const Bindings& bindings = kotor_renderer_gl_cells::capability_table;
}
namespace kotor_pbuffer {
const Bindings& bindings = kotor_renderer_gl_cells::pbuffer_table;
}
namespace kotor_soft_shadow {
const Bindings& bindings = kotor_renderer_gl_cells::soft_table;
}
namespace kotor_renderer_validation_required {
volatile std::uint32_t& cache_0078e508 = kotor_renderer_gl_cells::cache_0078e508;
const volatile std::uint32_t& mask_0078e4d0 = kotor_renderer_gl_procs::mask_0078e4d0;
const volatile std::uint32_t& capabilities_007bb85c = kotor_renderer_gl_cells::capabilities_007bb85c;
const volatile std::uint32_t& control_0078e4fc = kotor_renderer_gl_cells::control_0078e4fc;
const volatile std::uint32_t& mask_0078e4f4 = kotor_renderer_gl_procs::mask_0078e4f4;
}
