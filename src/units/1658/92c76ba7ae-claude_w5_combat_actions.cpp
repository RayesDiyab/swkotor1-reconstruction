// W5 queued-action bodies, lane worker w5-combat-actions:
//   005B6210 CSWSCreature::AIActionCombat        (+ the combat-round / queue-producer callees it needs)
//   00514270 CSWSCreature::AIActionCounterSpell
//   (mine actions and the rest: see the report; bodies whose own callee cascade exceeded the cap are not here)
// Transcribed from the original swkotor.exe (read-only); addresses in the comments are original instructions.
// Conventions (W5 t1..t3): thiscall -> extern "C" __fastcall(self, unused_edx, ...); x87 at 53-bit precision ->
// double intermediates, float exactly where the original stores a dword; live_operator_new / live_free only.
// Every callee binds to an existing selected provider of the c03 candidate map or is reconstructed below as
// kotor_fastcall_<addr>. No traps are introduced by this file.
#include "842d6b1618-w5_t3.hpp"
#include "5889fde5f1-w5_action_bodies.hpp"

using namespace w5;

namespace kotor_reconstruction {
// 005E5290 CExoString::operator==(const char*) [0077-borrowed-string-operations, thiscall member]
struct StartupString { int __thiscall BorrowedEquals005e5290(const char* text); };
}
namespace kotor_list { struct Internal; }
namespace base_ini_reconstruction {
void* __fastcall remove_head_005e9c80(kotor_list::Internal* list, void*);   // CExoLinkedListInternal::RemoveHead
}

extern "C" {
// ---------------- imports: existing providers (c03 candidate map; object in brackets) ----------------
U __fastcall frame_GetPauseState_004ae980(void* server, void*, U mask);                          // [0457]
void __fastcall kotor_m15e_slot090_void0_004015a0(void* self, void*);                            // void_return (~CSWSCombatRoundAction) [0832]
I __fastcall recovered_combat_round_has_current_action_004d3580(void* round, void*);             // GetActionPending [clear9]
void* __fastcall kotor_fastcall_004aede0(void* server, void*);                                   // GetWorldTimer [0457]
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, U* day, U* time);                // [0457]
U __fastcall kotor_fastcall_004fde40(void* creature, void*, U, U, U, U, U, U, U, U, U, U, U, U, U, U, U); // AddAttackActions [attack_producer]
U __fastcall recovered_creature_run_unequip_005023a0(void* creature, void*, U item, U repository, U a3, U a4, U a5); // RunUnequip [W1 b7]
void* __fastcall kotor_m15_menu_active_property_00553960(void* item, void*, I index);            // CSWSItem::GetActiveProperty
void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void* spells, void*, int id);         // CSWSpellArray::GetSpell [0820]
void* __fastcall kotor_fastcall_004ae910(void* server, void*, U id);                             // GetClientObjectByObjectId [0595]
void* __cdecl combat_reuse_message_004aedb0();                                                   // GetSWSMessage [W1 b7]
U __fastcall recovered_party_is_member_00563790(void* table, void*, U id);                       // CSWPartyTable::GetIsAvailable [w2_party]
unsigned char __fastcall recovered_item_can_equip_0051aa60(void* creature, void*, void* item, U* slot, U a, U b, U c); // CanEquipItem [1025]
void* __fastcall recovered_item_construct_005530a0(void* item, void*, U id);                     // CSWSItem::CSWSItem [0963]
void* __fastcall recovered_localized_assign_005e9e70(void* dst, void*, void* src);               // CExoLocString::operator= [0886]
U __fastcall recovered_module_register_tag_004c7de0(void* module, void*, void* tag, U id);       // AddObjectToLookupTable [1011]
U __fastcall recovered_item_in_repository_00555f40(void* repository, void*, void* item, U flag); // GetItemInRepository [w2_party]
U __fastcall recovered_remove_item_00555fd0(void* repository, void*, void* item);                // CItemRepository::RemoveItem [w2_party]
void __fastcall recovered_item_owner_00553210(void* item, void*, U owner, U a, U b, U c);        // CSWSItem::SetPossessor [1013]
U __fastcall recovered_creature_equip_004feb90(void* creature, void*, U slot, void* item, U a, U b, U c); // EquipItem [1035]
U __fastcall recovered_inventory_slot_from_item_005a4980(void* inventory, void*, void* item);    // GetSlotFromItem [W1 b7]
U __fastcall recovered_inventory_slot_message_0056fbd0(void* message, void*, U recipient, U id, U slot); // EquipCancel [clear9]
U __fastcall recovered_compare_item_00553cf0(void* item, void*, void* other);                    // CSWSItem::CompareItem [0886]
U __fastcall recovered_creature_unequip_item_004faa70(void* creature, void*, void* item);        // UnequipItem [W1 b7]
U __fastcall recovered_inventory_insert_0055d330(void* repository, void*, void** item, U a, U b, U c); // CItemRepository::AddItem
U __fastcall recovered_creature_remove_item_00510ef0(void* creature, void*, void* item, U a, U b, U c); // CSWSCreature::RemoveItem [w2_party]
void __fastcall recovered_item_add_to_area_00555720(void* item, void*, void* area, float x, float y, float z, U flag); // AddToArea [W1 b7]
void __fastcall recovered_creature_set_last_instant_equip_004efe00(void* creature, void*, U slot, U value); // [W1 b7]
void __fastcall w5_creature_set_combat_mode_0050ee80(void* creature, void*, U mode, U check);    // SetCombatMode [W5 t3 drive_control]
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* array, void*, int id);  // CSWBaseItemArray::GetBaseItem [0820]
void __fastcall kotor_fastcall_004d1d50(void* action, void*);                                    // CSWSCombatRoundAction::ClearData [round_queue]
void __fastcall kotor_fastcall_004d3660(void* round, void*, void* action, I index);              // CSWSCombatRound::AddAction [round_queue]
void __fastcall w5_object_clear_all_actions_004ccd80(void* object, void*, U clear_combat);       // ClearAllActions [W5 t3]
void* __fastcall kotor_fastcall_004cc630(void* object, void*, U type);                           // GetNodeById [producer_helpers]
int __fastcall recovered_stats_in_spell_like_005a68e0(void* stats, void*, U id);                 // GetIsInSpellLikeAbilityList
unsigned char __fastcall recovered_stats_spell_uses_left_005a5550(void* stats, void*, U a, U b, U c, U d); // GetSpellUsesLeft (AL)
void* __fastcall recovered_creature_visible_element_00517a20(void* creature, void*, U id);       // GetVisibleListElement [W1 b7]
void* __fastcall recovered_object_array_004aed70(void* server, void*);                           // GetObjectArray [w2_party]
void* __fastcall WindowCopy005e5b00(void* self, void*, void* other);                             // CExoString(const CExoString&) [0078]
void* __fastcall frame_LogAssign_005e5140(void* self, void*, const char* text);                  // CExoString::operator=(char*) [0459]
U __fastcall recovered_creature_add_move_to_point_action_004f8b60(void* creature, void*, U group, const Vector* point,
    U area_id, U target_id, U run, float range, float a6, U a7, U a8, U a9, U a10, U a11, U a12,
    const Vector* a13, U a14, U a15, U a16);                                                     // AddMoveToPointAction [vm_actions_t5]
void __fastcall kotor_fastcall_004fbbe0(void* creature, void*, U target);                        // BroadcastCombatStateToParty [sol-combat-relation]

// ---------------- exports reconstructed here (and, marked [owner ...], callees owned by other W5 files) ----------------
void* __fastcall kotor_fastcall_004d3630(void* round, void*);
void __fastcall kotor_fastcall_004d2b40(void* round, void*, U type);
void __fastcall kotor_fastcall_004d28f0(void* round, void*, U paused, U id);
void __fastcall kotor_fastcall_004d2920(void* round, void*, U time, U flag);
void __fastcall kotor_fastcall_004d39b0(void* round, void*, U, U, U, U, U, U, U, U, U, U, U, U, U, U, U, U);
void __fastcall kotor_fastcall_004d3a90(void* round, void*, U, U, U, U, U, U, U, U, U);
U __fastcall kotor_fastcall_0055d950(void* item, void*, void* source);   // CopyItem [owner claude_equip_items2]
void* __fastcall kotor_fastcall_0055f280(void* item, void*, I count);   // SplitItem [owner claude_w5_creature_actions]
U __fastcall kotor_fastcall_00501de0(void* creature, void*, U item_id, U slot, U value);   // RunEquip [owner claude_w5_creature_actions]
U __fastcall kotor_fastcall_004eb5a0(void* creature, void*, U group);   // AddPathfindingWaitActionToFront [owner claude_w5_movement_actions]
void __fastcall kotor_fastcall_004ef480(void* creature, void*, U time, U type); // StartGuiTimingBar [owner claude_w5_object_actions]
void __fastcall kotor_fastcall_004ec700(void* creature, void*, void* data);     // BroadcastSkillData [owner claude_w5_creature_actions]
}

