// lane-m15-b (L15-B, Milestone 1.5) unit m15b-rules-leaves:
// CSWClass / CSWRace / CSWFeat / CSWSkill / CSWPortrait accessors, CSWSpellArray::GetSpell,
// CSWBaseItemArray::GetBaseItem, CSWItem::GetBaseItem and C2DA::GetFLOATEntry(int,int,float*).
// Original: swkotor.exe GOG K1 (image base 0x400000); every body cites its instruction addresses
// (static dumpbin disassembly, lane work\asm\<addr>.asm). D1 fastcall veneers (m15b_stats.h).
// Unwind (D30-FINAL): the three GetNameText-style bodies keep the original EH-frame order through
// C++ locals (the STR_RES CExoString is destroyed on both the normal and the unwind path).
#include "bf662019daaaa2b6_m15b_stats.h"
#include "e1805047c9b7563b_startup_string.h"   // kotor_reconstruction::StartupString (005B3190/005E5A90/005E5B00/005E5C20, 005E5290)
#include "c53472623c7d6765_resources.h"        // menu_resources::Ref (16-byte CResRef)
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <new>

using kotor_reconstruction::StartupString;
using menu_resources::Ref;

// ---- existing owners (declarations identical in linkage/ABI to the owners' headers) ----
namespace kotor_sound_path { float __fastcall StringAsFloat005e5650(StartupString*, void*); }  // 005E5650 CExoString::AsFLOAT (lane-sound)
extern "C" {
extern void* kotor_cell_rules_007a3a28;   // [007A3A28] g_pRules (lane-prepare prepare_image_data, sole owner)
extern void* kotor_cell_tlk_007a3a08;     // [007A3A08] g_pTlkTable (lane-tlk, D44)
int __cdecl kotor_sscanf_006fae08(const char*, const char*, ...);                               // 006FAE08 sscanf (font-txi)
Ref* __fastcall c2da_default_ref_00405ed0(Ref*, void*);                                         // 00405ED0 CResRef::CResRef (D9)
Ref* __fastcall unresolved_ref_assign_00406290(Ref*, void*, Ref*, const char*);                // 00406290 CResRef::operator=(char const*)
const char* __cdecl c2da_empty_0073d71c();                                                       // .rdata "" 0073D71C (M41 accessor)
std::uint32_t __fastcall tlk_fetch_0041e550(void* tlk, void*, std::uint32_t strref, void* str_res,
                                            std::uint32_t gender) noexcept(false);              // 0041E550 CTlkTable::Fetch RET 0Ch (lane-tlk)
// 00550C00 CSWRules::GetFeat is a C++-linkage owner (rules-class-info); declared below.
}
namespace kotor_rules { void* __fastcall kotor_fastcall_00550c00(void* rules, void* unused_edx, std::uint16_t feat); }

