// claude_closeB_xp.cpp (lane worker closeB): the level-change tail of CSWSCreatureStats::SetExperience 005AF480 behind the
// 0886 gate NativeCreatureOwnerValues::notify_experience (portable creature_experience_005af480 calls it once the new
// XP is stored and the notify flag is set: every GiveXP / quest / kill / skill-use experience gain), with its missing
// callees, transcribed from the original swkotor.exe (read-only):
//   005AF555..005AF678 SetExperience tail (stats, value, delta, player)   closeB_stats_experience_notify_005af555
//   005AB1E0 CSWSCreatureStats::LevelDown(CSWLevelStats*)               RET 4   kotor_fastcall_005ab1e0
//   005AA940 CSWSCreatureStats::RemoveFeat(ushort)                      RET 4   kotor_fastcall_005aa940
// Register mapping of the tail (entry 005AF555): ESI = stats, EBP = new XP, EDI = new XP - old XP (also the rewritten
// first-argument slot [esp+34]), [esp+10] = the CSWSClient of the owner (GetClientObjectByObjectId result).
// Conventions (W5): thiscall -> extern "C" __fastcall(self, unused_edx, stack args...); byte-returning callees are
// imported as unsigned char so only AL is used, like the original's movzx; game heap only.
// Quirks kept: LevelDown only drops a class entry when the class index register equals FFh (never in practice), it
// stores the reduced base-attack total at creature+E0h as a dword, removes the level record from the history with an
// unconditional count decrement (CExoArrayList::Remove inline), and the known-spell re-add list grows with the inline
// x2/10h policy.
#include <cstdint>
#include <cstring>

namespace {
using U = std::uint32_t;
using U16 = std::uint16_t;
using U8 = std::uint8_t;
inline void* ptr(U a) { return reinterpret_cast<void*>(a); }
inline U word(const void* p) { return reinterpret_cast<U>(p); }
inline U rd(U a) { U v; std::memcpy(&v, ptr(a), 4); return v; }
inline U rd16(U a) { U16 v; std::memcpy(&v, ptr(a), 2); return v; }
inline U rd8(U a) { return *static_cast<const U8*>(ptr(a)); }
inline void wr(U a, U v) { std::memcpy(ptr(a), &v, 4); }
inline void wr8(U a, U v) { *static_cast<U8*>(ptr(a)) = static_cast<U8>(v); }
inline void wr16(U a, U v) { const U16 x = static_cast<U16>(v); std::memcpy(ptr(a), &x, 2); }
template <class R, class... A> R vcall(U object, U slot, A... a) {
    using F = R(__thiscall*)(void*, A...);
    return reinterpret_cast<F>(rd(rd(object) + slot))(ptr(object), a...);
}
}

namespace kotor { namespace server_mainloop { unsigned creature_force_class_005be4a0(unsigned); } } // 005BE4A0 CSWClass::IsJedi (cdecl) [0982]
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }                   // [007A39FC] CAppManager*

