#pragma once
// Lane gui-load (round 2): native constructors/destructors, parameter setters,
// trivial and small virtual slots, control events, panel helpers and method
// tables for the CSWGui* classes created by CSWGuiMainMenu::CSWGuiMainMenu 0067C4C0.
//
// Conventions (DECISIONS D1/D21/D24): every original __thiscall entry is exported
// as extern "C" __fastcall(Self*, void* unused_edx, original stack args...) with
// identical callee cleanup. Method tables are typed constant structs whose fields
// have the exact declared type of the function they hold (fastcall pointers, or
// single-inheritance PMFs for lane-input's kotor_input_hover members). Objects
// receive a table address through an object-pointer conversion; callers read the
// slot they need through the ABI-identical D1 views of gui_control_load.h.
// Layout views cite the instruction that establishes every named offset.
#include "2a3f105af3-gui_control_load.h"
#include "bf5740dc64-hover_path.h"          // lane-input: kotor_input_hover (PMF slot owners, Manager, ControlEvent)
#include "5413f244d4-window_shared_cells.h" // M40: kotor_menu_window_cells::manager_007a39fc (007A39FC), height_0078d1d8
#include "b035993e55-listbox.h"            // ListBox fork of this lane (shared declarations, NewCAurGUIImage proposal)
#include "55a7c26ac3-string_priority.h"    // M40: GuiConcat005e5d10 (CExoString::operator+ 005E5D10)
#include <cstddef>
#include <cstdint>

