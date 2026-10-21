// Lane W4 G13: per-frame client creature leaves (fade, conjuring visual timer, ground tilt, rolling footstep
// sound, equipped item hiding) and CExoLinkedListInternal::GetPrev.
#include "a5e2cda8f5-creature.hpp"
#include "b1751895fd-w4_conjure.h"
extern "C" menu_resources::Ref* __fastcall c2da_default_ref_00405ed0(menu_resources::Ref*, void*);
extern "C" void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void*, void*, std::int32_t);
namespace resource_ref_assignment {
menu_resources::Ref* __fastcall assign_00406120(menu_resources::Ref*, void*, menu_resources::Ref*, const menu_resources::Ref*);
}


using namespace w4;
using namespace w4c;

namespace {
U8* B(void* p) { return static_cast<U8*>(p); }
double u32_to_x87(U32 v) { return static_cast<double>(v); }
}

// 005E9BD0 CExoLinkedListInternal::GetPrev(POSITION*) (thiscall on the list; ECX unused; RET 4).
namespace kotor_menu20 { struct Debug; }
namespace base_ini_reconstruction { void __fastcall warning_005e9400(kotor_menu20::Debug*, void*, int, const char*, const char*); }
namespace {
const char exolist_file_0074e208[] = "exolinkedlistinternal.cpp";                                // .rdata 0074E208
const char getprev_null_0074e2e0[] = "CExoLinkedListInternal::GetPreviousg() Position paramter is NULL"; // .rdata 0074E2E0 (original spelling)
}
extern "C" U32 __fastcall kotor_fastcall_005e9bd0(void*, void*, void* position) {
    void* node = lp(position, 0);                                                            // 005E9BD0..005E9BD4
    if (node == nullptr) {                                                                   // 005E9BD6..005E9BD8
        base_ini_reconstruction::warning_005e9400(static_cast<kotor_menu20::Debug*>(lp(menu_continuation::exo_base_007a39e0, 8)), nullptr,
                                                  0x119, exolist_file_0074e208, getprev_null_0074e2e0); // 005E9BDA..005E9BF1
        return 0;                                                                            // 005E9BF6
    }
    void* prev = lp(node, 0);                                                                // 005E9BFB
    sw(position, 0, ad(prev));                                                               // 005E9C01 / 005E9C06
    return prev ? lw(prev, 8) : 0;                                                           // 005E9BFD..005E9C08 (eax = 0 when prev == 0)
}

// 006A0920 CSWCVisualEffectOnObject::GetHardCodedEffectClass() (thiscall, RET 0; AL).
extern "C" U8 __fastcall kotor_fastcall_006a0920(void* effect, void*) {
    return lb(effect, 0xD9);                                                                 // 006A0920
}

// 0063DE50 CSWCObject::GetVisualEffectAlpha() (thiscall, RET 0; float in ST0).
extern "C" float __fastcall kotor_fastcall_0063de50(void* o, void*) {
    void* list = lp(o, 0x6C);                                                                // 0063DE58
    U32 position = lw(list, 4);                                                              // 0063DE5B..0063DE5E [esp+10h]
    void* head = lp(list, 4);                                                                // 0063DE62
    U8 best = 0xFF;                                                                          // bl
    float alpha = -1.0f;                                                                     // [esp+0Ch]
    void* effect = head ? lp(head, 8) : nullptr;                                             // 0063DE68..0063DE79
    while (position != 0) {                                                                  // 0063DE77..0063DECA
        if (kotor_fastcall_006a0920(effect, nullptr) == 0x0D && lw(effect, 0xC0) == 0 && lw(effect, 0xE0) != 0) { // 0063DE80..0063DE9D
            const U8 priority = lb(effect, 0xD8);                                            // 0063DE9F
            if (priority < best) { alpha = lf(effect, 0xCC); best = priority; }              // 0063DEA5..0063DEB3
        }
        effect = pt(kotor_fastcall_005e9bd0(lp(o, 0x6C), nullptr, &position));               // 0063DEB5..0063DEC2
    }
    return alpha;                                                                            // 0063DECC
}

