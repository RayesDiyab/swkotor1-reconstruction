#pragma once
// W2 tier 4 (G07 player update): CSWSMessage::WriteGameObjUpdate_PlayerUpdate 00572DC0 ('I' record, written
// from MajorGUIPanels 00577670) and its small unowned callees: CSWSCreatureStats::CanLevelUp 005A6810,
// ComputeLastUpdate_GuiSkills 00567430, ComputeLastUpdate_PlayerState 00567600,
// ComputeLastUpdate_WriteSpellLikeAbility 0056C4D0, CSWSPlayerLastUpdateObject::Get/SetKnownSpellUsesLeft 005CA070 / 005CA0A0 and
// CSWSCreatureStats::GetSpellsPerDayLeft 005A5650. Logic over original 32-bit address words.
//
// The original keeps two scratch arrays in stale stack slots (flags 80h and 40h): a WORD array at
// [esp+14h] (only words 0 and 1 are zeroed; word 2/3 alias the saved `this` at [esp+18h]) and a BYTE array
// at the `player` argument slot [esp+4Ch] (only bytes 0 and 1 are zeroed). The logic therefore runs over an
// emulated frame: frame+N == [esp+N] after the prologue's last push (EDI). Callers pass a 0x80-byte frame.
#include "72239a78a7-w2_entry.hpp"
namespace w2::g07 {
using w2::entry::Word;
using w2::entry::Memory;

struct PlayerPorts {
    virtual ~PlayerPorts() = default;
    [[noreturn]] virtual void trap(Word address, const char* what) = 0;
    virtual Word armor_class_004ed1d0(Word creature) = 0;                       // AX (w2_g07_charsheet)
    virtual Word gold_004edd60(Word creature) = 0;                              // w2x_creature_gold_004edd60
    virtual Word skill_rank_005aa570(Word stats, Word skill, Word a, Word b) = 0;   // AL; @w1x_creature_skill_rank_005aa570@20
    virtual Word can_use_skill_005af880(Word stats, Word skill) = 0;            // w2_g07_charsheet
    virtual Word stats_level_005a5fd0(Word stats, Word flag) = 0;               // AL; @recovered_stats_level_005a5fd0@12
    virtual Word server_info_004aec90(Word server) = 0;                         // W1 tier6 @recovered_server_info_004aec90@8
    virtual Word dead_temp_004ef890(Word creature) = 0;                         // @recovered_creature_incapacitated_004ef890@8
    virtual Word detect_mode_004edae0(Word creature) = 0;                       // AL; @recovered_creature_detect_mode_004edae0@8
    virtual Word get_class_005a4e90(Word stats, Word index) = 0;                // AL; kotor::server_mainloop::creature_get_class_005a4e90
    virtual Word is_jedi_005be4a0(Word cls) = 0;                                // cdecl; kotor::server_mainloop::creature_force_class_005be4a0
    virtual Word get_feat_005a6920(Word stats, Word index) = 0;                 // AX (w2_g07_charsheet)
    virtual Word bonus_feat_005a6940(Word stats, Word index) = 0;               // AX (w2_g07_charsheet)
    virtual Word feat_remaining_uses_005a6680(Word stats, Word feat) = 0;       // AL (w2_g07_appearance)
    virtual Word str_stat_005a6190(Word stats) = 0;                             // AL (w2_g07_charsheet ability 0)
    virtual Word dex_stat_005a6550(Word stats) = 0;                             // ability 1
    virtual Word con_stat_005a6250(Word stats) = 0;                             // ability 2
    virtual Word int_stat_005a6310(Word stats) = 0;                             // ability 3
    virtual Word wis_stat_005a63d0(Word stats) = 0;                             // ability 4
    virtual Word cha_stat_005a6490(Word stats) = 0;                             // ability 5
    virtual Word will_save_005ab880(Word stats, Word flag) = 0;                 // AL (w2_g07_charsheet)
    virtual Word reflex_save_005ab8f0(Word stats, Word flag) = 0;
    virtual Word fort_save_005ab810(Word stats, Word flag) = 0;
    virtual Word resref_str_00405fe0(Word ref) = 0;                             // char*; @c2da_ref_00405fe0@8
    virtual Word max_force_points_004fd490(Word creature) = 0;                  // AX; @w1x_creature_max_fp_004fd490@8
    virtual Word compare_spell_like_0056c430(Word self, Word creature, Word plast) = 0;   // RET 8; w2_g07_playerlast
};

// 00572DC0 WriteGameObjUpdate_PlayerUpdate(player, player_last, last, flags) RET 10h. frame >= 0x80 bytes.
void player_update_00572dc0(Memory&, w2::entry::Ports&, PlayerPorts&, Word self, Word player, Word plast, Word last, Word flags, Word frame);
// small unowned callees (W2 bodies)
Word can_level_up_005a6810(Memory&, w2::entry::Ports&, PlayerPorts&, Word stats);                 // RET 0
Word gui_skills_00567430(Memory&, PlayerPorts&, Word creature);                                     // RET 4, AX
Word player_state_00567600(Memory&, PlayerPorts&, Word creature);                                   // RET 4, AX
Word known_spell_uses_005ca070(Memory&, Word plast, Word cls, Word level);                          // RET 8, AL
void set_known_spell_uses_005ca0a0(Memory&, Word plast, Word cls, Word level, Word value);          // RET 0Ch
Word spells_per_day_left_005a5650(Memory&, Word stats, Word cls, Word level);                       // RET 8, AL
// 0056C4D0 ComputeLastUpdate_WriteSpellLikeAbility(creature, player_last) RET 8
void spell_like_ability_0056c4d0(Memory&, w2::entry::Ports&, PlayerPorts&, Word self, Word creature, Word plast);
}
