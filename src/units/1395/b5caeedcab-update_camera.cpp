// Lane W4 G14: 005F5E10 CClientExoAppInternal::UpdateCamera and small camera leaves.
// Transcribed from dis/005F5E10.asm (original swkotor.exe); every statement cites its PCs.
#include "097d96e71e-camera.hpp"
#include <cmath>

// closeC: providers for the 005F625D event-capture block (real bodies in the candidate).
namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); }   // CExoString()
extern "C" {
void __cdecl frame_LogFormat_005e5680(void* string, const char* format, ...);                  // CExoString::Format
void __fastcall frame_LogWriteString_005e6a70(void* file, void*, void* string);                // CExoFile::Write(CExoString const&)
void* __fastcall kotor_fastcall_005ed460(void* app, void*);                                    // CClientExoApp::GetCaptureEventFile
}

namespace w4cam {
// W4-owned cells (original .data initial values; reference census in receipts/005F5E10.json).
std::uint32_t invert_mouse_007a22a4 = 1;
float edge_scroll_fraction_007a2444 = 0.001f;           // bits 3A83126F
std::uint32_t turn_sign_00832920 = 0;
float accel_interp_min_007a2430 = 2.5f;
float desired_turn_rate_007a2434 = 7.5f;
std::int32_t style_request_007a242c = -1;
std::uint32_t dialog_camera_debug_008327ec = 0;
std::uint32_t free_style_debug_00833938 = 0;
std::uint32_t orientation_debug_0083394c = 0;
float pending_yaw_007a2440 = -1.0f;
float turn_exponent_007a2448 = 2.0f;
float shake_period_007a246c = 30.0f;
float shake_degrees_007a2470 = 20.0f;
float follow_pitch_007a245c = 50.0f;
float follow_7a2458 = 1.0f;
float follow_7a2450 = 5.0f;
float follow_7a2454 = 20.0f;
std::uint32_t lock_camera_007a22b8 = 1;
std::uint32_t collision_enabled_007a2468 = 1;
float new_position_scale_007a243c = 0.5f;
}  // namespace w4cam

using namespace w4;
using namespace w4cam;

namespace {
const float k_one_0073d764 = 1.0f, k_minus_one_00740cbc = -1.0f, k_zero_0073d700 = 0.0f;
const float k_dist_scale_0074ea10 = 0.013333333656191826f;    // bits 3C5A740E
const double k_zoom_epsilon_007471d0 = 9.999999747378752e-05; // qword 007471D0
void* input_object() { return kotor_menu_window_cells::input_007a39e4; }  // [007A39E4] CExoInput*
double poll_axis() { return kotor_fastcall_005df4d0(input_object(), nullptr, 0x11C, 0); }
}  // namespace