namespace {
using Word = std::uint32_t;
template <class T> inline T ld(const void* base, std::uintptr_t off) noexcept {
    T v; std::memcpy(&v, static_cast<const unsigned char*>(base) + off, sizeof(T)); return v;
}
template <class T> inline void st(void* base, std::uintptr_t off, T v) noexcept {
    std::memcpy(static_cast<unsigned char*>(base) + off, &v, sizeof(T));
}
inline unsigned char* at(void* base, std::uintptr_t off) noexcept { return static_cast<unsigned char*>(base) + off; }
inline void* rules() noexcept { return kotor_cell_rules_007a3a28; }

// .rdata data used by these bodies (image values, read via tools pe.py):
constexpr char kFormatF_0073e2e4[] = "%f";          // 0073E2E4
const unsigned char kTlkGender_0073ecd0 = 0x00;      // 0073ECD0 (byte; pushed zero-extended, as gui-load gui_params)

// STR_RES local {CExoString text; CResRef sound; DWORD} built by 005BE0E0/005CD760/005CD970
// ([esp+8] / [esp+10h] / tail in their 20h frames).
struct StrRes {
    StartupString text;               // 005B3190 (state 0 -> 1)
    Ref sound;                        // 00405ED0 (state 2)
    unsigned char tail[4];
};
static_assert(offsetof(StrRes, sound) == 8 && sizeof(StrRes) == 0x1C, "STR_RES frame layout");

// Shared shape of 005BE0E0 / 005CD760 / 005CD970: Fetch(strref, &res, [0073ECD0]) on [007A3A08], then
// copy-construct the hidden return from res.text (005E5B00) and destroy res.text (005E5C20).
void* fetch_text(Word strref, void* out_string) {
    StrRes res;                                                       // CExoString() 005B3190
    c2da_default_ref_00405ed0(&res.sound, nullptr);                   // CResRef() 00405ED0
    tlk_fetch_0041e550(kotor_cell_tlk_007a3a08, nullptr, strref, &res, kTlkGender_0073ecd0); // Fetch 0041E550
    ::new (out_string) StartupString(res.text);                       // 005E5B00 copy into hidden return
    return out_string;                                                // ~CExoString(res.text) 005E5C20 at scope exit
}

// CSWClass feat entry list: +16C entries (8 bytes), +170 WORD count.
inline unsigned char* class_feats(void* cls) noexcept { return ld<unsigned char*>(cls, 0x16C); }
inline std::uint16_t class_feat_count(void* cls) noexcept { return ld<std::uint16_t>(cls, 0x170); }
// CSWClass skill entry list: +164 entries (12 bytes), +168 WORD count.
inline unsigned char* class_skills(void* cls) noexcept { return ld<unsigned char*>(cls, 0x164); }
inline std::uint16_t class_skill_count(void* cls) noexcept { return ld<std::uint16_t>(cls, 0x168); }
}  // namespace

extern "C" {

// ---- 00413350 C2DA::GetFLOATEntry(int row, int column, float* out), RET 0Ch ----
std::int32_t __fastcall kotor_m15b_c2da_get_float_entry_00413350(void* table, void*, std::int32_t row,
                                                                 std::int32_t column, float* out) noexcept(false) {
    if (row < 0 || row >= ld<std::int32_t>(table, 0x24) ||                      // 00413357..00413364
        column < 0 || column >= ld<std::int32_t>(table, 0x28)) {                // 0041336A..0041337B
        *out = kotor_sound_path::StringAsFloat005e5650(                         // 00413414..00413421 AsFLOAT(+1C default); fstp dword
            reinterpret_cast<StartupString*>(at(table, 0x1C)), nullptr);
        return 0;                                                               // 00413424
    }
    if (ld<Word>(table, 0x3C) != 0) {                                           // 00413381..00413386 binary table
        const std::int32_t index = ld<std::int32_t>(table, 0x28) * row + column; // 00413388..0041338B
        const Word offset = ld<std::uint16_t>(ld<unsigned char*>(table, 0x44), static_cast<std::uintptr_t>(index) * 2u); // 0041338D..00413390
        *out = 0.0f;                                                            // 00413394..00413398
        kotor_sscanf_006fae08(reinterpret_cast<const char*>(ld<unsigned char*>(table, 0x40) + offset), // 0041339E..004133AA
                              kFormatF_0073e2e4, out);
        return ld<unsigned char*>(table, 0x40)[offset] != 0 ? 1 : 0;           // 004133AF..004133C2 (re-reads +40h)
    }
    StartupString* const cell = reinterpret_cast<StartupString*>(               // 004133C7..004133D9 [[+38h]+row*4] + col*8
        ld<unsigned char*>(ld<unsigned char*>(table, 0x38), static_cast<std::uintptr_t>(row) * 4u) +
        static_cast<std::uintptr_t>(column) * 8u);
    if (cell->BorrowedEquals005e5290(c2da_empty_0073d71c()) != 0) {            // 004133D4..004133E2 operator==("")
        *out = 0.0f;                                                            // 004133E4..004133EA
        return 0;                                                               // 004133F0
    }
    *out = kotor_sound_path::StringAsFloat005e5650(cell, nullptr);             // 004133F6..00413408 AsFLOAT; fstp dword
    return 1;                                                                   // 0041340B
}

// ---- 0059B6D0 CSWSpellArray::GetSpell(int id), RET 4 ----
void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void* self, void*, std::int32_t id) {
    const std::uintptr_t data = ld<std::uintptr_t>(self, 4);                   // 0059B6D0
    if (data == 0 || id < 0 || id >= ld<std::int32_t>(self, 0)) return nullptr; // 0059B6D3..0059B6E1 / 0059B6EE
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(id) * 0x198u + data); // 0059B6E3..0059B6E9
}

