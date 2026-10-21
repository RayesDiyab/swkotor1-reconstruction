// model_postprocess.cpp - lane-3d-model: PartSkin::PostProcess 00489340 (Part vtbl+6C on skin parts) and the
// helpers it reaches that no other lane owns: MakeInlinePartList 004881F0, FindPartIndexDFS 004884D0,
// CExoArrayList<Part*>::operator[] 00437C40, MdlNodeTriMesh::BuildNeighborLists 004386B0.
// x87 intermediates as double with every original fstp dword kept as a float rounding (D30). EH unwind states
// (00717709 / 007176A8) are cited, not emitted.
// PATH NOTE: the body below 0048938C runs only when the skin node's vertex count word [+180] is 0 and [+1C8]
// is 0; binary MDL skins arrive with [+180] != 0 and take the short path at 0048A086 (RemoveTemporaryArrays).
#include "a7e200e217-model_instance.hpp"
// lane-texture-epoch nominal Material (task-17 material-ownership-r1 layout; ctor 0047B290 / dtor 0047B980).
#include "b22a9fca42-material.h"
#include <cstring>

using namespace aurora_model;
using kotor_scene3d::Vector;
using kotor_scene3d::Quaternion;
using kotor_scene3d::PointerList;

namespace gui_viewport_list {
// D58: CExoArrayList<int>::Add 00444FE0 single owner (M40 viewport-integration gui-viewport-list providers.h).
__declspec(noinline) void __fastcall add_00444fe0(kotor_reconstruction::ArrayMutations*, void*, int);
}

extern "C" {
// lane-3d-render / core sub-lane math (scene3d.h spellings)
Quaternion* __cdecl quaternion_multiply_004a9ac0(Quaternion* out, const Quaternion* a, const Quaternion* b);
void __fastcall quaternion_list_allocate_00437cc0(List* self, void* unused_edx, I capacity);  // CExoArrayList<Quaternion>::Allocate
void __fastcall vector_list_allocate_00421310(List* self, void* unused_edx, I capacity);      // lane-font CExoArrayList<Vector>::allocate
// lane-3d-anim (anim_skin.cpp)
void __cdecl gather_bones_00488930(void* part, PointerList* positions, PointerList* orientations, Vector position, Quaternion orientation);
// lane-3d-mesh (trimesh_f1.cpp)
U8 __fastcall aurora_material_has_bump_map_0047b8f0(void* material, void* unused_edx);
// this lane (model_material.cpp join to material_owner::Material::Material)
void* __fastcall aurora_material_construct_0047b290(void* self, void* unused_edx);
// authored below / still open in this lane
U* __fastcall part_list_at_00437c40(List* self, void* unused_edx, I index);
void __cdecl aurora_make_inline_part_list_004881f0(Part* part, List* list);
I __cdecl aurora_find_part_index_dfs_004884d0(Part* root, const char* name);
void __fastcall aurora_trimesh_build_neighbor_lists_004386b0(MdlNode* self, void* unused_edx);
void __fastcall aurora_trimesh_build_vertex_indices_0043c400(MdlNode* self, void* unused_edx);         // open (NvStripifier closure)
void __fastcall aurora_trimesh_calculate_tangent_space_0043a820(MdlNode* self, void* unused_edx);      // open
U __fastcall part_skin_post_process_00489340(void* self, void* unused_edx, U unused);
}

namespace {
const PartVTable* part_vt(Part* p) { return get<const PartVTable*>(p, 0); }
List* list_at(void* p, U off) { return reinterpret_cast<List*>(at(p, off)); }
Part* child_at(Part* p, I i) { return ptr<Part>(ptr<U>(get<U>(p, 0x30))[i]); }
I child_count(Part* p) { return get<I>(p, 0x34); }
float neg(float v) { return -v; }                                                  // fld; fchs; fstp dword (exact)
MaxTree* node_model(MdlNode* node) { return aurora_tree_as_model_0043e1c0(get<MaxTree*>(node, 8), nullptr); }
const char* part_name(Part* p) { return reinterpret_cast<const char*>(static_cast<std::uintptr_t>(part_vt(p)->name(p, nullptr))); }
}

// 00437C40 CExoArrayList<Part*>::operator[](index), RET4: &data[index].
U* __fastcall part_list_at_00437c40(List* self, void*, I index) {
    return ptr<U>(self->data) + index;                                              // 00437C40..00437C49
}

