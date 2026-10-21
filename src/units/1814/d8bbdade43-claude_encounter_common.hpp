#pragma once
// Lane "encounter": shared declarations for the CSWSEncounter reconstruction (claude_encounter*.cpp), transcribed from
// the original swkotor.exe (read-only, capstone). Conventions as the other srcw5\actions files: original __thiscall ->
// extern "C" __fastcall(self, unused_edx, ...); x87 at 53-bit precision (double intermediates, float where the original
// stores a dword); game heap only (live_operator_new / live_free = 006FA7E6 / 006FA390); globals by symbol.
//
// CSWSEncounter layout (0x31C bytes, object type 0Dh; from the ctor 00593C70, dtor 00593380 and the loaders):
//   +000 CSWSObject base (vtable 00749D98), +004 id, +018 tag, +0A8/+0AC last AIUpdate world time (day, ms),
//   +0CC ms since the previous AIUpdate, +22C =1 (ctor), +230 CExoLocString LocalizedName (8 bytes),
//   +238 Active, +23C Reset, +240 ResetTime (s), +244 SpawnOption ("SpawnOption" 00749D34), +248 difficulty modifier (SetDifficulty),
//   +24C DifficultyIndex, +250 RecCreatures, +254 MaxCreatures, +258/+25C/+260 CExoArrayList<CEncounterListEntry*>
//   spawn list, +264 active creature count, +268/+26C last heartbeat (day, ms), +270/+274 exhausted time (day, ms),
//   +278 spawned flag, +27C ?, +280 capacity / +284 data / +288 size of the activate-area id list (int[]),
//   +28C "activated" flag (set by UpdateActivateAreaList), +290 Vector[] geometry vertices / +294 count,
//   +298 int[] polygon indices / +29C count, +2A0.. (see loaders), +2B0 CEncounterListEntry[] creature list (new[]
//   with count cookie) / +2B4 count, +2B8 EncounterSpawnPoint[] (0x10 each) / +2BC count, +2C0 spawn level,
//   +2C4 Respawns ("Respawns" 00749D40; max respawns, script 0x117), +2C8 current respawn count (0x119), +2CC ?, +2D0 ?, +2D4 enemy points,
//   +2D8 spawn pool points, +2DC active points, +2E0 last entered id, +2E4 last left id,
//   +2E8 CExoString[5] OnEntered / OnExit / OnHeartbeat / OnExhausted / OnUserDefined, +310 CExoArrayList<float>*
//   fractional CR thresholds (LoadFractionalCRData), +314 last user-defined event number, +318 PlayerOnly.
#include "842d6b1618-w5_t3.hpp"

namespace frame_control { extern unsigned message_manager_007a3a00; }                        // [007A3A00] CVirtualMachine*
namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } }           // [007A39E0] CExoBase*
namespace kotor_movie { void* __fastcall string_element_construct_005b3190(void* string, void*); }   // CExoString() [0603]
namespace menu_resources { struct Ref { Ref* construct_00406d80(const char* text); }; }       // CResRef(char const*) [0030]
namespace kotor_q05 { struct StructHandle; struct CResGFF { unsigned GetElementType(StructHandle* s); }; } // 004111C0 [0290]

extern "C" {
using w5::U; using w5::I; using w5::U8; using w5::U16; using w5::Vector;
extern void* kotor_cell_rules_007a3a28;                                                         // [007A3A28] CSWRules*
// ---- CR -> points table (.bss 00832860 data / 00832864 size / 00832868 capacity) ----
extern float* w5_cell_encounter_points_table_00832860;                                          // [claude_w5_creature_actions]
extern I w5_cell_encounter_points_size_00832864;                                                // [claude_encounter]
extern I w5_cell_encounter_points_capacity_00832868;                                            // [claude_encounter]

// ---- existing providers (c40 map; object in brackets) ----
void* __fastcall kotor_fastcall_004cfcb0(void* self, void*, U type, U id, U flag);              // CSWSObject() [claude_aoe_base]
void __fastcall recovered_object_destroy_004d0220(void* self, void*);                           // ~CSWSObject [0963]
U __fastcall kotor_fastcall_004b0850(void* ai, void*, void* object, I level);                   // CServerAIMaster::AddObject [claude_aoe_base]
U __fastcall recovered_ai_remove_004af3d0(void* ai, void*, void* object);                       // CServerAIMaster::RemoveObject [1070]
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);                      // GetServerAIMaster
void* __fastcall server_get_module_004ae6b0(void* server, void*);                               // GetModule [0530]
void* __fastcall kotor_fastcall_004aede0(void* server, void*);                                  // GetWorldTimer [0457]
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, U* day, U* time);               // CWorldTimer::GetWorldTime [0457]
U __fastcall recovered_world_time_difference_004ae460(void* timer, void*, U day1, U time1, U day2, U time2,
                                                      U* out_day, U* out_time);                 // SubtractWorldTimes [1025b]
