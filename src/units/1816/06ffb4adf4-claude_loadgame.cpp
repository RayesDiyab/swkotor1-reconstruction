// claude_loadgame.cpp (lane worker loadgame): the server-side saved-game load entry and its missing cascade,
// transcribed from the original swkotor.exe (read-only):
//   004BA640 CServerExoAppInternal::LoadGame(ulong id, CExoString const& name, CExoString const& module, CSWSPlayer*)
//                                                                         RET 10h, EAX = LoadModule result  kotor_fastcall_004ba640
//   00565D20 CSWPartyTable::LoadTableInfo()                               RET 0, EAX 0/1      kotor_fastcall_00565d20
//   00563200 CSWPartyTable::ClearTable()                                  RET 0               kotor_fastcall_00563200
//   00563430 CSWPartyTable::LoadJournal(CResGFF*, CResStruct*)            RET 8               kotor_fastcall_00563430
//   0052ADE0 CSWGlobalVariableTable::Load()                               RET 0, EAX 0/1      kotor_fastcall_0052ade0
//   0052ABE0 CSWGlobalVariableTable::ReadTable(CExoString const&)         RET 4, EAX 0/1      kotor_fastcall_0052abe0
//   0052A280 CSWGlobalVariableTable::ReadTableWithCatalogue(CResGFF*, CResStruct*)    RET 8   kotor_fastcall_0052a280
//   00529EF0 CSWGlobalVariableTable::ReadTableWithoutCatalogue(CResGFF*, CResStruct*) RET 8   kotor_fastcall_00529ef0
// The W2 port EntryPorts::load_game_004ba640(internal, id, name, module, player) (w2_entry_native.hpp) binds to
// kotor_fastcall_004ba640 with the same argument order (module_message_00524800 sub 5).
// Conventions (W5): thiscall -> extern "C" __fastcall(self, unused_edx, stack args...); game heap only.
// Proven .rdata: [0073D71C] = "" ; strings "SAVES:", "%06d - %s", "PARTYTABLE", "PT  ", "GLOBALVARS", "GVT " etc. are
// the literals the original pushes (transcribed text). 004015A0 (void_return, a bare RET) is called where the original
// calls it (load-failure debug hook with 0BC4Eh).
// Global-variable catalogue lookups bind the existing 0855 bodies (recovered_global_catalog::find_00529030 /
// insert_00528ec0) through a Ports implementation identical to 0857's (same hash inputs as when the catalogue was built).
// The global-variable readers keep the original frame layout (one frame buffer per call, same offsets) so the
// per-category scratch arrays, cursors and their original overruns behave as in the original.
// Quirks kept: ReadTableWithCatalogue zeroes the five string slots without destroying them (old text leaks), formats
// the "won't fit" message into a discarded temporary and then still uses the failed (-1) slot; it skips "CatString"
// when "ValString" is missing; ReadTableWithoutCatalogue assigns string variables from the scratch array starting
// after the strings it read (index n, not 0); LoadTableInfo's PT_FB_MSG_COLOR travels in a dword whose upper bytes are
// the PT_AVAIL_NPCS count slot; the PARTYTABLE CResGFF is destroyed through its vtable slot 0 (scalar deleting dtor, 1).
// [00833A90] (journal sort order) has two rebuilt cells (w2_ifo_native recovered_journal_sort_order_00833a90 and
// panels_menus w4::journal_sort_00833a90); LoadJournal reads the W2 cell (as SaveJournal does) and writes both.
#include <cctype>
#include <cstdint>
#include <cstring>
#include <initializer_list>

namespace {
using U = std::uint32_t;
using I = std::int32_t;
using U8 = std::uint8_t;
using U16 = std::uint16_t;
inline void* ptr(U a) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a)); }
inline U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline U rd(U a) { U v; std::memcpy(&v, ptr(a), 4); return v; }
inline U rd8(U a) { return *static_cast<U8*>(ptr(a)); }
inline U rd16(U a) { U16 v; std::memcpy(&v, ptr(a), 2); return v; }
inline void wr(U a, U v) { std::memcpy(ptr(a), &v, 4); }
inline void wr8(U a, U v) { *static_cast<U8*>(ptr(a)) = static_cast<U8>(v); }
inline void wrf(U a, float v) { std::memcpy(ptr(a), &v, 4); }
template <class R, class... A> R vcall(U object, U slot, A... a) {
    using F = R(__thiscall*)(void*, A...);
    return reinterpret_cast<F>(rd(rd(object) + slot))(ptr(object), a...);
}
struct ExoString { U data, length; };
}

namespace kotor_reconstruction { struct StartupString; }
namespace kotor_movie { void* __fastcall string_element_construct_005b3190(void* s, void*); } // 005B3190 CExoString() [0603]
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }                // [007A39FC] CAppManager*
namespace frame_control { extern unsigned resource_manager_007a39e8; }                       // [007A39E8] CExoResMan* [0151]
namespace menu_resources {
struct Ref { unsigned char bytes[16]; Ref* __thiscall construct_00406d80(const char* text); }; // 00406D80 CResRef(char*) [0030]
struct Manager { unsigned __thiscall directory_00408800(kotor_reconstruction::StartupString* name); }; // 00408800 AddResourceDirectory [0027]
}
namespace w4 { extern volatile std::uint32_t journal_sort_00833a90; }                       // [00833A90] (panels_menus)
namespace directory_list { char __fastcall At005e5600(kotor_reconstruction::StartupString*, void*, int); } // 005E5600 operator[] [0288]
namespace ini_comparison { int __fastcall CompareNC005e58c0(kotor_reconstruction::StartupString*, void*, const char*); } // 005E58C0 [0154]
namespace options_video { struct String; }
namespace client_movies { int __fastcall StringNotEqual_005e5390(options_video::String*, void*, const char*); } // 005E5390 operator!=(char*) [0374]
kotor_reconstruction::StartupString* __fastcall GuiConcat005e5d10(kotor_reconstruction::StartupString* self, void*,
    kotor_reconstruction::StartupString* out, const kotor_reconstruction::StartupString* other); // 005E5D10 operator+ [0093]
void* async_ref_construct_string_00406d60(void* ref, void*, void* text);                   // 00406D60 CResRef(CExoString const&) (0886)
namespace recovered_global_catalog {                                                        // 0855 (same definition as global_catalog.hpp)
struct String { std::uint32_t data, length; };
struct Ports { virtual ~Ports() = default; virtual int length(String*) = 0; virtual signed char character(String*, int) = 0;
               virtual int uppercase(int) = 0; virtual const char* text(String*) = 0; virtual int equal_nc(String*, const char*) = 0; };
int insert_00528ec0(void*, String*, unsigned, unsigned, Ports&);                             // 00528EC0 AddIdentifier
int find_00529030(void*, String*, Ports&);                                                   // 00529030 FindIdentifierIndex
}

