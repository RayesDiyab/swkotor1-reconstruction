// Lane VM package vmD (sub-file b): NWScript engine-command handlers CSWVirtualMachineCommands::ExecuteCommand*
// for ambient sound / alignment / journal / galaxy map / faction / lock actions / module access / music / sound
// objects / time / movies / party commands, plus the callees with no implementation in the c31 candidate.
// Reconstructed from the original swkotor.exe (read-only, capstone); comments give original instruction ranges.
//  * Handler: thiscall(int routine, int argc) RET 8 -> extern "C" int __fastcall kotor_fastcall_<addr>(self, edx,
//    routine, argc) (@16).  Returns 0 / -2001 (0xFFFFF82F) pop failure / -2000 (0xFFFFF830) push failure.
//  * Conventions: claude_vm_actions_common.hpp (not modified).  Game heap only.  No original address dereferenced.
//  * CSWVirtualMachineCommands: +4 "run-script object valid" flag, +8 OBJECT_SELF id.
// Not implemented here: 00542760 SetEncounterData (CSWSEncounter::SetActive -> SpawnIfAppropriate cascade, see report).
#include "061872cb05-claude_vm_actions_common.hpp"

namespace frame_control { extern std::uint32_t sound_manager_007a39ec; }   // g_pExoSound 007A39EC [0673]
namespace w2 { unsigned __cdecl ftol2_006fae8c(double value); }             // __ftol2 low dword [w2_ftol_native]
namespace options_video { struct Client; }
namespace client_intro { void __fastcall PlayMovieQueue_00602d40(options_video::Client* self, void*, int argument); } // CClientExoAppInternal::PlayMovieQueue RET 4 [0371]
namespace kotor_sound_path { void __fastcall SoundSetMusicVolume005d5c50(void* sound, void*, float volume, unsigned apply); } // CExoSound::SetMusicVolume RET 8 [0676]

extern "C" {
// ---- existing C-linkage providers (c31 map; object in brackets) ----
void* __fastcall kotor_fastcall_004ae780(void* server, void*, U id);                          // GetAreaByGameObjectID [0530]
void* __fastcall kotor_fastcall_004ae800(void* server, void*, U id);                          // GetSoundObjectByGameObjectID [claude_vm_actions_game]
void* __fastcall kotor_fastcall_004aede0(void* server, void*);                                // GetWorldTimer [0457]
void* __fastcall kotor_fastcall_004aefc0(void* server, void*);                                // GetFactionManager [claude_w5_creature_actions]
U __fastcall kotor_fastcall_004aea40(void* server, void*);                                    // GetPlayerCreatureId [0595]
void* __fastcall recovered_object_array_004aed70(void* server, void*);                        // GetObjectArray [w2_party_native]
void* __fastcall recovered_server_player_list_004aed90(void* server, void*);                  // GetPlayerList [w1_native_b7]
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);                  // GetSWSMessage [creature_destructor_r2]
void* __fastcall server_get_module_004ae6b0(void* server, void*);                             // GetModule [0530]
void* __fastcall kotor_fastcall_00561080(void* player, void*);                                // CSWSPlayer::GetGameObject [0595]
void* __fastcall unresolved_list_at_pos_005e9b60(void* list, void*, void* pos);               // CExoLinkedListInternal::GetAtPos [0168]
unsigned __fastcall unresolved_list_next_005e9b90(void* list, void*, void** pos);               // CExoLinkedListInternal::GetNext [0168]
void* __fastcall kotor_fastcall_004edb00(void* creature, void*);                              // CSWSCreature::GetJournal [claude_dialogue_server]
I __fastcall kotor_fastcall_005c58a0(void* journal, void*, void* tag);                        // CSWSJournal::GetState [claude_dialogue_server]
void __fastcall kotor_fastcall_005c5640(void* list, void*, I index);                          // CExoArrayList<SJournalEntry>::Remove [claude_s2p_small_journal2]
void __fastcall kotor_fastcall_00527490(void* list, void*, U count);                          // CExoArrayList<QuestUpdates>::Allocate [claude_dialogue_server]
void __fastcall kotor_fastcall_0056a4e0(void* msg, void*, void* player, U a, U end, void* entry); // SendServerToPlayerJournalUpdated [claude_dialogue_server]
void* __fastcall kotor_fastcall_004ae910(void* server, void*, U id);                          // GetClientObjectByObjectId [0595]
U __fastcall exostring_compare_no_case_005e6450(void* s, void*, const void* other);          // CExoString::CompareNoCase [0286]
void* __fastcall lower_construct_005e6080(void* s, void*, void* out);                         // CExoString::LowerCase [0310]
U __fastcall async_string_length_005e5790(void* s, void*);                                    // CExoString::GetLength [0188]
void* __fastcall exo_locstring_construct_005e9e10(void* l, void*);                            // CExoLocString() [0265]
void __fastcall exo_locstring_destroy_005ea000(void* l, void*);                               // ~CExoLocString [0265]
void* __fastcall recovered_localized_assign_005e9e70(void* dst, void*, void* src);            // CExoLocString::operator= [0886]
void __fastcall kotor_fastcall_004d68a0(void* message, void*, U size, U a, U b);              // CSWMessage::CreateWriteMessage [w2_message_native]
void __fastcall kotor_fastcall_004d68e0(void* message, void*, U value);                       // CSWMessage::WriteBOOL [w2_message_native]
void __fastcall kotor_fastcall_004d6340(void* message, void*, U value, U bits);               // CSWMessage::WriteDWORD [w2_message_native]
void __fastcall kotor_fastcall_004d6960(void* message, void*, const void* s, U bits);         // CSWMessage::WriteCExoString [w2_message_native]
U __fastcall kotor_fastcall_004d6440(void* message, void*, void** data, U* size);             // CSWMessage::GetWriteMessage [w2_message_native]
U __fastcall kotor_fastcall_00567e60(void* message, void*, U player, U major, U minor, void* data, U size); // SendServerToPlayerMessage [w2_message_native]
U __fastcall kotor_fastcall_005c9820(void* sound, void*, void* out);                          // CSWSAmbientSound::GetPlayersInArea [claude_0886_bodies_ambient]
U __fastcall kotor_fastcall_005eda50(void* client, void*, U id);                              // ServerToClientObjectId [0453]
void* __fastcall recovered_client_get_sound_object_005ed620(void* client, void*, U id);       // CClientExoApp::GetSoundObjectByGameObjectID [1204-01]
void __fastcall recovered_client_sound_object_fade_and_stop_0066b190(void* self, void*, U time); // CSWCSoundObject::FadeAndStop [claude_client_sound_object]
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, void* days, void* ms);         // CWorldTimer::GetWorldTime [0457]
void __fastcall kotor_fastcall_00563e80(void* table, void*, int index, int clear_actions);    // CSWPartyTable::SaveMember [claude_w1_route_bodies]
U __fastcall kotor_fastcall_00563b00(void* table, void*, U planet);                           // CSWPartyTable::GetPlanetSelectable [claude_gui_journal_galaxy]
void* __fastcall kotor_fastcall_004ae770(void* server, void*, U id);                          // GetCreatureByGameObjectID [0595]
U __fastcall kotor_fastcall_005a5110(void* stats, void*);                                     // GetSimpleAlignmentGoodEvil [claude_combat_attack_effectbonus]
void* __fastcall recovered_message_data_construct_004d69f0(void* data, void*);                // CSWCCMessageData() [w1_native_b7]
void __fastcall recovered_event_integer_004d76b0(void* data, void*, U index, U value);        // CSWCCMessageData::SetInteger [1017]
void __fastcall recovered_creature_feedback_004ede10(void* creature, void*, U id, void* data); // SendFeedbackMessage [w1_native_b7]
void* __fastcall frame_R4_005ed690(void* client, void*);                                      // CClientExoApp::GetInGameGui [0457]
void __fastcall kotor_fastcall_0062eeb0(void* gui, void*, U kind, U value);                   // CGuiInGame::UpdateStatus [src_hud]
U __fastcall recovered_stats_can_use_skill_005af880(void* stats, void*, U skill);             // GetCanUseSkill [w2_g07_native]
unsigned __fastcall recovered_object_add_action_004cea20(void* object, void*, unsigned type, unsigned group,
    unsigned t0, void* v0, unsigned t1, void* v1, unsigned t2, void* v2, unsigned t3, void* v3, unsigned t4, void* v4, unsigned t5, void* v5,
    unsigned t6, void* v6, unsigned t7, void* v7, unsigned t8, void* v8, unsigned t9, void* v9, unsigned t10, void* v10, unsigned t11, void* v11,
    unsigned t12, void* v12);                                                                 // CSWSObject::AddAction [w1_native_b7]
