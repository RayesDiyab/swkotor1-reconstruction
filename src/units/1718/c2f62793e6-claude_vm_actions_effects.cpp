// Lane endar-actions, group "effects": NWScript engine-command bodies CSWVirtualMachineCommands::ExecuteCommand*
// that build / inspect CGameEffect engine structures, reconstructed from the original swkotor.exe (read-only,
// capstone linear sweep).  Addresses in the comments are the original instructions.
//  * thiscall(int routine, int argc) RET 8 -> extern "C" int __fastcall recovered_vm_<name>_<addr>(self, edx, routine, argc)
//  * CSWVirtualMachineCommands: +4 "run-script object valid" flag, +8 OBJECT_SELF id of the running script.
//  * CGameEffect (operator new 8Ch, ctor(1)): +8 WORD type, +0A WORD flags (bits 0..2 duration type, bit 3/4 sub-type),
//    +0C float duration, +34 int* integer array.  Engine structure 0 = effect, 2 = location.
//  * Original quirks kept: a failed operator new is not checked before the flag store (null dereference); a failed
//    StackPushEngineStructure leaks the new effect; EffectLinkEffects returns -2000 (not -2001) on a pop failure;
//    EffectDamage defaults the damage type to 3 and indexes its damage slot by round(log2(type)).
// Not implemented here (cascade too large, see report): 0052E430 ApplyEffectAtLocation (needs CSWSArea::ApplyEffect
// 0050C6B0 and the server CSWSAreaOfEffectObject class).
#include "061872cb05-claude_vm_actions_common.hpp"

extern "C" {
// ---- CGameEffect providers (exact decorated arity checked against the original RET) ----
void* __fastcall recovered_effect_construct_00503e40(void* effect, void*, U flag);        // CGameEffect::CGameEffect RET 4 [0886-cohort-patched]
void __fastcall recovered_effect_integer_005036a0(void* effect, void*, U index, U value);  // SetInteger RET 8 [0886-cohort-patched]
void __fastcall recovered_effect_creator_00503a00(void* effect, void*, U id);             // SetCreator RET 4 [0886-cohort-patched]
void __fastcall recovered_effect_destroy_00503590(void* effect, void*);                   // ~CGameEffect [w1_native_b7_0056cbf0]
void __fastcall recovered_effect_resize_00503630(void* effect, void*, U count);           // SetNumIntegers RET 4 [1091-effect_copy_native]
void __fastcall recovered_effect_set_object_id_005036f0(void* effect, void*, U index, U id); // SetObjectID RET 8 [1194u]
void __fastcall kotor_fastcall_005036d0(void* effect, void*, U index, U value_bits);      // SetFloat RET 8 [claude_w1_chains_damage]
void __fastcall kotor_fastcall_00504810(void* effect, void*, void* child, void* parent);  // SetLinked RET 8 [claude_combat_attack_damage]
void __fastcall kotor_fastcall_00504240(void* effect, void*);                             // UpdateLinked [claude_combat_attack_damage]
// ---- objects ----
unsigned __fastcall recovered_object_has_spell_effect_applied_004cdac0(void* object, void*, unsigned spell); // RET 4 [w1_native_b7_0056cbf0]
void __fastcall kotor_fastcall_004d0940(void* object, void*, U keep_own);                 // CSWSObject::ClearAllEffects RET 4 [claude_w5_creature_actions]
// ---- scratch dword the original FISTPs through (.bss 008327E0) ----
extern int recovered_effect_rank_scratch_008327e0;                                         // [0886-cohort-patched]
// ---- this group's export used by GetEffectType ----
U __fastcall kotor_fastcall_00503a70(void* effect, void*);
}
// 004015A0 is a bare `ret` (CScriptLocation trivial dtor); its only map provider is a 0486 static, so the call is
// written as a file-local no-op (no address credit), as the queries/world/movement files do.
static void location_dtor_004015a0(void*) {}
namespace w2 { namespace g07 { unsigned __cdecl is_beam_type_004ca960(unsigned type); } } // CLoopingVisualEffect::GetIsBeam (static) [w2_g07_object]

