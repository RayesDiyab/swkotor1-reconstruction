// lane-m15-d / unit m15d-abilities: CSWGuiAbilitiesCharGen 006F6B60..006F8880 (0x3DF4; method table 00759C68),
// opened by CSWGuiCustomPanel::OnSelectAbilitiesButton 006EF3F0 (L15-C). Ability order STR, DEX, CON, WIS, INT, CHA.
// Level-up branches ([this+3DF0h] != 0; only CSWGuiLevelUpPanel passes level_up = 1, in-game) call the L15-F D83
// function traps 006EE580 / 006EE0D0 / 006EE5B0; the char-gen parent is CSWGuiCustomPanel (L15-C 006EF480 / 006EF500).
// Original: swkotor.exe GOG K1, static disassembly only; every behaviour cites its instruction addresses.
#include "88764680e4-m15d_common.h"

using namespace kotor_m15d;

extern "C" {
extern const MenuColor borrowed_exit_color_007a23b4;      // input-cells 007A23B4
extern const MenuColor borrowed_enter_color_007a23c0;     // input-cells 007A23C0
I32 __fastcall kotor_m15d_abilities_refund_006f6b60(AbilitiesPanel* self, void*, I32 value);
I32 __fastcall kotor_m15d_abilities_point_cost_006f6bb0(AbilitiesPanel* self, void*, I32 value);
void __fastcall kotor_m15d_abilities_destruct_006f6bf0(AbilitiesPanel* self, void*);
void* __fastcall kotor_m15d_abilities_deleting_destruct_006f70c0(AbilitiesPanel* self, void*, Word flags);
void __fastcall kotor_m15d_abilities_on_panel_added_006f6db0(AbilitiesPanel* self, void*) noexcept(false);
void __fastcall kotor_m15d_abilities_handle_cancel_006f6de0(AbilitiesPanel* self, void*) noexcept(false);
void __fastcall kotor_m15d_abilities_complete_panel_006f6e70(AbilitiesPanel* self, void*) noexcept(false);
void __fastcall kotor_m15d_abilities_set_description_006f6fa0(AbilitiesPanel* self, void*, Word strref) noexcept(false);
void __fastcall kotor_m15d_abilities_on_accept_006f7330(AbilitiesPanel* self, void*) noexcept(false);
void __fastcall kotor_m15d_abilities_on_recommend_006f7390(AbilitiesPanel* self, void*) noexcept(false);
void __fastcall kotor_m15d_abilities_on_minus_006f8480(AbilitiesPanel* self, void*) noexcept(false);
void __fastcall kotor_m15d_abilities_on_plus_006f8670(AbilitiesPanel* self, void*) noexcept(false);
void __fastcall kotor_m15d_abilities_handle_input_006f8880(AbilitiesPanel* self, void*, I32 event, I32 value) noexcept(false);
}

