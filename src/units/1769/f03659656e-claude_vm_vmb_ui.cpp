// Lane vmB, file "ui": client/GUI, gold, item, store, journal and message NWScript engine-command handlers of
// package vmB (CSWVirtualMachineCommands::ExecuteCommand*), reconstructed from the original swkotor.exe (read-only,
// capstone).  Every handler: original int __thiscall (int routine, int argc), RET 8 (verified) ->
//   extern "C" int __fastcall kotor_fastcall_<addr>(self, edx, routine, argc) @16.
// Addresses in comments are original instruction ranges.  Conventions: claude_vm_vmB_common.hpp.
// .rdata literals: [00746644] 7F000000h OBJECT_INVALID, [00746634] byte 5 = object type creature,
// [007441DC] 0Ah = AI event queue type, [007471C0] "g_i_credits001", [0073D71C] "", [0074C5FC] 4 = sound mode.
// Not implemented here: 00543990 ShowUpgradeScreen (needs CGuiInGame::ShowUpgradeScreen 0062E760 and the whole
// CSWGuiUpgradeSelection panel class 006C78D0 + CSWGuiUpgradeItemSelect; see report).
#include "54db204e3b-claude_vm_vmb_common.hpp"

namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); }  // CExoString() [menu20_bindings]
namespace menu_resources { struct Ref { unsigned char bytes[16]; Ref* __thiscall construct_00406d80(const char* text); }; } // CResRef(char*) [0030]
namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void* server, void*); }       // CServerExoApp::GetPartyTable [0575]
namespace options_video { struct Client; struct String; }
namespace client_intro {                                                                                // CClientExoAppInternal::AddMovieToMovieQueue [0371]
int __fastcall AddMovieToMovieQueue_005fbf50(options_video::Client* self, void*, options_video::String* name, int flag);
}

extern "C" {
// ---- existing providers (c31 map; object in brackets) ----
void* __fastcall frame_R4_005ed690(void* client, void*);                                   // CClientExoApp::GetInGameGui [0457]
void __fastcall kotor_fastcall_0062eeb0(void* gui, void*, U kind, U value);                // CGuiInGame::UpdateStatus RET 8 [src_hud_main]
void __fastcall recovered_creature_add_gold_004f3db0(void* creature, void*, U amount, U feedback); // CSWSCreature::AddGold RET 8 [creature_add_gold]
void __fastcall kotor_fastcall_004f3ed0(void* creature, void*, U amount, U feedback);     // CSWSCreature::RemoveGold RET 8 [claude_w5_creature_actions]
unsigned __fastcall w2x_creature_gold_004edd60(void* creature, void*);                    // CSWSCreature::GetGold [0886]
void* __fastcall recovered_item_construct_005530a0(void* self, void*, U id);              // CSWSItem::CSWSItem RET 4 [0963]
U __fastcall recovered_item_template_005608b0(void* item, void*, void* resref);           // CSWSItem::LoadFromTemplate RET 4 [0995]
U __fastcall recovered_placeable_acquire_item_00584b10(void* placeable, void*, void* item_cell, U container, U flag); // AcquireItem RET 0Ch
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* array, void*, int id); // CSWBaseItemArray::GetBaseItem [0820]
U __fastcall recovered_inventory_insert_0055d330(void* repository, void*, void** item, U a, U b, U c); // CItemRepository::AddItem RET 10h [1088]
extern void* kotor_cell_rules_007a3a28;                                                    // [007A3A28] CSWRules* [0421]
extern void* kotor_cell_sound_007a39ec;                                                    // [007A39EC] CExoSound* [0673]
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);              // CServerExoApp::GetSWSMessage [creature_destructor_r2]
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);                // GetServerAIMaster [creature_destructor_r2]
void* __fastcall recovered_server_player_list_004aed90(void* server, void*);              // CServerExoApp::GetPlayerList
U __fastcall kotor_fastcall_004aea40(void* server, void*);                                // CServerExoApp::GetPlayerCreatureId [0595]
void* __fastcall kotor_fastcall_00561080(void* player, void*);                            // CSWSPlayer::GetGameObject [0595]
void* __fastcall recovered_waypoint_by_id_004ae7e0(void* server, void*, U id);            // GetWaypointByGameObjectID [w2_party_native]
void __fastcall recovered_item_add_to_area_00555720(void* item, void*, void* area, float x, float y, float z, U flag); // CSWSItem::AddToArea RET 14h [W1 b7]
void __fastcall kotor_fastcall_004c8170(void* module, void*, U index);                    // CSWSModule::DeleteWorldJournalEntry [claude_p2s_b_journal]
U __fastcall kotor_fastcall_0056a300(void* msg, void*, void* player, U index);           // SendServerToPlayerJournalDeleteWorld [claude_p2s_b_journal]
U __fastcall kotor_fastcall_0056aac0(void* message, void*, U player_id, U panel, U flag1, U flag2, U value, const void* text); // PopUpGUIPanel msg [claude_p2s_a_party]
void* __fastcall recovered_message_data_construct_004d69f0(void* data, void*);            // CSWCCMessageData() [W1]
void __fastcall recovered_message_data_destroy_004d6a40(void* data, void*);               // ~CSWCCMessageData [W1 b7]
void __fastcall recovered_event_string_set_004d6ae0(void* data, void*, U index, const void* s); // CSWCCMessageData::SetString [0886]
void __fastcall recovered_event_integer_004d76b0(void* data, void*, U index, U value);    // CSWCCMessageData::SetInteger [1017]
U __fastcall recovered_cc_message_00568570(void* message, void*, U player, U kind, void* data, U flag); // SendServerToPlayerCCMessage [cc_message]
void __fastcall kotor_fastcall_005ede10(void* app, void*, U pending, U delay_bits, U show); // CClientExoApp::SetEndGamePending RET 0Ch [0596]
void __fastcall kotor_fastcall_005edf20(void* client, void*);                             // CClientExoApp::DisableVideoEffect [video_effect]
void __fastcall kotor_fastcall_0048dde0(void* list, void*, int index);                    // CExoArrayList<12-byte>::DelIndex [0296]
// CSWMessage / CSWSMessage writers [w2_message_native]
U __fastcall kotor_fastcall_004d68a0(void* message, void*, U a, U b, U c);                // CreateWriteMessage RET 0Ch
void __fastcall kotor_fastcall_00578570(void* message, void*, U id);                      // WriteOBJECTIDServer
void __fastcall kotor_fastcall_004d6340(void* message, void*, U value, U bits);           // WriteDWORD
void __fastcall kotor_fastcall_004d62c0(void* message, void*, U value, U bits);           // WriteBYTE
void __fastcall kotor_fastcall_004d6300(void* message, void*, U value, U bits);           // WriteWORD
void __fastcall kotor_fastcall_005785c0(void* msg, void*, void* loc, U gender);           // WriteCExoLocStringServer
void __fastcall kotor_fastcall_004d68e0(void* message, void*, U value);                   // WriteBOOL
U __fastcall kotor_fastcall_004d6440(void* message, void*, void** data, U* size);         // GetWriteMessage
U __fastcall kotor_fastcall_00567e60(void* message, void*, U player, U major, U minor, void* data, U size); // SendServerToPlayerMessage
U __fastcall recovered_item_cost_00554000(void* item, void*);                             // CSWSItem::GetCost [w2_g07_native]
U __fastcall recovered_creature_can_use_item_0051b440(void* creature, void*, void* item, U flag); // CSWSCreature::CanUseItem [w2_g07_native]
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void* item, void*);               // CSWItem::GetBaseItem [0820]
void* __fastcall kotor_m15_menu_active_property_00553960(void* item, void*, I index);     // CSWSItem::GetActiveProperty RET 4
// GUI / client [various]
U __fastcall kotor_fastcall_005edc10(void* client, void*);                                // GetPausedByCombat [0714]
U __fastcall kotor_fastcall_005edc20(void* client, void*, U a, U b, U c);                 // SetPausedByCombat [0714]
void __fastcall kotor_fastcall_005edb70(void* client, void*, U a, U b);                   // CClientExoApp::IndicateObject RET 8 [1205-30]
void __fastcall frame_R4_0062aa00(void* gui, void*, U a, U b);                            // CGuiInGame::SetSWGuiStatus [0596]
void __fastcall frame_R4_005d5e80(void* sound, void*, U mode);                            // CExoSoundInternal::SetSoundMode [0709]
void __fastcall object_projectile_bark_006a9920(void* bubble, void*, U id, void* text, void* sound, U flags); // CSWGuiBarkBubble::SetBark
void __fastcall kotor_fastcall_0040bc70(void* manager, void*, void* panel, U a, U b);     // CSWGuiManager::AddPanel RET 0Ch [0298]
void __fastcall kotor_fastcall_0040a140(void* manager, void*, U sound);                   // CSWGuiManager::PlayGuiSound [1205-30]
void* __fastcall kotor_fastcall_006c1c00(void* self, void*, void* manager);               // CSWGuiStore::CSWGuiStore [panels_menus]
// ---- owned by lane "levelup" (not yet landed) ----
// ---- reconstructed in this file (section at the end) ----
void __fastcall kotor_fastcall_0062e310(void* gui, void*, U store, U pc);
U __fastcall kotor_fastcall_0056a6d0(void* msg, void*, void* player, U waypoint, U enabled);
U __fastcall kotor_fastcall_0056b670(void* msg, void*, U player, void* area, void* item, Vector position);
void __fastcall kotor_fastcall_00567090(void* msg, void*, void* item);
void __fastcall kotor_fastcall_00566f20(void* msg, void*, void* item, void* creature);
void __fastcall kotor_fastcall_005edc50(void* client, void*, U value);
void __fastcall kotor_fastcall_005f2f00(void* internal, void*, U value);
U __fastcall kotor_fastcall_005ede50(void* client, void*, I pattern);
U __fastcall kotor_fastcall_005f74b0(void* internal, void*, I pattern);
I __fastcall kotor_fastcall_005ee210(void* client, void*, void* name, I flag);
void __fastcall kotor_fastcall_00563ae0(void* party, void*, U planet, U value);
}

