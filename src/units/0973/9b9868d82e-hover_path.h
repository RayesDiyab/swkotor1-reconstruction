#pragma once
// Main-menu hover/click leaf path (milestone-1 work-ahead, lane-input).
// Consumed-prefix views only: every field below is an offset observed in the
// cited original instructions; unnamed gaps are opaque padding, not invented
// fields. No complete GUI class layout or size is claimed.
#include <cstddef>
#include <cstdint>
#include <cstring>
#include "06f8bd1a7d-matching.h"   // task-10 menu-enter-matching-r1: MenuColor, ColorReceiver
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error This bounded reconstruction requires MSVC Windows x86.
#endif

// M41 provider declarations, token-identical to canonical menu_targets.h (task-19 menu-startup-20-29).
namespace kotor_menu20 {
using Word=std::uint32_t;
struct ClientInternal {Word unknown;void* volatile options;};
struct ClientApp {Word unknown;ClientInternal* volatile internal;__declspec(noinline) void* __thiscall get_options_005ed700();};
}
namespace kotor_input_hover {
using U32 = std::uint32_t;

struct __single_inheritance GuiObject;
struct __single_inheritance AurGuiString;
struct Control;
struct Navigable;
struct Panel;
struct Manager;
struct ControlEvent;
struct CursorModel;

struct ListBox;
struct Extent { int x; int y; int width; int height; };   // CSWGuiExtent (0040AA09..1E copy)

// Original vtable slots are raw code words used with ECX=object (thiscall).
// A single-inheritance PMF is exactly one code word (static_asserted below),
// so slots and callbacks are typed PMFs: no function-pointer casts (D1/D21).
// Arg/return types of slots whose bodies are not authored here are inferred
// from callee RET sizes (rets.py) and are marked "inferred".
struct GuiObjectVtable {                                   // slots common to panels and controls
    void* (GuiObject::*scalar_destroy)(unsigned flags);    // 0x00 button 00419E20 RET4
    void (GuiObject::*set_extent)(const Extent* extent);   // 0x04 button 00417A20 RET4 (arg type inferred)
    void (GuiObject::*set_active_control)(Control*, int);  // 0x08 panel 0040A630 / control ret8 no-op 005B5E90
    void (GuiObject::*handle_mouse_over)(int);             // 0x0C button 0041A980 / panel ret4 no-op 0060E760
    int (GuiObject::*captured_movement)(int, int);         // 0x10 button 00409EB0 (xor eax; ret 8); called 0040C259
    void (GuiObject::*release_capture)();                  // 0x14 button/panel no-op 004015A0
    void (GuiObject::*handle_lmouse_down)();               // 0x18 control 0041AB90
    void (GuiObject::*handle_lmouse_up)();                 // 0x1C control 0041ABD0
    void (GuiObject::*slot20)();                           // 0x20 button 004015A0 RET0
    void (GuiObject::*handle_rmouse_up)();                 // 0x24 control 0041AC70 RET0
    Panel* (GuiObject::*as_panel)();                       // 0x28 panel self 00641DB0 / button zero 0063E7F0
    Control* (GuiObject::*as_control)();                   // 0x2C panel/button zero 0063E7F0
    GuiObject* (GuiObject::*slot30)();                     // 0x30 button self 00641DB0
    U32 (GuiObject::*slot34)();                            // 0x34 button 004064A0 (mov eax,[ecx+10h])
    void (GuiObject::*draw)(void* arg);                    // 0x38 button 00417AB0 RET4 (arg type inferred)
    void (GuiObject::*handle_input_event)(int, int);       // 0x3C button 0041AD40 / main menu 0067B380
};
struct ControlVtable : GuiObjectVtable {                   // control family continues after 0x3C
    void (GuiObject::*set_active)(int);                    // 0x40 button 00418E00
    Control* (GuiObject::*hit_check)(int, int);            // 0x44 control 004187C0 (called 0040B743)
    void (GuiObject::*load)(void*, void*);                 // 0x48 button 0041C930 RET8 (arg types inferred)
    Navigable* (GuiObject::*as_navigable)();               // 0x4C button self 00641DB0
    int (GuiObject::*slot50)();                            // 0x50 button zero
    int (GuiObject::*slot54)();                            // 0x54 button zero
    GuiObject* (GuiObject::*slot58)();                     // 0x58 button self
    int (GuiObject::*slot5c)();                            // 0x5C button zero
    int (GuiObject::*slot60)();                            // 0x60 button zero; tested 0040C326 / 0040C3AF
    int (GuiObject::*slot64)();                            // 0x64
    int (GuiObject::*slot68)();                            // 0x68
    int (GuiObject::*slot6c)();                            // 0x6C
    ListBox* (GuiObject::*as_listbox)();                   // 0x70 button zero; 0040C52D / 0040B51C
    int (GuiObject::*slot74)();                            // 0x74
    int (GuiObject::*slot78)();                            // 0x78 button zero; tested 0040C336 / 0040C3BA
    void (GuiObject::*handle_focus_change)(int);           // 0x7C control 00418960 RET4
    int (GuiObject::*is_selectable)();                     // 0x80 control 004189D0
    void (GuiObject::*handle_rmouse_down)();               // 0x84 control 0041AC30 RET0
    void (GuiObject::*set_enabled)(int);                   // 0x88 button 00418DB0 RET4
    void (GuiObject::*reset_font)(void*);                  // 0x8C button 00418DF0 RET4 (arg type inferred)
    void (GuiObject::*display_tooltip)();                  // 0x90 control 00418A90 RET0
    void (GuiObject::*slot94)(U32, U32);                   // 0x94 button 005B5E90 RET8
    void (GuiObject::*initialize2)(void*, void*);          // 0x98 button 00418D80 RET8 (arg types inferred)
    void (GuiObject::*initialize4)(void*, void*, void*, void*);  // 0x9C button 00417970 RET10 (arg types inferred)
};
struct PanelVtable : GuiObjectVtable {                     // panel family continues after 0x3C
    Control* (GuiObject::*hit_check_mouse)(int, int);      // 0x40 panel 0040B690
    Control* (GuiObject::*get_active_control)();           // 0x44 panel 0040B630 (called 0040C34D)
};

struct GuiObject {
    const GuiObjectVtable* vtable;                         // +0x00
    // D24 typed bodies for the shared trivial original functions used in vtables.
    void __thiscall VoidReturn2_005b5e90(Control*, int);   // 005B5E90 ret 8
    void __thiscall VoidReturn2u_005b5e90(U32, U32);       // 005B5E90 ret 8 (slot 0x94 typing)
    int __thiscall ReturnZero2_00409eb0(int, int);         // 00409EB0 xor eax,eax; ret 8
    void __thiscall VoidReturn_004015a0();                 // 004015A0 ret
    Panel* __thiscall ReturnZeroPanel_0063e7f0();          // 0063E7F0 xor eax,eax; ret
    Control* __thiscall ReturnZeroControl_0063e7f0();      // 0063E7F0
    int __thiscall ReturnZeroInt_0063e7f0();               // 0063E7F0
    ListBox* __thiscall ReturnZeroListBox_0063e7f0();      // 0063E7F0
    GuiObject* __thiscall SelfReturn_00641db0();           // 00641DB0 mov eax,ecx; ret
    Navigable* __thiscall SelfReturnNavigable_00641db0();  // 00641DB0
    U32 __thiscall GetProperty0x10_004064a0();             // 004064A0 mov eax,[ecx+10h]; ret
};

// CSWGuiControlEvent {object, func, id}: dispatched at 004187AB..004187B4 as
// ECX=object, push control, call func (callee cleans 4).
struct ControlEvent {
    GuiObject* object;                                     // +0
    void (GuiObject::*func)(Control*);                     // +4
    int id;                                                // +8
};

struct Control : GuiObject {
    int x;                                                 // +0x04 (004187CF)
    int y;                                                 // +0x08 (004187E2)
    int width;                                             // +0x0C (004187D8)
    int height;                                            // +0x10 (004187EB)
    Control* parent_control;                               // +0x14 (00418A50 walk)
    Control** children;                                    // +0x18 (00418A2A)
    int child_count;                                       // +0x1C (00418A20)
    unsigned char opaque20[0x14];
    GuiObject* owner;                                      // +0x34 (0041AD56, 004189D4)
    ControlEvent* events;                                  // +0x38 (0041878C) CExoArrayList<CSWGuiControlEvent> data
    int event_count;                                       // +0x3C (00418783)
    int event_capacity;                                    // +0x40 (0041A8A3)
    U32 flags;                                             // +0x44
    int last_event;                                        // +0x48 (004187AB)
    int last_event_value;                                  // +0x4C (004187AE)
    unsigned char opaque50[4];
    unsigned char click_sound;                             // +0x54 (0041AD92)
    unsigned char opaque55[3];

