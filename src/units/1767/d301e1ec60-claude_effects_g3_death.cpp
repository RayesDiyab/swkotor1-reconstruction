// effects-g3 (Claude), part D: CSWSEffectListHandler death / resurrection callbacks for the 0854 ai_resolve table.
// Transcribed from the original swkotor.exe (read-only); addresses in comments are original instructions.
// ABI: original __thiscall members are exported as extern "C" __fastcall(self, unused_edx, stack args...):
//   004E0AC0 CSWSEffectListHandler::OnApplyDeath(CSWSObject*, CGameEffect*, int load_game)        RET 0Ch, EAX = 1
//   004E13C0 CSWSEffectListHandler::OnApplyResurrection(CSWSObject*, CGameEffect*, int)           RET 0Ch, EAX = 1
// Missing callees reconstructed here:
//   004DF420 CSWSEffectListHandler::IsEffectToIgnoreOnDeath(CGameEffect*)                        RET 4,  EAX 0/1
//   004FB1E0 CSWSCreature::ApplyDeathExperience()                                                 RET 0
//   004F19E0 CSWSCreature::CalculateDeathExperience()                                             RET 0,  ST0
//   004EC500 CSWSCreature::BroadcastDeathDataToParty(CSWCCMessageData*)                           RET 4
//   005EDC70 CClientExoApp::SetLastPartyMemberTempKilled(ulong)  -> [ecx+4]; jmp 005F2FE0        RET 4
//   005F2FE0 CClientExoAppInternal::SetLastPartyMemberTempKilled(ulong)                           RET 4
//   0062B150 CGuiInGame::RemoveGUIPopUps()                                                        RET 0
//   004F28D0 CSWSCreature::RecomputeAmbientAnimationState()                                       RET 0
//   0056B320 CSWSMessage::SendServerToPlayerDestroyDeathGUI(ulong player)                         RET 4,  EAX = 1
// Imported from sibling effects-g3 parts: 004EFEF0 SetAmbientAnimationState (part damage).
// .rdata bound to proven values: [00746634]=5 (creature type byte), [00746638]=9 (placeable), [00746639]=0Ah (door),
//   [0074666C]=0 (object-array success byte), [007441DC]=0Ah / [007441E0]=0Bh (AI event ids), [007473B8]=0,
//   [007473BC]=3, [007473C0]=1, [007473C8]=4 (effect sub-type words), [00740C84]=3.0f, [0073D6FC]=1000.0f,
//   [0073DFEC]=0.01f, [0073D764]=1.0f, [0073D700]=0.0f, [007455BC]=900.0f; strings "DestroyObjectDelay" (00746AC4),
//   "(PC)" (00746ABC), "EffectType" (007469E8), "PercentXP" (00746F08).
// EH: the original unwind frames only release CExoString locals / new'd blocks if a callee throws; the bound callees do
// not throw in normal operation, so that cleanup is not reproduced (the normal path releases everything).
// x87: computations are done in double (53-bit precision control), rounded to float exactly where the original stores
// a dword.
#include <cmath>
#include <cstdint>
#include <cstring>

namespace {
using U = std::uint32_t;
using I = std::int32_t;
using U16 = std::uint16_t;
template <class T> inline T& at(void* object, U offset) { return *reinterpret_cast<T*>(static_cast<unsigned char*>(object) + offset); }
inline U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline float flt(U u) { float f; std::memcpy(&f, &u, 4); return f; }
inline U bits(float f) { U u; std::memcpy(&u, &f, 4); return u; }
template <class R, class... A> inline R vcall(void* self, U slot, A... a) {
    using Fn = R(__thiscall*)(void*, A...);
    return reinterpret_cast<Fn>(at<void*>(at<void*>(self, 0), slot))(self, a...);
}
struct ExoString { char* text; U length; };                                     // CExoString (8 bytes)
}

namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }    // [007A39FC] CAppManager*
namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } } // [007A39E0] CExoBase*
namespace frame_control { extern unsigned message_manager_007a3a00; }            // [007A3A00] CVirtualMachine*
namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); } // CExoString()
namespace kotor_reconstruction { struct StartupString; }
kotor_reconstruction::StartupString* __fastcall GuiConcat005e5d10(kotor_reconstruction::StartupString* self, void*,
    kotor_reconstruction::StartupString* out, const kotor_reconstruction::StartupString* rhs); // CExoString::operator+
namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void*, void*); } // GetPartyTable [0575]
namespace w2 { unsigned __cdecl ftol2_006fae8c(double value); }                  // __ftol2 (low dword)
namespace w2 { namespace g07 { unsigned __cdecl is_beam_type_004ca960(unsigned type); } } // CLoopingVisualEffect::GetIsBeam

