// W5 t3 (G18 server side): CSWSObject::AIActionOpenDoor 0057D490 and the callees it needs that have no
// implementation anywhere (reuse gate: build/1122 map, lane-w1 tier4b, lane-w2 t3b/t4, lane-w3, lane-w4).
// Transcribed from the original swkotor.exe (read-only); addresses in the comments are the original instructions.
#include "842d6b1618-w5_t3.hpp"
#include "5889fde5f1-w5_action_bodies.hpp"
#include "4f622be2f2-w5_area_direct_line.hpp"

using namespace w5;

extern "C" unsigned __fastcall recovered_object_get_reputation_0057cb80(void* self, void*, unsigned target, unsigned arg2); // 0057CB80 CSWSObject::GetReputation RET 8 (w1_native_b7)
extern "C" {
Vector* __fastcall w3_placeable_nearest_action_point_00584c20(void* placeable, void*, Vector* out, const Vector* from); // RET 8 [door_area_extension]
unsigned __fastcall recovered_trigger_nearest_point_0058c8a0(void* trigger, void*, void* from, void* out);             // RET 8 [w1_native_b7]
U __fastcall w5_trigger_in_trigger_0058ce40(void* trigger, void*, const Vector* p);                                    // RET 4 [w5_subarea]
U __fastcall kotor_fastcall_004cb4f0(void* object, void*);                                                            // StopDialog, RET 0 [dialogue D2]
}

namespace {
constexpr U k_closed_00002726 = 0x2726;           // door state "closed" (CSWSDoor +D4)
constexpr U k_action_complete_00745dc0 = 2;       // .rdata 00745DC0
constexpr U k_action_failed_00745dc4 = 3;         // .rdata 00745DC4
constexpr U k_ai_event_open_door_007441d0 = 7;    // .rdata 007441D0
constexpr U k_ai_event_script_007441dc = 10;      // .rdata 007441DC
constexpr U k_ai_event_unlocked_007441f4 = 16;    // .rdata 007441F4
constexpr U k_object_type_bound_00746633 = 4;     // .rdata 00746633 (byte): object type must be above it
constexpr U k_closed_mesh_0074c394 = 0;           // .rdata 0074C394..9C: CSWSDoor +32C mesh index per state
constexpr U k_open1_mesh_0074c398 = 1;
constexpr U k_open2_mesh_0074c39c = 2;
constexpr U k_half_0x3f000000 = 0x3f000000u;      // 0.5 immediate
constexpr U k_tenth_0x3dcccccd = 0x3dcccccdu;     // 0.1 immediate
constexpr U k_far_0xcb18967f = 0xcb18967fu;       // -9999999.0 immediate (unknown door state)
constexpr U k_1000_0x447a0000 = 0x447a0000u;      // 1000.0 immediate
constexpr U k_down_1000 = 0xc47a0000u;            // -1000.0 immediate
const float k_use_range_creature_00746d38 = 0.3f; // .rdata 00746D38
const float k_use_range_door_0073e31c = 0.75f;    // .rdata 0073E31C
const float k_in_range_slack_0073e798 = 0.1f;     // .rdata 0073E798
const float k_eye_height_0073f414 = 1.5f;         // .rdata 0073F414
const float k_use_range_placeable_00743264 = 5.0f; // .rdata 00743264
const float k_use_range_trigger_0073e9ac = 0.5f;  // .rdata 0073E9AC
const char k_walk_00747670[] = "Walk";            // .rdata 00747670

inline void* as_object(void* o) { return vcall<void*>(o, 0x10); }      // vtbl+10 AsSWSObject
inline void* as_door(void* o) { return vcall<void*>(o, 0x18); }        // vtbl+18 AsSWSDoor
inline void* as_creature(void* o) { return vcall<void*>(o, 0x30); }    // vtbl+30 AsSWSCreature
inline void* as_trigger(void* o) { return vcall<void*>(o, 0x40); }     // vtbl+40 AsSWSTrigger
inline void* as_placeable(void* o) { return vcall<void*>(o, 0x48); }   // vtbl+48 AsSWSPlaceable
inline U addr(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline Vector vec_of_bits(U x, U y, U z) { Vector v; std::memcpy(&v.x, &x, 4); std::memcpy(&v.y, &y, 4); std::memcpy(&v.z, &z, 4); return v; }
// CGameObjectArray::GetGameObject through the server object array (inline view of 004AED70); AL status 0 = found
inline bool lookup(U id, void** out) { return static_cast<unsigned char>(kotor_fastcall_004d8230(object_array(), nullptr, id, out)) == 0; }
// AddEventDeltaTime(0, 0, caller, target, type, event) on the server AI master (inline view of 004AED80)
inline void ai_event(U caller, U target, U type, void* event) {
    recovered_event_enqueue_004b08d0(ai_master(), nullptr, 0, 0, caller, target, type, event);
}
inline void* new_script_event(U16 type) {                                    // operator new(34h) + CScriptEvent ctor; type at +0
    void* memory = live_operator_new(0x34);
    void* event = memory ? recovered_event_construct_004d7540(memory, nullptr) : nullptr;
    at<U16>(event, 0) = type;
    return event;
}
}  // namespace

extern "C" {

// ---------------------------------------------------------------------------------------------------------
// 004CCB00 CSWSObject::AddActionToFront is W1's (ruling; lane-w1 tier6 @recovered_object_add_action_to_front_004ccb00@120).
// w5_add_action_to_front_body keeps the (type, group, pairs[26]) shape used below and calls W1's export.
void __cdecl w5_add_action_to_front_body(void* object, U type, U group_word, const U* q) {
    auto v = [&](int i) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(q[i])); };
    recovered_object_add_action_to_front_004ccb00(object, nullptr, type, group_word,
        q[0], v(1), q[2], v(3), q[4], v(5), q[6], v(7), q[8], v(9), q[10], v(11), q[12], v(13),
        q[14], v(15), q[16], v(17), q[18], v(19), q[20], v(21), q[22], v(23), q[24], v(25));
}

