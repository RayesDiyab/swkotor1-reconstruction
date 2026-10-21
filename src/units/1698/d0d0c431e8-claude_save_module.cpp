// claude_save_module.cpp (lane worker save-module): CServerExoAppInternal::StoreCurrentModule 004B2E70 (W1 router case,
// called by StartNewModule when Endar Spire is left for Taris) and the module half of its save cascade, transcribed from
// the original swkotor.exe (read-only):
//   004B2E70 CServerExoAppInternal::StoreCurrentModule()                   RET 0, EAX 1   kotor_fastcall_004b2e70
//   004C4B80 CSWSModule::GetModuleResourceName(CExoString* out)            RET 4, EAX out kotor_fastcall_004c4b80
//   004C8960 CSWSModule::SaveModuleStart(CExoString const& prefix, CExoString const& path) RET 8 kotor_fastcall_004c8960
//   004C7050 CSWSModule::SaveModuleIFOStart(CResGFF*, CResStruct*)         RET 8, EAX 1   kotor_fastcall_004c7050
//   004C5BB0 CSWSModule::SaveLimboCreatures(CResGFF*, CResStruct*)         RET 8          kotor_fastcall_004c5bb0
//   004C3960 CSWSModule::SaveModuleFAC()                                   RET 0          kotor_fastcall_004c3960
//   004C3B10 CSWSModule::SaveModuleInProgress()                            RET 0, EAX 0/1 kotor_fastcall_004c3b10
//   004CA680 CSWSModule::SaveModuleFinish(CExoString const& prefix, CExoString const& path) RET 8, EAX 0/1 kotor_fastcall_004ca680
//   004C5980 CSWSModule::SaveStatic(CERFFile*, CExoString const& ext, ushort type, int gff) RET 10h kotor_fastcall_004c5980
//   004C8B90 CSWSModule::SaveModuleIFOFinish(CResGFF*, CResStruct*, CERFFile*, CExoString const&, CExoArrayList<ulong>*)
//                                                                          RET 14h, EAX 1 kotor_fastcall_004c8b90
//   004C7870 CSWSModule::SavePlayers(CResGFF*, CResStruct*, CExoString const&, CExoArrayList<ulong>*) RET 10h kotor_fastcall_004c7870
//   004C5C70 CSWSModule::GetPlayerIndexInPlayerList(CSWSPlayer*)           RET 4          kotor_fastcall_004c5c70
//   004B0970 CServerAIMaster::SaveEventQueue(CResGFF*, CResStruct*)        RET 8, EAX 1   kotor_fastcall_004b0970
//   004AFEA0 CServerAIEventNode::SaveNode(CResGFF*, CResStruct*)           RET 8, EAX 1   kotor_fastcall_004afea0
//   004D64E0 CSWCCMessageData::SaveData(CResGFF*, CResStruct*)             RET 8, EAX 1   kotor_fastcall_004d64e0
//   004D73A0 CScriptEvent::SaveEvent(CResGFF*, CResStruct*)                RET 8, EAX 1   kotor_fastcall_004d73a0
//   005034F0 CSWSForcedAction::SaveForcedAction(CResGFF*, CResStruct*)     RET 8, EAX 1   kotor_fastcall_005034f0
//   004AF160 CServerAIBodyBagInfo::SaveBodyBag(CResGFF*, CResStruct*)      RET 8, EAX 1   kotor_fastcall_004af160
//   004EE230 CSWSSpellScriptData::SaveData(CResGFF*, CResStruct*)          RET 8, EAX 1   kotor_fastcall_004ee230
//   0052B790 CFactionManager::SaveFactions(CResGFF*, CResList*)            RET 8, EAX 1   kotor_fastcall_0052b790
//   0052B830 CFactionManager::SaveReputations(CResGFF*, CResList*)         RET 8, EAX 1   kotor_fastcall_0052b830
//   005624C0 CSWSPlayer::SaveServerCharacter()                             RET 0, EAX 0/1 kotor_fastcall_005624c0
//   005610B0 CSWSPlayer::GetPlayerName(CExoString* out)                    RET 4, EAX out kotor_fastcall_005610b0
//   004EF640 CSWSCreature::DumpToLog()                                     RET 0          kotor_fastcall_004ef640
//   0056C910 CSWSMessage::SendServerToPlayerLoadBar_UpdateStallEvent(ulong, ulong) RET 8 kotor_fastcall_0056c910
//   0062AD30 CGuiInGame::GetPlaceableCamera(int index)                     RET 4          kotor_fastcall_0062ad30
// Callees owned by the save-gff lane (claude_save_gff.cpp, research\SAVE-GFF-EXPORTS.txt) and the save-git lane
// (0050BA00 CSWSArea::SaveGIT) are declared with their original ABI below.
// Conventions (W5): thiscall -> extern "C" __fastcall(self, unused_edx, ...); live_operator_new / live_free only;
// FLOAT fields pass the stored dword bits; temporary CExoStrings are destroyed in the original order and scope guards
// stand in for the original EH states while unwinding.
#include "842d6b1618-w5_t3.hpp"

namespace kotor_reconstruction { struct StartupString; struct ArrayMutations; }
kotor_reconstruction::StartupString* __fastcall GuiConcat005e5d10(kotor_reconstruction::StartupString* self, void*,
    kotor_reconstruction::StartupString* out, const kotor_reconstruction::StartupString* other); // CExoString::operator+ [0093]
namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); } // CExoString() [menu20_bindings]
namespace base_ini_reconstruction {   // 005E6170 CExoString::Right(CExoString* out, int count) [0126]
kotor_reconstruction::StartupString* __fastcall right_005e6170(kotor_reconstruction::StartupString*, void*,
                                                                kotor_reconstruction::StartupString*, int);
}
namespace startup_rand {              // 005E6270 CExoString::SubString(CExoString* out, int start, int count) [0147]
kotor_reconstruction::StartupString* __fastcall SubString005e6270(kotor_reconstruction::StartupString*, void*,
                                                                   kotor_reconstruction::StartupString*, int, int);
}
namespace alias_path { struct AliasListView; }
namespace path_resolution {           // 005E68A0 CExoAliasList::ResolveFileName(CExoString* out, CExoString const&, ushort) [0241]
kotor_reconstruction::StartupString* __fastcall resolve_005e68a0(alias_path::AliasListView*, void*, kotor_reconstruction::StartupString*,
                                                                  kotor_reconstruction::StartupString*, unsigned);
}
namespace f200_ports {                // 005E6640 CExoBase::GetDirectoryList(list, CExoString const&, ushort, int, int) [0288]
void __fastcall Enumerate005e6640(void*, void*, void*, kotor_reconstruction::StartupString*, unsigned, unsigned, unsigned);
}
namespace options_video { struct String { char* text; unsigned length; }; }                 // CExoString passed by value
namespace client_intro {              // 004B2640 CExoArrayList<CExoString>::count(CExoString by value; callee destroys) [0371]
int __fastcall CountStrings_004b2640(void*, void*, options_video::String);
}
namespace kotor_runtime_reporter_dependencies { char* __cdecl string_copy_n_006fab80(char*, const char*, unsigned); } // _strncpy [0722]
namespace kotor_sound_internal {      // 005E95E0 CExoStringList::GetString(int) / 005E9860 ~CExoStringList [0671]
void* __fastcall StringListGet005e95e0(void*, void*, unsigned);
void __fastcall StringListDestroy005e9860(void*, void*);
}
namespace menu_list_growth { void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*, void*, unsigned int); } // [M41]
#ifndef W1R_MENU_RESOURCES_REF
#define W1R_MENU_RESOURCES_REF
namespace menu_resources { struct Ref { unsigned char bytes[16]; Ref* __thiscall construct_00406d80(const char* text); }; } // CResRef(char*) [0030]
#endif
namespace menu_resources { struct Manager; }
namespace kotor_resource_demand { extern menu_resources::Manager* volatile resource_manager_007a39e8; } // [007A39E8] CExoResMan*
namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } }       // [007A39E0] CExoBase* [0068]
namespace frame_control { extern unsigned message_manager_007a3a00; }                       // [007A3A00] g_pVirtualMachine [0456]

