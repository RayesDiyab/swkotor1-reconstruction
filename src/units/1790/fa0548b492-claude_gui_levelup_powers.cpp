// Lane W4 GUI successor (worker "levelup", fork lu-powers): CSWGuiPowersLevelUp (method table 00759780, 27 slots),
// the in-game level-up Force-power chooser opened by CSWGuiLevelUpPanel::OnSelectPowersButton 006EE350.
// Transcribed from the original swkotor.exe linear sweep (read-only); each statement carries its original PCs.
//   006F0EE0 ~CSWGuiPowersLevelUp                      thiscall() RET
//   006F10C0 OnPanelAdded                              thiscall() RET          (slot +48)
//   006F10F0 HandleCancelButton                        thiscall() RET
//   006F1130 OnAcceptButton                            thiscall() RET
//   006F1210 Draw(float)                               thiscall RET 4          (slot +34)
//   006F1280 DeterminePower(int spell, BYTE* out)      thiscall RET 8
//   006F1320 SetDescription(int strref)                thiscall RET 4
//   006F1440 scalar deleting destructor(flags)         thiscall RET 4          (slot +00)
//   006F1460 OnEnterPower(int spell)                   thiscall RET 4
//   006F15A0 BuildButtons                              thiscall() RET
//   006F1730 BuildAvailableList                        thiscall() RET
//   006F1940 OnPowerSelectionChanged(CSWGuiControl*)   thiscall RET 4          (LB_POWERS event 1F8h)
//   006F1990 InitiatePowers                            thiscall() RET
//   006F1C90 AddChosenPower(int spell)                 thiscall RET 4
//   006F1E40 RemoveChosenPower(int spell)              thiscall RET 4
//   006F1F30 OnRecommendButton                         thiscall() RET
//   006F2030 OnPowerPicked(int spell)                  thiscall RET 4
//   006F2110 OnDoubleClick(CSWGuiControl*)             thiscall RET 4          (LB_POWERS event 1F9h)
//   006F2180 CSWGuiPowersLevelUp(CSWGuiManager*, ulong creature_id)  thiscall RET 8, returns this
//   006F28C0 HandleInputEvent(int event, int value)    thiscall RET 8          (slot +3C)
// (006F2A20 in the same address range is CExoArrayList<ushort>::IndexOf, used only by CSWGuiFeatsCharGen 006F2D9A;
//  it already has the provider @kotor_m15_array_u16_index_of_006f2a20@12 in 0811-m15-l0-leaves and is not part
//  of this class.)
//
// Layout (offsets from the instructions): +18 manager, +44 panel flags, +64 CSWCCreature*, +68 parent level-up
// panel (set by the parent), labels (0x140) +6C..+BAC, LB_POWERS +CEC / LB_DESC +FCC (0x2E0), buttons (0x1C4)
// RECOMMENDED +12AC / SELECT +1470 / ACCEPT +1634 / BACK +17F8, +19BC remaining (byte), +19BD level for
// CanChooseSpell (byte), +19C0 bit0 = server stats [+6C] bit0, +19C4 current spell, +19C8 "select" text,
// +19D0 "remove" text, CExoArrayList<int> known +19D8 / available +19E4 / chosen +19F0, CSWGuiSkillFlowChart +19FC
// (flows +19FC, count +1A00, +1A08 / +1A09 selection bytes), +1A0C status byte of the current spell.
#include "6030a9ee65-claude_gui_char.hpp"

namespace kotor_menu_10_19 { struct String; struct Tlk { String* __thiscall simple_0041e8f0(String* out, unsigned strref); }; }
namespace kotor_m15_l0 { struct PanelEventThunks15 { void __thiscall OnYButtonPressed_00644720(kotor_input_hover::Control*); }; }
namespace alias_add {
int __fastcall Greater005e5490(kotor_reconstruction::StartupString*, void*, kotor_reconstruction::StartupString*); // operator>
}
namespace recovered_save_stats {
unsigned short spell_count_006487d0(void* stats, unsigned cls, unsigned level);   // CSWCCreatureStats::GetNumberKnownSpells (cdecl)
}

extern "C" {
// ---- rules / stats
void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void* spells, void*, w4::U32 id);     // CSWSpellArray::GetSpell
void* __fastcall feedback_spell_name_0059b940(void* spell, void*, void* out) noexcept(false);   // CSWSpell::GetSpellNameText
w4::U32 __fastcall kotor_fastcall_006495a0(void* stats, void*, w4::U32 cls, w4::U32 level, w4::U32 index) noexcept; // GetKnownSpell
void __fastcall recovered_creature_stats_add_known_spell_00649f90(void* stats, void*, w4::U32 cls, w4::U32 spell) noexcept(false); // AddKnownSpell
// client CSWCLevelUpStats rules (fork lu-crules, claude_gui_levelup_rules.cpp)
w4::U32 __fastcall kotor_fastcall_00649070(void* stats, void*);                                    // ComputeNumberKnownSpellsLeft (AL)
w4::U32 __fastcall kotor_fastcall_00649b50(void* stats, void*, w4::U32 spell, w4::U32 level, void* chosen); // CanChooseSpell RET 0Ch
w4::U32 __fastcall kotor_fastcall_006497a0(void* stats, void*, void* spell, void* chosen);         // HasSpellPrereq RET 8
void __fastcall kotor_fastcall_00649c30(void* stats, void*, void* list) noexcept(false);          // AddDefaultPackageSpells RET 4
// parent CSWGuiLevelUpPanel notifications (fork lu-panel, claude_gui_levelup_panel.cpp)
void __fastcall kotor_fastcall_006ee580(void* level_up_panel, void*);                              // CancelledByChild
void __fastcall kotor_fastcall_006ee5d0(void* level_up_panel, void*);                              // SelectionCompleted
// ---- lists
void __fastcall w1x_pointer_list_insert_00616940(void* list, void*, w4::U32 value, w4::U32 index) noexcept(false); // CExoArrayList::Insert
void __fastcall gui_load_array_add_00671c00(void* list, void*, w4::U32 value) noexcept(false);    // CExoArrayList::Add
// ---- skill flow chart / info box (0827-m15d-skillflow, claude_menu_handlers_b2)
void* __fastcall kotor_m15d_chart_construct_006cd510(void* self, void*);                          // CSWGuiSkillFlowChart()
void __fastcall kotor_m15d_chart_destruct_006cd530(void* self, void*);                             // ~CSWGuiSkillFlowChart
void __fastcall kotor_m15d_chart_set_skill_status_all_006cd2e0(void* chart, void*, w4::U32 status); // SetSkillStatusAll
void __fastcall kotor_m15d_chart_set_skill_status_006cd280(void* chart, void*, w4::I32 skill, w4::U32 status); // SetSkillStatus
w4::I32 __fastcall kotor_m15d_chart_set_top_left_006cdd10(void* chart, void*);                    // SetTopLeft
void __fastcall kotor_m15d_chart_set_selected_skill_006cdc00(void* chart, void*, w4::I32 skill);  // SetSelectedSkill
w4::I32 __fastcall kotor_m15d_chart_get_selected_006aba50(void* chart, void*);                    // GetSelected
w4::I32 __fastcall kotor_m15d_chart_handle_input_006cdd80(void* chart, void*, w4::I32 event);     // HandleInput
w4::I32 __fastcall kotor_m15d_skill_flow_hit_check_006cd1c0(void* flow, void*, w4::I32 x, w4::I32 y); // CSWGuiSkillFlow::SkillHitCheckMouse
void __fastcall kotor_fastcall_006ce0f0(void* chart, void*, void* stats, w4::U32 all);            // CreatePowerChart
void __fastcall kotor_m15d_skill_info_set_skill_list_006cdfc0(void* box, void*, void* list, w4::U32 flag) noexcept(false); // SetSkillList
// ---- gui
void __fastcall kotor_m15_panel_set_visible_0040b9d0(void* panel, void*, w4::U32 visible);         // CSWGuiPanel::SetVisible
void __fastcall gui_load_listbox_clear_items_00419a90(void* listbox, void*);                      // CSWGuiListBox::ClearItems
void* __fastcall kotor_m1_listbox_get_selected_control_0041a5d0(void* listbox, void*);            // CSWGuiListBox::GetSelectedControl
void __fastcall gui_load_listbox_set_selected_0041c040(void* listbox, void*, w4::I32 index, w4::I32 flag); // SetSelectedControl
void __fastcall gui_m48_listbox_add_controls_0041c2c0(void* listbox, void*, void** controls, w4::I32 count,
                                                      w4::U32, w4::U32, w4::U32) noexcept(false); // AddControls(array)
