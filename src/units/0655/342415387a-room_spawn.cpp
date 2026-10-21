// lane-3d-room: Scene::SpawnRoom (00456F30, CAurScene vtbl+70) and the room_spawn sub-lane address list
// assigned by lane-3d-render/SUBLANES.json ("room_spawn": 00421FC0, 0044FDB0, 004548A0, 00456F30, 00458BA0).
// Evidence: dumpbin /disasm of swkotor.exe (GOG K1, image base 00400000); instruction addresses cited per behavior.
// .rdata literals read from the pinned image: 0073D5D0 "%s", 0073D5D4 "a" (word 0x0061), 0073D71C "" (empty).
// Conventions (DECISIONS D1 + amendment): original __thiscall entries are __fastcall(Self*, void* edx_unused, ...);
// callers pass nullptr for EDX; no struct is returned by value through a D1 slot (VectorMin/VectorMax are cdecl
// with the original's explicit hidden out pointer). Virtual slots reload the receiver's vtable per call.
// Allocator family: original operator new 006FA7E6 / _free 006FA390 -> M41 live_operator_new / live_free
// (same binding as lane-3d-render). eh_vector_destructor_iterator 006FA852 -> c2da_destroy_array_006fa852 (strict link).
// D30 (provisional): no floating-point arithmetic is performed in this unit; all float data are copied as raw DWORDs
// or handed to the VectorMin/VectorMax owners. Unwind: the original EH states of 00456F30 (handler 0071675D:
// state 0 = delete room memory if CAurRoom::CAurRoom throws, 1 = delete nodedata memory, 2/3 = free the two local
// CExoArrayList buffers, 4 = delete[] the grass-bin array) and of 004548A0 (handler 00716653) are cited, not emitted:
// every callee here is extern "C" (assumed non-throwing under /EHsc) and live_operator_new returns null rather than
// throwing. The normal-path cleanup (004576B7..004577F0) is emitted exactly.
#include <cstdint>
#include <cstdio>
#include <cstring>
#include "6c81d8874d-scene3d.h"

// ---------------------------------------------------------------- lane-3d-model providers (exact declarations copied
// from lane-3d-model/source/model_load.hpp + model_instance.hpp; those headers cannot be included together with
// scene3d.h because scene3d.h re-declares model_add_ref_0043e240 (used here through scene3d.h: the symbol
// lane-3d-model actually defines is @model_add_ref_0043e240@12) / gob_construct_00449440 / model_construct_0044aa70
// with different return types -- see scene3d.h.proposed.diff).
namespace aurora_model {
struct MaxTree;
struct MdlNode;
struct Gob;
struct Part;
}
extern "C" {
aurora_model::MaxTree* __cdecl aurora_find_model_00464110(const char* name);
aurora_model::MdlNode* __fastcall aurora_node_as_trimesh_0043e400(aurora_model::MdlNode* self, void* unused_edx);
void __cdecl aurora_part_init_material_00442d70(aurora_model::Gob* gob, aurora_model::Part* part, const char* prefix);
aurora_model::Gob* __cdecl aurora_new_aur_object_00449cc0(const char* name, const char* gob_name, std::FILE* mdl, std::FILE* mdx);
// strict-link providers
void __stdcall c2da_destroy_array_006fa852(void* first, std::uint32_t size, std::int32_t count, void (__fastcall* destroy)(void*, void*));
std::int32_t __cdecl kotor_stricmp_0070acaf(const char*, const char*);
// lane-font provider (font_info_parse.cpp): CExoArrayList<Vector>::allocate, RET 4
void __fastcall vector_list_allocate_00421310(kotor_scene3d::PointerList* list, void* unused_edx, std::int32_t size);
}

