// claude_closeB_save.cpp (lane worker closeB, fork "save"): the server save-game path behind the 0886 NativePorts gates
// save_004b58a0 / save_done_004b3000 / unload_phase_004b3110 (CServerExoAppInternal::MainLoop 004BABB0), with its
// missing callee cascade, transcribed from the original swkotor.exe (read-only):
//   004B58A0 CServerExoAppInternal::SaveGame(ulong number, CExoString const& name, ulong player, int) RET 10h, EAX 0/1
//   004B3000 CServerExoAppInternal::ExportAllPlayers()                       RET 0, EAX 1     kotor_fastcall_004b3000
//   004B3110 CServerExoAppInternal::StallEventSaveGame()                     RET 0            kotor_fastcall_004b3110
//   005665C0 CSWPartyTable::Save(CExoString const& dir, int members)         RET 8            kotor_fastcall_005665c0
//   004C3C70 CSWSModule::SavePrimaryPlayerInfo(CSWSCreature*, CExoString const& dir) RET 8   kotor_fastcall_004c3c70
//   0056C850 CSWSMessage::SendServerToPlayerModule_SaveGameStatus(uchar)     RET 4            kotor_fastcall_0056c850
//   0056C8B0 CSWSMessage::SendServerToPlayerLoadBar_StartStallEvent(ulong)   RET 4            kotor_fastcall_0056c8b0
//   0056C980 CSWSMessage::SendServerToPlayerLoadBar_EndStallEvent(ulong, ulong) RET 8        kotor_fastcall_0056c980
//   0056CAC0 CSWSMessage::SendServerToPlayerModule_ExportReply(CSWSPlayer*)  RET 4            kotor_fastcall_0056cac0
//   005EE020 CClientExoApp::GetLiveContentName(int)                          RET 4            kotor_fastcall_005ee020
//   005E6650 CExoBase::GetDirectorySize(CExoString const&) -> internal       RET 4            kotor_fastcall_005e6650
//   005E7820 CExoBaseInternal::GetDirectorySize(CExoString const&)           RET 4            kotor_fastcall_005e7820
//   00638E80 CSWCameraOnAStick::SetCameraForScreenShot()                     RET 0            kotor_fastcall_00638e80
//   00401080 CAppManager::DoSaveGameScreenShot(CExoString const&)            RET 4            kotor_fastcall_00401080
//   00420F20 AurSaveGameSnapshot(char const*, int, uchar, uchar)             cdecl            kotor_cdecl_00420f20
//   0045D990 ImageFlipY(uchar*, int, int, int)                               cdecl            kotor_cdecl_0045d990
//   0045F2A0 ImageWriteTGA(char const*, int, int, int, uchar const*)         cdecl            kotor_cdecl_0045f2a0
// Conventions (W5): thiscall -> extern "C" __fastcall(self, unused_edx, stack args...); x87 at 53-bit precision ->
// double intermediates, float where the original stores a dword; game heap only (live_operator_new / live_free).
// Proven .rdata constants: [0074C5F8]=3, [0074C5EC]=0 (sound modes), [0074C52C]=FFFFFFF7h (all players),
// [0073ECD2]=7 (live content slots), [0073F370]=1907h (GL_RGB), [0073D700]=0.0f.
// .data/.bss cells referenced only by these functions are rebuilt here (zero-initialised as in the image):
// [007A47B0] snapshot counter, [007B9350] snapshot-render flag, [00831FE0] primary-player-save switch (never written by
// any original instruction, so StallEventSaveGame's SavePrimaryPlayerInfo branch is live only if something sets it).
// Quirks kept: SavePrimaryPlayerInfo leaks its 4-byte root struct; StallEventSaveGame writes LIVECONTENT as the full
// dword whose low byte is the mask (upper bytes = the client language word left in that slot); null allocation
// results are dereferenced exactly where the original does; GetDirectorySize re-adds the last size while FindNextFile
// fails with an error other than ERROR_NO_MORE_FILES; AurSaveGameSnapshot's width<=height arm scales to
// (name pointer value, third argument) as the original does. EH unwind states only destroy CExoString locals and are
// not reproduced beyond scope order (no bound callee throws in normal operation).
#include <cstdint>
#include <cstdio>
#include <cstring>

namespace {
using U = std::uint32_t;
using I = std::int32_t;
inline void* ptr(U a) { return reinterpret_cast<void*>(a); }
inline U word(const void* p) { return reinterpret_cast<U>(p); }
inline U rd(U a) { U v; std::memcpy(&v, ptr(a), 4); return v; }
inline void wr(U a, U v) { std::memcpy(ptr(a), &v, 4); }
inline unsigned char rd8(U a) { return *static_cast<unsigned char*>(ptr(a)); }
inline float flt(U u) { float f; std::memcpy(&f, &u, 4); return f; }
template <class R, class... A> R vcall(U object, U slot, A... a) {
    using F = R(__thiscall*)(void*, A...);
    return reinterpret_cast<F>(rd(rd(object) + slot))(ptr(object), a...);
}
struct V3 { float x, y, z; };
struct Q4 { U w[4]; };
constexpr U k_sound_mode_0074c5f8 = 3;
constexpr U k_sound_mode_0074c5ec = 0;
constexpr U k_all_players_0074c52c = 0xfffffff7u;
constexpr U k_live_slots_0073ecd2 = 7;
constexpr U k_gl_rgb_0073f370 = 0x1907;
constexpr U k_frame_dt_3d088889 = 0x3d088889u;                     // 1/30 s (immediate)
}

namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; extern void* volatile input_007a39e4; }
namespace frame_control { extern unsigned resource_manager_007a39e8; }     // [007A39E8] CExoResMan*
namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } }   // [007A39E0] CExoBase*
namespace kotor_reconstruction { struct StartupString; }
namespace w2 { std::uint32_t ftol2_006fae8c(double value); }                               // __ftol2 low dword [w2_ftol_native]
kotor_reconstruction::StartupString* __fastcall GuiConcat005e5d10(kotor_reconstruction::StartupString* self, void*,
    kotor_reconstruction::StartupString* out, const kotor_reconstruction::StartupString* other); // operator+ [0093]
namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); } // CExoString()
namespace base_ini_reconstruction {
kotor_reconstruction::StartupString* __fastcall right_005e6170(kotor_reconstruction::StartupString*, void*,
                                                                kotor_reconstruction::StartupString*, int); // Right [0126]
}
namespace alias_path { struct AliasListView; }
namespace path_resolution {
kotor_reconstruction::StartupString* __fastcall resolve_005e68a0(alias_path::AliasListView*, void*,
    kotor_reconstruction::StartupString*, kotor_reconstruction::StartupString*, unsigned);    // ResolveFileName [0241]
}
namespace directory_list { char __fastcall At005e5600(kotor_reconstruction::StartupString*, void*, int); } // operator[]
namespace options_video { struct String; }
namespace client_movies { int __fastcall StringNotEqual_005e5390(options_video::String*, void*, const char*); } // !=(char*)
namespace gui_ctor { struct Gui { void Draw_0040cc50(float dt); }; }                           // CSWGuiManager::Draw [0409]
namespace kotor_game_prepare { void __fastcall SetGlobalUnpauseableState0062b410(void*, void*, unsigned); } // [0461]
namespace kotor_input_required { unsigned __fastcall low_resolution_timer_005e9250(void*, void*); } // GetLowResolutionTimer
namespace kotor_renderer_init {
void __stdcall glGetIntegerv_0073d244(unsigned, int*);
void __stdcall glClear_0073d21c(unsigned);
void __stdcall glLoadIdentity_0073d230();
void __stdcall glReadPixels_0073d250(int, int, int, int, unsigned, unsigned, void*);
}

