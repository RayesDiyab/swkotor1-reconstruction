#pragma once
// lane-3d-mesh (SUBLANES.json area "trimesh_skin"): TriMesh / Skin render paths of swkotor.exe (GOG K1, base 00400000).
// Includes the canonical 3D header (lane-3d-render scene3d.h). The MeshCells view and the declarations below are
// PROPOSED for append to scene3d.h (see ../scene3d.h.proposed.diff); until adopted they live here.
//
// Conventions (DECISIONS D1/D1-amendment/D21/D24/D30-FINAL):
//  * original __thiscall bodies = __fastcall(Self*, void* unused_edx, stack...); callers pass nullptr for EDX;
//    no slot here returns a struct by value and none is variadic (D1-amendment scan: clean).
//  * virtual calls reload the receiver's vtable at every call and go through PUBLISHED typed tables only:
//      textures  -> lane-texture-epoch texture_vtable::head()/bind_tail() (font::Texture receivers)
//      Part/VertexPrimitive -> lane-3d-model aurora_vt::VTable_00740b68 / VTable_00742f74 (object-pointer view, D24)
//      render pool -> lane-renderer-bodies kotor_renderer_gl::RequestPoolManager (C++ virtual, thiscall)
//  * GL entry points: the single adapter owner kotor_renderer_init::<import>_<IAT> (lane-renderer-bodies).
//  * D30-FINAL: x87 wide intermediates are `double`; `float` only where the original stores with fstp dword.
//  * Objects without a published layout are byte-offset views (kotor_scene3d::field/at); every offset is cited.
#include "6c81d8874d-scene3d.h"
#include "283ae0c931-texture_vtable.hpp"   // lane-texture-epoch (E1) typed texture tables; material_owner::Material via texture_join.h
#include "7ee14b5ec3-model_vtables.hpp"    // lane-3d-model typed Part / VertexPrimitive tables (D39)
#include "16ea3f255d-render_pool.h"        // lane-renderer-bodies RequestPoolManager (pool cell 007A68A4)
#include "e4678f005c-vertex_program.h"     // lane-renderer-bodies / renderer-epoch VertexProgram (0x201C bytes)

// ---------------------------------------------------------------- single GL adapter owner (exact copies)
namespace kotor_renderer_init {
using U32 = std::uint32_t; using I32 = std::int32_t; using U8 = std::uint8_t;
extern U8 __stdcall glIsEnabled_0073d37c(U32);
extern void __stdcall glAlphaFunc_0073d238(U32, float);
extern void __stdcall glBegin_0073d2d0(U32);
extern void __stdcall glBlendFunc_0073d38c(U32, U32);
extern void __stdcall glColor3f_0073d2ec(float, float, float);
extern void __stdcall glColor4f_0073d224(float, float, float, float);
extern void __stdcall glColor4ubv_0073d2e8(const U8*);
extern void __stdcall glColorPointer_0073d2a0(I32, U32, I32, const void*);
extern void __stdcall glDepthMask_0073d28c(U8);
extern void __stdcall glDisableClientState_0073d264(U32);
extern void __stdcall glDrawArrays_0073d2ac(U32, I32, I32);
extern void __stdcall glDrawElements_0073d258(U32, I32, U32, const void*);
extern void __stdcall glEnableClientState_0073d260(U32);
extern void __stdcall glEnd_0073d2c4();
extern void __stdcall glFogf_0073d350(U32, float);
extern void __stdcall glFogfv_0073d354(U32, const float*);
extern void __stdcall glFogi_0073d34c(U32, I32);
extern void __stdcall glGetIntegerv_0073d244(U32, I32*);
extern void __stdcall glLineWidth_0073d380(float);
extern void __stdcall glPopAttrib_0073d26c();
extern void __stdcall glPopClientAttrib_0073d348();
extern void __stdcall glPopMatrix_0073d2fc();
extern void __stdcall glPushAttrib_0073d268(U32);
extern void __stdcall glPushClientAttrib_0073d344(U32);
extern void __stdcall glPushMatrix_0073d300();
extern void __stdcall glTexGeni_0073d314(U32, U32, I32);
extern void __stdcall glVertex3f_0073d2c8(float, float, float);
extern void __stdcall glVertex3fv_0073d2e4(const float*);
extern void __stdcall glVertexPointer_0073d2a8(I32, U32, I32, const void*);
// ADAPTER REQUEST (lane-renderer-bodies single adapter owner): user32 MessageBoxA, IAT 0073D418 (0047B940).
extern I32 __stdcall MessageBoxA_0073d418(void* window, const char* text, const char* caption, U32 type);
// Same alias as lane-renderer-bodies extension_loader.h (dispatch cells 007A47E0 / 007A6820 / 007A47DC).
using EnableDispatch=void(__cdecl*)(U32,texture::Basic*,texture::Basic*,texture::Basic*,texture::Basic*);
using ConstantDispatch=void(__cdecl*)(const float*,float);
}

