// lane-m46-3dcore fork E: tile / room-spawn part helpers and the dynamic-shadow projector of the 3D core
// (swkotor.exe GOG K1, image base 00400000). Every statement cites the original instruction(s) (d/<addr>.txt).
//
// Recursion: findnode 00452000 and SetPartTextureRotate 00452500 are recursive in the source; VC6 inlined eight
// levels of the recursion (00452050..004524E6 / 00452550..004528A0) before the self call (004523EC / 004527CC).
// The unrolled levels execute the same per-node statements in the same pre-order with the same child-count
// re-reads, so the C++ bodies below are written as the recursion itself (identical calls, order and result).
//
// Dispatch (D21/D41): Gob table through the published aurora_model::GobVTable (vtables_gen.h); Part-family
// tables through the lane-3d-model generated typed tables (model_vtables.hpp): VTable_00740ae8 (Part) for slots
// +00/+24/+2C/+34 and VTable_00740b68 (PartTriMesh) for slots +30/+48/+7C; every Part-family table types these
// slots identically (Slot1 at +00/+7C, Slot0 at +24/+2C/+30/+34/+48), so the view is the observed slot type.
// D30-FINAL: x87 values kept wide (double) and rounded only at the original `fstp dword` stores.
#include <cmath>
#include <cstdint>
#include <cstring>
#include "558510f9c9-tile_scene.h"          // lane-3d-bsp consumer declarations (bsp.h -> scene3d.h -> vtables_gen.h)
#include "7ee14b5ec3-model_vtables.hpp"     // lane-3d-model typed Part-family tables (aurora_vt)
#include "e04ccf8d50-m46_tile_decls.h"

namespace {
using kotor_scene3d::U;
using kotor_scene3d::I;
using kotor_scene3d::U8;
using kotor_scene3d::Vector;
using kotor_scene3d::Quaternion;
using kotor_scene3d::Plane;
using kotor_scene3d::PointerList;
using kotor_scene3d::field;
using kotor_scene3d::at;
using kotor_scene3d::bits_to_float;

inline double W(float v) { return static_cast<double>(v); }
inline float S(double v) { return static_cast<float>(v); }
inline U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline U float_bits(float v) { U bits; std::memcpy(&bits, &v, 4); return bits; }
inline void** elements(const PointerList* list) { return reinterpret_cast<void**>(static_cast<std::uintptr_t>(list->data)); }

inline const aurora_vt::VTable_00740ae8* part_vt(void* part) {
    return *static_cast<const aurora_vt::VTable_00740ae8* const volatile*>(part);
}
inline const aurora_vt::VTable_00740b68* trimesh_vt(void* trimesh) {
    return *static_cast<const aurora_vt::VTable_00740b68* const volatile*>(trimesh);
}
inline const aurora_model::GobVTable* gob_vt(void* gob) {
    return *static_cast<const aurora_model::GobVTable* const volatile*>(gob);
}
inline void* as_pointer(U value) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(value)); }

constexpr U kRotateStep_0073e40c = 0x42B40000;   // .rdata 0073E40C = 90.0f
constexpr U kHalf_0073e9ac = 0x3F000000;         // .rdata 0073E9AC = 0.5f
constexpr U kMilli = 0x3A83126F;                 // immediate push 3A83126Fh = 0.001f (00451E9D / 00451ECC / 00451EDF)
constexpr U kBoxInit = 0x47C35000;               // 100000.0f (00451DEF..00451DFF)
constexpr U kBoxInitNeg = 0xC7C35000;            // -100000.0f (00451DD7..00451DE7)
constexpr U kThousand_0073d6fc = 0x447A0000;     // .rdata 0073D6FC = 1000.0f
constexpr U kZero_0073d700 = 0x00000000;         // .rdata 0073D700 = 0.0f
constexpr U kLift_007415bc = 0x3D4CCCCD;         // .rdata 007415BC = 0.05f
constexpr U kRange_0044fb1a = 0x461C3C00;        // immediate push 461C3C00h = 10000.0f (0044FB1A)
// .rdata 007415A8 "AnimateStaticLight" (6D696E41 53657461 69746174 67694C63 00007468), pushed at 0044FB21.
constexpr char kAnimateStaticLight_007415a8[] = "AnimateStaticLight";