extern "C" {
extern void* kotor_cell_rules_007a3a28;                                          // [007A3A28] CSWRules*
// ---------------- existing providers (c27 map, exact decorated arity) ----------------
void* __cdecl live_operator_new(unsigned);                                       // 006FA7E6
void __cdecl live_free(void*);                                                   // 006FA390
int __cdecl crt_rand_006fc447();                                                 // _rand [0479]
void* __fastcall frame_LogStringCtor_005e5a90(void* string, void*, const char* text); // CExoString(char const*)
void __fastcall frame_LogStringDtor_005e5c20(void* string, void*);              // ~CExoString
void* __fastcall async_assign_string_005e5c50(void* self, void*, void* source); // CExoString::operator=
const char* __fastcall WindowCStr005e5670(void* string, void*);                 // CExoString::CStr
int __fastcall c2da_current_get_int_00413660(void* table, void*, int row, void* column, int* out);   // C2DA::GetINTEntry
U __fastcall c2da_current_get_float_00413430(void* table, void*, I row, void* column, float* out);   // C2DA::GetFLOATEntry
I __fastcall kotor_m15b_c2da_get_float_entry_00413350(void* table, void*, int row, U column, float* out); // GetFLOATEntry(int col)
void* __fastcall server_get_module_004ae6b0(void* server, void*);                // GetModule
void* __fastcall kotor_fastcall_004ae770(void* server, void*, U id);             // GetCreatureByGameObjectID
void* __fastcall kotor_fastcall_004ae7f0(void* server, void*, U id);             // GetEncounterByGameObjectID
void* __fastcall kotor_fastcall_004ae910(void* server, void*, U id);             // GetClientObjectByObjectId
U __fastcall kotor_fastcall_004aea40(void* server, void*);                       // GetPlayerCreatureId
void* __fastcall recovered_object_array_004aed70(void* server, void*);           // GetObjectArray
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);       // GetServerAIMaster
void* __fastcall recovered_server_player_list_004aed90(void* server, void*);     // GetPlayerList
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);     // GetSWSMessage
void* __fastcall kotor_fastcall_004aefc0(void* server, void*);                   // GetFactionManager
U __fastcall recovered_event_enqueue_004b08d0(void* ai, void*, U day, U ms, U source, U target, U kind, void* data); // AddEventDeltaTime
void __fastcall w1x_object_set_hp_004cc280(void* object, void*, U hp);           // CSWSObject::SetCurrentHitPoints
void* __fastcall recovered_object_render_004cc2b0(void* object, void*);          // CSWSObject::GetClientObject
void __fastcall w5_object_clear_all_actions_004ccd80(void* object, void*, U clear_combat); // ClearAllActions
U __fastcall recovered_object_remove_effect_004d06c0(void* object, void*, U id_lo, U id_hi); // RemoveEffectById
void __fastcall recovered_effect_apply_004d14f0(void* object, void*, void* effect, U a, U b); // ApplyEffect
U __fastcall kotor_fastcall_004d4620(void* round, void*, U flag);                // CSWSCombatRound::EndCombatRound
void* __fastcall recovered_message_data_construct_004d69f0(void*, void*);       // CSWCCMessageData ctor
void __fastcall recovered_message_data_destroy_004d6a40(void*, void*);          // CSWCCMessageData dtor
U __fastcall kotor_fastcall_004d6ee0(void* data, void*, void* other);            // CSWCCMessageData::CopyTo
void* __fastcall recovered_event_construct_004d7540(void* event, void*);        // CScriptEvent ctor
void __fastcall recovered_event_integer_004d76b0(void*, void*, U, U);           // CSWCCMessageData::SetInteger
void __fastcall recovered_event_object_004d7780(void*, void*, U, U);            // CSWCCMessageData::SetObjectID
unsigned __fastcall kotor_fastcall_004d8230(void* array, void*, unsigned id, void** out); // CGameObjectArray::GetGameObject
void __fastcall recovered_creature_feedback_004ede10(void* creature, void*, U id, void* data); // SendFeedbackMessage
void __fastcall w5_creature_auto_close_gui_panels_004eded0(void* creature, void*, U arg); // AutoCloseGUIPanels
U __fastcall recovered_creature_incapacitated_004ef890(void* creature, void*);   // GetDeadTemp
void __fastcall w5_creature_broadcast_voice_chat_004f2520(void* creature, void*, U voice); // BroadcastVoiceChat
void __fastcall w5_creature_set_lock_orientation_to_object_004f34a0(void* creature, void*, U id, U force);
void __fastcall recovered_creature_update_action_queue_004f6f30(void* creature, void*); // UpdateActionQueue
U __fastcall recovered_effect_integer_00503690(void* effect, void*, U index);    // CGameEffect::GetInteger
void __fastcall recovered_effect_integer_005036a0(void* effect, void*, U index, U value); // CGameEffect::SetInteger
U __fastcall kotor_fastcall_00503a70(void* effect, void*);                       // GetEffectTypeScriptConstant
void* __fastcall recovered_effect_construct_00503e40(void* memory, void*, U flag); // CGameEffect(int)
void* __fastcall kotor_fastcall_00503f70(void* self, void*, void* source);       // CGameEffect(CGameEffect*)
void* __fastcall combat_reuse_faction_00513fc0(void* creature, void*);           // CSWSCreature::GetFaction
U __fastcall recovered_faction_npc_reputation_0052b420(void* self, void*, U a, U b); // GetNPCFactionReputation
void* __fastcall kotor_fastcall_00561080(void* player, void*);                   // CSWSPlayer::GetGameObject
U __fastcall recovered_party_member_00563740(void* table, void*, U id);          // CSWPartyTable::GetIsMember
void __fastcall recovered_party_distribute_experience_005653a0(void* party, void*, U xp, U flag); // DistributeExperience
U __fastcall recovered_cc_message_00568570(void* message, void*, U player, U type, void* data, U a4); // SendServerToPlayerCCMessage
void __fastcall kotor_fastcall_00587770(void* placeable, void*);                 // ClosePlaceableForAllPlayers
void* __fastcall w5_door_get_linked_object_00589580(void* door, void*);          // CSWSDoor::GetLinkedObject
void __fastcall recovered_door_state_00589600(void* door, void*, U state, U notify); // CSWSDoor::SetOpenState
void __fastcall kotor_fastcall_00594310(void* encounter, void*, float cr, U flag); // RemoveFromActiveCreatureCount
void* __fastcall kotor_fastcall_005a4c60(void* stats, void*, void* out);         // CSWSCreatureStats::GetFullName
U __fastcall recovered_stats_effect_query_005a6960(void* stats, void*, U kind, void* other); // GetEffectImmunity
I __fastcall combat_reuse_members_005be780(void* faction, void*, U** list);      // CSWSFaction::GetMemberList
void __fastcall w2x_faction_add_member_005bfa70(void* faction, void*, U id, U flag); // CSWSFaction::AddMember
void __fastcall gui_scene_void_return_0c_005c5630(void*, void*, unsigned, unsigned, unsigned); // 005C5630 ret 0Ch
U __fastcall frame_R4_005d0fc0(void* vm, void*, void* script, U id, U flag);     // CVirtualMachine::RunScript
int __fastcall net_layer_send_message_to_player_005d4cb0(void* net, void*, U player, const char* data, U size, U flags);
void __fastcall recovered_debug_timestamp_005e9410(void* self, void*, void* out); // CExoDebug::GetCurrentTimestamp
void* __fastcall frame_R4_005ed690(void* client, void*);                         // CClientExoApp::GetInGameGui
void* __fastcall kotor_fastcall_005ed700(void* client, void*);                   // GetClientOptions
void* __fastcall kotor_fastcall_005ed8b0(void* client, void*);                   // GetSWParty
U __fastcall kotor_fastcall_005eda50(void* client, void*, U id);                 // ServerToClientObjectId
void __fastcall kotor_fastcall_005edea0(void* client, void*, U id, U type, I value); // AddFloatyText
void __fastcall kotor_fastcall_005edee0(void* client, void*, U a, U b);          // SetAutoPaused
U __fastcall kotor_fastcall_005edf80(void* app, void*, U, U, U);                 // ChangeCharacterToNextLivingPartyMember
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void* creature, void*); // GetServerCreature
U __fastcall recovered_client_creature_set_lock_orientation_00610950(void* creature, void*, U id, U force);
void* __fastcall frame_GetCharacter_006346c0(void* party, void*, U index);       // CSWParty::GetCharacter
U __fastcall slot_return_zero_0063e7f0_a0(void* self, void*);                    // return_zero 0063E7F0
int __fastcall frame_ContainsPanel_0040bd70(void* mgr, void*, void* panel);      // CSWGuiManager::PanelExists
// ---------------- sibling effects-g3 part (claude_effects_g3_damage.cpp) ----------------
void __fastcall kotor_fastcall_004efef0(void* creature, void*, U state);         // SetAmbientAnimationState
// ---------------- defined here ----------------
U __fastcall kotor_fastcall_004e0ac0(void* self, void*, void* object, void* effect, U load_game);
U __fastcall kotor_fastcall_004e13c0(void* self, void*, void* object, void* effect, U load_game);
U __fastcall kotor_fastcall_004df420(void* self, void*, void* effect);
void __fastcall kotor_fastcall_004fb1e0(void* creature, void*);
double __fastcall kotor_fastcall_004f19e0(void* creature, void*);
void __fastcall kotor_fastcall_004ec500(void* creature, void*, void* data);
void __fastcall kotor_fastcall_005edc70(void* client, void*, U id);
void __fastcall kotor_fastcall_005f2fe0(void* internal, void*, U id);
void __fastcall kotor_fastcall_0062b150(void* gui, void*);
void __fastcall kotor_fastcall_004f28d0(void* creature, void*);
U __fastcall kotor_fastcall_0056b320(void* message, void*, U player);
}

