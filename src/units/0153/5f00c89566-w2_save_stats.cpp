#include "301b028a69-w2_save_stats.hpp"
#include <cstdint>
namespace w2::save_stats {
namespace {
using I = std::int32_t;
Word b8(Word v) { return v & 0xffu; }
Word sx8(Word v) { return static_cast<Word>(static_cast<I>(static_cast<std::int8_t>(v & 0xffu))); }
Word sx16(Word v) { return static_cast<Word>(static_cast<I>(static_cast<std::int16_t>(v & 0xffffu))); }
}

// 005CB0F0: skill < [rules+ABh] (16-bit unsigned compare) ? BYTE [[this+24h] + skill] : 0
Word skill_rank_change_005cb0f0(Memory& m, Ports& p, Word ls, Word skill) {
    if ((skill & 0xffffu) >= m.read8(p.rules_007a3a28() + 0xab)) return 0;    // 005CB105 jb (AX vs DX)
    return m.read8(m.read(ls + 0x24) + (skill & 0xffffu));
}

// 00550F30 CCombatInformation::SaveData. s+0x28 = original parent slot (receives the CombatInfo struct),
// s+0x24 = original gff slot (receives each list element), s+0x0C = list handle.
Word combat_info_save_00550f30(Memory& m, Ports& p, Word ci, Word gff, Word parent, Word s) {
    const Word st = s + 0x28, el = s + 0x24, list = s + 0x0c;
    m.write(st, parent);
    p.add_struct_to_struct_004125b0(gff, st, parent, p.text("CombatInfo"), 0xcaaa);
    p.write_byte_00412620(gff, st, m.read8(ci), p.text("NumAttacks"));
    p.write_char_00412670(gff, st, m.read8(ci + 1), p.text("OnHandAttackMod"));
    p.write_char_00412670(gff, st, m.read8(ci + 2), p.text("OnHandDamageMod"));
    p.write_char_00412670(gff, st, m.read8(ci + 3), p.text("OffHandAttackMod"));
    p.write_char_00412670(gff, st, m.read8(ci + 4), p.text("OffHandDamageMod"));
    p.write_byte_00412620(gff, st, m.read8(ci + 5), p.text("ForceResistance"));
    p.write_byte_00412620(gff, st, m.read8(ci + 6), p.text("ArcaneSpellFail"));
    p.write_byte_00412620(gff, st, m.read8(ci + 7), p.text("ArmorCheckPen"));
    p.write_byte_00412620(gff, st, m.read8(ci + 8), p.text("UnarmedDamDice"));
    p.write_byte_00412620(gff, st, m.read8(ci + 9), p.text("UnarmedDamDie"));
    p.write_byte_00412620(gff, st, m.read8(ci + 0x0a), p.text("OnHandCritRng"));
    p.write_byte_00412620(gff, st, m.read8(ci + 0x0b), p.text("OnHandCritMult"));
    p.write_byte_00412620(gff, st, m.read8(ci + 0x0c), p.text("OffHandWeaponEq"));
    p.write_byte_00412620(gff, st, m.read8(ci + 0x10), p.text("OffHandCritRng"));
    p.write_byte_00412620(gff, st, m.read8(ci + 0x11), p.text("OffHandCritMult"));
    p.write_dword_00412760(gff, st, m.read(ci + 0x30), p.text("LeftEquip"));
    p.write_dword_00412760(gff, st, m.read(ci + 0x2c), p.text("RightEquip"));
    p.write_string_00412970(gff, st, ci + 0x3c, p.text("LeftString"));
    p.write_string_00412970(gff, st, ci + 0x34, p.text("RightString"));
    p.write_byte_00412620(gff, st, m.read8(ci + 0x44), p.text("DamageDice"));
    p.write_byte_00412620(gff, st, m.read8(ci + 0x45), p.text("DamageDie"));
    p.add_list_00412450(gff, list, st, p.text("AttackList"));
    for (Word i = 0; static_cast<I>(i) < static_cast<I>(m.read(ci + 0x18)); ++i) {
        p.add_list_element_004124e0(gff, el, list, 0xaaee);
        const Word a = m.read(m.read(ci + 0x14) + 4 * i);
        p.write_char_00412670(gff, el, m.read8(a), p.text("Modifier"));
        p.write_byte_00412620(gff, el, m.read8(m.read(m.read(ci + 0x14) + 4 * i) + 2), p.text("WeaponWield"));
        p.write_byte_00412620(gff, el, m.read8(m.read(m.read(ci + 0x14) + 4 * i) + 3), p.text("VersusGoodEvil"));
        p.write_byte_00412620(gff, el, m.read8(m.read(m.read(ci + 0x14) + 4 * i) + 4), p.text("VersusRace"));
    }
    p.add_list_00412450(gff, list, st, p.text("DamageList"));
    for (Word i = 0; static_cast<I>(i) < static_cast<I>(m.read(ci + 0x24)); ++i) {
        p.add_list_element_004124e0(gff, el, list, 0xddee);
        p.write_char_00412670(gff, el, m.read8(m.read(m.read(ci + 0x20) + 4 * i)), p.text("Modifier"));
        p.write_byte_00412620(gff, el, m.read8(m.read(m.read(ci + 0x20) + 4 * i) + 1), p.text("ModifierType"));
        p.write_byte_00412620(gff, el, m.read8(m.read(m.read(ci + 0x20) + 4 * i) + 2), p.text("WeaponWield"));
        p.write_byte_00412620(gff, el, m.read8(m.read(m.read(ci + 0x20) + 4 * i) + 3), p.text("VersusGoodEvil"));
        p.write_byte_00412620(gff, el, m.read8(m.read(m.read(ci + 0x20) + 4 * i) + 4), p.text("VersusRace"));
    }
    return 1;
}

// 005AEC90 CSWSCreatureStats::SaveClassInfo. Handles at the original esp offsets (4 registers pushed, no
// outstanding pushes): class list BCh / element 20h; spells-per-day list 94h / element 28h; known list 58h /
// element 1Ch; level list A8h / element 18h; skill list 6Ch / element 38h; feat list 80h / element 2Ch;
// special-ability list D0h / element 34h; strings: key 48h, per-level known 50h, known-remove 3Ch.
void save_class_info_005aec90(Memory& m, Ports& p, Word st, Word gff, Word parent, Word s) {
    const Word key = s + 0x48;
    p.str_default_005b3190(key);
    p.add_list_00412450(gff, s + 0xbc, parent, p.text("ClassList"));
    Word c = 0;                                                                   // BL
    if (m.read8(st + 0x89) != 0) do {                                             // 005AECF2 / loop test 005AEEEF
        const Word entry = st + 0x8c + 0x28 * c;                                  // +A7h id, +A8h level, +A5h left, +8Ch list, +90h count
        const Word id = m.read8(entry + 0x1b);
        p.add_list_element_004124e0(gff, s + 0x20, s + 0xbc, 2);
        p.write_int_004127b0(gff, s + 0x20, id, p.text("Class"));
        const Word level = c < m.read8(st + 0x89) ? m.read8(entry + 0x1c) : 0u;  // setl/dec/and: level & FFh
        p.write_short_00412710(gff, s + 0x20, level, p.text("ClassLevel"));
        const Word cid = c < m.read8(st + 0x89) ? m.read8(entry + 0x1b) : 0xffu;
        if (p.is_jedi_005be4a0(cid) != 0) {
            p.add_list_00412450(gff, s + 0x94, s + 0x20, p.text("SpellsPerDayList"));
            p.add_list_element_004124e0(gff, s + 0x28, s + 0x94, 0x4567);
            const Word left = c < m.read8(st + 0x89) ? m.read8(entry + 0x19) : 0u;
            p.write_byte_00412620(gff, s + 0x28, left, p.text("NumSpellsLeft"));
        }
        if (c < m.read8(st + 0x89)) {
            const Word known = m.read(entry + 4);                                 // [eax+ebp+90h]
            if (static_cast<std::int16_t>(known & 0xffffu) > 0) {
                p.str_format_s_d_005e5680(key, p.text("KnownList"), 0);
                p.add_list_00412450(gff, s + 0x58, s + 0x20, p.str_cstr_005e5670(key));
                const I n = static_cast<std::int16_t>(known & 0xffffu);
                Word j = 0;                                                       // byte counter [esp+13h]
                if (n > 0) {
                    do {
                        p.add_list_element_004124e0(gff, s + 0x1c, s + 0x58, 3);
                        Word spell;
                        if (c >= m.read8(st + 0x89)) spell = 0xffffffffu;
                        else if (static_cast<I>(j) >= static_cast<I>(m.read(entry + 4))) spell = 0xffffffffu;
                        else spell = m.read(m.read(entry) + 4 * j);
                        p.write_word_004126c0(gff, s + 0x1c, spell, p.text("Spell"));
                        j = (j + 1) & 0xffu;
                    } while (static_cast<I>(j) < n);
                }
            }
        }
        c = (c + 1) & 0xffu;
    } while (c < m.read8(st + 0x89));
    if (m.read(st + 0x6c) != 0 && m.read(p.party_table_004aee70(m.read(p.app_007a39fc() + 8)) + 0xf0) == 0xffffffffu) {
        p.add_list_00412450(gff, s + 0xa8, parent, p.text("LvlStatList"));
        Word lvl = 0;
        if ((p.stats_level_005a5fd0(st, 0) & 0xffu) != 0) {
            for (;;) {
                Word ls = 0;
                if (m.read(st + 0x6c) != 0 && static_cast<I>(lvl) < static_cast<I>(m.read(st + 0x2c))) ls = m.read(m.read(st + 0x28) + 4 * lvl);
                p.add_list_element_004124e0(gff, s + 0x18, s + 0xa8, 0);
                if (ls == 0) p.trap(0x005aef8d, "W2: SaveClassInfo level-stat entry is null (original dereferences it)");
                const Word e = s + 0x18;
                if (lvl != 0 && m.read8(ls + 0x2a) != 6) p.write_byte_00412620(gff, e, m.read8(ls + 0x2a), p.text("LvlStatAbility"));
                p.write_byte_00412620(gff, e, m.read8(ls + 0x2b), p.text("LvlStatHitDie"));
                p.write_byte_00412620(gff, e, m.read8(ls + 0x2d), p.text("LvlStatForce"));
                p.write_byte_00412620(gff, e, m.read8(ls + 0x2c), p.text("LvlStatClass"));
                const Word kn = m.read8(ls + 4);
                if (kn != 0) {
                    const Word k2 = s + 0x50;
                    p.str_default_005b3190(k2);
                    p.str_format_s_d_005e5680(k2, p.text("KnownList"), 0);
                    p.add_list_00412450(gff, s + 0x58, e, p.str_cstr_005e5670(k2));
                    for (Word j = 0; j < kn; j = (j + 1) & 0xffu) {
                        p.add_list_element_004124e0(gff, s + 0x1c, s + 0x58, 0);
                        p.write_word_004126c0(gff, s + 0x1c, m.read(m.read(ls) + 4 * j), p.text("Spell"));
                    }
                    p.str_destroy_005e5c20(k2);
                }
                const Word kr = m.read8(ls + 0x10);
                if (kr != 0) {
                    const Word k3 = s + 0x3c;
                    p.str_default_005b3190(k3);
                    p.str_format_s_d_005e5680(k3, p.text("KnownRemoveList"), 0);
                    p.add_list_00412450(gff, s + 0x58, e, p.str_cstr_005e5670(k3));
                    for (Word j = 0; j < kr; j = (j + 1) & 0xffu) {
                        p.add_list_element_004124e0(gff, s + 0x1c, s + 0x58, 0);
                        p.write_word_004126c0(gff, s + 0x1c, m.read(m.read(ls + 0x0c) + 4 * j), p.text("Spell"));
                    }
                    p.str_destroy_005e5c20(k3);
                }
                p.write_word_004126c0(gff, e, m.read16(ls + 0x28), p.text("SkillPoints"));
                p.add_list_00412450(gff, s + 0x6c, e, p.text("SkillList"));
                for (Word k = 0; k < m.read8(p.rules_007a3a28() + 0xab); k = (k + 1) & 0xffu) {
                    p.add_list_element_004124e0(gff, s + 0x38, s + 0x6c, 0);
                    p.write_byte_00412620(gff, s + 0x38, skill_rank_change_005cb0f0(m, p, ls, k), p.text("Rank"));
                }
                p.add_list_00412450(gff, s + 0x80, e, p.text("FeatList"));
                for (Word k = 0; k < m.read16(ls + 0x1c); k = (k + 1) & 0xffffu) {
                    p.add_list_element_004124e0(gff, s + 0x2c, s + 0x80, 0);
                    p.write_word_004126c0(gff, s + 0x2c, m.read16(m.read(ls + 0x18) + 2 * k), p.text("Feat"));
                }
                lvl = (lvl + 1) & 0xffu;
                if (lvl >= (p.stats_level_005a5fd0(st, 0) & 0xffu)) break;     // 005AF252 jb
            }
        }
    }
    p.write_word_004126c0(gff, parent, m.read16(st + 0x164), p.text("SkillPoints"));
    p.add_list_00412450(gff, s + 0x6c, parent, p.text("SkillList"));
    for (Word k = 0; k < 8; ++k) {
        p.add_list_element_004124e0(gff, s + 0x38, s + 0x6c, 0);
        Word rank;
        if (k >= m.read8(p.rules_007a3a28() + 0xab)) rank = 0;
        else {
            const I v = static_cast<std::int8_t>(m.read8(m.read(st + 0x168) + k));
            rank = v >= 0x7f ? 0x7fu : v <= -0x7f ? 0x81u : static_cast<Word>(v) & 0xffu;
        }
        p.write_byte_00412620(gff, s + 0x38, rank, p.text("Rank"));
    }
    if (static_cast<I>(m.read(st + 4)) > 0) {
        p.add_list_00412450(gff, s + 0x80, parent, p.text("FeatList"));
        for (Word j = 0; static_cast<I>(j) < static_cast<I>(m.read(st + 4)); j = (j + 1) & 0xffffu) {
            p.add_list_element_004124e0(gff, s + 0x2c, s + 0x80, 1);
            const Word feat = static_cast<I>(j) < static_cast<I>(m.read(st + 4)) ? m.read16(m.read(st) + 2 * j) : 0xffffu;
            p.write_word_004126c0(gff, s + 0x2c, feat, p.text("Feat"));
        }
    }
    const Word sa = m.read(st + 0x128);
    if (static_cast<I>(m.read(sa + 4)) > 0) {
        p.add_list_00412450(gff, s + 0xd0, parent, p.text("SpecAbilityList"));
        for (Word j = 0; static_cast<I>(j) < static_cast<I>(m.read(m.read(st + 0x128) + 4)); ++j) {
            p.add_list_element_004124e0(gff, s + 0x34, s + 0xd0, 4);
            const Word rec = m.read(m.read(st + 0x128)) + 0x0c * j;
            p.write_word_004126c0(gff, s + 0x34, m.read16(rec), p.text("Spell"));
            p.write_byte_00412620(gff, s + 0x34, m.read8(m.read(m.read(st + 0x128)) + 0x0c * j + 4), p.text("SpellFlags"));
            p.write_byte_00412620(gff, s + 0x34, m.read8(m.read(m.read(st + 0x128)) + 0x0c * j + 8), p.text("SpellCasterLevel"));
        }
    }
    p.str_destroy_005e5c20(key);
}

// 005B1B90 CSWSCreatureStats::SaveStats. st = stats (this), creature = [st+24h].
void save_stats_005b1b90(Memory& m, Ports& p, Word st, Word gff, Word parent, Word s, Word s2) {
    const Word ps = parent;
    p.write_locstring_00412a10(gff, ps, st + 0x34, p.text("FirstName"));
    p.write_locstring_00412a10(gff, ps, st + 0x3c, p.text("LastName"));
    p.write_locstring_00412a10(gff, ps, st + 0x58, p.text("Description"));
    p.write_byte_00412620(gff, ps, m.read(st + 0x6c), p.text("IsPC"));
    p.write_string_00412970(gff, ps, m.read(st + 0x24) + 0x18, p.text("Tag"));
    for (Word k = 0; k < 16; k += 4) m.write(s + 0x1c + k, m.read(st + 0x44 + k));   // CResRef copy (005B1C0E)
    p.write_resref_004128c0(gff, ps, s + 0x1c, p.text("Conversation"));
    p.write_byte_00412620(gff, ps, m.read(st + 0x54), p.text("Interruptable"));
    p.write_int_004127b0(gff, ps, m.read(st + 0x60), p.text("Age"));
    p.write_byte_00412620(gff, ps, m.read8(st + 0x64), p.text("Gender"));
    p.write_byte_00412620(gff, ps, m.read16(st + 0xdc), p.text("Race"));
    p.write_string_00412970(gff, ps, st + 0xe0, p.text("Subrace"));
    p.write_byte_00412620(gff, ps, m.read8(st + 0xe8), p.text("SubraceIndex"));
    p.write_byte_00412620(gff, ps, m.read8(st + 0x88), p.text("StartingPackage"));
    p.write_string_00412970(gff, ps, st + 0x1a4, p.text("Deity"));
    p.write_byte_00412620(gff, ps, b8(m.read8(st + 0x89) - 1), p.text("MClassLevUpIn"));
    p.write_short_00412710(gff, ps, sx8(m.read8(st + 0x1a1)), p.text("willbonus"));
    p.write_short_00412710(gff, ps, sx8(m.read8(st + 0x1a0)), p.text("fortbonus"));
    p.write_short_00412710(gff, ps, sx8(m.read8(st + 0x1a2)), p.text("refbonus"));
    p.write_dword_00412760(gff, ps, p.gold_004edd60(m.read(st + 0x24)), p.text("Gold"));
    p.write_char_00412670(gff, ps, b8(p.reflex_save_005ab8f0(st, 0)), p.text("RefSaveThrow"));
    const Word out = s + 0x10;
    m.write(out, 0);
    Word eff = p.effect_bonus_004f3fe0(m.read(st + 0x24), 3, 0, out, 0, 0, 3, 0, 0xff, 0xff, 0);   // will
    Word will = p.base_will_save_005aa2f0(st) + eff + m.read8(st + 0x1a1) + m.read8(st + 0xf2);
    p.write_char_00412670(gff, ps, b8(will), p.text("WillSaveThrow"));
    m.write(out, 0);
    eff = p.effect_bonus_004f3fe0(m.read(st + 0x24), 3, 0, out, 0, 0, 1, 0, 0xff, 0xff, 0);        // fortitude
    const Word fort = p.base_fort_save_005aa1b0(st) + eff + m.read8(st + 0x1a0) + m.read8(st + 0xee);
    p.write_char_00412670(gff, ps, b8(fort), p.text("FortSaveThrow"));
    p.write_short_00412710(gff, ps, p.armor_class_004ed1d0(m.read(st + 0x24)), p.text("ArmorClass"));
    p.write_byte_00412620(gff, ps, m.read8(st + 0xe9), p.text("Str"));
    p.write_byte_00412620(gff, ps, m.read8(st + 0xeb), p.text("Dex"));
    p.write_byte_00412620(gff, ps, m.read8(st + 0xef), p.text("Int"));
    p.write_byte_00412620(gff, ps, m.read8(st + 0xf1), p.text("Wis"));
    p.write_byte_00412620(gff, ps, m.read8(st + 0xed), p.text("Con"));
    p.write_byte_00412620(gff, ps, m.read8(st + 0xf3), p.text("Cha"));
    p.write_byte_00412620(gff, ps, m.read8(st + 0xf5), p.text("NaturalAC"));
    const Word c = m.read(st + 0x24);
    p.write_word_004126c0(gff, ps, m.read16(c + 0x9d8), p.text("SoundSetFile"));
    p.write_byte_00412620(gff, ps, m.read(m.read(st + 0x24) + 0xf8), p.text("Plot"));
    p.write_byte_00412620(gff, ps, m.read(m.read(st + 0x24) + 0x4f4), p.text("Disarmable"));
    p.write_byte_00412620(gff, ps, m.read(m.read(st + 0x24) + 0x200), p.text("Min1HP"));
    p.write_byte_00412620(gff, ps, m.read(m.read(st + 0x24) + 0x218), p.text("PartyInteract"));
    p.write_byte_00412620(gff, ps, m.read(m.read(st + 0x24) + 0x21c) == 0 ? 1u : 0u, p.text("NotReorienting"));
    p.write_byte_00412620(gff, ps, m.read8(m.read(st + 0x24) + 0x9da), p.text("BodyBag"));
    p.write_short_00412710(gff, ps, sx16(m.read16(m.read(st + 0x24) + 0xe0)), p.text("HitPoints"));
    {
        const Word cr = m.read(st + 0x24);
        Word slot = m.read16(cr + 0xe0);
        slot = (slot - p.vcall1(cr, 0x98, 1)) & 0xffffu;
        const Word cur = slot + p.vcall1(cr, 0x9c, 1);
        p.write_short_00412710(gff, ps, cur, p.text("CurrentHitPoints"));
    }
    p.write_short_00412710(gff, ps, p.vcall1(m.read(st + 0x24), 0x98, 1), p.text("MaxHitPoints"));
    p.write_short_00412710(gff, ps, p.vcall1(m.read(st + 0x24), 0x9c, 1), p.text("PregameCurrent"));
    p.write_short_00412710(gff, ps, sx16(m.read16(m.read(m.read(st + 0x24) + 0xa74) + 0x122)), p.text("ForcePoints"));
    {
        const Word fs = m.read(m.read(st + 0x24) + 0xa74);
        const Word fp = m.read16(fs + 0x122);
        const Word maxfp = p.max_force_points_004fd490(m.read(st + 0x24));
        const Word sum = (m.read16(fs + 0x124) + fp) & 0xffffu;               // 16-bit add (cx)
        p.write_short_00412710(gff, ps, sum - maxfp, p.text("CurrentForce"));
    }
    p.write_short_00412710(gff, ps, p.max_force_points_004fd490(m.read(st + 0x24)), p.text("MaxForcePoints"));
    p.write_dword_00412760(gff, ps, m.read(st + 0x68), p.text("Experience"));
    p.write_byte_00412620(gff, ps, m.read(st + 0x194), p.text("MovementRate"));
    if ((p.vcall0(m.read(st + 0x24), 0xd0) & 0xffffu) == 0xffffu) {
        const Word ref = p.vcall1(m.read(st + 0x24), 0xc8, s + 0x2c);
        p.write_resref_004128c0(gff, ps, ref, p.text("Portrait"));
    } else {
        p.write_word_004126c0(gff, ps, p.vcall0(m.read(st + 0x24), 0xd0), p.text("PortraitId"));
    }
    p.write_byte_00412620(gff, ps, m.read16(st + 0x17e), p.text("GoodEvil"));
    p.write_byte_00412620(gff, ps, m.read8(st + 0x182), p.text("Color_Skin"));
    p.write_byte_00412620(gff, ps, m.read8(st + 0x183), p.text("Color_Hair"));
    p.write_byte_00412620(gff, ps, m.read8(st + 0x184), p.text("Color_Tattoo1"));
    p.write_byte_00412620(gff, ps, m.read8(st + 0x185), p.text("Color_Tattoo2"));
    p.write_int_004127b0(gff, ps, m.read8(st + 0x188), p.text("Phenotype"));
    p.write_word_004126c0(gff, ps, m.read16(st + 0x186), p.text("Appearance_Type"));
    p.write_byte_00412620(gff, ps, m.read8(st + 0x189), p.text("Appearance_Head"));
    p.write_byte_00412620(gff, ps, m.read8(st + 0x18a), p.text("DuplicatingHead"));
    p.write_byte_00412620(gff, ps, m.read(st + 0x18c), p.text("UseBackupHead"));
    p.write_byte_00412620(gff, ps, m.read8(st + 0x190), p.text("Tail"));
    p.write_byte_00412620(gff, ps, m.read8(st + 0x191), p.text("Wings"));
    p.write_word_004126c0(gff, ps, m.read(st + 0x78), p.text("FactionID"));
    p.write_float_00412870(gff, ps, m.read(st + 0x84), p.text("ChallengeRating"));
    p.write_int_004127b0(gff, ps, m.read16(st + 0x13a), p.text("AIState"));
    save_class_info_005aec90(m, p, st, gff, parent, s2);
    const Word str = s + 0x14;
    p.str_default_005b3190(str);
    const Word server = m.read(p.app_007a39fc() + 8);
    const Word left = p.item_by_id_004ae760(server, m.read(m.read(st + 0x118) + 0x30));
    if (left != 0) p.locstring_get_005ea130(left + 0x280, 0, m.read(st + 0x118) + 0x3c, 0);
    const Word right = p.item_by_id_004ae760(m.read(p.app_007a39fc() + 8), m.read(m.read(st + 0x118) + 0x2c));
    if (right != 0) {
        p.locstring_get_005ea130(right + 0x280, 0, m.read(st + 0x118) + 0x34, 0);
        const Word base = p.base_item_005b31a0(m.read(p.rules_007a3a28() + 0x34), m.read(right + 0x0c));
        if (base != 0) {
            const Word ci = m.read(st + 0x118);
            const Word d0 = m.read8(base + 0x1c), d1 = m.read8(base + 0x1d);
            m.write16(ci + 0x44, d0 | (d1 << 8));                                // 005B2356 / 005B2362 (two byte stores)
        }
    }
    (void)combat_info_save_00550f30(m, p, m.read(st + 0x118), gff, parent, s + 0x24);
    p.str_destroy_005e5c20(str);
}
}
