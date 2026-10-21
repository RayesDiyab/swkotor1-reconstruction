// W5 queued-action bodies, lane worker combat-spell:
//   00514AF0 CSWSCreature::AIActionCastSpell      (export w5_action_cast_spell_00514af0)
//   0050F170 CSWSCreature::AIActionItemCastSpell  (export w5_action_item_cast_spell_0050f170)
// plus every callee of theirs that has no provider in the c07b candidate map (reconstructed below as
// kotor_fastcall_<addr>). AIActionSetMine 00519E30 and its trigger cascade live in claude_combat_spell_mine.cpp.
// Transcribed from the original swkotor.exe (read-only); addresses in the comments are original instructions.
// Conventions (W5 t1..t3): thiscall -> extern "C" __fastcall(self, unused_edx, ...); x87 at 53-bit precision ->
// double intermediates, float exactly where the original stores a dword; live_operator_new / live_free only.
// Callees owned by the concurrent combat-attack worker (StartCombatRound 004D5F70, ResolveEngagedState 004D2C30,
// ResolveMasterState 004D2D60, ResolveAttack 005BBA80) are only declared, with the original ABI.
#include "842d6b1618-w5_t3.hpp"
#include "5889fde5f1-w5_action_bodies.hpp"

using namespace w5;

namespace kotor_reconstruction { struct StartupString; }
namespace ini_comparison {   // 005E58C0 CExoString::CompareNoCase(const char*) [0886 ini comparison]
int __fastcall CompareNC005e58c0(kotor_reconstruction::StartupString* string, void*, const char* text);
}
namespace w2 { namespace g07 { unsigned __cdecl ftol2_low_006fae8c(double value); } }   // 006FAE8C __ftol2 [w2_g07]

extern "C" {
// ---------------- imports: existing providers (c07b candidate map; object in brackets) ----------------
void* __fastcall recovered_active_timer_004ae830(void* server, void*, U id);                     // GetActiveTimer [w1 b7]
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, U* day, U* time);                // CWorldTimer::GetWorldTime [0457]
void* __fastcall kotor_fastcall_004aede0(void* server, void*);                                   // GetWorldTimer [0457]
U __fastcall recovered_world_time_difference_004ae460(void* timer, void*, U day, U time, U old_day, U old_time,
    U* out_day, U* out_time);                                                                    // SubtractWorldTimes [1025b]
U __fastcall slot_return_zero_0063e7f0_a0(void* self, void*);                                    // return_zero 0063E7F0 [0832]
void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void* spells, void*, int id);         // CSWSpellArray::GetSpell [0820]
U __fastcall kotor_fastcall_004efa80(void* creature, void*);                                     // GetForceItemMask
unsigned __fastcall kt_feat_remaining_uses_005a6680(void* stats, void*, unsigned feat);          // GetFeatRemainingUses (AL)
void __fastcall w5_creature_set_lock_orientation_to_object_004f34a0(void* creature, void*, U id, U force); // [W5 t3]
void __fastcall recovered_combat_round_cleanup_004f7530(void* creature, void*);                  // ProcessPendingCombatActions [clear9]
void* __fastcall recovered_combat_round_attack_004d2b50(void* round, void*, unsigned index);     // CSWSCombatRound::GetAttack [w1 b7]
void __fastcall recovered_combat_round_advance_004d3440(void* round, void*, U time, U flag);     // DecrementRoundLength [clear9]
void __fastcall recovered_combat_round_clear_current_004d3ba0(void* round, void*);               // RemoveSpellAction [clear9]
unsigned __fastcall kt_get_class_005a4e90(void* stats, void*, unsigned index);                   // CSWSCreatureStats::GetClass
int __fastcall recovered_spell_adjusted_force_points_0059b8a0(void* spell, void*, U level, U kind); // GetAdjustedForcePoints
void* __fastcall recovered_message_data_construct_004d69f0(void* data, void*);                   // CSWCCMessageData() [W1 b7]
void __fastcall recovered_message_data_destroy_004d6a40(void* data, void*);                      // ~CSWCCMessageData [W1 b7]
void __fastcall recovered_event_integer_004d76b0(void* data, void*, U index, U value);           // CSWCCMessageData::SetInteger [1017]
U __fastcall recovered_message_get_object_004d64c0(void* data, void*, U index);                  // CSWCCMessageData::GetObjectID
unsigned __fastcall recovered_server_first_pc_004aea10(void* server, void*);                     // GetFirstPCObject
void* __cdecl combat_reuse_message_004aedb0();                                                   // GetSWSMessage [W1 b7]
U __fastcall recovered_cc_message_00568570(void* message, void*, U player, U type, void* data, U a4); // SendServerToPlayerCCMessage
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);                       // GetServerAIMaster
void __fastcall kotor_fastcall_004d68a0(void* message, void*, U a, U b, U c);                    // CSWMessage::CreateWriteMessage [w2_message]
void __fastcall kotor_fastcall_00578570(void* message, void*, U id);                             // WriteOBJECTIDServer [w2_message]
void __fastcall kotor_fastcall_004d6920(void* message, void*, U value_bits, U scale, U bits);    // WriteFLOAT [w2_message]
void __fastcall kotor_fastcall_004d6340(void* message, void*, U value, U bits);                  // WriteDWORD [w2_message]
void __fastcall kotor_fastcall_004d62c0(void* message, void*, U value, U bits);                  // WriteBYTE [w2_message]
U __fastcall kotor_fastcall_004d6440(void* message, void*, void** data, U* size);                // GetWriteMessage [w2_message]
U __fastcall kotor_fastcall_00567e60(void* message, void*, U player, U major, U minor, void* data, U size); // SendServerToPlayerMessage
void* __fastcall recovered_creature_visible_element_00517a20(void* creature, void*, U id);       // GetVisibleListElement [W1 b7]
void* __fastcall kotor_m15_menu_active_property_00553960(void* item, void*, I index);            // CSWSItem::GetActiveProperty
void __fastcall recovered_creature_set_combat_state_004f2610(void* creature, void*, U state, U flag); // SetCombatState [W1 b7]
I __fastcall c2da_current_get_string_00413270(void* table, void*, I row, void* column, void* out); // C2DA::GetCExoStringEntry [0379]
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void* item, void*);                     // CSWItem::GetBaseItem [0820]
void* __fastcall recovered_creature_client_creature_004efb20(void* creature, void*);             // GetClientCreature [W1 b7]
double __fastcall kotor_fastcall_0063c0e0(void* object, void*, U animation);                     // CSWCObject::GetAnimationLength (ST0)
U __fastcall slot_void_return_4_0060e760_a1(void* self, void*, U value);                         // void_return 0060E760 (RET 4)
U __fastcall kotor_fastcall_00555a60(void* item, void*, U8 index);                               // GetUsedActivePropertyUsesLeft (AX)
unsigned __fastcall recovered_stats_class_level_005a5090(void* stats, void*, unsigned index, unsigned flag); // GetClassLevel (AL)
unsigned __fastcall recovered_item_update_notify_00569fc0(void* message, void*, void* player, unsigned id); // ..ItemUpdate_DestroyItem

// ---------------- imports: callees owned by other workers (declared with the original ABI only) ----------------
U __fastcall kotor_fastcall_004d35c0(void* round, void*);                                        // GetSpellActionPending [claude_w5_creature_actions]
void __fastcall kotor_fastcall_004d28f0(void* round, void*, U paused, U id);                     // SetRoundPaused [claude_w5_combat_actions]
void __fastcall kotor_fastcall_004d2920(void* round, void*, U time, U flag);                     // SetPauseTimer [claude_w5_combat_actions]
// combat-attack worker: 004D5F70 StartCombatRound(target, a2, a3, a4, a5) RET 14h
void __fastcall kotor_fastcall_004d5f70(void* round, void*, U target, U a2, U a3, U a4, U a5);
// combat-attack worker: 004D2C30 ResolveEngagedState(target) RET 4, EAX
U __fastcall kotor_fastcall_004d2c30(void* round, void*, U target);
// combat-attack worker: 004D2D60 ResolveMasterState(target, int* engaged) RET 8, EAX
U __fastcall kotor_fastcall_004d2d60(void* round, void*, U target, U* engaged);
// combat-attack worker: 005BBA80 CSWSCreature::ResolveAttack(target, a2..a7) RET 1Ch
void __fastcall kotor_fastcall_005bba80(void* creature, void*, U target, U a2, U a3, U a4, U a5, U a6, U a7);

// owner claude_w5_pathfind_walk_distance: 004F0F90 CSWSCreature::GetPauseReadyAnimation RET 0, EAX
U __fastcall kotor_fastcall_004f0f90(void* creature, void*);
// owner combat-attack (claude_combat_attack_damagestats): 004AE9F0 CServerExoApp::GetDifficultyOption(option) RET 4
U __fastcall kotor_fastcall_004ae9f0(void* server, void*, U option);

// ---------------- exports reconstructed in this file ----------------
void __fastcall kotor_fastcall_004ef450(void* creature, void*);
void __fastcall kotor_fastcall_004d28b0(void* round, void*, U spell, U flag);
U __fastcall kotor_fastcall_004eddd0(void* creature, void*, U spell, U cls, U a3, U a4, U a5);
void __fastcall kotor_fastcall_005a6880(void* stats, void*, U spell, U ready, U cls);
U __fastcall kotor_fastcall_005a55c0(void* stats, void*, U spell);
U __fastcall kotor_fastcall_0059b650(void* spell, void*, U cls);
U __fastcall kotor_fastcall_00550b90(void* rules, void*, U meta);
void __fastcall kotor_fastcall_005a6720(void* stats, void*, U feat);
void __fastcall kotor_fastcall_004cb9e0(void* object, void*, U flags);
void __fastcall kotor_fastcall_004cdf50(void* object, void*, U spell, const Vector* location, U target, U a4, U a5,
    U a6, U a7, U a8, U a9);
void __fastcall kotor_fastcall_004cb890(void* object, void*, void* spell, void* data);
void __fastcall kotor_fastcall_004cdd30(void* object, void*, U caster, U target, const Vector* from, const Vector* to,
    U a5, U a6, U a7, U a8, U a9, U a10, U a11);
U __fastcall kotor_fastcall_0056e6e0(void* message, void*, void* player, U caster, U target, Vector from, Vector to,
    U a5, U a6, U a7, U item_byte, U a8, U a9, U a10, U a11);
void __fastcall kotor_fastcall_004fb9e0(void* creature, void*, U spell, U a2, U a3, U a4, U target);
void __fastcall kotor_fastcall_0055de20(void* item, void*, I index, U owner);
void __fastcall kotor_fastcall_0055d040(void* item, void*);
U __fastcall kotor_fastcall_00569e10(void* message, void*, void* player, U item, U active, U changed, const U8* uses);
}

