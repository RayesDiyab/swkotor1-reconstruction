#pragma once
// W2 tier 4 (G07): CSWSMessage::WriteGameObjUpdate_MajorGUIPanels 00577670 and
// WriteGameObjUpdate_PartyAIState 005779F0, plus their small unowned callees:
// CSWSPlayer::CreateNewPlayerLastUpdateObject 00561100, ComputeInventoryUpdateRequired 00567240,
// UpdateLastUpdateInventory 005683D0, CSWSFaction::GetMemberList 005BE780, CServerExoApp::GetServerInfo 004AEC90.
// Message bytes via the existing W2 writers with w2::entry::Ports.
#include "72239a78a7-w2_entry.hpp"
namespace w2::g07 {
using w2::entry::Word;
using w2::entry::Memory;

struct PanelPorts {
    virtual ~PanelPorts() = default;
    [[noreturn]] virtual void trap(Word address, const char* what) = 0;
    virtual Word server_info_004aec90(Word server) = 0;                // W1 tier6 @recovered_server_info_004aec90@8
    virtual Word allocate_006fa7e6(Word bytes) = 0;
    virtual void free_006fa390(Word p) = 0;
    virtual Word last_update_object_00561820(Word player, Word id) = 0;              // W2 t2
    virtual void player_last_destroy_005cad70(Word object) = 0;                       // ~CSWSPlayerLastUpdateObject, unowned
    virtual Word player_last_construct_005cac80(Word storage) = 0;                    // ctor, unowned
    virtual void clear_spell_uses_005ca060(Word object) = 0;                          // pending owner (endar-attachment)
    virtual void gui_known_spells_00570930(Word self, Word creature, Word plast) = 0; // unowned
    virtual void test_player_differences_00572a20(Word self, Word player, Word plast, Word last, Word flags_out) = 0;   // unowned
    virtual void player_update_00572dc0(Word self, Word player, Word plast, Word last, Word flags) = 0;                // unowned
    virtual void store_player_values_00574400(Word self, Word player, Word plast, Word last, Word flags) = 0;          // unowned
    virtual void repository_update_0056e360(Word self, Word player, Word object, Word repository, Word last_items, Word a, Word kind) = 0;  // unowned
    virtual Word object_lookup_004d8230(Word array, Word id, Word out) = 0;            // AL status
    virtual Word in_game_gui_005ed690(Word client) = 0;
    virtual Word party_access_panel_up_0062b4d0(Word gui, Word a) = 0;                // trap (ruling: stays a trap)
    virtual Word equip_screen_npc_0062fea0(Word gui) = 0;                             // client GUI
    virtual Word party_table_004aee70(Word server) = 0;
    virtual Word npc_object_00564700(Word table, Word index, Word create, Word apply) = 0;   // W2 t3
    virtual Word item_in_slot_005a4c20(Word inventory, Word slot) = 0;
    virtual void list_add_00671c00(Word list, Word value) = 0;
    virtual void item_appearance_00567090(Word self, Word item) = 0;                  // w2_g07_appearance
    virtual void active_item_properties_00566f20(Word self, Word item, Word creature) = 0;  // w2_g07_appearance
    virtual Word item_repository_004ef770(Word creature, Word flag) = 0;
    virtual Word faction_00513fc0(Word creature) = 0;
    virtual void test_party_differences_0056e850(Word self, Word player, Word creature, Word last_out, Word flags_out) = 0;  // unowned
    virtual void update_object_00574a10(Word self, Word player, Word object, Word last, Word flags, Word aflags) = 0;       // w2_g07_object
    virtual void store_party_values_00567cb0(Word self, Word creature, Word last, Word member, Word flags) = 0;             // unowned
    virtual Word list_at_pos_005e9b60(Word list, Word pos) = 0;
    virtual void list_next_005e9b90(Word list, Word pos_slot) = 0;
    virtual void list_remove_005e9c10(Word list, Word pos) = 0;
};


// 005BE780 CSWSFaction::GetMemberList(out*) RET 4: *out = [faction], returns [faction+4]
Word member_list_005be780(Memory&, Word faction, Word out);
// 00561100 CSWSPlayer::CreateNewPlayerLastUpdateObject() RET -> [player+5C]
Word create_new_player_last_00561100(Memory&, PanelPorts&, Word player);
// 00567240 ComputeInventoryUpdateRequired(player, slot) RET 8
Word compute_inventory_update_00567240(Memory&, w2::entry::Ports&, PanelPorts&, Word self, Word player, Word slot);
// 005683D0 UpdateLastUpdateInventory(player, slot) RET 8
void update_last_inventory_005683d0(Memory&, w2::entry::Ports&, PanelPorts&, Word self, Word player, Word slot);
// 00577670 WriteGameObjUpdate_MajorGUIPanels(player) RET 4. scratch >= 0x20
void major_gui_panels_00577670(Memory&, w2::entry::Ports&, PanelPorts&, Word self, Word player, Word scratch);
// 005779F0 WriteGameObjUpdate_PartyAIState(player) RET 4 -> 1. scratch >= 0x14
Word party_ai_state_005779f0(Memory&, w2::entry::Ports&, PanelPorts&, Word self, Word player, Word scratch);
}
