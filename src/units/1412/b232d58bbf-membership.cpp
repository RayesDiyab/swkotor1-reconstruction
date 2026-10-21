// Lane W4 (party fork): client party membership -- 006364C0 CSWParty::AddCharacter, 0060EF50 CSWCCreature::SetInParty,
// 00635480 CSWParty::SetLeader, 006349F0 / 00634650 CSWParty::RemoveCharacter, 005FB6E0 CrownFutureLeader and the
// small client-app leaves they reach. Transcribed from dis/<ADDR>.asm; each statement cites its original PCs.
#include "7b616bc567-party.hpp"

using w4::U32;
namespace kotor_reconstruction { struct ArrayMutations; }
namespace menu_list_growth { void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*, void*, unsigned); } // CExoArrayList<4>::Allocate
extern "C" {
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void* creature, void*);          // CSWCCreature::GetServerCreature
unsigned __fastcall kotor_m15_client_get_player_creature_id_005ed550(void* app, void*);            // CClientExoApp::GetPlayerCreatureId
void* __fastcall kotor_fastcall_005ed8b0(void* app, void*);                                        // CClientExoApp::GetSWParty
int __fastcall kotor_m15_client_is_player_creature_005eda30(void* app, void*, void* creature);     // CClientExoApp::IsPlayerCreature
unsigned __fastcall kotor_m15_client_get_area_not_ready_005edad0(void* app, void*);                // CClientExoApp::GetAreaNotReadyToDisplay
void* __fastcall kotor_fastcall_005ed6f0(void* app, void*);                                        // CClientExoApp::GetSWCMessage (W4 HUD)
void __fastcall deleteAt_006857a0(void* list, void*, int index);                                   // CExoArrayList::deleteAt
void __fastcall kotor_fastcall_005ede80(void* app, void*, U32 mode);                               // CClientExoApp::SetCombatMode (W4 party)
void __fastcall kotor_fastcall_0062b110(void* in_game, void*, U32 message);                        // CGuiInGame::SetCombatMessage (W4 HUD)
std::int32_t __fastcall recovered_party_index_006349c0(void* party, void*, U32 id);                // CSWParty::GetIndex (W1)
int __fastcall recovered_party_table_leader_00563a00(void* table, void*, U32 server_id);           // CSWPartyTable::GetIsLeader (W1)
void __fastcall recovered_creature_update_visible_0051b050(void* creature, void*, U32 flag);       // CSWSCreature::UpdateVisibleList (W1)
void __fastcall recovered_set_game_object_00561c40(void* player, void*, void* object);             // CSWSPlayer::SetGameObject (W2)
// Cross-lane pending (integration/party.json).
void __fastcall recovered_transfer_inventory_005641e0(void* table, void*, void* creature);                       // CSWPartyTable::TransferInventory (W1)
void __fastcall kotor_fastcall_00563a40(void* table, void*, U32 server_id);                        // CSWPartyTable 00563A40 (sets +ECh leader index; W1)
void* __fastcall recovered_server_player_list_004aed90(void* server, void*);                                     // CServerExoApp::GetPlayerList (W1/W2)
void __fastcall kotor_fastcall_0051afd0(void* creature, void*, void* list);                        // CSWSCreature::SetVisibilityList (W1)
void __fastcall kotor_fastcall_00679340(void* control, void*, U32 object_id);                      // CSWPlayerControl::SetPlayer (W5)
void __fastcall kotor_fastcall_00677a20(void* message, void*, U32 open);                           // CSWCMessage::SendPlayerToServerGuiInventory_Status (W2)
void __fastcall kotor_fastcall_00677820(void* message, void*);                                     // CSWCMessage::SendPlayerToServerJournalMessage_QuestScreenOpen (W2)
void __fastcall recovered_object_add_action_004cea20(void* object, void*, U32 action, U32 group,                // CSWSObject::AddAction (W1), 28 words, RET 70h
    U32, U32, U32, U32, U32, U32, U32, U32, U32, U32, U32, U32, U32, U32, U32, U32, U32, U32, U32, U32, U32, U32, U32, U32, U32, U32);