namespace {
constexpr U k_in_progress_00745dbc = 1;   // .rdata 00745DBC
constexpr U k_complete_00745dc0 = 2;      // .rdata 00745DC0
constexpr U k_failed_00745dc4 = 3;        // .rdata 00745DC4
constexpr U k_effect_silence_007466c4 = 0x12;   // .rdata 007466C4 (dword compared with the effect type word)
constexpr U k_event_spell_impact_007441d4 = 8;  // .rdata 007441D4
constexpr U k_event_destroy_007441e0 = 11;      // .rdata 007441E0
constexpr unsigned char k_lookup_ok_0074666c = 0;   // .rdata 0074666C (byte)
const float k_broadcast_range_007455bc = 900.0f;    // .rdata 007455BC (squared 30 m)
const float k_projectile_range_00747440 = 250.0f;   // .rdata 00747440
const float k_ln_scale_00740c84 = 3.0f;             // .rdata 00740C84
const float k_ln_bias_00741be0 = 2.0f;              // .rdata 00741BE0
const float k_scale_1_5_0073f414 = 1.5f;            // .rdata 0073F414
const float k_scale_0_5_0073e9ac = 0.5f;            // .rdata 0073E9AC
const float k_scale_0_4_0073e320 = 0.4f;            // .rdata 0073E320
const float k_zero_0073d700 = 0.0f;                 // .rdata 0073D700
const float k_ms_0073d6fc = 1000.0f;                // .rdata 0073D6FC
// .data 007A1B50 = 10.0f: read only by AIActionItemCastSpell (0050F612/18); no instruction in the image writes it
const float k_item_near_range_007a1b50 = 10.0f;
const char k_modeltype_00747c2c[] = "MODELTYPE";    // .rdata 00747C2C
const char k_model_b_00747c28[] = "B";              // .rdata 00747C28
const char k_model_f_0073ece0[] = "F";              // .rdata 0073ECE0

inline U addr(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline void* ptr(U v) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(v)); }
inline void* rules() { return kotor_cell_rules_007a3a28; }
inline void* round_of(void* c) { return at<void*>(c, 0x9c8); }          // CSWSCreature +9C8 CSWSCombatRound*
inline void* stats_of(void* c) { return at<void*>(c, 0xa74); }          // CSWSCreature +A74 CSWSCreatureStats*
inline void* spell_of(U id) { return kotor_m15b_spell_array_get_spell_0059b6d0(at<void*>(rules(), 0x8c), nullptr, static_cast<int>(id)); }
inline void* message() { return combat_reuse_message_004aedb0(); }
inline Vector vec_of_bits(U x, U y, U z) { Vector v; std::memcpy(&v.x, &x, 4); std::memcpy(&v.y, &y, 4); std::memcpy(&v.z, &z, 4); return v; }
inline void animate(void* creature, U animation) { vcall<void>(creature, 0x7c, animation); }   // vtbl+7C SetAnimation
// CGameObjectArray::GetGameObject through the server object array (inline view of 004AED70); AL status
inline unsigned char lookup(U id, void** out) { return static_cast<unsigned char>(kotor_fastcall_004d8230(object_array(), nullptr, id, out)); }
void* new_message_data() {                    // operator new(34h) + CSWCCMessageData(); unchecked like the original
    void* memory = live_operator_new(0x34);
    return memory != nullptr ? recovered_message_data_construct_004d69f0(memory, nullptr) : nullptr;
}
void delete_message_data(void* data) {
    if (data == nullptr) return;
    recovered_message_data_destroy_004d6a40(data, nullptr);
    live_free(data);
}
// party combat message for the creature's party slot (00514B44..82 / 0050F254..92)
void party_combat_message(void* self, U id) {
    void* client = client_app();
    const U client_id = kotor_fastcall_005eda50(client, nullptr, at<U>(self, 4));
    const I index = static_cast<I>(recovered_party_index_006349c0(kotor_fastcall_005ed8b0(client, nullptr), nullptr, client_id));
    if (index < 0) return;                                                          // jl
    void* party = kotor_fastcall_005ed8b0(client, nullptr);
    kotor_fastcall_006345e0(static_cast<unsigned char*>(party) + index * 0x88 + 0x24, nullptr, id);
}
// ln(x) exactly as the original: fldln2 / fxch / fyl2x (ST1 = ln 2, ST0 = x)
double x87_ln(double x) {
    double r;
    __asm { fldln2 }
    __asm { fld x }
    __asm { fyl2x }
    __asm { fstp r }
    return r;
}
bool base_type_is(void* item, U8 type) { return at<U8>(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0xac) == type; }
}  // namespace

