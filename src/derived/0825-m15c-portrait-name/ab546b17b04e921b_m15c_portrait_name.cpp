// lane-m15-c (L15-C, Milestone 1.5): CSWGuiPortraitCharGen and CSWGuiNameChargen.
//   CSWGuiPortraitCharGen: ctor 006F9430, dtors 006F9A50 / 006F9A30, OnPanelAdded 006F89D0, SetParentPanel 006F8A00,
//     OnSelectDisplayButton 006F8A20, UpdatePortraitButton 006F8AD0, Draw 006F8C60, HandleCancelButton 006F8F40,
//     HandleRightButton 006F8FC0, HandleInputEvent 006F8FF0, GetPortraitsList 006F90F0; table 00759EA8
//   CSWGuiNameChargen: ctor 006F9E70, dtors 006FA2E0 / 006FA2C0, SetParentPanel 006F9B80, HandleCancelButton 006F9BA0,
//     HandleRandomName 006F9BF0, HandleDoneButton 006F9CD0 (callback), OnPanelAdded 006FA140, HandleInputEvent 006FA220;
//     table 00759F38
// Original: swkotor.exe GOG K1 (image base 0x400000), static disassembly only; each behaviour cites its instruction
// addresses. Tables read from .rdata: identical to CSWGuiPanel 0073E010 except
//   00759EA8: +00 006F9A30, +34 006F8C60, +3C 006F8FF0, +48 006F89D0
//   00759F38: +00 006FA2C0, +3C 006FA220, +48 006FA140
// D30-FINAL: x87 wide intermediates are doubles; float only at the original FSTP/FST dword stores (006F89F5, 006F8CA2,
// 006F8DB2, 006F8EFA, 006F8C3C); compares are against the value the original compares (stored float or wide ST0).
#include "88acda4ea129c9a0_m15c_common.h"
#include "b3ed8cdd89724107_rules_common.h"     // lane-rules: new_c2da / delete_c2da / int_entry / string_entry
#include "aae5b036321f76e1_growth.h"           // menu_list_growth::allocate_005e03c0 (CExoArrayList::Allocate)

using namespace kotor_m15c;
using gui_load::Control;

extern "C" {
std::int32_t __cdecl crt_rand_006fc447();                                        // lane-3d-model (original CRT rand)
}

namespace {
Portrait* portrait_of(Panel* p) { return static_cast<Portrait*>(p); }
NameChargen* name_of(NameHandlers* h) { return reinterpret_cast<NameChargen*>(h); }
kih::GuiObject* target(Panel* p) { return reinterpret_cast<kih::GuiObject*>(p); }
void add_event(Control* c, I32 id, Panel* p, gui_load::EventFunc f) { gui_load_control_add_event_0041ab20(c, nullptr, id, target(p), f); }
void label_ctor(Label* p) { (void)gui_load_label_construct_0041acd0(p, nullptr); }
void label_dtor(Label* p) { gui_load_label_destruct_00419d10(p, nullptr); }
void button_ctor(Button* p) { (void)gui_load_button_construct_0041c0f0(p, nullptr); }
void button_dtor(Button* p) { gui_load_button_destruct_00419da0(p, nullptr); }
using SetActive40Fn = void (__fastcall*)(Object*, void*, Word);   // CSWGuiControl slot +40 SetActive(int)
float bits_float(Word bits) { float f; std::memcpy(&f, &bits, 4); return f; }
// .rdata 0073DFEC = 0.01f (3C23D70A), 0073D764 = 1.0f, 0073D700 = 0.0f.
float random_idle() {                                                             // 006F89D4..F5 / 006F8ED0..EFA
    const std::int32_t r = crt_rand_006fc447() % 300;                             // IDIV 12Ch
    return static_cast<float>(static_cast<double>(r) * static_cast<double>(bits_float(0x3C23D70Au)) + 1.0);
}
// CSWGui3DSceneView construction (CSWGuiControl 0041AA80, table 00752E30, CSWGuiScene 004174B0; unwind ~Control).
void scene_view_ctor(SceneView* v) {
    (void)gui_load_control_construct_0041aa80(v, nullptr);
    try {
        v->vtbl = reinterpret_cast<const gui_load::ObjectVtbl*>(&gui_load_scene_view_methods_00752e30);
        (void)gui_scene_construct_004174b0(&v->scene, nullptr);
    } catch (...) { gui_load_control_destruct_00419a20(v, nullptr); throw; }
}
void play_both(void* creature, const char* anim, Word a) {                       // parts FFh and FEh, null-checked
    void* const body = creature_model(creature, 0xFF);
    if (body) slot<GobPlay>(body, 0x18)(body, nullptr, anim, 1.0f, a, 0);
    void* const head = creature_model(creature, 0xFE);
    if (head) slot<GobPlay>(head, 0x18)(head, nullptr, anim, 1.0f, a, 0);
}
}

