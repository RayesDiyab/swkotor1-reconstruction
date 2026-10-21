// lane-m1-saveload (Milestone 1, L3): CSWGuiMainMenu::OnLoadSaveGame post-guard body (lifts TRAP #72 0067B1DD),
// CSWGuiSaveLoad (ctor/dtor/HandleInputEvent/PopulateGameList/ShowGame/VerifyLoadGame/DeleteGame/RemoveGame),
// CSWGuiSaveLoadEntry (ctor/dtors/LoadData/SetXboxTitle/portrait + directory getters), CSWGuiRadioButton dtors and
// mouse leaves, method tables 00757650 / 00757740 / 007566D8, and two new D83 function traps:
//   006CB0E0 CSWGuiSaveLoad::LoadGame         (creates the server: CAppManager::CreateServer 00401380 at 006CB331/006CB54E)
//   006CBB60 CSWGuiSaveLoad::HandleSaveButton (save mode only; sole caller of PromptForSaveName 006CB820 at 006CC022
//                                              and its sole address user 006CBE4E)
// Static disassembly of swkotor.exe (GOG K1) only. Conventions: see saveload.h.
//
// D30-FINAL unwind note: callees on these paths do not throw (original operator new 006FA7E6 returns NULL; the
// CExoString family allocates through it). The try/catch blocks reproduce the original EH-state cleanups where C++
// can express them; under /EHsc their emission for extern "C" bodies is not relied upon for any reachable path.
#include "60144004e6-saveload.h"
#include "dae7dce008-startup_views.h"            // M40 kotor_menu_10_19::Tlk::simple_0041e8f0 (CTlkTable::GetSimpleString)
#include "380d673b2c-kotor_unreachable_trap.h"   // D83 shared fatal trap (lane-m48-traps)
#include <cstdint>
#include <cstring>
#include <new>

using namespace kotor_m1_saveload;
using gui_load::Object;
using gui_load::Panel;
using gui_load::Control;
using gui_load::Label;
using gui_load::Button;
using gui_load::Extent;
using gui_load::EventFunc;
using kotor_reconstruction::StartupString;
using kotor_reconstruction::ArrayMutations;

// ---------------------------------------------------------------- existing owners (declarations repeated exactly)
namespace f200_ports { void __fastcall Enumerate005e6640(void*, void*, void*, StartupString*, unsigned, unsigned, unsigned); } // 005E6640
namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } }       // CExoBase* cell 007A39E0
namespace kotor_movie { void __fastcall string_element_destroy_005e5c20(void*, void*); }      // 005E5C20 as element dtor
namespace options_video { struct String; }
namespace client_movies { std::int32_t __fastcall StringFind_005e5eb0(options_video::String* self, void* unused_edx,
                                                                        options_video::String* needle, std::int32_t start); } // 005E5EB0
namespace base_ini_reconstruction {
StartupString* __fastcall left_005e5f70(StartupString*, void*, StartupString*, int);           // 005E5F70 Left(hidden, n)
StartupString* __fastcall right_005e6170(StartupString*, void*, StartupString*, int);          // 005E6170 Right(hidden, n)
}
namespace kotor_menu20 {                                  // token-identical to menu_targets.h (service-menu20-4 owner)
using Word=std::uint32_t;
struct String {char* volatile data;Word volatile capacity;
 __declspec(noinline) std::int32_t __thiscall as_int_005e5630();
};
}
namespace alias_path { struct AliasListView; }
namespace path_resolution { StartupString* __fastcall resolve_005e68a0(alias_path::AliasListView*, void*, StartupString*,
                                                                      StartupString*, unsigned); }  // 005E68A0 ResolveFileName
namespace tlk_file { struct File; }
namespace file_internal { tlk_file::File* __fastcall exofile_construct_005e68d0(tlk_file::File*, void*, StartupString*,
                                                                              unsigned, StartupString*); } // 005E68D0 CExoFile
namespace kotor_input_cells { extern void* volatile gui_manager_007a39f4; }                   // 007A39F4 (D22/D46)
extern "C" {
void __fastcall kotor_fastcall_005ed420(void* outer, void*, std::uint32_t argument);           // 005ED420 DisplayMainMenu
void __fastcall kotor_fastcall_006cb0e0(void* self, void*, void* entry);                      // 006CB0E0 CSWGuiSaveLoad::LoadGame (loadgame lane: claude_loadgame_gui)
void __fastcall kotor_fastcall_006cbb60(void* self, void*, void* control);                   // 006CBB60 CSWGuiSaveLoad::HandleSaveButton (savegui lane: claude_savegui)
void* __fastcall kotor_fastcall_0040be00(void* gui, void*);                                    // 0040BE00 PopModalPanel
void __fastcall kotor_fastcall_004088d0(void*, void*, void*);                                  // 004088D0 RemoveResourceDirectory
StartupString* __fastcall kotor_fastcall_005e6890(void* alias_list, void*, StartupString* alias, std::int32_t length); // 005E6890 GetAliasPath
unsigned __fastcall file_opened_005e6a10(tlk_file::File*, void*);                              // 005E6A10 CExoFile::FileOpened
void __fastcall file_destroy_005e69e0(tlk_file::File*, void*);                                 // 005E69E0 ~CExoFile
menu_resources::Ref* __fastcall async_ref_construct_string_00406d60(menu_resources::Ref*, void*, menu_resources::String*); // 00406D60
StartupString* __fastcall resman_string_from_int_005e5bc0(StartupString*, void*, int);         // 005E5BC0 CExoString(int)
void __stdcall c2da_destroy_array_006fa852(void* first, std::uint32_t size, std::int32_t count,
                                           void (__fastcall* destroy)(void*, void*));          // 006FA852 eh vector dtor iterator
void __fastcall kotor_fastcall_0041c1d0(gui_lb::ListBox* self, void*, ArrayMutations* controls,
                                        gui_load::Word suppress, gui_load::Word restore, gui_load::Word variable) noexcept(false); // 0041C1D0
void __fastcall gui_load_text_params_set_color_00414e10(void* params, void*, const MenuColor* color);  // 00414E10 (core47 owner)
void __fastcall gui_load_border_params_set_pulsing_00414d00(void* params, void*, gui_load::Word, gui_load::Word, gui_load::Word); // 00414D00
extern const MenuColor borrowed_exit_color_007a23b4;                                           // 007A23B4 (input-cells owner)
extern const unsigned char kotor_rdata_live_count_0073ecd2;                                     // .rdata 0073ECD2 (07h; lane-tlk tlk-cells)
}
StartupString* __fastcall GuiConcat005e5d10(StartupString*, void*, StartupString*, const StartupString*);  // 005E5D10 operator+
namespace kotor_sound_path { unsigned __fastcall StringIsEmpty005e58a0(StartupString*, void*); }  // 005E58A0 IsEmpty

namespace {
template<class T> T& at(void* object, unsigned offset) { return *reinterpret_cast<T*>(static_cast<unsigned char*>(object) + offset); }
unsigned char* bytes_of(void* object) { return static_cast<unsigned char*>(object); }
String* str(unsigned char* storage) { return reinterpret_cast<String*>(storage); }
options_video::String* find_view(String* s) { return reinterpret_cast<options_video::String*>(s); }
menu_resources::Manager* resman() { return kotor_resource_demand::resource_manager_007a39e8; }
void* app() { return at<void*>(kotor_menu_window_cells::manager_007a39fc, 4); }         // [[007A39FC]+4]
void* alias_list() { return at<void*>(kotor_menu_10_19::dependencies::base_007a39e0, 0x0C); } // [[007A39E0]+0Ch]
kotor_menu_10_19::Tlk* tlk() { return static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08); } // [007A3A08]
kih::Manager* manager_of(SaveLoad* self) { return reinterpret_cast<kih::Manager*>(self->manager); }
gui_lb::ListBox* games(SaveLoad* self) { return reinterpret_cast<gui_lb::ListBox*>(&self->lb_games); }
kih::GuiObject* target(SaveLoad* self) { return reinterpret_cast<kih::GuiObject*>(self); }
SaveLoad* view(SaveLoadHandlers* h) { return reinterpret_cast<SaveLoad*>(h); }
SaveLoadHandlers* handlers(SaveLoad* self) { return reinterpret_cast<SaveLoadHandlers*>(self); }
kih::Control* kc(void* control) { return static_cast<kih::Control*>(control); }
EventFunc handler(void (__thiscall SaveLoadHandlers::*f)(kih::Control*)) { return static_cast<EventFunc>(f); }  // D47
Word word_of(const void* p) { return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p)); }
void copy_color(void* destination, const MenuColor* source) { std::memcpy(destination, source->words, 12); }  // 3 dword moves

// Typed views of the ORIGINAL tables' called slots (field types as the owning tables declare them).
struct MessageBoxCallView { const void* s00_68[0x1B];
    void (__fastcall* set_message_strref)(void*, void*, Word); };                 // +6C 006249D0 (m48b table 0074FDB0)
static_assert(offsetof(MessageBoxCallView, set_message_strref) == 0x6C, "message box +6C");
struct ListBoxCallView { const void* s00_a0[0x29];
    void (__fastcall* select_left_a4)(gui_lb::ListBox*, void*, Word); };          // +A4 0041B120 (table 0073E840)
static_assert(offsetof(ListBoxCallView, select_left_a4) == 0xA4, "listbox +A4");
struct ButtonCallView { const void* s00;
    void* (__fastcall* deleting_destroy)(Object*, void*, Word);                   // (unused; layout padding)
    const void* s08_94[0x24];
    void (__fastcall* initialize98)(Object*, void*, Word, Word); };               // +98 00418D80 (tables 0073E658/00757740)
static_assert(offsetof(ButtonCallView, initialize98) == 0x98, "button +98");
struct GffCallView { void* (__fastcall* deleting_destroy)(void*, void*, Word); };   // +00 00413010 (table 0073E2D0)
template<class V> const V* vt(const void* object) { const void* p; std::memcpy(&p, object, 4); return static_cast<const V*>(p); }

