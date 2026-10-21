// lane-m1-opt-gameplay: CSWGuiInGameGameplay (options "Gameplay" sub-panel, optgameplay.gui).
//   ctor 006E69A0, dtor body 006E7130, scalar deleting dtor 006E7110, method table 00758E00,
//   HandleInputEvent 006E6180 (shared with CSWGuiOptionsMouse table 007585F8), OnPanelAdded 006E6170,
//   SetUpOptions 006E6770, SetDifficultyText 006E6520, SetDescription 006E6620, event callbacks
//   OnAutoLevelUp 006E62A0, OnAutoSave 006E62E0, OnInvertCam 006E6330, OnReverse 006E6380,
//   OnCombatMovement 006E63D0, OnKeyMap 006E6420, OnMouse 006E64A0, OnEnterDiff 006E6710, OnExitDiff 006E6750,
//   OnEnterSlot 006E68A0, OnDefault 006E68B0, OnDifficultyLeft 006E68E0, OnDifficultyRight 006E6930,
//   CClientOptions::SetAutoLevelUpNPCs 0061B740..0061B771, CClientOptions::SetDefaultGameplayOptions 0061D4E0.
// Original: swkotor.exe GOG K1 (static disassembly only). Conventions: D1 / D21 / D24 / D47; D30-FINAL (no float
// arithmetic; the 1.0f Initialize argument is passed as its raw bits 3F800000h as at 006E6E91).
// Back (event 28h/2Eh) only pops the panel (006E61AF..D1): the options are kept in CClientOptions and written to the
// run-dir swkotor.ini by CSWGuiOptionsMain's Back (SaveOptions 005EDF70, lane-m1-options).
#include "93783737f5-opt_gameplay.h"

using namespace kotor_m1_opt;

// ---- external providers (M54 functional.map 20261007T195950859939Z)
namespace lane_config_required {
void* __fastcall GetPartyTable_004aee70(void* server, void* unused_edx);       // startup46-config-leaves.obj (004AEE70)
void __fastcall AutoLevelUpNPCs_00563b90(void* party, void* unused_edx);       // m48t-main-traps-config.obj (00563B90; member loop = existing D83 branch trap 00563B9C)
}
extern "C" {
std::int32_t __fastcall c2da_current_get_int_00413660(void* c2da, void*, std::int32_t row, String* column,
                                                       std::int32_t* out);     // startup-misc-c2da.obj (C2DA::GetINTEntry 00413660)
extern void* kotor_cell_rules_007a3a28;                                         // CSWRules* cell 007A3A28 (game-prepare owner)
}

