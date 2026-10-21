// Lane W4 GUI successor (worker "gui-inv"), panel CSWGuiInGameInventory (construction: panels_menus.cpp 006B34C0):
//   006B3ED0 CSWGuiInGameInventory::HandleInputEvent   thiscall(int event, int down) RET 8 (slot trap replaced)
//   006B4430 CSWGuiInGameInventory::OnPanelRemoved     thiscall() RET 0                 (slot trap replaced)
//   006B4BB0 CSWGuiInGameInventory::Draw               thiscall(int) RET 4              (slot trap replaced)
//   006B4C30 CSWGuiInGameInventory::OnPanelAdded       thiscall() RET 0                 (slot trap replaced)
// Callees with no rebuilt body, reconstructed here:
//   006B2A80 SetNextFilter()                                                   RET 0
//   006B4810 PopulateItemListBox()                                             RET 0
//   006B28C0 UpdatePlayerStats()                                               RET 0
//   006B2C90 CheckFilter(CSWSItem*, CSWSCreature*, int equipped)               RET 0Ch
//   006B44D0 CreateItemEntry(CSWCCreature*, int* index, CExoArrayList<CSWGuiControl*>*, CSWSItem*, int equipped,
//            int mark)                                                          RET 18h
//   item entry event handlers registered by CreateItemEntry (thiscall(CSWGuiControl*) RET 4):
//   006B3D10 OnControlEntered, 006B25E0 OnControlSelected, 006B26E0 CantEquip, 006B2740 NotUseable,
//   006B27A0 NotUseableOnHawk, 006B2800 FullHealth, 006B2860 SquadFullHealth
//   00556050 CItemRepository::ClearNewItems(CExoArrayList<DWORD>*)             RET 4
//   004EFE30 CSWSCreature::InstantUseItem(DWORD item)                          RET 4
// Transcribed from the original swkotor.exe (read-only, capstone sweep); statement comments cite original PCs.
// Inventory fields: +564h LB_ITEMS, +844h LB_DESC, +1DC0h cached item entries {data,size,cap}, +1DCCh new-item id
// list, +1DD8h item to reselect, +1DDCh/+1DDEh saved listbox top/selection, +1DE0h party-select NPC (FFh = none),
// +1DE4h flags (bit0 repopulate, bit1 keep scroll position, bit2 description shows an equippable item).
#include "0a92cb2116-claude_gui_inv.hpp"
using namespace gui_inv;

namespace recovered_save_stats { unsigned short __cdecl maximum_hp_00647a80(void* stats); }        // CSWCCreatureStats::GetMaxHitPoints

extern "C" {
// ---- existing providers (c24 map)
void* __fastcall recovered_item_lookup_004ae760(void* server, void*, w4::U32 id);                 // CServerExoApp::GetItemByGameObjectID
void* __fastcall recovered_creature_inventory_004ef770(void* creature, void*, w4::U32 flag);      // CSWSCreature::GetItemRepository
w4::I32 __fastcall w2x_creature_gold_004edd60(void* creature, void*);                             // CSWSCreature::GetGold
void* __fastcall recovered_creature_client_creature_004efb20(void* creature, void*);              // CSWSCreature::GetClientCreature
w4::U32 __fastcall recovered_creature_can_use_item_0051b440(void* creature, void*, void* item, w4::U32); // CSWSCreature::CanUseItem
w4::U32 __fastcall kotor_fastcall_00616520(void* creature, void*, void* item, w4::U32 equipped, w4::U32 mask); // IsLeaderUsableItem
std::int16_t __fastcall kotor_m15b_creature_stats_get_armor_class_00647720(void* stats, void*, w4::U32); // CSWCCreatureStats::GetArmorClass
void* __fastcall kotor_m1_listbox_get_selected_control_0041a5d0(void* listbox, void*);            // CSWGuiListBox::GetSelectedControl
void __fastcall gui_load_listbox_set_active_control_0041c160(void* listbox, void*, void* control, w4::U32); // SetActiveControl
void __fastcall gui_m48_listbox_add_controls_0041c2c0(void* listbox, void*, void** controls, w4::I32 count,
                                                      w4::U32, w4::U32, w4::U32) noexcept(false); // AddControls(array)
void __fastcall kotor_fastcall_00627130(void* box, void*, w4::U32 allow);                         // CSWGuiMessageBox::SetAllowCancel
w4::U32 __fastcall async_string_length_005e5790(void* string, void*);                             // CExoString::GetLength
void* __fastcall kotor_fastcall_0055f340(void* item, void*, void* out);                           // CSWSItem::GetPropertyDescription
void __fastcall w1x_unique_append_006449d0(void* list, void*, w4::U32 value);                    // CExoArrayList<DWORD>::AddUnique
void* __fastcall kotor_m15_menu_active_property_00553960(void* item, void*, w4::I32 index);       // CSWSItem::GetActiveProperty
void __fastcall kotor_fastcall_004cdf50(void* object, void*, w4::U32 spell, const void* location, w4::U32 target,
                                        w4::U32, w4::U32 item, w4::U32, w4::U32, w4::U32, w4::U32); // CSWSObject::SpellCastAndImpact
void __fastcall kotor_fastcall_0055de20(void* item, void*, w4::I32 index, w4::U32 owner);        // CSWSItem::DecrementUses
void __fastcall show_tutorial_window_005edf40(void* app, void*, int, unsigned, unsigned, int);   // CClientExoApp::ShowTutorialWindow
// ---- other W4 owners
void __fastcall kotor_fastcall_006b2dc0(void* self, void*);                                       // UpdatePortraits (claude_menu_handlers_b.cpp)
w4::U32 __fastcall kotor_fastcall_005edf80(void* app, void*, w4::U32, w4::U32, w4::U32);          // ChangeCharacterToNextLivingPartyMember
// ---- defined in this file
void __fastcall kotor_fastcall_006b3ed0(void* self, void*, w4::U32 event, w4::U32 down);
void __fastcall kotor_fastcall_006b4430(void* self, void*);
void __fastcall kotor_fastcall_006b4bb0(void* self, void*, w4::U32 arg);
void __fastcall kotor_fastcall_006b4c30(void* self, void*);
void __fastcall kotor_fastcall_006b2a80(void* self, void*);
void __fastcall kotor_fastcall_006b4810(void* self, void*);
void __fastcall kotor_fastcall_006b28c0(void* self, void*);
w4::U32 __fastcall kotor_fastcall_006b2c90(void* self, void*, void* item, void* creature, w4::U32 equipped);
void __fastcall kotor_fastcall_006b44d0(void* self, void*, void* creature, w4::I32* index, void* list, void* item,
                                        w4::U32 equipped, w4::U32 mark);
void __fastcall kotor_fastcall_006b3d10(void* self, void*, void* control);
void __fastcall kotor_fastcall_006b25e0(void* self, void*, void* control);
void __fastcall kotor_fastcall_006b26e0(void* self, void*, void* control);
void __fastcall kotor_fastcall_006b2740(void* self, void*, void* control);
void __fastcall kotor_fastcall_006b27a0(void* self, void*, void* control);
void __fastcall kotor_fastcall_006b2800(void* self, void*, void* control);
void __fastcall kotor_fastcall_006b2860(void* self, void*, void* control);
void __fastcall kotor_fastcall_00556050(void* repository, void*, void* list);
void __fastcall kotor_fastcall_004efe30(void* creature, void*, w4::U32 item);
}

