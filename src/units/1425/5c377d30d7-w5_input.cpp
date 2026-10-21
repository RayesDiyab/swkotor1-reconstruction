// W5 t2: the input side of the keyboard drive.
// Covers CExoInput::PollInput (005DF4D0 -> CExoInputInternal 005E23C0 with GetEvent / IsEventInClass /
// ScaledValue) and the ProcessInput drive block 0062388A..00623F1F, which W4 calls in place of trap T-PI-5
// (see CONTROL-CONTRACT.md).
// CExoInputInternal layout used here:
//   +2C + class*30h   per-class event bit set (CExoArrayList<ulong> {data, size, alloc}; classes 0..5)
//   +128 / +12C       event-object array and count
//   +140              CExoRawInputInternal*
//   +164 + ctrl*4     axis index per control
// Event object layout:
//   +0  vtable (slot 0 = enabled)
//   +4  raw value                  +14 type
//   +18 device                     +1C control
//   +20 scaled flag                +24 second raw (type 3: new value; type 4: key A)
//   +28 scale                      +2C exponent
//   +30 / +34 type-4 key B control / raw
#include "d8371efa32-w5_t2.hpp"
#include <cmath>

using namespace w5;

namespace kotor_input_cells_required { extern volatile int word_008338f0; extern volatile int word_008338f4; } // [0774] / [0456]

extern "C" {
void* __fastcall frame_GetCharacter_006346c0(void* party, void*, int index);           // CSWParty::GetCharacter [0457..]
void __fastcall kotor_fastcall_00405150(void* list, void*, int size);                  // CExoArrayList<ulong>::SetSize
void __fastcall kotor_m15a_object_abort_fire_forget_animation_0063cad0(void* object, void*, U a);
// absent from the 1121 map, no lane owner yet: W5 import names with trap fallbacks (w5_t2_foreign_traps.cpp)
void __fastcall w5_t2_client_object_clear_all_actions_0063d470(void* object, void*);
void __fastcall w5_t2_client_creature_set_lock_orientation_00610950(void* creature, void*, U target, U flag);
void __fastcall w5_t2_client_creature_set_engaged_state_0060f140(void* creature, void*, U state);
void __fastcall recovered_client_creature_set_way_points_0060c790(void* creature, void*, U a, U b); // W3 name
}

namespace {
constexpr U k_invalid = 0x7f000000u;
const float k_minus_one_00740cbc = -1.0f;
const float k_one_0073d764 = 1.0f;
const float k_quarter_0073edf0 = 0.25f;
const float k_half_0073e9ac = 0.5f;
const float k_ten_007455c8 = 10.0f;        // 41200000
const float k_minus_30_0074fc4c = -30.0f;  // C1F00000
const float k_minus_60_0074fc48 = -60.0f;  // C2700000
const float k_minus_90_0074fc44 = -90.0f;  // C2B40000

// DEVIATION FROM ORIGINAL: see LIST_OF_CHANGES_FROM_ORIGINAL.md#high-fps-drive-breakout
// ---- #120 high-FPS drive break-out fix (changes behavior only above 66 FPS; identical at <= 66 FPS) ----
// Original 00623B3D..00623C24: while the drive is blocked (IsDriveAnimationBlocked 0060F0F0: engaged state
// creature+13Ch from AddFireForgetAnimation 00618462 (anim 277Dh), or server actions pending), holding WASD zeroes
// the input for 0.3 s, then lets it through for ONE frame (timer reset to 0.1 at 00623C10) and, 0.1 s later, breaks
// out only if GetCurSpeed 00679750 > 0.25 (00623BA7). One frame of input from rest gives |v| ~= driveaccl*dt/2
// (50*dt/2) in Control's RK4 (00679940), and Control's idle dead zone (00679E78..00679EC6) snaps |v| < 0.25 to 0,
// so above ~85 FPS (W) / ~70 FPS (diagonal) the speed never exceeds 0.25 and the player stays blocked forever
// (click-to-move still works). Fix: the pass-through burst lasts until it covers one 60 Hz frame of game time
// (continues while burst + 0.1*dt < 1/60). For dt >= (1/60)/1.1 the burst is exactly one frame, as in the original.
const float k_fpsroot_burst_window = 1.0f / 60.0f;
const float k_fpsroot_burst_tolerance = 0.1f;
float fpsroot_burst_time = 0.0f;           // game time of drive input passed in the current burst (0 = no burst)
}  // namespace