extern "C" {
extern void* kotor_cell_rules_007a3a28;                                                    // [007A3A28] CSWRules*
extern U recovered_journal_sort_order_00833a90;                                            // [00833A90] [w2_ifo_native]
void* __cdecl live_operator_new(unsigned);                                                 // 006FA7E6
void __cdecl live_free(void*);                                                             // 006FA390
void __fastcall kotor_m15e_slot090_void0_004015a0(void*, void*);                          // 004015A0 void_return (RET) [0832]
void* __fastcall gui_load_gff_construct_00410630(void* gff, void*, U type, const char* file_type, const void* ref); // 00410630 CResGFF [0289]
U __fastcall kotor_fastcall_00411240(void* gff, void*, void* out);                         // 00411240 GetTopLevelStruct [claude_dialogue_start]
U __fastcall gff_list_004118c0(void* gff, void*, void* out, void* parent, const char* label); // 004118C0 GetList [0291]
U __fastcall gff_list_count_00411940(void* gff, void*, void* list);                      // 00411940 GetListCount [0291]
U __fastcall gff_list_element_00411990(void* gff, void*, void* out, void* list, U index); // 00411990 GetListElement [0291]
U __fastcall gui_load_gff_struct_00411a10(void* gff, void*, void* out, void* parent, const char* label); // 00411A10 [0292]
unsigned char __fastcall gui_load_gff_byte_00411a60(void* gff, void*, void* st, const char* label, U* ok, unsigned char def); // 00411A60 [0292]
U __fastcall gui_load_gff_dword_00411c20(void* gff, void*, void* st, const char* label, U* ok, U def); // 00411C20 [0292]
int __fastcall gui_load_gff_int_00411c90(void* gff, void*, void* st, const char* label, U* ok, int def); // 00411C90 [0292]
float __fastcall gui_load_gff_float_00411d00(void* gff, void*, void* st, const char* label, U* ok, float def); // 00411D00 [0292]
void* __fastcall gui_load_gff_string_00411ec0(void* gff, void*, void* out, void* st, const char* label, U* ok, void* def); // 00411EC0 [0292]
void* __fastcall kotor_fastcall_00412380(void* gff, void*, void* st, void* dest, U size, const char* label, U* ok, void* def); // 00412380 ReadFieldVOID [claude_loadgame_queue]
void __fastcall kotor_fastcall_004088d0(void* resman, void*, void* name);                 // 004088D0 RemoveResourceDirectory [0576]
U __fastcall w1x_server_load_module_004b95b0(void* internal, void*, void* name, U player); // 004B95B0 CServerExoAppInternal::LoadModule (0886)
void* __fastcall frame_LogStringCtor_005e5a90(void* s, void*, const char* text);          // 005E5A90 CExoString(char*) [0459]
void __fastcall frame_LogStringDtor_005e5c20(void* s, void*);                             // 005E5C20 ~CExoString [0459]
void* __fastcall async_assign_string_005e5c50(void* s, void*, void* other);               // 005E5C50 CExoString::operator= [0199]
void* __fastcall GuiAssign005e5140(void* s, void*, const char* text);                     // 005E5140 operator=(char*) [0093]
const char* __fastcall frame_LogCStr_005e5670(void* s, void*);                            // 005E5670 CExoString::CStr [0459]
unsigned __fastcall async_string_length_005e5790(void* s, void*);                         // 005E5790 GetLength [0188]
void __cdecl frame_LogFormat_005e5680(void* self, const char* format, ...);               // 005E5680 CExoString::Format [0125]
void __stdcall crt_eh_vector_constructor_006fb55f(void* first, U size, int count, void* construct, void* destroy); // 006FB55F [d1_bodies]
void __stdcall c2da_destroy_array_006fa852(void* first, U size, int count, void* destroy); // 006FA852 [0186]
void* __fastcall kotor_fastcall_004ca7a0(void* location, void*);                          // 004CA7A0 CScriptLocation() [claude_vm_actions_queries]
void __fastcall kotor_m15_client_set_load_bar_progress_005edd40(void* client, void*, U a, U b); // 005EDD40 SetLoadBarProgress [0813]
void __fastcall kotor_fastcall_005edfd0(void* client, void*, U step, U value);           // 005EDFD0 SetLoadStep [0402]
void* __fastcall frame_R4_005ed690(void* client, void*);                                  // 005ED690 GetInGameGui [0457]
void __fastcall feedback_append_0062b5c0(void* gui, void*, void* text, U type, U color);  // 0062B5C0 AppendToMsgBuffer [feedback_lowers]
void __fastcall kotor_fastcall_0062b680(void* gui, void*, void* a, void* b);              // 0062B680 AppendToDialogBuffer [0701]
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* array, void*, U index); // 005B31A0 GetBaseItem [0820]
void __fastcall recovered_unstealth_party_00563c60(void* table, void*, U flag);           // 00563C60 UnstealthParty [w2_party_native]
void* __fastcall recovered_party_journal_005633c0(void* table, void*);                    // 005633C0 GetPartyJournal [w2_ifo_native]
void __fastcall kotor_fastcall_005c5a40(void* journal, void*, void* tag, U state, U flag); // 005C5A40 SetState [claude_dialogue_server]
void __fastcall kotor_fastcall_005c63f0(void* journal, void*, void* tag, U date, U flag);  // 005C63F0 SetDate [claude_dialogue_server]
void __fastcall kotor_fastcall_005c6650(void* journal, void*, void* tag, U time, U flag);  // 005C6650 SetTime [claude_dialogue_server]
void __fastcall kotor_fastcall_00555de0(void* repository, void*);                         // 00555DE0 ~CItemRepository [0718]
void __fastcall kotor_fastcall_005c58f0(void* journal, void*);                            // 005C58F0 ~CSWSJournal [0718]

void __fastcall kotor_fastcall_00563200(void* table, void*);
void __fastcall kotor_fastcall_00563430(void* table, void*, void* gff, void* st);
U __fastcall kotor_fastcall_00565d20(void* table, void*);
void __fastcall kotor_fastcall_00529ef0(void* vars, void*, void* gff, void* st);
void __fastcall kotor_fastcall_0052a280(void* vars, void*, void* gff, void* st);
U __fastcall kotor_fastcall_0052abe0(void* vars, void*, void* name);
U __fastcall kotor_fastcall_0052ade0(void* vars, void*);
U __fastcall kotor_fastcall_004ba640(void* internal, void*, U id, void* name, void* module, U player);
}

