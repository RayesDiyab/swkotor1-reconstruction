// model_binary.cpp - lane-3d-model: InputBinary (binary MDL/MDX image -> runtime Model) relocation,
// node/tree free lists, animation footprint, node-type casts, GLRender vertex-pool wrappers and the
// function-pointer thunks written into relocated objects. Evidence cited per behavior (GOG swkotor.exe).
#include "d57d20b49f-model_load.hpp"

using namespace aurora_model;

extern "C" {
volatile U aurora_anim_footprint_007a6990 = 0;
volatile I aurora_mesh_vertices_00832c80 = 0;
volatile I aurora_mesh_vertex_bytes_00832c88 = 0;
}

namespace {
U rel(void* p, U off, U base) { const U v = get<U>(p, off) + base; put<U>(p, off, v); return v; }
// "if (count field) pointer += base; else pointer = 0" - the recurring relocation idiom
// (e.g. 004A0B76..004A0B88, 004A0C31..004A0C3E).
void rel_if(void* p, U ptr_off, U count_off, U base) {
    if (get<U>(p, count_off) != 0) put<U>(p, ptr_off, get<U>(p, ptr_off) + base);
    else put<U>(p, ptr_off, 0);
}
U* words(void* p, U off) { return ptr<U>(get<U>(p, off)); }
List* list_at(void* p, U off) { return reinterpret_cast<List*>(at(p, off)); }
}

// ---------------------------------------------------------------- GLRender vertex pool (cell 007A68A4)
// 00423EE0 RequestPool(a1,a2,a3,unused,out): *out = pool->vtbl[1](a1,a2,a3) (00423EE4..00423EFE).
U __cdecl aurora_pool_request_00423ee0(U a1, U a2, U a3, U, U* out) {
    PoolObject* const pool = kotor_vertex_pool_007a68a4;
    const U handle = pool->vtable->request(pool, nullptr, a1, a2, a3);
    *out = handle;
    return handle;                                                                  // EAX still holds the handle at 00423F00 (0044AB8E consumes it)
}
// 00423F10 LockPool(handle,unused,unused,a4) -> pool->vtbl[2](handle, a4) (00423F10..00423F22).
void* __cdecl aurora_pool_lock_00423f10(U handle, U, U, U a4) {
    PoolObject* const pool = kotor_vertex_pool_007a68a4;
    return pool->vtable->lock(pool, nullptr, handle, a4);
}
// 00423F30 UnlockPool(handle) -> pool->vtbl[3](handle); raw EAX returned (00423F3D/00423F40).
U __cdecl aurora_pool_unlock_00423f30(U handle) {
    PoolObject* const pool = kotor_vertex_pool_007a68a4;
    return pool->vtable->unlock(pool, nullptr, handle);
}
// 00423F50 DeletePool(handle): handle == -1 -> return handle (EAX = argument); else pool->vtbl[6](handle).
U __cdecl aurora_pool_delete_00423f50(U handle) {
    if (handle == 0xFFFFFFFFu) return handle;
    PoolObject* const pool = kotor_vertex_pool_007a68a4;
    return pool->vtable->destroy(pool, nullptr, handle);
}

// 00423F70 ResizePool(handle, bytes) -> pool->vtbl[7](handle, bytes); EAX of the call returned (00423F70..00423F85).
U __cdecl gl_render_resize_pool_00423f70(U handle, U bytes) {
    PoolObject* const pool = kotor_vertex_pool_007a68a4;
    return pool->vtable->resize(pool, nullptr, handle, bytes);
}

// ---------------------------------------------------------------- node type casts
// Each returns this when (type & mask) == mask, else 0. Byte-wide tests use byte [this] (0043E360 etc.);
// AABB/Dangly/Saber test the 16-bit word (0043E340 / 0043E380 / 0043E3A0).
MdlNode* __fastcall aurora_node_as_aabb_0043e340(MdlNode* self, void*) { return (get<U16>(self, 0) & 0x221) == 0x221 ? self : nullptr; }
MdlNode* __fastcall aurora_node_as_anim_mesh_0043e360(MdlNode* self, void*) { return (get<U8>(self, 0) & 0xA1) == 0xA1 ? self : nullptr; }
MdlNode* __fastcall aurora_node_as_camera_0043e370(MdlNode* self, void*) { return (get<U8>(self, 0) & 0x09) == 0x09 ? self : nullptr; }
MdlNode* __fastcall aurora_node_as_dangly_0043e380(MdlNode* self, void*) { return (get<U16>(self, 0) & 0x121) == 0x121 ? self : nullptr; }
MdlNode* __fastcall aurora_node_as_saber_0043e3a0(MdlNode* self, void*) { return (get<U16>(self, 0) & 0x821) == 0x821 ? self : nullptr; }
MdlNode* __fastcall aurora_node_as_emitter_0043e3c0(MdlNode* self, void*) { return (get<U8>(self, 0) & 0x05) == 0x05 ? self : nullptr; }
MdlNode* __fastcall aurora_node_as_light_0043e3d0(MdlNode* self, void*) { return (get<U8>(self, 0) & 0x03) == 0x03 ? self : nullptr; }
MdlNode* __fastcall aurora_node_as_reference_0043e3e0(MdlNode* self, void*) { return (get<U8>(self, 0) & 0x11) == 0x11 ? self : nullptr; }
MdlNode* __fastcall aurora_node_as_skin_0043e3f0(MdlNode* self, void*) { return (get<U8>(self, 0) & 0x61) == 0x61 ? self : nullptr; }
MdlNode* __fastcall aurora_node_as_trimesh_0043e400(MdlNode* self, void*) { return (get<U8>(self, 0) & 0x21) == 0x21 ? self : nullptr; }

