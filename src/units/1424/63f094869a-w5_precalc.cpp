// W5 t2: CSWCCreature::SetPrecalculatedPosition 00614B90 and its server-side closure.
// Covers: walkmesh slide loop, BumpFriends, subarea/trigger update, the in-process server position update,
// and the party-follow path recorder (ProcessPoint).
#include "d8371efa32-w5_t2.hpp"
#include "e5e3766b7c-w5_subarea.hpp"
#include "1a370ea669-w5_foreign.hpp"
#include <cstdint>

namespace kotor_scene3d { struct Vector; }
namespace kotor_world_los { extern kotor_scene3d::Vector up_007a19b0; }   // (0,0,1) [world-los 0528]

using namespace w5;

namespace {
constexpr U k_invalid = 0x7f000000u;
inline Vector& vec(void* o, U off) { return at<Vector>(o, off); }
inline const Vector* up() { return reinterpret_cast<const Vector*>(&kotor_world_los::up_007a19b0); }
inline void* p_of(U v) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(v)); }
inline void* server_creature(U client_id) {
    return kotor_fastcall_004ae770(server_app(), nullptr, kotor_fastcall_004aea30(server_app(), nullptr, client_id));
}
// GetServerObject(c) ? GetServerObject(c)->vtbl[30h]() : 0  (the repeated 0063D4B0 / call [edx+30h] idiom)
inline void* server_object_of(void* client_object) {
    if (!kotor_m15a_object_get_server_object_0063d4b0(client_object, nullptr)) return nullptr;
    return vcall<void*>(kotor_m15a_object_get_server_object_0063d4b0(client_object, nullptr), 0x30);
}
const float k_bump_gap_0073e798 = 0.1f;
const float k_corner_push_00747678 = 0.03f;
const float k_wall_push_0074f5e8 = 0.06f;
const double k_follow_min_step_00749d88 = 0.25;     // qword
const float k_follow_far_00747c3c = 49.0f;
const float k_follow_member_far_00741838 = 100.0f;
}  // namespace

