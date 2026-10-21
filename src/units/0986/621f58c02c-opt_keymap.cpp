// lane-m1-opt-gameplay (fork: key mappings): CSWGuiInGameOptKeyMappings 006EDC90 (panel, table 00759358) and
// CSWGuiKeyMapButton 006EC630 (table 007593C8), opened by CSWGuiInGameGameplay::OnKeyMap 006E6420.
// Accept (006ED310) applies every changed binding to the live CExoInput event description (006ECE8B..BB) and writes
// "[Keymapping] <ActionId>=<key>" to .\swkotor.ini in the run cwd through CExoIni::WriteIniEntry (006ECEC2..F98).
// Data: keymap.2da ([[007A3A28]+B8]+68, CTwoDimArrays::Load2DArrays_Keymap 005C2400) and bindablekeys
// (+6C, Load2DArrays_BindableKey 005C24A0), both loaded at startup by the M54-linked rules-two-dim-arrays unit.
// Original: swkotor.exe GOG K1, static disassembly only.
#include "62dd3107f7-opt_keymap.h"
#include "99def0a2a7-input_system.h"     // lane-input: kih::EventDesc (CExoInputEventDesc view, +1C key, +30 alt key, [vt+4])
#include "716e85e6af-input_snapshot.h"   // M54 input-snapshot: InputView::SetCaptureKeyboardKeys (005DF720 owner)
#include "f89b4fe050-startup_views.h"    // M40 canonical kotor_menu_10_19::Tlk::simple_0041e8f0 (D44)
#include "186fd52797-base_ini.h"         // task-37 base_ini_reconstruction::PublicIni (CExoIni 005E6750 / 005E67D0 / 005E67E0)
#include <new>

using namespace kotor_m1_opt;
using kotor_reconstruction::StartupString;

namespace kotor_menu_window_cells { extern void* volatile input_007a39e4; }   // CExoInput* cell 007A39E4 (window-shared-cells)
extern "C" {
void* __fastcall gui_m48_input_get_event_005df4c0(void* input, void*, Word index, I32 input_class);   // m48b-gui-input 005DF4C0
int __fastcall kotor_fastcall_00414110(void* table, void*, StartupString* row, StartupString* column, std::int32_t* out);  // core47 C2DA::GetINTEntry
StartupString* __fastcall resman_string_from_int_005e5bc0(StartupString* self, void*, int value);    // lane-resman 005E5BC0
String* __fastcall kotor_fastcall_005ee280(void* app, void*, String* out, I32 key) noexcept(false); // m48t-f-token 005EE280
void __fastcall kotor_fastcall_00417ab0(gui_load::Object*, void*, gui_load::Word);                   // CSWGuiButton::Draw
void __fastcall gui_load_button_load_0041c930(gui_load::Button*, void*, gui_load::Gff*, gui_load::Struct*);
extern void* kotor_cell_rules_007a3a28;                                                             // CSWRules* (game-prepare)
// .data colours (values read from the image): 007A2390 {0, 0.64, 0.96}, 007A23FC {0.95, 0, 0.85}.
extern const MenuColor kotor_m1_color_007a2390 = {{0x00000000ul, 0x3F23D70Aul, 0x3F75C28Ful}};
extern const MenuColor kotor_m1_color_007a23fc = {{0x3F733333ul, 0x00000000ul, 0x3F59999Aul}};
extern const MenuColor kotor_m1_color_007a2384;                                                     // m1-saveload owner
}

namespace {
// .rdata constants of the image (read-only): key-id base [0074D3D8] = 6, Escape key id [0074D45C] = 1Fh.
constexpr Word kKeyBase_0074d3d8 = 6u;
constexpr Word kEscapeKey_0074d45c = 0x1Fu;
const char kIni[] = ".\\swkotor.ini";            // 0073D648
const char kKeymapping[] = "Keymapping";         // 0074E7C0

void* input() { return kotor_menu_window_cells::input_007a39e4; }
// 005DF720 CExoInput::SetCaptureKeyboardKeys (M54 owner InputView::SetCaptureKeyboardKeys, input-snapshot.obj).
void set_capture_keys(Word value) { static_cast<kotor_reconstruction::InputView*>(input())->SetCaptureKeyboardKeys(value); }
// [[007A3A28]+B8]+68 keymap.2da, +6C bindablekeys 2DA (CTwoDimArrays, 005C2462 / Load2DArrays_BindableKey).
void* keymap_2da() { return ptr_at(ptr_at(kotor_cell_rules_007a3a28, 0xB8), 0x68); }
void* bindable_2da() { return ptr_at(ptr_at(kotor_cell_rules_007a3a28, 0xB8), 0x6C); }
kotor_menu_10_19::Tlk* tlk() { return static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08); }
KeyMappings* panel_of(KeyMappingsHandlers* h) { return reinterpret_cast<KeyMappings*>(h); }
KeyMapButton* button_of(KeyMapButtonHandlers* h) { return reinterpret_cast<KeyMapButton*>(h); }
gui_load::EventFunc panel_handler(void (__thiscall KeyMappingsHandlers::*f)(kih::Control*)) { return static_cast<gui_load::EventFunc>(f); }
gui_load::EventFunc button_handler(void (__thiscall KeyMapButtonHandlers::*f)(kih::Control*)) { return static_cast<gui_load::EventFunc>(f); }

// CExoString hidden-return storage: constructed by the callee, destroyed by the caller (e.g. 006ED6CE..DA).
struct StringSlot {
    alignas(4) unsigned char bytes[sizeof(StartupString)];
    bool live = false;
    StartupString* raw() { return reinterpret_cast<StartupString*>(bytes); }
    String* get() { return std::launder(raw()); }
    ~StringSlot() { if (live) get()->~StartupString(); }
};

// CExoArrayList<CSWGuiKeyMapButton*> element access (data word +0, count +4, capacity +8).
KeyMapButton* button_at(const gui_load::Array& list, I32 index) {
    KeyMapButton* b; std::memcpy(&b, reinterpret_cast<const unsigned char*>(static_cast<std::uintptr_t>(list.data)) + 4 * index, 4);
    return b;
}
void set_button_at(gui_load::Array& list, I32 index, KeyMapButton* b) {
    std::memcpy(reinterpret_cast<unsigned char*>(static_cast<std::uintptr_t>(list.data)) + 4 * index, &b, 4);
}

// Border/text vslot +04 SetExtent; Button vslot +98 Initialize(extent, src) as typed in 0073E658 (Word args).
struct DrawableExtentView { const void* s00; void (__fastcall* set_extent)(void*, void*, const Extent*); };
struct ButtonInit98View { const void* s00_94[0x26]; void (__fastcall* initialize98)(Object*, void*, Word, Word); };
static_assert(offsetof(ButtonInit98View, initialize98) == 0x98, "+98");
// Control vslot +38 Draw(delta), +44 HitCheckMouse (PMF), +58 self_return; deleting dtor +00.
struct DrawView { const void* s00_34[0x0E]; void (__fastcall* draw)(Object*, void*, Word); };
static_assert(offsetof(DrawView, draw) == 0x38, "+38");
struct HitView { const void* s00_40[0x11]; kih::Control* (kih::Control::* hit)(int, int); };
static_assert(offsetof(HitView, hit) == 0x44, "+44");
struct Slot58View { const void* s00_54[0x16]; Object* (__fastcall* slot58)(Object*, void*); };
static_assert(offsetof(Slot58View, slot58) == 0x58, "+58");
struct DeleteView { void* (__fastcall* deleting_destroy)(Object*, void*, Word); };
// CSWGuiListBox vslot +A4 SetSelectOnLeftClick (0041B120, RET 4).
struct ListSelectView { const void* s00_a0[0x29]; void (__fastcall* select_on_left_click)(ListBox*, void*, Word); };
static_assert(offsetof(ListSelectView, select_on_left_click) == 0xA4, "+A4");

void copy_color(unsigned char* to, const MenuColor& color) { std::memcpy(to, color.words, 12); }
int get_int(void* table, String* row, const char* column, std::int32_t* out) {
    String name(column);                                                           // 005E5A90 (column temporary)
    return kotor_fastcall_00414110(table, nullptr, row, &name, out);               // 00414110 C2DA::GetINTEntry
}                                                                                  // 005E5C20
// SetDescription(GetActionId(id, type)) with the hidden-return temporary destroyed afterwards.
void describe(KeyMapButton* b) {
    StringSlot action;
    kotor_m1_opt_keymap_button_get_action_id_006ec120(b, nullptr, action.raw(), b->id, b->type); action.live = true;
    kotor_m1_opt_keymap_button_set_description_006ec7d0(b, nullptr, action.get());
}
// Inline CExoArrayList<T*>::Add growth (capacity ? capacity*2 : 10h; operator new 006FA7E6, copy, _free 006FA390).
void grow(gui_load::Array& list) {
    const I32 capacity = list.capacity ? list.capacity * 2 : 0x10;
    const Word old = list.data;
    list.capacity = capacity;
    void* fresh = c2da_directory_new_006fa7e6(static_cast<std::uint32_t>(capacity) * 4);
    list.data = word_of(fresh);
    for (I32 i = 0; i < list.size; ++i) set_button_at(list, i, [&] { KeyMapButton* b;
        std::memcpy(&b, reinterpret_cast<const unsigned char*>(static_cast<std::uintptr_t>(old)) + 4 * i, 4); return b; }());
    if (old) live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(old)));
}
}

