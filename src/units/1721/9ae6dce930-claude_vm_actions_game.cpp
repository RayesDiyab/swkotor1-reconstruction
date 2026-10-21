// Lane endar-actions, group "game": NWScript engine-command handlers (CSWVirtualMachineCommands::ExecuteCommand*)
// for GUI / party / sound / music / dialog-camera commands used by Endar Spire scripts, plus the callees that have no
// implementation in the c17 candidate. Transcribed from the original swkotor.exe (read-only, capstone); the comments
// give the original instruction ranges. Conventions: claude_vm_actions_common.hpp (handler = thiscall(int routine,
// int argc) RET 8 -> extern "C" __fastcall @16; 0 / -2001 pop failure / -2000 push failure).
// x87: 53-bit precision control is assumed; every x87 intermediate is a double (exact for the operations used here),
// every `fstp dword` is an explicit (float).
#include "061872cb05-claude_vm_actions_common.hpp"
#include <cmath>
#include <new>

namespace frame_control { extern std::uint32_t sound_manager_007a39ec; }   // g_pExoSound 007A39EC [0673]
namespace w2 { unsigned __cdecl ftol2_006fae8c(double value); }             // __ftol2 low dword [w2_ftol_native]

extern "C" {
extern void* kotor_cell_tlk_007a3a08;    // g_pTlkTable 007A3A08
extern void* kotor_cell_rules_007a3a28;  // g_pRules 007A3A28

// ---- existing C-linkage providers (c17 map; object in brackets) ----
U __fastcall kotor_fastcall_005edc20(void* client, void*, U a, U b, U c);                    // CClientExoApp::SetPausedByCombat [0714]
U __fastcall area_vfx_queue_announce_005fb490(void* internal, void*, I pattern);             // CClientExoAppInternal::PlayRumblePattern [area_vfx_announce]
void* __fastcall kotor_fastcall_004ae780(void* server, void*, U id);                         // CServerExoApp::GetAreaByGameObjectID [0530]
U __fastcall kotor_fastcall_004aea40(void* server, void*);                                   // CServerExoApp::GetPlayerCreatureId [0595]
U __fastcall recovered_npc_object_00564700(void* party, void*, I npc, U a, U b);             // CSWPartyTable::GetNPCObject [w2_party_native]
void __fastcall kotor_fastcall_0060ef50(void* creature, void*, U in_party);                  // CSWCCreature::SetInParty [1205-32]
void __fastcall recovered_set_in_party_004fdb20(void* creature, void*, U in_party, U flag);  // CSWSCreature::SetInParty [w2_ifo_native]
void __fastcall kotor_fastcall_004d0940(void* object, void*, U flag);                        // CSWSObject::ClearAllEffects [claude_w5_creature_actions]
U __fastcall show_tutorial_window_005edf40(void* client, void*, U window, U a, U b, U c);    // CClientExoApp::ShowTutorialWindow [0813]
void __fastcall kotor_fastcall_0062ec60(void* gui, void*, U state);                          // CGuiInGame::SetGlobalDialogState [dialogue_state]
void __fastcall recovered_party_distribute_experience_005653a0(void* party, void*, I xp, U flag); // CSWPartyTable::DistributeExperience [w2]
void __fastcall kotor_fastcall_0062eeb0(void* gui, void*, U kind, U value);                  // CGuiInGame::UpdateStatus [src_hud]
U __fastcall kotor_fastcall_00414110(void* table, void*, void* row, void* column, I* out);   // C2DA::GetINTEntry(CExoString,CExoString,int*) [0594]
void* __fastcall frame_LogStringCtor_005e5a90(void* s, void*, const char* text);             // CExoString(const char*) [0459]
void* __fastcall feedback_str_res_construct_00414dc0(void* self, void*);                     // STR_RES::STR_RES [feedback_lowers]
U __fastcall tlk_fetch_0041e550(void* tlk, void*, U strref, void* out, U flag);              // CTlkTable::Fetch [0385]
void __fastcall object_projectile_bark_0062ab30(void* gui, void*, U id, void* text, void* sound, U flag); // CGuiInGame::ShowBarkBubble [base13]
void __fastcall kotor_fastcall_005edbb0(void* client, void*, U id, void* text, void* sound, U flag);    // CServerExoApp(client)::BarkString [claude_w5_object_actions]
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);                 // CServerExoApp::GetSWSMessage [creature_destructor_r2]
void* __fastcall recovered_server_player_list_004aed90(void* server, void*);                 // CServerExoApp::GetPlayerList [w1_native_b7]
void* __fastcall kotor_fastcall_00561080(void* player, void*);                               // CSWSPlayer::GetGameObject [0595]
void* __fastcall c2da_005e9d70(void* list, void*, void* data);                               // CExoLinkedListInternal::AddHead [0190]
void __fastcall c2da_005e9c10(void* list, void*, void* position);                            // CExoLinkedListInternal::Remove [0191]
void __fastcall kotor_fastcall_004d68a0(void* message, void*, U size, U a, U b);             // CSWMessage::CreateWriteMessage [w2_message_native]
void __fastcall kotor_fastcall_00578570(void* message, void*, U id);                         // CSWSMessage::WriteOBJECTIDServer [w2_message_native]
U __fastcall kotor_fastcall_004d6440(void* message, void*, void** data, U* size);            // CSWMessage::GetWriteMessage [w2_message_native]
U __fastcall kotor_fastcall_00567e60(void* message, void*, U player, U major, U minor, void* data, U size); // SendServerToPlayerMessage [w2_message_native]
void* __fastcall kotor_fastcall_005ed580(void* client, void*, U id);                         // CClientExoApp::GetGameObject [0701]
void __fastcall kotor_fastcall_006146e0(void* creature, void*, U id, U flag);                // CSWCCreature::LookAt [1205-34]
void* __fastcall frame_GetWorldTimer_005ed4f0(void* client, void*);                          // CClientExoApp::GetWorldTimer [0457]
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, void* days, void* ms);        // CWorldTimer::GetWorldTime [0457]
U __fastcall frame_ContainsPanel_0040bd70(void* manager, void*, void* panel);                 // CSWGuiManager::PanelExists [0458]
void* __fastcall kotor_fastcall_005ed6f0(void* client, void*);                               // CClientExoApp::GetSWCMessage [1205-30]
void __fastcall frame_TogglePauseRequest_00677800(void* message, void*);                     // SendPlayerToServerInput_TogglePauseRequest [0457]
void __fastcall kotor_fastcall_0040a140(void* manager, void*, U sound);                      // CSWGuiManager::PlayGuiSound [1205-30]
void __fastcall frame_R4_005d5e80(void* sound, void*, U mode);                               // CExoSoundInternal(CExoSound)::SetSoundMode [0709]
void* __fastcall kotor_fastcall_006bfa40(void* self, void*, void* manager);                  // CSWGuiPartySelection::CSWGuiPartySelection [panels_menus]
void __fastcall kotor_fastcall_0040bc70(void* manager, void*, void* panel, U a, U b);        // CSWGuiManager::AddPanel [0298]
U __fastcall kotor_fastcall_0062bf30(void* gui, void*);                                      // CGuiInGame::InitializeShotCamera [claude_dialogue_camera]
void __fastcall kotor_fastcall_0062c030(void* gui, void*, U camera_id, U flag160, U effect); // CGuiInGame::SetComputerCameraShot [claude_dialogue_camera]
void __fastcall kotor_fastcall_005edf10(void* client, void*, I effect);                      // CClientExoApp::EnableVideoEffect [video_effect]
}

