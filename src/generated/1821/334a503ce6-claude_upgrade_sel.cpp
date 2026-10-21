// Lane W4 GUI successor (worker "upgrade", fork A1): CGuiInGame::ShowUpgradeScreen / HideUpgradeScreen, the
// CSWGuiUpgradeSelection panel (0x1158, method table 007571B0), the CSWUpgradeItemEntry control (0x3A4, method table
// 00757108) and the CSWGuiRadioButton constructor the selection panel's button array uses.
//   0062E760 CGuiInGame::ShowUpgradeScreen(ulong item_id)      thiscall RET 4
//   0062E870 CGuiInGame::HideUpgradeScreen()                   thiscall RET
//   006B66F0 CSWGuiRadioButton::CSWGuiRadioButton()            thiscall RET (returns this)
//   006C3E00 CSWUpgradeItemEntry::CSWUpgradeItemEntry()        thiscall RET (returns this)
//   006C5AD0 ~CSWUpgradeItemEntry                              thiscall RET      006C5AB0 deleting dtor RET 4
//   006C2650 SetExtent(const CSWGuiExtent&) RET 4   006C27C0 Draw(float) RET 4
//   006C2830 Initialize(CSWGuiText*, CSWGuiBorder*, CSWGuiBorder*, int width) RET 10h   (slot +A0)
//   006C3EA0 SetItem(ulong item_id, int equipped, ulong owner_id, ulong slot) RET 10h
//   006C78D0 CSWGuiUpgradeSelection(CSWGuiManager*) RET 4      006C4450 dtor RET   006C5B50 deleting dtor RET 4
//   006C2AB0 GetUpgradableItems(CSWSCreature*, int* available, C2DA*) RET 0Ch
//   006C2B10 HandleInputEvent(int, int) RET 8  (slot +3C)      006C4520 OnPanelAdded RET (slot +48)
//   006C2B60 OnTypeSelected(CSWGuiControl*) RET 4              006C2BD0 OnEnter(CSWGuiControl*) RET 4
// Transcribed from the original swkotor.exe linear sweep (read-only); each statement carries its original PCs.
#include "52a797cc9c-claude_upgrade.hpp"

namespace frame_control { extern std::uint32_t sound_manager_007a39ec; }                         // [007A39EC] CExoSound*
namespace resource_ref_assignment {                                                               // CResRef::operator=(const CResRef&) (by-value return)
menu_resources::Ref* __fastcall assign_00406120(menu_resources::Ref*, void*, menu_resources::Ref* out, const menu_resources::Ref* source);
}

extern "C" {
// ---- GUI
extern const unsigned char kotor_m1_radio_button_methods_007566d8[];                             // CSWGuiRadioButton (40 slots)
void __fastcall kotor_m1_radio_button_destruct_006be360(void* self, void*);                      // ~CSWGuiRadioButton
void* __fastcall gui_load_border_construct_004167f0(void* self, void*) noexcept(false);         // CSWGuiBorder()
void __fastcall gui_load_border_destruct_004151c0(void* self, void*);                           // ~CSWGuiBorder
void* __fastcall gui_load_text_construct_00417280(void* self, void*) noexcept(false);           // CSWGuiText()
void __fastcall gui_load_text_destruct_00415ff0(void* self, void*);                             // ~CSWGuiText
void* __fastcall gui_load_border_params_assign_004150c0(void* self, void*, const void* source);  // CSWGuiBorderParams::operator=
void* __fastcall gui_load_border_params_construct_00415000(void* self, void*, const void* corner, const void* edge,
    const void* fill, w4::U32 fill_style, w4::I32 dimension, float f0c, const void* color, w4::I32 inner) noexcept(false);
void* __fastcall gui_load_border_inner_extent_00415240(void* border, void*, void* out);          // CSWGuiBorder::GetInnerExtent
void* __cdecl gui_load_extent_intersection_0040a4a0(void* out, const void* a, const void* b);    // CSWGuiExtent::Intersection
void* __fastcall kotor_fastcall_00417120(void* self, void*, void* text, const void* font, const void* color, w4::U32 align, w4::U32 alpha_bits); // CSWGuiTextParams ctor
void* __fastcall kotor_fastcall_004171e0(void* self, void*, const void* source) noexcept(false); // CSWGuiTextParams::operator=
void __fastcall gui_load_slot_ret8_005b5e90(void*, void*, w4::U32, w4::U32);                     // 005B5E90 ret 8
void __fastcall light_manager_void_return_16_0063cfc0(void*, void*, w4::U32, w4::U32, w4::U32, w4::U32); // 0063CFC0 ret 10h
void* __fastcall c2da_default_ref_00405ed0(void* self, void*);                                   // CResRef::CResRef()
menu_resources::Ref* __fastcall unresolved_ref_assign_00406290(menu_resources::Ref*, void*, menu_resources::Ref* out, const char* text); // CResRef::operator=(const char*)
w4::I32 __fastcall async_string_length_005e5790(void* string, void*);                            // CExoString::GetLength
// ---- client / server
void* __fastcall kotor_fastcall_005ed6f0(void* app, void*);                                     // CClientExoApp::GetSWCMessage
w4::U32 __fastcall frame_TogglePauseRequest_00677800(void* message, void*);                     // SendPlayerToServerInput_TogglePauseRequest
extern const std::uint32_t kotor_rdata_sound_mode_0074c5fc;                                    // .rdata 0074C5FC
w4::U32 __fastcall kotor_fastcall_005ed4c0(void* app, void*);                                    // CClientExoApp::GetClientLanguage
w4::U32 __fastcall exo_locstring_get_005ea130(void* loc, void*, w4::U32 language, void* out, w4::U32 gender); // CExoLocString::GetString
menu_resources::Ref* __fastcall kotor_fastcall_005556b0(void* item, void*, menu_resources::Ref* out); // CSWSItem::GetIcon
void* __fastcall recovered_equipment_item_005a4c20(void* inventory, void*, w4::U32 slot);       // CSWInventory::GetItemInSlot
void* __fastcall recovered_server_player_list_004aed90(void* server, void*);                    // CServerExoApp::GetPlayerList
void* __fastcall recovered_party_repository_00563340(void* table, void*);                       // CSWPartyTable::GetPartyItems
w4::U32 __fastcall recovered_party_npc_selectable_005637c0(void* table, void*, w4::U32 npc);    // GetNPCSelectability (AL)
void* __fastcall w2x_inventory_item_005560b0(void* repository, void*, w4::U32 index);           // CItemRepository::ItemListGetItem
// ---- C2DA
void* __fastcall c2da_current_construct_00413cc0(void* self, void*, menu_resources::Ref name, w4::I32 mode) noexcept(false); // C2DA::C2DA
w4::I32 __fastcall c2da_current_load_004143b0(void* table, void*);                              // C2DA::Load2DArray
}

