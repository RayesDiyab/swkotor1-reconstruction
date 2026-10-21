// model_material.cpp - lane-3d-model: per-part material/texture binding at instance creation
// (PartInitMaterial -> Material::LoadTextures -> Material::InitializeTextures [lane-texture-epoch]) and the
// vertex-primitive objects created for each mesh part; SafePointer<CAurObject>::operator= 00447FA0.
// Normal paths only (allocation EH unwind states not modeled; see model_instance.cpp header).
#include "a7e200e217-model_instance.hpp"
// lane-texture-epoch nominal Material (task-17 material-ownership-r1 layout + initialize_textures_0047b560).
#include "b22a9fca42-material.h"
#include <new>

using namespace aurora_model;

extern "C" {
// renderer / material workers not authored here
U8 __fastcall aurora_material_has_bump_map_0047b8f0(void* material, void* unused_edx);  // Material::HasBumpMap
U __cdecl aurora_fog_arrays_info_ref_00425c60();                                       // GLRender::GetFogArraysInfoRef
}

namespace kotor_renderer_init {
std::uint32_t __cdecl BumpMapAvailable_0045fb10();   // lane-upload provider (?BumpMapAvailable_0045fb10@kotor_renderer_init@@YAIXZ)
}

namespace {
const PartVTable* part_vt(Part* p) { return get<const PartVTable*>(p, 0); }
void zero(void* p, U off) { put<U>(p, off, 0); }
}

// 00447FA0 SafePointer<CAurObject>::operator=(target), RET4, returns this: unlink this slot from the old
// target's registration list (+4, 00445080 RemoveReverse32); store; append this slot to the new target's
// list (growth cap ? 2*cap : 8 via 005E03C0). Symbol per task-33 font.hpp (consumed by lane-texture-epoch).
extern "C" font::Safe* __fastcall assign_00447fa0(font::Safe* self, void*, font::Texture* target) {
    font::Texture* const old = self->target;                                        // 00447FA3
    if (old != nullptr) old->registrations.RemoveReverse32(addr(self));            // 00447FA5..00447FAD
    self->target = target;                                                          // 00447FB8
    if (target != nullptr) {                                                        // 00447FB6..00447FBA
        List& l = target->registrations;                                            // 00447FC0
        if (l.size == l.capacity)                                                   // 00447FBC..00447FC8
            menu_list_growth::allocate_005e03c0(&l, nullptr, l.capacity ? static_cast<U>(l.capacity) * 2u : 8u); // 00447FCA..00447FDA
        ptr<U>(l.data)[l.size] = addr(self);                                        // 00447FDF..00447FE4
        l.size = l.size + 1;                                                        // 00447FE7
    }
    return self;                                                                    // 00447FEB
}

// lane-3d-render consumes the same 00447FA0 body as @safe_pointer_assign_00447fa0@12 (scene3d.h).
#pragma comment(linker, "/alternatename:@safe_pointer_assign_00447fa0@12=@assign_00447fa0@12")

// Join (no original address credit): PartTriMesh 004458CB calls Material::Material 0047B290 on the
// 0x4C-byte block; the nominal constructor is task-17 material_owner::Material::Material().
void* __fastcall aurora_material_construct_0047b290(void* self, void*) {
    return ::new (self) material_owner::Material();
}

// 0047B770 Material::LoadTextures(part), RET4: tm = part->vtbl[2C](); s = part->vtbl[34]();
// tm -> InitializeTextures(node+A8 (texture0), node+C8 (texture1/lightmap), 0, 0) with node = tm[4];
// else s -> InitializeTextures(s[4]+D0, 0, 0, 0).
void __fastcall aurora_material_load_textures_0047b770(void* material, void*, Part* part) {
    Part* const mesh = ptr<Part>(part_vt(part)->as_trimesh(part, nullptr));                    // 0047B777..0047B780
    Part* const other = ptr<Part>(part_vt(part)->query34(part, nullptr));                      // 0047B783..0047B789
    material_owner::Material* const m = static_cast<material_owner::Material*>(material);
    if (mesh != nullptr) {                                                          // 0047B78C..0047B78E
        unsigned char* const node = get<unsigned char*>(mesh, 4);                   // 0047B790
        m->initialize_textures_0047b560(reinterpret_cast<const char*>(node + 0xA8), // 0047B79D
                                        reinterpret_cast<const char*>(node + 0xC8), nullptr, nullptr); // 0047B793..0047B7A7
        return;
    }
    if (other != nullptr) {                                                         // 0047B7B3..0047B7B5
        unsigned char* const node = get<unsigned char*>(other, 4);                  // 0047B7B7
        m->initialize_textures_0047b560(reinterpret_cast<const char*>(node + 0xD0), nullptr, nullptr, nullptr); // 0047B7BA..0047B7C7 (ebx = 0)
    }
}