// 0063DEE0 CSWCObject::GetFinalFadeAlpha() (thiscall, RET 0; float in ST0, always an exact float value).
extern "C" float __fastcall kotor_fastcall_0063dee0(void* o, void*) {
    if (vcall<void*>(o, 0x2C) != nullptr) {                                                  // 0063DEE5..0063DEEC
        void* player = client_get_player_creature_005ed540(client_app(), nullptr);           // 0063DEEE..0063DEF7
        if (player == nullptr) goto visual;                                                  // 0063DEFC..0063DEFE
        const I32 count = li(player, 0x364);                                                 // 0063DF00
        for (I32 i = 0; i < count; ++i)                                                      // 0063DF06..0063DF21
            if (lw(lp(lp(player, 0x360), static_cast<U32>(i) * 4), 0) == lw(o, 4)) return 1.0f; // 0063DF15..0063DF19 -> 0063DF4C
    }
    if (lb(o, 0x91) != 1) return 0.0f;                                                       // 0063DF23..0063DF2C
visual: {
        const float v = kotor_fastcall_0063de50(o, nullptr);                                 // 0063DF36..0063DF38
        if (jcc_jp(v, -1.0f, 0x44)) return v;                                                // 0063DF3D..0063DF48
        return 1.0f;                                                                         // 0063DF4C
    }
}

// 0063DF60 CSWCObject::FadeObject() (thiscall, RET 0). Returns 1 when a timed fade-out delete is due.
extern "C" U32 __fastcall kotor_fastcall_0063df60(void* o, void*) {
    if (lb(o, 0x90) == lb(o, 0x91)) {                                                        // 0063DF66..0063DF73
        if (lw(o, 0x94) == 0) return 0;                                                      // 0063E196..0063E19E
        U32 d = 0, t = 0, out_day = 0, out_tick = 0;                                         // [esp+0Ch],[esp+10h],[esp+14h],[esp+8]
        frame_GetWorldTime_004ade40(frame_GetWorldTimer_005ed4f0(client_app(), nullptr), nullptr, &d, &t); // 0063E1A0..0063E1BA
        recovered_world_time_difference_004ae460(frame_GetWorldTimer_005ed4f0(client_app(), nullptr), nullptr, d, t,
                                                 lw(o, 0x80), lw(o, 0x84), &out_day, &out_tick); // 0063E1BF..0063E1F1
        return out_tick < lw(o, 0x8C) + lw(o, 0x88) ? 0u : 1u;                               // 0063E1F6..0063E216
    }
    const U64 diff = static_cast<U64>(static_cast<menu_continuation::WorldTimer*>(
        frame_GetWorldTimer_005ed4f0(client_app(), nullptr))->GetSnapshotTimeDifference_004adc80()); // 0063DF79..0063DF89
    const U32 ms = static_cast<U32>(diff / 1000u);                                           // 0063DF8E..0063DF9C __aulldiv
    const U8 target = lb(o, 0x91);                                                           // 0063DF9E
    if (target == 1) {                                                                       // 0063DFA4..0063DFA6
        if (jcc_jp(kotor_fastcall_0063dee0(o, nullptr), lf(o, 0x98), 0x41)) {               // 0063DFA8..0063DFBA
            sb(o, 0x90, 2);                                                                  // 0063DFE7
            sf(o, 0x98, static_cast<float>(u32_to_x87(ms) * static_cast<double>(0.0005f) + lf(o, 0x98))); // 0063DFCF..0063DFEE, 0063E131
        } else {
            const float final_alpha = kotor_fastcall_0063dee0(o, nullptr);                  // 0063DFBC..0063DFBE
            sb(o, 0x90, 1);                                                                  // 0063DFC3
            sf(o, 0x98, final_alpha);                                                        // 0063E131
        }
    } else if (target == 0) {                                                                // 0063DFF9..0063DFFB
        if (lw(o, 0x8C) != 0) {                                                              // 0063E001..0063E009
            U32 d = 0, t = 0, out_day = 0, out_tick = 0;                                     // [esp+0Ch],[esp+8],[esp+14h],[esp+10h]
            frame_GetWorldTime_004ade40(frame_GetWorldTimer_005ed4f0(client_app(), nullptr), nullptr, &d, &t); // 0063E00B..0063E025
            recovered_world_time_difference_004ae460(frame_GetWorldTimer_005ed4f0(client_app(), nullptr), nullptr, d, t,
                                                     lw(o, 0x80), lw(o, 0x84), &out_day, &out_tick); // 0063E02A..0063E05B
            if (out_tick <= lw(o, 0x8C)) return 0;                                           // 0063E060..0063E06A
        }
        if (void* creature = vcall<void*>(o, 0x2C)) {                                        // 0063E070..0063E079
            if (lw(creature, 0x3A4) != 0) kotor_fastcall_0060ef50(creature, nullptr, 0);     // 0063E07B..0063E089
        }
        if (jcc_jne(kotor_fastcall_0063dee0(o, nullptr), lf(o, 0x98), 0x01)) {              // 0063E08E..0063E0A0
            sb(o, 0x90, 3);                                                                  // 0063E124
            sf(o, 0x98, static_cast<float>(static_cast<double>(lf(o, 0x98)) - u32_to_x87(ms) * static_cast<double>(0.0005f))); // 0063E10C..0063E131
        } else {
            sf(o, 0x98, kotor_fastcall_0063dee0(o, nullptr));                                // 0063E0A2..0063E0A9
            sb(o, 0x90, 0);                                                                  // 0063E0B7
            if (lw(o, 0x94) != 0) {                                                          // 0063E0AF..0063E0BE
                frame_GetWorldTime_004ade40(frame_GetWorldTimer_005ed4f0(client_app(), nullptr), nullptr,
                                            reinterpret_cast<U32*>(B(o) + 0x80), reinterpret_cast<U32*>(B(o) + 0x84)); // 0063E0C0..0063E0DD
                vcall<void, U32>(lp(o, 0x68), 0x04, 0);                                      // 0063E0E2..0063E0E9
            }
            if (void* creature = vcall<void*>(o, 0x2C)) {                                    // 0063E0EC..0063E0F5
                if (lw(creature, 0x3A4) != 0) kotor_fastcall_0060ef50(creature, nullptr, 0); // 0063E0F7..0063E105
            }
        }
    }
    // 0063E137..0063E18E: clamp to [0,1] and push the alpha to the anim base.
    if (!jcc_jp(lf(o, 0x98), 0.0f, 0x05)) sw(o, 0x98, 0);                                    // 0063E137..0063E14A
    else if (!jcc_jne(lf(o, 0x98), 1.0f, 0x41)) sf(o, 0x98, 1.0f);                           // 0063E156..0063E169
    if (void* base = lp(o, 0x68)) vcall<void, U32, U32>(base, 0x94, lw(o, 0x98), 0xFF);      // 0063E173..0063E188
    return 0;
}