// 004881F0 MakeInlinePartList(part, list), cdecl: pre-order flattening of part's subtree into list. The root is
// appended inline (growth cap ? 2*cap : 8 via 005E03C0); the compiler unrolled ten further levels, each appended
// through CExoArrayList<int>::Add 00444FE0; the children of the tenth level recurse into 004881F0 (fetched via
// operator[] 00437C40).
namespace {
void inline_levels(Part* part, List* list, I depth) {
    for (I i = 0; i < child_count(part); ++i) {                                      // counts re-read each pass ([+34])
        if (depth <= 10) {
            Part* const child = child_at(part, i);                                  // 0048823B / 00488267 / ... / 004883A7
            gui_viewport_list::add_00444fe0(list, nullptr, static_cast<int>(addr(child))); // 00488245 .. 004883AD
            inline_levels(child, list, depth + 1);
        } else {
            aurora_make_inline_part_list_004881f0(ptr<Part>(*part_list_at_00437c40(list_at(part, 0x30), nullptr, i)), list); // 004883C0..004883CC
        }
    }
}
}
void __cdecl aurora_make_inline_part_list_004881f0(Part* part, List* list) {
    if (list->size == list->capacity)                                               // 004881F8..00488200
        menu_list_growth::allocate_005e03c0(list, nullptr, list->capacity ? static_cast<U>(list->capacity) * 2u : 8u); // 00488202..00488212
    ptr<U>(list->data)[list->size] = addr(part);                                    // 00488217..00488220
    list->size = list->size + 1;                                                    // 00488223
    inline_levels(part, list, 1);                                                   // 00488226..004884C2
}

// 004884D0 FindPartIndexDFS(root, name), cdecl: "root" (00740C00) -> 0; else the index of the first part in the
// pre-order list (004881F0) whose Name() (vtbl+04) matches case-insensitively; -1 when absent. The local list
// buffer is freed on every exit.
I __cdecl aurora_find_part_index_dfs_004884d0(Part* root, const char* name) {
    List list{};                                                                    // 004884EC..004884F4
    if (kotor_stricmp_0070acaf("root", name) == 0) {                                // 004884FC..00488510
        if (list.data != 0) live_free(ptr<void>(list.data));                        // 00488512..00488523
        return 0;                                                                   // 0048852C
    }
    aurora_make_inline_part_list_004881f0(root, &list);                             // 0048853E..00488549
    for (I i = 0; i < list.size; ++i) {                                             // 00488555..00488581
        Part* const p = ptr<Part>(ptr<U>(list.data)[i]);                            // 00488560..00488564
        if (kotor_stricmp_0070acaf(part_name(p), name) == 0) {                      // 00488567..00488578
            if (list.data != 0) live_free(ptr<void>(list.data));                    // 004885B1..004885C2
            return i;                                                               // 004885CE
        }
    }
    if (list.data != 0) live_free(ptr<void>(list.data));                            // 00488583..00488594
    return -1;                                                                      // 0048859E
}

