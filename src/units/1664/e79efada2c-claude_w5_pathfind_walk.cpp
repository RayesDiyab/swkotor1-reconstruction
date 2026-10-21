// W5 queued-action bodies, pathfind-walk lane (lane worker pathfind-walk), main file.
// CSWSCreature::AIActionMoveToPoint 0051F4F0 (W1 runner 0057F4A0 dispatch) and its walk entry points, transcribed from
// the original swkotor.exe (read-only). Addresses in the comments are the original instructions.
//   Contract: original thiscall(CSWSObjectActionNode* node), RET 4, EAX = disposition
//             (.rdata 00745DBC = 1 in progress, 00745DC0 = 2 complete, 00745DC4 = 3 failed).
// Split files of this lane (same conventions, cross-file calls through kotor_fastcall_<addr> names):
//   claude_w5_pathfind_walk_quick.cpp    0051D9C0 WalkUpdateLocation_QuickWalk, 0051C0D0 ForcePushed, 004EB190
//   claude_w5_pathfind_walk_follow.cpp   0051C360 QuickWalk_FollowLeader + 11 sub-states, 00634E80, 00518940
//   claude_w5_pathfind_walk_distance.cpp 00516630 WalkUpdateLocationDistance, 00512FD0, 00512F10, 004CC1C0,
//                                        004F0F90, 004CC440
//   claude_w5_pathfind_walk_avoid.cpp    CAvoidCreature (00514AB0, 0050E270, 005D0840 ...)
// pathfind-plan owns CSWSModule::PlotPath 004C6F70 and the planning cascade (declared and called here only).
#include "842d6b1618-w5_t3.hpp"
#include "5889fde5f1-w5_action_bodies.hpp"
#include "4f622be2f2-w5_area_direct_line.hpp"
#include "ab3fc15780-w5_drive.hpp"
#include "73b9db80fe-w5_pathfind_reset.hpp"
#include "e5e3766b7c-w5_subarea.hpp"

using namespace w5;

extern "C" {
// ---------------- existing providers (c05 map), exact C-linkage names ----------------
void* __fastcall kotor_fastcall_004aede0(void* server, void*);                          // GetWorldTimer [0457]
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, U* day, U* time);       // CWorldTimer::GetWorldTime [0457]
U __fastcall recovered_event_time_compare_004adfa0(void* timer, void*, U d1, U t1, U d2, U t2); // CompareWorldTimes [1017]
U __fastcall kotor_fastcall_004adf50(void* timer, void*, U d1, U t1, U d2, U t2, U* out_day, U* out_time); // AddWorldTimes [w4]
void* __fastcall recovered_active_timer_004ae830(void* server, void*, U id);             // GetActiveTimer [W1 b7]
void __fastcall recovered_object_add_action_node_parameter_004cac30(void* object, void*, void* node, unsigned index,
    unsigned type, void* value);                                                          // AddActionNodeParameter [W1 b7]
void __fastcall kotor_m15e_slot008_void8_005b5e90(void* self, void*, U a, U b);          // 005B5E90 void return, RET 8 [0832]
void __fastcall kotor_m15e_slot090_void0_004015a0(void* self, void*);                    // 004015A0 void return (~CSWSForcedAction) [0832]
unsigned __fastcall recovered_door_is_linked_005890d0(void* door, void*);                // CSWSDoor::GetIsLinked [W1 b7]
void __fastcall recovered_trigger_nearest_point_0058c8a0(void* trigger, void*, const Vector* from, Vector* out); // CalculateNearestPoint [W1 b7]
U __fastcall kotor_m15b_c2da_get_float_entry_00413350(void* table, void*, I row, I column, float* out); // C2DA::GetFLOATEntry [0820]
extern unsigned kotor_rules_cell32_007a2120;                                              // .data 007A2120 appearance.2da RUNDIST column index [0431]
extern unsigned kotor_rules_cell32_007a212c;                                              // .data 007A212C appearance.2da WALKDIST column index [0431]
void* __fastcall frame_GetCharacter_006346c0(void* party, void*, U index);              // CSWParty::GetCharacter [0457]
unsigned __fastcall recovered_module_get_waypoint_004c6e00(void* module, void*, const void* tag); // CSWSModule::GetWaypoint [w7 t4]
void __fastcall exo_array_list_destruct_0056b6f0(void* list, void*);                     // CExoArrayList::~CExoArrayList [0464]
void __fastcall vector_normalize_004ab130(Vector* self, void*);                          // Vector::Normalize [0483]
int __fastcall area_clear_line_of_sight_0050c330(void* area, void*, void* a, void* b, void* c, unsigned* d,
    unsigned caller, unsigned target, int flag, int e);                                   // CSWSArea::ClearLineOfSight [0526]
Vector* __cdecl vector_component_mult_00423bf0(Vector* out, const Vector* v, float s);  // vectorComponentMult [0528]
void __cdecl unresolved_ftol2_006fae8c();                                                // __ftol2 (ST0 -> EAX) [0064]

// ---------------- other lanes / workers (declare and call only) ----------------
// pathfind-plan: CSWSModule::PlotPath(pathfind, time slice) RET 8, EAX disposition
U __fastcall kotor_fastcall_004c6f70(void* module, void*, void* pf, U slice);
// pathfind-plan: CSWSCreature::ComputeTotalWalkDistance RET 0, ST0
double __fastcall kotor_fastcall_005111c0(void* creature, void*);
// claude_w5_movement_actions.cpp shared helper: CPathfindInformation::ComputeStepTolerance RET 0
void __fastcall kotor_fastcall_005cec90(void* pf, void*);
// claude_w5_movement_actions.cpp exports used by ForceMoveToPoint (same bodies the W1 runner dispatches)
U __fastcall w5_action_jump_to_point_0051d600(void* self, void*, void* node);
U __fastcall w5_action_jump_to_object_0051d110(void* self, void*, void* node);

// ---------------- this lane, other files ----------------
U __fastcall kotor_fastcall_0051d9c0(void* creature, void*, U arg);                     // WalkUpdateLocation_QuickWalk RET 4 [quick]
U __fastcall kotor_fastcall_00516630(void* creature, void*, float dist, Vector* new_pos, Vector* dir, void* list); // WalkUpdateLocationDistance RET 10h [distance]
void __fastcall kotor_fastcall_004cc440(void* pf, void*, U id);                          // CPathfindInformation::SetBlockingCreature RET 4 [distance]

// ---------------- defined in this file ----------------
U __fastcall w5_action_move_to_point_0051f4f0(void* self, void*, void* node);           // 0051F4F0 RET 4
void __fastcall kotor_fastcall_004edba0(void* creature, void*, void* forced);            // 004EDBA0 ForceMoveToPoint RET 4 (EAX 1)
void* __fastcall kotor_fastcall_005034d0(void* forced, void*);                           // 005034D0 CSWSForcedAction ctor RET 0
void __fastcall kotor_fastcall_0050e250(void* pf, void*, const Vector* v);               // 0050E250 RET 4
U __fastcall kotor_fastcall_0051bb10(void* creature, void*);                             // 0051BB10 WalkUpdateLocation RET 0
void* __fastcall kotor_fastcall_004cac00(void* node, void*);                             // 004CAC00 CSWSObjectActionNode ctor RET 0
U __fastcall kotor_fastcall_0058c830(void* trigger, void*);                              // 0058C830 CSWSTrigger::GetTargetArea RET 0
}