namespace game_vm {
struct V3 { U x, y, z; };      // Vector by value (three dwords)
struct Q4 { U w, x, y, z; };   // Quaternion by value (four dwords, memory order)
}
extern "C" {
double __cdecl kotor_cdecl_004acb90(game_vm::Q4 q);                                          // Roll [1205-01]
double __cdecl quaternion_pitch_004aa020(const void* q);                                     // Pitch [0625]
double __cdecl quaternion_yaw_004a9f40(const void* q);                                       // Yaw [0625]
void __fastcall kotor_fastcall_006bb590(void* self, void*, U type, game_vm::V3 position, game_vm::Q4 orientation, U fov); // CSWBehaviorCameraDialog::SetShot [claude_dialogue_camera]
}

// ---- callees reconstructed in this file (no implementation in the candidate) ----
extern "C" {
void* __fastcall kotor_fastcall_004ae800(void* server, void*, U id);
void* __fastcall kotor_fastcall_004b1a60(void* internal, void*, U id);
void __fastcall kotor_fastcall_005c8bd0(void* sound, void*);
void __fastcall kotor_fastcall_005c8c70(void* sound, void*);
U __fastcall kotor_fastcall_0056d510(void* message, void*, void* player, U id);
U __fastcall kotor_fastcall_0056d570(void* message, void*, void* player, U id);
U __fastcall kotor_fastcall_005646d0(void* party, void*, I npc);
U __fastcall kotor_fastcall_00565560(void* party, void*, I npc);
U __fastcall kotor_fastcall_005ede40(void* client, void*, I pattern);
void __fastcall kotor_fastcall_0062dd20(void* gui, void*, void* script, U flag, I forced1, I forced2);
void __fastcall kotor_fastcall_006be310(void* selection, void*, I npc1, I npc2);
void __fastcall kotor_fastcall_0062e900(void* gui, void*, U shot, U effect);
void __fastcall kotor_fastcall_0062f160(void* gui, void*, U id, U lock);
void __fastcall kotor_fastcall_0062f2b0(void* gui, void*, U id, U lock);
void __fastcall kotor_fastcall_0062f960(void* gui, void*, U ms);
void __fastcall kotor_fastcall_00508770(void* area, void*, U id);
void __fastcall kotor_fastcall_00566600(void* party, void*, void* plot, I percent);
}

using namespace vmx;

namespace {
constexpr unsigned char k_found_0074666c = 0;     // .rdata 0074666C (byte): CGameObjectArray status "found"
constexpr unsigned char k_type_sound_0074663e = 0x10; // .rdata 0074663E (byte): OBJECT_TYPE sound object
constexpr U k_sound_mode_0074c5fc = 4;            // .rdata 0074C5FC (dword)
constexpr U k_tlk_flag_0073ecd0 = 0;              // .rdata 0073ECD0 (byte, zero-extended)
const float k_zero_0073d700 = 0.0f;               // .rdata 0073D700
const float k_ms_0073d6fc = 1000.0f;              // .rdata 0073D6FC
const float k_percent_0073dfec = 0.01f;           // .rdata 0073DFEC (0x3C23D70A)
const char k_empty_0073d71c[] = "";               // .rdata 0073D71C
const char k_xp_007451d4[] = "XP";                // .rdata 007451D4

inline U rd(const void* p, U off) { return at<U>(const_cast<void*>(p), off); }
inline void* rp(const void* p, U off) { return at<void*>(const_cast<void*>(p), off); }
inline void wr(void* p, U off, U v) { at<U>(p, off) = v; }
inline float rf(const void* p, U off) { return at<float>(const_cast<void*>(p), off); }
inline U fbits(float f) { U u; std::memcpy(&u, &f, 4); return u; }
// CClientExoApp::GetInGameGui 005ED690 (mov eax,[ecx+4]; mov eax,[eax+40h]): inline view, no callable provider
inline void* in_game_gui() { return rp(rp(client_app(), 4), 0x40); }
// CServerExoApp::GetPartyTable 004AEE70 (mov eax,[ecx+4]; add eax,1B770h): inline view, no callable provider
inline void* party_table(void* server) { return static_cast<unsigned char*>(rp(server, 4)) + 0x1b770; }
inline I push_int_result(U pushed) { return pushed ? 0 : k_push_failed; }   // neg/sbb/and 7D0/add F830
}  // namespace

extern "C" {

// ============================================================================================ handlers
// 005464D0 GetSubScreenID(): gui && gui+30 ? gui+2C + 1 : 0 (RET 8)
int __fastcall recovered_vm_get_sub_screen_id_005464d0(void*, void*, int, unsigned) {
    void* gui = in_game_gui();                                                             // 005464D0..005464D8
    U id = 0;                                                                              // 005464DD
    if (gui && rd(gui, 0x30)) id = rd(gui, 0x2c) + 1;                                      // 005464DF..005464ED
    return push_int_result(recovered_vm_outer_push_integer_005d1010(vm(), nullptr, static_cast<I>(id))); // 005464EE..00546508
}

// 00546590 PauseGame(int): client->SetPausedByCombat(n, 0, 0) (RET 8)
int __fastcall recovered_vm_pause_game_00546590(void*, void*, int, unsigned) {
    I value;
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &value)) return k_pop_failed; // 00546590..005465AB
    (void)kotor_fastcall_005edc20(client_app(), nullptr, static_cast<U>(value), 0, 0);       // 005465AE..005465C0
    return 0;
}

