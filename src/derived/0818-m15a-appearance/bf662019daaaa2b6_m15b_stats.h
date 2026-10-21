#pragma once
// lane-m15-b (L15-B, Milestone 1.5): creature stats / rules / 2DA readers.
// Original: swkotor.exe GOG K1 (image base 0x400000), static disassembly only.
// Consumers (L15-A/C/D/E) include this header (header root: claude-takeover\lane-m15-b\src) or copy
// the declarations verbatim. Every function below is DEFINED by an L15-B unit (see PACKET.json):
//   m15b-rules-leaves (m15b_rules_leaves.cpp): CSWClass/CSWRace/CSWFeat/CSWSkill/CSWPortrait accessors,
//                      CSWSpellArray/CSWBaseItemArray/CSWItem GetBaseItem, C2DA::GetFLOATEntry(int,int)
//   m15b-stats        (m15b_stats.cpp): CSWCCreatureStats, CSWCCreatureStats_ClassInfo, CSWCLevelUpStats,
//                      typed vtables 00751C60 / 00751C64
//   m15b-combat-dtor  (m15b_combat_dtor.cpp): CCombatInformation::~CCombatInformation 00550E00
//
// Conventions (same as lane-m15-0; DECISIONS D1/D21/D24): every original __thiscall entry is an
// extern "C" __fastcall(Self*, void* unused_edx, <original stack words>) veneer with identical callee
// cleanup (RET N noted per entry); callers pass nullptr for EDX; no callee reads EDX. Object pointers
// are void* (C linkage: the mangled name is @name@<bytes>). By-value CExoString returns use the
// original hidden-return pointer as the first stack argument and return it in EAX.
// Return widths: where the original produces a full zero-extended EAX (movzx/setcc/mov eax,1) the
// return type is 32-bit; where it only defines AL (ability modifiers, byte getters) it is 8-bit.
#include <cstdint>

