// W5 combat-attack lane, part D2 ("damage"): the damage / on-hit / post-damage leg of CSWSCreature::ResolveAttack.
//   005BB440 ResolveDamage, 005BAFA0 ResolveOnHitEffect (+ its ten ApplyOnHit* handlers), 005B9040 ResolveOnHitVisuals,
//   005B8B00 ResolvePostMeleeDamage, 005B8C20 ResolvePostRangedDamage, 004EDB20 GetIsWeaponEffective,
//   005B5640 CalculateMaxElementalDamage, 00553990 CSWSItem::GetPassiveProperty
//   extras (no provider anywhere): 005B5C40 / 005B5DA0 CreateLinkedEffect, 00504810 CGameEffect::SetLinked,
//   00504240 CGameEffect::UpdateLinked, 004D7720 CSWCCMessageData::SetFloat
// Transcribed from the original swkotor.exe (read-only); addresses in the comments are original instructions.
// Conventions (W5): thiscall -> extern "C" __fastcall(self, unused_edx, ...); x87 at 53-bit precision -> double
// intermediates, float exactly where the original stores a dword; live_operator_new / live_free only.
#include "842d6b1618-w5_t3.hpp"
#include "5889fde5f1-w5_action_bodies.hpp"

using namespace w5;

extern "C" {
// ---------------- imports: existing providers (c07b map; object in brackets) ----------------
void* __fastcall recovered_combat_round_attack_004d2b50(void* round, void*, unsigned index);       // CSWSCombatRound::GetAttack [W1 b7]
void* __fastcall recovered_effect_construct_00503e40(void* effect, void*, U flag);                 // CGameEffect::CGameEffect [0886]
void __fastcall recovered_effect_creator_00503a00(void* effect, void*, U id);                      // CGameEffect::SetCreator [0886]
void __fastcall recovered_effect_integer_005036a0(void* effect, void*, U index, U value);          // CGameEffect::SetInteger [0886]
void __fastcall recovered_effect_destroy_00503590(void* effect, void*);                            // CGameEffect::~CGameEffect [W1 b7]
void* __fastcall recovered_message_data_construct_004d69f0(void* data, void*);                     // CSWCCMessageData() [W1 b7]
void __fastcall recovered_event_integer_004d76b0(void* data, void*, U index, U value);             // CSWCCMessageData::SetInteger [1017]
void* __fastcall frame_R4_005ed690(void* client, void*);                                           // CClientExoApp::GetInGameGui [0457]
void __fastcall w5_creature_broadcast_voice_chat_004f2520(void* creature, void*, U voice);         // BroadcastVoiceChat [claude_w5_object_actions]
void* __fastcall kotor_fastcall_005c49a0(void* two_dim, void*, U index);                           // CTwoDimArrays::GetIPRPCostTable [claude_equip_items2]
void* __fastcall frame_LogAssign_005e5140(void* self, void*, const char* text);                  // CExoString::operator=(char*) [0459]
int __cdecl crt_rand_006fc447();                                                                   // _rand [0479]
U __fastcall slot_void_return_8_005b5e90_a2(void*, void*, U, U);                                   // 005B5E90 void_return (RET 8) [0486]
unsigned __fastcall kotor_fastcall_004f3fe0(void* c, void*, unsigned a1, unsigned a2, unsigned a3,
    unsigned a4, unsigned a5, unsigned a6, unsigned a7, unsigned a8, unsigned a9, unsigned a10);  // GetTotalEffectBonus [effectbonus part; supersedes the partial W2 providers]

// ---------------- sibling parts of this lane (claude_combat_attack_*.cpp) ----------------
U __fastcall kotor_fastcall_004d2a00(void* round, void*);                                  // CSWSCombatRound::GetOffHandAttack [round]
void* __fastcall kotor_fastcall_004d4ff0(void* round, void*, U type);                      // GetCurrentAttackWeapon [round]
U __fastcall kotor_fastcall_004d1f80(void* data, void*, U flag);                           // CSWSCombatAttackData::GetDamage [round]
U __fastcall kotor_fastcall_004d2030(void* data, void*, U amount);                         // SetBaseDamage [round]
U __fastcall kotor_fastcall_004d2070(void* data, void*, U flags, U amount);                // SetDamage [round]
U __fastcall kotor_fastcall_004d2180(void* data, void*, U flag);                           // GetTotalDamage [round]
U __fastcall kotor_fastcall_004ed5e0(void* creature, void*);                               // CSWSCreature::GetDamageFlags [damagestats]
U __fastcall kotor_fastcall_005a9050(void* stats, void*, U target, U a2, U a3, U a4, U a5, U a6); // GetDamageRoll [damagestats]
U __fastcall kotor_fastcall_005b92b0(void* creature, void*, U a1, U a2, U a3, U a4, U a5, U a6, U a7); // SavingThrowRoll [rolls] (AL)

// ---------------- exports of this file ----------------
void* __fastcall kotor_fastcall_00553990(void* item, void*, I index);
U __fastcall kotor_fastcall_005b5640(void* self, void*, void* target, U unused);
U __fastcall kotor_fastcall_004edb20(void* self, void*, U target_id, U a2);
void __fastcall kotor_fastcall_00504240(void* effect, void*);
void __fastcall kotor_fastcall_00504810(void* effect, void*, void* a, void* b);
void __fastcall kotor_fastcall_004d7720(void* data, void*, I index, float value);
void* __fastcall kotor_fastcall_005b5c40(void* self, void*, U type1, U value1, U type2, U value2);
void* __fastcall kotor_fastcall_005b5da0(void* self, void*, void* linked, U type, U value);
void __fastcall kotor_fastcall_005b9980(void* self, void*, U target, U duration);
void __fastcall kotor_fastcall_005b9d40(void* self, void*, U target, U duration);
void __fastcall kotor_fastcall_005b9870(void* self, void*, U target, U duration);
void __fastcall kotor_fastcall_005b9670(void* self, void*, U target, U duration);
void __fastcall kotor_fastcall_005b9770(void* self, void*, U target, U duration);
void __fastcall kotor_fastcall_005b9b80(void* self, void*, U target, U duration);
void __fastcall kotor_fastcall_005b9e40(void* self, void*, U target, U property);
void __fastcall kotor_fastcall_005ba0d0(void* self, void*, U target, U property);
void __fastcall kotor_fastcall_005ba170(void* self, void*, U target, U property);
void __fastcall kotor_fastcall_005ba300(void* self, void*, U target, U property);
void __fastcall kotor_fastcall_005bafa0(void* self, void*, void* target, U a2, U a3);
void __fastcall kotor_fastcall_005b9040(void* self, void*, void* target);
void __fastcall kotor_fastcall_005bb440(void* self, void*, void* target, U flag, U amount);
void __fastcall kotor_fastcall_005b8b00(void* self, void*, void* target);
void __fastcall kotor_fastcall_005b8c20(void* self, void*, void* target);
}

