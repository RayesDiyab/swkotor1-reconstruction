#pragma once
// ui-save-state lane private declarations.
//
// This header declares accepted shared providers, accepted shared storage and the
// exact callee contracts used by the five owned original addresses. It defines no
// shared type body, no vtable and no constructor/destructor body. 007A3A00 is a
// declaration only (its definition/initializer belong to the accepted 0456/0854
// objects). 00832CA8 is declared here and defined exactly once by this lane's
// storage-only unit source/ui_save_state_00832ca8.cpp (no initializer function).

#include <cstddef>
#include <cstdint>
#include <cstring>

// ---------------------------------------------------------------------------
// CExoString / StartupString: accepted 8-byte layout (0012/0078/0125 objects).
// Declarations only; every method is defined by the accepted string objects.
// ---------------------------------------------------------------------------
namespace kotor_reconstruction {

#pragma pack(push, 1)
struct StartupString {
    char* data;              // +0x00 allocated NUL-terminated copy
    std::uint32_t capacity;  // +0x04 byte count including the NUL
    StartupString();
    StartupString(char const* text);
    ~StartupString();
    StartupString* Assign005e5c50(StartupString* rhs);
};
#pragma pack(pop)

static_assert(sizeof(StartupString) == 8, "accepted CExoString layout");
static_assert(offsetof(StartupString, data) == 0, "accepted CExoString data offset");
static_assert(offsetof(StartupString, capacity) == 4, "accepted CExoString length offset");

// Trivial 8-byte view used only for a vtable slot whose parameter is a CExoString
// by value. It is not a second CExoString implementation; the deep copy is made by
// the accepted @WindowCopy005e5b00@12 and the callee owns/destroys the argument.
struct RawString {
    void* data;
    std::uint32_t capacity;
};
static_assert(sizeof(RawString) == 8, "by-value argument layout");

}  // namespace kotor_reconstruction

// ---------------------------------------------------------------------------
// TLK accepted types (0389/0037 objects). Layout is the same 8-byte string.
// ---------------------------------------------------------------------------
namespace kotor_menu_10_19 {

#pragma pack(push, 1)
struct String {
    char* data;
    std::uint32_t capacity;
};
#pragma pack(pop)
static_assert(sizeof(String) == 8, "accepted TLK string layout");

struct Tlk {
    // Exact MAP symbol:
    //   ?simple_0041e8f0@Tlk@kotor_menu_10_19@@QAEPAUString@2@PAU32@I@Z
    // The parameter spelling `PAU32@` is an MSVC name backreference: 3 = the
    // `String` name already used by the return type, 2@ = kotor_menu_10_19.
    // It is NOT a literal type named U32.  The accepted 0037 object was compiled
    // from simple_string.cpp with `String* Tlk::simple_0041e8f0(String*, Word)`;
    // the same encoding appears as `PAU12@`/`PBU12@` in copy_005e5b00.  Keep the
    // declaration as String* so the selected symbol joins exactly.
    String* simple_0041e8f0(String* out, unsigned int arg);
};

namespace dependencies {
extern void* base_007a39e0;
}

}  // namespace kotor_menu_10_19

namespace tlk_core {
void __fastcall ParseStr0041dd30(kotor_menu_10_19::Tlk* tlk, void* unused,
                                 kotor_reconstruction::StartupString* text);
}

// ---------------------------------------------------------------------------
// Accepted shared storage cells (exact v5 symbols; no definitions here).
// ---------------------------------------------------------------------------
namespace frame_control {
extern std::uint32_t resource_manager_007a39e8;  // 0151-manager-cell-owner.obj
extern std::uint32_t sound_manager_007a39ec;      // 0673-sound-owners.obj
extern std::uint32_t message_manager_007a3a00;    // 0456-r4-lowers-r4-cells.obj
}

namespace kotor_menu_window_cells {
extern void* volatile manager_007a39fc;           // 0053-window-shared-cells.obj
}

namespace w4 {
extern const volatile std::uint32_t ingame_gui_single_panel_007a2370;  // 1205 object
}

extern "C" {
extern void* kotor_cell_tlk_007a3a08;             // 0384-tlk-cells.obj
// 00832CA8: single canonical 4-byte zero-initialized BSS cell owned by this lane.
// Defined once in ui_save_state_00832ca8.cpp; no initializer function, no writer,
// no trap.  The original has two direct reads and no direct writer.
extern unsigned int kotor_cell_00832ca8;
}

