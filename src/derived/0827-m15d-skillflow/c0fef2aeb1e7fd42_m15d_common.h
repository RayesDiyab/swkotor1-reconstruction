#pragma once
// lane-m15-d (L15-D, Milestone 1.5): internal shared declarations of the four L15-D units
//   m15d-skillflow  (m15d_skillflow.cpp)  CSWGuiFlowSkillStruct, CSWGuiSkillFlow (table 007578A8),
//                                         CSWGuiSkillFlowChart, CSWGuiSkillInfoBox::SetSkillList,
//                                         CSWGuiInGameSkillEntry::SetSkill, colours 007A23D8 / 007A23E4
//   m15d-feats      (m15d_feats.cpp)      CSWGuiFeatsCharGen (table 007598B0)
//   m15d-skills     (m15d_skills.cpp)     CSWGuiSkillsCharGen (table 00759990), thunks 0067CB40 / 0067CB50
//   m15d-abilities  (m15d_abilities.cpp)  CSWGuiAbilitiesCharGen (table 00759C68)
// Original: swkotor.exe GOG K1 (image base 0x400000), static disassembly only; every body cites its instruction
// addresses. Conventions: D1 fastcall veneers (callers pass nullptr EDX; callee cleanup identical), D21 typed constant
// method tables, D24 typed slot views read at the call site (vt<V>(object)), D47 single-inheritance member callbacks,
// D30-FINAL (no float arithmetic in these panels: only constant float stores), D107 (static PMF size asserts),
// D117 (no string extern punning), D132 (one helper load per original load; rp()/rd32() spellings for EXTRA-DEREF-SCAN).
// EH: every original EH state that destroys a constructed member is reproduced with C++ try/catch or RAII because
// the callees are noexcept(false) (D30-FINAL unwind rule).
#include "8c61ee1430fb8439_gui_objects.h"      // gui_load views, gui_lb (ListBox/Image, vt), kih (lane-input)
#include "c9d0373605f94236_mainmenu.h"         // AddPanel 0040BC70, SetCallback 00624A40, SetCustomToken 0041DB50, SetAllowCancel 00627130,
                              // GetInGameGui 005ED690, Panel::Draw 0040B760, TLK cell 007A3A08
#include "29a305efc7265995_m1_shared.h"        // 00624BA0/00624BB0/00624BC0 panel event thunks, GetSelectedControl 0041A5D0
#include "b8824978385052ca_m15_l0_gui.h"       // L15-0: OnYButtonPressed thunk 00644720, SetVisible 0040B9D0, ShowTutorialWindow,
                              //        CExoArrayList<WORD> helpers (m15_l0.h)
#include "bf662019daaaa2b6_m15b_stats.h"       // L15-B: CSWCLevelUpStats / CSWCCreatureStats / CSWClass / CSWFeat / CSWSkill accessors
#include "a200cba35d5f8b9c_m15c_chargen.h"     // L15-C: CSWGuiCustomPanel parent notifications 006EF480/006EF500/006EF520/006EF530
#include "525fe8bd23209b6c_m15_traps.h"        // L15-F: CSWGuiLevelUpPanel parent notification traps 006EE0D0/006EE580/006EE5B0/006EE5D0
#include "40ced824a174dc05_m15d_gui.h"         // this lane: PanelStepThunks 0067CB40/0067CB50
#include "232ef6efbadb2bc2_startup_views.h"    // M40 kotor_menu_10_19::Tlk::simple_0041e8f0 (CTlkTable::GetSimpleString)
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <new>