void __fastcall gui_load_listbox_set_active_control_0041c160(void* listbox, void*, void* control, w4::U32); // SetActiveControl
std::int32_t __fastcall gui_load_text_font_height_00414ee0(void* text, void*);                    // CSWGuiText::GetFontHeight
// ---- strings / tlk / resources
std::int32_t __fastcall async_string_length_005e5790(void* self, void*);                          // CExoString::GetLength
w4::U32 __fastcall tlk_fetch_0041e550(void* tlk, void*, w4::U32 strref, void* str_res, w4::U32 flag) noexcept(false); // CTlkTable::Fetch
void* __fastcall c2da_default_ref_00405ed0(void* self, void*);                                   // CResRef::CResRef()
void* __fastcall async_assign_string_005e5c50(void* self, void*, void* source);                  // CExoString::operator=
// ---- this file
void __fastcall kotor_fastcall_006f0ee0(void* self, void*);
void __fastcall kotor_fastcall_006f10c0(void* self, void*);
void __fastcall kotor_fastcall_006f10f0(void* self, void*) noexcept(false);
void __fastcall kotor_fastcall_006f1130(void* self, void*) noexcept(false);
void __fastcall kotor_fastcall_006f1210(void* self, void*, w4::U32 delta) noexcept(false);
void __fastcall kotor_fastcall_006f1280(void* self, void*, w4::U32 spell, w4::U8* out);
void __fastcall kotor_fastcall_006f1320(void* self, void*, w4::U32 strref) noexcept(false);
void* __fastcall kotor_fastcall_006f1440(void* self, void*, w4::U32 flags);
void __fastcall kotor_fastcall_006f1460(void* self, void*, w4::U32 spell) noexcept(false);
void __fastcall kotor_fastcall_006f15a0(void* self, void*) noexcept(false);
void __fastcall kotor_fastcall_006f1730(void* self, void*) noexcept(false);
void __fastcall kotor_fastcall_006f1940(void* self, void*, void* control) noexcept(false);
void __fastcall kotor_fastcall_006f1990(void* self, void*) noexcept(false);
void __fastcall kotor_fastcall_006f1c90(void* self, void*, w4::U32 spell) noexcept(false);
void __fastcall kotor_fastcall_006f1e40(void* self, void*, w4::U32 spell) noexcept(false);
void __fastcall kotor_fastcall_006f1f30(void* self, void*) noexcept(false);
void __fastcall kotor_fastcall_006f2030(void* self, void*, w4::U32 spell) noexcept(false);
void __fastcall kotor_fastcall_006f2110(void* self, void*, void* control) noexcept(false);
void* __fastcall kotor_fastcall_006f2180(void* self, void*, void* manager, w4::U32 creature_id) noexcept(false);
void __fastcall kotor_fastcall_006f28c0(void* self, void*, w4::U32 event, w4::U32 value) noexcept(false);
}

using namespace gchar;

