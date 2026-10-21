// W5 pathfind-walk lane, part D: CSWSCreature::WalkUpdateLocation_QuickWalk 0051D9C0 and its private callees
// (WalkUpdateLocation_QuickWalk_ForcePushed 0051C0D0, GetActivity 004EB190). Transcribed from the original
// swkotor.exe (read-only); addresses in the comments are the original instructions.
//   0051D9C0: thiscall(U reset) RET 4, EAX = action disposition (.rdata 00745DBC = 1 in progress, 00745DC0 = 2 done)
//   0051C0D0: thiscall() RET 0, EAX = disposition
//   004EB190: thiscall(U mask) RET 4, EAX 0/1
// EH note: the original registers SEH frames (0071C2AA / 0071C258) whose unwind actions free the 34h-byte event
// allocations, the CSWSForcedAction (void dtor) and the CExoArrayList on a C++ throw. None of the callees on these
// paths throws, so the normal-path cleanup below is the complete observable behaviour.
#include "842d6b1618-w5_t3.hpp"
#include "5889fde5f1-w5_action_bodies.hpp"
#include "4f622be2f2-w5_area_direct_line.hpp"
#include "ab3fc15780-w5_drive.hpp"
#include "73b9db80fe-w5_pathfind_reset.hpp"
#include "e5e3766b7c-w5_subarea.hpp"

using namespace w5;

namespace kotor_list { struct Internal; }
namespace base_ini_reconstruction { void* __fastcall remove_head_005e9c80(kotor_list::Internal*, void*); }  // [0091]

extern "C" {
// ---------------- existing providers (c05 map), exact C-linkage names ----------------
void* __fastcall recovered_active_timer_004ae830(void* server, void*, U id);              // GetActiveTimer [w1 b7]
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, U* day, U* time);        // CWorldTimer::GetWorldTime [0457]
void* __fastcall kotor_fastcall_004aede0(void* server, void*);                           // GetWorldTimer [0457]
U __fastcall recovered_world_time_difference_004ae460(void* timer, void*, U day1, U time1, U day2, U time2,
                                                      U* out_day, U* out_time);          // SubtractWorldTimes [1025b]
I __fastcall recovered_event_time_compare_004adfa0(void* timer, void*, U day1, U time1, U day2, U time2); // CompareWorldTimes [1017]
void __fastcall slot_void_return_8_005b5e90_a2(void* self, void*, U a, const char* b);   // 005B5E90 void_return RET 8 (debug print) [0486]
void __fastcall kotor_m15e_slot090_void0_004015a0(void* self, void*);                    // 004015A0 void_return (~CSWSForcedAction) [0832]
void* __fastcall c2da_005e9d70(void* list, void*, void* item);                           // CExoLinkedListInternal::AddHead [0190]
void* __fastcall recovered_creature_client_creature_004efb20(void* creature, void*);     // GetClientCreature [w1 b7]
void* __fastcall frame_GetCharacter_006346c0(void* party, void*, U index);               // CSWParty::GetCharacter [0457]
void __fastcall kotor_fastcall_006359c0(void* party, void*);                             // CSWParty::RecaulateFollowPoint [W4 t1b]
U __fastcall kotor_fastcall_004aea40(void* server, void*);                               // GetPlayerCreatureId [0595]
void __fastcall exo_array_list_destruct_0056b6f0(void* list, void*);                     // CExoArrayList::~CExoArrayList [0464]
Vector* __cdecl float_times_vector_00437a60(Vector* out, float scale, const Vector* v);  // operator*(float, Vector) [0555]
void __fastcall vector_normalize_004ab130(Vector* self, void*);                          // Vector::Normalize [0483]

// ---------------- other workers of this lane (cross-file interface) ----------------
U __fastcall kotor_fastcall_0051c360(void* creature, void*);                             // B: QuickWalk_FollowLeader RET 0
void* __fastcall kotor_fastcall_00518940(void* creature, void*, U type);                 // B: GetAction RET 4
U __fastcall kotor_fastcall_00516630(void* creature, void*, float dist, Vector* pos, Vector* dir, void* list); // C: RET 10h
double __fastcall kotor_fastcall_00512f10(void* creature, void*, float speed, I dt);     // C: ComputeSpeedFactor RET 8, ST0
U __fastcall kotor_fastcall_004f0f90(void* creature, void*);                             // C: GetPauseReadyAnimation RET 0
void __fastcall kotor_fastcall_004edba0(void* creature, void*, void* forced_action);     // parent: ForceMoveToPoint RET 4
void* __fastcall kotor_fastcall_005034d0(void* forced_action, void*);                    // parent: CSWSForcedAction ctor RET 0

// ---------------- defined here ----------------
U __fastcall kotor_fastcall_004eb190(void* creature, void*, U mask);
U __fastcall kotor_fastcall_0051c0d0(void* creature, void*);
U __fastcall kotor_fastcall_0051d9c0(void* creature, void*, U reset);
}

