// Lane W4 GUI successor (worker "upgrade", fork B2): CSWGuiUpgrade (0x2FB8, table 00757298) event handlers and the
// upgrade bookkeeping they reach. Transcribed from the original swkotor.exe linear sweep (read-only); every statement
// carries its original PCs.
//   006C3C30 OnEnterSlot(CSWGuiControl*)            thiscall RET 4  (event 0 handler of the slot buttons)
//   006C4D30 CloseItems()                           thiscall RET
//   006C5370 OnControlEntered(CSWGuiControl*)       thiscall RET 4  (event 0 handler of the item entries)
//   006C5510 OnUpgradeSelected(CSWGuiControl*)      thiscall RET 4  (events 27h/2Dh of the item entries)
//   006C58C0 CreateItemEntry(ulong id, uchar slot)  thiscall RET 8  -> CSWUpgradeItemEntry*
//   006C59A0 InsertUpgrade(CSWSItem*, int slot)     thiscall RET 8
//   006C6120 VerifyUpgrade(CSWSItem*)               thiscall RET 4  (message box callback)
//   006C6190 OnAssemble(CSWGuiControl*)             thiscall RET 4  (BTN_ASSEMBLE)
//   006C61F0 CancelPanel()                          thiscall RET
//   006C6500 OnSlotSelected(CSWGuiControl*)         thiscall RET 4  (events 27h/2Dh of the slot buttons)
//   006C6A80 HandleInputEvent(int event, int down)  thiscall RET 8  (table slot +3C)
// Quirks kept: OnControlEntered reads the item's name without checking the GetItemByGameObjectID result;
// CreateItemEntry dereferences the entry even when the 3A4h allocation failed; VerifyUpgrade's "no" path clears the
// slot bit in the +294 upgrade mask of the callback item (the upgrade item), not of the working item that
// OnSlotSelected had set it on; InsertUpgrade's found-in-removed-list path leaves the item out of the added list.
#include "52a797cc9c-claude_upgrade.hpp"

namespace kotor_reconstruction { struct StartupString { int __thiscall BorrowedEquals005e5290(const char* text); }; } // CExoString::operator==(const char*)

extern "C" {
void* __fastcall recovered_party_repository_00563340(void* table, void*);                       // CSWPartyTable::GetPartyItems
w4::U32 __fastcall recovered_find_item_with_tag_00555ed0(void* repository, void*, void* tag);   // CItemRepository::FindItemWithTag
w4::U32 __fastcall recovered_inventory_insert_0055d330(void* repository, void*, void** item, w4::U32, w4::U32, w4::U32); // CItemRepository::AddItem
w4::U32 __fastcall recovered_remove_item_00555fd0(void* repository, void*, void* item);         // CItemRepository::RemoveItem
void* __fastcall kotor_fastcall_0055f280(void* item, void*, w4::U32 count);                     // CSWSItem::SplitItem
void* __fastcall recovered_item_construct_005530a0(void* item, void*, w4::U32 id);              // CSWSItem::CSWSItem
w4::U32 __fastcall recovered_item_template_005608b0(void* item, void*, void* resref);           // CSWSItem::LoadFromTemplate
void __fastcall recovered_item_owner_00553210(void* item, void*, w4::U32 owner, w4::U32, w4::U32, w4::U32); // CSWSItem::SetPossessor
void __fastcall recovered_item_set_identified_00554170(void* item, void*, w4::U32 flag);       // CSWSItem::SetIdentified
w4::U32 __fastcall recovered_item_can_equip_0051aa60(void* creature, void*, void* item, w4::U32* slot, w4::U32, w4::U32, w4::U32); // CSWSCreature::CanEquipItem
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void* item, void*) noexcept(false);    // CSWItem::GetBaseItem
void* __fastcall async_ref_construct_string_00406d60(void* ref, void*, void* string);         // CResRef::CResRef(const CExoString&)
w4::I32 __fastcall c2da_current_get_int_00413660(void* table, void*, w4::I32 row, void* column, w4::I32* out);   // C2DA::GetINTEntry
w4::I32 __fastcall c2da_current_get_string_00413270(void* table, void*, w4::I32 row, void* column, void* out);   // C2DA::GetCExoStringEntry
void* __fastcall lower_construct_005e6080(void* string, void*, void* out);                      // CExoString::LowerCase (by value)
w4::U32 __fastcall kotor_fastcall_005e5310(void* string, void*, void* other);                   // CExoString::operator!=
void* __fastcall kotor_fastcall_005e9f80(void* self, void*, const void* source);                // CExoLocString(const CExoLocString&)
void __fastcall exo_locstring_destroy_005ea000(void* self, void*);                              // ~CExoLocString
w4::U32 __fastcall exo_locstring_get_005ea130(void* loc, void*, w4::U32 language, void* out, w4::U32 gender); // CExoLocString::GetString
w4::U32 __fastcall kotor_fastcall_005ed4c0(void* app, void*);                                   // CClientExoApp::GetClientLanguage
w4::I32 __fastcall gui_load_int_list_index_of_006c2a40(void* list, void*, w4::U32 value);       // CExoArrayList<int>::indexOf
void __fastcall deleteAt_006857a0(void* list, void*, int index);                                 // CExoArrayList::deleteAt
void __fastcall gui_load_array_add_00671c00(void* array, void*, w4::U32 value);                 // CExoArrayList<ptr>::Add
}