namespace {
const unsigned char rdata_0073ecd0 = 0x00;                                                       // .rdata 0073ECD0 (Fetch flag)
const char rdata_0073d5d8[] = " ";                                                               // .rdata 0073D5D8
constexpr U32 kKnown = 0x19D8, kAvailable = 0x19E4, kChosen = 0x19F0;                           // CExoArrayList<int>
constexpr U32 kChart = 0x19FC, kLbPowers = 0xCEC, kLbDesc = 0xFCC;

// STR_RES frame filled by CTlkTable::Fetch: text CExoString, sound CResRef, (length).
struct StrRes { U32 text[2]; unsigned char sound[16]; U32 length; };
static_assert(sizeof(StrRes) == 28, "STR_RES frame");

void* spells() { return lp(rules(), 0x8C); }                                                     // [CSWRules+8C]
void* get_spell(U32 id) { return kotor_m15b_spell_array_get_spell_0059b6d0(spells(), nullptr, id); }
void* stats_of(void* self) { return lp(lp(self, 0x64), 0x2F8); }                                // CSWCCreature +2F8
kotor_menu_10_19::Tlk* tlk() { return static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08); }
I32 lsize(void* self, U32 list) { return li(self, list + 4); }
U32 lat(void* self, U32 list, I32 i) { return lw(lp(self, list), static_cast<U32>(i) * 4u); }
void lset(void* self, U32 list, I32 i, U32 v) { sw(lp(self, list), static_cast<U32>(i) * 4u, v); }
// Inline IndexOf: first index whose element equals v, or -1 (size re-read only at entry, as the original loops).
I32 index_of(void* self, U32 list, U32 v) {
    const I32 n = lsize(self, list);
    for (I32 i = 0; i < n; ++i) if (lat(self, list, i) == v) return i;
    return -1;
}
// Inline Remove(index): --size, then shift down while index < size (re-read).
void remove_at(void* self, U32 list, I32 index) {
    sw(self, list + 4, static_cast<U32>(lsize(self, list) - 1));
    for (; index < lsize(self, list); ++index) lset(self, list, index, lat(self, list, index + 1));
}
// Inline grow of InitiatePowers: capacity*2 (0x10 when 0) stored first, operator new, copy, free the old block.
void grow_inline(void* self, U32 list) {
    U32 capacity = lw(self, list + 8);
    capacity = capacity != 0 ? capacity * 2u : 0x10u;
    void* const old = lp(self, list);
    sw(self, list + 8, capacity);
    sw(self, list, ad(live_operator_new(capacity * 4u)));
    for (I32 i = 0; i < lsize(self, list); ++i) lset(self, list, i, lw(old, static_cast<U32>(i) * 4u));
    if (old != nullptr) live_free(old);
}
// Name text of a spell into a caller temporary (GetSpell then CSWSpell::GetSpellNameText, hidden return).
void* spell_name(Str& out, U32 id) {
    void* const r = feedback_spell_name_0059b940(get_spell(id), nullptr, out.s);
    out.live = true;
    return r;
}
int greater(void* lhs, void* rhs) {
    return alias_add::Greater005e5490(static_cast<kotor_reconstruction::StartupString*>(lhs), nullptr,
                                      static_cast<kotor_reconstruction::StartupString*>(rhs));
}
// Close sequence: PopModalPanel; parent SetVisible(1); panel flags (& ~300h) | 400h.
void close_to_parent(void* self) {
    kotor_fastcall_0040be00(manager_of(self), nullptr);
    kotor_m15_panel_set_visible_0040b9d0(lp(self, 0x68), nullptr, 1);
    sw(self, 0x44, (lw(self, 0x44) & 0xFFFFFCFFu) | 0x400u);
}
// Message box [CGuiInGame+98h]: SetAllowCancel(0); SetCallback(0,0,0); vslot +6C(strref); AddPanel(box, 1, 1).
void show_message(void* self, U32 strref) {
    void* const box = lp(ingame_gui(), 0x98);
    kotor_fastcall_00627130(box, nullptr, 0);
    gui_load_messagebox_set_callback_00624a40(box, nullptr, nullptr, 0, 0);
    vcall<void, U32>(box, 0x6C, strref);
    kotor_fastcall_0040bc70(manager_of(self), nullptr, box, 1, 1);
}
// BTN_SELECT text (+15DC params), text colour, border colour (+1500, three dwords).
void set_select_button(void* self, U32 text, const void* text_color, const void* border_color) {
    gui_load_text_params_set_text_00415e00(at(self, 0x15DC), nullptr, at(self, text));
    gui_load_text_params_set_color_00414e10(at(self, 0x15DC), nullptr, text_color);
    std::memcpy(at(self, 0x1500), border_color, 12);
}
// EH cleanup of a member CExoArrayList (original funclets: _free(data)).
void __fastcall free_list(void* list, void*) { live_free(lp(list, 0)); }
// CExoString(const char*) temporary.
struct Lit {
    U32 s[2];
    explicit Lit(const char* text) { frame_LogStringCtor_005e5a90(s, nullptr, text); }
    ~Lit() { frame_LogStringDtor_005e5c20(s, nullptr); }
};
void simple(Str& out, U32 strref) {
    tlk()->simple_0041e8f0(static_cast<kotor_menu_10_19::String*>(static_cast<void*>(out.s)), strref);
    out.live = true;
}

// Method table 00759780 (27 slots): CSWGuiPanel 0073E010 except +00 006F1440, +34 006F1210, +3C 006F28C0,
// +48 006F10C0 (read from .rdata 00759780..007597EB).
const Override ovr_powers[] = {
    {0x00, fn(&kotor_fastcall_006f1440)},   // 006F1440 scalar deleting destructor
    {0x34, fn(&kotor_fastcall_006f1210)},   // 006F1210 Draw
    {0x3C, fn(&kotor_fastcall_006f28c0)},   // 006F28C0 HandleInputEvent
    {0x48, fn(&kotor_fastcall_006f10c0)},   // 006F10C0 OnPanelAdded
};
void* table_powers[27];
bool built_powers = false;
const void* vt_powers() {
    if (!built_powers) { vt_build(table_powers, 27, gui_load_panel_methods_0073e010, 27, ovr_powers, 4); built_powers = true; }
    return table_powers;
}
}  // namespace