// 0043E410 MdlNodeTriMesh::HasTangentBases: [+170] != -1.
U8 __fastcall aurora_trimesh_has_tangent_bases_0043e410(MdlNode* self, void*) {
    return get<U>(self, 0x170) != 0xFFFFFFFFu ? 1 : 0;
}

// 00478B50 VertexPrimitiveFlat::BuildVertexArrays: node = [this+4]; node[138] == 0 -> return. Copy the 8-byte
// texture coordinate pair of every vertex from the model MDX pool (lock (ACh,0,0,8), stride node[14C],
// offsets node[194]+node[160]) into the part pool tm[78] (lock (h,0,0,1)); unlock both.
U __fastcall aurora_vertex_flat_build_arrays_00478b50(void* self, void*) {
    void* const node0 = get<void*>(self, 4);                                        // 00478B53 (eax = node)
    if (get<U>(node0, 0x138) == 0) return addr(node0);                              // 00478B56..00478B5E (EAX still = node at 00478C1D)
    Part* const owner = get<Part*>(self, 8);                                        // 00478B64
    Part* const mesh = ptr<Part>((part_vt(owner)->as_trimesh(owner, nullptr)));       // 00478B67..00478B6C
    unsigned char* const dst = static_cast<unsigned char*>(aurora_pool_lock_00423f10(get<U>(mesh, 0x78), 0, 0, 1)); // 00478B6F..00478B7B
    MaxTree* const model = aurora_tree_as_model_0043e1c0(get<MaxTree*>(get<void*>(self, 4), 8), nullptr); // 00478B82..00478B8B
    unsigned char* const src = static_cast<unsigned char*>(aurora_pool_lock_00423f10(get<U>(model, 0xAC), 0, 0, 8)); // 00478B90..00478B9D
    for (I i = 0; i < static_cast<I>(get<I16>(get<void*>(self, 4), 0x180)); ++i) { // 00478BA2..00478BF1 (node re-read)
        void* const node = get<void*>(self, 4);
        const U off = get<U>(node, 0x14C) * static_cast<U>(i) + get<U>(node, 0x194) + get<U>(node, 0x160); // 00478BC0..00478BCF
        put<U>(dst, static_cast<U>(i) * 8, get<U>(src, off));                       // 00478BD5..00478BDA
        put<U>(dst, static_cast<U>(i) * 8 + 4, get<U>(src, off + 4));               // 00478BDD..00478BE0
    }
    aurora_pool_unlock_00423f30(get<U>(mesh, 0x78));                                // 00478BF3..00478BF7
    MaxTree* const model2 = aurora_tree_as_model_0043e1c0(get<MaxTree*>(get<void*>(self, 4), 8), nullptr); // 00478BFC..00478C05
    return aurora_pool_unlock_00423f30(get<U>(model2, 0xAC));                       // 00478C0A..00478C11 (EAX of the last call)
}