namespace {
inline void* client() { return ptr(rd(word(const_cast<void*>(kotor_menu_window_cells::manager_007a39fc)) + 4)); } // [[007A39FC]+4]
inline void* resman() { return ptr(frame_control::resource_manager_007a39e8); }
inline kotor_reconstruction::StartupString* ss(void* p) { return static_cast<kotor_reconstruction::StartupString*>(p); }
inline void* str_ctor(void* s) { return kotor_movie::string_element_construct_005b3190(s, nullptr); }
inline void str_dtor(void* s) { frame_LogStringDtor_005e5c20(s, nullptr); }
// CResGFF scalar deleting destructor (vtable slot 0, flag 1) as the original calls it
inline void gff_delete(U gff) { (void)vcall<void*>(gff, 0, 1u); }
// GFF reader helpers: the original passes the shared status slot every time
inline U r_dword(void* gff, void* st, const char* label, U* ok, U def) { return gui_load_gff_dword_00411c20(gff, nullptr, st, label, ok, def); }
inline U r_int(void* gff, void* st, const char* label, U* ok, int def) { return static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, label, ok, def)); }
inline U r_byte(void* gff, void* st, const char* label, U* ok, U def) { return gui_load_gff_byte_00411a60(gff, nullptr, st, label, ok, static_cast<unsigned char>(def)); }
// "" default + ReadFieldCExoString + operator= into target + both destructors (the original's inline sequence)
void read_string_into(void* gff, void* st, const char* label, U* ok, void* target, void* def_slot, void* out_slot) {
    frame_LogStringCtor_005e5a90(def_slot, nullptr, "");                                 // ([0073D71C])
    void* const text = gui_load_gff_string_00411ec0(gff, nullptr, out_slot, st, label, ok, def_slot);
    async_assign_string_005e5c50(target, nullptr, text);
    str_dtor(out_slot);
    str_dtor(def_slot);
}
}

// ===================================================================================== 00563200
void __fastcall kotor_fastcall_00563200(void* table, void*) {
    const U t = word(table);
    wr(t + 0xfc, 0); wr(t + 0xf4, 0); wr(t + 0xf0, 0xffffffffu); wr(t + 0xf8, 0);         // 0056320A..0056321C
    wr(t + 0x100, 1); wr(t + 0x104, 0x7db3); wr(t + 0x108, 0xa488);                      // 00563222..00563236
    wr(t + 0x114, 0); wr(t + 0x10c, 0);                                                  // 00563240..00563246
    for (U i = 0; i < 0x10; ++i) { wr(t + 0x60 + i * 4, 0); wr(t + 0xa0 + i * 4, 0); }   // 0056324C..00563260
    wr(t + 0xe0, 0xffffffffu); wr(t + 0x190, 0); wr(t + 0x194, 0);                       // 0056326A..00563276
    const U repository = rd(t + 0x118);                                                  // 00563262
    if (repository != 0) {                                                               // 00563268..0056327C
        kotor_fastcall_00555de0(ptr(repository), nullptr);                               // 0056327E..00563280
        live_free(ptr(repository));                                                      // 00563285..0056328B
        wr(t + 0x118, 0);                                                                // 0056328E
    }
    wr(t, 0); wr(t + 0xe4, 0); wr(t + 0xe8, 0); wr(t + 0xec, 0xffffffffu); wr(t + 0x110, 0); // 00563297..005632AB
    for (U i = 0; i < 9; ++i) {                                                          // 005632B1..005632D5
        wr(t + 0x30 + i * 4, 0);
        wr8(t + 0x54 + i, 1);
        wr(t + 0xc + i * 4, 0x7f000000);
    }
    for (U i = 0; i < 5; ++i) wr(t + 0x120 + i * 4, 2);                                  // 005632D7..005632F4
    std::memset(ptr(t + 0x134), 0, 0xd * 4);                                             // 005632FA..00563307
    std::memset(ptr(t + 0x168), 0xff, 0xa * 4);                                          // 00563309..00563316
    const U journal = rd(t + 0x11c);                                                     // 00563318
    if (journal != 0) {                                                                  // 0056331E..00563320
        kotor_fastcall_005c58f0(ptr(journal), nullptr);                                  // 00563322..00563324
        live_free(ptr(journal));                                                         // 00563329..0056332F
        wr(t + 0x11c, 0);                                                                // 00563332
    }
}

// ===================================================================================== 00563430
void __fastcall kotor_fastcall_00563430(void* table, void*, void* gff, void* st) {
    if (recovered_party_journal_005633c0(table, nullptr) == nullptr) return;             // 00563451..0056345A
    alignas(4) unsigned char list[0x14] = {};                                            // [esp+34h]
    if (gff_list_004118c0(gff, nullptr, list, st, "JNL_Entries") == 0) return;           // 00563466..0056347A
    U ok = 0;                                                                            // [esp+0Ch]
    const U sort = static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "JNL_SortOrder", &ok,
                                                            static_cast<int>(recovered_journal_sort_order_00833a90))); // 00563480..00563494
    recovered_journal_sort_order_00833a90 = sort;                                        // 0056349D
    w4::journal_sort_00833a90 = sort;                                                    // (same original cell)
    ExoString plot;                                                                      // [esp+1Ch]
    str_ctor(&plot);                                                                     // 00563499..005634A2
    const I count = static_cast<I>(gff_list_count_00411940(gff, nullptr, list));        // 005634A7..005634BB [esp+18h]
    alignas(4) unsigned char element[4] = {};                                            // [esp+14h]
    for (I i = 0; i < count; ++i) {                                                      // 005634BF, 005635BF..005635C6 (jl)
        gff_list_element_00411990(gff, nullptr, element, list, static_cast<U>(i));       // 005634C5..005634D2
        ExoString def, out;                                                              // [esp+24h] / [esp+2Ch]
        read_string_into(gff, element, "JNL_PlotID", &ok, &plot, &def, &out);            // 005634D7..00563530
        const U state = r_int(gff, element, "JNL_State", &ok, 0);                        // 00563535..00563548
        kotor_fastcall_005c5a40(recovered_party_journal_005633c0(table, nullptr), nullptr, &plot, state, 1); // 0056354D..0056355E
        const U date = r_dword(gff, element, "JNL_Date", &ok, 0);                        // 00563563..00563576
        kotor_fastcall_005c63f0(recovered_party_journal_005633c0(table, nullptr), nullptr, &plot, date, 1); // 0056357B..0056358C
        const U time = r_dword(gff, element, "JNL_Time", &ok, 0);                        // 00563591..005635A4
        kotor_fastcall_005c6650(recovered_party_journal_005633c0(table, nullptr), nullptr, &plot, time, 1); // 005635A9..005635BA
    }
    str_dtor(&plot);                                                                     // 005635CC..005635D8
}