// ---------------------------------------------------------------------------------------------------------
// 004F87D0 CSWSCreature::ActivityManager(mode). +9FC = active activity bits, +A00 = bits to keep.
void __fastcall w5_creature_activity_manager_004f87d0(void* c, void*, U mode) {
    if (mode == 4) {                                                                // 004F87D6..DB -> 004F89C8
        U bits = at<U>(c, 0x9fc); U one = bits & 1;
        if (one != 0 && (at<U8>(c, 0xa00) & 1) == 0) {                             // 004F89C8..DC
            at<U>(c, 0x9fc) = bits & 0xfffffffeu;                                  // 004F89DE..E3
            recovered_set_stealth_mode_0050ee50(c, nullptr, 0, 0);                  // 004F89EB..F1
        }
        bits = at<U>(c, 0x9fc); U four = bits & 4;
        if (four != 0 && (at<U8>(c, 0xa00) & 4) == 0) {                            // 004F89F6..0A
            at<U>(c, 0x9fc) = bits & 0xfffffffbu;
            kotor_fastcall_004cb4f0(c, nullptr);                                    // 004F8A19..1B
        }
        bits = at<U>(c, 0x9fc);
        if ((bits & 8) != 0 && (at<U8>(c, 0xa00) & 8) == 0) at<U>(c, 0x9fc) = bits & 0xfffffff7u; // 004F8A20..37
        return;
    }
    const U keep = at<U>(c, 0xa00);                                                 // 004F87E1
    if ((keep & 0x3f00) != 0x3f00) {                                                // 004F87E7..F5
        if ((at<U>(c, 0x9fc) & 0x100) != 0 && (keep & 0x100) == 0)                  // 004F87FB..09 (first test uses the
            recovered_set_activity_004f00a0(c, nullptr, 0x100, 0, 0);               //  +A00 read at 004F87E1)
        for (U bit = 0x200; bit <= 0x2000; bit <<= 1) {                             // 004F881B..C5 (re-read each time)
            if ((at<U>(c, 0x9fc) & bit) != 0 && (at<U>(c, 0xa00) & bit) == 0)
                recovered_set_activity_004f00a0(c, nullptr, bit, 0, 0);
        }
        at<U>(c, 0x9fc) &= 0xffffc0ffu;                                             // 004F88CA
    }
    if (mode != 1) return;                                                          // 004F88D4..D7
    U bits = at<U>(c, 0x9fc); U one = bits & 1;
    if (one != 0 && (at<U8>(c, 0xa00) & 1) == 0) {                                  // 004F88DD..F0
        at<U>(c, 0x9fc) = bits & 0xfffffffeu;
        recovered_set_stealth_mode_0050ee50(c, nullptr, 0, 0);                      // 004F88FF..05
    }
    bits = at<U>(c, 0x9fc); U four = bits & 4;
    if (four != 0 && (at<U8>(c, 0xa00) & 4) == 0) {                                // 004F890A..1E
        at<U>(c, 0x9fc) = bits & 0xfffffffbu;
        kotor_fastcall_004cb4f0(c, nullptr);                                        // 004F892D..2F
    }
    for (U bit = 8; bit <= 0x40; bit <<= 1) {                                       // 004F8934..9E
        bits = at<U>(c, 0x9fc);
        if ((bits & bit) != 0 && (at<U8>(c, 0xa00) & bit) == 0) at<U>(c, 0x9fc) = bits & ~bit;
    }
    bits = at<U>(c, 0x9fc);
    if ((bits & 0x80) != 0 && (at<U8>(c, 0xa00) & 0x80) == 0) at<U>(c, 0x9fc) = bits & 0xffffff7fu; // 004F89A4..BD (sign tests)
}