extern "C" {
// ======================================================================== 006F0EE0 destructor (RET)
void __fastcall kotor_fastcall_006f0ee0(void* self, void*) {
    set_vptr(self, vt_powers());                                                                // 006F0F00 (759780)
    gui_load_listbox_clear_items_00419a90(at(self, kLbDesc), nullptr);                          // 006F0F06..16
    gui_load_listbox_clear_items_00419a90(at(self, kLbPowers), nullptr);                        // 006F0F1B..23
    kotor_m15d_chart_destruct_006cd530(at(self, kChart), nullptr);                              // 006F0F28..33
    live_free(lp(self, kChosen));    zero(self, kChosen);                                        // 006F0F38..51
    live_free(lp(self, kAvailable)); zero(self, kAvailable);                                     // 006F0F51..62
    live_free(lp(self, kKnown));     zero(self, kKnown);                                         // 006F0F68..82
    frame_LogStringDtor_005e5c20(at(self, 0x19D0), nullptr);                                    // 006F0F7C..8D
    frame_LogStringDtor_005e5c20(at(self, 0x19C8), nullptr);                                    // 006F0F92..9D
    gui_load_button_destruct_00419da0(at(self, 0x17F8), nullptr);                               // 006F0FA2..AD
    gui_load_button_destruct_00419da0(at(self, 0x1634), nullptr);                               // 006F0FB2..BD
    gui_load_button_destruct_00419da0(at(self, 0x1470), nullptr);                               // 006F0FC2..CD
    gui_load_button_destruct_00419da0(at(self, 0x12AC), nullptr);                               // 006F0FD2..DD
    gui_load_listbox_destruct_0041b060(at(self, kLbDesc), nullptr);                             // 006F0FE2..E9
    gui_load_listbox_destruct_0041b060(at(self, kLbPowers), nullptr);                           // 006F0FEE..F5
    for (U32 i = 10; i-- > 0;) gui_load_label_destruct_00419d10(at(self, 0x6C + i * 0x140), nullptr); // 006F0FFA..1091
    gui_load_panel_destruct_0040cf70(self, nullptr);                                            // 006F1096..A0
}

// ======================================================================== 006F1440 scalar deleting dtor (RET 4)
void* __fastcall kotor_fastcall_006f1440(void* self, void*, U32 flags) {
    kotor_fastcall_006f0ee0(self, nullptr);                                                     // 006F1443
    if (flags & 1u) live_free(self);                                                            // 006F1448..55
    return self;                                                                                // 006F1458
}

// ======================================================================== 006F10C0 OnPanelAdded (RET)
void __fastcall kotor_fastcall_006f10c0(void* self, void*) {
    gui_load_panel_on_added_0040b870(self, nullptr);                                            // 006F10C0
    show_tutorial_window_005edf40(client_app(), nullptr, 0x13, 0x7F000000u, 0x7F000000u, 0);    // 006F10C5..DB
}

// ======================================================================== 006F10F0 HandleCancelButton (RET)
void __fastcall kotor_fastcall_006f10f0(void* self, void*) noexcept(false) {
    if (lp(self, 0x68) == nullptr) return;                                                      // 006F10F3..F8
    close_to_parent(self);                                                                      // 006F10FA..111C
    kotor_fastcall_006ee580(lp(self, 0x68), nullptr);                                           // 006F110F / 006F1120 (jmp)
}

// ======================================================================== 006F1130 OnAcceptButton (RET)
void __fastcall kotor_fastcall_006f1130(void* self, void*) noexcept(false) {
    if (lb(self, 0x19BC) > 0 && lsize(self, kAvailable) > 0) {                                  // 006F1133..46 (jbe / jle)
        show_message(self, 0xBC52);                                                             // 006F1148..85
        return;
    }
    const U32 cls = static_cast<U8>(lb(stats_of(self), 0x32) - 1);                              // 006F118D..A6 (dec dl; movzx)
    for (I32 i = 0; i < lsize(self, kChosen); ++i)                                              // 006F11A1..D2
        recovered_creature_stats_add_known_spell_00649f90(stats_of(self), nullptr, cls, lat(self, kChosen, i)); // 006F11B0..C4
    close_to_parent(self);                                                                      // 006F11D4..F8
    kotor_fastcall_006ee5d0(lp(self, 0x68), nullptr);                                           // 006F11E9 / 006F11FC (jmp)
}

// ======================================================================== 006F1210 Draw(delta) (RET 4)
void __fastcall kotor_fastcall_006f1210(void* self, void*, U32 delta) noexcept(false) {
    const U8 status = lb(self, 0x1A0C);                                                         // 006F1213
    if (status == 2 || status == 3)                                                             // 006F121A..21
        set_select_button(self, status == 2 ? 0x19D0u : 0x19C8u, kotor_m1_color_007a2408, kotor_m1_color_007a2384); // 006F1223..67
    kotor_panel_draw_0040B760(self, nullptr, delta);                                            // 006F126A..71
}

// ======================================================================== 006F1280 DeterminePower(spell, out) (RET 8)
// known -> 2, chosen -> 1, available -> 0, otherwise 3.
void __fastcall kotor_fastcall_006f1280(void* self, void*, U32 spell, U8* out) {
    if (index_of(self, kKnown, spell) != -1) { *out = 2; return; }                              // 006F1280..A2 / 006F12EC..FA
    if (index_of(self, kChosen, spell) != -1) { *out = 1; return; }                             // 006F12A4..C0 / 006F12FD..30B
    if (index_of(self, kAvailable, spell) != -1) { *out = 0; return; }                          // 006F12C2..DE / 006F130E..31C
    *out = 3;                                                                                   // 006F12E0..E9
}

// ======================================================================== 006F1320 SetDescription(strref) (RET 4)
void __fastcall kotor_fastcall_006f1320(void* self, void*, U32 strref) noexcept(false) {
    StrRes res;                                                                                 // [esp+1C]
    exo_default(res.text);                                                                      // 006F133C..40 (state 0)
    auto guard = w4gui::on_throw([&]() { frame_LogStringDtor_005e5c20(res.text, nullptr); });
    c2da_default_ref_00405ed0(res.sound, nullptr);                                              // 006F1345..51 (state 1)
    tlk_fetch_0041e550(kotor_cell_tlk_007a3a08, nullptr, strref, &res, rdata_0073ecd0);         // 006F1356..76
    gui_load_text_params_set_text_00415e00(at(self, 0xB54), nullptr, res.text);                 // 006F137B..86 (DESC item label)
    gui_load_listbox_clear_items_00419a90(at(self, kLbDesc), nullptr);                          // 006F138B..93
    void* const label = at(self, 0xA6C);
    I32 extent[4];
    std::memcpy(extent, at(label, 4), 16);                                                      // 006F1398..B9
    extent[3] = async_string_length_005e5790(res.text, nullptr) > 0                             // 006F13B1..C4
        ? vcall<I32>(at(self, 0xB3C), 0x08)                                                     // 006F13CC..CE
        : gui_load_text_font_height_00414ee0(at(self, 0xB3C), nullptr);                         // 006F13D3
    vcall<void, const void*>(label, 0x04, extent);                                              // 006F13D8..EB SetExtent
    void* controls[1] = {label};                                                                // 006F13FD
    gui_m48_listbox_add_controls_0041c2c0(at(self, kLbDesc), nullptr, controls, 1, 0, 0, 0);    // 006F13EE..401
    gui_load_listbox_set_active_control_0041c160(at(self, kLbDesc), nullptr, label, 0);         // 006F1406..0B
    guard.armed = false;
    frame_LogStringDtor_005e5c20(res.text, nullptr);                                            // 006F1410..1C
}

// ======================================================================== 006F1460 OnEnterPower(spell) (RET 4)
void __fastcall kotor_fastcall_006f1460(void* self, void*, U32 spell) noexcept(false) {
    kotor_fastcall_006f1280(self, nullptr, spell, static_cast<U8*>(at(self, 0x1A0C)));          // 006F1469..71
    sw(self, 0x19C4, spell);                                                                    // 006F1476
    void* const sp = get_spell(spell);                                                          // 006F147C..8D
    switch (lb(self, 0x1A0C)) {                                                                 // 006F148F..9B (table 006F1588)
    case 0: set_select_button(self, 0x19C8, borrowed_exit_color_007a23b4, kotor_m1_color_007a2378); break; // 006F14A2..DD
    case 1: set_select_button(self, 0x19D0, borrowed_exit_color_007a23b4, kotor_m1_color_007a2378); break; // 006F14E2..1D
    case 2: set_select_button(self, 0x19D0, kotor_m1_color_007a2408, kotor_m1_color_007a2384); break;      // 006F151F..62
    case 3: set_select_button(self, 0x19C8, kotor_m1_color_007a2408, kotor_m1_color_007a2384); break;      // 006F1527..62
    default: break;                                                                             // 006F1495 (ja 006F1565)
    }
    gui_load_text_params_set_strref_00415e50(at(self, 0xC94), nullptr, lw(sp, 8));              // 006F1565..6F LBL_POWER name
    kotor_fastcall_006f1320(self, nullptr, lw(sp, 0xC));                                        // 006F1574..7A description
}

// ======================================================================== 006F15A0 BuildButtons (RET)
void __fastcall kotor_fastcall_006f15a0(void* self, void*) noexcept(false) {
    void* const chart = at(self, kChart);
    kotor_m15d_chart_set_skill_status_all_006cd2e0(chart, nullptr, 3);                          // 006F15BD..C7
    for (I32 i = 0; i < lsize(self, kKnown); ++i)                                               // 006F15CC..FC
        kotor_m15d_chart_set_skill_status_006cd280(chart, nullptr, static_cast<I32>(lat(self, kKnown, i)), 1);
    for (I32 i = 0; i < lsize(self, kChosen); ++i)                                              // 006F15FE..2C
        kotor_m15d_chart_set_skill_status_006cd280(chart, nullptr, static_cast<I32>(lat(self, kChosen, i)), 4);
    for (I32 i = 0; i < lsize(self, kAvailable); ++i) {                                         // 006F162E..7C
        const U32 spell = lat(self, kAvailable, i);
        const U32 status = ((lb(self, 0x19C0) & 1u) == 0 && (spell == 6 || spell == 0xE)) ? 3u : 0u;   // 006F1640..69
        kotor_m15d_chart_set_skill_status_006cd280(chart, nullptr, static_cast<I32>(spell), status);   // 006F166B..6E
    }
    if (li(self, 0x19C4) == -1) {                                                               // 006F167E..89
        kotor_fastcall_0041c1d0(at(self, kLbPowers), nullptr, chart, 0, 1, 0);                  // 006F168B..98 AddControls
        sw(self, 0x19C4, static_cast<U32>(kotor_m15d_chart_set_top_left_006cdd10(chart, nullptr)));   // 006F169D..A4
    }
    kotor_m15d_chart_set_selected_skill_006cdc00(chart, nullptr, li(self, 0x19C4));             // 006F16AA..B3
    gui_load_listbox_set_selected_0041c040(at(self, kLbPowers), nullptr, lb(self, 0x1A09), 0);  // 006F16B8..CA
    kotor_fastcall_006f1460(self, nullptr, lw(self, 0x19C4));                                   // 006F16CF..D8
    Str remaining(static_cast<I32>(lb(self, 0x19BC)));                                          // 006F16DD..E9 CExoString(int)
    gui_load_text_params_set_text_00415e00(at(self, 0x514), nullptr, remaining.p());            // 006F16EE..701
}                                                                                               // 006F1706..0E

// ======================================================================== 006F1730 BuildAvailableList (RET)
// Appends every choosable spell not yet listed to the available list, sorted by name (insert before the first
// listed spell whose name is greater).
void __fastcall kotor_fastcall_006f1730(void* self, void*) noexcept(false) {
    Str name; name.empty();                                                                     // 006F1754..5C [esp+20] (state 0)
    void* const stats = stats_of(self);                                                         // 006F1761..64 [esp+1C]
    U32 counter = 0;                                                                            // [esp+18]
    for (U32 spell = 0; static_cast<I32>(spell) < li(spells(), 0); ) {                          // 006F176A..88 / 006F18F7..913
        bool skip = index_of(self, kKnown, spell) != -1;                                        // 006F1790..B3
        if (!skip) skip = index_of(self, kChosen, spell) != -1;                                 // 006F17B9..E1
        if (!skip) skip = index_of(self, kAvailable, spell) != -1;                              // 006F17E7..811
        if (!skip) skip = kotor_fastcall_00649b50(stats, nullptr, spell, lb(self, 0x19BD), at(self, kChosen)) == 0; // 006F1817..2D
        void* sp = nullptr;
        if (!skip) { sp = get_spell(spell); skip = li(sp, 0x174) == -2 || lw(sp, 8) == 0; }    // 006F1833..57
        if (!skip) {
            {
                Str t;                                                                          // [esp+28]
                void* const r = feedback_spell_name_0059b940(sp, nullptr, t.s); t.live = true;  // 006F185D..64
                async_assign_string_005e5c50(name.p(), nullptr, r);                             // 006F1869..73
            }                                                                                   // 006F1878..81
            I32 i = 0;
            for (; i < lsize(self, kAvailable); ++i) {                                          // 006F1886..8E
                int g;
                {
                    Str other;                                                                  // [esp+30]
                    g = greater(spell_name(other, lat(self, kAvailable, i)), name.p());         // 006F1890..C0
                }                                                                               // 006F18C5..D0
                if (g != 0) break;                                                              // 006F18D5..D7
            }
            if (i < lsize(self, kAvailable))
                w1x_pointer_list_insert_00616940(at(self, kAvailable), nullptr, spell, static_cast<U32>(i)); // 006F18E0..E4
            else
                gui_load_array_add_00671c00(at(self, kAvailable), nullptr, spell);              // 006F18EF..F2
        }
        ++counter;                                                                              // 006F18F7..909
        spell = counter & 0xFFFFu;                                                              // 006F190A movzx ebx, cx
    }
}                                                                                               // 006F1919..25

// ======================================================================== 006F1940 OnPowerSelectionChanged (RET 4)
void __fastcall kotor_fastcall_006f1940(void* self, void*, void*) noexcept(false) {
    void* const flow = kotor_m1_listbox_get_selected_control_0041a5d0(at(self, kLbPowers), nullptr);   // 006F1944..4A
    const I32 skill = kotor_m15d_skill_flow_hit_check_006cd1c0(flow, nullptr, li(self, 0xD6C), li(self, 0xD70)); // 006F194F..5F
    kotor_m15d_chart_set_selected_skill_006cdc00(at(self, kChart), nullptr, skill);             // 006F1966..6D
    kotor_fastcall_006f1460(self, nullptr, static_cast<U16>(skill));                            // 006F1972..78 (movzx di)
}

// ======================================================================== 006F1990 InitiatePowers (RET)
// Collects the creature's known spells (all classes) into the known list sorted by name, then the remaining count.
void __fastcall kotor_fastcall_006f1990(void* self, void*) noexcept(false) {
    Str name; name.empty();                                                                     // 006F19B4..B8 [esp+20] (state 0)
    void* const stats = stats_of(self);                                                         // 006F19BD..C0 ebp / [esp+1C]
    for (U8 cls = 0; cls < lb(stats, 0x32); ++cls) {                                            // 006F19C6..DC / 006F1C2E..3D
        U8 index = 0;                                                                           // 006F19EB [esp+14]
        if (recovered_save_stats::spell_count_006487d0(stats, cls, 0) == 0) continue;           // 006F19E2..F8
        do {
            const U32 spell = kotor_fastcall_006495a0(stats, nullptr, cls, 0, index);           // 006F1A00..0E (ebx, [esp+18])
            {
                Str t;                                                                          // [esp+28]
                void* const r = spell_name(t, spell);                                           // 006F1A13..34
                async_assign_string_005e5c50(name.p(), nullptr, r);                             // 006F1A39..43
            }                                                                                   // 006F1A48..51
            I32 i = 0;
            bool inserted = false;
            for (; i < lsize(self, kKnown); ++i) {                                              // 006F1A60..66
                int g;
                {
                    Str other;                                                                  // [esp+30]
                    g = greater(spell_name(other, lat(self, kKnown, i)), name.p());             // 006F1A6C..9F
                }                                                                               // 006F1AA4..AF
                if (g != 0) {                                                                   // 006F1AB4..B6
                    const I32 n = lsize(self, kKnown);                                          // 006F1ABF ebp
                    if (n == li(self, kKnown + 8)) grow_inline(self, kKnown);                   // 006F1AC5..B34
                    lset(self, kKnown, lsize(self, kKnown), spell);                             // 006F1B38..44
                    sw(self, kKnown + 4, static_cast<U32>(lsize(self, kKnown) + 1));            // 006F1B47..50
                    for (I32 k = n; k > i; --k) lset(self, kKnown, k, lat(self, kKnown, k - 1)); // 006F1B56..72
                    lset(self, kKnown, i, spell);                                               // 006F1B74..7E
                    inserted = true;
                    break;
                }
            }
            if (!inserted) {                                                                    // 006F1B86
                if (lsize(self, kKnown) == li(self, kKnown + 8)) grow_inline(self, kKnown);     // 006F1B86..BF4
                lset(self, kKnown, lsize(self, kKnown), spell);                                 // 006F1BF4..00
                sw(self, kKnown + 4, static_cast<U32>(lsize(self, kKnown) + 1));                // 006F1C03
            }
            ++index;                                                                            // 006F1C09..18
        } while (static_cast<U16>(index) < recovered_save_stats::spell_count_006487d0(stats, cls, 0)); // 006F1C1C..28
    }
    const U8 left = static_cast<U8>(kotor_fastcall_00649070(stats, nullptr));                   // 006F1C43..45
    sb(self, 0x19BC, left);                                                                     // 006F1C4A
    sb(self, 0x19BD, static_cast<U8>(left + 1));                                                // 006F1C50..54
    kotor_fastcall_006f1730(self, nullptr);                                                     // 006F1C52..5A
    kotor_fastcall_006f15a0(self, nullptr);                                                     // 006F1C5F..61
}                                                                                               // 006F1C66..72

// ======================================================================== 006F1C90 AddChosenPower(spell) (RET 4)
// Moves the spell from the available list to the chosen list (sorted by name).
void __fastcall kotor_fastcall_006f1c90(void* self, void*, U32 spell) noexcept(false) {
    const I32 index = index_of(self, kAvailable, spell);                                        // 006F1CB2..DD
    if (index == -1) return;                                                                    // 006F1CDF..F0 / 006F1CF3..F6
    remove_at(self, kAvailable, index);                                                         // 006F1CFC..28
    Str name;                                                                                   // [esp+10]
    spell_name(name, spell);                                                                    // 006F1D2A..47
    U32 before = 0;                                                                             // [esp+0C]
    I32 i = 0;
    for (; i < lsize(self, kChosen); ++i) {                                                     // 006F1D50..56
        int g;
        {
            Str other;                                                                          // [esp+18]
            g = greater(spell_name(other, lat(self, kChosen, i)), name.p());                    // 006F1D58..8B
        }                                                                                       // 006F1D90..9B
        if (g != 0) { before = 1; break; }                                                      // 006F1DA0..A7
    }
    sb(self, 0x19BC, static_cast<U8>(lb(self, 0x19BC) - 1));                                    // 006F1DAF..BD
    if (before != 0) {
        w1x_pointer_list_insert_00616940(at(self, kChosen), nullptr, spell, static_cast<U32>(i)); // 006F1DC5..D0
    } else {
        void* const list = at(self, kChosen);
        if (lsize(self, kChosen) == li(self, kChosen + 8)) {                                    // 006F1DD7..EB
            const U32 capacity = lw(self, kChosen + 8);
            menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(list), nullptr,
                                                capacity != 0 ? capacity * 2u : 0x10u);         // 006F1DED..FD
        }
        lset(self, kChosen, lsize(self, kChosen), spell);                                       // 006F1E02..0A
        sw(self, kChosen + 4, static_cast<U32>(lsize(self, kChosen) + 1));                      // 006F1E0D
    }
    kotor_fastcall_006f1730(self, nullptr);                                                     // 006F1E10..12
}                                                                                               // 006F1E17..23

