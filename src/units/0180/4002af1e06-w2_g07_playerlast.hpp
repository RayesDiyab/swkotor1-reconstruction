#pragma once
// W2 tier 4 (G07 player last-update object): CSWSPlayerLastUpdateObject ctor 005CAC80 / dtor 005CAD70 and its
// clears (005CA0D0 ClearActionQueue, 005CAB20 ClearKnownSpells, 005CA060 ClearKnownSpellUsesLeft,
// 005CAC30 ClearSpellAddDeleteLists, 005CAB60 ClearFeats, 005CA780 ClearVisibilityList), AddKnownSpell 005CABE0,
// GetKnownSpell 005CA740, CSWSMessage::TestPlayerUpdateDifferences 00572A20,
// StoreValuesInLastPlayerUpdateObject 00574400, ComputeLastUpdate_GuiKnownSpells 00570930, and the small
// unowned callees listed below. Logic over original 32-bit address words.
//
// CSWSPlayerLastUpdateObject layout (0x1B4 bytes; instruction = first reader/writer seen):
//   +000 dword gold (00574446)                 +004 word armor class (0057443D)   +006 word creature+A44 (0057444F)
//   +008 dword CanLevelUp (0057445E; ctor 0)   +00C dword level (00574471)         +010 dword XP stats+68 (0057447D)
//   +014 word GuiSkills (00574497)
//   +018/+01C/+020 CExoArrayList<WORD> feats (005744C6; ctor 0; ClearFeats 005CAB64)
//   +024/+028/+02C CExoArrayList<WORD> bonus feats (00574567)
//   +030/+034/+038 CExoArrayList<{WORD,BYTE}*> feat uses, 4-byte heap elements (00574640; ClearFeats frees each)
//   +03C word PlayerState (005744A8)
//   +040 10 x CSWActionNode (0x1C each: +0/+2/+4 WORD, +8 DWORD, +0C Vector, +18 object id) (005CACCD, 005CA0E2)
//   +158/+15C/+160 CExoArrayList<{DWORD id, BYTE bits}*> visibility list, 8-byte heap elements (005CA780, 005707C0)
//   +164 2 x CExoArrayList<DWORD> known spells per class (005CAB23, 005CA765)
//   +17C 2 x CExoArrayList<DWORD> known-spell delete lists (0057099B..00570A4F, 005CAC40)
//   +194 2 x CExoArrayList<DWORD> known-spell add lists (00570AB1..00570B30, 005CAC58)
//   +1AC word known-spell uses left (005CA066)
//   +1B0 CExoArrayList<SpellLikeAbility(0x0C)>* heap list (00570BC6, 0056C43B, 005CADA3)
#include "72239a78a7-w2_entry.hpp"
namespace w2::g07 {
using w2::entry::Word;
using w2::entry::Memory;

struct PlayerLastPorts {
    virtual ~PlayerLastPorts() = default;
    [[noreturn]] virtual void trap(Word address, const char* what) = 0;
    virtual void write8(Word address, Word value) = 0;
    virtual Word sla_construct_006475c0(Word self) = 0;                 // W3 @recovered_sla_construct_006475c0@8 (t6b)
    virtual Word allocate_006fa7e6(Word bytes) = 0;                     // live_operator_new
    virtual void free_006fa390(Word block) = 0;                         // live_free
    virtual void array_construct_005a76d0(Word list) = 0;               // ?exo_array_list_construct_005a76d0@@YIPAXPAX0@Z
    virtual void array_destruct_0056b6f0(Word list) = 0;                // @exo_array_list_destruct_0056b6f0@8
    virtual void list_allocate_005e03c0(Word list, Word capacity) = 0;  // menu_list_growth::allocate_005e03c0
    virtual void list_u16_allocate_004d2fa0(Word list, Word capacity) = 0;   // @kotor_m15_array_u16_allocate_004d2fa0@12
    virtual Word rules_007a3a28() = 0;
    virtual Word gui_skills_00567430(Word creature) = 0;                // AX (w2_g07_player)
    virtual Word player_state_00567600(Word creature) = 0;              // AX (w2_g07_player)
    virtual Word known_spell_uses_005ca070(Word plast, Word cls, Word level) = 0;      // AL (w2_g07_player)
    virtual Word spells_per_day_left_005a5650(Word stats, Word cls, Word level) = 0;   // AL (w2_g07_player)
    virtual Word can_level_up_005a6810(Word stats) = 0;                 // w2_g07_player
    virtual Word get_class_005a4e90(Word stats, Word index) = 0;        // AL; kotor::server_mainloop::creature_get_class_005a4e90
    virtual Word is_jedi_005be4a0(Word cls) = 0;                        // kotor::server_mainloop::creature_force_class_005be4a0
    virtual Word gold_004edd60(Word creature) = 0;                      // w2x_creature_gold_004edd60
    virtual Word armor_class_004ed1d0(Word creature) = 0;               // AX (w2_g07_charsheet)
    virtual Word stats_level_005a5fd0(Word stats, Word flag) = 0;       // AL; @recovered_stats_level_005a5fd0@12
    virtual Word str_stat_005a6190(Word stats) = 0;                     // AL (w2_g07_charsheet)
    virtual Word dex_stat_005a6550(Word stats) = 0;
    virtual Word con_stat_005a6250(Word stats) = 0;
    virtual Word int_stat_005a6310(Word stats) = 0;
    virtual Word wis_stat_005a63d0(Word stats) = 0;
    virtual Word cha_stat_005a6490(Word stats) = 0;
    virtual Word skill_rank_005aa570(Word stats, Word skill, Word a, Word b) = 0;      // AL; @w1x_creature_skill_rank_005aa570@20
    virtual Word resref_ne_00406dc0(Word a, Word b) = 0;                // @gui_load_resref_not_equal_00406dc0@12
    virtual void resref_assign_00406120(Word self, Word result, Word source) = 0;     // CResRef::operator=
    virtual Word max_force_points_004fd490(Word creature) = 0;          // AX; @w1x_creature_max_fp_004fd490@8
    virtual Word get_feat_005a6920(Word stats, Word index) = 0;         // AX (w2_g07_charsheet)
    virtual Word bonus_feat_005a6940(Word stats, Word index) = 0;       // AX (w2_g07_charsheet)
    virtual Word vector_equal_004aa980(Word a, Word b) = 0;             // cdecl; _aurora_vector_equal_004aa980
};

// ---- CSWSPlayerLastUpdateObject members
Word player_last_construct_005cac80(Memory&, PlayerLastPorts&, Word self);              // RET 0 -> self
void player_last_destroy_005cad70(Memory&, PlayerLastPorts&, Word self);                // RET 0 (does not free self)
void clear_action_queue_005ca0d0(Memory&, Word self);                                   // RET 0
void clear_known_spells_005cab20(Memory&, PlayerLastPorts&, Word self);                 // RET 0
void clear_known_spell_uses_005ca060(Memory&, Word self);                               // RET 0
void clear_spell_add_delete_005cac30(Memory&, PlayerLastPorts&, Word self);             // RET 0
void clear_feats_005cab60(Memory&, PlayerLastPorts&, Word self);                        // RET 0
void clear_visibility_list_005ca780(Memory&, PlayerLastPorts&, Word self);              // RET 0
void add_known_spell_005cabe0(Memory&, PlayerLastPorts&, Word self, Word cls, Word flag, Word spell);   // RET 0Ch
Word plast_known_spell_005ca740(Memory&, Word self, Word cls, Word flag, Word index);    // RET 0Ch
// ---- small unowned helpers
Word action_node_construct_004eadd0(Memory&, Word node);                                // RET 0 -> node
Word stats_known_spell_005a6750(Memory&, Word stats, Word cls, Word flag, Word index);  // RET 0Ch
Word stats_number_known_spells_005a67a0(Memory&, Word stats, Word cls, Word flag);      // RET 8, AX
void spell_like_allocate_0056b710(Memory&, PlayerLastPorts&, Word list, Word count);    // RET 4
// ---- CSWSMessage helpers (this = message, unused)
Word gui_known_spells_00570930(Memory&, PlayerLastPorts&, Word self, Word creature, Word plast);         // RET 8
Word gui_known_spell_uses_00567490(Memory&, PlayerLastPorts&, Word self, Word creature, Word plast);     // RET 8
Word action_queue_changed_00567590(Memory&, PlayerLastPorts&, Word self, Word creature, Word plast);     // RET 8
Word visibility_changed_0056b880(Memory&, Word self, Word creature, Word plast);                         // RET 8
Word gui_feats_0056c310(Memory&, PlayerLastPorts&, Word self, Word creature, Word plast);                // RET 8
Word compare_spell_like_0056c430(Memory&, Word self, Word creature, Word plast);                         // RET 8
void update_action_queue_00567520(Memory&, Word self, Word creature, Word plast);                        // RET 8
void update_visibility_list_005707a0(Memory&, PlayerLastPorts&, Word self, Word creature, Word plast);   // RET 8
void store_spell_like_00570ba0(Memory&, PlayerLastPorts&, Word self, Word creature, Word plast);         // RET 8
// 00572A20 TestPlayerUpdateDifferences(player, plast, last, flags*) RET 10h: ORs bits into *flags. scratch >= 0x10.
void test_player_differences_00572a20(Memory&, w2::entry::Ports&, PlayerLastPorts&, Word self, Word player, Word plast, Word last, Word flags_out, Word scratch);
// 00574400 StoreValuesInLastPlayerUpdateObject(player, plast, last, flags) RET 10h. scratch >= 0x20.
void store_player_values_00574400(Memory&, w2::entry::Ports&, PlayerLastPorts&, Word self, Word player, Word plast, Word last, Word flags, Word scratch);
}
