// Lane p2s-a: CSWSMessage::HandlePlayerToServerPartyMessage 00526910 (routed by the 0918 player-message dispatcher)
// and the party/invitation callees that have no implementation in the candidate (c16 map).
// Transcribed from the original swkotor.exe (read-only); addresses in comments are the original instructions.
// Handler ABI: original BOOL __thiscall HandlePlayerToServerPartyMessage(CSWSPlayer* player, BYTE minor), RET 8,
// exported as the dispatcher calls handlers: extern "C" unsigned __fastcall f(void* message, void*, void* player, unsigned minor).
// Conventions as W5: thiscall -> extern "C" __fastcall(self, unused_edx, ...); live_operator_new / live_free only.
// EH: the original registers an SEH frame whose handlers only free a CSWCCMessageData whose constructor throws and
// destroy the two CExoString locals; none of the bound callees throws in normal operation, so the unwind path is
// not reproduced (as in the other W5 bodies).
#include "58f963adf7-w5_types.hpp"

using namespace w5;

namespace kotor_movie { void* __fastcall string_element_construct_005b3190(void* string, void*); } // CExoString() [0603]

extern "C" {
// ---------------- imports: existing providers in the c16 candidate (exact names) ----------------
unsigned __fastcall kotor_fastcall_004d6230(void* message, void*, unsigned);              // MessageReadOverflow (-> 0917 overrun)
unsigned __fastcall kotor_fastcall_00578540(void* message, void*);                        // CSWSMessage::ReadOBJECTIDServer
void* __fastcall kotor_fastcall_00561080(void* player, void*);                            // CSWSPlayer::GetGameObject [0595]
void* __fastcall kotor_fastcall_004ae770(void* server, void*, U id);                      // GetCreatureByGameObjectID [0595]
void* __fastcall kotor_fastcall_004ae910(void* server, void*, U id);                      // GetClientObjectByObjectId [0595]
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);              // GetSWSMessage
void* __fastcall kotor_fastcall_004aede0(void* server, void*);                            // GetWorldTimer [0457]
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, U* day, U* time);         // CWorldTimer::GetWorldTime [0457]
U __fastcall recovered_world_time_difference_004ae460(void* timer, void*, U day1, U time1, U day2, U time2,
                                                      U* out_day, U* out_time);           // CWorldTimer::SubtractWorldTimes
void* __fastcall combat_reuse_faction_00513fc0(void* creature, void*);                    // CSWSCreature::GetFaction
void* __fastcall recovered_message_data_construct_004d69f0(void* data, void*);            // CSWCCMessageData()
void __fastcall recovered_event_object_004d7780(void* data, void*, U index, U id);        // CSWCCMessageData::SetObjectID
void __fastcall recovered_creature_feedback_004ede10(void* creature, void*, U id, void* data); // SendFeedbackMessage
void __fastcall recovered_creature_set_invited_004ef520(void* creature, void*, unsigned value); // SetInvitedToParty
void* __fastcall kotor_fastcall_005a4c60(void* stats, void*, void* out);                  // CSWSCreatureStats::GetFullName
void __fastcall frame_LogStringDtor_005e5c20(void* string, void*);                        // ~CExoString [0459]
unsigned __fastcall recovered_faction_singleton_party_005bf690(void* faction, void*);     // CSWSFaction::GetSingletonParty
void __fastcall recovered_faction_remove_005bf920(void* faction, void*, unsigned id);     // CSWSFaction::RemoveMember
unsigned __fastcall recovered_faction_leader_005bf5c0(void* faction, void*);              // CSWSFaction::GetLeader
void __fastcall w2x_faction_add_member_005bfa70(void* faction, void*, unsigned id, unsigned flag); // CSWSFaction::AddMember
void __fastcall recovered_int_list_remove_0066dcc0(void* list, void*, unsigned value);    // CExoArrayList<int>::remove
void __fastcall w1x_pointer_list_insert_00616940(void* list, void*, unsigned value, unsigned index); // CExoArrayList::Insert
void __fastcall kotor_fastcall_0048dde0(void* list, void*, int index);                    // CExoArrayList<12-byte>::DelIndex
void __fastcall gui_load_event_list_allocate_004186e0(void* list, void*, int count);      // CExoArrayList<12-byte>::Allocate
unsigned __fastcall recovered_party_list_00567f90(void* message, void*, unsigned player, unsigned count, const void* list,
                                                  unsigned subtype, unsigned excluded);    // SendServerToPlayerParty_List
