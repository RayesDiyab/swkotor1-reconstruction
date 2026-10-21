// W5 combat-attack lane, part E ("anim"): the ranged/melee per-attack resolvers of CSWSCreature::ResolveAttack
// 005BBA80 and their weapon-discharge / animation helpers:
//   005BB590 ResolveRangedAttack, 005BB890 ResolveMeleeAttack, 005EDCC0/005F33F0 GetClientRangedAnimation,
//   005EDCE0/005F32E0 GetClientMeleeAnimation, 0060D0E0 CSWCCreature::ResolveAttackIndex, 005B4FB0
//   GetWeaponDischargeRow, 005B5230 GetWeaponDischargeShots, 005B52E0 GetWeaponHitShots, 005B5390
//   GetWeaponDischargeTime, 005B5420 GetWeaponDischargeHand, 005B4F30 GetWeaponImpactTime, 005B6DB0 ResolveRangedHit,
//   005B6E90 ResolveRangedMiss, 005B4AA0 ResolveProjectileMissLocation, 005B6C40 ResolveRangedAnimations, 005B7470
//   ResolveMeleeAnimations, 005B7400 SetAttackResult, 005B5600 SetCombatReactionTime, 005B5540 / 005B5570 / 005B55A0
//   (unnamed attack-result / animation-class predicates).
// Transcribed from the original swkotor.exe (read-only); addresses in the comments are original instructions.
// Conventions (W5 t1..t3): thiscall -> extern "C" __fastcall(self, unused_edx, ...); x87 at 53-bit precision ->
// double intermediates, float exactly where the original stores a dword; live_operator_new / live_free only.
// CExoString temporaries are 8-byte frames driven through the existing CExoString providers; the original SEH
// unwind states only destroy those same temporaries (no callee here throws through them in the native build).
// Sibling parts (claude_combat_attack*.cpp) own the callees declared "[part X]". No traps are introduced here.
#include "842d6b1618-w5_t3.hpp"
#include "5889fde5f1-w5_action_bodies.hpp"

using namespace w5;

namespace kotor_reconstruction { struct StartupString; }
namespace kotor_menu20 { struct String { int as_int_005e5630(); }; }          // CExoString::AsINT [0021 service-menu20-4]
namespace w2 { std::uint32_t ftol2_006fae8c(double value); }                  // __ftol2 low dword [w2_ftol_native]
namespace startup_rand {                                                       // CExoString::SubString [0147]
kotor_reconstruction::StartupString* __fastcall SubString005e6270(kotor_reconstruction::StartupString* self, void*,
    kotor_reconstruction::StartupString* out, int start, int count);
}
kotor_reconstruction::StartupString* __fastcall GuiConcat005e5d10(kotor_reconstruction::StartupString* self, void*,
    kotor_reconstruction::StartupString* out, const kotor_reconstruction::StartupString* other); // CExoString::operator+ [0093]

extern "C" {
// ---------------- imports: existing providers (c07b candidate map; object in brackets) ----------------
void* __fastcall recovered_combat_round_attack_004d2b50(void* round, void*, U index);           // CSWSCombatRound::GetAttack [W1 b7]
void __fastcall recovered_combat_round_advance_004d3440(void* round, void*, U length, U flag);   // DecrementRoundLength [W1]
unsigned __fastcall recovered_stats_level_005a5fd0(void* stats, void*, U flag);                 // CSWSCreatureStats::GetLevel (AL)
int __cdecl crt_rand_006fc447();                                                                 // _rand [0479]
double __cdecl vector_magnitude_004ab100(const Vector* v);                                       // vector_magnitude [0464]
void* __fastcall kotor_fastcall_005e5bc0(void* self, void*, I value);                            // CExoString(int) [0266]
void* __fastcall WindowCopy005e5b00(void* self, void*, void* other);                             // CExoString(const CExoString&) [0078]
I __fastcall c2da_current_get_string_00413270(void* table, void*, I row, void* column, void* out); // C2DA::GetCExoStringEntry(int,..) [0379]
U __fastcall kotor_m15_c2da_get_string_entry_00413de0(void* table, void*, void* row, void* column, void* out); // GetCExoStringEntry(str,str) [0811]
I __fastcall kotor_fastcall_00414110(void* table, void*, void* row, void* column, I* out);       // C2DA::GetINTEntry(str,str) [0594]
I __fastcall object_vfx_table_00414260(void* table, void*, void* row, I column, I* out);         // C2DA::GetINTEntry(str,int) [object_vfx_tables]
U __fastcall kotor_fastcall_0060d3e0(void* creature, void*, void* target);                       // CSWCCreature::InCreatureCombat [0833]
U __fastcall kotor_fastcall_00613da0(void* creature, void*, U current, U weapon, U dual, U category); // UpdateMeleeAttackData [0833]
U __fastcall kotor_fastcall_00614110(void* creature, void*, U weapon, U category);               // UpdateRangedAttackData [0833]

// ---------------- sibling parts of this lane ----------------
U __fastcall kotor_fastcall_004d2eb0(void* round, void*);                                        // GetTotalAttacks [part B]
U __fastcall kotor_fastcall_004d3da0(void* round, void*);                                        // GetWeaponAttackType [part B]
U __fastcall kotor_fastcall_004d29f0(void* round, void*, U offhand);                             // SetOffHandAttack [part B]
U __fastcall kotor_fastcall_004d2a00(void* round, void*);                                        // GetOffHandAttack [part B]
U __fastcall kotor_fastcall_004d2970(void* round, void*, U id, U length, U flag);                // CheckActionLengthAtTime [part B]
U __fastcall kotor_fastcall_004d5a50(void* data, void*, U index, U time);                        // ResolveAttackToMeleeSubAttack [part B]
U __fastcall kotor_fastcall_004d5b60(void* data, void*, U index, U time, U rolled, U hit, U hand); // ResolveAttackToRangedSubAttack [part B]
U __fastcall kotor_fastcall_005baca0(void* creature, void*, void* target, U a2, U a3);           // ResolveAttackRoll [part D1]
U __fastcall kotor_fastcall_005ba540(void* creature, void*, void* target, U index);              // ResolveRangedSpecialAttack [part D1]
U __fastcall kotor_fastcall_005ba8e0(void* creature, void*, U index, U a2, void* target, U time); // ResolveMeleeSpecialAttack [part D1]
U __fastcall kotor_fastcall_005b78e0(void* creature, void*, void* target, U a2, U a3, U* deflected, U hand); // IsProjectileParryable [part D1]
U __fastcall kotor_fastcall_005bb440(void* creature, void*, void* target, U a2, U a3);           // ResolveDamage [part D2]
U __fastcall kotor_fastcall_005b8c20(void* creature, void*, void* target);                       // ResolvePostRangedDamage [part D2]
U __fastcall kotor_fastcall_005b8b00(void* creature, void*, void* target_creature);              // ResolvePostMeleeDamage [part D2]
U __fastcall kotor_fastcall_005b5690(void* creature, void*);                                     // IsLightsaberEquipped [part A]

// ---------------- exports reconstructed here ----------------
U __fastcall kotor_fastcall_005b5540(void* self, void*, U result);
U __fastcall kotor_fastcall_005b5570(void* self, void*, U result);
U __fastcall kotor_fastcall_005b55a0(void* self, void*, U animation);
void __fastcall kotor_fastcall_005b5600(void* self, void*, U index, U a2, U a3);
U __fastcall kotor_fastcall_005b7400(void* self, void*, U result);
void* __fastcall kotor_fastcall_005b4fb0(void* self, void*, void* out, U index);
U __fastcall kotor_fastcall_005b5230(void* self, void*, U index);
U __fastcall kotor_fastcall_005b52e0(void* self, void*, U index);
U __fastcall kotor_fastcall_005b5390(void* self, void*, U index, U shot);
U __fastcall kotor_fastcall_005b5420(void* self, void*, U index, U shot);
U __fastcall kotor_fastcall_005b4f30(void* self, void*, U animation, U index);
U __fastcall kotor_fastcall_0060d0e0(void* self, void*, U animation, U dual, U previous);
U __fastcall kotor_fastcall_005f32e0(void* internal, void*, U attacker, U target, U a3, U a4, U a5, U a6);
U __fastcall kotor_fastcall_005f33f0(void* internal, void*, U attacker, U a2, U a3, U a4);
U __fastcall kotor_fastcall_005edce0(void* app, void*, U attacker, U target, U a3, U a4, U a5, U a6);
U __fastcall kotor_fastcall_005edcc0(void* app, void*, U attacker, U a2, U a3, U a4);
Vector* __fastcall kotor_fastcall_005b4aa0(void* self, void*, Vector* out, void* target);
void __fastcall kotor_fastcall_005b6db0(void* self, void*, void* target);
void __fastcall kotor_fastcall_005b6e90(void* self, void*, void* target);
void __fastcall kotor_fastcall_005b6c40(void* self, void*, void* target, U length, U a3);
void __fastcall kotor_fastcall_005b7470(void* self, void*, U count, U a2, void* target, U length, U a5);
void __fastcall kotor_fastcall_005bb590(void* self, void*, void* target, U a2, U a3, U a4, U a5, U a6, U a7);
void __fastcall kotor_fastcall_005bb890(void* self, void*, void* target, U a2, U a3, U a4, U a5, U a6, U a7);
}

