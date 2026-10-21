// Lane vmC, part B (party / journal / GUI): NWScript engine-command handlers CSWVirtualMachineCommands::ExecuteCommand*
// and their missing callees, reconstructed from the original swkotor.exe (read-only, capstone linear sweep).
// Addresses in the comments are original instruction ranges.
//  * Every handler: int __thiscall (int routine, int argc), RET 8 ->
//    extern "C" int __fastcall kotor_fastcall_<addr>(void* self, void*, int routine, unsigned argc)  (@16).
//  * Return literals: 0 / -2001 (FFFFF82Fh) pop failure / -2000 (FFFFF830h) push failure (neg/sbb/and 7D0h/add).
//  * Memory: live_operator_new / live_free only.  No original address is dereferenced.
//  * x87: 53-bit precision; every x87 intermediate is a double, every `fstp dword` an explicit (float).
// Original quirks kept (documented at each site):
//  - SuppressStatusSummaryEntry returns -2000 (not -2001) when its pop fails.
//  - AddJournalQuestEntry returns 1 when the server player list is null.
//  - SwitchPlayerCharacter: reads the first player's +38h without a null check; leaks the CResGFF it saves the PC
//    into; the inventory loop counts the new creature's repository but fetches items from [party table+118h]; a null
//    operator new for the PC creature is not checked before LoadFromTemplate.
//  - AddWorldJournalEntryStrref ORs bit 0 into an uninitialised CWorldJournalEntry flags dword.
#include "1e78b2f608-claude_vm_vmc_common.hpp"

namespace frame_control { extern std::uint32_t sound_manager_007a39ec; }   // g_pExoSound 007A39EC [0673]
namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); }  // CExoString() [menu20_bindings]
namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void* server, void*); }      // CServerExoApp::GetPartyTable [0575]
namespace menu_resources { struct Ref { unsigned char bytes[16]; Ref* __thiscall construct_00406d80(const char* text); }; } // CResRef(char*) [0030]

namespace vmc_party {
struct WorldJournalEntryArg { w5::U w[9]; };   // CWorldJournalEntry by value (0x24 bytes; callee destroys it)
static_assert(sizeof(WorldJournalEntryArg) == 0x24, "CWorldJournalEntry by value");
}

extern "C" {
extern void* kotor_cell_rules_007a3a28;                                                              // g_pRules 007A3A28 [0421]
// ---- existing C-linkage providers (c31 map; object in brackets) ----
void* __fastcall frame_R4_005ed690(void* client, void*);                                             // CClientExoApp::GetInGameGui [0457]
U __fastcall kotor_fastcall_00563710(void* table, void*, I npc);                                     // CSWPartyTable::GetIsMember [claude_gui_char_party]
U __fastcall kotor_fastcall_00565620(void* table, void*, I npc, U id);                               // CSWPartyTable::AddMember [claude_gui_char_party]
U __fastcall kotor_fastcall_0060ef50(void* creature, void*, U in_party);                             // CSWCCreature::SetInParty [1205-32]
void __fastcall recovered_set_in_party_004fdb20(void* creature, void*, U in_party, U flag);          // CSWSCreature::SetInParty [w2_ifo_native]
void __fastcall kotor_fastcall_005ee1c0(void* client, void*, void* color, void* movie);              // CClientExoApp::StartCreditSequence [0810]
void* __fastcall kotor_m15c_namegen_construct_00711290(void* gen, void*);                            // CSWNameGen::CSWNameGen [0822]
void __fastcall kotor_m15c_namegen_load_00711340(void* gen, void*, void* name);                      // CSWNameGen::LoadNameTable [0822]
void* __fastcall kotor_m15c_namegen_random_letters_00711390(void* gen, void*, void* out, U flag);    // CSWNameGen::GetRandomName RET 8 [0822]
void __fastcall kotor_m15c_namegen_unload_00711090(void* gen, void*);                                // CSWNameGen::UnloadNameTable [0822]
void __fastcall kotor_m15c_namegen_destruct_007112e0(void* gen, void*);                              // CSWNameGen::~CSWNameGen [0822]
void* __fastcall frame_LogStringCtor_005e5a90(void* s, void*, const char* text);                     // CExoString(const char*) [0459]
void* __fastcall GuiAssign005e5140(void* s, void*, const char* text);                                // CExoString::operator=(const char*) [0093]
void* __fastcall kotor_fastcall_004aede0(void* server, void*);                                       // CServerExoApp::GetWorldTimer [0457]
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, void* days, void* ms);               // CWorldTimer::GetWorldTime [0457]
void* __fastcall recovered_server_player_list_004aed90(void* server, void*);                         // CServerExoApp::GetPlayerList [w1_native_b7]
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);                         // CServerExoApp::GetSWSMessage
void* __fastcall server_get_module_004ae6b0(void* server, void*);                                    // CServerExoApp::GetModule [0530]
void* __fastcall unresolved_list_at_pos_005e9b60(void* list, void*, void* pos);                      // CExoLinkedListInternal::GetAtPos [0168]
U __fastcall unresolved_list_next_005e9b90(void* list, void*, void** pos);                           // CExoLinkedListInternal::GetNext [0168]
void* __fastcall kotor_fastcall_00561080(void* player, void*);                                       // CSWSPlayer::GetGameObject [0595]
void* __fastcall kotor_fastcall_004edb00(void* creature, void*);                                     // CSWSCreature::GetJournal [claude_dialogue_server]
I __fastcall kotor_fastcall_005c58a0(void* journal, void*, void* tag);                               // CSWSJournal::GetState
I __fastcall kotor_fastcall_005c5850(void* journal, void*, void* tag);                               // CSWSJournal::GetQuestEntryXP
void __fastcall kotor_fastcall_005c63f0(void* journal, void*, void* tag, U date, U flag);            // CSWSJournal::SetDate
void __fastcall kotor_fastcall_005c6650(void* journal, void*, void* tag, U time, U flag);            // CSWSJournal::SetTime
void __fastcall kotor_fastcall_005c5a40(void* journal, void*, void* tag, U state, U flag);           // CSWSJournal::SetState
void __fastcall recovered_party_distribute_experience_005653a0(void* party, void*, I xp, U flag);    // CSWPartyTable::DistributeExperience [w2]
void __fastcall kotor_fastcall_004d68a0(void* message, void*, U size, U a, U b);                     // CSWMessage::CreateWriteMessage [w2_message_native]
void __fastcall kotor_fastcall_004d6340(void* message, void*, U value, U bits);                      // CSWMessage::WriteDWORD
void __fastcall kotor_fastcall_004d62c0(void* message, void*, U value, U bits);                      // CSWMessage::WriteBYTE
U __fastcall kotor_fastcall_004d6440(void* message, void*, void** data, U* size);                    // CSWMessage::GetWriteMessage
U __fastcall kotor_fastcall_00567e60(void* message, void*, U player, U major, U minor, void* data, U size); // SendServerToPlayerMessage
void* __fastcall kotor_fastcall_004c4080(void* entry, void*, const void* src);                       // CWorldJournalEntry(const&) [claude_p2s_b_journal]
void __fastcall kotor_fastcall_004c65e0(void* list, void*, vmc_party::WorldJournalEntryArg entry);   // CExoArrayList<CWorldJournalEntry>::Add [claude_p2s_b_journal]
void __fastcall world_journal_entry_destroy_004c4030(void* entry, void*);                            // ~CWorldJournalEntry [0577]
void __stdcall c2da_destroy_array_006fa852(void* first, U size, int count, void* destroy);           // eh_vector_destructor_iterator [0186]
U __fastcall kotor_fastcall_005edc10(void* client, void*);                                           // CClientExoApp::GetPausedByCombat
U __fastcall kotor_fastcall_005edc20(void* client, void*, U a, U b, U c);                            // CClientExoApp::SetPausedByCombat [0714]
void __fastcall kotor_fastcall_005edb70(void* client, void*, void* object, U flag);                  // CClientExoApp::IndicateObject [1205-30]
void __fastcall frame_R4_005d5e80(void* sound, void*, U mode);                                       // CExoSoundInternal::SetSoundMode [0709]
void __fastcall frame_R4_0062aa00(void* gui, void*, U a, U b);                                       // CGuiInGame::SetSWGuiStatus
void __fastcall object_projectile_bark_006a9920(void* bubble, void*, U id, void* text, void* sound, U flags); // CSWGuiBarkBubble::SetBark
// (declared in w5 headers) void __fastcall kotor_fastcall_005eda60(void* client, void*,
void* __fastcall kotor_fastcall_00695180(void* self, void*, void* manager);                          // CSWGuiInGameGalaxyMap() [panels_menus]
void __fastcall kotor_fastcall_00563b60(void* table, void*, I planet);                               // CSWPartyTable::SetSelectedPlanet [claude_menu_handlers_a]
void __fastcall kotor_fastcall_0040bc70(void* manager, void*, void* panel, U a, U b);                // CSWGuiManager::AddPanel [0298]
void __fastcall kotor_fastcall_0040a140(void* manager, void*, U sound);                              // CSWGuiManager::PlayGuiSound [1205-30]
void __cdecl frame_LogFormat_005e5680(void* self, const char* format, ...);                          // CExoString::Format [0125]
// (declared in w5 headers) int __fastcall c2da_current_get_int_00413660(void* table, vo
// (declared in w5 headers) double __cdecl kotor_m15a_yaw_004aa0f0(const void* vector);
void* __fastcall quaternion_from_axis_angle_004ac960(void* self, void*, Vector axis, float angle);   // Quaternion(axis, angle)
U __fastcall recovered_creature_add_move_to_point_action_004f8b60(void* creature, void*, U group, const void* point, U area_id,
    U target_id, U run, U range, U a6, U a7, U a8, U a9, U a10, U a11, U a12, const void* a13, U a14, U a15, U a16); // [vm_actions_t5]