// ---------------------------------------------------------------------------------------------------------
// 00581A10 CSWRoomSurfaceMesh::GetSurfaceMaterialWalkCheckOnly(point): vertical walkable ray in mesh space.
U __fastcall w5_surface_mesh_get_material_walk_check_only_00581a10(void* mesh, void*, Vector p) {
    Vector local;                                                                   // [esp+1C]
    collision_mesh_world_to_local_00596a00(mesh, nullptr, &local, &p);              // 00581A16..22
    SurfaceHit hit;                                                                 // [esp+28]
    hit.mask_0 = 0xffffffffu; hit.order_4 = 0; hit.hit_8 = 0; hit.face_c = -1;
    hit.point_10 = vec_of_bits(0, 0, 0); hit.bit_1c = 0;                            // 00581A27..BE
    w5_adapt_check_aabb_walkable_00581530(mesh, nullptr, vec_of_bits(bits(local.x), bits(local.y), k_1000_0x447a0000),
                                          vec_of_bits(bits(local.x), bits(local.y), k_down_1000), &hit); // 00581AC6
    if (hit.face_c == -1) return 0;                                                 // 00581ACB..D4
    return at<const U*>(mesh, 0x64)[hit.face_c];                                    // 00581ADD..E0 (material per face)
}

// 00506400 CSWSArea::PositionWalkable(point): room under the point, its material's surfacemat "Walk" column.
U __fastcall w5_area_position_walkable_00506400(void* area, void*, Vector p) {
    void* room = area_get_room(area, &p, nullptr);            // 00506419..20
    if (room == nullptr) return 0;                                                  // 00506425..27
    const U material = w5_surface_mesh_get_material_walk_check_only_00581a10(at<void*>(room, 0x3c), nullptr, p); // 0050642D..49
    alignas(4) unsigned char column[8];                                             // CExoString [esp+8]
    frame_LogStringCtor_005e5a90(column, nullptr, k_walk_00747670);                 // 0050644E..59
    I walk = 0;   // [esp+4]: the original leaves it unwritten; C2DA::GetINTEntry writes it on every row/column hit
    c2da_current_get_int_00413660(at<void*>(at<void*>(kotor_cell_rules_007a3a28, 0xb8), 0x14), nullptr, material, column, &walk); // 0050645E..82
    frame_LogStringDtor_005e5c20(column, nullptr);                                  // 00506487..93
    return walk != 0 ? 1u : 0u;                                                     // 00506498..A0
}

// ---------------------------------------------------------------------------------------------------------
// 005891B0 CSWSDoor::GetActionPoint(out, state, index): mesh (+32C per state) action point index*12 + 70h, world space.
Vector* __fastcall w5_door_get_action_point_005891b0(void* door, void*, Vector* out, U state, U index) {
    U mesh_index;
    if (state == k_closed_00002726) mesh_index = k_closed_mesh_0074c394;            // 005891B4..BA, 0058920A
    else if (state == 0x2742) mesh_index = k_open1_mesh_0074c398;                   // 005891BC..BF, 005891E0
    else if (state == 0x2743) mesh_index = k_open2_mesh_0074c39c;                   // 005891C1..C2, 005891D9
    else { *out = vec_of_bits(k_far_0xcb18967f, k_far_0xcb18967f, k_far_0xcb18967f); return out; } // 005891C4..D6
    void* mesh = at<void*>(door, 0x32c + mesh_index * 4);                            // 005891EB / 0058920F
    const U i = index & 0xff;                                                        // movzx byte
    collision_mesh_local_to_world_00596aa0(mesh, nullptr, out,
        reinterpret_cast<const Vector*>(static_cast<unsigned char*>(mesh) + 0x70 + i * 12)); // 005891F6..FF
    return out;
}

namespace {
// squared distance as the x87 sequence at 005892BD..05 / 00589305..4B: dy and dx stay in registers, dz/dy/dx are
// also stored as floats; sum = (dzf*dzf + dy*dyf) + dxf*dx (53-bit precision control -> double)
inline double action_point_distance(const Vector& from, const Vector& p) {
    const double dx = static_cast<double>(from.x) - p.x, dy = static_cast<double>(from.y) - p.y, dz = static_cast<double>(from.z) - p.z;
    const float dxf = static_cast<float>(dx), dyf = static_cast<float>(dy), dzf = static_cast<float>(dz);
    return (static_cast<double>(dzf) * dzf + dy * dyf) + static_cast<double>(dxf) * dx;
}
}  // namespace

