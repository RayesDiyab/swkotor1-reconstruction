// Lane W4 GUI successor (worker "gui-inv"), panel CSWGuiContainer (Endar Spire footlockers / corpses loot):
//   006B51D0 CSWGuiContainer::OnPanelAdded        thiscall() RET 0      (slot trap replaced)
//   006B5250 CSWGuiContainer::OnPanelRemoved      thiscall() RET 0      (slot trap replaced)
//   006B8770 CSWGuiContainer::Update              thiscall(int) RET 4   (slot trap replaced)
//   006B92F0 CSWGuiContainer::HandleInputEvent    thiscall(int event, int down) RET 8 (slot trap replaced)
// Callees with no rebuilt body, reconstructed here:
//   006B8410 CSWGuiContainer::SetupGiveMode(int keep_selection)                     RET 4
//   006B7170 CSWGuiContainer::GiveItem(CSWGuiControl*) (item entry events 27h/2Dh)  RET 4
//   006B4FE0 CSWGuiInGameItemEntry::GetNumItems()                                   RET 0
//   0060EEA0 CSWCCreature::PerformGiveItem(DWORD item, DWORD container, int count)  RET 0Ch
//   00677630 CSWCMessage::SendPlayerToServerGuiContainer_Close(DWORD id, int flag)  RET 8
//   00677870 CSWCMessage::SendPlayerToServerInput_GiveItem(DWORD,DWORD,DWORD,int)   RET 10h
// Transcribed from the original swkotor.exe (read-only, capstone sweep); statement comments cite original PCs.
// Container layout (as SetContainer 006B8130): +64h container object id, +68h BYTE flag, +6Ch state flags
// (bit0 give mode, bit1 refresh give list, bit2 skip one update), +7F0h item listbox, +ABCh listbox prototype.
#include "0a92cb2116-claude_gui_inv.hpp"
using namespace gui_inv;

namespace kotor_menu20 { struct String { int as_int_005e5630(); }; }                            // CExoString::AsINT [0021]
namespace kotor_input_cells_required { extern volatile int word_008338f8; }                     // [008338F8] (0774)

extern "C" {
// ---- existing providers (c24 map)
w4::U32 __fastcall kotor_fastcall_004d68a0(void* message, void*, w4::U32 size, w4::U32, w4::U32); // CSWMessage::CreateWriteMessage
void __fastcall kotor_fastcall_004d68e0(void* message, void*, w4::U32 value);                    // CSWMessage::WriteBOOL (004D68E0)
void __fastcall kotor_fastcall_004d6340(void* message, void*, w4::U32 value, w4::U32 bits);      // CSWMessage::WriteDWORD
w4::U32 __fastcall kotor_fastcall_004d6440(void* message, void*, void** data, w4::U32* size);   // CSWMessage::GetWriteMessage
w4::U32 __fastcall recovered_client_send_00677410(void* message, void*, w4::U32 major, w4::U32 minor, void* data, w4::U32 size); // SendPlayerToServerMessage
void __fastcall kotor_fastcall_0062d510(void* gui, void*);                                       // CGuiInGame::HideContainerGui
void __fastcall kotor_fastcall_006b8130(void* container, void*, w4::U32 id, w4::U32 flag);       // CSWGuiContainer::SetContainer
// ---- defined in this file
void __fastcall kotor_fastcall_006b51d0(void* self, void*);
void __fastcall kotor_fastcall_006b5250(void* self, void*);
void __fastcall kotor_fastcall_006b8770(void* self, void*, w4::U32);
void __fastcall kotor_fastcall_006b92f0(void* self, void*, w4::U32 event, w4::U32 down);
void __fastcall kotor_fastcall_006b8410(void* self, void*, w4::U32 keep_selection);
void __fastcall kotor_fastcall_006b7170(void* self, void*, void* control);
w4::I32 __fastcall kotor_fastcall_006b4fe0(void* entry, void*);
w4::U32 __fastcall kotor_fastcall_0060eea0(void* creature, void*, w4::U32 item, w4::U32 container, w4::I32 count);
w4::U32 __fastcall kotor_fastcall_00677630(void* message, void*, w4::U32 id, w4::U32 flag);
w4::U32 __fastcall kotor_fastcall_00677870(void* message, void*, w4::U32 creature, w4::U32 item, w4::U32 container, w4::I32 count);
}