// ======================================================================== D21 method table 00759358 (27 slots)
// Read from .rdata 00759358..007593C3: CSWGuiPanel 0073E010 except +00 006ECFB0, +38 006EE040, +3C 006EC510.
struct GuiM1OptKeyMappingsMethods {
    void* (__fastcall* s00)(KeyMappings*, void*, Word);                       // +00 006ECFB0
    void (__fastcall* s04)(Object*, void*, const Extent*);                    // +04 0040A5A0
    void (__fastcall* s08)(Object*, void*, Object*, Word);                    // +08 0040A630
    void (__fastcall* s0c)(Object*, void*, Word);                             // +0C 0060E760
    Word (__fastcall* s10)(Object*, void*, Word, Word);                       // +10 00409EB0
    void (__fastcall* s14)(Object*, void*);                                   // +14 004015A0
    void (__fastcall* s18)(Object*, void*);                                   // +18 004015A0
    void (__fastcall* s1c)(Object*, void*);                                   // +1C 004015A0
    void (__fastcall* s20)(Object*, void*);                                   // +20 004015A0
    void (__fastcall* s24)(Object*, void*);                                   // +24 004015A0
    Object* (__fastcall* s28)(Object*, void*);                                // +28 00641DB0
    Word (__fastcall* s2c)(Object*, void*);                                   // +2C 0063E7F0
    Word (__fastcall* s30)(Object*, void*);                                   // +30 0063E7F0
    void (__fastcall* s34)(Panel*, void*, Word);                              // +34 0040B760 CSWGuiPanel::Draw
    void (__fastcall* s38)(Object*, void*, Word);                             // +38 006EE040 Update
    void (__fastcall* s3c)(Object*, void*, I32, I32);                         // +3C 006EC510 HandleInputEvent
    Object* (__fastcall* s40)(Object*, void*, I32, I32);                      // +40 0040B690
    Object* (__fastcall* s44)(Object*, void*);                                // +44 0040B630
    void (__fastcall* s48)(Panel*, void*);                                    // +48 0040B870
    void (__fastcall* s4c)(Panel*, void*);                                    // +4C 0040C170
    void (__fastcall* s50)(Object*, void*, Word);                             // +50 0040B640
    void (__fastcall* s54)(Object*, void*, Word);                             // +54 0040B650
    void (__fastcall* s58)(Object*, void*, Word);                             // +58 0040B660
    void (__fastcall* s5c)(Object*, void*, Word);                             // +5C 0040B670
    void (__fastcall* s60)(Object*, void*, Word);                             // +60 0040B680
    String* (__fastcall* s64)(Panel*, void*, String*);                        // +64 0040A900
    void (__fastcall* s68)(Panel*, void*);                                    // +68 0040A9C0
};
static_assert(sizeof(GuiM1OptKeyMappingsMethods) == 0x6C, "00759358 slot count (27)");
extern "C" const GuiM1OptKeyMappingsMethods gui_m1_opt_keymap_methods_00759358 = {
    &kotor_m1_opt_keymap_deleting_destruct_006ecfb0,  // +00
    &gui_load_panel_set_extent_0040a5a0,              // +04
    &gui_load_panel_set_active_control_0040a630,      // +08
    &gui_load_slot_ret4_0060e760,                     // +0C
    &gui_load_slot_zero8_00409eb0,                    // +10
    &gui_load_slot_ret_004015a0,                      // +14
    &gui_load_slot_ret_004015a0,                      // +18
    &gui_load_slot_ret_004015a0,                      // +1C
    &gui_load_slot_ret_004015a0,                      // +20
    &gui_load_slot_ret_004015a0,                      // +24
    &gui_load_slot_self_00641db0,                     // +28
    &gui_load_slot_zero_0063e7f0,                     // +2C
    &gui_load_slot_zero_0063e7f0,                     // +30
    &kotor_panel_draw_0040B760,                       // +34
    &kotor_m1_opt_keymap_update_006ee040,             // +38
    &kotor_m1_opt_keymap_handle_input_006ec510,       // +3C
    &gui_load_panel_hit_check_0040b690,               // +40
    &gui_load_panel_active_control_0040b630,          // +44
    &gui_load_panel_on_added_0040b870,                // +48
    &gui_load_panel_on_removed_0040c170,              // +4C
    &gui_load_panel_button_a_0040b640,                // +50
    &gui_load_panel_button_b_0040b650,                // +54
    &gui_load_panel_button_x_0040b660,                // +58
    &gui_load_panel_button_y_0040b670,                // +5C
    &gui_load_panel_button_black_0040b680,            // +60
    &gui_load_panel_full_screen_bg_0040a900,          // +64
    &gui_load_panel_reset_font_0040a9c0,              // +68
};

// ======================================================================== D21 method table 007593C8 (40 slots)
// Read from .rdata 007593C8..00759467: CSWGuiButton 0073E658 except +00 006ED550, +04 006EC250, +38 006EC350,
// +44 006EC380.
struct GuiM1OptKeyMapButtonMethods {
    void* (__fastcall* s00)(KeyMapButton*, void*, Word);                                // +00 006ED550
    void (__fastcall* s04)(KeyMapButton*, void*, const Extent*);                        // +04 006EC250
    void (__fastcall* s08)(Object*, void*, Word, Word);                                 // +08 005B5E90
    void (kih::Navigable::* s0c)(int);                                                  // +0C 0041A980
    Word (__fastcall* s10)(Object*, void*, Word, Word);                                 // +10 00409EB0
    void (__fastcall* s14)(Object*, void*);                                             // +14 004015A0
    void (kih::Control::* s18)();                                                       // +18 0041AB90
    void (kih::Control::* s1c)();                                                       // +1C 0041ABD0
    void (__fastcall* s20)(Object*, void*);                                             // +20 004015A0
    void (__fastcall* s24)(Object*, void*);                                             // +24 0041AC70
    Word (__fastcall* s28)(Object*, void*);                                             // +28 0063E7F0
    Word (__fastcall* s2c)(Object*, void*);                                             // +2C 0063E7F0
    Object* (__fastcall* s30)(Object*, void*);                                          // +30 00641DB0
    Word (__fastcall* s34)(Object*, void*);                                             // +34 004064A0
    void (__fastcall* s38)(KeyMapButton*, void*, Word);                                 // +38 006EC350 Draw
    void (kih::Button::* s3c)(int, int);                                                // +3C 0041AD40 CSWGuiButton::HandleInputEvent
    void (kih::Button::* s40)(int);                                                     // +40 00418E00
    kih::Control* (kih::Control::* s44)(int, int);                                      // +44 006EC380 HitCheckMouse
    void (__fastcall* s48)(Button*, void*, gui_load::Gff*, gui_load::Struct*);          // +48 0041C930 CSWGuiButton::Load
    Object* (__fastcall* s4c)(Object*, void*);                                          // +4C 00641DB0
    Word (__fastcall* s50)(Object*, void*);                                             // +50 0063E7F0
    Word (__fastcall* s54)(Object*, void*);                                             // +54 0063E7F0
    Object* (__fastcall* s58)(Object*, void*);                                          // +58 00641DB0
    Word (__fastcall* s5c)(Object*, void*);                                             // +5C 0063E7F0
    Word (__fastcall* s60)(Object*, void*);                                             // +60 0063E7F0
    Word (__fastcall* s64)(Object*, void*);                                             // +64 0063E7F0
    Word (__fastcall* s68)(Object*, void*);                                             // +68 0063E7F0
    Word (__fastcall* s6c)(Object*, void*);                                             // +6C 0063E7F0
    Word (__fastcall* s70)(Object*, void*);                                             // +70 0063E7F0
    Word (__fastcall* s74)(Object*, void*);                                             // +74 0063E7F0
    Word (__fastcall* s78)(Object*, void*);                                             // +78 0063E7F0
    void (__fastcall* s7c)(Object*, void*, Word);                                       // +7C 00418960
    int (kih::Control::* s80)();                                                        // +80 004189D0
    void (__fastcall* s84)(Object*, void*);                                             // +84 0041AC30
    void (__fastcall* s88)(Object*, void*, Word);                                       // +88 00418DB0
    void (__fastcall* s8c)(Object*, void*);                                             // +8C 00418DF0
    Word (__fastcall* s90)(Object*, void*);                                             // +90 00418A90
    void (__fastcall* s94)(Object*, void*, Word, Word);                                 // +94 005B5E90
    void (__fastcall* s98)(Object*, void*, Word, Word);                                 // +98 00418D80
    void (__fastcall* s9c)(Object*, void*, Word, Word, Word, Word);                     // +9C 00417970
};
static_assert(sizeof(GuiM1OptKeyMapButtonMethods) == 0xA0, "007593C8 slot count (40)");
extern "C" const GuiM1OptKeyMapButtonMethods gui_m1_opt_keymap_button_methods_007593c8 = {
    &kotor_m1_opt_keymap_button_deleting_destruct_006ed550,   // +00
    &kotor_m1_opt_keymap_button_set_extent_006ec250,          // +04
    &gui_load_slot_ret8_005b5e90,                             // +08
    &kih::Navigable::HandleMouseOver,                         // +0C
    &gui_load_slot_zero8_00409eb0,                            // +10
    &gui_load_slot_ret_004015a0,                              // +14
    &kih::Control::HandleLMouseDown,                          // +18
    &kih::Control::HandleLMouseUp,                            // +1C
    &gui_load_slot_ret_004015a0,                              // +20
    &gui_load_control_handle_rmouse_up_0041ac70,              // +24
    &gui_load_slot_zero_0063e7f0,                             // +28
    &gui_load_slot_zero_0063e7f0,                             // +2C
    &gui_load_slot_self_00641db0,                             // +30
    &gui_load_slot_prop10_004064a0,                           // +34
    &kotor_m1_opt_keymap_button_draw_006ec350,                // +38
    &kih::Button::HandleInputEvent,                           // +3C
    &kih::Button::SetActive,                                  // +40
    static_cast<kih::Control* (kih::Control::*)(int, int)>(&KeyMapButtonHit::HitCheckMouse_006ec380),   // +44
    &gui_load_button_load_0041c930,                           // +48
    &gui_load_slot_self_00641db0,                             // +4C
    &gui_load_slot_zero_0063e7f0,                             // +50
    &gui_load_slot_zero_0063e7f0,                             // +54
    &gui_load_slot_self_00641db0,                             // +58
    &gui_load_slot_zero_0063e7f0,                             // +5C
    &gui_load_slot_zero_0063e7f0,                             // +60
    &gui_load_slot_zero_0063e7f0,                             // +64
    &gui_load_slot_zero_0063e7f0,                             // +68
    &gui_load_slot_zero_0063e7f0,                             // +6C
    &gui_load_slot_zero_0063e7f0,                             // +70
    &gui_load_slot_zero_0063e7f0,                             // +74
    &gui_load_slot_zero_0063e7f0,                             // +78
    &gui_load_control_focus_change_00418960,                  // +7C
    &kih::Control::GetIsSelectable,                           // +80
    &gui_load_control_rmouse_down_0041ac30,                   // +84
    &gui_load_button_set_enabled_00418db0,                    // +88
    &gui_load_button_reset_font_00418df0,                     // +8C
    &gui_load_control_tooltip_00418a90,                       // +90
    &gui_load_slot_ret8_005b5e90,                             // +94
    &gui_load_button_initialize_00418d80,                     // +98
    &gui_load_button_initialize_00417970,                     // +9C
};