// Gob::PartVolumeProject 00474710 (__thiscall, RET 24h): part, Vector position (by value), float range, Plane
// (16 bytes by value). Owner: mesh area (no provider in M45); new external in this packet.
extern "C" void __fastcall gob_part_volume_project_00474710(void* gob, void* unused_edx, void* part, Vector position,
                                                             float range, Plane plane);
} // namespace

extern "C" {

volatile std::uint32_t project_shadow_gate_007b9374 = 0;   // BSS 007B9374 (no writer instruction in the image scan)

// 004A96E0 Round(value, precision): fld value; fdiv precision; fadd 0.5; floor (006FC750); fmul precision.
// ST0 result. PC_53 (CW 027F, D30-FINAL): fdiv/fadd/fmul round to double exactly as the double operations below;
// CRT floor 006FC750 is exact on finite doubles (equivalence: std::floor; NaN/inf inputs not reachable from the
// part-vertex callers - recorded in review_needed).
double __cdecl round_004a96e0(float value, float precision) {
    const double scaled = W(value) / W(precision) + W(bits_to_float(kHalf_0073e9ac));   // 004A96E0..004A96EB
    return std::floor(scaled) * W(precision);                          // 004A96F1..004A96F9
}

// 0046DDF0 AnimateStaticLight(gob, event, part): UpdateStaticLighting([gob+68h], part).
void __cdecl animate_static_light_0046ddf0(void* gob, void*, aurora_model::Part* part) {
    update_static_lighting_0046ca30(field<void*>(gob, 0x68), part);    // 0046DDF0..0046DDFD
}

// 00452000 findnode(node, name): pre-order search over MdlNode children (+2C data, +30 count) by
// CAurNodeTranslation name (00438A00, inlined at the top level 00452004..0045202A with the same two 0043E1C0
// branches); string compare = inline strcmp equality (00452050..00452079).
void* __cdecl find_node_00452000(aurora_model::MdlNode* node, const char* name) {
    if (std::strcmp(aurora_node_translate_name_00438a00(node, nullptr), name) == 0)   // 00452004..0045207B
        return node;                                                   // 0045207E mov eax,ebx
    void* found = nullptr;                                             // 0045208B xor eax,eax
    for (I i = 0; i < field<I>(node, 0x30); ++i) {                     // 00452085..00452093 / 004524DC..004524E6 (count re-read)
        aurora_model::MdlNode* const child =
            static_cast<aurora_model::MdlNode*>(elements(static_cast<const PointerList*>(at(node, 0x2C)))[i]);   // 004520A0..004520A3
        found = find_node_00452000(child, name);                       // unrolled levels / 004523EC
        if (found != nullptr) break;                                   // 004524D8 test eax,eax; jne
    }
    return found;
}

// 00452500 SetPartTextureRotate(part, rotation): for every part in the subtree (children +30 data, +34 count),
// trimesh = part->[+2C](); if trimesh and node byte [[trimesh+4]+185h] != 0: trimesh->[+7C](rotation * 90.0f).
void __cdecl set_part_texture_rotate_00452500(aurora_model::Part* tree, I rotation) {
    void* const trimesh = as_pointer(part_vt(tree)->s02c(tree, nullptr));   // 00452508..0045250D call [eax+2Ch]
    if (trimesh != nullptr) {                                          // 00452510..00452512
        void* const node = field<void*>(trimesh, 0x04);                // 00452514 ecx = [eax+4]
        if (field<U8>(node, 0x185) != 0) {                             // 00452517..0045251F
            const float angle = S(static_cast<double>(rotation) * W(bits_to_float(kRotateStep_0073e40c)));   // 00452521..00452530 fild; fmul; fstp dword
            trimesh_vt(trimesh)->s07c(trimesh, nullptr, float_bits(angle));   // 00452525..00452533 call [edx+7Ch]
        }
    }
    for (I i = 0; i < field<I>(tree, 0x34); ++i)                       // 00452536..00452541 / 00452894..0045289E (count re-read)
        set_part_texture_rotate_00452500(
            static_cast<aurora_model::Part*>(elements(static_cast<const PointerList*>(at(tree, 0x30)))[i]), rotation);   // 00452550..00452553 / 004527CC
}

// 00451DC0 ComputePartBBox(part): min/max of the node's pool vertices transformed by the part orientation
// (+14h) and position (+08h), each coordinate rounded to 0.001 (Round 004A96E0), stored at part+4C (min) and
// part+58 (max). Initial box (+1e5,+1e5,+1e5)/(-1e5,-1e5,-1e5) (00451DD7..00451DFF).
void __cdecl compute_part_bbox_00451dc0(void* part) {
    void* const node = field<void*>(field<void*>(part, 0x70), 0x04);    // 00451DCE..00451DD1 esi = [[part+70h]+4]
    Vector minimum{bits_to_float(kBoxInit), bits_to_float(kBoxInit), bits_to_float(kBoxInit)};          // [esp+1C..24]
    Vector maximum{bits_to_float(kBoxInitNeg), bits_to_float(kBoxInitNeg), bits_to_float(kBoxInitNeg)}; // [esp+28..30]
    void* const model = aurora_tree_as_model_0043e1c0(field<aurora_model::MaxTree*>(node, 0x08), nullptr);   // 00451DD4..00451E07
    const U base = word(aurora_pool_lock_00423f10(field<U>(model, 0xAC), 0, 0, 8));   // 00451E0C..00451E19 (handle,0,0,8)
    const U data_offset = field<U>(node, 0x194);                       // 00451E1E..00451E38
    for (I i = 0; i < static_cast<I>(field<std::int16_t>(node, 0x180)); ++i) {   // 00451E2D..00451E3F / 00451F91..00451FA6 (movsx, re-read)
        const U vertex = field<U>(node, 0x14C) * static_cast<U>(i) + data_offset + base + field<U>(node, 0x154);   // 00451E50..00451E71
        Vector local;
        std::memcpy(&local.x, as_pointer(vertex), 4);                  // 00451E6E / 00451E8F (raw dword copy)
        local.y = *static_cast<const float*>(as_pointer(vertex + 4));  // 00451E7A fld; 00451E82 fstp dword
        local.z = *static_cast<const float*>(as_pointer(vertex + 8));  // 00451E77 fld; 00451E8A fstp dword
        Vector rotated;
        const Vector* const r = quaternion_rotate_004a9bb0(&rotated, static_cast<const Quaternion*>(at(part, 0x14)), &local);   // 00451E73..00451E93
        Vector world;
        world.x = S(W(r->x) + W(field<float>(part, 0x08)));            // 00451E98..00451EA2
        world.y = S(W(r->y) + W(field<float>(part, 0x0C)));            // 00451EA6..00451EAC
        world.z = S(W(r->z) + W(field<float>(part, 0x10)));            // 00451EB0..00451EB6
        Vector snapped;
        snapped.z = S(round_004a96e0(world.z, bits_to_float(kMilli))); // 00451EBA..00451EC4 (first call)
        snapped.y = S(round_004a96e0(world.y, bits_to_float(kMilli))); // 00451EC8..00451ED7
        snapped.x = S(round_004a96e0(world.x, bits_to_float(kMilli))); // 00451EDB..00451EEA
        Vector result;
        const Vector* const lo = vector_min_004aae70(&result, minimum, snapped);   // 00451EEE..00451F35 (a = box min, b = vertex)
        minimum = *lo;                                                 // 00451F3A..00451F53
        const Vector* const hi = vector_max_004aaee0(&result, maximum, snapped);   // 00451F4D..00451F84
        maximum = *hi;                                                 // 00451F89..00451FA2 (y kept in ebp)
    }
    aurora_pool_unlock_00423f30(field<U>(aurora_tree_as_model_0043e1c0(field<aurora_model::MaxTree*>(node, 0x08), nullptr), 0xAC));   // 00451FAC..00451FBB
    field<Vector>(part, 0x4C) = minimum;                               // 00451FC0..00451FE3
    field<Vector>(part, 0x58) = maximum;                               // 00451FDB..00451FEA
}

// 0044FAC0 ProcessStaticObject(gob, scene, tile, position, orientation).
void __cdecl process_static_object_0044fac0(void* gob, void* scene, void* tile, const Vector* position,
                                            const Quaternion* orientation) {
    field<void*>(tile, 0x24) = gob;                                    // 0044FAC0..0044FACD
    field<Vector>(gob, 0x78) = *position;                              // 0044FAD1..0044FAE1 (3 dword copies)
    field<Quaternion>(gob, 0x84) = *orientation;                       // 0044FAE4..0044FB0B (4 dword copies)
    field<void*>(gob, 0x170) = tile;                                   // 0044FB01
    gob_set_scene_0043f200(gob, nullptr, scene);                       // 0044FB07..0044FB11
    gob_vt(gob)->register_callback(static_cast<aurora_model::Gob*>(gob), nullptr, &animate_static_light_0046ddf0,
        kAnimateStaticLight_007415a8, nullptr, kRange_0044fb1a, 0);    // 0044FB16..0044FB2D call [edx+28h]
}

// 00454B80 ProcessStaticParts(parts, scene, tile, name, flag): classify each collapsed part; kept parts are
// appended to the tile list at tile+18h, the others deleted through the scalar deleting destructor (flag 1).
// `scene` is not read.
void __cdecl process_static_parts_00454b80(PointerList* parts, void*, void* tile, const char* name, U flag) {
    for (I index = 0; index < parts->size; ++index) {                  // 00454B85..00454B92 / 00454C00..00454C12 (size re-read)
        void* const part = elements(parts)[index];                     // 00454BA0..00454BAA
        void* const trimesh = as_pointer(part_vt(part)->s02c(part, nullptr));   // 00454BAD..00454BB4
        const bool tile_part = process_tile_part_004528b0(tile, part, name);   // 00454BB6..00454BC7 bl = al
        bool keep;
        if (trimesh != nullptr) {                                      // 00454BC5..00454BC9
            field<void*>(trimesh, 0x80) = tile;                        // 00454BCF
            if (trimesh_vt(trimesh)->s030(trimesh, nullptr) == 0)      // 00454BD5..00454BDA
                compute_part_bbox_00451dc0(trimesh);                   // 00454BDC..00454BE2
            keep = flag == 0 || trimesh_vt(trimesh)->s048(trimesh, nullptr) == 0;   // 00454BE5..00454BF6
        } else {
            void* const reference = as_pointer(part_vt(part)->s034(part, nullptr));   // 00454C1A..00454C1E
            if (flag != 0) {                                           // 00454C21..00454C27
                keep = false;
            } else if (tile_part) {                                    // 00454C29..00454C2B
                if (reference != nullptr) field<void*>(reference, 0x1EC) = tile;   // 00454C3E..00454C42
                keep = true;
            } else if (reference != nullptr) {                         // 00454C2D..00454C2F
                field<void*>(reference, 0x1EC) = tile;                 // 00454C42
                keep = true;
            } else {
                keep = part_vt(part)->s024(part, nullptr) != 0;        // 00454C31..00454C3A
            }
        }
        if (!keep) {
            part_vt(part)->s000(part, nullptr, 1);                     // 00454BF8..00454BFE push 1; call [eax]
            continue;
        }
        PointerList* const list = static_cast<PointerList*>(at(tile, 0x18));   // 00454C50 lea edi,[ebp+18h]
        if (list->size == list->capacity)                              // 00454C48..00454C53
            menu_list_growth::allocate_005e03c0(list, nullptr,
                static_cast<U>(list->capacity != 0 ? list->capacity + list->capacity : 8));   // 00454C55..00454C65
        elements(list)[list->size] = part;                             // 00454C6A..00454C6F
        list->size = list->size + 1;                                   // 00454C72
    }
}

// 0044FF50 ProjectDynamicShadow(gob, light): projection plane chosen from the light height or the current camera
// frustum plane (camera+1F8 data, row 1 at +10h, distance lifted by 0.05), then Gob::PartVolumeProject on the
// root part [gob+6Ch]. With 007B9374 != 0 the plane is all zero and nothing is allocated.
void __cdecl project_dynamic_shadow_0044ff50(void* gob, void* light) {
    if (project_shadow_gate_007b9374 == 0) {                           // 0044FF50..0044FF5C
        Plane* plane;
        if (W(field<float>(light, 0x58)) > W(bits_to_float(kThousand_0073d6fc))) {   // 0044FF66..0044FF76 fcomp; test ah,41h
            plane = static_cast<Plane*>(live_operator_new(0x10));      // 0044FF69 push 10h; 0044FF78 call 006FA7E6
            if (plane != nullptr) {                                    // 0044FF80..0044FF82
                plane->a = 0.0f; plane->b = 0.0f;                      // 0044FF84..0044FFA0
                plane->c = bits_to_float(0xBF800000u);                 // 0044FFA3..0044FFAF (-1.0f)
                plane->d = bits_to_float(0xBDCCCCCDu);                 // 0044FFB2 (-0.1f)
            }
        } else {
            void* const camera = *kotor_scene3d::cells.current_camera_007bb4e8;   // 0044FFBB mov eax,[007BB4E8]
            const Plane* const row = reinterpret_cast<const Plane*>(field<unsigned char*>(camera, 0x1F8) + 0x10);   // 0044FFC0..0044FFC6
            plane = static_cast<Plane*>(live_operator_new(0x10));      // 0044FFC9 (push 10h of 0044FF69)
            if (plane != nullptr) {                                    // 0044FFD1..0044FFD3
                plane->a = row->a; plane->b = row->b;                  // 0044FFD5..0044FFE7
                plane->d = S(W(row->d) + W(bits_to_float(kLift_007415bc)));   // 0044FFD7..0044FFED fld; fadd; fstp
                plane->c = row->c;                                     // 0044FFEA..0044FFF0
            }
        }
        float range = field<float>(light, 0x58);                       // 00450008..0045000F [esp+8]
        Vector position;
        position.x = field<float>(light, 0x94);                        // 0044FFFA / 00450019 (raw dword)
        position.y = field<float>(light, 0x98);                        // 00450000 fld; 0045000B fstp dword
        position.z = S(W(field<float>(light, 0x64)) + W(field<float>(light, 0x9C)));   // 0044FFF7 fld; 00450013 fadd; 0045001D fstp
        if (W(field<float>(light, 0x60)) > W(bits_to_float(kZero_0073d700)))   // 00450021..0045002F
            range = field<float>(light, 0x60);                         // 00450031..00450034
        gob_part_volume_project_00474710(gob, nullptr, field<void*>(gob, 0x6C), position, range, *plane);   // 00450038..0045007B (plane copied from [eax])
        live_free(plane);                                              // 00450080..00450081 call 006FA390
        return;
    }
    float range = field<float>(light, 0x58);                           // 00450096..004500A9
    Vector position;
    position.x = field<float>(light, 0x94);                            // 0045009F / 004500B3
    position.y = field<float>(light, 0x98);                            // 00450099 / 004500A5
    position.z = S(W(field<float>(light, 0x64)) + W(field<float>(light, 0x9C)));   // 00450093 / 004500AD / 004500B7
    if (W(field<float>(light, 0x60)) > W(bits_to_float(kZero_0073d700)))   // 004500BB..004500C9
        range = field<float>(light, 0x60);                             // 004500CB..004500CE
    const Plane zero{0.0f, 0.0f, 0.0f, 0.0f};                          // 004500D7..0045010F
    gob_part_volume_project_00474710(gob, nullptr, field<void*>(gob, 0x6C), position, range, zero);   // 00450112..00450138
}

} // extern "C"
