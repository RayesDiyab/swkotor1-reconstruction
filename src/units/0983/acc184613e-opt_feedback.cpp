// lane-m1-opt-gameplay: CSWGuiOptionsFeedback (0xC2C; operator new at 006E2E13 in CSWGuiOptionsMain::OnFeedbackOpt)
//   ctor 006E2A70, PopulateOptionsListbox 006E1300, SetUpOptions 006DE7C0, HandleInputEvent 006DE430,
//   SetDescription 006DE9B0, OnEnterSlot 006E0130, OnDefault 006E0100, the nine option handlers
//   006DE550 OnHide / 006DE5A0 OnTutorial / 006DE5F0 OnSubtitles / 006DE640 OnMiniMap / 006DE6A0 OnFloaty /
//   006DE6F0 OnSummary / 006DE740 OnHideInGameGUI / 006DE780 OnUseSmallFont / 006DEAA0 OnEnableToolTips,
//   dtors 006E0020 / 006E12E0, method table 007581E8,
//   CClientOptions::SetDefaultFeedbackOptions 0061D950, CClientOptions::SetUseSmallFonts 0061D910,
//   CGuiInGame::InitializeMap 0062B0A0 (+ D83 function trap CSWGuiMainInterface::InitializeMap 00687E40).
// Layout "optfeedback" (installed .gui, read-only through the resource manager). The handlers write
// CClientOptions+14h directly; the ini is rewritten by CSWGuiOptionsMain's Back (SaveOptions 005EDF70 -> run cwd).
// Original: swkotor.exe GOG K1, static disassembly only.
#include "98b60db287-opt_panel_table.h"
#include "9c420a0591-kotor_unreachable_trap.h"   // D83 shared fatal trap (lane-m48-traps)

using namespace kotor_m1_opt;

namespace menu_tables_required { void __fastcall UpdateAllFonts_0040b420(void* gui, void* unused_edx); }   // core47-gui-manager-leaves.obj (0040B420)
namespace kotor_input_required { void* __fastcall get_in_game_gui_005ed690(void* app, void* unused_edx); }  // r4-lowers-r4-frame-lowers.obj (005ED690)

namespace kotor_m1_opt {
// Layout established by the ctor 006E2A70 (member ctor calls) and dtor 006E0020 (reverse order).
struct Feedback : Panel {                      // +0000 CSWGuiPanel (0040B570 at 006E2A9B)
    ListBox lb_options;                        // +0064 (006E2AB3) "LB_OPTIONS": nine CSWGuiOptionsCheckbox items
    Label desc_item;                           // +0344 (006E2AC3) description item; Initialize from LB_DESC proto (006E2C5C)
    Label lbl_title;                           // +0484 (006E2AD3) "LBL_TITLE"
    ListBox lb_desc;                           // +05C4 (006E2AE3) "LB_DESC"; +885 = lb+2C1, +890 = lb proto
    Button btn_back;                           // +08A4 (006E2AF3) "BTN_BACK"
    Button btn_default;                        // +0A68 (006E2B05) "BTN_DEFAULT"
};
static_assert(offsetof(Feedback, lb_options) == 0x64 && offsetof(Feedback, desc_item) == 0x344 &&
              offsetof(Feedback, lbl_title) == 0x484 && offsetof(Feedback, lb_desc) == 0x5C4 &&
              offsetof(Feedback, btn_back) == 0x8A4 && offsetof(Feedback, btn_default) == 0xA68 &&
              sizeof(Feedback) == 0xC2C, "CSWGuiOptionsFeedback 0xC2C");

// CSWGuiControlEvent callbacks (thiscall members, RET 4; D47).
struct FeedbackHandlers : kih::GuiObject {
    void __thiscall OnEnterSlot_006e0130(kih::Control* control);
    void __thiscall OnDefault_006e0100(kih::Control* control);
    void __thiscall OnHide_006de550(kih::Control* control);
    void __thiscall OnTutorial_006de5a0(kih::Control* control);
    void __thiscall OnSubtitles_006de5f0(kih::Control* control);
    void __thiscall OnMiniMap_006de640(kih::Control* control);
    void __thiscall OnFloaty_006de6a0(kih::Control* control);
    void __thiscall OnSummary_006de6f0(kih::Control* control);
    void __thiscall OnHideInGameGUI_006de740(kih::Control* control);
    void __thiscall OnUseSmallFont_006de780(kih::Control* control);
    void __thiscall OnEnableToolTips_006deaa0(kih::Control* control);
};
}