using namespace vmx;

namespace {
constexpr U k_effect_invalid_0074667c = 0;       // .rdata 0074667C
constexpr U k_effect_resurrection_0074668c = 4;  // .rdata 0074668C
constexpr U k_effect_choke_0074669c = 8;         // .rdata 0074669C
constexpr U k_effect_visual_007466f4 = 0x1e;     // .rdata 007466F4
constexpr U k_effect_beam_007466fc = 0x20;       // .rdata 007466FC
constexpr U k_effect_damage_00746714 = 0x26;     // .rdata 00746714
constexpr U k_effect_heal_00746718 = 0x27;       // .rdata 00746718
constexpr U k_effect_link_0074671c = 0x28;       // .rdata 0074671C
constexpr U k_effect_force_push_0074676c = 0x3c; // .rdata 0074676C
constexpr I k_damage_type_max_00746884 = 0x2000; // .rdata 00746884
constexpr I k_damage_type_fallback_0074685c = 8; // .rdata 0074685C
constexpr I k_damage_power_max_007468ac = 6;     // .rdata 007468AC
constexpr I k_damage_power_default_00746894 = 0; // .rdata 00746894
const U k_inv_log10_2_00745ff8 = 0x40549a78u;    // .rdata 00745FF8 float 3.3219281 (1/log10(2))

inline U self_id(void* self) { return at<U>(self, 8); }
// operator new(8Ch) + CGameEffect(1); a null allocation yields a null effect that the caller then dereferences
inline void* new_effect() {
    void* memory = live_operator_new(0x8c);
    return memory ? recovered_effect_construct_00503e40(memory, nullptr, 1) : nullptr;
}
inline void set_type(void* e, U type) { at<U16>(e, 8) = static_cast<U16>(type); }
// xor r,r; mov r16,[e+0A]; and r,FFEF; or r,8; mov [e+0A],r16
inline void set_subtype_magical(void* e) { at<U16>(e, 0xa) = static_cast<U16>((at<U16>(e, 0xa) & 0xffefu) | 8u); }
// StackPushEngineStructure(0, e): failure -> -2000 (effect leaked); success -> ~CGameEffect + free, 0
inline int push_effect(void* e) {
    if (!recovered_vm_outer_push_engine_structure_005d10b0(vm(), nullptr, 0, e)) return k_push_failed;
    recovered_effect_destroy_00503590(e, nullptr);
    live_free(e);
    return 0;
}
}  // namespace

extern "C" {

// ===================================================================================================== 00532A70
// EffectHeal(int nDamageToHeal) [routine 78].  argc must be exactly 1.
int __fastcall recovered_vm_effect_heal_00532a70(void* self, void*, int, unsigned argc) {
    I amount = 0;
    if (argc != 1) return k_pop_failed;                                                  // 00532A8C..92 -> 00532AA8
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &amount)) return k_pop_failed; // 00532A94..A6
    void* e = new_effect();                                                              // 00532AC0..EA
    set_subtype_magical(e);                                                              // 00532AEC..03
    if (amount > 0) {                                                                    // 00532B07..0D (jle)
        set_type(e, k_effect_heal_00746718);                                             // 00532B0F..16
        recovered_effect_integer_005036a0(e, nullptr, 0, static_cast<U>(amount));        // 00532B1A..23
    } else {
        set_type(e, k_effect_invalid_0074667c);                                          // 00532B2A..31
    }
    void* object = nullptr;
    if (lookup(self_id(self), &object) && vcall<void*>(object, 0x30) != nullptr)         // 00532B35..66 AsSWSCreature
        recovered_effect_creator_00503a00(e, nullptr, self_id(self));                    // 00532B68..6E
    return push_effect(e);                                                               // 00532B73..BF
}