// ---------------------------------------------------------------- free lists
// 004451D0 MaxTree::ResetFreeLists: free [+30] (zero it), [+38] = [+34] = 0; free [+3C] (zero it), [+44] = [+40] = 0.
void __fastcall aurora_tree_reset_free_lists_004451d0(MaxTree* self, void*) {
    if (get<U>(self, 0x30) != 0) { live_free(ptr<void>(get<U>(self, 0x30))); put<U>(self, 0x30, 0); }  // 004451D9..004451E6
    put<U>(self, 0x38, 0);                                                          // 004451E9
    put<U>(self, 0x34, 0);                                                          // 004451EC
    if (get<U>(self, 0x3C) != 0) { live_free(ptr<void>(get<U>(self, 0x3C))); put<U>(self, 0x3C, 0); }  // 004451EF..004451FF
    put<U>(self, 0x44, 0);                                                          // 00445202
    put<U>(self, 0x40, 0);                                                          // 00445205
}
// 00445190 MaxTree::AddToFreeTokenList(token), RET4: list at +3C; grow via 00437D60 (cap ? 2*cap : 8); append.
void __fastcall aurora_tree_add_free_token_00445190(MaxTree* self, void*, U token) {
    List* const l = list_at(self, 0x3C);
    if (l->size == l->capacity) kotor_renderer_gl::allocate_00437d60(l, nullptr, static_cast<U>(l->capacity ? l->capacity + l->capacity : 8)); // 00445197..004451AC
    ptr<U>(l->data)[l->size] = token;                                               // 004451B1..004451BA
    l->size = l->size + 1;                                                          // 004451BD
}
// 00445310 MdlNode::AddToFreeTokenList(token), RET4: same append on [node+8]+3C.
void __fastcall aurora_node_add_free_token_00445310(MdlNode* self, void*, U token) {
    List* const l = list_at(get<MaxTree*>(self, 8), 0x3C);                          // 00445311..0044531A
    if (l->size == l->capacity) kotor_renderer_gl::allocate_00437d60(l, nullptr, static_cast<U>(l->capacity ? l->capacity + l->capacity : 8)); // 0044531D..00445331
    ptr<U>(l->data)[l->size] = token;                                               // 00445336..0044533F
    l->size = l->size + 1;                                                          // 00445342
}
// 004452D0 MdlNode::AddToFreeList(item), RET4: append on [node+8]+30, growth through 005E03C0.
void __fastcall aurora_node_add_free_list_004452d0(MdlNode* self, void*, void* item) {
    List* const l = list_at(get<MaxTree*>(self, 8), 0x30);                          // 004452D1..004452DA
    if (l->size == l->capacity) menu_list_growth::allocate_005e03c0(l, nullptr, l->capacity ? static_cast<U>(l->capacity) * 2u : 8u); // 004452DD..004452F1
    ptr<U>(l->data)[l->size] = addr(item);                                          // 004452F6..004452FF
    l->size = l->size + 1;                                                          // 00445302
}
// 00438A00 MdlNode::TranslateNodeName: index = dword [node+4]; tree = [node+8]; Model -> [[tree+B8]+i*4];
// otherwise animation -> [[[anim+84]+B8]+i*4] (owner model of the animation).
const char* __fastcall aurora_node_translate_name_00438a00(MdlNode* self, void*) {
    const U index = get<U>(self, 4);                                                // 00438A11
    if (aurora_tree_as_model_0043e1c0(get<MaxTree*>(self, 8), nullptr) != nullptr) { // 00438A03..00438A14
        MaxTree* const model = aurora_tree_as_model_0043e1c0(get<MaxTree*>(self, 8), nullptr); // 00438A16
        return ptr<const char*>(get<U>(model, 0xB8))[index];                        // 00438A1B..00438A22
    }
    MaxTree* const anim = aurora_tree_as_animation_0043e1d0(get<MaxTree*>(self, 8), nullptr); // 00438A28
    MaxTree* const owner = get<MaxTree*>(anim, 0x84);                               // 00438A2D
    return ptr<const char*>(get<U>(owner, 0xB8))[index];                            // 00438A33..00438A3E
}

// ---------------------------------------------------------------- footprint
// 00437DB0 UpdateAnimFootprint(node, scale), cdecl, recursive (the original inlines three levels):
// [007A6990] += [48]*scale*4; += [3C]*scale*16; += per-type size*scale for the first matching cast in the
// order AABB 1A0, AnimMesh 1D4, Camera 50, Dangly 1B8, Emitter 130, Light AC, Saber 1B0, Reference 74,
// Skin 200, TriMesh 19C; += [30]*scale*4; then recurse over the [2C]/[30] children.
void __cdecl aurora_update_anim_footprint_00437db0(MdlNode* node, I scale) {
    const U s = static_cast<U>(scale);
    aurora_anim_footprint_007a6990 = aurora_anim_footprint_007a6990 + get<U>(node, 0x48) * s * 4u;  // 00437DC3..00437DCC
    aurora_anim_footprint_007a6990 = aurora_anim_footprint_007a6990 + ((get<U>(node, 0x3C) * s) << 4); // 00437DD1..00437DDE
    U size = 0;
    if (aurora_node_as_aabb_0043e340(node, nullptr)) size = 0x1A0;                  // 00437DE3..00437DEE
    else if (aurora_node_as_anim_mesh_0043e360(node, nullptr)) size = 0x1D4;        // 00437DFB..00437E0B
    else if (aurora_node_as_camera_0043e370(node, nullptr)) size = 0x50;            // 00437E1F..00437E30
    else if (aurora_node_as_dangly_0043e380(node, nullptr)) size = 0x1B8;           // 00437E41..00437E4C
    else if (aurora_node_as_emitter_0043e3c0(node, nullptr)) size = 0x130;          // 00437E59..00437E69
    else if (aurora_node_as_light_0043e3d0(node, nullptr)) size = 0xAC;             // 00437E7D..00437E8D
    else if (aurora_node_as_saber_0043e3a0(node, nullptr)) size = 0x1B0;            // 00437E9E..00437EA9
    else if (aurora_node_as_reference_0043e3e0(node, nullptr)) size = 0x74;         // 00437EB3..00437EC3
    else if (aurora_node_as_skin_0043e3f0(node, nullptr)) size = 0x200;             // 00437ED1..00437EE1
    else if (aurora_node_as_trimesh_0043e400(node, nullptr)) size = 0x19C;          // 00437EEF..00437EFA
    if (size) aurora_anim_footprint_007a6990 = aurora_anim_footprint_007a6990 + size * s; // 00437F00 etc.
    aurora_anim_footprint_007a6990 = aurora_anim_footprint_007a6990 + get<U>(node, 0x30) * s * 4u;  // 00437F06..00437F15
    for (I i = 0; i < get<I>(node, 0x30); ++i)                                      // 00438386..004383A6
        aurora_update_anim_footprint_00437db0(ptr<MdlNode*>(get<U>(node, 0x2C))[i], scale); // 00438390..00438398
}

