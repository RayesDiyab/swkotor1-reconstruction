// claude_loadgame_gui.cpp (lane worker loadgame, fork "loadgame-gui"): the client side of loading a saved game from
// the CSWGuiSaveLoad panel (in-game Options -> Load Game, and the main-menu load panel), transcribed from the original
// swkotor.exe (read-only):
//   006CB0E0 CSWGuiSaveLoad::LoadGame(CSWGuiControl* entry)            thiscall RET 4     kotor_fastcall_006cb0e0
//   006CAAF0 CSWGuiSaveLoad::UnpackGame(int number, CExoString const& name, CSWGuiSaveLoadEntry*) RET 0Ch, EAX 0/1
//                                                                                         kotor_fastcall_006caaf0
//   006C9A90 CSWGuiSaveLoad::CopyGameToFutureGame(int number, CExoString const& name) RET 8, EAX 1
//                                                                                         kotor_fastcall_006c9a90
//   006CA250 CSWGuiSaveLoad::LoadPCAutoSave(CExoString const& module)  RET 4             kotor_fastcall_006ca250
//   006C9DE0 KOTOR_AUTOSAVE_PARAMS::LoadFromGFF(CResGFF*, CResStruct*) RET 8             kotor_fastcall_006c9de0
//   006C8490 CStatusSummary::LoadFromGFF(CResGFF*, CResStruct*)        RET 8             kotor_fastcall_006c8490
//   0062F040 CGuiInGame::SetStatusSummary(CStatusSummary const&)       RET 4             kotor_fastcall_0062f040
//   005EDFC0 CClientExoApp::SetLoadingSaveGame(int)                    RET 4             kotor_fastcall_005edfc0
//   005EDD50 CClientExoApp::SetLoadFromIFO(int)                        RET 4             kotor_fastcall_005edd50
//   005EDAA0 CClientExoApp::SetPlayerCharacterName(CExoString const&)  RET 4 (thunk)     kotor_fastcall_005edaa0
//   005F2B00 CClientExoAppInternal::SetPlayerCharacterName(CExoString const&) RET 4      kotor_fastcall_005f2b00
//   004AED00 CServerExoApp::SetLoadingModule(int)                      RET 4             kotor_fastcall_004aed00
//   004AF090 CServerExoApp::SetDoingStartNewModule(int)                RET 4             kotor_fastcall_004af090
//   004AF0C0 CServerExoApp::SetMoveToPausedTimeOfDay(ulong)            RET 4             kotor_fastcall_004af0c0
//   004AF0A0 CServerExoApp::SetMoveToPausedCalendarDay(ulong)          RET 4             kotor_fastcall_004af0a0
//   004AEE60 CServerExoApp::GetGlobalVariableTable()                   RET 0             kotor_fastcall_004aee60
//   006FE5C6 _rename(char const* old, char const* new)                 cdecl             kotor_cdecl_006fe5c6
//   006FFE8A _dosmaperr(ulong oserror)                                 cdecl             kotor_cdecl_006ffe8a
// The server half (CServerExoAppInternal::LoadGame 004BA640, CSWPartyTable::LoadTableInfo 00565D20,
// CSWGlobalVariableTable::Load 0052ADE0) belongs to the loadgame parent lane (claude_loadgame*.cpp).
// Conventions (W5): thiscall -> extern "C" __fastcall(self, unused_edx, stack args...); game heap only. No callee on
// these paths throws (operator new 006FA7E6 returns NULL), so the original EH states reduce to destructor order.
// Proven constants: [0074C5EC]=0 (sound mode), byte [0074C540]=10h (StartConnectToSession mode), 3D088889h = 1/30 s.
// Quirks kept: LoadPCAutoSave never deletes its CERFFile (leak) and passes SetMoveToWorldTime the dword reads at
// +40/+44/+45/+46/+48/+4A/+4C of KOTOR_AUTOSAVE_PARAMS (overlapping, upper bytes are the neighbouring fields; the
// structure's two padding bytes +47 / +4E..4F are zero here, stack garbage in the original); null allocations are
// used unchecked exactly where the original does (CERFFile Read on NULL); the live-content message uses TLK strref 0
// as the original; [00832CA8] (never written by any original instruction) gates the live-content prompt.
#include <cstdint>
#include <cstring>

namespace {
using U = std::uint32_t;
using I = std::int32_t;
using U8 = std::uint8_t;
using U16 = std::uint16_t;
inline void* ptr(U a) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a)); }
inline U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
template <class T> T& at(void* object, U offset) { return *reinterpret_cast<T*>(static_cast<unsigned char*>(object) + offset); }
template <class T> T& at(U object, U offset) { return at<T>(ptr(object), offset); }
inline U8* bytes(void* p) { return static_cast<U8*>(p); }
constexpr U k_sound_mode_0074c5ec = 0;
constexpr U k_connect_mode_0074c540 = 0x10;
constexpr U k_frame_dt_3d088889 = 0x3d088889u;
}

namespace kotor_reconstruction { struct StartupString; }
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }                    // [007A39FC] CAppManager*
namespace frame_control { extern unsigned resource_manager_007a39e8; }                          // [007A39E8] CExoResMan*
namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } }           // [007A39E0] CExoBase*
namespace kotor_menu_10_19 { struct String; struct Tlk { String* simple_0041e8f0(String* out, unsigned id); }; } // GetSimpleString [0037]
namespace tlk_core { void __fastcall ParseStr0041dd30(kotor_menu_10_19::Tlk*, void*, kotor_reconstruction::StartupString*); } // [0389]
namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); } // CExoString() [menu20]
kotor_reconstruction::StartupString* __fastcall GuiConcat005e5d10(kotor_reconstruction::StartupString* self, void*,
    kotor_reconstruction::StartupString* out, const kotor_reconstruction::StartupString* other);  // operator+ [0093]
namespace base_ini_reconstruction {
kotor_reconstruction::StartupString* __fastcall left_005e5f70(kotor_reconstruction::StartupString*, void*,
                                                               kotor_reconstruction::StartupString*, int); // Left [0126]
}
namespace alias_path { struct AliasListView; }
namespace path_resolution {
kotor_reconstruction::StartupString* __fastcall resolve_005e68a0(alias_path::AliasListView*, void*,
    kotor_reconstruction::StartupString*, kotor_reconstruction::StartupString*, unsigned);     // ResolveFileName [0241]
}
namespace tlk_file { struct File; }
namespace file_internal {
tlk_file::File* __fastcall exofile_construct_005e68d0(tlk_file::File*, void*, kotor_reconstruction::StartupString*, unsigned,
                                                     kotor_reconstruction::StartupString*);    // CExoFile::CExoFile [0230]
}
namespace options_video { struct String; }
namespace client_movies { int __fastcall StringNotEqual_005e5390(options_video::String*, void*, const char*); } // != [0374]
namespace kotor_game_prepare { void __fastcall SetGlobalUnpauseableState0062b410(void*, void*, unsigned); } // [0461]
#ifndef W1R_MENU_RESOURCES_REF
#define W1R_MENU_RESOURCES_REF
namespace menu_resources { struct Ref { unsigned char bytes[16]; Ref* __thiscall construct_00406d80(const char* text); }; } // [0030]
#endif
namespace menu_resources { struct Manager { unsigned directory_00408800(kotor_reconstruction::StartupString* path); }; } // AddResourceDirectory [0027]

