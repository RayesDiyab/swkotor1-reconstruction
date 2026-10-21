// Lane W4 GUI successor (worker "upgrade", fork A2): CSWGuiUpgradeItemSelect (0xC48, method table 00757228 = 27 slots,
// base CSWGuiPanel 0073E010, overrides +00 +3C +48). Transcribed from the original swkotor.exe linear sweep (read-only);
// every statement carries its original PCs.
//   006C7630 CSWGuiUpgradeItemSelect(CSWGuiManager*)    thiscall RET 4       006C47A0 ~CSWGuiUpgradeItemSelect RET
//   006C5B70 scalar deleting destructor                 thiscall RET 4
//   006C2A70 selected entry owner id (7F000000h none)   thiscall RET         006C2A90 selected entry slot (0 none) RET
//   006C2C80 SetDescription(const CExoString&)          thiscall RET 4       006C2D30 HandleInputEvent(int, int) RET 8
//   006C2DF0 OnUpgradableSelected(CSWGuiControl*)       thiscall RET 4       006C4880 OnControlEntered(CSWGuiControl*) RET 4
//   006C4960 BuildItemList(CSWSCreature*, CExoArrayList<CSWGuiControl*>*, C2DA*) thiscall RET 0Ch
//   006C5B90 BuildUpgradeList()                         thiscall RET         006C5E90 FinishUpgrading() RET
//   006C6330 OnPanelAdded()                             thiscall RET
// Original quirks kept (documented in place): failed entry / panel / C2DA allocations are used unchecked (null
// dereference, as the original); FinishUpgrading's search over LB_ITEMS stops only on a match (the loop condition
// re-tests the item count, not the index) and deletes the direct-mode entry without clearing +C3C; the GetPlayerList
// walk dereferences an empty list's null node.
#include "52a797cc9c-claude_upgrade.hpp"

extern "C" {
// ---- GUI
void* __fastcall kotor_m1_listbox_get_selected_control_0041a5d0(void* listbox, void*);            // CSWGuiListBox::GetSelectedControl
void __fastcall gui_m48_listbox_add_controls_0041c2c0(void* listbox, void*, void** controls, w4::U32 count, w4::U32, w4::U32, w4::U32); // AddControls
void __fastcall gui_load_listbox_set_active_control_0041c160(void* listbox, void*, void* control, w4::U32); // CSWGuiListBox::SetActiveControl
w4::U32 __fastcall gui_load_text_font_height_00414ee0(void* text, void*);                         // CSWGuiText::GetFontHeight
w4::I32 __fastcall async_string_length_005e5790(void* string, void*);                             // CExoString::GetLength
void __fastcall kotor_fastcall_00624a00(void* panel, void*, w4::U32 flag);                        // CSWGuiPanel::setToDestroy
// ---- C2DA
void* __fastcall c2da_current_construct_00413cc0(void* self, void*, menu_resources::Ref name, w4::I32 mode) noexcept(false); // C2DA::C2DA
w4::I32 __fastcall c2da_current_load_004143b0(void* table, void*);                                // C2DA::Load2DArray
// ---- server objects
void* __fastcall recovered_server_player_list_004aed90(void* server, void*);                      // CServerExoApp::GetPlayerList
void* __fastcall recovered_party_repository_00563340(void* table, void*);                         // CSWPartyTable::GetPartyItems
unsigned char __fastcall recovered_party_npc_selectable_005637c0(void* table, void*, w4::U32 npc); // GetNPCSelectability (AL)
void* __fastcall recovered_equipment_item_005a4c20(void* inventory, void*, w4::U32 slot);         // CSWInventory::GetItemInSlot
w4::U32 __fastcall recovered_inventory_slot_from_item_005a4980(void* inventory, void*, void* item); // CSWInventory::GetSlotFromItem
w4::U32 __fastcall recovered_creature_unequip_item_004faa70(void* creature, void*, void* item);   // CSWSCreature::UnequipItem
w4::U32 __fastcall recovered_creature_equip_004feb90(void* creature, void*, w4::U32 slot, void* item, w4::U32, w4::U32, w4::U32); // EquipItem
unsigned char __fastcall recovered_item_can_equip_0051aa60(void* creature, void*, void* item, w4::U32* slot, w4::U32, w4::U32, w4::U32); // CanEquipItem
w4::U32 __fastcall recovered_inventory_insert_0055d330(void* repository, void*, void** item, w4::U32, w4::U32, w4::U32); // CItemRepository::AddItem
w4::U32 __fastcall recovered_remove_item_00555fd0(void* repository, void*, void* item);           // CItemRepository::RemoveItem
void* __fastcall w2x_inventory_item_005560b0(void* repository, void*, w4::U32 index);             // CItemRepository::ItemListGetItem
void* __fastcall kotor_fastcall_0055f280(void* item, void*, w4::U32 count);                       // CSWSItem::SplitItem
void* __fastcall kotor_fastcall_0055f340(void* item, void*, void* out);                           // CSWSItem::GetPropertyDescription
}