unsigned long long __fastcall recovered_bridge_exo_timers_high_resolution_005e9260(void* timers, void*); // [1204-21]
U __fastcall recovered_module_remove_tag_004c5ef0(void* module, void*, void* tag, U id);        // [1011]
U __fastcall recovered_module_register_tag_004c7de0(void* module, void*, void* tag, U id);      // [1011]
void* __fastcall recovered_object_area_004cb120(void* object, void*);                           // GetArea [1060]
void __fastcall kotor_fastcall_004cb160(void* object, void*, void* area);                      // SetArea [claude_combat_spell_mine]
unsigned __fastcall recovered_area_insert_0050dfd0(void* area, void*, unsigned id, unsigned flag); // AddObjectToArea [1065]
U __fastcall kotor_fastcall_0050dc40(void* area, void*, U id);                                  // RemoveObjectFromArea [claude_combat_spell_mine]
void __fastcall recovered_int_list_remove_0066dcc0(void* list, void*, U value);                 // CExoArrayList<int>::Remove [w1_native_b7]
U __fastcall recovered_object_run_actions_0057f4a0(void* object, void*, U day, U time, U hr_low, U hr_high); // RunActions
void* __fastcall exo_locstring_construct_005e9e10(void* self, void*);                           // CExoLocString() [0265]
void __fastcall exo_locstring_destroy_005ea000(void* string, void*);                            // ~CExoLocString [0265]
void* __fastcall recovered_localized_assign_005e9e70(void* l, void*, const void* src);          // CExoLocString::operator= [0886]
void* __fastcall frame_LogStringCtor_005e5a90(void* self, void*, const char* text);             // CExoString(char const*)
void __fastcall frame_LogStringDtor_005e5c20(void* self, void*);                                // ~CExoString
void* __fastcall async_assign_string_005e5c50(void* self, void*, void* other);                  // CExoString::operator=
const char* __fastcall frame_LogCStr_005e5670(void* s, void*);                                  // CExoString::CStr
void __cdecl frame_LogFormat_005e5680(void* self, const char* format, ...);                     // CExoString::Format
void* __fastcall kotor_fastcall_00405ed0(void* ref, void*);                                     // CResRef() [0263]
void* __fastcall ref_assign_copy_00406120(void* self, void*, void* tmp, const void* other);     // CResRef::operator=(CResRef const&) [0278]
void* __fastcall unresolved_ref_assign_00406290(void* self, void*, void* tmp, const char* text); // CResRef::operator=(char const*) [0081]
void __fastcall c2da_ref_to_string_00405f70(void* ref, void*, void* out_string);                // CResRef::CopyToString
U __fastcall c2da_current_get_float_00413430(void* table, void*, U row, void* column_name, float* out); // C2DA::GetFLOATEntry [0376] (as w5_t2.hpp)
U __fastcall c2da_current_get_int_00413660(void* table, void*, U row, void* column, I* out);   // C2DA::GetINTEntry [0376]
void* __fastcall recovered_event_construct_004d7540(void* event, void*);                        // CScriptEvent() [1017]
void __fastcall recovered_event_destroy_004d7590(void* event, void*);                           // ~CScriptEvent
void __fastcall recovered_event_object_004d7780(void* event, void*, U index, U id);             // SetObjectID [1017]
U __fastcall recovered_message_get_object_004d64c0(void* data, void*, I index);                 // CSWCCMessageData::GetObjectID
I __fastcall recovered_message_get_integer_004d6480(void* data, void*, I index);                // CSWCCMessageData::GetInteger
unsigned __fastcall recovered_event_enqueue_004b08d0(void* ai, void*, U day, U ms, U caller, U target, U type, void* event);
void __fastcall frame_R4_005d0fc0(void* vm, void*, void* script, U id, U flag);                 // CVirtualMachine::RunScript
void __cdecl genuine_x87_pow_006fc1f0();                                                        // CRT _CIpow (ST1^ST0 -> ST0) [0365]
void __stdcall crt_eh_vector_constructor_006fb55f(void* first, U size, I count, void* construct, void* destroy);
void __stdcall c2da_destroy_array_006fa852(void* first, U size, I count, void* destroy);
void __fastcall kotor_m15e_slot090_void0_004015a0(void*, void*);                               // void_return 004015A0 [0832]
// existing encounter bodies of other lanes
U __fastcall recovered_encounter_in_activate_area_00590090(void* e, void*, const Vector* pos);  // GetInActivateArea [w2_party_native]
U __fastcall recovered_encounter_segment_intersect_0058ffd0(void* e, void*, const Vector* a, const Vector* b); // [w2_party_native]
double __fastcall kotor_fastcall_005919e0(void* encounter, void*, float cr);                    // CalculatePointsFromCR [claude_w5_creature_actions]
void __fastcall kotor_fastcall_00594310(void* encounter, void*, float cr, U flag);              // RemoveFromActiveCreatureCount [same]
void* __fastcall kotor_fastcall_004ae7f0(void* server, void*, U id);                            // GetEncounterByGameObjectID [same]
U __fastcall kotor_fastcall_00591350(void* self, void*, void* gff, void* s);                    // SaveEncounter [claude_save_git]

