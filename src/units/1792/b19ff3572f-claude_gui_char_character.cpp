// Lane W4 GUI successor (worker "gui-char"): CSWGuiInGameCharacter (0x59F8, table 00756100) slot functions and the
// character-sheet helpers they reach that had no provider in the c24 candidate map.
//   006AE980 OnPanelRemoved        thiscall() RET            (slot)
//   006B0B90 Draw                  thiscall(float) RET 4     (slot)
//   006B2220 OnPanelAdded          thiscall() RET            (slot)
//   006B2250 HandleInputEvent      thiscall(int, int) RET 8  (slot)
// helpers reconstructed here:
//   006AFDA0 SetStats                         thiscall() RET
//   006AE990 SetModText(label*, int)          thiscall RET 8
//   006AECD0 SetLabelBonusPenaltyColour(label*, int)  thiscall RET 8
//   006AED20 RotateCharacter(int)             thiscall RET 4
//   006B0BB0 ShowLevelUpGUI(int)              thiscall RET 4, returns int
//   006B0CB0 StartAutoLevelup                 thiscall() RET
//   006AEC80 DoAutoLevelup(int)               thiscall RET 4 (message-box callback; tail call AutoLevelup(0))
//   006E9CA0 CSWGuiScriptSelect::SetCreatureID(ulong)  thiscall RET 4
//   005EE190 CClientExoApp::GetCharacterChangeInProgress  thiscall() RET
//   004F7780 CSWSCreature::GetTotalACEffectBonus          thiscall() RET
// Transcribed from the original swkotor.exe linear sweep; each statement carries its original PCs.
#include "6030a9ee65-claude_gui_char.hpp"

namespace kotor_menu_10_19 { struct String; struct Tlk { String* __thiscall simple_0041e8f0(String* out, unsigned strref); }; }
namespace recovered_save_stats { unsigned short __cdecl maximum_hp_00647a80(void* stats); }       // CSWCCreatureStats::GetMaxHitPoints
namespace kotor { namespace server_mainloop { unsigned __cdecl creature_force_class_005be4a0(unsigned cls); } } // CSWClass::IsJedi (static)

extern "C" {
// ---- creature stats (server)
w4::U8 __fastcall recovered_creature_stats_str_005a6190(void* stats, void*);                    // GetSTRStat
w4::U8 __fastcall recovered_creature_stats_con_005a6250(void* stats, void*);                    // GetCONStat
w4::U8 __fastcall recovered_creature_stats_int_005a6310(void* stats, void*);                    // GetINTStat
w4::U8 __fastcall recovered_creature_stats_wis_005a63d0(void* stats, void*);                    // GetWISStat
w4::U8 __fastcall recovered_creature_stats_cha_005a6490(void* stats, void*);                    // GetCHAStat
w4::U8 __fastcall recovered_creature_stats_dex_005a6550(void* stats, void*);                    // GetDEXStat
w4::U32 __fastcall kotor_fastcall_005a5330(void* stats, void*, w4::U32 flag);                    // GetDEXMod (AL)
w4::U32 __fastcall kotor_fastcall_005ab810(void* stats, void*, w4::U32 flag);                    // GetFortSavingThrow (AL)
w4::U32 __fastcall kotor_fastcall_005ab880(void* stats, void*, w4::U32 flag);                    // GetWillSavingThrow (AL)
unsigned __fastcall recovered_stats_reflex_save_005ab8f0(void* stats, void*, unsigned flag);    // GetReflexSavingThrow (AL)
w4::U32 __fastcall kotor_fastcall_005a6610(void* stats, void*);                                  // GetExpNeededForLevelUp
int __fastcall kotor_fastcall_005a6810(void* stats, void*);                                      // CanLevelUp
unsigned __fastcall kotor_fastcall_004f3fe0(void*, void*, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned,
                                            unsigned, unsigned, unsigned, unsigned);             // CSWSCreature::GetTotalEffectBonus
unsigned __fastcall recovered_effect_integer_00503690(void* effect, void*, unsigned index);     // CGameEffect::GetInteger
// ---- creature stats (client)
w4::U8 __fastcall kotor_m15b_creature_stats_get_class_00647730(void* stats, void*, w4::U32 index); // CSWCCreatureStats::GetClass
w4::U8 __fastcall kotor_m15b_creature_stats_get_class_level_00647750(void* stats, void*, w4::U32 index); // GetClassLevel
w4::U16 __fastcall kotor_m15b_creature_stats_get_armor_class_00647720(void* stats, void*, w4::U32 flag); // GetArmorClass
std::int32_t __fastcall kotor_m15b_levelup_stats_get_level_00648430(void* self, void*);          // CSWCLevelUpStats::GetLevel (AL)
// ---- client creature / 3D preview
void __fastcall kotor_m15a_creature_set_appearance_006134c0(void* creature, void*, void* app, std::uint32_t a, std::uint32_t b);
void __fastcall kotor_m15a_appearance_set_appearance_alignment_00698150(void* appearance, void*, std::uint32_t value);
void __fastcall kotor_m15a_creature_setup_3d_scene_006100f0(void* creature, void*, void* view, void* source, w4::U32, w4::U32);
void __fastcall gui_m48_slider_set_cur_00417ee0(void* slider, void*, w4::I32 value);           // CSWGuiSlider::SetCurValue
void* __cdecl yaw_pitch_roll_004acac0(void* out_quaternion, float yaw, float pitch, float roll); // YawPitchRoll
void* __cdecl quaternion_rotate_004a9bb0(void* out, const void* quaternion, const void* vector); // operator*(Quaternion, Vector)
// ---- tlk / message box
void __fastcall kotor_fastcall_0041db50(void* tlk, void*, unsigned token, void* value);         // CTlkTable::SetCustomToken
// ---- panels / helpers owned elsewhere
void __fastcall kotor_fastcall_006aee30(void* self, void*);                                      // UpdatePortraits (claude_menu_handlers_b.cpp)
// ---- not reconstructed anywhere (see report): level-up GUI constructor and CSWSCreatureStats::AutoLevelup
void* __fastcall kotor_fastcall_006e8ef0(void* self, void*, void* manager, w4::U32 creature_id, void* preview); // CSWGuiLevelUpCharGen
void __fastcall kotor_fastcall_005b27e0(void* stats, void*, w4::U32 flag);                       // CSWSCreatureStats::AutoLevelup
// ---- defined here
w4::U32 __fastcall kotor_fastcall_005ee190(void* app, void*);
w4::I32 __fastcall kotor_fastcall_004f7780(void* creature, void*);
void __fastcall kotor_fastcall_006ae990(void* self, void*, void* label, w4::I32 value);
void __fastcall kotor_fastcall_006aecd0(void* self, void*, void* label, w4::I32 value);
void __fastcall kotor_fastcall_006aed20(void* self, void*, w4::U32 left);
void __fastcall kotor_fastcall_006afda0(void* self, void*);
w4::U32 __fastcall kotor_fastcall_006b0bb0(void* self, void*, w4::U32 flag);
void __fastcall kotor_fastcall_006b0cb0(void* self, void*);
void __fastcall kotor_fastcall_006aec80(void* self, void*, w4::U32 arg);
void __fastcall kotor_fastcall_006e9ca0(void* self, void*, w4::U32 creature_id);
void __fastcall kotor_fastcall_006ae980(void* self, void*);
void __fastcall kotor_fastcall_006b0b90(void* self, void*, std::uint32_t delta);
void __fastcall kotor_fastcall_006b2220(void* self, void*);
void __fastcall kotor_fastcall_006b2250(void* self, void*, std::uint32_t event, std::uint32_t down);
}