// Virtual calls through lane-input's typed PMF tables (receiver = object; original `mov reg,[obj]; call [reg+slot]`).
void control_input(void* object, int event, int value) {                         // vslot +3C
    kih::Control* c = kc(object); (c->*(c->vtable->handle_input_event))(event, value);
}
void control_set_enabled(void* object, int enabled) {                            // vslot +88
    kih::Control* c = kc(object); (c->*(c->cvt()->set_enabled))(enabled);
}
kih::GuiObject* control_slot58(void* object) {                                   // vslot +58
    kih::Control* c = kc(object); return (c->*(c->cvt()->slot58))();
}
void panel_set_active_control(SaveLoad* self, void* control, int flag) {         // vslot +08 (00757650 -> 0040A630)
    kih::GuiObject* p = target(self); (p->*(p->vtable->set_active_control))(kc(control), flag);
}
void entry_delete(SaveLoadEntry* entry) {                                        // vslot +00 (00757740 -> 006CBA70), flag 1
    kih::GuiObject* g = reinterpret_cast<kih::GuiObject*>(entry); (g->*(g->vtable->scalar_destroy))(1);
}

// CExoString produced by a callee into unconstructed hidden-return storage; destroyed at scope end.
struct Produced {
    alignas(4) unsigned char bytes[8];
    bool live = false;
    String* slot() { return reinterpret_cast<String*>(bytes); }
    String* take(String* produced) { live = true; return produced; }
    ~Produced() { if (live) slot()->~String(); }
};
// CExoString returned by GetSimpleString (kotor_menu_10_19 view of the same 8-byte storage).
String* simple_string(Produced& out, Word strref) {
    return out.take(reinterpret_cast<String*>(tlk()->simple_0041e8f0(reinterpret_cast<kotor_menu_10_19::String*>(out.bytes), strref)));
}
// Local CExoArrayList<CExoString> filled by GetDirectoryList (array-new storage, count word at data-4):
// released by eh_vector_destructor_iterator(data, 8, [data-4], 005E5C20) + free(data-4) (006CC593..006CC5AD).
struct StringList {
    StartupString* data = nullptr; std::int32_t size = 0; std::int32_t capacity = 0;
    ~StringList() {
        if (data) {
            unsigned char* const block = reinterpret_cast<unsigned char*>(data) - 4;
            std::int32_t count; std::memcpy(&count, block, 4);
            c2da_destroy_array_006fa852(data, 8, count, &kotor_movie::string_element_destroy_005e5c20);
            live_free(block);
        }
        data = nullptr;
    }
};
static_assert(sizeof(StringList) == 12, "CExoArrayList storage");
// Allocation freed when a constructor throws (EH state with unwind action free(allocation)).
struct FreeOnUnwind { void* block; bool armed; ~FreeOnUnwind() { if (armed) live_free(block); } };
// Inline CExoArrayList<CSWGuiControl*>::Add (006CC346..006CC3B5 / 006CC4AF..006CC523).
void add_control(ArrayMutations* list, Word value) {
    if (list->size == list->capacity) {
        const std::int32_t capacity = list->capacity ? list->capacity * 2 : 0x10;   // 006CC350..358
        const Word previous = list->data;
        list->capacity = capacity;                                                  // 006CC369
        void* grown = live_operator_new(static_cast<std::uint32_t>(capacity) * 4u); // 006CC36D
        list->data = word_of(grown);                                                // 006CC376
        for (std::int32_t i = 0; i < list->size; ++i)                               // 006CC383..394
            std::memcpy(reinterpret_cast<void*>(static_cast<std::uintptr_t>(list->data + 4u * i)),
                        reinterpret_cast<const void*>(static_cast<std::uintptr_t>(previous + 4u * i)), 4);
        if (previous) live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(previous)));  // 006CC396..3A0
    }
    std::memcpy(reinterpret_cast<void*>(static_cast<std::uintptr_t>(list->data + 4u * list->size)), &value, 4); // 006CC3AB
    list->size = list->size + 1;                                                    // 006CC3B2..B5
}
Word list_at(const ArrayMutations* list, std::int32_t index) {
    Word value; std::memcpy(&value, reinterpret_cast<const void*>(static_cast<std::uintptr_t>(list->data + 4u * index)), 4);
    return value;
}
void list_put(ArrayMutations* list, std::int32_t index, Word value) {
    std::memcpy(reinterpret_cast<void*>(static_cast<std::uintptr_t>(list->data + 4u * index)), &value, 4);
}
void* in_game_message_box() {                                                    // GetInGameGui()->+98h
    return at<void*>(frame_R4_005ed690(app(), nullptr), 0x98);
}

// .rdata literals
const char kSaveload[] = "saveload";       // 00757868
const char kSaves[] = "SAVES:";            // 00745174
const char kTemp[] = "TEMP:";              // 00745310
const char kEmpty[] = "";                  // 0073D71C
const char kSeparator[] = " - ";           // 00748E98
const char kCorrupt[] = "CORRUPT";         // 007513EC
const char kBackslash[] = "\\";            // 0073D8F4
const char kReadBinary[] = "rb";           // 0073D5B0
const char kSaveNfo[] = "savenfo";         // 0074542C
const char kNfoType[] = "NFO ";            // 00745424
const char kOldSave[] = "Old Save Game";   // 007575A0
const char kLiveFormat[] = "LIVE%d";       // 007453C0
const char kPortraitFormat[] = "%s%d";     // 0073EDD0
const char kPortrait[] = "PORTRAIT";       // 007453A8
const char kDash[] = "-";                  // 0073ED40
const char kScreen[] = "screen";           // 00757588
const char kCheatUsed[] = "Cheat Used";    // 0075757C
const char kGameDirFormat[] = "SAVES:%06d - %s";   // 007454D4
const char kBad[] = "Bad";                 // 0075773C
}

// ================================================================ method tables
extern "C" {
// .data colour cells (initial words = PE image .data words; no .text store to them).
const MenuColor kotor_m1_color_007a2378 = {{0x3F800000ul, 0x3F800000ul, 0x3F800000ul}};
const MenuColor kotor_m1_color_007a2384 = {{0x3EB33333ul, 0x3EB33333ul, 0x3EB33333ul}};
const MenuColor kotor_m1_color_007a2408 = {{0x00000000ul, 0x3EA8F5C3ul, 0x3EFAE148ul}};
}

// 00757650 CSWGuiSaveLoad (27 slots; = panel table 0073E010 except +00, +3C).
struct KotorM1SaveLoadMethods {
    void* (__fastcall* s00)(SaveLoad*, void*, Word);                               // +00 006CB0C0
    void (__fastcall* s04)(Object*, void*, const Extent*);                         // +04 0040A5A0
    void (__fastcall* s08)(Object*, void*, Object*, Word);                         // +08 0040A630
    void (__fastcall* s0c)(Object*, void*, Word);                                  // +0C 0060E760
    Word (__fastcall* s10)(Object*, void*, Word, Word);                            // +10 00409EB0
    void (__fastcall* s14)(Object*, void*);                                        // +14 004015A0
    void (__fastcall* s18)(Object*, void*);                                        // +18 004015A0
    void (__fastcall* s1c)(Object*, void*);                                        // +1C 004015A0
    void (__fastcall* s20)(Object*, void*);                                        // +20 004015A0
    void (__fastcall* s24)(Object*, void*);                                        // +24 004015A0
    Object* (__fastcall* s28)(Object*, void*);                                     // +28 00641DB0
    Word (__fastcall* s2c)(Object*, void*);                                        // +2C 0063E7F0
    Word (__fastcall* s30)(Object*, void*);                                        // +30 0063E7F0
    void (__fastcall* s34)(Panel*, void*, Word);                                   // +34 0040B760 CSWGuiPanel::Draw
    void (__fastcall* s38)(Object*, void*, Word);                                  // +38 0060E760
    void (__fastcall* s3c)(Object*, void*, I32, I32);                              // +3C 006C86D0
    Object* (__fastcall* s40)(Object*, void*, I32, I32);                           // +40 0040B690
    Object* (__fastcall* s44)(Object*, void*);                                     // +44 0040B630
    void (__fastcall* s48)(Panel*, void*);                                         // +48 0040B870
    void (__fastcall* s4c)(Panel*, void*);                                         // +4C 0040C170
    void (__fastcall* s50)(Object*, void*, Word);                                  // +50 0040B640
    void (__fastcall* s54)(Object*, void*, Word);                                  // +54 0040B650
    void (__fastcall* s58)(Object*, void*, Word);                                  // +58 0040B660
    void (__fastcall* s5c)(Object*, void*, Word);                                  // +5C 0040B670
    void (__fastcall* s60)(Object*, void*, Word);                                  // +60 0040B680
    String* (__fastcall* s64)(Panel*, void*, String*);                             // +64 0040A900
    void (__fastcall* s68)(Panel*, void*);                                         // +68 0040A9C0
};
static_assert(sizeof(KotorM1SaveLoadMethods) == 108, "00757650 slot count");
extern "C" const KotorM1SaveLoadMethods kotor_m1_saveload_methods_00757650 = {
    &kotor_m1_saveload_deleting_destruct_006cb0c0, &gui_load_panel_set_extent_0040a5a0,
    &gui_load_panel_set_active_control_0040a630, &gui_load_slot_ret4_0060e760, &gui_load_slot_zero8_00409eb0,
    &gui_load_slot_ret_004015a0, &gui_load_slot_ret_004015a0, &gui_load_slot_ret_004015a0, &gui_load_slot_ret_004015a0,
    &gui_load_slot_ret_004015a0, &gui_load_slot_self_00641db0, &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0,
    &kotor_panel_draw_0040B760, &gui_load_slot_ret4_0060e760, &kotor_m1_saveload_handle_input_006c86d0,
    &gui_load_panel_hit_check_0040b690, &gui_load_panel_active_control_0040b630, &gui_load_panel_on_added_0040b870,
    &gui_load_panel_on_removed_0040c170, &gui_load_panel_button_a_0040b640, &gui_load_panel_button_b_0040b650,
    &gui_load_panel_button_x_0040b660, &gui_load_panel_button_y_0040b670, &gui_load_panel_button_black_0040b680,
    &gui_load_panel_full_screen_bg_0040a900, &gui_load_panel_reset_font_0040a9c0,
};