// ===================================================================================================== 00531AE0
// EffectDamage(int nDamageAmount, int nDamageType, int nDamagePower) [routine 79]
int __fastcall recovered_vm_effect_damage_00531ae0(void* self, void*, int, unsigned argc) {
    I amount = 0, dmg_type = 0, power = 0;
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &amount)) return k_pop_failed;   // 00531AF8..0E
    if (static_cast<I>(argc) >= 2) {                                                     // 00531B26..31 (jl)
        if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &dmg_type)) return k_pop_failed; // 00531B33..45
    } else {
        dmg_type = 3;                                                                        // 00531B2C / 00531B5E (quirk: literal 3)
    }
    if (static_cast<I>(argc) >= 3) {                                                     // 00531B62..66 (jl)
        if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &power)) return k_pop_failed; // 00531B68..7A
    } else {
        power = k_damage_power_default_00746894;                                         // 00531B93..99
    }
    if (amount < 0 || amount > 0x2710) amount = 1;                                       // 00531BA3..B2
    if (dmg_type < 0 || dmg_type > k_damage_type_max_00746884) dmg_type = k_damage_type_fallback_0074685c; // 00531BBA..CF
    if (power < 0 || power > k_damage_power_max_007468ac) power = k_damage_power_default_00746894; // 00531BD3..E5
    void* e = new_effect();                                                              // 00531BE9..13
    set_subtype_magical(e);                                                              // 00531C15..31
    set_type(e, k_effect_damage_00746714);                                               // 00531C35..3B
    recovered_effect_resize_00503630(e, nullptr, 0x15);                                  // 00531C1C, 00531C3F
    for (U i = 0; i < 0xf; ++i) recovered_effect_integer_005036a0(e, nullptr, i, 0xffffffffu); // 00531C44..54
    // 00531C56..73: fldlg2; fild dmg_type; fyl2x; fmul [745FF8]; fstp dword tmp; fld tmp; frndint; fistp [008327E0]
    float rank_tmp = 0.0f;
    __asm {
        fldlg2
        fild dword ptr dmg_type
        fyl2x
        fmul dword ptr k_inv_log10_2_00745ff8
        fstp dword ptr rank_tmp
        fld dword ptr rank_tmp
        frndint
        fistp dword ptr recovered_effect_rank_scratch_008327e0
    }
    recovered_effect_integer_005036a0(e, nullptr, static_cast<U>(recovered_effect_rank_scratch_008327e0),
                                      static_cast<U>(amount));                           // 00531C75..83
    recovered_effect_integer_005036a0(e, nullptr, 0xe, static_cast<U>(amount));          // 00531C88..91
    recovered_effect_integer_005036a0(e, nullptr, 0x10, 0x3e8);                          // 00531C96..9F
    recovered_effect_integer_005036a0(e, nullptr, 0x11, static_cast<U>(dmg_type));           // 00531CA4..AD
    recovered_effect_integer_005036a0(e, nullptr, 0x12, static_cast<U>(power));          // 00531CB2..BB
    void* object = nullptr;
    if (lookup(self_id(self), &object) && object != nullptr)                             // 00531CC0..F0
        recovered_effect_creator_00503a00(e, nullptr, self_id(self));                    // 00531CF2..FC
    return push_effect(e);                                                               // 00531D01..4B
}

// ===================================================================================================== 00533660
// EffectResurrection() [routine 82]
int __fastcall recovered_vm_effect_resurrection_00533660(void* self, void*, int, unsigned) {
    void* e = new_effect();                                                              // 0053367A..A6
    set_type(e, k_effect_resurrection_0074668c);                                         // 005336A8..B4
    set_subtype_magical(e);                                                              // 005336AE..CE
    void* object = nullptr;
    if (lookup(self_id(self), &object) && object != nullptr)                             // 005336B8..F9
        recovered_effect_creator_00503a00(e, nullptr, self_id(self));                    // 005336FB..01
    return push_effect(e);                                                               // 00533706..52
}

