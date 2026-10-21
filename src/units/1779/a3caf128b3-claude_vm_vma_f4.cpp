// Lane vmA, sub-batch f4 ("effects"): NWScript engine-command handlers CSWVirtualMachineCommands::ExecuteCommand*
// that build / inspect CGameEffect engine structures, reconstructed from the original swkotor.exe (read-only,
// capstone linear sweep).  Addresses in comments are the original instructions.  Conventions: claude_vm_vmA_common.hpp.
//  * Every handler: int __thiscall (int routine, int argc), RET 8 (verified) -> kotor_fastcall_<addr> @16.
//  * CGameEffect: +8 WORD type, +0A WORD flags; engine structure 0 = effect.
//  * Original quirks kept: the operator new result is not checked before the type/flag stores; pop failures after
//    the effect allocation leak it; a failed StackPushEngineStructure leaks it; EffectSkillIncrease never frees its
//    effect (no ~CGameEffect after a successful push); GetIsLinkImmune / Versus*Effect return -2000 (push-failure
//    code) on an engine-structure / integer pop failure.
// .rdata literals: effect types [007466A4]=0Ah attack increase, [007466AC]=0Ch damage reduction,
// [007466BC]=10h damage immunity increase, [007466C0]=11h damage immunity decrease, [00746758]=37h skill increase,
// [00746770]=3Dh damage shield, [007467EC]=5Dh blaster deflection decrease, [00746800]=62h body fuel,
// [0074667C]=0 invalid; [007468DC]=7 / [007468C4]=0 attack-bonus max / fallback; [00746884]=2000h / [0074685C]=8
// damage-type max / fallback; [007468AC]=6 / [00746894]=0 damage-power max / fallback; [0074BF18] WORD 7 racial max.
#include "520143efd3-claude_vm_vma_common.hpp"

extern "C" {
extern void* kotor_cell_rules_007a3a28;                                                // [007A3A28] CSWRules* [0421]
U __fastcall kotor_fastcall_005a6a90(void* stats, void*, void* effect);                // CSWSCreatureStats::GetEffectLinkImmunity RET 4 [claude_combat_attack_rolls]
}

using namespace vma;

namespace {
constexpr U k_effect_invalid_0074667c = 0;
constexpr U k_effect_attack_increase_007466a4 = 0x0a;
constexpr U k_effect_damage_reduction_007466ac = 0x0c;
constexpr U k_effect_damage_immunity_increase_007466bc = 0x10;
constexpr U k_effect_damage_immunity_decrease_007466c0 = 0x11;
constexpr U k_effect_skill_increase_00746758 = 0x37;
constexpr U k_effect_damage_shield_00746770 = 0x3d;
constexpr U k_effect_blaster_deflection_decrease_007467ec = 0x5d;
constexpr U k_effect_body_fuel_00746800 = 0x62;
constexpr I k_attack_bonus_max_007468dc = 7;
constexpr I k_attack_bonus_fallback_007468c4 = 0;
constexpr I k_damage_type_max_00746884 = 0x2000;
constexpr I k_damage_type_fallback_0074685c = 8;
constexpr I k_damage_power_max_007468ac = 6;
constexpr I k_damage_power_fallback_00746894 = 0;
constexpr I k_racial_type_max_0074bf18 = 7;                                            // WORD at 0074BF18 (movzx)
inline U8 rules_byte(U offset) { return at<U8>(kotor_cell_rules_007a3a28, offset); }
// GetGameObject(self+8) found (status byte 0) and non-null -> SetCreator(self+8)
inline void creator_if_found(void* self, void* e) {
    void* object = nullptr;
    if (lookup(self_id(self), &object) && object != nullptr)
        recovered_effect_creator_00503a00(e, nullptr, self_id(self));
}
inline void seti(void* e, U index, I value) { recovered_effect_integer_005036a0(e, nullptr, index, static_cast<U>(value)); }
}  // namespace