using namespace gchar;

namespace {
using SS = kotor_reconstruction::StartupString;
inline SS* ss(void* p) { return static_cast<SS*>(p); }
const char k_empty_0073d71c[] = "";                                                              // 0073D71C
const char k_int_0073d720[] = "%d";                                                              // 0073D720
inline void copy12(void* dst, const void* src) { std::memcpy(dst, src, 12); }
inline void fill_named(void* self, U32 params, const char* name) {
    menu_resources::Ref ref; ref.construct_00406d80(name);                                     // CResRef(const char*)
    gui_load_border_params_set_fill_00414c00(at(self, params), nullptr, &ref, 0);              // SetFillImage(ref, 0)
}
// [control+44h] = ([control+44h] & ~300h) | 400h
inline void hide_flags(void* control) { sw(control, 0x44, (lw(control, 0x44) & 0xFFFFFCFFu) | 0x400u); }

// CSWUpgradeItemEntry method table 00757108 (41 slots): CSWGuiButton 0073E658 (40 slots) plus the slots where
// 00757108 differs (+00 +04 +38 +98 +9C) and the new +A0 (Initialize).
const Override ovr_upgrade_item_entry[] = {
    {0x00, fn(&kotor_fastcall_006c5ab0)},                   // 006C5AB0 ~CSWUpgradeItemEntry (deleting)
    {0x04, fn(&kotor_fastcall_006c2650)},                   // 006C2650 SetExtent
    {0x38, fn(&kotor_fastcall_006c27c0)},                   // 006C27C0 Draw
    {0x98, fn(&gui_load_slot_ret8_005b5e90)},               // 005B5E90 (ret 8)
    {0x9C, fn(&light_manager_void_return_16_0063cfc0)},     // 0063CFC0 (ret 10h)
    {0xA0, fn(&kotor_fastcall_006c2830)},                   // 006C2830 Initialize
};
void* table_upgrade_item_entry[41];
bool built_upgrade_item_entry = false;
const void* vt_upgrade_item_entry() {
    if (!built_upgrade_item_entry) {
        vt_build(table_upgrade_item_entry, 41, gui_load_button_methods_0073e658, 40, ovr_upgrade_item_entry, 6);
        built_upgrade_item_entry = true;
    }
    return table_upgrade_item_entry;
}

// CSWGuiUpgradeSelection method table 007571B0 (27 slots; base CSWGuiPanel 0073E010).
const Override ovr_upgrade_selection[] = {
    {0x00, fn(&kotor_fastcall_006c5b50)},                   // 006C5B50 scalar deleting destructor
    {0x3C, fn(&kotor_fastcall_006c2b10)},                   // 006C2B10 HandleInputEvent
    {0x48, fn(&kotor_fastcall_006c4520)},                   // 006C4520 OnPanelAdded
};
void* table_upgrade_selection[27];
bool built_upgrade_selection = false;
const void* vt_upgrade_selection() {
    if (!built_upgrade_selection) {
        vt_build(table_upgrade_selection, 27, gui_load_panel_methods_0073e010, 27, ovr_upgrade_selection, 3);
        built_upgrade_selection = true;
    }
    return table_upgrade_selection;
}

// Text params of SetItem's count label: CSWGuiTextParams(text, "fnt_d16x16", [007A23B4], 22h, 1.0f) assigned to
// +34Ch, then destroyed (its first member is the CExoString text; ~CExoString is the whole temporary destructor).
void count_text_params(void* self, void* text) {
    menu_resources::Ref font; font.construct_00406d80("fnt_d16x16");                           // 00753DA0
    U32 params[0x70 / 4];
    kotor_fastcall_00417120(params, nullptr, text, &font, borrowed_exit_color_007a23b4, 0x22, 0x3F800000u);
    {
        auto g = on_throw([&]() { frame_LogStringDtor_005e5c20(params, nullptr); });
        kotor_fastcall_004171e0(at(self, 0x34C), nullptr, params);
        g.armed = false;
    }
    frame_LogStringDtor_005e5c20(params, nullptr);
}
// The two "no item" tails of SetItem (006C4201..006C4406 / 006C4301..006C4406): hex 3 fills, GUI string 16Bh as the
// name, empty count text, icon "inone".
void no_item(void* self, menu_resources::Ref* icon) {
    fill_named(self, 0x1EC, "lbl_hex_3");                                                      // 00755EBC
    fill_named(self, 0x260, "lbl_hex_3");
    {
        Str text; void* s = gui_string(text, 0x16B);                                           // GetGUIString(16Bh)
        gui_load_text_params_set_text_00415e00(at(self, 0x16C), nullptr, s);                   // SetText(+16Ch)
    }
    {
        Str empty; empty.empty();                                                              // CExoString()
        count_text_params(self, empty.p());
    }
    menu_resources::Ref discarded;
    unresolved_ref_assign_00406290(icon, nullptr, &discarded, "inone");                        // 00755EC8
}
}  // namespace