extern "C" {
// =========================================================================================================
// Small CSWSCreature / CSWSCombatRound / CSWSCreatureStats / CSWSpell / CSWRules members
// ---------------------------------------------------------------------------------------------------------
// 004EF450 CSWSCreature::RemoveSpellActionFromRound: RET 0 (EAX not meaningful)
void __fastcall kotor_fastcall_004ef450(void* self, void*) {
    if (kotor_fastcall_004d35c0(round_of(self), nullptr) == 1)                      // 004EF453..61
        recovered_combat_round_clear_current_004d3ba0(round_of(self), nullptr);     // 004EF463..6A (tail jmp)
}

// 004D28B0 CSWSCombatRound::SetSpellCastRound(spell, flag): RET 8
void __fastcall kotor_fastcall_004d28b0(void* round, void*, U spell, U flag) {
    void* owner = at<void*>(round, 0x9b4);                                          // 004D28B4
    at<U>(round, 0x93c) = flag;                                                     // 004D28BA
    at<U>(round, 0x938) = 1;                                                        // 004D28C0
    at<U>(owner, 0x540) = 4;                                                        // 004D28CA
    at<U>(at<void*>(round, 0x9b4), 0x548) = spell;                                  // 004D28D4..DE
}

// 005A6880 CSWSCreatureStats::SetSpellLikeAbilityReady(spell, ready byte, class byte): RET 0Ch.
// Entries (12 bytes) of the list at +128: +0 spell, +4 ready, +8 class byte.
void __fastcall kotor_fastcall_005a6880(void* stats, void*, U spell, U ready, U cls) {
    void* list = at<void*>(stats, 0x128);                                           // 005A6880
    const I count = at<I>(list, 4);                                                 // 005A6887
    if (count <= 0) return;                                                         // 005A688C..8E
    unsigned char* entries = at<unsigned char*>(list, 0);                           // 005A689B
    const U8 r = static_cast<U8>(ready), c = static_cast<U8>(cls);
    for (I i = 0; i < count; ++i) {                                                 // 005A68A0..BB
        unsigned char* e = entries + i * 12;
        if (at<U>(e, 0) != spell) continue;                                         // 005A68A0..A6
        if (at<U>(e, 4) == r) continue;                                             // 005A68A8..AE
        if (at<U8>(e, 8) != c) continue;                                            // 005A68B0..B3
        at<U>(e, 4) = r;                                                            // 005A68C4..CA
        return;
    }
}

// 005A55C0 CSWSCreatureStats::UnReadySpell(spell): RET 4, EAX 0/1. Pays the adjusted force points from the
// current pool (+126), overflowing into +124 (clamped at 0).
U __fastcall kotor_fastcall_005a55c0(void* stats, void*, U spell_id) {
    void* spell = spell_of(spell_id);                                               // 005A55C7..D7
    if (spell == nullptr) return 0;                                                 // 005A55DC..DE
    const I cost = recovered_spell_adjusted_force_points_0059b8a0(spell, nullptr,
        static_cast<U>(static_cast<I>(at<short>(stats, 0x17e))), at<U>(at<void*>(stats, 0x24), 0xa88)); // 005A55E0..F7
    const I current = at<short>(stats, 0x126);                                      // 005A55FC
    if (current >= cost) {                                                          // 005A5603..05 (jl: signed)
        at<U16>(stats, 0x126) = static_cast<U16>(current - cost);                   // 005A5607..0A
        return 1;
    }
    at<U16>(stats, 0x124) = static_cast<U16>(at<U16>(stats, 0x124) + static_cast<U16>(current - cost)); // 005A561A..1C
    at<U16>(stats, 0x126) = 0;                                                      // 005A562A
    if (at<short>(stats, 0x124) < 0) at<U16>(stats, 0x124) = 0;                     // 005A5623..33 (jge: signed word)
    return 1;
}

// 004EDDD0 CSWSCreature::DecrementSpellReadyCount(spell, class byte, a3, a4, a5): RET 14h, EAX
U __fastcall kotor_fastcall_004eddd0(void* self, void*, U spell, U cls, U, U, U a5) {
    if (at<U>(self, 0x960) != 0) return 1;                                          // 004EDDD0..D8 -> 004EDDF8
    if (static_cast<U8>(cls) == 0xfe) {                                             // 004EDDDA..E5
        kotor_fastcall_005a6880(stats_of(self), nullptr, spell, 0, a5);             // 004EDDE7..F3
        return 1;                                                                   // 004EDDF8
    }
    return kotor_fastcall_005a55c0(stats_of(self), nullptr, spell);                 // 004EDE00..0A
}

// 0059B650 CSWSpell::GetSpellLevel(class byte): RET 4, AL
U __fastcall kotor_fastcall_0059b650(void* spell, void*, U cls) {
    switch (static_cast<U8>(cls)) {                                                 // 0059B650..5A, table 0059B680
    case 0: case 1: case 2: return 0xff;                                            // 0059B661 (or al, FF)
    case 3: return at<U8>(spell, 0x3c);                                             // 0059B666
    case 4: return at<U8>(spell, 0x3d);                                             // 0059B66C
    case 5: return at<U8>(spell, 0x3e);                                             // 0059B672
    default: return at<U8>(spell, 0x3f);                                            // 0059B678 (ja)
    }
}

// 00550B90 CSWRules::GetMetaMagicLevelCost(byte): RET 4, AL (index table 00550BD8, jump table 00550BC4)
U __fastcall kotor_fastcall_00550b90(void*, void*, U meta) {
    switch (static_cast<U8>(meta)) {
    case 1: return 2;                                                               // 00550BA9
    case 2: return 1;                                                               // 00550BAE
    case 4: return 3;                                                               // 00550BB3
    case 8: return 4;                                                               // 00550BB8
    case 16: case 32: return 1;                                                     // 00550BAE
    default: return 0;                                                              // 00550BBD
    }
}

// 005A6720 CSWSCreatureStats::DecrementFeatRemainingUses(word feat): RET 4. Increments the used count byte (+2)
// of every feat entry (+0C list, +10 count) with that feat id.
void __fastcall kotor_fastcall_005a6720(void* stats, void*, U feat) {
    if (at<I>(stats, 0x10) <= 0) return;                                            // 005A6720..27
    const U16 id = static_cast<U16>(feat);
    U16 i = 0;
    do {
        unsigned char* e = at<unsigned char**>(stats, 0xc)[i];                      // 005A6732..35 (list re-read)
        if (at<U16>(e, 0) == id) ++at<U8>(e, 2);                                    // 005A6738..3D
        ++i;                                                                        // 005A6743..44 (word counter)
    } while (static_cast<I>(i) < at<I>(stats, 0x10));                               // 005A6740..49 (count re-read)
}

// 004CB9E0 CSWSObject::CalculateLastSpellProjectileTime(byte flags): RET 4. +1C8 = projectile flight time (ms).
void __fastcall kotor_fastcall_004cb9e0(void* self, void*, U flags) {
    Vector delta = vec_of_bits(0, 0, 0);                                            // [esp+C..14]
    void* spell = spell_of(at<U>(self, 0x1c0));                                     // 004CB9E6..11
    if (spell == nullptr) return;                                                   // 004CBA16..1A
    if (at<U>(spell, 0xf4) == 0) { at<U>(self, 0x1c8) = 0; return; }                // 004CBA20..39
    if (at<U>(self, 0x1bc) != k_object_invalid) {                                   // 004CBA3C..48
        void* target = nullptr;                                                     // [esp+8]
        if (lookup(at<U>(self, 0x1bc), &target) == k_lookup_ok_0074666c) {         // 004CBA4A..71
            delta.x = static_cast<float>(static_cast<double>(at<float>(target, 0x90)) - at<float>(self, 0x90)); // 004CBA77..83
            delta.y = static_cast<float>(static_cast<double>(at<float>(target, 0x94)) - at<float>(self, 0x94)); // 004CBA87..93
            delta.z = static_cast<float>(static_cast<double>(at<float>(target, 0x98)) - at<float>(self, 0x98)); // 004CBA97..D7
        }
    } else {
        delta.x = static_cast<float>(static_cast<double>(at<float>(self, 0x1b0)) - at<float>(self, 0x90)); // 004CBA9F..AB
        delta.y = static_cast<float>(static_cast<double>(at<float>(self, 0x1b4)) - at<float>(self, 0x94)); // 004CBAAF..BB
        delta.z = static_cast<float>(static_cast<double>(at<float>(self, 0x1b8)) - at<float>(self, 0x98)); // 004CBABF..D7
    }
    float t = static_cast<float>(x87_ln(kotor_cdecl_004ab100(&delta)) * k_ln_scale_00740c84 + k_ln_bias_00741be0); // 004CBAE7..0C
    U8 kind = static_cast<U8>(flags);                                               // 004CBAF5
    if (kind == 0) kind = at<U8>(spell, 0x108);                                     // 004CBAFC..12
    switch (kind) {                                                                 // 004CBB18..21, table 004CBBD4
    case 1: t = static_cast<float>(static_cast<double>(t) + t); break;              // 004CBB28..2C
    case 3: at<U>(self, 0x1c8) = 0x7d0; return;                                     // 004CBB30..3F
    case 5: t = static_cast<float>(static_cast<double>(t) * k_scale_1_5_0073f414); break; // 004CBB42..46
    case 7: t = static_cast<float>(kotor_cdecl_004ab100(&delta) * k_scale_0_5_0073e9ac); break; // 004CBB4E..58
    case 8: t = static_cast<float>(static_cast<double>(t) * k_scale_0_4_0073e320); break; // 004CBB63..67
    default: break;                                                                 // 2, 4, 6, out of range
    }
    if (!(t > k_zero_0073d700) && !(t != t)) { at<U>(self, 0x1c8) = 1; return; }     // 004CBB71..91 (<= 0, ordered)
    const U ms = w2::g07::ftol2_low_006fae8c((kotor_cdecl_004ab100(&delta) / t) * k_ms_0073d6fc); // 004CBB94..AB
    at<U>(self, 0x1c8) = ms;                                                        // 004CBBB0
    if (at<U>(spell, 0x108) == 6) at<U>(self, 0x1c8) = ms + 0x9c4;                  // 004CBBB6..C4
}

// =========================================================================================================
// Spell broadcasting
// ---------------------------------------------------------------------------------------------------------
// 0056E6E0 CSWSMessage::SendServerToPlayerSafeProjectileSpell(player, caster, target, Vector from, Vector to,
//   a5, a6 (6/7 kind byte), a7, item byte, a8, a9, a10, a11): RET 44h
U __fastcall kotor_fastcall_0056e6e0(void* message, void*, void* player, U caster, U target, Vector from, Vector to,
    U a5, U a6, U a7, U item_byte, U a8, U a9, U a10, U a11) {
    kotor_fastcall_004d68a0(message, nullptr, 0x31, 0xffffffffu, 1);               // 0056E6E5..ED
    kotor_fastcall_00578570(message, nullptr, caster);                              // 0056E6F2..F9
    kotor_fastcall_00578570(message, nullptr, target);                              // 0056E6FE..05
    kotor_fastcall_004d6920(message, nullptr, bits(from.x), 0x3f800000u, 0x20);     // 0056E70A..18
    kotor_fastcall_004d6920(message, nullptr, bits(from.y), 0x3f800000u, 0x20);     // 0056E71D..2B
    kotor_fastcall_004d6920(message, nullptr, bits(from.z), 0x3f800000u, 0x20);     // 0056E730..3E
    kotor_fastcall_004d6920(message, nullptr, bits(to.x), 0x3f800000u, 0x20);       // 0056E743..51
    kotor_fastcall_004d6920(message, nullptr, bits(to.y), 0x3f800000u, 0x20);       // 0056E756..64
    kotor_fastcall_004d6920(message, nullptr, bits(to.z), 0x3f800000u, 0x20);       // 0056E769..77
    kotor_fastcall_004d6340(message, nullptr, a5, 0x20);                            // 0056E77C..85
    kotor_fastcall_004d62c0(message, nullptr, a6, 8);                               // 0056E78A..93
    kotor_fastcall_004d6340(message, nullptr, a10, 0x20);                           // 0056E798..A1
    kotor_fastcall_004d6340(message, nullptr, a11, 0x20);                           // 0056E7A6..AF
    const U8 kind = static_cast<U8>(a6);
    if (kind == 6) {                                                                // 0056E7B4..B9
        kotor_fastcall_004d6340(message, nullptr, a7, 0x20);                        // 0056E7BB..C2
    } else if (kind == 7) {                                                         // 0056E7C9..CC
        kotor_fastcall_004d6340(message, nullptr, a7, 0x20);                        // 0056E7CE..D5
        kotor_fastcall_004d62c0(message, nullptr, a9, 8);                           // 0056E7DA..03
    } else {
        kotor_fastcall_004d62c0(message, nullptr, a9, 8);                           // 0056E7E0..E7
        kotor_fastcall_004d62c0(message, nullptr, item_byte, 8);                    // 0056E7EC..F5
        kotor_fastcall_004d62c0(message, nullptr, a8, 8);                           // 0056E7FA..03
    }
    void* data = nullptr; U size = 0;                                               // [esp+C], [esp+8]
    if (kotor_fastcall_004d6440(message, nullptr, &data, &size) == 0) return 0;     // 0056E808..22
    return kotor_fastcall_00567e60(message, nullptr, at<U>(player, 4), 0x22, 1, data, size); // 0056E825..3D
}

// 004CDD30 CSWSObject::BroadcastSafeProjectile(caster, target, Vector* from, Vector* to, a5, a6, a7, a8, a9, a10,
//   a11): RET 2Ch. Every creature of the area with a client that is within 250 m of either end gets the message.
void __fastcall kotor_fastcall_004cdd30(void* self, void*, U caster, U target, const Vector* from, const Vector* to,
    U a5, U a6, U a7, U a8, U a9, U a10, U a11) {
    U item_byte = 0xff;                                                             // [esp+18] byte (004CDD46)
    void* caster_creature = kotor_fastcall_004ae770(server_app(), nullptr, caster); // 004CDD42..4B
    if (caster_creature != nullptr) {                                               // 004CDD50..54
        void* item = recovered_equipment_item_005a4c20(at<void*>(caster_creature, 0xa2c), nullptr, 0x10); // 004CDD56..60
        if (item != nullptr) item_byte = at<U8>(item, 0xc);                         // 004CDD65..6C
    }
    void* area_object = nullptr;                                                    // [esp+14]
    (void)lookup(at<U>(self, 0x8c), &area_object);                                  // 004CDD70..90 (status unused)
    if (area_object == nullptr) return;                                             // 004CDD95..9B
    void* area = vcall<void*>(area_object, 0x28);                                   // 004CDDA1..A3 vtbl+28 AsSWSArea
    if (area == nullptr) return;                                                    // 004CDDA6..A8
    unsigned char* list = static_cast<unsigned char*>(area) + 0x190;                // 004CDDAE..BA (null test kept below)
    if (list == nullptr) return;
    if (at<I>(list, 4) <= 0) return;                                                // 004CDDC0..C7
    const float range2 = static_cast<float>(static_cast<double>(k_projectile_range_00747440) * k_projectile_range_00747440); // 004CDE53..5F
    for (I i = 0; i < at<I>(list, 4); ++i) {                                        // 004CDDD5..32 (count re-read)
        void* creature = kotor_fastcall_004ae770(server_app(), nullptr, at<U*>(list, 0)[i]); // 004CDDD5..E8
        if (creature == nullptr) continue;                                          // 004CDDED..F1
        void* client = kotor_fastcall_004ae910(server_app(), nullptr, at<U>(creature, 4)); // 004CDDF7..04
        if (client == nullptr) continue;                                            // 004CDE09..0D
        const double dx1 = static_cast<double>(at<float>(creature, 0x90)) - from->x; // 004CDE13..19
        const double dy1 = static_cast<double>(at<float>(creature, 0x94)) - from->y; // 004CDE1B..21
        const double dz1 = static_cast<double>(at<float>(creature, 0x98)) - from->z; // 004CDE24..2A
        const float dx2 = static_cast<float>(static_cast<double>(at<float>(creature, 0x90)) - to->x); // 004CDE2D..35
        const float dy2 = static_cast<float>(static_cast<double>(at<float>(creature, 0x94)) - to->y); // 004CDE39..42
        const float dz2 = static_cast<float>(static_cast<double>(at<float>(creature, 0x98)) - to->z); // 004CDE46..4F
        const double near_from = (dz1 * dz1 + dy1 * dy1) + dx1 * dx1;              // 004CDE63..71
        if (!(near_from <= range2)) {                                               // 004CDE73..82 (jnp: <= ordered)
            const double near_to = (static_cast<double>(dz2) * dz2 + static_cast<double>(dy2) * dy2) +
                                   static_cast<double>(dx2) * dx2;                 // 004CDE84..9E
            if (!(near_to <= range2)) continue;                                     // 004CDEA0..A9 (jp: skip)
        }
        kotor_fastcall_0056e6e0(message(), nullptr, client, caster, target, *from, *to, a5, a6, a7, item_byte,
                                a8, a9, a10, a11);                                  // 004CDEAB..1B
    }
}

// 004CB890 CSWSObject::BroadcastSpellData(CSWSpell*, CSWCCMessageData*): RET 8. CC message 8 to the first PC's
// player when the first PC is within 30 m (squared 900).
void __fastcall kotor_fastcall_004cb890(void* self, void*, void*, void* data) {
    void* area_object = nullptr;                                                    // [esp+4]
    (void)lookup(at<U>(self, 0x8c), &area_object);                                  // 004CB891..B7
    if (area_object == nullptr) return;                                             // 004CB8BC..C2
    if (vcall<void*>(area_object, 0x28) == nullptr) return;                         // 004CB8C8..CF vtbl+28 AsSWSArea
    const U pc_id = recovered_server_first_pc_004aea10(server_app(), nullptr);       // 004CB8D5..E0
    void* pc = kotor_fastcall_004ae770(server_app(), nullptr, pc_id);               // 004CB8E5..F1
    if (pc == nullptr) return;                                                      // 004CB8F6..FA
    void* player = kotor_fastcall_004ae910(server_app(), nullptr, pc_id);           // 004CB900..09
    const double dx = static_cast<double>(at<float>(pc, 0x90)) - at<float>(self, 0x90); // 004CB90E..14
    const double dy = static_cast<double>(at<float>(pc, 0x94)) - at<float>(self, 0x94); // 004CB91E..24
    const double dz = static_cast<double>(at<float>(pc, 0x98)) - at<float>(self, 0x98); // 004CB92A..30
    if (player == nullptr) return;                                                  // 004CB936 -> 004CB9CD
    if (!((dz * dz + dy * dy) + dx * dx <= k_broadcast_range_007455bc)) return;      // 004CB93C..5D (jp: > or NaN)
    if (vcall<void*>(self, 0x30) != nullptr &&                                      // 004CB95F..6C vtbl+30 AsSWSCreature
        recovered_message_get_object_004d64c0(data, nullptr, 1) != k_object_invalid) { // 004CB96E..7C
        void* item = recovered_item_lookup_004ae760(server_app(), nullptr,
                                                    recovered_message_get_object_004d64c0(data, nullptr, 1)); // 004CB97E..92
        if (item != nullptr) recovered_event_integer_004d76b0(data, nullptr, 3, at<U>(item, 0xc)); // 004CB997..A3
    }
    recovered_cc_message_00568570(message(), nullptr, at<U>(player, 4), 8, data, 0); // 004CB9A8..C1
}

// 004FB9E0 CSWSCreature::BroadcastSpellCast(spell, a2, a3, a4, target): RET 14h. Rebuilds the creature's
// "spell cast seen by" id list (+950 CExoArrayList<ulong>) from the area creatures within the rules range (+14).
// The CC message data it builds is never sent (original behaviour): it is constructed, filled and destroyed.
void __fastcall kotor_fastcall_004fb9e0(void* self, void*, U spell_id, U, U, U, U target) {
    void* spell = spell_of(spell_id);                                               // 004FB9F8..14 ([esp+C])
    void* area = recovered_object_area_004cb120(self, nullptr);                     // 004FBA12..18
    if (area == nullptr) return;                                                    // 004FBA1D..21
    unsigned char* creatures = static_cast<unsigned char*>(area) + 0x190;           // 004FBA27..32 ([esp+24])
    if (creatures == nullptr) return;
    unsigned char* seen = static_cast<unsigned char*>(self) + 0x950;                // ESI
    if (at<void*>(seen, 0) != nullptr) {                                            // 004FBA38..47
        live_free(at<void*>(seen, 0));                                              // 004FBA49..4F
        at<void*>(seen, 0) = nullptr;                                               // 004FBA52
        at<U>(seen, 8) = 0;                                                         // 004FBA54
    }
    at<U>(seen, 4) = 0;                                                             // 004FBA59
    void* data = new_message_data();                                                // 004FBA57..77
    recovered_event_object_004d7780(data, nullptr, 0, at<U>(self, 4));              // 004FBA79..89
    recovered_event_object_004d7780(data, nullptr, 1, k_object_invalid);            // 004FBA8E..97
    recovered_event_object_004d7780(data, nullptr, 2, target);                      // 004FBA9C..A5
    recovered_event_integer_004d76b0(data, nullptr, 0, spell_id);                   // 004FBAAA..AF
    recovered_event_integer_004d76b0(data, nullptr, 2, 1);                          // 004FBAB4..BA
    for (I i = 0; i < at<I>(creatures, 4); ++i) {                                   // 004FBABF..AE (count re-read)
        void* creature = kotor_fastcall_004ae770(server_app(), nullptr, at<U*>(creatures, 0)[i]); // 004FBAD4..E7
        if (creature == nullptr) continue;                                          // 004FBAEC..F0
        const double dx = static_cast<double>(at<float>(creature, 0x90)) - at<float>(self, 0x90); // 004FBAF6..01
        const double dy = static_cast<double>(at<float>(creature, 0x94)) - at<float>(self, 0x94); // 004FBB07..0D
        const double dz = static_cast<double>(at<float>(creature, 0x98)) - at<float>(self, 0x98); // 004FBB13..19
        const double range = at<float>(rules(), 0x14);                              // 004FBB1F
        if (range * range < (dz * dz + dy * dy) + dx * dx) continue;                // 004FBB22..45 (jnp: below = skip)
        if (at<U8>(spell, 0x140) == 1 && at<U>(spell, 0x144) == 0) {                // 004FBB47..5C
            // the original stores the element over the loop counter slot, which is rewritten from EBX right after
            (void)recovered_creature_visible_element_00517a20(creature, nullptr, at<U>(self, 4)); // 004FBB5E..67
            continue;
        }
        const U id = at<U>(creature, 4);                                            // 004FBB6D
        if (at<U>(seen, 4) == at<U>(seen, 8)) {                                     // 004FBB70..76
            const U grow = at<U>(seen, 8) != 0 ? at<U>(seen, 8) * 2 : 0x10;         // 004FBB78..80
            menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(seen), nullptr, grow); // 004FBB85..88
        }
        at<U*>(seen, 0)[at<U>(seen, 4)] = id;                                       // 004FBB8D..92
        at<U>(seen, 4) += 1;                                                        // 004FBB95..9D
    }
    delete_message_data(data);                                                      // 004FBBB4..C6
}