using namespace gchar;

namespace {
// .rdata 0074673C / 00746740: effect types EFFECT_AC_INCREASE (30h) / EFFECT_AC_DECREASE (31h).
constexpr U32 k_effect_ac_increase_0074673c = 0x30;
constexpr U32 k_effect_ac_decrease_00746740 = 0x31;
const char k_empty_0073d71c[] = "";                                                              // 0073D71C
const char k_plus_00748ed4[] = "+";                                                              // 00748ED4
const char k_ratio_0075616c[] = "%d/%d";                                                         // 0075616C

inline I32 s8(U32 v) { return static_cast<std::int8_t>(static_cast<U8>(v)); }                  // movsx r32, al
inline void set_text(void* self, U32 params, void* string) { gui_load_text_params_set_text_00415e00(at(self, params), nullptr, string); }
inline void set_color(void* self, U32 params, const void* color) { gui_load_text_params_set_color_00414e10(at(self, params), nullptr, color); }
inline void text_int(void* self, U32 params, I32 value) { Str t(value); set_text(self, params, t.p()); }
inline void text_literal(void* self, U32 params, const char* value) { Str t(value); set_text(self, params, t.p()); }
// GetGUIString([[rules+ACh] + cls*1A0h]) (the class name strref).
inline void text_class(void* self, U32 params, U8 cls) {
    const U32 strref = lw(lp(rules(), 0xAC), static_cast<U32>(cls) * 0x1A0u);
    Str t; void* s = gui_string(t, strref);
    set_text(self, params, s);
}
// One ability row (006B0263..006B05BA): label text = current, mod label via SetModText, both coloured by cur-base.
using StatFn = U8(__fastcall*)(void*, void*);
void ability_row(void* self, void* sstats, U32 base_off, StatFn stat, U32 text, U32 mod_off, U32 mod_label, U32 mod_params) {
    const I32 base = lb(sstats, base_off);                                                      // movzx [sstats+base]; [esp+14]
    const I32 cur = stat(sstats, nullptr);                                                      // GetXXXStat; movzx edi, al
    text_int(self, text, cur);
    kotor_fastcall_006ae990(self, nullptr, at(self, mod_label), s8(lb(sstats, mod_off)));      // SetModText(label, movsx mod)
    const I32 diff = cur - base;
    set_color(self, text, bonus_color(diff));
    set_color(self, mod_params, bonus_color(diff));
}
// GetTotalEffectBonus(3, 0, &adjust, 0, 0, save, 0, 0xFF, 0xFF, 0) on the server creature (saving-throw bonus).
inline I32 save_bonus(void* creature, I32* adjust, U32 save) {
    void* server = kotor_m15a_creature_get_server_creature_0060fb20(creature, nullptr);
    return static_cast<I32>(kotor_fastcall_004f3fe0(server, nullptr, 3, 0, ad(adjust), 0, 0, save, 0, 0xFF, 0xFF, 0));
}
}  // namespace