U __fastcall kotor_fastcall_004cfb80(void* object, void*, U target, U a, U b);                // AddUnlockObjectAction [producer_helpers]
void* __fastcall kotor_fastcall_004ca7a0(void* location, void*);                              // CScriptLocation() [claude_vm_actions_queries]
void* __fastcall kotor_fastcall_0052b3b0(void* manager, void*, I faction);                    // CFactionManager::GetFaction [claude_vm_actions_world]
U __fastcall recovered_area_first_object_005071b0(void* area, void*, U* out);                 // GetFirstObjectInArea [w2_update_native]
U __fastcall recovered_area_next_object_005071e0(void* area, void*, U* out);                  // GetNextObjectInArea [w2_update_native]
void* __fastcall recovered_object_area_004cb120(void* object, void*);                         // CSWSObject::GetArea [1060]
void __fastcall w2x_faction_add_member_005bfa70(void* faction, void*, U id, U flag);          // CSWSFaction::AddMember [0886]
void __fastcall recovered_creature_set_combat_state_004f2610(void* creature, void*, U state, U flag); // SetCombatState [w1_native_b7]
void __fastcall w5_object_clear_all_actions_004ccd80(void* object, void*, U flag);            // ClearAllActions [1206-07]
void __fastcall kotor_fastcall_004fd960(void* creature, void*);                               // PacifyCreature [claude_effects_g3_a]
void __fastcall kotor_fastcall_005178f0(void* creature, void*);                               // ClearVisibleList [claude_w1_route_bodies]
void __fastcall recovered_creature_update_visible_0051b050(void* creature, void*, unsigned mode); // UpdateVisibleList [w1_native_b7]

// CSWSCreatureStats::ModifyAlignment (+ UpdatePureGoodEvilPowers cascade) defined by sibling package vmC
void __fastcall kotor_fastcall_005ae610(void* stats, void*, U delta);              // RET 4 [claude_vm_vmC_creature]
// ---- callees reconstructed in this file ----
void* __fastcall kotor_fastcall_004aee70(void* server, void*);
void __fastcall kotor_fastcall_00563b20(void* table, void*, U planet, U available);
U __fastcall kotor_fastcall_004ae0d0(void* timer, void*);
void __fastcall kotor_fastcall_005ee220(void* client, void*, int argument);
void __fastcall kotor_fastcall_004af080(void* server, void*, U value);
void __fastcall kotor_fastcall_005c8d10(void* sound, void*, U time);
void __fastcall kotor_fastcall_0056b020(void* message, void*, U player, U night_flag, U volume);
void __fastcall kotor_fastcall_005c9ed0(void* sound, void*, I volume);
void __fastcall kotor_fastcall_005c9f80(void* sound, void*, I volume);
U __fastcall kotor_fastcall_004cfc20(void* object, void*, U target);
void __fastcall kotor_fastcall_0056a130(void* message, void*, void* player, void* tag);
void __fastcall kotor_fastcall_005c68b0(void* journal, void*, void* tag);
}

using namespace vmx;