namespace {
constexpr U k_invalid_id = 0x7f000000u;                                          // OBJECT_INVALID
constexpr U k_event_0a_007441dc = 0xa;                                           // .rdata 007441DC
constexpr U k_event_0b_007441e0 = 0xb;                                           // .rdata 007441E0
constexpr U k_type_creature_00746634 = 5;                                        // .rdata 00746634 (byte)
constexpr U k_type_placeable_00746638 = 9;                                       // .rdata 00746638 (byte)
constexpr U k_type_door_00746639 = 0xa;                                          // .rdata 00746639 (byte)
constexpr U k_lookup_ok_0074666c = 0;                                            // .rdata 0074666C (byte)
constexpr U16 k_subtype_0_007473b8 = 0;                                          // .rdata 007473B8 (word)
constexpr U16 k_subtype_3_007473bc = 3;                                          // .rdata 007473BC (word)
constexpr U16 k_subtype_1_007473c0 = 1;                                          // .rdata 007473C0 (word)
constexpr U16 k_subtype_4_007473c8 = 4;                                          // .rdata 007473C8 (word)
constexpr U k_three_00740c84 = 0x40400000u;                                      // .rdata 00740C84 3.0f
constexpr U k_thousand_0073d6fc = 0x447a0000u;                                   // .rdata 0073D6FC 1000.0f
constexpr U k_percent_0073dfec = 0x3c23d70au;                                    // .rdata 0073DFEC 0.01f
constexpr U k_one_0073d764 = 0x3f800000u;                                        // .rdata 0073D764 1.0f
constexpr U k_zero_0073d700 = 0;                                                 // .rdata 0073D700 0.0f
constexpr U k_range_sq_007455bc = 0x44610000u;                                   // .rdata 007455BC 900.0f
const char k_destroy_delay_00746ac4[] = "DestroyObjectDelay";                    // .rdata 00746AC4
const char k_pc_00746abc[] = "(PC)";                                             // .rdata 00746ABC
const char k_effect_type_007469e8[] = "EffectType";                              // .rdata 007469E8
const char k_percent_xp_00746f08[] = "PercentXP";                                // .rdata 00746F08

inline void* app() { return kotor_menu_window_cells::manager_007a39fc; }
inline void* server() { return at<void*>(app(), 8); }                            // [[7A39FC]+8] CServerExoApp*
inline void* client() { return at<void*>(app(), 4); }                            // [[7A39FC]+4] CClientExoApp*
inline void* rules() { return kotor_cell_rules_007a3a28; }
inline void* two_da() { return at<void*>(rules(), 0xb8); }                       // CSWRules +B8 CTwoDimArrays*
inline void* ai_master() { return recovered_server_ai_master_004aed80(server(), nullptr); }
inline void* vm() { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::message_manager_007a3a00)); }
inline void* as_creature(void* o) { return vcall<void*>(o, 0x30); }              // vtbl +30 AsSWSCreature
inline short current_hp(void* o, U a) { return vcall<short>(o, 0x9c, a); }       // vtbl +9C GetCurrentHitPoints
inline void set_animation(void* o, U a) { vcall<void>(o, 0x7c, a); }             // vtbl +7C
inline I eff_int(void* e, U i) { return static_cast<I>(recovered_effect_integer_00503690(e, nullptr, i)); }
inline U volatile_read(const void* base, U offset) {                             // unchecked original read (may be null)
    return *reinterpret_cast<const volatile U*>(reinterpret_cast<std::uintptr_t>(base) + offset);
}
inline void* new_message() {                                                     // operator new(34h) + CSWCCMessageData()
    void* m = live_operator_new(0x34);
    return m != nullptr ? recovered_message_data_construct_004d69f0(m, nullptr) : nullptr;
}
inline void* new_script_event() {                                                // operator new(34h) + CScriptEvent()
    void* m = live_operator_new(0x34);
    return m != nullptr ? recovered_event_construct_004d7540(m, nullptr) : nullptr;
}
inline void delete_message(void* m) {                                            // ~CSWCCMessageData + free
    if (m == nullptr) return;
    recovered_message_data_destroy_004d6a40(m, nullptr);
    live_free(m);
}
}