// ======================================================================== external providers (M60 functional.map
// 20261007T235709513433Z-c0bca789 unless marked; declared exactly as their existing definitions)
extern "C" {
void __fastcall gui_load_text_params_set_color_00414e10(void* params, void*, const void* color);          // core47 00414E10
void __fastcall gui_load_border_params_set_pulsing_00414d00(void* params, void*, gui_load::Word a, gui_load::Word b,
                                                           gui_load::Word c);                             // gui-load 00414D00
void __fastcall gui_m48_listbox_add_controls_0041c2c0(gui_lb::ListBox*, void*, gui_load::Control** controls,
    gui_load::I32 count, gui_load::Word suppress, gui_load::Word restore, gui_load::Word variable) noexcept(false);  // m48b 0041C2C0
void __fastcall kotor_fastcall_0041c1d0(gui_lb::ListBox*, void*, void* list, gui_load::Word, gui_load::Word,
                                        gui_load::Word) noexcept(false);                                  // m48t 0041C1D0
void* __fastcall kotor_fastcall_0040be00(void* gui, void*);                                               // r4-lowers PopModalPanel
kotor_reconstruction::StartupString* __fastcall resman_string_from_int_005e5bc0(kotor_reconstruction::StartupString*,
                                                                                 void*, int) noexcept(false);   // 005E5BC0
void* __fastcall kotor_fastcall_005eda40(void* client, void*);                                            // GetSWGuiManager
menu_resources::Ref* __fastcall unresolved_ref_assign_00406290(menu_resources::Ref*, void*, menu_resources::Ref* out,
                                                                const char* text);                       // 00406290
void* __fastcall async_ref_assign_string_004061f0(void* ref, void*, void* out, void* text);              // 004061F0
extern void* kotor_cell_rules_007a3a28;                                                                   // CSWRules* cell
// L15-A (lane-m15-a, m15a-creature; MINE in its callee list): 0060FB20 CSWCCreature::GetServerCreature RET.
// Level-up branches only (ctors with level_up != 0: 006F53B3, 006F8042).
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void* creature, void*) noexcept(false);
}
// GuiAssign005e5140 (005E5140) and GuiConcat005e5d10 (005E5D10): declared by string_priority.h (via gui_objects.h).
namespace alias_add { int __fastcall Greater005e5490(kotor_reconstruction::StartupString*, void*,
                                                     kotor_reconstruction::StartupString*); }             // 005E5490
namespace tlk_core { void __fastcall ParseStr0041dd30(kotor_menu_10_19::Tlk*, void*,
                                                      kotor_reconstruction::StartupString*); }            // 0041DD30
namespace kotor_rules { void* __fastcall kotor_fastcall_00550c00(void* rules, void*, unsigned short feat); }   // GetFeat
namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void* server, void*); }          // 004AEE70

namespace kotor_m15d {
namespace kih = kotor_input_hover;
using gui_load::Word;
using gui_load::I32;
using gui_load::Object;
using gui_load::Panel;
using gui_load::Control;
using gui_load::Label;
using gui_load::Button;
using gui_load::Border;
using gui_load::Extent;
using gui_load::String;
using gui_load::Ref;
using gui_lb::ListBox;
using gui_lb::vt;
using kotor_reconstruction::StartupString;
using U8 = std::uint8_t;
using U16 = std::uint16_t;
using I8 = std::int8_t;

// ---- raw accessors (memcpy, no puns; exactly one memory access per call -- D132)
template <class T> inline T ld(const void* base, unsigned offset) {
    T value;
    std::memcpy(&value, static_cast<const unsigned char*>(base) + offset, sizeof value);
    return value;
}
template <class T> inline void st(void* base, unsigned offset, T value) {
    std::memcpy(static_cast<unsigned char*>(base) + offset, &value, sizeof value);
}
inline void* rp(const void* base, unsigned offset) { return ld<void*>(base, offset); }
inline Word rd32(const void* base, unsigned offset) { return ld<Word>(base, offset); }
inline unsigned char* at(void* base, unsigned offset) { return static_cast<unsigned char*>(base) + offset; }
inline const unsigned char* at(const void* base, unsigned offset) { return static_cast<const unsigned char*>(base) + offset; }
inline Word word_of(const void* p) { return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p)); }
inline void* ptr_of(Word w) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(w)); }

// ---- CExoArrayList<T> storage {data, count, capacity} (12 bytes; e.g. 006F3E94..ED6 zero-initialised by the ctor).
struct ExoList { Word data; I32 count; I32 capacity; };
static_assert(sizeof(ExoList) == 12, "CExoArrayList");
inline U16 u16_at(const ExoList& l, I32 i) { return ld<U16>(ptr_of(l.data), 2u * static_cast<unsigned>(i)); }
inline void set_u16_at(ExoList& l, I32 i, U16 v) { st<U16>(ptr_of(l.data), 2u * static_cast<unsigned>(i), v); }
inline Word u32_at(const ExoList& l, I32 i) { return ld<Word>(ptr_of(l.data), 4u * static_cast<unsigned>(i)); }
inline void set_u32_at(ExoList& l, I32 i, Word v) { st<Word>(ptr_of(l.data), 4u * static_cast<unsigned>(i), v); }
// Inline CExoArrayList::Add growth (cmp count,capacity; capacity ? capacity*2 : 10h; operator new 006FA7E6 (size *
// element); element copy loop; _free 006FA390 of the old block) -- the original inlines it at every Add site.
inline void grow_list(ExoList& l, unsigned element) {
    const I32 capacity = l.capacity != 0 ? l.capacity * 2 : 0x10;
    const Word old = l.data;
    l.capacity = capacity;
    void* fresh = c2da_directory_new_006fa7e6(static_cast<std::uint32_t>(capacity) * element);
    l.data = word_of(fresh);
    for (I32 i = 0; i < l.count; ++i)
        std::memcpy(at(fresh, static_cast<unsigned>(i) * element), at(ptr_of(old), static_cast<unsigned>(i) * element), element);
    if (old != 0) live_free(ptr_of(old));
}
// Inline linear search (cmp word ptr [p],x / inc / add 2) returning the index or -1.
inline I32 find_u16(const ExoList& l, U16 v) {
    for (I32 i = 0; i < l.count; ++i)
        if (u16_at(l, i) == v) return i;
    return -1;
}

