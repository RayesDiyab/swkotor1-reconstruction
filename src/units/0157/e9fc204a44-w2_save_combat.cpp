#include "cdab552184-w2_save_combat.hpp"
#include <cstdint>
namespace w2::save_combat {
namespace {
using I = std::int32_t;
struct Field { Word writer, offset; const char* label; };
// value as the original pushes it: movzx BYTE / movzx WORD / dword; CExoString and Vector pass the address
Word value_of(Memory& m, Word self, const Field& f) {
    switch (f.writer) {
    case kByte: case kChar: return m.read8(self + f.offset);
    case kWord: case kShort: return m.read16(self + f.offset);
    case kString: case kVector: return self + f.offset;
    default: return m.read(self + f.offset);
    }
}
template <unsigned N> Word write_all(Memory& m, Ports& p, Word self, Word gff, Word st, const Field (&fields)[N]) {
    Word r = 0;
    for (const Field& f : fields) r = p.write_field(f.writer, gff, st, value_of(m, self, f), f.label);
    return r;
}
const Field kRoundHead[] = {
    {kByte, 0x934, "RoundStarted"}, {kByte, 0x938, "SpellCastRound"}, {kByte, 0x978, "DeflectArrow"}, {kByte, 0x97c, "WeaponSucks"},
    {kDword, 0x9ac, "DodgeTarget"}, {kDword, 0x98c, "NewAttackTarget"}, {kInt, 0x9b8, "Engaged"}, {kInt, 0x9bc, "Master"},
    {kDword, 0x9c0, "MasterID"}, {kByte, 0x958, "RoundPaused"}, {kDword, 0x95c, "RoundPausedBy"}, {kByte, 0x964, "InfinitePause"},
    {kInt, 0x960, "PauseTimer"}, {kInt, 0x944, "Timer"}, {kInt, 0x94c, "RoundLength"}, {kInt, 0x950, "OverlapAmount"},
    {kInt, 0x954, "BleedTimer"}, {kByte, 0x96c, "CurrentAttack"}, {kWord, 0x930, "AttackID"}, {kByte, 0x974, "AttackGroup"},
    {kInt, 0x980, "ParryIndex"}, {kInt, 0x984, "NumAOOs"}, {kInt, 0x988, "NumCleaves"}, {kInt, 0x990, "OnHandAttacks"},
    {kInt, 0x994, "OffHandAttacks"}, {kInt, 0x9a0, "AdditAttacks"}, {kInt, 0x9a4, "EffectAttacks"}, {kByte, 0x9a8, "ParryActions"},
    {kInt, 0x998, "OffHandTaken"}, {kInt, 0x99c, "ExtraTaken"},
};
const Field kAttackHead[] = {
    {kByte, 0x00, "AttackGroup"}, {kWord, 0x08, "AnimationLength"}, {kDword, 0x18, "MissedBy"}, {kByte, 0x5c, "AttackResult"},
    {kDword, 0x0c, "ReactObject"}, {kWord, 0x10, "ReaxnDelay"}, {kWord, 0x12, "ReaxnAnimation"}, {kWord, 0x14, "ReaxnAnimLength"},
    {kByte, 0x3c, "Concealment"}, {kWord, 0x64, "AttackType"}, {kByte, 0x3b, "AttackMode"}, {kInt, 0x40, "RangedAttack"},
    {kInt, 0x44, "SneakAttack"}, {kByte, 0x3a, "WeaponAttackType"}, {kFloat, 0x68, "RangedTargetX"}, {kFloat, 0x6c, "RangedTargetY"},
    {kFloat, 0x70, "RangedTargetZ"},
};
const Field kAttackTail[] = {
    {kByte, 0x48, "KillingBlow"}, {kByte, 0x4c, "CoupDeGrace"}, {kByte, 0x50, "CriticalThreat"}, {kByte, 0x58, "AttackDeflected"},
    {kDword, 0x74, "AmmoItem"}, {kString, 0x78, "AttackDebugText"}, {kString, 0x80, "DamageDebugText"},
};
const Field kAction[] = {
    {kInt, 0x00, "ActionTimer"}, {kWord, 0x04, "Animation"}, {kInt, 0x08, "AnimationTime"}, {kInt, 0x0c, "NumAttacks"},
    {kByte, 0x10, "ActionType"}, {kDword, 0x14, "Target"}, {kByte, 0x18, "Retargettable"}, {kDword, 0x1c, "InventorySlot"},
    {kDword, 0x20, "TargetRepository"},
};
const Field kFollow[] = {
    {kDword, 0x00, "FollowObject"}, {kVector, 0x04, "FollowLocation"}, {kVector, 0x10, "LastLeaderPos"}, {kVector, 0x1c, "LastFollowerPos"},
    {kFloat, 0x28, "MaxSpeed"}, {kByte, 0x2c, "StickToPos"}, {kDword, 0x30, "Result"}, {kDword, 0x34, "TimeElapsed"},
    {kByte, 0x38, "InSafetyRange"},
};
// counted WORD list (004D41A6 / 004D4203): signed count, element type, one WORD field per element
void word_list(Memory& m, Ports& p, Word round, Word gff, Word parent, Word list, Word elem, const char* name,
               Word data_off, Word count_off, Word type, const char* field) {
    (void)p.add_list(gff, list, parent, name);
    for (Word i = 0; static_cast<I>(i) < static_cast<I>(m.read(round + count_off)); ++i) {
        (void)p.add_list_element(gff, elem, list, type);
        (void)p.write_field(kWord, gff, elem, m.read16(m.read(round + data_off) + 2 * i), field);
    }
}
}

Word round_action_save_004d1dd0(Memory& m, Ports& p, Word action, Word gff, Word st) {
    (void)write_all(m, p, action, gff, st, kAction);
    return 1;
}

Word attack_data_save_004d2210(Memory& m, Ports& p, Word attack, Word gff, Word st, Word s) {
    (void)write_all(m, p, attack, gff, st, kAttackHead);
    const Word list = s, elem = s + 0x14;
    (void)p.add_list(gff, list, st, "DamageList");                        // 004D2367
    for (Word i = 0; i < 15; ++i) {                                       // 004D236F: 0Fh elements, do-while
        (void)p.add_list_element(gff, elem, list, 0xdaee);
        (void)p.write_field(kShort, gff, elem, m.read16(attack + 0x1c + 2 * i), "DamageValue");   // movzx: zero-extended
    }
    (void)write_all(m, p, attack, gff, st, kAttackTail);
    return 1;
}

Word save_combat_round_004d3ec0(Memory& m, Ports& p, Word round, Word gff, Word st, Word s) {
    (void)write_all(m, p, round, gff, st, kRoundHead);
    const Word list = s, elem = s + 0x14, pos = s + 0x18;
    (void)p.add_list(gff, list, st, "AttackList");                        // 004D414B
    for (Word i = 0; i < 5; ++i) {                                        // 004D4153: 5 attacks, stride 14Ch from +4
        (void)p.add_list_element(gff, elem, list, 0xaaee);
        (void)attack_data_save_004d2210(m, p, round + 4 + 0x14c * i, gff, elem, s + 0x20);
    }
    word_list(m, p, round, gff, st, list, elem, "SpecAttackList", 0x918, 0x91c, 0xdaee, "SpecialAttack");
    word_list(m, p, round, gff, st, list, elem, "SpecAttackIdList", 0x924, 0x928, 0xdaae, "SpecialAttackId");
    (void)p.add_list(gff, list, st, "SchedActionList");                   // 004D425C
    const Word internal = m.read(m.read(round + 0x9b0));
    m.write(pos, m.read(internal));                                       // head position
    while (m.read(pos) != 0) {
        const Word action = p.list_at_pos_005e9b60(m.read(m.read(round + 0x9b0)), m.read(pos));
        if (action != 0) {
            (void)p.add_list_element(gff, elem, list, 0xdaea);
            (void)round_action_save_004d1dd0(m, p, action, gff, elem);
        }
        p.list_next_005e9b90(m.read(m.read(round + 0x9b0)), pos);
    }
    return 1;
}

Word save_listen_data_004cca50(Memory& m, Ports& p, Word o, Word gff, Word parent, Word s) {
    if (static_cast<I>(m.read(o + 0x190)) <= 0) return m.read(o + 0x190);   // EAX = count (004CCA5E)
    const Word list = s, elem = s + 0x14;
    (void)p.add_list(gff, list, parent, "ExpressionList");
    for (Word i = 0; static_cast<I>(i) < static_cast<I>(m.read(o + 0x190)); ++i) {
        (void)p.add_list_element(gff, elem, list, 5);
        const Word e = m.read(m.read(o + 0x18c) + 4 * i);
        (void)p.write_field(kInt, gff, elem, m.read(e), "ExpressionId");
        (void)p.write_field(kString, gff, elem, m.read(m.read(o + 0x18c) + 4 * i) + 0x18, "ExpressionString");
    }
    return m.read(o + 0x190);
}

Word follow_info_save_004eaf70(Memory& m, Ports& p, Word info, Word gff, Word st) {
    return write_all(m, p, info, gff, st, kFollow);                     // EAX = last WriteFieldBYTE result
}
}
