// Lane W4 GUI successor (worker "levelup", fork "lu-panel"): CSWGuiLevelUpPanel (0x1CEC bytes, table 00759568), the
// in-game level-up step panel ("LEVELUPPNL") owned by CSWGuiLevelUpCharGen ([this+68h] = parent, [this+64h] = client
// creature). Transcribed from the original swkotor.exe linear sweep 006EE0D0..006EEF52; each statement cites its PCs.
//   006EE0D0 NewAbilityScores(7 dwords)        thiscall RET 1Ch (mov ecx,[ecx+68h]; jmp 006E8500)  @36
//   006EE0E0 ChangeState(int back)             thiscall RET 4                                       @12
//   006EE350 OnSelectPowersButton(control)     thiscall RET 4                                       @12
//   006EE3D0 OnSelectFeatsButton(control)      thiscall RET 4                                       @12
//   006EE480 OnSelectSkillsButton(control)     thiscall RET 4                                       @12
//   006EE500 OnSelectAbilitiesButton(control)  thiscall RET 4                                       @12
//   006EE580 CancelledByChild()                thiscall RET                                         @8
//   006EE5B0 AbilitySelectionCompleted()       thiscall RET                                         @8
//   006EE5D0 SelectionCompleted()              thiscall RET                                         @8
//   006EE5F0 OnCancelPressed(control)          thiscall RET 4                                       @12
//   006EE640 SetCanCancel(int)                 thiscall RET 4                                       @12
//   006EE6C0 OnPanelAdded()                    thiscall RET (tail jmp CSWGuiPanel::OnPanelAdded)    @8   (slot 48h)
//   006EE720 HandleInputEvent(int, int)        thiscall RET 8                                       @16  (slot 3Ch)
//   006EE780 OnSelectAcceptButton(control)     thiscall RET 4                                       @12
//   006EE7D0 CSWGuiLevelUpPanel(manager, id)   thiscall RET 8, returns this                         @16
//   006EEE70 scalar deleting destructor(flags) thiscall RET 4                                       @12  (slot 00h)
//   006EEE90 ~CSWGuiLevelUpPanel()             thiscall RET                                         @8
// Layout: +6Ch LBL_BG label; +1ACh 5 x CSWGuiLabelHilight (1B4h) "LBL_%d"; +A30h 5 x label (140h) "LBL_NUM%d";
// +1070h 5 x button (1C4h) "BTN_STEPNAME%d"; +1944h BTN_BACK; +1B08h BTN_CANCEL; +1CCCh current step; +1CD0h last
// step; +1CD4h int[5] step enabled (0 abilities, 1 skills, 2 feats, 3 powers, 4 accept); +1CE8h can-cancel.
#include "6030a9ee65-claude_gui_char.hpp"

