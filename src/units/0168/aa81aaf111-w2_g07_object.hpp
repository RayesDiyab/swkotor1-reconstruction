#pragma once
// W2 tier 4 (G07 object updates): CSWSMessage::TestObjectVisible 0056DC60,
// DeleteLastUpdateObjectsForObject 0056E190, WriteGameObjUpdate_UpdateObject 00574A10 ('U' record),
// and the small unowned callees they share with the other t4 files: GetIsBeam 004CA9D0/004CA960,
// CExoArrayList<LARGE_INTEGER>::AddUnique 004E3B70, GetDamageLevel 004CB020, GetClassLevel 005A5090,
// GetTotalDamage 004D2180, GetSingletonParty 005BF690, GetLeader 005BF5C0, GetIsNPCFaction 0052B280.
// Message bytes go through the existing W2 writers with w2::entry::Ports.
#include "72239a78a7-w2_entry.hpp"
namespace w2::g07 {
using w2::entry::Word;
using w2::entry::Memory;

// 0052B280 CFactionManager::GetIsNPCFaction(id) RET 4: W1 tier6 @recovered_faction_is_npc_0052b280@12 (t4b ruling).
struct NpcFactionPort {
    virtual ~NpcFactionPort() = default;
    virtual Word is_npc_faction_0052b280(Word manager, Word id) = 0;
};
struct ObjectPorts : NpcFactionPort {
    virtual ~ObjectPorts() = default;
    [[noreturn]] virtual void trap(Word address, const char* what) = 0;
    virtual Word server_module_004ae6b0(Word server) = 0;
    virtual Word client_object_by_id_004ae910(Word server, Word id) = 0;
    virtual Word spot_bits_004efc70(Word creature) = 0;                  // ST0 -> m32
    virtual Word listen_bits_004efd20(Word creature) = 0;
    virtual Word invisible_00501950(Word creature, Word observer) = 0;
    virtual Word visible_element_00517a20(Word creature, Word id) = 0;
    virtual Word reputation_0057cb80(Word object, Word id, Word flag) = 0;
    virtual Word ai_state_reputation_0057cd90(Word creature, Word id) = 0;
    virtual Word array_count_004af240(Word list, Word value) = 0;
    virtual Word object_lookup_004d8230(Word array, Word id, Word out) = 0;
    virtual Word list_at_pos_005e9b60(Word list, Word pos) = 0;
    virtual void list_next_005e9b90(Word list, Word pos_slot) = 0;
    virtual void list_remove_005e9c10(Word list, Word pos) = 0;
    virtual void last_update_destroy_005ca6a0(Word last) = 0;
    virtual Word allocate_006fa7e6(Word bytes) = 0;
    virtual void free_006fa390(Word p) = 0;
    virtual void list_allocate8_004c4f50(Word list, Word capacity) = 0;  // CExoArrayList<8-byte>::Allocate
    virtual Word animation_stationary_004cae60(Word object, Word animation) = 0;
    virtual Word walk_rate_bits_004f1b20(Word creature) = 0;             // ST0 -> m32
    virtual Word run_rate_bits_004f1be0(Word creature) = 0;
    virtual Word max_force_points_004fd490(Word creature) = 0;
    virtual Word faction_00513fc0(Word creature) = 0;
    virtual Word detect_mode_004edae0(Word creature) = 0;
    virtual Word get_class_005a4e90(Word stats, Word index) = 0;
    virtual Word combat_attack_004d2b50(Word round, Word index) = 0;
    virtual Word item_in_slot_005a4c20(Word inventory, Word slot) = 0;
    virtual void update_appearance_00572070(Word self, Word object, Word last, Word flags) = 0;
    // t4 sibling owners (w2_g07_last / w2_g07_compute), bound natively
    virtual Word create_new_last_00577000(Word self, Word player, Word object, Word flags_out, Word aflags_out) = 0;
    virtual Word compute_update_0056b950(Word self, Word player, Word object, Word last, Word same) = 0;
    virtual Word compute_appearance_00567100(Word self, Word object, Word last) = 0;
    virtual void update_last_00575c80(Word self, Word player, Word object, Word last, Word flags) = 0;
    virtual void update_last_appearance_00568480(Word object, Word last, Word aflags) = 0;
};

// helpers (unowned, W2 bodies)
Word is_beam_type_004ca960(Word type);
Word is_beam_004ca9d0(Memory&, Word effect);
void add_unique_large_004e3b70(Memory&, ObjectPorts&, Word list, Word lo, Word hi);
Word damage_level_004cb020(Memory&, w2::entry::Ports&, Word object);
Word class_level_005a5090(Memory&, Word stats, Word index, Word flag);
Word total_damage_004d2180(Memory&, Word attack);
Word singleton_party_005bf690(Memory&, w2::entry::Ports&, Word faction);
Word leader_005bf5c0(Memory&, w2::entry::Ports&, NpcFactionPort&, Word faction);

// 0056DC60 TestObjectVisible(object, observer) RET 8
Word test_object_visible_0056dc60(Memory&, w2::entry::Ports&, ObjectPorts&, Word self, Word object, Word observer);
// 0056E190 DeleteLastUpdateObjectsForObject(player, id) RET 8. scratch >= 8
void delete_for_object_0056e190(Memory&, w2::entry::Ports&, ObjectPorts&, Word self, Word player, Word id, Word scratch);
// 00577FA0 TestObjectUpdateDifferences(player, object, last*, flags*, aflags*) RET 14h. scratch >= 4
void test_update_differences_00577fa0(Memory&, w2::entry::Ports&, ObjectPorts&, Word self, Word player, Word object, Word last_out, Word flags_out, Word aflags_out, Word scratch);
// 00576FC0 StoreValuesInLastUpdateObject(player, last, object, flags, aflags) RET 14h
void store_values_00576fc0(ObjectPorts&, Word self, Word player, Word last, Word object, Word flags, Word aflags);
// 00574A10 WriteGameObjUpdate_UpdateObject(player, object, last, flags, appearance_flags) RET 14h. scratch >= 0x60
void update_object_00574a10(Memory&, w2::entry::Ports&, ObjectPorts&, Word self, Word player, Word object, Word last, Word flags, Word aflags, Word scratch);
}