extern "C" {
// 005EE190 CClientExoApp::GetCharacterChangeInProgress: return [[this+4]+4B4h].
w4::U32 __fastcall kotor_fastcall_005ee190(void* app, void*) {
    return lw(lp(app, 4), 0x4B4);                                                               // 005EE190..005EE199
}

// 004F7780 CSWSCreature::GetTotalACEffectBonus: sum of AC increase minus AC decrease applied effects (+124h list).
w4::I32 __fastcall kotor_fastcall_004f7780(void* creature, void*) {
    void* list = at(creature, 0x124);                                                           // 004F7781 lea edi,[ecx+124h]
    if (list == nullptr) return 0;                                                              // 004F7787..004F778E
    I32 total = 0;                                                                              // 004F7793
    if (li(list, 4) > 0) {                                                                      // 004F778F..004F7797
        for (I32 i = 0; i < li(list, 4); ++i) {                                                 // 004F779A..004F77D3 (size re-read)
            void* effect = lp(lp(list, 0), static_cast<U32>(i) * 4u);
            const U32 type = lh(effect, 8);                                                     // movzx word [effect+8]
            if (type == k_effect_ac_increase_0074673c) total += static_cast<I32>(recovered_effect_integer_00503690(effect, nullptr, 1));
            else if (type == k_effect_ac_decrease_00746740) total -= static_cast<I32>(recovered_effect_integer_00503690(effect, nullptr, 1));
        }
    }
    return total;                                                                               // 004F77D6
}

// 006AE990 CSWGuiInGameCharacter::SetModText(CSWGuiLabel* label, int mod) RET 8: "+N" for positive, else "N".
void __fastcall kotor_fastcall_006ae990(void*, void*, void* label, w4::I32 value) {
    if (value > 0) {                                                                            // 006AE9AC..006AE9B8 (jle)
        Str number(value);                                                                      // 006AE9BA..006AE9BE [esp+4]  flag 1
        Str plus(k_plus_00748ed4);                                                              // 006AE9C3..006AE9DC [esp+1C] flag 2
        Str joined;                                                                             // [esp+14] flag 4
        void* r = GuiConcat005e5d10(plus.ss(), nullptr, joined.ss(),
                                    static_cast<kotor_reconstruction::StartupString*>(number.p())); // 006AE9E1..006AE9FD
        joined.live = true;
        gui_load_text_params_set_text_00415e00(at(label, 0xE8), nullptr, r);                    // 006AEA27..006AEA36
    } else {                                                                                    // destroyed: joined, plus, number
        Str number(value);                                                                      // 006AEA11..006AEA15 [esp+10] flag 8
        gui_load_text_params_set_text_00415e00(at(label, 0xE8), nullptr, number.p());           // 006AEA27..006AEA36
    }
}

// 006AECD0 CSWGuiInGameCharacter::SetLabelBonusPenaltyColour(CSWGuiLabel* label, int value) RET 8.
void __fastcall kotor_fastcall_006aecd0(void*, void*, void* label, w4::I32 value) {
    gui_load_text_params_set_color_00414e10(at(label, 0xE8), nullptr, bonus_color(value));      // 006AECD0..006AED1C
}

// 006AED20 CSWGuiInGameCharacter::RotateCharacter(int left) RET 4: rotate the preview by -/+10 degrees of yaw.
void __fastcall kotor_fastcall_006aed20(void* self, void*, w4::U32 left) {
    const float yaw = left != 0 ? bits_f(0xC1200000u) : bits_f(0x41200000u);                  // 006AED23..006AED37 (-10 / 10)
    void* preview = lp(self, 0x59E4);                                                          // 006AED3F
    unsigned char quaternion[16], rotated[12];
    void* q = yaw_pitch_roll_004acac0(quaternion, yaw, 0.0f, 0.0f);                            // 006AED4A..006AED5E
    void* v = quaternion_rotate_004a9bb0(rotated, q, at(preview, 0x30));                       // 006AED45..006AED72
    vcall<void, void*>(lp(self, 0x59E4), 0x88, v);                                             // 006AED6C..006AED76 [vtbl+88h]
}

// 006AFDA0 CSWGuiInGameCharacter::SetStats() RET 0.
void __fastcall kotor_fastcall_006afda0(void* self, void*) {
    void* client = client_app();                                                               // 006AFDC7..006AFDCC esi
    sw(self, 0x59E0, lw(kotor_fastcall_005ed8b0(client, nullptr), 0));                         // 006AFDCF..006AFDD8
    void* creature;
    const U8 npc = lb(self, 0x59F0);
    if ((lb(recovered_server_info_004aec90(server_app(), nullptr), 0x104) & 1) != 0 && npc != 0xFF) { // 006AFDDE..006AFE02
        const U32 object = recovered_npc_object_00564700(party_table(), nullptr, s8(npc), 0, 1); // 006AFE04..006AFE1C
        creature = pt(frame_R4_005ed5b0(client, nullptr, kotor_fastcall_005eda50(client, nullptr, object))); // 006AFE21..006AFE2C
    } else {
        creature = frame_GetCharacter_006346c0(kotor_fastcall_005ed8b0(client, nullptr), nullptr, 0); // 006AFE33..006AFE3E
    }
    void* stats = lp(creature, 0x2F8);                                                         // 006AFE45 ebx ([esp+2C])
    void* server = kotor_m15a_creature_get_server_creature_0060fb20(creature, nullptr);        // 006AFE53 ([esp+1C])
    void* sstats = lp(server, 0xA74);                                                          // 006AFE58 esi
    const U8 last = static_cast<U8>(lb(stats, 0x32) - 1);                                      // 006AFE62..006AFE67
    const U8 cls = kotor_m15b_creature_stats_get_class_00647730(stats, nullptr, last);         // 006AFE6B..006AFE7D [esp+13]
    {
        Str name; void* s = vcall<void*, void*>(creature, 0x70, name.s); name.live = true;     // 006AFE77..006AFE86 [vtbl+70h]
        set_text(self, 0x14C, s);                                                              // 006AFE89..006AFE9B LBL_NAME
    }
    if (lb(stats, 0x32) == 1) {                                                                // 006AFEB0..006AFEB7
        text_class(self, 0x3CC, kotor_m15b_creature_stats_get_class_00647730(stats, nullptr, 0)); // 006AFEBD..006AFF12
        text_literal(self, 0x50C, k_empty_0073d71c);                                           // 006AFF17..006AFF4B
        text_int(self, 0x78C, kotor_m15b_creature_stats_get_class_level_00647750(stats, nullptr, 0)); // 006AFF50..006AFF8C
        text_literal(self, 0x8CC, k_empty_0073d71c);                                           // 006AFF91..006AFFAA, 006B00C7..006B00E2
    } else {
        text_class(self, 0x3CC, kotor_m15b_creature_stats_get_class_00647730(stats, nullptr, 1)); // 006AFFAF..006B0004
        text_class(self, 0x50C, kotor_m15b_creature_stats_get_class_00647730(stats, nullptr, 0)); // 006B0009..006B0060
        text_int(self, 0x78C, kotor_m15b_creature_stats_get_class_level_00647750(stats, nullptr, 1)); // 006B0065..006B00A1
        text_int(self, 0x8CC, kotor_m15b_creature_stats_get_class_level_00647750(stats, nullptr, 0)); // 006B00A6..006B00E2
    }
    Str ratio; ratio.empty();                                                                  // 006B00E7..006B00F6 [esp+30] state 9
    {
        const std::int16_t hp = static_cast<std::int16_t>(lh(stats, 0x4C));                    // 006B00F0
        const std::int16_t max = static_cast<std::int16_t>(recovered_save_stats::maximum_hp_00647a80(stats)); // 006B0101
        frame_LogFormat_005e5680(ratio.p(), k_ratio_0075616c, static_cast<I32>(hp), static_cast<I32>(max)); // 006B0106..006B011D
        set_text(self, 0x190C, ratio.p());                                                     // 006B0120..006B012B LBL_VITALITY_STAT
        set_color(self, 0x190C, bonus_color(static_cast<std::int16_t>(lh(stats, 0x4A))));      // 006B0130..006B0153
    }
    text_int(self, 0x168C, static_cast<std::int16_t>(kotor_m15b_creature_stats_get_armor_class_00647720(stats, nullptr, 0))); // 006B0158..006B0192
    set_color(self, 0x168C, bonus_color(kotor_fastcall_004f7780(kotor_m15a_creature_get_server_creature_0060fb20(creature, nullptr), nullptr))); // 006B0197..006B01C6
    if (kotor::server_mainloop::creature_force_class_005be4a0(cls) != 0) {                     // 006B01CB..006B01E0 IsJedi
        const std::int16_t force = static_cast<std::int16_t>(lh(stats, 0x122) + lh(stats, 0x124)); // 006B01E2..006B01F0
        const std::int16_t max = static_cast<std::int16_t>(lh(stats, 0x11E));
        frame_LogFormat_005e5680(ratio.p(), k_ratio_0075616c, static_cast<I32>(force), static_cast<I32>(max)); // 006B01F7..006B020E
        set_text(self, 0x17CC, ratio.p());                                                     // 006B0211..006B021C LBL_FORCE_STAT
        sw(self, 0x1AE8, lw(self, 0x1AE8) | 2u);                                               // 006B0221..006B022E
        sw(self, 0x1728, lw(self, 0x1728) | 2u);                                               // 006B0234..006B0242
        kotor_fastcall_006aecd0(self, nullptr, at(self, 0x16E4), static_cast<std::int16_t>(lh(stats, 0x124))); // 006B0245..006B0250
    } else {
        sw(self, 0x1AE8, lw(self, 0x1AE8) & 0xFFFFFFFDu);                                      // 006B0257
        sw(self, 0x1728, lw(self, 0x1728) & 0xFFFFFFFDu);                                      // 006B025D
    }
    ability_row(self, sstats, 0xE9, &recovered_creature_stats_str_005a6190, 0x1E0C, 0xEA, 0x1FA4, 0x208C); // 006B0263..006B030A STR
    ability_row(self, sstats, 0xF1, &recovered_creature_stats_wis_005a63d0, 0x21CC, 0xF2, 0x2364, 0x244C); // 006B030F..006B03B6 WIS
    ability_row(self, sstats, 0xF3, &recovered_creature_stats_cha_005a6490, 0x258C, 0xF4, 0x2724, 0x280C); // 006B03BB..006B0462 CHA
    ability_row(self, sstats, 0xEF, &recovered_creature_stats_int_005a6310, 0x294C, 0xF0, 0x2AE4, 0x2BCC); // 006B0467..006B050E INT
    ability_row(self, sstats, 0xED, &recovered_creature_stats_con_005a6250, 0x2D0C, 0xEE, 0x2EA4, 0x2F8C); // 006B0513..006B05BA CON
    {                                                                                          // DEX 006B05BF..006B067C
        const I32 base = lb(sstats, 0xEB);                                                     // 006B05BF..006B05C8 [esp+14]
        const I32 cur = recovered_creature_stats_dex_005a6550(sstats, nullptr);                // 006B05CC..006B05D9 edi, [esp+24]
        text_int(self, 0x30CC, cur);                                                           // 006B05DD..006B0606
        kotor_fastcall_006ae990(self, nullptr, at(self, 0x3264), s8(kotor_fastcall_005a5330(sstats, nullptr, 1))); // 006B060B..006B0621
        I32 diff = cur - base;                                                                 // 006B0626
        set_color(self, 0x30CC, bonus_color(diff));                                            // 006B062A..006B0649
        if (cur == base)                                                                       // 006B064E..006B0656
            diff = s8(kotor_fastcall_005a5330(sstats, nullptr, 1)) - s8(kotor_fastcall_005a5330(sstats, nullptr, 0)); // 006B0658..006B0670
        kotor_fastcall_006aecd0(self, nullptr, at(self, 0x3264), diff);                        // 006B0672..006B067C
    }
    {
        I32 adjust = 0;                                                                        // 006B0689 [esp+20]
        text_int(self, 0xDCC, s8(kotor_fastcall_005ab810(sstats, nullptr, 0)));                // 006B0685..006B06C7 fortitude
        set_color(self, 0xDCC, bonus_color(save_bonus(creature, &adjust, 1)));                 // 006B06CC..006B0716
        text_int(self, 0xF0C, s8(recovered_stats_reflex_save_005ab8f0(sstats, nullptr, 0)));   // 006B071B..006B0755 reflex
        set_color(self, 0xF0C, bonus_color(save_bonus(creature, &adjust, 2)));                 // 006B075A..006B07A4
        text_int(self, 0x104C, s8(kotor_fastcall_005ab880(sstats, nullptr, 0)));               // 006B07A9..006B07E3 will
        set_color(self, 0x104C, bonus_color(save_bonus(creature, &adjust, 3)));                // 006B07E8..006B0832
    }
    text_int(self, 0x12CC, li(sstats, 0x68));                                                  // 006B0837..006B0868 LBL_EXPERIENCE_STAT
    text_int(self, 0x154C, static_cast<I32>(kotor_fastcall_005a6610(sstats, nullptr)));        // 006B086D..006B08A2 LBL_NEEDED_XP
    {
        U32 v = kotor_fastcall_005a6610(sstats, nullptr) != 0xFFFFFFFFu ? 1u : 0u;              // 006B08A7..006B08B6
        U32 f = lw(self, 0x14A8);
        sw(self, 0x14A8, f ^ (((v + v) ^ f) & 2u));                                            // 006B08B8..006B08CC
        v = kotor_fastcall_005a6610(sstats, nullptr) != 0xFFFFFFFFu ? 1u : 0u;                  // 006B08D2..006B08DF
        f = lw(self, 0x1368);
        sw(self, 0x1368, f ^ (((v + v) ^ f) & 2u));                                            // 006B08E1..006B08F3
    }
    {
        U32 can = 0;                                                                           // 006B08F9..006B092C
        if (lb(self, 0x59F0) == 0xFF || (lb(recovered_server_info_004aec90(server_app(), nullptr), 0x104) & 1) == 0)
            can = kotor_fastcall_005a6810(sstats, nullptr) != 0 ? 1u : 0u;
        const U32 bit = (can & 1u) + (can & 1u);
        sw(self, 0x49AC, (lw(self, 0x49AC) & 0xFFFFFFFDu) | bit);                              // 006B092C..006B093C BTN_LEVELUP
        sw(self, 0x47E8, (lw(self, 0x47E8) & 0xFFFFFFFDu) | bit);                              // 006B0942..006B0951 BTN_AUTO
    }
    I32 good = 0;                                                                              // 006B095D edi
    if (li(server, 0x8F8) > 0) {                                                               // 006B094D..006B0961
        for (; good < li(server, 0x8F8); ++good) {                                             // 006B0970..006B09AA (count re-read)
            if (good >= 10) break;                                                             // 006B0970..006B0973
            void* label_flags = at(self, 0x3668 + static_cast<U32>(good) * 0x140u);
            const U32 image = lw(lp(server, 0x8F4), static_cast<U32>(good) * 4u) + 2u;         // 006B0975..006B0982
            gui_load_border_params_set_fill_00414c00(at(label_flags, 0x2C), nullptr, pt(image), 0); // 006B0985..006B098B
            sw(label_flags, 0, lw(label_flags, 0) | 2u);                                       // 006B0990..006B0999
        }
    }
    {
        const U32 more = li(server, 0x8F8) > 10 ? 2u : 0u;                                     // 006B09B0..006B09C8
        const U32 f = lw(self, 0x42E8);
        sw(self, 0x42E8, f ^ ((more ^ f) & 2u));                                               // 006B09CA..006B09D4 LBL_MORE
    }
    for (; good < 10; ++good) {                                                                // 006B09D1..006B0A01
        void* label_flags = at(self, 0x3668 + static_cast<U32>(good) * 0x140u);
        if ((lb(label_flags, 0) & 2) == 0) break;                                              // quirk: stops at the first hidden one
        sw(label_flags, 0, lw(label_flags, 0) & 0xFFFFFFFDu);
    }
    if (lp(self, 0x59E8) != creature || li(self, 0x59EC) != static_cast<std::int16_t>(lh(sstats, 0x17E))) { // 006B0A03..006B0A1C
        unsigned char appearance[60];
        std::memcpy(appearance, lp(creature, 0x21C), 60);                                     // 006B0A22..006B0A37 rep movsd 0Fh
        sw(appearance, 0x10, 0x7F000000u);                                                     // 006B0A3F [esp+64]
        unsigned char portrait[16];
        void* ref = vcall<void*, void*, U32>(creature, 0x148, portrait, 0);                    // 006B0A47..006B0A52
        vcall<void, void*>(lp(self, 0x59E4), 0xEC, ref);                                       // 006B0A58..006B0A5F
        void* source = creature;
        if (kotor_cell_do3dgui_0078d1e4 != 0) {                                                // 006B0A65..006B0A6C
            kotor_m15a_creature_set_appearance_006134c0(lp(self, 0x59E4), nullptr, appearance, 3, 1); // 006B0A72..006B0A81
            source = creature;                                                                 // 006B0A86 esi = [esp+18]
            kotor_m15a_appearance_set_appearance_alignment_00698150(lp(lp(self, 0x59E4), 0x21C), nullptr,
                static_cast<U32>(static_cast<I32>(static_cast<std::int16_t>(lh(lp(source, 0x2F8), 0x80))))); // 006B0A8A..006B0AA4
            vcall<void, void*>(lp(self, 0x59E4), 0x94, lp(self, 0x57EC));                      // 006B0AA9..006B0AB8
            const float position[3] = {0.0f, 0.0f, 0.0f};                                      // 006B0AC4..006B0AD8 [esp+38]
            vcall<void, const void*>(lp(self, 0x59E4), 0x8C, position);                        // 006B0ABE..006B0AE3
            const float orientation[3] = {1.0f, 0.0f, 0.0f};                                   // 006B0AEF..006B0B03 [esp+44]
            vcall<void, const void*>(lp(self, 0x59E4), 0x88, orientation);                     // 006B0AE9..006B0B0E
            kotor_m15a_creature_setup_3d_scene_006100f0(lp(self, 0x59E4), nullptr, at(self, 0x577C), source, 0, 1); // 006B0B14..006B0B26
        }
        sw(self, 0x59E8, ad(source));                                                          // 006B0B31
        const I32 align = static_cast<std::int16_t>(lh(stats, 0x80));                          // 006B0B37 (client stats, quirk)
        sw(self, 0x59EC, static_cast<U32>(align));                                             // 006B0B4C
        gui_m48_slider_set_cur_00417ee0(at(self, 0x55C4), nullptr, 100 - align);               // 006B0B3E..006B0B52 SLD_ALIGN
    }
}                                                                                              // 006B0B57..006B0B66 ~ratio

// 006AEC80 CSWGuiInGameCharacter::DoAutoLevelup(int) RET 4 (CSWGuiMessageBox callback).
void __fastcall kotor_fastcall_006aec80(void*, void*, w4::U32) {
    void* box = lp(ingame_gui(), 0x98);                                                        // 006AEC80..006AEC8D
    if ((lb(box, 0x64) & 1) == 0) return;                                                      // 006AEC93..006AEC97
    void* pc = kotor_fastcall_00635460(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr); // 006AEC99..006AECA9
    void* server = kotor_m15a_creature_get_server_creature_0060fb20(pc, nullptr);              // 006AECAE..006AECB0
    if (server == nullptr) return;                                                             // 006AECB5..006AECB7
    kotor_fastcall_005b27e0(lp(server, 0xA74), nullptr, 0);                                    // 006AECB9..006AECC7 jmp AutoLevelup(0)
}

// 006B0BB0 CSWGuiInGameCharacter::ShowLevelUpGUI(int flag) RET 4.
w4::U32 __fastcall kotor_fastcall_006b0bb0(void* self, void*, w4::U32 flag) {
    if (lw(ingame_gui(), 0x10C) == 0) return 0;                                                // 006B0BC6..006B0BDE
    if (kotor_fastcall_005ee190(client_app(), nullptr) != 0) return 0;                         // 006B0BE4..006B0BF4
    kotor_fastcall_006afda0(self, nullptr);                                                    // 006B0BFB..006B0BFD SetStats
    void* raw = live_operator_new(0x2560);                                                     // 006B0C02..006B0C0C
    void* panel = nullptr;
    if (raw != nullptr) {                                                                      // 006B0C15..006B0C1F
        auto guard = on_throw([&]() { live_free(raw); });                                      // EH state 0
        void* player = client_get_player_creature_005ed540(client_app(), nullptr);             // 006B0C21..006B0C2A
        panel = kotor_fastcall_006e8ef0(raw, nullptr, manager_of(self), lw(player, 4), lp(self, 0x59E4)); // 006B0C2F..006B0C40
        guard.armed = false;
    }
    raw_store(panel, 0x68, ad(self));                                                          // 006B0C58 (unchecked, quirk)
    raw_store(panel, 0x2544, flag != 0 ? 0u : 2u);                                             // 006B0C49..006B0C68
    kotor_fastcall_0040bc70(manager_of(self), nullptr, panel, 3, 1);                           // 006B0C54..006B0C71 AddPanel
    return 1;                                                                                  // 006B0C77
}

// 006B0CB0 CSWGuiInGameCharacter::StartAutoLevelup() RET 0.
void __fastcall kotor_fastcall_006b0cb0(void* self, void*) {
    void* pc = kotor_fastcall_00635460(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr); // 006B0CC8..006B0CDC
    void* stats = lp(pc, 0x2F8);
    const U32 experience = lw(stats, 0x58);                                                    // 006B0CE1..006B0CE7 esi
    const U8 level = static_cast<U8>(kotor_m15b_levelup_stats_get_level_00648430(stats, nullptr)); // 006B0CEA..006B0CF5
    U8 next = static_cast<U8>(level + 1);                                                      // 006B0CF9
    Str text; text.empty();                                                                    // 006B0CF1..006B0CFB [esp+18] state 0
    void* table = rules();                                                                     // 006B0D00
    while (experience >= lw(table, static_cast<U32>(next) * 4u + 0x38u)) next = static_cast<U8>(next + 1); // 006B0D05..006B0D1F (unsigned)
    {
        Str token(static_cast<I32>(level));                                                    // 006B0D21..006B0D2B
        kotor_fastcall_0041db50(kotor_cell_tlk_007a3a08, nullptr, 0, token.p());               // 006B0D30..006B0D42
    }                                                                                          // 006B0D47..006B0D50
    {
        Str token(static_cast<I32>(next));                                                     // 006B0D55..006B0D5D
        kotor_fastcall_0041db50(kotor_cell_tlk_007a3a08, nullptr, 1, token.p());               // 006B0D62..006B0D74
    }                                                                                          // 006B0D79..006B0D82
    {
        Str simple;
        void* r = static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08)->simple_0041e8f0(
            static_cast<kotor_menu_10_19::String*>(simple.p()), 0x8FCB);                       // 006B0D87..006B0D97
        simple.live = true;
        async_assign_string_005e5c50(text.p(), nullptr, r);                                    // 006B0D9C..006B0DA6
    }                                                                                          // 006B0DAB..006B0DB4
    tlk_core::ParseStr0041dd30(static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08), nullptr,
                               static_cast<kotor_reconstruction::StartupString*>(text.p()));   // 006B0DB9..006B0DC4
    void* box = lp(ingame_gui(), 0x98);                                                        // 006B0DC9..006B0DD6
    kotor_fastcall_00627130(box, nullptr, 1);                                                  // 006B0DDC..006B0DE0 SetAllowCancel(1)
    ExoVal copy;
    WindowCopy005e5b00(copy.w, nullptr, text.p());                                             // 006B0DE5..006B0DF3 by-value copy
    reinterpret_cast<void(__thiscall*)(void*, ExoVal)>(vslot(box, 0x70))(box, copy);           // 006B0DF8..006B0DFC [vtbl+70h]
    gui_load_messagebox_set_callback_00624a40(box, nullptr, self, ad(reinterpret_cast<const void*>(&kotor_fastcall_006aec80)), 0); // 006B0DFF..006B0E09
    kotor_fastcall_0040bc70(manager_of(self), nullptr, box, 1, 1);                             // 006B0E0E..006B0E16 AddPanel
}                                                                                              // 006B0E1B..006B0E27 ~text