using namespace vmb;

namespace {
const char k_credits_007471c0[] = "g_i_credits001";   // .rdata 007471C0
const char k_empty_0073d71c[] = "";                   // .rdata 0073D71C
constexpr U k_sound_mode_0074c5fc = 4;                // .rdata 0074C5FC
inline void* in_game_gui() { return frame_R4_005ed690(client_app(), nullptr); }
inline void* rules() { return kotor_cell_rules_007a3a28; }
inline void exo_ctor(ExoString* s) {
    (void)kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(s), nullptr);
}
}  // namespace

extern "C" {

// ===================================================================================================== 0053E690
// GiveGoldToCreature(object oCreature, int nGP) [routine 322]
int __fastcall kotor_fastcall_0053e690(void*, void*, int, unsigned) {
    U id = 0; I amount = 0;
    if (!pop_object(&id)) return k_pop_failed;                                           // 0053E699..A5
    if (!pop_int(&amount)) return k_pop_failed;                                          // 0053E6A7..B9
    if (amount > 0) {                                                                    // 0053E6C6..CC (jle)
        void* object = nullptr;
        if (lookup(id, &object) && object != nullptr) {                                  // 0053E6CE..FB
            void* creature = as_creature(object);                                        // 0053E6FD..03
            if (creature != nullptr) {                                                   // 0053E705..07
                recovered_creature_add_gold_004f3db0(creature, nullptr, static_cast<U>(amount), 1); // 0053E709..12
                if (at<U>(creature, 0xa88) != 0)                                         // 0053E717..1F party member
                    kotor_fastcall_0062eeb0(in_game_gui(), nullptr, 1, static_cast<U>(amount)); // 0053E721..38
            }
        }
    }
    return 0;                                                                            // 0053E73E
}

// ===================================================================================================== 00544970
// TakeGoldFromCreature(int nAmount, object oCreatureToTakeFrom, int bDestroy=FALSE) [routine 444]
// OBJECT_SELF receives the gold: creature (AddGold), placeable with inventory (new credits item), item container
// (new credits item into its repository).  Quirks kept: GetGameObject(OBJECT_SELF) is not null-checked; the new
// credits item is used (LoadFromTemplate, stack size store) without a null check.
int __fastcall kotor_fastcall_00544970(void* self, void*, int, unsigned) {
    I amount = 0; U id = 0; I destroy = 0;
    if (!pop_int(&amount)) return k_pop_failed;                                          // 00544989..9D
    if (!pop_object(&id)) return k_pop_failed;                                           // 0054499F..B1
    if (!pop_int(&destroy)) return k_pop_failed;                                         // 005449B3..C5
    if (amount <= 0) return 0;                                                           // 005449DE..E4 (jle)
    void* creature = creature_by_id(id);                                                 // 005449EA..FD
    if (creature == nullptr) return 0;                                                   // 005449FF..01
    if (amount > static_cast<I>(w2x_creature_gold_004edd60(creature, nullptr)))          // 00544A07..14 (jle signed)
        amount = static_cast<I>(w2x_creature_gold_004edd60(creature, nullptr));         // 00544A16..1F
    if (amount <= 0) return 0;                                                           // 00544A23..25
    void* receiver = as_object(game_object(self_id(self)));                              // 00544A2B..43 (no null check)
    if (receiver == nullptr) return 0;                                                   // 00544A45..47
    const U8 type = at<U8>(receiver, 8);                                                 // 00544A4D
    if (type == 5) {                                                                     // 00544A50..56 creature
        kotor_fastcall_004f3ed0(creature, nullptr, static_cast<U>(amount), 1);           // 00544A58..5D
        if (destroy == 0) {                                                              // 00544A62..68
            void* c = as_creature(receiver);                                             // 00544A6E..72
            if (c != nullptr) recovered_creature_add_gold_004f3db0(c, nullptr, static_cast<U>(amount), 1); // 00544A75..86
        }
    } else if (type == 9) {                                                              // 00544A90..92 placeable
        kotor_fastcall_004f3ed0(creature, nullptr, static_cast<U>(amount), 1);           // 00544A98..9D
        if (destroy == 0) {                                                              // 00544AA2..A8
            void* placeable = as_placeable(receiver);                                    // 00544AAE..B2
            if (placeable != nullptr && at<U>(placeable, 0x324) != 0 && at<U>(placeable, 0x36c) != 0) { // 00544AB5..D5
                void* memory = live_operator_new(0x29c);                                 // 00544ADB..E5
                void* item = memory ? recovered_item_construct_005530a0(memory, nullptr, k_invalid) : nullptr; // 00544AEC..08
                menu_resources::Ref resref;                                              // [esp+1C]
                resref.construct_00406d80(k_credits_007471c0);                           // 00544B08..1D
                recovered_item_template_005608b0(item, nullptr, &resref);                // 00544B22..2B
                at<U>(item, 0x28c) = static_cast<U>(amount);                             // 00544B30..46 stack size
                recovered_placeable_acquire_item_00584b10(placeable, nullptr, &item, k_invalid, 1); // 00544B38..4C
            }
        }
    } else if (type == 6) {                                                              // 00544B56..58 item
        kotor_fastcall_004f3ed0(creature, nullptr, static_cast<U>(amount), 1);           // 00544B5E..63
        if (destroy == 0) {                                                              // 00544B68..6E
            void* container = as_item(receiver);                                         // 00544B74..78
            if (container != nullptr) {                                                  // 00544B7B..7F
                void* base = kotor_m15b_base_item_array_get_base_item_005b31a0(at<void*>(rules(), 0x34), nullptr,
                                                                               at<int>(container, 0xc)); // 00544B85..92
                if (at<U>(base, 0x70) != 0) {                                            // 00544B97..9C
                    void* repository = at<void*>(container, 0x26c);                      // 00544BA2
                    if (repository != nullptr) {                                         // 00544BA8..AA
                        void* memory = live_operator_new(0x29c);                         // 00544BB0..BA
                        void* item = memory ? recovered_item_construct_005530a0(memory, nullptr, k_invalid) : nullptr; // 00544BC1..DD
                        menu_resources::Ref resref;                                      // [esp+2C]
                        resref.construct_00406d80(k_credits_007471c0);                   // 00544BDD..F2
                        recovered_item_template_005608b0(item, nullptr, &resref);        // 00544BF7..00
                        at<U>(item, 0x28c) = static_cast<U>(amount);                     // 00544C05..17
                        recovered_inventory_insert_0055d330(repository, nullptr, &item, 1, 0, 1); // 00544C0D..20
                    }
                }
            }
        }
    } else {
        kotor_fastcall_004f3ed0(creature, nullptr, static_cast<U>(amount), 1);           // 00544C27..2C
    }
    if (at<U>(creature, 0xa88) != 0)                                                     // 00544C31..39
        kotor_fastcall_0062eeb0(in_game_gui(), nullptr, 1, static_cast<U>(-amount));     // 00544C3B..54
    return 0;                                                                            // 00544C5E
}

// ===================================================================================================== 00547F60
// CreateItemOnFloor(string sTemplate, location lLocation, int bUseAppearAnimation=FALSE) [routine 766]
// bUseAppearAnimation is popped but unused.  Quirks kept: the new item is not null-checked; a pop failure leaks an
// already popped location.
int __fastcall kotor_fastcall_00547f60(void*, void*, int, unsigned) {
    void* message = recovered_server_sws_message_004aedb0(server_app(), nullptr);        // 00547F78..93
    U result = k_invalid;                                                                // 00547F89 edi
    ExoString name; exo_ctor(&name);                                                     // 00547F8F..95
    void* location = nullptr;                                                            // [esp+10] (set by the pop)
    I appear = 0;                                                                        // 00547FAB
    if (!pop_string(&name) || !pop_engine(2, &location) || !pop_int(&appear)) {          // 00547F9A..E0
        exo_destroy(&name);                                                              // 00547FE2..EE
        return k_pop_failed;
    }
    (void)appear;
    void* module = server_get_module_004ae6b0(server_app(), nullptr);                    // 00547FFD..06
    if (module != nullptr) {                                                             // 0054800B..0D
        void* area = module_get_area_004c30f0(module, nullptr);                          // 00548013..15
        if (area != nullptr) {                                                           // 0054801C..1E
            menu_resources::Ref resref;                                                  // [esp+24]
            resref.construct_00406d80(WindowCStr005e5670(&name, nullptr));               // 00548024..32
            void* memory = live_operator_new(0x29c);                                     // 00548037..41
            void* item = memory ? recovered_item_construct_005530a0(memory, nullptr, k_invalid) : nullptr; // 00548044..5D
            recovered_item_template_005608b0(item, nullptr, &resref);                    // 0054805F..6B
            void* as_obj = static_cast<unsigned char*>(item) + 0x10;                     // 0054807A CSWSObject base
            recovered_object_position_004cd1a0(as_obj, nullptr, static_cast<const Vector*>(location), 1, 1, 0); // 00548070..80
            w1x_object_set_orientation_004cd170(as_obj, nullptr, static_cast<unsigned char*>(location) + 0xc); // 00548085..8F
            recovered_item_add_to_area_00555720(item, nullptr, area, at<float>(location, 0), at<float>(location, 4),
                                                at<float>(location, 8), 1);              // 00548094..A8
            result = at<U>(item, 0x14);                                                  // 005480AF
            if (message != nullptr) {                                                    // 005480AD..B2
                Vector position{at<float>(location, 0), at<float>(location, 4), at<float>(location, 8)}; // 005480B4..E7
                kotor_fastcall_0056b670(message, nullptr, k_invalid, area, item, position); // 005480E0..ED
            }
        }
    }
    const U pushed = push_object_raw(result);                                            // 005480F4..00
    if (location != nullptr) { location_dtor_004015a0(location); live_free(location); }  // 00548108..19 / 00548138..49
    exo_destroy(&name);                                                                  // 00548120..2C / 00548150..5C
    return pushed ? 0 : k_push_failed;                                                   // 00548131 / 00548161
}

// ===================================================================================================== 00540300
// OpenStore(object oStore, object oPC, int nBonusMarkUp=0, int nBonusMarkDown=0) [routine 378]
// Both bonuses are clamped to [-100, 100]; the store receives +250h = mark-up, +24Ch = mark-down, the GUI opens and
// AI event 16h (store opened) is queued from the PC (or the player creature when oPC has no client) to the store.
// Quirk kept: a failed CScriptEvent allocation is written through (null) unchecked.
int __fastcall kotor_fastcall_00540300(void*, void*, int, unsigned argc) {
    I markup = 0, markdown = 0;                                                          // 00540323..2B
    U store_id = 0, pc_id = 0;
    if (!pop_object(&store_id)) return k_pop_failed;                                     // 00540318..3A
    if (!pop_object(&pc_id)) return k_pop_failed;                                        // 00540340..52
    if (static_cast<I>(argc) > 2) {                                                      // 00540358..5D (jle signed)
        if (!pop_int(&markup)) return k_pop_failed;                                      // 0054035F..71
        if (!pop_int(&markdown)) return k_pop_failed;                                    // 00540377..89
    }
    if (markup > 100) markup = 100; else if (markup < -100) markup = -100;               // 0054038F..A9
    if (markdown > 100) markdown = 100; else if (markdown < -100) markdown = -100;       // 005403AF..BF
    void* client = kotor_fastcall_004ae910(server_app(), nullptr, pc_id);                // 005403C5..DD
    void* object = game_object(store_id);                                                // 005403D9..E8
    if (object == nullptr) return 0;                                                     // 005403ED..EF
    void* store = vcall<void*>(object, 0x64);                                            // 005403F5..F9 AsSWSStore
    if (store == nullptr) return 0;                                                      // 005403FC..FE
    at<I>(store, 0x250) = markup;                                                        // 00540404..08
    at<I>(store, 0x24c) = markdown;                                                      // 0054040E..12
    kotor_fastcall_0062e310(in_game_gui(), nullptr, store_id, pc_id);                    // 00540418..32
    void* memory = live_operator_new(0x34);                                              // 00540437..3E
    void* event = memory ? recovered_event_construct_004d7540(memory, nullptr) : nullptr; // 00540441..5C
    at<U16>(event, 0) = 0x16;                                                            // 00540468
    U caller;
    if (client == nullptr) {                                                             // 0054045E..6D
        caller = kotor_fastcall_004aea40(server_app(), nullptr);                         // 0054046F..87
    } else {
        caller = at<U>(kotor_fastcall_00561080(client, nullptr), 4);                     // 00540490..A3 (no null check)
    }
    recovered_event_enqueue_004b08d0(recovered_server_ai_master_004aed80(server_app(), nullptr), nullptr, 0, 0,
                                     caller, store_id, 0xa, event);                      // 005404A9..BC [007441DC]
    return 0;                                                                            // 005404C2
}

// 00543870 ShowLevelUpGUI moved to claude_vm_vmB_pending.cpp (pending on lane "levelup").

// ===================================================================================================== 00541320
// PopUpGUIPanel(object oPC, int nGUIPanel) [routine 388] /
// PopUpDeathGUIPanel(object oPC, int bRespawn=TRUE, int bWaitForHelp=TRUE, int nHelpStrRef=0, string sHelp="") [554]
// Both send SendServerToPlayerPopUpGUIPanel(oPC, panel, respawn!=0, wait!=0, strref, &text) unless oPC is invalid.
// Routine 554 pops all four optional arguments regardless of argc; its panel stays 0.
int __fastcall kotor_fastcall_00541320(void*, void*, int routine, unsigned) {
    U pc = k_invalid;                                                                    // 00541346
    I strref = 0, wait = 1, respawn = 1, panel = 0;                                      // 0054134E..5E
    ExoString text;
    frame_LogStringCtor_005e5a90(&text, nullptr, k_empty_0073d71c);                      // 0054133D..66
    if (!pop_object(&pc)) { exo_destroy(&text); return k_pop_failed; }                   // 0054136B..85 -> 0054142C
    if (routine == 0x184) {                                                              // 0054138B..94
        if (!pop_int(&panel)) { exo_destroy(&text); return k_pop_failed; }               // 00541396..BF
    } else if (routine == 0x22a) {                                                       // 005413D5..DA
        if (!pop_int(&respawn) || !pop_int(&wait) || !pop_int(&strref) || !pop_string(&text)) { // 005413DC..2A
            exo_destroy(&text);                                                          // 0054142C..38
            return k_pop_failed;
        }
    }
    if (pc != k_invalid) {                                                               // 00541453..5C
        kotor_fastcall_0056aac0(recovered_server_sws_message_004aedb0(server_app(), nullptr), nullptr, pc,
            static_cast<U>(panel), respawn != 0 ? 1u : 0u, wait != 0 ? 1u : 0u, static_cast<U>(strref), &text); // 0054145E..97
    }
    exo_destroy(&text);                                                                  // 0054149D..A9
    return 0;
}

// ===================================================================================================== 005472F0
// SetTutorialWindowsEnabled(int bEnabled) [routine 516]: client options +14h bit 1 = bEnabled bit 0
int __fastcall kotor_fastcall_005472f0(void*, void*, int, unsigned) {
    I enabled = 0;
    if (!pop_int(&enabled)) return k_pop_failed;                                         // 005472F0..03
    void* options = kotor_fastcall_005ed700(client_app(), nullptr);                      // 0054730E..1C
    const U flags = at<U>(options, 0x14);                                                // 00547321
    at<U>(options, 0x14) = flags ^ ((static_cast<U>(enabled) * 2u ^ flags) & 2u);        // 00547324..2E
    return 0;
}

// ===================================================================================================== 00548210
// QueueMovie(string sMovie, int bSkippable) [routine 769]
int __fastcall kotor_fastcall_00548210(void*, void*, int, unsigned) {
    ExoString movie; exo_ctor(&movie);                                                   // 00548228..2C
    I skippable = 0;
    if (!pop_string(&movie) || !pop_int(&skippable)) {                                   // 00548231..5F
        exo_destroy(&movie);                                                             // 00548261..6D
        return k_pop_failed;
    }
    (void)kotor_fastcall_005ee210(client_app(), nullptr, &movie, skippable);             // 00548288..9B
    exo_destroy(&movie);                                                                 // 005482A0..AC
    return 0;
}

// ===================================================================================================== 00535570
// EndGame(int nShowEndGameGui=TRUE) [routine 564]: SetEndGamePending(1, show ? 5.0f : 0.0f, show); show -> SetGameOver
int __fastcall kotor_fastcall_00535570(void*, void*, int, unsigned argc) {
    I show = 1;                                                                          // 0053557C
    if (static_cast<I>(argc) >= 1) {                                                     // 00535571..80 (jl signed)
        if (!pop_int(&show)) return k_pop_failed;                                        // 00535582..9C
    }
    const U delay = show != 0 ? 0x40a00000u : 0u;                                        // 0053559F..B1 (5.0f, reuses the argc slot)
    kotor_fastcall_005ede10(client_app(), nullptr, 1, delay, static_cast<U>(show));      // 005355B9..CA
    if (show != 0) kotor_fastcall_005edc50(client_app(), nullptr, 1);                    // 005355CF..E1
    return 0;
}

// ===================================================================================================== 005446C0
// StopRumblePattern(int nPattern) [routine 371]: pushes the client's StopRumblePattern result
int __fastcall kotor_fastcall_005446c0(void*, void*, int, unsigned) {
    I pattern = 0;
    if (!pop_int(&pattern)) return k_pop_failed;                                         // 005446C0..D3
    const U stopped = kotor_fastcall_005ede50(client_app(), nullptr, pattern);           // 005446DE..EC
    return push_int(static_cast<I>(stopped));                                            // 005446F1..0C
}

// ===================================================================================================== 00547220
// DisableVideoEffect() [routine 510]
int __fastcall kotor_fastcall_00547220(void*, void*, int, unsigned) {
    kotor_fastcall_005edf20(client_app(), nullptr);                                      // 00547220..28
    return 0;
}

// ===================================================================================================== 005468B0
// SetGalaxyMapPlanetSelectable(int nPlanet, int bSelectable) [routine 740]
int __fastcall kotor_fastcall_005468b0(void*, void*, int, unsigned) {
    I planet = 0, selectable = 0;
    if (!pop_int(&planet)) return k_pop_failed;                                          // 005468B0..C5
    if (!pop_int(&selectable)) return k_pop_failed;                                      // 005468C7..D9
    kotor_fastcall_00563ae0(lane_config_required::GetPartyTable_004aee70(server_app(), nullptr), nullptr,
                            static_cast<U>(planet), static_cast<U>(selectable));         // 005468E6..00
    return 0;
}

// ===================================================================================================== 0054B460
// SetMapPinEnabled(object oMapPin, int nEnabled) [routine 386]: waypoint +22Ch = nEnabled when it has a map note
// (+228h), then every player with a game object receives SendServerToPlayerMapPinEnabled.
int __fastcall kotor_fastcall_0054b460(void*, void*, int, unsigned) {
    U id = 0; I enabled = 0;
    if (!pop_object(&id)) return k_pop_failed;                                           // 0054B460..75
    if (!pop_int(&enabled)) return k_pop_failed;                                         // 0054B477..89
    void* waypoint = recovered_waypoint_by_id_004ae7e0(server_app(), nullptr, id);       // 0054B496..A4
    if (waypoint == nullptr || at<U>(waypoint, 0x228) == 0) return 0;                    // 0054B4A9..B9
    at<I>(waypoint, 0x22c) = enabled;                                                    // 0054B4BF..C3
    void* message = recovered_server_sws_message_004aedb0(server_app(), nullptr);        // 0054B4C9..D9
    void* list = recovered_server_player_list_004aed90(server_app(), nullptr);           // 0054B4DB..E8
    if (list == nullptr) return 0;                                                       // 0054B4EA..EC
    void* position = at<void*>(at<void*>(list, 0), 0);                                   // 0054B4EE..F4 head
    while (position != nullptr) {                                                        // 0054B4F8 / 0054B53F
        void* player = unresolved_list_at_pos_005e9b60(at<void*>(list, 0), nullptr, position); // 0054B500..03
        if (player != nullptr && kotor_fastcall_00561080(player, nullptr) != nullptr && message != nullptr) // 0054B50A..1B
            kotor_fastcall_0056a6d0(message, nullptr, player, id, static_cast<U>(enabled)); // 0054B51D..2A
        unresolved_list_next_005e9b90(at<void*>(list, 0), nullptr, &position);           // 0054B52F..36
    }
    return 0;
}

// ===================================================================================================== 00548B20
// DeleteJournalWorldEntry(int nEntryId) [routine 673]
int __fastcall kotor_fastcall_00548b20(void*, void*, int, unsigned) {
    void* message = recovered_server_sws_message_004aedb0(server_app(), nullptr);        // 00548B20..3B
    void* module = server_get_module_004ae6b0(server_app(), nullptr);                    // 00548B32..4D
    I index = 0;
    if (!pop_int(&index)) return k_pop_failed;                                           // 00548B42..62
    if (module != nullptr) kotor_fastcall_004c8170(module, nullptr, static_cast<U>(index)); // 00548B65..70
    void* list = recovered_server_player_list_004aed90(server_app(), nullptr);           // 00548B75..84
    if (list == nullptr) return 0;                                                       // 00548B86..88
    void* position = at<void*>(at<void*>(list, 0), 0);                                   // 00548B8A..94
    while (position != nullptr) {                                                        // 00548B94 / 00548BD2
        void* player = unresolved_list_at_pos_005e9b60(at<void*>(list, 0), nullptr, position); // 00548B96..99
        if (player != nullptr && kotor_fastcall_00561080(player, nullptr) != nullptr && message != nullptr) // 00548B9E..B1
            kotor_fastcall_0056a300(message, nullptr, player, static_cast<U>(index));    // 00548BB3..BB
        unresolved_list_next_005e9b90(at<void*>(list, 0), nullptr, &position);           // 00548BC0..C7
    }
    return 0;
}

// ===================================================================================================== 00535DB0
// FloatingTextStrRefOnCreature(int nStrRef, object oCreature, int bBroadcastToFaction=TRUE) [routine 525] /
// FloatingTextStringOnCreature(string sString, object oCreature, int bBroadcastToFaction=TRUE) [526]
// CC message 0Bh (integer 9 = 5Dh strref / 5Eh string) to oCreature's own client.  Quirk kept: with
// bBroadcastToFaction != 0 nothing is sent at all (the message data is built and freed).
int __fastcall kotor_fastcall_00535db0(void*, void*, int routine, unsigned argc) {
    ExoString text; exo_ctor(&text);                                                     // 00535DCA..CE
    I broadcast = 1;                                                                     // 00535DE5
    I strref = 0;
    U id = 0;
    I value = routine;                                                                   // 00535E25 ebp (only used by 525)
    bool ok = true;
    if (routine == 0x20d) {                                                              // 00535DD3..ED
        ok = pop_int(&strref) != 0;                                                      // 00535DEF..01
        if (ok) value = strref;                                                          // 00535E03
    } else if (routine == 0x20e) {                                                       // 00535E09..0F
        ok = pop_string(&text) != 0;                                                     // 00535E11..23
    }
    if (ok) ok = pop_object(&id) != 0;                                                   // 00535E29..3B
    if (ok && static_cast<I>(argc) >= 3) ok = pop_int(&broadcast) != 0;                  // 00535E3D..56 (jl signed)
    if (!ok) { exo_destroy(&text); return k_pop_failed; }                                // 00535E58..7E
    void* object = game_object(id);                                                      // 00535E81..8F
    if (object != nullptr && at<U8>(object, 8) == 5 && as_object(object) != nullptr) {   // 00535E94..B4 [00746634]
        void* memory = live_operator_new(0x34);                                          // 00535EBB..C2
        void* data = memory ? recovered_message_data_construct_004d69f0(memory, nullptr) : nullptr; // 00535EC5..DD
        if (data != nullptr) {                                                           // 00535EDF..E6
            recovered_event_object_004d7780(data, nullptr, 0, id);                       // 00535EEC..F5
            if (routine == 0x20d) {                                                      // 00535EFA..00
                recovered_event_integer_004d76b0(data, nullptr, 9, 0x5d);                // 00535F02..08
                recovered_event_integer_004d76b0(data, nullptr, 0, static_cast<U>(value)); // 00535F0D..12
            } else if (routine == 0x20e) {                                               // 00535F19..1F
                recovered_event_integer_004d76b0(data, nullptr, 9, 0x5e);                // 00535F21..27
                recovered_event_string_set_004d6ae0(data, nullptr, 0, &text);            // 00535F2C..35
            }
            if (broadcast == 0) {                                                        // 00535F3A..40
                void* client = kotor_fastcall_004ae910(server_app(), nullptr, id);       // 00535F42..50
                if (client != nullptr)                                                   // 00535F55..57
                    recovered_cc_message_00568570(recovered_server_sws_message_004aedb0(server_app(), nullptr), nullptr,
                                                  at<U>(client, 4), 0xb, data, 0);       // 00535F59..72
            }
            recovered_message_data_destroy_004d6a40(data, nullptr);                      // 00535F77..79
            live_free(data);                                                             // 00535F7E..84
        }
    }
    exo_destroy(&text);                                                                  // 00535F88..94
    return 0;
}

// =====================================================================================================
// Reconstructed callees without a callable provider in the c31 map (exact original ABI).
// =====================================================================================================

// 0062E310 CGuiInGame::ShowStoreGui(store id, pc id): thiscall, RET 8.  Only when the in-game GUI is up (+108h),
// no other blocking panel (+30h, +B4h): pause by combat, clear the indicated object, status 3, sound mode 4, clear
// the bark bubble, input class 2, create the CSWGuiStore panel (+84h, 2280h bytes) once, store ids, add + sound 4.
void __fastcall kotor_fastcall_0062e310(void* gui, void*, U store, U pc) {
    if (at<U>(gui, 0x108) == 0 || at<U>(gui, 0x30) != 0 || at<U>(gui, 0xb4) != 0) return; // 0062E32B..4C
    if (kotor_fastcall_005edc10(client_app(), nullptr) == 0)                             // 0062E352..61
        (void)kotor_fastcall_005edc20(client_app(), nullptr, 1, 0, 0);                   // 0062E363..70
    kotor_fastcall_005edb70(client_app(), nullptr, 0, 0);                                // 0062E375..82
    frame_R4_0062aa00(gui, nullptr, 3, 1);                                               // 0062E387..8D
    frame_R4_005d5e80(kotor_cell_sound_007a39ec, nullptr, k_sound_mode_0074c5fc);        // 0062E392..9E
    menu_resources::Ref sound;                                                           // [esp+14]
    sound.construct_00406d80(k_empty_0073d71c);                                          // 0062E3A3..AC
    ExoString text;                                                                      // [esp+0C]
    frame_LogStringCtor_005e5a90(&text, nullptr, k_empty_0073d71c);                      // 0062E3B1..BA
    object_projectile_bark_006a9920(at<void*>(gui, 0x4c), nullptr, k_invalid, &text, &sound, 0); // 0062E3BF..DB
    exo_destroy(&text);                                                                  // 0062E3E0..EC
    (void)kotor_fastcall_005eda60(client_app(), nullptr, 2, 1);                          // 0062E3F1..FD
    if (at<void*>(gui, 0x84) == nullptr) {                                               // 0062E402..0A
        void* memory = live_operator_new(0x2280);                                        // 0062E40C..16
        at<void*>(gui, 0x84) = memory ? kotor_fastcall_006c1c00(memory, nullptr, at<void*>(gui, 0x38)) : nullptr; // 0062E419..40
    }
    at<U>(at<void*>(gui, 0x84), 0x2278) = store;                                         // 0062E446..50
    at<U>(at<void*>(gui, 0x84), 0x227c) = pc;                                            // 0062E456..62
    kotor_fastcall_0040bc70(at<void*>(gui, 0x38), nullptr, at<void*>(gui, 0x84), 3, 1);  // 0062E460..74
    kotor_fastcall_0040a140(at<void*>(gui, 0x38), nullptr, 4);                           // 0062E479..7E
}

// 0056A6D0 CSWSMessage::SendServerToPlayerMapPinEnabled(CSWSPlayer*, waypoint id, enabled): thiscall, RET 0Ch.
// Message 20h/01h: OBJECTID waypoint, BOOL enabled.  EAX = SendServerToPlayerMessage result or 0.
U __fastcall kotor_fastcall_0056a6d0(void* msg, void*, void* player, U waypoint, U enabled) {
    (void)kotor_fastcall_004d68a0(msg, nullptr, 8, 0xffffffffu, 1);                     // 0056A6D4..DC
    kotor_fastcall_00578570(msg, nullptr, waypoint);                                     // 0056A6E1..E8
    kotor_fastcall_004d68e0(msg, nullptr, enabled);                                      // 0056A6ED..F4
    void* data = nullptr; U size = 0;
    if (kotor_fastcall_004d6440(msg, nullptr, &data, &size) == 0) return 0;              // 0056A6F9..0E
    return kotor_fastcall_00567e60(msg, nullptr, at<U>(player, 4), 0x20, 1, data, size); // 0056A715..2D
}

// 0056B670 CSWSMessage::SendServerToPlayerItemScriptAdd(player id, area, item, Vector): thiscall, RET 18h.
// Message 05h/08h: OBJECTID item, appearance, active properties (no creature).  Area and position are unused.
U __fastcall kotor_fastcall_0056b670(void* msg, void*, U player, void* area, void* item, Vector position) {
    (void)area; (void)position;
    (void)kotor_fastcall_004d68a0(msg, nullptr, 0x80, 0xffffffffu, 1);                  // 0056B675..80
    kotor_fastcall_00578570(msg, nullptr, at<U>(item, 0x14));                            // 0056B685..8F
    kotor_fastcall_00567090(msg, nullptr, item);                                         // 0056B694..97
    kotor_fastcall_00566f20(msg, nullptr, item, nullptr);                                // 0056B69C..A1
    void* data = nullptr; U size = 0;
    if (kotor_fastcall_004d6440(msg, nullptr, &data, &size) == 0) return 0;              // 0056B6A6..C0
    return kotor_fastcall_00567e60(msg, nullptr, player, 5, 8, data, size);              // 0056B6C3..D8
}

// 00567090 CSWSMessage::AddItemAppearanceToMessage(CSWSItem*): thiscall, RET 4.
void __fastcall kotor_fastcall_00567090(void* msg, void*, void* item) {
    const U base_id = at<U>(item, 0xc);                                                  // 00567097
    kotor_fastcall_004d6340(msg, nullptr, base_id, 0x20);                                // 0056709A..9F
    void* base = kotor_m15b_base_item_array_get_base_item_005b31a0(at<void*>(rules(), 0x34), nullptr,
                                                                   static_cast<int>(base_id)); // 005670A4..AD
    if (base == nullptr) return;                                                         // 005670B2..B6
    kotor_fastcall_004d62c0(msg, nullptr, at<U8>(item, 0x264), 8);                       // 005670B8..C5
    if (at<U8>(base, 0x18) != 1) return;                                                 // 005670CA..CE
    kotor_fastcall_004d62c0(msg, nullptr, at<U8>(item, 0x265), 8);                       // 005670D0..DD
    kotor_fastcall_004d62c0(msg, nullptr, at<U8>(item, 0x266), 8);                       // 005670E2..EF
}

// 00566F20 CSWSMessage::AddActiveItemPropertiesToMessage(CSWSItem*, CSWSCreature*): thiscall, RET 8.
void __fastcall kotor_fastcall_00566f20(void* msg, void*, void* item, void* creature) {
    if (at<U8>(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0xac) >= 0x1f &&   // 00566F2A..38 (jb)
        at<U8>(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0xac) <= 0x26)     // 00566F3A..48 (ja)
        kotor_fastcall_004d6300(msg, nullptr, at<U16>(item, 0x244), 0x10);               // 00566F4A..58
    kotor_fastcall_005785c0(msg, nullptr, static_cast<unsigned char*>(item) + 0x280, 0); // 00566F5D..68
    kotor_fastcall_004d68e0(msg, nullptr, 1);                                            // 00566F6D..71
    kotor_fastcall_004d6340(msg, nullptr, recovered_item_cost_00554000(item, nullptr), 0x20); // 00566F76..82
    kotor_fastcall_004d6340(msg, nullptr, at<U>(item, 0x28c), 0x20);                     // 00566F87..92
    const U usable = creature != nullptr ? recovered_creature_can_use_item_0051b440(creature, nullptr, item, 1) : 0; // 00566F97..A9
    kotor_fastcall_004d68e0(msg, nullptr, usable);                                       // 00566FAB..AE
    kotor_fastcall_004d68e0(msg, nullptr, (at<U>(item, 0x288) >> 5) & 1u);              // 00566FB3..C2
    const I count = at<I>(item, 0x248);                                                  // 00566FC7
    kotor_fastcall_004d62c0(msg, nullptr, static_cast<U>(count), 8);                     // 00566FCD..D2
    for (I i = 0; i < count; ++i) {                                                      // 00566FD7..3F (jl signed)
        kotor_fastcall_004d6300(msg, nullptr, at<U16>(kotor_m15_menu_active_property_00553960(item, nullptr, i), 0), 0x10); // 00566FE0..F2
        kotor_fastcall_004d6300(msg, nullptr, at<U16>(kotor_m15_menu_active_property_00553960(item, nullptr, i), 2), 0x10); // 00566FF7..0A
        kotor_fastcall_004d6300(msg, nullptr, at<U16>(kotor_m15_menu_active_property_00553960(item, nullptr, i), 6), 0x10); // 0056700F..20
        kotor_fastcall_004d62c0(msg, nullptr, at<U8>(kotor_m15_menu_active_property_00553960(item, nullptr, i), 9), 8);     // 00567025..37
    }
    kotor_fastcall_004d62c0(msg, nullptr, at<U8>(item, 0x238), 8);                       // 00567041..4E
    kotor_fastcall_004d62c0(msg, nullptr, 0xff, 8);                                      // 00567053..5C
    for (U k = 0; k < 8; ++k)                                                            // 00567061..80
        kotor_fastcall_004d62c0(msg, nullptr, at<U8>(item, 0x239 + k), 8);
}

// 005EDC50 CClientExoApp::SetGameOver(int): mov ecx,[ecx+4]; jmp 005F2F00
void __fastcall kotor_fastcall_005edc50(void* client, void*, U value) {
    kotor_fastcall_005f2f00(at<void*>(client, 4), nullptr, value);                       // 005EDC50..53
}

// 005F2F00 CClientExoAppInternal::SetGameOver(int): thiscall, RET 4.  +2C0h = value, +2C4h = 1.
void __fastcall kotor_fastcall_005f2f00(void* internal, void*, U value) {
    at<U>(internal, 0x2c0) = value;                                                      // 005F2F00..04
    at<U>(internal, 0x2c4) = 1;                                                          // 005F2F0A
}

// 005EDE50 CClientExoApp::StopRumblePattern(int): mov ecx,[ecx+4]; jmp 005F74B0
U __fastcall kotor_fastcall_005ede50(void* client, void*, I pattern) {
    return kotor_fastcall_005f74b0(at<void*>(client, 4), nullptr, pattern);              // 005EDE50..53
}

// 005F74B0 CClientExoAppInternal::StopRumblePattern(int): thiscall, RET 4.  Removes the first 12-byte entry of
// the rumble list (+34Ch data, +350h count) whose first dword equals the pattern; EAX 1 when removed, else 0.
U __fastcall kotor_fastcall_005f74b0(void* internal, void*, I pattern) {
    const I count = at<I>(internal, 0x350);                                              // 005F74B1
    const unsigned char* entry = at<unsigned char*>(internal, 0x34c);                    // 005F74BE
    for (I i = 0; i < count; ++i, entry += 0xc) {                                        // 005F74B7..DA (jle/jl signed)
        if (*reinterpret_cast<const I*>(entry) == pattern) {                             // 005F74D0..D2
            kotor_fastcall_0048dde0(static_cast<unsigned char*>(internal) + 0x34c, nullptr, i); // 005F74E3..E4
            return 1;                                                                    // 005F74EB
        }
    }
    return 0;                                                                            // 005F74DE
}

// 005EE210 CClientExoApp::AddMovieToMovieQueue(CExoString*, int): mov ecx,[ecx+4]; jmp 005FBF50
I __fastcall kotor_fastcall_005ee210(void* client, void*, void* name, I flag) {
    return client_intro::AddMovieToMovieQueue_005fbf50(at<options_video::Client*>(client, 4), nullptr,
                                                       static_cast<options_video::String*>(name), flag); // 005EE210..13
}

// 00563AE0 CSWPartyTable::SetPlanetSelectable(planet, value): thiscall, RET 8.  Ignores planet >= 10h (unsigned).
void __fastcall kotor_fastcall_00563ae0(void* party, void*, U planet, U value) {
    if (planet < 0x10) at<U>(party, 0xa0 + planet * 4) = value;                          // 00563AE0..ED
}

}  // extern "C"
