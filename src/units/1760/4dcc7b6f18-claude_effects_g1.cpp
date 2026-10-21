// effects-g1 (Claude): CSWSEffectListHandler callbacks 004D8420..004DA990 for the 0854 ai_resolve table.
// Transcribed from the original swkotor.exe (read-only, capstone sweep); addresses in comments are original instructions.
// ABI (verified per function from its RET): OnApply* are thiscall(CSWSObject* object, CGameEffect* effect, int load_game)
//   RET 0Ch -> extern "C" unsigned __fastcall kotor_fastcall_<addr>(self, edx, object, effect, load_game)  (@20);
//   OnRemove* are thiscall(CSWSObject* object, CGameEffect* effect) RET 8 -> (self, edx, object, effect)  (@16).
//   The handler `self` (CSWSEffectListHandler*) is never read by any of these bodies.
// Already present elsewhere (not redefined): 004D83A0 / 004D83F0 AssuredHit (W1 b7 recovered_effect_*_assured_hit_*).
// Not implemented (cascade beyond the cap, see EFFECTS-g1.json / report): 004D8680 OnApplySummonCreature and
//   004D8A10 OnRemoveSummonCreature (both need CSWSArea::ApplyEffect 0050C6B0 -> CSWSAreaOfEffectObject class).
// Missing callees reconstructed here: 004CAF20 CSWSObject::SetDamageImmunity, 004EC3B0 CSWSCreature::
//   SetMovementRateFactor.  Imported from sibling effects-g3: 004F28D0 CSWSCreature::RecomputeAmbientAnimationState.
// Inline views (no address credit): 005036C0 CGameEffect::GetFloat (fld [ecx+i*4+38h]), 005036E0 GetObjectID
//   ([ecx+i*4+78h]).
// .rdata bound to proven values: [0073D700]=0.0f [0073F414]=1.5f [0073F400]=0.125f [007469B8]=0.707 (qword)
//   [0073E798]=0.1f [0073E46C]=0.2f [0073DFEC]=0.01f [0073D764]=1.0f [007441DC]=0Ah [007441E0]=0Bh
//   [007473B8]=0 (word) [0074666C]=0 (byte); strings "BLOODCOLR" (007469A8) "R" (007469A4) "G" (007469A0) "Y" (0074699C).
// EH: the original unwind frames only release new'd blocks / CExoString locals when a callee throws; the bound callees
//   do not throw in normal operation, so that cleanup is not reproduced (as the sibling effects / W5 files).
// Quirks kept: unchecked operator new results (null dereference, as the original), unchecked AsSWSCreature of the
//   object found by id in OnApplyKnockdown, the stats fallback to the effect pointer in OnApplyAbilityDecrease, the
//   unchecked effect / creature in the OnApplySetStateInternal tail, the null attacker in OnApplyDisarm's event branch.
// x87: double intermediates (53-bit precision), float exactly where the original stores a dword.
#include "842d6b1618-w5_t3.hpp"

using namespace w5;
using I16 = std::int16_t;

namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); } // CExoString() [menu20]
namespace kotor_reconstruction {
struct StartupString { int __thiscall BorrowedEquals005e5290(const char* text); };     // CExoString::operator==(char*) [0077]
}
namespace frame_control { extern unsigned message_manager_007a3a00; }               // [007A3A00] CVirtualMachine* [0456]

extern "C" {
// ---------------- existing providers (c27 map; object in brackets) ----------------
U __fastcall recovered_effect_integer_00503690(void* effect, void*, U index);                       // CGameEffect::GetInteger RET 4 [W1 b7]
void __fastcall recovered_effect_integer_005036a0(void* effect, void*, U index, U value);           // SetInteger RET 8 [0886]
void __fastcall kotor_fastcall_005036d0(void* effect, void*, U index, U value_bits);                // SetFloat RET 8 [claude_w1_chains_damage]
void __fastcall recovered_effect_set_object_id_005036f0(void* effect, void*, U index, U id);       // SetObjectID RET 8 [1194u]
void* __fastcall recovered_effect_copy_string_00503700(void* effect, void*, void* out, U index);   // GetString RET 8 [w2_save_object]
void __fastcall kotor_fastcall_00503750(void* effect, void*, U day, U time);                        // SetExpiryTime RET 8 [claude_0886_bodies_party]
void __fastcall recovered_effect_expiry_00503770(void* effect, void*, unsigned* day, unsigned* ms); // GetExpiryTime RET 8 [W1 b7]
void __fastcall recovered_effect_creator_00503a00(void* effect, void*, U id);                      // SetCreator RET 4 [0886]
void* __fastcall recovered_effect_construct_00503e40(void* effect, void*, U flag);                 // CGameEffect(int) RET 4 [0886]
void* __fastcall kotor_fastcall_00503f70(void* self, void*, void* source);                         // CGameEffect(CGameEffect*) RET 4 [claude_w1_route_bodies]
void __fastcall recovered_effect_apply_004d14f0(void* object, void*, void* effect, U a, U b);      // CSWSObject::ApplyEffect RET 0Ch [0886]
U __fastcall recovered_stats_effect_query_005a6960(void* stats, void*, U kind, void* other);       // GetEffectImmunity RET 8 [1094]
void* __fastcall recovered_message_data_construct_004d69f0(void* data, void*);                     // CSWCCMessageData() [W1 b7]
U __fastcall kotor_fastcall_004d6ee0(void* data, void*, void* other);                              // CSWCCMessageData::CopyTo RET 4 [claude_combat_attack_round]
void __fastcall kotor_fastcall_004d7720(void* data, void*, I index, float value);                  // CSWCCMessageData::SetFloat RET 8 [claude_combat_attack_damage]
void __fastcall w5_object_clear_all_actions_004ccd80(void* object, void*, U clear_combat);         // ClearAllActions RET 4 [1206-07]
void __fastcall recovered_combat_round_remove_all_actions_004d3770(void* round, void*);           // CSWSCombatRound::RemoveAllActions [W1 b7]
void* __fastcall recovered_combat_round_attack_004d2b50(void* round, void*, unsigned index);       // CSWSCombatRound::GetAttack RET 4 [W1 b7]
void __fastcall kotor_fastcall_004d28f0(void* round, void*, U paused, U id);                       // SetPauseTimer RET 8 [claude_w5_combat_actions]
void __fastcall kotor_fastcall_004d2920(void* round, void*, U time, U flag);                       // SetRoundPaused RET 8 [claude_w5_combat_actions]
void __fastcall kotor_fastcall_005b60f0(void* self, void*, U unused, U target);                    // RET 8 [claude_w1_chains_damage]
void __fastcall kotor_fastcall_005178f0(void* creature, void*);                                    // RET 0 [claude_w1_route_bodies]
void __fastcall recovered_creature_update_visible_0051b050(void* creature, void*, unsigned mode);  // UpdateVisibleList RET 4 [W1 b7]
void __fastcall w5_creature_auto_close_gui_panels_004eded0(void* creature, void*, U flag);         // RET 4 [pf_w5_drive]
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);                         // GetServerAIMaster [creature_destructor_r2]
void* __fastcall recovered_object_array_004aed70(void* server, void*);                             // GetObjectArray [w2_party_native]
void* __fastcall kotor_fastcall_004ae780(void* server, void*, U id);                               // GetAreaByGameObjectID RET 4 [0530]
I __fastcall c2da_current_get_string_00413270(void* table, void*, I row, void* column, void* out); // C2DA::GetCExoStringEntry RET 0Ch [0379]
void __fastcall frame_R4_005d0fc0(void* vm, void*, void* script, U id, U flag);                    // CVirtualMachine::RunScript RET 0Ch [load_screen_thin]
void* __fastcall async_ref_construct_string_00406d60(void* ref, void*, const void* string);        // CResRef(CExoString const&) RET 4 [0200]
void* __fastcall recovered_server_creature_construct_004f7a10(void* memory, void*, U id, U flag);  // CSWSCreature() RET 8 [0963]
void __fastcall recovered_set_in_party_004fdb20(void* creature, void*, U in_party, U flag);        // CSWSCreature::SetInParty RET 8 [w2_ifo_native]
unsigned __fastcall w2x_creature_template_005026d0(void* creature, void*, void* ref, unsigned flag); // LoadFromTemplate RET 8 [0886]
unsigned __fastcall w1x_area_safe_location_004be860(void* area, void*, unsigned a, unsigned b, unsigned c, unsigned d, unsigned e); // ComputeSafeLocation RET 14h [0886]
void __fastcall recovered_creature_placement_004fa100(void* creature, void*, U area, U x, U y, U z, U saved, U flag); // AddToArea RET 18h [1060]
U __fastcall recovered_creature_remove_item_00510ef0(void* creature, void*, void* item, U a, U b, U c); // CSWSCreature::RemoveItem RET 10h [w2_party]
U __fastcall recovered_inventory_insert_0055d330(void* repository, void*, void** item, U a, U b, U c); // CItemRepository::AddItem RET 10h [1088]
void __fastcall recovered_item_add_to_area_00555720(void* item, void*, void* area, float x, float y, float z, U flag); // CSWSItem::AddToArea RET 14h [W1 b7]
void __fastcall recovered_item_owner_00553210(void* item, void*, U owner, U a, U b, U c);          // CSWSItem::SetPossessor RET 10h [1013]
int __cdecl crt_rand_006fc447();                                                                   // _rand [0479]
float __fastcall kotor_m15_screature_get_movement_rate_factor_004ec370(void* creature, void*);     // GetMovementRateFactor RET 0, ST0 [0811]
void __fastcall behavior_void_return_4_0060e760(void* self, void*, U value);                       // 0060E760 `ret 4` [0471]

