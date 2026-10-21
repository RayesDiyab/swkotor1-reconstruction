// Lane W4 GUI successor (worker "levelup", fork lu-chargen): CSWGuiLevelUpCharGen (0x2560, method table 00758F50) and
// the CSWGuiInGameCharacter::AttachModel helper its close paths call. Transcribed from the original swkotor.exe
// linear sweep (read-only); every statement carries its original PCs.
//   006E8EF0 CSWGuiLevelUpCharGen::CSWGuiLevelUpCharGen(manager, creature_id, CSWCCreature* preview) thiscall RET 0Ch
//   006E7B70 ~CSWGuiLevelUpCharGen                  thiscall RET      006E89D0 scalar deleting dtor  thiscall RET 4
//   006E7B40 OnPanelAdded                           thiscall RET      (slot 48h)
//   006E9A50 HandleInputEvent(event, down)          thiscall RET 8    (slot 3Ch)
//   006E7DD0 DetermineClass                         thiscall RET
//   006E7EC0 SetNameString(const CExoString&)       thiscall RET 4
//   006E7F70 AbilitiesChanged                       thiscall RET
//   006E8500 NewAbilityScores(str, cha, int, wis, con, dex, points)  thiscall RET 1Ch  (entry jmp'd to by
//            CSWGuiLevelUpPanel::NewAbilityScores 006EE0D0 with ECX = [panel+68h]; no Ghidra function there)
//   006E85A0 ClearAbilities / 006E8730 ClearSkills / 006E87E0 ClearPowers / 006E8DB0 ClearFeats   thiscall RET
//   006E88A0 BeginPlayGame / 006E8E20 CancelLevelUp thiscall RET
//   006E89F0 CopyCreatureStats(CSWCCreature* from, CSWCCreature* to) thiscall RET 8, returns 1 (ECX unused)
//   006AEBF0 CSWGuiInGameCharacter::AttachModel     thiscall RET
// Layout (Ghidra CSWGuiLevelUpCharGen + observed): +64 CSWCCreature* snapshot (new 44Ch, stats copied from the live
// creature at construction; restored from by the Clear* functions), +68 CSWGuiInGameCharacter* (stored by
// ShowLevelUpGUI), +6C CSWGuiLevelUpPanel* (new 1CECh), +70..+2230 28 labels (stride 140h), +2370 CSWGui3DSceneView
// (CSWGuiControl 5Ch + CSWGuiScene +23CC, room = scene+14h = +23E0, camera = scene+18h = +23E4), +2400 PORTRAIT_LBL,
// +2544 input class (CancelLevelUp is a no-op while 0), +2548 class name, +2550 name (CExoString), +255C creature id.
#include "6030a9ee65-claude_gui_char.hpp"

namespace kotor_reconstruction { struct StartupString; }
namespace base_ini_reconstruction {
kotor_reconstruction::StartupString* __fastcall left_005e5f70(kotor_reconstruction::StartupString* self, void*,
    kotor_reconstruction::StartupString* out, int count);                                         // CExoString::Left
kotor_reconstruction::StartupString* __fastcall right_005e6170(kotor_reconstruction::StartupString* self, void*,
    kotor_reconstruction::StartupString* out, int count);                                         // CExoString::Right
}
namespace recovered_save_stats {
unsigned short __cdecl maximum_hp_00647a80(void* stats);                                          // CSWCCreatureStats::GetMaxHitPoints
std::uint16_t __cdecl spell_count_006487d0(void* stats, unsigned cls, unsigned level);            // GetNumberKnownSpells
}
namespace kotor { namespace server_mainloop { unsigned __cdecl creature_force_class_005be4a0(unsigned cls); } } // CSWClass::IsJedi
namespace w4 { extern const volatile U32 ingame_gui_single_panel_007a2370; }
namespace frame_control { extern std::uint32_t sound_manager_007a39ec; }                          // [007A39EC] CExoSound*

