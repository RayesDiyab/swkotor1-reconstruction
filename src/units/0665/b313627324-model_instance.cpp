// model_instance.cpp - lane-3d-model: instance creation for a loaded Model (normal paths).
// Original fs:[0] EH frames around allocation+construction (handlers 0071619B, 0071617F, 0071636B,
// 00715DF8, 00715E23, 00715D78, 00717A6B, 00717B68, 00717C5C, 0071765B, 007167A6) free the raw block on a
// constructor throw (e.g. funclet 00716360: free([ebp-10h])). No callee on these paths throws in the
// original (operator new returns 0); the unwind states are not modeled here (open gate, as for the
// task-34 node constructor and task-17 Material packets). Evidence cited per behavior.
#include "a7e200e217-model_instance.hpp"
#include <cstdio>

using namespace aurora_model;

extern "C" {
List aurora_gobs_007a8a58 = {};
volatile I aurora_part_count_007a8a10 = 0;
volatile I aurora_trimesh_part_count_007a8a14 = 0;
volatile I aurora_gob_count_007a8a18 = 0;
volatile I aurora_gob_created_007a8a1c = 0;
volatile I aurora_mesh_serial_0078e328 = 1;           // .data initial value 1 (image)
volatile I aurora_emitter_count_00830470 = 0;
volatile U aurora_control_weight_00798b38 = 0x3F800000u; // .data initial 1.0f (image)
volatile U8 aurora_model_gate_007b8fc0 = 0;
}
// Scene list 007B9398 {data,size,capacity}: task-7 historical_storage_007b9398 (12-byte extent).
#include "2f42e97d01-storage.h"

namespace {
const GobVTable* gob_vt(Gob* g) { return get<const GobVTable*>(g, 0); }
const PartVTable* part_vt(Part* p) { return get<const PartVTable*>(p, 0); }
List* list_at(void* p, U off) { return reinterpret_cast<List*>(at(p, off)); }
void zero(void* p, U off) { put<U>(p, off, 0); }
void zero_range(void* p, U first, U last) { for (U o = first; o <= last; o += 4) put<U>(p, o, 0); }
constexpr U kOne = 0x3F800000u;   // 1.0f raw bits (D3: raw DWORD stores)
}

// ---------------------------------------------------------------- CSWGuiScene / lists
// 00671C00 CExoArrayList::Add(value), RET4: grow when size == capacity (cap ? 2*cap : 16) via 005E03C0; append.
// [M45 compose] 00671C00 body removed: existing M44 provider gui_load_array_add_00671c00 wins (definition removed; declaration kept)
void __fastcall aurora_list_add16_00671c00(List* self, void*, U value);
// 004167A0 CExoArrayList::insert(value, index), RET8: old = size; Add(value); shift (old .. index] up; [index] = value.
void __fastcall aurora_list_insert_at_004167a0(List* self, void*, U value, I index) {
    I i = self->size;                                                               // 004167A9
    aurora_list_add16_00671c00(self, nullptr, value);                               // 004167AC..004167AD
    while (i > index) {                                                             // 004167B6 / 004167CE
        U* const data = ptr<U>(self->data);
        data[i] = data[i - 1];                                                      // 004167C0..004167CC
        --i;
    }
    ptr<U>(self->data)[index] = value;                                              // 004167D0..004167D4 / 004167DB..004167DF
}
// 004175C0 CSWGuiScene::AddModel(gob, index, attach), RET0Ch: null gob -> 0. 0 <= index < [+20] ->
// insert at index into +1C, else Add; attach != 0 -> gob->vtbl[4C](scene [+14]). Returns 1.
I __fastcall aurora_gui_scene_add_model_004175c0(GuiScene* self, void*, Gob* gob, I index, I attach) {
    if (gob == nullptr) return 0;                                                   // 004175C6..004175D0
    if (index >= 0 && index < get<I>(self, 0x20))                                   // 004175D7..004175DE
        aurora_list_insert_at_004167a0(list_at(self, 0x1C), nullptr, addr(gob), index); // 004175E0..004175E5
    else
        aurora_list_add16_00671c00(list_at(self, 0x1C), nullptr, addr(gob));        // 004175EC..004175F0
    if (attach != 0)                                                                // 004175F5..004175FB
        gob_vt(gob)->attach_to_scene(gob, nullptr, get<Scene*>(self, 0x14));        // 004175FD..00417605
    return 1;
}
// 00417620 CSWGuiScene::AddModel(CExoString* name, index), RET8: gob = NewCAurObject(name.CStr(), "", 0, 0);
// AddModel(gob, index, 1) != 0 ? gob : 0 (neg/sbb/and).
Gob* __fastcall aurora_gui_scene_add_model_name_00417620(GuiScene* self, void*, void* name_string, I index) {
    const char* const name = async_string_cstr_005e5670(name_string, nullptr);     // 00417628..00417631
    Gob* const gob = aurora_new_aur_object_00449cc0(name, "", nullptr, nullptr);   // 00417622..00417637 (73D71C)
    const I added = aurora_gui_scene_add_model_004175c0(self, nullptr, gob, index, 1); // 00417641..0041764B
    return added != 0 ? gob : nullptr;                                              // 00417650..00417655
}

// ---------------------------------------------------------------- lookup helpers
// 0044FD60 FindScene(name): linear over 007B9398 (count 007B939C re-read): _stricmp(name, scene+10) == 0 -> scene.
void* __cdecl find_scene_0044fd60(const char* name) {
    for (I i = 0; i < get<I>(&historical_storage_007b9398, 4); ++i) {              // 0044FD60..0044FD92
        unsigned char* const scene = ptr<unsigned char>(ptr<U>(addr(historical_storage_007b9398.head))[i]); // 0044FD71..0044FD76
        if (kotor_stricmp_0070acaf(name, reinterpret_cast<const char*>(scene + 0x10)) == 0) // 0044FD79..0044FD88
            return ptr<void>(ptr<U>(addr(historical_storage_007b9398.head))[i]);     // 0044FD99..0044FD9F (re-read)
    }
    return nullptr;
}
// 00440410 FindGobIdx(name): bisection over 007A8A58 on _stricmp(gob+38, name): > 0 -> hi = mid-1;
// < 0 -> lo = mid+1; == 0 -> mid. Not found -> lo (insertion index). mid = (lo+hi) sar 1.
I __cdecl aurora_find_gob_index_00440410(const char* name) {
    I lo = 0;
    I hi = aurora_gobs_007a8a58.size - 1;                                          // 00440418..00440420
    while (hi >= lo) {                                                              // 00440421 / 0044044D
        const I mid = (hi + lo) >> 1;                                               // 0044042A..0044042D
        const char* const gob_name = reinterpret_cast<const char*>(at(ptr<void>(ptr<U>(aurora_gobs_007a8a58.data)[mid]), 0x38));
        const I r = kotor_stricmp_0070acaf(gob_name, name);                         // 0044042F..00440437
        if (r > 0) hi = mid - 1;                                                    // 00440441..00440443
        else if (r < 0) lo = mid + 1;                                               // 00440448..0044044A
        else return mid;                                                            // 00440458
    }
    return lo;                                                                      // 00440454
}