namespace {
// .rdata 00756444: filter caption strrefs (all, new, equipped(?), ..., indexed by GetInGameGui()+BC1h).
const U32 filter_names[6] = {0xa35e, 0xa4b5, 0xa35a, 0xa35d, 0xa35b, 0xa35c};
inline void* tlk_string(Str& out, U32 strref) {                                                   // CTlkTable::GetSimpleString
    void* r = static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08)->simple_0041e8f0(
        static_cast<kotor_menu_10_19::String*>(out.p()), strref);
    out.live = true;
    return r;
}
// The client creature shown: party-select NPC (+1DE0h, server info +104h bit 0) or the party's player character
// (006B4837..006B4892 / 006B28DD..006B2943).
void* shown_creature(void* self) {
    if (lb(self, 0x1de0) != 0xff && (lb(recovered_server_info_004aec90(server_app(), nullptr), 0x104) & 1u) != 0) {
        const I32 npc = static_cast<std::int8_t>(lb(self, 0x1de0));
        void* app = client_app();
        const U32 object = recovered_npc_object_00564700(party_table(), nullptr, npc, 0, 1);
        return pt(frame_R4_005ed5b0(app, nullptr, kotor_fastcall_005eda50(app, nullptr, object)));
    }
    return kotor_fastcall_00635460(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr);
}
// Message-box refusal shared by the entry handlers (006B27AE..006B27ED and copies): GetInGameGui()+98h.
void refuse(void* self, U32 strref) {
    void* box = lp(ingame_gui(), 0x98);                                                          // 006B27AE..006B27BD
    kotor_fastcall_00627130(box, nullptr, 0);                                                    // 006B27C3..006B27C7 SetAllowCancel(0)
    vcall<void, U32>(box, 0x6c, strref);                                                         // 006B27CC..006B27D5
    gui_load_messagebox_set_callback_00624a40(box, nullptr, nullptr, 0, 0);                      // 006B27D8..006B27E0 SetCallback(0,0,0)
    kotor_fastcall_0040bc70(lp(self, 0x18), nullptr, box, 1, 1);                                 // 006B27E5..006B27ED AddPanel(box,1,1)
}
inline void refuse_if_active(void* self, void* control, U32 strref) {
    if (lw(control, 0x4c) == 0) return;                                                          // 006B27A0..006B27AC
    refuse(self, strref);
}
inline void add_event(void* entry, I32 id, void* self, void (__fastcall* handler)(void*, void*, void*)) {
    gui_load_control_add_event_0041ab20(entry, nullptr, id, self, w4gui::fn(handler));
}
inline U16 current_hp(void* client_creature) { return lh(lp(client_creature, 0x2f8), 0x4c); }    // [[c+2F8h]+4Ch]
inline U16 maximum_hp(void* client_creature) { return recovered_save_stats::maximum_hp_00647a80(lp(client_creature, 0x2f8)); }
}