namespace {
constexpr U k_effect_visual_007466f4 = 0x1e;     // .rdata 007466F4 (CGameEffect type: visual effect)
constexpr U k_effect_death_007466c8 = 0x13;      // .rdata 007466C8 (death)
constexpr U k_effect_link_0074671c = 0x28;       // .rdata 0074671C (link head)
constexpr U k_effect_0074669c = 0x08;            // .rdata 0074669C
constexpr U k_effect_00746688 = 0x03;            // .rdata 00746688
constexpr U k_effect_00746708 = 0x23;            // .rdata 00746708
constexpr U k_effect_00746710 = 0x25;            // .rdata 00746710
constexpr U k_effect_00746788 = 0x43;            // .rdata 00746788
constexpr U k_subtype_007473b8 = 0;              // .rdata 007473B8 (duration type bits 0..2)
constexpr U k_subtype_007473c0 = 1;              // .rdata 007473C0
constexpr U k_subtype_007473c4 = 2;              // .rdata 007473C4
const float k_six_007455c0 = 6.0f;               // .rdata 007455C0 (seconds per round)
const float k_hundred_00741838 = 100.0f;         // .rdata 00741838
constexpr U k_thirty_0x41f00000 = 0x41f00000u;   // 30.0 immediate
const char k_ranged_effect_0074b30c[] = "RangedEffect";      // .rdata 0074B30C
const char k_visual_effect_0074b2fc[] = "VisualEffect";      // .rdata 0074B2FC
const char k_param1_resref_00748dd8[] = "Param1ResRef";      // .rdata 00748DD8
const char k_effect_chance_00748e18[] = "EffectChance";      // .rdata 00748E18
const char k_duration_rounds_00748e08[] = "DurationRounds";  // .rdata 00748E08
const char k_value_00744bfc[] = "Value";                     // .rdata 00744BFC

// 00832878: .data cell beyond the raw data (loader zero) with no writer in the image (only readers 004DFAD4 and
// 005BB4CC); a file-local zero cell keeps the original mutable read.
volatile U g_cell_00832878 = 0;

inline U addr(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline void* ptr(U v) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(v)); }
inline void* rules() { return kotor_cell_rules_007a3a28; }
inline void* two_dim() { return at<void*>(rules(), 0xb8); }                 // CSWRules +B8 CTwoDimArrays*
inline void* round_of(void* c) { return at<void*>(c, 0x9c8); }               // CSWSCreature +9C8 CSWSCombatRound*
// GetAttack(round, round+96C current attack index)
inline void* current_attack(void* self) {
    void* round = round_of(self);
    return recovered_combat_round_attack_004d2b50(round, nullptr, at<U8>(round, 0x96c));
}
// operator new(8Ch) + CGameEffect(1); null when the allocation fails (the callers then fault as the original)
inline void* new_effect() {
    void* memory = live_operator_new(0x8c);
    return memory != nullptr ? recovered_effect_construct_00503e40(memory, nullptr, 1) : nullptr;
}
// CGameEffect +A word: low three bits = duration type
inline void set_duration_type(void* e, U type) {
    at<U16>(e, 0xa) = static_cast<U16>((static_cast<U>(at<U16>(e, 0xa)) & 0xfff8u) | type);
}
// the inline CExoArrayList<CGameEffect*>::Add of the original (data +0, size +4, capacity +8)
void list_push(void* list, void* value) {
    if (at<I>(list, 4) == at<I>(list, 8)) {
        const U cap = at<U>(list, 8);
        menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(list), nullptr,
                                            cap != 0 ? cap + cap : 0x10u);
    }
    at<U*>(list, 0)[at<I>(list, 4)] = reinterpret_cast<std::uintptr_t>(value) & 0xffffffffu;
    at<I>(list, 4) += 1;
}
inline void* attack_effects(void* attack) { return static_cast<unsigned char*>(attack) + 0x88; }   // CExoArrayList<CGameEffect*>
inline void* attack_feedback(void* attack) { return static_cast<unsigned char*>(attack) + 0xa0; }  // CExoArrayList<CSWCCMessageData*>
inline void list_add_call(void* list, void* value) {                         // the out-of-line 00671C00 Add
    gui_load_array_add_00671c00(static_cast<kotor_reconstruction::ArrayMutations*>(list), nullptr, addr(value));
}
inline void delete_effect(void* e) {                                         // ~CGameEffect + free
    recovered_effect_destroy_00503590(e, nullptr);
    live_free(e);
}
// the visual-effect "lead" of the Sleep / Slow / AbilityDamage handlers: type 1Eh, int0 = vfx, creator = self,
// inline-added to the attack's effect list
bool lead_visual(void* self, U vfx, U duration_type, bool timed) {
    void* e = new_effect();
    if (e == nullptr) return false;
    at<U16>(e, 8) = static_cast<U16>(k_effect_visual_007466f4);
    recovered_effect_integer_005036a0(e, nullptr, 0, vfx);
    recovered_effect_creator_00503a00(e, nullptr, at<U>(self, 4));
    set_duration_type(e, duration_type);
    if (timed) at<U>(e, 0xc) = k_thirty_0x41f00000;
    list_push(attack_effects(current_attack(self)), e);
    return true;
}
struct Link { U type; U value; };
// CreateLinkedEffect(t1, v1, t2, v2) then CreateLinkedEffect(head, type, value) for each link; a failed link deletes
// the previous head (its children stay as the original's ~CGameEffect leaves them). Then the duration (rounds * 6 s,
// temporary), UpdateLinked and the attack's effect list (00671C00).
void linked_chain(void* self, U t1, U v1, U t2, U v2, const Link* links, int count, U duration) {
    void* head = kotor_fastcall_005b5c40(self, nullptr, t1, v1, t2, v2);
    if (head == nullptr) return;
    for (int k = 0; k < count; ++k) {
        void* next = kotor_fastcall_005b5da0(self, nullptr, head, links[k].type, links[k].value);
        if (next == nullptr) {
            if (head != nullptr) delete_effect(head);
            return;
        }
        head = next;
    }
    const float seconds = static_cast<float>(static_cast<double>(static_cast<I>(duration)) * k_six_007455c0); // fild; fmul
    const U type = (static_cast<U>(at<U16>(head, 0xa)) & 0xfff8u) | k_subtype_007473c0;
    at<float>(head, 0xc) = seconds;
    at<U16>(head, 0xa) = static_cast<U16>(type);
    at<U16>(head, 0xa) = static_cast<U16>((type & 0xffefu) | 8u);
    kotor_fastcall_00504240(head, nullptr);
    list_add_call(attack_effects(current_attack(self)), head);
}
// the post-damage "damage resistance feedback" effect (type 13h with int0 = 0, int1 = 1) of 005B8B82 / 005B8D10
void push_death_style_effect(void* self, void* attack) {
    void* e = new_effect();
    at<U16>(e, 8) = static_cast<U16>(k_effect_death_007466c8);
    set_duration_type(e, k_subtype_007473b8);
    recovered_effect_creator_00503a00(e, nullptr, at<U>(self, 4));
    recovered_effect_integer_005036a0(e, nullptr, 0, 0);
    recovered_effect_integer_005036a0(e, nullptr, 1, 1);
    list_add_call(attack_effects(attack), e);
}
using OnHit = void(__fastcall*)(void* self, void*, U a, U b);
}  // namespace