// Definitions in this unit.
std::int32_t __fastcall kotor_fastcall_006364c0(void* party, void*, U32 id);                       // CSWParty::AddCharacter
U32 __fastcall kotor_fastcall_0060ef50(void* creature, void*, U32 in_party);                       // CSWCCreature::SetInParty
void __fastcall kotor_fastcall_00635480(void* party, void*, std::int32_t which);                   // CSWParty::SetLeader
U32 __fastcall kotor_fastcall_006349f0(void* party, void*, U32 id);                                // CSWParty::RemoveCharacter(id)
U32 __fastcall kotor_fastcall_00634650(void* party, void*, std::int32_t index);                    // CSWParty::RemoveCharacter(index)
void __fastcall kotor_fastcall_005ede00(void* app, void*, U32 id);                                 // CClientExoApp::SetFutureLeader
void __fastcall kotor_fastcall_005edd70(void* app, void*, U32 target, U32 flag);                   // CClientExoApp::SetLastTarget
void __fastcall kotor_fastcall_005f4a20(void* client, void*, U32 target, U32 flag);                // CClientExoAppInternal::SetLastTarget
void __fastcall kotor_fastcall_005eddb0(void* app, void*, void* position);                         // CClientExoApp::UpdateRoom
}

using namespace w4;

namespace {
void* rules_float_table() { return lp(lp(kotor_cell_rules_007a3a28, 0xB8), 0xC4); }
U8* slot(void* party, I32 i) { return static_cast<U8*>(party) + static_cast<U32>(i) * 0x88u; }
float rules_float(int row, const char* column) {                                          // CExoString + GetFLOATEntry + ~CExoString
    // Deviation (documented, t1 review): the original passes an UNINITIALISED stack float; if the 2DA row/column
    // were missing it would return stack garbage. W4 starts from 0.0f (C++ cannot read the indeterminate value
    // without UB). Identical whenever the row exists (all rows read here exist in the shipped rules 2DA).
    U32 text[2]; float value = 0.0f;
    frame_LogStringCtor_005e5a90(text, nullptr, column);
    c2da_current_get_float_00413430(rules_float_table(), nullptr, row, text, &value);
    frame_LogStringDtor_005e5c20(text, nullptr);
    return value;
}
int is_leader(void* server, U32 client_id) {                                                // CTS -> GetPartyTable -> GetIsLeader
    const U32 sid = kotor_fastcall_004aea30(server, nullptr, client_id);
    return recovered_party_table_leader_00563a00(lane_config_required::GetPartyTable_004aee70(server, nullptr), nullptr, sid);
}
void* first_player(void* server) {                                                          // [[GetPlayerList()]] -> node+8
    void* node = lp(lp(recovered_server_player_list_004aed90(server, nullptr), 0), 0);
    return node ? lp(node, 8) : nullptr;
}
const char k_primary_00746f28[] = "PrimaryRange";
const char k_secondary_00746f38[] = "SecondaryRange";
const char k_leadchng_007514b0[] = "k_hen_leadchng";
}

// 005EDE00 CClientExoApp::SetFutureLeader(id) (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_005ede00(void* app, void*, U32 id) { sw(lp(app, 4), 0x338, id); }  // 005EDE00..005EDE07
// 005F4A20 CClientExoAppInternal::SetLastTarget(id, int) (thiscall, RET 8).
extern "C" void __fastcall kotor_fastcall_005f4a20(void* client, void*, U32 target, U32 flag) {



    const U32 changed = lw(client, 0x2B4) == target ? 0u : 0xFFFFFFFFu;                        // 005F4A25..005F4A3C (sete; dec)

    sw(client, 0x2B4, target);                                                               // 005F4A36
    const U32 old = lw(client, 0x37C);
    sw(client, 0x37C, old ^ (((changed & flag) ^ old) & 1u));                                // 005F4A3D..005F4A4C
}
// 005EDD70 CClientExoApp::SetLastTarget / 005EDDB0 CClientExoApp::UpdateRoom (mov ecx,[ecx+4]; jmp internal).
extern "C" void __fastcall kotor_fastcall_005edd70(void* app, void*, U32 target, U32 flag) { kotor_fastcall_005f4a20(lp(app, 4), nullptr, target, flag); }
extern "C" void __fastcall kotor_fastcall_005eddb0(void* app, void*, void* position) { kotor_fastcall_005ee860(lp(app, 4), nullptr, position); }