// 004792A0 VertexPrimitiveFlat::VertexPrimitiveFlat(part), RET4.
void* __fastcall aurora_vertex_flat_construct_004792a0(void* self, void*, Part* part) {
    put<Part*>(self, 8, part);                                                      // 004792C4
    zero(self, 0xC);                                                                // 004792C7
    put<U>(self, 0x10, 1);                                                          // 004792CA
    put<U8>(self, 0x14, 0);                                                         // 004792D1
    put<const void*>(self, 0, &aurora_vtable_vertex_flat_00742f74);                  // 004792D4
    void* const node = get<void*>(part, 4);                                         // 004792DA
    put<void*>(self, 4, node);                                                      // 004792DD
    const I16 vertices = get<I16>(node, 0x180);                                     // 004792E0
    if (vertices == 0) {                                                            // 004792E7..004792EE
        put<U>(self, 0xC, 1);                                                       // 004792F0
        put<U>(self, 0x18, 0xFFFFFFFFu);                                            // 004792F7
        return self;
    }
    if (get<U>(node, 0x138) != 0) {                                                 // 00479313..00479319
        aurora_pool_request_00423ee0(0, static_cast<U>(static_cast<I>(vertices)) << 3, 2, 0, ptr<U>(addr(at(part, 0x78)))); // 0047931B..0047932A
        put<U>(self, 0xC, get<U>(self, 0xC) + 2);                                   // 0047932F..00479338
    }
    aurora_vertex_flat_build_arrays_00478b50(self, nullptr);                        // 0047933B..0047933D
    put<U>(self, 0xC, get<U>(self, 0xC) + 6);                                       // 00479342..00479348
    put<U>(self, 0xC, get<U>(self, 0xC) + static_cast<U>(static_cast<I>(get<I16>(get<void*>(self, 4), 0x182))) * 2u); // 0047934B..0047935A
    if (aurora_material_has_bump_map_0047b8f0(get<void*>(part, 0x44), nullptr) != 0 &&           // 0047935D..00479367
        aurora_trimesh_has_tangent_bases_0043e410(get<MdlNode*>(self, 4), nullptr) != 0 &&      // 00479369..00479373
        kotor_renderer_init::BumpMapAvailable_0045fb10() != 0) {                                            // 00479375..0047937C
        put<U>(self, 0x18, aurora_fog_arrays_info_ref_00425c60());                  // 00479382..00479387
        put<U>(self, 0xC, get<U>(self, 0xC) + 9);                                   // 0047938A
        return self;
    }
    put<U>(self, 0x18, 0xFFFFFFFFu);                                                // 004792F7
    return self;
}
// 004793A0 VertexPrimitiveAnim::VertexPrimitiveAnim(part), RET4: Flat; vtable 0074301C; pool requests
// (0, n*12, 2, 0, &[1C]) and (0, n*8, 2, 0, &[20]) with n = (i16)node[180]; [10] = 0.
void* __fastcall aurora_vertex_anim_construct_004793a0(void* self, void*, Part* part) {
    aurora_vertex_flat_construct_004792a0(self, nullptr, part);                     // 004793BD..004793C2
    put<const void*>(self, 0, &aurora_vtable_vertex_anim_0074301c);                  // 004793CA
    aurora_pool_request_00423ee0(0, static_cast<U>(static_cast<I>(get<I16>(get<void*>(self, 4), 0x180)) * 12), 2, 0,
                                 ptr<U>(addr(at(self, 0x1C))));                     // 004793C7..004793F0
    aurora_pool_request_00423ee0(0, static_cast<U>(static_cast<I>(get<I16>(get<void*>(self, 4), 0x180))) << 3, 2, 0,
                                 ptr<U>(addr(at(self, 0x20))));                     // 004793F5..0047940D
    zero(self, 0x10);                                                               // 00479419
    return self;
}
// 00479430 VertexPrimitiveDangly::VertexPrimitiveDangly(part), RET4: Flat; vtable 00742FE4; n*12-byte block
// with n zeroed 12-byte vectors (null allocation -> 0); [1C] = block; [10] = 0.
void* __fastcall aurora_vertex_dangly_construct_00479430(void* self, void*, Part* part) {
    aurora_vertex_flat_construct_004792a0(self, nullptr, part);                     // 0047944F..00479454
    put<const void*>(self, 0, &aurora_vtable_vertex_dangly_00742fe4);                // 0047945C
    const I n = static_cast<I>(get<I16>(get<void*>(self, 4), 0x180));             // 00479459..00479462
    unsigned char* const block = static_cast<unsigned char*>(live_operator_new(static_cast<U>(n * 12))); // 00479469..00479476
    if (block != nullptr)                                                           // 0047947E..00479480
        for (I i = 0; i < n; ++i) { put<U>(block, 12 * i, 0); put<U>(block, 12 * i + 4, 0); put<U>(block, 12 * i + 8, 0); } // 00479482..0047949C
    put<void*>(self, 0x1C, block);                                                  // 004794A6
    zero(self, 0x10);                                                               // 004794A9
    return self;
}
// 004794C0 VertexPrimitiveSkinned::VertexPrimitiveSkinned(part), RET4: Flat; [C] += 6; vtable 00742FAC.
void* __fastcall aurora_vertex_skinned_construct_004794c0(void* self, void*, Part* part) {
    aurora_vertex_flat_construct_004792a0(self, nullptr, part);                     // 004794C5..004794C8
    put<U>(self, 0xC, get<U>(self, 0xC) + 6);                                       // 004794CD
    put<const void*>(self, 0, &aurora_vtable_vertex_skinned_00742fac);               // 004794D1
    return self;
}