extern "C" {
// =========================================================================================================
// 00553990 CSWSItem::GetPassiveProperty(index): RET 4. +24C count, +254 array of 1Ch-byte properties.
void* __fastcall kotor_fastcall_00553990(void* item, void*, I index) {
    if (index < 0 || index >= at<I>(item, 0x24c)) return nullptr;                   // 00553994..9E (jl / jge)
    return static_cast<unsigned char*>(at<void*>(item, 0x254)) + index * 0x1c;      // 005539A0..AB
}

// 005B5640 CSWSCreature::CalculateMaxElementalDamage(target, unused): RET 8.
U __fastcall kotor_fastcall_005b5640(void* self, void*, void* target, U) {
    if (target == nullptr || at<U>(target, 0xf8) != 0) return 0;                    // 005B5644..50
    U zero = 0;   // [esp+4] after the lea: the argument slot itself, zeroed at 005B566E
    return kotor_fastcall_004f3fe0(self, nullptr, 2, addr(target), addr(&zero), 1, 1, 0, 0,
                                                          0xff, 0xff, 0);           // 005B5652..76
}

// 004EDB20 CSWSCreature::GetIsWeaponEffective(target id, a2): RET 8, EAX 0/1.
U __fastcall kotor_fastcall_004edb20(void* self, void*, U target_id, U a2) {
    void* target = nullptr;                                                         // ESI
    void* o = kotor_fastcall_004ae750(server_app(), nullptr, target_id);            // 004EDB20..34
    if (o != nullptr && vcall<void*>(o, 0x10) != nullptr) {                         // 004EDB3B..48
        target = vcall<void*>(o, 0x10);                                             // 004EDB4A..51
        if (target != nullptr && at<U>(target, 0xf8) == 1) return 0;                // 004EDB53..65
    }
    const U damage = kotor_fastcall_005a9050(at<void*>(self, 0xa74), nullptr, addr(target), a2, 0, 0, 1, 0); // 004EDB68..82
    const U elemental = kotor_fastcall_005b5640(self, nullptr, target, a2);          // 004EDB82..88
    return static_cast<I>(elemental + damage) > 0 ? 1u : 0u;                         // 004EDB8D..93 (setg)
}

// =========================================================================================================
// 00504240 CGameEffect::UpdateLinked: propagate duration type, creator, duration, +0/+4/+10/+14/+1C into the two
// linked children (+28, +2C), recursively.
void __fastcall kotor_fastcall_00504240(void* e, void*) {
    for (U slot = 0x28; slot <= 0x2c; slot += 4) {                                  // 00504243 / 005042E9
        void* child = at<void*>(e, slot);
        if (child == nullptr) continue;
        const U kind = at<U8>(e, 0xa) & 0x18u;                                      // 0050424E..53
        if (kind == 8) at<U16>(child, 0xa) = static_cast<U16>((static_cast<U>(at<U16>(child, 0xa)) & 0xffefu) | 8u); // 0050427F
        else if (kind == 0x10) at<U16>(child, 0xa) = static_cast<U16>((static_cast<U>(at<U16>(child, 0xa)) & 0xfff7u) | 0x10u); // 0050426A
        else if (kind == 0x18) at<U8>(child, 0xa) = static_cast<U8>(at<U8>(child, 0xa) | kind);     // 00504265
        kotor_fastcall_00504240(at<void*>(e, slot), nullptr);                       // 00504292..95
        recovered_effect_creator_00503a00(at<void*>(e, slot), nullptr, at<U>(e, 0x18)); // 0050429A..A1
        at<float>(at<void*>(e, slot), 0xc) = at<float>(e, 0xc);                     // 005042A6..AE
        {
            void* c = at<void*>(e, slot);                                           // 005042B1..BD (low 3 bits copied)
            const U diff = (static_cast<U>(at<U8>(c, 0xa)) ^ at<U8>(e, 0xa)) & 7u;
            at<U16>(c, 0xa) = static_cast<U16>(at<U16>(c, 0xa) ^ diff);
        }
        at<U>(at<void*>(e, slot), 0x10) = at<U>(e, 0x10);                           // 005042C1..C7
        at<U>(at<void*>(e, slot), 0x14) = at<U>(e, 0x14);                           // 005042CA..D0
        at<U>(at<void*>(e, slot), 0x1c) = at<U>(e, 0x1c);                           // 005042D3..D9
        void* c = at<void*>(e, slot);                                               // 005042DC..E6
        at<U>(c, 0) = at<U>(e, 0);
        at<U>(c, 4) = at<U>(e, 4);
    }
}

// 00504810 CGameEffect::SetLinked(a, b): RET 8
void __fastcall kotor_fastcall_00504810(void* e, void*, void* a, void* b) {
    at<void*>(e, 0x28) = a;                                                         // 00504818
    at<void*>(e, 0x2c) = b;                                                         // 0050481B
    kotor_fastcall_00504240(e, nullptr);                                            // 0050481E
}

// 004D7720 CSWCCMessageData::SetFloat(index, value): RET 8. Float list at +10 (data, size +14, capacity +18).
void __fastcall kotor_fastcall_004d7720(void* d, void*, I index, float value) {
    if (index >= at<I>(d, 0x14)) {                                                  // 004D7723..2D (jl: signed)
        const I n = index + 1;                                                      // 004D7731
        void* list = static_cast<unsigned char*>(d) + 0x10;
        if (n == 0) {                                                               // 004D7734..39
            if (at<void*>(list, 0) != nullptr) {                                    // 004D773B..3F
                live_free(at<void*>(list, 0));                                      // 004D7741..42
                at<U>(list, 0) = 0; at<U>(list, 8) = 0; at<U>(list, 4) = 0;         // 004D774E..53
                at<float*>(d, 0x10)[index] = value;                                 // 004D7756..5A (quirk: data null, index -1)
                return;
            }
        } else {
            menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(list), nullptr,
                                                static_cast<U>(n));                 // 004D7763..66
        }
        at<I>(list, 4) = n;                                                         // 004D776B
    }
    at<float*>(d, 0x10)[index] = value;                                             // 004D7770..77
}

