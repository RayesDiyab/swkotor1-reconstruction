#pragma once
// W2 tier 6: CSWSCreature::SaveCreature 00500610 (thiscall (CResGFF* gff, CResStruct* st) RET 8, EAX = 1).
// Field order, labels and values are transcribed from the original (see src comments for addresses).
// Callees are ports: GFF writers (0873 exports + fork-A primitives), SaveStats / SaveItem / SaveCombatRound /
// SaveListenData / PartyFollowInfo::Save / SaveObjectState (W2 t6 siblings), SetInParty (W2 t2).
#include "1e16cfcf3d-server_mainloop.hpp"
namespace w2::save {
using kotor::server_mainloop::Word;
using kotor::server_mainloop::Memory;

struct CreaturePorts {
    virtual ~CreaturePorts() = default;
    [[noreturn]] virtual void trap(Word address, const char* what) = 0;
    virtual void set_in_party_004fdb20(Word creature, Word in_party, Word update) = 0;
    virtual void save_stats_005b1b90(Word stats, Word gff, Word st) = 0;
    virtual void write_byte_00412620(Word gff, Word st, Word value, const char* label) = 0;
    virtual void write_word_004126c0(Word gff, Word st, Word value, const char* label) = 0;
    virtual void write_dword_00412760(Word gff, Word st, Word value, const char* label) = 0;
    virtual void write_int_004127b0(Word gff, Word st, Word value, const char* label) = 0;
    virtual void write_float_00412870(Word gff, Word st, Word bits, const char* label) = 0;
    virtual void write_resref_004128c0(Word gff, Word st, Word ref, const char* label) = 0;
    virtual void ref_from_string_00406d60(Word ref, Word str) = 0;                   // CResRef(CExoString const&)
    virtual void add_list_00412450(Word gff, Word list_out, Word parent, const char* label) = 0;
    virtual void add_list_element_004124e0(Word gff, Word st_out, Word list, Word type) = 0;
    virtual void add_struct_to_struct_004125b0(Word gff, Word st_out, Word parent, const char* label, Word type) = 0;
    virtual Word item_in_slot_005a4c20(Word inventory, Word slot_mask) = 0;
    virtual Word item_repository_004ef770(Word creature, Word flag) = 0;
    virtual Word repository_item_005560b0(Word repository, Word index) = 0;
    virtual void save_item_0055ccd0(Word item, Word gff, Word st) = 0;
    virtual void save_combat_round_004d3ec0(Word round, Word gff, Word st) = 0;
    virtual void save_listen_data_004cca50(Word object, Word gff, Word st) = 0;
    virtual void follow_info_save_004eaf70(Word info, Word gff, Word st) = 0;
    virtual void save_object_state_004cec50(Word object, Word gff, Word st) = 0;
};

// s: >= 0x50 bytes scratch for the original's stack locals (ResRef, lists, structs).
Word save_creature_00500610(Memory&, CreaturePorts&, Word creature, Word gff, Word st, Word s);
}