// 00541120 PlayRumblePattern(int) -> int (RET 8)
int __fastcall recovered_vm_play_rumble_pattern_00541120(void*, void*, int, unsigned) {
    I pattern;
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &pattern)) return k_pop_failed; // 00541120..0054113B
    const U result = kotor_fastcall_005ede40(client_app(), nullptr, pattern);                // 0054113E..0054114C
    return push_int_result(recovered_vm_outer_push_integer_005d1010(vm(), nullptr, static_cast<I>(result))); // 00541151..0054116C
}

// 0053FFB0 MusicBackgroundPlay/Stop/ChangeDay/ChangeNight/SetDelay (routines 425..429) (RET 8).
// The integer is popped only when argc > 1 (signed); the area's music object (+208h) gets a virtual call.
int __fastcall recovered_vm_music_background_0053ffb0(void*, void*, int routine, unsigned argc) {
    U area_id = 0;                                                                         // 0053FFBE [esp+4] = 0
    I value = 0;  // [esp]: never initialised by the original; every routine that reads it has argc 2 (pop below)
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &area_id)) return k_pop_failed; // 0053FFB3..0053FFF2
    if (static_cast<I>(argc) > 1 &&                                                        // 0053FFCF..0053FFD4 (jle)
        !recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &value)) return k_pop_failed; // 0053FFD6..0053FFE8
    void* area = kotor_fastcall_004ae780(server_app(), nullptr, area_id);                    // 0053FFF5..00540003
    if (!area) return 0;                                                                   // 00540008..0054000A
    void* music = rp(area, 0x208);
    switch (static_cast<U>(routine) - 0x1a9u) {                                            // 00540010..0054001F (ja: unsigned)
    case 0: vcall<void>(music, 0x0, 1u); break;                                            // 00540026..00540032 slot 0 (play=1)
    case 1: vcall<void>(music, 0x0, 0u); break;                                            // 0054003C..00540048 slot 0 (0)
    case 2: vcall<void>(music, 0x4, static_cast<U>(value)); break;                         // 00540052..00540061
    case 3: vcall<void>(music, 0x8, static_cast<U>(value)); break;                         // 0054006C..0054007B
    case 4: vcall<void>(music, 0xc, static_cast<U>(value)); break;                         // 00540086..00540095
    default: break;                                                                        // 00540098
    }
    return 0;
}

// 005400C0 MusicBattlePlay/Stop/Change (routines 430..432) (RET 8)
int __fastcall recovered_vm_music_battle_005400c0(void*, void*, int routine, unsigned argc) {
    U area_id = 0;                                                                         // 005400CE
    I value = 0;  // [esp]: uninitialised in the original; read only by routine 432 (argc 2)
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &area_id)) return k_pop_failed; // 005400C3..00540102
    if (static_cast<I>(argc) > 1 &&                                                        // 005400DF..005400E4
        !recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &value)) return k_pop_failed; // 005400E6..005400F8
    void* area = kotor_fastcall_004ae780(server_app(), nullptr, area_id);                    // 00540105..00540113
    if (!area) return 0;                                                                   // 00540118..0054011A
    void* music = rp(area, 0x208);
    switch (routine - 0x1ae) {                                                             // 0054011C..0054012C
    case 0: vcall<void>(music, 0x10, 1u); break;                                           // 0054014C..00540158
    case 1: vcall<void>(music, 0x10, 0u); break;                                           // 00540148..00540158
    case 2: vcall<void>(music, 0x14, static_cast<U>(value)); break;                        // 0054012E..0054013D
    default: break;
    }
    return 0;
}

// 00543BE0 SoundObjectPlay(object) (RET 8)
int __fastcall recovered_vm_sound_object_play_00543be0(void*, void*, int, unsigned) {
    U id;
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &id)) return k_pop_failed;    // 00543BE0..00543BFB
    void* sound = kotor_fastcall_004ae800(server_app(), nullptr, id);                        // 00543BFE..00543C0C
    if (sound) kotor_fastcall_005c8bd0(sound, nullptr);                                    // 00543C11..00543C17
    return 0;
}

// 00543E10 SoundObjectStop(object) (RET 8)
int __fastcall recovered_vm_sound_object_stop_00543e10(void*, void*, int, unsigned) {
    U id;
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &id)) return k_pop_failed;    // 00543E10..00543E2B
    void* sound = kotor_fastcall_004ae800(server_app(), nullptr, id);                        // 00543E2E..00543E3C
    if (sound) kotor_fastcall_005c8c70(sound, nullptr);                                    // 00543E41..00543E47
    return 0;
}

// 00541B40 RemoveAvailableNPC(int npc) -> int (RET 8)
int __fastcall recovered_vm_remove_available_npc_00541b40(void*, void*, int, unsigned) {
    I npc;
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &npc)) return k_pop_failed;  // 00541B40..00541B5B
    const U result = kotor_fastcall_005646d0(party_table(server_app()), nullptr, npc);        // 00541B5E..00541B73
    return push_int_result(recovered_vm_outer_push_integer_005d1010(vm(), nullptr, static_cast<I>(result))); // 00541B78..00541B93
}

// 00541C00 RemovePartyMember(int npc) -> int (RET 8). The NPC object id is read before RemoveMember.
int __fastcall recovered_vm_remove_party_member_00541c00(void*, void*, int, unsigned) {
    I npc;
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &npc)) return k_pop_failed;  // 00541C00..00541C1B
    void* server = server_app();                                                           // 00541C1E..00541C28 (edi)
    void* party = party_table(server);                                                     // 00541C2B..00541C2D (esi)
    const U object = recovered_npc_object_00564700(party, nullptr, npc, 0, 1);              // 00541C32..00541C3F (ebx)
    const U removed = kotor_fastcall_00565560(party, nullptr, npc);                         // 00541C46..00541C4D (ebp)
    if (removed) {                                                                         // 00541C54..00541C56
        void* client = client_app();                                                       // 00541C58..00541C5E
        void* client_creature = reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_R4_005ed5b0(client, nullptr,
            kotor_fastcall_005eda50(client, nullptr, object))));                              // 00541C61..00541C6C
        void* creature = kotor_fastcall_004ae770(server, nullptr, object);                  // 00541C71..00541C76
        if (client_creature) kotor_fastcall_0060ef50(client_creature, nullptr, 0);          // 00541C7B..00541C85
        if (creature) recovered_set_in_party_004fdb20(creature, nullptr, 0, 1);             // 00541C8A..00541C94
    }
    return push_int_result(recovered_vm_outer_push_integer_005d1010(vm(), nullptr, static_cast<I>(removed))); // 00541C99..00541CB8
}