// 005B5C40 CSWSCreature::CreateLinkedEffect(type1, value1, type2, value2): RET 10h. Two leaf effects (type word +8,
// int0) under a link head (type 28h, creator = self). Quirk: a failed allocation leaks the effects already built.
void* __fastcall kotor_fastcall_005b5c40(void* self, void*, U type1, U value1, U type2, U value2) {
    void* e1 = new_effect();                                                        // 005B5C5A..86 (EBX)
    if (e1 == nullptr) return nullptr;                                              // 005B5C88..A6
    void* e2 = new_effect();                                                        // 005B5CAA..D4 (EDI)
    if (e2 == nullptr) return nullptr;                                              // 005B5CD6..F5
    void* head = new_effect();                                                      // 005B5CF8..20 (ESI)
    if (head == nullptr) return nullptr;                                            // 005B5D20..2A
    at<U16>(e1, 8) = static_cast<U16>(type1);                                       // 005B5D30..3A
    recovered_effect_integer_005036a0(e1, nullptr, 0, value1);                      // 005B5D35..3E
    at<U16>(e2, 8) = static_cast<U16>(type2);                                       // 005B5D47..51
    recovered_effect_integer_005036a0(e2, nullptr, 0, value2);                      // 005B5D4C..55
    at<U16>(head, 8) = static_cast<U16>(k_effect_link_0074671c);                    // 005B5D5A..65
    recovered_effect_creator_00503a00(head, nullptr, at<U>(self, 4));               // 005B5D61..6F
    kotor_fastcall_00504810(head, nullptr, e1, e2);                                 // 005B5D74..78
    return head;
}

// 005B5DA0 CSWSCreature::CreateLinkedEffect(linked, type, value): RET 0Ch. New leaf (type, int0 = value) and a link
// head (28h, creator = self) over (linked, leaf). Quirk: a failed second allocation leaks the leaf.
void* __fastcall kotor_fastcall_005b5da0(void* self, void*, void* linked, U type, U value) {
    void* leaf = new_effect();                                                      // 005B5DB9..E3 (EDI)
    if (leaf == nullptr) return nullptr;                                            // 005B5DE5..EF
    void* head = new_effect();                                                      // 005B5DF1..19 (ESI)
    if (head == nullptr) return nullptr;                                            // 005B5E19..23
    at<U16>(leaf, 8) = static_cast<U16>(type);                                      // 005B5E3F..49
    recovered_effect_integer_005036a0(leaf, nullptr, 0, value);                     // 005B5E44..4D
    at<U16>(head, 8) = static_cast<U16>(k_effect_link_0074671c);                    // 005B5E52..59
    recovered_effect_creator_00503a00(head, nullptr, at<U>(self, 4));               // 005B5E5D..63
    kotor_fastcall_00504810(head, nullptr, linked, leaf);                           // 005B5E68..70
    return head;
}

// =========================================================================================================
// ApplyOnHit* handlers (thiscall(target, duration-or-property), RET 8), selected by ResolveOnHitEffect's
// member-pointer table. The "A" handlers take the 2DA duration in rounds; the "B" handlers the item property.
// 005B9980 ApplyOnHitSleep: visual 5Eh, link(8:6, 1Eh:7) + (1Eh:CFh) + (43h:1Bh), timed
void __fastcall kotor_fastcall_005b9980(void* self, void*, U, U duration) {
    if (!lead_visual(self, 0x5e, k_subtype_007473b8, false)) return;                // 005B9998..A56
    const Link links[] = {{k_effect_visual_007466f4, 0xcf}, {k_effect_00746788, 0x1b}};
    linked_chain(self, k_effect_0074669c, 6, k_effect_visual_007466f4, 7, links, 2, duration); // 005B9A59..B5E
}
// 005B9D40 ApplyOnHitStun: link(8:4, 1Eh:D0h) + (1Eh:CFh) + (43h:1Dh), timed
void __fastcall kotor_fastcall_005b9d40(void* self, void*, U, U duration) {
    const Link links[] = {{k_effect_visual_007466f4, 0xcf}, {k_effect_00746788, 0x1d}};
    linked_chain(self, k_effect_0074669c, 4, k_effect_visual_007466f4, 0xd0, links, 2, duration); // 005B9D40..E2D
}
// 005B9870 ApplyOnHitHold: link(8:5, 1Eh:E8h) + (1Eh:52h) + (1Eh:CFh) + (43h:0Fh), timed
void __fastcall kotor_fastcall_005b9870(void* self, void*, U, U duration) {
    const Link links[] = {{k_effect_visual_007466f4, 0x52}, {k_effect_visual_007466f4, 0xcf}, {k_effect_00746788, 0x0f}};
    linked_chain(self, k_effect_0074669c, 5, k_effect_visual_007466f4, 0xe8, links, 3, duration); // 005B9870..974
}
// 005B9670 ApplyOnHitConfusion: link(8:1, 1Eh:D0h) + (1Eh:CFh) + (43h:11h), timed
void __fastcall kotor_fastcall_005b9670(void* self, void*, U, U duration) {
    const Link links[] = {{k_effect_visual_007466f4, 0xcf}, {k_effect_00746788, 0x11}};
    linked_chain(self, k_effect_0074669c, 1, k_effect_visual_007466f4, 0xd0, links, 2, duration); // 005B9670..75D
}
// 005B9770 ApplyOnHitFear: link(8:2, 1Eh:DAh) + (1Eh:CFh) + (43h:6), timed
void __fastcall kotor_fastcall_005b9770(void* self, void*, U, U duration) {
    const Link links[] = {{k_effect_visual_007466f4, 0xcf}, {k_effect_00746788, 0x06}};
    linked_chain(self, k_effect_0074669c, 2, k_effect_visual_007466f4, 0xda, links, 2, duration); // 005B9770..85D
}
// 005B9B80 ApplyOnHitSlow: visual 5Fh, link(3:0, 1Eh:CFh) + (43h:1Ch), timed
void __fastcall kotor_fastcall_005b9b80(void* self, void*, U, U duration) {
    if (!lead_visual(self, 0x5f, k_subtype_007473b8, false)) return;                // 005B9B98..C56
    const Link links[] = {{k_effect_00746788, 0x1c}};
    linked_chain(self, k_effect_00746688, 0, k_effect_visual_007466f4, 0xcf, links, 1, duration); // 005B9C59..D1B
}