// ---- lane "encounter" exports ----
struct EncounterListEntry;  // 0x24 bytes, see below
void* __fastcall kotor_fastcall_0058ff80(void* self, void*);                                    // CEncounterListEntry()  RET 0
void* __fastcall kotor_fastcall_0058ffc0(void* self, void*);                                    // EncounterSpawnPoint()  RET 0
void* __fastcall kotor_fastcall_00590250(void* self, void*, const void* other);                 // CEncounterListEntry(const&) RET 4
U __fastcall kotor_fastcall_00590290(void* self, void*, U id);                                  // ActivateAreaListContains RET 4
void __fastcall kotor_fastcall_005902d0(void* self, void*, U id);                               // AddToActivateAreaList RET 4
void __fastcall kotor_fastcall_005903b0(void* self, void*, U id);                               // RemoveFromActivateAreaList RET 4
U __fastcall kotor_fastcall_00590410(void* self, void*, void* gff, void* s);                    // LoadEncounterSpawnPoints RET 8
U __fastcall kotor_fastcall_00590580(void* self, void*, void* gff, void* s);                    // LoadEncounterGeometry RET 8
U __fastcall kotor_fastcall_00590820(void* self, void*, void* gff, void* s);                    // ReadEncounterScriptsFromGff RET 8
void __fastcall kotor_fastcall_00590a60(void* self, void*, I index);                            // SetDifficulty RET 4
void __fastcall kotor_fastcall_00590b10(void* self, void*, void* list, I* count);               // PadOutCreatureCount RET 8
void __fastcall kotor_fastcall_00590da0(void* self, void*, U id);                               // UpdateActivateAreaList RET 4
void __fastcall kotor_fastcall_00590f80(void* self, void*, void* list, I* count);               // BuildInitialSpawnList RET 8
void __fastcall kotor_fastcall_00591af0(void* self, void*);                                     // TallyEnemyRadiusPoints RET 0
void __fastcall kotor_fastcall_00591ca0(void* self, void*);                                     // SpawnCreatureFromSpawnList RET 0
void __fastcall kotor_fastcall_005921a0(void* self, void*);                                     // ClearSpawnList RET 0
void __fastcall kotor_fastcall_00592210(void* self, void*);                                     // RemoveFromArea RET 0
U __fastcall kotor_fastcall_00592430(void* self, void*, void* gff, void* s, U a, U b);          // ReadEncounterFromGff RET 10h
void __fastcall kotor_fastcall_005931d0(void* self, void*, float cr);                           // CalculateSpawnPool RET 4
void __fastcall kotor_fastcall_00593270(void* self, void*);                                     // LoadFractionalCRData RET 0
void __fastcall kotor_fastcall_00593380(void* self, void*);                                     // ~CSWSEncounter RET 0
void* __fastcall kotor_fastcall_00593510(void* self, void*);                                    // GetFirstName RET 0
void __fastcall kotor_fastcall_00593520(void* self, void*, void* area, float x, float y, float z, U flag); // AddToArea RET 14h
void __fastcall kotor_fastcall_005935d0(void* self, void*);                                     // BuildSpawnList RET 0
U __fastcall kotor_fastcall_00593830(void* self, void*, void* gff, void* s);                    // LoadEncounter RET 8
U __fastcall kotor_fastcall_00593a90(void* self, void*, void* resref, U a, U b);                // LoadFromTemplate RET 0Ch
void __fastcall kotor_fastcall_00593be0(void* self, void*, U id, float cr);                     // SpawnIfAppropriate RET 8
void __fastcall kotor_fastcall_00593c50(void* self, void*, U active);                           // SetActive RET 4
void* __fastcall kotor_fastcall_00593c70(void* self, void*, U id);                              // CSWSEncounter(id) RET 4
void* __fastcall kotor_fastcall_00593f90(void* self, void*, U flags);                           // deleting dtor RET 4
void __fastcall kotor_fastcall_00593fb0(void* self, void*);                                     // AIUpdate RET 0
void __fastcall kotor_fastcall_00594220(void* self, void*, U type, U caller, void* event, U a, U b); // EventHandler RET 14h
U __fastcall kotor_fastcall_00505060(void* area, void*, void* gff, void* root, U load_state, U from_template); // CSWSArea::LoadEncounters RET 10h
int __fastcall kotor_fastcall_00542760(void* commands, void*, int routine, unsigned argc);      // ExecuteCommandSetEncounterData RET 8
extern const void* const kotor_vtable_00749d98[56];                                             // CSWSEncounter vtable
}