// ---------------- sibling effects group ----------------
void __fastcall kotor_fastcall_004f28d0(void* creature, void*);       // RecomputeAmbientAnimationState RET 0 [claude_effects_g3_death]

// ---------------- reconstructed here ----------------
void __fastcall kotor_fastcall_004caf20(void* object, void*, U flags, U value);
void __fastcall kotor_fastcall_004ec3b0(void* creature, void*, float factor);
unsigned __fastcall kotor_fastcall_004d8420(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004d8490(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004d84d0(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004d8550(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004d8590(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004d8650(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004d8a40(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004d8cb0(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004d8d00(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004d8d80(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004d8f80(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004d9120(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004d9230(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004d92a0(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004d92d0(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004d9320(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004d9620(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004d9680(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004d9720(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004d9970(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004d99b0(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004d9c50(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004d9ce0(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004da050(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004da090(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004da260(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004da2c0(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004da330(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004da870(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004da910(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004da990(void*, void*, void* object, void* effect, unsigned load_game);
}

namespace {
constexpr U k_event_ai_007441dc = 0x0a;             // .rdata 007441DC dword
constexpr U k_event_ai_007441e0 = 0x0b;            // .rdata 007441E0 dword
constexpr U16 k_duration_bits_007473b8 = 0;        // .rdata 007473B8 word
constexpr unsigned char k_found_0074666c = 0;      // .rdata 0074666C byte (CGameObjectArray success)
const float k_one_half_0073f414 = 1.5f;            // .rdata 0073F414
const float k_eighth_0073f400 = 0.125f;            // .rdata 0073F400
const double k_cos45_007469b8 = 0.707;             // .rdata 007469B8 (qword)
const float k_tenth_0073e798 = 0.1f;               // .rdata 0073E798
const float k_fifth_0073e46c = 0.2f;               // .rdata 0073E46C
const float k_hundredth_0073dfec = 0.01f;          // .rdata 0073DFEC
const float k_one_0073d764 = 1.0f;                 // .rdata 0073D764
const char k_bloodcolr_007469a8[] = "BLOODCOLR";
const char k_r_007469a4[] = "R";
const char k_g_007469a0[] = "G";
const char k_y_0074699c[] = "Y";
struct ExoString { char* text; U length; };        // CExoString (8 bytes)
static_assert(sizeof(ExoString) == 8, "CExoString is 8 bytes");

inline U addr(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline void* ptr(U v) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(v)); }
inline void* as_creature(void* o) { return vcall<void*>(o, 0x30); }          // vtbl+30 AsSWSCreature
inline U is_dead(void* c) { return vcall<U>(c, 0x94); }                      // vtbl+94 GetDead
inline void set_animation(void* c, U anim) { vcall<void>(c, 0x7c, anim); }  // vtbl+7C SetAnimation(int)
inline void* stats_of(void* c) { return at<void*>(c, 0xa74); }               // CSWSCreature +A74 CSWSCreatureStats*
inline void* rules() { return kotor_cell_rules_007a3a28; }
inline U rules_byte_aa() { return at<unsigned char>(rules(), 0xaa); }        // movzx [rules+0AAh]
inline U get_int(void* e, U i) { return recovered_effect_integer_00503690(e, nullptr, i); }
inline void set_int(void* e, U i, U v) { recovered_effect_integer_005036a0(e, nullptr, i, v); }
inline float get_float_005036c0(void* e, U i) { return at<float>(e, 0x38 + i * 4); }   // inline view of 005036C0
inline U get_object_id_005036e0(void* e, U i) { return at<U>(e, 0x78 + i * 4); }       // inline view of 005036E0
inline void* creator_creature(void* e) { return kotor_fastcall_004ae770(server_app(), nullptr, at<U>(e, 0x18)); }
inline U immune(void* stats, U kind, void* other) { return recovered_stats_effect_query_005a6960(stats, nullptr, kind, other); }
inline void apply(void* o, void* e, U a) { recovered_effect_apply_004d14f0(o, nullptr, e, a, 0); }
inline U zero_0063e7f0(void* c) { return kotor_m15e_slot010_zero_0063e7f0(c, nullptr); }
inline U dead_temp(void* c) { return recovered_creature_incapacitated_004ef890(c, nullptr); }
// operator new(8Ch) + CGameEffect(CGameEffect*): a failed allocation yields a null effect the caller then uses
inline void* copy_effect(void* source) {
    void* memory = live_operator_new(0x8c);
    return memory != nullptr ? kotor_fastcall_00503f70(memory, nullptr, source) : nullptr;
}
inline void* new_effect() {
    void* memory = live_operator_new(0x8c);
    return memory != nullptr ? recovered_effect_construct_00503e40(memory, nullptr, 1) : nullptr;
}
inline void* new_message_data() {
    void* memory = live_operator_new(0x34);
    return memory != nullptr ? recovered_message_data_construct_004d69f0(memory, nullptr) : nullptr;
}
// (word [e+0A] & FFF8) | [007473B8]
inline void set_duration_bits(void* e) {
    at<U16>(e, 0xa) = static_cast<U16>((static_cast<U>(at<U16>(e, 0xa)) & 0xfff8u) | k_duration_bits_007473b8);
}
// immunity feedback shared by Entangled / Knockdown / Deaf: message data with object 0 = victim id; a copy goes to the
// creator (when found), the original to the victim (fetched again through AsSWSCreature)
void immunity_feedback(void* object, void* creator, U message) {
    void* data = new_message_data();
    recovered_event_object_004d7780(data, nullptr, 0, at<U>(object, 4));
    if (creator != nullptr) {
        void* copy = new_message_data();
        kotor_fastcall_004d6ee0(data, nullptr, copy);
        recovered_creature_feedback_004ede10(creator, nullptr, message, copy);
    }
    recovered_creature_feedback_004ede10(as_creature(object), nullptr, message, data);
}
inline bool str_equals(ExoString* s, const char* text) {
    return reinterpret_cast<kotor_reconstruction::StartupString*>(s)->BorrowedEquals005e5290(text) != 0;
}
}  // namespace

extern "C" {

// ===================================================================================================== 004CAF20
// CSWSObject::SetDamageImmunity(WORD flags, char value) RET 8: clamp to [-100, 100], store at the first set flag bit.
void __fastcall kotor_fastcall_004caf20(void* object, void*, U flags, U value) {
    signed char v = static_cast<signed char>(value & 0xffu);                  // 004CAF20 mov dl,[esp+8]
    if (v < -100) v = -100;                                                   // 004CAF24..2E (signed byte)
    else if (v > 100) v = 100;                                                // 004CAF32..37
    for (U i = 0; i < 0xf; ++i) {                                             // 004CAF3D..51
        if (((static_cast<U16>(flags) >> i) & 1u) != 0) {                     // 004CAF40..4B shr bx,cl; test bl,1
            at<unsigned char*>(object, 0x1ac)[i] = static_cast<unsigned char>(v); // 004CAF59..61
            return;
        }
    }
}

// ===================================================================================================== 004EC3B0
// CSWSCreature::SetMovementRateFactor(float) RET 4: < 0.125 -> 0.125, > 1.5 -> 1.5, else (and NaN) the raw value.
void __fastcall kotor_fastcall_004ec3b0(void* creature, void*, float factor) {
    if (factor < k_eighth_0073f400) { at<U>(creature, 0xa08) = 0x3e000000u; return; }    // 004EC3B0..CB (test ah,5; jp)
    if (factor > k_one_half_0073f414) { at<U>(creature, 0xa08) = 0x3fc00000u; return; }  // 004EC3CE..E9 (test ah,41h; jne)
    at<U>(creature, 0xa08) = bits(factor);                                               // 004EC3EC..F0
}

// ===================================================================================================== 004D8420
// OnApplyAssuredDeflection RET 0Ch
unsigned __fastcall kotor_fastcall_004d8420(void*, void*, void* object, void* effect, unsigned) {
    if (object == nullptr || as_creature(object) == nullptr) return 1;        // 004D8425..38
    void* c = as_creature(object);                                            // 004D843A..41
    if (at<U>(c, 0x8d8) != 0 || at<U>(c, 0x8dc) != 0 || effect == nullptr) return 1; // 004D8443..5D
    at<U>(c, 0x8dc) = 1;                                                      // 004D8461
    if (get_int(effect, 0) != 0) at<U>(c, 0x8d8) = 1;                         // 004D845F..70
    return 0;
}

// ===================================================================================================== 004D8490
// OnRemoveAssuredDeflection RET 8
unsigned __fastcall kotor_fastcall_004d8490(void*, void*, void* object, void*) {
    if (object != nullptr && as_creature(object) != nullptr) {               // 004D8495..A2
        at<U>(as_creature(object), 0x8dc) = 0;                                // 004D84A4..AB
        at<U>(as_creature(object), 0x8d8) = 0;                                // 004D84B5..BC
    }
    return 1;
}

// ===================================================================================================== 004D84D0
// OnApplyAbilityIncrease RET 0Ch
unsigned __fastcall kotor_fastcall_004d84d0(void*, void*, void* object, void* effect, unsigned load_game) {
    if (effect == nullptr) return 1;                                          // 004D84D1..DF
    if (as_creature(object) == nullptr) return 1;                             // 004D84E7..F0
    void* c = as_creature(object);                                            // 004D84F2..F6
    const I amount = static_cast<I>(get_int(effect, 1));                      // 004D84F9..504
    if (load_game == 0) {                                                     // 004D8506..0C
        if (is_dead(c) != 0) return 1;                                        // 004D850E..1A
        if (zero_0063e7f0(c) != 0) return 1;                                  // 004D851C..25
        if (dead_temp(c) != 0) return 1;                                      // 004D8527..30
    }
    return amount < 1 ? 1u : 0u;                                              // 004D8532..3C setl
}

// ===================================================================================================== 004D8550
// OnRemoveAbilityIncrease RET 8: both integers are read and discarded
unsigned __fastcall kotor_fastcall_004d8550(void*, void*, void* object, void* effect) {
    if (as_creature(object) != nullptr) {                                     // 004D8555..5E
        (void)as_creature(object);                                            // 004D8560..64
        (void)get_int(effect, 0);                                             // 004D856B..6F
        (void)get_int(effect, 1);                                             // 004D8574..78
    }
    return 1;
}

// ===================================================================================================== 004D8590
// OnApplyAbilityDecrease RET 0Ch
unsigned __fastcall kotor_fastcall_004d8590(void*, void*, void* object, void* effect, unsigned load_game) {
    if (effect == nullptr) return 1;                                          // 004D8591..9F
    if (as_creature(object) == nullptr) return 1;                             // 004D85A3..B0
    void* c = as_creature(object);                                            // 004D85B6..BE
    // 004D85C0..D0: stats of the creature, or (quirk, unreachable) the effect pointer when the second call is null
    void* stats = c != nullptr ? stats_of(c) : effect;
    if (immune(stats, 0x13, creator_creature(effect)) != 0) return 1;         // 004D85D0..EF
    const I amount = static_cast<I>(get_int(effect, 1));                      // 004D85F1..FA
    if (load_game == 0) {                                                     // 004D85FC..602
        if (is_dead(c) != 0) return 1;                                        // 004D8604..10
        if (zero_0063e7f0(c) != 0) return 1;                                  // 004D8612..1B
        if (dead_temp(c) != 0) return 1;                                      // 004D861D..26
    }
    if (amount < 1) return 1;                                                 // 004D8628..2B
    if (at<U>(c, 0xf8) != 0) return 1;                                        // 004D862D..35
    return 0;
}

// ===================================================================================================== 004D8650
// OnRemoveAbilityDecrease RET 8
unsigned __fastcall kotor_fastcall_004d8650(void*, void*, void* object, void* effect) {
    if (as_creature(object) != nullptr) {                                     // 004D8655..5E
        (void)as_creature(object);                                            // 004D8660..64
        (void)get_int(effect, 0);                                             // 004D8667..6D
    }
    return 1;
}

// ===================================================================================================== 004D8A40
// OnApplySummonParty RET 0Ch: spawns the party member template (string 0) at the location in object/floats 0..2.
unsigned __fastcall kotor_fastcall_004d8a40(void*, void*, void* object, void* effect, unsigned) {
    // 004D8A4E..68: CClientExoApp::GetSWParty()->count >= 3 -> 1
    if (at<I>(kotor_fastcall_005ed8b0(client_app(), nullptr), 0) >= 3) return 1;
    void* const target = as_creature(object);                                 // 004D8A81..8B
    if (effect == nullptr || target == nullptr) return 0;                     // 004D8A8E..9E
    void* creature = nullptr;                                                 // 004D8A92 xor edi,edi
    void* memory = live_operator_new(0xac8);                                  // 004D8AA4..AE
    if (memory != nullptr) creature = recovered_server_creature_construct_004f7a10(memory, nullptr, k_object_invalid, 0); // 004D8ABD..CA
    recovered_set_in_party_004fdb20(creature, nullptr, 1, 1);                 // 004D8ACC..DA
    unsigned char resref[0x10];                                               // [esp+2Ch] CResRef
    {
        ExoString name;                                                       // [esp+0Ch]
        recovered_effect_copy_string_00503700(effect, nullptr, &name, 0);     // 004D8ADF..E8
        async_ref_construct_string_00406d60(resref, nullptr, &name);          // 004D8AED..FA
        frame_LogStringDtor_005e5c20(&name, nullptr);                         // 004D8AFF..0B
    }
    w2x_creature_template_005026d0(creature, nullptr, resref, 0);             // 004D8B10..19
    if (get_object_id_005036e0(effect, 0) == k_object_invalid) {             // 004D8B1E..2C
        recovered_effect_set_object_id_005036f0(effect, nullptr, 0, at<U>(object, 0x8c));    // 004D8B2E..39
        kotor_fastcall_005036d0(effect, nullptr, 0, at<U>(object, 0x90));    // 004D8B3E..49
        kotor_fastcall_005036d0(effect, nullptr, 1, at<U>(object, 0x94));    // 004D8B4E..59
        kotor_fastcall_005036d0(effect, nullptr, 2, at<U>(object, 0x98));    // 004D8B5E..69
    }
    void* area = kotor_fastcall_004ae780(server_app(), nullptr, get_object_id_005036e0(effect, 0)); // 004D8B6E..87
    if (area == nullptr) {                                                    // 004D8B89..8B
        if (creature != nullptr) vcall<void>(creature, 0, 1u);                // 004D8C8E..98 scalar deleting dtor
        return 0;
    }
    float x = get_float_005036c0(effect, 0);                                  // 004D8B91..9A -> [esp+50]
    float y = get_float_005036c0(effect, 1);                                  // 004D8B9E..A7 -> [esp+4C]
    Vector position;                                                          // [esp+20]
    position.z = get_float_005036c0(effect, 2);                               // 004D8BAB..B4 -> [esp+28]
    position.x = x;                                                           // 004D8BB8..C6
    position.y = y;                                                           // 004D8BD2
    Vector safe; safe.x = 0.0f; safe.y = 0.0f; safe.z = 0.0f;                 // 004D8BE2..F2 [esp+14]
    float z;
    // 004D8BB8..FA: ComputeSafeLocation(&position, 10.0, creature+340, 0, &safe)
    if (w1x_area_safe_location_004be860(area, nullptr, addr(&position), 0x41200000u, at<U>(creature, 0x340), 0,
                                        addr(&safe)) == 1) {                  // 004D8BFF..02
        y = safe.y;                                                           // 004D8C04..14
        x = safe.x;                                                           // 004D8C08..1B
        z = flt(w1x_area_height_bits_004bc380(area, nullptr, &safe, 1, nullptr)); // 004D8C0C..24 ComputeHeight(&safe,1,0)
    } else {
        x = position.x; y = position.y; z = position.z;                       // 004D8C2A..3E
    }
    recovered_creature_placement_004fa100(creature, nullptr, addr(area), bits(x), bits(y), bits(z), 0, 0); // 004D8C42..58
    at<U>(creature, 0x34c) = 1;                                               // 004D8C69
    frame_R4_005d0fc0(ptr(frame_control::message_manager_007a3a00), nullptr,
                      static_cast<unsigned char*>(creature) + 0x270, at<U>(creature, 4), 1); // 004D8C5D..7A RunScript
    recovered_effect_set_object_id_005036f0(effect, nullptr, 1, at<U>(creature, 4)); // 004D8C7F..87
    return 0;
}

// ===================================================================================================== 004D8CB0
// OnApplySavingThrowIncrease RET 0Ch
unsigned __fastcall kotor_fastcall_004d8cb0(void*, void*, void* object, void* effect, unsigned) {
    if (as_creature(object) == nullptr) return 0;                             // 004D8CB5..BE
    void* c = as_creature(object);                                            // 004D8CC0..C4
    if (effect == nullptr) return 0;                                          // 004D8CC7..CF
    if (static_cast<I>(get_int(effect, 0)) < 1) return 1;                     // 004D8CD1..E3
    at<U>(c, 0x344) = 1;                                                      // 004D8CE6
    return 0;
}

// ===================================================================================================== 004D8D00
// OnApplySavingThrowDecrease RET 0Ch
unsigned __fastcall kotor_fastcall_004d8d00(void*, void*, void* object, void* effect, unsigned) {
    if (as_creature(object) == nullptr) return 0;                             // 004D8D05..0F
    void* c = as_creature(object);                                            // 004D8D11..15
    if (immune(stats_of(c), 0x19, creator_creature(effect)) != 0) return 1;   // 004D8D18..40
    if (static_cast<I>(get_int(effect, 0)) < 1) return 1;                     // 004D8D42..4D
    if (at<U>(c, 0xf8) != 0) return 1;                                        // 004D8D4F..57
    at<U>(c, 0x344) = 1;                                                      // 004D8D59
    return 0;
}

// ===================================================================================================== 004D8D80
// OnApplyACIncrease RET 0Ch: integers 0 (AC type), 1 (amount), 2 (racial type), 3, 4 (must be 0), 5 (read only).
unsigned __fastcall kotor_fastcall_004d8d80(void*, void*, void* object, void* effect, unsigned) {
    if (as_creature(object) == nullptr) return 0;                             // 004D8D84..92
    void* c = as_creature(object);                                            // 004D8D98..9C
    if (effect == nullptr) return 0;                                          // 004D8D9F..A7
    const U type = get_int(effect, 0);                                        // 004D8DAD..B1 [esp+10]
    const I amount = static_cast<I>(get_int(effect, 1));                      // 004D8DB6..BE ebx
    const U racial = get_int(effect, 2);                                      // 004D8DC3..C9 [esp+18]
    const U i3 = get_int(effect, 3);                                          // 004D8DCE..D6 ebp
    const U i4 = get_int(effect, 4);                                          // 004D8DDB..E1 [esp+1C]
    (void)get_int(effect, 5);                                                 // 004D8DE6..EE
    if (amount < 1) return 1;                                                 // 004D8DF3..02
    if (racial != rules_byte_aa() || i3 != 0 || i4 != 0 || type > 4) return 0; // 004D8E05..36
    const unsigned char b = static_cast<unsigned char>(amount);               // bl
    switch (type) {                                                           // 004D8E3C jump table 004D8F60
    case 0:                                                                   // 004D8E43..56
        at<unsigned char>(stats_of(c), 0x100) = static_cast<unsigned char>(at<unsigned char>(stats_of(c), 0x100) + b);
        return 0;
    case 1:                                                                   // 004D8E59..99
        if (at<signed char>(stats_of(c), 0xfe) > 0) recovered_creature_feedback_004ede10(c, nullptr, 0xbd, nullptr);
        if (amount > at<signed char>(stats_of(c), 0xfe)) at<unsigned char>(stats_of(c), 0xfe) = b;
        return 0;
    case 2:                                                                   // 004D8E9C..DC
        if (at<signed char>(stats_of(c), 0xf8) > 0) recovered_creature_feedback_004ede10(c, nullptr, 0xbe, nullptr);
        if (amount > at<signed char>(stats_of(c), 0xf8)) at<unsigned char>(stats_of(c), 0xf8) = b;
        return 0;
    case 3:                                                                   // 004D8EDF..1B
        if (at<signed char>(stats_of(c), 0xfc) > 0) recovered_creature_feedback_004ede10(c, nullptr, 0xbf, nullptr);
        if (amount > at<signed char>(stats_of(c), 0xfc)) at<unsigned char>(stats_of(c), 0xfc) = b;
        return 0;
    default:                                                                  // 004D8F1E..5A (type 4)
        if (at<signed char>(stats_of(c), 0xfa) > 0) recovered_creature_feedback_004ede10(c, nullptr, 0xc0, nullptr);
        if (amount > at<signed char>(stats_of(c), 0xfa)) at<unsigned char>(stats_of(c), 0xfa) = b;
        return 0;
    }
}

// ===================================================================================================== 004D8F80
// OnApplyACDecrease RET 0Ch
unsigned __fastcall kotor_fastcall_004d8f80(void*, void*, void* object, void* effect, unsigned) {
    if (as_creature(object) == nullptr) return 0;                             // 004D8F84..92
    void* c = as_creature(object);                                            // 004D8F98..9C
    if (immune(stats_of(c), 0x17, creator_creature(effect)) != 0) return 1;   // 004D8F9F..C7
    const U type = get_int(effect, 0);                                        // 004D8FCD..D0
    const I amount = static_cast<I>(get_int(effect, 1));                      // 004D8FD5..DD
    const U racial = get_int(effect, 2);                                      // 004D8FE2..E8
    const U i3 = get_int(effect, 3);                                          // 004D8FED..F5
    const U i4 = get_int(effect, 4);                                          // 004D8FFA..00
    (void)get_int(effect, 5);                                                 // 004D9005..0D
    if (amount < 1) return 1;                                                 // 004D9012..15
    if (at<U>(c, 0xf8) != 0) return 1;                                        // 004D901B..23
    if (racial != rules_byte_aa() || i3 != 0 || i4 != 0 || type > 4) return 0; // 004D9029..4F
    void* stats = stats_of(c);
    const unsigned char b = static_cast<unsigned char>(amount);
    switch (type) {                                                           // 004D9051 jump table 004D9100
    case 0: at<unsigned char>(stats, 0x101) = static_cast<unsigned char>(at<unsigned char>(stats, 0x101) + b); return 0; // 004D9058..5E
    case 1: if (amount > at<signed char>(stats, 0xff)) at<unsigned char>(stats, 0xff) = b; return 0;   // 004D906E..7F
    case 2: if (amount > at<signed char>(stats, 0xf9)) at<unsigned char>(stats, 0xf9) = b; return 0;   // 004D908F..A0
    case 3: if (amount > at<signed char>(stats, 0xfd)) at<unsigned char>(stats, 0xfd) = b; return 0;   // 004D90B0..C1
    default: if (amount > at<signed char>(stats, 0xfb)) at<unsigned char>(stats, 0xfb) = b; return 0;  // 004D90D1..E2
    }
}

// ===================================================================================================== 004D9120
// OnApplyAttackIncrease RET 0Ch
unsigned __fastcall kotor_fastcall_004d9120(void*, void*, void* object, void* effect, unsigned) {
    if (as_creature(object) == nullptr) return 0;                             // 004D9125..2E
    if (effect == nullptr) return 0;                                          // 004D9130..36
    if (static_cast<I>(get_int(effect, 0)) < 1) return 1;                     // 004D9138..4A
    at<U>(as_creature(object), 0x344) = 1;                                    // 004D914D..54
    return 0;
}

// ===================================================================================================== 004D9230
// OnApplyDamageDecrease RET 0Ch
unsigned __fastcall kotor_fastcall_004d9230(void*, void*, void* object, void* effect, unsigned) {
    if (as_creature(object) == nullptr) return 0;                             // 004D9235..3E
    void* creator = creator_creature(effect);                                 // 004D9240..57
    if (immune(stats_of(as_creature(object)), 0x15, creator) != 0) return 1;  // 004D9259..7B
    at<U>(as_creature(object), 0x344) = 1;                                    // 004D927E..85
    return 0;
}

// ===================================================================================================== 004D92A0
// OnRemoveStatChange0 RET 8
unsigned __fastcall kotor_fastcall_004d92a0(void*, void*, void* object, void*) {
    if (as_creature(object) != nullptr) at<U>(as_creature(object), 0x344) = 1; // 004D92A5..B7
    return 1;
}

// ===================================================================================================== 004D92D0
// OnApplyTemporaryHitpoints RET 0Ch: +E4 (dword store) = GetInteger(0) + (short)+E4
unsigned __fastcall kotor_fastcall_004d92d0(void*, void*, void* object, void* effect, unsigned) {
    if (as_creature(object) == nullptr) return 0;                             // 004D92D5..DE
    void* c = as_creature(object);                                            // 004D92E0..E4
    if (effect == nullptr) return 0;                                          // 004D92E7..EF
    const I16 current = at<I16>(c, 0xe4);                                     // 004D92F2
    at<U>(c, 0xe4) = get_int(effect, 0) + static_cast<U>(static_cast<I>(current)); // 004D92F9..305
    return 0;
}

// ===================================================================================================== 004D9320
// OnRemoveTemporaryHitpoints RET 8
unsigned __fastcall kotor_fastcall_004d9320(void*, void*, void* object, void* effect) {
    if (as_creature(object) == nullptr) return 1;                             // 004D933F..49
    void* c = as_creature(object);                                            // 004D934F..56
    const U zero = zero_0063e7f0(c);                                          // 004D9358..5F ebx
    const U dead = is_dead(c);                                                // 004D9361..6D [esp+30]
    const U temp = dead_temp(c);                                              // 004D936B..7C [esp+10]
    if (effect != nullptr) {                                                  // 004D9376..80
        const I16 a = at<I16>(c, 0xe4);                                       // 004D9382
        I value = 0;                                                          // 004D93B2
        if (static_cast<I>(a) - static_cast<I>(get_int(effect, 0)) > 0) {     // 004D9389..99
            const I16 b = at<I16>(c, 0xe4);                                   // 004D939B (re-read)
            value = static_cast<I>(b) - static_cast<I>(get_int(effect, 0));   // 004D93A2..AE
        }
        at<I>(c, 0xe4) = value;                                               // 004D93B4
    }
    if (zero == 0 && zero_0063e7f0(c) == 1) {                                 // 004D93BA..CC
        kotor_fastcall_004f28d0(c, nullptr);                                  // 004D93CE..D0
        at<U>(c, 0xe8) = zero;                                                // 004D93D5
        void* module = server_get_module_004ae6b0(server_app(), nullptr);     // 004D93DB..E4
        const U module_id = at<U>(module, 0x20);                              // 004D93E9
        void* memory = live_operator_new(0x34);                               // 004D93EF..F6
        void* ev = memory != nullptr ? recovered_event_construct_004d7540(memory, nullptr) : nullptr; // 004D93FD..0E
        at<U16>(ev, 0) = 0x20;                                                // 004D9411
        recovered_event_enqueue_004b08d0(recovered_server_ai_master_004aed80(server_app(), nullptr), nullptr,
                                         zero, zero, at<U>(c, 4), module_id, k_event_ai_007441dc, ev); // 004D9416..3B
        w5_creature_auto_close_gui_panels_004eded0(c, nullptr, zero);         // 004D9440..43
        return 1;
    }
    bool kill = false;
    if (dead == 0 && is_dead(c) == 1) kill = true;                            // 004D944D..62
    else if (temp == 0 && dead_temp(c) == 1) kill = true;                     // 004D9464..7A
    if (!kill) return 1;
    void* death = new_effect();                                               // 004D9480..AA edi
    at<U16>(death, 8) = 0x13;                                                 // 004D94B2
    set_duration_bits(death);                                                 // 004D94AC..D3
    recovered_effect_creator_00503a00(death, nullptr, k_object_invalid);      // 004D94B8..D7
    ExoString color;                                                          // [esp+10]
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(&color), nullptr); // 004D94DC..E0
    {
        ExoString column;                                                     // [esp+18]
        frame_LogStringCtor_005e5a90(&column, nullptr, k_bloodcolr_007469a8);  // 004D94EA..F7
        void* appearance = at<void*>(at<void*>(rules(), 0xb8), 0xc);         // 004D94FC..08
        c2da_current_get_string_00413270(appearance, nullptr, at<U16>(c, 0xa60), &column, &color); // 004D950B..24
        frame_LogStringDtor_005e5c20(&column, nullptr);                       // 004D9529..31
    }
    void* blood = new_effect();                                               // 004D9536..5D ebp
    at<U16>(blood, 8) = 0x1e;                                                 // 004D9565
    set_duration_bits(blood);                                                 // 004D955F..88
    if (str_equals(&color, k_r_007469a4)) set_int(blood, 0, 0x9e);            // 004D956B..96, 004D95C8..CC
    else if (str_equals(&color, k_g_007469a0)) set_int(blood, 0, 0x9f);       // 004D9598..AF
    else if (str_equals(&color, k_y_0074699c)) set_int(blood, 0, 0xa0);       // 004D95B1..C3
    apply(c, blood, 0);                                                       // 004D95D1..D8
    set_int(death, 1, 1);                                                     // 004D95DD..E3
    apply(c, death, 0);                                                       // 004D95E8..EF
    frame_LogStringDtor_005e5c20(&color, nullptr);                            // 004D95F4..600
    return 1;                                                                 // 004D9605..1C
}

// ===================================================================================================== 004D9620
// OnApplyDamageImmunityIncrease RET 0Ch
unsigned __fastcall kotor_fastcall_004d9620(void*, void*, void* object, void* effect, unsigned) {
    if (effect == nullptr) return 0;                                          // 004D9621..28
    const U flags = get_int(effect, 0);                                       // 004D962A..37 edi
    I amount = static_cast<I>(get_int(effect, 1));                            // 004D9633..3E esi
    if (amount < 0) return 1;                                                 // 004D9640..4B
    amount += static_cast<signed char>(vcall<unsigned char>(object, 0xb4, flags)); // 004D964F..61 GetDamageImmunity (AL)
    kotor_fastcall_004caf20(object, nullptr, flags, static_cast<U>(amount));  // 004D9663..67
    return 0;
}

// ===================================================================================================== 004D9680
// OnApplyDamageImmunityDecrease RET 0Ch
unsigned __fastcall kotor_fastcall_004d9680(void*, void*, void* object, void* effect, unsigned) {
    if (effect == nullptr) return 0;                                          // 004D9684..8A
    const U flags = get_int(effect, 0);                                       // 004D9690..94 ebp
    const I amount = static_cast<I>(get_int(effect, 1));                      // 004D9699..A4 ebx
    if (amount < 0) return 1;                                                 // 004D96A6..A8
    if (at<U>(object, 0xf8) != 0) return 1;                                   // 004D96AA..B6
    if (as_creature(object) != nullptr) {                                     // 004D96B8..C1
        void* creator = creator_creature(effect);                             // 004D96C3..D9
        if (immune(stats_of(as_creature(object)), 0x16, creator) != 0) return 1; // 004D96DB..F0
    }
    const I current = static_cast<signed char>(vcall<unsigned char>(object, 0xb4, flags)); // 004D96FE..09
    kotor_fastcall_004caf20(object, nullptr, flags, static_cast<U>(current - amount)); // 004D970C..12
    return 0;
}

// ===================================================================================================== 004D9720
// OnApplyEntangled RET 0Ch
unsigned __fastcall kotor_fastcall_004d9720(void*, void*, void* object, void* effect, unsigned load_game) {
    if (as_creature(object) == nullptr) return 0;                             // 004D973C..46
    void* c = as_creature(object);                                            // 004D974C..50 edi
    void* creator = creator_creature(effect);                                 // 004D9753..66 ebx
    if (immune(stats_of(c), 0xa, creator) != 0) {                             // 004D976B..7D
        immunity_feedback(object, creator, 0x90);                             // 004D9783..80F
        return 1;                                                             // 004D9814
    }
    if (at<U>(c, 0xf8) != 0) return 1;                                        // 004D981E..26
    w5_object_clear_all_actions_004ccd80(c, nullptr, 1);                      // 004D9828..2C
    void* hold = copy_effect(effect);                                         // 004D9831..5A esi
    void* speed = copy_effect(effect);                                        // 004D985C..8D ebx
    void* attack = copy_effect(effect);                                       // 004D988F..C0 ebp
    at<U16>(hold, 8) = 0xb;                                                   // 004D98D0
    set_int(hold, 0, 2);                                                      // 004D98C2..D6
    set_int(hold, 1, 0);                                                      // 004D98DB..E1
    set_int(hold, 2, rules_byte_aa());                                        // 004D98E6..F7
    apply(c, hold, load_game);                                                // 004D98FC..906
    at<U16>(speed, 8) = 0x25;                                                 // 004D9911
    set_int(speed, 0, 1);                                                     // 004D990B..17
    set_int(speed, 1, 4);                                                     // 004D991C..22
    apply(c, speed, load_game);                                               // 004D9927..31
    at<U16>(attack, 8) = 0x17;                                                // 004D993C
    set_int(attack, 0, static_cast<U>(-3));                                   // 004D9936..42
    apply(c, attack, load_game);                                              // 004D9947..4D
    return 0;
}

// ===================================================================================================== 004D9970
// OnRemoveEntangled RET 8
unsigned __fastcall kotor_fastcall_004d9970(void*, void*, void* object, void*) {
    if (object != nullptr && as_creature(object) != nullptr) {               // 004D9975..82
        void* c = as_creature(object);                                        // 004D9984..8B
        if (at<U>(c, 0x9d4) == 0) {                                           // 004D998D..95
            kotor_fastcall_005178f0(c, nullptr);                              // 004D9997..99
            recovered_creature_update_visible_0051b050(c, nullptr, 0);        // 004D999E..A2
        }
    }
    return 1;
}

// ===================================================================================================== 004D99B0
// OnApplyKnockdown RET 0Ch
unsigned __fastcall kotor_fastcall_004d99b0(void*, void*, void* object, void* effect, unsigned load_game) {
    const U creator_id = at<U>(effect, 0x18);                                 // 004D99BE..CE ebp
    void* c = as_creature(object);                                            // 004D99D3..DE esi
    if (c == nullptr) return 0;                                               // 004D99E0..E2
    void* creator = kotor_fastcall_004ae770(server_app(), nullptr, creator_id); // 004D99E8..F1 ebx
    if (immune(stats_of(as_creature(object)), 0x1c, creator) != 0) {         // 004D99F6..11
        immunity_feedback(object, creator, 0x81);                             // 004D9A17..AA3
        return 1;
    }
    if (at<U>(c, 0xf8) != 0) return 1;                                        // 004D9AB2..BA
    if (is_dead(c) != 0) return 0;                                            // 004D9ABC..C8
    if (zero_0063e7f0(c) != 0) return 0;                                      // 004D9ACE..D7
    if (dead_temp(c) != 0) return 0;                                          // 004D9ADD..E6
    kotor_fastcall_004d28f0(at<void*>(c, 0x9c8), nullptr, 1, creator_id);     // 004D9AEC..F5
    kotor_fastcall_004d2920(at<void*>(c, 0x9c8), nullptr, 0, 1);              // 004D9AFA..B04
    void* hold = copy_effect(effect);                                         // 004D9B09..36 ebx
    // 004D9B3C..4E: duration = effect duration + 1.5 (fstp dword)
    at<float>(hold, 0xc) = static_cast<float>(static_cast<double>(at<float>(effect, 0xc)) + static_cast<double>(k_one_half_0073f414));
    at<U16>(hold, 8) = 0x17;                                                  // 004D9B59
    set_int(hold, 0, 0xfffffeffu);                                            // 004D9B3F..5F
    apply(object, hold, load_game);                                           // 004D9B64..6E
    at<U>(object, 0xe8) = 0;                                                  // 004D9B73
    void* found = object;                                                     // [esp+54] (object slot reused as out)
    const unsigned char status = static_cast<unsigned char>(
        kotor_fastcall_004d8230(recovered_object_array_004aed70(server_app(), nullptr), nullptr, creator_id, &found)); // 004D9B7D..93
    if (status != k_found_0074666c) {                                         // 004D9B98..9E
        set_animation(c, 0x2717);                                             // 004D9C2A..33
        return 0;
    }
    void* attacker = as_creature(found);                                      // 004D9BA4..AA (unchecked)
    Vector delta, toward, facing;
    kotor_cdecl_00423bc0(&delta, reinterpret_cast<Vector*>(static_cast<unsigned char*>(attacker) + 0x90),
                         reinterpret_cast<Vector*>(static_cast<unsigned char*>(c) + 0x90)); // 004D9BAD..BF
    const Vector d = *normalize_004ab180(&toward, &delta);                    // 004D9BC4..EB
    const Vector* o = normalize_004ab180(&facing, reinterpret_cast<Vector*>(static_cast<unsigned char*>(c) + 0x9c)); // 004D9BDB..EF
    // 004D9BF4..0F: ((d.z*o.z) + d.y*o.y) + d.x*o.x
    double dot = static_cast<double>(d.z) * static_cast<double>(o->z);
    dot = dot + static_cast<double>(d.y) * static_cast<double>(o->y);
    dot = dot + static_cast<double>(d.x) * static_cast<double>(o->x);
    if (!(dot >= k_cos45_007469b8)) set_animation(c, 0x2715);                 // 004D9C11..25 (C0 set: below or unordered)
    else set_animation(c, 0x2717);                                            // 004D9C2C..33
    return 0;
}

// ===================================================================================================== 004D9C50
// OnRemoveKnockdown RET 8
unsigned __fastcall kotor_fastcall_004d9c50(void*, void*, void* object, void* effect) {
    void* c = as_creature(object);                                            // 004D9C52..5D
    if (c == nullptr || effect == nullptr) return 1;                          // 004D9C5F..69
    if (at<U>(at<void*>(c, 0x9c8), 0x958) != 0) {                             // 004D9C6B..79
        kotor_fastcall_004d28f0(at<void*>(c, 0x9c8), nullptr, 0, k_object_invalid); // 004D9C7B..82
        kotor_fastcall_004d2920(at<void*>(c, 0x9c8), nullptr, 0, 0);          // 004D9C87..91
    }
    if (is_dead(c) != 0) return 1;                                            // 004D9C96..A2
    if (zero_0063e7f0(c) != 0) return 1;                                      // 004D9CA4..AD
    if (dead_temp(c) != 0) return 1;                                          // 004D9CAF..B8
    set_animation(c, 0x2711);                                                 // 004D9CBA..C3
    at<U>(object, 0xe8) = 1;                                                  // 004D9CC6..CB
    return 1;
}

// ===================================================================================================== 004D9CE0
// OnApplyDisarm RET 0Ch
unsigned __fastcall kotor_fastcall_004d9ce0(void*, void*, void* object, void* effect, unsigned) {
    if (as_creature(object) == nullptr) return 1;                             // 004D9CE4..F2
    void* c = as_creature(object);                                            // 004D9CF8..FF esi
    Vector a; a.x = 0.0f; a.y = 0.0f; a.z = 0.0f;                             // 004D9D01..19 [esp+8]
    Vector b; b.x = 0.0f; b.y = 0.0f; b.z = 0.0f;                             // 004D9D19..29 [esp+14]
    if (at<U>(c, 0x4f4) == 0) return 1;                                       // 004D9D31..39
    void* attacker = creator_creature(effect);                                // 004D9D3F..51 edi
    const U mode = get_int(effect, 0);                                        // 004D9D56..5C
    void* item = nullptr;                                                     // [esp+24]
    if (mode == 1 && attacker != nullptr &&
        recovered_equipment_item_005a4c20(at<void*>(attacker, 0xa2c), nullptr, 0x10) == nullptr &&
        (item = recovered_equipment_item_005a4c20(at<void*>(c, 0xa2c), nullptr, 0x10)) != nullptr) { // 004D9D61..9B
        recovered_creature_remove_item_00510ef0(c, nullptr, item, 0, 1, 0);   // 004D9DA1..AA
        if ((at<unsigned char>(item, 0x288) & 8) != 0) {                      // 004D9DAF..BA
            void* repository = recovered_creature_inventory_004ef770(attacker, nullptr, 1); // 004D9DCB..CF
            if (recovered_inventory_insert_0055d330(repository, nullptr, &item, 1, 0, 1) == 0) { // 004D9DC0..DD
                if (item == nullptr) return 1;                                // 004D9DDF..E5
                b = *reinterpret_cast<Vector*>(static_cast<unsigned char*>(attacker) + 0x90); // 004D9DEB..03
                b.z = static_cast<float>(static_cast<double>(b.z) + static_cast<double>(k_tenth_0073e798)); // 004D9E07..13
                recovered_item_add_to_area_00555720(item, nullptr, recovered_object_area_004cb120(c, nullptr),
                                                    b.x, b.y, b.z, 1);         // 004D9E11..2A
            }
            if (item == nullptr) return 1;                                    // 004D9E2F..35
            recovered_item_owner_00553210(item, nullptr, at<U>(attacker, 4), 1, 1, k_object_invalid); // 004D9E3B..48
            return 1;
        }
        if (at<U>(item, 0xfc) != 1) return 1;                                 // 004D9E5A..61
    } else {
        item = recovered_equipment_item_005a4c20(at<void*>(c, 0xa2c), nullptr, 0x10); // 004D9E75..84
        if (item == nullptr) return 1;                                        // 004D9E88
        recovered_creature_remove_item_00510ef0(c, nullptr, item, 0, 1, 0);   // 004D9E8E..97
        if ((at<unsigned char>(item, 0x288) & 8) != 0) {                      // 004D9E9C..A7
            a = *reinterpret_cast<Vector*>(static_cast<unsigned char*>(c) + 0x90); // 004D9EAD..C3
            if (crt_rand_006fc447() % 2 != 0) {                               // 004D9EC7..D8
                const I r = crt_rand_006fc447() % 25;                         // 004D9EDA..E7
                a.x = static_cast<float>(static_cast<double>(r) * static_cast<double>(k_tenth_0073e798) + static_cast<double>(a.x)); // 004D9EEB..F5
            } else {
                const I r = crt_rand_006fc447() % 25;                         // 004D9EFB..08
                a.x = static_cast<float>(static_cast<double>(a.x) - static_cast<double>(r) * static_cast<double>(k_tenth_0073e798)); // 004D9F0C..16
            }                                                                 // 004D9F1A fstp
            if (crt_rand_006fc447() % 2 != 0) {                               // 004D9F1E..2F
                const I r = crt_rand_006fc447() % 25;                         // 004D9F31..3E
                a.y = static_cast<float>(static_cast<double>(r) * static_cast<double>(k_tenth_0073e798) + static_cast<double>(a.y)); // 004D9F42..4C
            } else {
                const I r = crt_rand_006fc447() % 25;                         // 004D9F52..5F
                a.y = static_cast<float>(static_cast<double>(a.y) - static_cast<double>(r) * static_cast<double>(k_tenth_0073e798)); // 004D9F63..6D
            }                                                                 // 004D9F75 fstp
            recovered_item_owner_00553210(item, nullptr, k_object_invalid, 1, 1, k_object_invalid); // 004D9F71..87
            // 004D9F8C..AD: GetArea()->ComputeSafeLocation(&a, 20.0, +340, 1, &b) (result ignored)
            (void)w1x_area_safe_location_004be860(recovered_object_area_004cb120(c, nullptr), nullptr, addr(&a), 0x41a00000u,
                                                  at<U>(c, 0x340), 1, addr(&b));
            // 004D9FB2..D9: b.z = GetArea()->ComputeHeight(&b, 0, 0) + 0.2
            const float h = flt(w1x_area_height_bits_004bc380(recovered_object_area_004cb120(c, nullptr), nullptr, &b, 0, nullptr));
            b.z = static_cast<float>(static_cast<double>(h) + static_cast<double>(k_fifth_0073e46c));
            recovered_item_add_to_area_00555720(item, nullptr, recovered_object_area_004cb120(c, nullptr),
                                                b.x, b.y, b.z, 1);             // 004D9FCF..F0
            return 1;
        }
        if (at<U>(item, 0xfc) != 1) return 1;                                 // 004DA002..09
    }
    // 004DA014..30: AIMaster->AddEventDeltaTime(0, 0, attacker id (attacker may be null here: original quirk),
    //               item +14, [007441E0]=0Bh, NULL)
    recovered_event_enqueue_004b08d0(recovered_server_ai_master_004aed80(server_app(), nullptr), nullptr, 0, 0,
                                     at<U>(attacker, 4), at<U>(item, 0x14), k_event_ai_007441e0, nullptr);
    return 1;
}

// ===================================================================================================== 004DA050
// OnApplySetAIState RET 0Ch: word +9F0 &= GetInteger(0)
unsigned __fastcall kotor_fastcall_004da050(void*, void*, void* object, void* effect, unsigned) {
    if (as_creature(object) == nullptr) return 0;                             // 004DA055..5E
    void* c = as_creature(object);                                            // 004DA060..64
    if (effect == nullptr) return 0;                                          // 004DA067..6F
    const U16 state = at<U16>(c, 0x9f0);                                      // 004DA072
    at<U16>(c, 0x9f0) = static_cast<U16>(get_int(effect, 0) & state);         // 004DA079..82
    return 0;
}

// ===================================================================================================== 004DA090
// OnApplyDeaf RET 0Ch
unsigned __fastcall kotor_fastcall_004da090(void*, void*, void* object, void* effect, unsigned load_game) {
    if (as_creature(object) == nullptr) return 0;                             // 004DA0AC..B6
    void* c = as_creature(object);                                            // 004DA0BC..C0 edi
    void* creator = creator_creature(effect);                                 // 004DA0C3..D6 ebx
    if (immune(stats_of(c), 8, creator) != 0) {                               // 004DA0DB..ED
        immunity_feedback(object, creator, 0x8c);                             // 004DA0F3..17F
        return 1;
    }
    if (at<U>(c, 0xf8) != 0) return 1;                                        // 004DA18E..96
    void* attack = copy_effect(effect);                                       // 004DA198..C3 ebx
    void* skill = copy_effect(effect);                                        // 004DA1C3..F6 esi
    at<U16>(attack, 8) = 0x17;                                                // 004DA204
    set_int(attack, 0, static_cast<U>(-0x11));                                // 004DA1F6..0A
    apply(c, attack, load_game);                                              // 004DA20F..19
    at<U16>(skill, 8) = 0x19;                                                 // 004DA224
    set_int(skill, 0, 0x14);                                                  // 004DA21E..2A
    apply(c, skill, load_game);                                               // 004DA22F..35
    return 0;
}

// ===================================================================================================== 004DA260
// OnApplyEnemyAttackBonus RET 0Ch: stats byte +103 += GetInteger(0)
unsigned __fastcall kotor_fastcall_004da260(void*, void*, void* object, void* effect, unsigned) {
    if (as_creature(object) == nullptr) return 0;                             // 004DA265..6E
    if (at<U>(as_creature(object), 0xf8) != 0) return 1;                      // 004DA270..87
    void* stats = stats_of(as_creature(object));                              // 004DA28A..97
    if (effect == nullptr) return 0;                                          // 004DA291..9D
    const unsigned char base = at<unsigned char>(stats, 0x103);               // 004DA2A0
    at<unsigned char>(stats, 0x103) = static_cast<unsigned char>(get_int(effect, 0) + base); // 004DA2A6..AF
    return 0;
}

// ===================================================================================================== 004DA2C0
// OnApplyArcaneSpellFailure RET 0Ch
unsigned __fastcall kotor_fastcall_004da2c0(void*, void*, void* object, void* effect, unsigned) {
    if (as_creature(object) == nullptr) return 0;                             // 004DA2C6..D0
    void* stats = stats_of(as_creature(object));                              // 004DA2D2..DF edi
    if (effect == nullptr) return 0;                                          // 004DA2D9..E5
    if (as_creature(object) != nullptr) at<U>(as_creature(object), 0x344) = 1; // 004DA2E7..F9
    const unsigned char base = at<unsigned char>(stats, 0x11c);               // 004DA304
    const U value = get_int(effect, 0);                                       // 004DA30A..0E
    // 004DA313..18: add al,bl; push eax (the upper bytes of the GetInteger result ride along) -> 0060E760 `ret 4`
    behavior_void_return_4_0060e760(stats, nullptr, (value & 0xffffff00u) | static_cast<unsigned char>(value + base));
    return 0;
}

// ===================================================================================================== 004DA330
// OnApplySetStateInternal RET 0Ch
unsigned __fastcall kotor_fastcall_004da330(void*, void*, void* object, void* effect, unsigned load_game) {
    void* c = as_creature(object);                                            // 004DA348..52 esi
    U cleared = 0;                                                            // [esp+30]
    U state = 0;                                                              // [esp+18]
    if (c != nullptr && effect != nullptr) {                                  // 004DA35D..6B
        state = get_int(effect, 0);                                           // 004DA371..81
        at<U>(c, 0x350) = 0;                                                  // 004DA385
        at<U>(c, 0x354) = 1;                                                  // 004DA38B
        if (state != 9) {                                                     // 004DA379..95
            at<U>(c, 0xe8) = 1;                                               // 004DA39A
            w5_object_clear_all_actions_004ccd80(c, nullptr, 1);              // 004DA397..A0
            recovered_combat_round_remove_all_actions_004d3770(at<void*>(c, 0x9c8), nullptr); // 004DA3A5..AB
            at<U>(c, 0xe8) = 0;                                               // 004DA3B4
            cleared = 1;                                                      // 004DA3BA
        }
        switch (state) {                                                      // 004DA3BE..CA jump table 004DA848 (state-2)
        case 4: {                                                             // 004DA3D1
            void* e = copy_effect(effect);
            at<U16>(e, 8) = 0x17; set_int(e, 0, static_cast<U>(-3));          // 004DA3F8..40B
            unsigned day = 0, ms = 0;                                         // [esp+14], [esp+10]
            recovered_effect_expiry_00503770(effect, nullptr, &day, &ms);     // 004DA410..1C
            kotor_fastcall_00503750(e, nullptr, day, ms);                     // 004DA421..2D
            apply(c, e, load_game);                                           // 004DA432..3C
            void* v = copy_effect(effect);                                    // 004DA441..6E
            at<U16>(v, 8) = 0x1e; set_int(v, 0, 0x7d2);                       // 004DA470..83
            kotor_fastcall_00503750(v, nullptr, day, ms);                     // 004DA488..94
            apply(c, v, load_game);                                           // 004DA499..9F
            kotor_fastcall_004f28d0(c, nullptr);                              // 004DA4A4..A6
            break;
        }
        case 7: case 8: case 9: case 10: {                                    // 004DA4B6 / 004DA78D / 004DA4E6 / 004DA742
            void* e = copy_effect(effect);
            at<U16>(e, 8) = 0x17; set_int(e, 0, static_cast<U>(-3));          // 004DA7AD..C0 / 004DA511..24 / 004DA76D..80
            apply(c, e, load_game);                                           // 004DA7C5..CE
            kotor_fastcall_004f28d0(c, nullptr);                              // 004DA7D3..D5
            break;
        }
        case 6: {                                                             // 004DA6F1
            void* e = copy_effect(effect);
            at<U16>(e, 8) = 0x17; set_int(e, 0, 0xfffffea1u);                 // 004DA71C..32
            apply(c, e, load_game);                                           // 004DA737..CE
            kotor_fastcall_004f28d0(c, nullptr);
            break;
        }
        case 5: {                                                             // 004DA6BA
            void* e = copy_effect(effect);
            at<U16>(e, 8) = 0x17; set_int(e, 0, static_cast<U>(-0xf));        // 004DA6E1..7C0
            apply(c, e, load_game);
            kotor_fastcall_004f28d0(c, nullptr);
            break;
        }
        case 2: {                                                             // 004DA534
            void* e = copy_effect(effect);
            at<U16>(e, 8) = 0x1b;                                             // 004DA56C
            set_int(e, 0, 2); set_int(e, 1, 0); set_int(e, 3, rules_byte_aa()); // 004DA55F..93
            apply(c, e, load_game);                                           // 004DA598..A1
            break;
        }
        case 3: {                                                             // 004DA5AB
            void* v = copy_effect(effect);
            at<U16>(v, 8) = 0x1e; set_int(v, 0, 0x3ef);                       // 004DA5D6..EC
            apply(c, v, load_game);                                           // 004DA5F1..FB
            void* e = copy_effect(effect);                                    // 004DA600..2B
            at<U16>(e, 8) = 0x1b;                                             // 004DA639
            set_int(e, 0, 2); set_int(e, 1, 0); set_int(e, 3, rules_byte_aa()); // 004DA62F..61
            apply(c, e, load_game);                                           // 004DA666..6C
            void* f = copy_effect(effect);                                    // 004DA671..9C
            at<U16>(f, 8) = 0x17; set_int(f, 0, static_cast<U>(-3));          // 004DA6A0..B0
            apply(c, f, load_game);                                           // 004DA6B5 -> 004DA499
            kotor_fastcall_004f28d0(c, nullptr);
            break;
        }
        default:                                                              // 004DA3C4 ja 004DA7DA
            break;
        }
    }
    // 004DA7DA..2A: creator = GetCreatureByGameObjectID(effect +18) (effect / creature unchecked when null: quirk)
    void* creator = creator_creature(effect);
    if (cleared == 0 && creator != nullptr) {                                 // 004DA7EE..F6
        void* round = at<void*>(creator, 0x9c8);
        void* attack = recovered_combat_round_attack_004d2b50(round, nullptr, at<unsigned char>(round, 0x96c)); // 004DA7F8..06
        if (attack != nullptr) {                                              // 004DA80B..0D
            at<unsigned char>(attack, 0x12d) = static_cast<unsigned char>(state); // 004DA80F..13
            at<U>(attack, 0x13c) = 1;                                         // 004DA819
            kotor_fastcall_005b60f0(creator, nullptr, state, at<U>(c, 4));    // 004DA823..2A
        }
    }
    return 0;
}

// ===================================================================================================== 004DA870
// OnRemoveSetStateInternal RET 8
unsigned __fastcall kotor_fastcall_004da870(void*, void*, void* object, void* effect) {
    void* c = as_creature(object);                                            // 004DA870..80 (object unchecked)
    if (effect == nullptr || c == nullptr) return 1;                          // 004DA87E..86
    const U index = get_int(effect, 0) - 1;                                   // 004DA888..8F
    // 004DA890..9C: byte table 004DA8F4 {0,2,0,0,0,0,0,0,0,0,2,1,1} -> targets 004DA8A3 / 004DA8BB / 004DA8C2
    static const unsigned char k_case_004da8f4[13] = {0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1};
    const unsigned char kind = index <= 0xc ? k_case_004da8f4[index] : 2;
    if (kind == 0)                                                            // 004DA8A3..B6 RunScript(+260, id, 1)
        frame_R4_005d0fc0(ptr(frame_control::message_manager_007a3a00), nullptr,
                          static_cast<unsigned char*>(c) + 0x260, at<U>(c, 4), 1);
    if (kind <= 1) kotor_fastcall_004f28d0(c, nullptr);                      // 004DA8BB..BD
    if (at<U>(c, 0x9d4) == 0) {                                               // 004DA8C2..CA
        kotor_fastcall_005178f0(c, nullptr);                                  // 004DA8CC..CE
        recovered_creature_update_visible_0051b050(c, nullptr, 0);            // 004DA8D3..D7
    }
    return 1;
}

// ===================================================================================================== 004DA910
// OnApplyMovementSpeedIncrease RET 0Ch
unsigned __fastcall kotor_fastcall_004da910(void*, void*, void* object, void* effect, unsigned) {
    if (as_creature(object) == nullptr) return 0;                             // 004DA915..1E
    void* c = as_creature(object);                                            // 004DA920..25 edi
    if (static_cast<I>(get_int(effect, 0)) < 100)                             // 004DA92C..3A
        set_int(effect, 0, get_int(effect, 0) + 100);                         // 004DA93C..4D
    const I percent = static_cast<I>(get_int(effect, 0));                     // 004DA952..5B
    const float scale = static_cast<float>(static_cast<double>(percent) * static_cast<double>(k_hundredth_0073dfec)); // 004DA95F..6B
    const double factor = static_cast<double>(kotor_m15_screature_get_movement_rate_factor_004ec370(c, nullptr)); // 004DA96F
    kotor_fastcall_004ec3b0(c, nullptr, static_cast<float>(factor * static_cast<double>(scale))); // 004DA974..7E
    return 0;
}

// ===================================================================================================== 004DA990
// OnApplyMovementSpeedDecrease RET 0Ch
unsigned __fastcall kotor_fastcall_004da990(void*, void*, void* object, void* effect, unsigned) {
    if (as_creature(object) == nullptr) return 0;                             // 004DA995..9F
    void* c = as_creature(object);                                            // 004DA9A1..A5 esi
    if (immune(stats_of(c), 0x18, creator_creature(effect)) != 0) return 1;   // 004DA9A8..D0
    if (static_cast<I>(get_int(effect, 0)) >= 100) return 1;                  // 004DA9D2..DD
    if (at<U>(c, 0xf8) != 0) return 1;                                        // 004DA9DF..E7
    const I percent = static_cast<I>(get_int(effect, 0));                     // 004DA9E9..F2
    // 004DA9F6..08: 1.0 - percent * 0.01 (fsubr), stored as float
    const float scale = static_cast<float>(static_cast<double>(k_one_0073d764) -
                                           static_cast<double>(percent) * static_cast<double>(k_hundredth_0073dfec));
    const double factor = static_cast<double>(kotor_m15_screature_get_movement_rate_factor_004ec370(c, nullptr)); // 004DAA0C
    kotor_fastcall_004ec3b0(c, nullptr, static_cast<float>(factor * static_cast<double>(scale))); // 004DAA11..1B
    return 0;
}

}  // extern "C"