// ---- CExoString hidden-return storage: constructed by the callee, destroyed by the caller.
struct StringSlot {
    alignas(4) unsigned char bytes[sizeof(StartupString)];
    bool live = false;
    StartupString* raw() { return reinterpret_cast<StartupString*>(bytes); }
    StartupString* get() { return std::launder(raw()); }
    ~StringSlot() { if (live) get()->~StartupString(); }
};
// CExoString passed BY VALUE (two words; the callee owns and destroys its copy), as GuiM48StringBits.
struct StringBits { Word data, capacity; };
static_assert(sizeof(StringBits) == 8, "CExoString by value");

// ---- Text / border / control offsets (gui_load views)
constexpr unsigned kLabelText = 0xE8;      // CSWGuiLabel text params: +D0 CSWGuiText, +18 params (0041B9D8)
constexpr unsigned kLabelTextObject = 0xD0;
constexpr unsigned kButtonText = 0x16C;    // CSWGuiButton text params: +154 CSWGuiText, +18 params (0041C9E2)
constexpr unsigned kButtonBorderColor = 0x90;   // CSWGuiButton +6C border, +14 params, +10 colour
constexpr unsigned kTextFlags = 0x38;      // CSWGuiTextParams flags (00415F27)
constexpr unsigned kControlFlags = 0x44;   // CSWGuiControl flags (bit1 enabled-look, bit2 sound)
constexpr unsigned kControlId = 0x58;      // CSWGuiControl +58 user word
constexpr unsigned kListProto = 0x2CC;     // CSWGuiListBox proto item
constexpr unsigned kListByte2C1 = 0x2C1;   // CSWGuiListBox +2C1

// ---- D24 slot views (only the called slots are typed; receiver = the object whose vptr is read)
struct LabelInitView { const void* s00_94[0x26]; void (__fastcall* initialize98)(Object*, void*, Word, Word, Word); };
static_assert(offsetof(LabelInitView, initialize98) == 0x98, "label +98 (00418CD0)");
struct TextIdealView { const void* s00; const void* s04; Word (__fastcall* ideal_height)(gui_load::Text*, void*); };
static_assert(offsetof(TextIdealView, ideal_height) == 0x08, "text +08 (00414EB0)");
struct InputView { const void* s00_38[0x0F]; void (__fastcall* handle_input_event)(void*, void*, I32, I32); };
static_assert(offsetof(InputView, handle_input_event) == 0x3C, "+3C HandleInputEvent");
struct ActiveView { const void* s00_3c[0x10]; void (__fastcall* set_active)(void*, void*, I32); };
static_assert(offsetof(ActiveView, set_active) == 0x40, "+40 SetActive");
struct ListSelectView { const void* s00_a0[0x29]; void (__fastcall* select_on_left_click)(ListBox*, void*, Word); };
static_assert(offsetof(ListSelectView, select_on_left_click) == 0xA4, "listbox +A4 (0041B120)");
struct ExtentView { const void* s00; void (__fastcall* set_extent)(void*, void*, const Extent*); };
static_assert(offsetof(ExtentView, set_extent) == 0x04, "+04 SetExtent");
struct Draw0CView { const void* s00_08[3]; void (__fastcall* draw)(void*, void*, Word); };
static_assert(offsetof(Draw0CView, draw) == 0x0C, "image/border +0C Draw");
struct DeleteView { void* (__fastcall* deleting_destroy)(void*, void*, Word); };
struct PanelActiveView { const void* s00_04[2]; void (__fastcall* set_active_control)(void*, void*, void*, Word); };
static_assert(offsetof(PanelActiveView, set_active_control) == 0x08, "+08 SetActiveControl");
// CSWGuiMessageBox (table 0074FDB0): +6C SetMessage(strref) 006249D0, +70 SetMessage(CExoString by value) 006271A0.
struct MessageBoxView {
    const void* s00_68[0x1B];
    void (__fastcall* set_message_strref)(void*, void*, Word);
    void (__fastcall* set_message)(void*, void*, StringBits);
};
static_assert(offsetof(MessageBoxView, set_message_strref) == 0x6C && offsetof(MessageBoxView, set_message) == 0x70,
              "message box +6C/+70");