// ===================================================================================================== 005346B0
// EffectVisualEffect(int nVisualEffectId, int nMissEffect=FALSE) [routine 180]
int __fastcall recovered_vm_effect_visual_effect_005346b0(void* self, void*, int, unsigned argc) {
    I visual = 0, miss = 0;                                                              // 005346D6 miss = 0
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &visual)) return k_pop_failed; // 005346C9..E5
    if (static_cast<I>(argc) >= 2 &&                                                     // 005346E7..EC (jl)
        !recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &miss)) return k_pop_failed; // 005346EE..00
    void* e = new_effect();                                                              // 0053471A..44
    set_type(e, k_effect_visual_007466f4);                                               // 00534746..53
    set_subtype_magical(e);                                                              // 0053474D..67
    recovered_effect_integer_005036a0(e, nullptr, 0, static_cast<U>(visual));            // 0053476B..74
    recovered_effect_integer_005036a0(e, nullptr, 2, static_cast<U>(miss));              // 00534779..82
    void* object = nullptr;
    if (lookup(self_id(self), &object) && object != nullptr)                             // 00534787..B2
        recovered_effect_creator_00503a00(e, nullptr, self_id(self));                    // 005347B4..BA
    return push_effect(e);                                                               // 005347BF..0B
}

// ===================================================================================================== 00532F80
// EffectLinkEffects(effect eChildEffect, effect eParentEffect) [routine 199]
int __fastcall recovered_vm_effect_link_effects_00532f80(void* self, void*, int, unsigned) {
    void* child = nullptr; void* parent = nullptr;
    void* e = new_effect();                                                              // 00532F9A..C6
    set_type(e, k_effect_link_0074671c);                                                 // 00532FC8..D3
    // quirk: both pop failures return -2000 (00532FEE / 00533004 -> 0053304E) and leak the new effect
    if (!recovered_vm_outer_pop_engine_structure_005d10a0(vm(), nullptr, 0, &child)) return k_push_failed;  // 00532FCE..EE
    if (!recovered_vm_outer_pop_engine_structure_005d10a0(vm(), nullptr, 0, &parent)) return k_push_failed; // 00532FF0..04
    kotor_fastcall_00504810(e, nullptr, child, parent);                                  // 00533006..12
    set_subtype_magical(e);                                                              // 00533017..28
    recovered_effect_creator_00503a00(e, nullptr, self_id(self));                        // 0053302C..30
    kotor_fastcall_00504240(e, nullptr);                                                 // 00533035..37
    return push_effect(e);                                                               // 0053303C..88
}

// ===================================================================================================== 00531260
// EffectBeam(int nBeamVisualEffect, object oEffector, int nBodyPart, int bMissEffect=FALSE) [routine 207]
int __fastcall recovered_vm_effect_beam_00531260(void* self, void*, int, unsigned) {
    I visual = 0, body_part = 0, miss = 0;                                               // 00531286 miss = 0
    U effector = 0;
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &visual)) return k_pop_failed;   // 00531279..95
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &effector)) return k_pop_failed;  // 00531297..A9
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &body_part)) return k_pop_failed; // 005312AB..BD
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &miss)) return k_pop_failed;     // 005312BF..D1
    void* e = new_effect();                                                              // 005312EB..15
    if (w2::g07::is_beam_type_004ca960(static_cast<U>(visual)) == 0) {                   // 00531317..2E
        set_type(e, k_effect_invalid_0074667c);                                          // 00531330..37 (flags untouched)
    } else {
        set_type(e, k_effect_beam_007466fc);                                             // 0053133D..43
        recovered_effect_creator_00503a00(e, nullptr, self_id(self));                    // 00531347..4D
        recovered_effect_integer_005036a0(e, nullptr, 0, static_cast<U>(visual));        // 00531352..5B
        recovered_effect_integer_005036a0(e, nullptr, 1, static_cast<U>(body_part));     // 00531360..69
        recovered_effect_integer_005036a0(e, nullptr, 2, static_cast<U>(miss));          // 0053136E..77
        recovered_effect_set_object_id_005036f0(e, nullptr, 0, effector);                // 0053137C..85
        set_subtype_magical(e);                                                          // 0053138A..98
    }
    return push_effect(e);                                                               // 0053139C..E8
}