extern "C" {
// Debug cells read by SetPrecalculatedPosition only (xref: no writer anywhere in swkotor.exe -> always 0).
U w5_cell_debug_008338c0 = 0, w5_cell_debug_008338c4 = 0, w5_cell_debug_008338c8 = 0, w5_cell_debug_008338cc = 0;
U w5_cell_follow_smooth_007a2418 = 1;               // .data initial 1; only reader 00636EE6, no writer
// ProcessPoint private statics (guard 00833934 bit 0 zero-initialises them on first use: same as load-time zero)
Vector w5_cell_follow_last_point_00833928 = {0, 0, 0};
float w5_cell_follow_last_yaw_00833924 = 0.0f;

// ---------------- small server helpers ----------------
// 004B14B0 CWalkHitInfo::CWalkHitInfo
void* __fastcall w5_walk_hit_construct_004b14b0(void* hit, void*) {
    for (U off = 4; off <= 0x3c; off += 4) at<U>(hit, off) = 0;
    at<U>(hit, 0) = 0; at<U>(hit, 0x44) = 0; at<U>(hit, 0x40) = k_invalid;
    return hit;
}
// 004AEF10 CServerExoApp::GetCreaturePathfindInformation(client id)
void* __fastcall w5_server_get_creature_pathfind_004aef10(void* server, void*, U client_id) {
    void* internal = at<void*>(server, 4);
    void* c = kotor_fastcall_004b1790(internal, nullptr, kotor_fastcall_004b1f70(internal, nullptr, client_id));
    return c ? at<void*>(c, 0x340) : nullptr;
}
// 004AEE80 CServerExoApp::TestDirectLine(id, area_arg, from, to, radius*, band, unused, hit)
I __fastcall w5_server_test_direct_line_004aee80(void* server, void*, U id, void* fallback_area, const Vector* from,
                                                 const Vector* to, const float* radius, float band, U unused, void* hit) {
    (void)fallback_area;                                                                   // a2 is never read by the original
    void* internal = at<void*>(server, 4);
    void* area;
    if (server_internal_get_module_004b14f0(internal, nullptr))                            // 004AEE83..8E
        area = module_get_area_004c30f0(server_internal_get_module_004b14f0(internal, nullptr), nullptr); // 004AEE90..A1
    else
        area = reinterpret_cast<void*>(static_cast<std::uintptr_t>(id));                  // 004AEEA3 mov edi,[esp+0Ch]: original quirk,
                                                                                           // the object-id argument (a1) becomes the area pointer
    void* c = kotor_fastcall_004b1790(internal, nullptr, kotor_fastcall_004b1f70(internal, nullptr, id)); // 004AEEA7..B8
    if (!c || !area) return 0;                                                             // 004AEEBD..C3
    at<void*>(area, 0x1ac) = at<void*>(c, 0x340);                                          // 004AEEC5, 004AEEDD
    return w5_area_test_direct_line_004bcb70(area, nullptr, from, to, radius, band, unused, hit); // 004AEEEF..F1
}
// 00604C40 CSWCArea::TestDirectLine(id, from, to, radius*, band, unused, hit)
I __fastcall w5_client_area_test_direct_line_00604c40(void* client_area, void*, U id, const Vector* from,
                                                      const Vector* to, const float* radius, float band, U unused, void* hit) {
    if (!server_app()) return 0;
    return w5_server_test_direct_line_004aee80(server_app(), nullptr, id, static_cast<unsigned char*>(client_area) + 0x10c,
                                               from, to, radius, band, unused, hit);
}
// 004AEA50 CServerExoApp::ComputeHeight(unused, point*, flag)
double __fastcall w5_server_compute_height_004aea50(void* server, void*, void* unused, Vector* point, U flag) {
    (void)unused;
    void* internal = at<void*>(server, 4);
    if (!server_internal_get_module_004b14f0(internal, nullptr)) return 0.0;               // 004AEA56..5D
    void* area = module_get_area_004c30f0(server_internal_get_module_004b14f0(internal, nullptr), nullptr);
    if (!area) return 0.0;                                                                 // 004AEA6E..70
    return flt(w1x_area_height_bits_004bc380(area, nullptr, point, flag, nullptr));       // 004AEA72..80 (push 0)
}
// 0060BF20 CSWCCreature::ComputeZValue(area, point*, z, unused)
void __fastcall w5_client_creature_compute_z_value_0060bf20(void*, void*, void* area, Vector* point, float z, U unused) {
    (void)z; (void)unused;
    if (!server_app()) return;                                                             // 0060BF20..2A
    point->z = 0.0f;                                                                       // 0060BF37
    point->z = static_cast<float>(w5_server_compute_height_004aea50(server_app(), nullptr,
        static_cast<unsigned char*>(area) + 0x10c, point, 1));                             // 0060BF3E..54
}
// 004B1CB0 CServerExoAppInternal::UpDatePlayerPosition(server id, pos*, orientation*)
void __fastcall w5_server_internal_update_player_position_004b1cb0(void* internal, void*, U server_id,
                                                                   const Vector* p, const Vector* orientation) {
    void* object = nullptr;
    const U status = kotor_fastcall_004d8230(at<void*>(internal, 0x1005c), nullptr, server_id, &object); // 004B1CB5..C9
    if (static_cast<U8>(status) != 0) return;                                              // 004B1CCE ([0074666C] byte 00)
    if (!object) return;
    if (at<U8>(object, 8) != 5) return;                                                    // 004B1CDE..E7 ([00746634] byte 05)
    void* so = vcall<void*>(object, 0x30);                                                 // 004B1CE9..EC
    if (!so) return;
    recovered_object_position_004cd1a0(so, nullptr, p, 1, 1, 0);                           // 004B1CF5..02
    w1x_object_set_orientation_004cd170(so, nullptr, const_cast<Vector*>(orientation));                        // 004B1D07..0E
}
// 004AEAA0 CServerExoApp::UpDatePlayerPosition(client id, pos*, orientation*)
void __fastcall w5_server_update_player_position_004aeaa0(void* server, void*, U client_id, const Vector* p,
                                                          const Vector* orientation) {
    void* internal = at<void*>(server, 4);
    w5_server_internal_update_player_position_004b1cb0(internal, nullptr,
        kotor_fastcall_004b1f70(internal, nullptr, client_id), p, orientation);
}
// 004B6CC0 (CServerExoAppInternal) update subareas for object id moving from -> to
void __fastcall w5_server_internal_update_subareas_004b6cc0(void* internal, void*, U id, const Vector* from, const Vector* to) {
    const U key = id == k_invalid ? k_invalid : (id & ~0x80000000u);                       // 004B6CDF..FB ([0074665C])
    void* object = nullptr;
    kotor_fastcall_004d8230(at<void*>(internal, 0x1005c), nullptr, key, &object);          // 004B6CFB..07 (status ignored)
    if (!object) return;
    void* creature = vcall<void*>(vcall<void*>(object, 0x10), 0x30);                       // 004B6D14..1E
    if (!creature) return;
    void* area = recovered_object_area_004cb120(creature, nullptr);                        // 004B6D27..29
    if (!area) return;
    U list[3] = {0, 0, 0};                                                                 // CExoArrayList<ulong> [esp+14h]
    w5_area_intersecting_line_segment_004bf2c0(area, nullptr, from, to, list);             // 004B6D48..55
    w5_creature_update_subareas_on_move_to_0051b7b0(creature, nullptr, from, to, 1, list, 0); // 004B6D5A..66
    live_free(p_of(list[0]));                                                              // 004B6D6B..78
}
// 004AEAD0 CServerExoApp::UpdateSubareasOnMoveTo: mov ecx,[ecx+4]; jmp 004B6CC0
void __fastcall w5_server_update_subareas_on_move_to_004aead0(void* server, void*, U id, const Vector* from, const Vector* to) {
    w5_server_internal_update_subareas_004b6cc0(at<void*>(server, 4), nullptr, id, from, to);
}

// 004A98B0 lineproject(out, a, b, p): projection of p onto the line a->b (statics 00831EE8..F4 are written
// only by lineproject and read by nobody -> not modelled)
Vector* __cdecl w5_line_project_004a98b0(Vector* out, const Vector* a, const Vector* b, const Vector* p) {
    const float dx = static_cast<float>(static_cast<double>(b->x) - a->x);                 // 004A98EA..F2
    const float dy = static_cast<float>(static_cast<double>(b->y) - a->y);
    const float dz = static_cast<float>(static_cast<double>(b->z) - a->z);
    double t = (static_cast<double>(dx) * dx + static_cast<double>(dz) * dz) + static_cast<double>(dy) * dy; // 004A991D..40
    if (t != 0.0) {                                                                        // 004A9942..4D
        const double s = ((static_cast<double>(p->z) - a->z) * dz + (static_cast<double>(p->x) - a->x) * dx) +
                         (static_cast<double>(p->y) - a->y) * dy;                          // 004A994F..73
        t = s / t;                                                                         // 004A9975 fdivrp
    }
    const float sx = static_cast<float>(dx * t), sy = static_cast<float>(dy * t);         // 004A9977..8B
    const double z = t * dz + a->z;                                                        // 004A998F..93
    out->x = static_cast<float>(static_cast<double>(sx) + a->x);
    out->y = static_cast<float>(static_cast<double>(sy) + a->y);
    out->z = static_cast<float>(z);
    return out;
}

// 005188E0 CSWSCreature::HasAction(type): scan the action list at +FC for a node whose type equals `type`
I __fastcall kotor_fastcall_005188e0(void* creature, void*, U type) {
    void* pos = at<void*>(at<void*>(creature, 0xfc), 0);                                   // 005188E4..EA
    while (pos) {
        void* node = unresolved_list_at_pos_005e9b60(at<void*>(creature, 0xfc), nullptr, pos);   // 00518900..07
        if (at<U>(node, 0) == type) return 1;                                              // 0051890C..0E
        unresolved_list_next_005e9b90(at<void*>(creature, 0xfc), nullptr, &pos);           // 00518910..1B
    }
    return 0;
}

// ---------------- 004F6390 CSWSCreature::BumpFriends(other, from*, to*) ----------------
// The CExoString debug text built at 004F63C4..0040 / 004F65D4..05 / 004F66E6..17 is never output (CStr result
// unused) -> not modelled.
I __fastcall w5_creature_bump_friends_004f6390(void* self, void*, void* other, const Vector* from, const Vector* to) {
    if (!recovered_creature_collision_flag_004f62a0(self, nullptr, other)) return 0;       // 004F63B1..BE
    const Vector op = vec(other, 0x90);                                                    // 004F6415..37 ([S-5C])
    const float reach = static_cast<float>((static_cast<double>(at<float>(at<void*>(other, 0x340), 8)) + k_bump_gap_0073e798) +
                                           at<float>(at<void*>(self, 0x340), 8));          // 004F643B..5A ([S-80])
    Vector line;                                                                           // [S-50]
    w5_line_project_004a98b0(&line, from, to, &op);                                        // 004F644D..61
    Vector away{static_cast<float>(static_cast<double>(op.x) - line.x),                    // 004F6466..90
                static_cast<float>(static_cast<double>(op.y) - line.y),
                static_cast<float>(static_cast<double>(op.z) - line.z)};
    Vector n;                                                                              // [S-2C]
    normalize_004ab180(&n, &away);                                                         // 004F6494
    const double nz = static_cast<double>(n.z) * reach;                                    // 004F6499..A3 (ST)
    const float ny = static_cast<float>(static_cast<double>(n.y) * reach);                 // 004F64AD..B5
    const float nx = static_cast<float>(static_cast<double>(n.x) * reach);                 // 004F64B9..C1
    Vector place{static_cast<float>(static_cast<double>(nx) + line.x),                     // 004F64C5..E1 ([S-68])
                 static_cast<float>(static_cast<double>(line.y) + ny),
                 static_cast<float>(nz + line.z)};
    const Vector saved = vec(self, 0x90);                                                  // 004F64E5..08 ([S-38])
    recovered_object_position_004cd1a0(self, nullptr, &line, 1, 0, 0);                    // 004F64ED..06
    void* other_pf = at<void*>(other, 0x340);
    float lo, hi;                                                                          // [S-7C] [S-80]
    if (place.z < op.z) {                                                                  // 004F650B..1E
        lo = static_cast<float>(static_cast<double>(place.z) - k_bump_gap_0073e798);
        hi = static_cast<float>(static_cast<double>(at<float>(other_pf, 0x14)) + place.z);
    } else {
        lo = static_cast<float>(static_cast<double>(op.z) - k_bump_gap_0073e798);
        hi = static_cast<float>(static_cast<double>(at<float>(other_pf, 0x14)) + op.z);
    }
    const U safe = w1x_area_safe_location_004be860(recovered_object_area_004cb120(self, nullptr), nullptr,
        static_cast<U>(reinterpret_cast<std::uintptr_t>(&place)), 0x40a00000u /*5.0*/,
        static_cast<U>(reinterpret_cast<std::uintptr_t>(other_pf)), 1,
        static_cast<U>(reinterpret_cast<std::uintptr_t>(&place)));                         // 004F6554..6B
    U blocker = 0;                                                                         // [S-74]
    const bool clear = w5_t2_area_no_nonwalk_polys_in_doors_0050ca10(recovered_object_area_004cb120(self, nullptr), nullptr,
        &op, &place, lo, hi, at<float>(other_pf, 4), k_invalid, &blocker, 0, 1, 0) == 1;  // 004F6570..BA
    if (safe != 0 && clear) {                                                              // 004F65BA..C0
        recovered_object_position_004cd1a0(other, nullptr, &place, 1, 1, 0);              // 004F65C2..CF
        recovered_object_position_004cd1a0(self, nullptr, &saved, 1, 1, 0);               // 004F6605..24
        return 1;
    }
    const Vector back{-at<float>(self, 0x9c), -at<float>(self, 0xa0), -at<float>(self, 0xa4)}; // 004F6615..56 ([S-20])
    Vector out{0, 0, 0};                                                                   // [S-44]
    const U moved = w5_t2_area_compute_safe_location_in_direction_004c0fa0(recovered_object_area_004cb120(self, nullptr), nullptr,
        to, &back, 4.0f, other_pf, 1, &out);                                               // 004F661B..7C
    if (moved != 1) {                                                                      // 004F6681..8A
        recovered_object_position_004cd1a0(self, nullptr, &saved, 1, 1, 0);               // 004F6756..5D
        return 0;
    }
    recovered_object_position_004cd1a0(other, nullptr, &out, 1, 1, 0);                    // 004F6690..97
    if (at<U>(other, 0xa88) != 0) {                                                        // 004F669C..A4
        void* client = client_app();
        const U client_id = kotor_fastcall_005eda50(client, nullptr, at<U>(other, 4));      // 004F66A6..B5
        const U index = recovered_party_index_006349c0(kotor_fastcall_005ed8b0(client, nullptr), nullptr, client_id); // 004F66BA..C4
        at<U>(kotor_fastcall_005ed8b0(client_app(), nullptr), index * 0x88 + 0x28) = 0;   // 004F66CB..DE
    }
    recovered_object_position_004cd1a0(self, nullptr, &saved, 1, 1, 0);                   // 004F6717..24
    return 1;
}

// ---------------- 00636A30 CSWPartyFollowDataArray::ProcessPoint(point*, yaw, unused) ----------------
// Ring of 100 entries (28h bytes: +0 point, +C yaw, +10/+1C formation points of members 1/2) at this+0;
// +FA0 CSWParty*, +FA4 next index, +FA8 count.
void __fastcall w5_party_follow_process_point_00636a30(void* f, void*, const Vector* p, float yaw, U unused) {
    (void)unused;
    I prev = at<I>(f, 0xfa4) - 1;                                                          // 00636A3B..45
    if (prev < 0) prev += 100;
    const Vector* pd = static_cast<const Vector*>(kotor_fastcall_00634cc0(f, nullptr, prev)); // 00636A76..79
    const double dx = static_cast<double>(p->x) - pd->x, dy = static_cast<double>(p->y) - pd->y,
                 dz = static_cast<double>(p->z) - pd->z;                                   // 00636A85..9B
    const float d2 = static_cast<float>((dz * dz + dy * dy) + dx * dx);                    // 00636AA4..B8 ([S-D0])
    void* party = at<void*>(f, 0xfa0);
    const U leader = at<U>(party, 0x24);                                                   // 00636AA6
    void* sc = server_creature(leader);                                                    // 00636AAB..D1 ([S-D8])
    unsigned char resref[16];                                                              // [S-3C] CResRef (unused by the callee)
    c2da_default_ref_00405ed0(resref, nullptr);                                            // 00636AD6..E1
    if (at<I>(f, 0xfa8) == 0) w5_cell_follow_last_point_00833928 = *p;                     // 00636AE6..08
    if (!sc) return;                                                                       // 00636B0D..13
    WalkHit hit{};                                                                         // [S-84] inline CWalkHitInfo
    hit.blocker_40 = k_invalid;                                                            // 00636BB7
    void* pf = at<void*>(sc, 0x340);
    float radius = at<float>(pf, 4);                                                       // 00636BC2..CB ([S-D4])
    const float band = at<float>(pf, 0x14);                                                // 00636BCF ([S-D8])
    const Vector* from = static_cast<const Vector*>(kotor_fastcall_00634cc0(f, nullptr, prev)); // 00636BD2..D9
    const I r = w5_server_test_direct_line_004aee80(server_app(), nullptr, leader, resref, from, p, &radius, band, 1, &hit); // 00636BDE..03
    const bool reach = r == 1 ||                                                           // 00636C0A..0D
        (r == -3 && hit.blocker_40 != k_invalid && recovered_party_member_00634620(party, nullptr, hit.blocker_40) != 0); // 00636C13..34
    if (!reach) {                                                                          // 00636C36..70: record the corner
        at<Vector>(f, static_cast<U>(at<I>(f, 0xfa4)) * 0x28) = w5_cell_follow_last_point_00833928;
        I next = at<I>(f, 0xfa4);
        next = next >= 99 ? 0 : next + 1;
        at<I>(f, 0xfa4) = next;
    }
    w5_cell_follow_last_point_00833928 = *p;                                               // 00636C76..92 / 00636CA0..BD
    w5_cell_follow_last_yaw_00833924 = yaw;                                                // 00636C98 / 00636CC2
    if ((d2 < k_follow_min_step_00749d88 || d2 != d2) && r == 1) return;                  // 00636CC8..DC (test ah,1: C0 also when unordered)
    I idx = at<I>(f, 0xfa4);
    at<Vector>(f, static_cast<U>(idx) * 0x28) = *p;                                        // 00636CE2..02
    at<float>(f, static_cast<U>(at<I>(f, 0xfa4)) * 0x28 + 0xc) = yaw;                      // 00636D05..0E
    idx = at<I>(f, 0xfa4);
    idx = idx >= 99 ? 0 : idx + 1;                                                         // 00636D12..22
    if (at<I>(f, 0xfa8) < 100) at<I>(f, 0xfa8) += 1;                                       // 00636D28..3A
    at<I>(f, 0xfa4) = idx;                                                                 // 00636D31
    if (at<I>(f, 0xfa8) < 3) return;                                                       // 00636D40..47
    I j = idx - 1;                                                                         // 00636D4D..52
    if (j < 0) j += 100;
    const float yaw_j = at<float>(f, static_cast<U>(j) * 0x28 + 0xc);                      // 00636D55..5D ([S-D0], EBX)
    I k = j - 1;                                                                           // 00636D58..63
    if (k < 0) k += 100;
    unsigned char* ek = static_cast<unsigned char*>(f) + static_cast<U>(k) * 0x28;         // [S-D8]
    const Vector base = at<Vector>(ek, 0);                                                 // [S-C0]
    for (U i = 1; i < 3; ++i) {                                                            // 00636D94..EE0
        const Vector offset = at<Vector>(party, i * 0x88 + 0x5c);                          // 00636DD0..F8 ([S-9C])
        // 00636DF6..37: Quaternion(up, DegToRad(yaw_k)) * offset -> [S-2C]: never read -> not modelled
        Quaternion q;
        quaternion_construct_004ac960(&q, nullptr, Vector{0, 0, 1}, static_cast<float>(deg_to_rad_004a96d0(yaw_j))); // 00636E3A..68
        Vector r2;
        quaternion_rotate_004a9bb0(&r2, &q, &offset);                                      // 00636E6D..73 ([S-90])
        const Vector pt{static_cast<float>(static_cast<double>(r2.x) + base.x),
                        static_cast<float>(static_cast<double>(r2.y) + base.y),
                        static_cast<float>(static_cast<double>(r2.z) + base.z)};           // 00636E78..9E
        at<Vector>(ek, i == 1 ? 0x10 : 0x1c) = pt;                                         // 00636EA2..D9
    }
    if (w5_cell_follow_smooth_007a2418 != 0 && at<I>(party, 0) > 1) {                      // 00636EE6..F8
        const int far_member = kotor_fastcall_00634da0(f, nullptr);                        // 00636EFA..FE
        const double d = kotor_fastcall_00634e00(f, nullptr, far_member, 1);               // 00636F03..06
        bool smooth = d >= k_follow_far_00747c3c;                                          // 00636F0B..16
        if (!smooth) {
            const I n = at<I>(party, 0);
            const float m1 = at<float>(party, 0xf4);
            if (n == 2) smooth = m1 >= k_follow_member_far_00741838;                       // 00636F20..36
            else if (n == 3) smooth = m1 >= k_follow_member_far_00741838 &&
                                      at<float>(party, 0x17c) >= k_follow_member_far_00741838; // 00636F38..61
        }
        if (smooth) w5_t2_party_follow_smooth_path_00635af0(f, nullptr);                   // 00636F63..65
    }
    if (at<U>(party, 0xac) != k_invalid) {                                                 // 00636F6A..7B
        void* m = server_creature(at<U>(party, 0xac));
        if (m && !kotor_fastcall_005188e0(m, nullptr, 0x3d)) {                     // 00636F9B..AA
            at<U>(party, 0xd0) = at<U>(party, 0xe0);                                       // 00636FAC..C0
            at<Vector>(party, 0xc4) = at<Vector>(party, 0xd4);                             // 00636FC3..D3
        }
    }
    if (at<U>(party, 0x134) != k_invalid) {                                                // 00636FD6..E7
        void* m = server_creature(at<U>(party, 0x134));
        if (m && !kotor_fastcall_005188e0(m, nullptr, 0x3d)) {                     // 00637007..16
            at<U>(party, 0x158) = at<U>(party, 0x168);                                     // 00637018..2F
            at<Vector>(party, 0x14c) = at<Vector>(party, 0x15c);                           // 0063702A..40
        }
    }
}

// ---------------- 00614B90 CSWCCreature::SetPrecalculatedPosition(target*) ----------------
I __fastcall w5_client_creature_set_precalculated_position_00614b90(void* c, void*, const Vector* target) {
    if (w5_cell_debug_008338c8)                                                            // 00614B90..A3
        kotor_unreachable_trap(0x00614BA9, "SetPrecalculatedPosition debug: ClosestPathPoint square (008338C8 has no writer)");
    if (w5_cell_debug_008338cc)                                                            // 00614C7C..8A
        kotor_unreachable_trap(0x00614C90, "SetPrecalculatedPosition debug: room triangle / line-of-sight squares (008338CC has no writer)");
    void* sc = server_object_of(c);                                                        // 00614F87..A4 ([S-214])
    if (sc) {
        if (w5x_creature_is_debilitated_005b4880(sc, nullptr)) return 1;                   // 00614FAD..B6
        if (!(at<U8>(sc, 0x9f0) & 2)) return 1;                                            // 00614FBC..C3
    }
    if (kotor_fastcall_0060f0f0(c, nullptr)) return 1;                                     // 00614FC9..D2
    void* so = server_object_of(c);                                                        // 00614FD8..F1
    // 00615000..02: no server object -> the original reads [0+340h] (EAX = 0) and faults; same unchecked read here.
    float radius = at<float>(at<void*>(so, 0x340), 4);                                     // 00615002..11 ([S-260])
    void* module = kotor_fastcall_005ed530(client_app(), nullptr);                         // 0061500B..18 ([S-278])
    Vector* pos = &vec(c, 0x24);                                                           // ESI
    const Vector delta{static_cast<float>(static_cast<double>(target->x) - pos->x),
                       static_cast<float>(static_cast<double>(target->y) - pos->y),
                       static_cast<float>(static_cast<double>(target->z) - pos->z)};       // 0061501D..47 ([S-270])
    Vector dir;                                                                            // [S-220]
    normalize_004ab180(&dir, &delta);                                                      // 0061504B
    if (at<U>(c, 0x3a8) != 1) return 1;                                                    // 00615050..5F
    if (!module) return 1;                                                                 // 00615065..6B
    if (!at<void*>(module, 0x48)) return 1;                                                // 00615071..76
    WalkHit hit;                                                                           // [S-208]
    w5_walk_hit_construct_004b14b0(&hit, nullptr);                                         // 0061507C..8B
    I n = 0;                                                                               // [S-274]
    if (w5_cell_debug_008338c4)                                                            // 00615090..97
        kotor_unreachable_trap(0x00615099, "SetPrecalculatedPosition debug: drawable squares (008338C4 has no writer)");
    const Vector backd{static_cast<float>(static_cast<double>(pos->x) - target->x),
                       static_cast<float>(static_cast<double>(pos->y) - target->y),
                       static_cast<float>(static_cast<double>(pos->z) - target->z)};       // 006150CD..07
    Vector back;                                                                           // [S-25C]
    Vector prev_normal{0, 0, 0};                                                           // [S-250]
    normalize_004ab180(&back, &backd);                                                     // 0061510B
    void* pf = w5_server_get_creature_pathfind_004aef10(server_app(), nullptr, at<U>(c, 4)); // 00615110..20 ([S-210])
    if (pf) at<U>(pf, 0x30) = k_invalid;                                                   // 00615125..30
    Vector tgt = *target;                                                                  // 00615137..47 ([S-288])
    I r = -1;                                                                              // EBP
    void* area = at<void*>(module, 0x48);
    const auto test = [&]() {
        return w5_client_area_test_direct_line_00604c40(area, nullptr, at<U>(c, 4), pos, &tgt, &radius,
                                                         at<float>(pf, 0x14), 1, &hit);
    };
    for (;;) {
        ++n;                                                                               // 00615160..76
        if (r != -1 && r != -2 && r != -3) break;                                          // 0061516F..84
        r = test();                                                                        // 00615229..67
        bool check = false;
        if (r == -3) {                                                                     // 00615269..6C
            void* blocker = kotor_fastcall_004ae770(server_app(), nullptr, hit.blocker_40);  // 00615272..82
            if (!blocker) check = true;                                                    // 00615287..89
            else {
                w5_creature_bump_friends_004f6390(sc, nullptr, blocker, pos, &tgt);        // 0061528F..9D
                r = test();                                                                // 006152A2..D9
                if (r == -3) {
                    void* sarea = recovered_object_area_004cb120(server_object_of(c), nullptr); // 006152E4..05
                    if (w2x_area_safe_point_004be5e0(sarea, nullptr, const_cast<Vector*>(pos), static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(pf))) != 0) check = true; // 0061530A..13
                    else {
                        w1x_area_safe_location_004be860(
                            recovered_object_area_004cb120(kotor_m15a_creature_get_server_creature_0060fb20(c, nullptr), nullptr),
                            nullptr, static_cast<U>(reinterpret_cast<std::uintptr_t>(pos)), 0x3f800000u,
                            static_cast<U>(reinterpret_cast<std::uintptr_t>(pf)), 1,
                            static_cast<U>(reinterpret_cast<std::uintptr_t>(pos)));        // 00615315..2F
                        r = test();                                                        // 00615334..6B
                    }
                }
            }
        }
        if (!check && r != -1 && r != -2 && r != -3) { if (n <= 5) continue; break; }     // 0061536D..7A, 0061587B..82
        if (hit.flag_0 != 1) { if (n <= 5) continue; break; }                              // 00615380..88
        // ---- slide (0061538E..006157F6) ----
        Vector c1, c2;                                                                     // [S-130] [S-148]
        vector_cross_004ab210(&c1, up(), &hit.normal_10);                                  // 0061538E..A3
        const float side_now = static_cast<float>((static_cast<double>(back.y) * c1.y + static_cast<double>(back.z) * c1.z) +
                                                  static_cast<double>(back.x) * c1.x);    // 006153A8..D2 ([S-20C])
        vector_cross_004ab210(&c2, up(), &prev_normal);                                    // 006153AF..D9
        const float side_prev = static_cast<float>((static_cast<double>(back.y) * c2.y + static_cast<double>(back.z) * c2.z) +
                                                   static_cast<double>(back.x) * c2.x);   // 006153DE..00 ([S-264])
        const bool corner = n > 1 && ((side_now > 0.0f && side_prev < 0.0f) || (side_now < 0.0f && side_prev > 0.0f)); // 00615404..56
        float nx, ny, nz;
        if (corner) {
            if (hit.type_44 != 2) break;                                                   // 0061545C..64 -> 00615888
            Vector c3; vector_cross_004ab210(&c3, up(), &dir);                             // 0061546A..7C
            const Vector push{static_cast<float>(static_cast<double>(c3.x) * k_corner_push_00747678),
                              static_cast<float>(static_cast<double>(c3.y) * k_corner_push_00747678),
                              static_cast<float>(static_cast<double>(c3.z) * k_corner_push_00747678)}; // 00615481..A6
            const double e34 = hit.scratch_1c[6], e38 = hit.scratch_1c[7];                // +34 / +38
            const double side = (static_cast<double>(dir.y) * e34 + static_cast<double>(dir.x) * -e38) +
                                static_cast<double>(dir.z) * 0.0;                          // 006154AA..D5
            Vector moved;
            if (side > 0.0) vector_sum_00423b90(&moved, pos, &push);                       // 006154EE..F6
            else kotor_cdecl_00423bc0(&moved, pos, &push);                                 // 006154FD..05
            nx = moved.x; ny = moved.y; nz = moved.z;                                      // 0061550A..1A
        } else if (hit.type_44 == 2) {                                                     // 00615522..2A
            Vector c4; vector_cross_004ab210(&c4, up(), &dir);                             // 00615530..42
            const double ex = static_cast<double>(c4.x) * k_wall_push_0074f5e8;           // 00615547..49 (ST)
            const float ey = static_cast<float>(static_cast<double>(c4.y) * k_wall_push_0074f5e8); // [S-1BC]
            const float ez = static_cast<float>(static_cast<double>(c4.z) * k_wall_push_0074f5e8); // [S-1B8]
            const double side = (static_cast<double>(dir.y) * hit.normal_10.x + -static_cast<double>(hit.normal_10.y) * dir.x) +
                                static_cast<double>(dir.z) * 0.0;                          // 00615572..A1
            if (side > 0.0) {                                                              // 006155AB..AE
                nx = static_cast<float>(pos->x - ex);                                      // 006155B0..B4
                ny = static_cast<float>(static_cast<double>(pos->y) - ey);
                nz = static_cast<float>(static_cast<double>(pos->z) - ez);
            } else {
                nx = static_cast<float>(ex + pos->x);                                      // 006155F9..FB
                ny = static_cast<float>(static_cast<double>(ey) + pos->y);
                nz = static_cast<float>(static_cast<double>(ez) + pos->z);
            }
        } else {                                                                           // 0061562C: project onto the hit plane
            const double ax = static_cast<double>(hit.point_4.x) - pos->x, ay = static_cast<double>(hit.point_4.y) - pos->y,
                         az = static_cast<double>(hit.point_4.z) - pos->z;
            const double bx = static_cast<double>(tgt.x) - pos->x, by = static_cast<double>(tgt.y) - pos->y;
            const float bz = static_cast<float>(static_cast<double>(tgt.z) - pos->z);     // [S-194]
            const double a2 = (az * az + ay * ay) + ax * ax;
            const double b2 = (static_cast<double>(bz) * bz + by * by) + bx * bx;          // 00615664..8E
            const Vector& nn = hit.normal_10;
            if (b2 < a2) {                                                                 // 0061569A..9F
                const double qx = static_cast<double>(hit.point_4.x) - tgt.x, qy = static_cast<double>(hit.point_4.y) - tgt.y,
                             qz = static_cast<double>(hit.point_4.z) - tgt.z;              // 006156A5..C2
                const double d = (static_cast<double>(nn.z) * qz + qy * nn.y) + qx * nn.x;  // 006156C6..E7
                const float py = static_cast<float>(d * nn.y), pz = static_cast<float>(static_cast<double>(nn.z) * d); // [S-18C] [S-188]
                nx = static_cast<float>(static_cast<double>(tgt.x) - static_cast<double>(nn.x) * d); // 00615712..18
                ny = static_cast<float>(static_cast<double>(tgt.y) - py);
                nz = static_cast<float>(static_cast<double>(tgt.z) - pz);
            } else {
                const double qx = static_cast<double>(tgt.x) - hit.point_4.x, qy = static_cast<double>(tgt.y) - hit.point_4.y,
                             qz = static_cast<double>(tgt.z) - hit.point_4.z;              // 0061574F..69
                const double d = (qz * nn.z + qy * nn.y) + qx * nn.x;                      // 00615770..8B
                const float py = static_cast<float>(d * nn.y), pz = static_cast<float>(static_cast<double>(nn.z) * d); // [S-180] [S-17C]
                nx = static_cast<float>(static_cast<double>(tgt.x) - static_cast<double>(nn.x) * d); // 006157B6..BA
                ny = static_cast<float>(static_cast<double>(tgt.y) - py);
                nz = static_cast<float>(static_cast<double>(tgt.z) - pz);
            }
        }
        tgt = Vector{nx, ny, nz};                                                          // 006157EE..0F
        prev_normal = hit.normal_10;                                                       // 006157F6..1E
        if (w5_cell_debug_008338c0 == 1)                                                   // 00615808..22
            kotor_unreachable_trap(0x00615824, "SetPrecalculatedPosition debug sprintf (008338C0 has no writer)");
        if (n <= 5) continue;                                                              // 0061587F..82
        break;
    }
    if (r != 1) return 0;                                                                  // 00615888..8B, 00615947
    w5_client_creature_compute_z_value_0060bf20(c, nullptr, area, &tgt, tgt.z, 0);         // 00615891..A7
    w5_server_update_subareas_on_move_to_004aead0(server_app(), nullptr, at<U>(c, 4), pos, &tgt); // 006158AC..BF
    vcall<void>(c, 0x8c, &tgt);                                                            // 006158C4..CD
    vec(c, 0x1dc) = tgt;                                                                   // 006158D3..EA
    w5_server_update_player_position_004aeaa0(server_app(), nullptr, at<U>(c, 4), &tgt, &vec(c, 0x30)); // 006158ED..02
    void* party = kotor_fastcall_005ed8b0(client_app(), nullptr);                         // 00615907..10
    const float yaw = static_cast<float>(kotor_m15a_yaw_004aa0f0(&vec(c, 0x30)));          // 0061591A..22
    w5_party_follow_process_point_00636a30(static_cast<unsigned char*>(party) + 0x2dc, nullptr, &tgt, yaw,
                                           bits(radius));                                  // 00615925..30
    return 1;
}
}  // extern "C"