// ---------------------------------------------------------------- Gob
// 0044EE00 CAurPathInfo::CAurPathInfo: [0] = 00740A94, [8] = [4] = 0; returns this.
void* __fastcall aurora_path_info_construct_0044ee00(void* self, void*) {
    put<const void*>(self, 0, &aurora_vtable_path_info_00740a94);
    zero(self, 8);
    zero(self, 4);
    return self;
}
// 0043FE00 FindPart(part, name), cdecl (the original unrolls five levels and recurses below them):
// null part or null name -> 0; _stricmp(TranslateNodeName(part[4]), name) == 0 -> part; else search the
// non-null children (+30, count +34 re-read) depth-first and return the first hit; 0 when none.
Part* __cdecl find_part_0043fe00(Part* part, const char* name) {
    if (part == nullptr) return nullptr;                                            // 0043FE04..0043FE12
    if (name == nullptr) return nullptr;                                            // 0043FE15..0043FE1C -> 004400F9
    if (kotor_stricmp_0070acaf(aurora_node_translate_name_00438a00(get<MdlNode*>(part, 4), nullptr), name) == 0) // 0043FE22..0043FE36
        return part;                                                                // 0043FE38..0043FE41
    for (I i = 0; i < get<I>(part, 0x34); ++i) {                                    // 0043FE42..004400F3
        Part* const child = ptr<Part>(ptr<U>(get<U>(part, 0x30))[i]);               // 0043FE53..0043FE56
        if (child == nullptr) continue;                                             // 0043FE59..0043FE5F
        Part* const hit = find_part_0043fe00(child, name);                          // inlined levels / 0044001D
        if (hit != nullptr) return hit;                                             // 004400E1..004400E3
    }
    return nullptr;                                                                 // 004400F9
}

// 0044F7C0 Gob::AttachToScene(scene), RET4: SetScene(scene) (0043F200, lane-3d-render); return 1.
I __fastcall aurora_gob_attach_to_scene_0044f7c0(Gob* self, void*, Scene* scene) {
    gob_set_scene_0043f200(self, nullptr, scene);
    return 1;
}