namespace {
constexpr unsigned char k_found_0074666c = 0;     // .rdata 0074666C (byte): CGameObjectArray "found"

inline U rd(const void* p, U off) { return at<U>(const_cast<void*>(p), off); }
inline void* rp(const void* p, U off) { return at<void*>(const_cast<void*>(p), off); }
inline I push_int_result(U pushed) { return pushed ? 0 : k_push_failed; }   // neg/sbb/and 7D0/add F830
inline void* sws_message() { return recovered_server_sws_message_004aedb0(server_app(), nullptr); }
inline void* lookup_or(U id, bool* found) {
    void* object = nullptr;
    *found = static_cast<unsigned char>(kotor_fastcall_004d8230(recovered_object_array_004aed70(server_app(), nullptr),
                                                                nullptr, id, &object)) == k_found_0074666c;
    return object;
}

// 005C59A0 CExoArrayList<CSWSPlayerJournalQuestUpdates>::Add({WORD, CExoString} by value) RET 0Ch.  File-local
// adaptation (as claude_dialogue_server): the by-value CExoString is passed by address and destroyed here.
void quest_updates_add_005c59a0(void* list, U16 flags, ExoString* tag_by_value) {
    if (rd(list, 4) == rd(list, 8)) {                                                        // 005C59B8..005C59C6
        const U a = rd(list, 8);
        kotor_fastcall_00527490(list, nullptr, a != 0 ? a + a : 0x10);                       // 005C59C8..005C59D8
    }
    const U c = rd(list, 4);
    unsigned char* slot = static_cast<unsigned char*>(rp(list, 0)) + c * 0xc;                // 005C59DD..005C59E6
    at<U>(list, 4) = c + 1;                                                                  // 005C59E5..005C59E9
    at<U16>(slot, 0) = flags;                                                                // 005C59EC..005C59F5
    (void)async_assign_string_005e5c50(slot + 4, nullptr, tag_by_value);                     // 005C59F1..005C59FC
    exo_destroy(tag_by_value);                                                               // 005C5A01..005C5A0D
}

// shared body of SetAmbientDayVolume 005C9ED0 / SetAmbientNightVolume 005C9F80 (RET 4)
void ambient_volume(void* sound, I volume, U field, U day_flag) {
    void* message = sws_message();                                                           // 005C9EE8..005C9EFF
    void* players = nullptr;                                                                 // 005C9F0B (ebp = 0, [esp+14h] = 0)
    if (volume >= 0 && volume <= 0x64 &&                                                     // 005C9F0F..005C9F1E (signed)
        static_cast<I>(at<unsigned char>(sound, field)) != volume) {                         // 005C9F20..005C9F26
        at<unsigned char>(sound, field) = static_cast<unsigned char>(volume);                // 005C9F2F
        const I count = static_cast<I>(kotor_fastcall_005c9820(sound, nullptr, &players));   // 005C9F28..005C9F37
        for (I i = 0; i < count; ++i)                                                        // 005C9F3D..005C9F55
            kotor_fastcall_0056b020(message, nullptr, static_cast<U*>(players)[i], day_flag, static_cast<U>(volume)); // 005C9F41..4D
    }
    live_free(players);                                                                      // 005C9F57..005C9F60
}

}  // namespace