// ===================================================================================================== 005351E0
// EffectChoke() [routine 159]
int __fastcall recovered_vm_effect_choke_005351e0(void*, void*, int, unsigned) {
    void* e = new_effect();                                                              // 005351F7..21
    set_type(e, k_effect_choke_0074669c);                                                // 00535223..37
    recovered_effect_integer_005036a0(e, nullptr, 0, 7);                                 // 00535229..3B
    return push_effect(e);                                                               // 00535240..8A
}

// ===================================================================================================== 00534AB0
// EffectForcePushTargeted(location lCentre, int nIgnoreTestDirectLine=0) [routine 269]
int __fastcall recovered_vm_effect_force_push_targeted_00534ab0(void* self, void*, int, unsigned) {
    void* location = nullptr; I ignore = 0;                                              // 00534ADB..DF
    if (!recovered_vm_outer_pop_engine_structure_005d10a0(vm(), nullptr, 2, &location)) return k_pop_failed; // 00534ACA..EA
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &ignore)) return k_pop_failed; // 00534AEC..FE (location leaked)
    void* e = new_effect();                                                              // 00534B19..3F
    set_type(e, k_effect_force_push_0074676c);                                           // 00534B41..48 (flags untouched)
    recovered_effect_creator_00503a00(e, nullptr, self_id(self));                        // 00534B4C..5A
    recovered_effect_integer_005036a0(e, nullptr, 0, 1);                                 // 00534B5F..64
    recovered_effect_integer_005036a0(e, nullptr, 1, static_cast<U>(ignore));            // 00534B69..72
    kotor_fastcall_005036d0(e, nullptr, 0, at<U>(location, 0));                          // 00534B77..81 position.x
    kotor_fastcall_005036d0(e, nullptr, 1, at<U>(location, 4));                          // 00534B86..92 position.y
    kotor_fastcall_005036d0(e, nullptr, 2, at<U>(location, 8));                          // 00534B97..A3 position.z
    if (location != nullptr) {                                                           // 00534BA8..B0
        location_dtor_004015a0(location);                                                // 00534BB2 CScriptLocation dtor (ret)
        live_free(location);                                                             // 00534BB7..BD
    }
    return push_effect(e);                                                               // 00534BC0..11
}

// ===================================================================================================== 00537C70
// GetEffectType(effect eEffect) [routine 170]
int __fastcall recovered_vm_get_effect_type_00537c70(void*, void*, int, unsigned) {
    void* e = nullptr;                                                                   // 00537C7E
    if (!recovered_vm_outer_pop_engine_structure_005d10a0(vm(), nullptr, 0, &e)) return k_pop_failed; // 00537C71..8D
    const U constant = kotor_fastcall_00503a70(e, nullptr);                              // 00537C98..9C
    if (!recovered_vm_outer_push_integer_005d1010(vm(), nullptr, static_cast<I>(constant))) return k_push_failed; // 00537CA1..B1 (effect leaked)
    if (e != nullptr) {                                                                  // 00537CBA..C3
        recovered_effect_destroy_00503590(e, nullptr);                                   // 00537CC5
        live_free(e);                                                                    // 00537CCA..D0
    }
    return 0;
}

// ===================================================================================================== 005394D0
// GetHasSpellEffect(int nSpell, object oObject=OBJECT_SELF) [routine 304]
int __fastcall recovered_vm_get_has_spell_effect_005394d0(void*, void*, int, unsigned) {
    I spell = 0; U id = 0; U result = 0;                                                 // 005394DF
    if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &spell)) return k_pop_failed; // 005394DA..E8
    if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &id)) return k_pop_failed;   // 005394F6..08
    void* object = nullptr;
    if (lookup(id, &object) && vcall<void*>(object, 0x10) != nullptr)                    // 0053950A..3C AsSWSObject
        result = recovered_object_has_spell_effect_applied_004cdac0(object, nullptr, static_cast<U>(spell)); // 0053953E..4C (on the looked-up pointer)
    return recovered_vm_outer_push_integer_005d1010(vm(), nullptr, static_cast<I>(result)) ? 0 : k_push_failed; // 0053954E..63
}