extern "C" {
// ---------------- existing providers (c40 map, exact decorated arity) ----------------
void* __cdecl live_operator_new(unsigned);                                                     // 006FA7E6
void __cdecl live_free(void*);                                                                 // 006FA390
extern void* kotor_cell_tlk_007a3a08;                                                          // [007A3A08] CTlkTable*
extern void* kotor_cell_sound_007a39ec;                                                        // [007A39EC] CExoSound*
extern unsigned kotor_cell_00832ca8;                                                           // [00832CA8] (.bss, never written)
void* __cdecl compat_getptd();                                                                 // 006FEEDF _getptd [0564]
__declspec(dllimport) int __stdcall MoveFileA(const char*, const char*);                       // [0073D130]
__declspec(dllimport) unsigned long __stdcall GetLastError();                                  // [0073D208]
void* __fastcall frame_LogStringCtor_005e5a90(void* s, void*, const char* text);              // CExoString(char*)
void __fastcall frame_LogStringDtor_005e5c20(void* s, void*);                                 // ~CExoString
void* __fastcall async_assign_string_005e5c50(void* self, void*, void* other);                // operator=
void* __fastcall frame_LogAssign_005e5140(void* self, void*, const char* text);               // operator=(char*)
void* __fastcall WindowCopy005e5b00(void* self, void*, void* other);                          // CExoString(CExoString const&)
void __cdecl frame_LogFormat_005e5680(void* self, const char* format, ...);                   // CExoString::Format
const char* __fastcall WindowCStr005e5670(void*, void*);                                      // CStr
U __fastcall async_string_length_005e5790(void* string, void*);                               // GetLength
void __fastcall frame_R4_005d5e80(void* sound, void*, U mode);                                // CExoSoundInternal::SetSoundMode [0709]
void __fastcall kotor_fastcall_005ee120(void* client, void*);                                 // BusyWaitMenuMusic [0794]
void __fastcall load_screen_busy_wait_mute_sound_005ee130(void* client, void*);               // BusyWaitMuteSound
void* __fastcall frame_R4_005ed690(void* client, void*);                                      // GetInGameGui [0457]
void __fastcall kotor_fastcall_00627130(void* box, void*, U allow);                           // CSWGuiMessageBox::SetAllowCancel [0596]
void __fastcall gui_load_messagebox_set_callback_00624a40(void* box, void*, U target, U handler, U param); // SetCallback [0298]
void __fastcall kotor_fastcall_0041db50(void* tlk, void*, U token, void* text);               // CTlkTable::SetCustomToken [0583]
void __fastcall kotor_fastcall_0040bc70(void* manager, void*, void* panel, U a, U b);         // CSWGuiManager::AddPanel [0298]
void __cdecl kotor_m15_party_reset_session_start_time_00563cf0();                              // CSWPartyTable::ResetCurrentSessionStartTime
void* __fastcall kotor_fastcall_0040be00(void* manager, void*);                               // CSWGuiManager::PopModalPanel [0458]
void __fastcall frame_R4_0062cba0(void* gui, void*, U flag);                                  // CGuiInGame::HideSWInGameGui [1205-30]
void __fastcall kotor_m15_client_set_load_screen_by_module_005edcf0(void* client, void*, void* module); // [0813]
void __fastcall load_screen_set_load_music_005edd30(void* client, void*, void* music);        // SetLoadMusic
void __fastcall kotor_m15_client_show_load_screen_005eda80(void* client, void*, U a, U b, U c); // ShowLoadScreen [0813]
void __fastcall kotor_m15_client_set_load_bar_progress_005edd40(void* client, void*, U value, U a); // SetLoadBarProgress [0813]
void __fastcall kotor_m15_client_hide_load_screen_005eda90(void* client, void*, U a, U b);   // HideLoadScreen [0813]
void __cdecl kotor_cdecl_00401c10(U dt_bits, U a, U b);                                        // UpdateScreen [0785]
void __fastcall frame_R4_00401280(void* app, void*);                                          // CAppManager::DestroyServer [0398]
void __fastcall recovered_client_unload_module_005ed430(void* client, void*);                 // CClientExoApp::UnloadModule [w3_traps]
U __fastcall recovered_create_server_00401380(void* app, void*, U flag);                       // CAppManager::CreateServer [0845]
void* __fastcall frame_GetWorldTimer_005ed4f0(void* client, void*);                           // CClientExoApp::GetWorldTimer [0457]
void* __fastcall kotor_fastcall_004aede0(void* server, void*);                                // CServerExoApp::GetWorldTimer [0457]
void __fastcall kotor_fastcall_004adff0(void* timer, void*);                                  // CWorldTimer::PauseWorldTimer [0453]
void* __fastcall recovered_support_client_name_005ed490(void* client, void*, void* out);      // GetClientName [0841]
U __fastcall recovered_support_net_start_005d5040(void* net, void*, U first, void* name, void* password, U fourth, U mode); // StartConnectToSession [0841]
void* __fastcall recovered_support_client_message_005ed6f0(void* client, void*);              // GetSWCMessage [0841]
U __fastcall recovered_support_request_status_006775e0(void* message, void*);                  // SendPlayerToServerStatus_RequestStatus [0841]
void __fastcall kotor_fastcall_005eda60(void* client, void*, U a, U b);                       // SetInputClass [0719]
void __fastcall disable_input_005edbe0(void* client, void*);                                  // DisableInput [r7]
void __fastcall load_screen_set_gameplay_hint_005ee060(void* client, void*, U hint);          // SetGamePlayHint
void __fastcall load_screen_set_story_hint_005ee080(void* client, void*, U hint);             // SetStoryHint
U __fastcall load_screen_get_next_load_screen_hint_strref_005ee090(void* client, void*, void* module, U a); // [adapters]
void __fastcall kotor_m15_client_set_load_screen_hint_005edf00(void* client, void*, U strref); // [0813]
void __fastcall load_screen_set_load_screen_hint_update_timer_005ee040(void* client, void*, U value); // [thin]
void __fastcall load_screen_set_load_from_save_game_004af060(void* server, void*, U value);   // CServerExoApp::SetLoadFromSaveGame
void __fastcall load_screen_send_load_game_request_005ed680(void* client, void*, U number, void* dir, void* module); // SendLoadGameRequest
U __fastcall gui_load_resman_nuke_directory_00409480(void* resman, void*, void* path, U a, U b); // NukeDirectory [0395]
U __fastcall gui_load_resman_create_directory_004068c0(void* resman, void*, void* path);      // CreateDirectory2 [0395]
U __fastcall gui_load_resman_clean_directory_00409460(void* resman, void*, void* path, U a, U b); // CleanDirectory [0298]
void __fastcall kotor_fastcall_004088d0(void* resman, void*, void* path);                     // RemoveResourceDirectory [0576]
U __fastcall kotor_fastcall_00406990(void* resman, void*, void* source, void* target, U source_type, U target_type); // FileCopy
void* __fastcall cerf_construct_cerf_file_005dd9c0(void* self);                               // CERFFile::CERFFile
void __fastcall cerf_destroy_cerf_file_005de3a0(void* self);                                  // CERFFile::~CERFFile
U __fastcall cerf_read_cerf_file_005dce50(void* self, void*, void* path);                     // CERFFile::Read
U __fastcall cerf_read_header_variance_005dd3c0(void* self, void*, void* path);               // CERFFile::ReadHeaderVariance
void __fastcall cerf_export_files_from_erf_005dd710(void* self, void*, void* path, void* out, int flags, void* callback,
                                                    int ctx1, int ctx2);                       // CERFFile::ExportFilesFromERF
int __cdecl cerf_load_screen_update_006c8750(int, int, int, int);                              // LoadScreenUpdate callback
U __fastcall kotor_fastcall_005edff0(void* client, void*, U a);                               // GetLoadStep (AL) [1205-20]
void __fastcall kotor_fastcall_005edfd0(void* client, void*, U step, U value);                // SetLoadStep [0402]
void __fastcall frame_LogWriteText_005e6a40(void* file, void*, const char* text);             // CExoFile::Write(char*) [0459]
void __fastcall cerf_c_exofile_flush_005e6a20(void* file);                                    // CExoFile::Flush
void __fastcall file_destroy_005e69e0(void* file, void*);                                     // ~CExoFile [0224]
void __fastcall frame_R4_004ae8a0(void* server, void*);                                       // CServerExoApp::UnloadModule [0457]
void* __fastcall kotor_fastcall_004aee70(void* server, void*);                                // GetPartyTable [vmD_b]
void* __fastcall gui_load_gff_construct_00410630(void* gff, void*, U type, const char* ext, void* ref); // CResGFF::CResGFF [0289]
void __fastcall kotor_fastcall_00411240(void* gff, void*, void* out);                         // GetTopLevelStruct
U __fastcall gui_load_gff_struct_00411a10(void* gff, void*, void* out, void* parent, const char* label); // GetStructFromStruct
unsigned char __fastcall gui_load_gff_byte_00411a60(void* gff, void*, void* st, const char* label, U* ok, unsigned char def);
unsigned short __fastcall recovered_gff_word_00411b40(void* gff, void*, void* st, const char* label, U* ok, unsigned short def);
U __fastcall gui_load_gff_dword_00411c20(void* gff, void*, void* st, const char* label, U* ok, U def);
int __fastcall gui_load_gff_int_00411c90(void* gff, void*, void* st, const char* label, U* ok, int def);
void* __fastcall gui_load_gff_string_00411ec0(void* gff, void*, void* out, void* st, const char* label, U* ok, void* def);
void* __fastcall kotor_fastcall_004b2840(void* self, void*);                                  // KOTOR_AUTOSAVE_PARAMS() [w1_chains]
void __fastcall kotor_fastcall_004b2230(void* self, void*);                                   // ~KOTOR_AUTOSAVE_PARAMS [w1_chains]
void __fastcall recovered_server_set_move_to_module_start_waypoint_004aed30(void* server, void*, void* tag); // [exit_queue]
void __fastcall kotor_fastcall_004aebc0(void* server, void*, U, U, U, U, U, U, U);            // SetMoveToWorldTime [0886 bodies]
void __fastcall recovered_client_add_movie_005edb50(void* client, void*, void* movie);        // AddMoveToModuleMovie [r3]
void __fastcall kotor_fastcall_004ba820(void* internal, void*, void* module);                 // internal ExecuteLoadModule [route_bodies]
// ---------------- parent loadgame lane (claude_loadgame*.cpp) ----------------
void __fastcall kotor_fastcall_00565d20(void* table, void*);                                  // CSWPartyTable::LoadTableInfo RET 0
void __fastcall kotor_fastcall_0052ade0(void* table, void*);                                  // CSWGlobalVariableTable::Load RET 0

// ---------------- bodies defined here ----------------
void __fastcall kotor_fastcall_006cb0e0(void* self, void*, void* entry);
U __fastcall kotor_fastcall_006caaf0(void* self, void*, I number, void* name, void* entry);
U __fastcall kotor_fastcall_006c9a90(void* self, void*, I number, void* name);
void __fastcall kotor_fastcall_006ca250(void* self, void*, void* module);
void __fastcall kotor_fastcall_006c9de0(void* self, void*, void* gff, void* st);
void __fastcall kotor_fastcall_006c8490(void* self, void*, void* gff, void* st);
void __fastcall kotor_fastcall_0062f040(void* gui, void*, const void* summary);
void __fastcall kotor_fastcall_005edfc0(void* client, void*, U value);
void __fastcall kotor_fastcall_005edd50(void* client, void*, U value);
void __fastcall kotor_fastcall_005edaa0(void* client, void*, void* name);
void __fastcall kotor_fastcall_005f2b00(void* internal, void*, void* name);
void __fastcall kotor_fastcall_004aed00(void* server, void*, U value);
void __fastcall kotor_fastcall_004af090(void* server, void*, U value);
void __fastcall kotor_fastcall_004af0c0(void* server, void*, U value);
void __fastcall kotor_fastcall_004af0a0(void* server, void*, U value);
void* __fastcall kotor_fastcall_004aee60(void* server, void*);
int __cdecl kotor_cdecl_006fe5c6(const char* old_name, const char* new_name);
void __cdecl kotor_cdecl_006ffe8a(unsigned long oserror);
}