// ---- 005B31A0 CSWBaseItemArray::GetBaseItem(int id), RET 4 ----
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* self, void*, std::int32_t id) {
    const std::uintptr_t data = ld<std::uintptr_t>(self, 4);                   // 005B31A0
    if (data == 0 || id < 0 || id >= ld<std::int32_t>(self, 0)) return nullptr; // 005B31A3..005B31B1 / 005B31BE
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(id) * 0xC8u + data); // 005B31B3..005B31B9
}

// ---- 005B4790 CSWItem::GetBaseItem(), RET (inlined 005B31A0 on [[007A3A28]+34h]) ----
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void* self, void*) {
    const std::int32_t id = ld<std::int32_t>(self, 0x0C);                       // 005B4790
    void* const items = ld<void*>(rules(), 0x34);                               // 005B4793..005B4799
    const std::uintptr_t data = ld<std::uintptr_t>(items, 4);                  // 005B479C
    if (data == 0 || id < 0 || id >= ld<std::int32_t>(items, 0)) return nullptr; // 005B479F..005B47A9 / 005B47B4
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(id) * 0xC8u + data); // 005B47AB..005B47B1
}

// ---- 005BCCB0 / 005BCCD0 / 005BCCF0 / 005BCD10 CSWClass per-level byte tables, RET 4 ----
// level in [1, 3Ch] -> byte [cls + base + level], else 0 (005BCCB4..005BCCC8 pattern).
std::uint8_t __fastcall kotor_m15b_class_get_attack_bonus_005bccb0(void* cls, void*, std::uint8_t level) {
    if (level < 1 || level > 0x3C) return 0;                                    // 005BCCB4..005BCCBA / 005BCCC6
    return ld<std::uint8_t>(cls, 0x17u + level);                                // 005BCCBC..005BCCBF
}
std::uint8_t __fastcall kotor_m15b_class_get_fort_save_bonus_005bccd0(void* cls, void*, std::uint8_t level) {
    if (level < 1 || level > 0x3C) return 0;                                    // 005BCCD4..005BCCDA / 005BCCE6
    return ld<std::uint8_t>(cls, 0x56u + level);                                // 005BCCDC..005BCCDF
}
std::uint8_t __fastcall kotor_m15b_class_get_ref_save_bonus_005bccf0(void* cls, void*, std::uint8_t level) {
    if (level < 1 || level > 0x3C) return 0;                                    // 005BCCF4..005BCCFA / 005BCD09
    return ld<std::uint8_t>(cls, 0xCEu + level);                                // 005BCCFC..005BCCFF
}
std::uint8_t __fastcall kotor_m15b_class_get_will_save_bonus_005bcd10(void* cls, void*, std::uint8_t level) {
    if (level < 1 || level > 0x3C) return 0;                                    // 005BCD14..005BCD1A / 005BCD29
    return ld<std::uint8_t>(cls, 0x92u + level);                                // 005BCD1C..005BCD1F
}

// ---- 005BCD60 CSWClass::GetSpellsKnownPerLevel(level, spell_level, ...), RET 14h ----
std::uint8_t __fastcall kotor_m15b_class_get_spells_known_per_level_005bcd60(void* cls, void*, std::uint8_t level,
    std::uint8_t spell_level, std::uint8_t, std::uint16_t, std::uint8_t) {
    if (level < 1 || level > 0x14) return 0;                                    // 005BCD64..005BCD6A / 005BCD93
    if (spell_level >= ld<std::uint8_t>(cls, 0x113u + level)) return 0;        // 005BCD6C..005BCD7A
    unsigned char* const* const table = ld<unsigned char* const*>(cls, 0x110);  // 005BCD7C
    if (!table) return 0;                                                       // 005BCD82..005BCD84
    return table[level - 1][spell_level];                                       // 005BCD86..005BCD8D
}