// 005438B0 ShowPartySelectionGUI(string exit_script, int forced1, int forced2) (RET 8).
// The two integers are popped only when argc == 3; otherwise the original passes its uninitialised locals.
int __fastcall recovered_vm_show_party_selection_gui_005438b0(void*, void*, int, unsigned argc) {
    ExoString script; exo_default(&script);                                                // 005438C8..005438CC
    I forced1 = 0, forced2 = 0;  // [esp+4]/[esp]: uninitialised unless argc == 3 (original)
    bool ok = recovered_vm_outer_pop_string_005d1080(vm(), nullptr, &script) != 0;         // 005438D1..005438EB
    if (ok && argc == 3) {                                                                 // 005438ED..005438F2
        ok = recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &forced1) != 0 &&      // 005438F4..00543906
             recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &forced2) != 0;        // 00543908..0054391A
    }
    if (!ok) { exo_destroy(&script); return k_pop_failed; }                                // 0054391C..00543940
    try {
        kotor_fastcall_0062dd20(in_game_gui(), nullptr, &script, 1, forced1, forced2);     // 00543943..00543963
    } catch (...) { exo_destroy(&script); throw; }
    exo_destroy(&script);                                                                  // 00543968..00543974
    return 0;
}

// 00546A80 SetDialogPlaceableCamera(int camera) (RET 8). A second integer is popped when argc >= 2 (signed),
// then discarded: the original overwrites it with -1 and passes -1 as the effect.
int __fastcall recovered_vm_set_dialog_placeable_camera_00546a80(void*, void*, int, unsigned argc) {
    I camera, discarded;
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &camera)) return k_pop_failed; // 00546A80..00546A95
    if (static_cast<I>(argc) >= 2 &&                                                       // 00546A97..00546A9C (jl)
        !recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &discarded)) return k_pop_failed; // 00546A9E..00546AB0
    kotor_fastcall_0062e900(in_game_gui(), nullptr, static_cast<U>(camera), 0xffffffffu);  // 00546ABD..00546ADC
    return 0;
}

// 00546C80 AwardStealthXP(object) (RET 8)
int __fastcall recovered_vm_award_stealth_xp_00546c80(void*, void*, int, unsigned) {
    U id;
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &id)) return k_pop_failed;    // 00546C80..00546C9B
    void* creature = kotor_fastcall_004ae770(server_app(), nullptr, id);                     // 00546C9E..00546CAD
    if (creature) {                                                                        // 00546CB2..00546CB6
        void* area = recovered_object_area_004cb120(creature, nullptr);                    // 00546CB8..00546CBA
        if (area) kotor_fastcall_00508770(area, nullptr, rd(creature, 4));                 // 00546CBF..00546CC9
    }
    return 0;
}

// 005470E0 SetLockOrientationInDialog(object, int) (RET 8)
int __fastcall recovered_vm_set_lock_orientation_in_dialog_005470e0(void*, void*, int, unsigned) {
    U id; I lock;
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &id) ||                       // 005470E0..005470F5
        !recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &lock)) return k_pop_failed; // 005470F7..00547113
    if (id != k_invalid) {                                                                 // 00547116..0054711F
        const U client_id = kotor_fastcall_005eda50(client_app(), nullptr, id);              // 00547121..0054712B
        kotor_fastcall_0062f160(in_game_gui(), nullptr, client_id, static_cast<U>(lock));   // 00547130..00547146
    }
    return 0;
}

// 00547160 SetLockHeadFollowInDialog(object, int) (RET 8)
int __fastcall recovered_vm_set_lock_head_follow_in_dialog_00547160(void*, void*, int, unsigned) {
    U id; I lock;
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &id) ||                       // 00547160..00547175
        !recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &lock)) return k_pop_failed; // 00547177..00547193
    if (id != k_invalid) {                                                                 // 00547196..0054719F
        const U client_id = kotor_fastcall_005eda50(client_app(), nullptr, id);              // 005471A1..005471AB
        kotor_fastcall_0062f2b0(in_game_gui(), nullptr, client_id, static_cast<U>(lock));   // 005471B0..005471C6
    }
    return 0;
}

// 00547340 ShowTutorialWindow(int): 0 -> window 9, 1 -> 28h, 2 -> 2Ah, other values do nothing (RET 8)
int __fastcall recovered_vm_show_tutorial_window_00547340(void*, void*, int, unsigned) {
    I which;
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &which)) return k_pop_failed; // 00547340..0054735B
    U window;
    switch (which) {                                                                       // 0054735E..0054736B
    case 0: window = 9; break;                                                             // 0054737B
    case 1: window = 0x28; break;                                                          // 00547374
    case 2: window = 0x2a; break;                                                          // 0054736D
    default: return 0;                                                                     // 0054736B -> 0054739F
    }
    (void)show_tutorial_window_005edf40(client_app(), nullptr, window, k_invalid, k_invalid, 0); // 00547380..0054739A
    return 0;
}

// 00547BC0 ResetDialogState(): gui->SetGlobalDialogState(0) (RET 8)
int __fastcall recovered_vm_reset_dialog_state_00547bc0(void*, void*, int, unsigned) {
    kotor_fastcall_0062ec60(in_game_gui(), nullptr, 0);                                    // 00547BC0..00547BD1
    return 0;
}

// 00547DC0 NoClicksFor(float seconds) (RET 8). Original quirk: a pop failure returns -2000 (0xFFFFF830).
// fcomp [0.0] / test ah,41h / jne: only an ordered seconds > 0.0 marks the no-click window (NaN skips).
int __fastcall recovered_vm_no_clicks_for_00547dc0(void*, void*, int, unsigned) {
    float seconds = 0.0f;                                                                  // 00547DCC
    if (!recovered_vm_outer_pop_float_005d1020(vm(), nullptr, &seconds)) return k_push_failed; // 00547DC1..00547DE3
    if (seconds > k_zero_0073d700) {                                                       // 00547DE6..00547DF5
        const U ms = w2::ftol2_006fae8c(static_cast<double>(seconds) *
                                        static_cast<double>(k_ms_0073d6fc));                // 00547DF7..00547E01
        kotor_fastcall_0062f960(in_game_gui(), nullptr, ms);                               // 00547E06..00547E17
    }
    return 0;
}