// 00757740 CSWGuiSaveLoadEntry / 007566D8 CSWGuiRadioButton (40 slots each; identical except +00).
// Differences from the button table 0073E658: +0C 00418E80, +18 00418680, +1C 00418E90, +60 00641DB0.
#define KOTOR_M1_RADIO_FAMILY_SLOTS(SELF)                                                                   \
    void* (__fastcall* s00)(SELF*, void*, Word);                                                           \
    void (__fastcall* s04)(Object*, void*, const Extent*);                                                 \
    void (__fastcall* s08)(Object*, void*, Word, Word);                                                    \
    void (kotor_m1_shared::ControlLeaves::* s0c)(int);                                                     \
    Word (__fastcall* s10)(Object*, void*, Word, Word);                                                    \
    void (__fastcall* s14)(Object*, void*);                                                                \
    void (RadioButtonLeaves::* s18)();                                                                     \
    void (RadioButtonLeaves::* s1c)();                                                                     \
    void (__fastcall* s20)(Object*, void*);                                                                \
    void (__fastcall* s24)(Object*, void*);                                                                \
    Word (__fastcall* s28)(Object*, void*);                                                                \
    Word (__fastcall* s2c)(Object*, void*);                                                                \
    Object* (__fastcall* s30)(Object*, void*);                                                             \
    Word (__fastcall* s34)(Object*, void*);                                                                \
    void (__fastcall* s38)(Object*, void*, Word);                                                          \
    void (kih::Button::* s3c)(int, int);                                                                   \
    void (kih::Button::* s40)(int);                                                                        \
    kih::Control* (kih::Control::* s44)(int, int);                                                         \
    void (__fastcall* s48)(Button*, void*, gui_load::Gff*, gui_load::Struct*);                             \
    Object* (__fastcall* s4c)(Object*, void*);                                                             \
    Word (__fastcall* s50)(Object*, void*);                                                                \
    Word (__fastcall* s54)(Object*, void*);                                                                \
    Object* (__fastcall* s58)(Object*, void*);                                                             \
    Word (__fastcall* s5c)(Object*, void*);                                                                \
    Object* (__fastcall* s60)(Object*, void*);                                                             \
    Word (__fastcall* s64)(Object*, void*);                                                                \
    Word (__fastcall* s68)(Object*, void*);                                                                \
    Word (__fastcall* s6c)(Object*, void*);                                                                \
    Word (__fastcall* s70)(Object*, void*);                                                                \
    Word (__fastcall* s74)(Object*, void*);                                                                \
    Word (__fastcall* s78)(Object*, void*);                                                                \
    void (__fastcall* s7c)(Object*, void*, Word);                                                          \
    int (kih::Control::* s80)();                                                                           \
    void (__fastcall* s84)(Object*, void*);                                                                \
    void (__fastcall* s88)(Object*, void*, Word);                                                          \
    void (__fastcall* s8c)(Object*, void*);                                                                \
    Word (__fastcall* s90)(Object*, void*);                                                                \
    void (__fastcall* s94)(Object*, void*, Word, Word);                                                    \
    void (__fastcall* s98)(Object*, void*, Word, Word);                                                    \
    void (__fastcall* s9c)(Object*, void*, Word, Word, Word, Word);
struct KotorM1SaveLoadEntryMethods { KOTOR_M1_RADIO_FAMILY_SLOTS(SaveLoadEntry) };
struct KotorM1RadioButtonMethods { KOTOR_M1_RADIO_FAMILY_SLOTS(Button) };
#undef KOTOR_M1_RADIO_FAMILY_SLOTS
static_assert(sizeof(KotorM1SaveLoadEntryMethods) == 160 && sizeof(KotorM1RadioButtonMethods) == 160, "40 slots");
#define KOTOR_M1_RADIO_FAMILY_TAIL                                                                         \
    &gui_load_button_set_extent_00417a20, &gui_load_slot_ret8_005b5e90,                                    \
    &kotor_m1_shared::ControlLeaves::HandleMouseOver_00418e80, &gui_load_slot_zero8_00409eb0,              \
    &gui_load_slot_ret_004015a0, &RadioButtonLeaves::HandleLMouseDown_00418680,                            \
    &RadioButtonLeaves::HandleLMouseUp_00418e90, &gui_load_slot_ret_004015a0,                              \
    &gui_load_control_handle_rmouse_up_0041ac70, &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0,\
    &gui_load_slot_self_00641db0, &gui_load_slot_prop10_004064a0, &kotor_fastcall_00417ab0,                \
    &kih::Button::HandleInputEvent, &kih::Button::SetActive, &kih::Control::HitCheckMouse,                 \
    &gui_load_button_load_0041c930, &gui_load_slot_self_00641db0, &gui_load_slot_zero_0063e7f0,            \
    &gui_load_slot_zero_0063e7f0, &gui_load_slot_self_00641db0, &gui_load_slot_zero_0063e7f0,              \
    &gui_load_slot_self_00641db0, &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0,              \
    &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0, &gui_load_slot_zero_0063e7f0,              \
    &gui_load_slot_zero_0063e7f0, &gui_load_control_focus_change_00418960, &kih::Control::GetIsSelectable, \
    &gui_load_control_rmouse_down_0041ac30, &gui_load_button_set_enabled_00418db0,                         \
    &gui_load_button_reset_font_00418df0, &gui_load_control_tooltip_00418a90, &gui_load_slot_ret8_005b5e90,\
    &gui_load_button_initialize_00418d80, &gui_load_button_initialize_00417970
extern "C" const KotorM1SaveLoadEntryMethods kotor_m1_saveload_entry_methods_00757740 = {
    &kotor_m1_saveload_entry_deleting_destruct_006cba70, KOTOR_M1_RADIO_FAMILY_TAIL };
extern "C" const KotorM1RadioButtonMethods kotor_m1_radio_button_methods_007566d8 = {
    &kotor_m1_radio_button_deleting_destruct_006b60d0, KOTOR_M1_RADIO_FAMILY_TAIL };
#undef KOTOR_M1_RADIO_FAMILY_TAIL

namespace {
template<class T> void set_vptr(void* object, const T* table) { const void* p = table; std::memcpy(object, &p, 4); }
}

// ================================================================ CSWGuiMainMenu::OnLoadSaveGame 0067B1A0 (RET 4)
namespace gui_load {
// Guard 0067B1B8..0067B1DB (identical to the M48 trap unit's authored guard): return when ([this+44h] & 600h) == 400h,
// [this+140Ch] == 0, or [control+4Ch] == 0. Post-guard body 0067B1DD..0067B239 (lifts D83 trap #72).
void __thiscall MainMenuHandlers::OnLoadSaveGame(kotor_input_hover::Control* control) {
    gui_load::MainMenu* self = reinterpret_cast<gui_load::MainMenu*>(this);   // gui-load layout view of the same object
    if ((self->flags & 0x600u) == 0x400u) return;                                  // 0067B1B8..C6
    if (self->u140c == 0) return;                                                  // 0067B1C8..D2
    if (static_cast<Word>(control->last_event_value) == 0) return;                 // 0067B1D4..DB ([arg+4Ch])
    void* allocation = live_operator_new(0x1160);                                  // 0067B1DD..E2
    SaveLoad* panel = nullptr;
    if (allocation) {
        FreeOnUnwind guard{allocation, true};                                      // EH state 0 (0067B1F0): free on unwind
        panel = kotor_m1_saveload_construct_006cc680(static_cast<SaveLoad*>(allocation), nullptr,
                                                     self->manager, 0, 1);          // 0067B1F6..FF (manager=[esi+18h], 0, 1)
        guard.armed = false;                                                       // state -1 (0067B20E)
    }
    kotor_fastcall_0040bc70(self->manager, nullptr, panel, 2, 1);                  // 0067B206..16 AddPanel(panel, 2, 1)
    // 0067B21B..2A: the new panel's own state read with no null check (allocation failure faults as in the original).
    if ((panel->flags & 0x600u) == 0x400u) return;
    self->flags = (self->flags & 0xFFFFFCFFu) | 0x400u;                             // 0067B22C..39 main menu -> state 400h
}
}

