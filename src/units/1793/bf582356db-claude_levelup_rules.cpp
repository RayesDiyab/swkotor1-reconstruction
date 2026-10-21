// Lane worker "levelup" (fork lu-srules): 005B27E0 CSWSCreatureStats::AutoLevelup and its server rules closure.
// Transcribed from the original swkotor.exe (read-only); statement comments cite original instruction addresses.
// Functions reconstructed here (no directly callable provider in the c31 map):
//   005B27E0 CSWSCreatureStats::AutoLevelup(int floaty)                         thiscall RET 4   -> @kotor_fastcall_005b27e0@12
//   005A59D0 CSWSCreatureStats::ComputeNumberKnownSpellsLeft(BYTE class_index)   thiscall RET 4   -> @kotor_fastcall_005a59d0@12 (AL)
//   005A6F40 CSWSCreatureStats::CalcLevelUpNumberFeats(BYTE, BYTE*, BYTE*)       thiscall RET 0Ch -> @kotor_fastcall_005a6f40@20
//   005AC8F0 CSWSCreatureStats::CanChooseSpell(DWORD spell, BYTE left, list*)    thiscall RET 0Ch -> @kotor_fastcall_005ac8f0@20
//   005A7110 CSWSCreatureStats::HasSpellPrereq(CSWSpell*, list*)                thiscall RET 8   -> @kotor_fastcall_005a7110@16
//   005B2530 CSWSCreatureStats::CanChooseFeat(WORD feat, BYTE normal, BYTE bonus, list*) thiscall RET 10h -> @kotor_fastcall_005b2530@24
//   005A6FE0 CSWSCreatureStats::SetNormalBonusFlags(WORD feat, int*, int*)       thiscall RET 0Ch -> @kotor_fastcall_005a6fe0@20
//   005AFB00 CSWSCreatureStats::FeatRequirementsMet(WORD feat, list*)            thiscall RET 8   -> @kotor_fastcall_005afb00@16
//   005A7230 CSWSCreatureStats::HasFeatPrereq(CSWFeat*, list*) (Ghidra: CSWCLevelUpStats) thiscall RET 8 -> @kotor_fastcall_005a7230@16
//   005A7080 CSWSCreatureStats::FeatAcquired(WORD feat, list*) (Ghidra: CSWCLevelUpStats)  thiscall RET 8 -> @kotor_fastcall_005a7080@16
//   005BE6D0 CSWClass::GetRecommendedSpell(int flag, int index)                 thiscall RET 8   -> @kotor_fastcall_005be6d0@16
// Callees already reconstructed in claude_p2s_b_levelup.cpp are imported (005CB120, 005CB150, 005AA810, 005CB270, 005AF950).
#include "6bd5403233-claude_dialogue_common.hpp"
#include <initializer_list>

using namespace dlg;

namespace kotor { namespace server_mainloop { unsigned creature_force_class_005be4a0(unsigned class_id); } }  // CSWClass::IsJedi (cdecl, 0982)

extern "C" {
extern void* kotor_cell_rules_007a3a28;                                                      // [007A3A28] CSWRules*
// rules / class leaves (0820-m15b-rules-leaves, 0428-rules-class-info)
void* __fastcall kotor_rules_get_feat_00550c00(void* rules, void*, U feat);                  // CSWRules::GetFeat (word) RET 4
std::int32_t __fastcall kotor_m15b_class_get_recommended_skill_005be430(void* cls, void*, std::int32_t rank); // RET 4
std::int32_t __fastcall kotor_m15b_class_get_recommended_feat_005be3c0(void* cls, void*, std::int32_t rank);  // RET 4
std::int32_t __fastcall kotor_m15b_class_is_skill_useable_005be070(void* cls, void*, std::uint16_t skill);   // RET 4
std::int32_t __fastcall kotor_m15b_class_is_skill_class_skill_005be010(void* cls, void*, std::uint16_t skill); // RET 4
std::int32_t __fastcall kotor_m15b_class_is_granted_feat_005be310(void* cls, void*, std::uint16_t feat, std::uint8_t* level_out); // RET 8
std::int32_t __fastcall kotor_m15b_class_is_bonus_feat_005be270(void* cls, void*, std::uint16_t feat);       // RET 4
std::int32_t __fastcall kotor_m15b_class_is_normal_feat_005be2c0(void* cls, void*, std::uint16_t feat);      // RET 4
std::uint8_t __fastcall kotor_m15b_class_get_level_feat_granted_005be220(void* cls, void*, std::uint16_t feat); // RET 4
std::uint8_t __fastcall kotor_m15b_class_get_spells_known_per_level_005bcd60(void* cls, void*, std::uint8_t level,
    std::uint8_t spell_level, std::uint8_t class_id, std::uint16_t race, std::uint8_t cha);                  // RET 14h
void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void* spells, void*, int id);                     // CSWSpellArray::GetSpell RET 4
U __fastcall kotor_fastcall_0059b650(void* spell, void*, U cls);                                             // CSWSpell::GetSpellLevel (claude_combat_spell)
// creature stats (server)
U8 __fastcall recovered_stats_level_005a5fd0(void* stats, void*, U flag);                    // CSWSCreatureStats::GetLevel (AL) (0963)
U __fastcall kotor_fastcall_005a60d0(void* stats, void*);                                     // GetBaseAttackBonus (claude_combat_attack_attackstats)
U __fastcall recovered_stats_has_spell_005a6e70(void* stats, void*, U cls, U spell, U flag);  // HasSpell RET 0Ch (vm_actions_t4)
U __fastcall recovered_stats_can_use_skill_005af880(void* stats, void*, U skill);             // GetCanUseSkill RET 4 (w2_g07_native)
void* __fastcall recovered_history_level_ctor_005cb1b0(void* level, void*);                  // CSWLevelStats::CSWLevelStats (0963)
// client floaty text
U __fastcall kotor_fastcall_005eda50(void* client, void*, U id);                              // ServerToClientObjectId RET 4
void __fastcall kotor_fastcall_005edea0(void* client, void*, U id, U kind, I value);          // CClientExoApp::AddFloatyText RET 0Ch
// claude_p2s_b_levelup.cpp
void __fastcall kotor_fastcall_005cb120(void* self, void*, U skill, U value);                // CSWLevelStats::SetSkillRankChange
void __fastcall kotor_fastcall_005cb150(void* self, void*, U feat);                          // CSWLevelStats::AddFeat
void __fastcall kotor_fastcall_005cb270(void* self, void*);                                  // CSWLevelStats::~CSWLevelStats
U __fastcall kotor_fastcall_005af950(void* stats, void*, void* level);                       // CSWSCreatureStats::ValidateLevelUp
void __fastcall kotor_fastcall_005aa810(void* stats, void*, U feat);                         // CSWSCreatureStats::AddFeat

