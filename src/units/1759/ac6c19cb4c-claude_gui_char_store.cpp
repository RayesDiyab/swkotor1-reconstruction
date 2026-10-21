// Lane W4 GUI successor (worker "gui-char"): CSWGuiStore (0x2280, table 00756E38) slot functions, its trapped event
// handler and the store helpers they reach that had no provider in the c24 candidate map.
//   006C1650 OnPanelRemoved        thiscall() RET            (slot)
//   006C2170 OnPanelAdded          thiscall() RET            (slot)
//   006C2190 HandleInputEvent      thiscall(int, int) RET 8  (slot)
//   006C1B00 OnControlXButton(CSWGuiControl*)  thiscall RET 4 (event handler, W4_GUI_TRAP_HANDLER in panels_menus.cpp)
// helpers reconstructed here:
//   0062E4A0 CGuiInGame::HideStoreGui()                    0062D3E0 CGuiInGame::ShowExamineBox(ulong, int) RET 8
//   004AE7B0 CServerExoApp::GetStoreByGameObjectID(ulong) RET 4 -> 004B1AB0 CServerExoAppInternal::GetStoreByGameObjectID
//   006C0610 PopulateStore   006C1B50 InitializeStoreType   006C13C0 ShowSellGUI   006C1A50 ShowBuyGUI
//   006C0850 PopulateInventoryItemListBox   006C1840 PopulateStoreItemListBox
//   006C1660 CreateItemEntry(int*, CExoArrayList*, ulong, int) RET 10h
//   006C0690 SetDescription(CExoString const&) RET 4   006C0AA0 OnControlEntered(CSWGuiControl*) RET 4
//   006C0790 GetItemBuyValue(CSWSItem*) RET 4          006C07F0 GetItemSellValue(CSWSItem*) RET 4
//   006C0F40 OnControlInvAButton RET 4   006C1130 OnControlStoreAButton RET 4
//   006C0EE0 SellItemCallback RET 4      006C0D20 BuyItemCallback RET 4
//   006C0D80 SellItem(CSWSItem*) RET 4   006C0BE0 BuyItem(CSWSItem*) RET 4
//   005C6F70 CSWSStore::SellItem(CSWSItem*, CSWSCreature*) RET 8
//   006B2590 CSWGuiListBox::SetTopVisible(short) RET 4
//   CSWGuiStoreItemEntry (0x394, table 00756850): ctor 006B71E0, dtor 006B87C0, deleting dtor 006B87A0,
//   Initialize 006B53F0 (slot +A0, RET 10h), SetItem 006B7270 (RET 8)
// Transcribed from the original swkotor.exe linear sweep; each statement carries its original PCs.
#include "6030a9ee65-claude_gui_char.hpp"

namespace frame_control { extern std::uint32_t sound_manager_007a39ec; }                         // [007A39EC] CExoSound*
namespace resource_ref_assignment {                                                               // CResRef::operator=(const CResRef&) (by-value return)
menu_resources::Ref* __fastcall assign_00406120(menu_resources::Ref*, void*, menu_resources::Ref* out, const menu_resources::Ref* source);
}

extern "C" {
// ---- GUI
void __fastcall gui_load_text_params_set_strref_00415e50(void* params, void*, w4::U32 strref) noexcept(false); // SetStrRef
void* __fastcall kotor_m1_listbox_get_selected_control_0041a5d0(void* listbox, void*);           // CSWGuiListBox::GetSelectedControl
void __fastcall gui_m48_listbox_add_controls_0041c2c0(void* listbox, void*, void** controls, w4::U32 count, w4::U32, w4::U32, w4::U32); // AddControls
void __fastcall gui_load_listbox_set_active_control_0041c160(void* listbox, void*, void* control, w4::U32); // SetActiveControl
w4::U32 __fastcall gui_load_text_font_height_00414ee0(void* text, void*);                        // CSWGuiText::GetFontHeight
w4::I32 __fastcall async_string_length_005e5790(void* string, void*);                            // CExoString::GetLength
void __fastcall frame_R4_0062aa00(void* in_game, void*, unsigned, unsigned);                     // CGuiInGame::SetSWGuiStatus
void* __fastcall gui_load_border_construct_004167f0(void* self, void*) noexcept(false);         // CSWGuiBorder()
void __fastcall gui_load_border_destruct_004151c0(void* self, void*);                           // ~CSWGuiBorder
void* __fastcall gui_load_text_construct_00417280(void* self, void*) noexcept(false);           // CSWGuiText()
void __fastcall gui_load_text_destruct_00415ff0(void* self, void*);                             // ~CSWGuiText
void* __fastcall gui_load_border_params_assign_004150c0(void* self, void*, const void* source);  // CSWGuiBorderParams::operator=
void* __fastcall gui_load_border_params_construct_00415000(void* self, void*, const void* corner, const void* edge,
    const void* fill, w4::U32 fill_style, w4::I32 dimension, float f0c, const void* color, w4::I32 inner) noexcept(false);
void* __fastcall kotor_fastcall_00417120(void* self, void*, void* text, const void* font, const void* color, w4::U32 align, w4::U32 alpha_bits); // CSWGuiTextParams ctor
void* __fastcall kotor_fastcall_004171e0(void* self, void*, const void* source) noexcept(false); // CSWGuiTextParams::operator=
void __fastcall gui_load_slot_ret8_005b5e90(void*, void*, w4::U32, w4::U32);                     // 005B5E90 ret 8
void __fastcall light_manager_void_return_16_0063cfc0(void*, void*, w4::U32, w4::U32, w4::U32, w4::U32); // 0063CFC0 ret 10h
void __fastcall kotor_fastcall_006b5270(void* self, void*, const void* extent);                  // CSWGuiStoreItemEntry::SetExtent (claude_equip_items.cpp)
void __fastcall kotor_fastcall_006b4d40(void* self, void*, w4::U32 arg);                         // CSWGuiStoreItemEntry::Draw (claude_equip_items.cpp)
void __fastcall kotor_fastcall_006c1330(void* self, void*);                                      // CSWGuiStore::DeleteItemEntries (panels_menus.cpp)
void* __fastcall c2da_default_ref_00405ed0(void* self, void*);                                   // CResRef::CResRef()
menu_resources::Ref* __fastcall unresolved_ref_assign_00406290(menu_resources::Ref*, void*, menu_resources::Ref* out, const char* text); // CResRef::operator=(const char*)
// ---- client / server
void* __fastcall kotor_fastcall_005eda40(void* app, void*);                                      // CClientExoApp::GetSWGuiManager
w4::U32 __fastcall kotor_fastcall_005ed4c0(void* app, void*);                                    // CClientExoApp::GetClientLanguage
w4::U32 __fastcall exo_locstring_get_005ea130(void* loc, void*, w4::U32 language, void* out, w4::U32 gender); // CExoLocString::GetString
void* __fastcall feedback_fetch_005ee390(void* tlk, void*, void* out, w4::U32 strref, void* object, w4::U32, w4::U32); // CSWTlkTable::Fetch
void __fastcall kotor_fastcall_0041db50(void* tlk, void*, unsigned token, void* value);         // CTlkTable::SetCustomToken
extern void* kotor_cell_tlk_client_007a3a0c;                                                     // [007A3A0C] CSWTlkTable*
w4::U32 __fastcall kotor_fastcall_004d8230(void* array, void*, w4::U32 id, void** out);          // CGameObjectArray::GetGameObject (AL)
w4::I32 __fastcall w2x_creature_gold_004edd60(void* creature, void*);                            // CSWSCreature::GetGold
void __fastcall kotor_fastcall_004edd90(void* creature, void*, w4::I32 gold);                    // CSWSCreature::SetGold
void* __fastcall recovered_creature_inventory_004ef770(void* creature, void*, w4::U32 flag);     // CSWSCreature::GetItemRepository
void* __fastcall w2x_inventory_item_005560b0(void* repository, void*, w4::U32 index);            // CItemRepository::ItemListGetItem
w4::U32 __fastcall recovered_remove_item_00555fd0(void* repository, void*, void* item);          // CItemRepository::RemoveItem
w4::U32 __fastcall recovered_store_acquire_item_005c7610(void* store, void*, void* item);        // CSWSStore::AcquireItem
w4::U32 __fastcall recovered_creature_acquire_item_005158e0(void* creature, void*, void** item, w4::U32, w4::U32, w4::U32); // CSWSCreature::AcquireItem
void* __fastcall recovered_item_construct_005530a0(void* item, void*, w4::U32 id);               // CSWSItem::CSWSItem
w4::U32 __fastcall kotor_fastcall_0055d950(void* item, void*, void* source);                     // CSWSItem::CopyItem
void* __fastcall kotor_fastcall_0055f280(void* item, void*, w4::U32 count);                      // CSWSItem::SplitItem
w4::U32 __fastcall recovered_item_cost_00554000(void* item, void*);                              // CSWSItem::GetCost
void* __fastcall kotor_fastcall_0055f340(void* item, void*, void* out);                          // CSWSItem::GetPropertyDescription
menu_resources::Ref* __fastcall kotor_fastcall_005556b0(void* item, void*, menu_resources::Ref* out); // CSWSItem::GetIcon
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void* item, void*) noexcept(false);      // CSWItem::GetBaseItem
w4::U8 __fastcall recovered_stats_level_005a5fd0(void* stats, void*, w4::U32 flag);              // CSWSCreatureStats::GetLevel
// ---- defined here
void __fastcall kotor_fastcall_0062e4a0(void* gui, void*);
void __fastcall kotor_fastcall_0062d3e0(void* gui, void*, w4::U32 strref, w4::U32 flag);
void* __fastcall kotor_fastcall_004b1ab0(void* internal, void*, w4::U32 id);
void __fastcall kotor_fastcall_006b2590(void* listbox, void*, w4::U32 top);
void* __fastcall kotor_fastcall_006b71e0(void* self, void*) noexcept(false);
void __fastcall kotor_fastcall_006b87c0(void* self, void*);
void* __fastcall kotor_fastcall_006b87a0(void* self, void*, w4::U32 flags);
void __fastcall kotor_fastcall_006b53f0(void* self, void*, void* text, void* border, void* highlight, w4::I32 width);
void __fastcall kotor_fastcall_006b7270(void* self, void*, w4::U32 id, w4::U32 unused);
w4::U32 __fastcall kotor_fastcall_005c6f70(void* store, void*, void* item, void* creature);
void __fastcall kotor_fastcall_006c0610(void* self, void*);
void __fastcall kotor_fastcall_006c0690(void* self, void*, void* text);
w4::U32 __fastcall kotor_fastcall_006c0790(void* self, void*, void* item);
w4::U32 __fastcall kotor_fastcall_006c07f0(void* self, void*, void* item);
void __fastcall kotor_fastcall_006c0850(void* self, void*);
void __fastcall kotor_fastcall_006c0aa0(void* self, void*, void* control);
void __fastcall kotor_fastcall_006c0be0(void* self, void*, void* item);
void __fastcall kotor_fastcall_006c0d20(void* self, void*, void* control);
void __fastcall kotor_fastcall_006c0d80(void* self, void*, void* item);
void __fastcall kotor_fastcall_006c0ee0(void* self, void*, void* control);
void __fastcall kotor_fastcall_006c0f40(void* self, void*, void* control);
void __fastcall kotor_fastcall_006c1130(void* self, void*, void* control);
void __fastcall kotor_fastcall_006c13c0(void* self, void*);
w4::U32 __fastcall kotor_fastcall_006c1660(void* self, void*, w4::I32* index, void* list, w4::U32 id, w4::U32 inventory);
void __fastcall kotor_fastcall_006c1840(void* self, void*);
void __fastcall kotor_fastcall_006c1a50(void* self, void*);
void __fastcall kotor_fastcall_006c1b50(void* self, void*);
void __fastcall kotor_fastcall_006c1b00(void* self, void*, void* control);
void __fastcall kotor_fastcall_006c1650(void* self, void*);
void __fastcall kotor_fastcall_006c2170(void* self, void*);
void __fastcall kotor_fastcall_006c2190(void* self, void*, std::uint32_t event, std::uint32_t down);
}

