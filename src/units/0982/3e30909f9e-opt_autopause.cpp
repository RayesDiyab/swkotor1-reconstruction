// lane-m1-opt-gameplay: CSWGuiInGameAutoPause (0x1984; operator new at 006DE2E3 in CSWGuiOptionsMain::OnAutopauseOpt)
//   ctor 006E76A0, SetupOptions 006E7280, SaveOptions 006E73D0, HandleInputEvent 006E74C0, OnDefault 006E7570,
//   SetDescription 006E75A0, OnEnterSlot 006E7690, dtors 006E7A90 / 006E7A70, method table 00758EE0,
//   CClientOptions::SetDefaultAutopauseOptions 0061D500.
// Layout "optautopause" (installed .gui, read-only through the resource manager). Back (28h/2Eh) stores the six
// check boxes into CClientOptions+14h bits 11..16 (SaveOptions) and pops the panel; the ini itself is rewritten
// later by CSWGuiOptionsMain's Back (SaveOptions 005EDF70 -> run-cwd swkotor.ini).
// Original: swkotor.exe GOG K1, static disassembly only.
#include "98b60db287-opt_panel_table.h"

using namespace kotor_m1_opt;

namespace kotor_m1_opt {
// Layout established by the ctor 006E76A0 (member ctor calls) and dtor 006E7A90 (reverse order).
struct AutoPause : Panel {                     // +0000 CSWGuiPanel (0040B570 at 006E76CB)
    Checkbox boxes[6];                         // +0064 eh_vector_constructor_iterator(+64, 2B4h, 6, 006E00D0, 006E00F0) 006E76F7
                                               //   [0] +064 CB_ENDROUND     [1] +318 CB_ENEMYSIGHTED [2] +5CC CB_MINESIGHTED
                                               //   [3] +880 CB_PARTYKILLED  [4] +B34 CB_ACTIONMENU   [5] +DE8 CB_TRIGGERS
    Label desc_item;                           // +109C (006E7707) description item; Initialize from LB_DETAILS proto (006E7A04)
    Label lbl_title;                           // +11DC (006E7717) "LBL_TITLE"
    Button btn_back;                           // +131C (006E7729) "BTN_BACK"
    Button btn_default;                        // +14E0 (006E7739) "BTN_DEFAULT"
    ListBox lb_details;                        // +16A4 (006E7749) "LB_DETAILS"; +1965 = lb+2C1, +1970 = lb proto
};
static_assert(offsetof(AutoPause, boxes) == 0x64 && offsetof(AutoPause, desc_item) == 0x109C &&
              offsetof(AutoPause, lbl_title) == 0x11DC && offsetof(AutoPause, btn_back) == 0x131C &&
              offsetof(AutoPause, btn_default) == 0x14E0 && offsetof(AutoPause, lb_details) == 0x16A4 &&
              sizeof(AutoPause) == 0x1984, "CSWGuiInGameAutoPause 0x1984");

// CSWGuiControlEvent callbacks registered by the ctor (thiscall members, RET 4; D47).
struct AutoPauseHandlers : kih::GuiObject {
    void __thiscall OnEnterSlot_006e7690(kih::Control* control);    // event 0 on boxes[0..5]
    void __thiscall OnDefault_006e7570(kih::Control* control);      // event 27h on BTN_DEFAULT
};
}

extern "C" {
void* __fastcall kotor_m1_opt_autopause_deleting_destruct_006e7a70(AutoPause* self, void*, Word flags);
void __fastcall kotor_m1_opt_autopause_destruct_006e7a90(AutoPause* self, void*);
void __fastcall kotor_m1_opt_autopause_handle_input_006e74c0(Object* self, void*, I32 event, I32 pressed);
void __fastcall kotor_m1_opt_autopause_setup_options_006e7280(AutoPause* self, void*);
void __fastcall kotor_m1_opt_autopause_save_options_006e73d0(AutoPause* self, void*);
void __fastcall kotor_m1_opt_autopause_set_description_006e75a0(AutoPause* self, void*, Word strref) noexcept(false);
void __fastcall kotor_m1_opt_client_options_default_autopause_0061d500(void* options, void*);
}