// 005F5E10 CClientExoAppInternal::UpdateCamera(float delta) (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_005f5e10(void* client, void*, float delta) {
    if (lw(client, 0x18) == 0) return;                                                     // 005F5E2D..005F5E33
    void* pause_timer = kotor_fastcall_005ed500(client_app(), nullptr);                    // 005F5E39..005F5E41
    const I64 ticks = static_cast<menu_continuation::WorldTimer*>(pause_timer)->GetSnapshotTimeDifference_004adc80(); // 005F5E48
    const U8 saved_gate = aurora_model_gate_007b8fc0;                                      // 005F5E4D
    aurora_model_gate_007b8fc0 = 0;                                                        // 005F5E64
    void* camera = lp(lp(client, 0x18), 0x40);                                             // 005F5E6B..005F5E6E edi
    // 005F5E60..005F5E7E: fild qword (exact for |ticks| < 2^53) * 1e-6f, stored float.
    const float d = static_cast<float>(static_cast<double>(ticks) * menu_continuation::scale_007439bc);
    const float sens = static_cast<float>(kotor_fastcall_0061d5b0(kotor_fastcall_005ed700(client_app(), nullptr), nullptr)); // 005F5E7B..005F5E8E
    float dx = 0.0f, dy = 0.0f;                                                            // [esp+1Ch], [esp+18h]
    kotor_fastcall_005df610(input_object(), nullptr, &dx, &dy);                            // 005F5E92..005F5EB2
    {



        const I32 sign = invert_mouse_007a22a4 != 0 ? 1 : -1;                              // 005F5EB7..005F5EC8

        const double tilt = static_cast<double>(sign) * (static_cast<double>(dy) * sens);  // 005F5ED0..005F5EDC
        kotor_fastcall_0063fcc0(lp(client, 0x18), nullptr, static_cast<float>(tilt));     // 005F5EDE..005F5EE1
    }
    float axis;                                                                            // [esp+14h]
    {
        bool to_low_check = jcc_jnp(poll_axis(), k_minus_one_00740cbc, 0x05);              // 005F5EE6..005F5F03 (< -1)
        if (!to_low_check) {
            const double p2 = poll_axis();                                                 // 005F5F05..005F5F12
            if (!jcc_jp(k_one_0073d764, p2, 0x05)) {                                       // 005F5F17..005F5F26 (1 < p)
                axis = 1.0f;                                                               // 005F5F28
                goto axis_done;
            }
        }
        if (!jcc_jp(poll_axis(), k_minus_one_00740cbc, 0x05)) {                            // 005F5F32..005F5F4F (p < -1)
            axis = -1.0f;                                                                  // 005F5F51
        } else {
            axis = static_cast<float>(poll_axis());                                        // 005F5F5B..005F5F6D
        }
    }