namespace {
constexpr U k_in_progress_00745dbc = 1;           // .rdata 00745DBC
constexpr U k_complete_00745dc0 = 2;              // .rdata 00745DC0
constexpr U k_failed_00745dc4 = 3;                // .rdata 00745DC4
constexpr U k_event_queue_type_007441dc = 10;     // .rdata 007441DC
const float k_zero_0073d700 = 0.0f;               // .rdata 0073D700
const float k_thousand_0073d6fc = 1000.0f;        // .rdata 0073D6FC
const float k_four_0073f404 = 4.0f;               // .rdata 0073F404
const float k_two_fifths_0073e320 = 0.4f;         // .rdata 0073E320 (3ECCCCCD)
const float k_two_00741be0 = 2.0f;                // .rdata 00741BE0
const float k_thousandth_0073d708 = 0.001f;       // .rdata 0073D708 (3A83126F)
const float k_minus_thousandth_00747d80 = -0.001f; // .rdata 00747D80 (BA83126F)
const float k_hundredth_0073dfec = 0.01f;         // .rdata 0073DFEC (3C23D70A)
const float k_twenty_five_00745e5c = 25.0f;       // .rdata 00745E5C
const float k_one_0073d764 = 1.0f;                // .rdata 0073D764
const float k_tenth_0073e798 = 0.1f;              // .rdata 0073E798 (3DCCCCCD)
const float k_half_0073e9ac = 0.5f;               // .rdata 0073E9AC
const float k_micro_007439bc = 1.0e-6f;           // .rdata 007439BC (358637BD)
const float k_walk_scale_00747d90 = 0.0005f;      // .rdata 00747D90 (3A03126F)
const float k_unsigned_bias_0073d770 = 4294967296.0f; // .rdata 0073D770 (4F800000)
const double k_fast_walk_00747d88 = 15.0;         // .rdata 00747D88 (qword)
constexpr U k_half_bits = 0x3f000000u;            // 0.5 immediate (0051FA6B, 0051BE84)
constexpr U k_twenty_bits = 0x41a00000u;          // 20.0 immediate (0051FF39)
constexpr U k_one_bits = 0x3f800000u;             // 1.0 immediate (004EDBEF, 0051BBC4)
// CAIObject time slice: [00832824] is written only by staticInitAITimeSlice (0073B140) from .rdata 00745DCC = 1000
constexpr U k_ai_time_slice_00832824 = 1000;
// [0083280C] and [00832814] are .bss dwords that no instruction of the image writes (xref: reads only) -> 0
constexpr U k_cell_0083280c = 0;
constexpr U k_cell_00832814 = 0;
const char k_force_timeout_00747db8[] = "Force Timeout, executing a forced move to point.";
const char k_no_legs_00747e5c[] = "I have no legs! I can't feel my legs!!!!";
const char k_unsafe_00747e34[] = "Bailed the desired position is unsafe.";
const char k_path_failed_00747e10[] = "The Path find has Failed... Why?";

inline void* as_object(void* o) { return vcall<void*>(o, 0x10); }      // vtbl+10 AsSWSObject
inline void* as_door(void* o) { return vcall<void*>(o, 0x18); }        // vtbl+18 AsSWSDoor
inline void* as_creature(void* o) { return vcall<void*>(o, 0x30); }    // vtbl+30 AsSWSCreature
inline void* as_trigger(void* o) { return vcall<void*>(o, 0x40); }     // vtbl+40 AsSWSTrigger
inline void* as_placeable(void* o) { return vcall<void*>(o, 0x48); }   // vtbl+48 AsSWSPlaceable
inline void* as_waypoint(void* o) { return vcall<void*>(o, 0x58); }    // vtbl+58 AsSWSWaypoint
inline void set_animation(void* c, U anim) { vcall<void>(c, 0x7c, anim); }   // vtbl+7C CSWSCreature::SetAnimation
inline U get_dead(void* c) { return vcall<U>(c, 0x94); }                // vtbl+94 CSWSCreature::GetDead
inline U addr(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline Vector vec(U x, U y, U z) { Vector v; std::memcpy(&v.x, &x, 4); std::memcpy(&v.y, &y, 4); std::memcpy(&v.z, &z, 4); return v; }
inline unsigned char* bytes(void* p) { return static_cast<unsigned char*>(p); }
inline Vector* vat(void* p, U off) { return reinterpret_cast<Vector*>(bytes(p) + off); }
inline bool lookup(U id, void** out) { return static_cast<unsigned char>(kotor_fastcall_004d8230(object_array(), nullptr, id, out)) == 0; }
inline void* pathfind(void* c) { return at<void*>(c, 0x340); }
inline void* server_internal() { return at<void*>(server_app(), 4); }
inline double sq(float f) { return static_cast<double>(f) * f; }
inline double len_sq(const Vector& v) { return (sq(v.z) + sq(v.y)) + sq(v.x); }   // fld z*z, + y*y, + x*x
inline U ftol2(double v) { U r; __asm { fld qword ptr v } __asm { call unresolved_ftol2_006fae8c } __asm { mov r, eax } return r; }
inline double x87_fsqrt(double v) { double r; __asm { fld qword ptr v } __asm { fsqrt } __asm { fstp qword ptr r } return r; }
// fld dword / fstp dword copy (x87 load-store of a float; quiets a signalling NaN exactly like the original)
inline void x87_copy(void* dst, const void* src) {
    __asm { mov eax, src } __asm { fld dword ptr [eax] } __asm { mov eax, dst } __asm { fstp dword ptr [eax] }
}
inline U unsigned_ms(void* c) { return at<U>(c, 0xcc); }
// fild dword [+CC]; jge; fadd 2^32 (unsigned milliseconds as a wide value)
inline double ms_wide(void* c) {
    double v = static_cast<double>(at<I>(c, 0xcc));
    if (at<I>(c, 0xcc) < 0) v = v + k_unsigned_bias_0073d770;
    return v;
}
inline void* new_script_event(U16 type) {                              // operator new(34h) + CScriptEvent ctor; type at +0
    void* memory = live_operator_new(0x34);
    void* event = memory ? recovered_event_construct_004d7540(memory, nullptr) : nullptr;
    at<U16>(event, 0) = type;
    return event;
}
inline void ai_event(U caller, U target, void* event) {               // AddEventDeltaTime(0, 0, caller, target, 10, event)
    recovered_event_enqueue_004b08d0(ai_master(), nullptr, 0, 0, caller, target, k_event_queue_type_007441dc, event);
}
inline U group_of(void* node) { return at<U16>(node, 0x6c); }
// AddMoveToPointActionToFront(group, point, area, target, run, range, 0.0, a7..a12, &offset, a14, a15, a16)
struct MoveArgs {
    U run, range, bit1, p54, b4_6, b7_8, bit3, bit9, day, time;
    Vector* offset;
};
inline void move_to_front(void* self, void* node, const Vector* point, U area_id, U target, const MoveArgs& m,
                          U range_bits, U a12, U a16) {
    w5_creature_add_move_to_point_action_to_front_004f8a50(self, nullptr, group_of(node), point, area_id, target, m.run,
        flt(range_bits), 0.0f, m.bit1, m.p54, m.b4_6, m.b7_8, m.bit3, a12, m.offset, m.day, m.time, a16);
}
}  // namespace

extern "C" {

// =========================================================================================================
// 0051F4F0 CSWSCreature::AIActionMoveToPoint(node)
//   node +38/+3C/+40 point, +44 area id, [+48 target (5+)], [+4C flags (6+)], [+50 range (7+)], [+54 (8+)],
//   [+58 timeout seconds (9+)], [+5C/+60 follow offset (10+)], [+64/+68 forced-move deadline day/time (12+)]
U __fastcall w5_action_move_to_point_0051f4f0(void* self, void*, void* node) {
    if (get_dead(self) == 1) return k_failed_00745dc4;                                      // 0051F50E..1C
    if (kotor_m15e_slot010_zero_0063e7f0(self, nullptr) == 1) return k_failed_00745dc4;    // 0051F522..2C
    if (recovered_creature_incapacitated_004ef890(self, nullptr) == 1) return k_failed_00745dc4; // 0051F532..3C
    if (at<U>(self, 0x1f8) != 0) {                                                           // 0051F542..52
        const U state = at<U>(self, 0xa8c);
        if (state == 0 || state == 4 || state == 5 || state == 6) return k_in_progress_00745dbc; // 0051F554..6C
    } else {
        at<U>(self, 0xa8c) = 0xffffffffu;                                                     // 0051F571
    }
    void* pf = pathfind(self);                                                                // 0051F57B
    if (pf == nullptr) return k_failed_00745dc4;                                             // 0051F581..83
    const U node_x = at<U>(node, 0x38), node_y = at<U>(node, 0x3c), node_z = at<U>(node, 0x40); // E-AC, E-B4, E-B0
    Vector goal = vec(node_x, node_y, node_z);                                               // E-F8 (0051F5AE..CB)
    const U area_id = at<U>(node, 0x44);                                                     // E-EC
    const U16 count = at<U16>(node, 0x6e);                                                   // DI
    U deadline_day = 0, deadline_time = 0;                                                   // E-104, E-100 (0051F5DE..E2)
    Vector offset = vec(0, 0, 0);                                                            // E-D0 (0051F5E6..F6)
    U target = count >= 5 ? at<U>(node, 0x48) : k_object_invalid;                            // E-128 (0051F5FE..09)
    const U flags = count >= 6 ? at<U>(node, 0x4c) : 0;                                      // 0051F611..17
    MoveArgs m;
    m.run = (flags & 1) != 0 ? 1u : 0u;                                                      // E-E8 (test al,1; setg)
    m.b4_6 = static_cast<U>(static_cast<I>(flags) >> 4) & 7;                                 // EDI
    m.b7_8 = static_cast<U>(static_cast<I>(flags) >> 7) & 3;                                 // EBP
    m.bit9 = static_cast<U>(static_cast<I>(flags) >> 9) & 1;                                 // EBX
    m.bit1 = static_cast<U>(static_cast<I>(flags) >> 1) & 1;                                 // E-E4
    const U bit2 = static_cast<U>(static_cast<I>(flags) >> 2) & 1;                           // E-10C
    const U bit10 = static_cast<U>(static_cast<I>(flags) >> 10) & 1;                         // E-B8
    at<U>(pf, 0x240) = bit10;                                                                // 0051F660
    m.bit3 = static_cast<U>(static_cast<I>(flags) >> 3) & 1;                                 // E-DC
    if (at<U>(self, 0x8e8) == 1) m.run = 0;                                                  // 0051F666..81
    m.offset = &offset;
    Vector to_point = vec(0, 0, 0);                                                          // E-124
    Vector second;                                                                           // E-118
    U range = 0;                                                                             // E-108
    if (count >= 7) {                                                                        // 0051F689..95
        range = at<U>(node, 0x50);                                                           // 0051F69D / 0051F6B1
        kotor_cdecl_00423bc0(&second, &goal, vat(self, 0x90));                               // 0051F6A0..B5
        const double dist_sq = len_sq(second);                                              // 0051F6BA..EF
        to_point = vec(0, 0, 0);                                                             // 0051F6C9..DD
        const double r2 = sq(flt(range));                                                   // 0051F6F1..F5
        if (r2 > dist_sq) {                                                                  // fcompp; test ah,41h; jne keep
            unsigned scratch = 0;                                                            // E-D4 (written by the callee)
            if (area_clear_line_of_sight_0050c330(recovered_object_area_004cb120(self, nullptr), nullptr, vat(self, 0x90),
                    &goal, &to_point, &scratch, k_object_invalid, k_object_invalid, 0, 0) == 0) // 0051F702..36
                range = 0;                                                                   // 0051F738
        }
    } else {
        range = 0;                                                                           // 0051F738
    }
    m.range = range;
    m.p54 = count >= 8 ? at<U>(node, 0x54) : 0;                                              // E-FC (0051F74B..5A)
    const U timeout = count >= 9 ? at<U>(node, 0x58) : 0;                                    // E-E0 (0051F762..71)
    if (count >= 10) { offset.x = at<float>(node, 0x5c); offset.y = at<float>(node, 0x60); } // 0051F779..89 (mov)
    if (count >= 12) { deadline_day = at<U>(node, 0x64); deadline_time = at<U>(node, 0x68); } // 0051F78D..9D
    U now_day, now_time;                                                                     // E-A8, E-A4
    if (deadline_day != 0 && deadline_time != 0) {                                           // 0051F7A1..B3 (jbe)
        frame_GetWorldTime_004ade40(kotor_fastcall_004aede0(server_app(), nullptr), nullptr, &now_day, &now_time); // 0051F7B9..D8
        if (static_cast<I>(recovered_event_time_compare_004adfa0(kotor_fastcall_004aede0(server_app(), nullptr), nullptr,
                now_day, now_time, deadline_day, deadline_time)) >= 0) {                    // 0051F7DD..0E (jl)
            kotor_m15e_slot008_void8_005b5e90(pathfind(self), nullptr, 0, addr(k_force_timeout_00747db8)); // 0051F814..21
            alignas(4) unsigned char forced[0x1c];                                           // CSWSForcedAction E-7C
            kotor_fastcall_005034d0(forced, nullptr);                                        // 0051F826..2D
            at<U>(forced, 8) = area_id;                                                      // 0051F83E (E-74)
            at<U>(forced, 0x10) = bits(goal.y);                                              // 0051F849 (E-6C)
            at<U>(forced, 0xc) = bits(goal.x);                                               // 0051F850 (E-70)
            at<U>(forced, 0x14) = bits(goal.z);                                              // 0051F862 (E-68)
            at<U>(forced, 0x18) = target;                                                    // 0051F877 (E-64)
            at<U>(self, 0xa8c) = 1;                                                          // 0051F87E
            kotor_fastcall_004edba0(self, nullptr, forced);                                  // 0051F869..88
            kotor_m15e_slot090_void0_004015a0(forced, nullptr);                              // 0051F893..A5 (~CSWSForcedAction)
            return k_complete_00745dc0;                                                      // 0051F88D / 0051F8AA
        }
    }
    m.day = deadline_day; m.time = deadline_time;
    // 0051F8B1..9C0: flag bit 2 with a positive timeout re-queues the move with a deadline of now + timeout
    if (bit2 == 1 && flt(timeout) > k_zero_0073d700) {                                       // fcomp 0; test ah,41h; jne
        frame_GetWorldTime_004ade40(kotor_fastcall_004aede0(server_app(), nullptr), nullptr, &now_day, &now_time); // 0051F8D1..F1
        const U ms = ftol2(static_cast<double>(flt(timeout)) * k_thousand_0073d6fc);        // 0051F8F6..17
        kotor_fastcall_004adf50(kotor_fastcall_004aede0(server_app(), nullptr), nullptr, now_day, now_time, 0, ms,
                                &deadline_day, &deadline_time);                              // 0051F91C..3A
        m.day = deadline_day; m.time = deadline_time;
        move_to_front(self, node, &goal, area_id, target, m, m.range, m.bit9, bit10);        // 0051F93F..94
        at<U>(pathfind(self), 0x26c) = 0; at<U>(pathfind(self), 0x270) = 0;                 // 0051F999..A5
        at<U>(pathfind(self), 0x274) = 0;                                                    // 0051F9AB
        at<U>(self, 0xa8c) = 1;                                                              // 0051F9B1
        return k_complete_00745dc0;
    }
    // 0051F9C5..A03: target object by type (5 creature, 7 trigger, 9 placeable, 10 door; table 00520968)
    void* obj = nullptr;                                                                     // E-D8
    if (lookup(target, &obj)) {
        switch (at<U8>(obj, 8)) {
        case 7: {                                                                            // 0051FA0A trigger
            to_point = vec(0, 0, 0);                                                         // 0051FA0A..1A
            kotor_fastcall_0050e250(pathfind(self), nullptr, vat(as_trigger(obj), 0x90));    // 0051FA22..33
            if (at<U>(as_trigger(obj), 0x2bc) != 0) {                                        // 0051FA38..49
                recovered_trigger_nearest_point_0058c8a0(as_trigger(obj), nullptr, vat(self, 0x90), &to_point); // 0051FA4F..66
                Vector away;                                                                 // E-24
                kotor_cdecl_00423bc0(&away, vat(self, 0x90), &to_point);                     // 0051FA6B..84
                Vector n;                                                                    // E-18
                normalize_004ab180(&n, &away);                                               // 0051FA89..92
                Vector half;                                                                 // E-60
                vector_component_mult_00423bf0(&half, &n, flt(k_half_bits));                 // 0051FA9A..A3
                Vector sum;                                                                  // E-3C
                const Vector* s = vector_sum_00423b90(&sum, &to_point, &half);               // 0051FAA8..B6
                goal.x = s->x; goal.y = s->y; goal.z = s->z;                                 // 0051FABB..E2 (mov)
                move_to_front(self, node, &goal, area_id, k_object_invalid, m, m.range, m.bit9, 0); // 0051FAC0..0E -> 0051FFF0
                return k_complete_00745dc0;                                                  // 00520007
            }
            if (w5_creature_get_is_in_use_range_004f6000(self, nullptr, target, 0.0f, 0) == 0) break; // 0051FB13..25
            void* event = new_script_event(0x1e);                                            // 0051FB2B..55
            recovered_event_object_004d7780(event, nullptr, 0, at<U>(self, 4));              // 0051FB5A..6D
            ai_event(at<U>(self, 4), target, event);                                         // 0051FB72..96
            return k_complete_00745dc0;                                                      // 0051FB9B
        }
        case 5: {                                                                            // 0051FBA5 creature
            if (server_get_module_004ae6b0(server_app(), nullptr) == nullptr) break;        // 0051FBA5..B4
            void* creature = as_creature(obj);                                               // 0051FBBA..C0 (E-E0)
            Vector* creature_pos = vat(creature, 0x90);                                      // E-B8
            kotor_fastcall_0050e250(pathfind(self), nullptr, creature_pos);                  // 0051FBC3..DA
            if (recovered_object_area_004cb120(creature, nullptr) == nullptr) {              // 0051FBDF..EA
                second = *vat(creature, 0x314);                                              // 0051FBF0..1A (mov)
                const double dx = static_cast<double>(second.x) - goal.x;                    // 0051FC00..04
                const double dy = static_cast<double>(second.y) - goal.y;                    // 0051FC12..16
                const double dz = static_cast<double>(second.z) - goal.z;                    // 0051FC1E..28
                const U creature_area = at<U>(creature, 0x310);                              // 0051FC22
                if (area_id == creature_area) {                                              // 0051FC2C..32
                    const double d = (dz * dz + dy * dy) + dx * dx;                          // 0051FC34..42
                    if (!(d > static_cast<double>(k_four_0073f404))) break;                  // fcomp 4; test ah,41h; je
                }
                move_to_front(self, node, &second, creature_area, target, m, m.range, m.bit9, 0); // 0051FCAF..F9
                return k_complete_00745dc0;                                                  // 0051FCFE
            }
            const U creature_area_id = at<U>(recovered_object_area_004cb120(creature, nullptr), 0x120); // 0051FD05..29 (E-10C)
            second = *creature_pos;                                                          // 0051FD2D..44 (mov)
            if (!(offset.x == k_zero_0073d700) || !(offset.y == k_zero_0073d700)) {         // 0051FD0E..55 (test ah,44h; jp)
                void* party = kotor_fastcall_005ed8b0(client_app(), nullptr);                // 0051FD57..60
                to_point = *vat(party, 0x2c0);                                               // 0051FD65..83 (mov)
                Vector rotated;                                                              // E-54
                const Vector* r = quaternion_rotate_004a9bb0(&rotated,
                    bytes(kotor_fastcall_005ed8b0(client_app(), nullptr)) + 0x2cc, &offset); // 0051FD87..9F
                Vector sum;                                                                  // E-48
                const Vector* s = vector_sum_00423b90(&sum, &to_point, r);                   // 0051FDA4..B2
                second.x = s->x; second.y = s->y; second.z = s->z;                           // 0051FDB7..CA (mov)
                target = k_object_invalid;                                                   // 0051FDCE
            }
            Vector apart;                                                                    // E-30
            double reach = kotor_cdecl_004ab100(kotor_cdecl_00423bc0(&apart, vat(self, 0x90), &second)) *
                           k_two_fifths_0073e320;                                            // 0051FDD6..F5
            if (!(reach > static_cast<double>(k_two_00741be0))) reach = k_two_00741be0;    // fcom 2; test ah,41h; je
            Vector from_second;                                                              // E-C4
            from_second.x = static_cast<float>(static_cast<double>(goal.x) - second.x);     // 0051FE13..25
            from_second.y = static_cast<float>(static_cast<double>(goal.y) - second.y);     // 0051FE29..31
            from_second.z = static_cast<float>(static_cast<double>(goal.z) - second.z);     // 0051FE35..3D
            bool near_goal;
            if (creature_area_id != area_id) {                                               // 0051FE17..41
                near_goal = false;                                                           // -> 0051FF25
            } else {
                const double d = len_sq(from_second);                                       // 0051FE47..61
                near_goal = !(reach * reach < d);                                            // fcompp; test ah,5; jnp far
            }
            if (!near_goal) {                                                                // 0051FF27
                w1x_area_safe_location_004be860(recovered_object_area_004cb120(creature, nullptr), nullptr, addr(&second),
                    k_twenty_bits, addr(pathfind(self)), 1, addr(&second));                  // 0051FF27..4A
                kotor_cdecl_00423bc0(&to_point, creature_pos, &second);                      // 0051FF4F..61
                const double lhs = len_sq(from_second);                                     // 0051FF66..89
                const double rhs = len_sq(to_point);                                        // 0051FF8B..A5
                if (rhs < lhs) {                                                             // fcompp; test ah,5; jp near
                    move_to_front(self, node, &second, creature_area_id, target, m, m.range, m.bit9, 0); // 0051FFB4..02
                    return k_complete_00745dc0;                                              // 00520007
                }
            }
            // 0051FE76: the goal is within reach of the creature
            const U use_target = target;                                                     // EDI
            Vector use_pos = vec(0, 0, 0);                                                   // E-94
            float use_range;                                                                 // E-D4 (written by GetUseRange)
            w5_creature_get_use_range_004ee440(self, nullptr, use_target, &use_pos, &use_range, 0); // 0051FE76..AD
            if (offset.x == k_zero_0073d700 && offset.y == k_zero_0073d700) {               // 0051FEB2..D6 (test ah,44h; jp)
                const float extra = static_cast<float>(static_cast<double>(flt(m.range)) - use_range); // 0051FEDC..E9
                if (w5_creature_get_is_in_use_range_004f6000(self, nullptr, use_target, extra, 0) == 0) break; // 0051FEED..F4
                w5_pathfind_reset_at_end_of_path_005d0ec0(pathfind(self), nullptr);          // 0051FEFA..00
                set_animation(self, 0x2710);                                                 // 0051FF05..0E
                at<U>(self, 0xa8c) = 1;                                                      // 0051FF11
                return k_complete_00745dc0;
            }
            kotor_cdecl_00423bc0(&to_point, vat(self, 0x90), &second);                       // 00520011..22
            if (!(len_sq(to_point) < static_cast<double>(k_thousandth_0073d708))) break;    // 00520027..51 (test ah,5; jp)
            w5_pathfind_reset_at_end_of_path_005d0ec0(pathfind(self), nullptr);              // 00520057..5D
            at<U>(self, 0xa8c) = 1;                                                          // 0052006B
            set_animation(self, 0x2710);                                                     // 00520062..75
            return k_complete_00745dc0;
        }
        case 10: {                                                                           // 00520082 door
            void* door = as_door(obj);                                                       // 00520082..84 (E-10C)
            kotor_fastcall_0050e250(pathfind(self), nullptr, vat(door, 0x90));               // 00520087..97
            if (recovered_door_is_linked_005890d0(door, nullptr) != 0 &&                     // 0052009C..A7
                w5_creature_get_is_in_use_range_004f6000(self, nullptr, target, 0.0f, 0) != 0) { // 005200AD..BF
                if (at<U8>(door, 0x2cc) == 0) {                                              // 005200C5..D3
                    void* event = new_script_event(0x1f);                                    // 005200D5..FC
                    ai_event(target, at<U>(self, 4), event);                                 // 00520101..2F (caller = door, target = self)
                    return k_in_progress_00745dbc;                                           // 00520134
                }
                void* event = new_script_event(0x1e);                                        // 0052013E..66
                recovered_event_object_004d7780(event, nullptr, 0, at<U>(self, 4));          // 0052016B..7E
                ai_event(at<U>(self, 4), target, event);                                     // 00520183..A7
                set_animation(self, 0x2710);                                                 // 005201AC..B5
                w5_pathfind_reset_grid_search_005cefa0(pathfind(self), nullptr);            // 005201B8..BE
                w5_pathfind_reset_way_point_data_005cf060(pathfind(self), nullptr);         // 005201C3..C9
                w5_pathfind_reset_inter_tile_search_005d00a0(pathfind(self), nullptr);      // 005201CE..D4
                at<U>(self, 0xa8c) = 1;                                                      // 005201D9
                return k_complete_00745dc0;
            }
            if (m.bit9 != 0) break;                                                          // 005201ED..EF
            (void)as_door(obj);                                                              // 005201F5..FB (result unused)
            second = vec(0, 0, 0);                                                           // 00520210..18
            float use_range;                                                                 // E-10C (over the door pointer)
            w5_creature_get_use_range_004ee440(self, nullptr, target, &second, &use_range, 0); // 005201FE..1C
            if (flt(m.range) == use_range) {                                                 // 00520221..2E (test ah,44h; jp)
                Vector d;                                                                    // E-88
                d.x = static_cast<float>(static_cast<double>(second.x) - goal.x);           // 00520230..40
                d.y = static_cast<float>(static_cast<double>(second.y) - goal.y);           // 00520247..4F
                d.z = static_cast<float>(static_cast<double>(second.z) - goal.z);           // 00520256..5E
                if (!(kotor_cdecl_004ab100(&d) > static_cast<double>(k_thousandth_0073d708))) break; // 00520265..78
            }
            move_to_front(self, node, &second, area_id, target, m, bits(use_range), 1, 0);  // 0052027E..CD
            at<U>(self, 0xa8c) = 1;                                                          // 005202D2
            return k_complete_00745dc0;
        }
        case 9: {                                                                            // 005202E6 placeable
            kotor_fastcall_0050e250(pathfind(self), nullptr, vat(as_placeable(obj), 0x90));  // 005202E6..F7
            if (m.bit9 != 0) break;                                                          // 005202FC..FE
            (void)as_placeable(obj);                                                         // 00520304..0A (result unused)
            second = vec(0, 0, 0);                                                           // 00520317..1F
            float use_range;                                                                 // E-10C
            w5_creature_get_use_range_004ee440(self, nullptr, target, &second, &use_range, 0); // 0052030D..2B
            if (flt(m.range) == use_range) {                                                 // 00520330..3D
                Vector d;                                                                    // E-A0
                d.x = static_cast<float>(static_cast<double>(second.x) - goal.x);           // 0052033F..4F
                d.y = static_cast<float>(static_cast<double>(second.y) - goal.y);           // 00520356..5E
                d.z = static_cast<float>(static_cast<double>(second.z) - goal.z);           // 00520365..6D
                if (!(kotor_cdecl_004ab100(&d) > static_cast<double>(k_thousandth_0073d708))) break; // 00520374..87
            }
            move_to_front(self, node, &second, area_id, target, m, bits(use_range), 1, 0);  // 0052038D..D8
            at<U>(self, 0xa8c) = 1;                                                          // 005203DD
            return k_complete_00745dc0;
        }
        default:
            break;                                                                           // 0051F9FD (ja) / table 6, 8
        }
    }
    // 0051FC57: no usable target action
    if ((at<U8>(self, 0x9f0) & 2) == 0) {                                                   // 0051FC57..5E
        if (m.bit1 == 1) w5_creature_terminate_client_side_path_005110b0(self, nullptr, m.p54); // 0051FC64..72
        kotor_m15e_slot008_void8_005b5e90(pathfind(self), nullptr, 0, addr(k_no_legs_00747e5c)); // 0051FC77..84
        return k_failed_00745dc4;                                                            // 0051FC89
    }
    // 005203F1: walking creature
    if (recovered_object_area_004cb120(self, nullptr) == nullptr) return k_in_progress_00745dbc; // 005203F1..FA -> 00520134
    if (recovered_object_area_004cb120(self, nullptr) != nullptr &&                          // 00520400..09
        at<U>(recovered_object_area_004cb120(self, nullptr), 0x120) == area_id) {            // 0052040F..27
        kotor_cdecl_00423bc0(&to_point, &goal, vat(self, 0x90));                             // 0052042D..3E
        if (!(len_sq(to_point) > static_cast<double>(k_hundredth_0073dfec)) &&               // 00520443..6D (test ah,41h; jp)
            !(len_sq(to_point) != len_sq(to_point))) {
            if (w2x_area_safe_point_004be5e0(recovered_object_area_004cb120(self, nullptr), nullptr, &goal,
                                             addr(pathfind(self))) != 0)                    // 0052046F..8B
                recovered_object_position_004cd1a0(self, nullptr, &goal, 1, 1, 0);           // 0052048D..9A
            else
                kotor_m15e_slot008_void8_005b5e90(pathfind(self), nullptr, 0, addr(k_unsafe_00747e34)); // 005204A1..AE
            set_animation(self, 0x2711);                                                     // 005204B3..BC
            return k_complete_00745dc0;
        }
    }
    // 005204CD: plan (or continue) the path
    pf = pathfind(self);
    at<U>(pf, 0) = m.bit1 != 0 ? 1u : 0u;                                                    // 005204CD..E0
    at<U>(pathfind(self), 0x2c) = at<U>(self, 4);                                            // 005204E2..EB
    at<U>(pathfind(self), 0x30) = target;                                                    // 005204EE..F8
    pf = pathfind(self);
    at<U>(self, 0xa14) = 0;                                                                  // 00520501
    U goal_area;
    Vector cur;                                                                              // E-124 (x), E-120 (y), E-11C (z)
    if (at<U>(pf, 0x84) == 0) {                                                              // 00520507..0D
        x87_copy(&cur.y, bytes(pf) + 0x60); x87_copy(&cur.z, bytes(pf) + 0x64);              // 0052050F..1E
        cur.x = at<float>(pf, 0x5c); goal_area = at<U>(pf, 0x70);                            // 00520512 / 0052052E
    } else {
        x87_copy(&cur.y, bytes(pf) + 0x78); x87_copy(&cur.z, bytes(pf) + 0x7c);              // 00520537..46
        cur.x = at<float>(pf, 0x74); goal_area = at<U>(pf, 0x80);                            // 0052053A / 00520556
    }
    second.x = cur.x; second.y = cur.y; second.z = cur.z;                                    // E-118/-114/-110 (int moves)
    to_point = cur;
    bool set_goal = true;
    if (goal_area == area_id) {                                                              // 00520560 / 0052058B
        const double dx = static_cast<double>(flt(node_x)) - second.x;                      // 00520562..69
        const double dy = static_cast<double>(flt(node_y)) - second.y;                      // 00520571..7C
        const double dz = static_cast<double>(flt(node_z)) - second.z;                      // 00520580..87
        const double d = (dz * dz + dy * dy) + dx * dx;                                      // 0052058D..9B
        if (d < static_cast<double>(k_hundredth_0073dfec) || d != d) set_goal = false;       // fcomp; test ah,1; jne
    }
    if (set_goal) {                                                                          // 005205BC
        at<U>(pf, 0x5c) = node_x;                                                            // 005205BC..C3
        at<U>(pathfind(self), 0x60) = node_y;                                                // 005205C6..D3
        at<U>(pathfind(self), 0x64) = node_z;                                                // 005205D6..E3
        at<U>(pathfind(self), 0x70) = area_id;                                               // 005205E6..F0
        at<U>(pathfind(self), 0x68) = m.range;                                               // 005205F3..F9
        kotor_fastcall_005cec90(pathfind(self), nullptr);                                     // 005205FC
        x87_copy(bytes(pathfind(self)) + 0x74, bytes(self) + 0x90);                          // 00520601..0D
        x87_copy(bytes(pathfind(self)) + 0x78, bytes(self) + 0x94);                          // 00520610..1C
        x87_copy(bytes(pathfind(self)) + 0x7c, bytes(self) + 0x98);                          // 0052061F..2B
        at<U>(pathfind(self), 0x80) = at<U>(self, 0x8c);                                     // 0052062E..3A
        at<U>(pathfind(self), 0x58) = 1;                                                     // 00520640..46
        at<U>(pathfind(self), 0x88) = 0;                                                     // 0052064D..53
    }
    if (at<U>(pathfind(self), 0x88) != 0) {                                                  // 00520659..65 -> 005208ED
        set_animation(self, m.run != 0 ? 0x2714u : 0x2712u);                                 // 005208ED..08
        frame_GetWorldTime_004ade40(recovered_active_timer_004ae830(server_app(), nullptr, at<U>(self, 4)), nullptr,
                                    &at<U>(self, 0xb0), &at<U>(self, 0xb4));                 // 0052090B..2D
        w5_creature_activity_manager_004f87d0(self, nullptr, 8);                             // 00520932..36
        const U quick = at<U>(self, 0x1f8);                                                  // 0052093B
        at<U>(self, 0xa8c) = 0;                                                              // 00520941
        if (quick != 0) return k_in_progress_00745dbc;                                       // 00520947..51
        return kotor_fastcall_0051bb10(self, nullptr);                                       // 00520956..61
    }
    at<U>(pathfind(self), 0x198) = m.bit3;                                                   // 0052066B..6F
    at<U>(pathfind(self), 0x268) = 0;                                                        // 0052068C
    kotor_cdecl_00423bc0(&to_point, &goal, vat(self, 0x90));                                 // 0052067B..92
    if (at<U>(self, 0x50c) != k_object_invalid) {                                            // 00520697..A5
        if (len_sq(to_point) < static_cast<double>(k_twenty_five_00745e5c))                  // 005206A7..CE (test ah,5; jp)
            at<U>(pathfind(self), 0x194) = 1;                                                // 005206D0..D6
    }
    if (at<U>(self, 0x1f8) != 0) at<U>(self, 0xa8c) = 2;                                     // 005206E0..E8
    const U plotted = kotor_fastcall_004c6f70(server_get_module_004ae6b0(server_app(), nullptr), nullptr, pathfind(self),
                                              k_ai_time_slice_00832824);                     // 005206F2..0F
    at<U>(pathfind(self), 0x198) = 0;                                                        // 00520714..1C
    if (plotted == k_complete_00745dc0) {                                                    // 00520722..28
        const U blocker = at<U>(pathfind(self), 0x30);                                       // 0052072E..34
        at<U>(self, 0x508) = blocker;                                                        // 0052073C
        kotor_fastcall_004cc440(pathfind(self), nullptr, k_object_invalid);                  // 00520737..42
        if (blocker != target) at<U>(node, 0x48) = blocker;                                  // 00520747..54
        at<U>(pathfind(self), 0x28) = static_cast<U8>(m.p54);                                // 00520757..62 (movzx byte)
        at<U>(pathfind(self), 0x9c) = 0;                                                     // 00520769..6F
        if (m.run != 0) { set_animation(self, 0x2714); at<U>(self, 0xa98) = 1; }             // 00520765..8D
        else { set_animation(self, 0x2712); at<U>(self, 0xa98) = 0; }                        // 0052078F..99
        w5_creature_activity_manager_004f87d0(self, nullptr, 8);                             // 0052079F..A3
        U result;
        if (at<U>(self, 0x1f8) != 0) {                                                       // 005207A8..AE
            frame_GetWorldTime_004ade40(recovered_active_timer_004ae830(server_app(), nullptr, at<U>(self, 4)), nullptr,
                                        &at<U>(self, 0xb0), &at<U>(self, 0xb4));             // 005207B0..D2
            at<U>(self, 0xa8c) = 0;                                                          // 005207DB
            result = kotor_fastcall_0051d9c0(self, nullptr, 1);                              // 005207D7..E1
        } else {
            result = kotor_fastcall_0051bb10(self, nullptr);                                 // 005207E8..EA
        }
        (void)kotor_fastcall_004b2a50(server_internal(), nullptr, at<U>(self, 4));           // 005207EF..01 (result unused)
        return result;                                                                       // 00520806
    }
    if (plotted != k_failed_00745dc4) return plotted;                                        // 0052080D..13 -> 0052095F
    kotor_m15e_slot008_void8_005b5e90(pathfind(self), nullptr, 0, addr(k_path_failed_00747e10)); // 00520819..25
    if (at<U>(self, 0x1f8) != 0) at<U>(self, 0xa8c) = 2;                                     // 0052082A..32
    at<U>(pathfind(self), 0x28) = static_cast<U8>(m.p54);                                    // 0052083C..49
    set_animation(self, 0x2710);                                                             // 0052084C..55
    if (m.bit1 == 1) w5_creature_terminate_client_side_path_005110b0(self, nullptr, m.p54);  // 00520858..62 (dword pushed)
    at<U>(pathfind(self), 0x5c) = at<U>(self, 0x90);                                         // 00520867..70 (mov)
    x87_copy(bytes(pathfind(self)) + 0x60, bytes(self) + 0x94);                              // 00520873..7F
    x87_copy(bytes(pathfind(self)) + 0x64, bytes(self) + 0x98);                              // 00520882..8E
    at<U>(pathfind(self), 0x70) = at<U>(recovered_object_area_004cb120(self, nullptr), 0x120); // 00520891..A9
    if (k_cell_0083280c != 0) return plotted;                                                // 005208AC..B3
    if (deadline_day == 0 || deadline_time == 0) return plotted;                             // 005208B9..CB (jbe)
    recovered_object_position_004cd1a0(self, nullptr, &goal, 1, 1, 0);                       // 005208D1..DE
    return k_complete_00745dc0;                                                              // 005208E3
}

// =========================================================================================================
// 004EDBA0 CSWSCreature::ForceMoveToPoint(CSWSForcedAction*): runs JumpToPoint (no target) or JumpToObject on a
// temporary action node. forced +4 group word, +8 area id, +C/+10/+14 point, +18 target id.
void __fastcall kotor_fastcall_004edba0(void* creature, void*, void* forced) {
    void* memory = live_operator_new(0x74);                                                  // 004EDBBA..BE
    void* node = memory ? kotor_fastcall_004cac00(memory, nullptr) : nullptr;                // 004EDBC6..E1
    U area_id = 0;                                                                           // E-38
    U safe = 1;                                                                              // E-34 (004EDBE7)
    U radius = k_one_bits;                                                                   // E-30 (004EDBEF)
    U facing = 0;                                                                            // E-2C (004EDBF7)
    at<U>(node, 0x70) = 1;                                                                   // 004EDBFF
    if (at<U>(forced, 0x18) == k_object_invalid) {                                           // 004EDC06..15
        at<U>(node, 0) = 5;                                                                  // 004EDC1B
        at<U16>(node, 0x6c) = at<U16>(forced, 4);                                            // 004EDC21..25
        area_id = at<U>(forced, 8);                                                          // 004EDC29 / 004EDC3F
        Vector p = *vat(forced, 0xc);                                                        // E-24 (004EDC30..52)
        recovered_object_add_action_node_parameter_004cac30(creature, nullptr, node, 0, 2, &p.x); // 004EDC38..56
        p = *vat(forced, 0xc);                                                               // 004EDC5B..79
        recovered_object_add_action_node_parameter_004cac30(creature, nullptr, node, 1, 2, &p.y); // 004EDC69..7D
        Vector q = *vat(forced, 0xc);                                                        // E-18 (004EDC82..9E)
        recovered_object_add_action_node_parameter_004cac30(creature, nullptr, node, 2, 2, &q.z); // 004EDC8E..A2
        recovered_object_add_action_node_parameter_004cac30(creature, nullptr, node, 3, 3, &area_id); // 004EDCA7..B3
        recovered_object_add_action_node_parameter_004cac30(creature, nullptr, node, 4, 1, &safe); // 004EDCB8..C4
        recovered_object_add_action_node_parameter_004cac30(creature, nullptr, node, 5, 2, &radius); // 004EDCC9..D5
        recovered_object_add_action_node_parameter_004cac30(creature, nullptr, node, 6, 2, &facing); // 004EDCDA..E6
        (void)w5_action_jump_to_point_0051d600(creature, nullptr, node);                     // 004EDCEB..EE
    } else {
        at<U>(node, 0) = 0x30;                                                               // 004EDCF6
        at<U16>(node, 0x6c) = at<U16>(forced, 4);                                            // 004EDCFC..00
        area_id = at<U>(forced, 0x18);                                                       // 004EDD04 / 004EDD13 (E-38)
        recovered_object_add_action_node_parameter_004cac30(creature, nullptr, node, 0, 3, &area_id); // 004EDD07..17
        recovered_object_add_action_node_parameter_004cac30(creature, nullptr, node, 1, 1, &safe); // 004EDD1C..28
        (void)w5_action_jump_to_object_0051d110(creature, nullptr, node);                    // 004EDD2D..30
    }
    recovered_action_node_destroy_004cc4d0(node, nullptr);                                   // 004EDD35..37
    live_free(node);                                                                         // 004EDD3C..3D
    // EAX = 1 (004EDD4A)
}

// 005034D0 CSWSForcedAction::CSWSForcedAction: +0/+C/+10/+14 = 0, word +4 = 0, +8/+18 = OBJECT_INVALID
void* __fastcall kotor_fastcall_005034d0(void* forced, void*) {
    at<U>(forced, 0xc) = 0; at<U>(forced, 0x10) = 0; at<U>(forced, 0x14) = 0;               // 005034D4..DA
    at<U>(forced, 0) = 0; at<U16>(forced, 4) = 0;                                            // 005034DD..DF
    at<U>(forced, 8) = k_object_invalid; at<U>(forced, 0x18) = k_object_invalid;             // 005034E3..EB
    return forced;
}

// 0050E250 CPathfindInformation (unnamed): +248 = target position
void __fastcall kotor_fastcall_0050e250(void* pf, void*, const Vector* v) {
    at<U>(pf, 0x248) = at<U>(const_cast<Vector*>(v), 0);                                     // 0050E254..5C
    at<U>(pf, 0x24c) = at<U>(const_cast<Vector*>(v), 4);                                     // 0050E25E..61
    at<U>(pf, 0x250) = at<U>(const_cast<Vector*>(v), 8);                                     // 0050E264..67
}

// 004CAC00 CSWSObjectActionNode::CSWSObjectActionNode: parameter types +4..+34 and values +38..+68 zeroed, +6E = 0
void* __fastcall kotor_fastcall_004cac00(void* node, void*) {
    for (U i = 0; i < 13; ++i) {                                                             // 004CAC06..19
        at<U>(node, 4 + i * 4) = 0;                                                          // [edx-34h]
        at<U>(node, 0x38 + i * 4) = 0;                                                       // [edx]
    }
    at<U16>(node, 0x6e) = 0;                                                                 // 004CAC1B
    return node;
}

// 0058C830 CSWSTrigger::GetTargetArea: area id of the trigger's linked waypoint (+2B4 linked, +230 tag)
U __fastcall kotor_fastcall_0058c830(void* trigger, void*) {
    if (at<U>(trigger, 0x2b4) == 0) return k_object_invalid;                                 // 0058C831..39 -> 0058C895
    const U id = recovered_module_get_waypoint_004c6e00(server_get_module_004ae6b0(server_app(), nullptr), nullptr,
                                                        bytes(trigger) + 0x230);             // 0058C83B..53
    void* obj = trigger;                                                                     // [esp] (push ecx slot)
    if (!lookup(id, &obj)) return k_object_invalid;                                          // 0058C858..79
    return at<U>(recovered_object_area_004cb120(as_waypoint(obj), nullptr), 0x120);          // 0058C87B..90
}

// =========================================================================================================
// 0051BB10 CSWSCreature::WalkUpdateLocation: advance a normal (non quick-walk) creature along its path by one AI
// update (+CC = elapsed milliseconds, +B8 accumulated time, +D4 current animation).
U __fastcall kotor_fastcall_0051bb10(void* self, void*) {
    if (unsigned_ms(self) == 0) return k_in_progress_00745dbc;                               // 0051BB2C..4D
    float rate;                                                                              // E-60
    float table_dist;                                                                        // E-5C
    const U anim = at<U>(self, 0xd4);                                                        // 0051BB4E
    if (anim == 0x2712 || anim == 0x2713 || anim == 0x2714) {                                // 0051BB54..67
        U column;
        if (anim == 0x2714) {
            rate = static_cast<float>(w5_creature_get_run_rate_004f1be0(self, nullptr));     // 0051BB69..6B / 0051BB9F
            column = kotor_rules_cell32_007a2120;                                            // 0051BB7B
        } else {
            rate = static_cast<float>(w5_creature_get_walk_rate_004f1b20(self, nullptr));    // 0051BB83..85 / 0051BB9F
            column = kotor_rules_cell32_007a212c;                                            // 0051BB96
        }
        void* table = at<void*>(at<void*>(kotor_cell_rules_007a3a28, 0xb8), 0xc);           // 0051BB70..9C
        kotor_m15b_c2da_get_float_entry_00413350(table, nullptr, at<U16>(self, 0xa60), static_cast<I>(column),
                                                  &table_dist);                              // 0051BBA3..B3
    } else {
        // original quirk: E-60 and E-5C are read uninitialised for any other animation (stack garbage)
        rate = 0.0f; table_dist = 0.0f;
    }
    table_dist = static_cast<float>(static_cast<double>(table_dist) * k_thousand_0073d6fc); // 0051BBB8..CC (not read again)
    (void)table_dist;
    float cap = k_one_0073d764;                                                              // E-58 (0051BBC4)
    const double total = kotor_fastcall_005111c0(self, nullptr);                             // 0051BBD0
    const double scaled = static_cast<double>(rate) * k_walk_scale_00747d90;                 // 0051BBD5..D9
    double step;
    if (!(total > scaled) && !(total != total) && !(scaled != scaled)) {                     // fcomp; test ah,41h; jp
        const double q = total / scaled;                                                     // 0051BBEA
        double s;
        if (q < static_cast<double>(k_hundredth_0073dfec) || q != q) s = k_tenth_0073e798;  // 0051BBEC..FF (test ah,1)
        else s = x87_fsqrt(q);                                                               // 0051BBF9
        step = s - ms_wide(self) * k_thousandth_0073d708;                                    // 0051BC05..21
    } else {
        step = cap;                                                                          // 0051BC25..29
    }
    if (!(at<float>(self, 0xb8) > k_one_0073d764) && !(at<float>(self, 0xb8) != at<float>(self, 0xb8))) { // 0051BC2D..3E
        cap = static_cast<float>(ms_wide(self) * k_thousandth_0073d708 + at<float>(self, 0xb8)); // 0051BC40..62
        if (step >= static_cast<double>(cap)) step = cap;                                    // 0051BC66..73 (test ah,1; jne keep)
    }
    if (step <= static_cast<double>(k_tenth_0073e798)) step = k_tenth_0073e798;            // 0051BC77..86 (test ah,41h; jp keep)
    const double dist = (((step + at<float>(self, 0xb8)) * ms_wide(self)) * rate) * k_half_0073e9ac * k_micro_007439bc; // 0051BC8C..B6
    rate = static_cast<float>(dist);                                                         // 0051BCBC (E-60 = distance)
    at<float>(self, 0xb8) = static_cast<float>(step);                                        // 0051BCC0
    if (k_cell_00832814 != 0) rate = static_cast<float>(static_cast<double>(rate) * k_fast_walk_00747d88); // 0051BCC6..D9
    Vector new_pos = vec(0, 0, 0);                                                           // E-3C (0051BCDF..EF)
    Vector dir = vec(0, 0, 0);                                                               // E-48 (0051BCF7..07)
    struct { void* data; U size; U alloc; } list = {nullptr, 0, 0};                          // CExoArrayList E-54/-50/-4C
    const U path_index = at<U>(pathfind(self), 0x9c);                                        // 0051BD1B..21 (EDI)
    U moved = kotor_fastcall_00516630(self, nullptr, rate, &new_pos, &dir, &list);           // 0051BD27..41 (E-58)
    const double dz = static_cast<double>(new_pos.z) - at<float>(self, 0x98);               // 0051BD46..4A
    const bool climbing = (dz < static_cast<double>(k_minus_thousandth_00747d80) ||
                           dz == static_cast<double>(k_minus_thousandth_00747d80)) ||        // fcom -0.001; test ah,41h; jnp
                          (!(dz < static_cast<double>(k_thousandth_0073d708)) && !(dz != dz)); // fcom 0.001; test ah,1; jne
    if (climbing) {                                                                          // 0051BD6E
        Vector slope;                                                                        // E-30
        slope.x = rate; slope.y = 0.0f; slope.z = static_cast<float>(dz);                    // 0051BD6E..84
        Vector n;                                                                            // E-18
        const Vector* pn = normalize_004ab180(&n, &slope);                                   // 0051BD76..8C
        const float flat = static_cast<float>(static_cast<double>(rate) * pn->x);            // 0051BD91..A5 (E-24)
        at<U>(pathfind(self), 0x9c) = path_index;                                            // 0051BDB1
        moved = kotor_fastcall_00516630(self, nullptr, flat, &new_pos, &dir, &list);         // 0051BDA0..C5
    }
    w5_creature_auto_close_gui_panels_004eded0(self, nullptr, 1);                            // 0051BDCD..D1
    w5_creature_update_subareas_on_move_to_0051b7b0(self, nullptr, vat(self, 0x90), &new_pos, 1, &list, 0); // 0051BDD6..EC
    recovered_object_position_004cd1a0(self, nullptr, &new_pos, 1, 1, 0);                    // 0051BDF1..FD
    dir.z = 0.0f;                                                                            // 0051BE06
    vector_normalize_004ab130(&dir, nullptr);                                                // 0051BE02..0E
    w1x_object_set_orientation_004cd170(self, nullptr, &dir);                                // 0051BE13..1A
    if (at<U>(self, 0xa88) != 0 &&                                                           // 0051BE1F..25
        frame_GetCharacter_006346c0(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr, 0) != nullptr) { // 0051BE27..3F
        void* leader = frame_GetCharacter_006346c0(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr, 0); // 0051BE41..52
        if (at<U>(self, 4) == kotor_fastcall_004aea30(server_app(), nullptr, at<U>(leader, 4))) { // 0051BE57..6E
            void* party = kotor_fastcall_005ed8b0(client_app(), nullptr);                   // 0051BE70..7E
            const float yaw = static_cast<float>(kotor_m15a_yaw_004aa0f0(&dir));             // 0051BE80..8F
            w5_party_follow_process_point_00636a30(bytes(party) + 0x2dc, nullptr, &new_pos, yaw, k_half_bits); // 0051BE92..9D
        }
    }
    const U goal_id = at<U>(pathfind(self), 0x30);                                           // 0051BEA2..A8 (EBP)
    void* obj = kotor_fastcall_004ae750(server_app(), nullptr, goal_id);                     // 0051BEAB..B9
    U result = 0;
    bool done = false;
    if (obj != nullptr) {
        const U8 type = at<U8>(obj, 8);                                                      // 0051BEC3
        if (type == 7) {                                                                     // 0051BEC6..C8
            if (at<U>(as_trigger(obj), 0x2bc) == 0 &&                                        // 0051BECE..D9
                w5_creature_get_is_in_use_range_004f6000(self, nullptr, goal_id, 0.0f, 0) != 0) { // 0051BEDF..EB
                void* event = new_script_event(0x1e);                                        // 0051BEF1..15
                recovered_event_object_004d7780(event, nullptr, 0, at<U>(self, 4));          // 0051BF1A..25
                ai_event(at<U>(self, 4), goal_id, event);                                    // 0051BF2A..49
                set_animation(self, 0x2710);                                                 // 0051BF4E..57
                result = k_complete_00745dc0; done = true;                                   // 0051BF5A -> 0051C057
            }
        } else if (type == 0xa) {                                                            // 0051BF5F..61
            void* door = as_door(obj);                                                       // 0051BF67..6C
            if (recovered_door_is_linked_005890d0(door, nullptr) != 0 &&                     // 0051BF6E..77
                w5_creature_get_is_in_use_range_004f6000(self, nullptr, goal_id, 0.0f, 0) != 0) { // 0051BF7D..89
                if (at<U8>(door, 0x2cc) == 0) {                                              // 0051BF8F..97
                    void* event = new_script_event(0x1f);                                    // 0051BF99..BA
                    ai_event(goal_id, at<U>(self, 4), event);                                // 0051BFBF..E4 (caller = door, target = self)
                    result = k_in_progress_00745dbc;                                         // 0051BFE9
                } else {
                    void* event = new_script_event(0x1e);                                    // 0051BFF1..13
                    recovered_event_object_004d7780(event, nullptr, 0, at<U>(self, 4));      // 0051C018..23
                    ai_event(at<U>(self, 4), goal_id, event);                                // 0051C028..46
                    set_animation(self, 0x2710);                                             // 0051C04B..54
                    result = k_complete_00745dc0;                                            // 0051C057
                }
                done = true;
            }
        }
    }
    if (done) {
        exo_array_list_destruct_0056b6f0(&list, nullptr);                                    // 0051C05D..69
        return result;                                                                       // 0051C06E -> 0051C0B6
    }
    if (moved == 1) {                                                                        // 0051C070..75
        w5_pathfind_reset_at_end_of_path_005d0ec0(pathfind(self), nullptr);                  // 0051C077..7D
        set_animation(self, 0x2710);                                                         // 0051C082..8B
        result = k_complete_00745dc0;                                                        // 0051C092
    } else {
        result = k_in_progress_00745dbc;                                                     // 0051C09F
    }
    live_free(list.data);                                                                    // 0051C098 / 0051C0A5..AE
    return result;
}

}  // extern "C"
