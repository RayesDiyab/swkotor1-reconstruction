// lane-r4-lowers: R4 MainLoop (00602EB0) / GUI Update (0040CE70) / Draw (0040CC50)
// first-frame lower bodies. Evidence: dumpbin of swkotor.exe 9c10e045..1435
// (lane-routes evidence/full.asm.txt); per-statement instruction addresses below.
// Calling conventions: original __thiscall bodies use the R4 consumer's D1 form
// (extern "C" __fastcall self, unused EDX, stack words; callee pops) or the R4
// member declaration; stack-word counts equal the original RET immediates.
#include "9505ba33d8-r4_lowers.h"
#include "3e8cd0d42f-startup_views.h"   // M41 kotor_menu_10_19::TimerDependency::high_resolution_005e9260

using namespace r4_lowers;
using menu_continuation::AppManager;

namespace {
// .rdata constants read by these bodies (PE image values, read-only).
const U32 k_hundred_00741838 = 0x42C80000u;     // 100.0f
const U32 k_two32_0073d770 = 0x4F800000u;       // 4294967296.0f
const U32 k_milli_0073d708 = 0x3A83126Fu;       // 0.001f
const U32 k_quarter_0073edf0 = 0x3E800000u;     // 0.25f
const U32 k_four_0073f404 = 0x40800000u;        // 4.0f
const U32 k_fifth_0073e46c = 0x3E4CCCCDu;       // 0.2f
const U64 k_scale_0074ea18 = 0xBFF4000001400000ull; // double -1.2500000046566129
const U32 k_min_0073d700 = 0x00000000u;         // 0.0f
const U32 k_tenthousandth_00741870 = 0x38D1B717u; // 0.0001f
constexpr unsigned char k_object_found_0074666c = 0x00u;  // .rdata byte 0074666C
constexpr unsigned char k_creature_type_00746634 = 0x05u; // .rdata byte 00746634
void* client_of(AppManager* m) { return pw(m, 4); }
void* server_of(AppManager* m) { return pw(m, 8); }
float as_float(U32 bits) { float v; std::memcpy(&v, &bits, 4); return v; }
}

// ======================= CWorldTimer =======================
// 004ADBD0 CWorldTimer::SetSnapshotTime, RET (R4 entry_providers.h).
extern "C" void __fastcall frame_R4_004adbd0(void* t, void*) {
    const U32 low = w(t, 0x0c), high = w(t, 0x10);                 // 004ADBDB / 004ADBD7
    sw(t, 0x14, low); sw(t, 0x18, high);                           // 004ADBDE / 004ADBE1
    if (w(t, 0) == 0) {                                            // 004ADBD4 / 004ADBE4
        const U64 now = static_cast<U64>(menu_continuation::exo_base_007a39e0->timers->GetHighResolutionTimer_005e9260()); // 004ADBE6..004ADBEF
        const U64 previous = (static_cast<U64>(w(t, 0x20)) << 32) | w(t, 0x1c); // 004ADBF6 / 004ADBFB
        const U64 elapsed = now - previous;                        // 004ADBFE..004ADC04 (sub/sbb)
        const U64 scale = static_cast<U64>(static_cast<I64>(static_cast<I32>(w(t, 8)))); // 004ADC06 / 004ADC09 cdq
        const U64 advance = (elapsed * scale) / 100u;              // 004ADC0E __allmul (low 64), 004ADC19 __aulldiv
        const U64 current = ((static_cast<U64>(w(t, 0x10)) << 32) | w(t, 0x0c)) + advance; // 004ADC1E..004ADC2A
        sw(t, 0x1c, static_cast<U32>(now)); sw(t, 0x20, static_cast<U32>(now >> 32)); // 004ADC27 / 004ADC2D
        sw(t, 0x0c, static_cast<U32>(current)); sw(t, 0x10, static_cast<U32>(current >> 32)); // 004ADC30 / 004ADC33
        return;
    }
    // 004ADC39..004ADC5E: signed (1000000 / [+4]) * [+8] / 100, sign-extended add.
    const I32 per = 1000000 / static_cast<I32>(w(t, 4));           // 004ADC3E idiv
    const U32 product = static_cast<U32>(per) * w(t, 8);           // 004ADC44 imul (32-bit wrap)
    const I32 step = static_cast<I32>(product) / 100;              // 004ADC48..004ADC59 (truncating)
    const U64 current = ((static_cast<U64>(high) << 32) | low) + static_cast<U64>(static_cast<I64>(step)); // 004ADC5C / 004ADC5E
    sw(t, 0x0c, static_cast<U32>(current)); sw(t, 0x10, static_cast<U32>(current >> 32)); // 004ADC61 / 004ADC64
}

// 004ADC80 CWorldTimer::GetSnapshotTimeDifference, RET, EDX:EAX.
menu_continuation::I64 __thiscall menu_continuation::WorldTimer::GetSnapshotTimeDifference_004adc80() {
    if (w(this, 0x24) != 0) return 0;                              // 004ADC80..004ADC8B
    const U64 current = (static_cast<U64>(w(this, 0x10)) << 32) | w(this, 0x0c);
    const U64 snapshot = (static_cast<U64>(w(this, 0x18)) << 32) | w(this, 0x14);
    return static_cast<I64>(current - snapshot);                   // 004ADC8C..004ADC9B (sub/sbb)
}

