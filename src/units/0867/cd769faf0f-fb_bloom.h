#pragma once
// lane-3d-fb: SUBLANES.json area "framebuffer_bloom" (27 functions) of swkotor.exe (GOG K1, image base 00400000).
//
// First-menu-frame execution (NOTES.md "Gates"): only 00426650 SetFilmNoiseTexture and 00450BB0
// Scene::RenderMotionBlur execute unconditionally; 0046EDA0 AurPixelShadersAvailable is reached only through
// PartTriMesh::Draw render paths 2/3/4/5 (bump/normal-mapped materials). Every other function is reached only
// through Scene+D4 != 0 (00451746 / 0045194F / 0045197A), which is 0 for the GUI scene (00458AD4), or through an
// emitter whose render mode is Motion_Blur (0049CA3B), which no menu model contains.
//
// Conventions: canonical 3D header scene3d.h (lane-3d-render). D1 __fastcall(Self*, void* edx_unused, ...) for
// __thiscall. Free functions keep the original __cdecl contract (plain RET, caller pops). GL imports are called
// through the single adapter owner kotor_renderer_init::<import>_<IAT> (lane-renderer-bodies), extension procs
// through the typed kotor_renderer_gl_procs::<name>_<cell> slots, reloaded per call as in the original.
// Renderer cells are borrowed from their owners (renderer_gl_cells.h / framebuffer.h / ati_setup.h descriptors,
// scene3d.h `cells`); cells with no owner anywhere are defined by this lane in fb_cells.cpp (original cold values).
// D30-FINAL: x87 intermediates are doubles; float only at original `fstp dword` stores; _ftol2 conversions go
// through the original-ABI boundary unresolved_ftol2_006fae8c (ST0 -> EDX:EAX) via MSVC inline asm (D21 precedent:
// lane-renderer-bodies framebuffer.cpp).
#include <cstdint>
#include "f078693989-scene3d.h"
#include "536853f13e-renderer_gl_cells.h"          // kotor_renderer_gl_cells (soft-shadow / caps / handles), kotor_renderer_gl_procs
#include "2078381901-renderer_gl_imports.h"        // kotor_renderer_init adapters (soft-shadow subset + IAT cells)
#include "ceeafce552-renderer_gl_imports_gen.h"    // kotor_renderer_init adapters (remaining opengl32 imports)
#include "23fd77bc4f-framebuffer.h"                // kotor_renderer_init::framebuffer (FB cells), FrameBufferAvailable_0045f820
#include "e43e08f0a4-ati_setup.h"                  // kotor_renderer_init::ati_setup (ATI fragment shader ids 007A690C..007A692C)
#include "ce9c64f5c8-soft_shadow.h"                // kotor_soft_shadow::ATISoftAvailable_0045f7e0, bindings
#include "a3091fe3c6-render_shared_lowers.h"       // kotor_renderer_gl::ActivateTextureStage_0041fe80

namespace kotor_fb {
using U = std::uint32_t;
using I = std::int32_t;
using U8 = std::uint8_t;
using U16 = std::uint16_t;

// ---------------------------------------------------------------- .rdata constants (immutable original values)
// Read from the pinned image (rd.py); cited at each use. Immutable section, so literal constants are exact.
constexpr U kGlTexCoordArray_0073f398 = 0x8078;    // 78 80 00 00
constexpr U kGlNormalArray_0073f39c = 0x8075;      // 75 80 00 00
constexpr U kGlColorArray_0073f3a0 = 0x8076;       // 76 80 00 00
constexpr U kGlVertexArray_0073f3a4 = 0x8074;      // 74 80 00 00
constexpr U kGlMaxGeneralCombinersNV_0073f26c = 0x854D;  // 4D 85 00 00
constexpr U kBlendSrcAlpha_0073f270 = 0x0302;      // 02 03 00 00
constexpr U kBlendOneMinusSrcAlpha_0073f274 = 0x0303; // 03 03 00 00
constexpr U kOne_0073f278 = 1;                     // 01 00 00 00
constexpr U kFloat0_0073d700 = 0x00000000u;        // 0.0f
constexpr U kFloat1_0073d764 = 0x3F800000u;        // 1.0f
constexpr U kFloat2p32_0073d770 = 0x4F800000u;     // 4294967296.0f (unsigned->float fixup)
constexpr U kFloat0p1_0073e798 = 0x3DCCCCCDu;      // 0.1f
constexpr U kFloatHalf_0073e9ac = 0x3F000000u;     // 0.5f
constexpr U kFloat255_0073ede4 = 0x437F0000u;      // 255.0f
constexpr U kLumaR_0073f4f0 = 0x3DA7EF9Eu;         // 0.0820f  (9E EF A7 3D)
constexpr U kLumaG_0073f4f4 = 0x3F1C01A3u;         // 0.6094f  (A3 01 1C 3F)
constexpr U kLumaB_0073f4f8 = 0x3E9E00D2u;         // 0.3086f  (D2 00 9E 3E)
constexpr std::uint64_t kDouble_00743308 = 0x3EE4F8B588E368F1ull; // double 1.0e-5 (F1 68 E3 88 B5 F8 E4 3E)
inline float f(U bits) { return kotor_scene3d::bits_to_float(bits); }
} // namespace kotor_fb