extern "C" {
// ======================================================================= method tables
#define KOTOR_M15C_PANEL_TABLE(name, s00, s34, s3c, s48)                                                        \
    const KotorM15cPanelMethods name = {                                                                         \
        s00, &gui_load_panel_set_extent_0040a5a0, &gui_load_panel_set_active_control_0040a630,                   \
        &gui_load_slot_ret4_0060e760, &gui_load_slot_zero8_00409eb0,                                             \
        {&gui_load_slot_ret_004015a0, &gui_load_slot_ret_004015a0, &gui_load_slot_ret_004015a0,                 \
         &gui_load_slot_ret_004015a0, &gui_load_slot_ret_004015a0},                                              \
        &gui_load_slot_self_00641db0, &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0, s34,           \
        &gui_load_slot_ret4_0060e760, s3c, &gui_load_panel_hit_check_0040b690, &gui_load_panel_active_control_0040b630, \
        s48, &gui_load_panel_on_removed_0040c170,                                                                \
        {&gui_load_panel_button_a_0040b640, &gui_load_panel_button_b_0040b650, &gui_load_panel_button_x_0040b660, \
         &gui_load_panel_button_y_0040b670, &gui_load_panel_button_black_0040b680},                              \
        &gui_load_panel_full_screen_bg_0040a900, &gui_load_panel_reset_font_0040a9c0}
void* __fastcall kotor_m15c_portrait_deleting_destruct_006f9a30(Panel* self, void*, Word flags);
void __fastcall kotor_m15c_portrait_draw_006f8c60(Panel* self, void*, Word delta_bits) noexcept(false);
void __fastcall kotor_m15c_portrait_handle_input_006f8ff0(Object* self, void*, I32 event, I32 pressed) noexcept(false);
void __fastcall kotor_m15c_portrait_on_panel_added_006f89d0(Panel* self, void*);
void* __fastcall kotor_m15c_name_deleting_destruct_006fa2c0(Panel* self, void*, Word flags);
void __fastcall kotor_m15c_name_handle_input_006fa220(Object* self, void*, I32 event, I32 pressed) noexcept(false);
void __fastcall kotor_m15c_name_on_panel_added_006fa140(Panel* self, void*) noexcept(false);
KOTOR_M15C_PANEL_TABLE(kotor_m15c_portrait_methods_00759ea8, &kotor_m15c_portrait_deleting_destruct_006f9a30,
                       &kotor_m15c_portrait_draw_006f8c60, &kotor_m15c_portrait_handle_input_006f8ff0,
                       &kotor_m15c_portrait_on_panel_added_006f89d0);
KOTOR_M15C_PANEL_TABLE(kotor_m15c_name_methods_00759f38, &kotor_m15c_name_deleting_destruct_006fa2c0,
                       &kotor_panel_draw_0040B760, &kotor_m15c_name_handle_input_006fa220, &kotor_m15c_name_on_panel_added_006fa140);

// ======================================================================= CSWGuiPortraitCharGen
// 006F90F0 GetPortraitsList(int sex) (RET 4): portraits.2da rows with Sex == sex and ForPC == 1 -> rows (+120C) and the
// class-dependent appearance column -> appearances (+1218); current (+1234) = position of the creature's portrait row.
void __fastcall kotor_m15c_portrait_get_list_006f90f0(Portrait* self, void*, I32 sex) noexcept(false) {
    kotor_rules::Table* const table = kotor_rules::new_c2da("PORTRAITS");        // 006F9113..4C (0074DEAC)
    (void)c2da_current_load_004143b0(table, nullptr);                             // 006F914E..58
    const I32 rows = kotor_rules::row_count(table);                               // 006F915D..64
    String base;                                                                  // 006F9160..68 [esp+58h]
    gui_load::Ref unused;                                                         // 006F916D..79 [esp+60h]
    (void)c2da_default_ref_00405ed0(&unused, nullptr);
    self->appearances.size = 0;                                                   // 006F9186
    self->rows.size = 0;                                                          // 006F918C
    // D2 (006F9224): ForPC is read from a stack word that GetINTEntry 00413660 does not write when the column or row
    // is missing; the original then compares a stale value. Value-initialised to 0 here (missing column -> skipped).
    I32 for_pc = 0;
    for (I32 row = 0; row < rows; ++row) {                                        // 006F91A0..F7
        I32 row_sex = -1;                                                         // 006F91A0..B0
        I32 appearance = -1;
        (void)kotor_rules::int_entry(table, row, "Sex", &row_sex);                // 006F91A3..DC (0075810C)
        if (row_sex != sex) continue;                                             // 006F91E1..E8
        (void)kotor_rules::int_entry(table, row, "ForPC", &for_pc);               // 006F91EE..21F (00758110)
        if (for_pc != 1) continue;                                                // 006F9224..29
        const std::uint8_t class_id = kotor_m15b_creature_stats_get_class_00647730(stats_of(self->creature), nullptr, 0);   // 006F922F..3A
        const char* column = "AppearanceNumber";                                  // default / class 1 (007580D0)
        if (class_id == 0) column = "Appearance_L";                               // 006F92DE (007580B0)
        else if (class_id == 2) column = "Appearance_S";                          // 006F9282 (007580C0)
        (void)kotor_rules::int_entry(table, row, column, &appearance);            // 006F9251..30F
        (void)kotor_rules::string_entry(table, row, "BaseResRef", &base);         // 006F9314..45 (00745E40)
        if (ld<std::uint16_t>(stats_of(self->creature), 0x11C) == static_cast<std::uint32_t>(row))   // 006F934A..5C
            self->current = self->rows.size;                                      // 006F935E..64
        if (self->rows.size == self->rows.capacity)                               // 006F936A..7E
            menu_list_growth::allocate_005e03c0(&self->rows, nullptr,
                                                self->rows.capacity ? static_cast<Word>(self->rows.capacity) * 2u : 0x10u);   // 006F9380..90
        reinterpret_cast<I32*>(static_cast<std::uintptr_t>(self->rows.data))[self->rows.size] = row;   // 006F9395..9A
        self->rows.size = self->rows.size + 1;                                    // 006F939D..AB
        if (self->appearances.size == self->appearances.capacity)                 // 006F93AE..B1
            menu_list_growth::allocate_005e03c0(&self->appearances, nullptr,
                                                self->appearances.capacity ? static_cast<Word>(self->appearances.capacity) * 2u : 0x10u);   // 006F93BA..C8
        reinterpret_cast<I32*>(static_cast<std::uintptr_t>(self->appearances.data))[self->appearances.size] = appearance;   // 006F93CD..E2
        self->appearances.size = self->appearances.size + 1;                      // 006F93E5..ED
    }
    kotor_rules::delete_c2da(table);                                              // 006F93FD..403 [vtbl+0](1) (no Unload2DArray)
}
// 006F8AD0 UpdatePortraitButton (RET): apply the selected row (portrait id, head appearance), refresh LBL_PORTRAIT and
// re-attach the head to the panel scene.
void __fastcall kotor_m15c_portrait_update_button_006f8ad0(Portrait* self, void*) noexcept(false) {
    const I32 row = reinterpret_cast<const I32*>(static_cast<std::uintptr_t>(self->rows.data))[self->current];   // 006F8AD6..F6
    void* const creature = self->creature;
    if (ld<std::uint16_t>(stats_of(creature), 0x11C) != static_cast<std::uint32_t>(row)) {   // 006F8AE5..FA
        slot<CreatureSetPortraitId>(creature, 0xF4)(creature, nullptr, static_cast<std::uint16_t>(row));   // 006F8B00..07 [+F4]
        unsigned char appearance[0x3C];                                           // [esp+1Ch]
        std::memcpy(appearance, ld<void*>(self->creature, 0x21C), sizeof(appearance));   // 006F8B10..25 REP MOVSD 0Fh
        const std::uint16_t head = static_cast<std::uint16_t>(
            reinterpret_cast<const I32*>(static_cast<std::uintptr_t>(self->appearances.data))[self->current]);   // 006F8B27..2D
        st<std::uint16_t>(appearance, 0x18, head);                                // 006F8B39
        (void)kotor_m15a_creature_set_appearance_006134c0(self->creature, nullptr, appearance, 2, 1);   // 006F8B31..41
        st<std::uint16_t>(ld<void*>(self->creature, 0x21C), 0x18, static_cast<std::uint16_t>(
            reinterpret_cast<const I32*>(static_cast<std::uintptr_t>(self->appearances.data))[self->current]));   // 006F8B46..5F
        if (kotor_cell_do3dgui_0078d1e4) play_both(self->creature, "pause1", 2);  // 006F8B63..C4 (0074F3F8)
    }
    {
        ResRef16 portrait;
        ResRef16* const ref = slot<CreatureGetPortrait>(self->creature, 0x148)(self->creature, nullptr, &portrait, 0);   // 006F8BC7..D3
        gui_load_border_params_set_fill_00414c00(self->portrait.border.params, nullptr,
                                                 reinterpret_cast<const gui_load::ResRef*>(ref), 0);   // 006F8BD9..E2 (+35C)
    }
    if (kotor_cell_do3dgui_0078d1e4) {                                            // 006F8BE7..EE
        slot<CreatureAttach>(self->creature, 0x94)(self->creature, nullptr, self->head.scene.aur_scene);   // 006F8BF0..FC
        if (unsigned char* const behavior = camera_behavior(self->head.scene.aur_camera)) {   // 006F8C02..19
            st<Word>(behavior, 0x48, 0);                                          // 006F8C1B
            const float h = head_height(self->creature);                          // 006F8C22..3C
            store_offset(behavior, 0.0f, h, 0.0f);                                // 006F8C35..55
        }
    }
}
// 006F89D0 OnPanelAdded (RET): idle timer = rand() % 300 * 0.01 + 1.0 (no base call).
void __fastcall kotor_m15c_portrait_on_panel_added_006f89d0(Panel* panel, void*) {
    portrait_of(panel)->idle = random_idle();                                     // 006F89D4..F5
}
// 006F8A00 SetParentPanel(parent, kind) (RET 8).
void __fastcall kotor_m15c_portrait_set_parent_panel_006f8a00(Portrait* self, void*, Panel* parent, Word kind) {
    self->parent = parent;                                                        // 006F8A08
    self->parent_kind = static_cast<unsigned char>(kind);                         // 006F8A0B
}
// 006F8A20 OnSelectDisplayButton (RET): commit the selection and notify the parent.
void __fastcall kotor_m15c_portrait_on_select_display_006f8a20(Portrait* self, void*) noexcept(false) {
    gui_load::Ref empty;
    empty.construct_00406d80("");                                                // 006F8A27..30 (0073D71C)
    slot<CreatureSetPortrait>(self->creature, 0xEC)(self->creature, nullptr, reinterpret_cast<const ResRef16*>(&empty));   // 006F8A35..3F
    const I32* const rows = reinterpret_cast<const I32*>(static_cast<std::uintptr_t>(self->rows.data));
    slot<CreatureSetPortraitId>(self->creature, 0xF4)(self->creature, nullptr, static_cast<std::uint16_t>(rows[self->current]));   // 006F8A45..5B
    const I32* const apps = reinterpret_cast<const I32*>(static_cast<std::uintptr_t>(self->appearances.data));
    st<std::uint16_t>(stats_of(self->creature), 0x8C, static_cast<std::uint16_t>(apps[self->current]));   // 006F8A61..7A
    (void)kotor_fastcall_0040be00(self->manager, nullptr);                        // 006F8A81..84 PopModalPanel
    mark_done(self);                                                              // 006F8A89..A0
    if (self->parent_kind == 1) kotor_m15c_quick_portrait_creation_completed_006eff60(static_cast<Quick*>(self->parent), nullptr);   // 006F8AA5..A8
    else if (self->parent_kind == 2) kotor_m15c_custom_portrait_creation_completed_006ef4c0(self->parent, nullptr);              // 006F8AB7..BA
}
// 006F8F40 HandleCancelButton (RET): restore the original portrait row and notify the parent.
void __fastcall kotor_m15c_portrait_handle_cancel_006f8f40(Portrait* self, void*) noexcept(false) {
    if (!self->parent) return;                                                    // 006F8F43..48
    const I32* const rows = reinterpret_cast<const I32*>(static_cast<std::uintptr_t>(self->rows.data));
    I32 index = -1;                                                               // 006F8F6F
    for (I32 i = 0; i < self->rows.size; ++i)                                     // 006F8F4A..6D
        if (rows[i] == self->original) { index = i; break; }
    self->current = index;                                                        // 006F8F74
    kotor_m15c_portrait_update_button_006f8ad0(self, nullptr);                    // 006F8F7A
    (void)kotor_fastcall_0040be00(self->manager, nullptr);                        // 006F8F7F..82
    mark_done(self);                                                              // 006F8F87..94
    if (self->parent_kind == 1) kotor_m15c_quick_cancelled_by_child_006eff10(static_cast<Quick*>(self->parent), nullptr);   // 006F8FA2..A6
    else if (self->parent_kind == 2) kotor_m15c_custom_cancelled_by_child_006ef480(self->parent, nullptr);               // 006F8FAF..B3
}
// 006F8FC0 HandleRightButton (RET): next row (wraps to 0), JMP UpdatePortraitButton.
void __fastcall kotor_m15c_portrait_handle_right_006f8fc0(Portrait* self, void*) noexcept(false) {
    if (++self->current >= self->rows.size) self->current = 0;                    // 006F8FC0..D7
    kotor_m15c_portrait_update_button_006f8ad0(self, nullptr);                    // 006F8FE1
}
// 006F8FF0 HandleInputEvent(event, pressed) (RET 8). Jump table 006F90B4 / index bytes 006F90C8 (event - 27h, 0..19h):
// 27h,2Dh accept; 28h,2Eh cancel; 2Fh,35h,3Fh left; 30h,36h,40h right; others default.
void __fastcall kotor_m15c_portrait_handle_input_006f8ff0(Object* object, void*, I32 event, I32 pressed) noexcept(false) {
    Portrait* const self = portrait_of(static_cast<Panel*>(object));
    if (pressed != 0) {
        switch (event) {
        case 0x27: case 0x2D:                                                     // 006F901F
            manager_of(self)->PlayGuiSound(0);
            kotor_m15c_portrait_on_select_display_006f8a20(self, nullptr);
            break;
        case 0x28: case 0x2E:                                                     // 006F903F
            manager_of(self)->PlayGuiSound(0);
            kotor_m15c_portrait_handle_cancel_006f8f40(self, nullptr);
            break;
        case 0x2F: case 0x35: case 0x3F:                                          // 006F905F: previous row (wraps)
            manager_of(self)->PlayGuiSound(1);
            if (--self->current < 0) self->current = self->rows.size - 1;         // 006F9069..78 (JNS)
            kotor_m15c_portrait_update_button_006f8ad0(self, nullptr);            // 006F907E..80
            break;
        case 0x30: case 0x36: case 0x40:                                          // 006F9094
            manager_of(self)->PlayGuiSound(1);
            kotor_m15c_portrait_handle_right_006f8fc0(self, nullptr);
            break;
        default: break;
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, pressed);          // 00409E60
}
// 006F8C60 Draw(float delta) (RET 4): expire queued control timers (+1224), keep the head attached, idle animations.
void __fastcall kotor_m15c_portrait_draw_006f8c60(Panel* panel, void*, Word delta_bits) noexcept(false) {
    Portrait* const self = portrait_of(panel);
    const float delta = bits_float(delta_bits);                                   // [esp+30h]
    for (I32 i = self->anims.size - 1; i >= 0; --i) {                             // 006F8C7B..D01
        PortraitAnim* const list = reinterpret_cast<PortraitAnim*>(static_cast<std::uintptr_t>(self->anims.data));
        list[i].time = static_cast<float>(static_cast<double>(list[i].time) - static_cast<double>(delta));   // 006F8C90..A2
        if (!(list[i].time <= 0.0f)) continue;                                    // 006F8CAA..BC (FCOMP 0073D700, TEST AH,41h / JP)
        Object* const control = list[i].control;
        slot<SetActive40Fn>(control, 0x40)(control, nullptr, 0);                  // 006F8CBE..C4 [+40](0)
        self->anims.size = self->anims.size - 1;                                  // 006F8CC7..D4
        for (I32 k = i; k < self->anims.size; ++k) {                              // 006F8CDA..FE
            PortraitAnim* const l = reinterpret_cast<PortraitAnim*>(static_cast<std::uintptr_t>(self->anims.data));
            l[k] = l[k + 1];
        }
    }
    if (self->creature && kotor_cell_do3dgui_0078d1e4) {                          // 006F8D03..15
        void* const body = creature_model(self->creature, 0xFF);                  // 006F8D1B..29
        if (slot<GobScene>(body, 0x104)(body, nullptr) != self->head.scene.aur_scene) {   // 006F8D2C..3E
            slot<CreatureAttach>(self->creature, 0x94)(self->creature, nullptr, self->head.scene.aur_scene);   // 006F8D40..4C
            if (unsigned char* const behavior = camera_behavior(self->head.scene.aur_camera)) {   // 006F8D52..69
                st<Word>(behavior, 0x48, 0);                                      // 006F8D6B
                const float h = head_height(self->creature);                      // 006F8D72..7D
                store_offset(behavior, 0.0f, h, 0.0f);                            // 006F8D81..A5
            }
        }
        const double wide = static_cast<double>(self->idle) - static_cast<double>(delta);   // 006F8DA8..AE (ST0)
        self->idle = static_cast<float>(wide);                                    // 006F8DB2 FST
        if (wide <= 0.0) {                                                        // 006F8DB8..C3 (compare of ST0)
            String anim;                                                          // 006F8DC9..CD
            String pause1("pause1");                                              // 006F8DD2..E3 (0074F3F8)
            void* const part_body = creature_model(self->creature, 0xFF);         // 006F8DE8..FE
            const std::int32_t pick = crt_rand_006fc447() % 2;                    // 006F8E03..17 (signed)
            if (pick == 0) (void)GuiAssign005e5140(&anim, nullptr, "pause2");     // 006F8E23 (0074F400)
            else if (pick == 1) (void)GuiAssign005e5140(&anim, nullptr, "listen");   // 006F8E1C (00759E34)
            if (part_body) {                                                      // 006F8E31..34
                slot<GobAddFlag>(part_body, 0x128)(part_body, nullptr, 1);        // 006F8E36..3C
                slot<GobPlay>(part_body, 0x18)(part_body, nullptr, WindowCStr005e5670(&anim, nullptr), 1.0f, 0x21, 0);    // 006F8E42..59
                slot<GobPlay>(part_body, 0x18)(part_body, nullptr, WindowCStr005e5670(&pause1, nullptr), 1.0f, 0x60, 0);  // 006F8E5C..73
            }
            void* const part_head = creature_model(self->creature, 0xFE);         // 006F8E76..8A
            if (part_head) {                                                      // 006F8E8C..8E
                slot<GobAddFlag>(part_head, 0x128)(part_head, nullptr, 1);        // 006F8E90..96
                slot<GobPlay>(part_head, 0x18)(part_head, nullptr, WindowCStr005e5670(&anim, nullptr), 1.0f, 0x21, 0);    // 006F8E9C..B3
                slot<GobPlay>(part_head, 0x18)(part_head, nullptr, WindowCStr005e5670(&pause1, nullptr), 1.0f, 0x60, 0);  // 006F8EB6..CD
            }
            self->idle = random_idle();                                           // 006F8ED0..FA
        }
    }
    kotor_panel_draw_0040B760(self, nullptr, delta_bits);                         // 006F8F17..1E
}
// 006F9430 ctor(CSWGuiManager*, CSWCCreature*) (RET 8). EH states 0..10 = members (FuncInfo 0073A4CA).
Portrait* __fastcall kotor_m15c_portrait_construct_006f9430(Portrait* self, void*, Object* manager, void* creature) noexcept(false) {
    (void)gui_load_panel_construct_0040b570(self, nullptr, manager);             // 006F945B
    int built = 0;
    try {
        self->vtbl = reinterpret_cast<const gui_load::ObjectVtbl*>(&kotor_m15c_portrait_methods_00759ea8);   // 006F9469
        label_ctor(&self->title); built = 1;                                      // 006F946F
        label_ctor(&self->subtitle); built = 2;                                   // 006F947F
        label_ctor(&self->portrait); built = 3;                                   // 006F948F
        for (int i = 0; i < 5; ++i) {                                             // 006F94B1 eh_vector_constructor_iterator (140h x5)
            try { label_ctor(&self->bevels[i]); }
            catch (...) { while (i-- > 0) label_dtor(&self->bevels[i]); throw; }
        }
        built = 4;
        scene_view_ctor(&self->head); built = 5;                                  // 006F94B6..DA (0041AA80, 00752E30, 004174B0)
        button_ctor(&self->accept); built = 6;                                    // 006F94EA
        button_ctor(&self->back); built = 7;                                      // 006F94FA
        button_ctor(&self->right); built = 8;                                     // 006F950A
        button_ctor(&self->left); built = 9;                                      // 006F951A
        self->rows = Array{};                                                     // 006F951F..2B
        self->appearances = Array{};                                              // 006F9531..3D
        self->anims = Array{};                                                    // 006F9543..4F
        self->creature = creature;                                                // 006F9555..58
        {
            gui_load::Ref layout;
            layout.construct_00406d80("PORTCUST");                               // 006F955D (00759E98)
            gui_load_panel_start_layout_0040a680(self, nullptr, &layout);         // 006F9576
        }
        init_control(self, &self->title, "MAIN_TITLE_LBL");                       // 006F957B
        init_control(self, &self->subtitle, "SUB_TITLE_LBL");                     // 006F95AD
        init_control(self, &self->portrait, "LBL_PORTRAIT");                      // 006F95E2 (00756044)
        init_control(self, &self->head, "LBL_HEAD");                              // 006F9617 (00759E8C)
        init_control(self, &self->bevels[0], "LBL_BEVEL_L");                      // 006F964C
        init_control(self, &self->bevels[1], "LBL_BEVEL_M");                      // 006F9681
        init_control(self, &self->bevels[2], "LBL_BEVEL_R");                      // 006F96B6
        init_control(self, &self->bevels[3], "LBL_BEVEL_T");                      // 006F96EB (00759E80)
        init_control(self, &self->bevels[4], "LBL_BEVEL_B");                      // 006F9720 (00759E74)
        init_control(self, &self->back, "BTN_BACK");                              // 006F9755
        init_control(self, &self->accept, "BTN_ACCEPT");                          // 006F978A (007548B8)
        init_control(self, &self->left, "BTN_ARRL");                              // 006F97BF (00759E68)
        init_control(self, &self->right, "BTN_ARRR");                             // 006F97F4 (00759E5C)
        gui_load_panel_stop_layout_0040b8f0(self, nullptr);                       // 006F982B
        self->original = ld<std::uint16_t>(stats_of(self->creature), 0x11C);      // 006F9830..40
        self->current = 0;                                                        // 006F9846
        kotor_m15c_portrait_get_list_006f90f0(self, nullptr, ld<std::uint8_t>(stats_of(self->creature), 0x31));   // 006F984C..59
        void* const aur_scene = self->head.scene.aur_scene;                       // 006F9864 ([+ADC])
        void* const camera = self->head.scene.aur_camera;                         // 006F986A ([+AE0])
        if (kotor_cell_do3dgui_0078d1e4) {                                        // 006F985E..78
            String hook;                                                          // 006F987E..82
            const char suffix = ld<std::uint8_t>(stats_of(self->creature), 0x31) != 0 ? 'f' : 'm';   // 006F9887..9D
            kotor_reconstruction::StartupFormat005e5680(&hook, "camerahook%c", static_cast<int>(suffix));   // 006F98A0..B3 (00759E4C)
            const float position[3] = {0.0f, 0.0f, 0.0f};                         // 006F98E4..F0
            const float orientation[4] = {0.0f, 0.0f, 0.0f, 1.0f};                // 006F98D0..E4
            slot<SceneSpawnRoom>(aur_scene, 0x70)(aur_scene, nullptr, "gui3D_room", position, orientation);   // 006F98B8..F0 (00753038)
            void* model;
            {
                String light("cghead_light");                                     // 006F98F3..FC (00759E3C)
                model = aurora_gui_scene_add_model_name_00417620(&self->head.scene, nullptr, &light, -1);   // 006F9901..1C
            }
            if (model) {                                                          // 006F992A..30
                slot<GobPlay>(model, 0x18)(model, nullptr, "default", 1.0f, 0, 0);   // 006F9932..40 (0073DF6C)
                reinterpret_cast<const gui_load::AurCameraVtbl*>(ld<const void*>(camera, 0))->attach_to_object(
                    camera, nullptr, model, WindowCStr005e5670(&hook, nullptr), 1);   // 006F9943..68 [+74]
                reinterpret_cast<const gui_load::AurCameraVtbl*>(ld<const void*>(camera, 0))->set_view_angle(
                    camera, nullptr, bits_float(0x41B5CED9u));                    // 006F996B..74 [+44]
            }
        }
        self->back.flags &= 0xFFFFFFFBu;                                          // 006F9986..A1
        add_event(&self->back, 0x27, self, kotor_m1_shared::event_func(&kotor_m1_shared::PanelEventThunks::OnBButtonPressed_00624bb0));   // 006F99A4
        self->accept.flags &= 0xFFFFFFFBu;                                        // 006F99A9..BF
        add_event(&self->accept, 0x27, self, kotor_m1_shared::event_func(&kotor_m1_shared::PanelEventThunks::AcceptButtonCallback_00624ba0));   // 006F99C2
        self->right.flags &= 0xFFFFFFFBu;                                         // 006F99C7..DD
        add_event(&self->right, 0x27, self, kotor_m15d::event_func(&kotor_m15d::PanelStepThunks::OnPlusButtonPushed_0067cb50));   // 006F99E0
        self->left.flags &= 0xFFFFFFFBu;                                          // 006F99E5..FB
        add_event(&self->left, 0x27, self, kotor_m15d::event_func(&kotor_m15d::PanelStepThunks::OnMinusButtonPushed_0067cb40));   // 006F99FE
        kotor_m15c_portrait_update_button_006f8ad0(self, nullptr);                // 006F9A03..05
        self->idle = 0.0f;                                                        // 006F9A0E (edi = 0)
    } catch (...) {
        if (built >= 9) button_dtor(&self->left);
        if (built >= 8) button_dtor(&self->right);
        if (built >= 7) button_dtor(&self->back);
        if (built >= 6) button_dtor(&self->accept);
        if (built >= 5) gui_load_scene_view_destruct_0067ac50(&self->head, nullptr);
        if (built >= 4) for (int i = 5; i-- > 0;) label_dtor(&self->bevels[i]);
        if (built >= 3) label_dtor(&self->portrait);
        if (built >= 2) label_dtor(&self->subtitle);
        if (built >= 1) label_dtor(&self->title);
        gui_load_panel_destruct_0040cf70(self, nullptr);
        throw;
    }
    return self;                                                                  // 006F9A15
}
// 006F9A50 dtor: free the three list buffers, members in reverse order, ~CSWGuiPanel.
void __fastcall kotor_m15c_portrait_destruct_006f9a50(Portrait* self, void*) {
    live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->anims.data)));         // 006F9A6E..7D
    self->anims.data = 0;                                                         // 006F9A84
    live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->appearances.data)));   // 006F9A8A..96
    self->appearances.data = 0;                                                   // 006F9A9B
    live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->rows.data)));          // 006F9AA1..AD
    self->rows.data = 0;                                                          // 006F9ABB
    button_dtor(&self->left);                                                     // 006F9AB5..C6
    button_dtor(&self->right);                                                    // 006F9ACB..D6
    button_dtor(&self->back);                                                     // 006F9ADB..E6
    button_dtor(&self->accept);                                                   // 006F9AEB..F6
    gui_load_scene_view_destruct_0067ac50(&self->head, nullptr);                  // 006F9AFB..B06
    for (int i = 5; i-- > 0;) label_dtor(&self->bevels[i]);                       // 006F9B0B..23 (006FA852)
    label_dtor(&self->portrait);                                                  // 006F9B28..33
    label_dtor(&self->subtitle);                                                  // 006F9B38..43
    label_dtor(&self->title);                                                     // 006F9B48..4F
    gui_load_panel_destruct_0040cf70(self, nullptr);                              // 006F9B54..5E
}
// 006F9A30 scalar deleting destructor (RET 4).
void* __fastcall kotor_m15c_portrait_deleting_destruct_006f9a30(Panel* self, void*, Word flags) {
    kotor_m15c_portrait_destruct_006f9a50(portrait_of(self), nullptr);
    if (flags & 1u) live_free(self);                                              // 006F9A38..45
    return self;
}

