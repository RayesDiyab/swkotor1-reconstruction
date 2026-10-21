// W5 t2: CSWPlayerControl / CSWPlayerControlCamRelative (00679xxx), CSWMGPhysicsState helpers (006707xx..), and
// the CSWCCreature members that the keyboard drive calls directly.
// CSWPlayerControlCamRelative layout (ctor 00679780, size 0CCh):
//   +00 vtable 00752D34   +04 player client id (7F000000 none)   +08 camera   +0C enabled
//   +10 up/down axis       +14 left/right axis                    +18 walking   +1C moving direction (Vector)
//   +28..+30 zero          +34 480.0 (Init)   +38/+3C turn rates (2DA, Init)   +40 turn sign (+-1)
//   +44 new PhysicsState   +5C current PhysicsState {velocity, position}       +74 previous input target
//   +80 CSWRK4SplitAcceleration (W4 ctor 006D10B0)                             +C0/+C4/+C8 keyboard camera options
#include "f49e249f4a-w5_t2.hpp"
#include <cmath>
#include <cfloat>

using namespace w5;

extern "C" {
U __fastcall recovered_object_get_reputation_0057cb80(void*,void*,U,U);
extern float kotor_cell_turn_rate_007a25f4; // existing900.0 canonicalW4cell; no duplicate storage
}


extern "C" {
void* __cdecl pointto_004acca0(void*,const void*);
double __cdecl kotor_cdecl_004acb90(Quaternion);
double __cdecl quaternion_pitch_004aa020(const Quaternion*);
double __cdecl quaternion_yaw_004a9f40(const Quaternion*);
PhysicsState* __fastcall kotor_fastcall_006d1190(void*,void*,PhysicsState*,Vector,float,U,U,U,U);
extern U recovered_camera_reset_cell_007a2288;
}

namespace {
// Original mode5 Roll/Pitch/Yaw sequence: retain provider ST0 until each original FSTP DWORD.
Quaternion* camera_mode5_orientation(Quaternion* out,const Vector* direction,U angle_bits) {
    Quaternion q;
    pointto_004acca0(&q,direction);
    float roll_value, pitch_value, yaw_value;
    Quaternion* returned;
    __asm {
        lea eax, q
        push dword ptr [eax+12]
        push dword ptr [eax+8]
        push dword ptr [eax+4]
        push dword ptr [eax]
        call kotor_cdecl_004acb90
        add esp, 16
        fstp dword ptr roll_value
        lea eax, q
        push eax
        call quaternion_pitch_004aa020
        add esp, 4
        fstp dword ptr pitch_value
        lea eax, q
        push eax
        call quaternion_yaw_004a9f40
        add esp, 4
        fadd dword ptr angle_bits
        fstp dword ptr yaw_value
        push dword ptr roll_value
        push dword ptr pitch_value
        push dword ptr yaw_value
        push out
        call yaw_pitch_roll_004acac0
        add esp, 16
        mov returned, eax
    }
    return returned;
}

inline void* client_creature(U id) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_R4_005ed5b0(client_app(), nullptr, id))); }
inline Vector& vec(void* o, U off) { return at<Vector>(o, off); }
inline PhysicsState& state(void* o, U off) { return at<PhysicsState>(o, off); }
constexpr U k_invalid = 0x7f000000u;
const float k_one_0073d764 = 1.0f;
const float k_zero_0073d700 = 0.0f;
const float k_fallback_accel_006798a9 = 25.0f;     // 0x41C80000 immediate
const float k_walk_speed_limit_00752d68 = 1.8f;    // bits 3FE66666
const float k_walk_accel_scale_00752d64 = 0.5555556f; // bits 3F0E38E4
const float k_default_max_speed_007455c0 = 6.0f;
const float k_drive_accel_running_00747f6c = 15.0f;
const float k_180_0073e408 = 180.0f;
const float k_360_00741868 = 360.0f;
const double k_turn_epsilon_00752d70 = 0.10000000149011612;   // qword
const double k_stop_speed_00749d88 = 0.25;                    // qword
const float k_max_speed_floor_00743fdc = 5000.0f;
const U k_move_threshold_bits = 0x3a8ebcc7u;                  // 0067A388 immediate (0.0010889)
const char k_max_turn_rate_00752cf8[] = "MaxTurnRate";        // .rdata 00752CF8 (2DA column -> +3C)
const char k_min_turn_rate_00752cec[] = "MinTurnRate";        // .rdata 00752CEC (2DA column -> +38)
// Original0067A597..61E: retain exact ST0/FST widths, wrap predicates and turn-rate clamp.
U camera_mode6_turn_bits(const Vector* target_direction,const Vector* current_orientation,float elapsed) {
    float target_yaw;
    U result;
    const float negative_half_turn = -180.0f; // original74F4F0 C3340000
    __asm {
        push target_direction
        call kotor_m15a_yaw_004aa0f0
        add esp, 4
        fstp dword ptr target_yaw
        push current_orientation
        call kotor_m15a_yaw_004aa0f0
        add esp, 4
        fsubr dword ptr target_yaw
        fcom dword ptr k_180_0073e408
        fst dword ptr result
        fnstsw ax
        test ah, 41h
        jnz check_negative_wrap
        fsub dword ptr k_360_00741868
        fst dword ptr result
    check_negative_wrap:
        fcom dword ptr negative_half_turn
        fnstsw ax
        test ah, 5
        jp compare_sign
        fadd dword ptr k_360_00741868
        fst dword ptr result
    compare_sign:
        fcomp dword ptr k_zero_0073d700
        fld dword ptr kotor_cell_turn_rate_007a25f4
        fmul dword ptr elapsed
        fnstsw ax
        test ah, 5
        jp clamp_positive
        fchs
        fld dword ptr result
        fcomp st(1)
        fnstsw ax
        test ah, 5
        jp retain_delta
        fstp dword ptr result
        jmp clamp_done
    clamp_positive:
        fld dword ptr result
        fcomp st(1)
        fnstsw ax
        test ah, 41h
        jz replace_delta
    retain_delta:
        fstp st(0)
        jmp clamp_done
    replace_delta:
        fstp dword ptr result
    clamp_done:
    }
    return result;
}
}  // namespace