extern "C" {
// ---- CSWGuiLevelUpCharGen (fork lu-chargen, claude_gui_levelup_chargen.cpp)
void __fastcall kotor_fastcall_006e8500(void* chargen, void*, w4::U32 str, w4::U32 cha, w4::U32 intel, w4::U32 wis,
                                        w4::U32 con, w4::U32 dex, w4::U32 points);              // NewAbilityScores RET 1Ch
void __fastcall kotor_fastcall_006e7f70(void* chargen, void*);                                  // AbilitiesChanged
void __fastcall kotor_fastcall_006e85a0(void* chargen, void*);                                  // ClearAbilities
void __fastcall kotor_fastcall_006e8730(void* chargen, void*);                                  // ClearSkills
void __fastcall kotor_fastcall_006e87e0(void* chargen, void*);                                  // ClearPowers
void __fastcall kotor_fastcall_006e8db0(void* chargen, void*);                                  // ClearFeats
void __fastcall kotor_fastcall_006e8e20(void* chargen, void*);                                  // CancelLevelUp
void __fastcall kotor_fastcall_006e88a0(void* chargen, void*);                                  // BeginPlayGame
// ---- CSWGuiPowersLevelUp (fork lu-powers) / CSWCLevelUpStats (fork lu-crules)
void* __fastcall kotor_fastcall_006f2180(void* self, void*, void* manager, w4::U32 creature_id); // CSWGuiPowersLevelUp RET 8
w4::U8 __fastcall kotor_fastcall_00649070(void* stats, void*);                                  // ComputeNumberKnownSpellsLeft (AL)
// ---- existing sub-panel constructors (objects 0828 / 0829 / 0830)
void* __fastcall kotor_m15d_feats_construct_006f3d60(void* self, void*, void* manager, void* creature, w4::U32 level_up, w4::U32 no_bonus) noexcept(false);
void* __fastcall kotor_m15d_skills_construct_006f51d0(void* self, void*, void* manager, void* creature, w4::U32 level_up) noexcept(false);
void* __fastcall kotor_m15d_abilities_construct_006f7600(void* self, void*, void* manager, void* creature, w4::U32 level_up) noexcept(false);
// ---- CSWCLevelUpStats (0819-m15b-stats.obj)
w4::U8 __fastcall kotor_m15b_levelup_stats_get_level_00648430(void* stats, void*);             // GetLevel (AL)
void __fastcall kotor_m15b_levelup_stats_add_skill_points_per_level_00648a00(void* stats, void*); // AddSkillPointsPerLevel
void __fastcall kotor_m15b_levelup_stats_calc_number_feats_00648fe0(void* stats, void*, w4::U8* normal, w4::U8* bonus); // CalcNumberFeats
void __fastcall kotor_m15b_levelup_stats_add_granted_feats_00649950(void* stats, void*, void* list) noexcept(false); // AddGrantedFeats
// ---- GUI providers
void __fastcall kotor_m15d_skill_info_set_skill_list_006cdfc0(void* box, void*, void* list, w4::U32 flag) noexcept(false); // CSWGuiSkillInfoBox::SetSkillList
void __fastcall kotor_m15_panel_set_visible_0040b9d0(void* panel, void*, w4::U32 visible);      // CSWGuiPanel::SetVisible
void* __fastcall gui_load_label_hilight_construct_0041b5c0(void* self, void*) noexcept(false);  // CSWGuiLabelHilight
void __fastcall gui_m48_label_hilight_destruct_0041b610(void* self, void*);                     // ~CSWGuiLabelHilight
// ---- defined here
void __fastcall kotor_fastcall_006ee0d0(void* self, void*, w4::U32, w4::U32, w4::U32, w4::U32, w4::U32, w4::U32, w4::U32);
void __fastcall kotor_fastcall_006ee0e0(void* self, void*, w4::U32 back);
void __fastcall kotor_fastcall_006ee350(void* self, void*, void* control);
void __fastcall kotor_fastcall_006ee3d0(void* self, void*, void* control);
void __fastcall kotor_fastcall_006ee480(void* self, void*, void* control);
void __fastcall kotor_fastcall_006ee500(void* self, void*, void* control);
void __fastcall kotor_fastcall_006ee580(void* self, void*);
void __fastcall kotor_fastcall_006ee5b0(void* self, void*);
void __fastcall kotor_fastcall_006ee5d0(void* self, void*);
void __fastcall kotor_fastcall_006ee5f0(void* self, void*, void* control);
void __fastcall kotor_fastcall_006ee640(void* self, void*, w4::U32 can_cancel);
void __fastcall kotor_fastcall_006ee6c0(void* self, void*);
void __fastcall kotor_fastcall_006ee720(void* self, void*, w4::U32 event, w4::U32 down);
void __fastcall kotor_fastcall_006ee780(void* self, void*, void* control);
void* __fastcall kotor_fastcall_006ee7d0(void* self, void*, void* manager, w4::U32 creature_id);
void* __fastcall kotor_fastcall_006eee70(void* self, void*, w4::U32 flags);
void __fastcall kotor_fastcall_006eee90(void* self, void*);
}

using namespace gchar;