// ======================================================================= CSWGuiNameChargen
// 006F9E70 ctor(CSWGuiManager*, CSWCCreature*) (RET 8). EH states 0..6 members, 7 CSWNameGen, 8 CExoString (FuncInfo 0073A66F).
NameChargen* __fastcall kotor_m15c_name_construct_006f9e70(NameChargen* self, void*, Object* manager, void* creature) noexcept(false) {
    (void)gui_load_panel_construct_0040b570(self, nullptr, manager);             // 006F9E9B
    int built = 0;
    try {
        self->vtbl = reinterpret_cast<const gui_load::ObjectVtbl*>(&kotor_m15c_name_methods_00759f38);   // 006F9EAD
        button_ctor(&self->end); built = 1;                                       // 006F9EB3
        (void)kotor_m15c_editbox_construct_0041c6b0(&self->edit, nullptr); built = 2;   // 006F9EC3
        label_ctor(&self->title); built = 3;                                      // 006F9ED3
        label_ctor(&self->subtitle); built = 4;                                   // 006F9EE3
        button_ctor(&self->back); built = 5;                                      // 006F9EF5
        button_ctor(&self->random); built = 6;                                    // 006F9F05
        (void)kotor_m15c_namegen_construct_00711290(&self->namegen, nullptr); built = 7;   // 006F9F15
        String unused;                                                            // 006F9F1A..23 [esp+20h] (state 8)
        self->creature = creature;                                                // 006F9F28..2B
        {
            gui_load::Ref layout;
            layout.construct_00406d80("NAME");                                   // 006F9F2E (00748E9C)
            gui_load_panel_start_layout_0040a680(self, nullptr, &layout);         // 006F9F48
        }
        init_control(self, &self->end, "END_BTN");                                // 006F9F4D..6A (00759F30)
        init_control(self, &self->edit, "NAME_BOX_EDIT");                         // 006F9F7D..A0 (00759F20)
        init_control(self, &self->title, "MAIN_TITLE_LBL");                       // 006F9FB3..D6 (00759090)
        init_control(self, &self->subtitle, "SUB_TITLE_LBL");                     // 006F9FE9..A00C (00759890)
        init_control(self, &self->random, "BTN_RANDOM");                          // 006FA01F..42 (00759F14)
        init_control(self, &self->back, "BTN_BACK");                              // 006FA055..72 (007548AC)
        gui_load_panel_stop_layout_0040b8f0(self, nullptr);                       // 006FA085..87
        add_event(&self->end, 0x27, self, handler(&NameHandlers::HandleDoneButton_006f9cd0));   // 006FA08C..96
        add_event(&self->end, 0x2D, self, handler(&NameHandlers::HandleDoneButton_006f9cd0));   // 006FA09B..A5
        self->random.flags &= 0xFFFFFFFBu;                                        // 006FA0AA..C1
        add_event(&self->random, 0x27, self,
                  kotor_m15_l0::event_func(&kotor_m15_l0::PanelEventThunks15::OnYButtonPressed_00644720));   // 006FA0B0..C4 (00644720)
        self->back.flags &= 0xFFFFFFFBu;                                          // 006FA0C9..D9
        add_event(&self->back, 0x27, self, kotor_m1_shared::event_func(&kotor_m1_shared::PanelEventThunks::OnBButtonPressed_00624bb0));   // 006FA0CC..DC
        self->end.flags &= 0xFFFFFFFBu;                                           // 006FA0E1
        // D2 (006FA0E5..FC): the original clears bit0 of the heap word +99C and keeps the other (indeterminate) bits;
        // no instruction ever reads them (only 006F9C99 OR 1 / 006FA0E5 AND ~1 touch +99C). Value-initialised to 0.
        self->flags = 0;
        self->edit.edit.max_length = 0x12;                                        // 006FA102 (+380)
        add_event(&self->edit, 0x27, self, handler(&NameHandlers::HandleDoneButton_006f9cd0));  // 006FA0EB..10B
    } catch (...) {
        if (built >= 7) kotor_m15c_namegen_destruct_007112e0(&self->namegen, nullptr);
        if (built >= 6) button_dtor(&self->random);
        if (built >= 5) button_dtor(&self->back);
        if (built >= 4) label_dtor(&self->subtitle);
        if (built >= 3) label_dtor(&self->title);
        if (built >= 2) kotor_m15c_editbox_destruct_0041c740(&self->edit, nullptr);
        if (built >= 1) button_dtor(&self->end);
        gui_load_panel_destruct_0040cf70(self, nullptr);
        throw;
    }
    return self;                                                                  // 006FA123
}
// 006FA2E0 dtor.
void __fastcall kotor_m15c_name_destruct_006fa2e0(NameChargen* self, void*) {
    kotor_m15c_namegen_destruct_007112e0(&self->namegen, nullptr);               // 006FA2FD..30B
    button_dtor(&self->random);                                                   // 006FA310..1B
    button_dtor(&self->back);                                                     // 006FA320..2B
    label_dtor(&self->subtitle);                                                  // 006FA330..3B
    label_dtor(&self->title);                                                     // 006FA340..4B
    kotor_m15c_editbox_destruct_0041c740(&self->edit, nullptr);                   // 006FA350..5B
    button_dtor(&self->end);                                                      // 006FA360..68
    gui_load_panel_destruct_0040cf70(self, nullptr);                              // 006FA36D..77
}
// 006FA2C0 scalar deleting destructor (RET 4).
void* __fastcall kotor_m15c_name_deleting_destruct_006fa2c0(Panel* self, void*, Word flags) {
    kotor_m15c_name_destruct_006fa2e0(static_cast<NameChargen*>(self), nullptr);
    if (flags & 1u) live_free(self);                                              // 006FA2C8..D5
    return self;
}
// 006F9B80 SetParentPanel(parent, kind) (RET 8).
void __fastcall kotor_m15c_name_set_parent_panel_006f9b80(NameChargen* self, void*, Panel* parent, Word kind) {
    self->parent = parent;                                                        // 006F9B88
    self->parent_kind = static_cast<unsigned char>(kind);                         // 006F9B8B
}
// 006F9BA0 HandleCancelButton (RET).
void __fastcall kotor_m15c_name_handle_cancel_006f9ba0(NameChargen* self, void*) noexcept(false) {
    if (!self->parent) return;                                                    // 006F9BA3..A8
    (void)kotor_fastcall_0040be00(self->manager, nullptr);                        // 006F9BAA..AD PopModalPanel
    kotor_m15_panel_set_visible_0040b9d0(self->parent, nullptr, 1);               // 006F9BB2..B7 parent SetVisible(1)
    mark_done(self);                                                              // 006F9BBC..C9
    if (self->parent_kind == 1) kotor_m15c_quick_cancelled_by_child_006eff10(static_cast<Quick*>(self->parent), nullptr);   // 006F9BD6..DA
    else if (self->parent_kind == 2) kotor_m15c_custom_cancelled_by_child_006ef480(self->parent, nullptr);               // 006F9BE3..E7
}
// 006F9BF0 HandleRandomName (RET): CSWNameGen::GetRandomName(race 6 (.rdata word 0074BF14), female 6 / male 5, 15).
void __fastcall kotor_m15c_name_handle_random_006f9bf0(NameChargen* self, void*) noexcept(false) {
    static const std::uint16_t kWord_0074bf14 = 6;                                // .rdata word 0074BF14
    String name;                                                                  // 006F9C0B..0F
    const bool female = ld<std::uint8_t>(stats_of(self->creature), 0x31) == 1;    // 006F9C14..20 (gender byte)
    {
        Fresh generated;
        generated.object = kotor_m15c_namegen_random_name_007118c0(&self->namegen, nullptr, generated.storage(),
                                                                   kWord_0074bf14, female ? 6u : 5u, 0x0Fu);   // 006F9C2A..6C
        name.Assign005e5c50(generated.object);                                    // 006F9C71..76
    }                                                                             // 006F9C7B..84
    kotor_m15c_edit_text_set_text_00416490(&self->edit.edit, nullptr, &name);     // 006F9C89..94 (+310)
    self->flags |= 1u;                                                            // 006F9C99..A6
}
// 006FA140 OnPanelAdded (RET): focus the edit box, current name or a random one.
void __fastcall kotor_m15c_name_on_panel_added_006fa140(Panel* panel, void*) noexcept(false) {
    NameChargen* const self = static_cast<NameChargen*>(panel);
    String name;                                                                  // 006FA15B..5F
    kotor_m15c_editbox_set_focus_00418660(&self->edit, nullptr);                  // 006FA164..72
    gui_load_panel_on_added_0040b870(self, nullptr);                              // 006FA177..79
    if (self->parent_kind == 1 || self->parent_kind == 2) {                       // 006FA17E..9E
        // Both kinds call CSWGuiQuickPanel::GetCurrentName 006EFFA0 on [+68] (it reads [parent+68]+22D4, valid for
        // the custom panel too: CSWGuiCustomPanel +68 is the CSWGuiMainCharGen as well).
        Fresh current;
        current.object = kotor_m15c_quick_get_current_name_006effa0(static_cast<Quick*>(self->parent), nullptr,
                                                                     current.storage());   // 006FA188..AD
        name.Assign005e5c50(current.object);                                      // 006FA1B2..B7
    }
    if (name.BorrowedLength005e5790() == 0) kotor_m15c_name_handle_random_006f9bf0(self, nullptr);   // 006FA1CA..D9
    else kotor_m15c_edit_text_set_text_00416490(&self->edit.edit, nullptr, &name);                   // 006FA1E0..EB
}
// 006FA220 HandleInputEvent(event, pressed) (RET 8). Jump table 006FA29C (event - 28h, 0..6):
// 28h,2Eh cancel; 2Ah random name; 2Dh sound; others default.
void __fastcall kotor_m15c_name_handle_input_006fa220(Object* object, void*, I32 event, I32 pressed) noexcept(false) {
    NameChargen* const self = static_cast<NameChargen*>(static_cast<Panel*>(object));
    if (pressed != 0) {
        switch (event) {
        case 0x28: case 0x2E:                                                     // 006FA259
            manager_of(self)->PlayGuiSound(0);
            kotor_m15c_name_handle_cancel_006f9ba0(self, nullptr);
            break;
        case 0x2A:                                                                // 006FA279
            manager_of(self)->PlayGuiSound(0);
            kotor_m15c_name_handle_random_006f9bf0(self, nullptr);
            break;
        case 0x2D:                                                                // 006FA240
            manager_of(self)->PlayGuiSound(0);
            break;
        default: break;
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, pressed);          // 00409E60
}
}