// ===================================================================================== 00565D20
U __fastcall kotor_fastcall_00565d20(void* table, void*) {
    const U t = word(table);
    void* const memory = live_operator_new(0xa0);                                        // 00565D42..00565D49
    U gff = 0;                                                                           // esi
    if (memory != nullptr) {                                                             // 00565D57..00565D64
        menu_resources::Ref ref;                                                         // [esp+54h]
        ref.construct_00406d80("PARTYTABLE");                                            // 00565D66..00565D6F
        gff = word(gui_load_gff_construct_00410630(memory, nullptr, 0, "PT  ", &ref));   // 00565D74..00565D87
    }
    if (gff == 0 || rd(gff + 0x94) == 0) {                                               // 00565D8D..00565DA8
        kotor_m15e_slot090_void0_004015a0(nullptr, nullptr);                             // 00566586..00566590 (push 0BC4Eh; RET)
        if (gff != 0) gff_delete(gff);                                                   // 00566593..0056659D
        return 0;                                                                        // 005665A4
    }
    void* const g = ptr(gff);
    kotor_fastcall_00563200(table, nullptr);                                             // 00565DAE..00565DB0
    alignas(4) unsigned char root[4] = {}, element[4] = {}, list[0x14] = {};             // [esp+18h] / [esp+14h] / [esp+64h]
    U ok = 0;                                                                            // [esp+10h]
    kotor_fastcall_00411240(g, nullptr, root);                                           // 00565DB5..00565DBC
    gff_list_004118c0(g, nullptr, list, root, "PT_MEMBERS");                             // 00565DC1..00565DD2 (result unchecked)
    wr(t + 0xfc, r_dword(g, root, "PT_GOLD", &ok, 0));                                   // 00565DD7..00565DF1
    wr(t + 0xf8, r_int(g, root, "PT_XP_POOL", &ok, 0));                                  // 00565DEF..00565E14
    ok = 1;                                                                              // 00565E26 (status slot preset)
    wr(t + 0x114, r_dword(g, root, "PT_PLAYEDSECONDS", &ok, 0));                         // 00565E0D..00565E33
    if (ok == 0) wr(t + 0x114, r_dword(g, root, "PT_PLAYEDMINUTES", &ok, 0) * 0x3c);     // 00565E39..00565E5C
    wr(t + 0xf0, r_int(g, root, "PT_CONTROLLED_NPC", &ok, -1));                          // 00565E62..00565E7A
    const U solo = r_byte(g, root, "PT_SOLOMODE", &ok, rd(t + 0x190));                   // 00565E80..00565E9D (default = low byte)
    if (solo == 0) recovered_unstealth_party_00563c60(table, nullptr, solo);             // 00565EA0..00565EA7
    wr(t + 0x190, solo);                                                                 // 00565EBF
    wr(t + 0x194, r_byte(g, root, "PT_CHEAT_USED", &ok, 0));                             // 00565EAC..00565ECF
    wr(t, r_byte(g, root, "PT_NUM_MEMBERS", &ok, 0));                                    // 00565ECD..00565EF0 (>= 0 always: jge)
    if (gff_list_count_00411940(g, nullptr, list) < rd(t))                               // 00565EFC..00565F0A (jae)
        wr(t, gff_list_count_00411940(g, nullptr, list));                                // 00565F0C..00565F18
    for (I i = 0; i < static_cast<I>(rd(t)); ++i) {                                      // 00565F1A..00565F82 (jl, re-read)
        gff_list_element_00411990(g, nullptr, element, list, static_cast<U>(i));         // 00565F25..00565F32
        const U slot = t + 4 + static_cast<U>(i) * 4;                                    // ebp
        wr(slot, r_int(g, element, "PT_MEMBER_ID", &ok, -1));                            // 00565F37..00565F56
        if (r_byte(g, element, "PT_IS_LEADER", &ok, 0) != 0) wr(t + 0xec, rd(slot));     // 00565F4F..00565F74
    }
    gff_list_004118c0(g, nullptr, list, root, "PT_AVAIL_NPCS");                          // 00565F84..00565F95
    U avail = gff_list_count_00411940(g, nullptr, list) < 9 ? gff_list_count_00411940(g, nullptr, list) : 9; // 00565F9A..00565FBD [esp+1Ch]
    for (I i = 0; i < static_cast<I>(avail); ++i) {                                      // 00565FC5..00566028 (jl)
        gff_list_element_00411990(g, nullptr, element, list, static_cast<U>(i));         // 00565FD2..00565FDF
        wr(t + 0x30 + static_cast<U>(i) * 4, r_byte(g, element, "PT_NPC_AVAIL", &ok, 0)); // 00565FE4..0056600F
        wr8(t + 0x54 + static_cast<U>(i), r_byte(g, element, "PT_NPC_SELECT", &ok, 0));  // 00565FFC..0056601A
    }
    wr(t + 0xe4, r_int(g, root, "PT_AISTATE", &ok, 0));                                  // 0056602A..00566044
    wr(t + 0xe8, r_int(g, root, "PT_FOLLOWSTATE", &ok, 0));                              // 00566042..00566069
    if (gui_load_gff_struct_00411a10(g, nullptr, element, root, "GlxyMap") != 0) {       // 00566060..0056607E
        const U points = r_dword(g, element, "GlxyMapNumPnts", &ok, 0);                  // 00566084..0056609E (edi)
        const U mask = r_dword(g, element, "GlxyMapPlntMsk", &ok, 0);                    // 0056609C..005660BD (ebp)
        wr(t + 0xe0, r_int(g, element, "GlxyMapSelPnt", &ok, -1));                       // 005660B6..005660D3
        if (points == 0x10) {                                                            // 005660D0..005660D9
            for (U i = 0; i < 0x10; ++i) {                                               // 005660DB..00566112
                if ((mask & (1u << i)) != 0) wr(t + 0x60 + i * 4, 1);                    // 005660E3..005660F0
                if ((mask & (1u << (i + 0x10))) != 0) wr(t + 0xa0 + i * 4, 1);           // 005660F7..00566105
            }
        }
    }
    gff_list_004118c0(g, nullptr, list, root, "PT_PAZAAKCARDS");                         // 00566114..00566125 (unchecked)
    for (U i = 0; i < 0x12; ++i) {                                                       // 0056612A..00566166
        gff_list_element_00411990(g, nullptr, element, list, i);
        wr(t + 0x120 + i * 4, r_int(g, element, "PT_PAZAAKCOUNT", &ok, 0));
    }
    gff_list_004118c0(g, nullptr, list, root, "PT_PAZSIDELIST");                         // 00566168..00566179 (unchecked)
    for (U i = 0; i < 0xa; ++i) {                                                        // 0056617E..005661BA
        gff_list_element_00411990(g, nullptr, element, list, i);
        wr(t + 0x168 + i * 4, r_int(g, element, "PT_PAZSIDECARD", &ok, 0));
    }
    alignas(4) unsigned char tutorial_default[6] = {};                                   // [esp+24h] dword + word zero
    const U gui = word(frame_R4_005ed690(client(), nullptr));                            // 005661BC..005661D0
    kotor_fastcall_00412380(g, nullptr, root, ptr(gui + 0xba8), 6, "PT_TUT_WND_SHOWN", &ok, tutorial_default); // 005661D5..005661F3
    void* const app_client = client();                                                   // 00566209..0056620F (edi)
    const U panel = r_int(g, root, "PT_LAST_GUI_PNL", &ok,
                          static_cast<int>(rd(word(frame_R4_005ed690(client(), nullptr)) + 0x2c))); // 005661F8..00566224
    wr(word(frame_R4_005ed690(app_client, nullptr)) + 0x2c, panel);                      // 00566229..00566236
    ExoString message;                                                                   // [esp+2Ch]
    str_ctor(&message);                                                                  // 00566232..00566239
    gff_list_004118c0(g, nullptr, list, root, "PT_FB_MSG_LIST");                         // 0056623E..0056625A
    for (I i = 0; i < static_cast<I>(gff_list_count_00411940(g, nullptr, list)); ++i) { // 0056625F..00566358 (jle / jl, re-read)
        gff_list_element_00411990(g, nullptr, element, list, static_cast<U>(i));         // 00566275..00566282
        ExoString def, out;                                                              // [esp+3Ch] / [esp+44h]
        read_string_into(g, element, "PT_FB_MSG_MSG", &ok, &message, &def, &out);        // 00566287..005662EC
        const U type = r_dword(g, element, "PT_FB_MSG_TYPE", &ok, 0x10000000);           // 005662F1..0056631F (ebp)
        const U color = r_byte(g, element, "PT_FB_MSG_COLOR", &ok, 0);                   // 0056630C..00566326
        avail = (avail & 0xffffff00u) | color;                                           // 00566326 mov [esp+1Ch],al
        feedback_append_0062b5c0(frame_R4_005ed690(client(), nullptr), nullptr, &message, type, avail); // 0056632A..00566344
    }
    ExoString speaker;                                                                   // [esp+34h]
    str_ctor(&speaker);                                                                  // 0056635E..00566362
    gff_list_004118c0(g, nullptr, list, root, "PT_DLG_MSG_LIST");                        // 00566367..00566380
    for (I i = 0; i < static_cast<I>(gff_list_count_00411940(g, nullptr, list)); ++i) { // 00566385..005664B3
        gff_list_element_00411990(g, nullptr, element, list, static_cast<U>(i));         // 005663A0..005663AD
        {
            ExoString def, out;                                                          // [esp+3Ch] / [esp+44h]
            read_string_into(g, element, "PT_DLG_MSG_SPKR", &ok, &speaker, &def, &out);  // 005663B2..00566417
        }
        {
            ExoString def, out;                                                          // [esp+1Ch] / [esp+4Ch]
            read_string_into(g, element, "PT_DLG_MSG_MSG", &ok, &message, &def, &out);   // 0056641C..00566481
        }
        kotor_fastcall_0062b680(frame_R4_005ed690(client(), nullptr), nullptr, &message, &speaker); // 00566486..0056649F
    }
    gff_list_004118c0(g, nullptr, list, root, "PT_COST_MULT_LIST");                      // 005664B9..005664CA (unchecked)
    auto items = [] { return ptr(rd(word(kotor_cell_rules_007a3a28) + 0x34)); };        // [[007A3A28]+34h] CSWBaseItemArray
    for (I i = 0; i < static_cast<I>(rd(word(items()))); ++i) {                          // 005664CF..00566532 (jl, re-read)
        gff_list_element_00411990(g, nullptr, element, list, static_cast<U>(i));         // 005664E0..005664ED
        const U base = word(kotor_m15b_base_item_array_get_base_item_005b31a0(items(), nullptr, static_cast<U>(i))); // 005664F2..00566506
        wrf(base + 0xc4, gui_load_gff_float_00411d00(g, nullptr, element, "PT_COST_MULT_VALUE", &ok, 1.0f)); // 00566501..0056651E
    }
    kotor_fastcall_00563430(table, nullptr, g, root);                                    // 00566534..0056653C
    gff_delete(gff);                                                                     // 00566541..00566547
    str_dtor(&speaker);                                                                  // 00566549..00566555
    str_dtor(&message);                                                                  // 0056655A..00566569
    return 1;                                                                            // 0056656E
}