// 004386B0 MdlNodeTriMesh::BuildNeighborLists: for each face f (count [+5C], 0x20-byte records at [+58]) of the
// PostProcess object P = [+134] (vertex positions P[0], per-face index triples P[48][f].indices):
// normal = NormalOf(v0, v1, v2) (004ACA00); dist = -(n.z*v0.z + n.y*v0.y + n.x*v0.x); for each edge (a, b) =
// (idx[e], idx[(e+1)%3]) the adjacency word face+14h+2e = the first other face g holding a at some j and b at
// (j+1)%3 or (j+2)%3, else -1.
void __fastcall aurora_trimesh_build_neighbor_lists_004386b0(MdlNode* self, void*) {
    U face_off = 0;                                                                 // [esp+28h]
    U rec_off = 0;                                                                  // [esp+20h]
    for (I f = 0; f < get<I>(self, 0x5C); ++f, rec_off += 0x14, face_off += 0x20) { // 004386B7..0043888B
        void* const pp = get<void*>(self, 0x134);
        const I* idx = ptr<const I>(get<U>(ptr<void>(get<U>(pp, 0x48) + rec_off), 0)); // 004386D4..004386DD
        const U verts = get<U>(pp, 0);                                              // 004386E3
        Vector n{};
        normal_of_004aca00(&n, ptr<const Vector>(verts + static_cast<U>(idx[0]) * 12),  // 004386E5..00438709
                           ptr<const Vector>(verts + static_cast<U>(idx[1]) * 12),
                           ptr<const Vector>(verts + static_cast<U>(idx[2]) * 12));
        unsigned char* const face = ptr<unsigned char>(get<U>(self, 0x58) + face_off);
        put<float>(face, 0, n.x); put<float>(face, 4, n.y); put<float>(face, 8, n.z); // 0043870E..0043871B
        {
            void* const pp2 = get<void*>(self, 0x134);                              // 0043871E (re-read)
            const I* idx2 = ptr<const I>(get<U>(ptr<void>(get<U>(pp2, 0x48) + rec_off), 0));
            const unsigned char* const v0 = ptr<unsigned char>(get<U>(pp2, 0) + static_cast<U>(idx2[0]) * 12);
            unsigned char* const fc = ptr<unsigned char>(get<U>(self, 0x58) + face_off);
            double d = static_cast<double>(get<float>(fc, 8)) * get<float>(v0, 8);  // 00438731..0043873A
            d = static_cast<double>(get<float>(fc, 4)) * get<float>(v0, 4) + d;     // 00438741..0043874A
            d = static_cast<double>(get<float>(fc, 0)) * get<float>(v0, 0) + d;     // 0043874C..00438750
            put<float>(fc, 0xC, static_cast<float>(-d));                            // 00438752..00438754
        }
        U adj_off = face_off + 0x14;                                                // 00438759 ([esp+18h])
        for (I e = 0; e < 3; ++e, adj_off += 2) {                                   // 00438760..00438865
            const I* const fi = ptr<const I>(get<U>(ptr<void>(get<U>(get<void*>(self, 0x134), 0x48) + rec_off), 0));
            const I a = fi[e];                                                      // 0043876C ([esp+2Ch])
            const I b = fi[(e + 1) % 3];                                            // 00438778..00438780 ([esp+24h])
            put<I16>(ptr<void>(get<U>(self, 0x58)), adj_off, -1);                   // 00438783..00438786
            U g_off = 0;                                                            // [esp+14h]
            for (I g = 0; g < get<I>(self, 0x5C); ++g, g_off += 0x14) {             // 00438793..0043884D
                if (f != g) {                                                       // 004387A5..004387A9
                    for (I j = 0; j < 3; ++j) {                                     // 004387AF..00438817 (edi = j + 2)
                        put<I16>(ptr<void>(get<U>(self, 0x58)), adj_off, -1);       // 004387C0..004387C3
                        const I* const gi = ptr<const I>(get<U>(ptr<void>(get<U>(get<void*>(self, 0x134), 0x48) + g_off), 0));
                        if (gi[j] != a) continue;                                   // 004387D9..004387E0
                        if (gi[(j + 1) % 3] == b || gi[(j + 2) % 3] == b) {         // 004387E2..00438807
                            put<I16>(ptr<void>(get<U>(self, 0x58)), adj_off, static_cast<I16>(g)); // 0043881F..0043882A
                            break;
                        }
                    }
                }
                if (get<I16>(ptr<void>(get<U>(self, 0x58)), adj_off) >= 0) break;   // 0043882E..00438836
            }
        }
    }
}