// 00589240 CSWSDoor::GetNearestActionPoint(out, from, closed-only flag)
Vector* __fastcall w5_door_get_nearest_action_point_00589240(void* door, void*, Vector* out, const Vector* from, U flag) {
    U state = at<U>(door, 0xd4);                                                    // 0058924C ([esp+14] and ESI)
    U best_x = 0, best_y = 0, best_z = 0;                                           // [esp+54..5C]
    float best = flt(k_1000_0x447a0000);                                            // [esp+10]
    void* area = recovered_object_area_004cb120(door, nullptr);                     // 00589276 (EBX, [esp+1C])
    if (flag != 0) state = k_closed_00002726;                                       // 00589284..94
    Vector p0, p1;                                                                  // [esp+24], [esp+30]
    w5_door_get_action_point_005891b0(door, nullptr, &p0, state, 0);                 // 00589298..A2
    w5_door_get_action_point_005891b0(door, nullptr, &p1, state, 1);                 // 005892A7..B1
    const double sum0 = action_point_distance(*from, p0);
    const float d0 = static_cast<float>(sum0);                                      // [esp+20]
    const float d1 = static_cast<float>(action_point_distance(*from, p1));          // [esp+18]
    bool chosen = false;
    if (sum0 < static_cast<double>(d1)) {                                           // 0058934D..56 (test ah,5; jp: !(a<b))
        if (area == nullptr) goto none;                                             // 00589358..5A
        if (w5_area_position_walkable_00506400(area, nullptr, p0)) {                // 00589360..82
            best_x = bits(p0.x); best_y = bits(p0.y); best_z = bits(p0.z); best = d0; chosen = true; // 00589384..9C
        }
    }
    if (!chosen) {                                                                  // 0058939E
        if (area == nullptr) goto none;
        if (!w5_area_position_walkable_00506400(area, nullptr, p1)) goto none;      // 005893A2..C4
        best_x = bits(p1.x); best_y = bits(p1.y); best_z = bits(p1.z); best = d1;   // 005893C6..DE
    }
    goto picked;
none:
    best_x = 0; best_y = 0;                                                         // 005893E0..E4 ([58], [54]; [5C] unchanged = 0)
picked:
    if (state != k_closed_00002726) {                                               // 005893E8..F0
        void* mesh = at<void*>(door, 0x32c + k_closed_mesh_0074c394 * 4);           // 005893F6..FC
        Vector q0, q1;                                                              // [esp+3C], [esp+48]
        collision_mesh_local_to_world_00596aa0(mesh, nullptr, &q0, reinterpret_cast<const Vector*>(static_cast<unsigned char*>(mesh) + 0x70));
        collision_mesh_local_to_world_00596aa0(mesh, nullptr, &q1, reinterpret_cast<const Vector*>(static_cast<unsigned char*>(mesh) + 0x7c));
        // 0058942C..C4: e1 as action_point_distance; e0 = (qdzf*qdzf + qdy*qdyf) + qdxf*qdxf (dx only as a float)
        const double qdx = static_cast<double>(from->x) - q0.x, qdy = static_cast<double>(from->y) - q0.y, qdz = static_cast<double>(from->z) - q0.z;
        const float qdxf = static_cast<float>(qdx), qdyf = static_cast<float>(qdy), qdzf = static_cast<float>(qdz);
        const float e1 = static_cast<float>(action_point_distance(*from, q1));     // [esp+20]
        const double e0 = (static_cast<double>(qdzf) * qdzf + qdy * qdyf) + static_cast<double>(qdxf) * qdxf;
        bool done = false;
        if (e0 < static_cast<double>(best) && area != nullptr &&                    // 005894C6..D5
            w5_area_position_walkable_00506400(area, nullptr, q0)) {                // 005894D7..F9
            best_x = bits(q0.x); best_y = bits(q0.y); best_z = bits(q0.z); done = true;
        }
        if (!done && static_cast<double>(e1) < static_cast<double>(best) && area != nullptr && // 00589509..1A
            w5_area_position_walkable_00506400(area, nullptr, q1)) {                // 0058951C..3E
            best_x = bits(q1.x); best_y = bits(q1.y); best_z = bits(q1.z);
        }
    }
    *out = vec_of_bits(best_x, best_y, best_z);                                     // 00589550..65
    return out;
}