// ------------------------------------------------------------------------------------------------
// Observed layouts (byte offsets read/written by the L15-B bodies; names = Ghidra placeholder names
// where they agree with the observed use, otherwise descriptive; unknown regions are not named).
// ------------------------------------------------------------------------------------------------
namespace kotor_m15b_layout {
// CSWRules (g_pRules = [007A3A28], cell kotor_cell_rules_007a3a28)
constexpr unsigned kRulesBaseItems    = 0x34;   // CSWBaseItemArray* (005B4793)
constexpr unsigned kRulesFeatCount    = 0xA4;   // WORD (0064996C)
constexpr unsigned kRulesSkillCount   = 0xAB;   // BYTE (006475E7)
constexpr unsigned kRulesClasses      = 0xAC;   // CSWClass[] stride 0x1A0 (00647849)
constexpr unsigned kRulesRaces        = 0xB0;   // CSWRace[] stride 0x34 (0064B335)
constexpr unsigned kRulesSkills       = 0xB4;   // CSWSkill[] stride 0x20 (00647808)
constexpr unsigned kRules2DA          = 0xB8;   // CTwoDimArrays* (+0C = C2DA used by 00649DF1)
constexpr unsigned kClassStride = 0x1A0, kRaceStride = 0x34, kSkillStride = 0x20, kFeatStride = 0x48;

// CSWCCreatureStats (0x128; vptr 00751C60). ctor 0064AFC0 / dtor 00649E40.
constexpr unsigned kStatsVptr          = 0x00;
constexpr unsigned kStatsResRef        = 0x04;  // CResRef (16)
constexpr unsigned kStatsFirstName     = 0x14;  // CExoString
constexpr unsigned kStatsLastName      = 0x1C;  // CExoString
constexpr unsigned kStatsRace          = 0x24;  // WORD (ctor: [0074BF14] = 6)
constexpr unsigned kStatsSubrace       = 0x28;  // CExoString
constexpr unsigned kStatsSubraceIndex  = 0x30;  // BYTE
constexpr unsigned kStatsGender        = 0x31;  // BYTE
constexpr unsigned kStatsClassCount    = 0x32;  // BYTE
constexpr unsigned kStatsByte33        = 0x33;  // BYTE (ctor 0)
constexpr unsigned kStatsStr = 0x34, kStatsDex = 0x35, kStatsCon = 0x36,           // BYTE totals (base + race adjust)
                   kStatsInt = 0x37, kStatsWis = 0x38, kStatsCha = 0x39;
constexpr unsigned kStatsStrBase = 0x3A, kStatsDexBase = 0x3B, kStatsConBase = 0x3C, // BYTE bases (Set*Base)
                   kStatsIntBase = 0x3D, kStatsWisBase = 0x3E, kStatsChaBase = 0x3F;
constexpr unsigned kStatsDexBonus      = 0x40;  // BYTE added to the DEX modifier (00648180, 0064A12A)
constexpr unsigned kStatsHitPointsBase = 0x48;  // WORD (006496D0 reads)
constexpr unsigned kStatsCurrentHp     = 0x4C;  // WORD (006496D0 writes)
constexpr unsigned kStatsMaxHp         = 0x4E;  // WORD (006496D0 writes)
constexpr unsigned kStatsArmorClass    = 0x50;  // WORD (00647720, 0064A100)
constexpr unsigned kStatsFloat54       = 0x54;  // float (ctor 6.0f)
constexpr unsigned kStatsExperience    = 0x58;  // DWORD (006496D0 / 00649950 test == 0)
constexpr unsigned kStatsFortSave = 0x5C, kStatsWillSave = 0x5D, kStatsReflexSave = 0x5E; // BYTE
constexpr unsigned kStatsCombatInfo    = 0x68;  // CCombatInformation* (0x48 allocation)
constexpr unsigned kStatsPortrait      = 0x70;  // CSWPortrait (CResRef 16)
constexpr unsigned kStatsAlignment     = 0x80;  // WORD (ctor 50)
constexpr unsigned kStatsSkillRanksMod = 0x84;  // BYTE[skill count] (base + ability modifier)
constexpr unsigned kStatsSkillRanks    = 0x88;  // BYTE[skill count] (base ranks)
constexpr unsigned kStatsAppearance    = 0x8C;  // WORD (ctor = race word)
constexpr unsigned kStatsDescription   = 0xA0;  // CExoString
constexpr unsigned kStatsAge           = 0xA8;  // DWORD
constexpr unsigned kStatsAbilityPoints = 0xAC;  // BYTE (LevelUp ctor 30)
constexpr unsigned kStatsSkillPoints   = 0xAE;  // WORD
constexpr unsigned kStatsFeats         = 0xB0;  // CExoArrayList<WORD> {data, count, capacity}
constexpr unsigned kStatsBonusFeats    = 0xBC;  // CExoArrayList<WORD>
constexpr unsigned kStatsPtrList       = 0xC8;  // CExoArrayList<void*> (elements freed by ClearFeats)
constexpr unsigned kStatsClassInfo     = 0xD4;  // CSWCCreatureStats_ClassInfo[2], stride 0x20
constexpr unsigned kStatsDeity         = 0x114; // CExoString
constexpr unsigned kStatsPortraitId    = 0x11C; // WORD (ctor FFFF)
constexpr unsigned kStatsSize          = 0x128;
// CSWCCreatureStats_ClassInfo (0x20): +0 CExoArrayList<DWORD> known spells, +0C CExoArrayList<DWORD>,
// +18..+1B bytes (ctor 0,0,FF,FF), +1C class id (BYTE), +1D level (BYTE).
constexpr unsigned kClassInfoStride = 0x20, kClassInfoClass = 0x1C, kClassInfoLevel = 0x1D;
// CSWCLevelUpStats (0x134; vptr 00751C64) = CSWCCreatureStats + CExoArrayList<CResRef(16)> at 0x128.
constexpr unsigned kLevelUpRefList = 0x128, kLevelUpSize = 0x134;

// CSWClass (0x1A0) fields read here: +00 name strref; +17+lvl attack bonus; +56 skill point base,
// +56+lvl fort; +92+lvl will; +CE+lvl reflex; +10C BYTE* AC table; +110 BYTE** spells known,
// +113+lvl spells-known level count; +13B+lvl / +14F+lvl feat counts (CalcNumberFeats);
// +164 skill entries {WORD skill; DWORD class_skill @+4; DWORD priority @+8} (12 bytes), +168 WORD count;
// +16C feat entries {WORD feat; BYTE level @+2; BYTE list type @+3; DWORD priority @+4} (8 bytes),
// +170 WORD count; +19C DWORD spell caster.
// CSWFeat (0x48): +08 name strref; +2C min BAB; +2D min level; +2E/+2F/+30/+31 min STR/DEX/INT/WIS;
// +32 min spell level; +38/+3A prerequisite feats; +3C..+45 five "any of" feats; +46 required skill.
// CSWSkill (0x20): +04 description strref; +18 key ability (0..5); +1C flags.
// CSWRace (0x34): +18 STR, +19 DEX, +1A INT, +1B CHA, +1C WIS, +1D CON adjust; +24 WORD feat count,
// +28 WORD* first-level feats; +30 WORD row used with column [007A2124].
}  // namespace kotor_m15b_layout