// ---------------------------------------------------------------------------
// Accepted CSWGuiSaveLoadEntry field names/offsets (saveload.h:55-80).
// This is a named-offset view, not a new object layout: the accepted header asserts
// sizeof(SaveLoadEntry)==0x248 and these offsets.  +0x1F0 is last_module[8]; the
// original passes the array address (&last_module[0]) at 00633F7A and 006340AB/CE.
// +0x210 is live_missing, also named in the accepted header.
// ---------------------------------------------------------------------------
namespace kotor_m1_saveload_offsets {
constexpr std::size_t entry_flags = 0x1C4;
constexpr std::size_t number = 0x1C8;
constexpr std::size_t time_played = 0x1CC;
constexpr std::size_t gameplay_hint = 0x1D4;
constexpr std::size_t story_hint = 0x1D5;
constexpr std::size_t save_name = 0x1D8;
constexpr std::size_t dir_suffix = 0x1E0;
constexpr std::size_t area_name = 0x1E8;
constexpr std::size_t last_module = 0x1F0;  // unsigned char last_module[8]
constexpr std::size_t title = 0x200;
constexpr std::size_t screenshot = 0x208;
constexpr std::size_t live_missing = 0x210;  // unsigned char live_missing[8]
constexpr std::size_t object_size = 0x248;
}

// ---------------------------------------------------------------------------
// Exact accepted callees (C symbols; decoration matches the v5 MAP).
// ---------------------------------------------------------------------------
extern "C" {

void* __fastcall kotor_fastcall_005eda40(void* internal, void* unused);  // 005EDA40
void __fastcall kotor_fastcall_0040bd20(void* manager, void* unused, void* panel);  // 0040BD20
void __fastcall frame_RemovePanel_0040c830(void* manager, void* unused, void* panel);  // 0040C830
void __fastcall frame_R4_005d0fc0(void* vm, void* unused,
                                  kotor_reconstruction::StartupString* script,
                                  unsigned int arg1, unsigned int arg2);  // 005D0FC0
void __fastcall kotor_fastcall_0040bc70(void* manager, void* unused, void* panel,
                                        unsigned int arg2, unsigned int arg3);  // 0040BC70
void __fastcall kotor_fastcall_0040be00(void* manager, void* unused);  // 0040BE00
int __fastcall gui_load_resman_create_directory_004068c0(
    void* resman, void* unused, kotor_reconstruction::StartupString* name);  // 004068C0
void __fastcall kotor_fastcall_004088d0(
    void* resman, void* unused, kotor_reconstruction::StartupString* name);  // 004088D0
void __fastcall gui_load_resman_clean_directory_00409460(
    void* resman, void* unused, kotor_reconstruction::StartupString* name,
    unsigned int arg2, unsigned int arg3);  // 00409460
void __fastcall kotor_fastcall_0041db50(
    void* tlk, void* unused, unsigned int token,
    kotor_reconstruction::StartupString* text);  // 0041DB50
void __fastcall kotor_m15_client_show_load_screen_005eda80(
    void* internal, void* unused, unsigned int arg1, unsigned int arg2,
    unsigned int arg3);  // 005EDA80
void __fastcall kotor_m15_client_set_load_screen_by_module_005edcf0(
    void* internal, void* unused, kotor_reconstruction::StartupString* module);  // 005EDCF0
void __fastcall kotor_m15_client_set_load_screen_hint_005edf00(
    void* internal, void* unused, unsigned int hint);  // 005EDF00
void __fastcall gui_load_messagebox_set_callback_00624a40(
    void* box, void* unused, unsigned int a, unsigned int b, unsigned int c);  // 00624A40
void __fastcall kotor_fastcall_00627130(
    void* box, void* unused, unsigned int allow_cancel);  // 00627130
void __fastcall frame_R4_0062cba0(
    void* gui, void* unused, unsigned int hidden);  // 0062CBA0
void __fastcall kotor_fastcall_00624bd0(
    void* menu, void* unused, unsigned int index, unsigned int one);  // 00624BD0
void __fastcall kotor_m15_ingame_start_global_fade_0062abf0(
    void* gui, void* unused, unsigned int arg1, unsigned int arg2,
    float duration, void* block);  // 0062ABF0

// Nested-provider consumer names (coordinator ruling). Bodies are owned by the
// nested-providers lane and are not authored/stubbed/aliased here.
void __fastcall kotor_fastcall_005f4460(void* internal, void* unused);  // 005F4460
void __fastcall kotor_fastcall_005ed670(
    void* internal, void* unused, unsigned int zero,
    kotor_reconstruction::StartupString* strB,
    kotor_reconstruction::StartupString* strA);  // 005ED670

// Accepted string providers (0012 / 0078).
kotor_reconstruction::StartupString* __fastcall WindowCopy005e5b00(
    kotor_reconstruction::StartupString* dest, void* unused,
    kotor_reconstruction::StartupString const* source);  // 005E5B00

// Element-destructor callback for the EH vector helper. The accepted body is the
// 0012 dtor; the alternatename pragma in ui_00633c50.cpp binds this exact view to it.
void __fastcall startup_string_element_destroy_005e5c20(void* element, void* unused);

// Accepted saveload-entry providers (0796 object).
void* __fastcall kotor_m1_saveload_entry_construct_006cb940(void* memory, void* unused);
void __fastcall kotor_m1_saveload_entry_load_data_006c8e50(
    void* entry, void* unused, kotor_reconstruction::StartupString* element);

// Accepted allocator and EH vector helper (0004 / 0186 objects).
void* __cdecl _live_operator_new(unsigned int bytes);
void __cdecl _live_free(void* memory);
void __stdcall c2da_destroy_array_006fa852(
    void* first, unsigned int size, int count,
    void (__fastcall* destroy)(void*, void*));

}  // extern "C"