// ---------------------------------------------------------------------------------------------------------
// 004EE440 CSWSCreature::GetUseRange(id, pos*, range*, flag)
U __fastcall w5_creature_get_use_range_004ee440(void* self, void*, U id, Vector* pos, float* range, U flag) {
    void* obj = game_object(id);                                                    // 004EE452..56
    void* pf = at<void*>(self, 0x340);
    *range = at<float>(pf, 4);                                                      // 004EE45B..6C (always written)
    if (obj == nullptr || as_object(obj) == nullptr || recovered_object_area_004cb120(as_object(obj), nullptr) == nullptr)
        return 0;                                                                   // 004EE46A..93
    const U type = at<U8>(obj, 8);                                                  // 004EE499
    switch (type) {                                                                 // 004EE49D..A9 (table 004EE750)
    case 5: {                                                                       // 004EE4B0 creature
        const U target = at<U>(pf, 0x38);
        *range = (id == target && target != k_object_invalid) ? at<float>(pf, 0xc) : at<float>(pf, 8); // 004EE4B6..CE
        copy_vector(pos, static_cast<unsigned char*>(as_object(obj)) + 0x90);       // 004EE4D0..ED
        const float other = at<float>(at<void*>(as_creature(obj), 0x340), 8);        // 004EE4F0..FD
        *range = static_cast<float>((static_cast<double>(other) + *range) + k_use_range_creature_00746d38); // 004EE4FD..10
        return 1;
    }
    case 9: {                                                                       // 004EE519 placeable
        Vector nearest;                                                             // [esp+14]
        const Vector* p = w3_placeable_nearest_action_point_00584c20(as_placeable(obj), nullptr, &nearest,
            static_cast<const Vector*>(static_cast<void*>(static_cast<unsigned char*>(self) + 0x90))); // 004EE519..2E
        copy_vector(pos, p);                                                        // 004EE533..46
        if (at<U>(as_placeable(obj), 0x33c) != 0 && flag == 0 &&                    // 004EE549..60
            w2x_area_safe_point_004be5e0(recovered_object_area_004cb120(self, nullptr), nullptr, pos,
                                         at<U>(self, 0x340)) == 1) {                // 004EE562..7B
            at<U>(range, 0) = k_tenth_0x3dcccccd;                                   // 004EE57D
        } else {
            *range = static_cast<float>(static_cast<double>(*range) + k_use_range_door_0073e31c); // 004EE585..8D
        }
        if (at<U>(as_placeable(obj), 0x44c) != 0)                                   // 004EE58F..9E
            *range = static_cast<float>(static_cast<double>(*range) + k_use_range_placeable_00743264); // 004EE5A0..A8
        return 1;                                                                   // 004EE5AA..B6
    }
    case 10: {                                                                      // 004EE5B9 door
        Vector nearest;                                                             // [esp+20]
        const Vector* p = w5_door_get_nearest_action_point_00589240(as_door(obj), nullptr, &nearest,
            static_cast<const Vector*>(static_cast<void*>(static_cast<unsigned char*>(self) + 0x90)), 0); // 004EE5B9..D0
        copy_vector(pos, p);                                                        // 004EE5D5..EA
        at<U>(pos, 8) = w1x_area_height_bits_004bc380(recovered_object_area_004cb120(as_door(obj), nullptr), nullptr,
                                                      pos, 1, nullptr);             // 004EE5E8..05 (ComputeHeight ST0 -> fstp)
        const U target = at<U>(pf, 0x38);                                           // 004EE608..0E
        if (id == target && target != k_object_invalid) {                           // 004EE611..1B
            *range = static_cast<float>(static_cast<double>(at<float>(pf, 0xc)) + *range); // 004EE61D..29
            return 1;
        }
        void* door = as_door(obj);
        if (at<U>(door, 0x3c4) != 0 && at<U>(as_door(obj), 0x2c4) != 0 && flag == 0 &&       // 004EE637..5F
            w2x_area_safe_point_004be5e0(recovered_object_area_004cb120(self, nullptr), nullptr, pos, addr(pf)) == 1) { // 004EE661..7A
            at<U>(range, 0) = k_tenth_0x3dcccccd;                                   // 004EE683
            return 1;
        }
        *range = static_cast<float>(static_cast<double>(*range) + k_use_range_door_0073e31c); // 004EE695..A5
        return 1;
    }
    case 7: {                                                                       // 004EE6B2 trigger
        void* trigger = as_trigger(obj);
        if (at<U>(trigger, 0x2b4) != 0) { copy_vector(pos, static_cast<unsigned char*>(trigger) + 0x90); return 1; } // 004EE6B9..E8
        recovered_trigger_nearest_point_0058c8a0(trigger, nullptr, static_cast<unsigned char*>(self) + 0x90, pos); // 004EE6EB..F5
        *range = static_cast<float>(static_cast<double>(*range) + k_use_range_trigger_0073e9ac); // 004EE6FA..05
        return 1;                                                                   // 004EE707..10
    }
    default:                                                                        // 004EE713 (6, 8 and out of table)
        copy_vector(pos, static_cast<unsigned char*>(as_object(obj)) + 0x90);
        return 1;
    }
}