// 00449440 Gob::Gob(name), RET4 (0x1CC-byte object). Field stores in original order.
Gob* __fastcall gob_construct_00449440(Gob* self, void*, const char* name) {
    zero(self, 0x8); zero(self, 0xC); zero(self, 0x4);                              // 00449464..0044946A
    put<const GobVTable*>(self, 0, &aurora_gob_vtable_00741268);                    // 0044946D
    zero(self, 0x24); zero(self, 0x28); zero(self, 0x20);                           // 00449477..0044947D
    zero(self, 0x78); zero(self, 0x7C); zero(self, 0x80);                           // 00449480..00449486
    zero(self, 0x90); zero(self, 0x8C); zero(self, 0x88);                           // 0044948C..00449498
    put<U>(self, 0x84, kOne);                                                       // 004494A3
    zero(self, 0x94); zero(self, 0x98); zero(self, 0x9C);                           // 004494A9..004494B5
    zero(self, 0xAC); zero(self, 0xA8); zero(self, 0xA4);                           // 004494BB..004494C7
    put<U>(self, 0xA0, kOne);                                                       // 004494CD
    zero(self, 0xB4); zero(self, 0xB8); zero(self, 0xB0);                           // 004494D3..004494DF
    zero(self, 0xCC); zero(self, 0xD0); zero(self, 0xD4);                           // 004494E5..004494F1
    zero(self, 0xE0); zero(self, 0xE4); zero(self, 0xE8);                           // 004494F7..00449503
    zero(self, 0xF0); zero(self, 0xF4); zero(self, 0xEC);                           // 00449509..00449515
    zero(self, 0xFC); zero(self, 0x100); zero(self, 0xF8);                          // 0044951B..00449527
    aurora_path_info_construct_0044ee00(at(self, 0x110), nullptr);                  // 0044952D..00449538
    zero_range(self, 0x120, 0x134);                                                 // 0044953D..0044955B
    zero_range(self, 0x13C, 0x150);                                                 // 00449561..0044957F
    zero(self, 0x194); zero(self, 0x198); zero(self, 0x190);                        // 00449585..00449591
    zero(self, 0x1A0); zero(self, 0x1A4); zero(self, 0x19C);                        // 00449597..004495A3
    zero(self, 0x1A8);                                                              // 004495A9..004495AF
    zero(self, 0x1C0); zero(self, 0x1C4); zero(self, 0x1BC);                        // 004495B1..004495BD
    put<U8>(self, 0x17, 1);                                                         // 004495C3
    put<U8>(self, 0x184, 0);                                                        // 004495C7
    aurora_gob_created_007a8a1c = aurora_gob_created_007a8a1c + 1;                  // 004495CD
    put<U8>(self, 0x154, 0);                                                        // 004495D7
    put<U8>(self, 0x18, 1);                                                         // 004495DD
    zero(self, 0x1C);                                                               // 004495E1
    aurora_gob_count_007a8a18 = aurora_gob_count_007a8a18 + 1;                      // 004495E4..004495ED
    put<U8>(self, 0x38, 0);                                                         // 004495F8
    if (name != nullptr) {                                                          // 004495EB / 004495FB
        char buffer[0x40];                                                          // [esp+28h] local
        std::sprintf(buffer, "%s", name);                                           // 00449601..0044960C (73D5D0)
        I suffix = 0;                                                               // 00449614
        for (;;) {
            if (find_scene_0044fd60(buffer) == nullptr) {                    // 00449620..0044962F
                const I idx = aurora_find_gob_index_00440410(buffer);               // 00449631..00449636
                if (idx >= aurora_gobs_007a8a58.size) break;                        // 0044963B..0044964A
                Gob* const other = ptr<Gob>(ptr<U>(aurora_gobs_007a8a58.data)[idx]);
                if (kotor_stricmp_0070acaf(reinterpret_cast<const char*>(at(other, 0x38)), buffer) != 0) break; // 0044964C..00449668
                if (ptr<U>(aurora_gobs_007a8a58.data)[idx] == 0) break;            // 0044966A..00449677
            }
            ++suffix;                                                               // 00449679..0044967F
            std::sprintf(buffer, "%s%d", name, suffix);                             // 00449683..00449692 (73EDD0)
        }
        std::strcpy(reinterpret_cast<char*>(at(self, 0x38)), buffer);              // 0044969C..004496B8
    }
    zero(self, 0x2C); zero(self, 0x6C); zero(self, 0x70); zero(self, 0x74); zero(self, 0x68); // 004496BA..004496C6
    zero(self, 0x188);                                                              // 004496C9
    U* const light_ref = ptr<U>(addr(at(self, 0x1A8)));                            // 004495A9 (edi)
    if (*light_ref != 0)                                                            // 004496CF..004496D3
        list_at(ptr<void>(*light_ref), 4)->RemoveReverse32(addr(light_ref));       // 004496D5..004496D9 (00445080)
    *light_ref = 0;                                                                 // 004496DE
    put<U>(self, 0xCC, kOne); put<U>(self, 0xD0, kOne); put<U>(self, 0xD4, kOne);   // 004496E0..00449710
    put<U>(self, 0x120, 0); put<U>(self, 0x124, 0); put<U>(self, 0x128, 0);        // 00449716..00449752
    put<U>(self, 0x12C, kOne); put<U>(self, 0x130, kOne); put<U>(self, 0x134, kOne); // 00449746..00449794
    put<U>(self, 0x13C, 0); put<U>(self, 0x140, 0); put<U>(self, 0x144, 0);        // 0044977C..004497CA
    put<U>(self, 0x148, 0); put<U>(self, 0x14C, 0);                                 // 004497B2..004497E7
    zero(self, 0xBC);                                                               // 004497ED
    put<U8>(self, 0xC0, 1);                                                         // 004497F3
    put<U>(self, 0x18C, 0xFFFFFFFFu);                                               // 004497FA
    zero(self, 0x58); zero(self, 0x5C); zero(self, 0x60); zero(self, 0x64);         // 00449804..0044980D
    put<U16>(self, 0xC4, 0); put<U16>(self, 0xC6, 0);                               // 00449810..00449817
    put<U>(self, 0xD8, kOne); put<U>(self, 0xDC, kOne);                             // 0044981E..00449824
    put<U16>(self, 0xC2, 1); put<U16>(self, 0xC8, 0);                               // 0044982A..00449831
    zero(self, 0x104);                                                              // 00449838
    put<U8>(self, 0x155, 0);                                                        // 0044983E
    put<U>(self, 0x180, 0x3FC00000u);                                               // 00449844 (1.5f)
    zero(self, 0x11C); zero(self, 0x108); zero(self, 0x10C);                        // 0044984E..0044985A
    put<U8>(self, 0x164, 0); put<U8>(self, 0x165, 0); put<U8>(self, 0x166, 0); put<U8>(self, 0x17C, 0); // 00449860..00449872
    zero(self, 0x168); zero(self, 0x16C);                                           // 00449878..0044987E
    put<U8>(self, 0x15, 0); put<U8>(self, 0x17F, 0); put<U8>(self, 0x17D, 0); put<U8>(self, 0x156, 0); // 00449884..00449893
    zero(self, 0x158); zero(self, 0x15C); zero(self, 0x160);                        // 00449899..004498A5
    put<U>(self, 0x138, kOne);                                                      // 004498AB
    put<U>(self, 0x150, kOne);                                                      // 004498B1 (ecx = [esp+24h] = 1.0f)
    zero(self, 0x1AC); zero(self, 0x170); zero(self, 0x1B0);                        // 004498B7..004498C5
    put<U8>(self, 0x167, 0); put<U8>(self, 0x10, 0); put<U8>(self, 0x11, 1); put<U8>(self, 0x17E, 0); // 004498CB..004498D8
    zero(self, 0x174); zero(self, 0x30);                                            // 004498DE..004498E4
    put<U>(self, 0x1B4, kOne); put<U>(self, 0x1B8, kOne);                           // 004498E7..004498ED
    zero(self, 0x34);                                                               // 004498F3
    put<U8>(self, 0x12, 0); put<U8>(self, 0x14, 0); put<U8>(self, 0x16, 0);         // 004498F6..004498FC
    unsigned char* container = static_cast<unsigned char*>(live_operator_new(0x24)); // 004498BD / 004498FF
    if (container != nullptr) {                                                     // 00449907..00449909
        put<const void*>(container, 0, &aurora_vtable_light_container_00740cdc);     // 0044990B (inlined CAurObjectLightContainer ctor)
        zero(container, 0x8); zero(container, 0xC); zero(container, 0x4);           // 00449911..00449917
        zero(container, 0x14); zero(container, 0x18); zero(container, 0x10);        // 0044991A..00449920
        zero(container, 0x1C); zero(container, 0x20);                              // 00449923..00449926
    }
    put<void*>(self, 0x178, container);                                             // 00449934
    put<U>(self, 0x1C8, 1);                                                         // 0044993A
    const I idx = aurora_find_gob_index_00440410(reinterpret_cast<const char*>(at(self, 0x38))); // 0044992B..00449940
    aurora_list_insert_00445020(&aurora_gobs_007a8a58, nullptr, addr(self), idx);   // 00449948..0044994F
    return self;
}