// exports of this file
void __fastcall kotor_fastcall_005b27e0(void* stats, void*, unsigned flag);
U8 __fastcall kotor_fastcall_005a59d0(void* stats, void*, U class_index);
void __fastcall kotor_fastcall_005a6f40(void* stats, void*, U class_id, U8* normal_out, U8* bonus_out);
U __fastcall kotor_fastcall_005ac8f0(void* stats, void*, U spell, U left, void* list);
U __fastcall kotor_fastcall_005a7110(void* stats, void*, void* spell, void* list);
U __fastcall kotor_fastcall_005b2530(void* stats, void*, U feat, U normal_left, U bonus_left, void* list);
void __fastcall kotor_fastcall_005a6fe0(void* stats, void*, U feat, U* normal, U* bonus);
U __fastcall kotor_fastcall_005afb00(void* stats, void*, U feat, void* list);
U __fastcall kotor_fastcall_005a7230(void* stats, void*, void* feat_info, void* list);
U __fastcall kotor_fastcall_005a7080(void* stats, void*, U feat, void* list);
U __fastcall kotor_fastcall_005be6d0(void* cls, void*, U flag, U index);
}

namespace {
constexpr U16 k_race_droid_0074bf10 = 5;                 // .rdata [0074BF10] word 0005h
inline void* rules() { return kotor_cell_rules_007a3a28; }
// Inlined "class id of the last class" pattern: index count-1 < count ? [stats+A7h+i*28h] : FFh.
inline U8 last_class_id(void* stats) {
    const U8 count = rd8(stats, 0x89);
    const U8 index = static_cast<U8>(count - 1);
    return index < count ? rd8(stats, 0xa7 + static_cast<U>(index) * 0x28) : static_cast<U8>(0xff);
}
// CSWRules class table entry: [rules+ACh] + id*1A0h (pointer arithmetic in integers, as the original).
inline void* class_info(U8 id) { return ptr(rd(rules(), 0xac) + static_cast<U>(id) * 0x1a0); }
// Stack CExoArrayList<T> {data, size, capacity}; destructor = the original _free(data) (EH cleanup and scope end).
struct LocalList {
    void* data = nullptr; I size = 0; I capacity = 0;
    LocalList() = default; LocalList(const LocalList&) = delete; LocalList& operator=(const LocalList&) = delete;
    ~LocalList() { live_free(data); }
};
static_assert(sizeof(LocalList) == 12, "CExoArrayList");
}  // namespace

// 005A59D0 CSWSCreatureStats::ComputeNumberKnownSpellsLeft(BYTE class_index) RET 4 -> AL
extern "C" U8 __fastcall kotor_fastcall_005a59d0(void* stats, void*, U class_index) {
    const U8 bl = static_cast<U8>(class_index);                                                // 005A59D1
    const U8 count = rd8(stats, 0x89);                                                         // 005A59D8
    const U8 id_a = bl < count ? rd8(stats, 0xa7 + static_cast<U>(bl) * 0x28) : static_cast<U8>(0xff); // 005A59DE..005A59EC
    const U info = rd(rules(), 0xac) + static_cast<U>(id_a) * 0x1a0;                          // 005A59F3..005A5A02
    const U8 id_b = bl < count ? rd8(stats, 0xa7 + static_cast<U>(bl) * 0x28) : static_cast<U8>(0xff); // 005A5A08..005A5A17
    if (info == 0) return 0;                                                                   // 005A5A1E..005A5A26
    if (kotor::server_mainloop::creature_force_class_005be4a0(id_b) == 0) return 0;           // 005A5A29..005A5A37 IsJedi
    if (bl >= rd8(stats, 0x89)) return 1;                                                      // 005A5A39..005A5A3F
    const U8 level = rd8(stats, 0xa8 + static_cast<U>(bl) * 0x28);                             // 005A5A41..005A5A51
    return level == 1 ? static_cast<U8>(2) : static_cast<U8>(1);                               // 005A5A53..005A5A59
}

// 005A6F40 CSWSCreatureStats::CalcLevelUpNumberFeats(BYTE class, BYTE* normal, BYTE* bonus) RET 0Ch.
// Quirk: the class argument is ignored; the last class and its next level are used.
extern "C" void __fastcall kotor_fastcall_005a6f40(void* stats, void*, U, U8* normal_out, U8* bonus_out) {
    const U8 count = rd8(stats, 0x89);                                                         // 005A6F44
    const U8 index = static_cast<U8>(count - 1);                                               // 005A6F4A..005A6F4C
    const U8 id = index < count ? rd8(stats, 0xa7 + static_cast<U>(index) * 0x28) : static_cast<U8>(0xff); // 005A6F4E..005A6F5E
    const U info = rd(rules(), 0xac) + static_cast<U>(id) * 0x1a0;                            // 005A6F65..005A6F74
    const U8 level = index < count ? rd8(stats, 0xa8 + static_cast<U>(index) * 0x28) : static_cast<U8>(0); // 005A6F7A..005A6F97
    const U next = static_cast<U8>(level + 1);                                                 // 005A6F99..005A6F9B
    const I slot = static_cast<I>(next) - 1;                                                   // 005A6F9E
    *normal_out = slot < 0x14 ? rd8(ptr(info), next + 0x14f) : static_cast<U8>(0);             // 005A6FA1..005A6FB6
    *bonus_out = slot < 0x14 ? rd8(ptr(info), next + 0x13b) : static_cast<U8>(0);              // 005A6FB9..005A6FD7
}