// 004ADE40 CWorldTimer::GetWorldTime(day*, ms*), RET 8.
extern "C" void __fastcall frame_GetWorldTime_004ade40(menu_continuation::WorldTimer* t, void*, unsigned* day, unsigned* ms) {
    if (w(t, 0x24) == 1) {                                         // 004ADE47 / 004ADE52
        *day = w(t, 0x28);                                         // 004ADE54..004ADE5B
        *ms = w(t, 0x2c);                                          // 004ADE5D..004ADE64
    } else {
        const U64 current = (static_cast<U64>(w(t, 0x10)) << 32) | w(t, 0x0c); // 004ADE4B / 004ADE4E
        const U64 seconds = current / 1000u;                       // 004ADE71 __aulldiv
        const U64 length = w(t, 0x3c);                             // 004ADE76
        const U64 quotient = seconds / length, remainder = seconds % length; // 004ADE7F __aulldvrm
        *day = w(t, 0x30) + static_cast<U32>(quotient);            // 004ADE84..004ADE99
        *ms = w(t, 0x34) + static_cast<U32>(remainder);            // 004ADE9B..004ADEA0
    }
    // 004ADEA2..004ADEE2: normalise, re-reading [+3C] each step (unsigned compares).
    while (*ms > 0xFFFFFFFFu - w(t, 0x3c)) { --*day; *ms += w(t, 0x3c); } // 004ADEA7..004ADEC5
    while (*ms >= w(t, 0x3c)) { ++*day; *ms -= w(t, 0x3c); }       // 004ADEC7..004ADEE2
}

// 004AE190 CWorldTimer::SetSnapshotScale(float), RET 4: [+8] = ftol2(scale * 100.0f).
extern "C" void __fastcall kotor_fastcall_004ae190(void* timer, void*, float scale) {
    U32 result;
    __asm {
        fld scale                                   // 004AE190
        fmul dword ptr k_hundred_00741838           // 004AE195
        call unresolved_ftol2_006fae8c              // 004AE19D
        // Declare the conversion helper's caller-saved register clobbers.
        mov eax, eax
        mov ecx, ecx
        mov edx, edx
        mov result, eax
    }
    sw(timer, 8, result);                                          // 004AE1A2
}

// ======================= CExoTimers / CClientExoApp getters =======================
// 005E9260 CExoTimers::GetHighResolutionTimer: the M41 provider is the genuine body on
// the same receiver (world-timer-high-resolution.obj); this member is the R4 spelling.
menu_continuation::I64 __thiscall menu_continuation::ExoTimers::GetHighResolutionTimer_005e9260() {
    return static_cast<I64>(reinterpret_cast<kotor_menu_10_19::TimerDependency*>(this)->high_resolution_005e9260());
}
// 005ED480 GetCaptureEventPlayBack: [[ecx+4]+90h].
extern "C" unsigned __fastcall frame_R4_005ed480(void* client, void*) { return w(pw(client, 4), 0x90); }
// 005ED690 GetInGameGui: [[ecx+4]+40h] (R4 +1A8 path; not reached on frame 1).
extern "C" unsigned __fastcall frame_R4_005ed690(void* client, void*) { return w(pw(client, 4), 0x40); }
// 005ED4F0 GetWorldTimer: [[ecx+4]+24h].
extern "C" menu_continuation::WorldTimer* __fastcall frame_GetWorldTimer_005ed4f0(void* client, void*) {
    return static_cast<menu_continuation::WorldTimer*>(pw(pw(client, 4), 0x24));
}
// 005ED500 GetPauseTimer: [[ecx+4]+2Ch]; 005ED510 GetAnimationTimer: [[ecx+4]+30h].
extern "C" void* __fastcall kotor_fastcall_005ed500(void* client, void*) { return pw(pw(client, 4), 0x2c); }
extern "C" void* __fastcall kotor_fastcall_005ed510(void* client, void*) { return pw(pw(client, 4), 0x30); }
// 004AEDE0 / 004AEE00 CServerExoApp::GetWorldTimer / GetPauseTimer: [[ecx+4]+10048h] / +10050h.
extern "C" void* __fastcall kotor_fastcall_004aede0(void* server, void*) { return pw(pw(server, 4), 0x10048); }
extern "C" void* __fastcall kotor_fastcall_004aee00(void* server, void*) { return pw(pw(server, 4), 0x10050); }
// 005ED530 GetModule: mov ecx,[ecx+4]; jmp 00644760 (mov eax,[ecx+18h]).
extern "C" void* __fastcall kotor_fastcall_005ed530(void* client, void*) { return pw(pw(client, 4), 0x18); }
// 005ED5B0 GetCreatureByGameObjectID: mov ecx,[ecx+4]; jmp 005F1180 (RET 4).
extern "C" unsigned __fastcall frame_R4_005ed5b0(void* client, void*, unsigned id) {
    return kotor_fastcall_005f1180(pw(client, 4), nullptr, id);
}
// 005F1180 CClientExoAppInternal::GetCreatureByGameObjectID, RET 4.
extern "C" unsigned __fastcall kotor_fastcall_005f1180(void* internal, void*, unsigned id) {
    void* object = nullptr;                                        // 005F118E (stored before the call)
    const unsigned char status = kotor_fastcall_004d8230(pw(internal, 0x14), nullptr, id, &object); // 005F1185..005F1196
    if (status != k_object_found_0074666c) return 0;               // 005F119B / 005F11A1
    if (!object) return 0;                                         // 005F11A7
    if (b8(object, 8) != k_creature_type_00746634) return 0;       // 005F11AB..005F11B4
    return vcall0(object, 0x2c);                                   // 005F11B6..005F11B8 (EAX result)
}
// 004D8230 CGameObjectArray::GetGameObject(id, out*), RET 8, AL status (0 found, 1 absent).
extern "C" unsigned char __fastcall kotor_fastcall_004d8230(void* array, void*, std::uint32_t id, void** out) {
    const U32 table = w(array, 0);                                 // 004D8234
    const U32 index = (((id >> 31) & 1u) << 12) + (id & 0xFFFu);   // 004D8236..004D824A
    U32 node = w(at(table), index * 4);                            // 004D824C
    const U32 key = id & 0x7FFFFFFFu;                              // 004D824F
    while (node) {                                                 // 004D8255 / 004D8267
        if (w(at(node), 0) == key) { *out = pw(at(node), 4); return 0; } // 004D8260 / 004D827B..004D8284
        node = w(at(node), 8);                                     // 004D8264
    }
    *out = nullptr;                                                // 004D826B..004D826F
    return 1;                                                      // 004D8275
}