extern "C" {
// ------------------------------------------------------------------------------------------------
// Typed vtables (D21/D24: typed constant tables of D1 fastcall slots; never raw 00751C60/00751C64).
// Slot 0 = scalar deleting destructor (RET 4): flags bit0 -> free the block (006FA390). EAX = this.
// ------------------------------------------------------------------------------------------------
struct KotorM15bStatsMethods {
    void* (__fastcall* scalar_deleting_destructor)(void* self, void* unused_edx, std::uint32_t flags);
};
extern const KotorM15bStatsMethods kotor_m15b_creature_stats_vtable_00751c60;  // CSWCCreatureStats
extern const KotorM15bStatsMethods kotor_m15b_levelup_stats_vtable_00751c64;   // CSWCLevelUpStats

// ================================================================ m15b-rules-leaves
// 00413350 C2DA::GetFLOATEntry(int row, int column, float* out) RET 0Ch -> 1 found / 0.
std::int32_t __fastcall kotor_m15b_c2da_get_float_entry_00413350(void* table, void*, std::int32_t row,
                                                                 std::int32_t column, float* out) noexcept(false);
// 0059B6D0 CSWSpellArray::GetSpell(int id) RET 4 -> element (stride 198h) or 0.
void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void* self, void*, std::int32_t id);
// 005B31A0 CSWBaseItemArray::GetBaseItem(int id) RET 4 -> element (stride C8h) or 0.
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* self, void*, std::int32_t id);
// 005B4790 CSWItem::GetBaseItem() RET 0 -> rules base item [this+0Ch] or 0.
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void* self, void*);
// CSWClass (ECX = class element)
std::uint8_t __fastcall kotor_m15b_class_get_attack_bonus_005bccb0(void* cls, void*, std::uint8_t level);     // RET 4
std::uint8_t __fastcall kotor_m15b_class_get_fort_save_bonus_005bccd0(void* cls, void*, std::uint8_t level);  // RET 4
std::uint8_t __fastcall kotor_m15b_class_get_ref_save_bonus_005bccf0(void* cls, void*, std::uint8_t level);   // RET 4
std::uint8_t __fastcall kotor_m15b_class_get_will_save_bonus_005bcd10(void* cls, void*, std::uint8_t level);  // RET 4
// 005BCD60 GetSpellsKnownPerLevel(level, spell_level, class_id, race, cha) RET 14h (last three unused).
std::uint8_t __fastcall kotor_m15b_class_get_spells_known_per_level_005bcd60(void* cls, void*, std::uint8_t level,
    std::uint8_t spell_level, std::uint8_t class_id, std::uint16_t race, std::uint8_t cha);