    const ControlVtable* cvt() const { return static_cast<const ControlVtable*>(vtable); }
    void __thiscall SetActive(int active);                                        // 004181D0
    void __thiscall HandleInputEvent(int event, int value);                       // 00418750
    void __thiscall HandleMouseOver(int over);                                    // 004189A0
    void __thiscall RemoveToolTip();                                              // 004176C0
    Control* __thiscall HitCheckMouse(int x, int y);                              // 004187C0
    Control* __thiscall GetSelectableParent();                                    // 00418A50
    int __thiscall GetIsSelectable();                                             // 004189D0
    void __thiscall HandleLMouseDown();                                           // 0041AB90
    void __thiscall HandleLMouseUp();                                             // 0041ABD0
    int __thiscall GetIsChild(Control* control);                                  // 00418A20
    void __thiscall HandleFocusChange(int focus);                                 // 00418960
    void __thiscall AddEvent(int id, GuiObject* object, void (GuiObject::*func)(Control*));  // 0041AB20 RET0C
    // Not authored (vtable references only; types inferred from RET sizes):
    void __thiscall HandleRMouseUp_0041ac70();
    void __thiscall HandleRMouseDown_0041ac30();
    void __thiscall DisplayToolTip_00418a90();
};

// CExoArrayList<CSWGuiControlEvent> receiver at control+0x38.
struct ControlEventArray {
    ControlEvent* data; int count; int capacity;
    void __thiscall Add(ControlEvent e);                   // 0041A8A0 RET0C (by-value 12 bytes)
    void __thiscall Allocate(int capacity);                // 004186E0 RET4
    void __thiscall DeleteAt(int index);                   // 0048DDE0 RET4 (12-byte element instantiation)
};

struct Navigable : Control {
    unsigned char opaque58[4];
    Control* neighbors[4];                                 // +0x5C..+0x68 (0041A9FC jump table 0041AA58)
    void __thiscall HandleMouseOver(int over);                                    // 0041A980
    void __thiscall HandleInputEvent(int event, int value);                       // 0041A9D0
};

// CSWGuiTextParams consumed prefix (button text params live at button+0x16C).
struct AurGuiStringVtable {                                // .rdata 00741878 (ctor 0045B9B5)
    const void* slot00_draw;
    const void* slot04_destructor;
    const void* slot08_get_color;
    void (AurGuiString::*set_color)(const MenuColor*);     // 0x0C 0045BB00
};
struct AurGuiString { const AurGuiStringVtable* vtable; };
struct GuiText {
    unsigned char opaque00[0x14];
    AurGuiString* gui_string;                              // +0x14 (00414E34)
};
struct TextParams {
    unsigned char opaque00[0x1C];
    MenuColor color;                                       // +0x1C (00414E22..2E)
    unsigned char opaque28[4];
    MenuColor default_color;                               // +0x2C (00414E80)
    U32 flags;                                             // +0x38 (00418E20 / 00418E31)
    unsigned char opaque3c[0x14];
    GuiText* text_object;                                  // +0x50 (00414E1B)
    ColorReceiver* receiver() { return reinterpret_cast<ColorReceiver*>(this); }
    void __thiscall RestoreDefaultColor();                                        // 00414E80
};

struct Button : Navigable {
    unsigned char opaque6c[0x16C - 0x6C];
    TextParams text;                                       // +0x16C (00418E15, 0067B459)
    void __thiscall SetActive(int active);                                        // 00418E00
    void __thiscall HandleInputEvent(int event, int value);                       // 0041AD40
    // Not authored (vtable references only; types inferred from RET sizes):
    void* __thiscall ScalarDestroy_00419e20(unsigned flags);
    void __thiscall SetExtent_00417a20(const Extent* extent);
    void __thiscall Draw_00417ab0(void* arg);
    void __thiscall Load_0041c930(void* a, void* b);
    void __thiscall SetEnabled_00418db0(int enabled);
    void __thiscall ReSetFont_00418df0(void* arg);
    void __thiscall Initialize_00418d80(void* a, void* b);
    void __thiscall Initialize_00417970(void* a, void* b, void* c, void* d);
};

// CSWGuiButton vtable 0073E658 as typed data (D21): Button-member PMF slots so that
// base-class bodies convert implicitly (no casts). Layout identical to ControlVtable.
struct ButtonVtable {
    void* (Button::*s00)(unsigned); void (Button::*s04)(const Extent*); void (Button::*s08)(Control*, int); void (Button::*s0c)(int);
    int (Button::*s10)(int, int); void (Button::*s14)(); void (Button::*s18)(); void (Button::*s1c)();
    void (Button::*s20)(); void (Button::*s24)(); Panel* (Button::*s28)(); Control* (Button::*s2c)();
    GuiObject* (Button::*s30)(); U32 (Button::*s34)(); void (Button::*s38)(void*); void (Button::*s3c)(int, int);
    void (Button::*s40)(int); Control* (Button::*s44)(int, int); void (Button::*s48)(void*, void*); Navigable* (Button::*s4c)();
    int (Button::*s50)(); int (Button::*s54)(); GuiObject* (Button::*s58)(); int (Button::*s5c)();
    int (Button::*s60)(); int (Button::*s64)(); int (Button::*s68)(); int (Button::*s6c)();
    ListBox* (Button::*s70)(); int (Button::*s74)(); int (Button::*s78)(); void (Button::*s7c)(int);
    int (Button::*s80)(); void (Button::*s84)(); void (Button::*s88)(int); void (Button::*s8c)(void*);
    void (Button::*s90)(); void (Button::*s94)(U32, U32); void (Button::*s98)(void*, void*); void (Button::*s9c)(void*, void*, void*, void*);
};

struct __single_inheritance CursorModel;
struct Vector3 { float x, y, z; };
struct CursorModelVtable {                                 // consumed slot only
    const void* slots00_58[0x17];
    void (CursorModel::*set_position)(Vector3* out, Vector3 position);  // 0x5C (0040C24B; same form as gui_mouse.cpp slot 0x5C)
};
struct CursorModel { const CursorModelVtable* vtable; };
struct ListBox {                                           // CSWGuiListBox (externals)
    int __thiscall HitCheckMouseLocal(int flag);           // 0041A5E0 RET4
    void __thiscall HandleMouseMove();                     // 0041C400 RET0
};

struct SoundSource {
    unsigned char opaque00[4];
    void* internal;                                        // +0x04 (005D5930)
    void __thiscall Play();                                // 005D5930
};

struct Panel : GuiObject {
    Extent extent;                                         // +0x04 (0040B699..A4)
    unsigned char opaque14[4];
    Manager* manager;                                      // +0x18 (0041AD89, 0067B435, 0040AA28)
    Control* active_control;                               // +0x1C (0040A638)
    Control** controls;                                    // +0x20 (0040B735)
    int control_count;                                     // +0x24 (0040B729)
    unsigned char opaque28[0x1C];
    U32 flags;                                             // +0x44 (0040B6A7, 0040AA21)
    Control* __thiscall HitCheckMouse(int x, int y);       // 0040B690 RET8
    void __thiscall GetExtentAccountingForPanelOffset(Extent* out);  // 0040AA00 RET4
    const PanelVtable* pvt() const { return static_cast<const PanelVtable*>(vtable); }
};

struct MainMenu : Panel {
    void __thiscall OnEnterButton(Control* control);       // 0067B450 (ECX ignored)
    void __thiscall OnExitButton(Control* control);        // 0067B470 (ECX ignored)
    // Not authored (registration references only):
    void __thiscall OnNewGamePicked_0067afb0(Control* control);
    void __thiscall OnLoadSaveGame_0067b1a0(Control* control);
    void __thiscall OnMoviesPicked_0067b250(Control* control);
    void __thiscall OnOptionsPicked_0067b2f0(Control* control);
    void __thiscall OnWarpButtonPressed_0067c4b0(Control* control);
    void __thiscall OnQuitButtonPressed_0067b4a0(Control* control);
};
// CSWGuiMainMenu ctor AddEvent sequence 0067C682..0067C81D: AddEvent(event, menu, func) on
// the button at menu+offset, in this order.
struct MainMenuEventRegistration { U32 button_offset; int event_id; void (MainMenu::*func)(Control*); };
void RegisterMainMenuEvents0067c682(MainMenu* menu);   // ctor-inline sequence helper (D47)

struct Manager {                                           // CSWGuiManager consumed prefix
    int mouse_x;                                           // +0x00 (0040B311)
    int mouse_y;                                           // +0x04 (0040B309)
    Control* mouse_over;                                   // +0x08
    int capture_flag;                                      // +0x0C (0040A1EC)
    Control* captured;                                     // +0x10 (0040A1E9)
    Control* last_hit;                                     // +0x14 (0040B342)
    unsigned char opaque18[4];
    U32 flags;                                             // +0x1C
    unsigned char opaque20[4];
    CursorModel* cursor_model;                             // +0x24 software cursor (0040C1EC)
    unsigned char opaque28[0x18];
    Control* tooltip_control;                              // +0x40 (0040C15C)
    U32 tooltip_timer;                                     // +0x44 (0040B4EF; float -1.0 bits at 0040B511)
    unsigned char opaque48[4];
    int tooltip_mode;                                      // +0x4C (0040B4D3)
    U32 tooltip_item;                                      // +0x50 (0040B538)
    unsigned char opaque54[0x10];
    U32 direction_time;                                    // +0x64 (0040C9EB)
    int last_input_event;                                  // +0x68 (0040C90A)
    std::int16_t width;                                    // +0x6C (0040AA2B)
    std::int16_t height;                                   // +0x6E (0040C201)
    unsigned char opaque70[2];
    unsigned char last_direction;                          // +0x72 (0040C9A4, 0040C9E8)
    unsigned char opaque73[0x15];
    Panel** panels;                                        // +0x88 (0040AC50)
    int panel_count;                                       // +0x8C (0040AC36)
    unsigned char opaque90[4];
    Panel** modal_panels;                                  // +0x94 (0040ABEE)
    int modal_count;                                       // +0x98 (0040ABE4)
    unsigned char opaque9c[4];
    SoundSource** sounds;                                  // +0xA0 (0040A151)
    signed char sound_count;                               // +0xA4 (0040A149, signed JGE)

