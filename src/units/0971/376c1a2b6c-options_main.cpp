// lane-m1-options / L1-options: CSWGuiMainMenu::OnOptionsPicked 0067B2F0 (post-guard body; lifts TRAP #74),
// CSWGuiOptionsMain ctor 006E3E80, HandleInputEvent 006DFF10 (Back: PopModalPanel + SaveOptions -> run-dir
// swkotor.ini), OnEnterSlot 006E0010, SetDescription 006DE340, dtors 006E42C0 / 006E42A0, method table 00758838,
// and the five OptionsMain sub-button handlers: Gameplay / Feedback / AutoPause bodies authored (lane-m1-opt-gameplay
// sub-panels); the others keep their original guard + D83 branch trap until their lane lands.
// Original: swkotor.exe GOG K1 (sha256 9c10e045...91435), static disassembly only; each behaviour cites its
// instruction addresses. Conventions: D1 fastcall veneers for thiscall entries, D21 typed method table,
// D24 object views of method tables, D47 member callbacks, D30-FINAL (no float arithmetic here; EH unwind
// emitted with C++ try/catch because every member ctor is noexcept(false)).
#include "d2a3b1dda5-options_main.h"
#include "9c420a0591-kotor_unreachable_trap.h"   // D83 shared fatal trap (lane-m48-traps)
#include <cstring>
#include "67605decb8-options_sound.h"      // lane-m1-opt-sound-gfx: CSWGuiOptionsSound 006E3550
#include "f7af356e7f-options_graphics.h"   // lane-m1-opt-sound-gfx: CSWGuiOptionsGraphics 006E2E70

using namespace kotor_m1_options;
using gui_load::Object;
using gui_load::Panel;
using gui_load::Control;
using gui_load::Label;
using gui_load::Button;
using gui_load::Extent;
using gui_load::String;
using gui_load::Ref;

// ---- external providers already linked by M49 (functional.map 20261007T145743537700Z-3f436fae)
namespace client_movies { int __fastcall SaveOptionsApp_005edf70(void* app, void* unused_edx); }  // lane-config-movie-list.obj
extern "C" {
void* __fastcall kotor_fastcall_0040be00(void* gui, void*);                                      // r4-lowers PopModalPanel 0040BE00
void __fastcall gui_m48_listbox_add_controls_0041c2c0(gui_lb::ListBox*, void*, Control** controls, I32 count,
    Word suppress, Word restore, Word variable) noexcept(false);                                 // m48b-gui-controls 0041C2C0
void __fastcall gui_load_border_params_set_pulsing_00414d00(void* params, void*, Word a, Word b, Word c);  // gui-load 00414D00
// lane-m1-opt-gameplay sub-panel constructors (thiscall ctor(CSWGuiManager*) RET 4 -> D1 fastcall veneers)
Panel* __fastcall kotor_m1_opt_gameplay_construct_006e69a0(void* self, void*, Object* manager) noexcept(false);   // 006E69A0
Panel* __fastcall kotor_m1_opt_feedback_construct_006e2a70(void* self, void*, Object* manager) noexcept(false);   // 006E2A70
Panel* __fastcall kotor_m1_opt_autopause_construct_006e76a0(void* self, void*, Object* manager) noexcept(false);  // 006E76A0
}