// 00695E10 CSWCSpellCastVisual::UpdateTimer(delta) (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_00695e10(void* v, void*, U32 delta) {
    for (U32 i = 0; i < 3; ++i) {                                                            // 00695E13..00695E7E
        if (lw(v, 0x24 + 4 * i) != 0 && lp(v, 4 * i) != nullptr) {
            vcall<void, U32, U32, U32>(lp(v, 4 * i), 0x50, 0, 0, 0);
            if (void* effect = lp(v, 4 * i)) vcall<void, U32>(effect, 0x00, 1);
            sw(v, 4 * i, 0);
        }
    }
    if (lw(v, 0x20) == 0) return;                                                            // 00695E81..00695E84
    const U32 conjure = lw(v, 0x18);                                                         // 00695E86
    if (conjure == 0 && lw(v, 0x1C) == 0) { sw(v, 0x20, 0); return; }                        // 00695E89..00695E90, 00695ED0
    if (conjure != 0) {                                                                      // 00695E92..00695E94
        if (conjure > delta) { sw(v, 0x18, conjure - delta); return; }                       // 00695E96..00695EB0
        kotor_m15e_spell_cast_visual_unload_conjure_visual_00695d10(v, nullptr);             // 00695E9E..00695EA0
        sw(v, 0x18, 0);                                                                      // 00695EA5
        return;
    }
    const U32 cast = lw(v, 0x1C);                                                            // 00695EB7
    if (cast == 0) return;                                                                   // 00695EBA..00695EBC
    if (cast > delta) { sw(v, 0x1C, cast - delta); return; }                                 // 00695EBE..00695EDA
    kotor_m15e_spell_cast_visual_unload_cast_visual_00695d90(v, nullptr);                    // 00695EC6..00695EC8
    sw(v, 0x1C, 0);                                                                          // 00695ECD
    sw(v, 0x20, 0);                                                                          // 00695ED0
}

