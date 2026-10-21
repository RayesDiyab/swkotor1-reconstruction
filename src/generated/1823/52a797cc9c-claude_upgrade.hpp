#pragma once
// Lane W4 GUI successor (worker "upgrade"): declarations shared by claude_upgrade_*.cpp — the item-upgrade workbench
// (ShowUpgradeScreen -> CSWGuiUpgradeSelection -> CSWGuiUpgradeItemSelect -> CSWGuiUpgrade) and the CSWSItem helpers
// it reaches. Every function declared under "defined in claude_upgrade_*.cpp" is reconstructed in one of those files;
// the ABI comment is the original (thiscall = __fastcall(self, unused EDX, stack args), callee cleanup).
//
// Layouts (from the original constructors / users):
//   CSWUpgradeItemEntry (0x3A4, table 00757108 = CSWGuiButton 0073E658 + 5 overrides + Initialize at +A0):
//     +16C text params (button), +1C4 item id, +1C8 owner creature id, +1CC equip slot, +1D0 bit0 equipped,
//     +1D4 upgrade-slot byte (FFh none), +1D8/+24C/+2C0 CSWGuiBorder (count hex normal / highlight / icon),
//     +334 CSWGuiText (stack count; text params +34C).
//   CSWGuiUpgradeSelection (0x1158, table 007571B0): +64 LBL_TITLE, +1A4 BTN_UPGRADEITEMS, +368 BTN_BACK,
//     +52C 4 x CSWGuiRadioButton (0x1C4: lightsaber, ranged, melee, armor), +C3C 4 x CSWGuiLabel (0x140),
//     +113C item id (ShowUpgradeScreen) / +1140 int[4] type available, +1150 CSWGuiUpgradeItemSelect*,
//     +1154 selected type button.
//   CSWGuiUpgradeItemSelect (0xC48, table 00757228): +64 LB_ITEMS, +344 LB_DESCRIPTION, +624 LBL_TITLE,
//     +764 description label, +8A4 BTN_UPGRADEITEM, +A68 BTN_BACK, +C2C upgradable type byte, +C30 equip flags,
//     +C34 other-hand item id, +C38 direct item (CSWSItem*), +C3C selected CSWUpgradeItemEntry*,
//     +C40 CSWGuiUpgrade*, +C44 CSWGuiUpgradeSelection*.
//   CSWGuiUpgrade (0x2FB8, table 00757298): see claude_upgrade_panel.cpp. +2F4C type byte, +2F54 working item,
//     +2F58 original copy, +2F5C/+2F68 CExoArrayList<CSWSItem*> removed / added upgrade items, +2F74 CSWSItem*[4]
//     per slot, +2F84 CExoString[4] slot tags, +2FA4 byte[4] upgrade row per slot, +2FA8 upgrade.2da C2DA*,
//     +2FAC upcrystals.2da C2DA*, +2FB0 selected slot control, +2FB4 owning CSWGuiUpgradeItemSelect*.
#include "6030a9ee65-claude_gui_char.hpp"

