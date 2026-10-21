#pragma once
// W2 tier 2: CSWSPlayer::LoadCharacterFromIFO (00561E30) and the unowned
// callees on the primary-player path, plus UpdateSubareasOnJumpPosition
// (0051B940). 004BF470 InSubAreas is W5-owned (t2b).
// Logic over original 32-bit address words; scratch layouts are documented
// per function. Literal ids are the original .rdata addresses.
#include "1e16cfcf3d-server_mainloop.hpp"
#include <cstdint>
namespace w2::ifo {
using kotor::server_mainloop::Word;
using kotor::server_mainloop::Memory;

struct Ports {
    virtual ~Ports() = default;
    virtual Word literal(Word original_address) = 0;                    // original string literal -> live text
    virtual void write8(Word address, Word value) = 0;
    virtual Word app_007a39fc() = 0;
    virtual Word resman_007a39e8() = 0;
    virtual Word allocate_006fa7e6(Word bytes) = 0;
    virtual void free_006fa390(Word p) = 0;
    virtual void str_literal_005e5a90(Word str, Word text) = 0;
    virtual void str_destroy_005e5c20(Word str) = 0;
    virtual void str_empty_005b3190(Word str) = 0;
    virtual void str_assign_005e5c50(Word dst, Word src) = 0;
    virtual void directory_add_00408800(Word manager, Word str) = 0;
    virtual void directory_remove_004088d0(Word manager, Word str) = 0;
    virtual void ref_construct_00406d80(Word ref, Word text) = 0;
    virtual Word resource_lookup_004074d0(Word manager, Word ref, Word type) = 0;
    virtual Word gff_construct_004105a0(Word storage) = 0;
    virtual void resource_register_00407680(Word manager, Word ref, Word type, Word resource) = 0;
    virtual void strncpy_006fab80(Word dst, Word src, Word count) = 0;
    virtual Word demand_00409b10(Word resource) = 0;
    virtual Word release_00409b80(Word resource) = 0;
    virtual Word manager_release_00409cf0(Word manager, Word resource) = 0;
    virtual void top_00411240(Word resource, Word out) = 0;
    virtual Word list_004118c0(Word resource, Word out, Word parent, Word label) = 0;
    virtual Word list_count_00411940(Word resource, Word list) = 0;
    virtual Word element_00411990(Word resource, Word out, Word list, Word index) = 0;
    virtual Word dword_00411c20(Word resource, Word structure, Word label, Word status, Word fallback) = 0;
    virtual Word byte_00411a60(Word resource, Word structure, Word label, Word status, Word fallback) = 0;
    virtual Word int_00411c90(Word resource, Word structure, Word label, Word status, Word fallback) = 0;
    virtual Word word_00411b40(Word resource, Word structure, Word label, Word status, Word fallback) = 0;
    virtual Word float_bits_00411d00(Word resource, Word structure, Word label, Word status, Word fallback_bits) = 0; // FSTP m32 result
    virtual Word string_00411ec0(Word resource, Word result, Word structure, Word label, Word status, Word fallback) = 0;
    virtual Word creature_construct_004f7a10(Word storage, Word id, Word flag) = 0;
    virtual void set_game_object_00561c40(Word player, Word creature) = 0;
    virtual Word stats_read_005afce0(Word stats, Word resource, Word structure, Word out) = 0;
    virtual void items_read_004ffda0(Word creature, Word resource, Word structure, Word a, Word b, Word c) = 0;
    virtual void object_state_004d1cf0(Word object, Word resource, Word structure) = 0;
    virtual void spells_read_005aeb30(Word stats, Word resource, Word structure) = 0;
    virtual void scripts_read_004ebf20(Word creature, Word resource, Word structure) = 0;
    virtual void set_orientation_004cd170(Word object, Word vector) = 0;
    virtual void post_process_004f1c40(Word creature) = 0;
    virtual void vcall1(Word object, Word slot, Word a) = 0;             // thiscall [[object]+slot](object, a)
    virtual Word vcall0(Word object, Word slot) = 0;
    virtual void set_ai_level_004b08a0(Word ai, Word creature, Word level) = 0;
    // journal (only reached when the IFO carries JNL_Entries)
    virtual void journal_set_state_005c5a40(Word journal, Word plot, Word state, Word flag) = 0;
    virtual void journal_set_date_005c63f0(Word journal, Word plot, Word date, Word flag) = 0;
    virtual void journal_set_time_005c6650(Word journal, Word plot, Word time, Word flag) = 0;
    virtual Word journal_sort_order_cell_00833a90() = 0;
    virtual Word array_list_construct_005c5a30(Word self) = 0;              // 0773 owner (DeviceBuffer::Construct005c5a30)                // address of the global cell 00833A90
    // effects / dialog (owners elsewhere)
    virtual Word effect_integer_00503690(Word effect, Word index) = 0;
    virtual void remove_effect_by_id_004d06c0(Word object, Word id_lo, Word id_hi) = 0;
    virtual Word effect_construct_00503e40(Word storage, Word flag) = 0;
    virtual void effect_creator_00503a00(Word effect, Word id) = 0;
    virtual void effect_set_integer_005036a0(Word effect, Word index, Word value) = 0;
    virtual void apply_effect_004d14f0(Word object, Word effect, Word a, Word b) = 0;
    virtual Word stop_dialog_004cb4f0(Word object) = 0;
    // sub-areas
    virtual Word object_lookup_004d8230(Word array, Word id, Word out) = 0;
    virtual Word game_object_by_id_004ae750(Word server, Word id) = 0;
    virtual Word aoe_by_id_004ae7d0(Word server, Word id) = 0;
    virtual void aoe_jump_00596360(Word aoe, Word area, Word position) = 0;
    virtual Word in_subareas_004bf470(Word area, Word position, Word list) = 0;   // W5 owner
    virtual void list_add_00671c00(Word list, Word value) = 0;
    virtual void list_allocate_005e03c0(Word list, Word capacity) = 0;
    virtual Word subarea_changes_00516020(Word creature, Word list, Word a, Word b, Word flag) = 0;
    virtual void list_assign_0051b720(Word dst, Word src) = 0;
    virtual Word sticky_subarea_cell_007a1b54() = 0;                    // address of the global cell 007A1B54
};

inline Word server_of(Memory& m, Ports& p) { return m.read(p.app_007a39fc() + 8); }

// 00561E30 CSWSPlayer::LoadCharacterFromIFO(index, flag, post) RET 0Ch.
// scratch: original post-prologue ESP offsets, >= 0x88 bytes; arguments are written by the caller at +7C/+80/+84.
Word load_character_00561e30(Memory&, Ports&, Word player, Word scratch);
// 004FDB20 CSWSCreature::SetInParty(in_party, update) RET 8
void set_in_party_004fdb20(Memory&, Ports&, Word creature, Word in_party, Word update);
// 004F79E0 CSWSCreaturePartyFollowInfo ctor / 004F0020 Clear
Word follow_info_construct_004f79e0(Memory&, Word self);
void follow_info_clear_004f0020(Memory&, Word self);
// 004F17D0 CSWSCreature::LoadJournal(resource, structure) RET 8. scratch >= 0x40
void load_journal_004f17d0(Memory&, Ports&, Word creature, Word resource, Word structure, Word scratch);
// 005633C0 CSWPartyTable::GetPartyJournal()
Word party_journal_005633c0(Memory&, Ports&, Word party);
// 004F00A0 CSWSCreature::SetActivity(mask, on, extra) RET 0Ch
void set_activity_004f00a0(Memory&, Ports&, Word creature, Word mask, Word on, Word extra);
// 0050EE30 SetDetectMode(byte) RET 4 / 0050EE50 SetStealthMode(byte, value) RET 8
void set_detect_mode_0050ee30(Memory&, Ports&, Word creature, Word mode);
void set_stealth_mode_0050ee50(Memory&, Ports&, Word creature, Word mode, Word value);
// 004F6D70 CSWSCreature::ComputeModifiedMovementRate()
void movement_rate_004f6d70(Memory&, Ports&, Word creature);
// 0051B940 CSWSCreature::UpdateSubareasOnJumpPosition(Vector*, area id, flag) RET 0Ch. scratch >= 0x20
Word update_subareas_0051b940(Memory&, Ports&, Word creature, Word position, Word area_id, Word flag, Word scratch);
}