namespace {
constexpr U32 k_hilight = 0x1AC, k_hilight_stride = 0x1B4;                                     // CSWGuiLabelHilight[5]
constexpr U32 k_number = 0xA30, k_number_stride = 0x140;                                        // CSWGuiLabel[5]
constexpr U32 k_step = 0x1070, k_step_stride = 0x1C4;                                           // CSWGuiButton[5]
constexpr U32 k_back = 0x1944, k_cancel = 0x1B08;
constexpr U32 k_state = 0x1CCC, k_last = 0x1CD0, k_enabled = 0x1CD4, k_can_cancel = 0x1CE8;
inline void* parent(void* self) { return lp(self, 0x68); }
inline void* creature(void* self) { return lp(self, 0x64); }
inline void* levelup_stats(void* self) { return lp(creature(self), 0x2F8); }                     // CSWCCreature +2F8h stats
inline void sh(void* o, U32 off, U16 v) { std::memcpy(static_cast<U8*>(o) + off, &v, 2); }   // mov word ptr
inline void set_color(void* params, const void* color) { gui_load_text_params_set_color_00414e10(params, nullptr, color); }

// Original 00759568 = CSWGuiPanel 0073E010 with slots 00h / 3Ch / 48h replaced.
const Override ovr_level_up_panel[] = {
    {0x00, fn(&kotor_fastcall_006eee70)},   // 006EEE70 CSWGuiLevelUpPanel::~CSWGuiLevelUpPanel (deleting)
    {0x3C, fn(&kotor_fastcall_006ee720)},   // 006EE720 CSWGuiLevelUpPanel::HandleInputEvent
    {0x48, fn(&kotor_fastcall_006ee6c0)},   // 006EE6C0 CSWGuiLevelUpPanel::OnPanelAdded
};
void* table_level_up_panel[27];
bool built_level_up_panel = false;
const void* vt_level_up_panel() {   // original 00759568 (27 slots; base 0073E010 27 slots)
    if (!built_level_up_panel) { vt_build(table_level_up_panel, 27, gui_load_panel_methods_0073e010, 27, ovr_level_up_panel, 3); built_level_up_panel = true; }
    return table_level_up_panel;
}

// BTN_BACK enable/colour shared by ChangeState's tail and SetCanCancel: enabled unless on the last step with no cancel.
U32 update_back_button(void* self) {
    const U32 enable = (li(self, k_state) == li(self, k_last) && lw(self, k_can_cancel) == 0) ? 0u : 1u;
    vcall<void, U32>(at(self, k_back), 0x88, enable);                                          // [btn]+88h(enable)
    set_color(at(self, k_back + 0x16C), enable != 0 ? static_cast<const void*>(borrowed_exit_color_007a23b4)
                                                     : static_cast<const void*>(kotor_m1_color_007a2408));
    return enable;
}
// Step button enable: AddEvent(27h) + AddEvent(2Dh) to handler, mark enabled, number label text = ++counter.
void enable_step(void* self, U32 index, const void* handler, I32 number) {
    void* button = at(self, k_step + index * k_step_stride);
    gui_load_control_add_event_0041ab20(button, nullptr, 0x27, self, handler);
    gui_load_control_add_event_0041ab20(button, nullptr, 0x2D, self, handler);
    sw(self, k_enabled + index * 4u, 1);
    Str text(number);                                                                           // CExoString(int) 005E5BC0
    gui_load_text_params_set_text_00415e00(at(self, k_number + index * k_number_stride + 0xE8), nullptr, text.p());
}
// Step disabled: clear bit 1 (visible) of the button, the hilight and the number label; mark not enabled.
void disable_step(void* self, U32 index) {
    const U32 offs[3] = {k_step + index * k_step_stride + 0x44, k_hilight + index * k_hilight_stride + 0x44,
                         k_number + index * k_number_stride + 0x44};
    for (U32 off : offs) sw(self, off, lw(self, off) & 0xFFFFFFFDu);
    sw(self, k_enabled + index * 4u, 0);
}
// Shared allocate / construct / attach / AddPanel(3, 1) shape of the four OnSelect*Button handlers.
template <class Make> void open_child(void* self, U32 size, Make make) {
    void* raw = live_operator_new(size);
    void* panel = nullptr;
    if (raw != nullptr) {
        auto guard = on_throw([&]() { live_free(raw); });                                      // EH state 0: operator delete
        panel = make(raw);
        guard.armed = false;
    }
    raw_store(panel, 0x68, ad(self));                                                            // [eax+68h] = this (unchecked)
    kotor_fastcall_0040bc70(manager_of(self), nullptr, panel, 3, 1);                             // CSWGuiManager::AddPanel
}
}  // namespace

