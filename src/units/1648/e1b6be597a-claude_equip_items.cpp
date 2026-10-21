// Lane W4 G15 (equip-items): CSWGuiInGameEquip item list (OnEnterSlot 006B9470, AddItemEntryToList 006B91C0) and the
// CSWGuiInGameItemEntry control class it builds (ctor 006B7EE0, dtors 006B7FA0/006B7F80, method table 007568F8 with
// SetExtent 006B5270 / Draw 006B4D40 / Initialize 006B4DB0, SetItem 006B6710, SetCanUse 006B5060), plus the server
// leaves OnEnterSlot needs without a callable provider: CSWSItem::GetClientItem 00554190, CSWSCreature::
// CheckProficiencies 00510E30 and CSWBaseItem::GetRequiredFeat 005B30C0. Transcribed from swkotor.exe (read-only);
// each statement cites the original instruction range.
//
// Class: CSWGuiButton (0x1C4) -> CSWGuiRadioButton (table 007566D8, inline ctor = vptr only) -> CSWGuiInGameItemEntry
// (0x39C): +1C4 item id, +1C8/+23C/+2B0 CSWGuiBorder (0x74), +324 CSWGuiText (0x70), +394 flags (bit0 pulsing, bit1
// equipped, bit2 marked), +398 can-use state byte. Table 007568F8 = 41 slots: the radio button table plus
// +00/+04/+38/+98/+9C and the new +A0 (Initialize).
#include "78d2073e39-gui_panels.hpp"
using namespace w4gui;

namespace kotor_reconstruction { struct StartupString; struct ArrayMutations; }
using SS = kotor_reconstruction::StartupString;
SS* __fastcall GuiConcat005e5d10(SS* self, void*, SS* out, const SS* rhs);                       // CExoString::operator+
namespace menu_list_growth { void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations* list, void*, unsigned count); } // CExoArrayList::Allocate
namespace resource_ref_assignment {                                                               // CResRef::operator=(const CResRef&) (by-value return)
menu_resources::Ref* __fastcall assign_00406120(menu_resources::Ref*, void*, menu_resources::Ref* out, const menu_resources::Ref* source);
}
extern "C" {
// colors (.data, 12 bytes each)
extern const unsigned char borrowed_exit_color_007a23b4[12];                                     // [007A23B4]
extern const unsigned char borrowed_enter_color_007a23c0[12];                                    // [007A23C0]
extern const unsigned char kotor_m15d_color_007a23d8[12];                                        // [007A23D8]
extern const unsigned char kotor_m1_color_007a23fc[12];                                          // [007A23FC]
extern void* kotor_cell_rules_007a3a28;                                                          // [007A3A28] CSWRules*
// radio button base (m1 saveload owner)
extern const unsigned char kotor_m1_radio_button_methods_007566d8[];                             // CSWGuiRadioButton (40 slots)
void __fastcall kotor_m1_radio_button_destruct_006be360(void* self, void*) noexcept(false);     // ~CSWGuiRadioButton
// generic GUI providers
void* __fastcall gui_load_border_construct_004167f0(void* self, void*) noexcept(false);         // CSWGuiBorder()
void __fastcall gui_load_border_destruct_004151c0(void* self, void*);                           // ~CSWGuiBorder
void* __fastcall gui_load_text_construct_00417280(void* self, void*) noexcept(false);           // CSWGuiText()
void __fastcall gui_load_text_destruct_00415ff0(void* self, void*);                             // ~CSWGuiText
void __fastcall gui_load_border_params_set_pulsing_00414d00(void* params, void*, U32, U32, U32); // SetPulsing
void __fastcall gui_load_border_params_set_fill_00414c00(void* params, void*, const void* ref, I32) noexcept(false); // SetFillImage
void* __fastcall gui_load_border_params_assign_004150c0(void* self, void*, const void* source);  // CSWGuiBorderParams::operator=
void* __fastcall gui_load_border_params_construct_00415000(void* self, void*, const void* corner, const void* edge,
    const void* fill, U32 fill_style, I32 dimension, float f0c, const void* color, I32 inner) noexcept(false);
void* __fastcall gui_load_border_inner_extent_00415240(void* border, void*, void* out);          // CSWGuiBorder::GetInnerExtent
void* __cdecl gui_load_extent_intersection_0040a4a0(void* out, const void* a, const void* b);    // CSWGuiExtent::Intersection
void* __fastcall kotor_fastcall_00417120(void* self, void*, void* text, const void* font, const void* color, U32 align, U32 alpha_bits); // CSWGuiTextParams ctor
void* __fastcall kotor_fastcall_004171e0(void* self, void*, const void* source) noexcept(false); // CSWGuiTextParams::operator=
U32 __fastcall async_string_length_005e5790(void* string, void*);                               // CExoString::GetLength
void __fastcall gui_load_listbox_clear_items_00419a90(void* listbox, void*);                     // CSWGuiListBox::ClearItems
void __fastcall kotor_fastcall_0041c1d0(void* listbox, void*, void* controls, U32 suppress, U32 restore, U32 variable) noexcept(false); // AddControls
void __fastcall gui_load_scrollbar_set_cur_004180a0(void* scrollbar, void*, I32 value);          // CSWGuiScrollBar::setCurValue
void __fastcall gui_load_array_add_00671c00(kotor_reconstruction::ArrayMutations* list, void*, U32 value); // CExoArrayList<ptr>::Add
void __fastcall gui_load_slot_ret8_005b5e90(void*, void*, U32, U32);                              // 005B5E90 ret 8
void __fastcall light_manager_void_return_16_0063cfc0(void*, void*, U32, U32, U32, U32);         // 0063CFC0 ret 10h
void* __fastcall c2da_default_ref_00405ed0(void* self, void*);                                   // CResRef::CResRef()
menu_resources::Ref* __fastcall unresolved_ref_assign_00406290(menu_resources::Ref*, void*, menu_resources::Ref* out, const char* text); // CResRef::operator=(const char*)
// client / server
void* __fastcall kotor_fastcall_005edeb0(void* app, void*, void* out, U32 id) noexcept(false);  // CClientExoApp::GetGUIString
U32 __fastcall kotor_fastcall_005ed4c0(void* app, void*);                                        // CClientExoApp::GetClientLanguage
U32 __fastcall exo_locstring_get_005ea130(void* loc, void*, U32 language, void* out, U32 gender); // CExoLocString::GetString
void* __fastcall kotor_m15_client_get_item_by_id_005ed5a0(void* app, void*, U32 id);             // CClientExoApp::GetItemByGameObjectID
U32 __fastcall kotor_m15a_creature_get_equipped_item_id_0060ca80(void* creature, void*, U32 slot); // CSWCCreature::GetEquippedItemID
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void* creature, void*);        // CSWCCreature::GetServerCreature
void* __fastcall recovered_creature_inventory_004ef770(void* creature, void*, U32 flag);          // CSWSCreature::GetItemRepository
void* __fastcall w2x_inventory_item_005560b0(void* repository, void*, U32 index);                // CItemRepository::GetItemInRepository
U32 __fastcall recovered_item_can_equip_0051aa60(void* creature, void*, void* item, U32* slot, U32, U32, U32); // CSWSCreature::CanEquipItem
void* __fastcall recovered_item_lookup_004ae760(void* server, void*, U32 id);                    // CServerExoApp::GetItemByGameObjectID
U32 __fastcall kotor_fastcall_004aea30(void* server, void*, U32 id);                             // CServerExoApp::ClientToServerObjectId
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void* item, void*) noexcept(false);      // CSWItem::GetBaseItem
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* array, void*, I32 id) noexcept(false); // CSWBaseItemArray::GetBaseItem
U32 __fastcall recovered_stats_has_feat_005a6630(void* stats, void*, U32 feat);                  // CSWSCreatureStats::HasFeat (low WORD used)
void* __fastcall recovered_object_render_004cc2b0(void* object, void*);                          // CSWSObject::GetClientObject
menu_resources::Ref* __fastcall kotor_fastcall_005556b0(void* item, void*, menu_resources::Ref* out); // CSWSItem::GetIcon
}

