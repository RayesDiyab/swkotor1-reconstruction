// anim_skin.cpp - lane-3d-anim: CPU skinning path of swkotor.exe (GOG K1): GatherBones 00488930 (bone world
// positions / orientations) and ExecuteSkinTransforms 00487570 (weighted vertex / normal blend into two static GL
// pools, then texture-coordinate / vertex / normal buffer setup and the draw). Every statement cites the original.
// Float discipline per D30-FINAL (anim3d.h).
#include "3775ec35d8-anim3d.h"

using namespace kotor_anim3d;

namespace {
inline float f32(double value) { return static_cast<float>(value); }
inline void* word_ptr(U word) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(word)); }
template <class T> inline T* list_data(const PointerList* list) { return static_cast<T*>(word_ptr(list->data)); }

// One weighted bone contribution (shared by both vertex loops of ExecuteSkinTransforms):
//   position += weight * (M_bone * P0 + B_bone)    normal += weight * (M_bone * N0)
// with the original's float stores (004878B6..004878F8 / 004878FD..00487947 and 00487B4C..00487BE5).
void accumulate_bone(PointerList* positions, PointerList* matrices, I bone, float weight,
                     const Vector& p0, const Vector& n0, Vector& position, Vector& normal, bool position_first) {
    const Vector* base = list_data<Vector>(positions) + bone;               // lea ebp,[edx+ecx*4] (12-byte rows)
    const float* matrix = reinterpret_cast<const float*>(list_data<unsigned char>(matrices) + 36 * bone);   // lea edi,[eax+eax*8]; shl edi,2
    Vector rotated;                                                          // [esp+90h]
    const Vector* r = matrix_multiply_vector_004a9a60(&rotated, matrix, &p0); // 004878AE / 00487B4C
    // first loop: fld r.x; fadd base.x   second loop: fld base.x; fadd r.x  (commutative: identical sums)
    const double x = position_first ? static_cast<double>(r->x) + base->x : static_cast<double>(base->x) + r->x;
    const double y = position_first ? static_cast<double>(r->y) + base->y : static_cast<double>(base->y) + r->y;
    const float z = f32(position_first ? static_cast<double>(r->z) + base->z : static_cast<double>(base->z) + r->z);   // fstp [esp+84h]
    Vector weighted;                                                         // [esp+6Ch] / [esp+60h]
    weighted.x = f32(x * weight);                                            // 004878D7..004878DD / 00487B75..00487B7B
    weighted.y = f32(y * weight);                                            // 004878E1..004878E5 / 00487B7F..00487B83
    weighted.z = f32(static_cast<double>(z) * weight);                      // 004878E9..004878F4 / 00487B87..00487B92
    vector_add_assign_004a9740(&position, nullptr, &weighted);               // 004878F8 / 00487B96
    const float* matrix2 = reinterpret_cast<const float*>(list_data<unsigned char>(matrices) + 36 * bone);   // 004878FD..0048790B (reloaded)
    Vector turned;                                                           // [esp+98h]
    const Vector* n = matrix_multiply_vector_004a9a60(&turned, matrix2, &n0);// 00487916 / 00487BB4
    const double wz = static_cast<double>(weight) * n->z;                   // 0048791B..0048791F
    const double wy = static_cast<double>(weight) * n->y;                   // 00487925..0048792D
    const double wx = static_cast<double>(weight) * n->x;                   // 00487930..00487934
    Vector scaled;                                                           // [esp+60h]
    scaled.x = f32(wx);                                                      // 0048793B
    scaled.y = f32(wy);                                                      // 0048793F
    scaled.z = f32(wz);                                                      // 00487943
    vector_add_assign_004a9740(&normal, nullptr, &scaled);                   // 00487947 / 00487BE5
}
}