namespace {
constexpr U k_in_progress_00745dbc = 1;   // .rdata 00745DBC
constexpr U k_complete_00745dc0 = 2;      // .rdata 00745DC0
constexpr U k_failed_00745dc4 = 3;        // .rdata 00745DC4
constexpr U k_requeue_00745dc8 = 4;       // .rdata 00745DC8
constexpr unsigned char k_lookup_ok_0074666c = 0;   // .rdata 0074666C (byte): CGameObjectArray status "found"
constexpr unsigned char k_object_type_bound_00746633 = 4; // .rdata 00746633 (byte)
constexpr U k_0_2f = 0x3e4ccccdu;         // 0.2 immediate
constexpr U k_0_1f = 0x3dcccccdu;         // 0.1 immediate
const float k_quarter_0073edf0 = 0.25f;   // .rdata 0073EDF0
const float k_tenth_0073e798 = 0.1f;      // .rdata 0073E798
const char k_forcehostile_0074a2ec[] = "FORCEHOSTILE";   // .rdata 0074A2EC
const char k_range_p_00746bb4[] = "P";    // .rdata 00746BB4 (personal)
const char k_range_t_00746bb0[] = "T";    // .rdata 00746BB0 (touch)
const char k_range_s_00746bac[] = "S";    // .rdata 00746BAC
const char k_range_m_00746ba8[] = "M";    // .rdata 00746BA8
const char k_range_l_00746ba4[] = "L";    // .rdata 00746BA4
const char k_range_w_00746ba0[] = "W";    // .rdata 00746BA0

inline U addr(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline void* as_object(void* o) { return vcall<void*>(o, 0x10); }      // vtbl+10 AsSWSObject
inline void* as_creature(void* o) { return vcall<void*>(o, 0x30); }    // vtbl+30 AsSWSCreature
inline void* round_of(void* c) { return at<void*>(c, 0x9c8); }          // CSWSCreature +9C8 CSWSCombatRound*
inline void* rules() { return kotor_cell_rules_007a3a28; }
inline U area_id_of(void* area) { return at<U>(area, 0x120); }          // CSWSArea +11C+4 (object id)
inline Vector vec_of_bits(U x, U y, U z) { Vector v; std::memcpy(&v.x, &x, 4); std::memcpy(&v.y, &y, 4); std::memcpy(&v.z, &z, 4); return v; }
inline U string_equals(void* s, const char* t) {
    return static_cast<U>(static_cast<kotor_reconstruction::StartupString*>(s)->BorrowedEquals005e5290(t));
}
// CExoString passed by value (two dwords): the callee owns and destroys it
struct ExoStringValue { U text; U length; };
static_assert(sizeof(ExoStringValue) == 8, "CExoString is 8 bytes");

// the AddAction / AddActionToFront (type, group, 13 pairs) shapes of the W5 producers
void add_action(void* o, U type, U group, const U* pairs) { w5_add_action_body(o, type, group, pairs); }
void add_action_front(void* o, U type, U group, const U* pairs) { w5_add_action_to_front_body(o, type, group, pairs); }

// destroy the round's current action (+9C8): inline ~CSWSCombatRoundAction (void_return) + free
void free_current_action(void* creature) {
    void* current = at<void*>(round_of(creature), 0x9c8);
    if (current != nullptr) {
        kotor_m15e_slot090_void0_004015a0(current, nullptr);
        live_free(current);
    }
    at<void*>(round_of(creature), 0x9c8) = nullptr;
}
}  // namespace

extern "C" {
// =========================================================================================================
// CSWSCombatRound members (round +944 current round time, +958 paused, +95C pausing object, +960 pause timer,
// +964 pause-timer mode, +968, +9B0 CExoLinkedList<CSWSCombatRoundAction>*, +9B4 owner creature, +9C8 current
// action, +9D0 current action type byte)
// ---------------------------------------------------------------------------------------------------------
// 004D3630 CSWSCombatRound::GetAction: RemoveHead, then null unless the action is due (+944 >= action+0).
// Quirk: a not-yet-due head is removed and leaked (the original drops it); callers test GetActionPending first.
void* __fastcall kotor_fastcall_004d3630(void* round, void*) {
    void* list = at<void*>(round, 0x9b0);                                           // 004D3633..3B
    if (list == nullptr) return nullptr;
    void* action = base_ini_reconstruction::remove_head_005e9c80(at<kotor_list::Internal*>(list, 0), nullptr); // 004D363D..3F
    if (action == nullptr) return nullptr;                                          // 004D3644..46
    if (at<I>(round, 0x944) < at<I>(action, 0)) return nullptr;                     // 004D3648..50 (jge: signed)
    return action;
}

// 004D2B40 CSWSCombatRound::SetCurrentAction(byte): RET 4
void __fastcall kotor_fastcall_004d2b40(void* round, void*, U type) {
    at<U8>(round, 0x9d0) = static_cast<U8>(type);                                   // 004D2B40..44
}

// 004D28F0 CSWSCombatRound::SetRoundPaused(paused, object id): RET 8
void __fastcall kotor_fastcall_004d28f0(void* round, void*, U paused, U id) {
    at<U>(round, 0x95c) = id;                                                       // 004D28F8
    const U mode = at<U>(round, 0x964);                                             // 004D28FE
    at<U>(round, 0x958) = paused;                                                   // 004D2906
    if (mode != 0 && paused == 0) at<U>(round, 0x964) = paused;                     // 004D290C..12
}

// 004D2920 CSWSCombatRound::SetPauseTimer(time, flag): RET 8
void __fastcall kotor_fastcall_004d2920(void* round, void*, U time, U flag) {
    at<U>(round, 0x964) = flag;                                                     // 004D2926
    if (flag != 0) { at<U>(round, 0x960) = time; return; }                          // 004D292C..38
    if (time == 0) at<U>(round, 0x960) = time;                                      // 004D293B..43
    const I current = at<I>(round, 0x960);                                          // 004D2949
    if (current > 0) at<U>(round, 0x960) = static_cast<U>(current) + time;          // 004D294F..55 (jle: signed)
    else at<U>(round, 0x960) = time;                                                // 004D295E
}

// 004D39B0 CSWSCombatRound::AddSWSpellAction(16 dwords): RET 40h. New 88h action, type 9, +4 = 2721h, +8 = 500.
void __fastcall kotor_fastcall_004d39b0(void* round, void*, U b1, U b2, U b3, U b4, U b5, U b6, U b7, U b8,
    U b9, U b10, U b11, U b12, U b13, U b14, U b15, U b16) {
    void* memory = live_operator_new(0x88);                                         // 004D39B1..B8
    void* a = nullptr;
    if (memory != nullptr) {                                                        // 004D39C4..C6
        at<U>(memory, 0x38) = 0; at<U>(memory, 0x3c) = 0; at<U>(memory, 0x40) = 0; // 004D39C8..CE (Vector ctor)
        kotor_fastcall_004d1d50(memory, nullptr);                                   // 004D39D1
        a = memory;
    }
    at<U>(a, 0x84) = b16; at<U>(a, 0x28) = b2; at<U>(a, 0x24) = b1;               // 004D39D8..F1
    at<U>(a, 0x30) = b4; at<U>(a, 0x2c) = b3; at<U>(a, 0x34) = b5;                  // 004D39F4..02
    at<U>(a, 0x38) = b6; at<U>(a, 0x3c) = b7; at<U>(a, 0x40) = b8;                  // 004D3A05..19
    at<U>(a, 0x48) = b10; at<U>(a, 0x4c) = b11; at<U8>(a, 0x50) = static_cast<U8>(b12); // 004D3A1C..32
    at<U>(a, 0x54) = b13; at<U>(a, 0x5c) = b14;                                     // 004D3A35..40
    at<U8>(a, 0x10) = 9; at<U16>(a, 4) = 0x2721; at<U>(a, 8) = 0x1f4;               // 004D3A47..51
    at<U>(a, 0x44) = b9; at<U8>(a, 0x60) = static_cast<U8>(b15);                    // 004D3A58..5B
    void* owner = at<void*>(round, 0x9b4);                                          // 004D3A5E
    if (at<U>(owner, 0x524) == k_object_invalid) at<U>(owner, 0x524) = b9;          // 004D3A64..70
    kotor_fastcall_004d3660(round, nullptr, a, -1);                                 // 004D3A76..7B
}

// 004D3A90 CSWSCombatRound::AddSWItemSpellAction(9 dwords): RET 24h. New 88h action, type 0Ah.
void __fastcall kotor_fastcall_004d3a90(void* round, void*, U c1, U c2, U c3, U c4, U c5, U c6, U c7, U c8, U c9) {
    void* memory = live_operator_new(0x88);                                         // 004D3A91..98
    void* a = nullptr;
    if (memory != nullptr) {
        at<U>(memory, 0x38) = 0; at<U>(memory, 0x3c) = 0; at<U>(memory, 0x40) = 0; // 004D3AA8..AE
        kotor_fastcall_004d1d50(memory, nullptr);                                   // 004D3AB1
        a = memory;
    }
    at<U>(a, 0x84) = c9; at<U>(a, 0x64) = c1; at<U>(a, 0x68) = c2; at<U>(a, 0x6c) = c3; // 004D3AB8..D4
    at<U>(a, 0x38) = c4; at<U>(a, 0x3c) = c5; at<U>(a, 0x40) = c6;                  // 004D3AD7..EB
    at<U8>(a, 0x10) = 0x0a; at<U>(a, 0x44) = c7; at<U>(a, 0x48) = c8;               // 004D3AEE..FD
    void* owner = at<void*>(round, 0x9b4);                                          // 004D3B00
    if (at<U>(owner, 0x524) == k_object_invalid) at<U>(owner, 0x524) = c7;          // 004D3B06..12
    kotor_fastcall_004d3660(round, nullptr, a, -1);                                 // 004D3B18..1D
}

// =========================================================================================================
// 004EB3A0 CSWSCreature::ComputeSpellRange(CExoString range by value, target id): RET 0Ch, ST0 float.
double __fastcall kotor_fastcall_004eb3a0(void* self, void*, ExoStringValue range, U target) {
    void* s = &range;
    float result = 0.0f;                                                            // [esp+8] = 0 (004EB3CA)
    if (string_equals(s, k_range_p_00746bb4) != 0) frame_LogAssign_005e5140(s, nullptr, k_range_t_00746bb0); // 004EB3B9..E4
    double value;
    void* r = rules();
    if (string_equals(s, k_range_p_00746bb4) != 0) value = at<float>(r, 4);         // 004EB3E9..00
    else if (string_equals(s, k_range_t_00746bb0) != 0) value = at<float>(r, 8);    // 004EB408..20
    else if (string_equals(s, k_range_s_00746bac) != 0) value = at<float>(r, 0xc);  // 004EB425..3D
    else if (string_equals(s, k_range_m_00746ba8) != 0) value = at<float>(r, 0x10); // 004EB442..59
    else if (string_equals(s, k_range_l_00746ba4) != 0) value = at<float>(r, 0x14); // 004EB45E..76
    else if (string_equals(s, k_range_w_00746ba0) != 0) value = at<float>(r, 0x18); // 004EB47B..93
    else value = result;                                                            // 004EB498 (the zeroed local)
    // 004EB49C..BE: local = (own personal space - 0.1) + table value
    result = static_cast<float>((static_cast<double>(at<float>(at<void*>(self, 0x340), 8)) - k_tenth_0073e798) + value);
    void* target_creature = kotor_fastcall_004ae770(server_app(), nullptr, target); // 004EB4B7..C0
    if (target_creature != nullptr)                                                 // 004EB4C5..C8
        result = static_cast<float>((static_cast<double>(at<float>(at<void*>(target_creature, 0x340), 8)) - k_tenth_0073e798) + result); // 004EB4CA..DD
    frame_LogStringDtor_005e5c20(s, nullptr);                                        // 004EB4E1..ED (callee-owned argument)
    return result;                                                                   // 004EB4F2 fld dword
}
}  // extern "C"

namespace {
// (Vector::operator-(target, self position)) squared length as 004F9840..62: (z*z + y*y) + x*x, doubles
double squared_distance_to(void* self, const Vector* target) {
    Vector d;
    kotor_cdecl_00423bc0(&d, target, static_cast<const Vector*>(static_cast<void*>(static_cast<unsigned char*>(self) + 0x90)));
    return (static_cast<double>(d.z) * d.z + static_cast<double>(d.y) * d.y) + static_cast<double>(d.x) * d.x;
}
// 004F9D11..50 / 004F93BF..FE: party combat message A5EDh for the creature's party slot
void party_combat_message(void* self) {
    void* client = client_app();                                                    // [[7A39FC]+4]
    const U client_id = kotor_fastcall_005eda50(client, nullptr, at<U>(self, 4));
    const I index = static_cast<I>(recovered_party_index_006349c0(kotor_fastcall_005ed8b0(client, nullptr), nullptr, client_id));
    if (index < 0) return;                                                          // jl
    void* party = kotor_fastcall_005ed8b0(client, nullptr);
    kotor_fastcall_006345e0(static_cast<unsigned char*>(party) + index * 0x88 + 0x24, nullptr, 0xa5ed);
}
// GetVisibleListElement gate shared by the two producers (004F961C..4D / 004F8DEE..23); false = return 0
bool target_visible(void* self, U target) {
    void* element = recovered_creature_visible_element_00517a20(self, nullptr, target);
    if (element == nullptr) return false;
    const U8 flags = at<U8>(element, 4);
    if ((flags & 0x0c) == 4) return false;
    if ((flags & 0x10) != 0 && (flags & 1) == 0) return false;
    return true;
}
}  // namespace

extern "C" {
// ---------------------------------------------------------------------------------------------------------
// 004F9460 CSWSCreature::AddCastSpellActions(18 dwords): RET 48h, EAX 0/1.
//   a1 spell, a2 class (FF = none, FE = spell-like ability), a3, a4, a5, a6..a8 target location, a9 target id,
//   a10, a11 (nonzero: remember the area instead of the target), a12 (0 = queue at the back), a13, a14 (byte),
//   a15 (1 = instant), a16, a17 (byte), a18 (0 = queue the round action only).
// Several argument slots are reused by the original as locals: a10/a18 keep the original target y/z, a17 the
// computed range, a13 the packed flags, a14/a15 the instant/flag words; kept as named locals here.
U __fastcall kotor_fastcall_004f9460(void* self, void*, U a1, U a2, U a3, U a4, U a5, U a6, U a7, U a8, U a9,
    U a10, U a11, U a12, U a13, U a14, U a15, U a16, U a17, U a18) {
    if (a18 == 0) {                                                                 // 004F9460..6F
        kotor_fastcall_004d39b0(round_of(self), nullptr, a1, a2, a3, a4, a5, a6, a7, a8, a9, a11, a13, a14, a15,
                                a16, a17, a10);                                     // 004F9475..D8
        if (at<U>(self, 0x4e0) == 0 &&                                              // 004F94DD..EA
            recovered_combat_round_has_current_action_004d3580(round_of(self), nullptr) == 0 && // 004F94EC..F9
            kotor_fastcall_004ae910(server_app(), nullptr, at<U>(self, 4)) != nullptr) { // 004F94FB..0E
            recovered_creature_compute_ai_state_004f1460(self, nullptr);            // 004F9510..12
            w5_object_clear_all_actions_004ccd80(self, nullptr, 1);                 // 004F9517..1A
        }
        if (kotor_fastcall_004cc630(self, nullptr, 0x3f) == nullptr) {             // 004F951F..2A
            U one = 1;                                                              // [esp+6C] (a14 slot)
            const U pairs[26] = {1, addr(&one)};
            add_action(self, 0x3f, 0xffff, pairs);                                  // 004F9530..61
        }
        return 1;                                                                   // 004F9566 / 004F9D8B
    }
    const U a17_byte = a17 & 0xff;                                                  // 004F9572 movzx -> [esp+18]
    at<U>(self, 0x524) = k_object_invalid; at<U>(self, 0x528) = k_object_invalid;   // 004F957D..88
    if (a13 == 0 && a5 == 0 && a2 != 0xff) {                                        // 004F9592..A5
        void* stats = at<void*>(self, 0xa74);
        if (a2 == 0xfe) {                                                           // 004F95A7..B2
            if (recovered_stats_in_spell_like_005a68e0(stats, nullptr, a1) == 0) return 0; // 004F95B4..C0
        } else if (recovered_stats_spell_uses_left_005a5550(stats, nullptr, a1, a2, a3, a4) == 0) {
            return 0;                                                               // 004F95CE..E5
        }
    }
    void* target_object = kotor_fastcall_004ae750(server_app(), nullptr, a9);       // 004F95E7..F5
    if (a13 == 0 && target_object != nullptr && as_creature(target_object) != nullptr && // 004F95FA..12
        at<U>(target_object, 4) != at<U>(self, 4) && at<U>(self, 0xa88) == 0) {      // 004F9614..24
        if (!target_visible(self, a9)) return 0;                                    // 004F9626..4D
    }
    Vector location = vec_of_bits(a6, a7, a8);                                      // a6..a8 (the position used below)
    Vector original = location;                                                     // [esp+10], a10 slot, a18 slot (004F9653..70)
    if (a9 != k_object_invalid) {                                                   // 004F9663..74
        void* found = nullptr;                                                      // [esp+20]
        if (static_cast<unsigned char>(kotor_fastcall_004d8230(recovered_object_array_004aed70(server_app(), nullptr),
                nullptr, a9, &found)) != k_lookup_ok_0074666c) return 0;            // 004F9676..97
        if (at<U8>(found, 8) <= k_object_type_bound_00746633) return 0;             // 004F969D..AA (jbe)
        void* found_area = recovered_object_area_004cb120(found, nullptr);          // 004F96B0
        if (found_area != recovered_object_area_004cb120(self, nullptr)) return 0;  // 004F96B5..C0
        copy_vector(&location, static_cast<unsigned char*>(found) + 0x90);          // 004F96C6..E0
    }
    void* spell = kotor_m15b_spell_array_get_spell_0059b6d0(at<void*>(rules(), 0x8c), nullptr, static_cast<int>(a1)); // 004F96E4..F4
    if (spell == nullptr) return 0;                                                 // 004F96F9..FD
    if (recovered_object_area_004cb120(self, nullptr) == nullptr) return 0;         // 004F9703..0C
    if (at<U>(self, 0xe8) == 0) return 0;                                           // 004F9712..1A
    U area_id = area_id_of(recovered_object_area_004cb120(self, nullptr));          // 004F9720..3A -> [esp+1C]
    ExoStringValue range_text;                                                      // the by-value argument slot
    WindowCopy005e5b00(&range_text, nullptr, static_cast<unsigned char*>(spell) + 0x20); // 004F9737..48
    float range = static_cast<float>(kotor_fastcall_004eb3a0(self, nullptr, range_text, a9)); // 004F974D..54 -> a17 slot
    U one_14 = 1;                                                                   // [esp+14]
    U flags = a14 & 0xff;                                                           // 004F975A..63 -> a13 slot
    if (a13 != 0) flags |= 0x80000000u;                                             // 004F9767..6E
    if (a15 != 0) flags |= 0x40000000u;                                             // 004F9772..7A
    U moved = 0;                                                                    // EBP
    U group = 0xffff;                                                               // EDI
    U one_24 = 1;                                                                   // [esp+24]
    U tx = bits(original.x), ty = bits(original.y), tz = bits(original.z);          // [esp+10], a10 slot, a18 slot
    U target = a9, range_bits = bits(range);
    U p1 = a1, p2 = a2, p3 = a3, p4 = a4, p5 = a5, p16 = a16, p18 = a17_byte;
    // the CastSpell (0Fh) parameter list (004F9799..808 / 004F99FF..6E / 004F9A7C..EF)
    auto cast_pairs = [&](U* q) {
        const U v[26] = {1, addr(&p1), 1, addr(&p2), 1, addr(&p3), 1, addr(&p4), 1, addr(&p5), 3, addr(&target),
                         2, addr(&tx), 2, addr(&ty), 2, addr(&tz), 1, addr(&flags), 1, addr(&p16), 1, addr(&p18), 0, 0};
        std::memcpy(q, v, sizeof(v));
    };
    U cast[26];
    if (a15 == 1) {                                                                 // 004F9784..93
        cast_pairs(cast);
        if (a12 == 0) add_action(self, 0x0f, group, cast);                          // 004F980C -> 004F9561
        else add_action_front(self, 0x0f, group, cast);                             // 004F9812
        return 1;                                                                   // 004F9817 (no +524 update)
    }
    U one_6c = 1;                                                                   // [esp+6C] (a14 slot)
    if (a12 == 0) {                                                                 // 004F9823..29
        const double d2 = squared_distance_to(self, &location);                     // 004F982F..62
        const double r2 = static_cast<double>(range) * range;                       // 004F9864..68
        if (r2 < d2) {                                                              // 004F986C..73 (test ah,5; jp)
            const Vector zero = vec_of_bits(0, 0, 0);                               // [esp+28] reused
            recovered_creature_add_move_to_point_action_004f8b60(self, nullptr, 0xffff, &location, area_id, a9, 1,
                range, 0.0f, 0, 0, 0, 0, 0, 0, &zero, 0, 0, 1);                     // 004F9879..C8
            U one_70 = 1;                                                           // [esp+70] (a15 slot)
            moved = 1; group = 0xfffe;                                              // 004F98EB..F2
            if (a9 != k_object_invalid) {                                           // 004F98CD / 004F98FB
                const U q[26] = {3, addr(&target), 1, addr(&one_14), 2, addr(&range_bits), 2, addr(&range_bits),
                                 1, addr(&one_24), 1, addr(&one_70), 1, addr(&one_6c)};
                add_action(self, 0x11, group, q);                                   // 004F98FD..83
            } else {
                const U q[26] = {2, addr(&tx), 2, addr(&ty), 2, addr(&tz), 3, addr(&area_id), 3, addr(&target),
                                 1, addr(&one_14), 2, addr(&range_bits)};
                add_action(self, 0x12, group, q);                                   // 004F993E..83
            }
        }
        if (a9 != k_object_invalid) {                                               // 004F9996..B8
            const U q[26] = {3, addr(&target)};
            add_action(self, 0x13, group, q);                                       // 004F99BA..F1
        } else {
            const U q[26] = {2, addr(&tx), 2, addr(&ty), 2, addr(&tz)};
            add_action(self, 0x31, group, q);                                       // 004F99D1..F1
        }
        if (moved == 0) group = 0xfffe;                                             // 004F99F6..FA
        cast_pairs(cast);
        add_action(self, 0x0f, group, cast);                                        // 004F99FF..72
    } else {
        cast_pairs(cast);
        add_action_front(self, 0x0f, 0xffff, cast);                                 // 004F9A7C..F3
        U zero_70 = 0;                                                              // [esp+70] (a15 slot) = 0 (004F9B26)
        if (a9 != k_object_invalid) {                                               // 004F9B06..33
            const U q1[26] = {3, addr(&target)};
            add_action_front(self, 0x13, 0xfffe, q1);                               // 004F9B39..54
            const U q2[26] = {3, addr(&target), 1, addr(&one_14), 2, addr(&range_bits), 2, addr(&range_bits),
                              1, addr(&one_24), 1, addr(&zero_70), 1, addr(&one_6c)};
            add_action_front(self, 0x11, 0xfffe, q2);                               // 004F9B59..C45
        } else {
            const U q1[26] = {2, addr(&tx), 2, addr(&ty), 2, addr(&tz)};
            add_action_front(self, 0x31, 0xfffe, q1);                               // 004F9BB9..DD
            // quirk: the front variant appends a float parameter whose storage is the integer 1 ([esp+6C])
            const U q2[26] = {2, addr(&tx), 2, addr(&ty), 2, addr(&tz), 3, addr(&area_id), 3, addr(&target),
                              1, addr(&one_14), 2, addr(&range_bits), 2, addr(&one_6c)};
            add_action_front(self, 0x12, 0xfffe, q2);                               // 004F9BE2..C45
        }
        const double d2 = squared_distance_to(self, &location);                     // 004F9C4A..7D
        const float d2f = static_cast<float>(d2);                                   // 004F9C7F fst [esp+64]
        const double r2 = static_cast<double>(range) * range;                       // 004F9C83..87
        if (d2 > r2) {                                                              // 004F9C8B..94 (test ah,41h; jne)
            const Vector zero = vec_of_bits(0, 0, 0);
            w5_creature_add_move_to_point_action_to_front_004f8a50(self, nullptr, 0xfffe, &location, area_id, a9,
                one_14, range, 0.0f, 0, 0, 0, 0, 0, 0, &zero, 0, 0, one_6c);        // 004F9C9A..F1
            if (at<U>(self, 0xa88) != 0 && static_cast<double>(d2f) > k_quarter_0073edf0) // 004F9CF6..0F
                party_combat_message(self);                                         // 004F9D11..50
        }
    }
    if (a11 == 0) { at<U>(self, 0x524) = a9; return 1; }                            // 004F9D55..67
    at<U>(self, 0x524) = area_id_of(recovered_object_area_004cb120(self, nullptr)); // 004F9D73..85
    return 1;
}

// ---------------------------------------------------------------------------------------------------------
// 004F8C70 CSWSCreature::AddItemCastSpellActions(11 dwords): RET 2Ch, EAX 0/1.
//   b1 item id, b2 active property index, b3, b4..b6 target location, b7 target id, b8, b9 (nonzero: remember the
//   area), b10 (0 = queue at the back), b11 (0 = queue the round action only).
U __fastcall kotor_fastcall_004f8c70(void* self, void*, U b1, U b2, U b3, U b4, U b5, U b6, U b7, U b8, U b9,
    U b10, U b11) {
    if (b11 == 0) {                                                                 // 004F8C73..87
        kotor_fastcall_004d3a90(round_of(self), nullptr, b1, b2, b3, b4, b5, b6, b7, b9, b8); // 004F8C8D..CA
        if (at<U>(self, 0x4e0) == 0 &&                                              // 004F8CCF..D7
            recovered_combat_round_has_current_action_004d3580(round_of(self), nullptr) != 0 && // 004F8CD9..E6 (je)
            kotor_fastcall_004ae910(server_app(), nullptr, at<U>(self, 4)) != nullptr) { // 004F8CE8..FC
            recovered_creature_compute_ai_state_004f1460(self, nullptr);            // 004F8CFE..00
            w5_object_clear_all_actions_004ccd80(self, nullptr, 1);                 // 004F8D05..08
        }
        if (kotor_fastcall_004cc630(self, nullptr, 0x3f) == nullptr) {             // 004F8D0D..18
            U one = 1;                                                              // [esp+58] (b9 slot)
            const U pairs[26] = {1, addr(&one)};
            add_action(self, 0x3f, 0xffff, pairs);                                  // 004F8D1A..4B
        }
        return 1;                                                                   // 004F8D50
    }
    U tx = b4, ty = b5, tz = b6;                                                    // [esp+10], [esp+0C], b8 slot (004F8D5B..86)
    at<U>(self, 0x524) = k_object_invalid; at<U>(self, 0x528) = k_object_invalid;   // 004F8D7A..80
    Vector location = vec_of_bits(b4, b5, b6);
    if (b7 != k_object_invalid) {                                                   // 004F8D70..8A
        void* o = kotor_fastcall_004ae750(server_app(), nullptr, b7);               // 004F8D90..9A
        if (o == nullptr) return 0;                                                 // 004F8DA1..A3
        void* o_area = recovered_object_area_004cb120(o, nullptr);                  // 004F8DA9..AB
        if (o_area != recovered_object_area_004cb120(self, nullptr)) return 0;      // 004F8DB0..BB
        copy_vector(&location, static_cast<unsigned char*>(o) + 0x90);              // 004F8DC1..D7
        if (as_creature(o) != nullptr && at<U>(o, 4) != at<U>(self, 4) && at<U>(self, 0xa88) == 0) { // 004F8DDB..F6
            if (!target_visible(self, b7)) return 0;                                // 004F8DF8..23
        }
    }
    void* item = recovered_item_lookup_004ae760(server_app(), nullptr, b1);         // 004F8E2E..3B
    if (item == nullptr) return 0;                                                  // 004F8E40..42
    void* property = kotor_m15_menu_active_property_00553960(item, nullptr, static_cast<I>(b2)); // 004F8E48..4F
    // quirk: without an active property the spell id is the target object id (b7)
    const U spell_id = property != nullptr ? at<U16>(property, 2) : b7;             // 004F8E54..5E
    void* spell = kotor_m15b_spell_array_get_spell_0059b6d0(at<void*>(rules(), 0x8c), nullptr, static_cast<int>(spell_id)); // 004F8E62..6F
    if (spell == nullptr) return 0;                                                 // 004F8E74..78
    if (recovered_object_area_004cb120(self, nullptr) == nullptr) return 0;         // 004F8E7E..87
    if (at<U>(self, 0xe8) == 0) return 0;                                           // 004F8E8D..95
    U area_id = area_id_of(recovered_object_area_004cb120(self, nullptr));          // 004F8E9B..BC -> [esp+1C]
    ExoStringValue range_text;
    WindowCopy005e5b00(&range_text, nullptr, static_cast<unsigned char*>(spell) + 0x20); // 004F8EB2..C0
    float range = static_cast<float>(kotor_fastcall_004eb3a0(self, nullptr, range_text, b7)); // 004F8EC5..CC -> b11 slot
    U one_18 = 1;                                                                   // [esp+18] = EBP (1)
    U moved = 0;                                                                    // EBP
    U group = 0xffff;                                                               // EDI
    U one_20 = 1;                                                                   // [esp+20]
    U one_14 = 1;                                                                   // [esp+14] (004F8C83)
    U target = b7, range_bits = bits(range), p1 = b1, p2 = b2, p3 = b3;
    // the ItemCastSpell (2Eh) parameter list (004F90E0..3D / 004F9147..A8)
    const U item_pairs[26] = {3, addr(&p1), 1, addr(&p2), 1, addr(&p3), 3, addr(&target),
                              2, addr(&tx), 2, addr(&ty), 2, addr(&tz)};
    if (b10 == 0) {                                                                 // 004F8ED0..E9
        const double d2 = squared_distance_to(self, &location);                     // 004F8EEF..22
        const double r2 = static_cast<double>(range) * range;                       // 004F8F24..28
        if (r2 < d2) {                                                              // 004F8F2C..33 (test ah,5; jp)
            const Vector zero = vec_of_bits(0, 0, 0);
            recovered_creature_add_move_to_point_action_004f8b60(self, nullptr, group, &location, area_id, b7, 1,
                range, 0.0f, 0, 0, 0, 0, 0, 0, &zero, 0, 0, one_14);                // 004F8F35..70
            moved = 1; group = 0xfffe;                                              // 004F8F75..7A
        }
        if (b7 != k_object_invalid) {                                               // 004F8F7F..9B
            U zero_5c = 0;                                                          // [esp+5C] (b10 slot) = 0 (004F8FE7)
            const U q1[26] = {3, addr(&target), 1, addr(&one_18), 2, addr(&range_bits), 2, addr(&range_bits),
                              1, addr(&one_20), 1, addr(&zero_5c), 1, addr(&one_14)};
            add_action(self, 0x11, group, q1);                                      // 004F8FA1..F2
            if (moved == 0) group = 0xfffe;                                         // 004F8FF7..FB
            const U q2[26] = {3, addr(&target)};
            add_action(self, 0x13, group, q2);                                      // 004F9000..3D -> 004F90DB
        } else {
            const U q1[26] = {2, addr(&tx), 2, addr(&ty), 2, addr(&tz), 3, addr(&area_id), 3, addr(&target),
                              1, addr(&one_18), 2, addr(&range_bits), 1, addr(&one_14)};
            add_action(self, 0x12, group, q1);                                      // 004F9042..88
            if (moved == 0) group = 0xfffe;                                         // 004F908D..91
            const U q2[26] = {2, addr(&tx), 2, addr(&ty), 2, addr(&tz)};
            add_action(self, 0x31, group, q2);                                      // 004F9096..DB
        }
        add_action(self, 0x2e, group, item_pairs);                                  // 004F90E0..3D
    } else {
        add_action_front(self, 0x2e, 0xffff, item_pairs);                           // 004F9147..A8
        if (b7 != k_object_invalid) {                                               // 004F91BB..DD
            const U q1[26] = {3, addr(&target)};
            add_action_front(self, 0x13, 0xfffe, q1);                               // 004F91E3..FE
            U zero_5c = 0;                                                          // [esp+5C] = 0 (004F925D)
            const U q2[26] = {3, addr(&target), 1, addr(&one_18), 2, addr(&range_bits), 2, addr(&range_bits),
                              1, addr(&one_20), 1, addr(&zero_5c), 1, addr(&one_14)};
            add_action_front(self, 0x11, 0xfffe, q2);                               // 004F9203..6A -> 004F92F3
        } else {
            const U q1[26] = {2, addr(&tx), 2, addr(&ty), 2, addr(&tz)};
            add_action_front(self, 0x31, 0xfffe, q1);                               // 004F926F..90
            const U q2[26] = {2, addr(&tx), 2, addr(&ty), 2, addr(&tz), 3, addr(&area_id), 3, addr(&target),
                              1, addr(&one_18), 2, addr(&range_bits), 1, addr(&one_14)};
            add_action_front(self, 0x12, 0xfffe, q2);                               // 004F9295..F3
        }
        const double d2 = squared_distance_to(self, &location);                     // 004F92F8..2B
        const float d2f = static_cast<float>(d2);                                   // 004F932D fst [esp+5C]
        const double r2 = static_cast<double>(range) * range;                       // 004F9331..35
        if (d2 > r2) {                                                              // 004F9339..42 (test ah,41h; jne)
            const Vector zero = vec_of_bits(0, 0, 0);
            w5_creature_add_move_to_point_action_to_front_004f8a50(self, nullptr, 0xfffe, &location, area_id, b7,
                one_18, range, 0.0f, 0, 0, 0, 0, 0, 0, &zero, 0, 0, one_14);        // 004F9348..9F
            if (at<U>(self, 0xa88) != 0 && static_cast<double>(d2f) > k_quarter_0073edf0) // 004F93A4..BD
                party_combat_message(self);                                         // 004F93BF..FE
        }
    }
    if (b9 == 0) { at<U>(self, 0x524) = b7; return 1; }                             // 004F9403..15
    at<U>(self, 0x524) = area_id_of(recovered_object_area_004cb120(self, nullptr)); // 004F9423..38
    return 1;
}

// =========================================================================================================


// ---------------------------------------------------------------------------------------------------------

// =========================================================================================================
// 005B6210 CSWSCreature::AIActionCombat(node): RET 4. Runs the next due CSWSCombatRoundAction of the round.
U __fastcall w5_action_combat_005b6210(void* self, void*, void* node) {
    void* round = round_of(self);
    if (at<U>(round, 0x958) != 0) return k_in_progress_00745dbc;                    // 005B622E..3E (round paused)
    if (frame_GetPauseState_004ae980(server_app(), nullptr, 2) != 0) return k_in_progress_00745dbc; // 005B6244..55
    {
        void* current = at<void*>(round_of(self), 0x9c8);                           // 005B625B..69
        if (current != nullptr) {
            kotor_m15e_slot090_void0_004015a0(current, nullptr);                    // 005B6271
            live_free(current);                                                     // 005B6276..7C
            at<void*>(round_of(self), 0x9c8) = nullptr;                             // 005B627F..85
        }
    }
    if (recovered_combat_round_has_current_action_004d3580(round_of(self), nullptr) == 0 || // 005B628B..98
        at<I>(round_of(self), 0x968) >= 1) {                                        // 005B629E..AD (jge)
        (void)recovered_combat_round_has_current_action_004d3580(round_of(self), nullptr); // 005B690E..14 (result unused)
        return at<U>(round_of(self), 0x934) == 0 ? k_complete_00745dc0 : k_in_progress_00745dbc; // 005B6919..2C
    }
    at<U>(node, 0x38) = 0;                                                          // 005B62B3..B7
    void* a = kotor_fastcall_004d3630(round_of(self), nullptr);                     // 005B62BA..C0
    if (a == nullptr) return k_in_progress_00745dbc;                                // 005B62C7..C9
    const U target = at<U>(a, 0x14);                                                // [esp+10]
    const U word4 = at<U16>(a, 4);                                                  // [esp+20] (word)
    const U actc = at<U>(a, 0xc);                                                   // [esp+1C]
    const U act8 = at<U>(a, 8);                                                     // EBX
    // [esp+28] receives only the word at +5C; the dword read at 005B63CE carries an unwritten high word in the
    // original frame. Its consumer (AddAttackActions argument 2, the group) is passed zero-extended here.
    const U word5c = at<U16>(a, 0x5c);
    const U8 type = at<U8>(a, 0x10);                                                // [esp+14] (byte)
    const U act74 = at<U>(a, 0x74);                                                 // [esp+24]
    at<void*>(round_of(self), 0x9c8) = a;                                           // 005B62FB..0D
    kotor_fastcall_004d2b40(round_of(self), nullptr, type);                         // 005B6313..1A
    if (type != 0x0a) recovered_set_activity_004f00a0(self, nullptr, 1, 0, 0);      // 005B631F..2E
    switch (type) {                                                                 // 005B6333..46 (table 005B6948, index type-1)
    case 1:
    case 11: {                                                                      // 005B634D
        U day = 0, time = 0;                                                        // [esp+14], [esp+68] (node slot)
        frame_GetWorldTime_004ade40(kotor_fastcall_004aede0(server_app(), nullptr), nullptr, &day, &time); // 005B635C..70 (unused)
        if (act74 != 0) {                                                           // 005B6375..79
            kotor_fastcall_004fde40(self, nullptr, target, 0, 0, 0, 0, 1, 1, type, word4, act8, actc, 1,
                                    at<U>(a, 0x78), at<U>(a, 0x7c), at<U>(a, 0x80)); // 005B637B..AC
        } else {
            kotor_fastcall_004fde40(self, nullptr, target, word5c, 0, 0, 0, 1, 1, type, word4, act8, actc, 0, 0, 4, 0); // 005B63BB..E4
        }
        return k_requeue_00745dc8;                                                  // 005B63B1 / 005B63E9
    }
    case 3:                                                                         // 005B63F3: pause
        kotor_fastcall_004d28f0(round_of(self), nullptr, 1, at<U>(self, 4));
        kotor_fastcall_004d2920(round_of(self), nullptr, act8, 0);
        return k_in_progress_00745dbc;
    case 6:                                                                         // 005B6417: equip
        kotor_fastcall_004d28f0(round_of(self), nullptr, 1, at<U>(self, 4));
        kotor_fastcall_004d2920(round_of(self), nullptr, act8, 0);
        kotor_fastcall_00501de0(self, nullptr, target, at<U>(a, 0x1c), at<U>(a, 0x54)); // 005B6436..45
        return k_in_progress_00745dbc;
    case 7:                                                                         // 005B644F: unequip
        kotor_fastcall_004d28f0(round_of(self), nullptr, 1, at<U>(self, 4));
        kotor_fastcall_004d2920(round_of(self), nullptr, act8, 0);
        recovered_creature_run_unequip_005023a0(self, nullptr, target, at<U>(a, 0x20), 1, 0, at<U>(a, 0x54)); // 005B646E..81
        return k_in_progress_00745dbc;
    case 9:                                                                         // 005B648B: cast spell
        kotor_fastcall_004f9460(self, nullptr, at<U>(a, 0x24), at<U>(a, 0x28), at<U>(a, 0x2c), at<U>(a, 0x30),
            at<U>(a, 0x34), at<U>(a, 0x38), at<U>(a, 0x3c), at<U>(a, 0x40), at<U>(a, 0x44), at<U>(a, 0x84),
            at<U>(a, 0x48), 1, at<U>(a, 0x4c), at<U8>(a, 0x50), at<U>(a, 0x54), at<U>(a, 0x5c), at<U8>(a, 0x60), 1); // 005B648B..E4
        free_current_action(self);                                                  // 005B64E9..0F
        return k_requeue_00745dc8;                                                  // 005B6519
    case 10: {                                                                      // 005B6523: item cast spell
        void* caster = kotor_fastcall_004ae750(server_app(), nullptr, at<U>(a, 0x44)); // 005B6523..30 (target object)
        if (caster != nullptr && as_object(caster) != nullptr && as_creature(caster) != nullptr && // 005B6539..51
            vcall<U>(as_creature(caster), 0x94) != 0)                               // 005B6553..66 (dead target)
            return k_in_progress_00745dbc;
        void* item = recovered_item_lookup_004ae760(server_app(), nullptr, at<U>(a, 0x64)); // 005B656C..78
        if (item != nullptr) {                                                      // 005B657D..7F
            void* property = kotor_m15_menu_active_property_00553960(item, nullptr, at<I>(a, 0x68)); // 005B6585..8B
            if (property != nullptr) {                                              // 005B6590..92
                const U spell_id = at<U16>(property, 2);                            // 005B6598 (EBX)
                alignas(4) unsigned char column[8];                                 // CExoString [esp+38]
                frame_LogStringCtor_005e5a90(column, nullptr, k_forcehostile_0074a2ec); // 005B659C..A9
                I hostile = 0;                                                      // [esp+68] (node slot) = 0 (005B65A5)
                c2da_current_get_int_00413660(at<void*>(at<void*>(rules(), 0xb8), 0xf4), nullptr, spell_id, column,
                                              &hostile);                            // 005B65AE..D1
                frame_LogStringDtor_005e5c20(column, nullptr);                      // 005B65D6..E2
                void* spell = kotor_m15b_spell_array_get_spell_0059b6d0(at<void*>(rules(), 0x8c), nullptr,
                                                                        static_cast<int>(spell_id)); // 005B65E7..F3
                if (spell != nullptr && at<U>(spell, 0x148) != 0)                   // 005B65F8..02
                    recovered_set_activity_004f00a0(self, nullptr, 1, 0, 0);        // 005B6604..0A
                if (hostile <= 0 && at<U>(a, 0x44) != at<U>(self, 4))               // 005B660F..1B (jg: signed)
                    at<U>(a, 0x44) = k_object_invalid;                              // 005B661D
            }
        }
        kotor_fastcall_004f8c70(self, nullptr, at<U>(a, 0x64), at<U>(a, 0x68), at<U>(a, 0x6c), at<U>(a, 0x38),
            at<U>(a, 0x3c), at<U>(a, 0x40), at<U>(a, 0x44), at<U>(a, 0x84), at<U>(a, 0x48), 1, 1); // 005B6624..5D
        free_current_action(self);                                                  // 005B6662..88
        return k_requeue_00745dc8;                                                  // 005B668E
    }
    case 12: {                                                                      // 005B6698: move to the target
        U area_id = area_id_of(recovered_object_area_004cb120(self, nullptr));      // 005B6698..AF -> [esp+18]
        void* t = kotor_fastcall_004ae750(server_app(), nullptr, at<U>(a, 0x14));   // 005B66AA..BA
        const U group = at<U16>(node, 0x6c);
        if (t != nullptr && as_object(t) != nullptr && at<U>(a, 0x14) != k_object_invalid) { // 005B66C1..DF
            U range = k_0_2f, one = 1, zero24 = 0, zero28 = 0;                      // [esp+20], [esp+1C], [esp+24], [esp+28]
            const U q[26] = {3, addr(static_cast<unsigned char*>(a) + 0x14), 1, addr(static_cast<unsigned char*>(a) + 0x54),
                             2, addr(&range), 2, addr(&range), 1, addr(&one), 1, addr(&zero24), 1, addr(&zero28)};
            add_action_front(self, 0x11, group, q);                                 // 005B66E5..4F
            const Vector zero = vec_of_bits(0, 0, 0);                               // [esp+40]
            void* o = as_object(t);                                                 // 005B676C..70
            w5_creature_add_move_to_point_action_to_front_004f8a50(self, nullptr, at<U16>(node, 0x6c),
                static_cast<const Vector*>(static_cast<void*>(static_cast<unsigned char*>(o) + 0x90)), area_id,
                at<U>(a, 0x14), at<U>(a, 0x54), flt(k_0_2f), flt(k_0_1f), 0, 0, 0, 0, 0, 0, &zero, 0, 0, 0); // 005B6773..B6
        } else {
            U zero2c = 0, range = k_0_2f, invalid = k_object_invalid;               // [esp+2C], [esp+30], [esp+34]
            const U q[26] = {2, addr(static_cast<unsigned char*>(a) + 0x38), 2, addr(static_cast<unsigned char*>(a) + 0x3c),
                             2, addr(static_cast<unsigned char*>(a) + 0x40), 3, addr(&area_id), 3, addr(&invalid),
                             1, addr(static_cast<unsigned char*>(a) + 0x54), 2, addr(&range), 1, addr(&zero2c)};
            add_action_front(self, 0x12, group, q);                                 // 005B67F9..74
            const Vector zero = vec_of_bits(0, 0, 0);                               // [esp+4C]
            w5_creature_add_move_to_point_action_to_front_004f8a50(self, nullptr, at<U16>(node, 0x6c),
                static_cast<const Vector*>(static_cast<void*>(static_cast<unsigned char*>(a) + 0x38)), area_id,
                k_object_invalid, at<U>(a, 0x54), flt(k_0_2f), flt(k_0_1f), 0, 0, 0, 0, 0, 0, &zero, 0, 0, 0); // 005B6879..D2
        }
        free_current_action(self);                                                  // 005B67BB..F1 / 005B68D7..07
        return k_requeue_00745dc8;
    }
    default:                                                                        // 2, 4, 5, 8 and out of table
        return k_in_progress_00745dbc;                                              // 005B692C
    }
}

// =========================================================================================================
// 004ECE60 CSWSCreature::ResolveInitiative: +4DC = (+A88 == 0)
void __fastcall kotor_fastcall_004ece60(void* self, void*) {
    at<U>(self, 0x4dc) = at<U>(self, 0xa88) == 0 ? 1u : 0u;                         // 004ECE60..6D
}


// ---------------------------------------------------------------------------------------------------------
// 00514270 CSWSCreature::AIActionCounterSpell(node): RET 4. Node +38 = the creature to counter.
U __fastcall w5_action_counter_spell_00514270(void* self, void*, void* node) {
    Vector target_position = vec_of_bits(0, 0, 0);                                  // [esp+20..28] (00514279..89)
    if (vcall<U>(self, 0x94) == 1) {                                                // 00514277..9A
        if (kotor_m15e_slot010_zero_0063e7f0(self, nullptr) == 1) return k_failed_00745dc4; // 0051429C..A6
    }
    if (recovered_creature_incapacitated_004ef890(self, nullptr) == 1) return k_failed_00745dc4; // 005142A8..B2
    w5_creature_activity_manager_004f87d0(self, nullptr, 4);                        // 005142C3..C7
    U target_id = at<U>(node, 0x38);                                                // 005142CC..D9 -> [esp+10]
    void* t = kotor_fastcall_004ae770(server_app(), nullptr, target_id);            // 005142D3..E1 (EBP)
    if (t == nullptr || vcall<U>(t, 0x94) != 0 || kotor_m15e_slot010_zero_0063e7f0(t, nullptr) != 0 ||
        recovered_creature_incapacitated_004ef890(t, nullptr) != 0)                 // 005142E6..1B
        return k_failed_00745dc4;                                                   // 0051447C
    void* t_area = recovered_object_area_004cb120(t, nullptr);                      // 00514321..23 (EBX)
    if (t_area == nullptr) return k_in_progress_00745dbc;                           // 0051432A..2C -> 00514459
    copy_vector(&target_position, static_cast<unsigned char*>(t) + 0x90);           // 00514332..4B
    U one_18 = 1, one_1c = 1;                                                       // [esp+18], [esp+1C] (0051435E..62)
    const float range = at<float>(rules(), 0x14);                                   // 00514342..6D -> [esp+14]
    Vector d;                                                                       // [esp+2C]
    kotor_cdecl_00423bc0(&d, &target_position, static_cast<const Vector*>(static_cast<void*>(static_cast<unsigned char*>(self) + 0x90))); // 00514366..77
    const U t_area_id = at<U>(t_area, 0x120);                                       // 0051437C (EBP)
    const bool same_area = t_area_id == area_id_of(recovered_object_area_004cb120(self, nullptr)); // 00514385..97
    if (same_area) {
        const double d2 = (static_cast<double>(d.z) * d.z + static_cast<double>(d.y) * d.y) + static_cast<double>(d.x) * d.x; // 0051439F..B9
        const double r2 = static_cast<double>(range) * range;                       // 005143BB..BF
        if (!(r2 < d2)) {                                                           // 005143C3..CA (test ah,5; jnp -> out of range)
            kotor_fastcall_004fbbe0(self, nullptr, target_id);                      // 005143D0..D7
            kotor_fastcall_004ece60(self, nullptr);                                 // 005143DC..DE
            const U8 mode = at<U8>(self, 0x4d2);                                    // 005143E3
            if (mode != 4) {                                                        // 005143E9..EB
                at<U8>(self, 0x4d2) = 4;                                            // 005143EF
                if (mode == 1) recovered_set_activity_004f00a0(self, nullptr, 0x400, 0, 0); // 0051441F
                else if (mode == 2) recovered_set_activity_004f00a0(self, nullptr, 0x100, 0, 0); // 00514416
                else if (mode == 3) recovered_set_activity_004f00a0(self, nullptr, 0x200, 0, 0); // 0051440D
                else recovered_set_activity_004f00a0(self, nullptr, 0x800, 1, 0);   // 00514404
            }
            if (at<U>(self, 0x4d4) != target_id) at<U>(self, 0x4d4) = target_id;    // 0051442B..37
            const U locked = vcall<U>(self, 0xc4);                                  // 0051443D..41
            if (locked != target_id) w5_creature_set_lock_orientation_to_object_004f34a0(self, nullptr, target_id, 0); // 00514447..54
            return k_in_progress_00745dbc;                                          // 00514459
        }
    }
    if ((at<U8>(self, 0x9f0) & 2) == 0) {                                           // 00514468..71
        recovered_creature_feedback_004ede10(self, nullptr, 7, nullptr);            // 00514473..77
        return k_failed_00745dc4;
    }
    const U group = at<U16>(node, 0x6c);
    {
        const U q[26] = {3, addr(&target_id)};
        add_action_front(self, 0x32, group, q);                                     // 0051448B..CB
    }
    U range_bits = bits(range);
    {
        const U q[26] = {3, addr(&target_id), 1, addr(&one_18), 2, addr(&range_bits), 2, addr(&range_bits),
                         1, addr(&one_1c)};
        add_action_front(self, 0x11, group, q);                                     // 005144D0..1E
    }
    const Vector zero = vec_of_bits(0, 0, 0);                                       // [esp+30] (0051452E..44)
    w5_creature_add_move_to_point_action_to_front_004f8a50(self, nullptr, group, &target_position, t_area_id,
        target_id, one_18, range, 0.0f, 0, 0, 0, 0, 0, 0, &zero, 0, 0, 0);          // 00514523..78
    kotor_fastcall_004eb5a0(self, nullptr, group);                                  // 0051457D..86
    return k_complete_00745dc0;                                                     // 0051458B
}
}  // extern "C"

// =========================================================================================================
// Mine (trap) actions: DisarmMine 00519570, RecoverMine 00518C40, FlagMine 0050E400, ExamineMine 0050E900.
// All four share the shape: walk into use range (action 19h/1Ah/1Bh/1Ch + ChangeFacingObject 13h + move), on the
// first in-range visit play the use animation (action 6) with a 4.5 s GUI timing bar (CSWSCreature +97C = 1),
// on the second visit roll the Demolitions skill (skill 1) against the mine/door/placeable DC and broadcast the
// result as CC message data (+97C = 0).
namespace menu_resources { struct Ref { Ref* __thiscall construct_00406d80(const char* text); }; } // CResRef(char*) [0030]

extern "C" {
void __fastcall recovered_creature_cancel_action_004ef4d0(void* creature, void*);                // StopGuiTimingBar [clear9]
unsigned __fastcall w1x_creature_skill_rank_005aa570(void* stats, void*, U skill, U a2, U a3);  // GetSkillRank (AL) [0886]
int __cdecl crt_rand_006fc447();                                                                 // _rand [0479]
void* __fastcall recovered_message_data_construct_004d69f0(void* data, void*);                   // CSWCCMessageData() [W1 b7]
void __fastcall recovered_message_data_destroy_004d6a40(void* data, void*);                      // ~CSWCCMessageData [W1 b7]
void __fastcall recovered_event_integer_004d76b0(void* data, void*, U index, U value);           // SetInteger [1017]
U __fastcall recovered_party_member_00563740(void* table, void*, U id);                          // CSWPartyTable::GetIsMember [1124]
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);                       // GetServerAIMaster
void __fastcall recovered_object_set_fade_out_time_004ce8a0(void* object, void*, U value);       // SetFadeOutTime [W1 b7]
I __fastcall exo_array_list_count_004af240(void* list, void*, U value);                          // CExoArrayList::Count (contains) [0567]
U __fastcall recovered_int_list_remove_0066dcc0(void* list, void*, U value);                     // CExoArrayList<int>::remove [W1 b7]
void* __fastcall recovered_creature_client_creature_004efb20(void* creature, void*);             // GetClientCreature [W1 b7]
void __fastcall recovered_client_creature_play_sound_set_sound_00611470(void* client, void*, U sound, U flag); // PlaySoundSetSound
I __fastcall c2da_current_get_string_00413270(void* table, void*, I row, void* column, void* out); // C2DA::GetCExoStringEntry [0379]
char* __fastcall WindowCStr005e5670(void* string, void*);                                        // CExoString::CStr [0078]
U __fastcall recovered_item_template_005608b0(void* item, void*, void* resref);                  // CSWSItem::LoadFromTemplate [0995]
U __fastcall recovered_cc_message_00568570(void* message, void*, U player, U type, void* data, U a4); // SendServerToPlayerCCMessage
U __fastcall recovered_action_cancel_message_0056b380(void* message, void*, void* player, U flag, U type, U time); // ...GuiTimingEvent
void __fastcall kotor_fastcall_004d68a0(void* message, void*, U a, U b, U c);                   // CSWMessage::CreateWriteMessage [w2_message]
void __fastcall kotor_fastcall_00578570(void* message, void*, U id);                             // WriteOBJECTIDServer [w2_message]
void __fastcall kotor_fastcall_004d68e0(void* message, void*, U value);                          // WriteBOOL [w2_message]
void __fastcall kotor_fastcall_004d62c0(void* message, void*, U value, U bits);                  // WriteBYTE [w2_message]
U __fastcall kotor_fastcall_004d6440(void* message, void*, void** data, U* size);                // GetWriteMessage [w2_message]
U __fastcall kotor_fastcall_00567e60(void* message, void*, U player, U major, U minor, void* data, U size); // SendServerToPlayerMessage



// 005D5170 CNetLayer::GetPlayerInfo(id): RET 4 (the info array at +20014h; the id is not used)
void* __fastcall kotor_fastcall_005d5170(void* net, void*, U) {
    return static_cast<unsigned char*>(net) + 0x20014;                              // 005D5170
}

// 0056F160 CSWSMessage::SendServerToPlayerExamineGui_MineData(player, id, creature, flag): RET 10h
U __fastcall kotor_fastcall_0056f160(void* message, void*, void* player, U id, void* creature, U flag) {
    U player_id = at<U>(player, 4);                                                 // 0056F174..77 (over the player slot)
    void* obj = kotor_fastcall_004ae750(server_app(), nullptr, id);                 // 0056F17B..7F
    if (obj == nullptr) return 0;                                                   // 0056F186..88
    const I rank = static_cast<signed char>(static_cast<U8>(
        w1x_creature_skill_rank_005aa570(at<void*>(creature, 0xa74), nullptr, 1, 0, 0))); // 0056F18E..B1
    const I skill = rank + 0x14;                                                    // 0056F1B7 (EBX)
    (void)kotor_fastcall_005d5170(vcall<void*>(server_app(), 0x10), nullptr, player_id); // 0056F1AD..BF (GetNetLayer; unused)
    kotor_fastcall_004d68a0(message, nullptr, 0x0a, 0xffffffffu, 1);                // 0056F1C4..CC
    kotor_fastcall_00578570(message, nullptr, id);                                  // 0056F1D1..D4
    kotor_fastcall_004d68e0(message, nullptr, flag);                                // 0056F1D9..E0
    const U8 type = at<U8>(obj, 8);                                                 // 0056F1E5
    I dc = 0; U known = 0; bool difficulty = true;
    if (type == 7) {                                                                // 0056F1E8 [746636] trigger
        dc = at<I>(vcall<void*>(obj, 0x40), 0x2dc);                                 // 0056F1F0..F7
        kotor_fastcall_004d62c0(message, nullptr, at<U8>(vcall<void*>(obj, 0x40), 0x2d4), 8); // 0056F1FD..11
        known = at<U>(vcall<void*>(obj, 0x40), 0x2c4);                              // 0056F216..1D
    } else if (type == 10) {                                                        // 0056F248 [746639] door
        dc = at<U8>(vcall<void*>(obj, 0x18), 0x2ec);                                // 0056F250..59
        kotor_fastcall_004d62c0(message, nullptr, at<U8>(vcall<void*>(obj, 0x18), 0x2f8), 8); // 0056F25A..71
        known = at<U>(vcall<void*>(obj, 0x18), 0x2fc);                              // 0056F276..7D
    } else if (type == 9) {                                                         // 0056F2A1 [746638] placeable
        dc = at<U8>(vcall<void*>(obj, 0x48), 0x27c);                                // 0056F2A9..B2
        kotor_fastcall_004d62c0(message, nullptr, at<U8>(vcall<void*>(obj, 0x48), 0x290), 8); // 0056F2B3..CA
        known = at<U>(vcall<void*>(obj, 0x48), 0x280);                              // 0056F2CF..D6
    } else {
        difficulty = false;                                                         // 0056F2A7 -> 0056F323
    }
    if (difficulty) {
        U level;
        if (known == 0 || dc > 0x23) level = 4;                                     // 0056F223..2E -> 0056F318
        else if (dc <= skill - 0xf) level = 0;                                      // 0056F234..41
        else if (dc <= skill - 0xa) level = 1;                                      // 0056F2F2..FB
        else if (dc <= skill - 5) level = 2;                                        // 0056F2FF..06
        else if (dc <= skill) level = 3;                                            // 0056F30A..10
        else level = 4;                                                             // 0056F314
        kotor_fastcall_004d62c0(message, nullptr, level, 8);                        // 0056F31E
    }
    void* data = nullptr; U size = id;                                              // [esp+10], the id slot
    if (kotor_fastcall_004d6440(message, nullptr, &data, &size) == 0) return 0;     // 0056F323..37
    return kotor_fastcall_00567e60(message, nullptr, player_id, 0x1b, 5, data, size); // 0056F342..57
}
}  // extern "C"