using namespace gchar;

namespace {
inline void* server() { return server_app(); }
inline void* party_items(void* srv) { return recovered_party_repository_00563340(lane_config_required::GetPartyTable_004aee70(srv, nullptr), nullptr); }
inline void* working(void* self) { return lp(self, 0x2F54); }                                   // working item copy
inline void** slot_item(void* self, U32 slot) { return static_cast<void**>(at(self, 0x2F74 + slot * 4u)); }
inline U8 slot_row(void* self, U32 slot) { return lb(self, 0x2FA4 + slot); }
inline void* slot_tag(void* self, U32 slot) { return at(self, 0x2F84 + slot * 8u); }
inline U8 type_of(void* self) { return lb(self, 0x2F4C); }
inline void clear_mask(void* item, U8 bit) { sw(item, 0x294, lw(item, 0x294) & ~(1u << (bit & 31u))); }
inline void set_mask(void* item, U8 bit) { sw(item, 0x294, lw(item, 0x294) | (1u << (bit & 31u))); }
inline void set_active(void* self, void* control) { vcall<void, void*, U32>(self, 0x08, control, 0); } // [vtbl+8] SetActiveControl
// Shared panel close: [+44] &= ~100h; then unless (flags & 600h) == 400h: flags = (flags & ~400h) | 200h.
inline void close_flags(void* panel) {
    const U32 f = lw(panel, 0x44) & 0xFFFFFEFFu;
    sw(panel, 0x44, f);
    if ((f & 0x600u) != 0x400u) sw(panel, 0x44, (f & 0xFFFFFBFFu) | 0x200u);
}
// Inlined CExoArrayList<CSWSItem*>::Add on the embedded list at +off (grow to capacity*2, 10h when empty).
inline void embedded_add(void* self, U32 off, void* value) {
    void* list = at(self, off);
    if (lw(list, 4) == lw(list, 8))
        menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(list), nullptr,
                                            lw(list, 8) != 0 ? lw(list, 8) * 2u : 0x10u);
    static_cast<void**>(lp(list, 0))[lw(list, 4)] = value;
    sw(list, 4, lw(list, 4) + 1u);
}
// CExoLocString (8 bytes) on the stack.
struct Loc {
    U32 s[2];
    explicit Loc(const void* source) { kotor_fastcall_005e9f80(s, nullptr, source); }
    Loc(const Loc&) = delete; Loc& operator=(const Loc&) = delete;
    ~Loc() { exo_locstring_destroy_005ea000(s, nullptr); }
};
}  // namespace