extern "C" {

// ============================================================================================ callees
// 004AEE70 CServerExoApp::GetPartyTable: [this+4] + 1B770h (RET 0)
void* __fastcall kotor_fastcall_004aee70(void* server, void*) {
    return static_cast<unsigned char*>(rp(server, 4)) + 0x1b770;                             // 004AEE70..004AEE78
}

// 004AF080 CServerExoApp::SetButtonMashCheck(int) RET 4
void __fastcall kotor_fastcall_004af080(void* server, void*, U value) {
    at<U>(rp(server, 4), 0x100d8) = value;                                                   // 004AF080..004AF087
}

// 00563B20 CSWPartyTable::SetPlanetAvailable(planet, value) RET 8: unsigned bound 10h
void __fastcall kotor_fastcall_00563b20(void* table, void*, U planet, U available) {
    if (planet < 0x10) at<U>(table, planet * 4 + 0x60) = available;                          // 00563B20..00563B2D (jae)
}

// 004AE0D0 CWorldTimer::GetWorldTimeMinute(): (ms / 1000 / 60) % minutes-per-hour byte [this+38h] (RET 0)
U __fastcall kotor_fastcall_004ae0d0(void* timer, void*) {
    U days = 0, ms = 0;
    frame_GetWorldTime_004ade40(timer, nullptr, &days, &ms);                                 // 004AE0D6..004AE0E2 ([esp+8] days, [esp+4] ms)
    const U per_hour = at<unsigned char>(timer, 0x38);                                       // 004AE0E7
    return ms / 1000u / 60u % per_hour;                                                      // 004AE0EB..004AE108 (unsigned div; 0 faults as original)
}

// 005EE220 CClientExoApp::PlayMovieQueue(int) RET 4: tail jump into the internal app
void __fastcall kotor_fastcall_005ee220(void* client, void*, int argument) {
    client_intro::PlayMovieQueue_00602d40(static_cast<options_video::Client*>(rp(client, 4)), nullptr, argument); // 005EE220..005EE223
}

// 005C8D10 CSWSSoundObject::FadeAndStop(time) RET 4
void __fastcall kotor_fastcall_005c8d10(void* sound, void*, U time) {
    const U id = rd(sound, 4);                                                               // 005C8D10
    at<U>(sound, 0x228) = 0;                                                                 // 005C8D13
    const U client_id = kotor_fastcall_005eda50(client_app(), nullptr, id);                  // 005C8D1D..005C8D27
    void* client_sound = recovered_client_get_sound_object_005ed620(client_app(), nullptr, client_id); // 005C8D2C..005C8D36
    if (client_sound) recovered_client_sound_object_fade_and_stop_0066b190(client_sound, nullptr, time); // 005C8D3B..005C8D41 (jmp)
}

// 0056B020 CSWSMessage::SendServerToPlayerAmbientSoundVolumeChange(player, bool, volume) RET 0Ch
void __fastcall kotor_fastcall_0056b020(void* message, void*, U player, U flag, U volume) {
    void* data = nullptr; U size = 0;
    kotor_fastcall_004d68a0(message, nullptr, 8, 0xffffffffu, 1);                            // 0056B024..0056B02C
    kotor_fastcall_004d68e0(message, nullptr, flag);                                         // 0056B031..0056B038
    kotor_fastcall_004d6340(message, nullptr, volume, 0x20);                                 // 0056B03D..0056B046
    if (!kotor_fastcall_004d6440(message, nullptr, &data, &size)) return;                    // 0056B04B..0056B05E
    (void)kotor_fastcall_00567e60(message, nullptr, player, 0x28, 8, data, size);            // 0056B067..0056B07C
}

// 005C9ED0 CSWSAmbientSound::SetAmbientDayVolume(int) RET 4 (byte +28h, message flag 1)
void __fastcall kotor_fastcall_005c9ed0(void* sound, void*, I volume) { ambient_volume(sound, volume, 0x28, 1); }
// 005C9F80 CSWSAmbientSound::SetAmbientNightVolume(int) RET 4 (byte +29h, message flag 0)
void __fastcall kotor_fastcall_005c9f80(void* sound, void*, I volume) { ambient_volume(sound, volume, 0x29, 0); }

// 004CFC20 CSWSObject::AddLockObjectAction(target) RET 4
U __fastcall kotor_fastcall_004cfc20(void* object, void*, U target) {
    if (vcall<void*>(object, 0x30) != nullptr &&                                             // 004CFC23..004CFC2A AsSWSCreature
        recovered_stats_can_use_skill_005af880(rp(vcall<void*>(object, 0x30), 0xa74), nullptr, 6) == 0) // 004CFC2C..004CFC44
        return 0;
    if (rd(object, 0xe8) == 0) return 0;                                                     // 004CFC46..004CFC52
    (void)recovered_object_add_action_004cea20(object, nullptr, 0x27, 0xffff,                // 004CFC56..004CFC96
        3, &target, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr,
        0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr);
    return 1;                                                                                // 004CFC9B
}

// 0056A130 CSWSMessage::SendServerToPlayerJournalRemoveQuest(CSWSPlayer*, CExoString const&) RET 8
void __fastcall kotor_fastcall_0056a130(void* message, void*, void* player, void* tag) {
    U size = async_string_length_005e5790(tag, nullptr);                                     // 0056A139..0056A13B
    void* data = nullptr;
    kotor_fastcall_004d68a0(message, nullptr, size, 0xffffffffu, 1);                         // 0056A140..0056A14B
    kotor_fastcall_004d6960(message, nullptr, tag, 0x20);                                    // 0056A150..0056A155
    if (!kotor_fastcall_004d6440(message, nullptr, &data, &size)) return;                    // 0056A15A..0056A16D (size slot reused)
    (void)kotor_fastcall_00567e60(message, nullptr, rd(player, 4), 0x1c, 7, data, size);     // 0056A175..0056A18D
}

// 005C68B0 CSWSJournal::Destroy(CExoString const& tag) RET 4
void __fastcall kotor_fastcall_005c68b0(void* journal, void*, void* tag) {
    void* message = sws_message();                                                           // 005C68C8..005C68E2 ([esp+14h])
    I index = static_cast<I>(rd(journal, 4)) - 1;                                            // 005C68D6, 005C68FC
    ExoString quest; exo_default(&quest);                                                    // 005C68DE..005C68E6 ([esp+2Ch], word [esp+28h])
    U16 quest_word = 0;                                                                      // (set before use at 005C6A08)
    alignas(4) unsigned char loc[8];
    (void)exo_locstring_construct_005e9e10(loc, nullptr);                                    // 005C68EB..005C68F7
    for (; index >= 0; --index)                                                              // 005C68FD..005C692A (signed)
        if (exostring_compare_no_case_005e6450(static_cast<unsigned char*>(rp(journal, 0)) + static_cast<U>(index) * 0x3c + 0x18,
                                               nullptr, tag) != 0) break;
    if (index >= 0) {
        unsigned char* entry = static_cast<unsigned char*>(rp(journal, 0)) + static_cast<U>(index) * 0x3c; // 005C6931..005C693C
        const U end_flag = rd(entry, 0x2c) & 1u;                                             // 005C6938..005C6946 (ebp)
        (void)recovered_localized_assign_005e9e70(loc, nullptr, entry);                      // 005C6941..005C694A
        kotor_fastcall_005c5640(journal, nullptr, index);                                    // 005C694F..005C6952
        void* server = server_app();                                                         // 005C6957..005C695D
        void* player = kotor_fastcall_004ae910(server, nullptr, kotor_fastcall_004aea40(server, nullptr)); // 005C6960..005C696F
        if (player) {                                                                        // 005C6971..005C6973
            void* updates = rp(player, 0x4c);                                                // 005C6979
            if (rd(updates, 0) != 0) {                                                       // 005C697C..005C697F
                ExoString lowered;
                (void)lower_construct_005e6080(tag, nullptr, &lowered);                      // 005C6981..005C698A
                kotor_fastcall_0056a130(message, nullptr, player, &lowered);                 // 005C698F..005C699A
                exo_destroy(&lowered);                                                       // 005C699F..005C69A8
            } else {
                if (rd(updates, 0x10) == 0) {                                                // 005C69B2..005C69B7
                    unsigned char* list = static_cast<unsigned char*>(updates) + 4;          // 005C69BD
                    if (list != nullptr) {                                                   // 005C69C0..005C69C2
                        const I count = static_cast<I>(rd(list, 4));                         // 005C69C8..005C69CF
                        I i = 0;
                        for (; i < count; ++i)                                               // 005C69CB..005C69F9 (signed)
                            if (exostring_compare_no_case_005e6450(static_cast<unsigned char*>(rp(list, 0)) + static_cast<U>(i) * 0xc + 4,
                                                                   nullptr, tag) != 0) break;
                        if (i < count) {
                            at<U16>(static_cast<unsigned char*>(rp(list, 0)) + static_cast<U>(i) * 0xc, 0) = 0x4000; // 005C6A9C..005C6AA5
                        } else {
                            ExoString lowered;
                            quest_word = 0x4000;                                             // 005C6A08
                            (void)lower_construct_005e6080(tag, nullptr, &lowered);          // 005C69FF..005C6A0F
                            (void)async_assign_string_005e5c50(&quest, nullptr, &lowered);   // 005C6A14..005C6A1E
                            exo_destroy(&lowered);                                           // 005C6A23..005C6A2C
                            ExoString arg;                                                   // 005C6A36..005C6A4A by-value {word, CExoString}
                            (void)WindowCopy005e5b00(&arg, nullptr, &quest);
                            quest_updates_add_005c59a0(list, quest_word, &arg);              // 005C6A4F..005C6A51
                        }
                    }
                }
                kotor_fastcall_0056a4e0(message, nullptr, player, 1, end_flag, loc);         // 005C6A56..005C6A63
            }
        }
    }
    exo_locstring_destroy_005ea000(loc, nullptr);                                            // 005C6A68..005C6A71
    exo_destroy(&quest);                                                                     // 005C6A76..005C6A82
}

// ============================================================================================ handlers
// 0052E270 AmbientSound family [433 AmbientSoundPlay, 434 AmbientSoundStop, 435 SetDayTrack, 436 SetNightTrack,
// 567 SetDayVolume, 568 SetNightVolume] (object oArea, int n): jump table on routine-1B1h
int __fastcall kotor_fastcall_0052e270(void*, void*, int routine, unsigned argc) {
    I value = 0;                                                                             // 0052E27E [esp+0]
    U area_id = 0;
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &area_id)) return k_pop_failed; // 0052E273..0052E28D
    if (static_cast<I>(argc) > 1 &&                                                          // 0052E28F..0052E294 (jle)
        !recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &value)) return k_pop_failed; // 0052E296..0052E2A8
    void* area = kotor_fastcall_004ae780(server_app(), nullptr, area_id);                    // 0052E2B5..0052E2C3
    if (!area) return 0;                                                                     // 0052E2C8..0052E2CA
    const U index = static_cast<U>(routine) - 0x1b1u;                                        // 0052E2D0..0052E2D4
    if (index > 0x87) return 0;                                                              // 0052E2DA..0052E2E0 (ja)
    void* sound = rp(area, 0x208);
    switch (routine) {                                                                       // 0052E2E6..0052E2ED (byte table 0052E3A4)
    case 433: vcall<void>(sound, 0x18, 1u); break;                                           // 0052E2F4..0052E300
    case 434: vcall<void>(sound, 0x18, 0u); break;                                           // 0052E30B..0052E317
    case 435: vcall<void>(sound, 0x1c, static_cast<U>(value)); break;                        // 0052E322..0052E331
    case 436: vcall<void>(sound, 0x20, static_cast<U>(value)); break;                        // 0052E33C..0052E34B
    case 567: kotor_fastcall_005c9ed0(sound, nullptr, value); break;                         // 0052E356..0052E361
    case 568: kotor_fastcall_005c9f80(sound, nullptr, value); break;                         // 0052E36E..0052E379
    default: break;                                                                          // table index 6 -> 0052E37E
    }
    return 0;
}