axis_done:
    I32 left, right;
    {
        void* gui = kotor_fastcall_005eda40(client_app(), nullptr);                        // 005F5F71..005F5F7A
        const I32 width = static_cast<std::int16_t>(lh(gui, 0x6C));                        // 005F5F7F movsx
        I32 margin = static_cast<I32>(static_cast<double>(width) * edge_scroll_fraction_007a2444); // 005F5F87..005F5F91 __ftol2
        if (margin < 2) margin = 2;                                                        // 005F5F96..005F5FA0
        const I32 mouse_x = li(kotor_input_cells::gui_manager_007a39f4, 0);               // 005F5FA2..005F5FA8
        left = mouse_x <= margin ? 1 : 0;                                                  // 005F5FB2..005F5FB4
        right = mouse_x >= width - margin ? 1 : 0;                                         // 005F5FB7..005F5FC3
    }
    I32 free_look = kotor_input_cells_required::word_008338f0;                             // 005F5FBD
    if (lb(kotor_fastcall_005ed700(client_app(), nullptr), 8) & 2)                         // 005F5FCA..005F5FD6
        free_look = free_look == 0 ? 1 : 0;                                                // 005F5FD8..005F5FDF
    const double sign_832920 = turn_sign_00832920 != 0 ? 1.0 : -1.0;
    bool turn_by_mouse = false;
    if (jcc_jp(axis, k_zero_0073d700, 0x44)) {                                             // 005F5FE1..005F5FF0 axis != 0
        const double a = sign_832920 * -static_cast<double>(axis);                         // 005F5FF7..005F601A
        kotor_fastcall_00640090(lp(client, 0x18), nullptr, f_bits(static_cast<float>(a)), f_bits(delta)); // 005F601C..005F601F
    } else if (left != 0 || right != 0) {                                                  // 005F6029..005F6033
        if (free_look != 0) {
            turn_by_mouse = true;                                                          // 005F6035..005F6037
        } else if (lb(kotor_fastcall_005ed700(client_app(), nullptr), 0x6D) == 5) {       // 005F6039..005F604A
            if (lb(kotor_fastcall_005ed700(client_app(), nullptr), 0x6D) == 5) turn_by_mouse = true; // 005F6089..005F609B
            else kotor_fastcall_0063fe10(lp(client, 0x18), nullptr, 0, f_bits(delta));    // 005F609D..005F60D5
        } else {
            const I32 s1 = turn_sign_00832920 != 0 ? 1 : -1;                               // 005F604C..005F6068
            const I32 s2 = left != 0 ? 1 : -1;                                             // 005F605C..005F606C
            kotor_fastcall_00640090(lp(client, 0x18), nullptr,
                                    f_bits(static_cast<float>(s1 * s2)), f_bits(delta)); // 005F6070..005F607E
        }
    } else {                                                                               // 005F6085
        if (free_look != 0) turn_by_mouse = true;
        else if (lb(kotor_fastcall_005ed700(client_app(), nullptr), 0x6D) == 5) turn_by_mouse = true; // 005F6089..005F609B
        else kotor_fastcall_0063fe10(lp(client, 0x18), nullptr, 0, f_bits(delta));        // 005F609D..005F60D5 TurnCamera(0, delta)
    }
    if (turn_by_mouse) {                                                                   // 005F60A6..005F60D5
        const double a = sign_832920 * (static_cast<double>(dx) * sens);
        kotor_fastcall_0063fe10(lp(client, 0x18), nullptr, f_bits(static_cast<float>(a)), f_bits(delta));
    }
    if (jcc_jp(lf(client, 0xAC), k_zero_0073d700, 0x44))                                   // 005F60DC..005F60ED (!= 0)
        sw(lp(client, 0x18), 0x104, 1);                                                    // 005F60EF..005F60F2
    if (camera != nullptr) {                                                               // 005F60FC..005F60FE
        const float margin = static_cast<float>(vcall<double>(camera, 0x6C) * k_dist_scale_0074ea10); // 005F6100..005F6107 GetDist
        aurora_hit_box_margin_007a8a4c[0] = margin;                                        // 005F610D
        aurora_hit_box_margin_007a8a4c[1] = margin;                                        // 005F6113
        aurora_hit_box_margin_007a8a4c[2] = margin;                                        // 005F6119
    }   // 005F611F: no else arm; with a null module camera the GetPosition vcall below reads [0] and faults as the original
    W4Vector position;                                                                     // [esp+3Ch]
    (void)vcall<W4Vector*, W4Vector*>(camera, 0x20, &position);                            // 005F611F..005F6128 GetPosition (result unused)
    W4Vector scroll = {0.0f, 0.0f, 0.0f};                                                  // 005F6131..005F6141
    if (li(client, 0xD0) != 0) {                                                           // 005F612B..005F6149
        W4Vector tmp;
        const W4Vector* r = vcall<W4Vector*, W4Vector*, I32, I32>(camera, 0x2C, &tmp, 0x28, 0); // 005F614B..005F6157 MouseDir
        scroll.x = static_cast<float>(static_cast<double>(li(client, 0xD0)) * r->x);      // 005F615A..005F6162
    }
    if (li(client, 0xD4) != 0) {                                                           // 005F6166..005F616C
        W4Vector tmp;
        const W4Vector* r = vcall<W4Vector*, W4Vector*, I32, I32>(camera, 0x2C, &tmp, 0, 0x28); // 005F616E..005F617A
        scroll.y = static_cast<float>(static_cast<double>(li(client, 0xD4)) * r->y);      // 005F617D..005F6186
    }
    kotor_fastcall_00640240(lp(client, 0x18), nullptr, scroll);                            // 005F618A..005F61A6 ScrollCamera
    aurora_model_gate_007b8fc0 = saved_gate;                                               // 005F61AB..005F61AF
    if (lw(client, 0x3BC) != lw(client, 0x20)) {                                           // 005F61B4..005F61BD
        const U32 creature = frame_R4_005ed5b0(client_app(), nullptr, lw(client, 0x20));  // 005F61BF..005F61C9
        sw(client, 0x3C0, creature);                                                       // 005F61D1
        sw(client, 0x3BC, lw(client, 0x20));                                               // 005F61CE..005F61D7
    }
    kotor_fastcall_00641760(lp(client, 0x18), nullptr, d);                                 // 005F61DD..005F61E5
    if (lw(client, 0x320) == 0) kotor_fastcall_006419a0(lp(client, 0x18), nullptr, d);     // 005F61EA..005F61F6
    const float zoom = static_cast<float>(
        kotor_fastcall_00644170(lp(client, 0x18), nullptr, d, lf(client, 0xAC)));          // 005F61FB..005F620B fstp float
    sf(client, 0xAC, zoom);                                                                // 005F6213..005F6219
    if (jcc_je(std::fabs(static_cast<double>(zoom)), k_zoom_epsilon_007471d0, 0x41))       // 005F620F..005F622A (|z| > eps)
        kotor_fastcall_006401d0(lp(client, 0x18), nullptr, zoom);                          // 005F622C..005F6232
    if (kotor_fastcall_005ed470(client_app(), nullptr) == 1 && lw(lp(client, 0x18), 0x80) == 1) { // 005F6237..005F6257
        // 005F625D..005F630A: event-capture logging of the module camera (capture mode [internal+8Ch] == 1).
        const float cap_a = static_cast<float>(vcall<double>(camera, 0x6C));               // 005F625D..005F6264 fstp [esp+24h]
        const float cap_b = static_cast<float>(vcall<double>(camera, 0x68));               // 005F6268..005F626F fstp [esp+28h]
        const float cap_c = static_cast<float>(vcall<double>(camera, 0x64));               // 005F6273..005F627A fstp [esp+64h]
        alignas(4) U8 line[8];                                                             // CExoString [esp+30h]
        kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(line), nullptr); // 005F627E..005F6282
        try {
            const unsigned long long t =
                kotor_fastcall_004adc70(frame_GetWorldTimer_005ed4f0(client_app(), nullptr), nullptr); // 005F628B..005F62B6
            const U32 ms = static_cast<U32>(t / 1000u);                                    // 005F62BB..005F62C3 __aulldiv (low dword)
            frame_LogFormat_005e5680(line, "%d %c %f %f %f\n", ms, 0x56,
                                     static_cast<double>(cap_a), static_cast<double>(cap_b),
                                     static_cast<double>(cap_c));                          // 005F6287..005F62D3 fmt 0074EA00
            frame_LogWriteString_005e6a70(kotor_fastcall_005ed460(client_app(), nullptr), nullptr, line); // 005F62D8..005F62F0
            sw(lp(client, 0x18), 0x80, 0);                                                 // 005F62F5..005F62FC
        } catch (...) {
            frame_LogStringDtor_005e5c20(line, nullptr);
            throw;
        }
        frame_LogStringDtor_005e5c20(line, nullptr);                                       // 005F6302..005F630A
    }
    sw(client, 0xAC, 0);                                                                   // 005F6312
    sw(client, 0xB0, 0);                                                                   // 005F6318
    sw(client, 0xB4, 0);                                                                   // 005F631E
    sw(lp(client, 0x18), 0x80, 0);                                                         // 005F630F/005F6324
}