// ===================================================================================== 0052A280 / 00529EF0
namespace {
struct CatalogPorts final : recovered_global_catalog::Ports {                              // as 0857 global_catalog_native
    int length(recovered_global_catalog::String* s) override { return static_cast<int>(async_string_length_005e5790(s, nullptr)); }
    signed char character(recovered_global_catalog::String* s, int i) override {
        return static_cast<signed char>(directory_list::At005e5600(ss(s), nullptr, i)); }
    int uppercase(int c) override { return std::toupper(c); }
    const char* text(recovered_global_catalog::String* s) override { return frame_LogCStr_005e5670(s, nullptr); }
    int equal_nc(recovered_global_catalog::String* s, const char* t) override { return ini_comparison::CompareNC005e58c0(ss(s), nullptr, t); }
};
// Frame buffer standing in for the original 0C60h/0C48h-byte frames (offsets = original ESP after the prologue).
struct VarFrame { alignas(8) unsigned char b[0xc70]; U at(U o) { return word(b) + o; } };
void construct_scratch(VarFrame& f, U locations, U strings) {
    crt_eh_vector_constructor_006fb55f(ptr(f.at(locations)), 0x18, 0x64, reinterpret_cast<void*>(&kotor_fastcall_004ca7a0),
                                       reinterpret_cast<void*>(&kotor_m15e_slot090_void0_004015a0));
    crt_eh_vector_constructor_006fb55f(ptr(f.at(strings)), 8, 5, reinterpret_cast<void*>(&kotor_movie::string_element_construct_005b3190),
                                       reinterpret_cast<void*>(&frame_LogStringDtor_005e5c20));
}
void destroy_scratch(VarFrame& f, U locations, U strings) {
    c2da_destroy_array_006fa852(ptr(f.at(strings)), 8, 5, reinterpret_cast<void*>(&frame_LogStringDtor_005e5c20));
    c2da_destroy_array_006fa852(ptr(f.at(locations)), 0x18, 0x64, reinterpret_cast<void*>(&kotor_m15e_slot090_void0_004015a0));
}
inline U entry_index(U vars, I slot) { return rd16(vars + static_cast<U>(slot) * 0x18 + 0x16) & 0x3fff; } // word [ebx+slot*18h+16h]
// bit k (MSB first) of a byte array, with the original's signed /8 and %8
inline U get_bit(U base, I k) { const I q = k / 8; const I r = k - q * 8; return (rd8(base + static_cast<U>(q)) >> (7 - r)) & 1u; }
inline void put_bit(U base, I k, U value) {
    const I q = k / 8; const I r = k - q * 8; const U8 m = static_cast<U8>(1u << (7 - r));
    const U a = base + static_cast<U>(q);
    wr8(a, value != 0 ? (rd8(a) | m) : (rd8(a) & static_cast<U8>(~m)));
}
// FindIdentifierIndex, else AddIdentifier into the category while below its limit; on failure the original formats a
// discarded message and keeps -1
I find_or_add(U vars, recovered_global_catalog::String* name, U type, U count_offset, I limit, const char* message, CatalogPorts& p) {
    I slot = recovered_global_catalog::find_00529030(ptr(vars), name, p);
    if (slot >= 0) return slot;
    slot = -1;
    const I count = static_cast<I>(rd(vars + count_offset));
    if (count < limit) {
        slot = recovered_global_catalog::insert_00528ec0(ptr(vars), name, type, static_cast<U>(count), p);
        wr(vars + count_offset, rd(vars + count_offset) + 1);
    }
    if (slot < 0) {
        ExoString text;
        str_ctor(&text);
        frame_LogFormat_005e5680(&text, message, frame_LogCStr_005e5670(name, nullptr));
        str_dtor(&text);
    }
    return slot;
}
}