// 0052DFE0 AdjustAlignment(object oSubject, int nAlignment, int nShift) [201]
int __fastcall kotor_fastcall_0052dfe0(void*, void*, int, unsigned) {
    U subject = 0; I alignment = 0, shift = 0;                                               // [esp+8] / [esp+4] / [esp+0] (uninitialised)
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &subject)) return k_pop_failed;    // 0052DFFF..0052E00B
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &alignment)) return k_pop_failed; // 0052E011..0052E023
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &shift)) return k_pop_failed;     // 0052E029..0052E03B
    void* creature = kotor_fastcall_004ae770(server_app(), nullptr, subject);                // 0052E041..0052E053
    if (!creature) return 0;                                                                 // 0052E055..0052E068
    void* stats = rp(creature, 0xa74);                                                       // 0052E06D
    (void)kotor_fastcall_005a5110(stats, nullptr);                                           // 0052E074..0052E076 (result unused)
    const short good_evil = at<short>(stats, 0x17e);                                         // 0052E07F (bx)
    I delta = 0;                                                                             // 0052E086
    if (alignment == 1) {                                                                    // 0052E088..0052E089 ALIGNMENT_NEUTRAL
        delta = shift;                                                                       // 0052E09F
        I limit = 0x32;
        if (good_evil > 0x32) {                                                              // 0052E0A8..0052E0AB
            limit = 0x32 - static_cast<I>(good_evil);                                        // 0052E0AD..0052E0B0
            delta = -delta;                                                                  // 0052E0B2
            if (!(delta > limit)) delta = limit;                                             // 0052E0B4..0052E0C3
        } else {
            limit = 0x32 - static_cast<I>(good_evil);                                        // 0052E0BA..0052E0BD
            if (!(delta < limit)) delta = limit;                                             // 0052E0BF..0052E0C3
        }
    } else if (alignment == 2) {                                                             // 0052E08B..0052E08C
        delta = shift;                                                                       // 0052E099
    } else if (alignment == 3) {                                                             // 0052E08E..0052E08F
        delta = -shift;                                                                      // 0052E091..0052E095
    }
    kotor_fastcall_005ae610(stats, nullptr, static_cast<U>(delta));                          // 0052E0C5..0052E0C8
    void* memory = live_operator_new(0x34);                                                  // 0052E0CD..0052E0D4
    void* data = memory ? recovered_message_data_construct_004d69f0(memory, nullptr) : nullptr; // 0052E0DB..0052E0EC (null unchecked)
    I amount;
    if (alignment != 1) {                                                                    // 0052E0EE..0052E0FF
        recovered_event_integer_004d76b0(data, nullptr, 0, static_cast<U>(alignment));       // 0052E101..0052E104
        amount = shift;                                                                      // 0052E109
    } else {
        recovered_event_integer_004d76b0(data, nullptr, 0, good_evil > 0x32 ? 3u : 2u);      // 0052E10F..0052E11D (jle, signed word)
        amount = shift < 0 ? -shift : shift;                                                 // 0052E122..0052E129 cdq/xor/sub
    }
    recovered_event_integer_004d76b0(data, nullptr, 1, static_cast<U>(amount));              // 0052E12B..0052E130
    recovered_creature_feedback_004ede10(creature, nullptr, 0x6c, data);                     // 0052E135..0052E13A
    if (static_cast<short>(delta) != 0 && rd(creature, 0xa88) != 0)                          // 0052E13F..0052E14C
        kotor_fastcall_0062eeb0(frame_R4_005ed690(client_app(), nullptr), nullptr, 4,
                                static_cast<U>(static_cast<I>(static_cast<short>(delta))));  // 0052E14E..0052E164
    return 0;
}