U __fastcall recovered_object_add_action_004cea20(void* self, void*, U type, U group,
    U t0, void* v0, U t1, void* v1, U t2, void* v2, U t3, void* v3, U t4, void* v4, U t5, void* v5,
    U t6, void* v6, U t7, void* v7, U t8, void* v8, U t9, void* v9, U t10, void* v10, U t11, void* v11,
    U t12, void* v12);                                                                               // CSWSObject::AddAction
// ---- SwitchPlayerCharacter callees ----
U __fastcall kotor_fastcall_004aea40(void* server, void*);                                           // CServerExoApp::GetPlayerCreatureId [0595]
void __fastcall kotor_fastcall_00563e80(void* table, void*, I npc, U clear_actions);                 // CSWPartyTable::SaveMember
void* __fastcall recovered_server_creature_construct_004f7a10(void* memory, void*, U id, U flag);    // CSWSCreature() RET 8 [0963]
void __fastcall w2x_party_mount_005638d0(void* table, void*);                                        // CSWPartyTable::AddGameInProgress
void __fastcall w2x_party_unmount_00563950(void* table, void*);                                      // CSWPartyTable::RemoveGameInProgress
U __fastcall w2x_creature_template_005026d0(void* creature, void*, void* ref, U flag);               // CSWSCreature::LoadFromTemplate RET 8
U __fastcall kotor_fastcall_00563810(void* table, void*, I npc, U fade);                             // CSWPartyTable::KillNPCObject
void* __fastcall recovered_gff_construct_004105a0(void* gff, void*);                                 // CResGFF::CResGFF
U __fastcall recovered_gff_00411260(void* gff, void*, void* root, void* type, void* version);        // CResGFF::CreateGFFFile
U __fastcall recovered_save_creature_00500610(void* creature, void*, void* gff, void* root);         // CSWSCreature::SaveCreature
U __fastcall recovered_gff_00413030(void* gff, void*, void* path, U type);                           // CResGFF::WriteGFFFile
U __fastcall recovered_npc_object_00564700(void* party, void*, I npc, U a, U b);                     // CSWPartyTable::GetNPCObject [w2_party_native]
void* __fastcall recovered_creature_client_creature_004efb20(void* creature, void*);                 // CSWSCreature::GetClientCreature [W1 b7]
void __fastcall recovered_client_object_set_fade_state_0063e220(void* object, void*, U state);       // CSWCObject::SetFadeState
void __fastcall recovered_client_object_set_desired_fade_state_0063ce60(void* object, void*, U a, U b, U c, U d); // SetDesiredFadeState
void* __fastcall recovered_creature_inventory_004ef770(void* creature, void*, U flag);               // CSWSCreature::GetItemRepository
void* __fastcall w2x_inventory_item_005560b0(void* repository, void*, U index);                      // CItemRepository::ItemListGetItem [0886]
void __fastcall recovered_item_owner_00553210(void* item, void*, U owner, U a, U b, U c);            // CSWSItem::SetPossessor [1013]
void __fastcall recovered_creature_placement_004fa100(void* creature, void*, U area, U x, U y, U z, U saved, U flag); // AddToArea [1060]
// (declared in w5 headers) void __fastcall w1x_object_set_orientation_004cd170(void* ob
void __fastcall recovered_set_game_object_00561c40(void* player, void*, void* creature);             // CSWSPlayer::SetGameObject (W2)
U __fastcall kotor_fastcall_00565560(void* table, void*, I npc);                                     // CSWPartyTable::RemoveMember
// ---- owned by sibling vmC part C (claude_vm_vmC_creature.cpp) ----
void __fastcall kotor_fastcall_00501d90(void* creature, void*);                                      // CSWSCreature::UpdatePureGoodEvilPowers
// ---- reconstructed in this file (section at the end) ----
U __fastcall kotor_fastcall_005637f0(void* table, void*, I npc, U selectable);
U __fastcall kotor_fastcall_005667c0(void* table, void*, I npc);
void __fastcall kotor_fastcall_00634a40(void* party, void*);
void __fastcall kotor_fastcall_0062d040(void* gui, void*, I planet);
void __fastcall kotor_fastcall_0062f0c0(void* gui, void*, I entry);
U __fastcall kotor_fastcall_004af070(void* server, void*);
U __fastcall kotor_fastcall_005a5490(void* stats, void*);
void __fastcall kotor_fastcall_005a5d10(void* stats, void*, U cls, U level);
void __fastcall kotor_fastcall_004c8030(void* module, void*, U strref, U title, U days, U ms);
U __fastcall kotor_fastcall_0056a260(void* message, void*, void* player, U strref, U title, U days, U ms);
void __fastcall kotor_fastcall_004c82f0(void* module, void*);
U __fastcall kotor_fastcall_0056a3e0(void* message, void*, void* player);
void __fastcall kotor_fastcall_00514700(void* creature, void*, U anchor, I pattern, I position);
}

namespace {
using namespace vmc;
using vmc_party::WorldJournalEntryArg;
inline void* party_table() { return lane_config_required::GetPartyTable_004aee70(server_app(), nullptr); }
inline void* in_game_gui() { return frame_R4_005ed690(client_app(), nullptr); }
inline void exo_ctor(ExoString* s) { (void)kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(s), nullptr); }
inline void* rules() { return kotor_cell_rules_007a3a28; }
inline void* sound() { return ptr(frame_control::sound_manager_007a39ec); }
// scalar deleting destructor: vtable slot 0, argument 1
inline void vdelete(void* object) { vcall<void*>(object, 0, 1u); }
constexpr U k_formation_table_fc = 0xfc;              // C2DA* at [[g_pRules]+B8h]+FCh
const U k_deg_to_rad_007415f8 = 0x3c8efa35u;          // .rdata 007415F8 float 0.017453292
}  // namespace

