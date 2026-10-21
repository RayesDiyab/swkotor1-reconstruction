// claude_w1_chains_autosave.cpp (lane worker w1-chains): CServerExoAppInternal::DoPCAutosave 004B8300 behind the W1
// port router's former trap case, with its missing cascade, transcribed from the original swkotor.exe (read-only):
//   004B8300 CServerExoAppInternal::DoPCAutosave(CExoString const& module, C2DA* modules) RET 8  kotor_fastcall_004b8300
//   004B30D0 UpdateLoadScreen(int index, int count, int base, int span) (cdecl ImportFiles callback) kotor_cdecl_004b30d0
//   005648C0 CSWPartyTable::SaveTableInfo(CExoString const& dir)          RET 4   kotor_fastcall_005648c0
//   00563BF0 CSWPartyTable::GetTotalPlayedSeconds()                       RET 0   kotor_fastcall_00563bf0
//   00563D90 CSWPartyTable::SaveJournal(CResGFF*, CResStruct*)            RET 8   kotor_fastcall_00563d90
//   0062A9E0 / 0062A9F0 CGuiInGame::GetMessageBuffer / GetMessageBufferSize RET 0  kotor_fastcall_0062a9e0/_0062a9f0
//   00406C60 CExoResMan::CreateSignatureFile(4 args) -> 1                  RET 10h kotor_fastcall_00406c60
//   0052AD10 CSWGlobalVariableTable::Save(CExoString const& dir)          RET 4   kotor_fastcall_0052ad10
//   005299B0 CSWGlobalVariableTable::WriteTable(CExoString const& file, int lists) RET 8 kotor_fastcall_005299b0
//   005EE050 / 005EE070 CClientExoApp::GetGamePlayHint / GetStoryHint     RET 0   kotor_fastcall_005ee050/_005ee070
//   004B2840 / 004B1400 / 004B2230 KOTOR_AUTOSAVE_PARAMS ctor / Reset / dtor RET 0
//   004B28E0 KOTOR_AUTOSAVE_PARAMS::SaveToGFF(CResGFF*, CResStruct*)      RET 8   kotor_fastcall_004b28e0
//   004B26C0 CStatusSummary::SaveToGFF(CResGFF*, CResStruct*)             RET 8   kotor_fastcall_004b26c0
//   005EDD00 CClientExoApp::GetLoadMusicByModuleName(CExoString* out, CExoString const&) RET 8 kotor_fastcall_005edd00
//   005F3650 CClientExoAppInternal::GetLoadMusicByModuleName(out, module) RET 8 kotor_fastcall_005f3650
//   004AED40 CServerExoApp::GetMoveToModuleStartWaypoint(CExoString* out) RET 4   kotor_fastcall_004aed40
//   0062F0A0 CGuiInGame::GetStatusSummary(CStatusSummary* out)            RET 4   kotor_fastcall_0062f0a0
// The CERFFile / CExoFile / FileCopy leaves are in claude_w1_chains_erf.cpp.
// Conventions (W5): thiscall -> extern "C" __fastcall(self, unused_edx, ...); x87 at 53-bit precision -> double;
// live_operator_new / live_free only. Temporary CExoStrings are destroyed in the original order; scope guards stand
// in for the original EH states while unwinding.
#include "842d6b1618-w5_t3.hpp"

namespace w2 { std::uint32_t ftol2_006fae8c(double value); }                                  // __ftol2 low dword [w2_ftol_native]
namespace kotor_reconstruction { struct StartupString; }
kotor_reconstruction::StartupString* __fastcall GuiConcat005e5d10(kotor_reconstruction::StartupString* self, void*,
    kotor_reconstruction::StartupString* out, const kotor_reconstruction::StartupString* other); // CExoString::operator+ [0093]
namespace kotor_reconstruction { struct StartupString { int BorrowedEquals005e5290(const char*); }; } // operator==(char*) [0077]
namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); } // CExoString() [menu20_bindings]
namespace kotor_menu_10_19 { struct String; struct Tlk { String* simple_0041e8f0(String* out, unsigned id); }; } // CTlkTable::GetSimpleString [0037]
namespace gui_ctor { struct Gui { void Draw_0040cc50(float dt); }; }                          // CSWGuiManager::Draw [0409]
#ifndef W1R_MENU_RESOURCES_REF
#define W1R_MENU_RESOURCES_REF
namespace menu_resources { struct Ref { unsigned char bytes[16]; Ref* __thiscall construct_00406d80(const char* text); }; } // CResRef(char*) [0030]
#endif
namespace menu_resources { struct Manager { unsigned exists_00408bc0(Ref* ref, unsigned type, unsigned* table); }; } // CExoResMan::Exists [0031]
namespace kotor_resource_demand { extern menu_resources::Manager* volatile resource_manager_007a39e8; } // [007A39E8] CExoResMan*
namespace options_video { struct String; }
namespace client_movies { int __fastcall StringFind_005e5eb0(options_video::String*, void*, options_video::String*, int); } // CExoString::Find [0374]
namespace base_ini_reconstruction {
kotor_reconstruction::StartupString* __fastcall left_005e5f70(kotor_reconstruction::StartupString*, void*,
                                                               kotor_reconstruction::StartupString*, int); // CExoString::Left [0126]
}
namespace kotor_menu_window_cells { extern void* volatile input_007a39e4; }                   // [007A39E4] CExoInput* [0053]
void* async_ref_construct_string_00406d60(void* ref, void*, void* string);                    // CResRef(CExoString const&) [0278 cdecl view]