namespace {
// Constructed-member stages of 006EDC90 (EH states 0..9, FuncInfo 00738B30).
enum Stage { kPanel = 0, kArray, kTitle, kDefault, kAccept, kCancel, kMove, kGame, kMini, kList };
void unwind_keymap(KeyMappings* self, int built) {
    if (built >= kList) gui_load_listbox_destruct_0041b060(&self->list, nullptr);
    if (built >= kMini) gui_load_button_destruct_00419da0(&self->btn_filter_mini, nullptr);
    if (built >= kGame) gui_load_button_destruct_00419da0(&self->btn_filter_game, nullptr);
    if (built >= kMove) gui_load_button_destruct_00419da0(&self->btn_filter_move, nullptr);
    if (built >= kCancel) gui_load_button_destruct_00419da0(&self->btn_cancel, nullptr);
    if (built >= kAccept) gui_load_button_destruct_00419da0(&self->btn_accept, nullptr);
    if (built >= kDefault) gui_load_button_destruct_00419da0(&self->btn_default, nullptr);
    if (built >= kTitle) gui_load_label_destruct_00419d10(&self->lbl_title, nullptr);
    if (built >= kArray) live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->buttons.data)));   // ~CExoArrayList
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
}

extern "C" {
// ======================================================================== 006EDC90 ctor (thiscall, RET 4)
gui_load::Panel* __fastcall kotor_m1_opt_keymap_construct_006edc90(void* memory, void*, Object* manager) noexcept(false) {
    KeyMappings* const self = static_cast<KeyMappings*>(memory);
    gui_load_panel_construct_0040b570(self, nullptr, manager);                    // 006EDCB6..BB
    int built = kPanel;
    try {
        set_vptr(self, &gui_m1_opt_keymap_methods_00759358);                       // 006EDCC2
        self->buttons.size = 0;                                                    // 006EDCCC (+68)
        self->buttons.capacity = 0;                                                // 006EDCCF (+6C)
        self->buttons.data = 0;                                                    // 006EDCD2 (+64)
        built = kArray;
        gui_load_label_construct_0041acd0(&self->lbl_title, nullptr);        built = kTitle;     // 006EDCD5..DD
        gui_load_button_construct_0041c0f0(&self->btn_default, nullptr);    built = kDefault;   // 006EDCE2..ED
        gui_load_button_construct_0041c0f0(&self->btn_accept, nullptr);     built = kAccept;    // 006EDCF2..FD
        gui_load_button_construct_0041c0f0(&self->btn_cancel, nullptr);     built = kCancel;    // 006EDD02..0D
        gui_load_button_construct_0041c0f0(&self->btn_filter_move, nullptr); built = kMove;     // 006EDD12..1F
        gui_load_button_construct_0041c0f0(&self->btn_filter_game, nullptr); built = kGame;     // 006EDD24..31
        gui_load_button_construct_0041c0f0(&self->btn_filter_mini, nullptr); built = kMini;     // 006EDD36..41
        gui_load_listbox_construct_0041be40(&self->list, nullptr);           built = kList;     // 006EDD46..51
        self->capturing = 0;                                                       // 006EDD56
        set_capture_keys(0);                                                       // 006EDD60..6D (005DF720)
        self->filter = 0;                                                          // 006EDD7D
        self->capture_button = nullptr;                                            // 006EDD83
        {
            Ref layout;
            layout.construct_00406d80("OPTKeyMapping");                            // 006EDD74..89 (00759528)
            gui_load_panel_start_layout_0040a680(self, nullptr, &layout);          // 006EDD8E..95
        }
        init_control(self, &self->lbl_title, "LBL_Title");                         // 006EDD9A..C8 (0075951C)
        init_control(self, &self->btn_default, "BTN_Default");                     // 006EDDCD..FE (00759510)
        init_control(self, &self->btn_accept, "BTN_Accept");                       // 006EDE03..34 (00756EA4)
        init_control(self, &self->btn_cancel, "BTN_Cancel");                       // 006EDE39..6A (00756EBC)
        init_control(self, &self->btn_filter_move, "BTN_Filter_Move");             // 006EDE6F..9A (00759500)
        init_control(self, &self->btn_filter_game, "BTN_Filter_Game");             // 006EDE9F..CA (007594F0)
        init_control(self, &self->btn_filter_mini, "BTN_Filter_Mini");             // 006EDECF..00 (007594E0)
        init_control(self, &self->list, "LST_EventList");                          // 006EDF05..36 (007594D0)
        gui_load_panel_stop_layout_0040b8f0(self, nullptr);                        // 006EDF3B..3D
        unsigned char* const bytes = reinterpret_cast<unsigned char*>(self);
        gui_load_text_params_set_color_00414e10(bytes + 0x868, nullptr, &borrowed_enter_color_007a23c0);   // 006EDF42..4D
        kotor_m1_opt_keymap_fill_list_with_events_006ed930(self, nullptr);         // 006EDF52..54
        kotor_m1_opt_keymap_fill_list_box_006ecfd0(self, nullptr, self->filter);   // 006EDF59..62
        kih::GuiObject* const target = event_target(self);
        gui_load_control_add_event_0041ab20(&self->btn_default, nullptr, 0x27, target,
            panel_handler(&KeyMappingsHandlers::OnDefaultClick_006ed2d0));         // 006EDF67..75
        gui_load_control_add_event_0041ab20(&self->btn_accept, nullptr, 0x27, target,
            panel_handler(&KeyMappingsHandlers::OnAcceptClick_006ed310));          // 006EDF7A..88
        gui_load_control_add_event_0041ab20(&self->btn_cancel, nullptr, 0x27, target,
            kotor_m1_shared::event_func(&kotor_m1_shared::PanelEventThunks::OnBButtonPressed_00624bb0));   // 006EDF8D..9B
        gui_load_control_add_event_0041ab20(&self->btn_filter_move, nullptr, 0x27, target,
            panel_handler(&KeyMappingsHandlers::OnFilterMove_006ed390));           // 006EDFA0..AA
        gui_load_control_add_event_0041ab20(&self->btn_filter_move, nullptr, 1, target,
            panel_handler(&KeyMappingsHandlers::FixFilterColors_006ec580));        // 006EDFAF..B9
        self->btn_filter_move.flags &= 0xFFFFFFFBu;                                // 006EDFBE..CE
        gui_load_control_add_event_0041ab20(&self->btn_filter_game, nullptr, 0x27, target,
            panel_handler(&KeyMappingsHandlers::OnFilterGame_006ed3e0));           // 006EDFC1..D1
        gui_load_control_add_event_0041ab20(&self->btn_filter_game, nullptr, 1, target,
            panel_handler(&KeyMappingsHandlers::FixFilterColors_006ec580));        // 006EDFD6..E0
        self->btn_filter_game.flags &= 0xFFFFFFFBu;                                // 006EDFE5..F4
        gui_load_control_add_event_0041ab20(&self->btn_filter_mini, nullptr, 0x27, target,
            panel_handler(&KeyMappingsHandlers::OnFilterMini_006ed430));           // 006EDFEF..002
        gui_load_control_add_event_0041ab20(&self->btn_filter_mini, nullptr, 1, target,
            panel_handler(&KeyMappingsHandlers::FixFilterColors_006ec580));        // 006EE007..11
        self->btn_filter_mini.flags &= 0xFFFFFFFBu;                                // 006EE016
        gui_load_panel_set_active_control_0040a630(self, nullptr, &self->list, 0); // 006EE019..24
    } catch (...) {
        unwind_keymap(self, built);
        throw;
    }
    return self;                                                                   // 006EE02E
}

// ======================================================================== 006EC3D0 dtor (thiscall, RET)
// vptr 00759358 (006EC3EE); ClearItems; delete every button from the last (vslot +00 with 1, DelIndex inline
// 006EC411..59); members in reverse; free the list storage (006EC4D5..E8); ~CSWGuiPanel.
void __fastcall kotor_m1_opt_keymap_destruct_006ec3d0(KeyMappings* self, void*) {
    set_vptr(self, &gui_m1_opt_keymap_methods_00759358);                          // 006EC3EE
    gui_load_listbox_clear_items_00419a90(&self->list, nullptr);                   // 006EC3F4..404
    while (self->buttons.size != 0) {                                              // 006EC409..0E / 006EC454..59
        const I32 index = self->buttons.size - 1;                                  // 006EC411..14
        KeyMapButton* const b = button_at(self->buttons, index);                   // 006EC417..1A
        if (b) vt<DeleteView>(b)->deleting_destroy(b, nullptr, 1);                 // 006EC41D..25
        set_button_at(self->buttons, index, nullptr);                              // 006EC427..2A
        self->buttons.size = self->buttons.size - 1;                               // 006EC431..3B
        for (I32 i = index; i < self->buttons.size; ++i)                           // 006EC437..52 (no-op for the tail)
            set_button_at(self->buttons, i, button_at(self->buttons, i + 1));
    }
    gui_load_listbox_destruct_0041b060(&self->list, nullptr);                      // 006EC45C..63
    gui_load_button_destruct_00419da0(&self->btn_filter_mini, nullptr);            // 006EC468..73
    gui_load_button_destruct_00419da0(&self->btn_filter_game, nullptr);            // 006EC478..83
    gui_load_button_destruct_00419da0(&self->btn_filter_move, nullptr);            // 006EC488..93
    gui_load_button_destruct_00419da0(&self->btn_cancel, nullptr);                 // 006EC498..A3
    gui_load_button_destruct_00419da0(&self->btn_accept, nullptr);                 // 006EC4A8..B3
    gui_load_button_destruct_00419da0(&self->btn_default, nullptr);                // 006EC4B8..C3
    gui_load_label_destruct_00419d10(&self->lbl_title, nullptr);                   // 006EC4C8..D0
    live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->buttons.data)));   // 006EC4D5..DE
    self->buttons.data = 0;                                                        // 006EC4E8
    gui_load_panel_destruct_0040cf70(self, nullptr);                               // 006EC4E6..F7
}
// ======================================================================== 006ECFB0 scalar deleting dtor (RET 4)
void* __fastcall kotor_m1_opt_keymap_deleting_destruct_006ecfb0(KeyMappings* self, void*, Word flags) {
    kotor_m1_opt_keymap_destruct_006ec3d0(self, nullptr);                          // 006ECFB3
    if (flags & 1u) live_free(self);                                               // 006ECFB8..C5
    return self;                                                                   // 006ECFC8
}

