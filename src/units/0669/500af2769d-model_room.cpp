// model_room.cpp - lane-3d-model (D42, review M-2): instance-tree helpers used by Scene::SpawnRoom 00456F30 and the
// room constructor: CreateInstanceTree 00449410, CollapsePartTree 00453F10, ExpandReferenceParts 00454470,
// Gob::SetProcessFlag 0043F270. Symbols as referenced by lane-3d-room
// (room_spawn.cpp). x87 as double with float rounding at each fstp (D30). Unwind not emitted.
#include "a7e200e217-model_instance.hpp"
#include <cstdio>

using namespace aurora_model;
using kotor_scene3d::Vector;
using kotor_scene3d::Quaternion;
using kotor_scene3d::PointerList;

namespace gui_viewport_list {
// D58: CExoArrayList<int>::Add 00444FE0 single owner (M40 viewport-integration gui-viewport-list providers.h).
__declspec(noinline) void __fastcall add_00444fe0(kotor_reconstruction::ArrayMutations*, void*, int);
}

extern "C" {
Quaternion* __cdecl quaternion_multiply_004a9ac0(Quaternion* out, const Quaternion* a, const Quaternion* b); // core sub-lane
I __cdecl crt_rand_006fc447();                                                                             // model_lifetime.cpp
Part* __cdecl aurora_create_instance_tree_00449410(MdlNode* node, Gob* gob);
void __fastcall gob_set_process_flag_0043f270(Gob* self, void*);
void __cdecl aurora_collapse_part_tree_00453f10(Gob* gob, Part* tree, PointerList* parts, Vector position,
                                                Quaternion orientation, void* scene, void* room);
void __cdecl aurora_expand_reference_parts_00454470(Part* part);
}

namespace {
const PartVTable* part_vt(Part* p) { return get<const PartVTable*>(p, 0); }
const GobVTable* gob_vt(Gob* g) { return get<const GobVTable*>(g, 0); }
List* list_at(void* p, U off) { return reinterpret_cast<List*>(at(p, off)); }
void append8(List* l, U value) {                                                    // inline Add, growth cap ? 2*cap : 8 (005E03C0)
    if (l->size == l->capacity)
        menu_list_growth::allocate_005e03c0(l, nullptr, l->capacity ? static_cast<U>(l->capacity) * 2u : 8u);
    ptr<U>(l->data)[l->size] = value;
    l->size = l->size + 1;
}
// p = origin + rotate(q, p) (each component rounded at its fstp), then o = q * o.
void place(Vector* p, Quaternion* o, const Vector& origin, const Quaternion* q) {
    Vector rotated;
    const Vector* const r = quaternion_rotate_004a9bb0(&rotated, q, p);
    const float x = static_cast<float>(static_cast<double>(origin.x) + r->x);
    const float y = static_cast<float>(static_cast<double>(origin.y) + r->y);
    const float z = static_cast<float>(static_cast<double>(origin.z) + r->z);
    p->x = x; p->y = y; p->z = z;
    Quaternion product;
    *o = *quaternion_multiply_004a9ac0(&product, q, o);
}
}

// 0043F270 Gob::SetProcessFlag: byte [+167] = 1.
void __fastcall gob_set_process_flag_0043f270(Gob* self, void*) {
    put<U8>(self, 0x167, 1);                                                        // 0043F270
}
// 00449410 CreateInstanceTree(node, gob), cdecl: part = CreateInstanceTreeR(node, gob); part->vtbl[6C](part) with
// the part pointer as argument (00449427..0044942A); returns part.
Part* __cdecl aurora_create_instance_tree_00449410(MdlNode* node, Gob* gob) {
    Part* const part = aurora_create_instance_tree_00449200(node, gob);            // 00449410..0044941B
    part_vt(part)->animate(part, nullptr, addr(part));                              // 00449420..0044942A
    return part;                                                                    // 0044942D
}

