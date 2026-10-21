// Authored packet source — reviewed, unbuilt; provider/EH gates remain.
//
// Exact provider/data declarations and POD views for the five
// dialog-core-dependencies bodies. New private header; no shared edit.

#pragma once

#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <cstdarg>

using U8 = std::uint8_t;
using U16 = std::uint16_t;
using U32 = std::uint32_t;
using I32 = std::int32_t;
using U64 = std::uint64_t;
using std::uintptr_t;

// ---------------------------------------------------------------------------
// KOTOR POD views (exact offsets from the Ghidra KOTOR type database).
// ---------------------------------------------------------------------------
struct CExoString {
    void* data;
    U32 length;
};
static_assert(sizeof(CExoString) == 8, "CExoString");

namespace kotor_reconstruction { struct StartupString; }
namespace alias_path { struct AliasListView; }
namespace options_video { struct String; }
namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } }

struct CResRef {
    char bytes[16];
};
static_assert(sizeof(CResRef) == 16, "CResRef");

struct CExoLocString {
    void* strref;
    void* internal;
};
static_assert(sizeof(CExoLocString) == 8, "CExoLocString");

struct CSWSDialogCamera {
    CExoString listener;             // +0x00
    U32 camera_angle;                // +0x08
    I32 camera_id;                   // +0x0C
    float camera_height_offset;      // +0x10
    float tar_height_offset;         // +0x14
    U16 camera_animation;            // +0x18
    U16 pad_1a;                      // +0x1A
    float camera_fov;                // +0x1C
    I32 camera_video_effect;         // +0x20
};
static_assert(sizeof(CSWSDialogCamera) == 0x24, "CSWSDialogCamera");

struct CSWSDialogBase {
    CExoString speaker;              // +0x00
    CExoLocString text;              // +0x08
    CResRef script;                  // +0x10
    U32 wait_flags;                  // +0x20
    U32 delay;                       // +0x24
    I32 field5_0x28;                 // +0x28
    U32 field6_0x2c;                 // +0x2C
    U32 field7_0x30;                 // +0x30
    CResRef sound;                   // +0x34
    CExoString field9_0x44;          // +0x44
    CExoString quest;                // +0x4C
    U32 quest_entry;                 // +0x54
    I32 plot_index;                  // +0x58
    float plot_xp_percentage;        // +0x5C
    U32 animation_count;             // +0x60
    void* animations;                // +0x64
    U32 fade_type;                   // +0x68
    float fade_color[3];             // +0x6C
    float fade_length;               // +0x78
    float fade_delay;                // +0x7C
    U8 sound_exists;                 // +0x80
    U8 pad_81[3];                    // +0x81
};
static_assert(sizeof(CSWSDialogBase) == 0x84, "CSWSDialogBase");

struct CSWSDialogNode {
    CSWSDialogBase base;             // +0x00
    CSWSDialogCamera camera;         // +0x84
};
static_assert(sizeof(CSWSDialogNode) == 0xA8, "CSWSDialogNode");

struct CSWSDialogLinkEntry {
    CResRef active;                  // +0x00
    U32 index;                       // +0x10
    U32 display_inactive;            // +0x14
};
static_assert(sizeof(CSWSDialogLinkEntry) == 0x18, "CSWSDialogLinkEntry");

struct CSWSDialogEntry {
    CSWSDialogNode node;             // +0x00
    U32 link_entry_count;            // +0xA8
    CSWSDialogLinkEntry* link_entries; // +0xAC
};
static_assert(sizeof(CSWSDialogEntry) == 0xB0, "CSWSDialogEntry");

struct CSWSDialogReply {
    CSWSDialogNode node;             // +0x00
    U32 field1_0xa8;                 // +0xA8
    U32 field2_0xac;                 // +0xAC
    I32 field3_0xb0;                 // +0xB0
};
static_assert(sizeof(CSWSDialogReply) == 0xB4, "CSWSDialogReply");

struct CExoLinkedListNode {
    CExoLinkedListNode* prev;        // +0x00
    CExoLinkedListNode* next;        // +0x04
    void* data;                      // +0x08
};
static_assert(sizeof(CExoLinkedListNode) == 0x0C, "CExoLinkedListNode");

