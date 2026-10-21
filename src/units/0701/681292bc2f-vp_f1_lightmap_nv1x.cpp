// vp_f1_lightmap_nv1x.cpp - lane-3d-vp fork F1 (SUBLANES "vp_nv_ati"):
//   0046FB10 RenderLightMappedNoShaders   (cdecl(VertexPrimitive*), plain RET; caller 00477830)
//   00470590 RenderNV1xEMLM               (cdecl(VertexPrimitive*), plain RET; caller 00477830)
// swkotor.exe GOG K1 (base 00400000). Every statement cites the original instruction(s) it reproduces.
// Object views (offsets observed; no published layout):
//   VertexPrimitive: +08 owner object whose Part-table slot +2C returns the Part (0046FB24..0046FB2B, 004705A5..004705AB)
//   Part:            +04 MdlNode (0046FB30), +40 word (004705B0), +44 Material (0046FB38)
//   Material:        +00 texture0, +04 texture1 (0046FB3B / 0046FB4F), +14 env-map texture (004705F0)
//   MdlNode (trimesh): +08 owner tree (0046FC4A), +14C/+150 words (0046FC5D/0046FC67), +180 s16 (0046FC3F),
//                    +194 word (0046FC39); Model (AsModel result) +AC word (0046FC57)
#include "4bf744a1f4-vp_nv_ati.h"
#include "7ee14b5ec3-model_vtables.hpp"   // lane-3d-model generated typed Part tables (D39/D41)

// ---------------------------------------------------------------- LOWERS (exact copies of existing declarations)
namespace aurora_model { struct MdlNode; }
namespace kotor_renderer_init {
extern void __stdcall glDisable_0073d22c(U32);   // scene3d.h / M40 adapter owner
extern void __stdcall glEnable_0073d390(U32);
}
namespace kotor_renderer_cells {   // lane-gui-draw writers/source/gl_enum_cells.h + gui_cells.cpp (sole owner)
extern std::uint32_t enable_0078d44c; // GL_ALPHA_TEST  0x0BC0
extern std::uint32_t enable_0078d458; // GL_TEXTURE_2D  0x0DE1
extern std::uint32_t disable_0078d45c;// GL_LIGHTING    0x0B50
}
extern "C" {
using kotor_scene3d::U; using kotor_scene3d::I;
// lane-3d-model model_load.hpp
aurora_model::MaxTree* __fastcall aurora_tree_as_model_0043e1c0(aurora_model::MaxTree* self, void* unused_edx);
aurora_model::MdlNode* __fastcall aurora_node_as_trimesh_0043e400(aurora_model::MdlNode* self, void* unused_edx);
// lane-3d-mesh trimesh_skin.h (authored there / hoisted lowers block)
void __cdecl render_flat_0046f900(void* primitive);
void __fastcall material_bind_texture0_0047adf0(void* material, void*);       // == scene3d.h
void __fastcall material_bind_texture1_0047ae60(void* material, void*);
void __fastcall material_bind_env_map_0047aeb0(void* material, void*);
std::uint8_t __fastcall material_has_env_map_0047b0e0(void* material, void*);   // AL
std::uint8_t __fastcall aur_texture_is_null_00420600(void* texture, void*);     // CAurTexture::IsNull, AL
void __cdecl gl_render_activate_texture_stage_00425250(U stage);               // client-texture stage -> 007A6898
void __cdecl do_env_map_transform_0046f780(void* texture, U stage);
void __cdecl undo_env_map_transform_0046f870(void* texture, U stage);
void __cdecl gl_blend_func_indexed_00401670(U source_index, U destination_index); // glBlendFunc(tbl[a],tbl[b]) tbl 0073F270
// ---------------------------------------------------------------- LOWERS (NEW names; owner sublane "core")
// 00425280 GLRender::SetLightmapTexCoordToStage0 / 004253D0 GLRender::SetDecalTexCoordToStage0: cdecl, 6 stack words,
// plain RET. 00425280 reads [esp+0C] pool index (00425296), [esp+18] node (004252C0), [esp+1C] byte offset (004252C4);
// the first two words are not read by 00425280.
void __cdecl gl_render_set_lightmap_tex_coord_to_stage0_00425280(U word_14c, U word_150, U pool_index, I count,
    void* node, U offset);
void __cdecl gl_render_set_decal_tex_coord_to_stage0_004253d0(U word_14c, U word_150, U pool_index, I count,
    void* node, U offset);
}
// ---------------------------------------------------------------- CELLS NEEDED (no owner symbol yet; declared, not defined)
// .data GL texgen enum cells (cold 0x0C60/0x0C61/0x0C62, no writer; also read by lane-3d-mesh / others).
// Proposed owner: lane-gui-draw gl_enum_cells.cpp (neighbouring 0078D448..0078D474 owner).
namespace kotor_renderer_cells {
extern std::uint32_t enable_0078d460; // GL_TEXTURE_GEN_S 0x0C60
extern std::uint32_t enable_0078d464; // GL_TEXTURE_GEN_T 0x0C61
extern std::uint32_t enable_0078d468; // GL_TEXTURE_GEN_R 0x0C62
}

