// lane-m15-d / unit m15d-skillflow: feat-chart controls used by CSWGuiFeatsCharGen (and the in-game level-up
// feats panel): CSWGuiFlowSkillStruct 006CCB80/006CCBE0, CSWGuiSkillFlow 006CCC40..006CD4C0 + method table 007578A8,
// CSWGuiSkillFlowChart 006ABA50 / 006CD240..006CDD80 / 006CE570, CSWGuiSkillInfoBox::SetSkillList 006CDFC0,
// CSWGuiInGameSkillEntry::SetSkill 006ACB40, and the .data colours 007A23D8 / 007A23E4.
// Original: swkotor.exe GOG K1, static disassembly only; every behaviour cites its instruction addresses.
// Not authored here (power chart, in-game only): 006CD590 BuildSortedPowerList, 006CD7E0 AddPowerSet,
// 006CE0F0 CreatePowerChart, 006CE370 CreateFeatChart(other overload).
#include "c0fef2aeb1e7fd42_m15d_common.h"

using namespace kotor_m15d;

extern "C" {
// .data colours (static words of the image, never written; values read from the file):
const KotorM15dColor kotor_m15d_color_007a23d8 = {{0x3F3D70A4ul, 0x3DE147AEul, 0x00000000ul}};   // 007A23D8..E0
const KotorM15dColor kotor_m15d_color_007a23e4 = {{0x3E8F5C29ul, 0x3F6B851Ful, 0x3DE147AEul}};   // 007A23E4..EC
extern const MenuColor borrowed_enter_color_007a23c0;                                            // input-cells 007A23C0
}

namespace {
constexpr Word kOne = 0x3F800000u;      // 1.0f (006CCFED mov edi,3F800000h)
constexpr Word kHalf = 0x3F000000u;     // 0.5f (006CD059)
constexpr Word kQuarter = 0x3E800000u;  // 0.25f (006CD090 / 006CD11F / 006CD12C)
// CSWGuiBorderParams +0C alpha (dword store), CSWGuiImageParams +14 alpha (dword store).
void set_border_alpha(FlowSkillStruct& cell, Word bits) { st<Word>(cell.border.params, 0x0C, bits); }
void set_image_alpha(gui_lb::Image& image, Word bits) { st<Word>(&image.params, 0x14, bits); }
void set_border_color(FlowSkillStruct& cell, const void* color) { copy_color(at(cell.border.params, 0x10), color); }
void draw0c(void* object, Word delta) { vt<Draw0CView>(static_cast<const Object*>(object))->draw(object, nullptr, delta); }
void extent04(void* object, const Extent* e) { vt<ExtentView>(static_cast<const Object*>(object))->set_extent(object, nullptr, e); }

void construct_cells(SkillFlow* self) {      // eh_vector_constructor_iterator(+5C, 128h, 3, 006CCB80, 006CCBE0)
    for (int i = 0; i < 3; ++i) {
        try { kotor_m15d_flow_skill_struct_construct_006ccb80(&self->cells[i], nullptr); }
        catch (...) { while (i-- > 0) kotor_m15d_flow_skill_struct_destruct_006ccbe0(&self->cells[i], nullptr); throw; }
    }
}
void construct_arrows(SkillFlow* self) {     // eh_vector_constructor_iterator(+3D4, 54h, 2, 00415A30, 00415AD0)
    for (int i = 0; i < 2; ++i) {
        try { gui_load_image_construct_00415a30(&self->arrows[i], nullptr); }
        catch (...) { while (i-- > 0) gui_load_image_destruct_00415ad0(&self->arrows[i], nullptr); throw; }
    }
}
void destroy_cells(SkillFlow* self) {        // eh_vector_destructor_iterator(+5C, 128h, 3, 006CCBE0): reverse order
    for (int i = 3; i-- > 0;) kotor_m15d_flow_skill_struct_destruct_006ccbe0(&self->cells[i], nullptr);
}
void destroy_arrows(SkillFlow* self) {       // eh_vector_destructor_iterator(+3D4, 54h, 2, 00415AD0)
    for (int i = 2; i-- > 0;) gui_load_image_destruct_00415ad0(&self->arrows[i], nullptr);
}
// Shared tail of SetSelectedSkill 006CDCBD..CF9 / HandleInput 006CDF14..F50: select the chart's current cell.
void select_current(SkillFlowChart* self) {
    const U8 col = self->col;                                                      // 006CDCBD / 006CDF14
    SkillFlow* const flow = flow_at(*self, self->row);                             // 006CDCC0..C6 / 006CDF17..1D
    FlowSkillStruct& cell = flow->cells[col];                                      // 006CDCCD..D6
    if ((cell.flags & 1u) == 0) {                                                  // 006CDCD9..E1
        cell.flags |= 1u;                                                          // 006CDCE3..E6
        kotor_m15d_skill_flow_set_skill_status_006ccfd0(flow, nullptr, col, cell.status);   // 006CDCEC..F9
    }
}
// Deselect cell (col) of chart row (row): 006CDC73..CB3 / 006CDEE1..F0F.
void deselect(SkillFlow* flow, U8 col) {
    FlowSkillStruct& cell = flow->cells[col];
    if ((cell.flags & 1u) != 0) {                                                  // 006CDC91 / 006CDEF8
        cell.flags &= ~1u;                                                         // 006CDC9D..A0 / 006CDEFC..FF
        kotor_m15d_skill_flow_set_skill_status_006ccfd0(flow, nullptr, col, cell.status);   // 006CDCA6..AE / 006CDF05..0F
    }
}
// CClientExoApp::GetSWGuiManager()->PlayGuiSound(1): PUSH 1 precedes the argument-less GetSWGuiManager call
// (006CDE98..A1 / 006CDEBA..C3), so the pushed word is PlayGuiSound's argument.
void play_click() {
    void* const app = client_app();                                                // [[007A39FC]+4] (006CDE90..95)
    reinterpret_cast<kih::Manager*>(kotor_fastcall_005eda40(app, nullptr))->PlayGuiSound(1);
}
}