// 004CDF50 CSWSObject::SpellCastAndImpact(spell, Vector* location, target, a4, a5 (item / 7F000000), a6, a7,
//   a8 (flags byte), a9): RET 24h. Sends the spell data / safe projectile once (+1CC) and queues the impact event
//   (8) carrying a 2Ch CSWSSpellImpactEvent-like record, delayed by the projectile time +1C8.
void __fastcall kotor_fastcall_004cdf50(void* self, void*, U spell_id, const Vector* location, U target, U, U a5,
    U, U, U a8, U a9) {
    alignas(4) unsigned char unused_text[8];                                        // CExoString [esp+14]
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(unused_text), nullptr); // 004CDF6E..72
    void* spell = spell_of(spell_id);                                               // 004CDF77..92 (EBP)
    if (spell != nullptr) {                                                         // 004CDF94..96
        const U kind = static_cast<U8>(a8) != 0 ? 7u : 6u;                          // 004CDF9C..AB ([esp+10] byte)
        if (at<U>(self, 0x1cc) == 0) {                                              // 004CDFAE..B4
            if (a9 == 0 && vcall<void*>(self, 0x30) != nullptr) {                   // 004CDFBA..CD
                void* data = new_message_data();                                    // 004CDFD3..F1 (unchecked)
                recovered_event_object_004d7780(data, nullptr, 0, at<U>(self, 4));  // 004CDFF3..00
                recovered_event_object_004d7780(data, nullptr, 1, a5);              // 004CE005..0E
                recovered_event_object_004d7780(data, nullptr, 2, target);          // 004CE013..1C
                recovered_event_integer_004d76b0(data, nullptr, 0, spell_id);       // 004CE021..26
                recovered_event_integer_004d76b0(data, nullptr, 1, at<U8>(spell, 0x14e)); // 004CE02B..37
                const short level = at<short>(stats_of(vcall<void*>(self, 0x30)), 0x17e); // 004CE03C..4B
                const U kind_fp = at<U>(vcall<void*>(self, 0x30), 0xa88);           // 004CE049..57
                recovered_event_integer_004d76b0(data, nullptr, 2,
                    static_cast<U>(recovered_spell_adjusted_force_points_0059b8a0(spell, nullptr,
                        static_cast<U>(static_cast<I>(level)), kind_fp)));         // 004CE05D..6E
                void* stats = stats_of(vcall<void*>(self, 0x30));                   // 004CE073..7A
                const short pool = static_cast<short>(at<U16>(stats, 0x126) + at<U16>(stats, 0x124)); // 004CE080..8E
                recovered_event_integer_004d76b0(data, nullptr, 3, static_cast<U>(static_cast<I>(pool))); // 004CE091..96
                kotor_fastcall_004cb890(self, nullptr, spell, data);                // 004CE09B..9F
                delete_message_data(data);                                          // 004CE0A4..B5
            }
            void* target_creature = kotor_fastcall_004ae770(server_app(), nullptr, target); // 004CE0BC..CA
            if (target_creature != nullptr)                                         // 004CE0CF..D5
                kotor_fastcall_004cdd30(self, nullptr, at<U>(self, 4), target,
                    reinterpret_cast<const Vector*>(static_cast<unsigned char*>(self) + 0x90),
                    reinterpret_cast<const Vector*>(static_cast<unsigned char*>(target_creature) + 0x90),
                    at<U>(self, 0x1c8), kind, spell_id, 0, a8, 0, 0);               // 004CE0D7..FD -> 004CE128
            else
                kotor_fastcall_004cdd30(self, nullptr, at<U>(self, 4), target,
                    reinterpret_cast<const Vector*>(static_cast<unsigned char*>(self) + 0x90),
                    reinterpret_cast<const Vector*>(static_cast<unsigned char*>(self) + 0x1b0),
                    at<U>(self, 0x1c8), kind, spell_id, 0, a8, 0, 0);               // 004CE0FF..28
            void* memory = live_operator_new(0x2c);                                 // 004CE12D..36
            void* event = nullptr;
            if (memory != nullptr) {                                                // 004CE13F..46
                at<U>(memory, 0x10) = 0; at<U>(memory, 0x14) = 0; at<U>(memory, 0x18) = 0; // 004CE14B..51 (Vector ctor)
                kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(
                    static_cast<unsigned char*>(memory) + 0x1c), nullptr);          // 004CE148..54
                event = memory;
            }
            at<U>(event, 0) = spell_id;                                             // 004CE169 (unchecked)
            assign_string_005e5c50(static_cast<unsigned char*>(event) + 0x1c, nullptr,
                                   static_cast<unsigned char*>(spell) + 0x34);      // 004CE15D..6B
            at<U>(event, 8) = target;                                               // 004CE170..78
            copy_vector(static_cast<unsigned char*>(event) + 0x10, location);       // 004CE17B..8B
            at<U>(event, 4) = at<U>(self, 4);                                       // 004CE18E..97
            at<U>(event, 0xc) = a5;                                                 // 004CE191..9A
            if (recovered_object_area_004cb120(self, nullptr) != nullptr)           // 004CE19D..A4
                at<U>(event, 0x24) = at<U>(recovered_object_area_004cb120(self, nullptr), 0x120); // 004CE1A6..B5
            else
                at<U>(event, 0x24) = k_object_invalid;                              // 004CE1BA..C0
            recovered_event_enqueue_004b08d0(recovered_server_ai_master_004aed80(server_app(), nullptr), nullptr,
                0, at<U>(self, 0x1c8), at<U>(self, 4), at<U>(self, 4), k_event_spell_impact_007441d4, event); // 004CE1C3..E9
        }
        at<U>(self, 0x1cc) = 1;                                                     // 004CE1EE
    }
    frame_LogStringDtor_005e5c20(unused_text, nullptr);                             // 004CE1F8..04
}

// =========================================================================================================
// Item uses
// ---------------------------------------------------------------------------------------------------------
// 00569E10 CSWSMessage::SendServerToPlayerUpdateActiveItemProperties(player, item, active mask, changed mask,
//   BYTE* uses): RET 14h
U __fastcall kotor_fastcall_00569e10(void* message, void*, void* player, U item, U active, U changed, const U8* uses) {
    kotor_fastcall_004d68a0(message, nullptr, 5, 0xffffffffu, 1);                   // 00569E17..1F
    kotor_fastcall_00578570(message, nullptr, item);                                // 00569E24..2B
    kotor_fastcall_004d62c0(message, nullptr, active, 8);                           // 00569E30..39
    kotor_fastcall_004d62c0(message, nullptr, changed, 8);                          // 00569E3E..47
    const U mask = static_cast<U8>(changed);                                        // 00569E4C
    for (U i = 0; i < 8; ++i)                                                       // 00569E56..78
        if ((mask & (1u << i)) != 0) kotor_fastcall_004d62c0(message, nullptr, uses[i], 8); // 00569E5F..71
    void* data = nullptr; U size = 0;                                               // [esp+14], [esp+10]
    if (kotor_fastcall_004d6440(message, nullptr, &data, &size) == 0) return 0;     // 00569E7A..96
    return kotor_fastcall_00567e60(message, nullptr, at<U>(player, 4), 0x18, 1, data, size); // 00569E99..B1
}