// ---------------------------------------------------------------- lowers with no provider yet (names proposed here;
// owners per SUBLANES.json in brackets). Conventions verified from each callee's prologue/RET.
extern "C" {
using kotor_scene3d::U; using kotor_scene3d::I; using kotor_scene3d::Vector; using kotor_scene3d::Quaternion;
using kotor_scene3d::PointerList;
// [core] 004204B0 / 004204C0: cdecl, no arguments, RET (EAX not consumed by 00456F53 / 00457751)
void __cdecl aur_texture_set_mode_static_004204b0();
void __cdecl aur_texture_set_mode_dynamic_004204c0();
// [core] 00452000 findnode(node, name): cdecl, RET; EAX tested at 00457079
void* __cdecl find_node_00452000(aurora_model::MdlNode* node, const char* name);
// [core] 00451DC0 ComputePartBBox(part): cdecl, RET; EAX not consumed (00457282)
void __cdecl compute_part_bbox_00451dc0(void* part);
// [core] 004AAE70 VectorMin / 004AAEE0 VectorMax: cdecl, explicit out pointer [esp+4], two by-value Vectors,
// EAX = out (consumed at 0045732B / 004572D8 / 004573F4 / 00457446)
Vector* __cdecl vector_min_004aae70(Vector* out, Vector a, Vector b);
Vector* __cdecl vector_max_004aaee0(Vector* out, Vector a, Vector b);
// [lane-3d-model, MAP provider] 00449410 CreateInstanceTree(node, gob): cdecl, RET (calls 00449200 then part vtbl+6C)
aurora_model::Part* __cdecl aurora_create_instance_tree_00449410(aurora_model::MdlNode* node, aurora_model::Gob* gob);
// [handoff lane-3d-model] 00453F10 CollapsePartTree: cdecl, 0x30 bytes of arguments (cleaned at 0045721B), RET;
// EAX not consumed by 004571A4
void __cdecl aurora_collapse_part_tree_00453f10(aurora_model::Gob* gob, aurora_model::Part* tree, PointerList* parts,
    Vector position, Quaternion orientation, void* scene, void* room);
// [bsp_visibility] 0046BFB0 nodedata::nodedata: thiscall, RET 0, EAX = this (00456FE4)
void* __fastcall nodedata_construct_0046bfb0(void* self, void* unused_edx);
// [bsp_visibility] 004AD630 BSPNode::BSPNode(Vector, int): thiscall, RET 10h; EAX not consumed by 004548F7
void* __fastcall bsp_node_construct_004ad630(void* self, void* unused_edx, Vector v, I flag);
// [bsp_visibility] 0046E290 SceneBSPAddPart(scene, part, 0): cdecl, RET; EAX not consumed (004574B7)
void __cdecl scene_bsp_add_part_0046e290(void* scene, void* part, I flag);
// [bsp_visibility] 00456E00 InitGrassBins(Vector min, Vector max, CExoArrayList<CExoArrayList<Bin*>>*): cdecl, RET
void __cdecl init_grass_bins_00456e00(Vector minimum, Vector maximum, PointerList* bins);
// [bsp_visibility] 004A67B0 CAurTriangleBin::FinishGeometry: thiscall, RET 0
void __fastcall triangle_bin_finish_geometry_004a67b0(void* bin, void* unused_edx);

// ---------------------------------------------------------------- authored here
PointerList* __fastcall vector_list_construct_00421fc0(PointerList* list, void* unused_edx, I size); // RET 4
void* __fastcall scene_get_object_0044fdb0(void* scene, void* unused_edx, const char* name);        // vtbl+0C, RET 4
void* __fastcall aur_room_construct_004548a0(void* room, void* unused_edx, const char* name);       // RET 4
void* __fastcall scene_spawn_room_00456f30(void* scene, void* unused_edx, const char* name,
    const Vector* position, const Quaternion* orientation);                                        // vtbl+70, RET 0Ch
void __fastcall scene_set_scene_focus_00458ba0(void* scene, void* unused_edx, Vector focus);        // vtbl+48, RET 0Ch
}