// ---------------------------------------------------------------- function-pointer thunks
// Stored into relocated trees/nodes (004A1185/004A118B, 004A1016/004A101C, 004A093B/004A0942, ...).
void __cdecl aurora_model_destructor_0044aa60(MaxTree* tree) { aurora_model_destroy_body_00449f60(tree, nullptr); }               // mov ecx,[esp+4]; jmp 00449F60
void __cdecl aurora_model_parse_field_0043e1e0(MaxTree* tree, char* line) { aurora_model_parse_field_body_004688a0(tree, nullptr, line); } // push line; ecx=tree; call 004688A0
void __cdecl aurora_maxtree_destructor_0044a8e0(MaxTree* tree) {                                                                   // 0044A8E0..0044A8EB
    MdlNode* const root = get<MdlNode*>(tree, 0x28);
    if (root != nullptr) aurora_node_destructor_dispatch_004488f0(root, nullptr);
}
void __cdecl aurora_maxtree_parse_field_0043e190(MaxTree* tree, char* line) { aurora_maxtree_parse_field_body_00468870(tree, nullptr, line); }
void __cdecl aurora_trimesh_gen_vertices_0043e480(MdlNode* node) { aurora_trimesh_gen_vertices_body_00439df0(node, nullptr); }
void __cdecl aurora_trimesh_remove_temporary_0043e490(MdlNode* node) { aurora_trimesh_remove_temporary_body_0043c090(node, nullptr); }
void __cdecl aurora_anim_mesh_gen_vertices_0043e420(MdlNode* node) { aurora_anim_mesh_gen_vertices_body_0043d450(node, nullptr); }
void __cdecl aurora_anim_mesh_remove_temporary_0043e430(MdlNode* node) { aurora_anim_mesh_remove_temporary_body_0043c320(node, nullptr); }
void __cdecl aurora_skin_gen_vertices_0043e440(MdlNode* node) { aurora_skin_gen_vertices_body_004885e0(node, nullptr); }
void __cdecl aurora_skin_remove_temporary_0043e450(MdlNode* node) { aurora_skin_remove_temporary_body_0043c1a0(node, nullptr); }
void __cdecl aurora_dangly_remove_temporary_0043e460(MdlNode* node) { aurora_dangly_remove_temporary_body_0043c130(node, nullptr); }
void __cdecl aurora_dangly_gen_vertices_0043e470(MdlNode* node) { aurora_dangly_gen_vertices_body_0043bd40(node, nullptr); }

namespace {
void set_mesh_fns(MdlNode* node, NodeMeshFn gen, NodeMeshFn remove) {
    put<NodeMeshFn>(node, 0x50, gen);
    put<NodeMeshFn>(node, 0x54, remove);
}
// Inlined SafePointer<CAurTexture>::operator= (004A06DE..004A0788 / 004A082F..004A08D7): unlink the slot
// from the old texture's referrer list at +4 (reverse search + shift = 00445080 RemoveReverse32), store
// the new texture, then append the slot to the new texture's list (growth cap ? 2*cap : 16, same
// algorithm as 005E03C0: capacity stored, new(cap*4), copy, free old).
void assign_texture(U* slot, void* texture) {
    void* const old = ptr<void>(*slot);                                             // 004A06DE
    if (old != nullptr) list_at(old, 4)->RemoveReverse32(addr(slot));              // 004A06E9..004A0725
    *slot = addr(texture);                                                          // 004A0729
    if (texture != nullptr) {                                                       // 004A0727 / 004A072B
        List* const l = list_at(texture, 4);
        if (l->size == l->capacity)                                                 // 004A072D..004A0733
            menu_list_growth::allocate_005e03c0(l, nullptr, l->capacity ? static_cast<U>(l->capacity) * 2u : 0x10u); // 004A0735..004A0777
        ptr<U>(l->data)[l->size] = addr(slot);                                      // 004A077F..004A0785
        l->size = l->size + 1;                                                      // 004A0788
    }
}
// Reverse texture-reference loop shared by 004A06B2..004A0791 and 004A0803..004A08E0.
void reference_textures(MdlNode* node) {
    I i = get<I>(node, 0x88);
    if (i == 0) return;
    do {
        --i;                                                                        // 004A06C9 / 004A081A
        U* const slot = ptr<U>(get<U>(node, 0x54)) + i;                             // 004A06C6..004A06D7
        const char* const name = ptr<const char*>(get<U>(node, 0x84))[i];          // 004A06C0 / 004A06D1
        font::Texture* const texture = font::reference_00423a60(name, nullptr);                    // 004A06D4..004A06D9
        assign_texture(slot, texture);
    } while (i != 0);                                                               // 004A078B..004A0791
}
}

// ---------------------------------------------------------------- node relocation (InputBinary, thiscall RET 0Ch)
// 004A0B60 ResetMdlNodeParts(node, base, mdx).
void __fastcall aurora_input_binary_reset_node_parts_004a0b60(InputObject* self, void*, MdlNode* node, MaxTree* base, unsigned char* mdx) {
    const U b = addr(base);
    rel(node, 0x08, b);                                                             // 004A0B6B..004A0B7C owner tree
    if (get<U>(node, 0x0C) != 0) rel(node, 0x0C, b); else put<U>(node, 0x0C, 0);   // 004A0B6E..004A0B88 parent
    if (get<I>(node, 0x30) != 0) {                                                  // 004A0B8B..004A0B90
        rel(node, 0x2C, b);                                                         // 004A0B92..004A0B99
        for (I i = 0; i < get<I>(node, 0x30); ++i) {                                // 004A0B9C / 004A0BC2..004A0BC8
            U* const children = words(node, 0x2C);
            children[i] = children[i] + b;                                          // 004A0BA3..004A0BAE
            aurora_input_binary_reset_node_004a0900(self, nullptr, ptr<MdlNode>(words(node, 0x2C)[i]), base, mdx); // 004A0BB0..004A0BBD
        }
    } else {
        put<U>(node, 0x2C, 0);                                                      // 004A0BCF
    }
    rel_if(node, 0x38, 0x3C, b);                                                    // 004A0BD2..004A0BDC controllers
    rel_if(node, 0x44, 0x48, b);                                                    // 004A0BDF..004A0BF3 controller data
}
// 004A0160 ResetMdlNodePartsLite(node, base, mdx): children only, through ResetMdlNodeLite.
void __fastcall aurora_input_binary_reset_node_parts_lite_004a0160(InputObject* self, void*, MdlNode* node, MaxTree* base, unsigned char* mdx) {
    if (get<I>(node, 0x30) == 0) return;                                            // 004A0166..004A016D
    for (I i = 0; i < get<I>(node, 0x30); ++i)                                      // 004A0172 / 004A0194..004A019A
        aurora_input_binary_reset_node_lite_004a0a90(self, nullptr, ptr<MdlNode>(words(node, 0x2C)[i]), base, mdx); // 004A0180..004A018F
}

