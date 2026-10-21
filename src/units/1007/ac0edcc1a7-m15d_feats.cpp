// lane-m15-d / unit m15d-feats: CSWGuiFeatsCharGen 006F2A50..006F4680 (0x1A1C; method table 007598B0), opened by
// CSWGuiCustomPanel::OnSelectFeatsButton 006EF2D0 (L15-C) and, for bonus feats, by its own OnAcceptButton 006F45DE.
// Level-up parent branches (flags bit0; only CSWGuiLevelUpPanel passes level_up = 1, in-game) call the L15-F D83
// function traps 006EE580 / 006EE5D0; the char-gen parent is CSWGuiCustomPanel (L15-C 006EF480 / 006EF530).
// Original: swkotor.exe GOG K1, static disassembly only; every behaviour cites its instruction addresses.
#include "88764680e4-m15d_common.h"

using namespace kotor_m15d;

extern "C" {
extern const MenuColor borrowed_exit_color_007a23b4;      // input-cells 007A23B4
extern const MenuColor kotor_m1_color_007a2378;           // m1-saveload 007A2378 {1,1,1}
extern const MenuColor kotor_m1_color_007a2384;           // m1-saveload 007A2384 {0.35,0.35,0.35}
extern const MenuColor kotor_m1_color_007a2408;           // m1-saveload 007A2408 {0, 0.33, 0.49}
void* __fastcall kotor_m15d_feats_deleting_destruct_006f2f10(FeatsPanel* self, void*, Word flags);
void __fastcall kotor_m15d_feats_destruct_006f2a50(FeatsPanel* self, void*);
void __fastcall kotor_m15d_feats_handle_cancel_006f2c70(FeatsPanel* self, void*) noexcept(false);
void __fastcall kotor_m15d_feats_draw_006f2cc0(FeatsPanel* self, void*, Word delta) noexcept(false);
void __fastcall kotor_m15d_feats_determine_feat_006f2d30(FeatsPanel* self, void*, Word feat, U8* out);
void __fastcall kotor_m15d_feats_set_description_006f2df0(FeatsPanel* self, void*, Word strref) noexcept(false);
void __fastcall kotor_m15d_feats_on_panel_added_006f2f30(FeatsPanel* self, void*) noexcept(false);
void __fastcall kotor_m15d_feats_on_enter_feat_006f2fb0(FeatsPanel* self, void*, Word feat) noexcept(false);
void __fastcall kotor_m15d_feats_build_buttons_006f30f0(FeatsPanel* self, void*) noexcept(false);
void __fastcall kotor_m15d_feats_build_available_list_006f3290(FeatsPanel* self, void*) noexcept(false);
void __fastcall kotor_m15d_feats_initiate_feats_006f3460(FeatsPanel* self, void*) noexcept(false);
void __fastcall kotor_m15d_feats_add_chosen_feat_006f37e0(FeatsPanel* self, void*, Word feat) noexcept(false);
void __fastcall kotor_m15d_feats_remove_chosen_feat_006f3990(FeatsPanel* self, void*, Word feat) noexcept(false);
void __fastcall kotor_m15d_feats_on_recommend_006f3a80(FeatsPanel* self, void*) noexcept(false);
void __fastcall kotor_m15d_feats_on_feat_picked_006f3c20(FeatsPanel* self, void*, Word feat) noexcept(false);
void __fastcall kotor_m15d_feats_on_accept_006f44c0(FeatsPanel* self, void*) noexcept(false);
void __fastcall kotor_m15d_feats_handle_input_006f4680(FeatsPanel* self, void*, I32 event, I32 value) noexcept(false);
}