struct CExoLinkedListInternal {
    CExoLinkedListNode* head;        // +0x00
    CExoLinkedListNode* tail;        // +0x04
    U32 count;                       // +0x08
};
static_assert(sizeof(CExoLinkedListInternal) == 0x0C, "CExoLinkedListInternal");

struct CSWSDialogPlayer {
    U32 field0;                      // +0x00
    U32 id;                          // +0x04
    U8 byte8;                        // +0x08
    U8 pad[3];
};
static_assert(sizeof(CSWSDialogPlayer) == 0x0C, "CSWSDialogPlayer");

struct CSWSDialog {
    void* vtable;                    // +0x00
    U32 entries_count;               // +0x04
    CSWSDialogEntry* entries;        // +0x08
    U32 reply_count;                 // +0x0C
    CSWSDialogReply* replies;        // +0x10
    U32 starting_count;              // +0x14
    void* starting_list;             // +0x18
    CResRef script_a;                // +0x1C
    CResRef script_b;                // +0x2C
    U32 f3c, f40, f44;               // +0x3C
    U32 stunt_model_count;           // +0x48
    void* stunt_models;              // +0x4C
    CResRef ref50;                   // +0x50
    U32 f60;                         // +0x60
    CExoLinkedListInternal* players; // +0x64
    U32 field17_0x68;                // +0x68
    U32 field18_0x6c;                // +0x6C
    U32 field19_0x70;                // +0x70
    U32 field20_0x74;                // +0x74
    U8 field21_0x78;                 // +0x78
    U8 pad_79[3];
    U32 player_object_id;            // +0x7C
    U32 owner_object_id;             // +0x80
    void* speakers;                  // +0x84
    U32 speaker_count;               // +0x88
    U32 field29_0x8c;                // +0x8C
    U32 f90;                         // +0x90
    U32 field31_0x94;                // +0x94
    U8 pad_98[0x74];                 // +0x98
    U32 field58_0x10c;               // +0x10C
    U32 f110;                        // +0x110
    void* list114;                   // +0x114
    void* list118;                   // +0x118
    U8 pad_11c[0x10];                // +0x11C
};
static_assert(offsetof(CSWSDialog, players) == 0x64, "CSWSDialog.players");
static_assert(offsetof(CSWSDialog, player_object_id) == 0x7C, "CSWSDialog.player");
static_assert(offsetof(CSWSDialog, owner_object_id) == 0x80, "CSWSDialog.owner");
static_assert(offsetof(CSWSDialog, field29_0x8c) == 0x8C, "CSWSDialog.field29");
static_assert(offsetof(CSWSDialog, field58_0x10c) == 0x10C, "CSWSDialog.field58");
static_assert(sizeof(CSWSDialog) == 0x12C, "CSWSDialog");

struct CSWSObject {
    void* vtable;                    // +0x00
    U32 id;                          // +0x04
    U8 pad_08[0x10];                 // +0x08
    CExoString tag;                  // +0x18
    U8 pad_20[0x14];                 // +0x20
    CSWSDialog* dialog;              // +0x34
    U8 pad_38[0x24];                 // +0x38
    U32 field23_0x5c;                // +0x5C
};
static_assert(offsetof(CSWSObject, tag) == 0x18, "CSWSObject.tag");
static_assert(offsetof(CSWSObject, dialog) == 0x34, "CSWSObject.dialog");
static_assert(offsetof(CSWSObject, field23_0x5c) == 0x5C, "CSWSObject.field23");

struct CSWSArea {
    U8 pad[0x190];
    U32* game_objects;               // +0x190
    U32 game_object_count;           // +0x194
};

// ---------------------------------------------------------------------------
// Selected real/PRIOR providers (exact symbols).
// ---------------------------------------------------------------------------
extern "C" void* __cdecl live_operator_new(U32 bytes);
extern "C" void __cdecl live_free(void* memory);

extern "C" void* __fastcall unresolved_list_next_005e9b90(void* list, void*, CExoLinkedListNode** cursor);
extern "C" void* __fastcall c2da_005e9c10(void* list, void*, void* node);

