// A3 private reconstruction: original 00687860 CSWGuiMainInterface::UpdatePortraits.
//
// Source-only lane. Intended placement is the donor HUD source directory
// (e.g. lane-w4/src/hud/hud_update_portraits.cpp) so that the relative include
// below resolves to the existing shared W4 definitions. This TU does not edit,
// fork or replace any shared header, table, provider or generator.
//
// Original ABI: thiscall() RET 0. The reconstruction exports the established
// __fastcall(self, dummy EDX) form consumed by the real Draw donor.
//
// Three nested providers are absent from the selected 1565 composition and are
// explicit linkage gates, not traps/fakes:
//   A3-G1 006857D0 CSWGuiMainInterfaceChar::SetHealth
//   A3-G2 005A6610 CSWSCreatureStats::GetExpNeededForLevelUp
//   A3-G3 006867F0 CSWGuiMainInterfaceChar::Show
// The extern declarations below are the exact callable contracts. The body is
// complete; the object is not link-closed until those real bodies exist.
#include "f3461e5d4b-w4_providers.hpp"

using namespace w4;

namespace kotor_menu20 {
struct String;
String* __fastcall ConstructString005b3190(String*, void*);   // CExoString default ctor, 005B3190
}

namespace kotor_original {
void __fastcall kotor_fastcall_00417fb0(void*, void*, std::int32_t); // ProgressBar::SetValue
}

extern "C" {
// Existing selected providers (selected 1565 symbols; do not redefine).
void* __fastcall kotor_fastcall_005ed8b0(void*, void*);                 // GetSWParty
void* __fastcall async_assign_string_005e5c50(void*, void*, void*);     // CExoString::operator=
void* __fastcall frame_GetCharacter_006346c0(void*, void*, U32);
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void*, void*);
void __fastcall gui_load_border_params_set_fill_00414c00(void*, void*, const void*, I32);
void __fastcall gui_load_text_params_set_color_00414e10(void*, void*, const void*);
void __fastcall gui_load_text_params_set_text_00415e00(void*, void*, void*);
void __fastcall gui_m48_progress_set_max_00417f90(void*, void*, I32);
unsigned __fastcall w1x_creature_max_fp_004fd490(void*, void*);
unsigned char __fastcall recovered_stats_level_005a5fd0(void*, void*, U32);
unsigned __fastcall recovered_can_level_up_005a6810(void*, void*);
unsigned __fastcall kotor_fastcall_005b4880(void*, void*);
unsigned __fastcall recovered_creature_incapacitated_004ef890(void*, void*);
void __fastcall kotor_fastcall_0062eeb0(void*, void*, U8, U32);
void __fastcall kotor_fastcall_00687700(void*, void*, U32);
void* __fastcall kotor_fastcall_005edeb0(void*, void*, void*, U32);
extern const U32 kotor_m15d_color_007a23d8[4];

// A3-G1/G2/G3: absent nested providers. Exact contracts only; no placeholder.
void __fastcall kotor_fastcall_006857d0(void*, void*, int current, int max, int kind); // RET 0xC
unsigned __fastcall kotor_fastcall_005a6610(void*, void*);                              // RET 0
void __fastcall kotor_fastcall_006867f0(void*, void*, int, int, int, int);              // RET 0x10
}

namespace {

// 00687895 / 00687E12: CExoString default-construct/destroy local.
struct ExoString {
    U32 storage[2];
    ExoString() {
        kotor_menu20::ConstructString005b3190(
            reinterpret_cast<kotor_menu20::String*>(storage), nullptr);
    }
    ~ExoString() { frame_LogStringDtor_005e5c20(storage, nullptr); }
};

// 00687BD4 / 00687C06: out-filled CExoString. The original does NOT default-
// construct this temp; GetGUIString fills the raw storage and only the dtor runs.
// The dtor guard is armed only *after* the fill (the original SEH state is set at
// 00687BE5, after the 005EDEB0 call), so a throw from the fill does not destroy
// uninitialized storage.
struct TempStringGuard {
    U32* storage;
    ~TempStringGuard() { frame_LogStringDtor_005e5c20(storage, nullptr); }
};

} // namespace