extern "C" {
// ---------------- existing providers (c35 map, exact decorated arity) ----------------
void* __cdecl live_operator_new(unsigned);                                                  // 006FA7E6
void __cdecl live_free(void*);                                                              // 006FA390
extern void* kotor_cell_sound_007a39ec;                                                     // [007A39EC] CExoSound* [0673]
extern unsigned char kotor_rules_cell8_007a39dc;                                            // [007A39DC]
void* __fastcall frame_LogStringCtor_005e5a90(void* string, void*, const char* text);      // CExoString(char*)
void __fastcall frame_LogStringDtor_005e5c20(void* string, void*);                         // ~CExoString
void* __fastcall async_assign_string_005e5c50(void* self, void*, void* other);             // operator=
void* __fastcall frame_LogAssign_005e5140(void* self, void*, const char* text);            // operator=(char*)
void __cdecl frame_LogFormat_005e5680(void* self, const char* format, ...);                // CExoString::Format
const char* __fastcall WindowCStr005e5670(void*, void*);                                   // CStr
U __fastcall async_string_length_005e5790(void* string, void*);                            // GetLength
int __fastcall find_005e5f20(void* string, void*, int ch, int start);                      // CExoString::Find(char,int)
void* __fastcall kotor_fastcall_005e6890(void* alias_list, void*, void* alias, I length);  // CExoAliasList::GetAliasPath RET 8
void __fastcall frame_R4_005d5e80(void* sound, void*, U mode);                             // SetSoundMode [0709]
U __fastcall gui_load_resman_create_directory_004068c0(void* resman, void*, void* path);   // CreateDirectory2
U __fastcall gui_load_resman_clean_directory_00409460(void* resman, void*, void* path, U a, U b); // CleanDirectory
U __fastcall gui_load_resman_free_disk_space_004067b0(void* resman, void*, void* path, std::uint64_t* out); // GetFreeDiskSpace
U __fastcall kotor_fastcall_0056cb80(void* message, void*, U player, U status, U value);   // SaveLoad_Status [0886 bodies]
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);               // GetSWSMessage
void* __fastcall recovered_server_get_module_004b14f0(void* internal, void*);              // CServerExoAppInternal::GetModule
void* __fastcall kotor_fastcall_005ed530(void* client, void*);                             // CClientExoApp::GetModule
void* __fastcall kotor_fastcall_005eda40(void* client, void*);                             // GetSWGuiManager [0417]
void __cdecl kotor_cdecl_00401c10(U dt_bits, U a, U b);                                    // UpdateScreen [0785]
void* __fastcall frame_R4_005ed690(void* client, void*);                                   // GetInGameGui [0457]
void __fastcall kotor_m15_ingame_start_global_fade_0062abf0(void*, void*, U, U, U, const void*); // StartGlobalFade [0814]
void __fastcall kotor_fastcall_005edc80(void* client, void*, U value);                     // SetCanSendDialog
void __fastcall kotor_fastcall_005ed310(void* object, void*);                              // void_return_this+4
void __fastcall kotor_fastcall_00604d70(void* area, void*);                                // CSWCArea::StartSounds
void __fastcall kotor_m15_client_set_load_screen_by_module_005edcf0(void* client, void*, void* module);
U __fastcall load_screen_get_next_load_screen_hint_strref_005ee090(void* client, void*, void* module, U a);
void __fastcall kotor_m15_client_set_load_screen_hint_005edf00(void* client, void*, U strref);
void __fastcall kotor_m15_client_show_load_screen_005eda80(void* client, void*, U a, U b, U c);
void __fastcall kotor_m15_client_set_load_bar_progress_005edd40(void* client, void*, U value, U a);
void __fastcall kotor_m15_client_hide_load_screen_005eda90(void* client, void*, U a, U b);
U __fastcall kotor_fastcall_004b2e70(void* internal, void*);                               // StoreCurrentModule [save_module]
void __fastcall kotor_fastcall_00563e80(void* table, void*, I index, I clear_actions);     // SaveMember [route_bodies]
void __fastcall kotor_fastcall_00564030(void* table, void*, U flag);                       // UpdateInventory [route_bodies]
void __fastcall kotor_fastcall_005648c0(void* table, void*, void* directory);              // SaveTableInfo [w1_autosave]
U __fastcall kotor_fastcall_00563bf0(void* table, void*);                                  // GetTotalPlayedSeconds
void __fastcall kotor_fastcall_0052ad10(void* table, void*, void* directory);              // CSWGlobalVariableTable::Save
U __fastcall kotor_fastcall_005ee050(void* client, void*);                                 // GetGamePlayHint
U __fastcall kotor_fastcall_005ee070(void* client, void*);                                 // GetStoryHint
I __cdecl kotor_cdecl_004b30d0(I index, I count, I base, I span);                          // UpdateLoadScreen callback
U __fastcall kotor_fastcall_004aea40(void* server, void*);                                 // GetPlayerCreatureId
void* __fastcall kotor_fastcall_004ae770(void* server, void*, U id);                       // GetCreatureByGameObjectID
void* __fastcall kotor_fastcall_004b2a50(void* internal, void*, U id);                     // internal GetClientObjectByObjectId
void* __fastcall kotor_fastcall_004ae910(void* server, void*, U id);                       // CServerExoApp::GetClientObjectByObjectId
void* __fastcall recovered_object_area_004cb120(void* object, void*);                      // CSWSObject::GetArea
void* __fastcall exo_locstring_get_005ea130(void* loc, void*, U lang, void* out, U gender); // CExoLocString::GetString
void* __fastcall kotor_fastcall_005ed8b0(void* client, void*);                             // GetSWParty
void* __fastcall frame_GetCharacter_006346c0(void* party, void*, U index);                 // CSWParty::GetCharacter
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void* creature, void*);  // GetServerCreature
void* __fastcall recovered_creature_client_creature_004efb20(void* creature, void*);       // GetClientCreature
void* __fastcall kotor_fastcall_005610b0(void* player, void*, void* out);                  // CSWSPlayer::GetPlayerName
U __fastcall kotor_fastcall_005624c0(void* player, void*);                                 // SaveServerCharacter
U __fastcall kotor_fastcall_00561870(void* player, void*);                                 // PackCreatureIntoMessage
U __fastcall recovered_save_creature_00500610(void* creature, void*, void* gff, void* s);  // CSWSCreature::SaveCreature
void* __fastcall recovered_gff_construct_004105a0(void* gff, void*);                       // CResGFF::CResGFF
U __fastcall recovered_gff_00411260(void* gff, void*, void* root, void* type, void* version); // CreateGFFFile
U __fastcall recovered_gff_00412620(void* gff, void*, void* s, U value, const char* label); // WriteFieldBYTE
U __fastcall recovered_gff_00412760(void* gff, void*, void* s, U value, const char* label); // WriteFieldDWORD
U __fastcall recovered_gff_004128c0(void* gff, void*, void* s, void* resref, const char* label); // WriteFieldCResRef
U __fastcall recovered_gff_00412970(void* gff, void*, void* s, void* string, const char* label); // WriteFieldCExoString
U __fastcall recovered_gff_00413030(void* gff, void*, void* path, U type);                 // WriteGFFFile
void* __fastcall cerf_construct_cerf_file_005dd9c0(void* self);                            // CERFFile::CERFFile
void __fastcall cerf_destroy_cerf_file_005de3a0(void* self);                               // CERFFile::~CERFFile
U __fastcall cerf_read_cerf_file_005dce50(void* self, void*, void* path);                  // CERFFile::Read
U __fastcall cerf_read_header_variance_005dd3c0(void* self, void*, void* path);            // CERFFile::ReadHeaderVariance
U __fastcall kotor_fastcall_005dcfc0(void* self, void*, void* path);                       // CERFFile::Create
void __fastcall kotor_fastcall_005dce30(void* self, void*, const char* version);           // SetVersion
U __fastcall kotor_fastcall_005dd080(void* self, void*);                                   // WriteHeader
U __fastcall kotor_fastcall_005dd5a0(void* self, void*);                                   // WriteStringTable
void __fastcall kotor_fastcall_005de1a0(void* self, void*, void* directory, U type, void* callback, U a4, U a5); // ImportFiles
U __fastcall kotor_fastcall_005dd0e0(void* self, void*);                                   // Finish
void __fastcall kotor_fastcall_004d68a0(void* message, void*, U size, U a, U b);           // CreateWriteMessage
void __fastcall kotor_fastcall_004d62c0(void* message, void*, U value, U bits);            // WriteBYTE
void __fastcall kotor_fastcall_004d6340(void* message, void*, U value, U bits);            // WriteDWORD
U __fastcall kotor_fastcall_004d6440(void* message, void*, void** data, U* size);          // GetWriteMessage
U __fastcall kotor_fastcall_00567e60(void* message, void*, U player, U major, U minor, void* data, U size); // SendServerToPlayerMessage
double __cdecl kotor_m15a_pitch_004aa130(const void* v);                                   // Pitch (ST0)
double __cdecl kotor_m15a_yaw_004aa0f0(const void* v);                                     // Yaw (ST0)
void* __cdecl normalize_004ab180(void* out, const void* v);                                // normalize
void* __cdecl yaw_pitch_roll_004acac0(void* out, float yaw, float pitch, float roll);      // YawPitchRoll
void* __cdecl find_gob_00440460(const char* name);                                         // FindGob [0469]
void* __cdecl image_scale_0045dad0(const void*, I, I, I, I, I);                             // ImageScale [0573]
void __cdecl image_tga_swap_bgr_to_rgb_0045da80(char*, I, I, I);                           // ImageTGASwapBGRtoRGB [0688]
U __fastcall unresolved_list_at_pos_005e9b60(void* list, void*, void* pos);                 // GetAtPos
U __fastcall unresolved_list_next_005e9b90(void* list, void*, void** pos);                  // GetNext
int __cdecl kotor_stricmp_0070acaf(const char*, const char*);                              // __stricmp [0155]
__declspec(dllimport) void* __stdcall FindFirstFileA(const char*, void*);
__declspec(dllimport) int __stdcall FindNextFileA(void*, void*);
__declspec(dllimport) int __stdcall FindClose(void*);
__declspec(dllimport) unsigned long __stdcall GetLastError();