// 0055D040 CSWSItem::UpdateUsedActiveProperties: RET 0. Recomputes the active-property mask (+238) and the
// per-property uses (+239..+240, first 8 properties) and tells the possessor's player what changed.
void __fastcall kotor_fastcall_0055d040(void* item, void*) {
    U8 active = 0;                                                                  // BL
    U8 uses[8] = {0, 0, 0, 0, 0, 0, 0, 0};                                          // [esp+14..1B]
    if (at<I>(item, 0x248) > 0) {                                                   // 0055D057..61
        U offset = 0;                                                               // EBP
        for (I i = 0; i < at<I>(item, 0x248); ++i, offset += 0x1c) {                // 0055D070..101 (count re-read)
            unsigned char* p = (static_cast<I>(offset) >= 0 && i < at<I>(item, 0x248))
                ? at<unsigned char*>(item, 0x250) + offset : nullptr;               // 0055D070..86
            if (at<U16>(p, 0) != 0xa) continue;                                     // 0055D088..8C (unchecked)
            const U type = at<U16>(p, 6);                                           // 0055D08E
            const U8 bit = static_cast<U8>((i & 31) < 8 ? (1u << (i & 31)) : 0u);   // shl dl, cl (byte, CL & 31)
            switch (type) {                                                         // 0055D092..9F, table 0055D268
            case 1:                                                                 // 0055D0A6
                if (at<U8>(p, 0xc) == 0) break;
                if (offset < 0xe0) uses[i] = static_cast<U8>(kotor_fastcall_00555a60(item, nullptr, static_cast<U8>(i))); // 0055D0AD..C7
                active |= bit;                                                      // 0055D0C1..CB / 0055D259..5F
                break;
            case 2: case 3: case 4: case 5: case 6:                                 // 0055D0CF
                if (at<I>(item, 0x258) <= 0) break;
                if (offset < 0xe0) uses[i] = static_cast<U8>(kotor_fastcall_00555a60(item, nullptr, static_cast<U8>(i))); // 0055D0D9..E9
                active |= bit;                                                      // 0055D0ED..F3
                break;
            case 7: case 13: case 14: case 15: case 16: case 17: case 18:           // 0055D0ED
                active |= bit;
                break;
            case 8: case 9: case 10: case 11: case 12:                              // 0055D23A
                if (at<U8>(p, 0x10) == 0) break;
                if (offset < 0xe0) uses[i] = static_cast<U8>(kotor_fastcall_00555a60(item, nullptr, static_cast<U8>(i))); // 0055D245..55
                active |= bit;                                                      // 0055D259..5F
                break;
            default: break;                                                         // ja
            }
        }
    }
    U8 changed = 0;                                                                 // [esp+10] byte
    for (U k = 0; k < 8; ++k)                                                       // 0055D10F..19C
        if (uses[k] != at<U8>(item, 0x239 + k)) changed = static_cast<U8>(changed | (1u << k));
    if (active == at<U8>(item, 0x238) && changed == 0) return;                      // 0055D19F..A9
    at<U8>(item, 0x238) = active;                                                   // 0055D1B3
    for (U k = 0; k < 8; ++k) at<U8>(item, 0x239 + k) = uses[k];                    // 0055D1B9..E8
    void* msg = message();                                                          // 0055D1EE..F7
    void* player = kotor_fastcall_004ae910(server_app(), nullptr, at<U>(item, 0x268)); // 0055D1FC..0E
    if (player == nullptr) return;                                                  // 0055D213..15
    kotor_fastcall_00569e10(msg, nullptr, player, at<U>(item, 0x14), at<U8>(item, 0x238), changed,
                            static_cast<const U8*>(static_cast<void*>(static_cast<unsigned char*>(item) + 0x239))); // 0055D217..2D
}

// 0055DE20 CSWSItem::DecrementUses(property index, owner id): RET 8. Consumes one use of the active property;
// a used-up stack / charge item is destroyed (DestroyItem update + AI event 0Bh), otherwise the properties
// are re-published. Unless the client options +8C ("unlimited uses") is set.
void __fastcall kotor_fastcall_0055de20(void* item, void*, I index, U owner) {
    if (at<U>(kotor_fastcall_005ed700(client_app(), nullptr), 0x8c) != 0) return;   // 0055DE20..3E
    unsigned char* p = (index >= 0 && index < at<I>(item, 0x248))
        ? at<unsigned char*>(item, 0x250) + index * 0x1c : nullptr;                 // 0055DE46..63 (ESI)
    U charge_property = 0;                                                          // EBP
    U destroy = 0;                                                                  // [esp+10]
    U timed = 0;                                                                    // [esp+20] (the index slot)
    U charges = 0;                                                                  // EAX for the charge cases
    switch (at<U16>(p, 6)) {                                                        // 0055DE65..84 (unchecked), tables 0055E168/0055E144
    case 1:                                                                         // 0055DE8B stack
        if (at<I>(item, 0x28c) > 1) at<I>(item, 0x28c) -= 1;                        // 0055DE8B..97 (jle: signed)
        else { at<U>(p, 0xc) = 0; destroy = 1; }                                    // 0055DEA2..A5
        break;
    case 2: charges = 5; goto charge;                                               // 0055DEB2
    case 3: charges = 4; goto charge;                                               // 0055DF2D
    case 4: charges = 3; goto charge;                                               // 0055DF39
    case 5: charges = 2; goto charge;                                               // 0055DF48
    case 6: charges = 1;                                                            // 0055DF57
    charge:
        charge_property = 1;
        if (at<I>(item, 0x258) >= static_cast<I>(charges)) at<I>(item, 0x258) -= static_cast<I>(charges); // 0055DEBC..C8
        if (at<I>(item, 0x248) > 0) {                                               // 0055DECE..D8
            // ESI walks the property array and is left on the last property for the test at 0055E01A
            for (I k = 0; k < at<I>(item, 0x248); ++k) {                            // 0055DEE0..14 (count re-read)
                const I off = k * 0x1c;
                p = (off >= 0 && k < at<I>(item, 0x248)) ? at<unsigned char*>(item, 0x250) + off : nullptr;
                if (p == nullptr || at<U16>(p, 0) != 0xa) continue;                 // 0055DEFC..08
                const U t = at<U16>(p, 6);                                          // 0055DF0E
                const I left = at<I>(item, 0x258);
                if ((t == 2 && left < 5) || (t == 3 && left < 4) || (t == 4 && left < 3) ||
                    (t == 5 && left < 2) || (t == 6 && left < 1))                   // 0055DF12..03
                    at<U>(p, 0xc) = 0;                                              // 0055E005
            }
        }
        break;
    case 8: case 9: case 10: case 11: case 12:                                      // 0055DF63 per-day uses
        timed = 1;                                                                  // 0055DF68
        if (at<U8>(p, 0x10) > 0) at<U8>(p, 0x10) = static_cast<U8>(at<U8>(p, 0x10) - 1); // 0055DF63..74
        if (at<U8>(p, 0x10) < 1) at<U>(p, 0xc) = 0;                                 // 0055DF77..81
        break;
    case 14: case 15: case 16: case 17: case 18: {                                  // 0055DF89 recharge timer
        timed = 1;                                                                  // 0055DF9C
        U day = 0, time = 0;                                                        // [esp+14], [esp+18]
        frame_GetWorldTime_004ade40(kotor_fastcall_004aede0(server_app(), nullptr), nullptr, &day, &time); // 0055DF89..AB
        at<U>(p, 0x14) = day;                                                       // 0055DFB0..B4
        at<U>(p, 0x18) = time;                                                      // 0055DFB7..BB
        at<U>(p, 0xc) = 0;                                                          // 0055DFBE
        break;
    }
    default: break;                                                                 // 7, 13, out of range
    }
    bool remove_item = false;
    if (at<U8>(p, 0xc) == 0) {                                                      // 0055E01A..1D (ESI, unchecked)
        if (base_type_is(item, 0x19) || base_type_is(item, 0x2d) || base_type_is(item, 0x2f)) { // 0055E023..55
            remove_item = true;
        } else if (charge_property != 0) {                                          // 0055E057..59
            const I count = at<I>(item, 0x248);                                     // EBP
            I k = 0;
            if (count > 0) {                                                        // 0055E063..65
                for (; k < at<I>(item, 0x248); ++k) {                               // 0055E070..B8
                    const I off = k * 0x1c;
                    unsigned char* q = (off >= 0 && k < count) ? at<unsigned char*>(item, 0x250) + off : nullptr;
                    if (q == nullptr || at<U16>(q, 0) != 0xa) continue;             // 0055E080..88
                    const U t = at<U16>(q, 6);
                    if (t != 2 && t != 3 && t != 4 && t != 5 && t != 6) continue;   // 0055E08A..A5
                    if (at<U8>(q, 0xc) != 0) break;                                 // 0055E0A7..AA
                }
            }
            if (k == count) remove_item = true;                                     // 0055E0BA..BC
        }
    }
    if (!remove_item && destroy == 0) { kotor_fastcall_0055d040(item, nullptr); return; } // 0055E0BE..C2 -> 0055E132
    if (timed != 0) { kotor_fastcall_0055d040(item, nullptr); return; }             // 0055E0C4..C8 -> 0055E132
    void* player = kotor_fastcall_004ae910(server_app(), nullptr, owner);           // 0055E0CA..D8
    if (player != nullptr)                                                          // 0055E0DD..DF
        recovered_item_update_notify_00569fc0(message(), nullptr, player, at<U>(item, 0x14)); // 0055E0E1..F6
    at<U>(item, 0x288) |= 0x100;                                                    // 0055E0FB
    recovered_event_enqueue_004b08d0(recovered_server_ai_master_004aed80(server_app(), nullptr), nullptr, 0, 0,
        owner, at<U>(item, 0x14), k_event_destroy_007441e0, nullptr);              // 0055E105..23
}