namespace {
const unsigned char k_tlk_flag_0073ecd0 = 0x00;           // .rdata byte 0073ECD0 (image value 00; as gui-load gui_params)
FeatsPanel* feats_of(FeatsHandlers* h) { return reinterpret_cast<FeatsPanel*>(h); }
gui_load::EventFunc handler(void (__thiscall FeatsHandlers::*f)(kih::Control*)) { return static_cast<gui_load::EventFunc>(f); }
void* skill_info_box() { return rp(frame_R4_005ed690(client_app(), nullptr), 0x9C); }   // [CGuiInGame+9Ch]
// mb = [CGuiInGame+98h]: SetAllowCancel(0); SetCallback(0, 0, 0); vslot +6C SetMessage(strref); AddPanel(mb, 1, 1)
// (006F3C97..D1 / 006F44F0..2D).
void show_message(FeatsPanel* self, Word strref) {
    void* const box = message_box();
    kotor_fastcall_00627130(box, nullptr, 0);
    gui_load_messagebox_set_callback_00624a40(box, nullptr, nullptr, nullptr, 0);
    vt<MessageBoxView>(box)->set_message_strref(box, nullptr, strref);
    kotor_fastcall_0040bc70(self->manager, nullptr, box, 1, 1);
}
// Text and colours of BTN_SELECT (text params +13A4, border colour +12C8): 006F2FEE..30B1 / 006F2CD6..D1A.
void set_select_button(FeatsPanel* self, StartupString* text, const MenuColor* text_color, const MenuColor* border_color) {
    void* const params = at(&self->btn_select, kButtonText);                       // +13A4
    gui_load_text_params_set_text_00415e00(params, nullptr, text);
    gui_load_text_params_set_color_00414e10(params, nullptr, text_color);
    copy_color(at(&self->btn_select, kButtonBorderColor), border_color);           // +12C8
}
// Close sequence (006F2C7A..9C / 006F457A..A1): PopModalPanel, parent SetVisible(1), panel state 400h.
void close_to_parent(FeatsPanel* self) {
    kotor_fastcall_0040be00(self->manager, nullptr);                               // PopModalPanel 0040BE00
    kotor_m15_panel_set_visible_0040b9d0(self->parent, nullptr, 1);                // SetVisible 0040B9D0 ([this+68h])
    self->flags = (self->flags & 0xFFFFFCFFu) | 0x400u;                            // [this+44h]
}
// CExoString temporary of the given TLK strref (CTlkTable::GetSimpleString 0041E8F0, hidden return).
void simple_string(StringSlot& slot, Word strref) {
    tlk()->simple_0041e8f0(reinterpret_cast<kotor_menu_10_19::String*>(slot.raw()), strref);
    slot.live = true;
}
// Member construction stages of 006F3D60 (EH states 0..16h of FuncInfo 00739917): built = number of members
// constructed in declaration order (1..10 labels, 11..14 buttons, 15..16 list boxes, 17 lists zeroed,
// 18..19 strings, 20 chart). Unwind destroys them in reverse, then the CSWGuiPanel base.
Label* label_at(FeatsPanel* self, int i) { return reinterpret_cast<Label*>(at(self, 0x6Cu + 0x140u * static_cast<unsigned>(i))); }      // 10 labels
Button* button_at(FeatsPanel* self, int i) { return reinterpret_cast<Button*>(at(self, 0xCECu + 0x1C4u * static_cast<unsigned>(i))); }  // 4 buttons
void unwind_feats(FeatsPanel* self, int built) {
    if (built >= 20) kotor_m15d_chart_destruct_006cd530(&self->chart, nullptr);
    if (built >= 19) self->remove_text.~StartupString();
    if (built >= 18) self->select_text.~StartupString();
    if (built >= 17) {
        live_free(ptr_of(self->chosen.data));
        live_free(ptr_of(self->available.data));
        live_free(ptr_of(self->granted_new.data));
        live_free(ptr_of(self->granted.data));
    }
    if (built >= 16) gui_load_listbox_destruct_0041b060(&self->lb_desc, nullptr);
    if (built >= 15) gui_load_listbox_destruct_0041b060(&self->lb_feats, nullptr);
    for (int i = 4; i-- > 0;) if (built >= 11 + i) gui_load_button_destruct_00419da0(button_at(self, i), nullptr);
    for (int i = 10; i-- > 0;) if (built >= 1 + i) gui_load_label_destruct_00419d10(label_at(self, i), nullptr);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
}

// ======================================================================== D21 method table 007598B0 (27 slots)
// Read from .rdata 007598B0..0075991B: CSWGuiPanel 0073E010 except +00 006F2F10, +34 006F2CC0, +3C 006F4680,
// +48 006F2F30.
struct GuiM15dFeatsMethods {
    void* (__fastcall* s00)(FeatsPanel*, void*, Word);                        // +00 006F2F10 deleting dtor
    void (__fastcall* s04)(Object*, void*, const Extent*);                    // +04 0040A5A0
    void (__fastcall* s08)(Object*, void*, Object*, Word);                    // +08 0040A630
    void (__fastcall* s0c)(Object*, void*, Word);                             // +0C 0060E760
    Word (__fastcall* s10)(Object*, void*, Word, Word);                       // +10 00409EB0
    void (__fastcall* s14)(Object*, void*);                                   // +14 004015A0
    void (__fastcall* s18)(Object*, void*);                                   // +18 004015A0
    void (__fastcall* s1c)(Object*, void*);                                   // +1C 004015A0
    void (__fastcall* s20)(Object*, void*);                                   // +20 004015A0
    void (__fastcall* s24)(Object*, void*);                                   // +24 004015A0
    Object* (__fastcall* s28)(Object*, void*);                                // +28 00641DB0
    Word (__fastcall* s2c)(Object*, void*);                                   // +2C 0063E7F0
    Word (__fastcall* s30)(Object*, void*);                                   // +30 0063E7F0
    void (__fastcall* s34)(FeatsPanel*, void*, Word);                         // +34 006F2CC0 Draw
    void (__fastcall* s38)(Object*, void*, Word);                             // +38 0060E760
    void (__fastcall* s3c)(FeatsPanel*, void*, I32, I32);                     // +3C 006F4680 HandleInputEvent
    Object* (__fastcall* s40)(Object*, void*, I32, I32);                      // +40 0040B690
    Object* (__fastcall* s44)(Object*, void*);                                // +44 0040B630
    void (__fastcall* s48)(FeatsPanel*, void*);                               // +48 006F2F30 OnPanelAdded
    void (__fastcall* s4c)(Panel*, void*);                                    // +4C 0040C170
    void (__fastcall* s50)(Object*, void*, Word);                             // +50 0040B640
    void (__fastcall* s54)(Object*, void*, Word);                             // +54 0040B650
    void (__fastcall* s58)(Object*, void*, Word);                             // +58 0040B660
    void (__fastcall* s5c)(Object*, void*, Word);                             // +5C 0040B670
    void (__fastcall* s60)(Object*, void*, Word);                             // +60 0040B680
    String* (__fastcall* s64)(Panel*, void*, String*);                        // +64 0040A900
    void (__fastcall* s68)(Panel*, void*);                                    // +68 0040A9C0
};
static_assert(sizeof(GuiM15dFeatsMethods) == 0x6C, "007598B0 slot count (27)");
extern "C" const GuiM15dFeatsMethods gui_m15d_feats_methods_007598b0 = {
    &kotor_m15d_feats_deleting_destruct_006f2f10,   // +00
    &gui_load_panel_set_extent_0040a5a0,            // +04
    &gui_load_panel_set_active_control_0040a630,    // +08
    &gui_load_slot_ret4_0060e760,                   // +0C
    &gui_load_slot_zero8_00409eb0,                  // +10
    &gui_load_slot_ret_004015a0,                    // +14
    &gui_load_slot_ret_004015a0,                    // +18
    &gui_load_slot_ret_004015a0,                    // +1C
    &gui_load_slot_ret_004015a0,                    // +20
    &gui_load_slot_ret_004015a0,                    // +24
    &gui_load_slot_self_00641db0,                   // +28
    &gui_load_slot_zero_0063e7f0,                   // +2C
    &gui_load_slot_zero_0063e7f0,                   // +30
    &kotor_m15d_feats_draw_006f2cc0,                // +34
    &gui_load_slot_ret4_0060e760,                   // +38
    &kotor_m15d_feats_handle_input_006f4680,        // +3C
    &gui_load_panel_hit_check_0040b690,             // +40
    &gui_load_panel_active_control_0040b630,        // +44
    &kotor_m15d_feats_on_panel_added_006f2f30,      // +48
    &gui_load_panel_on_removed_0040c170,            // +4C
    &gui_load_panel_button_a_0040b640,              // +50
    &gui_load_panel_button_b_0040b650,              // +54
    &gui_load_panel_button_x_0040b660,              // +58
    &gui_load_panel_button_y_0040b670,              // +5C
    &gui_load_panel_button_black_0040b680,          // +60
    &gui_load_panel_full_screen_bg_0040a900,        // +64
    &gui_load_panel_reset_font_0040a9c0,            // +68
};

// ======================================================================== member callbacks (thiscall, RET 4)
// 006F2C40 ShowGranted (tutorial-box callback): AddPanel(manager, [CGuiInGame+9Ch], 1, 1).
void __thiscall FeatsHandlers::ShowGranted_006f2c40(kih::Control*) {
    FeatsPanel* const self = feats_of(this);
    kotor_fastcall_0040bc70(self->manager, nullptr, skill_info_box(), 1, 1);       // 006F2C40..5E
}
// 006F3420 OnFeatSelectionChanged (LB_FEATS event 1F8h): hit-test the selected flow at the list's local mouse.
void __thiscall FeatsHandlers::OnFeatSelectionChanged_006f3420(kih::Control*) {
    FeatsPanel* const self = feats_of(this);
    SkillFlow* const flow = static_cast<SkillFlow*>(static_cast<Control*>(
        kotor_m1_listbox_get_selected_control_0041a5d0(&self->lb_feats, nullptr)));   // 006F3424..2A
    const I32 skill = kotor_m15d_skill_flow_hit_check_006cd1c0(flow, nullptr, self->lb_feats.local_x,
                                                                self->lb_feats.local_y);   // 006F342F..3F (+147C/+1480)
    kotor_m15d_chart_set_selected_skill_006cdc00(&self->chart, nullptr, skill);     // 006F3446..4D
    kotor_m15d_feats_on_enter_feat_006f2fb0(self, nullptr, static_cast<Word>(skill));   // 006F3452..55
}
// 006F3CF0 OnDoubleClick (LB_FEATS event 1F9h): picks the feat when the hit cell is the selected one.
void __thiscall FeatsHandlers::OnDoubleClick_006f3cf0(kih::Control*) {
    FeatsPanel* const self = feats_of(this);
    I32 selected = -1;                                                             // 006F3D24
    if (self->chart.flows.count > 0)                                               // 006F3CF3..FC
        selected = flow_at(self->chart, self->chart.row)->cells[self->chart.col].skill;   // 006F3CFE..1B (inline GetSelected)
    SkillFlow* const flow = static_cast<SkillFlow*>(static_cast<Control*>(
        kotor_m1_listbox_get_selected_control_0041a5d0(&self->lb_feats, nullptr)));   // 006F3D27..2D
    const I32 hit = kotor_m15d_skill_flow_hit_check_006cd1c0(flow, nullptr, self->lb_feats.local_x,
                                                              self->lb_feats.local_y);   // 006F3D32..42
    if (hit == selected)                                                           // 006F3D47..49
        kotor_m15d_feats_on_feat_picked_006f3c20(self, nullptr, static_cast<Word>(hit));   // 006F3D4B..4E
}

extern "C" {
// ======================================================================== 006F2A50 dtor (RET)
void __fastcall kotor_m15d_feats_destruct_006f2a50(FeatsPanel* self, void*) {
    gui_lb::set_vptr(self, &gui_m15d_feats_methods_007598b0);                     // 006F2A70 (7598B0)
    gui_load_listbox_clear_items_00419a90(&self->lb_desc, nullptr);                // 006F2A76..86
    gui_load_listbox_clear_items_00419a90(&self->lb_feats, nullptr);               // 006F2A8B..93
    kotor_m15d_chart_destruct_006cd530(&self->chart, nullptr);                     // 006F2A98..A3
    self->remove_text.~StartupString();                                            // 006F2AA8..B3
    self->select_text.~StartupString();                                            // 006F2AB8..C3
    live_free(ptr_of(self->chosen.data));      self->chosen.data = 0;              // 006F2AC8..E1
    live_free(ptr_of(self->available.data));   self->available.data = 0;           // 006F2AE1..F2
    live_free(ptr_of(self->granted_new.data)); self->granted_new.data = 0;         // 006F2AF8..09
    live_free(ptr_of(self->granted.data));     self->granted.data = 0;             // 006F2B0F..25
    gui_load_listbox_destruct_0041b060(&self->lb_desc, nullptr);                   // 006F2B23..30
    gui_load_listbox_destruct_0041b060(&self->lb_feats, nullptr);                  // 006F2B35..3C
    for (int i = 4; i-- > 0;) gui_load_button_destruct_00419da0(button_at(self, i), nullptr);   // 006F2B41..7C
    for (int i = 10; i-- > 0;) gui_load_label_destruct_00419d10(label_at(self, i), nullptr);    // 006F2B81..C18
    gui_load_panel_destruct_0040cf70(self, nullptr);                               // 006F2C1D..27
}
// 006F2F10 scalar deleting destructor (RET 4).
void* __fastcall kotor_m15d_feats_deleting_destruct_006f2f10(FeatsPanel* self, void*, Word flags) {
    kotor_m15d_feats_destruct_006f2a50(self, nullptr);                             // 006F2F13
    if (flags & 1u) live_free(self);                                               // 006F2F18..25
    return self;                                                                   // 006F2F28
}


// ======================================================================== 006F2C70 HandleCancelButton (RET)
void __fastcall kotor_m15d_feats_handle_cancel_006f2c70(FeatsPanel* self, void*) noexcept(false) {
    if (self->parent == nullptr) return;                                           // 006F2C73..78
    close_to_parent(self);                                                         // 006F2C7A..9C
    if ((self->feat_flags & 1u) != 0) kotor_fastcall_006ee580(self->parent, nullptr);   // 006F2C8F..A9 (L15-F trap)
    else kotor_m15c_custom_cancelled_by_child_006ef480(self->parent, nullptr);     // 006F2CAE (jmp, ECX = [this+68h])
}

// ======================================================================== 006F2CC0 Draw(delta) (RET 4)
// Status 2/3 (+1A00 text) or 4 (+19F8 text) repaint BTN_SELECT disabled (007A2408 text, 007A2384 border).
void __fastcall kotor_m15d_feats_draw_006f2cc0(FeatsPanel* self, void*, Word delta) noexcept(false) {
    const U8 status = self->status;                                                // 006F2CC3
    if (status >= 2) {                                                             // 006F2CC9..CB
        StartupString* text = nullptr;
        if (status <= 3) text = &self->remove_text;                                // 006F2CCD..D0 / 006F2CDE
        else if (status == 4) text = &self->select_text;                           // 006F2CD2..D6
        if (text != nullptr)
            set_select_button(self, text, &kotor_m1_color_007a2408, &kotor_m1_color_007a2384);   // 006F2CE4..D1A
    }
    kotor_panel_draw_0040B760(self, nullptr, delta);                               // 006F2D1E..25 (0040B760)
}

// ======================================================================== 006F2D30 DetermineFeat(feat, BYTE* out) (RET 8)
// 0 available, 1 chosen, 3 owned, 2 granted at this level, 4 none.
void __fastcall kotor_m15d_feats_determine_feat_006f2d30(FeatsPanel* self, void*, Word feat, U8* out) {
    const U16 id = static_cast<U16>(feat);
    if (find_u16(self->available, id) != -1) { *out = 0; return; }                 // 006F2D30..4B / 006F2DB4..BE
    if (find_u16(self->chosen, id) != -1) { *out = 1; return; }                    // 006F2D55..6A / 006F2DC5..CF
    if (find_u16(self->granted, id) != -1) { *out = 3; return; }                   // 006F2D74..89 / 006F2DD6..E0
    const std::int32_t index = kotor_m15_array_u16_index_of_006f2a20(&self->granted_new, nullptr, feat);   // 006F2D93..9A
    *out = static_cast<U8>(index == -1 ? 4 : 2);                                   // 006F2DA3..AF
}

// ======================================================================== 006F2DF0 SetDescription(strref) (RET 4)
// STR_RES {CExoString, CResRef} filled by CTlkTable::Fetch; description item text, height, LB_DESC refill.
void __fastcall kotor_m15d_feats_set_description_006f2df0(FeatsPanel* self, void*, Word strref) noexcept(false) {
    struct StrRes { unsigned char text[8]; Ref sound; Word duration; } res;        // [esp+1Ch] / [esp+24h]
    StartupString* const text = ::new (static_cast<void*>(res.text)) StartupString();   // 006F2E0C..10 (005B3190)
    try {
        c2da_default_ref_00405ed0(&res.sound, nullptr);                            // 006F2E15..21 (00405ED0)
        tlk_fetch_0041e550(kotor_cell_tlk_007a3a08, nullptr, strref, &res, k_tlk_flag_0073ecd0);   // 006F2E26..46
        gui_load_text_params_set_text_00415e00(at(&self->desc_item, kLabelText), nullptr, text);   // 006F2E4B..56 (+B54)
        gui_load_listbox_clear_items_00419a90(&self->lb_desc, nullptr);            // 006F2E5B..63
        Extent extent = self->desc_item.extent;                                    // 006F2E68..89 (+A70)
        gui_load::Text* const text_object = &self->desc_item.text;                 // 006F2E94 (+B3C)
        if (text->BorrowedLength005e5790() > 0)                                    // 006F2E81..9A (005E5790)
            extent.height = static_cast<I32>(vt<TextIdealView>(text_object)->ideal_height(text_object, nullptr));   // 006F2E9C..9E
        else
            extent.height = gui_load_text_font_height_00414ee0(text_object, nullptr);   // 006F2EA3 (00414EE0)
        Label* const item = &self->desc_item;                                      // 006F2EAC (+A6C)
        item->vtbl->set_extent(item, nullptr, &extent);                            // 006F2EB6..BB vslot +04
        Control* items[1] = {item};                                                // 006F2ECD (argument slot)
        gui_m48_listbox_add_controls_0041c2c0(&self->lb_desc, nullptr, items, 1, 0, 0, 0);   // 006F2EBE..D1
        gui_load_listbox_set_active_control_0041c160(&self->lb_desc, nullptr, item, 0);       // 006F2ED6..DB
    } catch (...) {
        text->~StartupString();                                                    // EH state 1/0
        throw;
    }
    text->~StartupString();                                                        // 006F2EE0..EC (005E5C20)
}

// ======================================================================== 006F2F30 OnPanelAdded (RET)
// With the granted-feats list prepared (flags bit2): tutorial reason 12h; when it shows, the tutorial box
// ([CGuiInGame+A0h]) calls ShowGranted on OK, otherwise the skill-info box is added now.
void __fastcall kotor_m15d_feats_on_panel_added_006f2f30(FeatsPanel* self, void*) noexcept(false) {
    gui_load_panel_on_added_0040b870(self, nullptr);                               // 006F2F33 (0040B870)
    if ((self->feat_flags & 4u) == 0) return;                                      // 006F2F38..3F
    if (show_tutorial_window_005edf40(client_app(), nullptr, 0x12, 0x7F000000u, 0x7F000000u, 0) != 0) {   // 006F2F41..5E
        void* const box = rp(frame_R4_005ed690(client_app(), nullptr), 0xA0);       // 006F2F60..6E
        gui_load_messagebox_set_callback_00624a40(box, nullptr, event_target(self),
                                                  handler(&FeatsHandlers::ShowGranted_006f2c40), 0);   // 006F2F74..7C
        return;
    }
    kotor_fastcall_0040bc70(self->manager, nullptr, skill_info_box(), 1, 1);       // 006F2F83..9F
}

// ======================================================================== 006F2FB0 OnEnterFeat(feat) (RET 4)
void __fastcall kotor_m15d_feats_on_enter_feat_006f2fb0(FeatsPanel* self, void*, Word feat) noexcept(false) {
    kotor_m15d_feats_determine_feat_006f2d30(self, nullptr, feat, &self->status);  // 006F2FB9..C1 (+1A18)
    self->current_feat = static_cast<U16>(feat);                                   // 006F2FC6
    const void* const info = feat_of(static_cast<U16>(feat));                      // 006F2FCD..D9
    switch (self->status) {                                                        // 006F2FDB..E7 (table 006F30D4)
    case 0:                                                                        // 006F2FEE
        set_select_button(self, &self->select_text, &borrowed_exit_color_007a23b4, &kotor_m1_color_007a2378);
        break;
    case 1:                                                                        // 006F302F
        set_select_button(self, &self->remove_text, &borrowed_exit_color_007a23b4, &kotor_m1_color_007a2378);
        break;
    case 2: case 3:                                                                // 006F306D
        set_select_button(self, &self->remove_text, &kotor_m1_color_007a2408, &kotor_m1_color_007a2384);
        break;
    case 4:                                                                        // 006F3075
        set_select_button(self, &self->select_text, &kotor_m1_color_007a2408, &kotor_m1_color_007a2384);
        break;
    default:
        break;
    }
    gui_load_text_params_set_strref_00415e50(at(&self->lbl_name, kLabelText), nullptr, rd32(info, 0x08));   // 006F30B4..BE (+C94)
    kotor_m15d_feats_set_description_006f2df0(self, nullptr, rd32(info, 0x0C));    // 006F30C3..C9
}

// ======================================================================== 006F30F0 BuildButtons (RET)
void __fastcall kotor_m15d_feats_build_buttons_006f30f0(FeatsPanel* self, void*) noexcept(false) {
    SkillFlowChart* const chart = &self->chart;                                    // 006F310D (+1A08)
    kotor_m15d_chart_set_skill_status_all_006cd2e0(chart, nullptr, 3);            // 006F3113..17
    for (I32 i = 0; i < self->granted.count; ++i)                                  // 006F311C..4D
        kotor_m15d_chart_set_skill_status_006cd280(chart, nullptr, u16_at(self->granted, i), 1);
    for (I32 i = 0; i < self->chosen.count; ++i)                                   // 006F314F..7D
        kotor_m15d_chart_set_skill_status_006cd280(chart, nullptr, u16_at(self->chosen, i), 4);
    for (I32 i = 0; i < self->available.count; ++i)                                // 006F317F..AD
        kotor_m15d_chart_set_skill_status_006cd280(chart, nullptr, u16_at(self->available, i), 0);
    for (I32 i = 0; i < self->granted_new.count; ++i)                              // 006F31AF..DD
        kotor_m15d_chart_set_skill_status_006cd280(chart, nullptr, u16_at(self->granted_new, i), 2);
    if (self->current_feat == 0xFFFFu) {                                           // 006F31DF..E8
        kotor_fastcall_0041c1d0(&self->lb_feats, nullptr, chart, 0, 1, 0);         // 006F31EA..F7 AddControls(list,0,1,0)
        self->current_feat = static_cast<U16>(kotor_m15d_chart_set_top_left_006cdd10(chart, nullptr));   // 006F31FC..203
    }
    kotor_m15d_chart_set_selected_skill_006cdc00(chart, nullptr, self->current_feat);   // 006F320A..14
    gui_load_listbox_set_selected_0041c040(&self->lb_feats, nullptr, chart->row, 0);    // 006F3219..2B (+1A15)
    kotor_m15d_feats_on_enter_feat_006f2fb0(self, nullptr, self->current_feat);         // 006F3230..3C
    set_text_int(at(&self->lbl_std_remaining, kLabelText), self->remaining);       // 006F3241..76 (+514, 005E5BC0)
}

// ======================================================================== 006F3290 BuildAvailableList (RET)
void __fastcall kotor_m15d_feats_build_available_list_006f3290(FeatsPanel* self, void*) noexcept(false) {
    StartupString unused;                                                          // 006F32AB..AF (005B3190; EH state 0)
    void* const stats = stats_of(self->creature);                                  // 006F32B4..BD
    for (I32 i = 0; i < static_cast<I32>(ld<U16>(rules(), 0xA6)); ++i) {           // 006F32C1..DB / 006F33D5..ED
        const U16 feat = ld<U16>(rp(rules(), 0xA0), 2u * static_cast<unsigned>(i));   // 006F32F0..F9
        if (find_u16(self->granted, feat) != -1) continue;                         // 006F32FD..22 (+19BC)
        if (find_u16(self->granted_new, feat) != -1) continue;                     // 006F3328..52 (+19C8)
        if (find_u16(self->chosen, feat) != -1) continue;                          // 006F3358..82 (+19E0)
        if (find_u16(self->available, feat) != -1) continue;                       // 006F3384..AA (+19D4)
        if (kotor_m15b_levelup_stats_can_choose_feat_0064a4c0(stats, nullptr, feat, self->normal_left,
                                                               self->bonus_left, &self->chosen) == 0)   // 006F33AC..CB
            continue;
        kotor_m15_array_u16_add_004d4330(&self->available, nullptr, feat);         // 006F33CD..D0
    }
}                                                                                  // 006F33F6..402 (005E5C20)

// ======================================================================== 006F3460 InitiateFeats (RET)
void __fastcall kotor_m15d_feats_initiate_feats_006f3460(FeatsPanel* self, void*) noexcept(false) {
    void* const stats = stats_of(self->creature);                                  // 006F347C..7F
    if ((self->feat_flags & 3u) == 0) kotor_m15b_creature_stats_clear_feats_00648320(stats, nullptr);   // 006F3485..94
    for (I32 i = 0; i < static_cast<I32>(ld<U16>(stats, 0xB4)); ++i) {             // 006F3499..A6 / 006F3551..5B
        const U16 feat = static_cast<U16>(kotor_m15b_creature_stats_get_feat_006486e0(stats, nullptr, static_cast<U16>(i)));   // 006F34B0..B3
        if (self->granted.count == self->granted.capacity) grow_list(self->granted, 2);   // 006F34B8..533
        set_u16_at(self->granted, self->granted.count, feat);                      // 006F3536..47
        ++self->granted.count;                                                     // 006F354B
    }
    self->feat_flags &= ~4u;                                                       // 006F3561..6E
    if ((self->feat_flags & 2u) == 0) {                                            // 006F356C..74
        ExoList added{0, 0, 0};                                                    // 006F357A..84 (EH state 0)
        try {
            kotor_m15b_levelup_stats_add_granted_feats_00649950(stats, nullptr, &added);   // 006F3588..93
            I32 n = added.count;                                                   // 006F3598
            if (n > 0) {                                                           // 006F359C..9E
                for (I32 i = 0; i < n; ++i) {                                      // 006F35A4..64A
                    const U16 feat = static_cast<U16>(u32_at(added, i));           // 006F35B0..B4
                    if (self->granted_new.count == self->granted_new.capacity) grow_list(self->granted_new, 2);   // 006F35B8..623
                    set_u16_at(self->granted_new, self->granted_new.count, feat);  // 006F3626..32
                    ++self->granted_new.count;                                     // 006F3636..44
                    n = added.count;                                               // 006F363C (re-read)
                }
                for (I32 j = n - 1; j >= 0; --j) {                                 // 006F3650..C9: drop already-owned feats
                    I32 matches = 0;                                               // 006F3668
                    const U16 feat = static_cast<U16>(u32_at(added, j));           // 006F3672
                    for (I32 g = 0; g < self->granted.count; ++g)                  // 006F3660..91
                        if (u16_at(self->granted, g) == feat) ++matches;           // 006F3680..85
                    if (matches == 0) continue;                                    // 006F3693..99
                    --n;                                                           // 006F369B
                    added.count = n;                                               // 006F36A0
                    for (I32 k = j; k < n; ++k) set_u32_at(added, k, u32_at(added, k + 1));   // 006F36B0..C2
                }
                void* const box = skill_info_box();                                // 006F36CB..D9
                kotor_m15d_skill_info_set_skill_list_006cdfc0(box, nullptr, &added, 1);   // 006F36DF..E8
                gui_load_text_params_set_strref_00415e50(at(box, 0x42C), nullptr, 0xA512);   // 006F36ED..F8
                self->feat_flags |= 4u;                                            // 006F36FD..70A
            }
        } catch (...) {
            live_free(ptr_of(added.data));                                         // EH state 0 (006FA390)
            throw;
        }
        live_free(ptr_of(added.data));                                             // 006F3710..1D
        added.data = 0;                                                            // 006F3725
    }
    kotor_m15b_levelup_stats_calc_number_feats_00648fe0(stats, nullptr, &self->normal_count, &self->bonus_count);   // 006F372D..3D
    self->normal_left = static_cast<U8>(self->normal_count != 0 ? self->normal_count + 1 : 0);   // 006F3742..51
    self->bonus_left = static_cast<U8>(self->bonus_count != 0 ? self->bonus_count + 1 : 0);     // 006F3757..70
    if (((self->feat_flags >> 1) & 1u) != 0) self->normal_count = 0;              // 006F3765..78
    else self->bonus_count = 0;                                                    // 006F377E
    self->remaining = static_cast<U8>(self->normal_count + self->bonus_count);     // 006F3781..87
    if (self->remaining == 0) {                                                    // 006F378E
        kotor_m15d_feats_on_accept_006f44c0(self, nullptr);                        // 006F3790..92
        return;
    }
    kotor_m15d_chart_create_feat_chart_006ce570(&self->chart, nullptr, stats, (self->feat_flags >> 1) & 1u);   // 006F37A8..B0
    kotor_m15d_feats_build_available_list_006f3290(self, nullptr);                 // 006F37B5..B7
    kotor_m15d_feats_build_buttons_006f30f0(self, nullptr);                        // 006F37BC..BE
}

// ======================================================================== 006F37E0 AddChosenFeat(feat) (RET 4)
// Moves the feat from the available to the chosen list, keeping the chosen list sorted by feat name.
void __fastcall kotor_m15d_feats_add_chosen_feat_006f37e0(FeatsPanel* self, void*, Word feat) noexcept(false) {
    const I32 index = find_u16(self->available, static_cast<U16>(feat));           // 006F3802..2E
    if (index == -1) return;                                                       // 006F3830..41 / 006F3844..47
    --self->available.count;                                                       // 006F384D..58
    for (I32 i = index; i < self->available.count; ++i)                            // 006F3856..7A
        set_u16_at(self->available, i, u16_at(self->available, i + 1));
    StringSlot name;                                                               // [esp+10h]
    kotor_m15b_feat_get_name_text_005cd760(feat_of(static_cast<U16>(feat)), nullptr, name.raw());   // 006F387C..93
    name.live = true;
    bool before = false;                                                           // [esp+0Ch] (006F3888)
    I32 i = 0;
    for (; i < self->chosen.count; ++i) {                                          // 006F389C..F2
        int greater = 0;
        {
            StringSlot other;                                                      // [esp+18h]
            kotor_m15b_feat_get_name_text_005cd760(feat_of(u16_at(self->chosen, i)), nullptr, other.raw());   // 006F38A8..C7
            other.live = true;
            greater = alias_add::Greater005e5490(other.get(), nullptr, name.get());   // 006F38CC..D8 (operator>)
        }                                                                          // 006F38DD..E8 (005E5C20)
        if (greater != 0) { before = true; break; }                                // 006F38ED..F4
    }
    --self->remaining;                                                             // 006F38FC..0A
    if (before) {
        kotor_m15_array_i16_insert_006169a0(&self->chosen, nullptr, feat, i);      // 006F3912..1D
    } else {
        if (self->chosen.count == self->chosen.capacity)                           // 006F3924..38
            kotor_m15_array_u16_allocate_004d2fa0(&self->chosen, nullptr,
                self->chosen.capacity != 0 ? self->chosen.capacity * 2 : 0x10);    // 006F393A..4A
        set_u16_at(self->chosen, self->chosen.count, static_cast<U16>(feat));      // 006F394F..58
        ++self->chosen.count;                                                      // 006F395C
    }
    kotor_m15d_feats_build_available_list_006f3290(self, nullptr);                 // 006F395F..61
}                                                                                  // 006F3966..72 (005E5C20)

// ======================================================================== 006F3990 RemoveChosenFeat(feat) (RET 4)
// Removes the feat; any chosen feat whose prerequisites now fail is removed recursively, else the available list
// is rebuilt from scratch.
void __fastcall kotor_m15d_feats_remove_chosen_feat_006f3990(FeatsPanel* self, void*, Word feat) noexcept(false) {
    const I32 index = find_u16(self->chosen, static_cast<U16>(feat));              // 006F3993..BE
    if (index == -1) return;                                                       // 006F39C0..C4 / 006F39C7..CA
    --self->chosen.count;                                                          // 006F39D0..D8
    for (I32 i = index; i < self->chosen.count; ++i)                               // 006F39D6..F3
        set_u16_at(self->chosen, i, u16_at(self->chosen, i + 1));
    ++self->remaining;                                                             // 006F39F5..A00
    void* const stats = stats_of(self->creature);                                  // 006F39FB..A06
    for (I32 i = 0; i < self->chosen.count; ++i) {                                 // 006F3A0C..42
        const U16 other = u16_at(self->chosen, i);                                 // 006F3A18..1C
        if (kotor_m15b_levelup_stats_has_feat_prereq_006498a0(stats, nullptr, feat_of(other), &self->chosen) == 0) {   // 006F3A20..37
            kotor_m15d_feats_remove_chosen_feat_006f3990(self, nullptr, other);    // 006F3A5C..67
            return;
        }
    }
    self->available.count = 0;                                                     // 006F3A46
    kotor_m15d_feats_build_available_list_006f3290(self, nullptr);                 // 006F3A44..50
}

// ======================================================================== 006F3A80 OnRecommendButton (RET)
// Replaces the chosen feats with the class package (AddDefaultPackageFeats) and shows them in the skill-info box.
void __fastcall kotor_m15d_feats_on_recommend_006f3a80(FeatsPanel* self, void*) noexcept(false) {
    ExoList recommended{0, 0, 0};                                                  // [esp+18h] (EH state 0)
    ExoList shown{0, 0, 0};                                                        // [esp+0Ch] (EH state 1)
    try {
        void* const stats = stats_of(self->creature);                              // 006F3ABB..BE
        const U8 class_id = kotor_m15b_creature_stats_get_class_00647730(
            stats, nullptr, static_cast<U8>(ld<U8>(stats, 0x32) - 1));            // 006F3AC4..D1
        const Word flags = self->feat_flags;                                       // 006F3AE2
        kotor_m15b_levelup_stats_add_default_package_feats_0064a770(stats_of(self->creature), nullptr, class_id,
            &recommended, static_cast<std::int32_t>((~(flags >> 1)) & 1u),
            static_cast<std::int32_t>((flags >> 1) & 1u));                         // 006F3AD6..FC
        while (self->chosen.count != 0)                                            // 006F3B01..29
            kotor_m15d_feats_remove_chosen_feat_006f3990(self, nullptr, u16_at(self->chosen, 0));
        const I32 count = recommended.count;                                       // 006F3B2B
        if (count == 0) {                                                          // 006F3B2F..33
            if (shown.data != 0) { live_free(ptr_of(shown.data)); shown.data = 0; shown.capacity = 0; }   // 006F3B35..4A
        } else {
            menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(&shown),
                                                nullptr, static_cast<Word>(count));   // 006F3B50..55
        }
        shown.count = count;                                                       // 006F3B5E
        for (I32 i = 0; i < recommended.count; ++i) {                              // 006F3B62..98
            kotor_m15d_feats_add_chosen_feat_006f37e0(self, nullptr, u16_at(recommended, i));   // 006F3B70..7D
            set_u32_at(shown, i, u16_at(recommended, i));                          // 006F3B82..8E
        }
        void* const box = skill_info_box();                                        // 006F3B9A..A8
        kotor_m15d_skill_info_set_skill_list_006cdfc0(box, nullptr, &shown, 1);    // 006F3BAE..B7
        gui_load_text_params_set_strref_00415e50(at(box, 0x42C), nullptr, 0xA510); // 006F3BBC..C7
        kotor_fastcall_0040bc70(self->manager, nullptr, box, 1, 1);                // 006F3BCC..D4
        kotor_m15d_feats_build_buttons_006f30f0(self, nullptr);                    // 006F3BD9..DB
    } catch (...) {
        live_free(ptr_of(shown.data));                                             // EH state 1
        live_free(ptr_of(recommended.data));                                       // EH state 0
        throw;
    }
    live_free(ptr_of(shown.data));                                                 // 006F3BE0..E9
    shown.data = 0;                                                                // 006F3BF3
    live_free(ptr_of(recommended.data));                                           // 006F3BEE..FF
}

// ======================================================================== 006F3C20 OnFeatPicked(feat) (RET 4)
void __fastcall kotor_m15d_feats_on_feat_picked_006f3c20(FeatsPanel* self, void*, Word feat) noexcept(false) {
    U8 status = 0;                                                                 // the argument slot's low byte
    kotor_m15d_feats_determine_feat_006f2d30(self, nullptr, feat, &status);        // 006F3C26..2E
    Word strref = 0;
    switch (status) {                                                              // 006F3C33..42 (table 006F3CDC)
    case 0:                                                                        // 006F3C49 available
        if (self->remaining > 0) {                                                 // 006F3C49..51
            kotor_m15d_feats_add_chosen_feat_006f37e0(self, nullptr, feat);        // 006F3C53..56
            kotor_m15d_feats_build_buttons_006f30f0(self, nullptr);                // 006F3C5B..5D
            return;
        }
        strref = 0xA622;                                                           // 006F3C68
        break;
    case 1:                                                                        // 006F3C6F chosen
        kotor_m15d_feats_remove_chosen_feat_006f3990(self, nullptr, feat);         // 006F3C6F..72
        kotor_m15d_feats_build_buttons_006f30f0(self, nullptr);                    // 006F3C77..79
        return;
    case 2: strref = 0xA4C7; break;                                                // 006F3C84
    case 3: strref = 0xA4C6; break;                                                // 006F3C8B
    case 4: strref = 0xA4C8; break;                                                // 006F3C92
    default: return;                                                               // 006F3C3B (ja 006F3CD7)
    }
    show_message(self, strref);                                                    // 006F3C97..D1
}

// ======================================================================== 006F3D60 ctor (thiscall, RET 10h)
void* __fastcall kotor_m15d_feats_construct_006f3d60(void* memory, void*, void* manager, void* creature,
                                                      std::uint32_t level_up, std::uint32_t bonus_feats) noexcept(false) {
    FeatsPanel* const self = static_cast<FeatsPanel*>(memory);
    gui_load_panel_construct_0040b570(self, nullptr, static_cast<Object*>(manager));   // 006F3D7E..8B
    int built = 0;
    try {
        gui_lb::set_vptr(self, &gui_m15d_feats_methods_007598b0);                 // 006F3D99 (7598B0)
        for (int i = 0; i < 10; ++i) { gui_load_label_construct_0041acd0(label_at(self, i), nullptr); built = 1 + i; }   // 006F3D92..E2F
        for (int i = 0; i < 4; ++i) { gui_load_button_construct_0041c0f0(button_at(self, i), nullptr); built = 11 + i; } // 006F3E34..6F
        gui_load_listbox_construct_0041be40(&self->lb_feats, nullptr);  built = 15; // 006F3E74..7F
        gui_load_listbox_construct_0041be40(&self->lb_desc, nullptr);   built = 16; // 006F3E84..8F
        self->granted.count = 0;     self->granted.capacity = 0;     self->granted.data = 0;       // 006F3E94..A0
        self->granted_new.count = 0; self->granted_new.capacity = 0; self->granted_new.data = 0;   // 006F3EA6..B2
        self->available.count = 0;   self->available.capacity = 0;   self->available.data = 0;     // 006F3EB8..C4
        self->chosen.count = 0;      self->chosen.capacity = 0;      self->chosen.data = 0;        // 006F3ECA..D6
        built = 17;
        ::new (static_cast<void*>(&self->select_text)) StartupString();  built = 18;   // 006F3EDC..E7 (005B3190)
        ::new (static_cast<void*>(&self->remove_text)) StartupString();  built = 19;   // 006F3EEC..F7
        kotor_m15d_chart_construct_006cd510(&self->chart, nullptr);     built = 20;   // 006F3EFC..07
        self->manager = static_cast<Object*>(kotor_fastcall_005eda40(client_app(), nullptr));   // 006F3F0C..29 GetSWGuiManager
        self->creature = creature;                                                 // 006F3F20 / 006F3F32
        self->feat_flags = (self->feat_flags & 0xFFFFFFFCu) | ((bonus_feats & 1u) << 1) | (level_up & 1u);   // 006F3F23..4D
        self->current_feat = 0xFFFFu;                                              // 006F3F53
        {
            Ref layout;
            layout.construct_00406d80("FTCHRGEN");                                // 006F3F44..5C (00759984)
            gui_load_panel_start_layout_0040a680(self, nullptr, &layout);          // 006F3F61..68
        }
        init_control(self, &self->lbl_main_title, "MAIN_TITLE_LBL");              // 006F3F6D..9A (00759090)
        init_control(self, &self->lbl_sub_title, "SUB_TITLE_LBL");                // 006F3F9F..CF (00759890)
        init_control(self, &self->lbl_remaining_box, "REMAINING_BOX_LBL");        // 006F3FD4..4004 (0075987C)
        init_control(self, &self->lbl_std_remaining, "STD_REMAINING_SELECTIONS_LBL");   // 006F4009..39 (00759964)
        init_control(self, &self->lbl_std_selections, "STD_SELECTIONS_REMAINING_LBL");  // 006F403E..6E (00759944)
        init_control(self, &self->lbl_desc, "DESC_LBL");                          // 006F4073..A3 (00759838)
        init_control(self, &self->lb_desc, "LB_DESC");                            // 006F40A8..D8 (00755D6C)
        {
            self->lb_desc.u2c1 = 1;                                                // 006F40E6 (+199D; before the string ctor)
            String name("LB_FEATS");                                               // 006F40DD..ED (00759938)
            gui_load_panel_init_control_0040b930(self, nullptr, &self->lb_feats, &name, 1);   // 006F40F2..107
        }                                                                          // 006F410C..14
        init_control(self, &self->lbl_name, "LBL_NAME");                          // 006F4119..49 (00754028)
        init_control(self, &self->btn_accept, "BTN_ACCEPT");                      // 006F414E..7E (007548B8)
        init_control(self, &self->btn_recommended, "BTN_RECOMMENDED");            // 006F4183..B3 (00759928)
        init_control(self, &self->btn_back, "BTN_BACK");                          // 006F41B8..E8 (007548AC)
        init_control(self, &self->btn_select, "BTN_SELECT");                      // 006F41ED..21D (0075991C)
        gui_load_panel_stop_layout_0040b8f0(self, nullptr);                        // 006F4222..24
        StringSlot feats_word;                                                     // [esp+10h] GetSimpleString(A5F7h)
        simple_string(feats_word, 0xA5F7);                                         // 006F4229..39 (state 25h)
        {
            String space(" ");                                                     // 006F423E..4C (0073D5D8)
            StringSlot verb; simple_string(verb, 0x9637);                          // 006F4251..66
            StringSlot head; GuiConcat005e5d10(verb.get(), nullptr, head.raw(), &space); head.live = true;          // 006F426B..7C
            StringSlot full; GuiConcat005e5d10(head.get(), nullptr, full.raw(), feats_word.get()); full.live = true; // 006F4281..92
            self->select_text.Assign005e5c50(full.get());                          // 006F4297..A3 (+19F8)
        }                                                                          // 006F42A8..DB (E, D, C, B)
        {
            String space(" ");                                                     // 006F42E0..E9
            StringSlot verb; simple_string(verb, 0x9638);                          // 006F42EE..303
            StringSlot head; GuiConcat005e5d10(verb.get(), nullptr, head.raw(), &space); head.live = true;          // 006F4308..19
            StringSlot full; GuiConcat005e5d10(head.get(), nullptr, full.raw(), feats_word.get()); full.live = true; // 006F431E..2F
            self->remove_text.Assign005e5c50(full.get());                          // 006F4334..40 (+1A00)
        }                                                                          // 006F4345..78
        if (level_up != 0)                                                         // 006F437D..80
            gui_load_text_params_set_strref_00415e50(at(&self->lbl_main_title, kLabelText), nullptr, 0x42F);   // 006F4382..8D (+154)
        if (bonus_feats != 0)                                                      // 006F4392..95
            gui_load_text_params_set_strref_00415e50(at(&self->lbl_sub_title, kLabelText), nullptr, 0x524);    // 006F4397..A2 (+294)
        {
            Control* const proto = self->lb_desc.proto;                            // 006F43A7 (+19A8)
            vt<LabelInitView>(static_cast<Object*>(&self->desc_item))->initialize98(
                &self->desc_item, nullptr, word_of(&proto->extent), word_of(proto), 0x3F800000u);   // 006F43AD..C3 (proto+4, proto, 1.0f)
        }
        kotor_m15d_feats_initiate_feats_006f3460(self, nullptr);                   // 006F43C9..CB
        gui_load_control_add_event_0041ab20(&self->lb_feats, nullptr, 0x1F8, event_target(self),
            handler(&FeatsHandlers::OnFeatSelectionChanged_006f3420));             // 006F43D0..E1 (006F3420)
        gui_load_control_add_event_0041ab20(&self->lb_feats, nullptr, 0x1F9, event_target(self),
            handler(&FeatsHandlers::OnDoubleClick_006f3cf0));                      // 006F43E6..F7 (006F3CF0)
        vt<ListSelectView>(&self->lb_feats)->select_on_left_click(&self->lb_feats, nullptr, 1);   // 006F43FC..0A vslot +A4
        self->btn_accept.flags &= 0xFFFFFFFBu;                                     // 006F4410..27 (+D30)
        gui_load_control_add_event_0041ab20(&self->btn_accept, nullptr, 0x27, event_target(self),
            kotor_m1_shared::event_func(&kotor_m1_shared::PanelEventThunks::OnXButtonPressed_00624bc0));   // 006F441C..2A
        self->btn_recommended.flags &= 0xFFFFFFFBu;                                // 006F442F..46 (+10B8)
        gui_load_control_add_event_0041ab20(&self->btn_recommended, nullptr, 0x27, event_target(self),
            kotor_m15_l0::event_func(&kotor_m15_l0::PanelEventThunks15::OnYButtonPressed_00644720));      // 006F443B..49
        self->btn_back.flags &= 0xFFFFFFFBu;                                       // 006F444E..65 (+EF4)
        gui_load_control_add_event_0041ab20(&self->btn_back, nullptr, 0x27, event_target(self),
            kotor_m1_shared::event_func(&kotor_m1_shared::PanelEventThunks::OnBButtonPressed_00624bb0));   // 006F445A..68
        self->btn_select.flags &= 0xFFFFFFFBu;                                     // 006F446D..84 (+127C)
        gui_load_control_add_event_0041ab20(&self->btn_select, nullptr, 0x27, event_target(self),
            kotor_m1_shared::event_func(&kotor_m1_shared::PanelEventThunks::AcceptButtonCallback_00624ba0));   // 006F4479..87
        gui_load_panel_set_active_control_0040a630(self, nullptr, &self->lb_feats, 0);   // 006F448C..96 (0040A630)
    } catch (...) {
        unwind_feats(self, built);
        throw;
    }
    return self;                                                                   // 006F449B..AD (feats_word destroyed)
}

// ======================================================================== 006F44C0 OnAcceptButton (RET)
void __fastcall kotor_m15d_feats_on_accept_006f44c0(FeatsPanel* self, void*) noexcept(false) {
    if (self->remaining > 0 && self->available.count > 0) {                        // 006F44DB..EE
        show_message(self, 0xBC57);                                                // 006F44F0..2D
        return;
    }
    for (I32 i = 0; i < self->chosen.count; ++i)                                   // 006F4543..78: commit the chosen feats
        kotor_m15_array_u16_add_unique_005a7670(at(stats_of(self->creature), 0xB0), nullptr,
                                                u16_at(self->chosen, i));          // 006F4550..6A (stats+B0)
    close_to_parent(self);                                                         // 006F457A..A1
    gui_load_listbox_clear_items_00419a90(&self->lb_feats, nullptr);               // 006F459B..A4
    kotor_m15d_chart_clear_006cd240(&self->chart, nullptr);                        // 006F45A9..AF
    U8 normal = 0, bonus = 0;                                                      // [esp+0Bh] / [esp+0Ah]
    kotor_m15b_levelup_stats_calc_number_feats_00648fe0(stats_of(self->creature), nullptr, &normal, &bonus);   // 006F45B4..C7
    const Word flags = self->feat_flags;                                           // 006F45CC
    if ((flags & 2u) == 0 && bonus > 0) {                                          // 006F45D2..DC: open the bonus-feats panel
        void* const memory = c2da_directory_new_006fa7e6(0x1A1C);                  // 006F45DE..E3
        void* panel = nullptr;                                                     // 006F4618
        if (memory != nullptr) {
            try {
                panel = kotor_m15d_feats_construct_006f3d60(memory, nullptr, self->manager, self->creature,
                                                             self->feat_flags & 1u, 1);   // 006F45FB..611
            } catch (...) {
                live_free(memory);                                                 // EH state 0 (006FA390)
                throw;
            }
        }
        st<void*>(panel, 0x68, self->parent);                                      // 006F461A..21 (no null check in the original)
        kotor_fastcall_0040bc70(self->manager, nullptr, panel, 3, 1);              // 006F461D..30 AddPanel(p, 3, 1)
        return;
    }
    if ((flags & 1u) != 0) kotor_fastcall_006ee5d0(self->parent, nullptr);         // 006F4646..4D (L15-F trap)
    else kotor_m15c_custom_feat_creation_completed_006ef530(self->parent, nullptr);    // 006F4663 (ECX = [this+68h])
}

// ======================================================================== 006F4680 HandleInputEvent(event, value) (RET 8)
// Byte table 006F47C4 / jump table 006F47A4 for event-27h in 0..19h: 27h/2Dh pick, 28h/2Eh cancel, 29h accept,
// 2Ah recommend, 2Fh..32h/3Dh..40h chart navigation, 39h/3Ah scroll LB_DESC; then CSWGuiPanel::HandleInputEvent.
void __fastcall kotor_m15d_feats_handle_input_006f4680(FeatsPanel* self, void*, I32 event, I32 value) noexcept(false) {
    if (value != 0) {                                                              // 006F4685..8F
        switch (event) {
        case 0x27: case 0x2D:                                                      // 006F46EF
            manager_of(self)->PlayGuiSound(0);                                     // 006F46EF..F4 (0040A140)
            kotor_m15d_feats_on_feat_picked_006f3c20(self, nullptr,
                static_cast<Word>(kotor_m15d_chart_get_selected_006aba50(&self->chart, nullptr)));   // 006F46F9..707
            break;
        case 0x28: case 0x2E:                                                      // 006F46AF
            manager_of(self)->PlayGuiSound(0);
            kotor_m15d_feats_handle_cancel_006f2c70(self, nullptr);                // 006F46B9..BB
            break;
        case 0x29:                                                                 // 006F46CF
            manager_of(self)->PlayGuiSound(0);
            kotor_m15d_feats_on_accept_006f44c0(self, nullptr);                    // 006F46D9..DB
            break;
        case 0x2A:                                                                 // 006F471B
            manager_of(self)->PlayGuiSound(0);
            kotor_m15d_feats_on_recommend_006f3a80(self, nullptr);                 // 006F4725..27
            break;
        case 0x2F: case 0x30: case 0x31: case 0x32:
        case 0x3D: case 0x3E: case 0x3F: case 0x40:                                // 006F473B
            kotor_m15d_feats_on_enter_feat_006f2fb0(self, nullptr,
                static_cast<Word>(kotor_m15d_chart_handle_input_006cdd80(&self->chart, nullptr, event)));   // 006F473B..4A
            break;
        case 0x39:                                                                 // 006F4780: LB_DESC vslot +3C(31h, 1)
            vt<InputView>(&self->lb_desc)->handle_input_event(&self->lb_desc, nullptr, 0x31, 1);
            break;
        case 0x3A:                                                                 // 006F475E: LB_DESC vslot +3C(32h, 1)
            vt<InputView>(&self->lb_desc)->handle_input_event(&self->lb_desc, nullptr, 0x32, 1);
            break;
        default:
            break;
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, value);            // 006F46C0 / 006F4793 (00409E60)
}
}