using namespace gchar;

namespace {
constexpr U32 k_invalid = 0x7F000000u;                                                           // OBJECT_INVALID
inline void* listbox_items(void* self) { return at(self, 0x64); }                                 // LB_ITEMS
inline void* upgrade_panel(void* self) { return lp(self, 0xC40); }                                // CSWGuiUpgrade*
inline void* entry_of(void* self) { return lp(self, 0xC3C); }                                     // selected CSWUpgradeItemEntry*
inline void* party_items() { return recovered_party_repository_00563340(party_table(), nullptr); } // GetPartyTable()->GetPartyItems()
// [panel+44h]: clear 100h, then unless (flags & 600h) == 400h replace 400h by 200h (two stores, as the original).
inline void close_flags(void* panel) {
    const U32 f = lw(panel, 0x44) & 0xFFFFFEFFu;
    sw(panel, 0x44, f);
    if ((f & 0x600u) != 0x400u) sw(panel, 0x44, (f & 0xFFFFFBFFu) | 0x200u);
}
// Hand the item over to the CSWGuiUpgrade panel and show it (006C2F46..006C2F72 / 006C6433..006C6465).
inline void open_upgrade(void* self, void* item) {
    sb(upgrade_panel(self), 0x2F4C, lb(self, 0xC2C));                                             // type byte
    sw(upgrade_panel(self), 0x2F54, ad(item));                                                    // working item
    kotor_fastcall_0040bc70(manager_of(self), nullptr, upgrade_panel(self), 3, 1);                // AddPanel(panel, 3, 1)
}
// GetPlayerList(): [[list]] head node -> node+8 player (null node dereferenced as the original) -> vtable +4 -> +38h id.
inline U32 player_creature_id(void* server) {
    void* list = recovered_server_player_list_004aed90(server, nullptr);
    void* node = lp(lp(list, 0), 0);
    void* player = node != nullptr ? lp(node, 8) : nullptr;
    return lw(vcall<void*>(player, 0x04), 0x38);
}
// new CSWUpgradeItemEntry (0x3A4) + Initialize from the list box prototype (+154h text, +6Ch border, +E0h highlight).
void* new_entry(void* self, void* proto, I32 width) {
    void* raw = live_operator_new(0x3A4);
    void* entry = nullptr;
    if (raw != nullptr) {
        auto g = on_throw([&]() { live_free(raw); });                                             // EH state: free the block
        entry = kotor_fastcall_006c3e00(raw, nullptr);                                            // CSWUpgradeItemEntry()
        g.armed = false;
    }
    reinterpret_cast<void(__thiscall*)(void*, void*, void*, void*, I32)>(vslot(entry, 0xA0))(
        entry, at(proto, 0x154), at(proto, 0x6C), at(proto, 0xE0), width);                       // [vtbl+A0h] Initialize
    gui_load_control_add_event_0041ab20(entry, nullptr, 0, self, fn(&kotor_fastcall_006c4880));   // OnControlEntered
    gui_load_control_add_event_0041ab20(entry, nullptr, 0x27, self, fn(&kotor_fastcall_006c2df0)); // OnUpgradableSelected
    gui_load_control_add_event_0041ab20(entry, nullptr, 0x2D, self, fn(&kotor_fastcall_006c2df0));
    return entry;
}
// Entry width = LB_ITEMS width (+2F8h) - 2 * border (+324h byte).
inline I32 entry_width(void* self) { return li(self, 0x2F8) - static_cast<I32>(lb(self, 0x324)) * 2; }

// ---- method table 00757228 (27 slots)
const Override ovr_item_select[] = {
    {0x00, fn(&kotor_fastcall_006c5b70)},   // 006C5B70 scalar deleting destructor
    {0x3C, fn(&kotor_fastcall_006c2d30)},   // 006C2D30 HandleInputEvent
    {0x48, fn(&kotor_fastcall_006c6330)},   // 006C6330 OnPanelAdded
};
void* table_item_select[27];
bool built_item_select = false;
const void* vt_item_select() {
    if (!built_item_select) {
        vt_build(table_item_select, 27, gui_load_panel_methods_0073e010, 27, ovr_item_select, 3);
        built_item_select = true;
    }
    return table_item_select;
}
}  // namespace