// ---------------------------------------------------------------- cells owned by this lane (fb_cells.cpp)
// No other lane defines these addresses (owners_scan.txt). Original cold values; writers cited.
extern "C" {
extern volatile float fb_speed_blur_ratio_0078d43c;          // .data 0.98f; writer AurSetSpeedBlurRatio 0044F134
extern volatile kotor_fb::U fb_effect_gate_0078d974;          // .data 1; no writer instruction (read 00435E23 / 00437793)
extern volatile kotor_fb::U fb_effect_gate_0078d978;          // .data 1; no writer (00435F07 ...)
extern volatile kotor_fb::U fb_effect_gate_0078d97c;          // .data 1; no writer (004361F0 path 00436232 / 004362E6)
extern volatile kotor_fb::U fb_effect_gate_0078d980;          // .data 1; no writer (00435E8B / 004377FA)
extern volatile kotor_fb::U fb_effect_gate_0078d984;          // .data 1; no writer (004375FC)
extern volatile kotor_fb::U fb_effect_gate_0078d988;          // .data 1; no writer (00435F8A ...)
extern volatile float fb_bloom_param_0078daf8;                // .data 0.35f; no writer (0042BC00 raw DWORD load)
extern volatile float fb_bloom_param_0078db00;                // .data 2.0f;  no writer (0042BC40 fsub)
extern volatile float fb_bloom_param_0078db04;                // .data 0.4f;  no writer (0042BB1F fld)
extern volatile kotor_fb::I fb_bloom_count_0078db08;          // .data 5;     no writer (0042CA9E)
extern volatile float fb_bloom_param_0078db0c;                // .data 64.0f; no writer (0042C9CF raw DWORD load)
extern volatile kotor_fb::U fb_convolution_gate_0078db20;     // .data 1;     no writer (0042C6E0)
extern volatile float fb_saturation_param_0078db24;           // .data 1.0f; writer AurSetSaturationParams 0044F10C
extern volatile float fb_saturation_param_0078db28;           // .data 1.0f; writer 0044F115
extern volatile float fb_saturation_param_0078db2c;           // .data 1.0f; writer 0044F11B
extern volatile float fb_saturation_param_007a6884;           // BSS 0.0f;  writer 0044F121
extern const float fb_shader_constants_0078e110[18][4];       // .data, no writer; addresses pushed as const float* (0042941E ...)
extern const float fb_bloom_kernel_0078e230[28];              // .data 5x5 kernel + 3 zero pad floats 0078E294..0078E29F (loop indexes 0..27, 0042BB25)
extern const float fb_bloom_kernel_0078e2a0[28];              // .data 5x5 kernel + 3 zero pad floats 0078E304..0078E30F (0042C273)
extern kotor_fb::I fb_pixel_shaders_cache_0078e5dc;           // .data -1; writers 0046EDC7 (glGetIntegerv) / 0046EDDB / 0046EDE1 / 0046EDF0
extern volatile kotor_fb::U8 fb_speed_blur_007a6880;          // BSS; AurEnable/DisableSpeedBlur 0044F0A0 / 0044F0B0
extern volatile kotor_fb::U8 fb_saturation_007a6881;          // BSS; AurEnable/DisableSaturation 0044F0C0 / 0044F0D0
extern volatile kotor_fb::U8 fb_scan_noise_007a6882;          // BSS; AurEnable/DisableScanNoise 0044F0E0 / 0044F0F0
extern volatile kotor_fb::U8 fb_film_noise_flag_007a6883;     // BSS; no writer instruction (00435F93 / 00437948)
extern volatile kotor_fb::U fb_word_007a68bc;                 // BSS; no writer instruction (00435DBB / 0043772B)
extern void* volatile fb_film_noise_texture_007a68d0;         // BSS; writer SetFilmNoiseTexture 00426654
extern volatile kotor_fb::U fb_word_007a6938;                 // BSS; no writer instruction (0042C9CA / 0042CEBC)
extern volatile kotor_fb::U8 fb_ati_frame_rendered_007a698c;  // BSS; writers 00435DAB / 00435DB4
extern volatile kotor_fb::U8 fb_frame_rendered_007a698d;      // BSS; writers 0043771B / 00437724
}
// D69: 00827FB0 single storage = lane-r4-lowers r4_lowers_cells::render_counts_00827fa8[2] (earliest-staged owner);
// the fb name is a view of that element (object-pointer cast, D24; no second storage).
namespace r4_lowers_cells { extern std::uint32_t volatile render_counts_00827fa8[6]; }
#define aurora_triangle_count_00827fb0 (*reinterpret_cast<volatile kotor_fb::I*>(&r4_lowers_cells::render_counts_00827fa8[2]))