// ---- application cells
// CAppManager* 007A39FC (M40 owner kotor_menu_window_cells::manager_007a39fc): +4 CClientExoApp*, +8 CServerExoApp*.
inline void* client_app() { return rp(kotor_menu_window_cells::manager_007a39fc, 4); }
inline void* rules() { return kotor_cell_rules_007a3a28; }                    // [007A3A28] CSWRules*
inline kotor_menu_10_19::Tlk* tlk() { return static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08); }
// [creature+2F8] CSWCLevelUpStats* (char-gen creature stats; 006F53B3.. etc.)
inline void* stats_of(const void* creature) { return rp(creature, 0x2F8); }
inline kih::Manager* manager_of(const Panel* panel) { return reinterpret_cast<kih::Manager*>(panel->manager); }  // [this+18h]
inline kih::GuiObject* event_target(void* panel) { return reinterpret_cast<kih::GuiObject*>(panel); }
inline void* feat_of(U16 feat) { return kotor_rules::kotor_fastcall_00550c00(rules(), nullptr, feat); }   // CSWRules::GetFeat

// String temporary + CSWGuiPanel::InitControl(control, &name, 1) (005E5A90 / 0040B930 / 005E5C20).
inline void init_control(Panel* panel, Control* control, const char* tag) {
    String name(tag);
    gui_load_panel_init_control_0040b930(panel, nullptr, control, &name, 1);
}
// CSWGuiTextParams::SetText(CExoString(int)) with the temporary destroyed afterwards (005E5BC0 / 00415E00 / 005E5C20).
inline void set_text_int(void* params, int value) {
    StringSlot text;
    resman_string_from_int_005e5bc0(text.raw(), nullptr, value); text.live = true;
    gui_load_text_params_set_text_00415e00(params, nullptr, text.get());
}
inline void copy_color(void* to, const void* color) { std::memcpy(to, color, 12); }
// Common message-box sequence: mb = [GetInGameGui()+98h]; SetAllowCancel(allow); SetCallback(0,0,0) or the given
// callback; vslot +6C SetMessage(strref); AddPanel(manager, mb, 1, 1).
inline void* message_box() { return rp(frame_R4_005ed690(client_app(), nullptr), 0x98); }
}

