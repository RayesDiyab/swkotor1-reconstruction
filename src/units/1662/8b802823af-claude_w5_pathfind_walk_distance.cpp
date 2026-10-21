// W5 pathfind-walk lane, part C (fork C of worker pathfind-walk): the per-frame walk step of the server creature.
//   00516630 CSWSCreature::WalkUpdateLocationDistance(dist, out_pos, out_dir, crossed_list)  thiscall RET 10h, EAX
//   00512FD0 CSWSCreature::WalkUpdateLocationTestDistance(from, to, out_point, out_id)       thiscall RET 10h, EAX
//   00512F10 CSWSCreature::ComputeSpeedFactor(float, int ms)                                 thiscall RET 8, ST0
//   004CC1C0 CSWSObject::AnimationWalking(word animation)                                    thiscall RET 4, EAX
//   004F0F90 CSWSCreature::GetPauseReadyAnimation()                                          thiscall RET 0, EAX
//   004CC440 CPathfindInformation::SetBlockingCreature(id)                                   thiscall RET 4
// Transcribed from the original swkotor.exe (read-only). Addresses in the comments are the original instructions.
// x87: double intermediates (53-bit precision control), float exactly where the original stores a dword.
#include "842d6b1618-w5_t3.hpp"
#include "4f622be2f2-w5_area_direct_line.hpp"
#include "ab3fc15780-w5_drive.hpp"
#include "e5e3766b7c-w5_subarea.hpp"

using namespace w5;

extern "C" {
// ---------------- existing providers (c05 map), exact C-linkage names ----------------
void* __fastcall recovered_object_render_004cc2b0(void* object, void*);                 // CSWSObject::GetClientObject [0886]
U __fastcall kotor_m15e_creature_get_pause_ready_animation_0060f020(void* creature, void*); // CSWCCreature::GetPauseReadyAnimation [0833]
void __fastcall kotor_m15e_slot008_void8_005b5e90(void* self, void*, U a, U b);         // 005B5E90 void_return RET 8 [0832]
U __fastcall kotor_fastcall_004aea40(void* server, void*);                              // CServerExoApp::GetPlayerCreatureId [0595]
void* __fastcall frame_R4_005ed690(void* client, void*);                                // CClientExoApp::GetInGameGui [0457]
void __fastcall kotor_fastcall_0062b110(void* gui, void*, U message);                   // CGuiInGame::SetCombatMessage RET 4
// ---------------- other workers (declare + call only) ----------------
double __fastcall kotor_fastcall_005111c0(void* creature, void*);                       // ComputeTotalWalkDistance (pathfind-plan), ST0
double __cdecl recovered_dot_product_00423c20(const Vector* a, const Vector* b);                  // dotProd (pathfind-plan), ST0
void* __fastcall kotor_fastcall_00514ab0(void* avoid, void*);                           // CAvoidCreature::CAvoidCreature (fork A)
I __fastcall kotor_fastcall_005d0840(void* avoid, void*, void* creature, void* other);  // PlotPathAroundCreature RET 8 (fork A)
// ---------------- defined here ----------------
U __fastcall kotor_fastcall_00516630(void* self, void*, float dist, Vector* out_pos, Vector* out_dir, void* list);
U __fastcall kotor_fastcall_00512fd0(void* self, void*, Vector* from, Vector* to, Vector* out_point, U* out_id);
double __fastcall kotor_fastcall_00512f10(void* self, void*, float dist, I ms);
U __fastcall kotor_fastcall_004cc1c0(void* object, void*, U animation);
U __fastcall kotor_fastcall_004f0f90(void* self, void*);
void __fastcall kotor_fastcall_004cc440(void* pf, void*, U id);
}