// =========================================================================================================
// 00514AF0 CSWSCreature::AIActionCastSpell(node): RET 4.
//   node +38 spell, +3C class byte (FF none, FE spell-like), +40, +44 metamagic, +48, +4C target, +50..58 target
//   location, +5C flags (bit 30 instant, bit 31 no-broadcast/"cheat", low byte projectile kind 6/7), +60 feat
//   (FFFFFFFF none), +64, +70 first-run flag.
U __fastcall w5_action_cast_spell_00514af0(void* self, void*, void* node) {
    alignas(4) unsigned char unused_text[8];                                        // CExoString [esp+68]
    Vector location = vec_of_bits(0, 0, 0);                                         // [esp+70] (00514B12..22)
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(unused_text), nullptr); // 00514B0E..2A
    U result = k_failed_00745dc4;
    if (at<U>(self, 0xa88) != 0) party_combat_message(self, 0xbc50);                // 00514B2F..82
    at<U>(self, 0x52c) = 0xbb8;                                                     // 00514B8B
    const U spell_id = at<U>(node, 0x38);                                           // [esp+14]
    const U cls = at<U>(node, 0x3c);                                                // [esp+20]
    const U n40 = at<U>(node, 0x40);                                                // [esp+3C]
    const U meta = at<U>(node, 0x44);                                               // [esp+28]
    const U n48 = at<U>(node, 0x48);                                                // [esp+40]
    const U target = at<U>(node, 0x4c);                                             // EBP
    const U flags = at<U>(node, 0x5c);                                              // [esp+48]
    const U feat = at<U>(node, 0x60);                                               // [esp+30]
    const U n64 = at<U>(node, 0x64);                                                // [esp+60]
    const U instant = (flags >> 30) & 1;                                            // [esp+50], EBX
    const U quiet = flags >> 31;                                                    // [esp+1C]
    const U kind = static_cast<U8>(flags) != 0 ? 7u : 6u;                           // [esp+4C] byte
    void* spell = nullptr;                                                          // [esp+18]
    void* target_object = nullptr;                                                  // [esp+10]
    U conjure = 0, cast = 0, extra = 0;                                             // [esp+2C], [esp+34], [esp+38]
    U elapsed = 0;                                                                  // [esp+44]
    if (vcall<U>(self, 0x94) == 1) goto fail;                                       // 00514B87..9E
    if (slot_return_zero_0063e7f0_a0(self, nullptr) == 1) goto fail;                // 00514BA4..AE
    if (recovered_creature_incapacitated_004ef890(self, nullptr) == 1) goto fail;   // 00514BB4..BE
    location = vec_of_bits(at<U>(node, 0x50), at<U>(node, 0x54), at<U>(node, 0x58)); // 00514BC4..2F
    if (target != k_object_invalid) {                                               // 00514C4A..60
        w5_creature_set_lock_orientation_to_object_004f34a0(self, nullptr, target, 0); // 00514C62..67
    } else {
        Vector direction;                                                           // [esp+7C]
        direction.x = static_cast<float>(static_cast<double>(location.x) - at<float>(self, 0x90)); // 00514C86..90
        direction.y = static_cast<float>(static_cast<double>(location.y) - at<float>(self, 0x94)); // 00514C7C..94
        direction.z = static_cast<float>(static_cast<double>(location.z) - at<float>(self, 0x98)); // 00514C6E..9B
        if (vector_is_zero_0046a620(&direction, nullptr) == 0) {                    // 00514CA2..A9
            vector_normalize_004ab130(&direction, nullptr);                         // 00514CAB..AF
            w1x_object_set_orientation_004cd170(self, nullptr, &direction);         // 00514CB4..BB
        }
    }
    spell = spell_of(spell_id);                                                     // 00514CC0..D1
    if (spell == nullptr) goto fail;                                                // 00514CD6..DE -> 00514D57
    {
        const U mask = kotor_fastcall_004efa80(self, nullptr);                      // 00514CE0
        if ((at<U>(spell, 0x18c) & mask) != 0) goto fail;                           // 00514CE5..EF
        const U required = at<U>(spell, 0x190);
        if ((required & mask) != required) goto fail;                               // 00514CF1..FF
    }
    if (feat != 0xffffffffu &&                                                      // 00514D01..08
        static_cast<U8>(kt_feat_remaining_uses_005a6680(stats_of(self), nullptr, feat)) == 0) goto fail; // 00514D0A..18
    at<U>(self, 0x528) = target == k_object_invalid ? at<U>(self, 0x8c) : target;    // 00514D1A..30
    target_object = kotor_fastcall_004ae750(server_app(), nullptr, target);         // 00514D36..4B
    if (target != k_object_invalid && target_object == nullptr) goto fail;          // 00514D45..53
    if (target_object != nullptr && vcall<void*>(target_object, 0x30) != nullptr) { // 00514D9A..A9
        if (vcall<U>(vcall<void*>(target_object, 0x30), 0x94) != 0) goto fail;      // 00514DAB..C0
        if (recovered_creature_incapacitated_004ef890(vcall<void*>(target_object, 0x30), nullptr) != 0) goto fail; // 00514DC2..D4
    }
    if (quiet == 0) w5_creature_activity_manager_004f87d0(self, nullptr, 1);        // 00514DDA..E6
    if (instant == 1) {                                                             // 00514DEB..EE
        kotor_fastcall_004d5f70(round_of(self), nullptr, target, 0, 0, 0, 0);       // 00514DF4..01
        kotor_fastcall_004d28f0(round_of(self), nullptr, 1, k_object_invalid);      // 00514E06..12
        kotor_fastcall_004d2920(round_of(self), nullptr, 0x5dc, 0);                 // 00514E17..23
        animate(self, 0x2719);                                                      // 00514E28..31
        void* attack = recovered_combat_round_attack_004d2b50(round_of(self), nullptr, at<U8>(round_of(self), 0x96c)); // 00514E34..42
        at<U16>(attack, 0x64) = 0x1c;                                               // 00514E55
        kotor_fastcall_005bba80(self, nullptr, target, 1, 0x5dc, 0, 0, 4, 0);       // 00514E47..5B
        at<U>(self, 0x1cc) = 0;                                                     // 00514E7E
        const Vector zero = vec_of_bits(0, 0, 0);                                   // [esp+7C] zeroed after its address is pushed
        kotor_fastcall_004cdf50(self, nullptr, spell_id, &zero, target, cls, k_object_invalid, 0, 0, 0, 0); // 00514E60..99
        at<U>(self, 0x1cc) = 0;                                                     // 00514E9E
        result = k_complete_00745dc0;                                               // 00514EA4
        goto done;
    }
    conjure = at<U>(spell, 0x40);                                                   // 00514EB3..C2
    cast = at<U>(spell, 0xa8);
    extra = at<U>(spell, 0xec);
    if (at<U>(round_of(self), 0x934) == 0) {                                        // 00514EC6..DC
        U engaged = kotor_fastcall_004d2c30(round_of(self), nullptr, target);       // 00514EE2..F4 ([esp+24])
        const U master = kotor_fastcall_004d2d60(round_of(self), nullptr, target, &engaged); // 00514EE8..F8 ([esp+54])
        kotor_fastcall_004d5f70(round_of(self), nullptr, target, engaged, master, 0, 0); // 00514EFD..12
        kotor_fastcall_004d28b0(round_of(self), nullptr, spell_id, quiet);          // 00514F17..27
        kotor_fastcall_004d28f0(round_of(self), nullptr, 1, at<U>(self, 4));        // 00514F2C..38
        const U total = extra + cast + conjure;                                     // 00514F3D..51 (EBX)
        kotor_fastcall_004d2920(round_of(self), nullptr, total, 0);                 // 00514F45..57
        recovered_combat_round_advance_004d3440(round_of(self), nullptr, total, 0); // 00514F5C..65
        if (engaged != 0 && target_object != nullptr && vcall<void*>(target_object, 0x30) != nullptr &&
            at<U>(self, 4) != target) {                                             // 00514F6A..90
            if (quiet != 0) goto timing;                                            // 00514F92..98
            void* other = vcall<void*>(target_object, 0x30);                        // 00514F9E..B0 ([esp+10])
            kotor_fastcall_004d5f70(at<void*>(other, 0x9c8), nullptr, at<U>(self, 4), engaged,
                                    master == 0 ? 1u : 0u, 0, 0);                   // 00514FA7..C8
            kotor_fastcall_004d28f0(at<void*>(other, 0x9c8), nullptr, 1, at<U>(other, 4)); // 00514FCD..DD
            kotor_fastcall_004d2920(at<void*>(other, 0x9c8), nullptr, total, 0);    // 00514FE2..EF
            recovered_combat_round_advance_004d3440(at<void*>(other, 0x9c8), nullptr, total, 0); // 00514FF4..01
        } else if (quiet != 0) {                                                    // 00515008..0E
            goto timing;
        }
        kotor_fastcall_004fb9e0(self, nullptr, spell_id, meta, cls, feat, target);  // 00515014..2B
    } else if (at<U>(round_of(self), 0x958) == 0 && at<I>(round_of(self), 0x968) < 1) { // 00515035..4A
        kotor_fastcall_004d28b0(round_of(self), nullptr, spell_id, quiet);          // 00515050..5A
        kotor_fastcall_004d28f0(round_of(self), nullptr, 1, at<U>(self, 4));        // 0051505F..6B
        const U total = extra + cast + conjure;                                     // 00515070..84
        kotor_fastcall_004d2920(round_of(self), nullptr, total, 0);                 // 00515078..8A
        recovered_combat_round_advance_004d3440(round_of(self), nullptr, total, 0); // 0051508F..98
        if (at<U>(round_of(self), 0x9b8) != 0 && target_object != nullptr &&
            vcall<void*>(target_object, 0x30) != nullptr) {                         // 0051509D..BE
            void* other = vcall<void*>(target_object, 0x30);                        // 005150C0..CC ([esp+24])
            kotor_fastcall_004d28f0(at<void*>(other, 0x9c8), nullptr, 1, at<U>(other, 4)); // 005150C9..DB
            kotor_fastcall_004d2920(at<void*>(other, 0x9c8), nullptr, total, 0);    // 005150E0..ED
            recovered_combat_round_advance_004d3440(at<void*>(other, 0x9c8), nullptr, total, 0); // 005150F2..FF
        }
    }
