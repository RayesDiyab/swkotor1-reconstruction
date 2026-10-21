#pragma once
// W2 tier 6 (fork B): creature-stats GFF save.
//   005B1B90 CSWSCreatureStats::SaveStats(CResGFF*, CResStruct*) RET 8
//   005AEC90 CSWSCreatureStats::SaveClassInfo(CResGFF*, CResStruct*) RET 8
//   005CB0F0 CSWLevelStats::GetSkillRankChange(WORD skill) RET 4 -> AL
//   00550F30 CCombatInformation::SaveData(CResGFF*, CResStruct*) RET 8 -> 1
// GFF handles live at the original stack offsets inside the caller-provided scratch (s + original esp offset);
// labels are passed as addresses of NUL-terminated text obtained from Ports::text().
#include "1e16cfcf3d-server_mainloop.hpp"
namespace w2::save_stats {
using kotor::server_mainloop::Word;
using kotor::server_mainloop::Memory;

struct Ports {
    virtual ~Ports() = default;
    [[noreturn]] virtual void trap(Word address, const char* what) = 0;
    virtual Word text(const char* label) = 0;                                  // address of a NUL-terminated label
    virtual Word app_007a39fc() = 0;
    virtual Word rules_007a3a28() = 0;
    // CResGFF writers (0873 / fork A). gff = CResGFF*, st = CResStruct*, label = text address.
    virtual void write_byte_00412620(Word gff, Word st, Word value, Word label) = 0;
    virtual void write_char_00412670(Word gff, Word st, Word value, Word label) = 0;
    virtual void write_word_004126c0(Word gff, Word st, Word value, Word label) = 0;
    virtual void write_short_00412710(Word gff, Word st, Word value, Word label) = 0;
    virtual void write_dword_00412760(Word gff, Word st, Word value, Word label) = 0;
    virtual void write_int_004127b0(Word gff, Word st, Word value, Word label) = 0;
    virtual void write_float_00412870(Word gff, Word st, Word bits, Word label) = 0;
    virtual void write_resref_004128c0(Word gff, Word st, Word ref, Word label) = 0;
    virtual void write_string_00412970(Word gff, Word st, Word str, Word label) = 0;
    virtual void write_locstring_00412a10(Word gff, Word st, Word loc, Word label) = 0;
    virtual void add_list_00412450(Word gff, Word out_list, Word parent, Word label) = 0;
    virtual void add_list_element_004124e0(Word gff, Word out_struct, Word list, Word type) = 0;
    virtual void add_struct_to_struct_004125b0(Word gff, Word out_struct, Word parent, Word label, Word type) = 0;
    // strings
    virtual void str_default_005b3190(Word str) = 0;
    virtual void str_format_s_d_005e5680(Word str, Word prefix_text, Word n) = 0;   // Format(str, "%s%d", prefix, n)
    virtual Word str_cstr_005e5670(Word str) = 0;
    virtual void str_destroy_005e5c20(Word str) = 0;
    virtual void locstring_get_005ea130(Word loc, Word language, Word out_str, Word gender) = 0;
    // creature / stats
    virtual Word gold_004edd60(Word creature) = 0;
    virtual Word reflex_save_005ab8f0(Word stats, Word flag) = 0;                 // AL
    virtual Word base_will_save_005aa2f0(Word stats) = 0;                         // AL
    virtual Word base_fort_save_005aa1b0(Word stats) = 0;                         // AL
    virtual Word effect_bonus_004f3fe0(Word creature, Word a1, Word a2, Word a3, Word a4, Word a5, Word a6, Word a7, Word a8, Word a9, Word a10) = 0;
    virtual Word armor_class_004ed1d0(Word creature) = 0;                        // AX
    virtual Word max_force_points_004fd490(Word creature) = 0;
    virtual Word vcall0(Word object, Word slot) = 0;                              // thiscall [[o]+slot](o)
    virtual Word vcall1(Word object, Word slot, Word a) = 0;                      // thiscall [[o]+slot](o, a)
    virtual Word stats_level_005a5fd0(Word stats, Word flag) = 0;                 // AL
    virtual Word is_jedi_005be4a0(Word class_id) = 0;                             // cdecl
    virtual Word party_table_004aee70(Word server) = 0;
    virtual Word item_by_id_004ae760(Word server, Word id) = 0;
    virtual Word base_item_005b31a0(Word array, Word type) = 0;
};

// s: >= 0x50 bytes (SaveStats locals), class-info scratch at s2 >= 0xF0 bytes.
void save_stats_005b1b90(Memory&, Ports&, Word stats, Word gff, Word parent, Word s, Word s2);
void save_class_info_005aec90(Memory&, Ports&, Word stats, Word gff, Word parent, Word s);
Word skill_rank_change_005cb0f0(Memory&, Ports&, Word level_stats, Word skill);
// s: >= 0x2C bytes; the CombatInfo struct handle lands at s + 0x28 (the original parent-argument slot).
Word combat_info_save_00550f30(Memory&, Ports&, Word info, Word gff, Word parent, Word s);
}