// 006346C0 CSWParty::GetCharacter(int), RET 4; tail-jumps to 005ED5B0.
extern "C" void* __fastcall frame_GetCharacter_006346c0(void* party, void*, int index) {
    if (index < 0) return nullptr;                                 // 006346C4 jl
    if (index >= static_cast<I32>(w(party, 0))) return nullptr;    // 006346C8 jge
    const U32 id = w(party, static_cast<U32>(index) * 0x88u + 0x24u); // 006346CC..006346D2
    void* client = client_of(menu_continuation::app_manager_007a39fc); // 006346D6..006346DC
    return at(frame_R4_005ed5b0(client, nullptr, id));             // 006346E3 jmp
}

// ======================= CClientExoAppInternal =======================
// 005F2F60 SetGameSpeed(float), RET 4.
extern "C" void __fastcall frame_R4_005f2f60(void*, void*, float speed) {
    AppManager* manager = menu_continuation::app_manager_007a39fc;     // 005F2F60
    if (!manager) return;                                          // 005F2F67
    if (client_of(manager)) {                                      // 005F2F69..005F2F73
        kotor_fastcall_004ae190(frame_GetWorldTimer_005ed4f0(client_of(manager), nullptr), nullptr, speed); // 005F2F75..005F2F7D
        manager = menu_continuation::app_manager_007a39fc;
        kotor_fastcall_004ae190(kotor_fastcall_005ed500(client_of(manager), nullptr), nullptr, speed); // 005F2F82..005F2F92
        manager = menu_continuation::app_manager_007a39fc;
        kotor_fastcall_004ae190(kotor_fastcall_005ed510(client_of(manager), nullptr), nullptr, speed); // 005F2F97..005F2FA8
        manager = menu_continuation::app_manager_007a39fc;         // 005F2FAD
    }
    if (!server_of(manager)) return;                               // 005F2FB2..005F2FB7
    kotor_fastcall_004ae190(kotor_fastcall_004aede0(server_of(manager), nullptr), nullptr, speed); // 005F2FB9..005F2FC1
    manager = menu_continuation::app_manager_007a39fc;             // 005F2FC6
    kotor_fastcall_004ae190(kotor_fastcall_004aee00(server_of(manager), nullptr), nullptr, speed); // 005F2FCC..005F2FD7
}

// 005F7330 UpdateDeathSequence, RET.
extern "C" void __fastcall frame_R4_005f7330(void* self, void*) {
    const U64 ticks = static_cast<U64>(menu_continuation::exo_base_007a39e0->timers->GetHighResolutionTimer_005e9260()); // 005F7330..005F733E
    const U32 now = static_cast<U32>(ticks / 1000u);               // 005F734C __aulldiv (low word kept)
    const U32 previous = w(self, 0x2c8);                           // 005F7351
    sw(self, 0x2c8, now);                                          // 005F7359
    U32 diff = now - previous;                                     // 005F7365
    if (w(self, 0x2c0) == 0) return;                               // 005F735F / 005F7369
    const U32 reset = w(self, 0x2c4);                              // 005F7387
    U32 rate = w(self, 0x2d0);                                     // 005F738F / 005F739B
    U32 speed = 0x3E4CCCCDu;                                       // 005F739F (0.2f)
    U32 elapsed = 0;
    unsigned char* base = bytes(self);
    __asm {
        mov ecx, base
        mov eax, diff
        fild dword ptr diff                         // 005F7375
        test eax, eax                               // 005F736F
        jge no_bias                                 // 005F7379
        fadd dword ptr k_two32_0073d770             // 005F737B
    no_bias:
        fmul dword ptr k_milli_0073d708             // 005F7381
        fadd dword ptr [ecx+2CCh]                   // 005F7395
        fst dword ptr [ecx+2CCh]                    // 005F73A7
        fst dword ptr elapsed                       // 005F73AD
        cmp reset, 0                                // 005F738D
        je no_reset                                 // 005F73B1
        fld dword ptr elapsed                       // 005F73B3
        mov dword ptr [ecx+2C4h], 0                 // 005F73B7
        fmul dword ptr k_quarter_0073edf0           // 005F73C1
        fldln2                                      // 005F73C7
        fxch st(1)                                  // 005F73C9
        fyl2x                                       // 005F73CB
        fmul qword ptr k_scale_0074ea18             // 005F73CD
        fst dword ptr [ecx+2D0h]                    // 005F73D3
        fstp dword ptr rate                         // 005F73D9
    no_reset:
        fcomp dword ptr k_four_0073f404             // 005F73DD (unrounded accumulated value)
        fnstsw ax
        test ah, 41h
        jp keep_speed                               // 005F73E8
        fld dword ptr elapsed                       // 005F73EA
        fmul dword ptr k_quarter_0073edf0           // 005F73EE
        fldln2
        fxch st(1)
        fyl2x                                       // 005F73F8
        fchs                                        // 005F73FA
        fdiv dword ptr rate                         // 005F73FC
        fadd dword ptr k_fifth_0073e46c             // 005F7400
        fstp dword ptr speed                        // 005F7406
    keep_speed:
    }
    frame_R4_005f2f60(self, nullptr, as_float(speed));             // 005F740A..005F7411
    auto* in_game = static_cast<menu_continuation::InGame*>(pw(self, 0x40)); // 005F7416
    if (in_game->IsGlobalFading_0062ac60() != 0 && w(self, 0x1a8) == 0) return; // 005F7419..005F742A
    if (!(f(self, 0x374) <= 0.0f)) return;                         // 005F742C..005F743D (jp: greater or unordered)
    frame_R4_005f2f60(self, nullptr, 1.0f);                        // 005F743F..005F7446
    sw(self, 0x2c0, 0);                                            // 005F744E
    sw(self, 0x2c4, 1);                                            // 005F7458
    kotor_fastcall_0062ac40(pw(self, 0x40), nullptr);              // 005F744B / 005F7462
    void* game = pw(self, 0x40);                                   // 005F7467
    sw(self, 0x1a8, 1);                                            // 005F746A
    sw(self, 0x374, 0);                                            // 005F7474
    sw(self, 0x1ac, 1);                                            // 005F747E
    void* box = pw(game, 0x98);                                    // 005F748E
    kotor_fastcall_0040be00(kotor_input_cells::gui_manager_007a39f4, nullptr); // 005F7488..005F7494
    frame_RemovePanel_0040c830(static_cast<menu_continuation::GuiManager*>(kotor_input_cells::gui_manager_007a39f4), nullptr, box); // 005F7499..005F74A0
}