// ======================================================================== 006F1E40 RemoveChosenPower(spell) (RET 4)
// Removes the spell; a chosen spell whose prerequisite is now missing is removed recursively (that call ends the
// work); otherwise the available list is emptied and rebuilt.
void __fastcall kotor_fastcall_006f1e40(void* self, void*, U32 spell) noexcept(false) {
    const I32 index = index_of(self, kChosen, spell);                                           // 006F1E43..6C
    if (index == -1) return;                                                                    // 006F1E6E..72 / 006F1E75..78
    remove_at(self, kChosen, index);                                                            // 006F1E7E..A1
    sb(self, 0x19BC, static_cast<U8>(lb(self, 0x19BC) + 1));                                    // 006F1EA3..AE
    void* const stats = stats_of(self);                                                         // 006F1EA9..B4 ebp
    for (I32 i = 0; i < lsize(self, kChosen); ++i) {                                            // 006F1EBA..FD
        void* const sp = get_spell(lat(self, kChosen, i));                                      // 006F1ED0..E3
        if (kotor_fastcall_006497a0(stats, nullptr, sp, at(self, kChosen)) == 0) {              // 006F1EE1..F2
            kotor_fastcall_006f1e40(self, nullptr, lat(self, kChosen, i));                      // 006F1F17..1F
            return;
        }
    }
    sw(self, kAvailable + 4, 0);                                                                // 006F1F01
    kotor_fastcall_006f1730(self, nullptr);                                                     // 006F1EFF..0B
}