// ---- 005BE010 CSWClass::IsSkillClassSkill(WORD skill), RET 4 -> entry DWORD +4 ----
std::int32_t __fastcall kotor_m15b_class_is_skill_class_skill_005be010(void* cls, void*, std::uint16_t skill) {
    if (skill >= ld<std::uint8_t>(rules(), 0xAB)) return 0;                     // 005BE010..005BE02B
    const std::uint16_t count = class_skill_count(cls);                        // 005BE02F
    for (std::uint16_t i = 0; i < count; ++i) {                                 // 005BE036..005BE053
        unsigned char* const e = class_skills(cls) + static_cast<std::uintptr_t>(i) * 12u; // 005BE03D..005BE046
        if (ld<std::uint16_t>(e, 0) == skill) return ld<std::int32_t>(e, 4);   // 005BE049 / 005BE05C..005BE062
    }
    return 0;                                                                   // 005BE055..005BE059
}

// ---- 005BE070 CSWClass::IsSkillUseable(WORD skill), RET 4 ----
std::int32_t __fastcall kotor_m15b_class_is_skill_useable_005be070(void* cls, void*, std::uint16_t skill) {
    void* const r = rules();                                                    // 005BE070
    if (skill >= ld<std::uint8_t>(r, 0xAB)) return 0;                           // 005BE076..005BE08C
    const std::uintptr_t entry = static_cast<std::uintptr_t>(skill) * 0x20u + ld<std::uintptr_t>(r, 0xB4); // 005BE08F..005BE09C
    if (entry == 0) return 0;                                                   // 005BE09E -> 005BE0D6
    if (ld<Word>(reinterpret_cast<void*>(entry), 0x1C) & 8u) return 1;          // 005BE0A0..005BE0AD (all classes)
    const std::uint16_t count = class_skill_count(cls);                        // 005BE0B0
    for (std::uint16_t i = 0; i < count; ++i)                                   // 005BE0B7..005BE0D4
        if (ld<std::uint16_t>(class_skills(cls), static_cast<std::uintptr_t>(i) * 12u) == skill) return 1; // 005BE0CA -> 005BE0A6
    return 0;                                                                   // 005BE0D6..005BE0DA
}

// ---- 005BE0E0 CSWClass::GetNameText() -> CExoString (hidden return), RET 4 ----
void* __fastcall kotor_m15b_class_get_name_text_005be0e0(void* cls, void*, void* out_string) noexcept(false) {
    return fetch_text(ld<Word>(cls, 0x00), out_string);                         // 005BE0F9..005BE177 (strref [esi])
}

// ---- 005BE220 CSWClass::GetLevelFeatGranted(WORD feat), RET 4 -> granted level or FF ----
std::uint8_t __fastcall kotor_m15b_class_get_level_feat_granted_005be220(void* cls, void*, std::uint16_t feat) {
    const std::uint16_t count = class_feat_count(cls);                         // 005BE222
    for (std::uint16_t i = 0; i < count; ++i) {                                 // 005BE229..005BE257
        unsigned char* const e = class_feats(cls) + static_cast<std::uintptr_t>(i) * 8u; // 005BE231..005BE247
        if (ld<std::uint16_t>(e, 0) == feat && ld<std::uint8_t>(e, 3) == 0)     // 005BE243..005BE251 (list type 0)
            return ld<std::uint8_t>(e, 2);                                      // 005BE261..005BE265
    }
    return 0xFF;                                                                // 005BE25B
}

// ---- 005BE270 CSWClass::IsBonusFeat(WORD feat), RET 4 -> (type >> 1) & 1 of the first match ----
std::int32_t __fastcall kotor_m15b_class_is_bonus_feat_005be270(void* cls, void*, std::uint16_t feat) {
    const std::uint16_t count = class_feat_count(cls);                         // 005BE270
    for (std::uint16_t i = 0; i < count; ++i) {                                 // 005BE278..005BE29D
        unsigned char* const e = class_feats(cls) + static_cast<std::uintptr_t>(i) * 8u;
        if (ld<std::uint16_t>(e, 0) == feat)                                    // 005BE293
            return static_cast<std::int32_t>((ld<std::uint8_t>(e, 3) >> 1) & 1u); // 005BE2A6..005BE2B1
    }
    return 0;                                                                   // 005BE29F..005BE2A3
}