extern "C" {
// ---- strings
std::int32_t __fastcall find_005e5f20(void* string, void*, std::uint32_t character, std::int32_t start); // CExoString::Find
std::int32_t __fastcall async_string_length_005e5790(void* string, void*);                     // CExoString::GetLength
void* __fastcall ref_assign_copy_00406120(void* ref, void*, void* out, const void* source);    // CResRef::operator= (by value)
// ---- client creature stats (m15b / m15e / w3 owners)
std::int8_t __fastcall kotor_m15b_creature_stats_ability_mod_00647630(void* stats, void*, std::uint8_t value);
std::int16_t __fastcall kotor_m15b_creature_stats_get_armor_class_00647720(void* stats, void*, std::uint32_t unused);
std::uint8_t __fastcall kotor_m15b_creature_stats_get_class_00647730(void* stats, void*, std::uint8_t index);
std::uint8_t __fastcall kotor_m15b_creature_stats_get_class_level_00647750(void* stats, void*, std::uint8_t index);
void* __fastcall kotor_m15b_creature_stats_get_classes_string_00648470(void* stats, void*, void* out, std::uint8_t separator) noexcept(false);
std::uint32_t __fastcall kotor_m15b_creature_stats_get_feat_006486e0(void* stats, void*, std::uint16_t index);
void __fastcall kotor_m15b_creature_stats_clear_skills_006475e0(void* stats, void*);
void __fastcall kotor_m15b_creature_stats_clear_feats_00648320(void* stats, void*);
void __fastcall kotor_m15b_creature_stats_clear_known_spells_00649520(void* stats, void*);
void __fastcall kotor_m15b_levelup_stats_set_str_base_0064a0d0(void* stats, void*, std::uint8_t value);
void __fastcall kotor_m15b_levelup_stats_set_dex_base_0064a100(void* stats, void*, std::uint8_t value, std::int32_t keep_ac_delta);
void __fastcall kotor_m15b_levelup_stats_set_con_base_006496d0(void* stats, void*, std::uint8_t value);
void __fastcall kotor_m15b_levelup_stats_set_int_base_006478a0(void* stats, void*, std::uint8_t value);
void __fastcall kotor_m15b_levelup_stats_set_wis_base_00649770(void* stats, void*, std::uint8_t value);
void __fastcall kotor_m15b_levelup_stats_set_cha_base_006478d0(void* stats, void*, std::uint8_t value);
std::uint32_t __fastcall kotor_m15b_levelup_stats_get_base_attack_bonus_006479f0(void* stats, void*);
std::int32_t __fastcall kotor_m15b_levelup_stats_get_level_00648430(void* stats, void*);
void __fastcall kotor_m15b_levelup_stats_set_skill_rank_base_006488e0(void* stats, void*, std::uint16_t skill, std::uint8_t rank);
void __fastcall kotor_m15b_levelup_stats_set_fort_saving_throw_00648b30(void* stats, void*);
void __fastcall kotor_m15b_levelup_stats_set_will_saving_throw_00648be0(void* stats, void*);
void __fastcall kotor_m15b_levelup_stats_set_reflex_saving_throw_00648c90(void* stats, void*);
void* __fastcall kotor_m15e_creature_stats_get_portrait_00647e80(void* stats, void*, void* out, w4::U32 variant) noexcept(false);
w4::U32 __fastcall kotor_fastcall_006495a0(void* stats, void*, w4::U32 cls, w4::U32 level, w4::U32 index) noexcept; // GetKnownSpell
void __fastcall recovered_creature_stats_add_known_spell_00649f90(void* stats, void*, w4::U32 cls, w4::U32 spell) noexcept(false);
void __fastcall kotor_m15_array_u16_add_unique_005a7670(void* list, void*, w4::U32 value);     // CExoArrayList<ushort>::AddUnique
// ---- client creature / 3D
void* __fastcall kotor_m15a_creature_construct_00616a20(void* self, void*) noexcept(false);    // CSWCCreature::CSWCCreature
void __fastcall kotor_m15a_creature_setup_3d_scene_006100f0(void* creature, void*, void* view, void* source, w4::U32, w4::U32);
void* __fastcall gui_load_control_construct_0041aa80(void* self, void*) noexcept(false);
void __fastcall gui_load_control_destruct_00419a20(void* self, void*);
extern const unsigned char gui_load_scene_view_methods_00752e30[];
void* __fastcall gui_scene_construct_004174b0(void* scene, void*) noexcept(false);
void __fastcall gui_load_scene_view_destruct_0067ac50(void* view, void*);
void* __fastcall aurora_gui_scene_add_model_name_00417620(void* scene, void*, void* name, w4::I32 index);
// ---- client app
w4::U32 __fastcall gui_m48_client_get_input_class_005eda70(void* app, void*);                  // CClientExoApp::GetInputClass
void* __fastcall kotor_fastcall_005ed6f0(void* app, void*);                                     // CClientExoApp::GetSWCMessage
void __fastcall frame_R4_005d5e80(void* sound, void*, unsigned mode);                          // CExoSoundInternal::SetSoundMode
extern const std::uint32_t kotor_rdata_sound_mode_0074c5ec;                                    // [0074C5EC]
// ---- CSWGuiLevelUpPanel (fork lu-panel) / client rules (fork lu-crules)
void* __fastcall kotor_fastcall_006ee7d0(void* self, void*, void* manager, w4::U32 creature_id); // CSWGuiLevelUpPanel ctor RET 8
void __fastcall kotor_fastcall_006ee640(void* panel, void*, w4::U32 can_cancel);                // SetCanCancel RET 4
void __fastcall kotor_fastcall_0064a1f0(void* stats, void*, w4::U32 index, w4::U32 class_id);   // CSWCLevelUpStats::SetClass RET 8
void __fastcall kotor_fastcall_0064a2a0(void* stats, void*, w4::U32 index, w4::U32 level);      // SetClassLevel RET 8
void __fastcall kotor_fastcall_00649580(void* stats, void*, w4::U32 index);                     // CSWCCreatureStats::ClearKnownSpells RET 4
void __fastcall kotor_fastcall_00678280(void* message, void*, void* before, void* after);       // SendPlayerToServer_LevelUp RET 8
// ---- defined here
void __fastcall kotor_fastcall_006aebf0(void* character, void*);
void __fastcall kotor_fastcall_006e7b40(void* self, void*);
void __fastcall kotor_fastcall_006e7b70(void* self, void*);
void __fastcall kotor_fastcall_006e7dd0(void* self, void*);
void __fastcall kotor_fastcall_006e7ec0(void* self, void*, void* name);
void __fastcall kotor_fastcall_006e7f70(void* self, void*);
void __fastcall kotor_fastcall_006e8500(void* self, void*, w4::U32 str, w4::U32 cha, w4::U32 intel, w4::U32 wis,
                                        w4::U32 con, w4::U32 dex, w4::U32 points);
void __fastcall kotor_fastcall_006e85a0(void* self, void*);
void __fastcall kotor_fastcall_006e8730(void* self, void*);
void __fastcall kotor_fastcall_006e87e0(void* self, void*);
void __fastcall kotor_fastcall_006e88a0(void* self, void*);
void* __fastcall kotor_fastcall_006e89d0(void* self, void*, w4::U32 flags);
w4::U32 __fastcall kotor_fastcall_006e89f0(void* self, void*, void* from, void* to);
void __fastcall kotor_fastcall_006e8db0(void* self, void*);
void __fastcall kotor_fastcall_006e8e20(void* self, void*);
void* __fastcall kotor_fastcall_006e8ef0(void* self, void*, void* manager, w4::U32 creature_id, void* preview);
void __fastcall kotor_fastcall_006e9a50(void* self, void*, w4::U32 event, w4::U32 down);
}

using namespace gchar;

namespace {
inline void sh(void* o, U32 off, U16 v) { std::memcpy(static_cast<U8*>(o) + off, &v, 2); }
inline I32 s8(U32 v) { return static_cast<std::int8_t>(static_cast<U8>(v)); }
inline I32 s16(U32 v) { return static_cast<std::int16_t>(static_cast<U16>(v)); }
inline void* stats_of(void* creature) { return lp(creature, 0x2F8); }                          // CSWCCreature +2F8h
inline void* snapshot_stats(void* self) { return stats_of(lp(self, 0x64)); }                    // [[this+64h]+2F8h]
// [[007A39FC]+4]->GetCreatureByGameObjectID([this+255Ch])
inline void* live_creature(void* self) { return pt(frame_R4_005ed5b0(client_app(), nullptr, lw(self, 0x255C))); }
inline U8 ability_mod(void* stats, U8 value) { return static_cast<U8>(kotor_m15b_creature_stats_ability_mod_00647630(stats, nullptr, value)); }
// CExoString(int) temporary -> CSWGuiTextParams::SetText (label text params = label + E8h) -> ~CExoString.
inline void text_int(void* self, U32 params, I32 value) {
    Str t(value);
    gui_load_text_params_set_text_00415e00(at(self, params), nullptr, t.p());
}
// [control+44h] = ([control+44h] & ~300h) | 400h
inline void hide_flags(void* control) { sw(control, 0x44, (lw(control, 0x44) & 0xFFFFFCFFu) | 0x400u); }
using SS = kotor_reconstruction::StartupString;
inline SS* ss(void* p) { return static_cast<SS*>(p); }
}  // namespace

// ======================= method table 00758F50 (27 slots; base CSWGuiPanel 0073E010) =======================
namespace {
const w4gui::Override ovr_levelup_chargen[] = {
    {0x00, w4gui::fn(&kotor_fastcall_006e89d0)},   // 006E89D0 scalar deleting destructor
    {0x3C, w4gui::fn(&kotor_fastcall_006e9a50)},   // 006E9A50 HandleInputEvent
    {0x48, w4gui::fn(&kotor_fastcall_006e7b40)},   // 006E7B40 OnPanelAdded
};
void* table_levelup_chargen[27];
bool built_levelup_chargen = false;
const void* vt_levelup_chargen() {
    if (!built_levelup_chargen) {
        w4gui::vt_build(table_levelup_chargen, 27, gui_load_panel_methods_0073e010, 27, ovr_levelup_chargen, 3);
        built_levelup_chargen = true;
    }
    return table_levelup_chargen;
}
}  // namespace