extern "C" {
void* __fastcall kotor_m1_opt_feedback_deleting_destruct_006e12e0(Feedback* self, void*, Word flags);
void __fastcall kotor_m1_opt_feedback_destruct_006e0020(Feedback* self, void*);
void __fastcall kotor_m1_opt_feedback_handle_input_006de430(Object* self, void*, I32 event, I32 pressed);
void __fastcall kotor_m1_opt_feedback_setup_options_006de7c0(Feedback* self, void*);
void __fastcall kotor_m1_opt_feedback_populate_006e1300(Feedback* self, void*) noexcept(false);
void __fastcall kotor_m1_opt_feedback_set_description_006de9b0(Feedback* self, void*, Word strref) noexcept(false);
void __fastcall kotor_m1_opt_client_options_default_feedback_0061d950(void* options, void*);
void __fastcall kotor_m1_opt_client_options_set_use_small_fonts_0061d910(void* options, void*, Word value);
void __fastcall kotor_m1_opt_in_game_initialize_map_0062b0a0(void* in_game_gui, void*);
void __fastcall kotor_fastcall_00687e40(void* main_interface, void*);   // CSWGuiMainInterface::InitializeMap 00687E40 (main_interface_initialize_map.repaired.obj)
}

namespace {
Feedback* feedback_of(FeedbackHandlers* h) { return reinterpret_cast<Feedback*>(h); }
gui_load::EventFunc handler(void (__thiscall FeedbackHandlers::*f)(kih::Control*)) { return static_cast<gui_load::EventFunc>(f); }
// Constructed-member stages of 006E2A70 (EH states 0..6 of FuncInfo 00737041; states 7..0Bh are the CExoString
// temporaries, released by their C++ destructors).
enum Stage { kPanel = 0, kOptions, kDescItem, kTitle, kDesc, kBack, kDefault };
void unwind_feedback(Feedback* self, int built) {
    if (built >= kDefault) gui_load_button_destruct_00419da0(&self->btn_default, nullptr);
    if (built >= kBack) gui_load_button_destruct_00419da0(&self->btn_back, nullptr);
    if (built >= kDesc) gui_load_listbox_destruct_0041b060(&self->lb_desc, nullptr);
    if (built >= kTitle) gui_load_label_destruct_00419d10(&self->lbl_title, nullptr);
    if (built >= kDescItem) gui_load_label_destruct_00419d10(&self->desc_item, nullptr);
    if (built >= kOptions) gui_load_listbox_destruct_0041b060(&self->lb_options, nullptr);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
// Toggle bit i of CClientOptions+14h as the handlers do: two GetClientOptions calls, the first read negated
// (006DE565..86 pattern).
void toggle_options_bit(unsigned bit) {
    const Word flipped = ~(word_at(client_options(), 0x14) >> bit) & 1u;          // first GetClientOptions, shr/not/and
    void* const options = client_options();                                        // second GetClientOptions
    const Word flags = word_at(options, 0x14);
    set_word(options, 0x14, (((flipped << bit) ^ flags) & (1u << bit)) ^ flags);   // masked xor store
}
// CSWGuiListBox slot +08 SetActiveControl(control, flag) (RET 8), called at 006E2CC2.
struct ListSetActiveView { const void* s00; const void* s04; void (__fastcall* set_active_control)(ListBox*, void*, Control*, Word); };
// Frees a CExoArrayList<CSWGuiOptionsCheckbox*> data block on scope exit (EH state 0 of 006E1300 and the
// normal-path free at 006E15EC..FB).
struct ArrayDataFree {
    gui_load::Array* list;
    ~ArrayDataFree() { live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(list->data))); }
};
}

// ======================================================================== D21 method table 007581E8 (27 slots)
// .rdata 007581E8..00758253: CSWGuiPanel 0073E010 slots except +00 006E12E0 and +3C 006DE430.
extern "C" const PanelMethods27<Feedback> gui_m1_opt_feedback_methods_007581e8 =
    KOTOR_M1_OPT_PANEL_METHODS27(kotor_m1_opt_feedback_deleting_destruct_006e12e0, kotor_m1_opt_feedback_handle_input_006de430);

