// Lane vmC, part A "effects": NWScript engine-command handlers CSWVirtualMachineCommands::ExecuteCommand* that build
// or inspect CGameEffect / CScriptEvent engine structures, reconstructed from the original swkotor.exe (read-only,
// capstone linear sweep).  Addresses in the comments are original instructions.
//  * thiscall(int routine, int argc) RET 8 (verified for every handler) ->
//    extern "C" int __fastcall kotor_fastcall_<addr>(self, edx, routine, argc)  (@16)
//  * CGameEffect (operator new 8Ch, ctor(1) 00503E40): +8 WORD type, +0A WORD flags (bits 0..2 duration type,
//    bits 3/4 sub-type), +0C float duration, +18 creator id, +20 dword (set to 1 by SetEffectIcon).
//  * Engine structure types: 0 effect, 1 event, 2 location.
//  * Original quirks kept (documented per handler): operator new failure is not checked before the first field
//    store (null dereference); a failed StackPushEngineStructure leaks the new effect/event; ApplyEffectAtLocation
//    releases popped effect/location with a raw free (no destructor) on a pop failure; EffectImmunity pops its
//    argument after building the effect and leaks it on a pop failure; EffectLightsaberThrow pops and ignores its
//    fourth (integer) argument.
// 0052E430 ApplyEffectAtLocation (pending lane "aoe") lives in claude_vm_vmC_pending.cpp.
#include "1e78b2f608-claude_vm_vmc_common.hpp"

namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); }  // CExoString() [menu20_bindings]

extern "C" {
// ---- CGameEffect providers (decorated arity checked against the original RET) ----
void* __fastcall recovered_effect_construct_00503e40(void* effect, void*, U flag);        // CGameEffect::CGameEffect(int) RET 4
void* __fastcall kotor_fastcall_00503f70(void* effect, void*, void* source);              // CGameEffect::CGameEffect(CGameEffect*) RET 4 [claude_w1_route_bodies]
void __fastcall recovered_effect_integer_005036a0(void* effect, void*, U index, U value);  // SetInteger RET 8
void __fastcall kotor_fastcall_005036d0(void* effect, void*, U index, U value_bits);      // SetFloat RET 8 [claude_w1_chains_damage]
void __fastcall recovered_effect_set_object_id_005036f0(void* effect, void*, U index, U id); // SetObjectID RET 8
void __fastcall kotor_fastcall_00503730(void* effect, void*, U index, const void* string); // SetString RET 8
void __fastcall recovered_effect_creator_00503a00(void* effect, void*, U id);             // SetCreator RET 4
void __fastcall recovered_effect_destroy_00503590(void* effect, void*);                   // ~CGameEffect
void __fastcall kotor_fastcall_00504810(void* effect, void*, void* child, void* parent);  // SetLinked RET 8 [claude_combat_attack_damage]
// ---- other providers ----
void* __fastcall recovered_event_construct_004d7540(void* event, void*);                  // CScriptEvent::CScriptEvent
void __fastcall recovered_event_destroy_004d7590(void* event, void*);                     // CScriptEvent::~CScriptEvent
U __fastcall kotor_fastcall_004aea40(void* server, void*);                                // CServerExoApp::GetPlayerCreatureId
void* __fastcall recovered_object_area_004cb120(void* object, void*);                     // CSWSObject::GetArea
U __fastcall recovered_stats_effect_query_005a6960(void* stats, void*, U type, void* versus); // CSWSCreatureStats::GetEffectImmunity RET 8
void* __fastcall recovered_array_copy_004fdd30(void* self, void*, const void* other);     // CExoArrayList<CExoString*> copy ctor RET 4
double __cdecl vector_magnitude_004ab100(const Vector* v);                                // Vector magnitude (ST0)
extern void* kotor_cell_rules_007a3a28;                                                    // [007A3A28] CSWRules*
}