// 005A7110 CSWSCreatureStats::HasSpellPrereq(CSWSpell* spell, CExoArrayList<DWORD>* chosen) RET 8
extern "C" U __fastcall kotor_fastcall_005a7110(void* stats, void*, void* spell, void* list) {
    // two -1 terminated prerequisite lists: +174h (each needed) then +160h (each needed)
    for (U off : {0x174u, 0x160u}) {                                                           // 005A7110..005A7128 / 005A7175..005A7187
        U cursor = word(spell) + off;
        U id = rd(ptr(cursor), 0);
        while (id != 0xffffffffu) {
            if (recovered_stats_has_spell_005a6e70(stats, nullptr, 0, id, 1) == 0) {           // 005A7130..005A713E / 005A7190..005A71A0
                const I size = rdi(list, 4);                                                   // 005A7140 / 005A71A2
                if (size <= 0) return 0;                                                       // 005A7143..005A7162
                I i = 0;
                U data = rd(list, 0);
                for (;;) {
                    if (rd(ptr(data), 0) == id) break;                                         // 005A7150..005A7152 (cmp eax,-1 never true)
                    ++i; data += 4;                                                            // 005A7154..005A7155
                    if (i >= size) return 0;                                                   // 005A7158..005A7162
                }
            }
            id = rd(ptr(cursor), 4);                                                           // 005A716A / 005A71CA
            cursor += 4;                                                                       // 005A716D / 005A71CD
        }
    }
    // [esp+18h] (the list argument slot) gets the class id in its low byte; the dword is passed on.
    const U8 cls = last_class_id(stats);                                                       // 005A71D7..005A71F9
    const U slot = (word(list) & 0xffffff00u) | cls;                                           // 005A71FD
    const U8 spell_level = static_cast<U8>(kotor_fastcall_0059b650(spell, nullptr, slot));     // 005A7201..005A720F GetSpellLevel
    const U8 level = recovered_stats_level_005a5fd0(stats, nullptr, 0);                        // 005A720B..005A7213
    return level < spell_level ? 0u : 1u;                                                      // 005A7218..005A7223 (cmp/sbb/inc)
}

// 005AC8F0 CSWSCreatureStats::CanChooseSpell(DWORD spell, BYTE left, CExoArrayList<DWORD>* chosen) RET 0Ch
extern "C" U __fastcall kotor_fastcall_005ac8f0(void* stats, void*, U spell, U left, void* list) {
    const U8 bl = static_cast<U8>(rd8(stats, 0x89) - 1);                                       // 005AC902..005AC90F
    void* const info = kotor_m15b_spell_array_get_spell_0059b6d0(rp(rules(), 0x8c), nullptr, static_cast<int>(spell)); // 005AC908..005AC911
    if (info == nullptr) return 0;                                                             // 005AC918..005AC929
    const U8 type = rd8(info, 0x140);                                                          // 005AC92C
    if (type != 1 && type != 5) return 0;                                                      // 005AC932..005AC938
    if (static_cast<U8>(left) == 0) return 0;                                                  // 005AC93A..005AC940
    const U8 count = rd8(stats, 0x89);                                                         // 005AC942..005AC94A
    U eax = 0;
    for (;;) {
        if (bl >= count) break;                                                                // 005AC950..005AC954
        const U entry = word(stats) + static_cast<U>(bl) * 0x28;                               // 005AC956..005AC964
        if (static_cast<U16>(eax) >= rd16(ptr(entry), 0x90)) break;                            // 005AC95C..005AC968 (jae)
        const U idx = static_cast<U8>(eax);                                                    // 005AC96A (movzx al)
        const U known = static_cast<I>(idx) < rdi(ptr(entry), 0x90)
                            ? rd(rp(ptr(entry), 0x8c), idx * 4) : 0xffffffffu;                 // 005AC96D..005AC980
        if (spell == known) return 0;                                                          // 005AC983..005AC985
        ++eax;                                                                                 // 005AC987
    }
    const I size = rdi(list, 4);                                                               // 005AC98A..005AC98E
    if (size > 0) {                                                                            // 005AC993..005AC995
        const U data = rd(list, 0);                                                            // 005AC997
        U16 i = 0;
        U ecx = 0;
        do {
            if (spell == rd(ptr(data), ecx * 4)) return 0;                                     // 005AC9A0..005AC9A7
            ++i; ecx = i;                                                                      // 005AC9AD..005AC9AE
        } while (static_cast<I>(ecx) < size);                                                  // 005AC9B1..005AC9B3
    }
    if (kotor_fastcall_005a7110(stats, nullptr, info, list) == 0) return 0;                    // 005AC9B9..005AC9C4
    const I diff = static_cast<I>(static_cast<U8>(left)) - rdi(list, 4);                       // 005AC9CA..005AC9D3 (size re-read)
    return diff > 0 ? 1u : 0u;                                                                 // 005AC9D6..005AC9DB
}

// 005A6FE0 CSWSCreatureStats::SetNormalBonusFlags(WORD feat, int* normal, int* bonus) RET 0Ch
extern "C" void __fastcall kotor_fastcall_005a6fe0(void* stats, void*, U feat, U* normal, U* bonus) {
    *normal = 0;                                                                               // 005A6FF0
    *bonus = 0;                                                                                // 005A6FF8
    if (kotor_rules_get_feat_00550c00(rules(), nullptr, feat) == nullptr) return;             // 005A6FFF..005A700D
    const U info = rd(rules(), 0xac) + static_cast<U>(last_class_id(stats)) * 0x1a0;          // 005A700F..005A7043
    if (info == 0) return;                                                                     // 005A7045
    if (kotor_m15b_class_is_normal_feat_005be2c0(ptr(info), nullptr, static_cast<U16>(feat)) != 0) *normal = 1; // 005A7047..005A7053
    if (kotor_m15b_class_is_bonus_feat_005be270(ptr(info), nullptr, static_cast<U16>(feat)) != 0) *bonus = 1;   // 005A7059..005A7065
}