namespace {
AutoPause* autopause_of(AutoPauseHandlers* h) { return reinterpret_cast<AutoPause*>(h); }
gui_load::EventFunc handler(void (__thiscall AutoPauseHandlers::*f)(kih::Control*)) { return static_cast<gui_load::EventFunc>(f); }
// Constructed-member stages of 006E76A0 (EH states 0..6 of FuncInfo 00737AD7; states 7..10h are the CExoString
// temporaries, released by their C++ destructors).
enum Stage { kPanel = 0, kBoxes, kDescItem, kTitle, kBack, kDefault, kListBox };
void unwind_autopause(AutoPause* self, int built) {
    if (built >= kListBox) gui_load_listbox_destruct_0041b060(&self->lb_details, nullptr);
    if (built >= kDefault) gui_load_button_destruct_00419da0(&self->btn_default, nullptr);
    if (built >= kBack) gui_load_button_destruct_00419da0(&self->btn_back, nullptr);
    if (built >= kTitle) gui_load_label_destruct_00419d10(&self->lbl_title, nullptr);
    if (built >= kDescItem) gui_load_label_destruct_00419d10(&self->desc_item, nullptr);
    if (built >= kBoxes)
        for (int i = 6; i-- > 0;) kotor_m1_opt_checkbox_destruct_006e00f0(&self->boxes[i], nullptr);   // eh_vector_destructor_iterator
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
}

// ======================================================================== D21 method table 00758EE0 (27 slots)
// .rdata 00758EE0..00758F4B: CSWGuiPanel 0073E010 slots except +00 006E7A70 and +3C 006E74C0.
extern "C" const PanelMethods27<AutoPause> gui_m1_opt_autopause_methods_00758ee0 =
    KOTOR_M1_OPT_PANEL_METHODS27(kotor_m1_opt_autopause_deleting_destruct_006e7a70, kotor_m1_opt_autopause_handle_input_006e74c0);

extern "C" {
// ======================================================================== 006E76A0 ctor(CSWGuiManager*) (thiscall, RET 4)
Panel* __fastcall kotor_m1_opt_autopause_construct_006e76a0(void* memory, void*, Object* manager) noexcept(false) {
    AutoPause* const self = static_cast<AutoPause*>(memory);
    gui_load_panel_construct_0040b570(self, nullptr, manager);                    // 006E76BE..CB
    int built = kPanel;
    try {
        set_vptr(self, &gui_m1_opt_autopause_methods_00758ee0);                    // 006E76F1 (758EE0)
        // 006E76D0..F7 eh_vector_constructor_iterator(+64, 2B4h, 6, 006E00D0, 006E00F0): element k failing destroys
        // elements k-1..0 before propagating.
        for (int i = 0; i < 6; ++i) {
            try { kotor_m1_opt_checkbox_construct_006e00d0(&self->boxes[i], nullptr); }
            catch (...) { while (i-- > 0) kotor_m1_opt_checkbox_destruct_006e00f0(&self->boxes[i], nullptr); throw; }
        }
        built = kBoxes;
        gui_load_label_construct_0041acd0(&self->desc_item, nullptr);    built = kDescItem;   // 006E76FC..07
        gui_load_label_construct_0041acd0(&self->lbl_title, nullptr);    built = kTitle;      // 006E770C..17
        gui_load_button_construct_0041c0f0(&self->btn_back, nullptr);    built = kBack;       // 006E771C..29
        gui_load_button_construct_0041c0f0(&self->btn_default, nullptr); built = kDefault;    // 006E772E..39
        gui_load_listbox_construct_0041be40(&self->lb_details, nullptr); built = kListBox;    // 006E773E..49
        {
            Ref layout;
            layout.construct_00406d80("optautopause");                            // 006E774E..5B (00758ED0)
            gui_load_panel_start_layout_0040a680(self, nullptr, &layout);          // 006E7760..67
        }
        init_control(self, &self->boxes[0], "CB_ENDROUND");                       // 006E776C..99 (00758EC4)
        init_control(self, &self->boxes[1], "CB_ENEMYSIGHTED");                   // 006E779E..CE (00758EB4)
        init_control(self, &self->boxes[2], "CB_MINESIGHTED");                    // 006E77D3..03 (00758EA4)
        init_control(self, &self->boxes[3], "CB_PARTYKILLED");                    // 006E7808..38 (00758E94)
        init_control(self, &self->boxes[4], "CB_ACTIONMENU");                     // 006E783D..6D (00758E84)
        init_control(self, &self->boxes[5], "CB_TRIGGERS");                       // 006E7872..A2 (00758E78)
        init_control(self, &self->lbl_title, "LBL_TITLE");                        // 006E78A7..D7 (00751A20)
        init_control(self, &self->lb_details, "LB_DETAILS");                      // 006E78DC..0C (00758E6C)
        init_control(self, &self->btn_default, "BTN_DEFAULT");                    // 006E7911..41 (007583B4)
        {
            // 006E7946..4F: lb_details+2C1 = 1 stored between the BTN_BACK string's argument push and its ctor.
            reinterpret_cast<unsigned char*>(self)[0x1965] = 1;                    // 006E794F
            init_control(self, &self->btn_back, "BTN_BACK");                      // 006E7946..77 (007548AC)
        }
        for (int i = 0; i < 6; ++i)                                                // 006E797C..A3 (six boxes, event 0)
            gui_load_control_add_event_0041ab20(&self->boxes[i], nullptr, 0, event_target(self),
                handler(&AutoPauseHandlers::OnEnterSlot_006e7690));                // 006E7983..8F (006E7690)
        // Description strrefs in CSWGuiControl+58h (read by OnEnterSlot 006E7694).
        self->boxes[0].u58 = 0xA5CDu;                                              // 006E79A7 (+0BC)
        self->boxes[1].u58 = 0xA5CEu;                                              // 006E79B1 (+370)
        self->boxes[2].u58 = 0xBFDDu;                                              // 006E79BB (+624)
        self->boxes[3].u58 = 0xA5CFu;                                              // 006E79C5 (+8D8)
        self->boxes[4].u58 = 0xBD20u;                                              // 006E79CF (+B8C)
        self->boxes[5].u58 = 0xBC56u;                                              // 006E79D9 (+E40)
        gui_load_panel_stop_layout_0040b8f0(self, nullptr);                        // 006E79A5 / 79E3
        {
            Control* const proto = self->lb_details.proto;                         // 006E79E8 ([esi+1970h])
            const LabelInitView* lv = vt<LabelInitView>(static_cast<Object*>(&self->desc_item));   // 006E79EE
            lv->initialize98(&self->desc_item, nullptr,
                             word_of(&proto->extent), word_of(proto), 0x3F800000u);   // 006E79F4..7A04 (proto+4, proto, 1.0f)
        }
        self->btn_default.flags &= 0xFFFFFFFBu;                                    // 006E7A0A..25
        gui_load_control_add_event_0041ab20(&self->btn_default, nullptr, 0x27, event_target(self),
            handler(&AutoPauseHandlers::OnDefault_006e7570));                      // 006E7A16..28 (006E7570)
        kotor_m1_opt_autopause_setup_options_006e7280(self, nullptr);              // 006E7A2D..2F
        self->btn_back.flags &= 0xFFFFFFFBu;                                       // 006E7A34..43
        gui_load_control_add_event_0041ab20(&self->btn_back, nullptr, 0x27, event_target(self),
            kotor_m1_shared::event_func(&kotor_m1_shared::PanelEventThunks::OnBButtonPressed_00624bb0));   // 006E7A37..46 (00624BB0)
        gui_load_panel_set_active_control_0040a630(self, nullptr, &self->boxes[0], 0);   // 006E7A4B..53 (0040A630 direct)
    } catch (...) {
        unwind_autopause(self, built);
        throw;
    }
    return self;                                                                   // 006E7A5D
}

// ======================================================================== 006E7A90 dtor (thiscall, RET)
// No vptr store in this body; members in reverse order, then CSWGuiPanel::~CSWGuiPanel.
void __fastcall kotor_m1_opt_autopause_destruct_006e7a90(AutoPause* self, void*) {
    gui_load_listbox_destruct_0041b060(&self->lb_details, nullptr);               // 006E7AAD..BB
    gui_load_button_destruct_00419da0(&self->btn_default, nullptr);               // 006E7AC0..CB
    gui_load_button_destruct_00419da0(&self->btn_back, nullptr);                  // 006E7AD0..DB
    gui_load_label_destruct_00419d10(&self->lbl_title, nullptr);                  // 006E7AE0..EB
    gui_load_label_destruct_00419d10(&self->desc_item, nullptr);                  // 006E7AF0..FB
    for (int i = 6; i-- > 0;)                                                      // 006E7B00..15 eh_vector_destructor_iterator
        kotor_m1_opt_checkbox_destruct_006e00f0(&self->boxes[i], nullptr);        //   (+64, 2B4h, 6, 006E00F0): reverse order
    gui_load_panel_destruct_0040cf70(self, nullptr);                               // 006E7B1A..24
}
// ======================================================================== 006E7A70 scalar deleting dtor (RET 4)
void* __fastcall kotor_m1_opt_autopause_deleting_destruct_006e7a70(AutoPause* self, void*, Word flags) {
    kotor_m1_opt_autopause_destruct_006e7a90(self, nullptr);                       // 006E7A73
    if (flags & 1u) live_free(self);                                               // 006E7A78..85 (_free 006FA390)
    return self;                                                                   // 006E7A88
}

// ======================================================================== 006E7280 SetupOptions() (thiscall, RET)
// options = GetClientOptions(); null -> return (006E7293..95). boxes[i] (i = 0..4) <- bit 11+i of options+14h,
// boxes[5] <- bit 0 of byte options+16h (= bit 16); each through vslot +5C (as toggle) and a masked xor into
// toggle +1C8h bit 0.
void __fastcall kotor_m1_opt_autopause_setup_options_006e7280(AutoPause* self, void*) {
    void* const options = client_options();                                        // 006E7280..8C (005ED700)
    if (!options) return;                                                          // 006E7293..95
    for (int i = 0; i < 5; ++i) {                                                  // 006E729B..7385
        const Word bit = (word_at(options, 0x14) >> (0x0B + i)) & 1u;              // 006E729F..A8 (shr 0Bh..0Fh)
        ButtonToggle* const t = as_toggle(&self->boxes[i]);                        // 006E72A2..AB vslot +5C
        t->toggle_flags = ((t->toggle_flags ^ bit) & 1u) ^ t->toggle_flags;        // 006E72AE..C1
    }
    const Word bit = byte_at(options, 0x16) & 1u;                                  // 006E7391..9F
    ButtonToggle* const t = as_toggle(&self->boxes[5]);                            // 006E738B..A1 vslot +5C
    t->toggle_flags = ((t->toggle_flags ^ bit) & 1u) ^ t->toggle_flags;            // 006E73A4..B7
}
// ======================================================================== 006E73D0 SaveOptions() (thiscall, RET)
// options+14h bit (11+i) <- boxes[i] toggle bit 0, i = 0..5 (shl 0Bh..10h; masked xor 006E73F4..74BA).
void __fastcall kotor_m1_opt_autopause_save_options_006e73d0(AutoPause* self, void*) {
    void* const options = client_options();                                        // 006E73D0..DC (005ED700)
    if (!options) return;                                                          // 006E73E3..E5
    for (int i = 0; i < 6; ++i) {
        const Word selected = as_toggle(&self->boxes[i])->toggle_flags;            // 006E73EB..F4 vslot +5C, [+1C8h]
        const Word mask = 1u << (0x0B + i);
        const Word flags = word_at(options, 0x14);                                 // 006E73FA
        set_word(options, 0x14, (((selected << (0x0B + i)) ^ flags) & mask) ^ flags);   // 006E73FD..740F
    }
}
// ======================================================================== 006E74C0 HandleInputEvent(event, pressed)
// thiscall, RET 8. pressed: 28h/2Eh (006E74D3..DE) -> PlayGuiSound(0), SaveOptions, PopModalPanel, state 400h;
// 2Dh (006E74DB) -> active control as toggle: PlayGuiSound(3), SetSelected(!selected). All paths end in
// CSWGuiPanel::HandleInputEvent 00409E60.
void __fastcall kotor_m1_opt_autopause_handle_input_006e74c0(Object* object, void*, I32 event, I32 pressed) {
    AutoPause* const self = static_cast<AutoPause*>(static_cast<Panel*>(object));
    if (pressed != 0) {                                                            // 006E74CB..CF
        if (event == 0x28 || event == 0x2E) {                                      // 006E74D1..DE
            manager_of(self)->PlayGuiSound(0);                                     // 006E74E0..E5 (0040A140)
            kotor_m1_opt_autopause_save_options_006e73d0(self, nullptr);           // 006E74EA..EC
            kotor_fastcall_0040be00(self->manager, nullptr);                       // 006E74F1..F4 PopModalPanel
            self->flags = (self->flags & 0xFFFFFCFFu) | 0x400u;                    // 006E74F9..7506
        } else if (event == 0x2D) {                                                // 006E74D8..DB
            Control* const active = self->active;                                  // 006E7518 ([esi+1Ch])
            if (active && as_toggle(active)) {                                     // 006E751B..26
                manager_of(self)->PlayGuiSound(3);                                 // 006E7528..2D
                const Word selected = ~as_toggle(self->active)->toggle_flags & 1u; // 006E7532..47
                kotor_m1_opt_toggle_set_selected_00685540(as_toggle(self->active), nullptr, selected);   // 006E7540..50
            }
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, pressed);          // 006E7509..0D / 7555..59 (00409E60)
}
// ======================================================================== 006E75A0 SetDescription(strref)
// thiscall, RET 4. EH state 0 (FuncInfo 00737A08) covers the CExoString copy.
void __fastcall kotor_m1_opt_autopause_set_description_006e75a0(AutoPause* self, void*, Word strref) noexcept(false) {
    unsigned char* const params = reinterpret_cast<unsigned char*>(self) + 0x1184; // desc_item text params (+D0+18)
    gui_load_text_params_set_strref_00415e50(params, nullptr, strref);             // 006E75B8..C9 (00415E50)
    String text(*reinterpret_cast<const String*>(params));                         // 006E75CE..D3 (005E5B00)
    gui_load_listbox_clear_items_00419a90(&self->lb_details, nullptr);             // 006E75D8..E8
    Extent extent = self->desc_item.extent;                                        // 006E75ED..760E (+10A0)
    gui_load::Text* const text_object = &self->desc_item.text;                     // 006E7619 (+116C)
    if (text.BorrowedLength005e5790() > 0)                                       // 006E7612..1F (005E5790)
        extent.height = static_cast<I32>(vt<TextIdealView>(text_object)->ideal_height(text_object, nullptr));  // 006E7621..23 vslot +08
    else
        extent.height = gui_load_text_font_height_00414ee0(text_object, nullptr);  // 006E7628 (00414EE0)
    Label* const item = &self->desc_item;                                          // 006E7631 (+109C)
    item->vtbl->set_extent(item, nullptr, &extent);                                // 006E763B..40 vslot +04
    Control* items[1] = { item };                                                  // 006E7652 (argument slot reused)
    gui_m48_listbox_add_controls_0041c2c0(&self->lb_details, nullptr, items, 1, 0, 0, 0);   // 006E7643..56
    gui_load_listbox_set_active_control_0041c160(&self->lb_details, nullptr, item, 0);       // 006E765B..60
}                                                                                  // 006E7665..71 (005E5C20)

// ======================================================================== 0061D500 CClientOptions::SetDefaultAutopauseOptions
// thiscall, RET: [+14h] = ([+14h] & FFFF77FFh) | 17000h.
void __fastcall kotor_m1_opt_client_options_default_autopause_0061d500(void* options, void*) {
    set_word(options, 0x14, (word_at(options, 0x14) & 0xFFFF77FFu) | 0x17000u);   // 0061D500..0D
}
}

// ======================================================================== event callbacks
// ---- 006E7690 OnEnterSlot(control): MOV EAX,[ESP+4]; MOV EDX,[EAX+58h]; MOV [ESP+4],EDX; JMP 006E75A0
void __thiscall AutoPauseHandlers::OnEnterSlot_006e7690(kih::Control* control) {
    kotor_m1_opt_autopause_set_description_006e75a0(autopause_of(this), nullptr, word_at(control, 0x58));   // 006E7690..9B
}
// ---- 006E7570 OnDefault(control) (RET 4; the control argument is not read).
void __thiscall AutoPauseHandlers::OnDefault_006e7570(kih::Control*) {
    kotor_m1_opt_client_options_default_autopause_0061d500(client_options(), nullptr);   // 006E7570..82 (005ED700, 0061D500)
    kotor_m1_opt_autopause_setup_options_006e7280(autopause_of(this), nullptr);    // 006E7587..89
}
