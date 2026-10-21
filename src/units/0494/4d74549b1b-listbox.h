#pragma once
// Lane gui-load / ListBox fork: CSWGuiListBox (LB_MODULES at CSWGuiMainMenu+0x64),
// its CSWGuiScrollBar (+0x104), CSWGuiImage subobjects, and the Load path
// CSWGuiListBox::Load 0041D5B0.
//
// Conventions (DECISIONS D1/D21/D24): every original __thiscall entry is a
// __fastcall(Self*, void* unused_edx, ...) veneer with identical stack layout and
// callee cleanup. Vtables are typed constant structs whose fields are either
// __fastcall pointers (this lane / main lane-gui-load / sibling lanes) or
// single-inheritance PMFs (lane-input kotor_input_hover members); the object's
// vptr word receives the table address through an object-pointer conversion
// (never a function-pointer cast). Virtual calls reload the receiver's vptr at
// each call site (original `mov reg,[obj]; call [reg+slot]`).
//
// Layouts: byte-offset views built on the main lane's gui_load::Control /
// Navigable / Border / Text; every added field cites the instruction that
// establishes it. Unknown bytes stay opaque.
#include "27c7999c0c-gui_control_load.h"
#include "e2684e9878-hover_path.h"       // lane-input: kotor_input_hover PMF slot owners
#include "7b8c81fcff-allocator_live.h"   // M40: live_free (allocation family of 006FA7E6 -> live_operator_new)
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace gui_lb {
using gui_load::Word;
using gui_load::Gff;
using gui_load::Struct;
using gui_load::String;
using gui_load::ResRef;
using gui_load::Vector;
using gui_load::Extent;
using gui_load::Object;
using gui_load::Control;
using gui_load::Navigable;
using gui_load::Border;
using gui_load::Text;
using I32 = std::int32_t;
using I16 = std::int16_t;
using Array = kotor_reconstruction::ArrayMutations;

// ---------------------------------------------------------------- CAurGUIImage / CAurGUIString views
// Only the slots observed on this path are typed (receiver = the Aurora object).
struct AurObject;
struct AurObjectVtbl {
    void* slot00;
    void (__fastcall* release)(AurObject*, void*, Word);        // +04 called with 1 (00415941, 00415B06)
    void* slot08;
    I32 (__fastcall* height)(AurObject*, void*);                 // +0C (00415B51 -> extent+C, 00415BBB -> +20)
    I32 (__fastcall* width)(AurObject*, void*);                  // +10 (00415B47 -> extent+8, 00415BC6 -> +1C)
    void* slots14_44[13];
    I32 (__fastcall* font_height)(AurObject*, void*);            // +48 CAurGUIString (00414EEC/00414EF9)
};
static_assert(offsetof(AurObjectVtbl, font_height) == 0x48, "aur slots");
struct AurObject { const AurObjectVtbl* vtbl; };

// ---------------------------------------------------------------- CSWGuiImageParams (0x2C) / CSWGuiImage (0x54)
struct Image;
struct ImageParams {
    ResRef image;              // +00 (00414D37 default ctor, 00414D53 assign)
    Word u10;                  // +10 ctor arg4 (00414D64); copied via FLD/FSTP (00416E18)
    Word u14;                  // +14 ctor arg6 (00414D6E); copied via FLD/FSTP (00416E2F)
    Vector color;              // +18 ctor arg7 copy (00414D98..A9)
    Word flags;                // +24 bit1 drawstyle, bits2..5 flip, bits6..11 alignment (00414D75..95)
    Image* owner;              // +28 (00414DA6 = 0; 004159F9 SetImageObject)
};
struct ImageVtbl;
struct Image {
    const void* vptr;          // +00 73E390 (00415A6C); 73E324 after dtor body (00415B14)
    Extent extent;             // +04 (00415B79..93)
    Extent draw;               // +14 (00415BDA/00415C6C), +18 (00415C24), +1C (00415BD0), +20 (00415BC1)
    AurObject* gui_image;      // +24 (00415AB3 = 0; 0041596F NewCAurGUIImage)
    ImageParams params;        // +28
    // params.flags = image+4C (00415BA1); params.owner = image+50
};
static_assert(sizeof(ImageParams) == 0x2C && sizeof(Image) == 0x54 && offsetof(Image, params) == 0x28, "image");

// ---------------------------------------------------------------- CSWGuiScrollBar (0x188)
struct ScrollBar : Control {
    I32 max_value;             // +5C (0041B03E; 00418080)
    I32 cur_value;             // +60 (0041B041; 004180B8)
    I32 visible_value;         // +64 (0041B044; 004180E6)
    Word sb_flags;             // +68 bit0 DRAWMODE (0041BE1E), bit1 setMaxValue arg2 (0041808C)
    Border border;             // +6C (0041B012)
    Image thumb;               // +E0 (0041B022; "THUMB" 0041BD82)
    Image dir;                 // +134 (0041B032; "DIR" 0041BD45)
};
static_assert(sizeof(ScrollBar) == 0x188 && offsetof(ScrollBar, border) == 0x6C &&
              offsetof(ScrollBar, thumb) == 0xE0 && offsetof(ScrollBar, dir) == 0x134, "scrollbar");

// ---------------------------------------------------------------- CSWGuiListBox (0x2E0)
struct ListBox : Navigable {
    I32 u6c;                   // +6C = -1 (0041BEF8)
    unsigned char u70[0x0C];
    Word u7c;                  // +7C = 0 (0041BF2B)
    I32 local_x;               // +80 (0041A662)
    I32 local_y;               // +84 (0041A668)
    Word u88;                  // +88 = 0 (0041BF1F)
    Word u8c;                  // +8C = 0 (0041BF25)
    Border border;             // +90 (0041BE75)
    ScrollBar scroll;          // +104 (0041BE85)
    Extent inner;              // +28C..+298 zeroed (0041BE96..A8); GetInnerExtent copy (0041BFF9..C017)
    Array items;               // +29C CExoArrayList<CSWGuiControl*> (0041BEAE)
    Array extents;             // +2A8 CExoArrayList<CSWGuiExtent> (0041BEBF; 16-byte entries)
    I32 item_height;           // +2B4 (0041B1D6..0D)
    float scale;               // +2B8 = 1.0 (0041BF3D)
    Word lb_flags;             // +2BC (x & 0xFFFFF8BA) | 0x183A (0041BEDF..F2)
    unsigned char padding;     // +2C0 = 0 (0041BEFB); PADDING (0041C1BA)
    unsigned char u2c1;        // +2C1 = 0xFF (0041BEEC)
    I16 u2c2;                  // +2C2 = 1 (0041BF34)
    I16 visible_count;         // +2C4 (0041B277)
    I16 selected;              // +2C6 = -1 (0041BEE5)
    I16 first;                 // +2C8 = 0 (0041BF11)
    I16 last;                  // +2CA = 0 (0041BF18)
    Control* proto;            // +2CC = 0 (0041BF01); LoadProtoItem (0041D53B)
    Vector color;              // +2D0..+2D8 zeroed (0041BEC4..D0); COLOR (0041D6BB..CE)
    Control* u2dc;             // +2DC = 0 (0041BF2E)
};
static_assert(offsetof(ListBox, border) == 0x90 && offsetof(ListBox, scroll) == 0x104 &&
              offsetof(ListBox, inner) == 0x28C && offsetof(ListBox, items) == 0x29C &&
              offsetof(ListBox, extents) == 0x2A8 && offsetof(ListBox, item_height) == 0x2B4 &&
              offsetof(ListBox, lb_flags) == 0x2BC && offsetof(ListBox, padding) == 0x2C0 &&
              offsetof(ListBox, visible_count) == 0x2C4 && offsetof(ListBox, proto) == 0x2CC &&
              offsetof(ListBox, color) == 0x2D0 && sizeof(ListBox) == 0x2E0, "listbox");

// ---------------------------------------------------------------- control-family vtable call view
// Generic receiver view for virtual calls on list items / self (slot meaning per
// cited call sites; tables 73E488/73E520/73E5B8/73E658/73E6F8/73E840).
struct ControlCallVtbl {
    void (__fastcall* deleting_destroy)(Control*, void*, Word);          // +00 (0041B099, 00419AE1)
    void (__fastcall* set_extent)(Control*, void*, const Extent*);       // +04
    void* slots08_28[9];
    Control* (__fastcall* as_control)(Control*, void*);                  // +2C (0041A6D0)
    void* slot30;
    I32 (__fastcall* property10)(Control*, void*);                       // +34 (0041B202, 00419F34)
    void* slot38;
    void (__fastcall* handle_input_event)(Control*, void*, I32, I32);    // +3C (0041C062)
    void* slot40;
    Control* (__fastcall* hit_check_mouse)(Control*, void*, I32, I32);   // +44 (0041A68D)
    void* slot48; void* slot4c;
    gui_load::Label* (__fastcall* as_label)(Control*, void*);            // +50 (0041B35E)
    void* slots54_68[6];
    I32 (__fastcall* slot6c)(Control*, void*);                           // +6C (00419AAD)
    void* slots70_7c[4];
    I32 (__fastcall* is_selectable)(Control*, void*);                    // +80 (0041A6E8)
    void* slots84_88[2];
    void (__fastcall* reset_font)(Control*, void*);                      // +8C (0041A7CB)
    Word (__fastcall* display_tooltip)(Control*, void*);                 // +90 (0041A80F)
    void* slot94; void* slot98;
    void (__fastcall* initialize9c)(Control*, void*, const Extent*, const void*, menu_resources::Ref*,
                                    menu_resources::Ref*);                // +9C (004197B0, 00419C4B scrollbar)
};
static_assert(offsetof(ControlCallVtbl, property10) == 0x34 && offsetof(ControlCallVtbl, as_label) == 0x50 &&
              offsetof(ControlCallVtbl, slot6c) == 0x6C && offsetof(ControlCallVtbl, reset_font) == 0x8C &&
              offsetof(ControlCallVtbl, initialize9c) == 0x9C, "control call view");
// ListBox-family continuation (73E840 +98/+9C): Initialize overloads.
struct ListBox;
struct ListBoxCallVtbl {
    void* prefix[0x26];
    void (__fastcall* initialize98)(ListBox*, void*, const Extent*, ListBox*);          // +98
    void (__fastcall* initialize9c)(ListBox*, void*, const Extent*, const void*, Word); // +9C (00419BA0)
};
static_assert(offsetof(ListBoxCallVtbl, initialize9c) == 0x9C, "listbox call view");
struct ScrollBar;
struct ScrollBarCallVtbl {
    void* prefix[0x26];
    void (__fastcall* initialize98)(ScrollBar*, void*, const Extent*, ScrollBar*);      // +98 (00419BC5)
    void (__fastcall* initialize9c)(ScrollBar*, void*, const Extent*, const void*,
                                    menu_resources::Ref*, menu_resources::Ref*);       // +9C (00419C4B)
};
static_assert(offsetof(ScrollBarCallVtbl, initialize9c) == 0x9C, "scrollbar call view");
struct ImageCallVtbl {
    void* slot00;
    void (__fastcall* set_extent)(Image*, void*, const Extent*);         // +04 (004159B6, 004194A1)
    void* slot08; void* slot0c;
    void (__fastcall* load)(Image*, void*, Gff*, Struct*, String*);      // +10 (0041BD55)
};
struct BorderCallVtbl {
    void* slot00;
    void (__fastcall* set_extent)(Border*, void*, const Extent*);        // +04 (0041BFEA, 00419544)
    void* slot08; void* slot0c;
    void (__fastcall* load)(Border*, void*, Gff*, Struct*, String*);     // +10 (0041D5FE, 0041BD18)
};

template<class V, class T> inline const V* vt(const T* object) {
    const void* table;
    std::memcpy(&table, object, sizeof(table));
    return static_cast<const V*>(table);
}
template<class T> inline void set_vptr(T* object, const void* table) {
    std::memcpy(object, &table, sizeof(table));
}
}