// ===================================================================================== 004E0AC0 OnApplyDeath
U __fastcall kotor_fastcall_004e0ac0(void* self, void*, void* object, void* effect, U load_game) {
    const U creator = at<U>(effect, 0x18);                                       // 004E0AEB ebx
    if (at<U>(object, 0xf8) != 0) return 1;                                      // 004E0ADE..004E0AF2
    if (at<U>(object, 0x200) != 0) {                                             // 004E0AF8..004E0B02
        w1x_object_set_hp_004cc280(object, nullptr, 1);                          // 004E0B04..004E0B06
        return 1;                                                                // 004E0B0B
    }
    void* const client_object = recovered_object_render_004cc2b0(object, nullptr); // 004E0B10..004E0B16
    if (client_object != nullptr && vcall<void*>(client_object, 0x2c) != nullptr) { // 004E0B18..004E0B25
        (void)recovered_client_creature_set_lock_orientation_00610950(vcall<void*>(client_object, 0x2c), nullptr,
                                                                      k_invalid_id, 1); // 004E0B27..004E0B37
    }
    if (as_creature(object) != nullptr) {                                        // 004E0B3C..004E0B46
        w5_creature_set_lock_orientation_to_object_004f34a0(as_creature(object), nullptr, k_invalid_id, 1); // ..0B59
    }
    w5_object_clear_all_actions_004ccd80(object, nullptr, 1);                    // 004E0B5E..004E0B62
    void* const killer = kotor_fastcall_004ae770(server(), nullptr, creator);    // 004E0B67..004E0B7B [esp+10]
    const U type = at<unsigned char>(object, 8);                                 // 004E0B7F

    if (type == k_type_creature_00746634) {                                      // 004E0B75..004E0B84
        void* const c = as_creature(object);                                     // 004E0B8A..004E0B92 esi
        if (c == nullptr || at<U16>(c, 0x9f0) == 0) {                            // 004E0B94..004E0BA4
            (void)crt_rand_006fc447();                                           // 004E121A (result discarded)
            return 1;
        }
        kotor_fastcall_005edc70(client(), nullptr, at<U>(c, 4));                 // 004E0BAA..004E0BB7
        if ((at<unsigned char>(effect, 0xa) & 0x18) == 8 &&                     // 004E0BBC..004E0BC5
            recovered_stats_effect_query_005a6960(at<void*>(c, 0xa74), nullptr, 0x20, killer) != 0 && // ..0BDF
            at<I>(effect, 0x1c) != -1) {                                         // 004E0BE5..004E0BE9
            // Death immunity: feedback 7Fh to the victim (and a copy to the killer). The messages are handed over
            // to SendFeedbackMessage (no delete here).
            void* const message = new_message();                                 // 004E0BEF..004E0C0E edi
            recovered_event_object_004d7780(message, nullptr, 0, at<U>(object, 4)); // 004E0C10..004E0C20
            if (killer != nullptr) {                                             // 004E0C25..004E0C2B
                void* const copy = new_message();                                // 004E0C2D..004E0C54 ebp
                (void)kotor_fastcall_004d6ee0(message, nullptr, copy);           // 004E0C54..004E0C5F
                recovered_creature_feedback_004ede10(killer, nullptr, 0x7f, copy); // 004E0C64..004E0C69
            }
            recovered_creature_feedback_004ede10(c, nullptr, 0x7f, message);     // 004E0C6E..004E0C73
            return 1;                                                            // 004E0C78
        }
        gui_scene_void_return_0c_005c5630(ai_master(), nullptr, at<U>(c, 4), creator, 2); // 004E0C7D..004E0C94
        // [esp+10] (the killer slot) is overwritten with GetInteger(0), the "spectacular death" flag.
        I spectacular = eff_int(effect, 0);                                      // 004E0C99..004E0CA6
        (void)eff_int(effect, 1);                                                // 004E0CA2..004E0CAA (discarded)
        at<U>(c, 0x154) = creator;                                               // 004E0CB1
        kotor_fastcall_004fb1e0(c, nullptr);                                     // 004E0CAF..004E0CB7
        if (at<U>(c, 0xa88) == 0) {                                              // 004E0CBC..004E0CC4
            const U id = at<U>(c, 4);                                            // 004E0CCF
            void* const factions = kotor_fastcall_004aefc0(server(), nullptr);   // 004E0CC6..004E0CD2
            w2x_faction_add_member_005bfa70(at<void*>(factions, 0), nullptr, id, 0); // 004E0CD7..004E0CDE
        }
        if (at<U>(c, 0xec) == 0) spectacular = 0;                                // 004E0CE3..004E0CEF
        (void)kotor_fastcall_004d4620(at<void*>(c, 0x9c8), nullptr, 0);          // 004E0CF3..004E0CFA
        at<U>(at<void*>(c, 0x9c8), 0x9cc) = k_invalid_id;                        // 004E0CFF..004E0D05
        (void)frame_R4_005d0fc0(vm(), nullptr, static_cast<unsigned char*>(c) + 0x280, at<U>(c, 4), 1); // ..0D22
        const U state = at<unsigned char>(c, 0x4c4);                             // 004E0D27
        if (state != 3 && state != 4 && state != 0xe) {                          // 004E0D2D..004E0D37
            if (crt_rand_006fc447() % 2 != 0) {                                  // 004E0D39..004E0D4C
                kotor_fastcall_004efef0(c, nullptr, 3);                          // 004E0D4E..004E0D50
                set_animation(c, 0x2710);                                        // 004E0D55..004E0D5E
            } else {
                kotor_fastcall_004efef0(c, nullptr, 4);                          // 004E0D63..004E0D65
                set_animation(c, 0x2710);                                        // 004E0D6A..004E0D73
            }
        }
        at<U>(c, 0x19c) = 0;                                                     // 004E0D7C
        at<U16>(c, 0x9f0) = 0;                                                   // 004E0D82
        if (current_hp(c, 1) > -11) w1x_object_set_hp_004cc280(c, nullptr, 0xfffffff5u); // 004E0D76..004E0D99
        w5_creature_auto_close_gui_panels_004eded0(c, nullptr, 0);               // 004E0D9E..004E0DA1
        recovered_creature_update_action_queue_004f6f30(c, nullptr);             // 004E0DA6..004E0DA8
        // Strip every removable effect; a removal restarts the scan from index 0.
        for (I i = 0; i < at<I>(c, 0x128);) {                                    // 004E0DAD..004E0E11 (signed)
            void* const e = at<void**>(c, 0x124)[i];                             // 004E0DC0..004E0DC6
            const U16 sub = static_cast<U16>(at<U16>(e, 0xa) & 7);              // 004E0DC9..004E0DCD
            if (sub == k_subtype_4_007473c8 || sub == k_subtype_3_007473bc ||    // 004E0DD1..004E0DE1
                kotor_fastcall_004df420(self, nullptr, e) != 0) {                // 004E0DE3..004E0DED (ebx = handler)
                ++i;                                                             // 004E0E0A
                continue;
            }
            void* const r = at<void**>(c, 0x124)[i];                             // 004E0DEF..004E0DF5 (re-read)
            (void)recovered_object_remove_effect_004d06c0(c, nullptr, at<U>(r, 0), at<U>(r, 4)); // 004E0DF8..004E0E01
            i = 0;                                                               // 004E0E06
        }
        if (current_hp(c, 1) > -11) w1x_object_set_hp_004cc280(c, nullptr, 0xfffffff5u); // 004E0E13..004E0E29
        // DestroyObjectDelay (appearance 2DA, row = appearance type); 3.0 seconds when the entry is missing.
        float delay_seconds;                                                     // [esp+4C] (object arg slot)
        ExoString column;                                                        // [esp+14]
        frame_LogStringCtor_005e5a90(&column, nullptr, k_destroy_delay_00746ac4); // 004E0E2E..004E0E37
        const U found = c2da_current_get_float_00413430(at<void*>(two_da(), 0xc), nullptr, at<U16>(c, 0xa60), &column,
                                                        &delay_seconds);         // 004E0E3C..004E0E66
        frame_LogStringDtor_005e5c20(&column, nullptr);                          // 004E0E74..004E0E7E
        const double seconds = found == 0 ? static_cast<double>(flt(k_three_00740c84))
                                          : static_cast<double>(delay_seconds);  // 004E0E83..004E0E8F
        const float delay_ms = static_cast<float>(seconds * static_cast<double>(flt(k_thousand_0073d6fc))); // ..0E9B
        if (recovered_creature_incapacitated_004ef890(c, nullptr) == 0) {        // 004E0E99..004E0EA6
            if (spectacular == 0) {                                              // 004E0EAC..004E0EB2
                const U ms = w2::ftol2_006fae8c(static_cast<double>(delay_ms));  // 004E0EB8..004E0ED6
                (void)recovered_event_enqueue_004b08d0(ai_master(), nullptr, 0, ms, at<U>(effect, 0x18),
                    at<U>(object, 4), k_event_0b_007441e0, nullptr);             // 004E0EB4..004E0EDE + 004E1038..1041
            } else {
                // Spectacular death: visual effect 1773h on the creature, the destroy event, encounter bookkeeping.
                void* const memory = live_operator_new(0x8c);                    // 004E0EE3..004E0EED
                void* const vfx = memory != nullptr ? recovered_effect_construct_00503e40(memory, nullptr, 1) : nullptr; // ..0F0D
                at<U16>(vfx, 8) = 0x1e;                                          // 004E0F15
                at<U16>(vfx, 0xa) = static_cast<U16>((at<U16>(vfx, 0xa) & 0xfff8) | k_subtype_0_007473b8); // ..0F38
                recovered_effect_integer_005036a0(vfx, nullptr, 0, 0x1773);      // 004E0F1B..004E0F3C
                recovered_effect_apply_004d14f0(as_creature(c), nullptr, vfx, load_game, 0); // 004E0F41..004E0F52
                const U ms = w2::ftol2_006fae8c(static_cast<double>(delay_ms));  // 004E0F57..004E0F79
                (void)recovered_event_enqueue_004b08d0(ai_master(), nullptr, 0, ms, at<U>(effect, 0x18),
                    at<U>(object, 4), k_event_0b_007441e0, nullptr);             // 004E0F5B..004E0F8A
                const U encounter_id = at<U>(c, 0xa24);                          // 004E0F8F
                if (encounter_id != k_invalid_id) {                              // 004E0F95..004E0F9A
                    void* const encounter = kotor_fastcall_004ae7f0(server(), nullptr, encounter_id); // ..0FAA
                    if (encounter != nullptr) {                                  // 004E0FAF..004E0FB1
                        // The CR bits are also parked in the effect argument slot ([esp+58]); no later reader.
                        const float cr = at<float>(at<void*>(c, 0xa74), 0x84);   // 004E0FB7..004E0FCA
                        kotor_fastcall_00594310(encounter, nullptr, cr, 0);      // 004E0FC3..004E0FCE
                        at<U>(c, 0xa28) = 1;                                     // 004E0FD3
                    }
                }
            }
        } else {
            void* const module = server_get_module_004ae6b0(server(), nullptr);  // 004E0FDF..004E0FE8
            const U module_id = at<U>(module, 0x20);                             // 004E0FED
            void* const event = new_script_event();                              // 004E0FF3..004E1016
            at<U16>(event, 0) = 0xa;                                             // 004E1019
            (void)recovered_event_enqueue_004b08d0(ai_master(), nullptr, 0, 0, at<U>(c, 4), module_id,
                                                   k_event_0a_007441dc, event);  // 004E101E..004E1041
        }
        w5_creature_broadcast_voice_chat_004f2520(c, nullptr, 0x10);             // 004E1046..004E104A
        // Debug log line pieces (timestamp, full name [+ "(PC)"]); the print itself is compiled out of the original.
        ExoString stamp;                                                         // [esp+24]
        ExoString name;                                                          // [esp+1C]
        kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(&stamp), nullptr); // ..1053
        kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(&name), nullptr);  // ..1064
        recovered_debug_timestamp_005e9410(at<void*>(kotor_menu_10_19::dependencies::base_007a39e0, 8), nullptr,
                                           &stamp);                              // 004E1069..004E107D
        ExoString full;                                                          // [esp+2C]
        (void)async_assign_string_005e5c50(&name, nullptr,
                                           kotor_fastcall_005a4c60(at<void*>(c, 0xa74), nullptr, &full)); // ..109C
        frame_LogStringDtor_005e5c20(&full, nullptr);                            // 004E10A1..004E10A9
        if (at<U>(c, 0x9d4) != 0) {                                              // 004E10AE..004E10B6
            ExoString pc;                                                        // [esp+2C]
            ExoString sum;                                                       // [esp+34]
            frame_LogStringCtor_005e5a90(&pc, nullptr, k_pc_00746abc);           // 004E10B8..004E10C1
            void* const joined = GuiConcat005e5d10(reinterpret_cast<kotor_reconstruction::StartupString*>(&name), nullptr,
                reinterpret_cast<kotor_reconstruction::StartupString*>(&sum),
                reinterpret_cast<const kotor_reconstruction::StartupString*>(&pc)); // 004E10C6..004E10D9
            (void)async_assign_string_005e5c50(&name, nullptr, joined);          // 004E10DE..004E10E8
            frame_LogStringDtor_005e5c20(&sum, nullptr);                         // 004E10ED..004E10F6
            frame_LogStringDtor_005e5c20(&pc, nullptr);                          // 004E10FB..004E1103
        }
        void* const options = kotor_fastcall_005ed700(client(), nullptr);        // 004E1108..004E1110
        if ((at<U>(options, 0x14) & 0x4000) != 0 && at<U>(c, 0xa88) != 0) {      // 004E1115..004E1129
            U any_alive = 0;                                                     // 004E1138 ebx
            if (at<I>(kotor_fastcall_005ed8b0(client(), nullptr), 0) > 0) {      // 004E112F..004E1144
                I i = 0;                                                         // 004E113A edi
                do {
                    void* const member = frame_GetCharacter_006346c0(kotor_fastcall_005ed8b0(client(), nullptr), nullptr,
                                                                     static_cast<U>(i)); // 004E1146..004E1157
                    if (member != nullptr &&                                     // 004E115C..004E1160
                        kotor_m15a_creature_get_server_creature_0060fb20(member, nullptr) != nullptr && // ..116B
                        recovered_creature_incapacitated_004ef890(
                            kotor_m15a_creature_get_server_creature_0060fb20(member, nullptr), nullptr) == 0) { // ..117D
                        any_alive = 1;                                           // 004E117F
                    }
                    ++i;                                                         // 004E118C
                } while (i < at<I>(kotor_fastcall_005ed8b0(client(), nullptr), 0)); // 004E1184..004E1194
            }
            if (any_alive != 0) kotor_fastcall_005edee0(client(), nullptr, 1, 9); // 004E1196..004E11A7
        }
        const U id = at<U>(c, 4);                                                // 004E11B5
        if (kotor_fastcall_004aea40(server(), nullptr) == id) {                  // 004E11AC..004E11BF
            (void)kotor_fastcall_005edf80(client(), nullptr, 1, 1, 1);           // 004E11C1..004E11CF
            kotor_fastcall_0062b150(frame_R4_005ed690(client(), nullptr), nullptr); // 004E11D4..004E11E4
        }
        (void)WindowCStr005e5670(&stamp, nullptr);                               // 004E11E9..004E11ED
        (void)WindowCStr005e5670(&name, nullptr);                                // 004E11F2..004E11F6
        frame_LogStringDtor_005e5c20(&name, nullptr);                            // 004E11FB..004E1204
        frame_LogStringDtor_005e5c20(&stamp, nullptr);                           // 004E1209..004E1215
        (void)crt_rand_006fc447();                                               // 004E121A (result discarded)
        return 1;                                                                // 004E121F -> 004E1399
    }

    if (type != k_type_placeable_00746638 && type != k_type_door_00746639) return 1; // 004E1224..004E1236
    if (type == k_type_placeable_00746638) {                                     // 004E1232 / 004E123C
        if (vcall<void*>(object, 0x48) != nullptr) {                             // 004E1240..004E124A AsSWSPlaceable
            kotor_fastcall_00587770(vcall<void*>(object, 0x48), nullptr);        // 004E124C..004E1256
        }
        set_animation(object, 0x2758);                                           // 004E125B..004E1265
    } else {                                                                     // 004E126D door
        recovered_door_state_00589600(vcall<void*>(object, 0x18), nullptr, 3, 1); // 004E1275..004E1283 AsSWSDoor
        void* const linked = w5_door_get_linked_object_00589580(vcall<void*>(object, 0x18), nullptr); // ..1292
        if (linked != nullptr && vcall<void*>(linked, 0x18) != nullptr &&        // 004E1297..004E12A6
            at<U>(linked, 0xd4) != 0x2758) {                                     // 004E12A8..004E12B2
            void* const memory = live_operator_new(0x8c);                        // 004E12B4..004E12BE
            void* const death = memory != nullptr ? kotor_fastcall_00503f70(memory, nullptr, effect) : nullptr; // ..12DD
            at<U16>(death, 8) = 0x13;                                            // 004E12ED (EFFECT death)
            recovered_effect_integer_005036a0(death, nullptr, 0, 0);             // 004E12DF..004E12F3
            recovered_effect_integer_005036a0(death, nullptr, 1, 1);             // 004E12F8..004E12FE
            recovered_effect_apply_004d14f0(linked, nullptr, death, load_game, 0); // 004E1303..004E130D
        }
    }
    void* const event = new_script_event();                                      // 004E1312..004E1335
    at<U16>(event, 0) = 0xa;                                                     // 004E1338
    (void)recovered_event_enqueue_004b08d0(ai_master(), nullptr, 0, 0, at<U>(effect, 0x18), at<U>(object, 4),
                                           k_event_0a_007441dc, event);          // 004E133D..004E1367
    (void)recovered_event_enqueue_004b08d0(ai_master(), nullptr, 0, 0x7d0, at<U>(effect, 0x18), at<U>(object, 4),
                                           k_event_0b_007441e0, nullptr);        // 004E136C..004E1394
    return 1;                                                                    // 004E1399..004E13B0
}