// 005B9E40 ApplyOnHitAbilityDamage(target, property): visual 5Bh (30 s), ability-decrease effect 25h (int0 = ability
// property+9, int1 = 1) linked with 43h:1Ch, 30 s, permanent-or-temporary by the property type, plus CC feedback 8Eh.
void __fastcall kotor_fastcall_005b9e40(void* self, void*, U target_bits, U property_bits) {
    void* target = ptr(target_bits);
    void* property = ptr(property_bits);
    if (!lead_visual(self, 0x5b, k_subtype_007473c0, true)) return;                 // 005B9E59..F26 (+C = 30.0)
    void* e = new_effect();                                                         // 005B9F21..4E (EDI)
    if (e == nullptr) return;                                                       // 005B9F50..56
    at<U16>(e, 8) = static_cast<U16>(k_effect_00746710);                           // 005B9F5C..67
    recovered_effect_integer_005036a0(e, nullptr, 0, at<U8>(property, 9));          // 005B9F6B..74
    recovered_effect_integer_005036a0(e, nullptr, 1, 1);                            // 005B9F79..7F
    void* head = kotor_fastcall_005b5da0(self, nullptr, e, k_effect_00746788, 0x1c); // 005B9F84..90
    if (head == nullptr) {                                                          // 005B9F97..AB
        delete_effect(e);
        return;
    }
    const U type = (static_cast<U>(at<U16>(head, 0xa)) & 0xfff8u) | k_subtype_007473c0; // 005B9FB0..BC
    at<U>(head, 0xc) = k_thirty_0x41f00000;                                         // 005B9FC3
    at<U16>(head, 0xa) = static_cast<U16>(type);                                    // 005B9FCA
    if (at<U16>(property, 0) == 0x30) at<U16>(head, 0xa) = static_cast<U16>((type & 0xfff7u) | 0x10u); // 005B9FCE..E2
    else at<U8>(head, 0xa) = static_cast<U8>(at<U8>(head, 0xa) | 0x18u);            // 005B9FE8
    kotor_fastcall_00504240(head, nullptr);                                         // 005B9FEC..EE
    list_add_call(attack_effects(current_attack(self)), head);                      // 005B9FF3..A011
    void* memory = live_operator_new(0x34);                                         // 005BA016..3B
    void* data = memory != nullptr ? recovered_message_data_construct_004d69f0(memory, nullptr) : nullptr;
    recovered_event_object_004d7780(data, nullptr, 0, at<U>(target, 4));            // 005BA03D..51
    recovered_event_integer_004d76b0(data, nullptr, 0, 0x8e);                       // 005BA056..5F
    recovered_event_integer_004d76b0(data, nullptr, 1, at<U8>(property, 9));        // 005BA064..6D
    recovered_event_integer_004d76b0(data, nullptr, 2, 1);                          // 005BA072..78
    kotor_fastcall_004d7720(data, nullptr, 0, flt(k_thirty_0x41f00000));            // 005BA07D..86
    list_add_call(attack_feedback(current_attack(self)), data);                     // 005BA08B..A9
}

// 005BA0D0 ApplyOnHitPoison(target, property): link(23h:property+9, 43h:17h), permanent-supernatural bits, inline add
void __fastcall kotor_fastcall_005ba0d0(void* self, void*, U, U property_bits) {
    void* property = ptr(property_bits);
    void* e = kotor_fastcall_005b5c40(self, nullptr, k_effect_00746708, at<U8>(property, 9), k_effect_00746788, 0x17); // 005BA0D0..F0
    if (e == nullptr) return;                                                       // 005BA0F5..F9
    at<U16>(e, 0xa) = static_cast<U16>((((static_cast<U>(at<U16>(e, 0xa)) & 0xfff8u) | k_subtype_007473c4) & 0xffefu) | 8u); // 005BA0FB..17
    list_push(attack_effects(current_attack(self)), e);                             // 005BA11B..63
}

// 005BA170 ApplyOnHitSlayRacialGroup(target, property): when the target creature's racial type (stats +DC word)
// equals property+9: visual 32h and a death effect.
void __fastcall kotor_fastcall_005ba170(void* self, void*, U target_bits, U property_bits) {
    void* target = ptr(target_bits);
    void* property = ptr(property_bits);
    void* creature = vcall<void*>(target, 0x30);                                    // 005BA189..8F
    if (creature == nullptr) return;                                                // 005BA192..94
    if (at<U16>(at<void*>(creature, 0xa74), 0xdc) != static_cast<U16>(at<U8>(property, 9))) return; // 005BA19A..B3
    void* v = new_effect();                                                         // 005BA1B9..E3
    if (v == nullptr) return;                                                       // 005BA1E5..EF
    at<U16>(v, 8) = static_cast<U16>(k_effect_visual_007466f4);                     // 005BA1F5..01
    recovered_effect_integer_005036a0(v, nullptr, 0, 0x32);                         // 005BA1FB..05
    recovered_effect_creator_00503a00(v, nullptr, at<U>(self, 4));                  // 005BA20A..10
    set_duration_type(v, k_subtype_007473b8);                                       // 005BA215..29
    list_add_call(attack_effects(current_attack(self)), v);                         // 005BA21B..4A
    void* d = new_effect();                                                         // 005BA24F..79
    if (d == nullptr) return;                                                       // 005BA27B..85
    at<U16>(d, 8) = static_cast<U16>(k_effect_death_007466c8);                      // 005BA287..8D
    recovered_effect_creator_00503a00(d, nullptr, at<U>(self, 4));                  // 005BA291..97
    set_duration_type(d, k_subtype_007473b8);                                       // 005BA29C..B5
    recovered_effect_integer_005036a0(d, nullptr, 0, 0);                            // 005BA2A2..B9
    recovered_effect_integer_005036a0(d, nullptr, 1, 1);                            // 005BA2BE..C4
    list_add_call(attack_effects(current_attack(self)), d);                         // 005BA2C9..E7
}