namespace {
constexpr U k_float_1_0 = 0x3f800000u;        // 1.0 immediate
constexpr U k_float_4_5 = 0x40900000u;        // 4.5 immediate
constexpr U k_float_0_25 = 0x3e800000u;       // 0.25 immediate
constexpr U k_event_script_007441dc = 10;     // .rdata 007441DC
constexpr U k_event_destroy_007441e0 = 11;    // .rdata 007441E0
const char k_resref_00747508[] = "ResRef";    // .rdata 00747508

I skill_rank(void* self) {                    // GetSkillRank(1, 0, 0) -> movsx al
    return static_cast<signed char>(static_cast<U8>(w1x_creature_skill_rank_005aa570(at<void*>(self, 0xa74), nullptr, 1, 0, 0)));
}
void* new_script_event(U16 type) {            // operator new(34h) + CScriptEvent; the type word is written unchecked
    void* memory = live_operator_new(0x34);
    void* event = memory != nullptr ? recovered_event_construct_004d7540(memory, nullptr) : nullptr;
    at<U16>(event, 0) = type;
    return event;
}
void enqueue(U caller, U target, U type, void* event) {   // AddEventDeltaTime(0, 0, caller, target, type, event)
    recovered_event_enqueue_004b08d0(recovered_server_ai_master_004aed80(server_app(), nullptr), nullptr, 0, 0,
                                     caller, target, type, event);
}
void* new_message_data() {
    void* memory = live_operator_new(0x34);
    return memory != nullptr ? recovered_message_data_construct_004d69f0(memory, nullptr) : nullptr;
}
void* party_table() { return static_cast<unsigned char*>(at<void*>(server_app(), 4)) + 0x1b770; } // inline view of 004AEE70
// the shared "walk into range" leg: <action>(target), ChangeFacingObject 13h(target), MoveToPoint in front
void approach(void* self, U* id, U group, U action, bool widen) {
    Vector position = vec_of_bits(0, 0, 0);                                         // zeroed in the prologue
    float range;                                                                    // written by GetUseRange
    w5_creature_get_use_range_004ee440(self, nullptr, *id, &position, &range, 0);
    if (widen) range = static_cast<float>(static_cast<double>(range) + k_quarter_0073edf0); // fadd [0073EDF0]
    const U q[26] = {3, addr(id)};
    add_action_front(self, action, group, q);
    add_action_front(self, 0x13, group, q);
    const Vector zero = vec_of_bits(0, 0, 0);
    w5_creature_add_move_to_point_action_to_front_004f8a50(self, nullptr, group, &position, at<U>(self, 0x8c),
        k_object_invalid, 1, range, 0.0f, 0, 0, 0, 0, 0, 0, &zero, 0, 0, 0);
}
// the shared first in-range visit: <action>(target), PlayAnimation 6 (anim, 1.0, 4.5, 1), ChangeFacing 13h, timing bar
void start_use(void* self, U* id, U group, U action, U animation, U bar_type) {
    at<U>(self, 0x97c) = 1;
    U anim = animation, one = 1, speed = k_float_1_0, duration = k_float_4_5;
    const U q[26] = {3, addr(id)};
    add_action_front(self, action, group, q);
    const U play[26] = {1, addr(&anim), 2, addr(&speed), 2, addr(&duration), 1, addr(&one)};
    add_action_front(self, 6, group, play);
    add_action_front(self, 0x13, group, q);
    kotor_fastcall_004ef480(self, nullptr, 0x1194, bar_type);
}
void finish_use(void* self, void* data) {
    kotor_fastcall_004ec700(self, nullptr, data);
    recovered_creature_cancel_action_004ef4d0(self, nullptr);
    at<U>(self, 0x97c) = 0;
}
// the area's trap list (+12C): drop the id when present (00519BC1..C1B / 005192FB..344)
void forget_trap(void* self, U id) {
    if (recovered_object_area_004cb120(self, nullptr) == nullptr) return;
    // the original also tests &area->+12C against null (never null for a non-null area)
    if (exo_array_list_count_004af240(static_cast<unsigned char*>(recovered_object_area_004cb120(self, nullptr)) + 0x12c,
                                      nullptr, id) != 1) return;
    recovered_int_list_remove_0066dcc0(static_cast<unsigned char*>(recovered_object_area_004cb120(self, nullptr)) + 0x12c,
                                       nullptr, id);
}
// critical failure: a trigger fires (event 0Ch with the creature as object 0), a door/placeable trap fires (1Ah)
void spring_trap(void* self, void* obj, U id) {
    if (vcall<void*>(obj, 0x40) != nullptr) {
        void* event = new_script_event(0x0c);
        recovered_event_object_004d7780(event, nullptr, 0, at<U>(self, 4));
        enqueue(at<U>(self, 4), id, k_event_script_007441dc, event);
    } else if (vcall<void*>(obj, 0x48) != nullptr || vcall<void*>(obj, 0x18) != nullptr) {
        enqueue(at<U>(self, 4), id, k_event_script_007441dc, new_script_event(0x1a));
    }
}
// own-mine test shared by Disarm/Recover (0051988E..926 / 00518F82..016): the creator lookup happens first;
// a mine set by the user is own; a mine set by a party member (or a creator with stats +6C) is own when the user
// is a party member (or has stats +6C)
bool own_mine(void* self, void* obj) {
    void* creator = kotor_fastcall_004ae770(server_app(), nullptr, at<U>(vcall<void*>(obj, 0x40), 0x2e4));
    if (at<U>(vcall<void*>(obj, 0x40), 0x2e4) == at<U>(self, 4)) return true;
    const bool friendly_creator =
        recovered_party_member_00563740(party_table(), nullptr, at<U>(vcall<void*>(obj, 0x40), 0x2e4)) != 0 ||
        (creator != nullptr && at<U>(at<void*>(creator, 0xa74), 0x6c) != 0);
    if (!friendly_creator) return false;
    return recovered_party_member_00563740(party_table(), nullptr, at<U>(self, 4)) != 0 ||
           at<U>(at<void*>(self, 0xa74), 0x6c) != 0;
}
}  // namespace