extern "C" {
// ======================================================================== 006E2A70 ctor(CSWGuiManager*) (thiscall, RET 4)
Panel* __fastcall kotor_m1_opt_feedback_construct_006e2a70(void* memory, void*, Object* manager) noexcept(false) {
    Feedback* const self = static_cast<Feedback*>(memory);
    gui_load_panel_construct_0040b570(self, nullptr, manager);                    // 006E2A8E..9B
    int built = kPanel;
    try {
        set_vptr(self, &gui_m1_opt_feedback_methods_007581e8);                     // 006E2AAD (7581E8)
        gui_load_listbox_construct_0041be40(&self->lb_options, nullptr); built = kOptions;    // 006E2AA0..B3
        gui_load_label_construct_0041acd0(&self->desc_item, nullptr);    built = kDescItem;   // 006E2AB8..C3
        gui_load_label_construct_0041acd0(&self->lbl_title, nullptr);    built = kTitle;      // 006E2AC8..D3
        gui_load_listbox_construct_0041be40(&self->lb_desc, nullptr);    built = kDesc;       // 006E2AD8..E3
        gui_load_button_construct_0041c0f0(&self->btn_back, nullptr);    built = kBack;       // 006E2AE8..F3
        gui_load_button_construct_0041c0f0(&self->btn_default, nullptr); built = kDefault;    // 006E2AF8..2B05
        {
            Ref layout;
            layout.construct_00406d80("optfeedback");                             // 006E2B0A..18 (00758664)
            gui_load_panel_start_layout_0040a680(self, nullptr, &layout);          // 006E2B1D..24
        }
        init_control(self, &self->lb_options, "LB_OPTIONS");                      // 006E2B29..54 (00756B1C)
        init_control(self, &self->lbl_title, "LBL_TITLE");                        // 006E2B59..8A (00751A20)
        init_control(self, &self->lb_desc, "LB_DESC");                            // 006E2B8F..C0 (00755D6C)
        reinterpret_cast<unsigned char*>(self)[0x885] = 1;                         // 006E2BCE (lb_desc+2C1), before the BTN_BACK string
        init_control(self, &self->btn_back, "BTN_BACK");                          // 006E2BC5..FD (007548AC)
        init_control(self, &self->btn_default, "BTN_DEFAULT");                    // 006E2C02..2D (007583B4)
        gui_load_panel_stop_layout_0040b8f0(self, nullptr);                        // 006E2C32..34
        kotor_m1_opt_feedback_populate_006e1300(self, nullptr);                    // 006E2C39..3B
        {
            Control* const proto = self->lb_desc.proto;                            // 006E2C40 ([esi+890h])
            const LabelInitView* lv = vt<LabelInitView>(static_cast<Object*>(&self->desc_item));   // 006E2C46
            lv->initialize98(&self->desc_item, nullptr,
                             word_of(&proto->extent), word_of(proto), 0x3F800000u);   // 006E2C4C..62 (proto+4, proto, 1.0f)
        }
        gui_load_control_add_event_0041ab20(&self->btn_default, nullptr, 0x27, event_target(self),
            handler(&FeedbackHandlers::OnDefault_006e0100));                       // 006E2C62..6C (006E0100)
        self->btn_default.flags &= 0xFFFFFFFBu;                                    // 006E2C71..81
        gui_load_control_add_event_0041ab20(&self->btn_default, nullptr, 0x2D, event_target(self),
            handler(&FeedbackHandlers::OnDefault_006e0100));                       // 006E2C74..84 (006E0100)
        self->btn_back.flags &= 0xFFFFFFFBu;                                       // 006E2C89..A0
        gui_load_control_add_event_0041ab20(&self->btn_back, nullptr, 0x27, event_target(self),
            kotor_m1_shared::event_func(&kotor_m1_shared::PanelEventThunks::OnBButtonPressed_00624bb0));   // 006E2C95..A3 (00624BB0)
        gui_load_panel_set_active_control_0040a630(self, nullptr, &self->lb_options, 0);   // 006E2CA8..AD (0040A630 direct)
        {
            ListBox* const lb = &self->lb_options;
            const ListSetActiveView* const v = vt<ListSetActiveView>(lb);          // 006E2CB2 (vptr read before the call)
            Control* const first = kotor_fastcall_0041a5a0(lb, nullptr, 0);        // 006E2CB4..BA GetControl(0)
            v->set_active_control(lb, nullptr, first, 0);                          // 006E2CBF..C2 vslot +08
        }
        kotor_m1_opt_feedback_setup_options_006de7c0(self, nullptr);               // 006E2CC5..C7
    } catch (...) {
        unwind_feedback(self, built);
        throw;
    }
    return self;                                                                   // 006E2CD1
}

// ======================================================================== 006E0020 dtor (thiscall, RET)
// Stores vptr 007581E8 (006E003D), destroys members in reverse order, then CSWGuiPanel::~CSWGuiPanel.
void __fastcall kotor_m1_opt_feedback_destruct_006e0020(Feedback* self, void*) {
    set_vptr(self, &gui_m1_opt_feedback_methods_007581e8);                         // 006E003D
    gui_load_button_destruct_00419da0(&self->btn_default, nullptr);               // 006E0043..51
    gui_load_button_destruct_00419da0(&self->btn_back, nullptr);                  // 006E0056..61
    gui_load_listbox_destruct_0041b060(&self->lb_desc, nullptr);                  // 006E0066..71
    gui_load_label_destruct_00419d10(&self->lbl_title, nullptr);                  // 006E0076..81
    gui_load_label_destruct_00419d10(&self->desc_item, nullptr);                  // 006E0086..91
    gui_load_listbox_destruct_0041b060(&self->lb_options, nullptr);               // 006E0096..9E
    gui_load_panel_destruct_0040cf70(self, nullptr);                               // 006E00A3..AD
}
// ======================================================================== 006E12E0 scalar deleting dtor (RET 4)
void* __fastcall kotor_m1_opt_feedback_deleting_destruct_006e12e0(Feedback* self, void*, Word flags) {
    kotor_m1_opt_feedback_destruct_006e0020(self, nullptr);                        // 006E12E3
    if (flags & 1u) live_free(self);                                               // 006E12E8..F5 (_free 006FA390)
    return self;                                                                   // 006E12F8
}

// ======================================================================== 006E1300 PopulateOptionsListbox() (thiscall, RET)
// Local CExoArrayList<CSWGuiControl*> {data, size, capacity} = {0, 0, 0} (006E1322..30), proto = lb_options+2CC
// (006E131C). GetClientOptions == 0 -> free(data) and return (006E1346..6F). Otherwise nine iterations
// (ebp 0..8, 006E1374..15BE): new(2B4h) CSWGuiOptionsCheckbox (ButtonToggle ctor 0041CA30 + vptr 00758258
// inlined at 006E1390..97; EH state 1 frees the block if the ctor throws), vslot +A0 Initialize(proto+4, proto),
// AddEvent(0, OnEnterSlot 006E0130), then per index (jump table 006E1618): SetStrRef(text params +16C, name strref),
// +58h = description strref, AddEvent(27h, handler), AddEvent(2Dh, handler); append (Allocate 005E03C0 with
// capacity ? 2*capacity : 10h when full). Then ClearItems, AddControls(&list, 1, 1, 0) (0041C1D0),
// lb_options+2BC &= ~40h, free(data).
void __fastcall kotor_m1_opt_feedback_populate_006e1300(Feedback* self, void*) noexcept(false) {
    Control* const proto = self->lb_options.proto;                                 // 006E131C..24 ([edi+330h])
    gui_load::Array list;
    list.data = 0; list.size = 0; list.capacity = 0;                               // 006E1322..30
    ArrayDataFree release{&list};                                                  // EH state 0; 006E134A..57 / 006E15E5..FB
    if (client_options() == nullptr) return;                                       // 006E1334..48 (005ED700)
    struct Option { Word name; Word description; void (__thiscall FeedbackHandlers::*on)(kih::Control*); };
    static const Option options[9] = {                                             // jump table 006E1618 (9 cases)
        { 0xA527u, 0xA52Eu, &FeedbackHandlers::OnHide_006de550 },                  // 006E13D6
        { 0xA528u, 0xA52Fu, &FeedbackHandlers::OnTutorial_006de5a0 },              // 006E1406
        { 0xA529u, 0xA530u, &FeedbackHandlers::OnSubtitles_006de5f0 },             // 006E1436
        { 0xA52Bu, 0xA531u, &FeedbackHandlers::OnMiniMap_006de640 },               // 006E1466
        { 0xA52Du, 0xA533u, &FeedbackHandlers::OnFloaty_006de6a0 },                // 006E1496
        { 0xA5D2u, 0xA5D3u, &FeedbackHandlers::OnSummary_006de6f0 },               // 006E14C6
        { 0xBFA5u, 0xBFA9u, &FeedbackHandlers::OnHideInGameGUI_006de740 },         // 006E14F6
        { 0xBFA6u, 0xBFAAu, &FeedbackHandlers::OnUseSmallFont_006de780 },          // 006E1523
        { 0xBFDBu, 0xBFDAu, &FeedbackHandlers::OnEnableToolTips_006deaa0 },        // 006E1550
    };
    for (int index = 0; index < 9; ++index) {                                      // 006E1372 / 006E15B6..BE
        void* const memory = c2da_directory_new_006fa7e6(0x2B4);                   // 006E1374..79 operator new
        Checkbox* box = nullptr;                                                   // 006E139F (xor esi,esi)
        if (memory) {                                                              // 006E1387..8E
            try {
                gui_load_button_toggle_construct_0041ca30(memory, nullptr);        // 006E1390..92
            } catch (...) {
                live_free(memory);                                                 // EH state 1 (free the new block)
                throw;
            }
            box = static_cast<Checkbox*>(memory);
            set_vptr(box, &gui_m1_opt_checkbox_methods_00758258);                  // 006E1397 (758258)
        }
        {
            struct InitA0View { const void* s00_9c[0x28]; void (__fastcall* init)(Checkbox*, void*, const Extent*, Control*); };
            static_assert(offsetof(InitA0View, init) == 0xA0, "+A0");
            vt<InitA0View>(box)->init(box, nullptr, &proto->extent, proto);         // 006E13A1..B2 vslot +A0(proto+4, proto)
        }
        gui_load_control_add_event_0041ab20(box, nullptr, 0, event_target(self),
            handler(&FeedbackHandlers::OnEnterSlot_006e0130));                     // 006E13B8..C1 (006E0130)
        const Option& o = options[index];                                          // 006E13C6..CF (ebp <= 8)
        gui_load_text_params_set_strref_00415e50(reinterpret_cast<unsigned char*>(box) + 0x16C, nullptr, o.name);   // SetStrRef(+16C)
        box->u58 = o.description;                                                  // [esi+58h]
        gui_load_control_add_event_0041ab20(box, nullptr, 0x27, event_target(self), handler(o.on));   // AddEvent(27h)
        gui_load_control_add_event_0041ab20(box, nullptr, 0x2D, event_target(self), handler(o.on));   // 006E157B..80 AddEvent(2Dh)
        if (list.size == list.capacity) {                                          // 006E1585..8D
            const Word capacity = static_cast<Word>(list.capacity);
            menu_list_growth::allocate_005e03c0(&list, nullptr, capacity != 0 ? capacity + capacity : 0x10u);   // 006E158F..A1
        }
        Control* const item = box;
        std::memcpy(reinterpret_cast<unsigned char*>(static_cast<std::uintptr_t>(list.data)) + 4 * list.size, &item, 4);   // 006E15A6..AE
        list.size = list.size + 1;                                                 // 006E15B1..BA
    }
    gui_load_listbox_clear_items_00419a90(&self->lb_options, nullptr);             // 006E15C4..C9
    kotor_fastcall_0041c1d0(&self->lb_options, nullptr, &list, 1, 1, 0);           // 006E15CE..DA AddControls(&list, 1, 1, 0)
    self->lb_options.lb_flags &= 0xFFFFFFBFu;                                      // 006E15DF..ED (+2BC)
}                                                                                  // 006E15E5..FB free(data)

// ======================================================================== 006DE7C0 SetUpOptions() (thiscall, RET)
// options null -> return. Items 0..8 of lb_options (GetControl 0041A5A0) <- options+14h bits 0,1,2,3,4,5,9,8,10
// (006DE7E3..997), each through vslot +5C (as toggle) and a masked xor into toggle +1C8h bit 0.
void __fastcall kotor_m1_opt_feedback_setup_options_006de7c0(Feedback* self, void*) {
    void* const options = client_options();                                        // 006DE7C0..CC (005ED700)
    if (!options) return;                                                          // 006DE7D3..D5
    static const unsigned char shifts[9] = { 0, 1, 2, 3, 4, 5, 9, 8, 0x0A };       // 006DE7EF / 81C / 84D / 87F / 8B1 / 8E1 / 915 / 947 / 979
    for (int i = 0; i < 9; ++i) {
        Control* const item = kotor_fastcall_0041a5a0(&self->lb_options, nullptr, i);   // GetControl(i)
        const Word bit = (word_at(options, 0x14) >> shifts[i]) & 1u;               // read before the slot call
        ButtonToggle* const t = as_toggle(item);                                   // vslot +5C
        t->toggle_flags = ((t->toggle_flags ^ bit) & 1u) ^ t->toggle_flags;        // masked xor into +1C8h
    }
}

// ======================================================================== 006DE430 HandleInputEvent(event, pressed)
// thiscall, RET 8. Jump table 006DE524 (5 entries) indexed by the byte table 006DE538 for event-28h in 0..12h:
// 00 04 04 04 04 01 00 04 04 04 04 04 04 04 04 04 04 02 03 -> 28h,2Eh: Back (006DE45F); 2Dh: toggle (006DE492);
// 39h: 006DE4DE; 3Ah: 006DE500; others: default (006DE513).
void __fastcall kotor_m1_opt_feedback_handle_input_006de430(Object* object, void*, I32 event, I32 pressed) {
    Feedback* const self = static_cast<Feedback*>(static_cast<Panel*>(object));
    if (pressed != 0) {                                                            // 006DE437..3F
        switch (event) {                                                           // 006DE445..58
        case 0x28: case 0x2E:                                                      // 006DE45F Back
            manager_of(self)->PlayGuiSound(0);                                     // 006DE45F..64 (0040A140)
            kotor_fastcall_0040be00(self->manager, nullptr);                       // 006DE469..6C PopModalPanel
            self->flags = (self->flags & 0xFFFFFCFFu) | 0x400u;                    // 006DE471..81 state 400h
            break;
        case 0x2D: {                                                               // 006DE492 toggle the active item
            Control* const active = self->active;                                  // 006DE492 ([esi+1Ch])
            if (active && as_toggle(active)) {                                     // 006DE495..A0
                manager_of(self)->PlayGuiSound(3);                                 // 006DE4A2..A7
                const Word selected = ~as_toggle(self->active)->toggle_flags & 1u; // 006DE4AC..C1
                kotor_m1_opt_toggle_set_selected_00685540(as_toggle(self->active), nullptr, selected);   // 006DE4BA..CA
            }
            break;
        }
        case 0x39: {                                                               // 006DE4DE: LB_DESC vslot +3C(31h, 1)
            ListBox* const lb = &self->lb_desc;
            vt<InputView>(static_cast<Object*>(lb))->handle_input_event(lb, nullptr, 0x31, 1);
            break;
        }
        case 0x3A: {                                                               // 006DE500: LB_DESC vslot +3C(32h, 1)
            ListBox* const lb = &self->lb_desc;
            vt<InputView>(static_cast<Object*>(lb))->handle_input_event(lb, nullptr, 0x32, 1);
            break;
        }
        default:
            break;
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, pressed);          // 006DE480..87 / 4CF..D3 / 4F1..F5 / 513..17
}

// ======================================================================== 006DE9B0 SetDescription(strref)
// thiscall, RET 4. EH state 0 (FuncInfo 00736788) covers the CExoString copy.
void __fastcall kotor_m1_opt_feedback_set_description_006de9b0(Feedback* self, void*, Word strref) noexcept(false) {
    unsigned char* const params = reinterpret_cast<unsigned char*>(self) + 0x42C;  // desc_item text params (+D0+18)
    gui_load_text_params_set_strref_00415e50(params, nullptr, strref);             // 006DE9C8..D9 (00415E50)
    String text(*reinterpret_cast<const String*>(params));                         // 006DE9DE..E3 (005E5B00)
    gui_load_listbox_clear_items_00419a90(&self->lb_desc, nullptr);                // 006DE9E8..F8
    Extent extent = self->desc_item.extent;                                        // 006DE9FD..1E (+348)
    gui_load::Text* const text_object = &self->desc_item.text;                     // 006DEA29 (+414)
    if (text.BorrowedLength005e5790() > 0)                                       // 006DEA22..2F (005E5790)
        extent.height = static_cast<I32>(vt<TextIdealView>(text_object)->ideal_height(text_object, nullptr));  // 006DEA31..33 vslot +08
    else
        extent.height = gui_load_text_font_height_00414ee0(text_object, nullptr);  // 006DEA38 (00414EE0)
    Label* const item = &self->desc_item;                                          // 006DEA41 (+344)
    item->vtbl->set_extent(item, nullptr, &extent);                                // 006DEA4B..50 vslot +04
    Control* items[1] = { item };                                                  // 006DEA62 (argument slot reused)
    gui_m48_listbox_add_controls_0041c2c0(&self->lb_desc, nullptr, items, 1, 0, 0, 0);   // 006DEA53..66
    gui_load_listbox_set_active_control_0041c160(&self->lb_desc, nullptr, item, 0);       // 006DEA6B..70
}                                                                                  // 006DEA75..81 (005E5C20)

// ======================================================================== 0061D950 CClientOptions::SetDefaultFeedbackOptions
// thiscall, RET. f = ([+14h] & FFFFFCBEh) | 4BEh; bit 8 re-stored from itself (0061D95D..73, net no change);
// [+14h] = f; byte [+80h] = (bit8 == 0) + 1; then CSWGuiManager::UpdateAllFonts when 007A39F4 != 0 (tail jump).
void __fastcall kotor_m1_opt_client_options_default_feedback_0061d950(void* options, void*) {
    const Word f = (word_at(options, 0x14) & 0xFFFFFCBEu) | 0x4BEu;                // 0061D950..58
    const Word small_fonts = (f >> 8) & 1u;                                              // 0061D95D..63
    const Word stored = (((small_fonts << 8) ^ f) & 0x100u) ^ f;                         // 0061D966..73
    set_word(options, 0x14, stored);                                               // 0061D97C
    set_byte(options, 0x80, static_cast<unsigned char>((small_fonts == 0 ? 1 : 0) + 1)); // 0061D975..7F (sete / inc)
    void* const gui = kotor_input_cells::gui_manager_007a39f4;                     // 0061D985
    if (gui) menu_tables_required::UpdateAllFonts_0040b420(gui, nullptr);          // 0061D98B..90 (jmp 0040B420)
}
// ======================================================================== 0061D910 CClientOptions::SetUseSmallFonts(int)
// thiscall, RET 4: [+14h] bit 8 = value bit 0; byte [+80h] = (value == 0) + 1; UpdateAllFonts when 007A39F4 != 0.
void __fastcall kotor_m1_opt_client_options_set_use_small_fonts_0061d910(void* options, void*, Word value) {
    const Word flags = word_at(options, 0x14);                                     // 0061D91A
    set_word(options, 0x14, (((value << 8) ^ flags) & 0x100u) ^ flags);            // 0061D914..2E
    set_byte(options, 0x80, static_cast<unsigned char>((value == 0 ? 1 : 0) + 1)); // 0061D927..31 (sete / inc)
    void* const gui = kotor_input_cells::gui_manager_007a39f4;                     // 0061D937
    if (gui) menu_tables_required::UpdateAllFonts_0040b420(gui, nullptr);          // 0061D93D..42
}
// ======================================================================== 0062B0A0 CGuiInGame::InitializeMap()
// thiscall, RET: main interface [+90h] != 0 -> JMP CSWGuiMainInterface::InitializeMap 00687E40.
void __fastcall kotor_m1_opt_in_game_initialize_map_0062b0a0(void* in_game_gui, void*) {
    void* const main_interface = ptr_at(in_game_gui, 0x90);                        // 0062B0A0
    if (main_interface) kotor_fastcall_00687e40(main_interface, nullptr);          // 0062B0A6..AA
}
}

// ======================================================================== event callbacks (thiscall, RET 4)
// ---- 006E0130 OnEnterSlot(control): MOV EAX,[ESP+4]; MOV EDX,[EAX+58h]; MOV [ESP+4],EDX; JMP 006DE9B0
void __thiscall FeedbackHandlers::OnEnterSlot_006e0130(kih::Control* control) {
    kotor_m1_opt_feedback_set_description_006de9b0(feedback_of(this), nullptr, word_at(control, 0x58));   // 006E0130..3B
}
// ---- 006E0100 OnDefault: [control+4Ch] != 0 -> SetDefaultFeedbackOptions(GetClientOptions()), SetUpOptions.
void __thiscall FeedbackHandlers::OnDefault_006e0100(kih::Control* control) {
    if (word_at(control, 0x4C) == 0) return;                                       // 006E0100..0C
    kotor_m1_opt_client_options_default_feedback_0061d950(client_options(), nullptr);   // 006E010E..1E
    kotor_m1_opt_feedback_setup_options_006de7c0(feedback_of(this), nullptr);      // 006E0123..25
}
// ---- 006DE550..006DE6F0: [control+4Ch] != 0 -> toggle CClientOptions+14h bit n (two GetClientOptions calls).
void __thiscall FeedbackHandlers::OnHide_006de550(kih::Control* control) {
    if (word_at(control, 0x4C) == 0) return;                                       // 006DE550..59
    toggle_options_bit(0);                                                         // 006DE55B..8D (bit 0)
}
void __thiscall FeedbackHandlers::OnTutorial_006de5a0(kih::Control* control) {
    if (word_at(control, 0x4C) == 0) return;                                       // 006DE5A0..A9
    toggle_options_bit(1);                                                         // 006DE5AB..DF (bit 1)
}
void __thiscall FeedbackHandlers::OnSubtitles_006de5f0(kih::Control* control) {
    if (word_at(control, 0x4C) == 0) return;                                       // 006DE5F0..F9
    toggle_options_bit(2);                                                         // 006DE5FB..34 (bit 2)
}
// ---- 006DE640 OnMiniMap: toggle bit 3, then CGuiInGame::InitializeMap(GetInGameGui()) (006DE687..97).
void __thiscall FeedbackHandlers::OnMiniMap_006de640(kih::Control* control) {
    if (word_at(control, 0x4C) == 0) return;                                       // 006DE640..49
    toggle_options_bit(3);                                                         // 006DE64B..84 (bit 3)
    void* const in_game_gui = kotor_input_required::get_in_game_gui_005ed690(client_app(), nullptr);   // 006DE687..90
    kotor_m1_opt_in_game_initialize_map_0062b0a0(in_game_gui, nullptr);            // 006DE695..97
}
void __thiscall FeedbackHandlers::OnFloaty_006de6a0(kih::Control* control) {
    if (word_at(control, 0x4C) == 0) return;                                       // 006DE6A0..A9
    toggle_options_bit(4);                                                         // 006DE6AB..E0 (bit 4)
}
void __thiscall FeedbackHandlers::OnSummary_006de6f0(kih::Control* control) {
    if (word_at(control, 0x4C) == 0) return;                                       // 006DE6F0..F9
    toggle_options_bit(5);                                                         // 006DE6FB..30 (bit 5)
}
// ---- 006DE740 OnHideInGameGUI: one GetClientOptions, null-checked; bit 9 ^= 1.
void __thiscall FeedbackHandlers::OnHideInGameGUI_006de740(kih::Control* control) {
    if (word_at(control, 0x4C) == 0) return;                                       // 006DE740..49
    void* const options = client_options();                                        // 006DE74B..54
    if (!options) return;                                                          // 006DE759..5B
    const Word flags = word_at(options, 0x14);                                     // 006DE75D
    set_word(options, 0x14, (((~(flags >> 9)) << 9 ^ flags) & 0x200u) ^ flags);   // 006DE760..74
}
// ---- 006DE780 OnUseSmallFont: SetUseSmallFonts(!bit 8) (tail jump 006DE7AE).
void __thiscall FeedbackHandlers::OnUseSmallFont_006de780(kih::Control* control) {
    if (word_at(control, 0x4C) == 0) return;                                       // 006DE780..89
    void* const options = client_options();                                        // 006DE78B..94
    if (!options) return;                                                          // 006DE799..9B
    const Word value = ~(word_at(options, 0x14) >> 8) & 1u;                        // 006DE79D..A5
    kotor_m1_opt_client_options_set_use_small_fonts_0061d910(options, nullptr, value);   // 006DE7A8..AE
}
// ---- 006DEAA0 OnEnableToolTips: one GetClientOptions, null-checked; bit 10 ^= 1.
void __thiscall FeedbackHandlers::OnEnableToolTips_006deaa0(kih::Control* control) {
    if (word_at(control, 0x4C) == 0) return;                                       // 006DEAA0..A9
    void* const options = client_options();                                        // 006DEAAB..B4
    if (!options) return;                                                          // 006DEAB9..BB
    const Word flags = word_at(options, 0x14);                                     // 006DEABD
    set_word(options, 0x14, (((~(flags >> 0x0A)) << 0x0A ^ flags) & 0x400u) ^ flags);   // 006DEAC0..D4
}
