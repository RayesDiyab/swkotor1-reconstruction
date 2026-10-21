// Framebuffer-modification storage + descriptors (lane-renderer-bodies, D13 round).
// Cell addresses: original 00427C90 operand scan (pushes/loads cited in NOTES);
// cold values from the pinned exe (.data raw bytes or loader-zero BSS).
#include "f90e611d0e-renderer_gl_cells.h"
#include "a8af050a1e-renderer_gl_imports.h"
#include "eca27cee56-renderer_original_data.h"
#include "27b4f16387-framebuffer.h"
#include "a7746c3ca6-ati_setup.h"
#include "88eb0e688f-window_shared_cells.h"
namespace kotor_renderer_gl_cells {
using kotor_renderer_init::FBWord;
using kotor_renderer_init::FBByte;
FBWord framebuffer_gate_0078d98c = 1;        // .data 01000000
FBByte framebuffer_enabled_0078daf4 = 1;     // .data 01
float bloom_intensity_0078daec = 0.5f;       // .data 0000003F
FBByte bloom_active_0078d990 = 1;            // .data 01
FBWord framebuffer_textures_007a47c4[6] = {};  // 007A47C4..007A47D8
void* framebuffer_dcs_007a47ec[6] = {};        // 007A47EC..007A4800
void* framebuffer_contexts_007a4804[6] = {};   // 007A4804..007A4818
void* framebuffer_pbuffers_007a6824[6] = {};   // 007A6824..007A6838
FBByte framebuffer_active_007a6860 = 0;
FBWord bloom_ramp_007a6864 = 0, bloom_ramp_007a6868 = 0, bloom_ramp_007a686c = 0;
FBWord ramp_007a6870 = 0, ramp_007a6874 = 0, ramp_007a6878 = 0;
FBWord bloom_texture_007a68c0 = 0;
void* bloom_pbuffer_007a68c4 = nullptr;
void* bloom_dc_007a68c8 = nullptr;
void* bloom_context_007a68cc = nullptr;
FBWord framebuffer_program_007a68d4 = 0;
FBWord ati_shader_007a692c = 0, ati_shader_007a6924 = 0, ati_shader_007a6920 = 0, ati_shader_007a691c = 0;
FBWord ati_shader_007a6918 = 0, ati_shader_007a690c = 0, ati_shader_007a6910 = 0, ati_shader_007a6914 = 0;
// .rdata constants (exact IEEE values of the original words).
const float half_0073e9ac = 0.5f;                       // 0000003F
const float inv255_0073ede0 = 0x1.010102p-8f;           // 8180803B
const float scale255_0073ede4 = 255.0f;                 // 00007F43
const double threshold_0073f3d0 = 0.5;                  // 000000000000E03F
const float quadratic_0073f404 = 4.0f;                  // 00008040
const float ramp40_0073f408 = 0x1.a41a42p-6f;           // 210DD23C
const float ramp_scale_0073f40c = 178.5f;               // 00803243
const float ramp22_0073f410 = 0x1.861862p-5f;           // 310C433D
namespace {
using namespace kotor_renderer_init;
const FrameBufferBindings framebuffer_table = {
    &framebuffer_gate_0078d98c, &framebuffer_enabled_0078daf4, &bloom_intensity_0078daec,
    &width_0078d1d4, &kotor_menu_window_cells::height_0078d1d8, &scene_texture_007a687c,
    framebuffer_textures_007a47c4, framebuffer_dcs_007a47ec, framebuffer_contexts_007a4804, framebuffer_pbuffers_007a6824,
    &main_dc_007a47e4, &main_context_007a6854, &framebuffer_active_007a6860,
    {&ramp_007a6870, &ramp_007a6874, &ramp_007a6878},
    {&bloom_ramp_007a6864, &bloom_ramp_007a6868, &bloom_ramp_007a686c},
    &power_width_007a6890, &power_height_007a6894,
    &bloom_active_0078d990, &bloom_texture_007a68c0,
    &bloom_pbuffer_007a68c4, &bloom_dc_007a68c8, &bloom_context_007a68cc,
    &framebuffer_program_007a68d4, framebuffer_program_0078de48, framebuffer_program_0078db80,
    &half_0073e9ac, &inv255_0073ede0, &scale255_0073ede4, &threshold_0073f3d0,
    &quadratic_0073f404, &ramp40_0073f408, &ramp_scale_0073f40c, &ramp22_0073f410,
    &iat_glGenTextures_0073d24c, &iat_glBindTexture_0073d240, &iat_glTexParameteri_0073d23c,
    &iat_glTexImage2D_0073d280, &iat_glTexImage1D_0073d2f0, &iat_glCopyTexImage2D_0073d2d8,
    &iat_wglMakeCurrent_0073d2b0, &iat_wglCreateContext_0073d2b8, &iat_wglShareLists_0073d2b4,
    &iat_glEnable_0073d390, &iat_glDisable_0073d22c,
    &wglGetPbufferDCARB_007bb6b8, &glGenProgramsARB_007bb528, &glBindProgramARB_007bb788,
    &glProgramStringARB_007bb580};
const ATISetupBindings ati_setup_table = {
    &ati_shader_007a692c, &ati_shader_007a6924, &ati_shader_007a6920, &ati_shader_007a691c,
    &ati_shader_007a6918, &ati_shader_007a690c, &ati_shader_007a6910, &ati_shader_007a6914,
    &iat_glEnable_0073d390, &iat_glDisable_0073d22c,
    &glGenFragmentShadersATI_007bb754, &glBindFragmentShaderATI_007bb6cc,
    &glBeginFragmentShaderATI_007bb7f4, &glEndFragmentShaderATI_007bb554,
    &glSampleMapATI_007bb620, &glPassTexCoordATI_007bb804,
    &glColorFragmentOp2ATI_007bb840, &glColorFragmentOp3ATI_007bb7fc,
    &glColorFragmentOp1ATI_007bb6d4, &glAlphaFragmentOp1ATI_007bb710};
}
}
namespace kotor_renderer_init {
const FrameBufferBindings& framebuffer = kotor_renderer_gl_cells::framebuffer_table;
const ATISetupBindings& ati_setup = kotor_renderer_gl_cells::ati_setup_table;
}