extern "C" {
// ================================================================ CSWGuiSaveLoad
// 006CC680 CSWGuiSaveLoad::CSWGuiSaveLoad(CSWGuiManager*, int mode, int fromMainMenu) RET 0Ch.
SaveLoad* __fastcall kotor_m1_saveload_construct_006cc680(SaveLoad* self, void*, Object* manager, Word mode,
                                                          Word from_main_menu) noexcept(false) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                    // 006CC6AB
    int built = 0;                                                                 // EH state (FuncInfo of 0073579B)
    try {
        set_vptr(self, &kotor_m1_saveload_methods_00757650);                       // 006CC6BB
        ::new (static_cast<void*>(self->directory)) String;                        // 006CC6C1 (005B3190)
        built = 1;
        gui_load_label_construct_0041acd0(&self->lbl_panelname, nullptr);  built = 2;   // 006CC6CE
        gui_load_label_construct_0041acd0(&self->lbl_screenshot, nullptr); built = 3;   // 006CC6DE
        gui_load_label_construct_0041acd0(&self->lbl_planetname, nullptr); built = 4;   // 006CC6EE
        gui_load_label_construct_0041acd0(&self->lbl_areaname, nullptr);   built = 5;   // 006CC6FE
        // 006CC720 eh_vector_constructor_iterator(+574, 140h, 3, 0041ACD0, 00419D10): a failing element destroys
        // the already-built ones (reverse) before propagating.
        for (int i = 0; i < 3; ++i) {
            try { gui_load_label_construct_0041acd0(&self->lbl_pm[i], nullptr); }
            catch (...) { for (int k = i; k-- > 0;) gui_load_label_destruct_00419d10(&self->lbl_pm[k], nullptr); throw; }
        }
        built = 6;
        gui_load_listbox_construct_0041be40(games(self), nullptr);        built = 7;   // 006CC730
        gui_load_button_construct_0041c0f0(&self->btn_saveload, nullptr); built = 8;   // 006CC742
        gui_load_button_construct_0041c0f0(&self->btn_back, nullptr);     built = 9;   // 006CC752
        gui_load_button_construct_0041c0f0(&self->btn_delete, nullptr);   built = 10;  // 006CC764
        {
            ResRef layout;
            layout.construct_00406d80(kSaveload);                                  // 006CC769..777
            gui_load_panel_start_layout_0040a680(self, nullptr, &layout);          // 006CC77C..783
        }
        struct Item { Control* control; const char* tag; };
        const Item items[11] = {                                                   // 006CC788..006CC9C6 InitControl(c, tag, 1)
            {&self->lbl_panelname, "LBL_PANELNAME"}, {&self->lbl_screenshot, "LBL_SCREENSHOT"},
            {&self->lbl_planetname, "LBL_PLANETNAME"}, {&self->lbl_areaname, "LBL_AREANAME"},
            {&self->lbl_pm[0], "LBL_PM1"}, {&self->lbl_pm[1], "LBL_PM2"}, {&self->lbl_pm[2], "LBL_PM3"},
            {&self->lb_games, "LB_GAMES"}, {&self->btn_saveload, "BTN_SAVELOAD"},
            {&self->btn_back, "BTN_BACK"}, {&self->btn_delete, "BTN_DELETE"},
        };                                                                         // .rdata 00757858/48 007548E8 00757838/30/28/20/14/04 007548AC 007577F8
        for (const Item& item : items) {
            String tag(item.tag);
            gui_load_panel_init_control_0040b930(self, nullptr, item.control, &tag, 1);
        }
        gui_load_panel_stop_layout_0040b8f0(self, nullptr);                        // 006CC9CD
        GuiAssign005e5140(str(self->directory), nullptr, kEmpty);                  // 006CC9D2..DA
        vt<ListBoxCallView>(games(self))->select_left_a4(games(self), nullptr, 1); // 006CC9DF..ED vslot +A4 (1)
        self->btn_saveload.flags &= 0xFFFFFFFBu;                                   // 006CC9F3..FF
        gui_load_control_add_event_0041ab20(&self->btn_saveload, nullptr, 0x27, target(self),
            kotor_m1_shared::event_func(&kotor_m1_shared::PanelEventThunks::AcceptButtonCallback_00624ba0)); // 006CCA06
        self->btn_back.flags &= 0xFFFFFFFBu;                                       // 006CCA0B..22
        gui_load_control_add_event_0041ab20(&self->btn_back, nullptr, 0x27, target(self),
            kotor_m1_shared::event_func(&kotor_m1_shared::PanelEventThunks::OnBButtonPressed_00624bb0));     // 006CCA25
        self->btn_delete.flags &= 0xFFFFFFFBu;                                     // 006CCA2A..36
        gui_load_control_add_event_0041ab20(&self->btn_delete, nullptr, 0x27, target(self),
            kotor_m1_shared::event_func(&kotor_m1_shared::PanelEventThunks::OnXButtonPressed_00624bc0));     // 006CCA3D
        gui_load_control_add_event_0041ab20(&self->btn_delete, nullptr, 0x27, target(self),
            handler(&SaveLoadHandlers::DeleteGame_006caa90));                      // 006CCA42..4C
        gui_load_control_add_event_0041ab20(&self->btn_saveload, nullptr, 0x27, target(self),
            mode ? handler(&SaveLoadHandlers::HandleSaveButton_006cbb60)          // 006CCA51..69 (mode != 0)
                 : handler(&SaveLoadHandlers::VerifyLoadGame_006cc0e0));           //              (mode == 0: main menu)
        // 006CCA6E..C5: proto item (+C00 = listbox+2CC) vslot +58 -> border colour +90 = [007A23B4], hilight colour
        // +104 = [007A23C0], hilight params +F4 SetPulsingAlpha(1, 1, 0).
        kih::GuiObject* proto = control_slot58(games(self)->proto);
        copy_color(bytes_of(proto) + 0x90, &borrowed_exit_color_007a23b4);
        copy_color(bytes_of(proto) + 0x104, &borrowed_enter_color_007a23c0);
        gui_load_border_params_set_pulsing_00414d00(bytes_of(proto) + 0xF4, nullptr, 1, 1, 0);
        self->sl_flags = (((from_main_menu & 1u) << 1) | (self->sl_flags & 0xFFFFFFFCu)) | (mode & 1u);  // 006CCACA..E1
        self->next_number = 2;                                                     // 006CCAE4
        if (self->sl_flags & 1u) {                                                 // 006CCADF..F1
            gui_load_text_params_set_strref_00415e50(bytes_of(self) + 0xD80, nullptr, 0x633);   // 006CCAF3..F8
            gui_load_text_params_set_strref_00415e50(bytes_of(self) + 0x15C, nullptr, 0x634);   // 006CCAFD / 006CCB13..19
        } else {
            gui_load_text_params_set_strref_00415e50(bytes_of(self) + 0xD80, nullptr, 0x635);   // 006CCB04..09
            gui_load_text_params_set_strref_00415e50(bytes_of(self) + 0x15C, nullptr, 0x631);   // 006CCB0E / 006CCB13..19
        }
        kotor_m1_saveload_populate_game_list_006cc160(self, nullptr);              // 006CCB1E..20
        {
            String temp(kTemp);                                                    // 006CCB25..2E
            resman()->directory_00408800(&temp);                                   // 006CCB33..43 AddResourceDirectory
        }                                                                          // 006CCB51
        gui_load_panel_set_active_control_0040a630(self, nullptr, &self->lb_games, 0);   // 006CCB56..61
    } catch (...) {
        if (built > 9) gui_load_button_destruct_00419da0(&self->btn_delete, nullptr);
        if (built > 8) gui_load_button_destruct_00419da0(&self->btn_back, nullptr);
        if (built > 7) gui_load_button_destruct_00419da0(&self->btn_saveload, nullptr);
        if (built > 6) gui_load_listbox_destruct_0041b060(games(self), nullptr);
        if (built > 5) for (int k = 3; k-- > 0;) gui_load_label_destruct_00419d10(&self->lbl_pm[k], nullptr);
        if (built > 4) gui_load_label_destruct_00419d10(&self->lbl_areaname, nullptr);
        if (built > 3) gui_load_label_destruct_00419d10(&self->lbl_planetname, nullptr);
        if (built > 2) gui_load_label_destruct_00419d10(&self->lbl_screenshot, nullptr);
        if (built > 1) gui_load_label_destruct_00419d10(&self->lbl_panelname, nullptr);
        if (built > 0) str(self->directory)->~String();
        gui_load_panel_destruct_0040cf70(self, nullptr);
        throw;
    }
    return self;                                                                   // 006CCB6B
}

// 006CA110 CSWGuiSaveLoad::~CSWGuiSaveLoad.
void __fastcall kotor_m1_saveload_destruct_006ca110(SaveLoad* self, void*) noexcept(false) {
    set_vptr(self, &kotor_m1_saveload_methods_00757650);                           // 006CA130
    {
        String temp(kTemp);                                                        // 006CA136..147
        kotor_fastcall_004088d0(resman(), nullptr, &temp);                         // 006CA14C..15C RemoveResourceDirectory
    }                                                                              // 006CA16A
    if (!kotor_sound_path::StringIsEmpty005e58a0(str(self->directory), nullptr))  // 006CA16F..17B
        kotor_fastcall_004088d0(resman(), nullptr, str(self->directory));          // 006CA17D..184
    gui_load_button_destruct_00419da0(&self->btn_delete, nullptr);                 // 006CA194
    gui_load_button_destruct_00419da0(&self->btn_back, nullptr);                   // 006CA1A4
    gui_load_button_destruct_00419da0(&self->btn_saveload, nullptr);               // 006CA1B4
    gui_load_listbox_destruct_0041b060(games(self), nullptr);                      // 006CA1C4
    for (int k = 3; k-- > 0;) gui_load_label_destruct_00419d10(&self->lbl_pm[k], nullptr);  // 006CA1E1 vector dtor (reverse)
    gui_load_label_destruct_00419d10(&self->lbl_areaname, nullptr);                // 006CA1F1
    gui_load_label_destruct_00419d10(&self->lbl_planetname, nullptr);              // 006CA201
    gui_load_label_destruct_00419d10(&self->lbl_screenshot, nullptr);              // 006CA211
    gui_load_label_destruct_00419d10(&self->lbl_panelname, nullptr);               // 006CA21E
    str(self->directory)->~String();                                               // 006CA22A
    gui_load_panel_destruct_0040cf70(self, nullptr);                               // 006CA239
}
// 006CB0C0 scalar deleting destructor RET 4.
void* __fastcall kotor_m1_saveload_deleting_destruct_006cb0c0(SaveLoad* self, void*, Word flags) noexcept(false) {
    kotor_m1_saveload_destruct_006ca110(self, nullptr);                            // 006CB0C3
    if (flags & 1u) live_free(self);                                               // 006CB0C8..D0
    return self;                                                                   // 006CB0D8
}

// 006C86D0 CSWGuiSaveLoad::HandleInputEvent(int event, int value) RET 8 (vtable 00757650 +3C).
void __fastcall kotor_m1_saveload_handle_input_006c86d0(Object* object, void*, I32 event, I32 value) noexcept(false) {
    SaveLoad* self = static_cast<SaveLoad*>(static_cast<Panel*>(object));
    if (value != 0 && (event == 0x28 || (event > 0x2C && event <= 0x2E))) {        // 006C86D5..EE (28h, 2Dh, 2Eh)
        manager_of(self)->PlayGuiSound(0);                                         // 006C86F0..F5
        self->flags = (self->flags & 0xFFFFFCFFu) | 0x400u;                         // 006C86FA..707 state 400h
        if (self->sl_flags & 2u) {                                                 // 006C870A..0E opened from the main menu
            kotor_fastcall_005ed420(app(), nullptr, 0);                            // 006C8710..1B DisplayMainMenu(0)
            gui_load_panel_handle_input_00409e60(self, nullptr, event, value);     // 006C8720..24
            return;
        }
        kotor_fastcall_0040be00(self->manager, nullptr);                           // 006C872F..32 PopModalPanel
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, value);             // 006C8737..3B
}