// 005BA300 ApplyOnHitInstantDeath(target, property): a death effect (arguments unused)
void __fastcall kotor_fastcall_005ba300(void* self, void*, U, U) {
    void* d = new_effect();                                                         // 005BA318..44
    if (d == nullptr) return;                                                       // 005BA346..50
    at<U16>(d, 8) = static_cast<U16>(k_effect_death_007466c8);                      // 005BA356..62
    set_duration_type(d, k_subtype_007473b8);                                       // 005BA35C..73
    recovered_effect_creator_00503a00(d, nullptr, at<U>(self, 4));                  // 005BA377..7D
    recovered_effect_integer_005036a0(d, nullptr, 0, 0);                            // 005BA382..88
    recovered_effect_integer_005036a0(d, nullptr, 1, 1);                            // 005BA38D..93
    list_push(attack_effects(current_attack(self)), d);                             // 005BA398..E0
}

// =========================================================================================================
// 005BAFA0 CSWSCreature::ResolveOnHitEffect(target, a2, a3): RET 0Ch (a2/a3 unread). For every passive "on hit"
// property (type 20h) of the current attack weapon: roll the 2DA chance and an optional saving throw, then apply.
void __fastcall kotor_fastcall_005bafa0(void* self, void*, void* target, U, U) {
    void* creature = vcall<void*>(target, 0x30);                                    // 005BAFBB..CD -> [S+2C]
    if (creature == nullptr) return;                                                // 005BAFD1
    if (at<U>(self, 0xa2c) == 0) return;                                            // 005BAFD7..DD
    void* attack = current_attack(self);                                            // 005BAFE3..F2
    void* weapon = kotor_fastcall_004d4ff0(round_of(self), nullptr, at<U8>(attack, 0x3a)); // 005BAFF7..02 -> [S+28]
    if (weapon == nullptr) return;                                                  // 005BB007..0F
    // locals: [S+10] chance, [S+20] duration and [S+24] DC are dword; [S+0C] save type and [S+08] save subtype are
    // written as bytes and pushed as dwords (their upper bytes are unwritten in the original frame; passed as zero
    // extension here). [S+14]/[S+18] handler pointers are only read on the arm that wrote them.
    I chance = 0;                                                                   // 005BB015
    I duration = 0;                                                                 // 005BB019
    I dc = 0;                                                                       // written by GetINTEntry or 005BB305
    I resref_flag = 0;                                                              // [S+34] written by GetINTEntry on a hit
    U save_type = 0, save_subtype = 0;
    OnHit handler_a = nullptr, handler_b = nullptr;
    if (at<I>(weapon, 0x24c) <= 0) return;                                          // 005BB01D..2B (jle)
    for (I index = 0; index < at<I>(weapon, 0x24c); ++index) {                      // 005BB3DA..EB (jl)
        void* property = kotor_fastcall_00553990(weapon, nullptr, index);           // 005BB032..3A (EBP)
        if (property == nullptr) continue;                                          // 005BB03C..3E
        const U8 condition = at<U8>(property, 0x11);                                // 005BB044
        if (condition != 0xff && (at<U>(weapon, 0x294) & (1u << (condition & 31u))) == 0) continue; // 005BB047..5B
        if (at<U16>(property, 0) != 0x20) continue;                                 // 005BB061..66
        const U subtype = at<U16>(property, 2);                                     // 005BB06C
        U use_a = 0, save = 0;                                                      // EDI, ESI
        if (subtype > 10) continue;                                                 // 005BB074..77 (ja)
        switch (subtype) {                                                          // 005BB07D table 005BB408
        case 0: use_a = 1; save = 1; save_type = 3; save_subtype = 0x0a; handler_a = kotor_fastcall_005b9980; break; // Sleep
        case 1: use_a = 1; save = 1; save_type = 3; save_subtype = 0x0a; handler_a = kotor_fastcall_005b9d40; break; // Stun
        case 2: use_a = 1; save = 1; save_type = 1; save_subtype = 0x12; handler_a = kotor_fastcall_005b9870; break; // Hold
        case 3: use_a = 1; save = 1; save_type = 3; save_subtype = 0x0a; handler_a = kotor_fastcall_005b9670; break; // Confusion
        case 4: use_a = 1; save = 1; save_type = 3; save_subtype = 0x08; handler_a = kotor_fastcall_005b9770; break; // Fear
        case 5: use_a = 1; save = 1; save_type = 3; save_subtype = 0x00; handler_a = kotor_fastcall_005b9b80; break; // Slow
        case 6: save = 1; save_type = 2; save_subtype = 0; handler_b = kotor_fastcall_005b9e40; chance = 100; break; // AbilityDamage
        case 7: save = 0; save_type = 1; save_subtype = 0x0c; handler_b = kotor_fastcall_005ba0d0; chance = 100; break; // Poison
        case 8: save = 1; save_type = 1; save_subtype = 4; handler_b = kotor_fastcall_005ba170; break;   // SlayRacialGroup (chance kept)
        case 9: save = 1; save_type = 1; save_subtype = 4;                                               // void_return (chance kept)
            handler_b = reinterpret_cast<OnHit>(reinterpret_cast<void (*)()>(&slot_void_return_8_005b5e90_a2)); break;
        default: handler_b = kotor_fastcall_005ba300; chance = 100; break;          // 10: InstantDeath (save bytes kept)
        }
        void* t = two_dim();
        {
            alignas(4) unsigned char column[8];                                     // CExoString [S+38]
            frame_LogStringCtor_005e5a90(column, nullptr, k_param1_resref_00748dd8); // 005BB1A6..AF
            const U found = c2da_current_get_int_00413660(at<void*>(t, 0xc8), nullptr, subtype, column, &resref_flag); // 005BB1B4..E0
            frame_LogStringDtor_005e5c20(column, nullptr);                          // 005BB1E5..F3
            if (found != 0 && resref_flag == 1) {                                   // 005BB1F8..0B
                alignas(4) unsigned char c2[8];                                     // [S+40]
                frame_LogStringCtor_005e5a90(c2, nullptr, k_effect_chance_00748e18); // 005BB211..1A
                c2da_current_get_int_00413660(at<void*>(at<void*>(rules(), 0xb8), 0xcc), nullptr, at<U8>(property, 9), c2, &chance); // 005BB21F..46
                frame_LogStringDtor_005e5c20(c2, nullptr);                          // 005BB24B..56
                alignas(4) unsigned char c3[8];                                     // [S+48]
                frame_LogStringCtor_005e5a90(c3, nullptr, k_duration_rounds_00748e08); // 005BB25B..64
                c2da_current_get_int_00413660(at<void*>(at<void*>(rules(), 0xb8), 0xcc), nullptr, at<U8>(property, 9), c3, &duration); // 005BB269..94
                frame_LogStringDtor_005e5c20(c3, nullptr);                          // 005BB299..A1
            }
        }
        {
            alignas(4) unsigned char c4[8];                                         // [S+50]
            frame_LogStringCtor_005e5a90(c4, nullptr, k_value_00744bfc);            // 005BB2A6..AF
            void* cost = kotor_fastcall_005c49a0(at<void*>(rules(), 0xb8), nullptr, 0x19); // 005BB2B4..DC
            const U got = c2da_current_get_int_00413660(cost, nullptr, at<U16>(property, 6), c4, &dc); // 005BB2E1..E3
            frame_LogStringDtor_005e5c20(c4, nullptr);                              // 005BB2EE..FC
            if (got == 0) dc = 0x14;                                                // 005BB2E8..05 (neg/sbb/inc)
        }
        if (use_a != 0) {                                                           // 005BB30D
            if (chance <= 0) continue;                                              // 005BB315..17
            if (duration <= 0) continue;                                            // 005BB31D..23
            if (crt_rand_006fc447() % 100 >= chance) continue;                      // 005BB329..3A (idiv: signed)
            if (save != 0 &&                                                        // 005BB340..6C
                static_cast<U8>(kotor_fastcall_005b92b0(creature, nullptr, save_type, static_cast<U>(dc), save_subtype,
                    at<U>(self, 4), 1, 0, 1)) != 0) continue;
            handler_a(self, nullptr, addr(target), static_cast<U>(duration));       // 005BB36E..7C
        } else {
            if (chance != 100 && crt_rand_006fc447() % 100 >= chance) continue;     // 005BB382..98
            if (save != 0 &&                                                        // 005BB39A..C6
                static_cast<U8>(kotor_fastcall_005b92b0(creature, nullptr, save_type, static_cast<U>(dc), save_subtype,
                    at<U>(self, 4), 1, 0, 1)) != 0) continue;
            handler_b(self, nullptr, addr(target), addr(property));                 // 005BB3C8..D2
        }
    }
}