extern "C" {

// 006EE0D0 CSWGuiLevelUpPanel::NewAbilityScores: mov ecx,[ecx+68h]; jmp CSWGuiLevelUpCharGen 006E8500 (RET 1Ch there).
void __fastcall kotor_fastcall_006ee0d0(void* self, void*, U32 a1, U32 a2, U32 a3, U32 a4, U32 a5, U32 a6, U32 a7) {
    kotor_fastcall_006e8500(parent(self), nullptr, a1, a2, a3, a4, a5, a6, a7);               // 006EE0D0..006EE0D3
}

// 006EE0E0 CSWGuiLevelUpPanel::ChangeState(int back) RET 4.
void __fastcall kotor_fastcall_006ee0e0(void* self, void*, U32 back) {
    I32 step;
    if (back != 0) {                                                                            // 006EE0E3..006EE0EA
        if (li(self, k_state) == li(self, k_last)) {                                            // 006EE0EC..006EE0F8
            if (lw(self, k_can_cancel) == 0) return;                                            // 006EE0FA..006EE102
            kotor_fastcall_006e8e20(parent(self), nullptr);                                     // 006EE108..006EE10B CancelLevelUp
            void* box = pt(lw(pt(frame_R4_005ed690(client_app(), nullptr)), 0x9C));            // 006EE110..006EE11E [gui+9Ch]
            if (frame_ContainsPanel_0040bd70(manager_of(self), nullptr, box) != 0)              // 006EE124..006EE12F
                frame_RemovePanel_0040c830(manager_of(self), nullptr, box);                     // 006EE135..006EE139
            return;                                                                             // 006EE13E..006EE140
        }
        step = li(self, k_state) - 1;                                                           // 006EE143..006EE14D
    } else {
        step = li(self, k_state) + 1;                                                           // 006EE150..006EE156
    }
    while (step >= 0) {                                                                         // 006EE157..006EE162
        if (step >= 5 || li(self, k_enabled + static_cast<U32>(step) * 4u) == 1) break;         // 006EE168..006EE170
        if (back != 0) --step; else ++step;                                                     // 006EE172..006EE180
    }
    if (step >= 0 && step < 5) {                                                                // 006EE182..006EE18D
        switch (step) {                                                                         // 006EE193..006EE198 (table 006EE340)
        case 0: kotor_fastcall_006e85a0(parent(self), nullptr); break;                          // 006EE1A9 ClearAbilities
        case 1: kotor_fastcall_006e8730(parent(self), nullptr); break;                          // 006EE1B3 ClearSkills
        case 2: kotor_fastcall_006e8db0(parent(self), nullptr); break;                          // 006EE1BD ClearFeats
        case 3: kotor_fastcall_006e87e0(parent(self), nullptr); break;                          // 006EE19F ClearPowers
        default: break;                                                                         // step 4 (accept): ja 006EE1C5
        }
        sw(self, k_state, static_cast<U32>(step));                                              // 006EE1C7
        for (U32 i = 0; i < 5; ++i) {                                                           // 006EE1CD..006EE2C4
            void* hilight = at(self, k_hilight + i * k_hilight_stride);
            void* number = at(self, k_number + i * k_number_stride);
            void* button = at(self, k_step + i * k_step_stride);
            if (static_cast<I32>(i) == li(self, k_state)) {                                     // 006EE1F0..006EE1FA
                vcall<void, U32>(number, 0x88, 1);                                              // 006EE1FC..006EE202
                vcall<void, U32>(button, 0x88, 1);                                              // 006EE208..006EE20E
                vcall<void, U32>(hilight, 0x40, 1);                                             // 006EE214..006EE21A
                set_color(at(number, 0xE8), borrowed_enter_color_007a23c0);                     // 006EE21D..006EE228
                set_color(at(button, 0x16C), borrowed_enter_color_007a23c0);                    // 006EE22D..006EE238
                sw(number, 0x120, lw(number, 0x120) | 0x40u);                                   // 006EE23D..006EE24A
                sw(button, 0x1A4, lw(button, 0x1A4) | 0x40u);                                   // 006EE250
            } else {
                vcall<void, U32>(hilight, 0x40, 0);                                             // 006EE258..006EE25E
                sw(number, 0x120, lw(number, 0x120) & 0xFFFFFFBFu);                             // 006EE261..006EE26E
                sw(button, 0x1A4, lw(button, 0x1A4) & 0xFFFFFFBFu);                             // 006EE274..006EE280
                vcall<void, U32>(number, 0x88, 0);                                              // 006EE28C..006EE294
                vcall<void, U32>(button, 0x88, 0);                                              // 006EE29A..006EE2A0
            }
        }
        vcall<void, void*, U32>(self, 0x08, at(self, k_step + lw(self, k_state) * k_step_stride), 0); // 006EE2CA..006EE2E5 SetActiveControl
    }
    update_back_button(self);                                                                   // 006EE2EA..006EE333
}

// 006EE350 CSWGuiLevelUpPanel::OnSelectPowersButton(control) RET 4: new CSWGuiPowersLevelUp (1A10h).
void __fastcall kotor_fastcall_006ee350(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                         // 006EE369..006EE371
    open_child(self, 0x1A10, [&](void* raw) {                                                   // 006EE373..006EE3B9
        return kotor_fastcall_006f2180(raw, nullptr, manager_of(self), lw(creature(self), 4));  // 006EE390..006EE39D
    });
}

// 006EE3D0 CSWGuiLevelUpPanel::OnSelectFeatsButton(control) RET 4: CalcNumberFeats, new CSWGuiFeatsCharGen (1A1Ch).
void __fastcall kotor_fastcall_006ee3d0(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                         // 006EE3E9..006EE3F4
    U8 normal = 0, bonus = 0;                                                                   // 006EE402 [arg slot] / 006EE407 [esp+7]
    kotor_m15b_levelup_stats_calc_number_feats_00648fe0(levelup_stats(self), nullptr, &normal, &bonus); // 006EE3F6..006EE413
    open_child(self, 0x1A1C, [&](void* raw) {                                                   // 006EE418..006EE469
        return kotor_m15d_feats_construct_006f3d60(raw, nullptr, manager_of(self), creature(self), 1,
                                                   normal == 0 ? 1u : 0u);                      // 006EE435..006EE44D
    });
}

// 006EE480 CSWGuiLevelUpPanel::OnSelectSkillsButton(control) RET 4: new CSWGuiSkillsCharGen (49D0h).
void __fastcall kotor_fastcall_006ee480(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                         // 006EE499..006EE4A1
    open_child(self, 0x49D0, [&](void* raw) {                                                   // 006EE4A3..006EE4E8
        return kotor_m15d_skills_construct_006f51d0(raw, nullptr, manager_of(self), creature(self), 1); // 006EE4C0..006EE4CC
    });
}

// 006EE500 CSWGuiLevelUpPanel::OnSelectAbilitiesButton(control) RET 4: new CSWGuiAbilitiesCharGen (3DF4h).
void __fastcall kotor_fastcall_006ee500(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                         // 006EE519..006EE521
    open_child(self, 0x3DF4, [&](void* raw) {                                                   // 006EE523..006EE568
        return kotor_m15d_abilities_construct_006f7600(raw, nullptr, manager_of(self), creature(self), 1); // 006EE540..006EE54C
    });
}

// 006EE580 CSWGuiLevelUpPanel::CancelledByChild() RET.
void __fastcall kotor_fastcall_006ee580(void* self, void*) {
    sw(self, k_state, lw(self, k_state) - 1u);                                                  // 006EE583..006EE58C
    kotor_fastcall_006ee0e0(self, nullptr, 0);                                                  // 006EE58A..006EE592
    kotor_m15_panel_set_visible_0040b9d0(parent(self), nullptr, 1);                             // 006EE597..006EE59C
}

// 006EE5B0 CSWGuiLevelUpPanel::AbilitySelectionCompleted() RET.
void __fastcall kotor_fastcall_006ee5b0(void* self, void*) {
    kotor_m15_panel_set_visible_0040b9d0(parent(self), nullptr, 1);                             // 006EE5B3..006EE5B8
    kotor_fastcall_006e7f70(parent(self), nullptr);                                             // 006EE5BD..006EE5C0 AbilitiesChanged
    kotor_fastcall_006ee0e0(self, nullptr, 0);                                                  // 006EE5C5..006EE5C9
}

// 006EE5D0 CSWGuiLevelUpPanel::SelectionCompleted() RET.
void __fastcall kotor_fastcall_006ee5d0(void* self, void*) {
    kotor_m15_panel_set_visible_0040b9d0(parent(self), nullptr, 1);                             // 006EE5D3..006EE5D8
    kotor_fastcall_006ee0e0(self, nullptr, 0);                                                  // 006EE5DD..006EE5E1
}

// 006EE5F0 CSWGuiLevelUpPanel::OnCancelPressed(control) RET 4 (BTN_CANCEL event 27h).
void __fastcall kotor_fastcall_006ee5f0(void* self, void*, void*) {
    if (lw(self, k_can_cancel) == 0) return;                                                    // 006EE5F3..006EE5FB
    kotor_fastcall_006e87e0(parent(self), nullptr);                                             // 006EE5FD..006EE600 ClearPowers
    kotor_fastcall_006e8db0(parent(self), nullptr);                                             // 006EE605..006EE608 ClearFeats
    kotor_fastcall_006e8730(parent(self), nullptr);                                             // 006EE60D..006EE610 ClearSkills
    kotor_fastcall_006e85a0(parent(self), nullptr);                                             // 006EE615..006EE618 ClearAbilities
    sw(self, k_state, 0);                                                                       // 006EE621
    kotor_fastcall_006ee0e0(self, nullptr, 1);                                                  // 006EE61D..006EE62B
}

// 006EE640 CSWGuiLevelUpPanel::SetCanCancel(int) RET 4.
void __fastcall kotor_fastcall_006ee640(void* self, void*, U32 can_cancel) {
    sw(self, k_can_cancel, can_cancel);                                                         // 006EE654
    update_back_button(self);                                                                   // 006EE647..006EE691
    const U32 cancel = lw(self, k_cancel + 0x44);                                               // 006EE696..006EE6AE BTN_CANCEL bit 1
    sw(self, k_cancel + 0x44, cancel ^ (((lw(self, k_can_cancel) << 1) ^ cancel) & 2u));
}

// 006EE6C0 CSWGuiLevelUpPanel::OnPanelAdded() (slot 48h).
void __fastcall kotor_fastcall_006ee6c0(void* self, void*) {
    sw(self, k_state, 0xFFFFFFFFu);                                                             // 006EE6C5
    kotor_fastcall_006ee0e0(self, nullptr, 0);                                                  // 006EE6C3..006EE6CF
    sw(self, k_last, lw(self, k_state));                                                        // 006EE6D4..006EE6DA
    if (lw(self, k_can_cancel) == 0) {                                                          // 006EE6E0..006EE6E8
        vcall<void, U32>(at(self, k_back), 0x88, 0);                                            // 006EE6EA..006EE6F8
        set_color(at(self, k_back + 0x16C), kotor_m1_color_007a2408);                           // 006EE6FE..006EE709
        sw(self, k_cancel + 0x44, lw(self, k_cancel + 0x44) & 0xFFFFFFFDu);                     // 006EE70E
    }
    gui_load_panel_on_added_0040b870(self, nullptr);                                            // 006EE715..006EE718 jmp
}

// 006EE720 CSWGuiLevelUpPanel::HandleInputEvent(int event, int down) RET 8 (slot 3Ch).
void __fastcall kotor_fastcall_006ee720(void* self, void*, U32 event, U32 down) {
    if (down != 0) {                                                                            // 006EE72B..006EE72F
        if (event == 0x28 || event == 0x2E) {                                                   // 006EE731..006EE73E
            kotor_fastcall_0040a140(manager_of(self), nullptr, 0);                              // 006EE740..006EE745 PlayGuiSound(0)
            kotor_fastcall_006ee0e0(self, nullptr, 1);                                          // 006EE74A..006EE74E
        } else if (event == 0x2D) {
            kotor_fastcall_0040a140(manager_of(self), nullptr, 0);                              // 006EE762..006EE767
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, down);                           // 006EE753..006EE757 / 006EE76C..006EE770
}

// 006EE780 CSWGuiLevelUpPanel::OnSelectAcceptButton(control) RET 4.
void __fastcall kotor_fastcall_006ee780(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                         // 006EE787..006EE78C
    if (lw(self, 0x1CDC) == 0) {                                                                // 006EE78E..006EE796 (no feat step)
        kotor_m15b_levelup_stats_add_granted_feats_00649950(levelup_stats(self), nullptr, nullptr); // 006EE798..006EE7A3
        kotor_m15_panel_set_visible_0040b9d0(parent(self), nullptr, 1);                         // 006EE7A8..006EE7AD
        kotor_fastcall_006ee0e0(self, nullptr, 0);                                              // 006EE7B2..006EE7B6
    }
    kotor_fastcall_006e88a0(parent(self), nullptr);                                             // 006EE7BB..006EE7BE BeginPlayGame
}

// 006EE7D0 CSWGuiLevelUpPanel::CSWGuiLevelUpPanel(CSWGuiManager*, ulong creature_id) RET 8.
void* __fastcall kotor_fastcall_006ee7d0(void* self, void*, void* manager, U32 creature_id) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                  // 006EE7F6..006EE7FB
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);
    set_vptr(self, vt_level_up_panel());                                                        // 006EE80D (00759568)
    label(self, 0x6C, u);                                                                       // 006EE800..006EE813
    vector_member(at(self, k_hilight), k_hilight_stride, 5, &gui_load_label_hilight_construct_0041b5c0,
                  &gui_m48_label_hilight_destruct_0041b610, u);                                 // 006EE818..006EE835
    vector_member(at(self, k_number), k_number_stride, 5, &gui_load_label_construct_0041acd0,
                  &gui_load_label_destruct_00419d10, u);                                        // 006EE83A..006EE857
    vector_member(at(self, k_step), k_step_stride, 5, &gui_load_button_construct_0041c0f0,
                  &gui_load_button_destruct_00419da0, u);                                       // 006EE85C..006EE879
    button(self, k_back, u);                                                                    // 006EE87E..006EE889
    button(self, k_cancel, u);                                                                  // 006EE88E..006EE899
    U32 name[2]; exo_default(name); u.push(&frame_LogStringDtor_005e5c20, name);                // 006EE89E..006EE8A7 [esp+38h]
    sw(self, 0x18, ad(kotor_fastcall_005eda40(client_app(), nullptr)));                         // 006EE8AC..006EE8BF GetSWGuiManager
    sw(self, 0x64, frame_R4_005ed5b0(client_app(), nullptr, creature_id));                      // 006EE8C2..006EE8D4
    sw(self, k_can_cancel, 0);                                                                  // 006EE8E2
    start_layout(self, "LEVELUPPNL");                                                           // 006EE8D9..006EE8F8 (0075955C)
    init_control(self, 0x6C, "LBL_BG");                                                         // 006EE8FD..006EE928 (00753468)
    for (I32 i = 1; i <= 5; ++i) {                                                              // 006EE92D..006EE9FC
        const U32 k = static_cast<U32>(i - 1);
        void* hilight = at(self, k_hilight + k * k_hilight_stride);
        frame_LogFormat_005e5680(name, "LBL_%d", i);                                            // 006EE950..006EE95B (00759554)
        gui_load_panel_init_control_0040b930(self, nullptr, hilight, name, 1);                  // 006EE963..006EE973
        gui_load_border_params_set_pulsing_00414d00(at(hilight, 0x154), nullptr, 1, 0, 1);       // 006EE978..006EE980
        frame_LogFormat_005e5680(name, "BTN_STEPNAME%d", i);                                    // 006EE985..006EE990 (00759544)
        gui_load_panel_init_control_0040b930(self, nullptr, at(self, k_step + k * k_step_stride), name, 1); // 006EE995..006EE9A6
        frame_LogFormat_005e5680(name, "LBL_NUM%d", i);                                         // 006EE9AB..006EE9B6 (00759538)
        gui_load_panel_init_control_0040b930(self, nullptr, at(self, k_number + k * k_number_stride), name, 1); // 006EE9BB..006EE9CC
    }
    init_control(self, k_back, "BTN_BACK");                                                     // 006EEA02..006EEA33 (007548AC)
    init_control(self, k_cancel, "BTN_CANCEL");                                                 // 006EEA38..006EEA69 (0074FD90)
    stop_layout(self);                                                                          // 006EEA6E..006EEA70
    I32 number = 0;                                                                             // edi (0 reloaded from [esp+28h] when skipped)
    // step 0: abilities on every fourth level (level % 4 == 0)
    if ((kotor_m15b_levelup_stats_get_level_00648430(levelup_stats(self), nullptr) & 3u) == 0) { // 006EEA75..006EEA92
        number = 1;                                                                             // 006EEAB8
        enable_step(self, 0, fn(&kotor_fastcall_006ee500), number);                             // 006EEA94..006EEAEF
    } else {
        disable_step(self, 0);                                                                  // 006EEAFB..006EEB1E
    }
    // step 1: skills when AddSkillPointsPerLevel leaves points ([stats+AEh])
    sh(levelup_stats(self), 0xAE, 0);                                                           // 006EEB28..006EEB31
    kotor_m15b_levelup_stats_add_skill_points_per_level_00648a00(levelup_stats(self), nullptr); // 006EEB3A..006EEB43
    if (lh(levelup_stats(self), 0xAE) != 0) {                                                   // 006EEB48..006EEB5A (movzx; jle)
        ++number;                                                                               // 006EEB80
        enable_step(self, 1, fn(&kotor_fastcall_006ee480), number);                             // 006EEB5C..006EEBB3
    } else {
        disable_step(self, 1);                                                                  // 006EEBBF..006EEBD1
    }
    // step 2: feats when CalcNumberFeats grants any; otherwise granted feats go to the skill-info box
    U8 normal = 0, bonus = 0;                                                                   // 006EEBDE [esp+0Fh] / 006EEBE3 [esp+0Eh]
    kotor_m15b_levelup_stats_calc_number_feats_00648fe0(levelup_stats(self), nullptr, &normal, &bonus); // 006EEBE8..006EEBF8
    if (static_cast<I32>(bonus) + static_cast<I32>(normal) > 0) {                               // 006EEBFD..006EEC0B
        ++number;                                                                               // 006EEC31
        enable_step(self, 2, fn(&kotor_fastcall_006ee3d0), number);                             // 006EEC0D..006EEC64
    } else {
        disable_step(self, 2);                                                                  // 006EEC73..006EEC8F
        PtrList granted;                                                                        // 006EEC95..006EEC9D {0,0,0} (EH state 0Eh)
        kotor_m15b_levelup_stats_add_granted_feats_00649950(levelup_stats(self), nullptr, &granted); // 006EECA1..006EECB4
        if (granted.size > 0) {                                                                 // 006EECB9..006EECBF
            void* box = pt(lw(pt(frame_R4_005ed690(client_app(), nullptr)), 0x9C));            // 006EECC1..006EECCE
            kotor_m15d_skill_info_set_skill_list_006cdfc0(box, nullptr, &granted, 1);           // 006EECD4..006EECDD
            gui_load_text_params_set_strref_00415e50(at(box, 0x42C), nullptr, 0xA512);          // 006EECE2..006EECED
            kotor_fastcall_0040bc70(manager_of(self), nullptr, box, 1, 1);                      // 006EECF2..006EECFA AddPanel
        }
    }                                                                                           // 006EED04..006EED0E _free(list.data)
    // step 3: powers when spells remain to choose
    if (kotor_fastcall_00649070(levelup_stats(self), nullptr) != 0) {                           // 006EED16..006EED29
        ++number;                                                                               // 006EED4F
        enable_step(self, 3, fn(&kotor_fastcall_006ee350), number);                             // 006EED2B..006EED82
    } else {
        disable_step(self, 3);                                                                  // 006EED89..006EED9B
    }
    // step 4: accept, always
    ++number;                                                                                   // 006EEDC9
    enable_step(self, 4, fn(&kotor_fastcall_006ee780), number);                                 // 006EEDA5..006EEDFC
    sw(self, 0x44, lw(self, 0x44) | 0x60u);                                                     // 006EEE01
    sw(self, k_back + 0x44, lw(self, k_back + 0x44) & 0xFFFFFFFBu);                             // 006EEE05..006EEE20
    add_event(self, k_back, 0x27, self, on_b());                                                // 006EEE11..006EEE23 (00624BB0)
    sw(self, k_cancel + 0x44, lw(self, k_cancel + 0x44) & 0xFFFFFFFBu);                         // 006EEE28..006EEE3E
    add_event(self, k_cancel, 0x27, self, fn(&kotor_fastcall_006ee5f0));                        // 006EEE34..006EEE41
    u.dismiss();
    frame_LogStringDtor_005e5c20(name, nullptr);                                                // 006EEE46..006EEE4F
    return self;
}