namespace {
// CExoString value (8 bytes) created by CExoString() (005B3190); destroyed by ~CExoString (005E5C20).
struct ExoDefault {
    U32 storage[2];
    ExoDefault() { exo_default(storage); }
    ~ExoDefault() { frame_LogStringDtor_005e5c20(storage, nullptr); }
};
// Hidden-return CExoString: lifetime begins in the provider; destroyed with the enclosing scope.
struct ExoResult {
    U32 storage[2];
    bool live = false;
    ~ExoResult() { if (live) frame_LogStringDtor_005e5c20(storage, nullptr); }
};
SS* ss(void* p) { return static_cast<SS*>(p); }
// Inline CExoArrayList<T*>::Add (grow: capacity ? 2*capacity : 10h).
void array_add(void* list, U32 value) {
    if (lw(list, 4) == lw(list, 8))
        menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(list), nullptr,
                                            lw(list, 8) != 0 ? lw(list, 8) + lw(list, 8) : 0x10u);
    sw(lp(list, 0), lw(list, 4) * 4u, value);
    sw(list, 4, lw(list, 4) + 1u);
}
void copy_color(void* self, U32 off, const unsigned char* color) { std::memcpy(at(self, off), color, 12); }
void set_fill_text(void* self, U32 params, const char* name) {
    menu_resources::Ref ref; ref.construct_00406d80(name);
    gui_load_border_params_set_fill_00414c00(at(self, params), nullptr, &ref, 0);
}
}  // namespace

extern "C" {
void* __fastcall kotor_fastcall_006b7f80(void* self, void*, U32 flags);
void __fastcall kotor_fastcall_006b5270(void* self, void*, const void* extent);
void __fastcall kotor_fastcall_006b4d40(void* self, void*, U32 arg);
void __fastcall kotor_fastcall_006b4db0(void* self, void*, void* text, void* border, void* highlight, I32 width, U32 pulsing);
}

namespace {
const Override ovr_item_entry[] = {
    {0x00, fn(&kotor_fastcall_006b7f80)},                   // 006B7F80 ~CSWGuiInGameItemEntry (deleting)
    {0x04, fn(&kotor_fastcall_006b5270)},                   // 006B5270 SetExtent
    {0x38, fn(&kotor_fastcall_006b4d40)},                   // 006B4D40 Draw
    {0x98, fn(&gui_load_slot_ret8_005b5e90)},               // 005B5E90 (ret 8)
    {0x9C, fn(&light_manager_void_return_16_0063cfc0)},     // 0063CFC0 (ret 10h)
    {0xA0, fn(&kotor_fastcall_006b4db0)},                   // 006B4DB0 Initialize
};
void* table_item_entry[41];
bool built_item_entry = false;
const void* vt_item_entry() {   // original 007568F8 (41 slots; base 007566D8 40 slots)
    if (!built_item_entry) { vt_build(table_item_entry, 41, kotor_m1_radio_button_methods_007566d8, 40, ovr_item_entry, 6); built_item_entry = true; }
    return table_item_entry;
}
}  // namespace