// Switch, read once (first in-game drive frame; the drive block 0062388A only runs once a player controller exists) the same way as the frame-rate cap (srchighfps\winmain\winmain.cpp
// kotor_winmain::deviation_frame_rate_cap):
//   [Reconstruction Options] High FPS Drive Fix=<n>   (absent -> 1 = fixed; 0 or negative -> original one-frame burst)
// With the switch off fpsroot_burst_time is never set, so every frame takes the original 00623C1C path.
extern "C" __declspec(dllimport) unsigned int __stdcall GetPrivateProfileIntA(const char*, const char*, int, const char*);

namespace {
const int k_fpsroot_drive_fix_default = 1;
int fpsroot_drive_fix_state = -1;          // -1 = not read yet, 0 = original, 1 = fixed
bool fpsroot_drive_fix_enabled() {
    if (fpsroot_drive_fix_state < 0) {
        const int v = static_cast<int>(GetPrivateProfileIntA("Reconstruction Options", "High FPS Drive Fix",
                                                             k_fpsroot_drive_fix_default, ".\\swkotor.ini"));
        fpsroot_drive_fix_state = v > 0 ? 1 : 0;
    }
    return fpsroot_drive_fix_state != 0;
}
}  // namespace

namespace kotor_menu_window_cells { extern void* volatile input_007a39e4; }               // [0053] CExoInput*

