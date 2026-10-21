// claude_w1_chains_damage.cpp (lane worker w1-chains): CSWSCreature::SignalDamage 005B8050 behind the W1 port router's
// former trap case, with its whole server-side missing cascade, transcribed from the original swkotor.exe (read-only):
//   005B8050 CSWSCreature::SignalDamage(int ranged, ulong target)                 RET 8   kotor_fastcall_005b8050
//   005B5EA0 CSWSCreature::SendAttackSummaryFeedback(ulong target)                RET 4   kotor_fastcall_005b5ea0
//   004D2060 CSWSCombatAttackData::GetBaseDamage()                                RET 0   kotor_fastcall_004d2060
//   004EC9E0 CSWSCreature::BroadcastAttackSummaryToParty(CSWCCMessageData*)       RET 4   kotor_fastcall_004ec9e0
//   004ECAE0 CSWSCreature::BroadcastBreakdownMessage(ulong kind, CSWCCMessageData*) RET 8 kotor_fastcall_004ecae0
//   004ECBE0 / 004ECC20 / 004ECC60 / 004ECCA0 / 004ECCE0 / 004ECD20 CSWSCreature::Broadcast{Attack,Threat,Defense,
//            Damage,EffectApplication,Deflect}Breakdown(ulong target, CSWCCMessageData*)  RET 8 (kinds 14h..1Ah)
//   005B6F30 CSWSCreature::SignalRangedDamage(ulong target)                       RET 4   kotor_fastcall_005b6f30
//   005B4E30 CSWSCreature::ResolveProjectileMissImpact(Vector const*, ulong ms)   RET 8   kotor_fastcall_005b4e30
//   005B60F0 CSWSCreature::SetAttackSummaryFeedbackEffectData(int unused, ulong)  RET 8   kotor_fastcall_005b60f0
//   005B49C0 CSWSCreature::ResolveSafeProjectile(index, ms, sub_a, sub_b)         RET 10h kotor_fastcall_005b49c0
//   005B75D0 CSWSCreature::SignalMeleeDamage(ulong target, int count)             RET 8   kotor_fastcall_005b75d0
//   005036D0 CGameEffect::SetFloat(int index, float value)                        RET 8   kotor_fastcall_005036d0
// The client floaty-text chain called by SignalMeleeDamage (005EDEA0 CClientExoApp::AddFloatyText ...) lives in
// claude_w1_chains_floaty.cpp.
// Conventions (W5): thiscall -> extern "C" __fastcall(self, unused_edx, ...); x87 at 53-bit precision -> double
// intermediates, float exactly where the original stores a dword; live_operator_new / live_free only. Failed
// allocations are not guarded where the original is not (it then calls through the null pointer).
#include "842d6b1618-w5_t3.hpp"

namespace w2 { std::uint32_t ftol2_006fae8c(double value); }                                  // __ftol2 low dword [w2_ftol_native]
namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void* server, void*); } // CServerExoApp::GetPartyTable [0575]

namespace w1c_damage {
using namespace w5;

extern "C" {
// ---------------- imports: existing providers (c14 candidate map; owner object in brackets) ----------------
extern U recovered_effect_rank_scratch_008327e0;                                                 // .bss 008327E0 fistp scratch [0886]
U __fastcall kotor_fastcall_004aea40(void* server, void*);                                      // CServerExoApp::GetPlayerCreatureId [0595]
void* __fastcall recovered_combat_round_attack_004d2b50(void* round, void*, U index);           // CSWSCombatRound::GetAttack [W1 b7]
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);                       // GetServerAIMaster [creature_destructor_r2]
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);                     // GetSWSMessage [creature_destructor_r2]
U __fastcall recovered_cc_message_00568570(void* message, void*, U player, U type, void* data, U a4); // SendServerToPlayerCCMessage
void* __fastcall recovered_message_data_construct_004d69f0(void* data, void*);                   // CSWCCMessageData() [W1 b7]
void __fastcall recovered_message_data_destroy_004d6a40(void* data, void*);                      // ~CSWCCMessageData [W1 b7]
void __fastcall recovered_event_integer_004d76b0(void* data, void*, U index, U value);           // CSWCCMessageData::SetInteger [1017]
void* __fastcall combat_reuse_faction_00513fc0(void* creature, void*);                           // CSWSCreature::GetFaction [W1 b7]
I __fastcall combat_reuse_members_005be780(void* faction, void*, U** list);                      // CSWSFaction::GetMemberList [W1 b7]
void* __fastcall kotor_fastcall_00561080(void* player, void*);                                   // CSWSPlayer::GetGameObject [0595]
U __fastcall recovered_party_member_00563740(void* table, void*, U id);                          // CSWPartyTable::GetIsMember
void* __fastcall recovered_effect_construct_00503e40(void* effect, void*, U flag);               // CGameEffect::CGameEffect(int) [0886]
void __fastcall recovered_effect_creator_00503a00(void* effect, void*, U id);                    // CGameEffect::SetCreator [0886]
void __fastcall recovered_effect_integer_005036a0(void* effect, void*, U index, U value);        // CGameEffect::SetInteger [0886]
U __fastcall recovered_effect_integer_00503690(void* effect, void*, U index);                    // CGameEffect::GetInteger [W1 b7]
void __fastcall recovered_effect_resize_00503630(void* effect, void*, unsigned count);           // CGameEffect::SetNumIntegers [1091]
void __fastcall recovered_effect_apply_004d14f0(void* object, void*, void* effect, U a, U b);    // CSWSObject::ApplyEffect [0886]
U __fastcall recovered_stats_effect_query_005a6960(void* stats, void*, U kind, void* other);      // GetEffectImmunity [1094]
U __fastcall recovered_stats_has_feat_005a6630(void* stats, void*, U feat);                      // CSWSCreatureStats::HasFeat [0963]
U __fastcall kotor_fastcall_004ed5e0(void* creature, void*);                                     // CSWSCreature::GetDamageFlags [damagestats]
Vector* __fastcall kotor_fastcall_005b4aa0(void* self, void*, Vector* out, void* target);        // ResolveProjectileMissLocation [anim]
void* __fastcall kotor_fastcall_004d4560(void* data, void*, void* out);                          // CSWSCombatAttackData::GetTopSubAttack [route_bodies]
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void* item, void*);                      // CSWItem::GetBaseItem [0820]
void __fastcall kotor_m15e_slot090_void0_004015a0(void*, void*);                                 // 004015A0 void_return (sub-attack dtor) [0832]
U __fastcall kotor_m15_client_get_player_creature_id_005ed550(void* client, void*);              // CClientExoApp::GetPlayerCreatureId [0811]
void __fastcall kotor_fastcall_004cdb00(void* self, void*, U a0, U a1, const Vector* from, const Vector* to,
    U a4, U a5, U a6, U a7, U a8, U a9);                                                         // BroadcastSafeProjectile [route_bodies]