extern "C" {

// 004EFE30 CSWSCreature::InstantUseItem(DWORD item) RET 4: casts the spell of the first active property of type 0Ah at
// the creature itself and decrements its uses. Quirk kept: the item lookup result is not checked.
void __fastcall kotor_fastcall_004efe30(void* creature, void*, U32 item_id) {
    void* item = recovered_item_lookup_004ae760(server_app(), nullptr, item_id);              // 004EFE30..004EFE4B
    for (I32 i = 0; i < li(item, 0x248); ++i) {                                                // 004EFE4D..004EFE77
        void* property = kotor_m15_menu_active_property_00553960(item, nullptr, i);           // 004EFE60..004EFE63
        if (lh(property, 0) != 0xa) continue;                                                  // 004EFE68..004EFE6C
        const U32 location[3] = {0, 0, 0};                                                     // 004EFE9E..004EFEAE [esp+10h]
        kotor_fastcall_004cdf50(creature, nullptr, lh(property, 2), location, lw(creature, 4), 0, item_id, 0, 0, 0, 0); // 004EFE83..004EFEB6
        kotor_fastcall_0055de20(item, nullptr, i, lw(creature, 4));                            // 004EFEBB..004EFEC2
        if (lw(creature, 0x4e0) != 0) sw(creature, 0xab0, 0xbb8);                              // 004EFEC7..004EFED1
        return;
    }
}

// 00556050 CItemRepository::ClearNewItems(CExoArrayList<DWORD>* ids) RET 4: from the back, clears the "new" bit 7 of
// +288h of every listed item that still exists, decrements the repository's +4 count and removes the id.
void __fastcall kotor_fastcall_00556050(void* repository, void*, void* list) {
    for (I32 i = li(list, 4) - 1; i >= 0; --i) {                                               // 00556052..0055605D, 005560A6..005560A7
        void* item = recovered_item_lookup_004ae760(server_app(), nullptr, lw(lp(list, 0), static_cast<U32>(i) * 4u)); // 00556060..0055606E
        if (item == nullptr) continue;                                                         // 00556073..00556075
        sw(item, 0x288, lw(item, 0x288) & 0xFFFFFF7Fu);                                        // 00556077
        sw(repository, 4, lw(repository, 4) - 1u);                                             // 00556081
        const I32 size = li(list, 4) - 1;                                                      // 00556084..00556087
        sw(list, 4, static_cast<U32>(size));                                                   // 0055608E
        for (I32 k = i; k < li(list, 4); ++k)                                                  // 0055608A..005560A4 (size re-read)
            sw(lp(list, 0), static_cast<U32>(k) * 4u, lw(lp(list, 0), static_cast<U32>(k + 1) * 4u));
    }
}

// 006B2C90 CSWGuiInGameInventory::CheckFilter(CSWSItem* item, CSWSCreature* creature, int equipped) RET 0Ch.
// Jump table 006B2D9C on GetInGameGui()+BC1h.
U32 __fastcall kotor_fastcall_006b2c90(void*, void*, void* item, void* creature, U32 equipped) {
    if (item == nullptr) return 0;                                                             // 006B2C92..006B2CA0
    switch (lb(ingame_gui(), 0xbc1)) {                                                         // 006B2CA3..006B2CC0
    case 0: return 1;                                                                          // 006B2CC7
    case 1: return (lw(item, 0x288) >> 7) & 1u;                                                // 006B2CD3..006B2CE3
    case 2: return lw(item, 0x108);                                                            // 006B2CE6..006B2CF0
    case 3:                                                                                    // 006B2CF3
        if (kotor_m15b_item_get_base_item_005b4790(item, nullptr) == nullptr) return 0;         // 006B2CF3..006B2CFC
        return lw(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 4) != 0 ? 1u : 0u;     // 006B2D02..006B2D19
    case 4:                                                                                    // 006B2D1C
        if (kotor_m15b_item_get_base_item_005b4790(item, nullptr) == nullptr) return 0;         // 006B2D1C..006B2D25
        if (lw(item, 0x108) != 0) return 0;                                                    // 006B2D27..006B2D2F
        return lw(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 4) == 0 ? 1u : 0u;     // 006B2D31..006B2D45
    case 5:                                                                                    // 006B2D48
        if (recovered_creature_client_creature_004efb20(creature, nullptr) == nullptr) return 0; // 006B2D49..006B2D56
        if (kotor_fastcall_00616520(recovered_creature_client_creature_004efb20(creature, nullptr), nullptr,
                                    item, equipped, 0xff) == 0) return 0;                      // 006B2D58..006B2D73
        if (recovered_creature_can_use_item_0051b440(creature, nullptr, item, 0) == 0) return 0; // 006B2D75..006B2D81
        return 1;                                                                              // 006B2D83..006B2D8D
    default: return 0;                                                                         // 006B2D93
    }
}

// Entry event handlers: message box refusals (thiscall(CSWGuiControl*) RET 4).
void __fastcall kotor_fastcall_006b26e0(void* self, void*, void* control) { refuse_if_active(self, control, 0xa5f5); } // CantEquip
void __fastcall kotor_fastcall_006b2740(void* self, void*, void* control) { refuse_if_active(self, control, 0xa5f6); } // NotUseable
void __fastcall kotor_fastcall_006b27a0(void* self, void*, void* control) { refuse_if_active(self, control, 0xbff9); } // NotUseableOnHawk
void __fastcall kotor_fastcall_006b2800(void* self, void*, void* control) { refuse_if_active(self, control, 0xa603); } // FullHealth
void __fastcall kotor_fastcall_006b2860(void* self, void*, void* control) { refuse_if_active(self, control, 0xbd6e); } // SquadFullHealth

// 006B25E0 CSWGuiInGameInventory::OnControlSelected(CSWGuiControl*) RET 4: uses the selected item instantly, or refuses
// (strref A5A9h) while the player is in combat (+4E0h, +AC0h == 1, +AB0h). Quirk kept: the player creature lookup is
// not checked.
void __fastcall kotor_fastcall_006b25e0(void* self, void*, void* control) {
    if (lw(control, 0x4c) == 0) return;                                                        // 006B25E0..006B25EC
    void* selected = kotor_m1_listbox_get_selected_control_0041a5d0(at(self, 0x564), nullptr); // 006B25F2..006B25F8
    if (selected == nullptr) return;                                                           // 006B25FD..006B25FF
    const U32 item = lw(selected, 0x1c4);                                                      // 006B2606
    if (item == 0x7f000000u) return;                                                           // 006B260C..006B2613
    void* server = server_app();                                                               // 006B2619..006B261F
    void* player = kotor_fastcall_004ae770(server, nullptr, kotor_fastcall_004aea40(server, nullptr)); // 006B2622..006B262C
    if (lw(player, 0x4e0) != 0 && lb(player, 0xac0) == 1 && lw(player, 0xab0) != 0) {        // 006B2631..006B264C
        refuse(self, 0xa5a9);                                                                  // 006B264E..006B268C
        return;
    }
    sw(player, 0x1cc, 0);                                                                      // 006B269A
    kotor_fastcall_004efe30(player, nullptr, item);                                            // 006B2697..006B26A4 InstantUseItem
    sw(self, 0x1de4, lw(self, 0x1de4) | 3u);                                                   // 006B26A9..006B26C0
    sw(self, 0x1dd8, item);                                                                    // 006B26C6
    std::memcpy(at(self, 0x1ddc), at(self, 0x82c), 2);                                         // 006B26AF / 006B26CC
    std::memcpy(at(self, 0x1dde), at(self, 0x82a), 2);                                         // 006B26B6 / 006B26D3
}

// 006B3D10 CSWGuiInGameInventory::OnControlEntered(CSWGuiControl* entry) RET 4: item description into LB_DESC.
void __fastcall kotor_fastcall_006b3d10(void* self, void*, void* control) {
    if (lw(control, 0x4c) == 0) return;                                                        // 006B3D2B..006B3D34
    const U32 item_id = lw(control, 0x1c4);                                                    // 006B3D3B
    void* const desc_params = at(self, 0x14c);                                                 // edi
    if (item_id != 0x7f000000u) {                                                              // 006B3D41..006B3D48
        void* item = recovered_item_lookup_004ae760(server_app(), nullptr, item_id);          // 006B3D4E..006B3D58 ebp
        Str description;                                                                       // [esp+8] (frame base)
        kotor_fastcall_0055f340(item, nullptr, description.s); description.live = true;        // 006B3D5D..006B3D66
        gui_load_text_params_set_text_00415e00(desc_params, nullptr, description.p());         // 006B3D6B..006B3D80
        U32 flags;
        if (lb(self, 0x1de0) != 0xff && (lb(recovered_server_info_004aec90(server_app(), nullptr), 0x104) & 1u) != 0) // 006B3D85..006B3DA2
            flags = lw(self, 0x1de4) & 0xFFFFFFFBu;                                            // 006B3DA4..006B3DAA
        else {
            const U32 old = lw(self, 0x1de4);                                                  // 006B3DAF..006B3DB6
            flags = (((lw(control, 0x58) << 2) ^ old) & 4u) ^ old;                             // 006B3DBC..006B3DC4
        }
        sw(self, 0x1de4, flags);                                                               // 006B3DC6
        if (static_cast<std::int8_t>(lb(item, 0x288)) < 0) w1x_unique_append_006449d0(at(self, 0x1dcc), nullptr, item_id); // 006B3DCC..006B3DDE
    } else {                                                                                   // 006B3DE9
        sw(self, 0x1de4, lw(self, 0x1de4) & 0xFFFFFFFBu);                                      // 006B3DE9..006B3DFB
        Str text("Error: Invalid item");                                                       // 006B3DF2..006B3E01 (0075654C) [esp+10h]
        gui_load_text_params_set_text_00415e00(desc_params, nullptr, text.p());                // 006B3E06..006B3E1B
    }                                                                                          // 006B3E20..006B3E2C ~string
    void* const listbox = at(self, 0x844);                                                     // 006B3E31 ebx
    gui_load_listbox_clear_items_00419a90(listbox, nullptr);                                   // 006B3E37..006B3E39
    U32 extent[4];                                                                             // [esp+18h]
    std::memcpy(extent, at(self, 0x68), 16);                                                   // 006B3E3E..006B3E5A
    void* const text = at(self, 0x134);
    I32 height;
    if (static_cast<I32>(async_string_length_005e5790(desc_params, nullptr)) > 0)             // 006B3E54..006B3E6B
        height = static_cast<I32>(vcall<U32>(text, 0x08));                                     // 006B3E6D..006B3E6F
    else
        height = static_cast<I32>(gui_load_text_font_height_00414ee0(text, nullptr));          // 006B3E74
    extent[3] = static_cast<U32>(height);                                                      // 006B3E80
    void* label = at(self, 0x64);                                                              // 006B3E7D
    vcall<void, void*>(label, 0x04, extent);                                                   // 006B3E79..006B3E89 SetExtent
    void* controls[1] = {label};                                                               // [esp+4] (frame base)
    gui_m48_listbox_add_controls_0041c2c0(listbox, nullptr, controls, 1, 0, 0, 0);             // 006B3E8C..006B3E9F
    gui_load_listbox_set_active_control_0041c160(listbox, nullptr, label, 0);                  // 006B3EA4..006B3EA9
}

// 006B44D0 CSWGuiInGameInventory::CreateItemEntry(CSWCCreature* creature, int* index, CExoArrayList* list,
// CSWSItem* item, int equipped, int mark) RET 18h. Entries are cached in +1DC0h (created when *index reaches the
// cache size); the 27h handler and the entry's +58h "usable" flag are chosen from the item / party state.
void __fastcall kotor_fastcall_006b44d0(void* self, void*, void* creature, I32* index, void* list, void* item,
                                        U32 equipped, U32 mark) {
    void* entry;                                                                               // ebp
    const I32 slot = *index;                                                                   // 006B44E6..006B44EA
    if (slot == li(self, 0x1dc4)) {                                                            // 006B44F1..006B44F7
        entry = new_item_entry();                                                              // 006B44FD..006B4527
        list_add(at(self, 0x1dc0), entry);                                                     // 006B4527..006B4562
        void* prototype = vcall<void*>(lp(self, 0x830), 0x58);                                 // 006B4565..006B456D
        const U32 width = lw(self, 0x7f8) - (static_cast<U32>(lb(self, 0x824)) << 1);         // 006B4570..006B4584
        vcall<void, void*, void*, void*, U32, U32>(entry, 0xa0, at(prototype, 0x154), at(prototype, 0x6c),
                                                   at(prototype, 0xe0), width, 1);            // 006B4582..006B459A Initialize
        add_event(entry, 0, self, &kotor_fastcall_006b3d10);                                   // 006B45A0..006B45AA OnControlEntered
    } else {
        entry = lp(lp(self, 0x1dc0), static_cast<U32>(slot) * 4u);                             // 006B45B1..006B45B7
    }
    if (lb(self, 0x1de0) != 0xff && (lb(recovered_server_info_004aec90(server_app(), nullptr), 0x104) & 1u) != 0) { // 006B45BA..006B45D8
        add_event(entry, 0x27, self, &kotor_fastcall_006b27a0);                                // 006B45DA..006B4731 NotUseableOnHawk
        sw(entry, 0x58, 0);                                                                    // 006B4736
    } else if (kotor_fastcall_00616520(creature, nullptr, item, equipped, 0xff) == 0) {        // 006B45E4..006B4602
        if (lw(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 4) != 0)                 // 006B4795..006B47A3
            add_event(entry, 0x27, self, &kotor_fastcall_006b26e0);                            // 006B47A5..006B47AD CantEquip
        else
            add_event(entry, 0x27, self, &kotor_fastcall_006b2740);                            // 006B47B7..006B47BF NotUseable
        sw(entry, 0x58, 0);                                                                    // 006B47B2 / 006B47C4
    } else {
        const U32 type = lb(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0xac);      // 006B4608..006B4617 [esp+10h]
        bool selectable = true;                                                                // -> 006B477D
        if (type == 0x2d || type == 0x1a) {                                                    // 006B4614..006B4620 (medpac / stim)
            const std::int16_t hp = static_cast<std::int16_t>(current_hp(creature));           // 006B4622..006B462C [esp+34h]
            if (hp >= static_cast<std::int16_t>(maximum_hp(creature))) {                        // 006B4631..006B463B
                selectable = false;
                void* server_creature = kotor_m15a_creature_get_server_creature_0060fb20(creature, nullptr); // 006B463D..006B463F
                if (server_creature == nullptr || lw(server_creature, 0x9e0) == 0) {           // 006B4644..006B4650
                    add_event(entry, 0x27, self, &kotor_fastcall_006b2800);                    // 006B4656..006B4660 FullHealth
                    sw(entry, 0x58, 0);                                                        // 006B4665
                }
            }
        } else if (type == 0x2f) {                                                             // 006B4675..006B4678 (squad medpac)
            selectable = false;
            U32 hurt = 0;                                                                      // 006B4688 ebx
            {
                const std::int16_t hp = static_cast<std::int16_t>(current_hp(creature));       // 006B467E..006B4684
                if (hp < static_cast<std::int16_t>(maximum_hp(creature))) hurt = 1;            // 006B468A..006B4694
            }
            for (U32 member = 1; member <= 2; ++member) {                                      // 006B4699..006B4713
                void* character = frame_GetCharacter_006346c0(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr, member);
                if (character == nullptr) continue;
                if (kotor_m15a_creature_get_server_creature_0060fb20(character, nullptr) == nullptr) continue;
                const std::int16_t hp = static_cast<std::int16_t>(current_hp(character));
                if (hp < static_cast<std::int16_t>(maximum_hp(character))) hurt |= 1;
            }
            void* server_creature = kotor_m15a_creature_get_server_creature_0060fb20(creature, nullptr); // 006B4716..006B471A
            if (hurt == 0 || server_creature == nullptr) {                                     // 006B471F..006B4725
                add_event(entry, 0x27, self, &kotor_fastcall_006b2860);                        // 006B4727..006B4731 SquadFullHealth
                sw(entry, 0x58, 0);                                                            // 006B4736
            }
        }
        if (selectable) {
            add_event(entry, 0x27, self, &kotor_fastcall_006b25e0);                            // 006B477D..006B4787 OnControlSelected
            sw(entry, 0x58, 1);                                                                // 006B478C
        }
    }
    kotor_fastcall_006b6710(entry, nullptr, lw(item, 0x14), mark, (lw(item, 0x288) >> 7) & 1u); // 006B4741..006B4759 SetItem
    vcall<void, U32>(entry, 0x40, 0);                                                          // 006B475E..006B4765
    list_add(list, entry);                                                                     // 006B4768..006B47E9
    *index = *index + 1;                                                                       // 006B47E4..006B47F0
}

// 006B4810 CSWGuiInGameInventory::PopulateItemListBox() RET 0: gold, equipped items (14 slots), then the creature's
// repository, filtered by CheckFilter. Quirks kept: the server creature of the shown creature is not checked; the
// entry pointer table is a stack list freed at exit (entries stay cached in +1DC0h).
void __fastcall kotor_fastcall_006b4810(void* self, void*) {
    void* const creature = shown_creature(self);                                               // 006B482D..006B4899 ebx
    void* const server_creature = kotor_m15a_creature_get_server_creature_0060fb20(creature, nullptr); // 006B489B..006B48A8 [esp+28h]
    void* const repository = recovered_creature_inventory_004ef770(server_creature, nullptr, 1); // 006B48A4..006B48B3 [esp+1Ch]
    {
        Str gold;                                                                              // [esp+14h]
        kotor_fastcall_005e5bc0(gold.s, nullptr, w2x_creature_gold_004edd60(server_creature, nullptr)); gold.live = true; // 006B48B1..006B48C1
        gui_load_text_params_set_text_00415e00(at(self, 0x50c), nullptr, gold.p());            // 006B48C6..006B48D1
    }                                                                                          // 006B48D6..006B48E1
    PtrList entries;                                                                           // 006B48E6..006B48F2 [esp+2Ch]
    I32 count = 0;                                                                             // 006B48FA [esp+10h]
    I32 reselect = -1;                                                                         // 006B48FE [esp+20h]
    for (U32 slot = 0; slot < 0xe; ++slot) {                                                   // 006B4902..006B49C2 [esp+14h]
        const U32 mask = 1u << slot;                                                           // 006B4910..006B4919 edi
        void* client_item = kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr,
            kotor_m15a_creature_get_equipped_item_id_0060ca80(creature, nullptr, mask));       // 006B491B..006B492F
        if (client_item == nullptr) continue;                                                  // 006B4934..006B4936
        void* item = kotor_m15closure_00646c90(client_item, nullptr);                          // 006B4938..006B4945 ebp
        const U32 weapon_hand = (mask == 0x80 || mask == 0x100 || mask == 0x400) ? 1u : 0u;    // 006B493F..006B495D edi
        if (item == nullptr) continue;                                                         // 006B4962..006B4964
        if ((lw(item, 0x288) & 0x100u) != 0) continue;                                         // 006B4966..006B496F
        if (kotor_fastcall_006b2c90(self, nullptr, item, server_creature, weapon_hand) == 0) continue; // 006B4971..006B4981
        if (lw(self, 0x1dd8) == lw(item, 0x14)) { reselect = count; sw(self, 0x1dd8, 0x7f000000u); } // 006B4983..006B4996
        kotor_fastcall_006b44d0(self, nullptr, creature, &count, &entries, item, weapon_hand, 1); // 006B49A0..006B49B1
    }
    for (U32 i = 0; i < lw(repository, 0x10); ++i) {                                           // 006B49C8..006B49D3, 006B4A37..006B4A41
        void* item = w2x_inventory_item_005560b0(repository, nullptr, i);                      // 006B49D5..006B49DF
        if (item == nullptr) continue;                                                         // 006B49E1..006B49E3
        if ((lw(item, 0x288) & 0x100u) != 0) continue;                                         // 006B49E5..006B49EE
        if (kotor_fastcall_006b2c90(self, nullptr, item, server_creature, 0) == 0) continue;   // 006B49F0..006B4A01
        if (lw(self, 0x1dd8) == lw(item, 0x14)) { reselect = count; sw(self, 0x1dd8, 0x7f000000u); } // 006B4A03..006B4A16
        kotor_fastcall_006b44d0(self, nullptr, creature, &count, &entries, item, 0, 0);       // 006B4A20..006B4A32
    }
    void* const listbox = at(self, 0x564);                                                     // 006B4A4D edi
    kotor_fastcall_0041c1d0(listbox, nullptr, &entries, 0, 1, 0);                              // 006B4A43..006B4A56 AddControls
    const I32 size = entries.size;                                                             // 006B4A5B ebx
    if (size > 0) {                                                                            // 006B4A5F..006B4A61
        if ((lb(self, 0x1de4) & 2u) != 0) {                                                    // 006B4A67..006B4A6E
            const I32 rows = li(listbox, 0x2a0);                                               // 006B4A70
            const I32 visible = static_cast<std::int16_t>(lh(listbox, 0x2c4));                // 006B4A76
            const I32 saved = static_cast<std::int16_t>(lh(self, 0x1ddc));                    // 006B4A7D..006B4A92
            I32 top_limit = rows - visible; if (top_limit < 0) top_limit = 0;                  // 006B4A84..006B4A8E
            I32 top;
            if (saved < top_limit) top = saved;                                                // 006B4A95..006B4A99
            else { top = rows - visible; if (top < 0) top = 0; }                               // 006B4A9D..006B4AA7
            const U16 top16 = static_cast<U16>(top);
            std::memcpy(at(listbox, 0x2c8), &top16, 2);                                        // 006B4AA9
            I32 selected = static_cast<std::int16_t>(lh(self, 0x1dde));                       // 006B4AB0
            if (selected >= size - 1) selected = size - 1;                                     // 006B4AB7..006B4ABE
            gui_load_listbox_set_selected_0041c040(listbox, nullptr, selected, 0);             // 006B4AC0..006B4AC5
            sw(self, 0x1de4, lw(self, 0x1de4) & 0xFFFFFFFDu);                                  // 006B4ACA
        } else {
            gui_load_listbox_set_selected_0041c040(listbox, nullptr, reselect == -1 ? 0 : reselect, 0); // 006B4AD6..006B4AEA
        }
    } else {
        gui_load_text_params_set_color_00414e10(at(self, 0x1494), nullptr, kotor_m1_color_007a2408); // 006B4AF1..006B4AFC
        std::memcpy(at(self, 0x13b8), kotor_m1_color_007a2384, 12);                            // 006B4B01..006B4B22
        gui_load_listbox_set_selected_0041c040(listbox, nullptr, -1, 0);                       // 006B4B1C..006B4B25
        Str blank("");                                                                         // 006B4B2A..006B4B33 (0073D71C)
        gui_load_text_params_set_text_00415e00(at(self, 0x14c), nullptr, blank.p());           // 006B4B38..006B4B48
    }                                                                                          // 006B4B4D..006B4B56
    sw(self, 0x1de4, lw(self, 0x1de4) & 0xFFFFFFFEu);                                          // 006B4B5B
}                                                                                              // 006B4B62..006B4B6F _free