// ======================================================================== 006EC510 HandleInputEvent (RET 8)
// pressed && (28h || 2Eh): stop capturing, PlayGuiSound(0), PopModalPanel, state 400h, return WITHOUT the base call;
// otherwise CSWGuiPanel::HandleInputEvent 00409E60.
void __fastcall kotor_m1_opt_keymap_handle_input_006ec510(Object* object, void*, I32 event, I32 pressed) {
    KeyMappings* const self = static_cast<KeyMappings*>(static_cast<Panel*>(object));
    if (pressed != 0 && (event == 0x28 || event == 0x2E)) {                        // 006EC514..27
        self->capturing = 0;                                                       // 006EC529
        set_capture_keys(0);                                                       // 006EC533..3B
        manager_of(self)->PlayGuiSound(0);                                         // 006EC540..45
        kotor_fastcall_0040be00(self->manager, nullptr);                           // 006EC54A..4D PopModalPanel
        self->flags = (self->flags & 0xFFFFFCFFu) | 0x400u;                        // 006EC552..5F
        return;                                                                    // 006EC562
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, pressed);           // 006EC566..6A
}

// ======================================================================== 006EE040 Update (vslot +38, RET 4)
void __fastcall kotor_m1_opt_keymap_update_006ee040(Object* object, void*, Word) {
    KeyMappings* const self = static_cast<KeyMappings*>(static_cast<Panel*>(object));
    if (self->capturing != 1) return;                                              // 006EE043..4A
    const Word key = kotor_m1_opt_input_last_captured_key_005df630(input(), nullptr);   // 006EE04C..52
    if (key == 0 || self->capture_button == nullptr) return;                       // 006EE057..63
    if (key == kEscapeKey_0074d45c) {                                              // 006EE065..6B ([0074D45C])
        kotor_m1_opt_keymap_set_capture_event_006ed480(self, nullptr, nullptr);    // 006EE06D..71
        manager_of(self)->PlayGuiSound(0);                                         // 006EE076..7B
        return;
    }
    const I32 result = kotor_m1_opt_keymap_button_update_event_006edbd0(self->capture_button, nullptr, key);   // 006EE084..85
    if (result == 0) {                                                             // 006EE08A..8D -> 006EE0A2
        kotor_m1_opt_keymap_set_capturing_006ec3b0(self, nullptr, 0);              // 006EE0A2..A6
        manager_of(self)->PlayGuiSound(0);                                         // 006EE0AB..B0
        kotor_m1_opt_keymap_set_capture_event_006ed480(self, nullptr, nullptr);    // 006EE0B5..B9
    } else if (result == 1 || result == 2) {                                       // 006EE08F..93
        kotor_m1_opt_keymap_set_capturing_006ec3b0(self, nullptr, 1);              // 006EE095..99
    }
}
// ======================================================================== 006EC3B0 (thiscall, RET 4)
// [+F2C] = v; tail-jump CExoInput::SetCaptureKeyboardKeys([007A39E4], v).
void __fastcall kotor_m1_opt_keymap_set_capturing_006ec3b0(KeyMappings* self, void*, Word value) {
    self->capturing = value;                                                       // 006EC3B0..B4
    set_capture_keys(value);                                                       // 006EC3BA..C4
}

// ======================================================================== 006ED480 SetCaptureEvent(button) (RET 4)
void __fastcall kotor_m1_opt_keymap_set_capture_event_006ed480(KeyMappings* self, void*, KeyMapButton* button) {
    KeyMapButton* const previous = self->capture_button;                           // 006ED484
    if (button != previous) {                                                      // 006ED48F..91
        if (previous) {                                                            // 006ED493..95
            kotor_m1_opt_keymap_button_update_color_006ec6e0(previous, nullptr);   // 006ED497..99
            gui_load_border_params_set_pulsing_00414d00(reinterpret_cast<unsigned char*>(previous) + 0x248, nullptr,
                                                        0, 0, 0);                  // 006ED49E..AA (event button border params)
        }
        vt<ListSelectView>(&self->list)->select_on_left_click(&self->list, nullptr, 0);   // 006ED4AF..BD vslot +A4
        self->capturing = 0;                                                       // 006ED4C3
        set_capture_keys(0);                                                       // 006ED4CD..D5
    }
    if (button == nullptr) return;                                                 // 006ED4DA..DC
    self->capture_button = button;                                                 // 006ED4DE
    copy_color(reinterpret_cast<unsigned char*>(button) + 0x258, borrowed_enter_color_007a23c0);   // 006ED4E4..504
    gui_load_border_params_set_pulsing_00414d00(reinterpret_cast<unsigned char*>(button) + 0x248, nullptr, 1, 1, 1);   // 006ED500..0F
    vt<ListSelectView>(&self->list)->select_on_left_click(&self->list, nullptr, 1);       // 006ED514..22
    self->capturing = 1;                                                           // 006ED529
    set_capture_keys(1);                                                           // 006ED535..43 (tail jump)
}