namespace {
using Str = kotor_reconstruction::StartupString;
inline Str* str(void* p) { return static_cast<Str*>(p); }
// One CExoString destroyed when its scope ends (the original EH states only destroy CExoString locals).
struct Scoped {
    alignas(4) unsigned char b[8]; bool live = false;
    void* p() { return b; }
    void* text(const char* t) { (void)frame_LogStringCtor_005e5a90(b, nullptr, t); live = true; return b; }
    void* empty() { (void)kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(b), nullptr); live = true; return b; }
    void* copy(void* o) { (void)WindowCopy005e5b00(b, nullptr, o); live = true; return b; }
    void* adopt() { live = true; return b; }                                                 // constructed by a callee
    void end() { if (live) { live = false; frame_LogStringDtor_005e5c20(b, nullptr); } }
    ~Scoped() { end(); }
};
inline void* app() { return const_cast<void*>(kotor_menu_window_cells::manager_007a39fc); }
inline void* client() { return at<void*>(app(), 4); }
inline void* server() { return at<void*>(app(), 8); }
inline void* resman() { return ptr(frame_control::resource_manager_007a39e8); }
inline alias_path::AliasListView* alias_list() { return at<alias_path::AliasListView*>(kotor_menu_10_19::dependencies::base_007a39e0, 0xc); }
inline kotor_menu_10_19::Tlk* tlk() { return static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08); }
inline void s_assign(void* s, void* o) { (void)async_assign_string_005e5c50(s, nullptr, o); }
inline void s_concat(void* self, void* out, void* other) { (void)GuiConcat005e5d10(str(self), nullptr, str(out), str(other)); }
inline const char* cstr(void* s) { return WindowCStr005e5670(s, nullptr); }
inline void progress(U value) { kotor_m15_client_set_load_bar_progress_005edd40(client(), nullptr, value, 1); }
inline void present() { kotor_cdecl_00401c10(k_frame_dt_3d088889, 0, 0); }
inline void pause_world_timers() {
    kotor_fastcall_004adff0(frame_GetWorldTimer_005ed4f0(client(), nullptr), nullptr);
    kotor_fastcall_004adff0(kotor_fastcall_004aede0(server(), nullptr), nullptr);
}
// GetClientName(&name) -> GetNetLayer() (vslot +10h) -> StartConnectToSession(0, name, password "", 0Ah, [0074C540])
void connect_to_local_session(void* c) {
    Scoped password; password.text("");                                                       // [0073D71C]
    Scoped name; void* const n = recovered_support_client_name_005ed490(c, nullptr, name.p()); name.adopt();
    using GetNet = void*(__thiscall*)(void*);
    void* const net = reinterpret_cast<GetNet>(at<void*>(at<void*>(c, 0), 0x10))(c);          // call [eax+10h]
    (void)recovered_support_net_start_005d5040(net, nullptr, 0, n, password.p(), 0xa, k_connect_mode_0074c540);
    name.end();
    password.end();
}
inline void panel_set_loading_flags(void* self) {                                             // [this+44h] &= ~300h; |= 400h
    at<U>(self, 0x44) = (at<U>(self, 0x44) & 0xfffffcffu) | 0x400u;
}
// "SAVES:" + ("%06d - %s" % (number, name) | name)
void save_directory(void* out_saves, void* game, I number, void* name) {
    if (number != -1) frame_LogFormat_005e5680(game, "%06d - %s", number, cstr(name));       // ("%06d - %s" at 007455AC)
    else s_assign(game, name);
    (void)frame_LogAssign_005e5140(out_saves, nullptr, "SAVES:");                             // [00745174]
    Scoped joined; s_concat(out_saves, joined.p(), game); joined.adopt();
    s_assign(out_saves, joined.p());
    joined.end();
}
}  // namespace

