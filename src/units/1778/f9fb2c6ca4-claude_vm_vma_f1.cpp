// Lane vmA, sub-batch F1 (party / game state / mines): NWScript engine-command handlers
// (CSWVirtualMachineCommands::ExecuteCommand*) reconstructed from the original swkotor.exe (read-only, capstone).
// Every handler: original int __thiscall (int routine, int argc), RET 8 -> extern "C" int __fastcall
// kotor_fastcall_<addr>(self, edx, routine, argc) (@16).  Conventions: claude_vm_vmA_common.hpp.
// .rdata facts used as literals: [00746636] 7 = trigger, [00746638] 9 = placeable, [00746639] 0Ah = door,
// [007441DC] 0Ah / [007441E0] 0Bh = AI event queue types, [0073D71C] "" , [007453C0] "LIVE%d",
// [00747028] "MULTIPLIER", [00751A48] "scriptloop0%d", [00751A38] "scriptloop%d".
#include "520143efd3-claude_vm_vma_common.hpp"

namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); }      // CExoString() [menu20_bindings]
namespace kotor_reconstruction { struct StartupString; void __cdecl StartupFormat005e5680(StartupString*, const char*, ...); } // CExoString::Format [0125]
namespace ini_comparison { int __fastcall CompareNC005e58c0(kotor_reconstruction::StartupString* s, void*, const char* text); } // CExoString::CompareNoCase [0154]
namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } }                     // [007A39E0] CExoBase* [0068]

extern "C" {
extern void* kotor_cell_rules_007a3a28;                                                                  // [007A3A28] CSWRules* [0421]
// ---- existing providers (c31 map; object in brackets) ----
void* __fastcall kotor_fastcall_004aede0(void* server, void*);                                          // CServerExoApp::GetWorldTimer [0457]
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, void* days, void* ms);                   // CWorldTimer::GetWorldTime [0457]
void* __fastcall server_get_module_004ae6b0(void* server, void*);                                       // CServerExoApp::GetModule [0530]
U __fastcall client_get_credit_sequence_in_progress_005ee1e0(void* client, void*);                      // CClientExoApp::GetCreditSequenceInProgress [0694]
U __fastcall kotor_fastcall_005edb40(void* client, void*);                                              // CClientExoApp::IsMoviePlaying [0402]
void* __fastcall frame_R4_005ed690(void* client, void*);                                                // CClientExoApp::GetInGameGui [0457]
void __fastcall kotor_fastcall_005edf10(void* client, void*, I effect);                                 // CClientExoApp::EnableVideoEffect [video_effect]
void* __fastcall kotor_fastcall_005ed700(void* client, void*);                                          // CClientExoApp::GetClientOptions [0021]
void* __fastcall kotor_fastcall_005ed530(void* client, void*);                                          // CClientExoApp::GetModule (client) [0457]
// c2da_current_get_float_00413430 (C2DA::GetFLOATEntry RET 0Ch [0376]) is declared in w5_t2.hpp
void* __fastcall frame_LogStringCtor_005e5a90(void* s, void*, const char* text);                        // CExoString(const char*) [0459]
void* __fastcall kotor_fastcall_005e6890(void* alias_list, void*, void* alias, I length);               // CExoAliasList::GetAliasPath RET 8 [0780]
I __fastcall async_string_length_005e5790(void* s, void*);                                              // CExoString::GetLength [0188]
void __fastcall c2da_ref_to_string_00405f70(void* ref, void*, void* out_string);                        // CResRef::CopyToString RET 4 [0204]
void __fastcall kotor_fastcall_00565500(void* party, void*, U solo, U unstealth);                       // CSWPartyTable::SetSoloMode RET 8 [claude_gui_hud2]
unsigned __fastcall recovered_spawn_npc_00565130(void* table, void*, unsigned index, unsigned use_given, void* pos, void* ori, unsigned flag); // CSWPartyTable::SpawnNPC RET 14h [w2_party_native]
I __fastcall recovered_party_add_npc_creature_00564300(void* table, void*, I npc, void* creature);      // CSWPartyTable::AddNPC RET 8 [w2_party_native]
U __fastcall kotor_fastcall_00563b40(void* table, void*, U planet);                                     // CSWPartyTable::GetGalaxyMapPlanetAvailable RET 4 [claude_gui_journal_galaxy]
void __cdecl object_vfx_lower_0044f130(float amount);                                                   // SetSpeedBlurAmount [object_vfx_render_lowers]
void __cdecl object_vfx_lower_0044f0a0();                                                               // EnableSpeedBlur [object_vfx_render_lowers]
void __cdecl object_vfx_lower_0044f0b0();                                                               // DisableSpeedBlur [object_vfx_render_lowers]
I __fastcall exo_array_list_count_004af240(void* list, void*, U value);                                 // CExoArrayList<ulong>::Count(value) RET 4 [0567]
void __fastcall w1x_unique_append_006449d0(void* list, void*, U value);                                 // CExoArrayList<ulong>::AddUnique RET 4 [0886]
void* __fastcall recovered_event_construct_004d7540(void* event, void*);                                // CScriptEvent::CScriptEvent [1017]
unsigned __fastcall recovered_event_enqueue_004b08d0(void* ai, void*, U day, U ms, U caller, U target, U type, void* event); // CServerAIMaster::AddEventDeltaTime RET 18h [1017]
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);                              // CServerExoApp::GetServerAIMaster [creature_destructor_r2]
// ---- reconstructed in this file (section at the end) ----
U __fastcall kotor_fastcall_004ae110(void* timer, void*);
void __fastcall kotor_fastcall_005edde0(void* client, void*, I* result, I* other);
void __fastcall kotor_fastcall_004aed10(void* server, void*, U value);
void __fastcall kotor_fastcall_004aece0(void* server, void*, U value);
void* __fastcall kotor_fastcall_004aefe0(void* server, void*, void* out);
void __fastcall kotor_fastcall_005636f0(void* table, void*, I npc, U id);
void* __fastcall kotor_fastcall_00605510(void* area, void*, void* name);
void __fastcall kotor_fastcall_00646180(void* room, void*, U animation);
}