timing:
    {
        U day = 0, time = 0;                                                        // [esp+5C], [esp+58]
        frame_GetWorldTime_004ade40(recovered_active_timer_004ae830(server_app(), nullptr, at<U>(self, 4)), nullptr,
                                    &day, &time);                                   // 00515104..22
        U day_delta = 0;                                                            // [esp+64]
        recovered_world_time_difference_004ae460(kotor_fastcall_004aede0(server_app(), nullptr), nullptr, day, time,
            at<U>(self, 0xbc), at<U>(self, 0xc0), &day_delta, &elapsed);            // 00515127..58
    }
    if (elapsed == 0) {                                                             // 0051515D..67
        at<U>(self, 0x1c0) = spell_id;                                              // 00515169..6D
        copy_vector(static_cast<unsigned char*>(self) + 0x1b0, &location);          // 00515173..96
        at<U8>(self, 0x1c4) = static_cast<U8>(n40);                                 // 0051518A
        at<U>(self, 0x1bc) = target;                                                // 00515190
        at<U>(self, 0x1cc) = 0;                                                     // 00515199
    }
    if (elapsed < conjure) {                                                        // 0051519F..A5 (jae: unsigned)
        const U16 style = at<U16>(spell, 0x44);                                     // 005151A7..AB
        if (style == 3) animate(self, 0x2af8);                                      // 005151AF..BE
        else if (style == 2) animate(self, 0x2720);                                 // 005151CC..DB
        else if (style == 7) animate(self, 0x27b2);                                 // 005151E9..FE (1 -> default)
        else animate(self, 0x271f);                                                 // 0051520C..15
        result = k_in_progress_00745dbc;
        goto done;
    }
    if (at<U>(node, 0x70) == 1) {                                                   // 00515223..27
        at<U>(node, 0x70) = at<U>(round_of(self), 0x9b8) != 0 ? 1u : 0u;            // 0051522D..46
        if (quiet == 0) {                                                           // 00515249
            if (recovered_object_area_004cb120(self, nullptr) == nullptr) goto fail; // 0051524F..58
            if (cls != 0xff && at<U>(self, 0x960) == 0) {                           // 0051525E..72
                if (n48 == 0) {                                                     // 00515274..7A
                    if (kotor_fastcall_004eddd0(self, nullptr, spell_id, cls, n40, meta, n64) == 0) goto fail; // 0051527C..98
                } else {
                    if (n40 == 0)                                                   // 0051529D..9F (results unused)
                        (void)kotor_fastcall_0059b650(spell, nullptr,
                            kt_get_class_005a4e90(stats_of(self), nullptr, cls));   // 005152A1..B2
                    (void)kotor_fastcall_00550b90(rules(), nullptr, meta);          // 005152B7..C2
                }
                at<U>(self, 0x960) = 1;                                             // 005152C7..CB
            }
            const I count = at<I>(self, 0x128);                                     // 005152D5..DF
            if (count > 0) {
                void** effects = at<void**>(self, 0x124);                           // 005152E1
                for (I i = 0; i < at<I>(self, 0x128); ++i) {                        // 005152F0..06 (count re-read)
                    if (static_cast<U>(at<U16>(effects[i], 8)) != k_effect_silence_007466c4) continue; // 005152F0..F8
                    animate(self, 0x2711);                                          // 0051530A..13
                    w5_creature_set_lock_orientation_to_object_004f34a0(self, nullptr, k_object_invalid, 0); // 00515316..20
                    at<U>(self, 0x960) = 0;                                         // 0051532B
                    if (instant == 0) recovered_creature_feedback_004ede10(self, nullptr, 0x41, nullptr); // 00515325..41
                    goto fail;                                                      // 00515335 / 00515346
                }
            }
        }
        at<U8>(self, 0x1d4) = static_cast<U8>(cls);                                 // 00515357
        at<U>(self, 0x95c) = k_object_invalid;                                      // 00515367
        at<U>(self, 0x1d0) = n48;                                                   // 00515370
        at<U8>(self, 0x1d5) = static_cast<U8>(meta);                                // 00515376
        kotor_fastcall_004cb9e0(self, nullptr, flags);                              // 0051536D..7C
    }
    if (elapsed >= conjure + cast && at<U>(self, 0x1cc) != 0) {                     // 0051538F..A6
        if (elapsed < extra + cast + conjure) {                                     // 005153AC..B6
            if (at<U16>(spell, 0xf0) == 0) animate(self, 0x27b1);                   // 005153B8..CF
            at<U>(self, 0x960) = 0;                                                 // 005153D4
            recovered_combat_round_cleanup_004f7530(self, nullptr);                 // 005153DE
            kotor_fastcall_004ef450(self, nullptr);                                 // 005153E5
            result = k_failed_00745dc4;                                             // 005153EA -> 00514D63
            goto done;
        }
        animate(self, kotor_fastcall_004f0f90(self, nullptr));                      // 005153EF..FB
        w5_creature_set_lock_orientation_to_object_004f34a0(self, nullptr, k_object_invalid, 0); // 005153FE..08
        at<U>(self, 0x960) = 0;                                                     // 00515414
        if (feat != 0xffffffffu) kotor_fastcall_005a6720(stats_of(self), nullptr, feat); // 0051540D..27
        kotor_fastcall_004ef450(self, nullptr);                                     // 0051542C..2E
        recovered_combat_round_cleanup_004f7530(self, nullptr);                     // 00515433..35
        result = k_complete_00745dc0;                                               // 0051543A
        goto done;
    }
    {
        const U16 style = at<U16>(spell, 0xa6);                                     // 00515445..49
        if (style == 2) animate(self, 0x2722);                                      // 00515450..5F
        else if (style == 1) animate(self, 0x2721);                                 // 00515464..6F
        else if (style == 3) animate(self, 0x2723);                                 // 00515471..80
        else if (style == 4) animate(self, 0x2724);                                 // 00515485..90
        else if (style == 7) animate(self, 0x274d);                                 // 00515492..A1
        else if (style != 8 && style != 9) animate(self, 0x274d);                   // 005154A6..BB
    }
    result = k_in_progress_00745dbc;                                                // 005155A5 / 0051555E
    if (at<U>(self, 0x1cc) != 0) goto done;                                         // 005154BE..C6
    if (quiet != 0) {                                                               // 005154CC..D2
        void* object = kotor_fastcall_004ae750(server_app(), nullptr, target);      // 005154D8..E2
        void* server_object = object != nullptr ? vcall<void*>(object, 0x10) : nullptr; // 005154E7..F4 vtbl+10 AsSWSObject
        const Vector* to = server_object != nullptr
            ? reinterpret_cast<const Vector*>(static_cast<unsigned char*>(server_object) + 0x90)   // 00515511
            : reinterpret_cast<const Vector*>(static_cast<unsigned char*>(self) + 0x1b0);          // 0051553D
        kotor_fastcall_004cdd30(self, nullptr, at<U>(self, 4), target,
            reinterpret_cast<const Vector*>(static_cast<unsigned char*>(self) + 0x90), to,
            at<U>(self, 0x1c8), kind, spell_id, 0, flags, 0, 0);                    // 005154F6..4F
        at<U>(self, 0x1cc) = 1;                                                     // 00515554
        goto done;
    }
    at<I>(round_of(self), 0x968) += 1;                                              // 00515569..84
    kotor_fastcall_004cdf50(self, nullptr, spell_id, &location, target, cls, k_object_invalid, 0, 0, flags, instant); // 00515569..A0
    goto done;
fail:                                                                               // 00514D55
    kotor_fastcall_004ef450(self, nullptr);                                         // 00514D57
    recovered_combat_round_cleanup_004f7530(self, nullptr);                         // 00514D5C..5E
    result = k_failed_00745dc4;                                                     // 00514D63
done:
    frame_LogStringDtor_005e5c20(unused_text, nullptr);                             // 00514D69..78
    return result;
}