// 006F9CD0 HandleDoneButton(control) (thiscall callback, RET 4).
void __thiscall NameHandlers::HandleDoneButton_006f9cd0(kih::Control*) {
    NameChargen* const self = name_of(this);
    String name;                                                                  // 006F9CF3..F7
    name.Assign005e5c50(str(self->edit.edit.value));                              // 006F9CFC..D0D (+388)
    if (name.BorrowedLength005e5790() == 0) {                                     // 006F9D12..1D
        void* const box = message_box();                                          // 006F9D1F..2D
        kotor_fastcall_00627130(box, nullptr, 0);                                 // 006F9D33..36 SetAllowCancel(0)
        gui_load_messagebox_set_callback_00624a40(box, nullptr, nullptr, nullptr, 0);   // 006F9D3B..40
        slot<void (__fastcall*)(void*, void*, Word)>(box, 0x6C)(box, nullptr, 0xA578u);   // 006F9D45..4E
        kotor_fastcall_0040bc70(self->manager, nullptr, box, 1, 1);               // 006F9D51..59 AddPanel(box, 1, 1)
        return;
    }
    while (directory_list::At005e5600(&name, nullptr, name.BorrowedLength005e5790() - 1) == ' ') {   // 006F9D63..79 / 006F9DB5..CB
        Fresh shorter;
        shorter.object = base_ini_reconstruction::left_005e5f70(&name, nullptr, shorter.storage(),
                                                                name.BorrowedLength005e5790() - 1);   // 006F9D80..94
        name.Assign005e5c50(shorter.object);                                      // 006F9D99..A3
    }
    {
        String last("");                                                          // 006F9DCD..D6 (0073D71C)
        kotor_m15a_creature_set_name_0060d600(self->creature, nullptr, &name, &last);   // 006F9DDB..ED
    }
    (void)kotor_fastcall_0040be00(self->manager, nullptr);                        // 006F9DFF..E02 PopModalPanel
    kotor_m15_panel_set_visible_0040b9d0(self->parent, nullptr, 1);               // 006F9E07..0C
    mark_done(self);                                                              // 006F9E11..1E
    if (self->parent_kind == 1) kotor_m15c_quick_name_creation_completed_006eff80(static_cast<Quick*>(self->parent), nullptr);   // 006F9E2B..2E
    else if (self->parent_kind == 2) kotor_m15c_custom_name_creation_completed_006ef4e0(self->parent, nullptr);               // 006F9E39..3C
}