// ---------------- imports: claude_w1_chains_floaty.cpp ----------------
void __fastcall kotor_fastcall_005edea0(void* client, void*, U id, U type, I value);             // CClientExoApp::AddFloatyText

// ---------------- exports of this file ----------------
void __fastcall kotor_fastcall_005b8050(void* self, void*, U ranged, U target);
void __fastcall kotor_fastcall_005b5ea0(void* self, void*, U target);
I __fastcall kotor_fastcall_004d2060(void* attack, void*);
void __fastcall kotor_fastcall_004ec9e0(void* self, void*, void* data);
void __fastcall kotor_fastcall_004ecae0(void* self, void*, U kind, void* data);
void __fastcall kotor_fastcall_004ecbe0(void* self, void*, U target, void* data);
void __fastcall kotor_fastcall_004ecc20(void* self, void*, U target, void* data);
void __fastcall kotor_fastcall_004ecc60(void* self, void*, U target, void* data);
void __fastcall kotor_fastcall_004ecca0(void* self, void*, U target, void* data);
void __fastcall kotor_fastcall_004ecce0(void* self, void*, U target, void* data);
void __fastcall kotor_fastcall_004ecd20(void* self, void*, U target, void* data);
void __fastcall kotor_fastcall_005b6f30(void* self, void*, U target);
void __fastcall kotor_fastcall_005b4e30(void* self, void*, const Vector* location, U ms);
void __fastcall kotor_fastcall_005b60f0(void* self, void*, U unused, U target);
void __fastcall kotor_fastcall_005b49c0(void* self, void*, U index, U ms, U sub_a, U sub_b);
void __fastcall kotor_fastcall_005b75d0(void* self, void*, U target, I count);
void __fastcall kotor_fastcall_005036d0(void* effect, void*, U index, U value_bits);
}