extern "C" {
void __fastcall kotor_fastcall_006b7920(void* self, void*, void* entry);                         // OnItemSelected (claude_equip_items2.cpp)
void __fastcall kotor_fastcall_006b9160(void* self, void*, void* control);                       // OnOKPressed (equip handlers file)

// ---------------------------------------------------------------- server leaves
// 005B30C0 CSWBaseItem::GetRequiredFeat(BYTE index) RET 4 -> WORD (AX).
U16 __fastcall kotor_fastcall_005b30c0(void* base, void*, U32 index) {
    const U8 i = static_cast<U8>(index);                                                       // 005B30C0
    if (!(i < lb(base, 0x80))) return 0xFFFFu;                                                 // 005B30C4..005B30D0 (jb)
    return lh(lp(base, 0x7C), static_cast<U32>(i) * 2u);                                       // 005B30D3..005B30DD
}

// 00510E30 CSWSCreature::CheckProficiencies(CSWSItem* item, int unused) RET 8. Null base items are dereferenced as in
// the original (no guard).
U32 __fastcall kotor_fastcall_00510e30(void* creature, void*, void* item, U32) {
    auto base = [&]() { return kotor_m15b_base_item_array_get_base_item_005b31a0(lp(kotor_cell_rules_007a3a28, 0x34), nullptr, li(item, 0x0C)); };
    if (lw(base(), 4) == 0) return 0;                                                          // 00510E36..00510E54
    if (recovered_stats_has_feat_005a6630(lp(creature, 0xA74), nullptr, 0x5D) != 0) return 1;  // 00510E57..00510E6F
    I32 i = 0;                                                                                 // 00510E80
    if (lb(base(), 0x80) != 0) {                                                               // 00510E72..00510E8F
        do {
            const U16 feat = kotor_fastcall_005b30c0(base(), nullptr, static_cast<U32>(i));   // 00510E91..00510EA6
            if (recovered_stats_has_feat_005a6630(lp(creature, 0xA74), nullptr, feat) == 0) return 0; // 00510EAB..00510EB9 / 00510EE3
            ++i;                                                                               // 00510EC7
        } while (i < static_cast<I32>(lb(base(), 0x80)));                                      // 00510EBB..00510ED6 (jl)
    }
    return 1;                                                                                  // 00510ED8..00510EE0
}

// 00554190 CSWSItem::GetClientItem(): GetClientObject on the CSWSObject base (+10h), then its vtable +34h (tail jump).
void* __fastcall kotor_fastcall_00554190(void* item, void*) {
    void* object = at(item, 0x10);                                                             // 00554191
    if (recovered_object_render_004cc2b0(object, nullptr) == nullptr) return nullptr;          // 00554194..0055419D / 005541AE
    void* client = recovered_object_render_004cc2b0(object, nullptr);                          // 0055419F..005541A1
    return vcall<void*>(client, 0x34);                                                         // 005541A6..005541AB
}

// ---------------------------------------------------------------- CSWGuiInGameItemEntry
// 006B7EE0 CSWGuiInGameItemEntry::CSWGuiInGameItemEntry() (thiscall, returns this).
void* __fastcall kotor_fastcall_006b7ee0(void* self, void*) {
    gui_load_button_construct_0041c0f0(self, nullptr);                                         // 006B7EFD
    Unwind u; u.push(&kotor_m1_radio_button_destruct_006be360, self);                          // EH state 0 -> 006BE360
    set_vptr(self, vt_item_entry());                                                           // 006B7F10 (007568F8)
    for (U32 off : {0x1C8u, 0x23Cu, 0x2B0u}) {                                                 // 006B7F02..006B7F36
        gui_load_border_construct_004167f0(at(self, off), nullptr); u.push(&gui_load_border_destruct_004151c0, at(self, off));
    }
    gui_load_text_construct_00417280(at(self, 0x324), nullptr);                                // 006B7F3B..006B7F46
    u.dismiss();
    sw(self, 0x394, lw(self, 0x394) & 0xFFFFFFF8u);                                            // 006B7F4B..006B7F58
    sw(self, 0x1C4, 0x7F000000u);                                                              // 006B7F5E
    sb(self, 0x398, 0);                                                                        // 006B7F68
    return self;
}

// 006B7FA0 CSWGuiInGameItemEntry::~CSWGuiInGameItemEntry (members reverse, then ~CSWGuiRadioButton inlined).
void __fastcall kotor_fastcall_006b7fa0(void* self, void*) {
    gui_load_text_destruct_00415ff0(at(self, 0x324), nullptr);                                 // 006B7FBD..006B7FCB
    gui_load_border_destruct_004151c0(at(self, 0x2B0), nullptr);                               // 006B7FD0..006B7FDB
    gui_load_border_destruct_004151c0(at(self, 0x23C), nullptr);                               // 006B7FE0..006B7FEB
    gui_load_border_destruct_004151c0(at(self, 0x1C8), nullptr);                               // 006B7FF0..006B7FFB
    set_vptr(self, kotor_m1_radio_button_methods_007566d8);                                    // 006B800A (007566D8)
    gui_load_button_destruct_00419da0(self, nullptr);                                          // 006B8010
}

// 006B7F80 scalar deleting destructor (thiscall(flags) RET 4).
void* __fastcall kotor_fastcall_006b7f80(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_006b7fa0, flags); }

// 006B4D40 CSWGuiInGameItemEntry::Draw(arg) RET 4 (shared with CSWGuiStoreItemEntry).
void __fastcall kotor_fastcall_006b4d40(void* self, void*, U32 arg) {
    void* second;
    if ((lb(self, 0x44) & 1u) != 0) {                                                          // 006B4D43..006B4D4D
        vcall<void, U32>(at(self, 0xE0), 0x0C, arg); second = at(self, 0x23C);                 // 006B4D4F..006B4D64
    } else {
        vcall<void, U32>(at(self, 0x6C), 0x0C, arg); second = at(self, 0x1C8);                 // 006B4D66..006B4D6F
    }
    vcall<void, U32>(second, 0x0C, arg);                                                       // 006B4D75..006B4D78
    vcall<void, U32>(at(self, 0x2B0), 0x0C, arg);                                              // 006B4D7B..006B4D88
    vcall<void, U32>(at(self, 0x154), 0x0C, arg);                                              // 006B4D8B..006B4D98
    vcall<void, U32>(at(self, 0x324), 0x0C, arg);                                              // 006B4D9B..006B4DA8
}