namespace w1c_autosave {
using namespace w5;

extern "C" {
// ---------------- imports: existing providers (c14 candidate map; owner object in brackets) ----------------
extern void* kotor_cell_tlk_007a3a08;                                                          // [007A3A08] CTlkTable* [0384]
extern volatile U kotor_cell_party_session_start_lo_007a3a20;                                 // [007A3A20] [0811]
extern volatile U kotor_cell_party_session_start_hi_007a3a24;                                 // [007A3A24] [0811]
extern U recovered_journal_sort_order_00833a90;                                                // [00833A90] [w2_ifo_native]
__declspec(dllimport) void __stdcall GetSystemTimeAsFileTime(void* file_time);                // [0073D194]
void* __fastcall WindowCopy005e5b00(void* self, void*, void* other);                           // CExoString(CExoString const&) [0078]
void* __fastcall async_assign_string_005e5c50(void* self, void*, void* other);                 // CExoString::operator= [0199]
void* __fastcall frame_LogAssign_005e5140(void* self, void*, const char* text);                // CExoString::operator=(char*) [0459]
U __fastcall async_string_length_005e5790(void* string, void*);                                // CExoString::GetLength [0188]
const char* __fastcall WindowCStr005e5670(void*, void*);                                       // CExoString::CStr [0078]
void __cdecl frame_LogFormat_005e5680(void* self, const char* format, ...);                   // CExoString::Format [0125]
void* __fastcall recovered_gff_construct_004105a0(void* gff, void*);                           // CResGFF::CResGFF [0886]
U __fastcall recovered_gff_00411260(void* gff, void*, void* root, void* type, void* version);  // CResGFF::CreateGFFFile
U __fastcall recovered_gff_00412450(void* gff, void*, void* list, void* parent, const char* label); // CResGFF::AddList
U __fastcall recovered_gff_004124e0(void* gff, void*, void* element, void* list, U type);      // CResGFF::AddListElement
U __fastcall recovered_gff_004125b0(void* gff, void*, void* child, void* parent, const char* label, U type); // AddStructToStruct
U __fastcall recovered_gff_00412620(void* gff, void*, void* s, U value, const char* label);    // CResGFF::WriteFieldBYTE
U __fastcall recovered_gff_004126c0(void* gff, void*, void* s, U value, const char* label);    // CResGFF::WriteFieldWORD
U __fastcall recovered_gff_00412760(void* gff, void*, void* s, U value, const char* label);    // CResGFF::WriteFieldDWORD
U __fastcall recovered_gff_004127b0(void* gff, void*, void* s, U value, const char* label);    // CResGFF::WriteFieldINT
U __fastcall recovered_gff_00412870(void* gff, void*, void* s, U value_bits, const char* label); // CResGFF::WriteFieldFLOAT
U __fastcall recovered_gff_004128c0(void* gff, void*, void* s, void* resref, const char* label); // CResGFF::WriteFieldCResRef
U __fastcall recovered_gff_00412970(void* gff, void*, void* s, void* string, const char* label); // CResGFF::WriteFieldCExoString
U __fastcall recovered_gff_00412c10(void* gff, void*, void* s, const void* data, U size, const char* label); // WriteFieldVOID
U __fastcall recovered_gff_00413030(void* gff, void*, void* path, U type);                     // CResGFF::WriteGFFFile
U __fastcall recovered_global_set_bit_005290c0(void* table, void*, void* bits, I index, U value); // CSWGlobalVariableTable::SetBit
U __fastcall kotor_fastcall_00414110(void* table, void*, void* row, void* column, I* out);     // C2DA::GetINTEntry(CExoString row) [0594]
U __fastcall kotor_m15_c2da_get_string_entry_00413de0(void* table, void*, void* row, void* column, void* out); // GetCExoStringEntry [0811]
U __fastcall gui_load_resman_create_directory_004068c0(void* resman, void*, void* path);       // CExoResMan::CreateDirectory2 [0298]
U __fastcall gui_load_resman_clean_directory_00409460(void* resman, void*, void* path, U a, U b); // CExoResMan::CleanDirectory [0298]
void __fastcall kotor_m15_client_set_load_screen_by_module_005edcf0(void* client, void*, void* module); // [0813]
U __fastcall load_screen_get_next_load_screen_hint_strref_005ee090(void* client, void*, void* module, U a); // [load_screen_adapters]
void __fastcall kotor_m15_client_set_load_screen_hint_005edf00(void* client, void*, U strref); // [0813]
void __fastcall kotor_m15_client_show_load_screen_005eda80(void* client, void*, U a, U b, U c); // [0813]
void __fastcall kotor_m15_client_set_load_bar_progress_005edd40(void* client, void*, U value, U a); // [0813]
void __fastcall frame_SelectedHilite_005f7500(void* internal, void*, U value);                 // CClientExoAppInternal::UpdateRumble [0457]
void __fastcall kotor_fastcall_005df570(void* input, void*);                                   // CExoInput::PauseRumble [0714]
void* __fastcall kotor_fastcall_005eda40(void* client, void*);                                 // CClientExoApp::GetSWGuiManager [0417]
void __cdecl kotor_cdecl_00401c10(U dt_bits, U a, U b);                                        // UpdateScreen [0785]
U __fastcall kotor_fastcall_005edff0(void* client, void*, U a);                                // CClientExoApp::GetLoadStep (AL) [1205]
void* __fastcall frame_R4_005ed690(void* client, void*);                                       // CClientExoApp::GetInGameGui [0457]
void* __fastcall kotor_fastcall_005ed8b0(void* client, void*);                                 // CClientExoApp::GetSWParty [0707]
void* __fastcall frame_GetCharacter_006346c0(void* party, void*, U index);                     // CSWParty::GetCharacter [0457]
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void* creature, void*);      // CSWCCreature::GetServerCreature
void* __fastcall recovered_creature_client_creature_004efb20(void* creature, void*);           // CSWSCreature::GetClientCreature
void* __fastcall kotor_fastcall_004aede0(void* server, void*);                                 // CServerExoApp::GetWorldTimer
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, void* day, void* time);        // CWorldTimer::GetWorldTime
void __fastcall recovered_timer_time_of_day_004add90(void* timer, void*, U ms, void* a, void* b, void* c, void* d); // ConvertFromTimeOfDay
void* __fastcall recovered_server_get_module_004b14f0(void* internal, void*);                  // CServerExoAppInternal::GetModule
void __fastcall recovered_module_time_004c4100(void* module, void*, void* a, void* b, void* c, void* d, void* e, void* f); // CSWSModule::GetTime
void* __fastcall recovered_party_journal_005633c0(void* table, void*);                         // CSWPartyTable::GetPartyJournal
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* array, void*, U id);  // CSWBaseItemArray::GetBaseItem [0820]
void __fastcall kotor_m15e_slot090_void0_004015a0(void*, void*);                               // 004015A0 void_return (CScriptLocation dtor) [0832]
void* __fastcall cerf_construct_cerf_file_005dd9c0(void* self);                                // CERFFile::CERFFile [cerf_quicksave]
void __fastcall cerf_destroy_cerf_file_005de3a0(void* self);                                   // CERFFile::~CERFFile [cerf_quicksave]
void* __fastcall kotor_fastcall_005edb60(void* client, void*);                                 // GetMoveToModuleMovies [route_bodies]
void* __fastcall kotor_fastcall_004b71e0(void* self, void*, void* source);                     // CExoArrayList<CExoString>(copy) [route_bodies]
// claude_w1_chains_erf.cpp
U __fastcall kotor_fastcall_005dcfc0(void* self, void*, void* path);
void __fastcall kotor_fastcall_005dce30(void* self, void*, const char* version);
U __fastcall kotor_fastcall_005dd080(void* self, void*);
U __fastcall kotor_fastcall_005dd5a0(void* self, void*);
void __fastcall kotor_fastcall_005de1a0(void* self, void*, void* directory, U type, void* callback, U a4, U a5);
U __fastcall kotor_fastcall_005dd0e0(void* self, void*);
U __fastcall kotor_fastcall_00406990(void* resman, void*, void* source, void* target, U source_type, U target_type);

// ---------------- exports of this file ----------------
void __fastcall kotor_fastcall_004b8300(void* self, void*, void* module, void* modules);
I __cdecl kotor_cdecl_004b30d0(I index, I count, I base, I span);
void __fastcall kotor_fastcall_005648c0(void* self, void*, void* directory);
U __fastcall kotor_fastcall_00563bf0(void* self, void*);
void __fastcall kotor_fastcall_00563d90(void* self, void*, void* gff, void* parent);
void* __fastcall kotor_fastcall_0062a9e0(void* gui, void*);
U __fastcall kotor_fastcall_0062a9f0(void* gui, void*);
U __fastcall kotor_fastcall_00406c60(void* resman, void*, void* a, U b, U c, U d);
void __fastcall kotor_fastcall_0052ad10(void* self, void*, void* directory);
void __fastcall kotor_fastcall_005299b0(void* self, void*, void* file, U lists);
U __fastcall kotor_fastcall_005ee050(void* client, void*);
U __fastcall kotor_fastcall_005ee070(void* client, void*);
void* __fastcall kotor_fastcall_004b2840(void* self, void*);
void __fastcall kotor_fastcall_004b1400(void* self, void*);
void __fastcall kotor_fastcall_004b2230(void* self, void*);
void __fastcall kotor_fastcall_004b28e0(void* self, void*, void* gff, void* parent);
void __fastcall kotor_fastcall_004b26c0(void* self, void*, void* gff, void* parent);
void* __fastcall kotor_fastcall_005edd00(void* client, void*, void* out, void* module);
void* __fastcall kotor_fastcall_005f3650(void* internal, void*, void* out, void* module);
void* __fastcall kotor_fastcall_004aed40(void* server, void*, void* out);
void __fastcall kotor_fastcall_0062f0a0(void* gui, void*, void* out);
}

namespace {
using Str = kotor_reconstruction::StartupString;
inline Str* str(void* p) { return static_cast<Str*>(p); }
struct alignas(4) ExoString { unsigned char b[8]; };                                         // CExoString {char*, len}
inline void s_default(void* s) { kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(s), nullptr); }
inline void s_text(void* s, const char* t) { (void)frame_LogStringCtor_005e5a90(s, nullptr, t); }
inline void s_destroy(void* s) { frame_LogStringDtor_005e5c20(s, nullptr); }
inline void s_assign(void* s, void* o) { (void)async_assign_string_005e5c50(s, nullptr, o); }
inline void s_concat(void* self, void* out, void* other) { (void)GuiConcat005e5d10(str(self), nullptr, str(out), str(other)); }
// One CExoString destroyed when its scope ends (normal exit or unwinding), mirroring the original EH states.
struct Scoped {
    ExoString s; bool live = false;
    void* p() { return s.b; }
    void* text(const char* t) { s_text(s.b, t); live = true; return s.b; }
    void* empty() { s_default(s.b); live = true; return s.b; }
    void* adopt() { live = true; return s.b; }                                                  // constructed by a callee
    void end() { if (live) { live = false; s_destroy(s.b); } }
    ~Scoped() { end(); }
};
inline void* client() { return client_app(); }
inline void* resman() { return kotor_resource_demand::resource_manager_007a39e8; }
constexpr U k_frame_dt_3d088889 = 0x3d088889u;                                                 // 1/30 s (immediate)
inline void draw_and_present() {                                                               // GetSWGuiManager()->Draw(1/30); UpdateScreen
    static_cast<gui_ctor::Gui*>(kotor_fastcall_005eda40(client(), nullptr))->Draw_0040cc50(flt(k_frame_dt_3d088889));
    kotor_cdecl_00401c10(k_frame_dt_3d088889, 0, 0);
}
inline void progress(U value) { kotor_m15_client_set_load_bar_progress_005edd40(client(), nullptr, value, 0); }
inline U percent(U step, U factor) { return static_cast<U>(static_cast<I>(step * factor) / 100); } // imul 51EB851Fh; sar 5; +sign
inline void gff_delete(void* gff) { if (gff != nullptr) reinterpret_cast<void(__thiscall*)(void*, U)>(at<void*>(at<void*>(gff, 0), 0))(gff, 1); }
inline void* new_gff() {                                                                      // operator new(A0h) + CResGFF()
    void* memory = live_operator_new(0xa0);
    if (memory == nullptr) return nullptr;
    try { return recovered_gff_construct_004105a0(memory, nullptr); } catch (...) { live_free(memory); throw; }
}
inline U field_byte(void* gff, void* s, U v, const char* l) { return recovered_gff_00412620(gff, nullptr, s, v, l); }
inline U field_word(void* gff, void* s, U v, const char* l) { return recovered_gff_004126c0(gff, nullptr, s, v, l); }
inline U field_dword(void* gff, void* s, U v, const char* l) { return recovered_gff_00412760(gff, nullptr, s, v, l); }
inline U field_int(void* gff, void* s, U v, const char* l) { return recovered_gff_004127b0(gff, nullptr, s, v, l); }
inline U field_string(void* gff, void* s, void* v, const char* l) { return recovered_gff_00412970(gff, nullptr, s, v, l); }
inline U add_list(void* gff, void* list, void* parent, const char* l) { return recovered_gff_00412450(gff, nullptr, list, parent, l); }
inline U add_element(void* gff, void* element, void* list) { return recovered_gff_004124e0(gff, nullptr, element, list, 0); }
struct GffList { unsigned char b[0x14]; };                                                     // CResList {struct id, char label[16]}
struct GffStruct { U id; };                                                                    // CResStruct
const char k_v20_00745338[] = "V2.0";
const char k_backslash_0073d8f4[] = "\\";
const char k_reboot_0073d8e8[] = "REBOOTDATA:";
const char k_empty_0073d71c[] = "";
const char k_load_fmt_00745580[] = "load_%s";
const char k_default_0073e304[] = "DEFAULT";
}  // namespace

