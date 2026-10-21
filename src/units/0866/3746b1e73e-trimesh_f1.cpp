// trimesh_f1.cpp - lane-3d-mesh fork F1: Material texture binds/queries, GLRender::DrawElements, debug draws
// (RenderSquares / RenderPersonalSpace / RenderPaths), PartTriMesh visibility helper, the small Draw slot bodies
// (PartAnimMesh / PartDanglyMesh / PartSkin), RenderFlatNonTextured / RenderFlatDecal / RenderBumpmapFallback.
// Every statement cites the original instruction(s) it reproduces (swkotor.exe GOG K1, dumpbin listings in d/).
#include "16aff3eca3-trimesh_skin.h"
#include "4282d8e4cc-ini_c_primitives.h" // stage header_root <stage>/modern-ini-support; kotor_functional::modern_ini_sprintf (M40 binding of CRT sprintf 006FADB0)

using namespace kotor_trimesh;
namespace gl = kotor_renderer_init;
using kotor_scene3d::cells;

// LOWERS hoisted to trimesh_skin.h (lane merge)
// lane-renderer-bodies extension_loader.h (exact copy)
namespace kotor_renderer_init { extern U32 __stdcall VertexBufferAvailable_0045fc90(); }

namespace {
// .rdata 0073F270 blend-factor table (8 words; 0047AF7E / 0047AF8C index it with Material state/mode words;
// the following words 0073F290.. belong to the next table, so the reachable domain is 0..7).
constexpr U kBlendFactor_0073f270[8] = {0x302u, 0x303u, 0x1u, 0x0u, 0x300u, 0x306u, 0x305u, 0x304u};
// .rdata 0073F334 primitive-mode table (10 words; 00425AD0; 0073F35C.. is the next table).
constexpr U kPrimitiveMode_0073f334[10] = {0x0u, 0x1u, 0x3u, 0x4u, 0x5u, 0x6u, 0x7u, 0x8u, 0x9u, 0x2u};
constexpr U kTexCoordArray_0073f398 = 0x8078u;  // .rdata GL_TEXTURE_COORD_ARRAY
constexpr U kNormalArray_0073f39c = 0x8075u;    // .rdata GL_NORMAL_ARRAY
constexpr U kColorArray_0073f3a0 = 0x8076u;     // .rdata GL_COLOR_ARRAY
constexpr U kVertexArray_0073f3a4 = 0x8074u;    // .rdata GL_VERTEX_ARRAY
constexpr U kGreater_0073f2b0 = 0x204u;         // .rdata GL_GREATER
constexpr U kOne_0073f278 = 0x1u, kZero_0073f27c = 0x0u;           // .rdata blend ONE / ZERO
constexpr U kSrcAlpha_0073f270 = 0x302u, kOneMinusSrcAlpha_0073f274 = 0x303u;
constexpr U kZeroFloat_0073d700 = 0x00000000u;  // .rdata 0.0f (fcomp operand)
constexpr U kMinAlpha_0073d708 = 0x3A83126Fu;   // .rdata 0.001f (fcomp operand)

inline Texture* safe_target(Material* m, int slot) { return m->textures[slot].target; }
inline I list_word(const texture::List& list, I index) { return reinterpret_cast<const volatile I*>(list.data)[index]; }

// Shared body of BindTexture0/1/BindEnvMap up to the GetNULL/Release check (0047ADF0..0047AE1E pattern).
inline void check_referenced(Texture* t) {
    Texture* const referenced = texture_head(t)->common.s08(t, nullptr);   // call [eax+8]
    if (referenced == nullptr) {                                            // test edi,edi; jne
        texture_join::get_null_004230c0();                                  // call 004230C0 (EAX dropped)
        return;
    }
    if (texture_head(referenced)->common.s10(referenced, nullptr) != 0) {   // call [edx+10h]; test al,al
        texture_join::get_null_004230c0();                                  // call 004230C0 (EAX dropped)
        font::release_00420d70(referenced);                                 // push edi; call 00420D70
    }
}
inline void bind(Texture* t) { texture_vtable::bind_tail(t)->sb4(t, nullptr); } // call [vtbl+0B4h]
} // namespace