namespace {
// Index-buffer upload shared by 004A0CE0..004A0D7B (full) and 004A0E60..004A0EFA (lite).
void upload_index_lists(MdlNode* node, U b) {
    for (I i = 0; i < get<I>(node, 0x11C); ++i) {                                   // 004A0D68..004A0D7B (count re-read)
        const U source = words(node, 0x10C)[i];                                     // 004A0CF3
        aurora_pool_request_00423ee0(1, words(node, 0x100)[i] << 1, 2, 0, words(node, 0x118) + i); // 004A0CF8..004A0D0A
        void* const dst = aurora_pool_lock_00423f10(words(node, 0x118)[i], 0, 0, 1); // 004A0D0F..004A0D1F
        std::memcpy(dst, ptr<unsigned char>(source + b), words(node, 0x100)[i] << 1); // 004A0D24..004A0D3F
        aurora_pool_unlock_00423f30(words(node, 0x118)[i]);                         // 004A0D41..004A0D4B
        aurora_node_add_free_token_00445310(node, nullptr, words(node, 0x118)[i]);  // 004A0D50..004A0D5F
    }
    if (get<I>(node, 0x128) != -1) {                                                // 004A0D81..004A0D90
        const U source = get<U>(node, 0x124);                                       // 004A0D92
        aurora_pool_request_00423ee0(1, get<U>(node, 0x12C) << 1, 2, 0, ptr<U>(addr(at(node, 0x128)))); // 004A0D98..004A0DA7
        void* const dst = aurora_pool_lock_00423f10(get<U>(node, 0x128), 0, 0, 1);  // 004A0DAC..004A0DB6
        std::memcpy(dst, ptr<unsigned char>(source + b), get<U>(node, 0x12C) << 1); // 004A0DBB..004A0DD3
        aurora_pool_unlock_00423f30(get<U>(node, 0x128));                           // 004A0DD5..004A0DD9
        aurora_node_add_free_token_00445310(node, nullptr, get<U>(node, 0x128));    // 004A0DDE..004A0DE7
    }
    const I vertices = static_cast<I>(get<I16>(node, 0x180));                       // 004A0DEC
    aurora_mesh_vertices_00832c80 = aurora_mesh_vertices_00832c80 + vertices;       // 004A0DF3..004A0E00
    aurora_mesh_vertex_bytes_00832c88 = static_cast<I>(static_cast<U>(aurora_mesh_vertex_bytes_00832c88) +
        static_cast<U>(get<I>(node, 0x14C)) * static_cast<U>(static_cast<I>(get<I16>(node, 0x180)))); // 004A0DFB..004A0E1B
}
}

