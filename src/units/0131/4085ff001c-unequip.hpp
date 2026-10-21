#pragma once
#include "93d73ae698-w1_ports.hpp"
// Tier 7: 005023A0 CSWSCreature::RunUnequip (W2's 00510EF0 RemoveItem reaches it for equipped items) and its leaves.
namespace w1::unequip {
// RunUnequip(item id, repository object id, combine, CSWSItem** out, value), thiscall ret 0x14. Frame 0x14 (+0x10 item).
U run_unequip_005023a0(U creature, U item_id, U repository_id, U combine, U out, U value, U frame, Ports& p);
U can_unequip_weapon_00513e30(U creature, U item, Ports& p);              // ret 4, AL: 1 plain, 2 off-hand moves over
U inventory_slot_from_item_005a4980(U inventory, U item, Ports& p);        // ret 4 (one argument)
U inventory_remove_item_005a4ba0(U inventory, U item, Ports& p);           // ret 4
void set_last_instant_equip_004efe00(U creature, U slot, U value, Ports& p);   // ret 8
U unequip_item_004faa70(U creature, U item, Ports& p);                     // ret 4, returns 1
void item_remove_properties_00553c30(U item, U creature, U slot, Ports& p);   // ret 8
// CSWSItem::AddToArea(area, x, y, z, flag), ret 0x14. `v` = 12 bytes of scratch for the local position.
void item_add_to_area_00555720(U item, U area, U x, U y, U z, U flag, U v, Ports& p);
}