extern "C" {
U w5_cell_max_speed_override_00833bac = 0;        // .bss; no writer in swkotor.exe (only reader 00679510)
float w5_cell_max_speed_override_007a25f8 = 15.0f; // .data initial 15.0; no writer (only reader 006795B2)

// Original005EDD80..89: GetLastTarget, raw client ID; no added null/fallback branch.
U __fastcall kotor_fastcall_005edd80(void* self,void*) {
    return at<U>(at<void*>(self,4),0x2b4);
}

// ---------------- vtables (00752D04 base, 00752D34 CamRelative) ----------------
// Base slots point at shared existing providers; they are void/zero leaf functions used by many vtables.
U __fastcall behavior_void_return_4_0060e760(void*, void*, U);          // 0060E760 ret 4
U __fastcall kotor_m15e_slot128_false4_0063e7c0(void*, void*, U);       // 0063E7C0 xor eax,eax; ret 4
void __fastcall kotor_m15e_slot090_void0_004015a0(void*, void*);        // 004015A0 ret
U __fastcall packed_file_get_async_file_00644760(void*, void*);         // 00644760 mov eax,[ecx+18h]; ret

void* const w5_vtable_00752d04[12] = {
    reinterpret_cast<void*>(&behavior_void_return_4_0060e760),        // +00 UpDown
    reinterpret_cast<void*>(&w5_player_control_return_zero_006796f0),  // +04 GetUpDown
    reinterpret_cast<void*>(&behavior_void_return_4_0060e760),        // +08 LeftRight
    reinterpret_cast<void*>(&w5_player_control_return_zero_006796f0),  // +0C GetLeftRight
    reinterpret_cast<void*>(&behavior_void_return_4_0060e760),        // +10 SetPlayerWalking
    reinterpret_cast<void*>(&kotor_m15e_slot010_zero_0063e7f0),       // +14 return_zero
    reinterpret_cast<void*>(&kotor_m15e_slot090_void0_004015a0),      // +18 void
    reinterpret_cast<void*>(&w5_player_control_return_zero_006796f0),  // +1C GetCurSpeed
    reinterpret_cast<void*>(&behavior_void_return_4_0060e760),        // +20
    reinterpret_cast<void*>(&behavior_void_return_4_0060e760),        // +24
    reinterpret_cast<void*>(&kotor_m15e_slot128_false4_0063e7c0),     // +28 Control -> false (ret 4)
    reinterpret_cast<void*>(&kotor_m15e_slot090_void0_004015a0),      // +2C ResetDriveAcceleration -> void
};
void* const w5_vtable_00752d34[12] = {
    reinterpret_cast<void*>(&w5_player_control_up_down_00679700),
    reinterpret_cast<void*>(&w5_player_control_get_up_down_00679710),
    reinterpret_cast<void*>(&w5_player_control_left_right_00679720),
    reinterpret_cast<void*>(&w5_player_control_get_left_right_00679730),
    reinterpret_cast<void*>(&w5_player_control_set_player_walking_00679740),
    reinterpret_cast<void*>(&packed_file_get_async_file_00644760),    // +14 GetProperty0x18 (returns +18 walking)
    reinterpret_cast<void*>(&kotor_m15e_slot090_void0_004015a0),      // +18
    reinterpret_cast<void*>(&w5_player_control_get_cur_speed_00679750),
    reinterpret_cast<void*>(&behavior_void_return_4_0060e760),        // +20
    reinterpret_cast<void*>(&behavior_void_return_4_0060e760),        // +24
    reinterpret_cast<void*>(&w5_player_control_control_00679940),
    reinterpret_cast<void*>(&w5_player_control_reset_drive_acceleration_006795c0),
};

// 006796F0 (fld [0073D700]; ret)
double __fastcall w5_player_control_return_zero_006796f0(void*, void*) { return k_zero_0073d700; }

// 006792D0 CSWPlayerControl::Initialize
void __fastcall w5_player_control_initialize_006792d0(void* self, void*) {
    at<U>(self, 4) = k_invalid; at<U>(self, 8) = 0; at<U>(self, 0xc) = 0;
}

// 00679340 CSWPlayerControl::SetPlayer(id)
void __fastcall kotor_fastcall_00679340(void* self, void*, U object_id) {
    if (at<U>(self, 4) != k_invalid) {                                                     // 00679343..4B
        if (void* old = client_creature(at<U>(self, 4)))                                   // 0067934D..5E
            kotor_fastcall_0060f090(old, nullptr, 0);                                      // 00679360..69 ([0074F220]=0)
    }
    at<U>(self, 4) = object_id;                                                            // 00679377
    if (object_id == k_invalid || at<U>(self, 0xc) == 0) return;                           // 0067937A..81
    if (void* now = client_creature(object_id))                                            // 00679383..94
        kotor_fastcall_0060f090(now, nullptr, 1);                                          // 00679396..A3 ([0074F224]=1)
}

// 006793B0 CSWPlayerControl::SetCamera
void __fastcall kotor_fastcall_006793b0(void* self, void*, void* camera) { at<void*>(self, 8) = camera; }

// 006793C0 CSWPlayerControlCamRelative::Init
void __fastcall w5_player_control_init_006793c0(void* self, void*) {
    at<U>(self, 0x34) = 0x43f00000u;                                                       // 006793DC (480.0)
    void* area = at<void*>(kotor_fastcall_005ed530(client_app(), nullptr), 0x48);         // 006793E3..F1
    const U row = at<U>(area, 0xb8);                                                       // 006793FA
    void* table = at<void*>(at<void*>(kotor_cell_rules_007a3a28, 0xb8), 0x74);            // 006793F4..0679406
    unsigned char name[8];                                                                 // CExoString [esp+0Ch]
    frame_LogStringCtor_005e5a90(name, nullptr, k_max_turn_rate_00752cf8);  // 00679409..12
    c2da_current_get_float_00413430(table, nullptr, row, name, &at<float>(self, 0x3c));   // 00679417..2B
    frame_LogStringDtor_005e5c20(name, nullptr);                                           // 00679430..3C
    frame_LogStringCtor_005e5a90(name, nullptr, k_min_turn_rate_00752cec);  // 00679441..4A
    c2da_current_get_float_00413430(table, nullptr, row, name, &at<float>(self, 0x38));   // 0067944F..63
    frame_LogStringDtor_005e5c20(name, nullptr);                                           // 00679468..74
    vec(self, 0x1c) = Vector{0, 0, 0};                                                     // 00679479..A5
    at<float>(self, 0xc0) = static_cast<float>(kotor_fastcall_0061d5e0(kotor_fastcall_005ed700(client_app(), nullptr), nullptr));
    at<float>(self, 0xc4) = static_cast<float>(kotor_fastcall_0061d610(kotor_fastcall_005ed700(client_app(), nullptr), nullptr));
    at<float>(self, 0xc8) = static_cast<float>(kotor_fastcall_0061d640(kotor_fastcall_005ed700(client_app(), nullptr), nullptr));
}
// 00679510 CSWPlayerControlCamRelative::GetMaxSpeed (ST0)
double __fastcall w5_player_control_get_max_speed_00679510(void* self, void*) {
    if (w5_cell_max_speed_override_00833bac != 0) return w5_cell_max_speed_override_007a25f8;  // 00679510..1A, 006795B2
    if (at<U>(kotor_fastcall_005ed700(client_app(), nullptr), 0x88) != 0)                        // 00679520..35
        return w5_cell_max_speed_override_007a25f8;
    if (!client_creature(at<U>(self, 4))) return k_default_max_speed_007455c0;                   // 00679537..4B, 006795AA
    (void)kotor_fastcall_005ed8b0(client_app(), nullptr);                                        // 0067954D..55 (result unused)
    void* creature = client_creature(at<U>(self, 4));                                            // 0067955A..67
    if (at<U8>(creature, 0x2d4) & 1)                                                             // 0067956C
        return at<float>(at<void*>(client_creature(at<U>(self, 4)), 0x21c), 0x60);              // 00679579..8C
    return at<float>(at<void*>(client_creature(at<U>(self, 4)), 0x21c), 0x5c);                  // 00679591..A5
}

// 006795C0 ResetDriveAcceleration: zero +68, +5C, +50, +44, +74, +1C (in that order)
void __fastcall w5_player_control_reset_drive_acceleration_006795c0(void* self, void*) {
    const Vector z{0, 0, 0};
    vec(self, 0x68) = z; vec(self, 0x5c) = z; vec(self, 0x50) = z; vec(self, 0x44) = z; vec(self, 0x74) = z;
    vec(self, 0x1c) = z;
}
void __fastcall w5_player_control_up_down_00679700(void* self, void*, float v) { at<float>(self, 0x10) = v; }
double __fastcall w5_player_control_get_up_down_00679710(void* self, void*) { return at<float>(self, 0x10); }
void __fastcall w5_player_control_left_right_00679720(void* self, void*, float v) { at<float>(self, 0x14) = v; }
double __fastcall w5_player_control_get_left_right_00679730(void* self, void*) { return at<float>(self, 0x14); }
void __fastcall w5_player_control_set_player_walking_00679740(void* self, void*, U walking) { at<U>(self, 0x18) = walking; }
// 00679750 GetCurSpeed: |+5C| when it exceeds float(|+60|), else float(|+60|) (ties and NaN take |+60|)
double __fastcall w5_player_control_get_cur_speed_00679750(void* self, void*) {
    const double a = std::fabs(static_cast<double>(at<float>(self, 0x5c)));               // 00679751..54 (ST)
    const float b = static_cast<float>(std::fabs(static_cast<double>(at<float>(self, 0x60)))); // 00679756..5B fstp [esp]
    return a > b ? a : static_cast<double>(b);                                             // 0067975F..6C (unordered -> b)
}

// 00679780 CSWPlayerControlCamRelative::CSWPlayerControlCamRelative(id, camera)
void* __fastcall kotor_fastcall_00679780(void* self, void*, U object_id, void* camera) {
    at<const void*>(self, 0) = w5_vtable_00752d04;                                        // 0067978E
    at<U>(self, 4) = k_invalid; at<U>(self, 8) = 0; at<U>(self, 0xc) = 0;                  // 00679794..9E
    kotor_fastcall_00679340(self, nullptr, object_id);                                     // 006797A1
    at<void*>(self, 8) = camera;                                                           // 006797AA
    at<const void*>(self, 0) = w5_vtable_00752d34;                                        // 006797AD
    for (U off = 0x1c; off <= 0x30; off += 4) at<U>(self, off) = 0;                        // 006797B3..C2
    state(self, 0x44) = PhysicsState{{0, 0, 0}, {0, 0, 0}};                                // 006797C5..EC
    state(self, 0x5c) = PhysicsState{{0, 0, 0}, {0, 0, 0}};                                // 006797EF..0067812
    vec(self, 0x74) = Vector{0, 0, 0};                                                     // 0067984B..51
    kotor_fastcall_006d10b0(static_cast<unsigned char*>(self) + 0x80, nullptr);            // 0067982D, 00679854
    w5_player_control_init_006793c0(self, nullptr);                                        // 00679859
    return self;
}

// 00679870 GetAcceleration(out, velocity, target): out = a*target - (a/max)*velocity
Vector* __fastcall w5_player_control_get_acceleration_00679870(void* self, void*, Vector* out, const Vector* v,
                                                               const Vector* t) {
    float a;                                                                               // [esp]
    if (client_creature(at<U>(self, 4)))                                                   // 00679870..89
        a = static_cast<float>(w5_client_creature_get_drive_acceleration_00610590(client_creature(at<U>(self, 4)), nullptr));
    else a = k_fallback_accel_006798a9;                                                    // 006798A9
    const double max = w5_player_control_get_max_speed_00679510(self, nullptr);           // 006798B3
    if (max <= k_walk_speed_limit_00752d68)                                                // 006798B8..C4 (jp skips when >)
        a = static_cast<float>(static_cast<double>(a) * max * k_walk_accel_scale_00752d64); // 006798C6..D2
    const double k = a / max;                                                              // 006798D6 fdivr
    const double kvx = k * v->x;                                                           // kept on ST
    const float kvy = static_cast<float>(k * v->y), kvz = static_cast<float>(k * v->z);  // 006798E7 / 006798F4
    const double atx = static_cast<double>(a) * t->x;                                      // kept on ST
    const float aty = static_cast<float>(static_cast<double>(a) * t->y);                  // 00679905
    const float atz = static_cast<float>(static_cast<double>(a) * t->z);                  // 00679914
    out->x = static_cast<float>(atx - kvx);                                                // 00679918..1A
    out->y = static_cast<float>(static_cast<double>(aty) - kvy);                          // 0067991E..26
    out->z = static_cast<float>(static_cast<double>(atz) - kvz);                          // 00679929..31
    return out;
}

// ---------------- CSWMGPhysicsState ----------------
PhysicsState* __fastcall w5_physics_state_construct_00670750(PhysicsState* self, void*, const Vector* a, const Vector* b) {
    *self = PhysicsState{{0, 0, 0}, {0, 0, 0}};
    self->a = *a; self->b = *b;
    return self;
}
PhysicsState* __cdecl w5_physics_state_from_006709c0(PhysicsState* out, const Vector* b, const Vector* a) {
    *out = PhysicsState{{0, 0, 0}, {0, 0, 0}};
    out->a = *a; out->b = *b;                                                              // +0 = arg3, +C = arg2
    return out;
}
// 006708C0 operator/(state, d): d == 0 -> zero state; else both triples through Vector operator/ 00437A90.
PhysicsState* __cdecl w5_physics_state_divide_006708c0(PhysicsState* out, const PhysicsState* s, float d) {
    if (d == 0.0f) { *out = PhysicsState{{0, 0, 0}, {0, 0, 0}}; return out; }              // 006708C0..D2 (jnp: equal)
    Vector b, a;
    vector_divide_00437a90(&b, &s->b, d);                                                  // 006708DB..E9 (second triple first)
    vector_divide_00437a90(&a, &s->a, d);                                                  // 006708EE..F7
    *out = PhysicsState{{0, 0, 0}, {0, 0, 0}};
    out->a = a; out->b = b;
    return out;
}

// ---------------- CSWCCreature leaves ----------------
// 0060B380 SetMovingOrientation: imported from W3 t2 (@recovered_client_creature_set_moving_orientation_0060b380@12, t2b ruling)
// 00610590 GetDriveAcceleration: running flag (+2D4 bit 0) -> 15.0, else appearance(+21C)+58
double __fastcall w5_client_creature_get_drive_acceleration_00610590(void* creature, void*) {
    if (at<U8>(creature, 0x2d4) & 1) return k_drive_accel_running_00747f6c;
    return at<float>(at<void*>(creature, 0x21c), 0x58);
}
// 0060F0D0 SetSWAnimationSpeed(speed, b): +3B4 = b, +3B0 = speed
void __fastcall w5_client_creature_set_sw_animation_speed_0060f0d0(void* creature, void*, float speed, U b) {
    at<U>(creature, 0x3b4) = b; at<float>(creature, 0x3b0) = speed;
}
// 006383B0 (CSWCameraOnAStick): +80 = 7F000000
void __fastcall w5_camera_stick_clear_target_006383b0(void* camera, void*) { at<U>(camera, 0x80) = k_invalid; }
// 0060B920 ResetCameraToFollow
void __fastcall w5_client_creature_reset_camera_to_follow_0060b920(void* creature, void*) {
    void* party = kotor_fastcall_005ed8b0(client_app(), nullptr);                         // 0060B920..32
    if (recovered_party_index_006349c0(party, nullptr, at<U>(creature, 4)) != 0) return;  // 0060B934..3B
    if (at<U8>(kotor_fastcall_005ed700(client_app(), nullptr), 0x6d) == 5) {
        recovered_camera_reset_cell_007a2288 = 1; // 0060B956 existing canonical W1 storage
        kotor_fastcall_00641dc0(kotor_fastcall_005ed530(client_app(),nullptr),nullptr);
        kotor_fastcall_005eda60(client_app(),nullptr,0,1); // 0060B97C; fresh manager/client reload
    }
    void* camera = at<void*>(kotor_fastcall_005ed530(client_app(), nullptr), 0x40);       // 0060B981..8F
    void* stick = vcall<void*>(camera, 0x80, static_cast<U>(0x106a));                     // 0060B992..99
    if (stick) w5_camera_stick_clear_target_006383b0(stick, nullptr);                      // 0060B99F..A5
}

// ---------------- 00679940 CSWPlayerControlCamRelative::Control(dt) ----------------
I __fastcall w5_player_control_control_00679940(void* self, void*, float arg) {
    const float dt = (k_one_0073d764 < arg) ? k_one_0073d764 : arg;                         // 00679940..75 ([S-1A4])
    const U id = at<U>(self, 4);
    if (id == k_invalid) return 0;                                                         // 00679975..7E
    if (!at<void*>(self, 8)) return 0;                                                     // 00679984..89
    if (!server_app()) return 0;                                                           // 0067998F..99
    void* cc = client_creature(id);                                                        // 0067999F..A8 (EBX, [S-174])
    void* sc = kotor_m15a_creature_get_server_creature_0060fb20(cc, nullptr);              // 006799AA..B0 (ESI, [S-170])
    if (!sc || !cc) return 0;                                                              // 006799B7..C5
    if (vcall<U>(sc, 0x94)) return 0;                                                      // 006799CB..D7
    if (kotor_m15e_slot010_zero_0063e7f0(sc, nullptr)) return 0;                           // 006799DD..E6
    if (recovered_creature_incapacitated_004ef890(sc, nullptr)) return 0;                  // 006799EC..F5
    if (at<U8>(kotor_fastcall_005ed700(client_app(), nullptr), 0x6d) == 5) {
        PhysicsState integrated;
        const Vector turn_input = {0.0f,at<float>(cc,0x448),0.0f};
        void* const integrated_result = kotor_fastcall_006d1190(static_cast<U8*>(self)+0x80,nullptr,&integrated,turn_input,dt,1,0,0,0);
        const U angle_bits = at<U>(integrated_result,0x10); // returned state velocity.y
        at<U>(cc,0x448)=0;
        Quaternion rotation;
        rotation=*camera_mode5_orientation(&rotation,&vec(cc,0x30),angle_bits);
        Vector direction;
        void* const client_direction=quaternion_zdir_0043e250(&rotation,nullptr,&direction);
        vcall<void>(cc,0x88,client_direction); // 00679B18 client before server
        void* const server_direction=quaternion_zdir_0043e250(&rotation,nullptr,&direction);
        w1x_object_set_orientation_004cd170(sc,nullptr,server_direction);
    } // original falls through ordinary movement00679B37

    // 00679B37..00679B8B: input = (-LeftRight, UpDown, 0); diagonal-only normalize; walk halves it
    Vector in;                                                                             // [S-198]
    in.z = 0.0f;                                                                           // 00679B3A
    in.x = -at<float>(self, 0x14);                                                         // 00679B37..44
    in.y = at<float>(self, 0x10);                                                          // 00679B48..4B
    if (in.x != 0.0f && in.y != 0.0f) vector_normalize_004ab130(&in, nullptr);             // 00679B4F..71
    if (at<U>(self, 0x18) != 0) vector_scale_assign_004a97a0(&in, nullptr, 0.5f);          // 00679B76..86

    // 00679B8B..00679C07: camera yaw quaternion from the camera orientation's z axis
    Quaternion q16c;                                                                       // [S-16C]
    Vector cz;                                                                             // [S-154]
    void* q = vcall<void*>(at<void*>(self, 8), 0x24, &q16c);                               // 00679B8B..9D
    quaternion_zdir_0043e250(q, nullptr, &cz);                                             // 00679BA0..A2
    cz.z = 0.0f;                                                                           // 00679BB4
    if (cz.x != 0.0f && cz.y != 0.0f) vector_normalize_004ab130(&cz, nullptr);             // 00679BA7..E1
    const float cam_yaw = static_cast<float>(kotor_m15a_yaw_004aa0f0(&cz));                // 00679BE6..F7
    Quaternion camq;                                                                       // [S-40]
    yaw_pitch_roll_004acac0(&camq, cam_yaw, 0.0f, 0.0f);                                   // 00679BFA..02
    const bool idle = vector_is_zero_0046a620(&in, nullptr) != 0;                          // 00679C0A..0E ([S-19D])
    at<float>(cc, 0x3b8) = 0.0f;                                                           // 00679C13

    if (at<U>(self, 0xc) != 0 && !idle) {                                                  // 00679C1D..2E
        Vector dir;                                                                        // [S-1BC]
        quaternion_rotate_004a9bb0(&dir, &camq, &in);                                      // 00679C34..46
        // 00679C4B..55 pointto(&[S-16C], &dir): its result is dead (overwritten at 00679DCF / never read on the
        // other paths) and its statics 00831FB4..00831FCC are private to pointto (xref) -> elided.
        const float target_yaw = static_cast<float>(kotor_m15a_yaw_004aa0f0(&dir));        // 00679C5A..68 ([S-1C0])
        double cy = kotor_m15a_yaw_004aa0f0(&vec(cc, 0x30));                               // 00679C68..6C (ST)
        if (cy < k_zero_0073d700) cy += k_360_00741868;                                    // 00679C71..81
        double t = target_yaw;
        if (target_yaw < k_zero_0073d700) t = target_yaw + static_cast<double>(k_360_00741868); // 00679C87..9C
        float diff;                                                                        // [S-1C4]
        if (!(t <= cy) && !(t != t || cy != cy)) {                                         // 00679CA2..A9 (t > cy)
            diff = static_cast<float>(t - cy);                                             // 00679CAB..AD
            at<float>(self, 0x40) = (diff <= k_180_0073e408) ? 1.0f : -1.0f;              // 00679CB3..CB / 00679CF1
        } else {
            diff = static_cast<float>(cy - t);                                             // 00679CCD..D1
            at<float>(self, 0x40) = (diff > k_180_0073e408 || diff != diff) ? 1.0f : -1.0f; // 00679CD7..EF
        }
        if (diff > k_180_0073e408) diff = static_cast<float>(static_cast<double>(k_360_00741868) - diff); // 00679CF8..13
        at<float>(cc, 0x3b8) = static_cast<float>(static_cast<double>(diff) * at<float>(self, 0x40)); // 00679D17..1E
        if (std::fabs(static_cast<double>(diff)) > k_turn_epsilon_00752d70 &&                // 00679D24..35
            std::fabs(static_cast<double>(at<float>(cc, 0x3b0))) > k_turn_epsilon_00752d70) {  // 00679D3B..4E
            const float speed = static_cast<float>(kotor_cdecl_004ab100(&vec(self, 0x5c)));   // 00679D54..5D ([S-19C])
            const double r = 1.0 - speed / w5_player_control_get_max_speed_00679510(self, nullptr); // 00679D64..6F
            float rate = static_cast<float>(((static_cast<double>(at<float>(self, 0x3c)) - at<float>(self, 0x38)) * r +
                                             at<float>(self, 0x38)) * dt * at<float>(self, 0x40)); // 00679D75..87
            if (rate > diff) rate = diff;                                                  // 00679D8D..A4
            else if (rate < -diff) rate = -diff;                                           // 00679DA6..BF
            Quaternion turn;                                                               // [S-16C]
            yaw_pitch_roll_004acac0(&turn, rate, 0.0f, 0.0f);                              // 00679DC1..CF
            Vector f, nf;                                                                  // [S-130], [S-1B0]
            quaternion_rotate_004a9bb0(&f, &turn, &vec(cc, 0x30));                         // 00679DD4..E2
            normalize_004ab180(&nf, &f);                                                   // 00679DE7..ED
            dir = nf;                                                                      // 00679DF2..02
            if (at<U8>(sc, 0x9f0) & 2) {                                                   // 00679E06..11
                vcall<void>(cc, 0x88, &dir);                                               // 00679E13..1C
                recovered_client_creature_set_moving_orientation_0060b380(cc, nullptr, &dir);     // 00679E22..29
            }
            vec(self, 0x1c) = dir;                                                         // 00679E2E..42
        }
    }

    float speed = 0.0f;                                                                    // 00679E4B ([S-1C0])
    PhysicsState& next = state(self, 0x44);
    PhysicsState& cur = state(self, 0x5c);
    if (idle && (cur.a.x != 0.0f || cur.a.y != 0.0f) &&                                    // 00679E45..76
        std::fabs(static_cast<double>(cur.a.x)) < k_stop_speed_00749d88 &&                 // 00679E78..87
        std::fabs(static_cast<double>(cur.a.y)) < k_stop_speed_00749d88) {                 // 00679E89..99
        cur.a = Vector{0, 0, 0};                                                           // 00679E9B..C6
        next = cur;                                                                        // 00679EC9, 0067A677..7C (+5C -> +44)
    } else {
        if (in.x == 0.0f && in.y != 0.0f) cur.a.x = 0.0f;                                  // 00679ED1..F3
        if (in.y == 0.0f && in.x != 0.0f) cur.a.y = 0.0f;                                  // 00679EFA..1C
        // 00679F23..0067A07C: five zero CSWMGPhysicsState temporaries
        const Vector zero{0, 0, 0};
        PhysicsState st, k1, k2, k3, k4;                                                   // [S-148] [S-E8] [S-B8] [S-100] [S-D0]
        w5_physics_state_construct_00670750(&st, nullptr, &zero, &zero);
        w5_physics_state_construct_00670750(&k1, nullptr, &zero, &zero);
        w5_physics_state_construct_00670750(&k2, nullptr, &zero, &zero);
        w5_physics_state_construct_00670750(&k3, nullptr, &zero, &zero);
        w5_physics_state_construct_00670750(&k4, nullptr, &zero, &zero);
        Vector sum, mid, acc;
        vector_sum_00423b90(&sum, &in, &vec(self, 0x74));                                  // 0067A081..97
        vector_component_mult_00423bf0(&mid, &sum, 0.5f);                                  // 0067A09F..A5 ([S-1B0])
        PhysicsState d, t1, t2;
        // k1 = dt * {accel(v0, target0), v0}
        w5_player_control_get_acceleration_00679870(self, nullptr, &acc, &cur.a, &vec(self, 0x74)); // 0067A0B1..C1
        w5_physics_state_from_006709c0(&d, &cur.a, &acc);                                  // 0067A0C6..D0
        kotor_cdecl_00670830(&t1, &d, dt); k1 = t1;                                        // 0067A0D8..06
        kotor_cdecl_00670830(&t1, &k1, 0.5f);                                              // 0067A0E5..08
        kotor_cdecl_006707a0(&t2, &cur, &t1); st = t2;                                     // 0067A10D..34
        // k2 at the half step toward the mid target
        w5_player_control_get_acceleration_00679870(self, nullptr, &acc, &st.a, &mid);     // 0067A134..4B
        w5_physics_state_from_006709c0(&d, &st.a, &acc);                                   // 0067A150..61
        kotor_cdecl_00670830(&t1, &d, dt); k2 = t1;                                        // 0067A169..91
        kotor_cdecl_00670830(&t1, &k2, 0.5f);                                              // 0067A185..9C
        kotor_cdecl_006707a0(&t2, &cur, &t1); st = t2;                                     // 0067A1A1..C6
        // k3
        w5_player_control_get_acceleration_00679870(self, nullptr, &acc, &st.a, &mid);     // 0067A1C8..DF
        w5_physics_state_from_006709c0(&d, &st.a, &acc);                                   // 0067A1E4..F5
        kotor_cdecl_00670830(&t1, &d, dt); k3 = t1;                                        // 0067A1FD..19
        kotor_cdecl_006707a0(&t2, &cur, &k3); st = t2;                                     // 0067A21B..47 (full step)
        // k4 toward the new input
        w5_player_control_get_acceleration_00679870(self, nullptr, &acc, &st.a, &in);      // 0067A249..5D
        w5_physics_state_from_006709c0(&d, &st.a, &acc);                                   // 0067A262..73
        kotor_cdecl_00670830(&t1, &d, dt); k4 = t1;                                        // 0067A27B..97
        PhysicsState q4, q3, q2, q1, s1, s2, s3, s4;
        w5_physics_state_divide_006708c0(&q4, &k4, 6.0f);                                  // 0067A299..AE
        w5_physics_state_divide_006708c0(&q3, &k3, 3.0f);                                  // 0067A2B7..CC
        w5_physics_state_divide_006708c0(&q2, &k2, 3.0f);                                  // 0067A2D5..EA
        w5_physics_state_divide_006708c0(&q1, &k1, 6.0f);                                  // 0067A2F3..08
        kotor_cdecl_006707a0(&s1, &cur, &q1);                                              // 0067A30D..17
        kotor_cdecl_006707a0(&s2, &s1, &q2);                                               // 0067A31F..25
        kotor_cdecl_006707a0(&s3, &s2, &q3);                                               // 0067A32D..36
        kotor_cdecl_006707a0(&s4, &s3, &q4);                                               // 0067A33E..47
        next = s4;                                                                         // 0067A356..62
        vec(self, 0x74) = in;                                                              // 0067A364..75
        Vector delta;                                                                      // [S-1BC]
        kotor_cdecl_00423bc0(&delta, &next.b, &cur.b);                                     // 0067A36E..7E
        const double threshold = w5_player_control_get_max_speed_00679510(self, nullptr) < k_max_speed_floor_00743fdc
            ? static_cast<double>(k_zero_0073d700) : static_cast<double>(flt(k_move_threshold_bits)); // 0067A386..AE
        const double d2 = (static_cast<double>(delta.z) * delta.z + static_cast<double>(delta.y) * delta.y) +
                          static_cast<double>(delta.x) * delta.x;                          // 0067A3AE..C8
        if (d2 > threshold) {                                                              // 0067A3CA..D3
            Vector f{at<float>(cc, 0x30), at<float>(cc, 0x34), 0.0f}, nf;                 // 0067A3DD..F7 ([S-1B0])
            normalize_004ab180(&nf, &f);                                                   // 0067A3FF
            vcall<void>(cc, 0x88, &nf);                                                    // 0067A407..0A
            const float len = static_cast<float>(kotor_cdecl_004ab100(&delta));            // 0067A410..1A
            Vector step, np, back;
            vector_component_mult_00423bf0(&step, &vec(cc, 0x30), len);                    // 0067A41D..26
            delta = step;                                                                  // 0067A42B..4C
            vector_sum_00423b90(&np, &vec(cc, 0x24), &delta);                              // 0067A440..50 ([S-130])
            kotor_cdecl_00423bc0(&back, &vec(cc, 0x24), &np);                              // 0067A455..66
            (void)kotor_cdecl_004ab100(&back);                                             // 0067A46B..73 (discarded)
            const double s = kotor_cdecl_004ab100(&next.a);                                // 0067A473..78
            speed = static_cast<float>(s);                                                 // 0067A47D fst
            if (!_finite(s)) speed = 0.0f;                                                 // 0067A484..91, 0067A4DF
            else if (!(speed > k_one_0073d764)) speed = 0.0f;                              // 0067A493..A2
            else {
                if (w5_client_creature_set_precalculated_position_00614b90(cc, nullptr, &np) == 0) // 0067A4A4..B5
                    vec(self, 0x1c) = Vector{0, 0, 0};                                     // 0067A4B7..D0
                w5_client_creature_reset_camera_to_follow_0060b920(cc, nullptr);           // 0067A4D3..D5
            }
        } else if (at<U8>(kotor_fastcall_005ed700(client_app(), nullptr), 0x6d) == 6) {   // 0067A4EC..FE
            void* const selected_manager = manager();                           //0067A504 reload once
            void* const selected_client = at<void*>(selected_manager,4);
            void* const selected_server = at<void*>(selected_manager,8);          // cached across GetLastTarget
            const U selected_client_id = kotor_fastcall_005edd80(selected_client,nullptr);
            const U selected_server_id = kotor_fastcall_004aea30(selected_server,nullptr,selected_client_id);
            void* const selected = kotor_fastcall_004ae770(server_app(),nullptr,selected_server_id); // fresh manager/server
            if(selected != nullptr) {
                const U reputation_bits = recovered_object_get_reputation_0057cb80(sc,nullptr,at<U>(selected,4),0);
                I reputation; std::memcpy(&reputation,&reputation_bits,sizeof reputation); // original signedEAX
                if(reputation <= 10 && kotor_fastcall_0060f0f0(cc,nullptr) == 0) {
                    Vector target_direction;
                    kotor_cdecl_00423bc0(&target_direction,&vec(selected,0x90),&vec(sc,0x90));
                    Vector current_orientation;
                    copy_vector(&current_orientation,&vec(sc,0x9c));              // cached after subtraction, before yaw calls
                    const U rotation_bits = camera_mode6_turn_bits(&target_direction,&current_orientation,dt);
                    Quaternion turn;
                    float rotation; std::memcpy(&rotation,&rotation_bits,sizeof rotation); // raw originalDWORD argument, no float-return bridge
                    void* const quaternion = yaw_pitch_roll_004acac0(&turn,rotation,0.0f,0.0f);
                    Vector direction;
                    quaternion_rotate_004a9bb0(&direction,quaternion,&current_orientation);
                    w1x_object_set_orientation_004cd170(sc,nullptr,&direction);    // server first; preserve same cached direction
                    vcall<void>(cc,0x88,&direction);                              // actual client receiver, no field fabrication
                }
            }
        }
        cur = next;                                                                        // 0067A66D..7C (+44 -> +5C)
    }
    w5_client_creature_set_sw_animation_speed_0060f0d0(cc, nullptr, speed, 0);             // 0067A67E..87
    void* anim = reinterpret_cast<void*>(static_cast<std::uintptr_t>(gob_get_property_0x68_0063c070(cc, nullptr))); // 0067A68C..9A
    vcall<void>(anim, 0xd0, static_cast<U>(0));                                            // 0067A69F..A5
    return 1;
}
}  // extern "C"