// 00548900 BarkString(object, int strref) (RET 8). Original quirk: an object pop failure returns -2000.
// Skipped while gui+B4 (dialog active) is set. OBJECT_INVALID -> GUI bark bubble, else the client bark path.
int __fastcall recovered_vm_bark_string_00548900(void*, void*, int, unsigned) {
    U id; I strref;
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &id)) return k_push_failed;   // 0054891E..0054893F
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &strref)) return k_pop_failed; // 00548942..00548969
    if (rd(in_game_gui(), 0xb4) != 0) return 0;                                            // 0054896C..00548982
    alignas(4) unsigned char res[0x1c];                                                    // STR_RES [esp+20h..3Bh]
    feedback_str_res_construct_00414dc0(res, nullptr);                                     // 00548988..0054898C
    ExoString text;                                                                        // [esp+8]
    U sound[4];                                                                            // [esp+10h..1Ch] CResRef copy
    try {
        (void)tlk_fetch_0041e550(kotor_cell_tlk_007a3a08, nullptr, static_cast<U>(strref), res,
                                 k_tlk_flag_0073ecd0);                                     // 00548991..005489B1
        std::memcpy(sound, res + 8, 16);                                                   // 005489B6..005489DB
        WindowCopy005e5b00(&text, nullptr, res);                                           // 005489C6..005489DF
    } catch (...) { exo_destroy(reinterpret_cast<ExoString*>(res)); throw; }
    try {
        if (id == k_invalid) {                                                             // 005489E4..005489F2
            object_projectile_bark_0062ab30(in_game_gui(), nullptr, k_invalid, &text, sound, 0); // 005489F4..00548A15
        } else {
            const U client_id = kotor_fastcall_005eda50(client_app(), nullptr, id);          // 00548A1C..00548A31
            kotor_fastcall_005edbb0(client_app(), nullptr, client_id, &text, sound, 0);      // 00548A2B..00548A45
        }
    } catch (...) { exo_destroy(&text); exo_destroy(reinterpret_cast<ExoString*>(res)); throw; }
    exo_destroy(&text);                                                                    // 00548A4A..00548A53
    exo_destroy(reinterpret_cast<ExoString*>(res));                                        // 00548A58..00548A64
    return 0;
}

// 00545E20 GivePlotXP(string plot, int percent) (RET 8). Original quirk: a string pop failure returns -2000.
// The string local is constructed from "" (0073D71C) before the pop.
int __fastcall recovered_vm_give_plot_xp_00545e20(void*, void*, int, unsigned) {
    ExoString plot;
    frame_LogStringCtor_005e5a90(&plot, nullptr, k_empty_0073d71c);                        // 00545E38..00545E41
    if (!recovered_vm_outer_pop_string_005d1080(vm(), nullptr, &plot)) {                   // 00545E46..00545E60
        exo_destroy(&plot); return k_push_failed;                                          // 00545E62..00545E86
    }
    I percent;
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &percent)) {               // 00545E89..00545E9B
        exo_destroy(&plot); return k_pop_failed;                                           // 00545E9D..00545EC1
    }
    try {
        kotor_fastcall_00566600(party_table(server_app()), nullptr, &plot, percent);       // 00545EC4..00545EDE
    } catch (...) { exo_destroy(&plot); throw; }
    exo_destroy(&plot);                                                                    // 00545EE3..00545EEF
    return 0;
}

// ============================================================================================ reconstructed callees
// 004AE800 CServerExoApp::GetSoundObjectByGameObjectID(id): mov ecx,[ecx+4]; jmp 004B1A60 (RET 4)
void* __fastcall kotor_fastcall_004ae800(void* server, void*, U id) {
    return kotor_fastcall_004b1a60(rp(server, 4), nullptr, id);
}

// 004B1A60 CServerExoAppInternal::GetSoundObjectByGameObjectID(id) (RET 4)
void* __fastcall kotor_fastcall_004b1a60(void* internal, void*, U id) {
    void* object = nullptr;                                                                // 004B1A71
    const unsigned char status = static_cast<unsigned char>(
        kotor_fastcall_004d8230(rp(internal, 0x1005c), nullptr, id, &object));             // 004B1A61..004B1A79
    if (status == k_found_0074666c && object &&                                            // 004B1A7E..004B1A8C
        at<unsigned char>(object, 8) == k_type_sound_0074663e)                             // 004B1A8E..004B1A97
        return vcall<void*>(object, 0x68);                                                 // 004B1A99..004B1A9B AsSWSSoundObject
    return nullptr;                                                                        // 004B1AA2
}

namespace {
// 005C8BD0 / 005C8C70 shared shape: set +228, then send to every player whose game object is a creature.
static void sound_object_broadcast(void* sound, U playing, U (__fastcall* send)(void*, void*, void*, U)) {
    wr(sound, 0x228, playing);                                                             // +04..+0E
    void* server = server_app();
    void* message = recovered_server_sws_message_004aedb0(server, nullptr);                // +0E..+18 (ebp)
    void* players = recovered_server_player_list_004aed90(server_app(), nullptr);          // +1D..+28 (edi)
    if (!message || !players) return;                                                      // +2D..+35
    void* position = rp(rp(players, 0), 0);                                                // +37..+3D head
    if (!position) return;                                                                 // +3D..+41
    do {
        void* player = unresolved_list_at_pos_005e9b60(rp(players, 0), nullptr, position);          // +44..+4C
        if (player && kotor_fastcall_00561080(player, nullptr) &&                          // +4E..+5B
            vcall<void*>(kotor_fastcall_00561080(player, nullptr), 0x30))                  // +5D..+6D AsSWSCreature
            (void)send(message, nullptr, player, rd(sound, 4));                            // +6F..+76
        (void)unresolved_list_next_005e9b90(rp(players, 0), nullptr, &position);           // +7B..+82
    } while (position);                                                                    // +87..+8D
}
}  // namespace

// 005C8BD0 CSWSSoundObject::Play() (RET 0)
void __fastcall kotor_fastcall_005c8bd0(void* sound, void*) { sound_object_broadcast(sound, 1, &kotor_fastcall_0056d510); }
// 005C8C70 CSWSSoundObject::Stop() (RET 0)
void __fastcall kotor_fastcall_005c8c70(void* sound, void*) { sound_object_broadcast(sound, 0, &kotor_fastcall_0056d570); }

namespace {
// 0056D510 / 0056D570: message 17h minor 2 (play) / 3 (stop) carrying the sound object id (RET 8)
static U send_sound_object(void* message, void* player, U id, U minor) {
    kotor_fastcall_004d68a0(message, nullptr, 4, 0xffffffffu, 1);                          // +04..+0C
    kotor_fastcall_00578570(message, nullptr, id);                                         // +11..+18
    void* data = nullptr; U size = 0;
    if (kotor_fastcall_004d6440(message, nullptr, &data, &size) == 0) return 0;            // +1D..+30 (EAX 0)
    return kotor_fastcall_00567e60(message, nullptr, rd(player, 4), 0x17, minor, data, size); // +39..+51
}
}  // namespace
// 0056D510 CSWSMessage::SendServerToPlayerSoundObject_Play(CSWSPlayer*, id)
U __fastcall kotor_fastcall_0056d510(void* message, void*, void* player, U id) { return send_sound_object(message, player, id, 2); }
// 0056D570 CSWSMessage::SendServerToPlayerSoundObject_Stop(CSWSPlayer*, id)
U __fastcall kotor_fastcall_0056d570(void* message, void*, void* player, U id) { return send_sound_object(message, player, id, 3); }