using namespace gchar;

namespace {
const char k_empty_0073d71c[] = "";                                                              // 0073D71C
const char k_int_0073d720[] = "%d";                                                              // 0073D720
// .rdata 0074666C (byte 0) / 0074663D (byte 0Eh): GetGameObject success code / OBJECT_TYPE_STORE.
constexpr U8 k_object_ok_0074666c = 0x00;
constexpr U8 k_object_type_store_0074663d = 0x0E;

inline void* store_of(void* self) { return kotor_fastcall_004ae7b0(server_app(), nullptr, lw(self, 0x2278)); }
inline void strref(void* self, U32 params, U32 id) { gui_load_text_params_set_strref_00415e50(at(self, params), nullptr, id); }
inline void text_literal(void* self, U32 params, const char* value) {
    Str t(value); gui_load_text_params_set_text_00415e00(at(self, params), nullptr, t.p());
}
inline void text_int(void* self, U32 params, I32 value) {
    Str t(value); gui_load_text_params_set_text_00415e00(at(self, params), nullptr, t.p());
}
inline void fill_named(void* self, U32 params, const char* name) {
    menu_resources::Ref ref; ref.construct_00406d80(name);                                     // CResRef(const char*)
    gui_load_border_params_set_fill_00414c00(at(self, params), nullptr, &ref, 0);              // SetFillImage(ref, 0)
}
inline void copy12(void* dst, const void* src) { std::memcpy(dst, src, 12); }

// CSWGuiStoreItemEntry method table 00756850 (41 slots): the CSWGuiButton table 0073E658 (40 slots) plus the slots
// where 00756850 differs from it (+00 +04 +38 +98 +9C) and the new +A0 (Initialize).
const Override ovr_store_item_entry[] = {
    {0x00, fn(&kotor_fastcall_006b87a0)},                   // 006B87A0 ~CSWGuiStoreItemEntry (deleting)
    {0x04, fn(&kotor_fastcall_006b5270)},                   // 006B5270 SetExtent
    {0x38, fn(&kotor_fastcall_006b4d40)},                   // 006B4D40 Draw
    {0x98, fn(&gui_load_slot_ret8_005b5e90)},               // 005B5E90 (ret 8)
    {0x9C, fn(&light_manager_void_return_16_0063cfc0)},     // 0063CFC0 (ret 10h)
    {0xA0, fn(&kotor_fastcall_006b53f0)},                   // 006B53F0 Initialize
};
void* table_store_item_entry[41];
bool built_store_item_entry = false;
const void* vt_store_item_entry() {
    if (!built_store_item_entry) {
        vt_build(table_store_item_entry, 41, gui_load_button_methods_0073e658, 40, ovr_store_item_entry, 6);
        built_store_item_entry = true;
    }
    return table_store_item_entry;
}

// Shared tail of the two "too expensive" confirmations (006C1029..006C110C / 006C1212..006C12F5).
void confirm_box(void* self, U32 threshold, U32 strref_id, const void* callback, void* control) {
    void* box = lp(ingame_gui(), 0x98);                                                        // GetInGameGui()->+98h
    Str message; message.empty();                                                              // CExoString() [esp+10] state 0
    {
        Str token(static_cast<I32>(threshold));                                                // CExoString(int)
        kotor_fastcall_0041db50(kotor_cell_tlk_007a3a08, nullptr, 0, token.p());               // SetCustomToken(0, token)
    }
    {
        Str fetched;
        void* r = feedback_fetch_005ee390(kotor_cell_tlk_client_007a3a0c, nullptr, fetched.s, strref_id, nullptr, 0, 0); // CSWTlkTable::Fetch
        fetched.live = true;
        async_assign_string_005e5c50(message.p(), nullptr, r);                                 // operator=
    }
    kotor_fastcall_00627130(box, nullptr, 1);                                                  // SetAllowCancel(1)
    ExoVal copy;
    WindowCopy005e5b00(copy.w, nullptr, message.p());                                          // by-value copy
    reinterpret_cast<void(__thiscall*)(void*, ExoVal)>(vslot(box, 0x70))(box, copy);           // [vtbl+70h]
    gui_load_messagebox_set_callback_00624a40(box, nullptr, self, ad(callback), ad(control));  // SetCallback(self, fn, control)
    kotor_fastcall_0040bc70(kotor_fastcall_005eda40(client_app(), nullptr), nullptr, box, 1, 1); // GetSWGuiManager()->AddPanel
}                                                                                              // ~message

// Player level purchase / sale limit: GetLevel(0)*50 (capped at 250 for selling), or the fallback.
U32 level_limit(void* player, U32 fallback, bool cap) {
    if (player == nullptr) return fallback;
    const U32 limit = static_cast<U32>(recovered_stats_level_005a5fd0(lp(player, 0xA74), nullptr, 0)) * 0x32u;
    if (cap && static_cast<I32>(limit) > 0xFA) return fallback;                                // jg (signed)
    if (!cap) return limit;
    return static_cast<U32>(recovered_stats_level_005a5fd0(lp(player, 0xA74), nullptr, 0)) * 0x32u; // re-read
}
}  // namespace