// 005A7080 CSWSCreatureStats::FeatAcquired(WORD feat, CExoArrayList<WORD>* chosen) RET 8
extern "C" U __fastcall kotor_fastcall_005a7080(void* stats, void*, U feat, void* list) {
    const U16 f = static_cast<U16>(feat);                                                      // 005A7081
    if (f == 0xffff) return 0;                                                                 // 005A7086..005A7090
    for (U off : {0x0u, 0x18u}) {                                                              // feats +0/+4, bonus feats +18h/+1Ch
        I n = rdi(stats, off + 4);                                                             // 005A7093 / 005A70B0
        if (n > 0) {                                                                           // 005A7099..005A709B / 005A70B5..005A70B7
            U found = 0;
            U cursor = rd(stats, off);
            do {
                if (rd16(ptr(cursor), 0) == f) ++found;                                        // 005A70A0..005A70A5 / 005A70C0..005A70C5
                cursor += 2;
            } while (--n != 0);                                                                // 005A70A9..005A70AA / 005A70C9..005A70CA
            if (found != 0) return 1;                                                          // 005A70AC..005A70AE / 005A70CC..005A70CE
        }
    }
    if (list != nullptr) {                                                                     // 005A70D0..005A70D6
        const I size = rdi(list, 4);                                                           // 005A70D8
        if (size > 0) {                                                                        // 005A70DD..005A70DF
            const U data = rd(list, 0);                                                        // 005A70E1
            U16 i = 0;
            U eax = 0;
            do {
                if (f == rd16(ptr(data), eax * 2)) return 1;                                   // 005A70E5..005A70E9
                ++i; eax = i;                                                                  // 005A70EB..005A70EC
            } while (static_cast<I>(eax) < size);                                              // 005A70EF..005A70F1
        }
    }
    return 0;                                                                                  // 005A70F3..005A70F7
}

// 005A7230 CSWSCreatureStats::HasFeatPrereq(CSWFeat* info, CExoArrayList<WORD>* chosen) RET 8
extern "C" U __fastcall kotor_fastcall_005a7230(void* stats, void*, void* info, void* list) {
    const U16 first = rd16(info, 0x38);                                                        // 005A7238
    if (first != 0xffff && kotor_fastcall_005a7080(stats, nullptr, first, list) == 0) return 0; // 005A723C..005A7253
    const U16 second = rd16(info, 0x3a);                                                       // 005A7257
    if (second != 0xffff && kotor_fastcall_005a7080(stats, nullptr, second, list) == 0) return 0; // 005A725B..005A7270
    U any = 0, ok = 0;                                                                         // 005A727B..005A727D edi / ebp
    U ebx = 0;
    do {
        if (static_cast<I>(ebx) >= 5) break;                                                   // 005A7281..005A7284
        if (static_cast<U8>(ebx) < 5) {                                                        // 005A7286..005A7289
            const U16 alt = rd16(info, 0x3c + static_cast<U>(static_cast<U8>(ebx)) * 2);       // 005A728B..005A7292
            if (alt != 0xffff) {                                                               // 005A7297..005A729B
                any = 1;                                                                       // 005A72A5
                if (kotor_fastcall_005a7080(stats, nullptr, alt, list) != 0) ok = any;         // 005A729D..005A72B3
            }
        }
        ++ebx;                                                                                 // 005A72B5
    } while (ok == 0);                                                                         // 005A72B6..005A72B8
    if (any != 0 && ok == 0) return 0;                                                         // 005A72BA..005A72C8
    return 1;                                                                                  // 005A72CB..005A72D4
}

// 005AFB00 CSWSCreatureStats::FeatRequirementsMet(WORD feat, CExoArrayList<WORD>* chosen) RET 8
extern "C" U __fastcall kotor_fastcall_005afb00(void* stats, void*, U feat, void* list) {
    void* const info = kotor_rules_get_feat_00550c00(rules(), nullptr, feat);                 // 005AFB06..005AFB13
    if (info == nullptr) return 0;                                                             // 005AFB18..005AFB24
    void* const cls = class_info(last_class_id(stats));                                        // 005AFB27..005AFB56
    const U8 granted = kotor_m15b_class_get_level_feat_granted_005be220(cls, nullptr, static_cast<U16>(feat)); // 005AFB5C..005AFB62
    if (granted != 0xff && granted > recovered_stats_level_005a5fd0(stats, nullptr, 0)) return 0; // 005AFB64..005AFB7F
    if (rd8(info, 0x32) != 0) {                                                                // 005AFB82..005AFB87
        U found = 0;                                                                           // 005AFB8D..005AFB8F ([esp+24h])
        U8 bl = 0;
        for (;;) {
            if (bl >= rd8(stats, 0x89)) {                                                      // 005AFBA0..005AFBA6
                if (found == 0) return 0;                                                      // 005AFC44..005AFC46
                break;
            }
            const U entry = word(stats) + static_cast<U>(bl) * 0x28;                           // 005AFBB8..005AFBC5
            const U8 class_id = rd8(ptr(entry), 0xa7);                                         // 005AFBBE..005AFBC8
            void* const c = class_info(class_id);                                              // 005AFBAC..005AFBDC
            if (rd(c, 0x19c) != 0) {                                                           // 005AFBD5..005AFBE0
                const U8 level = rd8(ptr(entry), 0xa8);                                        // 005AFBE2 / 005AFC01..005AFC08
                const U8 cha = rd8(stats, 0xf3);                                               // 005AFBE9..005AFBF6
                const U16 race = rd16(stats, 0xdc);                                            // 005AFBEF
                const U8 spell_level = rd8(info, 0x32);                                        // 005AFBFA..005AFBFD
                if (kotor_m15b_class_get_spells_known_per_level_005bcd60(c, nullptr, level, spell_level, class_id, race, cha) != 0) // 005AFC0A..005AFC2A
                    found = 1;                                                                 // 005AFC2C
            }
            ++bl;                                                                              // 005AFC38
            if (found != 0) break;                                                             // 005AFC3A..005AFC42
        }
    }
    const U8 bab = rd8(info, 0x2c);                                                            // 005AFC4C
    if (static_cast<I>(static_cast<U>(bab)) > static_cast<I>(kotor_fastcall_005a60d0(stats, nullptr))) return 0; // 005AFC4F..005AFC5B (jg)
    if (rd8(info, 0x2e) > rd8(stats, 0xe9)) return 0;                                          // 005AFC61..005AFC6A
    if (rd8(info, 0x2f) > rd8(stats, 0xeb)) return 0;                                          // 005AFC70..005AFC79
    if (rd8(info, 0x30) > rd8(stats, 0xef)) return 0;                                          // 005AFC7F..005AFC88
    if (rd8(info, 0x31) > rd8(stats, 0xf1)) return 0;                                          // 005AFC8E..005AFC97
    if (kotor_fastcall_005a7230(stats, nullptr, info, list) == 0) return 0;                    // 005AFC9D..005AFCAC
    const U16 skill = rd16(info, 0x46);                                                        // 005AFCB2
    if (skill != 0xffff && recovered_stats_can_use_skill_005af880(stats, nullptr, skill) == 0) return 0; // 005AFCB6..005AFCC7
    return 1;                                                                                  // 005AFCCD..005AFCD9
}