// 006B5270 CSWGuiInGameItemEntry::SetExtent(CSWGuiExtent*) RET 4 (shared with CSWGuiStoreItemEntry).
void __fastcall kotor_fastcall_006b5270(void* self, void*, const void* extent) {
    if (li(extent, 8) <= 0x38) return;                                                         // 006B527B..006B5285 (jle)
    std::memcpy(at(self, 4), extent, 16);                                                      // 006B528E..006B52A4
    I32 icon[4] = {li(self, 4), li(self, 8), 0x38, 0x38};                                      // 006B52A7..006B52CA [esp+10h]
    vcall<void, const void*>(at(self, 0x1C8), 0x04, icon);                                     // 006B52CE
    vcall<void, const void*>(at(self, 0x23C), 0x04, icon);                                     // 006B52D1..006B52E2
    vcall<void, const void*>(at(self, 0x2B0), 0x04, icon);                                     // 006B52E5..006B52F6
    I32 count[4] = {icon[0], icon[1], icon[2], icon[3]};                                       // 006B52F9..006B531B [esp+20h]
    const I32 length = static_cast<I32>(async_string_length_005e5790(at(self, 0x33C), nullptr)); // 006B5311..006B531F
    const I32 w = length <= 2 ? 0x15 : 0x2A;                                                   // 006B5324..006B533D
    count[3] = 0x13;                                                                           // 006B532E
    count[2] = w;                                                                              // 006B5345
    count[0] = count[0] + (0x38 - w);                                                          // 006B5343..006B5352
    count[1] = count[1] + 0x25;                                                                // 006B5349..006B5360
    vcall<void, const void*>(at(self, 0x324), 0x04, count);                                    // 006B5356..006B5367
    icon[2] = li(self, 0x0C) - 0x38;                                                           // 006B536A..006B5378
    icon[0] = icon[0] + 0x38;                                                                  // 006B5376..006B5386
    vcall<void, const void*>(at(self, 0x6C), 0x04, icon);                                      // 006B537C..006B538A
    vcall<void, const void*>(at(self, 0xE0), 0x04, icon);                                      // 006B538D..006B53A0
    I32 inner_highlight[4], inner_border[4], clip[4];                                          // [esp+30h] / [esp+40h] / [esp+50h]
    void* b = gui_load_border_inner_extent_00415240(at(self, 0xE0), nullptr, inner_highlight); // 006B53A9..006B53B6
    void* a = gui_load_border_inner_extent_00415240(at(self, 0x6C), nullptr, inner_border);    // 006B53BB..006B53C3
    void* r = gui_load_extent_intersection_0040a4a0(clip, a, b);                               // 006B53C8..006B53D3
    vcall<void, const void*>(at(self, 0x154), 0x04, r);                                        // 006B53D6..006B53D9
}

// 006B4DB0 CSWGuiInGameItemEntry::Initialize(CSWGuiText* text, CSWGuiBorder* border, CSWGuiBorder* highlight, int width,
// int pulsing) RET 14h (method table +A0).
void __fastcall kotor_fastcall_006b4db0(void* self, void*, void* text, void* border, void* highlight, I32 width, U32 pulsing) {
    kotor_fastcall_004171e0(at(self, 0x16C), nullptr, at(text, 0x18));                         // 006B4DC2..006B4DCC
    gui_load_border_params_assign_004150c0(at(self, 0x80), nullptr, at(border, 0x14));          // 006B4DD1..006B4DE4
    copy_color(self, 0x90, borrowed_exit_color_007a23b4);                                      // 006B4DE9..006B4E09
    gui_load_border_params_assign_004150c0(at(self, 0xF4), nullptr, at(highlight, 0x14));       // 006B4E0C..006B4E18
    copy_color(self, 0x104, borrowed_enter_color_007a23c0);                                    // 006B4E1D..006B4E42
    gui_load_border_params_set_pulsing_00414d00(at(self, 0xF4), nullptr, pulsing, 1, 0);       // 006B4E3E..006B4E48
    float color[3];                                                                            // [esp+0Ch] (also the extent below)
    for (U32 pass = 0; pass < 2; ++pass) {                                                     // 006B4E4D..006B4EC0 / 006B4EE1..006B4F54
        const U32 target = pass == 0 ? 0x1DCu : 0x250u;
        menu_resources::Ref fill, edge, corner;                                                // "lbl_hex_3", "", ""
        fill.construct_00406d80("lbl_hex_3");
        edge.construct_00406d80("");
        corner.construct_00406d80("");
        color[0] = 1.0f; color[1] = 1.0f; color[2] = 1.0f;                                     // 006B4EA0..006B4EB0
        alignas(4) unsigned char params[0x54];                                                 // [esp+4Ch] CSWGuiBorderParams
        void* p = gui_load_border_params_construct_00415000(params, nullptr, &corner, &edge, &fill, 0, 0, 1.0f, color, 0); // 006B4EB8
        gui_load_border_params_assign_004150c0(at(self, target), nullptr, p);                  // 006B4EBD..006B4EC0
        if (pass == 0) copy_color(self, 0x1EC, borrowed_exit_color_007a23b4);                  // 006B4EC5..006B4EDE
        else copy_color(self, 0x260, borrowed_enter_color_007a23c0);                           // 006B4F59..006B4F76
    }
    gui_load_border_params_set_pulsing_00414d00(at(self, 0x250), nullptr, pulsing, 0, 1);      // 006B4F72..006B4F7C
    I32 extent[4] = {0, 0, width, 0x38};                                                       // 006B4F81..006B4FA5
    vcall<void, const void*>(self, 0x04, extent);                                              // 006B4FAD SetExtent
    sw(self, 0x394, lw(self, 0x394) ^ ((lw(self, 0x394) ^ pulsing) & 1u));                     // 006B4FB0..006B4FC4
}