extern "C" {
// ======================= CGuiInGame helpers
// 0062E4A0 CGuiInGame::HideStoreGui() RET 0.
void __fastcall kotor_fastcall_0062e4a0(void* gui, void*) {
    if (lw(gui, 0x108) == 0) return;                                                           // 0062E4A3..0062E4AB
    if (kotor_fastcall_005edc10(client_app(), nullptr) != 0)                                   // 0062E4B1..0062E4C0 GetPausedByCombat
        kotor_fastcall_005edc20(client_app(), nullptr, 0, 0, 0);                               // 0062E4C2..0062E4D1
    frame_R4_0062aa00(gui, nullptr, 4, 1);                                                     // 0062E4D6..0062E4DC SetSWGuiStatus(4, 1)
    gui_m48_client_set_input_class_005eda60(client_app(), nullptr, 0, 1);                      // 0062E4E1..0062E4EE
    if (w4::ingame_gui_single_panel_007a2370 == 0) {                                           // 0062E4F3..0062E4FA
        void* store = lp(gui, 0x84);                                                           // 0062E4FC
        sw(store, 0x44, (lw(store, 0x44) & 0xFFFFFDFFu) | 0x500u);                             // 0062E502..0062E511
        zero(gui, 0x84);                                                                       // 0062E514
    } else {
        frame_RemovePanel_0040c830(lp(gui, 0x38), nullptr, lp(gui, 0x84));                    // 0062E520..0062E52A
    }
    kotor_fastcall_0040a140(lp(gui, 0x38), nullptr, 5);                                        // 0062E52F..0062E534 PlayGuiSound(5)
    frame_R4_005d5e80(pt(frame_control::sound_manager_007a39ec), nullptr, kotor_rdata_sound_mode_0074c5ec); // 0062E539..0062E545
}

// 0062D3E0 CGuiInGame::ShowExamineBox(ulong strref, int flag) RET 8.
void __fastcall kotor_fastcall_0062d3e0(void* gui, void*, w4::U32 strref_id, w4::U32 flag) {
    if (lw(gui, 0x108) == 0) return;                                                           // 0062D3E3..0062D3EB
    vcall<void, U32>(lp(gui, 0x50), 0x6C, strref_id);                                          // 0062D3ED..0062D3F7
    sw(lp(gui, 0x50), 0x984, flag);                                                            // 0062D3FA..0062D403
    kotor_fastcall_0040bc70(lp(gui, 0x38), nullptr, lp(gui, 0x50), 1, 1);                      // 0062D401..0062D412 AddPanel
    gui_m48_client_set_input_class_005eda60(client_app(), nullptr, 2, 1);                      // 0062D418..0062D430 jmp SetInputClass(2, 1)
}

// ======================= server
// 004AE7B0 CServerExoApp::GetStoreByGameObjectID(ulong id) RET 4: tail call on the internal app ([this+4]).
void* __fastcall kotor_fastcall_004ae7b0(void* server, void*, w4::U32 id) {
    return kotor_fastcall_004b1ab0(lp(server, 4), nullptr, id);                                // 004AE7B0..004AE7B3
}
// 004B1AB0 CServerExoAppInternal::GetStoreByGameObjectID(ulong id) RET 4.
void* __fastcall kotor_fastcall_004b1ab0(void* internal, void*, w4::U32 id) {
    void* object = nullptr;                                                                    // 004B1AC1 [esp]
    const U8 status = static_cast<U8>(kotor_fastcall_004d8230(lp(internal, 0x1005C), nullptr, id, &object)); // 004B1AB1..004B1AC9
    if (status != k_object_ok_0074666c) return nullptr;                                        // 004B1ACE..004B1AD4
    if (object == nullptr) return nullptr;                                                     // 004B1AD6..004B1ADC
    if (lb(object, 8) != k_object_type_store_0074663d) return nullptr;                         // 004B1ADE..004B1AE7
    return vcall<void*>(object, 0x64);                                                         // 004B1AE9..004B1AEB AsSWSStore
}

// 005C6F70 CSWSStore::SellItem(CSWSItem* item, CSWSCreature* buyer) RET 8.
w4::U32 __fastcall kotor_fastcall_005c6f70(void* store, void*, void* item, void* creature) {
    if ((lb(item, 0x288) & 4) != 0) {                                                          // 005C6F8B..005C6F96 infinite stock
        void* memory = live_operator_new(0x29C);                                               // 005C6F9C..005C6FA1
        void* copy = nullptr;
        if (memory != nullptr) {                                                               // 005C6FAD..005C6FB7
            auto guard = on_throw([&]() { live_free(memory); });                               // EH state 0
            copy = recovered_item_construct_005530a0(memory, nullptr, 0x7F000000u);            // 005C6FB9..005C6FC0
            guard.armed = false;
        }
        void* local = copy;                                                                    // 005C6FD8 [esp+4]
        kotor_fastcall_0055d950(copy, nullptr, item);                                          // 005C6FC9..005C6FDC CopyItem
        sw(local, 0x288, lw(local, 0x288) & 0xFFFFFFFBu);                                      // 005C6FE1..005C6FF2
        sw(local, 0x28C, 1);                                                                   // 005C6FF8..005C6FFE
        return recovered_creature_acquire_item_005158e0(creature, nullptr, &local, lw(store, 4), 0x7F000000u, 1); // 005C6FFC..005C7016
    }
    if (li(item, 0x28C) > 1) {                                                                 // 005C702D..005C7034
        void* local = kotor_fastcall_0055f280(item, nullptr, 1);                               // 005C7036..005C7038 SplitItem(1)
        return recovered_creature_acquire_item_005158e0(creature, nullptr, &local, lw(store, 4), 0x7F000000u, 1); // 005C703D..005C7055
    }
    if (recovered_remove_item_00555fd0(lp(store, 0x230), nullptr, item) == 0) return 0;       // 005C706C..005C707A / 005C70A7
    void* local = item;                                                                        // [esp+1C] (argument slot)
    return recovered_creature_acquire_item_005158e0(creature, nullptr, &local, lw(store, 4), 0x7F000000u, 1); // 005C707C..005C7090
}

// ======================= CSWGuiListBox
// 006B2590 CSWGuiListBox::SetTopVisible(short top) RET 4.
void __fastcall kotor_fastcall_006b2590(void* listbox, void*, w4::U32 top) {
    const I32 range = li(listbox, 0x2A0) - static_cast<std::int16_t>(lh(listbox, 0x2C4));      // 006B2590..006B259D
    const I32 limit = range < 0 ? 0 : range;                                                   // 006B259F..006B25AB (sets/dec/and)
    const I32 wanted = static_cast<std::int16_t>(static_cast<U16>(top));                       // 006B25AD
    if (wanted < limit) {                                                                      // 006B25B2..006B25B5
        const U16 v = static_cast<U16>(wanted); std::memcpy(at(listbox, 0x2C8), &v, 2);        // 006B25B7
        return;
    }
    const U16 v = static_cast<U16>(range < 0 ? 0 : range);                                     // 006B25C1..006B25C9
    std::memcpy(at(listbox, 0x2C8), &v, 2);                                                    // 006B25CB
}

// ======================= CSWGuiStoreItemEntry
// 006B71E0 CSWGuiStoreItemEntry::CSWGuiStoreItemEntry() (returns this).
void* __fastcall kotor_fastcall_006b71e0(void* self, void*) noexcept(false) {
    gui_load_button_construct_0041c0f0(self, nullptr);                                         // 006B71FD
    Unwind u; u.push(&gui_load_button_destruct_00419da0, self);                                // EH state 0
    set_vptr(self, vt_store_item_entry());                                                     // 006B7210 (00756850)
    for (U32 off : {0x1C8u, 0x23Cu, 0x2B0u}) {                                                 // 006B7202..006B7236
        gui_load_border_construct_004167f0(at(self, off), nullptr); u.push(&gui_load_border_destruct_004151c0, at(self, off));
    }
    gui_load_text_construct_00417280(at(self, 0x324), nullptr);                                // 006B723B..006B7246
    u.dismiss();
    sw(self, 0x1C4, 0x7F000000u);                                                              // 006B724F
    return self;
}
// 006B87C0 CSWGuiStoreItemEntry::~CSWGuiStoreItemEntry (members in reverse, then ~CSWGuiButton; no vptr store).
void __fastcall kotor_fastcall_006b87c0(void* self, void*) {
    gui_load_text_destruct_00415ff0(at(self, 0x324), nullptr);                                 // 006B87DD..006B87EB
    gui_load_border_destruct_004151c0(at(self, 0x2B0), nullptr);                               // 006B87F0..006B87FB
    gui_load_border_destruct_004151c0(at(self, 0x23C), nullptr);                               // 006B8800..006B880B
    gui_load_border_destruct_004151c0(at(self, 0x1C8), nullptr);                               // 006B8810..006B881B
    gui_load_button_destruct_00419da0(self, nullptr);                                          // 006B8820..006B882A
}
// 006B87A0 scalar deleting destructor (thiscall(flags) RET 4).
void* __fastcall kotor_fastcall_006b87a0(void* self, void*, w4::U32 flags) {
    return deleting(self, &kotor_fastcall_006b87c0, flags);                                    // 006B87A0..006B87BB
}
// 006B53F0 CSWGuiStoreItemEntry::Initialize(CSWGuiText* text, CSWGuiBorder* border, CSWGuiBorder* highlight, int width) RET 10h.
void __fastcall kotor_fastcall_006b53f0(void* self, void*, void* text, void* border, void* highlight, w4::I32 width) {
    kotor_fastcall_004171e0(at(self, 0x16C), nullptr, at(text, 0x18));                         // 006B53F6..006B540B
    gui_load_border_params_assign_004150c0(at(self, 0x80), nullptr, at(border, 0x14));          // 006B5410..006B5423
    copy12(at(self, 0x90), borrowed_exit_color_007a23b4);                                      // 006B5428..006B5448
    gui_load_border_params_assign_004150c0(at(self, 0xF4), nullptr, at(highlight, 0x14));       // 006B5438..006B5457
    copy12(at(self, 0x104), borrowed_enter_color_007a23c0);                                    // 006B545C..006B547A
    gui_load_border_params_set_pulsing_00414d00(at(self, 0xF4), nullptr, 1, 1, 0);             // 006B5476..006B5481
    for (U32 pass = 0; pass < 2; ++pass) {                                                     // 006B5486..006B54FE / 006B551A..006B5592
        menu_resources::Ref fill, edge, corner;                                                // "lbl_hex_3", "", ""
        fill.construct_00406d80("lbl_hex_3");
        edge.construct_00406d80(k_empty_0073d71c);
        corner.construct_00406d80(k_empty_0073d71c);
        float color[3] = {1.0f, 1.0f, 1.0f};                                                   // [esp+8]
        alignas(4) unsigned char params[0x54];                                                 // [esp+48] CSWGuiBorderParams
        void* p = gui_load_border_params_construct_00415000(params, nullptr, &corner, &edge, &fill, 0, 0, 1.0f, color, 0);
        gui_load_border_params_assign_004150c0(at(self, pass == 0 ? 0x1DCu : 0x250u), nullptr, p);
        if (pass == 0) copy12(at(self, 0x1EC), borrowed_exit_color_007a23b4);                  // 006B54FE..006B5517
        else copy12(at(self, 0x260), borrowed_enter_color_007a23c0);                           // 006B5592..006B55AF
    }
    gui_load_border_params_set_pulsing_00414d00(at(self, 0x250), nullptr, 1, 0, 1);            // 006B55AB..006B55B6
    I32 extent[4] = {0, 0, width, 0x38};                                                       // 006B55BB..006B55DF [esp+8]
    vcall<void, const void*>(self, 0x04, extent);                                              // 006B55C2..006B55E7 SetExtent
}
// 006B7270 CSWGuiStoreItemEntry::SetItem(ulong id, int) RET 8.
void __fastcall kotor_fastcall_006b7270(void* self, void*, w4::U32 id, w4::U32) {
    menu_resources::Ref icon; c2da_default_ref_00405ed0(&icon, nullptr);                       // 006B7296..006B729A [esp+2C]
    Str name; name.empty();                                                                    // 006B729F..006B72A3 [esp+1C] state 0
    Str count; count.empty();                                                                  // 006B72A8..006B72B7 [esp+24] state 1
    Str unused_c; unused_c.empty();                                                            // 006B72BC..006B72C8 [esp+44] state 2
    Str unused_d; unused_d.empty();                                                            // 006B72CD..006B72D9 [esp+3C] state 3
    void* server = server_app();                                                               // 006B72DE..006B72E3 ebx
    recovered_item_lookup_004ae760(server, nullptr, id);                                       // 006B72E9..006B72F4 (result unused)
    sw(self, 0x1C4, id);                                                                       // 006B72FF
    void* item = id != 0x7F000000u ? recovered_item_lookup_004ae760(server, nullptr, id) : nullptr; // 006B72F9..006B7313
    if (item != nullptr) {                                                                     // 006B7315..006B7317
        menu_resources::Ref fetched, discarded;
        menu_resources::Ref* r = kotor_fastcall_005556b0(item, nullptr, &fetched);             // 006B731D..006B7324 GetIcon
        resource_ref_assignment::assign_00406120(&icon, nullptr, &discarded, r);              // 006B7329..006B7333
        exo_locstring_get_005ea130(at(item, 0x280), nullptr, kotor_fastcall_005ed4c0(client_app(), nullptr), name.p(), 0); // 006B7338..006B7353
        gui_load_text_params_set_text_00415e00(at(self, 0x16C), nullptr, name.p());            // 006B7358..006B7363
        const I32 stack = li(item, 0x28C);                                                     // 006B7368
        if (stack > 1) {                                                                       // 006B736E..006B7375
            const char* hex = stack >= 100 ? "lbl_hex_7" : "lbl_hex_6";                        // 006B737B..006B737E (00756784 / 00756778)
            fill_named(self, 0x1DC, hex);                                                      // 006B7380..006B7397 / 006B73B3..006B73CA
            fill_named(self, 0x250, hex);                                                      // 006B739C..006B73EA
            frame_LogFormat_005e5680(count.p(), k_int_0073d720, stack);                        // 006B73EF..006B73FF
        } else {
            fill_named(self, 0x1DC, "lbl_hex_3");                                              // 006B7404..006B741B (00755EBC)
            fill_named(self, 0x250, "lbl_hex_3");                                              // 006B7420..006B743B
        }
        menu_resources::Ref font; font.construct_00406d80("fnt_d16x16");                       // 006B7440..006B7449 (00753DA0) [esp+C]
        U32 params[0x70 / 4];                                                                  // [esp+5C] CSWGuiTextParams
        kotor_fastcall_00417120(params, nullptr, count.p(), &font, borrowed_exit_color_007a23b4, 0x22, 0x3F800000u); // 006B744E..006B7468
        {
            auto g = on_throw([&]() { frame_LogStringDtor_005e5c20(params, nullptr); });       // EH state 4
            kotor_fastcall_004171e0(at(self, 0x33C), nullptr, params);                         // 006B746D..006B747C
            g.armed = false;
        }
        frame_LogStringDtor_005e5c20(params, nullptr);                                         // 006B7481..006B748D
    } else {
        {
            Str text; void* s = gui_string(text, 0x16B);                                       // 006B7494..006B74B4 / 006B74B6..006B74CE
            gui_load_text_params_set_text_00415e00(at(self, 0x16C), nullptr, s);               // 006B74D6..006B74DD
        }                                                                                      // 006B74E2..006B74EE
        menu_resources::Ref discarded;
        unresolved_ref_assign_00406290(&icon, nullptr, &discarded, "inone");                   // 006B74F3..006B7501 (00755EC8)
    }
    sw(self, 0x2E0, (lw(self, 0x2E0) & 0xFFFFFFFEu) | 2u);                                     // 006B7506..006B751F
    gui_load_border_params_set_fill_00414c00(at(self, 0x2C4), nullptr, &icon, 0);              // 006B7515..006B7522
}                                                                                              // 006B7527..006B7569 ~D ~C ~count ~name

// ======================= CSWGuiStore
// 006C0610 CSWGuiStore::PopulateStore() RET 0: show the owner's (player's) gold.
void __fastcall kotor_fastcall_006c0610(void* self, void*) {
    void* creature = kotor_fastcall_004ae770(server_app(), nullptr, lw(self, 0x227C));          // 006C0630..006C063A
    if (creature == nullptr) return;                                                           // 006C063F..006C0641
    const I32 gold = w2x_creature_gold_004edd60(creature, nullptr);                            // 006C0643..006C0645
    sw(self, 0x2270, static_cast<U32>(gold));                                                  // 006C064F
    text_int(self, 0x12E8, gold);                                                              // 006C064A..006C067A LBL_CREDITS_VALUE
}

// 006C0690 CSWGuiStore::SetDescription(const CExoString& text) RET 4.
void __fastcall kotor_fastcall_006c0690(void* self, void*, void* text) {
    gui_load_text_params_set_text_00415e00(at(self, 0xA28), nullptr, text);                    // 006C0696..006C06A3
    void* listbox = at(self, 0x1A40);                                                          // 006C06A8 LB_DESCRIPTION
    gui_load_listbox_clear_items_00419a90(listbox, nullptr);                                   // 006C06AE..006C06B0
    U32 extent[4];
    std::memcpy(extent, at(self, 0x944), 16);                                                  // 006C06B5..006C06D4
    U32 height;
    if (async_string_length_005e5790(text, nullptr) > 0)                                       // 006C06D2..006C06E5
        height = vcall<U32>(at(self, 0xA10), 0x08);                                            // 006C06E7..006C06E9
    else
        height = gui_load_text_font_height_00414ee0(at(self, 0xA10), nullptr);                 // 006C06EE
    extent[3] = height;                                                                        // 006C06F9
    void* label = at(self, 0x940);                                                             // 006C06FD edi
    vcall<void, void*>(label, 0x04, extent);                                                   // 006C06F3..006C070A SetExtent
    void* single = label;                                                                      // 006C071C (argument slot reused)
    gui_m48_listbox_add_controls_0041c2c0(listbox, nullptr, &single, 1, 0, 0, 0);              // 006C070D..006C0720
    gui_load_listbox_set_active_control_0041c160(listbox, nullptr, label, 0);                  // 006C0725..006C072A
    U32 show = 0;                                                                              // 006C072F..006C0766
    if (static_cast<I32>(lb(listbox, 0x2C0)) + li(listbox, 0x2B4) > li(listbox, 0x298) &&
        li(listbox, 0x2A0) > 0 && (lw(listbox, 0x2BC) & 0x100u) == 0) show = 1;
    const U32 f = lw(self, 0xFC4);
    sw(self, 0xFC4, f ^ (((show + show) ^ f) & 2u));                                           // 006C0766..006C0779
}

// 006C0790 CSWGuiStore::GetItemBuyValue(CSWSItem* item) RET 4.
w4::U32 __fastcall kotor_fastcall_006c0790(void* self, void*, void* item) {
    void* store = store_of(self);                                                              // 006C0790..006C07A2
    if (store == nullptr || item == nullptr) return 0;                                         // 006C07A7..006C07B1 / 006C07DC
    const U32 markup = lw(store, 0x250) + lw(store, 0x248);                                    // 006C07B3..006C07C0
    const U32 cost = recovered_item_cost_00554000(item, nullptr);                              // 006C07C2
    return (cost * markup) / 100u;                                                             // 006C07C7..006C07D5 (imul, unsigned /100)
}
// 006C07F0 CSWGuiStore::GetItemSellValue(CSWSItem* item) RET 4.
w4::U32 __fastcall kotor_fastcall_006c07f0(void* self, void*, void* item) {
    void* store = store_of(self);                                                              // 006C07F0..006C0802
    if (store == nullptr || item == nullptr) return 0;                                         // 006C0807..006C0811 / 006C083C
    const U32 markdown = lw(store, 0x24C) + lw(store, 0x244);                                  // 006C0813..006C0820
    const U32 cost = recovered_item_cost_00554000(item, nullptr);                              // 006C0822
    return (cost * markdown) / 100u;                                                           // 006C0827..006C0835
}

// 006C1660 CSWGuiStore::CreateItemEntry(int* index, CExoArrayList* list, ulong id, int inventory) RET 10h, returns 0.
w4::U32 __fastcall kotor_fastcall_006c1660(void* self, void*, w4::I32* index, void* list, w4::U32 id, w4::U32 inventory) {
    PtrList& pool = *static_cast<PtrList*>(at(self, inventory != 0 ? 0x74u : 0x68u));          // 006C1676..006C168C ebp
    void* entry;
    if (*index < pool.size) {                                                                  // 006C168C..006C1695 (jge)
        entry = pool.data[*index];                                                             // 006C1697..006C169A
    } else {
        void* memory = live_operator_new(0x394);                                               // 006C16A2..006C16A7
        entry = nullptr;
        if (memory != nullptr) {                                                               // 006C16AF..006C16B9
            auto guard = on_throw([&]() { live_free(memory); });                               // EH state 0
            entry = kotor_fastcall_006b71e0(memory, nullptr);                                  // 006C16BB..006C16BD
            guard.armed = false;
        }
        list_add(pool, entry);                                                                 // 006C16C4..006C16FE
        *reinterpret_cast<volatile U8*>(static_cast<std::uintptr_t>(ad(entry) + 0x54)) = 0xFF; // 006C1701 (unchecked, quirk)
        void* proto;
        I32 width;
        const void* activate;
        if (inventory != 0) {                                                                  // 006C1705
            proto = vcall<void*>(lp(self, 0x1A2C), 0x58);                                      // 006C1707..006C170F LB_INVITEMS prototype
            width = li(self, 0x19F4) - static_cast<I32>(static_cast<U32>(lb(self, 0x1A20)) * 2u); // 006C1712..006C1723
            activate = reinterpret_cast<const void*>(&kotor_fastcall_006c0f40);                // 006C173F (OnControlInvAButton)
        } else {
            proto = vcall<void*>(lp(self, 0x174C), 0x58);                                      // 006C1755..006C175D LB_SHOPITEMS prototype
            width = li(self, 0x1714) - static_cast<I32>(static_cast<U32>(lb(self, 0x1740)) * 2u); // 006C1760..006C1771
            activate = reinterpret_cast<const void*>(&kotor_fastcall_006c1130);                // 006C178D (OnControlStoreAButton)
        }
        vcall<void, void*, void*, void*, I32>(entry, 0xA0, at(proto, 0x154), at(proto, 0x6C), at(proto, 0xE0), width); // Initialize
        gui_load_control_add_event_0041ab20(entry, nullptr, 0x27, self, activate);             // 006C173F..006C1749 / 006C178D..006C1797
        gui_load_control_add_event_0041ab20(entry, nullptr, 0x2D, self, activate);             // 006C174E / 006C179C..006C17A6
        gui_load_control_add_event_0041ab20(entry, nullptr, 0, self, reinterpret_cast<const void*>(&kotor_fastcall_006c0aa0)); // 006C17AB..006C17B5
    }
    kotor_fastcall_006b7270(entry, nullptr, id, 0);                                            // 006C17BA..006C17C3 SetItem(id, 0)
    vcall<void, U32>(entry, 0x40, 0);                                                          // 006C17C8..006C17CE SetActive(0)
    list_add(*static_cast<PtrList*>(list), entry);                                             // 006C17D1..006C17FE
    ++*index;                                                                                  // 006C1801..006C1805
    return 0;                                                                                  // 006C180D
}

// 006C0850 CSWGuiStore::PopulateInventoryItemListBox() RET 0: the owner's sellable items grouped by base-item category.
void __fastcall kotor_fastcall_006c0850(void* self, void*) {
    volatile U32 stale[1];                                                                     // [esp+2C] before initialisation
    void* owner = kotor_fastcall_004ae770(server_app(), nullptr, lw(self, 0x227C));             // 006C0868..006C0881
    // Quirk: without an owner creature the original reads the uninitialised stack slot [esp+2C] as the repository.
    void* repository = owner != nullptr ? recovered_creature_inventory_004ef770(owner, nullptr, 1) : pt(stale[0]); // 006C0886..006C0899
    PtrList entries;                                                                           // 006C089D..006C08A5 [esp+30] state 0
    I32 next = 0;                                                                              // 006C08A9 [esp+2C]
    U32 total = 0;                                                                             // 006C08BC [esp+10]
    for (U32 i = 0; i < lw(repository, 0x10); ++i) {                                           // 006C08AD..006C08E0 (unsigned)
        void* item = w2x_inventory_item_005560b0(repository, nullptr, i);
        if (item != nullptr && lw(item, 0x108) == 0) ++total;
    }
    U32 made = 0, lowest = 0, found = 0, current = 0;                                          // [esp+18] [esp+14] [esp+1C] edi
    if (total != 0) {                                                                          // 006C08E2..006C08F4
        do {
            for (U32 i = 0; i < lw(repository, 0x10); ++i) {                                   // 006C0900..006C097A
                if (found != 0) break;                                                         // 006C0910..006C0916
                void* item = w2x_inventory_item_005560b0(repository, nullptr, i);              // 006C0918..006C091B
                if (item == nullptr || lw(item, 0x108) != 0) continue;                         // 006C0922..006C092E
                const U32 category = lb(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0x30); // 006C0930..006C0937
                if (category == current) {                                                     // 006C093B..006C093D
                    found = kotor_fastcall_006c1660(self, nullptr, &next, &entries, lw(item, 0x14), 1); // 006C093F..006C0958
                    ++made;                                                                    // 006C095C
                } else if (static_cast<I32>(category) > static_cast<I32>(current)) {           // 006C0962 (jle)
                    if (lowest == current || category < lowest) lowest = category;            // 006C0964..006C0970 (jge signed)
                }
            }
            current = lowest;                                                                  // 006C0984
        } while (made < total);                                                                // 006C097C..006C0988 (jb)
    }
    void* listbox = at(self, 0x1760);                                                          // 006C099C LB_INVITEMS
    kotor_fastcall_0041c1d0(listbox, nullptr, &entries, 0, 1, 0);                              // 006C0992..006C09A5 AddControls
    if ((lb(self, 0x17A4) & 2) != 0) {                                                         // 006C09AA..006C09B4
        if (entries.size > 0) {                                                                // 006C09BA..006C09C4
            gui_load_listbox_set_selected_0041c040(listbox, nullptr, 0, 0);                    // 006C09C6..006C09C8
        } else {
            gui_load_listbox_set_selected_0041c040(listbox, nullptr, -1, 0);                   // 006C09D2..006C09D4
            text_literal(self, 0xCA8, k_empty_0073d71c);                                       // 006C09D9..006C0A05 LBL_COST_VALUE
            text_literal(self, 0xF28, k_empty_0073d71c);                                       // 006C0A0A..006C0A35 LBL_STOCK_VALUE
            text_literal(self, 0xA28, k_empty_0073d71c);                                       // 006C0A3A..006C0A66 description label
        }
    }
    zero(self, 0x2274);                                                                        // 006C0A70
}                                                                                              // 006C0A6B..006C0A82 free(entries)

// 006C1840 CSWGuiStore::PopulateStoreItemListBox() RET 0: the store's items grouped by base-item category.
void __fastcall kotor_fastcall_006c1840(void* self, void*) {
    void* repository = lp(store_of(self), 0x230);                                              // 006C1863..006C1872 (store unchecked)
    PtrList entries;                                                                           // 006C1878..006C1882 [esp+28] state 0
    I32 next = 0;                                                                              // [esp+20]
    U32 made = 0, lowest = 0, found = 0, current = 0;                                          // [esp+18] [esp+14] [esp+1C] ebp
    if (lw(repository, 0x10) != 0) {                                                           // 006C1886..006C18A1
        do {
            for (U32 i = 0; i < lw(repository, 0x10); ++i) {                                   // 006C18A7..006C1925
                if (found != 0) break;                                                         // 006C18B3..006C18B7
                void* item = w2x_inventory_item_005560b0(repository, nullptr, i);              // 006C18B9..006C18C0
                if (item == nullptr || lw(item, 0x108) != 0) continue;                         // 006C18C7..006C18D3
                const U32 category = lb(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0x30); // 006C18D5..006C18DC
                if (category == current) {                                                     // 006C18E0..006C18E2
                    found = kotor_fastcall_006c1660(self, nullptr, &next, &entries, lw(item, 0x14), 0); // 006C18E4..006C18FB
                    ++made;                                                                    // 006C18FF
                } else if (static_cast<I32>(category) > static_cast<I32>(current)) {           // 006C1905 (jle)
                    if (lowest == current || category < lowest) lowest = category;            // 006C1907..006C1913
                }
            }
            current = lowest;                                                                  // 006C1930
        } while (made < lw(repository, 0x10));                                                 // 006C1929..006C1934 (jb; quirk: all items)
    }
    void* listbox = at(self, 0x1480);                                                          // 006C1942 LB_SHOPITEMS
    kotor_fastcall_0041c1d0(listbox, nullptr, &entries, 0, 1, 0);                              // 006C193A..006C194B AddControls
    if ((lb(self, 0x14C4) & 2) != 0) {                                                         // 006C1950..006C195A
        if (entries.size > 0) {                                                                // 006C1960..006C196A
            gui_load_listbox_set_selected_0041c040(listbox, nullptr, 0, 0);                    // 006C196C..006C196E
        } else {
            gui_load_listbox_set_selected_0041c040(listbox, nullptr, -1, 0);                   // 006C1978..006C197A
            text_literal(self, 0xCA8, k_empty_0073d71c);                                       // 006C197F..006C19AB
            text_literal(self, 0xF28, k_empty_0073d71c);                                       // 006C19B0..006C19DB
            text_literal(self, 0xA28, k_empty_0073d71c);                                       // 006C19E0..006C1A0C
        }
    }
    zero(self, 0x2274);                                                                        // 006C1A16
}                                                                                              // 006C1A11..006C1A28 free(entries)

// 006C13C0 CSWGuiStore::ShowSellGUI() RET 0.
void __fastcall kotor_fastcall_006c13c0(void* self, void*) {
    sw(self, 0x17A4, lw(self, 0x17A4) | 2u);                                                   // 006C13C3..006C13D3 LB_INVITEMS visible
    sw(self, 0x14C4, lw(self, 0x14C4) & 0xFFFFFFFDu);                                          // 006C13D6..006C13EA LB_SHOPITEMS hidden
    strref(self, 0x2A8, 0x7D82);                                                               // 006C13DF..006C13F0
    strref(self, 0x2214, 0x7D82);                                                              // 006C13F5..006C1400
    gui_load_control_add_event_0041ab20(at(self, 0x20A8), nullptr, 0x27, self, reinterpret_cast<const void*>(&kotor_fastcall_006c0f40)); // 006C1405..006C1413
    strref(self, 0x1428, 0x7D82);                                                              // 006C1418..006C1423
    strref(self, 0x7A8, 0xA3D1);                                                               // 006C1428..006C1433
    strref(self, 0x2050, 0xA3D1);                                                              // 006C1438..006C1443
    strref(self, 0xB68, 0xA3D9);                                                               // 006C1448..006C1453
    vcall<void, void*, U32>(self, 0x08, at(self, 0x1760), 0);                                  // 006C1458..006C145F
    kotor_fastcall_006c0850(self, nullptr);                                                    // 006C1466 jmp PopulateInventoryItemListBox
}
// 006C1A50 CSWGuiStore::ShowBuyGUI() RET 0.
void __fastcall kotor_fastcall_006c1a50(void* self, void*) {
    sw(self, 0x17A4, lw(self, 0x17A4) & 0xFFFFFFFDu);                                          // 006C1A53..006C1A5D
    sw(self, 0x14C4, lw(self, 0x14C4) | 2u);                                                   // 006C1A63..006C1A7D
    strref(self, 0x2A8, 0x7D84);                                                               // 006C1A72..006C1A80
    strref(self, 0x2214, 0x7D84);                                                              // 006C1A85..006C1A90
    gui_load_control_add_event_0041ab20(at(self, 0x20A8), nullptr, 0x27, self, reinterpret_cast<const void*>(&kotor_fastcall_006c1130)); // 006C1A95..006C1AA3
    strref(self, 0x1428, 0x7D84);                                                              // 006C1AA8..006C1AB3
    strref(self, 0x7A8, 0xA3D2);                                                               // 006C1AB8..006C1AC3
    strref(self, 0x2050, 0xA3D2);                                                              // 006C1AC8..006C1AD3
    strref(self, 0xB68, 0xA3D7);                                                               // 006C1AD8..006C1AE3
    vcall<void, void*, U32>(self, 0x08, at(self, 0x1480), 0);                                  // 006C1AE8..006C1AEF
    kotor_fastcall_006c1840(self, nullptr);                                                    // 006C1AF6 jmp PopulateStoreItemListBox
}
// 006C1B50 CSWGuiStore::InitializeStoreType() RET 0.
void __fastcall kotor_fastcall_006c1b50(void* self, void*) {
    void* store = store_of(self);                                                              // 006C1B50..006C1B62
    if (store == nullptr) return;                                                              // 006C1B67..006C1B69
    switch (lb(store, 0x256)) {                                                                // 006C1B6F..006C1B7D
    case 1:                                                                                    // 006C1BCD
        for (U32 off : {0x844u, 0x704u, 0x1F28u}) sw(self, off, lw(self, off) & 0xFFFFFFFDu);
        kotor_fastcall_006c1a50(self, nullptr);                                                // 006C1BEF jmp ShowBuyGUI
        return;
    case 2:                                                                                    // 006C1BA6
        for (U32 off : {0x844u, 0x704u, 0x1F28u}) sw(self, off, lw(self, off) & 0xFFFFFFFDu);
        kotor_fastcall_006c13c0(self, nullptr);                                                // 006C1BC8 jmp ShowSellGUI
        return;
    case 3:                                                                                    // 006C1B7F
        for (U32 off : {0x844u, 0x704u, 0x1F28u}) sw(self, off, lw(self, off) | 2u);
        kotor_fastcall_006c1a50(self, nullptr);                                                // 006C1BA1 jmp ShowBuyGUI
        return;
    default: return;                                                                           // 006C1BF4
    }
}

// 006C0AA0 CSWGuiStore::OnControlEntered(CSWGuiControl* control) RET 4 (event 0 of every item entry).
void __fastcall kotor_fastcall_006c0aa0(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                        // 006C0ABF..006C0AC4
    void* server = server_app();                                                               // 006C0ACA..006C0AD7 edi
    void* item = recovered_item_lookup_004ae760(server, nullptr, kotor_fastcall_004aea30(server, nullptr, lw(control, 0x1C4))); // 006C0AD0..006C0AE5
    if (item == nullptr) return;                                                               // 006C0AEA..006C0AEE
    const U32 value = (lb(self, 0x14C4) & 2) != 0 ? kotor_fastcall_006c0790(self, nullptr, item)   // 006C0AF4..006C0B00 GetItemBuyValue
                                                  : kotor_fastcall_006c07f0(self, nullptr, item);  // 006C0B19 GetItemSellValue
    text_int(self, 0xCA8, static_cast<I32>(value));                                            // 006C0B05..006C0B48 LBL_COST_VALUE
    if ((lb(item, 0x288) & 4) != 0) strref(self, 0xF28, 0xA3DF);                               // 006C0B4D..006C0B61 infinite
    else text_int(self, 0xF28, li(item, 0x28C));                                               // 006C0B68..006C0B98 stack size
    Str description;
    void* d = kotor_fastcall_0055f340(item, nullptr, description.s); description.live = true;  // 006C0B9D..006C0BA4
    kotor_fastcall_006c0690(self, nullptr, d);                                                 // 006C0BA9..006C0BB4 SetDescription
}                                                                                              // 006C0BB9..006C0BC5

// 006C0BE0 CSWGuiStore::BuyItem(CSWSItem* item) RET 4.
void __fastcall kotor_fastcall_006c0be0(void* self, void*, void* item) {
    void* server = server_app();                                                               // 006C0BEE..006C0C00 edi
    void* player = kotor_fastcall_004ae770(server, nullptr, kotor_fastcall_004aea40(server, nullptr)); // 006C0C05..006C0C0F
    void* store = store_of(self);                                                              // 006C0C14..006C0C26 ebp
    if (player == nullptr || store == nullptr) return;                                         // 006C0C2B..006C0C37
    if (recovered_creature_inventory_004ef770(player, nullptr, 1) == nullptr) return;           // 006C0C3D..006C0C48
    if (item == nullptr) return;                                                               // 006C0C4F..006C0C55
    play_sound(self, 9);                                                                       // 006C0C5B..006C0C60
    const U32 gold = lw(self, 0x2270) - kotor_fastcall_006c0790(self, nullptr, item);           // 006C0C65..006C0C73
    sw(self, 0x2270, gold);                                                                    // 006C0C7A
    kotor_fastcall_004edd90(player, nullptr, static_cast<I32>(gold));                          // 006C0C77..006C0C80 SetGold
    kotor_fastcall_005c6f70(store, nullptr, item, player);                                     // 006C0C85..006C0C89 CSWSStore::SellItem
    I32 selected = static_cast<std::int16_t>(lh(self, 0x1746));                                // 006C0C8E
    const I32 top = static_cast<std::int16_t>(lh(self, 0x1748));                               // 006C0C95
    kotor_fastcall_006c1840(self, nullptr);                                                    // 006C0C9C..006C0C9E
    if (selected >= li(self, 0x1720)) selected = li(self, 0x1720) - 1;                         // 006C0CA3..006C0CAD
    void* listbox = at(self, 0x1480);                                                          // 006C0CB0
    kotor_fastcall_006b2590(listbox, nullptr, static_cast<U32>(top));                          // 006C0CB6..006C0CB9 SetTopVisible
    gui_load_listbox_set_selected_0041c040(listbox, nullptr, selected, 0);                     // 006C0CBE..006C0CC3
    text_int(self, 0x12E8, li(self, 0x2270));                                                  // 006C0CC8..006C0CF8
}

// 006C0D20 CSWGuiStore::BuyItemCallback(CSWGuiControl* control) RET 4 (message-box callback).
void __fastcall kotor_fastcall_006c0d20(void* self, void*, void* control) {
    if ((lb(lp(ingame_gui(), 0x98), 0x64) & 1) == 0) return;                                   // 006C0D20..006C0D3A
    void* server = server_app();                                                               // 006C0D3C..006C0D4D
    void* item = recovered_item_lookup_004ae760(server, nullptr, kotor_fastcall_004aea30(server, nullptr, lw(control, 0x1C4))); // 006C0D42..006C0D5B
    if (item != nullptr) kotor_fastcall_006c0be0(self, nullptr, item);                         // 006C0D60..006C0D68
}

// 006C0D80 CSWGuiStore::SellItem(CSWSItem* item) RET 4.
void __fastcall kotor_fastcall_006c0d80(void* self, void*, void* item) {
    void* server = server_app();                                                               // 006C0D8E..006C0DA1 edi
    void* player = kotor_fastcall_004ae770(server, nullptr, kotor_fastcall_004aea40(server, nullptr)); // 006C0DA6..006C0DB0 ebx
    void* store = store_of(self);                                                              // 006C0DB5..006C0DC7 ebp
    if (item == nullptr || player == nullptr || store == nullptr) return;                      // 006C0DCC..006C0DE4
    void* repository = recovered_creature_inventory_004ef770(player, nullptr, 1);              // 006C0DEA..006C0DEE [esp+28]
    if (repository == nullptr) return;                                                         // 006C0DF3..006C0DF9
    play_sound(self, 0xB);                                                                     // 006C0DFF..006C0E04
    const U32 gold = lw(self, 0x2270) + kotor_fastcall_006c07f0(self, nullptr, item);           // 006C0E09..006C0E17
    sw(self, 0x2270, gold);                                                                    // 006C0E1E
    kotor_fastcall_004edd90(player, nullptr, static_cast<I32>(gold));                          // 006C0E1B..006C0E24 SetGold
    void* sold;
    if (li(item, 0x28C) > 1) {                                                                 // 006C0E29..006C0E30
        sold = kotor_fastcall_0055f280(item, nullptr, 1);                                      // 006C0E32..006C0E36 SplitItem(1)
    } else {
        recovered_remove_item_00555fd0(repository, nullptr, item);                             // 006C0E3E..006C0E43
        sold = item;
    }
    recovered_store_acquire_item_005c7610(store, nullptr, sold);                               // 006C0E49..006C0E4B
    I32 selected = static_cast<std::int16_t>(lh(self, 0x1A26));                                // 006C0E50
    const I32 top = static_cast<std::int16_t>(lh(self, 0x1A28));                               // 006C0E57
    kotor_fastcall_006c0850(self, nullptr);                                                    // 006C0E5E..006C0E60
    if (selected >= li(self, 0x1A00)) selected = li(self, 0x1A00) - 1;                         // 006C0E65..006C0E6F
    void* listbox = at(self, 0x1760);                                                          // 006C0E72
    kotor_fastcall_006b2590(listbox, nullptr, static_cast<U32>(top));                          // 006C0E78..006C0E7B SetTopVisible
    gui_load_listbox_set_selected_0041c040(listbox, nullptr, selected, 0);                     // 006C0E80..006C0E85
    text_int(self, 0x12E8, li(self, 0x2270));                                                  // 006C0E8A..006C0EBA
}

// 006C0EE0 CSWGuiStore::SellItemCallback(CSWGuiControl* control) RET 4 (message-box callback).
void __fastcall kotor_fastcall_006c0ee0(void* self, void*, void* control) {
    if ((lb(lp(ingame_gui(), 0x98), 0x64) & 1) == 0) return;                                   // 006C0EE0..006C0EFA
    void* server = server_app();                                                               // 006C0EFC..006C0F0D
    void* item = recovered_item_lookup_004ae760(server, nullptr, kotor_fastcall_004aea30(server, nullptr, lw(control, 0x1C4))); // 006C0F02..006C0F1B
    if (item != nullptr) kotor_fastcall_006c0d80(self, nullptr, item);                         // 006C0F20..006C0F28
}

// 006C0F40 CSWGuiStore::OnControlInvAButton(CSWGuiControl* control) RET 4 (sell).
void __fastcall kotor_fastcall_006c0f40(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                        // 006C0F5A..006C0F67
    if (lw(control, 0x48) == 0x2D) play_sound(self, 0);                                        // 006C0F6D..006C0F78
    if (control == at(self, 0x20A8)) {                                                         // 006C0F7D..006C0F85 BTN_Accept
        control = kotor_m1_listbox_get_selected_control_0041a5d0(at(self, 0x1760), nullptr);   // 006C0F87..006C0F92
        if (control == nullptr) return;                                                        // 006C0F94..006C0F96
    }
    void* server = server_app();                                                               // 006C0F9C..006C0FA8 esi
    void* item = recovered_item_lookup_004ae760(server, nullptr, kotor_fastcall_004aea30(server, nullptr, lw(control, 0x1C4))); // 006C0FA2..006C0FB6
    if (item == nullptr) return;                                                               // 006C0FBB..006C0FBF
    void* server2 = server_app();                                                              // 006C0FC5..006C0FCB edi
    void* player = kotor_fastcall_004ae770(server2, nullptr, kotor_fastcall_004aea40(server2, nullptr)); // 006C0FCE..006C0FD8
    const U32 limit = level_limit(player, 0xFA, true);                                         // 006C0FDD..006C1014
    if (kotor_fastcall_006c07f0(self, nullptr, item) <= limit) {                               // 006C1019..006C1023 (jbe)
        kotor_fastcall_006c0d80(self, nullptr, item);                                          // 006C110E..006C1111 SellItem
        return;
    }
    confirm_box(self, limit, 0xA401, reinterpret_cast<const void*>(&kotor_fastcall_006c0ee0), control); // 006C1029..006C110C
}

// 006C1130 CSWGuiStore::OnControlStoreAButton(CSWGuiControl* control) RET 4 (buy).
void __fastcall kotor_fastcall_006c1130(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                        // 006C114D..006C1157
    if (lw(control, 0x48) != 0) play_sound(self, 0);                                           // 006C115D..006C1169
    if (control == at(self, 0x20A8)) {                                                         // 006C116E..006C1176 BTN_Accept
        control = kotor_m1_listbox_get_selected_control_0041a5d0(at(self, 0x1480), nullptr);   // 006C1178..006C1183
        if (control == nullptr) return;                                                        // 006C1185..006C1187
    }
    void* server = server_app();                                                               // 006C118D..006C1199 esi
    void* item = recovered_item_lookup_004ae760(server, nullptr, kotor_fastcall_004aea30(server, nullptr, lw(control, 0x1C4))); // 006C1193..006C11A7
    if (item == nullptr) return;                                                               // 006C11AC..006C11B0
    if (lw(self, 0x2270) < kotor_fastcall_006c0790(self, nullptr, item)) {                     // 006C11B6..006C11CA (jb)
        kotor_fastcall_0062d3e0(ingame_gui(), nullptr, 0xA3DE, 1);                             // 006C1301..006C1312 ShowExamineBox
        return;
    }
    void* server2 = server_app();                                                              // 006C11D0 edi
    void* player = kotor_fastcall_004ae770(server2, nullptr, kotor_fastcall_004aea40(server2, nullptr)); // 006C11D3..006C11DD
    const U32 limit = level_limit(player, 0x32, false);                                        // 006C11E2..006C11FD
    if (kotor_fastcall_006c0790(self, nullptr, item) <= limit) {                               // 006C1202..006C120C (jbe)
        kotor_fastcall_006c0be0(self, nullptr, item);                                          // 006C12F7..006C12FA BuyItem
        return;
    }
    confirm_box(self, limit, 0xA424, reinterpret_cast<const void*>(&kotor_fastcall_006c0d20), control); // 006C1212..006C12F5
}

// 006C1B00 CSWGuiStore::OnControlXButton(CSWGuiControl* control) RET 4: toggle buy / sell for a buy-and-sell store.
void __fastcall kotor_fastcall_006c1b00(void* self, void*, void* control) {
    if (control != nullptr && lw(control, 0x4C) == 0) return;                                  // 006C1B00..006C1B10
    void* store = store_of(self);                                                              // 006C1B12..006C1B21
    if (store == nullptr) return;                                                              // 006C1B26..006C1B28
    if (lb(store, 0x256) != 3) return;                                                         // 006C1B2A..006C1B31
    if ((lb(self, 0x14C4) & 2) != 0) kotor_fastcall_006c13c0(self, nullptr);                   // 006C1B33..006C1B3E ShowSellGUI
    else kotor_fastcall_006c1a50(self, nullptr);                                               // 006C1B47 ShowBuyGUI
}

// ---- CSWGuiStore slots
// 006C1650 OnPanelRemoved.
void __fastcall kotor_fastcall_006c1650(void* self, void*) {
    kotor_fastcall_006c1330(self, nullptr);                                                    // 006C1653 DeleteItemEntries
    gui_load_panel_on_removed_0040c170(self, nullptr);                                         // 006C165B jmp
}
// 006C2170 OnPanelAdded.
void __fastcall kotor_fastcall_006c2170(void* self, void*) {
    kotor_fastcall_006c1b50(self, nullptr);                                                    // 006C2173 InitializeStoreType
    kotor_fastcall_006c0610(self, nullptr);                                                    // 006C2178..006C217A PopulateStore
    gui_load_panel_on_added_0040b870(self, nullptr);                                           // 006C217F..006C2181
    kotor_fastcall_006c1a50(self, nullptr);                                                    // 006C2186..006C2189 jmp ShowBuyGUI
}
// 006C2190 HandleInputEvent(int event, int down) RET 8 (base first). Jump table 006C2248 / bytes 006C225C.
void __fastcall kotor_fastcall_006c2190(void* self, void*, std::uint32_t event, std::uint32_t down) {
    gui_load_panel_handle_input_00409e60(self, nullptr, event, down);                          // 006C2197..006C219F
    if (down == 0 || event - 0x28u > 0x12u) return;                                            // 006C21A4..006C21B2
    switch (event) {
    case 0x28: case 0x2E:                                                                      // 006C21C6
        play_sound(self, 0);                                                                   // 006C21C6..006C21CB
        kotor_fastcall_0062e4a0(ingame_gui(), nullptr);                                        // 006C21D0..006C21E0 HideStoreGui
        return;
    case 0x3A: vcall<void, U32, U32>(at(self, 0x1A40), 0x3C, 0x32, 1); return;                 // 006C21EB..006C220A (tail)
    case 0x39: vcall<void, U32, U32>(at(self, 0x1A40), 0x3C, 0x31, 1); return;                 // 006C220D..006C222C (tail)
    case 0x29:                                                                                 // 006C222F
        play_sound(self, 0);                                                                   // 006C222F..006C2234
        kotor_fastcall_006c1b00(self, nullptr, nullptr);                                       // 006C2239..006C223D OnControlXButton(0)
        return;
    default: return;
    }
}
}  // extern "C"