// ======================================================================== 006F1F30 OnRecommendButton (RET)
void __fastcall kotor_fastcall_006f1f30(void* self, void*) noexcept(false) {
    PtrList recommended;                                                                        // [esp+08] (state 0; _free(data))
    kotor_fastcall_00649c30(stats_of(self), nullptr, &recommended);                             // 006F1F5A..6C
    while (lsize(self, kChosen) != 0)                                                           // 006F1F71..96
        kotor_fastcall_006f1e40(self, nullptr, lat(self, kChosen, 0));
    for (I32 i = 0; i < recommended.size; ++i)                                                  // 006F1F98..B6
        kotor_fastcall_006f1c90(self, nullptr, ad(recommended.data[i]));
    void* const box = lp(ingame_gui(), 0x9C);                                                   // 006F1FB8..C6 skill info box
    kotor_m15d_skill_info_set_skill_list_006cdfc0(box, nullptr, &recommended, 2);               // 006F1FCC..D5
    gui_load_text_params_set_strref_00415e50(at(box, 0x42C), nullptr, 0xA511);                  // 006F1FDA..E5
    kotor_fastcall_0040bc70(manager_of(self), nullptr, box, 1, 1);                              // 006F1FEA..F2
    kotor_fastcall_006f15a0(self, nullptr);                                                     // 006F1FF7..F9
}                                                                                               // 006F1FFE..200B _free