extern "C" {
// ---------------- existing providers (c35 map, exact decorated arity) ----------------
void* __cdecl live_operator_new(unsigned);                                                      // 006FA7E6
void __cdecl live_free(void*);                                                                  // 006FA390
extern void* kotor_cell_rules_007a3a28;                                                         // [007A3A28] CSWRules*
U8 __fastcall recovered_stats_level_005a5fd0(void* stats, void*, U flag);                       // 005A5FD0 GetLevel (AL) [0963]
void* __fastcall recovered_server_info_004aec90(void* server, void*);                           // 004AEC90 GetServerInfo [w1_native_b7]
U __fastcall recovered_can_level_up_005a6810(void* stats, void*);                               // 005A6810 CanLevelUp [w2_g07]
void __fastcall kotor_fastcall_005aabf0(void* stats, void*, void* level, U flag);               // 005AABF0 LevelUp RET 8 [p2s_b_levelup]
void __fastcall recovered_creature_feedback_004ede10(void* creature, void*, U message, void* data); // 004EDE10 SendFeedbackMessage RET 8
U8 __fastcall recovered_creature_stats_str_005a6190(void* stats, void*);                        // 005A6190 GetSTRStat (AL) [w2_g07]
U8 __fastcall recovered_creature_stats_dex_005a6550(void* stats, void*);                        // 005A6550 GetDEXStat
U8 __fastcall recovered_creature_stats_con_005a6250(void* stats, void*);                        // 005A6250 GetCONStat
U8 __fastcall recovered_creature_stats_int_005a6310(void* stats, void*);                        // 005A6310 GetINTStat
U8 __fastcall recovered_creature_stats_wis_005a63d0(void* stats, void*);                        // 005A63D0 GetWISStat
U8 __fastcall recovered_creature_stats_cha_005a6490(void* stats, void*);                        // 005A6490 GetCHAStat
void __fastcall w0886x_stats_adjust_spell_uses_005a6d80(void* stats, void*);                    // 005A6D80 AdjustSpellUsesPerDay (0886)
U8 __fastcall recovered_skill_rank_change_005cb0f0(void* level, void*, U skill);                // 005CB0F0 CSWLevelStats::GetSkillRankChange (AL) [w2_save_stats]
void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void* spells, void*, U id);          // 0059B6D0 CSWSpellArray::GetSpell [0820]
void __fastcall kotor_fastcall_005cb270(void* level, void*);                                    // 005CB270 ~CSWLevelStats [p2s_b_levelup]
void* __fastcall recovered_equipment_item_005a4c20(void* inventory, void*, U slot);             // 005A4C20 CSWInventory::GetItemInSlot [1025]
U8 __fastcall recovered_item_can_equip_0051aa60(void* creature, void*, void* item, U* slot, U a, U b, U c); // 0051AA60 CanEquipItem RET 14h [1025]
U __fastcall recovered_creature_unequip_item_004faa70(void* creature, void*, void* item);       // 004FAA70 UnequipItem RET 4 [w1_native_b7]
void __fastcall w1x_object_set_hp_004cc280(void* object, void*, U hp);                          // 004CC280 SetCurrentHitPoints RET 4 (0886)
void __fastcall recovered_u16_list_remove_004e3b20(void* list, void*, U value);                 // 004E3B20 CExoArrayList<ushort>::Remove RET 4 [w3]

// ---------------- bodies defined here ----------------
void __fastcall closeB_stats_experience_notify_005af555(void* stats, void*, U value, U delta, void* player);
void __fastcall kotor_fastcall_005ab1e0(void* stats, void*, void* level);
void __fastcall kotor_fastcall_005aa940(void* stats, void*, U feat);
}

namespace {
U rules() { return word(kotor_cell_rules_007a3a28); }
U server() { return rd(word(const_cast<void*>(kotor_menu_window_cells::manager_007a39fc)) + 8); }
// ability modifier store of the six LevelDown jump-table arms: (score - (score < 10 ? 11 : 10)) / 2 (cdq/sub/sar)
void store_modifier(U stats, U offset, U8 score) {
    const int v = static_cast<int>(score) - (score < 10 ? 11 : 10);
    wr8(stats + offset, static_cast<U>(v / 2));
}
}

// ===================================================================================== 005AA940
void __fastcall kotor_fastcall_005aa940(void* stats, void*, U feat) {
    const U s = word(stats);
    const U16 id = static_cast<U16>(feat);
    recovered_u16_list_remove_004e3b20(stats, nullptr, feat);                        // 005AA942..005AA949 (list at +0)
    U count = rd(s + 0x10);                                                          // 005AA94E
    if (static_cast<int>(count) <= 0) return;                                        // 005AA951..005AA953
    std::int16_t i = static_cast<std::int16_t>(count - 1);                           // 005AA955
    if (i < 0) return;                                                               // 005AA956..005AA959
    U entry = 0;
    for (;;) {                                                                       // 005AA960..005AA96F
        entry = rd(rd(s + 0xc) + static_cast<U>(static_cast<int>(i)) * 4);
        if (static_cast<U16>(rd16(entry)) == id) break;
        --i;
        if (i < 0) return;
    }
    const U last = rd(s + 0x10) - 1;                                                 // 005AA976..005AA979
    int j = static_cast<int>(i);
    wr(s + 0x10, last);                                                              // 005AA981
    while (j < static_cast<int>(rd(s + 0x10))) {                                     // 005AA97F..005AA998
        wr(rd(s + 0xc) + static_cast<U>(j) * 4, rd(rd(s + 0xc) + static_cast<U>(j) * 4 + 4));
        ++j;
    }
    live_free(ptr(entry));                                                           // 005AA99A..005AA99B
}

