// lane-m15-d / unit m15d-skills: CSWGuiSkillsCharGen 006F47E0..006F6A10 (0x49D0; method table 00759990), opened by
// CSWGuiCustomPanel::OnSelectSkillsButton 006EF360 (L15-C), and the generic minus/plus button thunks 0067CB40 /
// 0067CB50 (also registered by the Abilities panel here and by CSWGuiPortraitCharGen in L15-C).
// Level-up parent branches (flags bit0; only CSWGuiLevelUpPanel passes level_up = 1, in-game) call the L15-F D83
// function traps 006EE580 / 006EE5D0; the char-gen parent is CSWGuiCustomPanel (L15-C 006EF480 / 006EF520).
// Original: swkotor.exe GOG K1, static disassembly only; every behaviour cites its instruction addresses.
#include "88764680e4-m15d_common.h"

using namespace kotor_m15d;

extern "C" {
extern const MenuColor borrowed_exit_color_007a23b4;      // input-cells 007A23B4
extern const MenuColor borrowed_enter_color_007a23c0;     // input-cells 007A23C0
extern const MenuColor kotor_m1_color_007a2408;           // m1-saveload 007A2408 {0, 0.33, 0.49}
void* __fastcall kotor_m15d_skills_deleting_destruct_006f4bd0(SkillsPanel* self, void*, Word flags);
void __fastcall kotor_m15d_skills_destruct_006f47e0(SkillsPanel* self, void*);
void __fastcall kotor_m15d_skills_on_panel_added_006f49a0(SkillsPanel* self, void*) noexcept(false);
void __fastcall kotor_m15d_skills_handle_cancel_006f49d0(SkillsPanel* self, void*) noexcept(false);
void __fastcall kotor_m15d_skills_set_description_006f4ab0(SkillsPanel* self, void*, StartupString* text) noexcept(false);
Word __fastcall kotor_m15d_skills_is_class_skill_006f4b60(SkillsPanel* self, void*, Word skill);
void __fastcall kotor_m15d_skills_on_recommend_006f4e20(SkillsPanel* self, void*) noexcept(false);
void __fastcall kotor_m15d_skills_on_minus_006f6370(SkillsPanel* self, void*) noexcept(false);
void __fastcall kotor_m15d_skills_on_plus_006f6570(SkillsPanel* self, void*) noexcept(false);
void __fastcall kotor_m15d_skills_on_accept_006f6980(SkillsPanel* self, void*) noexcept(false);
void __fastcall kotor_m15d_skills_handle_input_006f6a10(SkillsPanel* self, void*, I32 event, I32 value) noexcept(false);
}