// ---------------------------------------------------------------- Parts
// 004457A0 Part::Part(node), RET4.
Part* __fastcall aurora_part_construct_004457a0(Part* self, void*, MdlNode* node) {
    put<const void*>(self, 0, &aurora_vtable_part_00740ae8);                         // 004457B9
    zero(self, 0x8); zero(self, 0xC); zero(self, 0x10); zero(self, 0x20); zero(self, 0x1C); zero(self, 0x18); // 004457BF..004457CE
    put<U>(self, 0x14, kOne);                                                       // 004457D6
    zero(self, 0x34); zero(self, 0x38); zero(self, 0x30);                           // 004457D9..004457DF
    put<U>(self, 0x24, kOne);                                                       // 004457E2
    put<MdlNode*>(self, 0x4, node);                                                 // 004457E9
    zero(self, 0x3C); zero(self, 0x40); zero(self, 0x48); zero(self, 0x44);         // 004457EC..004457F5
    aurora_part_count_007a8a10 = aurora_part_count_007a8a10 + 1;                    // 004457F8
    zero(self, 0x28);                                                               // 004457FE
    put<U8>(self, 0x2C, 0);                                                         // 00445801
    return self;
}
// 00445A40 PartLight::PartLight(node), RET4.
Part* __fastcall aurora_part_light_construct_00445a40(Part* self, void*, MdlNode* node) {
    aurora_part_construct_004457a0(self, nullptr, node);                            // 00445A48
    put<const void*>(self, 0, &aurora_vtable_part_light_00740c08);                   // 00445A4F
    zero(self, 0x4C); zero(self, 0x50); zero(self, 0x54);                           // 00445A55..00445A5B
    zero(self, 0x94); zero(self, 0x98); zero(self, 0x9C);                           // 00445A5E..00445A6A
    zero(self, 0x5C); zero(self, 0x68); zero(self, 0x6C); zero(self, 0x70); zero(self, 0x58); // 00445A75..00445A81
    zero(self, 0x7C); zero(self, 0x80); zero(self, 0x84); zero(self, 0x88); zero(self, 0x74); // 00445A84..00445A99
    zero(self, 0x90); zero(self, 0x64);                                             // 00445A9C..00445AA2
    put<U>(self, 0x78, kOne); put<U>(self, 0x8C, kOne);                             // 00445AA5..00445AA8
    put<U>(self, 0x60, 0xBF800000u);                                                // 00445AAE (-1.0f)
    return self;
}
// 00445840 PartTriMesh::PartTriMesh(node), RET4.
Part* __fastcall aurora_part_trimesh_construct_00445840(Part* self, void*, MdlNode* node) {
    aurora_part_construct_004457a0(self, nullptr, node);                            // 0044585F..00445864
    put<const void*>(self, 0, &aurora_vtable_part_trimesh_00740b68);                 // 0044586B
    zero_range(self, 0x4C, 0x6C);                                                   // 00445871..00445889
    zero(self, 0x80);                                                               // 00445895
    put<U>(self, 0x84, kOne);                                                       // 0044589B
    zero(self, 0x94); zero(self, 0x70);                                             // 004458A5..004458AB
    put<U>(self, 0x74, 0xFFFFFFFFu); put<U>(self, 0x78, 0xFFFFFFFFu);               // 004458AE..004458B1
    void* const material_block = live_operator_new(0x4C);                           // 0044588F..004458B4
    void* const material = material_block ? aurora_material_construct_0047b290(material_block, nullptr) : nullptr; // 004458C0..004458D2
    put<void*>(self, 0x44, material);                                               // 004458D4
    put<U8>(self, 0x7C, 0);                                                         // 004458D7
    put<U8>(self, 0x7D, 1);                                                         // 004458DA
    put<I>(self, 0x8C, aurora_mesh_serial_0078e328);                                // 004458DE..004458E3
    aurora_mesh_serial_0078e328 = aurora_mesh_serial_0078e328 + 1;                  // 004458E9
    MaxTree* const owner = aurora_tree_as_model_0043e1c0(get<MaxTree*>(node, 8), nullptr); // 004458EF..00445907 (inlined AsModel)
    if (owner != nullptr) put<U8>(self, 0x7D, get<U8>(owner, 0x53));               // 00445909..0044590E
    aurora_trimesh_part_count_007a8a14 = aurora_trimesh_part_count_007a8a14 + 1;    // 00445911
    MdlNode* const mesh = aurora_node_as_trimesh_0043e400(node, nullptr);           // 00445917..00445923 (inlined)
    if (get<U>(mesh, 0x138) != 0) {                                                 // 00445925..0044592B
        const I vertices = static_cast<I>(get<I16>(mesh, 0x180));
        unsigned char* const copy = static_cast<unsigned char*>(live_operator_new(static_cast<U>(vertices) << 3)); // 00445931..0044593C
        put<void*>(self, 0x90, copy);                                               // 00445941
        MaxTree* const model = aurora_tree_as_model_0043e1c0(get<MaxTree*>(mesh, 8), nullptr); // 00445947..0044595D
        unsigned char* const pool = static_cast<unsigned char*>(aurora_pool_lock_00423f10(get<U>(model, 0xAC), 0, 0, 8)); // 0044595F..00445966
        for (I i = 0; i < static_cast<I>(get<I16>(mesh, 0x180)); ++i) {            // 00445970..004459D2
            const U off0 = get<U>(mesh, 0x14C) * static_cast<U>(i) + get<U>(mesh, 0x194) + get<U>(mesh, 0x160); // 00445980..00445991
            ptr<U>(get<U>(self, 0x90))[i * 2] = get<U>(pool, off0);                // 00445997..004459A0
            const U off1 = get<U>(mesh, 0x14C) * static_cast<U>(i) + get<U>(mesh, 0x194) + get<U>(mesh, 0x160); // 004459A3..004459B4
            ptr<U>(get<U>(self, 0x90))[i * 2 + 1] = get<U>(pool, off1 + 4);        // 004459BA..004459C4
        }
        MaxTree* const model2 = aurora_tree_as_model_0043e1c0(get<MaxTree*>(mesh, 8), nullptr); // 004459D4..004459E4
        aurora_pool_unlock_00423f30(get<U>(model2, 0xAC));                          // 004459E6..004459ED
    } else {
        put<void*>(self, 0x90, nullptr);                                            // 004459F7
    }
    return self;
}
// 00447980 PartDanglyMesh::PartDanglyMesh(node), RET4.
Part* __fastcall aurora_part_dangly_construct_00447980(Part* self, void*, MdlNode* node) {
    aurora_part_trimesh_construct_00445840(self, nullptr, node);                    // 004479A0..004479A5
    put<const void*>(self, 0, &aurora_vtable_part_dangly_00740d78);                  // 004479AC
    zero(self, 0x98); zero(self, 0x9C); zero(self, 0xA0); zero(self, 0xB0); zero(self, 0xAC); zero(self, 0xA8); // 004479B2..004479D0
    put<U>(self, 0xA4, kOne);                                                       // 004479DA
    zero(self, 0xB4);                                                               // 004479E4
    const U bytes = static_cast<U>(static_cast<I>(get<I16>(aurora_node_as_trimesh_0043e400(node, nullptr), 0x180)) * 12); // 004479EA..00447A03
    unsigned char* const copy = static_cast<unsigned char*>(live_operator_new(bytes)); // 00447A06..00447A07
    put<void*>(self, 0xB4, copy);                                                   // 00447A0E
    MdlNode* const dangly = aurora_node_as_dangly_0043e380(node, nullptr);          // 00447A14..00447A44
    const U bytes2 = static_cast<U>(static_cast<I>(get<I16>(aurora_node_as_trimesh_0043e400(node, nullptr), 0x180)) * 12);
    std::memcpy(copy, ptr<void>(get<U>(dangly, 0x1B4)), bytes2);                   // 00447A46..00447A5E
    const U bytes3 = static_cast<U>(static_cast<I>(get<I16>(aurora_node_as_trimesh_0043e400(node, nullptr), 0x180)) * 12);
    aurora_pool_request_00423ee0(0, bytes3, 2, 0, ptr<U>(addr(at(self, 0xB8))));  // 00447A60..00447A8A
    void* const dst = aurora_pool_lock_00423f10(get<U>(self, 0xB8), 0, 0, 1);       // 00447A8F..00447A98
    const U bytes4 = static_cast<U>(static_cast<I>(get<I16>(aurora_node_as_trimesh_0043e400(node, nullptr), 0x180)) * 12);
    std::memcpy(dst, ptr<void>(get<U>(self, 0xB4)), bytes4);                        // 00447A9D..00447ACF
    aurora_pool_unlock_00423f30(get<U>(self, 0xB8));                                // 00447AD1..00447AD8
    return self;
}
// 004478D0 PartLightsaber::PartLightsaber(node), RET4.
Part* __fastcall aurora_part_saber_construct_004478d0(Part* self, void*, MdlNode* node) {
    aurora_part_trimesh_construct_00445840(self, nullptr, node);                    // 004478D5..004478D8
    put<const void*>(self, 0, &aurora_vtable_part_saber_00740ce0);                   // 004478DD
    for (U k = 0; k < 0x19; ++k) {                                                  // 004478E3..004478FC
        const U o = 0xFC + k * 12;
        zero(self, o); zero(self, o + 4); zero(self, o + 8);
    }
    for (U k = 0; k < 0x19; ++k) {                                                  // 004478FE..00447923
        const U o = 0x228 + k * 16;
        zero(self, o + 0xC); zero(self, o + 8); zero(self, o + 4); put<U>(self, o, kOne);
    }
    zero(self, 0x3C0); zero(self, 0x3C4); zero(self, 0x3C8);                        // 00447925..00447931
    put<U16>(self, 0x3BC, 0); put<U16>(self, 0x3BA, 0); put<U16>(self, 0x3B8, 0);   // 00447937..00447945
    return self;
}
// 00456A60 CHitInfo::ClearOutput.
void __fastcall aurora_hit_info_clear_output_00456a60(void* self, void*) {
    zero(self, 0x18);                                                               // 00456A6D
    U* const ref = ptr<U>(addr(at(self, 0x38)));                                   // 00456A6A
    if (*ref != 0) list_at(ptr<void>(*ref), 4)->RemoveReverse32(addr(ref));        // 00456A70..00456A92
    *ref = 0;                                                                       // 00456A9F
    const U triples[3] = {0x40u, 0x1Cu, 0x28u};                                   // 00456AA1..00456AC5 (three 12-byte zero vectors)
    for (U k = 0; k < 3; ++k) { zero(self, triples[k]); zero(self, triples[k] + 4); zero(self, triples[k] + 8); }
    zero(self, 0x34); zero(self, 0x4C); zero(self, 0x50); zero(self, 0x54);         // 00456AC9..00456AD2
    put<U>(self, 0x3C, 0xFFFFFFFFu);                                                // 00456AD5
}
// 004578F0 CHitInfo::CHitInfo(a, b, c), RET0Ch.
void* __fastcall aurora_hit_info_construct_004578f0(void* self, void*, U a, U b, U c) {
    zero(self, 0xC); zero(self, 0x10); zero(self, 0x8);                             // 0045790F..00457915
    zero_range(self, 0x1C, 0x30);                                                   // 00457918..00457927
    zero(self, 0x38); zero(self, 0x40); zero(self, 0x44); zero(self, 0x48);         // 0045792E..0045793B
    put<U>(self, 0x0, a);                                                           // 0045793E
    put<U>(self, 0x4, b);                                                           // 00457946
    put<U8>(self, 0x5D, 1);                                                         // 00457951
    put<U>(self, 0x14, c);                                                          // 00457956
    zero(self, 0xC); zero(self, 0x58);                                              // 00457959..0045795C
    put<U8>(self, 0x5C, 0); put<U8>(self, 0x5E, 0); put<U8>(self, 0x5F, 0); put<U8>(self, 0x60, 0); // 0045795F..00457968
    aurora_hit_info_clear_output_00456a60(self, nullptr);                           // 0045796B
    return self;
}
// 0049B380 PartEmitter::PartEmitter(node), RET4.
Part* __fastcall aurora_part_emitter_construct_0049b380(Part* self, void*, MdlNode* node) {
    aurora_part_construct_004457a0(self, nullptr, node);                            // 0049B3A0..0049B3A5
    put<const void*>(self, 0, &aurora_vtable_part_emitter_00743340);                 // 0049B3AA
    zero_range(self, 0x11C, 0x190);                                                 // 0049B3B2..0049B460
    zero(self, 0x1A0); zero(self, 0x19C); zero(self, 0x198);                        // 0049B466..0049B472
    put<U>(self, 0x194, kOne);                                                      // 0049B47D
    zero(self, 0x1B0); zero(self, 0x1AC); zero(self, 0x1A8);                        // 0049B483..0049B48F
    put<U>(self, 0x1A4, kOne);                                                      // 0049B495
    zero(self, 0x1C0); zero(self, 0x1BC); zero(self, 0x1B8);                        // 0049B4A1..0049B4AD
    put<U>(self, 0x1B4, kOne);                                                      // 0049B4B3
    zero(self, 0x1C4);                                                              // 0049B4BD
    zero(self, 0x1CC); zero(self, 0x1D0); zero(self, 0x1C8);                        // 0049B4C0..0049B4CC
    zero(self, 0x1D8); zero(self, 0x1DC); zero(self, 0x1D4);                        // 0049B4D2..0049B4DE
    zero(self, 0x1E4);                                                              // 0049B4EA
    zero(self, 0x50); zero(self, 0xD8); zero(self, 0x54);                           // 0049B4F6..0049B4FF
    put<U>(self, 0xDC, 0x437F0000u); put<U>(self, 0xE0, 0x437F0000u); put<U>(self, 0xE4, 0x437F0000u); // 0049B502..0049B50E (255.0f)
    zero(self, 0x58); zero(self, 0xF0);                                             // 0049B514..0049B517
    zero_range(self, 0x5C, 0x90);                                                   // 0049B51D..0049B555
    zero(self, 0xE8); zero(self, 0x94); zero(self, 0x98); zero(self, 0xEC);         // 0049B55B..0049B56D
    zero_range(self, 0x9C, 0xB0);                                                   // 0049B573..0049B591
    put<U>(self, 0xB4, kOne);                                                       // 0049B597
    zero_range(self, 0xB8, 0xC8);                                                   // 0049B59D..0049B5B5
    zero(self, 0x104); zero(self, 0x108); zero(self, 0x10C); zero(self, 0x110); zero(self, 0xFC); // 0049B5BB..0049B5D3
    put<U>(self, 0xF4, 0x461C4000u); put<U>(self, 0xF8, 0x461C4000u);               // 0049B5D9..0049B5DF (10000.0f)
    put<U8>(self, 0x1F6, 0);                                                        // 0049B5E5
    zero(self, 0x1E0);                                                              // 0049B5EB
    put<U>(self, 0xD0, kOne);                                                       // 0049B5F1
    zero(self, 0xD4); zero(self, 0x1EC);                                            // 0049B5F7..0049B5FD
    U* const ref_1e4 = ptr<U>(addr(at(self, 0x1E4)));                              // 0049B4E4 (edi)
    if (*ref_1e4 != 0) list_at(ptr<void>(*ref_1e4), 4)->RemoveReverse32(addr(ref_1e4)); // 0049B603..0049B60D
    *ref_1e4 = 0;                                                                   // 0049B612
    U* const ref_1c4 = ptr<U>(addr(at(self, 0x1C4)));                              // 0049B49B (ebp)
    if (*ref_1c4 != 0) list_at(ptr<void>(*ref_1c4), 0)->RemoveReverse32(addr(ref_1c4)); // 0049B614..0049B61C (ecx = [ebp], list at +0)
    *ref_1c4 = 0;                                                                   // 0049B623
    void* const hit_block = live_operator_new(0x64);                                // 0049B621..0049B626
    void* const hit = hit_block ? aurora_hit_info_construct_004578f0(hit_block, nullptr, 1, 0xBFFFFFFFu, 0) : nullptr; // 0049B632..0049B64C
    put<void*>(self, 0x1E8, hit);                                                   // 0049B652
    aurora_emitter_count_00830470 = aurora_emitter_count_00830470 + 1;              // 0049B658..0049B65F
    return self;
}
// 0049D1D0 LightningEmitter::LightningEmitter(node), RET4.
Part* __fastcall aurora_lightning_emitter_construct_0049d1d0(Part* self, void*, MdlNode* node) {
    aurora_part_emitter_construct_0049b380(self, nullptr, node);                    // 0049D1EA..0049D1ED
    put<const void*>(self, 0, &aurora_vtable_lightning_emitter_00743610);            // 0049D1F4
    zero_range(self, 0x1F8, 0x288);                                                 // 0049D1FA..0049D2DC
    zero(self, 0xC0);                                                               // 0049D2D6
    put<U8>(self, 0x28C, 0);                                                        // 0049D2E2
    return self;
}
// 0048D770 Detonate callback (cdecl): third argument (part) byte [+1F6] = 1.
void __cdecl aurora_emitter_detonate_0048d770(void*, void*, Part* part) {
    put<U8>(part, 0x1F6, 1);
}

