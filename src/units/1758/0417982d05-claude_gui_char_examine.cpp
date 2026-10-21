// Lane W4 GUI successor (worker "gui-char"): CSWGuiExamine (table vt_examine) and CSWGuiQuestItem (table
// vt_quest_item) slot functions and the helpers they reach that had no provider in the c24 candidate map.
//   006CEA30 CSWGuiExamine::HandleInputEvent    thiscall(int, int) RET 8  (slot)
//   006D24D0 CSWGuiQuestItem::HandleInputEvent  thiscall(int, int) RET 8  (slot)
//   006D2C20 CSWGuiQuestItem::OnPanelAdded      thiscall() RET            (slot)
// helpers reconstructed here:
//   0062D440 CGuiInGame::HideExamineBox                   thiscall() RET
//   006D29F0 CSWGuiQuestItem::PopulateItemListBox         thiscall() RET
//   006D2580 CSWGuiQuestItem::OnControlEntered(CSWGuiControl*)  thiscall RET 4 (also its event-0 handler)
// Transcribed from the original swkotor.exe linear sweep; each statement carries its original PCs.
#include "6030a9ee65-claude_gui_char.hpp"

extern "C" {
void __fastcall gui_m48_message_box_handle_input_006250f0(void* box, void*, w4::U32 event, w4::U32 down); // CSWGuiMessageBox::HandleInputEvent
void __fastcall gui_m48_panel_set_to_destroy_00624a00(void* panel, void*, w4::U32 flag);         // CSWGuiPanel::setToDestroy
void* __fastcall recovered_creature_inventory_004ef770(void* creature, void*, w4::U32 flag);     // CSWSCreature::GetItemRepository
void* __fastcall w2x_inventory_item_005560b0(void* repository, void*, w4::U32 index);            // CItemRepository::ItemListGetItem
void* __fastcall kotor_fastcall_006b7ee0(void* entry, void*);                                    // CSWGuiInGameItemEntry ctor
void __fastcall kotor_fastcall_006b6710(void* entry, void*, w4::U32 id, w4::U32 equipped, w4::U32 marked); // CSWGuiInGameItemEntry::SetItem
void* __fastcall resource_append_005e9dc0(void* list, void*, void* data);                        // CExoLinkedListInternal::AddTail
void* __fastcall kotor_fastcall_0055f340(void* item, void*, void* out);                          // CSWSItem::GetPropertyDescription
w4::I32 __fastcall async_string_length_005e5790(void* string, void*);                            // CExoString::GetLength
w4::U32 __fastcall gui_load_text_font_height_00414ee0(void* text, void*);                        // CSWGuiText::GetFontHeight
void __fastcall gui_m48_listbox_add_controls_0041c2c0(void* listbox, void*, void** controls, w4::U32 count, w4::U32, w4::U32, w4::U32); // AddControls
void __fastcall gui_load_listbox_set_active_control_0041c160(void* listbox, void*, void* control, w4::U32); // SetActiveControl
// ---- defined here
void __fastcall kotor_fastcall_0062d440(void* gui, void*);
void __fastcall kotor_fastcall_006d2580(void* self, void*, void* control);
void __fastcall kotor_fastcall_006d29f0(void* self, void*);
void __fastcall kotor_fastcall_006cea30(void* self, void*, std::uint32_t event, std::uint32_t down);
void __fastcall kotor_fastcall_006d24d0(void* self, void*, std::uint32_t event, std::uint32_t down);
void __fastcall kotor_fastcall_006d2c20(void* self, void*);
}

using namespace gchar;

namespace {
const char k_empty_0073d71c[] = "";                                                              // 0073D71C
const char k_invalid_item_0075654c[] = "Error: Invalid item";                                    // 0075654C
}