namespace save_module {
using namespace w5;

extern "C" {
// ---------------- imports: existing providers (c14 candidate map; owner object in brackets) ----------------
extern void* kotor_cell_tlk_007a3a08;                                                          // [007A3A08] CTlkTable* [0384]
extern U recovered_saved_state_007a1b40;                                                       // [007A1B40] CGameEffect next id lo [0886]
extern U recovered_saved_state_007a1b44;                                                       // [007A1B44] CGameEffect next id hi [0886]
__declspec(dllimport) int __stdcall DeleteFileA(const char* name);                            // [0073D198]
void* __fastcall WindowCopy005e5b00(void* self, void*, void* other);                           // CExoString(CExoString const&) [0078]
void* __fastcall frame_LogAssign_005e5140(void* self, void*, const char* text);                // CExoString::operator=(char*) [0459]
U __fastcall async_string_length_005e5790(void* string, void*);                                // CExoString::GetLength [0188]
const char* __fastcall WindowCStr005e5670(void*, void*);                                       // CExoString::CStr [0078]
void __cdecl frame_LogFormat_005e5680(void* self, const char* format, ...);                   // CExoString::Format [0125]
int __fastcall find_005e5f20(void* string, void*, int ch, int start);                          // CExoString::Find(char, int) [0241]
char __fastcall c2da_current_string_index_005e5600(void* string, void*, I index);              // CExoString::operator[] [0288]
void* __fastcall kotor_fastcall_005e5bc0(void* self, void*, I value);                          // CExoString(int) [0266]
void* __fastcall exo_locstring_construct_005e9e10(void* self, void*);                          // CExoLocString() [0265]
void __fastcall exo_locstring_destroy_005ea000(void* self, void*);                             // ~CExoLocString [0265]
char* __fastcall c2da_ref_00405fe0(void* ref, void*);                                          // CResRef::GetResRefStr [0228]
void* __fastcall async_ref_construct_string_00406d60(void* ref, void*, void* string);          // CResRef(CExoString const&) [0200]
U __fastcall c2da_not_equal_text_00406060(void* ref, void*, const char* text);                 // CResRef::operator!=(char*) [0264]
void* __fastcall ref_assign_copy_00406120(void* ref, void*, void* out, const void* other);     // CResRef::operator= (by value) [0278]
void* __fastcall c2da_ref_to_string_00405f70(void* ref, void*, void* out);                     // CResRef::CopyToString [0204]
void* __fastcall c2da_lookup_004074d0(void* resman, void*, void* ref, U type);                 // CExoResMan::GetResObject [0264]
void __fastcall c2da_register_00407680(void* resman, void*, void* ref, U type, void* res);     // CExoResMan::SetResObject [0264]
U __fastcall c2da_current_resource_demand_00409b10(void* res, void*);                          // CRes::Demand [0264]
U __fastcall c2da_release_demand_00409b80(void* res, void*);                                   // CRes::Release [0264]
U __fastcall c2da_release_00409cf0(void* resman, void*, void* res);                            // CExoResMan::ReleaseResObject [0264]
void* __fastcall c2da_resource_base_ctor_00406460(void* res, void*);                           // CRes::CRes [0184]
void* __fastcall recovered_gff_construct_004105a0(void* gff, void*);                           // CResGFF::CResGFF [0873]
U __fastcall recovered_gff_00411260(void* gff, void*, void* root, void* type, void* version);  // CResGFF::CreateGFFFile
U __fastcall recovered_gff_00412450(void* gff, void*, void* list, void* parent, const char* label); // CResGFF::AddList
U __fastcall recovered_gff_004124e0(void* gff, void*, void* element, void* list, U type);      // CResGFF::AddListElement
U __fastcall recovered_gff_004125b0(void* gff, void*, void* child, void* parent, const char* label, U type); // AddStructToStruct
U __fastcall recovered_gff_00412620(void* gff, void*, void* s, U value, const char* label);    // CResGFF::WriteFieldBYTE
U __fastcall recovered_gff_004126c0(void* gff, void*, void* s, U value, const char* label);    // CResGFF::WriteFieldWORD
U __fastcall recovered_gff_00412760(void* gff, void*, void* s, U value, const char* label);    // CResGFF::WriteFieldDWORD
U __fastcall recovered_gff_004127b0(void* gff, void*, void* s, U value, const char* label);    // CResGFF::WriteFieldINT
U __fastcall recovered_gff_00412800(void* gff, void*, void* s, U lo, U hi, const char* label); // CResGFF::WriteFieldDWORD64
U __fastcall recovered_gff_00412870(void* gff, void*, void* s, U value_bits, const char* label); // CResGFF::WriteFieldFLOAT
U __fastcall recovered_gff_004128c0(void* gff, void*, void* s, void* resref, const char* label); // CResGFF::WriteFieldCResRef
U __fastcall recovered_gff_00412970(void* gff, void*, void* s, void* string, const char* label); // CResGFF::WriteFieldCExoString
U __fastcall recovered_gff_00412a10(void* gff, void*, void* s, void* locstring, const char* label); // WriteFieldCExoLocString
U __fastcall recovered_gff_00412c10(void* gff, void*, void* s, const void* data, U size, const char* label); // WriteFieldVOID
U __fastcall recovered_gff_00413030(void* gff, void*, void* path, U type);                     // CResGFF::WriteGFFFile
U __fastcall kotor_fastcall_00411240(void* gff, void*, void* out);                             // CResGFF::GetTopLevelStruct
U __fastcall gff_list_004118c0(void* gff, void*, void* list, void* s, const char* label);      // CResGFF::GetList [0291]
U __fastcall gff_list_element_00411990(void* gff, void*, void* out, void* list, U index);      // CResGFF::GetListElement [0291]
U __fastcall gui_load_gff_dword_00411c20(void* gff, void*, void* s, const char* label, U* ok, U fallback); // ReadFieldDWORD [0292]
void* __fastcall gui_load_gff_string_00411ec0(void* gff, void*, void* out, void* s, const char* label, U* ok, void* fallback); // ReadFieldCExoString
U __fastcall recovered_save_allowed_004b20e0(void* internal, void*, void* module_name);         // IncludeModuleInSave [0841]
void* __fastcall kotor_fastcall_004ae780(void* server, void*, U id);                           // GetAreaByGameObjectID [0530]
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);                   // CServerExoApp::GetSWSMessage
void* __fastcall recovered_object_array_004aed70(void* server, void*);                         // CServerExoApp::GetObjectArray [w2_party_native]
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);                     // CServerExoApp::GetServerAIMaster
void* __fastcall kotor_fastcall_004aede0(void* server, void*);                                 // CServerExoApp::GetWorldTimer [0457]
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, void* day, void* time);        // CWorldTimer::GetWorldTime [0457]
void __fastcall recovered_timer_time_of_day_004add90(void* timer, void*, U ms, void* a, void* b, void* c, void* d); // ConvertFromTimeOfDay
void* __fastcall frame_GetCharacter_006346c0(void* party, void*, U index);                     // CSWParty::GetCharacter [0457]
void* __fastcall cerf_construct_cerf_file_005dd9c0(void* self);                                // CERFFile::CERFFile [cerf_quicksave]
void __fastcall cerf_destroy_cerf_file_005de3a0(void* self);                                   // CERFFile::~CERFFile [cerf_quicksave]
U __fastcall recovered_save_creature_00500610(void* creature, void*, void* gff, void* s);      // CSWSCreature::SaveCreature [w2]
U __fastcall recovered_save_var_table_0059adb0(void* table, void*, void* gff, void* s);        // CSWSScriptVarTable::SaveVarTable [w2]
U __fastcall recovered_sw_var_table_save_0059b250(void* table, void*, void* gff, void* s);     // CSWVarTable::SaveVarTable [w2]
U __fastcall recovered_save_game_effect_00503790(void* effect, void*, void* gff, void* s);     // CGameEffect::SaveGameEffect [w2]
U __fastcall recovered_attack_data_save_004d2210(void* data, void*, void* gff, void* s);       // CSWSCombatAttackData::SaveData [w2]
U __fastcall recovered_vm_save_situation_005d10f0(void* vm, void*, void* situation, void* gff, void* s); // SaveScriptSituation [w2]
void* __fastcall recovered_connection_player_00562c70(void* self, void*, U id);                // CSWSPlayer::CSWSPlayer [0911]
U __fastcall recovered_load_character_00561e30(void* player, void*, U index, U flag, U post);  // LoadCharacterFromIFO [w2_ifo]
void* __fastcall kotor_fastcall_00561080(void* player, void*);                                 // CSWSPlayer::GetGameObject [0595]
void __fastcall kotor_fastcall_004f0fd0(void* creature, void*, U flag);                        // RemoveFromArea [route_bodies]
U __fastcall recovered_faction_npc_reputation_0052b420(void* self, void*, U a, U b);           // GetNPCFactionReputation
void __fastcall recovered_effect_integer_005036a0(void* effect, void*, U index, U value);      // CGameEffect::SetInteger [0886]
U __fastcall recovered_object_remove_effect_004d06c0(void* object, void*, U lo, U hi);         // RemoveEffectById
void* __fastcall kotor_fastcall_005a4c60(void* stats, void*, void* out);                       // GetFullName [enter_handlers]
U __fastcall recovered_creature_stats_str_005a6190(void* stats, void*);                        // GetSTRStat [w2_g07]
U __fastcall recovered_creature_stats_int_005a6310(void* stats, void*);                        // GetINTStat [w2_g07]
U __fastcall recovered_creature_stats_wis_005a63d0(void* stats, void*);                        // GetWISStat [w2_g07]
U __fastcall recovered_creature_stats_dex_005a6550(void* stats, void*);                        // GetDEXStat [w2_g07]
U __fastcall recovered_creature_stats_con_005a6250(void* stats, void*);                        // GetCONStat [w2_g07]
U __fastcall recovered_creature_stats_cha_005a6490(void* stats, void*);                        // GetCHAStat [w2_g07]
void* __fastcall kotor_fastcall_005d5170(void* net, void*, U id);                              // CNetLayer::GetPlayerInfo [combat_actions]
void __fastcall kotor_fastcall_00405920(void* list, void*);                                    // ~CExoArrayList<CExoString> [0603]
void __fastcall kotor_fastcall_004d68a0(void* message, void*, U size, U a, U b);               // CreateWriteMessage [w2_message]
void __fastcall kotor_fastcall_004d6340(void* message, void*, U value, U bits);                // CSWMessage::WriteDWORD
U __fastcall kotor_fastcall_004d6440(void* message, void*, void** data, U* size);              // GetWriteMessage
U __fastcall kotor_fastcall_00567e60(void* message, void*, U player, U major, U minor, void* data, U size); // SendServerToPlayerMessage
// claude_w1_chains_erf.cpp
U __fastcall kotor_fastcall_005dcfc0(void* self, void*, void* path);                           // CERFFile::Create
void __fastcall kotor_fastcall_005dce30(void* self, void*, const char* version);               // CERFFile::SetVersion
U __fastcall kotor_fastcall_005dd080(void* self, void*);                                       // CERFFile::WriteHeader
U __fastcall kotor_fastcall_005dd5a0(void* self, void*);                                       // CERFFile::WriteStringTable
U __fastcall kotor_fastcall_005dda50(void* self, void*, U count);                              // CERFFile::SetNumEntries
U __fastcall kotor_fastcall_005dd0e0(void* self, void*);                                       // CERFFile::Finish
// save-gff lane (claude_save_gff.cpp, research\SAVE-GFF-EXPORTS.txt)
U __fastcall kotor_fastcall_005ddf30(void* erf, void*, const char* name, U res_type, void* res, U is_gff, U unused); // WriteResource
void* __fastcall kotor_fastcall_00408c60(void* resman, void*, U res_type, void* res);          // CExoResMan::GetResOfType
U __fastcall kotor_fastcall_00675dd0(void* self, void*);                                       // GetProperty0x4 (CExoStringList count)
U __fastcall kotor_fastcall_004088e0(void* resman, void*, void* name);                         // UpdateDirectoryKeyTable
U __fastcall kotor_fastcall_005e5950(void* string, void*, U lowercase, U allow_at_dot, U allow_extended); // StripNonAlphaNumeric
U __fastcall kotor_fastcall_005ea020(void* locstring, void*, void* other);                     // CExoLocString::Compare
// save-git lane: 0050BA00 CSWSArea::SaveGIT(CERFFile*, CExoString const&, CExoArrayList<ulong>*) RET 0Ch
void __fastcall kotor_fastcall_0050ba00(void* area, void*, void* erf, void* name, void* players);

// ---------------- exports of this file ----------------
U __fastcall kotor_fastcall_004b2e70(void* self, void*);
void* __fastcall kotor_fastcall_004c4b80(void* self, void*, void* out);
void __fastcall kotor_fastcall_004c8960(void* self, void*, void* prefix, void* path);
U __fastcall kotor_fastcall_004c7050(void* self, void*, void* gff, void* s);
U __fastcall kotor_fastcall_004c5bb0(void* self, void*, void* gff, void* s);
U __fastcall kotor_fastcall_004c3960(void* self, void*);
U __fastcall kotor_fastcall_004c3b10(void* self, void*);
U __fastcall kotor_fastcall_004ca680(void* self, void*, void* prefix, void* path);
U __fastcall kotor_fastcall_004c5980(void* self, void*, void* erf, void* ext, U type, U is_gff);
U __fastcall kotor_fastcall_004c8b90(void* self, void*, void* gff, void* s, void* erf, void* prefix, void* players);
U __fastcall kotor_fastcall_004c7870(void* self, void*, void* gff, void* s, void* prefix, void* players);
I __fastcall kotor_fastcall_004c5c70(void* self, void*, void* player);
U __fastcall kotor_fastcall_004b0970(void* self, void*, void* gff, void* s);
U __fastcall kotor_fastcall_004afea0(void* self, void*, void* gff, void* s);
U __fastcall kotor_fastcall_004d64e0(void* self, void*, void* gff, void* s);
U __fastcall kotor_fastcall_004d73a0(void* self, void*, void* gff, void* s);
U __fastcall kotor_fastcall_005034f0(void* self, void*, void* gff, void* s);
U __fastcall kotor_fastcall_004af160(void* self, void*, void* gff, void* s);
U __fastcall kotor_fastcall_004ee230(void* self, void*, void* gff, void* s);
U __fastcall kotor_fastcall_0052b790(void* self, void*, void* gff, void* list);
U __fastcall kotor_fastcall_0052b830(void* self, void*, void* gff, void* list);
U __fastcall kotor_fastcall_005624c0(void* self, void*);
void* __fastcall kotor_fastcall_005610b0(void* self, void*, void* out);
void __fastcall kotor_fastcall_004ef640(void* self, void*);
U __fastcall kotor_fastcall_0056c910(void* self, void*, U a, U b);
void* __fastcall kotor_fastcall_0062ad30(void* self, void*, I index);
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
inline const char* cstr(void* s) { return WindowCStr005e5670(s, nullptr); }
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
struct alignas(4) LocString { unsigned char b[8]; };                                          // CExoLocString
struct GffList { unsigned char b[0x14]; };                                                     // CResList {struct id, char label[16]}
struct GffStruct { U id; };                                                                    // CResStruct
struct ArrayU { U* data; I count; I alloc; };                                                  // CExoArrayList<ulong/int>
inline void* resman() { return kotor_resource_demand::resource_manager_007a39e8; }
inline void* manager_status() { return at<void*>(manager(), 0x14); }                           // [[7A39FC]+14h]
// scalar deleting destructor: vtable slot 0 with flag 1
inline void delete_virtual(void* object) { reinterpret_cast<void(__thiscall*)(void*, U)>(at<void*>(at<void*>(object, 0), 0))(object, 1); }
// obj->vtable[30h]() (AsSWSCreature) - the original dereferences the vtable even when obj is null (no guard).
inline void* as_creature(void* object) {
    void* vtable = *static_cast<void* volatile*>(object);
    return reinterpret_cast<void*(__thiscall*)(void*)>(at<void*>(vtable, 0x30))(object);
}
inline void* new_gff() {                                                                      // operator new(A0h) + CResGFF()
    void* memory = live_operator_new(0xa0);
    if (memory == nullptr) return nullptr;
    try { return recovered_gff_construct_004105a0(memory, nullptr); } catch (...) { live_free(memory); throw; }
}
inline U field_byte(void* gff, void* s, U v, const char* l) { return recovered_gff_00412620(gff, nullptr, s, v, l); }
inline U field_word(void* gff, void* s, U v, const char* l) { return recovered_gff_004126c0(gff, nullptr, s, v, l); }
inline U field_dword(void* gff, void* s, U v, const char* l) { return recovered_gff_00412760(gff, nullptr, s, v, l); }
inline U field_int(void* gff, void* s, U v, const char* l) { return recovered_gff_004127b0(gff, nullptr, s, v, l); }
inline U field_float(void* gff, void* s, U v, const char* l) { return recovered_gff_00412870(gff, nullptr, s, v, l); }
inline U field_resref(void* gff, void* s, void* v, const char* l) { return recovered_gff_004128c0(gff, nullptr, s, v, l); }
inline U field_string(void* gff, void* s, void* v, const char* l) { return recovered_gff_00412970(gff, nullptr, s, v, l); }
inline U field_loc(void* gff, void* s, void* v, const char* l) { return recovered_gff_00412a10(gff, nullptr, s, v, l); }
inline U add_list(void* gff, void* list, void* parent, const char* l) { return recovered_gff_00412450(gff, nullptr, list, parent, l); }
inline U add_element(void* gff, void* element, void* list, U type) { return recovered_gff_004124e0(gff, nullptr, element, list, type); }
inline U add_struct(void* gff, void* child, void* parent, const char* l, U type) {
    return recovered_gff_004125b0(gff, nullptr, child, parent, l, type);
}
inline void* list_at(void* list, void* pos) { return unresolved_list_at_pos_005e9b60(list, nullptr, pos); }
inline void list_next(void* list, void** pos) { (void)unresolved_list_next_005e9b90(list, nullptr, pos); }
inline void erf_delete(void* erf) { cerf_destroy_cerf_file_005de3a0(erf); live_free(erf); }
const char k_v20_00745338[] = "V2.0";
const char k_module_007442a8[] = "Module";
const char k_object_id_00744c24[] = "ObjectId";
const char k_empty_0073d71c[] = "";
const char k_backslash_0073d8f4[] = "\\";
const char k_server_vault_00745340[] = "SERVERVAULT:%s";
constexpr U k_res_ifo_7de = 0x7de;
constexpr U k_res_bic_7df = 0x7df;
constexpr U8 k_get_object_ok_0074666c = 0x00;                                                  // .rdata 0074666C byte 00
constexpr U8 k_creature_type_floor_00746633 = 0x04;                                            // .rdata 00746633 byte 04
constexpr U k_effect_type_0074677_3e = 0x3e;                                                   // .rdata 00746774 dword 3Eh
constexpr U k_token_floor_0073eccc = 9;                                                        // .rdata 0073ECCC dword 9
constexpr U k_load_bar_player_0074c52c = 0xfffffff7u;                                          // .rdata 0074C52C
}  // namespace