// 006B28C0 CSWGuiInGameInventory::UpdatePlayerStats() RET 0: portrait (+E14h), armour class (+110Ch), vitality
// (+FCCh; "%d/ %d" when the "%d/%d" text is taller than one line of +FB4h).
void __fastcall kotor_fastcall_006b28c0(void* self, void*) {
    void* creature = shown_creature(self);                                                     // 006B28DD..006B2948 esi
    if (creature == nullptr) return;                                                           // 006B294A..006B294C
    {
        menu_resources::Ref portrait;                                                          // [esp+1Ch]
        void* r = vcall<void*, void*, U32>(creature, 0x148, &portrait, 1);                     // 006B2952..006B295D GetPortrait
        gui_load_border_params_set_fill_00414c00(at(self, 0xe14), nullptr, r, 0);              // 006B2963..006B296C
    }
    {
        Str armour;                                                                            // [esp+14h]
        kotor_fastcall_005e5bc0(armour.s, nullptr,
            kotor_m15b_creature_stats_get_armor_class_00647720(lp(creature, 0x2f8), nullptr, 0)); armour.live = true; // 006B2971..006B2986
        gui_load_text_params_set_text_00415e00(at(self, 0x110c), nullptr, armour.p());         // 006B298B..006B299E
    }                                                                                          // 006B29A3..006B29AF
    Str vitality; vitality.empty();                                                            // 006B29B4..006B29B8 [esp+0Ch]
    {
        const I32 hp = static_cast<std::int16_t>(current_hp(creature));                       // 006B29BD..006B29C3
        const I32 max = static_cast<std::int16_t>(maximum_hp(creature));                      // 006B29CB..006B29D0
        frame_LogFormat_005e5680(vitality.s, "%d/%d", hp, max);                                // 006B29D3..006B29E2 (0075616C)
    }
    void* const vitality_text = at(self, 0xfcc);                                               // 006B29EE ebp
    gui_load_text_params_set_text_00415e00(vitality_text, nullptr, vitality.p());              // 006B29EA..006B29F7
    void* const label = at(self, 0xfb4);                                                       // 006B2A02 edi
    const I32 height = static_cast<I32>(vcall<U32>(label, 0x08));                              // 006B29FC..006B2A0A
    if (height > static_cast<I32>(gui_load_text_font_height_00414ee0(label, nullptr))) {        // 006B2A0D..006B2A18
        const I32 hp = static_cast<std::int16_t>(current_hp(creature));                       // 006B2A1A..006B2A20
        const I32 max = static_cast<std::int16_t>(maximum_hp(creature));                      // 006B2A24..006B2A29
        frame_LogFormat_005e5680(vitality.s, "%d/ %d", hp, max);                               // 006B2A2C..006B2A3B (0075645C)
        gui_load_text_params_set_text_00415e00(vitality_text, nullptr, vitality.p());          // 006B2A43..006B2A4A
    }
}                                                                                              // 006B2A4F..006B2A5B ~vitality