// ======================================================================== external names
extern "C" {
// ---- main lane-gui-load (requested names; see LISTBOX-PART.json "requests")
gui_load::Control* __fastcall gui_load_control_construct_0041aa80(gui_load::Control*, void*) noexcept(false);
void __fastcall gui_load_control_destruct_00419a20(gui_load::Control*, void*);
gui_load::Navigable* __fastcall gui_load_navigable_construct_0041b8b0(gui_load::Navigable*, void*) noexcept(false);
void __fastcall gui_load_navigable_destruct_00419ce0(gui_load::Navigable*, void*);
gui_load::Border* __fastcall gui_load_border_construct_004167f0(gui_load::Border*, void*) noexcept(false);
void __fastcall gui_load_border_destruct_004151c0(gui_load::Border*, void*);
void __fastcall gui_load_border_initialize_00415190(gui_load::Border*, void*, const gui_load::Extent*, const void*);
gui_load::Label* __fastcall gui_load_label_construct_0041acd0(gui_load::Label*, void*) noexcept(false);
gui_load::Button* __fastcall gui_load_button_construct_0041c0f0(gui_load::Button*, void*) noexcept(false);
void __fastcall gui_load_panel_local_mouse_0040ba20(gui_load::Object*, void*, gui_lb::I32*, gui_lb::I32*);
// Shared control-family trivial slot bodies (main lane, D24).
void __fastcall gui_load_slot_ret8_005b5e90(gui_load::Object*, void*, gui_load::Word, gui_load::Word);
void __fastcall gui_load_slot_ret_004015a0(gui_load::Object*, void*);
void __fastcall gui_load_slot_ret4_0060e760(gui_load::Object*, void*, gui_load::Word);
gui_load::Word __fastcall gui_load_slot_zero_0063e7f0(gui_load::Object*, void*);
gui_load::Object* __fastcall gui_load_slot_self_00641db0(gui_load::Object*, void*);
gui_load::Word __fastcall gui_load_slot_zero8_00409eb0(gui_load::Object*, void*, gui_load::Word, gui_load::Word);
gui_load::Word __fastcall gui_load_slot_prop10_004064a0(gui_load::Object*, void*);
void __fastcall gui_load_control_handle_rmouse_up_0041ac70(gui_load::Object*, void*);
void __fastcall gui_load_control_focus_change_00418960(gui_load::Object*, void*, gui_load::Word);
void __fastcall gui_load_control_rmouse_down_0041ac30(gui_load::Object*, void*);
void __fastcall gui_load_control_set_enabled_004176a0(gui_load::Object*, void*, gui_load::Word);
gui_load::Word __fastcall gui_load_control_tooltip_00418a90(gui_load::Object*, void*);

// ---- sibling lanes
kotor_reconstruction::ArrayMutations* __fastcall exo_array_list_construct_00416770(
    kotor_reconstruction::ArrayMutations*, void*, gui_load::Word) noexcept(false);                 // lane-3d-render
void __fastcall exo_array_list_destruct_0056b6f0(kotor_reconstruction::ArrayMutations*, void*);  // lane-3d-render
void __fastcall gui_drawable_destruct_00414f80(void*, void*);                      // lane-3d-render
menu_resources::Ref* __fastcall c2da_default_ref_00405ed0(menu_resources::Ref*, void*);         // lane-m42-menu-joins
menu_resources::Ref* __fastcall c2da_copy_00406120(menu_resources::Ref*, void*, menu_resources::Ref*,
                                                   menu_resources::Ref*);           // lane-tpc
void __cdecl frame_RenderBoundary_0044ed90();                                      // lane-routes (AurMakeCurrent)
void* __cdecl c2da_directory_new_006fa7e6(std::uint32_t) noexcept(false);           // M40 operator new

// ---- MISSING (proposed names; not on the first-frame path)
gui_lb::AurObject* __cdecl kotor_cdecl_0045a260(const char*) noexcept(false);        // NewCAurGUIImage
gui_load::Control* __fastcall gui_load_label_hilight_construct_0041b5c0(void*, void*) noexcept(false);
gui_load::Control* __fastcall gui_load_button_toggle_construct_0041ca30(void*, void*) noexcept(false);
gui_load::Control* __fastcall gui_load_slider_construct_0041baa0(void*, void*) noexcept(false);
gui_load::Word __fastcall gui_load_listbox_captured_movement_0041b670(gui_lb::ListBox*, void*, gui_load::Word, gui_load::Word);
void __fastcall gui_load_listbox_lmouse_down_0041c4a0(gui_lb::ListBox*, void*);
void __fastcall gui_load_listbox_lmouse_up_0041a700(gui_lb::ListBox*, void*);
void __fastcall gui_load_listbox_draw_0041a3e0(gui_lb::ListBox*, void*, gui_load::Word);
void __fastcall gui_load_listbox_handle_input_0041ce20(gui_lb::ListBox*, void*, gui_lb::I32, gui_lb::I32);
void __fastcall gui_load_scrollbar_draw_00419660(gui_lb::ScrollBar*, void*, gui_load::Word);
void __fastcall gui_load_image_draw_00415c90(gui_lb::Image*, void*, gui_load::Word);

// ---- authored here
gui_load::Word __fastcall gui_load_resref_not_equal_00406dc0(const menu_resources::Ref*, void*, const menu_resources::Ref*);
gui_load::Word __fastcall gui_load_resref_is_valid_00406030(const menu_resources::Ref*, void*);
gui_lb::ImageParams* __fastcall gui_load_image_params_construct_00414d30(gui_lb::ImageParams*, void*,
    menu_resources::Ref*, gui_load::Word, gui_load::Word, gui_load::Word, gui_load::Word, gui_load::Word,
    const gui_load::Vector*) noexcept(false);
void __fastcall gui_load_image_params_set_image_00415900(gui_lb::ImageParams*, void*, menu_resources::Ref*, gui_load::Word) noexcept(false);
void __fastcall gui_load_image_params_set_alignment_00415990(gui_lb::ImageParams*, void*, gui_load::Word);
void __fastcall gui_load_image_params_set_draw_style_004159c0(gui_lb::ImageParams*, void*, gui_load::Word);
void __fastcall gui_load_image_params_set_image_object_004159f0(gui_lb::ImageParams*, void*, gui_lb::Image*) noexcept(false);
gui_lb::ImageParams* __fastcall gui_load_image_params_assign_00416de0(gui_lb::ImageParams*, void*, gui_lb::ImageParams*) noexcept(false);
gui_lb::Image* __fastcall gui_load_image_construct_00415a30(gui_lb::Image*, void*) noexcept(false);
void __fastcall gui_load_image_destruct_00415ad0(gui_lb::Image*, void*);
void* __fastcall gui_load_image_deleting_destruct_00416e70(gui_lb::Image*, void*, gui_load::Word);
gui_load::Extent* __fastcall gui_load_image_get_extent_00415b30(gui_lb::Image*, void*, gui_load::Extent*);
void __fastcall gui_load_image_set_extent_00415b70(gui_lb::Image*, void*, const gui_load::Extent*);
void __fastcall gui_load_image_initialize_00416e90(gui_lb::Image*, void*, const gui_load::Extent*, gui_lb::ImageParams*) noexcept(false);
void __fastcall gui_load_image_load_00416ec0(gui_lb::Image*, void*, gui_load::Gff*, gui_load::Struct*, gui_load::String*) noexcept(false);
extern const struct GuiLoadImageMethods gui_load_image_methods_0073e390;

// main lane-gui-load owns GetBorderDim 00414CD0 / Border::GetInnerExtent 00415240.
gui_load::Extent* __fastcall gui_load_border_inner_extent_00415240(gui_load::Border*, void*, gui_load::Extent*);

gui_lb::ScrollBar* __fastcall gui_load_scrollbar_construct_0041afe0(gui_lb::ScrollBar*, void*) noexcept(false);
void __fastcall gui_load_scrollbar_destruct_00419e40(gui_lb::ScrollBar*, void*);
void* __fastcall gui_load_scrollbar_deleting_destruct_00419ec0(gui_lb::ScrollBar*, void*, gui_load::Word);
void __fastcall gui_load_scrollbar_set_max_00418070(gui_lb::ScrollBar*, void*, gui_lb::I32, gui_load::Word);
void __fastcall gui_load_scrollbar_set_cur_004180a0(gui_lb::ScrollBar*, void*, gui_lb::I32);
void __fastcall gui_load_scrollbar_set_visible_004180d0(gui_lb::ScrollBar*, void*, gui_lb::I32);
void __fastcall gui_load_scrollbar_set_extent_004193f0(gui_lb::ScrollBar*, void*, const gui_load::Extent*);
void __fastcall gui_load_scrollbar_load_0041bcd0(gui_lb::ScrollBar*, void*, gui_load::Gff*, gui_load::Struct*) noexcept(false);
void __fastcall gui_load_scrollbar_initialize_00419790(gui_lb::ScrollBar*, void*, const gui_load::Extent*, gui_lb::ScrollBar*) noexcept(false);
void __fastcall gui_load_scrollbar_initialize_00418100(gui_lb::ScrollBar*, void*, const gui_load::Extent*,
    const void* border_params, menu_resources::Ref* thumb, menu_resources::Ref* dir) noexcept(false);
extern const struct GuiLoadScrollBarMethods gui_load_scrollbar_methods_0073e6f8;

kotor_reconstruction::ArrayMutations* __fastcall gui_load_extent_list_construct_0041b830(
    kotor_reconstruction::ArrayMutations*, void*, gui_load::Word) noexcept(false);
void __fastcall gui_load_extent_list_allocate_0041a8f0(kotor_reconstruction::ArrayMutations*, void*, gui_load::Word) noexcept(false);
gui_lb::I32 __fastcall gui_load_int_list_index_of_006c2a40(kotor_reconstruction::ArrayMutations*, void*, gui_load::Word);
gui_lb::I32 __fastcall gui_load_text_font_height_00414ee0(gui_load::Text*, void*);

gui_lb::ListBox* __fastcall gui_load_listbox_construct_0041be40(gui_lb::ListBox*, void*) noexcept(false);
void __fastcall gui_load_listbox_destruct_0041b060(gui_lb::ListBox*, void*);
void* __fastcall gui_load_listbox_deleting_destruct_0041bf60(gui_lb::ListBox*, void*, gui_load::Word);
void __fastcall gui_load_listbox_clear_items_00419a90(gui_lb::ListBox*, void*);
void __fastcall gui_load_listbox_load_0041d5b0(gui_lb::ListBox*, void*, gui_load::Gff*, gui_load::Struct*) noexcept(false);
void __fastcall gui_load_listbox_load_proto_0041d3e0(gui_lb::ListBox*, void*, gui_load::Gff*, gui_load::Struct*) noexcept(false);
void __fastcall gui_load_listbox_scrollbar_left_00418270(gui_lb::ListBox*, void*, gui_load::Word);
void __fastcall gui_load_listbox_scrollbar_extent_004181f0(gui_lb::ListBox*, void*, gui_lb::I32, gui_load::Word);
void __fastcall gui_load_listbox_set_padding_0041c190(gui_lb::ListBox*, void*, gui_load::Word);
void __fastcall gui_load_listbox_organize_0041b140(gui_lb::ListBox*, void*);
void __fastcall gui_load_listbox_organize_unequal_00419ee0(gui_lb::ListBox*, void*);
void __fastcall gui_load_listbox_organize_oversized_0041a2d0(gui_lb::ListBox*, void*);
void __fastcall gui_load_listbox_set_extent_0041bf80(gui_lb::ListBox*, void*, const gui_load::Extent*);
void __fastcall gui_load_listbox_set_selected_0041c040(gui_lb::ListBox*, void*, gui_lb::I32, gui_load::Word);
void __fastcall gui_load_listbox_set_active_control_0041c160(gui_lb::ListBox*, void*, gui_load::Control*, gui_load::Word);
void __fastcall gui_load_listbox_set_active_control_0041c3a0(gui_lb::ListBox*, void*, gui_load::Control*, gui_load::Word);
gui_load::Control* __fastcall gui_load_listbox_hit_local_0041a5e0(gui_lb::ListBox*, void*, gui_lb::I32*);
gui_load::Word __fastcall gui_load_listbox_is_selectable_0041a6c0(gui_lb::ListBox*, void*);
void __fastcall gui_load_listbox_reset_font_0041a7b0(gui_lb::ListBox*, void*);
gui_load::Word __fastcall gui_load_listbox_tooltip_0041a800(gui_lb::ListBox*, void*);
void __fastcall gui_load_listbox_set_active_0041c3b0(gui_lb::ListBox*, void*, gui_load::Word);
void __fastcall gui_load_listbox_initialize_00419b80(gui_lb::ListBox*, void*, const gui_load::Extent*, gui_lb::ListBox*) noexcept(false);
void __fastcall gui_load_listbox_initialize_0041cd60(gui_lb::ListBox*, void*, const gui_load::Extent*, const void*, gui_load::Word) noexcept(false);
void __fastcall gui_load_listbox_select_left_00419bd0(gui_lb::ListBox*, void*, gui_load::Extent*, gui_load::Word,
    gui_load::Word, gui_load::Word, gui_load::Word);
void __fastcall gui_load_listbox_select_left_0041b120(gui_lb::ListBox*, void*, gui_load::Word);
extern const struct GuiLoadListBoxMethods gui_load_listbox_methods_0073e840;
}