// 005646D0 CSWPartyTable::RemoveNPC(int npc): 0 <= npc < 9 and slot +30+4n set -> clear it, 1; else 0 (RET 4)
U __fastcall kotor_fastcall_005646d0(void* party, void*, I npc) {
    if (npc < 0 || npc >= 9 || rd(party, 0x30 + 4u * static_cast<U>(npc)) == 0) return 0;  // 005646D0..005646E7
    wr(party, 0x30 + 4u * static_cast<U>(npc), 0);                                         // 005646EA
    return 1;                                                                              // 005646F2
}

// 00565560 CSWPartyTable::RemoveMember(int npc) (RET 4): only outside a forced-party state (+F0 == -1) with members;
// clears the NPC creature's effects, removes npc from the member list (+0 count, +4.. ids), clears +190 on empty.
U __fastcall kotor_fastcall_00565560(void* party, void*, I npc) {
    if (static_cast<I>(rd(party, 0xf0)) != -1) return 0;                                   // 00565565..0056556D
    if (static_cast<I>(rd(party, 0)) <= 0) return 0;                                       // 0056556F..00565572
    if (npc < 0 || npc >= 9) return 0;                                                     // 00565574..0056557F
    if (rd(party, 0x30 + 4u * static_cast<U>(npc)) == 0) return 0;                         // 00565581..00565587
    void* server = server_app();                                                           // 00565589..0056558E (edi)
    const U object = recovered_npc_object_00564700(party, nullptr, npc, 0, 1);              // 00565591..00565596
    void* creature = kotor_fastcall_004ae770(server, nullptr, object);                      // 0056559B..0056559E
    if (creature) kotor_fastcall_004d0940(creature, nullptr, 0);                            // 005655A3..005655AB
    const I count = static_cast<I>(rd(party, 0));                                          // 005655B0 (edi)
    I i = 0;                                                                               // 005655B2 (eax)
    if (count <= 0) return 0;                                                              // 005655B4..005655B6
    U cursor = 4;                                                                          // 005655B8..005655BB (ecx = party+4)
    for (;;) {
        const I member = static_cast<I>(rd(party, cursor));                                // 005655C0
        ++i;                                                                               // 005655C2
        if (npc == member) break;                                                          // 005655C3..005655C5
        cursor += 4;                                                                       // 005655C9
        if (!(i < static_cast<I>(rd(party, 0)))) return 0;                                 // 005655C7..005655CE
    }
    if (i < count) {                                                                       // 005655D9..005655DB (count read at 005655B0)
        do {
            wr(party, 4u * static_cast<U>(i), rd(party, 4 + 4u * static_cast<U>(i)));      // 005655E0..005655E3
            ++i;                                                                           // 005655E8
        } while (i < static_cast<I>(rd(party, 0)));                                        // 005655E6..005655EB
    }
    const U left = rd(party, 0) - 1; wr(party, 0, left);                                   // 005655ED dec [esi]
    if (left == 0 && rd(party, 0x190) != 0) wr(party, 0x190, 0);                           // 005655EF..005655FB
    return 1;                                                                              // 00565608
}

// 005EDE40 CClientExoApp::PlayRumblePattern(int): mov ecx,[ecx+4]; jmp 005FB490 (RET 4)
U __fastcall kotor_fastcall_005ede40(void* client, void*, I pattern) {
    return area_vfx_queue_announce_005fb490(rp(client, 4), nullptr, pattern);
}

// 0062DD20 CGuiInGame::ShowPartySelection(CExoString* exit_script, int flag, int forced1, int forced2) (RET 10h)
void __fastcall kotor_fastcall_0062dd20(void* gui, void*, void* script, U flag, I forced1, I forced2) {
    void* server = server_app();                                                           // 0062DD2E..0062DD3C (edi)
    if (rd(gui, 0x108) == 0) return;                                                       // 0062DD41..0062DD49
    if (static_cast<I>(rd(party_table(server), 0xf0)) != -1) return;                       // 0062DD4F..0062DD5F
    if (!kotor_fastcall_004ae770(server, nullptr, kotor_fastcall_004aea40(server, nullptr))) return; // 0062DD65..0062DD76
    if (!recovered_object_area_004cb120(
            kotor_fastcall_004ae770(server, nullptr, kotor_fastcall_004aea40(server, nullptr)), nullptr)) return; // 0062DD7C..0062DD94
    void* area = recovered_object_area_004cb120(
        kotor_fastcall_004ae770(server, nullptr, kotor_fastcall_004aea40(server, nullptr)), nullptr); // 0062DD9A..0062DDAB
    if (rd(area, 0x2ac) != 0 && flag == 0) return;                                         // 0062DDB0..0062DDC0
    if (frame_ContainsPanel_0040bd70(rp(gui, 0x38), nullptr, rp(gui, 0x78))) return;       // 0062DDC6..0062DDD4
    if (flag) {                                                                            // 0062DDDA..0062DDDC
        if (rd(gui, 0xb38) == 0)                                                           // 0062DDDE..0062DDE6
            frame_TogglePauseRequest_00677800(kotor_fastcall_005ed6f0(client_app(), nullptr), nullptr); // 0062DDE8..0062DDF8
        kotor_fastcall_0040a140(rp(gui, 0x38), nullptr, 4);                                // 0062DDFD..0062DE02
        frame_R4_005d5e80(reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::sound_manager_007a39ec)),
                          nullptr, k_sound_mode_0074c5fc);                                 // 0062DE07..0062DE13
    }
    if (rp(gui, 0x78) == nullptr) {                                                        // 0062DE18..0062DE1D
        void* memory = live_operator_new(0x3a80);                                          // 0062DE1F..0062DE29
        void* panel = nullptr;                                                             // 0062DE49
        if (memory) {                                                                      // 0062DE30..0062DE3A
            try { panel = kotor_fastcall_006bfa40(memory, nullptr, rp(gui, 0x38)); }       // 0062DE3C..0062DE42 (EH state 0)
            catch (...) { live_free(memory); throw; }
        }
        at<void*>(gui, 0x78) = panel;                                                      // 0062DE53
    }
    void* panel = rp(gui, 0x78);
    (void)async_assign_string_005e5c50(static_cast<unsigned char*>(panel) + 0x74, nullptr, script); // 0062DE56..0062DE61
    wr(rp(gui, 0x78), 0x70, flag);                                                         // 0062DE6A..0062DE72
    kotor_fastcall_006be310(rp(gui, 0x78), nullptr, forced1, forced2);                     // 0062DE66..0062DE79
    wr(rp(gui, 0x78), 0x64, flag != 0 ? 0u : 2u);                                          // 0062DE7E..0062DE90 (setne/dec/and 2)
    kotor_fastcall_0040bc70(rp(gui, 0x38), nullptr, rp(gui, 0x78), 3, 1);                  // 0062DE88..0062DE9A
    kotor_fastcall_005eda60(client_app(), nullptr, 2, 1);                                  // 0062DE9F..0062DEAB
}