extern "C" {

// 00677630 CSWCMessage::SendPlayerToServerGuiContainer_Close(DWORD id, int flag) RET 8 -> EAX.
U32 __fastcall kotor_fastcall_00677630(void* message, void*, U32 id, U32 flag) {
    U32 size = 8;                                                                              // 0067763C [esp+4] (overwritten below)
    void* data = nullptr;                                                                      // [esp+8]
    kotor_fastcall_004d68a0(message, nullptr, 8, 0xFFFFFFFFu, 1);                              // 00677634..00677644
    kotor_fastcall_004d68e0(message, nullptr, id);                                             // 00677649..00677650
    kotor_fastcall_004d68e0(message, nullptr, flag);                                           // 00677655..0067765C
    const U32 ok = kotor_fastcall_004d6440(message, nullptr, &data, &size);                    // 00677661..0067766D
    if (ok == 0) return ok;                                                                    // 00677672..0067767A
    return recovered_client_send_00677410(message, nullptr, 0x19, 2, data, size);              // 0067767D..0067768D
}

// 00677870 CSWCMessage::SendPlayerToServerInput_GiveItem(DWORD creature, DWORD item, DWORD container, int count) RET 10h.
U32 __fastcall kotor_fastcall_00677870(void* message, void*, U32 creature, U32 item, U32 container, I32 count) {
    U32 size = 0;                                                                              // [esp+4] (written by GetWriteMessage)
    void* data = nullptr;                                                                      // [esp+8]
    kotor_fastcall_004d68a0(message, nullptr, 0x10, 0xFFFFFFFFu, 1);                           // 00677874..0067787C
    kotor_fastcall_004d68e0(message, nullptr, creature);                                       // 00677881..00677888
    kotor_fastcall_004d68e0(message, nullptr, item);                                           // 0067788D..00677894
    kotor_fastcall_004d68e0(message, nullptr, container);                                      // 00677899..006778A0
    kotor_fastcall_004d6340(message, nullptr, static_cast<U32>(count), 0x20);                  // 006778A5..006778AE
    const U32 ok = kotor_fastcall_004d6440(message, nullptr, &data, &size);                    // 006778B3..006778BF
    if (ok == 0) return ok;                                                                    // 006778C4..006778CC
    return recovered_client_send_00677410(message, nullptr, 6, 0x24, data, size);              // 006778CF..006778DF
}

// 0060EEA0 CSWCCreature::PerformGiveItem(DWORD item, DWORD container, int count) RET 0Ch -> EAX.
U32 __fastcall kotor_fastcall_0060eea0(void* creature, void*, U32 item, U32 container, I32 count) {
    void* message = kotor_fastcall_005ed6f0(client_app(), nullptr);                            // 0060EEA0..0060EEAB
    if (message == nullptr) return 0;                                                          // 0060EEB0..0060EEB2
    return kotor_fastcall_00677870(message, nullptr, lw(creature, 4), item, container, count); // 0060EEB4..0060EEC9 ([this+4] id)
}

// 006B4FE0 CSWGuiInGameItemEntry::GetNumItems() RET 0: AsINT of the stack-count text (+16Ch), 1 when it is empty.
I32 __fastcall kotor_fastcall_006b4fe0(void* entry, void*) {
    Str text;                                                                                  // [esp+4]
    WindowCopy005e5b00(text.s, nullptr, at(entry, 0x16c)); text.live = true;                   // 006B4FF9..006B5009
    I32 count = 1;                                                                             // 006B5004 esi
    if (frame_LogIsEmpty_005e58a0(text.s, nullptr) == 0)                                       // 006B500E..006B5021
        count = reinterpret_cast<kotor_menu20::String*>(text.s)->as_int_005e5630();            // 006B5023..006B502C
    return count;                                                                              // 006B502E..006B5050 (~text)
}

// 006B7170 CSWGuiContainer::GiveItem(CSWGuiControl* entry) RET 4 (item entry events 27h and 2Dh in give mode).
void __fastcall kotor_fastcall_006b7170(void* self, void*, void* control) {
    if (lw(control, 0x4c) == 0) return;                                                        // 006B7176..006B717D
    if (lw(control, 0x48) != 0) play_sound(self, lb(control, 0x54));                           // 006B717F..006B718F
    I32 count = 1;                                                                             // 006B719C
    if (kotor_input_cells_required::word_008338f8 != 0) count = kotor_fastcall_006b4fe0(control, nullptr); // 006B7194..006B71A5
    const U32 item = lw(control, 0x1c4);                                                       // 006B71AD
    void* player = kotor_fastcall_00635460(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr); // 006B71B3..006B71C6
    kotor_fastcall_0060eea0(player, nullptr, item, lw(self, 0x64), count);                     // 006B71AA..006B71CD
    sw(self, 0x6c, lw(self, 0x6c) | 6u);                                                       // 006B71D2
}

// 006B8410 CSWGuiContainer::SetupGiveMode(int keep_selection) RET 4: lists the party items (not equipped: item +108h
// == 0) as give entries, captions "<GUI BB0Ch> <GUI 968Eh>", clears give-mode bit 0. Quirks kept: entries are owned by
// the listbox and only the stack pointer table is freed; the ItemListGetItemObjectID / ServerToClientObjectId pair is
// evaluated for every repository index, also for skipped items.
void __fastcall kotor_fastcall_006b8410(void* self, void*, U32 keep_selection) {
    PtrList entries;                                                                           // 006B842C..006B8438 [esp+48h]
    void* const prototype = lp(self, 0xabc);                                                   // 006B843C
    void* const text = at(prototype, 0xd0);                                                    // 006B8442 [esp+30h]
    void* const border = at(prototype, 0x5c);                                                  // 006B8448 [esp+28h]
    void* const highlight = at(prototype, 0x140);                                              // 006B844B..006B8450 [esp+20h]
    void* const client = client_app();                                                         // 006B8454..006B8460 [esp+1Ch]
    void* const repository = recovered_party_repository_00563340(party_table(), nullptr);     // 006B8464..006B8476 [esp+14h]
    const U32 width = lw(self, 0xa84) - (static_cast<U32>(lb(self, 0xab0)) << 1);             // 006B847B..006B8490 ebx
    void* const listbox = at(self, 0x7f0);                                                     // 006B8488 ebp
    U32 added = 0;                                                                             // edi (byte offset / 4)
    for (U32 i = 0; i < lw(repository, 0x10); ++i) {                                           // 006B8492..006B849F, 006B85BC..006B85CE
        const U32 id = kotor_fastcall_005eda50(client, nullptr,
                                               kotor_fastcall_005560e0(repository, nullptr, static_cast<I32>(i))); // 006B84B0..006B84CF [esp+38h]
        void* item = w2x_inventory_item_005560b0(repository, nullptr, i);                      // 006B84C6..006B84D3
        if (item == nullptr || lw(item, 0x108) != 0) continue;                                 // 006B84D8..006B84E8
        void* entry = new_item_entry();                                                        // 006B84EE..006B851D
        list_add(&entries, entry);                                                             // 006B851D..006B855A
        void* added_entry = entries.data[added];                                               // 006B8561 [edi+data]
        vcall<void, void*, void*, void*, U32, U32>(added_entry, 0xa0, text, border, highlight, width, 1); // 006B855B..006B8576 Initialize
        kotor_fastcall_006b6710(entries.data[added], nullptr, id, 0, 0);                        // 006B857C..006B858C SetItem
        gui_load_control_add_event_0041ab20(entries.data[added], nullptr, 0x27, self, w4gui::fn(&kotor_fastcall_006b7170)); // 006B8591..006B85A0
        gui_load_control_add_event_0041ab20(entries.data[added], nullptr, 0x2d, self, w4gui::fn(&kotor_fastcall_006b7170)); // 006B85A5..006B85B4
        ++added;                                                                               // 006B85B9
    }
    Str caption;                                                                               // [esp+40h]
    {
        Str space(" ");                                                                        // 006B85D6..006B85DF (0073D5D8) [esp+38h]
        Str gui_a, gui_b, joined;                                                              // [esp+20h] / [esp+28h] / [esp+30h]
        void* a = gui_string(gui_a, 0x968e);                                                   // 006B85E4..006B85FB ebx
        void* b = gui_string(gui_b, 0xbb0c);                                                   // 006B8602..006B8619
        void* left = concat(b, joined, space.p());                                             // 006B861E..006B862F
        concat(left, caption, a);                                                              // 006B8634..006B8641
        joined.destroy();                                                                      // 006B8646..006B864F
        gui_b.destroy();                                                                       // 006B8654..006B865D
        gui_a.destroy();                                                                       // 006B8662..006B866B
    }                                                                                          // 006B8670..006B8679 ~space
    gui_load_text_params_set_strref_00415e50(at(self, 0x158), nullptr, 0x188);                 // 006B867E..006B8689
    gui_load_text_params_set_strref_00415e50(at(self, 0x3d8), nullptr, 0x968f);                // 006B868E..006B8699
    gui_load_text_params_set_strref_00415e50(at(self, 0xc3c), nullptr, 0x968f);                // 006B869E..006B86A9
    gui_load_text_params_set_text_00415e00(at(self, 0xfc4), nullptr, caption.p());             // 006B86AE..006B86B9
    sw(self, 0x6c, lw(self, 0x6c) & 0xFFFFFFFEu);                                              // 006B86BE
    sw(listbox, 0x2bc, lw(listbox, 0x2bc) & 0xFFFFFDFFu);                                      // 006B86C2
    vcall<void, U32>(listbox, 0x88, 1);                                                        // 006B86CC..006B86D3
    I32 selected = 0;                                                                          // 006B86DD ebx
    if (keep_selection != 0 && entries.size > 0) {                                             // 006B86D9..006B86E9
        const I32 saved = static_cast<std::int16_t>(lh(self, 0xab6));                         // 006B86EB
        selected = entries.size - 1;                                                           // 006B86F2
        if (saved < selected) selected = saved;                                                // 006B86F5..006B86F9
    }
    kotor_fastcall_0041c1d0(listbox, nullptr, &entries, 1, 0, 0);                              // 006B86FB..006B8706 AddControls
    vcall<void, void*, U32>(self, 0x08, nullptr, 0);                                           // 006B870B..006B8711
    vcall<void, void*, U32>(self, 0x08, listbox, 0);                                           // 006B8714..006B871A
    if (entries.size > 0) gui_load_listbox_set_selected_0041c040(listbox, nullptr, selected, 0); // 006B871D..006B8727
}                                                                                              // 006B872C..006B8747 ~caption, _free

// 006B8770 CSWGuiContainer::Update(int) RET 4.
void __fastcall kotor_fastcall_006b8770(void* self, void*, U32) {
    const U32 flags = lw(self, 0x6c);                                                          // 006B8773
    if ((flags & 4u) != 0) { sw(self, 0x6c, flags & 0xFFFFFFFBu); return; }                    // 006B8776..006B8781
    if ((flags & 2u) != 0 && (flags & 1u) == 0) {                                              // 006B8784..006B878A
        kotor_fastcall_006b8410(self, nullptr, 1);                                             // 006B878C..006B8790 SetupGiveMode(1)
        sw(self, 0x6c, lw(self, 0x6c) & 0xFFFFFFFDu);                                          // 006B8795
    }
}

// 006B51D0 CSWGuiContainer::OnPanelAdded() RET 0: park the mouse at the centre of the item listbox extent (+AD4h, in
// panel coordinates), then CSWGuiPanel::OnPanelAdded (tail jump).
void __fastcall kotor_fastcall_006b51d0(void* self, void*) {
    const I32 panel_x = li(self, 4), panel_y = li(self, 8);                                    // 006B51D8..006B51DE edi / ebx
    const I32 x = li(self, 0xad4), y = li(self, 0xad8), w = li(self, 0xadc), h = li(self, 0xae0); // 006B51E7..006B520E
    gui_m48_manager_store_mouse_0040a3d0(gui_manager(), nullptr);                              // 006B5204..006B5212
    const I32 mouse_y = h / 2 + y + panel_y;                                                   // 006B5217..006B522C (cdq/sar)
    const I32 mouse_x = w / 2 + panel_x + x;                                                   // 006B522F..006B523A
    gui_m48_manager_move_mouse_0040c790(gui_manager(), nullptr, mouse_x, mouse_y);             // 006B5226..006B523D
    gui_load_panel_on_added_0040b870(self, nullptr);                                           // 006B5242..006B524B
}

// 006B5250 CSWGuiContainer::OnPanelRemoved() RET 0.
void __fastcall kotor_fastcall_006b5250(void* self, void*) {
    gui_m48_manager_restore_mouse_0040c7d0(gui_manager(), nullptr);                            // 006B5253..006B5259
    gui_load_panel_on_removed_0040c170(self, nullptr);                                         // 006B525E..006B5261
}

// 006B92F0 CSWGuiContainer::HandleInputEvent(int event, int down) RET 8. Jump table 006B9424 (event - 27h):
// 27h -> 006B9327, 28h/2Eh -> 006B938A, 29h -> 006B93D5, 2Dh -> 006B9317, 2Ah..2Ch -> default.
void __fastcall kotor_fastcall_006b92f0(void* self, void*, U32 event, U32 down) {
    if (down != 0) {                                                                           // 006B92F1..006B92FA
        bool close_give = false;
        switch (event) {                                                                       // 006B9300..006B9310
        case 0x2d:                                                                             // 006B9317
            if ((lb(self, 0x6c) & 1u) != 0) { close_give = true; break; }                      // 006B9317..006B931B
            play_sound(self, 0);                                                               // 006B931D..006B9322
            close_give = (lb(self, 0x6c) & 1u) != 0;                                           // 006B9327..006B932B (re-read)
            break;
        case 0x27:                                                                             // 006B9327
            close_give = (lb(self, 0x6c) & 1u) != 0;                                           // 006B9327..006B932B
            break;
        case 0x28: case 0x2e:                                                                  // 006B938A
            play_sound(self, 0);                                                               // 006B938A..006B938F
            kotor_fastcall_00677630(kotor_fastcall_005ed6f0(client_app(), nullptr), nullptr, lw(self, 0x64), 0); // 006B9394..006B93AA
            kotor_fastcall_0062d510(ingame_gui(), nullptr);                                    // 006B93AF..006B93BE
            break;
        case 0x29:                                                                             // 006B93D5
            play_sound(self, 0);                                                               // 006B93D5..006B93DA
            if ((lb(self, 0x6c) & 1u) != 0) kotor_fastcall_006b8410(self, nullptr, 0);         // 006B93DF..006B93E9 SetupGiveMode(0)
            else kotor_fastcall_006b8130(self, nullptr, lw(self, 0x64), lb(self, 0x68));        // 006B9400..006B940B SetContainer
            break;
        default:
            break;
        }
        if (close_give) {                                                                      // 006B9331
            const U32 flag = lb(self, 0x68) == 1 ? 1u : 0u;                                    // 006B9331..006B933B edi
            play_sound(self, 0);                                                               // 006B9340..006B9345
            kotor_fastcall_00677630(kotor_fastcall_005ed6f0(client_app(), nullptr), nullptr, lw(self, 0x64), flag); // 006B934A..006B935E
            kotor_fastcall_0062d510(ingame_gui(), nullptr);                                    // 006B9363..006B9372
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, down);                          // 006B9377..006B9380 / 006B9410..006B9418
}

}  // extern "C"