extern "C" {
// =========================================================================================================
// 004C4B80 CSWSModule::GetModuleResourceName(CExoString* out): strips every "prefix:" from the module name (+5Ch).
void* __fastcall kotor_fastcall_004c4b80(void* self, void*, void* out) {
    Scoped name; name.empty();                                                                 // 004C4B9C..4BA8 [esp+0Ch]
    s_assign(name.p(), static_cast<unsigned char*>(self) + 0x5c);                              // 004C4BAD..4BBE
    int colon = find_005e5f20(name.p(), nullptr, 0x3a, 0);                                     // 004C4BC3..4BCB
    while (colon != -1) {                                                                      // 004C4BD2..4C20
        const I count = static_cast<I>(async_string_length_005e5790(name.p(), nullptr)) - colon - 1; // 004C4BD7..4BE2
        {
            Scoped right;                                                                      // [esp+14h]
            (void)base_ini_reconstruction::right_005e6170(str(name.p()), nullptr, str(right.adopt()), count); // 004C4BE3..4BED
            s_assign(name.p(), right.p());                                                     // 004C4BF2..4BFC
        }                                                                                      // 004C4C01..4C09
        colon = find_005e5f20(name.p(), nullptr, 0x3a, 0);                                     // 004C4C0E..4C16
    }
    (void)WindowCopy005e5b00(out, nullptr, name.p());                                          // 004C4C22..4C2D
    return out;                                                                                // 004C4C32..4C56 (~name)
}

// 004B2E70 CServerExoAppInternal::StoreCurrentModule: writes "GAMEINPROGRESS:<module>" (the part after the last
// ':' of the module name, or the whole name) as a MOD ERF when IncludeModuleInSave agrees. Always returns 1.
U __fastcall kotor_fastcall_004b2e70(void* self, void*) {
    void* module = server_internal_get_module_004b14f0(self, nullptr);                         // 004B2E90..2E97
    if (module == nullptr) return 1;                                                           // 004B2E99..2E9B
    U include;
    {
        Scoped resource;                                                                       // [esp+18h]
        (void)kotor_fastcall_004c4b80(module, nullptr, resource.adopt());                      // 004B2EA1..2EA8
        include = recovered_save_allowed_004b20e0(self, nullptr, resource.p());                // 004B2EAD..2EB8
    }                                                                                          // 004B2EBD..2ECB
    if (include == 0) return 1;                                                                // 004B2ED0..2ED2
    Scoped prefix; prefix.text("GAMEINPROGRESS:");                                             // 004B2ED8..2EE1 [esp+10h] state 1
    Scoped path; (void)WindowCopy005e5b00(path.adopt(), nullptr, static_cast<unsigned char*>(module) + 0x5c); // 004B2EE6..2EF6 [esp+8]
    const int colon = find_005e5f20(path.p(), nullptr, 0x3a, 0);                               // 004B2EFB..2F08
    if (colon == -1) {                                                                         // 004B2F0F..2F12
        Scoped joined;                                                                         // [esp+18h]
        s_concat(prefix.p(), joined.adopt(), path.p());                                        // 004B2F14..2F22
        s_assign(path.p(), joined.p());                                                        // 004B2F27..2F31
    } else {                                                                                   // 004B2F36..2F91 (~joined)
        const I count = static_cast<I>(async_string_length_005e5790(path.p(), nullptr)) - colon - 1; // 004B2F3C..2F47
        Scoped right;                                                                          // [esp+28h]
        (void)base_ini_reconstruction::right_005e6170(str(path.p()), nullptr, str(right.adopt()), count); // 004B2F48..2F52
        {
            Scoped joined;                                                                     // [esp+20h]
            s_concat(prefix.p(), joined.adopt(), right.p());                                   // 004B2F57..2F66
            s_assign(path.p(), joined.p());                                                    // 004B2F6B..2F75
        }                                                                                      // 004B2F7A..2F83
    }                                                                                          // 004B2F88..2F91
    kotor_fastcall_004c8960(module, nullptr, prefix.p(), path.p());                            // 004B2F96..2FA2
    (void)kotor_fastcall_004c3b10(module, nullptr);                                            // 004B2FA7..2FA9
    (void)kotor_fastcall_004ca680(module, nullptr, prefix.p(), path.p());                      // 004B2FAE..2FBA
    path.end(); prefix.end();                                                                  // 004B2FBF..2FD9
    return 1;                                                                                  // 004B2FE3
}

// 004C8960 CSWSModule::SaveModuleStart(prefix, path): deletes the old file, opens the MOD ERF (+1E8h), the player id
// list (+1F4h) and the IFO GFF (+1F0h, struct +1ECh); on a failed CreateGFFFile everything is released again.
void __fastcall kotor_fastcall_004c8960(void* self, void*, void*, void* path) {
    at<U>(self, 0x1c8) = 1;                                                                    // 004C8981..4C8986
    Scoped resolved;                                                                           // [esp+1Ch]
    (void)path_resolution::resolve_005e68a0(
        static_cast<alias_path::AliasListView*>(at<void*>(kotor_menu_10_19::dependencies::base_007a39e0, 0xc)), nullptr,
        str(resolved.adopt()), str(path), 0xbc1);                                              // 004C8981..49A4
    (void)DeleteFileA(cstr(resolved.p()));                                                     // 004C89A6..89B4
    {
        void* memory = live_operator_new(0xd0);                                                // 004C89BA..89C4
        void* erf = nullptr;
        if (memory != nullptr) {
            try { erf = cerf_construct_cerf_file_005dd9c0(memory); } catch (...) { live_free(memory); throw; } // 004C89D4..89D6
        }
        at<void*>(self, 0x1e8) = erf;                                                          // 004C89E5
    }
    {
        U* players = static_cast<U*>(live_operator_new(0xc));                                  // 004C89DF..89EB
        if (players != nullptr) { players[1] = 0; players[2] = 0; players[0] = 0; }            // 004C89F7..89FD
        at<U*>(self, 0x1f4) = players;                                                         // 004C8A0E
    }
    (void)kotor_fastcall_005dcfc0(at<void*>(self, 0x1e8), nullptr, path);                      // 004C8A03..8A14
    kotor_fastcall_005dce30(at<void*>(self, 0x1e8), nullptr, "MOD V1.0");                       // 004C8A19..8A24
    (void)kotor_fastcall_005dd080(at<void*>(self, 0x1e8), nullptr);                             // 004C8A29..8A2F
    (void)kotor_fastcall_005dd5a0(at<void*>(self, 0x1e8), nullptr);                             // 004C8A34..8A3A
    at<U>(self, 0x20c) = 3;                                                                    // 004C8A47
    (void)kotor_fastcall_005dda50(at<void*>(self, 0x1e8), nullptr, 3);                          // 004C8A3F..8A51
    at<void*>(self, 0x1f0) = new_gff();                                                        // 004C8A56..8A81
    at<void*>(self, 0x1ec) = live_operator_new(4);                                             // 004C8A7B..8A98
    U created;
    {
        Scoped version; version.text(k_v20_00745338);                                          // 004C8A8F..8A9E [esp+14h]
        Scoped type; type.text("IFO ");                                                        // 004C8AA3..8AB1 [esp+0Ch]
        created = recovered_gff_00411260(at<void*>(self, 0x1f0), nullptr, at<void*>(self, 0x1ec), type.p(), version.p()); // 004C8AB6..8AD2
    }                                                                                          // 004C8AD7..8AEF
    if (created != 0) {                                                                        // 004C8AF4..8AF6
        (void)kotor_fastcall_004c7050(self, nullptr, at<void*>(self, 0x1f0), at<void*>(self, 0x1ec)); // 004C8AF8..8B08
        (void)kotor_fastcall_004c3960(self, nullptr);                                          // 004C8B0D..8B0F
    } else {
        void* erf = at<void*>(self, 0x1e8);                                                    // 004C8B16..8B1E
        if (erf != nullptr) erf_delete(erf);                                                   // 004C8B20..8B2D
        void* gff = at<void*>(self, 0x1f0);                                                    // 004C8B30
        at<void*>(self, 0x1e8) = nullptr;                                                      // 004C8B38
        if (gff != nullptr) delete_virtual(gff);                                               // 004C8B36..8B44
        void* root = at<void*>(self, 0x1ec);                                                   // 004C8B46
        at<void*>(self, 0x1f0) = nullptr;                                                      // 004C8B4D
        live_free(root);                                                                       // 004C8B4C..8B53
        at<void*>(self, 0x1ec) = nullptr;                                                      // 004C8B5B
    }
}                                                                                              // 004C8B61..8B83 (~resolved)

// 004C3960 CSWSModule::SaveModuleFAC: writes the faction manager (server internal +10054h) to
// "GAMEINPROGRESS:REPUTE" (GFF "FAC "). Returns the CreateGFFFile result.
U __fastcall kotor_fastcall_004c3960(void*, void*) {
    Scoped filename; filename.empty();                                                         // 004C397C..3980 [esp+1Ch]
    void* factions = at<void*>(at<void*>(server_app(), 4), 0x10054);                           // 004C3985..39A1
    void* gff = new_gff();                                                                     // 004C3998..39C5
    void* root = live_operator_new(4);                                                         // 004C39C7..39DE
    U created;
    {
        Scoped version; version.text(k_v20_00745338);                                          // 004C39D5..39E0 [esp+24h]
        Scoped type; type.text("FAC ");                                                        // 004C39E5..39F3 [esp+14h]
        created = recovered_gff_00411260(gff, nullptr, root, type.p(), version.p());           // 004C39F8..3A0A
    }                                                                                          // 004C3A0F..3A27
    if (created == 1) {                                                                        // 004C3A2C..3A2F
        GffList list;                                                                          // [esp+2Ch]
        (void)add_list(gff, &list, root, "FactionList");                                       // 004C3A35..3A42
        (void)kotor_fastcall_0052b790(factions, nullptr, gff, &list);                          // 004C3A47..3A51
        (void)add_list(gff, &list, root, "RepList");                                           // 004C3A56..3A63
        (void)kotor_fastcall_0052b830(factions, nullptr, gff, &list);                          // 004C3A68..3A72
        (void)frame_LogAssign_005e5140(filename.p(), nullptr, "GAMEINPROGRESS:REPUTE");         // 004C3A77..3A80
        {
            Scoped path; path.text(cstr(filename.p()));                                        // 004C3A85..3A93 [esp+24h]
            (void)recovered_gff_00413030(gff, nullptr, path.p(), 0x7f6);                       // 004C3A98..3AA9
        }                                                                                      // 004C3AAE..3AB6
        if (gff != nullptr) reinterpret_cast<void(__thiscall*)(void*, U)>(at<void*>(at<void*>(gff, 0), 0))(gff, created); // 004C3ABB..3AC4 (flag = EBP = 1)
    } else if (gff != nullptr) {
        delete_virtual(gff);                                                                   // 004C3AC8..3AD2
    }
    live_free(root);                                                                           // 004C3AD4..3ADA
    return created;                                                                            // 004C3ADD..3B02 (~filename)
}

// 004C3B10 CSWSModule::SaveModuleInProgress: SaveGIT of the current area into the MOD ERF as "tmpgit", then the
// load-bar stall event. RET 0, EAX 0 without an open ERF, else 1.
U __fastcall kotor_fastcall_004c3b10(void* self, void*) {
    Scoped name; name.empty();                                                                 // 004C3B2C..3B30 [esp+10h] state 0
    Scoped area_name; area_name.empty();                                                       // 004C3B35..3B41 [esp+18h] state 1
    Scoped git_name; git_name.empty();                                                         // 004C3B4B..3B53 [esp+8] state 2
    if (at<void*>(self, 0x1e8) == nullptr) return 0;                                           // 004C3B58..3BA5 (~ in reverse order)
    (void)frame_LogAssign_005e5140(area_name.p(), nullptr, "tmparea");                         // 004C3BA7..3BB0
    (void)frame_LogAssign_005e5140(git_name.p(), nullptr, "tmpgit");                           // 004C3BB5..3BBE
    void* area = kotor_fastcall_004ae780(server_app(), nullptr, at<U>(self, 0x40));            // 004C3BC3..3BCF
    s_assign(name.p(), git_name.p());                                                          // 004C3BD6..3BDF
    (void)kotor_fastcall_0050ba00(area, nullptr, at<void*>(self, 0x1e8), name.p(), at<void*>(self, 0x1f4)); // 004C3BE4..3BF9
    at<U>(manager_status(), 8) = 1;                                                            // 004C3BFE..3C07
    const U stall = at<U>(manager_status(), 0xc);                                              // 004C3C0A..3C12
    void* message = recovered_server_sws_message_004aedb0(server_app(), nullptr);              // 004C3C15..3C1C
    (void)kotor_fastcall_0056c910(message, nullptr, 1, stall);                                 // 004C3C18..3C23
    git_name.end(); area_name.end(); name.end();                                               // 004C3C28..3C4F
    return 1;                                                                                  // 004C3C5A
}

// 004CA680 CSWSModule::SaveModuleFinish(prefix, path): area statics ("ARE ", 7DCh) unless the module is an NWM file,
// IFO finish, ERF Finish; frees the ERF and the player id list. RET 8.
U __fastcall kotor_fastcall_004ca680(void* self, void*, void* prefix, void*) {
    if (at<void*>(self, 0x1e8) == nullptr) return 0;                                           // 004CA699..A6B6
    if (at<U>(self, 0x200) == 0) {                                                             // 004CA6B9..A6C1
        Scoped ext; ext.text("ARE ");                                                          // 004CA6C3..A6CC [esp+4]
        (void)kotor_fastcall_004c5980(self, nullptr, at<void*>(self, 0x1e8), ext.p(), 0x7dc, 1); // 004CA6D1..A6EE
    }                                                                                          // 004CA6F3..A6FF
    (void)kotor_fastcall_004c8b90(self, nullptr, at<void*>(self, 0x1f0), at<void*>(self, 0x1ec), at<void*>(self, 0x1e8),
                                  prefix, at<void*>(self, 0x1f4));                             // 004CA704..A728
    (void)kotor_fastcall_005dd0e0(at<void*>(self, 0x1e8), nullptr);                             // 004CA72D..A733
    void* erf = at<void*>(self, 0x1e8);                                                        // 004CA738..A740
    if (erf != nullptr) erf_delete(erf);                                                       // 004CA742..A74F
    U* players = at<U*>(self, 0x1f4);                                                          // 004CA752
    at<void*>(self, 0x1e8) = nullptr;                                                          // 004CA75A
    if (players != nullptr) {                                                                  // 004CA758..A764
        live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(players[0])));          // 004CA766..A769
        players[0] = 0;                                                                        // 004CA76F
        live_free(players);                                                                    // 004CA76E..A775
    }
    at<void*>(self, 0x1f4) = nullptr;                                                          // 004CA781
    return 1;                                                                                  // 004CA78C
}