extern "C" {
// 005E3A60 / 005E3AA0 CExoRawInputInternal::GetMaxUseable / GetMinUseable(device, axis) (ST0, RET 8)
double __fastcall w5_raw_input_get_max_useable_005e3a60(void*, void*, U device, U axis) {
    if (device == 0 || device == 1) return 0.0;
    return (axis == 0 || axis == 4 || axis == 0x18 || axis == 0x1c) ? 32767.0 : 0.0;
}
double __fastcall w5_raw_input_get_min_useable_005e3aa0(void*, void*, U device, U axis) {
    if (device == 0 || device == 1) return 0.0;
    return (axis == 0 || axis == 4 || axis == 0x18 || axis == 0x1c) ? 8191.75 : 0.0;
}

// 005E0D80 IsEventInClass and 005E2360 GetEvent are imported (t2b review ruling): m48's GetEvent export (object 0698)
// and the owner-patch export of InputInternal::IsEventInClass (tiers/t2b/owner-patch/input_m48.patch).
// 005DFD80 CExoInputInternal::ScaledValue(event*, control, raw) (ST0, RET 0Ch)
double __fastcall w5_input_internal_scaled_value_005dfd80(void* in, void*, void* event, U control, I raw) {
    const float v = static_cast<float>(raw);                                               // 005DFD81..8D
    if (!vcall<U>(event, 0)) return v;                                                     // 005DFD95..9B
    if (!at<U>(event, 0x20)) return v;                                                     // 005DFDA1..A6
    if (control >= 6 && control <= 87) return static_cast<double>(v) * 0.0078125f;        // 005DFDAC..CF
    if (control == 3 || control == 4) {                                                   // 005DFDD2..E4 -> 005DFED4
        const double r = static_cast<double>(v) * 0.01f;
        if (!(k_one_0073d764 < r) && k_minus_one_00740cbc > r) return k_minus_one_00740cbc; // 005DFEDE..0D
        if (k_one_0073d764 < r) return k_one_0073d764;                                     // 005DFF10..2C
        return r;
    }
    if (raw == 0) return v;                                                                // 005DFE0E..14 / 005DFE4F..55
    void* rawin = at<void*>(in, 0x140);
    const U axis = at<U>(in, 0x164 + control * 4);
    const float max = static_cast<float>(w5_raw_input_get_max_useable_005e3a60(rawin, nullptr, at<U>(event, 0x18), axis)); // fstp [esp+28h]
    const double min = w5_raw_input_get_min_useable_005e3aa0(rawin, nullptr, at<U>(event, 0x18), axis);
    double ratio = ((std::fabs(static_cast<double>(v)) - min) - k_one_0073d764) / (static_cast<double>(max) - min); // 005DFE8D..A1
    if (k_one_0073d764 < ratio) ratio = k_one_0073d764;                                    // 005DFEA5..B6
    return raw >= 0 ? ratio * k_one_0073d764 : ratio * k_minus_one_00740cbc;              // 005DFEBC..D1
}
// 005E23C0 CExoInputInternal::PollInput(event, class) (ST0, RET 8)
double __fastcall w5_input_internal_poll_input_005e23c0(void* in, void*, U event, U input_class) {
    const I cls = static_cast<I>(input_class);
    if (cls < 0 || cls >= 6) return 0.0;                                                   // 005E23D0..DE
    if (!gui_m48_input_internal_is_event_in_class_005e0d80(in, nullptr, static_cast<I>(event), cls)) return 0.0; // 005E23EB..F4
    void* e = gui_m48_input_internal_get_event_005e2360(in, nullptr, event, cls);              // 005E23F6..FF
    float value = 0.0f;                                                                    // [esp+0Ch] (fst 0.0 at 005E23CC)
    switch (at<U>(e, 0x14)) {                                                              // 005E2401..09 (table 005E24C8)
    case 0: case 2: case 5:
        value = static_cast<float>(w5_input_internal_scaled_value_005dfd80(in, nullptr, e, at<U>(e, 0x1c), at<I>(e, 4)));
        break;
    case 1: return 0.0;                                                                    // 005E2410
    case 3: {
        const I delta = at<I>(e, 0x24) - at<I>(e, 4);                                      // 005E2459..5F
        value = static_cast<float>(w5_input_internal_scaled_value_005dfd80(in, nullptr, e, at<U>(e, 0x1c), delta));
        at<U>(e, 4) = at<U>(e, 0x24);                                                      // 005E246E..71
        break;
    }
    case 4: {
        const float a = static_cast<float>(w5_input_internal_scaled_value_005dfd80(in, nullptr, e, at<U>(e, 0x1c), at<I>(e, 0x24))); // fstp [esp+18h]
        const double b = w5_input_internal_scaled_value_005dfd80(in, nullptr, e, at<U>(e, 0x30), at<I>(e, 0x34));
        value = static_cast<float>(static_cast<double>(a) - b);                            // 005E2453 fsubr
        break;
    }
    default: break;                                                                        // 005E2407 ja -> 005E2478
    }
    if (!vcall<U>(e, 0)) return value;                                                     // 005E2478..84
    // 005E2486..C4: x87 _pow 006FC1F0 (genuine_x87_pow) modelled with the CRT pow on doubles
    const double r = std::pow(static_cast<double>(value), static_cast<double>(at<float>(e, 0x2c)));
    if (value < 0.0f) return (r > 0.0 ? -r : r) * at<float>(e, 0x28);                      // 005E2495..B5
    return r * at<float>(e, 0x28);                                                         // 005E24B8..C4
}
// 005DF4D0 CExoInput::PollInput: mov ecx,[ecx+4]; jmp 005E23C0
double __fastcall kotor_fastcall_005df4d0(void* input, void*, U event, U input_class) {
    return w5_input_internal_poll_input_005e23c0(at<void*>(input, 4), nullptr, event, input_class);
}

// ---------------- ProcessInput 0062388A..00623F1F (drive controller input) ----------------
void __fastcall w5_process_input_drive_0062388a(void* ci, void*, float dt) {
    (void)fpsroot_drive_fix_enabled();     // DEVIATION FROM ORIGINAL: see LIST_OF_CHANGES_FROM_ORIGINAL.md#high-fps-drive-breakout (reads the switch once)
    void* const input = kotor_menu_window_cells::input_007a39e4;
    auto poll = [&](U event, U cls) { return kotor_fastcall_005df4d0(input, nullptr, event, cls); };
    void* ctl = at<void*>(ci, 0x2a0);
    void* pc0 = frame_GetCharacter_006346c0(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr, 0); // 00623851..66 (EDI)
    const auto mode = [] { return at<U8>(kotor_fastcall_005ed700(client_app(), nullptr), 0x6d); };
    if (at<U>(ctl, 0xc) != 0 && at<U>(ci, 0x9c) == 0 && at<U>(ci, 0x288) == 0 && mode() != 7) {  // 0062388A..C3
        float ud = static_cast<float>(poll(7, 0));                                         // 006238C9..D8 ([S-11C])
        float lr = static_cast<float>(poll(8, 0));                                         // 006238DC..EA ([S-120])
        if (ud == 0.0f && lr == 0.0f) {                                                    // 006238EE..12
            if (!(poll(0x119, 0) < k_minus_one_00740cbc) && k_one_0073d764 < poll(0x119, 0)) lr = k_one_0073d764; // 00623918..62
            else if (poll(0x119, 0) < k_minus_one_00740cbc) lr = k_minus_one_00740cbc;      // 00623964..8B
            else lr = static_cast<float>(poll(0x119, 0));                                  // 0062398D..9F
            if (!(poll(0x118, 0) < k_minus_one_00740cbc) && k_one_0073d764 < poll(0x118, 0)) ud = k_one_0073d764; // 006239A3..ED
            else if (poll(0x118, 0) < k_minus_one_00740cbc) ud = k_minus_one_00740cbc;      // 006239EF..16
            else ud = static_cast<float>(poll(0x118, 0));                                  // 00623A18..2A
        }
        int mouse_move = kotor_input_cells_required::word_008338f0;                        // 00623A37
        if (at<U8>(kotor_fastcall_005ed700(client_app(), nullptr), 8) & 2) mouse_move = mouse_move == 0; // 00623A3D..4F
        if (mouse_move && kotor_input_cells_required::word_008338f4 && ud <= 0.0f) ud = k_minus_one_00740cbc; // 00623A51..6F
        vcall<void>(ctl, 0x10, static_cast<U>(poll(0x109, 0) != 0.0 ? 1 : 0));            // 00623A77..A9
        if (pc0) {                                                                         // 00623AAC..AE
            if (kotor_fastcall_0060f0f0(pc0, nullptr) == 1) {                              // 00623AB4..BE
                if ((at<U8>(pc0, 0x440) & 1) && !(at<U8>(at<void*>(ci, 4), 8) & 0x10)) { // 00623AC4..D4
                    ud = 0.0f; lr = 0.0f; at<float>(ci, 0x2a4) = -1.0f;                    // 00623AD6..E6
                } else if (ud == 0.0f && lr == 0.0f) {
                    at<float>(ci, 0x2a4) = -1.0f;                                          // 00623AF2..14
                } else if (at<float>(ci, 0x2a4) <= 0.0f) {
                    at<U>(ci, 0x2a4) = 0x3e99999au;                                        // 00623B20..33 (0.3)
                    fpsroot_burst_time = 0.0f;                                             // #120: new hold, no burst
                }
                if (at<float>(ci, 0x2a4) > 0.0f) {                                         // 00623B3D..4E
                    at<float>(ci, 0x2a4) = static_cast<float>(static_cast<double>(at<float>(ci, 0x2a4)) - dt); // 00623B54..63
                    void* s = kotor_m15a_creature_get_server_creature_0060fb20(pc0, nullptr); // 00623B69
                    const U commandable = s ? at<U>(s, 0xe8) : 1u;                         // 00623B6E..77
                    if (at<float>(ci, 0x2a4) > 0.0f || at<float>(ci, 0x2a4) != at<float>(ci, 0x2a4)) {
                        if (fpsroot_burst_time > 0.0f &&
                            fpsroot_burst_time + dt * k_fpsroot_burst_tolerance < k_fpsroot_burst_window) {
                            fpsroot_burst_time = fpsroot_burst_time + dt;                  // #120: burst continues, input kept
                        } else {
                            fpsroot_burst_time = 0.0f;
                            ud = 0.0f; lr = 0.0f;                                          // 00623B7D..8E -> 00623C1C
                        }
                    } else if (commandable) {                                              // 00623B94..96
                        if (vcall<double>(ctl, 0x1c) > k_quarter_0073edf0) {               // 00623B9C..B2
                            w5_t2_client_object_clear_all_actions_0063d470(pc0, nullptr);  // 00623BB4..B6
                            kotor_m15a_object_abort_fire_forget_animation_0063cad0(pc0, nullptr, 0); // 00623BBB..BF
                            w5_t2_client_creature_set_lock_orientation_00610950(pc0, nullptr, k_invalid, 0); // 00623BC4..CD
                            w5_t2_client_creature_set_engaged_state_0060f140(pc0, nullptr, 0); // 00623BD2..D6
                            void* camera = at<void*>(kotor_fastcall_005ed530(client_app(), nullptr), 0x40);
                            if (void* stick = vcall<void*>(camera, 0x80, static_cast<U>(0x106a))) // 00623BDB..FA
                                kotor_fastcall_0063a360(stick, nullptr);                   // 00623BFC..FE
                            recovered_client_creature_set_way_points_0060c790(pc0, nullptr, 0, 0); // 00623C03..09
                        } else {
                            at<U>(ci, 0x2a4) = 0x3dcccccdu;                                // 00623C10 (0.1)
                            if (fpsroot_drive_fix_enabled()) fpsroot_burst_time = dt;      // #120: this frame starts the burst (DEVIATION)
                        }
                    }
                }
            }
            const I index = static_cast<I>(recovered_party_index_006349c0(at<void*>(ci, 0x270), nullptr, at<U>(pc0, 4))); // 00623C2C..36
            if (index >= 0) {                                                              // 00623C3B..3D
                unsigned char* member = static_cast<unsigned char*>(at<void*>(ci, 0x270)) + index * 0x88 + 0x24; // 00623C3F..4B
                if (member && (at<U8>(pc0, 0x440) & 1)) {                                  // 00623C4F..5A
                    if (vcall<double>(ctl, 0x1c) < k_quarter_0073edf0) {                   // 00623C5C..72
                        if (at<U>(member, 0x84) == 0xa5ec) kotor_fastcall_006345e0(member, nullptr, 0xbc50); // 00623C74..8E
                    } else {
                        kotor_fastcall_006345e0(member, nullptr, 0xa5ec);                  // 00623C87..8E
                    }
                }
            }
        }
        bool keep = false;                                                                 // 00623C93..D05: clear the last target after 0.5 s of driving
        if (at<U>(ci, 0x320) == 0 && at<float>(ci, 0x390) <= 0.0f && at<U>(ci, 0x2b4) != k_invalid &&  // 00623C93..BF
            vcall<double>(ctl, 0x1c) >= k_quarter_0073edf0) {                              // 00623CC1..D7 (NaN -> clear)
            const double timer = static_cast<double>(dt) + at<float>(ci, 0x36c);              // 00623CD9..E0
            at<float>(ci, 0x36c) = static_cast<float>(timer);                                  // 00623CE6 fst (ST0 keeps the sum)
            if (!(timer >= k_half_0073e9ac)) keep = true;                                      // 00623CEC..F7 (compares the unrounded sum)
            else at<U>(ci, 0x2b4) = k_invalid;                                             // 00623CF9
        }
        if (!keep) at<U>(ci, 0x36c) = 0;                                                   // 00623CFF
        vcall<void>(ctl, 0x00, ud);                                                        // 00623D05..12 UpDown
        vcall<void>(ctl, 0x08, lr);                                                        // 00623D14..19, 00623F18..20 LeftRight
        return;
    }
    // 00623D1E..00623F1F: controller disabled / input class busy / camera mode 7
    if (mode() == 7) {                                                                     // 00623D27..31
        // 00623D37..00623E41: camera mode 7 drives the module camera behaviour (type 1071h) from the input axes.
        float a = static_cast<float>(poll(7, 0));                                          // 00623D37..46 ([S+14h])
        float b = static_cast<float>(poll(8, 0));                                          // 00623D4A..58 ([S+10h], EBX = 8)
        if (a == 0.0f && b == 0.0f) {                                                      // 00623D5C..7C (jp: != 0 or NaN keeps)
            b = static_cast<float>(poll(0x119, 0));                                        // 00623D7E..90
            a = static_cast<float>(poll(0x118, 0));                                        // 00623D94..A6
        }
        const float c = static_cast<float>(poll(0xc, 0));                                  // 00623DAA..B9 ([S+18h])
        const float d = static_cast<float>(poll(0xd, 0));                                  // 00623DBD..CC ([S+1Ch])
        void* const module = kotor_fastcall_005ed530(client_app(), nullptr);               // 00623DD0..DE
        void* const behavior = vcall<void*>(at<void*>(module, 0x40), 0x80, -1);            // 00623DDE..E5
        if (behavior && vcall<U>(behavior, 0x10) == 0x1071) {                              // 00623DED..FD
            at<float>(behavior, 0x1c) = static_cast<float>(static_cast<double>(a) * k_ten_007455c8);       // 00623DFF..09
            at<float>(behavior, 0x24) = static_cast<float>(static_cast<double>(b) * k_ten_007455c8);       // 00623E0C..16
            at<float>(behavior, 0x18) = static_cast<float>(static_cast<double>(c) * k_minus_30_0074fc4c); // 00623E19..23
            at<float>(behavior, 0x14) = static_cast<float>(static_cast<double>(d) * k_minus_60_0074fc48); // 00623E26..30
            at<float>(behavior, 0x28) = static_cast<float>(static_cast<double>(d) * static_cast<double>(a) *
                                                           k_minus_90_0074fc44);                            // 00623E33..41
        }
    }
    if (mode() == 5) {                                                                     // 00623E44..55
        float ud = static_cast<float>(poll(7, 0));                                         // 00623E5B..6A
        float lr = static_cast<float>(poll(8, 0));                                         // 00623E6E..7C
        bool tilt = true;
        if (ud == 0.0f) {
            if (lr == 0.0f) {                                                              // 00623E91..A0
                lr = static_cast<float>(poll(0x119, 4));                                   // 00623EA2..B4
                ud = static_cast<float>(poll(0x118, 4));                                   // 00623EB8..CA
            }
            if (ud == 0.0f && lr == 0.0f) tilt = false;                                    // 00623ECE..EE
        }
        if (tilt) kotor_fastcall_0063fdb0(kotor_fastcall_005ed530(client_app(), nullptr), nullptr, ud); // 00623EF0..05
    }
    vcall<void>(ctl, 0x00, 0.0f);                                                          // 00623F0A..14
    vcall<void>(ctl, 0x08, 0.0f);                                                          // 00623F16..20
}
}  // extern "C"