extern "C" {
// 006C7630 CSWGuiUpgradeItemSelect::CSWGuiUpgradeItemSelect(CSWGuiManager* manager) RET 4.
void* __fastcall kotor_fastcall_006c7630(void* self, void*, void* manager) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                    // 006C7656..006C765B
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);                                    // EH state 0
    set_vptr(self, vt_item_select());                                                             // 006C766D (00757228)
    listbox(self, 0x64, u);                                                                       // 006C7660..006C7673 LB_ITEMS
    listbox(self, 0x344, u);                                                                      // 006C7678..006C7683 LB_DESCRIPTION
    label(self, 0x624, u);                                                                        // 006C7688..006C7693 LBL_TITLE
    label(self, 0x764, u);                                                                        // 006C7698..006C76A3 description label
    button(self, 0x8A4, u);                                                                       // 006C76A8..006C76B5 BTN_UPGRADEITEM
    button(self, 0xA68, u);                                                                       // 006C76BA..006C76C5 BTN_BACK
    start_layout(self, "upgradeitems");                                                           // 006C76CA..006C76E4 (00757438)
    init_control(self, 0x624, "LBL_TITLE");                                                       // 006C76E9..006C771A (00751A20)
    init_control(self, 0x64, "LB_ITEMS");                                                         // 006C771F..006C774A (00751A14)
    init_control(self, 0x344, "LB_DESCRIPTION");                                                  // 006C774F..006C7780 (007564C0)
    sb(self, 0x605, 1);                                                                           // 006C778E
    init_control(self, 0x8A4, "BTN_UPGRADEITEM");                                                 // 006C7785..006C77B7 (00757428)
    init_control(self, 0xA68, "BTN_BACK");                                                        // 006C77BC..006C77ED (007548AC)
    stop_layout(self);                                                                            // 006C77F2..006C77F4
    {
        const U32 proto = lw(self, 0x610);                                                        // 006C77F9 LB_DESCRIPTION prototype
        reinterpret_cast<void(__thiscall*)(void*, U32, U32, float)>(vslot(at(self, 0x764), 0x98))(
            at(self, 0x764), proto + 4u, proto, 1.0f);                                            // 006C77FF..006C7815 [vtbl+98h]
    }
    void* raw = live_operator_new(0x2FB8);                                                        // 006C781B..006C7825
    void* panel = nullptr;
    if (raw != nullptr) {                                                                         // 006C782C..006C7833
        auto g = on_throw([&]() { live_free(raw); });                                             // EH state 0Ch
        panel = kotor_fastcall_006c6b60(raw, nullptr, manager);                                   // 006C7835..006C783B CSWGuiUpgrade
        g.armed = false;
    }
    sw(self, 0xC40, ad(panel));                                                                   // 006C7844
    sw(panel, 0x2FB4, ad(self));                                                                  // 006C784A (unchecked)
    sw(at(self, 0x8A4), 0x44, lw(at(self, 0x8A4), 0x44) & 0xFFFFFFFBu);                           // 006C7850..006C785C
    add_event(self, 0x8A4, 0x27, self, fn(&kotor_fastcall_006c2df0));                             // 006C7853..006C7868
    sw(at(self, 0xA68), 0x44, lw(at(self, 0xA68), 0x44) & 0xFFFFFFFBu);                           // 006C786D..006C7884
    add_event(self, 0xA68, 0x27, self, on_b());                                                   // 006C7879..006C7887 (00624BB0)
    vcall<void, U32>(listbox_items(self), 0xA4, 1);                                               // 006C788C..006C7892 [vtbl+A4h](1)
    sb(self, 0xC2C, 0);                                                                           // 006C789D
    sw(self, 0xC38, 0);                                                                           // 006C78A4
    gui_load_panel_set_active_control_0040a630(self, nullptr, listbox_items(self), 0);            // 006C7898..006C78AE
    u.dismiss();
    return self;
}

// 006C47A0 CSWGuiUpgradeItemSelect::~CSWGuiUpgradeItemSelect().
void __fastcall kotor_fastcall_006c47a0(void* self, void*) {
    set_vptr(self, vt_item_select());                                                             // 006C47BF
    gui_load_listbox_clear_items_00419a90(at(self, 0x64), nullptr);                               // 006C47C5..006C47D2
    gui_load_listbox_clear_items_00419a90(at(self, 0x344), nullptr);                              // 006C47D7..006C47DF
    if (lp(self, 0xC40) != nullptr) {                                                             // 006C47E4..006C47EC
        vcall<void*, U32>(lp(self, 0xC40), 0x00, 1);                                              // 006C47EE..006C47F2 deleting dtor
        sw(self, 0xC40, 0);                                                                       // 006C47F4
    }
    gui_load_button_destruct_00419da0(at(self, 0xA68), nullptr);                                  // 006C47FE..006C4809
    gui_load_button_destruct_00419da0(at(self, 0x8A4), nullptr);                                  // 006C480E..006C4819
    gui_load_label_destruct_00419d10(at(self, 0x764), nullptr);                                   // 006C481E..006C4829
    gui_load_label_destruct_00419d10(at(self, 0x624), nullptr);                                   // 006C482E..006C4839
    gui_load_listbox_destruct_0041b060(at(self, 0x344), nullptr);                                 // 006C483E..006C4845
    gui_load_listbox_destruct_0041b060(at(self, 0x64), nullptr);                                  // 006C484A..006C4851
    gui_load_panel_destruct_0040cf70(self, nullptr);                                              // 006C4856..006C4860
}