namespace {
using namespace vmc;
// .rdata effect type words
constexpr U k_effect_invalid_0074667c = 0;
constexpr U k_effect_attack_decrease_007466a8 = 0x0b;
constexpr U k_effect_immunity_007466d4 = 0x16;
constexpr U k_effect_saving_throw_increase_007466e4 = 0x1a;
constexpr U k_effect_area_of_effect_007466f8 = 0x1f;
constexpr U k_effect_poison_00746708 = 0x23;
constexpr U k_effect_link_0074671c = 0x28;
constexpr U k_effect_modify_attacks_0074672c = 0x2c;
constexpr U k_effect_hp_change_when_dying_00746760 = 0x39;
constexpr U k_effect_force_pushed_0074676c = 0x3c;
constexpr U k_effect_icon_00746788 = 0x43;
constexpr U k_effect_blaster_deflection_increase_007467e8 = 0x5c;
constexpr U k_effect_psychic_static_00746804 = 0x63;
constexpr U k_effect_lightsaber_throw_00746808 = 0x64;
constexpr U k_effect_force_jump_00746810 = 0x66;
constexpr I k_attack_bonus_max_007468dc = 7;        // .rdata 007468DC
constexpr I k_attack_bonus_misc_007468c4 = 0;       // .rdata 007468C4
constexpr I k_immunity_max_00746978 = 0x21;         // .rdata 00746978
constexpr U16 k_duration_permanent_007473b8 = 0;    // .rdata 007473B8 (word)

// operator new(8Ch) + CGameEffect(1); a null allocation yields a null effect that the caller then dereferences
inline void* new_effect() {
    void* memory = live_operator_new(0x8c);
    return memory ? recovered_effect_construct_00503e40(memory, nullptr, 1) : nullptr;
}
inline void set_type(void* e, U type) { at<U16>(e, 8) = static_cast<U16>(type); }
// xor r,r; mov r16,[e+0A]; and r,FFEF; or r,8; mov [e+0A],r16
inline void set_subtype_magical(void* e) { at<U16>(e, 0xa) = static_cast<U16>((at<U16>(e, 0xa) & 0xffefu) | 8u); }
inline void destroy_effect(void* e) { recovered_effect_destroy_00503590(e, nullptr); live_free(e); }
// StackPushEngineStructure(0, e): failure -> -2000 (effect leaked); success -> ~CGameEffect + free, 0
inline int push_effect(void* e) {
    if (!push_engine_raw(0, e)) return k_push_failed;
    destroy_effect(e);
    return 0;
}
// GetObjectArray()->GetGameObject(OBJECT_SELF) found and non-null -> SetCreator(OBJECT_SELF)
inline void creator_if_self_exists(void* self, void* e) {
    void* object = nullptr;
    if (lookup(self_id(self), &object) && object != nullptr) recovered_effect_creator_00503a00(e, nullptr, self_id(self));
}
inline U rules_difficulty_byte() { return at<U8>(kotor_cell_rules_007a3a28, 0xaa); }      // movzx [[7A3A28]+0AAh]
// 004015A0 is a bare `ret` (CScriptLocation trivial dtor), written as a file-local no-op as the sibling lane files do.
inline void location_dtor_004015a0(void*) {}
inline void free_location(void* l) { if (l != nullptr) { location_dtor_004015a0(l); live_free(l); } }
inline void exo_construct(ExoString* s) {
    (void)kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(s), nullptr);
}
}  // namespace