// 005F7640 UpdateHostileHilite(float), RET 4.
void __thiscall menu_continuation::Client::UpdateHostileHilite_005f7640(float delta) {
    for (I32 i = static_cast<I32>(w(this, 0x318)) - 1; i >= 0; --i) { // 005F7645..005F764C / 005F7701..005F7702
        U32 data = w(this, 0x314);                                 // 005F7652
        sf(at(data), static_cast<U32>(i) * 8u + 4u, f(at(data), static_cast<U32>(i) * 8u + 4u) - delta); // 005F7658..005F7664
        data = w(this, 0x314);                                     // 005F7666
        if (!(f(at(data), static_cast<U32>(i) * 8u + 4u) <= 0.0f)) continue; // 005F766C..005F767E
        void* object = nullptr;                                    // 005F768F
        const unsigned char status = kotor_fastcall_004d8230(pw(this, 0x14), nullptr, w(at(data), static_cast<U32>(i) * 8u), &object); // 005F7684..005F7697
        if (status == k_object_found_0074666c && object && b8(object, 8) == k_creature_type_00746634) { // 005F769C..005F76B5
            const U32 creature = vcall0(object, 0x2c);             // 005F76B7..005F76B9
            if (creature) vcall1(at(creature), 0xa8, 0);           // 005F76BC..005F76C6
        }
        const I32 count = static_cast<I32>(w(this, 0x318)) - 1;    // 005F76CC..005F76D2
        sw(this, 0x318, static_cast<U32>(count));                  // 005F76D9
        if (i >= count) continue;                                  // 005F76D5 / 005F76DF
        for (I32 j = i; j < static_cast<I32>(w(this, 0x318)); ++j) { // 005F76E1..005F76FF
            const U32 slot = w(this, 0x314) + static_cast<U32>(j) * 8u;
            sw(at(slot), 0, w(at(slot), 8));                       // 005F76E7..005F76EE
            sw(at(slot), 4, w(at(slot), 0x0c));                    // 005F76F0..005F76F3
        }
    }
}

// 005F2AE0 LoadScreenExists: [+278] && [+274] ? PanelExists([+274],[+278]) : 0.
extern "C" unsigned __fastcall kotor_fastcall_005f2ae0(void* internal, void*) {
    void* screen = pw(internal, 0x278);                            // 005F2AE0
    if (!screen) return 0;
    void* gui = pw(internal, 0x274);                               // 005F2AEA
    if (!gui) return 0;
    return frame_ContainsPanel_0040bd70(static_cast<menu_continuation::GuiManager*>(gui), nullptr, screen); // 005F2AF4..005F2AF5
}

// 005F7500 UpdateRumble(float), RET 4 (R4 name frame_SelectedHilite_005f7500).
extern "C" void __fastcall frame_SelectedHilite_005f7500(menu_continuation::Client* client, void*, float delta) {
    void* self = client;
    U32 left = 0, right = 0;                                       // 005F7510 [L4], 005F7518 [L0]
    auto* in_game = static_cast<menu_continuation::InGame*>(pw(self, 0x40));
    if (delta > 0.0f && w(self, 0x288) == 0 &&                     // 005F7503..005F7533
        in_game->IsGlobalFadeObscuring_0062ded0() == 0 &&          // 005F7539..005F7543
        static_cast<menu_continuation::InGame*>(pw(self, 0x40))->IsGlobalFading_0062ac60() == 0 && // 005F7549..005F7553
        kotor_fastcall_005f2ae0(self, nullptr) == 0) {             // 005F7559..005F7562
        for (I32 i = static_cast<I32>(w(self, 0x350)) - 1; i >= 0; --i) { // 005F7569..005F7570 / 005F7600..005F7606
            void* entry = at(w(self, 0x34c) + static_cast<U32>(i) * 12u); // 005F7584..005F758A
            if (w(entry, 8)) sw(entry, 8, 0);                      // 005F7586..005F7590
            else sf(entry, 4, delta + f(entry, 4));                // 005F7599..005F75A0
            U32 a = 0, b = 0;  // [L8]/[LC]: written by the callee (value-initialised; see PACKET deviation note)
            const U32 time_bits = w(entry, 4);                     // 005F75A8
            void* pattern = at(w(self, 0x340) + w(entry, 0) * 36u); // 005F75AB..005F75BC
            if (kotor_fastcall_0068fdd0(pattern, nullptr, time_bits, &a, &b) == 0) { // 005F75BF..005F75C6
                kotor_fastcall_0048dde0(bytes(self) + 0x34c, nullptr, i); // 005F75C8..005F75CB
                continue;
            }
            if (!(as_float(left) > as_float(a))) left = a;         // 005F75D2..005F75E5 (je when greater)
            if (!(as_float(right) > as_float(b))) right = b;       // 005F75E9..005F75FC
        }
    }
    // 005F760F..005F7626: SetRumble(0, [L4], [L0], 927C0h).
    kotor_fastcall_005df550(kotor_menu_window_cells::input_007a39e4, nullptr, 0, left, right, 0x927C0u);
}