namespace gui_load {
using I32 = std::int32_t;
using Ref = menu_resources::Ref;
namespace kih = kotor_input_hover;

// CAurGUIImage / CAurGUIString (Aurora GUI objects): only called slots are typed.
struct AurImage;
struct AurImageVtbl {
    const void* slot00;
    void* (__fastcall* release)(AurImage*, void*, Word flags);   // +04 called with 1 (00414B24, 004151F5)
    const void* slot08;
    I32 (__fastcall* width)(AurImage*, void*);                  // +0C (00414CF4, 00415282)
    I32 (__fastcall* height)(AurImage*, void*);                 // +10 (0041528E)
};
struct AurImage { const AurImageVtbl* vtbl; };
struct AurString;
struct AurStringVtbl {
    const void* slot00;
    void* (__fastcall* release)(AurString*, void*, Word flags);  // +04 (00416026, 00417323)
    const void* slots08_14[4];
    void (__fastcall* set_text)(AurString*, void*, const char*);  // +18 (00415E33, 00415EE4)
    void (__fastcall* set_alignment)(AurString*, void*, Word);    // +1C (00415F52)
    const void* slots20_40[9];
    void (__fastcall* set_font)(AurString*, void*, const char*);  // +44 (00415D4D)
    I32 (__fastcall* font_height)(AurString*, void*);             // +48 (00414EBC)
    void (__fastcall* wrap)(AurString*, void*, I32);              // +4C (00414EAB) called with -1
    I32 (__fastcall* text_height)(AurString*, void*, I32);        // +50 (00414ECB/D3) called with -1
    const void* slots54_60[4];
    void (__fastcall* set_position)(AurString*, void*, I32, I32); // +64 (004162B3)
    const void* slots68_70[3];
    void (__fastcall* set_size)(AurString*, void*, I32, I32);     // +74 (004162C3)
};
static_assert(offsetof(AurStringVtbl, set_text) == 0x18 && offsetof(AurStringVtbl, set_font) == 0x44 &&
              offsetof(AurStringVtbl, wrap) == 0x4C && offsetof(AurStringVtbl, set_position) == 0x64 &&
              offsetof(AurStringVtbl, set_size) == 0x74, "aur string slots");
struct AurString { const AurStringVtbl* vtbl; };

// CSWGuiBorderParams (0x54 at CSWGuiBorder+14; ctor 00415000).
struct BorderParams {
    I32 dimension;          // +00 (00415071 a5; 004154FA DIMENSION; 00414CD0 read)
    I32 inner_offset;       // +04 (004150AC a8; INNEROFFSET via border+18)
    Word u08;               // +08 (004150A6 zero; 00415114 float copy)
    float f0c;              // +0C (00415081 a6 = 1.0f)
    Vector color;           // +10 (00415088..A9)
    Word flags;             // +1C bits0-1 fill style, 2-4 pulsing (00415073, 00414D28)
    Ref corner;             // +20 (00415019)
    Ref edge;               // +30 (00415023)
    Ref fill;               // +40 (0041502D)
    Border* owner;          // +50 (00414C99 SetBorder)
};
static_assert(sizeof(BorderParams) == 0x54 && offsetof(BorderParams, flags) == 0x1C &&
              offsetof(BorderParams, owner) == 0x50, "border params");
// CSWGuiBorder full view (0x74): images at +68/+6C/+70 (00416879..7F, 004151E4..1A).
struct BorderObject {
    const BorderVtbl* vtbl;
    Extent extent;          // +04
    BorderParams params;    // +14
    AurImage* corner;       // +68
    AurImage* edge;         // +6C
    AurImage* fill;         // +70
};
static_assert(sizeof(BorderObject) == sizeof(Border), "border view");

// CSWGuiTextParams (0x58 at CSWGuiText+18; ctor 00417050).
struct TextObject;
struct TextParams {
    unsigned char text[8];  // +00 CExoString (00417070)
    Word strref;            // +08 = -1 (004170E0); 00415E72 / 00415E12
    Ref font;               // +0C (0041707E; SetFont 00415D16)
    float color[3];         // +1C (004170A4/C8/DA)
    float f28;              // +28 = 1.0f (004170E7)
    float default_color[3]; // +2C (004170A1/C5/D7; 00414E5D)
    Word flags;             // +38 (x & 0xFFFFFF89) | 9 (004170B6/CB/EE)
    char base_font[0x14];   // +3C (SetBaseFont 00415DDA)
    TextObject* text_object;// +50 (004170DD; 00415F7C)
    Word u54;               // +54 (CSWGuiText+6C = 0x190 at 004172D6)
};
static_assert(sizeof(TextParams) == 0x58 && offsetof(TextParams, color) == 0x1C &&
              offsetof(TextParams, default_color) == 0x2C && offsetof(TextParams, flags) == 0x38 &&
              offsetof(TextParams, base_font) == 0x3C && offsetof(TextParams, text_object) == 0x50, "text params");
struct TextObject {
    const TextVtbl* vtbl;
    Extent extent;          // +04 (004172A1..AA)
    AurString* string;      // +14 (004172C9)
    TextParams params;      // +18
};
static_assert(sizeof(TextObject) == sizeof(Text), "text view");

// CSWGuiControlEvent array element (lane-input ControlEvent: object, PMF, id; 12 bytes).
using ControlEvent = kih::ControlEvent;
using EventFunc = void (kih::GuiObject::*)(kih::Control*);
static_assert(sizeof(ControlEvent) == 12 && sizeof(EventFunc) == 4, "single-inheritance PMF event");

// CSWGuiScene view (0x34 at CSWGui3DSceneView+5C / CSWGuiMainMenu+3BC; ctor 004174B0 lane-3d-render).
struct Scene3dView;
struct AurSceneVtbl { const void* slots[0x1C]; void (__fastcall* spawn_room)(void*, void*, const char*, const float*, const float*); };
struct AurModelVtbl { const void* slots[5];
    void (__fastcall* set_anim_scale)(void*, void*, float);                        // +14 (0067C8DE)
    void (__fastcall* play_animation)(void*, void*, const char*, float, Word, Word); // +18 (0067C8D2)
};
struct AurCameraVtbl { const void* slots[0x11];
    void (__fastcall* set_view_angle)(void*, void*, float);                        // +44 (0067C8F9)
    const void* slots48_70[0xB];
    void (__fastcall* attach_to_object)(void*, void*, void*, const char*, Word);   // +74 (0067C8ED)
};
static_assert(offsetof(AurSceneVtbl, spawn_room) == 0x70 && offsetof(AurModelVtbl, play_animation) == 0x18 &&
              offsetof(AurCameraVtbl, set_view_angle) == 0x44 && offsetof(AurCameraVtbl, attach_to_object) == 0x74,
              "3d slots");
struct GuiScene {           // 0x34 bytes
    unsigned char u00[0x14];
    void* aur_scene;        // +14 (menu+3D0 at 0067C82F)
    void* aur_camera;       // +18 (menu+3D4 at 0067C829)
    unsigned char u1c[0x18];
};
static_assert(sizeof(GuiScene) == 0x34, "gui scene");
}