namespace kotor_trimesh {
using kotor_scene3d::U; using kotor_scene3d::I; using kotor_scene3d::U8; using kotor_scene3d::U16;
using kotor_scene3d::Vector; using kotor_scene3d::Quaternion; using kotor_scene3d::PointerList;
using kotor_scene3d::field; using kotor_scene3d::at; using kotor_scene3d::bits_to_float; using kotor_scene3d::float_to_bits;
using Texture = font::Texture;                       // CAurTexture (Safe target), lane-texture-epoch E1
using Material = material_owner::Material;           // 76-byte Material, texture-epoch material.h
using PartTable = aurora_vt::VTable_00740b68;        // PartTriMesh layout (38 slots); common prefix of all Part tables
using PrimitiveTable = aurora_vt::VTable_00742f74;   // VertexPrimitive layout (14 slots)
using ProcU = void (__stdcall*)(std::uint32_t);                       // glActiveTextureARB / glEnable|DisableVertexAttribArrayARB
using ProcUU = void (__stdcall*)(std::uint32_t, std::uint32_t);       // glBindBufferARB

// Typed dispatch views (object-pointer conversions only, D24): reload [object] at each call.
inline const PartTable* part_table(void* part) { return static_cast<const PartTable*>(field<void*>(part, 0)); }
inline const PrimitiveTable* primitive_table(void* primitive) { return static_cast<const PrimitiveTable*>(field<void*>(primitive, 0)); }
inline const texture_vtable::Head* texture_head(Texture* t) { return texture_vtable::head(t); }
inline Material* material_of(void* part) { return field<Material*>(part, 0x44); }  // Part+44 (0046FA67, 00477A52)
// Slot words returned through the U-typed lane-3d-model tables carry object addresses.
inline void* as_object(U word) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(word)); }