// 004A0C00 ResetTriMeshParts(node, base, mdx).
void __fastcall aurora_input_binary_reset_trimesh_parts_004a0c00(InputObject* self, void*, MdlNode* node, MaxTree* base, unsigned char* mdx) {
    const U b = addr(base);
    aurora_input_binary_reset_node_parts_004a0b60(self, nullptr, node, base, mdx);  // 004A0C10..004A0C13
    if (get<I16>(node, 0x180) > 0) rel(node, 0x198, b); else put<U>(node, 0x198, 0); // 004A0C1A..004A0C2B
    rel_if(node, 0x58, 0x5C, b);                                                    // 004A0C31..004A0C3B faces
    const I count_e8 = get<I>(node, 0xEC);                                          // 004A0C3E
    if (count_e8 != 0) rel(node, 0xE8, b); else put<U>(node, 0xE8, 0);             // 004A0C44..004A0C50
    rel_if(node, 0x100, 0x104, b);                                                  // 004A0C56..004A0C66
    if (count_e8 > 0)                                                               // 004A0C6E..004A0C70
        for (I i = 0; i < get<I>(node, 0xEC); ++i) words(node, 0xE8)[i] += b;      // 004A0C72..004A0C8B
    rel_if(node, 0xF4, 0xF8, b);                                                    // 004A0C8D..004A0C9D
    const I count_118 = get<I>(node, 0x11C);                                        // 004A0CA3
    if (count_118 != 0) rel(node, 0x118, b); else put<U>(node, 0x118, 0);          // 004A0CA9..004A0CB5
    rel_if(node, 0x10C, 0x110, b);                                                  // 004A0CBB..004A0CCB
    upload_index_lists(node, b);                                                    // 004A0CD1..004A0E21
}
// 004A0E30 ResetTriMeshPartsLite(node, base, mdx): PartsLite, then the same uploads (no relocation).
void __fastcall aurora_input_binary_reset_trimesh_parts_lite_004a0e30(InputObject* self, void*, MdlNode* node, MaxTree* base, unsigned char* mdx) {
    aurora_input_binary_reset_node_parts_lite_004a0160(self, nullptr, node, base, mdx); // 004A0E40..004A0E43
    upload_index_lists(node, addr(base));                                           // 004A0E48..004A0FA5
}
// 004A0240 ResetTriMesh: [50]/[54] = 0043E480/0043E490; jmp ResetTriMeshParts.
void __fastcall aurora_input_binary_reset_trimesh_004a0240(InputObject* self, void*, MdlNode* node, MaxTree* base, unsigned char* mdx) {
    set_mesh_fns(node, &aurora_trimesh_gen_vertices_0043e480, &aurora_trimesh_remove_temporary_0043e490);
    aurora_input_binary_reset_trimesh_parts_004a0c00(self, nullptr, node, base, mdx);
}
// 004A0060 ResetAnim (animmesh): relocate 1A0/1AC/1B8/1C4/1C8 by counts 1A4/1B0/1BC/1CC/1D0;
// [50]/[54] = 0043E420/0043E430; ResetTriMeshParts.
void __fastcall aurora_input_binary_reset_anim_004a0060(InputObject* self, void*, MdlNode* node, MaxTree* base, unsigned char* mdx) {
    const U b = addr(base);
    rel_if(node, 0x1A0, 0x1A4, b);                                                  // 004A0064..004A007D
    rel_if(node, 0x1AC, 0x1B0, b);                                                  // 004A0083..004A0093
    rel_if(node, 0x1B8, 0x1BC, b);                                                  // 004A0099..004A00A9
    rel_if(node, 0x1C4, 0x1CC, b);                                                  // 004A00AF..004A00BF
    rel_if(node, 0x1C8, 0x1D0, b);                                                  // 004A00C5..004A00D5
    set_mesh_fns(node, &aurora_anim_mesh_gen_vertices_0043e420, &aurora_anim_mesh_remove_temporary_0043e430); // 004A00E2..004A00E9
    aurora_input_binary_reset_trimesh_parts_004a0c00(self, nullptr, node, base, mdx); // 004A00F0
}
// 004A0100 ResetDangly: ResetTriMeshParts first; [50]/[54] = 0043E470/0043E460; 19C by 1A0;
// (i16)[180] > 0 -> [1B4] += base (no zeroing).
void __fastcall aurora_input_binary_reset_dangly_004a0100(InputObject* self, void*, MdlNode* node, MaxTree* base, unsigned char* mdx) {
    const U b = addr(base);
    aurora_input_binary_reset_trimesh_parts_004a0c00(self, nullptr, node, base, mdx); // 004A010E..004A0111
    set_mesh_fns(node, &aurora_dangly_gen_vertices_0043e470, &aurora_dangly_remove_temporary_0043e460); // 004A011E..004A0125
    rel_if(node, 0x19C, 0x1A0, b);                                                  // 004A0116..004A0136
    if (get<I16>(node, 0x180) > 0) rel(node, 0x1B4, b);                             // 004A0140..004A014A
}
// 004A01B0 ResetSkin: ResetTriMeshParts; [50]/[54] = 0043E440/0043E450; 1B0/1B8/1C4/1D0 by 1B4/1BC/1C8/1D4.
void __fastcall aurora_input_binary_reset_skin_004a01b0(InputObject* self, void*, MdlNode* node, MaxTree* base, unsigned char* mdx) {
    const U b = addr(base);
    aurora_input_binary_reset_trimesh_parts_004a0c00(self, nullptr, node, base, mdx); // 004A01BE..004A01C1
    set_mesh_fns(node, &aurora_skin_gen_vertices_0043e440, &aurora_skin_remove_temporary_0043e450); // 004A01D0..004A01D7
    rel_if(node, 0x1B0, 0x1B4, b);                                                  // 004A01C6..004A01E8
    rel_if(node, 0x1B8, 0x1BC, b);                                                  // 004A01EE..004A01FE
    rel_if(node, 0x1C4, 0x1C8, b);                                                  // 004A0204..004A0214
    rel_if(node, 0x1D0, 0x1D4, b);                                                  // 004A021A..004A022B
}
// 004A0260 ResetAABBTree(aabb, base, mdx): left [+18] / right [+1C] child offsets; relocate and recurse
// on the left child, iterate on the right child (the original unrolls three levels, tail-loops at 004A0447).
void __fastcall aurora_input_binary_reset_aabb_tree_004a0260(InputObject* self, void*, unsigned char* aabb, MaxTree* base, unsigned char* mdx) {
    const U b = addr(base);
    for (;;) {
        if (get<U>(aabb, 0x18) != 0) {                                              // 004A0275..004A027A
            const U left = rel(aabb, 0x18, b);                                      // 004A0280..004A0283
            aurora_input_binary_reset_aabb_tree_004a0260(self, nullptr, ptr<unsigned char>(left), base, mdx);
        }
        if (get<U>(aabb, 0x1C) == 0) return;                                        // 004A0364..004A0369
        aabb = ptr<unsigned char>(rel(aabb, 0x1C, b));                              // 004A036F..004A0372 / 004A043E..004A0447
    }
}
// 004A0460 ResetLightsaber: ResetTriMeshParts; [50]/[54] = 0043E480/0043E490; pool requests
// (0, (i16)[00740A50]*32, 2, 1012h, &[1A8]) and (0, (i16)[00740A4C]*(i16)[00740A54]*20, 2, 1010h, &[1AC]);
// then 19C/1A0/1A4 += base unconditionally.
void __fastcall aurora_input_binary_reset_saber_004a0460(InputObject* self, void*, MdlNode* node, MaxTree* base, unsigned char* mdx) {
    const U b = addr(base);
    aurora_input_binary_reset_trimesh_parts_004a0c00(self, nullptr, node, base, mdx); // 004A046E..004A0471
    set_mesh_fns(node, &aurora_trimesh_gen_vertices_0043e480, &aurora_trimesh_remove_temporary_0043e490); // 004A047D..004A0484
    aurora_pool_request_00423ee0(0, static_cast<U>(static_cast<I>(kSaberWord50)) << 5, 2, 0x1012, ptr<U>(addr(at(node, 0x1A8)))); // 004A048B..004A049F
    aurora_pool_request_00423ee0(0, static_cast<U>(static_cast<I>(kSaberWord4C) * static_cast<I>(kSaberWord54) * 5) << 2, 2, 0x1010,
                                 ptr<U>(addr(at(node, 0x1AC))));                    // 004A04A4..004A04CC
    const U a = get<U>(node, 0x19C) + b, c = get<U>(node, 0x1A0) + b, d = get<U>(node, 0x1A4) + b; // 004A04D1..004A04EA
    put<U>(node, 0x19C, a); put<U>(node, 0x1A0, c); put<U>(node, 0x1A4, d);         // 004A04ED..004A04F9
}
// 004A0510 (lightsaber lite): ResetTriMeshPartsLite; the same two pool requests.
void __fastcall aurora_input_binary_reset_saber_lite_004a0510(InputObject* self, void*, MdlNode* node, MaxTree* base, unsigned char* mdx) {
    aurora_input_binary_reset_trimesh_parts_lite_004a0e30(self, nullptr, node, base, mdx); // 004A051D..004A0520
    aurora_pool_request_00423ee0(0, static_cast<U>(static_cast<I>(kSaberWord50)) << 5, 2, 0x1012, ptr<U>(addr(at(node, 0x1A8)))); // 004A0525..004A0540
    aurora_pool_request_00423ee0(0, static_cast<U>(static_cast<I>(kSaberWord4C) * static_cast<I>(kSaberWord54) * 5) << 2, 2, 0x1010,
                                 ptr<U>(addr(at(node, 0x1AC))));                    // 004A0545..004A056D
}
// 004A05E0 ResetLight(node, base, mdx): ResetMdlNodeParts; flare-name table [84] (count [88]) relocated
// with each entry; 60/6C/78 by 64/70/7C. SafePointer<CAurTexture> list at +54: count 0 -> inlined
// SetSize(0) (eh vector destructor 006FA852 with 00422380, free(data-4), data = 0, capacity = 0);
// else CExoArrayList<SafePointer>::allocate 0043DE00(count); size = count. Then textures referenced in
// reverse order (AurTextureGetReference(name, 0) 00423A60) and MdlNode::AddToFreeList(node, node).
void __fastcall aurora_input_binary_reset_light_004a05e0(InputObject* self, void*, MdlNode* node, MaxTree* base, unsigned char* mdx) {
    const U b = addr(base);
    aurora_input_binary_reset_node_parts_004a0b60(self, nullptr, node, base, mdx);  // 004A05F0..004A05F3
    if (get<I>(node, 0x88) != 0) {                                                  // 004A05F8..004A0602
        rel(node, 0x84, b);                                                         // 004A0604..004A0610
        for (I i = 0; i < get<I>(node, 0x88); ++i) words(node, 0x84)[i] += b;      // 004A0616..004A0639
    } else {
        put<U>(node, 0x84, 0);                                                      // 004A063D
    }
    rel_if(node, 0x60, 0x64, b);                                                    // 004A0643..004A064D
    rel_if(node, 0x6C, 0x70, b);                                                    // 004A0650..004A065A
    rel_if(node, 0x78, 0x7C, b);                                                    // 004A065D..004A0667
    const I count = get<I>(node, 0x88);                                             // 004A066A
    List* const textures = list_at(node, 0x54);                                     // 004A0672
    if (count == 0) {
        if (textures->data != 0) {                                                  // 004A0677..004A067B
            const U data = textures->data;
            c2da_destroy_array_006fa852(ptr<void>(data), 4, ptr<I>(data)[-1], &aurora_texture_safe_pointer_destroy_00422380); // 004A067D..004A068C
            live_free(ptr<void>(data - 4));                                         // 004A0691..004A0692
            textures->data = 0;                                                     // 004A069A
        }
        textures->capacity = 0;                                                     // 004A06A2
    } else {
        aurora_texture_ptr_list_allocate_0043de00(textures, nullptr, count);        // 004A06A7..004A06AA
    }
    textures->size = count;                                                         // 004A06AF
    reference_textures(node);                                                       // 004A06B2..004A0791
    aurora_node_add_free_list_004452d0(node, nullptr, node);                        // 004A0797..004A079A
}
// 004A07B0 (light lite): children via ResetMdlNodeLite; SetSize 0043DF60(+54, [88]); reverse texture
// references; MdlNode::AddToFreeList(node, node).
void __fastcall aurora_input_binary_reset_light_lite_004a07b0(InputObject* self, void*, MdlNode* node, MaxTree* base, unsigned char* mdx) {
    if (get<I>(node, 0x30) != 0)                                                    // 004A07B4..004A07BF
        for (I i = 0; i < get<I>(node, 0x30); ++i)                                  // 004A07C1 / 004A07E8..004A07EE
            aurora_input_binary_reset_node_lite_004a0a90(self, nullptr, ptr<MdlNode>(words(node, 0x2C)[i]), base, mdx); // 004A07D0..004A07DF
    aurora_texture_ptr_list_set_size_0043df60(at(node, 0x54), nullptr, get<I>(node, 0x88)); // 004A07F0..004A07FE
    reference_textures(node);                                                       // 004A0803..004A08E0
    aurora_node_add_free_list_004452d0(node, nullptr, node);                        // 004A08E6..004A08E9
}