extern "C" {
// ---------------------------------------------------------------------------------------------------------
// 00519570 CSWSCreature::AIActionDisarmMine(node): RET 4. Node +38 = trigger / door / placeable.
U __fastcall w5_action_disarm_mine_00519570(void* self, void*, void* node) {
    w5_creature_activity_manager_004f87d0(self, nullptr, 2);                        // 0051958C..A8
    U id = at<U>(node, 0x38);                                                       // 005195B1..BA -> [esp+10]
    void* obj = kotor_fastcall_004ae750(server_app(), nullptr, id);                 // 005195B4..C2 (EDI)
    if (obj == nullptr) {                                                           // 005195CB..CF
        recovered_creature_cancel_action_004ef4d0(self, nullptr);                   // 005195D1
        at<U>(self, 0x97c) = 0;
        return k_failed_00745dc4;
    }
    const U group = at<U16>(node, 0x6c);
    if (w5_creature_get_is_in_use_range_004f6000(self, nullptr, id, flt(k_float_0_25), 0) == 0) { // 005195E6..F8
        approach(self, &id, group, 0x19, true);                                     // 005195FE..C9 (range + 0.25)
        return k_complete_00745dc0;
    }
    if (at<U>(self, 0x97c) == 0) {                                                  // 005196D3..D9
        const U8 type = at<U8>(obj, 8);
        const U animation = type == 10 ? 0x2796u : (type == 9 ? 0x2797u : 0x2794u); // 005196EA..16
        start_use(self, &id, group, 0x19, animation, 3);                            // 005196DF..822
        return k_complete_00745dc0;
    }
    const I rank = skill_rank(self);                                                // 0051982C..3F -> [esp+54]
    U take20 = 0, success = 0, own = 0;                                             // [esp+20], [esp+18], [esp+24]
    I roll;                                                                         // [esp+14]
    if (at<U>(self, 0x4e0) != 0) roll = crt_rand_006fc447() % 0x14 + 1;            // 00519842..68
    else { roll = 0x14; take20 = 1; }                                               // 0051986E..76
    I dc = 1;                                                                       // EBP (00519834)
    bool failed = false;
    const U8 type = at<U8>(obj, 8);                                                 // 0051987A
    if (type == 7) {                                                                // [746636] trigger
        if (own_mine(self, obj)) {                                                  // -> 00519A09
            own = 1; success = 1;                                                   // 00519A09..14
            enqueue(at<U>(self, 4), id, k_event_script_007441dc, new_script_event(0x18)); // 00519A0E..68
            enqueue(at<U>(self, 4), id, k_event_destroy_007441e0, nullptr);         // 00519A6D..93
            recovered_object_set_fade_out_time_004ce8a0(vcall<void*>(obj, 0x40), nullptr, 2); // 00519A98..A3
            dc = rank;                                                              // 00519AA8 (quirk: DC reported = rank)
        } else {
            if (at<I>(vcall<void*>(obj, 0x40), 0x2dc) >= 1) dc = at<I>(vcall<void*>(obj, 0x40), 0x2dc); // 0051992C..49
            if (dc > 0x23 || roll + rank < dc) {                                    // 0051994F..64
                failed = true;
            } else {
                success = 1;                                                        // 0051996C
                enqueue(at<U>(self, 4), id, k_event_script_007441dc, new_script_event(0x18)); // 0051996A..C4
                enqueue(at<U>(self, 4), id, k_event_destroy_007441e0, nullptr);     // 005199C9..EF
                recovered_object_set_fade_out_time_004ce8a0(vcall<void*>(obj, 0x40), nullptr, 2); // 005199F4..FF
            }
        }
    } else if (type == 10 || type == 9) {                                           // [746639] door / [746638] placeable
        const U8 trap_dc = type == 10 ? at<U8>(vcall<void*>(obj, 0x18), 0x2ec) : at<U8>(vcall<void*>(obj, 0x48), 0x27c);
        if (trap_dc >= 1) {                                                         // 00519AC0..C7 / 00519B30..37 (jb)
            dc = type == 10 ? at<U8>(vcall<void*>(obj, 0x18), 0x2ec) : at<U8>(vcall<void*>(obj, 0x48), 0x27c);
            if (dc > 0x23) failed = true;                                           // 00519AD7..DA / 00519B47..4A
        }
        if (!failed && roll + rank < dc) failed = true;                             // 00519AE0..EC / 00519B50..5C
        if (!failed) {
            success = 1;                                                            // 00519AF4 / 00519B64
            enqueue(at<U>(self, 4), id, k_event_script_007441dc, new_script_event(0x18)); // 00519AF2..BBC
        }
    } else {
        dc = rank;                                                                  // 00519C20
        failed = true;
    }
    if (!failed) {
        forget_trap(self, id);                                                      // 00519BC1..C1B
    } else {
        recovered_client_creature_play_sound_set_sound_00611470(recovered_creature_client_creature_004efb20(self, nullptr),
                                                               nullptr, 0x18, 1);   // 00519C24..31
        if (roll + rank < dc - 0xa) spring_trap(self, obj, id);                     // 00519C36..D0E
    }
    void* data = new_message_data();                                                // 00519D13..3A
    recovered_event_object_004d7780(data, nullptr, 0, at<U>(self, 4));              // 00519D3A..46
    recovered_event_object_004d7780(data, nullptr, 1, id);                          // 00519D4B..54
    recovered_event_integer_004d76b0(data, nullptr, 0, 0x5f9);                      // 00519D59..62
    recovered_event_integer_004d76b0(data, nullptr, 1, static_cast<U>(roll));
    recovered_event_integer_004d76b0(data, nullptr, 2, static_cast<U>(rank));
    recovered_event_integer_004d76b0(data, nullptr, 3, static_cast<U>(dc));
    recovered_event_integer_004d76b0(data, nullptr, 4, take20);                     // 00519D8D..96
    const I total = roll + rank;
    if (own == 1) recovered_event_integer_004d76b0(data, nullptr, 5, 4);            // 00519D9B..A8
    else if (success == 1) recovered_event_integer_004d76b0(data, nullptr, 5, 1);   // 00519DAC..B2
    else if (take20 != 0 && total < dc) recovered_event_integer_004d76b0(data, nullptr, 5, 3); // 00519DB5..C8
    else if (total < dc - 0xa) recovered_event_integer_004d76b0(data, nullptr, 5, 2); // 00519DCC..DA
    else if (total < dc) recovered_event_integer_004d76b0(data, nullptr, 5, 0);     // 00519DDE..E2
    recovered_event_integer_004d76b0(data, nullptr, 6, 0x144);                      // 00519DED..F6
    finish_use(self, data);                                                         // 00519DFB..0A
    return k_complete_00745dc0;
}

// ---------------------------------------------------------------------------------------------------------
// 00518C40 CSWSCreature::AIActionRecoverMine(node): RET 4. A recovered mine becomes an item from the traps 2DA
// (rules +B8 -> +8C; row = trap type, column "ResRef") added to the creature's inventory.
U __fastcall w5_action_recover_mine_00518c40(void* self, void*, void* node) {
    I dc = 0;                                                                       // [esp+14] (00518C62)
    w5_creature_activity_manager_004f87d0(self, nullptr, 2);                        // 00518C5E..72
    U id = at<U>(node, 0x38);                                                       // 00518C7B..84
    void* obj = kotor_fastcall_004ae750(server_app(), nullptr, id);                 // 00518C8B..8C (ESI)
    if (obj == nullptr) {
        recovered_creature_cancel_action_004ef4d0(self, nullptr);                   // 00518C99
        at<U>(self, 0x97c) = 0;                                                     // 00518C9E (ESI = 0)
        return k_failed_00745dc4;
    }
    const U group = at<U16>(node, 0x6c);
    if (w5_creature_get_is_in_use_range_004f6000(self, nullptr, id, flt(k_float_0_25), 0) == 0) { // 00518CAE..C1
        approach(self, &id, group, 0x1a, true);                                     // 00518CC7..DCC
        return k_complete_00745dc0;
    }
    if (at<U>(self, 0x97c) == 0) {                                                  // 00518DD6..DE
        const U8 type = at<U8>(obj, 8);
        start_use(self, &id, group, 0x1a, (type == 10 || type == 9) ? 0x274cu : 0x279du, 2); // 00518DE4..F17
        return k_complete_00745dc0;
    }
    const I rank = skill_rank(self);                                                // 00518F21..37 -> [esp+18]
    U recovered = 0, take20 = 0, own = 0;                                           // EBP, [esp+24], [esp+28]
    I roll;                                                                         // [esp+6C] (node slot)
    if (at<U>(self, 0x4e0) != 0) roll = crt_rand_006fc447() % 0x14 + 1;            // 00518F3B..59
    else { roll = 0x14; take20 = 1; }                                               // 00518F5F..6B
    U8 trap_type = 0;                                                               // BL
    const U8 type = at<U8>(obj, 8);                                                 // 00518F73
    auto dc_of = [](I value) { return value + 0xa >= 1 ? value + 0xa : 1; };        // max(1, value + 10)
    if (type == 7) {                                                                // trigger
        if (own_mine(self, obj)) {                                                  // -> 00519066
            recovered = 1; own = 1;                                                 // 00519066..6B
        } else {
            dc = dc_of(at<I>(vcall<void*>(obj, 0x40), 0x2dc));                      // 00519018..47
            if (roll + rank >= dc) recovered = 1;                                   // 0051904B..5F
        }
        if (recovered == 1) {
            trap_type = at<U8>(vcall<void*>(obj, 0x40), 0x2d4);                     // 0051906F..7C
            enqueue(at<U>(self, 4), id, k_event_destroy_007441e0, nullptr);         // 00519076..A2
            recovered_object_set_fade_out_time_004ce8a0(vcall<void*>(obj, 0x40), nullptr, 2); // 005190A7..B2
        }
    } else if (type == 10 || type == 9) {                                           // door / placeable
        dc = dc_of(type == 10 ? at<U8>(vcall<void*>(obj, 0x18), 0x2ec) : at<U8>(vcall<void*>(obj, 0x48), 0x27c)); // 005190C8..FD / 00519159..8E
        if (roll + rank >= dc) {                                                    // 005190FD..0B / 0051918E..9C
            recovered = 1;
            trap_type = type == 10 ? at<U8>(vcall<void*>(obj, 0x18), 0x2f8) : at<U8>(vcall<void*>(obj, 0x48), 0x290); // 0051911D / 005191AE
            enqueue(at<U>(self, 4), id, k_event_script_007441dc, new_script_event(0x18)); // 00519123..206
        }
    }
    if (recovered == 1) {                                                           // 0051920B: create the item
        alignas(4) unsigned char resref_text[8];                                    // CExoString [esp+38]
        kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(resref_text), nullptr); // 0051920B..0F
        void* memory = live_operator_new(0x29c);                                    // 00519214..21
        void* item = memory != nullptr ? recovered_item_construct_005530a0(memory, nullptr, k_object_invalid) : nullptr; // 0051922D..46 -> [esp+20]
        alignas(4) unsigned char column[8];                                         // CExoString [esp+2C]
        frame_LogStringCtor_005e5a90(column, nullptr, k_resref_00747508);           // 00519246..58
        const I found = c2da_current_get_string_00413270(at<void*>(at<void*>(rules(), 0xb8), 0x8c), nullptr,
                                                          trap_type, column, resref_text); // 0051925D..84
        frame_LogStringDtor_005e5c20(column, nullptr);                              // 00519289..94
        if (found != 0) {                                                           // 00519299..9B
            alignas(4) unsigned char resref[16];                                    // CResRef [esp+4C]
            reinterpret_cast<menu_resources::Ref*>(resref)->construct_00406d80(WindowCStr005e5670(resref_text, nullptr)); // 005192A1..AF
            recovered_item_template_005608b0(item, nullptr, resref);                // 005192B4..BD
            // quirk: when AddItem refuses the item it is neither owned nor freed
            if (recovered_inventory_insert_0055d330(recovered_creature_inventory_004ef770(self, nullptr, 1), nullptr,
                                                    &item, 1, 0, 1) != 0 && item != nullptr) // 005192C2..E7
                recovered_item_owner_00553210(item, nullptr, at<U>(self, 4), 1, 1, k_object_invalid); // 005192E9..F6
            forget_trap(self, id);                                                  // 005192FB..44
        }
        frame_LogStringDtor_005e5c20(resref_text, nullptr);                         // 00519349..55
    } else if (roll + rank < dc - 5) {                                              // 00519363..76 (DC 0 for other types)
        spring_trap(self, obj, id);                                                 // 0051937C..447
    }
    void* data = new_message_data();                                                // 0051944C..73
    recovered_event_object_004d7780(data, nullptr, 0, at<U>(self, 4));              // 00519473..83
    recovered_event_object_004d7780(data, nullptr, 1, id);                          // 00519488..91
    recovered_event_integer_004d76b0(data, nullptr, 0, 0x5fb);                      // 00519496..9F
    recovered_event_integer_004d76b0(data, nullptr, 1, static_cast<U>(roll));
    recovered_event_integer_004d76b0(data, nullptr, 2, static_cast<U>(rank));
    recovered_event_integer_004d76b0(data, nullptr, 3, static_cast<U>(dc));         // 005194C0 (EBX = [esp+14])
    recovered_event_integer_004d76b0(data, nullptr, 4, take20);
    const I total = roll + rank;
    if (own == 1) recovered_event_integer_004d76b0(data, nullptr, 5, 4);            // 005194D8..DF
    else if (recovered == 1) recovered_event_integer_004d76b0(data, nullptr, 5, 1); // 005194E3..E8
    else if (take20 != 0 && total < dc) recovered_event_integer_004d76b0(data, nullptr, 5, 3); // 005194EB..01
    else if (total < dc - 0xa) recovered_event_integer_004d76b0(data, nullptr, 5, 2); // 00519505..16
    else if (total < dc) recovered_event_integer_004d76b0(data, nullptr, 5, 0);     // 0051951A..1E
    recovered_event_integer_004d76b0(data, nullptr, 6, 0x144);                      // 00519529..32
    finish_use(self, data);                                                         // 00519537..46
    return k_complete_00745dc0;
}