namespace {
const unsigned char k_tlk_flag_0073ecd0 = 0x00;           // .rdata byte 0073ECD0 (image value 00; as gui-load gui_params)
AbilitiesPanel* abilities_of(AbilitiesHandlers* h) { return reinterpret_cast<AbilitiesPanel*>(h); }
gui_load::EventFunc handler(void (__thiscall AbilitiesHandlers::*f)(kih::Control*)) { return static_cast<gui_load::EventFunc>(f); }
Label* ability_label(AbilitiesPanel* s, unsigned i) { return reinterpret_cast<Label*>(at(s, 0x110Cu + 0x140u * i)); }
Button* point_button(AbilitiesPanel* s, unsigned i) { return reinterpret_cast<Button*>(at(s, 0x188Cu + 0x1C4u * i)); }
Button* plus_button(AbilitiesPanel* s, unsigned i) { return reinterpret_cast<Button*>(at(s, 0x2870u + 0x1C4u * i)); }
Button* minus_button(AbilitiesPanel* s, unsigned i) { return reinterpret_cast<Button*>(at(s, 0x3308u + 0x1C4u * i)); }
I32& value_at(AbilitiesPanel* s, unsigned i) { return *reinterpret_cast<I32*>(at(s, 0x3DBCu + 4u * i)); }
I32& base_at(AbilitiesPanel* s, unsigned i) { return *reinterpret_cast<I32*>(at(s, 0x3DD4u + 4u * i)); }
void set_bit1(Button* b, bool value) { b->flags = ((b->flags ^ (value ? 2u : 0u)) & 2u) ^ b->flags; }
void set_active0(Button* b) { vt<ActiveView>(static_cast<Object*>(b))->set_active(b, nullptr, 0); }   // vslot +40 (0)
// Point cost inlined at 006F716C..92 / 006F870A..38 (same formula as 006F6BB0).
I32 inline_cost(const AbilitiesPanel* self, I32 value) {
    if (self->level_up != 0) return 1;
    if (value > 15) return 3;
    return (value > 13 ? 1 : 0) + 1;
}
void close_to_parent(AbilitiesPanel* self) {                                       // 006F6DEA..09 / 006F6F57..79
    kotor_fastcall_0040be00(self->manager, nullptr);                               // PopModalPanel 0040BE00
    kotor_m15_panel_set_visible_0040b9d0(self->parent, nullptr, 1);                // SetVisible 0040B9D0
    self->flags = (self->flags & 0xFFFFFCFFu) | 0x400u;                            // [this+44h]
}
// SetAllowCancel(0); vslot +6C SetMessage(strref); SetCallback(0,0,0); AddPanel(mb, 1, 1)
// (006F7351..7B / 006F84DC..506 / 006F86C9..F3).
void show_message(AbilitiesPanel* self, Word strref) {
    void* const box = message_box();
    kotor_fastcall_00627130(box, nullptr, 0);
    vt<MessageBoxView>(box)->set_message_strref(box, nullptr, strref);
    gui_load_messagebox_set_callback_00624a40(box, nullptr, nullptr, nullptr, 0);
    kotor_fastcall_0040bc70(self->manager, nullptr, box, 1, 1);
}
// Member construction stages of 006F7600 (EH states 0..12h of FuncInfo 0073A2DB).
Label* label_at(AbilitiesPanel* s, int i) { return reinterpret_cast<Label*>(at(s, 0x34Cu + 0x140u * static_cast<unsigned>(i))); }   // 11 labels
template <class T, class C, class D> void vector_construct(T* first, int count, C construct, D destroy) {
    for (int i = 0; i < count; ++i) {                                              // eh_vector_constructor_iterator 006FB55F
        try { construct(first + i, nullptr); }
        catch (...) { while (i-- > 0) destroy(first + i, nullptr); throw; }
    }
}
template <class T, class D> void vector_destroy(T* first, int count, D destroy) {
    for (int i = count; i-- > 0;) destroy(first + i, nullptr);                     // eh_vector_destructor_iterator 006FA852
}
// built: 1 list box, 2..0Ch labels, 0Dh ability labels, 0Eh points, 0Fh..11h accept/back/recommended, 12h plus,
// 13h minus (the EH states of the original advance in the same order).
void unwind_abilities(AbilitiesPanel* self, int built) {
    if (built >= 0x13) vector_destroy(self->minus_buttons, 6, gui_load_button_destruct_00419da0);
    if (built >= 0x12) vector_destroy(self->plus_buttons, 6, gui_load_button_destruct_00419da0);
    if (built >= 0x11) gui_load_button_destruct_00419da0(&self->btn_recommended, nullptr);
    if (built >= 0x10) gui_load_button_destruct_00419da0(&self->btn_back, nullptr);
    if (built >= 0x0F) gui_load_button_destruct_00419da0(&self->btn_accept, nullptr);
    if (built >= 0x0E) vector_destroy(self->point_buttons, 6, gui_load_button_destruct_00419da0);
    if (built >= 0x0D) vector_destroy(self->ability_labels, 6, gui_load_label_destruct_00419d10);
    for (int i = 11; i-- > 0;) if (built >= 2 + i) gui_load_label_destruct_00419d10(label_at(self, i), nullptr);
    if (built >= 1) gui_load_listbox_destruct_0041b060(&self->lb_desc, nullptr);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
}

// ======================================================================== D21 method table 00759C68 (27 slots)
// Read from .rdata 00759C68..00759CD3: CSWGuiPanel 0073E010 except +00 006F70C0, +3C 006F8880, +48 006F6DB0.
struct GuiM15dAbilitiesMethods {
    void* (__fastcall* s00)(AbilitiesPanel*, void*, Word);                    // +00 006F70C0 deleting dtor
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
    void (__fastcall* s3c)(AbilitiesPanel*, void*, I32, I32);                 // +3C 006F8880 HandleInputEvent
    Object* (__fastcall* s40)(Object*, void*, I32, I32);                      // +40 0040B690
    Object* (__fastcall* s44)(Object*, void*);                                // +44 0040B630
    void (__fastcall* s48)(AbilitiesPanel*, void*);                           // +48 006F6DB0 OnPanelAdded
    void (__fastcall* s4c)(Panel*, void*);                                    // +4C 0040C170
    void (__fastcall* s50)(Object*, void*, Word);                             // +50 0040B640
    void (__fastcall* s54)(Object*, void*, Word);                             // +54 0040B650
    void (__fastcall* s58)(Object*, void*, Word);                             // +58 0040B660
    void (__fastcall* s5c)(Object*, void*, Word);                             // +5C 0040B670
    void (__fastcall* s60)(Object*, void*, Word);                             // +60 0040B680
    String* (__fastcall* s64)(Panel*, void*, String*);                        // +64 0040A900
    void (__fastcall* s68)(Panel*, void*);                                    // +68 0040A9C0
};
static_assert(sizeof(GuiM15dAbilitiesMethods) == 0x6C, "00759C68 slot count (27)");
extern "C" const GuiM15dAbilitiesMethods gui_m15d_abilities_methods_00759c68 = {
    &kotor_m15d_abilities_deleting_destruct_006f70c0,   // +00
    &gui_load_panel_set_extent_0040a5a0,                // +04
    &gui_load_panel_set_active_control_0040a630,        // +08
    &gui_load_slot_ret4_0060e760,                       // +0C
    &gui_load_slot_zero8_00409eb0,                      // +10
    &gui_load_slot_ret_004015a0,                        // +14
    &gui_load_slot_ret_004015a0,                        // +18
    &gui_load_slot_ret_004015a0,                        // +1C
    &gui_load_slot_ret_004015a0,                        // +20
    &gui_load_slot_ret_004015a0,                        // +24
    &gui_load_slot_self_00641db0,                       // +28
    &gui_load_slot_zero_0063e7f0,                       // +2C
    &gui_load_slot_zero_0063e7f0,                       // +30
    &kotor_panel_draw_0040B760,                         // +34
    &gui_load_slot_ret4_0060e760,                       // +38
    &kotor_m15d_abilities_handle_input_006f8880,        // +3C
    &gui_load_panel_hit_check_0040b690,                 // +40
    &gui_load_panel_active_control_0040b630,            // +44
    &kotor_m15d_abilities_on_panel_added_006f6db0,      // +48
    &gui_load_panel_on_removed_0040c170,                // +4C
    &gui_load_panel_button_a_0040b640,                  // +50
    &gui_load_panel_button_b_0040b650,                  // +54
    &gui_load_panel_button_x_0040b660,                  // +58
    &gui_load_panel_button_y_0040b670,                  // +5C
    &gui_load_panel_button_black_0040b680,              // +60
    &gui_load_panel_full_screen_bg_0040a900,            // +64
    &gui_load_panel_reset_font_0040a9c0,                // +68
};

// ======================================================================== member callbacks (thiscall, RET 4)
// 006F6E40 OnExitPointsButton(control): clear text flag 40h of the ability label and points button ([control+58h]).
void __thiscall AbilitiesHandlers::OnExitPointsButton_006f6e40(kih::Control* control) {
    AbilitiesPanel* const self = abilities_of(this);
    const Word i = rd32(control, kControlId);                                      // 006F6E40..44
    unsigned char* const label_flags = at(ability_label(self, i), kLabelText + kTextFlags);   // 006F6E47..54 (+11F4+38)
    st<Word>(label_flags, 0, ld<Word>(label_flags, 0) & 0xFFFFFFBFu);
    const Word j = rd32(control, kControlId);                                      // 006F6E58 (re-read)
    unsigned char* const button_flags = at(point_button(self, j), kButtonText + kTextFlags);   // 006F6E5B..68 (+19F8+38)
    st<Word>(button_flags, 0, ld<Word>(button_flags, 0) & 0xFFFFFFBFu);
}
// 006F70E0 OnEnterPointsButton(control): highlight the entered ability (007A23C0) and reset the others (007A23B4);
// cost, modifier text ("-", "+n", "-n"), description; text flag 40h on the entered ability.
void __thiscall AbilitiesHandlers::OnEnterPointsButton_006f70e0(kih::Control* control) {
    AbilitiesPanel* const self = abilities_of(this);
    for (unsigned i = 0; i < 6; ++i) {                                             // 006F70FE..715D
        void* const button_text = at(point_button(self, i), kButtonText);          // edi (+19F8)
        void* const label_text = at(ability_label(self, i), kLabelText);           // ebx (+11F4)
        const MenuColor* color = &borrowed_exit_color_007a23b4;                    // 006F7137
        if (reinterpret_cast<void*>(control) == point_button(self, i)) {           // 006F7110..1E (edi-16Ch)
            self->current = static_cast<I32>(i);                                   // 006F7125
            color = &borrowed_enter_color_007a23c0;                                // 006F7120
        }
        gui_load_text_params_set_color_00414e10(button_text, nullptr, color);      // 006F712B / 006F713C
        gui_load_text_params_set_color_00414e10(label_text, nullptr, color);       // 006F7146..48
    }
    const I32 value = value_at(self, static_cast<unsigned>(self->current));        // 006F715F..65
    StringSlot cost_text;                                                          // [esp+20h] (EH state 0)
    resman_string_from_int_005e5bc0(cost_text.raw(), nullptr, inline_cost(self, value));   // 006F716C..99
    cost_text.live = true;
    StartupString modifier;                                                        // [esp+10h] (005B3190; state 1)
    const I32 mod = static_cast<I8>(kotor_m15b_creature_stats_ability_mod_00647630(stats_of(self->creature), nullptr,
        static_cast<U8>(value_at(self, static_cast<unsigned>(self->current)))));  // 006F71AF..D2
    if (mod < 0) {                                                                 // 006F71D5..D7
        StringSlot number;                                                         // [esp+18h]
        resman_string_from_int_005e5bc0(number.raw(), nullptr, mod);               // 006F71D9..DE
        number.live = true;
        modifier.Assign005e5c50(number.get());                                     // 006F71E3..F1
    } else if (mod == 0) {                                                         // 006F71FC
        GuiAssign005e5140(&modifier, nullptr, "-");                               // 006F71FE..07 (0073ED40)
    } else {                                                                       // 006F720E..10
        StringSlot number;                                                         // [esp+30h]
        resman_string_from_int_005e5bc0(number.raw(), nullptr, mod);               // 006F7212..17
        number.live = true;
        String plus("+");                                                          // 006F721E..2C (00748ED4) [esp+28h]
        StringSlot joined;                                                         // [esp+18h]
        GuiConcat005e5d10(&plus, nullptr, joined.raw(), number.get());             // 006F7231..3F
        joined.live = true;
        modifier.Assign005e5c50(joined.get());                                     // 006F7244..4E
    }                                                                              // 006F7253..77 (005E5C20)
    gui_load_text_params_set_text_00415e00(at(&self->lbl_cost_points, kLabelText), nullptr, cost_text.get());   // 006F727C..87 (+CF4)
    gui_load_text_params_set_text_00415e00(at(&self->lbl_ability_mod, kLabelText), nullptr, &modifier);         // 006F728C..97 (+F74)
    kotor_m15d_abilities_set_description_006f6fa0(self, nullptr,
        ld<Word>(self, 0x3DA0u + 4u * static_cast<unsigned>(self->current)));     // 006F729C..AC
    unsigned char* const label_flags = at(ability_label(self, static_cast<unsigned>(self->current)), kLabelText + kTextFlags);
    st<Word>(label_flags, 0, ld<Word>(label_flags, 0) | 0x40u);                    // 006F72B1..D2
    unsigned char* const button_flags = at(point_button(self, static_cast<unsigned>(self->current)), kButtonText + kTextFlags);
    st<Word>(button_flags, 0, ld<Word>(button_flags, 0) | 0x40u);                  // 006F72D5..F1
}                                                                                  // 006F72ED..30A (005E5C20 x2)

extern "C" {
// ======================================================================== 006F6B60 / 006F6BB0 point cost (RET 4)
// 006F6B60: refund for a decrement (negative): level-up -1; 9..14 -> -1; 15..16 -> -2; >= 17 -> -3; <= 8 -> 0.
I32 __fastcall kotor_m15d_abilities_refund_006f6b60(AbilitiesPanel* self, void*, I32 value) {
    if (self->level_up != 0) return -1;                                            // 006F6B60..6D
    if (value <= 0x0E && value > 8) return -1;                                     // 006F6B70..7C
    if (value >= 0x0F && value <= 0x10) return -2;                                 // 006F6B7E..8D
    return value < 0x11 ? 0 : -3;                                                  // 006F6B90..9E (setl/dec/and FFFFFFFDh)
}
// 006F6BB0: cost of an increment: level-up 1; > 15 -> 3; > 13 -> 2; else 1.
I32 __fastcall kotor_m15d_abilities_point_cost_006f6bb0(AbilitiesPanel* self, void*, I32 value) {
    if (self->level_up != 0) return 1;                                             // 006F6BB0..BF
    if (value > 0x0F) return 3;                                                    // 006F6BC2..D0
    return (value > 0x0D ? 1 : 0) + 1;                                             // 006F6BD3..DE
}
// ======================================================================== 006F6BF0 dtor (RET)
void __fastcall kotor_m15d_abilities_destruct_006f6bf0(AbilitiesPanel* self, void*) {
    gui_lb::set_vptr(self, &gui_m15d_abilities_methods_00759c68);                 // 006F6C0E (759C68)
    gui_load_listbox_clear_items_00419a90(&self->lb_desc, nullptr);                // 006F6C14..21
    vector_destroy(self->minus_buttons, 6, gui_load_button_destruct_00419da0);    // 006F6C26..3E
    vector_destroy(self->plus_buttons, 6, gui_load_button_destruct_00419da0);     // 006F6C43..5B
    gui_load_button_destruct_00419da0(&self->btn_recommended, nullptr);            // 006F6C60..6B
    gui_load_button_destruct_00419da0(&self->btn_back, nullptr);                   // 006F6C70..7B
    gui_load_button_destruct_00419da0(&self->btn_accept, nullptr);                 // 006F6C80..8B
    vector_destroy(self->point_buttons, 6, gui_load_button_destruct_00419da0);    // 006F6C90..A8
    vector_destroy(self->ability_labels, 6, gui_load_label_destruct_00419d10);    // 006F6CAD..C5
    for (int i = 11; i-- > 0;) gui_load_label_destruct_00419d10(label_at(self, i), nullptr);   // 006F6CCA..75
    gui_load_listbox_destruct_0041b060(&self->lb_desc, nullptr);                   // 006F6D7A..81
    gui_load_panel_destruct_0040cf70(self, nullptr);                               // 006F6D86..90
}
// 006F70C0 scalar deleting destructor (RET 4).
void* __fastcall kotor_m15d_abilities_deleting_destruct_006f70c0(AbilitiesPanel* self, void*, Word flags) {
    kotor_m15d_abilities_destruct_006f6bf0(self, nullptr);                         // 006F70C3
    if (flags & 1u) live_free(self);                                               // 006F70C8..D5
    return self;                                                                   // 006F70D8
}
// ======================================================================== 006F6DB0 OnPanelAdded (RET)
void __fastcall kotor_m15d_abilities_on_panel_added_006f6db0(AbilitiesPanel* self, void*) noexcept(false) {
    gui_load_panel_on_added_0040b870(self, nullptr);                               // 006F6DB0
    show_tutorial_window_005edf40(client_app(), nullptr, 0x10, 0x7F000000u, 0x7F000000u, 0);   // 006F6DB5..CB
}
// ======================================================================== 006F6DE0 HandleCancelButton (RET)
void __fastcall kotor_m15d_abilities_handle_cancel_006f6de0(AbilitiesPanel* self, void*) noexcept(false) {
    if (self->parent == nullptr) return;                                           // 006F6DE3..E8
    close_to_parent(self);                                                         // 006F6DEA..09
    if (self->level_up != 0) {                                                     // 006F6E0C..14
        kotor_fastcall_006ee580(self->parent, nullptr);                            // 006F6E16..1A (L15-F trap)
        return;
    }
    st<U8>(stats_of(self->creature), 0xAC, 0x1E);                                  // 006F6E1F..28 (ability points = 30)
    kotor_m15c_custom_cancelled_by_child_006ef480(self->parent, nullptr);          // 006F6E2F..33
}
// ======================================================================== 006F6E70 CompletePanel (RET)
void __fastcall kotor_m15d_abilities_complete_panel_006f6e70(AbilitiesPanel* self, void*) noexcept(false) {
    if (self->level_up != 0) {                                                     // 006F6E73..7B
        kotor_fastcall_006ee0d0(self->parent, nullptr,
            static_cast<std::uint32_t>(value_at(self, 0)), static_cast<std::uint32_t>(value_at(self, 5)),
            static_cast<std::uint32_t>(value_at(self, 4)), static_cast<std::uint32_t>(value_at(self, 3)),
            static_cast<std::uint32_t>(value_at(self, 2)), static_cast<std::uint32_t>(value_at(self, 1)),
            static_cast<std::uint32_t>(self->points));                             // 006F6E7D..B1 (L15-F trap)
    } else {
        kotor_m15b_levelup_stats_set_str_base_0064a0d0(stats_of(self->creature), nullptr, static_cast<U8>(value_at(self, 0)));   // 006F6EBB..CD
        kotor_m15b_levelup_stats_set_cha_base_006478d0(stats_of(self->creature), nullptr, static_cast<U8>(value_at(self, 5)));   // 006F6ED2..E4
        kotor_m15b_levelup_stats_set_int_base_006478a0(stats_of(self->creature), nullptr, static_cast<U8>(value_at(self, 4)));   // 006F6EE9..FB
        kotor_m15b_levelup_stats_set_wis_base_00649770(stats_of(self->creature), nullptr, static_cast<U8>(value_at(self, 3)));   // 006F6F00..12
        kotor_m15b_levelup_stats_set_con_base_006496d0(stats_of(self->creature), nullptr, static_cast<U8>(value_at(self, 2)));   // 006F6F17..29
        kotor_m15b_levelup_stats_set_dex_base_0064a100(stats_of(self->creature), nullptr, static_cast<U8>(value_at(self, 1)), 1);   // 006F6F2E..42
        st<U8>(stats_of(self->creature), 0xAC, 0);                                 // 006F6F47..50 (ability points = 0)
    }
    close_to_parent(self);                                                         // 006F6F57..79
    if (self->level_up != 0) kotor_fastcall_006ee5b0(self->parent, nullptr);       // 006F6F7C..87 (L15-F trap)
    else kotor_m15c_custom_ability_creation_completed_006ef500(self->parent, nullptr);   // 006F6F8C
}
// ======================================================================== 006F6FA0 SetDescription(strref) (RET 4)
void __fastcall kotor_m15d_abilities_set_description_006f6fa0(AbilitiesPanel* self, void*, Word strref) noexcept(false) {
    struct StrRes { unsigned char text[8]; Ref sound; Word duration; } res;        // [esp+1Ch] / [esp+24h]
    StartupString* const text = ::new (static_cast<void*>(res.text)) StartupString();   // 006F6FBC..C0 (005B3190)
    try {
        c2da_default_ref_00405ed0(&res.sound, nullptr);                            // 006F6FC5..D1
        tlk_fetch_0041e550(kotor_cell_tlk_007a3a08, nullptr, strref, &res, k_tlk_flag_0073ecd0);   // 006F6FD6..F6
        gui_load_text_params_set_text_00415e00(at(&self->desc_item, kLabelText), nullptr, text);   // 006F6FFB..7006 (+E34)
        gui_load_listbox_clear_items_00419a90(&self->lb_desc, nullptr);            // 006F700B..10
        Extent extent = self->desc_item.extent;                                    // 006F7015..36 (+D50)
        gui_load::Text* const text_object = &self->desc_item.text;                 // 006F7041 (+E1C)
        if (text->BorrowedLength005e5790() > 0)                                    // 006F702E..3F
            extent.height = static_cast<I32>(vt<TextIdealView>(text_object)->ideal_height(text_object, nullptr));   // 006F7049..4B
        else
            extent.height = gui_load_text_font_height_00414ee0(text_object, nullptr);   // 006F7050
        Label* const item = &self->desc_item;                                      // 006F7059 (+D4C)
        item->vtbl->set_extent(item, nullptr, &extent);                            // 006F7063..68 vslot +04
        Control* items[1] = {item};                                                // 006F707A (argument slot)
        gui_m48_listbox_add_controls_0041c2c0(&self->lb_desc, nullptr, items, 1, 0, 0, 0);   // 006F706B..7E
        gui_load_listbox_set_active_control_0041c160(&self->lb_desc, nullptr, item, 0);       // 006F7083..88
    } catch (...) {
        text->~StartupString();
        throw;
    }
    text->~StartupString();                                                        // 006F708D..99 (005E5C20)
}
// ======================================================================== 006F7330 OnAcceptButton (RET)
void __fastcall kotor_m15d_abilities_on_accept_006f7330(AbilitiesPanel* self, void*) noexcept(false) {
    if (self->points > 0) {                                                        // 006F7333..3B: unspent points
        show_message(self, 0xBC59);                                                // 006F733D..7B
        return;
    }
    kotor_m15d_abilities_complete_panel_006f6e70(self, nullptr);                   // 006F7383..86 (jmp)
}
// ======================================================================== 006F7390 OnRecommendButton (RET)
void __fastcall kotor_m15d_abilities_on_recommend_006f7390(AbilitiesPanel* self, void*) noexcept(false) {
    void* const stats = stats_of(self->creature);                                  // 006F73C4..C7 / 006F7451..54
    const U8 class_id = kotor_m15b_creature_stats_get_class_00647730(stats, nullptr,
        static_cast<U8>(ld<U8>(stats, 0x32) - 1));                                 // 006F73CD..DB / 006F745A..68
    const unsigned char* const cls = at(rp(rules(), 0xAC), static_cast<unsigned>(class_id) * 0x1A0u);   // 006F73E0..F5
    if (self->level_up != 0) {                                                     // 006F73B3..BE: +1 to the primary ability
        switch (ld<U8>(cls, 0x17A)) {                                              // 006F73F7..407 (table 006F75E8)
        case 0: ++value_at(self, 0); break;                                        // 006F740E STR
        case 1: ++value_at(self, 1); break;                                        // 006F743B DEX
        case 2: ++value_at(self, 2); break;                                        // 006F7446 CON
        case 3: ++value_at(self, 4); break;                                        // 006F7419 INT
        case 4: ++value_at(self, 3); break;                                        // 006F7430 WIS
        case 5: value_at(self, 5) = value_at(self, 5) + 1; break;                  // 006F7424..2B / 006F74CC CHA
        default: break;
        }
    } else {                                                                       // 006F7451..CC: class recommended scores
        value_at(self, 0) = ld<U8>(cls, 0x17B);                                    // STR (006F7484..8B)
        value_at(self, 1) = ld<U8>(cls, 0x17C);                                    // DEX (006F7491..98)
        value_at(self, 2) = ld<U8>(cls, 0x17D);                                    // CON (006F749E..A5)
        value_at(self, 3) = ld<U8>(cls, 0x17F);                                    // WIS (006F74AB..B2)
        value_at(self, 4) = ld<U8>(cls, 0x17E);                                    // INT (006F74B8..BF)
        value_at(self, 5) = ld<U8>(cls, 0x180);                                    // CHA (006F74C5..CC)
    }
    self->points = 0;                                                              // 006F74D7
    set_text_int(at(&self->lbl_remaining, kLabelText), 0);                         // 006F74D2..502 (+7F4)
    for (unsigned i = 0; i < 6; ++i) {                                             // 006F7507..B2
        set_text_int(at(point_button(self, i), kButtonText), value_at(self, i));   // 006F7518..48 (+19F8)
        if (self->level_up != 0) {                                                 // 006F754D..55
            set_bit1(minus_button(self, i), value_at(self, i) != base_at(self, i));   // 006F7557..6E
        } else {
            set_bit1(minus_button(self, i), value_at(self, i) != 8);               // 006F7572..87
            set_bit1(plus_button(self, i), value_at(self, i) != 0x12);             // 006F7589..A2
        }
    }
    reinterpret_cast<AbilitiesHandlers*>(self)->OnEnterPointsButton_006f70e0(
        reinterpret_cast<kih::Control*>(point_button(self, static_cast<unsigned>(self->current))));   // 006F75B8..CE
}
// ======================================================================== 006F7600 ctor (thiscall, RET 0Ch)
void* __fastcall kotor_m15d_abilities_construct_006f7600(void* memory, void*, void* manager, void* creature,
                                                          std::uint32_t level_up) noexcept(false) {
    AbilitiesPanel* const self = static_cast<AbilitiesPanel*>(memory);
    gui_load_panel_construct_0040b570(self, nullptr, static_cast<Object*>(manager));   // 006F761E..2B
    int built = 0;
    try {
        gui_lb::set_vptr(self, &gui_m15d_abilities_methods_00759c68);             // 006F763B (759C68)
        gui_load_listbox_construct_0041be40(&self->lb_desc, nullptr);  built = 1;  // 006F7630..41
        for (int i = 0; i < 11; ++i) { gui_load_label_construct_0041acd0(label_at(self, i), nullptr); built = 2 + i; }   // 006F7646..F3
        vector_construct(self->ability_labels, 6, gui_load_label_construct_0041acd0, gui_load_label_destruct_00419d10);  built = 0x0D;   // 006F76F8..715
        vector_construct(self->point_buttons, 6, gui_load_button_construct_0041c0f0, gui_load_button_destruct_00419da0); built = 0x0E;   // 006F771A..37
        gui_load_button_construct_0041c0f0(&self->btn_accept, nullptr);       built = 0x0F;   // 006F773C..47
        gui_load_button_construct_0041c0f0(&self->btn_back, nullptr);         built = 0x10;   // 006F774C..57
        gui_load_button_construct_0041c0f0(&self->btn_recommended, nullptr);  built = 0x11;   // 006F775C..67
        vector_construct(self->plus_buttons, 6, gui_load_button_construct_0041c0f0, gui_load_button_destruct_00419da0);  built = 0x12;   // 006F776C..89
        vector_construct(self->minus_buttons, 6, gui_load_button_construct_0041c0f0, gui_load_button_destruct_00419da0); built = 0x13;   // 006F778E..AB
        StartupString unused_string;                                               // [esp+2Ch] (005B3190; state 13h)
        Ref unused_ref;
        c2da_default_ref_00405ed0(&unused_ref, nullptr);                           // [esp+34h] (00405ED0; state 14h)
        self->level_up = level_up;                                                 // 006F77CC..D2
        self->creature = creature;                                                 // 006F77CF / 006F77E7
        {
            Ref layout;
            layout.construct_00406d80("ABCHRGEN");                                // 006F77DA..EA (00759E28)
            gui_load_panel_start_layout_0040a680(self, nullptr, &layout);          // 006F77EF..F6
        }
        init_control(self, &self->lbl_main_title, "MAIN_TITLE_LBL");              // 006F77FB..825
        if (level_up != 0)                                                         // 006F782A..2F
            gui_load_text_params_set_strref_00415e50(at(&self->lbl_main_title, kLabelText), nullptr, 0x42F);   // 006F7831..3C (+434)
        init_control(self, &self->lbl_sub_title, "SUB_TITLE_LBL");                // 006F7841..71
        init_control(self, &self->lbl_remaining_box, "REMAINING_BOX_LBL");        // 006F7876..A6
        init_control(self, &self->lbl_remaining, "REMAINING_SELECTIONS_LBL");     // 006F78AB..DB
        init_control(self, &self->lbl_selections, "SELECTIONS_REMAINING_LBL");    // 006F78E0..910
        init_control(self, &self->lbl_desc, "DESC_LBL");                          // 006F7915..45
        init_control(self, &self->lbl_cost, "COST_LBL");                          // 006F794A..7A
        init_control(self, &self->lbl_cost_points, "COST_POINTS_LBL");            // 006F797F..AF
        init_control(self, &self->lb_desc, "LB_DESC");                            // 006F79B4..E1
        self->lb_desc.u2c1 = 1;                                                    // 006F79EF (+32D)
        init_control(self, &self->lbl_ability_mod, "LBL_ABILITY_MOD");            // 006F79E6..A1D (00759E18)
        init_control(self, &self->lbl_modifier, "LBL_MODIFIER");                  // 006F7A22..52 (00759E08)
        init_control(self, &self->btn_accept, "BTN_ACCEPT");                      // 006F7A57..87
        init_control(self, &self->btn_recommended, "BTN_RECOMMENDED");            // 006F7A8C..BC
        init_control(self, &self->btn_back, "BTN_BACK");                          // 006F7AC1..F1
        struct AbilityTags { const char* points; const char* label; const char* minus; const char* plus; Word description; };
        static const AbilityTags tags[6] = {
            {"STR_POINTS_BTN", "STR_LBL", "STR_MINUS_BTN", "STR_PLUS_BTN", 0xDE},  // 006F7AF6..BC5; 006F7BD3 [3DA0]
            {"DEX_POINTS_BTN", "DEX_LBL", "DEX_MINUS_BTN", "DEX_PLUS_BTN", 0xDF},  // 006F7BCA..CA3; 006F7CB1 [3DA4]
            {"CON_POINTS_BTN", "CON_LBL", "CON_MINUS_BTN", "CON_PLUS_BTN", 0xE0},  // 006F7CA8..D81; 006F7D8F [3DA8]
            {"WIS_POINTS_BTN", "WIS_LBL", "WIS_MINUS_BTN", "WIS_PLUS_BTN", 0xE1},  // 006F7D86..E5F; 006F7E6D [3DAC]
            {"INT_POINTS_BTN", "INT_LBL", "INT_MINUS_BTN", "INT_PLUS_BTN", 0xE2},  // 006F7E64..F3D; 006F7F4B [3DB0]
            {"CHA_POINTS_BTN", "CHA_LBL", "CHA_MINUS_BTN", "CHA_PLUS_BTN", 0xE3},  // 006F7F42..801B; 006F8022 [3DB4]
        };
        for (unsigned i = 0; i < 6; ++i) {
            // Description strref i is stored after ability i's four controls are loaded (just before the next
            // ability's points-button string, or before StopLoadFromLayout for CHA).
            init_control(self, point_button(self, i), tags[i].points);
            init_control(self, ability_label(self, i), tags[i].label);
            init_control(self, minus_button(self, i), tags[i].minus);
            init_control(self, plus_button(self, i), tags[i].plus);
            st<Word>(self, 0x3DA0u + 4u * i, tags[i].description);
        }
        gui_load_panel_stop_layout_0040b8f0(self, nullptr);                        // 006F8020..2C
        if (self->level_up != 0) {                                                 // 006F8031..39
            void* const server_creature = kotor_m15a_creature_get_server_creature_0060fb20(self->creature, nullptr);   // 006F803F..42
            if (server_creature != nullptr) {                                      // 006F8047..49: server stats [+A74]
                value_at(self, 0) = ld<U8>(rp(server_creature, 0xA74), 0xE9);      // 006F804B..5A STR
                value_at(self, 1) = ld<U8>(rp(server_creature, 0xA74), 0xEB);      // 006F8060..73 DEX
                value_at(self, 2) = ld<U8>(rp(server_creature, 0xA74), 0xED);      // 006F8079..86 CON
                value_at(self, 3) = ld<U8>(rp(server_creature, 0xA74), 0xF1);      // 006F808C..99 WIS
                value_at(self, 4) = ld<U8>(rp(server_creature, 0xA74), 0xEF);      // 006F809F..AC INT
                value_at(self, 5) = ld<U8>(rp(server_creature, 0xA74), 0xF3);      // 006F80B2..BF CHA
            } else {                                                               // 006F80C7..12C: client stats bases
                value_at(self, 0) = ld<U8>(stats_of(self->creature), 0x3A);
                value_at(self, 1) = ld<U8>(stats_of(self->creature), 0x3B);
                value_at(self, 2) = ld<U8>(stats_of(self->creature), 0x3C);
                value_at(self, 3) = ld<U8>(stats_of(self->creature), 0x3E);
                value_at(self, 4) = ld<U8>(stats_of(self->creature), 0x3D);
                value_at(self, 5) = ld<U8>(stats_of(self->creature), 0x3F);
            }
            for (unsigned i = 0; i < 6; ++i) base_at(self, i) = value_at(self, i); // 006F8132..70
        } else {                                                                   // 006F8178..DD
            value_at(self, 0) = ld<U8>(stats_of(self->creature), 0x3A);            // STR base
            value_at(self, 1) = ld<U8>(stats_of(self->creature), 0x3B);            // DEX base
            value_at(self, 2) = ld<U8>(stats_of(self->creature), 0x3C);            // CON base
            value_at(self, 3) = ld<U8>(stats_of(self->creature), 0x3E);            // WIS base
            value_at(self, 4) = ld<U8>(stats_of(self->creature), 0x3D);            // INT base
            value_at(self, 5) = ld<U8>(stats_of(self->creature), 0x3F);            // CHA base
        }
        for (unsigned i = 0; i < 6; ++i) {                                         // 006F81E3..82FE
            Button* const points = point_button(self, i);
            gui_load_control_add_event_0041ab20(points, nullptr, 0, event_target(self),
                handler(&AbilitiesHandlers::OnEnterPointsButton_006f70e0));        // 006F8200..0E
            gui_load_control_add_event_0041ab20(points, nullptr, 1, event_target(self),
                handler(&AbilitiesHandlers::OnExitPointsButton_006f6e40));         // 006F8213..21
            set_text_int(at(points, kButtonText), value_at(self, i));              // 006F8226..53 (+19F8)
            points->u58 = i;                                                       // 006F8258..5C ([edi-1A68h])
            if (self->level_up != 0) {                                             // 006F8262..6A
                minus_button(self, i)->flags &= ~2u;                               // 006F826C
            } else {
                set_bit1(minus_button(self, i), value_at(self, i) != 8);           // 006F8271..8B
                set_bit1(plus_button(self, i), value_at(self, i) != 0x12);         // 006F828D..A6
            }
            minus_button(self, i)->flags &= 0xFFFFFFFBu;                           // 006F82AC
            plus_button(self, i)->flags &= 0xFFFFFFFBu;                            // 006F82AF..C3
            gui_load_control_add_event_0041ab20(minus_button(self, i), nullptr, 0x27, event_target(self),
                kotor_m15d::event_func(&PanelStepThunks::OnMinusButtonPushed_0067cb40));   // 006F82B5..C9 (0067CB40)
            gui_load_control_add_event_0041ab20(plus_button(self, i), nullptr, 0x27, event_target(self),
                kotor_m15d::event_func(&PanelStepThunks::OnPlusButtonPushed_0067cb50));    // 006F82CE..DC (0067CB50)
        }
        self->btn_accept.flags &= 0xFFFFFFFBu;                                     // 006F8304..1F
        gui_load_control_add_event_0041ab20(&self->btn_accept, nullptr, 0x27, event_target(self),
            kotor_m1_shared::event_func(&kotor_m1_shared::PanelEventThunks::AcceptButtonCallback_00624ba0));   // 006F830A..22
        self->btn_recommended.flags &= 0xFFFFFFFBu;                                // 006F8327..3D
        gui_load_control_add_event_0041ab20(&self->btn_recommended, nullptr, 0x27, event_target(self),
            kotor_m15_l0::event_func(&kotor_m15_l0::PanelEventThunks15::OnYButtonPressed_00644720));       // 006F832D..40
        self->btn_back.flags &= 0xFFFFFFFBu;                                       // 006F8345..5B
        gui_load_control_add_event_0041ab20(&self->btn_back, nullptr, 0x27, event_target(self),
            kotor_m1_shared::event_func(&kotor_m1_shared::PanelEventThunks::OnBButtonPressed_00624bb0));    // 006F834B..5E
        if (self->level_up != 0) {                                                 // 006F8363..70
            const I32 level = static_cast<I32>(static_cast<U8>(
                kotor_m15b_levelup_stats_get_level_00648430(stats_of(self->creature), nullptr)));   // 006F8372..7D
            self->points = (level % 4 == 0) ? 1 : 0;                               // 006F8380..9B (signed mod 4)
            String text("Points Remaining");                                       // 006F83A1..AA (00759CD4)
            gui_load_text_params_set_text_00415e00(at(&self->lbl_selections, kLabelText), nullptr, &text);   // 006F83AF..BF (+934)
        } else {
            self->points = ld<U8>(stats_of(self->creature), 0xAC);                 // 006F83D3..E0 (ability points)
        }
        set_text_int(at(&self->lbl_remaining, kLabelText), self->points);          // 006F83E6..413 (+7F4)
        {
            Control* const proto = self->lb_desc.proto;                            // 006F8418 (+338)
            vt<LabelInitView>(static_cast<Object*>(&self->desc_item))->initialize98(
                &self->desc_item, nullptr, word_of(&proto->extent), word_of(proto), 0x3F800000u);   // 006F841E..34
        }
        gui_load_panel_set_active_control_0040a630(self, nullptr, point_button(self, 0), 0);   // 006F843A..44
        self->current = 0;                                                         // 006F844D
    } catch (...) {
        unwind_abilities(self, built);
        throw;
    }
    return self;                                                                   // 006F8449..62 (005E5C20 of [esp+2Ch])
}
// ======================================================================== 006F8480 OnMinusButton (RET)
void __fastcall kotor_m15d_abilities_on_minus_006f8480(AbilitiesPanel* self, void*) noexcept(false) {
    const unsigned cur = static_cast<unsigned>(self->current);                     // 006F84A3
    const I32 value = value_at(self, cur);                                         // 006F84A9
    if (self->level_up != 0) {                                                     // 006F849B..B1
        if (value == base_at(self, cur)) return;                                   // 006F84B3..BA
    } else if (value == 8) {                                                       // 006F84C4..C7
        show_message(self, 0xA4C4);                                                // 006F84C9..506
        return;
    }
    if (value > 8) {                                                               // 006F851C..1F
        self->points -= kotor_m15d_abilities_refund_006f6b60(self, nullptr, value);   // 006F8521..29
        --value_at(self, cur);                                                     // 006F852F
    }
    StringSlot value_text;                                                         // [esp+10h] (state 0)
    resman_string_from_int_005e5bc0(value_text.raw(), nullptr, value_at(self, static_cast<unsigned>(self->current)));   // 006F8536..48
    value_text.live = true;
    gui_load_text_params_set_text_00415e00(at(point_button(self, static_cast<unsigned>(self->current)), kButtonText),
                                           nullptr, value_text.get());             // 006F854D..6D
    StringSlot points_text;                                                        // [esp+8] (state 1)
    resman_string_from_int_005e5bc0(points_text.raw(), nullptr, self->points);     // 006F8572..7D
    points_text.live = true;
    gui_load_text_params_set_text_00415e00(at(&self->lbl_remaining, kLabelText), nullptr, points_text.get());   // 006F8582..92
    reinterpret_cast<AbilitiesHandlers*>(self)->OnEnterPointsButton_006f70e0(
        reinterpret_cast<kih::Control*>(point_button(self, static_cast<unsigned>(self->current))));   // 006F8597..AD
    const unsigned now = static_cast<unsigned>(self->current);                     // 006F85BC / 006F85D6
    bool disable_minus;
    if (self->level_up != 0) disable_minus = value_at(self, now) == base_at(self, now);   // 006F85B2..D0
    else disable_minus = value_at(self, now) == 8;                                 // 006F85D6..E4
    if (disable_minus) {
        minus_button(self, static_cast<unsigned>(self->current))->flags &= ~2u;    // 006F85E6..FD
        set_active0(minus_button(self, static_cast<unsigned>(self->current)));     // 006F8600..1C vslot +40(0)
    }
    plus_button(self, static_cast<unsigned>(self->current))->flags |= 2u;          // 006F861F..3C
}                                                                                  // 006F863F..59 (005E5C20 x2)
// ======================================================================== 006F8670 OnPlusButton (RET)
void __fastcall kotor_m15d_abilities_on_plus_006f8670(AbilitiesPanel* self, void*) noexcept(false) {
    const I32 points = self->points;                                               // 006F868C (ebp)
    if (points <= 0) return;                                                       // 006F8692..94
    const Word level_up = self->level_up;                                          // 006F869B (edi)
    if (level_up == 0 && value_at(self, static_cast<unsigned>(self->current)) == 0x12) {   // 006F86A1..B3
        show_message(self, 0xA4C5);                                                // 006F86B5..F3
        return;
    }
    I32& value = value_at(self, static_cast<unsigned>(self->current));             // 006F870C..19 (edx)
    const I32 cost = inline_cost(self, value);                                     // 006F871B..38
    if (points - cost >= 0 && (level_up != 0 || value < 0x12)) {                   // 006F873A..47
        self->points = points - kotor_m15d_abilities_point_cost_006f6bb0(self, nullptr, value);   // 006F8749..53
        ++value;                                                                   // 006F8759
    }
    StringSlot value_text;                                                         // [esp+18h] (state 0)
    resman_string_from_int_005e5bc0(value_text.raw(), nullptr, value_at(self, static_cast<unsigned>(self->current)));   // 006F875B..6D
    value_text.live = true;
    gui_load_text_params_set_text_00415e00(at(point_button(self, static_cast<unsigned>(self->current)), kButtonText),
                                           nullptr, value_text.get());             // 006F8772..90
    StringSlot points_text;                                                        // [esp+10h] (state 1)
    resman_string_from_int_005e5bc0(points_text.raw(), nullptr, self->points);     // 006F8795..A0
    points_text.live = true;
    gui_load_text_params_set_text_00415e00(at(&self->lbl_remaining, kLabelText), nullptr, points_text.get());   // 006F87A5..B5
    reinterpret_cast<AbilitiesHandlers*>(self)->OnEnterPointsButton_006f70e0(
        reinterpret_cast<kih::Control*>(point_button(self, static_cast<unsigned>(self->current))));   // 006F87BA..D0
    if (self->level_up == 0 && value_at(self, static_cast<unsigned>(self->current)) == 0x12) {   // 006F87D5..EB
        plus_button(self, static_cast<unsigned>(self->current))->flags &= ~2u;     // 006F87ED..804
        set_active0(plus_button(self, static_cast<unsigned>(self->current)));      // 006F8807..22 vslot +40(0)
    }
    minus_button(self, static_cast<unsigned>(self->current))->flags |= 2u;         // 006F8825..42
}                                                                                  // 006F8845..5E (005E5C20 x2)
// ======================================================================== 006F8880 HandleInputEvent(event, value) (RET 8)
// Byte table 006F89B4 / jump table 006F8994 for event-27h in 0..19h: 27h/2Dh accept, 28h/2Eh cancel, 2Ah recommend
// (only with points left), 2Fh/3Fh minus, 30h/40h plus, 39h/3Ah scroll LB_DESC; then CSWGuiPanel::HandleInputEvent.
void __fastcall kotor_m15d_abilities_handle_input_006f8880(AbilitiesPanel* self, void*, I32 event, I32 value) noexcept(false) {
    if (value != 0) {                                                              // 006F8885..8F
        switch (event) {
        case 0x27: case 0x2D:                                                      // 006F890F
            manager_of(self)->PlayGuiSound(0);
            kotor_m15d_abilities_on_accept_006f7330(self, nullptr);
            break;
        case 0x28: case 0x2E:                                                      // 006F88AF
            manager_of(self)->PlayGuiSound(0);
            kotor_m15d_abilities_handle_cancel_006f6de0(self, nullptr);
            break;
        case 0x2A:                                                                 // 006F892F
            manager_of(self)->PlayGuiSound(0);
            if (self->points > 0) kotor_m15d_abilities_on_recommend_006f7390(self, nullptr);   // 006F8939..45
            break;
        case 0x2F: case 0x3F:                                                      // 006F88CF
            manager_of(self)->PlayGuiSound(1);
            kotor_m15d_abilities_on_minus_006f8480(self, nullptr);
            break;
        case 0x30: case 0x40:                                                      // 006F88EF
            manager_of(self)->PlayGuiSound(1);
            kotor_m15d_abilities_on_plus_006f8670(self, nullptr);
            break;
        case 0x39:                                                                 // 006F8975: LB_DESC vslot +3C(31h, 1)
            vt<InputView>(&self->lb_desc)->handle_input_event(&self->lb_desc, nullptr, 0x31, 1);
            break;
        case 0x3A:                                                                 // 006F8959: LB_DESC vslot +3C(32h, 1)
            vt<InputView>(&self->lb_desc)->handle_input_event(&self->lb_desc, nullptr, 0x32, 1);
            break;
        default:
            break;
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, value);            // 006F88C4 / 006F8986 (00409E60)
}
}