extern "C" {
// 004B4CF0 CServerExoAppInternal::IsActionsPending(object id): the creature's action list (+FC) has a head node.
// Reached from IsDriveAnimationBlocked 0060F0F0 on the keyboard path; unowned until now (W4 pending stub).
U __fastcall w5_server_internal_is_actions_pending_004b4cf0(void* internal, void*, U id) {
    const U key = id == k_invalid ? k_invalid : (id & ~0x80000000u);                       // 004B4CF0..0C
    void* object = nullptr;
    if (static_cast<U8>(kotor_fastcall_004d8230(at<void*>(internal, 0x1005c), nullptr, key, &object)) != 0) return 0; // 004B4D0C..2B
    if (!object || at<U8>(object, 8) != 5) return 0;                                        // 004B4D2D..3E
    void* creature = vcall<void*>(object, 0x30);                                             // 004B4D40..42
    if (!creature) return 0;
    return at<void*>(at<void*>(creature, 0xfc), 0) != nullptr ? 1u : 0u;                     // 004B4D49..58
}
// 004AEAE0 CServerExoApp::IsActionsPending: mov ecx,[ecx+4]; jmp 004B4CF0 (name expected by W4)
U __fastcall kotor_fastcall_004aeae0(void* server, void*, U id) {
    return w5_server_internal_is_actions_pending_004b4cf0(at<void*>(server, 4), nullptr, id);
}
}
