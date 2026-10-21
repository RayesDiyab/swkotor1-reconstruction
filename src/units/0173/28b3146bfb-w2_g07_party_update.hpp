#pragma once
// W2 tier 4 (G07, fork I): CSWSMessage::TestPartyObjectUpdateDifferences 0056E850,
// StoreValuesInLastUpdatePartyObject 00567CB0, WriteRepositoryUpdate 0056E360,
// CGuiInGame::GetCurrentEquipScreenNPC 0062FEA0, and the small unowned callees
// CItemRepository::ItemListGetItemObjectID 005560E0 and the CSWSPlayerLUOInventory list helpers
// 005CA4D0 / 005CA520 / 005CA570 / 005CA5C0 / 005CA630.
// Message bytes via the existing W2 writers with w2::entry::Ports; GetSingletonParty / GetLeader reuse
// the w2_g07_object bodies.
#include "72239a78a7-w2_entry.hpp"
#include "aa81aaf111-w2_g07_object.hpp"   // NpcFactionPort (0052B280, W1)
namespace w2::g07 {
using w2::entry::Word;
using w2::entry::Memory;

struct PartyUpdatePorts : NpcFactionPort {
    virtual ~PartyUpdatePorts() = default;
    [[noreturn]] virtual void trap(Word address, const char* what) = 0;
    virtual Word allocate_006fa7e6(Word bytes) = 0;
    virtual void free_006fa390(Word p) = 0;
    virtual void compute_ai_state_004f1460(Word creature) = 0;              // W1 @recovered_creature_compute_ai_state_004f1460@8
    virtual Word ai_state_reputation_0057cd90(Word object, Word id) = 0;    // W1 (AL)
    virtual Word max_force_points_004fd490(Word creature) = 0;              // AX; @w1x_creature_max_fp_004fd490@8
    virtual Word faction_00513fc0(Word creature) = 0;                       // Memory-model creature_faction_00513fc0
    virtual void ref_default_00405ed0(Word ref) = 0;                        // CResRef::CResRef()
    virtual Word resref_ne_00406dc0(Word a, Word b) = 0;                    // CResRef::operator!=(this=a, b)
    virtual Word resref_assign_00406120(Word self, Word result, Word source) = 0;   // CResRef::operator=
    virtual Word list_at_pos_005e9b60(Word list, Word pos) = 0;
    virtual void list_next_005e9b90(Word list, Word pos_slot) = 0;
    virtual void list_prev_005e9bd0(Word list, Word pos_slot) = 0;
    virtual Word list_remove_005e9c10(Word list, Word pos) = 0;            // returns the node data
    virtual void list_add_head_005e9d70(Word list, Word data) = 0;
    virtual Word item_by_id_004ae760(Word server, Word id) = 0;
    virtual Word item_cost_00554000(Word item) = 0;                         // w2_g07_appearance
    virtual void item_appearance_00567090(Word self, Word item) = 0;        // w2_g07_appearance
    virtual void active_item_properties_00566f20(Word self, Word item, Word creature) = 0;   // w2_g07_appearance
    virtual Word panel_exists_0040bd70(Word manager, Word panel) = 0;      // @frame_ContainsPanel_0040bd70@12
};

// 005560E0 CItemRepository::ItemListGetItemObjectID(index) RET 4
Word item_list_object_id_005560e0(Memory&, Word repository, Word index);
// CSWSPlayerLUOInventory list helpers; `which` selects +98h (0), +90h (1), +9Ch (2); other values do nothing.
Word luo_update_item_005ca4d0(Memory&, PartyUpdatePorts&, Word luo, Word pos, Word which);        // RET 8
void luo_set_end_005ca520(Memory&, Word luo, Word pos_out, Word which);                            // RET 8
void luo_get_prev_005ca570(Memory&, PartyUpdatePorts&, Word luo, Word pos_slot, Word which);       // RET 8
void luo_remove_005ca5c0(Memory&, PartyUpdatePorts&, Word luo, Word pos, Word which);             // RET 8 (labelled AddHead)
void luo_add_head_005ca630(Memory&, PartyUpdatePorts&, Word luo, Word id, Word which, Word stack, Word cost);   // RET 10h
// 0062FEA0 CGuiInGame::GetCurrentEquipScreenNPC() RET -> sign-extended byte or -1
// 0056E850 TestPartyObjectUpdateDifferences(player, creature, last*, flags*) RET 10h. scratch >= 0x14
void test_party_differences_0056e850(Memory&, w2::entry::Ports&, PartyUpdatePorts&, Word self, Word player, Word creature, Word last_out, Word flags_out, Word scratch);
// 00567CB0 StoreValuesInLastUpdatePartyObject(pc, last, member, flags) RET 10h. scratch >= 0x20
void store_party_values_00567cb0(Memory&, w2::entry::Ports&, PartyUpdatePorts&, Word self, Word pc, Word last, Word member, Word flags, Word scratch);
// 0056E360 WriteRepositoryUpdate(player, object, repository, luo, which, letter) RET 18h. scratch >= 4
void repository_update_0056e360(Memory&, w2::entry::Ports&, PartyUpdatePorts&, Word self, Word player, Word object, Word repository, Word luo, Word which, Word letter, Word scratch);
}