// ---------------- cells rebuilt here ----------------
U kotor_closeB_snapshot_counter_007a47b0 = 0;                                               // [007A47B0]
U kotor_closeB_snapshot_render_007b9350 = 0;                                                // [007B9350]
U kotor_closeB_primary_player_save_00831fe0 = 0;                                            // [00831FE0]

// ---------------- bodies defined here ----------------
U __fastcall kotor_fastcall_004b58a0(void* internal, void*, U number, void* name, U player, U unused);
U __fastcall kotor_fastcall_004b3000(void* internal, void*);
void __fastcall kotor_fastcall_004b3110(void* internal, void*);
void __fastcall kotor_fastcall_005665c0(void* table, void*, void* directory, U members);
void __fastcall kotor_fastcall_004c3c70(void* module, void*, void* creature, void* directory);
U __fastcall kotor_fastcall_0056c850(void* message, void*, U status);
U __fastcall kotor_fastcall_0056c8b0(void* message, void*, U value);
U __fastcall kotor_fastcall_0056c980(void* message, void*, U a, U b);
U __fastcall kotor_fastcall_0056cac0(void* message, void*, void* player);
void* __fastcall kotor_fastcall_005ee020(void* client, void*, I index);
U __fastcall kotor_fastcall_005e6650(void* base, void*, void* directory);
U __fastcall kotor_fastcall_005e7820(void* internal, void*, void* directory);
void __fastcall kotor_fastcall_00638e80(void* camera, void*);
void __fastcall kotor_fastcall_00401080(void* app, void*, void* name);
void __cdecl kotor_cdecl_00420f20(const char* name, I size, U square, U pad);
void __cdecl kotor_cdecl_0045d990(unsigned char* data, I width, I height, I components);
void __cdecl kotor_cdecl_0045f2a0(const char* file, I width, I height, I components, const unsigned char* data);
}

namespace {
using Str = kotor_reconstruction::StartupString;
inline Str* str(void* p) { return static_cast<Str*>(p); }
struct alignas(4) ExoString { unsigned char b[8]; };
// One CExoString destroyed when its scope ends, mirroring the original EH states.
struct Scoped {
    ExoString s; bool live = false;
    void* p() { return s.b; }
    void* text(const char* t) { (void)frame_LogStringCtor_005e5a90(s.b, nullptr, t); live = true; return s.b; }
    void* empty() { (void)kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(s.b), nullptr); live = true; return s.b; }
    void* adopt() { live = true; return s.b; }
    void end() { if (live) { live = false; frame_LogStringDtor_005e5c20(s.b, nullptr); } }
    ~Scoped() { end(); }
};
inline U app() { return word(const_cast<void*>(kotor_menu_window_cells::manager_007a39fc)); }
inline void* client() { return ptr(rd(app() + 4)); }
inline void* server() { return ptr(rd(app() + 8)); }
inline void* resman() { return ptr(frame_control::resource_manager_007a39e8); }
inline U exo_base() { return word(kotor_menu_10_19::dependencies::base_007a39e0); }
inline void* sws_message() { return recovered_server_sws_message_004aedb0(server(), nullptr); }
inline void s_assign(void* s, void* o) { (void)async_assign_string_005e5c50(s, nullptr, o); }
inline void s_concat(void* self, void* out, void* other) { (void)GuiConcat005e5d10(str(self), nullptr, str(out), str(other)); }
inline const char* cstr(void* s) { return WindowCStr005e5670(s, nullptr); }
inline void present() { kotor_cdecl_00401c10(k_frame_dt_3d088889, 0, 0); }
inline void draw_and_present() {
    static_cast<gui_ctor::Gui*>(kotor_fastcall_005eda40(client(), nullptr))->Draw_0040cc50(flt(k_frame_dt_3d088889));
    present();
}
inline void progress(U value) { kotor_m15_client_set_load_bar_progress_005edd40(client(), nullptr, value, 0); }
inline void* new_gff() {                                                                       // operator new(A0h) + CResGFF()
    void* memory = live_operator_new(0xa0);
    return memory != nullptr ? recovered_gff_construct_004105a0(memory, nullptr) : nullptr;
}
inline void gff_delete(void* gff) { if (gff != nullptr) (void)vcall<void*>(word(gff), 0, 1u); }
// Shared tail of the three LoadBar / status messages: GetWriteMessage, then send to all players.
U send_all(void* message, U major, U minor) {
    void* data = nullptr; U size = 0;
    if (kotor_fastcall_004d6440(message, nullptr, &data, &size) == 0) return 0;
    return kotor_fastcall_00567e60(message, nullptr, k_all_players_0074c52c, major, minor, data, size);
}
}