// 006B5060 CSWGuiInGameItemEntry::SetCanUse(BYTE state) RET 4. Jump table 006B51B4: 0,1 -> 006B507B; 2,3 -> 006B50FA.
void __fastcall kotor_fastcall_006b5060(void* self, void*, U32 state_arg) {
    const U8 state = static_cast<U8>(state_arg);                                               // 006B5061
    if (state <= 1) {                                                                          // 006B507B..006B50F8
        copy_color(self, 0x90, borrowed_exit_color_007a23b4);
        copy_color(self, 0x1EC, borrowed_exit_color_007a23b4);
        copy_color(self, 0x104, borrowed_enter_color_007a23c0);
        copy_color(self, 0x260, borrowed_enter_color_007a23c0);
    } else if (state <= 3) {                                                                   // 006B50FA..006B5172
        for (U32 off : {0x90u, 0x1ECu, 0x104u, 0x260u}) copy_color(self, off, kotor_m15d_color_007a23d8);
    }                                                                                          // > 3: 006B5175 directly
    gui_load_border_params_set_pulsing_00414d00(at(self, 0xF4), nullptr, lw(self, 0x394) & 1u, 1, 0);  // 006B5175..006B5189
    gui_load_border_params_set_pulsing_00414d00(at(self, 0x250), nullptr, lw(self, 0x394) & 1u, 0, 1); // 006B518E..006B51A2
    sb(self, 0x398, state);                                                                    // 006B51A7
}