namespace {
constexpr U k_in_progress_00745dbc = 1;           // .rdata 00745DBC
constexpr U k_complete_00745dc0 = 2;              // .rdata 00745DC0
constexpr U k_ai_event_script_007441dc = 10;      // .rdata 007441DC
constexpr U k_type_creature = 5, k_type_trigger = 7, k_type_door = 0xa;   // immediates (byte object type +8)
constexpr U k_cell_00832814 = 0;                  // .bss 00832814: read at 0051BCC6/0051DE6C only, no writer in the image
const float k_zero_0073d700 = 0.0f;               // .rdata 0073D700
const float k_milli_0073d708 = 0.001f;            // .rdata 0073D708 (3A83126F)
const float k_neg_milli_00747d80 = -0.001f;       // .rdata 00747D80 (BA83126F)
const float k_thousand_0073d6fc = 1000.0f;        // .rdata 0073D6FC
const float k_one_0073d764 = 1.0f;                // .rdata 0073D764
const float k_225_00747da4 = 225.0f;              // .rdata 00747DA4
const float k_0_9_00747da8 = 0.89999997615814208984375f;  // .rdata 00747DA8 (3F666666)
const float k_1_2_00747da0 = 1.2000000476837158203125f;   // .rdata 00747DA0 (3F99999A)
const float k_1_5_0073f414 = 1.5f;                // .rdata 0073F414
const float k_two32_0073d770 = 4294967296.0f;     // .rdata 0073D770 (unsigned fild fix-up)
const float k_half_0073e9ac = 0.5f;               // .rdata 0073E9AC
const float k_micro_007439bc = 9.999999974752427e-07f;    // .rdata 007439BC (358637BD)
const double k_fifteen_00747d88 = 15.0;           // .rdata 00747D88 (qword)
const float k_49_00747c3c = 49.0f;                // .rdata 00747C3C
const float k_four_0073f404 = 4.0f;               // .rdata 0073F404
const float k_neg_four_007471bc = -4.0f;          // .rdata 007471BC
const float k_25_00745e5c = 25.0f;                // .rdata 00745E5C
const float k_push_scale_00747d94 = 3.0303030014038086f;  // .rdata 00747D94 (4041F07C)
const char k_force_timeout_00747db8[] = "Force Timeout, executing a forced move to point."; // .rdata 00747DB8

inline void* as_creature(void* o) { return vcall<void*>(o, 0x30); }    // vtbl+30 AsSWSCreature
inline void* as_trigger(void* o) { return vcall<void*>(o, 0x40); }     // vtbl+40 AsSWSTrigger
inline void* as_door(void* o) { return vcall<void*>(o, 0x18); }        // vtbl+18 AsSWSDoor
inline unsigned char* bytes(void* p) { return static_cast<unsigned char*>(p); }
inline Vector* vat(void* p, U off) { return reinterpret_cast<Vector*>(bytes(p) + off); }
inline Vector vec(U x, U y, U z) { Vector v; std::memcpy(&v.x, &x, 4); std::memcpy(&v.y, &y, 4); std::memcpy(&v.z, &z, 4); return v; }
inline void* pathfind(void* c) { return at<void*>(c, 0x340); }
inline void* party() { return kotor_fastcall_005ed8b0(client_app(), nullptr); }
inline double sq(float f) { return static_cast<double>(f) * f; }
inline void set_animation(void* self, U animation) { vcall<void>(self, 0x7c, animation); }   // vtbl+7C SetAnimation
// fild dword + "jge; fadd 2^32": the unsigned value of a dword as the x87 sequence produces it
inline double fild_unsigned(U v) {
    double d = static_cast<double>(static_cast<I>(v));
    if (static_cast<I>(v) < 0) d = d + k_two32_0073d770;
    return d;
}
inline double x87_fsqrt(double v) { double r; __asm { fld qword ptr v } __asm { fsqrt } __asm { fstp qword ptr r } return r; }
// new CScriptEvent (operator new 34h + ctor, 0 when the allocation failed); type word written unconditionally
inline void* new_event(U16 type) {
    void* memory = live_operator_new(0x34);
    void* event = memory != nullptr ? recovered_event_construct_004d7540(memory, nullptr) : nullptr;
    at<U16>(event, 0) = type;              // original quirk: no null check (writes [0] when the allocation failed)
    return event;
}
// pop the head action node: the MoveToPoint node (type 1) is destroyed, any other node is put back
inline void drop_head_move_node(void* self, bool check_empty) {
    void* list = at<void*>(self, 0xfc);
    if (check_empty && *static_cast<void**>(list) == nullptr) return;
    void* node = base_ini_reconstruction::remove_head_005e9c80(static_cast<kotor_list::Internal*>(list), nullptr);
    if (at<U>(node, 0) == 1) {             // original quirk (0051DBF7): no null test when the queue was empty
        at<U>(self, 0x1e4) = 0;
        recovered_action_node_destroy_004cc4d0(node, nullptr);
        live_free(node);
        at<U>(self, 0xbc) = 0;
        at<U>(self, 0xc0) = 0;
    } else {
        c2da_005e9d70(at<void*>(self, 0xfc), nullptr, node);
    }
}
struct ArrayList { U data, count, alloc; };      // CExoArrayList<ulong> (12 bytes)
}  // namespace