// ======================================================================== 006ED930 FillListWithEvents (RET)
// Count reset (no release); seven event ids 118h..11Eh twice (type 2 then 1), then 42h ids C8h.. with type 0
// (CreateOption(id, LST_EventList proto [+F14], type)); non-null results are appended.
void __fastcall kotor_m1_opt_keymap_fill_list_with_events_006ed930(KeyMappings* self, void*) noexcept(false) {
    self->buttons.size = 0;                                                        // 006ED938
    Control* const proto = self->list.proto;                                       // 006ED93F (+F14)
    for (I32 id = 0x118, n = 7; n != 0; ++id, --n) {                               // 006ED94A..E5
        for (I32 type = 2; type >= 1; --type) {                                    // 006ED95B (2) / 006ED99D (1)
            KeyMapButton* const b = kotor_m1_opt_keymap_create_option_006ed610(self, nullptr, id, proto, type);
            if (!b) continue;                                                      // 006ED968..6A / 006ED9AA..AC
            if (self->buttons.size == self->buttons.capacity)                      // 006ED96C..77
                menu_list_growth::allocate_005e03c0(&self->buttons, nullptr,
                    static_cast<Word>(self->buttons.capacity ? self->buttons.capacity * 2 : 0x10));   // 006ED979..89
            set_button_at(self->buttons, self->buttons.size, b);                   // 006ED98E..93
            self->buttons.size = self->buttons.size + 1;                           // 006ED996
        }
    }
    for (I32 i = 0; i < 0x42; ++i) {                                               // 006ED9EB..6E
        KeyMapButton* const b = kotor_m1_opt_keymap_create_option_006ed610(self, nullptr, i + 0xC8, proto, 0);   // 006ED9F0..A00
        if (!b) continue;                                                          // 006EDA07..09
        if (self->buttons.size == self->buttons.capacity) grow(self->buttons);     // 006EDA0B..5B (inline Add growth)
        set_button_at(self->buttons, self->buttons.size, b);                       // 006EDA5E..64
        self->buttons.size = self->buttons.size + 1;                               // 006EDA67
    }
}

// ======================================================================== 006ECFD0 FillListBox(filter) (RET 4)
// Local CExoArrayList ordered by sort position (stable: first element with a strictly greater key), then
// ClearItems + AddControls(&list, 0, 1, 0) (0041C1D0), select-flag bit 40h cleared, local storage freed.
void __fastcall kotor_m1_opt_keymap_fill_list_box_006ecfd0(KeyMappings* self, void*, I32 filter) noexcept(false) {
    struct Local { KeyMapButton** data = nullptr; I32 count = 0; I32 capacity = 0; } local;   // 006ECFEB..FD
    struct Release { Local* l; ~Release() { live_free(l->data); } } release{&local};           // EH state 0 (00738.. funclet: _free)
    auto add_room = [&local]() {                                                   // 006ED06B..C1 / 006ED0DF..32
        const I32 capacity = local.capacity ? local.capacity * 2 : 0x10;
        KeyMapButton** const old = local.data;
        local.capacity = capacity;
        local.data = static_cast<KeyMapButton**>(c2da_directory_new_006fa7e6(static_cast<std::uint32_t>(capacity) * 4));
        for (I32 i = 0; i < local.count; ++i) local.data[i] = old[i];
        if (old) live_free(old);
    };
    for (I32 i = 0; i < self->buttons.size; ++i) {                                 // 006ED001..181
        KeyMapButton* const b = button_at(self->buttons, i);                       // 006ED020..23
        if (b->page != filter) continue;                                           // 006ED026..30
        if (local.count <= 0) {                                                    // 006ED03E..40 -> append
            if (local.count == local.capacity) add_room();                         // 006ED067..C1
            local.data[local.count] = b;                                           // 006ED0C4..C8
            local.count = local.count + 1;                                         // 006ED0CB..D0
            continue;
        }
        I32 at = local.count;                                                      // 006ED058..5E (no greater key: end)
        for (I32 j = 0; j < local.count; ++j)                                      // 006ED042..58
            if (local.data[j]->sort_pos > b->sort_pos) { at = j; break; }          // 006ED049..5C
        const I32 old_count = local.count;                                         // 006ED0DB (edi)
        if (local.count == local.capacity) add_room();                             // 006ED0D9..132
        local.data[local.count] = b;                                               // 006ED135..39
        local.count = local.count + 1;                                             // 006ED13C..43
        for (I32 k = old_count; k > at; --k) local.data[k] = local.data[k - 1];    // 006ED141..5E
        local.data[at] = b;                                                        // 006ED160..68
    }
    gui_load_listbox_clear_items_00419a90(&self->list, nullptr);                   // 006ED188..90
    kotor_fastcall_0041c1d0(&self->list, nullptr, &local, 0, 1, 0);                // 006ED195..A2 AddControls(&list, 0, 1, 0)
    self->list.lb_flags &= 0xFFFFFFBFu;                                            // 006ED1A7
}                                                                                  // 006ED1AE..BB _free(local.data)

// ======================================================================== 006ED610 CreateOption(id, proto, type) (RET 0Ch)
KeyMapButton* __fastcall kotor_m1_opt_keymap_create_option_006ed610(KeyMappings* self, void*, I32 id, Control* proto,
                                                                   I32 type) noexcept(false) {
    Extent extent{0, 0, 0, 0};                                                     // 006ED63A..46 ([esp+38h])
    void* const memory = c2da_directory_new_006fa7e6(0x3B8);                       // 006ED633..4A
    KeyMapButton* b = nullptr;                                                     // 006ED630 (esi = 0)
    if (memory) {
        try { b = kotor_m1_opt_keymap_button_construct_006ec630(memory, nullptr, self); }   // 006ED65E..66
        catch (...) { live_free(memory); throw; }                                  // EH state 0: _free
    }
    // No null check in the original (006ED671 reads [esi]); a failed allocation faults here as in the original.
    b->id = id;                                                                    // 006ED683
    b->type = type;                                                                // 006ED689
    vt<ButtonInit98View>(b)->initialize98(b, nullptr, word_of(&extent), word_of(proto));   // 006ED671..8F vslot +98
    vt<ButtonInit98View>(&b->event_button)->initialize98(&b->event_button, nullptr,
                                                          word_of(&extent), word_of(proto));   // 006ED695..AA
    {
        StringSlot number;
        resman_string_from_int_005e5bc0(number.raw(), nullptr, id); number.live = true;   // 006ED6B0..B5 (005E5BC0)
        gui_load_text_params_set_text_00415e00(reinterpret_cast<unsigned char*>(b) + 0x334, nullptr, number.get());   // 006ED6BA..C9
    }                                                                              // 006ED6CE..DA
    StringSlot action;
    kotor_m1_opt_keymap_button_get_action_id_006ec120(b, nullptr, action.raw(), id, type); action.live = true;   // 006ED6DF..E8
    // D2: [esp+1Ch] ("ReMappable") and [esp+10h] ("SortPos") have no initializing store in the original
    // (006ED702/006ED70A preset only ForceDisplay = 0 and Disabled = -1); value-initialized to 0 when the
    // 2DA lookup leaves them unwritten (006ED726, 006ED7F0).
    std::int32_t remappable = 0;                                                   // D2 006ED726
    std::int32_t force_display = 0;                                                // 006ED702
    std::int32_t disabled = -1;                                                    // 006ED70A
    std::int32_t sort_pos = 0;                                                     // D2 006ED7F0
    get_int(keymap_2da(), action.get(), "ReMappable", &remappable);                // 006ED6F5..745 (007594B0)
    get_int(keymap_2da(), action.get(), "ForceDisplay", &force_display);           // 006ED74A..789 (007594A0)
    get_int(keymap_2da(), action.get(), "Disabled", &disabled);                    // 006ED78E..7CE (0074E7CC)
    get_int(keymap_2da(), action.get(), "SortPos", &sort_pos);                     // 006ED7D3..813 (00759498)
    b->sort_pos = sort_pos;                                                        // 006ED818..1C
    if ((remappable == 0 && force_display == 0) || disabled == 1) {                // 006ED822..37
        vt<DeleteView>(b)->deleting_destroy(b, nullptr, 1);                        // 006ED839..3F vslot +00(1)
        return nullptr;                                                            // 006ED84E
    }
    kotor_m1_opt_keymap_button_set_description_006ec7d0(b, nullptr, action.get()); // 006ED864..6B
    const gui_load::BorderParams* const border = reinterpret_cast<const gui_load::BorderParams*>(
        reinterpret_cast<unsigned char*>(b) + 0x80);                               // +6C border params
    const gui_load::BorderParams* const hilight = reinterpret_cast<const gui_load::BorderParams*>(
        reinterpret_cast<unsigned char*>(b) + 0xF4);                               // +E0 hilight params
    const I32 d_border = gui_load_border_params_dim_00414cd0(border, nullptr);    // 006ED870..7C
    const I32 d_hilight = gui_load_border_params_dim_00414cd0(hilight, nullptr);  // 006ED881..85
    const I32 dim = gui_load_border_params_dim_00414cd0(
        d_hilight > d_border ? hilight : border, nullptr);   // 006ED88A..96
    gui_load::Text* const text = &b->text;                                         // +154
    const Word ideal = vt<TextIdealView>(text)->ideal_height(text, nullptr);       // 006ED89B..A9 vslot +08
    extent.height = static_cast<I32>((ideal + static_cast<Word>(dim)) << 1);                          // 006ED8AC..B4
    vt<DrawableExtentView>(b)->set_extent(b, nullptr, &extent);                    // 006ED8B0..BD vslot +04
    kih::GuiObject* const target = event_target(b);                                // event object = the button (006ED8C5)
    gui_load_control_add_event_0041ab20(b, nullptr, 0x27, target,
        button_handler(&KeyMapButtonHandlers::onEventClicked_006ed5e0));           // 006ED8C0..CA
    gui_load_control_add_event_0041ab20(b, nullptr, 0, target,
        button_handler(&KeyMapButtonHandlers::onControlEntered_006ed5c0));         // 006ED8CF..D9
    gui_load_control_add_event_0041ab20(b, nullptr, 1, target,
        button_handler(&KeyMapButtonHandlers::VoidReturn_0060e760));               // 006ED8DE..E8 (0060E760)
    if (force_display != 0)                                                        // 006ED8ED..F3
        kotor_m1_opt_keymap_button_set_unchangable_006ec770(b, nullptr);           // 006ED8F5..F7
    return b;                                                                      // 006ED8FC..912 (~action)
}