void __fastcall kotor_fastcall_0052a280(void* vars, void*, void* gff, void* st) {
    const U v = word(vars);
    CatalogPorts p;
    VarFrame f;
    std::memset(f.b, 0, sizeof f.b);
    construct_scratch(f, 0x308, 0x70);                                                   // 0052A2A4..0052A2E2
    auto* const name = reinterpret_cast<recovered_global_catalog::String*>(f.b + 0x18);  // [esp+18h]
    str_ctor(name);                                                                      // 0052A2E7..0052A2F3
    std::memset(ptr(v + 0xa668), 0, 0x71);                                               // 0052A2F8..0052A307
    std::memset(ptr(v + 0xa6d9), 0, 0x1f4);                                              // 0052A308..0052A318
    std::memset(ptr(v + 0xa8d0), 0, 0x960);                                              // 0052A31A..0052A325
    std::memset(ptr(v + 0xb230), 0, 0x28);                                               // 0052A327..0052A332 (strings zeroed raw)
    std::memset(ptr(f.at(0x98)), 0, 0x71);                                               // 0052A334..0052A342
    std::memset(ptr(f.at(0x110)), 0, 0x1f4);                                             // 0052A343..0052A351
    U* const ok = reinterpret_cast<U*>(f.b + 0x14);
    void* const deflt = f.b + 0x13;                                                      // [esp+13h] = 0
    void* const list = f.b + 0x3c;
    void* const element = f.b + 0x28;
    auto read_name = [&](U def_at, U out_at) {
        read_string_into(gff, element, "Name", ok, name, ptr(f.at(def_at)), ptr(f.at(out_at)));
        return *ok != 0 && client_movies::StringNotEqual_005e5390(reinterpret_cast<options_video::String*>(name), nullptr, "") != 0;
    };
    // ---- booleans
    kotor_fastcall_00412380(gff, nullptr, st, ptr(f.at(0x98)), 0x71, "ValBoolean", ok, deflt); // 0052A353..0052A37F
    if (*ok != 0) {                                                                      // 0052A384..0052A38A
        gff_list_004118c0(gff, nullptr, list, st, "CatBoolean");                         // 0052A390..0052A39D
        wr(f.at(0xc), 0);
        for (I i = 0; i < static_cast<I>(gff_list_count_00411940(gff, nullptr, list)); i = static_cast<I>(rd(f.at(0xc)))) { // 0052A3A2..0052A3B6, 0052A557..0052A56A
            gff_list_element_00411990(gff, nullptr, element, list, static_cast<U>(i));   // 0052A3C0..0052A3CD
            if (read_name(0x20, 0x2c)) {                                                 // 0052A3D2..0052A458
                const I slot = find_or_add(v, name, 0, 0xb258, 0x384, "ReadTableWithCat(): BOOLEAN '%s' won't fit!", p); // 0052A45E..0052A4D5
                put_bit(v + 0xa668, static_cast<I>(entry_index(v, slot)), get_bit(f.at(0x98), i)); // 0052A4DA..0052A54D
            }
            wr(f.at(0xc), static_cast<U>(i + 1));                                        // 0052A55C..0052A55F
        }
    }
    // ---- numbers
    kotor_fastcall_00412380(gff, nullptr, st, ptr(f.at(0x110)), 0x1f4, "ValNumber", ok, deflt); // 0052A570..0052A592
    if (*ok != 0) {                                                                      // 0052A597..0052A59D
        gff_list_004118c0(gff, nullptr, list, st, "CatNumber");                          // 0052A5A3..0052A5B0
        wr(f.at(0xc), 0);
        for (I i = 0; i < static_cast<I>(gff_list_count_00411940(gff, nullptr, list)); i = static_cast<I>(rd(f.at(0xc)))) { // 0052A5B5..0052A5C9, 0052A70E..0052A721
            gff_list_element_00411990(gff, nullptr, element, list, static_cast<U>(i));   // 0052A5D0..0052A5DD
            if (read_name(0x34, 0x2c)) {                                                 // 0052A5E2..0052A668
                const I slot = find_or_add(v, name, 1, 0xb25c, 0x1f4, "ReadTableWithCat(): NUMBER '%s' won't fit!", p); // 0052A66E..0052A6E5
                wr8(v + 0xa6d9 + entry_index(v, slot), rd8(f.at(0x110) + static_cast<U>(i))); // 0052A6EA..0052A707
            }
            wr(f.at(0xc), static_cast<U>(i + 1));                                        // 0052A712..0052A716
        }
    }
    // ---- locations
    kotor_fastcall_00412380(gff, nullptr, st, ptr(f.at(0x308)), 0x960, "ValLocation", ok, deflt); // 0052A727..0052A749
    if (*ok != 0) {                                                                      // 0052A74E..0052A754
        gff_list_004118c0(gff, nullptr, list, st, "CatLocation");                        // 0052A75A..0052A767
        wr(f.at(0xc), 0);
        wr(f.at(0x20), f.at(0x308));                                                     // 0052A786..0052A78D cursor
        for (I i = 0; i < static_cast<I>(gff_list_count_00411940(gff, nullptr, list)); i = static_cast<I>(rd(f.at(0xc)))) { // 0052A76C..0052A780, 0052A8D1..0052A8EF
            gff_list_element_00411990(gff, nullptr, element, list, static_cast<U>(i));   // 0052A791..0052A79E
            if (read_name(0x34, 0x50)) {                                                 // 0052A7A3..0052A829
                const I slot = find_or_add(v, name, 2, 0xb260, 0x64, "ReadTableWithCat(): LOCATION '%s' won't fit!", p); // 0052A82F..0052A8A3
                std::memcpy(ptr(v + 0xa8d0 + entry_index(v, slot) * 0x18), ptr(rd(f.at(0x20))), 0x18); // 0052A8A8..0052A8C8
            }
            wr(f.at(0x20), rd(f.at(0x20)) + 0x18);                                       // 0052A8D1..0052A8DC
            wr(f.at(0xc), static_cast<U>(i + 1));                                        // 0052A8E0..0052A8E4
        }
    }
    // ---- strings
    void* const string_list = f.b + 0x5c;
    if (gff_list_004118c0(gff, nullptr, string_list, st, "ValString") != 0) {            // 0052A8F5..0052A90C
        const U n = gff_list_count_00411940(gff, nullptr, string_list) > 5 ? 5 : gff_list_count_00411940(gff, nullptr, string_list); // 0052A912..0052A939
        wr(f.at(0xc), n);
        for (I i = 0; i < static_cast<I>(rd(f.at(0xc))); ++i) {                          // 0052A93D..0052A9E1 (jl)
            wr(f.at(0x20), f.at(0x70) + static_cast<U>(i) * 8);                          // 0052A94B..0052A94F / 0052A9CF..0052A9DD
            gff_list_element_00411990(gff, nullptr, f.b + 0x58, string_list, static_cast<U>(i)); // 0052A953..0052A960
            read_string_into(gff, f.b + 0x58, "String", ok, ptr(rd(f.at(0x20))), ptr(f.at(0x2c)), ptr(f.at(0x50))); // 0052A965..0052A9CA
        }
        gff_list_004118c0(gff, nullptr, list, st, "CatString");                          // 0052A9E7..0052A9F7
        wr(f.at(0xc), 0);
        wr(f.at(0x20), f.at(0x70));                                                      // 0052AA16..0052AA1A cursor
        for (I i = 0; i < static_cast<I>(gff_list_count_00411940(gff, nullptr, list)); i = static_cast<I>(rd(f.at(0xc)))) { // 0052A9FC..0052AA10, 0052AB5C..0052AB74
            gff_list_element_00411990(gff, nullptr, element, list, static_cast<U>(i));   // 0052AA20..0052AA2D
            if (read_name(0x2c, 0x50)) {                                                 // 0052AA32..0052AAB8
                const I slot = find_or_add(v, name, 3, 0xb264, 5, "ReadTableWithCat(): STRING '%s' won't fit!", p); // 0052AABE..0052AB32
                async_assign_string_005e5c50(ptr(v + 0xb230 + entry_index(v, slot) * 8), nullptr, ptr(rd(f.at(0x20)))); // 0052AB37..0052AB53
            }
            wr(f.at(0x20), rd(f.at(0x20)) + 8);                                          // 0052AB5C
            wr(f.at(0xc), static_cast<U>(i + 1));                                        // 0052AB65..0052AB69
        }
    }
    str_dtor(name);                                                                      // 0052AB7A..0052AB86
    destroy_scratch(f, 0x308, 0x70);                                                     // 0052AB8B..0052ABC2
}