// 005B9040 CSWSCreature::ResolveOnHitVisuals(target): RET 4. One visual effect per damage type 3..13 dealt (row of
// the visual 2DA [rules+B8]+D0, column RangedEffect/VisualEffect), then a lightsaber-type hit visual FB7h on the target.
void __fastcall kotor_fastcall_005b9040(void* self, void*, void* target) {
    void* attack = current_attack(self);                                            // 005B905F..78 (EDI, [esp+18])
    for (I type = 0; type < 0xe; ++type) {                                          // 005B907A..C1
        if (type == 1 || type == 2 || type == 0) continue;                          // 005B9082..96
        if (static_cast<I>(kotor_fastcall_004d1f80(attack, nullptr, 1u << type)) <= 0) continue; // 005B909C..AF (jle)
        alignas(4) unsigned char column[8];                                         // CExoString [esp+20]
        kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(column), nullptr); // 005B90B5..B9
        I vfx = 0;                                                                  // [esp+10] (005B90C5)
        frame_LogAssign_005e5140(column, nullptr,
            at<U>(attack, 0x40) != 0 ? k_ranged_effect_0074b30c : k_visual_effect_0074b2fc); // 005B90BE..DB
        c2da_current_get_int_00413660(at<void*>(two_dim(), 0xd0), nullptr, static_cast<U>(type), column, &vfx); // 005B90E0..FF
        void* e = new_effect();                                                     // 005B9104..27
        at<U16>(e, 8) = static_cast<U16>(k_effect_visual_007466f4);                 // 005B9129..3A
        set_duration_type(e, k_subtype_007473b8);                                   // 005B9134..4F
        recovered_effect_creator_00503a00(e, nullptr, at<U>(self, 4));              // 005B9153..59
        recovered_effect_integer_005036a0(e, nullptr, 0, static_cast<U>(vfx));      // 005B915E..67
        list_push(attack_effects(attack), e);                                       // 005B916C..A7
        frame_LogStringDtor_005e5c20(column, nullptr);                              // 005B91A3..B2
    }
    const I weapon_type = at<U8>(attack, 0x5c);                                     // 005B91C7 (movzx)
    if (weapon_type == -1) {                                                        // 005B91CB..CE (never true after movzx)
        if (at<U>(self, 0x558) != 2) return;                                        // 005B91D0..DB
    } else if (weapon_type < 8 || weapon_type > 0xa) {                              // 005B91E3..EF
        return;
    }
    void* e = new_effect();                                                         // 005B91F5..1B
    at<U16>(e, 8) = static_cast<U16>(k_effect_visual_007466f4);                     // 005B921D..2C
    set_duration_type(e, k_subtype_007473b8);                                       // 005B9226..45
    recovered_effect_creator_00503a00(e, nullptr, at<U>(target, 4));                // 005B9223..4F (creator = target)
    recovered_effect_integer_005036a0(e, nullptr, 0, 0xfb7);                        // 005B9254..5D
    list_push(attack_effects(attack), e);                                           // 005B9262..95
}