    void __thiscall PlayGuiSound(char sound);                                     // 0040A140
    void __thiscall ResetToolTipTimer();                                          // 0040B4D0
    void __thiscall RemoveToolTip();                                              // 0040C150
    void __thiscall SetTemporarilyDisableToolTip(int disable);                    // 0040B500
    void __thiscall HandleMouseMove(int x, int y);                                // 0040C1E0
    int __thiscall HandleLMouseDown(int value);                                   // 0040C570
    void __thiscall HandleInputEvent(int event, int value);                       // 0040C8E0
    void __thiscall RemovePanel(Panel* panel);                                    // 0040C830 RET4 (external)
    void __thiscall CaptureMouse(Control* control, int flag);                     // 0040A1C0
    void __thiscall ReleaseMouse(Control* control);                               // 0040A200
    int __thiscall HandleLMouseUp();                                              // 0040A170
    int __thiscall HitCheckMouse(int x, int y, Panel** panel, Control** control, int unused); // 0040ABE0
    void __thiscall UpdateMouseOverControl();                                     // 0040B300
};

// CClientExoApp -> internal -> options (005ED700); options consumed: +0x14.
struct ClientOptions { unsigned char opaque00[0x14]; U32 flags14; };
struct ClientInternalOptionsView { unsigned char opaque00[4]; ClientOptions* options; unsigned char opaque08[0x94]; U32 input_class; };  // +0x9C (005EDA73)
struct ClientApp {
    unsigned char opaque00[4];
    ClientInternalOptionsView* internal;                   // +0x04
    // 005ED700: D47 -- M41 provider kotor_menu20::ClientApp::get_options_005ed700 wins; inline forwarder only (no body here).
    ClientOptions* GetClientOptions() { return static_cast<ClientOptions*>(reinterpret_cast<kotor_menu20::ClientApp*>(this)->get_options_005ed700()); }
    U32 __thiscall GetInputClass();                        // 005EDA70 (internal +0x9C)
};
struct AppManager { unsigned char opaque00[4]; ClientApp* client; };   // *(007A39FC)+4 (0040B4DA)

// CClientExoAppInternal: opaque 0x4D4-byte view (D20, allocation 0x4D4 at 005EE309);
// only mouse_x/mouse_y are named. Other offsets via raw accessors (memcpy, no puns).
struct ClientInternal {
    unsigned char opaque000[0x3C8];
    int mouse_x;                                           // +0x3C8 (0061F7E4)
    int mouse_y;                                           // +0x3CC (0061F83B)
    unsigned char opaque3d0[0x4D4 - 0x3D0];
    template<class T> T get(U32 off) const { T v; std::memcpy(&v, reinterpret_cast<const unsigned char*>(this) + off, sizeof(T)); return v; }
    template<class T> void set(U32 off, T v) { std::memcpy(reinterpret_cast<unsigned char*>(this) + off, &v, sizeof(T)); }
    int __thiscall SetMouseX(int x);                       // 0061F7E0
    int __thiscall SetMouseY(int y);                       // 0061F830
    int __thiscall HandleInputEvent(int event, int value); // 00621210 (client_input_event.cpp)
    void __thiscall ProcessInput(float delta);             // 006227E0 (process_input.cpp)
};

// CExoInput family (input_system.cpp). Events delivered by GetEvents.
struct InputMessage { U32 hwnd; U32 message; U32 wparam; U32 lparam; };   // WndProc frame words
struct InputEventDesc;                                     // event description; game event id at +0x10
struct InputEvent {                                        // CExoInputEvent, 16 bytes (005E2C3C..51)
    int value;                                             // +0x00
    U32 word04;                                            // +0x04
    U32 word08;                                            // +0x08
    InputEventDesc* desc;                                  // +0x0C
};
struct InputEventDesc { unsigned char opaque00[0x10]; int id; };          // +0x10 event id (006229DB)
struct InputEventList { InputEvent* data; int count; int capacity; };   // CExoArrayList<CExoInputEvent>
struct RawInput;                                           // input_system.h
struct InputInternal;
struct InputVtable;
struct Input {
    const InputVtable* vtable;                             // +0x00 = 0074D5EC (005DF40E)
    InputInternal* internal;                               // +0x04 (005DF449)
    Input* __thiscall Construct();                         // 005DF3F0
    void* __thiscall ScalarDestroy(unsigned flags);        // 005DF670 RET4
    void __thiscall Configure();                           // 005DF500 (JMP [internal vt+4])
    int __thiscall GetActive();                            // 005DF510
    int __thiscall BufferEvent(const InputMessage* message);  // 005DF590
    void __thiscall UpdateMouseDelta(float delta);         // 005DF620
    void __thiscall WriteEventToFile(U32 time, int index, void* file);  // 005DF4F0 RET0C
    int __thiscall RefreshMouseCursor(int down);           // 005DF5B0 (JMP 005E0930)
    InputEventList __thiscall GetEvents(int* count, int input_class);  // 005DF6B0 (hidden return pointer)
};
static_assert(sizeof(void*) == 4, "x86");
static_assert(sizeof(void (GuiObject::*)(int)) == 4 && sizeof(void (AurGuiString::*)(const MenuColor*)) == 4, "one-word PMF slots");
static_assert(sizeof(ControlEvent) == 12 && offsetof(ControlEvent, id) == 8, "CSWGuiControlEvent");
static_assert(offsetof(GuiObjectVtable, set_active_control) == 0x08 && offsetof(GuiObjectVtable, handle_mouse_over) == 0x0C &&
              offsetof(GuiObjectVtable, release_capture) == 0x14 && offsetof(GuiObjectVtable, handle_lmouse_down) == 0x18 &&
              offsetof(GuiObjectVtable, handle_lmouse_up) == 0x1C && offsetof(GuiObjectVtable, as_panel) == 0x28 &&
              offsetof(GuiObjectVtable, as_control) == 0x2C && offsetof(GuiObjectVtable, handle_input_event) == 0x3C, "common slots");
static_assert(offsetof(ControlVtable, set_active) == 0x40 && offsetof(ControlVtable, as_navigable) == 0x4C &&
              offsetof(ControlVtable, is_selectable) == 0x80, "control slots");
static_assert(offsetof(PanelVtable, hit_check_mouse) == 0x40 && offsetof(PanelVtable, get_active_control) == 0x44, "panel slots");
static_assert(offsetof(ControlVtable, hit_check) == 0x44 && offsetof(ControlVtable, slot60) == 0x60 && offsetof(ControlVtable, as_listbox) == 0x70 &&
              offsetof(ControlVtable, slot78) == 0x78 && offsetof(ControlVtable, handle_focus_change) == 0x7C && sizeof(ControlVtable) == 0xA0, "control vtable 0xA0");
static_assert(offsetof(GuiObjectVtable, captured_movement) == 0x10 && offsetof(GuiObjectVtable, draw) == 0x38, "slots 10/38");
static_assert(offsetof(CursorModelVtable, set_position) == 0x5C, "cursor slot");
static_assert(offsetof(Panel, controls) == 0x20 && offsetof(Panel, flags) == 0x44, "panel");
static_assert(offsetof(Manager, cursor_model) == 0x24 && offsetof(Manager, tooltip_control) == 0x40 && offsetof(Manager, tooltip_item) == 0x50 &&
              offsetof(Manager, height) == 0x6E && offsetof(Manager, panels) == 0x88 && offsetof(Manager, last_direction) == 0x72 &&
              offsetof(Manager, direction_time) == 0x64, "manager tooltip/dims");
static_assert(offsetof(ClientInternalOptionsView, input_class) == 0x9C && offsetof(SoundSource, internal) == 4, "client/sound");
static_assert(offsetof(AurGuiStringVtable, set_color) == 0x0C, "string slot");
static_assert(offsetof(Control, parent_control) == 0x14 && offsetof(Control, owner) == 0x34 && offsetof(Control, events) == 0x38 &&
              offsetof(Control, flags) == 0x44 && offsetof(Control, last_event) == 0x48 && offsetof(Control, click_sound) == 0x54, "control");
static_assert(offsetof(Navigable, neighbors) == 0x5C, "navigable");
static_assert(offsetof(Button, text) == 0x16C, "button text params");
static_assert(offsetof(TextParams, color) == 0x1C && offsetof(TextParams, default_color) == 0x2C &&
              offsetof(TextParams, flags) == 0x38 && offsetof(TextParams, text_object) == 0x50, "text params");
static_assert(offsetof(GuiText, gui_string) == 0x14 && offsetof(Panel, manager) == 0x18, "text/panel");
static_assert(offsetof(Manager, flags) == 0x1C && offsetof(Manager, tooltip_timer) == 0x44 && offsetof(Manager, tooltip_mode) == 0x4C &&
              offsetof(Manager, panels) == 0x88 && offsetof(Manager, modal_panels) == 0x94 && offsetof(Manager, sounds) == 0xA0 &&
              offsetof(Manager, sound_count) == 0xA4, "manager");
static_assert(offsetof(ClientInternal, mouse_y) == 0x3CC && sizeof(ClientInternal) == 0x4D4 && offsetof(Input, internal) == 4, "client/input");
static_assert(sizeof(InputEvent) == 16 && offsetof(InputEventDesc, id) == 0x10 && sizeof(InputEventList) == 12, "events");
}

// New shared cells proposed by this lane (no M40 owner yet); see input_cells.cpp.
// D45: 0078D1D4 owned by lane-renderer-bodies (renderer_gl_cells.cpp); borrowed here.
namespace kotor_renderer_gl_cells { extern std::uint32_t width_0078d1d4; }
// D22: 007A39F4 canonical type void* volatile, sole owner lane-input; readers static_cast.
namespace kotor_input_cells {
extern void* volatile gui_manager_007a39f4;
}
namespace kotor_input_hover {
inline Manager* gui_manager() { return static_cast<Manager*>(kotor_input_cells::gui_manager_007a39f4); }
}
extern "C" const MenuColor borrowed_exit_color_007a23b4;
extern "C" const MenuColor borrowed_highlight_color_0078d3c0;

extern const kotor_input_hover::ButtonVtable button_vtable_0073e658;
extern const kotor_input_hover::MainMenuEventRegistration main_menu_event_registrations_0067c682[24];