// ======================================================================== 006EDA80 UpdateEvent(id, type, key, flags) (RET 10h)
// 0: key bindable and not in use, 1: bindable and IsKeyUsedAlready, 2: not bindable (bindablekeys "KEY%d"/"Bindable").
I32 __fastcall kotor_m1_opt_keymap_update_event_006eda80(KeyMappings* self, void*, I32 id, I32 type, Word key,
                                                         Word class_flags) noexcept(false) {
    String row;                                                                    // 006EDA9D..A1 (005B3190)
    kotor_reconstruction::StartupFormat005e5680(&row, "KEY%d", static_cast<I32>(key - kKeyBase_0074d3d8));   // 006EDAA6..C7 (007594C8)
    std::int32_t bindable = 0;                                                     // 006EDAD8
    const bool ok = get_int(bindable_2da(), &row, "Bindable", &bindable) != 0 && bindable != 0;   // 006EDACF..26 (007594BC)
    if (!ok) return 2;                                                             // 006EDB98..BF
    if (kotor_m1_opt_keymap_is_key_used_006ed1e0(self, nullptr, key, id, type, class_flags) != 0)   // 006EDB38..4F
        return 1;                                                                  // 006EDB5F..7A
    return 0;                                                                      // 006EDB7D..95
}

// ======================================================================== 006ED1E0 IsKeyUsedAlready(key, id, type, flags) (RET 10h)
// Every other option of an overlapping input class bound to the key is cleared (changed = 1, new key = 0),
// re-described and re-coloured; always returns 0 (006ED2B1).
I32 __fastcall kotor_m1_opt_keymap_is_key_used_006ed1e0(KeyMappings* self, void*, Word key, I32 id, I32 type,
                                                        Word class_flags) noexcept(false) {
    for (I32 i = 0; i < self->buttons.size; ++i) {                                 // 006ED1FC..2A4
        KeyMapButton* const b = button_at(self->buttons, i);                       // 006ED210..13
        if (b->id == id && b->type == type) continue;                              // 006ED216..2C
        if ((b->class_flags & class_flags) == 0) continue;                         // 006ED22E..38
        const Word bound = b->changed != 0 ? b->new_key : b->orig_key;             // 006ED23A..4A
        if (bound != key) continue;                                                // 006ED250..54
        b->changed = 1;                                                            // 006ED265
        b->new_key = 0;                                                            // 006ED26F
        describe(b);                                                               // 006ED256..92
        kotor_m1_opt_keymap_button_update_color_006ec6e0(b, nullptr);              // 006ED297..99
    }
    return 0;                                                                      // 006ED2B1
}