namespace {
// Per-part block shared by 00442D89..00442EBE (top level) and 00442ED3..00443020 (children).
void init_part(Gob* gob, Part* part) {
    Part* const mesh = ptr<Part>(part_vt(part)->as_trimesh(part, nullptr));                    // 00442D8D..00442D91
    if (mesh == nullptr) return;                                                    // 00442D96..00442D98
    MdlNode* const node = get<MdlNode*>(mesh, 4);                                   // 00442D9E
    if (get<U>(aurora_node_as_trimesh_0043e400(node, nullptr), 0x138) != 0) {      // 00442DA1..00442DB9 (inlined cast)
        if (get<U>(aurora_node_as_trimesh_0043e400(node, nullptr), 0x160) != 0xFFFFFFFFu && gob != nullptr) // 00442DBB..00442DDE
            put<U8>(gob, 0x184, 1);                                                 // 00442DE0
    }
    if (get<void*>(mesh, 0x44) != nullptr)                                          // 00442DEC..00442DF1
        aurora_material_load_textures_0047b770(get<void*>(mesh, 0x44), nullptr, part); // 00442DF3..00442DF4 (push part)
    void* primitive;
    if (part_vt(mesh)->query50(mesh, nullptr) != 0) {                         // 00442DF9..00442E02
        void* const block = live_operator_new(0x1C);                                // 00442E04..00442E06
        primitive = block ? aurora_vertex_skinned_construct_004794c0(block, nullptr, ptr<Part>(part_vt(mesh)->query50(mesh, nullptr))) : nullptr; // 00442E22..00442E2C
    } else if (part_vt(mesh)->query30(mesh, nullptr) != 0) {                  // 00442E36..00442E3F
        void* const block = live_operator_new(0x24);                                // 00442E41..00442E43
        primitive = block ? aurora_vertex_anim_construct_004793a0(block, nullptr, mesh) : nullptr; // 00442E5B..00442E5E
    } else if (part_vt(mesh)->query4c(mesh, nullptr) != 0) {                  // 00442E65..00442E6E
        void* const block = live_operator_new(0x20);                                // 00442E70..00442E72
        primitive = block ? aurora_vertex_dangly_construct_00479430(block, nullptr, mesh) : nullptr; // 00442E8A..00442E8D
    } else {
        void* const block = live_operator_new(0x1C);                                // 00442E94..00442E96
        primitive = block ? aurora_vertex_flat_construct_004792a0(block, nullptr, mesh) : nullptr; // 00442EAE..00442EB1
    }
    put<void*>(mesh, 0x70, primitive);                                              // 00442EBE
}
}
// 00442D70 PartInitMaterial(gob, part, prefix), cdecl: the per-part block for part, then for each child
// (list +30, count +34 re-read) the block for the child and a recursive call per grandchild.
void __cdecl aurora_part_init_material_00442d70(Gob* gob, Part* part, const char* prefix) {
    init_part(gob, part);                                                           // 00442D89..00442EBE
    for (I i = 0; i < get<I>(part, 0x34); ++i) {                                    // 00442EC1..0044305B
        Part* const child = ptr<Part>(ptr<U>(get<U>(part, 0x30))[i]);               // 00442ED3..00442ED6
        init_part(gob, child);                                                      // 00442ED9..00443020
        for (I j = 0; j < get<I>(child, 0x34); ++j)                                 // 00443023..0044304F
            aurora_part_init_material_00442d70(gob, ptr<Part>(ptr<U>(get<U>(child, 0x30))[j]), prefix); // 00443030..00443041
    }
}

// 004402E0 PartTriMesh::GetTextureAngle (Part tables slot +84): fld dword ptr [ecx+94h]; ret -> float in ST0
// (review M-1: typed SlotF0 in model_vtables.hpp).
extern "C" float __fastcall part_tri_mesh_get_texture_angle_004402e0(void* self, void*) {
    return get<float>(self, 0x94);                                                  // 004402E0
}