// 004C5980 CSWSModule::SaveStatic(CERFFile* erf, CExoString const& ext, ushort type, int gff): copies every resource
// of `type` that belongs to the "Module" IFO into the ERF; missing ones are registered first (a CResGFF tagged with
// `ext` when `gff`, else a plain CRes) and deleted again afterwards. RET 10h, EAX 0 when any resource failed.
U __fastcall kotor_fastcall_004c5980(void*, void*, void* erf, void* ext, U type, U is_gff) {
    U result = 1;                                                                              // 004C59A7 [esp+10h]
    U created = 0;                                                                             // EBX
    menu_resources::Ref ref;                                                                   // [esp+20h]
    ref.construct_00406d80(k_module_007442a8);                                                 // 004C599C..59AF
    void* module_res = c2da_lookup_004074d0(resman(), nullptr, &ref, k_res_ifo_7de);           // 004C59B4..59C4
    void* names = kotor_fastcall_00408c60(resman(), nullptr, type, module_res);                // 004C59C9..59DB
    if (names != nullptr && static_cast<I>(kotor_fastcall_00675dd0(names, nullptr)) > 0) {     // 004C59E0..59FB
        I index = 0;                                                                           // [esp+58h] (argument slot)
        do {
            void* name = kotor_sound_internal::StringListGet005e95e0(names, nullptr, static_cast<U>(index)); // 004C5A01..5A08
            ref.construct_00406d80(cstr(name));                                                // 004C5A0D..5A1B
            void* res = c2da_lookup_004074d0(resman(), nullptr, &ref, type);                   // 004C5A20..5A2C
            bool demand = true;
            if (res == nullptr) {                                                              // 004C5A31..5A35
                if (is_gff != 0) {                                                             // 004C5A3B..5A41
                    void* memory = live_operator_new(0xa0);                                    // 004C5A43..5A4D
                    if (memory != nullptr) {
                        try { res = recovered_gff_construct_004105a0(memory, nullptr); } catch (...) { live_free(memory); throw; } // 004C5A5C..5A63
                    }
                    (void)kotor_runtime_reporter_dependencies::string_copy_n_006fab80(
                        static_cast<char*>(res) + 0x8d, cstr(ext), 4);                         // 004C5A65..5A85 (no null guard)
                } else {
                    void* memory = live_operator_new(0x28);                                    // 004C5A8A..5A94
                    if (memory != nullptr) {
                        try { res = c2da_resource_base_ctor_00406460(memory, nullptr); } catch (...) { live_free(memory); throw; } // 004C5AA4..5AA6
                    }
                }
                menu_resources::Ref key;                                                       // [esp+30h]
                key.construct_00406d80(cstr(name));                                            // 004C5AB9..5AC5
                c2da_register_00407680(resman(), nullptr, &key, type, res);                    // 004C5ACA..5AD7
                created = 1;                                                                   // 004C5ADE
                if (res == nullptr) { result = 0; demand = false; }                            // 004C5ADC..5AE3 / 004C5B98..5BA0
            }
            if (demand) {
                if (c2da_current_resource_demand_00409b10(res, nullptr) != 0) {                // 004C5AE9..5AF2
                    (void)kotor_fastcall_005ddf30(erf, nullptr, cstr(name), type, res, is_gff, 0xffffffffu); // 004C5AF4..5B09
                    (void)c2da_release_demand_00409b80(res, nullptr);                          // 004C5B0E..5B10
                } else {
                    result = 0;                                                                // 004C5B17
                }
                (void)c2da_release_00409cf0(resman(), nullptr, res);                           // 004C5B1F..5B26
                if (created == 0) goto next;                                                   // 004C5B2B..5B2D
            }
            created = 0;                                                                       // 004C5B2F
            if (res != nullptr) delete_virtual(res);                                           // 004C5B31..5B3B
        next:
            ++index;                                                                           // 004C5B3D..5B46
        } while (index < static_cast<I>(kotor_fastcall_00675dd0(names, nullptr)));            // 004C5B4A..5B55
    }
    (void)c2da_release_00409cf0(resman(), nullptr, module_res);                                // 004C5B5F..5B66
    if (names != nullptr) {                                                                    // 004C5B6B..5B6D
        kotor_sound_internal::StringListDestroy005e9860(names, nullptr);                       // 004C5B6F..5B71
        live_free(names);                                                                      // 004C5B76..5B77
    }
    return result;                                                                             // 004C5B83
}

// 004C5BB0 CSWSModule::SaveLimboCreatures(CResGFF*, CResStruct*): "Creature List" of the limbo ids (+1DCh/+1E0h).
// RET 8; EAX = the last count read.
U __fastcall kotor_fastcall_004c5bb0(void* self, void*, void* gff, void* s) {
    void* objects = recovered_object_array_004aed70(server_app(), nullptr);                    // 004C5BB3..5BC0
    GffList list;                                                                              // [esp+18h]
    (void)add_list(gff, &list, s, "Creature List");                                            // 004C5BC5..5BDE
    for (I i = 0; i < at<I>(self, 0x1e0); ++i) {                                               // 004C5BE3..5C59
        void* object = nullptr;                                                                // [esp+18h]
        const U id = at<U*>(self, 0x1dc)[i];                                                   // 004C5BF0..5BF6
        if (static_cast<U8>(kotor_fastcall_004d8230(objects, nullptr, id, &object)) != k_get_object_ok_0074666c) continue; // 004C5BF9..5C0E
        void* creature = as_creature(object);                                                  // 004C5C10..5C16
        GffStruct element;                                                                     // [esp+10h]
        (void)add_element(gff, &element, &list, 4);                                            // 004C5C19..5C29
        (void)field_dword(gff, &element, at<U>(creature, 4), k_object_id_00744c24);            // 004C5C2E..5C3E
        (void)recovered_save_creature_00500610(creature, nullptr, gff, &element);              // 004C5C43..5C4B
    }
    return at<U>(self, 0x1e0);                                                                 // 004C5C50 / 004C5BE3
}