// ======================================================================== 006F2030 OnPowerPicked(spell) (RET 4)
void __fastcall kotor_fastcall_006f2030(void* self, void*, U32 spell) noexcept(false) {
    U8 status = 0;                                                                              // the argument slot's low byte
    kotor_fastcall_006f1280(self, nullptr, spell, &status);                                     // 006F2036..3E
    U32 strref = 0;
    switch (status) {                                                                           // 006F2043..52 (table 006F2100)
    case 0:                                                                                     // available
        if ((lb(self, 0x19C0) & 1u) == 0 && (spell == 6 || spell == 0xE)) {                     // 006F2059..6A
            strref = 0xA5E6;                                                                    // 006F206C
        } else if (lb(self, 0x19BC) > 0) {                                                      // 006F2073..7B
            kotor_fastcall_006f1c90(self, nullptr, spell);                                      // 006F207D..80
            kotor_fastcall_006f15a0(self, nullptr);                                             // 006F2085..87
            return;
        } else {
            strref = 0xA621;                                                                    // 006F2092
        }
        break;
    case 1:                                                                                     // chosen
        kotor_fastcall_006f1e40(self, nullptr, spell);                                          // 006F2099..9C
        kotor_fastcall_006f15a0(self, nullptr);                                                 // 006F20A1..A3
        return;
    case 2: strref = 0xA4C9; break;                                                             // 006F20AE known
    case 3: strref = 0xA4CA; break;                                                             // 006F20B5 not choosable
    default: return;                                                                            // 006F204B (ja 006F20FA)
    }
    show_message(self, strref);                                                                 // 006F20BA..F4
}

// ======================================================================== 006F2110 OnDoubleClick (RET 4)
// Picks the power when the hit cell equals the chart's selected cell.
void __fastcall kotor_fastcall_006f2110(void* self, void*, void*) noexcept(false) {
    I32 selected = -1;                                                                          // 006F2144
    if (li(self, 0x1A00) > 0) {                                                                 // 006F2113..1C
        void* const flow = lp(lp(self, kChart), static_cast<U32>(lb(self, 0x1A09)) * 4u);       // 006F2125..38
        selected = li(flow, static_cast<U32>(lb(self, 0x1A08)) * 0x128u + 0x178u);              // 006F211E..3B
    }
    void* const control = kotor_m1_listbox_get_selected_control_0041a5d0(at(self, kLbPowers), nullptr);   // 006F2147..4D
    const I32 hit = kotor_m15d_skill_flow_hit_check_006cd1c0(control, nullptr, li(self, 0xD6C), li(self, 0xD70)); // 006F2152..62
    if (hit == selected)                                                                        // 006F2167..69
        kotor_fastcall_006f2030(self, nullptr, static_cast<U16>(hit));                          // 006F216B..71 (movzx ax)
}