// ======================================================================== CSWGuiKeyMapButton
// ---- 006EC630 ctor(owner) (thiscall, RET 4; EH state 0 = ~CSWGuiButton(this))
KeyMapButton* __fastcall kotor_m1_opt_keymap_button_construct_006ec630(void* memory, void*, KeyMappings* owner) noexcept(false) {
    KeyMapButton* const self = static_cast<KeyMapButton*>(memory);
    gui_load_button_construct_0041c0f0(self, nullptr);                             // 006EC64F
    try {
        set_vptr(self, &gui_m1_opt_keymap_button_methods_007593c8);                // 006EC662
        gui_load_button_construct_0041c0f0(&self->event_button, nullptr);          // 006EC654..68
        try {
            self->owner = owner;                                                   // 006EC674
            self->orig_key = 0xFFFFFFFFu;                                          // 006EC67A
            self->sort_pos = -1;                                                   // 006EC680
            self->event_button.owner = reinterpret_cast<Object*>(owner);           // 006EC686 (+1FC)
            self->id = 0;                                                          // 006EC695
            self->changed = 0;                                                     // 006EC69B
            self->new_key = 0;                                                     // 006EC6A1
            self->unchangeable = 0;                                                // 006EC6A7
            self->page = 0;                                                        // 006EC6AD
            self->class_flags = 0;                                                 // 006EC6B3
            self->event_button.parent = self;                                      // 006EC6B9 (+1DC)
            Object* const child = vt<Slot58View>(&self->event_button)->slot58(&self->event_button, nullptr);   // 006EC68C..BF vslot +58
            gui_load_control_add_child_0041acc0(self, nullptr, static_cast<Control*>(child));   // 006EC6C2..C5
        } catch (...) { gui_load_button_destruct_00419da0(&self->event_button, nullptr); throw; }   // EH state 1
    } catch (...) { gui_load_button_destruct_00419da0(self, nullptr); throw; }     // EH state 0
    return self;                                                                   // 006EC6CF
}
// ---- 006ED570 dtor (no vptr store): ~CSWGuiButton(+1C8), ~CSWGuiButton(this)
void __fastcall kotor_m1_opt_keymap_button_destruct_006ed570(KeyMapButton* self, void*) {
    gui_load_button_destruct_00419da0(&self->event_button, nullptr);               // 006ED58D..9B
    gui_load_button_destruct_00419da0(self, nullptr);                              // 006ED5A0..AA
}
// ---- 006ED550 scalar deleting dtor (RET 4)
void* __fastcall kotor_m1_opt_keymap_button_deleting_destruct_006ed550(KeyMapButton* self, void*, Word flags) {
    kotor_m1_opt_keymap_button_destruct_006ed570(self, nullptr);                   // 006ED553
    if (flags & 1u) live_free(self);                                               // 006ED558..65
    return self;                                                                   // 006ED568
}
// ---- 006EC250 SetExtent(extent) (RET 4): own extent {x, y, w - C5h, h} to border/hilight; text = intersection of
// the inner extents; event button {x + (w - C5h) + 2, y, C3h, h}.
void __fastcall kotor_m1_opt_keymap_button_set_extent_006ec250(KeyMapButton* self, void*, const Extent* extent) {
    Extent local;
    local.left = extent->left;                                                     // 006EC25F / 006EC27B
    local.top = extent->top;                                                       // 006EC261 / 006EC296
    local.width = extent->width - 0xC5;                                            // 006EC264..70 / 006EC28D
    local.height = extent->height;                                                 // 006EC267 / 006EC27F
    self->extent = local;                                                          // 006EC26A..83 (+4..+10)
    vt<DrawableExtentView>(&self->border)->set_extent(&self->border, nullptr, &local);     // 006EC28A..9A (+6C)
    vt<DrawableExtentView>(&self->hilight)->set_extent(&self->hilight, nullptr, &local);   // 006EC29D..B0 (+E0)
    Extent hilight_inner, border_inner, text_extent;
    gui_load_border_inner_extent_00415240(&self->hilight, nullptr, &hilight_inner);        // 006EC2B9..C4
    gui_load_border_inner_extent_00415240(&self->border, nullptr, &border_inner);          // 006EC2CA..D1
    gui_load_extent_intersection_0040a4a0(&text_extent, &border_inner, &hilight_inner);    // 006EC2D6..E1
    vt<DrawableExtentView>(&self->text)->set_extent(&self->text, nullptr, &text_extent);   // 006EC2B3 / 006EC2E4..EF (+154)
    Extent child;
    child.left = local.left + local.width + 2;                                     // 006EC305..1F
    child.top = extent->top;                                                       // 006EC2F4 / 006EC309
    child.width = 0xC3;                                                            // 006EC32E
    child.height = extent->height;                                                 // 006EC2FE / 006EC311
    vt<DrawableExtentView>(&self->event_button)->set_extent(&self->event_button, nullptr, &child);   // 006EC319..36 (+1C8)
}
// ---- 006EC350 Draw(delta) (RET 4): CSWGuiButton::Draw, then the event button's vslot +38.
void __fastcall kotor_m1_opt_keymap_button_draw_006ec350(KeyMapButton* self, void*, Word delta) {
    kotor_fastcall_00417ab0(self, nullptr, delta);                                 // 006EC352..59
    vt<DrawView>(&self->event_button)->draw(&self->event_button, nullptr, delta);  // 006EC35E..6B
}
// ---- 006EC120 GetActionId(out, id, type) (thiscall RET 0Ch; `this` unused): "Action<id>", "Action<id>B" (type 1,
// 0075934C), "Action<id>A" (type 2, 00759344); other types: empty. Copied into the caller's hidden return slot.
String* __fastcall kotor_m1_opt_keymap_button_get_action_id_006ec120(KeyMapButton*, void*, String* out, I32 id,
                                                                      I32 type) noexcept(false) {
    String s;                                                                      // 006EC138..44 (005B3190)
    if (type == 0) {                                                               // 006EC149..57
        StringSlot number;
        resman_string_from_int_005e5bc0(number.raw(), nullptr, id); number.live = true;   // 006EC159..62
        String action("Action");                                                   // 006EC167..75 (0074E7D8)
        StringSlot sum;
        GuiConcat005e5d10(&action, nullptr, sum.raw(), number.get()); sum.live = true;    // 006EC17A..8B
        s.Assign005e5c50(sum.get());                                               // 006EC190..9A
    } else if (type == 1) {                                                        // 006EC1CB..CE
        kotor_reconstruction::StartupFormat005e5680(&s, "%s%dB", "Action", id);    // 006EC1D0..FF (0075934C)
    } else if (type == 2) {                                                        // 006EC1E6..E9
        kotor_reconstruction::StartupFormat005e5680(&s, "%s%dA", "Action", id);    // 006EC1EB..FF (00759344)
    }
    WindowCopy005e5b00(out, nullptr, &s);                                          // 006EC207..13 (into the hidden return)
    return out;                                                                    // 006EC232 (~s 006EC218..29)
}
// ---- 006EC6E0 UpdateColor: border (+90) and event-button border (+258) colour = changed ? 007A23FC : 007A2390.
void __fastcall kotor_m1_opt_keymap_button_update_color_006ec6e0(KeyMapButton* self, void*) {
    unsigned char* const bytes = reinterpret_cast<unsigned char*>(self);
    const MenuColor& color = self->changed != 0 ? kotor_m1_color_007a23fc : kotor_m1_color_007a2390;   // 006EC6E0..E8
    copy_color(bytes + 0x90, color);                                               // 006EC6EA..07 / 006EC729..43
    copy_color(bytes + 0x258, color);                                              // 006EC70A..25 / 006EC746..62
}
// ---- 006EC770 SetUnchangable: [+3A4] = 1; border (+90) and hilight (+104) colour 007A2384; text colour 007A2408.
void __fastcall kotor_m1_opt_keymap_button_set_unchangable_006ec770(KeyMapButton* self, void*) {
    unsigned char* const bytes = reinterpret_cast<unsigned char*>(self);
    self->unchangeable = 1;                                                        // 006EC773
    copy_color(bytes + 0x90, kotor_m1_color_007a2384);                             // 006EC77D..9A
    gui_load_text_params_set_color_00414e10(bytes + 0x16C, nullptr, &kotor_m1_color_007a2408);   // 006EC79D..A8
    copy_color(bytes + 0x104, kotor_m1_color_007a2384);                            // 006EC7AD..C9
}
// ---- 006EC7D0 SetDescription(CExoString* row) (RET 4)
void __fastcall kotor_m1_opt_keymap_button_set_description_006ec7d0(KeyMapButton* self, void*, String* row) noexcept(false) {
    void* const table = keymap_2da();
    // D2: [esp+14h] (ActionStrRef) has no initializing store in the original; value-initialized to 0 for a
    // failed lookup (006EC812, read at 006ECB57).
    std::int32_t action_strref = 0;
    get_int(table, row, "ActionStrRef", &action_strref);                           // 006EC7F3..838 (0075947C)
    std::int32_t desc = static_cast<std::int32_t>(self->desc_strref);
    get_int(table, row, "DescStrRef", &desc);                                      // 006EC83D..881 (00759470) -> +1C4
    self->desc_strref = static_cast<Word>(desc);
    self->u24 = self->desc_strref;                                                 // 006EC886..91 (+24 tooltip strref)
    self->event_button.u24 = self->desc_strref;                                    // 006EC894 (+1EC)
    get_int(table, row, "Page", &self->page);                                      // 006EC888..8D6 (00759468) -> +3AC
    std::int32_t value = 0;                                                        // 006EC8E6 (preset once)
    self->class_flags = 0;                                                         // 006EC8EA
    struct Class { const char* column; Word bit; };
    static const Class classes[6] = {
        {"ICPC", 1u}, {"ICMiniGame", 2u}, {"ICPCGUI", 4u},                         // 0074E7B4 / 0074E7A8 / 0074E7A0
        {"ICDialog", 8u}, {"ICFreeLook", 0x10u}, {"ICMovie", 0x20u},               // 0074E794 / 0074E788 / 0074E780
    };
    for (const Class& c : classes)                                                 // 006EC8DD..B50
        if (get_int(table, row, c.column, &value) != 0 && value == 1) self->class_flags |= c.bit;
    StringSlot name;
    tlk()->simple_0041e8f0(reinterpret_cast<kotor_menu_10_19::String*>(name.raw()),
                           static_cast<Word>(action_strref)); name.live = true;    // 006ECB57..67 (0041E8F0)
    unsigned char* const bytes = reinterpret_cast<unsigned char*>(self);
    gui_load_text_params_set_text_00415e00(bytes + 0x16C, nullptr, name.get());    // 006ECB6C..80
    {
        StringSlot none;
        tlk()->simple_0041e8f0(reinterpret_cast<kotor_menu_10_19::String*>(none.raw()), 0xBC10u); none.live = true;   // 006ECB85..95
        name.get()->Assign005e5c50(none.get());                                    // 006ECB9A..A4
    }                                                                              // 006ECBA9..B1
    kih::EventDesc* const ev = static_cast<kih::EventDesc*>(
        gui_m48_input_get_event_005df4c0(input(), nullptr, static_cast<Word>(self->id), -1));   // 006ECBB6..C5
    if (ev) {                                                                      // 006ECBCA..CE
        const bool alt = (ev->*(ev->vtable->has_alt_key))() != 0 && self->type > 1;   // 006ECBD4..E6 ([vt+4])
        Word key = alt ? ev->alt_key : ev->key;                                    // 006ECBF0 (+30) / 006ECC37 (+1C)
        if (self->changed != 0) key = self->new_key;                               // 006ECBE8..F5 / 006ECC2F..3C
        else self->orig_key = key;                                                 // 006ECBFD / 006ECC44
        if (key != 0) {                                                            // 006ECC03..05 / 006ECC4A..4C
            StringSlot shown;
            kotor_fastcall_005ee280(client_app(), nullptr, shown.raw(), static_cast<I32>(key)); shown.live = true;   // 006ECC07..15
            name.get()->Assign005e5c50(shown.get());                               // 006ECC1A..24
        }                                                                          // 006ECC29..78
    }
    gui_load_text_params_set_text_00415e00(bytes + 0x334, nullptr, name.get());    // 006ECC7D..88 (event button text)
}                                                                                  // 006ECC8D..99
// ---- 006ECCC0 ResetToDefault (RET): keymap.2da "Language0" default key for the action.
void __fastcall kotor_m1_opt_keymap_button_reset_to_default_006eccc0(KeyMapButton* self, void*) noexcept(false) {
    std::int32_t key = 0;   // [esp+8]: only read when the lookup succeeded (006ECD57..5F); 0 is never observed
    bool failed;
    {
        String column("Language0");                                                // 006ECCDC..E5 (0075948C)
        StringSlot action;
        kotor_m1_opt_keymap_button_get_action_id_006ec120(self, nullptr, action.raw(), self->id, self->type); action.live = true;   // 006ECCEA..D07
        failed = kotor_fastcall_00414110(keymap_2da(), nullptr, action.get(), &column, &key) == 0;   // 006ECD0C..3A
    }                                                                              // 006ECD36..52
    if (failed) return;                                                            // 006ECD57..59
    self->changed = self->orig_key != static_cast<Word>(key) ? 1u : 0u;            // 006ECD5F..73 / 006ECD84
    self->new_key = static_cast<Word>(key);                                        // 006ECD76
    describe(self);                                                                // 006ECD82..DB2
    kotor_m1_opt_keymap_button_update_color_006ec6e0(self, nullptr);               // 006ECDB7..E4A (same stores, inlined in the original)
}
// ---- 006ECE60 PostChangeToEvent (RET): live event description key (+1C, or +30 for an alt-key event with type > 1)
// = new key; then CExoIni::WriteIniEntry("%d" new key, ".\swkotor.ini", "Keymapping", ActionId).
void __fastcall kotor_m1_opt_keymap_button_post_change_006ece60(KeyMapButton* self, void*) noexcept(false) {
    kih::EventDesc* const ev = static_cast<kih::EventDesc*>(
        gui_m48_input_get_event_005df4c0(input(), nullptr, static_cast<Word>(self->id), -1));   // 006ECE7C..8B
    if (ev) {                                                                      // 006ECE92..94
        if ((ev->*(ev->vtable->has_alt_key))() != 0 && self->type > 1)             // 006ECE96..A8
            ev->alt_key = self->new_key;                                           // 006ECEAA..B0
        else
            ev->key = self->new_key;                                               // 006ECEB5..BB
    }
    base_ini_reconstruction::PublicIni ini;
    ini.construct_005e6750();                                                      // 006ECEBE..C2
    struct IniRelease { base_ini_reconstruction::PublicIni* p; ~IniRelease() { p->destroy_005e67e0(); } } release{&ini};   // 006ECF8C..98
    String value;                                                                  // 006ECEC7..D3 (005B3190)
    kotor_reconstruction::StartupFormat005e5680(&value, "%d", self->new_key);      // 006ECED8..EE (0073D720)
    String section(kKeymapping);                                                   // 006ECEF6..FF (0074E7C0)
    String file(kIni);                                                             // 006ECF04..12 (0073D648)
    StringSlot action;
    kotor_m1_opt_keymap_button_get_action_id_006ec120(self, nullptr, action.raw(), self->id, self->type); action.live = true;   // 006ECF17..31
    (void)ini.write_005e67d0(&value, &file, &section, action.get());               // 006ECF36..4F WriteIniEntry
}                                                                                  // 006ECF54..98
// ---- 006EDBD0 UpdateEvent(key) (RET 4)
I32 __fastcall kotor_m1_opt_keymap_button_update_event_006edbd0(KeyMapButton* self, void*, Word key) noexcept(false) {
    const I32 result = kotor_m1_opt_keymap_update_event_006eda80(self->owner, nullptr, self->id, self->type, key,
                                                                 self->class_flags);   // 006EDBF0..0D
    if (result == 0) {                                                             // 006EDC14..16
        self->new_key = key;                                                       // 006EDC35
        self->changed = self->orig_key != key ? 1u : 0u;                           // 006EDC18..3B
        describe(self);                                                            // 006EDC41..63
        kotor_m1_opt_keymap_button_update_color_006ec6e0(self, nullptr);           // 006EDC68..6A
    }
    return result;                                                                 // 006EDC73
}
}