std::int32_t __fastcall kotor_m15b_class_is_skill_class_skill_005be010(void* cls, void*, std::uint16_t skill); // RET 4
std::int32_t __fastcall kotor_m15b_class_is_skill_useable_005be070(void* cls, void*, std::uint16_t skill);    // RET 4
// 005BE0E0 CSWClass::GetNameText() -> CExoString by value (hidden return; RET 4).
void* __fastcall kotor_m15b_class_get_name_text_005be0e0(void* cls, void*, void* out_string) noexcept(false);
std::uint8_t __fastcall kotor_m15b_class_get_level_feat_granted_005be220(void* cls, void*, std::uint16_t feat); // RET 4, FF = none
std::int32_t __fastcall kotor_m15b_class_is_bonus_feat_005be270(void* cls, void*, std::uint16_t feat);    // RET 4
std::int32_t __fastcall kotor_m15b_class_is_normal_feat_005be2c0(void* cls, void*, std::uint16_t feat);   // RET 4
// 005BE310 IsGrantedFeat(feat, BYTE* level_out) RET 8 (*level_out = FF first).
std::int32_t __fastcall kotor_m15b_class_is_granted_feat_005be310(void* cls, void*, std::uint16_t feat, std::uint8_t* level_out);
std::int32_t __fastcall kotor_m15b_class_is_feat_useable_005be370(void* cls, void*, std::uint16_t feat);   // RET 4
std::int32_t __fastcall kotor_m15b_class_get_recommended_feat_005be3c0(void* cls, void*, std::int32_t rank);  // RET 4, -1 none
std::int32_t __fastcall kotor_m15b_class_get_recommended_skill_005be430(void* cls, void*, std::int32_t rank); // RET 4, -1 none
std::uint8_t __fastcall kotor_m15b_class_get_armor_bonus_005be770(void* cls, void*, std::int32_t level);   // RET 4
// 005C7CC0 CSWRace::IsFirstLevelGrantedFeat(feat) RET 4.
std::int32_t __fastcall kotor_m15b_race_is_first_level_granted_feat_005c7cc0(void* race, void*, std::uint16_t feat);
// 005CD760 CSWFeat::GetNameText() / 005CD970 CSWSkill::GetDescriptionText() -> CExoString (hidden return; RET 4).
void* __fastcall kotor_m15b_feat_get_name_text_005cd760(void* feat, void*, void* out_string) noexcept(false);
void* __fastcall kotor_m15b_skill_get_description_text_005cd970(void* skill, void*, void* out_string) noexcept(false);
// 00678EC0 CSWPortrait::CSWPortrait() RET 0, EAX = this.
void* __fastcall kotor_m15b_portrait_construct_00678ec0(void* self, void*) noexcept(false);

// ================================================================ m15b-stats
// CSWCCreatureStats (ECX = stats)
void* __fastcall kotor_m15b_creature_stats_construct_0064afc0(void* self, void*) noexcept(false);   // RET 0, EAX=this
void  __fastcall kotor_m15b_creature_stats_destruct_00649e40(void* self, void*) noexcept(false);    // RET 0
void* __fastcall kotor_m15b_creature_stats_deleting_destruct_0064b2b0(void* self, void*, std::uint32_t flags) noexcept(false); // RET 4
void  __fastcall kotor_m15b_creature_stats_clear_skills_006475e0(void* self, void*);
// 00647630 (Ghidra "GetWISMod"): ability modifier of the BYTE argument = floor(v*0.5) - 5. RET 4.
std::int8_t  __fastcall kotor_m15b_creature_stats_ability_mod_00647630(void* self, void*, std::uint8_t value);
std::int16_t __fastcall kotor_m15b_creature_stats_get_armor_class_00647720(void* self, void*, std::uint32_t unused); // RET 4
std::uint8_t __fastcall kotor_m15b_creature_stats_get_class_00647730(void* self, void*, std::uint8_t index);       // RET 4, FF
std::uint8_t __fastcall kotor_m15b_creature_stats_get_class_level_00647750(void* self, void*, std::uint8_t index); // RET 4
void __fastcall kotor_m15b_creature_stats_set_class_00647770(void* self, void*, std::uint8_t index, std::uint8_t class_id); // RET 8
std::int32_t __fastcall kotor_m15b_creature_stats_get_can_use_skill_006477e0(void* self, void*, std::uint16_t skill); // RET 4
std::int32_t __fastcall kotor_m15b_creature_stats_calculate_num_attacks_00647ae0(void* self, void*);
void* __fastcall kotor_m15b_creature_stats_get_full_name_00647ce0(void* self, void*, void* out_string) noexcept(false); // RET 4
std::int8_t __fastcall kotor_m15b_creature_stats_get_str_mod_00648150(void* self, void*);
std::int8_t __fastcall kotor_m15b_creature_stats_get_dex_mod_00648180(void* self, void*);
std::int8_t __fastcall kotor_m15b_creature_stats_get_con_mod_006481c0(void* self, void*);
std::int8_t __fastcall kotor_m15b_creature_stats_get_int_mod_006481f0(void* self, void*);
std::int8_t __fastcall kotor_m15b_creature_stats_get_wis_mod_00648220(void* self, void*);
std::int8_t __fastcall kotor_m15b_creature_stats_get_cha_mod_00648250(void* self, void*);
std::int8_t __fastcall kotor_m15b_creature_stats_get_base_int_mod_00648280(void* self, void*);
std::int32_t __fastcall kotor_m15b_creature_stats_feat_acquired_006482b0(void* self, void*, std::uint16_t feat); // RET 4
void __fastcall kotor_m15b_creature_stats_clear_feats_00648320(void* self, void*);
// 00648470 GetClassesString(separator char) -> CExoString "Name (lvl)" entries (hidden return; RET 8).
void* __fastcall kotor_m15b_creature_stats_get_classes_string_00648470(void* self, void*, void* out_string,
                                                                        std::uint8_t separator) noexcept(false);