namespace {
using namespace kotor_scene3d;

template<class Fn> inline Fn slot(void* object, unsigned offset) {
    void** table = *static_cast<void** volatile*>(object);
    return reinterpret_cast<Fn>(table[offset / 4]);
}
using SlotQuery = void* (__fastcall*)(void*, void*);                                   // Part vtbl+24/2C/30/34/48 (RET 0)
using SlotAddPart = void (__fastcall*)(void*, void*, void*);                           // [scene+80] vtbl+0C (RET 4)
using SlotDeleting = void (__fastcall*)(void*, void*, U);                              // vtbl+00 scalar deleting dtor (RET 4)
using SlotGrass = void (__fastcall*)(void*, void*, void*, Vector*, PointerList*);      // Scene vtbl+128 (RET 0Ch)

inline void** elements(const PointerList* list) {
    return reinterpret_cast<void**>(static_cast<std::uintptr_t>(list->data));
}
inline Vector vector_bits(U x, U y, U z) { return Vector{bits_to_float(x), bits_to_float(y), bits_to_float(z)}; }

// CExoArrayList<T*>::Add inlined with the M41 growth owner 005E03C0 (sites 0045734C..0045737C, 00457382..004573B2,
// 0045747F..004574A4, 004574CF..004574F9): grow to capacity*2 (8 when 0) only when size == capacity.
inline void add_with_growth(PointerList* list, void* value) {
    const I capacity = list->capacity;
    if (list->size == capacity)
        menu_list_growth::allocate_005e03c0(list, nullptr, static_cast<U>(capacity != 0 ? capacity + capacity : 8));
    elements(list)[list->size] = value;
    list->size = list->size + 1;
}

// CExoArrayList<T*>::Add fully inlined with operator new / copy / _free (sites 004575FE..00457660 on nodedata+60,
// 004576C2..0045774B on Scene+C8): new capacity stored first, then allocation, element copy (size re-read per
// iteration), old buffer freed when non-null.
inline void add_inline_growth(PointerList* list, void* value) {
    const I capacity = list->capacity;
    if (list->size == capacity) {
        const I grown = capacity != 0 ? capacity + capacity : 8;          // 0045760A..00457612 / 004576D0..004576D8
        void** previous = elements(list);                                  // 00457617 / 004576E1
        list->capacity = grown;                                            // 0045761A / 004576E7
        void* fresh = live_operator_new(static_cast<U>(grown) << 2);       // 0045761D..00457621 / 004576ED..004576F1
        list->data = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(fresh)); // 00457629 / 004576FC
        for (I index = 0; index < list->size; ++index)                     // 0045762F..00457644 / 00457705..00457725
            elements(list)[index] = previous[index];
        if (previous != nullptr) live_free(previous);                      // 00457646..0045764B / 00457727..0045772C
    }
    elements(list)[list->size] = value;                                    // 0045765D / 00457748
    list->size = list->size + 1;                                           // 00457660 / 0045774B
}

constexpr char kEmpty_0073d71c[] = "";      // 00457088 push 73D71Ch
constexpr char kFormat_0073d5d0[] = "%s";   // 0045710B push 73D5D0h
constexpr char kSuffix_0073d5d4[] = "a";    // 0045704A mov ax,word ptr [0073D5D4] (0x0061)
} // namespace