extern "C" void __stdcall c2da_destroy_array_006fa852(void* objects, U32 obj_size, I32 count, void* dtor);
extern "C" void __stdcall crt_eh_vector_constructor_006fb55f(void* dst, U32 obj_size, I32 count, void* ctor, void* dtor);

extern "C" void* __fastcall recovered_object_array_004aed70(void*, void*);
extern "C" unsigned __fastcall kotor_fastcall_004d8230(void*, void*, U32 id, void* out);
extern "C" void* __fastcall recovered_object_area_004cb120(void*, void*);
extern "C" U32 __fastcall kotor_fastcall_004aea40(void*, void*);
extern "C" U32 __fastcall kotor_fastcall_004cb480(void*, void*, U32, U32, I32, U32, I32);
extern "C" void* __fastcall frame_R4_005ed690(void*, void*);
extern "C" U32 __fastcall kotor_fastcall_005eda50(void*, void*, U32);
extern "C" void* __fastcall kotor_fastcall_005ed700(void*, void*);
extern "C" char __fastcall c2da_current_string_index_005e5600(void*, void*, I32);
extern "C" U32 __fastcall async_string_length_005e5790(void*, void*);
extern "C" int __fastcall exostring_compare_no_case_005e6450(void*, void*, const CExoString*);
extern "C" void __fastcall c2da_ref_to_string_00405f70(void*, void*, CExoString*);

// Exact selected 1568 donors under their C++ names.
namespace client_movies {
int __fastcall StringNotEqual_005e5390(options_video::String*, void*, const char*);
}
namespace ini_comparison {
int __fastcall CompareNC005e58c0(kotor_reconstruction::StartupString*, void*, const char*);
}
namespace path_resolution {
kotor_reconstruction::StartupString* __fastcall resolve_005e68a0(
    alias_path::AliasListView*, void*, kotor_reconstruction::StartupString* out,
    kotor_reconstruction::StartupString* name, unsigned type);
}

extern "C" int __fastcall kotor_fastcall_004cd500(void*, void*);
extern "C" int __fastcall kotor_fastcall_004cb4f0(void*, void*);

namespace kotor_menu_window_cells {
extern void* volatile manager_007a39fc;
}
extern "C" U8 kotor_cell_disable_sound_00832894;

namespace kotor_menu_10_19 {
struct String {
    void* data;
    U32 length;
    String* __thiscall default_005b3190();
    String* __thiscall copy_005e5b00(const String* source);
    void __thiscall destroy_005e5c20();
    const char* __thiscall text_005e5670() const;
};
static_assert(sizeof(String) == 8, "String");
}

namespace kotor_reconstruction {
struct StartupString {
    void* data;
    U32 length;
    StartupString* __thiscall Assign005e5c50(StartupString* source);
};
void __cdecl StartupFormat005e5680(StartupString* out, const char* format, ...);
}

