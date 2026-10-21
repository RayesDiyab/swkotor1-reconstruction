#pragma once
// W2 tier 5: party-table NPC availability / AddNPC, and the item-removal / store-insert leaves W6 imports.
//   005636B0 CSWPartyTable::GetIsNPCAvailable(npc) RET 4
//   005637C0 CSWPartyTable::GetNPCSelectability(npc) RET 4
//   005645F0 CSWPartyTable::AddNPC(npc, CExoString* template) RET 8
//   00564300 CSWPartyTable::AddNPC(npc, CSWSCreature*) RET 8
//   00554170 CSWSItem::SetIdentified(flag) RET 4
//   00510EF0 CSWSCreature::RemoveItem(item, a2, a3, CSWSItem** out) RET 10h
//   00584AC0 CSWSPlaceable::RemoveItem(item) RET 4
//   005C70C0 CSWSStore::AddItemToInventory(CSWSItem** cell) RET 4
//   0055D2C0 CItemRepository::InsertItem(item, position) RET 8
// Existing W2 bodies reused (w2_party.cpp): 005641E0 TransferInventory, 00563620 GetFilename, 00555ED0 FindItemWithTag,
// 00555F40 GetItemInRepository, 00555FD0 RemoveItem. 00616940 CExoArrayList<unsigned long>::Insert is the existing
// 1122 Memory-model owner kotor::server_mainloop::effect_insert_00616940 (called directly).
#include "4865336a87-w2_party.hpp"
namespace w2::npc_items {
using kotor::server_mainloop::Word;
using kotor::server_mainloop::Memory;

struct Ports {
    virtual ~Ports() = default;
    [[noreturn]] virtual void trap(Word address, const char* what) = 0;
    // 00564300
    virtual Word stats_level_005a5fd0(Word stats, Word flag) = 0;                 // AL
    virtual Word creature_faction_00513fc0(Word creature) = 0;
    virtual void faction_add_member_005bfa70(Word faction, Word id, Word flag) = 0;
    virtual void stats_movement_rate_005a5680(Word stats, Word rate) = 0;
    virtual void str_default_005b3190(Word str) = 0;                               // CExoString::CExoString()
    virtual void str_format_game_in_progress_005e5680(Word out, Word str) = 0;     // Format(out, "GAMEINPROGRESS:%s", *str)
    virtual Word str_cstr_005e5670(Word str) = 0;
    virtual Word gff_construct_004105a0(Word storage) = 0;                          // CResGFF::CResGFF()
    virtual Word gff_create_00411260(Word gff, Word top_out, Word type_str, Word version_str) = 0;
    virtual Word save_creature_00500610(Word creature, Word gff, Word top) = 0;
    virtual Word gff_write_00413030(Word gff, Word name_str, Word restype) = 0;
    // 00510EF0 / 005C70C0
    virtual Word inventory_has_item_005a4950(Word inventory, Word item) = 0;       // W1 GetItemInInventory
    virtual void run_unequip_005023a0(Word creature, Word id, Word target, Word a2, Word out, Word flag) = 0;
    virtual Word game_object_004ae750(Word server, Word id) = 0;
    virtual Word item_cost_00554000(Word item) = 0;                                 // W2 t4 body (w2_g07_appearance)
    virtual void array_grow_005e03c0(Word list, Word capacity) = 0;                 // CExoArrayList::Allocate
};

Word npc_available_005636b0(Memory&, Word party, Word npc);
Word npc_selectable_005637c0(Memory&, Word party, Word npc);
// s: >= 0x60 bytes of scratch (stack locals of 00564300).
Word add_npc_00564300(Memory&, w2::party::Ports&, Ports&, Word party, Word npc, Word creature, Word s);
// s: >= 0xA0 bytes of scratch (CResRef at s, 00564300 scratch at s + 0x40).
Word add_npc_template_005645f0(Memory&, w2::party::Ports&, Ports&, Word party, Word npc, Word template_str, Word s);
void set_identified_00554170(Memory&, Word item, Word flag);
Word creature_remove_item_00510ef0(Memory&, w2::party::Ports&, Ports&, Word creature, Word item, Word a2, Word a3, Word out);
Word placeable_remove_item_00584ac0(Memory&, w2::party::Ports&, Word placeable, Word item);
void repository_insert_item_0055d2c0(Memory&, Ports&, Word repository, Word item, Word position);
Word store_add_item_005c70c0(Memory&, w2::party::Ports&, Ports&, Word store, Word cell);
}