// ===================================================================================== 004E13C0 OnApplyResurrection
U __fastcall kotor_fastcall_004e13c0(void*, void*, void* object, void*, U) {
    if (at<U>(object, 0xf0) == 0) return 1;                                      // 004E13C5..004E13CD
    if (as_creature(object) == nullptr) return 1;                                // 004E13D3..004E13DC
    void* const c = as_creature(object);                                         // 004E13E2..004E13EB
    if (current_hp(c, 0) < 1) w1x_object_set_hp_004cc280(c, nullptr, 1);         // 004E13ED..004E1403
    w5_object_clear_all_actions_004ccd80(c, nullptr, 1);                         // 004E1408..004E140C
    at<U>(c, 0x19c) = 1;                                                         // 004E1413
    at<U16>(c, 0x9f0) = 0xffff;                                                  // 004E141D
    at<U>(c, 0xe8) = 1;                                                          // 004E1426
    kotor_fastcall_004f28d0(c, nullptr);                                         // 004E1430
    void* const player = kotor_fastcall_004ae910(server(), nullptr, at<U>(object, 4)); // 004E1435..004E1442
    if (player != nullptr) {                                                     // 004E1447..004E144B
        (void)kotor_fastcall_0056b320(recovered_server_sws_message_004aedb0(server(), nullptr), nullptr,
                                      at<U>(player, 4));                         // 004E144D..004E1461
    }
    // Remove type-39h effects of sub-type 1; the list is sorted by type (a larger type ends the scan). A removal
    // still advances the index (the next entry shifts into the removed slot and is skipped, original quirk).
    for (I i = 0; i < at<I>(c, 0x128); ++i) {                                    // 004E1466..004E14B9 (signed)
        void* const e = at<void**>(c, 0x124)[i];                                 // 004E1480..004E1486
        const U16 kind = at<U16>(e, 8);                                          // 004E1489
        if (kind > 0x39) break;                                                  // 004E148D (unsigned ja)
        if (kind == 0x39 && static_cast<U16>(at<unsigned char>(e, 0xa) & 7) == k_subtype_1_007473c0) { // ..14A0
            (void)recovered_object_remove_effect_004d06c0(c, nullptr, at<U>(e, 0), at<U>(e, 4)); // 004E14A2..004E14AB
        }
    }
    return 1;                                                                    // 004E14BD..004E14C3
}