void __fastcall kotor_fastcall_00529ef0(void* vars, void*, void* gff, void* st) {
    const U v = word(vars);
    VarFrame f;
    std::memset(f.b, 0, sizeof f.b);
    construct_scratch(f, 0x2f0, 0x58);                                                   // 00529F14..00529F4F
    std::memset(ptr(f.at(0x80)), 0, 0x71);                                               // 00529F57..00529F67
    std::memset(ptr(f.at(0xf8)), 0, 0x1f4);                                              // 00529F68..00529F76
    U* const ok = reinterpret_cast<U*>(f.b + 0x1c);
    void* const deflt = f.b + 0x13;                                                      // [esp+13h] = 0
    kotor_fastcall_00412380(gff, nullptr, st, ptr(f.at(0x80)), 0x71, "ValBoolean", ok, deflt);   // 00529F78..00529FA3
    kotor_fastcall_00412380(gff, nullptr, st, ptr(f.at(0xf8)), 0x1f4, "ValNumber", ok, deflt);   // 00529FA8..00529FC7
    kotor_fastcall_00412380(gff, nullptr, st, ptr(f.at(0x2f0)), 0x960, "ValLocation", ok, deflt); // 00529FCC..00529FEB
    void* const list = f.b + 0x44;
    I read = 0;                                                                          // ebx
    if (gff_list_004118c0(gff, nullptr, list, st, "ValString") != 0) {                   // 00529FF0..0052A004
        read = static_cast<I>(gff_list_count_00411940(gff, nullptr, list) > 5 ? 5 : gff_list_count_00411940(gff, nullptr, list)); // 0052A00A..0052A02E
        for (I i = 0; i < read; ++i) {                                                   // 0052A030..0052A0CC (jl)
            gff_list_element_00411990(gff, nullptr, f.b + 0x30, list, static_cast<U>(i)); // 0052A042..0052A04F
            read_string_into(gff, f.b + 0x30, "String", ok, ptr(f.at(0x58) + static_cast<U>(i) * 8),
                             ptr(f.at(0x34)), ptr(f.at(0x3c)));                          // 0052A054..0052A0B9
        }
    }
    U numbers = f.at(0xf8);                                                              // [esp+28h]
    U strings = f.at(0x58) + static_cast<U>(read) * 8;                                   // [esp+20h] (starts after the read ones)
    U locations = f.at(0x2f0);                                                           // [esp+24h]
    I booleans = 0;                                                                      // [esp+14h]
    for (U k = 0; k < 0x6ef; ++k) {                                                      // 0052A0D2..0052A213 (1775 entries)
        const U entry = v + k * 0x18;
        if (rd8(entry) == 0) continue;                                                   // 0052A110..0052A115
        const U code = rd16(entry + 0x16);                                               // 0052A11B
        const U t = code & 0x3fff;
        switch (code >> 14) {                                                            // 0052A11E..0052A12C (table 0052A26C)
        case 0:                                                                          // 0052A133
            put_bit(v + 0xa668, static_cast<I>(t), get_bit(f.at(0x80), booleans));
            ++booleans;                                                                  // 0052A19D
            break;
        case 1:                                                                          // 0052A1A3
            wr8(v + 0xa6d9 + t, rd8(numbers)); ++numbers;
            break;
        case 2:                                                                          // 0052A1BD
            std::memcpy(ptr(v + 0xa8d0 + t * 0x18), ptr(locations), 0x18); locations += 0x18;
            break;
        default:                                                                         // 3: 0052A1E3
            async_assign_string_005e5c50(ptr(v + 0xb230 + t * 8), nullptr, ptr(strings)); strings += 8;
            break;
        }
    }
    destroy_scratch(f, 0x2f0, 0x58);                                                     // 0052A219..0052A250
}