// 005DF550 CExoInput::SetRumble: mov ecx,[ecx+4]; jmp 00401590 (xor eax,eax; ret 10h).
extern "C" std::uint32_t __fastcall kotor_fastcall_005df550(void* input, void*, std::uint32_t a, std::uint32_t b, std::uint32_t c, std::uint32_t d) {
    return kotor_fastcall_00401590(pw(input, 4), nullptr, a, b, c, d);
}
extern "C" std::uint32_t __fastcall kotor_fastcall_00401590(void*, void*, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t) {
    return 0;                                                      // 00401590 xor eax,eax; 00401592 ret 10h
}

// ======================= CNetLayer =======================
// 005D57D0 ProcessReceivedFrames(int), RET 4.
extern "C" void __fastcall frame_R4_005d57d0(void* net, void*, unsigned enable) {
    if (enable == 0) return;                                       // 005D57D4 / 005D57D9
    if (w(net, 0x20000) == w(net, 0x20004)) return;                // 005D57DF..005D57EB
    U32 next;
    do {
        if (w(net, 0x20000) >= 0x10000u) sw(net, 0x20000, 0);      // 005D57F0..005D57FC
        const U32 position = w(net, 0x20000);                      // 005D5806
        const U32 length = w(net, position);                       // 005D580C
        const U32 body = position + 4;                             // 005D580F
        next = body - (length & 3u) + length + 4u;                 // 005D5812..005D581B
        const char* message = reinterpret_cast<const char*>(bytes(net) + body); // 005D581F
        sw(net, 0x20000, body);                                    // 005D5822
        if (message[0] == 'B' && message[1] == 'N')                // 005D5828..005D5832
            kotor_fastcall_005d5690(net, nullptr, 0, message, length); // 005D5834..005D583A
        else
            vcall4(pw(net, 0x20008), 4, 0, reinterpret_cast<U32>(message), length, 0); // 005D5841..005D584F
        sw(net, 0x20000, next);                                    // 005D5852
    } while (next != w(net, 0x20004));                             // 005D5858..005D5862
}
// 005D5650 UpdateStatusLoop(int), RET 4 (EAX=1, not consumed by R4).
extern "C" void __fastcall frame_R4_005d5650(void* net, void*, unsigned mode) {
    if (mode == 0x400000u && w(net, 0x20060) == 1 && w(net, 0x20070) == 1 && w(net, 0x20074) != 2) // 005D5650..005D5673
        kotor_fastcall_005d5140(net, nullptr);                     // 005D5675
}
// 005D5140: if [+20060] clear +20060/+20070/+20074/+20078 and return 1; else 0.
extern "C" unsigned __fastcall kotor_fastcall_005d5140(void* net, void*) {
    if (w(net, 0x20060) == 0) return 0;                            // 005D5140..005D514C
    sw(net, 0x20060, 0); sw(net, 0x20070, 0); sw(net, 0x20074, 0); sw(net, 0x20078, 0); // 005D514D..005D515F
    return 1;
}

// ======================= server wrappers =======================
// 004AE8A0 CServerExoApp::UnloadModule: mov ecx,[ecx+4]; jmp 004B9240 (not reached).
extern "C" void __fastcall frame_R4_004ae8a0(void* server, void*) { kotor_fastcall_004b9240(pw(server, 4), nullptr); }
// 004AE980 GetPauseState(mask): mov ecx,[ecx+4]; jmp 004B1D80 -> (byte[+10078h] & mask) != 0.
extern "C" unsigned __fastcall frame_GetPauseState_004ae980(void* server, void*, unsigned mask) {
    const unsigned char bits = static_cast<unsigned char>(b8(pw(server, 4), 0x10078) & static_cast<unsigned char>(mask)); // 004B1D82..004B1D88
    return bits != 0 ? 1u : 0u;                                    // 004B1D8C..004B1D92 (cmp/sbb/neg)
}
// 00677800 SendPlayerToServerInput_TogglePauseRequest, RET, EAX=1.
extern "C" unsigned __fastcall frame_TogglePauseRequest_00677800(void*, void*) {
    void* server = server_of(menu_continuation::app_manager_007a39fc); // 00677800..00677805
    if (server) kotor_fastcall_004ae960(server, nullptr, 2);       // 0067780A..0067780E
    return 1;                                                      // 00677813
}
// 00605640 CSWCArea::GetServerArea, RET.
extern "C" void* __fastcall frame_GetServerArea_00605640(void* area, void*) {
    if (w(area, 0x27c) == 0) {                                     // 00605643..0060564B
        void* server = server_of(menu_continuation::app_manager_007a39fc); // 0060564D..00605653
        const U32 id = kotor_fastcall_004aea30(server, nullptr, w(area, 0x104)); // 00605656..0060565F
        void* server_area = kotor_fastcall_004ae780(server, nullptr, id); // 00605664..00605667
        sw(area, 0x27c, reinterpret_cast<U32>(server_area));       // 0060566E
        if (server_area) kotor_fastcall_00506a60(server_area, nullptr, area); // 00605675..0060567A
    }
    return pw(area, 0x27c);                                        // 0060567F
}
// 00401280 CAppManager::DestroyServer: single body = lane-routes @kotor_fastcall_00401280@8 (D65 C2);
// frame_R4_00401280 is joined to it in m45-joins/integrator_joins_m45.cpp (fastcall(ECX,EDX) @8 both).