// 006CC160 CSWGuiSaveLoad::PopulateGameList() RET.
void __fastcall kotor_m1_saveload_populate_game_list_006cc160(SaveLoad* self, void*) noexcept(false) {
    StringList directories;                                                        // [esp+3C..44] zeroed 006CC180..188
    ArrayMutations controls;                                                       // [esp+30..38] zeroed 006CC190..198
    controls.data = 0; controls.size = 0; controls.capacity = 0;
    struct FreeControls { ArrayMutations* list; ~FreeControls() {                  // 006CC56D..583 / 006CC64E..664
        live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(list->data))); list->data = 0; } } free_controls{&controls};
    Control* const proto = games(self)->proto;                                     // [esp+14] = [ebp+C00h] (006CC19C/1BF)
    const Extent item_extent = { 0, 0, 0, 0x2A };                                  // [esp+48..54] (006CC1AB..1B7)
    String unused;                                                                 // [esp+28] 006CC1C3; dtor 006CC568/006CC649
    {
        String alias(kSaves);                                                      // 006CC1C8..1D6
        f200_ports::Enumerate005e6640(kotor_menu_10_19::dependencies::base_007a39e0, nullptr, &directories, &alias,
                                      0xFFFF, 1, 1);                               // 006CC1DB..1F9 GetDirectoryList
    }                                                                              // 006CC207
    Word created;
    {
        String temp(kTemp);                                                        // 006CC20C..215
        created = gui_load_resman_create_directory_004068c0(resman(), nullptr, &temp);   // 006CC21A..22A
    }                                                                              // 006CC240
    if (created == 0) {                                                            // 006CC22F..247 (NEG/SBB/INC; JE skips)
        String temp(kTemp);                                                        // 006CC249..252
        gui_load_resman_clean_directory_00409460(resman(), nullptr, &temp, 0, 0);  // 006CC257..269
    }                                                                              // 006CC277
    for (std::int32_t i = 0; i < directories.size; ++i) {                          // 006CC27C..28A / 006CC3C3..3CC
        void* memory = live_operator_new(0x248);                                   // 006CC290..295
        SaveLoadEntry* entry = nullptr;
        if (memory) {
            FreeOnUnwind guard{memory, true};                                      // EH state 6
            entry = kotor_m1_saveload_entry_construct_006cb940(static_cast<SaveLoadEntry*>(memory), nullptr);  // 006CC2AC
            guard.armed = false;
        }
        vt<ButtonCallView>(entry)->initialize98(entry, nullptr, word_of(&item_extent), word_of(proto));      // 006CC2B3..2C6 vslot +98
        kotor_m1_saveload_entry_load_data_006c8e50(entry, nullptr, &directories.data[i]);  // 006CC2CC..2D6
        kotor_m1_saveload_entry_set_xbox_title_006c9780(entry, nullptr, &directories.data[i]);  // 006CC2DB..2E5
        const Word save_mode = self->sl_flags & 1u;                                // 006CC2EA..2ED
        if (save_mode && (entry->number == 1 || entry->number == 0)) {             // 006CC2F2..2FF
            entry_delete(entry);                                                   // 006CC301..307 vslot +00 (1)
            continue;                                                              // 006CC309
        }
        gui_load_control_add_event_0041ab20(entry, nullptr, 0x27, target(self),
            save_mode ? handler(&SaveLoadHandlers::HandleSaveButton_006cbb60)
                      : handler(&SaveLoadHandlers::VerifyLoadGame_006cc0e0));      // 006CC30E..323
        gui_load_control_add_event_0041ab20(entry, nullptr, 0x29, target(self),
            handler(&SaveLoadHandlers::DeleteGame_006caa90));                      // 006CC328..332
        gui_load_control_add_event_0041ab20(entry, nullptr, 0, target(self),
            handler(&SaveLoadHandlers::ShowGame_006c89d0));                        // 006CC337..341
        add_control(&controls, word_of(entry));                                    // 006CC346..3B5
        if (i == 0) handlers(self)->ShowGame_006c89d0(kc(entry));                  // 006CC3B3..3BE
    }
    Word next = 2;                                                                 // 006CC3D5 EDI / [esp+10h]
    if (self->sl_flags & 1u) {                                                     // 006CC3DC..3E2 save mode
        Word highest = 0;                                                          // EBX
        for (std::int32_t k = 0; k < controls.size; ++k) {                         // 006CC3E8..41D
            Word number = static_cast<Word>(reinterpret_cast<SaveLoadEntry*>(
                static_cast<std::uintptr_t>(list_at(&controls, k)))->number);
            if (number == next) ++next;                                            // 006CC409..40D
            if (highest > number) number = highest;                                // 006CC40E..412 (JBE, unsigned)
            highest = number;                                                      // 006CC414
        }
        void* memory = live_operator_new(0x248);                                   // 006CC423..428
        SaveLoadEntry* entry = nullptr;
        if (memory) {
            FreeOnUnwind guard{memory, true};                                      // EH state 7
            entry = kotor_m1_saveload_entry_construct_006cb940(static_cast<SaveLoadEntry*>(memory), nullptr);  // 006CC43F
            guard.armed = false;
        }
        vt<ButtonCallView>(entry)->initialize98(entry, nullptr, word_of(&item_extent), word_of(proto));      // 006CC446..459
        gui_load_text_params_set_strref_00415e50(bytes_of(entry) + 0x16C, nullptr, 0x636);                     // 006CC45F..46A
        Word number = highest + 1;                                                 // 006CC46F
        if (number < 2) number = 2;                                                // 006CC472..477 (JAE)
        entry->entry_flags |= 0x44u;                                               // 006CC47C..48B
        entry->number = static_cast<I32>(number);                                  // 006CC495
        gui_load_control_add_event_0041ab20(entry, nullptr, 0x27, target(self),
            handler(&SaveLoadHandlers::HandleSaveButton_006cbb60));                // 006CC482..49B
        gui_load_control_add_event_0041ab20(entry, nullptr, 0, target(self),
            handler(&SaveLoadHandlers::ShowGame_006c89d0));                        // 006CC4A0..4AA
        const std::int32_t old_size = controls.size;                               // 006CC4AF..4B9 EBX
        add_control(&controls, word_of(entry));                                    // 006CC4BB..523
        for (std::int32_t k = old_size; k > 0; --k) list_put(&controls, k, list_at(&controls, k - 1));  // 006CC524..53E
        list_put(&controls, 0, word_of(entry));                                    // 006CC540..544
    } else if (controls.size == 0) {                                               // 006CC5C3..5C7
        self->flags = (self->flags & 0xFFFFFCFFu) | 0x400u;                         // 006CC5CD..5DE state 400h
        if (self->sl_flags & 2u) kotor_fastcall_005ed420(app(), nullptr, 0);       // 006CC5DC..5ED DisplayMainMenu(0)
        else kotor_fastcall_0040be00(self->manager, nullptr);                      // 006CC5F4..5F7 PopModalPanel
        void* box = in_game_message_box();                                         // 006CC5FC..609
        kotor_fastcall_00627130(box, nullptr, 0);                                  // 006CC60F..613 SetAllowCancel(0)
        gui_load_messagebox_set_callback_00624a40(box, nullptr, nullptr, nullptr, 0);   // 006CC618..620 SetCallback(0,0,0)
        vt<MessageBoxCallView>(box)->set_message_strref(box, nullptr, 0xA5FB);     // 006CC625..62E vslot +6C
        kotor_fastcall_0040bc70(self->manager, nullptr, box, 1, 1);                // 006CC631..63B AddPanel(box, 1, 1)
        return;                                                                    // ~unused, free(controls), ~directories
    }
    kotor_fastcall_0041c1d0(games(self), nullptr, &controls, 1, 0, 0);             // 006CC548..557 AddControls(list, 1, 0, 0)
    self->next_number = next;                                                      // 006CC560
}                                                                                  // 006CC568 ~unused, 006CC577 free, 006CC593.. list

} // extern "C"