std::uint32_t __fastcall kotor_m15b_creature_stats_get_feat_006486e0(void* self, void*, std::uint16_t index); // RET 4, FFFF
std::int8_t __fastcall kotor_m15b_creature_stats_get_skill_modifier_00648830(void* self, void*, std::uint16_t skill); // RET 4
std::int32_t __fastcall kotor_m15b_creature_stats_has_feat_00649340(void* self, void*, std::uint16_t feat); // RET 4
void __fastcall kotor_m15b_creature_stats_update_ability_modified_skill_ranks_00649380(void* self, void*);
void __fastcall kotor_m15b_creature_stats_clear_known_spells_00649520(void* self, void*);
void __fastcall kotor_m15b_creature_stats_initialize_combat_information_00649d60(void* self, void*);
// CSWCCreatureStats_ClassInfo (ECX = element; also the 006FB55F/006FA852 element callbacks)
void* __fastcall kotor_m15b_class_info_construct_0064a050(void* self, void*) noexcept(false);
void  __fastcall kotor_m15b_class_info_destruct_006495f0(void* self, void*);
void  __fastcall kotor_m15b_class_info_clear_known_spells_006488b0(void* self, void*);
// CSWCLevelUpStats (ECX = level-up stats)
void* __fastcall kotor_m15b_levelup_stats_construct_0064b2d0(void* self, void*) noexcept(false);   // RET 0, EAX=this
void  __fastcall kotor_m15b_levelup_stats_destruct_0064b400(void* self, void*) noexcept(false);    // RET 0
void* __fastcall kotor_m15b_levelup_stats_deleting_destruct_0064b3e0(void* self, void*, std::uint32_t flags) noexcept(false); // RET 4
std::uint8_t __fastcall kotor_m15b_levelup_stats_get_skill_rank_0060b350(void* self, void*, std::uint16_t skill); // RET 4
void __fastcall kotor_m15b_levelup_stats_set_str_base_0064a0d0(void* self, void*, std::uint8_t value);  // RET 4
// 0064A100 SetDEXBase(value, keep_ac_delta) RET 8.
void __fastcall kotor_m15b_levelup_stats_set_dex_base_0064a100(void* self, void*, std::uint8_t value, std::int32_t keep_ac_delta);
void __fastcall kotor_m15b_levelup_stats_set_con_base_006496d0(void* self, void*, std::uint8_t value);  // RET 4
void __fastcall kotor_m15b_levelup_stats_set_int_base_006478a0(void* self, void*, std::uint8_t value);  // RET 4
void __fastcall kotor_m15b_levelup_stats_set_wis_base_00649770(void* self, void*, std::uint8_t value);  // RET 4
void __fastcall kotor_m15b_levelup_stats_set_cha_base_006478d0(void* self, void*, std::uint8_t value);  // RET 4
// 00647900 GetSkillPointCostForSkillRank(skill, increasing) RET 8 -> 1 class skill / 2 cross-class / 0 unusable;
// negated when increasing == 0.
std::int32_t __fastcall kotor_m15b_levelup_stats_get_skill_point_cost_00647900(void* self, void*, std::uint16_t skill, std::int32_t increasing);
std::uint32_t __fastcall kotor_m15b_levelup_stats_get_base_attack_bonus_006479f0(void* self, void*);
std::int32_t __fastcall kotor_m15b_levelup_stats_get_level_00648430(void* self, void*);
void __fastcall kotor_m15b_levelup_stats_set_skill_rank_base_006488e0(void* self, void*, std::uint16_t skill, std::uint8_t rank); // RET 8
void __fastcall kotor_m15b_levelup_stats_add_skill_points_per_level_00648a00(void* self, void*);
void __fastcall kotor_m15b_levelup_stats_set_fort_saving_throw_00648b30(void* self, void*);
void __fastcall kotor_m15b_levelup_stats_set_will_saving_throw_00648be0(void* self, void*);
void __fastcall kotor_m15b_levelup_stats_set_reflex_saving_throw_00648c90(void* self, void*);
std::int32_t __fastcall kotor_m15b_levelup_stats_increase_skill_rank_00648d50(void* self, void*, std::uint16_t skill); // RET 4
void __fastcall kotor_m15b_levelup_stats_decrease_skill_rank_00648eb0(void* self, void*, std::uint16_t skill, std::uint8_t floor_rank); // RET 8
// 00648FE0 CalcNumberFeats(BYTE* out_a = class[+14F+lvl], BYTE* out_b = class[+13B+lvl]) RET 8 (0 when lvl-1 >= 20).
void __fastcall kotor_m15b_levelup_stats_calc_number_feats_00648fe0(void* self, void*, std::uint8_t* out_a, std::uint8_t* out_b);
// 00649110 SetNormalBonusFlags(feat, int* normal, int* bonus) RET 0Ch.
void __fastcall kotor_m15b_levelup_stats_set_normal_bonus_flags_00649110(void* self, void*, std::uint16_t feat,
                                                                          std::int32_t* normal, std::int32_t* bonus);