extern "C" {
// ======================= CGuiInGame
// 0062E760 CGuiInGame::ShowUpgradeScreen(ulong item_id) RET 4.
void __fastcall kotor_fastcall_0062e760(void* gui, void*, w4::U32 item_id) {
    if (lw(gui, 0x108) == 0) return;                                                           // 0062E778..0062E781
    if (lw(gui, 0x38) == 0) return;                                                            // 0062E787..0062E78C
    if (lw(gui, 0x88) != 0) return;                                                            // 0062E792..0062E79A
    if (item_id != 0x7F000000u &&                                                              // 0062E7A0..0062E7AA
        recovered_item_lookup_004ae760(server_app(), nullptr, item_id) == nullptr) return;     // 0062E7AC..0062E7BC
    frame_R4_005d5e80(pt(frame_control::sound_manager_007a39ec), nullptr, kotor_rdata_sound_mode_0074c5fc); // 0062E7C2..0062E7CF
    if (lw(gui, 0xB38) == 0)                                                                   // 0062E7D4..0062E7DC
        (void)frame_TogglePauseRequest_00677800(kotor_fastcall_005ed6f0(client_app(), nullptr), nullptr); // 0062E7DE..0062E7EE
    void* raw = live_operator_new(0x1158);                                                     // 0062E7F3..0062E7FD
    void* panel = nullptr;
    if (raw != nullptr) {                                                                      // 0062E804..0062E80E
        auto guard = on_throw([&]() { live_free(raw); });                                      // EH state 0
        panel = kotor_fastcall_006c78d0(raw, nullptr, lp(gui, 0x38));                          // 0062E810..0062E816
        guard.armed = false;
    }
    sw(gui, 0x88, ad(panel));                                                                  // 0062E81F
    raw_store(panel, 0x113C, item_id);                                                         // 0062E827 (unchecked)
    kotor_fastcall_0040bc70(lp(gui, 0x38), nullptr, lp(gui, 0x88), 3, 1);                      // 0062E825..0062E841 AddPanel(panel, 3, 1)
    gui_m48_client_set_input_class_005eda60(client_app(), nullptr, 2, 1);                      // 0062E846..0062E852 SetInputClass(2, 1)
}

// 0062E870 CGuiInGame::HideUpgradeScreen() RET.
void __fastcall kotor_fastcall_0062e870(void* gui, void*) {
    if (lw(gui, 0x108) == 0) return;                                                           // 0062E873..0062E87B
    if (lw(gui, 0x38) == 0) return;                                                            // 0062E87D..0062E882
    if (lw(gui, 0x88) == 0) return;                                                            // 0062E884..0062E88C
    if (lw(gui, 0xB38) == 0)                                                                   // 0062E88E..0062E896
        (void)frame_TogglePauseRequest_00677800(kotor_fastcall_005ed6f0(client_app(), nullptr), nullptr); // 0062E898..0062E8A7
    gui_m48_client_set_input_class_005eda60(client_app(), nullptr, 0, 1);                      // 0062E8AC..0062E8B9 SetInputClass(0, 1)
    frame_R4_005d5e80(pt(frame_control::sound_manager_007a39ec), nullptr, kotor_rdata_sound_mode_0074c5ec); // 0062E8BE..0062E8CB
    hide_flags(lp(gui, 0x88));                                                                 // 0062E8D0..0062E8E5
    zero(gui, 0x88);                                                                           // 0062E8E8
}

// ======================= CSWGuiRadioButton
// 006B66F0 CSWGuiRadioButton::CSWGuiRadioButton() (returns this).
void* __fastcall kotor_fastcall_006b66f0(void* self, void*) noexcept(false) {
    gui_load_button_construct_0041c0f0(self, nullptr);                                         // 006B66F3
    set_vptr(self, kotor_m1_radio_button_methods_007566d8);                                    // 006B66F8 (007566D8)
    return self;                                                                               // 006B66FE
}

// ======================= CSWUpgradeItemEntry
// 006C3E00 CSWUpgradeItemEntry::CSWUpgradeItemEntry() (returns this).
void* __fastcall kotor_fastcall_006c3e00(void* self, void*) noexcept(false) {
    gui_load_button_construct_0041c0f0(self, nullptr);                                         // 006C3E1D
    Unwind u; u.push(&gui_load_button_destruct_00419da0, self);                                // EH state 0
    set_vptr(self, vt_upgrade_item_entry());                                                   // 006C3E30 (00757108)
    for (U32 off : {0x1D8u, 0x24Cu, 0x2C0u}) {                                                 // 006C3E22..006C3E56 states 1..3
        gui_load_border_construct_004167f0(at(self, off), nullptr); u.push(&gui_load_border_destruct_004151c0, at(self, off));
    }
    gui_load_text_construct_00417280(at(self, 0x334), nullptr);                                // 006C3E5B..006C3E66
    u.dismiss();
    sw(self, 0x1D0, lw(self, 0x1D0) & 0xFFFFFFFEu);                                            // 006C3E6B..006C3E78
    sw(self, 0x1C4, 0x7F000000u);                                                              // 006C3E7E
    sb(self, 0x1D4, 0xFF);                                                                     // 006C3E88
    return self;                                                                               // 006C3E8F
}
// 006C5AD0 ~CSWUpgradeItemEntry (members in reverse, then ~CSWGuiButton; no vptr store).
void __fastcall kotor_fastcall_006c5ad0(void* self, void*) {
    gui_load_text_destruct_00415ff0(at(self, 0x334), nullptr);                                 // 006C5AED..006C5AFB
    gui_load_border_destruct_004151c0(at(self, 0x2C0), nullptr);                               // 006C5B00..006C5B0B
    gui_load_border_destruct_004151c0(at(self, 0x24C), nullptr);                               // 006C5B10..006C5B1B
    gui_load_border_destruct_004151c0(at(self, 0x1D8), nullptr);                               // 006C5B20..006C5B2B
    gui_load_button_destruct_00419da0(self, nullptr);                                          // 006C5B30..006C5B3A
}
// 006C5AB0 scalar deleting destructor (thiscall(flags) RET 4).
void* __fastcall kotor_fastcall_006c5ab0(void* self, void*, w4::U32 flags) {
    return deleting(self, &kotor_fastcall_006c5ad0, flags);                                    // 006C5AB0..006C5ACB
}
// 006C2650 CSWUpgradeItemEntry::SetExtent(const CSWGuiExtent& extent) RET 4.
void __fastcall kotor_fastcall_006c2650(void* self, void*, const void* extent) {
    if (li(extent, 8) <= 0x38) return;                                                         // 006C265B..006C2665 (jle)
    std::memcpy(at(self, 4), extent, 16);                                                      // 006C266C..006C2684
    I32 icon[4] = {li(self, 4), li(self, 8), 0x38, 0x38};                                      // 006C2687..006C26AA [esp+10h]
    vcall<void, const void*>(at(self, 0x1D8), 0x04, icon);                                     // 006C269B..006C26AE
    vcall<void, const void*>(at(self, 0x24C), 0x04, icon);                                     // 006C26B1..006C26C2
    vcall<void, const void*>(at(self, 0x2C0), 0x04, icon);                                     // 006C26C5..006C26D6
    I32 count[4] = {icon[0], icon[1], icon[2], icon[3]};                                       // 006C26D9..006C26FB [esp+20h]
    const I32 length = async_string_length_005e5790(at(self, 0x34C), nullptr);                 // 006C26F1..006C26FF
    const I32 w = length <= 2 ? 0x15 : 0x2A;                                                   // 006C2704..006C2719 (setle/dec/and/add)
    count[3] = 0x38;                                                                           // 006C270E
    count[2] = w;                                                                              // 006C271D
    count[0] = count[0] + (0x38 - w);                                                          // 006C270C..006C2731
    vcall<void, const void*>(at(self, 0x334), 0x04, count);                                    // 006C2727..006C2738
    icon[2] = li(self, 0x0C) - 0x38;                                                           // 006C273B..006C2749
    icon[0] = icon[0] + 0x38;                                                                  // 006C273E..006C2757
    vcall<void, const void*>(at(self, 0x6C), 0x04, icon);                                      // 006C274D..006C275B
    vcall<void, const void*>(at(self, 0xE0), 0x04, icon);                                      // 006C275E..006C2771
    I32 inner_highlight[4], inner_border[4], clip[4];                                          // [esp+30h] / [esp+40h] / [esp+54h]
    void* b = gui_load_border_inner_extent_00415240(at(self, 0xE0), nullptr, inner_highlight); // 006C277C..006C2783
    void* a = gui_load_border_inner_extent_00415240(at(self, 0x6C), nullptr, inner_border);    // 006C2788..006C2790
    void* r = gui_load_extent_intersection_0040a4a0(clip, a, b);                               // 006C2795..006C27A0
    vcall<void, const void*>(at(self, 0x154), 0x04, r);                                        // 006C2774..006C27A6
}
// 006C27C0 CSWUpgradeItemEntry::Draw(float) RET 4.
void __fastcall kotor_fastcall_006c27c0(void* self, void*, w4::U32 delta_bits) {
    void* second;
    if ((lb(self, 0x44) & 1u) != 0) {                                                          // 006C27C3..006C27CD
        vcall<void, U32>(at(self, 0xE0), 0x0C, delta_bits); second = at(self, 0x24C);          // 006C27CF..006C27E4
    } else {
        vcall<void, U32>(at(self, 0x6C), 0x0C, delta_bits); second = at(self, 0x1D8);          // 006C27E6..006C27EF
    }
    vcall<void, U32>(second, 0x0C, delta_bits);                                                // 006C27F5..006C27F8
    vcall<void, U32>(at(self, 0x2C0), 0x0C, delta_bits);                                       // 006C27FB..006C2808
    vcall<void, U32>(at(self, 0x154), 0x0C, delta_bits);                                       // 006C280B..006C2818
    vcall<void, U32>(at(self, 0x334), 0x0C, delta_bits);                                       // 006C281B..006C2828
}
// 006C2830 CSWUpgradeItemEntry::Initialize(CSWGuiText* text, CSWGuiBorder* border, CSWGuiBorder* highlight, int width) RET 10h.
void __fastcall kotor_fastcall_006c2830(void* self, void*, void* text, void* border, void* highlight, w4::I32 width) {
    kotor_fastcall_004171e0(at(self, 0x16C), nullptr, at(text, 0x18));                         // 006C2836..006C284B
    gui_load_border_params_assign_004150c0(at(self, 0x80), nullptr, at(border, 0x14));          // 006C2850..006C2863
    copy12(at(self, 0x90), borrowed_exit_color_007a23b4);                                      // 006C2868..006C2888
    gui_load_border_params_assign_004150c0(at(self, 0xF4), nullptr, at(highlight, 0x14));       // 006C288B..006C2897
    copy12(at(self, 0x104), borrowed_enter_color_007a23c0);                                    // 006C289C..006C28BA
    gui_load_border_params_set_pulsing_00414d00(at(self, 0xF4), nullptr, 1, 1, 0);             // 006C28B6..006C28C1
    for (U32 pass = 0; pass < 2; ++pass) {                                                     // 006C28C6..006C2939 / 006C295A..006C29CD
        menu_resources::Ref fill, edge, corner;                                                // [esp+38h] "lbl_hex_3", [esp+18h] "", [esp+28h] ""
        fill.construct_00406d80("lbl_hex_3");                                                  // 00755EBC
        edge.construct_00406d80(k_empty_0073d71c);
        corner.construct_00406d80(k_empty_0073d71c);
        float color[3] = {1.0f, 1.0f, 1.0f};                                                   // [esp+8]
        alignas(4) unsigned char params[0x54];                                                 // [esp+48h] CSWGuiBorderParams
        void* p = gui_load_border_params_construct_00415000(params, nullptr, &corner, &edge, &fill, 0, 0, 1.0f, color, 0);
        gui_load_border_params_assign_004150c0(at(self, pass == 0 ? 0x1ECu : 0x260u), nullptr, p);
        if (pass == 0) copy12(at(self, 0x1FC), borrowed_exit_color_007a23b4);                  // 006C293E..006C2957
        else copy12(at(self, 0x270), borrowed_enter_color_007a23c0);                           // 006C29D2..006C29EF
    }
    gui_load_border_params_set_pulsing_00414d00(at(self, 0x260), nullptr, 1, 0, 1);            // 006C29EB..006C29F6
    I32 extent[4] = {0, 0, width, 0x38};                                                       // 006C29FB..006C2A1F [esp+8]
    vcall<void, const void*>(self, 0x04, extent);                                              // 006C2A02..006C2A27 SetExtent
}
// 006C3EA0 CSWUpgradeItemEntry::SetItem(ulong item_id, int equipped, ulong owner_id, ulong slot) RET 10h.
void __fastcall kotor_fastcall_006c3ea0(void* self, void*, w4::U32 item_id, w4::U32 equipped, w4::U32 owner_id, w4::U32 slot) {
    menu_resources::Ref icon; c2da_default_ref_00405ed0(&icon, nullptr);                       // 006C3EC6..006C3ECA [esp+4Ch]
    const U32 flags = lw(self, 0x1D0);                                                         // 006C3ECF..006C3ED8
    sw(self, 0x1D0, flags ^ ((flags ^ equipped) & 1u));                                        // 006C3EE1..006C3EF0
    sw(self, 0x1C4, item_id);                                                                  // 006C3EF9
    sw(self, 0x1C8, owner_id);                                                                 // 006C3EFF
    sw(self, 0x1CC, slot);                                                                     // 006C3F05
    if (item_id == 0x7F000000u) {                                                              // 006C3EE8..006C3F0B
        no_item(self, &icon);                                                                  // 006C4301..006C4406
    } else {
        void* client = client_app();                                                           // 006C3F11..006C3F1A edi
        void* item = recovered_item_lookup_004ae760(server_app(), nullptr, item_id);           // 006C3F17..006C3F1E
        if (item == nullptr) {                                                                 // 006C3F23..006C3F27
            no_item(self, &icon);                                                              // 006C4201..006C4406
        } else {
            Str name; name.empty();                                                            // 006C3F2D..006C3F31 [esp+1Ch] state 0
            exo_locstring_get_005ea130(at(item, 0x280), nullptr, kotor_fastcall_005ed4c0(client, nullptr), name.p(), 0); // 006C3F36..006C3F56
            if ((lb(self, 0x1D0) & 1) != 0) {                                                  // 006C3F5B..006C3F62
                Str close(")");                                                                // 006C3F68..006C3F71 (0074F788) [esp+3Ch] state 1
                Str open(" (");                                                                // 006C3F76..006C3F87 (00748EA4) [esp+44h] state 2
                Str gui; void* g = gui_string(gui, 0x7E5A);                                    // 006C3F8C..006C3FA6 [esp+0Ch] state 3
                Str t1; GuiConcat005e5d10(ss(name.p()), nullptr, ss(t1.s), ss(open.p())); t1.live = true; // 006C3FAD..006C3FC3 state 4
                Str t2; void* r2 = GuiConcat005e5d10(ss(t1.s), nullptr, ss(t2.s), ss(g)); t2.live = true;   // 006C3FC8..006C3FD8 state 5
                Str t3; void* r3 = GuiConcat005e5d10(ss(r2), nullptr, ss(t3.s), ss(close.p())); t3.live = true; // 006C3FDD..006C3FF1 state 6
                async_assign_string_005e5c50(name.p(), nullptr, r3);                           // 006C3FF6..006C4003
            }                                                                                  // 006C4008..006C4069 ~t3 ~t2 ~t1 ~gui ~open ~close
            gui_load_text_params_set_text_00415e00(at(self, 0x16C), nullptr, name.p());        // 006C406E..006C4079
            Str count; count.empty();                                                          // 006C407E..006C4082 [esp+24h] state 7
            const I32 stack = li(item, 0x28C);                                                 // 006C4087
            if (stack > 1) {                                                                   // 006C408D..006C409C
                const char* hex = stack >= 0x64 ? "lbl_hex_7" : "lbl_hex_6";                   // 006C40A2..006C40A5 (00756784 / 00756778)
                fill_named(self, 0x1EC, hex);                                                  // 006C40A7..006C40BE / 006C40DA..006C40F1
                fill_named(self, 0x260, hex);                                                  // 006C40C3..006C4111
                frame_LogFormat_005e5680(count.p(), k_int_0073d720, stack);                    // 006C4116..006C4126
            } else {
                fill_named(self, 0x1EC, "lbl_hex_3");                                          // 006C412B..006C4142 (00755EBC)
                fill_named(self, 0x260, "lbl_hex_3");                                          // 006C4147..006C4162
            }
            count_text_params(self, count.p());                                                // 006C4167..006C41B7 state 8
            menu_resources::Ref fetched, discarded;
            menu_resources::Ref* r = kotor_fastcall_005556b0(item, nullptr, &fetched);         // 006C41BC..006C41C3 GetIcon
            resource_ref_assignment::assign_00406120(&icon, nullptr, &discarded, r);           // 006C41C8..006C41D2
        }                                                                                      // 006C41D7..006C41F7 ~count ~name
    }
    sw(self, 0x2F0, (lw(self, 0x2F0) & 0xFFFFFFFEu) | 2u);                                     // 006C440B..006C4424
    gui_load_border_params_set_fill_00414c00(at(self, 0x2D4), nullptr, &icon, 0);              // 006C4411..006C4427
}

// ======================= CSWGuiUpgradeSelection
// 006C78D0 CSWGuiUpgradeSelection::CSWGuiUpgradeSelection(CSWGuiManager* manager) RET 4.
void* __fastcall kotor_fastcall_006c78d0(void* self, void*, void* manager) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                 // 006C78F2..006C78F7
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);                                 // state 0
    set_vptr(self, vt_upgrade_selection());                                                    // 006C7909 (007571B0)
    label(self, 0x64, u);                                                                      // 006C78FC..006C790F state 1
    button(self, 0x1A4, u);                                                                    // 006C7914..006C791F state 2
    button(self, 0x368, u);                                                                    // 006C7924..006C792F state 3
    vector_member(at(self, 0x52C), 0x1C4, 4, &kotor_fastcall_006b66f0,                         // 006C7934..006C7951 state 4
                  &kotor_m1_radio_button_destruct_006be360, u);
    vector_member(at(self, 0xC3C), 0x140, 4, &gui_load_label_construct_0041acd0,               // 006C7956..006C7973 state 5
                  &gui_load_label_destruct_00419d10, u);
    start_layout(self, "upgradesel");                                                          // 006C797A..006C7993 (007574C0)
    init_control(self, 0x64, "LBL_TITLE");                                                     // 006C7998..006C79C2 (00751A20)
    init_control(self, 0x52C, "BTN_LIGHTSABER");                                               // 006C79C7..006C79F1 (007574B0)
    init_control(self, 0x6F0, "BTN_RANGED");                                                   // 006C79F6..006C7A26 (007574A4)
    init_control(self, 0x8B4, "BTN_MELEE");                                                    // 006C7A2B..006C7A5B (00757498)
    init_control(self, 0xA78, "BTN_ARMOR");                                                    // 006C7A60..006C7A90 (0075748C)
    init_control(self, 0xC3C, "LBL_LSABER");                                                   // 006C7A95..006C7AC5 (00757480)
    init_control(self, 0xD7C, "LBL_RANGED");                                                   // 006C7ACA..006C7AFA (00757474)
    init_control(self, 0xEBC, "LBL_MELEE");                                                    // 006C7AFF..006C7B2F (00757468)
    init_control(self, 0xFFC, "LBL_ARMOR");                                                    // 006C7B34..006C7B64 (0075745C)
    init_control(self, 0x368, "BTN_BACK");                                                     // 006C7B69..006C7B99 (007548AC)
    init_control(self, 0x1A4, "BTN_UPGRADEITEMS");                                             // 006C7B9E..006C7BCE (00757448)
    stop_layout(self);                                                                         // 006C7BD3..006C7BD5
    sw(self, 0x113C, 0x7F000000u);                                                             // 006C7BDF
    void* raw = live_operator_new(0xC48);                                                      // 006C7BDA..006C7BEE
    void* select = nullptr;
    if (raw != nullptr) {                                                                      // 006C7BF5..006C7BFC
        auto guard = on_throw([&]() { live_free(raw); });                                      // state 11h
        select = kotor_fastcall_006c7630(raw, nullptr, manager);                               // 006C7BFE..006C7C05
        guard.armed = false;
    }
    sw(self, 0x1150, ad(select));                                                              // 006C7C12
    raw_store(select, 0xC44, ad(self));                                                        // 006C7C18 (unchecked)
    for (U32 i = 0; i < 4; ++i) {                                                              // 006C7C1E..006C7CB8
        void* radio = at(self, 0x52C + i * 0x1C4);                                             // edi
        gui_load_control_add_event_0041ab20(radio, nullptr, 0x27, self, fn(&kotor_fastcall_006c2b60)); // 006C7C26..006C7C30
        gui_load_control_add_event_0041ab20(radio, nullptr, 0x2D, self, fn(&kotor_fastcall_006c2b60)); // 006C7C35..006C7C3F
        gui_load_control_add_event_0041ab20(radio, nullptr, 0, self, fn(&kotor_fastcall_006c2bd0));    // 006C7C44..006C7C4E
        sw(radio, 0x58, i);                                                                    // 006C7C53
        copy12(at(radio, 0x90), borrowed_exit_color_007a23b4);                                 // 006C7C56..006C7C71
        copy12(at(radio, 0x104), borrowed_enter_color_007a23c0);                               // 006C7C74..006C7C94
        gui_load_border_params_set_pulsing_00414d00(at(radio, 0xF4), nullptr, 1, 1, 0);        // 006C7C90..006C7C9F
        void* lbl = at(self, 0xC3C + i * 0x140);                                               // ebp = label + 44h
        sw(lbl, 0x44, lw(lbl, 0x44) & 0xFFFFFFFDu);                                            // 006C7CA4
    }
    sw(self, 0x3AC, lw(self, 0x3AC) & 0xFFFFFFFBu);                                            // 006C7CBE..006C7CD9 BTN_BACK +44h
    gui_load_control_add_event_0041ab20(at(self, 0x368), nullptr, 0x27, self, on_b());         // 006C7CC4..006C7CDC OnBButtonPressed 00624BB0
    sw(self, 0x1E8, lw(self, 0x1E8) & 0xFFFFFFFBu);                                            // 006C7CE1..006C7CF7 BTN_UPGRADEITEMS +44h
    gui_load_control_add_event_0041ab20(at(self, 0x1A4), nullptr, 0x27, self, fn(&kotor_fastcall_006c2b60)); // 006C7CE7..006C7CFA
    u.dismiss();
    zero(self, 0x1154);                                                                        // 006C7D04
    return self;                                                                               // 006C7D0E
}
// 006C4450 ~CSWGuiUpgradeSelection.
void __fastcall kotor_fastcall_006c4450(void* self, void*) {
    set_vptr(self, vt_upgrade_selection());                                                    // 006C446D (007571B0)
    if (void* select = lp(self, 0x1150)) {                                                     // 006C4473..006C4483
        vcall<void*, U32>(select, 0x00, 1);                                                    // 006C4485..006C4489 deleting dtor(1)
        zero(self, 0x1150);                                                                    // 006C448B
    }
    vector_destruct(at(self, 0xC3C), 0x140, 4, &gui_load_label_destruct_00419d10);             // 006C4495..006C44AD
    vector_destruct(at(self, 0x52C), 0x1C4, 4, &kotor_m1_radio_button_destruct_006be360);      // 006C44B2..006C44CA
    gui_load_button_destruct_00419da0(at(self, 0x368), nullptr);                               // 006C44CF..006C44DA
    gui_load_button_destruct_00419da0(at(self, 0x1A4), nullptr);                               // 006C44DF..006C44EA
    gui_load_label_destruct_00419d10(at(self, 0x64), nullptr);                                 // 006C44EF..006C44F7
    gui_load_panel_destruct_0040cf70(self, nullptr);                                           // 006C44FC..006C4506
}
// 006C5B50 scalar deleting destructor RET 4.
void* __fastcall kotor_fastcall_006c5b50(void* self, void*, w4::U32 flags) {
    return deleting(self, &kotor_fastcall_006c4450, flags);                                    // 006C5B50..006C5B6B
}
// 006C2AB0 CSWGuiUpgradeSelection::GetUpgradableItems(CSWSCreature* creature, int* available, C2DA* table) RET 0Ch.
void __fastcall kotor_fastcall_006c2ab0(void*, void*, void* creature, w4::I32* available, void* table) {
    for (U32 i = 0; i < 0x12; ++i) {                                                           // 006C2ABB..006C2AFA (signed jl)
        void* item = recovered_equipment_item_005a4c20(lp(creature, 0xA2C), nullptr, 1u << i); // 006C2AC0..006C2AD2 GetItemInSlot(1 << i)
        if (item == nullptr) continue;                                                         // 006C2AD7..006C2AD9
        const I32 type = kotor_fastcall_005541c0(item, nullptr, table);                        // 006C2ADB..006C2AE3 (movzx)
        if (type > 0) available[type - 1] = 1;                                                 // 006C2AE6..006C2AEE
    }
}
// 006C2B10 CSWGuiUpgradeSelection::HandleInputEvent(int event, int down) RET 8.
void __fastcall kotor_fastcall_006c2b10(void* self, void*, w4::U32 event, w4::U32 down) {
    if (down != 0 && (event == 0x28 || event == 0x2E)) {                                       // 006C2B1B..006C2B29
        play_sound(self, 0);                                                                   // 006C2B2B..006C2B30
        kotor_fastcall_0062e870(ingame_gui(), nullptr);                                        // 006C2B35..006C2B44 HideUpgradeScreen
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, down);                          // 006C2B49..006C2B4D
}
// 006C2B60 CSWGuiUpgradeSelection::OnTypeSelected(CSWGuiControl* control) RET 4.
void __fastcall kotor_fastcall_006c2b60(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                        // 006C2B66..006C2B6D
    if (lw(control, 0x48) == 0x2D) play_sound(self, 0);                                        // 006C2B6F..006C2B7A
    if (control == at(self, 0x1A4)) {                                                          // 006C2B7F..006C2B87 BTN_UPGRADEITEMS
        control = lp(self, 0x1154);                                                            // 006C2B89
        if (control == nullptr) return;                                                        // 006C2B8F..006C2B91
    }
    const U32 type = lw(control, 0x58);                                                        // 006C2B93
    if (lw(self, 0x1140 + type * 4) == 0) return;                                              // 006C2B96..006C2B9F
    sb(lp(self, 0x1150), 0xC2C, static_cast<U8>(type + 1));                                    // 006C2BA1..006C2BAB (inc al)
    kotor_fastcall_0040bc70(manager_of(self), nullptr, lp(self, 0x1150), 3, 1);                // 006C2BB1..006C2BBD AddPanel(select, 3, 1)
}
// 006C2BD0 CSWGuiUpgradeSelection::OnEnter(CSWGuiControl* control) RET 4.
void __fastcall kotor_fastcall_006c2bd0(void* self, void*, void* control) {
    const U32 type = lw(control, 0x58);                                                        // 006C2BDD
    if (lw(control, 0x4C) == 0) return;                                                        // 006C2BD8..006C2BE0
    void* entered = vcall<void*>(control, 0x60);                                               // 006C2BE6..006C2BE9
    if (entered == nullptr) return;                                                            // 006C2BEC..006C2BF0
    if (lw(self, 0x1140 + type * 4) != 0) {                                                    // 006C2BF2..006C2C01
        gui_load_text_params_set_color_00414e10(at(self, 0x310), nullptr, borrowed_exit_color_007a23b4); // 006C2BFB..006C2C08
        copy12(at(self, 0x234), kotor_m1_color_007a2378);                                      // 006C2C0D..006C2C31
        sw(self, 0x1154, ad(entered));                                                         // 006C2C29
    } else {
        gui_load_text_params_set_color_00414e10(at(self, 0x310), nullptr, kotor_m1_color_007a2408); // 006C2C38..006C2C3D
        copy12(at(self, 0x234), kotor_m1_color_007a2384);                                      // 006C2C42..006C2C5F
        zero(self, 0x1154);                                                                    // 006C2C62
    }
}
// 006C4520 CSWGuiUpgradeSelection::OnPanelAdded() RET.
void __fastcall kotor_fastcall_006c4520(void* self, void*) {
    void* table = nullptr;                                                                     // [esp+10h] edi
    void* raw = live_operator_new(0x54);                                                       // 006C453C..006C4545
    if (raw != nullptr) {                                                                      // 006C454E..006C4556
        auto guard = on_throw([&]() { live_free(raw); });                                      // EH state 0
        menu_resources::Ref name; name.construct_00406d80("upgrade");                          // 006C4558..006C4567 (0075721C, by value)
        table = c2da_current_construct_00413cc0(raw, nullptr, name, 0);                        // 006C456C..006C456E C2DA(ref, 0)
        guard.armed = false;
    }
    c2da_current_load_004143b0(table, nullptr);                                                // 006C4581..006C458B (unchecked)
    const U32 id = lw(self, 0x113C);                                                           // 006C4590
    void* item = id != 0x7F000000u ? recovered_item_lookup_004ae760(server_app(), nullptr, id) : nullptr; // 006C4596..006C45AC
    if (item != nullptr) {                                                                     // 006C45AE..006C45B0
        const U8 type = kotor_fastcall_005541c0(item, nullptr, table);                         // 006C45B2..006C45B5
        sb(lp(self, 0x1150), 0xC2C, type);                                                     // 006C45BA..006C45C3
        sw(lp(self, 0x1150), 0xC38, ad(item));                                                 // 006C45C9..006C45D1
        kotor_fastcall_0040bc70(manager_of(self), nullptr, lp(self, 0x1150), 3, 1);            // 006C45CF..006C45E3 AddPanel(select, 3, 1)
    } else {
        void* server = server_app();                                                           // 006C45ED..006C45F3 esi
        void* party = lane_config_required::GetPartyTable_004aee70(server, nullptr);           // 006C45F6..006C45FD ebx
        void* repository = recovered_party_repository_00563340(party, nullptr);                // 006C45FF..006C4606 [esp+14h]
        I32* available = reinterpret_cast<I32*>(at(self, 0x1140));                             // edi
        for (U32 k = 0; k < 4; ++k) available[k] = 0;                                          // 006C460A..006C461C
        void* players = recovered_server_player_list_004aed90(server, nullptr);                // 006C461F..006C4621
        void* head = lp(lp(players, 0), 0);                                                    // 006C4626..006C4628
        void* player = head != nullptr ? lp(head, 8) : nullptr;                                // 006C462A..006C4632
        void* object = vcall<void*>(player, 0x04);                                             // 006C4635..006C4637 (unchecked)
        void* creature = kotor_fastcall_004ae770(server, nullptr, lw(object, 0x38));           // 006C463A..006C4640
        kotor_fastcall_006c2ab0(self, nullptr, creature, available, table);                    // 006C4645..006C464E
        for (U32 npc = 0; npc < 9; ++npc) {                                                    // 006C4653..006C469D (jb)
            if (kotor_fastcall_005636b0(party, nullptr, static_cast<I32>(npc)) == 0) continue; // 006C4655..006C465F
            if (static_cast<U8>(recovered_party_npc_selectable_005637c0(party, nullptr, npc)) == 0) continue; // 006C4661..006C466B (test al)
            const U32 npc_id = recovered_npc_object_00564700(party, nullptr, static_cast<I32>(npc), 0, 1); // 006C466D..006C4674
            void* member = kotor_fastcall_004ae770(server, nullptr, npc_id);                   // 006C4679..006C467C
            if (member == nullptr) continue;                                                   // 006C4681..006C4683
            kotor_fastcall_006c2ab0(self, nullptr, member, available, table);                  // 006C4685..006C4694
        }
        if (lw(repository, 0x10) != 0) {                                                       // 006C469F..006C46AA (jbe)
            for (U32 i = 0; i < lw(repository, 0x10); ++i) {                                   // 006C46B0..006C46E0 (re-read, jb)
                void* party_item = w2x_inventory_item_005560b0(repository, nullptr, i);        // 006C46B0..006C46B3
                if (party_item == nullptr) continue;                                           // 006C46B8..006C46BA
                const I32 type = kotor_fastcall_005541c0(party_item, nullptr, table);          // 006C46BC..006C46C8 (movzx)
                if (type > 0) sw(self, 0x113C + static_cast<U32>(type) * 4u, 1);               // 006C46CB..006C46CF (jle)
            }
        }
        for (U32 i = 0; i < 4; ++i) {                                                          // 006C46E2..006C4760
            void* radio = at(self, 0x52C + i * 0x1C4);                                         // esi
            void* lbl = at(self, 0xC3C + i * 0x140);                                           // edi = label + 44h
            const U32 a = lw(self, 0x1140 + i * 4);                                            // [ebx]
            const U32 f = lw(lbl, 0x44);
            sw(lbl, 0x44, f ^ (((a << 1) ^ f) & 2u));                                          // 006C4700..006C470D
            if (lw(self, 0x1140 + i * 4) != 0) {                                               // 006C470F..006C4718
                gui_load_text_params_set_color_00414e10(at(radio, 0x16C), nullptr, borrowed_exit_color_007a23b4); // 006C471A..006C471F
                vcall<void, U32>(radio, 0x88, 1);                                              // 006C4724..006C472A SetEnabled(1)
            } else {
                gui_load_text_params_set_color_00414e10(at(radio, 0x16C), nullptr, kotor_m1_color_007a2408); // 006C4732..006C4737
                vcall<void, U32>(radio, 0x88, 0);                                              // 006C473C..006C4742 SetEnabled(0)
            }
        }
        vcall<void, void*, U32>(self, 0x08, at(self, 0x52C), 0);                               // 006C4762..006C4770 SetActiveControl
    }
    if (table != nullptr) vcall<void*, U32>(table, 0x00, 1);                                   // 006C4773..006C4781 delete C2DA
    gui_load_panel_on_added_0040b870(self, nullptr);                                           // 006C4783..006C4785
}
}  // extern "C"

static_assert(sizeof(menu_resources::Ref) == 16, "CResRef by value");