// 006B6710 CSWGuiInGameItemEntry::SetItem(DWORD id, int equipped, int marked) RET 0Ch.
void __fastcall kotor_fastcall_006b6710(void* self, void*, U32 id, U32 equipped, U32 marked) {
    menu_resources::Ref icon;                                                                  // [esp+4Ch]
    c2da_default_ref_00405ed0(&icon, nullptr);                                                 // 006B6736..006B673A
    sw(self, 0x394, ((((marked & 1u) << 1) | (equipped & 1u)) << 1) | (lw(self, 0x394) & 0xFFFFFFF9u)); // 006B673F..006B676A
    sw(self, 0x1C4, id);                                                                       // 006B6764
    // Empty slot (006B6C76) and missing item (006B6B76) run the same sequence; only the discarded 00406290 result
    // temporary differs ([esp+64h] vs [esp+0Ch]).
    auto no_item = [&]() {
        set_fill_text(self, 0x1DC, "lbl_hex_3");                                               // 006B6B76..006B6B91 / 006B6C76..006B6C91
        set_fill_text(self, 0x250, "lbl_hex_3");                                               // 006B6B96..006B6BB1 / 006B6C96..006B6CB1
        {
            ExoResult s;                                                                       // [esp+0Ch]
            kotor_fastcall_005edeb0(client_app(), nullptr, s.storage, 0x16B); s.live = true;   // 006B6BB6..006B6BC9
            gui_load_text_params_set_text_00415e00(at(self, 0x16C), nullptr, s.storage);       // 006B6BCE..006B6BE0
        }                                                                                      // 006B6BE5..006B6BF3
        menu_resources::Ref font; font.construct_00406d80("fnt_d16x16");                       // 006B6BF8..006B6C01 [esp+34h]
        {
            ExoDefault empty;                                                                  // 006B6C06..006B6C0A [esp+0Ch]
            U32 params[0x70 / 4];                                                              // [esp+74h] CSWGuiTextParams
            kotor_fastcall_00417120(params, nullptr, empty.storage, &font, borrowed_exit_color_007a23b4, 0x22, 0x3F800000u); // 006B6C0F..006B6C34
            { auto g = on_throw([&]() { frame_LogStringDtor_005e5c20(params, nullptr); });
              kotor_fastcall_004171e0(at(self, 0x33C), nullptr, params); g.armed = false; }    // 006B6C39..006B6C48
            frame_LogStringDtor_005e5c20(params, nullptr);                                     // 006B6C4D..006B6C58
        }                                                                                      // 006B6C5D..006B6C68
        menu_resources::Ref discarded;
        unresolved_ref_assign_00406290(&icon, nullptr, &discarded, "inone");                   // 006B6C6D / 006B6D6D..006B6D7B
    };
    if (id == 0x7F000000u) {                                                                   // 006B675F..006B6770
        no_item();
    } else {
        void* server = server_app();                                                           // 006B6776..006B677C
        void* item = recovered_item_lookup_004ae760(server, nullptr, kotor_fastcall_004aea30(server, nullptr, id)); // 006B6786..006B6791
        if (item == nullptr) {                                                                 // 006B6798..006B679A
            no_item();
        } else {
            ExoDefault name;                                                                   // 006B67A0..006B67A4 [esp+1Ch]
            exo_locstring_get_005ea130(at(item, 0x280), nullptr, kotor_fastcall_005ed4c0(client_app(), nullptr), name.storage, 0); // 006B67A9..006B67CB
            if (equipped != 0) {                                                               // 006B67D0..006B67D5
                ExoString close(")");                                                          // 006B67DB..006B67E4 [esp+44h]
                ExoString open(" (");                                                          // 006B67E9..006B67FA [esp+2Ch]
                ExoResult tag;                                                                 // [esp+0Ch]
                kotor_fastcall_005edeb0(client_app(), nullptr, tag.storage, 0x7E5A); tag.live = true; // 006B67FF..006B6819
                ExoResult t1, t2, t3;                                                          // [esp+34h] / [esp+64h] / [esp+5Ch]
                GuiConcat005e5d10(ss(name.storage), nullptr, ss(t1.storage), ss(open.storage)); t1.live = true;  // 006B6820..006B6836
                GuiConcat005e5d10(ss(t1.storage), nullptr, ss(t2.storage), ss(tag.storage)); t2.live = true;     // 006B683B..006B684B
                GuiConcat005e5d10(ss(t2.storage), nullptr, ss(t3.storage), ss(close.storage)); t3.live = true;   // 006B6850..006B6864
                async_assign_string_005e5c50(name.storage, nullptr, t3.storage);               // 006B6869..006B6876
            }                                                                                  // 006B687B..006B68DC (t3, t2, t1, tag, open, close)
            if (marked != 0) {                                                                 // 006B68E1..006B68E6
                copy_color(self, 0x90, kotor_m1_color_007a23fc);                               // 006B68EC..006B6909
                copy_color(self, 0x1EC, kotor_m1_color_007a23fc);                              // 006B690C..006B6927
                sw(self, 0x8C, 0x3F000000u); sw(self, 0x1E8, 0x3F000000u);                     // 006B692A..006B6935 (0.5f)
                copy_color(self, 0x104, kotor_m1_color_007a23fc);                              // 006B693B..006B6957
                copy_color(self, 0x260, kotor_m1_color_007a23fc);                              // 006B695A..006B6977
            } else {
                copy_color(self, 0x90, borrowed_exit_color_007a23b4);                          // 006B697C..006B6997
                copy_color(self, 0x1EC, borrowed_exit_color_007a23b4);                         // 006B699A..006B69B6
                copy_color(self, 0x104, borrowed_enter_color_007a23c0);                        // 006B69B9..006B69D6
                copy_color(self, 0x260, borrowed_enter_color_007a23c0);                        // 006B69D9..006B69F4
            }
            gui_load_text_params_set_text_00415e00(at(self, 0x16C), nullptr, name.storage);    // 006B69F7..006B6A02
            ExoDefault count;                                                                  // 006B6A07..006B6A0B [esp+24h]
            const I32 stack = li(item, 0x28C);                                                 // 006B6A10
            if (stack <= 1) {                                                                  // 006B6A16..006B6A25 (jle)
                set_fill_text(self, 0x1DC, "lbl_hex_3");                                       // 006B6AA0..006B6AB7
                set_fill_text(self, 0x250, "lbl_hex_3");                                       // 006B6ABC..006B6AD7
            } else {
                const char* fill = stack >= 0x64 ? "lbl_hex_7" : "lbl_hex_6";                  // 006B6A27..006B6A2A (jl)
                set_fill_text(self, 0x1DC, fill);                                              // 006B6A2C..006B6A43 / 006B6A7D..006B6A94
                set_fill_text(self, 0x250, fill);                                              // 006B6A48..006B6A63 / 006B6A99..
                frame_LogFormat_005e5680(count.storage, "%d", stack);                          // 006B6A68..006B6A78
            }
            menu_resources::Ref font; font.construct_00406d80("fnt_d16x16");                   // 006B6ADC..006B6AE5 [esp+0Ch]
            {
                U32 params[0x70 / 4];                                                          // [esp+74h] CSWGuiTextParams
                kotor_fastcall_00417120(params, nullptr, count.storage, &font, borrowed_exit_color_007a23b4, 0x22, 0x3F800000u); // 006B6AEA..006B6B07
                { auto g = on_throw([&]() { frame_LogStringDtor_005e5c20(params, nullptr); });
                  kotor_fastcall_004171e0(at(self, 0x33C), nullptr, params); g.armed = false; } // 006B6B0C..006B6B1B
                frame_LogStringDtor_005e5c20(params, nullptr);                                 // 006B6B20..006B6B2C
            }
            menu_resources::Ref made, discarded;                                               // [esp+0Ch] / [esp+34h]
            resource_ref_assignment::assign_00406120(&icon, nullptr, &discarded,
                kotor_fastcall_005556b0(item, nullptr, &made));                                // 006B6B31..006B6B47
        }                                                                                      // 006B6B4C..006B6B6C (~count, ~name)
    }
    sw(self, 0x2E0, (lw(self, 0x2E0) & 0xFFFFFFFEu) | 2u);                                     // 006B6D80..006B6D99
    gui_load_border_params_set_fill_00414c00(at(self, 0x2C4), nullptr, &icon, 0);              // 006B6D86..006B6D9C
}