void __fastcall kotor_fastcall_004d68a0(void* message, void*, U size, U a, U b);          // CreateWriteMessage
void __fastcall kotor_fastcall_004d68e0(void* message, void*, U value);                   // WriteBOOL
void __fastcall kotor_fastcall_004d6340(void* message, void*, U value, U bits);           // WriteDWORD
void __fastcall kotor_fastcall_004d6960(void* message, void*, const void* string, U bits); // WriteCExoString
U __fastcall kotor_fastcall_004d6440(void* message, void*, void** data, U* size);         // GetWriteMessage
U __fastcall kotor_fastcall_00567e60(void* message, void*, U player, U major, U minor, void* data, U size); // SendServerToPlayerMessage

// ---------------- exports ----------------
U __fastcall kotor_fastcall_00518450(void* creature, void*, U id);
U __fastcall kotor_fastcall_00518530(void* creature, void*, U id);
U __fastcall kotor_fastcall_00518580(void* creature, void*, U id);
U __fastcall kotor_fastcall_005188c0(void* creature, void*);
void __fastcall kotor_fastcall_0051b580(void* creature, void*, U id);
void __fastcall kotor_fastcall_0051b650(void* creature, void*, U id);
U __fastcall kotor_fastcall_0051d060(void* creature, void*, U reply, U inviter_id);
U __fastcall kotor_fastcall_0056aac0(void* message, void*, U player_id, U panel, U flag1, U flag2, U value, const void* text);
U __fastcall kotor_fastcall_005be790(void* faction, void*, U invitee_id, U inviter_id);
U __fastcall kotor_fastcall_005bfa30(void* faction, void*, U new_leader, U requester);
U __fastcall kotor_fastcall_005bfb20(void* faction, void*, U target, U requester);
U __fastcall recovered_player_to_server_party_00526910(void* message, void*, void* player, U minor);
}  // extern "C"

namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }

namespace {
inline void* server() { return at<void*>(kotor_menu_window_cells::manager_007a39fc, 8); }   // [[007A39FC]+8] CServerExoApp
inline void* creature_by_id(U id) { return kotor_fastcall_004ae770(server(), nullptr, id); }
inline void* client_by_id(U id) { return kotor_fastcall_004ae910(server(), nullptr, id); }
inline void* as_creature(void* o) {                                                  // vtbl+30 AsSWSCreature
    using F = void*(__fastcall*)(void*, void*);
    return reinterpret_cast<F>((*static_cast<void***>(o))[0x30 / 4])(o, nullptr);
}
inline void* faction_of(void* c) { return combat_reuse_faction_00513fc0(c, nullptr); }
// operator new(34h) + CSWCCMessageData ctor (null when the allocation fails), then SetObjectID(0, id)
inline void* new_feedback_data(U id) {
    void* memory = live_operator_new(0x34);
    void* data = memory != nullptr ? recovered_message_data_construct_004d69f0(memory, nullptr) : nullptr;
    recovered_event_object_004d7780(data, nullptr, 0, id);                           // original calls it on a null data too
    return data;
}
inline void feedback(void* creature, U message, void* data) {
    recovered_creature_feedback_004ede10(creature, nullptr, message, data);
}
// Shared body of 00518450 / 00518580: list of {id, day, time} entries; entries older than `limit` ms are dropped.
U invitation_recent(void* creature, U offset, U id, U limit) {
    void* list = at<void*>(creature, offset);
    if (list == nullptr) return 0;
    I i = static_cast<I>(at<U>(list, 4)) - 1;                                         // count read once
    for (U entry = static_cast<U>(i) * 12; i >= 0; --i, entry -= 12) {
        unsigned char* e = at<unsigned char*>(at<void*>(creature, offset), 0) + entry;
        if (at<U>(e, 0) != id) continue;
        U now_time, now_day;                                                          // [esp+0C] / [esp+10]
        frame_GetWorldTime_004ade40(kotor_fastcall_004aede0(server(), nullptr), nullptr, &now_day, &now_time);
        e = at<unsigned char*>(at<void*>(creature, offset), 0) + entry;
        U diff_time, diff_day;                                                        // [esp+14] / [esp+18]
        recovered_world_time_difference_004ae460(kotor_fastcall_004aede0(server(), nullptr), nullptr, now_day, now_time,
                                                 at<U>(e, 4), at<U>(e, 8), &diff_day, &diff_time);
        if (diff_time <= limit) return 1;                                             // only the time part is compared
        kotor_fastcall_0048dde0(at<void*>(creature, offset), nullptr, i);
    }
    return 0;
}
// Shared body of 0051B580 / 0051B650: append {id, now_day, now_time} (list created on demand).
void invitation_add(void* creature, U offset, U id) {
    U day, time;                                                                      // [esp+10] / [esp+14]
    frame_GetWorldTime_004ade40(kotor_fastcall_004aede0(server(), nullptr), nullptr, &day, &time);
    if (at<void*>(creature, offset) == nullptr) {
        void* list = live_operator_new(0xc);
        if (list != nullptr) { at<U>(list, 4) = 0; at<U>(list, 8) = 0; at<U>(list, 0) = 0; }
        at<void*>(creature, offset) = list;
    }
    void* list = at<void*>(creature, offset);
    const U capacity = at<U>(list, 8);
    if (at<U>(list, 4) == capacity)
        gui_load_event_list_allocate_004186e0(list, nullptr, static_cast<int>(capacity != 0 ? capacity * 2 : 0x10));
    unsigned char* e = at<unsigned char*>(list, 0) + at<U>(list, 4) * 12;
    at<U>(e, 0) = id; at<U>(e, 4) = day; at<U>(e, 8) = time;
    at<U>(list, 4) = at<U>(list, 4) + 1;
}
}  // namespace

