// model_destroy.cpp - lane-3d-model (D39 handoff "model tree/lifetime"): model / tree / node destruction.
// MaxTree::operator delete 0044AEC0, Model::~Model 0043F790, MaxTree::~MaxTree 0043F730, FreeTheFreeList 0044A9C0,
// Model::InternalDestructor 00449F60, MdlNode::DestructorDispatch 004488F0 / InternalDestructor 0043FDD0,
// MdlNode::~MdlNode 0044A040 and the TriMesh/AnimMesh/Skin/Dangly/AABB/Trigger destructors, MaxFace::~MaxFace.
// Light / Lightsaber / AABB-tree / Animation destructors belong to the lights, core and animation sub-lanes (names
// as declared by lane-3d-construct where present). Unwind states cited, not emitted (D30).
#include "a7e200e217-model_instance.hpp"
#include "2f42e97d01-storage.h"

using namespace aurora_model;

extern "C" {
volatile I aurora_face_count_007a8a00 = 0;        // --0043E140 / ++0043F71C
volatile I aurora_node_count_007a8a08 = 0;        // ++0044526C (task-34 MdlNode ctor binding counter_007a8a08) / --0043FDF1
volatile I aurora_node_created_007a8a0c = 0;      // ++004452AF (task-34 binding counter_007a8a0c) / --0044A21F
void __stdcall c2da_destroy_array_006fa852(void* first, U size, I count, void (__fastcall* destroy)(void*, void*));
// lights sub-lane (lane-3d-construct construct3d.h spellings)
void __fastcall aurora_light_node_internal_destructor_00448000(MdlNode* self, void*);
void __fastcall aurora_light_node_destruct_00448f80(MdlNode* self, void*);
void __fastcall aurora_light_node_binary_internal_destructor_0044a500(MdlNode* self, void*);
// core / animation sub-lanes (not authored here)
void __fastcall mdl_node_lightsaber_destruct_00449090(MdlNode* self, void*);
void __fastcall mdl_node_lightsaber_internal_destructor_0043fd10(MdlNode* self, void*);
void* __fastcall aabb_scalar_deleting_destructor_0043e2a0(void* self, void*, U flags);
void __fastcall animation_destruct_004850a0(MaxTree* self, void*);
// own bodies
void __fastcall aurora_node_destructor_dispatch_004488f0(MdlNode* self, void*);
void __fastcall aurora_node_internal_destructor_0043fdd0(MdlNode* self, void*);
void __fastcall aurora_node_destruct_0044a040(MdlNode* self, void*);
void __fastcall aurora_trimesh_destruct_00448bf0(MdlNode* self, void*);
void __fastcall aurora_anim_mesh_destruct_00448da0(MdlNode* self, void*);
void __fastcall aurora_skin_destruct_00448e60(MdlNode* self, void*);
void __fastcall aurora_dangly_destruct_004490f0(MdlNode* self, void*);
void __fastcall aurora_aabb_destruct_00449170(MdlNode* self, void*);
void __fastcall aurora_trigger_destruct_0043e1a0(MdlNode* self, void*);
void __fastcall aurora_maxtree_free_the_free_list_0044a9c0(MaxTree* self, void*);
void __fastcall aurora_maxtree_destruct_0043f730(MaxTree* self, void*);
void __fastcall aurora_maxface_destruct_0043e140(void* self, void*);
void __fastcall aurora_trimesh_postprocess_destroy_004453b0(void* self, void*);
}

