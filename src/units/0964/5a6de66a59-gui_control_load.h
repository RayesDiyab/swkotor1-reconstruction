#pragma once
// Lane gui-load: CSWGuiPanel / CSWGuiControl layout-load path reached from
// CClientExoAppInternal::DisplayMainMenu 005FCA30 -> CSWGuiMainMenu::CSWGuiMainMenu
// 0067C4C0 -> CSWGuiMainMenu::LoadFromLayout 0067ACE0.
// Object views: only offsets cited below are modeled; other bytes are opaque.
// Vtable views: only slots called on this path are typed; slot meaning is per
// cited vtable (73E488 control, 73E5B8 label, 73E658 button, 73E840 listbox,
// 752E30 3D-scene view, 73E010/752F70 panel, 73E338 border, 73E3A4 text).
#include "3e7771d18a-gff_readers.h"
#include "21e45d1112-growth.h"          // M40 menu_list_growth::allocate_005e03c0 + ArrayMutations
#include <cstddef>
#include <cstdint>

namespace gui_load {
using Array = kotor_reconstruction::ArrayMutations;
using List = task17_gff::List;
struct Extent { std::int32_t left, top, width, height; };   // CSWGuiExtent, 00409DC0 writes +0/+4/+8/+C
struct Object; struct Panel; struct Navigable; struct Border; struct Text;

// CSWGuiObject-family vtable *view* (D1: slots typed __fastcall(Self*, void* edx, ...),
// identical to the original thiscall ABI). Panel and control tables share the
// prefix through +44; +48.. differ per family (PanelVtbl below). Only slots called
// by this lane are typed; the rest are opaque words. Instances are the typed
// constant tables in gui_vtables.cpp, viewed through this type (D24 object cast).
struct ObjectVtbl {
    void* (__fastcall* deleting_destroy)(Object*, void*, Word flags);  // +00 (0040CFA7, 005FCAA9)
    void (__fastcall* set_extent)(Object*, void*, const Extent*);      // +04 RET 4 (0041B8FF/0040A7D6)
    void (__fastcall* set_active_control)(Object*, void*, Object*, Word); // +08 RET 8 (00418998)
    void (__fastcall* handle_mouse_over)(Object*, void*, std::int32_t); // +0C RET 4 (0040B8BF)
    const void* slot10;
    void (__fastcall* release_capture)(Object*, void*);                // +14 (0041ACAE tail)
    const void* slots18_24[4];
    Panel* (__fastcall* as_panel)(Object*, void*);                     // +28 (0041B939, 00418975)
    Word (__fastcall* as_control)(Object*, void*);                     // +2C (004189DF)
    const void* slots30_38[3];
    void (__fastcall* handle_input_event)(Object*, void*, std::int32_t, std::int32_t); // +3C RET 8 (0040A649)
    void (__fastcall* set_active)(Object*, void*, std::int32_t);       // +40 RET 4 (00418DE2)
    const void* slot44;
    void (__fastcall* load)(Object*, void*, Gff*, Struct*);            // +48 RET 8 (controls; 00418838/00418930)
    Navigable* (__fastcall* as_navigable)(Object*, void*);             // +4C (0040A8B0)
    const void* slots50_54[2];
    Word (__fastcall* slot58)(Object*, void*);                         // +58 (0040B8D1) control: 0063E7F0/00641DB0
    const void* slots5c_78[8];
    void (__fastcall* focus_change)(Object*, void*, Word);             // +7C RET 4 (0040BDE2)
    Word (__fastcall* is_selectable)(Object*, void*);                  // +80 (0041AC35)
    void (__fastcall* rmouse_down)(Object*, void*);                    // +84 (0041AC4F tail)
    const void* slot88;
    void (__fastcall* reset_font)(Object*, void*);                     // +8C (0040A9E0)
};
static_assert(offsetof(ObjectVtbl, set_extent) == 0x04 && offsetof(ObjectVtbl, as_panel) == 0x28 &&
              offsetof(ObjectVtbl, handle_input_event) == 0x3C && offsetof(ObjectVtbl, load) == 0x48 &&
              offsetof(ObjectVtbl, as_navigable) == 0x4C && offsetof(ObjectVtbl, slot58) == 0x58 &&
              offsetof(ObjectVtbl, focus_change) == 0x7C && offsetof(ObjectVtbl, is_selectable) == 0x80 && offsetof(ObjectVtbl, reset_font) == 0x8C, "slots");
// Panel-family continuation (73E010 / 752F70).
struct PanelVtbl {
    const void* prefix[0x12];                                           // +00..+44 as ObjectVtbl
    void (__fastcall* on_panel_added)(Panel*, void*);                  // +48 (0040BD0E)
    const void* slots4c_60[6];
    String* (__fastcall* full_screen_bg)(Panel*, void*, String* out);  // +64 RET 4 (0040AD2D/0040ADB1)
    void (__fastcall* reset_font)(Panel*, void*);                      // +68 (0040B877)
};
static_assert(offsetof(PanelVtbl, on_panel_added) == 0x48 && offsetof(PanelVtbl, full_screen_bg) == 0x64 &&
              offsetof(PanelVtbl, reset_font) == 0x68, "panel slots");

struct Object { const ObjectVtbl* vtbl; Extent extent; };          // +00 vptr, +04 extent (0040A70B/0041B8EF)

// CSWGuiDrawable family: CSWGuiBorder (0x74, ctor 004167F0, vtable 73E338) and
// CSWGuiText (0x70, ctor 00417280, vtable 73E3A4); base drawable vtable 73E324.
struct BorderVtbl {
    void* (__fastcall* deleting_destroy)(Border*, void*, Word flags); // +00
    void (__fastcall* set_extent)(Border*, void*, const Extent*);     // +04 RET 4 (0040A7D6, 004177AF)
    const void* slot08; const void* slot0c;
    void (__fastcall* load)(Border*, void*, Gff*, Struct*, String*);  // +10 RET 0Ch
};
struct TextVtbl {
    void* (__fastcall* deleting_destroy)(Text*, void*, Word flags);   // +00
    void (__fastcall* set_extent)(Text*, void*, const Extent*);       // +04 RET 4 (004177CF, 00417A9C)
    const void* slot08; const void* slot0c;
    void (__fastcall* load)(Text*, void*, Gff*, Struct*, String*);    // +10 RET 0Ch
};
struct Border { const BorderVtbl* vtbl; Extent extent; unsigned char params[0x54];   // +14 CSWGuiBorderParams
                unsigned char tail[0x0C]; };
struct Text { const TextVtbl* vtbl; unsigned char rest[0x6C]; };
static_assert(sizeof(Border) == 0x74 && offsetof(Border, params) == 0x14 && sizeof(Text) == 0x70, "border/text");

// CSWGuiControl (0x5C; ctor 0041AA80, dtor 00419A20).
struct Control : Object {
    Control* parent;                 // +14 written 0041B94B; zeroed 0041AAF3
    Array children;                  // +18 ctor 00416770(0) at 0041AABA; AddChildControl 0041ACC0
    Word u24;                        // +24 zeroed 0041AAF6
    unsigned char text28[8];         // +28 CExoString (005B3190 at 0041AAC7, 005E5C20 at 00419A5B)
    Word u30;                        // +30 zeroed 0041AAF9
    Object* owner;                   // +34 written 0041886E / 0041880C; zeroed 0041AAEA
    Array events;                    // +38 CExoArrayList<CSWGuiControlEvent> (12-byte entries; 0041A870, 0041AB20)
    Word flags;                      // +44 ctor: (x & 0xFFFFFF8E) | 0x0A (0041AADA..E7)
    Word u48;                        // +48
    Word u4c;                        // +4C
    std::int32_t id;                 // +50 = -1 in ctor (0041AAFC); written 0041B925, read 0040B956
    unsigned char b54;               // +54 = 0 (0041AAF0)
    unsigned char b55;               // +55 = 1 (0041AB03); PlayGuiSound id at 0040A66F
    unsigned char u56[2];
    Word u58;                        // +58 = 0 (0041AAED)
};
// CSWGuiNavigable (0x6C; ctor 0041B8B0). move_to[0..3] at +5C..+68: indices from
// MOVETO (UP +5C, LEFT +60, DOWN +64, RIGHT +68; 0041C8C6..0041C91C), later
// replaced by control pointers (0040A8E0).
struct Navigable : Control { Word move_to[4]; };
struct Label : Control { Border border; Text text; };                         // 0x140: +5C border, +D0 text (0041B99E/0041B9D8)
struct Button : Navigable { Border border; Border hilight; Text text; };      // 0x1C4: +6C, +E0, +154 (0041C96B/9A5/9E2)
struct ListBox : Navigable { unsigned char rest[0x2E0 - 0x6C]; };             // 0x2E0 (ListBox fork; menu +344..+35C are menu fields)
static_assert(sizeof(Control) == 0x5C && sizeof(Navigable) == 0x6C && sizeof(Label) == 0x140 &&
              sizeof(Button) == 0x1C4 && sizeof(ListBox) == 0x2E0, "control sizes");

// CSWGuiPanel (0x64; ctor 0040B570, dtor 0040CF70).
struct Panel : Object {
    std::int32_t u14;                // +14 = 1 (0040B615)
    Object* manager;                 // +18 ctor arg (0040B5F1); CSWGuiManager* (lane-input view)
    Control* active;                 // +1C active control (0040B5FA zero; 0040A638 SetActiveControl)
    Array controls;                  // +20 (0040B59F; read 0040B95D/0040BA04)
    Gff* layout;                     // +2C (0040A6E9; 0040B8F9)
    List layout_controls;            // +30 (0040A849 GetList "CONTROLS")
    Word flags;                      // +44 bit1 = layout open (0040A69B/0040A854/0040B911); ctor (x&0xFFFFF89C)|0x84
    unsigned char b48;               // +48 = 0 (0040B600); GetFullScreenBG index (0040A93D)
    unsigned char u49[3];
    float alpha;                     // +4C (00404B37), ctor 1.0 (0040B60A)
    Vector color;                    // +50 (0040A74D), ctor zero (0040B5B8..BE)
    Border* background;              // +5C (0040B5E4)
    unsigned char b60;               // +60 = 0xFF (0040B611); PlayGuiSound id at 0040BC8A
    unsigned char u61[3];
};
static_assert(sizeof(Panel) == 0x64, "panel size");

// CSWGuiMainMenu (0x1414; operator new at 005FCAB5).
struct MainMenu : Panel {
    ListBox lb_modules;              // +0064 "LB_MODULES" (0x2E0)
    Word u344, u348, u34c;           // +0344.. zeroed 0067C500..0C
    Word u350;                       // +0350 = 1 (0067C910)
    Word u354, u358, u35c;           // +0354.. zeroed 0067C512..1E
    Control lbl_3dview;              // +0360 "LBL_3DVIEW" (CSWGui3DSceneView, vtable 752E30)
    unsigned char scene[0x34];       // +03BC CSWGuiScene (004174B0)
    Button btn_newgame;              // +03F0
    Button btn_loadgame;             // +05B4
    Button btn_movies;               // +0778
    Button btn_options;              // +093C
    Button btn_b00;                  // +0B00 (not loaded by 0067ACE0)
    Label lbl_newcontent;            // +0CC4
    Label lbl_gamelogo;              // +0E04 (vector ctor 0067C5CE, 2 x 0x140)
    Label lbl_menubg;                // +0F44
    Button btn_exit;                 // +1084
    Button btn_warp;                 // +1248
    Word u140c, u1410;
};
static_assert(sizeof(MainMenu) == 0x1414, "main menu size");
}