// 006EEE90 CSWGuiLevelUpPanel::~CSWGuiLevelUpPanel() (no vptr store; members in reverse order, then the panel).
void __fastcall kotor_fastcall_006eee90(void* self, void*) {
    gui_load_button_destruct_00419da0(at(self, k_cancel), nullptr);                             // 006EEEAD..006EEEBB
    gui_load_button_destruct_00419da0(at(self, k_back), nullptr);                               // 006EEEC0..006EEECB
    vector_destruct(at(self, k_step), k_step_stride, 5, &gui_load_button_destruct_00419da0);    // 006EEED0..006EEEE8
    vector_destruct(at(self, k_number), k_number_stride, 5, &gui_load_label_destruct_00419d10); // 006EEEED..006EEF05
    vector_destruct(at(self, k_hilight), k_hilight_stride, 5, &gui_m48_label_hilight_destruct_0041b610); // 006EEF0A..006EEF22
    gui_load_label_destruct_00419d10(at(self, 0x6C), nullptr);                                  // 006EEF27..006EEF2F
    gui_load_panel_destruct_0040cf70(self, nullptr);                                            // 006EEF34..006EEF3E
}
// 006EEE70 scalar deleting destructor (flags) RET 4.
void* __fastcall kotor_fastcall_006eee70(void* self, void*, U32 flags) { return deleting(self, &kotor_fastcall_006eee90, flags); }
}  // extern "C"