// 0054AF60 RemoveJournalQuestEntry(string szPlotID) [368]
int __fastcall kotor_fastcall_0054af60(void*, void*, int, unsigned) {
    ExoString tag; exo_default(&tag);                                                        // 0054AF80..0054AF84
    if (!recovered_vm_outer_pop_string_005d1080(vm(), nullptr, &tag)) {                      // 0054AF89..0054AFA3
        exo_destroy(&tag); return k_pop_failed;                                              // 0054AFA5..0054AFB6
    }
    void* list = recovered_server_player_list_004aed90(server_app(), nullptr);               // 0054AFCE..0054AFDC
    if (!list) { exo_destroy(&tag); return 1; }                                              // 0054AFDE..0054AFF3 (quirk: returns 1)
    void* pos = rp(rp(list, 0), 0);                                                          // 0054B00B..0054B011
    while (pos) {                                                                            // 0054B015 / 0054B065..0054B067
        void* player = unresolved_list_at_pos_005e9b60(rp(list, 0), nullptr, pos);           // 0054B017..0054B01F
        if (player && kotor_fastcall_00561080(player, nullptr)) {                            // 0054B021..0054B02E
            void* creature = vcall<void*>(kotor_fastcall_00561080(player, nullptr), 0x30);   // 0054B030..0054B03B AsSWSCreature
            if (creature)                                                                    // 0054B03E..0054B040
                kotor_fastcall_005c68b0(kotor_fastcall_004edb00(creature, nullptr), nullptr, &tag); // 0054B042..0054B050
        }
        (void)unresolved_list_next_005e9b90(rp(list, 0), nullptr, &pos);                     // 0054B055..0054B05C
    }
    exo_destroy(&tag);                                                                       // 0054B069..0054B075
    return 0;
}

// 005467F0 SetGalaxyMapPlanetAvailable(int nPlanet, int bAvailable) [742]
int __fastcall kotor_fastcall_005467f0(void*, void*, int, unsigned) {
    I planet, available;
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &planet) ||                 // 005467F9..00546805 [esp+4]
        !recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &available)) return k_pop_failed; // 00546807..0054681B [esp]
    kotor_fastcall_00563b20(kotor_fastcall_004aee70(server_app(), nullptr), nullptr,
                            static_cast<U>(planet), static_cast<U>(available));             // 00546826..00546840
    return 0;
}

// 00546340 ChangeFactionByFaction / ChangeObjectFaction family [736, 737](int nFactionFrom, int nFactionTo)
int __fastcall kotor_fastcall_00546340(void* self, void*, int routine, unsigned) {
    I from, to;                                                                              // [esp+0Ch] / [esp+8]
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &from) ||                   // 00546344..00546358
        !recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &to)) return k_pop_failed;  // 0054635A..0054636E
    void* manager = kotor_fastcall_004aefc0(server_app(), nullptr);                          // 0054637A..00546389
    void* objects = recovered_object_array_004aed70(server_app(), nullptr);                  // 0054638B..00546393
    if (rd(self, 4) == 0) return 0;                                                          // 00546398..0054639D
    void* object = nullptr;
    if (static_cast<unsigned char>(kotor_fastcall_004d8230(objects, nullptr, rd(self, 8), &object)) != k_found_0074666c) // 005463A3..005463B9
        return 0;
    void* area;
    if (vcall<void*>(object, 0x10) != nullptr) {                                             // 005463BF..005463CF AsSWSObject
        area = recovered_object_area_004cb120(vcall<void*>(object, 0x10), nullptr);          // 005463D1..005463D8
    } else {
        if (vcall<void*>(object, 0x28) == nullptr) return 0;                                 // 005463DF..005463E6 AsSWSArea
        area = vcall<void*>(object, 0x28);                                                   // 005463EC..005463F2
    }
    if (!area || !manager) return 0;                                                         // 005463F5..00546401
    void* faction = kotor_fastcall_0052b3b0(manager, nullptr, to);                           // 00546407..0054640E (null unchecked)
    U id = 0;
    if (recovered_area_first_object_005071b0(area, nullptr, &id) != 1) return 0;             // 00546413..00546424
    do {
        void* creature = kotor_fastcall_004ae770(server_app(), nullptr, id);                 // 00546430..00546443
        if (creature) {                                                                      // 00546445..00546447
            (void)WindowCStr005e5670(static_cast<unsigned char*>(creature) + 0x18, nullptr); // 00546449..0054644C (unused)
            if (static_cast<I>(rd(rp(creature, 0xa74), 0x78)) == from) {                    // 00546451..0054645E
                w2x_faction_add_member_005bfa70(faction, nullptr, id, 0);                    // 00546460..00546469
                if (routine == 0x2e0) {                                                      // 0054646E..00546476
                    recovered_creature_set_combat_state_004f2610(creature, nullptr, 0, 1);   // 00546478..0054647C
                    w5_object_clear_all_actions_004ccd80(creature, nullptr, 1);              // 00546481..00546485
                    kotor_fastcall_004fd960(creature, nullptr);                              // 0054648A..0054648C
                } else {
                    kotor_fastcall_005178f0(creature, nullptr);                              // 00546493
                    recovered_creature_update_visible_0051b050(creature, nullptr, 0);        // 00546498..0054649C
                }
            }
        }
    } while (recovered_area_next_object_005071e0(area, nullptr, &id) == 1);                  // 005464A1..005464B0
    return 0;
}

