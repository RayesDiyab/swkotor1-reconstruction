#include "4085ff001c-unequip.hpp"
#include "1a847f7684-w1_x87.hpp"
// Reconstructed from swkotor.exe (GOG, sha256 9c10e045...). Comments cite original PCs.
// Tier 7: 005023A0 CSWSCreature::RunUnequip and the server leaves it reaches. Item pointers are CSWSItem*: the CSWSObject
// base sits at +0x10, so the item's object id is at +0x14; +0x268 possessor id, +0x26C repository (container item).
namespace w1::unequip {
namespace {
constexpr U kDropHeight = 0x3e4ccccdu;   // [0073E46C] 0.2f
constexpr U kSlotMainHand = 0x10, kSlotOffHand = 0x20;
}
U inventory_slot_from_item_005a4980(U inv, U item, Ports& p) {
  U id = item ? p.read(item + 0x14) : kInvalidObject;               // 005A4986..005A498F
  for (U i = 0; i < 0x12; ++i) if (p.read(inv + 4 + i * 4) == id) return 1u << i;
  return 0;
}
U inventory_remove_item_005a4ba0(U inv, U item, Ports& p) {
  U id = item ? p.read(item + 0x14) : kInvalidObject;
  for (U i = 0; i < 0x12; ++i)
    if (p.read(inv + 4 + i * 4) == id) { p.write(inv + 4 + i * 4, kInvalidObject); return 1; }
  return 0;
}
void set_last_instant_equip_004efe00(U c, U slot, U value, Ports& p) {
  if (slot == kSlotMainHand) p.write(c + 0xaac, value);             // 004EFE1B
  else if (slot == kSlotOffHand) p.write(c + 0xaa8, value);         // 004EFE0E
}
U can_unequip_weapon_00513e30(U c, U item, Ports& p) {
  U inv = p.read(c + 0xa2c);
  U main = p.call(0x5a4c20, inv, {kSlotMainHand});                  // GetItemInSlot
  U off = p.call(0x5a4c20, inv, {kSlotOffHand});
  if (!main || p.read(main + 0x14) != p.read(item + 0x14) || !off) return 1;
  U base = p.call(0x5b31a0, p.read(p.global(kRules) + 0x34), {p.read(off + 0xc)});   // CSWBaseItemArray::GetBaseItem
  if (!p.read8(base + 9)) return 1;                                 // 00513E75..00513E7A
  if (U client = p.call(0x4ae910, server(p), {p.read(c + 4)})) {    // GetClientObjectByObjectId
    p.write(p.read(p.read(client + 0x54)) + 0x10, kInvalidObject);
    p.write(p.read(p.read(client + 0x54)) + 0x14, kInvalidObject);
  }
  return 2;
}
void item_remove_properties_00553c30(U item, U creature, U slot, Ports& p) {
  if (U ai = ai_master(p)) {                                        // 00553C3C GetServerAIMaster
    U n = p.read(item + 0x24c);
    for (U i = 0; slt(i, n); ++i, n = p.read(item + 0x24c)) {
      U prop = slt(i, n) ? p.read(item + 0x254) + i * 0x1c : 0;     // 00553C57..00553C69 (bounds-checked element)
      U bit = p.read8(prop + 0x11);
      if (bit == 0xff || (p.read(item + 0x294) & (1u << (bit & 31))))
        p.vcall(p.read(ai + 0x5c), 0xc, {item, prop, creature, slot});   // 004AF120 OnItemPropertyRemoved (jmp [[+5C]]+0C)
    }
  }
  p.call(0x4d0820, creature, {p.read(item + 0x14)});                 // CSWSObject::RemoveEffectByCreator(item id)
}
U unequip_item_004faa70(U c, U item, Ports& p) {
  U slot = inventory_slot_from_item_005a4980(p.read(c + 0xa2c), item, p);
  item_remove_properties_00553c30(item, c, slot, p);
  inventory_remove_item_005a4ba0(p.read(c + 0xa2c), item, p);
  p.call(0x4ece80, c, {item, 1, 0});                                // CSWSCreature::ComputeArmourClass
  U weight = p.read(c + 0xa3c);
  p.write(c + 0x344, 1);
  p.write(c + 0xa3c, weight - p.call(0x5562a0, item, {}));          // CSWSItem::GetWeight
  if (p.call(0x5b4790, item, {}) && p.read8(p.call(0x5b4790, item, {}) + 0xac) == 0x2c && p.read8(c + 0x4d1) == 1)
    p.call(0x4f2a50, c, {1});                                       // CSWSCreature::ToggleMode
  return 1;
}
void item_add_to_area_00555720(U item, U area, U x, U y, U z, U flag, U v, Ports& p) {
  // v: the 12-byte local position (x, y, z) at [esp+4] after `push edi`
  p.write(v, x); p.write(v + 4, y); p.write(v + 8, z);
  p.call(0x5538a0, item, {});                                       // CSWSItem::RemoveFromArea
  if (!area) return;
  U o = item + 0x10;
  p.call(0x4cb160, o, {area});                                     // CSWSObject::SetArea (1059, via w1x)
  p.call(0x4cd1a0, o, {v, 0, 1, 0});                                // CSWSObject::SetPosition
  p.call(0x50dfd0, p.call(0x4cb120, o, {}), {p.read(item + 0x14), flag});   // CSWSArea::AddObjectToArea
}
U run_unequip_005023a0(U c, U item_id, U repository_id, U combine, U out, U value, U e, Ports& p) {
  // e: +0x10 item (its address is passed to AddItem), +0x20 the AddToArea position
  U srv = server(p);
  U client = p.call(0x4ae910, srv, {p.read(c + 4)});
  U msg = p.call(0x4aedb0, srv, {});                                // CServerExoApp::GetSWSMessage
  if (out) p.write(out, 0);
  auto cancel = [&](U m) {                                          // 005026A5..005026B8
    if (client && m) p.call(0x56fd80, m, {p.read(client + 4), item_id});   // SendServerToPlayerInventory_UnequipCancel
    return 0u;
  };
  U item = p.call(0x4ae760, srv, {item_id});                        // GetItemByGameObjectID
  p.write(e + 0x10, item);
  if (!item || p.read(item + 0x268) != p.read(c + 4)) return cancel(msg);
  if (p.call(0x5a4950, p.read(c + 0xa2c), {item}) != 1) return cancel(msg);   // CSWInventory::GetItemInInventory
  U repo = 0;
  if (repository_id == kInvalidObject) {
    repo = p.call(0x4ef770, c, {1});                                // CSWSCreature::GetItemRepository
  } else {
    U container = p.call(0x4ae760, srv, {repository_id});
    if (!container || p.read(container + 0x268) != p.read(c + 4)) return cancel(msg);
    repo = p.read(container + 0x26c);
  }
  if (!repo) return cancel(msg);
  U r = p.call(0x513e30, c, {item}) & 0xff;                         // CanUnEquipWeapon (AL)
  if (r != 1 && r != 2) return cancel(msg);
  set_last_instant_equip_004efe00(c, inventory_slot_from_item_005a4980(p.read(c + 0xa2c), item, p), value, p);
  p.call(0x4faa70, c, {item});                                      // CSWSCreature::UnequipItem
  if (!p.call(0x55d330, repo, {e + 0x10, combine, 0, 0})) {         // CItemRepository::AddItem; full -> drop at the feet
    U z = x87::add_float(p.read(c + 0x98), kDropHeight);
    p.call(0x555720, p.read(e + 0x10), {p.call(0x4cb120, c, {}), p.read(c + 0x90), p.read(c + 0x94), z, 1});
  }
  U off = 0;
  if (r == 2) {                                                     // 005025FE..00502624: off-hand moves to the main hand
    off = p.call(0x5a4c20, p.read(c + 0xa2c), {kSlotOffHand});
    if (off) { p.call(0x4faa70, c, {off}); p.call(0x4feb90, c, {kSlotMainHand, off, 1, 0, 0}); }   // EquipItem
  }
  if (out) p.write(out, p.read(e + 0x10));
  p.call(0x553210, p.read(e + 0x10), {repository_id == kInvalidObject ? p.read(c + 4) : repository_id, 0, 0, kInvalidObject});   // SetPossessor
  if (!client) return 0;                                            // 00502549 / 00502658
  if (!msg) return 0;                                               // 0050269D -> 005026A5 with edi = 0
  if (r == 1) {
    p.call(0x56fd10, msg, {p.read(client + 4), item_id, value});    // SendServerToPlayerInventory_Unequip
  } else {
    if (!off) p.call(0x502664, 0, {});                              // ORIGINAL BUG: no off-hand item -> [0+0x14] read faults
    p.call(0x56fd10, msg, {p.read(client + 4), p.read(off + 0x14), value});   // 00502664: the off-hand item (read unchecked)
    p.call(0x56fd10, msg, {p.read(client + 4), p.read(p.read(e + 0x10) + 0x14), value});
  }
  return 1;
}
}