extern "C" {
// ===================================================================================== small accessors
void __fastcall kotor_fastcall_005edfc0(void* client_app, void*, U value) { at<U>(at<void*>(client_app, 4), 0x33c) = value; } // 005EDFC0..CD
void __fastcall kotor_fastcall_005edd50(void* client_app, void*, U value) { at<U>(at<void*>(client_app, 4), 0x310) = value; } // 005EDD50..5D
void __fastcall kotor_fastcall_004aed00(void* server_app, void*, U value) { at<U>(at<void*>(server_app, 4), 0x100b4) = value; } // 004AED00..0D
void __fastcall kotor_fastcall_004af090(void* server_app, void*, U value) { at<U>(at<void*>(server_app, 4), 0x1007c) = value; } // 004AF090..9D
void __fastcall kotor_fastcall_004af0c0(void* server_app, void*, U value) { at<U>(at<void*>(server_app, 4), 0x100ac) = value; } // 004AF0C0..CD
void __fastcall kotor_fastcall_004af0a0(void* server_app, void*, U value) { at<U>(at<void*>(server_app, 4), 0x100b0) = value; } // 004AF0A0..AD
void* __fastcall kotor_fastcall_004aee60(void* server_app, void*) { return bytes(at<void*>(server_app, 4)) + 0x100fc; }       // 004AEE60..68
void __fastcall kotor_fastcall_005f2b00(void* internal, void*, void* name) {
    s_assign(bytes(internal) + 0x294, name);                                                   // 005F2B00..0E
    at<U>(internal, 0x290) = 1;                                                                // 005F2B13
}
void __fastcall kotor_fastcall_005edaa0(void* client_app, void*, void* name) { kotor_fastcall_005f2b00(at<void*>(client_app, 4), nullptr, name); } // jmp
// 0062F040 CGuiInGame::SetStatusSummary: copy 18h bytes into [+A8h]+64h, else into [+ACh] (allocated 18h on demand).
void __fastcall kotor_fastcall_0062f040(void* gui, void*, const void* summary) {
    void* const panel = at<void*>(gui, 0xa8);                                                  // 0062F044
    if (panel != nullptr) { std::memcpy(bytes(panel) + 0x64, summary, 0x18); return; }       // 0062F04E..5E
    if (at<void*>(gui, 0xac) == nullptr) at<void*>(gui, 0xac) = live_operator_new(0x18);      // 0062F061..75 (unchecked)
    std::memcpy(at<void*>(gui, 0xac), summary, 0x18);                                         // 0062F07B..8A
}

// ===================================================================================== CRT
// 006FFE8A _dosmaperr: ptd->_tdoserrno = e; ptd->_terrno from errtable 007A2C40 (2Dh pairs), else EACCES for
// 13h..24h, ENOEXEC for BCh..CAh, else EINVAL.
void __cdecl kotor_cdecl_006ffe8a(unsigned long oserror) {
    static const U table_007a2c40[45][2] = {
        {1,22},{2,2},{3,2},{4,24},{5,13},{6,9},{7,12},{8,12},{9,12},{10,7},{11,8},{12,22},{13,22},{15,2},{16,13},
        {17,18},{18,2},{33,13},{53,2},{65,13},{67,2},{80,17},{82,13},{83,13},{87,22},{89,11},{108,13},{109,32},
        {112,28},{114,9},{6,22},{128,10},{129,10},{130,9},{131,22},{132,13},{145,41},{158,13},{161,2},{164,11},
        {167,13},{183,17},{206,2},{215,11},{1816,12}};
    const U e = static_cast<U>(oserror);
    at<U>(compat_getptd(), 0xc) = e;                                                           // 006FFE8B..94
    for (U i = 0; i < 0x2d; ++i) {                                                             // 006FFE97..A6
        if (e == table_007a2c40[i][0]) { at<U>(compat_getptd(), 8) = table_007a2c40[i][1]; return; } // 006FFEC0..D0
    }
    if (e >= 0x13 && e <= 0x24) { at<U>(compat_getptd(), 8) = 0xd; return; }                  // 006FFEA8..BF
    if (e >= 0xbc && e <= 0xca) { at<U>(compat_getptd(), 8) = 8; return; }                    // 006FFED1..EE
    at<U>(compat_getptd(), 8) = 0x16;                                                          // 006FFEEF..FB
}
// 006FE5C6 _rename: MoveFileA(old, new); on failure _dosmaperr(GetLastError()) and -1, else 0.
int __cdecl kotor_cdecl_006fe5c6(const char* old_name, const char* new_name) {
    const unsigned long error = MoveFileA(old_name, new_name) != 0 ? 0 : GetLastError();      // 006FE5C6..E2
    if (error != 0) { kotor_cdecl_006ffe8a(error); return -1; }                                // 006FE5E4..F0
    return 0;                                                                                  // 006FE5F1
}

// ===================================================================================== 006C9A90
U __fastcall kotor_fastcall_006c9a90(void*, void*, I number, void* name) {
    Scoped saves, game, erf_path, s40;
    saves.empty(); game.empty(); erf_path.empty(); s40.empty();                                // 006C9AB0..9AE1 [18h][20h][10h][40h]
    save_directory(saves.p(), game.p(), number, name);                                        // 006C9AE6..9B54
    { Scoped dir; dir.text("FUTUREGAME:");                                                    // 006C9B59..9B84 [007513CC]
      (void)gui_load_resman_create_directory_004068c0(resman(), nullptr, dir.p()); }
    { Scoped dir; dir.text("FUTUREGAME:");                                                    // 006C9B89..9BB8
      (void)gui_load_resman_clean_directory_00409460(resman(), nullptr, dir.p(), 0, 0); }
    {
        Scoped savegame; savegame.text("SAVEGAME");                                           // 006C9BBD..9BC6 [007454C8]
        Scoped backslash; backslash.text("\\");                                               // 006C9BCB..9BD9 [0073D8F4]
        Scoped t1; s_concat(saves.p(), t1.p(), backslash.p()); t1.adopt();                    // 006C9BDE..9BF1
        Scoped t2; s_concat(t1.p(), t2.p(), savegame.p()); t2.adopt();                        // 006C9BF6..9C07
        s_assign(erf_path.p(), t2.p());                                                       // 006C9C0C..9C16
        t2.end(); t1.end(); backslash.end(); savegame.end();                                  // 006C9C1B..9C4D
    }
    void* const memory = live_operator_new(0xd0);                                             // 006C9C52..9C5C
    void* const erf = memory != nullptr ? cerf_construct_cerf_file_005dd9c0(memory) : nullptr; // 006C9C63..9C77
    (void)cerf_read_cerf_file_005dce50(erf, nullptr, erf_path.p());                           // 006C9C79..9C84 (NULL unchecked)
    (void)cerf_read_header_variance_005dd3c0(erf, nullptr, erf_path.p());                     // 006C9C89..9C90
    {
        Scoped out; out.text("FUTUREGAME:");                                                  // 006C9C95..9C9E
        const U step = kotor_fastcall_005edff0(client(), nullptr, 0) & 0xffu;                 // 006C9CA3..9CB8 movzx edx,al
        cerf_export_files_from_erf_005dd710(erf, nullptr, erf_path.p(), out.p(), -1,
            reinterpret_cast<void*>(&cerf_load_screen_update_006c8750), 0, static_cast<int>(step)); // 006C9CBB..9CD1
        out.end();                                                                            // 006C9CD6..9CDE
    }
    if (erf != nullptr) { cerf_destroy_cerf_file_005de3a0(erf); live_free(erf); }             // 006C9CE3..9CF4
    s40.end(); erf_path.end(); game.end(); saves.end();                                       // 006C9CF7..9D2D
    return 1;                                                                                 // 006C9D37
}

// ===================================================================================== 006CAAF0
U __fastcall kotor_fastcall_006caaf0(void* self, void*, I number, void* name, void*) {
    if (kotor_fastcall_006c9a90(self, nullptr, number, name) != 0) {                         // 006CAB17..AB20
        Scoped in_progress, future;                                                           // [18h] / [10h] (built by ResolveFileName)
        { Scoped alias; alias.text("GAMEINPROGRESS:");                                        // 006CAB26..AB65 [0074514C]
          (void)path_resolution::resolve_005e68a0(alias_list(), nullptr, str(in_progress.p()), str(alias.p()), 0xffff);
          in_progress.adopt(); }
        { Scoped alias; alias.text("FUTUREGAME:");                                            // 006CAB6A..ABA7
          (void)path_resolution::resolve_005e68a0(alias_list(), nullptr, str(future.p()), str(alias.p()), 0xffff);
          future.adopt(); }
        { Scoped dir; dir.text("GAMEINPROGRESS:");                                            // 006CABAC..ABDE
          (void)gui_load_resman_nuke_directory_00409480(resman(), nullptr, dir.p(), 1, 1); }
        { Scoped left; (void)base_ini_reconstruction::left_005e5f70(str(future.p()), nullptr, str(left.p()),
              static_cast<int>(async_string_length_005e5790(future.p(), nullptr) - 1)); left.adopt(); // 006CABE3..ABF7
          s_assign(future.p(), left.p()); }                                                   // 006CABFC..AC16
        { Scoped left; (void)base_ini_reconstruction::left_005e5f70(str(in_progress.p()), nullptr, str(left.p()),
              static_cast<int>(async_string_length_005e5790(in_progress.p(), nullptr) - 1)); left.adopt(); // 006CAC1B..AC2F
          s_assign(in_progress.p(), left.p()); }                                              // 006CAC34..AC4E
        const char* const new_name = cstr(in_progress.p());                                   // 006CAC53..AC5C
        const char* const old_name = cstr(future.p());                                        // 006CAC5D..AC66
        (void)kotor_cdecl_006fe5c6(old_name, new_name);                                       // 006CAC67..AC6C rename(future, in progress)
        future.end(); in_progress.end();                                                      // 006CAC6F..AC89
        return 1;                                                                             // 006CAC8E
    }
    // CopyGameToFutureGame failed: mark the save corrupt (SAVES:<game>\CORRUPT).
    Scoped game, saves; game.empty(); saves.empty();                                          // 006CACA7..ACBC [28h] / [20h]
    save_directory(saves.p(), game.p(), number, name);                                        // 006CACC1..AD2E
    kotor_m15_client_hide_load_screen_005eda90(client(), nullptr, 0, 1);                      // 006CAD33..AD3F
    {
        Scoped mode; mode.text("wb+");                                                        // 006CAD44..AD4D [0074A0A4]
        Scoped corrupt; corrupt.text("CORRUPT");                                              // 006CAD52..AD63 [007513EC]
        Scoped backslash; backslash.text("\\");                                               // 006CAD68..AD79
        alignas(4) unsigned char file[4];                                                     // [esp+0Ch] CExoFile
        {
            Scoped t1; s_concat(saves.p(), t1.p(), backslash.p()); t1.adopt();                // 006CAD7E..AD94
            Scoped t2; s_concat(t1.p(), t2.p(), corrupt.p()); t2.adopt();                     // 006CAD99..ADAD
            (void)file_internal::exofile_construct_005e68d0(reinterpret_cast<tlk_file::File*>(file), nullptr,
                                                           str(t2.p()), 0, str(mode.p()));    // 006CADB2..ADC6
            t2.end(); t1.end();                                                               // 006CADCB..ADD9
        }
        backslash.end(); corrupt.end(); mode.end();                                           // 006CADE7..AE0C
        frame_LogWriteText_005e6a40(file, nullptr, "CORRUPT");                                // 006CAE11..AE1A
        cerf_c_exofile_flush_005e6a20(file);                                                  // 006CAE1F..AE23
        file_destroy_005e69e0(file, nullptr);                                                 // 006CAE28..AE30
    }
    saves.end(); game.end();                                                                  // 006CAE35..AE4F
    return 0;                                                                                 // 006CAE5A
}

// ===================================================================================== 006C8490
// CStatusSummary {U flags (bit0 DISPLAYSPENDING, 1 SOUNDPENDING, 2 ITEMRECEIVED, 3 ITEMLOST, 4 JOURNAL, 5 LEVELUPSOUND,
// 6 NEWQUESTSOUND, 7 COMPLETESOUND, 8 CREDITSNET); +4 LIGHTSHIFT; +5 DARKSHIFT; +8 CREDITS; +C XP; +10 STEALTHXP;
// +14 SUPPRESSED}. Every read defaults to the current value.
void __fastcall kotor_fastcall_006c8490(void* self, void*, void* gff, void* st) {
    U ok = 0;                                                                                 // [esp+14h] (argument slot)
    auto flag = [&](const char* label, U bit) {
        const U current = (at<U>(self, 0) >> bit) & 0xffffff01u;
        const U v = gui_load_gff_byte_00411a60(gff, nullptr, st, label, &ok, static_cast<unsigned char>(current));
        at<U>(self, 0) ^= ((v << bit) ^ at<U>(self, 0)) & (1u << bit);                       // movzx/shl/xor/and/xor
    };
    flag("DISPLAYSPENDING", 0);                                                               // 006C8498..84D8
    flag("ITEMRECEIVED", 2);                                                                  // 006C84B9..84F1
    flag("ITEMLOST", 3);                                                                      // 006C84F3..851E
    flag("JOURNAL", 4);                                                                       // 006C8520..854B
    at<U8>(self, 4) = gui_load_gff_byte_00411a60(gff, nullptr, st, "LIGHTSHIFT", &ok, at<U8>(self, 4)); // 006C854D..8565
    at<U8>(self, 5) = gui_load_gff_byte_00411a60(gff, nullptr, st, "DARKSHIFT", &ok, at<U8>(self, 5));  // 006C8568..8580
    at<I>(self, 8) = gui_load_gff_int_00411c90(gff, nullptr, st, "CREDITS", &ok, at<I>(self, 8));       // 006C8583..85A5
    flag("CREDITSNET", 8);                                                                    // 006C8599..85CE
    at<I>(self, 0xc) = gui_load_gff_int_00411c90(gff, nullptr, st, "XP", &ok, at<I>(self, 0xc));        // 006C85D0..85F3
    at<I>(self, 0x10) = gui_load_gff_int_00411c90(gff, nullptr, st, "STEALTHXP", &ok, at<I>(self, 0x10)); // 006C85E2..85FB
    flag("SOUNDPENDING", 1);                                                                  // 006C85FE..8628
    flag("LEVELUPSOUND", 5);                                                                  // 006C862A..8654
    flag("NEWQUESTSOUND", 6);                                                                 // 006C8656..8680
    flag("COMPLETESOUND", 7);                                                                 // 006C8682..86AF
    at<I>(self, 0x14) = gui_load_gff_int_00411c90(gff, nullptr, st, "SUPPRESSED", &ok, at<I>(self, 0x14)); // 006C86B1..86C8
}

// ===================================================================================== 006C9DE0
void __fastcall kotor_fastcall_006c9de0(void* self, void*, void* gff, void* st) {
    U ok = 0;                                                                                 // [esp+14h]
    static const char* const labels[8] = {"LOADMUSIC", "STARTWAYPOINT", "MOVIE1", "MOVIE2", "MOVIE3", "MOVIE4",
                                          "MOVIE5", "MOVIE6"};
    for (U k = 0; k < 8; ++k) {                                                               // 006C9E06..9FC8
        void* const field = bytes(self) + k * 8;
        Scoped read;
        void* const text = gui_load_gff_string_00411ec0(gff, nullptr, read.p(), st, labels[k], &ok, field); // default = field
        read.adopt();
        s_assign(field, text);
        read.end();
    }
    const U year = gui_load_gff_dword_00411c20(gff, nullptr, st, "TIME_YEAR", &ok, 0);        // 006C9FCD..9FE7
    const U8 month = gui_load_gff_byte_00411a60(gff, nullptr, st, "TIME_MONTH", &ok, 0);      // 006C9FE1..9FF9
    const U8 day = gui_load_gff_byte_00411a60(gff, nullptr, st, "TIME_DAY", &ok, 0);          // 006C9FF7..A01E
    const U8 hour = gui_load_gff_byte_00411a60(gff, nullptr, st, "TIME_HOUR", &ok, 0);        // 006CA00F..A036
    const U16 minute = recovered_gff_word_00411b40(gff, nullptr, st, "TIME_MINUTE", &ok, 0);  // 006CA027..A043
    const U16 second = recovered_gff_word_00411b40(gff, nullptr, st, "TIME_SECOND", &ok, 0);  // 006CA03F..A066
    const U16 ms = recovered_gff_word_00411b40(gff, nullptr, st, "TIME_MILLISECOND", &ok, 0); // 006CA057..A06A
    at<U16>(self, 0x4c) = ms; at<U8>(self, 0x44) = month; at<U8>(self, 0x45) = day;          // 006CA079..A088
    at<U8>(self, 0x46) = hour; at<U16>(self, 0x48) = minute; at<U>(self, 0x40) = year;       // 006CA091..A0A5
    at<U16>(self, 0x4a) = second;                                                             // 006CA0A8
    at<U>(self, 0x50) = gui_load_gff_dword_00411c20(gff, nullptr, st, "TIME_PAUSEDAY", &ok, 0);   // 006CA077..A0C0
    at<U>(self, 0x54) = gui_load_gff_dword_00411c20(gff, nullptr, st, "TIME_PAUSETIME", &ok, 0);  // 006CA0B1..A0D5
    U child = 0;                                                                              // [esp+24h] CResStruct
    if (gui_load_gff_struct_00411a10(gff, nullptr, &child, st, "STATUSSUMMARY") != 0)         // 006CA0C8..A0DF
        kotor_fastcall_006c8490(bytes(self) + 0x58, nullptr, gff, &child);                    // 006CA0E1..A0EA
}

// ===================================================================================== 006CA250
void __fastcall kotor_fastcall_006ca250(void* self, void*, void* module) {
    Scoped saves, game, erf_path, s58;
    saves.empty(); game.empty(); erf_path.empty(); s58.empty();                               // 006CA276..A2AE [20h][40h][28h][58h]
    frame_R4_004ae8a0(server(), nullptr);                                                     // 006CA2B3..A2C7 UnloadModule
    kotor_fastcall_004aed00(server(), nullptr, 1);                                            // 006CA2CC..A2D7 SetLoadingModule(1)
    progress(5);                                                                              // 006CA2DC..A2E9
    frame_LogFormat_005e5680(game.p(), "%06d - %s", 1, "AUTOSAVE");                          // 006CA2EE..A304 [007455A0]
    (void)frame_LogAssign_005e5140(saves.p(), nullptr, "SAVES:");                             // 006CA307..A310
    { Scoped joined; s_concat(saves.p(), joined.p(), game.p()); joined.adopt();               // 006CA315..A345
      s_assign(saves.p(), joined.p()); }
    bool created_none;
    { Scoped dir; dir.text("GAMEINPROGRESS:");                                                // 006CA34A..A385
      created_none = gui_load_resman_create_directory_004068c0(resman(), nullptr, dir.p()) == 0; } // neg/sbb/inc
    if (created_none) {                                                                       // 006CA38A..A390
        Scoped dir; dir.text("GAMEINPROGRESS:");                                              // 006CA392..A3C5
        (void)gui_load_resman_clean_directory_00409460(resman(), nullptr, dir.p(), 0, 0);
    }
    void* const memory = live_operator_new(0xd0);                                             // 006CA3CA..A3D4
    void* const erf = memory != nullptr ? cerf_construct_cerf_file_005dd9c0(memory) : nullptr; // 006CA3D7..A3EE (never freed)
    {
        Scoped savegame; savegame.text("SAVEGAME");                                           // 006CA3F0..A400
        Scoped backslash; backslash.text("\\");                                               // 006CA405..A416
        Scoped t1; s_concat(saves.p(), t1.p(), backslash.p()); t1.adopt();                    // 006CA41B..A431
        Scoped t2; s_concat(t1.p(), t2.p(), savegame.p()); t2.adopt();                        // 006CA436..A44A
        s_assign(erf_path.p(), t2.p());                                                       // 006CA44F..A45C
        t2.end(); t1.end(); backslash.end(); savegame.end();                                  // 006CA461..A49F
    }
    (void)cerf_read_cerf_file_005dce50(erf, nullptr, erf_path.p());                           // 006CA4A4..A4AB
    (void)cerf_read_header_variance_005dd3c0(erf, nullptr, erf_path.p());                     // 006CA4B0..A4B7
    progress(0xa); present();                                                                 // 006CA4BC..A4DC
    { Scoped out; out.text("GAMEINPROGRESS:");                                                // 006CA4DF..A51C
      cerf_export_files_from_erf_005dd710(erf, nullptr, erf_path.p(), out.p(), -1,
          reinterpret_cast<void*>(&cerf_load_screen_update_006c8750), 0xa, 0xa); }
    { Scoped dir; dir.text("TEMP:");                                                          // 006CA521..A552 [00745310]
      (void)gui_load_resman_create_directory_004068c0(resman(), nullptr, dir.p()); }
    {
        Scoped pifo; pifo.text("pifo");                                                       // 006CA557..A560 [00745318]
        Scoped backslash; backslash.text("\\");                                               // 006CA565..A576
        Scoped t1; s_concat(saves.p(), t1.p(), backslash.p()); t1.adopt();                    // 006CA57B..A591
        Scoped t2; s_concat(t1.p(), t2.p(), pifo.p()); t2.adopt();                            // 006CA596..A5AA
        s_assign(erf_path.p(), t2.p());                                                       // 006CA5AF..A5BC
        t2.end(); t1.end(); backslash.end(); pifo.end();                                      // 006CA5C1..A5FF
    }
    { Scoped target; target.text("TEMP:pifo");                                                // 006CA604..A644 [0074554C]
      (void)kotor_fastcall_00406990(resman(), nullptr, erf_path.p(), target.p(), 0x7de, 0xffff); }
    kotor_fastcall_005edd50(client(), nullptr, 1);                                            // 006CA649..A654 SetLoadFromIFO(1)
    progress(0x19); present();                                                                // 006CA659..A679
    (void)static_cast<menu_resources::Manager*>(resman())->directory_00408800(str(saves.p())); // 006CA67C..A687
    kotor_fastcall_00565d20(kotor_fastcall_004aee70(server(), nullptr), nullptr);            // 006CA68C..A69C LoadTableInfo
    progress(0x1e); present();                                                                // 006CA6A1..A6C9
    kotor_fastcall_0052ade0(kotor_fastcall_004aee60(server(), nullptr), nullptr);            // 006CA6C1..A6D3 globals Load
    progress(0x23); present();                                                                // 006CA6D8..A6F3
    void* const gff_memory = live_operator_new(0xa0);                                         // 006CA6F8..A702
    void* gff = nullptr;
    if (gff_memory != nullptr) {                                                              // 006CA70B..A715
        menu_resources::Ref ref; ref.construct_00406d80("savenfo");                           // 006CA717..A720 [0074542C]
        gff = gui_load_gff_construct_00410630(gff_memory, nullptr, 0, "NFO ", &ref);          // 006CA725..A733 [00745424]
    }
    U root = 0, autosave = 0;                                                                 // [esp+38h] / [esp+3Ch]
    kotor_fastcall_00411240(gff, nullptr, &root);                                             // 006CA73E..A74C (NULL unchecked)
    progress(0x28); present();                                                                // 006CA751..A771
    alignas(8) unsigned char params[0x70] = {};                                               // [esp+60h] KOTOR_AUTOSAVE_PARAMS
    (void)kotor_fastcall_004b2840(params, nullptr);                                           // 006CA774..A778
    if (gui_load_gff_struct_00411a10(gff, nullptr, &autosave, &root, "AUTOSAVEPARAMS") != 0)  // 006CA77D..A79D [00745558]
        kotor_fastcall_006c9de0(params, nullptr, gff, &autosave);                             // 006CA79F..A7A9
    if (gff != nullptr) {                                                                     // 006CA7AE..A7B8 vslot 0 (1)
        using Delete = void*(__thiscall*)(void*, U);
        (void)reinterpret_cast<Delete>(at<void*>(at<void*>(gff, 0), 0))(gff, 1);
    }
    progress(0x2d); present();                                                                // 006CA7BA..A7D9
    if (client_movies::StringNotEqual_005e5390(reinterpret_cast<options_video::String*>(params + 8), nullptr, "*") != 0) { // [00743890]
        recovered_server_set_move_to_module_start_waypoint_004aed30(server(), nullptr, params + 8); // 006CA7EE..A7FC
    } else {
        Scoped none; none.text("");                                                           // 006CA803..A80C
        recovered_server_set_move_to_module_start_waypoint_004aed30(server(), nullptr, none.p()); // 006CA811..A826
    }
    U t[7];                                                                                   // dword reads at +40/+44/+45/+46/+48/+4A/+4C
    static const U offs[7] = {0x40, 0x44, 0x45, 0x46, 0x48, 0x4a, 0x4c};
    for (U k = 0; k < 7; ++k) std::memcpy(&t[k], params + offs[k], 4);                        // 006CA844..A87B
    kotor_fastcall_004aebc0(server(), nullptr, t[0], t[1], t[2], t[3], t[4], t[5], t[6]);    // 006CA87C SetMoveToWorldTime
    kotor_fastcall_004af0c0(server(), nullptr, at<U>(params, 0x54));                          // 006CA881..A891
    kotor_fastcall_004af0a0(server(), nullptr, at<U>(params, 0x50));                          // 006CA896..A8A6
    for (U k = 0; k < 6; ++k) recovered_client_add_movie_005edb50(client(), nullptr, params + 0x10 + k * 8); // 006CA8AB..A91E
    kotor_fastcall_0062f040(frame_R4_005ed690(client(), nullptr), nullptr, params + 0x58);    // 006CA923..A93A
    load_screen_set_load_music_005edd30(client(), nullptr, params);                           // 006CA93F..A94C
    kotor_fastcall_004088d0(resman(), nullptr, saves.p());                                    // 006CA951..A95C
    progress(0x32); present();                                                                // 006CA961..A97C
    panel_set_loading_flags(self);                                                            // 006CA981..A993
    progress(0xa);                                                                            // 006CA996..A9A2
    kotor_fastcall_005edfd0(client(), nullptr, 0, 0xa);                                       // 006CA9A7..A9B4
    kotor_fastcall_005edfd0(client(), nullptr, 1, 0x14);                                      // 006CA9B9..A9C6
    kotor_fastcall_005edfd0(client(), nullptr, 2, 0x17);                                      // 006CA9CB..A9D7
    kotor_fastcall_005edfd0(client(), nullptr, 3, 0x17);                                      // 006CA9DC..A9E8 (ebx = 3)
    kotor_fastcall_005edfd0(client(), nullptr, 4, 0x17);                                      // 006CA9ED..A9FA
    kotor_fastcall_004af090(server(), nullptr, 1);                                            // 006CA9FF..AA09 SetDoingStartNewModule
    kotor_fastcall_004ba820(at<void*>(server(), 4), nullptr, module);                         // 006CAA0E..AA1B ExecuteLoadModule thunk
    kotor_fastcall_004b2230(params, nullptr);                                                 // 006CAA20..AA2B
    s58.end(); erf_path.end(); game.end(); saves.end();                                       // 006CAA30..AA72
}

// ===================================================================================== 006CB0E0
// self = CSWGuiSaveLoad (+18h manager, +44h panel flags, +64h bit1 opened from the main menu); entry =
// CSWGuiSaveLoadEntry (+1C4h flags, +1C8h number, +1D4h/+1D5h hints, +1E0h directory suffix, +1F0h last module,
// +210h missing live content name).
void __fastcall kotor_fastcall_006cb0e0(void* self, void*, void* entry) {
    const bool from_main_menu = (at<U8>(self, 0x64) & 2u) != 0;
    if (!from_main_menu) {                                                                    // 006CB101..B107
        void* const box = at<void*>(frame_R4_005ed690(client(), nullptr), 0x98);              // 006CB109..B116
        if ((at<U8>(box, 0x64) & 1u) == 0) return;                                            // 006CB11C..B120
    }
    frame_R4_005d5e80(kotor_cell_sound_007a39ec, nullptr, k_sound_mode_0074c5ec);            // 006CB126..B133
    if ((at<U8>(self, 0x64) & 2u) != 0) kotor_fastcall_005ee120(client(), nullptr);          // 006CB138..B147 BusyWaitMenuMusic
    else load_screen_busy_wait_mute_sound_005ee130(client(), nullptr);                        // 006CB14E..B156
    void* const c = client();                                                                 // esi
    if ((at<U8>(entry, 0x1c4) & 0x20u) != 0 && kotor_cell_00832ca8 == 0) {                    // 006CB15B..B17B live content missing
        void* const box = at<void*>(frame_R4_005ed690(c, nullptr), 0x98);                     // 006CB181..B188
        kotor_fastcall_00627130(box, nullptr, 0);                                             // 006CB18E..B192
        gui_load_messagebox_set_callback_00624a40(box, nullptr, 0, 0, 0);                     // 006CB197..B19F
        Scoped message; message.empty();                                                      // 006CB1A4..B1A8 [10h]
        { Scoped text;                                                                        // [28h]
          (void)tlk()->simple_0041e8f0(reinterpret_cast<kotor_menu_10_19::String*>(text.p()), 0); text.adopt(); // 006CB1AD..B1C2
          s_assign(message.p(), text.p()); }                                                  // 006CB1C7..B1DF
        kotor_fastcall_0041db50(tlk(), nullptr, 0, bytes(entry) + 0x210);                     // 006CB1E4..B1F3
        tlk_core::ParseStr0041dd30(tlk(), nullptr, str(message.p()));                         // 006CB1F8..B203
        alignas(4) unsigned char by_value[8];                                                 // CExoString passed by value
        (void)WindowCopy005e5b00(by_value, nullptr, message.p());                             // 006CB208..B216
        U w0, w1; std::memcpy(&w0, by_value, 4); std::memcpy(&w1, by_value + 4, 4);
        using SetMessage = void(__thiscall*)(void*, U, U);                                    // vslot +70h (006271A0, callee destroys)
        reinterpret_cast<SetMessage>(at<void*>(at<void*>(box, 0), 0x70))(box, w0, w1);        // 006CB21B..B21F
        kotor_fastcall_0040bc70(at<void*>(self, 0x18), nullptr, box, 1, 1);                   // 006CB222..B22C
        message.end();                                                                        // 006CB231..B23D
        return;
    }
    kotor_m15_party_reset_session_start_time_00563cf0();                                      // 006CB256
    kotor_game_prepare::SetGlobalUnpauseableState0062b410(frame_R4_005ed690(client(), nullptr), nullptr, 1); // 006CB25B..B26C
    const U flags = at<U>(entry, 0x1c4);                                                      // 006CB271
    if ((flags & 8u) != 0) return;                                                            // 006CB277..B279 REBOOTAUTOSAVE
    if ((flags & 0x10u) != 0) {                                                               // 006CB27F..B281 PCAUTOSAVE
        (void)kotor_fastcall_0040be00(at<void*>(self, 0x18), nullptr);                        // 006CB287..B28A
        frame_R4_0062cba0(frame_R4_005ed690(c, nullptr), nullptr, 0);                         // 006CB28F..B29A
        void* const module = bytes(entry) + 0x1f0;                                            // edi += 1F0h
        kotor_m15_client_set_load_screen_by_module_005edcf0(c, nullptr, module);              // 006CB29F..B2A8
        { Scoped music; music.text("");                                                       // 006CB2AD..B2DB
          load_screen_set_load_music_005edd30(c, nullptr, music.p()); }
        kotor_m15_client_show_load_screen_005eda80(c, nullptr, 1, 1, 0);                      // 006CB2E0..B2E8
        kotor_m15_client_set_load_bar_progress_005edd40(c, nullptr, 0, 1);                    // 006CB2ED..B2F3
        present();                                                                            // 006CB2F8..B309
        if ((at<U8>(self, 0x64) & 2u) == 0) {                                                 // 006CB306..B30E
            frame_R4_00401280(app(), nullptr);                                                // 006CB310..B316 DestroyServer
            recovered_client_unload_module_005ed430(client(), nullptr);                       // 006CB31B..B324
        }
        (void)recovered_create_server_00401380(app(), nullptr, 0);                            // 006CB329..B331
        pause_world_timers();                                                                 // 006CB336..B35A
        connect_to_local_session(c);                                                          // 006CB35F..B3C0
        (void)recovered_support_request_status_006775e0(recovered_support_client_message_005ed6f0(c, nullptr), nullptr); // 006CB3C5..B3CE
        kotor_fastcall_005eda60(c, nullptr, 0, 1);                                            // 006CB3D3..B3D9 SetInputClass
        disable_input_005edbe0(c, nullptr);                                                   // 006CB3DE..B3E0
        kotor_fastcall_006ca250(self, nullptr, module);                                       // 006CB3E5..B3E8 LoadPCAutoSave
        panel_set_loading_flags(self);                                                        // 006CB3ED..B3FC
        return;
    }
    if ((at<U8>(self, 0x64) & 2u) != 0) {                                                     // 006CB413..B417 main menu
        kotor_m15_client_set_load_screen_by_module_005edcf0(c, nullptr, bytes(entry) + 0x1f0); // 006CB41D..B426
        { Scoped music; music.text("");                                                       // 006CB42B..B459
          load_screen_set_load_music_005edd30(c, nullptr, music.p()); }
        kotor_m15_client_show_load_screen_005eda80(c, nullptr, 0, 1, 0);                      // 006CB45E..B466
        load_screen_set_gameplay_hint_005ee060(c, nullptr, at<U8>(entry, 0x1d4));             // 006CB46B..B47C (byte in a dword slot)
        load_screen_set_story_hint_005ee080(c, nullptr, at<U8>(entry, 0x1d5));                // 006CB481..B492
        { Scoped none; none.text("");                                                         // 006CB497..B4CF
          kotor_m15_client_set_load_screen_hint_005edf00(c, nullptr,
              load_screen_get_next_load_screen_hint_strref_005ee090(c, nullptr, none.p(), 2)); }
        load_screen_set_load_screen_hint_update_timer_005ee040(c, nullptr, 0);                // 006CB4D4..B4D8
        const U number = at<U>(entry, 0x1c8);                                                 // 006CB4DD [esp+18h]
        void* const directory = bytes(entry) + 0x1e0;                                         // [esp+10h]
        Scoped dir_copy; dir_copy.copy(directory);                                            // 006CB4E3..B4F6 [28h]
        Scoped module_copy; module_copy.copy(bytes(entry) + 0x1f0);                           // 006CB4FB..B50E [20h]
        if (kotor_fastcall_006caaf0(self, nullptr, static_cast<I>(at<U>(entry, 0x1c8)), directory, entry) != 0) { // 006CB513..B52E
            panel_set_loading_flags(self);                                                    // 006CB534..B543
            (void)recovered_create_server_00401380(app(), nullptr, 0);                        // 006CB546..B54E
            pause_world_timers();                                                             // 006CB553..B577
            connect_to_local_session(c);                                                      // 006CB57C..B5D7
            (void)recovered_support_request_status_006775e0(recovered_support_client_message_005ed6f0(c, nullptr), nullptr); // 006CB5DC..B5E5
            { Scoped player; player.text("test");                                             // 006CB5EA..B612 [0074A254]
              kotor_fastcall_005edaa0(c, nullptr, player.p()); }
            kotor_fastcall_005eda60(c, nullptr, 0, 1);                                        // 006CB617..B61D
            disable_input_005edbe0(c, nullptr);                                               // 006CB622..B624
            load_screen_set_load_from_save_game_004af060(server(), nullptr, 1);               // 006CB629..B634
            kotor_fastcall_005edfc0(client(), nullptr, 1);                                    // 006CB639..B643
            load_screen_send_load_game_request_005ed680(client(), nullptr, number, dir_copy.p(), module_copy.p()); // 006CB648..B660
        }
        module_copy.end(); dir_copy.end();                                                    // 006CB665..B67F
        return;
    }
    // in game
    kotor_m15_client_set_load_screen_by_module_005edcf0(client(), nullptr, bytes(entry) + 0x1f0); // 006CB698..B6A9 (esi reloaded)
    { Scoped music; music.text("");                                                           // 006CB6AE..B6DC
      load_screen_set_load_music_005edd30(client(), nullptr, music.p()); }
    kotor_m15_client_show_load_screen_005eda80(client(), nullptr, 0, 1, 0);                   // 006CB6E1..B6E9
    load_screen_set_gameplay_hint_005ee060(client(), nullptr, at<U8>(entry, 0x1d4));          // 006CB6EE..B6FF
    load_screen_set_story_hint_005ee080(client(), nullptr, at<U8>(entry, 0x1d5));             // 006CB704..B715
    { Scoped none; none.text("");                                                             // 006CB71A..B752
      kotor_m15_client_set_load_screen_hint_005edf00(client(), nullptr,
          load_screen_get_next_load_screen_hint_strref_005ee090(client(), nullptr, none.p(), 2)); }
    load_screen_set_load_screen_hint_update_timer_005ee040(client(), nullptr, 0);             // 006CB757..B75B
    if (kotor_fastcall_006caaf0(self, nullptr, static_cast<I>(at<U>(entry, 0x1c8)), bytes(entry) + 0x1e0, entry) == 0) // 006CB760..B778
        return;
    panel_set_loading_flags(self);                                                            // 006CB77E..B790
    while (at<void*>(at<void*>(self, 0x18), 0x98) != nullptr)                                 // 006CB793..B7B3 pop every modal panel
        (void)kotor_fastcall_0040be00(at<void*>(self, 0x18), nullptr);
    frame_R4_0062cba0(frame_R4_005ed690(client(), nullptr), nullptr, 1);                      // 006CB7B5..B7C0
    load_screen_set_load_from_save_game_004af060(server(), nullptr, 1);                       // 006CB7C5..B7D0
    kotor_fastcall_005edfc0(client(), nullptr, 1);                                            // 006CB7D5..B7E0
    load_screen_send_load_game_request_005ed680(client(), nullptr, at<U>(entry, 0x1c8),
                                                bytes(entry) + 0x1e0, bytes(entry) + 0x1f0); // 006CB7E5..B803
}
}  // extern "C"