// 00453F10 CollapsePartTree(gob, tree, parts, position, orientation, scene, room), cdecl (30h argument bytes).
// Flattens tree into parts (world placement applied down the hierarchy), instancing reference parts into the
// room's object list +68 when room and scene are given; with a gob, the child named "<model name>a" is kept as
// the tree's only child and the tree's original placement is restored.
void __cdecl aurora_collapse_part_tree_00453f10(Gob* gob, Part* tree, PointerList* parts, Vector position,
                                                Quaternion orientation, void* scene, void* room) {
    char special_name[0x30];                                                        // [esp+90h]
    Vector saved_position = {0.0f, 0.0f, 0.0f};                                     // [esp+2Ch..34h]
    Quaternion saved_orientation = {1.0f, 0.0f, 0.0f, 0.0f};                        // [esp+38h..44h]
    Part* special = nullptr;                                                        // [esp+48h]
    if (gob != nullptr) {                                                           // 00453F22..00453F61
        std::strcpy(special_name, reinterpret_cast<const char*>(at(get<MaxTree*>(gob, 0x58), 8))); // 00453F63..00453F7A
        std::strcat(special_name, "a");                                             // 00453F7C..00453F9E (word 0073D5D4)
        std::memcpy(&saved_position, at(tree, 8), 12);                              // 00453F99..00453FB4
        std::memcpy(&saved_orientation, at(tree, 0x14), 16);                        // 00453FAB..00453FCD
    }
    Vector* const tree_position = reinterpret_cast<Vector*>(at(tree, 8));           // [esp+4Ch]
    Quaternion* const tree_orientation = reinterpret_cast<Quaternion*>(at(tree, 0x14)); // [esp+50h]
    place(tree_position, tree_orientation, position, &orientation);                 // 00453FDA..00454061
    List* const list = reinterpret_cast<List*>(parts);
    append8(list, addr(tree));                                                      // 00454068..0045409A
    for (I i = 0; i < list->size; ++i) {                                            // 0045409D..0045438F (size re-read)
        Part* const part = ptr<Part>(ptr<U>(list->data)[i]);                        // 004540A7..004540AD
        Part* const ref = ptr<Part>(part_vt(part)->query38(part, nullptr));        // 004540B0..004540B5
        if (ref != nullptr && room != nullptr && scene != nullptr) {                // 004540B8..004540D4
            Gob* const obj = aurora_new_aur_object_00449cc0(reinterpret_cast<const char*>(at(get<MdlNode*>(ref, 4), 0x50)), "", nullptr, nullptr); // 004540DA..004540E8
            if (obj != nullptr) {                                                   // 004540F2..004540F4
                Vector where;
                std::memcpy(&where, at(part, 8), 12);                               // 004540FF..00454111
                Vector out_position;
                gob_vt(obj)->s05c_gob_set_position(obj, nullptr, &out_position, where); // 00454114..0045411B
                U o[4];
                std::memcpy(o, at(part, 0x14), 16);                                 // 00454123..0045413B
                Quaternion out_orientation;
                gob_vt(obj)->s060_gob_set_orientation(obj, nullptr, addr(&out_orientation), o[0], o[1], o[2], o[3]); // 0045413E..00454148
                const I d1 = crt_rand_006fc447() % 100;                             // 0045414B..00454156
                const float speed = static_cast<float>((static_cast<double>(d1) - 50.0) * static_cast<double>(0.001f) + 1.0); // 0045415C..00454172
                const I d2 = crt_rand_006fc447() % 100;                             // 00454176..00454181
                const float offset = static_cast<float>(static_cast<double>(d2) * static_cast<double>(0.01f)); // 0045418B..00454197
                U speed_bits, offset_bits;
                std::memcpy(&speed_bits, &speed, 4); std::memcpy(&offset_bits, &offset, 4);
                gob_vt(obj)->s018_gob_play_animation(obj, nullptr, addr("default"), speed_bits, 0, offset_bits); // 0045419B..004541AA (73DF6C)
                gob_set_scene_0043f200(obj, nullptr, scene);                        // 004541AD..004541B7
                append8(list_at(room, 0x68), addr(obj));                            // 004541BC..004541ED
            }
        }
        for (I j = 0; j < get<I>(part, 0x34); ++j) {                                // 004541F9..0045435A
            Part* const child = ptr<Part>(ptr<U>(get<U>(part, 0x30))[j]);           // 00454206..0045420D
            if (gob != nullptr) {                                                   // 00454210..0045421D
                MdlNode* const node = get<MdlNode*>(child, 4);                      // 0045421F
                const U index = get<U>(node, 4);                                    // 0045422C (inline TranslateNodeName)
                MaxTree* owner = aurora_tree_as_model_0043e1c0(get<MaxTree*>(node, 8), nullptr); // 00454222..00454238
                if (owner == nullptr)
                    owner = get<MaxTree*>(aurora_tree_as_animation_0043e1d0(get<MaxTree*>(node, 8), nullptr), 0x84); // 0045423F..00454244
                const char* const name = ptr<const char*>(get<U>(owner, 0xB8))[index]; // 0045424A..00454259
                if (kotor_stricmp_0070acaf(name, special_name) == 0) {              // 0045425C..0045426F
                    special = child;                                                // 00454271
                    continue;                                                       // 00454275
                }
            }
            place(reinterpret_cast<Vector*>(at(child, 8)), reinterpret_cast<Quaternion*>(at(child, 0x14)),
                  *reinterpret_cast<const Vector*>(at(part, 8)), reinterpret_cast<const Quaternion*>(at(part, 0x14))); // 0045427A..004542E6
            append8(list, addr(child));                                             // 004542E9..00454349
        }
        put<I>(part, 0x34, 0);                                                      // 0045436E
        put<U>(part, 0x3C, 0);                                                      // 00454371
        Gob* const owner_gob = get<Gob*>(part, 0x40);                               // 00454369
        if (owner_gob != nullptr && get<Part*>(owner_gob, 0x6C) == part) put<U>(owner_gob, 0x6C, 0); // 00454374..0045437B
        put<U>(part, 0x40, 0);                                                      // 00454382
    }
    if (gob == nullptr) return;                                                     // 00454395..0045439E
    list->RemoveReverse32(addr(tree));                                              // 004543A4..004543D8 (inlined 00445080)
    append8(list_at(tree, 0x30), addr(special));                                    // 004543DA..00454414
    put<Gob*>(tree, 0x40, gob);                                                     // 00454422
    put<Part*>(gob, 0x6C, tree);                                                    // 00454425
    std::memcpy(tree_position, &saved_position, 12);                               // 0045441E..0045444D
    std::memcpy(tree_orientation, &saved_orientation, 16);                         // 00454438..00454450
    gob_vt(gob)->slot184(gob, nullptr);                                             // 00454453..00454457
    gob_set_process_flag_0043f270(gob, nullptr);                                    // 0045445D..0045445F
}