// 0052CFF0 ActionUnlockObject [483] / ActionLockObject [484](object oTarget)
int __fastcall kotor_fastcall_0052cff0(void* self, void*, int routine, unsigned) {
    U target;
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &target)) return k_pop_failed; // 0052CFF2..0052D00F
    void* object = kotor_fastcall_004ae750(server_app(), nullptr, rd(self, 8));              // 0052D012..0052D024
    if (!object || vcall<void*>(object, 0x10) == nullptr) return 0;                          // 0052D026..0052D033 AsSWSObject
    if (routine == 0x1e3)                                                                    // 0052D035..0052D03E
        (void)kotor_fastcall_004cfb80(vcall<void*>(object, 0x10), nullptr, target, 0x7f000000u, 0); // 0052D040..0052D055
    else if (routine == 0x1e4)                                                               // 0052D061..0052D066
        (void)kotor_fastcall_004cfc20(vcall<void*>(object, 0x10), nullptr, target);          // 0052D068..0052D076
    return 0;
}

// 0053F800 module accessors [52 GetFirstPC?/0x34 +19Ch, 291 +16Ch, 410 +170h, 411 entry location, 419 +174h]
int __fastcall kotor_fastcall_0053f800(void*, void*, int routine, unsigned) {
    U location[6];                                                                           // CScriptLocation [esp+4..1Bh]
    (void)kotor_fastcall_004ca7a0(location, nullptr);                                        // 0053F819..0053F81D
    U id = k_invalid;                                                                        // 0053F832
    void* module = server_get_module_004ae6b0(server_app(), nullptr);                        // 0053F822..0053F837
    bool push_object = false, push_location = false;
    if (module) {                                                                            // 0053F83C..0053F842
        if (routine == 0x123) { id = rd(module, 0x16c); push_object = true; }               // 0053F848..0053F856
        else if (routine == 0x19a) { id = rd(module, 0x170); push_object = true; }          // 0053F858..0053F866
        else if (routine == 0x19b) {                                                         // 0053F868..0053F86E
            void* entry = rp(module, 0x58);
            location[0] = rd(entry, 0x10); location[1] = rd(entry, 0x14); location[2] = rd(entry, 0x18); // 0053F870..0053F88A
            location[3] = rd(entry, 0x1c); location[4] = rd(entry, 0x20);                    // 0053F88E..0053F89E (6th dword kept)
            push_location = true;                                                            // 0053F8A2..0053F8A8
        } else if (routine == 0x1a3) { id = rd(module, 0x174); push_object = true; }        // 0053F8EB..0053F8F9 -> 0053F8A2..B1
        else if (routine == 0x34) { id = rd(module, 0x19c); push_object = true; }           // 0053F8FB..0053F906
    } else {                                                                                 // 0053F908..0053F91D
        if (routine > 0x19a) {                                                               // jg -> 0053F8A2
            if (routine == 0x19b) push_location = true;
            else if (routine == 0x1a3) push_object = true;
        } else if (routine == 0x19a || routine == 0x34 || routine == 0x123) {
            push_object = true;
        }
    }
    I result = 0;
    if (push_object) {
        if (!recovered_vm_outer_push_object_005d10d0(vm(), nullptr, id)) result = k_push_failed; // 0053F8B3..0053F8D4
    } else if (push_location) {
        if (!recovered_vm_outer_push_engine_structure_005d10b0(vm(), nullptr, 2, location)) result = k_push_failed; // 0053F944..0053F96F
    }
    // ~CScriptLocation 004015A0 is a bare `ret` (0053F8CF / 0053F92B / 0053F966)
    return result;
}

// 00549F30 GetJournalEntry(string szPlotID) [369]
int __fastcall kotor_fastcall_00549f30(void*, void*, int, unsigned) {
    ExoString tag; exo_default(&tag);                                                        // 00549F49..00549F4D
    I state = 0;                                                                             // 00549F5C (ebx)
    if (!recovered_vm_outer_pop_string_005d1080(vm(), nullptr, &tag)) {                     // 00549F52..00549F6A
        exo_destroy(&tag); return k_pop_failed;                                              // 00549F6C..00549F7D
    }
    void* list = recovered_server_player_list_004aed90(server_app(), nullptr);               // 00549F94..00549FA3
    if (!list) { exo_destroy(&tag); return 1; }                                              // 00549FA5..00549FBB (quirk: returns 1)
    void* pos = rp(rp(list, 0), 0);                                                          // 00549FD2..00549FD8
    while (pos) {                                                                            // 00549FDC / 0054A01B
        void* player = unresolved_list_at_pos_005e9b60(rp(list, 0), nullptr, pos);           // 00549FE0..00549FE8
        if (player && kotor_fastcall_00561080(player, nullptr)) {                            // 00549FEA..00549FF7
            void* creature = vcall<void*>(kotor_fastcall_00561080(player, nullptr), 0x30);   // 00549FF9..0054A004
            if (creature) {                                                                  // 0054A007..0054A009
                state = kotor_fastcall_005c58a0(kotor_fastcall_004edb00(creature, nullptr), nullptr, &tag); // 0054A021..0054A034
                break;                                                                       // first PC creature only
            }
        }
        (void)unresolved_list_next_005e9b90(rp(list, 0), nullptr, &pos);                     // 0054A00B..0054A012
    }
    const U pushed = recovered_vm_outer_push_integer_005d1010(vm(), nullptr, state);         // 0054A037..0054A043
    exo_destroy(&tag);                                                                       // 0054A053 / 0054A070
    return pushed ? 0 : k_push_failed;
}

