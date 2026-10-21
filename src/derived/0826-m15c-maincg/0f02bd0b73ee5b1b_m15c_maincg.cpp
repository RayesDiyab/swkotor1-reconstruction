// lane-m15-c (L15-C, Milestone 1.5): CSWGuiMainCharGen - the char-gen frame (stat labels, portrait, 3D model view)
// hosting the QuickOrCustom / Quick / Custom sub-panels.
//   ctor 006EB420, dtors 006EBF00 / 006EBEE0, SetVisiblePanel 006EA760, OnPanelAdded 006EA810, RemoveSubPanels 006EA830,
//   DetermineClass 006EA910, SetNameString 006EA9B0, PortraitChanged 006EA9F0, AbilitiesChanged 006EAC30,
//   ClearAbilities 006EB010, Draw 006EB340, NameChanged 006EEF70; method table 007592A8 (= CSWGuiPanel 0073E010 except
//   +00 006EBEE0, +34 006EB340, +48 006EA810). BeginPlayGame 006EB320 is the lane-m15-f Play-boundary trap.
// Original: swkotor.exe GOG K1 (image base 0x400000), static disassembly only; each behaviour cites its instruction
// addresses. D30-FINAL: wide x87 values are doubles, floats only at the original FSTP dword stores (006EAB4F, 006EABCE,
// 006EABE1, 006EBEB6, 006EAB34, 006EB3EE). Recorded deviation: Yaw 004AA0F0 / Pitch 004AA130 (lane-m15-a) return a
// C++ float, so the ST0 value used by FSUBR/FADD at 006EABC4 / 006EABD7 is the float-rounded result.
#include "88acda4ea129c9a0_m15c_common.h"

using namespace kotor_m15c;
using gui_load::Control;

extern "C" {
V3* __fastcall vector_subtract_assign_004a9770(V3* self, void* unused_edx, const V3* other);   // core46-math 004A9770
void __fastcall vector_normalize_004ab130(V3* self, void* unused_edx);                          // 3d-model 004AB130
Q4* __cdecl yaw_pitch_roll_004acac0(Q4* out, float yaw, float pitch, float roll);               // core46-math 004ACAC0
}

namespace {
MainCharGen* mcg(Panel* p) { return static_cast<MainCharGen*>(p); }
void* label_params(MainCharGen* self, MainLabel k) { return label_text(&self->labels[k]); }
float bits_float(Word bits) { float f; std::memcpy(&f, &bits, 4); return f; }
// SetText(label, CExoString(int)) with the temporary destroyed after the call (006EAC87..CB8 pattern).
void set_number(MainCharGen* self, MainLabel k, int value) {
    Fresh number;
    number.object = kotor_fastcall_005e5bc0(number.storage(), nullptr, value);   // 005E5BC0
    set_text(label_params(self, k), number.object);                               // 00415E00
}
void set_empty(MainCharGen* self, MainLabel k) {
    String empty("");                                                             // 005E5A90 (0073D71C)
    set_text(label_params(self, k), &empty);
}
// RemoveSubPanels step (006EA83B..6F etc.): a panel still registered is marked 400h; otherwise it is deleted.
template <class P> void retire(MainCharGen* self, P*& panel) {
    if (frame_ContainsPanel_0040bd70(self->manager, nullptr, panel)) {             // 0040BD70 PanelExists
        mark_done(panel);
        return;
    }
    if (panel) panel->vtbl->deleting_destroy(panel, nullptr, 1);                  // [vtbl+0](1)
    panel = nullptr;
}
void label_ctor(Label* p) { (void)gui_load_label_construct_0041acd0(p, nullptr); }
void label_dtor(Label* p) { gui_load_label_destruct_00419d10(p, nullptr); }
}