// 005B2530 CSWSCreatureStats::CanChooseFeat(WORD feat, BYTE normal_left, BYTE bonus_left, CExoArrayList<WORD>* chosen) RET 10h
extern "C" U __fastcall kotor_fastcall_005b2530(void* stats, void*, U feat, U normal_left, U bonus_left, void* list) {
    U normal = 0, bonus = 0;                                                                   // 005B255E..005B2562 ([esp+10h]/[esp+0Ch])
    if (kotor_rules_get_feat_00550c00(rules(), nullptr, feat) == nullptr) return 0;           // 005B2551..005B256D
    U8 a = static_cast<U8>(normal_left), b = static_cast<U8>(bonus_left);
    if (a == 0 && b == 0) return 0;                                                            // 005B256F..005B257D
    const U16 f = static_cast<U16>(feat);
    {
        const U edx = rd(stats, 4);                                                            // 005B2595 (known feats size)
        if (static_cast<U16>(edx) != 0) {                                                      // 005B259A..005B259E (word jbe)
            U16 i = 0;
            do {
                const U idx = i;                                                               // 005B25A0
                const U16 known = static_cast<I>(idx) < static_cast<I>(edx) ? rd16(rp(stats, 0), idx * 2)
                                                                             : static_cast<U16>(0xffff); // 005B25A3..005B25B1
                if (f == known) return 0;                                                      // 005B25B5..005B25B8
                ++i;                                                                           // 005B25BE
            } while (i < static_cast<U16>(edx));                                               // 005B25BF..005B25C2
        }
    }
    {
        const I size = rdi(list, 4);                                                           // 005B25C4..005B25C8
        if (size > 0) {                                                                        // 005B25CD..005B25CF
            const U data = rd(list, 0);                                                        // 005B25D1
            U16 i = 0;
            U eax = 0;
            do {
                if (f == rd16(ptr(data), eax * 2)) return 0;                                   // 005B25E0..005B25E9
                ++i; eax = i;                                                                  // 005B25EF..005B25F0
            } while (static_cast<I>(eax) < size);                                              // 005B25F3..005B25F5
        }
    }
    if (kotor_fastcall_005afb00(stats, nullptr, feat, list) == 0) return 0;                    // 005B25FD..005B2608
    kotor_fastcall_005a6fe0(stats, nullptr, feat, &normal, &bonus);                            // 005B260E..005B261B
    if (normal == 0 && bonus == 0) return 0;                                                   // 005B2620..005B262A
    U used = 0;                                                                                // 005B263A ([esp+44h])
    LocalList marks;                                                                           // 005B2646..005B2657 (EH state 0)
    {
        const I n = rdi(list, 4);                                                              // 005B2652
        if (n != 0) {                                                                          // 005B2655..005B265B
            marks.capacity = n;                                                                // 005B2665
            marks.data = live_operator_new(static_cast<U>(n) * 4);                             // 005B265D..005B2673
        }
        for (I i = 0; i < rdi(list, 4); ++i) wr(marks.data, static_cast<U>(i) * 4, 0);         // 005B2677..005B268E
        marks.size = n;                                                                        // 005B267E
    }
    if (rdi(list, 4) > 0) {                                                                    // 005B2690..005B2695
        U16 i = 0;
        U esi = 0;
        do {
            kotor_fastcall_005a6fe0(stats, nullptr, rd16(rp(list, 0), esi * 2), &normal, &bonus); // 005B26A0..005B26B7
            if (normal != 0) {                                                                 // 005B26BC..005B26C6
                if (bonus == 0) {                                                              // 005B26C8..005B26CA
                    if (a == 0) return 0;                                                      // 005B26CC..005B26D2 -> 005B273D
                    --a;                                                                       // 005B26D4..005B26D6
                    wr(marks.data, esi * 4, 1);                                                // 005B26EE
                }
            } else if (bonus != 0 && b != 0) {                                                 // 005B26DC..005B26E6
                --b;                                                                           // 005B26E8..005B26EA
                wr(marks.data, esi * 4, 1);                                                    // 005B26EE
            }
            ++i; esi = i;                                                                      // 005B26F6..005B26FA
        } while (static_cast<I>(esi) < rdi(list, 4));                                          // 005B26FD..005B26FF
    }
    kotor_fastcall_005a6fe0(stats, nullptr, feat, &normal, &bonus);                            // 005B2701..005B2714
    U8 dl, bl;
    if (normal != 0) {                                                                         // 005B2719..005B2723
        if (bonus != 0) {                                                                      // 005B2725..005B2727
            dl = a; bl = b;                                                                    // 005B276E / 005B2772
        } else {
            dl = a;                                                                            // 005B2729
            if (dl == 0) return 0;                                                             // 005B272D..005B272F
            --dl; used = 1;                                                                    // 005B2731..005B2733
            bl = b;                                                                            // 005B2772
        }
    } else if (bonus != 0) {                                                                   // 005B2752..005B2754
        bl = b;                                                                                // 005B2756
        if (bl == 0) return 0;                                                                 // 005B275A..005B275C
        dl = a; --bl; used = 1;                                                                // 005B275E..005B2764
    } else {
        dl = a; bl = b;                                                                        // 005B276E / 005B2772
    }
    const I n = rdi(list, 4);                                                                  // 005B2776
    if (n > 0) {                                                                               // 005B277B..005B277D
        U16 i = 0;
        U eax = 0;
        do {
            if (rd(marks.data, eax * 4) == 0) {                                                // 005B2781..005B2786
                if (dl != 0) --dl;                                                             // 005B2788..005B278C
                else if (bl != 0) --bl;                                                        // 005B2790..005B2794
                else return 0;                                                                 // 005B2792 -> 005B273D
            }
            ++i; eax = i;                                                                      // 005B2796..005B2797
        } while (static_cast<I>(eax) < n);                                                     // 005B279A..005B279C
    }
    if (used == 0 && dl == 0 && bl == 0) return 0;                                             // 005B279E..005B27AC
    return 1;                                                                                  // 005B27AE..005B27BF (marks freed at scope end)
}