namespace {
U16 type_word(MdlNode* n) { return get<U16>(n, 0); }
U8 type_byte(MdlNode* n) { return get<U8>(n, 0); }
void free_if(void* p, U off) { if (get<U>(p, off) != 0) live_free(ptr<void>(get<U>(p, off))); }
void free_member(void* p, U off) { if (get<U>(p, off) != 0) { live_free(ptr<void>(get<U>(p, off))); put<U>(p, off, 0); } }

// Per-child typed destructor selection shared by 0044A085..0044A208 and 0044AF3C..0044B0D2: the first matching
// inline cast (byte/word & mask == mask, in the original order) selects the class destructor; the duplicated
// second test of each arm is always true. The child block is then freed (0044A20D / 0044B0D7).
void destroy_child(MdlNode* c) {
    if ((type_byte(c) & 0x03) == 0x03) aurora_light_node_destruct_00448f80(c, nullptr);                 // 0044A08C / 0044AF3C
    else if ((type_byte(c) & 0x09) == 0x09) aurora_node_destruct_0044a040(c, nullptr);                  // 0044A0AC (0044AF74: 0043E1A0 jmp 0044A040)
    else if ((type_word(c) & 0x121) == 0x121) aurora_dangly_destruct_004490f0(c, nullptr);              // 0044A0C5 / 0044AF7E
    else if ((type_byte(c) & 0xA1) == 0xA1) aurora_anim_mesh_destruct_00448da0(c, nullptr);             // 0044A0F6 / 0044AFAF
    else if ((type_byte(c) & 0x05) == 0x05) aurora_node_destruct_0044a040(c, nullptr);                  // 0044A118 / 0044AFD1
    else if ((type_byte(c) & 0x11) == 0x11) aurora_node_destruct_0044a040(c, nullptr);                  // 0044A131 / 0044AFF1
    else if ((type_word(c) & 0x221) == 0x221) aurora_aabb_destruct_00449170(c, nullptr);                // 0044A14C / 0044B013
    else if ((type_byte(c) & 0x61) == 0x61) aurora_skin_destruct_00448e60(c, nullptr);                  // 0044A17E / 0044B045
    else if ((type_word(c) & 0x401) == 0x401) aurora_node_destruct_0044a040(c, nullptr);                // 0044A19B / 0044B062
    else if ((type_word(c) & 0x821) == 0x821) mdl_node_lightsaber_destruct_00449090(c, nullptr);        // 0044A1BE / 0044B08C
    else if ((type_byte(c) & 0x21) == 0x21) aurora_trimesh_destruct_00448bf0(c, nullptr);               // 0044A1E9 / 0044B0B7
    else aurora_node_destruct_0044a040(c, nullptr);                                                     // 0044A202..0044A208 / 0044B0D0
    live_free(c);                                                                                        // 0044A20D / 0044B0D7
}
}

// 0043E140 MaxFace::~MaxFace: --[007A8A00].
void __fastcall aurora_maxface_destruct_0043e140(void*, void*) { aurora_face_count_007a8a00 = aurora_face_count_007a8a00 - 1; }
// 0043E1A0 MdlNodeTrigger::~MdlNodeTrigger: jmp 0044A040.
void __fastcall aurora_trigger_destruct_0043e1a0(MdlNode* self, void*) { aurora_node_destruct_0044a040(self, nullptr); }

