// lane-m15-c (L15-C, Milestone 1.5): char-gen flow panels.
//   CSWGuiQuickOrCustomPanel: ctor 006F09F0, dtors 006F0D50 / 006F0D30, HandleInputEvent 006F0E10, HandleCancelButton
//     006F07D0, SetDescription 006F0860, callbacks 006F0800 / 006F0830 / 006F09D0 / 006F09E0; table 00759710
//   CSWGuiQuickPanel: ctor 006F0390, dtors 006F0700 / 006F06E0, SetState 006EFC10, DoQuickStats 006EFFD0, OnPanelAdded
//     006F0260, HandleInputEvent 006F0280, CancelledByChild 006EFF10, Portrait/NameCreationCompleted 006EFF60 / 006EFF80,
//     GetCurrentName 006EFFA0, callbacks 006EFD60 / 006EFD80 / 006EFE10 / 006F0220 / 006F0310; table 00759668
//   CSWGuiCustomPanel: ctor 006EF730, dtors 006EFB40 / 006EFB20, SetParentPanel 006EEF60, SetState 006EEFD0,
//     HandleInputEvent 006EF610, CancelledByChild 006EF480, *CreationCompleted 006EF4C0 / 006EF4E0 / 006EF500 / 006EF520 /
//     006EF530, ClearAbilities 006EF540, CancelPanel 006EF570, callbacks 006EF120 / 006EF220 / 006EF240 / 006EF2D0 /
//     006EF360 / 006EF3F0 / 006EF5B0 / 006EF6A0; table 007595E0
// Original: swkotor.exe GOG K1 (image base 0x400000), static disassembly only; each behaviour cites its instruction
// addresses. Tables read from .rdata: identical to CSWGuiPanel 0073E010 except
//   00759710: +00 006F0D30, +3C 006F0E10        00759668: +00 006F06E0, +3C 006F0280, +48 006F0260
//   007595E0: +00 006EFB20, +3C 006EF610
// The Play buttons (006EFD60 / 006EF220) call CSWGuiMainCharGen::BeginPlayGame 006EB320, the lane-m15-f D83 trap.
// Conventions: D1 fastcall veneers, D21 typed tables, D24 vptr slot views, D47 member callbacks; EH unwind of the
// constructors is emitted with C++ try/catch in the original member order (FuncInfo states cited per ctor).
#include "88acda4ea129c9a0_m15c_common.h"

using namespace kotor_m15c;
using gui_load::Control;
using gui_load::Extent;

namespace {
QuickOrCustom* qoc_of(QuickOrCustomHandlers* h) { return reinterpret_cast<QuickOrCustom*>(h); }
Quick* quick_of(QuickHandlers* h) { return reinterpret_cast<Quick*>(h); }
Custom* custom_of(CustomHandlers* h) { return reinterpret_cast<Custom*>(h); }
kih::GuiObject* target(Panel* p) { return reinterpret_cast<kih::GuiObject*>(p); }
void add_event(Control* c, I32 id, Panel* p, gui_load::EventFunc f) { gui_load_control_add_event_0041ab20(c, nullptr, id, target(p), f); }
gui_load::EventFunc b_thunk() {
    return kotor_m1_shared::event_func(&kotor_m1_shared::PanelEventThunks::OnBButtonPressed_00624bb0);   // 00624BB0
}
using MessageSetText = void (__fastcall*)(void*, void*, Word);        // CSWGuiMessageBox slot +6C (strref)
using LabelInit98 = void (__fastcall*)(Object*, void*, Word, Word, Word);
using SetEnabled88 = void (__fastcall*)(Object*, void*, Word);
using SetActive40 = void (__fastcall*)(Object*, void*, Word);
void set_enabled(Object* o, Word on) { slot<SetEnabled88>(o, 0x88)(o, nullptr, on); }
void set_active(Object* o, Word on) { slot<SetActive40>(o, 0x40)(o, nullptr, on); }
Word& text_flags(void* params) { return *reinterpret_cast<Word*>(bytes(params) + 0x38); }   // CSWGuiTextParams +38

// Step-panel state display shared by Quick SetState 006EFC10 and Custom SetState 006EEFD0 (identical bodies; count 3/6).
template <unsigned N>
void show_state(Panel* self, I32 state, Button* back, LabelHilight* steps, Label* nums, Button* buttons) {
    if (state == 0) {                                                                     // 006EEFED / 006EFC2D
        set_enabled(back, 0);                                                             // [+88](0)
        gui_load_text_params_set_color_00414e10(button_text(back), nullptr, &kotor_m1_color_007a2408);   // 007A2408
    } else {
        set_enabled(back, 1);                                                             // [+88](1)
        gui_load_text_params_set_color_00414e10(button_text(back), nullptr, &borrowed_exit_color_007a23b4);   // 007A23B4
    }
    for (unsigned i = 0; i < N; ++i) {                                                    // 006EF030..F0 / 006EFC70..D30
        if (static_cast<I32>(i) == state) {
            set_active(&steps[i], 1);                                                     // hilight [+40](1)
            set_enabled(&nums[i], 1);                                                     // [+88](1)
            set_enabled(&buttons[i], 1);
            gui_load_text_params_set_color_00414e10(label_text(&nums[i]), nullptr, &borrowed_enter_color_007a23c0);
            gui_load_text_params_set_color_00414e10(button_text(&buttons[i]), nullptr, &borrowed_enter_color_007a23c0);
            text_flags(label_text(&nums[i])) |= 0x40u;                                    // +120
            text_flags(button_text(&buttons[i])) |= 0x40u;                                // +1A4
        } else {
            set_active(&steps[i], 0);                                                     // [+40](0)
            text_flags(label_text(&nums[i])) &= 0xFFFFFFBFu;
            text_flags(button_text(&buttons[i])) &= 0xFFFFFFBFu;
            set_enabled(&nums[i], 0);
            set_enabled(&buttons[i], 0);
        }
    }
    self->vtbl->set_active_control(self, nullptr, &buttons[state], 0);                   // [+08](&buttons[state], 0)
}
// Message box "cancel character?" (strref 0BD9Dh) with the panel's ConfirmCancel callback (006EF6AD..F2 / 006F031D..62).
void ask_cancel(Panel* self, gui_load::EventFunc confirm) {
    void* const box = message_box();                                                      // [[[7A39FC]+4] GetInGameGui]+98
    kotor_fastcall_00627130(box, nullptr, 1);                                             // SetAllowCancel(1)
    slot<MessageSetText>(box, 0x6C)(box, nullptr, 0xBD9Du);                              // [+6C](BD9Dh)
    gui_load_messagebox_set_callback_00624a40(box, nullptr, target(self), confirm, 0);    // SetCallback(this, f, 0)
    kotor_fastcall_0040bc70(self->manager, nullptr, box, 1, 1);                           // AddPanel(box, 1, 1)
}
bool message_box_accepted() { return (ld<unsigned char>(message_box(), 0x64) & 1u) != 0u; }   // test byte [box+64h],1
// Parent reset used by the Quick panel (006F023C..4D, 006F02B4..C5, 006F0367..78).
void quick_reset_parent(Quick* self) {
    if (MainCharGen* const parent = self->parent) {
        kotor_m15c_main_chargen_set_visible_panel_006ea760(parent, nullptr, 1);
        kotor_m15c_main_chargen_clear_abilities_006eb010(self->parent, nullptr);
    }
}
// Custom reset (006EF570 CancelPanel body, also inlined at 006EF5CC..EF and 006EF6F7..1A).
void custom_reset_parent(Custom* self) {
    if (self->parent) {
        kotor_m15c_main_chargen_set_visible_panel_006ea760(self->parent, nullptr, 1);
        if (self->parent) kotor_m15c_main_chargen_clear_abilities_006eb010(self->parent, nullptr);
        st<unsigned char>(stats_of(self->creature), 0xAC, 0x1E);                         // ability points 30
    }
}
// Model idle animation "pause2" on both model parts (FFh, FEh) - 006EF19A..F5 / 006EFE8A..E5.
void play_pause2(void* creature) {
    if (!gob_get_property_0x68_0063c070(creature, nullptr)) return;
    void* const body = creature_model(creature, 0xFF);
    slot<GobPlay>(body, 0x18)(body, nullptr, "pause2", 1.0f, 0, 0);                      // 0074F400
    void* const head = creature_model(creature, 0xFE);
    slot<GobPlay>(head, 0x18)(head, nullptr, "pause2", 1.0f, 0, 0);
}
// Element-wise construction of an eh_vector_constructor_iterator (006FB55F) range with its unwind.
template <class T, class C, class D> void construct_range(T* first, int n, C ctor, D dtor) {
    for (int i = 0; i < n; ++i) {
        try { ctor(&first[i]); }
        catch (...) { while (i-- > 0) dtor(&first[i]); throw; }
    }
}
template <class T, class D> void destroy_range(T* first, int n, D dtor) {         // eh_vector_destructor_iterator 006FA852
    for (int i = n; i-- > 0;) dtor(&first[i]);
}
void hilight_ctor(LabelHilight* p) { (void)gui_load_label_hilight_construct_0041b5c0(p, nullptr); }
void hilight_dtor(LabelHilight* p) { gui_m48_label_hilight_destruct_0041b610(p, nullptr); }
void label_ctor(Label* p) { (void)gui_load_label_construct_0041acd0(p, nullptr); }
void label_dtor(Label* p) { gui_load_label_destruct_00419d10(p, nullptr); }
void button_ctor(Button* p) { (void)gui_load_button_construct_0041c0f0(p, nullptr); }
void button_dtor(Button* p) { gui_load_button_destruct_00419da0(p, nullptr); }
}