// 00454470 ExpandReferenceParts(part), cdecl: for a reference part, instance the referenced model's tree
// (FindModel(node+50) -> CreateInstanceTree(root, 0)), append it to the reference's children, and run
// PartInitMaterial(0, part, sprintf("%s", node name)); then recurse into the children (count +34 re-read; the
// original unrolls seven levels, level 0 inlines the Add, deeper levels call 00444FE0).
void __cdecl aurora_expand_reference_parts_00454470(Part* part) {
    Part* const ref = ptr<Part>(part_vt(part)->query38(part, nullptr));            // 0045447F..00454486
    if (ref != nullptr) {                                                           // 0045448B..0045448D
        MdlNode* const node = get<MdlNode*>(ref, 4);                                // 0045448F
        MaxTree* const model = aurora_find_model_00464110(reinterpret_cast<const char*>(at(node, 0x50))); // 00454492..00454496
        Part* const child = aurora_create_instance_tree_00449410(get<MdlNode*>(model, 0x28), nullptr); // 0045449B..004544A1
        gui_viewport_list::add_00444fe0(list_at(ref, 0x30), nullptr, static_cast<int>(addr(child)));           // 004544A6..004544DB
        char prefix[0x20];                                                          // [esp+3Ch] (0x20 apart per level)
        std::sprintf(prefix, "%s", aurora_node_translate_name_00438a00(node, nullptr)); // 004544D9..004544EE (73D5D0)
        aurora_part_init_material_00442d70(nullptr, part, prefix);                  // 004544F3..004544FB
    }
    for (I i = 0; i < get<I>(part, 0x34); ++i)                                      // 00454503..0045488C
        aurora_expand_reference_parts_00454470(ptr<Part>(ptr<U>(get<U>(part, 0x30))[i])); // 00454514 / 004547F7
}