// 00634650 CSWParty::RemoveCharacter(int index) (thiscall, RET 4; returns EAX of the lookup below).
extern "C" U32 __fastcall kotor_fastcall_00634650(void* party, void*, std::int32_t index) {
    if (kotor_menu_window_cells::manager_007a39fc == nullptr) return 0;                      // 00634653..0063465D
    U8* cell = slot(party, index) + 0x24;                                                    // 00634663..00634674
    const U32 result = frame_R4_005ed5b0(client_app(), nullptr, lw(cell, 0));                // 00634678..0063467B (result kept in EAX)
    if (index < 2) {                                                                         // 00634680..00634683
        for (I32 n = 2 - index; n != 0; --n, cell += 0x88) sw(cell, 0, lw(cell, 0x88));      // 00634685..0063469F
    }
    sw(party, 0x134, 0x7F000000u);                                                           // 006346A6
    sw(party, 0, lw(party, 0) - 1);                                                          // 006346A1..006346B0
    return result;
}
// 006349F0 CSWParty::RemoveCharacter(id) (thiscall, RET 4).
extern "C" U32 __fastcall kotor_fastcall_006349f0(void* party, void*, U32 id) {
    if (id == 0x7F000000u) return 0;                                                         // 006349F6..006349FC
    const I32 count = li(party, 0);
    for (I32 i = 0; i < count; ++i) {                                                        // 006349FE..00634A1D
        if (lw(slot(party, i), 0x24) != id) continue;
        if (i == -1) return 0;                                                               // 00634A26..00634A29
        return kotor_fastcall_00634650(party, nullptr, i) != 0 ? 1u : 0u;                    // 00634A2B..00634A3C
    }
    return 0;
}

// 006364C0 CSWParty::AddCharacter(id) (thiscall, RET 4).
extern "C" std::int32_t __fastcall kotor_fastcall_006364c0(void* party, void*, U32 id) {
    I32 result = -1;                                                                         // [esp+14h]
    if (id == 0x7F000000u) return -1;                                                        // 006364E4..00636501
    void* const server = server_app();                                                       // ebx
    void* const app = client_app();                                                          // [esp+18h]
    if (server == nullptr) return -1;                                                        // 0063650D..00636532
    void* sc = kotor_fastcall_004ae770(server, nullptr, kotor_fastcall_004aea30(server, nullptr, id)); // 0063651A..0063652A
    if (sc == nullptr) return -1;                                                            // 0063652C..00636532
    I32 free_slot = -1;
    for (I32 i = 0; i < 3; ++i) {                                                            // 00636537..0063655B
        const U32 occupant = lw(slot(party, i), 0x24);
        if (occupant == id) return i;                                                        // 00636546..00636568
        if (occupant == 0x7F000000u) { free_slot = i; break; }                               // 0063654A..0063654F
    }
    if (free_slot >= 0) {                                                                    // 0063656D..0063672B
        sw(slot(party, free_slot), 0x24, id);                                                // 00636584
        sw(slot(party, free_slot), 0x28, 0xFFFFFFFFu);                                       // 00636587
        if (free_slot == 0) {                                                                // 0063658F..00636591
            const float primary = rules_float(0x0C, k_primary_00746f28);                     // 0063657B..006365C8 [esp+14h]
            const float secondary = rules_float(0x0C, k_secondary_00746f38);                 // 006365CD..00636610 [esp+1Ch]
            kotor_fastcall_00677a20(kotor_fastcall_005ed6f0(app, nullptr), nullptr, 1);      // 00636615..00636624
            kotor_fastcall_00677820(kotor_fastcall_005ed6f0(app, nullptr), nullptr);         // 00636629..00636632
            sf(sc, 0x914, primary);                                                          // 00636637..0063663F
            sf(sc, 0x918, secondary);                                                        // 00636645
        } else {
            const float primary = rules_float(0x0B, k_primary_00746f28);                     // 00636650..00636685 [esp+20h]
            const float secondary = rules_float(0x0B, k_secondary_00746f38);                 // 0063668A..006366CC [esp+24h]
            sf(sc, 0x918, secondary);                                                        // 00636712
            sf(sc, 0x914, primary);                                                          // 0063671C
            recovered_object_add_action_004cea20(sc, nullptr, 0x3D, 0xFFFF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0);                                    // 006366D1..00636722
        }

        sw(party, 0, lw(party, 0) + 1);                                                      // 0063672B..0063672F

        result = free_slot;                                                                  // 00636731
    }
    if (id == kotor_m15_client_get_player_creature_id_005ed550(app, nullptr))                // 00636735..00636742
        recovered_transfer_inventory_005641e0(lane_config_required::GetPartyTable_004aee70(server, nullptr), nullptr, sc); // 00636744..0063674E
    if (kotor_m15_client_get_area_not_ready_005edad0(app, nullptr) != 0) {                   // 00636753..0063675F
        if (is_leader(server, id) != 0) {                                                    // 00636761..00636777
            kotor_fastcall_005ede00(app, nullptr, id);                                       // 00636779..0063677C
        } else if (is_leader(server, lw(party, 0x24)) == 0 && lw(lp(sc, 0xA74), 0x6C) != 0) { // 00636786..006367B3
            kotor_fastcall_00635480(party, nullptr, result);                                 // 006367B5..00636816
        }
    } else {
        if (is_leader(server, id) != 0)                                                      // 006367B9..006367CF
            kotor_fastcall_00635480(party, nullptr, result);                                 // 006367D1..00636816
        else if (is_leader(server, lw(party, 0x24)) == 0 && lw(lp(sc, 0xA74), 0x6C) != 0)    // 006367DC..0063680B
            kotor_fastcall_00635480(party, nullptr, result);                                 // 0063680D..00636816
    }
    sw(party, 0x18, 1);                                                                      // 0063681B..00636823
    return result;                                                                           // 0063681F
}