// ===================================================================================== 004DF420 IsEffectToIgnoreOnDeath
// CTwoDimArrays +124h table: any row whose "EffectType" equals the effect's script constant (or 21 for a beam
// visual effect, type 1Eh) keeps the effect on death. The row count is re-read every iteration.
U __fastcall kotor_fastcall_004df420(void*, void*, void* effect) {
    void* const table = at<void*>(two_da(), 0x124);                              // 004DF43E..004DF44C edi
    for (I row = 0; row < at<I>(table, 0x24); ++row) {                           // 004DF452..004DF4D9
        int value;                                                               // [esp+0C]
        ExoString column;                                                        // [esp+10]
        frame_LogStringCtor_005e5a90(&column, nullptr, k_effect_type_007469e8);  // 004DF460..004DF469
        const int ok = c2da_current_get_int_00413660(table, nullptr, row, &column, &value); // 004DF46E..004DF483
        frame_LogStringDtor_005e5c20(&column, nullptr);                          // 004DF488..004DF496
        if (ok == 0) continue;                                                   // 004DF49B..004DF49D
        if (static_cast<int>(kotor_fastcall_00503a70(effect, nullptr)) == value) return 1; // 004DF49F..004DF4AF
        if (value == 0x15 && at<U16>(effect, 8) == 0x1e &&                       // 004DF4B1..004DF4BB
            w2::g07::is_beam_type_004ca960(recovered_effect_integer_00503690(effect, nullptr, 0)) != 0) { // ..4D1
            return 1;                                                            // 004DF4F1..004DF507
        }
    }
    return 0;                                                                    // 004DF4DB..004DF4EE
}