extern "C" {
// ---- authored (thiscall-equivalent fastcall veneers: ECX self, EDX unused) ----
void __fastcall gui_load_array_add_00671c00(kotor_reconstruction::ArrayMutations*, void*, gui_load::Word);
void __fastcall gui_load_nav_set_move_to_00417660(gui_load::Navigable*, void*, std::int32_t, gui_load::Word);
gui_load::Word __fastcall gui_load_nav_get_move_to_00417680(gui_load::Navigable*, void*, std::int32_t);
gui_load::Control* __fastcall gui_load_panel_get_control_0040ba00(gui_load::Panel*, void*, std::int32_t);
void __fastcall gui_load_control_add_child_0041acc0(gui_load::Control*, void*, gui_load::Control*);
void __fastcall gui_load_panel_set_alpha_00404b00(gui_load::Panel*, void*, float);
void __fastcall gui_load_panel_resolve_move_to_0040a890(gui_load::Panel*, void*);
void __fastcall gui_load_extent_load_00409dc0(gui_load::Extent*, void*, gui_load::Gff*, gui_load::Struct*);
void __fastcall gui_load_control_load_0041b8e0(gui_load::Control*, void*, gui_load::Gff*, gui_load::Struct*);
void __fastcall gui_load_navigable_load_0041c890(gui_load::Navigable*, void*, gui_load::Gff*, gui_load::Struct*);
void __fastcall gui_load_label_load_0041b960(gui_load::Label*, void*, gui_load::Gff*, gui_load::Struct*);
void __fastcall gui_load_button_load_0041c930(gui_load::Button*, void*, gui_load::Gff*, gui_load::Struct*);
void __fastcall gui_load_control_load_label_00418800(gui_load::Control*, void*, gui_load::Object*,
    gui_load::Gff*, gui_load::Struct*, gui_load::String*);
void __fastcall gui_load_control_load_tag_00418840(gui_load::Control*, void*, gui_load::Object*,
    gui_load::Gff*, gui_load::List*, gui_load::String*);
void __fastcall gui_load_panel_init_control_0040b930(gui_load::Panel*, void*, gui_load::Control*,
    gui_load::String*, std::int32_t);
void __fastcall gui_load_panel_stop_layout_0040b8f0(gui_load::Panel*, void*);
void __fastcall gui_load_panel_start_layout_0040a680(gui_load::Panel*, void*, gui_load::ResRef*);
void __fastcall gui_load_mainmenu_load_layout_0067ace0(gui_load::MainMenu*, void*, gui_load::Word unused);

// ---- external dependencies ----
// D18: CRes::Demand 00409B10 / CRes::Release 00409B80 single provider lane-resman resman_objects.
std::uint32_t __fastcall c2da_current_resource_demand_00409b10(void*, void*);
void __fastcall c2da_release_demand_00409b80(void*, void*);
// M40 (m39-directory-allocator.obj) original operator new 006FA7E6.
void* __cdecl c2da_directory_new_006fa7e6(std::uint32_t) noexcept(false);
// MISSING: CResGFF::CResGFF(WORD type, char const* "GUI ", CResRef const&) 00410630 RET 0Ch.
gui_load::Gff* __fastcall gui_load_gff_construct_00410630(gui_load::Gff*, void*, gui_load::Word type,
    const char* file_type, const gui_load::ResRef* name);
// MISSING native: CSWGuiBorderParams::SetFillImage(CResRef const&, int force) 00414C00 RET 8
// (compares/assigns params+40, then NewCAurGUIImage 0045A260 = fill texture request).
// Donor (Context form only): task-17/gui-parameter-closure-r1 border_params.cpp.
void __fastcall gui_load_border_params_set_fill_00414c00(void* params, void*, const gui_load::ResRef*,
    std::int32_t force) noexcept(false);
}