// Borrowed original globals with an owner outside this area (Main publishes once; pointers, never copies).
struct MeshCells {
    // GL enum cells (.data)
    const volatile U* gl_texgen_s_0078d460;         // 0x0C60 (also 0046FB10/00470590)
    const volatile U* gl_texgen_t_0078d464;         // 0x0C61
    const volatile U* gl_texgen_r_0078d468;         // 0x0C62
    const volatile U* vertex_program_0078d46c;      // 0x8620 (lane-renderer-bodies vertex_enum_0078d46c)
    const volatile U* gl_texture_cube_map_0078d478; // 0x8513 (lane-upload gl_texture_cube_map_0078d478)
    // render-path gates shared with other areas (.data, no writer instruction, no data-table reference)
    const volatile U* gate_0078e5fc;                // cold 1 (also 0048A520)
    const volatile U* gate_0078e608;                // cold 1 (also 004A7860)
    const volatile U* gate_0078e614;                // cold 1 (also 004509B0/00450AC0/00456300)
    const volatile float* scale_0078e630;           // cold 1.0f (also 0048A520)
    const volatile float* scale_0078e648;           // cold 1.0f (also 0048A520)
    const volatile U* gate_0078e650;                // cold 1 (also 0048A520)
    const volatile U* gate_0078e66c;                // cold 1 (also 00473900/0048A520)
    const volatile U* gate_0078e844;                // cold 1 (also 00484290)
    const volatile U* skin_programs_0078e8c0;       // cold 1 (also 00425520/00473900/...); D31 VP gate
    const volatile U* gate_00798a8c;                // cold 1 (also 0047B000)
    const volatile U* alpha_ref_00798a90;         // raw float DWORD (owner kotor_gui_render_cells::alpha_test_ref_00798a90 is U)       // cold 0.35f (also 0047B000)
    const volatile U* gate_00798b48;                // cold 1 (also 0048A520)
    const volatile U* gate_00798b4c;                // cold 1 (also 0048A520)
    // renderer dispatch / pool / extension procedures (lane-renderer-bodies owners)
    kotor_renderer_init::EnableDispatch const volatile* enable_dispatch_007a47e0;
    kotor_renderer_init::ConstantDispatch const volatile* constant0_dispatch_007a6820;
    kotor_renderer_init::ConstantDispatch const volatile* constant1_dispatch_007a47dc;
    kotor_renderer_gl::RequestPoolManager* const volatile* pool_007a68a4;
    ProcU const volatile* active_texture_007bb80c;           // glActiveTextureARB
    ProcUU const volatile* bind_buffer_007bb7b4;             // glBindBufferARB
    ProcU const volatile* enable_vertex_attrib_007bb79c;     // glEnableVertexAttribArrayARB
    ProcU const volatile* disable_vertex_attrib_007bb770;    // glDisableVertexAttribArrayARB
    // statistics / shared BSS
    volatile U* triangles_00827fb0;   // = &r4_lowers_cells::render_counts_00827fa8[2] (unsigned owner array)                 // += count/3 (0046F9B2..0046F9CE); also core/grass/emitters
    volatile U* draws_00827fb4;       // = &r4_lowers_cells::render_counts_00827fa8[3]                     // ++ (0046F955); also 0046EE00
    void** bss_00827fec;              // renderer_program_cells::output_00827FEC (void*)                       // shared with 0044DAB0/0044E3B0/0044EA30
    void** bss_00827ff0;
    void** bss_00827ff4;
    void** bss_00827ff8;
    void** bss_00827ffc;
    void* volatile* vertex_program_00828014;        // VertexProgram* (00477A41); written by 0044DAB0/0044EA30
    std::uint32_t* volatile* bss_00830294;   // kotor_vp_cells::parameters_00830294 (U* volatile)                   // also 0048A520
    std::uint32_t** bss_00830560;           // renderer_program_cells::parameters_00830560 (U*)                   // also VP family / grass
};
extern const MeshCells& mesh_cells;
} // namespace kotor_trimesh

// ---------------------------------------------------------------- cells owned by this area (only these 37
// functions reference them; no .data/.rdata table holds their addresses). Storage: trimesh_skin_cells.cpp.
extern "C" {
extern volatile kotor_scene3d::U gate_0078e5f4, gate_0078e5f8, gate_0078e600, gate_0078e604, gate_0078e60c,
    gate_0078e61c, gate_0078e620, gate_0078e62c, gate_0078e634, gate_0078e638, gate_0078e63c, gate_0078e640,
    gate_0078e644, gate_0078e658, gate_0078e65c, gate_0078e670, gate_0078e68c, gate_00798b68;   // .data cold 1
extern volatile kotor_scene3d::U bss_00827fcc, bss_00827fd4, bss_00827fe8, bss_00828000;
}