// ===================================================================================== 004F19E0 CalculateDeathExperience
// XP table (CTwoDimArrays +C0h) at [PC level - 1][victim CR + 1], scaled by PercentXP row 9 (CTwoDimArrays +F0h) / 100.
// The first player's game object id is read through the player list without a null check (original quirk).
double __fastcall kotor_fastcall_004f19e0(void* creature, void*) {
    void* const list = recovered_server_player_list_004aed90(server(), nullptr); // 004F19EE..004F1A03
    void* const node = at<void*>(at<void*>(list, 0), 0);                         // 004F1A08..004F1A0A
    void* const player = node != nullptr ? at<void*>(node, 8) : nullptr;         // 004F1A0C..004F1A10
    void* const pc = kotor_fastcall_004ae770(server(), nullptr, volatile_read(player, 0x38)); // 004F1A13..004F1A20
    if (pc == nullptr) return static_cast<double>(flt(k_zero_0073d700));         // 004F1A25..004F1A27 / 004F1AFE..1B13
    const U experience = at<U>(at<void*>(pc, 0xa74), 0x68);                      // 004F1A2D..004F1A33
    // [esp+10]: one stack slot is the level fallback when the experience is below every threshold (it is not
    // written before the loop: stale contents, original quirk) and then the XP-table output.
    unsigned char slot[4];
    I level;                                                                     // edi
    I i = 0x14;                                                                  // 004F1A3E
    for (;;) {
        if (experience >= at<U>(rules(), 0x38 + static_cast<U>(static_cast<unsigned char>(i)) * 4)) { // ..1A4A (jae)
            level = i + 1;                                                       // 004F1A55
            break;
        }
        if (--i < 0) { std::memcpy(&level, slot, 4); break; }                    // 004F1A4C..004F1A53
    }
    const I column = static_cast<I>(w2::ftol2_006fae8c(
        static_cast<double>(at<float>(at<void*>(creature, 0xa74), 0x84)))) + 1;  // 004F1A58..004F1A6E
    float table_xp = 0.0f;                                                       // 004F1A6F (slot := 0)
    (void)kotor_m15b_c2da_get_float_entry_00413350(at<void*>(two_da(), 0xc0), nullptr, level - 1,
                                                   static_cast<U>(column), &table_xp); // 004F1A69..004F1A86
    float percent = 0.0f;                                                        // 004F1A94 [esp+0C]
    ExoString name;                                                              // [esp+14]
    frame_LogStringCtor_005e5a90(&name, nullptr, k_percent_xp_00746f08);         // 004F1A8B..004F1A9C
    (void)c2da_current_get_float_00413430(at<void*>(two_da(), 0xf0), nullptr, 9, &name, &percent); // ..1AC8
    frame_LogStringDtor_005e5c20(&name, nullptr);                                // 004F1ACD..004F1AD9
    return (static_cast<double>(percent) * static_cast<double>(flt(k_percent_0073dfec))) *
           static_cast<double>(table_xp);                                        // 004F1ADE..004F1AEB (ST0)
}

// ===================================================================================== 004FB1E0 ApplyDeathExperience
void __fastcall kotor_fastcall_004fb1e0(void* creature, void*) {
    void* const table = lane_config_required::GetPartyTable_004aee70(server(), nullptr); // 004FB1EE..004FB209 ebx
    if (at<U>(creature, 0x9d4) != 0) return;                                     // 004FB20B..004FB213
    if (table != nullptr && recovered_party_member_00563740(table, nullptr, at<U>(creature, 4)) != 0) return; // ..B22A
    const U faction_id = at<U>(at<void*>(creature, 0xa74), 0x78);                // 004FB230..004FB236
    if (static_cast<I>(recovered_faction_npc_reputation_0052b420(kotor_fastcall_004aefc0(server(), nullptr), nullptr,
                                                                 faction_id, 0)) > 10) return; // 004FB239..004FB254
    I percent = 0;                                                               // 004FB25D [esp+14]
    float xp = static_cast<float>(kotor_fastcall_004f19e0(creature, nullptr));   // 004FB25B..004FB26A [esp+10]
    ExoString name;                                                              // [esp+20]
    frame_LogStringCtor_005e5a90(&name, nullptr, k_percent_xp_00746f08);         // 004FB280..004FB289
    (void)c2da_current_get_int_00413660(at<void*>(two_da(), 0xf0), nullptr, 0xa, &name, &percent); // ..004FB2A4
    frame_LogStringDtor_005e5c20(&name, nullptr);                                // 004FB2A9..004FB2B5
    if (percent > 0) {                                                           // 004FB2BA..004FB2C0
        // Party bonus: xp *= percent * 0.01 * [table] + 1 ([table] read unchecked, original quirk).
        const I members = static_cast<I>(volatile_read(table, 0));               // 004FB2C6..004FB2C8
        const double scale = static_cast<double>(percent) * static_cast<double>(flt(k_percent_0073dfec)) *
                             static_cast<double>(members) + static_cast<double>(flt(k_one_0073d764)); // ..B2D6
        xp = static_cast<float>(static_cast<double>(xp) * scale);                // 004FB2DC..004FB2E2
    }
    const U award = w2::ftol2_006fae8c(std::ceil(static_cast<double>(xp)));      // 004FB2E8..004FB2FA (_ceil, __ftol2)
    recovered_party_distribute_experience_005653a0(table, nullptr, award, 0);    // 004FB2FF..004FB308
    void* found = nullptr;                                                       // [esp+0C] (written by the lookup)
    void* killer = nullptr;                                                      // ebx
    if (static_cast<unsigned char>(kotor_fastcall_004d8230(recovered_object_array_004aed70(server(), nullptr), nullptr,
                                                           at<U>(creature, 0x154), &found)) == k_lookup_ok_0074666c) { // ..B334
        void* source = found;                                                    // 004FB37C
        if (vcall<void*>(found, 0x40) != nullptr && at<U>(vcall<void*>(found, 0x40), 0x2e4) != k_invalid_id) { // ..B356
            source = kotor_fastcall_004ae770(server(), nullptr, at<U>(vcall<void*>(found, 0x40), 0x2e4)); // ..B376
        }
        if (source != nullptr) killer = as_creature(source);                     // 004FB380..004FB38B
    }
    if (killer == nullptr) {                                                     // 004FB38D..004FB38F / 004FB391
        killer = kotor_fastcall_004ae770(server(), nullptr, kotor_fastcall_004aea40(server(), nullptr)); // ..B3A8
    }
    if (killer != nullptr) {                                                     // 004FB3AA..004FB3AC
        void* const message = new_message();                                     // 004FB3B2..004FB3D7 / 004FB3F4..B419
        recovered_event_object_004d7780(message, nullptr, 0, at<U>(creature, 4)); // 004FB3D9..004FB3E9 / ..B42B
        recovered_event_object_004d7780(message, nullptr, 1, at<U>(killer, 4));  // 004FB3EE..004FB438
        recovered_event_integer_004d76b0(message, nullptr, 0, w2::ftol2_006fae8c(static_cast<double>(xp))); // ..B44B
        kotor_fastcall_004ec500(killer, nullptr, message);                       // 004FB450..004FB453
        delete_message(message);                                                 // 004FB458..004FB469
    }
    void* const client_app = client();                                           // 004FB470..004FB476
    const U client_id = kotor_fastcall_005eda50(client_app, nullptr, at<U>(creature, 4)); // 004FB479..004FB482
    kotor_fastcall_005edea0(client_app, nullptr, client_id, 3, static_cast<I>(award)); // 004FB46C..004FB48A
}