// ===================================================================================================== 00545D40
// ClearAllEffects() [routine 710]: on OBJECT_SELF when the run-script object is valid; no stack arguments
int __fastcall recovered_vm_clear_all_effects_00545d40(void* self, void*, int, unsigned) {
    if (at<U>(self, 4) == 1) {                                                           // 00545D41..45
        void* object = nullptr;
        if (lookup(self_id(self), &object))                                              // 00545D47..6C
            kotor_fastcall_004d0940(object, nullptr, 0);                                 // 00545D6E..74
    }
    return 0;                                                                            // 00545D79
}

// =====================================================================================================
// Extra callee (no provider in any lane): 00503A70 CGameEffect::GetEffectTypeScriptConstant, thiscall, plain RET.
// Maps the internal effect type (WORD +8) to the NWScript EFFECT_TYPE_* constant; choke (type 8) is split by
// integer 0 through the jump table at 00503E24.  Unmatched types return 0.
U __fastcall kotor_fastcall_00503a70(void* effect, void*) {
    const U type = at<U16>(effect, 8);                                                   // 00503A70 movzx
    struct Row { U type; U constant; };
    static const Row rows_a[] = {                                                        // 00503A7D..00503B27 (in order)
        {0, 0}, {2, 1}, {7, 3}, {12, 7}, {15, 9}, {18, 0xb}, {21, 0xd}, {22, 0xf}, {24, 0x11}, {25, 0x12},
        {31, 0x14}, {32, 0x15}};
    for (const Row& r : rows_a) if (type == r.type) return r.constant;
    if (type == 8) {                                                                     // 00503B28..2E [0074669C]
        const U sub = at<U>(at<void*>(effect, 0x34), 0) - 1;                             // 00503B30..35
        switch (sub) {                                                                   // 00503B36..3F (unsigned ja)
        case 0: return 0x18;                                                             // 00503B46
        case 1: return 0x19;                                                             // 00503B4D
        case 2: return 0x23;                                                             // 00503B69
        case 3: return 0x1d;                                                             // 00503B5B
        case 4: return 0x1b;                                                             // 00503B54
        case 5: return 0x1e;                                                             // 00503B62
        default: return 0;                                                               // 00503E20
        }
    }
    static const Row rows_b[] = {                                                        // 00503B70..00503E1B (in order)
        {35, 0x1f}, {5, 0x20}, {45, 0x21}, {46, 0x22}, {1, 0x24}, {3, 0x25}, {36, 0x26}, {37, 0x27}, {10, 0x28},
        {11, 0x29}, {13, 0x2a}, {14, 0x2b}, {16, 0x2c}, {17, 0x2d}, {48, 0x2e}, {49, 0x2f}, {28, 0x30}, {29, 0x31},
        {26, 0x32}, {27, 0x33}, {33, 0x34}, {34, 0x35}, {55, 0x36}, {56, 0x37}, {47, 0x38}, {74, 0x3a}, {51, 0x3b},
        {61, 0x3c}, {82, 0x3d}, {62, 0x3e}, {63, 0x3f}, {72, 0x40}, {70, 0x41}, {64, 0x42}, {73, 0x43}, {65, 0x44},
        {52, 0x45}, {71, 0x46}, {75, 0x47}, {76, 0x48}, {50, 0x49}, {101, 0x4a}, {30, 0x4b}, {100, 0x4c},
        {102, 0x4d}, {104, 0x4e}};
    for (const Row& r : rows_b) if (type == r.type) return r.constant;
    return 0;                                                                            // 00503E20
}

}  // extern "C"