// ======================================================================== this lane's types
namespace kotor_m15d {
// CSWGuiFlowSkillStruct (0x128; ctor 006CCB80, dtor 006CCBE0).
struct FlowSkillStruct {
    Border border;                // +000 CSWGuiBorder (006CCB9D)
    gui_lb::Image icon;           // +074 CSWGuiImage (006CCBAD)
    gui_lb::Image indent;         // +0C8 CSWGuiImage (006CCBBD)
    I32 skill;                    // +11C = -1 (006CCE07); id (006CDB43)
    U8 status;                    // +120 = 0 (006CCE0A); SetSkillStatus byte (006CCFF2)
    U8 pad121[3];
    Word flags;                   // +124 bit0 selected (006CCE04, 006CD4F7)
};
static_assert(offsetof(FlowSkillStruct, icon) == 0x74 && offsetof(FlowSkillStruct, indent) == 0xC8 &&
              offsetof(FlowSkillStruct, skill) == 0x11C && offsetof(FlowSkillStruct, status) == 0x120 &&
              offsetof(FlowSkillStruct, flags) == 0x124 && sizeof(FlowSkillStruct) == 0x128, "flow skill struct");
// CSWGuiSkillFlow (0x47C, operator new at 006CDAA6; ctor 006CCCC0; table 007578A8).
struct SkillFlow : Control {
    FlowSkillStruct cells[3];     // +05C (eh vector 128h x 3, 006CCCE6..D0A)
    gui_lb::Image arrows[2];      // +3D4 (eh vector 54h x 2, 006CCD0F..29)
};
static_assert(offsetof(SkillFlow, cells) == 0x5C && offsetof(SkillFlow, arrows) == 0x3D4 && sizeof(SkillFlow) == 0x47C,
              "skill flow");
// CSWGuiSkillFlowChart (embedded, 0x10; ctor 006CD510): CExoArrayList<CSWGuiSkillFlow*> + selected column/row.
struct SkillFlowChart {
    ExoList flows;                // +00 data, +04 count, +08 capacity (006CD514..1A)
    U8 col;                       // +0C (006CD51C; 006ABA5D)
    U8 row;                       // +0D (006CD51F; 006ABA57)
    U8 pad0e[2];
};
static_assert(sizeof(SkillFlowChart) == 0x10 && offsetof(SkillFlowChart, col) == 0x0C && offsetof(SkillFlowChart, row) == 0x0D,
              "skill flow chart");
inline SkillFlow* flow_at(const SkillFlowChart& c, I32 i) { return static_cast<SkillFlow*>(rp(ptr_of(c.flows.data), 4u * static_cast<unsigned>(i))); }

// CSWGuiFeatsCharGen (0x1A1C; ctor 006F3D60; table 007598B0; dtor 006F2A50 reverse order).
struct FeatsPanel : Panel {
    void* creature;               // +0064 (006F3F32)
    void* parent;                 // +0068 (set by the caller after the ctor: 006EF334 / 006F4621)
    Label lbl_main_title;         // +006C "MAIN_TITLE_LBL"
    Label lbl_sub_title;          // +01AC "SUB_TITLE_LBL"
    Label lbl_remaining_box;      // +02EC "REMAINING_BOX_LBL"
    Label lbl_std_remaining;      // +042C "STD_REMAINING_SELECTIONS_LBL" (remaining count text +514)
    Label lbl_std_selections;     // +056C "STD_SELECTIONS_REMAINING_LBL"
    Label lbl_desc;               // +06AC "DESC_LBL"
    Label lbl_7ec;                // +07EC (constructed, never loaded)
    Label lbl_92c;                // +092C (constructed, never loaded)
    Label desc_item;              // +0A6C description item (vslot +98 from LB_DESC proto, 006F43A7..C3)
    Label lbl_name;               // +0BAC "LBL_NAME"
    Button btn_accept;            // +0CEC "BTN_ACCEPT"
    Button btn_back;              // +0EB0 "BTN_BACK"
    Button btn_recommended;       // +1074 "BTN_RECOMMENDED"
    Button btn_select;            // +1238 "BTN_SELECT" (text +13A4, border colour +12C8)
    ListBox lb_feats;             // +13FC "LB_FEATS"
    ListBox lb_desc;              // +16DC "LB_DESC"
    ExoList granted;              // +19BC CExoArrayList<WORD> creature feats (InitiateFeats 006F34B0..)
    ExoList granted_new;          // +19C8 feats granted at this level (AddGrantedFeats, 006F35B0..)
    ExoList available;            // +19D4 choosable feats (BuildAvailableList 006F33D0)
    ExoList chosen;               // +19E0 chosen feats (AddChosenFeat 006F391D / 006F3958)
    U16 current_feat;             // +19EC (= FFFFh 006F3F53)
    U8 normal_count;              // +19EE CalcNumberFeats out_a (006F373D)
    U8 bonus_count;               // +19EF CalcNumberFeats out_b
    U8 normal_left;               // +19F0 (006F3751)
    U8 bonus_left;                // +19F1 (006F3770)
    U8 remaining;                 // +19F2 (006F3787)
    U8 pad19f3;
    Word feat_flags;              // +19F4 bit0 level-up, bit1 bonus feats, bit2 granted list shown (006F3F2C..4D, 006F370A)
    String select_text;           // +19F8 (006F42A3)
    String remove_text;           // +1A00 (006F4340)
    SkillFlowChart chart;         // +1A08 (006F3F07)
    U8 status;                    // +1A18 DetermineFeat result for the current feat (006F2FB9)
    U8 pad1a19[3];
};
static_assert(offsetof(FeatsPanel, creature) == 0x64 && offsetof(FeatsPanel, lbl_main_title) == 0x6C &&
              offsetof(FeatsPanel, desc_item) == 0xA6C && offsetof(FeatsPanel, lbl_name) == 0xBAC &&
              offsetof(FeatsPanel, btn_accept) == 0xCEC && offsetof(FeatsPanel, btn_select) == 0x1238 &&
              offsetof(FeatsPanel, lb_feats) == 0x13FC && offsetof(FeatsPanel, lb_desc) == 0x16DC &&
              offsetof(FeatsPanel, granted) == 0x19BC && offsetof(FeatsPanel, chosen) == 0x19E0 &&
              offsetof(FeatsPanel, current_feat) == 0x19EC && offsetof(FeatsPanel, remaining) == 0x19F2 &&
              offsetof(FeatsPanel, feat_flags) == 0x19F4 && offsetof(FeatsPanel, select_text) == 0x19F8 &&
              offsetof(FeatsPanel, chart) == 0x1A08 && offsetof(FeatsPanel, status) == 0x1A18 &&
              sizeof(FeatsPanel) == 0x1A1C, "CSWGuiFeatsCharGen 0x1A1C (006EF2FC push 1A1Ch)");

// CSWGuiSkillsCharGen (0x49D0; ctor 006F51D0; table 00759990; dtor 006F47E0).
struct SkillsPanel : Panel {
    void* creature;               // +0064 (006F53AE)
    void* parent;                 // +0068 (caller 006EF3C2)
    ListBox lb_desc;              // +006C "LB_DESC"
    Label lbl_main_title;         // +034C "MAIN_TITLE_LBL" (text params +434)
    Label lbl_sub_title;          // +048C "SUB_TITLE_LBL"
    Label lbl_remaining_box;      // +05CC "REMAINING_BOX_LBL"
    Label lbl_remaining;          // +070C "REMAINING_SELECTIONS_LBL" (text +7F4)
    Label lbl_selections;         // +084C "SELECTIONS_REMAINING_LBL"
    Label lbl_desc;               // +098C "DESC_LBL"
    Label lbl_cost;               // +0ACC "COST_LBL"
    Label lbl_cost_points;        // +0C0C "COST_POINTS_LBL" (text +CF4)
    Label lbl_class_skill;        // +0D4C "CLASSSKL_LBL" (text +E34)
    Label desc_item;              // +0E8C description item (vslot +98, 006F5F9A..AE)
    Label skill_labels[8];        // +0FCC (eh vector 140h x 8)
    Button point_buttons[8];      // +19CC (eh vector 1C4h x 8)
    Button btn_accept;            // +27EC "BTN_ACCEPT"
    Button btn_back;              // +29B0 "BTN_BACK"
    Button btn_recommended;       // +2B74 "BTN_RECOMMENDED"
    Button plus_buttons[8];       // +2D38
    Button minus_buttons[8];      // +3B58
    String descriptions[8];       // +4978 CSWSkill::GetDescriptionText per skill
    I32 points;                   // +49B8 remaining skill points
    I32 current;                  // +49BC current skill index
    U8 base_ranks[8];             // +49C0 ranks at panel entry
    Word skill_flags;             // +49C8 bit0 level-up, bit1 party/char-gen, bit2 droid (race word 0074BF10)
    I32 initial_points;           // +49CC
};
static_assert(offsetof(SkillsPanel, lb_desc) == 0x6C && offsetof(SkillsPanel, lbl_main_title) == 0x34C &&
              offsetof(SkillsPanel, lbl_remaining) == 0x70C && offsetof(SkillsPanel, lbl_cost_points) == 0xC0C &&
              offsetof(SkillsPanel, desc_item) == 0xE8C && offsetof(SkillsPanel, skill_labels) == 0xFCC &&
              offsetof(SkillsPanel, point_buttons) == 0x19CC && offsetof(SkillsPanel, btn_accept) == 0x27EC &&
              offsetof(SkillsPanel, btn_back) == 0x29B0 && offsetof(SkillsPanel, btn_recommended) == 0x2B74 &&
              offsetof(SkillsPanel, plus_buttons) == 0x2D38 && offsetof(SkillsPanel, minus_buttons) == 0x3B58 &&
              offsetof(SkillsPanel, descriptions) == 0x4978 && offsetof(SkillsPanel, points) == 0x49B8 &&
              offsetof(SkillsPanel, base_ranks) == 0x49C0 && offsetof(SkillsPanel, skill_flags) == 0x49C8 &&
              sizeof(SkillsPanel) == 0x49D0, "CSWGuiSkillsCharGen 0x49D0 (006EF38C push 49D0h)");

// CSWGuiAbilitiesCharGen (0x3DF4; ctor 006F7600; table 00759C68; dtor 006F6BF0). Ability order STR, DEX, CON, WIS,
// INT, CHA (CompletePanel 006F6EBB..6F42).
struct AbilitiesPanel : Panel {
    void* creature;               // +0064 (006F77E7)
    void* parent;                 // +0068 (caller 006EF452)
    ListBox lb_desc;              // +006C "LB_DESC"
    Label lbl_main_title;         // +034C (text params +434)
    Label lbl_sub_title;          // +048C
    Label lbl_remaining_box;      // +05CC
    Label lbl_remaining;          // +070C (text +7F4)
    Label lbl_selections;         // +084C (text +934)
    Label lbl_desc;               // +098C
    Label lbl_cost;               // +0ACC
    Label lbl_cost_points;        // +0C0C (text +CF4)
    Label desc_item;              // +0D4C description item (vslot +98, 006F8418..34; text +E34, object +E1C)
    Label lbl_ability_mod;        // +0E8C "LBL_ABILITY_MOD" (text +F74)
    Label lbl_modifier;           // +0FCC "LBL_MODIFIER"
    Label ability_labels[6];      // +110C
    Button point_buttons[6];      // +188C
    Button btn_accept;            // +2324
    Button btn_back;              // +24E8
    Button btn_recommended;       // +26AC
    Button plus_buttons[6];       // +2870
    Button minus_buttons[6];      // +3308
    Word descriptions[6];         // +3DA0 strrefs DEh..E3h (006F7BD3..006F8022)
    I32 points;                   // +3DB8
    I32 values[6];                // +3DBC
    I32 bases[6];                 // +3DD4
    I32 current;                  // +3DEC
    Word level_up;                // +3DF0 (006F77D2)
};
static_assert(offsetof(AbilitiesPanel, lb_desc) == 0x6C && offsetof(AbilitiesPanel, desc_item) == 0xD4C &&
              offsetof(AbilitiesPanel, lbl_ability_mod) == 0xE8C && offsetof(AbilitiesPanel, ability_labels) == 0x110C &&
              offsetof(AbilitiesPanel, point_buttons) == 0x188C && offsetof(AbilitiesPanel, btn_accept) == 0x2324 &&
              offsetof(AbilitiesPanel, btn_back) == 0x24E8 && offsetof(AbilitiesPanel, btn_recommended) == 0x26AC &&
              offsetof(AbilitiesPanel, plus_buttons) == 0x2870 && offsetof(AbilitiesPanel, minus_buttons) == 0x3308 &&
              offsetof(AbilitiesPanel, descriptions) == 0x3DA0 && offsetof(AbilitiesPanel, points) == 0x3DB8 &&
              offsetof(AbilitiesPanel, values) == 0x3DBC && offsetof(AbilitiesPanel, bases) == 0x3DD4 &&
              offsetof(AbilitiesPanel, current) == 0x3DEC && offsetof(AbilitiesPanel, level_up) == 0x3DF0 &&
              sizeof(AbilitiesPanel) == 0x3DF4, "CSWGuiAbilitiesCharGen 0x3DF4 (006EF41C push 3DF4h)");

// CSWGuiSkillInfoBox view (0x24E8; owner m48b-gui-controls ctor 006CE7C0, [CGuiInGame+9Ch]).
struct SkillInfoBoxView : Panel {
    ListBox list;                 // +064 "LB_SKILLS"
    Label message;                // +344 "LBL_MESSAGE" (text params +42C)
    Button ok;                    // +484
    unsigned char entries[10][0x310];   // +648 CSWGuiInGameSkillEntry x 10
};
static_assert(offsetof(SkillInfoBoxView, message) == 0x344 && offsetof(SkillInfoBoxView, entries) == 0x648 &&
              sizeof(SkillInfoBoxView) == 0x24E8, "skill info box view");

// Panel member callbacks (thiscall, RET 4; registered with AddEvent / CSWGuiMessageBox::SetCallback; D47).
struct FeatsHandlers : kih::GuiObject {
    void __thiscall ShowGranted_006f2c40(kih::Control* unused);              // message-box callback (006F2F76)
    void __thiscall OnFeatSelectionChanged_006f3420(kih::Control* control);  // LB_FEATS event 1F8h (006F43D0)
    void __thiscall OnDoubleClick_006f3cf0(kih::Control* control);           // LB_FEATS event 1F9h (006F43E6)
};
struct SkillsHandlers : kih::GuiObject {
    void __thiscall OnExitPointsButton_006f4a80(kih::Control* control);      // event 1 (006F5FE3)
    void __thiscall OnEnterPointsButton_006f4bf0(kih::Control* control);     // event 0 (006F5FD0)
    void __thiscall ConfirmAccept_006f4dc0(kih::Control* unused);            // message-box callback (006F69B8)
};
struct AbilitiesHandlers : kih::GuiObject {
    void __thiscall OnExitPointsButton_006f6e40(kih::Control* control);      // event 1 (006F8213)
    void __thiscall OnEnterPointsButton_006f70e0(kih::Control* control);     // event 0 (006F8200)
};
static_assert(sizeof(void (__thiscall FeatsHandlers::*)(kih::Control*)) == 4 &&
              sizeof(void (__thiscall SkillsHandlers::*)(kih::Control*)) == 4 &&
              sizeof(void (__thiscall AbilitiesHandlers::*)(kih::Control*)) == 4, "single-inheritance PMFs (D107)");
}