// 004A0900 ResetMdlNode(node, base, mdx): exact type-word dispatch (compares with .rdata words 00740A18..44).
void __fastcall aurora_input_binary_reset_node_004a0900(InputObject* self, void*, MdlNode* node, MaxTree* base, unsigned char* mdx) {
    const U16 type = get<U16>(node, 0);                                             // 004A0905
    if (type == kNodeAABB) {                                                        // 004A0908
        const U aabb = rel(node, 0x19C, addr(base));                                // 004A0914..004A092B
        aurora_input_binary_reset_aabb_tree_004a0260(self, nullptr, ptr<unsigned char>(aabb), base, mdx); // 004A0926..004A0931
        set_mesh_fns(node, &aurora_trimesh_gen_vertices_0043e480, &aurora_trimesh_remove_temporary_0043e490); // 004A093B..004A0942
        aurora_input_binary_reset_trimesh_parts_004a0c00(self, nullptr, node, base, mdx); // 004A0936..004A0949
    } else if (type == kNodeAnimMesh) {                                             // 004A0955
        aurora_input_binary_reset_anim_004a0060(self, nullptr, node, base, mdx);
    } else if (type == kNodeHeader || type == kNodeEmitter) {                       // 004A0975 / 004A09D5
        aurora_input_binary_reset_node_parts_004a0b60(self, nullptr, node, base, mdx);
    } else if (type == kNodeCamera || type == kNodeTrigger) {                       // 004A0995 / 004A0A3E
        aurora_input_binary_reset_node_parts_004a0b60(self, nullptr, node, base, mdx);
    } else if (type == kNodeDangly) {                                               // 004A09B5
        aurora_input_binary_reset_dangly_004a0100(self, nullptr, node, base, mdx);
    } else if (type == kNodeLight) {                                                // 004A09DE
        aurora_input_binary_reset_light_004a05e0(self, nullptr, node, base, mdx);
    } else if (type == kNodeReference) {                                            // 004A09FE
        aurora_input_binary_reset_node_parts_004a0b60(self, nullptr, node, base, mdx);
    } else if (type == kNodeSkin) {                                                 // 004A0A1E
        aurora_input_binary_reset_skin_004a01b0(self, nullptr, node, base, mdx);
    } else if (type == kNodeSaber) {                                                // 004A0A4B
        aurora_input_binary_reset_saber_004a0460(self, nullptr, node, base, mdx);
    } else if (type == kNodeTriMesh) {                                              // 004A0A6B
        aurora_input_binary_reset_trimesh_004a0240(self, nullptr, node, base, mdx);
    }
}
// 004A0A90 ResetMdlNodeLite(node, base, mdx): tail-jump dispatch (same type words).
void __fastcall aurora_input_binary_reset_node_lite_004a0a90(InputObject* self, void*, MdlNode* node, MaxTree* base, unsigned char* mdx) {
    const U16 type = get<U16>(node, 0);                                             // 004A0A94
    if (type == kNodeAABB || type == kNodeAnimMesh || type == kNodeDangly || type == kNodeSkin || type == kNodeTriMesh)
        aurora_input_binary_reset_trimesh_parts_lite_004a0e30(self, nullptr, node, base, mdx); // 004A0A97/0AA4/0AD5/0B14/0B41 -> 004A0B4A
    else if (type == kNodeHeader || type == kNodeCamera || type == kNodeEmitter || type == kNodeReference || type == kNodeTrigger)
        aurora_input_binary_reset_node_parts_lite_004a0160(self, nullptr, node, base, mdx); // 004A0AB1/0AC3/0ADE/0B02/0B1D
    else if (type == kNodeLight)
        aurora_input_binary_reset_light_lite_004a07b0(self, nullptr, node, base, mdx);  // 004A0AF0..004A0AFD
    else if (type == kNodeSaber)
        aurora_input_binary_reset_saber_lite_004a0510(self, nullptr, node, base, mdx);  // 004A0B2F..004A0B3C
}