extern "C" {

// ===================================================================================================== 0053F100
// IsNPCPartyMember(int nNPC) [routine 699]
int __fastcall kotor_fastcall_0053f100(void*, void*, int, unsigned) {
    I npc = 0;
    if (!pop_int(&npc)) return k_pop_failed;                                             // 0053F101..1B
    const U member = kotor_fastcall_00563710(party_table(), nullptr, npc);               // 0053F11E..33
    return push_int(static_cast<I>(member));                                             // 0053F138..53
}

// ===================================================================================================== 00546970
// GetSelectedPlanet() [routine 744]
int __fastcall kotor_fastcall_00546970(void*, void*, int, unsigned) {
    return push_int(at<I>(party_table(), 0xe0));                                         // 00546970..9D
}

// ===================================================================================================== 00547D20
// CancelPostDialogCharacterSwitch() [routine 757]
int __fastcall kotor_fastcall_00547d20(void*, void*, int, unsigned) {
    at<U>(in_game_gui(), 0xba4) = k_invalid;                                             // 00547D20..2D
    return 0;                                                                            // 00547D37
}

// ===================================================================================================== 00547E30
// HoldWorldFadeInForDialog() [routine 760]
int __fastcall kotor_fastcall_00547e30(void*, void*, int, unsigned) {
    at<U>(in_game_gui(), 0xb98) = 1;                                                     // 00547E30..3D
    return 0;                                                                            // 00547E47
}

// ===================================================================================================== 00547E50
// SuppressStatusSummaryEntry(int nNumEntries) [routine 763].  Quirk: a pop failure returns -2000.
int __fastcall kotor_fastcall_00547e50(void*, void*, int, unsigned) {
    I entries = 0;
    if (!pop_int(&entries)) return k_push_failed;                                        // 00547E51..6B (FFFFF830h)
    kotor_fastcall_0062f0c0(in_game_gui(), nullptr, entries);                            // 00547E6E..83
    return 0;                                                                            // 00547E88
}

// ===================================================================================================== 00546A10
// GetButtonMashCheck() [routine 267]
int __fastcall kotor_fastcall_00546a10(void*, void*, int, unsigned) {
    return push_int(static_cast<I>(kotor_fastcall_004af070(server_app(), nullptr)));     // 00546A10..37
}

// ===================================================================================================== 005467B0
// ShowGalaxyMap(int nPlanet) [routine 739]
int __fastcall kotor_fastcall_005467b0(void*, void*, int, unsigned) {
    I planet = 0;
    if (!pop_int(&planet)) return k_pop_failed;                                          // 005467B1..CB
    kotor_fastcall_0062d040(in_game_gui(), nullptr, planet);                             // 005467CE..E3
    return 0;                                                                            // 005467E8
}

// ===================================================================================================== 00543300
// SetNPCSelectability(int nNPC, int nSelectability) [routine 708]
int __fastcall kotor_fastcall_00543300(void*, void*, int, unsigned) {
    I npc = 0, selectable = 0;
    if (!pop_int(&npc)) return k_pop_failed;                                             // 00543306..15
    if (!pop_int(&selectable)) return k_pop_failed;                                      // 00543317..29
    kotor_fastcall_005637f0(party_table(), nullptr, npc, static_cast<U>(selectable));    // 00543336..50
    return 0;                                                                            // 00543355
}

// ===================================================================================================== 00544910
// SwitchPlayerCharacter(int nNPC) [routine 11]
int __fastcall kotor_fastcall_00544910(void*, void*, int, unsigned) {
    I npc = 0;
    if (!pop_int(&npc)) return k_pop_failed;                                             // 00544911..2B
    const U result = kotor_fastcall_005667c0(party_table(), nullptr, npc);               // 0054492E..43
    return push_int(static_cast<I>(result));                                             // 00544948..63
}

// ===================================================================================================== 0052DE70
// AddPartyMember(int nNPC, object oCreature) [routine 574]
int __fastcall kotor_fastcall_0052de70(void*, void*, int, unsigned) {
    I npc = 0; U id = 0;
    if (!pop_int(&npc)) return k_pop_failed;                                             // 0052DE76..8F
    if (!pop_object(&id)) return k_pop_failed;                                           // 0052DE92..A4
    void* server = server_app();                                                         // 0052DEAA..B6 edi
    const U added = kotor_fastcall_00565620(party_table(), nullptr, npc, id);            // 0052DEB9..C9 ebx
    if (added != 0) {                                                                    // 0052DECB..CD
        void* client = client_app();                                                     // 0052DECF..DA esi
        const U client_id = kotor_fastcall_005eda50(client, nullptr, id);                // 0052DEDD..E0
        void* client_creature = ptr(frame_R4_005ed5b0(client, nullptr, client_id));      // 0052DEE5..E8
        void* creature = kotor_fastcall_004ae770(server, nullptr, id);                   // 0052DEED..F6
        if (client_creature != nullptr) kotor_fastcall_0060ef50(client_creature, nullptr, 1); // 0052DEFB..05
        if (creature != nullptr) recovered_set_in_party_004fdb20(creature, nullptr, 1, 1);    // 0052DF0A..15
    }
    return push_int(static_cast<I>(added));                                              // 0052DF1A..39
}

// ===================================================================================================== 005473E0
// StartCreditSequence(int bTransparentBackground) [routine 518]; an optional second string argument (argc > 1).
int __fastcall kotor_fastcall_005473e0(void*, void*, int, unsigned argc) {
    ExoString movie;
    exo_ctor(&movie);                                                                    // 005473F8..FC [esp+4]
    I transparent = 0;
    if (!pop_int(&transparent) ||                                                        // 00547401..1B
        (static_cast<I>(argc) > 1 && !pop_string(&movie))) {                             // 0054741D..36 (jle signed)
        exo_destroy(&movie);                                                             // 00547438..44
        return k_pop_failed;                                                             // 00547449
    }
    Vector color;
    if (transparent != 0) {                                                              // 0054745F..65
        color.x = flt(0xbf800000u); color.y = flt(0xbf800000u); color.z = flt(0xbf800000u); // 0054746D..7D (-1.0)
    } else {
        color.x = 0.0f; color.y = 0.0f; color.z = 0.0f;                                 // 00547499..A9
    }
    kotor_fastcall_005ee1c0(client_app(), nullptr, &color, &movie);                      // 00547485..BE
    exo_destroy(&movie);                                                                 // 005474C3..CF
    return 0;                                                                            // 005474D8
}

// ===================================================================================================== 00541A30
// RandomName() [routine 249]
int __fastcall kotor_fastcall_00541a30(void*, void*, int, unsigned) {
    alignas(4) unsigned char generator[0x24];                                            // [esp+10h]..[esp+34h]
    kotor_m15c_namegen_construct_00711290(generator, nullptr);                           // 00541A48..4C
    ExoString result;
    exo_ctor(&result);                                                                   // 00541A51..5D [esp]
    ExoString table;
    frame_LogStringCtor_005e5a90(&table, nullptr, "names");                              // 00541A62..70 [0074872C]
    kotor_m15c_namegen_load_00711340(generator, nullptr, &table);                        // 00541A75..83
    exo_destroy(&table);                                                                 // 00541A88..91
    ExoString temp;                                                                      // [esp+8] (constructed by the callee)
    void* name = kotor_m15c_namegen_random_letters_00711390(generator, nullptr, &temp, 0); // 00541A96..A1
    async_assign_string_005e5c50(&result, nullptr, name);                                // 00541AA6..B0
    exo_destroy(&temp);                                                                  // 00541AB5..BE
    kotor_m15c_namegen_unload_00711090(generator, nullptr);                              // 00541AC3..C7
    const U pushed = recovered_vm_outer_push_string_005d1090(vm(), nullptr, &result);    // 00541ACC..D7
    exo_destroy(&result);                                                                // 00541AE9 / 00541B15
    kotor_m15c_namegen_destruct_007112e0(generator, nullptr);                            // 00541AEE..FA / 00541B1A..26
    return pushed ? 0 : k_push_failed;                                                   // 00541AFF / 00541B2F
}

// ===================================================================================================== 00546230
// SetFormation(object oAnchor, object oCreature, int nFormationPattern, int nPosition) [routine 729]
int __fastcall kotor_fastcall_00546230(void*, void*, int, unsigned) {
    U anchor = k_invalid, creature_id = k_invalid;                                       // 00546239..42
    I pattern = 0, position = 0;
    if (!pop_object(&anchor)) return k_pop_failed;                                       // 00546246..52
    if (!pop_object(&creature_id)) return k_pop_failed;                                  // 00546254..66
    if (!pop_int(&pattern)) return k_pop_failed;                                         // 00546268..7A
    if (!pop_int(&position)) return k_pop_failed;                                        // 0054627C..8E
    void* creature = creature_by_id(creature_id);                                        // 0054629B..A9
    if (creature != nullptr)                                                             // 005462AE..B0
        kotor_fastcall_00514700(creature, nullptr, anchor, pattern, position);           // 005462B2..C3
    return 0;                                                                            // 005462C8
}

// ===================================================================================================== 0052DDC0
// AddMultiClass(int nClassType, object oSource) [routine 389]
int __fastcall kotor_fastcall_0052ddc0(void*, void*, int, unsigned) {
    I cls = 0; U id = 0;
    if (!pop_int(&cls)) return k_pop_failed;                                             // 0052DDC6..D5
    if (!pop_object(&id)) return k_pop_failed;                                           // 0052DDD7..E9
    void* creature = creature_by_id(id);                                                 // 0052DDF6..0A edi
    if (creature != nullptr) {                                                           // 0052DE0C..0E
        void* stats = at<void*>(creature, 0xa74);
        const U xp = at<U>(stats, 0x68);                                                 // 0052DE10..17 ebx
        const U needed = kotor_fastcall_005a5490(stats, nullptr);                        // 0052DE1B
        const U delta = needed - xp;                                                     // 0052DE26..2F esi
        kotor_fastcall_005a5d10(at<void*>(creature, 0xa74), nullptr, static_cast<U>(cls), 0); // 0052DE20..31
        if (party_table() != nullptr)                                                    // 0052DE36..46
            recovered_party_distribute_experience_005653a0(party_table(), nullptr, static_cast<I>(delta), 1); // 0052DE48..5A
    }
    return 0;                                                                            // 0052DE61
}

// ===================================================================================================== 005483F0
// AddJournalQuestEntry(string szPlotID, int nState, int bAllowOverrideHigher=FALSE) [routine 367]
int __fastcall kotor_fastcall_005483f0(void*, void*, int, unsigned argc) {
    ExoString tag;
    exo_ctor(&tag);                                                                      // 00548411..15 [esp+20h]
    I state = 0, allow_override = 0;                                                     // 0054842B allow = 0
    U xp = 0;                                                                            // 00548424 ebx
    if (!pop_string(&tag) || !pop_int(&state) ||                                         // 0054841A..4A
        (static_cast<I>(argc) >= 3 && !pop_int(&allow_override))) {                      // 0054844C..64 (jl signed)
        exo_destroy(&tag);                                                               // 00548466..72
        return k_pop_failed;                                                             // 00548477
    }
    if (state < 0) state = static_cast<I>(0u - static_cast<U>(state));                   // 00548490..9D cdq/xor/sub
    U days = 0, ms = 0;                                                                  // [esp+18h] / [esp+1Ch]
    frame_GetWorldTime_004ade40(kotor_fastcall_004aede0(server_app(), nullptr), nullptr, &days, &ms); // 005484A1..BA
    void* players = recovered_server_player_list_004aed90(server_app(), nullptr);        // 005484BF..CD edi
    if (players == nullptr) {                                                            // 005484CF..D1
        exo_destroy(&tag);                                                               // 005484D3..DF
        return 1;                                                                        // 005484E4 (quirk)
    }
    void* pos = at<void*>(at<void*>(players, 0), 0);                                     // 005484FD..03
    while (pos != nullptr) {                                                             // 00548507 / 005485DF
        void* player = unresolved_list_at_pos_005e9b60(at<void*>(players, 0), nullptr, pos); // 00548510..13
        if (player != nullptr && kotor_fastcall_00561080(player, nullptr) != nullptr) {   // 0054851A..2B
            void* creature = as_creature(kotor_fastcall_00561080(player, nullptr));      // 00548531..3C
            if (creature != nullptr &&                                                   // 0054853F..43
                (allow_override != 0 ||                                                  // 00548549..4F
                 kotor_fastcall_005c58a0(kotor_fastcall_004edb00(creature, nullptr), nullptr, &tag) < state)) { // 00548551..68 (jge)
                kotor_fastcall_005c63f0(kotor_fastcall_004edb00(creature, nullptr), nullptr, &tag, days, 0); // 0054856A..7F
                kotor_fastcall_005c6650(kotor_fastcall_004edb00(creature, nullptr), nullptr, &tag, ms, 0);   // 00548584..99
                kotor_fastcall_005c5a40(kotor_fastcall_004edb00(creature, nullptr), nullptr, &tag,
                                        static_cast<U>(state), 0);                       // 0054859E..B3
                xp = static_cast<U>(kotor_fastcall_005c5850(kotor_fastcall_004edb00(creature, nullptr), nullptr, &tag)); // 005485B8..CB
            }
        }
        (void)unresolved_list_next_005e9b90(at<void*>(players, 0), nullptr, &pos);       // 005485CD..D4
    }
    if (xp != 0 && party_table() != nullptr)                                             // 005485E5..F8
        recovered_party_distribute_experience_005653a0(party_table(), nullptr, static_cast<I>(xp), 1); // 005485FA..0D
    exo_destroy(&tag);                                                                   // 00548612..1E
    return 0;                                                                            // 00548629
}

// ===================================================================================================== 005487F0
// AddJournalWorldEntryStrref(int strref, int strrefTitle) [routine 670]
int __fastcall kotor_fastcall_005487f0(void*, void*, int, unsigned) {
    void* message = recovered_server_sws_message_004aedb0(server_app(), nullptr);        // 005487F0..0B ebx
    void* module = server_get_module_004ae6b0(server_app(), nullptr);                    // 00548802..1D esi
    I strref = 0, title = 0;                                                             // [esp+14h] / [esp+10h]
    if (!pop_int(&strref)) return k_pop_failed;                                          // 00548812..26
    if (!pop_int(&title)) return k_pop_failed;                                           // 00548828..3A
    U days = 0, ms = 0;                                                                  // [esp+0Ch] / [esp+8]
    frame_GetWorldTime_004ade40(kotor_fastcall_004aede0(server_app(), nullptr), nullptr, &days, &ms); // 00548849..63
    if (module != nullptr)                                                               // 00548868..6A
        kotor_fastcall_004c8030(module, nullptr, static_cast<U>(strref), static_cast<U>(title), days, ms); // 0054886C..82
    void* players = recovered_server_player_list_004aed90(server_app(), nullptr);        // 00548887..96 edi
    if (players != nullptr) {                                                            // 00548898..9A
        void* pos = at<void*>(at<void*>(players, 0), 0);                                 // 0054889C..A2
        while (pos != nullptr) {                                                         // 005488A6 / 005488F3
            void* player = unresolved_list_at_pos_005e9b60(at<void*>(players, 0), nullptr, pos); // 005488A8..AB
            if (player != nullptr && kotor_fastcall_00561080(player, nullptr) != nullptr && message != nullptr) // 005488B2..C3
                (void)kotor_fastcall_0056a260(message, nullptr, player, static_cast<U>(strref),
                                              static_cast<U>(title), days, ms);           // 005488C5..DC
            (void)unresolved_list_next_005e9b90(at<void*>(players, 0), nullptr, &pos);   // 005488E1..E8
        }
    }
    return 0;                                                                            // 005488F7
}

// ===================================================================================================== 00548A80
// DeleteJournalWorldAllEntries() [routine 672]
int __fastcall kotor_fastcall_00548a80(void*, void*, int, unsigned) {
    void* message = recovered_server_sws_message_004aedb0(server_app(), nullptr);        // 00548A81..99 ebx
    void* module = server_get_module_004ae6b0(server_app(), nullptr);                    // 00548A90..9B
    if (module != nullptr) kotor_fastcall_004c82f0(module, nullptr);                     // 00548AA0..A6
    void* players = recovered_server_player_list_004aed90(server_app(), nullptr);        // 00548AAB..B4 edi
    if (players != nullptr) {                                                            // 00548AB9..BD
        void* pos = at<void*>(at<void*>(players, 0), 0);                                 // 00548ABF..C5
        while (pos != nullptr) {                                                         // 00548AC9 / 00548B07
            void* player = unresolved_list_at_pos_005e9b60(at<void*>(players, 0), nullptr, pos); // 00548AD0..D3
            if (player != nullptr && kotor_fastcall_00561080(player, nullptr) != nullptr && message != nullptr) // 00548ADA..EB
                (void)kotor_fastcall_0056a3e0(message, nullptr, player);                 // 00548AED..F0
            (void)unresolved_list_next_005e9b90(at<void*>(players, 0), nullptr, &pos);   // 00548AF5..FC
        }
    }
    return 0;                                                                            // 00548B0B
}

// =====================================================================================================
// Callees reconstructed here (no provider in the c31 candidate)
// =====================================================================================================

// 005637F0 CSWPartyTable::SetNPCSelectability(int npc, int selectable) RET 8
U __fastcall kotor_fastcall_005637f0(void* table, void*, I npc, U selectable) {
    if (npc >= 0 && npc < 9 && at<U>(table, 0x30 + static_cast<U>(npc) * 4) != 0)       // 005637F0..03
        at<U8>(table, 0x54 + static_cast<U>(npc)) = static_cast<U8>(selectable);         // 00563805..09
    return static_cast<U>(npc);                                                          // EAX = npc on every exit
}

// 0062F0C0 CGuiInGame::SuppressStatusSummary(int entries) RET 4
void __fastcall kotor_fastcall_0062f0c0(void* gui, void*, I entries) {
    void* summary = at<void*>(gui, 0xa8);                                                // 0062F0C0
    if (summary != nullptr && entries >= 0) at<I>(summary, 0x78) = entries;              // 0062F0C6..D2
}

// 004AF070 CServerExoApp::GetButtonMashCheck() (plain RET)
U __fastcall kotor_fastcall_004af070(void* server, void*) {
    return at<U>(at<void*>(server, 4), 0x100d8);                                         // 004AF070..73
}

// 005A5490 CSWSCreatureStats::CalculateExpNeededForNextLevel() (plain RET): first table entry above the current XP
U __fastcall kotor_fastcall_005a5490(void* stats, void*) {
    const U xp = at<U>(stats, 0x68);                                                     // 005A5490
    void* table = rules();                                                               // 005A5493
    for (U i = 0; static_cast<I>(i) <= 0x14; ++i) {                                      // 005A549A..AD (movzx al; jle)
        const U threshold = at<U>(table, 0x38 + (i & 0xffu) * 4);
        if (xp < threshold) return threshold;                                            // 005A54A3..A7 (jb) / 005A54B3..BA
    }
    return 0;                                                                            // 005A54AF
}

// 005A5D10 CSWSCreatureStats::AddClass(BYTE class, BYTE level) RET 8
void __fastcall kotor_fastcall_005a5d10(void* stats, void*, U cls, U level) {
    if (at<U8>(stats, 0x89) < 2) at<U8>(stats, 0x89) = static_cast<U8>(at<U8>(stats, 0x89) + 1); // 005A5D10..1C
    U8 count = at<U8>(stats, 0x89);                                                      // 005A5D22
    U8 last = static_cast<U8>(count - 1);
    if (last < count) at<U8>(stats, 0xa7 + static_cast<U>(last) * 0x28) = static_cast<U8>(cls);   // 005A5D28..3A
    count = at<U8>(stats, 0x89);                                                         // 005A5D41
    last = static_cast<U8>(count - 1);
    if (last < count) at<U8>(stats, 0xa8 + static_cast<U>(last) * 0x28) = static_cast<U8>(level); // 005A5D47..59
    void* creature = at<void*>(stats, 0x24);                                             // 005A5D60
    if (creature != nullptr) kotor_fastcall_00501d90(creature, nullptr);                 // 005A5D63..67
}

// 00634A40 CSWParty::ClearParty() (plain RET)
void __fastcall kotor_fastcall_00634a40(void* party, void*) {
    for (I i = 0; i < at<I>(party, 0); ++i)                                              // 00634A40..61 (count re-read)
        at<U>(party, 0x24 + static_cast<U>(i) * 0x88) = k_invalid;                       // 00634A50
    at<U>(party, 0) = 0;                                                                 // 00634A64
}

// 0056A260 CSWSMessage::SendServerToPlayerJournalAddWorldStrref(CSWSPlayer*, strref, title, days, ms) RET 14h
U __fastcall kotor_fastcall_0056a260(void* message, void*, void* player, U strref, U title, U days, U ms) {
    U size = 0x10;                                                                       // 0056A26C [esp+4]
    void* data = nullptr;                                                                // [esp+8]
    kotor_fastcall_004d68a0(message, nullptr, 0x10, 0xffffffffu, 1);                     // 0056A264..74
    kotor_fastcall_004d6340(message, nullptr, strref, 0x20);                             // 0056A279..82
    kotor_fastcall_004d6340(message, nullptr, title, 0x20);                              // 0056A287..90
    kotor_fastcall_004d6340(message, nullptr, days, 0x20);                               // 0056A295..9E
    kotor_fastcall_004d6340(message, nullptr, ms, 0x20);                                 // 0056A2A3..AC
    if (kotor_fastcall_004d6440(message, nullptr, &data, &size) == 0) return 0;          // 0056A2B1..CA
    return kotor_fastcall_00567e60(message, nullptr, at<U>(player, 4), 0x1c, 2, data, size); // 0056A2CD..E5
}

// 0056A3E0 CSWSMessage::SendServerToPlayerJournalDeleteWorldAll(CSWSPlayer*) RET 4
U __fastcall kotor_fastcall_0056a3e0(void* message, void*, void* player) {
    U size = 1;                                                                          // 0056A3EC [esp+4]
    void* data = nullptr;                                                                // [esp+8]
    kotor_fastcall_004d68a0(message, nullptr, 1, 0xffffffffu, 1);                        // 0056A3E4..F4
    kotor_fastcall_004d62c0(message, nullptr, 1, 8);                                     // 0056A3F9..FF
    if (kotor_fastcall_004d6440(message, nullptr, &data, &size) == 0) return 0;          // 0056A404..1D
    return kotor_fastcall_00567e60(message, nullptr, at<U>(player, 4), 0x1c, 5, data, size); // 0056A420..38
}

// 004C8030 CSWSModule::AddWorldJournalEntryStrref(strref, title, days, ms) RET 10h
void __fastcall kotor_fastcall_004c8030(void* module, void*, U strref, U title, U days, U ms) {
    void* message = recovered_server_sws_message_004aedb0(server_app(), nullptr);        // 004C8048..5F edi
    // Local CWorldJournalEntry at [esp+14h]: +0 / +8 CExoStrings, +10h days, +14h ms, +18h title, +1Ch strref,
    // +20h flags (bit 0 set, upper bits indeterminate stack contents in the original, as here).
    alignas(4) unsigned char entry[0x24];
    exo_ctor(reinterpret_cast<ExoString*>(entry));                                       // 004C805B..61
    exo_ctor(reinterpret_cast<ExoString*>(entry + 8));                                   // 004C8066..72
    at<U>(entry, 0x18) = title;                                                          // 004C8077 / 004C8083
    at<U>(entry, 0x14) = ms;                                                             // 004C8087..8B
    U bits;
    std::memcpy(&bits, entry + 0x20, 4);                                                 // 004C808F (uninitialised local)
    at<U>(entry, 0x10) = days;                                                           // 004C807B / 004C809D
    at<U>(entry, 0x20) = bits | 1u;                                                      // 004C8093..A1
    at<U>(entry, 0x1c) = strref;                                                         // 004C807F / 004C80B4
    WorldJournalEntryArg copy;
    (void)kotor_fastcall_004c4080(&copy, nullptr, entry);                                // 004C809A..B8 copy into the arg area
    kotor_fastcall_004c65e0(&at<U>(module, 0x1cc), nullptr, copy);                       // 004C80BD..C3 (callee destroys it)
    if (message != nullptr) {                                                            // 004C80C8..CA
        void* players = recovered_server_player_list_004aed90(server_app(), nullptr);    // 004C80CC..D9 ebx
        if (players != nullptr) {                                                        // 004C80DB..DD
            void* pos = at<void*>(at<void*>(players, 0), 0);                             // 004C80DF..E5
            while (pos != nullptr) {                                                     // 004C80E9 / 004C8133
                void* player = unresolved_list_at_pos_005e9b60(at<void*>(players, 0), nullptr, pos); // 004C80F0..F3
                if (player != nullptr && kotor_fastcall_00561080(player, nullptr) != nullptr) // 004C80FA..07
                    (void)kotor_fastcall_0056a260(message, nullptr, player, strref, title, days, ms); // 004C8109..1C
                (void)unresolved_list_next_005e9b90(at<void*>(players, 0), nullptr, &pos); // 004C8121..28
            }
        }
    }
    exo_destroy(reinterpret_cast<ExoString*>(entry + 8));                                // 004C8135..41
    exo_destroy(reinterpret_cast<ExoString*>(entry));                                    // 004C8146..52
}

// 004C82F0 CSWSModule::DeleteWorldJournalAllEntries() (plain RET)
void __fastcall kotor_fastcall_004c82f0(void* module, void*) {
    void* message = recovered_server_sws_message_004aedb0(server_app(), nullptr);        // 004C82F1..03 ebp
    unsigned char* data = at<unsigned char*>(module, 0x1cc);                             // 004C8305
    if (data != nullptr) {                                                               // 004C830B..0D
        c2da_destroy_array_006fa852(data, 0x24, at<int>(data - 4, 0),
                                    reinterpret_cast<void*>(&world_journal_entry_destroy_004c4030)); // 004C830F..1E
        live_free(data - 4);                                                             // 004C8323..29 delete[]
        at<U>(module, 0x1cc) = 0;                                                        // 004C832C
        at<U>(module, 0x1d4) = 0;                                                        // 004C8336
    }
    at<U>(module, 0x1d0) = 0;                                                            // 004C8340
    void* players = recovered_server_player_list_004aed90(server_app(), nullptr);        // 004C834A..58 edi
    if (players == nullptr) return;                                                      // 004C835A..5C
    void* pos = at<void*>(at<void*>(players, 0), 0);                                     // 004C835E..64
    while (pos != nullptr) {                                                             // 004C8368 / 004C83A7
        void* player = unresolved_list_at_pos_005e9b60(at<void*>(players, 0), nullptr, pos); // 004C8370..73
        if (player != nullptr && kotor_fastcall_00561080(player, nullptr) != nullptr && message != nullptr) // 004C837A..8B
            (void)kotor_fastcall_0056a3e0(message, nullptr, player);                     // 004C838D..90
        (void)unresolved_list_next_005e9b90(at<void*>(players, 0), nullptr, &pos);       // 004C8395..9C
    }
}

// 0062D040 CGuiInGame::ShowGalaxyMapGui(int planet) RET 4
void __fastcall kotor_fastcall_0062d040(void* gui, void*, I planet) {
    if (at<U>(gui, 0x108) == 0) return;                                                  // 0062D05B..63
    if (at<U>(gui, 0x30) != 0) return;                                                   // 0062D069..6E
    if (at<U>(in_game_gui(), 0xb4) != 0) return;                                         // 0062D074..89
    if (kotor_fastcall_005edc10(client_app(), nullptr) == 0)                             // 0062D08F..9F
        (void)kotor_fastcall_005edc20(client_app(), nullptr, 1, 0, 0);                   // 0062D0A1..AE
    kotor_fastcall_005edb70(client_app(), nullptr, nullptr, 0);                          // 0062D0B3..BF
    frame_R4_005d5e80(sound(), nullptr, 4);                                              // 0062D0C4..D1 [0074C5FC] = 4
    frame_R4_0062aa00(gui, nullptr, 3, 1);                                               // 0062D0D6..DC
    menu_resources::Ref sound_ref;
    sound_ref.construct_00406d80("");                                                    // 0062D0E1..EA [0073D71C]
    ExoString text;
    frame_LogStringCtor_005e5a90(&text, nullptr, "");                                    // 0062D0EF..F8 [0073D71C]
    object_projectile_bark_006a9920(at<void*>(gui, 0x4c), nullptr, k_invalid, &text, &sound_ref, 0); // 0062D0FD..19
    exo_destroy(&text);                                                                  // 0062D11E..2A
    (void)kotor_fastcall_005eda60(client_app(), nullptr, 2, 1);                                // 0062D12F..3C
    if (at<void*>(gui, 0x80) == nullptr) {                                               // 0062D141..49
        void* memory = live_operator_new(0x2550);                                        // 0062D14B..55
        void* panel = memory ? kotor_fastcall_00695180(memory, nullptr, at<void*>(gui, 0x38)) : nullptr; // 0062D15C..75
        at<void*>(gui, 0x80) = panel;                                                    // 0062D17F
    }
    kotor_fastcall_00563b60(party_table(), nullptr, planet);                             // 0062D185..99
    kotor_fastcall_0040bc70(at<void*>(gui, 0x38), nullptr, at<void*>(gui, 0x80), 2, 1);  // 0062D19E..AC
    kotor_fastcall_0040a140(at<void*>(gui, 0x38), nullptr, 4);                           // 0062D1B1..B6
}

// 00514700 CSWSCreature::SetFormation(OBJECT_ID anchor, int pattern, int position) RET 0Ch
void __fastcall kotor_fastcall_00514700(void* creature, void*, U anchor_id, I pattern, I position) {
    void* anchor = kotor_fastcall_004ae750(server_app(), nullptr, anchor_id);            // 00514712..2F esi
    if (anchor == nullptr) return;                                                       // 00514731..33
    ExoString angle_column, distance_column;                                             // [esp+24h] / [esp+1Ch]
    exo_ctor(&angle_column);                                                             // 00514739..3D
    frame_LogFormat_005e5680(&angle_column, "ANGLE_%d", position);                       // 00514742..5E [00747C60]
    exo_ctor(&distance_column);                                                          // 00514761..65
    frame_LogFormat_005e5680(&distance_column, "DISTANCE_%d", position);                 // 0051476A..7A [00747C54]
    void* table = at<void*>(at<void*>(rules(), 0xb8), k_formation_table_fc);             // 0051477F..8B
    I angle = 0, distance = 0;                                                           // [esp+0Ch] / [esp+70h] (anchor slot)
    if (c2da_current_get_int_00413660(table, nullptr, static_cast<U>(pattern), &angle_column, &angle) != 0 && // 00514791..AF
        c2da_current_get_int_00413660(at<void*>(at<void*>(rules(), 0xb8), k_formation_table_fc), nullptr,
                                      static_cast<U>(pattern), &distance_column, &distance) != 0) {       // 005147B5..DA
        const Vector offset = {static_cast<float>(distance), 0.0f, 0.0f};                // 005147E0..F3 fild/fstp, 0, 0
        const Vector axis = {0.0f, 0.0f, 1.0f};                                          // 00514817..27
        const Vector* orientation = &at<Vector>(anchor, 0x9c);                                  // 005147E4 edi
        const double yaw = kotor_m15a_yaw_004aa0f0(orientation);                         // 005147EA..2F (ST0)
        const float radians = static_cast<float>((yaw + static_cast<double>(angle)) *
                                                 static_cast<double>(flt(k_deg_to_rad_007415f8))); // 00514834..4B
        alignas(4) unsigned char quaternion[0x10];                                       // [esp+50h]
        quaternion_from_axis_angle_004ac960(quaternion, nullptr, axis, radians);         // 00514838..5F
        Vector rotated;                                                                  // [esp+2Ch] (overwrites the axis)
        const Vector* r = quaternion_rotate_004a9bb0(&rotated, quaternion, &offset);     // 00514864..73
        const double z = static_cast<double>(at<float>(anchor, 0x98)) + static_cast<double>(r->z); // 00514878..7E
        const double y = static_cast<double>(at<float>(anchor, 0x94)) + static_cast<double>(r->y); // 00514884..8C
        const double x = static_cast<double>(r->x) + static_cast<double>(at<float>(anchor, 0x90)); // 00514891..95
        Vector target;                                                                   // [esp+38h]
        target.x = static_cast<float>(x);                                                // 0051489B..A3
        target.y = static_cast<float>(y);                                                // 005148A7..B1 (fst: st0 kept)
        target.z = static_cast<float>(z);                                                // 005148AF..C1
        float face_x = static_cast<float>(static_cast<double>(target.x) +
                                          static_cast<double>(at<float>(anchor, 0x9c))); // 005148BD..D0 [esp+18h]
        float face_y = static_cast<float>(y + static_cast<double>(at<float>(anchor, 0xa0))); // 005148D6..E4 [esp+14h]
        float face_z = static_cast<float>(z + static_cast<double>(at<float>(anchor, 0xa4))); // 005148EA..FB [esp+10h]
        const Vector zero = {0.0f, 0.0f, 0.0f};                                          // 00514907..17 [esp+2Ch]
        (void)recovered_creature_add_move_to_point_action_004f8b60(creature, nullptr, 0xffff, &target,
            at<U>(anchor, 0x8c), k_invalid, 1, 0, 0, 0, 0, 0, 0, 0, 0, &zero, 0, 0, 0); // 005148C7..26
        (void)recovered_object_add_action_004cea20(creature, nullptr, 0x31, 0xfffe,
            2, &face_x, 2, &face_y, 2, &face_z, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr,
            0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr);                // 0051492B..71
    }
    exo_destroy(&distance_column);                                                       // 00514976..7F
    exo_destroy(&angle_column);                                                          // 00514984..90
}

// 005667C0 CSWPartyTable::SwitchPlayerCharacter(int npc) RET 4
U __fastcall kotor_fastcall_005667c0(void* table, void*, I npc) {
    ExoString path;
    exo_ctor(&path);                                                                     // 005667DE..E2 [esp+30h]
    void* client = client_app();                                                         // 005667E7..F2 [esp+20h]
    void* server = server_app();                                                         // 005667EF ebp
    void* head = at<void*>(at<void*>(recovered_server_player_list_004aed90(server, nullptr), 0), 0); // 005667F8..05
    void* player = head ? at<void*>(head, 8) : nullptr;                                  // 00566807..14 [esp+10h]
    void* old_creature = kotor_fastcall_004ae770(server, nullptr, at<U>(player, 0x38));  // 00566818..2B ebx (quirk: unchecked)
    void* area = recovered_object_area_004cb120(old_creature, nullptr);                  // 00566829..3D [esp+24h]
    U add_to_area = 1;                                                                   // 00566841 [esp+14h]
    U position[3] = {0, 0, 0};                                                           // 00566849..59 [esp+48h]
    U orientation[3] = {0, 0, 0};                                                        // 00566861..71 [esp+54h]
    I next_current = 0;                                                                  // [esp+18h]
    void* creature = nullptr;                                                            // edi
    const I current = at<I>(table, 0xf0);
    if (npc == -1) {                                                                     // 00566838..79
        if (current == -1) { exo_destroy(&path); return 1; }                             // 0056687F..85 -> 005669A7
        void* pc = kotor_fastcall_004ae770(server, nullptr, kotor_fastcall_004aea40(server, nullptr)); // 0056688B..95
        at<U>(at<void*>(pc, 0xa74), 0x6c) = 0;                                           // 0056689A..A2
        at<U>(pc, 0x9d4) = 0;                                                            // 005668A5
        kotor_fastcall_00563e80(table, nullptr, at<I>(table, 0xf0), 0);                  // 005668AB..B5
        void* memory = live_operator_new(0xac8);                                         // 005668BA..C4
        if (memory != nullptr) creature = recovered_server_creature_construct_004f7a10(memory, nullptr, k_invalid, 0); // 005668C7..E1
        w2x_party_mount_005638d0(table, nullptr);                                        // 005668E3..EA
        menu_resources::Ref ref;
        ref.construct_00406d80("PC");                                                    // 005668EF..F8 [00749354]
        if (w2x_creature_template_005026d0(creature, nullptr, &ref, 0) == 0) {           // 005668FD..0D (creature unchecked)
            if (creature != nullptr) vdelete(creature);                                  // 0056690F..19
            exo_destroy(&path);                                                          // 00566D58..5C
            return 0;
        }
        w2x_party_unmount_00563950(table, nullptr);                                      // 00566928..2A
        std::memcpy(position, &at<U>(old_creature, 0x90), 12);                           // 0056692F..5A
        std::memcpy(orientation, &at<U>(old_creature, 0x9c), 12);                        // 0056693D..67
        (void)kotor_fastcall_00563810(table, nullptr, at<I>(table, 0xf0), 1);            // 0056694D..6E
        next_current = -1;                                                               // 00566973
    } else {
        if (npc < 0 || npc >= 9 || at<U>(table, 0x30 + static_cast<U>(npc) * 4) == 0) {  // 00566980..97
            exo_destroy(&path); return 0;                                                // 00566D54..61
        }
        if (current == npc) { exo_destroy(&path); return 1; }                            // 0056699D..B9
        if (current == -1) {                                                             // 005669BE..C0
            (void)GuiAssign005e5140(&path, nullptr, "GAMEINPROGRESS:PC");                // 005669C6..CF [00749340]
            void* memory = live_operator_new(0xa0);                                      // 005669D4..DE
            void* gff = memory ? recovered_gff_construct_004105a0(memory, nullptr) : nullptr; // 005669E1..F9 edi
            ExoString version, type;                                                     // [esp+38h] / [esp+28h]
            frame_LogStringCtor_005e5a90(&version, nullptr, "V2.0");                     // 005669FB..09 [00745338]
            frame_LogStringCtor_005e5a90(&type, nullptr, "UTC ");                        // 00566A0E..1C [00747368]
            U root = 0;                                                                  // [esp+18h] CResStruct (4 bytes)
            (void)recovered_gff_00411260(gff, nullptr, &root, &type, &version);          // 00566A21..37
            exo_destroy(&type);                                                          // 00566A3C..45
            exo_destroy(&version);                                                       // 00566A4A..53
            (void)recovered_save_creature_00500610(old_creature, nullptr, gff, &root);   // 00566A58..60
            ExoString file;
            frame_LogStringCtor_005e5a90(&file, nullptr, WindowCStr005e5670(&path, nullptr)); // 00566A65..73
            const U written = recovered_gff_00413030(gff, nullptr, &file, 0x7eb);        // 00566A78..89
            exo_destroy(&file);                                                          // 00566A94..9F
            if (written == 0) { exo_destroy(&path); return 0; }                          // 00566A8E..B0 (gff leaked)
            // 00566AE6..EA: ebx / edi reloaded (old creature, npc); the CResGFF is leaked (quirk)
        } else {
            void* pc = kotor_fastcall_004ae770(server, nullptr, kotor_fastcall_004aea40(server, nullptr)); // 00566AB5..BF
            at<U>(at<void*>(pc, 0xa74), 0x6c) = 0;                                       // 00566AC4..CC
            at<U>(pc, 0x9d4) = 0;                                                        // 00566AD0
            kotor_fastcall_00563e80(table, nullptr, at<I>(table, 0xf0), 0);              // 00566AD6..DF
        }
        U id = k_invalid;                                                                // 00566B02
        if (at<U>(table, 0x30 + static_cast<U>(npc) * 4) != 0 &&                         // 00566AEE..F4
            at<U>(table, 0xc + static_cast<U>(npc) * 4) != k_invalid)                    // 00566AF6..00
            id = at<U>(table, 0xc + static_cast<U>(npc) * 4);
        creature = kotor_fastcall_004ae770(server, nullptr, id);                         // 00566B07..0F
        if (creature == nullptr) {                                                       // 00566B11..13
            const U npc_id = recovered_npc_object_00564700(table, nullptr, npc, 1, 1);   // 00566B15..20
            creature = kotor_fastcall_004ae770(server, nullptr, npc_id);                 // 00566B25..28
            std::memcpy(position, &at<U>(old_creature, 0x90), 12);                       // 00566B2D..50
            std::memcpy(orientation, &at<U>(old_creature, 0x9c), 12);                    // 00566B41..5F
        } else {
            add_to_area = 0;                                                             // 00566B65
        }
        if (at<I>(table, 0xf0) == -1) {                                                  // 00566B6D..76
            void* client_creature = recovered_creature_client_creature_004efb20(old_creature, nullptr); // 00566B78..7F
            if (client_creature != nullptr) {                                            // 00566B81..83
                recovered_client_object_set_fade_state_0063e220(client_creature, nullptr, 0);         // 00566B85..89
                recovered_client_object_set_desired_fade_state_0063ce60(client_creature, nullptr, 0, 1, 0, 0); // 00566B8E..98
                at<U>(client_creature, 0xe4) = 1;                                        // 00566B9D
            }
            if (old_creature != nullptr) vdelete(old_creature);                          // 00566BA7..B1
        } else {
            (void)kotor_fastcall_00563810(table, nullptr, at<I>(table, 0xf0), 1);        // 00566BB5..BA
        }
        next_current = npc;                                                              // 00566BBF..C3
    }
    if (creature == nullptr) { exo_destroy(&path); return 0; }                           // 00566BC7..C9 -> 00566D51
    kotor_fastcall_00634a40(kotor_fastcall_005ed8b0(client, nullptr), nullptr);          // 00566BCF..DA
    recovered_set_in_party_004fdb20(creature, nullptr, 1, 1);                            // 00566BDF..E8
    at<U>(at<void*>(creature, 0xa74), 0x6c) = 1;                                         // 00566BED..F3
    at<U>(creature, 0x9d4) = 1;                                                          // 00566BF9
    void* repository = recovered_creature_inventory_004ef770(creature, nullptr, 1);      // 00566BF6..FF ebp
    if (repository != nullptr) {                                                         // 00566C04..08
        for (U i = 0; i < at<U>(repository, 0x10); ++i) {                                // 00566C0A..3D (jb)
            void* item = w2x_inventory_item_005560b0(at<void*>(table, 0x118), nullptr, i); // 00566C13..1A (quirk: [esi+118h])
            if (item != nullptr)                                                         // 00566C1F..21
                recovered_item_owner_00553210(item, nullptr, at<U>(creature, 4), 0, 0, k_invalid); // 00566C23..32
        }
    }
    if (add_to_area != 0) {                                                              // 00566C3F..45
        recovered_creature_placement_004fa100(creature, nullptr, word(area), position[0], position[1], position[2], 0, 0); // 00566C47..61
        w1x_object_set_orientation_004cd170(creature, nullptr, orientation);             // 00566C66..6D
    }
    at<U>(player, 0x38) = at<U>(creature, 4);                                            // 00566C72..7A
    recovered_set_game_object_00561c40(player, nullptr, creature);                       // 00566C75..7D
    I index = at<I>(table, 0) - 1;                                                       // 00566C82..84
    if (index >= 0) {                                                                    // 00566C85 (js)
        I remaining = index + 1;                                                         // 00566C8F..90 [esp+70h]
        U slot = 4 + static_cast<U>(index) * 4;                                          // 00566C8B ebx
        do {
            const I member = at<I>(table, slot);                                         // 00566C94
            (void)kotor_fastcall_00565560(table, nullptr, member);                       // 00566C96..99
            if (member >= 0 && member < 9) {                                             // 00566C9E..A9
                bool still_member = false;
                for (I k = 0; k < at<I>(table, 0); ++k)                                  // 00566CAB..C2
                    if (member == at<I>(table, 4 + static_cast<U>(k) * 4)) { still_member = true; break; } // 00566CB6..B8
                if (!still_member) {
                    const U member_id = at<U>(table, 0xc + static_cast<U>(member) * 4);  // 00566CC4
                    void* member_creature = kotor_fastcall_004ae770(server_app(), nullptr, member_id); // 00566CC8..D2
                    at<U>(table, 0xc + static_cast<U>(member) * 4) = k_invalid;          // 00566CDB
                    if (member_creature != nullptr) {                                    // 00566CD9..E3
                        void* client_creature = recovered_creature_client_creature_004efb20(member_creature, nullptr); // 00566CE5..E7
                        if (client_creature != nullptr) {                                // 00566CEE..F0
                            recovered_client_object_set_fade_state_0063e220(client_creature, nullptr, 0);         // 00566CF2..F6
                            recovered_client_object_set_desired_fade_state_0063ce60(client_creature, nullptr, 0, 1, 0, 0); // 00566CFB..05
                            at<U>(client_creature, 0xe4) = 1;                            // 00566D0A
                        }
                        vdelete(member_creature);                                        // 00566D14..1B
                    }
                }
            }
            slot -= 4;                                                                   // 00566D21
        } while (--remaining != 0);                                                      // 00566D1D..29
    }
    at<I>(table, 0xf0) = next_current;                                                   // 00566D2F..37
    exo_destroy(&path);                                                                  // 00566D45
    return 1;                                                                            // 00566D4A
}

}  // extern "C"