namespace {
constexpr U16 kRaceWord_0074bf10 = 5;                     // .rdata word 0074BF10 (image value 5; compared at 006F541F)
SkillsPanel* skills_of(SkillsHandlers* h) { return reinterpret_cast<SkillsPanel*>(h); }
gui_load::EventFunc handler(void (__thiscall SkillsHandlers::*f)(kih::Control*)) { return static_cast<gui_load::EventFunc>(f); }
// Element addresses of the per-skill arrays computed as the original does (base + i * size, no bound: the loops run
// to the rules skill count [rules+ABh]).
Label* skill_label(SkillsPanel* s, unsigned i) { return reinterpret_cast<Label*>(at(s, 0x0FCCu + 0x140u * i)); }
Button* point_button(SkillsPanel* s, unsigned i) { return reinterpret_cast<Button*>(at(s, 0x19CCu + 0x1C4u * i)); }
Button* plus_button(SkillsPanel* s, unsigned i) { return reinterpret_cast<Button*>(at(s, 0x2D38u + 0x1C4u * i)); }
Button* minus_button(SkillsPanel* s, unsigned i) { return reinterpret_cast<Button*>(at(s, 0x3B58u + 0x1C4u * i)); }
U8& base_rank(SkillsPanel* s, unsigned i) { return *at(s, 0x49C0u + i); }
// Inline skill-rank read (skill < BYTE [rules+ABh] ? stats->ranks[+88h][skill] : 0), e.g. 006F6402..2B.
U8 rank_of(void* stats, unsigned skill) {
    if (static_cast<U16>(skill) >= static_cast<U16>(ld<U8>(rules(), 0xAB))) return 0;
    return ld<U8>(rp(stats, 0x88), static_cast<U16>(skill));
}
// Control flag bit1 (enabled look) := value (xor/and/xor sequence, e.g. 006F612C..3E).
void set_bit1(Button* b, bool value) { b->flags = ((b->flags ^ (value ? 2u : 0u)) & 2u) ^ b->flags; }
// Skill unusable look (006F6215..37 / 006F50F5..16): !(skill flags & 10h) or (droid and !(skill flags & 20h)).
bool greyed(Word panel_flags, unsigned skill) {
    const Word skill_flags = rd32(rp(rules(), 0xB4), (skill << 5) + 0x1C);         // [[rules+B4]+skill*20h+1Ch]
    if ((skill_flags & 0x10u) == 0) return true;
    return (panel_flags & 4u) != 0 && (skill_flags & 0x20u) == 0;
}
void show_greyed(SkillsPanel* self, unsigned skill) {                              // 006F6239..57 / 006F5118..3F
    gui_load_text_params_set_color_00414e10(at(skill_label(self, skill), kLabelText), nullptr, &kotor_m1_color_007a2408);
    gui_load_text_params_set_color_00414e10(at(point_button(self, skill), kButtonText), nullptr, &kotor_m1_color_007a2408);
    plus_button(self, skill)->flags &= ~2u;
}
void close_to_parent(SkillsPanel* self) {                                          // 006F4A43..65 / 006F4DDC..08 / 006F69D5..F7
    kotor_fastcall_0040be00(self->manager, nullptr);                               // PopModalPanel 0040BE00
    kotor_m15_panel_set_visible_0040b9d0(self->parent, nullptr, 1);                // SetVisible 0040B9D0
    self->flags = (self->flags & 0xFFFFFCFFu) | 0x400u;                            // [this+44h]
}
void set_active0(Button* b) { vt<ActiveView>(static_cast<Object*>(b))->set_active(b, nullptr, 0); }   // vslot +40 (0)
// Member construction stages of 006F51D0 (EH states 0..13h of FuncInfo 00739DF5).
Label* label_at(SkillsPanel* s, int i) { return reinterpret_cast<Label*>(at(s, 0x34Cu + 0x140u * static_cast<unsigned>(i))); }   // 10 labels
template <class T, class C, class D> void vector_construct(T* first, int count, C construct, D destroy) {
    for (int i = 0; i < count; ++i) {                                              // eh_vector_constructor_iterator 006FB55F
        try { construct(first + i, nullptr); }
        catch (...) { while (i-- > 0) destroy(first + i, nullptr); throw; }
    }
}
template <class T, class D> void vector_destroy(T* first, int count, D destroy) {
    for (int i = count; i-- > 0;) destroy(first + i, nullptr);                     // eh_vector_destructor_iterator 006FA852
}
void string_ctor(StartupString* s, void*) { ::new (static_cast<void*>(s)) StartupString(); }   // 005B3190
void string_dtor(StartupString* s, void*) { s->~StartupString(); }                // 005E5C20
void unwind_skills(SkillsPanel* self, int built) {
    if (built >= 0x13) vector_destroy(self->descriptions, 8, string_dtor);
    if (built >= 0x12) vector_destroy(self->minus_buttons, 8, gui_load_button_destruct_00419da0);
    if (built >= 0x11) vector_destroy(self->plus_buttons, 8, gui_load_button_destruct_00419da0);
    if (built >= 0x10) gui_load_button_destruct_00419da0(&self->btn_recommended, nullptr);
    if (built >= 0x0F) gui_load_button_destruct_00419da0(&self->btn_back, nullptr);
    if (built >= 0x0E) gui_load_button_destruct_00419da0(&self->btn_accept, nullptr);
    if (built >= 0x0D) vector_destroy(self->point_buttons, 8, gui_load_button_destruct_00419da0);
    if (built >= 0x0C) vector_destroy(self->skill_labels, 8, gui_load_label_destruct_00419d10);
    for (int i = 10; i-- > 0;) if (built >= 2 + i) gui_load_label_destruct_00419d10(label_at(self, i), nullptr);
    if (built >= 1) gui_load_listbox_destruct_0041b060(&self->lb_desc, nullptr);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
// Message box with an assembled CExoString (006F691B..54): SetAllowCancel(0), SetCallback(0,0,0), vslot +70 with a
// by-value copy (copy constructor 005E5B00 into the argument area; the callee owns it), AddPanel(mb, 1, 1).
void show_message_text(SkillsPanel* self, void* box, StartupString* text) {
    kotor_fastcall_00627130(box, nullptr, 0);                                      // 006F691B..1F
    gui_load_messagebox_set_callback_00624a40(box, nullptr, nullptr, nullptr, 0);  // 006F6924..2C
    StringBits copy;                                                               // 006F6931 (sub esp,8)
    WindowCopy005e5b00(reinterpret_cast<String*>(&copy), nullptr, text);           // 006F6934..3F (005E5B00)
    vt<MessageBoxView>(box)->set_message(box, nullptr, copy);                      // 006F6944..49 vslot +70
    kotor_fastcall_0040bc70(self->manager, nullptr, box, 1, 1);                    // 006F694C..54
}
}

// ======================================================================== 0067CB40 / 0067CB50 (thiscall, RET 4)
void __thiscall PanelStepThunks::OnMinusButtonPushed_0067cb40(kih::Control*) {
    vt<InputView>(this)->handle_input_event(this, nullptr, 0x2F, 1);               // 0067CB40..49 vslot +3C(2Fh, 1)
}
void __thiscall PanelStepThunks::OnPlusButtonPushed_0067cb50(kih::Control*) {
    vt<InputView>(this)->handle_input_event(this, nullptr, 0x30, 1);               // 0067CB50..59 vslot +3C(30h, 1)
}

// ======================================================================== D21 method table 00759990 (27 slots)
// Read from .rdata 00759990..007599FB: CSWGuiPanel 0073E010 except +00 006F4BD0, +3C 006F6A10, +48 006F49A0.
struct GuiM15dSkillsMethods {
    void* (__fastcall* s00)(SkillsPanel*, void*, Word);                       // +00 006F4BD0 deleting dtor
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
    void (__fastcall* s34)(Panel*, void*, Word);                              // +34 0040B760 CSWGuiPanel::Draw
    void (__fastcall* s38)(Object*, void*, Word);                             // +38 0060E760
    void (__fastcall* s3c)(SkillsPanel*, void*, I32, I32);                    // +3C 006F6A10 HandleInputEvent
    Object* (__fastcall* s40)(Object*, void*, I32, I32);                      // +40 0040B690
    Object* (__fastcall* s44)(Object*, void*);                                // +44 0040B630
    void (__fastcall* s48)(SkillsPanel*, void*);                              // +48 006F49A0 OnPanelAdded
    void (__fastcall* s4c)(Panel*, void*);                                    // +4C 0040C170
    void (__fastcall* s50)(Object*, void*, Word);                             // +50 0040B640
    void (__fastcall* s54)(Object*, void*, Word);                             // +54 0040B650
    void (__fastcall* s58)(Object*, void*, Word);                             // +58 0040B660
    void (__fastcall* s5c)(Object*, void*, Word);                             // +5C 0040B670
    void (__fastcall* s60)(Object*, void*, Word);                             // +60 0040B680
    String* (__fastcall* s64)(Panel*, void*, String*);                        // +64 0040A900
    void (__fastcall* s68)(Panel*, void*);                                    // +68 0040A9C0
};
static_assert(sizeof(GuiM15dSkillsMethods) == 0x6C, "00759990 slot count (27)");
extern "C" const GuiM15dSkillsMethods gui_m15d_skills_methods_00759990 = {
    &kotor_m15d_skills_deleting_destruct_006f4bd0,  // +00
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
    &kotor_m15d_skills_handle_input_006f6a10,       // +3C
    &gui_load_panel_hit_check_0040b690,             // +40
    &gui_load_panel_active_control_0040b630,        // +44
    &kotor_m15d_skills_on_panel_added_006f49a0,     // +48
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
// 006F4A80 OnExitPointsButton(control): clear text flag 40h of the skill's label and points button
// (index = [control+58h]).
void __thiscall SkillsHandlers::OnExitPointsButton_006f4a80(kih::Control* control) {
    SkillsPanel* const self = skills_of(this);
    const Word i = rd32(control, kControlId);                                      // 006F4A80..84
    unsigned char* const label_flags = at(skill_label(self, i), kLabelText + kTextFlags);   // 006F4A87..94 (+10B4+38)
    st<Word>(label_flags, 0, ld<Word>(label_flags, 0) & 0xFFFFFFBFu);
    const Word j = rd32(control, kControlId);                                      // 006F4A98 (re-read)
    unsigned char* const button_flags = at(point_button(self, j), kButtonText + kTextFlags);   // 006F4A9B..A8 (+1B38+38)
    st<Word>(button_flags, 0, ld<Word>(button_flags, 0) & 0xFFFFFFBFu);
}
// 006F4BF0 OnEnterPointsButton(control): highlight the entered skill (007A23C0, or 007A2408 when unusable), the
// others normal (007A23B4 / 007A2408); class-skill label, cost, description; text flag 40h on the entered skill.
void __thiscall SkillsHandlers::OnEnterPointsButton_006f4bf0(kih::Control* control) {
    SkillsPanel* const self = skills_of(this);
    for (unsigned i = 0; i < 8; ++i) {                                             // 006F4C14..D5
        const Word flags = self->skill_flags;                                      // 006F4C20
        const bool grey = (flags & 2u) == 0 && greyed(flags, i);                   // 006F4C26..54
        void* const button_text = at(point_button(self, i), kButtonText);          // edi (+1B38)
        void* const label_text = at(skill_label(self, i), kLabelText);             // ebp (+10B4)
        const MenuColor* color;
        if (reinterpret_cast<void*>(control) == point_button(self, i)) {           // 006F4C58..64 (edi-16Ch)
            self->current = static_cast<I32>(i);                                   // 006F4C68
            color = grey ? &kotor_m1_color_007a2408 : &borrowed_enter_color_007a23c0;   // 006F4C70..92
        } else {
            color = grey ? &kotor_m1_color_007a2408 : &borrowed_exit_color_007a23b4;    // 006F4C94..B5
        }
        gui_load_text_params_set_color_00414e10(button_text, nullptr, color);
        gui_load_text_params_set_color_00414e10(label_text, nullptr, color);       // 006F4CBA..BC
    }
    const Word is_class = kotor_m15d_skills_is_class_skill_006f4b60(self, nullptr, static_cast<U16>(self->current));   // 006F4CDB..E7
    gui_load_text_params_set_strref_00415e50(at(&self->lbl_class_skill, kLabelText), nullptr,
                                             is_class != 0 ? 0x9508u : 0x9509u);   // 006F4CEC..4D02 (+E34)
    const std::int32_t cost = kotor_m15b_levelup_stats_get_skill_point_cost_00647900(stats_of(self->creature), nullptr,
        static_cast<U16>(self->current), 1);                                       // 006F4D07..1C
    set_text_int(at(&self->lbl_cost_points, kLabelText), cost);                    // 006F4D21..3E (+CF4)
    kotor_m15d_skills_set_description_006f4ab0(self, nullptr, &self->descriptions[self->current]);   // 006F4D43..53
    unsigned char* const label_flags = at(skill_label(self, static_cast<unsigned>(self->current)), kLabelText + kTextFlags);
    st<Word>(label_flags, 0, ld<Word>(label_flags, 0) | 0x40u);                    // 006F4D58..79
    unsigned char* const button_flags = at(point_button(self, static_cast<unsigned>(self->current)), kButtonText + kTextFlags);
    st<Word>(button_flags, 0, ld<Word>(button_flags, 0) | 0x40u);                  // 006F4D7C..98
}                                                                                  // 006F4D94..A3 (005E5C20)
// 006F4DC0 ConfirmAccept (message-box callback): when the box was accepted ([mb+64h] bit0) close and notify.
void __thiscall SkillsHandlers::ConfirmAccept_006f4dc0(kih::Control*) {
    SkillsPanel* const self = skills_of(this);
    void* const box = message_box();                                               // 006F4DC0..D0
    if ((ld<U8>(box, 0x64) & 1u) == 0) return;                                     // 006F4DD6..DA
    close_to_parent(self);                                                         // 006F4DDC..08
    if ((self->skill_flags & 1u) != 0) kotor_fastcall_006ee5d0(self->parent, nullptr);   // 006F4E0B..0D (L15-F trap)
    else kotor_m15c_custom_skill_creation_completed_006ef520(self->parent, nullptr);      // 006F4E16
}

extern "C" {
// ======================================================================== 006F47E0 dtor (RET)
void __fastcall kotor_m15d_skills_destruct_006f47e0(SkillsPanel* self, void*) {
    gui_lb::set_vptr(self, &gui_m15d_skills_methods_00759990);                    // 006F47FE (759990)
    gui_load_listbox_clear_items_00419a90(&self->lb_desc, nullptr);                // 006F4804..11
    vector_destroy(self->descriptions, 8, string_dtor);                            // 006F4816..2B
    vector_destroy(self->minus_buttons, 8, gui_load_button_destruct_00419da0);    // 006F4830..48
    vector_destroy(self->plus_buttons, 8, gui_load_button_destruct_00419da0);     // 006F484D..65
    gui_load_button_destruct_00419da0(&self->btn_recommended, nullptr);            // 006F486A..75
    gui_load_button_destruct_00419da0(&self->btn_back, nullptr);                   // 006F487A..85
    gui_load_button_destruct_00419da0(&self->btn_accept, nullptr);                 // 006F488A..95
    vector_destroy(self->point_buttons, 8, gui_load_button_destruct_00419da0);    // 006F489A..B2
    vector_destroy(self->skill_labels, 8, gui_load_label_destruct_00419d10);      // 006F48B7..CF
    for (int i = 10; i-- > 0;) gui_load_label_destruct_00419d10(label_at(self, i), nullptr);   // 006F48D4..6F
    gui_load_listbox_destruct_0041b060(&self->lb_desc, nullptr);                   // 006F4974..7B
    gui_load_panel_destruct_0040cf70(self, nullptr);                               // 006F4980..8A
}
// 006F4BD0 scalar deleting destructor (RET 4).
void* __fastcall kotor_m15d_skills_deleting_destruct_006f4bd0(SkillsPanel* self, void*, Word flags) {
    kotor_m15d_skills_destruct_006f47e0(self, nullptr);                            // 006F4BD3
    if (flags & 1u) live_free(self);                                               // 006F4BD8..E5
    return self;                                                                   // 006F4BE8
}
// ======================================================================== 006F49A0 OnPanelAdded (RET)
void __fastcall kotor_m15d_skills_on_panel_added_006f49a0(SkillsPanel* self, void*) noexcept(false) {
    gui_load_panel_on_added_0040b870(self, nullptr);                               // 006F49A0
    show_tutorial_window_005edf40(client_app(), nullptr, 0x11, 0x7F000000u, 0x7F000000u, 0);   // 006F49A5..BB
}
// ======================================================================== 006F49D0 HandleCancelButton (RET)
void __fastcall kotor_m15d_skills_handle_cancel_006f49d0(SkillsPanel* self, void*) noexcept(false) {
    if (self->parent == nullptr) return;                                           // 006F49D3..D8
    if ((self->skill_flags & 1u) == 0) {                                           // 006F49DE..E5: undo the char-gen ranks
        for (unsigned i = 0; i < 8; ++i) {                                         // 006F49E8..2E
            kotor_m15b_levelup_stats_set_skill_rank_base_006488e0(stats_of(self->creature), nullptr, static_cast<U16>(i), 0);   // 006F49F0..FC
            if (static_cast<U16>(i) < static_cast<U16>(ld<U8>(rules(), 0xAB)))     // 006F4A01..12
                st<U8>(rp(stats_of(self->creature), 0x84), i, 0);                  // 006F4A14..26
        }
        st<U16>(stats_of(self->creature), 0xAE, 0);                                // 006F4A30..39
    }
    close_to_parent(self);                                                         // 006F4A43..65
    if ((self->skill_flags & 1u) != 0) kotor_fastcall_006ee580(self->parent, nullptr);   // 006F4A58..72 (L15-F trap)
    else kotor_m15c_custom_cancelled_by_child_006ef480(self->parent, nullptr);     // 006F4A77
}
// ======================================================================== 006F4AB0 SetDescription(const CExoString&) (RET 4)
void __fastcall kotor_m15d_skills_set_description_006f4ab0(SkillsPanel* self, void*, StartupString* text) noexcept(false) {
    gui_load_text_params_set_text_00415e00(at(&self->desc_item, kLabelText), nullptr, text);   // 006F4ABC..C3 (+F74)
    gui_load_listbox_clear_items_00419a90(&self->lb_desc, nullptr);                // 006F4AC8..CD
    Extent extent = self->desc_item.extent;                                        // 006F4AD2..F1 (+E90)
    gui_load::Text* const text_object = &self->desc_item.text;                     // 006F4AFC (+F5C)
    if (text->BorrowedLength005e5790() > 0)                                        // 006F4AEF..FA (005E5790)
        extent.height = static_cast<I32>(vt<TextIdealView>(text_object)->ideal_height(text_object, nullptr));   // 006F4B04..06
    else
        extent.height = gui_load_text_font_height_00414ee0(text_object, nullptr);  // 006F4B0B
    Label* const item = &self->desc_item;                                          // 006F4B1A (+E8C)
    item->vtbl->set_extent(item, nullptr, &extent);                                // 006F4B10..27 vslot +04
    Control* items[1] = {item};                                                    // 006F4B39 (argument slot)
    gui_m48_listbox_add_controls_0041c2c0(&self->lb_desc, nullptr, items, 1, 0, 0, 0);   // 006F4B2A..3D
    gui_load_listbox_set_active_control_0041c160(&self->lb_desc, nullptr, item, 0);       // 006F4B42..47
}
// ======================================================================== 006F4B60 IsClassSkill(skill) (RET 4)
Word __fastcall kotor_m15d_skills_is_class_skill_006f4b60(SkillsPanel* self, void*, Word skill) {
    for (unsigned i = 0; i < ld<U8>(stats_of(self->creature), 0x32); ++i) {        // 006F4B65..BA (class count, re-read)
        void* const stats = stats_of(self->creature);
        const U8 class_id = kotor_m15b_creature_stats_get_class_00647730(stats, nullptr, static_cast<U8>(i));   // 006F4B80..81
        void* const cls = at(rp(rules(), 0xAC), static_cast<unsigned>(class_id) * 0x1A0u);   // 006F4B86..9F
        if (kotor_m15b_class_is_skill_class_skill_005be010(cls, nullptr, static_cast<U16>(skill)) != 0) return 1;   // 006F4B9E..A8
    }
    return 0;                                                                      // 006F4BBC..C1
}

// ======================================================================== 006F4E20 OnRecommendButton (RET)
void __fastcall kotor_m15d_skills_on_recommend_006f4e20(SkillsPanel* self, void*) noexcept(false) {
    void* stats = stats_of(self->creature);                                        // 006F4E43..46
    const U8 class_id = kotor_m15b_creature_stats_get_class_00647730(stats, nullptr,
        static_cast<U8>(ld<U8>(stats, 0x32) - 1));                                 // 006F4E4C..60 ([esp+13h])
    if ((self->skill_flags & 1u) == 0) {                                           // 006F4E64..6B
        st<U16>(stats_of(self->creature), 0xAE, 0);                                // 006F4E6D..76
        kotor_m15b_levelup_stats_add_skill_points_per_level_00648a00(stats_of(self->creature), nullptr);   // 006F4E7F..88
        kotor_m15b_creature_stats_clear_skills_006475e0(stats_of(self->creature), nullptr);                // 006F4E8D..96
        kotor_m15b_creature_stats_update_ability_modified_skill_ranks_00649380(stats_of(self->creature), nullptr);   // 006F4E9B..A4
    } else {
        st<U16>(stats_of(self->creature), 0xAE, static_cast<U16>(self->initial_points));   // 006F4EAB..BB (+49CC)
    }
    for (unsigned b = 0; b < ld<U8>(rules(), 0xAB); ++b)                           // 006F4EC2..F03
        kotor_m15b_levelup_stats_set_skill_rank_base_006488e0(stats_of(self->creature), nullptr, static_cast<U16>(b),
                                                              base_rank(self, b));  // 006F4ED4..EF
    kotor_m15b_levelup_stats_add_default_package_skills_006491a0(stats_of(self->creature), nullptr, class_id,
        static_cast<std::int32_t>((self->skill_flags >> 1) & 1u));                 // 006F4F05..20
    for (unsigned b = 0; b < ld<U8>(rules(), 0xAB); ++b) {                         // 006F4F25..515E ([esp+13h] reused)
        stats = stats_of(self->creature);
        I32 max = static_cast<I32>(static_cast<U8>(kotor_m15b_levelup_stats_get_level_00648430(stats, nullptr))) + 3;   // 006F4F50..65
        if (kotor_m15d_skills_is_class_skill_006f4b60(self, nullptr, static_cast<U16>(b)) == 0) max /= 2;   // 006F4F68..7F
        set_text_int(at(point_button(self, b), kButtonText), static_cast<I8>(rank_of(stats_of(self->creature), b)));   // 006F4F83..EE
        if ((self->skill_flags & 1u) != 0)                                         // 006F4FF3..FA
            set_bit1(minus_button(self, b),
                     static_cast<I32>(static_cast<I8>(rank_of(stats_of(self->creature), b))) != base_rank(self, b));   // 006F4FFC..49
        else
            set_bit1(minus_button(self, b), rank_of(stats_of(self->creature), b) != 0);   // 006F5051..93
        set_bit1(plus_button(self, b), static_cast<I32>(static_cast<I8>(rank_of(stats_of(self->creature), b))) != max);   // 006F5099..E4
        if ((self->skill_flags & 2u) == 0 && greyed(self->skill_flags, b))        // 006F50EA..16
            show_greyed(self, b);                                                  // 006F5118..3F
    }
    const Word points = ld<U16>(stats_of(self->creature), 0xAE);                   // 006F5164..6D
    self->points = static_cast<I32>(points);                                       // 006F5179
    set_text_int(at(&self->lbl_remaining, kLabelText), static_cast<int>(points));  // 006F5174..A8 (+7F4)
    kotor_m15b_creature_stats_update_ability_modified_skill_ranks_00649380(stats_of(self->creature), nullptr);   // 006F51AD..B6
}

// ======================================================================== 006F51D0 ctor (thiscall, RET 0Ch)
void* __fastcall kotor_m15d_skills_construct_006f51d0(void* memory, void*, void* manager, void* creature,
                                                       std::uint32_t level_up) noexcept(false) {
    SkillsPanel* const self = static_cast<SkillsPanel*>(memory);
    gui_load_panel_construct_0040b570(self, nullptr, static_cast<Object*>(manager));   // 006F51EE..FB
    int built = 0;
    try {
        gui_lb::set_vptr(self, &gui_m15d_skills_methods_00759990);                // 006F520B (759990)
        gui_load_listbox_construct_0041be40(&self->lb_desc, nullptr);  built = 1;  // 006F5200..11
        for (int i = 0; i < 10; ++i) { gui_load_label_construct_0041acd0(label_at(self, i), nullptr); built = 2 + i; }   // 006F5216..B3
        vector_construct(self->skill_labels, 8, gui_load_label_construct_0041acd0, gui_load_label_destruct_00419d10);   built = 0x0C;   // 006F52B8..D5
        vector_construct(self->point_buttons, 8, gui_load_button_construct_0041c0f0, gui_load_button_destruct_00419da0); built = 0x0D;   // 006F52DA..F7
        gui_load_button_construct_0041c0f0(&self->btn_accept, nullptr);       built = 0x0E;   // 006F52FC..307
        gui_load_button_construct_0041c0f0(&self->btn_back, nullptr);         built = 0x0F;   // 006F530C..17
        gui_load_button_construct_0041c0f0(&self->btn_recommended, nullptr);  built = 0x10;   // 006F531C..27
        vector_construct(self->plus_buttons, 8, gui_load_button_construct_0041c0f0, gui_load_button_destruct_00419da0);  built = 0x11;   // 006F532C..49
        vector_construct(self->minus_buttons, 8, gui_load_button_construct_0041c0f0, gui_load_button_destruct_00419da0); built = 0x12;   // 006F534E..6B
        vector_construct(self->descriptions, 8, string_ctor, string_dtor);    built = 0x13;   // 006F5370..8A (005B3190/005E5C20)
        self->skill_flags ^= (self->skill_flags ^ level_up) & 1u;                  // 006F538F..A0 (bit0 = level_up)
        self->creature = creature;                                                 // 006F5398 / 006F53AE
        Word party = 1;                                                            // 006F53F5
        if (level_up != 0) {                                                       // 006F53A6..B1
            void* const server_creature = kotor_m15a_creature_get_server_creature_0060fb20(creature, nullptr);   // 006F53B3
            if (rp(rp(server_creature, 0xA74), 0x6C) == nullptr) party = 0;       // 006F53B8..C3
            else if (rp(kotor_menu_window_cells::manager_007a39fc, 8) == nullptr) party = 0;   // 006F53C5..CF
            else if (lane_config_required::GetPartyTable_004aee70(rp(kotor_menu_window_cells::manager_007a39fc, 8), nullptr) == nullptr)
                party = 0;                                                         // 006F53D1..D8
            else if (rd32(lane_config_required::GetPartyTable_004aee70(rp(kotor_menu_window_cells::manager_007a39fc, 8), nullptr), 0xF0)
                     != 0xFFFFFFFFu)
                party = 0;                                                         // 006F53DA..EF
        }
        self->skill_flags ^= (self->skill_flags ^ (party << 1)) & 2u;              // 006F53FA..0D (bit1)
        const bool droid = ld<U16>(stats_of(self->creature), 0x24) == kRaceWord_0074bf10;   // 006F5413..26
        self->skill_flags ^= (self->skill_flags ^ (droid ? 4u : 0u)) & 4u;         // 006F5429..36 (bit2)
        {
            void* const stats = stats_of(self->creature);                          // 006F5433..3C
            (void)kotor_m15b_creature_stats_get_class_00647730(stats, nullptr, static_cast<U8>(ld<U8>(stats, 0x32) - 1));   // 006F5442..4A (unused)
        }
        {
            Ref layout;
            layout.construct_00406d80("SKCHRGEN");                                // 006F544F..58 (00759C58)
            gui_load_panel_start_layout_0040a680(self, nullptr, &layout);          // 006F545D..64
        }
        init_control(self, &self->lbl_main_title, "MAIN_TITLE_LBL");              // 006F5469..99
        if (level_up != 0)                                                         // 006F549E..A3
            gui_load_text_params_set_strref_00415e50(at(&self->lbl_main_title, kLabelText), nullptr, 0x42F);   // 006F54A5..B0 (+434)
        init_control(self, &self->lbl_sub_title, "SUB_TITLE_LBL");                // 006F54B5..E5
        init_control(self, &self->lbl_remaining_box, "REMAINING_BOX_LBL");        // 006F54EA..1A
        init_control(self, &self->lbl_remaining, "REMAINING_SELECTIONS_LBL");     // 006F551F..4F
        init_control(self, &self->lbl_selections, "SELECTIONS_REMAINING_LBL");    // 006F5554..84
        init_control(self, &self->lbl_desc, "DESC_LBL");                          // 006F5589..B9
        init_control(self, &self->lbl_cost, "COST_LBL");                          // 006F55BE..EE
        init_control(self, &self->lbl_cost_points, "COST_POINTS_LBL");            // 006F55F3..23
        init_control(self, &self->lbl_class_skill, "CLASSSKL_LBL");               // 006F5628..58
        init_control(self, &self->lb_desc, "LB_DESC");                            // 006F565D..8A
        {
            self->lb_desc.u2c1 = 1;                                                // 006F5698 (+32D; before the string ctor)
            String name("BTN_ACCEPT");                                             // 006F568F..9F (007548B8)
            gui_load_panel_init_control_0040b930(self, nullptr, &self->btn_accept, &name, 1);   // 006F56A4..B9
        }
        init_control(self, &self->btn_recommended, "BTN_RECOMMENDED");            // 006F56CB..FB
        init_control(self, &self->btn_back, "BTN_BACK");                          // 006F5700..30
        // Per-skill controls in the original's order (skill i: points button, label, description, minus, plus).
        struct SkillTags { unsigned skill; const char* points; const char* label; const char* minus; const char* plus;
                           unsigned description_offset; };
        static const SkillTags tags[8] = {
            {0, "COMPUTER_USE_POINTS_BTN", "COMPUTER_USE_LBL", "COM_MINUS_BTN", "COM_PLUS_BTN", 0x00},   // 006F5735..5803
            {1, "DEMOLITIONS_POINTS_BTN", "DEMOLITIONS_LBL", "DEM_MINUS_BTN", "DEM_PLUS_BTN", 0x20},     // 006F583D..5942
            {2, "STEALTH_POINTS_BTN", "STEALTH_LBL", "STE_MINUS_BTN", "STE_PLUS_BTN", 0x40},             // 006F5947..5A4D
            {4, "PERSUADE_POINTS_BTN", "PERSUADE_LBL", "PER_MINUS_BTN", "PER_PLUS_BTN", 0x80},           // 006F5A52..5B90
            {5, "REPAIR_POINTS_BTN", "REPAIR_LBL", "REP_MINUS_BTN", "REP_PLUS_BTN", 0xA0},               // 006F5B95..5C68
            {6, "SECURITY_POINTS_BTN", "SECURITY_LBL", "SEC_MINUS_BTN", "SEC_PLUS_BTN", 0xC0},           // 006F5C6D..5D76
            {3, "AWARENESS_POINTS_BTN", "AWARENESS_LBL", "AWA_MINUS_BTN", "AWA_PLUS_BTN", 0x60},         // 006F5D7B..5E81
            {7, "TREAT_INJURY_POINTS_BTN", "TREAT_INJURY_LBL", "TRE_MINUS_BTN", "TRE_PLUS_BTN", 0xE0},   // 006F5E86..5F8E
        };
        for (const SkillTags& t : tags) {
            init_control(self, point_button(self, t.skill), t.points);
            init_control(self, skill_label(self, t.skill), t.label);
            {
                StringSlot description;                                            // [esp+20h] (hidden return)
                kotor_m15b_skill_get_description_text_005cd970(at(rp(rules(), 0xB4), t.description_offset), nullptr,
                                                               description.raw()); // e.g. 006F579F..B0
                description.live = true;
                self->descriptions[t.skill].Assign005e5c50(description.get());     // e.g. 006F57B5..C1 (+4978 + i*8)
            }
            init_control(self, minus_button(self, t.skill), t.minus);
            init_control(self, plus_button(self, t.skill), t.plus);
        }
        gui_load_panel_stop_layout_0040b8f0(self, nullptr);                        // 006F5F93..95
        {
            Control* const proto = self->lb_desc.proto;                            // 006F5F9A (+338)
            vt<LabelInitView>(static_cast<Object*>(&self->desc_item))->initialize98(
                &self->desc_item, nullptr, word_of(&proto->extent), word_of(proto), 0x3F800000u);   // 006F5FA0..AE
        }
        self->current = 0;                                                         // 006F5FB6
        for (unsigned i = 0; i < 8; ++i) {                                         // 006F5FBC..627A
            Button* const points = point_button(self, i);
            gui_load_control_add_event_0041ab20(points, nullptr, 0, event_target(self),
                handler(&SkillsHandlers::OnEnterPointsButton_006f4bf0));           // 006F5FD0..DE
            gui_load_control_add_event_0041ab20(points, nullptr, 1, event_target(self),
                handler(&SkillsHandlers::OnExitPointsButton_006f4a80));            // 006F5FE3..F1
            points->u58 = i;                                                       // 006F5FF6..FA ([edi-1358h])
            if ((self->skill_flags & 1u) == 0) {                                   // 006F6000..07
                kotor_m15b_levelup_stats_set_skill_rank_base_006488e0(stats_of(self->creature), nullptr,
                                                                      static_cast<U16>(i), 0);   // 006F6009..15
                if (static_cast<U16>(i) < static_cast<U16>(ld<U8>(rules(), 0xAB)))  // 006F601A..36
                    st<U8>(rp(stats_of(self->creature), 0x84), i, 0);              // 006F6038..43
            }
            const U8 rank = rank_of(stats_of(self->creature), i);                  // 006F604B..74
            base_rank(self, i) = rank;                                             // 006F607F
            set_text_int(at(points, kButtonText), static_cast<int>(rank));         // 006F6077..A8 (movzx; +1B38)
            I32 max = static_cast<I32>(static_cast<U8>(kotor_m15b_levelup_stats_get_level_00648430(stats_of(self->creature), nullptr))) + 3;   // 006F60AD..C8
            if (kotor_m15d_skills_is_class_skill_006f4b60(self, nullptr, i) == 0) max /= 2;   // 006F60C5..DE
            if ((self->skill_flags & 1u) != 0)                                     // 006F60E2..E9
                set_bit1(minus_button(self, i),
                         static_cast<I32>(static_cast<I8>(rank_of(stats_of(self->creature), i))) != base_rank(self, i));   // 006F60EB..3E
            else
                set_bit1(minus_button(self, i), rank_of(stats_of(self->creature), i) != 0);   // 006F614A..90
            set_bit1(plus_button(self, i), static_cast<I32>(static_cast<I8>(rank_of(stats_of(self->creature), i))) != max);   // 006F6196..E3
            minus_button(self, i)->flags &= 0xFFFFFFFBu;                           // 006F61E5 ([edi+E20h])
            plus_button(self, i)->flags &= 0xFFFFFFFBu;                            // 006F61EC ([edi])
            gui_load_control_add_event_0041ab20(minus_button(self, i), nullptr, 0x27, event_target(self),
                kotor_m15d::event_func(&PanelStepThunks::OnMinusButtonPushed_0067cb40));   // 006F61D2..F5 (0067CB40)
            gui_load_control_add_event_0041ab20(plus_button(self, i), nullptr, 0x27, event_target(self),
                kotor_m15d::event_func(&PanelStepThunks::OnPlusButtonPushed_0067cb50));    // 006F61FA..205 (0067CB50)
            if ((self->skill_flags & 2u) == 0 && greyed(self->skill_flags, i))    // 006F620A..37
                show_greyed(self, i);                                              // 006F6239..57
        }
        self->btn_accept.flags &= 0xFFFFFFFBu;                                     // 006F6280..9B
        gui_load_control_add_event_0041ab20(&self->btn_accept, nullptr, 0x27, event_target(self),
            kotor_m1_shared::event_func(&kotor_m1_shared::PanelEventThunks::AcceptButtonCallback_00624ba0));   // 006F628C..9E
        self->btn_recommended.flags &= 0xFFFFFFFBu;                                // 006F62A3..B9
        gui_load_control_add_event_0041ab20(&self->btn_recommended, nullptr, 0x27, event_target(self),
            kotor_m15_l0::event_func(&kotor_m15_l0::PanelEventThunks15::OnYButtonPressed_00644720));       // 006F62AF..BC
        self->btn_back.flags &= 0xFFFFFFFBu;                                       // 006F62C1..D7
        gui_load_control_add_event_0041ab20(&self->btn_back, nullptr, 0x27, event_target(self),
            kotor_m1_shared::event_func(&kotor_m1_shared::PanelEventThunks::OnBButtonPressed_00624bb0));    // 006F62CD..DA
        kotor_m15b_levelup_stats_add_skill_points_per_level_00648a00(stats_of(self->creature), nullptr);   // 006F62DF..E8
        const Word points = ld<U16>(stats_of(self->creature), 0xAE);               // 006F62ED..F6
        self->points = static_cast<I32>(points);                                   // 006F6302
        self->initial_points = static_cast<I32>(points);                           // 006F6308
        set_text_int(at(&self->lbl_remaining, kLabelText), static_cast<int>(points));   // 006F62FD..30 (+7F4)
        gui_load_panel_set_active_control_0040a630(self, nullptr, point_button(self, static_cast<unsigned>(self->current)), 0);   // 006F6335..4D
    } catch (...) {
        unwind_skills(self, built);
        throw;
    }
    return self;                                                                   // 006F6357
}

// ======================================================================== 006F6370 OnMinusButton (RET)
void __fastcall kotor_m15d_skills_on_minus_006f6370(SkillsPanel* self, void*) noexcept(false) {
    void* const stats = stats_of(self->creature);                                  // 006F638C..9E
    const U16 skill = static_cast<U16>(self->current);                             // 006F63A4
    const U8 base = base_rank(self, static_cast<unsigned>(self->current));         // 006F63C0..C6
    if (static_cast<I32>(static_cast<I8>(rank_of(stats, skill))) == base) return;  // 006F63AB..D6
    kotor_m15b_levelup_stats_decrease_skill_rank_00648eb0(stats, nullptr, skill, base);   // 006F63DC..E0
    const Word points = ld<U16>(stats_of(self->creature), 0xAE);                   // 006F63E5..EE
    self->points = static_cast<I32>(points);                                       // 006F63FC
    StringSlot rank_text;                                                          // [esp+14h] (EH state 0)
    resman_string_from_int_005e5bc0(rank_text.raw(), nullptr,
        static_cast<I8>(rank_of(stats_of(self->creature), static_cast<U16>(self->current))));   // 006F6402..33
    rank_text.live = true;
    gui_load_text_params_set_text_00415e00(at(point_button(self, static_cast<unsigned>(self->current)), kButtonText),
                                           nullptr, rank_text.get());              // 006F6438..58
    StringSlot points_text;                                                        // [esp+0Ch] (EH state 1)
    resman_string_from_int_005e5bc0(points_text.raw(), nullptr, static_cast<int>(self->points));   // 006F645D..68
    points_text.live = true;
    gui_load_text_params_set_text_00415e00(at(&self->lbl_remaining, kLabelText), nullptr, points_text.get());   // 006F646D..7D (+7F4)
    reinterpret_cast<SkillsHandlers*>(self)->OnEnterPointsButton_006f4bf0(
        reinterpret_cast<kih::Control*>(point_button(self, static_cast<unsigned>(self->current))));   // 006F6482..98
    plus_button(self, static_cast<unsigned>(self->current))->flags |= 2u;          // 006F649D..BA
    if (static_cast<I32>(static_cast<I8>(rank_of(stats_of(self->creature), static_cast<U16>(self->current))))
        == base_rank(self, static_cast<unsigned>(self->current))) {                // 006F64BD..503
        minus_button(self, static_cast<unsigned>(self->current))->flags &= ~2u;    // 006F6505..1C
        set_active0(minus_button(self, static_cast<unsigned>(self->current)));     // 006F651F..3B vslot +40(0)
    }
}                                                                                  // 006F653E..58 (005E5C20 x2)

// ======================================================================== 006F6570 OnPlusButton (RET)
void __fastcall kotor_m15d_skills_on_plus_006f6570(SkillsPanel* self, void*) noexcept(false) {
    const I32 skill = self->current;                                               // 006F6593
    Word refusal = 0;
    if ((skill == 2 || skill == 7) && (self->skill_flags & 4u) != 0) refusal = 0xA5E7;   // 006F6656..63 (droid)
    else if (skill == 4 && (self->skill_flags & 2u) == 0) refusal = 0xA5E5;        // 006F6642..4F
    if (refusal != 0) {                                                            // 006F6668..A0
        void* const box = message_box();
        kotor_fastcall_00627130(box, nullptr, 0);                                  // SetAllowCancel(ebx = 0)
        gui_load_messagebox_set_callback_00624a40(box, nullptr, nullptr, nullptr, 0);
        vt<MessageBoxView>(box)->set_message_strref(box, nullptr, refusal);        // vslot +6C
        kotor_fastcall_0040bc70(self->manager, nullptr, box, 1, 1);
        return;
    }
    I32 max = static_cast<I32>(static_cast<U8>(kotor_m15b_levelup_stats_get_level_00648430(stats_of(self->creature), nullptr))) + 3;   // 006F65BF..D9
    const Word is_class = kotor_m15d_skills_is_class_skill_006f4b60(self, nullptr, static_cast<Word>(self->current));   // 006F65D0..E1 (ebx)
    if (is_class == 0) max /= 2;                                                   // 006F65E3..EE
    bool enough = false, below_max = false;                                        // [esp+10h] / [esp+18h]
    {
        void* const stats = stats_of(self->creature);                              // 006F65F0..F3
        const Word points = ld<U16>(stats, 0xAE);                                  // 006F65FF
        if (static_cast<I32>(points) >= kotor_m15b_levelup_stats_get_skill_point_cost_00647900(stats, nullptr,
                static_cast<U16>(self->current), 1))                               // 006F6606..12
            enough = true;                                                         // 006F6614
    }
    if (static_cast<I32>(static_cast<I8>(rank_of(stats_of(self->creature), static_cast<U16>(self->current)))) < max) {   // 006F661C..C6
        below_max = true;                                                          // 006F66D7
        if (enough) {                                                              // 006F66CC..DB
            kotor_m15b_levelup_stats_increase_skill_rank_00648d50(stats_of(self->creature), nullptr,
                                                                  static_cast<U16>(self->current));   // 006F66E1..E8
            self->points = static_cast<I32>(ld<U16>(stats_of(self->creature), 0xAE));   // 006F66ED..706
            StringSlot rank_text;                                                  // [esp+30h] (state 5)
            resman_string_from_int_005e5bc0(rank_text.raw(), nullptr, static_cast<I8>(
                kotor_m15b_levelup_stats_get_skill_rank_0060b350(stats_of(self->creature), nullptr,
                                                                 static_cast<U16>(self->current))));   // 006F670C..22
            rank_text.live = true;
            gui_load_text_params_set_text_00415e00(at(point_button(self, static_cast<unsigned>(self->current)), kButtonText),
                                                   nullptr, rank_text.get());      // 006F6727..48
            StringSlot points_text;                                                // [esp+28h] (state 6)
            resman_string_from_int_005e5bc0(points_text.raw(), nullptr, static_cast<int>(self->points));   // 006F674D..58
            points_text.live = true;
            gui_load_text_params_set_text_00415e00(at(&self->lbl_remaining, kLabelText), nullptr, points_text.get());   // 006F675D..6D
            reinterpret_cast<SkillsHandlers*>(self)->OnEnterPointsButton_006f4bf0(
                reinterpret_cast<kih::Control*>(point_button(self, static_cast<unsigned>(self->current))));   // 006F6772..88
            minus_button(self, static_cast<unsigned>(self->current))->flags |= 2u;     // 006F678D..AA
            if (static_cast<I32>(static_cast<I8>(kotor_m15b_levelup_stats_get_skill_rank_0060b350(
                    stats_of(self->creature), nullptr, static_cast<U16>(self->current)))) == max) {   // 006F67AD..CA
                plus_button(self, static_cast<unsigned>(self->current))->flags &= ~2u;   // 006F67CC..E9
                set_active0(plus_button(self, static_cast<unsigned>(self->current)));    // 006F67EC..808 vslot +40(0)
            }
            return;                                                                // 006F680B..1C / 006F695D..65
        }
    }
    void* const box = message_box();                                               // 006F6821..2E (ebp)
    StartupString message;                                                         // 006F6834..38 (005B3190; state 0)
    if (!enough) {                                                                 // 006F683D..4B
        StringSlot text;                                                           // [esp+18h]
        tlk()->simple_0041e8f0(reinterpret_cast<kotor_menu_10_19::String*>(text.raw()), 0xA5E0);   // 006F684D..5D
        text.live = true;
        message.Assign005e5c50(text.get());                                        // 006F6862..6C
    } else if (!below_max) {                                                       // 006F6884..8A
        {
            StringSlot text;
            tlk()->simple_0041e8f0(reinterpret_cast<kotor_menu_10_19::String*>(text.raw()),
                                   is_class != 0 ? 0xA4C2u : 0xA4C3u);             // 006F6890..BA
            text.live = true;
            message.Assign005e5c50(text.get());                                    // 006F68C4..C9
        }                                                                          // 006F68CE..D7
        {
            StringSlot limit;
            resman_string_from_int_005e5bc0(limit.raw(), nullptr, max);            // 006F68DC..E1 (edi)
            limit.live = true;
            kotor_fastcall_0041db50(kotor_cell_tlk_007a3a08, nullptr, 0, limit.get());   // 006F68E6..F8 SetCustomToken(0, max)
        }                                                                          // 006F68FD..906
        tlk_core::ParseStr0041dd30(tlk(), nullptr, &message);                      // 006F690B..16
    }
    show_message_text(self, box, &message);                                        // 006F691B..54
}                                                                                  // 006F6959..65 (005E5C20)

// ======================================================================== 006F6980 OnAcceptButton (RET)
void __fastcall kotor_m15d_skills_on_accept_006f6980(SkillsPanel* self, void*) noexcept(false) {
    if (self->points > 0) {                                                        // 006F6983..8B: unspent points
        void* const box = message_box();                                           // 006F698D..9B
        kotor_fastcall_00627130(box, nullptr, 1);                                  // 006F69A1..A5 SetAllowCancel(1)
        vt<MessageBoxView>(box)->set_message_strref(box, nullptr, 0xA357);         // 006F69AA..B3 vslot +6C
        gui_load_messagebox_set_callback_00624a40(box, nullptr, event_target(self),
                                                  handler(&SkillsHandlers::ConfirmAccept_006f4dc0), 0);   // 006F69B6..C0
        kotor_fastcall_0040bc70(self->manager, nullptr, box, 1, 1);                // 006F69C5..CD
        return;
    }
    close_to_parent(self);                                                         // 006F69D5..F7
    if ((self->skill_flags & 1u) != 0) kotor_fastcall_006ee5d0(self->parent, nullptr);   // 006F69FA..A04 (L15-F trap)
    else kotor_m15c_custom_skill_creation_completed_006ef520(self->parent, nullptr);      // 006F6A09
}

// ======================================================================== 006F6A10 HandleInputEvent(event, value) (RET 8)
// Byte table 006F6B38 / jump table 006F6B18 for event-27h in 0..19h: 27h/2Dh accept, 28h/2Eh cancel, 2Ah recommend,
// 2Fh/3Fh minus, 30h/40h plus, 39h/3Ah scroll LB_DESC; then CSWGuiPanel::HandleInputEvent.
void __fastcall kotor_m15d_skills_handle_input_006f6a10(SkillsPanel* self, void*, I32 event, I32 value) noexcept(false) {
    if (value != 0) {                                                              // 006F6A15..1F
        switch (event) {
        case 0x27: case 0x2D:                                                      // 006F6A9F
            manager_of(self)->PlayGuiSound(0);
            kotor_m15d_skills_on_accept_006f6980(self, nullptr);
            break;
        case 0x28: case 0x2E:                                                      // 006F6A3F
            manager_of(self)->PlayGuiSound(0);
            kotor_m15d_skills_handle_cancel_006f49d0(self, nullptr);
            break;
        case 0x2A:                                                                 // 006F6ABF
            manager_of(self)->PlayGuiSound(0);
            kotor_m15d_skills_on_recommend_006f4e20(self, nullptr);
            break;
        case 0x2F: case 0x3F:                                                      // 006F6A5F
            manager_of(self)->PlayGuiSound(1);
            kotor_m15d_skills_on_minus_006f6370(self, nullptr);
            break;
        case 0x30: case 0x40:                                                      // 006F6A7F
            manager_of(self)->PlayGuiSound(1);
            kotor_m15d_skills_on_plus_006f6570(self, nullptr);
            break;
        case 0x39:                                                                 // 006F6AFB: LB_DESC vslot +3C(31h, 1)
            vt<InputView>(&self->lb_desc)->handle_input_event(&self->lb_desc, nullptr, 0x31, 1);
            break;
        case 0x3A:                                                                 // 006F6ADF: LB_DESC vslot +3C(32h, 1)
            vt<InputView>(&self->lb_desc)->handle_input_event(&self->lb_desc, nullptr, 0x32, 1);
            break;
        default:
            break;
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, value);            // 006F6A50 / 006F6B08 (00409E60)
}
}