// ---------------------------------------------------------------- per-node instance creators (thiscall RET4)
namespace {
template <class Fn> Part* make(U size, Fn construct) {
    void* const block = live_operator_new(size);
    return block ? construct(static_cast<Part*>(block)) : nullptr;
}
}
// 00447B20: new(4C) Part::Part.
Part* __fastcall aurora_node_instance_header_00447b20(MdlNode* self, void*, Gob*) {
    return make(0x4C, [&](Part* p) { return aurora_part_construct_004457a0(p, nullptr, self); });
}
// 00447B90: new(A0) PartLight; copy +A0/+9C/+98/+94 to +5C/+6C/+70/+68; [+48] = 0; gob: (node[98] || node[9C]) ->
// gob[BC] = 1; node[98] == 0 -> append part to gob +19C list (growth cap ? 2*cap : 8 via 005E03C0).
Part* __fastcall aurora_node_instance_light_00447b90(MdlNode* self, void*, Gob* gob) {
    Part* const part = make(0xA0, [&](Part* p) { return aurora_part_light_construct_00445a40(p, nullptr, self); }); // 00447BA8..00447BD3
    put<U>(part, 0x5C, get<U>(self, 0xA0));                                         // 00447BD5..00447BDB
    put<U>(part, 0x6C, get<U>(self, 0x9C));                                         // 00447BDE..00447BE4
    put<U>(part, 0x70, get<U>(self, 0x98));                                         // 00447BE7..00447BED
    put<U>(part, 0x68, get<U>(self, 0x94));                                         // 00447BF0..00447BF6
    put<U>(part, 0x48, 0);                                                          // 00447C07
    if (gob != nullptr) {                                                           // 00447BF9..00447C0E
        if (get<U>(self, 0x98) != 0 || get<U>(self, 0x9C) != 0) put<U>(gob, 0xBC, 1); // 00447C10..00447C24
        if (get<U>(self, 0x98) == 0) {                                              // 00447C2E..00447C36
            List* const l = list_at(gob, 0x19C);
            if (l->size == l->capacity)                                             // 00447C38..00447C49
                menu_list_growth::allocate_005e03c0(l, nullptr, l->capacity ? static_cast<U>(l->capacity) * 2u : 8u); // 00447C4B..00447C5B
            ptr<U>(l->data)[l->size] = addr(part);                                  // 00447C60..00447C65
            l->size = l->size + 1;                                                  // 00447C68
        }
    }
    return part;
}
// 00447C80: new(4C) Part::Part, vtable 00740EB0.
Part* __fastcall aurora_node_instance_camera_00447c80(MdlNode* self, void*, Gob*) {
    return make(0x4C, [&](Part* p) { aurora_part_construct_004457a0(p, nullptr, self); put<const void*>(p, 0, &aurora_vtable_part_camera_00740eb0); return p; });
}
// 00447CE0: new(54) Part::Part, vtable 00740F30, [+4C] = 0.
Part* __fastcall aurora_node_instance_reference_00447ce0(MdlNode* self, void*, Gob*) {
    return make(0x54, [&](Part* p) { aurora_part_construct_004457a0(p, nullptr, self); put<const void*>(p, 0, &aurora_vtable_part_reference_00740f30); zero(p, 0x4C); return p; });
}
// 00447D60: new(98) PartTriMesh.
Part* __fastcall aurora_node_instance_trimesh_00447d60(MdlNode* self, void*, Gob*) {
    return make(0x98, [&](Part* p) { return aurora_part_trimesh_construct_00445840(p, nullptr, self); });
}
// 00447DD0: new(98) PartTriMesh, vtable 00740FB0 (animmesh).
Part* __fastcall aurora_node_instance_anim_mesh_00447dd0(MdlNode* self, void*, Gob*) {
    return make(0x98, [&](Part* p) { aurora_part_trimesh_construct_00445840(p, nullptr, self); put<const void*>(p, 0, &aurora_vtable_part_anim_mesh_00740fb0); return p; });
}
// 00447E30: new(98) PartTriMesh, vtable 00741048 (AABB).
Part* __fastcall aurora_node_instance_aabb_00447e30(MdlNode* self, void*, Gob*) {
    return make(0x98, [&](Part* p) { aurora_part_trimesh_construct_00445840(p, nullptr, self); put<const void*>(p, 0, &aurora_vtable_part_aabb_00741048); return p; });
}
// 00447E90: new(BC) PartDanglyMesh.
Part* __fastcall aurora_node_instance_dangly_00447e90(MdlNode* self, void*, Gob*) {
    return make(0xBC, [&](Part* p) { return aurora_part_dangly_construct_00447980(p, nullptr, self); });
}
// 004480C0: new(3CC) PartLightsaber.
Part* __fastcall aurora_node_instance_saber_004480c0(MdlNode* self, void*, Gob*) {
    return make(0x3CC, [&](Part* p) { return aurora_part_saber_construct_004478d0(p, nullptr, self); });
}
// 00487510: new(98) PartTriMesh, vtable 00740E10 (skin).
Part* __fastcall aurora_node_instance_skin_00487510(MdlNode* self, void*, Gob*) {
    return make(0x98, [&](Part* p) { aurora_part_trimesh_construct_00445840(p, nullptr, self); put<const void*>(p, 0, &aurora_vtable_part_skin_00740e10); return p; });
}
// 0049D5C0: emitter update string at node+70: "Fountain"/"Explosion"/"Single" (byte compares including NUL,
// repe cmpsb 9/10/7) -> new(1F8) PartEmitter + subtype vtable; Explosion with a gob registers
// gob->vtbl[28](Detonate 0048D770, "Detonate", part, 9999.0f raw 461C3C00, 0); otherwise new(290)
// LightningEmitter. Then part[40] = gob; part->vtbl[7C]().
Part* __fastcall aurora_node_instance_emitter_0049d5c0(MdlNode* self, void*, Gob* gob) {
    const unsigned char* const update = at(self, 0x70);                            // 0049D5DF
    Part* part;
    if (std::memcmp(update, "Fountain", 9) == 0) {                                  // 0049D5E2..0049D5F2 (7436C0)
        part = make(0x1F8, [&](Part* p) { aurora_part_emitter_construct_0049b380(p, nullptr, self); put<const void*>(p, 0, &aurora_vtable_part_emitter_fountain_00743478); return p; });
    } else if (std::memcmp(update, "Explosion", 10) == 0) {                         // 0049D62A..0049D63A (7436B4)
        part = make(0x1F8, [&](Part* p) { aurora_part_emitter_construct_0049b380(p, nullptr, self); put<const void*>(p, 0, &aurora_vtable_part_emitter_explosion_00743500); return p; });
        if (gob != nullptr)                                                         // 0049D66D..0049D677
            get<const GobVTable*>(gob, 0)->register_callback(gob, nullptr, &aurora_emitter_detonate_0048d770, "Detonate", part, 0x461C3C00u, 0); // 0049D67D..0049D694 (7436A8)
    } else if (std::memcmp(update, "Single", 7) == 0) {                             // 0049D699..0049D6A9 (7436A0)
        part = make(0x1F8, [&](Part* p) { aurora_part_emitter_construct_0049b380(p, nullptr, self); put<const void*>(p, 0, &aurora_vtable_part_emitter_single_00743588); return p; });
    } else {
        part = make(0x290, [&](Part* p) { return aurora_lightning_emitter_construct_0049d1d0(p, nullptr, self); }); // 0049D6DE..0049D707
    }
    put<Gob*>(part, 0x40, gob);                                                     // 0049D715
    part_vt(part)->initialize(part, nullptr);                                       // 0049D711..0049D718
    return part;
}
// 0063E7C0 return_false_4: xor eax,eax; ret 4.
// [M45 compose] 0063E7C0 (xor eax,eax; ret 4) body removed: D68 earliest-staged owner lane-packed-file packed_return_false_0063e7c0 (M43) (definition removed; declaration kept)
U __fastcall aurora_return_false_4_0063e7c0(void*, void*, U);