// 006B2A80 CSWGuiInGameInventory::SetNextFilter() RET 0: advance GetInGameGui()+BC1h (0..5), caption +28Ch =
// "<TLK 7DABh> - <current filter>", next-filter button +1658h = "<TLK A577h> <next filter>".
void __fastcall kotor_fastcall_006b2a80(void* self, void*) {
    U32 current = static_cast<U32>(lb(ingame_gui(), 0xbc1)) + 1u;                             // 006B2A9E..006B2AB7 ebx
    if (static_cast<I32>(current) >= 6) current = 0;                                           // 006B2AB8..006B2ABD
    sb(ingame_gui(), 0xbc1, static_cast<U8>(current));                                         // 006B2ABF..006B2AD3
    U32 next = current + 1u;                                                                   // 006B2ACD edi
    if (static_cast<I32>(next) >= 6) next = 0;                                                 // 006B2AD0..006B2ADB
    Str caption;                                                                               // [esp+10h]
    {
        Str space(" ");                                                                        // 006B2ADD..006B2AE6 (0073D5D8) [esp+18h]
        Str name, label, joined;                                                               // [esp+38h] / [esp+30h] / [esp+28h]
        void* next_name = tlk_string(name, filter_names[next]);                                // 006B2AEB..006B2B06
        void* prefix = tlk_string(label, 0xa577);                                              // 006B2B0D..006B2B22
        void* left = concat(prefix, joined, space.p());                                        // 006B2B27..006B2B38
        concat(left, caption, next_name);                                                      // 006B2B3D..006B2B4A
        joined.destroy(); label.destroy(); name.destroy();                                     // 006B2B4F..006B2B74
    }                                                                                          // 006B2B79..006B2B82 ~space
    gui_load_text_params_set_text_00415e00(at(self, 0x1658), nullptr, caption.p());            // 006B2B87..006B2B92
    {
        Str dash(" - ");                                                                       // 006B2B97..006B2BA0 (00748E98) [esp+20h]
        Str name, label, joined, title;                                                        // [esp+58h] / [esp+50h] / [esp+48h] / [esp+40h]
        void* current_name = tlk_string(name, filter_names[current]);                          // 006B2BA5..006B2BBD
        void* prefix = tlk_string(label, 0x7dab);                                              // 006B2BC4..006B2BD9
        void* left = concat(prefix, joined, dash.p());                                         // 006B2BDE..006B2BEF
        void* whole = concat(left, title, current_name);                                       // 006B2BF4..006B2C02
        async_assign_string_005e5c50(caption.s, nullptr, whole);                               // 006B2C07..006B2C11
        title.destroy(); joined.destroy(); label.destroy(); name.destroy();                    // 006B2C16..006B2C48
    }                                                                                          // 006B2C4D..006B2C56 ~dash
    gui_load_text_params_set_text_00415e00(at(self, 0x28c), nullptr, caption.p());             // 006B2C5B..006B2C66
}                                                                                              // 006B2C6B..006B2C77 ~caption