extern "C" {
// ======================= defined in claude_upgrade_*.cpp
// ---- CGuiInGame (claude_upgrade_sel.cpp)
void __fastcall kotor_fastcall_0062e760(void* gui, void*, w4::U32 item_id);                       // ShowUpgradeScreen(ulong) RET 4
void __fastcall kotor_fastcall_0062e870(void* gui, void*);                                        // HideUpgradeScreen() RET
// ---- CSWGuiRadioButton (claude_upgrade_sel.cpp; no callable provider existed)
void* __fastcall kotor_fastcall_006b66f0(void* self, void*) noexcept(false);                      // CSWGuiRadioButton() RET
// ---- CSWUpgradeItemEntry (claude_upgrade_sel.cpp)
void* __fastcall kotor_fastcall_006c3e00(void* self, void*) noexcept(false);                      // ctor RET
void __fastcall kotor_fastcall_006c5ad0(void* self, void*);                                       // dtor RET
void* __fastcall kotor_fastcall_006c5ab0(void* self, void*, w4::U32 flags);                       // deleting dtor RET 4
void __fastcall kotor_fastcall_006c2650(void* self, void*, const void* extent);                   // SetExtent RET 4
void __fastcall kotor_fastcall_006c27c0(void* self, void*, w4::U32 delta_bits);                   // Draw(float) RET 4
void __fastcall kotor_fastcall_006c2830(void* self, void*, void* text, void* border, void* highlight, w4::I32 width); // Initialize RET 10h
void __fastcall kotor_fastcall_006c3ea0(void* self, void*, w4::U32 item_id, w4::U32 equipped, w4::U32 owner_id, w4::U32 slot); // SetItem RET 10h
// ---- CSWGuiUpgradeSelection (claude_upgrade_sel.cpp)
void* __fastcall kotor_fastcall_006c78d0(void* self, void*, void* manager);                       // ctor RET 4
void __fastcall kotor_fastcall_006c4450(void* self, void*);                                       // dtor RET
void* __fastcall kotor_fastcall_006c5b50(void* self, void*, w4::U32 flags);                       // deleting dtor RET 4
void __fastcall kotor_fastcall_006c2ab0(void* self, void*, void* creature, w4::I32* available, void* table); // GetUpgradableItems RET 0Ch
void __fastcall kotor_fastcall_006c2b10(void* self, void*, w4::U32 event, w4::U32 down);          // HandleInputEvent RET 8
void __fastcall kotor_fastcall_006c2b60(void* self, void*, void* control);                        // OnTypeSelected RET 4
void __fastcall kotor_fastcall_006c2bd0(void* self, void*, void* control);                        // OnEnter RET 4
void __fastcall kotor_fastcall_006c4520(void* self, void*);                                       // OnPanelAdded RET
// ---- CSWGuiUpgradeItemSelect (claude_upgrade_itemsel.cpp)
void* __fastcall kotor_fastcall_006c7630(void* self, void*, void* manager);                       // ctor RET 4
void __fastcall kotor_fastcall_006c47a0(void* self, void*);                                       // dtor RET
void* __fastcall kotor_fastcall_006c5b70(void* self, void*, w4::U32 flags);                       // deleting dtor RET 4
w4::U32 __fastcall kotor_fastcall_006c2a70(void* self, void*);                                    // selected entry owner id RET
w4::U32 __fastcall kotor_fastcall_006c2a90(void* self, void*);                                    // selected entry slot RET
void __fastcall kotor_fastcall_006c2c80(void* self, void*, void* text);                           // SetDescription RET 4
void __fastcall kotor_fastcall_006c2d30(void* self, void*, w4::U32 event, w4::U32 down);          // HandleInputEvent RET 8
void __fastcall kotor_fastcall_006c2df0(void* self, void*, void* control);                        // OnUpgradableSelected RET 4
void __fastcall kotor_fastcall_006c4880(void* self, void*, void* control);                        // OnControlEntered RET 4
void __fastcall kotor_fastcall_006c4960(void* self, void*, void* creature, void* list, void* table); // BuildItemList RET 0Ch
void __fastcall kotor_fastcall_006c5b90(void* self, void*);                                       // BuildUpgradeList RET
void __fastcall kotor_fastcall_006c5e90(void* self, void*);                                       // FinishUpgrading RET
void __fastcall kotor_fastcall_006c6330(void* self, void*);                                       // OnPanelAdded RET
// ---- CSWGuiUpgrade (claude_upgrade_panel.cpp / claude_upgrade_panel2.cpp)
void* __fastcall kotor_fastcall_006c6b60(void* self, void*, void* manager);                       // ctor RET 4
void __fastcall kotor_fastcall_006c4ad0(void* self, void*);                                       // dtor RET
void* __fastcall kotor_fastcall_006c6100(void* self, void*, w4::U32 flags);                       // deleting dtor RET 4
void __fastcall kotor_fastcall_006c25f0(void* self, void*);                                       // OnPanelRemoved RET
void __fastcall kotor_fastcall_006c2f80(void* self, void*, w4::U32 show);                         // ShowItems(int) RET 4
void __fastcall kotor_fastcall_006c32e0(void* self, void*, void* text);                           // SetDescription RET 4
void __fastcall kotor_fastcall_006c33a0(void* self, void*, w4::U32 delta_bits);                   // Draw(float) RET 4
void __fastcall kotor_fastcall_006c3460(void* self, void*, w4::U32 row, w4::U32 model_variation, w4::U32 texture_variation,
                                        void* model_out, void* texture_out);                      // GetArmorModelAndTextureResRef RET 14h
void __fastcall kotor_fastcall_006c3630(void* self, void*);                                       // Update3DView RET
void __fastcall kotor_fastcall_006c3aa0(void* self, void*);                                       // UpdateItems RET
void __fastcall kotor_fastcall_006c3c30(void* self, void*, void* control);                        // OnEnterSlot RET 4
void __fastcall kotor_fastcall_006c4d30(void* self, void*);                                       // CloseItems RET
void __fastcall kotor_fastcall_006c4d70(void* self, void*);                                       // OnPanelAdded RET
void __fastcall kotor_fastcall_006c5370(void* self, void*, void* control);                        // OnControlEntered RET 4
void __fastcall kotor_fastcall_006c5510(void* self, void*, void* control);                        // OnUpgradeSelected RET 4
void* __fastcall kotor_fastcall_006c58c0(void* self, void*, w4::U32 item_id, w4::U32 slot);       // CreateItemEntry RET 8
void __fastcall kotor_fastcall_006c59a0(void* self, void*, void* item, w4::U32 slot);             // InsertUpgrade RET 8
void __fastcall kotor_fastcall_006c6120(void* self, void*, void* item);                           // VerifyUpgrade (message box callback) RET 4
void __fastcall kotor_fastcall_006c6190(void* self, void*, void* control);                        // OnAssemble RET 4
void __fastcall kotor_fastcall_006c61f0(void* self, void*);                                       // CancelPanel RET
void __fastcall kotor_fastcall_006c6500(void* self, void*, void* control);                        // OnSlotSelected RET 4
void __fastcall kotor_fastcall_006c6a80(void* self, void*, w4::U32 event, w4::U32 down);          // HandleInputEvent RET 8
// ---- CSWSItem (claude_upgrade_items.cpp)
w4::U8 __fastcall kotor_fastcall_005541c0(void* item, void*, void* table);                        // GetUpgradableType(C2DA*) RET 4
void* __fastcall kotor_fastcall_0055f510(void* item, void*, void* out, w4::U32 key);              // GetKeyedPropertyString(byte) RET 8
}

namespace upgrade {
// .rdata 00756FB0: per (upgradable type 1..4, slot 0..3) entry, indexed [(type * 4 + slot - 4)]: the upgrade.2da
// "UpgradeType" value of the slot (-1 none), the empty-slot icon resref text, the slot-name strref. The original reads
// [00756FB0 + 12 * index] (+0 type, +4 icon, +8 strref); the values below are the original .rdata words.
struct SlotInfo { std::int32_t upgrade_type; const char* icon; std::uint32_t strref; };
extern const SlotInfo k_slots_00756fb0[16];
// Original index expression (type * 4 + slot - 4) evaluated exactly as the original lea chains do (no bound check).
inline const SlotInfo& slot_info(w4::U32 type, w4::U32 slot) { return k_slots_00756fb0[type * 4u + slot - 4u]; }
}  // namespace upgrade