extern "C" {

// =========================================================================================================
// 004EB190 CSWSCreature::GetActivity(mask): (+9FC & mask) != 0
U __fastcall kotor_fastcall_004eb190(void* creature, void*, U mask) {
    return (at<U>(creature, 0x9fc) & mask) != 0 ? 1u : 0u;                          // 004EB190..A0 (neg/sbb/neg)
}

// =========================================================================================================
// 0051C0D0 CSWSCreature::WalkUpdateLocation_QuickWalk_ForcePushed
//   +A8C 4 = fixed push (5 m at 25 m/s); otherwise the push length is the first path segment (pathfind +90 points)
//   +A90 travelled distance, +A94 height term, +D0 elapsed ms
U __fastcall kotor_fastcall_0051c0d0(void* self, void*) {
    U result = k_in_progress_00745dbc;                                              // 0051C0E9 (EBP)
    double speed;                                                                   // ST0
    float length;                                                                   // [esp+8] (E-40)
    if (at<U>(self, 0xa8c) == 4) {                                                  // 0051C0F2..F9
        speed = k_25_00745e5c;                                                      // 0051C0FB
        length = flt(0x40a00000u);                                                  // 0051C101 5.0
    } else {
        const float* p = at<const float*>(pathfind(self), 0x90);                    // 0051C10B..11
        const double dy = static_cast<double>(p[3]) - p[1];                         // 0051C117..22
        length = static_cast<float>(dy);                                            // 0051C128 (fstp dword)
        const double dx = static_cast<double>(p[2]) - p[0];                         // 0051C124..30 (ST0, unrounded)
        const double len = x87_fsqrt(dx * dx + sq(length));                         // 0051C136..44
        length = static_cast<float>(len);                                           // 0051C146 (fst dword)
        speed = len * k_push_scale_00747d94;                                        // 0051C14A (unrounded len)
    }
    const double step = speed * fild_unsigned(at<U>(self, 0xd0)) * k_milli_0073d708; // 0051C150..68
    const float step_f = static_cast<float>(step);                                  // 0051C16E [esp+10] (E-38)
    if (step == static_cast<double>(k_zero_0073d700)) return result;                // 0051C172..7D (jnp: equal only)
    Vector pos = vec(0, 0, 0);                                                      // [esp+20] (E-30)
    Vector dir = vec(0, 0, 0);                                                      // [esp+38] (E-18)
    ArrayList list = {0, 0, 0};                                                     // [esp+2C] (E-24)
    if (kotor_fastcall_00516630(self, nullptr, step_f, &pos, &dir, &list) == 0) {  // 0051C1C3..E4
        w5_creature_update_subareas_on_move_to_0051b7b0(self, nullptr, vat(self, 0x90), &pos, 1, &list, 0); // 0051C1EA..200
        const float travelled = at<float>(self, 0xa90);                             // 0051C205..0B [esp+14] (E-3C)
        const double t = static_cast<double>(travelled) / length;                   // 0051C20F..19
        const double a = static_cast<double>(k_zero_0073d700) * t;                  // 0051C223..29
        const double b = (a * t) * k_neg_four_007471bc;                             // 0051C22B..2F
        const float height = static_cast<float>(b + a * k_four_0073f404);           // 0051C235..3F [esp+1C] (E-34)
        const U id = at<U>(self, 4);                                                // 0051C220 (EDI)
        if (id == kotor_fastcall_004aea40(server_app(), nullptr)) {                 // 0051C245..4C
            const U radius = at<U>(pathfind(self), 4);                              // 0051C24E..5F (over E-40)
            void* p = party();                                                      // 0051C257..63 (EDI)
            const float yaw = static_cast<float>(kotor_m15a_yaw_004aa0f0(vat(self, 0x9c))); // 0051C26C..7B
            w5_party_follow_process_point_00636a30(bytes(p) + 0x2dc, nullptr, &pos, yaw, radius); // 0051C27E..89
        }
        recovered_object_position_004cd1a0(self, nullptr, &pos, 1, 1, 0);           // 0051C28E..9A
        at<float>(self, 0xa94) = height;                                            // 0051C2A7..AB
        at<float>(self, 0xa90) = static_cast<float>(static_cast<double>(travelled) + step_f); // 0051C29F..B1
    } else {
        const U id = at<U>(self, 4);                                                // 0051C2C1
        if (id == kotor_fastcall_004aea40(server_app(), nullptr)) {                 // 0051C2B9..CB
            const U radius = at<U>(pathfind(self), 4);                              // 0051C2CD..DC (over E-34)
            void* p = party();                                                      // 0051C2D6..E8
            const float yaw = static_cast<float>(kotor_m15a_yaw_004aa0f0(vat(self, 0x9c))); // 0051C2EE..FB
            w5_party_follow_process_point_00636a30(bytes(p) + 0x2dc, nullptr, &pos, yaw, radius); // 0051C2FE..09
        }
        recovered_object_position_004cd1a0(self, nullptr, &pos, 1, 1, 0);           // 0051C30E..1A
        at<U>(self, 0xa90) = 0;                                                     // 0051C31F
        result = k_complete_00745dc0;                                               // 0051C325
    }
    live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(list.data)));     // 0051C32B..38 (inline array dtor)
    return result;
}