// 004F6000 CSWSCreature::GetIsInUseRange(id, extra, flag)
U __fastcall w5_creature_get_is_in_use_range_004f6000(void* self, void*, U id, float extra, U flag) {
    void* obj = game_object(id);                                                    // 004F600F..15
    Vector pos = vec_of_bits(0, 0, 0);                                              // [esp+10]
    float range = 0.0f;                                                             // [esp+44] (the id argument slot)
    if (obj == nullptr || as_object(obj) == nullptr || recovered_object_area_004cb120(as_object(obj), nullptr) == nullptr)
        return 0;                                                                   // 004F601C..63
    if (at<U>(self, 0x8c) != at<U>(as_object(obj), 0x8c)) return 0;                // 004F6069..7E (same area id)
    extra = static_cast<float>(static_cast<double>(extra) + k_in_range_slack_0073e798); // 004F6084..92
    if (as_trigger(obj) != nullptr && at<U>(as_trigger(obj), 0x2b4) != 0) {        // 004F6088..AC
        return w5_trigger_in_trigger_0058ce40(as_trigger(obj), nullptr,             // 004F60AE..C9 (EAX passed through)
            static_cast<const Vector*>(static_cast<void*>(static_cast<unsigned char*>(self) + 0x90)));
    }
    w5_creature_get_use_range_004ee440(self, nullptr, id, &pos, &range, 0);          // 004F60CC..DB
    Vector a, b;                                                                     // [esp+1C] eye of self, [esp+28] use point
    copy_vector(&a, static_cast<unsigned char*>(self) + 0x90);                       // 004F60E0..F1
    a.z = static_cast<float>(static_cast<double>(a.z) + k_eye_height_0073f414);      // 004F60FF..16
    copy_vector(&b, &pos);                                                           // 004F610D..32
    b.z = static_cast<float>(static_cast<double>(pos.z) + k_eye_height_0073f414);    // 004F611A..36
    Vector zero = vec_of_bits(0, 0, 0);                                              // [esp+34]
    U out[3];                                                                        // [esp+10] (written by the callee)
    if (area_clear_line_of_sight_0050c330(recovered_object_area_004cb120(as_object(obj), nullptr), nullptr, &a, &b, &zero,
            out, at<U>(self, 4), id, static_cast<int>(flag), 0) == 0) return 0;     // 004F613A..7D
    const double dx = static_cast<double>(b.x) - a.x, dy = static_cast<double>(b.y) - a.y; // 004F617F..8B
    const double r = static_cast<double>(range) + extra;                             // 004F618F..93
    const double d2 = dy * dy + dx * dx, r2 = r * r;                                 // 004F6197..A3
    return (r2 < d2 || r2 != r2 || d2 != d2) ? 0u : 1u;                              // 004F61A5..B2 (fcompp; test ah,1: C0)
}

// ---------------------------------------------------------------------------------------------------------
// 004F0B20 CSWSCreature::EvaluateLock(object, desired): the key (door +2D0 / placeable +264 tag) in the repository
// or an equipment slot lets the creature set the lock state; feedback 10h on success.
U __fastcall w5_creature_evaluate_lock_004f0b20(void* c, void*, void* object, U desired) {
    (void)ai_master();   // 004F0B2F ([esp+10]): the AI master; ai_event() below reads it again (inline view of 004AED80)
    U lock_off, tag_off; bool door_kind;
    void* target;
    if (as_door(object) != nullptr) { target = as_door(object); door_kind = true; lock_off = 0x2c4; tag_off = 0x2d0; }  // 004F0B40..68
    else if (as_placeable(object) != nullptr) { target = as_placeable(object); door_kind = false; lock_off = 0x260; tag_off = 0x264; } // 004F0C54..7A
    else return 0;                                                                  // 004F0C5B
    if (at<U>(target, lock_off) == desired) return 1;                               // 004F0B58..62 / 004F0C6A..74 (no feedback)
    void* tag = static_cast<unsigned char*>(target) + tag_off;
    if (frame_LogIsEmpty_005e58a0(tag, nullptr) != 0) return 0;                      // 004F0B70..77
    const U item_id = recovered_find_item_with_tag_00555ed0(recovered_creature_inventory_004ef770(c, nullptr, 1), nullptr, tag); // 004F0B7D..89
    void* item = recovered_item_lookup_004ae760(server_app(), nullptr, item_id);    // 004F0B8E..98
    auto notify = [&](void* key) {                                                  // door: byte +2F0; placeable: dword +270
        const bool remove = door_kind ? at<U8>(target, 0x2f0) != 0 : at<U>(target, 0x270) != 0;
        if (remove) ai_event(at<U>(c, 4), at<U>(key, 0x14), k_ai_event_unlocked_007441f4, nullptr); // 004F0BAB..C4
    };
    if (item == nullptr) {                                                          // 004F0B9F / 004F0BD4: equipment slots 0..17
        void* key = nullptr;
        for (U slot = 0; slot < 0x12 && key == nullptr; ++slot) {
            void* equipped = recovered_equipment_item_005a4c20(at<void*>(c, 0xa2c), nullptr, 1u << slot); // 004F0BE0..F2
            if (equipped != nullptr && async_string_equal_005e5200(static_cast<unsigned char*>(equipped) + 0x28, nullptr, tag) != 0)
                key = equipped;                                                     // 004F0BF9..08
        }
        if (key == nullptr) return 0;                                               // 004F0C10
        item = key;
    }
    notify(item);
    at<U>(target, lock_off) = desired;                                              // 004F0BC9 / 004F0C49
    recovered_creature_feedback_004ede10(c, nullptr, 0x10, nullptr);                // 004F0D50..56
    return 1;
}