// 005BE6D0 CSWClass::GetRecommendedSpell(int flag, int index) RET 8 (returns EAX; FFFFFFFFh when none)
extern "C" U __fastcall kotor_fastcall_005be6d0(void* cls, void*, U flag, U index) {
    const U16 count = rd16(cls, 0x178);                                                        // 005BE6D2..005BE6D4
    U eax = 0xffffffffu;                                                                       // 005BE6DE
    U esi = 0;                                                                                 // 005BE6E1
    if (count == 0) return eax;                                                                // 005BE6E5..005BE6EC
    const U table = rd(cls, 0x174);                                                            // 005BE6EE
    U16 di = 0;
    do {
        const U entry = table + static_cast<U>(di) * 0x10;                                     // 005BE700..005BE70A
        const U8 kind = rd8(ptr(entry), 0xc);                                                  // 005BE706
        if (kind == 1 || kind == 5) {                                                          // 005BE70C..005BE714
            U edx;
            if (flag != 0) {                                                                   // 005BE716..005BE71C
                edx = rd(ptr(entry), 4);                                                       // 005BE71E
                if (edx == index) return rd16(ptr(entry), 0);                                  // 005BE721..005BE734
            } else {
                edx = rd(ptr(entry), 8);                                                       // 005BE737
                if (edx == index) return rd16(ptr(entry), 0);                                  // 005BE73A..005BE769
            }
            if (edx == 0 && esi != index) {                                                    // 005BE73E..005BE744
                eax = rd16(ptr(entry), 0);                                                     // 005BE746
                ++esi;                                                                         // 005BE749
            }
        }
        ++di;                                                                                  // 005BE74A
    } while (di < count);                                                                      // 005BE74B..005BE750
    return eax;                                                                                // 005BE752..005BE757
}