// ======================= CGuiInGame =======================
// 0062B020 SetAreaTransition(id, position*), RET 8.
extern "C" void __fastcall frame_R4_0062b020(void* in_game, void*, unsigned id, unsigned* position) {
    sw(in_game, 0xc0c, position[0]);                               // 0062B025..0062B02D
    sw(in_game, 0xc10, position[1]);                               // 0062B02F..0062B032
    sw(in_game, 0xc14, position[2]);                               // 0062B035..0062B038
    sw(in_game, 0xc18, id);                                        // 0062B03B..0062B03F
}
// 0062EF70 GetPendingStatusSummary.
menu_continuation::I32 __thiscall menu_continuation::InGame::GetPendingStatusSummary_0062ef70() {
    void* summary = pw(this, 0xa8);                                // 0062EF70
    return (summary && (b8(summary, 0x64) & 3u) != 0) ? 1 : 0;     // 0062EF76..0062EF88
}
// 0062AC60 IsGlobalFading.
menu_continuation::I32 __thiscall menu_continuation::InGame::IsGlobalFading_0062ac60() {
    void* fade = pw(this, 0x6c);                                   // 0062AC60
    if (!fade) return 0;                                           // 0062AC63..0062AC65
    return kotor_fastcall_006244b0(fade, nullptr) == 0 ? 1 : 0;    // 0062AC67..0062AC78
}
// 0062DED0 IsGlobalFadeObscuring: [[+6C]+4Ch] > [0073D708] (no null test, as original).
menu_continuation::I32 __thiscall menu_continuation::InGame::IsGlobalFadeObscuring_0062ded0() {
    return f(pw(this, 0x6c), 0x4c) > frame_control::profile_scale_0073d708 ? 1 : 0; // 0062DED0..0062DEEB (test ah,41h)
}
// 006244B0 CSWGuiFade::IsFadeDone.
extern "C" int __fastcall kotor_fastcall_006244b0(void* fade, void*) {
    int done = 0;
    unsigned char* base = bytes(fade);
    __asm {
        mov ecx, base
        fld dword ptr [ecx+6Ch]                     // 006244B0
        fcomp dword ptr k_min_0073d700              // 006244B3
        fnstsw ax
        test ah, 44h
        jnp is_done                                 // 006244BE
        fld dword ptr [ecx+68h]                     // 006244C0
        fadd dword ptr [ecx+64h]                    // 006244C3
        fcomp dword ptr [ecx+6Ch]                   // 006244C6
        fnstsw ax
        test ah, 5
        jp not_done                                 // 006244CE
    is_done:
        mov done, 1                                 // 006244D0
    not_done:
    }
    return done;
}
// 0062AC40 StopGlobalFade, RET.
extern "C" void __fastcall kotor_fastcall_0062ac40(void* in_game, void*) {
    void* fade = pw(in_game, 0x6c);                                // 0062AC43
    if (!fade) return;                                             // 0062AC46..0062AC48
    kotor_fastcall_006247a0(fade, nullptr);                        // 0062AC4A
    frame_RemovePanel_0040c830(static_cast<menu_continuation::GuiManager*>(pw(in_game, 0x38)), nullptr, pw(in_game, 0x6c)); // 0062AC4F..0062AC56
}
// 0062B050 RePopulateMainInterface: [+90] ? jmp 00689D80.
extern "C" void __fastcall frame_RePopulate_0062b050(menu_continuation::InGame* in_game, void*) {
    void* main_interface = pw(in_game, 0x90);                      // 0062B050
    if (main_interface) kotor_fastcall_00689d80(main_interface, nullptr); // 0062B056..0062B05A
}
// 0062F780 UpdateControllerPauseState, RET (R4 name frame_UpdateDisableDialogue_0062f780).
extern "C" void __fastcall frame_UpdateDisableDialogue_0062f780(menu_continuation::InGame* in_game, void*) {
    if (kotor_fastcall_005df560(kotor_menu_window_cells::input_007a39e4, nullptr) == 0) return; // 0062F783..0062F790
    if (w(in_game, 0xd8) != 0) return;                             // 0062F792..0062F79A (ja)
    if (w(in_game, 0xdc) != 0) return;                             // 0062F79C..0062F7A4
    sw(in_game, 0xc08, kotor_fastcall_005df510(kotor_menu_window_cells::input_007a39e4, nullptr)); // 0062F7A6..0062F7B1
    kotor_fastcall_005df540(kotor_menu_window_cells::input_007a39e4, nullptr, 1); // 0062F7B7..0062F7BF
    kotor_fastcall_0062f590(in_game, nullptr);                     // 0062F7C4..0062F7C6
    sw(in_game, 0xdc, 1);                                          // 0062F7CB
}
// 005DF560 CExoInput::GetMainDeviceLost: mov ecx,[ecx+4]; jmp 005DFFD0.
extern "C" int __fastcall kotor_fastcall_005df560(void* input, void*) { return kotor_fastcall_005dffd0(pw(input, 4), nullptr); }
// 005DFFD0: mov ecx,[ecx+140h]; jmp 0063E7F0 (xor eax,eax; ret): result is 0.
extern "C" int __fastcall kotor_fastcall_005dffd0(void*, void*) { return 0; }