// 006E9CA0 CSWGuiScriptSelect::SetCreatureID(ulong id) RET 4: select the creature's current AI state entry.
void __fastcall kotor_fastcall_006e9ca0(void* self, void*, w4::U32 creature_id) {
    void* creature = kotor_fastcall_004ae770(server_app(), nullptr, creature_id);              // 006E9CA0..006E9CB2
    if (creature == nullptr) { sw(self, 0xC3C, 0x7F000000u); return; }                         // 006E9CB7..006E9CB9, 006E9D38
    sw(self, 0xC3C, creature_id);                                                              // 006E9CBB
    const U32 state = lh(lp(creature, 0xA74), 0x13A);                                          // 006E9CC1..006E9CCA movzx word
    if (li(self, 0x68) <= 0) return;                                                           // 006E9CC7..006E9CD5
    void* states = at(self, 0x70);                                                             // LST_AIState
    for (I32 i = 0; i < li(self, 0x68); ++i) {                                                 // 006E9CE0..006E9D2F (count re-read)
        if (lw(lp(self, 0x64), static_cast<U32>(i) * 8u + 4u) == state) {                      // 006E9CE0..006E9CE9
            gui_load_listbox_set_selected_0041c040(states, nullptr, i, 0);                     // 006E9CEB..006E9CEE
            void* toggle = vcall<void*>(kotor_fastcall_0041a5a0(states, nullptr, i), 0x5C);    // 006E9CF3..006E9CFF
            sw(toggle, 0x1C8, lw(toggle, 0x1C8) | 1u);                                         // 006E9D02..006E9D08
        } else {
            void* toggle = vcall<void*>(kotor_fastcall_0041a5a0(states, nullptr, i), 0x5C);    // 006E9D0D..006E9D17
            sw(toggle, 0x1C8, lw(toggle, 0x1C8) & 0xFFFFFFFEu);                                // 006E9D1A..006E9D20
        }
    }
}