extern "C" {

// ===================================================================================================== 005475A0
// SetEffectIcon(effect eEffect, int nIcon) [routine 552]: returns a link effect (copy of eEffect linked with an
// icon effect).  If a copy allocation fails the popped effect is pushed back unchanged.
int __fastcall kotor_fastcall_005475a0(void* self, void*, int, unsigned) {
    void* source; I icon;
    if (!pop_engine(0, &source)) return k_pop_failed;                                   // 005475B9..D3
    if (!pop_int(&icon)) {                                                              // 005475F1..03
        if (source != nullptr) destroy_effect(source);                                  // 00547605..16
        return k_pop_failed;
    }
    void* result = source;                                                              // ebx
    void* memory = live_operator_new(0x8c);                                             // 00547632..3C
    void* icon_effect = memory ? kotor_fastcall_00503f70(memory, nullptr, source) : nullptr; // 00547643..5B
    if (icon_effect != nullptr) {                                                       // 0054765D..67
        set_type(icon_effect, k_effect_icon_00746788);                                  // 0054766D..7A
        at<U>(icon_effect, 0x20) = 1;                                                   // 0054767E
        recovered_effect_creator_00503a00(icon_effect, nullptr, self_id(self));         // 00547681..87
        at<U>(icon_effect, 0x20) = 1;                                                   // 0054768C
        recovered_effect_integer_005036a0(icon_effect, nullptr, 0, static_cast<U>(icon)); // 0054768F..98
        void* memory2 = live_operator_new(0x8c);                                        // 0054769D..A7
        void* link = memory2 ? kotor_fastcall_00503f70(memory2, nullptr, source) : nullptr; // 005476AE..C2
        if (link != nullptr) {                                                          // 005476C4..CE
            set_type(link, k_effect_link_0074671c);                                     // 005476D0..DB
            at<U>(link, 0x20) = 1;                                                      // 005476DF
            recovered_effect_creator_00503a00(link, nullptr, self_id(self));            // 005476E2..E8
            kotor_fastcall_00504810(link, nullptr, icon_effect, source);                // 005476ED..F1
            result = link;                                                              // 005476F6
        } else {
            destroy_effect(icon_effect);                                                // 005476FA..07
        }
    }
    if (!push_engine_raw(0, result)) return k_push_failed;                              // 0054770B..33 (result leaked)
    if (result != nullptr) destroy_effect(result);                                      // 00547736..47
    return 0;
}

// ===================================================================================================== 00533560
// EffectPoison(int nPoisonType) [routine 250]
int __fastcall kotor_fastcall_00533560(void* self, void*, int, unsigned) {
    I poison;
    if (!pop_int(&poison)) return k_pop_failed;                                         // 00533579..A3
    void* e = new_effect();                                                             // 005335A7..D1
    set_subtype_magical(e);                                                             // 005335D3..EA
    recovered_effect_creator_00503a00(e, nullptr, self_id(self));                       // 005335EE..F4
    set_type(e, k_effect_poison_00746708);                                              // 005335F9..FF
    recovered_effect_integer_005036a0(e, nullptr, 0, static_cast<U>(poison));           // 00533603..0C
    return push_effect(e);                                                              // 00533611..5D
}

// ===================================================================================================== 00533910
// EffectSavingThrowIncrease(int nSave, int nValue, int nSaveType=SAVING_THROW_TYPE_ALL) [routine 117]
int __fastcall kotor_fastcall_00533910(void* self, void*, int, unsigned argc) {
    I save, value, save_type = 0;                                                       // 00533936 save_type = 0
    if (!pop_int(&save)) return k_pop_failed;                                           // 00533929..45
    if (save < 0 || save >= 4) save = 0;                                                // 00533947..54
    if (!pop_int(&value)) return k_pop_failed;                                          // 0053395C..6E
    if (static_cast<I>(argc) > 2 && !pop_int(&save_type)) return k_pop_failed;          // 00533970..89 (jle)
    void* e = new_effect();                                                             // 005339A3..CD
    set_type(e, k_effect_saving_throw_increase_007466e4);                               // 005339CF..DB
    set_subtype_magical(e);                                                             // 005339D5..F0
    recovered_effect_integer_005036a0(e, nullptr, 0, static_cast<U>(value));            // 005339F4..FD
    recovered_effect_integer_005036a0(e, nullptr, 1, static_cast<U>(save));             // 00533A02..0B
    recovered_effect_integer_005036a0(e, nullptr, 2, static_cast<U>(save_type));        // 00533A10..19
    recovered_effect_integer_005036a0(e, nullptr, 3, rules_difficulty_byte());          // 00533A1E..30
    creator_if_self_exists(self, e);                                                    // 00533A35..69
    return push_effect(e);                                                              // 00533A6E..BA
}

// ===================================================================================================== 005353F0
// EffectLightsaberThrow(object oTarget1, object oTarget2=OBJECT_INVALID, object oTarget3=OBJECT_INVALID,
// int nAdvancedDamage=0) [routine 702]: the integer is popped but not stored (original).
int __fastcall kotor_fastcall_005353f0(void*, void*, int, unsigned argc) {
    U target1 = k_invalid, target2 = k_invalid, target3 = k_invalid;                    // 0053540E..1B
    I advanced = 0;                                                                     // 00535424
    if (!pop_object(&target1)) return k_pop_failed;                                     // 0053541F..48
    const I n = static_cast<I>(argc);                                                   // 0053544C
    if (n >= 2 && !pop_object(&target2)) return k_pop_failed;                           // 00535450..7D
    if (n >= 3 && !pop_object(&target3)) return k_pop_failed;                           // 00535480..97
    if (n >= 4 && !pop_int(&advanced)) return k_pop_failed;                             // 00535499..B0
    (void)advanced;
    void* e = new_effect();                                                             // 005354B2..DC
    set_type(e, k_effect_lightsaber_throw_00746808);                                    // 005354DE..E5 (flags untouched)
    recovered_effect_set_object_id_005036f0(e, nullptr, 0, target1);                    // 005354E9..FA
    recovered_effect_set_object_id_005036f0(e, nullptr, 1, target2);                    // 005354FF..08
    recovered_effect_set_object_id_005036f0(e, nullptr, 2, target3);                    // 0053550D..16
    return push_effect(e);                                                              // 0053551B..65
}

// ===================================================================================================== 00530F60
// EffectAttackDecrease(int nPenalty, int nModifierType=ATTACK_BONUS_MISC) [routine 447]: both pops unconditional.
int __fastcall kotor_fastcall_00530f60(void* self, void*, int, unsigned) {
    I penalty, modifier;
    if (!pop_int(&penalty)) return k_pop_failed;                                        // 00530F79..8D
    if (!pop_int(&modifier)) return k_pop_failed;                                       // 00530F8F..A1
    if (modifier < 0 || modifier > k_attack_bonus_max_007468dc)                         // 00530FBA..C8
        modifier = k_attack_bonus_misc_007468c4;                                        // 00530FCA..D0
    void* e = new_effect();                                                             // 00530FD5..FF
    set_type(e, k_effect_attack_decrease_007466a8);                                     // 00531001..0D
    set_subtype_magical(e);                                                             // 00531007..22
    recovered_effect_integer_005036a0(e, nullptr, 0, static_cast<U>(penalty));          // 00531026..2F
    recovered_effect_integer_005036a0(e, nullptr, 1, static_cast<U>(modifier));         // 00531034..3D
    recovered_effect_integer_005036a0(e, nullptr, 2, rules_difficulty_byte());          // 00531042..54
    creator_if_self_exists(self, e);                                                    // 00531059..8C
    return push_effect(e);                                                              // 00531091..DD
}

// ===================================================================================================== 00532D00
// EffectImmunity(int nImmunityType) [routine 273]: the effect is built before the pop (leaked on a pop failure).
int __fastcall kotor_fastcall_00532d00(void* self, void*, int, unsigned) {
    void* e = new_effect();                                                             // 00532D1A..46
    set_type(e, k_effect_immunity_007466d4);                                            // 00532D48..4E
    recovered_effect_creator_00503a00(e, nullptr, self_id(self));                       // 00532D52..60
    set_subtype_magical(e);                                                             // 00532D65..79
    I type;
    if (!pop_int(&type)) return k_pop_failed;                                           // 00532D6B..A1 (effect leaked)
    // 00532DA4..C4: ecx = min(max, type); ecx < 0 -> 0; else max < type -> max
    const I limit = k_immunity_max_00746978;
    const I smaller = limit < type ? limit : type;
    if (smaller < 0) type = 0;
    else if (limit < type) type = limit;
    recovered_effect_integer_005036a0(e, nullptr, 0, static_cast<U>(type));             // 00532DC8..CD
    recovered_effect_integer_005036a0(e, nullptr, 1, rules_difficulty_byte());          // 00532DD2..E4
    return push_effect(e);                                                              // 00532DE9..35
}

// ===================================================================================================== 005331C0
// EffectModifyAttacks(int nAttacks) [routine 485]: more than 5 attacks -> EFFECT_INVALID (no creator/flags).
int __fastcall kotor_fastcall_005331c0(void* self, void*, int, unsigned) {
    I attacks;
    if (!pop_int(&attacks)) return k_pop_failed;                                        // 005331D9..03
    void* e = new_effect();                                                             // 00533207..31
    if (attacks > 5) {                                                                  // 00533233..40 (jle)
        set_type(e, k_effect_invalid_0074667c);                                         // 00533242..49
    } else {
        set_type(e, k_effect_modify_attacks_0074672c);                                  // 0053324F..56
        recovered_effect_creator_00503a00(e, nullptr, self_id(self));                   // 0053325A..60
        set_subtype_magical(e);                                                         // 00533265..74
        recovered_effect_integer_005036a0(e, nullptr, 0, static_cast<U>(attacks));      // 00533278..81
    }
    return push_effect(e);                                                              // 00533286..D2
}

// ===================================================================================================== 005349F0
// EffectForcePushed() [routine 392]
int __fastcall kotor_fastcall_005349f0(void* self, void*, int, unsigned) {
    void* e = new_effect();                                                             // 00534A08..34
    set_type(e, k_effect_force_pushed_0074676c);                                        // 00534A36..3C (flags untouched)
    recovered_effect_creator_00503a00(e, nullptr, self_id(self));                       // 00534A40..4E
    return push_effect(e);                                                              // 00534A53..9F
}

// ===================================================================================================== 00535340
// EffectPsychicStatic() [routine 676]
int __fastcall kotor_fastcall_00535340(void*, void*, int, unsigned) {
    void* e = new_effect();                                                             // 00535357..81
    set_type(e, k_effect_psychic_static_00746804);                                      // 00535383..8A
    return push_effect(e);                                                              // 0053538E..DF
}

// ===================================================================================================== 00534CD0
// EffectBlasterDeflectionIncrease(int nChange) [routine 469]
int __fastcall kotor_fastcall_00534cd0(void*, void*, int, unsigned) {
    I change;
    if (!pop_int(&change)) return k_pop_failed;                                         // 00534CEE..0F
    void* e = new_effect();                                                             // 00534D13..3D
    set_type(e, k_effect_blaster_deflection_increase_007467e8);                         // 00534D3F..46
    recovered_effect_integer_005036a0(e, nullptr, 0, static_cast<U>(change));           // 00534D4A..5B
    return push_effect(e);                                                              // 00534D60..AA
}

// ===================================================================================================== 00537AA0
// GetEffectCreator(effect eEffect) [routine 91]
int __fastcall kotor_fastcall_00537aa0(void*, void*, int, unsigned) {
    void* e = nullptr;                                                                  // 00537AAF
    U creator = k_invalid;                                                              // 00537AB7
    if (!pop_engine(0, &e)) return k_pop_failed;                                        // 00537AA8..CC
    if (e != nullptr) creator = at<U>(e, 0x18);                                         // 00537ACF..D7
    if (!recovered_vm_outer_push_object_005d10d0(vm(), nullptr, creator)) return k_push_failed; // 00537ADA..F1 (effect leaked)
    if (e != nullptr) destroy_effect(e);                                                // 00537AF4..09
    return 0;
}

// ===================================================================================================== 00537C00
// GetEffectSubType(effect eEffect) [routine 90]: flags byte & 18h (no null check, original)
int __fastcall kotor_fastcall_00537c00(void*, void*, int, unsigned) {
    void* e = nullptr;                                                                  // 00537C0E
    if (!pop_engine(0, &e)) return k_pop_failed;                                        // 00537C07..25
    const I subtype = static_cast<I>(at<U8>(e, 0xa) & 0x18u);                           // 00537C28..37
    if (!recovered_vm_outer_push_integer_005d1010(vm(), nullptr, subtype)) return k_push_failed; // 00537C3A..4A (leak)
    if (e != nullptr) destroy_effect(e);                                                // 00537C4D..63
    return 0;
}

// ===================================================================================================== 00539C80
// GetIsImmune(object oCreature, int nImmunityType, object oVersus=OBJECT_INVALID) [routine 274]
int __fastcall kotor_fastcall_00539c80(void*, void*, int, unsigned argc) {
    U target, versus; I type; I result = 0;                                             // 00539C8F edi = 0
    if (!pop_object(&target)) return k_pop_failed;                                      // 00539C8A..A3
    if (!pop_int(&type)) return k_pop_failed;                                           // 00539CA6..B8
    versus = k_invalid;                                                                 // 00539CBF
    if (static_cast<I>(argc) >= 3 && !pop_object(&versus)) return k_pop_failed;         // 00539CBA..DB
    void* versus_creature = creature_by_id(versus);                                     // 00539CDD..F0
    void* object = nullptr;
    if (lookup(target, &object) && as_creature(object) != nullptr &&                    // 00539CF2..23
        type > 0 && type < k_immunity_max_00746978) {                                   // 00539D25..33
        void* stats = at<void*>(as_creature(object), 0xa74);                            // 00539D35..42
        result = static_cast<I>(recovered_stats_effect_query_005a6960(stats, nullptr, static_cast<U>(type), versus_creature)); // 00539D48..51
    }
    return push_int(result);                                                            // 00539D53..72
}

// ===================================================================================================== 00530CE0
// EffectAreaOfEffect(int nAreaEffectId, string sOnEnterScript="", string sHeartbeatScript="",
// string sOnExitScript="") [routine 171]
int __fastcall kotor_fastcall_00530ce0(void* self, void*, int, unsigned) {
    ExoString on_enter, heartbeat, on_exit;                                             // [esp+28] [esp+20] [esp+18]
    exo_construct(&on_enter);                                                           // 00530D03..07
    exo_construct(&heartbeat);                                                          // 00530D0E..16
    exo_construct(&on_exit);                                                            // 00530D1B..24
    I id;
    I status = k_pop_failed;
    if (pop_int(&id) && pop_string(&on_enter) && pop_string(&heartbeat) && pop_string(&on_exit)) { // 00530D29..88
        void* e = new_effect();                                                         // 00530D8E..B5
        set_type(e, k_effect_area_of_effect_007466f8);                                  // 00530DB7..C4
        set_subtype_magical(e);                                                         // 00530DBE..D8
        recovered_effect_integer_005036a0(e, nullptr, 0, static_cast<U>(id));           // 00530DDC..E2
        kotor_fastcall_005036d0(e, nullptr, 0, 0);                                      // 00530DE7..EB
        kotor_fastcall_005036d0(e, nullptr, 1, 0);                                      // 00530DF0..F5
        kotor_fastcall_005036d0(e, nullptr, 2, 0);                                      // 00530DFA..FF
        kotor_fastcall_00503730(e, nullptr, 0, &on_enter);                              // 00530E04..0C
        kotor_fastcall_00503730(e, nullptr, 1, &heartbeat);                             // 00530E11..1A
        kotor_fastcall_00503730(e, nullptr, 2, &on_exit);                               // 00530E1F..28
        creator_if_self_exists(self, e);                                                // 00530E2D..5F
        status = push_effect(e);                                                        // 00530E64..C7 (leak on failure)
    }
    exo_destroy(&on_exit);                                                              // 00530E75 / 00530ECA / 00530F0C
    exo_destroy(&heartbeat);
    exo_destroy(&on_enter);
    return status;
}

// ===================================================================================================== 00532BD0
// EffectHitPointChangeWhenDying(float fHitPointChangePerRound) [routine 387]
int __fastcall kotor_fastcall_00532bd0(void* self, void*, int, unsigned) {
    float change;
    if (!pop_float(&change)) return k_pop_failed;                                       // 00532BE9..13
    void* e = new_effect();                                                             // 00532C17..41
    // 00532C43..5A: fcomp 0.0; test ah,44h; jp -> taken unless equal (unordered also takes the jump)
    if (change == 0.0f) {
        set_type(e, k_effect_invalid_0074667c);                                         // 00532C5C..63
    } else {
        recovered_effect_creator_00503a00(e, nullptr, self_id(self));                   // 00532C69..6F
        set_type(e, k_effect_hp_change_when_dying_00746760);                            // 00532C74..80
        at<U16>(e, 0xa) = static_cast<U16>((at<U16>(e, 0xa) & 0xfff8u) | k_duration_permanent_007473b8); // 00532C7A..91
        kotor_fastcall_005036d0(e, nullptr, 0, bits(change));                           // 00532C95..9E
    }
    return push_effect(e);                                                              // 00532CA3..EF
}

// ===================================================================================================== 005348C0
// EffectForceJump(object oTarget, int nAdvanced=0) [routine 153]
int __fastcall kotor_fastcall_005348c0(void* self, void*, int, unsigned argc) {
    U target = k_invalid;                                                               // 005348E6
    if (!pop_object(&target)) return k_pop_failed;                                      // 005348D9..F5
    I advanced = 0;                                                                     // 005348FC
    if (static_cast<I>(argc) >= 2 && !pop_int(&advanced)) return k_pop_failed;          // 005348F7..18
    void* e = new_effect();                                                             // 00534932..5C
    set_type(e, k_effect_force_jump_00746810);                                          // 0053495E..65 (flags untouched)
    recovered_effect_set_object_id_005036f0(e, nullptr, 0, target);                     // 00534969..7A
    recovered_effect_integer_005036a0(e, nullptr, 0, static_cast<U>(advanced));         // 0053497F..88
    recovered_effect_creator_00503a00(e, nullptr, self_id(self));                       // 0053498D..93
    return push_effect(e);                                                              // 00534998..E4
}

// ===================================================================================================== 005376C0
// GetDistanceBetweenLocations2D(location lLocationA, location lLocationB) [routine 334]
int __fastcall kotor_fastcall_005376c0(void*, void*, int, unsigned) {
    void* a; void* b;
    if (!pop_engine(2, &a)) return k_pop_failed;                                        // 005376C9..E1
    if (!pop_engine(2, &b)) { free_location(a); return k_pop_failed; }                  // 005376E5..1C
    Vector d;
    d.x = static_cast<float>(static_cast<double>(at<float>(a, 0)) - static_cast<double>(at<float>(b, 0))); // 0053771F..4F fstp
    d.y = static_cast<float>(static_cast<double>(at<float>(a, 4)) - static_cast<double>(at<float>(b, 4))); // 00537753..68 fstp
    d.z = 0.0f;                                                                         // 0053776C
    const float distance = static_cast<float>(vector_magnitude_004ab100(&d));           // 00537774..79 fstp dword
    const U pushed = recovered_vm_outer_push_float_005d1030(vm(), nullptr, distance);   // 0053777D..8B
    free_location(a);                                                                   // 00537798..A9 / 005377D8..E9
    free_location(b);                                                                   // 005377B4..C9 / 005377F4..09
    return pushed ? 0 : k_push_failed;                                                  // 005377CC / 0053780C
}

// ===================================================================================================== 0054C830
// GetMatchedSubstring(int nString) [routine 178]: string nString of OBJECT_SELF's matched-string list (+1A0)
int __fastcall kotor_fastcall_0054c830(void* self, void*, int, unsigned) {
    ExoString result;
    exo_construct(&result);                                                             // 0054C84B..4F
    I index;
    if (!pop_int(&index)) { exo_destroy(&result); return k_pop_failed; }                // 0054C85A..95
    void* object = nullptr;
    if (lookup(self_id(self), &object) && object != nullptr) {                          // 0054C898..C4
        struct { void** data; I count; I allocated; } copy;                             // CExoArrayList<CExoString*> [esp+14]
        recovered_array_copy_004fdd30(&copy, nullptr, static_cast<unsigned char*>(object) + 0x1a0); // 0054C8C6..D0
        if (index >= 0 && index < copy.count)                                           // 0054C8D5..EA
            async_assign_string_005e5c50(&result, nullptr, copy.data[index]);           // 0054C8EC..F4
        live_free(copy.data);                                                           // 0054C8F9..04
    }
    const U pushed = recovered_vm_outer_push_string_005d1090(vm(), nullptr, &result);   // 0054C907..12
    exo_destroy(&result);                                                               // 0054C927 / 0054C943
    return pushed ? 0 : k_push_failed;
}

// ===================================================================================================== 005358C0
// EventConversation() [routine 295]: CScriptEvent type 7
int __fastcall kotor_fastcall_005358c0(void*, void*, int, unsigned) {
    void* memory = live_operator_new(0x34);                                             // 005358D7..DE
    void* event = memory ? recovered_event_construct_004d7540(memory, nullptr) : nullptr; // 005358E5..F6
    at<U16>(event, 0) = 7;                                                              // 005358F9 (null store if allocation failed)
    if (!push_engine_raw(1, event)) return k_push_failed;                               // 005358F8..2B (event leaked)
    recovered_event_destroy_004d7590(event, nullptr);                                   // 0053592E..30
    live_free(event);                                                                   // 00535935..3B
    return 0;
}

}  // extern "C"