// ---- 005BE2C0 CSWClass::IsNormalFeat(WORD feat), RET 4 -> type & 1 of the first match ----
std::int32_t __fastcall kotor_m15b_class_is_normal_feat_005be2c0(void* cls, void*, std::uint16_t feat) {
    const std::uint16_t count = class_feat_count(cls);                         // 005BE2C0
    for (std::uint16_t i = 0; i < count; ++i) {                                 // 005BE2C8..005BE2ED
        unsigned char* const e = class_feats(cls) + static_cast<std::uintptr_t>(i) * 8u;
        if (ld<std::uint16_t>(e, 0) == feat)                                    // 005BE2E3
            return static_cast<std::int32_t>(ld<std::uint8_t>(e, 3) & 1u);     // 005BE2F6..005BE2FF
    }
    return 0;                                                                   // 005BE2EF..005BE2F3
}

// ---- 005BE310 CSWClass::IsGrantedFeat(WORD feat, BYTE* level), RET 8 ----
std::int32_t __fastcall kotor_m15b_class_is_granted_feat_005be310(void* cls, void*, std::uint16_t feat, std::uint8_t* level_out) {
    *level_out = 0xFF;                                                          // 005BE313..005BE317
    const std::uint16_t count = class_feat_count(cls);                         // 005BE31A
    for (std::uint16_t i = 0; i < count; ++i) {                                 // 005BE321..005BE340
        unsigned char* const e = class_feats(cls) + static_cast<std::uintptr_t>(i) * 8u;
        if (ld<std::uint16_t>(e, 0) == feat) {                                  // 005BE336 (first match only)
            if (ld<std::uint8_t>(e, 3) != 0) return 0;                          // 005BE34A..005BE355 -> 005BE342
            *level_out = ld<std::uint8_t>(e, 2);                                // 005BE357..005BE35A
            return 1;                                                           // 005BE35E
        }
    }
    return 0;                                                                   // 005BE342..005BE347
}

// ---- 005BE370 CSWClass::IsFeatUseable(WORD feat), RET 4 ----
std::int32_t __fastcall kotor_m15b_class_is_feat_useable_005be370(void* cls, void*, std::uint16_t feat) {
    if (!kotor_rules::kotor_fastcall_00550c00(rules(), nullptr, feat)) return 0; // 005BE378..005BE386 CSWRules::GetFeat
    const std::uint16_t count = class_feat_count(cls);                         // 005BE388
    for (std::uint16_t i = 0; i < count; ++i)                                   // 005BE38F..005BE3AD
        if (ld<std::uint16_t>(class_feats(cls), static_cast<std::uintptr_t>(i) * 8u) == feat) return 1; // 005BE3A3 / 005BE3B6
    return 0;                                                                   // 005BE3AF..005BE3B3
}

// ---- 005BE3C0 CSWClass::GetRecommendedFeat(int rank), RET 4 ----
// Entry with priority == rank wins; otherwise the result is the feat of the last priority-0 entry seen
// while the running counter (starting at -1, incremented per priority-0 entry) was != rank.
std::int32_t __fastcall kotor_m15b_class_get_recommended_feat_005be3c0(void* cls, void*, std::int32_t rank) {
    std::int32_t counter = -1;                                                  // 005BE3CB ebx
    std::int32_t result = -1;                                                   // 005BE3D4 [esp+10h]
    const std::uint16_t count = class_feat_count(cls);                         // 005BE3C1
    for (std::uint16_t i = 0; i < count; ++i) {                                 // 005BE3CE..005BE406
        unsigned char* const e = class_feats(cls) + static_cast<std::uintptr_t>(i) * 8u; // 005BE3E4..005BE3E7
        const std::int32_t priority = ld<std::int32_t>(e, 4);                  // 005BE3EB
        if (priority == rank) return ld<std::uint16_t>(e, 0);                   // 005BE3EE / 005BE414..005BE417
        if (priority == 0 && counter != rank) {                                 // 005BE3F2..005BE3F8
            result = ld<std::uint16_t>(e, 0);                                   // 005BE3FA / 005BE3FE
            ++counter;                                                          // 005BE3FD
        }
    }
    return result;                                                              // 005BE408
}

