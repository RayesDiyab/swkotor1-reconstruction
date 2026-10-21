// W5 G16: CSWSMessage::HandlePlayerToServerInputMessage (005254C0) sub-dispatcher (major 6 "Input",
// client-dispatch route 005254C0/00748250), with the drive branch (minor 1Dh) and its companions
// AbortDriveControl 00525450 (minor 21h) and CancelGuiTimingEvent 00523570 (minor 22h) reconstructed.
// Original inline minors2 Attack,7 UseSkill,8 StartDialogue restored.
// closeC: every remaining inline minor of the 0052684C jump table (index = minor-1, 24h entries) and
// HandlePlayerToServerInputWalkToWaypoint 005235B0 reconstructed; no trap remains.
#include "ee682cef1a-w5_drive_control.hpp"
#include "8fbd11ef5e-w5_server_env.hpp"
#include "15d9510918-w5_input_message.hpp"
#include "0b71ef6517-w5_foreign.hpp"
#include "28cae2ed07-w5_action_queue.hpp"

extern "C" {
// G18 producers: bodies in tiers/t2/source/w5_door.cpp (t1-only links fall back to traps, w5_foreign_traps.cpp)
U __fastcall w5_object_add_open_door_action_004cfac0(void* object, void*, U door_id, U arg2);   // RET 8
U __fastcall w5_object_add_close_door_action_004cfb20(void* object, void*, U door_id, U arg2);  // RET 8
U __fastcall w5_object_add_use_object_action_0057c810(void* object, void*, U target_id);       // RET 4
}

using namespace w5;
extern "C" {
void __fastcall frame_LogStringCtor_005e5a90(void*, void*, const char*);
void __fastcall frame_LogStringDtor_005e5c20(void*, void*);
U __fastcall recovered_ai_level_004b08a0(void*, void*, void*, U);
U __fastcall kotor_fastcall_004fde40(void*, void*, U, U, U, U, U, U, U, U, U, U, U, U, U, U, U);
U __fastcall kotor_fastcall_004fbe40(void*, void*, U, U, U, const Vector*, U, U, U);
// ---- closeC: providers for the remaining inline minors and 005235B0 (c35 map owners in brackets)
unsigned __fastcall kotor_fastcall_004d66f0(void*, void*);                                   // ReadBOOL RET 0 [w2_message_native]
unsigned __fastcall kotor_fastcall_004d6780(void*, void*, unsigned bits);                    // ReadDWORD RET 4 [w2_message_native]
unsigned __fastcall recovered_object_add_action_to_front_004ccb00(void* object, void*, unsigned type, unsigned group,
    unsigned t0, void* v0, unsigned t1, void* v1, unsigned t2, void* v2, unsigned t3, void* v3, unsigned t4, void* v4, unsigned t5, void* v5,
    unsigned t6, void* v6, unsigned t7, void* v7, unsigned t8, void* v8, unsigned t9, void* v9, unsigned t10, void* v10, unsigned t11, void* v11,
    unsigned t12, void* v12);                                                                // AddActionToFront RET 70h [w1_native_b7]
void* __fastcall recovered_item_lookup_004ae760(void* server, void*, U id);                  // GetItemByGameObjectID [1025]
void* __fastcall recovered_placeable_lookup_004ae7a0(void* server, void*, U id);             // GetPlaceableByGameObjectID [0886]
void* __fastcall recovered_creature_inventory_004ef770(void* creature, void*, U flag);       // GetItemRepository RET 4 [1086]
U __fastcall kotor_fastcall_004ecee0(void* creature, void*, U feat, U sub, U target, U area); // UseFeat RET 10h [vmD_c]
U __fastcall kotor_fastcall_004fc210(void* creature, void*, U item, U a2, U a3, U target, const void* position, U area, U a7); // UseItem RET 1Ch
U __fastcall kotor_fastcall_0057c870(void* object, void*, U item, U target, I count, U a4);  // AddGiveItemActions RET 10h [p2s_a_inv]
U __fastcall kotor_fastcall_004cfb80(void* object, void*, U target, U key, U a3);            // AddUnlockObjectAction RET 0Ch
U __fastcall kotor_fastcall_004cfc20(void* object, void*, U target);                         // AddLockObjectAction RET 4 [vmD_b]
unsigned __fastcall kotor_fastcall_004f1ef0(void*, void*, unsigned, unsigned);               // CancelAction RET 8
U __fastcall kotor_fastcall_004f9460(void* self, void*, U a1, U a2, U a3, U a4, U a5, U a6, U a7, U a8, U a9,
    U a10, U a11, U a12, U a13, U a14, U a15, U a16, U a17, U a18);                          // AddCastSpellActions RET 48h
U __fastcall kotor_fastcall_00500ca0(void* creature, void*, U type, U id, U target, U a4, U a5, U a6, U a7, U a8); // AddUseTalentOnObjectActions RET 20h
U __fastcall kotor_fastcall_004fd070(void* creature, void*, U type, U id, const void* position, U a4, U a5, U a6, U a7, U a8); // AddUseTalentAtLocationActions RET 20h
unsigned __fastcall kotor_fastcall_004f2a50(void*, void*, unsigned);                         // ToggleMode RET 4
U __fastcall kotor_fastcall_004eb190(void* creature, void*, U mask);                         // GetActivity RET 4
void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void* spells, void*, int id);     // CSWSpellArray::GetSpell RET 4
unsigned char __fastcall recovered_stats_spell_uses_left_005a5550(void* stats, void*, U a, U b, U c, U d); // GetSpellUsesLeft RET 10h
void __fastcall kotor_fastcall_004ae9a0(void*, void*, unsigned, unsigned);                   // SetPauseState RET 8
void __fastcall kotor_fastcall_004ae960(void*, void*, unsigned);                             // TogglePauseState RET 4
void* __fastcall recovered_server_info_004aec90(void*, void*);                               // GetServerInfo
void* __fastcall recovered_server_sws_message_004aedb0(void*, void*);                        // GetSWSMessage
void __fastcall recovered_item_owner_00553210(void*, void*, unsigned, unsigned, unsigned, unsigned); // SetPossessor RET 10h
unsigned __fastcall recovered_inventory_insert_0055d330(void*, void*, void**, unsigned, unsigned, unsigned); // CItemRepository::AddItem RET 10h
unsigned __fastcall recovered_placeable_remove_item_00584ac0(void*, void*, void*);           // CSWSPlaceable::RemoveItem RET 4
unsigned __fastcall recovered_object_remove_effect_004d06c0(void*, void*, unsigned, unsigned); // RemoveEffectById RET 8
unsigned __fastcall gui_load_slot_zero_0063e7f0(void*, void*);                               // return_zero 0063E7F0
unsigned __fastcall kotor_m15e_slot128_false4_0063e7c0(void*, void*, unsigned);              // return_false_4 0063E7C0 (RET 4)
void __fastcall r8_copy_construct_005e5b00(void* self, void*, void* other);                  // CExoString(CExoString const&)
struct CloseCExoStringArg { void* text; U length; };                                         // CExoString by value (callee destroys)
U __fastcall recovered_send_chat_00571960(void* message, void*, U mode, U speaker, CloseCExoStringArg text, U target,
                                          void* extra);                                      // SendServerToPlayerChatMessage [W2]