extern "C" {
// ======================================================================= method tables
#define KOTOR_M15C_PANEL_TABLE(name, s00, s34, s3c, s48)                                                        \
    const KotorM15cPanelMethods name = {                                                                         \
        s00, &gui_load_panel_set_extent_0040a5a0, &gui_load_panel_set_active_control_0040a630,                   \
        &gui_load_slot_ret4_0060e760, &gui_load_slot_zero8_00409eb0,                                             \
        {&gui_load_slot_ret_004015a0, &gui_load_slot_ret_004015a0, &gui_load_slot_ret_004015a0,                 \
         &gui_load_slot_ret_004015a0, &gui_load_slot_ret_004015a0},                                              \
        &gui_load_slot_self_00641db0, &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0, s34,           \
        &gui_load_slot_ret4_0060e760, s3c, &gui_load_panel_hit_check_0040b690, &gui_load_panel_active_control_0040b630, \
        s48, &gui_load_panel_on_removed_0040c170,                                                                \
        {&gui_load_panel_button_a_0040b640, &gui_load_panel_button_b_0040b650, &gui_load_panel_button_x_0040b660, \
         &gui_load_panel_button_y_0040b670, &gui_load_panel_button_black_0040b680},                              \
        &gui_load_panel_full_screen_bg_0040a900, &gui_load_panel_reset_font_0040a9c0}
KOTOR_M15C_PANEL_TABLE(kotor_m15c_quick_or_custom_methods_00759710, &kotor_m15c_quick_or_custom_deleting_destruct_006f0d30,
                       &kotor_panel_draw_0040B760, &kotor_m15c_quick_or_custom_handle_input_006f0e10, &gui_load_panel_on_added_0040b870);
KOTOR_M15C_PANEL_TABLE(kotor_m15c_quick_methods_00759668, &kotor_m15c_quick_deleting_destruct_006f06e0,
                       &kotor_panel_draw_0040B760, &kotor_m15c_quick_handle_input_006f0280, &kotor_m15c_quick_on_panel_added_006f0260);
KOTOR_M15C_PANEL_TABLE(kotor_m15c_custom_methods_007595e0, &kotor_m15c_custom_deleting_destruct_006efb20,
                       &kotor_panel_draw_0040B760, &kotor_m15c_custom_handle_input_006ef610, &gui_load_panel_on_added_0040b870);

// ======================================================================= CSWGuiQuickOrCustomPanel
// 006F09F0 ctor(CSWGuiManager*, CSWCCreature*) (RET 8). EH states 0..7 = members (FuncInfo 0073908D).
QuickOrCustom* __fastcall kotor_m15c_quick_or_custom_construct_006f09f0(QuickOrCustom* self, void*, Object* manager,
                                                                       void* creature) noexcept(false) {
    (void)gui_load_panel_construct_0040b570(self, nullptr, manager);             // 006F0A1B
    int built = 0;
    try {
        self->vtbl = reinterpret_cast<const gui_load::ObjectVtbl*>(&kotor_m15c_quick_or_custom_methods_00759710);   // 006F0A2D
        button_ctor(&self->quick); built = 1;                                     // 006F0A33
        button_ctor(&self->custom); built = 2;                                    // 006F0A45
        label_ctor(&self->desc_item); built = 3;                                  // 006F0A55
        label_ctor(&self->decoration); built = 4;                                 // 006F0A65
        label_ctor(&self->ranalog); built = 5;                                    // 006F0A75
        label_ctor(&self->rbg); built = 6;                                        // 006F0A85
        (void)gui_load_listbox_construct_0041be40(&self->lb_desc, nullptr); built = 7;   // 006F0A95
        button_ctor(&self->back); built = 8;                                      // 006F0AA5
        self->creature = creature;                                                // 006F0AAD
        {
            gui_load::Ref layout;
            layout.construct_00406d80("QORCPNL");                                // 006F0AB0 (00759708)
            gui_load_panel_start_layout_0040a680(self, nullptr, &layout);         // 006F0ACA
        }
        init_control(self, &self->quick, "QUICK_CHAR_BTN");                       // 006F0ACF..AEC (007596F8)
        init_control(self, &self->custom, "CUST_CHAR_BTN");                       // 006F0AFF..B1C (007596E8)
        init_control(self, &self->lb_desc, "LB_DESC");                            // 006F0B2F..B52 (00755D6C)
        bytes(self)[0xBB5] = 1;                                                   // 006F0B6E (lb_desc+2C1)
        init_control(self, &self->decoration, "LBL_DECORATION");                  // 006F0B65..B8F (0075964C)
        init_control(self, &self->ranalog, "LBL_RANALOG");                        // 006F0BA2..BC5 (007596DC)
        init_control(self, &self->rbg, "LBL_RBG");                                // 006F0BD8..BFB (007596D4)
        init_control(self, &self->back, "BTN_BACK");                              // 006F0C0E..C31 (007548AC)
        gui_load_panel_stop_layout_0040b8f0(self, nullptr);                       // 006F0C46
        {
            Control* const proto = self->lb_desc.proto;                           // 006F0C4B ([esi+BC0h])
            slot<LabelInit98>(&self->desc_item, 0x98)(&self->desc_item, nullptr, word_of(&proto->extent), word_of(proto),
                                                      0x3F800000u);               // 006F0C51..63 (proto+4, proto, 1.0f)
        }
        add_event(&self->quick, 0, self, handler(&QuickOrCustomHandlers::OnEnterQuick_006f09d0));    // 006F0C69
        add_event(&self->custom, 0, self, handler(&QuickOrCustomHandlers::OnEnterCustom_006f09e0));  // 006F0C78
        add_event(&self->quick, 0x27, self, handler(&QuickOrCustomHandlers::OnSelectQuick_006f0800));    // 006F0C87
        add_event(&self->custom, 0x27, self, handler(&QuickOrCustomHandlers::OnSelectCustom_006f0830));  // 006F0C96
        add_event(&self->quick, 0x2D, self, handler(&QuickOrCustomHandlers::OnSelectQuick_006f0800));    // 006F0CA5
        add_event(&self->custom, 0x2D, self, handler(&QuickOrCustomHandlers::OnSelectCustom_006f0830));  // 006F0CB4
        gui_load_border_params_set_pulsing_00414d00(self->quick.hilight.params, nullptr, 1, 1, 0);       // 006F0CC3 (+160)
        gui_load_border_params_set_pulsing_00414d00(self->custom.hilight.params, nullptr, 1, 1, 0);      // 006F0CD4 (+324)
        self->back.flags &= 0xFFFFFFFBu;                                          // 006F0CE5..FC
        add_event(&self->back, 0x27, self, b_thunk());                            // 006F0CF1..FF (00624BB0)
        gui_load_panel_set_active_control_0040a630(self, nullptr, &self->quick, 0);   // 006F0D04..09
        self->flags |= 0x60u;                                                     // 006F0D0E..18
    } catch (...) {
        if (built >= 8) button_dtor(&self->back);
        if (built >= 7) gui_load_listbox_destruct_0041b060(&self->lb_desc, nullptr);
        if (built >= 6) label_dtor(&self->rbg);
        if (built >= 5) label_dtor(&self->ranalog);
        if (built >= 4) label_dtor(&self->decoration);
        if (built >= 3) label_dtor(&self->desc_item);
        if (built >= 2) button_dtor(&self->custom);
        if (built >= 1) button_dtor(&self->quick);
        gui_load_panel_destruct_0040cf70(self, nullptr);
        throw;
    }
    return self;                                                                  // 006F0D1C
}
// 006F0D50 dtor: members in reverse order, CSWGuiPanel::~CSWGuiPanel (no vptr store).
void __fastcall kotor_m15c_quick_or_custom_destruct_006f0d50(QuickOrCustom* self, void*) {
    button_dtor(&self->back);                                                     // 006F0D6D..7B
    gui_load_listbox_destruct_0041b060(&self->lb_desc, nullptr);                  // 006F0D80..8B
    label_dtor(&self->rbg);                                                       // 006F0D90
    label_dtor(&self->ranalog);                                                   // 006F0DA0
    label_dtor(&self->decoration);                                                // 006F0DB0
    label_dtor(&self->desc_item);                                                 // 006F0DC0
    button_dtor(&self->custom);                                                   // 006F0DD0
    button_dtor(&self->quick);                                                    // 006F0DE0
    gui_load_panel_destruct_0040cf70(self, nullptr);                              // 006F0DF7
}
// 006F0D30 scalar deleting destructor (RET 4).
void* __fastcall kotor_m15c_quick_or_custom_deleting_destruct_006f0d30(Panel* self, void*, Word flags) {
    kotor_m15c_quick_or_custom_destruct_006f0d50(static_cast<QuickOrCustom*>(self), nullptr);
    if (flags & 1u) live_free(self);                                              // 006F0D38..45
    return self;
}
// 006F07D0 HandleCancelButton (RET): parent RemoveSubPanels 006EA830; state 400h.
void __fastcall kotor_m15c_quick_or_custom_handle_cancel_006f07d0(QuickOrCustom* self, void*) noexcept(false) {
    if (self->parent) kotor_m15c_main_chargen_remove_sub_panels_006ea830(self->parent, nullptr);   // 006F07D3..DA
    mark_done(self);                                                              // 006F07DF..EC
}
// 006F0860 SetDescription(strref) (RET 4): TLK fetch into the description item, re-add it to LB_DESC, and the
// LBL_RANALOG flags bit1 = list needs scrolling.
void __fastcall kotor_m15c_quick_or_custom_set_description_006f0860(QuickOrCustom* self, void*, Word strref) noexcept(false) {
    struct StrRes { unsigned char text[8]; gui_load::Ref sound; unsigned char tail[4]; } res;   // [esp+1Ch] / [esp+24h]
    new (res.text) String();                                                      // 006F087C..80 (state 0)
    try {
        (void)c2da_default_ref_00405ed0(&res.sound, nullptr);                     // 006F0885..91 (state 1)
        static const unsigned char rdata_0073ecd0 = 0x00;                         // .rdata byte 0073ECD0 (image value 00)
        (void)tlk_fetch_0041e550(kotor_cell_tlk_007a3a08, nullptr, strref, &res, rdata_0073ecd0);   // 006F0896..B6
        set_text(label_text(&self->desc_item), str(res.text));                   // 006F08BB..C6 (+4DC)
        gui_load_listbox_clear_items_00419a90(&self->lb_desc, nullptr);           // 006F08CB..D3
        Extent extent = self->desc_item.extent;                                   // 006F08D8..F9 (+3F8)
        gui_load::Text* const text = &self->desc_item.text;                       // 006F0904 (+4C4)
        if (str(res.text)->BorrowedLength005e5790() > 0)                          // 006F08F1..0A
            extent.height = static_cast<I32>(slot<Word (__fastcall*)(gui_load::Text*, void*)>(text, 0x08)(text, nullptr));  // 006F090C..0E
        else
            extent.height = gui_load_text_font_height_00414ee0(text, nullptr);    // 006F0913
        Label* const item = &self->desc_item;                                     // 006F091D (+3F4)
        item->vtbl->set_extent(item, nullptr, &extent);                           // 006F0927..2C [+04]
        Control* items[1] = { item };                                             // 006F093E
        gui_m48_listbox_add_controls_0041c2c0(&self->lb_desc, nullptr, items, 1, 0, 0, 0);   // 006F092F..42
        gui_load_listbox_set_active_control_0041c160(&self->lb_desc, nullptr, item, 0);       // 006F0947..4C
        const gui_lb::ListBox& lb = self->lb_desc;
        const Word scroll = (static_cast<I32>(lb.padding) + lb.item_height > lb.inner.height &&   // 006F0951..69
                             lb.items.size > 0 && (lb.lb_flags & 0x100u) == 0u) ? 1u : 0u;      // 006F096B..85
        self->ranalog.flags ^= ((scroll * 2u) ^ self->ranalog.flags) & 2u;        // 006F0989..9F (+6B8)
    } catch (...) {
        str(res.text)->~StartupString();
        throw;
    }
    str(res.text)->~StartupString();                                              // 006F09AD
}
// 006F0E10 HandleInputEvent(event, pressed) (RET 8). Jump table 006F0EB8 / index bytes 006F0ECC (event - 28h):
// 28h,2Eh cancel (006F0E54); 2Dh sound (006F0E3B); 39h LB_DESC 31h (006F0E96); 3Ah LB_DESC 32h (006F0E74).
void __fastcall kotor_m15c_quick_or_custom_handle_input_006f0e10(Object* object, void*, I32 event, I32 pressed) noexcept(false) {
    QuickOrCustom* const self = static_cast<QuickOrCustom*>(static_cast<Panel*>(object));
    if (pressed != 0) {
        switch (event) {
        case 0x28: case 0x2E:
            manager_of(self)->PlayGuiSound(0);                                    // 006F0E54..59
            kotor_m15c_quick_or_custom_handle_cancel_006f07d0(self, nullptr);     // 006F0E5E..60
            break;
        case 0x2D:
            manager_of(self)->PlayGuiSound(0);                                    // 006F0E3B..40
            break;
        case 0x39: {
            gui_lb::ListBox* const lb = &self->lb_desc;
            gui_lb::vt<gui_lb::ControlCallVtbl>(static_cast<Control*>(lb))->handle_input_event(lb, nullptr, 0x31, 1);   // 006F0E96..A6
            break;
        }
        case 0x3A: {
            gui_lb::ListBox* const lb = &self->lb_desc;
            gui_lb::vt<gui_lb::ControlCallVtbl>(static_cast<Control*>(lb))->handle_input_event(lb, nullptr, 0x32, 1);   // 006F0E74..84
            break;
        }
        default: break;
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, pressed);          // 00409E60 (every path)
}
}

// ---- callbacks (thiscall, RET 4)
void __thiscall QuickOrCustomHandlers::OnSelectQuick_006f0800(kih::Control* control) {
    if (ld<Word>(control, 0x4C) == 0) return;                                    // 006F0800..09
    if (MainCharGen* const parent = qoc_of(this)->parent)                         // 006F080B..10
        kotor_m15c_main_chargen_set_visible_panel_006ea760(parent, nullptr, 2);   // 006F0812..1A (tail jump, arg 2)
}
void __thiscall QuickOrCustomHandlers::OnSelectCustom_006f0830(kih::Control* control) {
    if (ld<Word>(control, 0x4C) == 0) return;                                    // 006F0830..39
    if (MainCharGen* const parent = qoc_of(this)->parent)                         // 006F083B..40
        kotor_m15c_main_chargen_set_visible_panel_006ea760(parent, nullptr, 3);   // 006F0842..4A
}
void __thiscall QuickOrCustomHandlers::OnEnterQuick_006f09d0(kih::Control*) {
    kotor_m15c_quick_or_custom_set_description_006f0860(qoc_of(this), nullptr, 0xF1);   // 006F09D0..D8 (stack word replaced)
}
void __thiscall QuickOrCustomHandlers::OnEnterCustom_006f09e0(kih::Control*) {
    kotor_m15c_quick_or_custom_set_description_006f0860(qoc_of(this), nullptr, 0xF2);   // 006F09E0..E8
}

extern "C" {
// ======================================================================= CSWGuiQuickPanel
// 006F0390 ctor(CSWGuiManager*, CSWCCreature*) (RET 8). EH states 0..6 members, 7 = CExoString name (FuncInfo 00738F30).
Quick* __fastcall kotor_m15c_quick_construct_006f0390(Quick* self, void*, Object* manager, void* creature) noexcept(false) {
    (void)gui_load_panel_construct_0040b570(self, nullptr, manager);             // 006F03BB
    int built = 0;
    try {
        self->vtbl = reinterpret_cast<const gui_load::ObjectVtbl*>(&kotor_m15c_quick_methods_00759668);   // 006F03CD
        label_ctor(&self->decoration); built = 1;                                 // 006F03D3
        construct_range(self->steps, 3, hilight_ctor, hilight_dtor); built = 2;   // 006F03F5 (1B4h x3, 0041B5C0/0041B610)
        construct_range(self->nums, 3, label_ctor, label_dtor); built = 3;        // 006F0417 (140h x3)
        construct_range(self->buttons, 3, button_ctor, button_dtor); built = 4;   // 006F0439 (1C4h x3)
        button_ctor(&self->back); built = 5;                                      // 006F0449
        button_ctor(&self->cancel); built = 6;                                    // 006F0459
        String name;                                                              // 006F045E..67 [esp+28h]
        self->creature = creature;                                                // 006F046C..7D
        {
            gui_load::Ref layout;
            layout.construct_00406d80("QUICKPNL");                               // 006F046F (0075965C)
            gui_load_panel_start_layout_0040a680(self, nullptr, &layout);         // 006F048C
        }
        init_control(self, &self->decoration, "LBL_DECORATION");                  // 006F0491..AE
        for (int i = 1; i <= 3; ++i) {                                            // 006F04E2..58E
            kotor_reconstruction::StartupFormat005e5680(&name, "LBL_%d", i);     // 006F04E2..ED (00759554)
            gui_load_panel_init_control_0040b930(self, nullptr, &self->steps[i - 1], &name, 1);   // 006F04F5..505
            gui_load_border_params_set_pulsing_00414d00(self->steps[i - 1].hilight.params, nullptr, 1, 0, 1);   // 006F050A..12
            kotor_reconstruction::StartupFormat005e5680(&name, "BTN_STEPNAME%d", i);   // 006F0517..22 (00759544)
            gui_load_panel_init_control_0040b930(self, nullptr, &self->buttons[i - 1], &name, 1);   // 006F052E..38
            kotor_reconstruction::StartupFormat005e5680(&name, "LBL_NUM%d", i);  // 006F053D..48 (00759538)
            gui_load_panel_init_control_0040b930(self, nullptr, &self->nums[i - 1], &name, 1);   // 006F0554..5E
        }
        init_control(self, &self->back, "BTN_BACK");                              // 006F0594..B7
        init_control(self, &self->cancel, "BTN_CANCEL");                          // 006F05CA..ED (0074FD90)
        gui_load_panel_stop_layout_0040b8f0(self, nullptr);                       // 006F0602
        add_event(&self->buttons[0], 0x27, self, handler(&QuickHandlers::OnSelectPortrait_006efe10));   // 006F0607
        add_event(&self->buttons[1], 0x27, self, handler(&QuickHandlers::OnSelectName_006efd80));       // 006F061A
        add_event(&self->buttons[2], 0x27, self, handler(&QuickHandlers::OnSelectPlay_006efd60));       // 006F062D
        add_event(&self->buttons[0], 0x2D, self, handler(&QuickHandlers::OnSelectPortrait_006efe10));   // 006F0640
        add_event(&self->buttons[1], 0x2D, self, handler(&QuickHandlers::OnSelectName_006efd80));       // 006F0653
        add_event(&self->buttons[2], 0x2D, self, handler(&QuickHandlers::OnCancelPressed_006f0310));    // 006F0666 (sic)
        self->back.flags &= 0xFFFFFFFBu;                                          // 006F0679..89
        add_event(&self->back, 0x27, self, b_thunk());                            // 006F067C..8C (00624BB0)
        self->cancel.flags &= 0xFFFFFFFBu;                                        // 006F0691..9D
        add_event(&self->cancel, 0x27, self, handler(&QuickHandlers::OnCancelPressed_006f0310));        // 006F0694..A4
        kotor_m15c_quick_set_state_006efc10(self, nullptr, 0);                    // 006F06A9..AD
        self->flags |= 0x60u;                                                     // 006F06B2..BC
    } catch (...) {
        if (built >= 6) button_dtor(&self->cancel);
        if (built >= 5) button_dtor(&self->back);
        if (built >= 4) destroy_range(self->buttons, 3, button_dtor);
        if (built >= 3) destroy_range(self->nums, 3, label_dtor);
        if (built >= 2) destroy_range(self->steps, 3, hilight_dtor);
        if (built >= 1) label_dtor(&self->decoration);
        gui_load_panel_destruct_0040cf70(self, nullptr);
        throw;
    }
    return self;                                                                  // 006F06CE
}
// 006F0700 dtor (reverse member order; eh_vector_destructor_iterator 006FA852 for the arrays).
void __fastcall kotor_m15c_quick_destruct_006f0700(Quick* self, void*) {
    button_dtor(&self->cancel);                                                   // 006F071D..2B
    button_dtor(&self->back);                                                     // 006F0730..3B
    destroy_range(self->buttons, 3, button_dtor);                                 // 006F0740..58
    destroy_range(self->nums, 3, label_dtor);                                     // 006F075D..75
    destroy_range(self->steps, 3, hilight_dtor);                                  // 006F077A..92
    label_dtor(&self->decoration);                                                // 006F0797..9F
    gui_load_panel_destruct_0040cf70(self, nullptr);                              // 006F07A4..AE
}
// 006F06E0 scalar deleting destructor (RET 4).
void* __fastcall kotor_m15c_quick_deleting_destruct_006f06e0(Panel* self, void*, Word flags) {
    kotor_m15c_quick_destruct_006f0700(static_cast<Quick*>(self), nullptr);
    if (flags & 1u) live_free(self);                                              // 006F06E8..F5
    return self;
}
// 006EFC10 SetState(int state) (RET 4).
void __fastcall kotor_m15c_quick_set_state_006efc10(Quick* self, void*, I32 state) noexcept(false) {
    self->state = state;                                                          // 006EFC21
    show_state<3>(self, state, &self->back, self->steps, self->nums, self->buttons);   // 006EFC27..D52
}
// 006EFF10 CancelledByChild (RET): state 0 -> parent PortraitChanged; step button text colour + flag 40h.
void __fastcall kotor_m15c_quick_cancelled_by_child_006eff10(Quick* self, void*) noexcept(false) {
    if (self->state == 0) kotor_m15c_main_chargen_portrait_changed_006ea9f0(self->parent, nullptr);   // 006EFF13..20
    void* const params = button_text(&self->buttons[self->state]);                // 006EFF25..36 (+BF4)
    gui_load_text_params_set_color_00414e10(params, nullptr, &borrowed_enter_color_007a23c0);   // 006EFF31..3D
    text_flags(button_text(&self->buttons[self->state])) |= 0x40u;                // 006EFF42..55
}
// 006EFF60 PortraitCreationCompleted (RET).
void __fastcall kotor_m15c_quick_portrait_creation_completed_006eff60(Quick* self, void*) noexcept(false) {
    kotor_m15c_main_chargen_portrait_changed_006ea9f0(self->parent, nullptr);     // 006EFF63..66
    kotor_m15c_quick_set_state_006efc10(self, nullptr, 1);                        // 006EFF6B..6F
}
// 006EFF80 NameCreationCompleted (RET).
void __fastcall kotor_m15c_quick_name_creation_completed_006eff80(Quick* self, void*) noexcept(false) {
    kotor_m15c_main_chargen_name_changed_006eef70(self->parent, nullptr);         // 006EFF83..86
    kotor_m15c_quick_set_state_006efc10(self, nullptr, 2);                        // 006EFF8B..8F
}
// 006EFFA0 GetCurrentName() -> CExoString (hidden return; RET 4): copy of the parent's +22D4.
String* __fastcall kotor_m15c_quick_get_current_name_006effa0(Quick* self, void*, String* out) noexcept(false) {
    return new (out) String(*str(self->parent->name));                           // 006EFFA1..B9 (005E5B00)
}
// 006EFFD0 DoQuickStats (RET): class package abilities, granted + package feats (unique into stats feats +B0),
// skill points, package skills.
void __fastcall kotor_m15c_quick_do_quick_stats_006effd0(Quick* self, void*) noexcept(false) {
    void* const stats = stats_of(self->creature);                                 // 006EFFEE..F1
    const std::uint8_t index = static_cast<std::uint8_t>(ld<std::uint8_t>(stats, 0x32) - 1u);   // 006EFFF7..FC class count - 1
    const std::uint8_t class_id = kotor_m15b_creature_stats_get_class_00647730(stats, nullptr, index);   // 006F0000..05
    const unsigned char* const cls = ld<const unsigned char*>(kotor_cell_rules_007a3a28, 0xAC) + class_id * 0x1A0u;  // 006F0010..2A
    kotor_m15b_levelup_stats_set_str_base_0064a0d0(stats_of(self->creature), nullptr, cls[0x17B]);        // 006F0023..3B
    kotor_m15b_levelup_stats_set_dex_base_0064a100(stats_of(self->creature), nullptr, cls[0x17C], 1);     // 006F0040..5A
    kotor_m15b_levelup_stats_set_con_base_006496d0(stats_of(self->creature), nullptr, cls[0x17D]);        // 006F005F..77
    kotor_m15b_levelup_stats_set_wis_base_00649770(stats_of(self->creature), nullptr, cls[0x17F]);        // 006F007C..94
    kotor_m15b_levelup_stats_set_int_base_006478a0(stats_of(self->creature), nullptr, cls[0x17E]);        // 006F0099..B1
    kotor_m15b_levelup_stats_set_cha_base_006478d0(stats_of(self->creature), nullptr, cls[0x180]);        // 006F00B6..CE
    struct WordList { std::uint16_t* data; I32 size; I32 capacity; } added = {nullptr, 0, 0};             // 006F00D5..DD
    try {
        kotor_m15b_levelup_stats_add_granted_feats_00649950(stats_of(self->creature), nullptr, nullptr);  // 006F00E1..EF
        (void)kotor_m15b_levelup_stats_add_default_package_feats_0064a770(stats_of(self->creature), nullptr, class_id,
                                                                          &added, 1, 1);                   // 006F00F4..107
        for (I32 i = 0; i < added.size; ++i) {                                    // 006F010C..1AC
            WordList* const feats = reinterpret_cast<WordList*>(bytes(stats_of(self->creature)) + 0xB0);   // 006F0135
            const std::uint16_t feat = added.data[i];                             // 006F0129..3E
            I32 matches = 0;
            for (I32 k = 0; k < feats->size; ++k)                                 // 006F0143..68 (counts every match)
                if (feats->data[k] == feat) ++matches;
            if (feats->size > 0 && matches != 0) continue;                        // 006F016C..6E
            if (feats->size == feats->capacity)                                   // 006F0170..75
                kotor_m15_array_u16_allocate_004d2fa0(feats, nullptr, feats->capacity ? feats->capacity * 2 : 0x10);   // 006F0177..87
            feats->data[feats->size] = feat;                                      // 006F018C..96
            ++feats->size;                                                        // 006F019A
        }
        st<std::uint16_t>(stats_of(self->creature), 0xAE, 0);                    // 006F01B2..BB skill points
        kotor_m15b_levelup_stats_add_skill_points_per_level_00648a00(stats_of(self->creature), nullptr);   // 006F01C2..CB
        kotor_m15b_levelup_stats_add_default_package_skills_006491a0(stats_of(self->creature), nullptr, class_id, 1);  // 006F01D0..DC
        kotor_m15b_creature_stats_update_ability_modified_skill_ranks_00649380(stats_of(self->creature), nullptr);    // 006F01E1..EA
    } catch (...) {
        live_free(added.data);                                                    // EH state 0: CExoArrayList dtor
        throw;
    }
    live_free(added.data);                                                        // 006F01EF..FC (_free 006FA390)
}
// 006F0260 OnPanelAdded (RET): DoQuickStats, parent AbilitiesChanged, CSWGuiPanel::OnPanelAdded (tail jump).
void __fastcall kotor_m15c_quick_on_panel_added_006f0260(Panel* panel, void*) noexcept(false) {
    Quick* const self = static_cast<Quick*>(panel);
    kotor_m15c_quick_do_quick_stats_006effd0(self, nullptr);                      // 006F0263
    kotor_m15c_main_chargen_abilities_changed_006eac30(self->parent, nullptr);    // 006F0268..6B
    gui_load_panel_on_added_0040b870(self, nullptr);                              // 006F0270..73
}
// 006F0280 HandleInputEvent(event, pressed) (RET 8): 28h/2Eh step back (or leave at step 0); 2Dh sound.
void __fastcall kotor_m15c_quick_handle_input_006f0280(Object* object, void*, I32 event, I32 pressed) noexcept(false) {
    Quick* const self = static_cast<Quick*>(static_cast<Panel*>(object));
    if (pressed != 0) {
        if (event == 0x28 || event == 0x2E) {                                     // 006F0291..9E
            manager_of(self)->PlayGuiSound(0);                                    // 006F02A0..A5
            if (self->state == 0) {                                               // 006F02AA..B2
                quick_reset_parent(self);                                         // 006F02B4..C5
                kotor_m15c_quick_set_state_006efc10(self, nullptr, 0);            // 006F02CA..F1
            } else {
                --self->state;                                                    // 006F02E7..E8
                kotor_m15c_quick_set_state_006efc10(self, nullptr, self->state);  // 006F02EE..F1
            }
        } else if (event == 0x2D) {
            manager_of(self)->PlayGuiSound(0);                                    // 006F02CE..D3
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, pressed);          // 00409E60
}
}

// ---- Quick callbacks (thiscall, RET 4)
void __thiscall QuickHandlers::OnSelectPlay_006efd60(kih::Control* control) {
    Quick* const self = quick_of(this);
    if (self->state < 2) return;                                                  // 006EFD60..67
    if (ld<Word>(control, 0x4C) == 0) return;                                    // 006EFD69..72
    kotor_fastcall_006eb320(self->parent, nullptr);                               // 006EFD74..77 BeginPlayGame (L15-F trap)
}
void __thiscall QuickHandlers::OnSelectName_006efd80(kih::Control* control) {
    Quick* const self = quick_of(this);
    if (self->state < 1) return;                                                  // 006EFD98..A0
    if (ld<Word>(control, 0x4C) == 0) return;                                    // 006EFDA2..AD
    NameChargen* const panel = static_cast<NameChargen*>(new_panel(0x9C4, [self](void* m) {   // 006EFDAF..D7
        return static_cast<void*>(kotor_m15c_name_construct_006f9e70(static_cast<NameChargen*>(m), nullptr, self->manager,
                                                                      self->creature));
    }));
    kotor_m15c_name_set_parent_panel_006f9b80(panel, nullptr, self, 1);           // 006EFDD9..E6
    kotor_fastcall_0040bc70(self->manager, nullptr, panel, 3, 1);                 // 006EFDEB..F3
}
void __thiscall QuickHandlers::OnSelectPortrait_006efe10(kih::Control* control) {
    Quick* const self = quick_of(this);
    if (self->state < 0) return;                                                  // 006EFE28..31
    if (ld<Word>(control, 0x4C) == 0) return;                                    // 006EFE37..40
    Portrait* const panel = static_cast<Portrait*>(new_panel(0x1240, [self](void* m) {   // 006EFE46..72
        return static_cast<void*>(kotor_m15c_portrait_construct_006f9430(static_cast<Portrait*>(m), nullptr, self->manager,
                                                                          self->creature));
    }));
    kotor_m15c_portrait_set_parent_panel_006f8a00(panel, nullptr, self, 1);       // 006EFE78..85
    play_pause2(self->creature);                                                  // 006EFE8A..E5
    kotor_fastcall_0040bc70(self->manager, nullptr, panel, 3, 1);                 // 006EFEE8..F0
}
void __thiscall QuickHandlers::ConfirmCancel_006f0220(kih::Control*) {
    Quick* const self = quick_of(this);
    if (!message_box_accepted()) return;                                          // 006F0220..3A
    quick_reset_parent(self);                                                     // 006F023C..4D
    kotor_m15c_quick_set_state_006efc10(self, nullptr, 0);                        // 006F0252..56
}
void __thiscall QuickHandlers::OnCancelPressed_006f0310(kih::Control*) {
    Quick* const self = quick_of(this);
    if (self->state != 0) {                                                       // 006F0313..1B
        ask_cancel(self, handler(&QuickHandlers::ConfirmCancel_006f0220));        // 006F031D..62 (006F0220)
        return;
    }
    quick_reset_parent(self);                                                     // 006F0367..78
    kotor_m15c_quick_set_state_006efc10(self, nullptr, 0);                        // 006F037D..81
}

extern "C" {
// ======================================================================= CSWGuiCustomPanel
// 006EF730 ctor(CSWGuiManager*, CSWCCreature*) (RET 8). EH states 0..6 members, 7 = CExoString (FuncInfo 00738DC1).
Custom* __fastcall kotor_m15c_custom_construct_006ef730(Custom* self, void*, Object* manager, void* creature) noexcept(false) {
    (void)gui_load_panel_construct_0040b570(self, nullptr, manager);             // 006EF75B
    int built = 0;
    try {
        self->vtbl = reinterpret_cast<const gui_load::ObjectVtbl*>(&kotor_m15c_custom_methods_007595e0);   // 006EF77D
        construct_range(self->steps, 6, hilight_ctor, hilight_dtor); built = 1;   // 006EF783 (1B4h x6)
        construct_range(self->nums, 6, label_ctor, label_dtor); built = 2;        // 006EF7A5 (140h x6)
        construct_range(self->buttons, 6, button_ctor, button_dtor); built = 3;   // 006EF7C7 (1C4h x6)
        label_ctor(&self->bg); built = 4;                                         // 006EF7D9
        button_ctor(&self->back); built = 5;                                      // 006EF7EB
        button_ctor(&self->cancel); built = 6;                                    // 006EF7FB
        String name;                                                              // 006EF800..09 [esp+30h]
        self->manager = static_cast<Object*>(kotor_fastcall_005eda40(client_app(), nullptr));   // 006EF80E..21 GetSWGuiManager
        self->creature = creature;                                                // 006EF824..30
        {
            gui_load::Ref layout;
            layout.construct_00406d80("CUSTPNL");                                // 006EF827 (007595D4)
            gui_load_panel_start_layout_0040a680(self, nullptr, &layout);         // 006EF83F
        }
        init_control(self, &self->bg, "LBL_BG");                                  // 006EF844..61 (00753468)
        for (int i = 1; i <= 6; ++i) {                                            // 006EF8A0..959
            kotor_reconstruction::StartupFormat005e5680(&name, "LBL_%d", i);     // 006EF8A0..AB
            gui_load_panel_init_control_0040b930(self, nullptr, &self->steps[i - 1], &name, 1);   // 006EF8B7..C6
            gui_load_border_params_set_pulsing_00414d00(self->steps[i - 1].hilight.params, nullptr, 1, 0, 1);   // 006EF8CB..D5
            kotor_reconstruction::StartupFormat005e5680(&name, "BTN_STEPNAME%d", i);   // 006EF8DA..E5
            gui_load_panel_init_control_0040b930(self, nullptr, &self->buttons[i - 1], &name, 1);   // 006EF8F1..FB
            kotor_reconstruction::StartupFormat005e5680(&name, "LBL_NUM%d", i);  // 006EF900..0B
            gui_load_panel_init_control_0040b930(self, nullptr, &self->nums[i - 1], &name, 1);   // 006EF917..21
        }
        init_control(self, &self->back, "BTN_BACK");                              // 006EF95F..7C
        init_control(self, &self->cancel, "BTN_CANCEL");                          // 006EF98F..B2
        gui_load_panel_stop_layout_0040b8f0(self, nullptr);                       // 006EF9C7
        void (__thiscall CustomHandlers::* const picks[6])(kih::Control*) = {
            &CustomHandlers::OnSelectPortrait_006ef120, &CustomHandlers::OnSelectAbilities_006ef3f0,
            &CustomHandlers::OnSelectSkills_006ef360, &CustomHandlers::OnSelectFeats_006ef2d0,
            &CustomHandlers::OnSelectName_006ef240, &CustomHandlers::OnSelectPlay_006ef220,
        };
        for (int i = 0; i < 6; ++i) add_event(&self->buttons[i], 0x27, self, handler(picks[i]));   // 006EF9CC..A39
        for (int i = 0; i < 6; ++i) add_event(&self->buttons[i], 0x2D, self, handler(picks[i]));   // 006EFA3E..AB
        self->back.flags &= 0xFFFFFFFBu;                                          // 006EFAB0..C0
        add_event(&self->back, 0x27, self, b_thunk());                            // 006EFAB3..C3 (00624BB0)
        self->cancel.flags &= 0xFFFFFFFBu;                                        // 006EFAC8..D4
        add_event(&self->cancel, 0x27, self, handler(&CustomHandlers::OnCancelPressed_006ef6a0));   // 006EFACB..DB
        kotor_m15c_custom_set_state_006eefd0(self, nullptr, 0);                   // 006EFAE0..E4
        self->flags |= 0x60u;                                                     // 006EFAE9..F3
    } catch (...) {
        if (built >= 6) button_dtor(&self->cancel);
        if (built >= 5) button_dtor(&self->back);
        if (built >= 4) label_dtor(&self->bg);
        if (built >= 3) destroy_range(self->buttons, 6, button_dtor);
        if (built >= 2) destroy_range(self->nums, 6, label_dtor);
        if (built >= 1) destroy_range(self->steps, 6, hilight_dtor);
        gui_load_panel_destruct_0040cf70(self, nullptr);
        throw;
    }
    return self;                                                                  // 006EFB05
}
// 006EFB40 dtor.
void __fastcall kotor_m15c_custom_destruct_006efb40(Custom* self, void*) {
    button_dtor(&self->cancel);                                                   // 006EFB5D..6B
    button_dtor(&self->back);                                                     // 006EFB70..7B
    label_dtor(&self->bg);                                                        // 006EFB80..8B
    destroy_range(self->buttons, 6, button_dtor);                                 // 006EFB90..A8
    destroy_range(self->nums, 6, label_dtor);                                     // 006EFBAD..C5
    destroy_range(self->steps, 6, hilight_dtor);                                  // 006EFBCA..DF
    gui_load_panel_destruct_0040cf70(self, nullptr);                              // 006EFBE4..EE
}
// 006EFB20 scalar deleting destructor (RET 4).
void* __fastcall kotor_m15c_custom_deleting_destruct_006efb20(Panel* self, void*, Word flags) {
    kotor_m15c_custom_destruct_006efb40(static_cast<Custom*>(self), nullptr);
    if (flags & 1u) live_free(self);                                              // 006EFB28..35
    return self;
}
// 006EEF60 SetParentPanel(CSWGuiMainCharGen*) (RET 4).
void __fastcall kotor_m15c_custom_set_parent_panel_006eef60(Custom* self, void*, MainCharGen* parent) {
    self->parent = parent;                                                        // 006EEF64
}
// 006EEFD0 SetState(int state) (RET 4).
void __fastcall kotor_m15c_custom_set_state_006eefd0(Custom* self, void*, I32 state) noexcept(false) {
    self->state = state;                                                          // 006EEFE1
    show_state<6>(self, state, &self->back, self->steps, self->nums, self->buttons);   // 006EEFE7..F112
}
// 006EF480 CancelledByChild (RET).
void __fastcall kotor_m15c_custom_cancelled_by_child_006ef480(void* custom_panel, void*) noexcept(false) {
    Custom* const self = static_cast<Custom*>(custom_panel);
    gui_load_text_params_set_color_00414e10(button_text(&self->buttons[self->state]), nullptr,
                                            &borrowed_enter_color_007a23c0);     // 006EF483..9B (+1390)
    text_flags(button_text(&self->buttons[self->state])) |= 0x40u;                // 006EF4A0..B3
}
// 006EF4C0 PortraitCreationCompleted (RET): parent PortraitChanged, SetState(1).
void __fastcall kotor_m15c_custom_portrait_creation_completed_006ef4c0(void* custom_panel, void*) noexcept(false) {
    Custom* const self = static_cast<Custom*>(custom_panel);
    kotor_m15c_main_chargen_portrait_changed_006ea9f0(self->parent, nullptr);     // 006EF4C3..C6
    kotor_m15c_custom_set_state_006eefd0(self, nullptr, 1);                       // 006EF4CB..CF
}
// 006EF4E0 NameCreationCompleted (RET): parent NameChanged, SetState(5).
void __fastcall kotor_m15c_custom_name_creation_completed_006ef4e0(void* custom_panel, void*) noexcept(false) {
    Custom* const self = static_cast<Custom*>(custom_panel);
    kotor_m15c_main_chargen_name_changed_006eef70(self->parent, nullptr);         // 006EF4E3..E6
    kotor_m15c_custom_set_state_006eefd0(self, nullptr, 5);                       // 006EF4EB..EF
}
// 006EF500 AbilityCreationCompleted (RET): parent AbilitiesChanged, SetState(2).
void __fastcall kotor_m15c_custom_ability_creation_completed_006ef500(void* custom_panel, void*) noexcept(false) {
    Custom* const self = static_cast<Custom*>(custom_panel);
    kotor_m15c_main_chargen_abilities_changed_006eac30(self->parent, nullptr);    // 006EF503..06
    kotor_m15c_custom_set_state_006eefd0(self, nullptr, 2);                       // 006EF50B..0F
}
// 006EF520 SkillCreationCompleted (RET): SetState(3).
void __fastcall kotor_m15c_custom_skill_creation_completed_006ef520(void* custom_panel, void*) noexcept(false) {
    kotor_m15c_custom_set_state_006eefd0(static_cast<Custom*>(custom_panel), nullptr, 3);   // 006EF520..22
}
// 006EF530 FeatCreationCompleted (RET): SetState(4).
void __fastcall kotor_m15c_custom_feat_creation_completed_006ef530(void* custom_panel, void*) noexcept(false) {
    kotor_m15c_custom_set_state_006eefd0(static_cast<Custom*>(custom_panel), nullptr, 4);   // 006EF530..32
}
// 006EF540 ClearAbilities (RET): parent ClearAbilities (if any); stats ability points = 30.
void __fastcall kotor_m15c_custom_clear_abilities_006ef540(Custom* self, void*) noexcept(false) {
    if (self->parent) kotor_m15c_main_chargen_clear_abilities_006eb010(self->parent, nullptr);   // 006EF543..4A
    st<unsigned char>(stats_of(self->creature), 0xAC, 0x1E);                     // 006EF54F..58
}
// 006EF570 CancelPanel (RET).
void __fastcall kotor_m15c_custom_cancel_panel_006ef570(Custom* self, void*) noexcept(false) {
    custom_reset_parent(self);                                                    // 006EF573..96
    kotor_m15c_custom_set_state_006eefd0(self, nullptr, 0);                       // 006EF59D..A1
}
// 006EF610 HandleInputEvent(event, pressed) (RET 8).
void __fastcall kotor_m15c_custom_handle_input_006ef610(Object* object, void*, I32 event, I32 pressed) noexcept(false) {
    Custom* const self = static_cast<Custom*>(static_cast<Panel*>(object));
    if (pressed != 0) {
        if (event == 0x28 || event == 0x2E) {                                     // 006EF621..2E
            manager_of(self)->PlayGuiSound(0);                                    // 006EF630..35
            if (self->state == 0) {                                               // 006EF63A..44
                kotor_m15c_custom_cancel_panel_006ef570(self, nullptr);           // 006EF646
            } else {
                --self->state;                                                    // 006EF673..75
                kotor_m15c_custom_set_state_006eefd0(self, nullptr, self->state); // 006EF674..7B
                if (self->state == 1) kotor_m15c_custom_clear_abilities_006ef540(self, nullptr);   // 006EF680..8B
            }
        } else if (event == 0x2D) {
            manager_of(self)->PlayGuiSound(0);                                    // 006EF65A..5F
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, pressed);          // 00409E60
}
}

// ---- Custom callbacks (thiscall, RET 4)
void __thiscall CustomHandlers::OnSelectPortrait_006ef120(kih::Control* control) {
    Custom* const self = custom_of(this);
    if (self->state < 0) return;                                                  // 006EF138..41
    if (ld<Word>(control, 0x4C) == 0) return;                                    // 006EF147..50
    Portrait* const panel = static_cast<Portrait*>(new_panel(0x1240, [self](void* m) {   // 006EF156..82
        return static_cast<void*>(kotor_m15c_portrait_construct_006f9430(static_cast<Portrait*>(m), nullptr, self->manager,
                                                                          self->creature));
    }));
    kotor_m15c_portrait_set_parent_panel_006f8a00(panel, nullptr, self, 2);       // 006EF188..95
    play_pause2(self->creature);                                                  // 006EF19A..F5
    kotor_fastcall_0040bc70(self->manager, nullptr, panel, 3, 1);                 // 006EF1F8..200
}
void __thiscall CustomHandlers::OnSelectPlay_006ef220(kih::Control* control) {
    Custom* const self = custom_of(this);
    if (self->state < 5) return;                                                  // 006EF220..27
    if (ld<Word>(control, 0x4C) == 0) return;                                    // 006EF229..32
    kotor_fastcall_006eb320(self->parent, nullptr);                               // 006EF234..37 BeginPlayGame (L15-F trap)
}
void __thiscall CustomHandlers::OnSelectName_006ef240(kih::Control* control) {
    Custom* const self = custom_of(this);
    if (self->state < 4) return;                                                  // 006EF259..60
    if (ld<Word>(control, 0x4C) == 0) return;                                    // 006EF262..6D
    NameChargen* const panel = static_cast<NameChargen*>(new_panel(0x9C4, [self](void* m) {   // 006EF26F..97
        return static_cast<void*>(kotor_m15c_name_construct_006f9e70(static_cast<NameChargen*>(m), nullptr, self->manager,
                                                                      self->creature));
    }));
    kotor_m15c_name_set_parent_panel_006f9b80(panel, nullptr, self, 2);           // 006EF299..A6
    kotor_fastcall_0040bc70(self->manager, nullptr, panel, 3, 1);                 // 006EF2AB..B3
}
// Sub-panel openers: the parent is stored after the ctor returns ([eax+68h] = this, 006EF334 / 006EF3C2 / 006EF452);
// a null allocation stores through 68h (fault), as the original's xor eax,eax path.
namespace {
void attach_and_add(Custom* self, void* panel) {
    *reinterpret_cast<Custom**>(static_cast<std::uintptr_t>(reinterpret_cast<std::uintptr_t>(panel) + 0x68u)) = self;
    kotor_fastcall_0040bc70(self->manager, nullptr, panel, 3, 1);
}
}
void __thiscall CustomHandlers::OnSelectFeats_006ef2d0(kih::Control* control) {
    Custom* const self = custom_of(this);
    if (self->state < 3) return;                                                  // 006EF2E8..EF
    if (ld<Word>(control, 0x4C) == 0) return;                                    // 006EF2F1..FA
    void* const panel = new_panel(0x1A1C, [self](void* m) {                       // 006EF2FC..27
        return kotor_m15d_feats_construct_006f3d60(m, nullptr, self->manager, self->creature, 0, 0);
    });
    attach_and_add(self, panel);                                                  // 006EF330..43
}
void __thiscall CustomHandlers::OnSelectSkills_006ef360(kih::Control* control) {
    Custom* const self = custom_of(this);
    if (self->state < 2) return;                                                  // 006EF378..7F
    if (ld<Word>(control, 0x4C) == 0) return;                                    // 006EF381..8A
    void* const panel = new_panel(0x49D0, [self](void* m) {                       // 006EF38C..B5
        return kotor_m15d_skills_construct_006f51d0(m, nullptr, self->manager, self->creature, 0);
    });
    attach_and_add(self, panel);                                                  // 006EF3BE..D1
}
void __thiscall CustomHandlers::OnSelectAbilities_006ef3f0(kih::Control* control) {
    Custom* const self = custom_of(this);
    if (self->state < 1) return;                                                  // 006EF408..0F
    if (ld<Word>(control, 0x4C) == 0) return;                                    // 006EF411..1A
    void* const panel = new_panel(0x3DF4, [self](void* m) {                       // 006EF41C..45
        return kotor_m15d_abilities_construct_006f7600(m, nullptr, self->manager, self->creature, 0);
    });
    attach_and_add(self, panel);                                                  // 006EF44E..61
}
void __thiscall CustomHandlers::ConfirmCancel_006ef5b0(kih::Control*) {
    Custom* const self = custom_of(this);
    if (!message_box_accepted()) return;                                          // 006EF5B0..CA
    custom_reset_parent(self);                                                    // 006EF5CC..EF
    kotor_m15c_custom_set_state_006eefd0(self, nullptr, 0);                       // 006EF5F6..FA
}
void __thiscall CustomHandlers::OnCancelPressed_006ef6a0(kih::Control*) {
    Custom* const self = custom_of(this);
    if (self->state != 0) {                                                       // 006EF6A3..AB
        ask_cancel(self, handler(&CustomHandlers::ConfirmCancel_006ef5b0));       // 006EF6AD..F2 (006EF5B0)
        return;
    }
    custom_reset_parent(self);                                                    // 006EF6F7..1A
    kotor_m15c_custom_set_state_006eefd0(self, nullptr, 0);                       // 006EF721..25
}
