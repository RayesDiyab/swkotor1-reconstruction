#pragma once
// Original globals used by fork F2 bodies. Declarations only: the lane owner
// (renderer_gl_cells.cpp) defines storage with the original cold values.
#include <cstdint>
namespace kotor_renderer_gl_cells {
extern std::uint32_t current_stage_007a6898;      // uiCurrentStage, BSS 0 (= renderer_system::Views zero_007a6898)
extern std::uint32_t current_tid_007a683c[6];     // glCurrentTID[6], BSS 0 (= renderer_system::Views cache6_007a683c)
extern std::uint32_t env_map_on_00827fa0;         // envMapOn, BSS 0
extern std::uint32_t tex1_on_00827fa4;            // tex1On, BSS 0
extern std::uint32_t attenuation_texture_007a68a0;// GL id written by InitAttenuationTexture 0042646F, BSS 0
extern std::uint32_t combiner_flag_0078d96c;      // .data cold 1 (read at 00430087)
extern std::uint32_t look_up_texture_007bb868;    // lookUpTexture GL id, BSS 0
extern std::uint32_t multitexture_cache_0078e514; // aurMultiTextureAvailable cache, .data cold 0xFFFFFFFF
extern float homogenize_a_00831f18[4];            // homogenizeA (Quaternion), BSS 0
}
