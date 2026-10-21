// W5 pathfind-walk lane, fork B: party follow-the-leader quick walk (CSWSCreature::WalkUpdateLocation_QuickWalk_FollowLeader
// 0051C360 and its eleven sub-states), CSWPartyFollowDataArray::AdvancePointOnPath 00634E80 and CSWSCreature::GetAction
// 00518940. Transcribed from the original swkotor.exe (read-only); addresses in the comments are the original instructions.
//
// Conventions (as claude_w5_movement_actions.cpp): thiscall -> extern "C" __fastcall(self, unused_edx, ...), exact RET
// bytes; ST0 returns as double; x87 at 53-bit precision -> double intermediates, float exactly where the original stores a
// dword (fstp/fst dword); live_operator_new / live_free only; dispositions are the proven .rdata values
// ([00745DBC]=1 in progress, [00745DC0]=2 complete).
//
// Follow state (party + index*88h + 24h, "st" below): +4 state (-1..10), +8 follow distance, +18 path point, +24 its path
// index, +28 goal point, +34 goal index, +38 formation offset, +44 tick counter, +48 last speed, +4C facing, +58 offset
// ratio, +5C walked position, +68 reset-to-creature flag, +6C accumulated ms, +70 first-wait flag, +74 leader position.
// The follow-data array is party + 2DCh (CSWPartyFollowDataArray: GetPathData entries {pos, yaw degrees}, +FA4 head,
// +FA8 count).
#include "842d6b1618-w5_t3.hpp"
#include "5889fde5f1-w5_action_bodies.hpp"
#include "4f622be2f2-w5_area_direct_line.hpp"
#include "ab3fc15780-w5_drive.hpp"
#include <cmath>
#include <initializer_list>

using namespace w5;

extern "C" {
// ---------------- existing providers (c05 map), exact C-linkage names ----------------
void* __fastcall frame_GetCharacter_006346c0(void* party, void*, U index);               // CSWParty::GetCharacter [0457]
void* __fastcall recovered_creature_client_creature_004efb20(void* creature, void*);      // GetClientCreature [W1 b7]
double __fastcall kotor_fastcall_0060e250(void* client_creature, void*);                  // GetHeadTurnHorizontal [W4], ST0
U __fastcall kotor_fastcall_006146e0(void* client_creature, void*, U target, float radius); // CSWCCreature::LookAt [W4]
I __fastcall kotor_fastcall_006350f0(void* follow, void*, float distance, Vector* out, U* out_index, I member); // GetPointBackFromStart [W4]
void __fastcall kotor_fastcall_006359c0(void* party, void*);                              // CSWParty::RecaulateFollowPoint [W4]
const char* __fastcall WindowCStr005e5670(void* string, void*);                           // CExoString::CStr [0078]
extern U recovered_debug_0083281c;                // .bss 0083281C debug flag (no writer in the image) [action730]
extern float kotor_cell_drive_run_ratio_007a22f4; // .data 007A22F4 0.6 [W4 creature_ai]

// ---------------- exported here ----------------
U __fastcall kotor_fastcall_0051c360(void* creature, void*);                              // FollowLeader, RET 0
void* __fastcall kotor_fastcall_00518940(void* creature, void*, U type);                  // GetAction, RET 4
double __fastcall kotor_fastcall_00634e80(void* follow, void*, float distance, I unused_index, const Vector* from,
    I from_index, const Vector* target, I target_index, Vector* out, I* out_index);       // AdvancePointOnPath, RET 20h
U __fastcall kotor_fastcall_0051ac10(void* c, void*, void* st, void* follow, float* speed, I index);   // RET 10h
U __fastcall kotor_fastcall_00511290(void* c, void*, void* st, void* follow, float* speed, I index);
U __fastcall kotor_fastcall_00511860(void* c, void*, void* st, void* follow, float* speed, I index);
U __fastcall kotor_fastcall_00511b60(void* c, void*, void* st, void* follow, float* speed, I index);
U __fastcall kotor_fastcall_00512430(void* c, void*, void* st, void* follow, float* speed, I index);
U __fastcall kotor_fastcall_005127d0(void* c, void*, void* st, void* follow, float* speed, I index);
U __fastcall kotor_fastcall_00512a60(void* c, void*, void* st, void* follow, float* speed, I index);
U __fastcall kotor_fastcall_00512b40(void* c, void*, void* st, void* follow, float* speed, I index);
U __fastcall kotor_fastcall_00512c20(void* c, void*, void* st, void* follow, float* speed, I index);
U __fastcall kotor_fastcall_00512d00(void* c, void*, void* st, void* follow, float* speed, I index);
U __fastcall kotor_fastcall_00512de0(void* c, void*, void* st, void* follow, float* speed, I index);

// 00832808: written only by 0051C3F9..0051C420 (debug branch); rebuilt storage owned here
U w5_cell_follow_debug_index_00832808 = 0;
}