// 004A0FB0 ResetAnimation(anim, base, mdx): [84] = base (owner model); events [78] by [7C]; root [28] += base;
// ResetMdlNode(root); UpdateAnimFootprint(root, 1); [007A6990] += 8C, then += [7C]*36;
// [0]/[4] = 0044A8E0/0043E190.
void __fastcall aurora_input_binary_reset_animation_004a0fb0(InputObject* self, void*, MaxTree* anim, MaxTree* base, unsigned char* mdx) {
    const U b = addr(base);
    put<U>(anim, 0x84, b);                                                          // 004A0FBF
    rel_if(anim, 0x78, 0x7C, b);                                                    // 004A0FB9..004A0FCC
    const U root = rel(anim, 0x28, b);                                              // 004A0FD3..004A0FDF
    aurora_input_binary_reset_node_004a0900(self, nullptr, ptr<MdlNode>(root), base, mdx); // 004A0FDA..004A0FE2
    aurora_update_anim_footprint_00437db0(get<MdlNode*>(anim, 0x28), 1);            // 004A0FE7..004A0FED
    aurora_anim_footprint_007a6990 = aurora_anim_footprint_007a6990 + 0x8C;          // 004A0FF2..004A0FFE
    aurora_anim_footprint_007a6990 = aurora_anim_footprint_007a6990 + get<U>(anim, 0x7C) * 36u; // 004A1004..004A1010
    put<TreeDestroyFn>(anim, 0, &aurora_maxtree_destructor_0044a8e0);                // 004A1016
    put<TreeParseFn>(anim, 4, &aurora_maxtree_parse_field_0043e190);                 // 004A101C
}

// 004A1030 InputBinary::Reset(tree, mdx, res) -> tree.
MaxTree* __fastcall aurora_input_binary_reset_004a1030(InputObject* self, void*, MaxTree* tree, unsigned char* mdx, Resource* res) {
    const U b = addr(tree);
    put<Resource*>(tree, 0x48, res);                                                // 004A1041
    const U8 type = static_cast<U8>(aurora_tree_get_type_0043e130(tree, nullptr));  // 004A1044
    put<U8>(tree, 0x4C, static_cast<U8>(type | 0x80));                              // 004A104F..004A1052
    MaxTree* const super = aurora_find_model_00464110(reinterpret_cast<const char*>(at(tree, 0x88))); // 004A1049..004A1055
    put<MaxTree*>(tree, 0x64, super);                                               // 004A105F
    if (super != nullptr) model_add_ref_0043e240(super, nullptr, tree);      // 004A1062..004A1067
    aurora_tree_reset_free_lists_004451d0(tree, nullptr);                           // 004A106E..004A1070
    const U mdx_offset = get<U>(tree, 0xAC);                                        // 004A107B
    aurora_pool_request_00423ee0(0, get<U>(tree, 0xB0), 2, 0, ptr<U>(addr(at(tree, 0xAC)))); // 004A1075..004A108F
    void* const dst = aurora_pool_lock_00423f10(get<U>(tree, 0xAC), 0, 0, 1);       // 004A1094..004A109E
    std::memcpy(dst, mdx + mdx_offset, get<U>(tree, 0xB0));                         // 004A10A3..004A10BD
    aurora_pool_unlock_00423f30(get<U>(tree, 0xAC));                                // 004A10BF..004A10C3
    aurora_tree_add_free_token_00445190(tree, nullptr, get<U>(tree, 0xAC));         // 004A10C8..004A10D1
    const I names = get<I>(tree, 0xBC);                                             // 004A10D6
    if (names != 0) rel(tree, 0xB8, b); else put<U>(tree, 0xB8, 0);                 // 004A10DE..004A10EA
    if (names > 0)                                                                  // 004A10F2..004A10F4
        for (I i = 0; i < get<I>(tree, 0xBC); ++i) words(tree, 0xB8)[i] += b;      // 004A1100..004A1119
    const I anims = get<I>(tree, 0x5C);                                             // 004A111B
    if (anims != 0) rel(tree, 0x58, b); else put<U>(tree, 0x58, 0);                 // 004A111E..004A1127
    if (anims > 0)                                                                  // 004A112C..004A112E
        for (I i = 0; i < get<I>(tree, 0x5C); ++i) {                                // 004A114E..004A1154
            words(tree, 0x58)[i] += b;                                              // 004A1130..004A1136
            aurora_input_binary_reset_animation_004a0fb0(self, nullptr, ptr<MaxTree>(words(tree, 0x58)[i]), tree, mdx); // 004A1138..004A1149
        }
    if (get<U>(tree, 0xA8) != 0) rel(tree, 0xA8, b);                                // 004A1156..004A1164
    const U root = rel(tree, 0x28, b);                                              // 004A116A..004A117C
    aurora_input_binary_reset_node_004a0900(self, nullptr, ptr<MdlNode>(root), tree, mdx); // 004A116D..004A117F
    put<TreeDestroyFn>(tree, 0, &aurora_model_destructor_0044aa60);                  // 004A1185
    put<TreeParseFn>(tree, 4, &aurora_model_parse_field_0043e1e0);                   // 004A118B
    aurora_insert_model_0046a5f0(tree);                                             // 004A1184..004A1192
    return tree;
}
// 004A11B0 InputBinary::ResetLite(tree, mdx, res) -> tree (res unused): supermodel lookup/AddRef,
// ResetFreeLists, per animation ResetMdlNodeLite + UpdateAnimFootprint(root, 1) + footprint 8C + [7C]*36,
// ResetMdlNodeLite(tree root), InsertModel.
MaxTree* __fastcall aurora_input_binary_reset_lite_004a11b0(InputObject* self, void*, MaxTree* tree, unsigned char* mdx, Resource*) {
    MaxTree* const super = aurora_find_model_00464110(reinterpret_cast<const char*>(at(tree, 0x88))); // 004A11B7..004A11C0
    put<MaxTree*>(tree, 0x64, super);                                               // 004A11CA
    if (super != nullptr) model_add_ref_0043e240(super, nullptr, tree);      // 004A11CD..004A11D2
    aurora_tree_reset_free_lists_004451d0(tree, nullptr);                           // 004A11D7..004A11D9
    for (I i = 0; i < get<I>(tree, 0x5C); ++i) {                                    // 004A11DE..004A11E5 / 004A1232..004A1238
        MaxTree* const anim = ptr<MaxTree>(words(tree, 0x58)[i]);                   // 004A11F0..004A11F7
        aurora_input_binary_reset_node_lite_004a0a90(self, nullptr, get<MdlNode*>(anim, 0x28), tree, mdx); // 004A11FA..004A1202
        aurora_update_anim_footprint_00437db0(get<MdlNode*>(anim, 0x28), 1);        // 004A1207..004A120D
        aurora_anim_footprint_007a6990 = aurora_anim_footprint_007a6990 + 0x8C;      // 004A1212..004A121C
        aurora_anim_footprint_007a6990 = aurora_anim_footprint_007a6990 + get<U>(anim, 0x7C) * 36u; // 004A1221..004A122D
    }
    aurora_input_binary_reset_node_lite_004a0a90(self, nullptr, get<MdlNode*>(tree, 0x28), tree, mdx); // 004A123B..004A1247
    aurora_insert_model_0046a5f0(tree);                                             // 004A124C..004A124D
    return tree;
}