// ======================================================================== D21 method table 007578A8 (38 slots)
// Read from .rdata 007578A8..0075793F: CSWGuiControl 0073E488 except +00 006CD420, +04 006CCE30, +38 006CCC40 and
// +74 00641DB0 (self_return instead of return_zero).
struct GuiM15dSkillFlowMethods {
    void* (__fastcall* s00)(SkillFlow*, void*, Word);                              // +00 006CD420 deleting dtor
    void (__fastcall* s04)(SkillFlow*, void*, const Extent*);                      // +04 006CCE30 SetExtent
    void (__fastcall* s08)(Object*, void*, Word, Word);                            // +08 005B5E90
    void (kih::Control::* s0c)(int);                                               // +0C 004189A0 HandleMouseOver
    Word (__fastcall* s10)(Object*, void*, Word, Word);                            // +10 00409EB0
    void (__fastcall* s14)(Object*, void*);                                        // +14 004015A0
    void (kih::Control::* s18)();                                                  // +18 0041AB90 HandleLMouseDown
    void (kih::Control::* s1c)();                                                  // +1C 0041ABD0 HandleLMouseUp
    void (__fastcall* s20)(Object*, void*);                                        // +20 004015A0
    void (__fastcall* s24)(Object*, void*);                                        // +24 0041AC70 HandleRMouseUp
    Word (__fastcall* s28)(Object*, void*);                                        // +28 0063E7F0
    Word (__fastcall* s2c)(Object*, void*);                                        // +2C 0063E7F0
    Object* (__fastcall* s30)(Object*, void*);                                     // +30 00641DB0
    Word (__fastcall* s34)(Object*, void*);                                        // +34 004064A0
    void (__fastcall* s38)(SkillFlow*, void*, Word);                               // +38 006CCC40 Draw
    void (kih::Control::* s3c)(int, int);                                          // +3C 00418750 HandleInputEvent
    void (kih::Control::* s40)(int);                                               // +40 004181D0 SetActive
    kih::Control* (kih::Control::* s44)(int, int);                                 // +44 004187C0 HitCheckMouse
    void (__fastcall* s48)(Control*, void*, gui_load::Gff*, gui_load::Struct*);    // +48 0041B8E0 Load
    Word (__fastcall* s4c_70[10])(Object*, void*);                                 // +4C..+70 0063E7F0
    Object* (__fastcall* s74)(Object*, void*);                                     // +74 00641DB0 (as skill flow)
    Word (__fastcall* s78)(Object*, void*);                                        // +78 0063E7F0
    void (__fastcall* s7c)(Object*, void*, Word);                                  // +7C 00418960 HandleFocusChange
    int (kih::Control::* s80)();                                                   // +80 004189D0 GetIsSelectable
    void (__fastcall* s84)(Object*, void*);                                        // +84 0041AC30 HandleRMouseDown
    void (__fastcall* s88)(Object*, void*, Word);                                  // +88 004176A0 SetEnabled
    void (__fastcall* s8c)(Object*, void*);                                        // +8C 004015A0
    Word (__fastcall* s90)(Object*, void*);                                        // +90 00418A90 DisplayToolTip
    void (__fastcall* s94)(Object*, void*, Word, Word);                            // +94 005B5E90
};
static_assert(sizeof(GuiM15dSkillFlowMethods) == 0x98 && offsetof(GuiM15dSkillFlowMethods, s38) == 0x38 &&
              offsetof(GuiM15dSkillFlowMethods, s74) == 0x74 && offsetof(GuiM15dSkillFlowMethods, s94) == 0x94,
              "007578A8 slot count (38)");
extern "C" const GuiM15dSkillFlowMethods gui_m15d_skill_flow_methods_007578a8 = {
    &kotor_m15d_skill_flow_deleting_destruct_006cd420,   // +00
    &kotor_m15d_skill_flow_set_extent_006cce30,          // +04
    &gui_load_slot_ret8_005b5e90,                        // +08
    &kih::Control::HandleMouseOver,                      // +0C
    &gui_load_slot_zero8_00409eb0,                       // +10
    &gui_load_slot_ret_004015a0,                         // +14
    &kih::Control::HandleLMouseDown,                     // +18
    &kih::Control::HandleLMouseUp,                       // +1C
    &gui_load_slot_ret_004015a0,                         // +20
    &gui_load_control_handle_rmouse_up_0041ac70,         // +24
    &gui_load_slot_zero_0063e7f0,                        // +28
    &gui_load_slot_zero_0063e7f0,                        // +2C
    &gui_load_slot_self_00641db0,                        // +30
    &gui_load_slot_prop10_004064a0,                      // +34
    &kotor_m15d_skill_flow_draw_006ccc40,                // +38
    &kih::Control::HandleInputEvent,                     // +3C
    &kih::Control::SetActive,                            // +40
    &kih::Control::HitCheckMouse,                        // +44
    &gui_load_control_load_0041b8e0,                     // +48
    {&gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0,   // +4C +50 +54
     &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0,   // +58 +5C +60
     &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0,   // +64 +68 +6C
     &gui_load_slot_zero_0063e7f0},                                                                // +70
    &gui_load_slot_self_00641db0,                        // +74
    &gui_load_slot_zero_0063e7f0,                        // +78
    &gui_load_control_focus_change_00418960,             // +7C
    &kih::Control::GetIsSelectable,                      // +80
    &gui_load_control_rmouse_down_0041ac30,              // +84
    &gui_load_control_set_enabled_004176a0,              // +88
    &gui_load_slot_ret_004015a0,                         // +8C
    &gui_load_control_tooltip_00418a90,                  // +90
    &gui_load_slot_ret8_005b5e90,                        // +94
};