// ======================================================================== 006F2180 constructor (RET 8)
void* __fastcall kotor_fastcall_006f2180(void* self, void*, void* manager, U32 creature_id) noexcept(false) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                  // 006F21A6..AB
    Unwind u; u.push(&gui_load_panel_destruct_0040cf70, self);
    set_vptr(self, vt_powers());                                                                // 006F21B9 (759780)
    for (U32 i = 0; i < 10; ++i) label(self, 0x6C + i * 0x140, u);                              // 006F21B2..24F (states 1..A)
    listbox(self, kLbPowers, u); listbox(self, kLbDesc, u);                                     // 006F2254..271 (B, C)
    button(self, 0x12AC, u); button(self, 0x1470, u); button(self, 0x1634, u); button(self, 0x17F8, u);   // 006F2276..2B1
    exo_default(at(self, 0x19C8)); u.push(&frame_LogStringDtor_005e5c20, at(self, 0x19C8));    // 006F22B6..C1 (state 11h)
    exo_default(at(self, 0x19D0)); u.push(&frame_LogStringDtor_005e5c20, at(self, 0x19D0));    // 006F22C6..D1
    zero(self, 0x19DC); zero(self, 0x19E0); zero(self, 0x19D8);                                 // 006F22D6..E2 known
    zero(self, 0x19E8); zero(self, 0x19EC); zero(self, 0x19E4);                                 // 006F22E8..F4 available
    zero(self, 0x19F4); zero(self, 0x19F8); zero(self, 0x19F0);                                 // 006F22FA..306 chosen
    u.push(&free_list, at(self, kKnown)); u.push(&free_list, at(self, kAvailable)); u.push(&free_list, at(self, kChosen));
    kotor_m15d_chart_construct_006cd510(at(self, kChart), nullptr);                             // 006F230C..17 (state 15h)
    u.push(&kotor_m15d_chart_destruct_006cd530, at(self, kChart));                              // (state 16h)
    sw(self, 0x18, ad(kotor_fastcall_005eda40(client_app(), nullptr)));                        // 006F231C..30 GetSWGuiManager
    void* const creature = pt(frame_R4_005ed5b0(client_app(), nullptr, creature_id));           // 006F2333..40
    sw(self, 0x64, ad(creature));                                                               // 006F2347
    void* const server = kotor_m15a_creature_get_server_creature_0060fb20(creature, nullptr);   // 006F2345..4A
    const U32 flags = lw(self, 0x19C0);                                                         // 006F2358
    sw(self, 0x19C0, flags ^ ((lw(lp(server, 0xA74), 0x6C) ^ flags) & 1u));                     // 006F234F..36E (bit0 copied)
    sw(self, 0x19C4, 0xFFFFFFFFu);                                                              // 006F2374
    start_layout(self, "pwrlvlup");                                                             // 006F2365..8A (007598A0)
    init_control(self, 0x6C, "MAIN_TITLE_LBL");                                                 // 006F238F..BC (00759090)
    init_control(self, 0x1AC, "SUB_TITLE_LBL");                                                 // 006F23C1..F1 (00759890)
    init_control(self, 0x2EC, "REMAINING_BOX_LBL");                                             // 006F23F6..426 (0075987C)
    init_control(self, 0x42C, "REMAINING_SELECTIONS_LBL");                                      // 006F242B..45B (00759860)
    init_control(self, 0x56C, "SELECTIONS_REMAINING_LBL");                                      // 006F2460..490 (00759844)
    init_control(self, 0x6AC, "DESC_LBL");                                                      // 006F2495..4C5 (00759838)
    init_control(self, kLbDesc, "LB_DESC");                                                     // 006F24CA..4FA (00755D6C)
    sb(self, 0x128D, 1);                                                                        // 006F2508 (LB_DESC +2C1, before the string ctor)
    init_control(self, kLbPowers, "LB_POWERS");                                                 // 006F24FF..530 (0075982C)
    init_control(self, 0xBAC, "LBL_POWER");                                                     // 006F2535..565 (00759820)
    init_control(self, 0x12AC, "RECOMMENDED_BTN");                                              // 006F256A..59A (00759810)
    init_control(self, 0x1470, "SELECT_BTN");                                                   // 006F259F..5CF (00759804)
    init_control(self, 0x1634, "ACCEPT_BTN");                                                   // 006F25D4..604 (007597F8)
    init_control(self, 0x17F8, "BACK_BTN");                                                     // 006F2609..639 (007597EC)
    stop_layout(self);                                                                          // 006F263E..40
    Str powers;                                                                                 // [esp+10] (state 24h)
    simple(powers, 0xA5F8);                                                                     // 006F2645..55
    {
        Lit space(rdata_0073d5d8);                                                              // 006F265A..68 [esp+18]
        Str verb; simple(verb, 0x9637);                                                         // 006F266D..82 [esp+30]
        Str head; GuiConcat005e5d10(verb.ss(), nullptr, head.ss(),
                                    static_cast<kotor_reconstruction::StartupString*>(static_cast<void*>(space.s))); head.live = true; // 006F2687..98 [esp+28]
        Str full; GuiConcat005e5d10(head.ss(), nullptr, full.ss(), powers.ss()); full.live = true;   // 006F269D..AE [esp+20]
        async_assign_string_005e5c50(at(self, 0x19C8), nullptr, full.p());                     // 006F26B3..BF
    }                                                                                           // 006F26C4..F7
    {
        Lit space(rdata_0073d5d8);                                                              // 006F26FC..705 [esp+18]
        Str verb; simple(verb, 0x9638);                                                         // 006F270A..71F [esp+20]
        Str head; GuiConcat005e5d10(verb.ss(), nullptr, head.ss(),
                                    static_cast<kotor_reconstruction::StartupString*>(static_cast<void*>(space.s))); head.live = true; // 006F2724..35 [esp+28]
        Str full; GuiConcat005e5d10(head.ss(), nullptr, full.ss(), powers.ss()); full.live = true;   // 006F273A..4B [esp+30]
        async_assign_string_005e5c50(at(self, 0x19D0), nullptr, full.p());                     // 006F2750..5C
    }                                                                                           // 006F2761..94
    {
        void* const proto = lp(self, 0x1298);                                                   // 006F2799 (LB_DESC +2CC prototype)
        vcall<void, const void*, void*, float>(at(self, 0xA6C), 0x98, at(proto, 4), proto, 1.0f);   // 006F279F..7B5
    }
    kotor_fastcall_006ce0f0(at(self, kChart), nullptr,
                            lp(kotor_m15a_creature_get_server_creature_0060fb20(lp(self, 0x64), nullptr), 0xA74), 1); // 006F27BB..D2
    kotor_fastcall_006f1990(self, nullptr);                                                     // 006F27D7..D9
    add_event(self, kLbPowers, 0x1F8, self, fn(&kotor_fastcall_006f1940));                      // 006F27DE..EB
    add_event(self, kLbPowers, 0x1F9, self, fn(&kotor_fastcall_006f2110));                      // 006F27F0..FD
    vcall<void, U32>(at(self, kLbPowers), 0xA4, 1);                                             // 006F2802..08
    sw(self, 0x12F0, lw(self, 0x12F0) & 0xFFFFFFFBu);                                           // 006F280E..25 RECOMMENDED
    add_event(self, 0x12AC, 0x27, self, pmf_code(&kotor_m15_l0::PanelEventThunks15::OnYButtonPressed_00644720)); // 006F2814..28
    sw(self, 0x14B4, lw(self, 0x14B4) & 0xFFFFFFFBu);                                           // 006F282D..44 SELECT
    add_event(self, 0x1470, 0x27, self, on_x());                                                // 006F2833..47
    sw(self, 0x1678, lw(self, 0x1678) & 0xFFFFFFFBu);                                           // 006F284C..63 ACCEPT
    add_event(self, 0x1634, 0x27, self, on_accept());                                           // 006F2852..66
    sw(self, 0x183C, lw(self, 0x183C) & 0xFFFFFFFBu);                                           // 006F286B..82 BACK
    add_event(self, 0x17F8, 0x27, self, on_b());                                                // 006F2871..85
    gui_load_panel_set_active_control_0040a630(self, nullptr, at(self, kLbPowers), 0);          // 006F288A..8F
    u.dismiss();
    return self;                                                                                // 006F2894..8B4 (~powers)
}

// ======================================================================== 006F28C0 HandleInputEvent (RET 8)
void __fastcall kotor_fastcall_006f28c0(void* self, void*, U32 event, U32 value) noexcept(false) {
    static const U8 index_006f2a04[0x1A] = {0, 1, 2, 3, 7, 7, 0, 1, 4, 4, 4, 4, 7, 7, 7, 7, 7, 7, 5, 6, 7, 7, 4, 4, 4, 4};
    if (value != 0 && event - 0x27u <= 0x19u) {                                                 // 006F28C5..DB
        switch (index_006f2a04[event - 0x27u]) {                                                // 006F28E1..E8 (table 006F29E4)
        case 0:                                                                                 // 006F290F
            kotor_fastcall_0040a140(manager_of(self), nullptr, 0);
            kotor_fastcall_006f1130(self, nullptr);
            break;
        case 1:                                                                                 // 006F28EF
            kotor_fastcall_0040a140(manager_of(self), nullptr, 0);
            kotor_fastcall_006f10f0(self, nullptr);
            break;
        case 2:                                                                                 // 006F294F
            kotor_fastcall_006f2030(self, nullptr, static_cast<U32>(kotor_m15d_chart_get_selected_006aba50(at(self, kChart), nullptr)));
            kotor_fastcall_0040a140(manager_of(self), nullptr, 0);
            break;
        case 3:                                                                                 // 006F292F
            kotor_fastcall_0040a140(manager_of(self), nullptr, 0);
            kotor_fastcall_006f1f30(self, nullptr);
            break;
        case 4:                                                                                 // 006F297B
            kotor_fastcall_006f1460(self, nullptr,
                static_cast<U32>(kotor_m15d_chart_handle_input_006cdd80(at(self, kChart), nullptr, static_cast<I32>(event))));
            break;
        case 5: vcall<void, U32, U32>(at(self, kLbDesc), 0x3C, 0x31, 1); break;                 // 006F29C0..D0
        case 6: vcall<void, U32, U32>(at(self, kLbDesc), 0x3C, 0x32, 1); break;                 // 006F299E..AE
        default: break;                                                                         // 006F29D3
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, value);                          // CSWGuiPanel::HandleInputEvent
}
}