// 00448A70 MdlNode::CreateInstanceDispatch(gob), RET4: switch on the signed type word (jump table 00448BB0 /
// index table 00448BCC for types 1..21h; explicit compares for 61h, A1h, 121h, 221h, 401h, 821h).
// Each arm passes the inlined cast (type & mask == mask ? node : 0) and tail-jumps; unknown -> 0.
Part* __fastcall aurora_node_create_instance_00448a70(MdlNode* self, void*, Gob* gob) {
    switch (static_cast<I>(get<I16>(self, 0))) {                                   // 00448A70..00448A73
    case 0x001: return aurora_node_instance_header_00447b20(self, nullptr, gob);   // 00448A99
    case 0x003: return aurora_node_instance_light_00447b90(aurora_node_as_light_0043e3d0(self, nullptr), nullptr, gob);       // 00448A9E
    case 0x005: return aurora_node_instance_emitter_0049d5c0(aurora_node_as_emitter_0043e3c0(self, nullptr), nullptr, gob);   // 00448AB1
    case 0x009: return aurora_node_instance_camera_00447c80(aurora_node_as_camera_0043e370(self, nullptr), nullptr, gob);     // 00448AC4
    case 0x011: return aurora_node_instance_reference_00447ce0(aurora_node_as_reference_0043e3e0(self, nullptr), nullptr, gob); // 00448AD7
    case 0x021: return aurora_node_instance_trimesh_00447d60(aurora_node_as_trimesh_0043e400(self, nullptr), nullptr, gob);   // 00448AEA
    case 0x061: return aurora_node_instance_skin_00487510(aurora_node_as_skin_0043e3f0(self, nullptr), nullptr, gob);         // 00448AFD
    case 0x0A1: return aurora_node_instance_anim_mesh_00447dd0(aurora_node_as_anim_mesh_0043e360(self, nullptr), nullptr, gob); // 00448B40
    case 0x121: return aurora_node_instance_dangly_00447e90(aurora_node_as_dangly_0043e380(self, nullptr), nullptr, gob);     // 00448B2A
    case 0x221: return aurora_node_instance_aabb_00447e30(aurora_node_as_aabb_0043e340(self, nullptr), nullptr, gob);         // 00448B53
    case 0x401: aurora_return_false_4_0063e7c0(self, nullptr, addr(gob)); return nullptr;                                     // 00448B98 (0063E7C0)
    case 0x821: return aurora_node_instance_saber_004480c0(aurora_node_as_saber_0043e3a0(self, nullptr), nullptr, gob);       // 00448B80
    default: return nullptr;                                                                                                   // 00448B7B
    }
}
// 00444FA0 CExoArrayList<Part*>::SetSize(n), RET4: n == 0 -> free data (data = 0), size = 0, capacity = 0;
// else 005E03C0(n), size = n.
void __fastcall aurora_part_list_set_size_00444fa0(List* self, void*, I count) {
    if (count == 0) {
        if (self->data != 0) { live_free(ptr<void>(self->data)); self->data = 0; } // 00444FAC..00444FBB
        self->size = 0;                                                             // 00444FBD
        self->capacity = 0;                                                         // 00444FC1
        return;
    }
    menu_list_growth::allocate_005e03c0(self, nullptr, static_cast<U>(count));     // 00444FCC..00444FCF
    self->size = count;                                                             // 00444FD4
}
// 00449200 CreateInstanceTreeR(node, gob), cdecl (the original inlines two recursion levels; level 1 uses
// an inlined SetSize, deeper levels call 00444FA0 - same effect): null -> 0; part = dispatch(gob);
// part[40] = gob; [00798B38] = 1.0f; node->ControlDispatch(part, -1.0f, 0, 1.0f); children list +30 sized to
// node[30]; each child instance stored at [i] and its [3C] = part; part[28] = (i16)node[2].
Part* __cdecl aurora_create_instance_tree_00449200(MdlNode* node, Gob* gob) {
    if (node == nullptr) return nullptr;                                            // 00449204..00449212
    Part* const part = aurora_node_create_instance_00448a70(node, nullptr, gob);   // 0044921A..0044921D
    put<Gob*>(part, 0x40, gob);                                                     // 00449230
    aurora_control_weight_00798b38 = 0x3F800000u;                                   // 0044923A
    aurora_node_control_dispatch_0043f890(node, nullptr, part, 0xBF800000u, 0, 0x3F800000u); // 00449222..00449244
    aurora_part_list_set_size_00444fa0(list_at(part, 0x30), nullptr, get<I>(node, 0x30)); // 00449249..00449275 (inlined at level 1)
    for (I i = 0; i < get<I>(node, 0x30); ++i) {                                    // 00449278..004493F1
        Part* const child = aurora_create_instance_tree_00449200(ptr<MdlNode>(ptr<U>(get<U>(node, 0x2C))[i]), gob); // 00449290..004493D2
        ptr<U>(get<U>(part, 0x30))[i] = addr(child);                                // 004493D8..004493DB
        put<Part*>(ptr<Part>(ptr<U>(get<U>(part, 0x30))[i]), 0x3C, part);           // 004493DE..004493E4
    }
    put<I>(part, 0x28, static_cast<I>(get<I16>(node, 2)));                          // 004493F7..004493FD
    return part;
}