namespace menu_tables_required {
void __fastcall SoundSetMode_005d5e80(void* sound, void* unused, unsigned int mode);  // 005D5E80
}

namespace f200_ports {
void __fastcall Enumerate005e6640(void* base, void* unused, void* vector,
                                  kotor_reconstruction::StartupString* directory,
                                  unsigned int flags, unsigned int one1,
                                  unsigned int one2);  // 005E6640
}

namespace kotor_input_required {
void* __fastcall get_in_game_gui_005ed690(void* internal, void* unused);  // 005ED690
}

// Accepted party-table provider (0811 object). C symbol; the caller leaves ECX stale.
extern "C" void __cdecl kotor_m15_party_reset_session_start_time_00563cf0();  // 00563CF0

// ---------------------------------------------------------------------------
// Bind-only load-screen contracts. No exact accepted symbol/object was supplied in
// this lane's inputs; these are unresolved external contracts with exact original
// ABI. They are not definitions, stubs or aliases.
// ---------------------------------------------------------------------------
extern "C" {
void __fastcall load_screen_set_load_from_save_game_004af060(
    void* server, void* unused, unsigned int one);
void __fastcall load_screen_send_load_game_request_005ed680(
    void* internal, void* unused, int number, void* suffix, void* module);
void __fastcall load_screen_set_load_screen_hint_update_timer_005ee040(
    void* internal, void* unused, unsigned int zero);
void __fastcall load_screen_set_gameplay_hint_005ee060(
    void* internal, void* unused, unsigned int hint);
void __fastcall load_screen_set_story_hint_005ee080(
    void* internal, void* unused, unsigned int hint);
void __fastcall load_screen_busy_wait_mute_sound_005ee130(void* internal, void* unused);
int __fastcall load_screen_unpack_quick_save_game_006323a0(
    void* self, void* unused, int number, void* suffix);
void __fastcall load_screen_update_created_in_game_gui_0062b1e0(
    void* self, void* unused, unsigned int old_index, unsigned int new_index);
void __fastcall load_screen_set_load_music_005edd30(
    void* internal, void* unused, kotor_reconstruction::StartupString* name);
unsigned int __fastcall load_screen_get_next_load_screen_hint_strref_005ee090(
    void* internal, void* unused, kotor_reconstruction::StartupString* name,
    unsigned int arg);
}

// ---------------------------------------------------------------------------
// Owned original-address definitions (exact accepted 0769 import names).
// ---------------------------------------------------------------------------
namespace kotor_input_required {
void __fastcall do_quick_load_00633c50(void* self, void* unused);
void __fastcall do_quick_save_005f4b50(void* self, void* unused);
void __fastcall hurry_death_sequence_005f2f20(void* self, void* unused);
void __fastcall player_flourish_weapons_005ede90(void* app, void* unused);
void __fastcall switch_to_sw_in_game_gui_0062cf10(void* self, void* unused,
                                                  std::uint32_t index);
}

// ---------------------------------------------------------------------------
// Opaque field accessors (no shared object layout is authored here).
// ---------------------------------------------------------------------------
template <class T>
inline T ui_load_at(void const* base, std::size_t offset) {
    T value;
    std::memcpy(&value, static_cast<unsigned char const*>(base) + offset, sizeof(T));
    return value;
}

template <class T>
inline void ui_store_at(void* base, std::size_t offset, T value) {
    std::memcpy(static_cast<unsigned char*>(base) + offset, &value, sizeof(T));
}
