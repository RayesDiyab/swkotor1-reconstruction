#pragma once
#include "93d73ae698-w1_ports.hpp"
// G02: CSWSItem event handler and its inventory/repository leaves.
namespace w1::item_event {
// 0055EE10 NVThunkItemGameObject_EventHandler(code, caller, data, unused, unused): thiscall, ECX = the item's CGameObject
// subobject (item + 0x10), ret 0x14.
void handler_0055ee10(U self, U code, U caller, U data, Ports& p);
// 005A4950 CSWInventory::GetItemInInventory(item): thiscall ret 4; 1 when one of the 18 slots (+4..) holds item's id.
U inventory_has_005a4950(U inventory, U item, Ports& p);
// 00555F40 / 00555FD0 CItemRepository::GetItemInRepository / RemoveItem are W2-owned (imported through the native binding).
// 005C6BB0 CSWSStore::GetItemInInventory(item id): thiscall ret 4.
U store_has_005c6bb0(U store, U id, Ports& p);
// 005C6BA0 CSWSStore::RemoveItem(item): thunk to 00555FD0 on [store+0x230], ret 4.
U store_remove_005c6ba0(U store, U item, Ports& p);
}