// =========================================================================================================
// 0051D9C0 CSWSCreature::WalkUpdateLocation_QuickWalk(reset)
U __fastcall kotor_fastcall_0051d9c0(void* self, void*, U reset) {
    if (at<U>(self, 0x1f8) == 0) return k_complete_00745dc0;                       // 0051D9E0..EB
    if (at<U>(self, 0xa8c) == 1) return k_complete_00745dc0;                       // 0051D9F1..FA
    if (vcall<U>(self, 0x94) != 0) return k_complete_00745dc0;                     // 0051DA00..0A GetDead
    if (kotor_m15e_slot010_zero_0063e7f0(self, nullptr) != 0) return k_complete_00745dc0; // 0051DA10..19
    if (recovered_creature_incapacitated_004ef890(self, nullptr) != 0) return k_complete_00745dc0; // 0051DA1F..28
    U day = 0, time = 0, delta_days = 0;                                            // [esp+28], [esp+24], [esp+68]
    frame_GetWorldTime_004ade40(recovered_active_timer_004ae830(server_app(), nullptr, at<U>(self, 4)), nullptr,
                                &day, &time);                                       // 0051DA2E..58
    recovered_world_time_difference_004ae460(kotor_fastcall_004aede0(server_app(), nullptr), nullptr, day, time,
        at<U>(self, 0xb0), at<U>(self, 0xb4), &delta_days, &at<U>(self, 0xd0));     // 0051DA5D..90 (EBX = &+D0)
    if (reset != 0) {                                                               // 0051DA95..9C
        at<U>(self, 0xd0) = 0;                                                      // 0051DAA6
        at<U>(self, 0xb0) = day;                                                    // 0051DAA8
        at<U>(self, 0xb4) = time;                                                   // 0051DAAE
    }
    const U state = at<U>(self, 0xa8c);                                             // 0051DAB4
    if (state == 3) {                                                               // 0051DABA..BD
        const U r = kotor_fastcall_0051c360(self, nullptr);                         // 0051DABF..C1
        at<U>(self, 0xb0) = day; at<U>(self, 0xb4) = time;                          // 0051DAC6..D4
        return r;
    }
    if (state == 4 || state == 5 || state == 6) {                                   // 0051DADF..F4 -> 0051E663
        const U r = kotor_fastcall_0051c0d0(self, nullptr);                         // 0051E663..65
        at<U>(self, 0xb0) = day; at<U>(self, 0xb4) = time;                          // 0051E66A..78
        return r;
    }
    // ---- forced-move timeout (pathfind +26C armed, deadline +270/+274) ----
    if (at<U>(pathfind(self), 0x26c) != 0) {                                        // 0051DAFA..06
        U now_day, now_time;                                                        // [esp+10], [esp+14]
        frame_GetWorldTime_004ade40(kotor_fastcall_004aede0(server_app(), nullptr), nullptr, &now_day, &now_time); // 0051DB0C..26
        void* pf = pathfind(self);
        if (recovered_event_time_compare_004adfa0(kotor_fastcall_004aede0(server_app(), nullptr), nullptr, now_day,
                now_time, at<U>(pf, 0x270), at<U>(pf, 0x274)) >= 0) {              // 0051DB2B..60 (jl)
            slot_void_return_8_005b5e90_a2(pathfind(self), nullptr, 0, k_force_timeout_00747db8); // 0051DB66..72
            pf = pathfind(self);
            const U x = at<U>(pf, 0x5c);                                            // 0051DB80 [esp+2C]
            const float y = at<float>(pf, 0x60), z = at<float>(pf, 0x64);           // 0051DB7D..95 [esp+30], [esp+34]
            const U area_id = at<U>(pf, 0x70);                                      // 0051DB86 (EDI)
            alignas(4) unsigned char forced[0x1c];                                  // [esp+6C] CSWSForcedAction
            kotor_fastcall_005034d0(forced, nullptr);                               // 0051DB91..99
            at<float>(forced, 0x10) = y; at<float>(forced, 0x14) = z;               // 0051DB9E..B4
            at<U>(forced, 8) = area_id; at<U>(forced, 0xc) = x;                     // 0051DBBB..BF
            at<U>(forced, 0x18) = at<U>(pathfind(self), 0x30);                      // 0051DBAE..D4
            at<U>(self, 0xa8c) = 1;                                                 // 0051DBDB
            kotor_fastcall_004edba0(self, nullptr, forced);                         // 0051DBC6..E5
            drop_head_move_node(self, false);                                       // 0051DBEA..27
            set_animation(self, kotor_fastcall_004f0f90(self, nullptr));            // 0051DC2C..38
            pf = pathfind(self);
            at<U>(pf, 0x26c) = 0; at<U>(pf, 0x270) = 0; at<U>(pf, 0x274) = 0;      // 0051DC3B..4D
            kotor_m15e_slot090_void0_004015a0(forced, nullptr);                     // 0051DC59..68 (~CSWSForcedAction)
            return k_complete_00745dc0;
        }
    }
    if (at<U>(self, 0xd0) == 0) return k_in_progress_00745dbc;                      // 0051DC74..76 -> 0051DD04
    at<U>(self, 0xb0) = day; at<U>(self, 0xb4) = time;                              // 0051DC7C..8A
    void* follow_party = party();                                                   // 0051DC90..98 (EDI)
    // ---- speed ----
    double rate;                                                                    // ST0
    if (kotor_fastcall_004eb190(self, nullptr, 1) != 0) {                           // 0051DC9D..AA
        rate = w5_creature_get_walk_rate_004f1b20(self, nullptr);                   // 0051DCAC..AE
    } else {
        const U8 flags = at<U8>(self, 0x9f0);                                       // 0051DCBB
        if (at<U>(self, 0xa98) == 0) {                                              // 0051DCB5..C1
            if ((flags & 2) != 0) set_animation(self, 0x2712);                      // 0051DCC3..D0
            rate = w5_creature_get_walk_rate_004f1b20(self, nullptr);               // 0051DCD3..D5
        } else {
            if ((flags & 2) != 0) set_animation(self, 0x2714);                      // 0051DCDC..E9
            rate = w5_creature_get_run_rate_004f1be0(self, nullptr);                // 0051DCEC..EE
        }
    }
    float speed = static_cast<float>(rate);                                         // 0051DCF3 [esp+10] (fst)
    if (rate == static_cast<double>(k_zero_0073d700)) return k_in_progress_00745dbc; // 0051DCF7..02 (jp: not equal/NaN go on)
    if (at<U>(self, 0xa88) != 0 && kotor_fastcall_005188e0(self, nullptr, 0x3d) != 0) { // 0051DD0E..27
        const U client_id = at<U>(recovered_creature_client_creature_004efb20(self, nullptr), 4); // 0051DD2D..34
        const I index = static_cast<I>(recovered_party_index_006349c0(party(), nullptr, client_id)); // 0051DD37..48
        if (index >= 0) {                                                           // 0051DD4D..4F (jl)
            unsigned char* member = bytes(follow_party) + static_cast<U>(index) * 0x88 + 0x24; // 0051DD55..5B
            Vector d;                                                               // [esp+18]
            kotor_cdecl_00423bc0(&d, reinterpret_cast<Vector*>(member + 0x18), reinterpret_cast<Vector*>(member + 0x28)); // 0051DD5F..6C
            const double sum = (sq(d.z) + sq(d.y)) + sq(d.x);                       // 0051DD74..8E
            const float sum_f = static_cast<float>(sum);                            // 0051DD90 [esp+14] (fst)
            double scaled;                                                          // ST0
            if (!(sum >= static_cast<double>(k_milli_0073d708))) {                 // 0051DD94..9F (C0: less or NaN)
                scaled = speed;                                                     // 0051DE07
            } else {
                speed = at<float>(frame_GetCharacter_006346c0(party(), nullptr, 0), 0x3b0); // 0051DDA1..BF (over [esp+10])
                const double run = w5_creature_get_run_rate_004f1be0(self, nullptr); // 0051DDC3
                if (static_cast<double>(speed) * k_thousand_0073d6fc < run)         // 0051DDC8..D9 (test ah,5; jp)
                    scaled = w5_creature_get_run_rate_004f1be0(self, nullptr);      // 0051DDDB..DD
                else
                    scaled = static_cast<double>(at<float>(frame_GetCharacter_006346c0(party(), nullptr, 0), 0x3b0))
                             * k_thousand_0073d6fc;                                 // 0051DDE4..FF
            }
            if (sum_f <= k_one_0073d764) scaled = scaled * k_0_9_00747da8;          // 0051DE0B..1C (test ah,41h; jp)
            else if (sum_f <= k_225_00747da4) scaled = scaled * k_1_2_00747da0;     // 0051DE24..35
            else scaled = scaled * k_1_5_0073f414;                                  // 0051DE3D
            speed = static_cast<float>(scaled);                                     // 0051DE43
        }
    }
    const double factor = kotor_fastcall_00512f10(self, nullptr, speed, at<I>(self, 0xd0)); // 0051DE47..51 (ST0)
    double travel = (factor + at<float>(self, 0xb8)) * fild_unsigned(at<U>(self, 0xd0)); // 0051DE56..71
    travel = travel * speed;                                                        // 0051DE75
    travel = travel * k_half_0073e9ac;                                              // 0051DE79
    travel = travel * k_micro_007439bc;                                             // 0051DE7F
    float dist = static_cast<float>(travel);                                        // 0051DE85 [esp+10]
    if (k_cell_00832814 != 0) dist = static_cast<float>(static_cast<double>(dist) * k_fifteen_00747d88); // 0051DE6C..95
    at<float>(self, 0xb8) = static_cast<float>(factor);                             // 0051DE99
    // ---- move ----
    Vector pos = vec(0, 0, 0);                                                      // [esp+50] (E-50)
    Vector dir = vec(0, 0, 0);                                                      // [esp+5C] (E-44)
    ArrayList list = {0, 0, 0};                                                     // [esp+44] (E-5C)
    const U saved_9c = at<U>(pathfind(self), 0x9c);                                 // 0051DEDB..E1 (EDI)
    U moved = kotor_fastcall_00516630(self, nullptr, dist, &pos, &dir, &list);      // 0051DEEB..08
    const double dz = static_cast<double>(pos.z) - at<float>(self, 0x98);           // 0051DF0D..11
    if (dz <= static_cast<double>(k_neg_milli_00747d80) || dz >= static_cast<double>(k_milli_0073d708)) { // 0051DF1B..33
        Vector slope;                                                               // [esp+38] (E-68)
        slope.x = dist; slope.y = 0.0f; slope.z = static_cast<float>(dz);           // 0051DF35..4B
        Vector n;                                                                   // [esp+18]
        const Vector* nn = normalize_004ab180(&n, &slope);                          // 0051DF3D..53
        Vector step;                                                                // [esp+2C] (E-74)
        float_times_vector_00437a60(&step, dist, nn);                               // 0051DF58..5F
        at<U>(pathfind(self), 0x9c) = saved_9c;                                     // 0051DF64..6D
        moved = kotor_fastcall_00516630(self, nullptr, step.x, &pos, &dir, &list);  // 0051DF73..89 (horizontal part)
    }
    w5_creature_auto_close_gui_panels_004eded0(self, nullptr, 1);                   // 0051DF96..9A
    w5_creature_update_subareas_on_move_to_0051b7b0(self, nullptr, vat(self, 0x90), &pos, 1, &list, 0); // 0051DF9F..B5
    recovered_object_position_004cd1a0(self, nullptr, &pos, 1, 1, 0);               // 0051DFBA..C6
    if (at<U>(self, 0xa88) != 0) {                                                  // 0051DFCB..D3
        const U client_id = kotor_fastcall_005eda50(client_app(), nullptr, at<U>(self, 4)); // 0051DFD5..E2
        const I index = static_cast<I>(recovered_party_index_006349c0(party(), nullptr, client_id)); // 0051DFE7..F8
        if (index != -1) {                                                          // 0051DFFF..02
            unsigned char* member = bytes(party()) + static_cast<U>(index) * 0x88 + 0x24; // 0051E004..18
            if (member != nullptr) copy_vector(member + 0x5c, &pos);                // 0051E01C..34
        }
    }
    dir.z = 0.0f;                                                                   // 0051E03B
    vector_normalize_004ab130(&dir, nullptr);                                       // 0051E037..43
    w1x_object_set_orientation_004cd170(self, nullptr, &dir);                       // 0051E048..4F
    if (at<U>(self, 0xa88) != 0) {                                                  // 0051E054..5A
        if (frame_GetCharacter_006346c0(party(), nullptr, 0) != nullptr &&          // 0051E060..78
            at<U>(self, 4) == kotor_fastcall_004aea30(server_app(), nullptr,
                at<U>(frame_GetCharacter_006346c0(party(), nullptr, 0), 4))) {      // 0051E07A..A7
            void* p = party();                                                      // 0051E0A9..B7 (EDI)
            const float yaw = static_cast<float>(kotor_m15a_yaw_004aa0f0(&dir));    // 0051E0B9..C8
            w5_party_follow_process_point_00636a30(bytes(p) + 0x2dc, nullptr, &pos, yaw, 0x3f000000u); // 0051E0CB..D6 (0.5)
            void* cc = recovered_creature_client_creature_004efb20(self, nullptr);  // 0051E0DB..DD
            if (cc != nullptr) w5_client_creature_reset_camera_to_follow_0060b920(cc, nullptr); // 0051E0E2..E8
        }
        if (at<U>(self, 0xa88) != 0) {                                              // 0051E0ED..F3
            const U client_id = kotor_fastcall_005eda50(client_app(), nullptr, at<U>(self, 4)); // 0051E0F9..06
            const I index = static_cast<I>(recovered_party_index_006349c0(party(), nullptr, client_id)); // 0051E10B..1C
            if (index != -1) {                                                      // 0051E121..26
                void* p = party();                                                  // 0051E12C..35
                unsigned char* member = bytes(p) + static_cast<U>(index) * 0x88 + 0x24; // 0051E13A..49 (EDI)
                void* follow = bytes(p) + 0x2dc;                                    // 0051E14D (EBX)
                void* leader = kotor_fastcall_004ae770(server_app(), nullptr,
                    kotor_fastcall_004aea30(server_app(), nullptr, at<U>(p, 0x24))); // 0051E140..66
                if (leader != nullptr) {                                            // 0051E16B..6D
                    Vector tmp;                                                     // [esp+38]
                    const Vector* r = kotor_cdecl_00423bc0(&tmp, vat(leader, 0x90), reinterpret_cast<Vector*>(member + 0x74)); // 0051E173..82
                    Vector d = *r;                                                  // 0051E187..9E [esp+18]
                    if (kotor_fastcall_005188e0(self, nullptr, 0x3d) != 0 && moved == 0 && // 0051E196..B3
                        ((sq(d.z) + sq(d.y)) + sq(d.x) >= static_cast<double>(k_49_00747c3c))) { // 0051E1B9..E0 (C0)
                        kotor_fastcall_006359c0(party(), nullptr);                  // 0051E1E6..F6
                        const U path_index = at<U>(member, 0x34);                   // 0051E1FB (EBP)
                        Vector point = *reinterpret_cast<Vector*>(member + 0x28);   // 0051E1FE..14 [esp+18]
                        const U degrees = at<U>(kotor_fastcall_00634cc0(follow, nullptr, static_cast<int>(path_index)), 0xc); // 0051E20D..24
                        copy_vector(member + 0x18, &point);                         // 0051E228..3B
                        at<U>(member, 0x24) = path_index;                           // 0051E244
                        Vector axis = vec(0, 0, 0x3f800000u);                       // 0051E247..57 [esp+38]
                        const float radians = static_cast<float>(deg_to_rad_004a96d0(flt(degrees))); // 0051E23E..64
                        Quaternion q;                                               // [esp+6C]
                        Quaternion* qp = quaternion_construct_004ac960(&q, nullptr, axis, radians); // 0051E267..87
                        Vector rotated;                                             // [esp+88]
                        const Vector* rp = quaternion_rotate_004a9bb0(&rotated, qp, reinterpret_cast<Vector*>(member + 0x38)); // 0051E28C..95
                        Vector goal;                                                // [esp+2C]
                        vector_sum_00423b90(&goal, &point, rp);                     // 0051E29A..A5
                        goal.z = flt(w1x_area_height_bits_004bc380(recovered_object_area_004cb120(self, nullptr), nullptr,
                                                                   &goal, 1, nullptr)); // 0051E2AD..C4
                        bool safe = false;
                        Vector chosen;                                              // EDI, EBP, EBX
                        if (at<U>(member, 4) == 6 &&                                // 0051E2C8..CC
                            w2x_area_safe_point_004be5e0(recovered_object_area_004cb120(self, nullptr), nullptr, &goal,
                                static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(pathfind(self)))) != 0) { // 0051E2CE..EA
                            chosen = goal; safe = true;                             // 0051E2EC..F4
                        } else if (at<U>(member, 4) == 7 &&                         // 0051E2FA..FE
                            w2x_area_safe_point_004be5e0(recovered_object_area_004cb120(self, nullptr), nullptr, &point,
                                static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(pathfind(self)))) != 0) { // 0051E304..20
                            chosen = point; safe = true;                            // 0051E326..2E
                        }
                        if (safe) {
                            at<U>(self, 0xa8c) = 0xffffffffu;                       // 0051E338
                            copy_vector(bytes(pathfind(self)) + 0x248, &chosen);    // 0051E332..50
                            void* node = kotor_fastcall_00518940(self, nullptr, 1); // 0051E34C..53
                            if (node != nullptr) copy_vector(bytes(node) + 0x38, &chosen); // 0051E358..66
                        }
                        exo_array_list_destruct_0056b6f0(&list, nullptr);           // 0051E4D1..E6
                        return k_in_progress_00745dbc;
                    }
                }
            }
        }
    }
    // ---- arrival at the target object (pathfind +30) ----
    U result = k_in_progress_00745dbc;
    bool finish = true;                                                             // reaches 0051E5D5
    const U target = at<U>(pathfind(self), 0x30);                                   // 0051E36E..74 (EBP)
    void* obj = kotor_fastcall_004ae750(server_app(), nullptr, target);             // 0051E377..81
    if (obj != nullptr) {                                                           // 0051E388..8C
        const U type = at<U8>(obj, 8);                                              // 0051E392
        if (type == k_type_trigger) {                                               // 0051E395..97
            if (at<U>(as_trigger(obj), 0x2bc) == 0 &&                               // 0051E39D..A8
                w5_creature_get_is_in_use_range_004f6000(self, nullptr, target, 0.0f, 0) != 0) { // 0051E3AE..BA
                void* event = new_event(0x1e);                                      // 0051E3C0..DD / 0051E506..17
                recovered_event_object_004d7780(event, nullptr, 0, at<U>(self, 4)); // 0051E3E2..F1
                recovered_event_enqueue_004b08d0(ai_master(), nullptr, 0, 0, at<U>(self, 4), target,
                                                 k_ai_event_script_007441dc, event); // 0051E3F6..15
                set_animation(self, 0x2710);                                        // 0051E41A..23
                result = k_complete_00745dc0; finish = false;                       // 0051E426
            }
        } else if (type == k_type_door) {                                           // 0051E431..33
            void* door = as_door(obj);                                              // 0051E439..3E (EDI)
            if (w5_door_get_is_linked_005890d0(door, nullptr) != 0 &&               // 0051E440..49
                w5_creature_get_is_in_use_range_004f6000(self, nullptr, target, 0.0f, 0) != 0) { // 0051E44F..5B
                if (at<U8>(door, 0x2cc) == 0) {                                     // 0051E461..6B
                    void* event = new_event(0x1f);                                  // 0051E471..95
                    recovered_event_enqueue_004b08d0(ai_master(), nullptr, 0, 0, target, at<U>(self, 4),
                                                     k_ai_event_script_007441dc, event); // 0051E49A..C0
                    set_animation(self, 0x2710);                                    // 0051E4C5..CE
                    result = k_in_progress_00745dbc; finish = false;                // 0051E4D1
                } else {
                    void* event = new_event(0x1e);                                  // 0051E4F2..17
                    recovered_event_object_004d7780(event, nullptr, 0, at<U>(self, 4)); // 0051E3E2..F1
                    recovered_event_enqueue_004b08d0(ai_master(), nullptr, 0, 0, at<U>(self, 4), target,
                                                     k_ai_event_script_007441dc, event); // 0051E3F6..15
                    set_animation(self, 0x2710);                                    // 0051E41A..23
                    result = k_complete_00745dc0; finish = false;
                }
            }
        } else if (type == k_type_creature) {                                       // 0051E51C..1E
            const Vector p = *vat(as_creature(obj), 0x90);                          // 0051E524..39
            void* pf = pathfind(self);                                              // 0051E529
            const Vector last = *vat(pf, 0x248);                                    // 0051E53C..4B
            const double dz = static_cast<double>(p.z) - last.z;                    // 0051E54E..5A
            const double dy = static_cast<double>(p.y) - last.y;                    // 0051E55E..6A
            const double dx = static_cast<double>(p.x) - last.x;                    // 0051E56E..76
            if ((dx * dx + dy * dy) + dz * dz >= static_cast<double>(k_four_0073f404)) { // 0051E57A..9B (C0)
                at<U>(self, 0xa8c) = 0xffffffffu;                                   // 0051E59D..A2
                copy_vector(bytes(pf) + 0x248, &p);                                 // 0051E5A8..B3
                result = k_in_progress_00745dbc; finish = false;                    // 0051E5B6
            }
        }
    }
    if (finish) {                                                                   // 0051E5D5
        if (moved == 1 && at<U>(self, 0xa8c) != 2) {                                // 0051E5D5..ED
            w5_pathfind_reset_at_end_of_path_005d0ec0(pathfind(self), nullptr);     // 0051E5F3..F9
            at<U>(self, 0xa8c) = 1;                                                 // 0051E604
            drop_head_move_node(self, true);                                        // 0051E5FE..44
            set_animation(self, kotor_fastcall_004f0f90(self, nullptr));            // 0051E649..55
            result = k_complete_00745dc0;                                           // 0051E658
        } else {
            result = k_in_progress_00745dbc;                                        // 0051E4D1
        }
    }
    exo_array_list_destruct_0056b6f0(&list, nullptr);                               // 0051E4D7..E6
    return result;
}

}  // extern "C"