extern "C" {
// 0062D440 CGuiInGame::HideExamineBox() RET 0.
void __fastcall kotor_fastcall_0062d440(void* gui, void*) {
    if (lw(gui, 0x108) == 0) return;                                                           // 0062D443..0062D44B
    if (frame_ContainsPanel_0040bd70(lp(gui, 0x38), nullptr, lp(gui, 0x50)) == 0) return;      // 0062D44D..0062D45B PanelExists
    kotor_fastcall_0040be00(lp(gui, 0x38), nullptr);                                           // 0062D45D..0062D460 PopModalPanel
    frame_RemovePanel_0040c830(lp(gui, 0x38), nullptr, lp(gui, 0x50));                         // 0062D465..0062D46C RemovePanel
    if (lw(lp(gui, 0x50), 0x984) == 1)                                                         // 0062D471..0062D47D
        gui_m48_client_set_input_class_005eda60(client_app(), nullptr, 2, 1);                  // 0062D47F..0062D489
    else
        gui_m48_client_set_input_class_005eda60(client_app(), nullptr, 0, 1);                  // 0062D490..0062D49B
}

// 006CEA30 CSWGuiExamine::HandleInputEvent(int event, int down) RET 8. Jump table 006CEA80 / bytes 006CEA88.
void __fastcall kotor_fastcall_006cea30(void* self, void*, std::uint32_t event, std::uint32_t down) {
    if (down != 0 && (event == 0x27 || event == 0x28 || event == 0x2D || event == 0x2E)) {     // 006CEA34..006CEA4C
        play_sound(self, 0);                                                                   // 006CEA53..006CEA58
        kotor_fastcall_0062d440(ingame_gui(), nullptr);                                        // 006CEA5D..006CEA6C HideExamineBox
        return;                                                                                // 006CEA71..006CEA72
    }
    gui_m48_message_box_handle_input_006250f0(self, nullptr, event, down);                     // 006CEA75..006CEA77
}

// 006D2580 CSWGuiQuestItem::OnControlEntered(CSWGuiControl* control) RET 4.
void __fastcall kotor_fastcall_006d2580(void* self, void*, void* control) {
    if (control != nullptr && lw(control, 0x4C) != 0) {                                        // 006D259C..006D25AD
        sw(self, 0xA6C, ad(control));                                                          // 006D25AF
        void* item = recovered_item_lookup_004ae760(server_app(), nullptr, lw(control, 0x1C4)); // 006D25B5..006D25C5
        if (item != nullptr) {                                                                 // 006D25CA..006D25CC
            Str description;
            void* s = kotor_fastcall_0055f340(item, nullptr, description.s); description.live = true; // 006D25CE..006D25D5
            gui_load_text_params_set_text_00415e00(at(self, 0x430), nullptr, s);               // 006D25DA..006D25E9
        } else {                                                                               // 006D25EE..006D2626 ~description
            Str error(k_invalid_item_0075654c);                                                // 006D25F4..006D25FD
            gui_load_text_params_set_text_00415e00(at(self, 0x430), nullptr, error.p());       // 006D2602..006D2615
        }
    }
    U32 extent[4];
    std::memcpy(extent, at(self, 0x34C), 16);                                                  // 006D262B..006D264E
    U32 height;
    if (async_string_length_005e5790(at(self, 0x430), nullptr) > 0)                            // 006D2644..006D265F
        height = vcall<U32>(at(self, 0x418), 0x08);                                            // 006D2661..006D2663
    else
        height = gui_load_text_font_height_00414ee0(at(self, 0x418), nullptr);                 // 006D2668
    extent[3] = height;                                                                        // 006D2677
    void* label = at(self, 0x348);                                                             // 006D2671 edi
    vcall<void, void*>(label, 0x04, extent);                                                   // 006D267B..006D2680 SetExtent
    void* single = label;                                                                      // 006D2695 (argument slot reused)
    gui_m48_listbox_add_controls_0041c2c0(at(self, 0x68), nullptr, &single, 1, 0, 0, 0);       // 006D2683..006D2699
    gui_load_listbox_set_active_control_0041c160(at(self, 0x68), nullptr, label, 0);           // 006D269E..006D26A3
}

// 006D29F0 CSWGuiQuestItem::PopulateItemListBox() RET 0: list the player's plot items.
void __fastcall kotor_fastcall_006d29f0(void* self, void*) {
    void* pc = kotor_fastcall_00635460(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr); // 006D2A08..006D2A1D
    void* repository = recovered_creature_inventory_004ef770(kotor_m15a_creature_get_server_creature_0060fb20(pc, nullptr), nullptr, 1); // 006D2A22..006D2A2D
    PtrList entries;                                                                           // 006D2A3A..006D2A42 [esp+24] state 0
    const I32 width = li(self, 0x71C) - static_cast<I32>(static_cast<U32>(lb(self, 0x748)) * 2u); // 006D2A46..006D2A5D
    void* proto = lp(self, 0x754);                                                             // 006D2A53 ebp
    for (U32 i = 0; i < lw(repository, 0x10); ++i) {                                           // 006D2A61..006D2B52 (unsigned)
        void* item = w2x_inventory_item_005560b0(repository, nullptr, i);                      // 006D2A72..006D2A79
        if (item == nullptr || lw(item, 0x108) == 0) continue;                                 // 006D2A80..006D2A90
        void* memory = live_operator_new(0x39C);                                               // 006D2A96..006D2A9B
        void* entry = nullptr;
        if (memory != nullptr) {                                                               // 006D2AA7..006D2AAE
            auto guard = on_throw([&]() { live_free(memory); });                               // EH state 1
            entry = kotor_fastcall_006b7ee0(memory, nullptr);                                  // 006D2AB0..006D2AB2
            guard.armed = false;
        }
        if (entry == nullptr) continue;                                                        // 006D2ABD..006D2AC4
        vcall<void, void*, void*, void*, I32, U32>(entry, 0xA0, at(proto, 0x154), at(proto, 0x6C), at(proto, 0xE0), width, 0); // 006D2AC6..006D2AE3
        gui_load_control_add_event_0041ab20(entry, nullptr, 0, self, reinterpret_cast<const void*>(&kotor_fastcall_006d2580)); // 006D2AE9..006D2AF3
        kotor_fastcall_006b6710(entry, nullptr, lw(item, 0x14), 0, 0);                         // 006D2AF8..006D2B02 SetItem
        resource_append_005e9dc0(lp(self, 0x64), nullptr, entry);                              // 006D2B07..006D2B0B AddTail
        list_add(entries, entry);                                                              // 006D2B10..006D2B3C
    }
    void* listbox = at(self, 0x488);                                                           // 006D2B62
    kotor_fastcall_0041c1d0(listbox, nullptr, &entries, 0, 1, 0);                              // 006D2B5A..006D2B6B AddControls
    if (void* selected = lp(self, 0xA6C)) vcall<void, U32>(selected, 0x40, 0);                 // 006D2B70..006D2B7D
    if (entries.size > 0) {                                                                    // 006D2B80..006D2B87
        gui_load_listbox_set_selected_0041c040(listbox, nullptr, 0, 0);                        // 006D2B89..006D2B8A
    } else {
        gui_load_listbox_set_selected_0041c040(listbox, nullptr, -1, 0);                       // 006D2B91..006D2B93
        {
            Str empty(k_empty_0073d71c);                                                       // 006D2B98..006D2BA1
            gui_load_text_params_set_text_00415e00(at(self, 0x430), nullptr, empty.p());       // 006D2BA6..006D2BB6
        }                                                                                      // 006D2BBB..006D2BC4
        kotor_fastcall_006d2580(self, nullptr, nullptr);                                       // 006D2BC9..006D2BCC OnControlEntered(0)
    }
}                                                                                              // 006D2BD1..006D2BDE free(entries)

// 006D24D0 CSWGuiQuestItem::HandleInputEvent(int event, int down) RET 8. Jump table 006D255C / bytes 006D256C.
void __fastcall kotor_fastcall_006d24d0(void* self, void*, std::uint32_t event, std::uint32_t down) {
    if (down != 0 && event - 0x28u <= 0x12u) {                                                 // 006D24D7..006D24E7
        if (event == 0x28 || event == 0x2E) {                                                  // 006D24F7
            play_sound(self, 0);                                                               // 006D24F7..006D24FC
            kotor_fastcall_0040be00(manager_of(self), nullptr);                                // 006D2501..006D2504 PopModalPanel
            gui_m48_panel_set_to_destroy_00624a00(self, nullptr, 0);                           // 006D2509..006D250D
        } else if (event == 0x39) {                                                            // 006D2521
            vcall<void, U32, U32>(at(self, 0x68), 0x3C, 0x31, 1);                              // 006D2521..006D252B
        } else if (event == 0x3A) {                                                            // 006D253D
            vcall<void, U32, U32>(at(self, 0x68), 0x3C, 0x32, 1);                              // 006D253D..006D2547
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, down);                          // 006D2512 / 006D252E / 006D254A
}

// 006D2C20 CSWGuiQuestItem::OnPanelAdded.
void __fastcall kotor_fastcall_006d2c20(void* self, void*) {
    kotor_fastcall_006d29f0(self, nullptr);                                                    // 006D2C23 PopulateItemListBox
    gui_load_panel_on_added_0040b870(self, nullptr);                                           // 006D2C2B jmp
}
}  // extern "C"