// 004C7050 CSWSModule::SaveModuleIFOStart(CResGFF* gff, CResStruct* s): the module IFO fields, limbo creatures,
// expansion / cutscene / area / token lists, both variable tables and the AI event queue. RET 8, EAX 1.
U __fastcall kotor_fastcall_004c7050(void* self, void*, void* gff, void* s) {
    unsigned char* m = static_cast<unsigned char*>(self);
    LocString unused;                                                                          // [esp+40h] (constructed, never used)
    (void)exo_locstring_construct_005e9e10(&unused, nullptr);                                  // 004C706E..7072
    struct LocGuard { LocString* p; ~LocGuard() { exo_locstring_destroy_005ea000(p, nullptr); } } loc_guard{&unused};
    void* objects = recovered_object_array_004aed70(server_app(), nullptr);                    // 004C7077..7087
    (void)recovered_gff_00412c10(gff, nullptr, s, at<void*>(self, 0x54), 0x20, "Mod_ID");      // 004C708C..70A7
    (void)field_int(gff, s, at<U>(at<void*>(self, 0x54), 0x20), "Mod_Creator_ID");             // 004C70AC..70BB
    (void)field_dword(gff, s, at<U>(at<void*>(self, 0x54), 0x24), "Mod_Version");              // 004C70C0..70CF
    (void)field_loc(gff, s, m + 0x64, "Mod_Name");                                             // 004C70D4..70E0
    (void)field_loc(gff, s, m + 0x4c, "Mod_Description");                                      // 004C70E5..70F1
    (void)field_byte(gff, s, at<U8>(self, 0x1c8), "Mod_IsSaveGame");                           // 004C70F6..7107
    (void)field_byte(gff, s, at<U8>(self, 0x200), "Mod_IsNWMFile");                            // 004C710C..711D
    if (at<U>(self, 0x200) != 0) (void)field_string(gff, s, m + 0x204, "Mod_NWMResName");      // 004C7122..713B
    {
        Scoped hak; hak.text(c2da_ref_00405fe0(m + 0x6c, nullptr));                            // 004C7140..714D [esp+30h]
        (void)field_string(gff, s, hak.p(), "Mod_Hak");                                        // 004C7152..7167
    }                                                                                          // 004C716C..7175
    (void)recovered_gff_00412800(gff, nullptr, s, recovered_saved_state_007a1b40, recovered_saved_state_007a1b44,
                                 "Mod_Effect_NxtId");                                          // 004C717A..718F
    (void)field_dword(gff, s, at<U>(objects, 0xc), "Mod_NextCharId0");                         // 004C7194..71A0
    (void)field_dword(gff, s, at<U>(objects, 0x10), "Mod_NextCharId1");                        // 004C71A5..71B1
    (void)field_dword(gff, s, at<U>(objects, 4), "Mod_NextObjId0");                            // 004C71B6..71C2
    (void)field_dword(gff, s, at<U>(objects, 8), "Mod_NextObjId1");                            // 004C71C7..71D3
    (void)field_string(gff, s, m + 0x1f8, "Mod_Tag");                                          // 004C71D8..71E7
    (void)field_resref(gff, s, at<void*>(self, 0x58), "Mod_Entry_Area");                       // 004C71EC..71F8
    (void)field_float(gff, s, at<U>(at<void*>(self, 0x58), 0x10), "Mod_Entry_X");              // 004C71FD..720C
    (void)field_float(gff, s, at<U>(at<void*>(self, 0x58), 0x14), "Mod_Entry_Y");              // 004C7211..7220
    (void)field_float(gff, s, at<U>(at<void*>(self, 0x58), 0x18), "Mod_Entry_Z");              // 004C7225..7234
    (void)field_float(gff, s, at<U>(at<void*>(self, 0x58), 0x1c), "Mod_Entry_Dir_X");          // 004C7239..7248
    (void)field_float(gff, s, at<U>(at<void*>(self, 0x58), 0x20), "Mod_Entry_Dir_Y");          // 004C724D..725C
    (void)field_byte(gff, s, at<U8>(self, 0x1a0), "Mod_MinPerHour");                           // 004C7261..7272
    (void)field_byte(gff, s, at<U8>(self, 0x1a1), "Mod_DawnHour");                             // 004C7277..7288
    (void)field_byte(gff, s, at<U8>(self, 0x1a2), "Mod_DuskHour");                             // 004C728D..729E
    U day = 0, time = 0;                                                                       // [esp+28h] / [esp+18h]
    frame_GetWorldTime_004ade40(kotor_fastcall_004aede0(server_app(), nullptr), nullptr, &day, &time); // 004C72A3..72BD
    U hour = 0, minute = 0, second = 0, millisecond = 0;                                       // [esp+2Ch] / 1Ch / 20h / 24h
    recovered_timer_time_of_day_004add90(kotor_fastcall_004aede0(server_app(), nullptr), nullptr, time, &hour, &minute,
                                         &second, &millisecond);                               // 004C72C2..72EA
    const U pause_day = at<U>(kotor_fastcall_004aede0(server_app(), nullptr), 0x28);           // 004C72EF..7305
    const U pause_time = at<U>(kotor_fastcall_004aede0(server_app(), nullptr), 0x2c);          // 004C72FC..730D [esp+80h]
    (void)field_dword(gff, s, at<U>(self, 0x1a8), "Mod_StartYear");                            // 004C7310..7326
    (void)field_byte(gff, s, at<U8>(self, 0x1ac), "Mod_StartMonth");                           // 004C732B..733C
    (void)field_byte(gff, s, at<U8>(self, 0x1b0), "Mod_StartDay");                             // 004C7341..7352
    (void)field_byte(gff, s, at<U8>(self, 0x1b4), "Mod_StartHour");                            // 004C7357..7368
    (void)field_dword(gff, s, at<U>(self, 0x1b8), "Mod_Transition");                           // 004C736D..737C
    (void)field_word(gff, s, minute, "Mod_StartMinute");                                       // 004C7381..738E
    (void)field_word(gff, s, second, "Mod_StartSecond");                                       // 004C7393..73A0
    (void)field_word(gff, s, millisecond, "Mod_StartMiliSec");                                 // 004C73A5..73B2
    (void)field_dword(gff, s, pause_day, "Mod_PauseDay");                                      // 004C73B7..73C0
    (void)field_dword(gff, s, pause_time, "Mod_PauseTime");                                    // 004C73C5..73D5
    (void)field_byte(gff, s, at<U8>(self, 0x1a6), "Mod_XPScale");                              // 004C73DA..73EB
    (void)kotor_fastcall_004c5bb0(self, nullptr, gff, s);                                      // 004C73F0..73F4
    GffList list;                                                                              // [esp+58h]
    GffStruct element;                                                                         // [esp+10h]
    void* pos;                                                                                 // [esp+14h]
    (void)add_list(gff, &list, s, "Mod_Expan_List");                                           // 004C73F9..7406
    pos = at<void*>(at<void*>(self, 0x28), 0);                                                 // 004C740B..7412
    while (pos != nullptr) {                                                                   // 004C7416..7478
        void* node = list_at(at<void*>(self, 0x28), pos);                                      // 004C7420..7424
        (void)add_element(gff, &element, &list, 0);                                            // 004C742B..7439
        (void)field_loc(gff, &element, node, "Expansion_Name");                                // 004C743E..744B
        (void)field_int(gff, &element, at<U>(node, 8), "Expansion_ID");                        // 004C7450..7460
        list_next(at<void*>(self, 0x28), &pos);                                                // 004C7465..746D
    }
    (void)add_list(gff, &list, s, "Mod_CutSceneList");                                         // 004C747A..7487
    pos = at<void*>(at<void*>(self, 0x2c), 0);                                                 // 004C748C..7491
    while (pos != nullptr) {                                                                   // 004C7497..74F8
        void* node = list_at(at<void*>(self, 0x2c), pos);                                      // 004C74A0..74A4
        (void)add_element(gff, &element, &list, 1);                                            // 004C74A9..74B9
        (void)field_resref(gff, &element, node, "CutScene_Name");                              // 004C74BE..74CB
        (void)field_dword(gff, &element, at<U>(node, 0x10), "CutScene_ID");                    // 004C74D0..74E0
        list_next(at<void*>(self, 0x2c), &pos);                                                // 004C74E5..74ED
    }
    static const struct { U offset; const char* label; } scripts[14] = {
        {0xb0, "Mod_OnHeartbeat"},  {0xb8, "Mod_OnUsrDefined"}, {0xd0, "Mod_OnClientEntr"}, {0xd8, "Mod_OnClientLeav"},
        {0xe0, "Mod_OnActvtItem"},  {0xe8, "Mod_OnAcquirItem"}, {0xf0, "Mod_OnUnAqreItem"}, {0xc0, "Mod_OnModLoad"},
        {0xc8, "Mod_OnModStart"},   {0xf8, "Mod_OnPlrDeath"},   {0x100, "Mod_OnPlrDying"},  {0x108, "Mod_OnSpawnBtnDn"},
        {0x110, "Mod_OnPlrRest"},   {0x118, "Mod_OnPlrLvlUp"}};
    menu_resources::Ref script;                                                                // [esp+30h]
    for (const auto& entry : scripts) {                                                        // 004C74FA..76D1
        (void)async_ref_construct_string_00406d60(&script, nullptr, m + entry.offset);
        (void)field_resref(gff, s, &script, entry.label);
    }
    (void)add_list(gff, &list, s, "Mod_Area_list");                                            // 004C76D6..76E3
    void* area = kotor_fastcall_004ae780(server_app(), nullptr, at<U>(self, 0x40));            // 004C76E8..76F5
    U area_ref[4];                                                                             // [esp+48h]
    std::memcpy(area_ref, static_cast<unsigned char*>(area) + 0x10c, 16);                      // 004C76FA..7716
    (void)add_element(gff, &element, &list, 6);                                                // 004C771A..7728
    (void)field_resref(gff, &element, area_ref, "Area_Name");                                  // 004C772D..773E
    (void)field_dword(gff, &element, at<U>(self, 0x40), k_object_id_00744c24);                 // 004C7743..7753
    at<U>(manager_status(), 0xc) = 1;                                                          // 004C7758..776D
    (void)add_list(gff, &list, s, "Mod_Tokens");                                               // 004C7760..7774
    for (U i = 0; i < at<U>(kotor_cell_tlk_007a3a08, 0x4c); ++i) {                             // 004C7779..780A (count re-read)
        if (at<U>(at<unsigned char*>(kotor_cell_tlk_007a3a08, 0x48) + i * 0xc, 0) <= k_token_floor_0073eccc) continue; // 004C7790..779C
        (void)add_element(gff, &element, &list, 7);                                            // 004C779E..77AC
        (void)field_dword(gff, &element, at<U>(at<unsigned char*>(kotor_cell_tlk_007a3a08, 0x48) + i * 0xc, 0),
                          "Mod_TokensNumber");                                                 // 004C77B1..77CA
        (void)field_string(gff, &element, at<unsigned char*>(kotor_cell_tlk_007a3a08, 0x48) + i * 0xc + 4,
                           "Mod_TokensValue");                                                 // 004C77CF..77E9
    }
    (void)recovered_save_var_table_0059adb0(m + 0x8c, nullptr, gff, s);                        // 004C780C..7814
    (void)recovered_sw_var_table_save_0059b250(m + 0x9c, nullptr, gff, s);                     // 004C7819..7821
    (void)kotor_fastcall_004b0970(recovered_server_ai_master_004aed80(server_app(), nullptr), nullptr, gff, s); // 004C7826..7838
    return 1;                                                                                  // 004C783D..7865 (~unused)
}

// 004C8B90 CSWSModule::SaveModuleIFOFinish(gff, s, erf, prefix, players): players, then the IFO goes into the ERF
// as "Module" (7DEh, GFF); deletes the GFF and frees the struct. RET 14h, EAX 1.
U __fastcall kotor_fastcall_004c8b90(void* self, void*, void* gff, void* s, void* erf, void* prefix, void* players) {
    Scoped a; a.empty();                                                                       // 004C8BAD..8BB1 [esp+1Ch]
    Scoped b; b.empty();                                                                       // 004C8BB6..8BC2 [esp+14h]
    Scoped c; c.empty();                                                                       // 004C8BC7..8BD0 [esp+0Ch]
    (void)kotor_fastcall_004c7870(self, nullptr, gff, s, prefix, players);                     // 004C8BD5..8BF0
    (void)kotor_fastcall_005ddf30(erf, nullptr, k_module_007442a8, k_res_ifo_7de, gff, 1, 0xffffffffu); // 004C8BF5..8C08
    if (gff != nullptr) delete_virtual(gff);                                                   // 004C8C0D..8C17
    live_free(s);                                                                              // 004C8C19..8C1A
    c.end(); b.end(); a.end();                                                                 // 004C8C22..8C4A
    return 1;                                                                                  // 004C8C55
}

// 004C5C70 CSWSModule::GetPlayerIndexInPlayerList(CSWSPlayer*): index of the IFO player entry (+48h list) whose
// community name and first/last names match, else -1. RET 4.
I __fastcall kotor_fastcall_004c5c70(void* self, void*, void* player) {
    I index = 0;                                                                               // [esp+10h]
    void* pos = at<void*>(at<void*>(self, 0x48), 0);                                           // 004C5C91..5CA2
    while (pos != nullptr) {                                                                   // 004C5CA6..5D4E
        unsigned char* entry = static_cast<unsigned char*>(list_at(at<void*>(self, 0x48), pos)); // 004C5CB0..5CB4
        U same;
        {
            Scoped name;                                                                       // [esp+18h]
            (void)kotor_fastcall_005610b0(player, nullptr, name.adopt());                      // 004C5CB9..5CC3
            same = async_string_equal_005e5200(entry, nullptr, name.p());                      // 004C5CC8..5CD3
        }                                                                                      // 004C5CD8..5CE6
        if (same != 0) {                                                                       // 004C5CEB..5CED
            void* creature = kotor_fastcall_004ae770(server_app(), nullptr, at<U>(player, 0x38)); // 004C5CEF..5D04
            if (kotor_fastcall_005ea020(entry + 8, nullptr, at<unsigned char*>(creature, 0xa74) + 0x34) != 0 && // 004C5D06..5D1A
                kotor_fastcall_005ea020(entry + 0x10, nullptr, at<unsigned char*>(creature, 0xa74) + 0x3c) != 0) // 004C5D1C..5D30
                return index;                                                                  // 004C5D6B..5D80
        }
        list_next(at<void*>(self, 0x48), &pos);                                                // 004C5D32..5D3A
        ++index;                                                                               // 004C5D3F..5D4A
    }
    return -1;                                                                                 // 004C5D54
}