// ---------------------------------------------------------------- CSWGuiInGameEquip
// 006B91C0 CSWGuiInGameEquip::AddItemEntryToList(int index) RET 4 -> CSWGuiInGameItemEntry*. Entries are cached in the
// +1B8C array; an index other than the current count reuses [array+index*4] unchecked (original quirk).
void* __fastcall kotor_fastcall_006b91c0(void* self, void*, I32 index) {
    if (index != li(self, 0x1B90)) {                                                           // 006B91CE..006B91E4
        void* entry = lp(lp(self, 0x1B8C), static_cast<U32>(index) * 4u);                      // 006B92C5..006B92CB
        sw(entry, 0x8C, 0);                                                                    // 006B92D4
        return entry;
    }
    void* raw = live_operator_new(0x39C);                                                      // 006B91EA..006B91F4
    void* entry = nullptr;
    if (raw != nullptr) {                                                                      // 006B91FB..006B9205
        auto g = on_throw([&]() { live_free(raw); });                                          // EH state 0
        entry = kotor_fastcall_006b7ee0(raw, nullptr);                                         // 006B9207..006B920E
        g.armed = false;
    }
    array_add(at(self, 0x1B8C), ad(entry));                                                    // 006B9214..006B924F
    void* proto = lp(self, 0x33A4);                                                            // 006B9259 (LB_ITEMS prototype)
    const I32 width = li(self, 0x336C) - static_cast<I32>(static_cast<U32>(lb(self, 0x3398)) * 2u); // 006B9252..006B926B
    vcall<void, void*, void*, void*, I32, U32>(entry, 0xA0, at(proto, 0x154), at(proto, 0x6C), at(proto, 0xE0), width, 1); // 006B9265..006B9281
    gui_load_control_add_event_0041ab20(entry, nullptr, 0, self, fn(&kotor_fastcall_006b7920));      // 006B9287..006B9291
    gui_load_control_add_event_0041ab20(entry, nullptr, 0x27, self, fn(&kotor_fastcall_006b9160));   // 006B9296..006B92A0
    sw(entry, 0x8C, 0);                                                                        // 006B92A5
    return entry;
}