// 004488F0 MdlNode::DestructorDispatch: switch on the signed type word (jump table 00448A28 / index 00448A44 for
// 1..21h, compares above): 3 -> Light InternalDestructor 00448000; 821h -> Lightsaber 0043FD10; 1, 5, 9, 11h, 21h,
// 61h, A1h, 121h, 221h, 401h -> MdlNode::InternalDestructor 0043FDD0 (inline casts are identity for these exact
// types); any other value -> return.
void __fastcall aurora_node_destructor_dispatch_004488f0(MdlNode* self, void*) {
    switch (static_cast<I>(get<I16>(self, 0))) {                                    // 004488F0..004488F3
    case 0x003: aurora_light_node_internal_destructor_00448000(self, nullptr); return;               // 00448919..00448927
    case 0x821: mdl_node_lightsaber_internal_destructor_0043fd10(self, nullptr); return;            // 004489F6..00448A07
    case 0x001: case 0x005: case 0x009: case 0x011: case 0x021: case 0x061:
    case 0x0A1: case 0x121: case 0x221: case 0x401:
        aurora_node_internal_destructor_0043fdd0(self, nullptr); return;            // 00448A1F / 0044892C..00448A1F
    default: return;                                                                // 00448A24
    }
}
// 0043FDD0 MdlNode::InternalDestructor: dispatch each child last-to-first (count +30 read once, base re-read);
// --[007A8A08].
void __fastcall aurora_node_internal_destructor_0043fdd0(MdlNode* self, void*) {
    I n = get<I>(self, 0x30);                                                       // 0043FDD4
    while (n != 0) {                                                                // 0043FDD7 / 0043FDEF
        MdlNode* const child = ptr<MdlNode>(ptr<U>(get<U>(self, 0x2C))[n - 1]);     // 0043FDE0..0043FDE3
        --n;                                                                        // 0043FDE7
        aurora_node_destructor_dispatch_004488f0(child, nullptr);                   // 0043FDE8
    }
    aurora_node_count_007a8a08 = aurora_node_count_007a8a08 - 1;                    // 0043FDF1..0043FDF8
}
// 0044A040 MdlNode::~MdlNode: type word != 0 -> DestructorDispatch; type word = 0; destroy and free children
// last-to-first (count +30 read once); --[007A8A0C]; member lists +44, +38, +2C freed when non-null.
void __fastcall aurora_node_destruct_0044a040(MdlNode* self, void*) {
    if (get<U16>(self, 0) != 0) aurora_node_destructor_dispatch_004488f0(self, nullptr); // 0044A05D..0044A06B
    I n = get<I>(self, 0x30);                                                       // 0044A071
    put<U16>(self, 0, 0);                                                           // 0044A076
    while (n != 0) {                                                                // 0044A07B / 0044A218
        MdlNode* const child = ptr<MdlNode>(ptr<U>(get<U>(self, 0x2C))[n - 1]);     // 0044A082..0044A085
        --n;                                                                        // 0044A08B
        // A null child faults at the first type read (0044A089) before the default arm's null test (0044A202),
        // so that test never skips; destroy_child reproduces the reachable behavior.
        destroy_child(child);
    }
    aurora_node_created_007a8a0c = aurora_node_created_007a8a0c - 1;                // 0044A21F
    free_if(self, 0x44);                                                            // 0044A225..0044A233
    free_if(self, 0x38);                                                            // 0044A23B..0044A248
    free_if(self, 0x2C);                                                            // 0044A250..0044A261
}
// 00448BF0 MdlNodeTriMesh::~MdlNodeTriMesh.
void __fastcall aurora_trimesh_destruct_00448bf0(MdlNode* self, void*) {
    if (get<U16>(self, 0) != 0) aurora_node_destructor_dispatch_004488f0(self, nullptr); // 00448C0E..00448C1C
    void* const post = ptr<void>(get<U>(self, 0x134));                              // 00448C21
    put<U16>(self, 0, 0);                                                           // 00448C29
    if (post != nullptr) { aurora_trimesh_postprocess_destroy_004453b0(post, nullptr); live_free(post); } // 00448C2E..00448C38
    if (get<U>(self, 0x128) != 0xFFFFFFFFu) {                                       // 00448C40..00448C49
        aurora_pool_delete_00423f50(get<U>(self, 0x128));                           // 00448C4B..00448C4C
        put<U>(self, 0x128, 0xFFFFFFFFu);                                           // 00448C54
    }
    for (I i = 0; i < get<I>(self, 0x11C); ++i) {                                   // 00448C5E..00448C98
        aurora_pool_delete_00423f50(ptr<U>(get<U>(self, 0x118))[i]);                // 00448C70..00448C7A
        ptr<U>(get<U>(self, 0x118))[i] = 0xFFFFFFFFu;                               // 00448C7F..00448C88
    }
    for (I i = 0; i < get<I>(self, 0xEC); ++i) live_free(ptr<void>(ptr<U>(get<U>(self, 0xE8))[i])); // 00448C9A..00448CC1
    const U positions = get<U>(self, 0x198);                                        // 00448CC3
    put<U>(self, 0xEC, 0);                                                          // 00448CCA
    live_free(ptr<void>(positions));                                                // 00448CD4
    free_if(self, 0x118);                                                           // 00448CD9..00448CEC
    free_if(self, 0x10C);                                                           // 00448CF4..00448D04
    free_if(self, 0x100);                                                           // 00448D0C..00448D1C
    free_if(self, 0xF4);                                                            // 00448D24..00448D34
    free_if(self, 0xE8);                                                            // 00448D3C..00448D4C
    if (get<U>(self, 0x58) != 0) {                                                  // 00448D54..00448D5E (faces, 20h each)
        const U data = get<U>(self, 0x58);
        c2da_destroy_array_006fa852(ptr<void>(data), 0x20, ptr<I>(data)[-1], &aurora_maxface_destruct_0043e140); // 00448D60..00448D6F
        live_free(ptr<void>(data - 4));                                             // 00448D74..00448D75
    }
    aurora_node_destruct_0044a040(self, nullptr);                                   // 00448D7D..00448D87
}
// 00448DA0 MdlNodeAnimMesh::~MdlNodeAnimMesh (DestructorDispatch is called a second time after the type word
// is cleared; with type 0 it returns at 00448A24).
void __fastcall aurora_anim_mesh_destruct_00448da0(MdlNode* self, void*) {
    if (get<U16>(self, 0) != 0) aurora_node_destructor_dispatch_004488f0(self, nullptr); // 00448DBD..00448DCB
    put<U16>(self, 0, 0);                                                           // 00448DD2
    aurora_node_destructor_dispatch_004488f0(self, nullptr);                        // 00448DD0..00448DD7
    live_free(ptr<void>(get<U>(self, 0x1C4)));                                      // 00448DDC..00448DE3
    live_free(ptr<void>(get<U>(self, 0x1C8)));                                      // 00448DE8..00448DEF
    free_if(self, 0x1B8);                                                           // 00448DF4..00448E07
    free_if(self, 0x1AC);                                                           // 00448E0F..00448E1F
    free_if(self, 0x1A0);                                                           // 00448E27..00448E37
    aurora_trimesh_destruct_00448bf0(self, nullptr);                                // 00448E3F..00448E49
}
// 00448E60 MdlNodeSkin::~MdlNodeSkin.
void __fastcall aurora_skin_destruct_00448e60(MdlNode* self, void*) {
    if (get<U16>(self, 0) != 0) aurora_node_destructor_dispatch_004488f0(self, nullptr); // 00448E80..00448E8D
    const U first = get<U>(self, 0x1B0);                                            // 00448E92
    put<U16>(self, 0, 0);                                                           // 00448E9A
    if (first != 0) { live_free(ptr<void>(first)); put<U>(self, 0x1B0, 0); }        // 00448E98..00448EA8
    free_member(self, 0x1C4);                                                       // 00448EAE..00448EC1
    put<U>(self, 0x1CC, 0); put<U>(self, 0x1C8, 0);                                 // 00448EC7..00448ECD
    free_member(self, 0x1B8);                                                       // 00448ED3..00448EE6
    put<U>(self, 0x1C0, 0); put<U>(self, 0x1BC, 0);                                 // 00448EEC..00448EF2
    free_if(self, 0x1D0);                                                           // 00448EF8..00448F07
    free_if(self, 0x1C4);                                                           // 00448F10..00448F1F
    free_if(self, 0x1B8);                                                           // 00448F28..00448F37
    free_if(self, 0x19C);                                                           // 00448F40..00448F4E
    aurora_trimesh_destruct_00448bf0(self, nullptr);                                // 00448F57..00448F61
}
// 004490F0 MdlNodeDanglyMesh::~MdlNodeDanglyMesh.
void __fastcall aurora_dangly_destruct_004490f0(MdlNode* self, void*) {
    if (get<U16>(self, 0) != 0) aurora_node_destructor_dispatch_004488f0(self, nullptr); // 0044910D..0044911B
    const U constraints = get<U>(self, 0x1B4);                                      // 00449120
    put<U16>(self, 0, 0);                                                           // 00449127
    live_free(ptr<void>(constraints));                                              // 0044912C
    free_if(self, 0x19C);                                                           // 00449131..00449144
    aurora_trimesh_destruct_00448bf0(self, nullptr);                                // 0044914C..00449156
}
// 00449170 MdlNodeAABB::~MdlNodeAABB: tree root [+19C]: its +18/+1C children through AABB scalar deleting
// destructor 0043E2A0(1); free(root); then the TriMesh destructor.
void __fastcall aurora_aabb_destruct_00449170(MdlNode* self, void*) {
    if (get<U16>(self, 0) != 0) aurora_node_destructor_dispatch_004488f0(self, nullptr); // 0044918E..0044919C
    unsigned char* const root = ptr<unsigned char>(get<U>(self, 0x19C));            // 004491A1
    put<U16>(self, 0, 0);                                                           // 004491A9
    if (root != nullptr) {                                                          // 004491AE
        if (get<U>(root, 0x18) != 0) aabb_scalar_deleting_destructor_0043e2a0(ptr<void>(get<U>(root, 0x18)), nullptr, 1); // 004491B0..004491B9
        if (get<U>(root, 0x1C) != 0) aabb_scalar_deleting_destructor_0043e2a0(ptr<void>(get<U>(root, 0x1C)), nullptr, 1); // 004491BE..004491C7
        live_free(root);                                                            // 004491CC..004491CD
    }
    aurora_trimesh_destruct_00448bf0(self, nullptr);                                // 004491D5..004491DF
}