// 006BE310 CSWGuiPartySelection::SetForcedNPC(int a, int b) (RET 8): each valid index (0..8) sets +6C = 1 and
// ORs 4 into the member button flags at +4C4 + 454h*n.
void __fastcall kotor_fastcall_006be310(void* selection, void*, I npc1, I npc2) {
    if (npc1 >= 0 && npc1 < 9) {                                                           // 006BE310..006BE320
        wr(selection, 0x6c, 1);                                                            // 006BE32F
        wr(selection, 0x4c4 + 0x454u * static_cast<U>(npc1), rd(selection, 0x4c4 + 0x454u * static_cast<U>(npc1)) | 4u); // 006BE322..006BE332
    }
    if (npc2 >= 0 && npc2 < 9) {                                                           // 006BE335..006BE340
        wr(selection, 0x6c, 1);                                                            // 006BE34F
        wr(selection, 0x4c4 + 0x454u * static_cast<U>(npc2), rd(selection, 0x4c4 + 0x454u * static_cast<U>(npc2)) | 4u); // 006BE342..006BE352
    }
}

// 0062E900 CGuiInGame::SetShotFromScript(int shot, int effect) (RET 8). Only during a dialog (+B4). With the
// computer panel up (+3C == +44): InitializeShotCamera if needed, then SetComputerCameraShot(shot, +160, effect).
// Otherwise the dialog camera (module camera 106Dh) takes placed-camera shot `shot` from the +1B4 table (30h each).
// Unlike SetComputerCameraShot, effect -1 and -2 both leave the video effect untouched, and +160 is not written.
void __fastcall kotor_fastcall_0062e900(void* gui, void*, U shot, U effect) {
    if (rd(gui, 0xb4) == 0) return;                                                        // 0062E908..0062E911
    if (rd(gui, 0x3c) == rd(gui, 0x44)) {                                                  // 0062E917..0062E91D
        if (rd(gui, 0xb20) == 0) (void)kotor_fastcall_0062bf30(gui, nullptr);              // 0062EADB..0062EAE7
        kotor_fastcall_0062c030(gui, nullptr, shot, rd(gui, 0x160), effect);               // 0062EAEC..0062EAFF
        return;
    }
    void* module = kotor_fastcall_005ed530(client_app(), nullptr);                           // 0062E923..0062E92C
    void* camera = vcall<void*>(rp(module, 0x40), 0x80, 0x106du);                          // 0062E931..0062E93B
    if (!camera) return;                                                                   // 0062E941..0062E947
    if (static_cast<I>(rd(gui, 0x1b0)) <= 0) return;                                       // 0062E94D..0062E957
    U i = 0;                                                                               // ebx
    for (;;) {
        if (rd(gui, 0x1b4 + i * 0x30) == shot) break;                                      // 0062E961..0062E969
        ++i;                                                                               // 0062E971
        if (!(static_cast<I>(i) < static_cast<I>(rd(gui, 0x1b0)))) return;                 // 0062E96B..0062E977
    }
    void* entry = static_cast<unsigned char*>(gui) + i * 0x30;                             // 0062E983..0062E989 (edi)
    game_vm::V3 position;
    position.x = rd(entry, 0x1b8);                                                         // 0062E992 (ebp)
    position.y = rd(entry, 0x1bc);                                                         // 0062E994 (edx)
    position.z = fbits(static_cast<float>(static_cast<double>(rf(entry, 0x1c0)) +
                                          static_cast<double>(rf(entry, 0x1d8))));         // 0062E997..0062E9B6
    game_vm::Q4 orientation;                                                               // [esp+20h..2Ch]
    std::memcpy(&orientation, static_cast<unsigned char*>(entry) + 0x1c4, 16);             // 0062E9A8..0062E9CF
    game_vm::Q4 source;
    std::memcpy(&source, static_cast<unsigned char*>(entry) + 0x1c4, 16);                  // 0062E9D3..0062E9EB
    const float roll = static_cast<float>(kotor_cdecl_004acb90(source));                   // 0062E9EE..0062E9F3
    const float pitch = static_cast<float>(quaternion_pitch_004aa020(&orientation) +
                                           static_cast<double>(rf(entry, 0x1d4)));         // 0062E9FA..0062EA0E
    const float yaw = static_cast<float>(quaternion_yaw_004a9f40(&orientation));           // 0062EA0A..0062EA17
    game_vm::Q4 rotated;                                                                   // [esp+3Ch]
    const void* r = yaw_pitch_roll_004acac0(&rotated, yaw, pitch, roll);                   // 0062EA1A..0062EA1F
    std::memcpy(&orientation, r, 16);                                                      // 0062EA24..0062EA3D
    const U shot_height = fbits(rf(gui, 0x1e0 + i * 0x30));                                // 0062EA42..0062EA52 (into the arg slot)
    const U fov = fbits(rf(entry, 0x1dc));                                                 // 0062EA58..0062EA62
    game_vm::Q4 shot_orientation;
    std::memcpy(&shot_orientation, r, 16);                                                 // 0062EA50..0062EA77 (from the result again)
    kotor_fastcall_006bb590(camera, nullptr, 6, position, shot_orientation, fov);          // 0062EA7A..0062EA8D
    wr(gui, 0xe8, position.x);                                                             // 0062EA96..0062EA9C
    wr(gui, 0xf4, shot_height);                                                            // 0062EA92..0062EA9E
    wr(gui, 0xec, position.y);                                                             // 0062EAAB
    wr(gui, 0xe4, 1);                                                                      // 0062EAAE
    wr(gui, 0xf0, position.z);                                                             // 0062EAB8
    if (effect == 0xffffffffu || effect == 0xfffffffeu) return;                            // 0062EAA4..0062EAC0
    kotor_fastcall_005edf10(client_app(), nullptr, static_cast<I>(effect));                  // 0062EAC2..0062EACC
}