// ---------------------------------------------------------------------------------------------------------
// 004F34A0 CSWSCreature::SetLockOrientationToObject(id, force)
void __fastcall w5_creature_set_lock_orientation_to_object_004f34a0(void* c, void*, U id, U force) {
    if (force == 0) {                                                               // 004F34A0..AC
        if (vcall<U>(c, 0x94) != 0) return;                                         // 004F34AE..B8
        if (recovered_creature_incapacitated_004ef890(c, nullptr) != 0) return;     // 004F34BE..C7
    }
    const U locked = at<U>(c, 0x944);
    if (id == k_object_invalid && locked != k_object_invalid && locked != at<U>(c, 4)) { // 004F34CD..F1
        void* other = nullptr;
        if (lookup(locked, &other) && at<U8>(other, 8) > k_object_type_bound_00746633) { // 004F34F3..23
            Vector d; kotor_cdecl_00423bc0(&d, static_cast<Vector*>(static_cast<void*>(static_cast<unsigned char*>(other) + 0x90)),
                                           static_cast<Vector*>(static_cast<void*>(static_cast<unsigned char*>(c) + 0x90))); // 004F3525..37
            Vector n; const Vector* r = normalize_004ab180(&n, &d);                 // 004F353C..46
            Vector dir; copy_vector(&dir, r);                                        // 004F354B..65
            w1x_object_set_orientation_004cd170(c, nullptr, const_cast<Vector*>(&dir));                 // 004F3569
        }
    }
    at<U>(c, 0x944) = id;                                                           // 004F356E
}

// ---------------------------------------------------------------------------------------------------------
// 004F8A50 CSWSCreature::AddMoveToPointActionToFront: packs a7..a16 into one flag word and queues action 1 at the front
U __fastcall w5_creature_add_move_to_point_action_to_front_004f8a50(void* c, void*, U group, const Vector* point,
    U area_id, U target_id, U run, float range, float a6, U a7, U a8, U a9, U a10, U a11, U a12,
    const Vector* a13, U a14, U a15, U a16) {
    U flags = (a7 + a7) | run;                                                      // 004F8A61..67
    if (!(a6 == 0.0f)) flags |= 4;                                                  // 004F8A50..70 (test ah,44h; jnp: equal)
    flags |= ((((a9 & 7) << 1) | a11) << 3);                                        // 004F8A73..85
    if (a10 != 0) flags |= (a10 & 3) << 7;                                          // 004F8A87..95
    flags |= ((((a16 & 1) << 1) | (a12 & 1)) << 9);                                 // 004F8A97..B2
    a7 = flags;                                                                     // 004F8AB4 (stored over the a7 slot)
    at<U>(c, 0xa8c) = 2;                                                            // 004F8ABF
    void* pf = at<void*>(c, 0x340);
    at<U>(pf, 0x26c) = 0; at<U>(pf, 0x270) = 0; at<U>(pf, 0x274) = 0;               // 004F8AC9..DD
    const U pairs[26] = {2, addr(&point->x), 2, addr(&point->y), 2, addr(&point->z), 3, addr(&area_id), 3, addr(&target_id),
                         1, addr(&a7), 2, addr(&range), 1, addr(&a8), 2, addr(&a6), 2, addr(&a13->x), 2, addr(&a13->y),
                         1, addr(&a14), 1, addr(&a15)};                             // 004F8AB8..3D
    w5_add_action_to_front_body(c, 1, group, pairs);                                // 004F8B41
    w5_creature_set_lock_orientation_to_object_004f34a0(c, nullptr, k_object_invalid, 0); // 004F8B46..4F
    return 1;
}

}  // extern "C"
namespace {
// 0057D911..9A0: open now (the AI event 7 to the door) and wait 0.5 s in the node's group
U open_door_now(void* self, void* node, U door_id) {
    at<U>(self, 0x1f0) = 0;                                                         // 0057D911..17
    ai_event(at<U>(self, 4), door_id, k_ai_event_open_door_007441d0, nullptr);      // 0057D915..41
    U half = k_half_0x3f000000;                                                     // [esp+20]
    const U pairs[26] = {2, addr(&half)};
    w5_add_action_to_front_body(self, 0x1e, at<U16>(node, 0x6c), pairs);           // 0057D946..96 (non-virtual ECX = self)
    return k_action_complete_00745dc0;                                              // 0057D99B
}
}  // namespace