extern "C" {

// ================================================================== 005310E0 EffectAttackIncrease(nBonus, nModifierType) [routine 118]
// Both pops are unconditional (no argc test).  Integer 2 = byte [[007A3A28]+0AAh].
int __fastcall kotor_fastcall_005310e0(void* self, void*, int, unsigned) {
    I bonus = 0, modifier = 0;
    if (!pop_int(&bonus)) return k_pop_failed;                                         // 005310F9..0D [esp+8]
    if (!pop_int(&modifier)) return k_pop_failed;                                      // 0053110F..21 [esp+4]
    if (modifier < 0 || modifier > k_attack_bonus_max_007468dc)                        // 0053113A..48 (jl / jle signed)
        modifier = k_attack_bonus_fallback_007468c4;                                   // 0053114A..50
    void* e = new_effect();                                                            // 00531155..7F
    set_effect_type(e, k_effect_attack_increase_007466a4);                             // 00531181..8D
    set_subtype_magical(e);                                                            // 00531187..A2
    seti(e, 0, bonus);                                                                 // 005311A6..AF
    seti(e, 1, modifier);                                                              // 005311B4..BD
    seti(e, 2, rules_byte(0xaa));                                                      // 005311C2..D4
    creator_if_found(self, e);                                                         // 005311D9..0C
    return push_effect(e);                                                             // 00531211..5D
}

// ================================================================== 00532050 EffectDamageImmunityIncrease(nDamageType, nPercent) [routine 275]
// The effect is allocated and typed before the pops (leaked on a pop failure).  SetCreator is unconditional.
int __fastcall kotor_fastcall_00532050(void* self, void*, int, unsigned) {
    void* e = new_effect();                                                            // 0053206A..96
    set_effect_type(e, k_effect_damage_immunity_increase_007466bc);                    // 00532098..A2
    I damage_type = 0, percent = 0;
    if (!pop_int(&damage_type)) return k_pop_failed;                                   // 0053209E..BC [esp+0C]
    if (!pop_int(&percent)) return k_pop_failed;                                       // 005320BE..D0 [esp+8]
    if (percent > 0x64) percent = 0x64;                                                // 005320EA..F1 / 00532101
    else if (percent < 0) percent = 0;                                                 // 005320F3..F7
    if (damage_type < 0 || damage_type > k_damage_type_max_00746884)                   // 00532109..17
        damage_type = k_damage_type_fallback_0074685c;                                 // 00532119..1E
    set_subtype_magical(e);                                                            // 00532122..31
    recovered_effect_creator_00503a00(e, nullptr, self_id(self));                      // 00532135..3B
    seti(e, 0, damage_type);                                                           // 00532140..49
    seti(e, 1, percent);                                                               // 0053214E..57
    return push_effect(e);                                                             // 0053215C..A8
}

// ================================================================== 00535290 EffectBodyFuel() [routine 224]
// No subtype, no creator.
int __fastcall kotor_fastcall_00535290(void*, void*, int, unsigned) {
    void* e = new_effect();                                                            // 005352A7..D1
    set_effect_type(e, k_effect_body_fuel_00746800);                                   // 005352D3..DA
    return push_effect(e);                                                             // 005352D9..2F
}

// ================================================================== 00533D00 EffectSkillIncrease(nSkill, nValue) [routine 351]
// Valid skill: 0 < nSkill < byte [[007A3A28]+0ABh] (signed compare), or nSkill == 0FFh; otherwise the effect type is
// reset to invalid.  Quirk (original): skill 0 is rejected (jle); the effect is never freed after the push.
int __fastcall kotor_fastcall_00533d00(void* self, void*, int, unsigned) {
    I skill = 0, value = 0;
    if (!pop_int(&skill)) return k_pop_failed;                                         // 00533D19..2D [esp+4]
    if (!pop_int(&value)) return k_pop_failed;                                         // 00533D46..58 [esp+8]
    void* e = new_effect();                                                            // 00533D5B..85
    set_effect_type(e, k_effect_skill_increase_00746758);                              // 00533D87..94
    set_subtype_magical(e);                                                            // 00533D8E..A8
    if ((skill > 0 && skill < static_cast<I>(rules_byte(0xab))) || skill == 0xff) {    // 00533DAC..CA
        seti(e, 0, skill);                                                             // 00533DCC..D1
        seti(e, 1, value);                                                             // 00533DD6..DF
        seti(e, 2, rules_byte(0xaa));                                                  // 00533DE4..F6
    } else {
        set_effect_type(e, k_effect_invalid_0074667c);                                 // 00533DFD..03
    }
    creator_if_found(self, e);                                                         // 00533E07..3B
    return push_status(recovered_vm_outer_push_engine_structure_005d10b0(vm(), nullptr, 0, e)); // 00533E40..6C
}

// ================================================================== 005458D0 GetIsLinkImmune(oTarget, eEffect) [routine 390]
// Quirks (original): the effect pop failure returns -2000; the found object is not null-checked before its
// vtable is read; AsSWSCreature is called twice.  The popped effect is freed only after a successful push.
int __fastcall kotor_fastcall_005458d0(void*, void*, int, unsigned) {
    I result = 0;                                                                      // 005458DF xor esi,esi
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 005458DA..F3
    void* effect = nullptr;
    if (!pop_struct(0, &effect)) return k_push_failed;                                 // 005458F6..0A -> 0054596D
    void* object = nullptr;
    if (lookup(id, &object) && as_creature(object) != nullptr)                         // 0054590C..3E
        result = static_cast<I>(kotor_fastcall_005a6a90(at<void*>(as_creature(object), 0xa74), nullptr, effect)); // 00545940..5B
    if (!recovered_vm_outer_push_integer_005d1010(vm(), nullptr, result)) return k_push_failed; // 0054595D..76
    if (effect != nullptr) {                                                           // 00545979..81
        recovered_effect_destroy_00503590(effect, nullptr);                            // 00545983
        live_free(effect);                                                             // 00545988..8E
    }
    return 0;
}

// ================================================================== 00545470 VersusAlignmentEffect / VersusRacialTypeEffect / VersusTrapEffect
// [routines 355 / 356 / 357].  Pops the effect, then int A when argc > 1 and int B when argc > 2 (signed).  Validity:
// 355 needs -1 < A < 4; 356 needs 0 <= A <= 7; 357 always.  For a valid non-null effect the versus integers are
// written at type-specific indices; the (possibly modified) effect is pushed back.
// Deviation (unavoidable): the original leaves A/B uninitialised when argc is short (compiler-generated calls always
// pass every argument); here they start at 0.
int __fastcall kotor_fastcall_00545470(void*, void*, int routine, unsigned argc) {
    void* e = nullptr;
    if (!pop_struct(0, &e)) return k_push_failed;                                      // 00545479..91
    I a = 0, b = 0;
    if (static_cast<I>(argc) > 1 && !pop_int(&a)) return k_push_failed;                // 00545495..BB (jle signed)
    if (static_cast<I>(argc) > 2 && !pop_int(&b)) return k_push_failed;                // 005454BE..D5
    bool valid = true;                                                                 // 005454E5 esi = 1
    if (routine == 0x163) {                                                            // 005454DB..EA
        if (a <= -1 || a >= 4) valid = false;                                          // 005454EC..F6
    } else if (routine == 0x164) {                                                     // 005454F8..FE
        if (a < 0 || a > k_racial_type_max_0074bf18) valid = false;                    // 00545500..0D
    }
    if (e != nullptr && valid) {                                                       // 00545511..1F
        const U type = at<U16>(e, 8);                                                  // 00545525
        // per-type writes: alignment (355) -> (ia, A) then (ib, B); racial (356) -> (ir, A); trap (357) none.
        // ia < 0 means "no A write" for 355; ir < 0 means 356 does nothing; skip means the type has no versus data.
        struct Row { U type; I ia; I ib; I ir; };
        static const Row rows[] = {
            {0x0a, 3, 4, 2},   // [007466A4] 00545529..47 / 005456C0
            {0x0b, 3, 4, 2},   // [007466A8] 0054554C..6A / 0054577E
            {0x0d, 3, 4, 2},   // [007466B0] 0054556F..8D / 005457A5
            {0x0e, 3, 4, 2},   // [007466B4] 00545592..B0 / 005456C0
            {0x3f, -1, 3, 1},  // [00746778] 005455B5..DB (355 writes only (3, B))
            {0x2f, -1, 3, 1},  // [00746738] 005455E0..06 (355 writes only (3, B))
            {0x4c, 2, 3, 1},   // [007467AC] 0054560B..39
            {0x02, -2, -2, -2}, {0x0c, -2, -2, -2}, {0x10, -2, -2, -2}, {0x11, -2, -2, -2}, // 0054563E..68 no-op
            {0x16, 2, 3, 1},   // [007466D4] 0054566E..9C
            {0x30, 3, 4, 2},   // [0074673C] 005456A1..BB / 005456C0
            {0x31, 3, 4, 2},   // [00746740] 005456D2..F0 / 0054577E
            {0x21, -2, -2, -2}, {0x22, -2, -2, -2},                                       // 005456F5..07 no-op
            {0x1a, 4, 5, 3},   // [007466E4] 0054570D..3B
            {0x1b, 4, 5, 3},   // [007466E8] 0054573D..64
            {0x37, 3, 4, 2},   // [00746758] 00545766..7C / 0054577E
            {0x38, 3, 4, 2},   // [0074675C] 0054578D..A3 / 005457A5
        };
        for (const Row& r : rows) {
            if (type != r.type) continue;
            if (r.ir == -2) break;                                                     // je 005457C1
            if (routine == 0x163) {                                                    // sub edx,163h; je
                if (r.ia >= 0) seti(e, static_cast<U>(r.ia), a);
                seti(e, static_cast<U>(r.ib), b);                                      // 005457B2..B8 / 005455D4..DB
            } else if (routine == 0x164) {                                             // dec edx; jne 005457C1
                seti(e, static_cast<U>(r.ir), a);                                      // 005457B8
            }
            break;
        }
    }
    if (!recovered_vm_outer_push_engine_structure_005d10b0(vm(), nullptr, 0, e)) return k_push_failed; // 005457C1..D1
    if (e != nullptr) {                                                                // 005457D7..DF
        recovered_effect_destroy_00503590(e, nullptr);
        live_free(e);
    }
    return 0;                                                                          // 005457EF
}

// ================================================================== 00532350 EffectDamageReduction(nAmount, nDamagePower, nLimit) [routine 119]
// All three pops unconditional.  Negative amount / limit become 0 (setl/dec/and).
int __fastcall kotor_fastcall_00532350(void* self, void*, int, unsigned) {
    I amount = 0, power = 0, limit = 0;
    if (!pop_int(&amount)) return k_pop_failed;                                        // 00532369..7D [esp+4]
    if (!pop_int(&power)) return k_pop_failed;                                         // 0053237F..91 [esp+8]
    if (!pop_int(&limit)) return k_pop_failed;                                         // 00532393..A5 [esp+0C]
    if (power < 0 || power > k_damage_power_max_007468ac) power = k_damage_power_fallback_00746894; // 005323BE..D3
    amount = amount < 0 ? 0 : amount;                                                  // 005323D7..F6
    limit = limit < 0 ? 0 : limit;                                                     // 005323ED..FD
    void* e = new_effect();                                                            // 005323E5..26
    set_effect_type(e, k_effect_damage_reduction_007466ac);                            // 00532428..34
    set_subtype_magical(e);                                                            // 0053242E..49
    seti(e, 0, amount);                                                                // 0053244D..56
    seti(e, 1, power);                                                                 // 0053245B..64
    seti(e, 2, limit);                                                                 // 00532469..72
    creator_if_found(self, e);                                                         // 00532477..AB
    return push_effect(e);                                                             // 005324B0..FC
}

// ================================================================== 00532790 EffectDamageShield(nDamageAmount, nRandomAmount, nDamageType) [routine 487]
// SetCreator is unconditional and precedes the subtype store.
int __fastcall kotor_fastcall_00532790(void* self, void*, int, unsigned) {
    I amount = 0, random = 0, damage_type = 0;
    if (!pop_int(&amount)) return k_pop_failed;                                        // 005327A9..BD
    if (!pop_int(&random)) return k_pop_failed;                                        // 005327BF..D1
    if (!pop_int(&damage_type)) return k_pop_failed;                                   // 005327D3..E5
    if (amount < 0 || amount > 0x2710) amount = 1;                                     // 005327FE..0D
    if (random <= 0 || random > 0xa) random = 0;                                       // 00532815..22
    if (damage_type < 0 || damage_type > k_damage_type_max_00746884)                   // 0053282A..38
        damage_type = k_damage_type_fallback_0074685c;                                 // 0053283A..3F
    void* e = new_effect();                                                            // 00532843..6E
    set_effect_type(e, k_effect_damage_shield_00746770);                               // 00532870..77
    recovered_effect_creator_00503a00(e, nullptr, self_id(self));                      // 0053287B..89
    set_subtype_magical(e);                                                            // 0053288E..9C
    seti(e, 0, amount);                                                                // 005328A0..A9
    seti(e, 1, random);                                                                // 005328AE..B7
    seti(e, 2, damage_type);                                                           // 005328BC..C5
    return push_effect(e);                                                             // 005328CA..16
}

// ================================================================== 00531EF0 EffectDamageImmunityDecrease(nDamageType, nPercent) [routine 449]
// As the increase, but the percent clamps to [-100, 100].
int __fastcall kotor_fastcall_00531ef0(void* self, void*, int, unsigned) {
    void* e = new_effect();                                                            // 00531F0A..36
    set_effect_type(e, k_effect_damage_immunity_decrease_007466c0);                    // 00531F38..42
    I damage_type = 0, percent = 0;
    if (!pop_int(&damage_type)) return k_pop_failed;                                   // 00531F3E..5C [esp+0C]
    if (!pop_int(&percent)) return k_pop_failed;                                       // 00531F5E..70 [esp+8]
    if (percent > 0x64) percent = 0x64;                                                // 00531F8A..91 / 00531FA2
    else if (percent < -0x64) percent = -0x64;                                         // 00531F93..98
    if (damage_type < 0 || damage_type > k_damage_type_max_00746884)                   // 00531FAA..B8
        damage_type = k_damage_type_fallback_0074685c;                                 // 00531FBA..BF
    set_subtype_magical(e);                                                            // 00531FC3..D2
    recovered_effect_creator_00503a00(e, nullptr, self_id(self));                      // 00531FD6..DC
    seti(e, 0, damage_type);                                                           // 00531FE1..EA
    seti(e, 1, percent);                                                               // 00531FEF..F8
    return push_effect(e);                                                             // 00531FFD..49
}

// ================================================================== 00534DB0 EffectBlasterDeflectionDecrease(nChange) [routine 470]
// No subtype, no creator.
int __fastcall kotor_fastcall_00534db0(void*, void*, int, unsigned) {
    I change = 0;
    if (!pop_int(&change)) return k_pop_failed;                                        // 00534DCE..DA
    void* e = new_effect();                                                            // 00534DF2..1D
    set_effect_type(e, k_effect_blaster_deflection_decrease_007467ec);                 // 00534E1F..26
    seti(e, 0, change);                                                                // 00534E2A..3B
    return push_effect(e);                                                             // 00534E40..8A
}

}  // extern "C"