// 0060EF50 CSWCCreature::SetInParty(int) (thiscall, RET 4; returns AddCharacter/RemoveCharacter's EAX or 1).
extern "C" U32 __fastcall kotor_fastcall_0060ef50(void* creature, void*, U32 in_party) {
    if (lw(creature, 0x3A4) == in_party) return 1;                                           // 0060EF57..0060EF5D, 0060EFCB
    sw(creature, 0x3A4, in_party);                                                           // 0060EF61
    if (in_party != 0)                                                                       // 0060EF5F..0060EF67
        return static_cast<U32>(kotor_fastcall_006364c0(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr, lw(creature, 4))); // 0060EF69..0060EF7C
    if (kotor_m15_client_is_player_creature_005eda30(client_app(), nullptr, creature) != 0)  // 0060EF85..0060EF96
        kotor_fastcall_00635480(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr, -3); // 0060EF98..0060EFAA
    return kotor_fastcall_006349f0(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr, lw(creature, 4)); // 0060EFAF..0060EFC2
}

// 00635480 CSWParty::SetLeader(int which) (thiscall, RET 4). which: member index (>0), -2 = member 1,
// -3 = first member whose server creature has [[+A74h]+6Ch] set.
extern "C" void __fastcall kotor_fastcall_00635480(void* party, void*, std::int32_t which) {
    void* const app = client_app();                                                          // [esp+18h]
    void* const server = server_app();                                                       // [esp+8]
    const U32 old_leader = lw(party, 0x24);                                                  // [esp+0Ch]
    const I32 count = li(party, 0);
    if (count <= 1 || which == 0) return;                                                    // 006354AF..006354CC
    U32 leader;                                                                              // ebx / arg slot
    if (which == -2) {
        leader = lw(party, 0xAC);                                                            // 006354D9
    } else if (which == -3) {
        I32 i = 0;
        for (; i < 3; ++i) {                                                                 // 006354EA..00635521
            void* sc = kotor_fastcall_004ae770(server, nullptr, kotor_fastcall_004aea30(server, nullptr, lw(slot(party, i), 0x24)));
            if (sc != nullptr && lw(lp(sc, 0xA74), 0x6C) != 0) break;                        // 00635506..00635515
        }
        if (i == 3) return;                                                                  // 00635523
        leader = lw(slot(party, i), 0x24);                                                   // 00635528..00635538
    } else {
        if (which <= 0 || which >= count) return;                                            // 0063553E..00635548
        leader = lw(slot(party, which), 0x24);                                               // 0063554E..00635558
    }
    if (leader == 0x7F000000u) return;                                                       // 00635560..00635566
    U32 target = 0x7F000000u;                                                                // esi
    if (li(party, 0) > 0 && frame_R4_005ed5b0(client_app(), nullptr, lw(party, 0x24)) != 0) { // 0063556C..00635584
        void* c = li(party, 0) > 0 ? pt(frame_R4_005ed5b0(client_app(), nullptr, lw(party, 0x24))) : nullptr; // 00635586..0063559F
        if (kotor_m15a_creature_get_server_creature_0060fb20(c, nullptr) != nullptr) {       // 006355A1..006355AA
            void* c2 = li(party, 0) > 0 ? pt(frame_R4_005ed5b0(client_app(), nullptr, lw(party, 0x24))) : nullptr; // 006355AC..006355C5
            target = lw(kotor_m15a_creature_get_server_creature_0060fb20(c2, nullptr), 0x504); // 006355C7..006355CE
        }
    }
    sw(party, 0xA4, target);                                                                 // 006355D4
    kotor_fastcall_005edd70(client_app(), nullptr, target, 0);                               // 006355DA..006355E5
    while (lw(party, 0x24) != leader) {                                                      // 006355EA..00635643
        const U32 last_target = lw(party, 0xA4);                                             // [esp+18h]
        const U32 first = lw(party, 0x24);                                                   // esi
        I32 k = 0;
        for (;;) {                                                                           // 006355FE..00635626
            U8* cell = slot(party, k) + 0x24;
            const U32 next = lw(cell, 0x88);
            sw(cell, 0, next);
            sw(cell, 0x80, lw(cell, 0x108));
            if (next == 0x7F000000u) break;
            if (++k >= 2) break;
        }
        sw(slot(party, k), 0x24, first);                                                     // 00635628..00635638
        sw(slot(party, k), 0xA4, last_target);                                               // 0063563B
    }
    void* leader_creature = pt(frame_R4_005ed5b0(app, nullptr, leader));                    // 00635645..00635653 [esp+38h]
    if (leader_creature != nullptr)                                                          // 00635657
        kotor_fastcall_005eddb0(app, nullptr, static_cast<U8*>(leader_creature) + 0x24);    // 00635659..0063565F
    if (lw(party, 8) == 0) return;                                                           // 00635664..00635669
    const float primary0 = rules_float(0x0C, k_primary_00746f28);                            // 0063566F..006356B4 [esp+28h]
    const float secondary0 = rules_float(0x0C, k_secondary_00746f38);                        // 006356B9..006356FB [esp+2Ch]
    const float primary1 = rules_float(0x0B, k_primary_00746f28);                            // 00635700..00635742 [esp+30h]
    const float secondary1 = rules_float(0x0B, k_secondary_00746f38);                        // 00635747..00635789 [esp+34h]
    void* player = first_player(server);                                                     // 0063578E..006357AA [esp+18h]
    void* leader_sc = kotor_fastcall_004ae770(server, nullptr, kotor_fastcall_004aea30(server, nullptr, lw(party, 0x24))); // 006357AE..006357BB esi
    {
        const U32 sid = kotor_fastcall_004aea30(server, nullptr, leader);                    // 006357C0..006357CB
        kotor_fastcall_00563a40(lane_config_required::GetPartyTable_004aee70(server, nullptr), nullptr, sid); // 006357D0..006357DC
    }
    recovered_set_game_object_00561c40(player, nullptr, leader_sc);                          // 006357E1..006357E6
    sf(leader_sc, 0x914, primary0);                                                          // 006357EB..006357EF
    sf(leader_sc, 0x918, secondary0);                                                        // 006357F5..006357FD
    void* old_creature = pt(frame_R4_005ed5b0(client_app(), nullptr, old_leader));          // 00635803..0063580D
    if (old_creature != nullptr && old_leader != leader) {                                   // 00635812..00635822
        void* old_sc = kotor_m15a_creature_get_server_creature_0060fb20(old_creature, nullptr); // 00635828..0063582A [esp+18h]
        if (old_sc != nullptr) {                                                             // 0063582F..00635835
            U8* list = static_cast<U8*>(leader_sc) + 0x920;                                  // ebx
            const I32 n = li(leader_sc, 0x924);
            for (I32 i = n - 1; n > 0 && i >= 0; --i) {                                      // 0063583B..00635886
                void* item = pt(lw(lp(list, 0), static_cast<U32>(i) * 4u));                  // 00635860..00635862
                deleteAt_006857a0(list, nullptr, i);                                         // 00635865..0063586C
                if (item != nullptr) live_free(item);                                        // 00635871..0063587A
            }
            kotor_fastcall_0051afd0(leader_sc, nullptr, static_cast<U8*>(old_sc) + 0x920);    // 00635888..00635894
            sf(old_sc, 0x914, primary1);                                                     // 00635899..006358A1
            sf(old_sc, 0x918, secondary1);                                                   // 006358A7..006358AF
            kotor_fastcall_0051afd0(old_sc, nullptr, nullptr);                               // 006358AB..006358B5
            recovered_creature_update_visible_0051b050(old_sc, nullptr, 0);                  // 006358BA..006358BE
        }
    }
    if (old_leader != lw(party, 0x24)) {                                                     // 006358C6..006358CC
        U32 script[2];                                                                       // [esp+18h]
        frame_LogStringCtor_005e5a90(script, nullptr, k_leadchng_007514b0);                  // 006358CE..006358D7
        const U32 sid = kotor_fastcall_004aea30(server_app(), nullptr, lw(party, 0x24));     // 006358DC..006358F0
        frame_R4_005d0fc0(pt(frame_control::message_manager_007a3a00), nullptr, script, sid,
                          sid != 0x7F000000u ? 1u : 0u);                                     // 006358F5..0063590C
        sw(party, 0x94, 1); sw(party, 0x11C, 1); sw(party, 0x1A4, 1);                       // 00635911..00635926
        frame_LogStringDtor_005e5c20(script, nullptr);                                       // 00635916..00635930
    }
    kotor_fastcall_005edd70(client_app(), nullptr, lw(party, 0xA4), 0);                      // 0063593C..0063594E
    if (leader_creature == nullptr) return;                                                  // 00635953..00635959
    kotor_fastcall_005ede80(client_app(), nullptr, lw(leader_creature, 0x440) & 1u);         // 0063595B..0063596D
    if ((lw(leader_creature, 0x440) & 1u) == 0) return;                                      // 00635972..00635979
    const U32 message = lw(party, 0xA8);                                                     // 0063597B
    if (message == 0xBC50u) return;                                                          // 00635981..00635987
    kotor_fastcall_0062b110(pt(frame_R4_005ed690(client_app(), nullptr)), nullptr, message); // 00635989..0063599A
}