namespace {
using namespace kotor_vp;
using namespace kotor_renderer_init;
using kotor_scene3d::cells;
using PartTable = aurora_vt::VTable_00740b68;   // PartTriMesh table (common prefix of all Part tables; slot +2C)

// .rdata words read through memory operands (values from the pinned image)
constexpr U kTexCoordArray_0073f398 = 0x8078;   // GL_TEXTURE_COORD_ARRAY (0046FC2A / 00470629)
constexpr U kTextureEnv = 0x2300, kTextureEnvMode = 0x2200, kModulate = 0x2100, kDecal = 0x2101;   // immediates
constexpr U kS = 0x2000, kT = 0x2001, kR = 0x2002, kTextureGenMode = 0x2500, kReflectionMap = 0x8512;
constexpr U kOneF = 0x3F800000;                   // push 3F800000h

inline U load(const std::uint32_t& cell) { return *static_cast<const volatile std::uint32_t*>(&cell); }
inline void* primitive_part(void* primitive) {     // mov ecx,[prim+8]; mov eax,[ecx]; call [eax+2Ch]
    void* const owner = field<void*>(primitive, 8);
    const PartTable* table = *static_cast<const PartTable* const volatile*>(owner);
    return as_object(table->s02c(owner, nullptr));
}
inline void* material_of(void* part) { return field<void*>(part, 0x44); }   // mov reg,[part+44h] (reloaded per use)
inline bool texture_null(void* texture) { return aur_texture_is_null_00420600(texture, nullptr) != 0; }   // AL test
inline void* tree_model(void* node) {               // mov ecx,[node+8]; call 0043E1C0
    return aurora_tree_as_model_0043e1c0(field<aurora_model::MaxTree*>(node, 8), nullptr);
}
// Shared lightmap+decal coordinate setup (0046FC39..0046FCB7 / 0046FDA8..0046FE29): both calls read
// [node+194], movsx [node+180]+1, AsModel([node+8])+AC, [node+150], [node+14C] in this order.
inline void set_lightmap_coords(void* node) {
    const U offset = field<U>(node, 0x194);                                  // 0046FC39 / 0046FDAF
    const I count = static_cast<I>(field<std::int16_t>(node, 0x180)) + 1;    // 0046FC3F movsx, 0046FC4D inc
    const U pool = field<U>(tree_model(node), 0xAC);                        // 0046FC4E..0046FC57
    gl_render_set_lightmap_tex_coord_to_stage0_00425280(field<U>(node, 0x14C), field<U>(node, 0x150), pool, count,
        node, offset);                                                       // 0046FC5D..0046FC6F
}
inline void set_decal_coords(void* node) {
    const U offset = field<U>(node, 0x194);                                  // 0046FC85 / 0046FDF7
    const I count = static_cast<I>(field<std::int16_t>(node, 0x180)) + 1;    // 0046FC7E movsx, 0046FC95 inc
    const U pool = field<U>(tree_model(node), 0xAC);                        // 0046FC96..0046FC9F
    gl_render_set_decal_tex_coord_to_stage0_004253d0(field<U>(node, 0x14C), field<U>(node, 0x150), pool, count,
        node, offset);                                                       // 0046FCA5..0046FCB7
}
inline texture::Basic* basic(void* texture) { return static_cast<texture::Basic*>(texture); }   // object pointer (D24)
inline void enable_dispatch(U op, void* a, void* b, void* c, void* d) {     // call dword ptr [007A47E0]
    const auto fn = *static_cast<const volatile kotor_renderer_init::EnableDispatch*>(
        &kotor_renderer_gl_cells::enable_dispatch_007a47e0);
    fn(op, basic(a), basic(b), basic(c), basic(d));
}
}