// 006C5B70 scalar deleting destructor (thiscall(flags) RET 4).
void* __fastcall kotor_fastcall_006c5b70(void* self, void*, w4::U32 flags) {
    return deleting(self, &kotor_fastcall_006c47a0, flags);                                       // 006C5B70..006C5B8B
}

// 006C2A70: owner creature id of the selected entry, OBJECT_INVALID without one.
w4::U32 __fastcall kotor_fastcall_006c2a70(void* self, void*) {
    void* entry = entry_of(self);                                                                 // 006C2A70..006C2A78
    return entry != nullptr ? lw(entry, 0x1C8) : k_invalid;                                       // 006C2A7A / 006C2A81
}
// 006C2A90: equip slot of the selected entry, 0 without one.
w4::U32 __fastcall kotor_fastcall_006c2a90(void* self, void*) {
    void* entry = entry_of(self);                                                                 // 006C2A90..006C2A98
    return entry != nullptr ? lw(entry, 0x1CC) : 0u;                                              // 006C2A9A / 006C2AA1
}

// 006C2C80 CSWGuiUpgradeItemSelect::SetDescription(const CExoString& text) RET 4.
void __fastcall kotor_fastcall_006c2c80(void* self, void*, void* text) {
    gui_load_text_params_set_text_00415e00(at(self, 0x84C), nullptr, text);                       // 006C2C8C..006C2C93
    void* listbox = at(self, 0x344);                                                              // 006C2C98 LB_DESCRIPTION
    gui_load_listbox_clear_items_00419a90(listbox, nullptr);                                      // 006C2C9E..006C2CA0
    U32 extent[4];
    std::memcpy(extent, at(self, 0x768), 16);                                                     // 006C2CA5..006C2CC4
    U32 height;
    if (async_string_length_005e5790(text, nullptr) > 0)                                          // 006C2CC2..006C2CD5
        height = vcall<U32>(at(self, 0x834), 0x08);                                               // 006C2CD7..006C2CD9
    else
        height = gui_load_text_font_height_00414ee0(at(self, 0x834), nullptr);                    // 006C2CDE
    extent[3] = height;                                                                           // 006C2CE9
    void* label = at(self, 0x764);                                                                // 006C2CED
    vcall<void, void*>(label, 0x04, extent);                                                      // 006C2CE3..006C2CFA SetExtent
    void* single = label;                                                                         // 006C2D0C (argument slot reused)
    gui_m48_listbox_add_controls_0041c2c0(listbox, nullptr, &single, 1, 0, 0, 0);                 // 006C2CFD..006C2D10
    gui_load_listbox_set_active_control_0041c160(listbox, nullptr, label, 0);                     // 006C2D15..006C2D1A
}