extern "C" {
// =========================================================================================================
// Small accessors.
U __fastcall kotor_fastcall_00406c60(void*, void*, void*, U, U, U) { return 1; }               // 00406C60 CreateSignatureFile
void* __fastcall kotor_fastcall_0062a9e0(void* gui, void*) { return at<void*>(gui, 0xf8); }     // 0062A9E0 GetMessageBuffer
U __fastcall kotor_fastcall_0062a9f0(void* gui, void*) { return at<U>(gui, 0x100); }           // 0062A9F0 GetMessageBufferSize
// 005EE050 / 005EE070: EAX = [this+4] with AL = byte [[this+4]+490h / +491h] (upper bytes stay the pointer's).
U __fastcall kotor_fastcall_005ee050(void* client, void*) {
    const U p = at<U>(client, 4);
    return (p & 0xffffff00u) | at<U8>(at<void*>(client, 4), 0x490);                           // 005EE050..EE053
}
U __fastcall kotor_fastcall_005ee070(void* client, void*) {
    const U p = at<U>(client, 4);
    return (p & 0xffffff00u) | at<U8>(at<void*>(client, 4), 0x491);                           // 005EE070..EE073
}
// 004AED40 CServerExoApp::GetMoveToModuleStartWaypoint(CExoString* out): copy of internal +1008Ch. RET 4.
void* __fastcall kotor_fastcall_004aed40(void* server, void*, void* out) {
    (void)WindowCopy005e5b00(out, nullptr, static_cast<unsigned char*>(at<void*>(server, 4)) + 0x1008c); // 004AED41..ED59
    return out;
}
// 0062F0A0 CGuiInGame::GetStatusSummary(CStatusSummary* out): 18h bytes from [this+A8h]+64h when present. RET 4.
void __fastcall kotor_fastcall_0062f0a0(void* gui, void*, void* out) {
    void* summary = at<void*>(gui, 0xa8);                                                      // 0062F0A0
    if (summary != nullptr) std::memcpy(out, static_cast<unsigned char*>(summary) + 0x64, 0x18); // 0062F0A6..F0B8
}

// 004B30D0 UpdateLoadScreen(int index, int count, int base, int span): progress = ftol(index / count * span) + base.
I __cdecl kotor_cdecl_004b30d0(I index, I count, I base, I span) {
    const U value = w2::ftol2_006fae8c(static_cast<double>(index) / static_cast<double>(count) * static_cast<double>(span)) +
                    static_cast<U>(base);                                                      // 004B30D0..30E3
    progress(value);                                                                           // 004B30E7..30F0
    kotor_cdecl_00401c10(k_frame_dt_3d088889, 0, 0);                                           // 004B30F5..3103
    return 0;                                                                                  // 004B3106
}

// =========================================================================================================
// 004B1400 KOTOR_AUTOSAVE_PARAMS::Reset: empties the eight strings (+8 twice) and zeroes the time block. RET 0.
void __fastcall kotor_fastcall_004b1400(void* self, void*) {
    unsigned char* p = static_cast<unsigned char*>(self);
    (void)frame_LogAssign_005e5140(p + 8, nullptr, k_empty_0073d71c);                          // 004B1404..1413
    (void)frame_LogAssign_005e5140(p, nullptr, k_empty_0073d71c);                              // 004B1413..141F
    for (U off = 8; off <= 0x38; off += 8) (void)frame_LogAssign_005e5140(p + off, nullptr, k_empty_0073d71c); // 004B141F..1474
    at<U>(self, 0x40) = 0;                                                                     // 004B147E
    at<U8>(self, 0x44) = 0; at<U8>(self, 0x45) = 0; at<U8>(self, 0x46) = 0;                    // 004B1481..1487
    at<U16>(self, 0x48) = 0; at<U16>(self, 0x4a) = 0; at<U16>(self, 0x4c) = 0;                 // 004B148A..1492
    at<U>(self, 0x50) = 0; at<U>(self, 0x54) = 0;                                              // 004B1496..1499
    std::memset(p + 0x58, 0, 0x18);                                                            // 004B149C..14A1
}
// 004B2840 KOTOR_AUTOSAVE_PARAMS::KOTOR_AUTOSAVE_PARAMS: eight CExoStrings then Reset. RET 0, EAX = this.
void* __fastcall kotor_fastcall_004b2840(void* self, void*) {
    unsigned char* p = static_cast<unsigned char*>(self);
    U built = 0;
    try {
        for (; built < 8; ++built) s_default(p + built * 8);                                   // 004B2857..28BB (EH states 0..7)
        kotor_fastcall_004b1400(self, nullptr);                                                // 004B28C0..28C7
    } catch (...) {
        while (built > 0) { --built; s_destroy(p + built * 8); }
        throw;
    }
    return self;
}
// 004B2230 KOTOR_AUTOSAVE_PARAMS::~KOTOR_AUTOSAVE_PARAMS: strings +38h .. +0. RET 0.
void __fastcall kotor_fastcall_004b2230(void* self, void*) {
    unsigned char* p = static_cast<unsigned char*>(self);
    for (I off = 0x38; off >= 0; off -= 8) s_destroy(p + off);                                 // 004B224D..22B5
}
// 004B26C0 CStatusSummary::SaveToGFF(CResGFF*, CResStruct*): the flag word is passed shifted with AND FFFFFF01h
// (upper bits kept, as the original). RET 8.
void __fastcall kotor_fastcall_004b26c0(void* self, void*, void* gff, void* s) {
    const U flags_byte = at<U8>(self, 0);
    (void)field_byte(gff, s, flags_byte & 0xffffff01u, "DISPLAYSPENDING");                     // 004B26C8..26DF
    (void)field_byte(gff, s, (at<U>(self, 0) >> 2) & 0xffffff01u, "ITEMRECEIVED");             // 004B26E4..26F8
    (void)field_byte(gff, s, (at<U>(self, 0) >> 3) & 0xffffff01u, "ITEMLOST");                 // 004B26FD..2711
    (void)field_byte(gff, s, (at<U>(self, 0) >> 4) & 0xffffff01u, "JOURNAL");                  // 004B2716..2729
    (void)field_byte(gff, s, at<U8>(self, 4), "LIGHTSHIFT");                                   // 004B272E..273C
    (void)field_byte(gff, s, at<U8>(self, 5), "DARKSHIFT");                                    // 004B2741..274F
    (void)field_int(gff, s, at<U>(self, 8), "CREDITS");                                        // 004B2754..2760
    (void)field_byte(gff, s, (at<U>(self, 0) >> 8) & 0xffffff01u, "CREDITSNET");               // 004B2765..2779
    (void)field_int(gff, s, at<U>(self, 0xc), "XP");                                           // 004B277E..278A
    (void)field_int(gff, s, at<U>(self, 0x10), "STEALTHXP");                                   // 004B278F..279B
    (void)field_byte(gff, s, (at<U>(self, 0) >> 1) & 0xffffff01u, "SOUNDPENDING");             // 004B27A0..27B3
    (void)field_byte(gff, s, (at<U>(self, 0) >> 5) & 0xffffff01u, "LEVELUPSOUND");             // 004B27B8..27CC
    (void)field_byte(gff, s, (at<U>(self, 0) >> 6) & 0xffffff01u, "NEWQUESTSOUND");            // 004B27D1..27E4
    (void)field_byte(gff, s, (at<U>(self, 0) >> 7) & 0xffffff01u, "COMPLETESOUND");            // 004B27E9..27FD
    (void)field_int(gff, s, at<U>(self, 0x14), "SUPPRESSED");                                  // 004B2802..280E
}
// 004B28E0 KOTOR_AUTOSAVE_PARAMS::SaveToGFF(CResGFF*, CResStruct*): RET 8.
void __fastcall kotor_fastcall_004b28e0(void* self, void*, void* gff, void* s) {
    unsigned char* p = static_cast<unsigned char*>(self);
    static const char* const labels[8] = {"LOADMUSIC", "STARTWAYPOINT", "MOVIE1", "MOVIE2", "MOVIE3", "MOVIE4",
                                          "MOVIE5", "MOVIE6"};
    for (U k = 0; k < 8; ++k) (void)field_string(gff, s, p + k * 8, labels[k]);                // 004B28ED..296D
    (void)field_dword(gff, s, at<U>(self, 0x40), "TIME_YEAR");                                 // 004B2972..297E
    (void)field_byte(gff, s, at<U8>(self, 0x44), "TIME_MONTH");                                // 004B2983..2991
    (void)field_byte(gff, s, at<U8>(self, 0x45), "TIME_DAY");                                  // 004B2996..29A4
    (void)field_byte(gff, s, at<U8>(self, 0x46), "TIME_HOUR");                                 // 004B29A9..29B7
    (void)field_word(gff, s, at<U16>(self, 0x48), "TIME_MINUTE");                              // 004B29BC..29CB
    (void)field_word(gff, s, at<U16>(self, 0x4a), "TIME_SECOND");                              // 004B29D0..29DF
    (void)field_word(gff, s, at<U16>(self, 0x4c), "TIME_MILLISECOND");                         // 004B29E4..29F3
    (void)field_dword(gff, s, at<U>(self, 0x50), "TIME_PAUSEDAY");                             // 004B29F8..2A04
    (void)field_dword(gff, s, at<U>(self, 0x54), "TIME_PAUSETIME");                            // 004B2A09..2A15
    GffStruct child;                                                                           // [esp+14h] (argument slot)
    (void)recovered_gff_004125b0(gff, nullptr, &child, s, "STATUSSUMMARY", 0);                 // 004B2A1A..2A29
    kotor_fastcall_004b26c0(p + 0x58, nullptr, gff, &child);                                   // 004B2A2E..2A37
}

// =========================================================================================================
// 005F3650 CClientExoAppInternal::GetLoadMusicByModuleName(CExoString* out, CExoString const& module): RET 8.
void* __fastcall kotor_fastcall_005f3650(void*, void*, void* out, void* module) {
    Scoped music;                                                                              // [esp+10h]
    music.empty();                                                                             // 005F3670..367C
    if (async_string_length_005e5790(module, nullptr) == 0) {                                  // 005F3681..3695
        (void)frame_LogAssign_005e5140(music.p(), nullptr, k_empty_0073d71c);                  // 005F3697..36A0
    } else {
        void* table = at<void*>(at<void*>(kotor_cell_rules_007a3a28, 0xb8), 0x18);              // 005F36C7..36D3
        U found;
        {
            Scoped column; column.text("MusicResRef");                                         // 005F36B9..36C2
            found = kotor_m15_c2da_get_string_entry_00413de0(table, nullptr, module, column.p(), music.p()); // 005F36D6..36E8
        }                                                                                      // 005F36F3..36FE
        if (found == 0) {                                                                      // 005F36ED..3705
            frame_LogFormat_005e5680(music.p(), k_load_fmt_00745580, at<U>(module, 0), at<U>(module, 4)); // 005F370B..371C (by value)
            menu_resources::Ref ref;                                                           // [esp+20h]
            (void)async_ref_construct_string_00406d60(&ref, nullptr, music.p());              // 005F3724..372D
            if (static_cast<menu_resources::Manager*>(resman())->exists_00408bc0(&ref, 8, nullptr) == 0) { // 005F3732..3748
                Scoped column; column.text("MusicResRef");                                     // 005F374A..3753 [esp+20h]
                Scoped row; row.text(k_default_0073e304);                                      // 005F375A..3767 [esp+18h]
                void* table2 = at<void*>(at<void*>(kotor_cell_rules_007a3a28, 0xb8), 0x18);     // 005F376C..3778
                (void)kotor_m15_c2da_get_string_entry_00413de0(table2, nullptr, row.p(), column.p(), music.p()); // 005F377B..3791
            }                                                                                  // 005F3796..37AC (~row, ~column)
        }
    }
    (void)WindowCopy005e5b00(out, nullptr, music.p());                                         // 005F36A5..36AF / 005F37B1..37BB
    return out;                                                                                // 005F37C0..37D1 (~music)
}
// 005EDD00 CClientExoApp::GetLoadMusicByModuleName: forwards to the internal app. RET 8, EAX = out.
void* __fastcall kotor_fastcall_005edd00(void* client_object, void*, void* out, void* module) {
    (void)kotor_fastcall_005f3650(at<void*>(client_object, 4), nullptr, out, module);          // 005EDD01..DD17
    return out;
}

// =========================================================================================================
// 00563BF0 CSWPartyTable::GetTotalPlayedSeconds: (now - session start + 0.5 s) / 1 s + stored seconds; a negative
// difference counts as 0. RET 0.
U __fastcall kotor_fastcall_00563bf0(void* self, void*) {
    U ft[2];                                                                                   // FILETIME [esp+8]
    GetSystemTimeAsFileTime(ft);                                                               // 00563BF5..3BFC
    const unsigned long long now = (static_cast<unsigned long long>(ft[1]) << 32) | ft[0];
    const unsigned long long start = (static_cast<unsigned long long>(kotor_cell_party_session_start_hi_007a3a24) << 32) |
                                     kotor_cell_party_session_start_lo_007a3a20;               // 00563C10..3C18
    unsigned long long diff = now - start;                                                     // 00563C1E..3C20
    if (static_cast<long long>(diff) < 0) diff = 0;                                            // 00563C22..3C2E (only a negative high dword)
    const unsigned long long seconds = (diff + 0x4c4b40u) / 0x989680u;                         // 00563C30..3C42 __aulldiv
    return static_cast<U>(seconds) + at<U>(self, 0x114);                                       // 00563C47..3C4E
}

// 00563D90 CSWPartyTable::SaveJournal(CResGFF* gff, CResStruct* parent): RET 8.
void __fastcall kotor_fastcall_00563d90(void* self, void*, void* gff, void* parent) {
    void* journal = recovered_party_journal_005633c0(self, nullptr);                           // 00563D94
    if (journal == nullptr || at<U>(journal, 4) == 0) return;                                  // 00563D9B..3DA8
    (void)field_int(gff, parent, recovered_journal_sort_order_00833a90, "JNL_SortOrder");      // 00563DAE..3DC7
    GffList list;                                                                              // [esp+10h]
    (void)add_list(gff, &list, parent, "JNL_Entries");                                         // 00563DCC..3DD9
    GffStruct element;                                                                         // argument slot [esp+30h]
    for (I i = 0; i < at<I>(journal, 4); ++i) {                                                // 00563DDE..3E6C
        unsigned char* entry = at<unsigned char*>(journal, 0) + static_cast<U>(i) * 0x3c;
        (void)add_element(gff, &element, &list);                                               // 00563DF0..3DFE
        entry = at<unsigned char*>(journal, 0) + static_cast<U>(i) * 0x3c;                     // 00563E03 (re-read)
        (void)field_string(gff, &element, entry + 0x18, "JNL_PlotID");                         // 00563E05..3E16
        (void)field_int(gff, &element, at<U>(at<unsigned char*>(journal, 0) + static_cast<U>(i) * 0x3c, 0x20), "JNL_State"); // 00563E1B..3E2E
        (void)field_dword(gff, &element, at<U>(at<unsigned char*>(journal, 0) + static_cast<U>(i) * 0x3c, 0x10), "JNL_Date"); // 00563E33..3E46
        (void)field_dword(gff, &element, at<U>(at<unsigned char*>(journal, 0) + static_cast<U>(i) * 0x3c, 0x14), "JNL_Time"); // 00563E4B..3E5E
    }
}

// 005648C0 CSWPartyTable::SaveTableInfo(CExoString const& dir): writes <dir>\PARTYTABLE (GFF "PT  "). RET 4.
void __fastcall kotor_fastcall_005648c0(void* self, void*, void* directory) {
    Scoped filename; filename.empty();                                                         // [esp+24h] 005648DE..48E2
    U galaxy_bits = 0;                                                                         // [esp+10h] 005648F2
    void* gff = new_gff();                                                                     // 005648E9..4912
    GffStruct root;                                                                            // [esp+18h]
    U created;
    {
        Scoped version; version.text(k_v20_00745338);                                          // 00564914..4922 [esp+2Ch]
        Scoped type; type.text("PT  ");                                                        // 00564927..4936 [esp+1Ch]
        created = recovered_gff_00411260(gff, nullptr, &root, type.p(), version.p());          // 0056493B..4951
    }                                                                                          // 00564956..496E
    if (created == 1) {                                                                        // 00564973..4976
        {
            Scoped name; name.text("PARTYTABLE");                                              // 0056497C..4985 [esp+1Ch]
            Scoped sep; sep.text(k_backslash_0073d8f4);                                        // 0056498A..4998 [esp+2Ch]
            Scoped first; s_concat(directory, first.adopt(), sep.p());                         // 0056499D..49B1 [esp+3Ch]
            Scoped second; s_concat(first.p(), second.adopt(), name.p());                      // 005649B6..49C7 [esp+34h]
            s_assign(filename.p(), second.p());                                                // 005649CC..49D6
            second.end(); first.end(); sep.end(); name.end();                                  // 005649DB..4A0D
        }
        at<U>(self, 0x114) = kotor_fastcall_00563bf0(self, nullptr);                           // 00564A12..4A1E
        U ft[2];                                                                               // [esp+2Ch]
        GetSystemTimeAsFileTime(ft);                                                           // 00564A19..4A24
        kotor_cell_party_session_start_hi_007a3a24 = ft[1];                                    // 00564A2E..4A3A
        kotor_cell_party_session_start_lo_007a3a20 = ft[0];                                    // 00564A2A..4A40
        (void)field_dword(gff, &root, at<U>(self, 0xfc), "PT_GOLD");                           // 00564A45..4A58
        (void)field_int(gff, &root, at<U>(self, 0xf8), "PT_XP_POOL");                          // 00564A5D..4A70
        (void)field_dword(gff, &root, at<U>(self, 0x114), "PT_PLAYEDSECONDS");                 // 00564A75..4A88
        (void)field_int(gff, &root, at<U>(self, 0xf0), "PT_CONTROLLED_NPC");                   // 00564A8D..4AA0
        (void)field_byte(gff, &root, at<U>(self, 0x190), "PT_SOLOMODE");                       // 00564AA5..4AB8
        (void)field_byte(gff, &root, at<U>(self, 0x194), "PT_CHEAT_USED");                     // 00564ABD..4AD0
        (void)field_byte(gff, &root, at<U8>(self, 0), "PT_NUM_MEMBERS");                       // 00564AD5..4AE7
        GffList list;                                                                          // [esp+44h]
        GffStruct element;                                                                     // [esp+14h]
        (void)add_list(gff, &list, &root, "PT_MEMBERS");                                       // 00564AEC..4AFD
        for (I i = 0; i < at<I>(self, 0); ++i) {                                               // 00564B02..4B69
            const U id = at<U>(self, 4 + static_cast<U>(i) * 4);                               // 00564B10..4B16
            const U leader = id == at<U>(self, 0xec) ? 1u : 0u;                                // 00564B18..4B2C
            (void)add_element(gff, &element, &list);                                           // 00564B1A..4B2D
            (void)field_int(gff, &element, at<U>(self, 4 + static_cast<U>(i) * 4), "PT_MEMBER_ID"); // 00564B32..4B41
            (void)field_byte(gff, &element, leader, "PT_IS_LEADER");                           // 00564B46..4B53
        }
        (void)add_list(gff, &list, &root, "PT_AVAIL_NPCS");                                    // 00564B6B..4B7C
        for (U k = 0; k < 9; ++k) {                                                            // 00564B81..4BCE
            (void)add_element(gff, &element, &list);                                           // 00564B86..4B94
            (void)field_byte(gff, &element, at<U8>(self, 0x30 + k * 4), "PT_NPC_AVAIL");       // 00564B99..4BAA
            (void)field_byte(gff, &element, at<U8>(self, 0x54 + k), "PT_NPC_SELECT");          // 00564BAF..4BC2
        }
        (void)field_int(gff, &root, at<U>(self, 0xe4), "PT_AISTATE");                          // 00564BD0..4BE3
        (void)field_int(gff, &root, at<U>(self, 0xe8), "PT_FOLLOWSTATE");                      // 00564BE8..4BFB
        for (U n = 0; n < 0x10; ++n) {                                                         // 00564C00..4D69 (unrolled x8, twice)
            if (at<U>(self, 0x60 + n * 4) != 0) galaxy_bits |= 1u << n;                        // [edx-40h+4j] -> bit n
            if (at<U>(self, 0xa0 + n * 4) != 0) galaxy_bits |= 1u << (n + 0x10);               // [edx+4j]     -> bit n+10h
        }
        GffStruct map;                                                                         // [esp+14h]
        (void)recovered_gff_004125b0(gff, nullptr, &map, &root, "GlxyMap", 0);                 // 00564D6F..4D82
        (void)field_dword(gff, &map, 0x10, "GlxyMapNumPnts");                                  // 00564D87..4D95
        (void)field_dword(gff, &map, galaxy_bits, "GlxyMapPlntMsk");                           // 00564D9A..4DAB
        (void)field_int(gff, &map, at<U>(self, 0xe0), "GlxyMapSelPnt");                        // 00564DB0..4DC3
        (void)add_list(gff, &list, &root, "PT_PAZAAKCARDS");                                   // 00564DC8..4DD9
        for (U k = 0; k < 0x12; ++k) {                                                         // 00564DDE..4E1B
            (void)add_element(gff, &element, &list);                                           // 00564DF0..4DFE
            (void)field_int(gff, &element, at<U>(self, 0x120 + k * 4), "PT_PAZAAKCOUNT");      // 00564E03..4E12
        }
        (void)add_element(gff, &element, &list);                                               // 00564E1D..4E2B (19th element)
        (void)field_byte(gff, &element, at<U>(self, 0x194), "PT_PAZAAKCOUNT");                 // 00564E30..4E43 (original)
        (void)add_list(gff, &list, &root, "PT_PAZSIDELIST");                                   // 00564E48..4E59
        for (U k = 0; k < 0xa; ++k) {                                                          // 00564E5E..4E9B
            (void)add_element(gff, &element, &list);                                           // 00564E70..4E7E
            (void)field_int(gff, &element, at<U>(self, 0x168 + k * 4), "PT_PAZSIDECARD");      // 00564E83..4E92
        }
        void* gui = frame_R4_005ed690(client(), nullptr);                                      // 00564E9D..4EA5
        (void)recovered_gff_00412c10(gff, nullptr, &root, static_cast<unsigned char*>(gui) + 0xba8, 6, "PT_TUT_WND_SHOWN"); // 00564EAA..4EBE
        (void)field_int(gff, &root, at<U>(frame_R4_005ed690(client(), nullptr), 0x2c), "PT_LAST_GUI_PNL"); // 00564EC3..4EE1
        (void)add_list(gff, &list, &root, "PT_FB_MSG_LIST");                                   // 00564EE6..4EF7
        unsigned char* message = static_cast<unsigned char*>(kotor_fastcall_0062a9e0(frame_R4_005ed690(client(), nullptr), nullptr)); // 00564EFC..4F0B
        for (I i = 0; i < static_cast<I>(kotor_fastcall_0062a9f0(frame_R4_005ed690(client(), nullptr), nullptr)); ++i) { // 00564F10..4F9E
            (void)add_element(gff, &element, &list);                                           // 00564F30..4F3E
            (void)field_string(gff, &element, message, "PT_FB_MSG_MSG");                       // 00564F43..4F53
            (void)field_dword(gff, &element, at<U>(message, 8), "PT_FB_MSG_TYPE");             // 00564F58..4F68
            (void)field_byte(gff, &element, at<U8>(message, 0xc), "PT_FB_MSG_COLOR");          // 00564F6D..4F7E
            message += 0x10;                                                                   // 00564F8D
        }
        (void)add_list(gff, &list, &root, "PT_DLG_MSG_LIST");                                  // 00564FA0..4FB1
        unsigned char* dialog = at<unsigned char*>(frame_R4_005ed690(client(), nullptr), 0xfc); // 00564FB6..4FCD
        for (I i = 0; i < at<I>(frame_R4_005ed690(client(), nullptr), 0x104); ++i) {           // 00564FD3..5036
            (void)add_element(gff, &element, &list);                                           // 00564FE4..4FF2
            (void)field_string(gff, &element, dialog + 8, "PT_DLG_MSG_SPKR");                  // 00564FF7..5007
            (void)field_string(gff, &element, dialog, "PT_DLG_MSG_MSG");                       // 0056500C..5019
            dialog += 0x10;                                                                    // 00565028
        }
        (void)add_list(gff, &list, &root, "PT_COST_MULT_LIST");                                // 00565038..5049
        for (I i = 0; i < at<I>(at<void*>(kotor_cell_rules_007a3a28, 0x34), 0); ++i) {          // 0056504E..50AD
            (void)add_element(gff, &element, &list);                                           // 00565060..506E
            void* base = kotor_m15b_base_item_array_get_base_item_005b31a0(at<void*>(kotor_cell_rules_007a3a28, 0x34), nullptr,
                                                                          static_cast<U>(i));  // 00565073..507C
            (void)recovered_gff_00412870(gff, nullptr, &element, at<U>(base, 0xc4), "PT_COST_MULT_VALUE"); // 00565081..509A
        }
        kotor_fastcall_00563d90(self, nullptr, gff, &root);                                    // 005650AF..50B7
        (void)recovered_gff_00413030(gff, nullptr, filename.p(), 0);                           // 005650BC..50C5
    }
    if (str(directory)->BorrowedEquals005e5290(k_reboot_0073d8e8) != 0)                       // 005650CA..50DA
        (void)kotor_fastcall_00406c60(resman(), nullptr, filename.p(), 0, 0, 0);               // 005650DC..50ED
    gff_delete(gff);                                                                           // 005650F2..50FC
}                                                                                              // 005650FE..510A (~filename)

// =========================================================================================================
// 005299B0 CSWGlobalVariableTable::WriteTable(CExoString const& file, int lists): GFF "GVT " with the 6EFh slots
// packed by kind (bool bits / number bytes / 18h-byte locations / strings). The value arrays live in one frame
// with the original layout and sizes (5 strings, 64h locations), so overruns land where the original's would.
// RET 8.
void __fastcall kotor_fastcall_005299b0(void* self, void*, void* file, U lists) {
    alignas(8) unsigned char f[0xcb0];                                                         // [esp+0..CAFh]
    unsigned char* const s = static_cast<unsigned char*>(self);
    for (U k = 0; k < 0x64; ++k) std::memset(f + 0x350 + k * 0x18, 0, 0x18);                  // 005299D4..99EC CScriptLocation x64h (004CA7A0)
    for (U k = 0; k < 5; ++k) s_default(f + 0x7c + k * 8);                                     // 005299F1..9A12 CExoString x5
    struct ArrayGuard {
        unsigned char* f;
        ~ArrayGuard() {
            for (I k = 4; k >= 0; --k) s_destroy(f + 0x7c + k * 8);                            // 00529E7E..9E97 (reverse)
            for (I k = 0x63; k >= 0; --k) kotor_m15e_slot090_void0_004015a0(f + 0x350 + k * 0x18, nullptr); // 00529E9C..9EB8
        }
    } arrays{f};
    Scoped path; path.empty();                                                                 // [esp+48h] 00529A17..9A23
    {
        Scoped colon; colon.text(":");                                                         // 00529A28..9A39 [esp+18h]
        const int pos = client_movies::StringFind_005e5eb0(static_cast<options_video::String*>(file), nullptr, static_cast<options_video::String*>(colon.p()), 0);       // 00529A3E..9A52
        colon.end();                                                                           // 00529A57..9A65
        if (pos != -1) {                                                                       // 00529A6A..9A6D
            Scoped left;                                                                       // [esp+40h]
            (void)base_ini_reconstruction::left_005e5f70(str(file), nullptr, str(left.adopt()), pos + 1); // 00529A6F..9A78
            s_assign(path.p(), left.p());                                                      // 00529A7D..9A8A
        }                                                                                      // 00529A8F..9A9B
    }
    void* gff = new_gff();                                                                     // 00529AA0..9AD4 [esp+14h]
    GffStruct& root = *reinterpret_cast<GffStruct*>(f + 0x10);                                 // [esp+10h]
    {
        Scoped version; version.text(k_v20_00745338);                                          // 00529AD8..9AE9 [esp+34h]
        Scoped type; type.text("GVT ");                                                        // 00529AEE..9AFF [esp+18h]
        (void)recovered_gff_00411260(gff, nullptr, &root, type.p(), version.p());              // 00529B04..9B1D
    }                                                                                          // 00529B22..9B3F
    void* const cat_boolean = f + 0xb8; void* const cat_number = f + 0x54;
    void* const cat_location = f + 0xa4; void* const cat_string = f + 0xcc;
    if (lists != 0) {                                                                          // 00529B44..9B49
        (void)add_list(gff, cat_boolean, &root, "CatBoolean");                                 // 00529B4B..9B5F
        (void)add_list(gff, cat_number, &root, "CatNumber");                                   // 00529B64..9B75
        (void)add_list(gff, cat_location, &root, "CatLocation");                               // 00529B7A..9B8E
        (void)add_list(gff, cat_string, &root, "CatString");                                   // 00529B93..9BA7
    }
    unsigned char* const bools = f + 0xe0;                                                     // 71h bytes
    std::memset(bools, 0, 0x71);                                                               // 00529BAC..9BBC
    unsigned char* const numbers = f + 0x158;                                                  // 1F4h bytes
    std::memset(numbers, 0, 0x7d * 4);                                                         // 00529BBD..9BCB
    void* list = f + 0x30;                                                                     // edi (= [esp+30h], first read as data)
    std::memcpy(&list, f + 0x30, 4);
    I bool_count = 0;                                                                          // [esp+3Ch]
    I string_count = 0;                                                                        // [esp+24h]
    unsigned char* number_out = numbers;                                                       // [esp+34h]
    unsigned char* string_out = f + 0x7c;                                                      // [esp+20h]
    unsigned char* location_out = f + 0x350;                                                   // [esp+2Ch]
    void* const element = f + 0x50;                                                            // [esp+50h]
    for (U slot = 0; slot < 0x6ef; ++slot) {                                                   // 00529BFE..9D75
        unsigned char* const entry = s + slot * 0x18;                                          // [esp+28h] - 16h
        if (entry[0] == 0) continue;                                                           // 00529C10..9C15
        const U word = at<U16>(entry, 0x16);                                                   // 00529C1B
        const U index = word & 0x3fff;
        switch (word >> 14) {                                                                  // 00529C1E..9C2C (table 00529ED4)
        case 0: {                                                                              // 00529C33 boolean
            const U bit = (static_cast<U>(s[0xa668 + static_cast<U>(static_cast<I>(index) / 8)]) >>
                           (7 - (index - (static_cast<U>(static_cast<I>(index) / 8) * 8)))) & 1u; // 00529C38..9C64
            (void)recovered_global_set_bit_005290c0(self, nullptr, bools, bool_count, bit);   // 00529C67..9C71
            ++bool_count;                                                                      // 00529C76..9C77
            list = cat_boolean;                                                                // 00529C7F
            break;
        }
        case 1:                                                                                // 00529C8B number
            *number_out++ = s[0xa6d9 + index];                                                 // 00529C90..9CA2
            list = cat_number;                                                                 // 00529C9E
            break;
        case 2:                                                                                // 00529CA8 location
            std::memcpy(location_out, s + 0xa8d0 + index * 0x18, 0x18);                        // 00529CA8..9CC4
            location_out += 0x18;                                                              // 00529CCA..9CD4
            list = cat_location;                                                               // 00529CCD
            break;
        default:                                                                               // 00529CDA string (word >> 14 == 3)
            s_assign(string_out, s + 0xb230 + index * 8);                                      // 00529CDA..9CEB
            ++string_count;                                                                    // 00529CF0..9CF8
            string_out += 8;                                                                   // 00529CF9
            list = cat_string;                                                                 // 00529CFC
            break;
        }
        if (lists == 0) continue;                                                              // 00529D0B..9D10
        (void)add_element(gff, element, list);                                                 // 00529D12..9D1C
        Scoped name; name.text(reinterpret_cast<const char*>(entry));                          // 00529D21..9D2D
        (void)field_string(gff, element, name.p(), "Name");                                    // 00529D32..9D4B
    }                                                                                          // 00529D50..9D5C (~name)
    const I bool_total = at<I>(self, 0xb258);                                                  // 00529D7B
    (void)recovered_gff_00412c10(gff, nullptr, &root, bools, static_cast<U>(bool_total / 8 + 1), "ValBoolean"); // 00529D81..9DA0
    (void)recovered_gff_00412c10(gff, nullptr, &root, numbers, at<U>(self, 0xb25c), "ValNumber"); // 00529DA5..9DC0
    (void)recovered_gff_00412c10(gff, nullptr, &root, f + 0x350, 0x960, "ValLocation");        // 00529DC5..9DDE
    void* const val_string = f + 0x68;                                                         // [esp+68h]
    (void)add_list(gff, val_string, &root, "ValString");                                       // 00529DE3..9DF4
    void* const string_element = f + 0x30;                                                     // [esp+30h]
    for (I k = 0; k < string_count; ++k) {                                                     // 00529DF9..9E2E
        (void)add_element(gff, string_element, val_string);                                    // 00529E05..9E13
        (void)field_string(gff, string_element, f + 0x7c + static_cast<U>(k) * 8, "String");   // 00529E18..9E25
    }
    (void)recovered_gff_00413030(gff, nullptr, file, 0);                                       // 00529E30..9E38
    gff_delete(gff);                                                                           // 00529E3D..9E47
    if (str(path.p())->BorrowedEquals005e5290(k_reboot_0073d8e8) != 0)                        // 00529E49..9E59
        (void)kotor_fastcall_00406c60(resman(), nullptr, file, 0, 0, 0);                       // 00529E5B..9E68
}                                                                                              // 00529E6D.. (~path, arrays)

// 0052AD10 CSWGlobalVariableTable::Save(CExoString const& dir): WriteTable(dir + "\" + "GLOBALVARS", 1). RET 4.
void __fastcall kotor_fastcall_0052ad10(void* self, void*, void* directory) {
    Scoped name; name.text("GLOBALVARS");                                                      // 0052AD2B..AD34 [esp+0Ch]
    Scoped sep; sep.text(k_backslash_0073d8f4);                                                // 0052AD39..AD4A [esp+4]
    Scoped first; s_concat(directory, first.adopt(), sep.p());                                 // 0052AD4F..AD62 [esp+1Ch]
    Scoped path; s_concat(first.p(), path.adopt(), name.p());                                  // 0052AD67..AD78 [esp+14h]
    first.end(); sep.end(); name.end();                                                        // 0052AD7D..ADA2
    kotor_fastcall_005299b0(self, nullptr, path.p(), 1);                                       // 0052ADA7..ADB0
}                                                                                              // 0052ADB5..ADC1 (~path)

// =========================================================================================================
// 004B8300 CServerExoAppInternal::DoPCAutosave(CExoString const& module, C2DA* modules): RET 8.
void __fastcall kotor_fastcall_004b8300(void* self, void*, void* module, void* modules) {
    U temp_flags = 0;                                                                          // [esp+0Ch]
    kotor_m15_client_set_load_screen_by_module_005edcf0(client(), nullptr, module);            // 004B832F..833E
    {
        void* c = client();                                                                    // 004B8343..8349 ebx
        const U hint = load_screen_get_next_load_screen_hint_strref_005ee090(c, nullptr, module, 0); // 004B834C..8351
        kotor_m15_client_set_load_screen_hint_005edf00(c, nullptr, hint);                      // 004B8356..8359
    }
    kotor_m15_client_show_load_screen_005eda80(client(), nullptr, 0, 1, 1);                    // 004B835E..836D
    progress(0);                                                                               // 004B8372..837E
    frame_SelectedHilite_005f7500(at<void*>(client(), 4), nullptr, 0);                         // 004B8383..8393
    kotor_fastcall_005df570(kotor_menu_window_cells::input_007a39e4, nullptr);                 // 004B8398..839E
    draw_and_present();                                                                        // 004B83A3..83CB
    {
        Scoped saves; saves.text("SAVES:");                                                    // 004B83CE..83D7 [esp+34h]
        (void)gui_load_resman_create_directory_004068c0(resman(), nullptr, saves.p());         // 004B83DC..83F2
    }                                                                                          // 004B83F7..8406
    Scoped path; path.empty();                                                                 // [esp+2Ch] 004B840B..840F
    frame_LogFormat_005e5680(path.p(), "SAVES:%06d - %s", 1, "AUTOSAVE");                      // 004B8414..8430
    if (gui_load_resman_create_directory_004068c0(resman(), nullptr, path.p()) == 0)           // 004B8435..844A
        (void)gui_load_resman_clean_directory_00409460(resman(), nullptr, path.p(), 0, 0);     // 004B844C..8459
    const U step = kotor_fastcall_005edff0(client(), nullptr, 0) & 0xffu;                      // 004B845E..8477 (movzx ebx, al)
    progress(percent(step, 5));                                                                // 004B847A..8491
    draw_and_present();                                                                        // 004B8496..84BE
    kotor_fastcall_005648c0(static_cast<unsigned char*>(self) + 0x1b770, nullptr, path.p());   // 004B84C1..84CC
    progress(percent(step, 10));                                                               // 004B84D1..84F2
    draw_and_present();                                                                        // 004B84F7..851F
    kotor_fastcall_0052ad10(static_cast<unsigned char*>(self) + 0x100fc, nullptr, path.p());   // 004B8522..852D
    progress(percent(step, 15));                                                               // 004B8532..8553
    draw_and_present();                                                                        // 004B8558..8580
    {
        Scoped sep; sep.text(k_backslash_0073d8f4);                                            // 004B8583..858C [esp+34h]
        Scoped joined; s_concat(path.p(), joined.adopt(), sep.p());                            // 004B8591..85A7 [esp+14h]
        s_assign(path.p(), joined.p());                                                        // 004B85AC..85B9
    }                                                                                          // 004B85BE..85DB
    Scoped nfo_path;                                                                           // [esp+ACh]
    {
        Scoped nfo; nfo.text("savenfo");                                                       // 004B85E0..85EC [esp+A4h]
        s_concat(path.p(), nfo_path.adopt(), nfo.p());                                         // 004B85F1..860D
    }                                                                                          // 004B8612..8621
    void* gff = new_gff();                                                                     // 004B8626..865A [esp+44h]
    GffStruct root;                                                                            // [esp+10h]
    {
        Scoped version; version.text(k_v20_00745338);                                          // 004B865E..866F [esp+14h]
        Scoped type; type.text("NFO ");                                                        // 004B8674..8685 [esp+34h]
        (void)recovered_gff_00411260(gff, nullptr, &root, type.p(), version.p());              // 004B868A..86A3
    }                                                                                          // 004B86A8..86C5
    Scoped area_name; area_name.empty();                                                       // [esp+7Ch] 004B86CA..86CE
    I strref = 0;                                                                              // [esp+90h]
    U have_name;
    {
        Scoped column; column.text("AreaName");                                                // 004B86D3..86E4 [esp+14h]
        have_name = kotor_fastcall_00414110(modules, nullptr, module, column.p(), &strref);     // 004B86E9..8702
    }                                                                                          // 004B8707..8717
    if (have_name != 0) {                                                                      // 004B871C..8722
        Scoped tlk_text;                                                                       // [esp+14h]
        void* text = static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08)->simple_0041e8f0(
            static_cast<kotor_menu_10_19::String*>(tlk_text.adopt()), static_cast<unsigned>(strref)); // 004B8724..8737
        s_assign(area_name.p(), text);                                                         // 004B873C..874C
    }                                                                                          // 004B8751..875D
    Scoped screenshot; screenshot.empty();                                                     // [esp+50h] 004B8762..8766
    U have_bmp;
    {
        Scoped column; column.text("BMPResRef");                                               // 004B876B..877C [esp+14h]
        void* loadscreens = at<void*>(at<void*>(kotor_cell_rules_007a3a28, 0xb8), 0x18);        // 004B8781..878D
        have_bmp = kotor_m15_c2da_get_string_entry_00413de0(loadscreens, nullptr, module, column.p(), screenshot.p()); // 004B8790..87A5
    }                                                                                          // 004B87AA..87C0 (flag at [esp+27h])
    if (have_bmp == 0) {                                                                       // 004B87C5..87CB
        frame_LogFormat_005e5680(screenshot.p(), k_load_fmt_00745580, at<U>(module, 0), at<U>(module, 4)); // 004B87D1..87E2 (by value)
        menu_resources::Ref ref;                                                               // [esp+68h]
        (void)async_ref_construct_string_00406d60(&ref, nullptr, screenshot.p());             // 004B87EA..87F3
        if (static_cast<menu_resources::Manager*>(resman())->exists_00408bc0(&ref, 3, nullptr) == 0) { // 004B87F8..880E
            (void)async_ref_construct_string_00406d60(&ref, nullptr, screenshot.p());         // 004B8814..881D
            if (static_cast<menu_resources::Manager*>(resman())->exists_00408bc0(&ref, 0xbbf, nullptr) == 0) { // 004B8822..883B
                Scoped column; column.text("BMPResRef");                                       // 004B883D..8846 [esp+34h]
                Scoped row; row.text(k_default_0073e304);                                      // 004B884B..885C [esp+14h]
                void* loadscreens = at<void*>(at<void*>(kotor_cell_rules_007a3a28, 0xb8), 0x18); // 004B8861..886C
                (void)kotor_m15_c2da_get_string_entry_00413de0(loadscreens, nullptr, row.p(), column.p(), screenshot.p()); // 004B886F..8888
            }                                                                                  // 004B888D..88AA
        }
    }
    (void)field_string(gff, &root, area_name.p(), "AREANAME");                                 // 004B88AF..88C3
    (void)field_string(gff, &root, module, "LASTMODULE");                                      // 004B88C8..88D5
    (void)field_dword(gff, &root, kotor_fastcall_00563bf0(static_cast<unsigned char*>(self) + 0x1b770, nullptr), "TIMEPLAYED"); // 004B88DA..88F6
    (void)field_byte(gff, &root, at<U>(self, 0x1b904), "CHEATUSED");                           // 004B88FB..8912
    (void)field_byte(gff, &root, 1, "PCAUTOSAVE");                                             // 004B8917..8925
    (void)field_string(gff, &root, screenshot.p(), "SCREENSHOT");                              // 004B892A..893B
    (void)field_byte(gff, &root, kotor_fastcall_005ee050(client(), nullptr), "GAMEPLAYHINT");  // 004B8940..895B
    (void)field_byte(gff, &root, kotor_fastcall_005ee070(client(), nullptr), "STORYHINT");     // 004B8960..897B
    alignas(4) unsigned char params[0x70];                                                     // [esp+C8h] KOTOR_AUTOSAVE_PARAMS
    (void)kotor_fastcall_004b2840(params, nullptr);                                            // 004B8980..8987
    struct ParamsGuard { unsigned char* p; ~ParamsGuard() { kotor_fastcall_004b2230(p, nullptr); } } params_guard{params};
    {
        Scoped music;                                                                          // [esp+14h]
        (void)kotor_fastcall_005edd00(client(), nullptr, music.adopt(), module);               // 004B898C..89A2
        s_assign(params, music.p());                                                           // 004B89A7..89B7
    }                                                                                          // 004B89BC..89C8
    struct Movies { U* data; I count; I allocated; } movies = {nullptr, 0, 0};                 // [esp+68h]
    (void)kotor_fastcall_004b71e0(&movies, nullptr, kotor_fastcall_005edb60(client(), nullptr)); // 004B89CD..89DF
    struct MoviesGuard {
        Movies* m;
        ~MoviesGuard() {
            if (m->data != nullptr) {                                                          // 004B919B..91A9
                const I n = static_cast<I>(m->data[-1]);
                for (I k = n - 1; k >= 0; --k) s_destroy(m->data + 2 * k);                     // 004B91AB..91BA eh_vector_destructor_iterator
                live_free(m->data - 1);                                                        // 004B91BF..91C5
            }
        }
    } movies_guard{&movies};
    const I movie_count = movies.count;                                                        // 004B89E4 esi
    for (I k = 0; k < 6; ++k) {                                                                // 004B89E8..8C1C (unrolled)
        Scoped fallback;                                                                       // [esp+14h], flag bit k of [esp+0Ch]
        void* source;
        if (movie_count > k) {
            source = movies.data + 2 * k;                                                      // edi + 8k
        } else {
            source = fallback.text(k_empty_0073d71c);                                          // 004B89FC..8A16 etc.
            temp_flags |= 1u << k;
        }
        s_assign(params + 0x10 + static_cast<U>(k) * 8, source);                               // 004B8A1E..8A26 etc.
        if ((temp_flags & (1u << k)) != 0) {                                                   // 004B8A2B..8A4C etc.
            if (k != 5) temp_flags &= ~(1u << k);                                              // (the last flag is not cleared)
            fallback.end();
        }
    }
    U day = 0, time = 0;                                                                       // [esp+98h], [esp+5Ch]
    frame_GetWorldTime_004ade40(kotor_fastcall_004aede0(server_app(), nullptr), nullptr, &day, &time); // 004B8C1C..8C39
    alignas(4) unsigned char t58[4] = {}, t78[4] = {}, t94[4] = {}, ta0[4] = {};               // [esp+58h], [esp+78h], [esp+94h], [esp+A0h]
    recovered_timer_time_of_day_004add90(kotor_fastcall_004aede0(server_app(), nullptr), nullptr, time, t58, ta0, t94, t78); // 004B8C3E..8C6D
    alignas(4) unsigned char t9c[4] = {}, t84[4] = {}, t88[4] = {}, t4f[4] = {}, tc4[4] = {};  // [esp+9Ch], [84h], [88h], [4Fh], [C4h]
    recovered_module_time_004c4100(recovered_server_get_module_004b14f0(self, nullptr), nullptr, t9c, t84, t88, t58, t4f, tc4); // 004B8C72..8CA7
    std::memcpy(params + 0x40, t9c, 4);                                                        // 004B8CAC..8CC1 TIME_YEAR
    params[0x45] = t88[0];                                                                     // 004B8CB3..8CCC
    params[0x44] = t84[0];                                                                     // 004B8CBA..8CDB
    params[0x46] = t58[0];                                                                     // 004B8CC8..8CEA
    std::memcpy(params + 0x4a, t94, 2);                                                        // 004B8CD3..8CF6
    std::memcpy(params + 0x48, ta0, 2);                                                        // 004B8CE2..8D02
    std::memcpy(params + 0x4c, t78, 2);                                                        // 004B8CF1..8D11
    std::memcpy(params + 0x54, &time, 4);                                                      // 004B8CFE..8D1E
    std::memcpy(params + 0x50, &day, 4);                                                       // 004B8D0A..8D29
    {
        Scoped waypoint;                                                                       // [esp+14h]
        (void)kotor_fastcall_004aed40(server_app(), nullptr, waypoint.adopt());                // 004B8D25..8D34
        s_assign(params + 8, waypoint.p());                                                    // 004B8D39..8D49
    }                                                                                          // 004B8D4E..8D5A
    unsigned char summary[0x18];                                                               // [esp+138h] (left as is when there is none)
    kotor_fastcall_0062f0a0(frame_R4_005ed690(client(), nullptr), nullptr, summary);           // 004B8D5F..8D76
    std::memcpy(params + 0x58, summary, 0x18);                                                 // 004B8D7B..8D90
    GffStruct autosave;                                                                        // [esp+8Ch]
    if (recovered_gff_004125b0(gff, nullptr, &autosave, &root, "AUTOSAVEPARAMS", 0) != 0)       // 004B8D8E..8DB1
        kotor_fastcall_004b28e0(params, nullptr, gff, &autosave);                              // 004B8DB3..8DC3
    progress(percent(step, 20));                                                               // 004B8DC8..8DEA
    draw_and_present();                                                                        // 004B8DEF..8E12
    void* party = kotor_fastcall_005ed8b0(client(), nullptr);                                  // 004B8E17..8E28 [esp+0Ch]
    Scoped portrait; portrait.empty();                                                         // [esp+60h] 004B8E2A..8E32
    for (I i = 0; i < at<I>(party, 0); ++i) {                                                  // 004B8E37..8ED4
        void* creature = kotor_m15a_creature_get_server_creature_0060fb20(
            frame_GetCharacter_006346c0(party, nullptr, static_cast<U>(i)), nullptr);          // 004B8E50..8E65 [esp+28h]
        if (creature == nullptr) continue;                                                     // 004B8E5F..8E65
        frame_LogFormat_005e5680(portrait.p(), "%s%d", "PORTRAIT", i);                         // 004B8E67..8E77
        void* client_creature = recovered_creature_client_creature_004efb20(creature, nullptr); // 004B8E7C..8E83
        const char* label = WindowCStr005e5670(portrait.p(), nullptr);                         // 004B8E88..8E8E
        alignas(4) unsigned char resref[16];                                                   // [esp+14h] / [esp+34h]
        void* ref;
        if (client_creature != nullptr)                                                        // 004B8E93..8E96
            ref = reinterpret_cast<void*(__thiscall*)(void*, void*, U)>(at<void*>(at<void*>(client_creature, 0), 0x148))(
                client_creature, resref, 1);                                                   // 004B8E98..8EA3 GetPortraitResRef
        else
            ref = reinterpret_cast<void*(__thiscall*)(void*, void*)>(at<void*>(at<void*>(creature, 0), 0xc8))(creature, resref); // 004B8EAB..8EB6
        (void)recovered_gff_004128c0(gff, nullptr, &root, ref, label);                         // 004B8EBC..8EC6
    }
    progress(percent(step, 25));                                                               // 004B8EDA..8EFC
    draw_and_present();                                                                        // 004B8F01..8F24
    (void)recovered_gff_00413030(gff, nullptr, nfo_path.p(), 0);                               // 004B8F29..8F3C
    gff_delete(gff);                                                                           // 004B8F41..8F4B
    void* erf_memory = live_operator_new(0xd0);                                                // 004B8F4D..8F57
    void* erf = nullptr;
    if (erf_memory != nullptr) {                                                               // 004B8F5E..8F68
        try { erf = cerf_construct_cerf_file_005dd9c0(erf_memory); } catch (...) { live_free(erf_memory); throw; } // 004B8F6A..8F6C
    }
    Scoped save_path;                                                                          // [esp+B4h]
    {
        Scoped name; name.text("SAVEGAME");                                                    // 004B8F77..8F88 [esp+14h]
        s_concat(path.p(), save_path.adopt(), name.p());                                       // 004B8F8D..8FA6
    }                                                                                          // 004B8FAB..8FB7
    (void)kotor_fastcall_005dcfc0(erf, nullptr, save_path.p());                                // 004B8FBC..8FC6
    kotor_fastcall_005dce30(erf, nullptr, "MOD V1.0");                                         // 004B8FCB..8FD2
    (void)kotor_fastcall_005dd080(erf, nullptr);                                               // 004B8FD7..8FD9
    (void)kotor_fastcall_005dd5a0(erf, nullptr);                                               // 004B8FDE..8FE0
    const U base = percent(step, 0x1e);                                                        // 004B8FE5..9004 edi
    progress(base);                                                                            // 004B9006..9009
    kotor_cdecl_00401c10(k_frame_dt_3d088889, 0, 0);                                           // 004B900E..901C
    {
        Scoped in_progress; in_progress.text("GAMEINPROGRESS:");                               // 004B901F..9028 [esp+34h]
        kotor_fastcall_005de1a0(erf, nullptr, in_progress.p(), 0xffffffffu, reinterpret_cast<void*>(&kotor_cdecl_004b30d0),
                                base, percent(step, 0x3c));                                    // 004B902D..905B
    }                                                                                          // 004B9060..906C
    (void)kotor_fastcall_005dd0e0(erf, nullptr);                                               // 004B9071..9073
    if (erf != nullptr) {                                                                      // 004B9078..907A
        cerf_destroy_cerf_file_005de3a0(erf);                                                  // 004B907C..907E
        live_free(erf);                                                                        // 004B9083..9089
    }
    Scoped pifo_path;                                                                          // [esp+BCh]
    {
        Scoped pifo; pifo.text("pifo");                                                        // 004B908C..9095 [esp+34h]
        s_concat(path.p(), pifo_path.adopt(), pifo.p());                                       // 004B909A..90B3
    }                                                                                          // 004B90B8..90C4
    {
        Scoped temp_pifo; temp_pifo.text("TEMP:pifo");                                         // 004B90C9..90D2 [esp+44h]
        (void)kotor_fastcall_00406990(resman(), nullptr, temp_pifo.p(), pifo_path.p(), 0x7de, 0xffff); // 004B90D7..90FC
    }                                                                                          // 004B9101..910D
    progress(percent(step, 100));                                                              // 004B9112..9132
    draw_and_present();                                                                        // 004B9137..915F
}   // 004B9162..9221: ~pifo_path, ~save_path, ~portrait, movies[], ~params, ~screenshot, ~area_name, ~nfo_path, ~path
}  // extern "C"
}  // namespace w1c_autosave