namespace {
// shared body of 0062F160 / 0062F2B0: the GUI keeps a linked list of client ids (4-byte heap cells).
// lock == 1: add the id once if it names a client object whose slot +0C (AsClientObject-like) is non-null;
// otherwise remove the first matching node (the 4-byte cell is not freed, original). Returns true when added.
static bool dialog_lock_list(void* gui, U list_offset, U id, U lock, void** object_out) {
    void* list = rp(gui, list_offset);
    if (lock == 1) {
        void* object = kotor_fastcall_005ed580(client_app(), nullptr, id);                   // +0F..+1C
        *object_out = object;
        if (!object || !vcall<void*>(object, 0xc)) return false;                           // +21..+32
        list = rp(gui, list_offset);
        void* position = rp(list, 0);                                                      // +38..+40
        U* data = position ? static_cast<U*>(rp(position, 8)) : nullptr;                   // +44..+4A
        while (position) {                                                                 // +4D..+4F
            if (*data == id) return false;                                                 // +51..+53
            data = reinterpret_cast<U*>(static_cast<std::uintptr_t>(
                unresolved_list_next_005e9b90(rp(gui, list_offset), nullptr, &position))); // +59..+64
        }
        U* cell = static_cast<U*>(live_operator_new(4));                                   // +71..+78
        if (!cell) return false;                                                           // +7B..+7D
        *cell = id;                                                                        // +7F
        (void)c2da_005e9d70(rp(gui, list_offset), nullptr, cell);                          // +81..+88
        return true;
    }
    void* position = rp(list, 0);                                                          // remove path: head
    U* data = position ? static_cast<U*>(rp(position, 8)) : nullptr;
    while (position) {
        if (*data == id) { c2da_005e9c10(rp(gui, list_offset), nullptr, position); return false; } // Remove(pos)
        data = reinterpret_cast<U*>(static_cast<std::uintptr_t>(
            unresolved_list_next_005e9b90(rp(gui, list_offset), nullptr, &position)));
    }
    return false;
}
}  // namespace

// 0062F160 CGuiInGame::SetLockObjectOrientationInDialog(client id, int lock) (RET 8): list at +B44
void __fastcall kotor_fastcall_0062f160(void* gui, void*, U id, U lock) {
    void* object = nullptr;
    (void)dialog_lock_list(gui, 0xb44, id, lock, &object);                                 // 0062F160..0062F23F
}

// 0062F2B0 CGuiInGame::SetLockHeadFollowInDialog(client id, int lock) (RET 8): list at +B48; after adding,
// a client creature (slot 2Ch AsSWCCreature) looks at nothing: LookAt(OBJECT_INVALID, 0).
void __fastcall kotor_fastcall_0062f2b0(void* gui, void*, U id, U lock) {
    void* object = nullptr;
    if (!dialog_lock_list(gui, 0xb48, id, lock, &object)) return;                          // 0062F2B0..0062F33B
    if (!vcall<void*>(object, 0x2c)) return;                                               // 0062F340..0062F349
    kotor_fastcall_006146e0(vcall<void*>(object, 0x2c), nullptr, k_invalid, 0);            // 0062F34B..0062F35B
}

// 0062F960 CGuiInGame::MarkNoClickEvent(ms) (RET 4): world time -> +BCC/+BD0, duration -> +BD4
void __fastcall kotor_fastcall_0062f960(void* gui, void*, U ms) {
    unsigned char* g = static_cast<unsigned char*>(gui);
    frame_GetWorldTime_004ade40(frame_GetWorldTimer_005ed4f0(client_app(), nullptr), nullptr, g + 0xbcc, g + 0xbd0); // 0062F960..0062F980
    wr(gui, 0xbd4, ms);                                                                    // 0062F985..0062F989
}

// 00508770 CSWSArea::AwardStealthXP(creature id) (RET 4): pending stealth XP (+2B8) when +2C0 is set.
// The party-table null test is always true (004AEE70 returns internal+1B770), kept as written.
void __fastcall kotor_fastcall_00508770(void* area, void*, U id) {
    if (!kotor_fastcall_004ae770(server_app(), nullptr, id)) return;                         // 00508770..00508787
    if (rd(area, 0x2c0) == 0) return;                                                      // 0050878D..00508795
    if (rd(area, 0x2b8) == 0) return;                                                      // 00508797..0050879F (jbe)
    if (party_table(server_app()))                                                         // 005087A1..005087B0
        recovered_party_distribute_experience_005653a0(party_table(server_app()), nullptr,
                                                       static_cast<I>(rd(area, 0x2b8)), 1); // 005087B2..005087CB
    if (in_game_gui())                                                                     // 005087D0..005087E0
        kotor_fastcall_0062eeb0(in_game_gui(), nullptr, 3, rd(area, 0x2b8));               // 005087E2..005087FB
    wr(area, 0x2b8, 0);                                                                    // 00508800
    wr(area, 0x2c0, 0);                                                                    // 0050880A
}

// 00566600 CSWPartyTable::GivePlotXP(CExoString* plot, int percent) (RET 8): plot XP from rules table (+B8)+EC
// row `plot`, column "XP"; xp = ceil((XP * percent) * 0.01f) (imul wraps; fild/fmul/ceil/ftol2).
void __fastcall kotor_fastcall_00566600(void* party, void*, void* plot, I percent) {
    I value = 0;                                                                           // 0056661D [esp] = 0
    void* table = rp(rp(kotor_cell_rules_007a3a28, 0xb8), 0xec);                           // 00566618..0056662C
    if (!table) return;                                                                    // 00566632..00566637
    ExoString column;
    frame_LogStringCtor_005e5a90(&column, nullptr, k_xp_007451d4);                         // 0056663D..00566646
    try {
        (void)kotor_fastcall_00414110(table, nullptr, plot, &column, &value);              // 0056664B..00566664
    } catch (...) { exo_destroy(&column); throw; }
    exo_destroy(&column);                                                                  // 00566669..00566675
    const I scaled = static_cast<I>(static_cast<U>(value) * static_cast<U>(percent));      // 0056667A..00566683 (imul)
    const double product = static_cast<double>(scaled) * static_cast<double>(k_percent_0073dfec); // 0056668A..00566694
    const I xp = static_cast<I>(w2::ftol2_006fae8c(std::ceil(product)));                   // 00566697..005666A4 (_ceil, __ftol2)
    if (xp <= 0) return;                                                                   // 005666A6..005666A8
    recovered_party_distribute_experience_005653a0(party, nullptr, xp, 1);                 // 005666AA..005666AF
    kotor_fastcall_0062eeb0(in_game_gui(), nullptr, 2, static_cast<U>(xp));                // 005666B4..005666C7
}

}  // extern "C"