// ---------------------------------------------------------------------------------------------------------
// 0057D490 CSWSObject::AIActionOpenDoor(node): node +38 door id, +3C (when 2+ parameters and nonzero) the run flag.
extern "C" U __fastcall w5_action_open_door_0057d490(void* self, void*, void* node) {
    U dead = 0;
    if (as_creature(self) != nullptr) dead = recovered_creature_incapacitated_004ef890(as_creature(self), nullptr); // 0057D4B2..C7
    if (vcall<U>(self, 0x94) == 1) goto fail;                                       // 0057D4C9..DA
    if (kotor_m15e_slot010_zero_0063e7f0(self, nullptr) == 1) goto fail;           // 0057D4E0..E9 (return_zero)
    if (dead == 1) goto fail;                                                       // 0057D4EF..F1
    if (as_creature(self) != nullptr) w5_creature_activity_manager_004f87d0(as_creature(self), nullptr, 2); // 0057D4F7..0D
    {
        U run = 1;                                                                  // [esp+18]
        U door_id = at<U>(node, 0x38);                                              // [esp+10]
        if (at<U16>(node, 0x6e) > 1 && at<U>(node, 0x3c) != 0) run = at<U>(node, 0x3c); // 0057D512..2E
        Vector pos = vec_of_bits(0, 0, 0);                                          // [esp+24]
        float range;                                                                // [esp+1C]
        void* door_obj = nullptr;                                                   // [esp+14]
        if (!lookup(door_id, &door_obj)) goto fail;                                 // 0057D532..6B
        if (as_creature(self) == nullptr) return open_door_now(self, node, door_id);                            // 0057D571..7A
        void* door = as_door(door_obj);                                             // 0057D580..86 (EDI)
        if (door == nullptr) goto fail;                                             // 0057D58B..8D
        (void)recovered_object_area_004cb120(door_obj, nullptr);                    // 0057D593..97 (result unused)
        w5_creature_get_use_range_004ee440(as_creature(self), nullptr, door_id, &pos, &range, 0); // 0057D59C..B6
        if (at<U>(door, 0xd4) != k_closed_00002726) return k_action_complete_00745dc0; // 0057D5BB..C5 -> 0057D99B
        const bool same_area = recovered_object_area_004cb120(as_object(door_obj), nullptr) ==
                               recovered_object_area_004cb120(self, nullptr);       // 0057D5CB..E6
        if (same_area && w5_creature_get_is_in_use_range_004f6000(as_creature(self), nullptr, door_id, 0.0f, 0) != 0) { // 0057D5EC..05
            if (at<U>(door, 0x2e8) != 0 &&                                          // 0057D60B..11
                static_cast<std::int32_t>(recovered_object_get_reputation_0057cb80(door, nullptr, at<U>(self, 4), 0)) < 0x5a) { // 0057D617..26 GetReputation RET 8, jge signed
                const U door_faction = at<U>(door, 0x2b8);                          // 0057D62A (read before the virtual call)
                if (door_faction != at<U>(at<void*>(as_creature(self), 0xa74), 0x78)) { // 0057D628..3E
                    ai_event(at<U>(self, 4), door_id, k_ai_event_script_007441dc, new_script_event(0x1a)); // 0057D640..91 (no [1F0h] reset)
                    return k_action_failed_00745dc4;                                // 0057D696 -> 0057D9AC
                }
            }
            if (w5_creature_evaluate_lock_004f0b20(as_creature(self), nullptr, as_object(door_obj), 0) == 0) { // 0057D69B..B6
                ai_event(at<U>(self, 4), door_id, k_ai_event_script_007441dc, new_script_event(0x22)); // 0057D6B8..0C (OnFailToOpen)
                at<U>(self, 0x1f0) = 0;                                             // 0057D711
                return k_action_failed_00745dc4;                                    // 0057D717 -> 0057D9AC
            }
            vcall<void>(self, 0xc0, door_id);                                       // 0057D71C..25
            if (at<U>(self, 0x1f0) != 0) return open_door_now(self, node, door_id);                             // 0057D72B..31
            const U pairs[26] = {3, addr(&door_id), 1, addr(&run)};
            w5_add_action_to_front_body(as_creature(self), 0x14, 0xffff, pairs);    // 0057D737..6B
            at<U>(self, 0x1f0) = 1;                                                 // 0057D770
            return k_action_complete_00745dc0;                                      // 0057D77A
        }
        {                                                                           // 0057D784: walk there first
            const U pairs[26] = {3, addr(&door_id), 1, addr(&run)};
            w5_add_action_to_front_body(as_creature(self), 0x14, 0xffff, pairs);    // 0057D784..CE
        }
        if (at<U>(door, 0x3c4) != 0 && at<U>(door, 0x2c4) != 0) {                   // 0057D7D3..E9 (lockable and locked)
            U half = k_half_0x3f000000;                                             // over the node argument slot [E+4]
            const U wait_pairs[26] = {2, addr(&half)};
            w5_add_action_to_front_body(as_creature(self), 0x1e, 0xfffe, wait_pairs); // 0057D7EF..44
            const U unlock_pairs[26] = {3, addr(&door_id)};
            w5_add_action_to_front_body(as_creature(self), 0x13, 0xfffe, unlock_pairs); // 0057D849..90
        }
        Vector zero = vec_of_bits(0, 0, 0);                                         // [esp+30]
        w5_creature_add_move_to_point_action_to_front_004f8a50(as_creature(self), nullptr, 0xfffe, &pos,
            at<U>(as_object(door_obj), 0x8c), k_object_invalid, run, range, 0.0f, 0, 0, 0, 0, 0, 0, &zero, 0, 0, 0); // 0057D895..F8
        at<U>(self, 0x1f0) = 1;                                                     // 0057D8FD
        return k_action_complete_00745dc0;                                          // 0057D907
    }
fail:
    at<U>(self, 0x1f0) = 0;                                                         // 0057D9A2
    return k_action_failed_00745dc4;                                                // 0057D9AC
}