// ---------------------------------------------------------------- lowers owned by other areas (declared here)
extern "C" {
// vp_nv_ati (SUBLANES): 0045FB90 aurATIFragmentShadersBumpMapAvailable, AL result (0045FBBE xor al,al / 0046EDAF test al,al)
bool __cdecl aur_ati_fragment_shaders_bump_map_available_0045fb90();
// core (SUBLANES, not authored yet): render-to-texture / mipmap filters / quad emitter
void __cdecl gl_render_frame_to_texture_004337a0(kotor_fb::U texture);          // 00437712 push ecx; add esp,8 (with 0041FE80 arg)
void __cdecl gl_render_frame_to_texture_ati_00433580(kotor_fb::U texture);      // vp_nv_ati; 00435DA2 push eax; add esp,4
// core 0041FE80 is authored by lane-renderer-bodies F2 (kotor_renderer_gl::ActivateTextureStage_0041fe80) and reused.
// core 00420600 CAurTexture::IsNull: same declaration as lane-3d-mesh trimesh_f1.cpp (AL result, ECX = texture).
std::uint8_t __fastcall aur_texture_is_null_00420600(void* texture, void* unused_edx);
// 00401670 _glBlendFunc (cdecl, plain RET; owner unassigned in SUBLANES.json "calls_into_other_areas": core)
void __cdecl gl_blend_func_00401670(kotor_fb::U src, kotor_fb::U dst);
}

// ---------------------------------------------------------------- authored by this lane
extern "C" {
using kotor_fb::U;
// fb_executed.cpp (first-frame executed)
// void __cdecl gl_set_film_noise_texture_00426650(void* texture);  -- declared in scene3d.h (core caller 00451935)
// void __fastcall scene_render_motion_blur_00450bb0(void* scene, void*);  -- Scene vtbl+CC; declared in vtables_gen.h
//   (proposed diff: return type U -> void, see proposed/vtables_gen.h.diff; original EAX is the last glDepthMask's
//   undefined return and the only caller 0045191E ignores it)
U __cdecl aur_pixel_shaders_available_0046eda0();                      // EAX = cache value (0046EDDB / 0046EDF0)
// fb_pixel_shaders.cpp
void __cdecl gl_frame_buffer_modification_pixel_shaders_ati_004293f0(U mode, U t0, U t1, U t2, U t3);
void __cdecl gl_frame_buffer_modification_pixel_shaders_004299c0(U mode, U t0, U t1, U t2, U t3);
// fb_bloom_conv.cpp / fb_soft_shadows.cpp
void __cdecl gl_render_overlay_convolution_ati_0042c6e0(U texture);
void __cdecl gl_render_overlay_convolution_0042cc60(U texture);
void __cdecl gl_set_up_soft_shadows_0042d1c0();
void __cdecl gl_finish_soft_shadows_ati_0042d250();
void __cdecl gl_finish_soft_shadows_0042d9a0();
// fb_overlays.cpp
void __cdecl gl_render_overlay_film_noise_ati_00432780(U texture, U flag);
void __cdecl gl_render_overlay_film_noise_00432ad0(U texture, U flag);
void __cdecl gl_render_force_distortion_ati_00432d70(U texture);
void __cdecl gl_render_force_distortion_004331d0(U texture);
void __cdecl gl_render_accumulation_blur_ati_00433970(U texture);
void __cdecl gl_render_accumulation_blur_00433da0(U texture);
void __cdecl gl_render_overlay_overbrighten_004340f0(U texture);
void __cdecl gl_render_overlay_overbrighten_ati_00434240(U texture);
void __cdecl gl_render_saturation_adjustment_ati_004343d0(U texture);
void __cdecl gl_render_saturation_adjustment_00434890(U texture);
// fb_modifications.cpp
void __cdecl gl_frame_buffer_modifications_ati_00435bd0(U mask);
void __cdecl gl_frame_buffer_modifications_ati2_00436070(U mask);
// void __cdecl gl_frame_buffer_modifications2_004361f0(U mask);   -- declared in scene3d.h
// void __cdecl gl_frame_buffer_modifications_00437520(U mask);    -- declared in scene3d.h
}