// 006B9470 CSWGuiInGameEquip::OnEnterSlot(CSWGuiControl* slot_button) RET 4. Slot table .rdata 00756560 (stride 14h):
// +0 inventory slot mask, +0Ch / +10h description strref (non-droid / droid).
void __fastcall kotor_fastcall_006b9470(void* self, void*, void* control) {
    struct SlotRow { U32 mask; U32 strref; U32 strref_droid; };
    static const SlotRow rows[9] = {
        {0x020, 0x7A92, 0x7A92}, {0x010, 0x7A93, 0x7A93}, {0x001, 0x7A8F, 0xA352}, {0x080, 0x7A90, 0xA356},
        {0x100, 0x7A91, 0xA356}, {0x002, 0x7A94, 0xA355}, {0x008, 0x7A97, 0xA353}, {0x200, 0x7A9C, 0xA353},
        {0x400, 0x7A96, 0xA354}};
    if (lp(self, 0x64) == nullptr) return;                                                     // 006B948A..006B9493 / 006B995A
    const U32 slot = lw(control, 0x58);                                                        // 006B9499..006B949E (ebp)
    const SlotRow& row = rows[slot];                                                           // [esp+24h] = slot*14h (unchecked)
    sw(self, 0x4278, row.mask);                                                                // 006B94AC..006B94B9
    U32 changed = 0;                                                                           // [esp+14h]
    if (row.mask == 0xFFFFFFFFu) {                                                             // 006B94B2..006B94BF
        gui_load_listbox_clear_items_00419a90(at(self, 0x30D8), nullptr);                      // 006B9917..006B991D
        ExoString empty("");                                                                   // 006B9922..006B992B
        gui_load_text_params_set_text_00415e00(at(self, 0x2E00), nullptr, empty.storage);      // 006B9930..006B9943
        return;                                                                                // 006B9948..006B996A
    }
    void* creature = kotor_m15a_creature_get_server_creature_0060fb20(lp(self, 0x64), nullptr); // 006B94C6..006B94CB (ebx, [esp+1Ch])
    void* repository = recovered_creature_inventory_004ef770(creature, nullptr, 1);            // 006B94CD..006B94DA ([esp+20h])
    struct List { U32 data, size, capacity; ~List() { live_free(pt(data)); } } list{0, 0, 0}; // [esp+34h]; EH state 0 frees data
    {
        void* entry = kotor_fastcall_006b91c0(self, nullptr, 0);                               // 006B94EA..006B94F1
        kotor_fastcall_006b6710(entry, nullptr, 0x7F000000u, 0, 0);                            // 006B94F6..006B9503
        kotor_fastcall_006b5060(entry, nullptr, 0);                                            // 006B9508..006B950C
        sb(entry, 0x54, 0xFF);                                                                 // 006B9511
        array_add(&list, ad(entry));                                                           // 006B9515..006B9552 (inline Add)
    }
    I32 next = 1;                                                                              // 006B9556 [esp+0Ch]
    if (lw(self, 0x427C + slot * 4u) != 0x7F000000u) {                                         // 006B9545..006B955E
        void* entry = kotor_fastcall_006b91c0(self, nullptr, 1);                               // 006B9560..006B9564
        const U32 id = lw(self, 0x427C + slot * 4u);                                           // 006B9569
        if (!(lw(entry, 0x1C4) == id && ((lw(entry, 0x394) >> 1) & 1u) != 0 && lb(entry, 0x398) == 1)) changed = 1; // 006B9572..006B9591
        kotor_fastcall_006b6710(entry, nullptr, id, 1, 0);                                     // 006B9599..006B95A0
        kotor_fastcall_006b5060(entry, nullptr, 1);                                            // 006B95A5..006B95A9
        sb(entry, 0x54, 0xFF);                                                                 // 006B95B3
        gui_load_array_add_00671c00(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(&list), nullptr, ad(entry)); // 006B95AE..006B95B7
        next = 2;                                                                              // 006B95BC
    }
    bool scan = true;
    if (lw(self, 0x4278) == 0x20 && lw(self, 0x4280) != 0x7F000000u) {                         // 006B95C4..006B95D8
        void* server = server_app();                                                           // 006B95DA..006B95E0
        void* right = recovered_item_lookup_004ae760(server, nullptr, kotor_fastcall_004aea30(server, nullptr, lw(self, 0x4280))); // 006B95E3..006B95EE
        if (right != nullptr && lb(kotor_m15b_item_get_base_item_005b4790(right, nullptr), 0x1B) == 4) scan = false; // 006B95F3..006B960C
    }
    if (scan) {
        for (U32 i = 0; i < lw(repository, 0x10); ++i) {                                       // 006B9612..006B9621 / 006B981C..006B982E (jb)
            void* item = w2x_inventory_item_005560b0(repository, nullptr, i);                  // 006B9627..006B962D ([esp+30h])
            if (item == nullptr) continue;                                                     // 006B962F..006B9635
            if ((lw(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 4) & lw(self, 0x4278)) == 0) continue; // 006B963B..006B964B
            const U8 restriction = lb(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0xB4); // 006B9651..006B965E
            if (restriction == 1) {                                                            // 006B9686..006B969A
                if (lh(lp(creature, 0xA74), 0xDC) != 6) continue;                              // [0074BF14] = 6
            } else if (restriction == 2) {                                                     // 006B966B..006B9681
                if (lh(lp(creature, 0xA74), 0xDC) != 5) continue;                              // [0074BF10] = 5
            }
            // [esp+54h] = the argument slot of `control`, reused as the can-use state byte (original stack reuse).
            U8 state = 0;                                                                      // 006B96B4
            U32 mask = lw(self, 0x4278);                                                       // 006B96A0..006B96B9 [esp+8]
            if (static_cast<U8>(recovered_item_can_equip_0051aa60(creature, nullptr, item, &mask, 0, 0, 0)) == 0) state = 2; // 006B96BD..006B96C6
            void* options = kotor_fastcall_005ed700(client_app(), nullptr);                    // 006B96CB..006B96D9
            if (options != nullptr && (lb(options, 0x14) & 1u) != 0) {                         // 006B96DE..006B96E6
                U32 proficient = 1;                                                            // 006B96D4 (ebp)
                if (kotor_fastcall_00510e30(creature, nullptr, item, lw(self, 0x4278)) == 0) proficient = 0; // 006B96E8..006B96FB
                mask = lw(self, 0x4278);                                                       // 006B96FD..006B970E
                if (static_cast<U8>(recovered_item_can_equip_0051aa60(creature, nullptr, item, &mask, 0, 0, 0)) == 0) continue; // 006B9715..006B971C
                if (proficient == 0) continue;                                                 // 006B9722..006B9724
            }
            void* client_item = kotor_fastcall_00554190(item, nullptr);                        // 006B972A..006B9731 (ebx)
            if (client_item == nullptr) continue;                                              // 006B9733..006B9735 / 006B9818
            void* first = nullptr;                                                             // esi
            void* second = nullptr;                                                            // ebp
            bool pair = false;
            if (lw(self, 0x4278) == 0x10) {                                                    // 006B973B..006B9744 / 006B988E..006B98AC
                first = client_item;
                second = kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr,
                    kotor_m15a_creature_get_equipped_item_id_0060ca80(lp(self, 0x64), nullptr, 0x20));
                pair = true;
            } else if (lw(self, 0x4278) == 0x20) {                                             // 006B974A..006B976C
                first = kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr,
                    kotor_m15a_creature_get_equipped_item_id_0060ca80(lp(self, 0x64), nullptr, 0x10));
                second = client_item;
                pair = true;
            }
            if (pair && first != nullptr && second != nullptr) {                               // 006B976E..006B9774
                bool skip = false;
                if (first == client_item && lb(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0x1B) == 4) skip = true; // 006B9776..006B9787
                if (!skip) {
                    const U32 a = lb(kotor_m15b_item_get_base_item_005b4790(first, nullptr), 8);  // 006B9789..006B9790
                    const U32 b = lb(kotor_m15b_item_get_base_item_005b4790(second, nullptr), 8); // 006B9794..006B979B
                    if (!(a == b && (a == 2 || a == 4))) state = 3;                            // 006B979F..006B97AD
                }
            }
            void* entry = kotor_fastcall_006b91c0(self, nullptr, next);                        // 006B97B2..006B97BE
            const U32 id = lw(client_item, 0x14);                                              // 006B97C6
            if (!(lw(entry, 0x1C4) == id && ((lw(entry, 0x394) >> 1) & 1u) == 0 && lb(entry, 0x398) == state)) changed = 1; // 006B97C0..006B97E6
            kotor_fastcall_006b6710(entry, nullptr, id, 0, 0);                                 // 006B97EE..006B97F5
            kotor_fastcall_006b5060(entry, nullptr, state);                                    // 006B97FA..006B9801
            sb(entry, 0x54, 0xFF);                                                             // 006B980B
            gui_load_array_add_00671c00(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(&list), nullptr, ad(entry)); // 006B9806..006B980F
            ++next;                                                                            // 006B9814
        }
    }
    if (changed != 0 || list.size != lw(self, 0x3378)) {                                       // 006B9834..006B9847
        kotor_fastcall_0041c1d0(at(self, 0x30D8), nullptr, &list, 0, 1, 0);                    // 006B9849..006B985A AddControls(list, 0, 1, 0)
        gui_load_scrollbar_set_cur_004180a0(at(self, 0x31DC), nullptr, 0);                     // 006B985F..006B9867
    }
    const U32 strref = lh(lp(lp(self, 0x64), 0x2F8), 0x24) == 5 ? row.strref_droid : row.strref; // 006B986C..006B98B5 ([0074BF10] = 5)
    {
        ExoResult text;                                                                        // [esp+24h]
        kotor_fastcall_005edeb0(client_app(), nullptr, text.storage, strref); text.live = true; // 006B98BB..006B98CA
        gui_load_text_params_set_text_00415e00(at(self, 0x2E00), nullptr, text.storage);       // 006B98CF..006B98DB
    }                                                                                          // 006B98E0..006B98E9
}                                                                                              // 006B98EE..006B98FB free(list.data)
}  // extern "C"