// 005EDE70 CClientExoApp::GetCombatMode() (thiscall, RET 0).
extern "C" U32 __fastcall kotor_fastcall_005ede70(void* app, void*) {
    return lw(lp(app, 4), 0x320);                                                          // 005EDE70..005EDE73
}

// 0061D5B0 CClientOptions::GetMouseSenSetting() (thiscall, RET 0; result on ST0).
extern "C" double __fastcall kotor_fastcall_0061d5b0(void* options, void*) {
    const double v = static_cast<double>(lb(options, 0x50));                               // 0061D5B1..0061D5B9 fild
    return (v * 0.01f) * 45.0f + 10.0f;                                                    // 0061D5BD..0061D5C9 (PC53 each step)
}

// 0061D5E0 / 0061D610 / 0061D640 CClientOptions::GetCameraKeyboard{DPS,Acceleration,Deceleration}().
extern "C" double __fastcall kotor_fastcall_0061d5e0(void* options, void*) {
    const float g = menu_tables_m41::cells::keyboard_camera_007a2320;                      // 0061D5E0
    if (jcc_jnp(g, k_minus_one_00740cbc, 0x44)) return lf(options, 0x94);                  // 0061D5E6..0061D5F1 (== -1)
    return g;                                                                              // 0061D5F3
}
extern "C" double __fastcall kotor_fastcall_0061d610(void* options, void*) {
    const float g = menu_tables_m41::cells::keyboard_camera_007a2324;                      // 0061D610
    if (jcc_jnp(g, k_minus_one_00740cbc, 0x44)) return lf(options, 0x98);                  // 0061D616..0061D621
    return g;                                                                              // 0061D623
}
extern "C" double __fastcall kotor_fastcall_0061d640(void* options, void*) {
    const float g = menu_tables_m41::cells::keyboard_camera_007a2328;                      // 0061D640
    if (jcc_jnp(g, k_minus_one_00740cbc, 0x44)) return lf(options, 0x9C);                  // 0061D646..0061D651
    return g;                                                                              // 0061D653
}