// ---- 005BE430 CSWClass::GetRecommendedSkill(int rank), RET 4 (same shape on the skill entries, +8 priority) ----
std::int32_t __fastcall kotor_m15b_class_get_recommended_skill_005be430(void* cls, void*, std::int32_t rank) {
    std::int32_t counter = -1;                                                  // 005BE43B ebp
    std::int32_t result = -1;                                                   // 005BE444 [esp+10h]
    const std::uint16_t count = class_skill_count(cls);                        // 005BE431
    for (std::uint16_t i = 0; i < count; ++i) {                                 // 005BE43E..005BE478
        unsigned char* const e = class_skills(cls) + static_cast<std::uintptr_t>(i) * 12u; // 005BE454..005BE45A
        const std::int32_t priority = ld<std::int32_t>(e, 8);                  // 005BE45D
        if (priority == rank) return ld<std::uint16_t>(e, 0);                   // 005BE460 / 005BE486..005BE48E
        if (priority == 0 && counter != rank) {                                 // 005BE464..005BE46A
            result = ld<std::uint16_t>(e, 0);                                   // 005BE46C / 005BE470
            ++counter;                                                          // 005BE46F
        }
    }
    return result;                                                              // 005BE47A
}

// ---- 005BE770 CSWClass::GetArmorBonus(int level), RET 4 -> byte [[+10Ch] + level] ----
std::uint8_t __fastcall kotor_m15b_class_get_armor_bonus_005be770(void* cls, void*, std::int32_t level) {
    return ld<std::uint8_t>(ld<unsigned char*>(cls, 0x10C), static_cast<std::uintptr_t>(level)); // 005BE770..005BE77A
}

// ---- 005C7CC0 CSWRace::IsFirstLevelGrantedFeat(WORD feat), RET 4 ----
std::int32_t __fastcall kotor_m15b_race_is_first_level_granted_feat_005c7cc0(void* race, void*, std::uint16_t feat) {
    unsigned char* const feats = ld<unsigned char*>(race, 0x28);                // 005C7CC0
    if (!feats) return 0;                                                       // 005C7CC3..005C7CC7 (eax = 0)
    const std::uint16_t count = ld<std::uint16_t>(race, 0x24);                  // 005C7CC9
    for (std::uint16_t i = 0; i < count; ++i)                                   // 005C7CCD..005C7CED
        if (ld<std::uint16_t>(feats, static_cast<std::uintptr_t>(i) * 2u) == feat) return 1; // 005C7CE3 / 005C7CF6
    return 0;                                                                   // 005C7CEF..005C7CF3
}

// ---- 005CD760 CSWFeat::GetNameText() -> CExoString, RET 4 (strref [feat+8]) ----
void* __fastcall kotor_m15b_feat_get_name_text_005cd760(void* feat, void*, void* out_string) noexcept(false) {
    return fetch_text(ld<Word>(feat, 0x08), out_string);                        // 005CD779..005CD7F8
}

// ---- 005CD970 CSWSkill::GetDescriptionText() -> CExoString, RET 4 (strref [skill+4]) ----
void* __fastcall kotor_m15b_skill_get_description_text_005cd970(void* skill, void*, void* out_string) noexcept(false) {
    return fetch_text(ld<Word>(skill, 0x04), out_string);                       // 005CD989..005CDA08
}

// ---- 00678EC0 CSWPortrait::CSWPortrait(), RET, EAX = this ----
void* __fastcall kotor_m15b_portrait_construct_00678ec0(void* self, void*) noexcept(false) {
    Ref* const ref = static_cast<Ref*>(self);
    c2da_default_ref_00405ed0(ref, nullptr);                                    // 00678EC6 CResRef()
    Ref discarded;                                                              // [esp+4] hidden return of 00406290
    unresolved_ref_assign_00406290(ref, nullptr, &discarded, c2da_empty_0073d71c()); // 00678ECB..00678ED7 = ""
    return self;                                                                // 00678EDC
}

}  // extern "C"