// 0060BB20: HandleConjuringCreature, RET0.
extern "C" void __fastcall kotor_fastcall_0060bb20(void* c, void*) {
    U32 active = 0;
    void* row = nullptr;
    switch (lh(c, 0x48)) {
    case 0x271F: case 0x2720: case 0x2AF8: case 0x2721: case 0x2722: case 0x2723: case 0x2724:
    case 0x274D: case 0x280A: case 0x280B: case 0x280C: case 0x280D: case 0x2815: case 0x28A1: {
        const I32 spell = li(c, 0x2C4);
        row = kotor_m15b_spell_array_get_spell_0059b6d0(lp(kotor_cell_rules_007a3a28, 0x8C), nullptr, spell);
        active = 1;
        break;
    }
    default: break;
    }
    // The spell lookup can change the animation; original reloads its WORD.
    switch (lh(c, 0x48)) {
    case 0x2721: case 0x2722: case 0x2723: case 0x2724: case 0x274D:
    case 0x280A: case 0x280B: case 0x280C: case 0x280D: case 0x2815: case 0x28A1:
        if (lw(c, 0x2C0) == 0) {
            void* visual = lp(c, 0x2F0);
            sw(visual, 0x18, 5000);
            sw(visual, 0x1C, 5000);
            sw(lp(c, 0x2F0), 0x20, 0);
            sw(c, 0x2C0, 1);
        }
        break;
    default: break;
    }
    if (active == 1) {
        if (lw(c, 0x2C0) == 0 && row != nullptr) {
            menu_resources::Ref first, second, discarded;
            c2da_default_ref_00405ed0(&first, nullptr);
            unresolved_ref_assign_00406290(&first, nullptr, &discarded, "");
            const U32 appearance = lh(lp(c, 0x2F8), 0x24);
            if (lw(lp(kotor_cell_rules_007a3a28, 0xB0), appearance * 0x34u + 0x20u) != 0) {
                const U32 offset = lb(lp(c, 0x21C), 0x1B) == 1 ? 0x86u : 0x76u;
                resource_ref_assignment::assign_00406120(&first, nullptr, &discarded,
                    static_cast<const menu_resources::Ref*>(static_cast<const void*>(B(row) + offset)));
            }
            c2da_default_ref_00405ed0(&second, nullptr);
            resource_ref_assignment::assign_00406120(&second, nullptr, &discarded,
                static_cast<const menu_resources::Ref*>(static_cast<const void*>(B(row) + 0x96)));
            kotor_m15e_spell_cast_visual_unload_conjure_visual_00695d10(lp(c, 0x2F0), nullptr);
            kotor_m15e_spell_cast_visual_unload_cast_visual_00695d90(lp(c, 0x2F0), nullptr);
            menu_resources::Ref head, hand, ground;
            std::memcpy(&ground, B(row) + 0x66, 16);
            std::memcpy(&hand, B(row) + 0x56, 16);
            std::memcpy(&head, B(row) + 0x46, 16);
            recovered_spell_load_conjure_00695f30(lp(c, 0x2F0), nullptr, head, hand, ground, first, second, c);
            void* visual = lp(c, 0x2F0);
            sw(visual, 0x18, 5000);
            sw(visual, 0x1C, 5000);
            sw(lp(c, 0x2F0), 0x20, 0);
        }
    } else if (lw(c, 0x2C0) == 1) {
        void* visual = lp(c, 0x2F0);
        if (lw(visual, 0x20) == 0) {
            kotor_m15e_spell_cast_visual_unload_conjure_visual_00695d10(visual, nullptr);
            kotor_m15e_spell_cast_visual_unload_cast_visual_00695d90(lp(c, 0x2F0), nullptr);
        }
        sw(c, 0x2C4, 0xFFFFFFFFu);
    }
    const U32 delta = lw(c, 0x44);
    kotor_fastcall_00695e10(lp(c, 0x2F0), nullptr, delta);
    sw(c, 0x2C0, active);
}