// ---------------------------------------------------------------------------------------------------------
// 0050E400 CSWSCreature::AIActionFlagMine(node): RET 4. Marks the mine (trigger +2C8 / door +2F4 / placeable
// +28C) as flagged when the check succeeds.
U __fastcall w5_action_flag_mine_0050e400(void* self, void*, void* node) {
    w5_creature_activity_manager_004f87d0(self, nullptr, 2);                        // 0050E41C..38
    U id = at<U>(node, 0x38);                                                       // 0050E441..4A
    void* obj = kotor_fastcall_004ae750(server_app(), nullptr, id);                 // 0050E451..57 (ESI)
    if (obj == nullptr) {
        recovered_creature_cancel_action_004ef4d0(self, nullptr);                   // 0050E461
        at<U>(self, 0x97c) = 0;
        return k_failed_00745dc4;
    }
    const U group = at<U16>(node, 0x6c);
    if (w5_creature_get_is_in_use_range_004f6000(self, nullptr, id, 0.0f, 0) == 0) { // 0050E476..84
        approach(self, &id, group, 0x1b, false);                                    // 0050E48A..547 (no extra range)
        return k_complete_00745dc0;
    }
    if (at<U>(self, 0x97c) == 0) {                                                  // 0050E551..57
        const U8 type = at<U8>(obj, 8);
        start_use(self, &id, group, 0x1b, (type == 10 || type == 9) ? 0x274cu : 0x274bu, 1); // 0050E55D..68F
        return k_complete_00745dc0;
    }
    const I rank = skill_rank(self);                                                // 0050E699..AF -> [esp+4C]
    U own = 0, take20 = 0;                                                          // [esp+18], [esp+14]
    I roll;                                                                         // EBP
    if (at<U>(self, 0x4e0) != 0) roll = crt_rand_006fc447() % 0x14 + 1;            // 0050E6B3..D2
    else { roll = 0x14; take20 = 1; }                                               // 0050E6D5..DA
    I dc = 1;                                                                       // EBX (0050E6A1)
    const U8 type = at<U8>(obj, 8);                                                 // 0050E6DE
    auto dc_of = [](I value) { return value - 5 >= 1 ? value - 5 : 1; };            // max(1, value - 5)
    if (type == 7) {                                                                // trigger
        if (at<U>(vcall<void*>(obj, 0x40), 0x2e4) == at<U>(self, 4)) {              // 0050E6ED..701 (own mine)
            at<U>(vcall<void*>(obj, 0x40), 0x2c8) = 1;                              // 0050E707..0C
            own = 1;                                                                // 0050E712
            dc = rank;                                                              // 0050E716 (quirk: DC reported = rank)
        } else {
            dc = dc_of(at<I>(vcall<void*>(obj, 0x40), 0x2dc));                      // 0050E79D..BC
            if (rank + roll >= dc) at<U>(vcall<void*>(obj, 0x40), 0x2c8) = 1;       // 0050E7BF..D5
        }
    } else if (type == 10) {                                                        // door
        dc = dc_of(at<U8>(vcall<void*>(obj, 0x18), 0x2ec));                         // 0050E7EC..0F
        if (rank + roll >= dc) at<U>(vcall<void*>(obj, 0x18), 0x2f4) = 1;           // 0050E812..28
    } else if (type == 9) {                                                         // placeable
        dc = dc_of(at<U8>(vcall<void*>(obj, 0x48), 0x27c));                         // 0050E843..66
        if (rank + roll >= dc) at<U>(vcall<void*>(obj, 0x48), 0x28c) = 1;           // 0050E869..7F
    } else {
        dc = rank;                                                                  // 0050E83D -> 0050E716
    }
    void* data = new_message_data();                                                // 0050E71A..39 (ESI)
    recovered_event_object_004d7780(data, nullptr, 0, at<U>(self, 4));              // 0050E73B..4B
    recovered_event_integer_004d76b0(data, nullptr, 0, 0);                          // 0050E750..56
    recovered_event_integer_004d76b0(data, nullptr, 1, static_cast<U>(roll));
    recovered_event_integer_004d76b0(data, nullptr, 2, static_cast<U>(rank));
    recovered_event_integer_004d76b0(data, nullptr, 3, static_cast<U>(dc));
    recovered_event_integer_004d76b0(data, nullptr, 4, take20);                     // 0050E77D..86
    if (own == 1) recovered_event_integer_004d76b0(data, nullptr, 5, 4);            // 0050E78B..96
    else if (roll + rank >= dc) recovered_event_integer_004d76b0(data, nullptr, 5, 1); // 0050E88E..98
    else if (take20 != 0) recovered_event_integer_004d76b0(data, nullptr, 5, 3);    // 0050E89C..A4
    else recovered_event_integer_004d76b0(data, nullptr, 5, 0);                     // 0050E8A8
    recovered_event_integer_004d76b0(data, nullptr, 6, 0x144);                      // 0050E8B3..BC
    finish_use(self, data);                                                         // 0050E8C1..D0
    return k_complete_00745dc0;
}