// ===================================================================================== 004EC500 BroadcastDeathDataToParty
// CC message type 4 to every faction member's player whose creature is in the same area within 30 m.
void __fastcall kotor_fastcall_004ec500(void* creature, void*, void* data) {
    if (combat_reuse_faction_00513fc0(creature, nullptr) == nullptr) return;     // 004EC502..004EC50B
    U* members = nullptr;                                                        // [esp+8]
    (void)combat_reuse_members_005be780(combat_reuse_faction_00513fc0(creature, nullptr), nullptr, &members); // ..C520
    for (I i = 0; i < at<I>(combat_reuse_faction_00513fc0(creature, nullptr), 4); ++i) { // 004EC525..004EC5F3
        void* const player = kotor_fastcall_004ae910(server(), nullptr, members[i]); // 004EC540..004EC551
        if (player == nullptr) continue;                                         // 004EC556..004EC55A
        void* const object = kotor_fastcall_00561080(player, nullptr);           // 004EC560..004EC562
        if (object == nullptr) continue;                                         // 004EC567..004EC569
        if (at<U>(creature, 0x8c) != at<U>(object, 0x8c)) continue;              // 004EC56F..004EC57B
        void* const o = kotor_fastcall_00561080(player, nullptr);                // 004EC57D..004EC57F
        const double dx = static_cast<double>(at<float>(o, 0x90)) - static_cast<double>(at<float>(creature, 0x90));
        const double dy = static_cast<double>(at<float>(o, 0x94)) - static_cast<double>(at<float>(creature, 0x94));
        const double dz = static_cast<double>(at<float>(o, 0x98)) - static_cast<double>(at<float>(creature, 0x98));
        const double d2 = (dz * dz + dy * dy) + dx * dx;                         // 004EC584..004EC5B5
        if (!(d2 <= static_cast<double>(flt(k_range_sq_007455bc)))) continue;    // 004EC5B7..004EC5C8 (jp: > or NaN)
        (void)recovered_cc_message_00568570(recovered_server_sws_message_004aedb0(server(), nullptr), nullptr,
                                            at<U>(player, 4), 4, data, 0);       // 004EC5CA..004EC5E3
    }
}

// ===================================================================================== 005EDC70 / 005F2FE0
void __fastcall kotor_fastcall_005f2fe0(void* internal, void*, U id) {
    if (id == k_invalid_id) return;                                              // 005F2FE5..005F2FEE
    void* const c = kotor_fastcall_004ae770(server(), nullptr, id);              // 005F2FF0..005F2FF9
    if (c == nullptr || at<U>(c, 0xa88) == 0) return;                            // 005F2FFE..005F300A
    at<U>(internal, 0x2d4) = kotor_fastcall_005eda50(client(), nullptr, id);     // 005F300C..005F301B
}
void __fastcall kotor_fastcall_005edc70(void* client_app, void*, U id) {
    kotor_fastcall_005f2fe0(at<void*>(client_app, 4), nullptr, id);              // 005EDC70..005EDC73
}

// ===================================================================================== 0062B150 RemoveGUIPopUps
// Four popup panels (+98h, +A0h, +8Ch, +54h): when the GUI manager (+38h) still holds one, panel vtbl +3C (28h, 1).
void __fastcall kotor_fastcall_0062b150(void* gui, void*) {
    static const U panels[4] = {0x98, 0xa0, 0x8c, 0x54};                         // 0062B153 / 0062B175 / 0062B197 / 0062B1B9
    for (U k = 0; k < 4; ++k) {
        if (frame_ContainsPanel_0040bd70(at<void*>(gui, 0x38), nullptr, at<void*>(gui, panels[k])) == 0) continue;
        vcall<void>(at<void*>(gui, panels[k]), 0x3c, 0x28u, 1u);
    }
}

// ===================================================================================== 004F28D0 RecomputeAmbientAnimationState
void __fastcall kotor_fastcall_004f28d0(void* creature, void*) {
    unsigned char& state = at<unsigned char>(creature, 0x4c4);
    if (vcall<U>(creature, 0x94) == 0 && slot_return_zero_0063e7f0_a0(creature, nullptr) == 0 && // 004F28D3..004F28EC
        recovered_creature_incapacitated_004ef890(creature, nullptr) == 0) {     // 004F28F2..004F28FB
        const U mode = at<unsigned char>(creature, 0x8ed);                       // 004F2901
        switch (mode) {                                                          // 004F2907..004F29F7
        case 6: state = 0xb; break;
        case 4: state = 2; break;
        case 7: state = 5; break;
        case 9: state = 9; break;
        case 0xc: state = 0xa; break;
        case 0xd: state = 0xd; break;
        case 0xa: state = 6; break;
        case 8: state = 7; break;
        case 3: state = 8; break;
        default: state = static_cast<unsigned char>(mode == 5 ? 0xc : 0); break; // 004F29E6..004F29F7 (setne/dec/and)
        }
        set_animation(creature, 0x2710);
        return;
    }
    if (state != 4 && state != 3 && state != 0xe) {                             // 004F2A04..004F2A14
        const U anim = at<U>(creature, 0xd4);                                    // 004F2A16
        state = static_cast<unsigned char>((anim == 0x279b || anim == 0x2785 || anim == 0x28a0) ? 0xe : 4); // ..2A35
    }
    set_animation(creature, 0x2710);                                             // 004F2A3B..004F2A44
}

// ===================================================================================== 0056B320 SendServerToPlayerDestroyDeathGUI
// 3-byte packet {50h, 24h, 02h} through the server's net layer (CServerExoApp vtbl +10h); always returns 1.
U __fastcall kotor_fastcall_0056b320(void* message, void*, U player) {
    unsigned char* const packet = static_cast<unsigned char*>(live_operator_new(3)); // 0056B322..0056B32B
    at<U>(message, 0x4c) = 0xffffffffu;                                          // 0056B332
    if (packet == nullptr) return 1;                                             // 0056B330..0056B339
    packet[0] = 0x50;                                                            // 0056B341
    packet[1] = 0x24;                                                            // 0056B344
    packet[2] = 2;                                                               // 0056B348
    void* const net = vcall<void*>(server(), 0x10);                              // 0056B34C..0056B35A
    (void)net_layer_send_message_to_player_005d4cb0(net, nullptr, player, reinterpret_cast<const char*>(packet), 3, 0); // ..B35F
    live_free(packet);                                                           // 0056B364..0056B36A
    return 1;                                                                    // 0056B36E..0056B374
}