extern "C" void __fastcall kotor_fastcall_00687860(void* self, void*) {
    void* const app = app_manager();                                   // [007A39FC]
    void* const client = lp(app, 4);                                   // [[007A39FC]+4]
    void* const party = kotor_fastcall_005ed8b0(client, nullptr);      // 0068788E

    ExoString scratch;                                                 // 00687895
    (void)scratch;

    for (I32 i = 0; i < 3; ++i) {                                      // 006878A8..00687E00
        void* const member = frame_GetCharacter_006346c0(party, nullptr, static_cast<U32>(i)); // 006878B5
        void* server = nullptr;
        if (member != nullptr)                                         // 006878BC..006878C9
            server = kotor_m15a_creature_get_server_creature_0060fb20(member, nullptr);

        U32 slot = static_cast<U32>(i);                                // 006878E2: initial slot = i
        U32 show = server != nullptr ? 1u : 0u;                        // 006878E6
        U8 state = 0;                                                  // 006878D6
        U8 counter_a = 0;                                              // 006878DA
        U8 counter_b = 0;                                              // 006878DE

        const float timer = lf(self, 0x5A28);                          // 006878EE
        if (jcc_jne(timer, 0.0, 0x41)) {                               // 006878FF: <= or unordered
            if (lw(party, 0) == 2) {                                   // 00687939
                if (i == 1) { slot = 2; show = 0; }                    // 00687920..00687947
                else if (i == 2) { show = 1; }                         // 00687951..00687956
                // i == 0 leaves slot 0 and show unchanged.
            }
            // party->[0] != 2 leaves slot = i and show unchanged.
        } else {                                                       // timer > 0
            if (i == 0) {
                slot = 1;                                              // 0068792F
            } else if (i == 1) {
                if (lw(party, 0) == 2) { slot = 0; show = 0; }         // 00687920..00687929
                else slot = 2;                                        // 00687947
            } else {                                                   // i == 2
                if (lw(party, 0) == 2) show = 1;                       // 0068790D..00687912
                slot = 0;                                              // 0068791A
            }
        }

        if (server != nullptr) {                                       // 0068795E
            U8* const c = static_cast<U8*>(self) + slot * 0xEA8u + 0x1F88u; // 00687976/006879CA

            // 00687983 / 00687998: two CExoString assignments, each re-loads
            // the client creature stats pointer.
            async_assign_string_005e5c50(
                c + 0x18, nullptr,
                static_cast<U8*>(lp(member, 0x2F8)) + 0x14);
            async_assign_string_005e5c50(
                c + 0x20, nullptr,
                static_cast<U8*>(lp(member, 0x2F8)) + 0x1C);

            // 006879A2..006879B8: client GetPortrait(out,1) at vtable +0x148,
            // then SetFillImage(c+0x6D8,ref,0).
            alignas(4) unsigned char ref1[16];                         // [esp+0x48]
            void* const portrait = vcall<void*, void*, U32>(member, 0x148, ref1, 1u);
            gui_load_border_params_set_fill_00414c00(c + 0x6D8, nullptr, portrait, 0);

            // 006879BD..006879EC. kind is captured before the vcalls; the
            // original's stack reuse leaves it as SetHealth arg 3.
            const U32 kind = lw(server, 0x9E0);
            const short max_hp = static_cast<short>(
                vcall<unsigned short, U32>(server, 0x98, 1u));          // maximum_hp
            const short cur_hp = static_cast<short>(
                vcall<unsigned, U32>(server, 0x9C, 0u));                // current_hp
            kotor_fastcall_006857d0(c, nullptr, cur_hp, max_hp, static_cast<int>(kind)); // A3-G1

            // 006879F1..00687A4C: force bar. Zero max uses literal 0 for the
            // SetValue argument; only the nonzero path uses cur_fp.
            void* stats = lp(server, 0xA74);
            const short max_fp = static_cast<short>(w1x_creature_max_fp_004fd490(server, nullptr));
            const unsigned short fp_sum = static_cast<unsigned short>(
                static_cast<unsigned short>(lh(stats, 0x124)) +
                static_cast<unsigned short>(lh(stats, 0x126)));
            const short cur_fp = static_cast<short>(fp_sum);
            gui_m48_progress_set_max_00417f90(
                c + 0x8F8, nullptr, max_fp == 0 ? 1 : static_cast<int>(max_fp));
            kotor_original::kotor_fastcall_00417fb0(
                c + 0x8F8, nullptr, max_fp == 0 ? 0 : static_cast<int>(cur_fp));
            sw(c, 0x08, static_cast<U32>(cur_fp));
            sw(c, 0x0C, static_cast<U32>(max_fp));
            sw(c, 0xEA4, 1u);

            // 00687A52..00687A62: level, zero-extended byte.
            stats = lp(server, 0xA74);
            sw(c, 0x10, static_cast<U32>(recovered_stats_level_005a5fd0(stats, nullptr, 0)));
            sw(c, 0xEA4, 1u);

            // 00687A6B..00687A93: XP-to-next. The positive path calls
            // GetExpNeededForLevelUp twice, re-loading stats and stats+0x68.
            I32 next_xp = 0;
            {
                stats = lp(server, 0xA74);
                const I32 base1 = static_cast<I32>(lw(stats, 0x68));
                const I32 e1 = static_cast<I32>(kotor_fastcall_005a6610(stats, nullptr)); // A3-G2
                const I32 d1 = e1 - base1;
                if (d1 > 0) {
                    stats = lp(server, 0xA74);
                    const I32 base2 = static_cast<I32>(lw(stats, 0x68));
                    const I32 e2 = static_cast<I32>(kotor_fastcall_005a6610(stats, nullptr));
                    next_xp = e2 - base2;
                }
            }
            sw(c, 0x14, static_cast<U32>(next_xp));
            sw(c, 0xEA4, 1u);

            // 00687A96..00687B77: state machine.
            if (vcall<U32>(server, 0x94) != 0) {                        // creature_dead
                state = 1;
            } else if (recovered_creature_incapacitated_004ef890(server, nullptr) != 0) {
                state = 1;                                             // GetDeadTemp
            } else {
                stats = lp(server, 0xA74);
                if (recovered_can_level_up_005a6810(stats, nullptr) != 0) {
                    state = 4;                                         // 00687AD7
                    if ((lb(member, 0x440) & 2u) != 0) {                // 00687AD0 byte test
                        void* const in_game = pt(frame_R4_005ed690(client, nullptr));
                        kotor_fastcall_0062eeb0(in_game, nullptr, static_cast<U8>(9), 0u); // 00687AF6
                        sw(member, 0x440, lw(member, 0x440) & ~2u);      // 00687B08 dword clear
                    }
                } else if (kotor_fastcall_005b4880(server, nullptr) != 0) {
                    state = 2;                                         // 00687B77
                    if (i == 0)
                        kotor_fastcall_00687700(self, nullptr, 0xBB2Bu); // 00687B31
                } else {
                    const I32 count = li(server, 0x8F8);               // 00687B38
                    if (count > 0) {
                        U8* const entries = static_cast<U8*>(lp(server, 0x8F4));
                        for (I32 j = 0; j < count; ++j) {
                            void* const entry = lp(entries, static_cast<U32>(j) * 4u);
                            if (lw(entry, 0x1C) != 0) {
                                if (lw(entry, 0x14) == 0) ++counter_a;
                                else ++counter_b;
                            }
                        }
                    }
                }
            }

            // 00687B7C..00687C10: level-up message gate.
            if (kotor_fastcall_005b4880(server, nullptr) == 0) {
                if (lw(self, 0x7728) == 0xBB2Bu && lw(self, 0x7724) == 0xBF800000u) {
                    U8* const button = static_cast<U8*>(self) + 0x7444;
                    gui_load_text_params_set_color_00414e10(
                        button, nullptr, kotor_m15d_color_007a23d8);   // 00687BB4
                    sw(self, 0x7728, 0xBC50u);                          // 00687BBD
                    U32 text_storage[2];                                // raw, no ctor
                    kotor_fastcall_005edeb0(client, nullptr, text_storage, 0xBC50u); // 00687BD9
                    TempStringGuard text{ text_storage };              // SEH state at 00687BE5
                    gui_load_text_params_set_text_00415e00(button, nullptr, text_storage); // 00687BEA
                    sf(self, 0x746C, 1.0f);                             // 00687BF3
                    // text dtor at scope exit (00687C06)
                }
            }

            // 00687C10..00687C23: both Show calls use (1,state,counter_b,counter_a).
            kotor_fastcall_006867f0(
                c, nullptr, 1, static_cast<int>(state),
                static_cast<int>(counter_b), static_cast<int>(counter_a)); // A3-G3

            // 00687C28..00687D85: fourth char, only timer>0 and slot==0.
            if (!jcc_jne(lf(self, 0x5A28), 0.0, 0x41) && slot == 0) {
                U8* const c4 = static_cast<U8*>(self) + 0x4B80u;

                async_assign_string_005e5c50(
                    c4 + 0x18, nullptr,
                    static_cast<U8*>(lp(member, 0x2F8)) + 0x14);   // 00687C61
                async_assign_string_005e5c50(
                    c4 + 0x20, nullptr,
                    static_cast<U8*>(lp(member, 0x2F8)) + 0x1C);   // 00687C76

                alignas(4) unsigned char ref2[16];                  // [esp+0x58], 00687C80
                void* const portrait4 = vcall<void*, void*, U32>(member, 0x148, ref2, 1u);
                gui_load_border_params_set_fill_00414c00(
                    c4 + 0x6D8, nullptr, portrait4, 0);             // 00687C96

                const U32 kind4 = lw(server, 0x9E0);                // 00687C9B
                const short max_hp4 = static_cast<short>(
                    vcall<unsigned short, U32>(server, 0x98, 1u));
                const short cur_hp4 = static_cast<short>(
                    vcall<unsigned, U32>(server, 0x9C, 0u));
                kotor_fastcall_006857d0(
                    c4, nullptr, cur_hp4, max_hp4, static_cast<int>(kind4));

                void* stats4 = lp(server, 0xA74);                   // 00687CCF
                const short max_fp4 = static_cast<short>(
                    w1x_creature_max_fp_004fd490(server, nullptr));
                const unsigned short fp_sum4 = static_cast<unsigned short>(
                    static_cast<unsigned short>(lh(stats4, 0x124)) +
                    static_cast<unsigned short>(lh(stats4, 0x126)));
                const short cur_fp4 = static_cast<short>(fp_sum4);
                gui_m48_progress_set_max_00417f90(
                    c4 + 0x8F8, nullptr, max_fp4 == 0 ? 1 : static_cast<int>(max_fp4));
                kotor_original::kotor_fastcall_00417fb0(
                    c4 + 0x8F8, nullptr, max_fp4 == 0 ? 0 : static_cast<int>(cur_fp4));
                sw(c4, 0x08, static_cast<U32>(cur_fp4));
                sw(c4, 0x0C, static_cast<U32>(max_fp4));
                sw(c4, 0xEA4, 1u);

                stats4 = lp(server, 0xA74);
                sw(c4, 0x10, static_cast<U32>(
                    recovered_stats_level_005a5fd0(stats4, nullptr, 0)));
                sw(c4, 0xEA4, 1u);

                I32 next_xp4 = 0;
                {
                    stats4 = lp(server, 0xA74);
                    const I32 base1 = static_cast<I32>(lw(stats4, 0x68));
                    const I32 e1 = static_cast<I32>(kotor_fastcall_005a6610(stats4, nullptr));
                    const I32 d1 = e1 - base1;
                    if (d1 > 0) {
                        stats4 = lp(server, 0xA74);
                        const I32 base2 = static_cast<I32>(lw(stats4, 0x68));
                        const I32 e2 = static_cast<I32>(kotor_fastcall_005a6610(stats4, nullptr));
                        next_xp4 = e2 - base2;
                    }
                }
                sw(c4, 0x14, static_cast<U32>(next_xp4));
                sw(c4, 0xEA4, 1u);

                kotor_fastcall_006867f0(
                    c4, nullptr, 1, static_cast<int>(state),
                    static_cast<int>(counter_b), static_cast<int>(counter_a));
            }
        }

        // 00687D93..00687E00: hide the loop-index char, not the mapped slot.
        if (show == 0) {
            U8* const c = static_cast<U8*>(self) + static_cast<U32>(i) * 0xEA8u + 0x1F88u;
            const U32 v = lw(self, static_cast<U32>(i) * 0xEA8u + 0x2634u);
            sw(c, 0x6AC, v & ~2u);
            sw(c, 0x6C, lw(c, 0x6C) & ~2u);
            sw(c, 0x7EC, lw(c, 0x7EC) & ~2u);
            sw(c, 0x93C, lw(c, 0x93C) & ~2u);
            sw(c, 0x1AC, lw(c, 0x1AC) & ~2u);
            sw(c, 0x2EC, lw(c, 0x2EC) & ~2u);
            sw(c, 0x42C, lw(c, 0x42C) & ~2u);
            sw(c, 0x56C, lw(c, 0x56C) & ~2u);
            sb(c, 0xEA0, 0);
            sb(c, 0xEA1, 0);
            sw(c, 0xD0C, lw(c, 0xD0C) & ~2u);
        }
    }
}