// ---------------------------------------------------------------------------------------------------------
// 0050E900 CSWSCreature::AIActionExamineMine(node): RET 4. Rolls Demolitions and sends the examine GUI data.
U __fastcall w5_action_examine_mine_0050e900(void* self, void*, void* node) {
    w5_creature_activity_manager_004f87d0(self, nullptr, 2);                        // 0050E91B..37
    U id = at<U>(node, 0x38);                                                       // 0050E940..49
    void* obj = kotor_fastcall_004ae750(server_app(), nullptr, id);                 // 0050E950..56 (ESI)
    if (obj == nullptr) {
        recovered_creature_cancel_action_004ef4d0(self, nullptr);                   // 0050E95E
        at<U>(self, 0x97c) = 0;                                                     // 0050E963 (ESI = 0)
        return k_failed_00745dc4;
    }
    const U group = at<U16>(node, 0x6c);
    if (w5_creature_get_is_in_use_range_004f6000(self, nullptr, id, 0.0f, 0) == 0) { // 0050E982..92
        approach(self, &id, group, 0x1c, false);                                    // 0050E998..A8F
        return k_complete_00745dc0;
    }
    if (at<U>(self, 0x97c) == 0) {                                                  // 0050EA99..A2
        const U8 type = at<U8>(obj, 8);
        start_use(self, &id, group, 0x1c, (type == 10 || type == 9) ? 0x274cu : 0x274bu, 4); // 0050EAA8..BD8
        return k_complete_00745dc0;
    }
    const I rank = skill_rank(self);                                                // 0050EBE2..FD (EBP)
    U take20 = 0, success = 0, own = 0;                                             // [esp+18], [esp+14], [esp+1C]
    I roll;                                                                         // [esp+50] (node slot)
    if (at<U>(self, 0x4e0) != 0) roll = crt_rand_006fc447() % 0x14 + 1;            // 0050EC02..20
    else { roll = 0x14; take20 = 1; }                                               // 0050EC26..2E
    I dc = 1;                                                                       // EBX (0050EBEC)
    auto dc_of = [](I value) { return value - 7 >= 1 ? value - 7 : 1; };            // max(1, value - 7)
    const U8 type = at<U8>(obj, 8);                                                 // 0050EC32
    bool test = true;
    if (type == 7) {                                                                // trigger
        if (at<U>(vcall<void*>(obj, 0x40), 0x2e4) == at<U>(self, 4)) {              // 0050EC41..51
            success = 1; own = 1;                                                   // 0050EC57..5B
            dc = roll;                                                              // 0050EC5F (quirk: DC reported = the roll)
            test = false;
        } else {
            dc = dc_of(at<I>(vcall<void*>(obj, 0x40), 0x2dc));                      // 0050ECE9..0D
        }
    } else if (type == 10) {
        dc = dc_of(at<U8>(vcall<void*>(obj, 0x18), 0x2ec));                         // 0050ED17..3A
    } else if (type == 9) {
        dc = dc_of(at<U8>(vcall<void*>(obj, 0x48), 0x27c));                         // 0050ED48..6B
    } else {
        dc = roll; test = false;                                                    // 0050ED42 -> 0050EC5F
    }
    if (test && roll + rank >= dc) success = 1;                                     // 0050ED6E..7C
    void* data = new_message_data();                                                // 0050EC63..82 (ESI)
    recovered_event_object_004d7780(data, nullptr, 0, at<U>(self, 4));              // 0050EC84..94
    recovered_event_integer_004d76b0(data, nullptr, 0, 0x5fc);                      // 0050EC99..A2
    recovered_event_integer_004d76b0(data, nullptr, 1, static_cast<U>(roll));
    recovered_event_integer_004d76b0(data, nullptr, 2, static_cast<U>(rank));
    recovered_event_integer_004d76b0(data, nullptr, 3, static_cast<U>(dc));
    recovered_event_integer_004d76b0(data, nullptr, 4, take20);                     // 0050ECC9..D2
    if (own == 1) recovered_event_integer_004d76b0(data, nullptr, 5, 4);            // 0050ECD7..E2
    else if (roll + rank >= dc) recovered_event_integer_004d76b0(data, nullptr, 5, 1); // 0050ED89..93
    else if (take20 != 0) recovered_event_integer_004d76b0(data, nullptr, 5, 3);    // 0050ED97..9F
    else recovered_event_integer_004d76b0(data, nullptr, 5, 0);                     // 0050EDA3
    recovered_event_integer_004d76b0(data, nullptr, 6, 0x144);                      // 0050EDAE..B7
    kotor_fastcall_004ec700(self, nullptr, data);                                   // 0050EDBC..BF
    void* player = kotor_fastcall_004ae910(server_app(), nullptr, at<U>(self, 4));  // 0050EDC4..D1
    if (player != nullptr)                                                          // 0050EDD6..D8
        kotor_fastcall_0056f160(combat_reuse_message_004aedb0(), nullptr, player, id, self, success); // 0050EDDA..F6
    recovered_creature_cancel_action_004ef4d0(self, nullptr);                       // 0050EDFB..FD
    at<U>(self, 0x97c) = 0;                                                         // 0050EE02
    return k_complete_00745dc0;
}
}  // extern "C"