// ================================================================ SaveLoad event / message-box callbacks
// 006C89D0 CSWGuiSaveLoad::ShowGame(CSWGuiControl* entry) RET 4.
void __thiscall SaveLoadHandlers::ShowGame_006c89d0(kih::Control* control) {
    SaveLoad* self = view(this);
    SaveLoadEntry* entry = reinterpret_cast<SaveLoadEntry*>(control);
    if (static_cast<Word>(control->last_event_value) == 0) return;                 // 006C89E9..9F5 [ebx+4Ch]
    if (!control_slot58(entry)) return;                                            // 006C89FB..A04 vslot +58
    const Word flags = entry->entry_flags;                                         // 006C8A0A
    if ((flags & 1u) && !(flags & 2u)) {                                           // 006C8A10..1C
        control_set_enabled(&self->btn_delete, 1);                                 // 006C8A22..30 vslot +88 (1)
        std::int32_t position;
        {
            String dash(kDash);                                                    // 006C8A36..3F
            position = client_movies::StringFind_005e5eb0(find_view(str(entry->area_name)), nullptr, find_view(&dash), 0);  // 006C8A44..5B
        }                                                                          // 006C8A6E
        if (position == -1) {                                                      // 006C8A73..76
            gui_load_text_params_set_text_00415e00(bytes_of(self) + 0x3DC, nullptr, str(entry->area_name));   // 006C8A78..7F
            String empty(kEmpty);                                                  // 006C8A84..8D
            gui_load_text_params_set_text_00415e00(bytes_of(self) + 0x51C, nullptr, &empty);                  // 006C8A92..A5
        } else {
            Produced left;
            base_ini_reconstruction::left_005e5f70(str(entry->area_name), nullptr, left.slot(), position - 1); // 006C8AB0..BB
            left.live = true;
            const std::int32_t length = str(entry->area_name)->BorrowedLength005e5790();                      // 006C8AC0..CA
            Produced right;
            base_ini_reconstruction::right_005e6170(str(entry->area_name), nullptr, right.slot(),
                                                    length + (-2 - position));     // 006C8ACF..AE0
            right.live = true;
            gui_load_text_params_set_text_00415e00(bytes_of(self) + 0x3DC, nullptr, left.slot());   // 006C8AE5..AF5
            gui_load_text_params_set_text_00415e00(bytes_of(self) + 0x51C, nullptr, right.slot());  // 006C8AFA..B05
        }                                                                          // 006C8B0A..B24 (~right, ~left)
        for (int i = 0; i < 3; ++i) {                                              // 006C8B29..B52
            ResRef portrait;
            ResRef* produced = kotor_m1_saveload_entry_get_portrait_006c8200(entry, nullptr, &portrait, i);   // 006C8B31..39
            gui_load_border_params_set_fill_00414c00(bytes_of(self) + 0x5E4 + 0x140u * i, nullptr, produced, 0);  // 006C8B3E..43
        }
        void* screenshot_params = bytes_of(self) + 0x224;                          // LBL_SCREENSHOT border params
        if (!(entry->entry_flags & 0x18u)) {                                       // 006C8B54..5F
            {
                ResRef none; none.construct_00406d80(kEmpty);                      // 006C8B65..6A
                gui_load_border_params_set_fill_00414c00(screenshot_params, nullptr, &none, 0);  // 006C8B6F..7E
            }
            if (!kotor_sound_path::StringIsEmpty005e58a0(str(self->directory), nullptr))         // 006C8B83..8F
                kotor_fastcall_004088d0(resman(), nullptr, str(self->directory));  // 006C8B91..98
            {
                Produced directory;
                kotor_m1_saveload_entry_get_game_directory_006c8250(entry, nullptr, directory.slot());   // 006C8B9D..BA4
                directory.live = true;
                str(self->directory)->Assign005e5c50(directory.slot());            // 006C8BA9..BB4
            }                                                                      // 006C8BC5
            resman()->directory_00408800(str(self->directory));                    // 006C8BCA..BD1 AddResourceDirectory
            ResRef screen; screen.construct_00406d80(kScreen);                     // 006C8BD6..BDF
            gui_load_border_params_set_fill_00414c00(screenshot_params, nullptr, &screen, 0);   // 006C8BE4..C08
        } else {
            ResRef shot;
            async_ref_construct_string_00406d60(&shot, nullptr, str(entry->screenshot));   // 006C8BEF..BF6
            gui_load_border_params_set_fill_00414c00(screenshot_params, nullptr, &shot, 0); // 006C8BFB..C08
        }
        void* screenshot_text = bytes_of(self) + 0x29C;                            // LBL_SCREENSHOT text params (EDI)
        {
            String empty(kEmpty);                                                  // 006C8C0D..16
            gui_load_text_params_set_text_00415e00(screenshot_text, nullptr, &empty);   // 006C8C1B..30
        }                                                                          // 006C8C40
        if (entry->entry_flags & 0x80u) {                                          // 006C8C45..4D (byte sign test)
            {
                String cheat(kCheatUsed);                                          // 006C8C53..5C
                gui_load_text_params_set_text_00415e00(screenshot_text, nullptr, &cheat);   // 006C8C61..70
            }                                                                      // 006C8C7D
            gui_load_text_params_set_color_00414e10(screenshot_text, nullptr, &borrowed_enter_color_007a23c0);  // 006C8C82..89
            self->lbl_screenshot.flags |= 2u;                                      // 006C8C8E (+1F8)
        }
    } else {                                                                       // 006C8C9A
        {
            String empty(kEmpty);
            gui_load_text_params_set_text_00415e00(bytes_of(self) + 0x3DC, nullptr, &empty);   // 006C8C9A..CBB
        }
        {
            String empty(kEmpty);
            gui_load_text_params_set_text_00415e00(bytes_of(self) + 0x51C, nullptr, &empty);   // 006C8CD0..CF1
        }
        for (int i = 0; i < 3; ++i) {                                              // 006C8D03..D2A
            ResRef none; c2da_default_ref_00405ed0(&none, nullptr);
            gui_load_border_params_set_fill_00414c00(bytes_of(self) + 0x5E4 + 0x140u * i, nullptr, &none, 0);
        }
        {
            ResRef none; c2da_default_ref_00405ed0(&none, nullptr);                // 006C8D2C..30
            gui_load_border_params_set_fill_00414c00(bytes_of(self) + 0x224, nullptr, &none, 0);   // 006C8D35..3E
        }
        gui_load_text_params_set_strref_00415e50(bytes_of(self) + 0x29C, nullptr, 0x636);       // 006C8D43..4E
        control_set_enabled(&self->btn_delete, 0);                                 // 006C8D53..61 vslot +88 (0)
    }
    if (!(self->sl_flags & 1u)) {                                                  // 006C8D67..6D load mode
        const bool missing = (entry->entry_flags & 0x20u) != 0;                    // 006C8D6F / 006C8D8E
        gui_load_text_params_set_color_00414e10(bytes_of(self) + 0xD80, nullptr,
            missing ? &kotor_m1_color_007a2408 : &borrowed_exit_color_007a23b4);   // 006C8D76..89
        copy_color(bytes_of(self) + 0xCA4, missing ? &kotor_m1_color_007a2384 : &kotor_m1_color_007a2378);  // 006C8D95..B4
    } else {
        gui_load_text_params_set_color_00414e10(bytes_of(self) + 0xD80, nullptr, &borrowed_exit_color_007a23b4);  // 006C8DB9..C4
        copy_color(bytes_of(self) + 0xCA4, &kotor_m1_color_007a2378);              // 006C8DC9..E5
    }
    const bool valid = (entry->entry_flags & 1u) != 0;                             // 006C8DE8 / 006C8E07
    gui_load_text_params_set_color_00414e10(bytes_of(self) + 0x1108, nullptr,
        valid ? &borrowed_exit_color_007a23b4 : &kotor_m1_color_007a2408);         // 006C8DEF..E02
    copy_color(bytes_of(self) + 0x102C, valid ? &kotor_m1_color_007a2378 : &kotor_m1_color_007a2384);   // 006C8E0E..2D
}

// 006C8900 CSWGuiSaveLoad::RemoveGame(CSWGuiControl*) RET 4 (message-box callback; argument unused).
void __thiscall SaveLoadHandlers::RemoveGame_006c8900(kih::Control*) {
    SaveLoad* self = view(this);
    void* box = in_game_message_box();                                             // 006C890E..928
    if (!(at<Word>(box, 0x64) & 1u)) return;                                       // 006C892E..932 result bit
    gui_lb::ListBox* list = games(self);
    Control* selected = kotor_m1_listbox_get_selected_control_0041a5d0(list, nullptr);  // 006C8939..941
    if (!selected) return;                                                         // 006C8946..948
    {
        Produced directory;
        kotor_m1_saveload_entry_get_game_directory_006c8250(static_cast<SaveLoadEntry*>(static_cast<Button*>(selected)),
                                                            nullptr, directory.slot());   // 006C894B..952
        directory.live = true;
        gui_load_resman_nuke_directory_00409480(resman(), nullptr, directory.slot(), 1, 1);   // 006C8957..96A
    }                                                                              // 006C897B
    const std::int32_t index = list->selected;                                     // 006C8980 MOVSX [esi+BFAh]
    kotor_m1_saveload_populate_game_list_006cc160(self, nullptr);                  // 006C8987..989
    std::int32_t last = list->items.size - 1;                                      // 006C898E..994
    if (index < last) last = index;                                                // 006C8995..999
    gui_load_listbox_set_selected_0041c040(list, nullptr, last, 0);                // 006C899B..9A0
    panel_set_active_control(self, nullptr, 0);                                    // 006C89A5..9AD vslot +08 (0, 0)
    panel_set_active_control(self, list, 0);                                       // 006C89B0..9B7 vslot +08 (list, 0)
}

// 006CAA90 CSWGuiSaveLoad::DeleteGame(CSWGuiControl*) RET 4.
void __thiscall SaveLoadHandlers::DeleteGame_006caa90(kih::Control* control) {
    if (static_cast<Word>(control->last_event_value) == 0) return;                 // 006CAA91..A9D
    SaveLoad* self = view(this);
    void* box = in_game_message_box();                                             // 006CAA9F..AAD
    kotor_fastcall_00627130(box, nullptr, 1);                                      // 006CAAB3..AB7 SetAllowCancel(1)
    vt<MessageBoxCallView>(box)->set_message_strref(box, nullptr, 0x638);          // 006CAABC..AC5 vslot +6C
    gui_load_messagebox_set_callback_00624a40(box, nullptr, target(self),
        handler(&SaveLoadHandlers::RemoveGame_006c8900), word_of(control));        // 006CAAC8..AD1
    kotor_fastcall_0040bc70(self->manager, nullptr, box, 1, 1);                    // 006CAAD6..ADE AddPanel(box, 1, 1)
}

// 006CC0E0 CSWGuiSaveLoad::VerifyLoadGame(CSWGuiControl*) RET 4.
void __thiscall SaveLoadHandlers::VerifyLoadGame_006cc0e0(kih::Control* control) {
    if (static_cast<Word>(control->last_event_value) == 0) return;                 // 006CC0E0..0EC
    SaveLoad* self = view(this);
    Control* selected = kotor_m1_listbox_get_selected_control_0041a5d0(games(self), nullptr);   // 006CC0EF..0F5
    if (!selected) return;                                                         // 006CC0FA..0FE
    if (self->sl_flags & 2u) {                                                     // 006CC100..104
        LoadGame_006cb0e0(kc(selected));                                           // 006CC106..109 (TRAP: leaves the menu)
        return;
    }
    void* box = in_game_message_box();                                             // 006CC113..122
    kotor_fastcall_00627130(box, nullptr, 1);                                      // 006CC128..12C
    vt<MessageBoxCallView>(box)->set_message_strref(box, nullptr, 0x7D9B);         // 006CC131..13A
    gui_load_messagebox_set_callback_00624a40(box, nullptr, target(self),
        handler(&SaveLoadHandlers::LoadGame_006cb0e0), word_of(selected));         // 006CC13D..146
    kotor_fastcall_0040bc70(self->manager, nullptr, box, 1, 1);                    // 006CC14B..153
}

// 006CB0E0 CSWGuiSaveLoad::LoadGame(CSWGuiControl*) RET 4: D83 FUNCTION TRAP. The body (0x740 bytes) tears down the
// menu and creates the server (CAppManager::CreateServer 00401380 at 006CB331 / 006CB54E) -> module load, Milestone 2.
// Reached only by save-entry Load activation (VerifyLoadGame 006CC109 direct; message-box callback 006CC13E).
void __thiscall SaveLoadHandlers::LoadGame_006cb0e0(kih::Control* entry) {
    kotor_fastcall_006cb0e0(this, nullptr, entry);                                 // loadgame: the reconstructed body (thiscall RET 4)
}
// 006CBB60 CSWGuiSaveLoad::HandleSaveButton(CSWGuiControl*) RET 4 (savegui lane body). Registered only when the panel
// is in save mode (ctor 006CCA5A with mode != 0; PopulateGameList 006CC314 / 006CC482 with [this+64h] bit0); the main
// menu constructs mode 0 (0067B1FB). Contains every use of PromptForSaveName 006CB820 (006CBE4E, 006CC022).
void __thiscall SaveLoadHandlers::HandleSaveButton_006cbb60(kih::Control* control) {
    kotor_fastcall_006cbb60(this, nullptr, control);                               // savegui: the reconstructed body (thiscall RET 4)
}