using namespace vma;

namespace {
constexpr U k_type_trigger_00746636 = 7;
constexpr U k_type_placeable_00746638 = 9;
constexpr U k_type_door_00746639 = 0xa;
constexpr U k_event_queue_007441dc = 0xa;
constexpr U k_event_queue_007441e0 = 0xb;
const char k_empty_0073d71c[] = "";
const char k_live_format_007453c0[] = "LIVE%d";
const char k_multiplier_00747028[] = "MULTIPLIER";
const char k_scriptloop0_00751a48[] = "scriptloop0%d";
const char k_scriptloop_00751a38[] = "scriptloop%d";
// inline view of 004AEE70 CServerExoApp::GetPartyTable: [this+4] + 1B770h
inline void* party_table() { return static_cast<unsigned char*>(at<void*>(server_app(), 4)) + 0x1b770; }
inline kotor_menu20::String* ms(ExoString* s) { return reinterpret_cast<kotor_menu20::String*>(s); }
inline kotor_reconstruction::StartupString* ss(void* s) { return static_cast<kotor_reconstruction::StartupString*>(s); }
inline void* field(void* base, U offset) {                                       // add eax, off (no null fold)
    return ptr(word(base) + offset);
}
}  // namespace

extern "C" {

// ================================================================== 0053E0E0 GetTimeSecond() [routine 18]
int __fastcall kotor_fastcall_0053e0e0(void*, void*, int, unsigned) {
    const U second = kotor_fastcall_004ae110(kotor_fastcall_004aede0(server_app(), nullptr), nullptr); // 0053E0E0..EF
    return push_int(static_cast<I>(second));                                           // 0053E0F4..0E
}

// ================================================================== 0053B1E0 GetLastPazaakResult() [routine 365]
int __fastcall kotor_fastcall_0053b1e0(void*, void*, int, unsigned) {
    I result = 0, other = 0;                                                           // 0053B1E9..EF
    kotor_fastcall_005edde0(client_app(), nullptr, &result, &other);                   // 0053B1F3..00
    return push_int(result);                                                           // 0053B205..26
}

// ================================================================== 00547CE0 SetSoloMode(int) [routine 753]
// Quirk (original): a failed pop returns -2000 (0xFFFFF830), not -2001.
int __fastcall kotor_fastcall_00547ce0(void*, void*, int, unsigned) {
    I mode = 0;
    if (!pop_int(&mode)) return k_push_failed;                                         // 00547CE1..FB
    kotor_fastcall_00565500(party_table(), nullptr, static_cast<U>(mode), 1);          // 00547CFE..15
    return 0;                                                                          // 00547D1A
}

// ================================================================== 00530660 DoSinglePlayerAutoSave() [routine 512]
int __fastcall kotor_fastcall_00530660(void*, void*, int, unsigned) {
    kotor_fastcall_004aed10(server_app(), nullptr, 1);                                 // 00530660..6A
    return 0;                                                                          // 0053066F
}

// ================================================================== 00535C70 ExportAllCharacters() [routine 557]
int __fastcall kotor_fastcall_00535c70(void*, void*, int, unsigned) {
    kotor_fastcall_004aece0(server_app(), nullptr, 1);                                 // 00535C70..7A
    return 0;                                                                          // 00535C7F
}

// ================================================================== 005473B0 IsCreditSequenceInProgress() [routine 519]
int __fastcall kotor_fastcall_005473b0(void*, void*, int, unsigned) {
    return push_int(static_cast<I>(client_get_credit_sequence_in_progress_005ee1e0(client_app(), nullptr))); // 005473B0..D7
}

// ================================================================== 005481E0 IsMoviePlaying() [routine 768]
int __fastcall kotor_fastcall_005481e0(void*, void*, int, unsigned) {
    return push_int(static_cast<I>(kotor_fastcall_005edb40(client_app(), nullptr)));   // 005481E0..07
}

// ================================================================== 0053C980 GetPCLevellingUp() [routine 542]
int __fastcall kotor_fastcall_0053c980(void*, void*, int, unsigned) {
    U result = k_invalid;                                                              // 0053C989
    void* module = server_get_module_004ae6b0(server_app(), nullptr);                  // 0053C980..8E
    if (module) result = at<U>(module, 0x194);                                         // 0053C993..97
    return push_object(result);                                                        // 0053C99D..B8
}

// ================================================================== 00539900 GetIsDay() [routine 405]
int __fastcall kotor_fastcall_00539900(void*, void*, int, unsigned) {
    I result = 0;                                                                      // 00539909
    void* module = server_get_module_004ae6b0(server_app(), nullptr);                  // 00539900..0B
    if (module && at<U8>(module, 0x1bc) == 1) result = 1;                              // 00539910..1D
    return push_int(result);                                                           // 00539922..3D
}

// ================================================================== 00547E90 GetCheatCode(int) [routine 764]
// Quirk (original): a failed pop returns -2000 (0xFFFFF830), not -2001.
int __fastcall kotor_fastcall_00547e90(void*, void*, int, unsigned) {
    I result = 0;                                                                      // 00547E9D
    I code = 0;
    if (!pop_int(&code)) return k_push_failed;                                         // 00547E98..AF
    if (code == 0) result = at<I>(frame_R4_005ed690(client_app(), nullptr), 0xc20);    // 00547EB2..C8
    return push_int(result);                                                           // 00547ECE..EA
}

// ================================================================== 005471E0 EnableVideoEffect(int) [routine 508]
int __fastcall kotor_fastcall_005471e0(void*, void*, int, unsigned) {
    I effect = 0;
    if (!pop_int(&effect)) return k_pop_failed;                                        // 005471E1..FB
    kotor_fastcall_005edf10(client_app(), nullptr, effect);                            // 005471FE..0C
    return 0;                                                                          // 00547211
}

// ================================================================== 00543550 SWMG_SetSpeedBlurEffect(int bEnabled, float fRatio=0.75) [routine 563]
// The ratio is popped into the argc slot (default 0.75f, 3F400000h) and only applied when argc >= 2.
int __fastcall kotor_fastcall_00543550(void*, void*, int, unsigned argc) {
    I enabled = 0;
    if (!pop_int(&enabled)) return k_pop_failed;                                       // 00543551..63
    if (static_cast<I>(argc) >= 2) {                                                   // 00543565..6A (jl)
        float ratio = flt(0x3f400000u);                                                // 00543577
        if (!pop_float(&ratio)) return k_pop_failed;                                   // 0054356C..8E
        object_vfx_lower_0044f130(ratio);                                              // 00543591..9B
    }
    if (enabled != 0) object_vfx_lower_0044f0a0();                                     // 0054359E..A6
    else object_vfx_lower_0044f0b0();                                                  // 005435B1
    return 0;                                                                          // 005435AB / 005435B6
}

// ================================================================== 00543ED0 SpawnAvailableNPC(int nNPC, location lPosition) [routine 698]
// Quirk (original): a failed object push returns -2000 and leaks the popped location.
int __fastcall kotor_fastcall_00543ed0(void*, void*, int, unsigned) {
    I npc = 0;
    void* location = nullptr;
    if (!pop_int(&npc)) return k_pop_failed;                                           // 00543ED0..E5
    if (!pop_struct(2, &location)) return k_pop_failed;                                // 00543EE7..FB
    const U id = recovered_spawn_npc_00565130(party_table(), nullptr, static_cast<U>(npc), 1, location,
                                              static_cast<unsigned char*>(location) + 0xc, 1); // 00543F08..2A
    if (!recovered_vm_outer_push_object_005d10d0(vm(), nullptr, id)) return k_push_failed; // 00543F2F..47
    if (location) live_free(location);                                                 // 00543F4A..60 (~CScriptLocation 004015A0 is a bare ret)
    return 0;                                                                          // 00543F63
}

// ================================================================== 00548180 SetAvailableNPCId(int nNPC, object oidNPC) [routine 767]
int __fastcall kotor_fastcall_00548180(void*, void*, int, unsigned) {
    I npc = 0;
    U id = 0;
    if (!pop_int(&npc)) return k_pop_failed;                                           // 00548180..95
    if (!pop_object(&id)) return k_pop_failed;                                         // 00548197..A9
    kotor_fastcall_005636f0(party_table(), nullptr, npc, id);                          // 005481B6..D0
    return 0;                                                                          // 005481D5
}

// ================================================================== 0052DC40 AddAvailableNPCByObject(int nNPC, object oCreature) [routine 694]
int __fastcall kotor_fastcall_0052dc40(void*, void*, int, unsigned) {
    void* server = server_app();                                                       // 0052DC40..49 (read before the pops)
    I npc = 0;
    U id = 0;
    if (!pop_int(&npc)) return k_pop_failed;                                           // 0052DC4C..69
    if (!pop_object(&id)) return k_pop_failed;                                         // 0052DC6C..7E
    void* creature = kotor_fastcall_004ae770(server, nullptr, id);                     // 0052DC80..87
    void* table = static_cast<unsigned char*>(at<void*>(server, 4)) + 0x1b770;         // 0052DC92..94 (inline view of 004AEE70)
    const I result = recovered_party_add_npc_creature_00564300(table, nullptr, npc, creature); // 0052DC99..9B
    return push_int(result);                                                           // 0052DCA0..BE
}

// ================================================================== 00546850 GetGalaxyMapPlanetAvailable(int) [routine 743]
int __fastcall kotor_fastcall_00546850(void*, void*, int, unsigned) {
    I planet = 0;
    if (!pop_int(&planet)) return k_pop_failed;                                        // 00546851..6B
    const U result = kotor_fastcall_00563b40(party_table(), nullptr, static_cast<U>(planet)); // 0054686E..83
    return push_int(static_cast<I>(result));                                           // 00546888..A3
}

// ================================================================== 0053F010 GetLastConversation() [routine 711]
// Quirk (original game bug, reproduced): the push status is folded with neg/sbb/and F830h only, so a SUCCESSFUL
// StackPushString returns -2000 (0xFFFFF830) and a failed one returns 0.
int __fastcall kotor_fastcall_0053f010(void*, void*, int, unsigned) {
    ExoString text;
    kotor_fastcall_004aefe0(server_app(), nullptr, &text);                             // 0053F028..36
    const U pushed = recovered_vm_outer_push_string_005d1090(vm(), nullptr, &text);    // 0053F03B..4A
    exo_destroy(&text);                                                                // 0053F04F..5D
    return pushed ? k_push_failed : 0;                                                 // 0053F066..6C
}

// ================================================================== 005474F0 GetDifficultyModifier() [routine 523]
int __fastcall kotor_fastcall_005474f0(void*, void*, int, unsigned) {
    float value = 1.0f;                                                                // 0054750D 3F800000h
    const I difficulty = at<U8>(kotor_fastcall_005ed700(client_app(), nullptr), 4);   // 00547508..1E
    ExoString column;
    frame_LogStringCtor_005e5a90(&column, nullptr, k_multiplier_00747028);             // 00547522..2B
    void* table = at<void*>(at<void*>(kotor_cell_rules_007a3a28, 0xb8), 0x7c);         // 00547530..3C
    (void)c2da_current_get_float_00413430(table, nullptr, static_cast<U>(difficulty), &column, &value); // 0054753F..54
    exo_destroy(&column);                                                              // 00547559..65
    return push_float(value);                                                          // 0054756A..97
}

// ================================================================== 00547AB0 GetIsLiveContentAvailable(int nPkg) [routine 748]
int __fastcall kotor_fastcall_00547ab0(void*, void*, int, unsigned) {
    ExoString path;
    kotor_menu20::ConstructString005b3190(ms(&path), nullptr);                         // 00547AC9..CF
    I result = 0;                                                                      // 00547ACD
    I package = 0;
    if (!pop_int(&package)) { exo_destroy(&path); return k_pop_failed; }               // 00547AD4..11
    if (package == 1) {                                                                // 00547B14..1F
        result = package;
    } else if (package >= 2 && package <= 6) {                                         // 00547B21..29 (signed)
        kotor_reconstruction::StartupFormat005e5680(ss(&path), k_live_format_007453c0, package); // 00547B2B..36
        void* aliases = at<void*>(kotor_menu_10_19::dependencies::base_007a39e0, 0xc); // 00547B3B..40
        void* resolved = kotor_fastcall_005e6890(aliases, nullptr, &path, 0);          // 00547B46..4C
        result = async_string_length_005e5790(resolved, nullptr) != 0 ? 1 : 0;        // 00547B51..5E
    }
    if (!recovered_vm_outer_push_integer_005d1010(vm(), nullptr, result)) {            // 00547B60..7A
        exo_destroy(&path); return k_push_failed;                                      // 00547B7C..95
    }
    exo_destroy(&path);                                                                // 00547B98
    return 0;                                                                          // 00547BA1
}

// ================================================================== 00540DD0 PlayRoomAnimation(string sRoom, int nAnimation) [routine 738]
int __fastcall kotor_fastcall_00540dd0(void*, void*, int, unsigned) {
    ExoString room_name;
    kotor_menu20::ConstructString005b3190(ms(&room_name), nullptr);                    // 00540DE8..EC
    I animation = 0;
    if (!pop_string(&room_name) || !pop_int(&animation)) {                             // 00540DF1..1F
        exo_destroy(&room_name); return k_pop_failed;                                  // 00540E21..45
    }
    void* module = kotor_fastcall_005ed530(client_app(), nullptr);                     // 00540E48..52
    if (module) {
        void* area = at<void*>(module, 0x48);                                          // 00540E5B
        if (area) {
            ExoString name;                                                            // [esp+10h] temporary copy
            frame_LogStringCtor_005e5a90(&name, nullptr, WindowCStr005e5670(&room_name, nullptr)); // 00540E62..70
            void* room = kotor_fastcall_00605510(area, nullptr, &name);                // 00540E75..81
            exo_destroy(&name);                                                        // 00540E86..91
            if (room) kotor_fastcall_00646180(room, nullptr, static_cast<U>(animation)); // 00540E96..A1
        }
    }
    exo_destroy(&room_name);                                                           // 00540EA6..B2
    return 0;                                                                          // 00540EBB
}

// ================================================================== 0054C0E0 GetMineInfo family [routines 527..536, 550, 555]
// One handler for the trap ("mine") routines; branches on the routine id exactly like the original compare chain.
// Object types: trigger (7, vtbl+40h), placeable (9, vtbl+48h), door (0Ah, vtbl+18h).
// Quirks (original): the cast result is not null-checked; the 534 string field is pushed by address.
int __fastcall kotor_fastcall_0054c0e0(void* self, void*, int routine, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 0054C0F9..23
    void* o = game_object(id);                                                         // 0054C126..3A
    auto type = [&]() { return static_cast<U>(at<U8>(o, 8)); };
    switch (routine) {
    case 0x20f: {                                                                      // 527 GetTrapDisarmable? (+2C4/+280/+2FC)
        I r = 0;
        if (o) {
            if (type() == k_type_trigger_00746636) r = at<I>(as_trigger(o), 0x2c4);
            else if (type() == k_type_placeable_00746638) r = at<I>(as_placeable(o), 0x280);
            else if (type() == k_type_door_00746639) r = at<I>(as_door(o), 0x2fc);
        }
        return push_int(r);
    }
    case 0x210: {                                                                      // 528 (+2C0/+284/+300)
        I r = 0;
        if (o) {
            if (type() == k_type_trigger_00746636) r = at<I>(as_trigger(o), 0x2c0);
            else if (type() == k_type_placeable_00746638) r = at<I>(as_placeable(o), 0x284);
            else if (type() == k_type_door_00746639) r = at<I>(as_door(o), 0x300);
        }
        return push_int(r);                                                              // 0054C212..3B
    }
    case 0x211: {                                                                      // 529 detected-by list count
        I r = 0;                                                                       // 0054C254 xor edi,edi
        U other = 0;
        if (!pop_object(&other)) return k_pop_failed;                                  // 0054C249..74
        if (o) {
            if (type() == k_type_trigger_00746636)
                r = exo_array_list_count_004af240(field(as_trigger(o), 0x2a8), nullptr, other);   // 0054C286..9D
            else if (type() == k_type_placeable_00746638)
                r = exo_array_list_count_004af240(field(as_placeable(o), 0x318), nullptr, other); // 0054C2AD..C4
            else if (type() == k_type_door_00746639)
                r = exo_array_list_count_004af240(field(as_door(o), 0x2dc), nullptr, other);      // 0054C2D4..EB
        }
        return push_int(r);                                                              // 0054C29F / 0054C2ED -> 0054C194
    }
    case 0x212: {                                                                      // 530 (+2C8 dword / +28C byte / +2F4 byte)
        I r = 0;
        if (o) {
            if (type() == k_type_trigger_00746636) r = at<I>(as_trigger(o), 0x2c8);
            else if (type() == k_type_placeable_00746638) r = at<U8>(as_placeable(o), 0x28c);
            else if (type() == k_type_door_00746639) r = at<U8>(as_door(o), 0x2f4);
        }
        return push_int(r);
    }
    case 0x213: {                                                                      // 531 (default -1; bytes +2D4/+290/+2F8)
        I r = -1;                                                                      // 0054C362
        if (o) {
            if (type() == k_type_trigger_00746636) r = at<U8>(as_trigger(o), 0x2d4);
            else if (type() == k_type_placeable_00746638) r = at<U8>(as_placeable(o), 0x290);
            else if (type() == k_type_door_00746639) r = at<U8>(as_door(o), 0x2f8);
        }
        return push_int(r);
    }
    case 0x214: {                                                                      // 532 (+2D0/+288/+304)
        I r = 0;
        if (o) {
            if (type() == k_type_trigger_00746636) r = at<I>(as_trigger(o), 0x2d0);
            else if (type() == k_type_placeable_00746638) r = at<I>(as_placeable(o), 0x288);
            else if (type() == k_type_door_00746639) r = at<I>(as_door(o), 0x304);
        }
        return push_int(r);
    }
    case 0x215: {                                                                      // 533 trigger +2E4 object (creator)
        U r = k_invalid;                                                               // 0054C434
        if (o && type() == k_type_trigger_00746636) r = at<U>(as_trigger(o), 0x2e4);   // 0054C432..4D
        return push_object(r);                                                         // 0054C453..5F -> 0054C19F
    }
    case 0x216: {                                                                      // 534 CExoString field (+27C/+264/+2D0)
        void* s = nullptr;
        if (o) {
            if (type() == k_type_trigger_00746636) s = field(as_trigger(o), 0x27c);    // 0054C47E..85
            else if (type() == k_type_placeable_00746638) s = field(as_placeable(o), 0x264); // 0054C494..9B
            else if (type() == k_type_door_00746639) s = field(as_door(o), 0x2d0);     // 0054C4AA..B1
        }
        if (s != nullptr)                                                              // 0054C4B6..B8
            return push_status(recovered_vm_outer_push_string_005d1090(vm(), nullptr, s)); // 0054C508..14
        ExoString empty;
        frame_LogStringCtor_005e5a90(&empty, nullptr, k_empty_0073d71c);               // 0054C4BA..C4
        const U pushed = recovered_vm_outer_push_string_005d1090(vm(), nullptr, &empty); // 0054C4C9..DC
        exo_destroy(&empty);                                                           // 0054C4E7..F5
        return pushed ? 0 : k_push_failed;                                             // 0054C4E1..503
    }
    case 0x217: {                                                                      // 535 (+2DC dword / +27C byte / +2EC byte)
        I r = 0;
        if (o) {
            if (type() == k_type_trigger_00746636) r = at<I>(as_trigger(o), 0x2dc);
            else if (type() == k_type_placeable_00746638) r = at<U8>(as_placeable(o), 0x27c);
            else if (type() == k_type_door_00746639) r = at<U8>(as_door(o), 0x2ec);
        }
        return push_int(r);
    }
    case 0x218: {                                                                      // 536 (+2D8 dword / +276 byte / +2ED byte)
        I r = 0;
        if (o) {
            if (type() == k_type_trigger_00746636) r = at<I>(as_trigger(o), 0x2d8);
            else if (type() == k_type_placeable_00746638) r = at<U8>(as_placeable(o), 0x276);
            else if (type() == k_type_door_00746639) r = at<U8>(as_door(o), 0x2ed);
        }
        return push_int(r);
    }
    case 0x226: {                                                                      // 550 add creature to detected-by list
        I r = 0;                                                                       // 0054C5FF xor edi,edi
        U other = 0;                                                                   // popped into the routine slot
        if (!pop_object(&other)) return k_pop_failed;                                  // 0054C5F4..08
        if (kotor_fastcall_004ae770(server_app(), nullptr, other) != nullptr && o != nullptr) { // 0054C60E..27
            if (type() == k_type_trigger_00746636) {
                w1x_unique_append_006449d0(field(as_trigger(o), 0x2a8), nullptr, other); r = 1;   // 0054C634..4B
            } else if (type() == k_type_placeable_00746638) {
                w1x_unique_append_006449d0(field(as_placeable(o), 0x318), nullptr, other); r = 1; // 0054C65E..75
            } else if (type() == k_type_door_00746639) {
                w1x_unique_append_006449d0(field(as_door(o), 0x2dc), nullptr, other); r = 1;      // 0054C688..9F
            }
        }
        return push_int(r);
    }
    case 0x22b: {                                                                      // 555 trigger the mine (events, no push)
        if (!o) return 0;                                                              // 0054C6B5..B7
        U kind;
        if (type() == k_type_trigger_00746636) {
            if (at<I>(as_trigger(o), 0x2bc) != 1) return 0;                            // 0054C6CC..DC
            kind = 1;
        } else if (type() == k_type_placeable_00746638) {
            if (at<I>(as_placeable(o), 0x278) != 1) return 0;                          // 0054C764..72
            kind = 2;
        } else if (type() == k_type_door_00746639) {
            if (at<I>(as_door(o), 0x2e8) != 1) return 0;                               // 0054C7A3..B1
            kind = 3;
        } else {
            return 0;
        }
        void* memory = live_operator_new(0x34);                                        // 0054C6E2..E9 / 0054C778.. / 0054C7B3..
        void* event = memory ? recovered_event_construct_004d7540(memory, nullptr) : nullptr; // (null not checked below)
        at<U16>(event, 0) = 0x18;                                                      // 0054C708 / 0054C7D8
        recovered_event_enqueue_004b08d0(recovered_server_ai_master_004aed80(server_app(), nullptr), nullptr,
                                         0, 0, self_id(self), at<U>(o, 4), k_event_queue_007441dc, event); // 0054C70D..37 / 0054C7DD..09
        if (kind == 1)                                                                 // trigger: second, timed event
            recovered_event_enqueue_004b08d0(recovered_server_ai_master_004aed80(server_app(), nullptr), nullptr,
                                             0, 1, self_id(self), at<U>(o, 4), k_event_queue_007441e0, nullptr); // 0054C73C..57 -> 0054C800..09
        return 0;                                                                      // 0054C80E..20
    }
    default:
        return 0;                                                                      // 0054C6AF -> 0054C80E
    }
}

// =====================================================================================================
// Reconstructed callees without a callable C-linkage provider in the c31 map (exact original ABI).
// =====================================================================================================

// 004AE110 CWorldTimer::GetTimeSecond(): thiscall, RET; (ms / 1000) % 60 of GetWorldTime
U __fastcall kotor_fastcall_004ae110(void* timer, void*) {
    U ms = 0, days = 0;
    frame_GetWorldTime_004ade40(timer, nullptr, &days, &ms);                           // 004AE113..1D
    return (ms / 1000u) % 60u;                                                         // 004AE122..39 (mul 10624DD3h, shr 6; div 3Ch)
}

// 005EDDE0 CClientExoApp::GetLastPazaakResult(int* result, int* other): thiscall, RET 8
void __fastcall kotor_fastcall_005edde0(void* client, void*, I* result, I* other) {
    void* internal = at<void*>(client, 4);                                             // 005EDDE0
    *result = at<I>(internal, 0x80);                                                   // 005EDDE3..ED
    *other = at<I>(internal, 0x7c);                                                    // 005EDDEF..F6
}

// 004AED10 CServerExoApp::SetDoSinglePlayerAutoSave(int): thiscall, RET 4; [[this+4]+100B8h] = value
void __fastcall kotor_fastcall_004aed10(void* server, void*, U value) {
    at<U>(at<void*>(server, 4), 0x100b8) = value;                                      // 004AED10..1D
}

// 004AECE0 CServerExoApp::SetExportAllCharacters(int): thiscall, RET 4; [[this+4]+100BCh] = value
void __fastcall kotor_fastcall_004aece0(void* server, void*, U value) {
    at<U>(at<void*>(server, 4), 0x100bc) = value;                                      // 004AECE0..ED
}

// 004AEFE0 CServerExoApp::GetLastConversation(CExoString* out): thiscall, RET 4; copy-constructs out from
// [this+4]+1B908h and returns out
void* __fastcall kotor_fastcall_004aefe0(void* server, void*, void* out) {
    WindowCopy005e5b00(out, nullptr, static_cast<unsigned char*>(at<void*>(server, 4)) + 0x1b908); // 004AEFE1..F9
    return out;                                                                        // 004AEFFE
}

// 005636F0 CSWPartyTable::SetAvailableNPCId(int npc, ulong id): thiscall, RET 8.
// Only for 0 <= npc < 9 whose availability slot (+30h + npc*4) is set: +0Ch + npc*4 = id.
void __fastcall kotor_fastcall_005636f0(void* table, void*, I npc, U id) {
    if (npc < 0 || npc >= 9) return;                                                   // 005636F0..FB
    if (at<U>(table, 0x30 + static_cast<U>(npc) * 4) == 0) return;                     // 005636FD..03
    at<U>(table, 0xc + static_cast<U>(npc) * 4) = id;                                  // 00563705..09
}

// 00605510 CSWCArea::GetRoom(CExoString const& name): thiscall, RET 4.  Rooms: count +25Ch, array +260h of 48h-byte
// entries with the CResRef at +20h; returns the first room whose resref equals the name (no case), else 0.
void* __fastcall kotor_fastcall_00605510(void* area, void*, void* name) {
    const I count = at<I>(area, 0x25c);                                                // 00605532
    if (count <= 0) return nullptr;                                                    // 0060553D..3F
    for (I i = 0; i < at<I>(area, 0x25c); ++i) {                                       // 006055BC..C8 (count re-read)
        unsigned char ref[16];
        std::memcpy(ref, static_cast<unsigned char*>(at<void*>(area, 0x260)) + static_cast<U>(i) * 0x48 + 0x20, 16); // 00605550..76
        ExoString text;
        kotor_menu20::ConstructString005b3190(ms(&text), nullptr);                     // 00605572..7A
        c2da_ref_to_string_00405f70(ref, nullptr, &text);                              // 0060557F..90
        const int equal = ini_comparison::CompareNC005e58c0(ss(name), nullptr, WindowCStr005e5670(&text, nullptr)); // 00605595..A2
        if (equal != 0) {                                                              // 006055A7..B1
            void* room = static_cast<unsigned char*>(at<void*>(area, 0x260)) + static_cast<U>(i) * 0x48; // 006055E0..E9
            exo_destroy(&text);                                                        // 006055EC..F0
            return room;
        }
        exo_destroy(&text);                                                            // 006055B3..B7
    }
    return nullptr;                                                                    // 006055CA
}

// 00646180 CSWCRoom::PlayAnimation(unsigned short animation): thiscall, RET 4.  When the room has a model (+40h):
// name = "scriptloop0%d" (< 10) or "scriptloop%d", then model->vtbl[18h](name, 1.0f, 8, 0).
void __fastcall kotor_fastcall_00646180(void* room, void*, U animation) {
    void* model = at<void*>(room, 0x40);                                               // 0064619B
    if (!model) return;                                                                // 0064619E..A0
    ExoString name;
    kotor_menu20::ConstructString005b3190(ms(&name), nullptr);                         // 006461A2..A6
    const U16 anim = static_cast<U16>(animation);                                      // 006461AB word
    if (anim < 0xa)                                                                    // 006461B0..BC (jae: unsigned)
        kotor_reconstruction::StartupFormat005e5680(ss(&name), k_scriptloop0_00751a48, static_cast<U>(anim)); // 006461BE..DC
    else
        kotor_reconstruction::StartupFormat005e5680(ss(&name), k_scriptloop_00751a38, static_cast<U>(anim));  // 006461CE..DC
    const char* text = WindowCStr005e5670(&name, nullptr);                             // 006461F3..F7
    vcall<void>(at<void*>(room, 0x40), 0x18, text, 1.0f, 8u, 0u);                      // 006461FC..00200 (vtbl read from [+40h] at 006461E8)
    exo_destroy(&name);                                                                // 00646203..0F
}

}  // extern "C"