// 0046FB10 RenderLightMappedNoShaders
extern "C" void __cdecl render_light_mapped_no_shaders_0046fb10(void* primitive) {
    if (kotor_vp_cells::gate_0078e628 == 0) return;                          // 0046FB11..0046FB19 -> 0046FF65 (no draw)
    void* const part = primitive_part(primitive);                             // 0046FB20..0046FB2E (edi)
    void* const node = aurora_node_as_trimesh_0043e400(field<aurora_model::MdlNode*>(part, 4), nullptr); // 0046FB30..0046FB3D (esi)
    if (texture_null(field<void*>(material_of(part), 0)) ||                  // 0046FB38..0046FB46
        texture_null(field<void*>(material_of(part), 4)) ||                  // 0046FB4C..0046FB59
        *cells.active_scene_007b92a0 == nullptr) {                            // 0046FB5F..0046FB66
        render_flat_0046f900(primitive);                                      // 0046FF59..0046FF5F
        return;
    }
    if (kotor_renderer_gl::MultitextureAvailable_0045f710() != 0 &&          // 0046FB6C..0046FB73
        material_has_env_map_0047b0e0(material_of(part), nullptr) == 0) {     // 0046FB79..0046FB83
        // single pass: lightmap on stage 0 (decal), base texture on stage 1 (modulate)
        glColor4f_0073d224(F(kOneF), F(kOneF), F(kOneF), F(kOneF));          // 0046FB89..0046FB9D
        glDisable_0073d22c(load(kotor_renderer_cells::disable_0078d45c));     // 0046FBA3..0046FBA9
        gl_blend_func_indexed_00401670(0, 1);                                // 0046FBAF..0046FBB3
        kotor_renderer_gl::ActivateTextureStage_0041fe80(0);                 // 0046FBB8..0046FBBA
        glEnable_0073d390(load(kotor_renderer_cells::enable_0078d458));       // 0046FBBF..0046FBCF
        material_bind_texture1_0047ae60(material_of(part), nullptr);         // 0046FBD1..0046FBD4
        glTexEnvi_0073d2f4(kTextureEnv, kTextureEnvMode, static_cast<I>(kDecal));   // 0046FBD9..0046FBEE
        kotor_renderer_gl::ActivateTextureStage_0041fe80(1);                 // 0046FBF0..0046FBF2
        glEnable_0073d390(load(kotor_renderer_cells::enable_0078d458));       // 0046FBF7..0046FC01
        material_bind_texture0_0047adf0(material_of(part), nullptr);         // 0046FC03..0046FC06
        glTexEnvi_0073d2f4(kTextureEnv, kTextureEnvMode, static_cast<I>(kModulate)); // 0046FC0B..0046FC1A
        kotor_renderer_gl::ActivateTextureStage_0041fe80(0);                 // 0046FC1C..0046FC1E
        gl_render_activate_texture_stage_00425250(1);                         // 0046FC23..0046FC25
        glEnableClientState_0073d260(kTexCoordArray_0073f398);                // 0046FC2A..0046FC33
        set_lightmap_coords(node);                                            // 0046FC39..0046FC6F
        render_flat_0046f900(primitive);                                      // 0046FC74..0046FC79 ([esp+30h] = arg)
        set_decal_coords(node);                                               // 0046FC7E..0046FCB7
        gl_render_activate_texture_stage_00425250(1);                         // 0046FCBC..0046FCBE
        glDisableClientState_0073d264(kTexCoordArray_0073f398);               // 0046FCC3..0046FCCD
        gl_render_activate_texture_stage_00425250(0);                         // 0046FCD3..0046FCD5
        kotor_renderer_gl::ActivateTextureStage_0041fe80(1);                 // 0046FCDA..0046FCDC
        glTexEnvi_0073d2f4(kTextureEnv, kTextureEnvMode, static_cast<I>(kModulate)); // 0046FCE4..0046FCF3
        glDisable_0073d22c(load(kotor_renderer_cells::enable_0078d458));      // 0046FCF5..0046FCFC
        kotor_renderer_gl::ActivateTextureStage_0041fe80(0);                 // 0046FD02..0046FD04
        glTexEnvi_0073d2f4(kTextureEnv, kTextureEnvMode, static_cast<I>(kModulate)); // 0046FD0C..0046FD1B
        material_bind_texture0_0047adf0(material_of(part), nullptr);         // 0046FD1D..0046FD20
        gl_blend_func_indexed_00401670(0, 1);                                // 0046FD25..0046FD29
        glColor4f_0073d224(0.0f, 0.0f, 0.0f, F(kOneF));                     // 0046FD31..0046FD3C
        glEnable_0073d390(load(kotor_renderer_cells::disable_0078d45c));      // 0046FD42..0046FD48
        return;                                                               // 0046FD4E..0046FD53
    }
    // multipass: base, lightmap (blend), optional cube env map
    glColor4f_0073d224(F(kOneF), F(kOneF), F(kOneF), F(kOneF));              // 0046FD54..0046FD68
    glDisable_0073d22c(load(kotor_renderer_cells::disable_0078d45c));         // 0046FD6E..0046FD7B
    material_bind_texture0_0047adf0(material_of(part), nullptr);             // 0046FD7D..0046FD80
    gl_blend_func_indexed_00401670(2, 3);                                    // 0046FD85..0046FD89
    render_flat_0046f900(primitive);                                          // 0046FD8E..0046FD8F (ebx)
    material_bind_texture1_0047ae60(material_of(part), nullptr);             // 0046FD94..0046FD9A
    gl_blend_func_indexed_00401670(5, 3);                                    // 0046FD9F..0046FDA3
    set_lightmap_coords(node);                                                // 0046FDA8..0046FDE1
    render_flat_0046f900(primitive);                                          // 0046FDE6..0046FDEB ([esp+30h] = arg)
    set_decal_coords(node);                                                   // 0046FDF0..0046FE29
    // esi = glEnable adapter from 0046FE3B on (both branches below use it)
    if (material_has_env_map_0047b0e0(material_of(part), nullptr) != 0 &&   // 0046FE2E..0046FE41
        kotor_renderer_init::CubeMapAvailable_0045f8b0() != 0) {              // 0046FE47..0046FE4E
        glDisable_0073d22c(load(kotor_renderer_cells::enable_0078d44c));      // 0046FE54..0046FE5B (ebp)
        material_bind_texture0_0047adf0(material_of(part), nullptr);         // 0046FE5D..0046FE60
        gl_blend_func_indexed_00401670(3, 2);                                // 0046FE65..0046FE69
        render_flat_0046f900(primitive);                                      // 0046FE6E..0046FE73 ([esp+20h] = arg)
        material_bind_env_map_0047aeb0(material_of(part), nullptr);          // 0046FE78..0046FE7E
        gl_blend_func_indexed_00401670(6, 2);                                // 0046FE83..0046FE87
        do_env_map_transform_0046f780(field<void*>(material_of(part), 0x14), 0); // 0046FE8C..0046FE95
        glTexGeni_0073d314(kS, kTextureGenMode, static_cast<I>(kReflectionMap)); // 0046FE9A..0046FEB2
        glTexGeni_0073d314(kT, kTextureGenMode, static_cast<I>(kReflectionMap)); // 0046FEB4..0046FEC3
        glTexGeni_0073d314(kR, kTextureGenMode, static_cast<I>(kReflectionMap)); // 0046FEC5..0046FED4
        glEnable_0073d390(load(kotor_renderer_cells::enable_0078d460));       // 0046FED6..0046FEDD
        glEnable_0073d390(load(kotor_renderer_cells::enable_0078d464));       // 0046FEDF..0046FEE5
        glEnable_0073d390(load(kotor_renderer_cells::enable_0078d468));       // 0046FEE7..0046FEEE
        render_flat_0046f900(primitive);                                      // 0046FEF0..0046FEF5 ([esp+18h] = arg)
        undo_env_map_transform_0046f870(field<void*>(material_of(part), 0x14), 0); // 0046FEFA..0046FF03
        glDisable_0073d22c(load(kotor_renderer_cells::enable_0078d460));      // 0046FF08..0046FF12
        glDisable_0073d22c(load(kotor_renderer_cells::enable_0078d464));      // 0046FF14..0046FF1A
        glDisable_0073d22c(load(kotor_renderer_cells::enable_0078d468));      // 0046FF1C..0046FF23
        glEnable_0073d390(load(kotor_renderer_cells::enable_0078d44c));       // 0046FF25..0046FF2C
    }
    glColor4f_0073d224(0.0f, 0.0f, 0.0f, F(kOneF));                         // 0046FF2E..0046FF39
    glEnable_0073d390(load(kotor_renderer_cells::disable_0078d45c));          // 0046FF3F..0046FF45
    gl_blend_func_indexed_00401670(0, 1);                                    // 0046FF47..0046FF4B
}                                                                             // 0046FF50..0046FF58