// 006C2D30 CSWGuiUpgradeItemSelect::HandleInputEvent(int event, int down) RET 8.
// Jump table 006C2DC8 {006C2D57, 006C2DA3, 006C2D81, 006C2DB6} indexed by bytes 006C2DD8[event - 28h]:
// 28h, 2Eh -> 0 (close); 39h -> 1; 3Ah -> 2; everything else -> 3 (default).
void __fastcall kotor_fastcall_006c2d30(void* self, void*, w4::U32 event, w4::U32 down) {
    if (down != 0 && event - 0x28u <= 0x12u) {                                                    // 006C2D3B..006C2D47
        if (event == 0x28 || event == 0x2E) {                                                     // 006C2D57
            play_sound(self, 0);                                                                  // 006C2D57..006C2D5C PlayGuiSound(0)
            kotor_fastcall_00624a00(self, nullptr, 0);                                            // 006C2D61..006C2D65 setToDestroy(0)
            kotor_fastcall_0040be00(manager_of(self), nullptr);                                   // 006C2D6A..006C2D6D PopModalPanel
        } else if (event == 0x39) {                                                               // 006C2DA3
            vcall<void, U32, U32>(at(self, 0x344), 0x3C, 0x31, 1);                                // 006C2DA3..006C2DB3
        } else if (event == 0x3A) {                                                               // 006C2D81
            vcall<void, U32, U32>(at(self, 0x344), 0x3C, 0x32, 1);                                // 006C2D81..006C2D91
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, down);                             // CSWGuiPanel::HandleInputEvent
}

// 006C2DF0 CSWGuiUpgradeItemSelect::OnUpgradableSelected(CSWGuiControl* control) RET 4.
void __fastcall kotor_fastcall_006c2df0(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                           // 006C2DF5..006C2DFD
    if (lw(control, 0x48) == 0x2D) play_sound(self, 0);                                           // 006C2E03..006C2E0E
    void* server = server_app();                                                                  // 006C2E13..006C2E21 edi
    if (control == at(self, 0x8A4)) {                                                             // 006C2E18..006C2E24 BTN_UPGRADEITEM
        void* selected = kotor_m1_listbox_get_selected_control_0041a5d0(listbox_items(self), nullptr); // 006C2E26..006C2E29
        sw(self, 0xC3C, ad(selected));                                                            // 006C2E30
        if (selected == nullptr) return;                                                          // 006C2E2E..006C2E36
    } else {
        sw(self, 0xC3C, ad(control));                                                             // 006C2E3E
    }
    void* item = recovered_item_lookup_004ae760(server, nullptr, lw(entry_of(self), 0x1C4));     // 006C2E44..006C2E54 ebp
    sw(self, 0xC30, lw(self, 0xC30) & 0xFFFFFFFEu);                                               // 006C2E59..006C2E6A
    if ((lb(entry_of(self), 0x1D0) & 1) != 0) {                                                   // 006C2E64..006C2E77 equipped
        void* creature = kotor_fastcall_004ae770(server, nullptr, lw(entry_of(self), 0x1C8));     // 006C2E7D..006C2E8B
        const U32 slot = lw(entry_of(self), 0x1CC);                                               // 006C2E8D..006C2E93
        if (slot == 0x10 || slot == 0x20) {                                                       // 006C2E99..006C2EA1
            void* right = recovered_equipment_item_005a4c20(lp(creature, 0xA2C), nullptr, 0x10);  // 006C2EA3..006C2EB8 ebx
            void* left = recovered_equipment_item_005a4c20(lp(creature, 0xA2C), nullptr, 0x20);   // 006C2EB0..006C2EBA
            if (left != nullptr) {                                                                // 006C2EBF..006C2EC1
                if (right == item) {                                                              // 006C2EC3..006C2EC5
                    sw(self, 0xC30, (lw(self, 0xC30) & 0xFFFFFFFDu) | 1u);                        // 006C2EC7..006C2ED3
                    sw(self, 0xC34, lw(left, 0x14));                                              // 006C2ED9..006C2EDF
                    recovered_creature_unequip_item_004faa70(creature, nullptr, left);            // 006C2EDC..006C2EE5
                    recovered_creature_unequip_item_004faa70(creature, nullptr, item);            // 006C2EEA..006C2EED
                } else if (left == item) {                                                        // 006C2EF4..006C2EF6
                    sw(self, 0xC30, lw(self, 0xC30) | 3u);                                        // 006C2EF8
                    sw(self, 0xC34, lw(right, 0x14));                                             // 006C2EFF..006C2F05
                    recovered_creature_unequip_item_004faa70(creature, nullptr, item);            // 006C2F02..006C2F0B
                    recovered_creature_unequip_item_004faa70(creature, nullptr, right);           // 006C2F10..006C2F13
                }
                open_upgrade(self, item);                                                         // 006C2F46..006C2F72
                return;
            }
        }
        recovered_creature_unequip_item_004faa70(creature, nullptr, item);                        // 006C2EEA..006C2EED
    } else if (li(item, 0x28C) > 1) {                                                             // 006C2F1A..006C2F21 stack
        item = kotor_fastcall_0055f280(item, nullptr, 1);                                         // 006C2F23..006C2F2C SplitItem(1)
    } else {
        recovered_remove_item_00555fd0(party_items(), nullptr, item);                             // 006C2F30..006C2F41 RemoveItem
    }
    open_upgrade(self, item);                                                                     // 006C2F46..006C2F72
}

// 006C4880 CSWGuiUpgradeItemSelect::OnControlEntered(CSWGuiControl* control) RET 4.
void __fastcall kotor_fastcall_006c4880(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                           // 006C489F..006C48A4
    const U32 id = lw(control, 0x1C4);                                                            // 006C48AA
    if (id != k_invalid) {                                                                        // 006C48B0..006C48B5
        void* item = recovered_item_lookup_004ae760(server_app(), nullptr, id);                   // 006C48B7..006C48C1
        if (item != nullptr) {                                                                    // 006C48C6..006C48C8
            Str d;                                                                                // [esp+4] state 0
            void* r = kotor_fastcall_0055f340(item, nullptr, d.s);                                // 006C48CA..006C48D1 GetPropertyDescription
            d.live = true;
            kotor_fastcall_006c2c80(self, nullptr, r);                                            // 006C48D6..006C48E1
            return;                                                                               // 006C48E6..006C4942 ~d
        }
        Str e("");                                                                                // 006C48EC..006C48F5 (0073D71C) state 1
        kotor_fastcall_006c2c80(self, nullptr, e.p());                                            // 006C48FA..006C4909
        return;
    }
    Str e("");                                                                                    // 006C4914..006C491D (0073D71C) state 2
    kotor_fastcall_006c2c80(self, nullptr, e.p());                                                // 006C4922..006C4931
}

// 006C4960 CSWGuiUpgradeItemSelect::BuildItemList(CSWSCreature* creature, CExoArrayList<CSWGuiControl*>* list,
// C2DA* table) RET 0Ch: one entry per equipped item of the selected upgradable type.
void __fastcall kotor_fastcall_006c4960(void* self, void*, void* creature, void* list, void* table) {
    void* proto = lp(self, 0x330);                                                                // 006C4985 ebp
    const I32 width = entry_width(self);                                                          // 006C497E..006C4999 [esp+14h]
    for (U32 i = 0; i < 0x12; ++i) {                                                              // 006C499D..006C4AAE
        const U32 slot = 1u << i;                                                                 // 006C49A5..006C49AC ebx
        void* item = recovered_equipment_item_005a4c20(lp(creature, 0xA2C), nullptr, slot);       // 006C49AE..006C49B5
        if (item == nullptr) continue;                                                            // 006C49BA..006C49C0
        if (kotor_fastcall_005541c0(item, nullptr, table) != lb(self, 0xC2C)) continue;           // 006C49C6..006C49D8
        void* entry = new_entry(self, proto, width);                                              // 006C49DE..006C4A53
        kotor_fastcall_006c3ea0(entry, nullptr, lw(item, 0x14), 1, lw(creature, 4), slot);        // 006C4A58..006C4A6D SetItem
        list_add(*static_cast<PtrList*>(list), entry);                                            // 006C4A72..006C4AA3 (inline Add, Allocate)
    }
}

// 006C5B90 CSWGuiUpgradeItemSelect::BuildUpgradeList() RET: equipped items of the player and the selectable party
// members, then the party inventory, into LB_ITEMS.
void __fastcall kotor_fastcall_006c5b90(void* self, void*) {
    PtrList list;                                                                                 // [esp+28h] {data,size,cap} EH state 0
    void* server = server_app();                                                                  // 006C5BC0..006C5BC5 ebp
    void* party = lane_config_required::GetPartyTable_004aee70(server, nullptr);                  // 006C5BC8..006C5BCA ebx
    void* repository = recovered_party_repository_00563340(party, nullptr);                       // 006C5BD1..006C5BD3 [esp+18h]
    void* proto = lp(self, 0x330);                                                                // 006C5BDF [esp+20h]
    const I32 width = entry_width(self);                                                          // 006C5BD8..006C5C00 [esp+1Ch]
    void* raw = live_operator_new(0x54);                                                          // 006C5BFA..006C5C04
    void* table = nullptr;                                                                        // [esp+10h]
    if (raw != nullptr) {                                                                         // 006C5C12..006C5C19
        auto g = on_throw([&]() { live_free(raw); });                                             // EH state 1
        menu_resources::Ref name; name.construct_00406d80("upgrade");                             // 006C5C1D..006C5C2B (0075721C)
        table = c2da_current_construct_00413cc0(raw, nullptr, name, 0);                           // 006C5C1B..006C5C32
        g.armed = false;
    }
    c2da_current_load_004143b0(table, nullptr);                                                   // 006C5C4B..006C5C52 (unchecked)
    {
        void* player = kotor_fastcall_004ae770(server, nullptr, player_creature_id(server));      // 006C5C57..006C5C78
        kotor_fastcall_006c4960(self, nullptr, player, &list, table);                             // 006C5C7D..006C5C86
    }
    for (U32 npc = 0; npc < 9; ++npc) {                                                           // 006C5C8B..006C5CD6 (jb)
        if (kotor_fastcall_005636b0(party, nullptr, static_cast<I32>(npc)) == 0) continue;        // 006C5C90..006C5C9A
        if (recovered_party_npc_selectable_005637c0(party, nullptr, npc) == 0) continue;          // 006C5C9C..006C5CA6
        void* creature = kotor_fastcall_004ae770(server, nullptr,
            recovered_npc_object_00564700(party, nullptr, static_cast<I32>(npc), 0, 1));          // 006C5CA8..006C5CB7
        if (creature == nullptr) continue;                                                        // 006C5CBC..006C5CBE
        kotor_fastcall_006c4960(self, nullptr, creature, &list, table);                           // 006C5CC0..006C5CCD
    }
    for (U32 i = 0; i < lw(repository, 0x10); ++i) {                                              // 006C5CD8..006C5E33 (jb)
        void* item = w2x_inventory_item_005560b0(repository, nullptr, i);                         // 006C5CF0..006C5CF3
        if (item == nullptr) continue;                                                            // 006C5CF8..006C5CFC
        if (kotor_fastcall_005541c0(item, nullptr, table) != lb(self, 0xC2C)) continue;           // 006C5D02..006C5D14
        void* entry = new_entry(self, proto, width);                                              // 006C5D1A..006C5D92 (EH state 2)
        kotor_fastcall_006c3ea0(entry, nullptr, lw(item, 0x14), 0, k_invalid, 0);                 // 006C5D97..006C5DA6 SetItem
        if (list.size == list.capacity) {                                                         // 006C5DAB..006C5DB3 inline growth
            const I32 capacity = list.capacity != 0 ? list.capacity * 2 : 0x10;                   // 006C5DB5..006C5DC2
            void** old = list.data;                                                               // 006C5DC2 ebp
            list.capacity = capacity;                                                             // 006C5DCE
            list.data = static_cast<void**>(live_operator_new(static_cast<U32>(capacity) * 4u));  // 006C5DC6..006C5DDB
            for (I32 k = 0; k < list.size; ++k) list.data[k] = old[k];                            // 006C5DE2..006C5E02
            if (old != nullptr) live_free(old);                                                   // 006C5E04..006C5E09
        }
        list.data[list.size] = entry;                                                             // 006C5E11..006C5E19
        ++list.size;                                                                              // 006C5E1C..006C5E25
    }
    if (table != nullptr) vcall<void*, U32>(table, 0x00, 1);                                      // 006C5E39..006C5E45 delete C2DA
    kotor_fastcall_0041c1d0(listbox_items(self), nullptr, &list, 1, 0, 0);                        // 006C5E47..006C5E56 AddControls(list, 1, 0, 0)
}                                                                                                 // 006C5E5B..006C5E68 _free(list.data)

// 006C5E90 CSWGuiUpgradeItemSelect::FinishUpgrading() RET: return the upgraded item to its owner (re-equip) or the
// party inventory, then close (direct mode) or refresh LB_ITEMS.
void __fastcall kotor_fastcall_006c5e90(void* self, void*) {
    void* server = server_app();                                                                  // 006C5E93..006C5E99 ebx
    void* item = lp(upgrade_panel(self), 0x2F54);                                                 // 006C5EA0..006C5EB2 [esp+0Ch] working item
    void* entry = entry_of(self);                                                                 // 006C5EAC
    if ((lb(entry, 0x1D0) & 1) != 0) {                                                            // 006C5EB6..006C5EBE equipped
        U32 slot = lw(entry, 0x1CC);                                                              // 006C5EC4..006C5ECA [esp+10h]
        void* creature = kotor_fastcall_004ae770(server, nullptr, lw(entry, 0x1C8));              // 006C5ECE..006C5ED7 edi
        U32 equip = 1;                                                                            // 006C5EF0 ebp
        if (recovered_item_can_equip_0051aa60(creature, nullptr, item, &slot, 0, 0, 0) == 0) {    // 006C5EDC..006C5EFC
            recovered_inventory_insert_0055d330(party_items(), nullptr, &item, 1, 0, 0);          // 006C5EFE..006C5F18 AddItem(&item, 1, 0, 0)
            kotor_fastcall_006c3ea0(entry_of(self), nullptr, lw(item, 0x14), 0, k_invalid, 0);    // 006C5F1D..006C5F34 SetItem
            equip = 0;                                                                            // 006C5F39
        }
        const U32 flags = lw(self, 0xC30);                                                        // 006C5F3B
        if ((flags & 1u) != 0) {                                                                  // 006C5F41..006C5F43
            if ((flags & 2u) != 0) {                                                              // 006C5F49..006C5F51
                recovered_creature_equip_004feb90(creature, nullptr, 0x10,
                    recovered_item_lookup_004ae760(server, nullptr, lw(self, 0xC34)), 1, 0, 0);   // 006C5F53..006C5F66
                if (equip != 0) recovered_creature_equip_004feb90(creature, nullptr, 0x20, item, 1, 0, 0); // 006C5F6B..006C5F82
            } else if (equip != 0) {                                                              // 006C5F8C..006C5F8E
                recovered_creature_equip_004feb90(creature, nullptr, 0x10, item, 1, 0, 0);        // 006C5F90..006C5F99
                recovered_creature_equip_004feb90(creature, nullptr, 0x20,
                    recovered_item_lookup_004ae760(server, nullptr, lw(self, 0xC34)), 1, 0, 0);   // 006C5F9E..006C5FB7
            } else {
                recovered_creature_equip_004feb90(creature, nullptr, 0x10,
                    recovered_item_lookup_004ae760(server, nullptr, lw(self, 0xC34)), 1, 0, 0);   // 006C5FBE..006C5FD1
            }
        } else if (equip != 0) {                                                                  // 006C5FD8..006C5FDA
            recovered_creature_equip_004feb90(creature, nullptr, slot, item, 1, 0, 0);            // 006C5FDC..006C5FEE
        }
    } else {
        recovered_inventory_insert_0055d330(party_items(), nullptr, &item, 1, 0, 0);              // 006C5FF5..006C6010 AddItem(&item, 1, 0, 0)
    }
    if (lw(self, 0xC38) != 0) {                                                                   // 006C6015..006C601F direct mode
        close_flags(self);                                                                        // 006C6021..006C6049
        kotor_fastcall_0040be00(manager_of(self), nullptr);                                       // 006C604C..006C604F PopModalPanel
        kotor_fastcall_0062e870(ingame_gui(), nullptr);                                           // 006C6054..006C6064 HideUpgradeScreen
        sw(self, 0xC38, 0);                                                                       // 006C6069
        void* selected = entry_of(self);                                                          // 006C606F (+C3C not cleared)
        if (selected != nullptr) vcall<void*, U32>(selected, 0x00, 1);                            // 006C6075..006C607F deleting dtor
        return;
    }
    if (lw(entry_of(self), 0x1C4) != lw(item, 0x14)) kotor_fastcall_006c5b90(self, nullptr);      // 006C6089..006C60A0 BuildUpgradeList
    if (lw(self, 0x304) != 0) {                                                                   // 006C60A5..006C60AB LB_ITEMS count
        I32 i = 0;
        do {                                                                                      // 006C60B0..006C60D0 (stops only on a match)
            void* control = kotor_fastcall_0041a5a0(listbox_items(self), nullptr, i);             // GetControl(i)
            if (lw(control, 0x1C4) == lw(item, 0x14)) { sw(self, 0xC3C, ad(control)); break; }    // 006C60B8..006C60D4
            ++i;
        } while (lw(self, 0x304) != 0);
    }
    gui_load_listbox_set_selected_0041c040(listbox_items(self), nullptr, -1, 0);                  // 006C60DA..006C60E3 SetSelectedControl(-1, 0)
    gui_load_listbox_set_active_control_0041c160(listbox_items(self), nullptr, entry_of(self), 0); // 006C60E8..006C60F3
}

// 006C6330 CSWGuiUpgradeItemSelect::OnPanelAdded() RET.
void __fastcall kotor_fastcall_006c6330(void* self, void*) {
    const U8 type = lb(self, 0xC2C);                                                              // 006C634A
    if (type == 0 || type > 4) { close_flags(self); return; }                                     // 006C6350..006C635B / 006C64B5..006C64DD
    void* item = lp(self, 0xC38);                                                                 // 006C6361
    if (item != nullptr) {                                                                        // 006C6367..006C6369 direct mode (ShowUpgradeScreen(item))
        void* server = server_app();                                                              // 006C636F..006C637C ebp
        void* creature = kotor_fastcall_004ae770(server, nullptr, lw(item, 0x268));               // 006C6375..006C6382 possessor
        const U32 slot = recovered_inventory_slot_from_item_005a4980(lp(creature, 0xA2C), nullptr, lp(self, 0xC38)); // 006C6387..006C6396
        void* raw = live_operator_new(0x3A4);                                                     // 006C639B..006C63A2
        void* entry = nullptr;
        if (raw != nullptr) {                                                                     // 006C63AE..006C63B8
            auto g = on_throw([&]() { live_free(raw); });                                         // EH state 0
            entry = kotor_fastcall_006c3e00(raw, nullptr);                                        // 006C63BA..006C63BC
            g.armed = false;
        }
        sw(self, 0xC3C, ad(entry));                                                               // 006C63CF
        if (static_cast<I32>(slot) > 0) {                                                         // 006C63C5..006C63D5 (jle)
            kotor_fastcall_006c3ea0(entry, nullptr, lw(lp(self, 0xC38), 0x14), 1, lw(creature, 4), slot); // 006C63D7..006C63EA SetItem
            recovered_creature_unequip_item_004faa70(creature, nullptr, lp(self, 0xC38));        // 006C63EF..006C63F8
        } else if (li(lp(self, 0xC38), 0x28C) > 1) {                                              // 006C63FF..006C640C
            sw(self, 0xC38, ad(kotor_fastcall_0055f280(lp(self, 0xC38), nullptr, 1)));            // 006C640E..006C6415 SplitItem(1)
        } else {
            recovered_remove_item_00555fd0(recovered_party_repository_00563340(
                lane_config_required::GetPartyTable_004aee70(server, nullptr), nullptr), nullptr, lp(self, 0xC38)); // 006C641D..006C642E
        }
        open_upgrade(self, lp(self, 0xC38));                                                      // 006C6433..006C6465
        return;                                                                                   // (no CSWGuiPanel::OnPanelAdded)
    }
    kotor_fastcall_006c5b90(self, nullptr);                                                       // 006C647D..006C647F BuildUpgradeList
    gui_load_listbox_set_selected_0041c040(listbox_items(self), nullptr, 0, 0);                   // 006C6484..006C648D SetSelectedControl(0, 0)
    vcall<void, void*, U32>(self, 0x08, listbox_items(self), 0);                                  // 006C6492..006C6499 SetActiveControl
    gui_load_panel_on_added_0040b870(self, nullptr);                                              // 006C649C..006C649E
}
}  // extern "C"