// ================================================================ CSWGuiRadioButton mouse leaves
// 00418680 CSWGuiRadioButton::HandleLMouseDown() RET.
void __thiscall RadioButtonLeaves::HandleLMouseDown_00418680() {
    if (!(flags & 8u)) return;                                                     // 00418683..687
    (this->*(vtable->handle_input_event))(0, 1);                                   // 00418689..68F vslot +3C (0, 1)
    (this->*(cvt()->handle_focus_change))(1);                                      // 00418692..698 vslot +7C (1)
    static_cast<kih::Manager*>(kotor_input_cells::gui_manager_007a39f4)->CaptureMouse(this, 1);   // 0041869B..6A4
}
// 00418E90 CSWGuiRadioButton::HandleLMouseUp() RET (tail jump through vslot +14).
void __thiscall RadioButtonLeaves::HandleLMouseUp_00418e90() {
    kih::Control* over = static_cast<kih::Manager*>(kotor_input_cells::gui_manager_007a39f4)->mouse_over;   // 00418E90..E98
    if (over && (over == this || over->GetSelectableParent() == this) &&          // 00418E9B..EAA
        (static_cast<kih::Manager*>(kotor_input_cells::gui_manager_007a39f4)->flags & 1u)) {   // 00418EAC..EB6
        (this->*(vtable->handle_input_event))(0x27, 1);                            // 00418EB8..EC0
        (this->*(vtable->handle_input_event))(0x1F9, 1);                           // 00418EC3..ECE
    }
    static_cast<kih::Manager*>(kotor_input_cells::gui_manager_007a39f4)->ReleaseMouse(this);   // 00418ED1..ED8
    (this->*(vtable->release_capture))();                                          // 00418EDD..EE2 jmp [edx+14h]
}