// =========================================================================================================
// 0050F170 CSWSCreature::AIActionItemCastSpell(node): RET 4.
//   node +38 item, +3C active property index, +44 target, +48..50 target location, +54/+58 start world time,
//   +5C/+60/+64 pending follow-up animation (flag, animation word, duration), +6E state word, +70.
U __fastcall w5_action_item_cast_spell_0050f170(void* self, void*, void* node) {
    const U item_id = at<U>(node, 0x38);                                            // [esp+24]
    const U property = at<U>(node, 0x3c);                                           // [esp+34]
    const U target = at<U>(node, 0x44);                                             // [esp+18]
    const Vector location = vec_of_bits(at<U>(node, 0x48), at<U>(node, 0x4c), at<U>(node, 0x50)); // [esp+64..6C]
    U start_day = 0, start_time = 0;                                                // [esp+30], [esp+2C]
    if (at<U16>(node, 0x6e) < 9) {                                                  // 0050F1B3..E0 (jae: unsigned)
        frame_GetWorldTime_004ade40(recovered_active_timer_004ae830(server_app(), nullptr, at<U>(self, 4)), nullptr,
                                    &start_day, &start_time);                       // 0050F1E2..00
        at<U>(node, 0x54) = start_day;                                              // 0050F20D
        at<U>(node, 0x58) = start_time;                                             // 0050F210
        at<U16>(node, 0x6e) = 9;                                                    // 0050F213
    } else {
        start_day = at<U>(node, 0x54);                                              // 0050F219..23
        start_time = at<U>(node, 0x58);
    }
    U pending_animation = 0, pending_time = 0, pending = 0;                         // [esp+1C], [esp+20], EBX
    if (at<U16>(node, 0x6e) > 9) {                                                  // 0050F22B..37 (jbe)
        pending_animation = at<U16>(node, 0x60);                                    // 0050F239..43
        pending_time = at<U>(node, 0x64);                                           // 0050F23D..48
        pending = at<U>(node, 0x5c);                                                // 0050F240
    }
    if (at<U>(self, 0xa88) != 0) party_combat_message(self, 0xbc50);                // 0050F24C..92
    at<U>(self, 0x52c) = 0xbb8;                                                     // 0050F29F
    if (vcall<U>(self, 0x94) == 1 || slot_return_zero_0063e7f0_a0(self, nullptr) == 1 ||
        recovered_creature_incapacitated_004ef890(self, nullptr) == 1) {            // 0050F297..D2
        animate(self, 0x2710);                                                      // 0050FADA..E3
        at<U>(self, 0x96c) = 0;                                                     // 0050FAE6
        kotor_fastcall_004ef450(self, nullptr);                                     // 0050FAF0..F2
        recovered_combat_round_cleanup_004f7530(self, nullptr);                     // 0050FAF7..F9
        return k_failed_00745dc4;                                                   // 0050FAFE
    }
    U elapsed = 0;                                                                  // [esp+28]
    {
        U day = 0, time = 0;                                                        // [esp+44], [esp+40]
        frame_GetWorldTime_004ade40(recovered_active_timer_004ae830(server_app(), nullptr, at<U>(self, 4)), nullptr,
                                    &day, &time);                                   // 0050F2D8..F6
        U day_delta = 0;                                                            // [esp+58]
        recovered_world_time_difference_004ae460(kotor_fastcall_004aede0(server_app(), nullptr), nullptr, day, time,
            start_day, start_time, &day_delta, &elapsed);                           // 0050F2FB..28
    }
    if (pending != 0) {                                                             // 0050F32D..2F
        if (elapsed < pending_time) {                                               // 0050F331..3B (jae: unsigned)
            animate(self, pending_animation);                                       // 0050F33D..45
            return k_in_progress_00745dbc;                                          // 0050F348
        }
        animate(self, kotor_fastcall_004f0f90(self, nullptr));                      // 0050F352..5C
        at<U>(self, 0x96c) = 0;                                                     // 0050F361
        kotor_fastcall_004ef450(self, nullptr);                                     // 0050F36B
        recovered_combat_round_cleanup_004f7530(self, nullptr);                     // 0050F372
        return k_complete_00745dc0;                                                 // 0050F377
    }
    void* object = kotor_fastcall_004ae750(server_app(), nullptr, target);          // 0050F381..8B (EDI)
    bool bad_target = target != k_object_invalid && object == nullptr;              // 0050F390..9C
    if (!bad_target && object != nullptr && vcall<void*>(object, 0x30) != nullptr) { // 0050F3B6..BF
        if (vcall<U>(vcall<void*>(object, 0x30), 0x94) != 0) bad_target = true;     // 0050F3C1..D4
        else if (recovered_creature_incapacitated_004ef890(vcall<void*>(object, 0x30), nullptr) != 0) bad_target = true; // 0050F3D6..E6
    }
    if (bad_target) {                                                               // 0050F39E
        animate(self, kotor_fastcall_004f0f90(self, nullptr));                      // 0050F39E..AA
        at<U>(self, 0x96c) = 0;                                                     // 0050FAE6
        kotor_fastcall_004ef450(self, nullptr);
        recovered_combat_round_cleanup_004f7530(self, nullptr);
        return k_failed_00745dc4;
    }
    at<U>(self, 0x528) = target == k_object_invalid ? at<U>(self, 0x8c) : target;    // 0050F3E8..FE
    void* target_creature = object != nullptr ? vcall<void*>(object, 0x30) : nullptr; // 0050F404..17 ([esp+20])
    void* item = recovered_item_lookup_004ae760(server_app(), nullptr, item_id);    // 0050F41B..28 ([esp+14])
    void* active = nullptr;                                                         // [esp+54]
    void* spell = nullptr;                                                          // EBP
    U spell_id = 0;                                                                 // [esp+3C]
    if (item != nullptr) {                                                          // 0050F42F..37
        active = kotor_m15_menu_active_property_00553960(item, nullptr, static_cast<I>(property)); // 0050F439..47
        if (active != nullptr) {                                                    // 0050F44B
            spell_id = at<U16>(active, 2);                                          // 0050F44D..5E
            spell = spell_of(spell_id);                                             // 0050F451..67
        }
    }
    if (spell == nullptr) {                                                         // 0050F469..6B
        animate(self, kotor_fastcall_004f0f90(self, nullptr));                      // 0050F46D..79
        at<U>(self, 0x96c) = 0;                                                     // 0050F47C
        kotor_fastcall_004ef450(self, nullptr);                                     // 0050FAF0
        recovered_combat_round_cleanup_004f7530(self, nullptr);
        return k_failed_00745dc4;
    }
    if (at<U>(spell, 0x148) != 0) {                                                 // 0050F487..91
        w5_creature_activity_manager_004f87d0(self, nullptr, 1);                    // 0050F493..95
        recovered_creature_set_combat_state_004f2610(self, nullptr, 1, 1);          // 0050F49A..A0
    } else {
        w5_creature_activity_manager_004f87d0(self, nullptr, 2);                    // 0050F4A7..A9
    }
    alignas(4) unsigned char model[8];                                              // CExoString [esp+5C]
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(model), nullptr); // 0050F4C2..D6
    {
        alignas(4) unsigned char column[8];                                         // CExoString [esp+48]
        frame_LogStringCtor_005e5a90(column, nullptr, k_modeltype_00747c2c);        // 0050F4DB..EB
        c2da_current_get_string_00413270(at<void*>(at<void*>(rules(), 0xb8), 0xc), nullptr,
            static_cast<I>(at<U16>(self, 0xa60)), column, model);                  // 0050F4F0..1B (appearance row)
        frame_LogStringDtor_005e5c20(column, nullptr);                              // 0050F520..2C
    }
    auto* model_string = reinterpret_cast<kotor_reconstruction::StartupString*>(model);
    const U large = (ini_comparison::CompareNC005e58c0(model_string, nullptr, k_model_b_00747c28) != 0 ||
                     ini_comparison::CompareNC005e58c0(model_string, nullptr, k_model_f_0073ece0) != 0) ? 1u : 0u; // 0050F531..59 (EDI)
    at<U>(node, 0x70) = 0;                                                          // 0050F55E..69
    U close = 0;                                                                    // [esp+38]
    U animation = 0;                                                                // [esp+10]
    U threshold = 0;                                                                // EDI
    U duration = 0;                                                                 // EBP
    switch (at<U8>(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0xac)) {  // 0050F567..8F (tables 0050FB30/0050FB18)
    case 6:                                                                         // 0050F5D9
        if (large != 0) {
            Vector delta;                                                           // [esp+48]
            kotor_cdecl_00423bc0(&delta, &location, reinterpret_cast<const Vector*>(static_cast<unsigned char*>(self) + 0x90)); // 0050F5DD..EE
            const double distance2 = (static_cast<double>(delta.z) * delta.z + static_cast<double>(delta.y) * delta.y) +
                                     static_cast<double>(delta.x) * delta.x;       // 0050F5F3..10
            const double near2 = static_cast<double>(k_item_near_range_007a1b50) * k_item_near_range_007a1b50; // 0050F612..18
            if (near2 < distance2 || near2 == distance2) {                          // 0050F61E..25 (jp not taken)
                animation = 0x2791;                                                 // 0050F627
                threshold = 0x320;                                                  // 0050F658
            } else {
                animation = 0x2792;                                                 // 0050F631
                threshold = 0x2bc;                                                  // 0050F639
            }
        } else {
            animation = 0x2711;                                                     // 0050F650
            threshold = 0x320;                                                      // 0050F658
        }
        duration = 0x5dc;                                                           // 0050F63E / 0050F65D
        close = 1;                                                                  // 0050F643 / 0050F662
        break;
    case 12: {                                                                      // 0050F68C
        void* client = recovered_creature_client_creature_004efb20(self, nullptr);  // 0050F68C..8E
        duration = 0x5dc;                                                           // 0050F695
        if (client != nullptr)                                                      // 0050F693..9A
            threshold = w2::g07::ftol2_low_006fae8c(kotor_fastcall_0063c0e0(client, nullptr, 0x2af9)) - 0x32; // 0050F69C..AF
        else
            threshold = 0x12c;                                                      // 0050F6B4
        animation = elapsed < threshold ? 0x2af9u : 0x2afau;                        // 0050F6B9..C4 (sbb: unsigned)
        break;
    }
    case 20:                                                                        // 0050F66C
        animation = large != 0 ? 0x2798u : 0x2711u;
        threshold = 0x258;                                                          // 0050F680
        duration = 0x3e8;                                                           // 0050F685
        break;
    case 25: case 26: case 45:                                                      // 0050F596
        animation = large != 0 ? 0x2756u : 0x2711u;
        threshold = 0x2ee;                                                          // 0050F5A7
        duration = 0x5dc;                                                           // 0050F5AC
        break;
    case 47:                                                                        // 0050F5B6
        animation = large != 0 ? 0x2798u : 0x2711u;
        threshold = 0x2ee;                                                          // 0050F5CA
        duration = 0x5dc;                                                           // 0050F5CF
        break;
    default:                                                                        // 0050F6CA
        animation = 0x2721;                                                         // 0050F6D0
        threshold = 1;                                                              // 0050F6D8
        duration = at<U>(spell, 0xa8) + 1;                                          // 0050F6CA / 0050F6DD
        break;
    }
    U result;
    if (at<U>(round_of(self), 0x934) == 0) {                                        // 0050F6E0..F2
        U engaged = 0;                                                              // [esp+1C]
        U master;
        if (close != 0) {                                                           // 0050F6F8..06
            engaged = 1;                                                            // 0050F708..0D
            master = 1;
        } else {
            engaged = kotor_fastcall_004d2c30(round_of(self), nullptr, target);     // 0050F713..25
            master = kotor_fastcall_004d2d60(round_of(self), nullptr, target, &engaged); // 0050F719..29
        }
        kotor_fastcall_004d5f70(round_of(self), nullptr, target, engaged, master, 0, 0); // 0050F72E..3F
        kotor_fastcall_004d28b0(round_of(self), nullptr, 0, 0);                     // 0050F744..4E
        kotor_fastcall_004d28f0(round_of(self), nullptr, 1, at<U>(self, 4));        // 0050F753..5F
        kotor_fastcall_004d2920(round_of(self), nullptr, duration, 0);              // 0050F764..6D
        recovered_combat_round_advance_004d3440(round_of(self), nullptr, duration, 0); // 0050F772..7B
    }
    if (at<U>(self, 0x96c) == 0) {                                                  // 0050F780..88
        if (base_type_is(item, 0x19) || base_type_is(item, 0x19)) {                 // 0050F78A..AC (the same test twice)
            if (at<U>(self, 0x9d4) == 0 || kotor_fastcall_004ae9f0(server_app(), nullptr, 2) == 0) // 0050F7AE..CA
                slot_void_return_4_0060e760_a1(self, nullptr, k_object_invalid);    // 0050F7CC..D3
        }
        at<U>(self, 0x96c) = 1;                                                     // 0050F7D8
    }
    if (elapsed < threshold) {                                                      // 0050F7E2..E6 (jae: unsigned)
        const U16 uses = static_cast<U16>(kotor_fastcall_00555a60(item, nullptr, static_cast<U8>(property))); // 0050F7EC..F7
        const U16 type = at<U16>(active, 6);                                        // 0050F802..06
        if (uses < 1 && type != 0xd && type != 7) {                                 // 0050F7FC..14
            animate(self, kotor_fastcall_004f0f90(self, nullptr));                  // 0050F816..22
            at<U>(self, 0x96c) = 0;                                                 // 0050F827
            kotor_fastcall_004ef450(self, nullptr);                                 // 0050F831
            recovered_combat_round_cleanup_004f7530(self, nullptr);                 // 0050F838
            recovered_creature_feedback_004ede10(self, nullptr, 0x17, nullptr);     // 0050F83D..43
            result = k_failed_00745dc4;                                             // 0050F848
        } else {
            at<U>(self, 0x1c0) = spell_id;                                          // 0050F853..5B
            copy_vector(static_cast<unsigned char*>(self) + 0x1b0, &location);      // 0050F857..89
            at<U>(self, 0x95c) = item_id;                                           // 0050F874..7A
            at<U>(self, 0x1bc) = target;                                            // 0050F883
            at<U>(self, 0x1cc) = 0;                                                 // 0050F88C
            at<U>(self, 0x964) = 0;                                                 // 0050F892
            kotor_fastcall_004cb9e0(self, nullptr, 0);                              // 0050F880..98
            void* stats = stats_of(self);                                           // 0050F89D
            at<U>(self, 0x968) = static_cast<U8>(recovered_stats_class_level_005a5090(stats, nullptr,
                static_cast<U8>(at<U8>(stats, 0x89) - 1), 0));                     // 0050F8A3..BD
            if (target_creature != nullptr &&
                (base_type_is(item, 0x19) || base_type_is(item, 0x2d) || base_type_is(item, 0x2f))) // 0050F8B4..F3
                animate(target_creature, static_cast<U16>(animation));              // 0050F8F5..FF
            else
                animate(self, static_cast<U16>(animation));                         // 0050F90D..17
            result = k_in_progress_00745dbc;                                        // 0050F902 / 0050F91A
        }
    } else {
        if (at<U>(self, 0x1cc) == 0) {                                              // 0050F925..2D
            copy_vector(static_cast<unsigned char*>(self) + 0x1b0, &location);      // 0050F933..52
            at<U>(self, 0x1c0) = spell_id;                                          // 0050F959
            at<U>(self, 0x1bc) = target;                                            // 0050F95F
            at<U>(self, 0x1cc) = 0;                                                 // 0050F965
            at<U>(self, 0x95c) = item_id;                                           // 0050F96F
            at<U>(self, 0x964) = 1;                                                 // 0050F975
            kotor_fastcall_004cb9e0(self, nullptr, 0);                              // 0050F955..7F
            void* stats = stats_of(self);                                           // 0050F984
            at<U>(self, 0x968) = static_cast<U8>(recovered_stats_class_level_005a5090(stats, nullptr,
                static_cast<U8>(at<U8>(stats, 0x89) - 1), 0));                     // 0050F98A..9F
            if (target_creature != nullptr &&
                (base_type_is(item, 0x19) || base_type_is(item, 0x2d) || base_type_is(item, 0x2f))) // 0050F9A5..E1
                animate(target_creature, static_cast<U16>(animation));              // 0050F9E3..F5
            else
                animate(self, static_cast<U16>(animation));                         // 0050F9EB..F5
            at<I>(round_of(self), 0x968) += 1;                                      // 0050F9F8..0D
            kotor_fastcall_004cdf50(self, nullptr, spell_id, &location, target, 0xff, item_id, 0, 0, 0, 0); // 0050FA04..29
            if (kotor_fastcall_004d35c0(round_of(self), nullptr) == 1)              // 0050FA2E..3C
                recovered_combat_round_clear_current_004d3ba0(round_of(self), nullptr); // 0050FA3E..44
            kotor_fastcall_0055de20(item, nullptr, static_cast<I>(property), at<U>(self, 4)); // 0050FA49..58
            if (base_type_is(item, 0xc)) {                                          // 0050FA5D..6C
                at<U>(node, 0x5c) = 1;                                              // 0050FA75
                at<U16>(node, 0x60) = 0x2afa;                                       // 0050FA7C
                at<U>(node, 0x64) = duration;                                       // 0050FA82
                at<U16>(node, 0x6e) = 0xc;                                          // 0050FA85
            }
        }
        if (elapsed < duration) {                                                   // 0050FA8B..8F (jb)
            result = k_in_progress_00745dbc;                                        // 0050F91A
        } else {
            animate(self, kotor_fastcall_004f0f90(self, nullptr));                  // 0050FA95..A1
            at<U>(self, 0x96c) = 0;                                                 // 0050FAA6
            kotor_fastcall_004ef450(self, nullptr);                                 // 0050FAB0
            recovered_combat_round_cleanup_004f7530(self, nullptr);                 // 0050FAB7
            result = k_complete_00745dc0;                                           // 0050FABC
        }
    }
    frame_LogStringDtor_005e5c20(model, nullptr);                                   // 0050FAC2..D1
    return result;
}
}  // extern "C"