// ---------------------------------------------------------------- authored in this area
extern "C" {
using kotor_scene3d::U; using kotor_scene3d::I; using kotor_scene3d::Vector;
// GLRender / scene debug
void __cdecl gl_render_draw_elements_00425ab0(U mode_index, I count, U first, I pool_index);
// (render_squares_00426000: declared in scene3d.h as (void*); the word is a render-pool index, retype proposed)
// (render_personal_space_00450fa0 / render_paths_004510e0: declared in scene3d.h)
// PartTriMesh visibility helper
U __cdecl part_tri_mesh_visible_0046ee40(void* part);
// TriMesh render paths: cdecl(VertexPrimitive*)
void __cdecl render_flat_0046f900(void* primitive);
void __cdecl render_light_mapped_0046fa40(void* primitive);
void __cdecl render_environment_mapped_0046ff70(void* primitive, U arg1, U arg2);
void __cdecl render_emlm_004703f0(void* primitive);
void __cdecl render_flat_non_textured_00470750(void* primitive);
void __cdecl render_flat_decal_004707a0(void* primitive);
void __cdecl render_embm_00470860(void* primitive);
I __fastcall part_tri_mesh_get_render_path_00470d30(void* part, void*);                    // RET 0
void __cdecl render_bumpmap_fallback_004713d0(void* primitive);
void __cdecl render_light_mapped_bump_diffuse_00474ea0(void* primitive);
void __cdecl render_light_mapped_bump_specular_00475450(void* primitive);
void __cdecl render_diffuse_bump_mapped_pass_00475bb0(void* primitive);
void __cdecl render_specular_bump_mapped_pass_004760d0(void* primitive);
void __cdecl render_bump_mapped_00476690(void* primitive);
void __cdecl render_normal_mapped_00477050(void* primitive);
// Part vtable +64 Draw bodies (lane-3d-model model_vtables.cpp spelling: U __fastcall(void*, void*, U))
U __fastcall part_anim_mesh_draw_00471500(void* self, void* unused_edx, U);
U __fastcall part_tri_mesh_draw_00477830(void* self, void* unused_edx, U);
U __fastcall part_dangly_mesh_draw_00477b60(void* self, void* unused_edx, U);
U __fastcall part_skin_draw_0048d6c0(void* self, void* unused_edx, U);
void __fastcall part_skin_draw_bumped_out_shell_0048d690(void* part, void*);                // RET 0
// Material (receiver void* = consumers' spelling; Material+.. offsets cited in bodies)
// (material_bind_texture0_0047adf0 / material_set_blending_mode_0047af70: declared in scene3d.h)
void __fastcall material_bind_texture1_0047ae60(void* material, void*);
void __fastcall material_bind_env_map_0047aeb0(void* material, void*);
U __fastcall material_bump_map_is_specular_0047af50(void* material, void*);                 // full EAX (movzx)
std::uint8_t __fastcall material_has_bumpy_shiny_map_0047b0c0(void* material, void*);      // AL
std::uint8_t __fastcall material_has_env_map_0047b0e0(void* material, void*);               // AL
std::uint8_t __fastcall aurora_material_has_bump_map_0047b8f0(void* material, void* unused_edx); // AL (lane-3d-model spelling)
Vector* __fastcall material_get_bump_map_specular_color_00471430(void* material, void*, Vector* out); // RET 4 -> out
// RenderSkinned (signature fixed by its fork; see trimesh_skinned.cpp)
}

// D13 program texts (definitions: trimesh_skin_cells.cpp from the LOCAL generated header).
namespace kotor_trimesh_data {
extern char arb_skin_program_0078e8c8[1835];
extern char arb_program_0078eff8[1537];
extern char nv_program_00799dc8[1101];
extern char nv_program_0079a218[994];
extern char nv_program_0079a600[880];
extern char nv_program_0079a970[438];
extern char nv_program_0079ab28[574];
extern char nv_program_0079b358[591];
}