extern "C" {
// ===================================================================================== 0056C850 / 0056C8B0 / 0056C980
U __fastcall kotor_fastcall_0056c850(void* message, void*, U status) {
    kotor_fastcall_004d68a0(message, nullptr, 1, 0xffffffffu, 1);                    // 0056C854..0056C85C
    kotor_fastcall_004d62c0(message, nullptr, status, 8);                            // 0056C861..0056C86A
    return send_all(message, 3, 0xd);                                                // 0056C86F..0056C8A2
}
U __fastcall kotor_fastcall_0056c8b0(void* message, void*, U value) {
    kotor_fastcall_004d68a0(message, nullptr, 4, 0xffffffffu, 1);                    // 0056C8B4..0056C8BC
    kotor_fastcall_004d6340(message, nullptr, value, 0x20);                          // 0056C8C1..0056C8CA
    return send_all(message, 0x2c, 1);                                               // 0056C8CF..0056C902
}
U __fastcall kotor_fastcall_0056c980(void* message, void*, U a, U b) {
    kotor_fastcall_004d68a0(message, nullptr, 4, 0xffffffffu, 1);                    // 0056C984..0056C98C
    kotor_fastcall_004d6340(message, nullptr, a, 0x20);                              // 0056C991..0056C99A
    kotor_fastcall_004d6340(message, nullptr, b, 3);                                 // 0056C99F..0056C9A8
    return send_all(message, 0x2c, 3);                                               // 0056C9AD..0056C9DF
}
// ===================================================================================== 0056CAC0
U __fastcall kotor_fastcall_0056cac0(void* message, void*, void* player) {
    (void)kotor_fastcall_00561870(player, nullptr);                                  // 0056CAC3..0056CACB
    void* data = nullptr; U size = 0;                                                // [esp+8] / argument slot
    if (kotor_fastcall_004d6440(message, nullptr, &data, &size) == 0) return 0;      // 0056CAD0..0056CAE8
    return kotor_fastcall_00567e60(message, nullptr, rd(word(player) + 4), 3, 0xa, data, size); // 0056CAEB..0056CAFF
}
// ===================================================================================== 005EE020
void* __fastcall kotor_fastcall_005ee020(void* client_object, void*, I index) {
    return ptr(static_cast<U>(index) * 16 + rd(word(client_object) + 4) + 0x418);   // 005EE020..005EE02A
}

// ===================================================================================== 005E7820 / 005E6650
U __fastcall kotor_fastcall_005e7820(void*, void*, void* directory) {
    Scoped path; path.empty();                                                       // [esp+10h]
    Scoped name; name.empty();                                                       // [esp+18h]
    Scoped unused; unused.empty();                                                   // [esp+20h]
    U total = 0;                                                                     // esi
    {
        Scoped resolved;                                                             // [esp+28h]
        (void)path_resolution::resolve_005e68a0(static_cast<alias_path::AliasListView*>(ptr(rd(exo_base() + 0xc))),
                                                nullptr, str(resolved.adopt()), str(directory), 0xffff); // 005E7870..005E7890
        s_assign(path.p(), resolved.p());                                            // 005E7895..005E78A2
    }                                                                                // 005E78A7..005E78B2
    const int last = static_cast<int>(async_string_length_005e5790(path.p(), nullptr)) - 1; // 005E78B7..005E78C0
    if (directory_list::At005e5600(str(path.p()), nullptr, last) == '\\')           // 005E78C1..005E78D1
        frame_LogFormat_005e5680(path.p(), "%s*.*", cstr(path.p()));                // 005E78D3..005E78F5
    else
        frame_LogFormat_005e5680(path.p(), "%s\\*.*", cstr(path.p()));              // 005E78E5..005E78F5
    alignas(4) unsigned char find[0x140];                                            // [esp+30h] WIN32_FIND_DATAA
    void* const handle = FindFirstFileA(cstr(path.p()), find);                       // 005E78FD..005E790C
    if (handle == reinterpret_cast<void*>(static_cast<std::intptr_t>(-1))) return 0; // 005E7912..005E7917, 005E79A5..
    U error = 0;                                                                     // ebx
    do {
        (void)frame_LogAssign_005e5140(name.p(), nullptr, reinterpret_cast<const char*>(find + 0x2c)); // 005E7920..005E7929 cFileName
        total += rd(word(find) + 0x20);                                              // 005E792E nFileSizeLow
        if (FindNextFileA(handle, find) == 0) error = GetLastError();                // 005E7932..005E7948
    } while (error != 0x12);                                                         // 005E794A..005E794D ERROR_NO_MORE_FILES
    (void)FindClose(handle);                                                         // 005E794F..005E7950
    return total;                                                                    // 005E798C
}
U __fastcall kotor_fastcall_005e6650(void* base, void*, void* directory) {
    return kotor_fastcall_005e7820(ptr(rd(word(base) + 0x14)), nullptr, directory);  // 005E6650..005E6653
}

// ===================================================================================== 0045D990 / 0045F2A0
void __cdecl kotor_cdecl_0045d990(unsigned char* data, I width, I height, I components) {
    const I size = width * height * components;                                      // 0045D9A3..0045D9A8
    unsigned char* const flipped = static_cast<unsigned char*>(live_operator_new(static_cast<U>(size))); // 0045D9AB..0045D9BA
    if (height > 0) {                                                                // 0045D9B8..0045D9BE
        const I row = width * components;                                            // 0045D9C8..0045D9CA
        const unsigned char* src = data;                                             // [esp+38h]
        unsigned char* dst = flipped + (height - 1) * width * components;            // 0045D9D1..0045D9E0
        for (I y = height; y != 0; --y) {                                            // 0045D9F2..0045DA4C
            if (width > 0) {                                                         // 0045D9F2..0045D9F4
                const unsigned char* s = src; unsigned char* d = dst;
                for (I x = width; x != 0; --x) {                                     // 0045DA04..0045DA2B
                    std::memcpy(d, s, static_cast<U>(components));
                    d += components; s += components;
                }
            }
            src += row;                                                              // 0045DA3D
            dst -= row;                                                              // 0045DA45
        }
    }
    std::memcpy(data, flipped, static_cast<U>(size));                                // 0045DA52..0045DA67
    live_free(flipped);                                                              // 0045DA66..0045DA69
}
void __cdecl kotor_cdecl_0045f2a0(const char* file, I width, I height, I components, const unsigned char* data) {
    unsigned char header[0x12];                                                      // [esp+18h]
    header[0] = 0; header[1] = 0; header[2] = 2;                                     // 0045F2C4..0045F2CC id, colour map, type
    header[3] = header[4] = header[5] = header[6] = header[7] = 0;                   // 0045F2D1..0045F2E1 colour map spec
    const std::uint16_t zero = 0, w = static_cast<std::uint16_t>(width), h = static_cast<std::uint16_t>(height);
    std::memcpy(header + 8, &zero, 2); std::memcpy(header + 10, &zero, 2);           // 0045F2E5..0045F2EA origin
    std::memcpy(header + 12, &w, 2); std::memcpy(header + 14, &h, 2);                // 0045F2EF..0045F2F4
    header[16] = static_cast<unsigned char>(static_cast<unsigned char>(components) << 3); // 0045F2B9..0045F2F9
    header[17] = 0;                                                                  // 0045F2FD
    std::FILE* const f = std::fopen(file, "wb");                                     // 0045F2BE..0045F301 (unchecked)
    (void)std::fwrite(header, 0x12, 1, f);                                           // 0045F306..0045F314
    const U size = static_cast<U>(width * height * components);                      // 0045F319..0045F31C
    unsigned char* const copy = static_cast<unsigned char*>(live_operator_new(size)); // 0045F31F..0045F320
    std::memcpy(copy, data, size);                                                   // 0045F325..0045F33F
    kotor_cdecl_0045d990(copy, width, height, components);                           // 0045F341..0045F34D
    image_tga_swap_bgr_to_rgb_0045da80(reinterpret_cast<char*>(copy), width, height, components); // 0045F352..0045F35A
    (void)std::fwrite(copy, 1, size, f);                                             // 0045F35F..0045F368
    live_free(copy);                                                                 // 0045F370..0045F371
    (void)std::fclose(f);                                                            // 0045F376..0045F377
}

// ===================================================================================== 00420F20
void __cdecl kotor_cdecl_00420f20(const char* name, I size, U square, U pad) {
    char file[0x40];                                                                 // [esp+28h] (to the return slot)
    U out_w = word(name);                                                            // argument slot [esp+6Ch]
    bool is_null = name == nullptr;                                                  // 00420F27..00420F2D
    if (!is_null) {                                                                  // 00420F2F..00420F3F repe cmpsb, 5 bytes
        U k = 0;
        while (k < 5 && name[k] == "NULL"[k]) ++k;
        is_null = k == 5;
    }
    if (is_null) {
        std::sprintf(file, "snap%d.tga", static_cast<int>(++kotor_closeB_snapshot_counter_007a47b0)); // 00420F9C..00420FB2
    } else {
        for (U k = 0;; ++k) { file[k] = name[k]; if (name[k] == 0) break; }           // 00420F41..00420F4F (byte copy)
        const I len = static_cast<I>(std::strlen(file));                             // 00420F51..00420F5F
        if (!(len > 3 && kotor_stricmp_0070acaf(file + len - 4, ".tga") == 0))        // 00420F61..00420F7A
            std::memcpy(file + len, ".tga", 5);                                      // 00420F7C..00420F97 (dword + NUL byte)
    }
    int viewport[4];                                                                 // [esp+18h]
    kotor_renderer_init::glGetIntegerv_0073d244(0xba2, viewport);                    // 00420FBA..00420FC4 GL_VIEWPORT
    I width = viewport[2];                                                           // esi
    const I height = viewport[3];                                                    // ebp
    const I rem = width % 4;                                                         // 00420FD2..00420FE7 (signed remainder)
    if (rem != 0) width = width - rem + 4;                                           // 00420FE8..00420FEF
    I out_h;                                                                         // ebx
    if (width > height) {                                                            // 00420FF3..00420FF5
        if (static_cast<unsigned char>(square) != 0) {                               // 00420FF7..00420FFD
            out_h = size; out_w = static_cast<U>(size);                              // 00420FFF..00421003
        } else {
            out_w = static_cast<U>(size);                                            // 0042100D..00421011
            out_h = static_cast<I>(w2::ftol2_006fae8c(                                // 00421009..00421022 fild/fimul/fidiv/__ftol2
                static_cast<double>(height) * static_cast<double>(size) / static_cast<double>(width)));
        }
    } else {
        out_h = static_cast<I>(square);                                              // 00421026 (full dword; out_w keeps the name slot)
    }
    void* const gob = find_gob_00440460("camera");                                   // 0042102A..00421034
    const U camera = gob != nullptr ? word(gob) - 4 : 0;                             // 00421037..00421040
    kotor_renderer_init::glClear_0073d21c(0x4500);                                   // 00421042..00421047
    kotor_renderer_init::glLoadIdentity_0073d230();                                  // 0042104D
    kotor_closeB_snapshot_render_007b9350 = 1;                                       // 00421053
    vcall<void>(camera, 8);                                                          // 0042105D..00421061 (NULL camera faults as the original)
    const U bytes = static_cast<U>(height * width * 3);                              // 00421064..00421069
    kotor_closeB_snapshot_render_007b9350 = 0;                                       // 0042106D
    unsigned char* const pixels = static_cast<unsigned char*>(live_operator_new(bytes)); // 0042106C..00421077
    std::memset(pixels, 0, bytes);                                                   // 0042107C..00421099
    kotor_renderer_init::glReadPixels_0073d250(0, 0, width, height, k_gl_rgb_0073f370, 0x1401, pixels); // 0042108E..004210AB
    kotor_cdecl_0045d990(pixels, width, height, 3);                                  // 004210B1..004210B6
    unsigned char* const scaled = static_cast<unsigned char*>(
        image_scale_0045dad0(pixels, width, height, 3, static_cast<I>(out_w), out_h)); // 004210BB..004210C6
    if (static_cast<unsigned char>(square) == 0 && static_cast<unsigned char>(pad) != 0) { // 004210CD..004210E9
        const I side = static_cast<I>(out_w) > out_h ? static_cast<I>(out_w) : out_h;     // 004210EB..004210FD
        const U side_bytes = static_cast<U>(side * side * 3);                        // 004210FF..00421102
        unsigned char* const padded = static_cast<unsigned char*>(live_operator_new(side_bytes)); // 00421105..00421106
        std::memset(padded, 0x7f, side_bytes);                                       // 00421110..00421129
        std::memcpy(padded, scaled, static_cast<U>(out_h * static_cast<I>(out_w) * 3)); // 0042110B..00421144
        kotor_cdecl_0045f2a0(file, side, side, 3, padded);                           // 00421137..00421150
        live_free(padded);                                                           // 00421155..0042115A
    } else {
        kotor_cdecl_0045f2a0(file, static_cast<I>(out_w), out_h, 3, scaled);         // 00421168..00421176
    }
    live_free(scaled);                                                               // 0042117E..0042117F
    live_free(pixels);                                                               // 00421184..00421185
}

// ===================================================================================== 00401080
void __fastcall kotor_fastcall_00401080(void*, void*, void* name) {
    kotor_cdecl_00420f20(cstr(name), 0x100, 1, 1);                                   // 00401080..00401093
}

// ===================================================================================== 00638E80
void __fastcall kotor_fastcall_00638e80(void* camera, void*) {
    const U s = word(camera);
    const U target = rd(s + 0x14);                                                   // 00638E8C ebx
    V3 own_buf, target_buf;                                                          // [esp+18h], [esp+28h]
    const V3* const own = vcall<const V3*>(rd(s + 0x10), 0x64, &own_buf);            // 00638E87..00638E95 GetPosition
    const V3* const at = vcall<const V3*>(target, 0x64, &target_buf);                // 00638E98..00638EA3
    V3 d;                                                                            // [esp+0Ch]
    d.z = static_cast<float>(static_cast<double>(at->z) - static_cast<double>(own->z)); // 00638EA6..00638EA9
    d.y = static_cast<float>(static_cast<double>(at->y) - static_cast<double>(own->y)); // 00638EB0..00638EB4
    d.x = static_cast<float>(static_cast<double>(at->x) - static_cast<double>(own->x)); // 00638EB7..00638EC3
    (void)kotor_m15a_pitch_004aa130(&d);                                             // 00638EC7..00638ECC (result popped)
    d.z = 0.0f;                                                                      // 00638ED5
    const double len2 = static_cast<double>(d.x) * d.x + static_cast<double>(d.y) * d.y; // 00638ED1..00638EE9
    if (!(len2 > 0.0)) return;                                                       // 00638EEB..00638EF6 (test ah,41h: <=, NaN exit)
    V3 n;                                                                            // [esp+28h]
    const float yaw = static_cast<float>(kotor_m15a_yaw_004aa0f0(normalize_004ab180(&n, &d))); // 00638F01..00638F16
    Q4 q;                                                                            // [esp+18h]
    const Q4 value = *static_cast<const Q4*>(yaw_pitch_roll_004acac0(&q, yaw, flt(rd(s + 0x11c)), 0.0f)); // 00638EF8..00638F22
    Q4 scratch;                                                                      // [esp+34h] (pointer argument)
    vcall<void>(rd(s + 0x10), 0x60, &scratch, value);                                // 00638F27..00638F4C SetOrientation
}

// ===================================================================================== 005665C0
void __fastcall kotor_fastcall_005665c0(void* table, void*, void* directory, U members) {
    if (members != 0) {                                                              // 005665C0..005665C9
        for (I i = 0; i < 9; ++i) kotor_fastcall_00563e80(table, nullptr, i, 0);     // 005665CC..005665DE
        kotor_fastcall_00564030(table, nullptr, 0);                                  // 005665E0..005665E4
    }
    kotor_fastcall_005648c0(table, nullptr, directory);                              // 005665EA..005665F1
}

// ===================================================================================== 004C3C70
void __fastcall kotor_fastcall_004c3c70(void*, void*, void* creature, void* directory) {
    const U c = word(creature);
    void* const player = kotor_fastcall_004ae910(server(), nullptr, rd(c + 4));     // 004C3C8E..004C3CA1 [esp+10h]
    void* const gff = new_gff();                                                     // 004C3CA6..004C3CCC esi
    void* const root = live_operator_new(4);                                         // 004C3CCE..004C3CD8 edi (leaked)
    U created;                                                                       // [esp+14h]
    {
        Scoped version; version.text("V2.0");                                        // 004C3CE0..004C3CEB [esp+28h]
        Scoped type; type.text("BIC ");                                              // 004C3CF0..004C3D01 [esp+20h]
        created = recovered_gff_00411260(gff, nullptr, root, type.p(), version.p()); // 004C3D06..004C3D18
    }                                                                                // 004C3D1D..004C3D3B
    if (created != 0) {                                                              // 004C3D40..004C3D46
        Scoped path; path.empty();                                                   // [esp+18h]
        {
            Scoped name;                                                             // [esp+38h]
            void* const text = kotor_fastcall_005610b0(player, nullptr, name.adopt()); // 004C3D55..004C3D66
            (void)recovered_gff_00412970(gff, nullptr, root, text, "Mod_CommntyName"); // 004C3D6B..004C3D79
        }                                                                            // 004C3D7E..004C3D87
        (void)recovered_gff_00412620(gff, nullptr, root, rd(word(player) + 0x3c), "Mod_IsPrimaryPlr"); // 004C3D8C..004C3D9C
        (void)recovered_gff_00412760(gff, nullptr, root, rd(c + 4), "ObjectId");     // 004C3DA1..004C3DAD
        (void)recovered_save_creature_00500610(creature, nullptr, gff, root);        // 004C3DB2..004C3DB6
        {
            Scoped name; name.text("Player");                                        // 004C3DBB..004C3DC4 [esp+38h]
            Scoped sep; sep.text("\\");                                              // 004C3DC9..004C3DD7 [esp+30h]
            Scoped first; s_concat(directory, first.adopt(), sep.p());               // 004C3DDC..004C3DEE [esp+48h]
            Scoped second; s_concat(first.p(), second.adopt(), name.p());            // 004C3DF3..004C3E05 [esp+40h]
            s_assign(path.p(), second.p());                                          // 004C3E0A..004C3E14
            second.end(); first.end(); sep.end(); name.end();                        // 004C3E19..004C3E4B
        }
        (void)recovered_gff_00413030(gff, nullptr, path.p(), 0x7df);                 // 004C3E50..004C3E5C
    }                                                                                // 004C3E61..004C3E6D
    gff_delete(gff);                                                                 // 004C3E72..004C3E7C
}

// ===================================================================================== 004B3000
U __fastcall kotor_fastcall_004b3000(void* internal, void*) {
    const U s = word(internal);
    void* pos = ptr(rd(rd(rd(s + 0x10064))));                                        // 004B3004..004B3010 [esp+4]
    if (pos != nullptr) {                                                            // 004B300E..004B3014
        do {
            const U player = unresolved_list_at_pos_005e9b60(ptr(rd(rd(s + 0x10064))), nullptr, pos); // 004B3016..004B301F GetAtPos
            const unsigned char state = rd8(player + 0x64);                          // 004B3024
            if (state == 1)                                                          // 004B3027..004B302A
                (void)kotor_fastcall_0056cac0(ptr(rd(s + 0x10010)), nullptr, ptr(player)); // 004B303F..004B3046
            else if (state > 2 && state <= 4)                                        // 004B302C..004B3034
                (void)kotor_fastcall_005624c0(ptr(player), nullptr);                 // 004B3036..004B3038
            (void)unresolved_list_next_005e9b90(ptr(rd(rd(s + 0x10064))), nullptr, &pos); // 004B304B..004B3058 GetNext
        } while (pos != nullptr);                                                    // 004B305D..004B3063
    }
    return 1;                                                                        // 004B3065
}

// ===================================================================================== 004B58A0
U __fastcall kotor_fastcall_004b58a0(void* internal, void*, U number, void* name, U player, U) {
    const U s = word(internal);
    Scoped path; path.empty();                                                       // [esp+10h]
    Scoped second; second.empty();                                                   // [esp+18h]
    frame_R4_005d5e80(kotor_cell_sound_007a39ec, nullptr, k_sound_mode_0074c5f8);    // 004B58DB..004B58EC
    {
        Scoped saves; saves.text("SAVES:");                                          // 004B58F1..004B58FA [esp+20h]
        (void)gui_load_resman_create_directory_004068c0(resman(), nullptr, saves.p()); // 004B58FF..004B590F
    }                                                                                // 004B5914..004B591D
    frame_LogFormat_005e5680(path.p(), "SAVES:%06d - %s", number, cstr(name));       // 004B5922..004B5939
    std::uint64_t free_bytes = 0;                                                    // [esp+28h]
    U have_space;
    {
        Scoped saves; saves.text("SAVES:");                                          // 004B5941..004B594A [esp+20h]
        have_space = gui_load_resman_free_disk_space_004067b0(resman(), nullptr, saves.p(), &free_bytes); // 004B594F..004B5964
    }                                                                                // 004B5969..004B5974
    if (have_space == 1) {                                                           // 004B5979..004B597C
        const U needed = rd(s + 0x100c8);                                            // 004B5984 esi
        const std::uint64_t total = free_bytes +
            static_cast<std::uint64_t>(kotor_fastcall_005e6650(ptr(exo_base()), nullptr, path.p())); // 004B597E..004B599C
        if ((total >> 32) == 0 && needed > static_cast<U>(total)) {                  // 004B59A0..004B59A8
            (void)kotor_fastcall_0056cb80(ptr(rd(s + 0x10010)), nullptr, player, 2, 1); // 004B59AA..004B59B8
            return 0;                                                                // 004B59BD..004B59EE
        }
    }
    if (gui_load_resman_create_directory_004068c0(resman(), nullptr, path.p()) == 0 && // 004B59F1..004B5A03
        gui_load_resman_clean_directory_00409460(resman(), nullptr, path.p(), 0, 0) == 0) // 004B5A05..004B5A19
        return 0;                                                                    // 004B59BD..004B59EE
    {
        Scoped game; game.text("SAVEGAME");                                          // 004B5A1B..004B5A24 [esp+30h]
        Scoped sep; sep.text("\\");                                                  // 004B5A29..004B5A37 [esp+20h]
        Scoped first; s_concat(path.p(), first.adopt(), sep.p());                    // 004B5A3C..004B5A4F [esp+40h]
        Scoped joined; s_concat(first.p(), joined.adopt(), game.p());                // 004B5A54..004B5A65 [esp+38h]
        s_assign(second.p(), joined.p());                                            // 004B5A6A..004B5A74
        joined.end(); first.end(); sep.end(); game.end();                            // 004B5A79..004B5AAC
    }
    const U state = rd(app() + 0x14);                                                // 004B5AB1..004B5AB6
    wr(state, 1); wr(state + 4, 2); wr(state + 0x10, 1); wr(state + 0x38, 0);        // 004B5AB9..004B5ACD
    s_assign(ptr(rd(app() + 0x14) + 0x18), path.p());                                // 004B5AD0..004B5AE1
    s_assign(ptr(rd(app() + 0x14) + 0x20), second.p());                              // 004B5AE6..004B5AF6
    wr(rd(app() + 0x14) + 8, 0);                                                     // 004B5AFB..004B5B04
    wr(rd(app() + 0x14) + 0xc, 0);                                                   // 004B5B07..004B5B0F
    (void)kotor_fastcall_0056c8b0(sws_message(), nullptr, 2);                        // 004B5B12..004B5B24
    {
        Scoped alias; alias.text("SAVES");                                           // 004B5B29..004B5B32 [esp+30h]
        s_assign(path.p(), kotor_fastcall_005e6890(ptr(rd(exo_base() + 0xc)), nullptr, alias.p(), 0)); // 004B5B37..004B5B54
    }                                                                                // 004B5B59..004B5B62
    if (client_movies::StringNotEqual_005e5390(reinterpret_cast<options_video::String*>(path.p()), nullptr, "") != 0) { // 004B5B67..004B5B77
        const U cameras = rd(word(kotor_fastcall_005ed530(client(), nullptr)) + 0x40); // 004B5B79..004B5B87
        void* const camera = vcall<void*>(cameras, 0x80, 0x106au);                   // 004B5B8A..004B5B91
        if (camera != nullptr) kotor_fastcall_00638e80(camera, nullptr);             // 004B5B97..004B5B9D
        frame_LogFormat_005e5680(second.p(), "%s%06d - %s%sScreen", cstr(path.p()), number, cstr(name), "\\"); // 004B5BA2..004B5BC8
        kotor_fastcall_00401080(ptr(app()), nullptr, second.p());                    // 004B5BCD..004B5BDB
    }
    if (number != 0) draw_and_present();                                             // 004B5BE0..004B5C0A
    wr(s + 0x1b930, 0);                                                              // 004B5C11
    wr(s + 0x1b938, 0);                                                              // 004B5C17
    return 1;                                                                        // 004B5C1D..004B5C4D
}

// ===================================================================================== 004B3110
void __fastcall kotor_fastcall_004b3110(void* internal, void*) {
    const U s = word(internal);
    Scoped module; module.empty();                                                   // [esp+28h]
    Scoped area_name; area_name.empty();                                             // [esp+58h]
    Scoped nfo_path; nfo_path.empty();                                               // [esp+50h]
    Scoped portrait; portrait.empty();                                               // [esp+48h]
    s_assign(module.p(), ptr(word(recovered_server_get_module_004b14f0(internal, nullptr)) + 0x5c)); // 004B3173..004B318A
    const int colon = find_005e5f20(module.p(), nullptr, 0x3a, 0);                   // 004B318F..004B3197
    if (colon != -1) {                                                               // 004B319C..004B31A1
        const int n = static_cast<int>(async_string_length_005e5790(module.p(), nullptr)) - colon - 1; // 004B31A3..004B31AE
        Scoped right;                                                                // [esp+60h]
        (void)base_ini_reconstruction::right_005e6170(str(module.p()), nullptr, str(right.adopt()), n); // 004B31AF..004B31B9
        s_assign(module.p(), right.p());                                             // 004B31BE..004B31CB
    }                                                                                // 004B31D0..004B31DC
    kotor_m15_client_set_load_screen_by_module_005edcf0(client(), nullptr, module.p()); // 004B31E1..004B31EF
    {
        void* const c = client();                                                    // 004B31F4..004B31F9 esi
        const U hint = load_screen_get_next_load_screen_hint_strref_005ee090(c, nullptr, module.p(), 0); // 004B31FC..004B3205
        kotor_m15_client_set_load_screen_hint_005edf00(c, nullptr, hint);            // 004B320A..004B320D
    }
    kotor_m15_client_show_load_screen_005eda80(client(), nullptr, 0, 1, 1);          // 004B3212..004B3221
    progress(0);                                                                     // 004B3226..004B3232
    draw_and_present();                                                              // 004B3237..004B325A
    (void)kotor_input_required::low_resolution_timer_005e9250(ptr(rd(exo_base() + 4)), nullptr); // 004B325F..004B326B
    const U state = rd(app() + 0x14);                                                // 004B3270..004B327C [esp+24h]
    Scoped directory; directory.empty();                                             // [esp+30h]
    Scoped archive; archive.empty();                                                 // [esp+38h]
    s_assign(directory.p(), ptr(state + 0x18));                                      // 004B3296..004B32A6
    s_assign(archive.p(), ptr(state + 0x20));                                        // 004B32AB..004B32B3
    (void)kotor_fastcall_0056c850(sws_message(), nullptr, 0);                        // 004B32B8..004B32CA
    (void)kotor_fastcall_004b2e70(internal, nullptr);                                // 004B32CF..004B32D1
    progress(5);                                                                     // 004B32D6..004B32E2
    present();                                                                       // 004B32E7..004B32F0
    void* const party_table = ptr(s + 0x1b770);                                      // 004B32FE ebx
    kotor_fastcall_005665c0(party_table, nullptr, directory.p(), 1);                 // 004B32F8..004B3307
    progress(10);                                                                    // 004B330C..004B3319
    present();                                                                       // 004B331E..004B3327
    kotor_fastcall_0052ad10(ptr(s + 0x100fc), nullptr, directory.p());               // 004B332F..004B333A
    progress(0xf);                                                                   // 004B333F..004B334C
    present();                                                                       // 004B3351..004B335A
    void* const srv = server();                                                      // 004B335F..004B3365 esi
    void* const creature = kotor_fastcall_004ae770(srv, nullptr, kotor_fastcall_004aea40(srv, nullptr)); // 004B336B..004B337A ebp
    const U player = word(kotor_fastcall_004b2a50(internal, nullptr, rd(word(creature) + 4))); // 004B337C..004B3387 [esp+20h]
    if (kotor_closeB_primary_player_save_00831fe0 != 0)                              // 004B338B..004B3392
        kotor_fastcall_004c3c70(recovered_server_get_module_004b14f0(internal, nullptr), nullptr, creature, directory.p()); // 004B3394..004B33A3
    progress(0x14);                                                                  // 004B33A8..004B33B5
    present();                                                                       // 004B33BA..004B33C3
    {
        Scoped name; name.text("savenfo");                                           // 004B33CB..004B33D4 [esp+60h]
        Scoped sep; sep.text("\\");                                                  // 004B33D9..004B33EA [esp+18h]
        Scoped first; s_concat(directory.p(), first.adopt(), sep.p());               // 004B33EF..004B3405 [esp+78h]
        Scoped joined; s_concat(first.p(), joined.adopt(), name.p());                // 004B340A..004B341E [esp+68h]
        s_assign(nfo_path.p(), joined.p());                                          // 004B3423..004B3430
        joined.end(); first.end(); sep.end(); name.end();                            // 004B3435..004B3474
    }
    void* const gff = new_gff();                                                     // 004B3479..004B34A1 esi
    U root = 0;                                                                      // [esp+14h] CResStruct
    {
        Scoped version; version.text("V2.0");                                        // 004B34A3..004B34B4 [esp+18h]
        Scoped type; type.text("NFO ");                                              // 004B34B9..004B34CA [esp+60h]
        (void)recovered_gff_00411260(gff, nullptr, &root, type.p(), version.p());    // 004B34CF..004B34E8
    }                                                                                // 004B34ED..004B350A
    U slot20 = rd(player + 8);                                                       // 004B350F..004B3516 [esp+20h] client language
    void* const area = recovered_object_area_004cb120(creature, nullptr);            // 004B351A..004B351C
    (void)exo_locstring_get_005ea130(ptr(word(area) + 0x150), nullptr, slot20, area_name.p(), 0); // 004B3521..004B3533
    (void)recovered_gff_00412970(gff, nullptr, &root, area_name.p(), "AREANAME");    // 004B3538..004B3549
    (void)recovered_gff_00412970(gff, nullptr, &root, module.p(), "LASTMODULE");     // 004B354E..004B355F
    (void)recovered_gff_00412760(gff, nullptr, &root, kotor_fastcall_00563bf0(party_table, nullptr), "TIMEPLAYED"); // 004B3564..004B3578
    (void)recovered_gff_00412620(gff, nullptr, &root, rd(s + 0x1b904), "CHEATUSED"); // 004B357D..004B3590
    (void)recovered_gff_00412970(gff, nullptr, &root, ptr(s + 0x1b93c), "SAVEGAMENAME"); // 004B3595..004B35A8
    (void)recovered_gff_00412620(gff, nullptr, &root, kotor_fastcall_005ee050(client(), nullptr), "GAMEPLAYHINT"); // 004B35AD..004B35C7
    (void)recovered_gff_00412620(gff, nullptr, &root, kotor_fastcall_005ee070(client(), nullptr), "STORYHINT");    // 004B35CC..004B35E7
    void* const party = kotor_fastcall_005ed8b0(client(), nullptr);                  // 004B35EC..004B35F5 [esp+18h]
    unsigned char mask = 0;                                                          // bl
    slot20 &= 0xffffff00u;                                                           // 004B3604 byte [esp+20h] = 0
    Scoped live; live.empty();                                                       // [esp+40h]
    if (k_live_slots_0073ecd2 > 1) {                                                 // 004B360D..004B3621
        for (U i = 1; i < k_live_slots_0073ecd2; ++i) {                              // 004B3623..004B368F
            frame_LogFormat_005e5680(live.p(), "LIVE%d", i);                         // 004B3623..004B362E
            void* const content = kotor_fastcall_005ee020(client(), nullptr, static_cast<I>(i)); // 004B3633..004B364B
            (void)recovered_gff_00412970(gff, nullptr, &root, content, cstr(live.p())); // 004B363E..004B3658
            void* const resolved = kotor_fastcall_005e6890(ptr(rd(exo_base() + 0xc)), nullptr, live.p(), 0); // 004B365D..004B366C
            if (static_cast<I>(async_string_length_005e5790(resolved, nullptr)) > 0) // 004B3671..004B367A
                mask = static_cast<unsigned char>(mask | (1u << (i - 1)));           // 004B367C..004B3683
        }
        slot20 = (slot20 & 0xffffff00u) | mask;                                      // 004B3691
    }
    (void)recovered_gff_00412620(gff, nullptr, &root, slot20, "LIVECONTENT");        // 004B3695..004B36A6 (full dword)
    for (I i = 0; i < static_cast<I>(rd(word(party))); ++i) {                        // 004B36AB..004B3742
        void* const member = kotor_m15a_creature_get_server_creature_0060fb20(
            frame_GetCharacter_006346c0(party, nullptr, static_cast<U>(i)), nullptr); // 004B36C0..004B36CC
        if (member == nullptr) continue;                                             // 004B36D1..004B36D5
        frame_LogFormat_005e5680(portrait.p(), "%s%d", "PORTRAIT", i);               // 004B36D7..004B36E7
        void* const client_creature = recovered_creature_client_creature_004efb20(member, nullptr); // 004B36EF..004B36F1
        const char* const label = cstr(portrait.p());                                // 004B36F6..004B36FC
        alignas(4) unsigned char resref[0x10];                                       // [esp+78h] / [esp+68h]
        void* ref;
        if (client_creature != nullptr)                                              // 004B3701..004B3704
            ref = vcall<void*>(word(client_creature), 0x148, static_cast<void*>(resref), 1u); // 004B3706..004B3714
        else
            ref = vcall<void*>(word(member), 0xc8, static_cast<void*>(resref));      // 004B371C..004B3726
        (void)recovered_gff_004128c0(gff, nullptr, &root, ref, label);               // 004B372C..004B3734
    }
    {
        Scoped file; file.text(cstr(nfo_path.p()));                                  // 004B3748..004B3756 [esp+60h]
        (void)recovered_gff_00413030(gff, nullptr, file.p(), 0);                     // 004B375B..004B376D
    }                                                                                // 004B3772..004B377E
    gff_delete(gff);                                                                 // 004B3783..004B3791
    progress(0x19);                                                                  // 004B3793..004B379E
    present();                                                                       // 004B37A3..004B37AA
    void* const memory = live_operator_new(0xd0);                                    // 004B37AF..004B37B4
    void* const erf = memory != nullptr ? cerf_construct_cerf_file_005dd9c0(memory) : nullptr; // 004B37BC..004B37D7 esi
    (void)kotor_fastcall_005dcfc0(erf, nullptr, archive.p());                        // 004B37D9..004B37E8
    kotor_fastcall_005dce30(erf, nullptr, "MOD V1.0");                               // 004B37ED..004B37F4
    (void)kotor_fastcall_005dd080(erf, nullptr);                                     // 004B37F9..004B37FB
    (void)kotor_fastcall_005dd5a0(erf, nullptr);                                     // 004B3800..004B3802
    progress(0x1e);                                                                  // 004B3807..004B3813
    present();                                                                       // 004B3818..004B381F
    {
        Scoped in_progress; in_progress.text("GAMEINPROGRESS:");                     // 004B3827..004B3830 [esp+60h]
        kotor_fastcall_005de1a0(erf, nullptr, in_progress.p(), 0xffffffffu,
                                reinterpret_cast<void*>(&kotor_cdecl_004b30d0), 0x1e, 0x3c); // 004B3835..004B384F
    }                                                                                // 004B3854..004B3860
    (void)kotor_fastcall_005dd0e0(erf, nullptr);                                     // 004B3865..004B3867
    (void)cerf_read_cerf_file_005dce50(erf, nullptr, archive.p());                   // 004B386C..004B3873
    (void)cerf_read_header_variance_005dd3c0(erf, nullptr, archive.p());             // 004B3878..004B387F
    if (erf != nullptr) {                                                            // 004B3884..004B3886
        cerf_destroy_cerf_file_005de3a0(erf);                                        // 004B3888..004B388A
        live_free(erf);                                                              // 004B388F..004B3890
    }
    (void)kotor_input_required::low_resolution_timer_005e9250(ptr(rd(exo_base() + 4)), nullptr); // 004B3898..004B38A0
    progress(100);                                                                   // 004B38A5..004B38B1
    present();                                                                       // 004B38B6..004B38BD
    frame_R4_005d5e80(kotor_cell_sound_007a39ec, nullptr, k_sound_mode_0074c5ec);    // 004B38C2..004B38D2
    kotor_m15_client_hide_load_screen_005eda90(client(), nullptr, 0, 0);             // 004B38D7..004B38E1
    if (rd(word(frame_R4_005ed690(client(), nullptr)) + 0x30) == 0) {                // 004B38E6..004B38F7
        if (rd(word(frame_R4_005ed690(client(), nullptr)) + 0xb4) != 0) {            // 004B38F9..004B390D
            wr(word(frame_R4_005ed690(client(), nullptr)) + 0xb98, 1);               // 004B390F..004B391C
        } else {
            const U color[3] = {0, 0, 0};                                            // [esp+78h]
            kotor_m15_ingame_start_global_fade_0062abf0(frame_R4_005ed690(client(), nullptr), nullptr,
                                                        1, 0x3f000000u, 0x3f800000u, color); // 004B3924..004B3965
        }
    }
    kotor_fastcall_005edc80(client(), nullptr, 1);                                   // 004B396A..004B3973
    kotor_fastcall_005ed310(const_cast<void*>(kotor_menu_window_cells::input_007a39e4), nullptr); // 004B3978..004B397E
    kotor_game_prepare::SetGlobalUnpauseableState0062b410(frame_R4_005ed690(client(), nullptr), nullptr, 0); // 004B3983..004B3994
    kotor_fastcall_00604d70(ptr(rd(word(kotor_fastcall_005ed530(client(), nullptr)) + 0x48)), nullptr); // 004B3999..004B39AA
    if (rd(state + 0x14) != 1) {                                                     // 004B39AF..004B39B6
        wr(state + 0x14, 1);                                                         // 004B39B8
        if (kotor_rules_cell8_007a39dc == 2) { wr(state + 0x14, 0); wr(state, 0); wr(state + 4, 0); } // 004B39BB..004B39C9
        (void)kotor_fastcall_0056c850(sws_message(), nullptr, 1);                    // 004B39CC..004B39DC
        (void)kotor_fastcall_0056c980(sws_message(), nullptr, 2, 0);                 // 004B39E1..004B39F4
    }
}   // 004B39F9..004B3A6E: ~live, ~archive, ~directory, ~portrait, ~nfo_path, ~area_name, ~module
}  // extern "C"