// 006B3ED0 CSWGuiInGameInventory::HandleInputEvent(int event, int down) RET 8. Index table 006B4004 (event - 28h),
// jump table 006B3FEC: 28h/2Dh/2Eh/DFh -> 006B3F88, 29h -> 006B3FC3, 39h -> 006B3F23, 3Ah -> 006B3F01, CEh -> 006B3F45.
void __fastcall kotor_fastcall_006b3ed0(void* self, void*, U32 event, U32 down) {
    if (down != 0) {                                                                           // 006B3ED7..006B3EDF
        switch (event) {
        case 0x28: case 0x2d: case 0x2e: case 0xdf:                                            // 006B3F88
            if (frame_R4_0062cba0(ingame_gui(), nullptr, 0) != 0)                              // 006B3F88..006B3FA1
                kotor_fastcall_005eda60(client_app(), nullptr, 0, 1);                          // 006B3FA3..006B3FAF
            break;
        case 0x29:                                                                             // 006B3FC3
            kotor_fastcall_006b2a80(self, nullptr);                                            // 006B3FC3..006B3FC5 SetNextFilter
            play_sound(self, 1);                                                               // 006B3FCA..006B3FCF
            sw(self, 0x1de4, lw(self, 0x1de4) | 1u);                                           // 006B3FD4
            break;
        case 0x39:                                                                             // 006B3F23
            vcall<void, U32, U32>(at(self, 0x844), 0x3c, 0x31, 1);                             // 006B3F23..006B3F33
            break;
        case 0x3a:                                                                             // 006B3F01
            vcall<void, U32, U32>(at(self, 0x844), 0x3c, 0x32, 1);                             // 006B3F01..006B3F11
            break;
        case 0xce:                                                                             // 006B3F45
            play_sound(self, 1);                                                               // 006B3F45..006B3F4A
            kotor_fastcall_005edf80(client_app(), nullptr, 0, 0, 1);                           // 006B3F4F..006B3F5E
            sw(self, 0x1de4, lw(self, 0x1de4) | 1u);                                           // 006B3F63..006B3F6E
            kotor_fastcall_006b2dc0(self, nullptr);                                            // 006B3F6C..006B3F74 UpdatePortraits
            break;
        default:
            break;
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, down);                          // 006B3FDB..006B3FDF (and copies)
}

// 006B4430 CSWGuiInGameInventory::OnPanelRemoved() RET 0. Quirk kept: CSWGuiPanel::OnPanelRemoved is not called.
void __fastcall kotor_fastcall_006b4430(void* self, void*) {
    gui_load_listbox_clear_items_00419a90(at(self, 0x564), nullptr);                           // 006B4435..006B443B
    for (I32 i = 0; i < li(self, 0x1dc4); ++i) {                                               // 006B4440..006B4475
        if (void* entry = lp(lp(self, 0x1dc0), static_cast<U32>(i) * 4u)) vcall<void*, U32>(entry, 0x00, 1);
        sw(lp(self, 0x1dc0), static_cast<U32>(i) * 4u, 0);
    }
    sw(self, 0x1dc4, 0);                                                                       // 006B447F
    if (lp(self, 0x1dc0) != nullptr) {                                                         // 006B4477..006B4485
        live_free(lp(self, 0x1dc0));                                                           // 006B4487..006B448D
        sw(self, 0x1dc0, 0); sw(self, 0x1dc8, 0);                                              // 006B4490..006B4496
    }
    sw(self, 0x1dc4, 0);                                                                       // 006B449C
    kotor_fastcall_00556050(recovered_party_repository_00563340(party_table(), nullptr), nullptr, at(self, 0x1dcc)); // 006B44A2..006B44C0
}

// 006B4BB0 CSWGuiInGameInventory::Draw(int) RET 4. Quirk kept: the player creature lookup is not checked.
void __fastcall kotor_fastcall_006b4bb0(void* self, void*, U32 arg) {
    if ((lb(self, 0x1de4) & 1u) != 0) kotor_fastcall_006b4810(self, nullptr);                  // 006B4BB3..006B4BBC PopulateItemListBox
    kotor_fastcall_006b28c0(self, nullptr);                                                    // 006B4BC1..006B4BC3 UpdatePlayerStats
    const void* color = kotor_m1_color_007a2408;                                               // 006B4C07
    if ((lb(self, 0x1de4) & 4u) != 0) {                                                        // 006B4BC8..006B4BCF
        void* server = server_app();                                                           // 006B4BD1..006B4BD7
        void* player = kotor_fastcall_004ae770(server, nullptr, kotor_fastcall_004aea40(server, nullptr)); // 006B4BDA..006B4BE4
        if (lw(player, 0xab0) == 0) color = borrowed_exit_color_007a23b4;                      // 006B4BE9..006B4BFA
    }
    gui_load_text_params_set_color_00414e10(at(self, 0x1494), nullptr, color);                 // 006B4BF2..006B4C0C
    kotor_panel_draw_0040B760(self, nullptr, arg);                                             // 006B4C11..006B4C18 CSWGuiPanel::Draw
}

// 006B4C30 CSWGuiInGameInventory::OnPanelAdded() RET 0.
void __fastcall kotor_fastcall_006b4c30(void* self, void*) {
    sb(self, 0x1de0, 0xff);                                                                    // 006B4C38
    sw(self, 0x1708, 0xFFFFFFFEu); sw(self, 0x18cc, 0xFFFFFFFEu);                              // 006B4C33..006B4C45
    kotor_fastcall_006b2dc0(self, nullptr);                                                    // 006B4C4B UpdatePortraits
    kotor_fastcall_006b4810(self, nullptr);                                                    // 006B4C50..006B4C52 PopulateItemListBox
    vcall<void, U32>(at(self, 0x1164), 0x0c, 0);                                               // 006B4C57..006B4C65
    gui_load_panel_on_added_0040b870(self, nullptr);                                           // 006B4C68..006B4C6A
    show_tutorial_window_005edf40(client_app(), nullptr, 0x14, 0x7f000000u, 0x7f000000u, 0);   // 006B4C6F..006B4C86
}

}  // extern "C"