extern "C" {
// ================================================================ Material
// 0047ADF0 Material::BindTexture0 (thiscall, RET 0). EAX on return = AurTextureGetTID result (not used by
// the scene3d.h `void` spelling; callers 00455710/0046FB10/00478260/... discard it - see F1.json).
void __fastcall material_bind_texture0_0047adf0(void* material, void*) {
    Material* const m = static_cast<Material*>(material);
    check_referenced(safe_target(m, 0));                                    // 0047ADF3..0047AE1E ([esi])
    if (*mesh_cells.gate_0078e5fc != 0 && safe_target(m, 6) != nullptr) {  // 0047AE21..0047AE2F ([esi+18h])
        bind(safe_target(m, 6));                                            // 0047AE31..0047AE33
        aur_texture_get_tid_00420bf0(safe_target(m, 0));                    // 0047AE39..0047AE3C
        return;
    }
    bind(safe_target(m, 0));                                                // 0047AE47..0047AE4B
    aur_texture_get_tid_00420bf0(safe_target(m, 0));                        // 0047AE51..0047AE54
}
// 0047AE60 Material::BindTexture1 (thiscall, RET 0): texture slot [esi+4].
void __fastcall material_bind_texture1_0047ae60(void* material, void*) {
    Material* const m = static_cast<Material*>(material);
    check_referenced(safe_target(m, 1));                                    // 0047AE63..0047AE8F
    bind(safe_target(m, 1));                                                // 0047AE92..0047AE97
    aur_texture_get_tid_00420bf0(safe_target(m, 1));                        // 0047AE9D..0047AEA1
}
// 0047AEB0 Material::BindEnvMap (thiscall, RET 0): texture slot [esi+14h].
void __fastcall material_bind_env_map_0047aeb0(void* material, void*) {
    Material* const m = static_cast<Material*>(material);
    check_referenced(safe_target(m, 5));                                    // 0047AEB3..0047AEDF
    bind(safe_target(m, 5));                                                // 0047AEE2..0047AEE7
    aur_texture_get_tid_00420bf0(safe_target(m, 5));                        // 0047AEED..0047AEF1
}
// 0047AF50 Material::BumpMapIsSpecular (thiscall, RET 0): movzx eax,al of slot +18 on [ecx+10h].
U __fastcall material_bump_map_is_specular_0047af50(void* material, void*) {
    Texture* const t = safe_target(static_cast<Material*>(material), 4);    // 0047AF50
    if (t == nullptr) return 0;                                             // 0047AF53..0047AF55 / 0047AF60
    return static_cast<std::uint8_t>(texture_head(t)->common.s18(t, nullptr)); // 0047AF57..0047AF5C (AL only)
}
// 0047AF70 Material::SetBlendingMode (thiscall, RET 4).
void __fastcall material_set_blending_mode_0047af70(void* material, void*, U mode) {
    Material* const m = static_cast<Material*>(material);
    const I index = static_cast<I>(mode);                                   // 0047AF77 edi = [esp+0Ch]
    const U dst = kBlendFactor_0073f270[list_word(m->modes, index)];        // 0047AF73..0047AF7E ([esi+34h])
    const U src = kBlendFactor_0073f270[list_word(m->states, index)];       // 0047AF85..0047AF8C ([esi+28h])
    gl::glBlendFunc_0073d38c(src, dst);                                     // 0047AF94
    const std::uint8_t depth_write =
        (list_word(m->states, index) == 0 && list_word(m->modes, index) == 2) ? 0 : 1; // 0047AF9A..0047AFB0
    gl::glDepthMask_0073d28c(depth_write);                                  // 0047AFB2
    if (*mesh_cells.gate_00798a8c == 0) return;                             // 0047AFB8..0047AFBF
    if (list_word(m->states, index) == 2 && list_word(m->modes, index) == 3) { // 0047AFC1..0047AFD1
        gl::glAlphaFunc_0073d238(kGreater_0073f2b0, bits_to_float(*mesh_cells.alpha_ref_00798a90)); // 0047AFD3..0047AFE1 push dword [00798A90] (raw DWORD reinterpreted as the float arg; lane-m48-a: no value conversion)
        return;
    }
    gl::glAlphaFunc_0073d238(kGreater_0073f2b0, bits_to_float(0u));         // 0047AFEC..0047AFF4 (push 0)
}
// 0047B0C0 Material::HasBumpyShinyMap (thiscall, RET 0, AL): slot +0C (IsCubeMap) of [ecx+14h].
std::uint8_t __fastcall material_has_bumpy_shiny_map_0047b0c0(void* material, void*) {
    Texture* const t = safe_target(static_cast<Material*>(material), 5);    // 0047B0C0
    if (t != nullptr && texture_head(t)->common.s0c(t, nullptr) != 0) return 1; // 0047B0C3..0047B0D0
    return 0;                                                               // 0047B0D3
}
// 0047B0E0 Material::HasEnvMap (thiscall, RET 0, AL).
std::uint8_t __fastcall material_has_env_map_0047b0e0(void* material, void*) {
    Material* const m = static_cast<Material*>(material);
    Texture* const t = safe_target(m, 5);                                   // 0047B0E3
    if (t == nullptr) return 0;                                             // 0047B0E6..0047B0E8
    if (aur_texture_is_null_00420600(t, nullptr) != 0) return 0;            // 0047B0EA..0047B0F1
    Texture* const again = safe_target(m, 5);                               // 0047B0F3 (reloaded)
    if (texture_head(again)->common.s10(again, nullptr) != 0) return 0;     // 0047B0F6..0047B0FD
    return 1;                                                               // 0047B0FF
}
// 0047B8F0 Material::HasBumpMap (thiscall, RET 0, AL). Invalid bump maps raise the original message box and
// drop the texture from the material ([esi]=0) after unregistering the Safe slot from the texture's list.
std::uint8_t __fastcall aurora_material_has_bump_map_0047b8f0(void* material, void* /*unused_edx*/) {
    char message[0x100];                                                    // 0047B8F0 sub esp,100h
    font::Safe* const slot = &static_cast<Material*>(material)->textures[4]; // 0047B8F7 lea esi,[ecx+10h]
    Texture* const t = slot->target;                                        // 0047B8FA
    if (t == nullptr) return 0;                                             // 0047B8FC..0047B8FE -> 0047B967
    if (texture_head(slot->target)->common.s28(slot->target, nullptr) == 0) return 1; // 0047B900..0047B907 (IsFullyLoaded; test al,al / je 0047B971: not yet loaded -> 1)
    if (static_cast<std::uint8_t>(texture_head(slot->target)->common.s1c(slot->target, nullptr)) != 0) return 1; // 0047B909..0047B912 (AL)
    char* const name = texture_head(slot->target)->common.s40(slot->target, nullptr); // 0047B914..0047B919 GetName
    kotor_functional::modern_ini_sprintf(message, "Invalid bumpmap: %s.tga", name); // 0047B91C..0047B927 (fmt .rdata 007430D4)
    gl::MessageBoxA_0073d418(nullptr, message, "Invalid Bumpmap!", 0x1030u);  // 0047B92F..0047B940 (caption .rdata 007430C0)
    Texture* const held = slot->target;                                     // 0047B946 edi = [esi]
    if (held != nullptr)                                                    // 0047B948..0047B94C
        texture_list_remove_00445080(&held->registrations, nullptr,         // 0047B94E..0047B952 lea ecx,[eax+4]
            static_cast<texture::U>(reinterpret_cast<std::uintptr_t>(slot)));//   push esi (Safe slot address)
    slot->target = nullptr;                                                 // 0047B958
    font::release_00420d70(held);                                           // 0047B957 push edi / 0047B95E
    return 0;                                                               // 0047B967
}
// 00471430 Material::GetBumpMapSpecularColor (thiscall, RET 4 -> out): texture slot +88 on [ecx+10h] (no null test).
Vector* __fastcall material_get_bump_map_specular_color_00471430(void* material, void*, Vector* out) {
    Texture* const t = safe_target(static_cast<Material*>(material), 4);    // 00471430
    texture_head(t)->common.s88(t, nullptr, out);                           // 00471433..0047143B
    return out;                                                             // 00471441 mov eax,esi
}

// ================================================================ GLRender / debug draws
// 00425AB0 GLRender::DrawElements (cdecl): index pool from 007A68A4 slot +14 (GetIndexPool).
void __cdecl gl_render_draw_elements_00425ab0(U mode_index, I count, U first, I pool_index) {
    kotor_renderer_gl::RequestPoolManager* const pool = *mesh_cells.pool_007a68a4;  // 00425AB0
    unsigned char* const indices = static_cast<unsigned char*>(pool->GetIndexPool(pool_index)); // 00425AB6..00425ABD
    gl::glDrawElements_0073d258(kPrimitiveMode_0073f334[mode_index], count, 0x1403u,  // 00425AC0..00425ADE
        indices + first * 2u);                                              //   lea edx,[eax+ecx*2]
}
// 00426000 RenderSquares: NOT defined here (lane-m48-a adaptation). Single owner = canonical lane-3d-render scene3d.cpp
// render_squares_00426000(U pool_index) (scene3d.h:395, staged unit 3d-render-scene3d); this fork's void* copy (C2733) dropped.
// 00450FA0 RenderPersonalSpace / 004510E0 RenderPaths: reassigned to lane-3d-render core (SUBLANES.json update; scene3d.cpp owner). Not defined here.

// ================================================================ PartTriMesh helpers / Draw slot bodies
// 0046EE40 (PartTriMesh visibility test, cdecl(part) -> 0/1).
U __cdecl part_tri_mesh_visible_0046ee40(void* part) {
    void* const node = field<void*>(part, 4);                               // 0046EE4A ecx = [eax+4]
    // fld [eax+84h]; fcomp 0.001f; test ah,41h; jp continue: continue when greater or unordered.
    if (static_cast<double>(field<float>(part, 0x84)) <= static_cast<double>(bits_to_float(kMinAlpha_0073d708))) return 0; // 0046EE44..0046EE5C
    if (gate_0078e60c != 0 && field<std::uint8_t>(node, 0x189) == 0) return 0;  // 0046EE5D..0046EE6E
    if (bss_00827fcc != 0 && field<U>(node, 0xA4) != 0) return 0;           // 0046EE70..0046EE81
    if (field<U16>(node, 0x180) == 0) return 0;                             // 0046EE83..0046EE8B
    return 1;                                                               // 0046EE8D
}
// 00471500 PartAnimMesh::Draw (Part vtbl+64, thiscall RET 4; stack word unused).
U __fastcall part_anim_mesh_draw_00471500(void* self, void* /*unused_edx*/, U) {
    if (field<U>(self, 0x40) == 0) {                                        // 00471503..00471508
        gl::glPushMatrix_0073d300();                                        // 0047150A
        if (*cells.transform_enabled_0078e5f0 != 0)                         // 00471510..00471517
            gl_transform_0046f060(&field<Vector>(self, 8), &field<Quaternion>(self, 0x14), 0); // 00471519..00471523
    }
    void* const primitive = field<void*>(self, 0x70);                       // 0047152B
    primitive_table(primitive)->s018(primitive, nullptr);                   // 0047152E..00471530 call [edx+18h]
    void* const primitive_again = field<void*>(self, 0x70);                 // 00471533 (reloaded)
    render_flat_0046f900(as_object(primitive_table(primitive_again)->s004(primitive_again, nullptr))); // 00471536..0047153C
    const U attached = field<U>(self, 0x40);                                // 00471541
    if (attached == 0) gl::glPopMatrix_0073d2fc();                          // 00471547..0047154C
    return attached;  // EAX = [esi+40h] on the no-pop path; after glPopMatrix EAX is unspecified (callers discard)
}
// 00477B60 PartDanglyMesh::Draw (Part vtbl+64, thiscall RET 4): node cast result is discarded.
U __fastcall part_dangly_mesh_draw_00477b60(void* self, void* /*unused_edx*/, U flag) {
    const U gate = *mesh_cells.gate_0078e844;                               // 00477B60
    if (gate == 0) return gate;                                             // 00477B65..00477B6A (EAX = 0)
    aurora_node_as_dangly_0043e380(field<aurora_model::MdlNode*>(self, 4), nullptr); // 00477B6C..00477B6F
    return part_tri_mesh_draw_00477830(self, nullptr, flag);                // 00477B74..00477B7B
}
// 0048D690 PartSkin::DrawBumpedOutShell (thiscall, RET 0).
void __fastcall part_skin_draw_bumped_out_shell_0048d690(void* part, void*) {
    if (aur_nv2x_level_shaders_available_0045fbd0()) {                     // 0048D693..0048D69A (AL)
        part_skin_draw_bumped_out_shell_vp_0048d040(part, nullptr);         // 0048D69C..0048D69F tail jmp
        return;
    }
    void* const primitive = field<void*>(part, 0x70);                       // 0048D6A4
    render_skinned_0048a0d0(as_object(primitive_table(primitive)->s008(primitive, nullptr)), 1); // 0048D6A7..0048D6AF
}
// 0048D6C0 PartSkin::Draw (Part vtbl+64, thiscall RET 4; [esp+8] low byte = blur flag).
U __fastcall part_skin_draw_0048d6c0(void* self, void* /*unused_edx*/, U flag) {
    if (gate_00798b68 == 0) return 0;                                       // 0048D6C0..0048D6CA (EAX = gate = 0)
    if (*mesh_cells.skin_programs_0078e8c0 != 0 &&                          // 0048D6D0..0048D6D7
        aur_nv2x_level_shaders_available_0045fbd0() &&                      // 0048D6D9..0048D6E0
        *cells.active_scene_007b92a0 != nullptr) {                          // 0048D6E2..0048D6E9
        void* const primitive = field<void*>(self, 0x70);                   // 0048D6F1
        if (static_cast<std::uint8_t>(flag) == 0)                           // 0048D6EB..0048D6F4
            render_skinned_vp_0048a520(as_object(primitive_table(primitive)->s008(primitive, nullptr)));      // 0048D6F6..0048D6FC
        else
            render_skinned_vp_blur_0048c650(as_object(primitive_table(primitive)->s008(primitive, nullptr))); // 0048D706..0048D70C
    } else {
        gl::glDisableClientState_0073d264(kColorArray_0073f3a0);            // 0048D716..0048D71C
        void* const primitive = field<void*>(self, 0x70);                   // 0048D722
        render_skinned_0048a0d0(as_object(primitive_table(primitive)->s008(primitive, nullptr)), 0); // 0048D725..0048D72D
    }
    void* const shell = field<void*>(self, 0x40);                           // 0048D735
    if (shell == nullptr) return 0;                                         // 0048D738..0048D73A
    // fld [eax+11Ch]; fcomp 0.0f; test ah,44h; jnp skip: skip only when exactly equal (NaN draws).
    if (static_cast<double>(field<float>(shell, 0x11C)) == static_cast<double>(bits_to_float(kZeroFloat_0073d700))) return 0; // 0048D73C..0048D74D
    if (*cells.active_scene_007b92a0 == nullptr) return 0;                  // 0048D74F..0048D756
    part_skin_draw_bumped_out_shell_0048d690(self, nullptr);                // 0048D758..0048D75A
    return 0;  // D2-style label: EAX at 0048D760 is an unconsumed scratch value (callers of +64 discard it)
}

// ================================================================ simple render paths
// 00470750 RenderFlatNonTextured (cdecl, primitive).
void __cdecl render_flat_non_textured_00470750(void* primitive) {
    if (*mesh_cells.gate_0078e614 == 0) return;                             // 00470750..00470757
    gl::glDisableClientState_0073d264(kTexCoordArray_0073f398);             // 00470759..0047075F
    gl::glDisable_0073d22c(*kotor_scene3d::cells.gl_texture_2d_0078d458);             // 00470765..0047076C
    render_flat_0046f900(primitive);                                        // 00470772..00470777
    gl::glEnable_0073d390(*kotor_scene3d::cells.gl_texture_2d_0078d458);              // 0047077C..00470785
    gl::glEnableClientState_0073d260(kTexCoordArray_0073f398);              // 0047078B..00470792
}
// 004707A0 RenderFlatDecal (cdecl, primitive).
void __cdecl render_flat_decal_004707a0(void* primitive) {
    if (*mesh_cells.gate_0078e614 == 0) return;                             // 004707A0..004707A7
    if (*cells.active_scene_007b92a0 == nullptr) { render_flat_0046f900(primitive); return; } // 004707AD..004707B6 tail jmp
    material_bind_texture0_0047adf0(material_of(field<void*>(primitive, 8)), nullptr); // 004707BC..004707C8
    Texture* const texture0 = material_of(field<void*>(primitive, 8))->textures[0].target; // 004707CD..004707D3
    (*mesh_cells.enable_dispatch_007a47e0)(1u, reinterpret_cast<texture::Basic*>(texture0), nullptr, nullptr, nullptr); // 004707D5..004707DE
    if (aur_multi_texture_available_0045f710() != 0)                        // 004707E7..004707EE
        (*cells.client_active_texture_007bb6b0)(0x84C0u);                   // 004707F0..004707F5
    *cells.current_stage_007a6898 = 0;                                      // 00470808
    gl::glEnableClientState_0073d260(kTexCoordArray_0073f398);              // 004707FB..00470812
    gl::glEnableClientState_0073d260(kVertexArray_0073f3a4);                // 00470814..0047081A
    gl::glDisableClientState_0073d264(kNormalArray_0073f39c);               // 0047081C..00470829
    gl::glDisableClientState_0073d264(kColorArray_0073f3a0);                // 0047082B..00470832
    render_flat_0046f900(primitive);                                        // 00470834..00470835
    gl::glEnableClientState_0073d260(kNormalArray_0073f39c);                // 0047083A..00470843
    gl::glEnableClientState_0073d260(kColorArray_0073f3a0);                 // 00470845..0047084C
}
// 004713D0 RenderBumpmapFallback (cdecl): argument is PartTriMesh+70 (VertexPrimitive) pushed raw (00477915).
void __cdecl render_bumpmap_fallback_004713d0(void* primitive) {
    gl::glDisable_0073d22c(*cells.gl_alpha_test_0078d44c);                  // 004713D0..004713D7
    gl::glBlendFunc_0073d38c(kOne_0073f278, kZero_0073f27c);                // 004713DD..004713F1
    render_flat_0046f900(as_object(primitive_table(primitive)->s004(primitive, nullptr))); // 004713F3..004713FD
    gl::glBlendFunc_0073d38c(kSrcAlpha_0073f270, kOneMinusSrcAlpha_0073f274); // 00471402..00471413
    gl::glEnable_0073d390(*cells.gl_alpha_test_0078d44c);                   // 00471415..0047141B
}
} // extern "C"