extern "C" {
// ======================================================================= authored here
// ---- generic leaves shared by many slots (D24; original addresses cited per body)
void __fastcall gui_load_slot_ret8_005b5e90(gui_load::Object*, void*, gui_load::Word, gui_load::Word);
void __fastcall gui_load_slot_ret_004015a0(gui_load::Object*, void*);
void __fastcall gui_load_slot_ret4_0060e760(gui_load::Object*, void*, gui_load::Word);
gui_load::Word __fastcall gui_load_slot_zero_0063e7f0(gui_load::Object*, void*);
gui_load::Object* __fastcall gui_load_slot_self_00641db0(gui_load::Object*, void*);
gui_load::Word __fastcall gui_load_slot_zero8_00409eb0(gui_load::Object*, void*, gui_load::Word, gui_load::Word);
gui_load::Word __fastcall gui_load_slot_prop10_004064a0(gui_load::Object*, void*);
// ---- control family
void __fastcall gui_load_object_set_extent_00409e90(gui_load::Object*, void*, const gui_load::Extent*);
void* __fastcall gui_load_object_deleting_destruct_00409ec0(gui_load::Object*, void*, gui_load::Word);
void* __fastcall gui_load_drawable_deleting_destruct_00414f90(void*, void*, gui_load::Word);
gui_load::Control* __fastcall gui_load_control_construct_0041aa80(gui_load::Control*, void*) noexcept(false);
void __fastcall gui_load_control_destruct_00419a20(gui_load::Control*, void*);
void* __fastcall gui_load_control_deleting_destruct_00419cc0(gui_load::Control*, void*, gui_load::Word);
gui_load::Navigable* __fastcall gui_load_navigable_construct_0041b8b0(gui_load::Navigable*, void*) noexcept(false);
void __fastcall gui_load_navigable_destruct_00419ce0(gui_load::Navigable*, void*);
void* __fastcall gui_load_navigable_deleting_destruct_00419cf0(gui_load::Navigable*, void*, gui_load::Word);
gui_load::Label* __fastcall gui_load_label_construct_0041acd0(gui_load::Label*, void*) noexcept(false);
void __fastcall gui_load_label_destruct_00419d10(gui_load::Label*, void*);
void* __fastcall gui_load_label_deleting_destruct_00419d80(gui_load::Label*, void*, gui_load::Word);
gui_load::Button* __fastcall gui_load_button_construct_0041c0f0(gui_load::Button*, void*) noexcept(false);
void __fastcall gui_load_button_destruct_00419da0(gui_load::Button*, void*);
void* __fastcall gui_load_button_deleting_destruct_00419e20(gui_load::Button*, void*, gui_load::Word);
void __fastcall gui_load_scene_view_destruct_0067ac50(gui_load::Control*, void*);
void* __fastcall gui_load_scene_view_deleting_destruct_0067acc0(gui_load::Control*, void*, gui_load::Word);
void __fastcall gui_load_control_handle_rmouse_up_0041ac70(gui_load::Object*, void*);
void __fastcall gui_load_control_focus_change_00418960(gui_load::Object*, void*, gui_load::Word);
void __fastcall gui_load_control_rmouse_down_0041ac30(gui_load::Object*, void*);
void __fastcall gui_load_control_set_enabled_004176a0(gui_load::Object*, void*, gui_load::Word);
void __fastcall gui_load_label_set_extent_00417780(gui_load::Object*, void*, const gui_load::Extent*);
void __fastcall gui_load_button_set_extent_00417a20(gui_load::Object*, void*, const gui_load::Extent*);
void __fastcall gui_load_scene_view_set_extent_00418380(gui_load::Object*, void*, const gui_load::Extent*);
// ---- control events
kotor_reconstruction::ArrayMutations* __fastcall gui_load_event_list_construct_0041a870(
    kotor_reconstruction::ArrayMutations*, void*, gui_load::Word) noexcept(false);
void __fastcall gui_load_event_list_allocate_004186e0(kotor_reconstruction::ArrayMutations*, void*, gui_load::Word) noexcept(false);
void __fastcall gui_load_event_list_add_0041a8a0(kotor_reconstruction::ArrayMutations*, void*, gui_load::ControlEvent) noexcept(false);
void __fastcall gui_load_list12_delete_at_0048dde0(kotor_reconstruction::ArrayMutations*, void*, gui_load::I32);
void __fastcall gui_load_control_add_event_0041ab20(gui_load::Control*, void*, gui_load::I32 id,
    kotor_input_hover::GuiObject* object, gui_load::EventFunc func) noexcept(false);
// ---- border / text
gui_load::BorderParams* __fastcall gui_load_border_params_construct_00415000(gui_load::BorderParams*, void*,
    const gui_load::Ref*, const gui_load::Ref*, const gui_load::Ref*, gui_load::Word fill_style,
    gui_load::I32 dimension, float f0c, const gui_load::Vector* color, gui_load::I32 inner_offset) noexcept(false);
gui_load::BorderParams* __fastcall gui_load_border_params_assign_004150c0(gui_load::BorderParams*, void*,
    const gui_load::BorderParams*) noexcept(false);
void __fastcall gui_load_border_params_set_border_00414c90(gui_load::BorderParams*, void*, gui_load::Border*) noexcept(false);
gui_load::I32 __fastcall gui_load_border_params_dim_00414cd0(const gui_load::BorderParams*, void*);
void __fastcall gui_load_border_params_set_corner_00414ae0(void*, void*, const gui_load::Ref*, gui_load::I32) noexcept(false);
void __fastcall gui_load_border_params_set_edge_00414b70(void*, void*, const gui_load::Ref*, gui_load::I32) noexcept(false);
void __fastcall gui_load_border_params_set_fill_00414c00(void*, void*, const gui_load::Ref*, gui_load::I32) noexcept(false);
gui_load::Border* __fastcall gui_load_border_construct_004167f0(gui_load::Border*, void*) noexcept(false);
void __fastcall gui_load_border_destruct_004151c0(gui_load::Border*, void*);
void* __fastcall gui_load_border_deleting_destruct_004168a0(gui_load::Border*, void*, gui_load::Word);
void __fastcall gui_load_border_initialize_00415190(gui_load::Border*, void*, const gui_load::Extent*, const void*);
gui_load::Extent* __fastcall gui_load_border_inner_extent_00415240(gui_load::Border*, void*, gui_load::Extent*);
gui_load::Extent* __cdecl gui_load_extent_intersection_0040a4a0(gui_load::Extent*, const gui_load::Extent*, const gui_load::Extent*);
gui_load::TextParams* __fastcall gui_load_text_params_construct_00417050(gui_load::TextParams*, void*) noexcept(false);
void __fastcall gui_load_text_params_set_base_font_00415dd0(void*, void*, const gui_load::Ref*) noexcept(false);
void __fastcall gui_load_text_params_set_font_rename_00415d60(gui_load::TextParams*, void*) noexcept(false);
void __fastcall gui_load_manager_updated_font_name_0040b360(void* manager, void*, char* name);
void __fastcall gui_load_text_params_set_font_00415d00(gui_load::TextParams*, void*, const gui_load::Ref*);
void __fastcall gui_load_text_params_set_text_00415e00(void*, void*, const gui_load::String*) noexcept(false);
void __fastcall gui_load_text_wrap_00414e90(gui_load::TextObject*, void*);
void __fastcall gui_load_text_params_set_alignment_00415f20(void*, void*, std::int32_t);
void __fastcall gui_load_text_params_set_strref_00415e50(void*, void*, gui_load::Word) noexcept(false);
void __fastcall gui_load_text_params_set_default_color_00414e50(void*, void*, const gui_load::Vector*);
void __fastcall gui_load_text_params_set_text_object_00415f70(void*, void*, gui_load::Text*) noexcept(false);
gui_load::Text* __fastcall gui_load_text_construct_00417280(gui_load::Text*, void*) noexcept(false);
void __fastcall gui_load_text_destruct_00415ff0(gui_load::Text*, void*);
void* __fastcall gui_load_text_deleting_destruct_004172f0(gui_load::Text*, void*, gui_load::Word);
void __fastcall gui_load_text_set_extent_00416280(gui_load::Text*, void*, const gui_load::Extent*);
// ---- panel
gui_load::Panel* __fastcall gui_load_panel_construct_0040b570(gui_load::Panel*, void*, gui_load::Object* manager) noexcept(false);
void __fastcall gui_load_panel_destruct_0040cf70(gui_load::Panel*, void*);
void* __fastcall gui_load_panel_deleting_destruct_0040d010(gui_load::Panel*, void*, gui_load::Word);
void __fastcall gui_load_panel_set_extent_0040a5a0(gui_load::Object*, void*, const gui_load::Extent*);
void __fastcall gui_load_panel_set_active_control_0040a630(gui_load::Object*, void*, gui_load::Object*, gui_load::Word);
void __fastcall gui_load_panel_handle_input_00409e60(gui_load::Object*, void*, gui_load::I32, gui_load::I32);
gui_load::Object* __fastcall gui_load_panel_active_control_0040b630(gui_load::Object*, void*);
void __fastcall gui_load_panel_on_added_0040b870(gui_load::Panel*, void*);
void __fastcall gui_load_panel_button_a_0040b640(gui_load::Object*, void*, gui_load::Word);
void __fastcall gui_load_panel_button_b_0040b650(gui_load::Object*, void*, gui_load::Word);
void __fastcall gui_load_panel_button_x_0040b660(gui_load::Object*, void*, gui_load::Word);
void __fastcall gui_load_panel_button_y_0040b670(gui_load::Object*, void*, gui_load::Word);
void __fastcall gui_load_panel_button_black_0040b680(gui_load::Object*, void*, gui_load::Word);
gui_load::String* __fastcall gui_load_panel_full_screen_bg_0040a900(gui_load::Panel*, void*, gui_load::String*) noexcept(false);
gui_load::String* __cdecl gui_load_manager_resolution_string_0040a3e0(gui_load::String*, gui_load::I32, gui_load::I32) noexcept(false);
void __fastcall gui_load_panel_reset_font_0040a9c0(gui_load::Panel*, void*);
void __fastcall gui_load_panel_local_mouse_0040ba20(gui_load::Object*, void*, gui_load::I32*, gui_load::I32*);
void __fastcall gui_load_panel_offset_extent_0040aa00(gui_load::Panel*, void*, gui_load::Extent*);  // EAX not meaningful (0040AA56)
// ---- method tables (typed constant structs; gui_vtables.cpp)
extern const struct GuiLoadObjectMethods gui_load_object_methods_0073df20;
extern const struct GuiLoadDrawableMethods gui_load_drawable_methods_0073e324;
extern const struct GuiLoadBorderMethods gui_load_border_methods_0073e338;
extern const struct GuiLoadTextMethods gui_load_text_methods_0073e3a4;
extern const struct GuiLoadControlMethods gui_load_control_methods_0073e488;
extern const struct GuiLoadNavigableMethods gui_load_navigable_methods_0073e520;
extern const struct GuiLoadLabelMethods gui_load_label_methods_0073e5b8;
extern const struct GuiLoadButtonMethods gui_load_button_methods_0073e658;
extern const struct GuiLoadSceneViewMethods gui_load_scene_view_methods_00752e30;
extern const struct GuiLoadPanelMethods gui_load_panel_methods_0073e010;
extern const struct GuiLoadMainMenuMethods gui_load_mainmenu_methods_00752f70;

// ======================================================================= external dependencies
// lane-3d-render
kotor_reconstruction::ArrayMutations* __fastcall exo_array_list_construct_00416770(
    kotor_reconstruction::ArrayMutations*, void*, gui_load::Word) noexcept(false);
void __fastcall exo_array_list_destruct_0056b6f0(kotor_reconstruction::ArrayMutations*, void*);
// lane-m42-menu-joins (D9) / lane-tpc
menu_resources::Ref* __fastcall c2da_default_ref_00405ed0(menu_resources::Ref*, void*);
menu_resources::Ref* __fastcall c2da_copy_00406120(menu_resources::Ref*, void*, menu_resources::Ref*,
                                                   menu_resources::Ref*);
// ListBox fork of this lane (listbox_image.cpp)
gui_load::Word __fastcall gui_load_resref_not_equal_00406dc0(const menu_resources::Ref*, void*, const menu_resources::Ref*);
gui_load::Word __fastcall gui_load_resref_is_valid_00406030(const menu_resources::Ref*, void*);
// M40 strings
gui_load::String* __fastcall WindowCopy005e5b00(gui_load::String*, void*, const gui_load::String* volatile);
char* __fastcall WindowCStr005e5670(gui_load::String*, void*);
// M40 allocator family of 006FA7E6 / 006FA390
void* __cdecl c2da_directory_new_006fa7e6(std::uint32_t) noexcept(false);
// MISSING (lane-gui-render / Aurora GUI objects; proposed names)
gui_load::AurString* __cdecl gui_load_new_aur_gui_string_0045bdf0(const char*, const char*, const void*, float) noexcept(false);
// MISSING (lane-tlk, D23): CTlkTable::Fetch 0041E550 RET 0Ch and its cell 007A3A08.
gui_load::Word __fastcall tlk_fetch_0041e550(void* tlk, void*, gui_load::Word strref, void* str_res, gui_load::Word flag) noexcept(false);
extern void* kotor_cell_tlk_007a3a08;                       // lane-tlk tlk_cells.obj (D23)
// MISSING (3D lanes): CSWGuiScene::~CSWGuiScene 004165E0 (name in lane-3d-render style).
void __fastcall gui_scene_destruct_004165e0(void*, void*);
// lane-r4-lowers (r4_gui_lowers.cpp): PanelExists 0040BD70, RemovePanel 0040C830.
unsigned __fastcall frame_ContainsPanel_0040bd70(void* gui, void*, void* panel);
void __fastcall frame_RemovePanel_0040c830(void* gui, void*, void* panel);
void __fastcall gui_load_border_load_004153e0(gui_load::Border*, void*, gui_load::Gff*, gui_load::Struct*, gui_load::String*);  // gui_border_text_load.cpp
void __fastcall gui_load_text_load_00416050(gui_load::Text*, void*, gui_load::Gff*, gui_load::Struct*, gui_load::String*);      // gui_border_text_load.cpp
gui_load::Word __fastcall gui_load_text_ideal_height_00414eb0(gui_load::Text*, void*);   // authored (gui_objects.cpp)
void __fastcall gui_3d_scene_view_draw_0067acb0(void*, void*, std::uint32_t);           // lane-3d-render
int __cdecl _purecall(void);                                                            // CRT (original 006FB8DE)
// MISSING slot bodies (proposed names; lane-gui-render draws, input/tooltip paths, not first-frame unless noted)
void __fastcall kotor_fastcall_004168c0(gui_load::Border*, void*, gui_load::Word);     // lane-gui-render
void __fastcall kotor_fastcall_00417750(gui_load::Object*, void*, gui_load::Word);      // lane-gui-render
void __fastcall kotor_fastcall_00417ab0(gui_load::Object*, void*, gui_load::Word);     // lane-gui-render
void __fastcall gui_load_label_set_enabled_00418d00(gui_load::Object*, void*, gui_load::Word);
void __fastcall gui_load_label_reset_font_00418d40(gui_load::Object*, void*);
void __fastcall gui_load_label_initialize_00418cd0(gui_load::Object*, void*, gui_load::Word, gui_load::Word, gui_load::Word);
void __fastcall gui_load_label_initialize_004176e0(gui_load::Object*, void*, gui_load::Word, gui_load::Word, gui_load::Word, gui_load::Word);
void __fastcall gui_load_button_set_enabled_00418db0(gui_load::Object*, void*, gui_load::Word);
void __fastcall gui_load_button_reset_font_00418df0(gui_load::Object*, void*);
void __fastcall gui_load_button_initialize_00418d80(gui_load::Object*, void*, gui_load::Word, gui_load::Word);
void __fastcall gui_load_button_initialize_00417970(gui_load::Object*, void*, gui_load::Word, gui_load::Word, gui_load::Word, gui_load::Word);
gui_load::Object* __fastcall gui_load_panel_hit_check_0040b690(gui_load::Object*, void*, gui_load::I32, gui_load::I32);
void __fastcall gui_load_panel_on_removed_0040c170(gui_load::Panel*, void*);
void __fastcall gui_load_mainmenu_handle_input_0067b380(gui_load::Object*, void*, gui_load::I32, gui_load::I32);
void __fastcall gui_load_mainmenu_on_added_0067b6c0(gui_load::Panel*, void*) noexcept(false);                // on AddPanel path (gate)
// Not authored (control tooltip display; input/2DA/TLK path, not first-frame).
gui_load::Word __fastcall gui_load_control_tooltip_00418a90(gui_load::Object*, void*);
}
// lane-renderer-bodies sole owner of the 0078D1D4/0078D1D8 window cells (renderer_gl_cells.h / window_shared_cells.h).
namespace kotor_renderer_gl_cells { extern std::uint32_t width_0078d1d4; }
// lane-input (D22) owner of 007A39F4.
namespace kotor_input_cells { extern void* volatile gui_manager_007a39f4; }
// MISSING storage owner: CExoString[] at 007A3D50 (full-screen BG suffixes, indexed by panel+48 at
// 0040A941); also consumed by the M40 GUI-manager ctor bindings (gui_ctor.h suffix_007a3d50).
extern "C" unsigned char kotor_cells_gui_suffix_007a3d50[40];   // rev2 owner gui_suffix_names.cpp (0073AD80 initializer)
// M40 allocator free (live-policy.obj; family of 006FA390)
extern "C" void __cdecl live_free(void*);