// 0043F730 MaxTree::~MaxTree: member lists +3C, +30 freed when non-null.
void __fastcall aurora_maxtree_destruct_0043f730(MaxTree* self, void*) {
    free_if(self, 0x3C);                                                            // 0043F74D..0043F75D
    free_if(self, 0x30);                                                            // 0043F765..0043F775
}
// 0044A9C0 MaxTree::FreeTheFreeList: each +30 entry through Light BinaryInternalDestructor 0044A500 (inline light
// cast), free/zero +30, sizes 0; each +3C pool handle deleted and set to -1, free/zero +3C, sizes 0.
void __fastcall aurora_maxtree_free_the_free_list_0044a9c0(MaxTree* self, void*) {
    for (I i = 0; i < get<I>(self, 0x34); ++i) {                                    // 0044A9C4..0044A9F1
        MdlNode* const n = ptr<MdlNode>(ptr<U>(get<U>(self, 0x30))[i]);             // 0044A9D0..0044A9D3
        aurora_light_node_binary_internal_destructor_0044a500((type_byte(n) & 3) == 3 ? n : nullptr, nullptr); // 0044A9D6..0044A9E6
    }
    free_member(self, 0x30);                                                        // 0044A9F3..0044AA03
    put<U>(self, 0x38, 0); put<U>(self, 0x34, 0);                                   // 0044AA06..0044AA09
    for (I i = 0; i < get<I>(self, 0x40); ++i) {                                    // 0044AA0C..0044AA34
        aurora_pool_delete_00423f50(ptr<U>(get<U>(self, 0x3C))[i]);                 // 0044AA15..0044AA1C
        ptr<U>(get<U>(self, 0x3C))[i] = 0xFFFFFFFFu;                                // 0044AA21..0044AA27
    }
    free_member(self, 0x3C);                                                        // 0044AA36..0044AA46
    put<U>(self, 0x44, 0); put<U>(self, 0x40, 0);                                   // 0044AA4A..0044AA4D
}