// ===================================================================================== 0052ABE0 / 0052ADE0
U __fastcall kotor_fastcall_0052abe0(void* vars, void*, void* name) {
    void* const memory = live_operator_new(0xa0);                                        // 0052ABFA..0052AC01
    U gff = 0;
    if (memory != nullptr) {                                                             // 0052AC0B..0052AC19
        menu_resources::Ref ref;                                                         // [esp+10h]
        async_ref_construct_string_00406d60(&ref, nullptr, name);                        // 0052AC1B..0052AC24
        gff = word(gui_load_gff_construct_00410630(memory, nullptr, 0, "GVT ", &ref));   // 0052AC29..0052AC37
    }
    if (gff == 0 || rd(gff + 0x94) == 0) {                                               // 0052AC42..0052AC5A
        kotor_m15e_slot090_void0_004015a0(nullptr, nullptr);                             // 0052ACDA..0052ACE4 (push 0BC4Eh; RET)
        if (gff != 0) gff_delete(gff);                                                   // 0052ACE7..0052ACF1
        return 0;
    }
    alignas(4) unsigned char root[4] = {}, list[0x14] = {};                              // [esp+8h] / [esp+20h]
    kotor_fastcall_00411240(ptr(gff), nullptr, root);                                    // 0052AC5C..0052AC63
    if (gff_list_004118c0(ptr(gff), nullptr, list, root, "CatBoolean") != 0)             // 0052AC68..0052AC82
        kotor_fastcall_0052a280(vars, nullptr, ptr(gff), root);                          // 0052AC84..0052AC8A
    else
        kotor_fastcall_00529ef0(vars, nullptr, ptr(gff), root);                          // 0052ACAF..0052ACB5
    gff_delete(gff);                                                                     // 0052AC8F..0052AC95 / 0052ACBA..0052ACC0
    return 1;
}
U __fastcall kotor_fastcall_0052ade0(void* vars, void*) {
    ExoString name;                                                                      // [esp+4h]
    frame_LogStringCtor_005e5a90(&name, nullptr, "GLOBALVARS");                          // 0052ADFB..0052AE04
    const U result = kotor_fastcall_0052abe0(vars, nullptr, &name);                      // 0052AE09..0052AE18
    str_dtor(&name);                                                                     // 0052AE1D..0052AE2B
    return result;
}

// ===================================================================================== 004BA640
U __fastcall kotor_fastcall_004ba640(void* internal, void*, U id, void* name, void* module, U player) {
    const U in = word(internal);
    ExoString directory, game, unused_a, unused_b;                                       // [esp+10h] [esp+18h] [esp+30h] [esp+28h]
    str_ctor(&directory); str_ctor(&game); str_ctor(&unused_a); str_ctor(&unused_b);     // 004BA663..004BA692
    kotor_m15_client_set_load_bar_progress_005edd40(client(), nullptr, 0xa, 1);          // 004BA697..004BA6A8
    kotor_fastcall_005edfd0(client(), nullptr, 0, 0xa);                                  // 004BA6AD..004BA6B9
    kotor_fastcall_005edfd0(client(), nullptr, 1, 0x14);                                 // 004BA6BE..004BA6CB
    kotor_fastcall_005edfd0(client(), nullptr, 2, 0x17);                                 // 004BA6D0..004BA6DC
    kotor_fastcall_005edfd0(client(), nullptr, 3, 0x17);                                 // 004BA6E1..004BA6EE
    kotor_fastcall_005edfd0(client(), nullptr, 4, 0x17);                                 // 004BA6F3..004BA700
    if (id != 0xffffffffu)                                                               // 004BA705..004BA70E
        frame_LogFormat_005e5680(&game, "%06d - %s", id, frame_LogCStr_005e5670(name, nullptr)); // 004BA710..004BA726
    else
        async_assign_string_005e5c50(&game, nullptr, name);                              // 004BA72B..004BA730
    GuiAssign005e5140(&directory, nullptr, "SAVES:");                                    // 004BA735..004BA73E
    {
        ExoString joined;                                                                // [esp+20h]
        GuiConcat005e5d10(ss(&directory), nullptr, ss(&joined), ss(&game));              // 004BA743..004BA751
        async_assign_string_005e5c50(&directory, nullptr, &joined);                      // 004BA756..004BA760
        str_dtor(&joined);                                                               // 004BA765..004BA76E
    }
    static_cast<menu_resources::Manager*>(resman())->directory_00408800(ss(&directory)); // 004BA773..004BA77E
    kotor_fastcall_00565d20(ptr(in + 0x1b770), nullptr);                                 // 004BA783..004BA789 party table
    kotor_fastcall_0052ade0(ptr(in + 0x100fc), nullptr);                                 // 004BA78E..004BA794 global variables
    kotor_fastcall_004088d0(resman(), nullptr, &directory);                              // 004BA799..004BA7A4
    wr(in + 0x1b87c, 1);                                                                 // 004BA7B3 load-from-save
    wr(in + 0x1b930, 0);                                                                 // 004BA7BD
    wr(in + 0x1b938, 0);                                                                 // 004BA7C3
    const U result = w1x_server_load_module_004b95b0(internal, nullptr, module, player);  // 004BA7A9..004BA7C9
    str_dtor(&unused_b); str_dtor(&unused_a); str_dtor(&game); str_dtor(&directory);     // 004BA7CE..004BA805
    return result;                                                                       // 004BA80F
}
