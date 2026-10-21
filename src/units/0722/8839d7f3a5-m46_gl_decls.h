#pragma once
// lane-m46-3dcore fork E: exported declarations of the GLRender / texture / tile helpers authored here.
// Each prototype is the exact consumer ABI found in the M45 stage (file cited); a consumer-check unit
// (m46_gl_consumer_check.cpp / m46_tile_consumer_check.cpp) includes the consumer headers together with this one.
// All are extern "C"; cdecl entries end in a plain RET (caller pops), fastcall entries are the D1 spelling of the
// original __thiscall (ECX receiver, unused EDX).
#include <cstdint>

namespace aurora_model { struct MdlNode; struct Part; struct Gob; }
namespace kotor_scene3d { struct Vector; struct Quaternion; }
namespace kotor_reconstruction { struct ArrayMutations; }

extern "C" {
// ---- m46_gl_fb.cpp
// 00401670 _glBlendFunc(source_index, destination_index): fb_bloom.h:105 spelling; second spelling
// _gl_blend_func_indexed_00401670 (construct3d.h:196, trimesh_skin.h:212, vp_f1/vp_f2) joined by /alternatename.
void __cdecl gl_blend_func_00401670(std::uint32_t source_index, std::uint32_t destination_index);
// 0041FEB0 AurTextureGetMaxTexID (bsp.h:264)
std::int32_t __cdecl aur_texture_get_max_tex_id_0041feb0();
// 004204B0 / 004204C0 AurTextureSetModeStatic / Dynamic (tile_scene.h:29-30, room_spawn.cpp:50-51)
void __cdecl aur_texture_set_mode_static_004204b0();
void __cdecl aur_texture_set_mode_dynamic_004204c0();
// 00420600 CAurTexture::IsNull (fb_bloom.h:103, trimesh_skin.h:202, vp_f1:36): AL result
std::uint8_t __fastcall aur_texture_is_null_00420600(void* texture, void* unused_edx);
// 00425250 GLRender::ActivateTextureStage (client array stage) (triangle_bin.h:131); second spelling
// _gl_render_activate_texture_stage_00425250 (trimesh_skin.h:204) joined by /alternatename.
void __cdecl gl_render_activate_client_texture_stage_00425250(std::uint32_t stage);
// 00425280 / 004253D0 (vp_f1_lightmap_nv1x.cpp:45/47)
void __cdecl gl_render_set_lightmap_tex_coord_to_stage0_00425280(std::uint32_t word_14c, std::uint32_t word_150,
    std::uint32_t pool_index, std::int32_t count, void* node, std::uint32_t offset);
void __cdecl gl_render_set_decal_tex_coord_to_stage0_004253d0(std::uint32_t word_14c, std::uint32_t word_150,
    std::uint32_t pool_index, std::int32_t count, void* node, std::uint32_t offset);
// 00426230 GLRender::DrawLightmappedGrass (triangle_bin.h:133)
void __cdecl gl_render_draw_lightmapped_grass_00426230(const void* vertices, std::uint32_t position_stride,
    const void* attributes, std::int32_t attribute_stride, std::int32_t first, std::int32_t quads);
// 0042B370 / 0042B5D0 GLRender::MipmapFilter ATI / NV (fb_bloom_conv.cpp:19/21)
void __cdecl gl_mipmap_filter_ati_0042b370(void* dc, void* context, float width, float height, void* pbuffer,
    std::uint32_t texture, float size);
void __cdecl gl_mipmap_filter_0042b5d0(void* dc, void* context, float width, float height, void* pbuffer,
    std::uint32_t texture, float size);
// 00431F70 GLRender::RenderObjectsVCUP (construct3d.h:195)
void __cdecl gl_render_objects_vcup_00431f70(std::uint32_t mode, void* vertices, std::int32_t count);
// 004337A0 GLRender::RenderFrameToTexture (fb_bloom.h:99)
void __cdecl gl_render_frame_to_texture_004337a0(std::uint32_t texture);
// 0044F880 glMaterialfv(face index, pname index, params) wrapper (triangle_bin.h:129)
void __cdecl gl_material_fv_0044f880(std::uint32_t face_index, std::uint32_t pname_index, const float* params);

// ---- cells defined in m46_gl_fb.cpp (no owner anywhere: build/E_xref.py scan; original cold values)
extern volatile std::uint8_t aur_texture_mode_dynamic_0078d400;   // .data byte 01 (AurTextureSetMode*; read 00423269..)
}