namespace {
Gameplay* gameplay_of(GameplayHandlers* h) { return reinterpret_cast<Gameplay*>(h); }
gui_load::EventFunc handler(void (__thiscall GameplayHandlers::*f)(kih::Control*)) { return static_cast<gui_load::EventFunc>(f); }
Word control_value(const kih::Control* control) { return word_at(control, 0x4C); }   // [control+4Ch] (CSWGuiControl last event value)
// CSWRules (007A3A28) +B8 -> +7C: difficulty C2DA (006E654A..55, 006E693E..4A); C2DA +24 row count (006E694D, 006E65F1).
// LB_DESC at +2E4 in both panels that use 006E6180 (CSWGuiInGameGameplay and CSWGuiOptionsMouse: 006E622E/006E6250).
Object* lb_desc_of(Panel* panel) { return reinterpret_cast<ListBox*>(reinterpret_cast<unsigned char*>(panel) + 0x2E4); }
void* difficulty_table() { return ptr_at(ptr_at(kotor_cell_rules_007a3a28, 0xB8), 0x7C); }
void add_event(Control* control, I32 id, Gameplay* self, void (__thiscall GameplayHandlers::*f)(kih::Control*)) {
    gui_load_control_add_event_0041ab20(control, nullptr, id, event_target(self), handler(f));
}
// Constructed-member stages of 006E69A0 (EH states 0..10h of FuncInfo 00737903; 11h+ are String temporaries).
enum Stage { kPanel = 0, kDesc, kTitle, kList, kBack, kDefault, kKeymap, kMouse, kDifficulty, kRight, kLeft,
             kLevelUp, kInvertCam, kAutoSave, kReverse, kDisableMove, kSlider, kLabelSen };
void unwind_gameplay(Gameplay* self, int built) {
    if (built >= kLabelSen) gui_load_label_destruct_00419d10(&self->lbl_mousesen, nullptr);
    if (built >= kSlider) gui_m48_slider_destruct_0041bb40(&self->sli_mousesen, nullptr);
    if (built >= kDisableMove) gui_m48_button_toggle_destruct_0041cad0(&self->cb_disablemove, nullptr);
    if (built >= kReverse) gui_m48_button_toggle_destruct_0041cad0(&self->cb_reverse, nullptr);
    if (built >= kAutoSave) gui_m48_button_toggle_destruct_0041cad0(&self->cb_autosave, nullptr);
    if (built >= kInvertCam) gui_m48_button_toggle_destruct_0041cad0(&self->cb_invertcam, nullptr);
    if (built >= kLevelUp) gui_m48_button_toggle_destruct_0041cad0(&self->cb_levelup, nullptr);
    if (built >= kLeft) gui_load_button_destruct_00419da0(&self->btn_diffleft, nullptr);
    if (built >= kRight) gui_load_button_destruct_00419da0(&self->btn_diffright, nullptr);
    if (built >= kDifficulty) gui_load_button_destruct_00419da0(&self->btn_difficulty, nullptr);
    if (built >= kMouse) gui_load_button_destruct_00419da0(&self->btn_mouse, nullptr);
    if (built >= kKeymap) gui_load_button_destruct_00419da0(&self->btn_keymap, nullptr);
    if (built >= kDefault) gui_load_button_destruct_00419da0(&self->btn_default, nullptr);
    if (built >= kBack) gui_load_button_destruct_00419da0(&self->btn_back, nullptr);
    if (built >= kList) gui_load_listbox_destruct_0041b060(&self->lb_desc, nullptr);
    if (built >= kTitle) gui_load_label_destruct_00419d10(&self->lbl_title, nullptr);
    if (built >= kDesc) gui_load_label_destruct_00419d10(&self->desc_item, nullptr);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
// Sub-panel open sequence shared by OnKeyMap 006E6420 / OnMouse 006E64A0 (and the CSWGuiOptionsMain handlers):
// new(size) (006FA7E6); ctor(this->manager) when non-null else 0; AddPanel(p, 3, 1) (0040BC70). EH state 0
// (FuncInfo 0073773B) frees the allocation if the ctor throws.
void open_sub_panel(Panel* owner, std::uint32_t size,
                    Panel* (__fastcall* construct)(void*, void*, Object*) noexcept(false)) {
    void* const memory = c2da_directory_new_006fa7e6(size);
    Panel* panel = nullptr;
    if (memory) {
        try { panel = construct(memory, nullptr, owner->manager); }
        catch (...) { live_free(memory); throw; }
    }
    kotor_fastcall_0040bc70(owner->manager, nullptr, panel, 3, 1);
}
}

// ======================================================================== D21 method table 00758E00 (27 slots)
// Read from the original .rdata 00758E00..00758E6B: CSWGuiPanel 0073E010 slots except +00 006E7110,
// +3C 006E6180, +48 006E6170.
extern "C" const GuiM1OptGameplayMethods gui_m1_opt_gameplay_methods_00758e00 = {
    &kotor_m1_opt_gameplay_deleting_destruct_006e7110,   // +00
    &gui_load_panel_set_extent_0040a5a0,                 // +04
    &gui_load_panel_set_active_control_0040a630,         // +08
    &gui_load_slot_ret4_0060e760,                        // +0C
    &gui_load_slot_zero8_00409eb0,                       // +10
    &gui_load_slot_ret_004015a0,                         // +14
    &gui_load_slot_ret_004015a0,                         // +18
    &gui_load_slot_ret_004015a0,                         // +1C
    &gui_load_slot_ret_004015a0,                         // +20
    &gui_load_slot_ret_004015a0,                         // +24
    &gui_load_slot_self_00641db0,                        // +28
    &gui_load_slot_zero_0063e7f0,                        // +2C
    &gui_load_slot_zero_0063e7f0,                        // +30
    &kotor_panel_draw_0040B760,                          // +34
    &gui_load_slot_ret4_0060e760,                        // +38
    &kotor_m1_opt_gameplay_handle_input_006e6180,        // +3C
    &gui_load_panel_hit_check_0040b690,                  // +40
    &gui_load_panel_active_control_0040b630,             // +44
    &kotor_m1_opt_gameplay_on_panel_added_006e6170,      // +48
    &gui_load_panel_on_removed_0040c170,                 // +4C
    &gui_load_panel_button_a_0040b640,                   // +50
    &gui_load_panel_button_b_0040b650,                   // +54
    &gui_load_panel_button_x_0040b660,                   // +58
    &gui_load_panel_button_y_0040b670,                   // +5C
    &gui_load_panel_button_black_0040b680,               // +60
    &gui_load_panel_full_screen_bg_0040a900,             // +64
    &gui_load_panel_reset_font_0040a9c0,                 // +68
};

extern "C" {
// ======================================================================== 006E69A0 ctor (thiscall, RET 4)
Panel* __fastcall kotor_m1_opt_gameplay_construct_006e69a0(void* memory, void*, Object* manager) noexcept(false) {
    Gameplay* const self = static_cast<Gameplay*>(memory);
    gui_load_panel_construct_0040b570(self, nullptr, manager);                    // 006E69C6..CB
    int built = kPanel;
    try {
        set_vptr(self, &gui_m1_opt_gameplay_methods_00758e00);                     // 006E69DB
        gui_load_label_construct_0041acd0(&self->desc_item, nullptr);       built = kDesc;        // 006E69D0..E1
        gui_load_label_construct_0041acd0(&self->lbl_title, nullptr);       built = kTitle;       // 006E69E6..F1
        gui_load_listbox_construct_0041be40(&self->lb_desc, nullptr);       built = kList;        // 006E69F6..A01
        gui_load_button_construct_0041c0f0(&self->btn_back, nullptr);       built = kBack;        // 006E6A06..11
        gui_load_button_construct_0041c0f0(&self->btn_default, nullptr);    built = kDefault;     // 006E6A16..21
        gui_load_button_construct_0041c0f0(&self->btn_keymap, nullptr);     built = kKeymap;      // 006E6A26..31
        gui_load_button_construct_0041c0f0(&self->btn_mouse, nullptr);      built = kMouse;       // 006E6A36..41
        gui_load_button_construct_0041c0f0(&self->btn_difficulty, nullptr); built = kDifficulty;  // 006E6A46..53
        gui_load_button_construct_0041c0f0(&self->btn_diffright, nullptr);  built = kRight;       // 006E6A58..63
        gui_load_button_construct_0041c0f0(&self->btn_diffleft, nullptr);   built = kLeft;        // 006E6A68..73
        // Inlined CSWGuiOptionsCheckbox ctors (0041CA30 + vptr 00758258 = body of 006E00D0).
        kotor_m1_opt_checkbox_construct_006e00d0(&self->cb_levelup, nullptr);     built = kLevelUp;     // 006E6A78..8A
        kotor_m1_opt_checkbox_construct_006e00d0(&self->cb_invertcam, nullptr);   built = kInvertCam;   // 006E6A90..A2
        kotor_m1_opt_checkbox_construct_006e00d0(&self->cb_autosave, nullptr);    built = kAutoSave;    // 006E6AA8..BA
        kotor_m1_opt_checkbox_construct_006e00d0(&self->cb_reverse, nullptr);     built = kReverse;     // 006E6AC0..D2
        kotor_m1_opt_checkbox_construct_006e00d0(&self->cb_disablemove, nullptr); built = kDisableMove; // 006E6AD8..EA
        gui_load_slider_construct_0041baa0(&self->sli_mousesen, nullptr);   built = kSlider;      // 006E6AF0..FB
        gui_load_label_construct_0041acd0(&self->lbl_mousesen, nullptr);    built = kLabelSen;    // 006E6B00..0B
        {
            Ref layout;
            layout.construct_00406d80("optgameplay");                                // 006E6B12..1F (00758DF4)
            gui_load_panel_start_layout_0040a680(self, nullptr, &layout);           // 006E6B24..2B
        }
        init_control(self, &self->btn_difficulty, "BTN_DIFFICULTY");              // 006E6B30..5A (00758DE4)
        init_control(self, &self->btn_diffright, "BTN_DIFFRIGHT");                 // 006E6B5F..8F (00758DD4)
        init_control(self, &self->btn_diffleft, "BTN_DIFFLEFT");                   // 006E6B94..C4 (00758DC4)
        init_control(self, &self->cb_invertcam, "CB_INVERTCAM");                   // 006E6BC9..F9 (00758DB4)
        init_control(self, &self->cb_levelup, "CB_LEVELUP");                       // 006E6BFE..2E (00758DA8)
        init_control(self, &self->cb_autosave, "CB_AUTOSAVE");                     // 006E6C33..63 (00758D9C)
        init_control(self, &self->cb_reverse, "CB_REVERSE");                       // 006E6C68..98 (00758D90)
        init_control(self, &self->cb_disablemove, "CB_DISABLEMOVE");               // 006E6C9D..CD (00758D80)
        init_control(self, &self->sli_mousesen, "SLI_MOUSESEN");                   // 006E6CD2..D02 (007585DC)
        init_control(self, &self->lbl_mousesen, "LBL_MOUSESEN");                   // 006E6D07..37 (007585CC)
        init_control(self, &self->lbl_title, "LBL_TITLE");                         // 006E6D3C..6C (00751A20)
        init_control(self, &self->lb_desc, "LB_DESC");                             // 006E6D71..A1 (00755D6C)
        init_control(self, &self->btn_back, "BTN_BACK");                           // 006E6DA6..D6 (007548AC)
        init_control(self, &self->btn_default, "BTN_DEFAULT");                     // 006E6DDB..E0B (007583B4)
        init_control(self, &self->btn_keymap, "BTN_KEYMAP");                       // 006E6E10..40 (00758D74)
        init_control(self, &self->btn_mouse, "BTN_MOUSE");                         // 006E6E45..75 (00758D68)
        set_byte(self, 0x5A5, 1);                                                  // 006E6E7C (lb_desc+2C1)
        gui_load_panel_stop_layout_0040b8f0(self, nullptr);                        // 006E6E7A..83
        {
            Control* const proto = self->lb_desc.proto;                            // 006E6E88 ([esi+5B0h])
            vt<LabelInitView>(static_cast<Object*>(&self->desc_item))->initialize98(
                &self->desc_item, nullptr, word_of(&proto->extent), word_of(proto), 0x3F800000u);   // 006E6E8E..9E
        }
        add_event(&self->btn_difficulty, 0x2F, self, &GameplayHandlers::OnDifficultyLeft_006e68e0);    // 006E6EA4..AE
        add_event(&self->btn_difficulty, 0x30, self, &GameplayHandlers::OnDifficultyRight_006e6930);   // 006E6EB3..BD
        add_event(&self->btn_difficulty, 0, self, &GameplayHandlers::OnEnterDiff_006e6710);            // 006E6EC2..CC
        add_event(&self->btn_difficulty, 1, self, &GameplayHandlers::OnExitDiff_006e6750);             // 006E6ED1..DB
        self->btn_diffleft.flags &= 0xFFFFFFFBu;                                   // 006E6EE0..FB
        add_event(&self->btn_diffleft, 0x27, self, &GameplayHandlers::OnDifficultyLeft_006e68e0);      // 006E6EEC..FE
        self->btn_diffright.flags &= 0xFFFFFFFBu;                                  // 006E6F03..19
        add_event(&self->btn_diffright, 0x27, self, &GameplayHandlers::OnDifficultyRight_006e6930);    // 006E6F0F..1C
        struct Pick { Checkbox* box; void (__thiscall GameplayHandlers::*func)(kih::Control*); };
        const Pick picks[5] = {
            { &self->cb_levelup, &GameplayHandlers::OnAutoLevelUp_006e62a0 },      // 006E6F21..4F
            { &self->cb_invertcam, &GameplayHandlers::OnInvertCam_006e6330 },      // 006E6F54..82
            { &self->cb_autosave, &GameplayHandlers::OnAutoSave_006e62e0 },        // 006E6F87..B5
            { &self->cb_reverse, &GameplayHandlers::OnReverse_006e6380 },          // 006E6FBA..E8
            { &self->cb_disablemove, &GameplayHandlers::OnCombatMovement_006e63d0 },   // 006E6FED..701B
        };
        for (const Pick& p : picks) {
            add_event(p.box, 0x27, self, p.func);                                  // per box: 27h,
            add_event(p.box, 0x2D, self, p.func);                                  //          2Dh,
            add_event(p.box, 0, self, &GameplayHandlers::OnEnterSlot_006e68a0);    //          0 -> 006E68A0
        }
        self->btn_keymap.flags &= 0xFFFFFFFBu;                                     // 006E7020..3B
        add_event(&self->btn_keymap, 0x27, self, &GameplayHandlers::OnKeyMap_006e6420);   // 006E702C..3E
        self->btn_mouse.flags &= 0xFFFFFFFBu;                                      // 006E7043..59
        add_event(&self->btn_mouse, 0x27, self, &GameplayHandlers::OnMouse_006e64a0);     // 006E704F..5C
        self->btn_back.flags &= 0xFFFFFFFBu;                                       // 006E7061..6F
        gui_load_control_add_event_0041ab20(&self->btn_back, nullptr, 0x27, event_target(self),
            kotor_m1_shared::event_func(&kotor_m1_shared::PanelEventThunks::OnBButtonPressed_00624bb0));   // 006E7072..7A
        self->btn_default.flags &= 0xFFFFFFFBu;                                    // 006E707F..95
        add_event(&self->btn_default, 0x27, self, &GameplayHandlers::OnDefault_006e68b0); // 006E708B..98
        // Description strrefs in CSWGuiControl+58h (read by OnEnterSlot 006E68A4).
        self->cb_invertcam.u58 = 0xBB8Eu;                                          // 006E70A2 (+152C)
        self->cb_levelup.u58 = 0xA51Au;                                            // 006E70AC (+1278)
        self->cb_autosave.u58 = 0x9496u;                                           // 006E70B6 (+17E0)
        self->cb_reverse.u58 = 0xBC55u;                                            // 006E70C0 (+1A94)
        self->cb_disablemove.u58 = 0xA5F4u;                                        // 006E70CA (+1D48)
        self->btn_mouse.u58 = 0xBFAFu;                                             // 006E70D4 (+B68)
        self->btn_keymap.u58 = 0xBFB0u;                                            // 006E70DE (+9A4)
        gui_load_panel_set_active_control_0040a630(self, nullptr, &self->btn_difficulty, 0);   // 006E709D..E8
        kotor_m1_opt_gameplay_set_up_options_006e6770(self, nullptr);              // 006E70ED..EF
    } catch (...) {
        unwind_gameplay(self, built);
        throw;
    }
    return self;                                                                   // 006E70F9
}

// ======================================================================== 006E7130 dtor body (no vptr store)
void __fastcall kotor_m1_opt_gameplay_destruct_006e7130(Gameplay* self, void*) {
    gui_load_label_destruct_00419d10(&self->lbl_mousesen, nullptr);                // 006E714D..5B
    gui_m48_slider_destruct_0041bb40(&self->sli_mousesen, nullptr);                // 006E7160..6B
    gui_m48_button_toggle_destruct_0041cad0(&self->cb_disablemove, nullptr);       // 006E7170..7B
    gui_m48_button_toggle_destruct_0041cad0(&self->cb_reverse, nullptr);           // 006E7180..8B
    gui_m48_button_toggle_destruct_0041cad0(&self->cb_autosave, nullptr);          // 006E7190..9B
    gui_m48_button_toggle_destruct_0041cad0(&self->cb_invertcam, nullptr);         // 006E71A0..AB
    gui_m48_button_toggle_destruct_0041cad0(&self->cb_levelup, nullptr);           // 006E71B0..BB
    gui_load_button_destruct_00419da0(&self->btn_diffleft, nullptr);               // 006E71C0..CB
    gui_load_button_destruct_00419da0(&self->btn_diffright, nullptr);              // 006E71D0..DB
    gui_load_button_destruct_00419da0(&self->btn_difficulty, nullptr);             // 006E71E0..EB
    gui_load_button_destruct_00419da0(&self->btn_mouse, nullptr);                  // 006E71F0..FB
    gui_load_button_destruct_00419da0(&self->btn_keymap, nullptr);                 // 006E7200..0B
    gui_load_button_destruct_00419da0(&self->btn_default, nullptr);                // 006E7210..1B
    gui_load_button_destruct_00419da0(&self->btn_back, nullptr);                   // 006E7220..2B
    gui_load_listbox_destruct_0041b060(&self->lb_desc, nullptr);                   // 006E7230..3B
    gui_load_label_destruct_00419d10(&self->lbl_title, nullptr);                   // 006E7240..4B
    gui_load_label_destruct_00419d10(&self->desc_item, nullptr);                   // 006E7250..58
    gui_load_panel_destruct_0040cf70(self, nullptr);                               // 006E725D..67
}
// ======================================================================== 006E7110 scalar deleting dtor (RET 4)
void* __fastcall kotor_m1_opt_gameplay_deleting_destruct_006e7110(Object* object, void*, Word flags) {
    Gameplay* const self = static_cast<Gameplay*>(static_cast<Panel*>(object));
    kotor_m1_opt_gameplay_destruct_006e7130(self, nullptr);                        // 006E7113
    if (flags & 1u) live_free(self);                                               // 006E7118..25 (_free 006FA390)
    return self;                                                                   // 006E7128
}
// ======================================================================== 006E6170 OnPanelAdded: JMP 0040B870
void __fastcall kotor_m1_opt_gameplay_on_panel_added_006e6170(Panel* self, void*) {
    gui_load_panel_on_added_0040b870(self, nullptr);                               // 006E6170
}

// ======================================================================== 006E6180 HandleInputEvent(event, pressed)
// thiscall RET 8; slot +3C of 00758E00 and 007585F8 (both panels: LB_DESC at +2E4). Jump table 006E6274
// {006E61AF, 006E61E2, 006E622E, 006E6250, 006E6263} indexed by the bytes at 006E6288 for event-28h in 0..12h:
// 00 04 04 04 04 01 00 04 04 04 04 04 04 04 04 04 04 02 03 (read from the image) -> 28h/2Eh Back, 2Dh toggle,
// 39h/3Ah list scroll, others default. Every path ends in CSWGuiPanel::HandleInputEvent 00409E60.
void __fastcall kotor_m1_opt_gameplay_handle_input_006e6180(Object* object, void*, I32 event, I32 pressed) {
    Panel* const self = static_cast<Panel*>(object);
    if (pressed != 0) {                                                            // 006E6187..8F
        switch (event) {                                                           // 006E6195..A8
        case 0x28: case 0x2E:                                                      // 006E61AF Back
            manager_of(self)->PlayGuiSound(0);                                     // 006E61AF..B4
            kotor_fastcall_0040be00(self->manager, nullptr);                       // 006E61B9..BC PopModalPanel
            self->flags = (self->flags & 0xFFFFFCFFu) | 0x400u;                    // 006E61C1..D1
            break;
        case 0x2D:                                                                 // 006E61E2 toggle the active checkbox
            if (self->active && as_toggle(self->active)) {                         // 006E61E2..F0
                manager_of(self)->PlayGuiSound(3);                                 // 006E61F2..F7
                const Word selected = ~as_toggle(self->active)->toggle_flags & 1u; // 006E61FC..11
                kotor_m1_opt_toggle_set_selected_00685540(as_toggle(self->active), nullptr, selected);   // 006E6214..1A
            }
            break;
        case 0x39: {                                                               // 006E622E LB_DESC vslot +3C(31h, 1)
            Object* const lb = lb_desc_of(self);
            vt<InputView>(lb)->handle_input_event(lb, nullptr, 0x31, 1);
            break;
        }
        case 0x3A: {                                                               // 006E6250 LB_DESC vslot +3C(32h, 1)
            Object* const lb = lb_desc_of(self);
            vt<InputView>(lb)->handle_input_event(lb, nullptr, 0x32, 1);
            break;
        }
        default:
            break;
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, pressed);          // 006E61D7/6223/6245/6267
}

// ======================================================================== 006E6770 SetUpOptions (thiscall, RET)
// options+4 byte -> difficulty (+229C); checkbox selection bits from options+8 bits 0..4.
void __fastcall kotor_m1_opt_gameplay_set_up_options_006e6770(Gameplay* self, void*) {
    void* const options = client_options();                                        // 006E6770..7C
    if (!options) return;                                                          // 006E6783..85
    self->difficulty = byte_at(options, 4);                                        // 006E678B..8F (movzx)
    kotor_m1_opt_gameplay_set_difficulty_text_006e6520(self, nullptr);             // 006E6796..98
    Checkbox* const boxes[5] = { &self->cb_levelup, &self->cb_invertcam, &self->cb_autosave,
                                 &self->cb_reverse, &self->cb_disablemove };       // 006E679D / 7CB / 7FB / 82C / 85D
    for (unsigned bit = 0; bit < 5; ++bit) {
        const Word value = (word_at(options, 8) >> bit) & 1u;                      // [edi+8] >> bit & 1
        ButtonToggle* const t = as_toggle(boxes[bit]);                             // vslot +5C
        t->toggle_flags = ((t->toggle_flags ^ value) & 1u) ^ t->toggle_flags;      // +1C8 bit0
    }
}
// ======================================================================== 006E6520 SetDifficultyText (thiscall, RET)
// C2DA::GetINTEntry(difficulty, "NAME", &strref) on CSWRules+B8+7C; on success SetStrRef on the difficulty
// button text (+E40); arrows enabled (flags |= 2) then disabled at the ends (0 -> left; rows-2 -> right).
void __fastcall kotor_m1_opt_gameplay_set_difficulty_text_006e6520(Gameplay* self, void*) noexcept(false) {
    std::int32_t strref;                                                           // [esp+8]: written by 00413660 on success
    std::int32_t found;
    {
        String column("NAME");                                                     // 006E653C..45 (00748E9C)
        found = c2da_current_get_int_00413660(difficulty_table(), nullptr, self->difficulty, &column, &strref);   // 006E654A..73
    }                                                                              // 006E6578..86 (~String)
    if (found != 0)                                                                // 006E658B..8D
        gui_load_text_params_set_strref_00415e50(reinterpret_cast<unsigned char*>(self) + 0xE40, nullptr,
                                                 static_cast<Word>(strref));       // 006E658F..9A
    self->btn_diffleft.flags |= 2u;                                                // 006E659F..AC
    self->btn_diffright.flags |= 2u;                                               // 006E65B2
    if (self->difficulty == 0) {                                                   // 006E65B8..C0
        self->btn_diffleft.flags &= 0xFFFFFFFDu;                                   // 006E65C2..CC
        return;
    }
    const I32 rows = static_cast<I32>(word_at(difficulty_table(), 0x24));          // 006E65E2..F1
    if (self->difficulty == rows - 2)                                              // 006E65F4..F8
        self->btn_diffright.flags &= 0xFFFFFFFDu;                                  // 006E65FA
}
// ======================================================================== 006E6620 SetDescription(strref) (RET 4)
// Same shape as CSWGuiOptionsMain::SetDescription 006DE340 with this panel's offsets (desc +64, LB +2E4).
void __fastcall kotor_m1_opt_gameplay_set_description_006e6620(Gameplay* self, void*, Word strref) noexcept(false) {
    unsigned char* const params = reinterpret_cast<unsigned char*>(self) + 0x14C;  // desc_item text params (+D0+18)
    gui_load_text_params_set_strref_00415e50(params, nullptr, strref);             // 006E6640..49
    String text(*reinterpret_cast<const String*>(params));                         // 006E664E..53 (005E5B00); EH state 0
    gui_load_listbox_clear_items_00419a90(&self->lb_desc, nullptr);                // 006E6658..68
    Extent extent = self->desc_item.extent;                                        // 006E666D..8B (+68)
    gui_load::Text* const text_object = &self->desc_item.text;                     // 006E6696 (+134)
    if (text.BorrowedLength005e5790() > 0)                                         // 006E668F..9C
        extent.height = static_cast<I32>(vt<TextIdealView>(text_object)->ideal_height(text_object, nullptr));   // 006E669E..A0
    else
        extent.height = gui_load_text_font_height_00414ee0(text_object, nullptr);  // 006E66A5
    Label* const item = &self->desc_item;                                          // 006E66AE
    item->vtbl->set_extent(item, nullptr, &extent);                                // 006E66B5..BA vslot +04
    Control* items[1] = { item };                                                  // 006E66CC (argument slot reused)
    gui_m48_listbox_add_controls_0041c2c0(&self->lb_desc, nullptr, items, 1, 0, 0, 0);   // 006E66BD..D0
    gui_load_listbox_set_active_control_0041c160(&self->lb_desc, nullptr, item, 0);       // 006E66D5..DA
}                                                                                  // 006E66DF..EB ~String

// ======================================================================== CClientOptions
// ---- 0061B740 SetAutoLevelUpNPCs(int) (thiscall RET 4): options+8 bit0 = value; when the new bit is set and the
// server app exists ([[007A39FC]+8]), CSWPartyTable::AutoLevelUpNPCs(GetPartyTable()). At the main menu the server
// is 0 (m49 TRAPS.json entry 00563B9C gate: CAppManager+8 written only by CreateServer 00401406), so only the bit changes.
void __fastcall kotor_m1_opt_client_options_set_auto_level_up_npcs_0061b740(void* options, void*, Word value) {
    const Word flags = word_at(options, 8);                                        // 0061B740 / 0061B747
    const Word updated = ((flags ^ value) & 1u) ^ flags;                           // 0061B743..4D
    set_word(options, 8, updated);                                                 // 0061B753
    if ((updated & 1u) == 0) return;                                               // 0061B74F..56
    void* const server = server_app();                                             // 0061B758..5E
    if (!server) return;                                                           // 0061B761..63
    lane_config_required::AutoLevelUpNPCs_00563b90(
        lane_config_required::GetPartyTable_004aee70(server, nullptr), nullptr);  // 0061B765..6C
}                                                                                  // 0061B771 RET 4
// ---- 0061D4E0 SetDefaultGameplayOptions (thiscall RET): difficulty byte = 1; flags = (flags & ~0Bh) | 14h.
void __fastcall kotor_m1_opt_client_options_set_default_gameplay_0061d4e0(void* options, void*) {
    const Word flags = (word_at(options, 8) & 0xFFFFFFF4u) | 0x14u;                // 0061D4E0..E6
    set_byte(options, 4, 1);                                                       // 0061D4E9
    set_word(options, 8, flags);                                                   // 0061D4ED
}
}

// ======================================================================== event callbacks (thiscall, RET 4)
namespace {
// Shared shape of OnAutoSave/OnInvertCam/OnReverse/OnCombatMovement: if [control+4Ch]: v = ~(options+8 >> bit) & 1
// (first GetClientOptions call), options+8 bit = v (second GetClientOptions call).
void toggle_option_bit(const kih::Control* control, unsigned bit) {
    if (control_value(control) == 0) return;
    const Word value = ~(word_at(client_options(), 8) >> bit) & 1u;
    void* const options = client_options();
    const Word flags = word_at(options, 8);
    set_word(options, 8, (((value << bit) ^ flags) & (1u << bit)) ^ flags);
}
}
// ---- 006E62A0 OnAutoLevelUp: SetAutoLevelUpNPCs(~options+8 & 1) (006E62A4..D2).
void __thiscall GameplayHandlers::OnAutoLevelUp_006e62a0(kih::Control* control) {
    if (control_value(control) == 0) return;                                       // 006E62A4..A9
    const Word value = ~word_at(client_options(), 8) & 1u;                         // 006E62AB..C7
    kotor_m1_opt_client_options_set_auto_level_up_npcs_0061b740(client_options(), nullptr, value);   // 006E62BC..D2
}
void __thiscall GameplayHandlers::OnAutoSave_006e62e0(kih::Control* control) { toggle_option_bit(control, 2); }       // 006E62E0..24 (bit 2)
void __thiscall GameplayHandlers::OnInvertCam_006e6330(kih::Control* control) { toggle_option_bit(control, 1); }      // 006E6330..6F (bit 1)
void __thiscall GameplayHandlers::OnReverse_006e6380(kih::Control* control) { toggle_option_bit(control, 3); }        // 006E6380..C4 (bit 3)
void __thiscall GameplayHandlers::OnCombatMovement_006e63d0(kih::Control* control) { toggle_option_bit(control, 4); } // 006E63D0..10 (bit 4)
// ---- 006E6420 OnKeyMap: new(0F34h) CSWGuiInGameOptKeyMappings 006EDC90 + AddPanel(p, 3, 1) (006E6443..7F).
void __thiscall GameplayHandlers::OnKeyMap_006e6420(kih::Control* control) {
    if (control_value(control) == 0) return;                                       // 006E6439..41
    open_sub_panel(gameplay_of(this), 0xF34, &kotor_m1_opt_keymap_construct_006edc90);
}
// ---- 006E64A0 OnMouse: new(0EF8h) CSWGuiOptionsMouse 006E2600 + AddPanel(p, 3, 1) (006E64C3..FF).
void __thiscall GameplayHandlers::OnMouse_006e64a0(kih::Control* control) {
    if (control_value(control) == 0) return;                                       // 006E64B9..C1
    open_sub_panel(gameplay_of(this), 0xEF8, &kotor_m1_opt_mouse_construct_006e2600);
}
// ---- 006E6710 OnEnterDiff: difficulty text colour 007A23C0 + flags |= 40h, SetDifficultyText, SetDescription(A519h).
void __thiscall GameplayHandlers::OnEnterDiff_006e6710(kih::Control*) {
    Gameplay* const self = gameplay_of(this);
    unsigned char* const params = reinterpret_cast<unsigned char*>(self) + 0xE40;  // btn_difficulty text params
    gui_load_text_params_set_color_00414e10(params, nullptr, &borrowed_enter_color_007a23c0);   // 006E6714..21
    set_word(params, 0x38, word_at(params, 0x38) | 0x40u);                         // 006E6726
    kotor_m1_opt_gameplay_set_difficulty_text_006e6520(self, nullptr);             // 006E672A..2C
    kotor_m1_opt_gameplay_set_description_006e6620(self, nullptr, 0xA519u);        // 006E6731..38
}
// ---- 006E6750 OnExitDiff: colour 007A23B4, flags &= ~40h.
void __thiscall GameplayHandlers::OnExitDiff_006e6750(kih::Control*) {
    unsigned char* const params = reinterpret_cast<unsigned char*>(gameplay_of(this)) + 0xE40;
    gui_load_text_params_set_color_00414e10(params, nullptr, &borrowed_exit_color_007a23b4);    // 006E6751..5E
    set_word(params, 0x38, word_at(params, 0x38) & 0xFFFFFFBFu);                   // 006E6763
}
// ---- 006E68A0 OnEnterSlot: replace the stack word with [control+58h]; JMP 006E6620.
void __thiscall GameplayHandlers::OnEnterSlot_006e68a0(kih::Control* control) {
    kotor_m1_opt_gameplay_set_description_006e6620(gameplay_of(this), nullptr, word_at(control, 0x58));   // 006E68A0..AB
}
// ---- 006E68B0 OnDefault: SetDefaultGameplayOptions + SetUpOptions.
void __thiscall GameplayHandlers::OnDefault_006e68b0(kih::Control* control) {
    if (control_value(control) == 0) return;                                       // 006E68B7..BC
    kotor_m1_opt_client_options_set_default_gameplay_0061d4e0(client_options(), nullptr);   // 006E68BE..CE
    kotor_m1_opt_gameplay_set_up_options_006e6770(gameplay_of(this), nullptr);     // 006E68D3..D5
}
// ---- 006E68E0 OnDifficultyLeft: if difficulty > 0: --difficulty; options+4 byte; text; PlayGuiSound(1).
void __thiscall GameplayHandlers::OnDifficultyLeft_006e68e0(kih::Control* control) {
    Gameplay* const self = gameplay_of(this);
    if (control_value(control) == 0) return;                                       // 006E68E7..EC
    if (self->difficulty <= 0) return;                                             // 006E68EE..F6
    const I32 value = self->difficulty - 1;                                        // 006E68F8
    self->difficulty = value;                                                      // 006E68F9
    set_byte(client_options(), 4, static_cast<unsigned char>(value));              // 006E68FF..12
    kotor_m1_opt_gameplay_set_difficulty_text_006e6520(self, nullptr);             // 006E6910..15
    manager_of(self)->PlayGuiSound(1);                                             // 006E691A..1F
}
// ---- 006E6930 OnDifficultyRight: unless difficulty == rows-2: ++difficulty; options+4 byte; text; PlayGuiSound(1).
void __thiscall GameplayHandlers::OnDifficultyRight_006e6930(kih::Control* control) {
    Gameplay* const self = gameplay_of(this);
    if (control_value(control) == 0) return;                                       // 006E6937..3C
    const I32 rows = static_cast<I32>(word_at(difficulty_table(), 0x24));          // 006E693E..4D
    if (self->difficulty == rows - 2) return;                                      // 006E6950..5B
    self->difficulty = self->difficulty + 1;                                       // 006E695D..5E
    set_byte(client_options(), 4, static_cast<unsigned char>(self->difficulty));   // 006E6964..7A
    kotor_m1_opt_gameplay_set_difficulty_text_006e6520(self, nullptr);             // 006E6978..7D
    manager_of(self)->PlayGuiSound(1);                                             // 006E6982..87
}