namespace {
// .rdata constants (proven values read from the original)
constexpr U k_event_attack_007441f0 = 0xf;              // [007441F0]
constexpr U k_event_item_007441c8 = 5;                  // [007441C8] (apply effect event)
constexpr U k_event_on_hit_0074420c = 0x16;             // [0074420C]
constexpr U k_event_self_00744200 = 0x13;               // [00744200]
constexpr U k_event_damage_00744220 = 0x1b;             // [00744220]
constexpr U k_effect_damage_type_0074669c = 8;          // [0074669C] (CGameEffect type compared with word [effect+8])
constexpr U16 k_effect_damage_00746714 = 0x26;          // word [00746714]
constexpr U16 k_effect_visual_007466f4 = 0x1e;          // word [007466F4]
constexpr U16 k_duration_instant_007473b8 = 0;          // word [007473B8]
constexpr U8 k_immunity_00746934 = 0x10;               // byte [00746934]
constexpr U8 k_immunity_0074690c = 4;                   // byte [0074690C]
constexpr U8 k_immunity_0074692c = 0xc;                 // byte [0074692C]
constexpr U8 k_immunity_00746914 = 6;                   // byte [00746914]
constexpr U8 k_immunity_00746930 = 0xd;                 // byte [00746930]
const float k_party_range_007455bc = 900.0f;            // .rdata 007455BC (squared 30 m)
const float k_ms_per_unit_0074b2ec = 23.809524536132812f; // .rdata 0074B2EC (0x41BE79E8)
const float k_log_scale_00745ff8 = 3.321928024291992f;  // .rdata 00745FF8 (0x40549A78)
const char k_multiplier_00747028[] = "MULTIPLIER";      // .rdata 00747028

inline void* message_data_new() {                       // operator new(34h) + CSWCCMessageData(); null when it fails
    void* memory = live_operator_new(0x34);
    return memory != nullptr ? recovered_message_data_construct_004d69f0(memory, nullptr) : nullptr;
}
inline void message_data_delete(void* data) {          // if (p) { p->~CSWCCMessageData(); free(p); }
    if (data != nullptr) {
        recovered_message_data_destroy_004d6a40(data, nullptr);
        live_free(data);
    }
}
inline void set_int(void* data, U index, U value) { recovered_event_integer_004d76b0(data, nullptr, index, value); }
inline U zx8(void* p, U off) { return at<U8>(p, off); }
inline U sx8(void* p, U off) { return static_cast<U>(static_cast<I>(at<signed char>(p, off))); }
inline U zx16(void* p, U off) { return at<U16>(p, off); }
inline U sx16(void* p, U off) { return static_cast<U>(static_cast<I>(at<std::int16_t>(p, off))); }
inline void* current_attack(void* creature) {           // GetAttack(round, round->current)  (+9C8 round, +96C index)
    void* round = at<void*>(creature, 0x9c8);
    return recovered_combat_round_attack_004d2b50(round, nullptr, at<U8>(round, 0x96c));
}
inline void* new_effect() {                             // operator new(8Ch) + CGameEffect(1); null when it fails
    void* memory = live_operator_new(0x8c);
    return memory != nullptr ? recovered_effect_construct_00503e40(memory, nullptr, 1) : nullptr;
}
inline void set_effect_type(void* effect, U16 type) {   // word [e+8] = type; [e+A] = ([e+A] & FFF8h) | instant
    const U16 flags = at<U16>(effect, 0xa);
    at<U16>(effect, 8) = type;
    at<U16>(effect, 0xa) = static_cast<U16>((flags & 0xfff8u) | k_duration_instant_007473b8);
}
inline void add_event(void* ai, U ms, U caller, U target, U type, void* data) {
    recovered_event_enqueue_004b08d0(ai, nullptr, 0, ms, caller, target, type, data);
}
// damage-type index = frndint((float)(log10(flags) * 3.3219...)) via the global fistp scratch 008327E0.
// Exact x87 sequence: fldlg2; fild; fyl2x; fmul dword; fstp dword; fld; frndint; fistp dword [008327E0].
U damage_index(U flags) {
    U value = flags;
    float scaled;
    const float scale = k_log_scale_00745ff8;
    __asm {
        fldlg2
        fild dword ptr [value]
        fyl2x
        fmul dword ptr [scale]
        fstp dword ptr [scaled]
        fld dword ptr [scaled]
        frndint
        fistp dword ptr [recovered_effect_rank_scratch_008327e0]
    }
    return recovered_effect_rank_scratch_008327e0;
}
// CSWSCreature virtual slots used by the deflect path (thiscall).
using DamageReduceFn = U(__thiscall*)(void* self, void* attacker, I damage, U flags, U a3, U a4, U a5);   // [vtbl+A8h] RET 18h
using DamageResistFn = U(__thiscall*)(void* self, void* attacker, U damage, U flags, U a3, U a4);         // [vtbl+ACh] RET 14h
template <class F> inline F vslot(void* object, U offset) { return reinterpret_cast<F>(at<void*>(at<void*>(object, 0), offset)); }

// 004EC9E0 / 004ECAE0 shared body: each faction member's player whose creature is in the same area within 30 m
// receives SendServerToPlayerCCMessage(player id, kind, data, 0). The member list is not freed (original).
void broadcast_to_party(void* self, U kind, void* data) {
    void* faction = combat_reuse_faction_00513fc0(self, nullptr);                               // 004EC9E7
    if (faction == nullptr) return;                                                             // 004EC9EE..9F0
    U* list = nullptr;                                                                          // 004EC9FD
    const I count = combat_reuse_members_005be780(faction, nullptr, &list);                     // 004EC9F6..A01
    for (I i = 0; i < count; ++i) {                                                             // 004ECA06..A0C, ECABA..AC1
        void* player = kotor_fastcall_004ae910(server_app(), nullptr, list[i]);                 // 004ECA20..A31
        if (player == nullptr) continue;                                                        // 004ECA38..A3A
        void* object = kotor_fastcall_00561080(player, nullptr);                                // 004ECA3C..A3E
        if (object == nullptr) continue;                                                        // 004ECA43..A45
        if (at<U>(self, 0x8c) != at<U>(object, 0x8c)) continue;                                 // 004ECA47..A53
        const double dx = static_cast<double>(at<float>(object, 0x90)) - at<float>(self, 0x90); // 004ECA55..A5B
        const double dy = static_cast<double>(at<float>(object, 0x94)) - at<float>(self, 0x94); // 004ECA61..A67
        const double dz = static_cast<double>(at<float>(object, 0x98)) - at<float>(self, 0x98); // 004ECA6D..A73
        const double d2 = (dz * dz + dy * dy) + dx * dx;                                        // 004ECA79..A87
        if (!(d2 <= k_party_range_007455bc)) continue;                                          // 004ECA89..A9A (test ah,41h / jp)
        const U player_id = at<U>(player, 4);                                                   // 004ECA9C
        void* message = recovered_server_sws_message_004aedb0(server_app(), nullptr);           // 004ECA9F..AB3
        (void)recovered_cc_message_00568570(message, nullptr, player_id, kind, data, 0);        // 004ECAA8..AB5
    }
}
// 004ECBE0..004ECD20 shape: BroadcastBreakdownMessage(kind) on self, then on the target creature if it exists.
void broadcast_breakdown(void* self, U kind, U target, void* data) {
    kotor_fastcall_004ecae0(self, nullptr, kind, data);                                         // +00..+08
    void* creature = kotor_fastcall_004ae770(server_app(), nullptr, target);                     // +0D..+1A
    if (creature != nullptr) kotor_fastcall_004ecae0(creature, nullptr, kind, data);             // +1F..+28
}
}  // namespace