// 005FB6E0 CClientExoAppInternal::CrownFutureLeader() (thiscall, RET 0; returns 0/1).
extern "C" void __fastcall kotor_fastcall_005fb6e0(void* client, void*) {
    const U32 future = lw(client, 0x338);
    if (future == 0x7F000000u) return;                                                       // 005FB6FB..005FB70A (EAX 0)
    const I32 index = recovered_party_index_006349c0(lp(client, 0x270), nullptr, future);    // 005FB710..005FB71A
    void* const server = server_app();                                                       // edi
    void* new_sc = kotor_fastcall_004ae770(server, nullptr, kotor_fastcall_004aea30(server, nullptr, lw(client, 0x338))); // 005FB71F..005FB73A
    const U32 old_id = lw(client, 0x20);                                                     // [esp+10h]
    if (index > 0) {                                                                         // 005FB73F..005FB74A
        kotor_fastcall_00635480(lp(client, 0x270), nullptr, index);                          // 005FB74C..005FB753
    } else if (index == 0) {                                                                 // 005FB75A
        void* player = first_player(server);                                                 // 005FB75C..005FB772
        sw(client, 0x20, lw(client, 0x338));                                                 // 005FB77A..005FB780
        if (lp(client, 0x270) != nullptr) kotor_fastcall_006364c0(lp(client, 0x270), nullptr, lw(client, 0x338)); // 005FB772..005FB786
        recovered_set_game_object_00561c40(player, nullptr, new_sc);                         // 005FB78B..005FB78E
    }
    if (old_id != lw(client, 0x338)) {                                                       // 005FB793..005FB79D
        void* old_sc = kotor_fastcall_004ae770(server, nullptr, kotor_fastcall_004aea30(server, nullptr, old_id)); // 005FB7A3..005FB7AE
        // Swap the CSWSCreature visibility lists (+920h data, +924h count, +928h capacity) through a temporary.
        U32 temp[3] = {0, 0, 0};                                                             // [esp+18h..20h]
        U8* old_list = static_cast<U8*>(old_sc) + 0x920;                                     // edi
        const I32 n = li(old_sc, 0x924);                                                     // ebx
        if (n != 0) menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(temp), nullptr, static_cast<unsigned>(n)); // 005FB7C7..005FB7DA
        temp[1] = static_cast<U32>(n);                                                       // 005FB7E6
        for (I32 i = 0; i < li(old_list, 4); ++i)                                            // 005FB7DF..005FB7FE
            sw(pt(temp[0]), static_cast<U32>(i) * 4u, lw(lp(old_list, 0), static_cast<U32>(i) * 4u));
        sw(old_list, 4, 0);                                                                  // 005FB800
        const I32 m = li(new_sc, 0x924);                                                     // 005FB803 ebp
        if (m == 0) {
            if (lw(old_list, 0) != 0) { live_free(lp(old_list, 0)); sw(old_list, 0, 0); sw(old_list, 8, 0); } // 005FB80D..005FB81E
        } else {
            menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(old_list), nullptr, static_cast<unsigned>(m)); // 005FB823..005FB826
        }
        sw(old_list, 4, static_cast<U32>(m));                                                // 005FB82B
        for (I32 i = 0; i < li(new_sc, 0x924); ++i)                                          // 005FB82E..005FB857
            sw(lp(old_list, 0), static_cast<U32>(i) * 4u, lw(lp(new_sc, 0x920), static_cast<U32>(i) * 4u));
        sw(new_sc, 0x924, 0);                                                                // 005FB85B
        if (n == 0) {                                                                        // 005FB859..005FB865
            if (lw(new_sc, 0x920) != 0) { live_free(lp(new_sc, 0x920)); sw(new_sc, 0x920, 0); sw(new_sc, 0x928, 0); } // 005FB867..005FB882
        } else {
            void* previous = lp(new_sc, 0x920);                                              // 005FB88A
            sw(new_sc, 0x928, static_cast<U32>(n));                                          // 005FB898
            sw(new_sc, 0x920, ad(live_operator_new(static_cast<unsigned>(n) * 4u)));         // 005FB890..005FB8A9
            for (I32 i = 0; i < li(new_sc, 0x924); ++i)                                      // 005FB8A3..005FB8D5 (count is 0 here)
                sw(lp(new_sc, 0x920), static_cast<U32>(i) * 4u, lw(previous, static_cast<U32>(i) * 4u));
            if (previous != nullptr) live_free(previous);                                    // 005FB8D7..005FB8DC
        }
        sw(new_sc, 0x924, static_cast<U32>(n));                                              // 005FB8EC



        for (I32 i = 0; i < n; ++i)                                                          // 005FB8E4..005FB903

            sw(lp(new_sc, 0x920), static_cast<U32>(i) * 4u, lw(pt(temp[0]), static_cast<U32>(i) * 4u));
        live_free(pt(temp[0]));                                                              // 005FB905..005FB90E
    }
    kotor_fastcall_00679340(lp(client, 0x2A0), nullptr, lw(client, 0x338));                 // 005FB91A..005FB927
    sw(client, 0x338, 0x7F000000u);                                                          // 005FB92F
}