extern "C" {
// 006AEBF0 CSWGuiInGameCharacter::AttachModel() RET 0.
void __fastcall kotor_fastcall_006aebf0(void* character, void*) {
    if (kotor_cell_do3dgui_0078d1e4 == 0) return;                                              // 006AEBF0..006AEBFD
    void* preview = lp(character, 0x59E4);                                                     // 006AEBFF
    vcall<void, void*>(preview, 0x94, lp(character, 0x57EC));                                  // 006AEC05..006AEC0F
    void* hook = vcall<void*, U32>(lp(character, 0x57F0), 0x80, 0x3EA);                        // 006AEC15..006AEC22
    if (hook == nullptr) return;                                                               // 006AEC28..006AEC2C
    sw(hook, 0x48, 1);                                                                         // 006AEC2E
    const float height = vcall<float>(lp(character, 0x59E4), 0xD8);                            // 006AEC35..006AEC4D
    sw(hook, 0x3C, 0); sw(hook, 0x40, 0);                                                      // 006AEC3D..006AEC68 ({0, 0} locals)
    sf(hook, 0x44, height);                                                                    // 006AEC60..006AEC6B fstp dword
}

// 006E7B40 OnPanelAdded: SetCanCancel(0) on the step panel, AddPanel(panel, 1, 1), then CSWGuiPanel::OnPanelAdded.
void __fastcall kotor_fastcall_006e7b40(void* self, void*) {
    kotor_fastcall_006ee640(lp(self, 0x6C), nullptr, 0);                                       // 006E7B43..006E7B48
    kotor_fastcall_0040bc70(manager_of(self), nullptr, lp(self, 0x6C), 1, 1);                  // 006E7B4D..006E7B58
    gui_load_panel_on_added_0040b870(self, nullptr);                                           // 006E7B5D..006E7B60 jmp
}

// 006E7B70 ~CSWGuiLevelUpCharGen (the level-up panel +6Ch is not destroyed here; original behaviour).
void __fastcall kotor_fastcall_006e7b70(void* self, void*) {
    set_vptr(self, vt_levelup_chargen());                                                      // 006E7B8D
    if (void* creature = lp(self, 0x64)) vcall<void*, U32>(creature, 0x00, 1);                 // 006E7B93..006E7BA6
    zero(self, 0x64);                                                                          // 006E7BAE
    frame_LogStringDtor_005e5c20(at(self, 0x2550), nullptr);                                   // 006E7BA8..006E7BBA
    frame_LogStringDtor_005e5c20(at(self, 0x2548), nullptr);                                   // 006E7BBF..006E7BCA
    gui_load_label_destruct_00419d10(at(self, 0x2400), nullptr);                               // 006E7BCF..006E7BDA
    gui_load_scene_view_destruct_0067ac50(at(self, 0x2370), nullptr);                          // 006E7BDF..006E7BEA
    for (U32 i = 28; i-- > 0;) gui_load_label_destruct_00419d10(at(self, 0x70 + i * 0x140), nullptr); // 006E7BEF..006E7DA7
    gui_load_panel_destruct_0040cf70(self, nullptr);                                           // 006E7DAC..006E7DB6
}
// 006E89D0 scalar deleting destructor RET 4.
void* __fastcall kotor_fastcall_006e89d0(void* self, void*, w4::U32 flags) {
    kotor_fastcall_006e7b70(self, nullptr);                                                    // 006E89D3
    if ((flags & 1u) != 0) live_free(self);                                                    // 006E89D8..006E89E5
    return self;                                                                               // 006E89E8
}

// 006E7DD0 DetermineClass: class name = GetClassesString('\n') of the snapshot, text after the first '\n', before '('.
void __fastcall kotor_fastcall_006e7dd0(void* self, void*) {
    void* name = at(self, 0x2548);
    {
        Str t;
        void* r = kotor_m15b_creature_stats_get_classes_string_00648470(snapshot_stats(self), nullptr, t.s, 0x0A); // 006E7DEC..006E7DFC
        t.live = true;                                                                         // state 0
        async_assign_string_005e5c50(name, nullptr, r);                                        // 006E7E01..006E7E12
    }                                                                                          // 006E7E17..006E7E23
    {
        const I32 position = find_005e5f20(name, nullptr, 0x0A, 0);                            // 006E7E28..006E7E2E
        I32 count = -1 - position;                                                             // 006E7E33..006E7E38
        count += async_string_length_005e5790(name, nullptr);                                  // 006E7E36..006E7E3F
        Str t;
        SS* r = base_ini_reconstruction::right_005e6170(ss(name), nullptr, ss(t.s), count);    // 006E7E41..006E7E49
        t.live = true;                                                                         // state 1
        async_assign_string_005e5c50(name, nullptr, r);                                        // 006E7E4E..006E7E59
    }                                                                                          // 006E7E5E..006E7E6A
    {
        const I32 position = find_005e5f20(name, nullptr, 0x28, 0);                            // 006E7E6F..006E7E75
        Str t;
        SS* r = base_ini_reconstruction::left_005e5f70(ss(name), nullptr, ss(t.s), position);  // 006E7E7A..006E7E82
        t.live = true;                                                                         // state 2
        async_assign_string_005e5c50(name, nullptr, r);                                        // 006E7E87..006E7E92
    }                                                                                          // 006E7E97..006E7EA3
}

// 006E7EC0 SetNameString(const CExoString& name) RET 4.
void __fastcall kotor_fastcall_006e7ec0(void* self, void*, void* name) {
    void* stored = at(self, 0x2550);
    async_assign_string_005e5c50(stored, nullptr, name);                                       // 006E7EE0..006E7EE9
    const U8 level = static_cast<U8>(kotor_m15b_levelup_stats_get_level_00648430(snapshot_stats(self), nullptr)); // 006E7EEE..006E7EF7
    Str t(static_cast<I32>(level) + 1);                                                        // 006E7EFC..006E7F05 movzx; inc
    gui_load_text_params_set_text_00415e00(at(self, 0x298), nullptr, stored);                  // 006E7F0A..006E7F19 LBL_NAME
    gui_load_text_params_set_text_00415e00(at(self, 0x3D8), nullptr, at(self, 0x2548));        // 006E7F1E..006E7F2B LBL_CLASS
    gui_load_text_params_set_text_00415e00(at(self, 0x658), nullptr, t.p());                   // 006E7F30..006E7F3B LBL_LEVEL_VAL
}                                                                                              // 006E7F40..006E7F4C

// 006E7F70 AbilitiesChanged: refresh the level-up preview of the live creature (bases, saves, VP/FP gain, AC).
void __fastcall kotor_fastcall_006e7f70(void* self, void*) {
    void* creature = live_creature(self);                                                      // 006E7F8E..006E7FA3
    if (creature == nullptr) return;                                                           // 006E7FA8..006E7FAC
    kotor_m15b_levelup_stats_set_fort_saving_throw_00648b30(stats_of(creature), nullptr);      // 006E7FB2..006E7FB8
    kotor_m15b_levelup_stats_set_will_saving_throw_00648be0(stats_of(creature), nullptr);      // 006E7FBD..006E7FC3
    kotor_m15b_levelup_stats_set_reflex_saving_throw_00648c90(stats_of(creature), nullptr);    // 006E7FC8..006E7FCE
    text_int(self, 0x1558, lb(stats_of(creature), 0x3F));                                      // 006E7FD3..006E800A CHA_AB_LBL
    text_int(self, 0x0F18, lb(stats_of(creature), 0x3A));                                      // 006E800F..006E8043 STR_AB_LBL
    text_int(self, 0x12D8, lb(stats_of(creature), 0x3D));                                      // 006E8048..006E807C INT_AB_LBL
    text_int(self, 0x1418, lb(stats_of(creature), 0x3E));                                      // 006E8081..006E80B5 WIS_AB_LBL
    text_int(self, 0x1198, lb(stats_of(creature), 0x3C));                                      // 006E80BA..006E80EE CON_AB_LBL
    text_int(self, 0x1058, lb(stats_of(creature), 0x3B));                                      // 006E80F3..006E8127 DEX_AB_LBL
    const U8 cls = kotor_m15b_creature_stats_get_class_00647730(
        stats_of(creature), nullptr, static_cast<U8>(lb(stats_of(creature), 0x32) - 1));        // 006E812C..006E8140 [esp+17]
    // CON: modifier delta against the server creature's stats byte +EEh.
    void* stats = stats_of(creature);                                                          // 006E8145 ebx
    const U8 con = lb(stats, 0x36);                                                            // 006E814B
    U8 server = lb(lp(kotor_m15a_creature_get_server_creature_0060fb20(creature, nullptr), 0xA74), 0xEE); // 006E8152..006E8163
    const U8 con_delta = static_cast<U8>(s8(ability_mod(stats, con)) - s8(server));            // 006E8169..006E818A ebx
    stats = stats_of(creature);                                                                // 006E817E ebp
    const U8 wis = lb(stats, 0x38);                                                            // 006E8184
    server = lb(lp(kotor_m15a_creature_get_server_creature_0060fb20(creature, nullptr), 0xA74), 0xF2); // 006E818C..006E81A1
    const U8 wis_delta = static_cast<U8>(s8(ability_mod(stats, wis)) - s8(server));            // 006E81A7..006E81C3 [esp+24]
    stats = stats_of(creature);                                                                // 006E81B8 ebp
    const U8 cha = lb(stats, 0x39);                                                            // 006E81C7
    server = lb(lp(kotor_m15a_creature_get_server_creature_0060fb20(creature, nullptr), 0xA74), 0xF4); // 006E81CA..006E81DB
    const U8 cha_delta = static_cast<U8>(s8(ability_mod(stats, cha)) - s8(server));            // 006E81E1..006E8210 [esp+28]
    const U32 class_row = static_cast<U32>(cls) * 0x1A0u;                                      // 006E81F1..006E8208 ebp
    const U8 hit_die = lb(lp(rules(), 0xAC), class_row + 0x54);                                // 006E81FB..006E821A [esp+17]
    void* vstats = stats_of(creature);                                                         // 006E821E [esp+1C]
    const U8 vcon = lb(vstats, 0x36);                                                          // 006E8224
    U8 vitality = static_cast<U8>(static_cast<U8>(kotor_m15b_levelup_stats_get_level_00648430(snapshot_stats(self), nullptr)) * con_delta); // 006E822B..006E8243 imul bl
    vitality = static_cast<U8>(vitality + ability_mod(vstats, vcon));                          // 006E8245..006E8253
    vitality = static_cast<U8>(vitality + hit_die);                                            // 006E8255..006E8259
    if (s8(vitality) <= 1) vitality = 1;                                                       // 006E825B..006E8260 (signed jg)
    sh(stats_of(creature), 0x48, static_cast<U16>(s8(hit_die) + s16(lh(snapshot_stats(self), 0x48)))); // 006E8262..006E827F
    {
        void* target = stats_of(creature);                                                     // 006E8286..006E8292 [esp+1C]
        const U16 max_hp = recovered_save_stats::maximum_hp_00647a80(snapshot_stats(self));    // 006E8283..006E8296
        sh(target, 0x4E, static_cast<U16>(max_hp + s8(vitality)));                             // 006E829B..006E82A1
    }
    sh(stats_of(creature), 0x4C, static_cast<U16>(s8(vitality) + s16(lh(snapshot_stats(self), 0x4C)))); // 006E82A5..006E82BF
    if (kotor::server_mainloop::creature_force_class_005be4a0(cls) != 0) {                     // 006E82BA..006E82CD IsJedi
        const U8 force_die = lb(lp(rules(), 0xAC), class_row + 0x55);                          // 006E82D3..006E82DF [esp+16]
        void* fstats = stats_of(creature);                                                     // 006E82E3 ebp
        const U8 fcha = lb(fstats, 0x39);                                                      // 006E82E9 [esp+20]
        const U8 fwis = lb(fstats, 0x38);                                                      // 006E82EC [esp+1C]
        const U8 cha_mod = ability_mod(fstats, fcha);                                          // 006E82F7..006E8310 [esp+17]
        const U8 level = static_cast<U8>(kotor_m15b_levelup_stats_get_level_00648430(snapshot_stats(self), nullptr)); // 006E8307..006E8314
        U8 force = static_cast<U8>(level * static_cast<U8>(wis_delta + cha_delta));            // 006E8319..006E8325 imul cl
        force = static_cast<U8>(force + ability_mod(fstats, fwis));                            // 006E8327..006E8335
        force = static_cast<U8>(force + cha_mod);                                              // 006E8331..006E8337
        force = static_cast<U8>(force + force_die);                                            // 006E8339..006E833D
        if (s8(force) <= 1) force = 1;                                                         // 006E833F..006E8344 (signed jg)
        sh(stats_of(creature), 0x120, static_cast<U16>(s8(force_die) + s16(lh(snapshot_stats(self), 0x120)))); // 006E8346..006E8362
        sh(stats_of(creature), 0x11E, static_cast<U16>(s16(lh(snapshot_stats(self), 0x11E)) + s8(force)));     // 006E8369..006E8385
        {
            void* prev = snapshot_stats(self);                                                 // 006E838C..006E838F
            const U16 sum = static_cast<U16>(lh(prev, 0x124) + lh(prev, 0x122));               // 006E8395..006E839E
            sh(stats_of(creature), 0x122, static_cast<U16>(s8(force) + sum));                  // 006E83A5..006E83AD
        }
    }
    text_int(self, 0x17D8, kotor_m15b_creature_stats_get_armor_class_00647720(stats_of(creature), nullptr, 0)); // 006E83B4..006E83F1 LBL_DEF (movsx)
    text_int(self, 0x1B98, s8(lb(stats_of(creature), 0x5C)));                                  // 006E83F6..006E842A NEW_FORT_LBL
    text_int(self, 0x1A58, s8(lb(stats_of(creature), 0x5E)));                                  // 006E842F..006E8463 NEW_REFL_LBL
    text_int(self, 0x1698, s16(recovered_save_stats::maximum_hp_00647a80(stats_of(creature)))); // 006E8468..006E84A0 LBL_VIT (movsx)
    text_int(self, 0x1918, s8(lb(stats_of(creature), 0x5D)));                                  // 006E84A5..006E84D9 NEW_WILL_LBL
}

// 006E8500 NewAbilityScores(str, cha, int, wis, con, dex, points) RET 1Ch: write the chosen bases into the live
// creature's level-up stats; the remaining ability points go to stats+ACh.
void __fastcall kotor_fastcall_006e8500(void* self, void*, w4::U32 str, w4::U32 cha, w4::U32 intel, w4::U32 wis,
                                        w4::U32 con, w4::U32 dex, w4::U32 points) {
    void* creature = live_creature(self);                                                      // 006E8500..006E8513
    if (creature == nullptr) return;                                                           // 006E8518..006E851C
    kotor_m15b_levelup_stats_set_str_base_0064a0d0(stats_of(creature), nullptr, static_cast<U8>(str));   // 006E851E..006E8529
    kotor_m15b_levelup_stats_set_cha_base_006478d0(stats_of(creature), nullptr, static_cast<U8>(cha));   // 006E852E..006E8539
    kotor_m15b_levelup_stats_set_int_base_006478a0(stats_of(creature), nullptr, static_cast<U8>(intel)); // 006E853E..006E8549
    kotor_m15b_levelup_stats_set_wis_base_00649770(stats_of(creature), nullptr, static_cast<U8>(wis));   // 006E854E..006E8559
    kotor_m15b_levelup_stats_set_con_base_006496d0(stats_of(creature), nullptr, static_cast<U8>(con));   // 006E855E..006E8569
    kotor_m15b_levelup_stats_set_dex_base_0064a100(stats_of(creature), nullptr, static_cast<U8>(dex), 1); // 006E856E..006E857B
    sb(stats_of(creature), 0xAC, static_cast<U8>(points));                                     // 006E8580..006E858A
}

// 006E85A0 ClearAbilities: restore bases, saves, VP and FP of the live creature from the snapshot.
void __fastcall kotor_fastcall_006e85a0(void* self, void*) {
    void* creature = live_creature(self);                                                      // 006E85A0..006E85B3
    if (creature == nullptr) return;                                                           // 006E85B8..006E85BC
    kotor_m15b_levelup_stats_set_str_base_0064a0d0(stats_of(creature), nullptr, lb(snapshot_stats(self), 0x3A)); // 006E85C2..006E85D8
    kotor_m15b_levelup_stats_set_cha_base_006478d0(stats_of(creature), nullptr, lb(snapshot_stats(self), 0x3F)); // 006E85DD..006E85F2
    kotor_m15b_levelup_stats_set_int_base_006478a0(stats_of(creature), nullptr, lb(snapshot_stats(self), 0x3D)); // 006E85F7..006E860C
    kotor_m15b_levelup_stats_set_wis_base_00649770(stats_of(creature), nullptr, lb(snapshot_stats(self), 0x3E)); // 006E8611..006E8626
    kotor_m15b_levelup_stats_set_con_base_006496d0(stats_of(creature), nullptr, lb(snapshot_stats(self), 0x3C)); // 006E862B..006E8640
    kotor_m15b_levelup_stats_set_dex_base_0064a100(stats_of(creature), nullptr, lb(snapshot_stats(self), 0x3B), 1); // 006E8645..006E865C
    kotor_m15b_levelup_stats_set_fort_saving_throw_00648b30(stats_of(creature), nullptr);      // 006E8661..006E8667
    kotor_m15b_levelup_stats_set_will_saving_throw_00648be0(stats_of(creature), nullptr);      // 006E866C..006E8672
    kotor_m15b_levelup_stats_set_reflex_saving_throw_00648c90(stats_of(creature), nullptr);    // 006E8677..006E867D
    sh(stats_of(creature), 0x48, lh(snapshot_stats(self), 0x48));                              // 006E8682..006E8695
    {
        void* target = stats_of(creature);                                                     // 006E86A2 ebx
        sh(target, 0x4E, recovered_save_stats::maximum_hp_00647a80(snapshot_stats(self)));     // 006E8699..006E86AD
    }
    sh(stats_of(creature), 0x4C, lh(snapshot_stats(self), 0x4C));                              // 006E86B1..006E86C4
    sh(stats_of(creature), 0x120, lh(snapshot_stats(self), 0x120));                            // 006E86C8..006E86DE
    sh(stats_of(creature), 0x11E, lh(snapshot_stats(self), 0x11E));                            // 006E86E5..006E86FB
    {
        void* prev = snapshot_stats(self);                                                     // 006E8702..006E8705
        sh(stats_of(creature), 0x122, static_cast<U16>(lh(prev, 0x124) + lh(prev, 0x122)));    // 006E870B..006E871F
    }
}

// 006E8730 ClearSkills: restore the skill points and every base rank (inlined GetSkillRank bound check).
void __fastcall kotor_fastcall_006e8730(void* self, void*) {
    void* creature = live_creature(self);                                                      // 006E8731..006E8744
    if (creature == nullptr) return;                                                           // 006E8749..006E874D
    sh(stats_of(creature), 0xAE, lh(snapshot_stats(self), 0xAE));                              // 006E8753..006E8769
    U8 count = lb(rules(), 0xAB);                                                              // 006E8770..006E8776
    for (U8 skill = 0; skill < count;) {                                                       // 006E877D..006E8781 (unsigned)
        U8 rank = 0;                                                                           // 006E8790
        if (static_cast<U16>(skill) < static_cast<U16>(count))                                 // 006E8783..006E878E
            rank = lb(lp(snapshot_stats(self), 0x88), skill);                                  // 006E8797..006E87AC
        kotor_m15b_levelup_stats_set_skill_rank_base_006488e0(stats_of(creature), nullptr, skill, rank); // 006E87B0..006E87BC
        count = lb(rules(), 0xAB);                                                             // 006E87C1..006E87C6
        ++skill;                                                                               // 006E87CC..006E87D0 (jb)
    }
}

// 006E87E0 ClearPowers: replace the live creature's known spells of the last class by the snapshot's.
void __fastcall kotor_fastcall_006e87e0(void* self, void*) {
    void* creature = live_creature(self);                                                      // 006E87E3..006E87F6
    if (creature == nullptr) return;                                                           // 006E87FB..006E8803
    const U32 cls = static_cast<U32>(lb(snapshot_stats(self), 0x32)) - 1u;                     // 006E8809..006E881E movzx; dec
    kotor_m15b_creature_stats_clear_known_spells_00649520(stats_of(creature), nullptr);        // 006E8818..006E881F
    U8 index = 0;                                                                              // 006E882F..006E8832
    if (recovered_save_stats::spell_count_006487d0(snapshot_stats(self), cls, 0) == 0) return; // 006E8824..006E883E
    do {
        void* target = stats_of(creature);                                                     // 006E885D ebp
        const U32 spell = kotor_fastcall_006495a0(snapshot_stats(self), nullptr, cls, 0, index); // 006E8850..006E8867
        recovered_creature_stats_add_known_spell_00649f90(target, nullptr, cls, spell);        // 006E886C..006E8870
        ++index;                                                                               // 006E8880
    } while (static_cast<U16>(index) < recovered_save_stats::spell_count_006487d0(snapshot_stats(self), cls, 0)); // 006E8875..006E8893
}

// 006E88A0 BeginPlayGame (Accept): send the level-up to the server and close.
void __fastcall kotor_fastcall_006e88a0(void* self, void*) {
    kotor_fastcall_0040be00(manager_of(self), nullptr);                                        // 006E88A3..006E88A7 PopModalPanel
    hide_flags(lp(self, 0x6C));                                                                // 006E88AC..006E88BE
    void* creature = live_creature(self);                                                      // 006E88C1..006E88D6 (unchecked)
    kotor_m15b_creature_stats_get_class_00647730(stats_of(creature), nullptr,
        static_cast<U8>(lb(stats_of(creature), 0x32) - 1));                                    // 006E88D8..006E88E6 (result unused)
    void* message = kotor_fastcall_005ed6f0(client_app(), nullptr);                            // 006E88EB..006E88F3
    kotor_fastcall_00678280(message, nullptr, snapshot_stats(self), stats_of(creature));       // 006E88F8..006E890B
    sw(creature, 0x440, lw(creature, 0x440) | 2u);                                             // 006E8910
    kotor_fastcall_0040be00(manager_of(self), nullptr);                                        // 006E8917..006E891A
    hide_flags(self);                                                                          // 006E891F..006E8931
    kotor_fastcall_006aebf0(lp(self, 0x68), nullptr);                                          // 006E8922..006E8934 AttachModel
    if (lw(self, 0x2544) == 0 && kotor_fastcall_005edc10(client_app(), nullptr) != 0)          // 006E8939..006E8952
        kotor_fastcall_005edc20(client_app(), nullptr, 0, 0, 0);                               // 006E8954..006E8963
    if (w4::ingame_gui_single_panel_007a2370 == 0 && lw(self, 0x2544) != 2) {                  // 006E8968..006E8978
        hide_flags(lp(self, 0x68));                                                            // 006E897A..006E898C
        sw(ingame_gui(), 0x14, 0);                                                             // 006E898F..006E899C
        frame_R4_005d5e80(pt(frame_control::sound_manager_007a39ec), nullptr, kotor_rdata_sound_mode_0074c5ec); // 006E89A3..006E89B0
    }
    gui_m48_client_set_input_class_005eda60(client_app(), nullptr, lw(self, 0x2544), 1);       // 006E89B5..006E89C6
}

// 006E89F0 CopyCreatureStats(from, to) RET 8 -> 1. Copies the client stats of `from` into `to` (ECX unused).
w4::U32 __fastcall kotor_fastcall_006e89f0(void*, void*, void* from, void* to) {
    void* dst = stats_of(to);                                                                  // 006E89F4..006E89FE esi
    void* src = stats_of(from);                                                                // 006E89F0..006E8A05 edi
    async_assign_string_005e5c50(at(dst, 0x14), nullptr, at(src, 0x14));                       // 006E8A0B..006E8A12 first name
    async_assign_string_005e5c50(at(dst, 0x1C), nullptr, at(src, 0x1C));                       // 006E8A17..006E8A1E last name
    {
        U8 portrait[16];                                                                       // [esp+14]
        U8 discarded[16];                                                                      // [esp+24]
        void* r = kotor_m15e_creature_stats_get_portrait_00647e80(src, nullptr, portrait, 0);  // 006E8A23..006E8A2C
        ref_assign_copy_00406120(at(dst, 0x70), nullptr, discarded, r);                        // 006E8A31..006E8A3A
    }
    sw(dst, 0x58, lw(src, 0x58));                                                              // 006E8A3F..006E8A42 experience
    sb(dst, 0x33, lb(src, 0x33));                                                              // 006E8A45..006E8A48
    sb(dst, 0x31, lb(src, 0x31));                                                              // 006E8A4B..006E8A4E gender
    {
        const I32 align = s16(lh(src, 0x80));                                                  // 006E8A51..006E8A6E clamp 0..100
        sh(dst, 0x80, static_cast<U16>(align < 0 ? 0 : (align > 0x64 ? 0x64 : align)));
    }
    kotor_m15b_levelup_stats_set_str_base_0064a0d0(dst, nullptr, lb(src, 0x3A));               // 006E8A75..006E8A7D
    kotor_m15b_levelup_stats_set_dex_base_0064a100(dst, nullptr, lb(src, 0x3B), 1);            // 006E8A82..006E8A8C
    kotor_m15b_levelup_stats_set_con_base_006496d0(dst, nullptr, lb(src, 0x3C));               // 006E8A91..006E8A99
    kotor_m15b_levelup_stats_set_int_base_006478a0(dst, nullptr, lb(src, 0x3D));               // 006E8A9E..006E8AA6
    kotor_m15b_levelup_stats_set_wis_base_00649770(dst, nullptr, lb(src, 0x3E));               // 006E8AAB..006E8AB3
    kotor_m15b_levelup_stats_set_cha_base_006478d0(dst, nullptr, lb(src, 0x3F));               // 006E8AB8..006E8AC0
    sh(dst, 0x48, lh(src, 0x48));                                                              // 006E8AC5..006E8AC9
    sh(dst, 0x4C, lh(src, 0x4C));                                                              // 006E8ACD..006E8AD1
    sh(dst, 0x4E, recovered_save_stats::maximum_hp_00647a80(src));                             // 006E8AD5..006E8ADC
    sh(dst, 0x120, lh(src, 0x120));                                                            // 006E8AE0..006E8AE7
    sh(dst, 0x122, static_cast<U16>(lh(src, 0x124) + lh(src, 0x122)));                         // 006E8AEE..006E8AFC
    sh(dst, 0x11E, lh(src, 0x11E));                                                            // 006E8B03..006E8B0A
    U8 index = 0;                                                                              // 006E8B14..006E8B18 [esp+10]
    if (lb(src, 0x32) != 0) {                                                                  // 006E8B11..006E8B1C (jbe)
        do {
            const U8 next = static_cast<U8>(index + 1);                                        // 006E8B26 [esp+3C]
            sb(dst, 0x32, next);                                                               // 006E8B2F class count
            const U8 cls = kotor_m15b_creature_stats_get_class_00647730(src, nullptr, index);  // 006E8B28..006E8B32
            kotor_fastcall_0064a1f0(dst, nullptr, index, cls);                                 // 006E8B37..006E8B3B SetClass
            const U8 level = kotor_m15b_creature_stats_get_class_level_00647750(src, nullptr, index); // 006E8B40..006E8B43
            kotor_fastcall_0064a2a0(dst, nullptr, index, level);                               // 006E8B48..006E8B4C SetClassLevel
            kotor_fastcall_00649580(dst, nullptr, index);                                      // 006E8B51..006E8B54 ClearKnownSpells
            U8 spell = 0;                                                                      // 006E8B5B..006E8B60 [esp+38]
            if (recovered_save_stats::spell_count_006487d0(src, index, 0) != 0) {              // 006E8B59..006E8B6C
                do {
                    const U32 id = kotor_fastcall_006495a0(src, nullptr, index, 0, spell);     // 006E8B70..006E8B7A
                    recovered_creature_stats_add_known_spell_00649f90(dst, nullptr, index, id); // 006E8B7F..006E8B83
                    ++spell;                                                                   // 006E8B8A
                } while (static_cast<U16>(spell) < recovered_save_stats::spell_count_006487d0(src, index, 0)); // 006E8B88..006E8B9F
            }
            index = next;                                                                      // 006E8BA1..006E8BA8
        } while (index < lb(src, 0x32));                                                       // 006E8BA5..006E8BAC (jb)
    }
    {
        const U16 race = lh(src, 0x24);                                                        // 006E8BB2
        sh(dst, 0x24, race);                                                                   // 006E8BBD
        async_assign_string_005e5c50(at(dst, 0x28), nullptr, at(src, 0x28));                   // 006E8BB6..006E8BC1 subrace
    }
    sb(dst, 0x30, lb(src, 0x30));                                                              // 006E8BC6..006E8BC9
    kotor_m15b_levelup_stats_set_fort_saving_throw_00648b30(dst, nullptr);                     // 006E8BCC..006E8BCE
    kotor_m15b_levelup_stats_set_will_saving_throw_00648be0(dst, nullptr);                     // 006E8BD3..006E8BD5
    kotor_m15b_levelup_stats_set_reflex_saving_throw_00648c90(dst, nullptr);                   // 006E8BDA..006E8BDC
    for (U32 n = 4; n != 0; --n)                                                               // 006E8BE1..006E8BF1 (four calls, quirk)
        sb(dst, 0x64, static_cast<U8>(kotor_m15b_levelup_stats_get_base_attack_bonus_006479f0(src, nullptr)));
    {
        const I32 align = s16(lh(src, 0x80));                                                  // 006E8BF3..006E8C10 clamp again
        sh(dst, 0x80, static_cast<U16>(align < 0 ? 0 : (align > 0x64 ? 0x64 : align)));
    }
    kotor_m15b_creature_stats_clear_skills_006475e0(dst, nullptr);                             // 006E8C10..006E8C19
    sh(dst, 0xAE, lh(src, 0xAE));                                                              // 006E8C1E..006E8C25
    {
        U8 count = lb(rules(), 0xAB);                                                          // 006E8C2C..006E8C31
        for (U8 skill = 0; skill < count;) {                                                   // 006E8C37..006E8C3B
            U8 rank = 0;                                                                       // 006E8C4D
            if (static_cast<U16>(skill) < static_cast<U16>(count)) rank = lb(lp(src, 0x88), skill); // 006E8C40..006E8C60
            kotor_m15b_levelup_stats_set_skill_rank_base_006488e0(dst, nullptr, skill, rank);  // 006E8C64..006E8C6C
            count = lb(rules(), 0xAB);                                                         // 006E8C71..006E8C76
            ++skill;                                                                           // 006E8C7C..006E8C80
        }
    }
    kotor_m15b_creature_stats_clear_feats_00648320(dst, nullptr);                              // 006E8C82..006E8C84
    U32 feat_index = 0;                                                                        // 006E8C91 [esp+3C]
    if (lh(src, 0xB4) != 0) {                                                                  // 006E8C89..006E8C99
        do {
            const U16 feat = static_cast<U16>(kotor_m15b_creature_stats_get_feat_006486e0(src, nullptr, static_cast<U16>(feat_index))); // 006E8CA0..006E8CA7 ebp
            I32 count = li(dst, 0xB4);                                                         // 006E8CAC
            I32 found = 0;                                                                     // 006E8CB2 ebx
            if (count > 0) {                                                                   // 006E8CB4..006E8CB8 (signed jle)
                const U8* data = static_cast<const U8*>(lp(dst, 0xB0));                        // 006E8CBA
                for (I32 n = count; n != 0; --n, data += 2)                                    // 006E8CC2..006E8CCC
                    if (lh(data, 0) == feat) ++found;
            }
            if (count <= 0 || found == 0) {                                                    // 006E8CCE..006E8CD0
                if (count == li(dst, 0xB8)) {                                                  // 006E8CD6..006E8CDE inlined Add: grow
                    const I32 capacity = li(dst, 0xB8) != 0 ? li(dst, 0xB8) * 2 : 0x10;        // 006E8CE0..006E8CE8
                    void* old = lp(dst, 0xB0);                                                 // 006E8CED ebx
                    sw(dst, 0xB8, static_cast<U32>(capacity));                                 // 006E8CF7
                    sw(dst, 0xB0, ad(live_operator_new(static_cast<std::size_t>(capacity) * 2u))); // 006E8CF3..006E8D08
                    for (I32 i = 0; i < li(dst, 0xB4); ++i)                                    // 006E8D11..006E8D37 (count re-read)
                        sh(lp(dst, 0xB0), static_cast<U32>(i) * 2u, lh(old, static_cast<U32>(i) * 2u));
                    if (old != nullptr) live_free(old);                                        // 006E8D39..006E8D3E
                }
                sh(lp(dst, 0xB0), lw(dst, 0xB4) * 2u, feat);                                   // 006E8D46..006E8D52
                sw(dst, 0xB4, lw(dst, 0xB4) + 1u);                                             // 006E8D56
            }
            ++feat_index;                                                                      // 006E8D5C..006E8D60
        } while (static_cast<U16>(feat_index) < lh(src, 0xB4));                                // 006E8D61..006E8D6C (word compare, jb)
    }
    sb(dst, 0xAC, lb(src, 0xAC));                                                              // 006E8D72..006E8D7C
    sh(dst, 0x50, static_cast<U16>(kotor_m15b_creature_stats_get_armor_class_00647720(src, nullptr, 0))); // 006E8D78..006E8D8B
    sh(dst, 0x50, static_cast<U16>(kotor_m15b_creature_stats_get_armor_class_00647720(src, nullptr, 0))); // 006E8D87..006E8D95 (twice, quirk)
    return 1;                                                                                  // 006E8D9B
}

// 006E8DB0 ClearFeats: clear the live creature's feats, then AddUnique every snapshot feat.
void __fastcall kotor_fastcall_006e8db0(void* self, void*) {
    void* creature = live_creature(self);                                                      // 006E8DB0..006E8DC3
    if (creature == nullptr) return;                                                           // 006E8DC8..006E8DCC
    kotor_m15b_creature_stats_clear_feats_00648320(stats_of(creature), nullptr);               // 006E8DCE..006E8DD5
    U32 index = 0;                                                                             // 006E8DE3
    if (lh(snapshot_stats(self), 0xB4) == 0) return;                                           // 006E8DDA..006E8DEC (jbe)
    do {
        void* target = stats_of(creature);                                                     // 006E8DF0 ebp
        const U32 feat = kotor_m15b_creature_stats_get_feat_006486e0(snapshot_stats(self), nullptr, static_cast<U16>(index)); // 006E8DF6..006E8DF7
        kotor_m15_array_u16_add_unique_005a7670(at(target, 0xB0), nullptr, feat);              // 006E8DFC..006E8E03
        ++index;                                                                               // 006E8E11
    } while (static_cast<U16>(index) < lh(snapshot_stats(self), 0xB4));                        // 006E8E08..006E8E19
}

// 006E8E20 CancelLevelUp: restore the live creature from the snapshot and close (no-op while [this+2544h] == 0).
void __fastcall kotor_fastcall_006e8e20(void* self, void*) {
    if (lw(self, 0x2544) == 0) return;                                                         // 006E8E23..006E8E2B
    void* creature = live_creature(self);                                                      // 006E8E31..006E8E40
    kotor_fastcall_006e89f0(self, nullptr, lp(self, 0x64), creature);                          // 006E8E45..006E8E4C
    kotor_fastcall_0040be00(manager_of(self), nullptr);                                        // 006E8E51..006E8E54
    kotor_fastcall_0040be00(manager_of(self), nullptr);                                        // 006E8E59..006E8E5C
    hide_flags(lp(self, 0x6C));                                                                // 006E8E61..006E8E73
    hide_flags(self);                                                                          // 006E8E76..006E8E88
    kotor_fastcall_006aebf0(lp(self, 0x68), nullptr);                                          // 006E8E79..006E8E8B AttachModel
    if (w4::ingame_gui_single_panel_007a2370 == 0 && lw(self, 0x2544) != 2) {                  // 006E8E90..006E8EA0
        hide_flags(lp(self, 0x68));                                                            // 006E8EA2..006E8EB4
        sw(ingame_gui(), 0x14, 0);                                                             // 006E8EB7..006E8EC5
    }
    gui_m48_client_set_input_class_005eda60(client_app(), nullptr, lw(self, 0x2544), 1);       // 006E8ECC..006E8EDE
}

// ======================= 006E8EF0 CSWGuiLevelUpCharGen(manager, creature_id, preview) RET 0Ch =======================
void* __fastcall kotor_fastcall_006e8ef0(void* self, void*, void* manager, w4::U32 creature_id, void* preview) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                 // 006E8F0E..006E8F1B
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);                                 // state 0
    set_vptr(self, vt_levelup_chargen());                                                      // 006E8F2B (00758F50)
    for (U32 i = 0; i < 28; ++i) label(self, 0x70 + i * 0x140, u);                             // 006E8F20..006E90E1 states 1..1Ch
    void* view = at(self, 0x2370);                                                             // 006E90E6..006E910A CSWGui3DSceneView (inlined)
    gui_load_control_construct_0041aa80(view, nullptr);                                        // 006E90F7 state 1Dh
    set_vptr(view, gui_load_scene_view_methods_00752e30);                                      // 006E9104 (00752E30)
    { auto g = on_throw([&]() { gui_load_control_destruct_00419a20(view, nullptr); });
      gui_scene_construct_004174b0(at(view, 0x5C), nullptr); g.armed = false; }                // 006E90FC..006E910A
    u.push(&gui_load_scene_view_destruct_0067ac50, view);                                      // state 1Eh
    label(self, 0x2400, u);                                                                    // 006E910F..006E911A state 1Fh
    exo_default(at(self, 0x2548)); u.push(&frame_LogStringDtor_005e5c20, at(self, 0x2548));    // 006E911F..006E912A state 20h
    exo_default(at(self, 0x2550)); u.push(&frame_LogStringDtor_005e5c20, at(self, 0x2550));    // 006E912F..006E913A state 21h
    void* client = client_app();                                                               // 006E913F..006E9145 edi
    void* source = pt(frame_R4_005ed5b0(client, nullptr, creature_id));                        // 006E9148..006E915B [esp+10]
    sw(self, 0x18, ad(kotor_fastcall_005eda40(client, nullptr)));                              // 006E915F..006E9169 GetSWGuiManager
    {
        void* raw = live_operator_new(0x44C);                                                  // 006E9164..006E916C
        void* creature = nullptr;
        if (raw != nullptr) {                                                                  // 006E9178..006E917F state 22h
            auto g = on_throw([&]() { live_free(raw); });
            creature = kotor_m15a_creature_construct_00616a20(raw, nullptr);                   // 006E9181..006E9183
            g.armed = false;
        }
        sw(self, 0x255C, creature_id);                                                         // 006E9194
        sw(self, 0x64, ad(creature));                                                          // 006E91A1
        kotor_fastcall_006e89f0(self, nullptr, source, creature);                              // 006E918C..006E91A4 CopyCreatureStats
    }
    if (void* live = live_creature(self)) {                                                    // 006E91A9..006E91BF
        void* stats = stats_of(live);                                                          // 006E91C1 edi
        const U8 index = static_cast<U8>(lb(stats, 0x32) - 1);                                 // 006E91C7..006E91CC [esp+14]
        const U8 level = kotor_m15b_creature_stats_get_class_level_00647750(stats, nullptr, index); // 006E91D0..006E91D7
        kotor_fastcall_0064a2a0(stats, nullptr, index, static_cast<U8>(level + 1));            // 006E91DC..006E91E6 SetClassLevel
    }
    kotor_fastcall_006e7dd0(self, nullptr);                                                    // 006E91EB..006E91ED DetermineClass
    start_layout(self, "MAINCG");                                                              // 006E91F2..006E9207
    static const char* const names[28] = {
        "MAIN_TITLE_LBL", "LBL_NAME", "LBL_CLASS", "LBL_LEVEL", "LBL_LEVEL_VAL", "STR_LBL", "DEX_LBL", "CON_LBL",
        "INT_LBL", "WIS_LBL", "CHA_LBL", "STR_AB_LBL", "DEX_AB_LBL", "CON_AB_LBL", "INT_AB_LBL", "WIS_AB_LBL",
        "CHA_AB_LBL", "LBL_VIT", "LBL_DEF", "NEW_WILL_LBL", "NEW_REFL_LBL", "NEW_FORT_LBL", "LBL_BEVEL_L",
        "LBL_BEVEL_M", "LBL_BEVEL_R", "LBL_FORTITUDE", "LBL_REFLEX", "LBL_WILL"};
    for (U32 i = 0; i < 28; ++i) init_control(self, 0x70 + i * 0x140, names[i]);              // 006E920C..006E97D0 states 23h..3Eh
    init_control(self, 0x2370, "MODEL_LBL");                                                   // 006E97D5..006E9805 state 3Fh
    init_control(self, 0x2400, "PORTRAIT_LBL");                                                // 006E980A..006E983A state 40h
    stop_layout(self);                                                                         // 006E983F..006E9841
    gui_load_text_params_set_strref_00415e50(at(self, 0x158), nullptr, 0x42F);                 // 006E9846..006E9851 MAIN_TITLE_LBL
    {
        void* raw = live_operator_new(0x1CEC);                                                 // 006E9856..006E985B
        void* panel = nullptr;
        if (raw != nullptr) {                                                                  // 006E9863..006E986E state 41h
            auto g = on_throw([&]() { live_free(raw); });
            panel = kotor_fastcall_006ee7d0(raw, nullptr, manager_of(self), lw(self, 0x255C)); // 006E9870..006E987D
            g.armed = false;
        }
        sw(self, 0x6C, ad(panel));                                                             // 006E9886
        raw_store(panel, 0x68, ad(self));                                                      // 006E9889 (unchecked, quirk)
    }
    {
        Str t;
        void* r = vcall<void*, void*>(lp(self, 0x64), 0x70, t.s);                              // 006E988C..006E989A by-value name
        t.live = true;                                                                         // state 42h
        kotor_fastcall_006e7ec0(self, nullptr, r);                                             // 006E989D..006E98A5 SetNameString
    }                                                                                          // 006E98AA..006E98B2
    kotor_fastcall_006e7f70(self, nullptr);                                                    // 006E98B7..006E98B9 AbilitiesChanged
    {
        menu_resources::Ref ref;                                                               // [esp+30]
        void* r = vcall<void*, void*, U32>(source, 0x148, &ref, 1);                            // 006E98BE..006E98CB portrait
        gui_load_border_params_set_fill_00414c00(at(self, 0x2470), nullptr, r, 0);             // 006E98D1..006E98DA PORTRAIT_LBL
    }
    void* camera = lp(self, 0x23E4);                                                           // 006E98DF..006E98EB [esp+18]
    void* room = lp(self, 0x23E0);                                                             // 006E98E5..006E98F6 [esp+14]
    if (kotor_cell_do3dgui_0078d1e4 != 0) {                                                    // 006E98EF..006E98FA
        const float position[3] = {0.0f, 0.0f, 0.0f};                                          // [esp+24]
        const float orientation[4] = {0.0f, 0.0f, 0.0f, 1.0f};                                 // [esp+30]
        vcall<void, const char*, const void*, const void*>(room, 0x70, "gui3D_room", position, orientation); // 006E9900..006E9949
        {
            Str model("charrec_light");                                                        // 006E994C..006E9955 state 43h
            aurora_gui_scene_add_model_name_00417620(at(self, 0x23CC), nullptr, model.p(), -1); // 006E995A..006E996C (result unused)
        }                                                                                      // 006E9971..006E9979
        kotor_m15a_creature_setup_3d_scene_006100f0(preview, nullptr, view, source, 0, 1);     // 006E997E..006E998D
        vcall<void, void*>(preview, 0x94, room);                                               // 006E9992..006E999B
        const float origin[3] = {0.0f, 0.0f, 0.0f};                                            // 006E99AA..006E99BA [esp+24]
        vcall<void, const void*>(preview, 0x8C, origin);                                       // 006E99A1..006E99C2
        void* hook = vcall<void*, U32>(camera, 0x80, 0x3EA);                                   // 006E99C8..006E99D3
        if (hook != nullptr) {                                                                 // 006E99D9..006E99DD
            sw(hook, 0x48, 1);                                                                 // 006E99DF
            const float height = vcall<float>(preview, 0xD8);                                  // 006E99E6..006E99FA
            sw(hook, 0x3C, 0); sw(hook, 0x40, 0);                                              // 006E99EA..006E9A15
            sf(hook, 0x44, height);                                                            // 006E9A0D..006E9A18 fstp dword
        }
    }
    sw(self, 0x2544, gui_m48_client_get_input_class_005eda70(client_app(), nullptr));          // 006E9A1B..006E9A2D
    u.dismiss();
    return self;                                                                               // 006E9A34
}

// 006E9A50 HandleInputEvent(event, down) RET 8: 28h / 2Eh pressed close the level-up GUI without applying.
void __fastcall kotor_fastcall_006e9a50(void* self, void*, w4::U32 event, w4::U32 down) {
    if (down != 0 && (event == 0x28 || event == 0x2E)) {                                       // 006E9A5B..006E9A69
        play_sound(self, 0);                                                                   // 006E9A6B..006E9A70
        kotor_fastcall_0040be00(manager_of(self), nullptr);                                    // 006E9A75..006E9A78
        hide_flags(lp(self, 0x6C));                                                            // 006E9A7D..006E9A8F
        hide_flags(self);                                                                      // 006E9A92..006E9AA4
        kotor_fastcall_006aebf0(lp(self, 0x68), nullptr);                                      // 006E9A95..006E9AA7 AttachModel
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, down);                          // 006E9AAC..006E9AB0
}
}  // extern "C"