namespace {
constexpr U k_invalid_00746644 = 0x7f000000u;     // .rdata 00746644
constexpr I k_avoid_failed_0074c45c = 0;          // .rdata 0074C45C (PlotPathAroundCreature result)
constexpr I k_avoid_turn_0074c464 = -2;           // .rdata 0074C464 (FFFFFFFE)
constexpr U k_closed_00002726 = 0x2726;           // door state "closed" (CSWSDoor +D4)
constexpr U k_ai_event_script_007441dc = 10;      // .rdata 007441DC
const float k_tenth_0073e798 = 0.1f;              // .rdata 0073E798 (3DCCCCCD)
const float k_hundredth_0073dfec = 0.01f;         // .rdata 0073DFEC (3C23D70A)
const float k_eye_0073f414 = 1.5f;                // .rdata 0073F414
const float k_four_0073f404 = 4.0f;               // .rdata 0073F404
const float k_back_00746ffc = -0.707f;            // .rdata 00746FFC (BF34FDF4)
const float k_tiny_00747504 = 1e-05f;             // .rdata 00747504 (3727C5AC)
const float k_zero_0073d700 = 0.0f;               // .rdata 0073D700
const float k_one_0073d764 = 1.0f;                // .rdata 0073D764
const float k_1000_0073d6fc = 1000.0f;            // .rdata 0073D6FC
const float k_scale_00747c40 = 5e-07f;            // .rdata 00747C40 (350637BD)
const float k_milli_0073d708 = 0.001f;            // .rdata 0073D708 (3A83126F)
const char k_msg_00747d08[] = "aborted walking, Maximum number of bumps happened";
const char k_msg_00747cb8[] = "aborted walking, we are totaly blocked. can't get around this creature at all.";
const char k_msg_00747c70[] = "aborted walking, Bumped into this creature at this position already.";

inline U addr(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline unsigned char* bytes(void* p) { return static_cast<unsigned char*>(p); }
inline Vector* vat(void* p, U off) { return reinterpret_cast<Vector*>(bytes(p) + off); }
inline void* pathfind(void* c) { return at<void*>(c, 0x340); }
inline double sq(float f) { return static_cast<double>(f) * f; }
inline double x87_fsqrt(double v) { double r; __asm { fld qword ptr v } __asm { fsqrt } __asm { fstp qword ptr r } return r; }
// CSWSArea::ComputeHeight through W1's 0886 wrapper (portable provider contract: IEEE single bits)
inline float height(void* area, Vector* p, U flag) { return flt(w1x_area_height_bits_004bc380(area, nullptr, p, flag, nullptr)); }
// mov reg,[esi] ; GetPauseReadyAnimation ; call [reg+7Ch] (SetAnimation): the vtable is read before the call
inline void pause_ready(void* self) {
    void* const* vt = *static_cast<void* const* const*>(self);
    const U anim = kotor_fastcall_004f0f90(self, nullptr);
    reinterpret_cast<void(__fastcall*)(void*, void*, U)>(vt[0x7c / 4])(self, nullptr, anim);
}
// CPathfindInformation debug print (005B5E90 is an empty RET 8 in the shipped image)
inline void debug_print(void* self, const char* text) { kotor_m15e_slot008_void8_005b5e90(pathfind(self), nullptr, 1, addr(text)); }
}  // namespace

extern "C" {

// =========================================================================================================
// 004CC440 CPathfindInformation::SetBlockingCreature(id): +254 id; an invalid id also clears the +258 position
void __fastcall kotor_fastcall_004cc440(void* pf, void*, U id) {
    at<U>(pf, 0x254) = id;                                                          // 004CC44C
    if (id != k_invalid_00746644) return;                                           // 004CC447..52
    at<U>(pf, 0x258) = 0; at<U>(pf, 0x25c) = 0; at<U>(pf, 0x260) = 0;              // 004CC454..83 (via stack temps)
}

// 004CC1C0 CSWSObject::AnimationWalking(word animation)
U __fastcall kotor_fastcall_004cc1c0(void* object, void*, U animation) {
    (void)object;
    const U16 a = static_cast<U16>(animation);                                      // 004CC1C0 (ax)
    return (a == 0x2712 || a == 0x276d || a == 0x2795) ? 1u : 0u;                   // 004CC1C5..E1
}

// 004F0F90 CSWSCreature::GetPauseReadyAnimation: the client creature's choice, else 2710h (+1 when +4E0 is set)
U __fastcall kotor_fastcall_004f0f90(void* self, void*) {
    if (recovered_object_render_004cc2b0(self, nullptr) != nullptr) {              // 004F0F93..9A
        void* client = recovered_object_render_004cc2b0(self, nullptr);             // 004F0F9C..9E
        void* creature = vcall<void*>(client, 0x2c);                                // 004F0FA3..A7 AsSWCCreature
        if (creature != nullptr)                                                    // 004F0FAA..AC
            return kotor_m15e_creature_get_pause_ready_animation_0060f020(creature, nullptr); // 004F0FAE..B1 tail jump
    }
    return (at<U>(self, 0x4e0) != 0 ? 1u : 0u) + 0x2710;                            // 004F0FB6..C4
}

// =========================================================================================================
// 00512F10 CSWSCreature::ComputeSpeedFactor(float distance, int ms): ST0
double __fastcall kotor_fastcall_00512f10(void* self, void*, float dist, I ms) {
    const float total = static_cast<float>(kotor_fastcall_005111c0(self, nullptr)); // 00512F14..19 [esp+4]
    dist = static_cast<float>(static_cast<double>(dist) * k_1000_0073d6fc * k_scale_00747c40); // 00512F1D..2D (arg slot)
    double speed = k_one_0073d764;                                                  // 00512F31
    if (static_cast<double>(total) <= dist) {                                       // 00512F37..44 (test ah,41h; jp: >/NaN skip)
        double q = static_cast<double>(total) / dist;                               // 00512F48..4C
        if (!(q >= k_hundredth_0073dfec)) q = k_tenth_0073e798;                     // 00512F50..5B, 00512F61..63 (C0 incl. NaN)
        else q = x87_fsqrt(q);                                                      // 00512F5D
        speed = q - static_cast<double>(ms) * k_milli_0073d708;                     // 00512F69..73
    }
    if (static_cast<double>(at<float>(self, 0xb8)) <= k_one_0073d764) {             // 00512F75..86 (jp: >/NaN skip)
        const float cap = static_cast<float>(static_cast<double>(ms) * k_milli_0073d708 + at<float>(self, 0xb8)); // 00512F88..98
        if (speed >= cap) speed = cap;                                              // 00512F9C..A9 (C0/NaN keeps)
    }
    if (speed <= k_tenth_0073e798) speed = k_tenth_0073e798;                        // 00512FAD..BD (jp: >/NaN keeps)
    return speed;
}

// =========================================================================================================
// 00512FD0 CSWSCreature::WalkUpdateLocationTestDistance(from, to, out_point, out_id): 1 = the step is clear
U __fastcall kotor_fastcall_00512fd0(void* self, void*, Vector* from, Vector* to, Vector* out_point, U* out_id) {
    U result = 1;                                                                   // 00512FEA..F1 (EDI, [esp+C])
    if (recovered_object_area_004cb120(self, nullptr) == nullptr) return result;    // 00512FF5..FC -> 005132C3
    const U saved_target = at<U>(pathfind(self), 0x30);                             // 00513002..08 [E-58]
    at<U>(pathfind(self), 0x30) = k_invalid_00746644;                               // 0051300B..15
    double low_z, high_z;                                                           // 00513018..3A (ST order)
    if (from->z < to->z) { low_z = from->z; high_z = to->z; }                       // (test ah,5; jp: >=/NaN -> 00513037)
    else { low_z = to->z; high_z = from->z; }
    const float low = static_cast<float>(low_z - k_tenth_0073e798);                 // 00513043..53 [E-5C]
    const float high = static_cast<float>((static_cast<double>(at<float>(pathfind(self), 0x14)) + high_z) +
                                          k_tenth_0073e798);                        // 00513057..8E (stored in the arg-2 slot)
    U door_id = k_invalid_00746644;                                                 // [E-68]
    U creature_id = k_invalid_00746644;                                             // [E-60]
    WalkHit hit;                                                                    // [E-54] CWalkHitInfo, inline-zeroed
    std::memset(&hit, 0, sizeof(hit));                                              // 00513064..EF
    hit.blocker_40 = k_invalid_00746644;                                            // 00513072
    void* pf = pathfind(self);
    const float radius = at<float>(pf, 4);                                          // 005130FD..00 (stored in the arg-1 slot)
    const U ebp = area_no_nonwalk_polys_detailed_00506650(recovered_object_area_004cb120(self, nullptr), nullptr,
        from, to, low, high, radius, at<U>(pf, 0xbc), &door_id, 1, nullptr, 0, &hit, 0); // 005130F7..36
    if (ebp != 0) {                                                                 // 0051313B..3F
        if (w5x_area_no_creatures_on_line_004bc590(recovered_object_area_004cb120(self, nullptr), nullptr, from, to,
                pathfind(self), 1, 0, static_cast<I>(addr(&creature_id)), 1, &hit) != 0) // 00513141..6A
            goto done;
    }
    result = 0;                                                                     // 0051317D
    if (out_id != nullptr) *out_id = creature_id;                                   // 00513170..87
    if (out_point != nullptr) *out_point = hit.point_4;                             // 00513189..A5
    if (creature_id != k_invalid_00746644) {                                        // 005131A8..AD
        void* creature = kotor_fastcall_004ae770(server_app(), nullptr, creature_id); // 005131AF..B9
        if (creature == nullptr) goto done;                                         // 005131BE..C0
        // original argument order: (other, to, from)
        if (w5_creature_bump_friends_004f6390(self, nullptr, creature, to, from) == 0) goto done; // 005131C6..D2
        // Original quirk: the creature-id out slot of this second line test is the caller's out_id argument slot
        // (lea [esp+90h] = E+10h), which is overwritten and never read again.
        U arg_slot_10 = addr(out_id);
        if (w5x_area_no_creatures_on_line_004bc590(recovered_object_area_004cb120(self, nullptr), nullptr, from, to,
                pathfind(self), 1, 0, static_cast<I>(addr(&arg_slot_10)), 0, nullptr) == 1) // 005131D8..02
            result = 1;                                                             // 00513208
        goto done;
    }
    if (ebp != 0) goto done;                                                        // 00513211..13
    if (door_id == k_invalid_00746644) goto done;                                   // 00513219..22
    {
        void* door = w5_server_get_door_by_id_004ae7c0(server_app(), nullptr, door_id); // 00513224..2E
        if (door == nullptr || at<U>(door, 0xd4) != k_closed_00002726) goto done;   // 00513233..41
        void* memory = live_operator_new(0x34);                                     // 00513243..4A
        void* event = memory != nullptr ? recovered_event_construct_004d7540(memory, nullptr) : nullptr; // 00513251..62
        at<U16>(event, 0) = 0x1f;                                                   // 00513265 (unchecked null kept)
        recovered_event_enqueue_004b08d0(ai_master(), nullptr, 0, 0, door_id, at<U>(self, 4),
                                         k_ai_event_script_007441dc, event);        // 0051326A..98 (door -> self)
    }
done:
    at<U>(pathfind(self), 0x30) = saved_target;                                     // 0051329D..AA
    return result;
}

// =========================================================================================================
// 00516630 CSWSCreature::WalkUpdateLocationDistance(dist, out_pos, out_dir, crossed_list)
//   dist      : distance to walk this frame (float, consumed segment by segment)
//   out_pos   : receives the new position (unchanged position when stopped)
//   out_dir   : receives the new facing (starts as the current orientation +9C)
//   list      : CExoArrayList of subareas crossed (IntersectingLineSegment output)
//   EAX       : 1 = end of the path reached / walk stopped, 0 = still walking
// The creature position itself is not written here (the caller applies out_pos).
U __fastcall kotor_fastcall_00516630(void* self, void*, float dist, Vector* out_pos, Vector* out_dir, void* list) {
    void* const area = recovered_object_area_004cb120(self, nullptr);               // 00516639 [E-E4]
    if (area == nullptr) return 0;                                                  // 0051663E..4D (EAX = GetArea = 0)
    const I count = at<I>(pathfind(self), 0x8c);                                    // 00516656 [E-D4]
    I index = at<I>(pathfind(self), 0x9c);                                          // 0051665C (EBP)
    float remaining = dist;                                                         // 00516662..7D [E-E8]
    Vector* const pos = vat(self, 0x90);                                            // EDI
    Vector cur = *pos;                                                              // 00516681..8F [E-F4]
    *out_dir = *vat(self, 0x9c);                                                    // 00516693..AD
    const float face_x = vat(self, 0x9c)->x, face_y = vat(self, 0x9c)->y;           // 00516730..37 [E-8C], [E-88]
    Vector seg_point = {0.0f, 0.0f, 0.0f};                                          // [E-C8]
    Vector delta = {0.0f, 0.0f, 0.0f};                                              // [E-100]
    U blocker = 0;                                                                  // [E-D0] (written by TestDistance)
    U finished = (index == count || count == 0) ? 1u : 0u;                          // 005166B0..46 [E-CC]

    // ---- 0051674E: close enough to the interaction target -> stop
    {
        const U target = at<U>(pathfind(self), 0x30);                               // 0051674E..5D
        if (target != k_invalid_00746644) {
            Vector use = {0.0f, 0.0f, 0.0f};                                        // [E-110]
            float range = 0.0f;                                                     // [E-114]
            if (w5_creature_get_use_range_004ee440(self, nullptr, target, &use, &range, 0) != 0) { // 00516763..9B
                const double dx = static_cast<double>(pos->x) - use.x;              // 0051679D..A9
                const double dy = static_cast<double>(pos->y) - use.y;
                const double dz = static_cast<double>(pos->z) - use.z;
                const double sum = (dz * dz + dy * dy) + dx * dx;                   // 005167B1..BF
                if (sq(range) >= sum) goto pause_and_stop;                          // 005167C1..D6 (test ah,1: C0/NaN skip)
            }
        }
    }
    // ---- 00516810: line-of-sight early stop (pathfind +240, MoveToPoint flag bit 10)
    if (at<U>(pathfind(self), 0x240) != 0) {                                        // 00516810..1E
        Vector use = {0.0f, 0.0f, 0.0f};                                            // [E-B0]
        float range = 0.0f;                                                         // [E-114]
        const U target = at<U>(pathfind(self), 0x30);                               // 0051684D..55
        if (target != k_invalid_00746644 &&
            w5_creature_get_use_range_004ee440(self, nullptr, target, &use, &range, 0) != 0) { // 0051685B..74
            void* const pfb = pathfind(self);                                       // 0051687A (EBX)
            if (static_cast<double>(at<float>(pfb, 0x68)) > range) {                // 00516880..8C (test ah,41h)
                Vector d;                                                           // [E-110]
                kotor_cdecl_00423bc0(&d, pos, &use);                                // 00516892..A0
                Vector eye = {pos->x, pos->y, static_cast<float>(static_cast<double>(pos->z) + k_eye_0073f414)}; // 005168A5..CF [E-E0]
                Vector aim = {use.x, use.y, static_cast<float>(static_cast<double>(use.z) + k_eye_0073f414)};    // 005168BD..F7 [E-120]
                Vector clear = {0.0f, 0.0f, 0.0f};                                  // 005168FB..11 [E-BC]
                const double sum = (sq(d.z) + sq(d.y)) + sq(d.x);                   // 0051691F..3C
                if (sq(at<float>(pfb, 0x68)) >= sum) {                              // 0051693E..4B (test ah,1)
                    unsigned los_out;                                               // [E-104]
                    if (area_clear_line_of_sight_0050c330(recovered_object_area_004cb120(self, nullptr), nullptr, &eye,
                            &aim, &clear, &los_out, at<U>(self, 4), at<U>(pfb, 0x30), 0, 0) != 0) { // 00516951..84
                        const float r = at<float>(pathfind(self), 4);               // 00516986..90 (also [E-D0])
                        blocker = bits(r);
                        const float hi = static_cast<float>(static_cast<double>(eye.z) + k_hundredth_0073dfec); // 0051698C..B3
                        const float lo = static_cast<float>(static_cast<double>(eye.z) - k_hundredth_0073dfec); // 005169BE..D5
                        if (area_no_nonwalk_polys_detailed_00506650(recovered_object_area_004cb120(self, nullptr), nullptr,
                                &eye, &aim, lo, hi, r, k_invalid_00746644, &los_out, 1, nullptr, 0, nullptr, 0) == 1) // 00516999..E9
                            goto pause_and_stop;                                    // -> 00517058
                    }
                }
            }
        }
        if (count > 2) {                                                            // 005169EF..F6 (jle)
            void* pf = pathfind(self);
            const float* pts = at<const float*>(pf, 0x90);                          // 005169FC..02
            const Vector last = {pts[count - 2], static_cast<float>(static_cast<double>(pts[count - 1])), 0.0f}; // 00516A08..20 [E-110]
            const Vector here = *pos;                                               // 00516A1C..45 [E-120]
            const double dx = static_cast<double>(last.x) - here.x;                 // 00516A24..2C
            const double dy = static_cast<double>(last.y) - here.y;                 // 00516A33..3E
            const double sum = dy * dy + dx * dx;                                   // 00516A49..51
            if (sq(at<float>(pf, 0x68)) >= sum) {                                   // 00516A42..64 (test ah,1)
                Vector eye = {pos->x, pos->y, static_cast<float>(static_cast<double>(pos->z) + k_eye_0073f414)}; // 00516A6A..92 [E-E0]
                Vector aim = {last.x, last.y, 0.0f};                                // 00516A82..AD [E-120]
                aim.z = static_cast<float>(static_cast<double>(height(recovered_object_area_004cb120(self, nullptr), &aim, 0)) +
                                           k_eye_0073f414);                         // 00516A9A..CA (see quirk: float-bits provider)
                Vector clear = {0.0f, 0.0f, 0.0f};                                  // 00516AED..03 [E-BC]
                if (area_clear_line_of_sight_0050c330(recovered_object_area_004cb120(self, nullptr), nullptr, &eye, &aim,
                        &clear, &blocker, at<U>(self, 4), k_invalid_00746644, 0, 0) != 0) { // 00516AC3..1C
                    const float r = at<float>(pathfind(self), 4);                   // 00516B1E..28 (also [E-104])
                    const float hi = static_cast<float>(static_cast<double>(eye.z) + k_hundredth_0073dfec); // 00516B24..4B
                    const float lo = static_cast<float>(static_cast<double>(eye.z) - k_hundredth_0073dfec); // 00516B53..68
                    if (area_no_nonwalk_polys_detailed_00506650(recovered_object_area_004cb120(self, nullptr), nullptr,
                            &eye, &aim, lo, hi, r, k_invalid_00746644, &blocker, 1, nullptr, 0, nullptr, 0) == 1) // 00516B31..7E
                        goto pause_and_stop;                                        // -> 005167D8
                }
            }
        }
    }

    // ---- 00516B84: walk the path points (+90 float pairs, +9C current index)
    while (index < count) {                                                         // 00516B8C..8E (jge -> 005172A8)
        const float* pts = at<const float*>(pathfind(self), 0x90);                  // 00516B94..9A
        seg_point.x = pts[index];                                                   // 00516BA4..AA (dword copy)
        seg_point.y = static_cast<float>(static_cast<double>(pts[index + 1]));      // 00516BA0..AE (fld/fstp)
        seg_point.z = 0.0f;                                                         // 00516BDC..E8
        delta.x = static_cast<float>(static_cast<double>(seg_point.x) - cur.x);     // 00516BB2..D4
        delta.y = static_cast<float>(static_cast<double>(seg_point.y) - cur.y);     // 00516BD0..FA
        delta.z = 0.0f;                                                             // 00516BF0..16 (-cur.z stored, then 0)
        const float seg2 = static_cast<float>(sq(delta.y) + sq(delta.x));           // 00516C02..24 [E-104]
        const double dist2 = sq(remaining);                                         // 00516C28..2C
        const float dist2f = static_cast<float>(dist2);                             // 00516C30 fst [E-E4]
        float step;                                                                 // [E-114]
        if (dist2 > seg2 && !(seg2 == k_zero_0073d700))                             // 00516C34..4E (unrounded ST0; NaN -> dist)
            step = static_cast<float>(x87_fsqrt(seg2));                             // 00516C50..56
        else
            step = remaining;                                                       // 00516C5C..60
        Vector p0 = cur;                                                            // 00516C64..8C [E-A4]
        p0.z = height(area, &p0, 1);                                                // 00516C70..98
        Vector n;                                                                   // [E-B0]
        const Vector* nd = normalize_004ab180(&n, &delta);                          // 00516C9F..AC
        const double sx = static_cast<double>(step) * nd->x;                        // 00516CB1..B5
        const double sy = static_cast<double>(step) * nd->y;                        // 00516CBA..C0
        const float sz = static_cast<float>(static_cast<double>(step) * nd->z);     // 00516CC5..CC [E-B4]
        Vector p1;                                                                  // [E-98] (copies of [E-E0]/[E-DC]/[E-D8])
        p1.x = static_cast<float>(sx + cur.x);                                      // 00516CD3..E1
        p1.y = static_cast<float>(sy + cur.y);                                      // 00516CE8..07
        p1.z = static_cast<float>(static_cast<double>(sz) + cur.z);                 // 00516CF8..18
        p1.z = height(area, &p1, 1);                                                // 00516CEC..24
        if (seg2 > k_zero_0073d700 && step > k_zero_0073d700) {                     // 00516D2B..4B (test ah,41h)
            Vector hit_point;                                                       // [E-80]
            if (kotor_fastcall_00512fd0(self, nullptr, &p0, &p1, &hit_point, &blocker) == 0) goto bumped; // 00516D4D..73
        }
        if (dist2f <= seg2) {                                                       // 00516D79..86 (jnp: < or ==)
            // ---- 00516E70: the remaining distance ends on this segment
            if (seg2 == k_zero_0073d700) { finished = 0; break; }                   // 00516E70..7F -> 00516F5D
            Vector m;                                                               // [E-80]
            const Vector* md = normalize_004ab180(&m, &delta);                      // 00516E85..92
            const double ex = static_cast<double>(remaining) * md->x;               // 00516E97..9B
            const double ey = static_cast<double>(remaining) * md->y;               // 00516EA4..AB
            const float ez = static_cast<float>(static_cast<double>(remaining) * md->z); // 00516EAF..C0 [E-B4]
            Vector end;                                                             // [E-120]
            end.x = static_cast<float>(ex + cur.x);                                 // 00516EC7..CF
            end.y = static_cast<float>(ey + cur.y);                                 // 00516ED3..D7
            end.z = static_cast<float>(static_cast<double>(ez) + cur.z);            // 00516EDB..E6
            w5_area_intersecting_line_segment_004bf2c0(area, nullptr, &cur, &end, list); // 00516EAE..EA
            Vector m2;                                                              // [E-80]
            *out_dir = *normalize_004ab180(&m2, &delta);                            // 00516EEF..1B
            cur = end;                                                              // 00516F17..3C
            cur.z = height(recovered_object_area_004cb120(self, nullptr), &cur, 1); // 00516F29..4C
            finished = 0;                                                           // 00516F50
            break;                                                                  // -> 005172AF
        }
        // ---- 00516D8C: this segment is consumed entirely
        w5_area_intersecting_line_segment_004bf2c0(area, nullptr, &cur, &seg_point, list); // 00516D8C..A0
        remaining = static_cast<float>(static_cast<double>(remaining) - kotor_cdecl_004ab100(&delta)); // 00516DA5..BF
        cur = seg_point;                                                            // 00516DB3..D9
        cur.z = height(recovered_object_area_004cb120(self, nullptr), &cur, 1);     // 00516DC6..E9
        index += 2;                                                                 // 00516DF7
        at<I>(pathfind(self), 0x9c) = index;                                        // 00516DF1..FC
        if (index == count) {                                                       // 00516DFA..02
            const double s = (sq(delta.z) + sq(delta.y)) + sq(delta.x);             // 00516E08..22
            if (!(s == k_zero_0073d700)) {                                          // 00516E24..2F (jnp: equal skips)
                Vector m;                                                           // [E-80]
                *out_dir = *normalize_004ab180(&m, &delta);                         // 00516E35..60
            }
            finished = 1;                                                           // 00516E63 / 00516F78
            break;
        }
    }

    // ---- 005172AF: two-point paths: turn around instead of walking backwards
    if (count == 4) {                                                               // 005172AF..B4
        const float* pts = at<const float*>(pathfind(self), 0x90);                  // 005172BA..C0
        seg_point.x = pts[2];                                                       // 005172C9..E0
        seg_point.y = static_cast<float>(static_cast<double>(pts[3]));              // 005172C6..D0
        delta.x = static_cast<float>(static_cast<double>(seg_point.x) - pos->x);    // 005172D4..07
        delta.y = static_cast<float>(static_cast<double>(seg_point.y) - pos->y);    // 005172F4..1E
        delta.z = 0.0f;                                                             // 00517315..31 (-pos.z stored, then 0)
        if (kotor_fastcall_004cc1c0(self, nullptr, at<U>(self, 0xd4)) != 0) {       // 0051730B..40
            const double s = (sq(delta.z) + sq(delta.y)) + sq(delta.x);             // 00517346..60
            if (s <= k_four_0073f404) {                                             // 00517362..6D (jp: >/NaN skip)
                const double facing = static_cast<double>(face_y) * out_dir->y +
                                      static_cast<double>(face_x) * out_dir->x;     // 0051736F..82
                if (facing < k_back_00746ffc) {                                     // 00517384..8F
                    const float nx = static_cast<float>(-static_cast<double>(out_dir->x)); // 00517391..A1
                    out_dir->x = nx;                                                // 005173AC
                    const float ny = static_cast<float>(-static_cast<double>(out_dir->y)); // 005173A9..B0
                    out_dir->y = ny;                                                // 005173B8
                    out_dir->z = 0.0f;                                              // 005173BB
                    if (at<U>(self, 0xd4) != 0x2713) vcall<void>(self, 0x7c, 0x2713u); // 005173BE..D3 SetAnimation
                }
            }
        }
    }
    *out_pos = cur;                                                                 // 005173D6..F5
    return finished;

pause_and_stop:                                                                     // 005167D8 / 00517058
    pause_ready(self);
    *out_pos = *pos;
    return 1;

bumped: {                                                                           // 00516F85
    void* pf = pathfind(self);
    const I bumps = at<I>(pf, 0x268) + 1;                                           // 00516F85..95
    at<I>(pf, 0x268) = bumps;
    if (bumps > 5) {                                                                // 00516F9B (jle)
        at<I>(pathfind(self), 0x8c) = 0;                                            // 00516F9D..A5
        live_free(at<void*>(pathfind(self), 0x90));                                 // 00516FAB..B8 (unconditional)
        at<void*>(pathfind(self), 0x90) = nullptr;                                  // 00516FBD..C3
        pause_ready(self);                                                          // 00516FC9..D8
        debug_print(self, k_msg_00747d08);                                          // 00516FDB..E8
        *out_pos = *pos;                                                            // 00516FED..04
        return 1;
    }
    const U target = at<U>(pathfind(self), 0x30);                                   // 00517016..1C
    if (target != k_invalid_00746644 && blocker == target) {                        // 0051701F..2D
        at<I>(pathfind(self), 0x8c) = 0;                                            // 0051702F..31
        live_free(at<void*>(pathfind(self), 0x90));                                 // 00517037..44
        at<void*>(pathfind(self), 0x90) = nullptr;                                  // 00517049..52
        goto pause_and_stop;                                                        // 00517058
    }
    void* const creature = kotor_fastcall_004ae770(server_app(), nullptr, blocker); // 00517090..9E (EBP)
    if (creature == nullptr) goto bumped_again;                                     // 005170A0..A2
    if (blocker == at<U>(pathfind(self), 0x254)) {                                  // 005170A8..B4
        // Original: both differences are written over the entry orientation copy ([E-8C], [E-80]); that copy is
        // read only on the count==4 tail, which this path never reaches.
        Vector* last = vat(pathfind(self), 0x258);                                  // 005170BC
        Vector a, b;                                                                // [E-80], [E-8C]
        const Vector* pa = kotor_cdecl_00423bc0(&a, last, vat(creature, 0x90));     // 005170B6..CF
        const Vector* pb = kotor_cdecl_00423bc0(&b, last, vat(creature, 0x90));     // 005170D4..E6
        if (!(recovered_dot_product_00423c20(pb, pa) > k_tiny_00747504)) goto bumped_again;   // 005170EE..02 (<=, ==, NaN)
    }
    *out_pos = *pos;                                                                // 00517108..20
    alignas(4) unsigned char avoid[0x74];                                           // [E-74] CAvoidCreature
    kotor_fastcall_00514ab0(avoid, nullptr);                                        // 00517123..2A
    at<U>(avoid, 0) = at<U>(pathfind(self), 0x244);                                 // 0051712F..44
    const I plotted = kotor_fastcall_005d0840(avoid, nullptr, self, creature);      // 0051713B..4B [E-E4]
    if (plotted == k_avoid_failed_0074c45c) {                                       // 00517150..5A
        kotor_fastcall_004cc440(pathfind(self), nullptr, k_invalid_00746644);       // 00517160..6B
        pause_ready(self);                                                          // 00517170..7C
        debug_print(self, k_msg_00747cb8);                                          // 0051717F..8C
        *out_pos = *pos;                                                            // 00517191..9E
        if (at<U>(self, 4) == kotor_fastcall_004aea40(server_app(), nullptr))       // 005171A1..B4
            kotor_fastcall_0062b110(frame_R4_005ed690(client_app(), nullptr), nullptr, 0xbaf3); // 005171B6..CB
        return 1;
    }
    if (at<U>(pathfind(self), 0x244) != at<U>(avoid, 0)) at<U>(pathfind(self), 0x244) = at<U>(avoid, 0); // 005171E2..F7
    const double dot = recovered_dot_product_00423c20(vat(self, 0x9c), vat(creature, 0x9c));   // 005171FD..10
    if (dot < k_zero_0073d700)                                                      // 00517210..24 (test ah,5; jp)
        at<U>(pathfind(creature), 0x244) = at<U>(pathfind(self), 0x244);            // 00517226..32
    else
        at<U>(pathfind(creature), 0x244) = at<U>(pathfind(self), 0x244) == 0 ? 1u : 0u; // 0051723A..4D
    if (plotted == k_avoid_turn_0074c464) {                                         // 00517253..5D
        pause_ready(self);                                                          // 0051725F..6B
        return 1;
    }
    return 0;                                                                       // 00517280..8C
}
bumped_again:                                                                       // 0051728F
    pause_ready(self);
    debug_print(self, k_msg_00747c70);                                              // 0051729E -> 00516FE0
    *out_pos = *pos;
    return 1;
}

}  // extern "C"