extern "C" {
// ======================================================================== CSWGuiFlowSkillStruct
// 006CCB80 ctor (RET, EAX = this): Border +0, Image +74, Image +C8 (EH states 0..1 destroy the built members).
void* __fastcall kotor_m15d_flow_skill_struct_construct_006ccb80(void* memory, void*) noexcept(false) {
    FlowSkillStruct* const self = static_cast<FlowSkillStruct*>(memory);
    gui_load_border_construct_004167f0(&self->border, nullptr);                    // 006CCB97..9D
    try {
        gui_load_image_construct_00415a30(&self->icon, nullptr);                   // 006CCBA2..AD (state 0)
        try { gui_load_image_construct_00415a30(&self->indent, nullptr); }          // 006CCBB2..BD (state 1)
        catch (...) { gui_load_image_destruct_00415ad0(&self->icon, nullptr); throw; }
    } catch (...) { gui_load_border_destruct_004151c0(&self->border, nullptr); throw; }
    return self;                                                                   // 006CCBC6
}
// 006CCBE0 dtor (RET): reverse member order.
void __fastcall kotor_m15d_flow_skill_struct_destruct_006ccbe0(void* object, void*) {
    FlowSkillStruct* const self = static_cast<FlowSkillStruct*>(object);
    gui_load_image_destruct_00415ad0(&self->indent, nullptr);                      // 006CCBFD..0B
    gui_load_image_destruct_00415ad0(&self->icon, nullptr);                        // 006CCC10..18
    gui_load_border_destruct_004151c0(&self->border, nullptr);                     // 006CCC1D..27
}

// ======================================================================== CSWGuiSkillFlow
// 006CCC40 Draw(delta) (RET 4): cells with a skill draw indent, icon, border (vslot +0C); arrow k is drawn when
// cell k+1 holds a skill.
void __fastcall kotor_m15d_skill_flow_draw_006ccc40(SkillFlow* self, void*, Word delta) {
    for (int k = 0; k < 3; ++k) {                                                  // 006CCC4A..84
        FlowSkillStruct& cell = self->cells[k];
        if (cell.skill == -1) continue;                                            // 006CCC55..59
        draw0c(&cell.indent, delta);                                               // 006CCC5B..60
        draw0c(&cell.icon, delta);                                                 // 006CCC63..6A
        draw0c(&cell.border, delta);                                               // 006CCC6D..7A
    }
    for (int k = 0; k < 2; ++k) {                                                  // 006CCC86..AE
        if (self->cells[k + 1].skill == -1) continue;                              // 006CCC97..9A (+2A0 / +3C8)
        draw0c(&self->arrows[k], delta);                                           // 006CCC9C..A1
    }
}

// 006CCCC0 ctor (RET, EAX = this). EH states: 0 control, 1 cells, 2 arrows.
SkillFlow* __fastcall kotor_m15d_skill_flow_construct_006cccc0(SkillFlow* self, void*) noexcept(false) {
    gui_load_control_construct_0041aa80(self, nullptr);                            // 006CCCDA..E1
    int built = 0;
    try {
        gui_lb::set_vptr(self, &gui_m15d_skill_flow_methods_007578a8);           // 006CCD03 (7578A8)
        construct_cells(self);                       built = 1;                    // 006CCCE6..D0A
        construct_arrows(self);                      built = 2;                    // 006CCD0F..29
        for (int k = 0; k < 3; ++k) {                                              // 006CCD33..B4
            FlowSkillStruct& cell = self->cells[k];
            Ref corner; corner.construct_00406d80("border2d");                    // 006CCD40..49 (00757898)
            gui_load_border_params_set_corner_00414ae0(cell.border.params, nullptr, &corner, 0);   // 006CCD4E..5D
            Ref edge; edge.construct_00406d80("border1d");                        // 006CCD62..6B (0075788C)
            gui_load_border_params_set_edge_00414b70(cell.border.params, nullptr, &edge, 0);       // 006CCD70..79
            gui_load_image_params_set_draw_style_004159c0(&cell.icon.params, nullptr, 1);         // 006CCD7E..82
            gui_load_image_params_set_alignment_00415990(&cell.icon.params, nullptr, 0x12);       // 006CCD87..8B
            Ref indent; indent.construct_00406d80("lbl_indent");                  // 006CCD90..99 (00757880)
            gui_load_image_params_set_image_00415900(&cell.indent.params, nullptr, &indent, 0);   // 006CCD9E..A8
        }
        for (int k = 0; k < 2; ++k) {                                              // 006CCDB6..F3
            gui_lb::Image& arrow = self->arrows[k];
            gui_load_image_params_set_draw_style_004159c0(&arrow.params, nullptr, 1);   // 006CCDC1..C5
            gui_load_image_params_set_alignment_00415990(&arrow.params, nullptr, 0x12); // 006CCDCA..CE
            Ref image; image.construct_00406d80("lbl_skarr");                     // 006CCDD3..DC (00757874)
            gui_load_image_params_set_image_00415900(&arrow.params, nullptr, &image, 0);  // 006CCDE1..EA
        }
        for (int k = 0; k < 3; ++k) {                                              // 006CCDF5..E14
            self->cells[k].flags &= ~1u;                                           // 006CCE04
            self->cells[k].skill = -1;                                             // 006CCE07
            self->cells[k].status = 0;                                             // 006CCE0A
        }
    } catch (...) {
        if (built >= 2) destroy_arrows(self);
        if (built >= 1) destroy_cells(self);
        gui_load_control_destruct_00419a20(self, nullptr);
        throw;
    }
    return self;                                                                   // 006CCE1B
}

// 006CCE30 SetExtent(extent) (RET 4): square cells of the extent height at left / centre / right; arrows (20h x 20h)
// between them; icons fill the border's inner extent; finally this->extent = *extent.
void __fastcall kotor_m15d_skill_flow_set_extent_006cce30(SkillFlow* self, void*, const Extent* e) {
    Extent cell{e->left, e->top, e->height, e->height};                            // 006CCE36..63 ([esp+10h])
    Extent arrow{0, 0, 0x20, 0x20};                                                // 006CCE4B..6B ([esp+20h])
    extent04(&self->cells[0].border, &cell);                                       // 006CCE6F..77 (+5C)
    extent04(&self->cells[0].indent, &cell);                                       // 006CCE7A..8B (+124)
    cell.left = (e->width - e->height) / 2 + e->left;                              // 006CCE8E..A5 (cdq/sub/sar)
    extent04(&self->cells[1].border, &cell);                                       // 006CCE9B..B4 (+184)
    extent04(&self->cells[1].indent, &cell);                                       // 006CCEB7..C8 (+24C)
    cell.left = e->left - e->height + e->width;                                    // 006CCECB..DD
    extent04(&self->cells[2].border, &cell);                                       // 006CCED5..EC (+2AC)
    extent04(&self->cells[2].indent, &cell);                                       // 006CCEEF..F00 (+374)
    arrow.top = (e->height - 0x20) / 2 + e->top;                                   // 006CCF03..13
    arrow.left = (e->width + e->height) / 4 + e->left - 0x10;                      // 006CCF17..2B (cdq/and 3/sar 2)
    extent04(&self->arrows[0], &arrow);                                            // 006CCF2F..40 (+3D4)
    arrow.left = (e->width * 3 - e->height) / 4 + e->left - 0x10;                  // 006CCF43..63
    extent04(&self->arrows[1], &arrow);                                            // 006CCF5D..72 (+428)
    for (int k = 0; k < 3; ++k) {                                                  // 006CCF75..A7 ([esp+44h] counter)
        Extent inner;
        extent04(&self->cells[k].icon,
                 gui_load_border_inner_extent_00415240(&self->cells[k].border, nullptr, &inner));   // 006CCF83..95
    }
    self->extent = *e;                                                             // 006CCFA9..BF (+4..+10)
}

// 006CCFD0 SetSkillStatus(BYTE col, BYTE status) (RET 8). Jump table 006CD198 (7 entries) for status 0..6.
void __fastcall kotor_m15d_skill_flow_set_skill_status_006ccfd0(SkillFlow* self, void*, Word col_word, Word status_word) {
    const U8 col = static_cast<U8>(col_word);                                      // 006CCFD9 (cl)
    const U8 status = static_cast<U8>(status_word);                                // 006CCFD1 (bl)
    FlowSkillStruct& cell = self->cells[col];                                      // 006CCFDD..E8
    cell.status = status;                                                          // 006CCFF2 (+17C)
    set_border_alpha(cell, kOne);                                                  // 006CCFF8 (+7C)
    set_image_alpha(cell.icon, kOne);                                              // 006CCFFB (+10C)
    set_image_alpha(cell.indent, kOne);                                            // 006CD001 (+160)
    if (col > 0) set_image_alpha(self->arrows[col - 1], kOne);                     // 006CD007..0E (+3BC + col*54h)
    const bool selected = (cell.flags & 1u) != 0;                                  // test [eax+180h],1
    switch (status) {                                                              // 006CD015..26
    case 0:                                                                        // 006CD12C
        set_image_alpha(cell.icon, kQuarter);
        if (selected) set_border_color(cell, &borrowed_enter_color_007a23c0);     // 006CD13E..5B (007A23C0..C8)
        else set_border_alpha(cell, 0);                                            // 006CD160
        break;
    case 1: case 2:                                                                // 006CD02D
        set_border_color(cell, &kotor_m15d_color_007a23e4);                        // 006CD02D..4A (007A23E4..EC)
        if (!selected) set_border_alpha(cell, kHalf);                              // 006CD04D..59
        break;
    case 3:                                                                        // 006CD08A
        set_image_alpha(cell.icon, kQuarter);                                      // 006CD095
        set_image_alpha(cell.indent, kQuarter);                                    // 006CD09B
        if (selected) set_border_color(cell, &kotor_m15d_color_007a23d8);         // 006CD0A3..C0 (007A23D8..E0)
        else set_border_alpha(cell, 0);                                            // 006CD0C5
        if (col > 0) set_image_alpha(self->arrows[col - 1], 0);                    // 006CD0CC..D7
        break;
    case 4:                                                                        // 006CD065
        set_border_color(cell, &kotor_m15d_color_007a23e4);                        // 006CD065..82
        break;
    case 5:                                                                        // 006CD0E7
        if (selected) set_border_color(cell, &borrowed_enter_color_007a23c0);     // 006CD0EF..0C
        else set_border_alpha(cell, 0);                                            // 006CD111
        if (col > 0) set_image_alpha(self->arrows[col - 1], kQuarter);             // 006CD118..1F
        break;
    case 6:                                                                        // 006CD136
        if (selected) set_border_color(cell, &borrowed_enter_color_007a23c0);     // 006CD13E..5B
        else set_border_alpha(cell, 0);                                            // 006CD160
        break;
    default:
        break;
    }
    if (selected) gui_load_border_params_set_pulsing_00414d00(cell.border.params, nullptr, 1, 1, 0);   // 006CD16F..76
    else gui_load_border_params_set_pulsing_00414d00(cell.border.params, nullptr, 0, 0, 0);           // 006CD182..8B
}

// 006CD1C0 SkillHitCheckMouse(x, y) (RET 8): index of the first cell holding a skill whose border extent contains
// (x, y) (inclusive bounds), else 0; returns that cell's skill.
I32 __fastcall kotor_m15d_skill_flow_hit_check_006cd1c0(SkillFlow* self, void*, I32 x, I32 y) {
    I32 found = 0;                                                                 // 006CD1D3 ([esp+10h] = 0)
    for (I32 k = 0; k < 3; ++k) {                                                  // 006CD1E0..18
        const FlowSkillStruct& cell = self->cells[k];
        if (cell.skill == -1) continue;                                            // 006CD1E0..E7 ([esi+118h])
        const Extent& r = cell.border.extent;                                      // 006CD1E9..F5 (+60)
        if (x < r.left || x > r.width + r.left || y < r.top || y > r.height + r.top) continue;   // 006CD1ED..0C
        found = k;                                                                 // 006CD21C
        break;
    }
    return self->cells[found].skill;                                               // 006CD220..2E (+178)
}

// 006CD420 scalar deleting destructor (RET 4).
void* __fastcall kotor_m15d_skill_flow_deleting_destruct_006cd420(SkillFlow* self, void*, Word flags) {
    kotor_m15d_skill_flow_destruct_006cd440(self, nullptr);                         // 006CD423
    if (flags & 1u) live_free(self);                                               // 006CD428..35 (_free 006FA390)
    return self;                                                                   // 006CD438
}
// 006CD440 dtor (RET): no vptr store; arrows, cells, then CSWGuiControl::~CSWGuiControl.
void __fastcall kotor_m15d_skill_flow_destruct_006cd440(SkillFlow* self, void*) {
    destroy_arrows(self);                                                          // 006CD45D..75
    destroy_cells(self);                                                           // 006CD47A..8F
    gui_load_control_destruct_00419a20(self, nullptr);                             // 006CD494..9E
}
// 006CD4C0 SetSkillStatus(BYTE col, int selected) (RET 8): updates the selected bit and re-applies the status.
void __fastcall kotor_m15d_skill_flow_set_selected_006cd4c0(SkillFlow* self, void*, Word col, Word selected) {
    FlowSkillStruct& cell = self->cells[static_cast<U8>(col)];                     // 006CD4C5..D5
    const Word flags = cell.flags;                                                 // 006CD4CE
    if ((flags & 1u) == selected) return;                                          // 006CD4DD..E4
    cell.flags = ((flags ^ selected) & 1u) ^ flags;                                // 006CD4E6..F7
    kotor_m15d_skill_flow_set_skill_status_006ccfd0(self, nullptr, col, cell.status);   // 006CD4EF..FF
}

// ======================================================================== CSWGuiSkillFlowChart
// 006ABA50 GetSelected (RET): count > 0 ? flows[row]->cells[col].skill : -1.
I32 __fastcall kotor_m15d_chart_get_selected_006aba50(SkillFlowChart* self, void*) {
    if (self->flows.count <= 0) return -1;                                         // 006ABA50..55 / 006ABA72
    return flow_at(*self, self->row)->cells[self->col].skill;                      // 006ABA57..6A (+178)
}
// 006CD240 ClearChart (RET): delete every flow (vslot +00 with 1), zero its slot, count = 0.
void __fastcall kotor_m15d_chart_clear_006cd240(SkillFlowChart* self, void*) {
    for (I32 i = 0; i < self->flows.count; ++i) {                                  // 006CD243..6E
        SkillFlow* const flow = flow_at(*self, i);                                 // 006CD250..52
        if (flow != nullptr)
            vt<DeleteView>(static_cast<const Object*>(flow))->deleting_destroy(flow, nullptr, 1);   // 006CD255..5D
        set_u32_at(self->flows, i, 0);                                             // 006CD25F..61
    }
    self->flows.count = 0;                                                         // 006CD271
}
// 006CD280 SetSkillStatus(int skill, BYTE status) (RET 8): first flow holding the skill gets the status.
void __fastcall kotor_m15d_chart_set_skill_status_006cd280(SkillFlowChart* self, void*, I32 skill, Word status) {
    const I32 count = self->flows.count;                                           // 006CD282
    if (count <= 0) return;                                                        // 006CD287..89
    const Word data = self->flows.data;                                            // 006CD291 (read once)
    for (I32 i = 0; i < count; ++i) {                                              // 006CD293..BC
        SkillFlow* const flow = static_cast<SkillFlow*>(rp(ptr_of(data), 4u * static_cast<unsigned>(i)));   // 006CD295
        for (I32 col = 0; col < 3; ++col) {                                        // 006CD297..AD
            if (flow->cells[col].skill != skill) continue;                         // 006CD2A0..A2
            kotor_m15d_skill_flow_set_skill_status_006ccfd0(flow, nullptr, static_cast<Word>(col), status);   // 006CD2C5..CF
            return;
        }
    }
}
// 006CD2E0 SetSkillStatusAll(BYTE status) (RET 4): every cell of every flow.
void __fastcall kotor_m15d_chart_set_skill_status_all_006cd2e0(SkillFlowChart* self, void*, Word status) {
    for (I32 i = 0; i < self->flows.count; ++i)                                    // 006CD2E3..0D
        for (Word col = 0; col < 3; ++col)                                         // 006CD2F3..05
            kotor_m15d_skill_flow_set_skill_status_006ccfd0(flow_at(*self, i), nullptr, col, status);   // 006CD2F5..FC
}
// 006CD510 ctor (RET, EAX = this).
SkillFlowChart* __fastcall kotor_m15d_chart_construct_006cd510(SkillFlowChart* self, void*) {
    self->flows.count = 0;                                                         // 006CD514
    self->flows.capacity = 0;                                                      // 006CD517
    self->flows.data = 0;                                                          // 006CD51A
    self->col = 0;                                                                 // 006CD51C
    self->row = 0;                                                                 // 006CD51F
    return self;
}
// 006CD530 dtor (RET): ClearChart, free the array, data = 0.
void __fastcall kotor_m15d_chart_destruct_006cd530(SkillFlowChart* self, void*) {
    kotor_m15d_chart_clear_006cd240(self, nullptr);                                // 006CD555
    live_free(ptr_of(self->flows.data));                                           // 006CD55A..65 (_free 006FA390)
    self->flows.data = 0;                                                          // 006CD571
}
// 006CD960 BuildSortedFeatList(CExoArrayList<WORD>* list) (RET 4; ECX unused): every rules feat with a name strref,
// ordered by prerequisite depth (0 none, 1 one, 2 two) with stable insertion before the first deeper entry.
void __fastcall kotor_m15d_chart_build_sorted_feat_list_006cd960(SkillFlowChart*, void*, void* list_ptr) noexcept(false) {
    ExoList* const list = static_cast<ExoList*>(list_ptr);
    for (I32 id = 0; id < static_cast<I32>(ld<U16>(rules(), 0xA4)); ++id) {        // 006CD960..70 / 006CDA5C..6C
        const void* const feat = feat_of(static_cast<U16>(id));                    // 006CD980..81
        if (feat == nullptr || rd32(feat, 0x08) == 0) continue;                    // 006CD986..93
        const I32 depth = ld<U16>(feat, 0x38) == 0xFFFFu ? 0
                        : (ld<U16>(feat, 0x3A) != 0xFFFFu ? 1 : 0) + 1;            // 006CD999..AD
        bool inserted = false;
        for (I32 j = 0; j < list->count; ++j) {                                    // 006CD9AE..F4
            const void* const other = feat_of(u16_at(*list, j));                   // 006CD9B7..C6
            if (other == nullptr) continue;                                        // 006CD9CB..CD
            const I32 other_depth = ld<U16>(other, 0x38) == 0xFFFFu ? 0
                                  : (ld<U16>(other, 0x3A) != 0xFFFFu ? 1 : 0) + 1; // 006CD9CF..E8
            if (other_depth > depth) {                                             // 006CD9EA..EC
                kotor_m15_array_i16_insert_006169a0(list, nullptr, static_cast<std::uint32_t>(id), j);   // 006CDA06..0A
                inserted = true;
                break;
            }
        }
        if (inserted) continue;
        if (list->count == list->capacity) grow_list(*list, 2);                     // 006CD9F6..A4D (inline growth)
        set_u16_at(*list, list->count, static_cast<U16>(id));                      // 006CDA50..55
        ++list->count;                                                             // 006CDA59
    }
}
// 006CDA80 AddFeatSet(feat, second, third) (RET 0Ch): new CSWGuiSkillFlow (0x47C) with the three feats' icons,
// extent {0, 0, F2h, 28h}, appended to the chart.
void __fastcall kotor_m15d_chart_add_feat_set_006cda80(SkillFlowChart* self, void*, I32 a, I32 b, I32 c) noexcept(false) {
    Ref icon;
    c2da_default_ref_00405ed0(&icon, nullptr);                                     // 006CDA9D..A1
    void* const memory = c2da_directory_new_006fa7e6(0x47C);                       // 006CDAA6..AB (operator new)
    SkillFlow* flow = nullptr;                                                     // 006CDACC (xor ebp,ebp)
    if (memory != nullptr) {                                                       // 006CDAB9..BF
        try { flow = kotor_m15d_skill_flow_construct_006cccc0(static_cast<SkillFlow*>(memory), nullptr); }   // 006CDAC1..C3
        catch (...) { live_free(memory); throw; }                                  // EH state 0 (FuncInfo 007358DB)
    }
    const I32 ids[3] = {a, b, c};                                                  // 006CDACE..EB ([esp+14h..1Ch])
    for (unsigned k = 0; k < 3; ++k) {                                             // 006CDAEF..59
        const void* const feat = feat_of(static_cast<U16>(ids[k]));                // 006CDAF1..FF
        Ref discarded;
        if (feat != nullptr)                                                       // 006CDB04..06
            c2da_copy_00406120(&icon, nullptr, &discarded,
                               reinterpret_cast<Ref*>(const_cast<unsigned char*>(at(feat, 0x18))));   // 006CDB08..15
        else
            unresolved_ref_assign_00406290(&icon, nullptr, &discarded, "");       // 006CDB1C..2A (0073D71C "")
        FlowSkillStruct& cell = flow->cells[static_cast<U8>(k)];                   // 006CDB33..3C
        cell.skill = ids[k];                                                       // 006CDB2F / 006CDB43
        gui_load_image_params_set_image_00415900(&cell.icon.params, nullptr, &icon, 0);   // 006CDB3E..50 (+F8)
    }
    const Extent r{0, 0, 0xF2, 0x28};                                              // 006CDB65..75
    extent04(flow, &r);                                                            // 006CDB5B..7D (vslot +04)
    if (self->flows.count == self->flows.capacity) grow_list(self->flows, 4);      // 006CDB80..D0 (inline growth)
    set_u32_at(self->flows, self->flows.count, word_of(flow));                     // 006CDBD3..D8
    ++self->flows.count;                                                           // 006CDBDB..E4
}
// 006CDC00 SetSelectedSkill(int skill) (RET 4).
void __fastcall kotor_m15d_chart_set_selected_skill_006cdc00(SkillFlowChart* self, void*, I32 skill) {
    if (self->flows.count <= 0) return;                                            // 006CDC04..0F
    const Word data = self->flows.data;                                            // 006CDC17 (ebp, read once)
    I32 row = 0, col = -1;
    for (row = 0; row < self->flows.count; ++row) {                                // 006CDC1E..4F
        const SkillFlow* const flow = static_cast<const SkillFlow*>(rp(ptr_of(data), 4u * static_cast<unsigned>(row)));   // 006CDC20
        for (I32 k = 0; k < 3; ++k)                                                // 006CDC22..3D
            if (flow->cells[k].skill == skill) { col = k; break; }                 // 006CDC30..32
        if (col != -1) break;                                                      // 006CDC41..44
    }
    if (col == -1) return;                                                         // 006CDC51..5A
    if (self->col != col || self->row != row) {                                    // 006CDC5D..71
        SkillFlow* const old = static_cast<SkillFlow*>(rp(ptr_of(data), 4u * self->row));   // 006CDC73..77 (data from ebp)
        deselect(old, self->col);                                                  // 006CDC7B..B3
        self->col = static_cast<U8>(col);                                          // 006CDCB7
        self->row = static_cast<U8>(row);                                          // 006CDCBA
    }
    select_current(self);                                                          // 006CDCBD..F9
}
// 006CDD10 SetTopLeft (RET): deselect the current cell (when not 0/0 and its row exists), select flow 0 cell 0;
// returns its skill or -1 when the chart is empty.
I32 __fastcall kotor_m15d_chart_set_top_left_006cdd10(SkillFlowChart* self, void*) {
    const U8 col = self->col;                                                      // 006CDD13
    if (col != 0 || self->row != 0) {                                              // 006CDD16..1F
        if (static_cast<I32>(self->row) < self->flows.count)                       // 006CDD21..28
            kotor_m15d_skill_flow_set_selected_006cd4c0(flow_at(*self, self->row), nullptr, col, 0);   // 006CDD2A..32
        self->row = 0;                                                             // 006CDD37
        self->col = 0;                                                             // 006CDD3B
    }
    if (self->flows.count <= 0) return -1;                                         // 006CDD3F..44 / 006CDD79
    SkillFlow* const first = flow_at(*self, 0);                                    // 006CDD46..48
    FlowSkillStruct& cell = first->cells[0];
    if ((cell.flags & 1u) == 0) {                                                  // 006CDD4A..52
        cell.flags |= 1u;                                                          // 006CDD54..57
        kotor_m15d_skill_flow_set_skill_status_006ccfd0(first, nullptr, 0, cell.status);   // 006CDD5D..68
    }
    return flow_at(*self, 0)->cells[0].skill;                                      // 006CDD6D..71 (re-read)
}
// 006CDD80 HandleInput(event) (RET 4): byte table 006CDF8C / jump table 006CDF78 for event-2Fh in 0..11h:
// 2Fh/3Fh left, 30h/40h right, 31h/3Dh up, 32h/3Eh down; returns the (possibly new) selected skill.
I32 __fastcall kotor_m15d_chart_handle_input_006cdd80(SkillFlowChart* self, void*, I32 event) {
    const U8 old_row = self->row;                                                  // 006CDD88 (edi)
    const U8 old_col = self->col;                                                  // 006CDD8B (ebx)
    switch (event) {                                                               // 006CDD8E..A9
    case 0x2F: case 0x3F:                                                          // 006CDEAD left
        if (old_col > 0) { play_click(); self->col = static_cast<U8>(self->col - 1); }   // 006CDEAD..CD
        break;
    case 0x30: case 0x40:                                                          // 006CDE71 right
        if (old_col < 2 && flow_at(*self, old_row)->cells[old_col + 1].skill != -1) {    // 006CDE71..8E
            play_click();                                                          // 006CDE90..A1
            self->col = static_cast<U8>(self->col + 1);                            // 006CDEA6..CD
        }
        break;
    case 0x31: case 0x3D:                                                          // 006CDE14 up (wraps to the last row)
        if (old_row > 0) self->row = static_cast<U8>(old_row - 1);                 // 006CDE14..1D
        else self->row = static_cast<U8>(static_cast<U8>(self->flows.count) - 1);  // 006CDE1F..24
        if (flow_at(*self, self->row)->cells[old_col].skill == -1) {               // 006CDE27..44
            do { self->col = static_cast<U8>(self->col - 1); }                     // 006CDE50
            while (flow_at(*self, self->row)->cells[self->col].skill == -1);       // 006CDE53..6D
        }
        break;
    case 0x32: case 0x3E:                                                          // 006CDDB0 down (wraps to row 0)
        if (static_cast<I32>(old_row) == self->flows.count - 1) self->row = 0;     // 006CDDB0..BC
        else self->row = static_cast<U8>(old_row + 1);                             // 006CDDBE..C0
        if (flow_at(*self, self->row)->cells[old_col].skill == -1) {               // 006CDDC3..E0
            do { self->col = static_cast<U8>(self->col - 1); }                     // 006CDDF0
            while (flow_at(*self, self->row)->cells[self->col].skill == -1);       // 006CDDF3..0D
        }
        break;
    default:                                                                       // 006CDED0 / 006CDED1
        break;
    }
    if (old_row != self->row || old_col != self->col) {                            // 006CDED1..DF
        deselect(flow_at(*self, old_row), old_col);                                // 006CDEE1..F0F
        select_current(self);                                                      // 006CDF14..50
    }
    return flow_at(*self, self->row)->cells[self->col].skill;                      // 006CDF55..68
}
// 006CE570 CreateFeatChart(CSWCCreatureStats* stats, int bonus) (RET 8): one flow per root feat (no prerequisite)
// the creature has or may use and that is granted-and-owned / bonus / normal for its last class, with up to two
// successor feats (one with a single prerequisite = the root, one with two).
void __fastcall kotor_m15d_chart_create_feat_chart_006ce570(SkillFlowChart* self, void*, void* stats, Word bonus) noexcept(false) {
    ExoList sorted{0, 0, 0};                                                       // 006CE594..9C (EH state 0)
    try {
        const U8 class_id = kotor_m15b_creature_stats_get_class_00647730(
            stats, nullptr, static_cast<U8>(ld<U8>(stats, 0x32) - 1));            // 006CE5A0..AE
        void* const cls = at(rp(rules(), 0xAC), static_cast<unsigned>(class_id) * 0x1A0u);   // 006CE5B3..C1
        kotor_m15d_chart_clear_006cd240(self, nullptr);                            // 006CE5C7..CD
        kotor_m15d_chart_build_sorted_feat_list_006cd960(self, nullptr, &sorted);  // 006CE5D2..D9
        bool more = true;                                                          // [esp+24h] = 1 (006CE5E7)
        for (I32 i = 0; more && i < sorted.count; ++i) {                           // 006CE5F1..FA / 006CE77D..8B
            const U16 root = u16_at(sorted, i);                                    // 006CE606..08 ([ebp])
            const void* const feat = feat_of(root);                                // 006CE60C..0D
            if (feat == nullptr) continue;                                         // 006CE612..14
            if (ld<U16>(feat, 0x38) != 0xFFFFu) { more = false; continue; }        // 006CE61A..2A (sorted: roots first)
            if (!kotor_m15b_creature_stats_has_feat_00649340(stats, nullptr, root) &&
                !kotor_m15b_class_is_feat_useable_005be370(cls, nullptr, root))   // 006CE62F..53
                continue;
            U8 granted_level;                                                      // [esp+13h]
            bool add;
            if (kotor_m15b_class_is_granted_feat_005be310(cls, nullptr, root, &granted_level) &&
                kotor_m15b_creature_stats_has_feat_00649340(stats, nullptr, root)) // 006CE659..82
                add = true;
            else if (bonus != 0)                                                   // 006CE684..8A
                add = kotor_m15b_class_is_bonus_feat_005be270(cls, nullptr, root) != 0;    // 006CE68C..9C
            else
                add = kotor_m15b_class_is_normal_feat_005be2c0(cls, nullptr, root) != 0;   // 006CE6A3..B3
            if (!add) continue;
            I32 single = -1, dual = -1;                                            // 006CE6C1..CA ([esp+1Ch] / [esp+18h])
            for (I32 j = i + 1; j < sorted.count; ++j) {                           // 006CE6B9..5B (esi = i+1)
                if (single != -1 && dual != -1) break;                             // 006CE6E0..EA
                const U16 next = u16_at(sorted, j);                                // 006CE6F4
                const void* const succ = feat_of(next);                            // 006CE6F8..F9
                if (succ == nullptr || ld<U16>(succ, 0x38) != root) continue;      // 006CE700..0C
                if (!kotor_m15b_creature_stats_has_feat_00649340(stats, nullptr, next) &&
                    !kotor_m15b_class_is_feat_useable_005be370(cls, nullptr, next))   // 006CE70E..34
                    continue;
                if (ld<U16>(succ, 0x3A) == 0xFFFFu) single = next;                 // 006CE736..46
                else dual = next;                                                  // 006CE748..4C
            }
            kotor_m15d_chart_add_feat_set_006cda80(self, nullptr, root, single, dual);   // 006CE761..74
        }
    } catch (...) {
        live_free(ptr_of(sorted.data));                                            // EH state 0 (006FA390)
        throw;
    }
    live_free(ptr_of(sorted.data));                                                // 006CE791..9A
}

// ======================================================================== CSWGuiSkillInfoBox::SetSkillList
// 006CDFC0 SetSkillList(CExoArrayList<int>* list, BYTE type) (RET 8): entry k = list[k] (SetSkill), all entries
// added to LB_SKILLS (AddControls 0041C1D0), LB_SKILLS made active (vslot +08), temporary list freed.
void __fastcall kotor_m15d_skill_info_set_skill_list_006cdfc0(void* box, void*, void* list_ptr, Word type) noexcept(false) {
    SkillInfoBoxView* const self = static_cast<SkillInfoBoxView*>(box);
    const ExoList* const list = static_cast<const ExoList*>(list_ptr);
    ExoList controls{0, 0, 0};                                                     // 006CDFE1..E9 (EH state 0)
    try {
        for (I32 i = 0; i < list->count; ++i) {                                    // 006CDFF1..91
            unsigned char* const entry = at(self, 0x648u + 0x310u * static_cast<unsigned>(i));   // 006CDFFF / 006CE085 (no bound)
            kotor_m15d_skill_entry_set_skill_006acb40(entry, nullptr, u32_at(*list, i), type);   // 006CE005..13
            if (controls.count == controls.capacity) grow_list(controls, 4);       // 006CE018..6E
            set_u32_at(controls, controls.count, word_of(entry));                  // 006CE071..79
            ++controls.count;                                                      // 006CE07C..8D
        }
        kotor_fastcall_0041c1d0(&self->list, nullptr, &controls, 0, 0, 0);         // 006CE09E..AB
        vt<PanelActiveView>(static_cast<const Object*>(self))->set_active_control(self, nullptr, &self->list, 0);   // 006CE0B0..B9
    } catch (...) {
        live_free(ptr_of(controls.data));                                          // EH state 0 (006FA390)
        throw;
    }
    live_free(ptr_of(controls.data));                                              // 006CE0BC..C9
}

// ======================================================================== CSWGuiInGameSkillEntry::SetSkill
// 006ACB40 SetSkill(id, BYTE type) (RET 8): icon ResRef + name strref from the skill (0), feat (1) or spell (2);
// icon border params (+2B0) flags = (flags & ~2) | 1, SetFillImage(icon, 0), text SetStrRef(name).
void __fastcall kotor_m15d_skill_entry_set_skill_006acb40(void* entry, void*, Word id, Word type) noexcept(false) {
    Ref icon;
    icon.construct_00406d80("inone");                                             // 006ACB48..51 (00755EC8)
    Word strref = 0;                                                               // 006ACB5B (xor esi,esi)
    switch (static_cast<U8>(type)) {                                               // 006ACB56..65
    case 0: {                                                                      // 006ACBCB skill
        const Word skill_word = rd32(rules(), 0xB4) + (static_cast<Word>(static_cast<U16>(id)) << 5);   // 006ACBCB..DF
        void* const skill = ptr_of(skill_word);
        if (skill_word != 0) {                                                     // 006ACBE1 (je on the sum)
            Ref discarded;
            async_ref_assign_string_004061f0(&icon, nullptr, &discarded, at(skill, 0x10));   // 006ACBE3..F0
            strref = rd32(skill, 0x00);                                            // 006ACBF5
        }
        break;
    }
    case 1: {                                                                      // 006ACB9E feat
        const void* const feat = feat_of(static_cast<U16>(id));                    // 006ACB9E..A9
        if (feat != nullptr) {                                                     // 006ACBB0..B2
            Ref discarded;
            c2da_copy_00406120(&icon, nullptr, &discarded,
                               reinterpret_cast<Ref*>(const_cast<unsigned char*>(at(feat, 0x18))));   // 006ACBB4..C1
            strref = rd32(feat, 0x08);                                             // 006ACBC6
        }
        break;
    }
    case 2: {                                                                      // 006ACB6B spell
        void* const spell = kotor_m15b_spell_array_get_spell_0059b6d0(rp(rules(), 0x8C), nullptr,
                                                                       static_cast<std::int32_t>(id));   // 006ACB6B..7C
        if (spell != nullptr) {                                                    // 006ACB83..85
            Ref discarded;
            c2da_copy_00406120(&icon, nullptr, &discarded, reinterpret_cast<Ref*>(at(spell, 0x10)));   // 006ACB87..94
            strref = rd32(spell, 0x08);                                            // 006ACB99
        }
        break;
    }
    default:
        break;
    }
    unsigned char* const params = at(entry, 0x2B0);                                // 006ACBFD (+29C border, +14 params)
    st<Word>(params, 0x1C, (rd32(entry, 0x2CC) & 0xFFFFFFFDu) | 1u);               // 006ACBF7..10
    gui_load_border_params_set_fill_00414c00(params, nullptr, &icon, 0);           // 006ACC06..13
    gui_load_text_params_set_strref_00415e50(at(entry, kLabelText), nullptr, strref);   // 006ACC18..1F (+E8)
}
}