// ======================================================================== event callbacks (thiscall, RET 4)
// ---- 006EC580 FixFilterColors(control): active filter text = 007A23C0; when control == 0 the two others = 007A2390.
void __thiscall KeyMappingsHandlers::FixFilterColors_006ec580(kih::Control* control) {
    KeyMappings* const self = panel_of(this);
    unsigned char* const bytes = reinterpret_cast<unsigned char*>(self);
    unsigned char* const move = bytes + 0x868;
    unsigned char* const game = bytes + 0xA2C;
    unsigned char* const mini = bytes + 0xBF0;
    unsigned char* active; unsigned char* other1; unsigned char* other2;
    switch (self->filter) {                                                        // 006EC583..92
    case 0: active = move; other1 = game; other2 = mini; break;                    // 006EC5F4 / 006EC60C
    case 1: active = game; other1 = move; other2 = mini; break;                    // 006EC5D4 / 006EC5EC
    case 2: active = mini; other1 = move; other2 = game; break;                    // 006EC598 / 006EC5B0
    default: return;                                                               // 006EC592 -> 006EC62C
    }
    gui_load_text_params_set_color_00414e10(active, nullptr, &borrowed_enter_color_007a23c0);
    if (control != nullptr) return;                                                // 006EC5A8..AE / 5E4..EA / 604..0A
    gui_load_text_params_set_color_00414e10(other1, nullptr, &kotor_m1_color_007a2390);
    gui_load_text_params_set_color_00414e10(other2, nullptr, &kotor_m1_color_007a2390);
}
// ---- 006ED2D0 OnDefaultClick: ResetToDefault on every changeable option.
void __thiscall KeyMappingsHandlers::OnDefaultClick_006ed2d0(kih::Control*) {
    KeyMappings* const self = panel_of(this);
    for (I32 i = 0; i < self->buttons.size; ++i) {                                 // 006ED2D4..FB
        KeyMapButton* const b = button_at(self->buttons, i);
        if (b->unchangeable == 0) kotor_m1_opt_keymap_button_reset_to_default_006eccc0(b, nullptr);   // 006ED2E6..F0
    }
}
// ---- 006ED310 OnAcceptClick: PostChangeToEvent for every changed, changeable option; stop capturing; close.
void __thiscall KeyMappingsHandlers::OnAcceptClick_006ed310(kih::Control*) {
    KeyMappings* const self = panel_of(this);
    for (I32 i = 0; i < self->buttons.size; ++i) {                                 // 006ED313..45
        KeyMapButton* const b = button_at(self->buttons, i);
        if (b->unchangeable == 0 && b->changed != 0)                               // 006ED326..38
            kotor_m1_opt_keymap_button_post_change_006ece60(b, nullptr);           // 006ED33A
    }
    self->capturing = 0;                                                           // 006ED347
    set_capture_keys(0);                                                           // 006ED351..59
    manager_of(self)->PlayGuiSound(0);                                             // 006ED35E..63
    kotor_fastcall_0040be00(self->manager, nullptr);                               // 006ED368..6B PopModalPanel
    self->flags = (self->flags & 0xFFFFFCFFu) | 0x400u;                            // 006ED370..80
}
namespace {
void select_filter(KeyMappings* self, I32 filter, unsigned active, unsigned other1, unsigned other2) {
    unsigned char* const bytes = reinterpret_cast<unsigned char*>(self);
    self->filter = filter;
    gui_load_text_params_set_color_00414e10(bytes + active, nullptr, &borrowed_enter_color_007a23c0);
    gui_load_text_params_set_color_00414e10(bytes + other1, nullptr, &kotor_m1_color_007a2390);
    gui_load_text_params_set_color_00414e10(bytes + other2, nullptr, &kotor_m1_color_007a2390);
    kotor_m1_opt_keymap_fill_list_box_006ecfd0(self, nullptr, self->filter);
}
}
// ---- 006ED390 / 006ED3E0 / 006ED430 filters (filter stored before the first SetColor: 006ED39E / 3EE / 43E).
void __thiscall KeyMappingsHandlers::OnFilterMove_006ed390(kih::Control*) {
    select_filter(panel_of(this), 0, 0x868, 0xA2C, 0xBF0);                         // 006ED393..D6
}
void __thiscall KeyMappingsHandlers::OnFilterGame_006ed3e0(kih::Control*) {
    select_filter(panel_of(this), 1, 0xA2C, 0x868, 0xBF0);                         // 006ED3E3..426
}
void __thiscall KeyMappingsHandlers::OnFilterMini_006ed430(kih::Control*) {
    select_filter(panel_of(this), 2, 0xBF0, 0x868, 0xA2C);                         // 006ED433..476
}
// ---- 006ED5C0 onControlEntered: owner->SetCaptureEvent(0) (tail jump with the stack word replaced by 0).
void __thiscall KeyMapButtonHandlers::onControlEntered_006ed5c0(kih::Control*) {
    kotor_m1_opt_keymap_set_capture_event_006ed480(button_of(this)->owner, nullptr, nullptr);   // 006ED5C0..CE
}
// ---- 006ED5E0 onEventClicked(control): changeable and [control+4Ch] != 0 -> owner->SetCaptureEvent(this).
void __thiscall KeyMapButtonHandlers::onEventClicked_006ed5e0(kih::Control* control) {
    KeyMapButton* const self = button_of(this);
    if (self->unchangeable != 0) return;                                           // 006ED5E0..E8
    if (word_at(control, 0x4C) == 0) return;                                       // 006ED5EA..F3
    kotor_m1_opt_keymap_set_capture_event_006ed480(self->owner, nullptr, self);    // 006ED5F5..FF
}
// ---- 0060E760: RET 4 (shared original leaf; typed body for this event slot, D24).
void __thiscall KeyMapButtonHandlers::VoidReturn_0060e760(kih::Control*) {}
// ---- 006EC380 HitCheckMouse(x, y) (RET 8): CSWGuiControl::HitCheckMouse 004187C0, else the event button's +44.
kih::Control* __thiscall KeyMapButtonHit::HitCheckMouse_006ec380(int px, int py) {
    kih::Control* const hit = kih::Control::HitCheckMouse(px, py);                   // 006EC38B..8F
    if (hit) return hit;                                                           // 006EC394..96
    KeyMapButton* const self = reinterpret_cast<KeyMapButton*>(this);
    kih::Control* const child = reinterpret_cast<kih::Control*>(&self->event_button);
    return (child->*(vt<HitView>(child)->hit))(px, py);                              // 006EC398..A6 vslot +44
}