extern "C" {
// 00518450 CSWSCreature::GetIsInInvitationsOffered(id): list +93C, 30000 ms window. RET 4.
U __fastcall kotor_fastcall_00518450(void* creature, void*, U id) { return invitation_recent(creature, 0x93c, id, 0x7530); }
// 00518580 CSWSCreature::GetIsInInvitationsIgnored(id): list +940, 60000 ms window. RET 4.
U __fastcall kotor_fastcall_00518580(void* creature, void*, U id) { return invitation_recent(creature, 0x940, id, 0xea60); }

// 00518530 CSWSCreature::RemoveFromInvitationsOffered(id): delete every +93C entry with that id (descending). RET 4.
// Returns whatever EAX holds (the original is void); 0 here.
U __fastcall kotor_fastcall_00518530(void* creature, void*, U id) {
    void* list = at<void*>(creature, 0x93c);                                         // 00518533..3B
    if (list == nullptr) return 0;
    for (I i = static_cast<I>(at<U>(list, 4)) - 1; i >= 0; --i) {                    // 0051853E..69
        void* now = at<void*>(creature, 0x93c);
        if (at<U>(at<unsigned char*>(now, 0) + static_cast<U>(i) * 12, 0) == id)
            kotor_fastcall_0048dde0(now, nullptr, i);                                // 0051855D..5E
    }
    return 0;
}

// 005188C0 CSWSCreature::GetNumInvited(): +93C list count, 0 without a list. RET 0.
U __fastcall kotor_fastcall_005188c0(void* creature, void*) {
    void* list = at<void*>(creature, 0x93c);
    return list != nullptr ? at<U>(list, 4) : 0;
}

// 0051B580 CSWSCreature::AddToInvitationsOffered(id). RET 4.
void __fastcall kotor_fastcall_0051b580(void* creature, void*, U id) { invitation_add(creature, 0x93c, id); }
// 0051B650 CSWSCreature::AddToInvitationsIgnored(id). RET 4.
void __fastcall kotor_fastcall_0051b650(void* creature, void*, U id) { invitation_add(creature, 0x940, id); }

// 0051D060 CSWSCreature::ReplyToInvitation(reply, inviter id): 0 accept, 1 decline, 2 ignore. RET 8.
U __fastcall kotor_fastcall_0051d060(void* self, void*, U reply, U inviter_id) {
    void* inviter = creature_by_id(inviter_id);                                       // 0051D06F..75
    if (inviter == nullptr) return 0;
    if (kotor_fastcall_00518450(inviter, nullptr, at<U>(self, 4)) != 1) return 0;    // 0051D080..8E
    if (at<U>(self, 0x92c) != inviter_id) return 0;                                  // 0051D090..96
    if (reply == 0) {                                                                // 0051D098..9E -> 0051D0B0
        if (faction_of(inviter) == nullptr) return 0;                                // 0051D0B0..B9
        w2x_faction_add_member_005bfa70(faction_of(inviter), nullptr, at<U>(self, 4), 0); // 0051D0BB..CA
    } else if (reply == 2) {                                                         // 0051D0A3..A4
        kotor_fastcall_0051b650(self, nullptr, inviter_id);                          // 0051D0A6..A9
    } else if (reply != 1) {
        return 0;
    }
    kotor_fastcall_00518530(inviter, nullptr, at<U>(self, 4));                       // 0051D0CF..DA
    recovered_creature_set_invited_004ef520(self, nullptr, 0);                       // 0051D0DF..E3
    at<U>(self, 0x92c) = 0x7f000000u;                                                // 0051D0E9
    return 1;
}

// 0056AAC0 CSWSMessage::SendServerToPlayerPopUpGUIPanel(player id, panel, bool, bool, dword, CExoString const&). RET 18h.
U __fastcall kotor_fastcall_0056aac0(void* message, void*, U player_id, U panel, U flag1, U flag2, U value, const void* text) {
    void* client = client_by_id(player_id);                                           // 0056AACF..D3
    if (client == nullptr) return 1;                                                  // -> 0056AC13
    void* data;                                                                       // [esp+0C]
    U size;                                                                           // reuses the player-id slot
    if (panel == 0) {                                                                 // [00747E8C] = 0
        void* creature = creature_by_id(player_id);                                   // 0056AAF2..FC
        if (creature == nullptr || at<U>(at<void*>(creature, 0xa74), 0x6c) == 0) return 1;
        kotor_fastcall_004d68a0(message, nullptr, 0x80, 0xffffffffu, 1);              // 0056AB1A..25
        kotor_fastcall_004d68e0(message, nullptr, flag1);                             // 0056AB2A..31
        kotor_fastcall_004d68e0(message, nullptr, flag2);                             // 0056AB36..3D
        kotor_fastcall_004d6340(message, nullptr, value, 0x20);                       // 0056AB42..4B
        kotor_fastcall_004d6960(message, nullptr, text, 0x20);                        // 0056AB50..59
        if (kotor_fastcall_004d6440(message, nullptr, &data, &size) == 0) return 0;   // 0056AB5E..71
        return kotor_fastcall_00567e60(message, nullptr, at<U>(client, 4), 0x24, 1, data, size); // 0056AB73..87
    }
    if (panel != 1) return 1;                                                         // [00747E90] = 1
    void* creature = creature_by_id(player_id);                                       // 0056AB9B..A4
    if (creature == nullptr || at<U>(at<void*>(creature, 0xa74), 0x6c) == 0) return 1;
    kotor_fastcall_004d68a0(message, nullptr, 8, 0xffffffffu, 1);                     // 0056ABBA..C2
    kotor_fastcall_004d6960(message, nullptr, text, 0x20);                            // 0056ABC7..D0
    if (kotor_fastcall_004d6440(message, nullptr, &data, &size) == 0) return 0;       // 0056ABD5..E8
    return kotor_fastcall_00567e60(message, nullptr, at<U>(client, 4), 0x24, 3, data, size); // 0056ABF3..07
}

// 005BE790 CSWSFaction::InviteMember(invitee id, inviter id). RET 8.
U __fastcall kotor_fastcall_005be790(void*, void*, U invitee_id, U inviter_id) {
    (void)recovered_server_sws_message_004aedb0(server(), nullptr);                   // 005BE790..9B (result unused)
    void* invitee = creature_by_id(invitee_id);                                       // 005BE7A0..AE
    if (invitee != nullptr) {
        recovered_creature_set_invited_004ef520(invitee, nullptr, 1);                 // 005BE7BD..C1
        at<U>(invitee, 0x92c) = inviter_id;                                           // 005BE7C6
    }
    void* inviter = creature_by_id(inviter_id);                                       // 005BE7CC..D6
    if (inviter == nullptr) return 0;
    kotor_fastcall_0051b580(inviter, nullptr, invitee_id);                            // 005BE7DF..E2
    return 1;
}

// 005BFA30 CSWSFaction::TransferLeadership(new leader, requester): requester must lead; move to index 0. RET 8.
U __fastcall kotor_fastcall_005bfa30(void* faction, void*, U new_leader, U requester) {
    if (recovered_faction_leader_005bf5c0(faction, nullptr) != requester) return 0;   // 005BFA33..3C
    recovered_int_list_remove_0066dcc0(faction, nullptr, new_leader);                 // 005BFA43..46
    w1x_pointer_list_insert_00616940(faction, nullptr, new_leader, 0);                // 005BFA4B..50
    return 1;
}

// 005BFB20 CSWSFaction::AttemptRemove(target, requester): more than one member, and self-removal or leader. RET 8.
U __fastcall kotor_fastcall_005bfb20(void* faction, void*, U target, U requester) {
    if (static_cast<I>(at<U>(faction, 4)) <= 1) return 0;                             // 005BFB24..29
    if (target != requester && recovered_faction_leader_005bf5c0(faction, nullptr) != requester) return 0; // 005BFB33..3E
    recovered_faction_remove_005bf920(faction, nullptr, target);                      // 005BFB40..43
    return 1;
}

// ---------------------------------------------------------------------------------------------------------
// 00526910 CSWSMessage::HandlePlayerToServerPartyMessage(CSWSPlayer* player, BYTE minor). RET 8.
// Jump table 00527354 indexed by minor-2 (0..A): 2 list, 3/4/5 reject, 6 leave, 7 kick, 8 transfer leadership,
// 9 invite, 10 ignore, 11 accept, 12 decline. Other minors are rejected (EAX 0).
U __fastcall recovered_player_to_server_party_00526910(void* message, void*, void* player, U minor) {
    alignas(4) unsigned char string_28[8];                                            // CExoString [esp+28]
    alignas(4) unsigned char resref_30[16];                                           // CResRef [esp+30]
    alignas(4) unsigned char string_18[8];                                            // CExoString [esp+18]
    kotor_movie::string_element_construct_005b3190(string_28, nullptr);              // 0052692E..32
    std::memset(resref_30, 0, sizeof resref_30);                                      // 00526939..41 CResRef() zero-fills (inline)
    kotor_movie::string_element_construct_005b3190(string_18, nullptr);              // 00526946..52
    (void)resref_30;
    U result = 0;
    const U case_index = static_cast<U>(static_cast<unsigned char>(minor)) - 2u;      // 00526979..7E
    if (kotor_fastcall_004d6230(message, nullptr, 1) != 0 || player == nullptr || case_index > 0xa) {
        result = 0;                                                                   // 00526957..84 -> 0052731E
    } else {
        result = 1;                                                                   // every case ends at 005272F8
        switch (case_index) {
        case 0: {                                                                     // minor 2: 005272B3 party list
            void* object = kotor_fastcall_00561080(player, nullptr);
            if (object != nullptr) {
                void* creature = as_creature(kotor_fastcall_00561080(player, nullptr));  // 005272BE..C9
                void* faction = faction_of(creature);                                 // 005272CE..D0
                const U creature_id = at<U>(creature, 4);                             // 005272D5
                recovered_party_list_00567f90(message, nullptr, at<U>(player, 4), at<U>(faction, 4),
                                              at<void*>(faction, 0), 1, creature_id); // 005272D8..F3
            } else {
                recovered_party_list_00567f90(message, nullptr, at<U>(player, 4), 0, nullptr, 1, 0x7f000000u); // 005272E4..F3
            }
            break;
        }
        case 1: case 2: case 3: result = 0; break;                                    // minors 3/4/5 -> 0052731E
        case 4: {                                                                     // minor 6: 00526991 leave party
            (void)kotor_fastcall_00578540(message, nullptr);                          // 00526991..93 (id read, unused)
            const U self_id = at<U>(player, 0x38);                                    // 00526998
            void* creature = creature_by_id(self_id);                                 // 0052699B..A4
            void* client = client_by_id(self_id);                                     // 005269A9..B5
            if (creature == nullptr || client == nullptr) break;
            void* faction = faction_of(creature);                                     // 005269CA..CC
            if (faction == nullptr) break;
            recovered_faction_remove_005bf920(faction, nullptr, self_id);             // 005269DF..E2
            void* const* members = at<void* const*>(faction, 0);                      // 005269EA (cached)
            for (I i = 0; i < static_cast<I>(at<U>(faction, 4)); ++i) {              // 005269E7..F0 / 00526A4A..54
                void* member = creature_by_id(reinterpret_cast<const U*>(members)[i]);  // 005269F2..02
                if (member != nullptr) feedback(member, 0x2c, new_feedback_data(self_id)); // 00526A0D..45
            }
            void* self = creature_by_id(self_id);                                     // 00526A56..60
            if (self != nullptr) feedback(self, 0x2d, nullptr);                       // 00526A6D..73
            break;
        }
        case 5: {                                                                     // minor 7: 00526A7D remove member
            const U id = kotor_fastcall_00578540(message, nullptr);
            void* creature = as_creature(kotor_fastcall_00561080(player, nullptr));   // 00526A84..91
            void* target = creature_by_id(id);                                        // 00526A96..9F ([esp+10])
            void* client = client_by_id(id);                                          // 00526AA4..B4
            void* faction = faction_of(creature);                                     // 00526AB9..BD (before the null tests)
            if (target == nullptr || client == nullptr || creature == nullptr || faction == nullptr) break;
            if (kotor_fastcall_005bfb20(faction, nullptr, id, at<U>(creature, 4)) == 1) { // 00526AE6..F5
                void* const* members = at<void* const*>(faction, 0);                  // 00526B02 (cached)
                for (I i = 0; i < static_cast<I>(at<U>(faction, 4)); ++i) {          // 00526AFF..0C / 00526B6C..72
                    void* member = creature_by_id(reinterpret_cast<const U*>(members)[i]);
                    if (member != nullptr) feedback(member, 0x20, new_feedback_data(id)); // 00526B2B..63
                }
                void* removed = creature_by_id(id);                                   // 00526B74..7E
                if (removed != nullptr) feedback(removed, 0x21, nullptr);             // 00526B87..8D
            }
            if (at<U>(target, 0x4e8) != 0) at<U>(target, 0x4e8) = 0;                  // 00526B92..A4
            break;
        }
        case 6: {                                                                     // minor 8: 00526BB3 transfer leadership
            const U id = kotor_fastcall_00578540(message, nullptr);
            void* target = creature_by_id(id);                                        // 00526BBA..C6
            void* creature = as_creature(kotor_fastcall_00561080(player, nullptr));   // 00526BCB..D8
            void* faction = faction_of(creature);                                     // 00526BDD..DF
            if (target == nullptr || creature == nullptr || faction == nullptr) break;
            (void)kotor_fastcall_005bfa30(faction, nullptr, id, at<U>(creature, 4));  // 00526BFE..05 (result unused)
            void* const* members = at<void* const*>(faction, 0);                      // 00526C0A..0D (cached)
            for (I i = 0; i < static_cast<I>(at<U>(faction, 4)); ++i) {              // 00526C11..1E / 00526C8D..97
                void* member = creature_by_id(reinterpret_cast<const U*>(members)[i]);
                if (member != nullptr) feedback(member, 0x1f, new_feedback_data(id)); // 00526C4C..84
            }
            break;
        }
        case 7: {                                                                     // minor 9: 00526C9E invite
            const U id = kotor_fastcall_00578540(message, nullptr);
            void* target = creature_by_id(id);                                        // 00526CA5..B1
            void* creature = as_creature(kotor_fastcall_00561080(player, nullptr));   // 00526CB6..C3
            if (target == nullptr || creature == nullptr) break;
            if (at<U>(target, 0x930) != 0 || at<U>(creature, 0x930) != 0) {           // 00526CD8..EE -> 00526EEC
                feedback(creature, 0x22, new_feedback_data(id));
                break;
            }
            if (kotor_fastcall_005188c0(target, nullptr) != 0) {                      // 00526CF4..FD
                feedback(creature, 0x23, new_feedback_data(id));
                break;
            }
            if (kotor_fastcall_00518580(target, nullptr, at<U>(creature, 4)) == 1) {  // 00526D41..4F
                feedback(creature, 0x27, new_feedback_data(id));
                break;
            }
            if (recovered_faction_singleton_party_005bf690(faction_of(target), nullptr) == 0) { // 00526D93..A3
                feedback(creature, 0xca, new_feedback_data(id));
                break;
            }
            void* own_faction = faction_of(creature);                                 // 00526DEA..EC
            if (client_by_id(id) == nullptr || own_faction == nullptr) break;         // 00526DF3..0B
            if (kotor_fastcall_005be790(own_faction, nullptr, id, at<U>(creature, 4)) != 1) break; // 00526E11..20
            feedback(target, 0x25, new_feedback_data(at<U>(creature, 4)));            // 00526E26..61
            feedback(creature, 0x24, new_feedback_data(id));                          // 00526E66..9E
            alignas(4) unsigned char name_20[8];                                      // CExoString [esp+20]
            void* stats = at<void*>(creature, 0xa74);                                 // 00526EA3
            void* name = kotor_fastcall_005a4c60(stats, nullptr, name_20);            // 00526EA9..B0
            kotor_fastcall_0056aac0(recovered_server_sws_message_004aedb0(server(), nullptr), nullptr,
                                    id, 1, 0, 0, 0, name);                            // 00526EB5..D4
            frame_LogStringDtor_005e5c20(name_20, nullptr);                           // 00526ED9..E2
            break;
        }
        case 8: {                                                                     // minor 10: 00526F2E ignore invitation
            const U id = kotor_fastcall_00578540(message, nullptr);
            void* inviter = creature_by_id(id);                                       // 00526F37..40
            (void)client_by_id(id);                                                   // 00526F45..51 (result unused)
            void* creature = as_creature(kotor_fastcall_00561080(player, nullptr));   // 00526F56..61
            if (creature != nullptr &&
                kotor_fastcall_0051d060(creature, nullptr, 2, at<U>(inviter, 4)) == 1) { // 00526F66..7E
                feedback(creature, 0x28, new_feedback_data(at<U>(inviter, 4)));       // 00526F84..BF
                feedback(inviter, 0x27, new_feedback_data(at<U>(creature, 4)));       // 00526FC4..FF
            } else {                                                                  // 00527009 (also for a null creature)
                feedback(inviter, 0x2b, new_feedback_data(at<U>(creature, 4)));       // 00527009..44
            }
            break;
        }
        case 9: {                                                                     // minor 11: 0052704E accept invitation
            void* creature = as_creature(kotor_fastcall_00561080(player, nullptr));   // 0052704E..59 ([esp+58])
            if (creature == nullptr) break;
            U id = kotor_fastcall_00578540(message, nullptr);                         // 0052706A..6C
            if (id == 0x7f000000u) id = at<U>(creature, 0x92c);                       // 00527071..78
            void* inviter = creature_by_id(id);                                       // 0052707E..88
            if (inviter == nullptr) break;
            if (kotor_fastcall_0051d060(creature, nullptr, 0, at<U>(inviter, 4)) == 1) { // 00527097..AB
                void* const* members = at<void* const*>(faction_of(inviter), 0);      // 005270B1..BC
                if (static_cast<I>(at<U>(faction_of(inviter), 4)) <= 0) break;        // 005270C2..CC
                I i = 0;
                do {                                                                  // 005270D2..3C
                    void* member = creature_by_id(reinterpret_cast<const U*>(members)[i]);
                    if (member != nullptr) feedback(member, 0x26, new_feedback_data(at<U>(creature, 4)));
                    ++i;
                } while (i < static_cast<I>(at<U>(faction_of(inviter), 4)));          // GetFaction re-called each time
            } else {
                feedback(creature, 0x2b, new_feedback_data(at<U>(inviter, 4)));       // 00527143..7E
            }
            break;
        }
        case 10: {                                                                    // minor 12: 00527188 decline invitation
            const U id = kotor_fastcall_00578540(message, nullptr);
            void* inviter = creature_by_id(id);                                       // 0052718F..9B
            void* creature = as_creature(kotor_fastcall_00561080(player, nullptr));   // 005271A0..AD
            void* client = client_by_id(id);                                          // 005271B2..BB
            if (inviter == nullptr || client == nullptr || creature == nullptr) break;
            if (kotor_fastcall_0051d060(creature, nullptr, 1, at<U>(inviter, 4)) == 1) { // 005271D8..EA
                feedback(creature, 0x2a, new_feedback_data(at<U>(inviter, 4)));       // 005271F0..29
                feedback(inviter, 0x29, new_feedback_data(at<U>(creature, 4)));       // 0052722E..69
            } else {
                feedback(creature, 0x2b, new_feedback_data(at<U>(inviter, 4)));       // 00527273..AC
            }
            break;
        }
        default: break;
        }
    }
    frame_LogStringDtor_005e5c20(string_18, nullptr);                                 // 005272F8..01 / 0052731E..27
    frame_LogStringDtor_005e5c20(string_28, nullptr);                                 // 00527306..12 / 0052732C..38
    return result;
}
}  // extern "C"