extern "C" {
// 00488930 GatherBones(Part* part, CExoArrayList<Vector>* positions, CExoArrayList<Quaternion>* orientations,
//                      Vector position, Quaternion orientation) cdecl, recursive over Part children (+30/+34).
// The original inlines three child levels (00488AB0..004892F3) that append through CExoArrayList<Vector>::Add
// 004213E0 / CExoArrayList<Quaternion>::Add 00438AF0; those differ from the inline growth below only when the list
// is empty (initial capacity 8 vs 16 for 00438AF0), which cannot occur after this level's own append.
void __cdecl gather_bones_00488930(void* part, PointerList* positions, PointerList* orientations, Vector position, Quaternion orientation) {
    Vector rotated;                                                          // [esp+0E8h]
    const Vector* r = quaternion_rotate_004a9bb0(&rotated, &orientation, &field<Vector>(part, 0x08));   // 00488941..00488955
    Vector world;                                                            // [esp+0D8h]
    world.x = f32(static_cast<double>(position.x) + r->x);                  // 0048895A..0048896E
    world.y = f32(static_cast<double>(position.y) + r->y);                  // 00488975..0048897F
    world.z = f32(static_cast<double>(position.z) + r->z);                  // 00488986..00488995
    Quaternion combined;                                                     // [esp+10h]
    const Quaternion* q = quaternion_multiply_004a9ac0(&combined, &orientation, &field<Quaternion>(part, 0x14));   // 00488990..0048899D
    orientation = *q;                                                        // 004889A2..004889D4 (stored over the argument)
    if (positions->size == positions->capacity) {                            // 004889C9..004889DB
        const I grow = (positions->capacity != 0) ? positions->capacity * 2 : 8;   // 004889DD..004889E5
        vector_list_allocate_00421310(positions, nullptr, grow);             // 004889EA..004889ED
    }
    list_data<Vector>(positions)[positions->size] = world;                   // 004889F2..00488A17
    positions->size = positions->size + 1;                                   // 00488A1A..00488A33
    if (orientations->size == orientations->capacity) {                      // 00488A3D..00488A52
        const I grow = (orientations->capacity != 0) ? orientations->capacity * 2 : 8;   // 00488A54..00488A5C
        quaternion_list_allocate_00437cc0(orientations, nullptr, grow);      // 00488A61..00488A64
    }
    list_data<Quaternion>(orientations)[orientations->size] = orientation;  // 00488A69..00488A83
    orientations->size = orientations->size + 1;                             // 00488A86
    for (I i = 0; i < field<I>(part, 0x34); ++i) {                           // 00488A89..00489323 (count reloaded)
        void* child = word_ptr(list_data<U>(static_cast<PointerList*>(at(part, 0x30)))[i]);   // 00488AED..00488AFB
        gather_bones_00488930(child, positions, orientations, world, orientation);   // inlined levels / 0048926B
    }
}

// 00487570 ExecuteSkinTransforms(MdlNodeSkin* node, CExoArrayList<Vector>* positions, CExoArrayList<Matrix>* matrices,
//                                VertexPrimitive* primitive, bool flag) cdecl. Static pools 008303AC (vertices) and
// 008303A8 (normals) are requested once (guard 008303B4 bits 1/2/4; EH funclets 00717678/00717686 clear the bits if
// a request throws - RequestPool does not throw on the reachable paths, D30-FINAL unwind note).
void __cdecl execute_skin_transforms_00487570(void* node, PointerList* positions, PointerList* matrices, void* primitive, U flag_word) {
    const U8 flag = static_cast<U8>(flag_word);                             // 00487C0D / 00487EDF mov al,byte ptr [esp+0C8h]
    U guard = skin_guard_008303b4;                                           // 00487576
    U capacity;
    if ((guard & 1u) == 0) {                                                 // 00487591..0048759F
        capacity = static_cast<U>(static_cast<I>(field<I16>(node, 0x180)));  // 004875A1 movsx
        guard |= 1u; skin_guard_008303b4 = guard;                            // 004875A8..004875AB
        skin_pool_vertices_008303b0 = capacity;                              // 004875B1
    } else {
        capacity = skin_pool_vertices_008303b0;                              // 004875B8
    }
    if ((guard & 2u) == 0) {                                                 // 004875C0..004875C3
        guard |= 2u; skin_guard_008303b4 = guard;                            // 004875C5..004875C8
        const U handle = aurora_pool_request_00423ee0(0, capacity * 12u, 0x42, 0, &skin_vertex_pool_008303ac);   // 004875CE..004875EB
        guard = skin_guard_008303b4;                                         // 004875F0
        skin_vertex_pool_008303ac = handle;                                  // 004875F6 (after the callee's own store)
        capacity = skin_pool_vertices_008303b0;                              // 004875FB
    }
    if ((guard & 4u) == 0) {                                                 // 0048760A..0048760D
        guard |= 4u; skin_guard_008303b4 = guard;                            // 0048760F..00487612
        const U handle = aurora_pool_request_00423ee0(0, capacity * 12u, 0x42, 0, &skin_normal_pool_008303a8);   // 00487618..00487635
        skin_normal_pool_008303a8 = handle;                                  // 0048763A
        capacity = skin_pool_vertices_008303b0;                              // 0048763F
    }
    const U rows = static_cast<U>(static_cast<I>(field<I16>(node, 0x180)));  // 0048764E movsx
    if (rows > capacity) {                                                   // 00487655..00487657 (unsigned jbe)
        skin_pool_vertices_008303b0 = rows;                                  // 0048765F
        gl_render_resize_pool_00423f70(skin_vertex_pool_008303ac, rows * 12u);   // 00487659..0048766D
        gl_render_resize_pool_00423f70(skin_normal_pool_008303a8, skin_pool_vertices_008303b0 * 12u);   // 00487672..00487684
    }
    Vector* vertices = static_cast<Vector*>(aurora_pool_lock_00423f10(skin_vertex_pool_008303ac, 0, 0, 1));   // 0048768C..00487699 [esp+58h]
    Vector* normals = static_cast<Vector*>(aurora_pool_lock_00423f10(skin_normal_pool_008303a8, 0, 0, 1));    // 0048769E..004876BA [esp+5Ch]
    void* model = aurora_tree_as_model_0043e1c0(field<aurora_model::MaxTree*>(node, 0x08), nullptr);           // 004876B4..004876BE
    unsigned char* mdx = static_cast<unsigned char*>(aurora_pool_lock_00423f10(field<U>(model, 0xAC), 0, 0, 0x10));   // 004876C3..004876DC (ebx)
    void* skin = field<void*>(primitive, 0x04);                              // 004876D5..004876DE
    Vector p0 = {0.0f, 0.0f, 0.0f}, n0 = {0.0f, 0.0f, 0.0f};                // 004876E1..00487709 [esp+40h] / [esp+4Ch]
    Vector position = {0.0f, 0.0f, 0.0f}, normal = {0.0f, 0.0f, 0.0f};      // 00487711..00487739 [esp+2Ch] / [esp+1Ch]
    auto vertex_base = [&](U v) -> U { return field<U>(node, 0x14C) * v + field<U>(node, 0x194); };
    // ---- per bone-group vertex lists: float bone indices (__ftol2)
    for (I group = 0; group < field<I>(skin, 0x11C); ++group) {              // 00487741..004879F4 (count reloaded)
        const U16* indices = static_cast<const U16*>(aurora_pool_lock_00423f10(field<U*>(skin, 0x118)[group], 0, 0, 8));   // 00487760..00487770 [esp+3Ch]
        for (I j = 0; j < field<I*>(skin, 0x100)[group]; ++j) {              // 00487775..004879C2 (count reloaded)
            const U v = indices[j];                                          // 004877A0..004877A4 movzx
            const U vb = vertex_base(v);                                     // 004877A8..004877B7
            std::memcpy(&p0, mdx + vb + field<U>(node, 0x154), 12);          // 004877BA..004877DE
            std::memcpy(&n0, mdx + vb + field<U>(node, 0x158), 12);          // 004877C0..004877F2
            position = {0.0f, 0.0f, 0.0f};                                   // 004877FC..0048780C
            normal = {0.0f, 0.0f, 0.0f};                                     // 00487814..00487824
            for (U b = 0; b < 16u; b += 4u) {                                // 0048782C / 0048794C..0048795E
                const U offset = b + field<U>(node, 0x194) + field<U>(node, 0x14C) * v;   // 00487830..0048784A
                const float weight = *reinterpret_cast<const float*>(mdx + offset + field<U>(node, 0x1A8));   // 0048784C..0048784F fst [esp+10h]
                if (weight == kZero_0073d700) continue;                      // 00487853..0048785E (test ah,44h; jnp)
                const I slot = ftol2(*reinterpret_cast<const float*>(mdx + field<U>(node, 0x1AC) + b + field<U>(node, 0x194) +
                                                                      field<U>(node, 0x14C) * v));   // 00487864..00487873 __ftol2
                const I bone = static_cast<I>(field<I16>(node, 0x1DC + 2u * static_cast<U>(slot)));   // 00487878 movsx
                accumulate_bone(positions, matrices, bone, weight, p0, n0, position, normal, true);
            }
            vertices[v] = {f32(position.x), f32(position.y), f32(position.z)};   // 00487964..00487985 (fld/fstp)
            normals[v] = {f32(normal.x), f32(normal.y), f32(normal.z)};          // 00487989..004879A0
        }
        aurora_pool_unlock_00423f30(field<U*>(field<void*>(primitive, 0x04), 0x118)[group]);   // 004879CF..004879DC
    }
    // ---- shared vertex list: 16-bit bone indices
    const U16* shared = static_cast<const U16*>(aurora_pool_lock_00423f10(field<U>(field<void*>(primitive, 0x04), 0x128), 0, 0, 8));   // 004879FA..00487A0F [esp+3Ch]
    for (U j = 0; j < field<U>(field<void*>(primitive, 0x04), 0x12C); ++j) {   // 00487A13..00487A27 / 00487C9E..00487CB9 (unsigned jb)
        const U v = shared[j];                                               // 00487A30..00487A38 (ebp)
        const U vb = vertex_base(v);                                         // 00487A3C..00487A51
        std::memcpy(&p0, mdx + vb + field<U>(node, 0x154), 12);              // 00487A53..00487A71
        std::memcpy(&n0, mdx + vb + field<U>(node, 0x158), 12);              // 00487A5D..00487A8B
        position = {0.0f, 0.0f, 0.0f};                                       // 00487A8F..00487A9F
        normal = {0.0f, 0.0f, 0.0f};                                         // 00487AA7..00487AB7
        for (U b = 0; b < 4u; ++b) {                                         // 00487ABF..00487C07 (edi += 4, [esp+14h] += 2)
            const U row = field<U>(node, 0x14C) * v;
            const float weight = *reinterpret_cast<const float*>(mdx + 4u * b + row + field<U>(node, 0x194) + field<U>(node, 0x1A8));   // 00487AD0..00487AEF
            if (weight == kZero_0073d700) continue;                          // 00487AF3..00487AFE
            const U slot = *reinterpret_cast<const U16*>(mdx + field<U>(node, 0x1AC) + 2u * b + row + field<U>(node, 0x194));   // 00487B04..00487B12 movzx
            const I bone = static_cast<I>(field<I16>(node, 0x1DC + 2u * slot));   // 00487B16 movsx
            accumulate_bone(positions, matrices, bone, weight, p0, n0, position, normal, false);
        }
        if (flag != 0) {                                                     // 00487C0D..00487C16
            const double extrude = field<float>(field<void*>(field<void*>(primitive, 0x08), 0x40), 0x11C);   // 00487C18..00487C25
            Vector offset;                                                   // [esp+84h]
            offset.x = f32(static_cast<double>(normal.x) * extrude);        // 00487C32..00487C3D
            offset.y = f32(static_cast<double>(normal.y) * extrude);        // 00487C44..00487C4A
            offset.z = f32(extrude * normal.z);                              // 00487C51..00487C55
            vector_add_assign_004a9740(&position, nullptr, &offset);         // 00487C5C
        }
        vertices[v] = {f32(position.x), f32(position.y), f32(position.z)};   // 00487C61..00487C7F
        normals[v] = {f32(normal.x), f32(normal.y), f32(normal.z)};          // 00487C83..00487C9A
    }
    aurora_pool_unlock_00423f30(field<U>(field<void*>(primitive, 0x04), 0x128));   // 00487CC8..00487CD2
    aurora_pool_unlock_00423f30(skin_vertex_pool_008303ac);                  // 00487CD7..00487CDE
    aurora_pool_unlock_00423f30(skin_normal_pool_008303a8);                  // 00487CE3..00487CE9
    void* model2 = aurora_tree_as_model_0043e1c0(field<aurora_model::MaxTree*>(node, 0x08), nullptr);   // 00487CEE..00487CF4
    aurora_pool_unlock_00423f30(field<U>(model2, 0xAC));                     // 00487CF9..00487D00
    // ---- GL setup
    const Cells& c = anim_cells;
    if (aur_multi_texture_available_0045f710() != 0) (*c.client_active_texture_007bb6b0)(0x84C0u);   // 00487D08..00487D16
    *c.current_stage_007a6898 = 0;                                           // 00487D1C
    if (field<I16>(node, 0x182) > 0) kotor_renderer_init::glEnableClientState_0073d260(kGlTextureCoordArray_0073f398);   // 00487D22..00487D32
    else kotor_renderer_init::glDisableClientState_0073d264(kGlTextureCoordArray_0073f398);                              // 00487D3A..00487D41
    auto tex_coords = [&](U texture_unit, U stage, U field_offset) {
        if (aur_multi_texture_available_0045f710() != 0) (*c.client_active_texture_007bb6b0)(texture_unit);   // e.g. 00487D5F..00487D6D
        *c.current_stage_007a6898 = stage;                                   // e.g. 00487D73
        const I count = static_cast<I>(field<I16>(node, 0x180));             // e.g. 00487D80 movsx
        void* m = aurora_tree_as_model_0043e1c0(field<aurora_model::MaxTree*>(node, 0x08), nullptr);   // e.g. 00487D7D..00487D87
        gl_render_set_tex_coord_buffer_00425a40(field<U>(node, 0x14C), field<U>(node, 0x194) + field<U>(node, field_offset),
                                                field<U>(m, 0xAC), count);   // e.g. 00487D8C..00487DAA
    };
    switch (static_cast<I>(field<I16>(node, 0x182)) - 1) {                   // 00487D47..00487D58 (table 004880B8)
    case 3: tex_coords(0x84C3u, 3u, 0x16C);                                  // 00487D5F..00487DAF
            [[fallthrough]];
    case 2: tex_coords(0x84C2u, 2u, 0x168);                                  // 00487DB2..00487E00
            [[fallthrough]];
    case 1: tex_coords(0x84C1u, 1u, 0x164);                                  // 00487E03..00487E53
            [[fallthrough]];
    case 0: tex_coords(0x84C0u, 0u, 0x160);                                  // 00487E56..00487EA2
            break;
    default: break;                                                          // ja 00487EA5
    }
    gl_render_set_normal_buffer_004259e0(12, 0, skin_normal_pool_008303a8, static_cast<I>(field<I16>(node, 0x180)));   // 00487EA5..00487EB6
    gl_render_set_vertex_buffer_00425900(12, 0, skin_vertex_pool_008303ac, static_cast<I>(field<I16>(node, 0x180)));   // 00487EBB..00487ECD
    void* owner = field<void*>(primitive, 0x08);                             // 00487ED2
    void* part = as_object(part_table(owner)->s050(owner, nullptr));         // 00487ED5..00487EDD (ebx)
    if (flag != 0) {                                                         // 00487EDF..00487EE8
        *c.current_stage_007a6898 = 0;                                       // 00487EEA
        if (aur_multi_texture_available_0045f710() != 0) (*c.active_texture_007bb80c)(*c.current_stage_007a6898 + 0x84C0u);   // 00487EF0..00487F06
        void* texture = field<void*>(field<void*>(part, 0x44), 0x0C);        // 00487F0C..00487F0F
        slots<TextureSlots>(texture)->slotb4(texture, nullptr);              // 00487F12..00487F14
        if (field<I>(field<void*>(part, 0x44), 0x2C) > 1) material_set_blending_mode_0047af70(field<void*>(part, 0x44), nullptr, 1);   // 00487F1A..00487F25
        render_flat_0046f900(primitive);                                     // 00487F2A..00487F2B
        material_bind_texture0_0047adf0(field<void*>(part, 0x44), nullptr);  // 00487F30..00487F36
        kotor_renderer_init::glBlendFunc_0073d38c(kGlSrcAlpha_0073f270, kGlOneMinusSrcAlpha_0073f274);   // 00487F3B..00487F49
        return;
    }
    void* object = field<void*>(part, 0x40);                                 // 00487F54
    if ((object != nullptr && field<U>(object, 0x1A8) != 0) ||               // 00487F57..00487F61
        material_has_env_map_0047b0e0(field<void*>(part, 0x44), nullptr) != 0) {   // 00487F63..00487F6D
        render_environment_mapped_0046ff70(primitive, 0, 0);                 // 00487F6F..00487F72
        return;
    }
    if (aurora_material_has_bump_map_0047b8f0(field<void*>(part, 0x44), nullptr) != 0 &&
        aurora_trimesh_has_tangent_bases_0043e410(node, nullptr) != 0) {     // 00487F7F..00487F98
        if (aur_ati_fragment_shaders_bump_map_available_0045fb90() != 0) {   // 00487F9A..00487FA1
            (*c.enable_dispatch_007a47e0)(0x17u, nullptr, nullptr, nullptr, nullptr);   // 00487FA3..00487FA9
            render_flat_0046f900(primitive);                                 // 00487FAF..00487FB0
            (*c.enable_dispatch_007a47e0)(0u, nullptr, nullptr, nullptr, nullptr);      // 00487FB5..00487FBA
            return;
        }
        kotor_renderer_init::glDisable_0073d22c(*c.gl_alpha_test_0078d44c);  // 00487FC8..00487FCE
        kotor_renderer_init::glBlendFunc_0073d38c(kGlOne_0073f278, kGlZero_0073f27c);   // 00487FD4..00487FE8
        render_flat_0046f900(primitive);                                     // 00487FEA..00487FEB
        kotor_renderer_init::glBlendFunc_0073d38c(kGlSrcAlpha_0073f270, kGlOneMinusSrcAlpha_0073f274);   // 00487FF0..00488000
        kotor_renderer_init::glEnable_0073d390(*c.gl_alpha_test_0078d44c);   // 00488002..00488009
        return;
    }
    if (aur_ati_fragment_shaders_bump_map_available_0045fb90() == 0 && !aur_nv2x_level_shaders_available_0045fbd0() &&
        aur_multi_texture_available_0045f710() != 0 && aur_vertex_programs_nv_available_0045f740() != 0) {   // 00488014..00488036
        *c.current_stage_007a6898 = 1;                                       // 00488038
        if (aur_multi_texture_available_0045f710() != 0) (*c.active_texture_007bb80c)(*c.current_stage_007a6898 + 0x84C0u);   // 00488042..00488056
        kotor_renderer_init::glEnable_0073d390(*c.gl_texture_2d_0078d458);   // 0048805C..00488063
        gl_render_bind_texture_00420440(4, 0);                               // 00488069..0048806C
        *c.current_stage_007a6898 = 0;                                       // 00488074
        if (aur_multi_texture_available_0045f710() != 0) (*c.active_texture_007bb80c)(*c.current_stage_007a6898 + 0x84C0u);   // 0048807A..00488090
    }
    render_flat_0046f900(primitive);                                         // 00488096..00488097
}
} // extern "C"