namespace {
constexpr U k_in_progress_00745dbc = 1;           // .rdata 00745DBC
constexpr U k_complete_00745dc0 = 2;              // .rdata 00745DC0
constexpr U k_event_queue_007441dc = 10;          // .rdata 007441DC
// 00832818: .bss dword read at 0051C5CF/0051C6AC, no writer anywhere in the image -> always 0
constexpr U k_cell_00832818 = 0;
// 007A1B58: .data float 0.5 (3F000000), no writer in the image (only reader 0051CA08)
constexpr float k_cell_half_007a1b58 = 0.5f;
const float k_zero_0073d700 = 0.0f;               // .rdata 0073D700
const float k_tenth_0073e798 = 0.1f;              // .rdata 0073E798
const float k_seven_tenths_00746f4c = 0.7f;       // .rdata 00746F4C (3F333333)
const float k_inv_seven_tenths_00747c38 = 1.42857146f; // .rdata 00747C38 (3FB6DB6E)
const float k_49_00747c3c = 49.0f;                // .rdata 00747C3C
const float k_five_00743264 = 5.0f;               // .rdata 00743264
const double k_sixteen_00747db0 = 16.0;           // .rdata 00747DB0 (qword)
const float k_100_00741838 = 100.0f;              // .rdata 00741838
const float k_1000_0073d6fc = 1000.0f;            // .rdata 0073D6FC
const float k_thousandth_0073d708 = 0.001f;       // .rdata 0073D708
const float k_nine_tenths_00747da8 = 0.9f;        // .rdata 00747DA8 (3F666666)
const float k_four_0073f404 = 4.0f;               // .rdata 0073F404
const float k_2pow32_0073d770 = 4294967296.0f;    // .rdata 0073D770 (fild unsigned fix-up)
const float k_one_0073d764 = 1.0f;                // .rdata 0073D764
const float k_225_00747da4 = 225.0f;              // .rdata 00747DA4
const float k_six_fifths_00747da0 = 1.20000005f;  // .rdata 00747DA0 (3F99999A)
const float k_three_halves_0073f414 = 1.5f;       // .rdata 0073F414
const float k_micro_007439bc = 9.99999997e-07f;   // .rdata 007439BC (358637BD)
const float k_deg_to_rad_007415f8 = 0.0174532924f; // .rdata 007415F8 (3C8EFA35)
const float k_million_00743750 = 1000000.0f;      // .rdata 00743750
const double k_ten_thousandth_00747d98 = 0.0001;  // .rdata 00747D98 (qword)
const float k_half_0073e9ac = 0.5f;               // .rdata 0073E9AC
const float k_ten_007455c8 = 10.0f;               // .rdata 007455C8

inline unsigned char* bytes(void* p) { return static_cast<unsigned char*>(p); }
inline Vector* vat(void* p, U off) { return reinterpret_cast<Vector*>(bytes(p) + off); }
inline U addr(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline Vector vec(U x, U y, U z) { Vector v; std::memcpy(&v.x, &x, 4); std::memcpy(&v.y, &y, 4); std::memcpy(&v.z, &z, 4); return v; }
inline Vector load(const void* p) { Vector v; std::memcpy(&v, p, 12); return v; }     // three DWORD moves
inline void* pathfind(void* c) { return at<void*>(c, 0x340); }
inline void* party_of(void* client) { return kotor_fastcall_005ed8b0(client, nullptr); }
inline void* path_data(void* follow, I index) { return kotor_fastcall_00634cc0(follow, nullptr, index); }
inline double fild_unsigned(U v) {                                    // fild dword; jge; fadd [0073D770] (2^32)
    double r = static_cast<double>(static_cast<I>(v));
    if (static_cast<I>(v) < 0) r = r + k_2pow32_0073d770;
    return r;
}
inline WalkHit blank_hit() {                                         // inline CWalkHitInfo init (zeros, blocker invalid)
    WalkHit h; std::memset(&h, 0, sizeof h); h.blocker_40 = k_object_invalid; return h;
}
// CSWSArea::TestDirectLine on the creature's area (GetArea each time, as the original)
inline I direct_line(void* c, const Vector* from, const Vector* to, const float* radius, float band, void* hit) {
    return w5_area_test_direct_line_004bcb70(recovered_object_area_004cb120(c, nullptr), nullptr, from, to, radius, band,
                                             0, hit);
}
// Quaternion(axis (0,0,1), (float)DegToRad(yaw)) * offset  (the repeated 004A96D0 / 004AC960 / 004A9BB0 sequence)
inline const Vector* rotate_by_yaw(Vector* out, U yaw_bits, const Vector* offset) {
    const Vector axis = vec(0, 0, 0x3f800000u);
    const float radians = static_cast<float>(deg_to_rad_004a96d0(flt(yaw_bits)));   // fstp dword over the argument slot
    Quaternion q;
    quaternion_construct_004ac960(&q, nullptr, axis, radians);
    return quaternion_rotate_004a9bb0(out, &q, offset);
}
// is the walk-hit blocker a party member (ServerToClientObjectId + GetSWParty + IsPartyMember on the client app)
inline U blocker_is_party_member(const WalkHit& hit) {
    void* client = client_app();
    const U id = kotor_fastcall_005eda50(client, nullptr, hit.blocker_40);
    return recovered_party_member_00634620(party_of(client), nullptr, id);
}
// operator new(34h) + CScriptEvent ctor, type 1Fh (bumped), AddEventDeltaTime(0, 0, caller, target, 10, event).
// No null test on the event before the type store (original quirk: a failed allocation faults at the store).
inline void queue_bump_event(U caller, U target) {
    void* memory = live_operator_new(0x34);
    void* event = nullptr;
    if (memory != nullptr) {
        try { event = recovered_event_construct_004d7540(memory, nullptr); }
        catch (...) { live_free(memory); throw; }                    // SEH state 0 cleanup
    }
    at<U16>(event, 0) = 0x1f;
    recovered_event_enqueue_004b08d0(ai_master(), nullptr, 0, 0, caller, target, k_event_queue_007441dc, event);
}
inline void front(void* self, U type, U group, std::initializer_list<U> list) {
    U pairs[26] = {};
    U i = 0;
    for (U v : list) pairs[i++] = v;
    w5_add_action_to_front_body(self, type, group, pairs);
}

// the four *WaitDest sub-states: scan the follow ring from st+24 to the goal index st+34 (stop at the head +FA4);
// when the goal is the current index, wait unless the point is closer to the goal than to the reached point
U wait_dest(void* st, void* follow, U next_state) {
    const I cur = at<I>(st, 0x24);                                                  // EDX
    const I goal = at<I>(st, 0x34);                                                 // EBX
    const I head = at<I>(follow, 0xfa4);                                            // EDI
    I i = cur;
    if (i == head) return k_complete_00745dc0;                                      // ..7A / ..90
    while (i != goal) {                                                             // ..80
        ++i;                                                                        // ..84
        if (i >= 100) i = 0;                                                        // ..85..8A
        if (i == head) return k_complete_00745dc0;                                  // ..8C..8E
    }
    if (goal == cur) {                                                              // ..9E..A0
        const Vector p = load(path_data(follow, cur));                              // ..A6..C7
        const double a0 = static_cast<double>(p.x) - at<float>(st, 0x18);           // ..B8..BC
        const double a1 = static_cast<double>(p.y) - at<float>(st, 0x1c);
        const double a2 = static_cast<double>(p.z) - at<float>(st, 0x20);
        const double b0 = static_cast<double>(p.x) - at<float>(st, 0x28);           // ..D5..E7
        const double b1 = static_cast<double>(p.y) - at<float>(st, 0x2c);
        const float b2 = static_cast<float>(static_cast<double>(p.z) - at<float>(st, 0x30)); // fstp dword
        const double sa = (a2 * a2 + a1 * a1) + a0 * a0;                            // ..EE..FC
        const double sb = (static_cast<double>(b2) * b2 + b1 * b1) + b0 * b0;       // ..FE..10
        if (!(sb < sa)) return k_complete_00745dc0;                                 // fcompp; test ah,5; jp (>=, NaN)
    }
    at<U>(st, 4) = next_state;                                                      // ..2A
    return k_in_progress_00745dbc;
}
}  // namespace

extern "C" {

// =========================================================================================================
// 00518940 CSWSCreature::GetAction(type): first queued node (+FC list) whose type (+0) matches, else 0
void* __fastcall kotor_fastcall_00518940(void* creature, void*, U type) {
    void* list = at<void*>(creature, 0xfc);
    void* position = *static_cast<void**>(list);                                    // 00518944..4A
    if (position == nullptr) return nullptr;                                        // ..4C..59
    do {
        void* node = unresolved_list_at_pos_005e9b60(at<void*>(creature, 0xfc), nullptr, position); // ..60..67
        if (at<U>(node, 0) == type) return node;                                    // ..6C..6E
        (void)unresolved_list_next_005e9b90(at<void*>(creature, 0xfc), nullptr, &position); // ..70..7B
    } while (position != nullptr);                                                  // ..80..86
    return nullptr;
}

// =========================================================================================================
// 00634E80 CSWPartyFollowDataArray::AdvancePointOnPath: walk `distance` along the ring from `from` (index from_index)
// toward target; out = reached point, *out_index = reached index; ST0 = remaining distance
double __fastcall kotor_fastcall_00634e80(void* follow, void*, float distance, I unused_index, const Vector* from,
    I from_index, const Vector* target, I target_index, Vector* out, I* out_index) {
    (void)unused_index;
    float remaining = distance;                                                     // 00634E89 fst [E-38]
    Vector current = load(from);                                                    // ..A2..B8 [E-24]
    *out = load(from);                                                              // ..BC..D8
    I index = from_index;                                                           // ESI
    U done = 0;                                                                     // [E-34]
    if (!(distance > k_zero_0073d700)) { *out_index = index; return distance; }     // ..95, ..CD..DB -> 006350CB (ST0 = arg)
    double st0 = distance;
    for (;;) {
        if (done != 0) { *out_index = index; return st0; }                          // 00634EF0..F6 -> 006350DA
        const void* p = path_data(follow, index);                                   // ..FC..01 (fstp st(0) first)
        Vector d;                                                                   // [E-30]
        d.z = static_cast<float>(static_cast<double>(at<float>(const_cast<void*>(p), 8)) - current.z); // ..06..27
        d.y = static_cast<float>(static_cast<double>(at<float>(const_cast<void*>(p), 4)) - current.y);
        d.x = static_cast<float>(static_cast<double>(at<float>(const_cast<void*>(p), 0)) - current.x);
        const float length = static_cast<float>(kotor_cdecl_004ab100(&d));         // ..2B..34 (over the from slot)
        Vector n;                                                                   // [E-C]
        d = *normalize_004ab180(&n, &d);                                            // ..38..63
        const float step = (length < remaining) ? length : remaining;               // ..43..77 (test ah,5; jp: >=,NaN -> rem)
        Vector move;                                                                // [E-18]
        move.x = static_cast<float>(static_cast<double>(d.x) * step);               // ..7B..A4
        move.y = static_cast<float>(static_cast<double>(d.y) * step);
        move.z = static_cast<float>(static_cast<double>(d.z) * step);
        vector_add_assign_004a9740(out, nullptr, &move);                            // ..A8
        const double tx = static_cast<double>(target->x) - from->x;                 // ..AD..BA (original `from`)
        const double ty = static_cast<double>(target->y) - from->y;
        const double tz = static_cast<double>(target->z) - from->z;
        const double to_target = (tz * tz + ty * ty) + tx * tx;                     // ..BD..CB
        const double reach = static_cast<double>(distance) * distance;              // ..CD..D1
        if (reach >= to_target) {                                                   // ..D5..E2 (test ah,1; je: C0 clear)
            *out = load(target);                                                    // 0063506E..89
            *out_index = target_index;                                              // ..8C
            Vector t;                                                               // [E-18]
            t.x = static_cast<float>(static_cast<double>(target->x) - from->x);     // ..8F..AC
            t.y = static_cast<float>(static_cast<double>(target->y) - from->y);
            t.z = static_cast<float>(static_cast<double>(target->z) - from->z);
            (void)kotor_cdecl_004ab100(&t);                                         // ..B0..B5 (result dropped)
            *out_index = target_index;                                              // ..C0
            return remaining;                                                       // ..B7 fld [E-38]
        }
        if (remaining >= length) {                                                  // ..E8..F5 (test ah,1; jne: <, NaN skip)
            current = load(path_data(follow, index));                               // ..F7..13
            ++index;                                                                // ..07
            if (index >= 100) index = 0;                                            // ..08..19
            const I head = at<I>(follow, 0xfa4);                                    // ..1B
            if (index == head - 1) done = 1;                                        // ..21..28
            if (index == 99 && head == 0) done = 1;                                 // ..30..39
        }
        st0 = static_cast<double>(remaining) - step;                                // 00635041..45
        remaining = static_cast<float>(st0);                                        // ..4F fst
        if (!(st0 > k_zero_0073d700)) { *out_index = index; return st0; }           // ..49..58 -> 0063505E
    }
}

// =========================================================================================================
// 00512A60 / 00512B40 / 00512C20 / 00512D00 FollowLeader_{Offset,Path,ToOffset,ToPath}WaitDest: identical bodies,
// next state 1 / 3 / 4 / 2. RET 10h.
U __fastcall kotor_fastcall_00512a60(void* c, void*, void* st, void* follow, float* speed, I index) {
    (void)c; (void)speed; (void)index; return wait_dest(st, follow, 1);              // 00512B2A
}
U __fastcall kotor_fastcall_00512b40(void* c, void*, void* st, void* follow, float* speed, I index) {
    (void)c; (void)speed; (void)index; return wait_dest(st, follow, 3);              // 00512C0A
}
U __fastcall kotor_fastcall_00512c20(void* c, void*, void* st, void* follow, float* speed, I index) {
    (void)c; (void)speed; (void)index; return wait_dest(st, follow, 4);              // 00512CEA
}
U __fastcall kotor_fastcall_00512d00(void* c, void*, void* st, void* follow, float* speed, I index) {
    (void)c; (void)speed; (void)index; return wait_dest(st, follow, 2);              // 00512DCA
}

// =========================================================================================================
// 00512DE0 FollowLeader_TeleportForward: jump to the first safe ring point after st+24 (before the head)
U __fastcall kotor_fastcall_00512de0(void* c, void*, void* st, void* follow, float* speed, I index) {
    (void)index;
    I i = at<I>(st, 0x24);                                                          // 00512DEA ESI
    const I head = at<I>(follow, 0xfa4);                                            // ..F2 (kept over the arg slot)
    (void)path_data(follow, at<I>(st, 0x34));                                       // ..FA..04 (result unused)
    Vector p = load(path_data(follow, i));                                          // ..09..25 [E-C]
    ++i;                                                                            // ..21
    if (i >= 100) i = 0;                                                            // ..22..2B
    p = load(path_data(follow, i));                                                 // ..2D..45
    if (w2x_area_safe_point_004be5e0(recovered_object_area_004cb120(c, nullptr), nullptr, &p, addr(pathfind(c))) == 0) { // ..49..65
        for (;;) {
            if (i == head) return k_complete_00745dc0;                              // 00512E70..74
            ++i;                                                                    // ..7A
            if (i >= 100) i = 0;                                                    // ..7B..80
            p = load(path_data(follow, i));                                         // ..82..A8
            if (w2x_area_safe_point_004be5e0(recovered_object_area_004cb120(c, nullptr), nullptr, &p,
                                             addr(pathfind(c))) != 0) break;        // ..9A..BA
        }
    }
    if (i == head) return k_complete_00745dc0;                                      // 00512EBC..C0
    at<I>(st, 0x24) = i;                                                            // ..CA
    copy_vector(bytes(st) + 0x18, &p);                                              // ..CD..DB
    copy_vector(bytes(st) + 0x5c, &p);                                              // ..DE..E6
    *speed = 0.0f;                                                                  // ..E9..ED
    at<U>(st, 4) = 2;                                                               // ..F3
    return k_complete_00745dc0;
}

// =========================================================================================================
// 005127D0 FollowLeader_WaitingForPath
U __fastcall kotor_fastcall_005127d0(void* c, void*, void* st, void* follow, float* speed, I index) {
    (void)speed; (void)index;
    if (at<I>(follow, 0xfa8) < 5) return k_complete_00745dc0;                       // 005127DC..EA (jl)
    U back_index = at<U>(st, 8);                                                    // ..F7..FC [E-90] (preset to st+8 bits)
    Vector back = vec(0, 0, 0);                                                     // [E-7C]
    if (kotor_fastcall_006350f0(follow, nullptr, at<float>(st, 8), &back, &back_index, 0) == 0) // ..FA..2E
        return k_complete_00745dc0;
    copy_vector(bytes(st) + 0x18, &back);                                           // ..34..4B
    at<U>(st, 0x24) = back_index;                                                   // ..4E..52
    copy_vector(bytes(st) + 0x28, &back);                                           // ..55..62
    at<U>(st, 0x34) = back_index;                                                   // ..65
    const Vector leader = load(bytes(frame_GetCharacter_006346c0(party_of(client_app()), nullptr, 0)) + 0x24); // ..68..95
    const double dx = static_cast<double>(leader.x) - at<float>(c, 0x90);          // ..8B..8F
    const double dy = static_cast<double>(leader.y) - at<float>(c, 0x94);          // ..A1..A5
    const double dz = static_cast<double>(leader.z) - at<float>(c, 0x98);          // ..AF..B3
    if (at<U>(st, 0x70) != 0) {                                                     // ..98..B9
        const double sum = (dz * dz + dy * dy) + dx * dx;                           // ..BB..C9
        if (sum < static_cast<double>(k_49_00747c3c))                               // ..CB..DC (test ah,5; jp: >=, NaN go on)
            return k_complete_00745dc0;
    }
    at<U>(st, 0x70) = 0;                                                            // 005128FA
    WalkHit hit;                                                                    // [E-48]
    w5_walk_hit_construct_004b14b0(&hit, nullptr);                                  // ..F6..01
    const U yaw = at<U>(path_data(follow, at<I>(st, 0x24)), 0xc);                   // ..06..16
    Vector rot;                                                                     // [E-64]
    const Vector* r = rotate_by_yaw(&rot, yaw, vat(st, 0x38));                      // ..19..6C
    Vector to;                                                                      // [E-70]
    to.z = static_cast<float>(static_cast<double>(at<float>(st, 0x20)) + r->z);     // ..71..74
    to.y = static_cast<float>(static_cast<double>(at<float>(st, 0x1c)) + r->y);     // ..7A..81
    to.x = static_cast<float>(static_cast<double>(r->x) + at<float>(st, 0x18));     // ..85..8D
    float radius = static_cast<float>(static_cast<double>(at<float>(pathfind(c), 4)) - k_tenth_0073e798); // ..A4..AD [E-80]
    float band = at<float>(pathfind(c), 0x14);                                      // ..B1
    if (direct_line(c, vat(st, 0x5c), &to, &radius, band, &hit) == 1) {             // ..B6..D3
        at<U>(st, 4) = 4;                                                           // ..D5
        return k_complete_00745dc0;
    }
    float radius2 = static_cast<float>(static_cast<double>(at<float>(pathfind(c), 4)) - k_tenth_0073e798); // ..EE..FD [E-94]
    band = at<float>(pathfind(c), 0x14);                                            // 00512A01
    if (direct_line(c, vat(st, 0x5c), vat(st, 0x18), &radius2, band, &hit) == 1) { // ..04..2A
        at<U>(st, 4) = 2;                                                           // ..2C
        return k_complete_00745dc0;
    }
    at<U>(st, 4) = 0;                                                               // ..46
    return k_in_progress_00745dbc;
}

// =========================================================================================================
// 00511860 FollowLeader_Moving_To_Path: walk st+5C toward the path point st+18
U __fastcall kotor_fastcall_00511860(void* c, void*, void* st, void* follow, float* speed, I index) {
    (void)follow; (void)index;
    const float step = static_cast<float>(static_cast<double>(*speed) * k_seven_tenths_00746f4c); // 00511883..99 [E-94]
    const Vector target = load(bytes(st) + 0x18);                                   // ..9D..C1 [E-90]
    Vector* walked = vat(st, 0x5c);                                                 // ESI
    Vector d;                                                                       // [E-80]
    d.z = static_cast<float>(static_cast<double>(target.z) - walked->z);            // ..AC..B0
    d.y = static_cast<float>(static_cast<double>(target.y) - walked->y);            // ..BA..BE
    d.x = static_cast<float>(static_cast<double>(target.x) - walked->x);            // ..C5..CD
    Vector n;                                                                       // [E-60]
    normalize_004ab180(&n, &d);                                                     // ..CF..E1
    U state = at<U>(st, 4);                                                         // ..EA EBP
    const double sum = (static_cast<double>(d.z) * d.z + static_cast<double>(d.y) * d.y) + static_cast<double>(d.x) * d.x; // ..E6..06
    const double step2 = static_cast<double>(step) * step;                          // ..08..0C
    U result;                                                                       // EBX
    Vector next;                                                                    // [E-90]
    if (!(sum > step2 || std::isnan(sum))) {                                        // ..10..19 (test ah,41h; jp)
        state = 3;                                                                  // ..1D
        *speed = static_cast<float>(static_cast<double>(*speed) - std::sqrt(sum) * k_inv_seven_tenths_00747c38); // ..1B..2A
        result = k_in_progress_00745dbc;                                            // ..2C
        next = target;
    } else {
        const double mz = static_cast<double>(n.z) * step;                          // 00511936..3A
        const float my = static_cast<float>(static_cast<double>(n.y) * step);       // ..3E..46 [E-84]
        const float mx = static_cast<float>(static_cast<double>(n.x) * step);       // ..4A..52 [E-90]
        const double z = mz + walked->z;                                            // ..56
        const double y = static_cast<double>(my) + walked->y;                       // ..59..5D
        const double x = static_cast<double>(mx) + walked->x;                       // ..60..64
        *speed = 0.0f;                                                              // ..66
        result = k_complete_00745dc0;                                               // ..6C
        next.x = static_cast<float>(x); next.y = static_cast<float>(y); next.z = static_cast<float>(z); // ..72..7A
    }
    const Vector moved = next;                                                      // 0051197E..92 [E-74]
    WalkHit hit = blank_hit();                                                      // ..A0..31 [E-54]
    float radius = static_cast<float>(static_cast<double>(at<float>(pathfind(c), 4)) - k_tenth_0073e798); // ..3C..45 [E-64]
    const float band = at<float>(pathfind(c), 0x14);                                // ..49
    const I r = direct_line(c, walked, &moved, &radius, band, &hit);                // ..50..6D
    if (r == -3) {                                                                  // ..72..75
        if (blocker_is_party_member(hit) == 0) {                                    // ..77..9F
            at<U>(st, 4) = 10;                                                      // ..A1
            return k_complete_00745dc0;
        }
    } else if (r == 1) {                                                            // ..AD..B0
        *walked = moved;                                                            // ..B2..CA
        at<U>(st, 0x6c) = 0;                                                        // ..C3
        at<U>(st, 4) = state;                                                       // ..CD
        return result;
    }
    queue_bump_event(hit.blocker_40, at<U>(c, 4));                                  // 00511AD4..37 (caller = blocker)
    return k_complete_00745dc0;
}

// =========================================================================================================
// 00512430 FollowLeader_Moving_To_Offset: walk st+5C toward path point + rotated formation offset
U __fastcall kotor_fastcall_00512430(void* c, void*, void* st, void* follow, float* speed, I index) {
    (void)index;
    const float step = static_cast<float>(static_cast<double>(*speed) * k_seven_tenths_00746f4c); // 0051244C..62 [E-A4]
    WalkHit hit = blank_hit();                                                      // ..37..F5 [E-64]
    const U yaw = at<U>(path_data(follow, at<I>(st, 0x24)), 0xc);                   // ..5D..05
    Vector rot;                                                                     // [E-70]
    const Vector* r = rotate_by_yaw(&rot, yaw, vat(st, 0x38));                      // ..08..5E
    Vector target;                                                                  // [E-A0]
    target.x = static_cast<float>(static_cast<double>(r->x) + at<float>(st, 0x18)); // ..63..6E
    target.y = static_cast<float>(static_cast<double>(at<float>(st, 0x1c)) + r->y); // ..72..78
    target.z = static_cast<float>(static_cast<double>(at<float>(st, 0x20)) + r->z); // ..7C..82
    Vector* walked = vat(st, 0x5c);                                                 // ESI
    Vector d;                                                                       // [E-7C]
    d.x = static_cast<float>(static_cast<double>(target.x) - walked->x);            // ..86..8C
    d.y = static_cast<float>(static_cast<double>(target.y) - walked->y);
    d.z = static_cast<float>(static_cast<double>(target.z) - walked->z);
    Vector n1;                                                                      // [E-1C]
    normalize_004ab180(&n1, &d);                                                    // ..A2..B3 (result unused)
    U state = at<U>(st, 4);                                                         // ..C0..C7 [E-94]
    Vector next = vec(0, 0, 0);                                                     // ..D2..EC [E-90]
    const double sum = (static_cast<double>(d.z) * d.z + static_cast<double>(d.y) * d.y) + static_cast<double>(d.x) * d.x; // ..B8..F4
    const double step2 = static_cast<double>(step) * step;                          // ..F6..FA
    U result;                                                                       // [E-A4]
    if (!(sum > step2 || std::isnan(sum))) {                                        // ..FE..07 (test ah,41h; jp)
        next = target;                                                              // ..0B..26
        state = 1;                                                                  // ..2A
        *speed = static_cast<float>(static_cast<double>(*speed) - std::sqrt(sum) * k_inv_seven_tenths_00747c38); // ..09..3A
        result = k_in_progress_00745dbc;                                            // ..3C..42
    } else {
        Vector n2;                                                                  // [E-70]
        const Vector* n = normalize_004ab180(&n2, &d);                              // 00512648..54
        const double mx = static_cast<double>(step) * n->x;                         // ..59..5D
        const double my = static_cast<double>(step) * n->y;                         // ..62..66
        const float mz = static_cast<float>(static_cast<double>(step) * n->z);      // ..69..70 [E-68]
        next.x = static_cast<float>(mx + walked->x);                                // ..74..80
        next.y = static_cast<float>(my + walked->y);                                // ..84..9D
        *speed = 0.0f;                                                              // ..A1
        result = k_complete_00745dc0;                                               // ..A7..B9
        next.z = static_cast<float>(static_cast<double>(mz) + walked->z);           // ..92..B5
    }
    float radius = static_cast<float>(static_cast<double>(at<float>(pathfind(c), 4)) - k_tenth_0073e798); // 005126BD..D7 [E-80]
    float band = at<float>(pathfind(c), 0x14);                                      // ..DB [E-84]
    if (direct_line(c, walked, &next, &radius, band, &hit) == 1) {                  // ..E9..FD
        *walked = next;                                                             // ..FF..18
        at<U>(st, 4) = state;                                                       // ..1B
        return result;
    }
    float radius2 = static_cast<float>(static_cast<double>(at<float>(pathfind(c), 4)) - k_tenth_0073e798); // ..2B..41 [E-84]
    band = at<float>(pathfind(c), 0x14);                                            // ..45 [E-80]
    const I r2 = direct_line(c, walked, vat(st, 0x18), &radius2, band, &hit);       // ..4E..5F
    if (r2 == -3) {                                                                 // ..64..67
        if (blocker_is_party_member(hit) == 0) at<U>(st, 4) = 10;                   // ..69..94
        return k_complete_00745dc0;
    }
    if (r2 == 1) {                                                                  // ..AD..B0
        at<U>(st, 4) = 2;                                                           // ..B2
        return k_in_progress_00745dbc;
    }
    at<U>(st, 4) = 10;                                                              // 00512794
    return k_complete_00745dc0;
}

// =========================================================================================================
// 00511B60 FollowLeader_Following_Path: advance along the leader's breadcrumb ring, then look ahead up to 5 m
U __fastcall kotor_fastcall_00511b60(void* c, void*, void* st, void* follow, float* speed, I index) {
    Vector advanced = vec(0, 0, 0);                                                 // 00511BBD..CD [E-178]
    I advanced_index;                                                               // [E-188] (written by the callee)
    const float remaining = static_cast<float>(kotor_fastcall_00634e80(follow, nullptr, *speed, index, vat(st, 0x18),
        at<I>(st, 0x24), vat(st, 0x28), at<I>(st, 0x34), &advanced, &advanced_index)); // ..85..DA [E-16C]
    at<U>(st, 0x6c) += at<U>(c, 0xd0);                                              // ..DE..E4
    WalkHit hit = blank_hit();                                                      // ..ED..A2 [E-F0]
    float radius = static_cast<float>(static_cast<double>(at<float>(pathfind(c), 4)) - k_tenth_0073e798); // ..AD..BF [E-18C]
    float band = at<float>(pathfind(c), 0x14);                                      // ..C3 [E-194]
    I r = direct_line(c, vat(st, 0x18), &advanced, &radius, band, &hit);            // ..B6..DF
    if (r == -3) {                                                                  // ..E4..E7
        if (blocker_is_party_member(hit) != 0) return k_complete_00745dc0;          // ..ED..19
        const I cur = at<I>(st, 0x24);                                              // ..1F
        if (cur != advanced_index) {                                                // ..22..28
            const I limit = advanced_index;                                         // ..2A [E-194]
            advanced_index = cur;                                                   // ..31
            advanced = load(path_data(follow, cur));                                // ..2E..56
            if (aurora_vector_equal_004aa980(&advanced, vat(c, 0x90)) != 0) {       // ..4A..64
                I next = advanced_index + 1;                                        // ..66..6E
                if (!(next > limit)) {                                              // ..6F..71 (jg)
                    if (next >= 100) next = 0;                                      // ..73..7E
                    advanced_index = next;
                    advanced = load(path_data(follow, next));                       // ..82..9A
                }
            }
        }
        radius = static_cast<float>(static_cast<double>(at<float>(pathfind(c), 4)) - k_tenth_0073e798); // 00511D9E..BB
        band = at<float>(pathfind(c), 0x14);                                        // ..BF..C4
        r = direct_line(c, vat(st, 0x18), &advanced, &radius, band, &hit);          // ..B4..D9
        if (r == -3) {                                                              // ..DE..E1
            if (blocker_is_party_member(hit) == 0) at<U>(st, 4) = 10;               // ..E3..11
            return k_complete_00745dc0;
        }
    }
    if (r != 1) {                                                                   // 00511E1D..20
        if (r == -3) { at<U>(st, 4) = 10; return k_complete_00745dc0; }             // 00512388..94
        *speed = 0.0f;                                                              // ..96..9F
        queue_bump_event(hit.blocker_40, at<U>(c, 4));                              // ..9D..03 (caller = blocker)
        return k_complete_00745dc0;
    }
    copy_vector(bytes(st) + 0x18, &advanced);                                       // 00511E26..4E
    copy_vector(bytes(st) + 0x5c, &advanced);                                       // ..34..55
    *speed = remaining;                                                             // ..58, ..67
    at<I>(st, 0x24) = advanced_index;                                               // ..51..5C
    at<U>(st, 0x44) = 0;                                                            // ..5F
    const U yaw = at<U>(path_data(follow, advanced_index), 0xc);                    // ..66..6E
    Vector rot;                                                                     // [E-60]
    const Vector* rr = rotate_by_yaw(&rot, yaw, vat(st, 0x38));                     // ..71..CA
    Vector offset_point;                                                            // [E-148]
    offset_point.z = static_cast<float>(static_cast<double>(advanced.z) + rr->z);   // ..CF..D3
    offset_point.y = static_cast<float>(static_cast<double>(advanced.y) + rr->y);   // ..D9..DF
    offset_point.x = static_cast<float>(static_cast<double>(advanced.x) + rr->x);   // ..E2..E6
    WalkHit hit2 = blank_hit();                                                     // ..EE..AD [E-A8]
    float radius2 = at<float>(pathfind(c), 4);                                      // ..B8..BB [E-18C] (no 0.1 margin)
    band = at<float>(pathfind(c), 0x14);                                            // ..BF
    const I r2 = direct_line(c, &advanced, &offset_point, &radius2, band, &hit2);   // ..C2..EA
    if (r2 != 1) return k_complete_00745dc0;                                        // ..EF..F2
    double accum = k_zero_0073d700;                                                 // 00511FF8 ST0
    float accum_f = static_cast<float>(accum);                                      // ..06 [E-150]
    U done = 0;                                                                     // ..0A [E-16C]
    U ok = static_cast<U>(r2);                                                      // ..11 [E-194]
    I i = at<I>(st, 0x24);                                                          // ..0E EBX / [E-190]
    Vector previous = offset_point;                                                 // ..1D..25 [E-168]
    for (;;) {
        if (accum > k_five_00743264 || std::isnan(accum)) break;                    // 00512030..3B (test ah,41h; jp)
        const U yaw2 = at<U>(path_data(follow, i), 0xc);                            // ..41..54 [E-18C]
        Vector next = vec(0, 0, 0);                                                 // ..4E..64 [E-184]
        if (i == at<I>(st, 0x34)) {                                                 // ..5B..6C
            Vector rot2;                                                            // [E-38]
            const Vector* r3 = rotate_by_yaw(&rot2, yaw2, vat(st, 0x38));           // ..72..BF
            Vector goal;                                                            // [E-15C]
            goal.x = static_cast<float>(static_cast<double>(at<float>(st, 0x28)) + r3->x); // ..C4..C9
            goal.y = static_cast<float>(static_cast<double>(at<float>(st, 0x2c)) + r3->y); // ..D1..D7
            goal.z = static_cast<float>(static_cast<double>(at<float>(st, 0x30)) + r3->z); // ..DB..F4
            next.x = goal.x; next.y = goal.y; next.z = goal.z;                      // ..E5..14
            Vector d;                                                               // [E-108]
            d.x = static_cast<float>(static_cast<double>(goal.x) - previous.x);     // ..F8..0D
            d.y = static_cast<float>(static_cast<double>(goal.y) - previous.y);     // ..18..20
            d.z = static_cast<float>(static_cast<double>(goal.z) - previous.z);     // ..27..2F
            const float left = static_cast<float>(static_cast<double>(k_five_00743264) - accum_f); // ..36..40 [E-190]
            Vector n;                                                               // [E-44]
            const Vector* nn = normalize_004ab180(&n, &d);                          // ..04..44
            const double mx = static_cast<double>(left) * nn->x;                    // ..49..4D
            const double my = static_cast<double>(left) * nn->y;                    // ..52..56
            const float mz = static_cast<float>(static_cast<double>(left) * nn->z); // ..59..60 [E-58]
            next.x = static_cast<float>(mx + previous.x);                           // ..67..7B
            next.y = static_cast<float>(my + previous.y);                           // ..7F..83
            next.z = static_cast<float>(static_cast<double>(mz) + previous.z);      // ..8A..95
            done = 1;                                                               // ..B2
        } else {
            const void* p2 = path_data(follow, i);                                  // 005121BF..E5 EBX
            Vector rot3;                                                            // [E-54]
            const Vector* r4 = rotate_by_yaw(&rot3, yaw2, vat(st, 0x38));           // ..C6..2C
            next.x = static_cast<float>(static_cast<double>(at<float>(const_cast<void*>(p2), 0)) + r4->x); // ..31..38
            next.y = static_cast<float>(static_cast<double>(r4->y) + at<float>(const_cast<void*>(p2), 4)); // ..40..46
            next.z = static_cast<float>(static_cast<double>(r4->z) + at<float>(const_cast<void*>(p2), 8)); // ..4A..60
        }
        if (aurora_vector_equal_004aa980(&previous, &next) != 0) return k_complete_00745dc0; // 0051226C..80
        float radius3 = at<float>(pathfind(c), 4);                                  // ..86..91 [E-14C]
        const float band3 = at<float>(pathfind(c), 0x14);                           // ..95 [E-118]
        if (direct_line(c, &previous, &next, &radius3, band3, nullptr) != 1) {      // ..8F..C4 (no walk hit)
            done = 1;                                                               // ..C6
            ok = 0;                                                                 // ..CE
        }
        Vector seg;                                                                 // [E-FC]
        seg.x = static_cast<float>(static_cast<double>(next.x) - previous.x);       // ..D6..E6
        seg.y = static_cast<float>(static_cast<double>(next.y) - previous.y);       // ..ED..F5
        seg.z = static_cast<float>(static_cast<double>(next.z) - previous.z);       // ..FC..04
        accum = kotor_cdecl_004ab100(&seg) + accum_f;                               // ..0B..10 (ST0 kept)
        accum_f = static_cast<float>(accum);                                        // ..20 fst
        previous = next;                                                            // ..2B..33
        ++i;                                                                        // ..27
        if (i >= 100) i = 0;                                                        // ..28..45
        if (done != 0) break;                                                       // ..49..4F (fstp st(0))
    }
    if (ok == 1 && at<U>(st, 0x6c) >= 3000u) {                                      // 00512357..69 (jb)
        at<U>(st, 4) = 4;                                                           // ..76
        *speed = 0.0f;                                                              // ..7D
    }
    return k_complete_00745dc0;
}

// =========================================================================================================
// 00511290 FollowLeader_Offset: move st+5C onto the interpolated formation-offset line of the advanced point
U __fastcall kotor_fastcall_00511290(void* c, void*, void* st, void* follow, float* speed, I index) {
    WalkHit hit = blank_hit();                                                      // 005112A8..A5 [E-58]
    Vector advanced = vec(0, 0, 0);                                                 // ..BD..C5 [E-80]
    Vector next = vec(0, 0, 0);                                                     // ..C9..D1 [E-8C]
    I advanced_index;                                                               // [E-74] (written by the callee)
    (void)kotor_fastcall_00634e80(follow, nullptr, *speed, index, vat(st, 0x18), at<I>(st, 0x24), vat(st, 0x28),
                                  at<I>(st, 0x34), &advanced, &advanced_index);     // ..D5..B5 (fstp st(0))
    float radius = at<float>(pathfind(c), 4);                                       // ..B7..C0 [E-E0]
    float band = at<float>(pathfind(c), 0x14);                                      // ..C4..D4 [E-DC]
    (void)direct_line(c, vat(st, 0x18), &advanced, &radius, band, &hit);            // ..C7..EF (result unused)
    I previous_index = advanced_index - 1;                                          // ..F4..F8
    if (previous_index < 0) previous_index += 100;                                  // ..FB..FF
    const U yaw_a = at<U>(path_data(follow, advanced_index), 0xc);                  // 00511402..0D [E-E0]
    const U yaw_b = at<U>(path_data(follow, previous_index), 0xc);                  // ..11..19 [E-DC]
    const Vector offset = load(bytes(st) + 0x38);                                   // ..1C..40 [E-64]
    const Vector pa = load(path_data(follow, advanced_index));                      // ..36..5F [E-98]
    const Vector pb = load(path_data(follow, previous_index));                      // ..5C..70 [E-B4]
    const Vector a = advanced;                                                      // ..74..9D [E-D8]
    Vector rot1;                                                                    // [E-C0]
    const Vector* r1 = rotate_by_yaw(&rot1, yaw_a, &offset);                        // ..A1..E9
    Vector offset_a;                                                                // [E-CC]
    offset_a.x = static_cast<float>(static_cast<double>(pa.x) + r1->x);             // ..EE..F7
    offset_a.y = static_cast<float>(static_cast<double>(pa.y) + r1->y);             // ..FB..02
    offset_a.z = static_cast<float>(static_cast<double>(pa.z) + r1->z);             // ..06..25
    Vector rot2;                                                                    // [E-C0]
    const Vector* r2 = rotate_by_yaw(&rot2, yaw_b, &offset);                        // ..0D..69
    Vector offset_b;                                                                // [E-A8]
    offset_b.x = static_cast<float>(static_cast<double>(pb.x) + r2->x);             // ..6E..79
    offset_b.y = static_cast<float>(static_cast<double>(pb.y) + r2->y);             // ..7D..84
    offset_b.z = static_cast<float>(static_cast<double>(pb.z) + r2->z);             // ..88..8F
    Vector t1;                                                                      // [E-C0]
    t1.x = static_cast<float>(static_cast<double>(a.x) - pb.x);                     // ..93..9B
    t1.y = static_cast<float>(static_cast<double>(a.y) - pb.y);                     // ..9F..A7
    t1.z = static_cast<float>(static_cast<double>(a.z) - pb.z);                     // ..AB..B3
    const float m1 = static_cast<float>(kotor_cdecl_004ab100(&t1));                 // ..B7..BC [E-E0]
    Vector t2;                                                                      // [E-C0]
    t2.x = static_cast<float>(static_cast<double>(pa.x) - pb.x);                    // ..C0..CD
    t2.y = static_cast<float>(static_cast<double>(pa.y) - pb.y);                    // ..D1..D9
    t2.z = static_cast<float>(static_cast<double>(pa.z) - pb.z);                    // ..DD..E5
    const float ratio = static_cast<float>(static_cast<double>(m1) / kotor_cdecl_004ab100(&t2)); // ..E9..F5 (fdivr) [E-E0]
    Vector w;                                                                       // [E-70]
    w.x = static_cast<float>(static_cast<double>(offset_a.x) - offset_b.x);         // ..F9..0D
    w.y = static_cast<float>(static_cast<double>(offset_a.y) - offset_b.y);         // ..14..1C
    w.z = static_cast<float>(static_cast<double>(offset_a.z) - offset_b.z);         // ..23..2B
    vector_scale_assign_004a97a0(&w, nullptr, ratio);                               // ..32
    next.x = static_cast<float>(static_cast<double>(w.x) + offset_b.x);             // ..37..42 (->[E-D8]->[E-8C])
    next.y = static_cast<float>(static_cast<double>(w.y) + offset_b.y);             // ..46..59
    next.z = static_cast<float>(static_cast<double>(w.z) + offset_b.z);             // ..5D..70
    if (recovered_object_area_004cb120(c, nullptr) != nullptr)                      // ..7C..85
        at<U>(&next, 8) = w1x_area_height_bits_004bc380(recovered_object_area_004cb120(c, nullptr), nullptr, &next, 1,
                                                         nullptr);                  // ..87..9E
    // 005116A2..3B: st+58 = |offB-offA|^2 / |pB-pA|^2 with the original mix of rounded and unrounded components
    const double ux = static_cast<double>(pb.x) - pa.x;                             // ..A2..A9
    const double uy = static_cast<double>(pb.y) - pa.y;                             // ..B1..B5
    const double uz = static_cast<double>(pb.z) - pa.z;                             // ..BD..C1
    const float ux_f = static_cast<float>(ux), uy_f = static_cast<float>(uy), uz_f = static_cast<float>(uz);
    const double wx = static_cast<double>(offset_b.x) - offset_a.x;                 // ..D7..DB
    const double wy = static_cast<double>(offset_b.y) - offset_a.y;                 // ..E3..E7
    const double wz = static_cast<double>(offset_b.z) - offset_a.z;                 // ..EF..F3
    const float wx_f = static_cast<float>(wx), wy_f = static_cast<float>(wy), wz_f = static_cast<float>(wz);
    const double ww = (static_cast<double>(wz_f) * wz_f + wy * wy_f) + static_cast<double>(wx_f) * wx_f; // ..05..1D
    const double uu = (static_cast<double>(uz_f) * uz_f + uy * uy_f) + static_cast<double>(ux_f) * ux_f; // ..1F..35
    at<float>(st, 0x58) = static_cast<float>(ww / uu);                              // ..37..3B (fdivp)
    float radius2 = at<float>(pathfind(c), 4);                                      // ..3E..47 [E-DC]
    float band2 = at<float>(pathfind(c), 0x14);                                     // ..4B..5B [E-E0]
    if (direct_line(c, vat(st, 0x5c), &next, &radius2, band2, &hit) != 1) {        // ..55..7B
        at<U>(st, 4) = 2;                                                           // 005117F5
        return k_in_progress_00745dbc;
    }
    float radius3 = at<float>(pathfind(c), 4);                                      // ..7D..86
    float band3 = at<float>(pathfind(c), 0x14);                                     // ..8A..9A
    const I r3 = direct_line(c, &next, &advanced, &radius3, band3, &hit);           // ..94..B9
    const U blocker_client = kotor_fastcall_005eda50(client_app(), nullptr, hit.blocker_40); // ..BE..D1 (always called)
    if (r3 == -3) {                                                                 // ..D6..D9
        if (recovered_party_member_00634620(party_of(client_app()), nullptr, blocker_client) == 0) { // ..DB..F3
            at<U>(st, 4) = 2;
            return k_in_progress_00745dbc;
        }
    } else if (r3 != 1) {                                                           // 0051180E..11
        at<U>(st, 4) = 2;
        return k_in_progress_00745dbc;
    }
    copy_vector(bytes(st) + 0x5c, &next);                                           // 00511813..33
    copy_vector(bytes(st) + 0x18, &advanced);                                       // ..21..44
    at<U>(st, 0x44) = 0;                                                            // ..3D
    at<I>(st, 0x24) = advanced_index;                                               // ..47
    return k_complete_00745dc0;
}

// =========================================================================================================
// 0051AC10 FollowLeader_FindPath: re-plan to the goal point (+ offset) with a queued move (type 40h follow-up action)
U __fastcall kotor_fastcall_0051ac10(void* c, void*, void* st, void* follow, float* speed, I index) {
    (void)speed; (void)index;
    kotor_fastcall_006359c0(party_of(client_app()), nullptr);                       // 0051AC13..28
    const I goal_index = at<I>(st, 0x34);                                           // ..31 EBX
    Vector goal = load(bytes(st) + 0x28);                                           // ..34..4C [E-40]
    const U yaw = at<U>(path_data(follow, goal_index), 0xc);                        // ..47..5F (over the arg slot)
    Vector rot;                                                                     // [E-1C]
    const Vector* r = rotate_by_yaw(&rot, yaw, vat(st, 0x38));                      // ..58..AB
    Vector point;                                                                   // [E-34]
    point.z = static_cast<float>(static_cast<double>(goal.z) + r->z);               // ..B0..B4
    point.y = static_cast<float>(static_cast<double>(goal.y) + r->y);               // ..BA..C0
    point.x = static_cast<float>(static_cast<double>(goal.x) + r->x);               // ..C5..CD
    at<U>(&point, 8) = w1x_area_height_bits_004bc380(recovered_object_area_004cb120(c, nullptr), nullptr, &point, 1,
                                                     nullptr);                      // ..BE..EA
    copy_vector(bytes(st) + 0x18, &goal);                                           // ..EE..02
    at<I>(st, 0x24) = goal_index;                                                   // ..05
    void* pf = pathfind(c);                                                         // ..08
    at<U>(pf, 0x254) = k_object_invalid;                                            // ..0E..13 (inline SetBlockingCreature(invalid))
    Vector zero = vec(0, 0, 0);                                                     // ..19..29 [E-28]
    copy_vector(bytes(pf) + 0x258, &zero);                                          // ..31..47
    U target = k_object_invalid;                                                    // ..4A [E-48]
    U run = 1;                                                                      // ..50 [E-4C]
    U wait = 0x3f000000u;                                                           // ..58 [E-50] 0.5
    U area_id = at<U>(recovered_object_area_004cb120(c, nullptr), 0x120);           // ..60..77 [E-44] (no null test)
    const U saved_e8 = at<U>(c, 0xe8);                                              // ..65 EBP
    at<U>(c, 0xe8) = 1;                                                             // ..7B
    void* node = kotor_fastcall_00518940(c, nullptr, 0x3d);                         // ..73..85
    const U group = node != nullptr ? at<U16>(node, 0x6c) : 0xffffu;                // ..8A..93
    if (w2x_area_safe_point_004be5e0(recovered_object_area_004cb120(c, nullptr), nullptr, &point, addr(pathfind(c))) != 0) { // ..97..B3
        front(c, 0x40, group, {2, addr(&point.x), 2, addr(&point.y), 2, addr(&point.z), 3, addr(&area_id),
                               3, addr(&target), 1, addr(&run), 2, addr(&wait)});   // ..B9..10
        zero = vec(0, 0, 0);                                                        // ..4D..60
        w5_creature_add_move_to_point_action_to_front_004f8a50(c, nullptr, 0xfffe, &point, at<U>(c, 0x8c),
            k_object_invalid, 1, 0.0f, flt(0x41f00000u), 0, 0, 0, 0, 0, 0, &zero, 0, 0, 0); // ..15..6B
        at<U>(st, 4) = 6;                                                           // ..70
    } else {
        (void)w1x_area_safe_location_004be860(recovered_object_area_004cb120(c, nullptr), nullptr, addr(&goal),
            0x40400000u, addr(pathfind(c)), 0, addr(&goal));                        // 0051AE7C..9D (3.0; in place)
        front(c, 0x40, group, {2, addr(&goal.x), 2, addr(&goal.y), 2, addr(&goal.z), 3, addr(&area_id),
                               3, addr(&target), 1, addr(&run), 2, addr(&wait)});   // ..A2..F3
        zero = vec(0, 0, 0);                                                        // ..30..43
        w5_creature_add_move_to_point_action_to_front_004f8a50(c, nullptr, 0xfffe, &goal, at<U>(c, 0x8c),
            k_object_invalid, 1, 0.0f, flt(0x41f00000u), 0, 0, 0, 0, 0, 0, &zero, 0, 0, 0); // ..F8..4E
        at<U>(st, 4) = 7;                                                           // ..53
    }
    at<U>(c, 0xa8c) = 0xffffffffu;                                                  // 0051AF5A
    at<U>(c, 0xe8) = saved_e8;                                                      // ..64
    const U leader_id = kotor_fastcall_004aea30(server_app(), nullptr, at<U>(party_of(client_app()), 0x24)); // ..6A..85
    void* leader = kotor_fastcall_004ae770(server_app(), nullptr, leader_id);       // ..8A..94
    if (leader != nullptr) copy_vector(bytes(st) + 0x74, bytes(leader) + 0x90);     // ..99..B2
    at<U>(st, 0x68) = 1;                                                            // ..B5
    return k_complete_00745dc0;
}

// =========================================================================================================
// 0051C360 CSWSCreature::WalkUpdateLocation_QuickWalk_FollowLeader: per-frame follow state machine for a party member
U __fastcall kotor_fastcall_0051c360(void* c, void*) {
    void* client = client_app();                                                    // 0051C363..73 [E-48]
    const U client_id = kotor_fastcall_005eda50(client, nullptr, at<U>(c, 4));      // ..6F..77
    const I index = static_cast<I>(recovered_party_index_006349c0(party_of(client_app()), nullptr, client_id)); // ..7C..92
    if (index == -1) return k_in_progress_00745dbc;                                 // ..94..A3
    if (index == 0) return k_complete_00745dc0;                                     // ..A4..B2
    void* party = party_of(client_app());                                           // ..B3..BD [E-50]
    void* st = bytes(party) + static_cast<U>(index) * 0x88 + 0x24;                  // ..C2..CA ESI
    if (at<U>(st, 0x68) != 0) {                                                     // ..D2..D7
        copy_vector(bytes(st) + 0x5c, bytes(c) + 0x90);                             // ..D9..EF
        at<U>(st, 0x68) = 0;                                                        // ..F2
    }
    if (recovered_debug_0083281c != 0) {                                            // ..F9..00 (debug-only, results unused)
        (void)party_of(client_app());                                               // ..02..0A
        (void)party_of(client_app());                                               // ..0F..18
        w5_cell_follow_debug_index_00832808 = static_cast<U>(index);                // ..20
        (void)WindowCStr005e5670(bytes(c) + 0x18, nullptr);                         // ..1D..26
        Vector d;                                                                   // [E-3C]
        d.x = static_cast<float>(static_cast<double>(at<float>(st, 0x28)) - at<float>(st, 0x18)); // ..2B..36
        d.y = static_cast<float>(static_cast<double>(at<float>(st, 0x2c)) - at<float>(st, 0x1c));
        d.z = static_cast<float>(static_cast<double>(at<float>(st, 0x30)) - at<float>(st, 0x20));
        (void)kotor_cdecl_004ab100(&d);                                             // ..4E..53
    }
    if (fild_unsigned(at<U>(c, 0xd0)) == static_cast<double>(k_zero_0073d700))     // ..58..79 (test ah,44h; jp)
        return k_in_progress_00745dbc;
    const double ax = static_cast<double>(at<float>(st, 0x28)) - at<float>(st, 0x18); // ..87..8B
    const double ay = static_cast<double>(at<float>(st, 0x2c)) - at<float>(st, 0x1c); // ..91..94
    const double az = static_cast<double>(at<float>(st, 0x30)) - at<float>(st, 0x20); // ..97..9A
    const double spread = (az * az + ay * ay) + ax * ax;                            // ..9D..AB
    const float spread_f = static_cast<float>(spread);                              // ..AD fst [E-44]
    Vector e;                                                                       // [E-3C]
    e.x = static_cast<float>(static_cast<double>(at<float>(st, 0x28)) - at<float>(st, 0x5c)); // ..B7..BD
    e.y = static_cast<float>(static_cast<double>(at<float>(st, 0x2c)) - at<float>(st, 0x60)); // ..C1..C7
    e.z = static_cast<float>(static_cast<double>(at<float>(st, 0x30)) - at<float>(st, 0x64)); // ..CB..D1
    auto debug_818 = [&]() {                                                        // 0051C5CF / 0051C6AC ([00832818] = 0)
        (void)frame_GetCharacter_006346c0(party_of(client), nullptr, 0);
        (void)recovered_creature_client_creature_004efb20(c, nullptr);
    };
    float speed;                                                                    // [E-4C]
    bool far = !(spread < k_sixteen_00747db0 || std::isnan(spread));                // ..D5..E0 (test ah,1; je)
    if (!far) {
        const double es = (static_cast<double>(e.z) * e.z + static_cast<double>(e.y) * e.y) + static_cast<double>(e.x) * e.x; // ..E6..00
        far = !(es < k_100_00741838 || std::isnan(es));                             // ..02..0D
    }
    if (!far) {
        if (at<float>(frame_GetCharacter_006346c0(party_of(client), nullptr, 0), 0x3b0) > k_tenth_0073e798) { // ..13..36 (test ah,41h; jne)
            speed = static_cast<float>(static_cast<double>(at<float>(frame_GetCharacter_006346c0(party_of(client), nullptr, 0),
                                                                     0x3b0)) * k_1000_0073d6fc); // ..38..56 -> ..A8
            if (k_cell_00832818 != 0) debug_818();                                  // 0051C6AC
        } else {
            Vector f;                                                               // [E-3C]
            kotor_cdecl_00423bc0(&f, vat(st, 0x18), vat(st, 0x28));                 // 0051C55B..65
            const double fs = (static_cast<double>(f.z) * f.z + static_cast<double>(f.y) * f.y) + static_cast<double>(f.x) * f.x; // ..6A..89
            if (!(fs < k_thousandth_0073d708 || std::isnan(fs))) {                  // ..8B..96 (test ah,1; jne)
                const U last = at<U>(st, 0x48);                                     // ..98..9B [E-40]
                const double walk = w5_creature_get_walk_rate_004f1b20(c, nullptr) * k_nine_tenths_00747da8; // ..9F..A4
                if (walk < flt(last)) speed = flt(last);                            // ..AA..BC (test ah,5; jp: >=,NaN)
                else speed = static_cast<float>(w5_creature_get_walk_rate_004f1b20(c, nullptr) * k_nine_tenths_00747da8); // ..BE..CB
            } else if (spread_f > k_four_0073f404 || std::isnan(spread_f)) {        // 0051C5FA..09 (test ah,41h; jp)
                speed = static_cast<float>(w5_creature_get_run_rate_004f1be0(c, nullptr)); // ..18..1D -> ..CB
            } else {
                speed = static_cast<float>(w5_creature_get_run_rate_004f1be0(c, nullptr) *
                                           kotor_cell_drive_run_ratio_007a22f4);    // ..0B..16 -> ..CB
            }
            if (k_cell_00832818 != 0) { debug_818(); if (k_cell_00832818 != 0) debug_818(); } // ..CF..F5 -> ..AC
        }
    } else {
        if (!(at<float>(frame_GetCharacter_006346c0(party_of(client), nullptr, 0), 0x3b0) > k_tenth_0073e798)) { // 0051C61F..44
            speed = static_cast<float>(w5_creature_get_run_rate_004f1be0(c, nullptr)); // ..A1..A3
        } else {
            const float leader_speed = at<float>(frame_GetCharacter_006346c0(party_of(client), nullptr, 0), 0x3b0); // ..46..5C [E-40]
            const double run = w5_creature_get_run_rate_004f1be0(c, nullptr);      // ..60..62
            const double lead = static_cast<double>(leader_speed) * k_1000_0073d6fc; // ..67..6B
            if (lead < run) speed = static_cast<float>(w5_creature_get_run_rate_004f1be0(c, nullptr)); // ..71..81 (test ah,5; jp)
            else speed = static_cast<float>(static_cast<double>(at<float>(frame_GetCharacter_006346c0(party_of(client),
                                                                          nullptr, 0), 0x3b0)) * k_1000_0073d6fc); // ..83..9F
        }
        if (k_cell_00832818 != 0) debug_818();                                      // 0051C6AC..C9
    }
    const float base_speed = speed;                                                 // 0051C6D2..DC [E-40]
    double factored;                                                                // ST0
    if (!(spread_f > k_one_0073d764 || std::isnan(spread_f))) factored = static_cast<double>(speed) * k_nine_tenths_00747da8; // ..CE..F1
    else if (spread_f > k_225_00747da4 || std::isnan(spread_f)) factored = static_cast<double>(speed) * k_three_halves_0073f414; // ..F3..10
    else factored = static_cast<double>(speed) * k_six_fifths_00747da0;             // ..08
    float step = static_cast<float>((fild_unsigned(at<U>(c, 0xd0)) * factored) * k_micro_007439bc); // ..16..47 [E-4C]
    const Vector old_position = load(bytes(c) + 0x90);                              // ..2E..55 [E-C]
    const I other = index == 1 ? 2 : (index == 2 ? 1 : 0);                          // ..42..67
    (void)kotor_fastcall_004aea30(server_app(), nullptr,
                                  at<U>(party_of(client_app()), static_cast<U>(other) * 0x88 + 0x24)); // ..6C..8D (unused)
    at<U>(pathfind(c), 0x34) = k_object_invalid;                                    // ..92..98
    void* pf_for_area = pathfind(c);                                                // ..9F
    at<void*>(recovered_object_area_004cb120(c, nullptr), 0x1ac) = pf_for_area;     // ..A5..AC
    at<U>(st, 0x58) = 0x3f800000u;                                                  // ..B2 1.0
    U result = k_in_progress_00745dbc;                                              // ..B9 EBP
    void* follow = bytes(party) + 0x2dc;
    for (;;) {                                                                      // 0051C7D0
        if (!(step >= k_zero_0073d700)) break;                                      // ..D0..DF (test ah,1; jne: <0, NaN)
        at<I>(st, 0x44) += 1;                                                       // ..E5..F0
        const U slot = at<U>(st, 4) + 1;                                            // ..E8..EC
        if (slot <= 11) {                                                           // ..ED..F3 (ja)
            switch (slot) {                                                         // ..F9 table 0051CCEC
            case 0:                                                                 // 0051C800: state -1
                copy_vector(bytes(st) + 0x5c, bytes(c) + 0x90);
                at<U>(st, 4) = 5;
                break;
            case 1: result = kotor_fastcall_0051ac10(c, nullptr, st, follow, &step, index); break;  // 0051C827
            case 2: result = kotor_fastcall_00511290(c, nullptr, st, follow, &step, index); break;  // 0051C844
            case 3: result = kotor_fastcall_00511860(c, nullptr, st, follow, &step, index); break;  // 0051C861
            case 4: result = kotor_fastcall_00511b60(c, nullptr, st, follow, &step, index); break;  // 0051C87E
            case 5: result = kotor_fastcall_00512430(c, nullptr, st, follow, &step, index); break;  // 0051C89B
            case 6: result = kotor_fastcall_005127d0(c, nullptr, st, follow, &step, index); break;  // 0051C8B8
            case 7: result = kotor_fastcall_00512a60(c, nullptr, st, follow, &step, index); at<U>(st, 0x44) = 0; break; // 0051C8D5
            case 8: result = kotor_fastcall_00512b40(c, nullptr, st, follow, &step, index); at<U>(st, 0x44) = 0; break; // 0051C8EF
            case 9: result = kotor_fastcall_00512c20(c, nullptr, st, follow, &step, index); at<U>(st, 0x44) = 0; break; // 0051C909
            case 10: result = kotor_fastcall_00512d00(c, nullptr, st, follow, &step, index); at<U>(st, 0x44) = 0; break; // 0051C923
            default: result = kotor_fastcall_00512de0(c, nullptr, st, follow, &step, index); at<U>(st, 0x44) = 0; break; // 0051C93D (11)
            }
        }
        if (at<I>(st, 0x44) > 50) {                                                 // 0051C95E..62 (jle)
            at<U>(st, 4) = 0xffffffffu;
            at<U>(st, 0x44) = 0;
        }
        if (result != k_in_progress_00745dbc) break;                                // ..72..78
    }
    // 0051C97E: commit the walked point
    Vector* position = vat(c, 0x90);                                                // EBX
    const double fz = static_cast<double>(at<float>(st, 0x64)) - position->z;       // ..7E..84
    const double fy = static_cast<double>(at<float>(st, 0x60)) - position->y;       // ..90..95
    Vector f;                                                                       // [E-30]
    f.x = static_cast<float>(static_cast<double>(at<float>(st, 0x5c)) - position->x); // ..9A..9E
    f.y = static_cast<float>(fy);                                                   // ..A2 fst
    f.z = static_cast<float>(fz);                                                   // ..A6..A8
    const double fsum = (fy * fy + fz * fz) + static_cast<double>(f.x) * f.x;       // ..AC..BE
    float radius = at<float>(pathfind(c), 4);                                       // ..CF..D8 [E-48]
    const float band = at<float>(pathfind(c), 0x14);                                // ..DC..E3 [E-44]
    if (!(fsum < k_thousandth_0073d708 || std::isnan(fsum))) {                      // ..C0..F1 (test ah,1; jne)
        if (direct_line(c, vat(st, 0x5c), position, &radius, band, nullptr) == 1) { // ..F7..06
            vector_scale_assign_004a97a0(&f, nullptr, k_cell_half_007a1b58);        // 0051CA08..13
            f.z = 0.0f;                                                             // ..23
            Vector half;                                                            // [E-3C]
            vector_sum_00423b90(&half, position, &f);                               // ..18..2B
            at<U>(&half, 8) = w1x_area_height_bits_004bc380(recovered_object_area_004cb120(c, nullptr), nullptr, &half, 1,
                                                            nullptr);               // ..33..4A
            recovered_object_position_004cd1a0(c, nullptr, &half, 1, 1, 0);         // ..4E..59 -> ..99
        } else if (w2x_area_safe_point_004be5e0(recovered_object_area_004cb120(c, nullptr), nullptr, vat(st, 0x5c),
                                                addr(pathfind(c))) == 1) {          // 0051CA5B..77
            recovered_object_position_004cd1a0(c, nullptr, vat(st, 0x5c), 1, 1, 0); // ..8F..9B
        } else {
            at<I>(st, 0x44) += 1;                                                   // ..79
        }
    } else if (direct_line(c, vat(st, 0x5c), position, &radius, band, nullptr) == 1) { // 0051CA7E..8D
        recovered_object_position_004cd1a0(c, nullptr, vat(st, 0x5c), 1, 1, 0);     // ..8F..9B
    }
    w5_server_update_subareas_on_move_to_004aead0(server_app(), nullptr, at<U>(c, 4), &old_position, position); // 0051CAA0..B2
    const Vector now = load(position);                                              // ..B7..DA [E-3C]
    Vector g;                                                                       // [E-18]
    g.x = static_cast<float>(static_cast<double>(now.x) - old_position.x);          // ..C5..D6
    g.y = static_cast<float>(static_cast<double>(now.y) - old_position.y);          // ..DE..EE
    g.z = 0.0f;                                                                     // ..E2
    const float moved = static_cast<float>(kotor_cdecl_004ab100(&g));               // ..F2..F7 [E-44]
    void* leader = frame_GetCharacter_006346c0(party, nullptr, 0);                  // ..FB..04
    Vector h;                                                                       // [E-3C]
    h.z = static_cast<float>(static_cast<double>(at<float>(leader, 0x2c)) - position->z); // ..09..0C
    h.y = static_cast<float>(static_cast<double>(at<float>(leader, 0x28)) - position->y); // ..12..19
    h.x = static_cast<float>(static_cast<double>(at<float>(leader, 0x24)) - position->x); // ..1C..22
    Vector dir;                                                                     // [E-24]
    normalize_004ab180(&dir, &h);                                                   // ..24..32
    if (g.x == k_zero_0073d700 && g.y == k_zero_0073d700) {                         // ..37..5E (test ah,44h; jp)
        const float dot = static_cast<float>((static_cast<double>(dir.y) * at<float>(c, 0xa0) +
                                              static_cast<double>(dir.z) * at<float>(c, 0xa4)) +
                                             static_cast<double>(dir.x) * at<float>(c, 0x9c)); // ..64..88 [E-48]
        const double head = kotor_fastcall_0060e250(recovered_creature_client_creature_004efb20(c, nullptr), nullptr); // ..8C..93
        double cosine;
        { const double a = head * k_deg_to_rad_007415f8; __asm { fld qword ptr a } __asm { fcos } __asm { fstp qword ptr cosine } } // ..98..9E
        bool face = false;
        if (static_cast<double>(dot) < cosine && static_cast<double>(dot) >= k_zero_0073d700) face = true; // ..A0..BC -> ..DE
        else if (!(-cosine >= static_cast<double>(dot) || std::isnan(dot) || std::isnan(cosine)) &&
                 !(dot > k_zero_0073d700 || std::isnan(dot))) face = true;          // 0051CBBE..DA -> ..E0
        if (face) copy_vector(bytes(st) + 0x4c, &dir);                              // 0051CBE0..F4
        const U leader_id = at<U>(frame_GetCharacter_006346c0(party, nullptr, 0), 4); // ..F7..02
        kotor_fastcall_006146e0(recovered_creature_client_creature_004efb20(c, nullptr), nullptr, leader_id, 8.0f); // ..05..14
    } else {
        kotor_fastcall_006146e0(recovered_creature_client_creature_004efb20(c, nullptr), nullptr, k_object_invalid, 8.0f); // 0051CC1B..2E
        w1x_object_set_orientation_004cd170(c, nullptr, normalize_004ab180(&h, &g)); // ..33..48 (out over [E-3C])
        at<U>(st, 0x44) = 0;                                                        // ..4D
        copy_vector(bytes(st) + 0x4c, bytes(c) + 0x9c);                             // ..54..6A
    }
    double v = (static_cast<double>(moved) * k_million_00743750) / fild_unsigned(at<U>(c, 0xd0)); // 0051CC6D..8D
    const float last = at<float>(st, 0x48);                                         // ..8F..92 [E-44]
    const float v_f = static_cast<float>(v);                                        // ..9C fst [E-48]
    if (!(v <= k_ten_thousandth_00747d98)) v = (static_cast<double>(last) + v_f) * k_half_0073e9ac; // ..96..B1 (jnp: <, ==)
    if (v < k_ten_007455c8) v = k_zero_0073d700;                                    // ..B7..C6 (test ah,5; jp)
    if (base_speed < v) v = base_speed;                                             // ..CC..DB
    at<float>(st, 0x48) = static_cast<float>(v);                                    // ..E1
    return result;
}

}  // extern "C"