// 006339C0 UpdateDialog, RET.
extern "C" void __fastcall frame_GuiDispatch_006339c0(void* self, void*) {
    if (w(self, 0xb4) == 0) return;                                // 006339C6..006339D1
    if (w(self, 0xbfc) == 1) {                                     // 006339D7
        void* module = kotor_fastcall_005ed530(client_of(menu_continuation::app_manager_007a39fc), nullptr); // 006339E0..006339E8
        void* camera = pw(module, 0x40);                           // 006339ED
        if (camera) {                                              // 006339F2
            vcall1(camera, 0x44, w(self, 0xc00));                  // 006339F4..006339FD
            sw(self, 0xbfc, 0);                                    // 00633A00
        }
    }
    if (w(self, 0xd0) == 0) return;                                // 00633A06
    const U32 speakers = w(self, 0x120);                           // 00633A12
    U32 speaker = w(self, 0x170);                                  // 00633A1A
    const U32 node = w(self, 0xc8);                                // 00633A21
    if (speakers == 0) return;                                     // 00633A28
    const U32 entry = w(at(speakers), node * 4);                   // 00633A2E
    if (entry != 0x7F000000u) speaker = entry;                     // 00633A31..00633A38
    const U32 listener = w(self, 0x184);                           // 00633A43
    void* object = kotor_fastcall_005ed580(client_of(menu_continuation::app_manager_007a39fc), nullptr, speaker); // 00633A3A..00633A4A
    if (!object || vcall0(object, 0x0c) == 0) return;              // 00633A4F..00633A60
    object = kotor_fastcall_005ed580(client_of(menu_continuation::app_manager_007a39fc), nullptr, listener); // 00633A66..00633A6F
    if (!object || vcall0(object, 0x0c) == 0) return;              // 00633A74..00633A85
    if (static_cast<I32>(w(at(w(self, 0x154)), node * 4)) > 0) {   // 00633A8B..00633A95
        I32 i = 0;
        do {
            const U32 id = w(at(w(at(w(self, 0x158)), node * 4)), static_cast<U32>(i) * 4u); // 00633AA0..00633AA9
            void* participant = kotor_fastcall_005ed580(client_of(menu_continuation::app_manager_007a39fc), nullptr, id); // 00633AAC..00633AB6
            if (!participant || vcall0(participant, 0x0c) == 0) return; // 00633ABB..00633ACC
            ++i;                                                   // 00633ADB
        } while (i < static_cast<I32>(w(at(w(self, 0x154)), node * 4))); // 00633AD2..00633ADE
    }
    kotor_fastcall_00630560(self, nullptr, at(w(self, 0x118) + w(self, 0xc8) * 8u), listener); // 00633AE0..00633AF3
    if (w(self, 0xd4) == 0) {                                      // 00633AF8..00633B00
        kotor_fastcall_00633660(self, nullptr, w(self, 0xc8), w(self, 0xcc)); // 00633C31..00633C41
        return;
    }
    U32 reply;                                                     // [L4]
    if (static_cast<I32>(w(self, 0xc8)) >= 0) reply = kotor_fastcall_006a6d80(pw(self, 0x3c), nullptr, w(self, 0xc8)); // 00633B06..00633B19
    else reply = 0xFFFFFFFFu;                                      // 00633B1F
    const U32 ended = w(self, 0xc8) == 0xFFFFFFFDu ? 1u : 0u;      // 00633B27..00633B3B [LC]
    const U32 player = kotor_fastcall_004aea40(server_of(menu_continuation::app_manager_007a39fc), nullptr); // 00633B3F..00633B42
    void* player_object = kotor_fastcall_004ae910(server_of(menu_continuation::app_manager_007a39fc), nullptr, player); // 00633B47..00633B51 [L8]
    const U32 owner_id = kotor_fastcall_004aea30(server_of(menu_continuation::app_manager_007a39fc), nullptr, w(pw(self, 0x3c), 0x19c0)); // 00633B56..00633B6D
    void* owner = kotor_fastcall_004ae750(server_of(menu_continuation::app_manager_007a39fc), nullptr, owner_id); // 00633B72..00633B81
    if (w(self, 0xc0) == 0) {                                      // 00633B83..00633B8B
        const U32 count = w(at(w(self, 0x154)), node * 4);         // 00633B8D..00633B93 [L0]
        kotor_fastcall_00630620(self, nullptr, count);             // 00633B96..00633B9D
        for (I32 i = 0; i < static_cast<I32>(count); ++i) {        // 00633BA2..00633BDC
            const U32 animation = static_cast<std::uint16_t>(w(at(w(at(w(self, 0x15c)), node * 4)), static_cast<U32>(i) * 2u)); // 00633BB0..00633BC1
            const U32 id = w(at(w(at(w(self, 0x158)), node * 4)), static_cast<U32>(i) * 4u); // 00633BC5..00633BCB
            kotor_fastcall_00628710(self, nullptr, static_cast<U32>(i), id, animation); // 00633BCA..00633BD0
        }
        kotor_fastcall_006313a0(self, nullptr);                    // 00633BDE..00633BE0
    }
    if (owner && vcall0(owner, 0x10) != 0) {                       // 00633BE5..00633BF3
        const U32 a5 = w(pw(self, 0x3c), 0x64);                    // 00633BF5..00633BF8
        const U32 a1 = w(player_object, 4);                        // 00633BFF..00633C03
        const U32 server_object = vcall0(owner, 0x10);             // 00633C0E..00633C15 (after the pushes)
        kotor_fastcall_004cb480(at(server_object), nullptr, a1, reply, ended, a5, 1); // 00633C06..00633C1A
    }
    sw(self, 0xd0, 0);                                             // 00633C22
}