// ---------------------------------------------------------------- NewCAurObject
// 00449CC0 NewCAurObject(name, gob_name, FILE* mdl, FILE* mdx), cdecl.
Gob* __cdecl aurora_new_aur_object_00449cc0(const char* name, const char* gob_name, std::FILE* mdl, std::FILE* mdx) {
    const U8 saved_gate = aurora_model_gate_007b8fc0;                               // 00449CD8..00449CDF
    MaxTree* variant_x = nullptr;                                                   // ebp
    MaxTree* variant_z = nullptr;                                                   // ebx
    aurora_model_gate_007b8fc0 = 0;                                                 // 00449CEF
    MaxTree* model;
    if (mdl == nullptr) {                                                           // 00449CEC..00449CF6
        model = aurora_find_model_00464110(name);                                   // 00449CFC..00449D06
        if (aurora_model_variants_00828068 != 0) {                                  // 00449D0A..00449D14
            char buffer[0x40];                                                      // [esp+18h]
            const std::size_t length = std::strlen(name);
            std::memcpy(buffer, name, length + 1);                                  // 00449D1A..00449D2A
            buffer[length + 2] = 0; buffer[length] = '_'; buffer[length + 1] = 'x'; // 00449D3A..00449D49
            variant_x = aurora_find_model_00464110(buffer);                         // 00449D4E
            std::memcpy(buffer, name, length + 1);                                  // 00449D58..00449D68
            buffer[length] = '_'; buffer[length + 1] = 'z'; buffer[length + 2] = 0; // 00449D7E..00449D87
            variant_z = aurora_find_model_00464110(buffer);                         // 00449D8B
        }
    } else {
        model = aurora_load_model_00464200(mdl, mdx);                               // 00449D97..00449D9D
    }
    if (model == nullptr) {                                                         // 00449DA9..00449DAF
        aurora_model_gate_007b8fc0 = saved_gate;                                    // 00449DB1..00449DB8
        return nullptr;
    }
    const char* const instance_name = (gob_name != nullptr && gob_name[0] != 0)     // 00449DCE..00449DD9
        ? gob_name : reinterpret_cast<const char*>(at(model, 8));                   // 00449DDB
    void* const block = live_operator_new(0x1CC);                                   // 00449DDE..00449DE3
    Gob* const gob = block ? gob_construct_00449440(static_cast<Gob*>(block), nullptr, instance_name) : nullptr; // 00449DEB..00449E07
    put<MaxTree*>(gob, 0x58, model);                                                // 00449E0D
    put<I>(model, 0x54, get<I>(model, 0x54) + 1);                                   // 00449E10..00449E16
    put<MaxTree*>(gob, 0x5C, variant_x);                                            // 00449E21
    if (variant_x != nullptr) put<I>(variant_x, 0x54, get<I>(variant_x, 0x54) + 1); // 00449E24..00449E26
    put<MaxTree*>(gob, 0x60, variant_z);                                            // 00449E2B
    if (variant_z != nullptr) put<I>(variant_z, 0x54, get<I>(variant_z, 0x54) + 1); // 00449E2E..00449E30
    Part* const root = aurora_create_instance_tree_00449200(get<MdlNode*>(model, 0x28), gob); // 00449E33..00449E38
    part_vt(root)->animate(root, nullptr, addr(root));                              // 00449E3F..00449E47 (push edi; ecx = edi)
    put<Part*>(gob, 0x6C, root);                                                    // 00449E4A
    aurora_node_process_skin_seams_00438fa0(get<MdlNode*>(root, 4), nullptr);       // 00449E4D..00449E50
    put<Gob*>(get<Part*>(gob, 0x6C), 0x40, gob);                                    // 00449E57..00449E5A
    if (variant_x != nullptr) {                                                     // 00449E55..00449E5D
        Part* const part = aurora_create_instance_tree_00449200(get<MdlNode*>(variant_x, 0x28), gob); // 00449E5F..00449E64
        part_vt(part)->animate(part, nullptr, addr(part));                          // 00449E69..00449E73
        put<Part*>(gob, 0x70, part);                                                // 00449E76
        put<Gob*>(part, 0x40, gob);                                                 // 00449E79
    }
    if (variant_z != nullptr) {                                                     // 00449E7C..00449E7E
        Part* const part = aurora_create_instance_tree_00449200(get<MdlNode*>(variant_z, 0x28), gob); // 00449E80..00449E85
        part_vt(part)->animate(part, nullptr, addr(part));                          // 00449E8A..00449E94
        put<Part*>(gob, 0x74, part);                                                // 00449E97
        put<Gob*>(part, 0x40, gob);                                                 // 00449E9A
    }
    aurora_create_reference_objects_004448c0(gob, get<Part*>(gob, 0x6C));          // 00449E9D..00449EA2
    if (get<Part*>(gob, 0x70) != nullptr) aurora_create_reference_objects_004448c0(gob, get<Part*>(gob, 0x70)); // 00449EA7..00449EB3
    if (get<Part*>(gob, 0x74) != nullptr) aurora_create_reference_objects_004448c0(gob, get<Part*>(gob, 0x74)); // 00449EBB..00449EC4
    gob_vt(gob)->do_event(gob, nullptr, "_EmitterTarget", 0);                       // 00449ECC..00449ED7 (7413FC)
    aurora_part_init_material_00442d70(gob, get<Part*>(gob, 0x6C), "");            // 00449EDD..00449EE7 (73D71C)
    if (get<Part*>(gob, 0x70) != nullptr) aurora_part_init_material_00442d70(gob, get<Part*>(gob, 0x70), ""); // 00449EEC..00449EFD
    if (get<Part*>(gob, 0x74) != nullptr) aurora_part_init_material_00442d70(gob, get<Part*>(gob, 0x74), ""); // 00449F05..00449F13
    aurora_model_gate_007b8fc0 = saved_gate;                                        // 00449F1B..00449F1F
    gob_vt(gob)->slot184(gob, nullptr);                                             // 00449F25..00449F29
    put<U8>(gob, 0x167, 1);                                                         // 00449F35
    gob_vt(gob)->animate(gob, nullptr, 0);                                          // 00449F2F..00449F3C (push 0)
    return gob;
}