// ===================================================================================== 005AB1E0
void __fastcall kotor_fastcall_005ab1e0(void* stats, void*, void* level_record) {
    const U s = word(stats);
    const U lv = word(level_record);                                                 // EBP
    const U max_before = vcall<U>(rd(s + 0x24), 0x98, 1u);                           // 005AB1E8..005AB1F0 [esp+18]
    const U8 classes = static_cast<U8>(rd8(s + 0x89));                               // 005AB1F6 BL
    U8 index = 0;                                                                    // CL / [esp+12]
    if (classes != 0) {                                                              // 005AB202..005AB20C
        const U8 wanted = static_cast<U8>(rd8(lv + 0x2c));                           // 005AB20E
        bool found = false;
        for (;;) {
            const U8 cls = index < classes ? static_cast<U8>(rd8(s + index * 40u + 0xa7)) : U8(0xff); // 005AB211..005AB21F
            if (cls == wanted) { found = true; break; }                              // 005AB226..005AB228
            ++index;                                                                 // 005AB230
            if (index >= static_cast<U8>(rd8(s + 0x89))) break;                      // 005AB22A..005AB234
        }
        if (found) {                                                                 // 005AB23C
            const U8 levels = index < classes ? static_cast<U8>(rd8(s + index * 40u + 0xa8)) : U8(0); // 005AB23C..005AB25D
            if (index < classes) wr8(s + index * 40u + 0xa8, static_cast<U8>(levels - 1)); // 005AB25F..005AB26B
        }
    }
    {
        const U8 count = static_cast<U8>(rd8(s + 0x89));                             // 005AB272
        const bool keep = index < count && rd8(s + index * 40u + 0xa8) >= 1;         // 005AB278..005AB291
        if (!keep && index == 0xff) {                                                // 005AB293..005AB296
            const U8 reduced = static_cast<U8>(count - 1);                           // 005AB298
            wr8(s + 0x89, reduced);                                                  // 005AB29C
            if (index < reduced) wr8(s + index * 40u + 0xa7, 0);                     // 005AB29A..005AB2AA
        }
    }
    {
        const U creature = rd(s + 0x24);                                             // 005AB2B2
        const int bab = static_cast<std::int16_t>(rd16(creature + 0xe0)) - static_cast<int>(rd8(lv + 0x2b)); // 005AB2B5..005AB2C0
        wr(creature + 0xe0, static_cast<U>(bab));                                    // 005AB2C2 (dword store)
    }
    switch (rd8(lv + 0x2a)) {                                                        // 005AB2C8..005AB2D5 table 005AB7F0
    case 0: wr8(s + 0xe9, rd8(s + 0xe9) - 1);                                        // 005AB2DC..005AB2E6
            store_modifier(s, 0xea, recovered_creature_stats_str_005a6190(stats, nullptr)); break; // 005AB2EC..005AB319
    case 1: wr8(s + 0xeb, rd8(s + 0xeb) - 1);                                        // 005AB31E..005AB328
            store_modifier(s, 0xec, recovered_creature_stats_dex_005a6550(stats, nullptr)); break; // 005AB32E..005AB35B
    case 2: wr8(s + 0xed, rd8(s + 0xed) - 1);                                        // 005AB360..005AB36A
            store_modifier(s, 0xee, recovered_creature_stats_con_005a6250(stats, nullptr)); break; // 005AB370..005AB39D
    case 3: wr8(s + 0xef, rd8(s + 0xef) - 1);                                        // 005AB3A2..005AB3AC
            store_modifier(s, 0xf0, recovered_creature_stats_int_005a6310(stats, nullptr)); // 005AB3B2..005AB3DC
            w0886x_stats_adjust_spell_uses_005a6d80(stats, nullptr); break;          // 005AB449..005AB44B
    case 4: wr8(s + 0xf1, rd8(s + 0xf1) - 1);                                        // 005AB3DE..005AB3E8
            store_modifier(s, 0xf2, recovered_creature_stats_wis_005a63d0(stats, nullptr)); // 005AB3EE..005AB418
            w0886x_stats_adjust_spell_uses_005a6d80(stats, nullptr); break;
    case 5: wr8(s + 0xf3, rd8(s + 0xf3) - 1);                                        // 005AB41A..005AB424
            store_modifier(s, 0xf4, recovered_creature_stats_cha_005a6490(stats, nullptr)); // 005AB42A..005AB443
            w0886x_stats_adjust_spell_uses_005a6d80(stats, nullptr); break;
    default: break;                                                                  // ja 005AB454
    }
    // skills: undo this level's rank changes, clamped through a signed char
    if (rd8(rules() + 0xab) != 0) {                                                  // 005AB454..005AB467
        U8 skill = 0;                                                                // [esp+13]
        do {
            if (recovered_skill_rank_change_005cb0f0(level_record, nullptr, skill) != 0) { // 005AB470..005AB47E
                U8 rank;
                if (skill >= rd8(rules() + 0xab)) rank = 0;                          // 005AB480..005AB48E
                else {
                    const int r = static_cast<std::int8_t>(rd8(rd(s + 0x168) + skill)); // 005AB492..005AB49B
                    rank = r >= 0x7f ? U8(0x7f) : (r > -0x7f ? static_cast<U8>(r) : U8(0x81)); // 005AB49F..005AB4AD / 005AB58F
                }
                rank = static_cast<U8>(rank - recovered_skill_rank_change_005cb0f0(level_record, nullptr, skill)); // 005AB4AF..005AB4C1
                if (skill < rd8(rules() + 0xab)) wr8(rd(s + 0x168) + skill, rank);  // 005AB4C3..005AB4D4
            }
            ++skill;                                                                 // 005AB4E7
        } while (skill < rd8(rules() + 0xab));                                       // 005AB4DB..005AB4EF
    }
    wr16(s + 0x164, 0);                                                              // 005AB4F9
    for (U8 i = 0; static_cast<U16>(i) < static_cast<U16>(rd16(lv + 0x1c)); ++i)     // 005AB502..005AB52E
        kotor_fastcall_005aa940(stats, nullptr, rd16(rd(lv + 0x18) + i * 2u));       // 005AB510..005AB51F
    // spells gained at this level: remove from the class's known list (CExoArrayList<ulong>::Remove inline)
    for (U8 i = 0; i < static_cast<U8>(rd8(lv + 4)); ++i) {                         // 005AB534..005AB5E6
        const U spell = rd(rd(lv) + i * 4u);                                         // 005AB545..005AB54E
        if (kotor_m15b_spell_array_get_spell_0059b6d0(ptr(rd(rules() + 0x8c)), nullptr, spell) == nullptr) continue; // 005AB551..005AB565
        const U list = s + index * 40u + 0x8c;                                       // 005AB567..005AB56F
        const int count = static_cast<int>(rd(list + 4));                            // 005AB576
        int at = 0;
        if (count <= 0) continue;                                                    // 005AB57B..005AB57D
        while (rd(rd(list) + static_cast<U>(at) * 4) != spell) {                     // 005AB57F..005AB58B
            ++at;
            if (at >= count) break;
        }
        if (at >= count || at < 0) continue;                                         // 005AB58D / 005AB596..005AB598
        const int total = static_cast<int>(rd(list + 4));                            // 005AB59A
        int k = 0;
        if (total > 0) {                                                             // 005AB59F..005AB5A1
            while (rd(rd(list) + static_cast<U>(k) * 4) != spell) {                  // 005AB5A3..005AB5AF (bound: first count)
                ++k;
                if (k >= count) break;
            }
        }
        wr(list + 4, static_cast<U>(total - 1));                                     // 005AB5B1..005AB5B6
        while (k < static_cast<int>(rd(list + 4))) {                                 // 005AB5B4..005AB5D1
            wr(rd(list) + static_cast<U>(k) * 4, rd(rd(list) + static_cast<U>(k) * 4 + 4));
            ++k;
        }
    }
    // spells lost at this level (re-learn list): add back when allowed
    for (U8 i = 0; i < static_cast<U8>(rd8(lv + 0x10)); ++i) {                       // 005AB5F0..005AB710
        const U spell = rd(rd(lv + 0xc) + i * 4u);                                   // 005AB605..005AB60B
        if (index >= static_cast<U8>(rd8(s + 0x89))) continue;                       // 005AB60E..005AB614
        if (kotor_m15b_spell_array_get_spell_0059b6d0(ptr(rd(rules() + 0x8c)), nullptr, spell) == nullptr) continue; // 005AB61A..005AB62E
        if (rd(s + 0x6c) != 0) {                                                     // 005AB634..005AB639
            const U8 cls = index < static_cast<U8>(rd8(s + 0x89)) ? static_cast<U8>(rd8(s + index * 40u + 0xa7)) : U8(0xff); // 005AB63B..005AB651
            if (kotor::server_mainloop::creature_force_class_005be4a0(cls) == 0) continue; // 005AB658..005AB666
        }
        const U list = s + index * 40u + 0x8c;                                       // 005AB66C..005AB674 EDI
        const int count = static_cast<int>(rd(list + 4));                            // 005AB67B
        int at = 0;
        bool present = false;
        if (count > 0) {                                                             // 005AB680..005AB682
            U p = rd(list);
            while (rd(p) != spell) {                                                 // 005AB684..005AB690
                ++at; p += 4;
                if (at >= count) break;
            }
            present = at < count && at != -1;                                        // 005AB694..005AB697
        }
        if (present) continue;
        U alloc = rd(list + 8);                                                      // 005AB699
        if (rd(list + 4) == alloc) {                                                 // 005AB69C..005AB69F
            alloc = alloc != 0 ? alloc + alloc : 0x10;                               // 005AB6A1..005AB6A9
            const U old = rd(list);                                                  // 005AB6AE
            wr(list + 8, alloc);                                                     // 005AB6B8
            wr(list, word(live_operator_new(alloc * 4)));                            // 005AB6B0..005AB6C3
            for (int k = 0; k < static_cast<int>(rd(list + 4)); ++k)                 // 005AB6C8..005AB6DF
                wr(rd(list) + static_cast<U>(k) * 4, rd(old + static_cast<U>(k) * 4));
            if (old != 0) live_free(ptr(old));                                       // 005AB6E1..005AB6E6
        }
        wr(rd(list) + rd(list + 4) * 4, spell);                                      // 005AB6EE..005AB6F7
        wr(list + 4, rd(list + 4) + 1);                                              // 005AB6FA
    }
    // drop the record from the level history (CExoArrayList<CSWLevelStats*>::Remove inline)
    {
        const int count = static_cast<int>(rd(s + 0x2c));                            // 005AB716
        int at = 0;
        if (count > 0) {                                                             // 005AB71B..005AB71D
            U p = rd(s + 0x28);
            while (rd(p) != lv) {                                                    // 005AB722..005AB72F
                ++at; p += 4;
                if (at >= static_cast<int>(rd(s + 0x2c))) break;
            }
        }
        wr(s + 0x2c, static_cast<U>(count - 1));                                     // 005AB731..005AB736
        while (at < static_cast<int>(rd(s + 0x2c))) {                                // 005AB734..005AB751
            wr(rd(s + 0x28) + static_cast<U>(at) * 4, rd(rd(s + 0x28) + static_cast<U>(at) * 4 + 4));
            ++at;
        }
    }
    kotor_fastcall_005cb270(level_record, nullptr);                                  // 005AB753..005AB755
    live_free(level_record);                                                         // 005AB75A..005AB75B
    for (U slot = 0; slot < 0x12; ++slot) {                                          // 005AB763..005AB7AC
        U mask = 1u << slot;                                                         // 005AB765..005AB771 [esp+14]
        const U creature = rd(s + 0x24);
        void* const item = recovered_equipment_item_005a4c20(ptr(rd(creature + 0xa2c)), nullptr, mask); // 005AB76E..005AB77C
        if (item != nullptr &&                                                       // 005AB781..005AB785
            recovered_item_can_equip_0051aa60(ptr(creature), nullptr, item, &mask, 0, 0, 1) == 0) // 005AB787..005AB79D
            (void)recovered_creature_unequip_item_004faa70(ptr(rd(s + 0x24)), nullptr, item); // 005AB79F..005AB7A3
    }
    const U creature = rd(s + 0x24);                                                 // 005AB7AE / 005AB7BB
    const U max_after = vcall<U>(creature, 0x98, 1u);                                // 005AB7B1..005AB7B5 EDI
    const U current = vcall<U>(creature, 0x9c, 1u);                                  // 005AB7BE..005AB7C6 AX
    const int hp = static_cast<std::int16_t>(current) +
        (static_cast<std::int16_t>(max_after) - static_cast<std::int16_t>(max_before)); // 005AB7CC..005AB7D9
    w1x_object_set_hp_004cc280(ptr(creature), nullptr, static_cast<U>(hp));          // 005AB7DB..005AB7DE
}