// 0043F790 Model::~Model: type byte != 0 -> type = 0; delete animations last-to-first through 0044AEC0 (count
// stored back each step); DeletePool([AC]); [AC] = -1; free each name-table entry (count +BC re-read).
// Member lists +B8, +58, +3C, +30 then freed when non-null.
void __fastcall aurora_model_destroy_0043f790(MaxTree* self, void*) {
    if (get<U8>(self, 0x4C) != 0) {                                                 // 0043F7AD..0043F7BA
        I n = get<I>(self, 0x5C);                                                   // 0043F7BD
        put<U8>(self, 0x4C, 0);                                                     // 0043F7C2
        while (n != 0) {                                                            // 0043F7C6 / 0043F7DE
            MaxTree* const anim = ptr<MaxTree>(ptr<U>(get<U>(self, 0x58))[n - 1]);  // 0043F7C8..0043F7CB
            --n;                                                                    // 0043F7CF
            aurora_maxtree_delete_0044aec0(anim);                                   // 0043F7D0..0043F7D1
            put<I>(self, 0x5C, n);                                                  // 0043F7DB
        }
        aurora_pool_delete_00423f50(get<U>(self, 0xAC));                            // 0043F7E0..0043F7E7
        put<U>(self, 0xAC, 0xFFFFFFFFu);                                            // 0043F7F9
        for (I i = 0; i < get<I>(self, 0xBC); ++i) live_free(ptr<void>(ptr<U>(get<U>(self, 0xB8))[i])); // 0043F7EC..0043F820
    }
    free_if(self, 0xB8);                                                            // 0043F823..0043F833
    free_if(self, 0x58);                                                            // 0043F83B..0043F848
    free_if(self, 0x3C);                                                            // 0043F850..0043F860
    free_if(self, 0x30);                                                            // 0043F868..0043F878
}