// 005B27E0 CSWSCreatureStats::AutoLevelup(int floaty) RET 4
extern "C" void __fastcall kotor_fastcall_005b27e0(void* stats, void*, unsigned flag) {
    LocalList feats;                                                                           // 005B2802..005B280A [esp+48h] (EH state 1)
    LocalList spells;                                                                          // 005B2812..005B281A [esp+3Ch]
    const U8 count = rd8(stats, 0x89);                                                         // 005B281E
    U8 bl = static_cast<U8>(count - 1);                                                        // 005B2824..005B2826
    const U8 class_index = bl;                                                                 // 005B282F [esp+38h]
    const U8 class_id = bl < count ? rd8(stats, 0xa7 + static_cast<U>(bl) * 0x28) : static_cast<U8>(0xff); // 005B2828..005B2849 [esp+28h]
    void* const info = class_info(class_id);                                                   // 005B284D..005B2868 [esp+1Ch]
    {
        const U exp = rd(stats, 0x68);                                                         // 005B2864
        const U8 level = recovered_stats_level_005a5fd0(stats, nullptr, 0);                    // 005B2867..005B286C
        if (exp < rd(rules(), static_cast<U>(level) * 4 + 0x38)) goto done;                   // 005B2871..005B287A (jb)
    }
    for (;;) {
        // 005B2890: new level in the class at class_index.
        const U8 level_in_class = bl < rd8(stats, 0x89) ? rd8(stats, 0xa8 + static_cast<U>(bl) * 0x28) : static_cast<U8>(0); // 005B2890..005B28B5
        const U8 new_level = static_cast<U8>(level_in_class + 1);                              // 005B28B7..005B28BD [esp+12h]
        void* level = nullptr;
        {
            void* const memory = live_operator_new(0x30);                                      // 005B28BB..005B28C9 (EH state 2)
            if (memory != nullptr) {                                                           // 005B28CD..005B28D4
                try { level = recovered_history_level_ctor_005cb1b0(memory, nullptr); }        // 005B28D6..005B28DD
                catch (...) { live_free(memory); throw; }
            }
        }
        wr8(level, 0x2c, class_id);                                                            // 005B28E3..005B28F9
        if ((new_level & 3u) == 0) wr8(level, 0x2a, rd8(info, 0x17a));                         // 005B28E7..005B290F (signed mod 4 == 0)
        wr8(level, 0x2b, rd8(info, 0x54));                                                     // 005B2915..005B2918
        wr8(level, 0x2d, rd8(info, 0x55));                                                     // 005B291B..005B291E
        I points = static_cast<I>(rd16(stats, 0x164));                                         // 005B2921
        const U8 skill_base = rd8(info, 0x56);                                                 // 005B2928
        if (new_level == 1) {                                                                  // 005B2912 / 005B292B
            const I v = static_cast<I>(static_cast<U>(skill_base) >> 1) + static_cast<std::int8_t>(rd8(stats, 0xf0)); // 005B292D..005B293B
            points += (v > 1 ? v : 1) * 4;                                                     // 005B293D..005B2957
        } else {
            const I v = (static_cast<std::int8_t>(rd8(stats, 0xf0)) + static_cast<I>(skill_base)) / 2; // 005B295D..005B296E
            points += v > 1 ? v : 1;                                                           // 005B2970..005B2989
        }
        U unrestricted = 0;                                                                    // [esp+2Ch]
        if (rd(stats, 0x6c) != 0) {                                                            // 005B298B..005B2998
            void* const party = lane_config_required::GetPartyTable_004aee70(server(), nullptr); // 005B299A..005B29A3
            unrestricted = rd(party, 0xf0) == 0xffffffffu ? 1u : 0u;                           // 005B29A8..005B29B9
        }
        // ---- skills (005B29C1..005B2B0A)
        U skill_rank = 1;                                                                      // 005B2990 [esp+18h]
        if (points > 0) {                                                                      // 005B29C1..005B29C3
            do {
                if (static_cast<I>(skill_rank) > static_cast<I>(rd8(rules(), 0xab))) break;   // 005B29D0..005B29DF (jg)
                const I skill = kotor_m15b_class_get_recommended_skill_005be430(info, nullptr, static_cast<I>(skill_rank)); // 005B29E5..005B29F4
                if (skill != -1) {                                                             // 005B29F4..005B29F8
                    U8 max_rank = static_cast<U8>(new_level + 3);                              // 005B29FE..005B2A13
                    const U entry = static_cast<U>(static_cast<U16>(skill)) * 0x20 + rd(rules(), 0xb4); // 005B2A02..005B2A16
                    if (entry != 0 &&                                                          // 005B2A18
                        ((rd(ptr(entry), 0x1c) & 8u) != 0 ||                                   // 005B2A1E..005B2A22
                         kotor_m15b_class_is_skill_useable_005be070(info, nullptr, static_cast<U16>(skill)) != 0)) { // 005B2A24..005B2A32
                        U class_skill;
                        if (kotor_m15b_class_is_skill_class_skill_005be010(info, nullptr, static_cast<U16>(skill)) != 0) { // 005B2A38..005B2A46
                            class_skill = 1;                                                   // 005B2A48
                        } else {
                            class_skill = 0;                                                   // 005B2A4F
                            max_rank = static_cast<U8>(max_rank >> 1);                         // 005B2A51
                        }
                        std::int8_t rank;
                        if (static_cast<U8>(skill) >= rd8(rules(), 0xab)) {                    // 005B2A53..005B2A63
                            rank = 0;                                                          // 005B2A65
                        } else {
                            const I r = static_cast<std::int8_t>(rd8(rp(stats, 0x168), static_cast<U8>(skill))); // 005B2A69..005B2A76
                            rank = r >= 0x7f ? static_cast<std::int8_t>(0x7f)                  // 005B2A7A..005B2A7D / 005B2ACA
                                 : r <= -0x7f ? static_cast<std::int8_t>(-0x7f)                // 005B2A7F..005B2A84
                                 : static_cast<std::int8_t>(r);
                        }
                        I diff = static_cast<I>(max_rank) - static_cast<I>(rank);              // 005B2A86..005B2A90
                        bool allowed = true;
                        if (unrestricted == 0) {                                               // 005B2A92..005B2A94
                            const U skill_flags = rd(ptr(entry), 0x1c);                        // 005B2A96
                            if ((skill_flags & 0x10u) == 0) allowed = false;                   // 005B2A99..005B2A9B
                            else if (rd16(stats, 0xdc) == k_race_droid_0074bf10 && (skill_flags & 0x20u) == 0) allowed = false; // 005B2A9D..005B2AB3
                        }
                        if (allowed && diff > 0) {                                             // 005B2AB5..005B2AB7
                            U value;
                            if (class_skill != 0) {                                            // 005B2AB9..005B2ABB
                                if (diff >= points) diff = points;                             // 005B2ABD..005B2AC1
                                value = static_cast<U8>(diff);                                 // 005B2AC3
                                points -= static_cast<I>(value);                               // 005B2AC6
                            } else {
                                const I half = points / 2;                                     // 005B2ACE..005B2AD3
                                if (diff >= half) diff = half;                                 // 005B2AD5..005B2AD9
                                value = static_cast<U8>(diff);                                 // 005B2ADB..005B2ADE
                                points -= static_cast<I>(value) * 2;                           // 005B2AE0..005B2AE2
                            }
                            kotor_fastcall_005cb120(level, nullptr, static_cast<U>(skill), value); // 005B2AE6..005B2AF0
                        }
                    }
                }
                ++skill_rank;                                                                  // 005B2AF5..005B2AFC
            } while (points > 0);                                                              // 005B2AFA..005B2B00
        }
        wr16(level, 0x28, static_cast<U16>(points));                                           // 005B2B06..005B2B0A
        // ---- granted feats at this class level go straight to the creature (005B2B0E..005B2B5A)
        if (rd16(rules(), 0xa4) != 0) {                                                        // 005B2B0E..005B2B1D
            U i = 0;
            do {
                U8 granted_level;
                if (kotor_m15b_class_is_granted_feat_005be310(info, nullptr, static_cast<U16>(i), &granted_level) != 0 && // 005B2B27..005B2B36
                    granted_level == new_level)                                                // 005B2B38..005B2B40
                    kotor_fastcall_005aa810(stats, nullptr, i);                                // 005B2B42..005B2B45
                ++i;                                                                           // 005B2B57
            } while (static_cast<I>(i) < static_cast<I>(static_cast<U>(rd16(rules(), 0xa4))));  // 005B2B4A..005B2B5A
        }
        // ---- chosen feats (005B2B5C..005B2C8B)
        U8 normal_count = 0, bonus_count = 0;                                                  // [esp+34h] / [esp+30h]
        kotor_fastcall_005a6f40(stats, nullptr, class_id, &normal_count, &bonus_count);        // 005B2B5C..005B2B6F
        I feats_left = static_cast<I>(normal_count) + static_cast<I>(bonus_count);             // 005B2B74..005B2B85 [esp+20h]
        const I feat_count = static_cast<I>(static_cast<U>(rd16(rules(), 0xa4)));              // 005B2B7E / 005B2B97 [esp+2Ch]
        feats.size = 0;                                                                        // 005B2B9B
        U feat_rank = 1;                                                                       // 005B2B8F [esp+18h]
        if (feats_left > 0) {                                                                  // 005B2B89..005B2B9F
            do {
                if (static_cast<I>(feat_rank) > feat_count) break;                             // 005B2BA5..005B2BAF
                const U feat = static_cast<U>(kotor_m15b_class_get_recommended_feat_005be3c0(info, nullptr, static_cast<I>(feat_rank))); // 005B2BB5..005B2BBF
                // 005B2BC1..005B2BC7: movzx edx,bx / cmp edx,-1 can never be equal (original dead check).
                void* const feat_info = kotor_rules_get_feat_00550c00(rules(), nullptr, feat); // 005B2BCD..005B2BD4
                if (feat_info != nullptr && !(rd8(feat_info, 0x2d) > new_level) &&             // 005B2BD9..005B2BE8 (ja)
                    kotor_fastcall_005b2530(stats, nullptr, feat, normal_count, bonus_count, &feats) != 0) { // 005B2BEE..005B2C09
                    kotor_fastcall_005cb150(level, nullptr, feat);                             // 005B2C0B..005B2C10
                    if (feats.size == feats.capacity) {                                        // 005B2C15..005B2C1B (inlined Add)
                        const I cap = feats.capacity != 0 ? feats.capacity * 2 : 0x10;         // 005B2C1D..005B2C25
                        void* const old = feats.data;                                          // 005B2C2A
                        feats.capacity = cap;                                                  // 005B2C32
                        feats.data = live_operator_new(static_cast<U>(cap) * 2);               // 005B2C2E..005B2C40
                        for (I k = 0; k < feats.size; ++k)                                     // 005B2C3E..005B2C5B
                            wr16(feats.data, static_cast<U>(k) * 2, rd16(old, static_cast<U>(k) * 2));
                        if (old != nullptr) live_free(old);                                    // 005B2C61..005B2C66
                    }
                    wr16(feats.data, static_cast<U>(feats.size) * 2, static_cast<U16>(feat));  // 005B2C6E..005B2C72
                    ++feats.size;                                                              // 005B2C76..005B2C78
                    --feats_left;                                                              // 005B2C77..005B2C7C
                }
                ++feat_rank;                                                                   // 005B2C80..005B2C87
            } while (feats_left > 0);                                                          // 005B2C85..005B2C8B
        }
        // ---- force powers for force-using classes (005B2C91..005B2E2D)
        if (rd(info, 0x19c) != 0) {                                                            // 005B2C91..005B2C9D
            I spells_left = kotor_fastcall_005a59d0(stats, nullptr, class_index);              // 005B2CA3..005B2CCB [esp+20h]
            const I spell_count = rdi(rp(rules(), 0x8c), 0);                                   // 005B2CB1..005B2CCF [esp+2Ch]
            spells.size = 0;                                                                   // 005B2CD7
            U spell_rank = 1;                                                                  // 005B2CC6 [esp+18h]
            if (spells_left > 0) {                                                             // 005B2CC4..005B2CDB
                do {
                    if (static_cast<I>(spell_rank) > spell_count) break;                       // 005B2CE5..005B2CE9
                    const std::int16_t alignment = static_cast<std::int16_t>(rd16(stats, 0x17e)); // 005B2CEF..005B2CF3
                    const U8 side = alignment <= 0x28 ? static_cast<U8>(3) : static_cast<U8>((alignment >= 0x3c ? 1 : 0) + 1); // 005B2CFA..005B2D0B
                    const U spell = static_cast<U16>(kotor_fastcall_005be6d0(info, nullptr, side != 3 ? 1u : 0u, spell_rank)); // 005B2D0D..005B2D1F
                    // 005B2D22..005B2D25: cmp ebp,-1 after movzx can never be equal (original dead check).
                    if (kotor_fastcall_005ac8f0(stats, nullptr, spell, static_cast<U>(spells_left), &spells) != 0) { // 005B2D2B..005B2D41
                        if (rd(level, 4) == rd(level, 8)) {                                    // 005B2D47..005B2D4D (inlined Add on level+0)
                            const U cap = rd(level, 8) != 0 ? rd(level, 8) * 2 : 0x10;         // 005B2D4F..005B2D57
                            void* const old = rp(level, 0);                                    // 005B2D5C
                            wr(level, 8, cap);                                                 // 005B2D66
                            wp(level, 0, live_operator_new(cap * 4));                          // 005B2D5E..005B2D71
                            for (I k = 0; k < rdi(level, 4); ++k)                              // 005B2D76..005B2D8E
                                wr(rp(level, 0), static_cast<U>(k) * 4, rd(old, static_cast<U>(k) * 4));
                            if (old != nullptr) live_free(old);                                // 005B2D90..005B2D95
                        }
                        wr(rp(level, 0), rd(level, 4) * 4, spell);                             // 005B2D9D..005B2DA2
                        wr(level, 4, rd(level, 4) + 1);                                        // 005B2DA5..005B2DAF
                        if (spells.size == spells.capacity) {                                  // 005B2DA8..005B2DB2 (inlined Add on the local)
                            const I cap = spells.capacity != 0 ? spells.capacity * 2 : 0x10;   // 005B2DB4..005B2DBC
                            void* const old = spells.data;                                     // 005B2DC1
                            spells.capacity = cap;                                             // 005B2DCD
                            spells.data = live_operator_new(static_cast<U>(cap) * 4);          // 005B2DC5..005B2DDB
                            for (I k = 0; k < spells.size; ++k)                                // 005B2DD9..005B2DF0
                                wr(spells.data, static_cast<U>(k) * 4, rd(old, static_cast<U>(k) * 4));
                            if (old != nullptr) live_free(old);                                // 005B2DF6..005B2DFB
                        }
                        wr(spells.data, static_cast<U>(spells.size) * 4, spell);               // 005B2E03..005B2E0F
                        ++spells.size;                                                         // 005B2E12..005B2E14
                        --spells_left;                                                         // 005B2E13..005B2E18
                    }
                    ++spell_rank;                                                              // 005B2E1C..005B2E23
                } while (spells_left > 0);                                                     // 005B2E21..005B2E27
            }
        }
        // ---- apply (005B2E33..005B2E4E)
        if (kotor_fastcall_005af950(stats, nullptr, level) == 0) {                             // 005B2E33..005B2E3F
            kotor_fastcall_005cb270(level, nullptr);                                           // 005B2E41..005B2E43
            live_free(level);                                                                  // 005B2E48..005B2E4E
        }
        const U exp = rd(stats, 0x68);                                                         // 005B2E51..005B2E55
        const U8 lvl = recovered_stats_level_005a5fd0(stats, nullptr, 0);                      // 005B2E58..005B2E5A
        if (exp < rd(rules(), static_cast<U>(lvl) * 4 + 0x38)) break;                          // 005B2E5F..005B2E6C (jae loops)
        bl = class_index;                                                                      // 005B2882
    }
done:
    if (flag != 0) {                                                                           // 005B2E72..005B2E78
        const U id = rd(rp(stats, 0x24), 4);                                                   // 005B2E7E..005B2E86
        void* const client_app = client();                                                     // 005B2E81..005B2E89
        const U8 level = recovered_stats_level_005a5fd0(stats, nullptr, 0);                    // 005B2E8C..005B2E93
        const U client_id = kotor_fastcall_005eda50(client_app, nullptr, id);                  // 005B2E99..005B2E9C
        kotor_fastcall_005edea0(client_app, nullptr, client_id, 4, static_cast<I>(level));     // 005B2E96..005B2EA4
    }
    // 005B2EA9..005B2EC5: _free(spells.data) then _free(feats.data) (LocalList destructors, reverse declaration order).
}