// 00547EF0 SetMusicVolume(float fVolume) [765]
int __fastcall kotor_fastcall_00547ef0(void*, void*, int, unsigned) {
    float volume;
    if (!recovered_vm_outer_pop_float_005d1020(vm(), nullptr, &volume)) return k_push_failed; // 00547EF0..00547F0B (quirk: -2000)
    if (0.0f > volume) volume = 0.0f;                                                        // 00547F0E..00547F1D, 00547F50 (ordered)
    else if (!(volume < 1.0f)) volume = 1.0f;                                                // 00547F1F..00547F30 (NaN -> 1.0)
    kotor_sound_path::SoundSetMusicVolume005d5c50(
        reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::sound_manager_007a39ec)), nullptr, volume, 1); // 00547F38..00547F45
    return 0;
}

// 00543E60 SoundObjectFadeAndStop(object oSound, float fSeconds) [745]
int __fastcall kotor_fastcall_00543e60(void*, void*, int, unsigned) {
    U sound_id; float seconds;
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &sound_id) ||                // 00543E66..00543E75 [esp+4]
        !recovered_vm_outer_pop_float_005d1020(vm(), nullptr, &seconds)) return k_pop_failed; // 00543E77..00543E8B [esp]
    const U ms = w2::ftol2_006fae8c(static_cast<double>(seconds) * 1000.0);                  // 00543E96..00543EA1 fmul [0073D6FC] (exact)
    void* sound = kotor_fastcall_004ae800(server_app(), nullptr, sound_id);                  // 00543EA6..00543EB6
    if (sound) kotor_fastcall_005c8d10(sound, nullptr, ms);                                  // 00543EBB..00543EC2
    return 0;
}

// 0053E0A0 GetTimeMinute() [17]
int __fastcall kotor_fastcall_0053e0a0(void*, void*, int, unsigned) {
    const U minute = kotor_fastcall_004ae0d0(kotor_fastcall_004aede0(server_app(), nullptr), nullptr); // 0053E0A0..0053E0AF
    return push_int_result(recovered_vm_outer_push_integer_005d1010(vm(), nullptr, static_cast<I>(minute))); // 0053E0B4..0053E0C9
}

// 00541170 PlaySound(string sSoundName) [46]: queue action 17h with the string parameter on OBJECT_SELF
int __fastcall kotor_fastcall_00541170(void* self, void*, int, unsigned) {
    ExoString sound; exo_default(&sound);                                                    // 0054118B..0054118F
    if (!recovered_vm_outer_pop_string_005d1080(vm(), nullptr, &sound)) {                    // 00541194..005411AE
        exo_destroy(&sound); return k_pop_failed;                                            // 005411B0..005411C1
    }
    if (rd(self, 4) == 1) {                                                                  // 005411D8..005411DC
        bool found = false;
        void* object = lookup_or(rd(self, 8), &found);                                       // 005411DE..00541202
        if (found && rd(object, 0xe8) != 0)                                                  // 00541204..00541210
            (void)recovered_object_add_action_004cea20(object, nullptr, 0x17, 0xffff,        // 00541212..00541250
                4, &sound, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr,
                0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr);
    }
    exo_destroy(&sound);                                                                     // 00541255..00541261
    return 0;
}

// 005482D0 PlayMovieQueue(int bAllowSkips) [770]
int __fastcall kotor_fastcall_005482d0(void*, void*, int, unsigned) {
    I allow;
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &allow)) return k_pop_failed; // 005482D0..005482EB
    kotor_fastcall_005ee220(client_app(), nullptr, allow);                                   // 005482EE..005482FC
    return 0;
}

// 005421A0 SaveNPCState(int nNPC) [734]
int __fastcall kotor_fastcall_005421a0(void*, void*, int, unsigned) {
    I npc;
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &npc)) return k_pop_failed;  // 005421A0..005421BB
    kotor_fastcall_00563e80(kotor_fastcall_004aee70(server_app(), nullptr), nullptr, npc, 0); // 005421BE..005421D5
    return 0;
}

// 00546A40 SetButtonMashCheck(int nCheck) [268]
int __fastcall kotor_fastcall_00546a40(void*, void*, int, unsigned) {
    I check = 0;                                                                             // 00546A4C
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &check)) return k_pop_failed; // 00546A41..00546A63
    kotor_fastcall_004af080(server_app(), nullptr, static_cast<U>(check));                   // 00546A66..00546A74
    return 0;
}

// 00546910 GetGalaxyMapPlanetSelectable(int nPlanet) [741]
int __fastcall kotor_fastcall_00546910(void*, void*, int, unsigned) {
    I planet;
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &planet)) return k_pop_failed; // 00546910..0054692B
    const U selectable = kotor_fastcall_00563b00(kotor_fastcall_004aee70(server_app(), nullptr), nullptr,
                                                 static_cast<U>(planet));                    // 0054692E..00546943
    return push_int_result(recovered_vm_outer_push_integer_005d1010(vm(), nullptr, static_cast<I>(selectable))); // 00546948..0054695D
}

// 00545C50 SetPartyAIStyle(int nStyle) [706]: party table +E4h
int __fastcall kotor_fastcall_00545c50(void*, void*, int, unsigned) {
    I style = 0;                                                                             // 00545C5C
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &style)) return k_pop_failed; // 00545C51..00545C73
    at<U>(kotor_fastcall_004aee70(server_app(), nullptr), 0xe4) = static_cast<U>(style);     // 00545C76..00545C89
    return 0;
}

}  // extern "C"