extern "C" {
// =========================================================================================================
// 004D2060 CSWSCombatAttackData::GetBaseDamage: movsx eax, word [ecx+38h]. RET 0.
I __fastcall kotor_fastcall_004d2060(void* attack, void*) {
    return static_cast<I>(at<std::int16_t>(attack, 0x38));                                      // 004D2060..2064
}

// 005036D0 CGameEffect::SetFloat(int index, float value): fld dword [esp+8]; fstp dword [ecx+index*4+38h]. RET 8.
// The value travels as its dword bits; the x87 load/store quiets a signalling NaN.
void __fastcall kotor_fastcall_005036d0(void* effect, void*, U index, U value_bits) {
    U v = value_bits;
    if ((v & 0x7f800000u) == 0x7f800000u && (v & 0x007fffffu) != 0) v |= 0x00400000u;
    at<U>(effect, index * 4 + 0x38) = v;                                                        // 005036D0..36DC
}

// 004EC9E0 CSWSCreature::BroadcastAttackSummaryToParty(CSWCCMessageData*): kind 12h. RET 4.
void __fastcall kotor_fastcall_004ec9e0(void* self, void*, void* data) {
    broadcast_to_party(self, 0x12, data);                                                       // 004EC9E0..4ECACE
}

// 004ECAE0 CSWSCreature::BroadcastBreakdownMessage(ulong kind, CSWCCMessageData*): RET 8.
void __fastcall kotor_fastcall_004ecae0(void* self, void*, U kind, void* data) {
    broadcast_to_party(self, kind, data);                                                       // 004ECAE0..4ECBD5
}

void __fastcall kotor_fastcall_004ecbe0(void* self, void*, U target, void* data) { broadcast_breakdown(self, 0x14, target, data); } // AttackBreakdown
void __fastcall kotor_fastcall_004ecc20(void* self, void*, U target, void* data) { broadcast_breakdown(self, 0x15, target, data); } // ThreatBreakdown
void __fastcall kotor_fastcall_004ecc60(void* self, void*, U target, void* data) { broadcast_breakdown(self, 0x16, target, data); } // DefenseBreakdown
void __fastcall kotor_fastcall_004ecca0(void* self, void*, U target, void* data) { broadcast_breakdown(self, 0x17, target, data); } // DamageBreakdown
void __fastcall kotor_fastcall_004ecce0(void* self, void*, U target, void* data) { broadcast_breakdown(self, 0x19, target, data); } // EffectApplicationBreakdown
void __fastcall kotor_fastcall_004ecd20(void* self, void*, U target, void* data) { broadcast_breakdown(self, 0x1a, target, data); } // DeflectBreakdown

// =========================================================================================================
// 005B5EA0 CSWSCreature::SendAttackSummaryFeedback(ulong target): RET 4.
void __fastcall kotor_fastcall_005b5ea0(void* self, void*, U target) {
    void* round = at<void*>(self, 0x9c8);                                                       // 005B5EBE
    void* data = message_data_new();                                                            // 005B5EC4..5EE7
    void* victim = kotor_fastcall_004ae770(server_app(), nullptr, target);                       // 005B5EE9..5F05
    if (round != nullptr) {                                                                     // 005B5F03..5F07
        void* attack = recovered_combat_round_attack_004d2b50(round, nullptr, at<U8>(round, 0x96c)); // 005B5F0D..5F1C
        I base = kotor_fastcall_004d2060(attack, nullptr);                                     // 005B5F1E..5F27
        if (victim != nullptr) {                                                                // 005B5F25..5F2B
            void* table = lane_config_required::GetPartyTable_004aee70(server_app(), nullptr);   // 005B5F31..5F3A
            const bool member = table != nullptr && recovered_party_member_00563740(table, nullptr, target) != 0; // 005B5F3F..5F51
            if (member || at<U>(victim, 0x9d4) != 0) {                                          // 005B5F53..5F5B
                void* options = kotor_fastcall_005ed700(client_app(), nullptr);                 // 005B5F5D..5F65
                const U row = at<U8>(options, 4);                                               // 005B5F6A
                alignas(4) unsigned char column[8];
                frame_LogStringCtor_005e5a90(column, nullptr, k_multiplier_00747028);           // 005B5F6E..5F77
                float multiplier;                                                               // [esp+18h]
                void* difficulty = at<void*>(at<void*>(kotor_cell_rules_007a3a28, 0xb8), 0x7c); // 005B5F7C..5F88
                const U ok = c2da_current_get_float_00413430(difficulty, nullptr, row, column, &multiplier); // 005B5F8B..5FA0
                frame_LogStringDtor_005e5c20(column, nullptr);                                  // 005B5FA5..5FB3
                if (ok != 0)                                                                    // 005B5FB8..5FBA
                    base = static_cast<I>(w2::ftol2_006fae8c(static_cast<double>(base) * multiplier)); // 005B5FBC..5FC9
            }
        }
        set_int(data, 0, zx8(attack, 0x5c));                                                    // 005B5FD1..5FDA
        set_int(data, 1, zx16(attack, 0x64));                                                   // 005B5FDF..5FE8
        set_int(data, 2, sx8(attack, 0xc2) + zx8(attack, 0xc1));                                // 005B5FED..6002
        set_int(data, 3, sx8(attack, 0xf4));                                                    // 005B6007..6013
        set_int(data, 4, static_cast<U>(base));                                                 // 005B6018..6021
        set_int(data, 5, at<U>(attack, 0xec));                                                  // 005B6026..6031
        set_int(data, 6, at<U>(attack, 0x44));                                                  // 005B6036..603E
        set_int(data, 7, at<U>(attack, 0x4c));                                                  // 005B6043..604B
        set_int(data, 8, zx8(attack, 0x12d));                                                   // 005B6050..605C
        set_int(data, 9, at<U>(attack, 0x138));                                                 // 005B6061..606C
        set_int(data, 10, at<U>(attack, 0xd8));                                                 // 005B6071..607C
        set_int(data, 11, at<U>(attack, 0xdc));                                                 // 005B6081..608C
        recovered_event_object_004d7780(data, nullptr, 0, at<U>(self, 4));                      // 005B6091..6099
        recovered_event_object_004d7780(data, nullptr, 1, target);                              // 005B609E..60A7
        kotor_fastcall_004ec9e0(self, nullptr, data);                                           // 005B60AC..60AF
        if (victim != nullptr) kotor_fastcall_004ec9e0(victim, nullptr, data);                  // 005B60B4..60BB
    }
    message_data_delete(data);                                                                  // 005B60C0..60D1
}

// =========================================================================================================
// 005B4E30 CSWSCreature::ResolveProjectileMissImpact(Vector const* location, ulong ms): visual effect 0FC0h at the
// location, delivered to the creature's area by an apply-effect event. RET 8.
void __fastcall kotor_fastcall_005b4e30(void* self, void*, const Vector* location, U ms) {
    void* effect = new_effect();                                                                // 005B4E49..4E75
    set_effect_type(effect, k_effect_visual_007466f4);                                          // 005B4E77..4E9C
    recovered_effect_creator_00503a00(effect, nullptr, at<U>(self, 4));                         // 005B4EA0..4EA6
    recovered_effect_integer_005036a0(effect, nullptr, 0, 0xfc0);                               // 005B4EAB..4EB4
    kotor_fastcall_005036d0(effect, nullptr, 0, bits(location->x));                             // 005B4EB9..4EC4
    kotor_fastcall_005036d0(effect, nullptr, 1, bits(location->y));                             // 005B4EC9..4ED1
    kotor_fastcall_005036d0(effect, nullptr, 2, bits(location->z));                             // 005B4ED6..4EDE
    void* area = recovered_object_area_004cb120(self, nullptr);                                 // 005B4EE3..4EE5
    const U area_id = at<U>(area, 0x11c + 4);                                                   // 005B4EFD..4F02
    void* ai = recovered_server_ai_master_004aed80(server_app(), nullptr);                       // 005B4EF3..4F0F
    add_event(ai, ms, at<U>(self, 4), area_id, k_event_item_007441c8, effect);                  // 005B4EEA..4F16
}

// 005B60F0 CSWSCreature::SetAttackSummaryFeedbackEffectData(int unused, ulong target): marks attack +138h when the
// target is immune to the attack's feedback effect (by attack +12Dh kind) or has the matching feats. RET 8.
void __fastcall kotor_fastcall_005b60f0(void* self, void*, U, U target) {
    void* victim = kotor_fastcall_004ae770(server_app(), nullptr, target);                       // 005B60F0..6106
    if (victim == nullptr) return;                                                              // 005B6108..610A
    void* attack = current_attack(self);                                                        // 005B6110..6124
    void* stats = at<void*>(victim, 0xa74);
    auto immune = [&](U8 kind) { return recovered_stats_effect_query_005a6960(stats, nullptr, kind, self) != 0; };
    auto feat = [&](U f) { return recovered_stats_has_feat_005a6630(stats, nullptr, f) != 0; };
    bool hit = false;
    switch (at<U8>(attack, 0x12d)) {                                                            // 005B6126..6137 (dec; ja; table 005B61F8)
    case 1: hit = immune(k_immunity_00746934); break;                                          // 005B6185..6199 -> 61E3
    case 2: hit = immune(k_immunity_0074690c) || feat(0x62) || feat(0x63) || feat(0x64); break;  // 005B619B..61E3
    case 3: return;                                                                             // table -> 005B61F1
    case 4: hit = immune(k_immunity_0074692c) || feat(0x63) || feat(0x64); break;                // 005B616E..6183 -> 61C1
    case 5: hit = immune(k_immunity_00746914) || feat(0x64); break;                              // 005B6158..616C -> 61D2
    case 6: hit = immune(k_immunity_00746930); break;                                           // 005B613E..6153 -> 61E3
    default: return;                                                                            // 005B6131 ja 005B61F1
    }
    if (hit) at<U>(attack, 0x138) = 1;                                                          // 005B61E3..61E7
}

// 005B49C0 CSWSCreature::ResolveSafeProjectile(index, ms, sub_a, sub_b): RET 10h. The byte locals live in the
// argument slots, so the passed dwords keep the slots' upper bytes (original).
void __fastcall kotor_fastcall_005b49c0(void* self, void*, U index, U ms, U sub_a, U sub_b) {
    const U slot = sub_b == 0 ? 0x10u : (sub_b == 1 ? 0x20u : 0u);                              // 005B49C2..49DB
    void* item = recovered_equipment_item_005a4c20(at<void*>(self, 0xa2c), nullptr, slot);      // 005B49E0..49E7
    if (item == nullptr) return;                                                                // 005B49EE..49F0
    void* attack = recovered_combat_round_attack_004d2b50(at<void*>(self, 0x9c8), nullptr, index); // 005B49F6..4A02
    at<U>(attack, 0x74) = at<U>(item, 0x14);                                                    // 005B4A07..4A12
    U arg_index = index;                                                                        // [esp+14h] = arg1 slot
    arg_index = (arg_index & 0xffffff00u) | (sub_a != 0 ? at<U8>(attack, 0x5c) : 4u);           // 005B4A0C..4A20
    (void)kotor_fastcall_004d2060(attack, nullptr);                                             // 005B4A25..4A27 (unused)
    void* base = kotor_m15b_item_get_base_item_005b4790(item, nullptr);                         // 005B4A2C..4A2E
    const U arg_sub_a = (sub_a & 0xffffff00u) | at<U8>(base, 0x8e);                             // 005B4A33..4A39 [esp+1Ch]
    const U kind = at<U16>(attack, 0x64);                                                       // 005B4A3D
    const U flag = (kind == 0x12 || kind == 0x1d || kind == 0x52) ? 1u : 0u;                    // 005B4A41..4A55
    const U arg_sub_b = (sub_b & 0xffffff00u) | flag;                                           // [esp+20h]
    kotor_fastcall_004cdb00(self, nullptr, at<U>(self, 4), at<U>(attack, 0xc),
                            reinterpret_cast<const Vector*>(static_cast<unsigned char*>(self) + 0x90),
                            reinterpret_cast<const Vector*>(static_cast<unsigned char*>(attack) + 0x68),
                            ms, arg_index, sub_b, arg_sub_a, arg_sub_b, at<U>(attack, 0x18)); // 005B4A5A..4A88
}

// =========================================================================================================
// 005B6F30 CSWSCreature::SignalRangedDamage(ulong target): RET 4.
void __fastcall kotor_fastcall_005b6f30(void* self, void*, U target) {
    void* round = at<void*>(self, 0x9c8);                                                       // 005B6F52
    const U index = at<U8>(round, 0x96c);                                                       // 005B6F59 [esp+38h]
    void* attack = recovered_combat_round_attack_004d2b50(round, nullptr, index);              // 005B6F60..6F6A
    const U ms = at<U16>(attack, 0x10);                                                         // 005B6F6C ebx
    alignas(8) unsigned char sub[0xd0];                                                         // [esp+48h] CSWSCombatSubAttack
    kotor_fastcall_004d4560(attack, nullptr, sub);                                              // 005B6F70..6F77
    const U sub_a = at<U>(sub, 0x14);                                                           // 005B6F7C [esp+24h]
    const U sub_b = at<U>(sub, 0x2c);                                                           // 005B6F80 [esp+34h]
    U state = at<U>(sub, 0x10);                                                                 // 005B6F84 [esp+18h]
    void* ai = recovered_server_ai_master_004aed80(server_app(), nullptr);                       // 005B6F8C..6FB5
    if (state != 0)                                                                             // 005B6FAF..6FB9
        add_event(ai, index, at<U>(self, 4), target, k_event_attack_007441f0, attack);          // 005B6FBB..6FD8
    for (I i = 0; i < at<I>(attack, 0xa4); ++i)                                                 // 005B6FDD..7024
        add_event(ai, ms, at<U>(self, 4), target, k_event_on_hit_0074420c,
                  at<void*>(at<void*>(attack, 0xa0), static_cast<U>(i) * 4));                   // 005B6FF0..7016
    at<U>(attack, 0xa4) = 0;                                                                    // 005B702E
    if (sub_a == 0) {                                                                           // 005B702C..7034
        kotor_fastcall_005b4e30(self, nullptr,
            reinterpret_cast<const Vector*>(static_cast<unsigned char*>(attack) + 0x68), ms);   // 005B739C..73A5
    } else {
        state = 0;                                                                              // 005B7041
        void* victim = kotor_fastcall_004ae770(server_app(), nullptr, target);                   // 005B703A..7053 [esp+14h]
        const U8 result = at<U8>(attack, 0x5c);                                                 // 005B7053
        const U r = result;
        bool effect_path = true;
        if (r == 0xffffffffu) {                                                                 // 005B705F..7066 (movzx: never)
            if (at<U>(self, 0x558) == 2) effect_path = false;                                   // 005B7068..706F
        } else if (r >= 8 && r <= 10) {                                                         // 005B70A2..70AA
            effect_path = false;
        }
        if (!effect_path) {
            if (victim == nullptr) effect_path = true;                                          // 005B70AC..70AE
            else if (result != 8 && result != 10) { state = 1; effect_path = true; }            // 005B70B0..70C2
        }
        if (!effect_path) {
            // 005B70C4..7142: the projectile misses: impact at the miss location after its flight time.
            Vector location;                                                                    // [esp+28h]
            kotor_fastcall_005b4aa0(victim, nullptr, &location, self);                          // 005B70C4..70CC
            Vector delta;                                                                       // [esp+3Ch]
            delta.z = static_cast<float>(static_cast<double>(location.z) - at<float>(victim, 0x98)); // 005B70D1..70D5, 70FC
            delta.y = static_cast<float>(static_cast<double>(location.y) - at<float>(victim, 0x94)); // 005B70DF..70E4, 70F8
            delta.x = static_cast<float>(static_cast<double>(location.x) - at<float>(victim, 0x90)); // 005B70EA..70EE, 70F4
            const U flight = w2::ftol2_006fae8c(kotor_cdecl_004ab100(&delta) * k_ms_per_unit_0074b2ec); // 005B70E3..710E
            state = flight;                                                                     // 005B711B [esp+18h]
            kotor_fastcall_005b4e30(victim, nullptr, &location, flight);                        // 005B7113..711F
            copy_vector(static_cast<unsigned char*>(attack) + 0x68, &location);                 // 005B7124..713C
            at<U>(attack, 0x18) = state;                                                        // 005B7138..713F
        } else {
            // 005B7071..72CD: damage effect 26h carrying the attack's damage words.
            void* effect = new_effect();                                                        // 005B7071..709B / 7147
            set_effect_type(effect, k_effect_damage_00746714);                                  // 005B7149..716E
            recovered_effect_creator_00503a00(effect, nullptr, at<U>(self, 4));                 // 005B7172..717C
            recovered_effect_resize_00503630(effect, nullptr, 0x16);                            // 005B7181..7185
            for (U k = 0; k < 0xf; ++k)                                                         // 005B718A..71B2
                recovered_effect_integer_005036a0(effect, nullptr, k, sx16(attack, 0x1c + 2 * k)); // 005B7193..719E
            recovered_effect_integer_005036a0(effect, nullptr, 0x10, ms);                       // 005B71B4..71B9
            recovered_effect_integer_005036a0(effect, nullptr, 0x13, 1);                        // 005B71BE..71C4
            recovered_effect_integer_005036a0(effect, nullptr, 0x14, 1);                        // 005B71C9..71CF
            recovered_effect_integer_005036a0(effect, nullptr, 0x15, 1);                        // 005B71D4..71DA
            if (state == 0) {                                                                   // 005B71DF..71E5
                add_event(ai, ms, at<U>(self, 4), target, k_event_item_007441c8, effect);        // 005B72A9..72C8
            } else if (victim != nullptr) {                                                     // 005B71EB..71F1 (else effect leaks)
                // 005B71F7..72C8: deflected back at the attacker: damage reduced/resisted on self.
                const I base = static_cast<I>(at<std::int16_t>(attack, 0x38));                  // 005B71F7
                const U f1 = kotor_fastcall_004ed5e0(self, nullptr);                            // 005B720B
                const U reduced = vslot<DamageReduceFn>(self, 0xa8)(self, self, base, f1, 0, 1, 1); // 005B7201..721B
                const U f2 = kotor_fastcall_004ed5e0(self, nullptr);                            // 005B722F
                const U damage = vslot<DamageResistFn>(self, 0xac)(self, self, reduced, f2, 0, 1); // 005B7227..723F
                const U f3 = kotor_fastcall_004ed5e0(self, nullptr);                            // 005B7245..724B
                recovered_effect_integer_005036a0(effect, nullptr, damage_index(f3), damage);   // 005B7250..727C
                recovered_effect_integer_005036a0(effect, nullptr, 0xe, damage);                // 005B7281..7286
                add_event(ai, ms + ms, target, at<U>(self, 4), k_event_item_007441c8, effect);  // 005B728B..72C8
            }
        }
        // 005B72D1..739A: queued damage effects, then self events.
        for (I i = 0; i < at<I>(attack, 0x8c); ++i) {                                           // 005B72D1..734F
            add_event(ai, ms, at<U>(self, 4), target, k_event_item_007441c8,
                      at<void*>(at<void*>(attack, 0x88), static_cast<U>(i) * 4));               // 005B72E0..7306
            void* queued = at<void*>(at<void*>(attack, 0x88), static_cast<U>(i) * 4);          // 005B730B..7311
            if (at<U16>(queued, 8) == k_effect_damage_type_0074669c) {                          // 005B7314..731E
                const U value = recovered_effect_integer_00503690(queued, nullptr, 0);          // 005B7320..732A
                kotor_fastcall_005b60f0(self, nullptr, value, target);                          // 005B732F..7334
            }
            at<void*>(at<void*>(attack, 0x88), static_cast<U>(i) * 4) = nullptr;               // 005B7339..733F
        }
        at<U>(attack, 0x8c) = 0;                                                                // 005B7353
        for (I i = 0; i < at<I>(attack, 0x98); ++i)                                             // 005B7359..738E
            add_event(ai, ms, at<U>(self, 4), at<U>(self, 4), k_event_self_00744200,
                      at<void*>(at<void*>(attack, 0x94), static_cast<U>(i) * 4));               // 005B7361..7380
        at<U>(attack, 0x98) = 0;                                                                // 005B7390
    }
    kotor_fastcall_005b49c0(self, nullptr, index, ms, sub_a, sub_b);                            // 005B73AA..73BE
    kotor_m15e_slot090_void0_004015a0(sub, nullptr);                                            // 005B73C3..73D2 (sub-attack dtor)
}

// =========================================================================================================
// 005B75D0 CSWSCreature::SignalMeleeDamage(ulong target, int count): RET 8.
void __fastcall kotor_fastcall_005b75d0(void* self, void*, U target, I count) {
    void* ai = recovered_server_ai_master_004aed80(server_app(), nullptr);                       // 005B75E8..7601 [esp+8]
    void* round = at<void*>(self, 0x9c8);                                                       // 005B75F9 [esp+18h]
    U ms = 0;                                                                                   // [esp+10h]
    for (I i = 0; i < count; ++i) {                                                             // 005B7605..7615, 76C2..76CD
        void* attack = recovered_combat_round_attack_004d2b50(round, nullptr, static_cast<U>(i)); // 005B7620..762A
        add_event(ai, static_cast<U>(i), at<U>(self, 4), target, k_event_attack_007441f0, attack); // 005B762A..7645
        (void)recovered_combat_round_attack_004d2b50(at<void*>(self, 0x9c8), nullptr, static_cast<U>(i)); // 005B764A..7651 (unused)
        const U r = at<U8>(attack, 0x5c);                                                       // 005B7656
        bool hit;
        if (r == 0xffffffffu) hit = at<U>(self, 0x558) == 0;                                    // 005B765A..7669 (movzx: never)
        else hit = r > 0 && r <= 3;                                                             // 005B76E8..76EF
        if (!hit) {
            // 005B766F..76AD: the player's own miss shows floaty text 2 over the target.
            void* client = client_app();                                                        // 005B766F..7675
            const U own = kotor_fastcall_005eda50(client, nullptr, at<U>(self, 4));             // 005B7678..7682
            if (own == kotor_m15_client_get_player_creature_id_005ed550(client, nullptr)) {     // 005B7687..7698
                const U id = kotor_fastcall_005eda50(client, nullptr, target);                  // 005B769A..76A5
                kotor_fastcall_005edea0(client, nullptr, id, 2, 0);                             // 005B769E..76AD
            }
        } else {
            // 005B76F5..78DB: damage effect 26h, its on-hit items, queued effects and self events.
            void* effect = new_effect();                                                        // 005B76F5..771B
            set_effect_type(effect, k_effect_damage_00746714);                                  // 005B771D..7742
            recovered_effect_creator_00503a00(effect, nullptr, at<U>(self, 4));                 // 005B7746..774C
            recovered_effect_resize_00503630(effect, nullptr, 0x16);                            // 005B7751..7755
            for (U k = 0; k < 0xf; ++k)                                                         // 005B775A..7780
                recovered_effect_integer_005036a0(effect, nullptr, k, sx16(attack, 0x1c + 2 * k)); // 005B7761..776C
            recovered_effect_integer_005036a0(effect, nullptr, 0xf, 1);                         // 005B7782..7788
            recovered_effect_integer_005036a0(effect, nullptr, 0x10, zx16(attack, 8) >> 1);     // 005B778D..7798
            recovered_effect_integer_005036a0(effect, nullptr, 0x13, 1);                        // 005B779D..77A3
            recovered_effect_integer_005036a0(effect, nullptr, 0x15, 1);                        // 005B77A8..77AE
            add_event(ai, ms, at<U>(self, 4), target, k_event_item_007441c8, effect);            // 005B77B3..77CE
            for (I k = 0; k < at<I>(attack, 0xa4); ++k)                                         // 005B77D3..780F
                add_event(ai, ms, at<U>(self, 4), target, k_event_on_hit_0074420c,
                          at<void*>(at<void*>(attack, 0xa0), static_cast<U>(k) * 4));           // 005B77E0..7801
            at<U>(attack, 0xa4) = 0;                                                            // 005B7813
            for (I k = 0; k < at<I>(attack, 0x8c); ++k) {                                       // 005B7819..788B
                void* victim = kotor_fastcall_004ae770(server_app(), nullptr, target);           // 005B7821..7834
                void* queued = at<void*>(at<void*>(attack, 0x88), static_cast<U>(k) * 4);
                if (victim != nullptr)
                    recovered_effect_apply_004d14f0(victim, nullptr, queued, 0, 0);             // 005B7838..7848
                else
                    add_event(ai, ms, at<U>(self, 4), target, k_event_item_007441c8, queued);    // 005B784F..7870
                at<void*>(at<void*>(attack, 0x88), static_cast<U>(k) * 4) = nullptr;           // 005B7875..787B
            }
            at<U>(attack, 0x8c) = 0;                                                            // 005B788F
            for (I k = 0; k < at<I>(attack, 0x98); ++k)                                         // 005B7895..78CB
                add_event(ai, ms, at<U>(self, 4), at<U>(self, 4), k_event_self_00744200,
                          at<void*>(at<void*>(attack, 0x94), static_cast<U>(k) * 4));           // 005B78A0..78BD
            at<U>(attack, 0x98) = 0;                                                            // 005B78D1
        }
        ms += static_cast<U>(at<U16>(attack, 0x14) >> 1);                                       // 005B76B2..76C5
    }
}

// =========================================================================================================
// 005B8050 CSWSCreature::SignalDamage(int ranged, ulong target): RET 8.
void __fastcall kotor_fastcall_005b8050(void* self, void*, U ranged, U target) {
    void* attack = current_attack(self);                                                        // 005B8067..807E
    if (target == kotor_fastcall_004aea40(server_app(), nullptr) && at<U8>(attack, 0x5c) == 2) { // 005B807E..809A
        void* ai = recovered_server_ai_master_004aed80(server_app(), nullptr);                   // 005B80A9..80D8
        add_event(ai, ranged != 0 ? at<U16>(attack, 0x10) : 0u, at<U>(self, 4), target,
                  k_event_damage_00744220, reinterpret_cast<void*>(static_cast<std::uintptr_t>(0x14))); // 005B809C..80DF
    }
    if (at<U>(attack, 0x40) == 0 || at<U>(attack, 4) != 0) {                                    // 005B80E4..80F1
        kotor_fastcall_005b5ea0(self, nullptr, target);                                         // 005B80F7..80FA
        // 005B80FF..82A8: attack breakdown (kind 14h)
        void* data = message_data_new();                                                        // 005B80FF..811C
        set_int(data, 0, sx8(attack, 0xc2) + zx8(attack, 0xc1));                                // 005B811E..813B
        set_int(data, 1, zx8(attack, 0xc1));                                                    // 005B8140..814C
        set_int(data, 2, sx8(attack, 0xc2));
        set_int(data, 3, sx8(attack, 0xc3));
        set_int(data, 4, sx8(attack, 0xc4));
        set_int(data, 5, zx16(attack, 0x64));
        set_int(data, 6, sx8(attack, 0xc6));
        set_int(data, 7, at<U>(attack, 0xc8));
        set_int(data, 8, sx8(attack, 0xcd));
        set_int(data, 9, sx8(attack, 0xce));
        set_int(data, 10, zx8(attack, 0xcf));
        set_int(data, 11, sx8(attack, 0xd0));
        set_int(data, 12, zx8(attack, 0xd1));
        set_int(data, 13, sx8(attack, 0xd2));
        set_int(data, 14, sx8(attack, 0xd3));
        set_int(data, 15, sx8(attack, 0xd4));
        set_int(data, 16, sx8(attack, 0xd5));
        set_int(data, 17, zx8(attack, 0xc7));
        set_int(data, 18, sx8(attack, 0xcc));
        set_int(data, 19, at<U>(attack, 0xd8));
        set_int(data, 20, at<U>(attack, 0xdc));                                                 // ..005B8289
        kotor_fastcall_004ecbe0(self, nullptr, target, data);                                   // 005B828E..8292
        message_data_delete(data);                                                              // 005B8297..82A8
        // 005B82AB..8372: threat breakdown (kind 15h)
        data = message_data_new();
        set_int(data, 0, zx8(attack, 0xc1));                                                    // 005B82D2..82E6
        set_int(data, 1, zx8(attack, 0xe0));
        set_int(data, 2, at<U>(attack, 0xe4));
        set_int(data, 3, zx8(attack, 0xe8) + sx8(attack, 0xc2));
        set_int(data, 4, sx8(attack, 0xf4));
        set_int(data, 5, zx8(attack, 0xf0));
        set_int(data, 6, at<U>(attack, 0xec));                                                  // ..005B8353
        kotor_fastcall_004ecc20(self, nullptr, target, data);                                   // 005B8358..835C
        message_data_delete(data);
        // 005B8375..8457: defense breakdown (kind 16h)
        data = message_data_new();
        for (U k = 0; k < 9; ++k) set_int(data, k, sx8(attack, 0xf4 + k));                     // 005B839C..8438
        kotor_fastcall_004ecc60(self, nullptr, target, data);                                   // 005B843D..8441
        message_data_delete(data);
        // 005B845A..85AA: damage breakdown (kind 17h)
        data = message_data_new();                                                              // ebp
        U k = 0;
        for (; k < 0xf; ++k) set_int(data, k, sx16(attack, 0x1c + 2 * k));                     // 005B8481..84B1
        set_int(data, k++, zx8(attack, 0xfe));                                                  // 005B84B3..84BE
        set_int(data, k++, sx8(attack, 0x102));
        set_int(data, k++, zx16(attack, 0x64));
        set_int(data, k++, zx8(attack, 0xff));
        set_int(data, k++, zx8(attack, 0x100));
        set_int(data, k++, zx8(attack, 0x101));
        set_int(data, k++, zx8(attack, 0x103));
        set_int(data, k++, sx8(attack, 0xf1));
        set_int(data, k++, zx8(attack, 0x104));
        set_int(data, k++, zx8(attack, 0x105));
        set_int(data, k++, zx8(attack, 0x106));
        set_int(data, k++, zx8(attack, 0x107));
        set_int(data, k, zx8(attack, 0x108));                                                   // ..005B8587
        kotor_fastcall_004ecca0(self, nullptr, target, data);                                   // 005B858C..8594
        message_data_delete(data);
        // 005B85AD..86AE: effect application breakdown (kind 19h)
        if (at<U>(attack, 0x13c) != 0) {                                                        // 005B85AD..85B5
            data = message_data_new();
            recovered_event_object_004d7780(data, nullptr, 0, at<U>(attack, 0xc));              // 005B85E2..85F2
            for (U j = 0; j < 8; ++j) set_int(data, j, zx8(attack, 0x12c + j));                 // 005B85F7..867A
            set_int(data, 8, sx8(attack, 0x134));                                               // 005B867F..868B
            kotor_fastcall_004ecce0(self, nullptr, target, data);                               // 005B8690..8698
            message_data_delete(data);
        }
        // 005B86B1..87CE: deflect breakdown (kind 1Ah) for results 8..10
        const U r = at<U8>(attack, 0x5c);
        bool deflect;
        if (r == 0xffffffffu) deflect = at<U>(self, 0x558) == 2;                                // 005B86B5..86C1 (movzx: never)
        else deflect = r >= 8 && r <= 10;                                                       // 005B86C9..86D5
        if (deflect) {
            data = message_data_new();
            recovered_event_object_004d7780(data, nullptr, 0, at<U>(attack, 0xc));              // 005B8702..8712
            static const U8 fields[9] = {0x45, 0x41, 0x48, 0x40, 0x43, 0x42, 0x46, 0x47, 0x44}; // +100h + field
            for (U j = 0; j < 9; ++j) set_int(data, j, zx8(attack, 0x100 + fields[j]));         // 005B8717..87AB
            kotor_fastcall_004ecd20(self, nullptr, target, data);                               // 005B87B0..87B8
            message_data_delete(data);
        }
    }
    if (ranged != 0) kotor_fastcall_005b6f30(self, nullptr, target);                            // 005B87D5..87E1
    else kotor_fastcall_005b75d0(self, nullptr, target, 1);                                     // 005B87FA..87FD
}
}  // extern "C"
}  // namespace w1c_damage