// 00489340 PartSkin::PostProcess(unused), RET4. node = [this+4].
//  [+180] != 0 (vertex count; every binary skin) -> 0048A086: node->[+54](node) (RemoveTemporaryArrays), return.
//  [+1C8] != 0 -> return.
//  Otherwise: T/Q = the inverse bind transform of this part accumulated up the parent chain; GatherBones from the
//  chain root; per bone append Rotate(conj(o), -p) to node list +1C4 and conj(o) to +1B8; resolve the four bone
//  names of each +19C weight record (0x34 bytes) to part indices (FindPartIndexDFS) and free the names;
//  node->[+50](node) (GenVertices); [+180] = P[64]; BuildVertexIndices; BuildNeighborLists; lay out the MDX row
//  (flags +150, stride +14C, offsets +154/158/160..170/1A8/1AC) from the first P[60] vertex record's [+54] bits
//  and a temporary Material's HasBumpMap; grow the model pool ([AC], size [B0]) by (n+1) rows and fill
//  positions/normals/uvs, the 1e6 sentinel row, the bone map (+1B0/+1B4, +1D0/1D4/1D8, words +1DC) and the
//  weight/index slots; free +19C; unlock; CalculateTangentSpaceBasis when bump-mapped; then RemoveTemporaryArrays.
// RETURN: EAX is 0 on the two early paths; after the closing free() it is whatever the CRT leaves, which no
// caller consumes (00440110 / NewCAurObject discard it) - returned as 0 (documented deviation).
U __fastcall part_skin_post_process_00489340(void* self_raw, void*, U) {
    Part* const self = static_cast<Part*>(self_raw);
    MdlNode* const node = get<MdlNode*>(self, 4);                                   // 00489360
    U has_bump = 0;                                                                 // 00489368 ([esp+34h])
    List inline_parts{};                                                            // 0048936C..00489374 ([esp+40h])
    if (get<I16>(node, 0x180) != 0) {                                               // 00489378..00489386
        get<NodeMeshFn>(node, 0x54)(node);                                          // 0048A086..0048A087
        if (inline_parts.data != 0) live_free(ptr<void>(inline_parts.data));        // 0048A08A..0048A0A1
        return 0;
    }
    List orientations{};                                                            // 0048938E..0048939C ([esp+80h])
    List positions{};                                                               // 004893A3..004893AB ([esp+74h])
    if (get<U>(node, 0x1C8) != 0) return 0;                                         // 004893AF..004893BD (EH state 2; nothing to free)

    // ---- inverse bind transform of this part, accumulated up the parent chain (004893C3..004894FF)
    Part* root = self;
    Vector T{};
    Quaternion Q{};
    {
        const Quaternion* const q = ptr<const Quaternion>(addr(at(self, 0x14)));
        Vector p{neg(get<float>(self, 8)), neg(get<float>(self, 0xC)), neg(get<float>(self, 0x10))}; // 004893C5..004893ED
        Quaternion cq{};
        quaternion_negate_w_004a9b50(&cq, q);                                       // 004893F1
        quaternion_rotate_004a9bb0(&T, &cq, &p);                                    // 004893FF
        quaternion_negate_w_004a9b50(&Q, q);                                        // 0048940A
    }
    for (Part* parent = get<Part*>(self, 0x3C); parent != nullptr; parent = get<Part*>(root, 0x3C)) { // 0048940F..004894FF
        root = parent;                                                              // 00489420
        const Quaternion* const qp = ptr<const Quaternion>(addr(at(parent, 0x14)));
        Vector pp{neg(get<float>(parent, 8)), neg(get<float>(parent, 0xC)), neg(get<float>(parent, 0x10))}; // 00489422..0048944A
        Quaternion cp{};
        quaternion_negate_w_004a9b50(&cp, qp);                                      // 0048944E
        Vector r1{};
        quaternion_rotate_004a9bb0(&r1, &cp, &pp);                                  // 0048945F
        Vector r2{};
        const Vector* const r = quaternion_rotate_004a9bb0(&r2, &Q, &r1);           // 00489472
        const float tz = static_cast<float>(static_cast<double>(T.z) + r->z);       // 00489477..0048947B
        const float ty = static_cast<float>(static_cast<double>(T.y) + r->y);       // 0048947F..00489483
        const float tx = static_cast<float>(static_cast<double>(T.x) + r->x);       // 00489486..0048948A
        T.x = tx; T.y = ty; T.z = tz;                                               // 0048948C..004894B4
        Quaternion cp2{};
        quaternion_negate_w_004a9b50(&cp2, qp);                                     // 004894B8
        Quaternion qq{};
        const Quaternion* const m = quaternion_multiply_004a9ac0(&qq, &Q, &cp2);     // 004894CB
        Q = *m;                                                                     // 004894D0..004894F0
    }
    gather_bones_00488930(root, &positions, &orientations, T, Q);                   // 00489505..0048954F

    // ---- per-bone inverse bind data (00489554..004896F4)
    {
        List* const inv_pos = list_at(node, 0x1C4);
        List* const inv_rot = list_at(node, 0x1B8);
        for (I i = 0; i < positions.size; ++i) {
            const Vector* const pi = ptr<const Vector>(positions.data) + i;
            const Quaternion* const oi = ptr<const Quaternion>(orientations.data) + i;
            Vector np{neg(pi->x), neg(pi->y), neg(pi->z)};                          // 00489580..004895BC
            Quaternion co{};
            quaternion_negate_w_004a9b50(&co, oi);                                  // 004895C0
            Vector v{};
            const Vector rv = *quaternion_rotate_004a9bb0(&v, &co, &np);            // 004895D1..004895F1
            if (inv_pos->size == inv_pos->capacity)                                 // 004895E2..004895F5
                vector_list_allocate_00421310(inv_pos, nullptr, inv_pos->capacity ? inv_pos->capacity * 2 : 8); // 004895F7..00489607
            ptr<Vector>(inv_pos->data)[inv_pos->size] = rv;                         // 0048960C..00489628
            inv_pos->size = inv_pos->size + 1;                                      // 0048962B..00489633
            Quaternion c2{};
            const Quaternion rc = *quaternion_negate_w_004a9b50(&c2, oi);           // 0048963F..0048965C
            if (inv_rot->size == inv_rot->capacity)                                 // 00489666..0048967F
                quaternion_list_allocate_00437cc0(inv_rot, nullptr, inv_rot->capacity ? inv_rot->capacity * 2 : 8); // 00489681..00489691
            ptr<Quaternion>(inv_rot->data)[inv_rot->size] = rc;                     // 00489696..004896C9
            inv_rot->size = inv_rot->size + 1;                                      // 004896CC..004896D7
        }
    }

    // ---- weight records: bone names -> part indices; names freed (004896FA..004897F8)
    for (I j = 0; j < get<I>(node, 0x1A0); ++j) {
        const U rec_off = static_cast<U>(j) * 0x34;
        if (get<I>(ptr<void>(get<U>(node, 0x19C) + rec_off), 0x20) == -1) {         // 00489712..00489722
            for (U k = 0; k < 4; ++k) {                                             // 00489724..00489776 (four inline calls)
                void* const rec = ptr<void>(get<U>(node, 0x19C) + rec_off);
                put<I>(rec, 0x20 + 4 * k, aurora_find_part_index_dfs_004884d0(root, ptr<const char>(get<U>(rec, 4 * k))));
            }
        }
        for (U k = 0; k < 4; ++k)                                                   // 00489779..004897B3
            live_free(ptr<void>(get<U>(ptr<void>(get<U>(node, 0x19C) + rec_off), 4 * k)));
        for (U k = 0; k < 4; ++k)                                                   // 004897B8..004897DD
            put<U>(ptr<void>(get<U>(node, 0x19C) + rec_off), 4 * k, 0);
    }

    get<NodeMeshFn>(node, 0x50)(node);                                              // 004897FE..004897FF
    put<I16>(node, 0x180, get<I16>(get<void*>(node, 0x134), 0x64));                 // 00489802..0048980C
    aurora_trimesh_build_vertex_indices_0043c400(node, nullptr);                    // 00489816..00489818
    aurora_trimesh_build_neighbor_lists_004386b0(node, nullptr);                    // 0048981D..0048981F

    // ---- MDX row layout (00489824..004899BC)
    void* first = nullptr;                                                          // 00489826
    if (get<I16>(node, 0x180) > 0)                                                  // 00489828..0048982F
        first = ptr<void>(ptr<U>(get<U>(get<void*>(node, 0x134), 0x60))[0]);        // 00489831..0048983A
    put<U>(node, 0x150, get<U>(node, 0x150) | 0x21);                                // 0048983C..00489845
    U row = 0x18;                                                                   // 0048984D (edi)
    put<U>(node, 0x154, 0);                                                         // 00489852
    put<U>(node, 0x158, 0xC);                                                       // 00489858
    put<U>(node, 0x14C, row);                                                       // 00489862
    const U bits = get<U8>(first, 0x54);                                            // 00489868 (read through the record; null when [+180] <= 0)
    if (bits & 1) {                                                                 // 00489868..00489871
        put<I16>(node, 0x182, static_cast<I16>(get<I16>(node, 0x182) + 1));         // 00489873
        put<U>(node, 0x150, get<U>(node, 0x150) | 2);                               // 0048987A / 00489883
        put<U>(node, 0x160, row);                                                   // 0048987D
        row = 0x20;                                                                 // 00489889
        put<U>(node, 0x14C, row);                                                   // 0048988B
    }
    if (get<U8>(first, 0x54) & 2) {                                                 // 00489891..00489895
        put<I16>(node, 0x182, static_cast<I16>(get<I16>(node, 0x182) + 1));         // 0048989D
        put<U>(node, 0x150, get<U>(node, 0x150) | 4);                               // 00489897..004898B0
        put<U>(node, 0x164, row);                                                   // 004898A7
        row += 8;                                                                   // 004898AD
        put<U>(node, 0x14C, row);                                                   // 004898B6
    }
    if (get<U8>(first, 0x54) & 4) {                                                 // 004898BC..004898C0
        put<I16>(node, 0x182, static_cast<I16>(get<I16>(node, 0x182) + 1));         // 004898C8
        put<U>(node, 0x150, get<U>(node, 0x150) | 8);                               // 004898C2..004898DB
        put<U>(node, 0x168, row);                                                   // 004898D2
        row += 8;                                                                   // 004898D8
        put<U>(node, 0x14C, row);                                                   // 004898E1
    }
    if (get<U8>(first, 0x54) & 8) {                                                 // 004898E7..004898EB
        put<I16>(node, 0x182, static_cast<I16>(get<I16>(node, 0x182) + 1));         // 004898F3
        put<U>(node, 0x150, get<U>(node, 0x150) | 0x10);                            // 004898ED..00489906
        put<U>(node, 0x16C, row);                                                   // 004898FD
        row += 8;                                                                   // 00489903
        put<U>(node, 0x14C, row);                                                   // 0048990C
    }
    (void)bits;
    {
        void* const block = live_operator_new(0x4C);                                // 00489912..00489914 (EH state 3)
        material_owner::Material* material = nullptr;                               // 00489824 (ebx = 0)
        if (block != nullptr)                                                       // 00489920..0048992A
            material = static_cast<material_owner::Material*>(aurora_material_construct_0047b290(block, nullptr)); // 0048992C..0048992E
        material->initialize_textures_0047b560(reinterpret_cast<const char*>(at(node, 0xA8)), nullptr, nullptr, nullptr); // 00489935..0048994C
        if (aurora_material_has_bump_map_0047b8f0(material, nullptr) != 0) {        // 00489951..0048995A
            put<U>(node, 0x150, get<U>(node, 0x150) | 0x80);                        // 0048995C..00489968
            put<U>(node, 0x170, row);                                               // 00489971
            has_bump = 1;                                                           // 00489977
            put<U>(node, 0x14C, get<U>(node, 0x14C) + 0x24);                        // 00489962..00489985
            row += 0x24;                                                            // 0048998B
        }
        if (material != nullptr) {                                                  // 0048998E..00489990
            material->~Material();                                                  // 00489992..00489994
            live_free(material);                                                    // 00489999..0048999A
        }
    }
    put<U>(node, 0x1A8, row);                                                       // 004899AA (weights, 4 floats)
    put<U>(node, 0x14C, get<U>(node, 0x14C) + 0x20);                                // 004899A2..004899B0
    put<U>(node, 0x1AC, row + 0x10);                                                // 004899B6..004899BC (indices, 4 floats)

    // ---- grow the model MDX pool by (n+1) rows and fill them (004899C2..00489C43)
    const U pad = get<U>(node_model(node), 0xB0) & 0xF;                             // 004899C2..004899D0
    {
        const U grow = static_cast<U>(static_cast<I>(get<I16>(node, 0x180)) + 1) * get<U>(node, 0x14C); // 004899D8..004899E9
        const U total = get<U>(node_model(node), 0xB0) + grow + pad;                // 004899DF..004899F2
        gl_render_resize_pool_00423f70(get<U>(node_model(node), 0xAC), total);      // 004899F4..00489A01
    }
    unsigned char* const pool = static_cast<unsigned char*>(aurora_pool_lock_00423f10(get<U>(node_model(node), 0xAC), 0, 0, 1)); // 00489A06..00489A1E
    const U base = get<U>(node_model(node), 0xB0) + pad;                            // 00489A2B..00489A39
    put<U>(node, 0x194, base);                                                      // 00489A3B
    {
        MaxTree* const model = node_model(node);                                    // 00489A41
        const U grow = static_cast<U>(static_cast<I>(get<I16>(node, 0x180)) + 1) * get<U>(node, 0x14C) + pad; // 00489A46..00489A60
        put<U>(model, 0xB0, get<U>(model, 0xB0) + grow);                            // 00489A4D..00489A64
    }
    put<void*>(node, 0x198, live_operator_new(static_cast<U>(static_cast<I>(get<I16>(node, 0x180)) + 1) * 12)); // 00489A66..00489A86
    I last = 0;                                                                     // 00489A8C ([esp+30h])
    for (I i = 0; i < static_cast<I>(get<I16>(node, 0x180)); ++i) {                 // 00489A90..00489BC3
        const unsigned char* const rec = ptr<unsigned char>(ptr<U>(get<U>(get<void*>(node, 0x134), 0x60))[i]); // 00489AA0..00489AB2
        const U stride = get<U>(node, 0x14C);
        const U uv_offs[4] = {0x160, 0x164, 0x168, 0x16C};
        const U uv_src[4] = {0x0C, 0x18, 0x24, 0x30};
        for (U s = 0; s < 4; ++s) {                                                 // 00489AA6..00489B3D (four inline blocks)
            const U off = get<U>(node, uv_offs[s]);
            if (off == 0xFFFFFFFFu) continue;
            unsigned char* const dst = pool + base + stride * static_cast<U>(i) + off;
            put<U>(dst, 0, get<U>(rec, uv_src[s]));
            put<U>(dst, 4, get<U>(rec, uv_src[s] + 4));
        }
        unsigned char* const vpos = pool + base + get<U>(node, 0x14C) * static_cast<U>(i) + get<U>(node, 0x154); // 00489B40..00489B53
        std::memcpy(vpos, rec, 12);                                                 // 00489B4F..00489B62
        std::memcpy(ptr<unsigned char>(get<U>(node, 0x198)) + static_cast<U>(i) * 12, rec, 12); // 00489B66..00489B83
        unsigned char* const vnrm = pool + base + get<U>(node, 0x14C) * static_cast<U>(i) + get<U>(node, 0x158); // 00489B86..00489BA0
        std::memcpy(vnrm, rec + 0x3C, 12);                                          // 00489B9B..00489BAD
    }
    {
        const U n = static_cast<U>(static_cast<I>(get<I16>(node, 0x180)));
        std::memset(pool + base + n * get<U>(node, 0x14C) + get<U>(node, 0x154), 0, get<U>(node, 0x14C)); // 00489BC9..00489BF3
        for (U c = 0; c < 3; ++c)                                                   // 00489BF5..00489C43 (1e6 sentinel position)
            put<U>(pool, base + static_cast<U>(static_cast<I>(get<I16>(node, 0x180))) * get<U>(node, 0x14C) + get<U>(node, 0x154) + 4 * c, 0x49742400u);
    }

    // ---- inline part list of the chain root; bone map arrays (00489C47..00489D44)
    {
        Part* top = self;                                                           // 00489C47
        while (get<Part*>(top, 0x3C) != nullptr) top = get<Part*>(top, 0x3C);       // 00489C4B..00489C59
        aurora_make_inline_part_list_004881f0(top, &inline_parts);                  // 00489C5B..00489C61
    }
    const I count = inline_parts.size;                                              // 00489C66..00489C71 ([esp+2Ch])
    if (count == 0) {                                                               // 00489C75
        if (get<U>(node, 0x1D0) != 0) {                                             // 00489C77..00489C7F
            live_free(ptr<void>(get<U>(node, 0x1D0)));                              // 00489C81..00489C82
            put<U>(node, 0x1D0, 0);                                                 // 00489C8A
        }
        put<I>(node, 0x1D8, 0);                                                     // 00489C90
    } else {
        const U old = get<U>(node, 0x1D0);                                          // 00489C98
        put<I>(node, 0x1D8, count);                                                 // 00489C9E
        put<void*>(node, 0x1D0, live_operator_new(static_cast<U>(inline_parts.size) * 4)); // 00489CA4..00489CBB
        for (I k = 0; k < get<I>(node, 0x1D4); ++k)                                 // 00489CC4..00489CE5
            ptr<U>(get<U>(node, 0x1D0))[k] = ptr<U>(old)[k];
        if (old != 0) live_free(ptr<void>(old));                                    // 00489CE7..00489CEC
    }
    put<I>(node, 0x1D4, count);                                                     // 00489CF4..00489CF8
    put<I>(node, 0x1B4, inline_parts.size);                                         // 00489CFE..00489D02
    put<void*>(node, 0x1B0, live_operator_new(static_cast<U>(inline_parts.size) * 4)); // 00489D08..00489D19
    for (I k = 0; k < inline_parts.size; ++k)                                       // 00489D26..00489D44
        put<U>(ptr<void>(get<U>(node, 0x1B0)), static_cast<U>(k) * 4, 0xBF800000u); // -1.0f

    // ---- per-vertex weights / bone slots (00489D46..00489F17)
    I next_bone = 0;                                                                // 00489D4D ([esp+38h])
    {
        I bone = 0;                                                                 // [esp+10h] (kept across k when a weight is 0)
        for (I i = 0; i < static_cast<I>(get<I16>(node, 0x180)); ++i) {             // 00489D51..00489F0F
            const unsigned char* const w = ptr<unsigned char>(get<U>(node, 0x19C) + static_cast<U>(i) * 0x34); // 00489D63..00489D6F
            last = i;                                                               // 00489D73
            for (U k = 0; k < 4; ++k) {                                             // 00489D77..00489EF6
                const U slot = base + get<U>(node, 0x14C) * static_cast<U>(i) + k * 4;
                put<U>(pool, slot + get<U>(node, 0x1AC), 0xBF800000u);              // 00489D80..00489DA3 (-1.0f)
                put<U>(pool, slot + get<U>(node, 0x1A8), 0xBF800000u);              // 00489DAA..00489DCB
                const float weight = get<float>(w, 0x10 + 4 * k);                   // 00489DD2..00489DFA
                put<float>(pool, base + get<U>(node, 0x14C) * static_cast<U>(i) + k * 4 + get<U>(node, 0x1A8), weight); // 00489DFD..00489E1A (fst)
                if (weight == 0.0f) continue;                                       // 00489E03 / 00489E1D..00489E22 (0073D700; jnp only on ==)
                bone = get<I>(w, 0x20 + 4 * k);                                     // 00489E28..00489E5F
                float* const map = ptr<float>(get<U>(node, 0x1B0)) + bone;          // 00489E63..00489E6C
                if (*map < 0.0f) {                                                  // 00489E69..00489E7A (fcomp 0.0; jp skips >=, unordered)
                    *map = static_cast<float>(next_bone);                           // 00489E7C..00489E89 (fild/fstp)
                    next_bone = next_bone + 1;
                }
                put<U>(pool, base + get<U>(node, 0x14C) * static_cast<U>(i) + k * 4 + get<U>(node, 0x1AC), get<U>(ptr<void>(get<U>(node, 0x1B0)), static_cast<U>(bone) * 4)); // 00489E8B..00489EAF
                const I slot_index = static_cast<I>(get<float>(pool, base + get<U>(node, 0x14C) * static_cast<U>(i) + k * 4 + get<U>(node, 0x1AC))); // 00489EB2..00489ED4 (_ftol, truncation)
                put<I16>(node, 0x1DC + static_cast<U>(slot_index) * 2, static_cast<I16>(bone)); // 00489EDD
            }
        }
        (void)bone;
    }
    {
        const U m = static_cast<U>(last + 1);                                       // 00489F19..00489F2F
        const U row_off = base + get<U>(node, 0x14C) * m;
        for (U k = 0; k < 4; ++k) put<U>(pool, row_off + get<U>(node, 0x1AC) + 4 * k, 0); // 00489F30..00489F8D
        put<U>(pool, row_off + get<U>(node, 0x1A8), 0x3F800000u);                  // 00489F91..00489FAA (1.0f)
        for (U k = 1; k < 4; ++k) put<U>(pool, row_off + get<U>(node, 0x1A8) + 4 * k, 0); // 00489FB1..0048A004
    }
    if (get<U>(node, 0x19C) != 0) {                                                 // 0048A008..0048A010
        live_free(ptr<void>(get<U>(node, 0x19C)));                                  // 0048A012..0048A013
        put<U>(node, 0x19C, 0);                                                     // 0048A01B
    }
    put<U>(node, 0x1A4, 0);                                                         // 0048A021
    put<U>(node, 0x1A0, 0);                                                         // 0048A027
    aurora_pool_unlock_00423f30(get<U>(node_model(node), 0xAC));                    // 0048A02D..0048A03C
    if (positions.data != 0) live_free(ptr<void>(positions.data));                  // 0048A041..0048A055 (EH state 1)
    if (orientations.data != 0) live_free(ptr<void>(orientations.data));            // 0048A05D..0048A071 (EH state 0)
    if (has_bump != 0) aurora_trimesh_calculate_tangent_space_0043a820(node, nullptr); // 0048A079..0048A081
    get<NodeMeshFn>(node, 0x54)(node);                                              // 0048A086..0048A087
    if (inline_parts.data != 0) live_free(ptr<void>(inline_parts.data));            // 0048A08A..0048A0A1
    return 0;
}