// ---------------------------------------------------------------- lowers outside this area (hoisted from forks F1-F4)
// Existing owner declarations are exact copies; NEW names are proposals (owner area in comment).
namespace aurora_model { struct MdlNode; }
extern "C" {
using kotor_scene3d::Quaternion; using kotor_scene3d::PointerList;
// existing owners
aurora_model::MdlNode* __fastcall aurora_node_as_dangly_0043e380(aurora_model::MdlNode* self, void* unused_edx); // lane-3d-model
void __fastcall texture_list_remove_00445080(texture::List*, void*, texture::U);          // lane-texture-epoch (CExoArrayList::Remove)
std::uint32_t __cdecl aur_texture_get_tid_00420bf0(void*);                                // lane-upload
float* __cdecl kotor_cdecl_00423bc0(float* out, const float* a, const float* b);           // lane-r4-lowers (Vector a-b)
PointerList* __fastcall light_manager_get_bump_map_lights_004a28a0(void* lights, void*);   // lane-3d-construct spelling (lights)
// NEW - core
std::uint8_t __fastcall aur_texture_is_null_00420600(void* texture, void*);                // CAurTexture::IsNull, AL
// transform_0046f6b0: scene3d.h (lane-3d-render owner)
void __cdecl gl_render_activate_texture_stage_00425250(U stage);                          // client-texture stage -> 007A6898
std::uint8_t __cdecl aur_texture_env_map_disabled_004204d0();                              // AL
U __cdecl aur_one_pass_env_map_available_0045fbf0();
void __cdecl do_env_map_transform_0046f780(void* texture, U stage);
void __cdecl undo_env_map_transform_0046f870(void* texture, U stage);
Vector* __cdecl vector_divide_00437a90(Vector* out, const Vector* v, float s);
Vector* __cdecl vector_multiply_004ab1e0(Vector* out, const Vector* a, const Vector* b);
Quaternion* __cdecl quaternion_multiply_004a9ac0(Quaternion* out, const Quaternion* a, const Quaternion* b);
void __cdecl gl_blend_func_indexed_00401670(U source_index, U destination_index);          // glBlendFunc(tbl[a],tbl[b]) tbl 0073F270
// NEW - bsp_visibility
void __cdecl gl_render_bind_texture_00420440(U target_index, U texture_name);
// NEW - framebuffer_bloom
U __cdecl aur_pixel_shaders_available_0046eda0();
// NEW - lights (LightManager vtable 00743958 slots +2C/+30)
U __fastcall light_manager_disable_bump_map_lights_004a5680(void* lights, void* unused_edx);
U __fastcall light_manager_enable_bump_map_lights_004a5710(void* lights, void* unused_edx);
// NEW - animation
void __cdecl gather_bones_00488930(void* part, PointerList* positions, PointerList* orientations,
                                   Vector translation, Quaternion rotation);              // cdecl, add esp,28h
void __cdecl execute_skin_transforms_00487570(void* node, PointerList* positions, PointerList* matrices,
                                              void* primitive, U flag);                   // cdecl, add esp,14h
void __fastcall matrix_list_allocate_00488160(PointerList* list, void*, I capacity);       // RET 4 (core per SUBLANES)
// NEW - vp_nv_ati
std::uint8_t __cdecl get_ati_fragment_shaders_disabled_0045f5a0();
std::uint8_t __cdecl aur_ati_fragment_shaders_bump_map_available_0045fb90();
void __cdecl render_light_mapped_no_shaders_0046fb10(void* primitive);
void __cdecl render_nv1x_emlm_00470590(void* primitive);
void __cdecl render_flat_vp_00470fe0(void* primitive, void* part);
void __cdecl render_lightmapped_vp_004710b0(void* primitive, void* part);
void __cdecl render_environment_mapped_vp_00471180(void* primitive, void* part);
void __cdecl render_emlm_vp_004712b0(void* primitive, void* part);
void __fastcall part_skin_draw_bumped_out_shell_vp_0048d040(void* part, void* unused_edx);
void __cdecl render_skinned_vp_0048a520(void* primitive);
void __cdecl render_skinned_vp_blur_0048c650(void* primitive);
// authored in this area (F4)
void __cdecl render_skinned_0048a0d0(void* primitive, U flag);
}
namespace kotor_trimesh {
// TABLE REQUEST (area lights owns LightManager vtable 00743958): typed prefix view, slot types per callee RET.
struct LightManagerTablePrefix {
    const void* s00_s28[11];                                         // +00..+28 (not called here)
    decltype(&light_manager_disable_bump_map_lights_004a5680) s2c;   // +2C 004A5680 DisableBumpMapLights
    decltype(&light_manager_enable_bump_map_lights_004a5710) s30;    // +30 004A5710 EnableBumpMapLights
};
static_assert(offsetof(LightManagerTablePrefix, s2c) == 0x2c && offsetof(LightManagerTablePrefix, s30) == 0x30, "slots");
}
