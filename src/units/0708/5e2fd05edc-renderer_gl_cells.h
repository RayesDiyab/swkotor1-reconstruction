#pragma once
// Sole storage owner for the renderer capability/extension/soft-shadow cells used
// by the 0042E040/0044F2F0/0044F140/0045F770/00426CC0 cohort. Each object is one
// original .data cell at the address in its name, with the original PE image
// cold value (BSS cells zero). No runtime value is preset or forced: capability
// bits only change through the real probe/loader ORs, caches only through the
// real predicates, and extension slots only through real wglGetProcAddress.
// Main must pass these same objects (not copies) wherever another descriptor
// (renderer_adoption::Inputs, renderer_system::Views, options) borrows them.
#include <cstdint>
#include "d4ae4bb6c0-aa_probe.h"
#include "467f3b0d9b-aa_query.h"
#include "35c42231c1-pbuffer.h"
#include "a9b377eecb-soft_shadow.h"
#include "61e3cacb4e-renderer_gl_procs.h"
namespace kotor_renderer_gl_cells {
using U32 = std::uint32_t;
using I32 = std::int32_t;
using U16 = std::uint16_t;
using U8 = std::uint8_t;
// Capability word and extension text (BSS). Extension procedure slots and the
// 28 loader capability masks live in renderer_gl_procs.h (same namespace view).
extern U32 capabilities_007bb85c;
extern kotor_renderer_init::ExtensionString extension_proc_007bb7ec;
extern const char* extension_string_007a688c;
extern U32 ati_soft_gate_007bb860;
using namespace kotor_renderer_gl_procs;
// Masks outside the loader table (.data, original cold values).
extern U32 mask_multisample_0078e4ec, control_0078e4fc;
// Predicate caches (.data, cold sentinel 0xFFFFFFFF).
extern U32 cache_0078e508, cache_vertex_0078e51c, cache_soft_0078e520;
extern U32 cache_ati_soft_0078e524, cache_frame_0078e528, cache_ati_frame_0078e52c;
extern U32 cache_antialias_0078e5d8;
// Loader-path caches/state (.data cold values; BSS zero).
extern U32 cache_anisotropic_0078e534, cache_bumpy_shiny_0078e510, cache_vertex_buffer_0078e548;
extern I32 vertex_buffer_level_0078e500;          // cold 2; compared signed (0045FCC1 jl)
extern U8 vertex_buffer_disabled_007b927d;        // BSS; 0045FC90 cmp byte ==1
extern float max_anisotropy_007a6858;             // BSS; glGetFloatv output (0043727E)
extern U32 attenuation_texture_007a68a0;          // BSS; glGenTextures output (00426476)
extern U32 current_tid_007a683c[6];            // BSS; renderer_system::Views cache6
extern U32 current_stage_007a6898;                // BSS; renderer_system::Views zero word
extern U8 renderer_flag_0078e398;                 // cold 1; cleared at 00437264
extern U32 vertex_enum_0078d46c;                  // cold 0x8620; SharedBindings member
extern I32 initialized_0078e38c;                  // cold 1; SharedBindings member
extern U16 format_0078e388;                       // cold 0xFFFF; SharedBindings member
extern U8 vsync_0078d438;                         // cold 1; SharedBindings member
// Requested by fork F2 (NV combiner closure).
extern U32 env_map_on_00827fa0, tex1_on_00827fa4;   // BSS
extern U32 combiner_flag_0078d96c;                  // cold 1
extern U32 look_up_texture_007bb868;                // BSS
extern U32 multitexture_cache_0078e514;             // cold 0xFFFFFFFF
extern float homogenize_a_00831f18[4];              // BSS
// Shared renderer scalars (also SharedBindings members; same objects).
extern I32 antialias_007a6888, previous_antialias_0078d440, programs_0078e5ec;
extern U16 color_bits_007b9220, depth_bits_007b9224;
// Renderer-system handle words (renderer_system::Views context/dc).
extern U32 main_context_007a6854, main_dc_007a47e4;
// Soft-shadow state.
extern U8 soft_enabled_0078dae9;
extern U32 soft_requested_0078e420, width_0078d1d4;
extern U32 scene_texture_007a687c, scratch_texture_007a68f8;
extern U32 power_width_007a6890, power_height_007a6894;
extern void* pbuffer1_007a68e8; extern void* dc1_007a68ec; extern void* context1_007a68f0;
extern U32 texture1_007a68f4;
extern void* pbuffer2_007a68fc; extern void* dc2_007a6900; extern void* context2_007a6904;
extern U32 texture2_007a6908;
extern U32 soft_shader_007a6928;
// Immutable original strings used as call arguments.
extern const char class_name_0078e310[];
extern const char window_title_0073f478[];
extern const char extension_proc_name_0073f45c[];
extern const char choose_proc_name_0073f444[];
extern const char ati_fragment_shader_name_0073f42c[];
extern const char multisample_name_0073f418[];
}
// Borrowed from their single owners in other lanes (not defined here):
// 0078D458 GL_TEXTURE_2D enum cell -> lane-gui-draw gl_enum_cells.cpp;
// 007A685C anisotropy level -> lane-upload upload_cells.cpp.
namespace kotor_renderer_cells { extern std::uint32_t enable_0078d458; }
namespace texture_upload_cells { extern std::int32_t anisotropy_007a685c; }