extern "C" {

// 00421FC0 CExoArrayList<Vector>::CExoArrayList(int size), RET 4.
PointerList* __fastcall vector_list_construct_00421fc0(PointerList* list, void*, I size) {
    list->size = 0;                                                        // 00421FCB [esi+4]
    list->capacity = 0;                                                    // 00421FCE [esi+8]
    list->data = 0;                                                        // 00421FD1 [esi]
    if (size != 0) vector_list_allocate_00421310(list, nullptr, size);     // 00421FC9 cmp/je, 00421FD5..00421FD8
    return list;                                                           // 00421FDD mov eax,esi
}

// 0044FDB0 Scene::GetObjectA(name) (CAurScene vtbl+0C), RET 4: linear search of the Scene+54 object list
// (count Scene+58 re-read every iteration, signed) for the first object whose name at +38 compares equal with
// 0070ACAF(name, object+38); returns that element or 0.
void* __fastcall scene_get_object_0044fdb0(void* scene, void*, const char* name) {
    for (I index = 0; index < field<I>(scene, 0x58); ++index) {           // 0044FDB5..0044FDBC, 0044FDD9..0044FDDF
        char* object = static_cast<char*>(field<void**>(scene, 0x54)[index]); // 0044FDC2..0044FDC5
        if (kotor_stricmp_0070acaf(name, object + 0x38) == 0)              // 0044FDC8..0044FDD7
            return field<void**>(scene, 0x54)[index];                      // 0044FDE9..0044FDEC (reloaded)
    }
    return nullptr;                                                        // 0044FDE3
}

// 004548A0 CAurRoom::CAurRoom(const char* name), RET 4 (object size 0xB8 from 00456F81).
void* __fastcall aur_room_construct_004548a0(void* room, void*, const char* name) {
    bsp_node_construct_004ad630(room, nullptr, vector_bits(0, 0, 0), 0);  // 004548BA..004548F2 (three zero DWORDs + push 0)
    field<U>(room, 0x60) = 0;                                              // 004548F7
    field<U>(room, 0x64) = 0;                                              // 004548FA
    field<U>(room, 0x5C) = 0;                                              // 004548FD
    field<U>(room, 0x6C) = 0;                                              // 00454907
    field<U>(room, 0x70) = 0;                                              // 0045490A
    field<U>(room, 0x68) = 0;                                              // 0045490D
    char* target = static_cast<char*>(at(room, 0x78));                     // 00454904 lea edx,[esi+78h]
    const char* source = name;                                             // 00454900 [esp+24h]
    char c;
    do { c = *source++; *target++ = c; } while (c != 0);                   // 00454910..0045491A (inline strcpy)
    field<U>(room, 0x74) = 0;                                              // 00454920
    return room;                                                           // 00454923 mov eax,esi
}

// 00458BA0 Scene::SetSceneFocus(Vector) (CAurScene vtbl+48), RET 0Ch: three raw DWORD stores to Scene+A8.
void __fastcall scene_set_scene_focus_00458ba0(void* scene, void*, Vector focus) {
    std::memcpy(at(scene, 0xA8), &focus, sizeof(Vector));                  // 00458BA0..00458BB7
}

// 00456F30 Scene::SpawnRoom(name, &position, &orientation) (CAurScene vtbl+70), RET 0Ch; EAX = Gob or 0.
void* __fastcall scene_spawn_room_00456f30(void* scene, void*, const char* name, const Vector* position,
                                          const Quaternion* orientation) {
    aur_texture_set_mode_static_004204b0();                                // 00456F53
    aurora_model::MaxTree* const model = aurora_find_model_00464110(name); // 00456F5F..00456F6E ([esp+30])
    if (model == nullptr) return nullptr;                                  // 00456F72..00456F76 (xor eax,eax)
    field<U8>(model, 0x51) = static_cast<U8>(field<U8>(model, 0x51) | 1); // 00456F7B
    aurora_model::Gob* gob = nullptr;                                      // 00456F7F / 00456F86 ([esp+6C] = 0)

    void* room = live_operator_new(0xB8);                                  // 00456F81..00456F8A (EH state 0)
    if (room != nullptr) room = aur_room_construct_004548a0(room, nullptr, name); // 00456F9F..00456FA9
    // else room = 0 (00456FAF)
    void* nodedata = live_operator_new(0x98);                              // 00456FB6..00456FC2 (EH state 1)
    if (nodedata != nullptr) nodedata = nodedata_construct_0046bfb0(nodedata, nullptr); // 00456FDB..00456FE4
    // else esi stays 0 (00456F67)

    bool grass_bounds = false;                                             // 00456FF7 byte [esp+13] = 0
    Vector minimum = vector_bits(0x49742400u, 0x49742400u, 0x49742400u);  // 00456FFC..0045700C [esp+40] (1.0e6f)
    Vector maximum = vector_bits(0xC9742400u, 0xC9742400u, 0xC9742400u);  // 00457014..00457024 [esp+4C] (-1.0e6f)

    char node_name[0x40];                                                  // [esp+B8] (64 bytes up to the EH record at [esp+F8])
    {
        const char* source = static_cast<const char*>(at(model, 0x08));   // 00456FE6 lea eax,[edi+8]
        char* target = node_name;
        char c;
        do { c = *source++; *target++ = c; } while (c != 0);               // 0045702C..00457038 (inline strcpy)
        char* end = node_name;
        while (*end != 0) ++end;                                           // 0045703A..00457048 (inline strcat scan)
        std::memcpy(end, kSuffix_0073d5d4, 2);                             // 0045704A..0045705B word store "a\0"
    }

    aurora_model::Part* tree;                                              // [esp+20]
    if (find_node_00452000(field<aurora_model::MdlNode*>(model, 0x28), node_name) != nullptr) { // 0045705E..0045707D
        gob = aurora_new_aur_object_00449cc0(name, kEmpty_0073d71c, nullptr, nullptr); // 0045707B..00457093
        tree = field<aurora_model::Part*>(gob, 0x6C);                      // 00457095..00457098
        std::memcpy(at(gob, 0x78), position, 12);                          // 0045709C..004570AE (3 raw DWORDs)
        std::memcpy(at(gob, 0x84), orientation, 16);                       // 004570B1..004570DA (4 raw DWORDs)
        gob_set_scene_0043f200(gob, nullptr, scene);                       // 004570C9..004570DD
        field<aurora_model::Gob*>(room, 0x74) = gob;                       // 004570E2..004570E6
    } else {
        tree = aurora_create_instance_tree_00449410(field<aurora_model::MdlNode*>(model, 0x28), nullptr); // 0045707B, 004570EB..004570F3
        char prefix[0x20];                                                 // [esp+98] (32 bytes up to [esp+B8])
        std::sprintf(prefix, kFormat_0073d5d0, name);                      // 004570F8..00457111 (006FADB0)
        aurora_part_init_material_00442d70(nullptr, tree, prefix);         // 00457116..00457125
    }

    PointerList parts;                                                     // [esp+58] (EH state 2)
    parts.size = 0; parts.capacity = 0; parts.data = 0;                    // 0045712F..00457137
    PointerList grass_parts;                                               // [esp+34] (EH state 2)
    grass_parts.size = 0; grass_parts.capacity = 0; grass_parts.data = 0;  // 00457146..0045714E
    aurora_collapse_part_tree_00453f10(gob, tree, &parts, *position, *orientation, scene, room); // 00457152..0045719F (EH state 3)

    field<U>(room, 0x28) = 0xC7C35000u;                                    // 004571A8..004571BE (-1.0e5f)
    field<U>(room, 0x2C) = 0xC7C35000u;                                    // 004571C0..004571CC
    field<U>(room, 0x30) = 0xC7C35000u;                                    // 004571CF..004571DB
    field<U>(room, 0x1C) = 0x47C35000u;                                    // 004571DE..004571F4 (1.0e5f)
    field<U>(room, 0x20) = 0x47C35000u;                                    // 004571F6..0045720E
    field<U>(room, 0x24) = 0x47C35000u;                                    // 00457202..00457211
    Vector* const room_maximum = static_cast<Vector*>(at(room, 0x28));     // [esp+68]
    Vector* const room_minimum = static_cast<Vector*>(at(room, 0x1C));     // [esp+64]

    if (parts.size > 0) {                                                  // 00457214..00457228 (jle 004576B7)
        for (I index = 0; index < parts.size; ++index) {                   // [esp+20]; 0045751C..0045752B (size re-read)
            // Local names light/emitter/walk are INFERRED from what the original does with each slot result
            // (vtbl+24 -> [Scene+80] vtbl+0C + nodedata+0 + SceneBSPAddPart; vtbl+34 -> nodedata+24; vtbl+48 -> grass bins).
            void* part = elements(&parts)[index];
            void* const trimesh = slot<SlotQuery>(part, 0x2C)(part, nullptr); // 00457238..0045723D
            part = elements(&parts)[index];
            void* const light = slot<SlotQuery>(part, 0x24)(part, nullptr);   // 00457240..00457247 ([esp+70])
            part = elements(&parts)[index];
            void* const emitter = slot<SlotQuery>(part, 0x34)(part, nullptr); // 0045724A..00457253 ([esp+2C])
            part = elements(&parts)[index];
            void* const walk = slot<SlotQuery>(part, 0x48)(part, nullptr);    // 00457256..0045725F (edi)
            if (trimesh != nullptr) {                                      // 00457262..00457266
                if (slot<SlotQuery>(trimesh, 0x30)(trimesh, nullptr) == nullptr) { // 0045726C..00457276
                    compute_part_bbox_00451dc0(trimesh);                   // 0045727C..0045727D
                    aurora_model::MdlNode* const mesh =
                        aurora_node_as_trimesh_0043e400(field<aurora_model::MdlNode*>(trimesh, 0x04), nullptr); // 00457282..00457288
                    if (field<U8>(mesh, 0x186) == 0) {                     // 0045728D..00457295
                        Vector out;
                        const Vector* grown = vector_max_004aaee0(&out, *room_maximum, field<Vector>(trimesh, 0x58)); // 0045729B..004572D3
                        std::memcpy(room_maximum, grown, 12);              // 004572D8..004572E5
                        Vector out2;
                        const Vector* shrunk = vector_min_004aae70(&out2, *room_minimum, field<Vector>(trimesh, 0x4C)); // 004572E8..00457326
                        std::memcpy(room_minimum, shrunk, 12);             // 0045732B..0045733B
                    }
                }
                model_add_ref_0043e240(model, nullptr, nullptr);       // 0045733E..00457344
                add_with_growth(static_cast<PointerList*>(at(nodedata, 0x18)), trimesh); // 00457349..0045737C
                if (walk != nullptr) {                                     // 00457377..0045737C
                    field<void*>(nodedata, 0x6C) = walk;                   // 0045738A
                    add_with_growth(&grass_parts, walk);                   // 00457382..004573B2
                    Vector out;
                    const Vector* grown = vector_max_004aaee0(&out, maximum, field<Vector>(walk, 0x58)); // 004573B6..004573EF
                    std::memcpy(&maximum, grown, 12);                      // 004573F4..00457407
                    Vector out2;
                    const Vector* shrunk = vector_min_004aae70(&out2, minimum, field<Vector>(walk, 0x4C)); // 0045740B..00457441
                    std::memcpy(&minimum, shrunk, 12);                     // 00457446..00457459
                    grass_bounds = true;                                   // 0045745D
                }
            } else if (light != nullptr) {                                 // 00457467..0045746D
                void* const lights = field<void*>(scene, 0x80);            // 0045746F..00457473
                slot<SlotAddPart>(lights, 0x0C)(lights, nullptr, light);   // 00457479..0045747C
                add_with_growth(static_cast<PointerList*>(nodedata), light); // 0045747F..004574A4 (nodedata+0)
                if (field<U>(light, 0x70) != 0)                            // 004574A7..004574AC
                    scene_bsp_add_part_0046e290(scene, light, 0);          // 004574AE..004574B7
                model_add_ref_0043e240(model, nullptr, nullptr);       // 004574BA..004574C0
            } else if (emitter != nullptr) {                               // 004574C7..004574CD
                add_with_growth(static_cast<PointerList*>(at(nodedata, 0x24)), emitter); // 004574CF..004574F9
                model_add_ref_0043e240(model, nullptr, nullptr);       // 004574FC..00457502
            } else {
                void* const unused = elements(&parts)[index];              // 00457509..0045750D
                if (unused != nullptr) slot<SlotDeleting>(unused, 0x00)(unused, nullptr, 1u); // 00457510..0045751A
            }
        }

        if (grass_parts.size > 0) {                                        // 00457531..00457537
            PointerList bins;                                              // [esp+14] (EH state 4)
            bins.size = 0; bins.capacity = 0; bins.data = 0;               // 0045753D..00457547
            init_grass_bins_00456e00(minimum, maximum, &bins);             // 0045754B..0045758A
            const I walk_count = grass_parts.size;                         // 0045758F (read once)
            for (I index = 0; index < walk_count; ++index)                 // 00457596..004575BF
                slot<SlotGrass>(scene, 0x128)(scene, nullptr, elements(&grass_parts)[index], &minimum, &bins); // 004575A0..004575B6
            for (I outer = 0; outer < bins.size; ++outer) {                // [esp+20]; 004575C3..004575CF, 00457674..00457686 (size re-read)
                PointerList* const row = reinterpret_cast<PointerList*>(elements(&bins)) + outer; // [eax+ebp], ebp = outer*0Ch
                for (I inner = 0; inner < row->size; ++inner) {            // 004575D7..004575DF, 00457667..0045766E (size re-read)
                    void* bin = elements(row)[inner];                      // 004575E5..004575E8
                    if (bin != nullptr) {                                  // 004575EB..004575ED
                        triangle_bin_finish_geometry_004a67b0(bin, nullptr); // 004575EF
                        bin = elements(row)[inner];                        // 004575F4..004575FB (reloaded)
                        add_inline_growth(static_cast<PointerList*>(at(nodedata, 0x60)), bin); // 004575FE..00457660
                    }
                }
            }
            if (bins.data != 0) {                                          // 0045768E..00457698 (EH state back to 3)
                char* const first = reinterpret_cast<char*>(elements(&bins));
                // delete[]: element count stored in the DWORD before the array (0045769A..0045769D)
                c2da_destroy_array_006fa852(first, 0x0Cu, *reinterpret_cast<I*>(first - 4), exo_array_list_free_0047abc0); // 004576A0..004576A9
                live_free(first - 4);                                      // 004576AE..004576AF
            }
        }
    }

    field<void*>(room, 0x58) = nodedata;                                   // 004576B7..004576BF
    add_inline_growth(static_cast<PointerList*>(at(scene, 0xC8)), room);   // 004576C2..0045774B (Scene+CC incremented)
    aur_texture_set_mode_dynamic_004204c0();                               // 00457751
    if (!grass_bounds) {                                                   // 00457756..0045775C
        field<U>(room, 0x28) = 0xC7C35000u;                                // 0045775E..0045776E (-1.0e5f; observed: resets the
        field<U>(room, 0x2C) = 0xC7C35000u;                                // 00457770..00457788  trimesh-accumulated bounds when no
        field<U>(room, 0x30) = 0xC7C35000u;                                // 0045777C..0045778B  vtbl+48 part was found)
        field<U>(room, 0x1C) = 0x47C35000u;                                // 0045778E..0045779E (1.0e5f)
        field<U>(room, 0x20) = 0x47C35000u;                                // 004577A0..004577B8
        field<U>(room, 0x24) = 0x47C35000u;                                // 004577AC..004577BB
    }
    if (grass_parts.data != 0) live_free(elements(&grass_parts));          // 004577BE..004577CF (EH state 2 -> -1)
    if (parts.data != 0) live_free(elements(&parts));                      // 004577D7..004577EB
    return gob;                                                            // 004577F3 mov eax,[esp+6Ch]
}

} // extern "C"