extern "C" {
// 006BE360 CSWGuiRadioButton::~CSWGuiRadioButton: vptr 007566D8, jmp CSWGuiButton::~CSWGuiButton 00419DA0.
void __fastcall kotor_m1_radio_button_destruct_006be360(Button* self, void*) noexcept(false) {
    set_vptr(self, &kotor_m1_radio_button_methods_007566d8);
    gui_load_button_destruct_00419da0(self, nullptr);
}
// 006B60D0 scalar deleting destructor RET 4.
void* __fastcall kotor_m1_radio_button_deleting_destruct_006b60d0(Button* self, void*, Word flags) noexcept(false) {
    kotor_m1_radio_button_destruct_006be360(self, nullptr);                        // 006B60D3
    if (flags & 1u) live_free(self);                                               // 006B60D8..E0
    return self;
}

// ================================================================ CSWGuiSaveLoadEntry
// 006CB940 CSWGuiSaveLoadEntry::CSWGuiSaveLoadEntry() RET.
SaveLoadEntry* __fastcall kotor_m1_saveload_entry_construct_006cb940(SaveLoadEntry* self, void*) noexcept(false) {
    gui_load_button_construct_0041c0f0(self, nullptr);                             // 006CB960
    unsigned char* const strings[8] = { self->save_name, self->dir_suffix, self->area_name, self->last_module,
                                        self->s1f8, self->title, self->screenshot, self->live_missing };
    int built = 0;
    try {
        set_vptr(self, &kotor_m1_saveload_entry_methods_00757740);                 // 006CB975
        for (; built < 8; ++built) ::new (static_cast<void*>(strings[built])) String;   // 006CB97B..9EB (+1D8..+210)
        for (ResRef& portrait : self->portraits) c2da_default_ref_00405ed0(&portrait, nullptr);   // 006CB9F5..A0B
        // 006CBA0D..1B: AND [+1C4],0FFFFFF20h on storage no constructor initialises (operator new 006CC295): the
        // original keeps the prior heap bits 5 and 8..31. D2 deviation: value-initialised to 0 first.
        self->entry_flags = 0;                                                     // D2 (006CBA0D)
        self->entry_flags &= 0xFFFFFF20u;                                          // 006CBA13..1B
        self->gameplay_hint = 0;                                                   // 006CBA28
        self->story_hint = 0;                                                      // 006CBA2F
        self->number = 0;                                                          // 006CBA36
        self->time_played = 0;                                                     // 006CBA3C
        self->u1d0 = 0;                                                            // 006CBA42
        GuiAssign005e5140(str(self->save_name), nullptr, kBad);                    // 006CBA21..48
    } catch (...) {
        while (built-- > 0) str(strings[built])->~String();
        gui_load_button_destruct_00419da0(self, nullptr);
        throw;
    }
    return self;
}
// 006CBA90 CSWGuiSaveLoadEntry::~CSWGuiSaveLoadEntry: strings +210..+1D8 (reverse), vptr 007566D8, button dtor.
void __fastcall kotor_m1_saveload_entry_destruct_006cba90(SaveLoadEntry* self, void*) noexcept(false) {
    str(self->live_missing)->~String();                                            // 006CBABB
    str(self->screenshot)->~String();                                              // 006CBACB
    str(self->title)->~String();                                                   // 006CBADB
    str(self->s1f8)->~String();                                                    // 006CBAEB
    str(self->last_module)->~String();                                             // 006CBAFB
    str(self->area_name)->~String();                                               // 006CBB0B
    str(self->dir_suffix)->~String();                                              // 006CBB1B
    str(self->save_name)->~String();                                               // 006CBB2B
    set_vptr(self, &kotor_m1_radio_button_methods_007566d8);                       // 006CBB3A
    gui_load_button_destruct_00419da0(self, nullptr);                              // 006CBB40
}
// 006CBA70 scalar deleting destructor RET 4.
void* __fastcall kotor_m1_saveload_entry_deleting_destruct_006cba70(SaveLoadEntry* self, void*, Word flags) noexcept(false) {
    kotor_m1_saveload_entry_destruct_006cba90(self, nullptr);                      // 006CBA73
    if (flags & 1u) live_free(self);                                               // 006CBA78..80
    return self;
}

// 006C8200 CSWGuiSaveLoadEntry portrait getter (hidden CResRef*, int index) RET 8.
ResRef* __fastcall kotor_m1_saveload_entry_get_portrait_006c8200(SaveLoadEntry* self, void*, ResRef* out, I32 index) noexcept(false) {
    if (index >= 0 && index < 3) {                                                 // 006C8204..20C
        std::memcpy(out, &self->portraits[index], 16);                             // 006C820E..233 four dword moves
        return out;
    }
    c2da_default_ref_00405ed0(out, nullptr);                                       // 006C823A..240
    return out;
}
// 006C8250 CSWGuiSaveLoadEntry::GetGameDirectory(hidden CExoString*) RET 4:
// Format(tmp, "SAVES:%06d - %s", number, dir_suffix by value (data, capacity words)); copy-construct out; ~tmp.
String* __fastcall kotor_m1_saveload_entry_get_game_directory_006c8250(SaveLoadEntry* self, void*, String* out) noexcept(false) {
    String formatted;                                                              // 006C8277
    const StartupString* suffix = str(self->dir_suffix);
    char* const data = suffix->data;                                               // 006C8282 [esi+1E0h]
    const std::uint32_t capacity = suffix->capacity;                               // 006C827C [esi+1E4h]
    kotor_reconstruction::StartupFormat005e5680(&formatted, kGameDirFormat, self->number, data, capacity);  // 006C8288..2A3
    WindowCopy005e5b00(out, nullptr, &formatted);                                  // 006C82AF..2B6
    return out;                                                                    // 006C82CC ~formatted
}

// 006C8E50 CSWGuiSaveLoadEntry::LoadData(CExoString const& directoryName) RET 4.
void __fastcall kotor_m1_saveload_entry_load_data_006c8e50(SaveLoadEntry* self, void*, String* name) noexcept(false) {
    String portrait_label;                                                         // [esp+40] 006C8E7A
    String directory;                                                              // [esp+2C] 006C8E8E
    String unused58;                                                               // [esp+58] 006C8E9F
    std::int32_t separator;
    {
        String dash(kSeparator);                                                   // 006C8EA4..B5
        separator = client_movies::StringFind_005e5eb0(find_view(name), nullptr, find_view(&dash), 0);   // 006C8EBA..ED3
    }                                                                              // 006C8EE1
    {
        Produced left;
        String* produced = left.take(base_ini_reconstruction::left_005e5f70(name, nullptr, left.slot(), separator));  // 006C8EE6..EEE
        self->number = reinterpret_cast<kotor_menu20::String*>(produced)->as_int_005e5630();                       // 006C8EF3..F06
    }                                                                              // 006C8F14
    {
        const std::int32_t length = name->BorrowedLength005e5790();                // 006C8F19..1B
        Produced right;
        base_ini_reconstruction::right_005e6170(name, nullptr, right.slot(), length + (-3 - separator));  // 006C8F20..31
        right.live = true;
        str(self->dir_suffix)->Assign005e5c50(right.slot());                       // 006C8F36..45
    }                                                                              // 006C8F56
    {
        String saves(kSaves);                                                      // 006C8F5B..64 [esp+48]
        Produced joined;
        GuiConcat005e5d10(&saves, nullptr, joined.slot(), name);                   // 006C8F69..7A
        joined.live = true;
        directory.Assign005e5c50(joined.slot());                                   // 006C8F7F..8C
    }                                                                              // 006C8F9C ~joined, 006C8FAD ~saves
    resman()->directory_00408800(&directory);                                      // 006C8FB2..FBD AddResourceDirectory
    Produced resolved;                                                             // [esp+50] ResolveFileName output
    {
        String corrupt(kCorrupt);                                                  // [esp+68] 006C8FC2..CB
        String separator_text(kBackslash);                                         // [esp+60] 006C8FD0..E1
        Produced with_separator;                                                   // [esp+88]
        GuiConcat005e5d10(&directory, nullptr, with_separator.slot(), &separator_text);   // 006C8FE6..FFF
        with_separator.live = true;
        Produced with_name;                                                        // [esp+80]
        GuiConcat005e5d10(with_separator.slot(), nullptr, with_name.slot(), &corrupt);    // 006C9004..1B
        with_name.live = true;
        path_resolution::resolve_005e68a0(static_cast<alias_path::AliasListView*>(alias_list()), nullptr,
                                          resolved.slot(), with_name.slot(), 0);   // 006C9020..38
        resolved.live = true;
    }   // 006C903D ~[80], 006C9051 ~[88], 006C9065 ~[60], 006C9076 ~[68]: ~with_name, ~with_separator, ~separator_text, ~corrupt
    alignas(4) unsigned char file_storage[4];                                      // [esp+34] CExoFile
    tlk_file::File* const file = reinterpret_cast<tlk_file::File*>(file_storage);
    {
        String mode(kReadBinary);                                                  // [esp+38] 006C9087..90
        String corrupt(kCorrupt);                                                  // [esp+48] 006C9095..A6
        String separator_text(kBackslash);                                         // [esp+70] 006C90AB..BC
        Produced with_separator;                                                   // [esp+90]
        GuiConcat005e5d10(&directory, nullptr, with_separator.slot(), &separator_text);   // 006C90C1..DA
        with_separator.live = true;
        Produced with_name;                                                        // [esp+78]
        GuiConcat005e5d10(with_separator.slot(), nullptr, with_name.slot(), &corrupt);    // 006C90DF..F3
        with_name.live = true;
        file_internal::exofile_construct_005e68d0(file, nullptr, with_name.slot(), 0, &mode);   // 006C90F8..10C
    }   // 006C9111 ~[78], ~[90], ~[70], ~[48], ~[38]
    struct FileGuard { tlk_file::File* f; ~FileGuard() { file_destroy_005e69e0(f, nullptr); } } file_guard{file};  // 006C96B1..B9
    if (file_opened_005e6a10(file, nullptr)) {                                     // 006C916A..175
        self->entry_flags |= 3u;                                                   // 006C9177..184 corrupt
        kotor_fastcall_004088d0(resman(), nullptr, &directory);                    // 006C918A..191
        return;                                                                    // 006C9196 -> 006C96AD
    }
    void* memory = live_operator_new(0xA0);                                        // 006C919B..1A0
    gui_load::Gff* gff = nullptr;
    if (memory) {
        FreeOnUnwind guard{memory, true};                                          // EH state 1Ch
        ResRef nfo; nfo.construct_00406d80(kSaveNfo);                              // 006C91BA..1C6
        gff = gui_load_gff_construct_00410630(static_cast<gui_load::Gff*>(memory), nullptr, 0, kNfoType, &nfo);  // 006C91CB..1DC
        guard.armed = false;
    }
    gui_load::Struct top;                                                          // [esp+0C]
    gff->GetTopLevelStruct(&top);                                                  // 006C91E7..1F5 (00411240, no null check)
    Word success;                                                                  // [esp+10]
    auto read_string = [&](const char* label, unsigned char* destination) {        // ReadFieldCExoString(.., label, &success, "")
        String empty(kEmpty);
        Produced value;
        gui_load_gff_string_00411ec0(gff, nullptr, value.slot(), &top, label, &success, &empty);
        value.live = true;
        str(destination)->Assign005e5c50(value.slot());
    };                                                                             // ~value, ~empty
    read_string("AREANAME", self->area_name);                                      // 006C91FA..260 (00745418)
    read_string("LASTMODULE", self->last_module);                                  // 006C9265..2CB (0074540C)
    self->time_played = gui_load_gff_dword_00411c20(gff, nullptr, &top, "TIMEPLAYED", &success, 0);   // 006C92D0..2F1 (00745400)
    read_string("SAVEGAMENAME", self->save_name);                                  // 006C92E8..354 (007453E4)
    if (!success) {                                                                // 006C9359..35F
        String old(kOldSave);                                                      // 006C9361..36A
        str(self->save_name)->Assign005e5c50(&old);                                // 006C936F..37E
    }
    auto read_flag = [&](const char* label, unsigned shift) {                      // ReadFieldBYTE(.., 0) -> single bit
        const Word value = gui_load_gff_byte_00411a60(gff, nullptr, &top, label, &success, 0);
        const Word flags = self->entry_flags;
        self->entry_flags = flags ^ (((value << shift) ^ flags) & (1u << shift));
    };
    read_flag("CHEATUSED", 7);                                                     // 006C9393..3C0 (007453F4)
    read_flag("REBOOTAUTOSAVE", 3);                                                // 006C93C6..3F1 (00757590)
    read_flag("PCAUTOSAVE", 4);                                                    // 006C93F7..422 (00745574)
    read_string("SCREENSHOT", self->screenshot);                                   // 006C9428..48E (00745568)
    self->gameplay_hint = gui_load_gff_byte_00411a60(gff, nullptr, &top, "GAMEPLAYHINT", &success,
                                                     self->gameplay_hint);         // 006C9493..4B8 (007453D4)
    self->story_hint = gui_load_gff_byte_00411a60(gff, nullptr, &top, "STORYHINT", &success,
                                                  self->story_hint);               // 006C94BE..4F6 (007453C8)
    const unsigned char live = gui_load_gff_byte_00411a60(gff, nullptr, &top, "LIVECONTENT", &success, 0);  // 006C94E3..503 (007453B4)
    if (live != 0) {                                                               // 006C9507
        String live_label;                                                         // [esp+24] 006C950D
        for (std::uint32_t i = 1; i < kotor_rdata_live_count_0073ecd2; ++i) {      // 006C9516..52D / 006C95C0..5D2
            kotor_reconstruction::StartupFormat005e5680(&live_label, kLiveFormat, i);   // 006C9533..547
            Produced value;                                                        // [esp+18]
            gui_load_gff_string_00411ec0(gff, nullptr, value.slot(), &top, WindowCStr005e5670(&live_label, nullptr),
                                         &success, &live_label);                   // 006C954A..56A
            value.live = true;
            StartupString* path = kotor_fastcall_005e6890(alias_list(), nullptr, &live_label, 0);   // 006C956F..587 GetAliasPath
            if (path->BorrowedLength005e5790() == 0 && (live & (1u << (i - 1)))) {  // 006C958C..5A9
                self->entry_flags |= 0x21u;                                        // 006C971C
                str(self->live_missing)->Assign005e5c50(value.slot());             // 006C9723..72E
                if (gff) vt<GffCallView>(gff)->deleting_destroy(gff, nullptr, 1);  // 006C9733..73D vslot +00 (1)
                kotor_fastcall_004088d0(resman(), nullptr, &directory);            // 006C973F..74A
                return;                                                            // 006C974F ~value, 006C9760 ~live_label, -> 006C96AD
            }
        }                                                                          // 006C95AF ~value
    }                                                                              // 006C95D8 ~live_label
    for (int i = 0; i < 3; ++i) {                                                  // 006C95E8..669
        kotor_reconstruction::StartupFormat005e5680(&portrait_label, kPortraitFormat, kPortrait, i);   // 006C95F4..609
        ResRef none; none.construct_00406d80(kEmpty);                              // 006C960C..618 [esp+A8]
        ResRef read;                                                               // [esp+B8]
        ResRef* produced = gui_load_gff_resref_00411e10(gff, nullptr, &read, &top,
            WindowCStr005e5670(&portrait_label, nullptr), &success, &none);        // 006C961D..643
        ResRef assigned;                                                           // [esp+C8] hidden return of operator=
        c2da_copy_00406120(&self->portraits[i], nullptr, &assigned, produced);     // 006C9648..655
    }
    if (gff) vt<GffCallView>(gff)->deleting_destroy(gff, nullptr, 1);              // 006C966B..675 vslot +00 (1)
    kotor_fastcall_004088d0(resman(), nullptr, &directory);                        // 006C9677..682
    self->entry_flags &= 0xFFFFFFDDu;                                              // 006C9687..68D
    GuiAssign005e5140(str(self->live_missing), nullptr, kEmpty);                   // 006C9690..6A1
    self->entry_flags |= 1u;                                                       // 006C96A6
}   // 006C96AD ~file, 006C96BE ~resolved, ~unused58, ~directory, ~portrait_label

// 006C9780 CSWGuiSaveLoadEntry::SetXboxTitle(CExoString const&) RET 4.
void __fastcall kotor_m1_saveload_entry_set_xbox_title_006c9780(SaveLoadEntry* self, void*, String* name) noexcept(false) {
    String* const title = str(self->title);
    title->Assign005e5c50(name);                                                   // 006C97A6..7B3
    String unused28;                                                               // [esp+28] 006C97BC
    if (self->number == 1) {                                                       // 006C97C1..7D2
        Produced text; title->Assign005e5c50(simple_string(text, 0x639));          // 006C97D4..811
    } else if (self->number == 0) {                                                // 006C97F0..7F2
        Produced text; title->Assign005e5c50(simple_string(text, 0xBB77));         // 006C97F4..811
    }                                                                              // 006C981F
    String message;                                                                // [esp+18] 006C9828
    const std::uint32_t seconds = self->time_played;                               // 006C982D
    const std::uint32_t hours = seconds / 3600u;                                   // 006C9833..83A / 006C9856 (MUL 91A2B3C5h, SHR 0Bh)
    const std::uint32_t minutes = (seconds / 60u) % 60u;                           // 006C983C..84F (MUL 88888889h, SHR 5, DIV 3Ch)
    auto token = [&](Word index, int value) {                                      // CExoString(int) 005E5BC0 + SetCustomToken
        alignas(4) unsigned char bytes[8];
        String* text = resman_string_from_int_005e5bc0(reinterpret_cast<String*>(bytes), nullptr, value);
        try { kotor_fastcall_0041db50(kotor_cell_tlk_007a3a08, nullptr, index, text); }
        catch (...) { text->~String(); throw; }
        text->~String();
    };
    if (self->number != 0 && self->number != 1) {                                  // 006C9861..876
        token(0, self->number - 1);                                                // 006C987C..8A6
        token(1, static_cast<int>(hours));                                         // 006C98AB..8D4
        token(2, static_cast<int>(minutes));                                       // 006C98D9..906
        kotor_fastcall_0041db50(kotor_cell_tlk_007a3a08, nullptr, 3, str(self->save_name));   // 006C990B..91D
        { Produced text; message.Assign005e5c50(simple_string(text, 0xBD87)); }    // 006C9922..94E
        tlk_core::ParseStr0041dd30(tlk(), nullptr, &message);                      // 006C9953..95E
        title->Assign005e5c50(&message);                                           // 006C9963..96A
    } else {
        token(0, static_cast<int>(hours));                                         // 006C9974..99D
        token(1, static_cast<int>(minutes));                                       // 006C99A2..9CF
        { Produced text; message.Assign005e5c50(simple_string(text, 0xBC4D)); }    // 006C99D4..A00
        tlk_core::ParseStr0041dd30(tlk(), nullptr, &message);                      // 006C9A05..A10
        Produced joined;
        GuiConcat005e5d10(title, nullptr, joined.slot(), &message);                // 006C9A15..A21
        joined.live = true;
        title->Assign005e5c50(joined.slot());                                      // 006C9A26..A2E
    }                                                                              // 006C9A3B
    gui_load_text_params_set_text_00415e00(bytes_of(self) + 0x16C, nullptr, title);   // 006C9A40..A4B
}   // 006C9A59 ~message, 006C9A6A ~unused28
} // extern "C"