// 004C7870 CSWSModule::SavePlayers(gff, s, prefix (unused), players): "Mod_PlayerList" with every live player
// creature (after SaveServerCharacter) and then every IFO player entry that is not live, loaded into a temporary
// CSWSPlayer. RET 10h, EAX 0 when the "Module" IFO resource or one of its entries is missing.
U __fastcall kotor_fastcall_004c7870(void* self, void*, void* gff, void* s, void*, void* players) {
    void* objects = recovered_object_array_004aed70(server_app(), nullptr);                    // 004C7888..789F [esp+1Ch]
    ArrayU found = {nullptr, 0, 0};                                                            // [esp+48h] data/count/alloc
    void* server = server_app();                                                               // 004C78AF..78BC EBX
    GffList list;                                                                              // [esp+58h]
    GffStruct element;                                                                         // [esp+10h]
    (void)add_list(gff, &list, s, "Mod_PlayerList");                                           // 004C78C6..78DA
    for (I i = 0; i < at<I>(players, 4); ++i) {                                                // 004C78DF..7AA1 [esp+8Ch]
        void* object = nullptr;                                                                // [esp+24h]
        if (static_cast<U8>(kotor_fastcall_004d8230(objects, nullptr, at<U*>(players, 0)[i], &object)) !=
            k_get_object_ok_0074666c) continue;                                                // 004C7900..791B
        void* creature = as_creature(object);                                                  // 004C7921..792A
        void* player = kotor_fastcall_004ae910(server, nullptr, at<U>(creature, 4));           // 004C792C..7937
        if (player == nullptr) {                                                               // 004C7939..793B
            void* party = kotor_fastcall_005ed8b0(client_app(), nullptr);                      // 004C793D..7946
            void* character = frame_GetCharacter_006346c0(party, nullptr, 0);                  // 004C7945..794D
            const U id = kotor_fastcall_004aea30(server, nullptr, at<U>(character, 4));        // 004C7952..795D
            player = kotor_fastcall_004ae910(server, nullptr, id);                             // 004C795D..7965
            if (player == nullptr) { kotor_fastcall_004ef640(creature, nullptr); continue; }   // 004C7967..7A81
        }
        (void)kotor_fastcall_005624c0(player, nullptr);                                        // 004C796F..7971
        const I index = kotor_fastcall_004c5c70(self, nullptr, player);                        // 004C7976..797B [esp+90h]
        if (index != -1) {                                                                     // 004C7980..798A (inline CExoArrayList<int>::Add)
            if (found.count == found.alloc)                                                    // 004C798C..7994
                menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(&found), nullptr,
                                                    found.alloc != 0 ? static_cast<U>(found.alloc * 2) : 0x10u); // 004C7996..79A7
            found.data[found.count] = static_cast<U>(index);                                   // 004C79B3..79BB
            ++found.count;                                                                     // 004C79BE..79BF
        }
        (void)add_element(gff, &element, &list, 0xbead);                                       // 004C79C3..79D4
        {
            Scoped name;                                                                       // [esp+38h] state 1
            (void)kotor_fastcall_005610b0(player, nullptr, name.adopt());                      // 004C79D9..79E0
            (void)field_string(gff, &element, name.p(), "Mod_CommntyName");                    // 004C79E5..79FA
        }                                                                                      // 004C79FF..7A0B
        (void)field_byte(gff, &element, at<U>(player, 0x3c), "Mod_IsPrimaryPlr");              // 004C7A10..7A20 (dword pushed)
        (void)field_loc(gff, &element, at<unsigned char*>(creature, 0xa74) + 0x34, "Mod_FirstName"); // 004C7A25..7A3B
        (void)field_loc(gff, &element, at<unsigned char*>(creature, 0xa74) + 0x3c, "Mod_LastName");  // 004C7A40..7A56
        (void)field_dword(gff, &element, at<U>(creature, 4), k_object_id_00744c24);            // 004C7A5B..7A6B
        (void)recovered_save_creature_00500610(creature, nullptr, gff, &element);              // 004C7A70..7A78
    }
    U* found_data = found.data;                                                                // 004C7AA7 EDI
    menu_resources::Ref ref;                                                                   // [esp+38h]
    ref.construct_00406d80(k_module_007442a8);                                                 // 004C7AAB..7AB4
    void* ifo = c2da_lookup_004074d0(resman(), nullptr, &ref, k_res_ifo_7de);                  // 004C7AB9..7AC9 EBX
    Scoped name; name.empty();                                                                 // 004C7ACE..7AD4 [esp+30h] state 2
    if (ifo == nullptr) { name.end(); live_free(found_data); return 0; }                       // 004C7AD9..7AE3 -> 7B25..7B4C
    (void)c2da_current_resource_demand_00409b10(ifo, nullptr);                                 // 004C7AE5..7AE7
    GffStruct top;                                                                             // [esp+28h]
    (void)kotor_fastcall_00411240(ifo, nullptr, &top);                                         // 004C7AEC..7AF3
    GffList ifo_list;                                                                          // [esp+68h]
    if (gff_list_004118c0(ifo, nullptr, &ifo_list, &top, "Mod_PlayerList") == 0) {            // 004C7AF8..7B10
        (void)c2da_release_demand_00409b80(ifo, nullptr);                                      // 004C7B12..7B14
        (void)c2da_release_00409cf0(resman(), nullptr, ifo);                                   // 004C7B19..7B20
        name.end(); live_free(found_data); return 0;                                           // 004C7B25..7B4C
    }
    for (U j = 0; j < at<U>(at<void*>(self, 0x48), 8); ++j) {                                  // 004C7B51..7D7A [esp+90h]
        bool skip = false;
        for (I k = 0; k < found.count; ++k)                                                    // 004C7B70..7B8C
            if (found_data[k] == j) { skip = true; break; }                                    // 004C7B80..7B83
        if (skip) continue;                                                                    // -> 004C7D62
        GffStruct entry;                                                                       // [esp+18h]
        if (gff_list_element_00411990(ifo, nullptr, &entry, &ifo_list, j) == 0) {              // 004C7B8E..7BA2
            (void)c2da_release_00409cf0(resman(), nullptr, ifo);                               // 004C7B19..7B20 (no CRes::Release)
            name.end(); live_free(found_data); return 0;                                       // 004C7B25..7B4C
        }
        U ok = 0;                                                                              // [esp+2Ch]
        const U id = gui_load_gff_dword_00411c20(ifo, nullptr, &entry, k_object_id_00744c24, &ok, 0x7f000000u); // 004C7BA8..7BC3
        {
            Scoped fallback; (void)kotor_fastcall_005e5bc0(fallback.adopt(), nullptr, -1);     // 004C7BC3..7BCB [esp+1Ch] state 3
            Scoped read;                                                                       // [esp+38h] state 4
            (void)gui_load_gff_string_00411ec0(ifo, nullptr, read.adopt(), &entry, "Mod_CommntyName", &ok, fallback.p()); // 004C7BD0..7BF3
            s_assign(name.p(), read.p());                                                      // 004C7BF8..7C05
        }                                                                                      // 004C7C0A..7C27
        I matches = 0;                                                                         // 004C7C2C..7C41
        for (I k = 0; k < at<I>(players, 4); ++k)
            if (at<U*>(players, 0)[k] == id) ++matches;
        if (matches != 0 || id == 0x7f000000u) continue;                                       // 004C7C43..7C51
        void* memory = live_operator_new(0x84);                                                // 004C7C57..7C61
        void* player = nullptr;
        if (memory != nullptr) {
            try { player = recovered_connection_player_00562c70(memory, nullptr, id); } catch (...) { live_free(memory); throw; } // 004C7C77..7C7A
        }
        (void)recovered_load_character_00561e30(player, nullptr, j, 1, 0);                     // 004C7C85..7C9B
        void* creature = kotor_fastcall_00561080(player, nullptr);                             // 004C7CA0..7CA2
        (void)add_element(gff, &element, &list, 0xbead);                                       // 004C7CA7..7CBA
        (void)field_string(gff, &element, name.p(), "Mod_CommntyName");                        // 004C7CBF..7CD0
        (void)field_byte(gff, &element, at<U>(player, 0x3c), "Mod_IsPrimaryPlr");              // 004C7CD5..7CE5
        (void)field_loc(gff, &element, at<unsigned char*>(creature, 0xa74) + 0x34, "Mod_FirstName"); // 004C7CEA..7D00
        (void)field_loc(gff, &element, at<unsigned char*>(creature, 0xa74) + 0x3c, "Mod_LastName");  // 004C7D05..7D1B
        (void)field_dword(gff, &element, at<U>(creature, 4), k_object_id_00744c24);            // 004C7D20..7D30
        (void)recovered_save_creature_00500610(creature, nullptr, gff, &element);              // 004C7D35..7D3D
        kotor_fastcall_004f0fd0(creature, nullptr, 0);                                         // 004C7D42..7D46
        delete_virtual(player);                                                                // 004C7D4B..7D52 (no null guard)
    }
    (void)c2da_release_demand_00409b80(ifo, nullptr);                                          // 004C7D80..7D82
    (void)c2da_release_00409cf0(resman(), nullptr, ifo);                                       // 004C7D87..7D8E
    name.end();                                                                                // 004C7D93..7D9F
    live_free(found_data);                                                                     // 004C7DA4..7DB0
    return 1;                                                                                  // 004C7DB8
}

// =========================================================================================================
// 004B0970 CServerAIMaster::SaveEventQueue(gff, s): "EventQueue" list of every queued CServerAIEventNode (+54h).
U __fastcall kotor_fastcall_004b0970(void* self, void*, void* gff, void* s) {
    GffList list;                                                                              // [esp+8]
    (void)add_list(gff, &list, s, "EventQueue");                                               // 004B0970..098C
    if (at<U>(at<void*>(self, 0x54), 8) == 0) return 1;                                        // 004B0991..0999
    void* pos = at<void*>(at<void*>(self, 0x54), 0);                                           // 004B099B..099F
    while (pos != nullptr) {                                                                   // 004B09A3..09E7
        void* node = list_at(at<void*>(self, 0x54), pos);                                      // 004B09A6..09AA
        GffStruct element;
        (void)add_element(gff, &element, &list, 0xabcd);                                       // 004B09AF..09C2
        (void)kotor_fastcall_004afea0(node, nullptr, gff, &element);                           // 004B09C7..09CF
        list_next(at<void*>(self, 0x54), &pos);                                                // 004B09D4..09DC
    }
    return 1;                                                                                  // 004B09EB
}

// 004AFEA0 CServerAIEventNode::SaveNode(gff, s): header fields, then the event data by event id (+10h) through the
// 27-entry jump table at 004B0234/004B0270. Cases 1 and 27 have no null check on the data (+14h).
U __fastcall kotor_fastcall_004afea0(void* self, void*, void* gff, void* s) {
    (void)field_dword(gff, s, at<U>(self, 0), "Day");                                          // 004AFEAD..FEB8
    (void)field_dword(gff, s, at<U>(self, 4), "Time");                                         // 004AFEBD..FEC9
    (void)field_dword(gff, s, at<U>(self, 0xc), k_object_id_00744c24);                         // 004AFECE..FEDA
    (void)field_dword(gff, s, at<U>(self, 8), "CallerId");                                     // 004AFEDF..FEEB
    (void)field_dword(gff, s, at<U>(self, 0x10), "EventId");                                   // 004AFEF0..FEFC
    void* data = at<void*>(self, 0x14);
    GffStruct child;                                                                           // argument slot [esp+14h]
    const char* const label = "EventData";
    switch (at<U>(self, 0x10)) {                                                               // 004AFF01..FF15
    case 1:                                                                                    // 004B0154 (no null check)
        (void)add_struct(gff, &child, s, label, 0x7777);
        (void)recovered_vm_save_situation_005d10f0(reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::message_manager_007a3a00)),
                                                   nullptr, at<void*>(self, 0x14), gff, &child);
        break;
    case 4:                                                                                    // 004AFF92
        if (data == nullptr) break;
        (void)add_struct(gff, &child, s, label, 0x9999);
        (void)field_byte(gff, &child, at<U8>(self, 0x14), "Value");
        break;
    case 5: case 14:                                                                           // 004AFF1C
        if (data == nullptr) break;
        (void)add_struct(gff, &child, s, label, 0x1111);
        (void)recovered_save_game_effect_00503790(at<void*>(self, 0x14), nullptr, gff, &child);
        break;
    case 8: case 19:                                                                           // 004B0119
        if (data == nullptr) break;
        (void)add_struct(gff, &child, s, label, 0x6666);
        (void)kotor_fastcall_004ee230(at<void*>(self, 0x14), nullptr, gff, &child);
        break;
    case 9:                                                                                    // 004B0053 -> 004B0202
        if (data == nullptr) break;
        (void)add_struct(gff, &child, s, label, 0x3333);
        (void)field_int(gff, &child, at<U>(self, 0x14), "Value");
        break;
    case 10:                                                                                   // 004B0068
        if (data == nullptr) break;
        (void)add_struct(gff, &child, s, label, 0x4444);
        (void)kotor_fastcall_004d73a0(at<void*>(self, 0x14), nullptr, gff, &child);
        break;
    case 15: case 21:                                                                          // 004AFF57
        if (data == nullptr) break;
        (void)add_struct(gff, &child, s, label, 0x2222);
        (void)recovered_attack_data_save_004d2210(at<void*>(self, 0x14), nullptr, gff, &child);
        break;
    case 17:                                                                                   // 004B00DE
        if (data == nullptr) break;
        (void)add_struct(gff, &child, s, label, 0x5555);
        (void)kotor_fastcall_004af160(at<void*>(self, 0x14), nullptr, gff, &child);
        break;
    case 18:                                                                                   // 004B00A3
        if (data == nullptr) break;
        (void)add_struct(gff, &child, s, label, 0x8888);
        (void)kotor_fastcall_005034f0(at<void*>(self, 0x14), nullptr, gff, &child);
        break;
    case 20:                                                                                   // 004AFFD6
        if (data == nullptr) break;
        (void)add_struct(gff, &child, s, label, 0x3333);
        (void)field_dword(gff, &child, at<U>(self, 0x14), "Value");
        break;
    case 22:                                                                                   // 004B0018
        if (data == nullptr) break;
        (void)add_struct(gff, &child, s, label, 0xcccc);
        (void)kotor_fastcall_004d64e0(at<void*>(self, 0x14), nullptr, gff, &child);
        break;
    case 24:                                                                                   // 004B018B
        if (data == nullptr) break;
        (void)add_struct(gff, &child, s, label, 0xdddd);
        (void)kotor_fastcall_004d73a0(at<void*>(self, 0x14), nullptr, gff, &child);
        break;
    case 26:                                                                                   // 004B01C6
        if (data == nullptr) break;
        (void)add_struct(gff, &child, s, label, 0xeeee);
        (void)kotor_fastcall_004d73a0(at<void*>(self, 0x14), nullptr, gff, &child);
        break;
    case 27:                                                                                   // 004B01FD -> 004B0202 (no null check)
        (void)add_struct(gff, &child, s, label, 0xffff);
        (void)field_int(gff, &child, at<U>(self, 0x14), "Value");
        break;
    default:                                                                                   // 004B0229 (2,3,6,7,11..13,16,23,25, >27)
        break;
    }
    return 1;
}