// ---- CSWGuiInGameCharacter slots
// 006AE980 OnPanelRemoved.
void __fastcall kotor_fastcall_006ae980(void* self, void*) {
    zero(self, 0x59E8);                                                                        // 006AE980
    gui_load_panel_on_removed_0040c170(self, nullptr);                                         // 006AE98A jmp
}
// 006B0B90 Draw(float).
void __fastcall kotor_fastcall_006b0b90(void* self, void*, std::uint32_t delta) {
    kotor_fastcall_006afda0(self, nullptr);                                                    // 006B0B93 SetStats
    kotor_panel_draw_0040B760(self, nullptr, delta);                                           // 006B0B98..006B0B9F
}
// 006B2220 OnPanelAdded.
void __fastcall kotor_fastcall_006b2220(void* self, void*) {
    sb(self, 0x59F0, 0xFF);                                                                    // 006B2228
    sw(self, 0x4B84, 0xFFFFFFFEu); sw(self, 0x4D48, 0xFFFFFFFEu);                              // 006B2223..006B2235
    kotor_fastcall_006afda0(self, nullptr);                                                    // 006B223B SetStats
    kotor_fastcall_006aee30(self, nullptr);                                                    // 006B2240..006B2242 UpdatePortraits
    gui_load_panel_on_added_0040b870(self, nullptr);                                           // 006B224A jmp
}
// 006B2250 HandleInputEvent(int event, int down) RET 8. Jump table 006B24AC / index bytes 006B24CC (event - 27h, 0..B8h).
void __fastcall kotor_fastcall_006b2250(void* self, void*, std::uint32_t event, std::uint32_t down) {
    frame_GetCharacter_006346c0(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr, 0);  // 006B2250..006B2264 (result unused)
    if (down != 0 && event - 0x27u <= 0xB8u) {                                                 // 006B2269..006B2281
        switch (event) {
        case 0x27:                                                                             // 006B2295 level up
            if (lw(ingame_gui(), 0x10C) != 0 && kotor_fastcall_005ee190(client_app(), nullptr) == 0) {
                play_sound(self, 0);                                                           // 006B22C6..006B22CA
                kotor_fastcall_006b0bb0(self, nullptr, 0);                                     // 006B22CF..006B22D3
            }
            break;
        case 0x29: {                                                                           // 006B22DD scripts
            play_sound(self, 0);                                                               // 006B22DD..006B22E2
            void* character = frame_GetCharacter_006346c0(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr, 0); // 006B22E7..006B22F9
            if (character != nullptr) {                                                        // 006B22FE..006B2300
                const U32 id = kotor_fastcall_004aea30(server_app(), nullptr, lw(character, 4)); // 006B2306..006B2313
                kotor_fastcall_006e9ca0(lp(self, 0x59F4), nullptr, id);                        // 006B2318..006B231F
                kotor_fastcall_0040bc70(manager_of(self), nullptr, lp(self, 0x59F4), 3, 1);    // 006B2324..006B2332
            }
            break;
        }
        case 0x2A:                                                                             // 006B233C auto level up
            if (lw(ingame_gui(), 0x10C) != 0 && kotor_fastcall_005ee190(client_app(), nullptr) == 0) {
                play_sound(self, 0);                                                           // 006B236E..006B2372
                kotor_fastcall_006b0cb0(self, nullptr);                                        // 006B2377..006B2379
            }
            break;
        case 0xCE: {                                                                           // 006B2383 next party member
            play_sound(self, 1);                                                               // 006B2383..006B238B
            if ((lb(recovered_server_info_004aec90(server_app(), nullptr), 0x104) & 1) != 0) { // 006B2390..006B23A4
                I32 npc = s8(lb(self, 0x59F0)) + 1;                                            // 006B23AA..006B23B3
                U32 found = 0;                                                                 // 006B23B1 ebp
                for (; npc < 9 && found == 0; ++npc) {                                         // 006B23B4..006B2420
                    if (kotor_fastcall_005636b0(party_table(), nullptr, npc) == 0) continue;   // 006B23C8..006B23E0
                    void* client = client_app();                                               // 006B23E2..006B23EA esi
                    const U32 object = recovered_npc_object_00564700(party_table(), nullptr, npc, found, 1); // 006B23ED..006B23F8
                    if (frame_R4_005ed5b0(client, nullptr, kotor_fastcall_005eda50(client, nullptr, object)) != 0) { // 006B23FD..006B240F
                        sb(self, 0x59F0, static_cast<U8>(npc));                                // 006B2411
                        found = 1;                                                             // 006B2417
                    }
                }
                if (found == 0) sb(self, 0x59F0, 0xFF);                                        // 006B2426..006B2428
            } else {
                kotor_fastcall_005edf80(client_app(), nullptr, 0, 0, 1);                       // 006B2439..006B2448
            }
            kotor_fastcall_006aee30(self, nullptr);                                            // 006B242F / 006B244F UpdatePortraits
            break;
        }
        case 0x28: case 0x2D: case 0x2E: case 0xDF:                                            // 006B2459 close
            if (frame_R4_0062cba0(ingame_gui(), nullptr, 0) != 0)                              // 006B2459..006B2471
                gui_m48_client_set_input_class_005eda60(client_app(), nullptr, 0, 1);          // 006B2473..006B2480
            break;
        case 0x3B: kotor_fastcall_006aed20(self, nullptr, 1); break;                           // 006B2487..006B248F
        case 0x3C: kotor_fastcall_006aed20(self, nullptr, 0); break;                           // 006B248B..006B248F
        default: break;
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, down);                          // 006B2494..006B24A0
}
}  // extern "C"