void __fastcall kotor_m15b_levelup_stats_add_default_package_skills_006491a0(void* self, void*, std::uint8_t class_id,
                                                                              std::int32_t ignore_flags); // RET 8
// 00649260 FeatAcquired(feat, CExoArrayList<WORD>* pending) RET 8.
std::int32_t __fastcall kotor_m15b_levelup_stats_feat_acquired_00649260(void* self, void*, std::uint16_t feat, void* pending);
std::int32_t __fastcall kotor_m15b_levelup_stats_has_feat_prereq_006498a0(void* self, void*, void* feat, void* pending); // RET 8
// 00649950 AddGrantedFeats(CExoArrayList* added_or_null) RET 4 (00671C00 adds DWORD elements).
void __fastcall kotor_m15b_levelup_stats_add_granted_feats_00649950(void* self, void*, void* added);
void __fastcall kotor_m15b_levelup_stats_clear_package_00649a70(void* self, void*);
std::int32_t __fastcall kotor_m15b_levelup_stats_feat_requirements_met_0064a2e0(void* self, void*, std::uint16_t feat, void* pending); // RET 8
// 0064A4C0 CanChooseFeat(feat, normal_left, bonus_left, CExoArrayList<WORD>* pending) RET 10h.
std::int32_t __fastcall kotor_m15b_levelup_stats_can_choose_feat_0064a4c0(void* self, void*, std::uint16_t feat,
    std::uint8_t normal_left, std::uint8_t bonus_left, void* pending) noexcept(false);
// 0064A770 AddDefaultPackageFeats(class_id, CExoArrayList<WORD>* out, allow_a, allow_b) RET 10h -> remaining count.
std::uint8_t __fastcall kotor_m15b_levelup_stats_add_default_package_feats_0064a770(void* self, void*, std::uint8_t class_id,
    void* out_list, std::int32_t allow_a, std::int32_t allow_b) noexcept(false);

// ================================================================ m15b-combat-dtor
// 00550E00 CCombatInformation::~CCombatInformation() RET 0 (ctor 00552280 is lane-m15-0's).
void __fastcall kotor_m15b_combat_information_destruct_00550e00(void* self, void*) noexcept(false);
}