namespace {
Word word_at(const void* p, unsigned off) { Word v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
void* ptr_at(const void* p, unsigned off) { void* v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
Word word_of(const void* p) { return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p)); }
OptionsMain* options_of(OptionsMainHandlers* h) { return reinterpret_cast<OptionsMain*>(h); }
kih::GuiObject* event_target(OptionsMain* self) { return reinterpret_cast<kih::GuiObject*>(self); }
gui_load::EventFunc handler(void (__thiscall OptionsMainHandlers::*f)(kih::Control*)) { return static_cast<gui_load::EventFunc>(f); }
kih::Manager* manager_of(const Panel* panel) { return reinterpret_cast<kih::Manager*>(panel->manager); }   // [this+18h]

// CSWGuiLabel method table 0073E5B8 slot +98 (00418CD0 Initialize; 3 stack words, RET 0Ch), as typed in gui_vtables.
using LabelInit98 = void (__fastcall*)(Object*, void*, Word, Word, Word);
struct LabelInitView { const void* s00_94[0x26]; LabelInit98 initialize98; };
static_assert(offsetof(LabelInitView, initialize98) == 0x98, "label +98");
// CSWGuiText method table 0073E3A4 slot +08 (00414EB0 GetIdealHeight).
using TextIdeal08 = Word (__fastcall*)(gui_load::Text*, void*);
struct TextIdealView { const void* s00; const void* s04; TextIdeal08 ideal_height; };
static_assert(offsetof(TextIdealView, ideal_height) == 0x08, "text +08");

// Constructed-member stages of 006E3E80 (EH states 0..5 of FuncInfo at 007374E9; states 6..0Dh are the
// CExoString temporaries, released by their C++ destructors).
enum Stage { kPanel = 0, kButtons, kDescItem, kTitle, kBack, kListBox };
void unwind_options(OptionsMain* self, int built) {
    if (built >= kListBox) gui_load_listbox_destruct_0041b060(&self->lb_desc, nullptr);
    if (built >= kBack) gui_load_button_destruct_00419da0(&self->btn_back, nullptr);
    if (built >= kTitle) gui_load_label_destruct_00419d10(&self->lbl_title, nullptr);
    if (built >= kDescItem) gui_load_label_destruct_00419d10(&self->desc_item, nullptr);
    if (built >= kButtons)
        for (int i = 6; i-- > 0;) gui_load_button_destruct_00419da0(&self->buttons[i], nullptr);   // eh_vector_destructor_iterator
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
void init_control(OptionsMain* self, Control* control, const char* tag) {         // String temp + InitControl(c, &s, 1)
    String name(tag);                                                              // 005E5A90
    gui_load_panel_init_control_0040b930(self, nullptr, control, &name, 1);        // 0040B930
}                                                                                  // 005E5C20
}

// ======================================================================== D21 method table 00758838 (27 slots)
// Read from the original .rdata 00758838..007588A3: identical to CSWGuiPanel 0073E010 except +00 (006E42A0)
// and +3C (006DFF10).
struct GuiM1OptionsMainMethods {
    void* (__fastcall* s00)(OptionsMain*, void*, Word);                       // +00 006E42A0 ~CSWGuiOptionsMain (deleting)
    void (__fastcall* s04)(Object*, void*, const Extent*);                    // +04 0040A5A0 CSWGuiPanel::SetExtent
    void (__fastcall* s08)(Object*, void*, Object*, Word);                    // +08 0040A630 CSWGuiPanel::SetActiveControl
    void (__fastcall* s0c)(Object*, void*, Word);                             // +0C 0060E760 void_return [RET 4]
    Word (__fastcall* s10)(Object*, void*, Word, Word);                       // +10 00409EB0 return_zero_0x8
    void (__fastcall* s14)(Object*, void*);                                   // +14 004015A0
    void (__fastcall* s18)(Object*, void*);                                   // +18 004015A0
    void (__fastcall* s1c)(Object*, void*);                                   // +1C 004015A0
    void (__fastcall* s20)(Object*, void*);                                   // +20 004015A0
    void (__fastcall* s24)(Object*, void*);                                   // +24 004015A0
    Object* (__fastcall* s28)(Object*, void*);                                // +28 00641DB0 self_return
    Word (__fastcall* s2c)(Object*, void*);                                   // +2C 0063E7F0 return_zero
    Word (__fastcall* s30)(Object*, void*);                                   // +30 0063E7F0 return_zero
    void (__fastcall* s34)(Panel*, void*, Word);                              // +34 0040B760 CSWGuiPanel::Draw
    void (__fastcall* s38)(Object*, void*, Word);                             // +38 0060E760 void_return [RET 4]
    void (__fastcall* s3c)(Object*, void*, I32, I32);                         // +3C 006DFF10 CSWGuiOptionsMain::HandleInputEvent
    Object* (__fastcall* s40)(Object*, void*, I32, I32);                      // +40 0040B690 CSWGuiPanel::HitCheckMouse
    Object* (__fastcall* s44)(Object*, void*);                                // +44 0040B630 CSWGuiPanel::GetActiveControl
    void (__fastcall* s48)(Panel*, void*);                                    // +48 0040B870 CSWGuiPanel::OnPanelAdded
    void (__fastcall* s4c)(Panel*, void*);                                    // +4C 0040C170 CSWGuiPanel::OnPanelRemoved
    void (__fastcall* s50)(Object*, void*, Word);                             // +50 0040B640 OnAButtonPressed
    void (__fastcall* s54)(Object*, void*, Word);                             // +54 0040B650 OnBButtonPressed
    void (__fastcall* s58)(Object*, void*, Word);                             // +58 0040B660 OnXButtonPressed
    void (__fastcall* s5c)(Object*, void*, Word);                             // +5C 0040B670 OnYButtonPressed
    void (__fastcall* s60)(Object*, void*, Word);                             // +60 0040B680 OnBlackButtonPressed
    String* (__fastcall* s64)(Panel*, void*, String*);                        // +64 0040A900 GetFullScreenBG
    void (__fastcall* s68)(Panel*, void*);                                    // +68 0040A9C0 ResetFont
};
static_assert(sizeof(GuiM1OptionsMainMethods) == 0x6C, "00758838 slot count (27)");
extern "C" const GuiM1OptionsMainMethods gui_m1_options_main_methods_00758838 = {
    &kotor_m1_options_deleting_destruct_006e42a0,   // +00
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
    &kotor_panel_draw_0040B760,                     // +34
    &gui_load_slot_ret4_0060e760,                   // +38
    &kotor_m1_options_handle_input_006dff10,        // +3C
    &gui_load_panel_hit_check_0040b690,             // +40
    &gui_load_panel_active_control_0040b630,        // +44
    &gui_load_panel_on_added_0040b870,              // +48
    &gui_load_panel_on_removed_0040c170,            // +4C
    &gui_load_panel_button_a_0040b640,              // +50
    &gui_load_panel_button_b_0040b650,              // +54
    &gui_load_panel_button_x_0040b660,              // +58
    &gui_load_panel_button_y_0040b670,              // +5C
    &gui_load_panel_button_black_0040b680,          // +60
    &gui_load_panel_full_screen_bg_0040a900,        // +64
    &gui_load_panel_reset_font_0040a9c0,            // +68
};

// ======================================================================== CSWGuiMainMenu::OnOptionsPicked
// 0067B2F0 (thiscall, RET 4). Guard 0067B308..0067B32A identical to the M49 trap unit's authored guard.
void __thiscall gui_load::MainMenuHandlers::OnOptionsPicked(kotor_input_hover::Control* control) {
    gui_load::MainMenu* const menu = reinterpret_cast<gui_load::MainMenu*>(this);
    if ((menu->flags & 0x600u) == 0x400u) return;                                  // 0067B308..15
    if (menu->u140c == 0) return;                                                  // 0067B317..1F (+140C, set by Draw 0067AC28)
    if (word_at(control, 0x4C) == 0) return;                                       // 0067B321..2A
    void* const memory = c2da_directory_new_006fa7e6(0x1220);                     // 0067B32C..36 operator new (006FA7E6)
    OptionsMain* panel = nullptr;                                                  // 0067B356 (xor eax,eax when new returned 0)
    if (memory) {                                                                  // 0067B33D..47
        try {
            panel = kotor_m1_options_construct_006e3e80(static_cast<OptionsMain*>(memory), nullptr,
                                                        menu->manager);            // 0067B349..4F ([esi+18h])
        } catch (...) {
            live_free(memory);                                                     // EH state 0 funclet 0072E300 -> 006FA390
            throw;
        }
    }
    kotor_fastcall_0040bc70(menu->manager, nullptr, panel, 3, 1);                  // 0067B358..68 AddPanel(p, 3, 1)
}

extern "C" {
// ======================================================================== 006E3E80 ctor (thiscall, RET 4)
OptionsMain* __fastcall kotor_m1_options_construct_006e3e80(OptionsMain* self, void*, Object* manager) noexcept(false) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                    // 006E3E9E..A7
    int built = kPanel;
    try {
        self->vtbl = reinterpret_cast<const gui_load::ObjectVtbl*>(&gui_m1_options_main_methods_00758838);   // 006E3EC9
        // 006E3ECF eh_vector_constructor_iterator(+64, 1C4h, 6, 0041C0F0, 00419DA0): element k failing destroys
        // elements k-1..0 before propagating.
        for (int i = 0; i < 6; ++i) {
            try { gui_load_button_construct_0041c0f0(&self->buttons[i], nullptr); }
            catch (...) { while (i-- > 0) gui_load_button_destruct_00419da0(&self->buttons[i], nullptr); throw; }
        }
        built = kButtons;
        gui_load_label_construct_0041acd0(&self->desc_item, nullptr);  built = kDescItem;   // 006E3ED4..DF
        gui_load_label_construct_0041acd0(&self->lbl_title, nullptr);  built = kTitle;      // 006E3EE4..EF
        gui_load_button_construct_0041c0f0(&self->btn_back, nullptr);  built = kBack;       // 006E3EF4..F01
        gui_load_listbox_construct_0041be40(&self->lb_desc, nullptr);  built = kListBox;    // 006E3F06..11
        {
            Ref layout;
            layout.construct_00406d80("optionsmain");                             // 006E3F18..25 (0075882C)
            gui_load_panel_start_layout_0040a680(self, nullptr, &layout);          // 006E3F2A..31
        }
        init_control(self, &self->lbl_title, "LBL_TITLE");                        // 006E3F36..66 (00751A20)
        init_control(self, &self->lb_desc, "LB_DESC");                            // 006E3F6B..9B (00755D6C)
        init_control(self, &self->buttons[0], "BTN_GAMEPLAY");                    // 006E3FA0..CA (00755DB0)
        init_control(self, &self->buttons[1], "BTN_FEEDBACK");                    // 006E3FCF..FF (00755DA0)
        init_control(self, &self->buttons[2], "BTN_AUTOPAUSE");                   // 006E4004..34 (00755D90)
        init_control(self, &self->buttons[3], "BTN_GRAPHICS");                    // 006E4039..69 (00755D80)
        init_control(self, &self->buttons[4], "BTN_SOUND");                       // 006E406E..9E (00755D74)
        {
            String name("BTN_BACK");                                               // 006E40A3..DE (007548AC)
            // Description strrefs in CSWGuiControl+58h (read by OnEnterSlot 006E0014), stored between the
            // BTN_BACK string construction and its InitControl.
            self->buttons[0].u58 = 0xBB86u;                                        // 006E40AC (+0BC)
            self->buttons[1].u58 = 0xBD99u;                                        // 006E40B6 (+280)
            self->buttons[2].u58 = 0xBDBCu;                                        // 006E40C0 (+444)
            self->buttons[3].u58 = 0xBB8Au;                                        // 006E40CA (+608)
            self->buttons[4].u58 = 0xBB8Cu;                                        // 006E40D4 (+7CC)
            gui_load_panel_init_control_0040b930(self, nullptr, &self->btn_back, &name, 1);   // 006E40E3..F2
        }                                                                          // 006E40F7..FF
        for (int i = 0; i < 6; ++i) {                                              // 006E4104..4E: all six buttons
            Button* const b = &self->buttons[i];
            Extent moved = b->extent;                                              // 006E4110..35 copy +4..+10
            moved.top = b->extent.top + 0x14;                                      // 006E4124 (y + 20)
            b->vtbl->set_extent(b, nullptr, &moved);                               // 006E4139..3C vslot +04
        }
        reinterpret_cast<unsigned char*>(self)[0x1201] = 1;                        // 006E4152 (lb_desc+2C1)
        gui_load_panel_stop_layout_0040b8f0(self, nullptr);                        // 006E4150..59
        gui_load_control_add_event_0041ab20(&self->btn_back, nullptr, 0x27, event_target(self),
            kotor_m1_shared::event_func(&kotor_m1_shared::PanelEventThunks::OnBButtonPressed_00624bb0));   // 006E415E..68 (00624BB0)
        self->btn_back.flags &= 0xFFFFFFFBu;                                       // 006E416D
        {
            Control* const proto = self->lb_desc.proto;                            // 006E4171 ([esi+120Ch])
            const LabelInitView* lv = gui_lb::vt<LabelInitView>(static_cast<Object*>(&self->desc_item));   // 006E4177
            lv->initialize98(&self->desc_item, nullptr,
                             word_of(&proto->extent), word_of(proto), 0x3F800000u);   // 006E417D..93 (proto+4, proto, 1.0f)
        }
        struct Pick { Button* button; void (__thiscall OptionsMainHandlers::*func)(kih::Control*); };
        const Pick picks[5] = {
            { &self->buttons[0], &OptionsMainHandlers::OnGameplayOpt_006de240 },   // 006E4193 / 006E41F2
            { &self->buttons[1], &OptionsMainHandlers::OnFeedbackOpt_006e2df0 },   // 006E41A2 / 006E4201
            { &self->buttons[2], &OptionsMainHandlers::OnAutopauseOpt_006de2c0 },  // 006E41B7 / 006E4210
            { &self->buttons[3], &OptionsMainHandlers::OnGraphicsOpt_006e3d80 },   // 006E41CC / 006E421F
            { &self->buttons[4], &OptionsMainHandlers::OnSoundOpt_006e3e00 },      // 006E41DF / 006E4232
        };
        for (const Pick& p : picks)                                                // 006E4193..41ED (event 27h)
            gui_load_control_add_event_0041ab20(p.button, nullptr, 0x27, event_target(self), handler(p.func));
        for (const Pick& p : picks)                                                // 006E41F2..4240 (event 2Dh)
            gui_load_control_add_event_0041ab20(p.button, nullptr, 0x2D, event_target(self), handler(p.func));
        for (int i = 0; i < 6; ++i) {                                              // 006E4245..77 (six buttons)
            Button* const b = &self->buttons[i];
            gui_load_control_add_event_0041ab20(b, nullptr, 0, event_target(self),
                handler(&OptionsMainHandlers::OnEnterSlot_006e0010));              // 006E4250..5E (006E0010)
            gui_load_border_params_set_pulsing_00414d00(b->hilight.params, nullptr, 1, 1, 0);   // 006E4263..6B (button+F4)
        }
        self->vtbl->set_active_control(self, nullptr, &self->buttons[0], 0);       // 006E4279..80 vslot +08
    } catch (...) {
        unwind_options(self, built);
        throw;
    }
    return self;                                                                   // 006E4288
}

// ======================================================================== 006E42C0 dtor (thiscall, RET)
// No vptr store in this body (006E42C0..4358); members in reverse order, then CSWGuiPanel::~CSWGuiPanel.
void __fastcall kotor_m1_options_destruct_006e42c0(OptionsMain* self, void*) {
    gui_load_listbox_destruct_0041b060(&self->lb_desc, nullptr);                  // 006E42DD..EB
    gui_load_button_destruct_00419da0(&self->btn_back, nullptr);                  // 006E42F0..FB
    gui_load_label_destruct_00419d10(&self->lbl_title, nullptr);                  // 006E4300..0B
    gui_load_label_destruct_00419d10(&self->desc_item, nullptr);                  // 006E4310..1B
    for (int i = 6; i-- > 0;)                                                      // 006E4320..35 eh_vector_destructor_iterator
        gui_load_button_destruct_00419da0(&self->buttons[i], nullptr);             //   (+64, 1C4h, 6, 00419DA0): reverse order
    gui_load_panel_destruct_0040cf70(self, nullptr);                               // 006E433A..44
}
// ======================================================================== 006E42A0 scalar deleting dtor (RET 4)
void* __fastcall kotor_m1_options_deleting_destruct_006e42a0(OptionsMain* self, void*, Word flags) {
    kotor_m1_options_destruct_006e42c0(self, nullptr);                             // 006E42A3
    if (flags & 1u) live_free(self);                                               // 006E42A8..B5 (_free 006FA390)
    return self;                                                                   // 006E42B8
}

// ======================================================================== 006DFF10 HandleInputEvent(event, pressed)
// thiscall, RET 8. Jump table 006DFFE0 (5 entries) indexed by the byte table 006DFFF4 for event-28h in 0..12h:
// index bytes 00 04 04 04 04 01 00 04 04 04 04 04 04 04 04 04 04 02 03 -> 28h,2Eh: Back (006DFF58);
// 2Dh: sound (006DFF3F); 39h: 006DFF99; 3Ah: 006DFFBB; others: default (006DFFCE).
void __fastcall kotor_m1_options_handle_input_006dff10(Object* object, void*, I32 event, I32 pressed) {
    OptionsMain* const self = static_cast<OptionsMain*>(static_cast<Panel*>(object));
    if (pressed != 0) {                                                            // 006DFF17..1F
        switch (event) {                                                           // 006DFF25..38
        case 0x28: case 0x2E:                                                      // 006DFF58 Back
            manager_of(self)->PlayGuiSound(0);                                     // 006DFF58..5D (0040A140)
            kotor_fastcall_0040be00(self->manager, nullptr);                       // 006DFF62..65 PopModalPanel
            self->flags = (self->flags & 0xFFFFFCFFu) | 0x400u;                    // 006DFF6A..79 state 400h
            client_movies::SaveOptionsApp_005edf70(
                ptr_at(kotor_menu_window_cells::manager_007a39fc, 4), nullptr);    // 006DFF7C..85 ([[007A39FC]+4]) -> ini
            break;
        case 0x2D:                                                                 // 006DFF3F
            manager_of(self)->PlayGuiSound(0);                                     // 006DFF3F..44
            break;
        case 0x39: {                                                               // 006DFF99: LB_DESC vslot +3C(31h, 1)
            gui_lb::ListBox* const lb = &self->lb_desc;
            gui_lb::vt<gui_lb::ControlCallVtbl>(static_cast<Control*>(lb))->handle_input_event(lb, nullptr, 0x31, 1);
            break;
        }
        case 0x3A: {                                                               // 006DFFBB: LB_DESC vslot +3C(32h, 1)
            gui_lb::ListBox* const lb = &self->lb_desc;
            gui_lb::vt<gui_lb::ControlCallVtbl>(static_cast<Control*>(lb))->handle_input_event(lb, nullptr, 0x32, 1);
            break;
        }
        default:
            break;
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, pressed);          // 006DFF49/8C/AE/D0 (00409E60)
}

// ======================================================================== 006DE340 SetDescription(strref)
// thiscall, RET 4. EH state 0 (FuncInfo 00736788) covers the CExoString copy.
void __fastcall kotor_m1_options_set_description_006de340(OptionsMain* self, void*, Word strref) noexcept(false) {
    unsigned char* const params = reinterpret_cast<unsigned char*>(self) + 0xBE4;  // desc_item text params (+D0+18)
    gui_load_text_params_set_strref_00415e50(params, nullptr, strref);             // 006DE35D..69 (00415E50)
    // 006DE36E..73 CExoString copy (005E5B00 via the M49 lifetime shell r8-copy-lifetime-join -> WindowCopy005e5b00);
    // its destructor (005E5C20, 006DE405..11) also runs on unwind (EH state 0).
    String text(*reinterpret_cast<const String*>(params));
    gui_load_listbox_clear_items_00419a90(&self->lb_desc, nullptr);                // 006DE378..88
    Extent extent = self->desc_item.extent;                                        // 006DE38D..AE (+B00)
    gui_load::Text* const text_object = &self->desc_item.text;                     // 006DE3B9 (+BCC)
    if (text.BorrowedLength005e5790() > 0)                                       // 006DE3B2..BF (005E5790)
        extent.height = static_cast<I32>(gui_lb::vt<TextIdealView>(text_object)->ideal_height(text_object, nullptr));  // 006DE3C1..C3 vslot +08
    else
        extent.height = gui_load_text_font_height_00414ee0(text_object, nullptr);  // 006DE3C8 (00414EE0)
    Label* const item = &self->desc_item;                                          // 006DE3D1 (+AFC)
    item->vtbl->set_extent(item, nullptr, &extent);                                // 006DE3DB..E0 vslot +04
    Control* items[1] = { item };                                                  // 006DE3F2 (argument slot reused)
    gui_m48_listbox_add_controls_0041c2c0(&self->lb_desc, nullptr, items, 1, 0, 0, 0);   // 006DE3E3..F6
    gui_load_listbox_set_active_control_0041c160(&self->lb_desc, nullptr, item, 0);       // 006DE3FB..400
}
}

// ======================================================================== event callbacks
// ---- 006E0010 OnEnterSlot(control): MOV EAX,[ESP+4]; MOV EDX,[EAX+58h]; MOV [ESP+4],EDX; JMP 006DE340
// (ECX unchanged; SetDescription's RET 4 cleans the replaced stack word).
void __thiscall OptionsMainHandlers::OnEnterSlot_006e0010(kih::Control* control) {
    kotor_m1_options_set_description_006de340(options_of(this), nullptr, word_at(control, 0x58));   // 006E0010..1B
}
// ---- Sub-panel handlers (thiscall, RET 4). Original: guard [control+4Ch] == 0 -> epilogue, then
// new(size) sub-panel ctor(manager) + AddPanel(p, 3, 1). The sub-panels are M1b debt (205 fn / 0xC3F0):
// D83 branch trap at the first post-guard instruction (the operator-new push); see TRAPS-DELTA.json.
void __thiscall OptionsMainHandlers::OnGameplayOpt_006de240(kih::Control* control) {
    if (word_at(control, 0x4C) == 0) return;                                       // 006DE24E..61
    void* const memory = c2da_directory_new_006fa7e6(0x22A0);                     // 006DE263..6D operator new (006FA7E6)
    Panel* panel = nullptr;                                                        // 006DE28D (xor eax,eax when new returned 0)
    if (memory) {
        try {
            panel = kotor_m1_opt_gameplay_construct_006e69a0(memory, nullptr, options_of(this)->manager);   // 006DE280..86 ([esi+18h])
        } catch (...) {
            live_free(memory);                                                     // EH state 0 (FuncInfo 00786A30) funclet 00736740 -> 006FA390
            throw;
        }
    }
    kotor_fastcall_0040bc70(options_of(this)->manager, nullptr, panel, 3, 1);      // 006DE28F..9F AddPanel(p, 3, 1)
}
void __thiscall OptionsMainHandlers::OnAutopauseOpt_006de2c0(kih::Control* control) {
    if (word_at(control, 0x4C) == 0) return;                                       // 006DE2CE..E1
    void* const memory = c2da_directory_new_006fa7e6(0x1984);                     // 006DE2E3..ED operator new (006FA7E6)
    Panel* panel = nullptr;                                                        // 006DE30D (xor eax,eax when new returned 0)
    if (memory) {
        try {
            panel = kotor_m1_opt_autopause_construct_006e76a0(memory, nullptr, options_of(this)->manager);   // 006DE300..06 ([esi+18h])
        } catch (...) {
            live_free(memory);                                                     // EH state 0 (FuncInfo 00786A30) funclet 00736740 -> 006FA390
            throw;
        }
    }
    kotor_fastcall_0040bc70(options_of(this)->manager, nullptr, panel, 3, 1);      // 006DE30F..1F AddPanel(p, 3, 1)
}
void __thiscall OptionsMainHandlers::OnFeedbackOpt_006e2df0(kih::Control* control) {
    if (word_at(control, 0x4C) == 0) return;                                       // 006E2DFE..11
    void* const memory = c2da_directory_new_006fa7e6(0xC2C);                     // 006E2E13..1D operator new (006FA7E6)
    Panel* panel = nullptr;                                                        // 006E2E3D (xor eax,eax when new returned 0)
    if (memory) {
        try {
            panel = kotor_m1_opt_feedback_construct_006e2a70(memory, nullptr, options_of(this)->manager);   // 006E2E30..36 ([esi+18h])
        } catch (...) {
            live_free(memory);                                                     // EH state 0 (FuncInfo 00786A30) funclet 00736740 -> 006FA390
            throw;
        }
    }
    kotor_fastcall_0040bc70(options_of(this)->manager, nullptr, panel, 3, 1);      // 006E2E3F..4F AddPanel(p, 3, 1)
}
void __thiscall OptionsMainHandlers::OnGraphicsOpt_006e3d80(kih::Control* control) {
    if (word_at(control, 0x4C) == 0) return;                                       // 006E3D8E..A1
    void* const memory = c2da_directory_new_006fa7e6(0x18C0);                     // 006E3DA3..AD operator new (006FA7E6)
    kotor_m1_options::OptionsGraphics* panel = nullptr;                            // 006E3DCD (xor eax,eax when new returned 0)
    if (memory) {                                                                  // 006E3DB4..BE
        try {
            panel = kotor_m1_options_graphics_construct_006e2e70(static_cast<kotor_m1_options::OptionsGraphics*>(memory),
                                                                 nullptr, options_of(this)->manager);   // 006E3DC0..C6 ([esi+18h])
        } catch (...) {
            live_free(memory);                                                     // EH state 0 (FuncInfo 0073674B) -> 006FA390
            throw;
        }
    }
    kotor_fastcall_0040bc70(options_of(this)->manager, nullptr, panel, 3, 1);      // 006E3DCF..DF AddPanel(p, 3, 1)
}
void __thiscall OptionsMainHandlers::OnSoundOpt_006e3e00(kih::Control* control) {
    if (word_at(control, 0x4C) == 0) return;                                       // 006E3E0E..21
    void* const memory = c2da_directory_new_006fa7e6(0x16F0);                     // 006E3E23..2D operator new (006FA7E6)
    kotor_m1_options::OptionsSound* panel = nullptr;                               // 006E3E4D (xor eax,eax when new returned 0)
    if (memory) {                                                                  // 006E3E34..3E
        try {
            panel = kotor_m1_options_sound_construct_006e3550(static_cast<kotor_m1_options::OptionsSound*>(memory),
                                                              nullptr, options_of(this)->manager);   // 006E3E40..46 ([esi+18h])
        } catch (...) {
            live_free(memory);                                                     // EH state 0 (FuncInfo 0073674B) -> 006FA390
            throw;
        }
    }
    kotor_fastcall_0040bc70(options_of(this)->manager, nullptr, panel, 3, 1);      // 006E3E4F..5F AddPanel(p, 3, 1)
}