namespace {
constexpr U k_radius_0x3ecccf6d = 0x3ecccf6du;    // 0.4000199 immediate (005B4AD2)
const float k_impact_scale_0074b2ec = 23.809524f; // .rdata 0074B2EC
const float k_size_scale_0074b2ac = 0.6667f;      // .rdata 0074B2AC (0x3F2AACDA)
const float k_dist_scale_0074b2a8 = 0.015f;       // .rdata 0074B2A8
const float k_thousandth_0073d708 = 0.001f;       // .rdata 0073D708
const float k_three_quarters_0073e31c = 0.75f;    // .rdata 0073E31C
const float k_two_thirds_0074b2a4 = 0.6666667f;   // .rdata 0074B2A4
const float k_eye_height_0073f414 = 1.5f;         // .rdata 0073F414
const float k_thousand_0073d6fc = 1000.0f;        // .rdata 0073D6FC
constexpr unsigned char k_lookup_ok_0074666c = 0; // .rdata 0074666C (byte)
constexpr unsigned char k_creature_type_00746634 = 5; // .rdata 00746634 (byte): client object type creature
const char k_droid_0074b2c4[] = "Droid";          // .rdata 0074B2C4
const char k_race_0074b2bc[] = "RACE";            // .rdata 0074B2BC
const char k_underscore_0073d704[] = "_";         // .rdata 0073D704
const char k_three_0074b2b8[] = "3_";             // .rdata 0074B2B8
const char k_shots_0074b2cc[] = "Shots";          // .rdata 0074B2CC
const char k_hits_0074b2b0[] = "Hits";            // .rdata 0074B2B0
const char k_switch_mask_0074b2d4[] = "SwitchMask"; // .rdata 0074B2D4

inline Vector vec0() { Vector v; v.x = 0.0f; v.y = 0.0f; v.z = 0.0f; return v; }   // three zero DWORD stores
inline void* rules() { return kotor_cell_rules_007a3a28; }
inline void* rules_2da(U offset) { return at<void*>(at<void*>(rules(), 0xb8), offset); }   // [[7A3A28]+B8]+offset
inline void* round_of(void* c) { return at<void*>(c, 0x9c8); }
inline void* attack_at(void* round, U index) { return recovered_combat_round_attack_004d2b50(round, nullptr, index); }
// GetAttack(round +96C current attack) as the repeated "mov ecx,[x+9C8]; movzx eax,byte [ecx+96C]; call 4D2B50"
inline void* current_attack(void* c) { void* r = round_of(c); return attack_at(r, at<U8>(r, 0x96c)); }
inline kotor_reconstruction::StartupString* ss(void* p) { return static_cast<kotor_reconstruction::StartupString*>(p); }
inline void str_text(void* s, const char* t) { frame_LogStringCtor_005e5a90(s, nullptr, t); }
inline void str_empty(void* s) { kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(s), nullptr); }
inline void str_int(void* s, I v) { kotor_fastcall_005e5bc0(s, nullptr, v); }
inline void str_free(void* s) { frame_LogStringDtor_005e5c20(s, nullptr); }
// CClientExoAppInternal object lookup shared by 005F32E0 / 005F33F0: server id -> client id (| 80000000h unless
// invalid), [internal+14] CGameObjectArray::GetGameObject, client type 5 -> vtbl+2C AsSWCCreature
void* client_creature(void* internal, U server_id) {
    const U id = server_id == k_object_invalid ? k_object_invalid : (server_id | 0x80000000u); // 005F32E4..FC
    void* found = nullptr;                                                          // out slot zeroed (005F3307)
    if (static_cast<unsigned char>(kotor_fastcall_004d8230(at<void*>(internal, 0x14), nullptr, id, &found)) !=
        k_lookup_ok_0074666c) return nullptr;                                       // 005F330B..16
    if (found == nullptr) return nullptr;                                           // 005F331C..1E
    if (at<U8>(found, 8) != k_creature_type_00746634) return nullptr;               // 005F3320..29
    return vcall<void*>(found, 0x2c);                                               // 005F332B..2D
}
// target position minus attacker position as stored floats (fld / fsub dword / fstp dword per component)
Vector delta_from(const void* pos, void* self) {
    const float* p = static_cast<const float*>(pos);
    Vector d;
    d.x = static_cast<float>(static_cast<double>(p[0]) - at<float>(self, 0x90));
    d.y = static_cast<float>(static_cast<double>(p[1]) - at<float>(self, 0x94));
    d.z = static_cast<float>(static_cast<double>(p[2]) - at<float>(self, 0x98));
    return d;
}
}  // namespace