void* __fastcall recovered_object_array_004aed70(void*, void*);                           // GetObjectArray [w2_party_native]
void __fastcall w1x_object_set_orientation_004cd170(void* object, void*, void* direction);   // SetOrientation [0886]
// 005235B0 callees
void __fastcall w5_pathfind_reset_at_end_of_path_005d0ec0(void* pf, void*);                  // ResetAtEndOfPath [1206-08]
void __fastcall kotor_fastcall_004d28f0(void* round, void*, U paused, U id);                 // SetRoundPaused RET 8
void __fastcall kotor_fastcall_004d2920(void* round, void*, U time, U flag);                 // SetPauseTimer RET 8
Vector* __cdecl vector_cross_004ab210(Vector* out, const Vector* a, const Vector* b);        // vectorCrossProduct [0464]
U __fastcall recovered_creature_add_move_to_point_action_004f8b60(void* creature, void*, U group, const void* point, U area_id,
    U target_id, U run, U range, U a6, U a7, U a8, U a9, U a10, U a11, U a12, const void* a13, U a14, U a15, U a16);  // RET 44h
// closeC new bodies (srcw5\actions\claude_closeC_srv.cpp)
void __fastcall kotor_fastcall_005b56e0(void* creature, void*, U step, U time, U target);   // CSWSCreature::DoCombatStep RET 0Ch
U __fastcall kotor_fastcall_0056ebe0(void* message, void*, void* player, U id);              // ExamineGui_CreatureData RET 8
U __fastcall kotor_fastcall_0056ee40(void* message, void*, void* player, U id);              // ExamineGui_ItemData RET 8
U __fastcall kotor_fastcall_0056f0a0(void* message, void*, void* player, U id);              // ExamineGui_PlaceableData RET 8
U __fastcall kotor_fastcall_0056f370(void* message, void*, void* player, U id);              // ExamineGui_DoorData RET 8
U __fastcall kotor_fastcall_00570230(void* message, void*, void* player, U id);              // DebugInfo_Creature RET 8
U __fastcall kotor_fastcall_0056a7f0(void* message, void*, void* player, U id);              // DebugInfo_Item RET 8
U __fastcall kotor_fastcall_0056a8c0(void* message, void*, void* player, U id);              // DebugInfo_Placeable RET 8
U __fastcall kotor_fastcall_0056aa30(void* message, void*, void* player, U id);              // DebugInfo_Area RET 8
U __fastcall kotor_fastcall_0056cac0(void* message, void*, void* player);                    // Module_ExportReply RET 4
void __fastcall kotor_fastcall_004fd1e0(void* creature, void*, U flag);                         // CSWSCreature::Rest RET 4
U __fastcall kotor_fastcall_004fce90(void* creature, void*, U target);                       // AddCounterSpellActions RET 4
void __fastcall kotor_fastcall_004f0a60(void* creature, void*, void* item, U count);            // CSWSCreature::SplitItem RET 8
void __fastcall kotor_fastcall_004f0990(void* creature, void*, void* item, void* into);         // CSWSCreature::MergeItem RET 8
}
namespace {
// Original dispatcher state2 owns the string only after 005E5A90 returns.
struct DialogueString {
    alignas(4) unsigned char object[8];
    DialogueString() { frame_LogStringCtor_005e5a90(object, nullptr, ""); } // literal0073D71C
    ~DialogueString() { frame_LogStringDtor_005e5c20(object, nullptr); }
};
struct LocalString {                                                    // CExoString local with EH cleanup
    alignas(4) unsigned char object[8];
    explicit LocalString(const char* text) { frame_LogStringCtor_005e5a90(object, nullptr, text); }
    ~LocalString() { frame_LogStringDtor_005e5c20(object, nullptr); }
};
constexpr U k_invalid_00746644 = 0x7f000000u;                           // .rdata 00746644
constexpr U k_effect_type_00746774 = 0x3e;                              // .rdata 00746774 (dword 3Eh)
constexpr float k_wp_close_0073dfec = 0.01f;                            // 3C23D70A
constexpr float k_wp_step_range_00747d7c = 2.25f;                       // 40100000
constexpr float k_cos45_00748024 = 0.70700002f;                         // 3F34FDF4
constexpr float k_mcos45_00746ffc = -0.70700002f;                       // BF34FDF4
constexpr float k_zero_0073d700 = 0.0f;
void* server() { return server_app(); }                                 // [[007A39FC]+8], reloaded per use
void* objects() { return recovered_object_array_004aed70(server(), nullptr); }
U low_byte_into(U slot, U8 value) { return (slot & 0xffffff00u) | value; } // mov byte ptr [arg slot], al
U addr(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
}
extern "C" void* __fastcall recovered_object_array_004aed70(void*, void*);

extern "C" {

// 00525450 HandlePlayerToServerInputAbortDriveControl(player): clear the repeat count of a head drive node.
U __fastcall w5_message_input_abort_drive_00525450(void* message, void*, void* player) {
    if (recovered_player_message::overrun_004d6230(message) != 0) return 0;
    if (recovered_player_message::underrun_004d6240(message) != 0) return 0;
    void* object = kotor_fastcall_00561080(player, nullptr);
    if (object == nullptr) return 0;
    void* creature = vcall<void*>(object, 0x30);
    if (creature != nullptr && at<U>(creature, 0xe8) == 1) {
        void** list = &at<void*>(creature, 0xfc);                         // 0052548C (lea; never null)
        void* head = *static_cast<void**>(*list);
        if (head != nullptr) {
            void* node = at<void*>(head, 8);
            if (node != nullptr && at<U>(node, 0) == 0x33) at<U>(node, 0x4c) = 0;   // 005254A4..005254A9
        }
    }
    return 1;
}

// 00523570 HandlePlayerToServerInputCancelGuiTimingEvent(player)
U __fastcall w5_message_input_cancel_gui_timing_00523570(void*, void*, void* player) {
    void* object = kotor_fastcall_00561080(player, nullptr);
    if (object == nullptr) return 0;                                      // EAX = null object
    void* creature = vcall<void*>(object, 0x30);
    if (creature != nullptr && at<U>(creature, 0xe8) == 1) w5_object_clear_all_actions_004ccd80(creature, nullptr, 1);
    return 1;
}

// 005235B0 HandlePlayerToServerInputWalkToWaypoint(player): thiscall RET 4, EAX 0 on read error / missing object, else 1.
U __fastcall w5_message_input_walk_to_waypoint_005235b0(void* message, void*, void* player) {
    const U id = kotor_fastcall_00578540(message, nullptr);                       // 005235B9 EBP
    Vector point;                                                                  // [esp+14..1C]
    point.x = kotor_fastcall_004d67b0(message, nullptr, 0x3f800000u, 0x20);        // 005235C9..CE
    point.y = kotor_fastcall_004d67b0(message, nullptr, 0x3f800000u, 0x20);        // 005235DB..E0
    point.z = kotor_fastcall_004d67b0(message, nullptr, 0x3f800000u, 0x20);        // 005235ED..F2
    const U b = kotor_fastcall_004d6720(message, nullptr, 8);                      // 005235FA movzx -> EBX, [esp+20]
    const U f1 = kotor_fastcall_004d66f0(message, nullptr);                        // 00523608 [esp+28]
    const U f2 = kotor_fastcall_004d66f0(message, nullptr);                        // 00523613 [esp+24]
    const U8 c = kotor_fastcall_004d6720(message, nullptr, 8);                     // 00523620 byte [esp+13]
    const U t = kotor_fastcall_00578540(message, nullptr);                         // 0052362B [esp+2C]
    if (recovered_player_message::overrun_004d6230(message) != 0) return 0;        // 00523630..3F
    if (recovered_player_message::underrun_004d6240(message) != 0) return 0;       // 00523645..50
    void* object = kotor_fastcall_00561080(player, nullptr);                       // 0052365C ESI
    if (kotor_fastcall_00561080(player, nullptr) == nullptr) return 0;             // 00523665..6C
    if (object == nullptr) return 0;                                               // 00523672..74
    if (recovered_object_area_004cb120(object, nullptr) == nullptr) return 0;      // 0052367A..83
    if (vcall<void*>(kotor_fastcall_00561080(player, nullptr), 0x30) != nullptr) { // 00523689..99
        if (at<U>(vcall<void*>(kotor_fastcall_00561080(player, nullptr), 0x30), 0x9dc) != 0) return 0; // 0052369F..B5
        void* pf = at<void*>(vcall<void*>(kotor_fastcall_00561080(player, nullptr), 0x30), 0x340);    // 005236BB..C9
        Vector end;                                                                // [esp+3C..44]
        U end_id;
        if (at<U>(pf, 0x84) == 0) {                                                // 005236CF..D7
            end.x = at<float>(pf, 0x5c); end.y = at<float>(pf, 0x60); end.z = at<float>(pf, 0x64); end_id = at<U>(pf, 0x70);
        } else {                                                                   // 00523709
            end.x = at<float>(pf, 0x74); end.y = at<float>(pf, 0x78); end.z = at<float>(pf, 0x7c); end_id = at<U>(pf, 0x80);
        }
        const double dx = static_cast<double>(point.x) - end.x;                    // 0052373C..54 (x87, 53-bit)
        const double dy = static_cast<double>(point.y) - end.y;
        const double dz = static_cast<double>(point.z) - end.z;
        if (end_id == id) {                                                        // 0052373A cmp / 00523754 jne
            const double sq = (dz * dz + dy * dy) + dx * dx;                       // 00523756..64
            if (sq < static_cast<double>(k_wp_close_0073dfec)) {                   // 00523766..77 (unordered -> reset)
                at<U>(pf, 0x28) = b & 0xff;                                        // 00523779..7C movzx bl
                return 1;
            }
        }
        w5_pathfind_reset_at_end_of_path_005d0ec0(pf, nullptr);                    // 00523794..96
        if (c == 1) at<U>(pf, 0x194) = 1;                                          // 0052379B..A2
    }
    if (vcall<void*>(object, 0x30) != nullptr) {                                   // 005237AC..B5
        Vector target = point;                                                     // [esp+48..50]
        void* creature = vcall<void*>(object, 0x30);                               // 005237CF..DA EDI
        Vector* const position = &at<Vector>(creature, 0x90);                      // EBX
        Vector delta;                                                              // [esp+3C..44]
        kotor_cdecl_00423bc0(&delta, position, &target);                           // 005237DC..ED position - target
        const double sq = (static_cast<double>(delta.z) * delta.z + static_cast<double>(delta.y) * delta.y)
                          + static_cast<double>(delta.x) * delta.x;                // 005237F2..0F
        if (sq < static_cast<double>(k_wp_step_range_00747d7c) && at<U8>(creature, 0x9f2) == 2) { // 00523811..29
            const U enemy = at<U>(creature, 0x9f4);                                // 0052382F EBP
            void* other = kotor_fastcall_004ae750(server(), nullptr, enemy);       // 00523835..74
            if (other == nullptr || vcall<void*>(other, 0x10) == nullptr) return 1;  // 00523876..87
            other = vcall<void*>(other, 0x10);                                     // 0052388D..94
            if (other == nullptr) return 1;                                        // 00523896..98
            kotor_fastcall_004d28f0(at<void*>(creature, 0x9c8), nullptr, 1, at<U>(creature, 4));  // 0052389E..AA
            kotor_fastcall_004d2920(at<void*>(creature, 0x9c8), nullptr, 0x1f4, 0);              // 005238AF..BC
            Vector tmp, dir1, dir2;                                                // [esp+54], [esp+30..38], [esp+3C..44]
            kotor_cdecl_00423bc0(&tmp, &at<Vector>(other, 0x90), position);        // 005238C1..CE
            Vector n1; normalize_004ab180(&n1, &tmp);                              // 005238D3..D9 -> [esp+60]
            dir1 = n1;                                                             // 005238DE..F9
            Vector tmp2; kotor_cdecl_00423bc0(&tmp2, &target, position);           // 005238EE..FD
            Vector n2; normalize_004ab180(&n2, &tmp2);                             // 00523902..08 -> [esp+54]
            dir2.x = n2.x; dir2.y = n2.y;                                          // 00523911..1E [esp+3C/40]
            dir2.z = dir1.z;                                                       // 00523938 quirk: [esp+44] <- dir1.z overwrites n2.z
            const double dot = (static_cast<double>(dir1.y) * dir2.y + static_cast<double>(dir1.x) * dir2.x)
                               + static_cast<double>(dir1.z) * dir1.z;             // 0052390D..46 quirk: z1*z1
            if (dot >= static_cast<double>(k_cos45_00748024)) {                    // 00523948..53 C0 clear (unordered -> next test)
                kotor_fastcall_005b56e0(creature, nullptr, 2, 0x1f4, enemy);       // 00523955..61
                return 1;
            }
            // 00523975 fcomp -0.707: unordered and > fall through to the cross test
            if (dot < static_cast<double>(k_mcos45_00746ffc) || dot == static_cast<double>(k_mcos45_00746ffc)) {
                kotor_fastcall_005b56e0(creature, nullptr, 3, 0x1f4, enemy);       // 00523982..8C
                return 1;
            }
            Vector cross; vector_cross_004ab210(&cross, &dir1, &dir2);             // 005239A0..AF
            const bool step5 = !(cross.z >= k_zero_0073d700);                      // 005239C4..E0 C0 set on < or unordered
            kotor_fastcall_005b56e0(creature, nullptr, step5 ? 5 : 4, 0x1f4, enemy); // 005239E2 / 005239FA
            return 1;
        }
    }
    void* creature = vcall<void*>(object, 0x30);                                   // 00523A12..19 EBX
    if (creature == nullptr) return 1;                                             // 00523A1D
    if (at<U>(creature, 0xe8) != 1) return 1;                                      // 00523A23..2A
    recovered_creature_compute_ai_state_004f1460(creature, nullptr);              // 00523A30..37
    const U group = at<U8>(creature, 0x9f2) == 1 ? 0xfffeu : 0xffffu;              // 00523A32..45 EDI
    w5_creature_set_combat_mode_0050ee80(vcall<void*>(object, 0x30), nullptr, 0, 1); // 00523A4A..57
    w5_combat_clear_all_special_attacks_004d4f60(at<void*>(vcall<void*>(object, 0x30), 0x9c8), nullptr); // 00523A5C..69
    w5_creature_action_manager_004f8770(vcall<void*>(object, 0x30), nullptr, 8);   // 00523A6E..79
    Vector destination = point;                                                    // [esp+3C..44]
    Vector zero{0.0f, 0.0f, 0.0f};                                                 // [esp+48..50]
    (void)recovered_creature_add_move_to_point_action_004f8b60(vcall<void*>(object, 0x30), nullptr, group,
        &destination, id, t, f1, 0, 0, f2, b, 0, c, 0, 0, &zero, 0, 0, 0);         // 00523A7E..F6
    return 1;
}

// 005254C0 HandlePlayerToServerInputMessage(player, minor): thiscall RET 8; EAX = handler result, 0 otherwise.
U __fastcall w5_message_input_dispatch_005254c0(void* message, void*, void* player, U minor_arg) {
    void* creature = nullptr;                                             // EDI
    void* stats = nullptr;                                                // [esp+10]
    void* object = kotor_fastcall_00561080(player, nullptr);            // original EBX survives the creature cast
    if (object != nullptr) {        // 005254EA..00525516
        creature = vcall<void*>(object, 0x30);
        if (creature != nullptr) {
            stats = at<void*>(creature, 0xa74);
            if (at<U>(creature, 0x9dc) != 0) creature = nullptr;
        }
    }
    if (recovered_player_message::overrun_004d6230(message) != 0) return 0;   // 00525518..00525523
    const U minor = static_cast<U8>(minor_arg);
    const U player_slot = addr(player);                                   // [esp+6C] argument slot reused as a byte local
    switch (minor) {                                                      // table 0052684C, minor 1..24h
    case 0x01: return w5_message_input_walk_to_waypoint_005235b0(message, nullptr, player);
    case 0x1d: return w5_message_input_drive_control_00523450(message, nullptr, player);
    case 0x21: return w5_message_input_abort_drive_00525450(message, nullptr, player);
    case 0x22: return w5_message_input_cancel_gui_timing_00523570(message, nullptr, player);
    case 0x0f: case 0x13: case 0x1b: case 0x20: return 0;                 // table entries -> 00526835
    case 0x02: {                                                          // [00525573,005255EB),120bytes Attack
        const U target = kotor_fastcall_00578540(message, nullptr);
        const U mode = kotor_fastcall_004d6750(message, nullptr, 16);
        if (recovered_player_message::overrun_004d6230(message) != 0) return 0;
        if (recovered_player_message::underrun_004d6240(message) != 0) return 0;
        if (creature == nullptr) return 0;
        (void)kotor_fastcall_004fde40(creature, nullptr, target, mode, 1, 0, 0, 0, 0,
            1, 0x2719, 1500, 1, 0, 0, 4, 0);
        if (at<U>(creature, 0x50c) != target) at<U>(creature, 0x510) = target;
        return 1;
    }
    case 0x04: {                                                          // 00525D6B PlayAnimation / move
        const U animation = kotor_fastcall_004d6750(message, nullptr, 0x10);   // 00525D6F BX
        U target = kotor_fastcall_00578540(message, nullptr);                  // 00525D78 -> [esp+70]
        float x = kotor_fastcall_004d67b0(message, nullptr, 0x3f800000u, 0x20); // [esp+28]
        float y = kotor_fastcall_004d67b0(message, nullptr, 0x3f800000u, 0x20); // [esp+24]
        float z = kotor_fastcall_004d67b0(message, nullptr, 0x3f800000u, 0x20); // [esp+20]
        U animation_word = animation & 0xffff;                                 // [esp+18] movzx
        float speed = 1.0f;                                                    // [esp+14]
        float duration = 0.0f;                                                 // [esp+10]
        U one = 1;                                                             // [esp+6C]
        if (recovered_player_message::overrun_004d6230(message) != 0) return 0;   // 00525DDA..E1
        if (recovered_player_message::underrun_004d6240(message) != 0) return 0;  // 00525DEB..F2
        if (creature == nullptr) return 0;                                     // 00525DF8..FA
        if (at<U>(creature, 0xe8) == 0) return 0;                              // 00525E00..08
        if (at<U8>(creature, 0x9fc) & 8) w5_object_clear_all_actions_004ccd80(creature, nullptr, 1); // 00525E0E..1B
        if (at<U>(creature, 4) == target) {                                    // 00525E2A..4E
            (void)recovered_object_add_action_to_front_004ccb00(creature, nullptr, 6, 0xffff,
                1, &animation_word, 2, &speed, 2, &duration, 1, &one, 0, nullptr, 0, nullptr, 0, nullptr,
                0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr);    // 00525E50..78
            return 1;
        }
        if (target != 0x7f000000u)                                             // 00525E87..AB
            (void)recovered_object_add_action_004cea20(creature, nullptr, 0x13, 0xffff, 3, &target,
                0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr,
                0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr);
        else                                                                   // 00525EAD..CA
            (void)recovered_object_add_action_004cea20(creature, nullptr, 0x31, 0xffff, 2, &x, 2, &y, 2, &z,
                0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr,
                0, nullptr, 0, nullptr, 0, nullptr);
        (void)recovered_object_add_action_004cea20(creature, nullptr, 6, 0xfffe,
            1, &animation_word, 2, &speed, 2, &duration, 1, &one, 0, nullptr, 0, nullptr, 0, nullptr,
            0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr);        // 00525ED3..1F
        return 1;
    }
    case 0x05: {                                                          // 0052599F Examine
        const U id = kotor_fastcall_00578540(message, nullptr);           // EBX
        if (recovered_player_message::overrun_004d6230(message) != 0) return 0;
        if (recovered_player_message::underrun_004d6240(message) != 0) return 0;
        if (creature == nullptr) return 0;                                // 005259CA..CC
        void* found = nullptr;                                            // [esp+70]
        if (static_cast<U8>(kotor_fastcall_004d8230(objects(), nullptr, id, &found)) != 0) return 1; // 005259D2..F3 vs byte [0074666C]=0
        switch (at<U8>(found, 8)) {                                       // 005259F9..0D table 005268DC (type-5)
        case 5: return kotor_fastcall_0056ebe0(message, nullptr, player, id);              // 00525A14
        case 6:                                                                             // 00525A22
            (void)kotor_m15e_slot128_false4_0063e7c0(creature, nullptr, id);
            return kotor_fastcall_0056ee40(message, nullptr, player, id);
        case 7: {                                                                           // 00525A46 trap object: Examine skill 66h
            void* area = recovered_object_area_004cb120(creature, nullptr);
            Vector none{0.0f, 0.0f, 0.0f};                                                  // [esp+50..58]
            (void)kotor_fastcall_004fbe40(creature, nullptr, 1, 0x66, id, &none, at<U>(area, 0x120), 0x7f000000u, 0);
            return 1;
        }
        case 9: return kotor_fastcall_0056f0a0(message, nullptr, player, id);              // 00525A38
        case 10: return kotor_fastcall_0056f370(message, nullptr, player, id);             // 00525A90
        default: {                                                                          // 00525A9E (type 8, <5, >10)
            LocalString text("Feature not implemented: Examine unhandled for this object type."); // 00748030 [esp+28]
            LocalString extra("");                                                          // 0073D71C [esp+30]
            const U target = at<U>(player, 4);                                              // 00525AC2
            CloseCExoStringArg by_value;                                                    // copy destroyed by the callee
            r8_copy_construct_005e5b00(&by_value, nullptr, text.object);                    // 00525ACB..DE
            (void)recovered_send_chat_00571960(message, nullptr, 5, 0x7f000000u, by_value, target, extra.object); // 00525AE3..EC
            return 1;                                                                       // 00525AF1..B10
        }
        }
    }
    case 0x06: {                                                          // 005255EB UseFeat
        const U feat = kotor_fastcall_004d6750(message, nullptr, 0x10);   // EBX
        const U sub = kotor_fastcall_004d6750(message, nullptr, 0x10);    // EBP
        const U target = kotor_fastcall_00578540(message, nullptr);       // [esp+70]
        if (recovered_player_message::overrun_004d6230(message) != 0) return 0;
        if (recovered_player_message::underrun_004d6240(message) != 0) return 0;
        if (creature == nullptr) return 0;
        if (recovered_object_area_004cb120(creature, nullptr) == nullptr) return 0;          // 00525636..3F
        void* area = recovered_object_area_004cb120(creature, nullptr);                     // 00525645..4C
        (void)kotor_fastcall_004ecee0(creature, nullptr, feat, sub, target, at<U>(area, 0x120)); // 0052564C..61
        return 1;
    }
    case 0x07: {                                                          // [00525670,00525741),209bytes UseSkill
        const U skill = kotor_fastcall_004d6720(message, nullptr, 8);
        const U subtype = kotor_fastcall_004d6720(message, nullptr, 8);
        const U target = kotor_fastcall_00578540(message, nullptr);
        Vector position;
        position.x = kotor_fastcall_004d67b0(message, nullptr, 0x3f800000u, 32);
        position.y = kotor_fastcall_004d67b0(message, nullptr, 0x3f800000u, 32);
        position.z = kotor_fastcall_004d67b0(message, nullptr, 0x3f800000u, 32);
        if (recovered_player_message::overrun_004d6230(message) != 0) return 0;
        if (recovered_player_message::underrun_004d6240(message) != 0) return 0;
        if (creature == nullptr) return 0;
        void* area = recovered_object_area_004cb120(creature, nullptr);
        const U area_id = at<U>(area, 0x120); // original required area dereference; no fabricated area
        (void)kotor_fastcall_004fbe40(creature, nullptr, skill, subtype, target, &position, area_id, 0x7f000000u, 0);
        return 0; // original0052573C jumps00526835 even after real action producer
    }
    case 0x08: {                                                          // 00525F2E..00526002 StartDialogue
        if (creature == nullptr) return 0;
        U target = kotor_fastcall_00578540(message, nullptr);
        U invalid = 0x7f000000u;
        w5_creature_action_manager_004f8770(creature, nullptr, 8);
        DialogueString text;
        U zero = 0, one = 1, final_zero = 0;
        if (at<U>(creature, 0x78) == 0)
            (void)recovered_ai_level_004b08a0(ai_master(), nullptr, object, 1);
        // Original AddAction(18h,FFFFh), six typed pairs then seven empty pairs.
        (void)recovered_object_add_action_004cea20(object, nullptr, 0x18, 0xffff,
            3, &target, 4, text.object, 1, &zero, 1, &one, 1, &final_zero, 3, &invalid,
            0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr);
        return 1; // string destroyed before original success return, also on provider exceptions
    }
    case 0x09: {                                                          // 00525741 UseItem
        Vector position{0.0f, 0.0f, 0.0f};                                // [esp+38..40]
        const U item = kotor_fastcall_00578540(message, nullptr);         // EBP
        U slot6c = low_byte_into(player_slot, kotor_fastcall_004d6720(message, nullptr, 8));   // byte [esp+6C]
        U slot70 = minor_arg;
        if (kotor_fastcall_004d66f0(message, nullptr) != 0)               // 00525771..78
            slot70 = low_byte_into(slot70, kotor_fastcall_004d6720(message, nullptr, 8)); // byte [esp+70]
        else
            slot70 = low_byte_into(slot70, 0);                            // 00525789
        U target;
        if (kotor_fastcall_004d66f0(message, nullptr) == 1) target = kotor_fastcall_00578540(message, nullptr); // 0052578E..A1
        else target = k_invalid_00746644;                                 // 005257A5
        if (kotor_fastcall_004d66f0(message, nullptr) == 1) {             // 005257AB..B5
            position.x = kotor_fastcall_004d67b0(message, nullptr, 0x3f800000u, 0x20);
            position.y = kotor_fastcall_004d67b0(message, nullptr, 0x3f800000u, 0x20);
            position.z = kotor_fastcall_004d67b0(message, nullptr, 0x3f800000u, 0x20);
        }
        if (recovered_player_message::overrun_004d6230(message) != 0) return 0;
        if (recovered_player_message::underrun_004d6240(message) != 0) return 0;
        if (creature == nullptr) return 0;
        void* area = recovered_object_area_004cb120(creature, nullptr);   // 00525817..1E (unchecked)
        const U area_id = at<U>(area, 0x120);
        if (at<U>(creature, 0x4e0) == 0) w5_creature_action_manager_004f8770(creature, nullptr, 1); // 00525829..37
        if (kotor_fastcall_004fc210(creature, nullptr, item, slot6c, slot70, target, &position, area_id, 1) == 0) // 0052583C..59
            recovered_creature_feedback_004ede10(creature, nullptr, 0x17, nullptr);                  // 0052585F..64
        return 0;
    }
    case 0x0a: {                                                          // 00525915 ToggleMode / counterspell
        const U8 mode = kotor_fastcall_004d6720(message, nullptr, 8);     // BL
        const U slot70 = low_byte_into(minor_arg, mode);                  // byte [esp+70]
        if (recovered_player_message::overrun_004d6230(message) != 0) return 0;
        const U target = mode == 5 ? kotor_fastcall_00578540(message, nullptr) : slot70;   // 00525935..45 EBP
        if (recovered_player_message::overrun_004d6230(message) != 0) return 0;
        if (recovered_player_message::underrun_004d6240(message) != 0) return 0;
        if (creature == nullptr) return 0;
        if (at<U>(creature, 0xe8) == 0) return 0;                         // 00525973..7B
        if (mode == 5) (void)kotor_fastcall_004fce90(creature, nullptr, target); // 00525981..89
        (void)kotor_fastcall_004f2a50(creature, nullptr, slot70);         // 0052598E..95
        return 0;
    }
    case 0x03: {                                                          // 00525B68 ChangeDoorState (G18, t2)
        const U door = kotor_fastcall_00578540(message, nullptr);         // 00525B68..6A ReadOBJECTIDServer
        const U state = kotor_fastcall_004d6750(message, nullptr, 0x10);   // 00525B6F..75 ReadWORD(16)
        if (recovered_player_message::overrun_004d6230(message) != 0) return 0;   // 00525B7A..87
        if (recovered_player_message::underrun_004d6240(message) != 0) return 0;  // 00525B8D..98
        if (creature == nullptr) return 0;                                // 00525B9E..A0
        w5_creature_action_manager_004f8770(creature, nullptr, 2);        // 00525BA6..AA
        if (static_cast<U16>(state) == 0x2725) w5_object_add_open_door_action_004cfac0(creature, nullptr, door, 0); // 00525BAF..BB
        else w5_object_add_close_door_action_004cfb20(creature, nullptr, door, 0);  // 00525BCA
        return 1;
    }
    case 0x0b: {                                                          // 00525B1A UseObject (G18, t2)
        const U target = kotor_fastcall_00578540(message, nullptr);       // 00525B1A..1C
        if (recovered_player_message::overrun_004d6230(message) != 0) return 0;   // 00525B21..2E
        if (recovered_player_message::underrun_004d6240(message) != 0) return 0;  // 00525B34..3F
        if (creature == nullptr) return 0;                                // 00525B45..47
        w5_creature_action_manager_004f8770(creature, nullptr, 2);        // 00525B4D..51
        w5_object_add_use_object_action_0057c810(creature, nullptr, target);       // 00525B56..59
        return 1;
    }
    case 0x0c: {                                                          // 00525BD9 Unlock
        const U target = kotor_fastcall_00578540(message, nullptr);       // EBX
        const U key = kotor_fastcall_00578540(message, nullptr);          // EBP
        if (recovered_player_message::overrun_004d6230(message) != 0) return 0;
        if (recovered_player_message::underrun_004d6240(message) != 0) return 0;
        if (creature == nullptr) return 0;
        if (at<U>(creature, 0xe8) == 0) return 0;                         // 00525C15..1D
        w5_creature_action_manager_004f8770(creature, nullptr, 2);        // 00525C23..27
        void* found = nullptr;
        if (static_cast<U8>(kotor_fastcall_004d8230(objects(), nullptr, target, &found)) == 0) { // 00525C2C..4D
            if (vcall<void*>(found, 0x18) != nullptr && at<U>(vcall<void*>(found, 0x18), 0x2c4) != 0) // 00525C4F..6D door locked
                (void)kotor_fastcall_004cfb80(creature, nullptr, target, key, 0);
            if (vcall<void*>(found, 0x48) != nullptr && at<U>(vcall<void*>(found, 0x48), 0x260) != 0) // 00525C7A..98 placeable
                (void)kotor_fastcall_004cfb80(creature, nullptr, target, 0x7f000000u, 0);
        }
        return 1;
    }
    case 0x0d:                                                            // 00526003 Rest
        if (creature != nullptr && at<U>(creature, 0xe8) != 0) {
            if (kotor_fastcall_004eb190(creature, nullptr, 8) == 0) kotor_fastcall_004fd1e0(creature, nullptr, 0); // 00526011..25
            return 0;
        }
        recovered_creature_feedback_004ede10(creature, nullptr, 0xd5, nullptr);    // 0052602F..38 (creature may be null: original)
        return 0;
    case 0x0e: {                                                          // 00525CB3 Lock
        const U target = kotor_fastcall_00578540(message, nullptr);
        if (recovered_player_message::overrun_004d6230(message) != 0) return 0;
        if (recovered_player_message::underrun_004d6240(message) != 0) return 0;
        if (creature == nullptr) return 0;
        w5_creature_action_manager_004f8770(creature, nullptr, 2);        // 00525CE6..EA
        void* found = nullptr;
        if (static_cast<U8>(kotor_fastcall_004d8230(objects(), nullptr, target, &found)) == 0) { // 00525CEF..0F
            if (vcall<void*>(found, 0x18) != nullptr && at<U>(vcall<void*>(found, 0x18), 0x2c4) == 0)
                (void)kotor_fastcall_004cfc20(creature, nullptr, target);  // 00525D11..34
            if (vcall<void*>(found, 0x48) != nullptr && at<U>(vcall<void*>(found, 0x48), 0x260) == 0)
                (void)kotor_fastcall_004cfc20(creature, nullptr, target);  // 00525D39..5C
        }
        return 1;
    }
    case 0x10:                                                            // 00526042 falls into 11h
        (void)kotor_fastcall_004d6720(message, nullptr, 8);
        (void)kotor_fastcall_004d6780(message, nullptr, 0x20);
        [[fallthrough]];
    case 0x11:                                                            // 00526054
        (void)kotor_fastcall_004d6720(message, nullptr, 8);
        (void)kotor_fastcall_004d6720(message, nullptr, 8);
        (void)kotor_fastcall_004d6720(message, nullptr, 8);
        if (recovered_player_message::overrun_004d6230(message) != 0) return 0;
        (void)recovered_player_message::underrun_004d6240(message);       // 00526080..89 result unused
        return 0;
    case 0x12: {                                                          // 0052608E CastSpell
        Vector position{0.0f, 0.0f, 0.0f};                                // [esp+38..40]
        U has_position = 0;                                               // EBP
        U target = 0x7f000000u;                                           // [esp+70]
        const U spell = kotor_fastcall_004d6780(message, nullptr, 0x20);  // [esp+28]
        const U8 b1 = kotor_fastcall_004d6720(message, nullptr, 8);       // byte [esp+14]
        const U8 b2 = kotor_fastcall_004d6720(message, nullptr, 8);       // byte [esp+18]
        const U8 b3 = kotor_fastcall_004d6720(message, nullptr, 8);       // byte [esp+6C]
        const U slot6c = low_byte_into(player_slot, b3);
        const U8 flags = kotor_fastcall_004d6720(message, nullptr, 8);    // BL
        if (flags & 1) target = kotor_fastcall_00578540(message, nullptr);    // 005260E3..EF
        if (flags & 2) {                                                   // 005260F3..2E
            position.x = kotor_fastcall_004d67b0(message, nullptr, 0x3f800000u, 0x20);
            position.y = kotor_fastcall_004d67b0(message, nullptr, 0x3f800000u, 0x20);
            position.z = kotor_fastcall_004d67b0(message, nullptr, 0x3f800000u, 0x20);
            has_position = 1;
        }
        const U skip = (flags >> 2) & 1;                                  // 00526133..3A
        if (recovered_player_message::overrun_004d6230(message) != 0) return 0;
        if (recovered_player_message::underrun_004d6240(message) != 0) return 0;
        if (creature == nullptr) return 0;
        if (stats == nullptr) return 0;                                   // 00526163..69
        (void)kotor_m15b_spell_array_get_spell_0059b6d0(at<void*>(kotor_cell_rules_007a3a28, 0x8c), nullptr,
                                                        static_cast<int>(spell)); // 0052616F..80 result unused
        if (skip != 0) return 0;                                          // 00526185..87
        // 0052618D..A1: the original pushes the whole [esp+14]/[esp+18] dwords (upper bytes uninitialised stack);
        // the byte values are the only defined content.
        if (recovered_stats_spell_uses_left_005a5550(stats, nullptr, spell, b1, b2, slot6c) == 0) return 0; // 005261A6..A8
        if (at<U>(creature, 0x4e0) != 1) w5_creature_action_manager_004f8770(creature, nullptr, 1); // 005261AE..BB
        (void)kotor_fastcall_004f9460(creature, nullptr, spell, b1, b2, b3, 0,
            bits(position.x), bits(position.y), bits(position.z), target, 1, has_position, 1, 0, 0, 0,
            0xffffffffu, 0xff, 0);                                        // 005261C0..0C
        return 0;
    }
    case 0x14: {                                                          // 0052633C CancelAction
        const U a = kotor_fastcall_004d6750(message, nullptr, 0x10);      // EBX
        const U b = kotor_fastcall_004d6750(message, nullptr, 0x10);      // EBP
        if (recovered_player_message::overrun_004d6230(message) != 0) return 0;
        if (creature == nullptr) return 0;
        (void)kotor_fastcall_004f1ef0(creature, nullptr, a, b);           // 0052636B..6F
        return 0;
    }
    case 0x15: {                                                          // 00526379 DebugInfo
        const U id = kotor_fastcall_00578540(message, nullptr);           // EDI
        if (recovered_player_message::overrun_004d6230(message) != 0) return 0;
        void* found = kotor_fastcall_004ae750(server(), nullptr, id);     // 00526393..A2
        if (found == nullptr) return 0;
        switch (at<U8>(found, 8)) {                                       // table 005268F4 (type-4)
        case 4: (void)kotor_fastcall_0056aa30(message, nullptr, player, id); break;   // 005263EB area
        case 5: (void)kotor_fastcall_00570230(message, nullptr, player, id); break;   // 005263C1 creature
        case 6: (void)kotor_fastcall_0056a7f0(message, nullptr, player, id); break;   // 005263CF item
        case 9: (void)kotor_fastcall_0056a8c0(message, nullptr, player, id); break;   // 005263DD placeable
        default: break;                                                               // 7, 8, out of range
        }
        return 0;
    }
    case 0x16: {                                                          // 005263F9 MergeItem / take into own inventory
        void* container = nullptr;                                        // [esp+18]
        U allowed = 0;                                                    // [esp+6C]
        const U first_id = kotor_fastcall_00578540(message, nullptr);     // EBX
        const U second_id = kotor_fastcall_00578540(message, nullptr);    // [esp+28]
        if (recovered_player_message::overrun_004d6230(message) != 0) return 0;
        if (creature == nullptr) return 0;
        if (at<U>(creature, 0xe8) == 0) return 0;
        void* first = recovered_item_lookup_004ae760(server(), nullptr, first_id);    // 0052643E..51
        void* second = recovered_item_lookup_004ae760(server(), nullptr, second_id);  // 00526453..63 [esp+70]
        if (first == nullptr || second == nullptr) return 0;
        U possessor = at<U>(second, 0x268);                               // 00526475
        if (at<U>(first, 0x268) == possessor && at<U>(creature, 4) == possessor) allowed = 1; // 0052647B..8D
        if (possessor != at<U>(creature, 4)) {                            // 00526498
            container = recovered_item_lookup_004ae760(server(), nullptr, possessor);    // 0052649D..AE
            if (container != nullptr && at<U>(container, 0x268) == at<U>(creature, 4)) allowed = 1; // 005264B4..BF
        } else {
            allowed = 1;                                                  // 005264BF
        }
        possessor = at<U>(second, 0x268);                                 // 005264C3..CA
        bool merge = false;
        if (**reinterpret_cast<U**>(static_cast<unsigned char*>(player) + 0x58) == possessor &&
            at<U>(first, 0x268) == at<U>(creature, 4)) {                  // 005264C7..DD
            if (container == nullptr) {                                   // 005264DF..E5 -> 0052651D
                void* placeable = recovered_placeable_lookup_004ae7a0(server(), nullptr, possessor); // 0052651D..2C
                if (placeable != nullptr) {
                    (void)recovered_placeable_remove_item_00584ac0(placeable, nullptr, second);       // 00526532..39
                    void* repository = recovered_creature_inventory_004ef770(creature, nullptr, 1);   // 0052653E..4D
                    if (recovered_inventory_insert_0055d330(repository, nullptr, &second, 0, 0, 1) == 0) { // 00526552..5B
                        (void)recovered_inventory_insert_0055d330(at<void*>(placeable, 0x36c), nullptr, &second, 0, 0, 1); // 0052655D..6C
                    } else {
                        recovered_item_owner_00553210(second, nullptr, at<U>(creature, 4), 1, 0, 0x7f000000u); // 00526576..87
                        merge = true;                                                                 // jmp 005264F7
                    }
                }
            } else if (at<U>(container, 0x14) != possessor) {             // 005264E7..EA
                merge = true;
            }
        }
        if (!merge && allowed != 1) return 0;                             // 005264EC..F1
        kotor_fastcall_004f0990(creature, nullptr, first, second);  // 005264F7..FF
        recovered_item_owner_00553210(first, nullptr, at<U>(creature, 4), 1, 0, 0x7f000000u); // 00526504..13
        return 0;
    }
    case 0x17: {                                                          // 00526591 SplitItem
        const U id = kotor_fastcall_00578540(message, nullptr);           // EBX
        const U count = kotor_fastcall_004d6750(message, nullptr, 0x10);  // EBP
        if (recovered_player_message::overrun_004d6230(message) != 0) return 0;
        if (creature == nullptr) return 0;
        if (at<U>(creature, 0xe8) == 0) return 0;
        void* item = recovered_item_lookup_004ae760(server(), nullptr, id);   // 005265CC..DB
        if (item == nullptr) return 0;
        kotor_fastcall_004f0a60(creature, nullptr, item, count & 0xffff); // 005265E3..EA movzx bp
        return 0;
    }
    case 0x18:                                                            // 005265F4 TogglePause
        if (creature == nullptr) return 0;
        if (at<U>(creature, 0x9dc) != 0) return 0;                        // unreachable after the prologue filter; original re-test
        if (recovered_object_area_004cb120(creature, nullptr) == nullptr) return 0;
        if (at<U>(recovered_server_info_004aec90(server(), nullptr), 0xc4) == 0) return 0;   // 00526619..2F
        kotor_fastcall_004ae960(server(), nullptr, 2);                    // 00526635..3F
        return 0;
    case 0x19: {                                                          // 00526649 SetPause
        const U value = kotor_fastcall_004d66f0(message, nullptr);        // EBX
        if (recovered_player_message::overrun_004d6230(message) != 0) return 0;
        if (recovered_player_message::underrun_004d6240(message) != 0) return 0;
        if (creature != nullptr && at<U>(creature, 0x9dc) != 0) return 0; // 00526674..80
        kotor_fastcall_004ae9a0(server(), nullptr, 2, value);             // 00526686..92
        return 0;
    }
    case 0x1a:                                                            // 005266B7
        (void)kotor_fastcall_004d66f0(message, nullptr);
        if (recovered_player_message::overrun_004d6230(message) != 0) return 0;
        at<U>(player, 0x60) = 1;                                          // 005266CF
        return 0;
    case 0x1c: {                                                          // 005266DB SetFacing
        Vector facing{0.0f, 0.0f, 0.0f};                                  // [esp+44..4C]
        facing.x = kotor_fastcall_004d67b0(message, nullptr, 0x3f800000u, 0x20);
        facing.y = kotor_fastcall_004d67b0(message, nullptr, 0x3f800000u, 0x20);
        vector_normalize_004ab130(&facing, nullptr);                      // 00526717..1B
        if (recovered_player_message::overrun_004d6230(message) != 0) return 0;
        void* self = kotor_fastcall_00561080(player, nullptr);            // 00526731..33 ESI
        if (self == nullptr) return 0;
        U dead = 0;                                                       // EBX
        if (vcall<void*>(self, 0x30) != nullptr) dead = recovered_creature_incapacitated_004ef890(vcall<void*>(self, 0x30), nullptr);
        if (at<U>(self, 0xe8) == 0) return 0;                             // 0052675F..67
        if (gui_load_slot_zero_0063e7f0(self, nullptr) == 1) return 0;    // 0052676D..77
        if (vcall<U>(self, 0x94) == 1) return 0;                          // 0052677D..8A
        if (dead == 1) return 0;
        if (creature != nullptr && at<U>(creature, 0x9dc) != 0) return 0; // 00526799..A5
        w1x_object_set_orientation_004cd170(self, nullptr, &facing);      // 005267AB..B2
        return 0;
    }
    case 0x1e: {                                                          // 005267B9 remove effect type 3Eh
        const U id = kotor_fastcall_00578540(message, nullptr);
        if (recovered_player_message::overrun_004d6230(message) != 0) return 0;
        void* target = kotor_fastcall_004ae770(server(), nullptr, id);    // 005267CF..DE
        if (target == nullptr) return 0;
        if (at<U>(target, 0x9dc) != 0) return 0;
        void* const list = static_cast<unsigned char*>(target) + 0x124;  // 005267EC (lea; never null)
        const I count = at<I>(list, 4);
        void** const data = at<void**>(list, 0);
        for (I i = 0; i < count; ++i) {                                   // 00526810..20
            void* effect = data[i];
            if (at<U16>(effect, 8) == k_effect_type_00746774) {
                (void)recovered_object_remove_effect_004d06c0(target, nullptr, at<U>(effect, 0), at<U>(effect, 4)); // 00526824..30
                return 0;
            }
        }
        return 0;
    }
    case 0x1f: {                                                          // 0052669C export character
        (void)kotor_fastcall_0056cac0(recovered_server_sws_message_004aedb0(server(), nullptr), nullptr, player);
        return 0;
    }
    case 0x23: {                                                          // 00526216 UseTalent
        Vector position{0.0f, 0.0f, 0.0f};                                // [esp+38..40]
        U target = 0x7f000000u;                                           // EBP
        const U talent = kotor_fastcall_004d6780(message, nullptr, 0x20); // EBX
        const U slot6c = low_byte_into(player_slot, kotor_fastcall_004d6720(message, nullptr, 8)); // byte [esp+6C]
        const U8 flags = kotor_fastcall_004d6720(message, nullptr, 8);    // byte [esp+70]
        if (flags & 1) target = kotor_fastcall_00578540(message, nullptr);
        if (flags & 2) {
            position.x = kotor_fastcall_004d67b0(message, nullptr, 0x3f800000u, 0x20);
            position.y = kotor_fastcall_004d67b0(message, nullptr, 0x3f800000u, 0x20);
            position.z = kotor_fastcall_004d67b0(message, nullptr, 0x3f800000u, 0x20);
        }
        if (recovered_player_message::overrun_004d6230(message) != 0) return 0;
        if (recovered_player_message::underrun_004d6240(message) != 0) return 0;
        if (creature == nullptr) return 0;
        if (stats == nullptr) return 0;
        (void)kotor_m15b_spell_array_get_spell_0059b6d0(at<void*>(kotor_cell_rules_007a3a28, 0x8c), nullptr,
                                                        static_cast<int>(talent)); // 005262D9..E5 result unused
        w5_creature_action_manager_004f8770(creature, nullptr, 1);        // 005262EA..EE
        if (target != k_invalid_00746644)
            (void)kotor_fastcall_00500ca0(creature, nullptr, 0, talent, target, 0xfe, k_invalid_00746644,
                                          0xffffffffu, slot6c, 0);        // 005262FE..11
        else
            (void)kotor_fastcall_004fd070(creature, nullptr, 0, talent, &position, 0xfe, k_invalid_00746644,
                                          0xffffffffu, slot6c, 0);        // 0052631B..32
        return 0;
    }
    case 0x24: {                                                          // 0052586E GiveItem
        const U creature_id = kotor_fastcall_00578540(message, nullptr);  // EBP
        const U item_id = kotor_fastcall_00578540(message, nullptr);      // EDI
        const U placeable_id = kotor_fastcall_00578540(message, nullptr); // EBX
        const U count = kotor_fastcall_004d6780(message, nullptr, 0x20);  // [esp+70]
        if (recovered_player_message::overrun_004d6230(message) != 0) return 0;
        if (recovered_player_message::underrun_004d6240(message) != 0) return 0;
        void* giver = kotor_fastcall_004ae770(server(), nullptr, creature_id);       // 005258B8..C7
        void* item = recovered_item_lookup_004ae760(server(), nullptr, item_id);     // 005258C9..D7
        void* placeable = recovered_placeable_lookup_004ae7a0(server(), nullptr, placeable_id); // 005258D7..E3
        if (giver == nullptr || item == nullptr || placeable == nullptr) return 0;
        (void)kotor_fastcall_0057c870(giver, nullptr, item_id, placeable_id, static_cast<I>(count), 0); // 00525900..0B
        return 0;
    }
    default: break;                                                       // minor 0 / > 24h -> 00526835
    }
    return 0;
}
}