extern "C" {
// ======================================================================= method table 007592A8
const KotorM15cPanelMethods kotor_m15c_main_chargen_methods_007592a8 = {
    &kotor_m15c_main_chargen_deleting_destruct_006ebee0, &gui_load_panel_set_extent_0040a5a0,
    &gui_load_panel_set_active_control_0040a630, &gui_load_slot_ret4_0060e760, &gui_load_slot_zero8_00409eb0,
    {&gui_load_slot_ret_004015a0, &gui_load_slot_ret_004015a0, &gui_load_slot_ret_004015a0, &gui_load_slot_ret_004015a0,
     &gui_load_slot_ret_004015a0},
    &gui_load_slot_self_00641db0, &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0,
    &kotor_m15c_main_chargen_draw_006eb340, &gui_load_slot_ret4_0060e760, &gui_load_panel_handle_input_00409e60,
    &gui_load_panel_hit_check_0040b690, &gui_load_panel_active_control_0040b630, &kotor_m15c_main_chargen_on_panel_added_006ea810,
    &gui_load_panel_on_removed_0040c170,
    {&gui_load_panel_button_a_0040b640, &gui_load_panel_button_b_0040b650, &gui_load_panel_button_x_0040b660,
     &gui_load_panel_button_y_0040b670, &gui_load_panel_button_black_0040b680},
    &gui_load_panel_full_screen_bg_0040a900, &gui_load_panel_reset_font_0040a9c0,
};

// ======================================================================= 006EB420 ctor(manager, creature, class selection)
// RET 0Ch. EH states 0..1Eh = members (27 labels, scene view control, scene, 2 strings; FuncInfo 007385F2).
void* __fastcall kotor_m15c_main_chargen_construct_006eb420(void* memory, void*, void* manager, void* creature,
                                                            void* class_selection) noexcept(false) {
    MainCharGen* const self = static_cast<MainCharGen*>(memory);
    (void)gui_load_panel_construct_0040b570(self, nullptr, static_cast<Object*>(manager));   // 006EB446..4B
    int labels = 0, built = 0;
    try {
        self->vtbl = reinterpret_cast<const gui_load::ObjectVtbl*>(&kotor_m15c_main_chargen_methods_007592a8);   // 006EB45D
        for (; labels < 27; ++labels) label_ctor(&self->labels[labels]);          // 006EB463..603
        (void)gui_load_control_construct_0041aa80(&self->model, nullptr); built = 1;   // 006EB619
        self->model.vtbl = reinterpret_cast<const gui_load::ObjectVtbl*>(&gui_load_scene_view_methods_00752e30);   // 006EB626
        (void)gui_scene_construct_004174b0(&self->model.scene, nullptr); built = 2;   // 006EB62C
        new (self->class_name) String(); built = 3;                               // 006EB63C
        new (self->name) String(); built = 4;                                     // 006EB64C
        self->class_selection = class_selection;                                  // 006EB651..54
        self->manager = static_cast<Object*>(kotor_fastcall_005eda40(client_app(), nullptr));   // 006EB657..6B GetSWGuiManager
        self->creature = creature;                                                // 006EB66E..73
        kotor_m15c_main_chargen_determine_class_006ea910(self, nullptr);          // 006EB676
        {
            gui_load::Ref layout;
            layout.construct_00406d80("MAINCG");                                 // 006EB67B..84 (007590A0)
            gui_load_panel_start_layout_0040a680(self, nullptr, &layout);         // 006EB689..90
        }
        static const struct { MainLabel label; const char* tag; } kTags[] = {     // 006EB695..BC49 (order of the original)
            {kMainTitle, "MAIN_TITLE_LBL"}, {kName, "LBL_NAME"}, {kClass, "LBL_CLASS"}, {kStr, "STR_LBL"},
            {kDex, "DEX_LBL"}, {kCon, "CON_LBL"}, {kInt, "INT_LBL"}, {kWis, "WIS_LBL"}, {kCha, "CHA_LBL"},
            {kStrAb, "STR_AB_LBL"}, {kDexAb, "DEX_AB_LBL"}, {kConAb, "CON_AB_LBL"}, {kIntAb, "INT_AB_LBL"},
            {kWisAb, "WIS_AB_LBL"}, {kChaAb, "CHA_AB_LBL"}, {kNewWill, "NEW_WILL_LBL"}, {kNewRefl, "NEW_REFL_LBL"},
            {kNewFort, "NEW_FORT_LBL"}, {kBevelL, "LBL_BEVEL_L"}, {kBevelM, "LBL_BEVEL_M"}, {kBevelR, "LBL_BEVEL_R"},
            {kFortitude, "LBL_FORTITUDE"}, {kReflex, "LBL_REFLEX"}, {kWill, "LBL_WILL"}, {kVit, "LBL_VIT"},
            {kDef, "LBL_DEF"},
        };
        for (const auto& t : kTags) init_control(self, &self->labels[t.label], t.tag);
        init_control(self, &self->model, "MODEL_LBL");                            // 006EBBF1..C14 (00758FCC)
        init_control(self, &self->labels[kPortrait], "PORTRAIT_LBL");             // 006EBC26..49 (00758FBC)
        gui_load_panel_stop_layout_0040b8f0(self, nullptr);                       // 006EBC5B..5D
        self->quick_or_custom = static_cast<QuickOrCustom*>(new_panel(0xD98, [self](void* m) {   // 006EBC62..8F
            return static_cast<void*>(kotor_m15c_quick_or_custom_construct_006f09f0(static_cast<QuickOrCustom*>(m), nullptr,
                                                                                    self->manager, self->creature));
        }));
        st<MainCharGen*>(reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(self->quick_or_custom)), 0x68, self);   // 006EBC9B
        self->quick = static_cast<Quick*>(new_panel(0x1360, [self](void* m) {     // 006EBC92..C6
            return static_cast<void*>(kotor_m15c_quick_construct_006f0390(static_cast<Quick*>(m), nullptr, self->manager,
                                                                          self->creature));
        }));
        st<MainCharGen*>(reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(self->quick)), 0x68, self);   // 006EBCD2
        self->custom = static_cast<Custom*>(new_panel(0x2188, [self](void* m) {   // 006EBCC9..D04
            return static_cast<void*>(kotor_m15c_custom_construct_006ef730(static_cast<Custom*>(m), nullptr, self->manager,
                                                                           self->creature));
        }));
        kotor_m15c_custom_set_parent_panel_006eef60(self->custom, nullptr, self); // 006EBCFD..D07
        self->visible_panel = 1;                                                  // 006EBD15
        {
            String empty("");                                                     // 006EBD0C..1F
            str(self->name)->Assign005e5c50(&empty);                              // 006EBD24..34
            set_text(label_params(self, kName), str(self->name));                 // 006EBD39..46 (+2A0)
            set_text(label_params(self, kClass), str(self->class_name));          // 006EBD4B..58 (+3E0)
        }
        {
            ResRef16 portrait;
            ResRef16* const ref = slot<CreatureGetPortrait>(self->creature, 0x148)(self->creature, nullptr, &portrait, 0);   // 006EBD6A..76
            gui_load_border_params_set_fill_00414c00(self->labels[kPortrait].border.params, nullptr,
                                                     reinterpret_cast<const gui_load::ResRef*>(ref), 0);   // 006EBD7C..85 (+19E8)
        }
        void* const aur_scene = self->model.scene.aur_scene;                      // 006EBD91 ([+22A8])
        void* const camera = self->model.scene.aur_camera;                        // 006EBD97 ([+22AC])
        if (kotor_cell_do3dgui_0078d1e4) {                                        // 006EBD8A..A5
            const float position[3] = {0.0f, 0.0f, 0.0f};                         // 006EBDDC..EC
            const float orientation[4] = {0.0f, 0.0f, 0.0f, 1.0f};                // 006EBDBC..D4
            slot<SceneSpawnRoom>(aur_scene, 0x70)(aur_scene, nullptr, "gui3D_room", position, orientation);   // 006EBDAB..F4 (00753038)
            {
                String light("charrec_light");                                    // 006EBDF7..E00 (00756174)
                (void)aurora_gui_scene_add_model_name_00417620(&self->model.scene, nullptr, &light, -1);   // 006EBE05..17
            }
            kotor_m15a_creature_setup_3d_scene_006100f0(self->creature, nullptr, &self->model, nullptr, nullptr, 0);   // 006EBE29..33
            slot<CreatureAttach>(self->creature, 0x94)(self->creature, nullptr, aur_scene);   // 006EBE38..42
            const V3 origin = {0.0f, 0.0f, 0.0f};                                 // 006EBE4F..5F
            slot<CreatureSetPosition>(self->creature, 0x8C)(self->creature, nullptr, &origin);   // 006EBE48..6A
            if (unsigned char* const behavior = camera_behavior(camera)) {        // 006EBE70..85
                st<Word>(behavior, 0x48, 1);                                      // 006EBE87
                const float h = head_height(self->creature);                      // 006EBE8E..B6
                store_offset(behavior, 0.0f, 0.0f, h);                            // 006EBEA9..C1
            }
        }
    } catch (...) {
        if (built >= 4) str(self->name)->~StartupString();
        if (built >= 3) str(self->class_name)->~StartupString();
        if (built >= 2) gui_scene_destruct_004165e0(&self->model.scene, nullptr);
        if (built >= 1) gui_load_control_destruct_00419a20(&self->model, nullptr);
        while (labels-- > 0) label_dtor(&self->labels[labels]);
        gui_load_panel_destruct_0040cf70(self, nullptr);
        throw;
    }
    return self;                                                                  // 006EBEC9
}
// 006EBF00 dtor (no vptr store): strings, scene view, labels in reverse, ~CSWGuiPanel. The sub-panels are not owned
// here (RemoveSubPanels 006EA830 retires them).
void __fastcall kotor_m15c_main_chargen_destruct_006ebf00(MainCharGen* self, void*) {
    str(self->name)->~StartupString();                                            // 006EBF1D..2B
    str(self->class_name)->~StartupString();                                      // 006EBF30..3B
    gui_load_scene_view_destruct_0067ac50(&self->model, nullptr);                 // 006EBF40..4B
    for (int k = 27; k-- > 0;) label_dtor(&self->labels[k]);                      // 006EBF50..F8
    gui_load_panel_destruct_0040cf70(self, nullptr);                              // 006EBFFD..C107
}
// 006EBEE0 scalar deleting destructor (RET 4).
void* __fastcall kotor_m15c_main_chargen_deleting_destruct_006ebee0(Panel* self, void*, Word flags) {
    kotor_m15c_main_chargen_destruct_006ebf00(mcg(self), nullptr);
    if (flags & 1u) live_free(self);                                              // 006EBEE8..F5
    return self;
}
// 006EA760 SetVisiblePanel(int panel) (RET 4): 1 QuickOrCustom, 2 Quick, 3 Custom; anything else -> 4 (none).
void __fastcall kotor_m15c_main_chargen_set_visible_panel_006ea760(MainCharGen* self, void*, Word panel) noexcept(false) {
    if (panel == self->visible_panel) return;                                     // 006EA768..6E
    (void)kotor_fastcall_0040be00(self->manager, nullptr);                        // 006EA774..78 PopModalPanel
    switch (self->visible_panel) {                                                // 006EA77D..97
    case 1: frame_RemovePanel_0040c830(self->manager, nullptr, self->quick_or_custom); break;
    case 2: frame_RemovePanel_0040c830(self->manager, nullptr, self->quick); break;
    case 3: frame_RemovePanel_0040c830(self->manager, nullptr, self->custom); break;
    default: self->visible_panel = 4; break;
    }
    if (self->visible_panel == 4) return;                                         // 006EA7B1..B7
    switch (panel) {                                                              // 006EA7B9..EB
    case 1: kotor_fastcall_0040bc70(self->manager, nullptr, self->quick_or_custom, 1, 1); break;
    case 2: kotor_fastcall_0040bc70(self->manager, nullptr, self->quick, 1, 1); break;
    case 3: kotor_fastcall_0040bc70(self->manager, nullptr, self->custom, 1, 1); break;
    default: self->visible_panel = 4; break;
    }
    if (self->visible_panel != 4) self->visible_panel = panel;                    // 006EA7F0..F8
}
// 006EA810 OnPanelAdded (RET): AddPanel(QuickOrCustom, 1, 1); CSWGuiPanel::OnPanelAdded (tail jump).
void __fastcall kotor_m15c_main_chargen_on_panel_added_006ea810(Panel* panel, void*) noexcept(false) {
    MainCharGen* const self = mcg(panel);
    kotor_fastcall_0040bc70(self->manager, nullptr, self->quick_or_custom, 1, 1); // 006EA813..1E
    gui_load_panel_on_added_0040b870(self, nullptr);                              // 006EA823..26
}
// 006EA830 RemoveSubPanels (RET).
void __fastcall kotor_m15c_main_chargen_remove_sub_panels_006ea830(MainCharGen* self, void*) noexcept(false) {
    (void)kotor_fastcall_0040be00(self->manager, nullptr);                        // 006EA833..36
    retire(self, self->custom);                                                   // 006EA83B..6F
    retire(self, self->quick_or_custom);                                          // 006EA876..AA
    retire(self, self->quick);                                                    // 006EA8B1..E5
    (void)kotor_fastcall_0040be00(self->manager, nullptr);                        // 006EA8EC..EF
    mark_done(self);                                                              // 006EA8F4..901
}
// 006EA910 DetermineClass (RET): class_name = GetClassesString(' ') up to the first '('.
void __fastcall kotor_m15c_main_chargen_determine_class_006ea910(MainCharGen* self, void*) noexcept(false) {
    {
        Fresh classes;
        classes.object = static_cast<String*>(kotor_m15b_creature_stats_get_classes_string_00648470(
            stats_of(self->creature), nullptr, classes.storage(), 0x20));        // 006EA92B..3B
        str(self->class_name)->Assign005e5c50(classes.object);                    // 006EA940..51
    }
    const std::int32_t open = find_005e5f20(str(self->class_name), nullptr, 0x28, 0);   // 006EA967..6D
    Fresh left;
    left.object = base_ini_reconstruction::left_005e5f70(str(self->class_name), nullptr, left.storage(), open);   // 006EA972..7A
    str(self->class_name)->Assign005e5c50(left.object);                           // 006EA97F..8A
}
// 006EA9B0 SetNameString(const CExoString&) (RET 4).
void __fastcall kotor_m15c_main_chargen_set_name_string_006ea9b0(MainCharGen* self, void*, gui_load::String* name) noexcept(false) {
    str(self->name)->Assign005e5c50(name);                                        // 006EA9B8..C1
    set_text(label_params(self, kName), str(self->name));                         // 006EA9C6..CD (+2A0)
    set_text(label_params(self, kClass), str(self->class_name));                  // 006EA9D2..DF (+3E0)
}
// 006EEF70 NameChanged (RET): SetNameString(creature->GetName()).
void __fastcall kotor_m15c_main_chargen_name_changed_006eef70(MainCharGen* self, void*) noexcept(false) {
    Fresh name;
    name.object = slot<CreatureGetName>(self->creature, 0x70)(self->creature, nullptr, name.storage());   // 006EEF8B..95 [+70]
    kotor_m15c_main_chargen_set_name_string_006ea9b0(self, nullptr, name.object); // 006EEF98..A3
}
// 006EA9F0 PortraitChanged (RET): portrait label image, model appearance refresh, camera framing on the head.
void __fastcall kotor_m15c_main_chargen_portrait_changed_006ea9f0(MainCharGen* self, void*) noexcept(false) {
    {
        ResRef16 portrait;
        ResRef16* const ref = slot<CreatureGetPortrait>(self->creature, 0x148)(self->creature, nullptr, &portrait, 0);   // 006EAA0E..1A
        gui_load_border_params_set_fill_00414c00(self->labels[kPortrait].border.params, nullptr,
                                                 reinterpret_cast<const gui_load::ResRef*>(ref), 0);   // 006EAA20..29 (+19E8)
    }
    {
        String empty("");                                                         // 006EAA2E..37
        set_text(label_params(self, kPortrait), &empty);                          // 006EAA3C..52 (+1A60)
    }
    unsigned char appearance[0x3C];                                               // [esp+4Ch]
    std::memcpy(appearance, ld<void*>(self->creature, 0x21C), sizeof(appearance)); // 006EAA6B..83 REP MOVSD 0Fh
    void* const camera = self->model.scene.aur_camera;                            // 006EAA74 ([+22AC])
    st<std::uint16_t>(appearance, 0x18, ld<std::uint16_t>(stats_of(self->creature), 0x8C));   // 006EAA85..9A
    if (kotor_cell_do3dgui_0078d1e4) {                                            // 006EAA92..9F
        (void)kotor_m15a_creature_set_appearance_006134c0(self->creature, nullptr, appearance, 3, 1);   // 006EAAA5..B0
        slot<CreatureAttach>(self->creature, 0x94)(self->creature, nullptr, self->model.scene.aur_scene);   // 006EAAB5..C1
        const V3 origin = {0.0f, 0.0f, 0.0f};                                     // 006EAACA..DE
        slot<CreatureSetPosition>(self->creature, 0x8C)(self->creature, nullptr, &origin);   // 006EAAC7..E9
        if (unsigned char* const behavior = camera_behavior(camera)) {            // 006EAAEF..03
            st<Word>(behavior, 0x48, 1);                                          // 006EAB05
            const float h = head_height(self->creature);                          // 006EAB0C..34
            store_offset(behavior, 0.0f, 0.0f, h);                                // 006EAB27..3F
        }
    }
    const float h = head_height(self->creature);                                  // 006EAB42..4F (FSTP dword)
    V3 eye = {2.5f, 0.0f, h};                                                     // 006EAB56..7C (40200000h, 0, h)
    {
        V3 discarded;
        (void)slot<CameraSetPosition>(camera, 0x18)(camera, nullptr, &discarded, eye);   // 006EAB7F..86 [+18]
    }
    const V3 up = {0.0f, 0.0f, 1.0f};                                             // 006EAB92..A2
    (void)vector_subtract_assign_004a9770(&eye, nullptr, &up);                    // 006EAB89..AA
    vector_normalize_004ab130(&eye, nullptr);                                     // 006EABAF..B3
    const float pitch = static_cast<float>(static_cast<double>(bits_float(0x42B40000u)) -
                                           static_cast<double>(kotor_m15a_pitch_004aa130(&eye)));   // 006EABBF..CE (90 - pitch)
    const float yaw = static_cast<float>(static_cast<double>(kotor_m15a_yaw_004aa0f0(&eye)) +
                                         static_cast<double>(bits_float(0x43340000u)));   // 006EABD2..E1 (yaw + 180)
    Q4 rotation;
    const Q4* const q = yaw_pitch_roll_004acac0(&rotation, yaw, pitch, 0.0f);    // 006EABE4..E5 (roll = pushed 0)
    Q4 discarded;
    (void)slot<CameraSetOrientation>(camera, 0x1C)(camera, nullptr, &discarded, *q);   // 006EABEA..C0C [+1C]
}
// 006EAC30 AbilitiesChanged (RET): saving throws, ability labels, defense, vitality.
void __fastcall kotor_m15c_main_chargen_abilities_changed_006eac30(MainCharGen* self, void*) noexcept(false) {
    kotor_m15b_levelup_stats_set_fort_saving_throw_00648b30(stats_of(self->creature), nullptr);     // 006EAC53..5D
    kotor_m15b_levelup_stats_set_will_saving_throw_00648be0(stats_of(self->creature), nullptr);     // 006EAC62..6B
    kotor_m15b_levelup_stats_set_reflex_saving_throw_00648c90(stats_of(self->creature), nullptr);   // 006EAC70..79
    set_number(self, kChaAb, ld<std::uint8_t>(stats_of(self->creature), 0x3F));   // 006EAC7E..B8 (+12E0)
    set_number(self, kStrAb, ld<std::uint8_t>(stats_of(self->creature), 0x3A));   // 006EACBD..F4 (+CA0)
    set_number(self, kIntAb, ld<std::uint8_t>(stats_of(self->creature), 0x3D));   // 006EACF9..D30 (+1060)
    set_number(self, kWisAb, ld<std::uint8_t>(stats_of(self->creature), 0x3E));   // 006EAD35..6C (+11A0)
    set_number(self, kConAb, ld<std::uint8_t>(stats_of(self->creature), 0x3C));   // 006EAD71..A8 (+F20)
    set_number(self, kDexAb, ld<std::uint8_t>(stats_of(self->creature), 0x3B));   // 006EADAD..E4 (+DE0)
    void* stats = stats_of(self->creature);                                       // 006EADE9..EC
    const std::int8_t dex_mod = kotor_m15b_creature_stats_ability_mod_00647630(stats, nullptr, ld<std::uint8_t>(stats, 0x35));   // 006EADF2..00
    int defense = static_cast<int>(static_cast<std::int8_t>(dex_mod + ld<std::int8_t>(stats, 0x40))) + 10;   // 006EAE05..17
    const unsigned char* cls = nullptr;
    const std::uint8_t count = ld<std::uint8_t>(stats_of(self->creature), 0x32);  // 006EAE0B..14
    if (count == 0) {
        // 006EAE83: with no class the original takes a dead stack word (the destroyed CExoString's data field, 0 after
        // 005E5C20) as the class pointer and faults at MOV BL,[EDI+54h] (006EAEA4). Char-gen creatures always carry one
        // class (CSWGuiClassSelection sets it before 006DBA3F), so this branch is a D83 branch trap (TRAPS-DELTA.json).
        kotor_unreachable_trap(0x006EAE83ul, "CSWGuiMainCharGen::AbilitiesChanged (no class)");
    }
    for (Word i = 0; i < ld<std::uint8_t>(stats_of(self->creature), 0x32); ++i) {   // 006EAE26..7F (count re-read)
        const std::uint8_t class_id = kotor_m15b_creature_stats_get_class_00647730(stats_of(self->creature), nullptr,
                                                                                   static_cast<std::uint8_t>(i));   // 006EAE26..2B
        cls = ld<const unsigned char*>(kotor_cell_rules_007a3a28, 0xAC) + class_id * 0x1A0u;   // 006EAE34..42
        const std::uint8_t level = kotor_m15b_creature_stats_get_class_level_00647750(stats_of(self->creature), nullptr,
                                                                                      static_cast<std::uint8_t>(i));   // 006EAE48..52
        defense += static_cast<std::int8_t>(kotor_m15b_class_get_armor_bonus_005be770(const_cast<unsigned char*>(cls), nullptr,
                                                                                       level));   // 006EAE57..6C
    }
    st<std::uint16_t>(stats_of(self->creature), 0x50, static_cast<std::uint16_t>(defense));   // 006EAE87..90
    stats = stats_of(self->creature);                                             // 006EAE94..97
    const std::int16_t hp_base = ld<std::int16_t>(stats, 0x48);                   // 006EAE9D
    const std::uint8_t hit_die = cls[0x54];                                       // 006EAEA4
    const std::int8_t con_mod = kotor_m15b_creature_stats_ability_mod_00647630(stats, nullptr, ld<std::uint8_t>(stats, 0x36));   // 006EAEA1..B5
    const int vitality = static_cast<int>(con_mod) + static_cast<int>(hp_base) + static_cast<int>(hit_die);   // 006EAEBD..CA
    set_number(self, kDef, kotor_m15b_creature_stats_get_armor_class_00647720(stats_of(self->creature), nullptr, 0));   // 006EAECC..F06 (+1560)
    set_number(self, kNewFort, ld<std::int8_t>(stats_of(self->creature), 0x5C));  // 006EAF0B..42 (+1920)
    set_number(self, kNewRefl, ld<std::int8_t>(stats_of(self->creature), 0x5E));  // 006EAF47..7E (+17E0)
    set_number(self, kVit, vitality);                                             // 006EAF83..AD (+1420)
    set_number(self, kNewWill, ld<std::int8_t>(stats_of(self->creature), 0x5D));  // 006EAFB2..E9 (+16A0)
}
// 006EB010 ClearAbilities (RET): blank the ability / save / defense labels, reset bases to 8, clear the package.
void __fastcall kotor_m15c_main_chargen_clear_abilities_006eb010(MainCharGen* self, void*) noexcept(false) {
    set_empty(self, kChaAb);                                                      // 006EB032..63 (+12E0)
    set_empty(self, kStrAb);                                                      // 006EB068..96 (+CA0)
    set_empty(self, kIntAb);                                                      // 006EB09B..C9 (+1060)
    set_empty(self, kWisAb);                                                      // 006EB0CE..FC (+11A0)
    set_empty(self, kConAb);                                                      // 006EB101..2F (+F20)
    set_empty(self, kDexAb);                                                      // 006EB134..62 (+DE0)
    kotor_m15b_levelup_stats_set_str_base_0064a0d0(stats_of(self->creature), nullptr, 8);       // 006EB167..72
    kotor_m15b_levelup_stats_set_int_base_006478a0(stats_of(self->creature), nullptr, 8);       // 006EB177..82
    kotor_m15b_levelup_stats_set_wis_base_00649770(stats_of(self->creature), nullptr, 8);       // 006EB187..92
    kotor_m15b_levelup_stats_set_con_base_006496d0(stats_of(self->creature), nullptr, 8);       // 006EB197..A2
    kotor_m15b_levelup_stats_set_dex_base_0064a100(stats_of(self->creature), nullptr, 8, 1);    // 006EB1A7..B4
    kotor_m15b_levelup_stats_set_cha_base_006478d0(stats_of(self->creature), nullptr, 8);       // 006EB1B9..C4
    kotor_m15b_levelup_stats_set_fort_saving_throw_00648b30(stats_of(self->creature), nullptr);     // 006EB1C9..D2
    kotor_m15b_levelup_stats_set_will_saving_throw_00648be0(stats_of(self->creature), nullptr);     // 006EB1D7..E0
    kotor_m15b_levelup_stats_set_reflex_saving_throw_00648c90(stats_of(self->creature), nullptr);   // 006EB1E5..EE
    set_empty(self, kDef);                                                        // 006EB1F3..221 (+1560)
    set_empty(self, kNewFort);                                                    // 006EB226..254 (+1920)
    set_empty(self, kNewRefl);                                                    // 006EB259..287 (+17E0)
    set_empty(self, kVit);                                                        // 006EB28C..2BA (+1420)
    set_empty(self, kNewWill);                                                    // 006EB2BF..2ED (+16A0)
    kotor_m15b_levelup_stats_clear_package_00649a70(stats_of(self->creature), nullptr);         // 006EB2F2..FB
}
// 006EB340 Draw(float delta) (RET 4): keep the model in the panel scene and run the char-gen idle cycle while the panel
// is shown (+44 bit7), then CSWGuiPanel::Draw.
void __fastcall kotor_m15c_main_chargen_draw_006eb340(Panel* panel, void*, Word delta_bits) noexcept(false) {
    MainCharGen* const self = mcg(panel);
    if (self->creature && kotor_cell_do3dgui_0078d1e4 && (static_cast<unsigned char>(self->flags) & 0x80u)) {   // 006EB34B..68
        void* const body = creature_model(self->creature, 0xFF);                  // 006EB36F..7D
        void* const aur_scene = self->model.scene.aur_scene;                      // 006EB382 ([+22A8])
        if (slot<GobScene>(body, 0x104)(body, nullptr) != aur_scene) {            // 006EB380..92
            slot<CreatureAttach>(self->creature, 0x94)(self->creature, nullptr, self->model.scene.aur_scene);   // 006EB394..A0
            if (unsigned char* const behavior = camera_behavior(self->model.scene.aur_camera)) {   // 006EB3A6..BD
                st<Word>(behavior, 0x48, 1);                                      // 006EB3BF
                const float h = head_height(self->creature);                      // 006EB3C6..EE
                store_offset(behavior, 0.0f, 0.0f, h);                            // 006EB3E1..F9
            }
        }
        kotor_m15a_creature_cg_pause_cycle_0060f7c0(self->creature, nullptr, bits_float(delta_bits), 0);   // 006EB3FC..402
    }
    kotor_panel_draw_0040B760(self, nullptr, delta_bits);                         // 006EB408..0B
}
}