// =========================================================================================================
// 005BB440 CSWSCreature::ResolveDamage(target, flag, amount): RET 0Ch. Rolls the damage (or takes `amount` when
// flag != 0, or 0 for a dead target / GUI-invulnerable party / the 00832878 cell) into the current attack.
void __fastcall kotor_fastcall_005bb440(void* self, void*, void* target, U flag, U amount) {
    void* attack = current_attack(self);                                            // 005BB448..5C (EBX)
    const U sneak = at<U8>(attack, 0x5c) == 2 ? 1u : 0u;                            // 005BB45E..75 (EBP)
    const U attack44 = at<U>(attack, 0x44);                                         // [esp+18]
    const U offhand = kotor_fastcall_004d2a00(round_of(self), nullptr);             // 005BB46F..7C [esp+14]
    U rolled = 0;                                                                   // [esp+10] (005BB488)
    void* gui = frame_R4_005ed690(client_app(), nullptr);                           // 005BB480..90
    const U invulnerable = (at<U>(gui, 0xb4) != 0 && at<U>(self, 0xa88) != 0) ? 1u : 0u; // 005BB495..B2
    if (at<U>(target, 0xf8) == 0 && invulnerable == 0 && g_cell_00832878 == 0) {    // 005BB4B6..D3
        if (flag == 0) {                                                            // 005BB4D5..DB
            rolled = kotor_fastcall_005a9050(at<void*>(self, 0xa74), nullptr, addr(target), offhand, sneak, attack44, 0, 1); // 005BB4DD..F3
            if (vcall<void*>(target, 0x30) != nullptr)                              // 005BB4F8..05
                (void)vcall<U>(vcall<void*>(target, 0x30), 0x9c, 0u);               // 005BB507..14 (result unused)
            kotor_fastcall_004d2030(attack, nullptr, rolled);                       // 005BB51A..21
            kotor_fastcall_005bafa0(self, nullptr, target, offhand, sneak);         // 005BB526..2F
            kotor_fastcall_005b9040(self, nullptr, target);                         // 005BB534..37
            if (sneak != 0) w5_creature_broadcast_voice_chat_004f2520(self, nullptr, 0x11); // 005BB53C..44
            return;
        }
        // flag != 0: 005BB55B
    }
    const U value = flag != 0 ? amount : rolled;                                    // 005BB553..61
    kotor_fastcall_004d2030(attack, nullptr, value);                                // 005BB565..68
    kotor_fastcall_004d2070(attack, nullptr, kotor_fastcall_004ed5e0(self, nullptr), value); // 005BB56D..78
}

// 005B8B00 CSWSCreature::ResolvePostMeleeDamage(target): RET 4. Marks a killing blow (+48) and queues the 13h effect
// when the attack carries +4C.
void __fastcall kotor_fastcall_005b8b00(void* self, void*, void* target) {
    if (target == nullptr) return;                                                  // 005B8B18..21
    void* attack = current_attack(self);                                            // 005B8B27..3A (EDI)
    const I total = static_cast<I>(kotor_fastcall_004d2180(attack, nullptr, 1));    // 005B8B3C..45 (EBX)
    const I hp = static_cast<short>(static_cast<U16>(vcall<U>(target, 0x9c, 0u)));  // 005B8B47..53 (movsx ax)
    if (total >= hp) at<U>(attack, 0x48) = 1;                                       // 005B8B56..5A (jl)
    if (vcall<void*>(target, 0x30) == nullptr) return;                              // 005B8B61..6A
    (void)vcall<void*>(target, 0x30);                                               // 005B8B70..74 (result unused)
    if (at<U>(attack, 0x4c) == 0) return;                                           // 005B8B77..7C
    push_death_style_effect(self, attack);                                          // 005B8B82..C00
}

// 005B8C20 CSWSCreature::ResolvePostRangedDamage(target): RET 4. As the melee variant, plus: a +4C attack only
// queues the 13h effect within 10 m (squared distance <= 100); a non-damaging hit against which the weapon is
// ineffective adds the "weapon ineffective" feedback (CC message 75h) once per round (+97C).
void __fastcall kotor_fastcall_005b8c20(void* self, void*, void* target) {
    if (target == nullptr) return;                                                  // 005B8C3A..42
    void* attack = current_attack(self);                                            // 005B8C48..5D (EBX)
    const I total = static_cast<I>(kotor_fastcall_004d2180(attack, nullptr, 1));    // 005B8C5F..6B (ESI, [esp+28])
    const U offhand_flag = at<U8>(attack, 0x3a) == 2 ? 1u : 0u;                     // 005B8C68..7D [esp+10]
    if (vcall<void*>(target, 0x30) != nullptr) {                                    // 005B8C72..8A
        (void)vcall<void*>(target, 0x30);                                           // 005B8C90 (result unused)
        void* creature = vcall<void*>(target, 0x30);                                // 005B8C93..97
        const I hp = static_cast<short>(static_cast<U16>(vcall<U>(creature, 0x9c, 0u))); // 005B8C9A..A6
        if (total >= hp || at<U>(attack, 0x4c) == 1) at<U>(attack, 0x48) = 1;      // 005B8CA9..B7
        const double dz = static_cast<double>(at<float>(self, 0x98)) - at<float>(target, 0x98); // 005B8CBA..C5
        const double dy = static_cast<double>(at<float>(self, 0x94)) - at<float>(target, 0x94); // 005B8CCB..D1
        const double dx = static_cast<double>(at<float>(self, 0x90)) - at<float>(target, 0x90); // 005B8CD7..DD
        if (at<U>(attack, 0x4c) == 1) {                                             // 005B8CC0..E3
            const double d2 = (dx * dx + dy * dy) + dz * dz;                        // 005B8CE9..F7
            if (d2 <= static_cast<double>(k_hundred_00741838)) {                    // 005B8CF9..0A (test ah,41h; jp)
                push_death_style_effect(self, attack);                              // 005B8D10..88
            }
        }
    } else {
        const I hp = static_cast<short>(static_cast<U16>(vcall<U>(target, 0x9c, 0u))); // 005B8D93..9B
        if (total >= hp) at<U>(attack, 0x48) = 1;                                   // 005B8D9E..A2
    }
    if (total > 0) return;                                                          // 005B8DB1..B3 (jg)
    if (at<U>(round_of(self), 0x97c) != 0) return;                                  // 005B8DB5..C3
    if (kotor_fastcall_004edb20(self, nullptr, at<U>(target, 4), offhand_flag) != 0) return; // 005B8DC5..D7
    void* memory = live_operator_new(0x34);                                         // 005B8DD9..FF
    void* data = memory != nullptr ? recovered_message_data_construct_004d69f0(memory, nullptr) : nullptr;
    at<U8>(data, 0) = 3;                                                            // 005B8E0F
    recovered_event_integer_004d76b0(data, nullptr, 0, 0x75);                       // 005B8E01..12
    list_add_call(attack_feedback(attack), data);                                   // 005B8E17..1E
    at<U>(round_of(self), 0x97c) = 1;                                               // 005B8E23..29
}
}  // extern "C"