// 0069DE90 CSWCAnimBase::SetGroundTilt(Vector* normal, Vector* orientation) (thiscall, RET 8).
extern "C" void __fastcall kotor_fastcall_0069de90(void* ab, void*, W4Vector* normal, W4Vector* orientation) {
    if (lw(ab, 0xB8) == 0) return;                                                           // 0069DE97..0069DE9F
    const float tilt_deg = static_cast<float>(static_cast<double>(90.0f) - pitch_st0(normal)); // 0069DEA5..0069DEBF (arg slot)
    W4Vector axis;                                                                           // [esp+8]
    vector_cross_004ab210(&axis, reinterpret_cast<const W4Vector*>(&kotor_world_los::up_007a19b0), normal); // 0069DEB5..0069DEC4
    const float radians = static_cast<float>(static_cast<double>(tilt_deg) * static_cast<double>(bits_f(0x3C8EFA35u))); // 0069DEC9..0069DEDD ([007415F8] exact bits)
    W4Quaternion target;                                                                     // [esp+14h]
    quaternion_from_axis_angle_004ac960(&target, nullptr, axis, radians);                    // 0069DEED..0069DEF1
    W4Quaternion halfway;                                                                    // [esp+34h]
    quaternion_slerp_004a9e00(&halfway, target, reinterpret_cast<W4Quaternion*>(static_cast<U8*>(ab) + 0xE0), 0.5f); // 0069DEF6..0069DF27
    std::memcpy(static_cast<U8*>(ab) + 0xE0, &target, 16);                                  // 0069DF2C..0069DF50 (stores the target, not the slerp)
    const float yaw = static_cast<float>(yaw_st0(orientation));                              // 0069DF44..0069DF58
    W4Quaternion heading;                                                                    // [esp+44h]
    yaw_pitch_roll_004acac0(&heading, yaw, 0.0f, 0.0f);                                      // 0069DF5B..0069DF60
    W4Quaternion combined;                                                                   // [esp+24h]
    quaternion_multiply_004a9ac0(&combined, &halfway, &heading);                             // 0069DF65..0069DF70
    vcall<void, W4Quaternion*, W4Quaternion>(lp(ab, 0xB8), 0x60, &heading, combined);       // 0069DF75..0069DFA2 Gob::SetOrientation
}

// 006107C0 CSWCCreature::PlayRollingFootstepSound(int run) (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_006107c0(void* c, void*, U32 run) {
    if (lw(c, 0x430) == 0) return;                                                           // 006107C3..006107CB
    kotor_m15_sound_source_set_position_005d59e0(lp(c, 0x430), nullptr, B(c) + 0x24, 0.0f);  // 006107CE..006107D4
    if (lw(c, 0x438) != 0 && lw(c, 0x43C) == run) return;                                    // 006107D9..006107ED
    const float variance = run != 0 ? static_cast<float>(static_cast<double>(lf(c, 0x434)) + 1.0f) : 1.0f; // 006107EF..00610805
    kotor_fastcall_005d5a30(lp(c, 0x430), nullptr, variance);                                // 0061080A..00610810
    kotor_fastcall_005d5930(lp(c, 0x430), nullptr);                                          // 00610815..0061081B
    sw(c, 0x438, 1);                                                                         // 00610820
    sw(c, 0x43C, run);                                                                       // 0061082A
}

// 00610840 CSWCCreature::UpdateRollingFootstepSound() (thiscall, RET 0). Jump table 006108B4/006108C0.
extern "C" void __fastcall kotor_fastcall_00610840(void* c, void*) {
    if (lw(c, 0x430) == 0) return;                                                           // 00610843..0061084B
    const U32 anim = kotor_m15a_object_get_current_animation_0063c060(c, nullptr) & 0xFFFFu; // 0061084D..00610852
    switch (anim) {                                                                          // 00610855..00610868
    case 0x2712: case 0x2713: case 0x276D: case 0x2795:
        kotor_fastcall_006107c0(c, nullptr, 0); return;                                      // 0061086F..00610878
    case 0x2714: case 0x276E:
        kotor_fastcall_006107c0(c, nullptr, 1); return;                                      // 0061087A..00610883
    default: break;
    }
    if (lw(c, 0x430) != 0 && lw(c, 0x438) != 0) {                                            // 00610885..00610897
        kotor_m15_sound_source_stop_005d5a20(lp(c, 0x430), nullptr);                         // 00610899
        sw(c, 0x438, 0);                                                                     // 0061089E
        sw(c, 0x43C, 0);                                                                     // 006108A8
    }
}

// 00614630 CSWCCreature::HideEquippedItems(CResRef, CResRef) (thiscall, RET 8).
extern "C" void __fastcall kotor_fastcall_00614630(void* c, void*, void* ref_a, void* ref_b) {
    if (lw(c, 0x68) == 0) return;                                                            // 00614633..00614638
    (void)gob_get_property_0x68_0063c070(c, nullptr);                                        // 0061463A (unused)
    vcall<void, void*, void*>(lp(c, 0x68), 0x80, ref_a, ref_b);                              // 0061463F..0061464E
    sw(c, 0x3CC, 1);                                                                         // 00614654
}