// ---------------------------------------------------------------------------
// Missing providers: declared only, never defined here.
// ---------------------------------------------------------------------------
extern "C" int __fastcall kotor_fastcall_0059ed70(void*, void*, void* object, const void* script);
extern "C" void __fastcall dialog_reply_ctor_0059f5b0(void*, void*);
extern "C" void __fastcall dialog_node_dtor_0059f5e0(void*, void*);
extern "C" int __fastcall kotor_fastcall_0059ec90(void*, void*, void* object, const void* script);
extern "C" int __fastcall kotor_fastcall_0059fb60(void*, void*, U32 id);
extern "C" void* __fastcall kotor_fastcall_0059f140(void*, void*, const void* source);
extern "C" void* __fastcall kotor_fastcall_0059f320(void*, void*, void* out, const void* loc, U8 gender);
extern "C" void* __fastcall kotor_fastcall_0059fc30(void*, void*, void* object, void* out);
extern "C" void* __fastcall kotor_fastcall_005a0360(void*, void*, const void* source);
extern "C" int __fastcall kotor_fastcall_005a1c00(void*, void*, U32, U32, U8, void*, U32, U32, U32, I32);
extern "C" int __fastcall kotor_fastcall_005a3820(void*, void*, void*, U32);
extern "C" int __fastcall kotor_fastcall_005a13d0(void*, void*, U32, U32, U32, U32, U32, U8, void*, void*, U32*);
extern "C" void __fastcall kotor_fastcall_004ef910(void*, void*, void* object);
extern "C" void __fastcall kotor_fastcall_00585460(void*, void*, void*);
extern "C" int __fastcall kotor_fastcall_005ee0d0(void*, void*, U8 resource);
extern "C" int __fastcall kotor_fastcall_0062e100(void*, void*, U32, U32, U32, void*, U32, U8, I32);
extern "C" int __fastcall kotor_fastcall_0062e1d0(void*, void*, U32 id, const void* sound);
extern "C" int __fastcall kotor_fastcall_0062abd0(void*, void*);
extern "C" void __fastcall kotor_fastcall_0062b130(void*, void*);
extern "C" U32 __fastcall kotor_fastcall_004ae810(void*, void*, U32 player_id);
extern "C" float __fastcall kotor_fastcall_005ea270(void*, void*, U32, U32);
extern "C" void __fastcall kotor_fastcall_004cb1a0(void*, void*, U32 delay);
extern "C" float __fastcall kotor_fastcall_0059ff50(void*, void*, void*, CExoLocString* text, U32, U32, U32);
extern "C" void __fastcall kotor_fastcall_0059ef00(void*, void*, void*, U32, U32);
extern "C" void __fastcall kotor_fastcall_0059f3c0(void*, void*, I32, float);

// ---------------------------------------------------------------------------
// Helpers.
// ---------------------------------------------------------------------------
namespace dialog_dep {

inline CExoString* string_of(CExoString* s) { return s; }
inline kotor_menu_10_19::String* kstring(CExoString* s) {
    return reinterpret_cast<kotor_menu_10_19::String*>(s);
}
inline kotor_reconstruction::StartupString* sstring(CExoString* s) {
    return reinterpret_cast<kotor_reconstruction::StartupString*>(s);
}

inline void* load_ptr(const void* base, std::uintptr_t offset) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(base) + offset;
    return *reinterpret_cast<void* const*>(address);
}
inline void* load_server() {
    return load_ptr(kotor_menu_window_cells::manager_007a39fc, 8);
}
inline void* load_client() {
    return load_ptr(kotor_menu_window_cells::manager_007a39fc, 4);
}
inline void* load_base() {
    return kotor_menu_10_19::dependencies::base_007a39e0;
}
inline void* load_alias_list() {
    return load_ptr(load_base(), 0xC);
}

inline void str_default(CExoString* s) { (void)kstring(s)->default_005b3190(); }
inline void str_copy(CExoString* dst, const CExoString* src) {
    (void)kstring(dst)->copy_005e5b00(reinterpret_cast<const kotor_menu_10_19::String*>(src));
}
inline void str_destroy(CExoString* s) { kstring(s)->destroy_005e5c20(); }
inline const char* str_text(CExoString* s) { return kstring(s)->text_005e5670(); }
inline void str_assign(CExoString* dst, CExoString* src) {
    (void)sstring(dst)->Assign005e5c50(reinterpret_cast<kotor_reconstruction::StartupString*>(src));
}
inline U32 str_length(CExoString* s) { return async_string_length_005e5790(s, nullptr); }
inline char str_at(CExoString* s, I32 i) { return c2da_current_string_index_005e5600(s, nullptr, i); }
inline int str_nc_eq(CExoString* a, CExoString* b) { return exostring_compare_no_case_005e6450(a, nullptr, b); }
inline int str_ne(CExoString* a, const char* b) {
    return client_movies::StringNotEqual_005e5390(
        reinterpret_cast<options_video::String*>(a), nullptr, b);
}
inline int str_nc_eq_chars(CExoString* a, const char* b) {
    return ini_comparison::CompareNC005e58c0(sstring(a), nullptr, b);
}

using VFn0 = void* (__fastcall*)(void*, void*);
inline void* vcall0(void* obj, U32 off) {
    void** vt = *reinterpret_cast<void***>(obj);
    return reinterpret_cast<VFn0>(vt[off / 4])(obj, nullptr);
}

}  // namespace dialog_dep