// 00449F60 Model::InternalDestructor (target of the 0044AA60 thunk): remove this model from the cache when present
// (count matches first, 00445080); release the supermodel (inline DelRef; 007A8A04 gate); footprint
// [007A6990] -= [5C]*4 + [5C]*88h; for each animation last-to-first: footprint -= [7C]*24h; UpdateAnimFootprint
// (root, -1); anim->destroy_fn(anim); finally DestructorDispatch(root) when present (tail jump).
void __fastcall aurora_model_destroy_body_00449f60(MaxTree* self, void*) {
    List* const cache = reinterpret_cast<List*>(&historical_storage_007a8a64);
    I matches = 0;
    for (I i = 0; i < cache->size; ++i) if (ptr<U>(cache->data)[i] == addr(self)) ++matches; // 00449F65..00449F89
    if (matches != 0) cache->RemoveReverse32(addr(self));                           // 00449F8B..00449F95
    MaxTree* const super = get<MaxTree*>(self, 0x64);                               // 00449F9A
    if (super != nullptr) {                                                         // 00449F9D..00449F9F
        if (aurora_model_no_delete_007a8a04 == 0) {                                 // 00449FA1..00449FA9
            const I count = get<I>(super, 0x54) - 1;                                // 00449FAB
            put<I>(super, 0x54, count);
            if (count == 0) aurora_maxtree_delete_0044aec0(super);                  // 00449FAE..00449FB1
        }
        put<U>(self, 0x64, 0);                                                      // 00449FB9
    }
    I n = get<I>(self, 0x5C);                                                       // 00449FC0
    aurora_anim_footprint_007a6990 = aurora_anim_footprint_007a6990 + static_cast<U>(-n) * 4u;         // 00449FC3..00449FD0
    aurora_anim_footprint_007a6990 = aurora_anim_footprint_007a6990 + get<U>(self, 0x5C) * 0xFFFFFF78u; // 00449FD5..00449FE2
    while (n != 0) {                                                                // 00449FE0..0044A02B
        MaxTree* const last = ptr<MaxTree>(ptr<U>(get<U>(self, 0x58))[n - 1]);      // 00449FF0..00449FF3
        aurora_anim_footprint_007a6990 = aurora_anim_footprint_007a6990 + get<U>(last, 0x7C) * 0xFFFFFFDCu; // 00449FF7..0044A006
        --n;                                                                        // 0044A003
        aurora_update_anim_footprint_00437db0(get<MdlNode*>(ptr<MaxTree>(ptr<U>(get<U>(self, 0x58))[n]), 0x28), -1); // 0044A00C..0044A018
        MaxTree* const anim = ptr<MaxTree>(ptr<U>(get<U>(self, 0x58))[n]);          // 0044A01D..0044A020
        get<TreeDestroyFn>(anim, 0)(anim);                                          // 0044A023..0044A024
    }
    MdlNode* const root = get<MdlNode*>(self, 0x28);                                // 0044A02D
    if (root != nullptr) aurora_node_destructor_dispatch_004488f0(root, nullptr);   // 0044A030..0044A037
}