// 0060B2A0 lockcamera(const char*) (cdecl).
extern "C" U32 __cdecl kotor_cdecl_0060b2a0(const char* arg) {
    void* module = kotor_fastcall_005ed530(client_app(), nullptr);                         // 0060B2A0..0060B2A8
    if (module == nullptr) return lock_camera_007a22b8;                                    // 0060B2AD..0060B2AF
    void* camera = lp(module, 0x40);                                                       // 0060B2B1
    if (camera == nullptr) return lock_camera_007a22b8;                                    // 0060B2B4..0060B2B6
    void* behavior = vcall<void*, I32>(camera, 0x80, -1);                                  // 0060B2B8..0060B2BC
    if (behavior == nullptr) return lock_camera_007a22b8;                                  // 0060B2C2..0060B2C4
    void* stick = vcall<void*>(behavior, 0x1C);                                            // 0060B2C6..0060B2CA
    if (stick == nullptr || arg == nullptr) return lock_camera_007a22b8;                   // 0060B2CF..0060B2D9
    const I32 c = static_cast<signed char>(arg[0]) - 0x30;                                 // 0060B2DB..0060B2DE
    if (c == 0) {                                                                          // 0060B2E1 -> 0060B2FC
        lock_camera_007a22b8 = 0; sw(stick, 0x64, 0); sw(stick, 0x58, 0);                  // 0060B2FC..0060B306
        return 0;
    }
    if (c != 1) return lock_camera_007a22b8;                                               // 0060B2E3..0060B2E4
    lock_camera_007a22b8 = 1; sw(stick, 0x64, 0x3F800000u); sw(stick, 0x58, 0x3F800000u);  // 0060B2E6..0060B2F8
    return 1;                                                                              // eax = 1
}

// 00697580 CSWCCreatureAppearance::GetCameraOffset() (thiscall, RET 0; ST0).
extern "C" double __fastcall kotor_fastcall_00697580(void* appearance, void*) {
    float value = 0.0f;                                                                    // [esp] = 0
    void* table = lp(lp(kotor_cell_rules_007a3a28, 0xB8), 0x0C);                           // 00697583..0069759B appearance 2DA
    const int found = kotor_m15b_c2da_get_float_entry_00413350(table, nullptr, lh(appearance, 0x18),
                                                               kotor_rules_cell32_007a20c8, &value); // 0069759E..006975AB
    if (found != 0) return value;                                                          // 006975B0..006975B4
    return k_zero_0073d700;                                                                // 006975BA
}

// 0060F010 CSWCCreature::GetCameraOffset() (thiscall; mov ecx,[ecx+21Ch]; jmp 00697580).
extern "C" double __fastcall kotor_fastcall_0060f010(void* creature, void*) {
    return kotor_fastcall_00697580(lp(creature, 0x21C), nullptr);
}

// 005DF610 CExoInput::GetMouseDelta(float*, float*) (mov ecx,[ecx+4]; jmp 005E00F0).
extern "C" void* __fastcall kotor_fastcall_005df610(void* input, void*, float* dx, float* dy) {
    kotor_fastcall_005e00f0(lp(input, 4), nullptr, dx, dy);
    return nullptr;
}
// 005E00F0 CExoInputInternal::GetMouseDelta(float*, float*) (thiscall, RET 8).
extern "C" void __fastcall kotor_fastcall_005e00f0(void* internal, void*, float* dx, float* dy) {
    std::memcpy(dx, static_cast<U8*>(internal) + 0x3A0, 4);                                // 005E00F0..005E00FA
    std::memcpy(dy, static_cast<U8*>(internal) + 0x3A4, 4);                                // 005E00FC..005E0106
}
