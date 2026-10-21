#include "55014e1687-w2_save_creature.hpp"
#include <cstdint>
namespace w2::save {
namespace {
using I = std::int32_t;
struct ScriptField { Word offset; const char* label; };
// 005006C3..0050089A: CResRef(CExoString at creature+off) then WriteFieldCResRef(label)
constexpr ScriptField kScripts[] = {
    {0x230, "ScriptHeartbeat"}, {0x238, "ScriptOnNotice"}, {0x240, "ScriptSpellAt"}, {0x248, "ScriptAttacked"},
    {0x250, "ScriptDamaged"}, {0x258, "ScriptDisturbed"}, {0x260, "ScriptEndRound"}, {0x268, "ScriptDialogue"},
    {0x270, "ScriptSpawn"}, {0x278, "ScriptRested"}, {0x280, "ScriptDeath"}, {0x288, "ScriptUserDefine"},
    {0x290, "ScriptOnBlocked"}, {0x298, "ScriptEndDialogue"}};
}

// Original locals (sub esp,48h + 4 regs): +10h list element struct, +14h perception element, +18h CombatRoundData
// struct, +1Ch saved [creature+A88h], +20h CResRef (16 bytes), +30h list (Equip_ItemList / ItemList), +44h
// PerceptionList list; the FollowInfo struct reuses the st argument slot (s + 0x48 here).
Word save_creature_00500610(Memory& m, CreaturePorts& p, Word c, Word gff, Word st, Word s) {
    const Word elem = s + 0x10, pelem = s + 0x14, round_st = s + 0x18, ref = s + 0x20, list = s + 0x30, plist = s + 0x44, follow_st = s + 0x48;
    const Word saved_party = m.read(c + 0xa88);                                // 00500619
    p.set_in_party_004fdb20(c, 0, 1);                                          // 00500627
    p.save_stats_005b1b90(m.read(c + 0xa74), gff, st);                         // 0050063C
    p.write_byte_00412620(gff, st, m.read8(c + 0x4d0), "DetectMode");
    p.write_byte_00412620(gff, st, m.read8(c + 0x4d1), "StealthMode");
    p.write_int_004127b0(gff, st, m.read(c + 0x4f8), "CreatureSize");
    p.write_byte_00412620(gff, st, m.read8(c + 0xec), "IsDestroyable");
    p.write_byte_00412620(gff, st, m.read8(c + 0xf0), "IsRaiseable");
    p.write_byte_00412620(gff, st, m.read8(c + 0xf4), "DeadSelectable");
    for (const ScriptField& f : kScripts) {
        p.ref_from_string_00406d60(ref, c + f.offset);
        p.write_resref_004128c0(gff, st, ref, f.label);
    }
    // 0050089F: equipped items, slot masks 1 << 0 .. 1 << 17; the list element type is the slot mask
    p.add_list_00412450(gff, list, st, "Equip_ItemList");
    for (Word mask = 1, n = 0x12; n != 0; mask += mask, --n) {
        const Word item = p.item_in_slot_005a4c20(m.read(c + 0xa2c), mask);
        if (item == 0) continue;
        p.add_list_element_004124e0(gff, elem, list, mask);
        p.write_dword_00412760(gff, elem, m.read(item + 0x14), "ObjectId");
        p.save_item_0055ccd0(item, gff, elem);
    }
    // 00500925: inventory (unsigned count, re-read through GetItemRepository each pass)
    p.add_list_00412450(gff, list, st, "ItemList");
    for (Word i = 0; i < m.read(p.item_repository_004ef770(c, 1) + 0x10); ++i) {
        const Word item = p.repository_item_005560b0(p.item_repository_004ef770(c, 1), i);
        p.add_list_element_004124e0(gff, elem, list, 0);
        if (item == 0) p.trap(0x0050097c, "W2: SaveCreature inventory slot without an item (original dereferences null)");
        p.write_dword_00412760(gff, elem, m.read(item + 0x14), "ObjectId");
        p.save_item_0055ccd0(item, gff, elem);
    }
    // 005009AF: perception list [creature+920h] (count +924h, signed); entries {id, flags byte}
    p.add_list_00412450(gff, plist, st, "PerceptionList");
    for (Word i = 0; static_cast<I>(i) < static_cast<I>(m.read(c + 0x924)); ++i) {
        const Word e = m.read(m.read(c + 0x920) + 4 * i);
        if (e == 0) continue;
        p.add_list_element_004124e0(gff, pelem, plist, 0);
        p.write_dword_00412760(gff, pelem, m.read(e), "ObjectId");
        const Word b = m.read8(e + 4);
        p.write_byte_00412620(gff, pelem, (b | ((b >> 3) & 2u)) & 0x0fu, "PerceptionData");   // 00500A0D
    }
    p.add_struct_to_struct_004125b0(gff, round_st, st, "CombatRoundData", 0xcada);   // 00500A52
    if (m.read(m.read(c + 0x9c8) + 0x934) == 1)
        p.save_combat_round_004d3ec0(m.read(c + 0x9c8), gff, round_st);       // 00500A72
    p.write_dword_00412760(gff, st, m.read(c + 0x8c), "AreaId");
    p.write_byte_00412620(gff, st, m.read8(c + 0x4c4), "AmbientAnimState");
    p.write_int_004127b0(gff, st, m.read(c + 0xd4), "Animation");
    p.write_byte_00412620(gff, st, m.read8(c + 0x34c), "CreatnScrptFird");
    p.write_byte_00412620(gff, st, m.read8(c + 0xa48), "PM_IsDisguised");
    if (m.read(c + 0xa48) == 1) p.write_word_004126c0(gff, st, m.read16(c + 0xa4c), "PM_Appearance");   // full dword compare
    p.write_byte_00412620(gff, st, m.read8(c + 0x19c), "Listening");
    p.save_listen_data_004cca50(c, gff, st);                                    // 00500B1A
    p.write_float_00412870(gff, st, m.read(c + 0x90), "XPosition");
    p.write_float_00412870(gff, st, m.read(c + 0x94), "YPosition");
    p.write_float_00412870(gff, st, m.read(c + 0x98), "ZPosition");
    p.write_float_00412870(gff, st, m.read(c + 0x9c), "XOrientation");
    p.write_float_00412870(gff, st, m.read(c + 0xa0), "YOrientation");
    p.write_float_00412870(gff, st, m.read(c + 0xa4), "ZOrientation");
    p.write_int_004127b0(gff, st, m.read(c + 0x22c), "JoiningXP");
    p.set_in_party_004fdb20(c, saved_party, 1);                                // 00500BB3
    if (m.read(c + 0x4c0) != 0) {
        p.add_struct_to_struct_004125b0(gff, follow_st, st, "FollowInfo", 0);
        p.follow_info_save_004eaf70(m.read(c + 0x4c0), gff, follow_st);         // 00500BE2
    }
    p.save_object_state_004cec50(c, gff, st);                                   // 00500BEB
    return 1;
}
}