// 004D64E0 CSWCCMessageData::SaveData(gff, s): type byte and the four parameter lists (always written).
U __fastcall kotor_fastcall_004d64e0(void* self, void*, void* gff, void* s) {
    (void)field_byte(gff, s, at<U8>(self, 0), "Type");                                         // 004D64EC..64FE
    GffList list;                                                                              // [esp+10h]
    GffStruct element;                                                                         // argument slot
    (void)add_list(gff, &list, s, "IntList");                                                  // 004D6503..6510
    for (I i = 0; i < at<I>(self, 8); ++i) {                                                   // 004D6515..6554
        (void)add_element(gff, &element, &list, 0xbaad);
        (void)field_int(gff, &element, at<U*>(self, 4)[i], "IntegerValue");
    }
    (void)add_list(gff, &list, s, "FloatList");                                                // 004D6556..6563
    for (I i = 0; i < at<I>(self, 0x14); ++i) {                                                // 004D6568..65A5
        (void)add_element(gff, &element, &list, 0xbaad);
        (void)field_float(gff, &element, at<U*>(self, 0x10)[i], "FloatValue");
    }
    (void)add_list(gff, &list, s, "ObjectIDList");                                             // 004D65A7..65B4
    for (I i = 0; i < at<I>(self, 0x2c); ++i) {                                                // 004D65B9..65F6
        (void)add_element(gff, &element, &list, 0xbaad);
        (void)field_dword(gff, &element, at<U*>(self, 0x28)[i], "ObjectValue");
    }
    (void)add_list(gff, &list, s, "StringList");                                               // 004D65F8..6605
    for (I i = 0; i < at<I>(self, 0x20); ++i) {                                                // 004D660A..6647
        (void)add_element(gff, &element, &list, 0xbaad);
        (void)field_string(gff, &element, at<unsigned char*>(self, 0x1c) + static_cast<U>(i) * 8, "StringValue");
    }
    return 1;                                                                                  // 004D664C
}

// 004D73A0 CScriptEvent::SaveEvent(gff, s): event type word and the non-empty parameter lists.
U __fastcall kotor_fastcall_004d73a0(void* self, void*, void* gff, void* s) {
    (void)field_word(gff, s, at<U16>(self, 0), "EventType");                                   // 004D73AC..73BF
    GffList list;                                                                              // [esp+10h]
    GffStruct element;                                                                         // argument slot
    if (at<I>(self, 8) > 0) {                                                                  // 004D73C4..73C9
        (void)add_list(gff, &list, s, "IntList");                                              // 004D73CB..73D8
        for (I i = 0; i < at<I>(self, 8); ++i) {                                               // 004D73DD..7417
            (void)add_element(gff, &element, &list, 0x69);
            (void)field_int(gff, &element, at<U*>(self, 4)[i], "Parameter");
        }
    }
    if (at<I>(self, 0x14) > 0) {                                                               // 004D7419..741E
        (void)add_list(gff, &list, s, "FloatList");                                            // 004D7420..742D
        for (I i = 0; i < at<I>(self, 0x14); ++i) {                                            // 004D7432..7471
            (void)add_element(gff, &element, &list, 0x69);
            (void)field_float(gff, &element, at<U*>(self, 0x10)[i], "Parameter");
        }
    }
    if (at<I>(self, 0x20) > 0) {                                                               // 004D7473..7478
        (void)add_list(gff, &list, s, "StringList");                                           // 004D747A..7487
        for (I i = 0; i < at<I>(self, 0x20); ++i) {                                            // 004D748C..74C6
            (void)add_element(gff, &element, &list, 0x69);
            (void)field_string(gff, &element, at<unsigned char*>(self, 0x1c) + static_cast<U>(i) * 8, "Parameter");
        }
    }
    if (at<I>(self, 0x2c) > 0) {                                                               // 004D74C8..74CD
        (void)add_list(gff, &list, s, "ObjectList");                                           // 004D74CF..74DC
        for (I i = 0; i < at<I>(self, 0x2c); ++i) {                                            // 004D74E1..7521
            (void)add_element(gff, &element, &list, 0x69);
            (void)field_dword(gff, &element, at<U*>(self, 0x28)[i], "Parameter");
        }
    }
    return 1;                                                                                  // 004D7526
}

// 005034F0 CSWSForcedAction::SaveForcedAction(gff, s).
U __fastcall kotor_fastcall_005034f0(void* self, void*, void* gff, void* s) {
    (void)field_dword(gff, s, at<U>(self, 0), "ActionId");                                     // 005034FD..3508
    (void)field_word(gff, s, at<U16>(self, 4), "GroupId");                                     // 0050350D..351C
    (void)field_dword(gff, s, at<U>(self, 8), "Target_Area");                                  // 00503521..352D
    (void)field_dword(gff, s, at<U>(self, 0x18), "Target_Object");                             // 00503532..353E
    (void)field_float(gff, s, at<U>(self, 0xc), "Target_LocX");                                // 00503543..354F
    (void)field_float(gff, s, at<U>(self, 0x10), "Target_LocY");                               // 00503554..3560
    (void)field_float(gff, s, at<U>(self, 0x14), "Target_LocZ");                               // 00503565..3571
    return 1;
}

// 004AF160 CServerAIBodyBagInfo::SaveBodyBag(gff, s).
U __fastcall kotor_fastcall_004af160(void* self, void*, void* gff, void* s) {
    (void)field_dword(gff, s, at<U>(self, 0), "BodyBagId");                                    // 004AF16D..F178
    (void)field_float(gff, s, at<U>(self, 4), "PositionX");                                    // 004AF17D..F189
    (void)field_float(gff, s, at<U>(self, 8), "PositionY");                                    // 004AF18E..F19A
    (void)field_float(gff, s, at<U>(self, 0xc), "PositionZ");                                  // 004AF19F..F1AB
    return 1;
}

// 004EE230 CSWSSpellScriptData::SaveData(gff, s).
U __fastcall kotor_fastcall_004ee230(void* self, void*, void* gff, void* s) {
    (void)field_int(gff, s, at<U>(self, 0), "SpellId");                                        // 004EE23D..E248
    (void)field_dword(gff, s, at<U>(self, 4), "CasterId");                                     // 004EE24D..E259
    (void)field_dword(gff, s, at<U>(self, 8), "TargetId");                                     // 004EE25E..E26A
    (void)field_dword(gff, s, at<U>(self, 0x24), "AreaId");                                    // 004EE26F..E27B
    (void)field_dword(gff, s, at<U>(self, 0xc), "ItemId");                                     // 004EE280..E28C
    (void)field_string(gff, s, static_cast<unsigned char*>(self) + 0x1c, "Script");            // 004EE291..E29D
    (void)field_float(gff, s, at<U>(self, 0x10), "TargetPosX");                                // 004EE2A2..E2AE
    (void)field_float(gff, s, at<U>(self, 0x14), "TargetPosY");                                // 004EE2B3..E2BF
    (void)field_float(gff, s, at<U>(self, 0x18), "TargetPosZ");                                // 004EE2C4..E2D0
    return 1;
}

// =========================================================================================================
// 0052B790 CFactionManager::SaveFactions(gff, list): factions 0..count (+1Ch) inclusive; an index outside the
// faction array (+24h) yields a null faction that is dereferenced unchecked (original).
U __fastcall kotor_fastcall_0052b790(void* self, void*, void* gff, void* list) {
    for (I i = 0; i < at<I>(self, 0x1c) + 1; ++i) {                                            // 0052B793..B814 (count re-read)
        unsigned char* faction = nullptr;                                                      // 0052B7A4..B7B7
        if (i >= 0 && i < at<I>(at<void*>(self, 0x24), 4)) faction = at<unsigned char**>(at<void*>(self, 0x24), 0)[i];
        GffStruct element;                                                                     // argument slot
        (void)add_element(gff, &element, list, static_cast<U>(i));                             // 0052B7B9..B7C6
        (void)field_string(gff, &element, faction + 0x18, "FactionName");                      // 0052B7CB..B7DB
        (void)field_dword(gff, &element, at<U>(faction, 0x10), "FactionParentID");             // 0052B7E0..B7F0
        (void)field_word(gff, &element, at<U16>(faction, 0x20), "FactionGlobal");              // 0052B7F5..B808
    }
    return 1;                                                                                  // 0052B819
}

// 0052B830 CFactionManager::SaveReputations(gff, list): every (ID1, ID2) pair in 0..count inclusive whose NPC
// reputation is not 100, numbered by a running element counter.
U __fastcall kotor_fastcall_0052b830(void* self, void*, void* gff, void* list) {
    U element_index = 0;                                                                       // [esp+4]
    if (at<I>(self, 0x1c) < 0) return 1;                                                       // 0052B833..B843
    for (I a = 0; a <= at<I>(self, 0x1c); ++a) {                                               // 0052B850..B8C8 EBP
        for (I b = 0; b <= at<I>(self, 0x1c); ++b) {                                           // 0052B850..B8C0 EDI
            const U reputation = recovered_faction_npc_reputation_0052b420(self, nullptr, static_cast<U>(b), static_cast<U>(a)); // 0052B856..B85D
            if (reputation == 100) continue;                                                   // 0052B85F..B862
            GffStruct element;                                                                 // argument slot
            (void)add_element(gff, &element, list, element_index);                             // 0052B864..B875
            (void)field_dword(gff, &element, static_cast<U>(a), "FactionID1");                 // 0052B87A..B887
            (void)field_dword(gff, &element, static_cast<U>(b), "FactionID2");                 // 0052B88C..B899
            (void)field_dword(gff, &element, reputation, "FactionRep");                        // 0052B89E..B8AB
            ++element_index;                                                                   // 0052B8B0
        }
    }
    return 1;                                                                                  // 0052B8CD
}

// =========================================================================================================
// 005610B0 CSWSPlayer::GetPlayerName(CExoString* out): the net layer's player info name (+4), else "". RET 4.
void* __fastcall kotor_fastcall_005610b0(void* self, void*, void* out) {
    void* net = vcall<void*>(server_app(), 0x10);                                              // 005610B1..610CA (CServerExoApp vtable +10h)
    void* info = kotor_fastcall_005d5170(net, nullptr, at<U>(self, 4));                        // 005610BF..610CF
    if (info != nullptr) (void)WindowCopy005e5b00(out, nullptr, static_cast<unsigned char*>(info) + 4); // 005610D4..610E2
    else s_text(out, k_empty_0073d71c);                                                        // 005610EE..610F3
    return out;
}

// 004EF640 CSWSCreature::DumpToLog: evaluates the full name and the six ability scores; the log output is compiled
// out in the shipped executable, so nothing is written.
void __fastcall kotor_fastcall_004ef640(void* self, void*) {
    {
        Scoped name;                                                                           // [esp+4]
        (void)cstr(kotor_fastcall_005a4c60(at<void*>(self, 0xa74), nullptr, name.adopt()));    // 004EF65B..F675
    }                                                                                          // 004EF67A..F686
    (void)recovered_creature_stats_str_005a6190(at<void*>(self, 0xa74), nullptr);              // 004EF68B..F691
    (void)recovered_creature_stats_int_005a6310(at<void*>(self, 0xa74), nullptr);              // 004EF696..F69C
    (void)recovered_creature_stats_wis_005a63d0(at<void*>(self, 0xa74), nullptr);              // 004EF6A1..F6A7
    (void)recovered_creature_stats_dex_005a6550(at<void*>(self, 0xa74), nullptr);              // 004EF6AC..F6B2
    (void)recovered_creature_stats_con_005a6250(at<void*>(self, 0xa74), nullptr);              // 004EF6B7..F6BD
    (void)recovered_creature_stats_cha_005a6490(at<void*>(self, 0xa74), nullptr);              // 004EF6C2..F6C8
}