// ===================================================================================== 005AF555..005AF678
void __fastcall closeB_stats_experience_notify_005af555(void* stats, void*, U value, U delta, void* player) {
    const U s = word(stats);
    if (static_cast<int>(delta) < 0) {                                               // 005AF555..005AF558 (jge not taken)
        U8 level = recovered_stats_level_005a5fd0(stats, nullptr, 0);                // 005AF55E..005AF570 BL
        U info = word(recovered_server_info_004aec90(ptr(server()), nullptr));       // 005AF567..005AF572
        if (static_cast<int>(level) > static_cast<int>(rd(info + 0x90))) {           // 005AF577..005AF582 (jle exit)
            for (;;) {
                if (level == 0) break;                                               // 005AF588..005AF58A
                --level;                                                             // 005AF596
                if (rd(rules() + level * 4u + 0x38) <= rd(s + 0x68)) break;          // 005AF590..005AF5A2 (jbe exit)
                U record = 0;                                                        // 005AF5A8..005AF5C4
                if (rd(s + 0x6c) != 0 && static_cast<int>(level) < static_cast<int>(rd(s + 0x2c)))
                    record = rd(rd(s + 0x28) + level * 4u);
                kotor_fastcall_005ab1e0(stats, nullptr, ptr(record));                // 005AF5C7..005AF5CA
                info = word(recovered_server_info_004aec90(ptr(server()), nullptr)); // 005AF5CF..005AF5D7
                if (!(static_cast<int>(level) > static_cast<int>(rd(info + 0x90)))) break; // 005AF5DC..005AF5E9
            }
        }
    } else if (delta != 0) {                                                         // 005AF5EB (jle exit on 0)
        if (recovered_can_level_up_005a6810(stats, nullptr) != 0) {                  // 005AF5ED..005AF5F6
            do {
                const int top = static_cast<int>(rd(s + 0x2c)) - 1;                  // 005AF5F8 / 005AF607
                if (top < static_cast<int>(recovered_stats_level_005a5fd0(stats, nullptr, 0))) break; // 005AF5FB..005AF60A
                const U8 next = recovered_stats_level_005a5fd0(stats, nullptr, 0);   // 005AF60C..005AF610
                const U record = rd(rd(s + 0x28) + static_cast<U>(next) * 4);        // 005AF615..005AF61B
                if (record != 0) kotor_fastcall_005aabf0(stats, nullptr, ptr(record), 0); // 005AF61E..005AF627
            } while (recovered_can_level_up_005a6810(stats, nullptr) != 0);          // 005AF62C..005AF635
        }
    }
    // 005AF63B: level-up available notice when the previous total was below the next threshold
    if (player == nullptr) return;                                                   // 005AF63B..005AF642
    if (recovered_can_level_up_005a6810(stats, nullptr) == 0) return;                // 005AF644..005AF64D
    const U8 level = recovered_stats_level_005a5fd0(stats, nullptr, 0);              // 005AF64F..005AF658
    const U threshold = rd(rules() + static_cast<U>(level) * 4 + 0x38);              // 005AF65B..005AF660
    if (value - delta >= threshold) return;                                          // 005AF664..005AF668 (jae)
    recovered_creature_feedback_004ede10(ptr(rd(s + 0x24)), nullptr, 0xb, nullptr);  // 005AF66A..005AF673
}
