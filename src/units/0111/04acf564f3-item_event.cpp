#include "844cc6557a-item_event.hpp"
// Reconstructed from swkotor.exe (GOG, sha256 9c10e045...). Comments cite original PCs.
// G02: 0055EE10 CSWSItem event handler (CGameObject-subobject thunk) and its inventory/repository leaves.
namespace w1::item_event {
namespace {
constexpr U kTimedEvent = 1, kDestroyObject = 11, kItemCharge = 16;   // [007441B8] [007441E0] [007441F4]
constexpr U kTypeCreature = 5, kTypeItem = 6, kTypePlaceable = 9, kTypeStore = 14;
}
U inventory_has_005a4950(U inventory, U item, Ports& p) {
  U id = item ? p.read(item + 0x14) : kInvalidObject;              // 005A4950..005A495D
  U slot = inventory + 4;
  for (U i = 0; slt(i, 0x12); ++i, slot += 4)                       // 005A4960..005A4970: 18 equipment slots
    if (p.read(slot) == id) return 1;
  return 0;
}
U store_has_005c6bb0(U store, U id, Ports& p) {
  U item = p.call(0x4ae760, server(p), {id});                       // 005C6BC0
  return item && p.call(0x555f40, p.read(store + 0x230), {item, 0}) ? 1u : 0u;   // CItemRepository::GetItemInRepository (W2)   // 005C6BC5..005C6BE4
}
U store_remove_005c6ba0(U store, U item, Ports& p) {
  return p.call(0x555fd0, p.read(store + 0x230), {item});            // CItemRepository::RemoveItem (W2)   // 005C6BA0 thunk
}
void handler_0055ee10(U self, U code, U, U data, Ports& p) {
  if (code == kTimedEvent) {                                        // 0055EE17..0055EE3E
    if (data) p.call(0x5d0fd0, p.global(kVirtualMachine), {data, p.read(self + 4), 1});   // RunScriptSituation
    return;
  }
  if (code == kItemCharge) {                                        // 0055EE41..0055EEE0
    U owner = p.call(0x4ae770, server(p), {p.read(self + 0x258)});
    U charges = p.read(self + 0x27c);
    if (sgt(charges, 1)) p.write(self + 0x27c, charges - 1);
    else p.call(0x4b08d0, ai_master(p), {0, 0, p.read(self + 4), p.read(self + 4), kDestroyObject, 0});   // AddEventDeltaTime
    if (!owner || !inventory_has_005a4950(p.read(owner + 0xa2c), self - 0x10, p)) return;
    U base = p.call(0x5b31a0, p.read(p.global(kRules) + 0x34), {p.read(self - 4)});
    p.write(owner + 0xa3c, p.read(owner + 0xa3c) - p.read8(base + 0x89));   // 0055EED1..0055EED8
    return;
  }
  if (code != kDestroyObject || !p.read(self + 0xec)) return;        // 0055EEE3..0055EEF7
  U item = self - 0x10;                                             // 0055EEFD
  p.call(0x55dc10, item, {});                                       // CloseItemForAllPlayers
  U holder = p.call(0x4ae750, server(p), {p.read(self + 0x258)});
  if (holder) {
    U kind = p.read8(holder + 8);
    auto release = [&] { p.call(0x553210, item, {kInvalidObject, 1, 1, kInvalidObject}); };   // SetPossessor
    if (kind == kTypeCreature) {                                    // 0055EF31..0055EF77
      U cr = p.vcall(holder, 0x30, {});
      p.call(0x510ef0, cr, {item, 0, 1, 0});                        // CSWSCreature::RemoveItem
      if (p.read(cr + 0xa88) && data == 1)
        p.call(0x62eeb0, p.call(0x5ed690, p.read(p.global(kAppManager) + 4), {}), {8, 0});   // CGuiInGame::UpdateStatus
    } else if (kind == kTypeItem) {                                 // 0055EF84..0055EFBF container item
      U container = p.vcall(holder, 0x38, {});
      if (p.call(0x555f40, p.read(container + 0x26c), {item, 0}) == 1) p.call(0x555fd0, p.read(container + 0x26c), {item});
      release();
    } else if (kind == kTypePlaceable) {                            // 0055EFC9..0055EFF2
      U pl = p.vcall(holder, 0x48, {});
      if (p.read(pl + 0x324) == 1 && p.call(0x555f40, p.read(pl + 0x36c), {item, 0}) == 1)
        p.call(0x555fd0, p.read(pl + 0x36c), {item});
      release();
    } else if (kind == kTypeStore) {                                // 0055EFFC..0055F031
      U store = p.vcall(holder, 0x64, {});
      if (store_has_005c6bb0(store, p.read(self + 4), p) == 1) store_remove_005c6ba0(store, item, p);
      release();
    }
  }
  p.call(0x5538a0, item, {});                                       // 0055F032 CSWSItem::RemoveFromArea
  if (item) p.vcall(item, 0x00, {1});                               // scalar deleting destructor
}
}