// CEncounterListEntry (0x24): +00 CResRef, +10 float CR (field f10), +14 float points = CalculatePointsFromCR(CR) (field "cr"),
// +18 used flag, +1C ?, +20 SingleSpawn flag.
struct EncounterListEntry { unsigned char resref[16]; w5::U f10; float cr; w5::U f18; w5::U f1c; w5::U f20; };
static_assert(sizeof(EncounterListEntry) == 0x24, "CEncounterListEntry is 0x24 bytes");
// AddCreaturesToSpawnList takes the entry by value (RET 30h = 3 dwords + 0x24 bytes).
extern "C" void __fastcall kotor_fastcall_00592280(void* self, void*, EncounterListEntry* list, w5::I* count, w5::I index,
                                                   EncounterListEntry entry);                    // RET 30h

namespace enc {
using namespace w5;
constexpr U k_invalid_00746644 = 0x7f000000u;          // OBJECT_INVALID
constexpr U k_event_script_007441dc = 10;              // .rdata 007441DC
constexpr U8 k_encounter_type_0074663c = 0x0d;         // .rdata 0074663C
constexpr U k_minus_one_bits_740cbc = 0xbf800000u;     // .rdata 00740CBC float -1.0
inline unsigned char* bytes(void* p) { return static_cast<unsigned char*>(p); }
inline void* vm() { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::message_manager_007a3a00)); }
inline void* server() { return server_app(); }
inline void* ai() { return recovered_server_ai_master_004aed80(server(), nullptr); }
inline void* world_timer() { return kotor_fastcall_004aede0(server(), nullptr); }
inline void* two_da() { return at<void*>(kotor_cell_rules_007a3a28, 0xb8); }                    // [[007A3A28]+B8] CTwoDimArrays
struct ExoString { void* data; U length; };
struct ResRef { unsigned char raw[0x10]; };
// __ftol2 (006FAE8C) on a 53-bit value: truncation toward zero to 64 bits, low dword kept; NaN / out of range -> 0.
inline U ftol2_low(double v) {
    if (!(v > -9223372036854775808.0 && v < 9223372036854775808.0)) return 0;
    return static_cast<U>(static_cast<unsigned long long>(static_cast<long long>(v)));
}
// _CIpow 006FC1F0 (fld base; fld exponent; call) in its own noinline helper (inline-asm CALL clobber rule).
__declspec(noinline) inline double x87_pow(double base, double exponent) {
    double result;
    __asm {
        fld qword ptr base
        fld qword ptr exponent
        call genuine_x87_pow_006fc1f0
        fstp qword ptr result
    }
    return result;
}
}  // namespace enc