// 00470590 RenderNV1xEMLM
extern "C" void __cdecl render_nv1x_emlm_00470590(void* primitive) {
    if (aur_ati_fragment_shaders_bump_map_available_0045fb90() == 0) return; // 00470593..0047059A -> 0047074C (no draw)
    void* const part = primitive_part(primitive);                             // 004705A1..004705AE (esi)
    if (field<U>(part, 0x40) != 0) return;                                    // 004705B0..004705B5 -> 0047074A (no draw)
    if (*cells.active_scene_007b92a0 == nullptr ||                            // 004705BB..004705C2
        texture_null(field<void*>(material_of(part), 0)) ||                  // 004705C8..004705D4
        texture_null(field<void*>(material_of(part), 4)) ||                  // 004705DA..004705E7
        texture_null(field<void*>(material_of(part), 0x14))) {               // 004705ED..004705FA
        render_flat_0046f900(primitive);                                      // 00470741..00470747
        return;
    }
    glDisable_0073d22c(load(kotor_renderer_cells::enable_0078d44c));          // 00470600..0047060E (edi = glDisable)
    glDisable_0073d22c(kotor_renderer_gl_cells::vertex_enum_0078d46c);        // 00470610..00470617
    gl_blend_func_indexed_00401670(2, 3);                                    // 00470619..0047061D
    gl_render_activate_texture_stage_00425250(1);                             // 00470622..00470624
    glEnableClientState_0073d260(kTexCoordArray_0073f398);                    // 00470629..00470632
    gl_render_activate_texture_stage_00425250(0);                             // 00470638..0047063A
    enable_dispatch(0x1A, field<void*>(material_of(part), 0), field<void*>(material_of(part), 4),
        nullptr, nullptr);                                                    // 0047063F..0047064F
    render_flat_0046f900(primitive);                                          // 00470655..00470656 (ebx)
    enable_dispatch(0x20, nullptr, nullptr, nullptr, nullptr);                // 0047065B..00470665
    gl_render_activate_texture_stage_00425250(1);                             // 0047066B..0047066D
    glDisableClientState_0073d264(kTexCoordArray_0073f398);                   // 00470672..0047067B
    gl_render_activate_texture_stage_00425250(0);                             // 00470681..00470683
    gl_blend_func_indexed_00401670(2, 2);                                    // 00470688..0047068C
    enable_dispatch(0x15, field<void*>(material_of(part), 0), field<void*>(material_of(part), 0x14),
        nullptr, nullptr);                                                    // 00470691..004706A1
    do_env_map_transform_0046f780(field<void*>(material_of(part), 0x14), 1);  // 004706A7..004706B0
    float constant[3];                                                        // [esp+0Ch] local vector (sub esp,0Ch)
    constant[0] = F(kOneF);                                                   // 004706BF
    constant[1] = F(kOneF);                                                   // 004706C7
    constant[2] = F(kOneF);                                                   // 004706CF
    const auto constant0 = *static_cast<const volatile kotor_renderer_init::ConstantDispatch*>(
        &kotor_renderer_gl_cells::constant0_dispatch_007a6820);
    constant0(constant, F(kOneF));                                            // 004706B5..004706D7 call [007A6820]
    render_flat_0046f900(primitive);                                          // 004706DD..004706DE
    enable_dispatch(0x20, nullptr, nullptr, nullptr, nullptr);                // 004706E3..004706ED
    glDisable_0073d22c(load(kotor_renderer_cells::enable_0078d460));          // 004706F3..004706FC
    glDisable_0073d22c(load(kotor_renderer_cells::enable_0078d464));          // 004706FE..00470705
    glDisable_0073d22c(load(kotor_renderer_cells::enable_0078d468));          // 00470707..0047070E
    undo_env_map_transform_0046f870(field<void*>(material_of(part), 0x14), 1); // 00470710..00470719
    glEnable_0073d390(load(kotor_renderer_cells::enable_0078d44c));           // 0047071E..00470728
    gl_blend_func_indexed_00401670(0, 1);                                    // 0047072E..00470732
}                                                                             // 0047073A..00470740