// 005624C0 CSWSPlayer::SaveServerCharacter: for player types 3/4 writes the controlled creature as a BIC into
// SERVERVAULT: (resref +65h, or a fresh unique "<name><n>" when it is empty or the write failed). RET 0, EAX 0/1.
U __fastcall kotor_fastcall_005624c0(void* self, void*) {
    unsigned char* p = static_cast<unsigned char*>(self);
    Scoped resref_text; resref_text.empty();                                                   // [esp+34h] state 0
    Scoped path; path.empty();                                                                 // [esp+3Ch] state 1
    Scoped vault; vault.empty();                                                               // [esp+4Ch] state 2
    Scoped base; base.empty();                                                                 // [esp+2Ch] state 3
    I suffix = 1;                                                                              // [esp+18h]
    Scoped number; number.empty();                                                             // [esp+44h] state 4
    struct ListGuard {                                                                         // [esp+84h] CExoArrayList<CExoString> state 5
        void* data = nullptr; I count = 0; I alloc = 0;
        ~ListGuard() { kotor_fastcall_00405920(this, nullptr); }                               // inline copies are identical to 00405920
    } files;
    const U8 type = at<U8>(self, 0x64);                                                        // 00562553
    if (type != 3 && type != 4) return 0;                                                      // 00562556..5625CC
    void* object = kotor_fastcall_004ae750(server_app(), nullptr, at<U>(self, 0x28));          // 00562566..62572
    if (object == nullptr || !(at<U8>(object, 8) > k_creature_type_floor_00746633)) object = nullptr; // 00562577..62586
    unsigned char* creature = static_cast<unsigned char*>(as_creature(object));                // 00562588..6258C (null not guarded)
    if (creature == nullptr) return 0;                                                         // 0056258F..625CC
    if (at<U>(creature, 0xa48) == 1 && at<I>(creature, 0x128) > 0) {                           // 005625D1..625E1
        void** effects = at<void**>(creature, 0x124);
        for (I i = 0; i < at<I>(creature, 0x128); ++i) {                                       // 005625E3..62606
            if (static_cast<U>(at<U16>(effects[i], 8)) != k_effect_type_0074677_3e) continue;  // 005625F0..625FC
            recovered_effect_integer_005036a0(at<void**>(creature, 0x124)[i], nullptr, 2, 1);  // 00562608..6260F
            void* effect = at<void**>(creature, 0x124)[i];                                     // 00562614..6261A
            (void)recovered_object_remove_effect_004d06c0(creature, nullptr, at<U>(effect, 0), at<U>(effect, 4)); // 0056261D..62626
            break;
        }
    }
    void* gff = new_gff();                                                                     // 0056262D..6265B [esp+14h]
    void* root = live_operator_new(4);                                                         // 0056265B..62676 EDI
    U failed;
    {
        Scoped version; version.text(k_v20_00745338);                                          // 0056266D..62678 [esp+1Ch]
        Scoped kind; kind.text("BIC ");                                                        // 0056267D..6268E [esp+24h]
        failed = recovered_gff_00411260(gff, nullptr, root, kind.p(), version.p()) == 0 ? 1u : 0u; // 00562693..626B9
    }                                                                                          // 005626B5..626D4
    if (failed != 0) {                                                                         // 005626D9..626DB
        if (gff != nullptr) delete_virtual(gff);                                               // 005626DD..626E9
        live_free(root);                                                                       // 005626EB..626EC
        return 0;                                                                              // 005626F1..6272F
    }
    if (at<U>(creature, 0x9dc) != 0) {                                                         // 00562734..6273F
        at<U>(creature, 0xf8) = 0;                                                             // 00562746
        const U saved = recovered_save_creature_00500610(creature, nullptr, gff, root);        // 00562741..62750
        if (saved == 0) {                                                                      // 00562755..62757
            if (gff != nullptr) delete_virtual(gff);                                           // 00562759..62763
            live_free(root);                                                                   // 00562765..62766
            at<U>(creature, 0xf8) = 1;                                                         // 0056276E
            return 0;                                                                          // 00562778 -> 00562AFD
        }
        at<U>(creature, 0xf8) = 1;                                                             // 0056277D
    } else if (recovered_save_creature_00500610(creature, nullptr, gff, root) == 0) {          // 00562789..62795
        if (gff != nullptr) delete_virtual(gff);                                               // 00562797..627A3
        live_free(root);                                                                       // 005627A5..627A6
        return 0;                                                                              // 005627AE -> 00562AFD
    }
    live_free(root);                                                                           // 005627B3..627B8
    unsigned char* resref = p + 0x65;                                                          // 005627C0 [esp+1Ch]
    if (c2da_not_equal_text_00406060(resref, nullptr, k_empty_0073d71c) != 0) {                // 005627C3..627D5
        (void)c2da_ref_to_string_00405f70(resref, nullptr, resref_text.p());                   // 005627DB..627E2
        if (at<U8>(self, 0x64) == 4) {                                                         // 005627E7..627EB
            Scoped name;                                                                       // [esp+54h] state 9
            (void)kotor_fastcall_005610b0(self, nullptr, name.adopt());                        // 005627ED..627F4
            frame_LogFormat_005e5680(path.p(), "SERVERVAULT:%s%s%s", cstr(name.p()), k_backslash_0073d8f4,
                                     cstr(resref_text.p()));                                   // 005627F9..62824
        } else {                                                                               // 00562829..62838 (~name)
            frame_LogFormat_005e5680(path.p(), k_server_vault_00745340, cstr(resref_text.p())); // 00562843..62857
        }
        if (recovered_gff_00413030(gff, nullptr, path.p(), k_res_bic_7df) != 0) {              // 0056285F..62872
            menu_resources::Ref fresh; menu_resources::Ref discard;                            // [esp+74h] / [esp+64h]
            (void)async_ref_construct_string_00406d60(&fresh, nullptr, resref_text.p());      // 00562874..6287D
            (void)ref_assign_copy_00406120(resref, nullptr, &discard, &fresh);                 // 00562882..6288E
            goto written;                                                                      // 00562893 -> 00562BA6
        }
    }
    at<U8>(self, 0x64) = 4;                                                                    // 0056289F
    {
        Scoped name;                                                                           // [esp+54h] state 0Ah
        (void)kotor_fastcall_005610b0(self, nullptr, name.adopt());                            // 00562898..628A3
        frame_LogFormat_005e5680(vault.p(), k_server_vault_00745340, cstr(name.p()));         // 005628A8..628C2
    }                                                                                          // 005628CA..628D6
    {
        Scoped full;                                                                           // [esp+54h] state 0Bh
        (void)kotor_fastcall_005a4c60(at<void*>(creature, 0xa74), nullptr, full.adopt());      // 005628DB..628E8
        s_assign(base.p(), full.p());                                                          // 005628ED..628FA
    }                                                                                          // 005628FF..6290B
    (void)kotor_fastcall_005e5950(base.p(), nullptr, 1, 0, 0);                                 // 00562910..6291A
    if (c2da_current_string_index_005e5600(base.p(), nullptr, 0) == 0) {                       // 0056291F..62930
        (void)frame_LogAssign_005e5140(base.p(), nullptr, "0");                                // 00562932..62937
    } else {
        Scoped head;                                                                           // [esp+54h] state 0Ch
        (void)startup_rand::SubString005e6270(str(base.p()), nullptr, str(head.adopt()), 0, 0x10); // 0056293E..62947
        s_assign(base.p(), head.p());                                                          // 0056294C..62959
    }                                                                                          // 0056295E..6296A
    s_assign(resref_text.p(), base.p());                                                       // 0056296F..62978
    f200_ports::Enumerate005e6640(kotor_menu_10_19::dependencies::base_007a39e0, nullptr, &files, str(vault.p()),
                                  k_res_bic_7df, 0, 0);                                        // 0056297D..62999
    for (;;) {                                                                                 // 005629A0..62AA4
        I count;
        {
            Scoped ext; ext.text(".bic");                                                      // 005629A0..629A9 [esp+54h] state 0Dh
            options_video::String candidate;                                                   // by-value argument slot (sub esp,8)
            s_concat(resref_text.p(), &candidate, ext.p());                                    // 005629AE..629C9
            count = client_intro::CountStrings_004b2640(&files, nullptr, candidate);           // 005629CE..629D5 (callee destroys it)
        }                                                                                      // 005629DA..629E8
        if (count == 0) break;                                                                 // 005629ED..629EF
        {
            Scoped digits;                                                                     // [esp+5Ch] state 0Eh
            (void)kotor_fastcall_005e5bc0(digits.adopt(), nullptr, suffix);                    // 005629F5..629FE
            s_assign(number.p(), digits.p());                                                  // 00562A03..62A10
        }                                                                                      // 00562A15..62A21
        const I keep = 0x10 - static_cast<I>(async_string_length_005e5790(number.p(), nullptr)); // 00562A26..62A34
        {
            Scoped head;                                                                       // [esp+64h] state 0Fh
            (void)startup_rand::SubString005e6270(str(base.p()), nullptr, str(head.adopt()), 0, keep); // 00562A36..62A42
            s_assign(base.p(), head.p());                                                      // 00562A47..62A53
        }                                                                                      // 00562A58..62A64
        {
            Scoped joined;                                                                     // [esp+74h] state 10h
            s_concat(base.p(), joined.adopt(), number.p());                                    // 00562A69..62A77
            s_assign(resref_text.p(), joined.p());                                             // 00562A7C..62A89
        }                                                                                      // 00562A8E..62A9A
        ++suffix;                                                                              // 00562A9F..62AA0
    }
    frame_LogFormat_005e5680(path.p(), "%s%s%s", cstr(vault.p()), k_backslash_0073d8f4, cstr(resref_text.p())); // 00562AA9..62ACC
    if (recovered_gff_00413030(gff, nullptr, path.p(), k_res_bic_7df) == 0) {                  // 00562AD1..62AEB
        if (gff != nullptr) delete_virtual(gff);                                               // 00562AF1..62AFB
        return 0;                                                                              // 00562AFD..62B80
    }
    {
        menu_resources::Ref fresh; menu_resources::Ref discard;                                // [esp+74h] / [esp+64h]
        (void)async_ref_construct_string_00406d60(&fresh, nullptr, resref_text.p());          // 00562B81..62B8A
        (void)ref_assign_copy_00406120(resref, nullptr, &discard, &fresh);                     // 00562B8F..62B9D
    }
written:
    {
        Scoped vault_root; vault_root.text("SERVERVAULT:");                                    // 00562BA6..62BAF [esp+54h] state 11h
        (void)kotor_fastcall_004088e0(resman(), nullptr, vault_root.p());                      // 00562BB4..62BC7
    }                                                                                          // 00562BCC..62BD8
    if (gff != nullptr) delete_virtual(gff);                                                   // 00562BDD..62BE7
    return 1;                                                                                  // 00562BE9..62C6F (~files, ~strings)
}

// =========================================================================================================
// 0056C910 CSWSMessage::SendServerToPlayerLoadBar_UpdateStallEvent(a, b): message 2Ch/2 to player FFFFFFF7h with
// two 32-bit values. RET 8, EAX = send result, or 0 when no message was produced.
U __fastcall kotor_fastcall_0056c910(void* self, void*, U a, U b) {
    kotor_fastcall_004d68a0(self, nullptr, 4, 0xffffffffu, 1);                                 // 0056C914..C91C
    kotor_fastcall_004d6340(self, nullptr, a, 0x20);                                           // 0056C921..C92A
    kotor_fastcall_004d6340(self, nullptr, b, 0x20);                                           // 0056C92F..C938
    void* data = nullptr; U size = 0;                                                          // [esp+8] / [esp+4]
    if (kotor_fastcall_004d6440(self, nullptr, &data, &size) == 0) return 0;                   // 0056C93D..C956
    return kotor_fastcall_00567e60(self, nullptr, k_load_bar_player_0074c52c, 0x2c, 2, data, size); // 0056C959..C96F
}

// 0062AD30 CGuiInGame::GetPlaceableCamera(int index): null when index > count (+1B0h, signed jle: index == count is
// allowed), else this + 1B4h + 30h * index. RET 4.
void* __fastcall kotor_fastcall_0062ad30(void* self, void*, I index) {
    if (index > at<I>(self, 0x1b0)) return nullptr;                                            // 0062AD30..AD3E
    return static_cast<unsigned char*>(self) + static_cast<U>(index) * 0x30 + 0x1b4;           // 0062AD41..AD4E
}
}  // extern "C"
}  // namespace save_module