// 00632180 UpdateAreaTransitionGUI, RET.
extern "C" void __fastcall frame_GuiDispatch_00632180(void* self, void*) {
    if (w(self, 0x108) == 0 || w(self, 0x38) == 0 || w(self, 0x94) == 0) return; // 00632186..006321A7
    bool hide = true;
    do {
        if (!kotor_fastcall_005ed530(client_of(menu_continuation::app_manager_007a39fc), nullptr)) break; // 006321AD..006321BD
        void* area = pw(kotor_fastcall_005ed530(client_of(menu_continuation::app_manager_007a39fc), nullptr), 0x48); // 006321C3..006321D1
        if (!area) break;                                          // 006321D4
        void* server_area = frame_GetServerArea_00605640(area, nullptr); // 006321DC
        if (!server_area) break;                                   // 006321E3
        if (w(self, 0x34) != 1) break;                             // 006321EB
        hide = false;
        void* panel = pw(self, 0x94);                              // 006321F5
        float position[3];                                         // [esp+8..10h]
        std::memcpy(position, bytes(panel) + 0x424, 12);           // 006321FB..00632210
        if (w(panel, 0x430) == w(self, 0xc18)) {                   // 00632203 / 00632217
            float difference[3];                                   // [esp+14h..]
            int moved = 0;
            float* diff_out = difference; float* diff_a = position; unsigned char* diff_b = bytes(self) + 0xc0c;
            __asm {
                push diff_b                                        // 00632229
                push diff_a                                        // 0063222E
                push diff_out                                      // 00632233
                call kotor_cdecl_00423bc0                          // 00632234
                push eax                                           // 00632239
                call kotor_cdecl_004ab100                          // 0063223A
                fcomp dword ptr k_tenthousandth_00741870           // 0063223F
                add esp, 10h                                       // 00632245
                fnstsw ax
                test ah, 5
                jnp done_compare                                   // 0063224D (less -> return)
                mov moved, 1
            done_compare:
            }
            if (!moved) return;                                    // 006322EB
        }
        const U32 target = w(self, 0xc18);                         // 00632253
        if (target == 0x7F000000u) { hide = true; break; }         // 00632259..0063225E
        if (w(server_area, 0x2c4) != 0) {                          // 00632264..0063226C
            if (frame_ContainsPanel_0040bd70(static_cast<menu_continuation::GuiManager*>(pw(self, 0x38)), nullptr, pw(self, 0x94)) == 0) return; // 0063226E..0063227F
            kotor_fastcall_0040bd20(pw(self, 0x38), nullptr, pw(self, 0x94)); // 00632281..0063228B
            return;
        }
        kotor_fastcall_006c7ec0(pw(self, 0x94), nullptr, target, bytes(self) + 0xc0c); // 00632296..006322A4
        if (frame_ContainsPanel_0040bd70(static_cast<menu_continuation::GuiManager*>(pw(self, 0x38)), nullptr, pw(self, 0x94)) != 0) return; // 006322A9..006322BA
        kotor_fastcall_0040bc70(static_cast<gui_ctor::Gui*>(pw(self, 0x38)), nullptr, pw(self, 0x94), 4, 1); // 006322BC..006322CA
        kotor_fastcall_0040bd20(pw(self, 0x38), nullptr, pw(self, 0x94)); // 006322CF..006322D9
        return;
    } while (false);
    if (hide) kotor_fastcall_0062eb10(self, nullptr);              // 006322E4..006322E6
}

// 00423BC0 Vector::operator-= helper (cdecl, out = a - b, EAX = out).
extern "C" float* __cdecl kotor_cdecl_00423bc0(float* out, const float* a, const float* b) {
    auto* const resultPointer = out;
    // Keep the original x87 register/m32 operand roles and load every input
    // before the first store: early widening changes signalling-NaN selection.
    __asm {
        mov eax, a
        mov ecx, b
        fld dword ptr [eax+8]
        fsub dword ptr [ecx+8]
        fld dword ptr [eax+4]
        fsub dword ptr [ecx+4]
        fld dword ptr [eax]
        fsub dword ptr [ecx]
        mov eax, resultPointer
        fstp dword ptr [eax]
        fstp dword ptr [eax+4]
        fstp dword ptr [eax+8]
    }
    return out;
}
// 004AB100 vector_magnitude: single body = lane-3d-render _vector_magnitude_004ab100 (D50/D65); kotor_cdecl_004ab100 joined in m45-joins.

// ======================= CClientExoApp::GetInFreeLook (R4 frame_UnknownClientFlag_005ee230) =======================
extern "C" unsigned __fastcall frame_UnknownClientFlag_005ee230(void* client, void*) {
    if (!pw(pw(client, 4), 0x18)) return 0;                        // 005EE233..005EE23D (00644760)
    if (!pw(pw(pw(client, 4), 0x18), 0x40)) return 0;              // 005EE23F..005EE24C
    void* camera = pw(pw(pw(client, 4), 0x18), 0x40);              // 005EE24E..005EE256
    return vcall1(camera, 0x80, 0x106E) != 0 ? 1u : 0u;            // 005EE259..005EE26A
}

// ======================= Client::ShowMouse (R4 member spelling) =======================
// One genuine body (kotor_game_prepare::ShowMouse0061fa00, adopted from lane-prepare);
// the R4 member forwards with the same receiver and argument word.
void __thiscall menu_continuation::Client::ShowMouse_0061fa00(I32 flags) {
    kotor_game_prepare::ShowMouse0061fa00(this, nullptr, flags);
}

// ======================= CExoString::IsEmpty =======================
extern "C" unsigned __fastcall frame_LogIsEmpty_005e58a0(void* string, void*) {
    const char* text = static_cast<const char*>(pw(string, 0));    // 005E58A0
    return (!text || *text == 0) ? 1u : 0u;                       // 005E58A2..005E58B3
}