// 004A1260 InputBinary::Read(mdl, mdx, lite) -> tree.
// [this+0] = mdl data size, [this+4] = mdx size (two 4-byte reads from the MDL descriptor, no null check
// on the second); copy MDL data into new(size); copy MDX data (if size != 0) into new(size); the MDX
// bytes are released through the MDL descriptor (004A1330..004A1332, original quirk); Reset/ResetLite;
// AsModel(tree)+B4 = mdx descriptor, AsModel(tree)+48 = mdl descriptor; AurResFree(mdl, 0);
// AurResRemove(mdx); free(mdx copy). The MDL copy becomes the model image.
MaxTree* __fastcall aurora_input_binary_read_004a1260(InputObject* self, void*, Resource* mdl, Resource* mdx, U8 lite) {
    unsigned char* const first = static_cast<unsigned char*>(aurres_data_bytes_0044bd10(4, mdl)); // 004A1269..004A126E
    if (first == nullptr) return nullptr;                                           // 004A1276..004A127F
    put<U>(self, 0, get<U>(first, 0));                                              // 004A1282..004A1288
    aurres_free_data_bytes_0044bd50(mdl, first);                                    // 004A1286..004A128A
    unsigned char* const second = static_cast<unsigned char*>(aurres_data_bytes_0044bd10(4, mdl)); // 004A128F..004A1292
    put<U>(self, 4, get<U>(second, 0));                                             // 004A1297..004A129B
    aurres_free_data_bytes_0044bd50(mdl, second);                                   // 004A1299..004A129E
    unsigned char* const mdl_bytes = static_cast<unsigned char*>(aurres_data_bytes_0044bd10(get<U>(self, 0), mdl)); // 004A12A3..004A12A7
    unsigned char* const image = static_cast<unsigned char*>(live_operator_new(get<U>(self, 0))); // 004A12AC..004A12B5
    std::memcpy(image, mdl_bytes, get<U>(self, 0));                                 // 004A12BA..004A12D4
    aurres_free_data_bytes_0044bd50(mdl, mdl_bytes);                                // 004A12C9..004A12D6
    unsigned char* const mdx_bytes = get<U>(self, 4) != 0                           // 004A12DB..004A12E5
        ? static_cast<unsigned char*>(aurres_data_bytes_0044bd10(get<U>(self, 4), mdx)) : nullptr; // 004A12E7..004A12FB
    unsigned char* mdx_copy = nullptr;
    if (get<U>(self, 4) != 0) {                                                     // 004A12FF..004A1304
        mdx_copy = static_cast<unsigned char*>(live_operator_new(get<U>(self, 4)));  // 004A1306..004A1307
        std::memcpy(mdx_copy, mdx_bytes, get<U>(self, 4));                          // 004A130C..004A1324
    }
    aurres_free_data_bytes_0044bd50(mdl, mdx_bytes);                                // 004A132C..004A1332 (MDL descriptor)
    MaxTree* const tree = lite == 0                                                 // 004A1337..004A1342
        ? aurora_input_binary_reset_004a1030(self, nullptr, ptr<MaxTree>(addr(image)), mdx_copy, mdl)       // 004A1344..004A134B
        : aurora_input_binary_reset_lite_004a11b0(self, nullptr, ptr<MaxTree>(addr(image)), mdx_copy, mdl); // 004A1352..004A1359
    put<Resource*>(aurora_tree_as_model_0043e1c0(tree, nullptr), 0xB4, mdx);       // 004A1360..004A136D
    put<Resource*>(aurora_tree_as_model_0043e1c0(tree, nullptr), 0x48, mdl);       // 004A1373..004A137B
    aurres_release_0044c8f0(mdl, 0);                                                // 004A1378..004A137E
    aurres_remove_0044bdc0(mdx);                                                    // 004A1383..004A1384
    live_free(mdx_copy);                                                            // 004A1389..004A138A
    return tree;
}