extern "C" {
// 006C3C30 CSWGuiUpgrade::OnEnterSlot(CSWGuiControl* control) RET 4.
void __fastcall kotor_fastcall_006c3c30(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                         // 006C3C50..006C3C57
    const U32 slot = lw(control, 0x58);                                                         // 006C3C5E
    const U8 type = type_of(self);                                                              // 006C3C61
    const U32 strref_id = upgrade::slot_info(type, slot).strref;                                // 006C3C69..006C3C7A [00756FB8]
    if (type == 1) {                                                                            // 006C3C67 / 006C3C7E
        if (*slot_item(self, slot) != nullptr) {                                                // 006C3C84..006C3C8B
            Str name; name.empty();                                                             // 006C3C8D..006C3C91 [esp+0Ch]
            Loc loc(at(*slot_item(self, slot), 0x280));                                         // 006C3C96..006C3CAB (state 0)
            exo_locstring_get_005ea130(loc.s, nullptr, kotor_fastcall_005ed4c0(client_app(), nullptr), name.p(), 0); // 006C3CB0..006C3CCE
            gui_load_text_params_set_text_00415e00(at(self, 0x27C8), nullptr, name.p());        // 006C3CD3..006C3CDE
        }                                                                                       // 006C3CE3..006C3CFC ~loc ~name
        else {
            gui_load_text_params_set_strref_00415e50(at(self, 0x27C8), nullptr, strref_id);    // 006C3D01..006C3D08
        }
        return;
    }
    void* srv = server();                                                                       // 006C3D21..006C3D28 ebp
    void* tag = slot_tag(self, slot);                                                           // 006C3D2B (pushed before GetPartyTable)
    const U32 id = recovered_find_item_with_tag_00555ed0(party_items(srv), nullptr, tag);       // 006C3D32..006C3D43
    void* item = recovered_item_lookup_004ae760(srv, nullptr, id);                              // 006C3D48..006C3D4B
    I32 count = 0;                                                                              // ebx = 0 (006C3C53)
    if (item != nullptr) count = li(item, 0x28C);                                               // 006C3D50..006C3D55
    gui_load_text_params_set_strref_00415e50(at(self, 0x2688), nullptr, strref_id);            // 006C3D5B..006C3D66 LBL_SLOTNAME
    {
        Str n(count);                                                                           // 006C3D6B..006C3D70 CExoString(int)
        gui_load_text_params_set_text_00415e00(at(self, 0x2A48), nullptr, n.p());              // 006C3D75..006C3D88 LBL_UPGRADE_COUNT
    }                                                                                           // 006C3D8D..006C3D98
    gui_load_text_params_set_strref_00415e50(at(self, 0x2908), nullptr, 0xA42Au);               // 006C3D9D..006C3DA8 LBL_UPGRADES
    {
        Str keyed;                                                                              // [esp+1Ch]
        void* r = kotor_fastcall_0055f510(working(self), nullptr, keyed.s, slot_row(self, slot)); // 006C3DAD..006C3DC2
        keyed.live = true;
        gui_load_text_params_set_text_00415e00(at(self, 0x2548), nullptr, r);                   // 006C3DC7..006C3DD6 LBL_PROPERTY
    }                                                                                           // 006C3DDB..006C3DE3
}

// 006C4D30 CSWGuiUpgrade::CloseItems() RET.
void __fastcall kotor_fastcall_006c4d30(void* self, void*) {
    kotor_fastcall_006c2f80(self, nullptr, 0);                                                  // 006C4D31..006C4D35 ShowItems(0)
    void* selected = lp(self, 0x2FB0);                                                          // 006C4D40
    sw(self, 0x2F48, lw(self, 0x2F48) & 0xFFFFFFFEu);                                           // 006C4D3A..006C4D50
    set_active(self, selected);                                                                 // 006C4D46..006C4D56
    kotor_fastcall_006c3aa0(self, nullptr);                                                     // 006C4D59..006C4D5B UpdateItems
    kotor_fastcall_006c3c30(self, nullptr, lp(self, 0x1C));                                     // 006C4D60..006C4D66 OnEnterSlot([+1C])
}

// 006C5370 CSWGuiUpgrade::OnControlEntered(CSWGuiControl* control) RET 4.
void __fastcall kotor_fastcall_006c5370(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                         // 006C5392..006C539B
    const U32 id = lw(control, 0x1C4);                                                          // 006C53A1
    if (id == 0x7F000000u) {                                                                    // 006C53A7..006C53AC
        Str empty(""); kotor_fastcall_006c32e0(self, nullptr, empty.p());                       // 006C54C7..006C54E4 (0073D71C)
        return;                                                                                 // 006C54E9..006C54F5
    }
    void* item = recovered_item_lookup_004ae760(server(), nullptr, id);                         // 006C53B2..006C53BC (unchecked)
    Str name; name.empty();                                                                     // 006C53C1..006C53C7 [esp+18h] state 0
    exo_locstring_get_005ea130(at(item, 0x270), nullptr, kotor_fastcall_005ed4c0(client_app(), nullptr), name.p(), 0); // 006C53CC..006C53F2
    if (static_cast<kotor_reconstruction::StartupString*>(name.p())->BorrowedEquals005e5290("") != 0) { // 006C53F7..006C5407 (0073D71C)
        Str g; gui_string(g, 0x7DAC);                                                           // 006C5409..006C541C
        async_assign_string_005e5c50(name.p(), nullptr, g.p());                                 // 006C5421..006C542B
    }                                                                                           // 006C5430..006C5439
    if (type_of(self) == 1 && lw(lp(self, 0x2FB0), 0x58) != 1) {                                // 006C543E..006C5453
        const U8 key = lb(control, 0x1D4);                                                      // 006C5455..006C545F
        Str keyed;                                                                              // [esp+20h]
        void* k = kotor_fastcall_0055f510(working(self), nullptr, keyed.s, key);                // 006C5463..006C546F
        keyed.live = true;                                                                      // state 2
        {
            Str sum;                                                                            // [esp+10h]
            GuiConcat005e5d10(static_cast<kotor_reconstruction::StartupString*>(k), nullptr, sum.ss(), name.ss()); // 006C5474..006C5486
            sum.live = true;                                                                    // state 3
            async_assign_string_005e5c50(name.p(), nullptr, sum.p());                           // 006C548B..006C5495
        }                                                                                       // 006C549A..006C54A2
    }                                                                                           // 006C54A7..006C54B0 ~keyed
    kotor_fastcall_006c32e0(self, nullptr, name.p());                                           // 006C54B5..006C54BC SetDescription
}                                                                                               // 006C54C1..006C54F5 ~name

// 006C5510 CSWGuiUpgrade::OnUpgradeSelected(CSWGuiControl* control) RET 4.
void __fastcall kotor_fastcall_006c5510(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                         // 006C552E..006C5535
    if (lw(control, 0x48) == 0x2D) kotor_fastcall_0040a140(manager_of(self), nullptr, 0);       // 006C553B..006C5546
    void* srv = server();                                                                       // 006C554B..006C5551 ebx
    void* repo = party_items(srv);                                                              // 006C5555..006C5569 [esp+10h]
    const U32 slot = lw(lp(self, 0x2FB0), 0x58);                                                // 006C5563..006C5573 [esp+14h]
    bool apply = false;
    if (slot == 1) {                                                                            // 006C5570..006C5577
        if (lb(control, 0x1D4) != 0xFF) {                                                       // 006C557D..006C5584
            Str tmpl; tmpl.empty();                                                             // 006C558A..006C558E [esp+24h] state 0
            Str column; column.empty();                                                         // 006C5593..006C559F [esp+1Ch] state 1
            const U8 kind = lb(kotor_m15b_item_get_base_item_005b4790(working(self), nullptr), 0xAC); // 006C55A4..006C55B4
            const char* col = nullptr;
            if (kind == 0x27) col = "DoubleMdlVar";                                             // 006C55BB..006C55BE / 006C55D4 (00757310)
            else if (kind == 0x28) col = "ShortMdlVar";                                         // 006C55C0..006C55C1 / 006C55CD (00757320)
            else if (kind == 0x29) col = "LongMdlVar";                                          // 006C55C3..006C55C4 / 006C55C6 (0075732C)
            if (col != nullptr) GuiAssign005e5140(column.p(), nullptr, col);                    // 006C55D9..006C55DD
            c2da_current_get_string_00413270(lp(self, 0x2FAC), nullptr, lb(control, 0x1D4), column.p(), tmpl.p()); // 006C55E2..006C55FC
            void* raw = live_operator_new(0x29C);                                               // 006C5601..006C560B
            void* item = nullptr;
            if (raw != nullptr) {                                                               // 006C5612..006C5619
                auto g = on_throw([&]() { live_free(raw); });                                   // state 2
                item = recovered_item_construct_005530a0(raw, nullptr, 0x7F000000u);            // 006C561B..006C5627
                g.armed = false;
            }
            menu_resources::Ref ref;                                                            // [esp+2Ch]
            async_ref_construct_string_00406d60(&ref, nullptr, tmpl.p());                       // 006C562D..006C563B
            recovered_item_template_005608b0(item, nullptr, &ref);                              // 006C5640..006C5647
            recovered_item_owner_00553210(item, nullptr, kotor_fastcall_004aea40(srv, nullptr), 0, 0, 0x7F000000u); // 006C564C..006C565F
            recovered_item_set_identified_00554170(item, nullptr, 1);                           // 006C5664..006C5668
            sw(item, 0x294, lw(working(self), 0x294));                                          // 006C566D..006C567F
            const U32 flags = lw(item, 0x288);                                                  // 006C5679
            sw(item, 0x288, flags ^ ((((lw(working(self), 0x288) >> 5) & 1u) << 5 ^ flags) & 0x20u)); // 006C5685..006C56A3
            void* old = working(self);                                                          // 006C56A9
            if (old != nullptr) vcall<void*, U32>(old, 0x00, 1);                                // 006C56AF..006C56B7 deleting dtor
            sw(self, 0x2F54, ad(item));                                                         // 006C56BB
            kotor_fastcall_006c3630(self, nullptr);                                             // 006C56B9..006C56C1 Update3DView
            apply = true;
        }                                                                                       // 006C56C6..006C56E0 ~column ~tmpl
    } else if (lb(control, 0x1D4) != 0xFF || lw(control, 0x1C4) == 0x7F000000u) {               // 006C56EF..006C5702
        apply = true;
    }
    if (apply) {
        void** entry = slot_item(self, slot);                                                   // 006C5708 ebp
        if (*entry != nullptr) {                                                                // 006C570F..006C5713
            recovered_item_owner_00553210(*entry, nullptr, kotor_fastcall_004aea40(srv, nullptr), 0, 0, 0x7F000000u); // 006C5719..006C572D
            const I32 index = gui_load_int_list_index_of_006c2a40(at(self, 0x2F68), nullptr, ad(*entry)); // 006C5732..006C573E
            if (index != -1) {                                                                  // 006C5743..006C5746
                deleteAt_006857a0(at(self, 0x2F68), nullptr, index);                            // 006C5748..006C574B
                recovered_inventory_insert_0055d330(repo, nullptr, entry, 1, 0, 0);             // 006C5750..006C575B
            } else {
                recovered_inventory_insert_0055d330(repo, nullptr, entry, 1, 0, 0);             // 006C5762..006C576D
                gui_load_array_add_00671c00(at(self, 0x2F5C), nullptr, ad(*entry));             // 006C5772..006C577C
            }
            *entry = nullptr;                                                                   // 006C578C
            if (slot != 1) clear_mask(working(self), slot_row(self, slot));                     // 006C5781..006C57B3
        }
        void* item = recovered_item_lookup_004ae760(srv, nullptr, lw(control, 0x1C4));          // 006C57B9..006C57C2
        if (item != nullptr) {                                                                  // 006C57C7..006C57CB
            const I32 index = gui_load_int_list_index_of_006c2a40(at(self, 0x2F5C), nullptr, ad(item)); // 006C57D1..006C57DA
            if (index != -1) {                                                                  // 006C57DF..006C57E2
                deleteAt_006857a0(at(self, 0x2F5C), nullptr, index);                            // 006C57E4..006C57E7
                if (li(item, 0x28C) > 1) item = kotor_fastcall_0055f280(item, nullptr, 1);       // 006C57EC..006C57FE
                else recovered_remove_item_00555fd0(repo, nullptr, item);                       // 006C5802..006C5807
            } else {
                if (li(item, 0x28C) > 1) item = kotor_fastcall_0055f280(item, nullptr, 1);       // 006C580E..006C5820
                else recovered_remove_item_00555fd0(repo, nullptr, item);                       // 006C5824..006C5829
                gui_load_array_add_00671c00(at(self, 0x2F68), nullptr, ad(item));               // 006C582E..006C5835
            }
            *entry = item;                                                                      // 006C5841
            if (slot != 1) {                                                                    // 006C583A..006C5844
                const U8 row = lb(control, 0x1D4);                                              // 006C5846..006C584A
                sb(self, 0x2FA4 + slot, row);                                                   // 006C5850
                set_mask(working(self), row);                                                   // 006C5857..006C5864
            }
        }
    }
    kotor_fastcall_006c2f80(self, nullptr, 0);                                                  // 006C586A..006C586E ShowItems(0)
    void* selected = lp(self, 0x2FB0);                                                          // 006C5879
    sw(self, 0x2F48, lw(self, 0x2F48) & 0xFFFFFFFEu);                                           // 006C5873..006C5889
    set_active(self, selected);                                                                 // 006C587F..006C588F
    kotor_fastcall_006c3aa0(self, nullptr);                                                     // 006C5892..006C5894 UpdateItems
    kotor_fastcall_006c3c30(self, nullptr, lp(self, 0x1C));                                     // 006C5899..006C589F OnEnterSlot([+1C])
}

// 006C58C0 CSWGuiUpgrade::CreateItemEntry(ulong id, uchar slot) RET 8.
void* __fastcall kotor_fastcall_006c58c0(void* self, void*, w4::U32 item_id, w4::U32 slot) {
    void* listbox_proto = lp(self, 0x184C);                                                     // 006C58E9 ebx (LB_DESC +2CCh)
    const I32 width = li(self, 0x1814) - static_cast<I32>(lb(self, 0x1840)) * 2;                // 006C58DC..006C58F6 ebp
    void* raw = live_operator_new(0x3A4);                                                       // 006C58F1..006C58FD
    void* entry = nullptr;
    if (raw != nullptr) {                                                                       // 006C5904..006C590C
        auto g = on_throw([&]() { live_free(raw); });                                           // state 0
        entry = kotor_fastcall_006c3e00(raw, nullptr);                                          // 006C590E..006C5915
        g.armed = false;
    }
    vcall<void, void*, void*, void*, I32>(entry, 0xA0, at(listbox_proto, 0x154), at(listbox_proto, 0x6C),
                                          at(listbox_proto, 0xE0), width);                      // 006C5917..006C5936 Initialize
    gui_load_control_add_event_0041ab20(entry, nullptr, 0, self, fn(&kotor_fastcall_006c5370));    // 006C593C..006C5946
    gui_load_control_add_event_0041ab20(entry, nullptr, 0x27, self, fn(&kotor_fastcall_006c5510)); // 006C594B..006C5955
    gui_load_control_add_event_0041ab20(entry, nullptr, 0x2D, self, fn(&kotor_fastcall_006c5510)); // 006C595A..006C5964
    kotor_fastcall_006c3ea0(entry, nullptr, item_id, 0, 0x7F000000u, 0);                        // 006C5969..006C5979 SetItem
    sb(entry, 0x1D4, static_cast<U8>(slot));                                                    // 006C597E..006C5986
    return entry;
}

// 006C59A0 CSWGuiUpgrade::InsertUpgrade(CSWSItem* item, int slot) RET 8.
void __fastcall kotor_fastcall_006c59a0(void* self, void*, void* item, w4::U32 slot) {
    if (item == nullptr) return;                                                                // 006C59A2..006C59AA
    void* removed = at(self, 0x2F5C);                                                           // 006C59B6 ecx
    const I32 size = li(self, 0x2F60);                                                          // 006C59B0 edx
    I32 index = 0;                                                                              // 006C59BC
    bool found = false;
    if (size > 0) {                                                                             // 006C59BE..006C59C1
        void** data = static_cast<void**>(lp(removed, 0));                                      // 006C59C3
        for (; index < size; ++index) if (data[index] == item) { found = true; break; }         // 006C59C5..006C59CF
    }
    if (found && index != -1) {                                                                 // 006C59E7..006C59EA
        deleteAt_006857a0(removed, nullptr, index);                                             // 006C59EC..006C59ED
        if (li(item, 0x28C) > 1) item = kotor_fastcall_0055f280(item, nullptr, 1);               // 006C59F2..006C5A09
        else recovered_remove_item_00555fd0(party_items(server()), nullptr, item);              // 006C5A17..006C5A2E
        *slot_item(self, slot) = item;                                                          // 006C5A0B / 006C5A38
        return;
    }
    if (li(item, 0x28C) > 1) item = kotor_fastcall_0055f280(item, nullptr, 1);                  // 006C59D1..006C59E3
    else recovered_remove_item_00555fd0(party_items(server()), nullptr, item);                  // 006C5A44..006C5A5C
    embedded_add(self, 0x2F68, item);                                                           // 006C5A61..006C5A98
    *slot_item(self, slot) = item;                                                              // 006C5A9C
}

// 006C6120 CSWGuiUpgrade::VerifyUpgrade(CSWSItem* item) RET 4 (message box callback).
void __fastcall kotor_fastcall_006c6120(void* self, void*, void* item) {
    const U32 slot = lw(lp(self, 0x1C), 0x58);                                                  // 006C6129..006C6130
    void* box = lp(ingame_gui(), 0x98);                                                         // 006C6123..006C6138
    if ((lb(box, 0x64) & 1u) != 0) {                                                            // 006C613E..006C6146
        kotor_fastcall_006c59a0(self, nullptr, item, slot);                                     // 006C6148..006C614C InsertUpgrade
        kotor_fastcall_006c3aa0(self, nullptr);                                                 // 006C6151..006C6153 UpdateItems
        kotor_fastcall_006c3c30(self, nullptr, lp(self, 0x1C));                                 // 006C6158..006C615E OnEnterSlot
        return;
    }
    clear_mask(item, slot_row(self, slot));                                                     // 006C6168..006C6182 (callback item's mask)
}

// 006C6190 CSWGuiUpgrade::OnAssemble(CSWGuiControl* control) RET 4.
void __fastcall kotor_fastcall_006c6190(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                         // 006C6197..006C619C
    if (lw(control, 0x48) == 0x2D)                                                              // 006C619E..006C61A2
        kotor_fastcall_0040a140(manager_of(self), nullptr, static_cast<U32>(static_cast<I32>(static_cast<std::int8_t>(lb(control, 0x54))))); // 006C61A4..006C61AC
    kotor_fastcall_006c5e90(lp(self, 0x2FB4), nullptr);                                         // 006C61B1..006C61B7 FinishUpgrading
    close_flags(self);                                                                          // 006C61BC..006C61E1
    kotor_fastcall_0040be00(manager_of(self), nullptr);                                         // 006C61E4..006C61E7 PopModalPanel
}

// 006C61F0 CSWGuiUpgrade::CancelPanel() RET.
void __fastcall kotor_fastcall_006c61f0(void* self, void*) {
    void* current = working(self);                                                              // 006C61F4
    if (current != nullptr) vcall<void*, U32>(current, 0x00, 1);                                // 006C61FD..006C6205
    sw(self, 0x2F54, lw(self, 0x2F58));                                                         // 006C6207..006C620D restore the original
    sw(self, 0x2F58, 0);                                                                        // 006C6213
    void* repo = party_items(server());                                                         // 006C6219..006C6230 ebp
    void* local = nullptr;                                                                      // [esp+10h] (entry push slot)
    for (I32 k = 0; k < li(self, 0x2F6C); ++k) {                                                // 006C6232..006C6286
        local = static_cast<void**>(lp(self, 0x2F68))[k];                                       // 006C6240..006C6249
        for (U32 s = 0; s < 4; ++s)                                                             // 006C624D..006C625C
            if (*slot_item(self, s) == local) { *slot_item(self, s) = nullptr; break; }         // 006C6251 / 006C6260
        recovered_inventory_insert_0055d330(repo, nullptr, &local, 1, 0, 0);                    // 006C626B..006C6278 AddItem
    }
    for (I32 k = 0; k < li(self, 0x2F60); ++k) {                                                // 006C6288..006C62E0
        void* item = static_cast<void**>(lp(self, 0x2F5C))[k];                                  // 006C62A0..006C62A9
        local = item;
        const I32 count = li(item, 0x28C);                                                      // 006C62AD
        if (count > 1) { sw(item, 0x28C, static_cast<U32>(count - 1)); continue; }              // 006C62B3..006C62B9
        recovered_remove_item_00555fd0(repo, nullptr, item);                                    // 006C62C1..006C62C4
        if (local != nullptr) vcall<void*, U32>(local, 0x00, 1);                                // 006C62C9..006C62D5 deleting dtor
    }
    kotor_fastcall_006c5e90(lp(self, 0x2FB4), nullptr);                                         // 006C62E2..006C62E8 FinishUpgrading
    close_flags(self);                                                                          // 006C62ED..006C6317
    kotor_fastcall_0040be00(manager_of(self), nullptr);                                         // 006C631A..006C631D PopModalPanel
}

// 006C6500 CSWGuiUpgrade::OnSlotSelected(CSWGuiControl* control) RET 4.
void __fastcall kotor_fastcall_006c6500(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                         // 006C6522..006C652B
    if (lw(control, 0x48) == 0x2D) kotor_fastcall_0040a140(manager_of(self), nullptr, 0);       // 006C6531..006C653C
    const U32 slot = lw(control, 0x58);                                                         // 006C6546..006C654C [esp+1Ch]
    void* repo = party_items(server());                                                         // 006C6541..006C6563 [esp+10h]
    if (type_of(self) == 1) {                                                                   // 006C655C..006C6567
        PtrList entries;                                                                        // [esp+44h] state 0
        if (slot == 1) {                                                                        // 006C657B..006C6582
            Str tag; tag.empty();                                                               // 006C6588..006C658C [esp+1Ch] state 1
            gui_load_array_add_00671c00(&entries, nullptr,
                ad(kotor_fastcall_006c58c0(self, nullptr, lw(lp(self, 0x2F78), 0x14), 0xFFFFFFFFu))); // 006C6591..006C65AE
            for (I32 row = 0; row < li(lp(self, 0x2FAC), 0x24); ++row) {                        // 006C65B3..006C667C
                { Str column("Template");                                                       // 006C65D0..006C65D9 (00757304) state 2
                  c2da_current_get_string_00413270(lp(self, 0x2FAC), nullptr, row, column.p(), tag.p()); } // 006C65DE..006C6602
                { Str low; lower_construct_005e6080(tag.p(), nullptr, low.s); low.live = true;  // 006C6607..006C6610 state 3
                  async_assign_string_005e5c50(tag.p(), nullptr, low.p()); }                    // 006C6615..006C662C
                if (kotor_fastcall_005e5310(tag.p(), nullptr, at(lp(self, 0x2F78), 0x28)) != 0) { // 006C6631..006C6646
                    const U32 id = recovered_find_item_with_tag_00555ed0(repo, nullptr, tag.p()); // 006C6648..006C6651
                    if (id != 0x7F000000u)                                                      // 006C6656..006C665B
                        gui_load_array_add_00671c00(&entries, nullptr,
                            ad(kotor_fastcall_006c58c0(self, nullptr, id, static_cast<U32>(row)))); // 006C665D..006C666B
                }
            }
        } else {                                                                                // 006C6698
            Str tag; tag.empty();                                                               // 006C6698..006C669C [esp+24h] state 4
            const U32 other = slot != 0 ? 0u : 2u;                                              // 006C66A1..006C66BA [esp+18h]
            gui_load_array_add_00671c00(&entries, nullptr,
                ad(kotor_fastcall_006c58c0(self, nullptr, 0x7F000000u, 0xFFFFFFFFu)));         // 006C66A8..006C66C8
            if (*slot_item(self, slot) != nullptr)                                              // 006C66CD..006C66D6
                gui_load_array_add_00671c00(&entries, nullptr,
                    ad(kotor_fastcall_006c58c0(self, nullptr, lw(*slot_item(self, slot), 0x14), slot_row(self, slot)))); // 006C66D8..006C66F1
            for (I32 row = 0; row < li(lp(self, 0x2FA8), 0x24); ++row) {                        // 006C66F6..006C6814
                I32 kind = 0;                                                                   // [esp+14h]
                { Str column("UpgradeType");                                                    // 006C6710..006C6719 (00748DB8) state 5
                  c2da_current_get_int_00413660(lp(self, 0x2FA8), nullptr, row, column.p(), &kind); } // 006C671E..006C6742
                if (kind != 0) continue;                                                        // 006C6747..006C674D
                { Str column("Template");                                                       // 006C6753..006C675C (00757304) state 6
                  c2da_current_get_string_00413270(lp(self, 0x2FA8), nullptr, row, column.p(), tag.p()); } // 006C6761..006C6785
                U32 id;
                { Str low; lower_construct_005e6080(tag.p(), nullptr, low.s); low.live = true;  // 006C678A..006C6793 state 7
                  id = recovered_find_item_with_tag_00555ed0(repo, nullptr, low.p()); }         // 006C6798..006C67B2
                if (id == 0x7F000000u) continue;                                                // 006C67B7..006C67BD
                if (*slot_item(self, other) != nullptr && static_cast<U32>(row) == slot_row(self, other)) continue; // 006C67BF..006C67D8
                if (*slot_item(self, slot) != nullptr && static_cast<U32>(row) == slot_row(self, slot)) continue;   // 006C67DA..006C67F3
                gui_load_array_add_00671c00(&entries, nullptr,
                    ad(kotor_fastcall_006c58c0(self, nullptr, id, static_cast<U32>(row))));     // 006C67F5..006C6803
            }
        }                                                                                       // ~tag (006C6682..006C668B / 006C681D..006C6826)
        void* listbox = at(self, 0x1580);                                                       // 006C6835 LB_ITEMS
        sw(self, 0x2FB0, ad(control));                                                          // 006C683E
        kotor_fastcall_0041c1d0(listbox, nullptr, &entries, 1, 0, 0);                           // 006C682B..006C6844 AddControls
        kotor_fastcall_006c2f80(self, nullptr, 1);                                              // 006C6849..006C684D ShowItems(1)
        set_active(self, listbox);                                                              // 006C6852..006C6859
        sw(self, 0x2F48, lw(self, 0x2F48) | 1u);                                                // 006C685C..006C686A
        return;                                                                                 // 006C6870..006C6878 _free(entries.data)
    }
    void** entry = slot_item(self, slot);                                                       // 006C6894..006C689D ebx
    if (*entry != nullptr) {                                                                    // 006C689B..006C68A4
        clear_mask(working(self), slot_row(self, slot));                                        // 006C68AA..006C68C8
        const I32 index = gui_load_int_list_index_of_006c2a40(at(self, 0x2F68), nullptr, ad(*entry)); // 006C68CE..006C68D9
        if (index != -1) {                                                                      // 006C68DE..006C68E1
            deleteAt_006857a0(at(self, 0x2F68), nullptr, index);                                // 006C68E3..006C68E6
            recovered_inventory_insert_0055d330(repo, nullptr, entry, 1, 0, 0);                 // 006C68EB..006C68F6
        } else {
            recovered_inventory_insert_0055d330(repo, nullptr, entry, 1, 0, 0);                 // 006C6906..006C6911
            gui_load_array_add_00671c00(at(self, 0x2F5C), nullptr, ad(*entry));                 // 006C6916..006C691F
        }
        *entry = nullptr;                                                                       // 006C68FB / 006C6924
    } else {
        const U32 id = recovered_find_item_with_tag_00555ed0(repo, nullptr, slot_tag(self, slot)); // 006C692F..006C6939
        void* item = recovered_item_lookup_004ae760(server(), nullptr, id);                     // 006C693E..006C6948 [esp+14h]
        if (item == nullptr) return;                                                            // 006C694D..006C6953
        void* select = lp(self, 0x2FB4);                                                        // 006C6959 ebx
        U32 equip_slot = 0;                                                                     // 006C695F..006C6961 [esp+10h]
        void* creature = nullptr;                                                               // eax = 0
        void* selected = lp(select, 0xC3C);                                                     // 006C6965
        if (selected != nullptr && (lw(selected, 0x1D0) & 1u) != 0) {                           // 006C696B..006C697A
            equip_slot = kotor_fastcall_006c2a90(select, nullptr);                              // 006C697C..006C6989
            void* srv = server();                                                               // 006C6983..006C6992 [esp+1Ch]
            creature = kotor_fastcall_004ae770(srv, nullptr, kotor_fastcall_006c2a70(select, nullptr)); // 006C6990..006C69A0
        }
        set_mask(working(self), slot_row(self, slot));                                          // 006C69A5..006C69C3
        if (creature != nullptr &&                                                              // 006C69C1..006C69C9
            static_cast<U8>(recovered_item_can_equip_0051aa60(creature, nullptr, working(self), &equip_slot, 0, 0, 0)) == 0) { // 006C69CB..006C69E6
            void* box = lp(ingame_gui(), 0x98);                                                 // 006C69E8..006C69F5
            kotor_fastcall_00627130(box, nullptr, 1);                                           // 006C69FB..006C69FF SetAllowCancel(1)
            gui_load_messagebox_set_callback_00624a40(box, nullptr, self, ad(fn(&kotor_fastcall_006c6120)), ad(item)); // 006C6A04..006C6A11
            vcall<void, U32>(box, 0x6C, 0xA5F9u);                                               // 006C6A16..006C6A1F
            kotor_fastcall_0040bc70(manager_of(self), nullptr, box, 1, 1);                      // 006C6A22..006C6A2A AddPanel
            return;
        }
        kotor_fastcall_006c59a0(self, nullptr, item, slot);                                     // 006C6A43..006C6A4B InsertUpgrade
    }
    kotor_fastcall_006c3aa0(self, nullptr);                                                     // 006C6A50..006C6A52 UpdateItems
    kotor_fastcall_006c3c30(self, nullptr, control);                                            // 006C6A57..006C6A5D OnEnterSlot(control)
}

// 006C6A80 CSWGuiUpgrade::HandleInputEvent(int event, int down) RET 8. Switch on event-28h (index bytes 006C6B4C,
// targets 006C6B3C): 28h/2Eh -> close/cancel, 39h -> list scroll 31h, 3Ah -> list scroll 32h.
void __fastcall kotor_fastcall_006c6a80(void* self, void*, w4::U32 event, w4::U32 down) {
    if (down != 0 && event - 0x28u <= 0x12u) {                                                  // 006C6A8B..006C6A9B
        if (event == 0x28 || event == 0x2E) {                                                   // 006C6AAF
            kotor_fastcall_0040a140(manager_of(self), nullptr, 0);                              // 006C6AAF..006C6AB4
            if ((lb(self, 0x2F48) & 1u) != 0) kotor_fastcall_006c4d30(self, nullptr);           // 006C6AB9..006C6AC4 CloseItems
            else kotor_fastcall_006c61f0(self, nullptr);                                        // 006C6AD8 CancelPanel
        } else if (event == 0x39 || event == 0x3A) {                                            // 006C6B17 / 006C6AEC
            const U32 code = event == 0x39 ? 0x31u : 0x32u;
            void* listbox = type_of(self) == 1 ? at(self, 0x1B40) : at(self, 0x1860);           // 006C6AF0..006C6B23
            vcall<void, U32, U32>(listbox, 0x3C, code, 1);                                      // 006C6B05 / 006C6B29
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, down);                           // CSWGuiPanel::HandleInputEvent
}
}