// 0044AEC0 MaxTree::operator delete(tree), cdecl. Binary images (type bit 7): FreeTheFreeList; supermodel DelRef
// and clear; tree->destroy_fn(tree); AsModel(tree)+B4 (MDX descriptor) -> AurResFree(.., 0); free(tree).
// ASCII trees (type != 0): destroy_fn; root -> class destructor + free; then Model::~Model (type 2) or
// Animation::~Animation (type 5) and free; other types are not freed. Type 0 -> nothing.
void __cdecl aurora_maxtree_delete_0044aec0(MaxTree* tree) {
    if (tree == nullptr) return;                                                    // 0044AEC1..0044AEC7
    const U8 type = get<U8>(tree, 0x4C);                                            // 0044AECD
    if ((type & 0x80u) != 0) {                                                      // 0044AED0..0044AED2
        aurora_maxtree_free_the_free_list_0044a9c0(tree, nullptr);                  // 0044AED4..0044AED6
        if (get<MaxTree*>(tree, 0x64) != nullptr) {                                 // 0044AEDB..0044AEE0
            aurora_model_del_ref_0043e210(get<MaxTree*>(tree, 0x64), nullptr, tree); // 0044AEE2..0044AEE3
            put<U>(tree, 0x64, 0);                                                  // 0044AEE8
        }
        get<TreeDestroyFn>(tree, 0)(tree);                                          // 0044AEEF..0044AEF0
        MaxTree* const model = aurora_tree_as_model_0043e1c0(tree, nullptr);        // 0044AEF2..0044AF02 (inline cast)
        if (get<U>(model, 0xB4) != 0) aurres_release_0044c8f0(ptr<void>(get<U>(model, 0xB4)), 0); // 0044AF04..0044AF11
        live_free(tree);                                                            // 0044AF19..0044AF1A
        return;
    }
    if (type == 0) return;                                                          // 0044AF24
    get<TreeDestroyFn>(tree, 0)(tree);                                              // 0044AF2B..0044AF2C
    MdlNode* const root = get<MdlNode*>(tree, 0x28);                                // 0044AF2E
    if (root != nullptr) destroy_child(root);                                       // 0044AF34..0044B0DD
    const U8 now = static_cast<U8>(get<U8>(tree, 0x4C) & 0x7Fu);                    // 0044B0E0..0044B0E3
    if (now == 2) {                                                                 // 0044B0E5..0044B0F1
        aurora_model_destroy_0043f790(tree, nullptr);                               // 0044B0F7..0044B0F9
        live_free(tree);                                                            // 0044B0FE..0044B0FF
        return;
    }
    if (now == 5) {                                                                 // 0044B10A..0044B116
        animation_destruct_004850a0(tree, nullptr);                                 // 0044B11C
        live_free(tree);                                                            // 0044B121..0044B122
    }
}

// 0043FD10 MdlNodeLightsaber::InternalDestructor: delete the two pools (+1A8, +1AC; each set to -1), dispatch each
// child last-to-first (count +30 read once), --[007A8A08].
void __fastcall mdl_node_lightsaber_internal_destructor_0043fd10(MdlNode* self, void*) {
    aurora_pool_delete_00423f50(get<U>(self, 0x1A8));                               // 0043FD13..0043FD1B
    put<U>(self, 0x1A8, 0xFFFFFFFFu);                                               // 0043FD2A
    aurora_pool_delete_00423f50(get<U>(self, 0x1AC));                               // 0043FD20..0043FD30
    put<U>(self, 0x1AC, 0xFFFFFFFFu);                                               // 0043FD35
    I n = get<I>(self, 0x30);                                                       // 0043FD3B
    while (n != 0) {                                                                // 0043FD41 / 0043FD52
        MdlNode* const child = ptr<MdlNode>(ptr<U>(get<U>(self, 0x2C))[n - 1]);     // 0043FD45..0043FD48
        --n;                                                                        // 0043FD4C
        aurora_node_destructor_dispatch_004488f0(child, nullptr);                   // 0043FD4D
    }
    aurora_node_count_007a8a08 = aurora_node_count_007a8a08 - 1;                    // 0043FD56..0043FD5D
}
// 00449090 MdlNodeLightsaber::~MdlNodeLightsaber: type word != 0 -> DestructorDispatch; type word = 0; then the
// trimesh destructor 00448BF0 (EH state 0 cited).
void __fastcall mdl_node_lightsaber_destruct_00449090(MdlNode* self, void*) {
    if (get<U16>(self, 0) != 0) aurora_node_destructor_dispatch_004488f0(self, nullptr); // 004490AD..004490BB
    put<U16>(self, 0, 0);                                                           // 004490C2
    aurora_trimesh_destruct_00448bf0(self, nullptr);                                // 004490C0..004490CF
}
// 0043E2A0 AABB::`scalar deleting destructor'(flags), RET4: delete the two child boxes (+18, +1C) with flags 1,
// then free this when flags & 1; EAX = this.
void* __fastcall aabb_scalar_deleting_destructor_0043e2a0(void* self, void*, U flags) {
    if (get<U>(self, 0x18) != 0) aabb_scalar_deleting_destructor_0043e2a0(ptr<void>(get<U>(self, 0x18)), nullptr, 1); // 0043E2A3..0043E2AC
    if (get<U>(self, 0x1C) != 0) aabb_scalar_deleting_destructor_0043e2a0(ptr<void>(get<U>(self, 0x1C)), nullptr, 1); // 0043E2B1..0043E2BA
    if ((flags & 1) != 0) live_free(self);                                          // 0043E2BF..0043E2C7
    return self;                                                                    // 0043E2CF
}