extern "C" {
// =========================================================================================================
// Small predicates / setters on CSWSCreature +558 (last attack result class: 0 hit, 1 parried, 2 deflected)
// ---------------------------------------------------------------------------------------------------------
// 005B5540 (unnamed): RET 4. 1 when the result is a parry (4..6), or -1 with +558 == 1.
U __fastcall kotor_fastcall_005b5540(void* self, void*, U result) {
    const I r = static_cast<I>(result);
    if (r == -1) return at<U>(self, 0x558) == 1 ? 1u : 0u;                         // 005B5546..59
    return (r >= 4 && r <= 6) ? 1u : 0u;                                            // 005B555C..6B (signed)
}

// 005B5570 (unnamed): RET 4. 1 when the result is a deflection (8..10), or -1 with +558 == 2.
U __fastcall kotor_fastcall_005b5570(void* self, void*, U result) {
    const I r = static_cast<I>(result);
    if (r == -1) return at<U>(self, 0x558) == 2 ? 1u : 0u;                          // 005B5576..89
    return (r >= 8 && r <= 0x0a) ? 1u : 0u;                                         // 005B558C..9B (signed)
}

// 005B55A0 (unnamed): RET 4. Animation-class predicate on a word animation id.
// Quirk: the "-1 = use +D4" test compares the zero-extended word with -1 and never succeeds.
U __fastcall kotor_fastcall_005b55a0(void* self, void*, U animation) {
    U16 a = static_cast<U16>(animation);
    const I widened = static_cast<I>(static_cast<U>(a));                            // 005B55A4 movzx eax, dx
    if (widened == -1) a = at<U16>(self, 0xd4);                                     // 005B55A7..AC (never taken)
    const I v = static_cast<I>(static_cast<U>(a));                                  // 005B55B3 movzx ecx, dx
    if (v > 0x2757) return (v == 0x28a1 || v == 0x2af8) ? 1u : 0u;                 // 005B55B8..F8
    if (v >= 0x2756) return 1;                                                      // 005B55C0..C6
    if (v < 0x271f) return 0;                                                       // 005B55C8..CE
    if (v <= 0x2724) return 1;                                                      // 005B55D0..D6
    return v == 0x274d ? 1u : 0u;                                                   // 005B55D8..E0
}

// 005B5600 CSWSCreature::SetCombatReactionTime(attack index, word a2 -> attack+14, word a3 -> attack+10): RET 0Ch
void __fastcall kotor_fastcall_005b5600(void* self, void*, U index, U a2, U a3) {
    at<U16>(attack_at(round_of(self), index), 0x14) = static_cast<U16>(a2);         // 005B5608..19
    at<U16>(attack_at(round_of(self), index), 0x10) = static_cast<U16>(a3);         // 005B561D..2F
}

// 005B7400 CSWSCreature::SetAttackResult(result): RET 4. +558 = 0 (1..3 or -1 with 0), 1 (4..6), 2 (8..10).
U __fastcall kotor_fastcall_005b7400(void* self, void*, U result) {
    const I r = static_cast<I>(result);
    if (r == -1) {                                                                  // 005B7404..07
        const U last = at<U>(self, 0x558);                                          // 005B7409
        if (last == 0) { at<U>(self, 0x558) = 0; return result; }                   // 005B740F..11 -> 005B742F
        if (last == 1) { at<U>(self, 0x558) = 1; return result; }                   // 005B7413..16 -> 005B7446
        if (last == 2) at<U>(self, 0x558) = last;                                   // 005B7418..1D
        return result;
    }
    if (r > 0 && r <= 3) { at<U>(self, 0x558) = 0; return result; }                 // 005B7426..2F
    if (r >= 4 && r <= 6) { at<U>(self, 0x558) = 1; return result; }                // 005B743C..46
    if (r >= 8 && r <= 0x0a) at<U>(self, 0x558) = 2;                                // 005B7453..5D
    return result;                                                                  // EAX = the argument
}

// =========================================================================================================
// Weapon discharge table (rules +B8 -> +E4) helpers
// ---------------------------------------------------------------------------------------------------------
// 005B4FB0 CSWSCreature::GetWeaponDischargeRow(CExoString* out, index): RET 8, EAX = out.
//   row = CExoString(index); a nonzero "Droid" column selects "<RACE of appearance +A60>_<index>";
//   otherwise a +A60 race id of 3 selects "3_<index>".
void* __fastcall kotor_fastcall_005b4fb0(void* self, void*, void* out, U index) {
    alignas(4) unsigned char row[8];                                                // [esp+10] (E-44h)
    str_int(row, static_cast<I>(index));                                            // 005B4FD7..DC
    I droid = 0;                                                                    // the index argument slot (005B4FF2)
    {
        alignas(4) unsigned char column[8];                                         // [esp+20] (E-34h)
        str_text(column, k_droid_0074b2c4);                                         // 005B4FE1..F6
        kotor_fastcall_00414110(rules_2da(0xe4), nullptr, row, column, &droid);     // 005B4FFB..22 (status ignored)
        str_free(column);                                                           // 005B5027..30
    }
    if (droid != 0) {                                                               // 005B5035..39
        alignas(4) unsigned char number[8], race[8], prefix[8], label[8], sep[8], t1[8], t2[8];
        str_int(number, static_cast<I>(index));                                     // 005B503F..44 [E-24h]
        str_empty(race);                                                            // 005B5049..52 [E-34h]
        str_empty(prefix);                                                          // 005B5057..60 [E-3Ch]
        str_text(label, k_race_0074b2bc);                                           // 005B5067..74 [E-2Ch]
        c2da_current_get_string_00413270(rules_2da(0x0c), nullptr, at<U16>(self, 0xa60), label, race); // 005B5079..A1
        str_free(label);                                                            // 005B50A6..AE
        kotor_m15_c2da_get_string_entry_00413de0(rules_2da(0xe8), nullptr, race, number, prefix); // 005B50B3..D6
        str_text(sep, k_underscore_0073d704);                                       // 005B50DB..E4 [E-2Ch]
        GuiConcat005e5d10(ss(prefix), nullptr, ss(t1), ss(sep));                    // 005B50E9..FC [E-14h] = prefix + "_"
        GuiConcat005e5d10(ss(t1), nullptr, ss(t2), ss(row));                        // 005B5101..12 [E-1Ch] = t1 + row
        async_assign_string_005e5c50(row, nullptr, t2);                             // 005B5117..21
        str_free(t2);                                                               // 005B5126..2F
        str_free(t1);                                                               // 005B5134..3D
        str_free(sep);                                                              // 005B5142..4A
        str_free(prefix);                                                           // 005B514F..58
        str_free(race);                                                             // 005B515D..66
        str_free(number);                                                           // 005B516B / 005B51DB..E0
    } else if (at<U16>(self, 0xa60) == 3) {                                         // 005B5171..79
        alignas(4) unsigned char number[8], prefix[8], t1[8];
        str_int(number, static_cast<I>(index));                                     // 005B517B..85 [E-24h]
        str_text(prefix, k_three_0074b2b8);                                         // 005B5187..95 [E-1Ch]
        GuiConcat005e5d10(ss(prefix), nullptr, ss(t1), ss(number));                 // 005B519A..A8 [E-14h] = "3_" + number
        async_assign_string_005e5c50(row, nullptr, t1);                             // 005B51AD..B7
        str_free(t1);                                                               // 005B51BC..C4
        str_free(prefix);                                                           // 005B51C9..D2
        str_free(number);                                                           // 005B51D7..E0
    }
    WindowCopy005e5b00(out, nullptr, row);                                          // 005B51E5..F0
    str_free(row);                                                                  // 005B51F5..06
    return out;                                                                     // 005B5210
}

namespace {
// 005B5230 / 005B52E0 share the shape: row = GetWeaponDischargeRow(index); GetINTEntry(row, column) (out preset 0)
U discharge_int(void* self, U index, const char* name) {
    alignas(4) unsigned char row[8];                                                // [esp+0C]
    kotor_fastcall_005b4fb0(self, nullptr, row, index);
    I value = 0;                                                                    // the index argument slot
    alignas(4) unsigned char column[8];                                             // [esp+04]
    str_text(column, name);
    kotor_fastcall_00414110(rules_2da(0xe4), nullptr, row, column, &value);         // status ignored
    str_free(column);
    str_free(row);
    return static_cast<U>(value);
}
}  // namespace

// 005B5230 CSWSCreature::GetWeaponDischargeShots(index): RET 4. "Shots" column.
U __fastcall kotor_fastcall_005b5230(void* self, void*, U index) {
    return discharge_int(self, index, k_shots_0074b2cc);                           // 005B5248..D6
}

// 005B52E0 CSWSCreature::GetWeaponHitShots(index): RET 4. "Hits" column.
U __fastcall kotor_fastcall_005b52e0(void* self, void*, U index) {
    return discharge_int(self, index, k_hits_0074b2b0);                            // 005B52F8..86
}

// 005B5390 CSWSCreature::GetWeaponDischargeTime(index, shot): RET 8. Integer column shot + 4.
U __fastcall kotor_fastcall_005b5390(void* self, void*, U index, U shot) {
    alignas(4) unsigned char row[8];                                                // [esp+04]
    kotor_fastcall_005b4fb0(self, nullptr, row, index);                             // 005B53A8..B3
    I value = 0;                                                                    // index slot (005B53C2)
    object_vfx_table_00414260(rules_2da(0xe4), nullptr, row, static_cast<I>(shot + 4), &value); // 005B53C6..E7
    str_free(row);                                                                  // 005B53F0..FC
    return static_cast<U>(value);
}

// 005B5420 CSWSCreature::GetWeaponDischargeHand(index, shot): RET 8. AsINT of SwitchMask[shot] (one character).
U __fastcall kotor_fastcall_005b5420(void* self, void*, U index, U shot) {
    alignas(4) unsigned char row[8];                                                // [esp+14]
    kotor_fastcall_005b4fb0(self, nullptr, row, index);                             // 005B5438..43
    alignas(4) unsigned char mask[8];                                               // [esp+04]
    str_empty(mask);                                                                // 005B5448..54
    {
        alignas(4) unsigned char column[8];                                         // [esp+0C]
        str_text(column, k_switch_mask_0074b2d4);                                   // 005B5459..67
        kotor_m15_c2da_get_string_entry_00413de0(rules_2da(0xe4), nullptr, row, column, mask); // 005B546C..93
        str_free(column);                                                           // 005B5498..A1
    }
    alignas(4) unsigned char one[8];                                                // [esp+0C] reused
    startup_rand::SubString005e6270(ss(mask), nullptr, ss(one), static_cast<int>(shot), 1); // 005B54A6..B6
    const U hand = static_cast<U>(reinterpret_cast<kotor_menu20::String*>(one)->as_int_005e5630()); // 005B54BB..C2
    str_free(one);                                                                  // 005B54C7..D2
    str_free(mask);                                                                 // 005B54D7..E0
    str_free(row);                                                                  // 005B54E5..F1
    return hand;
}

// 005B4F30 CSWSCreature::GetWeaponImpactTime(animation, index): RET 8. rules +B8 -> +78 GetINTEntry(
//   CExoString(animation), index + 1).
U __fastcall kotor_fastcall_005b4f30(void* self, void*, U animation, U index) {
    (void)self;
    alignas(4) unsigned char row[8];                                                // [esp+04]
    str_int(row, static_cast<I>(animation));                                        // 005B4F48..52
    I value = 0;                                                                    // animation slot (005B4F63)
    object_vfx_table_00414260(rules_2da(0x78), nullptr, row, static_cast<I>(index + 1), &value); // 005B4F57..82
    str_free(row);                                                                  // 005B4F87..97
    return static_cast<U>(value);
}

// =========================================================================================================
// Client animation selection (CClientExoApp -> CClientExoAppInternal -> CSWCCreature)
// ---------------------------------------------------------------------------------------------------------
// 0060D0E0 CSWCCreature::ResolveAttackIndex(word animation, dual, previous): RET 0Ch. +3C0 = +3C4 = new index.
//   Animation 277Dh outside dual combat picks rand()%5 different from both +3C4 and `previous`; otherwise
//   rand() % 2 (signed modulo).
U __fastcall kotor_fastcall_0060d0e0(void* self, void*, U animation, U dual, U previous) {
    I index;
    if (static_cast<U16>(animation) == 0x277d && dual == 0) {                       // 0060D0E0..F3
        index = at<I>(self, 0x3c4);                                                 // 0060D0F5
        while (index == at<I>(self, 0x3c4) || index == static_cast<I>(previous))    // 0060D100..0A (re-read)
            index = crt_rand_006fc447() % 5;                                        // 0060D10C..19 (idiv)
    } else {
        index = crt_rand_006fc447() % 2;                                            // 0060D11D..2D (and 80000001h)
    }
    at<I>(self, 0x3c0) = index;                                                     // 0060D12F
    at<I>(self, 0x3c4) = index;                                                     // 0060D135
    return static_cast<U>(index);
}

// 005F32E0 CClientExoAppInternal::GetClientMeleeAnimation(attacker, target, a3, a4, a5, a6): RET 18h.
//   a5 != 0: the attacker's +3E8 = a6, EAX = a6. Otherwise ResolveAttackIndex + UpdateMeleeAttackData.
U __fastcall kotor_fastcall_005f32e0(void* internal, void*, U attacker, U target, U a3, U a4, U a5, U a6) {
    void* creature = client_creature(internal, attacker);                          // 005F32E0..32 (ESI)
    void* defender = client_creature(internal, target);                            // 005F3336..81 (EDI)
    if (creature == nullptr) return 0;                                              // 005F3385..89
    if (a5 != 0) { at<U>(creature, 0x3e8) = a6; return a6; }                        // 005F338B..9E
    void* property = reinterpret_cast<void*>(static_cast<std::uintptr_t>(gob_get_property_0x68_0063c070(creature, nullptr))); // 005F33A2..A4
    const U8 category = at<U8>(property, 0xc5);                                     // 005F33A9 (BL)
    const U dual = kotor_fastcall_0060d3e0(creature, nullptr, defender);            // 005F33AF..B2
    kotor_fastcall_0060d0e0(creature, nullptr, a3, dual, 0xffffffffu);              // 005F33B7..C3
    return kotor_fastcall_00613da0(creature, nullptr, a3, a4, dual, category);      // 005F33C8..D5
}

// 005F33F0 CClientExoAppInternal::GetClientRangedAnimation(attacker, a2, a3, a4): RET 10h.
U __fastcall kotor_fastcall_005f33f0(void* internal, void*, U attacker, U a2, U a3, U a4) {
    void* creature = client_creature(internal, attacker);                          // 005F33F0..40 (ESI)
    if (creature == nullptr) return 0;                                              // 005F3444..48
    if (a3 != 0) { at<U>(creature, 0x3e8) = a4; return a4; }                        // 005F344A..5D
    void* property = reinterpret_cast<void*>(static_cast<std::uintptr_t>(gob_get_property_0x68_0063c070(creature, nullptr))); // 005F3460..62
    return kotor_fastcall_00614110(creature, nullptr, a2, at<U8>(property, 0xc5));  // 005F3467..76
}

// 005EDCE0 CClientExoApp::GetClientMeleeAnimation: ECX = [this+4], tail jump to 005F32E0 (RET 18h).
U __fastcall kotor_fastcall_005edce0(void* app, void*, U attacker, U target, U a3, U a4, U a5, U a6) {
    return kotor_fastcall_005f32e0(at<void*>(app, 4), nullptr, attacker, target, a3, a4, a5, a6); // 005EDCE0..E3
}

// 005EDCC0 CClientExoApp::GetClientRangedAnimation: zeroes its own a3/a4 argument slots, ECX = [this+4], tail jump
// to 005F33F0 (RET 10h). The caller's a3/a4 values are never used.
U __fastcall kotor_fastcall_005edcc0(void* app, void*, U attacker, U a2, U a3, U a4) {
    a3 = 0;                                                                         // 005EDCC8 [esp+0C]
    a4 = 0;                                                                         // 005EDCC0 [esp+10]
    return kotor_fastcall_005f33f0(at<void*>(app, 4), nullptr, attacker, a2, a3, a4); // 005EDCD0..D3
}

// =========================================================================================================
// Ranged hit / miss projectile targets
// ---------------------------------------------------------------------------------------------------------
// 005B4AA0 CSWSCreature::ResolveProjectileMissLocation(Vector* out, CSWSObject* target): RET 8, EAX = out.
//   A point beside the target (perpendicular offset scaled by the target's size, x3d6 spread for result 0Ah),
//   projected 1000 units along the eye line; the line-of-sight end (or the blocking point) is returned.
Vector* __fastcall kotor_fastcall_005b4aa0(void* self, void*, Vector* out, void* target) {
    Vector blocked = vec0();                                                        // [esp+50] (005B4AB0..C0)
    Vector reached;                                                                 // [esp+44]
    if (target == nullptr) { *out = blocked; return out; }                          // 005B4AAB..C8 -> 005B4DFD
    float radius = flt(k_radius_0x3ecccf6d);                                        // [esp+0C] (005B4AD2)
    float spread = 1.0f;                                                            // [esp+08] (005B4ADA)
    if (vcall<void*>(target, 0x30) != nullptr) {                                    // 005B4ACE..E7
        void* sized = vcall<void*>(target, 0x30);                                   // 005B4AE9..ED
        radius = static_cast<float>(static_cast<double>(at<float>(at<void*>(sized, 0x340), 4)) * k_size_scale_0074b2ac); // 005B4AF0..03
        (void)vcall<void*>(target, 0x30);                                           // 005B4B07 (result unused)
        void* creature = vcall<void*>(target, 0x30);                                // 005B4B0A..0E
        if (at<U8>(current_attack(creature), 0x5c) == 0x0a) {                       // 005B4B11..28
            I sum = crt_rand_006fc447() % 3;                                        // 005B4B2B..38
            sum += crt_rand_006fc447() % 3;                                         // 005B4B3A..47
            sum += crt_rand_006fc447() % 3;                                         // 005B4B49..56
            spread = static_cast<float>(sum);                                       // 005B4B58..61 (fild / fstp dword)
        }
    }
    Vector away;                                                                    // [esp+2C] attacker - target
    away.x = static_cast<float>(static_cast<double>(at<float>(self, 0x90)) - at<float>(target, 0x90)); // 005B4B65..7B
    away.y = static_cast<float>(static_cast<double>(at<float>(self, 0x94)) - at<float>(target, 0x94)); // 005B4B7F..8B
    away.z = static_cast<float>(static_cast<double>(at<float>(self, 0x98)) - at<float>(target, 0x98)); // 005B4B8F..9B
    Vector unit;                                                                    // [esp+74]
    normalize_004ab180(&unit, &away);                                               // 005B4B9F
    Vector side = {0.0f, 0.0f, 1.0f};                                               // [esp+38] (005B4BB6..C6)
    Vector cross;                                                                   // [esp+14]
    vector_cross_004ab210(&cross, &unit, &side);                                    // 005B4BA4..D3
    if (crt_rand_006fc447() % 2 != 0) {                                             // 005B4BD6..E7 (signed modulo)
        side.x = -cross.x; cross.x = side.x;                                        // 005B4BE9..FB (fchs)
        side.y = -cross.y; cross.y = side.y;                                        // 005B4BF7..0F
        side.z = -cross.z; cross.z = side.z;                                        // 005B4C05..1B
    }
    side.x = static_cast<float>(static_cast<double>(cross.x) * spread); cross.x = side.x; // 005B4C1F..33
    side.y = static_cast<float>(static_cast<double>(cross.y) * spread); cross.y = side.y; // 005B4C2F..4B
    side.z = static_cast<float>(static_cast<double>(cross.z) * spread); cross.z = side.z; // 005B4C3F..5F
    const double ox = static_cast<double>(side.x) * radius;                         // 005B4C53..5B (kept in ST)
    const double oy = static_cast<double>(side.y) * radius;                         // 005B4C63..67
    const float oz = static_cast<float>(static_cast<double>(side.z) * radius);      // 005B4C6B..73 [esp+64]
    side.x = static_cast<float>(ox + at<float>(target, 0x90));                      // 005B4C77..7F
    side.y = static_cast<float>(oy + at<float>(target, 0x94));                      // 005B4C83..89
    side.z = static_cast<float>(static_cast<double>(oz) + at<float>(target, 0x98)); // 005B4C8D..97
    radius = static_cast<float>(((static_cast<double>(away.z) * away.z + static_cast<double>(away.y) * away.y) +
                                 static_cast<double>(away.x) * away.x) * k_dist_scale_0074b2a8); // 005B4C9B..BD
    const I jitter = crt_rand_006fc447() % 0x3e8;                                   // 005B4CC1..CC
    Vector eye;                                                                     // [esp+20] attacker position
    eye.x = at<float>(self, 0x90); eye.y = at<float>(self, 0x94); eye.z = at<float>(self, 0x98); // 005B4CCE..F0
    const double lift = ((static_cast<double>(jitter) * k_thousandth_0073d708) * radius) * k_three_quarters_0073e31c; // 005B4CE6..FE
    const double drop = static_cast<double>(k_eye_height_0073f414) - static_cast<double>(radius) * k_two_thirds_0074b2a4; // 005B4D04..0E
    const double aim_z = (lift + drop) + side.z;                                    // 005B4D14..16
    eye.z = static_cast<float>(static_cast<double>(eye.z) + k_eye_height_0073f414); // 005B4D1A..24
    Vector line;                                                                    // [esp+5C]
    line.x = static_cast<float>(static_cast<double>(side.x) - eye.x);               // 005B4D28..30
    line.y = static_cast<float>(static_cast<double>(side.y) - eye.y);               // 005B4D34..3C
    line.z = static_cast<float>(aim_z - eye.z);                                     // 005B4D40..4D
    Vector dir;                                                                     // [esp+68]
    normalize_004ab180(&dir, &line);                                                // 005B4D44..52
    const double fx = static_cast<double>(dir.x) * k_thousand_0073d6fc;             // 005B4D57..5B
    const double fy = static_cast<double>(dir.y) * k_thousand_0073d6fc;             // 005B4D64..6B
    const float fz = static_cast<float>(static_cast<double>(dir.z) * k_thousand_0073d6fc); // 005B4D74..84 [esp+64]
    reached.x = static_cast<float>(fx + eye.x);                                     // 005B4D88..98
    reached.y = static_cast<float>(fy + eye.y);                                     // 005B4DA1..AC
    reached.z = static_cast<float>(static_cast<double>(fz) + eye.z);                // 005B4DB0..D3
    blocked = vec0();                                                               // 005B4DBB..CB
    unsigned hit_object = k_object_invalid;                                         // [esp+10] (005B4DD7)
    void* area = recovered_object_area_004cb120(self, nullptr);                     // 005B4DDF
    if (area_clear_line_of_sight_0050c330(area, nullptr, &eye, &reached, &blocked, &hit_object,
                                          at<U>(self, 4), at<U>(target, 4), 1, 1) != 0) { // 005B4D78..ED
        *out = reached;                                                             // 005B4DEF..FB / 005B4E09..17
        return out;
    }
    *out = blocked;                                                                 // 005B4DFD..17
    return out;
}

// 005B6DB0 CSWSCreature::ResolveRangedHit(CSWSObject* target): RET 4. Attack +12 = hit (271Eh) / deflect (271Ch)
//   animation, +68 = target position, +10 = word(|target - attacker| * 23.8095) projectile travel time.
void __fastcall kotor_fastcall_005b6db0(void* self, void*, void* target) {
    if (target == nullptr) return;                                                  // 005B6DB4..BD
    void* attack = current_attack(self);                                            // 005B6DC3..D7 (EDI)
    const I r = static_cast<I>(at<U8>(attack, 0x5c));                               // 005B6DD9
    if (r == -1) {                                                                  // 005B6DDD..E0 (never: movzx)
        if (at<U>(self, 0x558) == 0) at<U16>(attack, 0x12) = 0x271e;                // 005B6DE2..EA
        else if (at<U>(self, 0x558) == 2) at<U16>(attack, 0x12) = 0x271c;           // 005B6DF1..FA
    } else if (r > 0 && r <= 3) {
        at<U16>(attack, 0x12) = 0x271e;                                             // 005B6DFC..05
    } else if (r >= 8 && r <= 0x0a) {
        at<U16>(attack, 0x12) = 0x271c;                                             // 005B6E0D..17
    }
    copy_vector(static_cast<unsigned char*>(attack) + 0x68, static_cast<unsigned char*>(target) + 0x90); // 005B6E1D..35
    Vector d = delta_from(static_cast<unsigned char*>(target) + 0x90, self);        // 005B6E38..5F
    const double t = vector_magnitude_004ab100(&d) * k_impact_scale_0074b2ec;       // 005B6E63..68
    at<U16>(attack, 0x10) = static_cast<U16>(w2::ftol2_006fae8c(t));               // 005B6E71..76
}

// 005B6E90 CSWSCreature::ResolveRangedMiss(CSWSObject* target): RET 4. Attack +68 = ResolveProjectileMissLocation,
//   +10 = word(|miss point - attacker| * 23.8095).
void __fastcall kotor_fastcall_005b6e90(void* self, void*, void* target) {
    if (target == nullptr) return;                                                  // 005B6E94..9D
    void* attack = current_attack(self);                                            // 005B6E9F..BB (EDI)
    Vector miss;                                                                    // [esp+10]
    const Vector* p = kotor_fastcall_005b4aa0(self, nullptr, &miss, target);        // 005B6EB3..BD
    copy_vector(static_cast<unsigned char*>(attack) + 0x68, p);                     // 005B6EC2..D4
    Vector d = delta_from(static_cast<unsigned char*>(attack) + 0x68, self);        // 005B6ED7..FE
    const double t = vector_magnitude_004ab100(&d) * k_impact_scale_0074b2ec;       // 005B6F02..07
    at<U16>(attack, 0x10) = static_cast<U16>(w2::ftol2_006fae8c(t));               // 005B6F10..15
}

// =========================================================================================================
// Reaction animations
// ---------------------------------------------------------------------------------------------------------
// 005B6C40 CSWSCreature::ResolveRangedAnimations(CSWSObject* target, length, a3): RET 0Ch.
void __fastcall kotor_fastcall_005b6c40(void* self, void*, void* target, U length, U a3) {
    if (target == nullptr) return;                                                  // 005B6C42..4B
    const U index = at<U8>(round_of(self), 0x96c);                                  // 005B6C51..61 [esp+10]
    void* attack = attack_at(round_of(self), index);                                // 005B6C65 (kept in the target slot)
    at<U16>(attack, 8) = static_cast<U16>(length);                                  // 005B6C70
    at<U16>(attack, 0x10) = 0;                                                      // 005B6C74
    recovered_combat_round_advance_004d3440(round_of(self), nullptr, length, 0);   // 005B6C6E..85
    void* creature = vcall<void*>(target, 0x30);                                    // 005B6C8A..92 (ESI)
    if (creature == nullptr) return;                                                // 005B6C94..96
    U dead = 0;                                                                     // the length slot = 0 (005B6CA4)
    if (at<U8>(creature, 0x8ed) != 0 || recovered_creature_incapacitated_004ef890(creature, nullptr) != 0) dead = 1; // 005B6C9C..C4
    at<U>(round_of(self), 0x948) = dead;                                            // 005B6CC4..CA
    if (a3 == 0 && kotor_fastcall_005b5690(creature, nullptr) == 0 &&               // 005B6CD0..E1
        kotor_fastcall_005b55a0(creature, nullptr, 0xffff) == 0 &&                  // 005B6CE3..F1
        at<U>(round_of(creature), 0x9b8) != 0)                                      // 005B6CF3..01
        at<U16>(attack, 0x12) = 0x271b;                                             // 005B6D03..07
    else
        at<U16>(attack, 0x12) = 0x2711;                                             // 005B6D0F..13
    if (at<U8>(creature, 0x8ed) != 0 || recovered_creature_incapacitated_004ef890(creature, nullptr) != 0) { // 005B6D19..2C
        at<U16>(attack, 0x12) = 0x2711;                                             // 005B6D2E..35
        return;
    }
    const U self_id = at<U>(self, 4);                                               // 005B6D40..48
    void* target_round = round_of(vcall<void*>(target, 0x30));                      // 005B6D43..53
    if (kotor_fastcall_004d2970(target_round, nullptr, self_id, length, 0) == 0) return; // 005B6D59..66
    if (at<U>(round_of(self), 0x9b8) == 0) return;                                  // 005B6D68..76
    kotor_fastcall_005b5600(self, nullptr, index, length, 0);                       // 005B6D78..82
    vcall<void>(creature, 0x7c, static_cast<U>(at<U16>(attack, 0x12)));             // 005B6D87..94
    recovered_combat_round_advance_004d3440(round_of(creature), nullptr, length, 0); // 005B6D97..A0
}

// 005B7470 CSWSCreature::ResolveMeleeAnimations(count, a2, CSWSObject* target, length, a5): RET 14h (count and a2
// are not read).
void __fastcall kotor_fastcall_005b7470(void* self, void*, U count, U a2, void* target, U length, U a5) {
    (void)count; (void)a2;
    if (target == nullptr) return;                                                  // 005B7472..7B
    const U index = at<U8>(round_of(self), 0x96c);                                  // 005B7481..91 [esp+10]
    void* attack = attack_at(round_of(self), index);                                // 005B7495..9E (EBP)
    at<U16>(attack, 8) = static_cast<U16>(length);                                  // 005B74A2
    recovered_combat_round_advance_004d3440(round_of(self), nullptr, length, 0);   // 005B74A0..AD
    void* creature = vcall<void*>(target, 0x30);                                    // 005B74B2..B9 (ESI)
    if (creature == nullptr) return;                                                // 005B74BB..BD
    const I r = static_cast<I>(at<U8>(attack, 0x5c));                               // 005B74C3
    bool hit;
    if (r == -1) hit = at<U>(self, 0x558) == 0;                                     // 005B74C7..D4 (never: movzx)
    else hit = r > 0 && r <= 3;                                                     // 005B74EA..F1
    if (hit) {
        at<U16>(attack, 0x12) = 0x271e;                                             // 005B74F3
    } else if (kotor_fastcall_005b5570(self, nullptr, static_cast<U>(r)) != 0) {    // 005B74D6..E0
        at<U16>(attack, 0x12) = 0x271c;                                             // 005B74E2
    } else if (kotor_fastcall_005b5540(self, nullptr, at<U8>(attack, 0x5c)) != 0) { // 005B74FB..09
        at<U16>(attack, 0x12) = 0x271b;                                             // 005B750B
    }
    U dead = 0;                                                                     // the target slot = 0 (005B7519)
    if (at<U8>(creature, 0x8ed) != 0 || recovered_creature_incapacitated_004ef890(creature, nullptr) != 0) dead = 1; // 005B7511..39
    at<U>(round_of(self), 0x948) = dead;                                            // 005B7539..3F
    if (at<U8>(creature, 0x8ed) != 0 || recovered_creature_incapacitated_004ef890(creature, nullptr) != 0) { // 005B7545..58
        at<U16>(attack, 0x12) = 0x2711;                                             // 005B75B9
        return;
    }
    if (kotor_fastcall_004d2970(round_of(creature), nullptr, at<U>(self, 4), length, 0) == 0 && a5 == 0) { // 005B755A..76
        at<U16>(attack, 0x12) = 0x2711;
        return;
    }
    if (at<U>(round_of(self), 0x9b8) == 0) { at<U16>(attack, 0x12) = 0x2711; return; } // 005B7578..86
    kotor_fastcall_005b5600(self, nullptr, index, length, 0);                       // 005B7588..92
    vcall<void>(creature, 0x7c, static_cast<U>(at<U16>(attack, 0x12)));             // 005B7597..A0
    recovered_combat_round_advance_004d3440(round_of(creature), nullptr, length, 0); // 005B75A3..AC
}

// =========================================================================================================
// 005BB590 CSWSCreature::ResolveRangedAttack(CSWSObject* target, a2, a3, a4, a5, a6, a7): RET 1Ch.
//   a4 != 0: a5 is the discharge animation (and attack +B8/+BC = 1/a5 at the end); a2 is not read.
//   For each discharge shot a random pick against the remaining shots decides whether the shot is one of the
//   round's (total attacks) rolled attacks; parried/deflected results consult IsProjectileParryable.
void __fastcall kotor_fastcall_005bb590(void* self, void*, void* target, U a2, U a3, U a4, U a5, U a6, U a7) {
    (void)a2;
    if (target == nullptr) return;                                                  // 005BB594..9D
    void* attack = current_attack(self);                                            // 005BB5A3..B8 (EDI)
    at<U>(attack, 0x40) = 1;                                                        // 005BB5BA
    at<U>(attack, 0xc) = at<U>(target, 4);                                          // 005BB5C1..C4
    {
        void* round = round_of(self);                                               // 005BB5C7
        const U8 n = static_cast<U8>(at<U8>(round, 0x974) + 1);                     // 005BB5CD..D3
        at<U8>(round, 0x974) = n;                                                   // 005BB5D5
        at<U8>(attack, 0) = n;                                                      // 005BB5DD
    }
    U animation;                                                                    // [esp+10] / EBX
    if (a4 != 0) animation = a5;                                                    // 005BB5DF..EF
    else animation = kotor_fastcall_005edcc0(client_app(), nullptr, at<U>(self, 4), at<U16>(attack, 0x64), 0, 0); // 005BB5F1..10
    const I shots = static_cast<I>(kotor_fastcall_005b5230(self, nullptr, animation)); // 005BB612..15 [esp+28]
    I shots_left = shots;                                                           // [esp+20]
    (void)kotor_fastcall_005b52e0(self, nullptr, animation);                        // 005BB61A..25 (quirk: result overwritten)
    I rolls_left = static_cast<I>(static_cast<U8>(kotor_fastcall_004d2eb0(round_of(self), nullptr))); // 005BB62A..42 [esp+1C]
    I shot = 0;                                                                     // EBX
    I rolled_index = 0;                                                             // [esp+14]
    U any_hit = 0;                                                                  // [esp+2C]
    if (shots > 0) {                                                                // 005BB640..4E (jle)
        do {
            U hit = 0;                                                              // [esp+18] (005BB660)
            U rolled = 0;                                                           // [esp+24] (005BB664)
            at<U8>(attack, 0x5c) = 4;                                               // 005BB668
            // quirk: shots_left reaches 0 only after the last shot; a zero "Shots" column skips the loop
            const I pick = crt_rand_006fc447() % shots_left;                        // 005BB66C..72 (idiv)
            if (pick < rolls_left) {                                                // 005BB676..7C (jge: signed)
                --rolls_left;                                                       // 005BB68C..8D
                {
                    void* round = round_of(self);                                   // 005BB682
                    kotor_fastcall_004d29f0(round, nullptr, rolled_index >= static_cast<I>(at<U8>(round, 0x990)) ? 1u : 0u); // 005BB691..A8
                }
                rolled = 1;                                                         // 005BB69F
                const U offhand = kotor_fastcall_004d2a00(round_of(self), nullptr); // 005BB6AD..B3
                at<U8>(attack, 0x3a) = static_cast<U8>((offhand != 0 ? 1 : 0) + 1); // 005BB6C4..D0
                kotor_fastcall_005baca0(self, nullptr, target, a4, a6);             // 005BB6B8..D3
                if (at<U16>(attack, 0x64) != 0)                                     // 005BB6D8..DD
                    kotor_fastcall_005ba540(self, nullptr, target, static_cast<U>(rolled_index)); // 005BB6DF..E7
                const I r = static_cast<I>(at<U8>(attack, 0x5c));                   // 005BB6EC
                bool landed;
                if (r == -1) landed = at<U>(self, 0x558) == 0 || at<U>(self, 0x558) == 2; // 005BB6F0..18 (never: movzx)
                else landed = (r > 0 && r <= 3) || (r >= 8 && r <= 0x0a);           // 005BB701..22
                if (landed) { hit = 1; any_hit = 1; }                               // 005BB724..2D
                ++rolled_index;                                                     // 005BB731..3A
            }
            const I r2 = static_cast<I>(at<U8>(attack, 0x5c));                      // 005BB73E
            const bool parried = r2 == -1 ? at<U>(self, 0x558) == 1 : (r2 >= 4 && r2 <= 6); // 005BB742..5A
            if (parried) {
                const U hand = kotor_fastcall_005b5420(self, nullptr, animation, static_cast<U>(shot)); // 005BB75C..64
                hit = 0;                                                            // 005BB776 (written before the call)
                const U parryable = kotor_fastcall_005b78e0(self, nullptr, target, 1, 1, &hit, hand); // 005BB769..7E
                if (parryable != 0)                                                 // 005BB785..87
                    at<U8>(current_attack(self), 0x5c) = static_cast<U8>(hit != 0 ? 10 : 8); // 005BB789..A9
                hit = parryable;                                                    // 005BB7AC
            }
            if (hit != 0) {                                                         // 005BB7B4..BD
                kotor_fastcall_005b6db0(self, nullptr, target);                     // 005BB7BF
                const U8 cl = at<U8>(attack, 0x5c);                                 // 005BB7C4
                const I r3 = static_cast<I>(cl);
                bool damage;
                if (r3 == -1) damage = at<U>(self, 0x558) == 0 || cl == 9;          // 005BB7CA..D9 (never: movzx)
                else damage = (r3 > 0 && r3 <= 3) || cl == 9;                       // 005BB7DB..E7
                if (damage) {
                    kotor_fastcall_005bb440(self, nullptr, target, a4, a7);         // 005BB7E9..F6
                    kotor_fastcall_005b8c20(self, nullptr, target);                 // 005BB7FB..FE
                }
            } else {
                kotor_fastcall_005b6e90(self, nullptr, target);                     // 005BB805
            }
            const U time = kotor_fastcall_005b5390(self, nullptr, animation, static_cast<U>(shot)); // 005BB80A..1F
            const U hand = kotor_fastcall_005b5420(self, nullptr, animation, static_cast<U>(shot)); // 005BB817..21
            kotor_fastcall_004d5b60(attack, nullptr, static_cast<U>(shot), time, rolled, hit, hand); // 005BB826..35
            --shots_left;                                                           // 005BB83A..4A
            ++shot;                                                                 // 005BB847
        } while (shot < shots);                                                     // 005BB848..4E
    }
    kotor_fastcall_005b6c40(self, nullptr, target, a3, any_hit);                    // 005BB854..61
    if (a4 != 0) {                                                                  // 005BB866..6C
        at<U>(attack, 0xb8) = 1;                                                    // 005BB872
        at<U>(attack, 0xbc) = a5;                                                   // 005BB86E / 005BB87C
    }
}

// 005BB890 CSWSCreature::ResolveMeleeAttack(CSWSObject* target, a2, a3, a4, a5, a6, a7): RET 1Ch.
void __fastcall kotor_fastcall_005bb890(void* self, void*, void* target, U a2, U a3, U a4, U a5, U a6, U a7) {
    if (target == nullptr) return;                                                  // 005BB893..9B
    void* attack = current_attack(self);                                            // 005BB8A1..BC (EBX)
    const I count = static_cast<I>(static_cast<U8>(kotor_fastcall_004d2eb0(round_of(self), nullptr))); // 005BB8B6..CA [esp+10]
    I i = 0;                                                                        // EBP
    if (count > 0) {                                                                // 005BB8C8..CE
        do {
            at<U>(attack, 0xc) = at<U>(target, 4);                                  // 005BB8D4..D7
            at<U8>(attack, 0x3a) = static_cast<U8>(kotor_fastcall_004d3da0(round_of(self), nullptr)); // 005BB8DA..E5
            {
                void* round = round_of(self);                                       // 005BB8E8
                kotor_fastcall_004d29f0(round, nullptr, i >= static_cast<I>(at<U8>(round, 0x990)) ? 1u : 0u); // 005BB8EE..FD
            }
            void* creature = vcall<void*>(target, 0x30);                            // 005BB902..06 [esp+18]
            if (creature != nullptr &&                                              // 005BB909..0F
                (at<U8>(creature, 0x8ed) != 0 || recovered_creature_incapacitated_004ef890(creature, nullptr) != 0) && // 005BB911..24
                static_cast<U8>(recovered_stats_level_005a5fd0(at<void*>(creature, 0xa74), nullptr, 0)) <= 4 && // 005BB92A..3B (ja)
                at<U>(creature, 0xa88) == 0)                                        // 005BB93D..49
                at<U>(current_attack(self), 0x4c) = 1;                              // 005BB94B..5E
            kotor_fastcall_005baca0(self, nullptr, target, a4, a6);                 // 005BB965..72
            void* client = client_app();                                            // 005BB977..7D ([esp+18])
            const U animation = kotor_fastcall_005edce0(client, nullptr, at<U>(self, 4), at<U>(target, 4),
                at<U>(self, 0xd4), at<U16>(attack, 0x64), a4, a5);                  // 005BB980..A6
            const U impact = kotor_fastcall_005b4f30(self, nullptr, animation, static_cast<U>(i)); // 005BB9AB..B9 [esp+18]
            if (at<U16>(attack, 0x64) != 0)                                         // 005BB9B4..BD
                kotor_fastcall_005ba8e0(self, nullptr, static_cast<U>(i), a2, target, impact); // 005BB9BF..C9
            const I r = static_cast<I>(at<U8>(attack, 0x5c));                       // 005BB9CE
            bool damage;
            if (r == -1) damage = at<U>(self, 0x558) == 0;                          // 005BB9D2..E1 (never: movzx)
            else damage = r > 0 && r <= 3;                                          // 005BB9E3..EA
            if (damage) {
                kotor_fastcall_005bb440(self, nullptr, target, a4, a7);             // 005BB9EC..F9
                kotor_fastcall_005b8b00(self, nullptr, vcall<void*>(target, 0x30)); // 005BB9FE..08
            }
            kotor_fastcall_004d5a50(attack, nullptr, static_cast<U>(i), impact);    // 005BBA0D..15
            kotor_fastcall_005b7400(self, nullptr, at<U8>(attack, 0x5c));           // 005BBA1A..21
            ++i;                                                                    // 005BBA2A
        } while (i < count);                                                        // 005BBA26..2D
    }
    at<U>(self, 0x550) = at<U16>(attack, 0x64);                                     // 005BBA33..41
    kotor_fastcall_005b7470(self, nullptr, static_cast<U>(i), a2, target, a3, a4);  // 005BBA37..50
    if (a4 != 0) {                                                                  // 005BBA55..5B
        at<U>(attack, 0xb8) = 1;                                                    // 005BBA61
        at<U>(attack, 0xbc) = a5;                                                   // 005BBA5D / 005BBA6B
    }
}
}  // extern "C"