extern "C" {
// ======================================================================== authored by this lane (D1 veneers)
// ---- m15d-skillflow
void* __fastcall kotor_m15d_flow_skill_struct_construct_006ccb80(void* self, void*) noexcept(false);   // RET
void __fastcall kotor_m15d_flow_skill_struct_destruct_006ccbe0(void* self, void*);                    // RET
void __fastcall kotor_m15d_skill_flow_draw_006ccc40(kotor_m15d::SkillFlow* self, void*, gui_load::Word delta);  // RET 4
kotor_m15d::SkillFlow* __fastcall kotor_m15d_skill_flow_construct_006cccc0(kotor_m15d::SkillFlow* self, void*) noexcept(false);
void __fastcall kotor_m15d_skill_flow_set_extent_006cce30(kotor_m15d::SkillFlow* self, void*, const gui_load::Extent* e);  // RET 4
void __fastcall kotor_m15d_skill_flow_set_skill_status_006ccfd0(kotor_m15d::SkillFlow* self, void*, gui_load::Word col,
                                                                gui_load::Word status);        // RET 8 (low bytes)
gui_load::I32 __fastcall kotor_m15d_skill_flow_hit_check_006cd1c0(kotor_m15d::SkillFlow* self, void*, gui_load::I32 x,
                                                                   gui_load::I32 y);           // RET 8
void* __fastcall kotor_m15d_skill_flow_deleting_destruct_006cd420(kotor_m15d::SkillFlow* self, void*, gui_load::Word flags);
void __fastcall kotor_m15d_skill_flow_destruct_006cd440(kotor_m15d::SkillFlow* self, void*);
void __fastcall kotor_m15d_skill_flow_set_selected_006cd4c0(kotor_m15d::SkillFlow* self, void*, gui_load::Word col,
                                                            gui_load::Word selected);          // RET 8
gui_load::I32 __fastcall kotor_m15d_chart_get_selected_006aba50(kotor_m15d::SkillFlowChart* self, void*);
void __fastcall kotor_m15d_chart_clear_006cd240(kotor_m15d::SkillFlowChart* self, void*);
void __fastcall kotor_m15d_chart_set_skill_status_006cd280(kotor_m15d::SkillFlowChart* self, void*, gui_load::I32 skill,
                                                           gui_load::Word status);             // RET 8
void __fastcall kotor_m15d_chart_set_skill_status_all_006cd2e0(kotor_m15d::SkillFlowChart* self, void*, gui_load::Word status);
kotor_m15d::SkillFlowChart* __fastcall kotor_m15d_chart_construct_006cd510(kotor_m15d::SkillFlowChart* self, void*);
void __fastcall kotor_m15d_chart_destruct_006cd530(kotor_m15d::SkillFlowChart* self, void*);
void __fastcall kotor_m15d_chart_build_sorted_feat_list_006cd960(kotor_m15d::SkillFlowChart* self, void*, void* list) noexcept(false);
void __fastcall kotor_m15d_chart_add_feat_set_006cda80(kotor_m15d::SkillFlowChart* self, void*, gui_load::I32 a,
                                                       gui_load::I32 b, gui_load::I32 c) noexcept(false);   // RET 0Ch
void __fastcall kotor_m15d_chart_set_selected_skill_006cdc00(kotor_m15d::SkillFlowChart* self, void*, gui_load::I32 skill);
gui_load::I32 __fastcall kotor_m15d_chart_set_top_left_006cdd10(kotor_m15d::SkillFlowChart* self, void*);
gui_load::I32 __fastcall kotor_m15d_chart_handle_input_006cdd80(kotor_m15d::SkillFlowChart* self, void*, gui_load::I32 event);
void __fastcall kotor_m15d_skill_info_set_skill_list_006cdfc0(void* box, void*, void* list, gui_load::Word type) noexcept(false);
void __fastcall kotor_m15d_chart_create_feat_chart_006ce570(kotor_m15d::SkillFlowChart* self, void*, void* stats,
                                                            gui_load::Word bonus) noexcept(false);   // RET 8
void __fastcall kotor_m15d_skill_entry_set_skill_006acb40(void* entry, void*, gui_load::Word id, gui_load::Word type) noexcept(false);
extern const struct GuiM15dSkillFlowMethods gui_m15d_skill_flow_methods_007578a8;
}
