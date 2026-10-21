// Lane effects-g4: CSWSEffectListHandler callbacks 004E14D0..004E4790 (0854 ai_resolve group "effects"[3]),
// reconstructed from the original swkotor.exe (read-only) instruction by instruction.
//  * Apply handlers: thiscall OnApplyX(CSWSObject* object, CGameEffect* effect, int load_game), RET 0Ch
//      -> extern "C" unsigned __fastcall kotor_fastcall_<addr>(void* self, void*, void* object, void* effect, unsigned load_game)  (@20)
//  * Remove handlers: thiscall OnRemoveX(CSWSObject* object, CGameEffect* effect), RET 8
//      -> extern "C" unsigned __fastcall kotor_fastcall_<addr>(void* self, void*, void* object, void* effect)  (@16)
//    The handler object (ECX) is never read by any of these bodies.
//  * AsSWSCreature = object vtable +30h (called exactly as often as the original calls it).
//  * Game heap only (live_operator_new / live_free); no original address is dereferenced; .rdata constants are
//    bound by their proven values (listed at their use).  x87: double intermediates (53-bit PC), float exactly where
//    the original stores a dword.
//  * new + constructor pairs keep the original EH cleanup (free the block when the constructor throws).
//  * Shared callees reconstructed here (absent from the c27 map): see the "shared callees" section at the end.
#include "842d6b1618-w5_t3.hpp"
#include <cstdint>
#include <cstring>

namespace kotor_movie { void* __fastcall string_element_construct_005b3190(void* s, void*); }       // CExoString::CExoString() [0603]
namespace directory_list { struct TimeList; void __fastcall AllocateTimes004c4f50(TimeList*, void*, int); } // CExoArrayList<LARGE_INTEGER>::Allocate [0288]

extern "C" {
using w5::U; using w5::I; using w5::U16; using w5::U8; using w5::Vector;
// ---- providers in the c27 candidate (object in brackets) ----
U __fastcall recovered_effect_integer_00503690(void* effect, void*, U index);                     // CGameEffect::GetInteger [W1 b7]
void __fastcall recovered_effect_integer_005036a0(void* effect, void*, U index, U value);         // CGameEffect::SetInteger [0886]
void __fastcall recovered_effect_creator_00503a00(void* effect, void*, U id);                      // CGameEffect::SetCreator [0886]
void* __fastcall recovered_effect_construct_00503e40(void* effect, void*, U flag);                 // CGameEffect::CGameEffect(int) [0886]
void* __fastcall kotor_fastcall_00503f70(void* effect, void*, void* source);                       // CGameEffect::CGameEffect(CGameEffect*) [claude_w1_route_bodies]
void __fastcall recovered_effect_destroy_00503590(void* effect, void*);                            // CGameEffect::~CGameEffect [W1 b7]
void __fastcall recovered_effect_copy_00504090(void* effect, void*, void* source, U skip);          // CGameEffect::CopyEffect [1091]
void __fastcall recovered_effect_apply_004d14f0(void* object, void*, void* effect, U load, U b);    // CSWSObject::ApplyEffect [0886]
void __fastcall recovered_object_remove_effect_004d05c0(void* object, void*, void* effect);        // CSWSObject::RemoveEffect [W1 b7]
U __fastcall recovered_object_remove_effect_004d06c0(void* object, void*, U id_lo, U id_hi);       // CSWSObject::RemoveEffectById [W1 b7]
U __fastcall recovered_stats_effect_query_005a6960(void* stats, void*, U kind, void* versus);       // CSWSCreatureStats::GetEffectImmunity [1094]
U __fastcall kotor_fastcall_005a6a90(void* stats, void*, void* effect);                            // CSWSCreatureStats::GetEffectLinkImmunity [claude_combat_attack_rolls]
U __fastcall recovered_stats_has_feat_005a6630(void* stats, void*, U feat);                        // CSWSCreatureStats::HasFeat [0963]
U __fastcall recovered_stats_level_005a5fd0(void* stats, void*, U flag);                           // CSWSCreatureStats::GetLevel (AL) [0963]
void __fastcall recovered_stats_refresh_005addc0(void* stats, void*);                              // CSWSCreatureStats::UpdateCombatInformation [1046]
void* __fastcall recovered_combat_round_attack_004d2b50(void* round, void*, U index);              // CSWSCombatRound::GetAttack [W1 b7]
void* __fastcall recovered_message_data_construct_004d69f0(void* data, void*);                      // CSWCCMessageData::CSWCCMessageData [W1 b7]
void __fastcall recovered_message_data_destroy_004d6a40(void* data, void*);                         // CSWCCMessageData::~CSWCCMessageData [W1 b7]
U __fastcall kotor_fastcall_004d6ee0(void* data, void*, void* other);                              // CSWCCMessageData::CopyTo [claude_combat_attack_round]
void __fastcall recovered_event_integer_004d76b0(void* data, void*, U index, U value);              // CSWCCMessageData::SetInteger [1017]
U __fastcall kotor_fastcall_005b92b0(void* creature, void*, U type, U dc, U subtype, U versus_id, U a5, U a6, U store); // SavingThrowRoll (AL) [claude_combat_attack_rolls]
void __fastcall kotor_fastcall_004ef560(void* creature, void*, I value);                           // CSWSCreature::DisplayFloatyDataToSelf [claude_w1_route_bodies]
void* __fastcall recovered_creature_visible_element_00517a20(void* creature, void*, U id);          // CSWSCreature::GetVisibleListElement [W1 b7]
void __fastcall recovered_creature_remove_visible_00517960(void* creature, void*, U id);           // CSWSCreature::RemoveFromVisibleList [W1 b7]
void __fastcall recovered_creature_pacify_004fda20(void* creature, void*, U id);                   // CSWSCreature::PacifyCreature [W1 b7]
U __fastcall kotor_fastcall_0051d9c0(void* creature, void*, U arg);                                 // CSWSCreature::WalkUpdateLocation_QuickWalk [claude_w5_pathfind_walk_quick]
void __fastcall kotor_fastcall_004ed6e0(void* creature, void*);                                    // CSWSCreature::UpdatePersonalSpace [claude_p2s_a_objupdate]
U __fastcall recovered_rules_roll_dice_00550b60(void* rules, void*, U count, U sides);              // CSWRules::RollDice (AX) [W1 b7]
U __fastcall recovered_cc_message_00568570(void* message, void*, U player, U kind, void* data, U flag); // SendServerToPlayerCCMessage [cc_message]
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);                         // CServerExoApp::GetSWSMessage [creature_destructor_r2]
void* __fastcall recovered_object_array_004aed70(void* server, void*);                               // CServerExoApp::GetObjectArray [w2_party_native]
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);                           // CServerExoApp::GetServerAIMaster [creature_destructor_r2]
void* __fastcall kotor_fastcall_004aede0(void* server, void*);                                       // CServerExoApp::GetWorldTimer [0457]
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, U* day, U* time);                   // CWorldTimer::GetWorldTime [0457]
U __fastcall kotor_fastcall_004adf50(void* timer, void*, U d1, U t1, U d2, U t2, U* out_day, U* out_time); // CWorldTimer::AddWorldTimes [w4-creature_anim]
void __fastcall w1x_unique_append_006449d0(void* list, void*, U value);                             // CExoArrayList<ulong>::AddUnique [0886]
U __fastcall recovered_int_list_remove_0066dcc0(void* list, void*, U value);                        // CExoArrayList<int>::Remove [W1 b7]
void __fastcall kotor_m15_array_u16_allocate_004d2fa0(void* list, void*, U capacity);               // CExoArrayList<ushort>::Allocate [0811]
void __fastcall recovered_u16_list_remove_004e3b20(void* list, void*, U value);                     // CExoArrayList<ushort>::Remove [1204-19 w3t2b]
void __fastcall w1x_pointer_list_insert_00616940(void* list, void*, U value, U index);              // CExoArrayList<void*>::Insert [0886]
I __fastcall c2da_current_get_string_00413270(void* table, void*, I row, void* column, void* out);   // C2DA::GetCExoStringEntry [0379]
void* __fastcall async_ref_assign_string_004061f0(void* ref, void*, void* result, void* string);    // CResRef::operator=(CExoString const&) [0200]
U __fastcall slot_void_return_4_0060e760_a1(void* self, void*, U value);                             // void_return 0060E760 (RET 4) [0486]
unsigned __fastcall w1x_area_safe_location_004be860(void* area, void*, unsigned a, unsigned b, unsigned c, unsigned d, unsigned e); // CSWSArea::ComputeSafeLocation [0886]
// ---- shared callees defined by the concurrent effect groups (imported, not redefined) ----
void __fastcall kotor_fastcall_004ed850(void* creature, void*, U update_movement);               // UpdateAppearanceDependantInfo, RET 4 [claude_effects_g2]
void __fastcall kotor_fastcall_004caf20(void* object, void*, U flags, U value);                     // CSWSObject::SetDamageImmunity, RET 8 [claude_effects_g1]
void __fastcall kotor_fastcall_004ec3b0(void* creature, void*, float factor);                      // CSWSCreature::SetMovementRateFactor, RET 4 [claude_effects_g1]
void __fastcall kotor_fastcall_005afac0(void* stats, void*, U value);                              // CSWSCreatureStats::SetForceResistance, RET 4 [claude_effects_g2]
void __fastcall kotor_fastcall_005afae0(void* stats, void*, U value);                              // CSWSCreatureStats::SetForceResistancePenalty, RET 4 [claude_effects_g2]

// ---- exports of this file ----
unsigned __fastcall kotor_fastcall_004e14d0(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004e16e0(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004e18d0(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004e19a0(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004e1a70(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004e1b00(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004e1b80(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004e1c20(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e22f0(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004e2490(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004e2570(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004e2650(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004e2720(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e2a90(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e2d50(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004e2f40(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e2fe0(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004e3050(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004e30d0(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004e31d0(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e32a0(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004e3320(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e3480(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004e35b0(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e3690(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004e3760(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004e3800(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e3be0(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e3c30(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004e3d10(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e3e70(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004e40d0(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e43c0(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e46e0(void*, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004e4750(void*, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004e4790(void*, void*, void* object, void* effect, unsigned load_game);
// shared callees reconstructed in this file (end section)
float __fastcall kotor_fastcall_005036c0(void* effect, void*, I index);                           // CGameEffect::GetFloat (ST0), RET 4
I __fastcall kotor_fastcall_004df510(void* list, void*, U lo, U hi);                               // CExoArrayList<LARGE_INTEGER>::Count, RET 8
void __fastcall kotor_fastcall_004e3b70(void* list, void*, U lo, U hi);                            // CExoArrayList<LARGE_INTEGER>::AddUnique, RET 8
U __fastcall kotor_fastcall_004ee010(void* creature, void*, U appearance, void* effect, U load_game); // CSWSCreature::Disguise, RET 0Ch
}

namespace {
using namespace w5;
// ---- proven .rdata constants ----
constexpr U16 k_duration_instant_007473b8 = 0;     // word [007473B8]
constexpr U16 k_duration_temporary_007473c0 = 1;   // word [007473C0]
constexpr U16 k_duration_permanent_007473c4 = 2;   // word [007473C4]
constexpr U16 k_duration_innate_007473c8 = 4;      // word [007473C8]
constexpr U k_ms_per_minute_00744178 = 60000;      // dword [00744178] = 0EA60h
constexpr U k_event_remove_effect_007441ec = 0xe;  // dword [007441EC]
constexpr U k_speed_step_0073dfec = 0x3c23d70au;   // float [0073DFEC] = 0.01f
constexpr U k_zero_0073d700 = 0x00000000u;         // float [0073D700] = 0.0f
// byte [0074666C] = 0: CGameObjectArray::GetGameObject success status

struct ExoString { char* text; U length; };
struct LargeList { U* data; I count; I alloc; };   // CExoArrayList<LARGE_INTEGER>: 8-byte elements
struct DwordList { U* data; I count; I alloc; };   // CExoArrayList<unsigned long>
static_assert(sizeof(ExoString) == 8 && sizeof(LargeList) == 12 && sizeof(DwordList) == 12, "list layouts");

inline U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline void* ptr(U a) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a)); }
inline void* as_creature(void* object) { return vcall<void*>(object, 0x30); }    // CSWSObject vtable +30h
inline void* stats_of(void* creature) { return at<void*>(creature, 0xa74); }
inline I count_of(void* object) { return at<I>(object, 0x128); }                   // effect list count
inline void* effect_at(void* object, I i) { return at<void**>(object, 0x124)[i]; }  // effect list data
inline U16 type_of(void* effect) { return at<U16>(effect, 8); }
inline U gi(void* effect, U index) { return recovered_effect_integer_00503690(effect, nullptr, index); }
inline void si(void* effect, U index, U value) { recovered_effect_integer_005036a0(effect, nullptr, index, value); }
inline void* rules() { return kotor_cell_rules_007a3a28; }
inline bool same_id(void* a, void* b) { return at<U>(a, 0) == at<U>(b, 0) && at<U>(a, 4) == at<U>(b, 4); }
inline float flt_of(U bits) { return flt(bits); }
inline void* game_rules_table(U offset) { return at<void*>(at<void*>(rules(), 0xb8), offset); }

// operator new(34h) + CSWCCMessageData::CSWCCMessageData (EH state frees the block if the constructor throws)
void* new_message_data() {
    void* memory = live_operator_new(0x34);
    if (memory == nullptr) return nullptr;
    try { return recovered_message_data_construct_004d69f0(memory, nullptr); } catch (...) { live_free(memory); throw; }
}
// operator new(8Ch) + CGameEffect::CGameEffect(CGameEffect*)
void* new_effect_copy(void* source) {
    void* memory = live_operator_new(0x8c);
    if (memory == nullptr) return nullptr;
    try { return kotor_fastcall_00503f70(memory, nullptr, source); } catch (...) { live_free(memory); throw; }
}
// operator new(8Ch) + CGameEffect::CGameEffect(int)
void* new_effect(U flag) {
    void* memory = live_operator_new(0x8c);
    if (memory == nullptr) return nullptr;
    try { return recovered_effect_construct_00503e40(memory, nullptr, flag); } catch (...) { live_free(memory); throw; }
}
// CExoString(const char*) / ~CExoString
inline void exo_literal(ExoString* s, const char* text) { frame_LogStringCtor_005e5a90(s, nullptr, text); }
inline void exo_destroy(ExoString* s) { frame_LogStringDtor_005e5c20(s, nullptr); }

// The "immune" feedback shape shared by OnApplySetState / OnApplyDisease / OnApplyHasteOrSlow:
//   data = new CSWCCMessageData; data->SetObjectID(0, object id);
//   if (creator) { copy = new CSWCCMessageData; data->CopyTo(copy); creator->SendFeedbackMessage(id, copy); [attack->+138 = 1] }
//   object->AsSWSCreature()->SendFeedbackMessage(id, data)
void immune_feedback(void* object, void* creator, U message, void* attack) {
    void* data = new_message_data();
    recovered_event_object_004d7780(data, nullptr, 0, at<U>(object, 4));
    if (creator != nullptr) {
        void* copy = new_message_data();
        (void)kotor_fastcall_004d6ee0(data, nullptr, copy);
        recovered_creature_feedback_004ede10(creator, nullptr, message, copy);
        if (attack != nullptr) at<U>(attack, 0x138) = 1;
    }
    recovered_creature_feedback_004ede10(as_creature(object), nullptr, message, data);
}
}  // namespace

// =====================================================================================================================
// 004E14D0 CSWSEffectListHandler::OnRemoveACIncrease(object, effect), RET 8
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e14d0(void*, void*, void* object, void* effect) {
    if (as_creature(object) == nullptr) return 1;                                 // 004E14D4..E1
    void* creature = as_creature(object);                                         // 004E14E7..F7 (EBP)
    if (effect == nullptr) return 1;                                              // 004E14F1..F9
    const U kind = gi(effect, 0);                                                 // 004E14FF..10 [esp+8]
    const U amount = gi(effect, 1);                                               // 004E150A..19 [esp+C]
    const U i2 = gi(effect, 2);                                                   // 004E1515..26 (ESI)
    const U i3 = gi(effect, 3);                                                   // 004E1522..31 (EBX)
    const U i4 = gi(effect, 4);                                                   // 004E152D..45
    void* stats = stats_of(creature);                                             // 004E1538
    I index = at<U16>(stats, 0x12e);                                              // 004E153E..57
    if (i2 != at<U8>(rules(), 0xaa)) return 1;                                     // 004E1549..5B
    if (i3 != 0) return 1;                                                        // 004E1561..63
    if (i4 != 0) return 1;                                                        // 004E1569..6F
    if (kind == 0) {                                                              // 004E1575..7B
        at<U8>(stats, 0x100) = static_cast<U8>(at<U8>(stats, 0x100) - static_cast<U8>(amount)); // 004E157D..8C
        return 1;
    }
    U best = 0;                                                                   // 004E15A4..A8
    for (; index < count_of(creature); ++index) {                                 // 004E159E..AC, 004E162F..40
        void* e = effect_at(creature, index);                                     // 004E15B2..B8
        if (type_of(e) != 0x30) break;                                            // 004E15BB..C0
        if (e == effect) continue;                                                // 004E15C6..C8
        if (gi(e, 0) != kind) continue;                                           // 004E15CA..D7
        const U8 aa = at<U8>(rules(), 0xaa);                                      // 004E15D9..DF
        if (gi(e, 2) != aa) continue;                                             // 004E15E5..F3
        if (gi(e, 3) != 0) continue;                                              // 004E15F5..600
        if (gi(e, 4) != 0) continue;                                              // 004E1602..0D
        if (static_cast<I>(gi(e, 1)) > static_cast<I>(best)) best = gi(e, 1);     // 004E160F..27 (jle)
    }
    switch (kind - 1) {                                                           // 004E1646..50 jump table 004E16C4
    case 0: at<U8>(stats_of(creature), 0xfe) = static_cast<U8>(best); break;     // 004E1657
    case 1: at<U8>(stats_of(creature), 0xf8) = static_cast<U8>(best); break;     // 004E168D
    case 2: at<U8>(stats_of(creature), 0xfc) = static_cast<U8>(best); break;     // 004E16A8
    case 3: at<U8>(stats_of(creature), 0xfa) = static_cast<U8>(best); break;     // 004E1672
    default: break;
    }
    return 1;
}

// =====================================================================================================================
// 004E16E0 CSWSEffectListHandler::OnRemoveACDecrease(object, effect), RET 8
//   quirk kept: the scan starts at the same stats+12E index as ACIncrease and, unlike it, does not test integers 3/4
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e16e0(void*, void*, void* object, void* effect) {
    if (as_creature(object) == nullptr) return 1;                                 // 004E16E2..EF
    void* creature = as_creature(object);                                         // 004E16F5..707 (ESI)
    if (effect == nullptr) return 1;
    const U kind = gi(effect, 0);                                                 // 004E170D..1A
    const U amount = gi(effect, 1);                                               // 004E1716..27
    const U i2 = gi(effect, 2);                                                   // 004E1723..34 (EBX)
    const U i3 = gi(effect, 3);                                                   // 004E1730..3F
    const U i4 = gi(effect, 4);                                                   // 004E173B..43
    void* stats = stats_of(creature);                                             // 004E1757
    I index = at<U16>(stats, 0x12e);                                              // 004E175D
    if (i2 != at<U8>(rules(), 0xaa)) return 1;                                     // 004E1748..64
    if (i3 != 0) return 1;                                                        // 004E176A..70
    if (i4 != 0) return 1;                                                        // 004E1776..78
    if (kind == 0) {                                                              // 004E177E..84
        at<U8>(stats, 0x101) = static_cast<U8>(at<U8>(stats, 0x101) - static_cast<U8>(amount)); // 004E1786..95
        return 1;
    }
    U best = 0;
    for (; index < count_of(creature); ++index) {                                 // 004E17A5..B3, 004E182E..3B
        void* e = effect_at(creature, index);
        if (type_of(e) != 0x31) break;                                            // 004E17C9..CE
        if (e == effect) continue;                                                // 004E17D0..D2
        if (gi(e, 0) != kind) continue;                                           // 004E17D4..DF
        const U8 aa = at<U8>(rules(), 0xaa);                                      // 004E17E7..F0
        if (gi(effect_at(creature, index), 2) != aa) continue;                    // 004E17F6..802
        if (static_cast<I>(gi(effect_at(creature, index), 1)) > static_cast<I>(best)) // 004E1804..18 (jle)
            best = gi(effect_at(creature, index), 1);                            // 004E181A..2A
    }
    switch (kind - 1) {                                                           // 004E183D..47 jump table 004E18B4
    case 0: at<U8>(stats_of(creature), 0xff) = static_cast<U8>(best); break;     // 004E184E
    case 1: at<U8>(stats_of(creature), 0xf9) = static_cast<U8>(best); break;     // 004E1867
    case 2: at<U8>(stats_of(creature), 0xfd) = static_cast<U8>(best); break;     // 004E1880
    case 3: at<U8>(stats_of(creature), 0xfb) = static_cast<U8>(best); break;     // 004E1899
    default: break;
    }
    return 1;
}

// =====================================================================================================================
// 004E18D0 OnRemoveDamageImmunityIncrease / 004E19A0 OnRemoveDamageImmunityDecrease (object, effect), RET 8
//   Identical bodies except the virtual slot (B4h / B0h, result discarded).  The sum includes the effect being
//   removed and then subtracts its amount once more (both variants subtract): original behaviour kept.
// =====================================================================================================================
namespace {
U remove_damage_immunity(void* object, void* effect, U slot) {
    if (effect == nullptr) return 1;                                              // 004E18D6..D8
    const U type = gi(effect, 0);                                                 // 004E18E1..EE (EBP)
    const U amount = gi(effect, 1);                                               // 004E18EA..F9 [esp+10]
    (void)vcall<U>(object, slot, type);                                           // 004E18FD..902
    U total = 0;                                                                  // 004E1910 (EBX)
    for (I i = 0; i < count_of(object); ++i) {                                    // 004E1908..18, 004E196C..7D
        void* e = effect_at(object, i);                                           // 004E1920..26
        bool match = false;
        if (type_of(e) == 0x10 && gi(e, 0) == type) match = true;                 // 004E1929..3B
        else if (type_of(e) == 0x11 && gi(e, 0) == type) match = true;            // 004E193D..4F
        if (!match) continue;
        if (type_of(e) == 0x10) total += gi(e, 1);                                // 004E1951..63
        else total -= gi(e, 1);                                                   // 004E1965..6A
    }
    total = (total & 0xffffff00u) | static_cast<U8>(static_cast<U8>(total) - static_cast<U8>(amount)); // 004E197F
    kotor_fastcall_004caf20(object, nullptr, type, total);                        // 004E1983..87
    return 1;
}
}  // namespace
extern "C" unsigned __fastcall kotor_fastcall_004e18d0(void*, void*, void* object, void* effect) {
    return remove_damage_immunity(object, effect, 0xb4);                          // 004E1902 call [eax+B4]
}
extern "C" unsigned __fastcall kotor_fastcall_004e19a0(void*, void*, void* object, void* effect) {
    return remove_damage_immunity(object, effect, 0xb0);                          // 004E19D2 call [eax+B0]
}

// =====================================================================================================================
// 004E1A70 CSWSEffectListHandler::OnRemoveSetAIState(object, effect), RET 8
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e1a70(void*, void*, void* object, void* effect) {
    if (as_creature(object) == nullptr) return 1;                                 // 004E1A71..7E
    void* creature = as_creature(object);                                         // 004E1A80..87
    if (at<U16>(creature, 0x9f0) == 0) return 1;                                  // 004E1A89..91
    U mask = 0xffff;                                                              // 004E1A9C (EBX)
    if (count_of(creature) > 0) {                                                 // 004E1A93..A1
        for (I i = at<U16>(stats_of(creature), 0x138); i < count_of(creature); ++i) { // 004E1AA3..B4, 004E1ADE..E7
            void* e = effect_at(creature, i);
            if (type_of(e) != 0x17) break;                                        // 004E1AC9..CD
            if (e != effect) mask &= gi(e, 0);                                    // 004E1ACF..DC
        }
    }
    at<U16>(creature, 0x9f0) = static_cast<U16>(mask);                            // 004E1AEB
    return 1;
}

// =====================================================================================================================
// 004E1B00 CSWSEffectListHandler::OnRemoveEnemyAttackBonus(object, effect), RET 8
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e1b00(void*, void*, void* object, void* effect) {
    if (as_creature(object) == nullptr) return 1;                                 // 004E1B01..0E
    void* creature = as_creature(object);                                         // 004E1B10..17 (ESI)
    U total = 0;                                                                  // 004E1B20 (EBX)
    if (effect != nullptr && count_of(creature) > 0) {                            // 004E1B22..30
        for (I i = 0; i < count_of(creature); ++i) {                              // 004E1B32, 004E1B54..5D
            void* e = effect_at(creature, i);
            const U16 t = type_of(e);
            if (t > 0x18) break;                                                  // 004E1B3F..43
            if (t == 0x18 && e != effect) total += gi(e, 0);                      // 004E1B45..52
        }
    }
    at<U8>(stats_of(creature), 0x103) = static_cast<U8>(total);                  // 004E1B5F..67
    return 1;
}

// =====================================================================================================================
// 004E1B80 CSWSEffectListHandler::OnRemoveArcaneSpellFailure(object, effect), RET 8
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e1b80(void*, void*, void* object, void* effect) {
    if (as_creature(object) == nullptr) return 1;                                 // 004E1B81..8E
    void* creature = as_creature(object);                                         // 004E1B90..9A (ESI)
    U total = 0;                                                                  // 004E1BA2 (EBP)
    if (effect != nullptr && count_of(creature) > 0) {                            // 004E1B9C..B0
        for (I i = 0; i < count_of(creature); ++i) {                              // 004E1BB2, 004E1BD6..DF
            void* e = effect_at(creature, i);
            const U16 t = type_of(e);
            if (t > 0x19) break;                                                  // 004E1BBB..C3
            if (t == 0x19 && e != effect) total += gi(e, 0);                      // 004E1BC5..D4
        }
    }
    if (as_creature(object) != nullptr) at<U>(as_creature(object), 0x344) = 1;    // 004E1BE1..F3
    (void)slot_void_return_4_0060e760_a1(stats_of(creature), nullptr, total);     // 004E1BFD..04 (void_return, RET 4)
    return 1;
}

// =====================================================================================================================
// 004E1C20 CSWSEffectListHandler::OnApplySetState(object, effect, load_game), RET 0Ch   (Endar cutscene paralyze/stun)
//   returns 1 = rejected (immunity / link immunity), 0 = applied
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e1c20(void*, void*, void* object, void* effect, unsigned load_game) {
    void* creature = as_creature(object);                                         // 004E1C38..47 [esp+14]
    void* attack = nullptr;                                                       // 004E1C4B [esp+10]
    if (creature == nullptr) return 0;                                            // 004E1C53
    if (effect == nullptr) return 0;                                              // 004E1C59..65
    void* stats = stats_of(creature);                                             // 004E1C5F (EDI)
    const U linked = gi(effect, 1);                                               // 004E1C6B..76 [esp+18]
    if (linked == 0 && stats != nullptr &&                                        // 004E1C7A..7E
        kotor_fastcall_005a6a90(stats, nullptr, effect) != 0) return 1;            // 004E1C80..8A -> 004E2196
    void* creator = kotor_fastcall_004ae770(server_app(), nullptr, at<U>(effect, 0x18)); // 004E1C90..A6 (ESI)
    const I state = static_cast<I>(gi(effect, 0));                                // 004E1CA2..AF (EBX)
    if (creator != nullptr) {                                                     // 004E1CAD..B1
        void* round = at<void*>(creator, 0x9c8);                                  // 004E1CB3..BB
        if (round != nullptr) {
            attack = recovered_combat_round_attack_004d2b50(round, nullptr, at<U8>(round, 0x96c)); // 004E1CBD..CC
            if (attack != nullptr) at<U8>(attack, 0x12d) = static_cast<U8>(state); // 004E1CD0..D2
        }
    }
    U message = 0;                                                                // feedback id, 0 = apply
    switch (state) {
    case 6:                                                                       // 004E1CD8..EE
        if (recovered_stats_effect_query_005a6960(stats, nullptr, 0xd, creator) == 1) message = 0x87;
        break;
    case 5:                                                                       // 004E1D90..BE
        if (linked != 0) break;
        if (recovered_stats_effect_query_005a6960(stats, nullptr, 6, creator) != 0 ||
            recovered_stats_has_feat_005a6630(stats, nullptr, 0x64) != 0) message = 0x82;
        break;
    case 4:                                                                       // 004E1E66..A1
        if (linked != 0) break;
        if (recovered_stats_effect_query_005a6960(stats, nullptr, 0xc, creator) != 0 ||
            recovered_stats_has_feat_005a6630(stats, nullptr, 0x63) != 0 ||
            recovered_stats_has_feat_005a6630(stats, nullptr, 0x64) != 0) message = 0x8a;
        break;
    case 1:                                                                       // 004E1F49..5E (cmp eax,ebx = 1)
        if (recovered_stats_effect_query_005a6960(stats, nullptr, 0x10, creator) == 1) message = 0x89;
        break;
    case 2:                                                                       // 004E2006..1B
        if (recovered_stats_effect_query_005a6960(stats, nullptr, 4, creator) != 0) message = 0x80;
        break;
    case 8:                                                                       // 004E20C3..E3
        if (linked != 0) break;
        if (recovered_stats_has_feat_005a6630(stats, nullptr, 0x62) != 0) message = 0xe3;
        break;
    default: break;                                                               // 004E20C6 -> 004E21A0
    }
    if (message != 0) {                                                           // feedback blocks 004E1CF4..2191
        immune_feedback(object, creator, message, attack);
        return 1;                                                                 // 004E2196
    }
    // 004E21A0: apply a SetAIState (17h) copy with integer 0 = FFFFFEFFh
    void* copy = new_effect_copy(effect);                                         // 004E21A0..C9 (ESI)
    at<U16>(copy, 8) = 0x17;                                                      // 004E21DB
    si(copy, 0, 0xfffffeffu);                                                     // 004E21CB..E1
    recovered_effect_apply_004d14f0(creature, nullptr, copy, load_game, 0);       // 004E21E6..F4
    if (state <= static_cast<I>(at<U8>(creature, 0x8ed))) return 0;               // 004E21F9..202 (jle)
    at<U8>(creature, 0x8ed) = static_cast<U8>(state);                             // 004E220E
    for (I i = 0; i < count_of(object); ++i) {                                    // 004E2214..21E, 004E2245..4E
        void* e = effect_at(object, i);                                           // 004E2220..26
        const U16 t = type_of(e);
        if (t > 9) break;                                                         // 004E2229..31
        if (t == 9) (void)recovered_object_remove_effect_004d06c0(creature, nullptr, at<U>(e, 0), at<U>(e, 4)); // 004E2235..40
        // quirk kept: the index advances after a removal as well
    }
    void* state_effect = new_effect(1);                                           // 004E2250..78 (ESI)
    recovered_effect_creator_00503a00(state_effect, nullptr, at<U>(effect, 0x18)); // 004E227C..8A
    const U16 subtype = at<U16>(state_effect, 0xa);                               // 004E228F..91
    at<U>(state_effect, 0x20) = 0;                                                // 004E22A7
    at<U16>(state_effect, 0xa) = static_cast<U16>((subtype & 0xfff8) | k_duration_innate_007473c8); // 004E229A..AE
    at<U16>(state_effect, 8) = 9;                                                 // 004E22B2
    at<U>(state_effect, 0x1c) = at<U>(effect, 0x1c);                              // 004E22B8..BB
    si(state_effect, 0, static_cast<U>(state));                                   // 004E2295..BE
    recovered_effect_apply_004d14f0(creature, nullptr, state_effect, load_game, 0); // 004E22C3..CF
    return 0;                                                                     // 004E22D4
}

// =====================================================================================================================
// 004E22F0 CSWSEffectListHandler::OnRemoveSetState(object, effect), RET 8   (Endar)
//   quirk kept: the type-9 removal loop does not advance after a removal (relies on RemoveEffectById shrinking the list)
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e22f0(void*, void*, void* object, void* effect) {
    void* creature = as_creature(object);                                         // 004E2308..14 (EBP)
    if (creature == nullptr) return 1;                                            // 004E2316..18
    at<U>(object, 0xe8) = 1;                                                      // 004E231E
    at<U8>(creature, 0x8ed) = 0;                                                  // 004E2328
    if (count_of(object) <= 0) return 1;                                          // 004E232F..37
    I i = 0;                                                                      // 004E233D (EDI)
    I best = 0;                                                                   // 004E233F (EBX)
    U creator = 0x7f000000u;                                                      // 004E2343 [esp+20]
    while (i < count_of(object)) {                                                // 004E2341..4B, 004E2398..A1
        void* e = effect_at(object, i);                                           // 004E2350..56
        const U16 t = type_of(e);
        if (t > 8) break;                                                         // 004E2359..61
        if (t == 8 && e != effect &&                                              // 004E2363..69
            static_cast<I>(gi(e, 0)) > best) {                                    // 004E236B..74 (jle)
            best = static_cast<I>(gi(effect_at(object, i), 0));                   // 004E2376..86
            creator = at<U>(effect_at(object, i), 0x18);                          // 004E2388..94
        }
        ++i;
    }
    while (i < count_of(object)) {                                                // 004E23A3..A9, 004E23D6..DC
        void* e = effect_at(object, i);                                           // 004E23B0..B6
        const U16 t = type_of(e);
        if (t > 9) break;                                                         // 004E23B9..C1
        if (t == 9) (void)recovered_object_remove_effect_004d06c0(creature, nullptr, at<U>(e, 0), at<U>(e, 4)); // 004E23C5..CE
        else ++i;                                                                 // 004E23D5
    }
    if (best == 0) return 1;                                                      // 004E23DE..E0
    at<U8>(creature, 0x8ed) = static_cast<U8>(best);                              // 004E23E6
    at<U>(object, 0xe8) = 0;                                                      // 004E23F1
    void* state_effect = new_effect(1);                                           // 004E23EC..1E (ESI)
    recovered_effect_creator_00503a00(state_effect, nullptr, creator);            // 004E2420..2F
    const U16 subtype = at<U16>(state_effect, 0xa);                               // 004E2434..36
    at<U>(state_effect, 0x20) = 0;                                                // 004E244B
    at<U16>(state_effect, 0xa) = static_cast<U16>((subtype & 0xfff8) | k_duration_permanent_007473c4); // 004E243E..4E
    at<U16>(state_effect, 8) = 9;                                                 // 004E2452
    si(state_effect, 0, static_cast<U>(best));                                    // 004E243A..58
    recovered_effect_apply_004d14f0(creature, nullptr, state_effect, 0, 0);       // 004E245D..62
    return 1;                                                                     // 004E246E
}

// =====================================================================================================================
// 004E2490 CSWSEffectListHandler::OnRemoveMovementSpeedChange(object, effect), RET 8
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e2490(void*, void*, void* object, void* effect) {
    if (as_creature(object) == nullptr) return 1;                                 // 004E2492..9F
    (void)as_creature(object);                                                    // 004E24A5..A9
    if (effect == nullptr) return 1;                                              // 004E24AC..B2
    float factor = flt_of(0x3f800000u);                                           // 004E24BD [esp+10] = 1.0f
    void* creature = as_creature(object);                                         // 004E24B8..C8 (ESI)
    const double step = static_cast<double>(flt_of(k_speed_step_0073dfec));
    for (I i = 0; i < count_of(creature); ++i) {                                  // 004E24CA..D4, 004E2546..4F
        void* e = effect_at(creature, i);                                         // 004E24E0..E6
        if (type_of(e) > 0x1d) break;                                             // 004E24E9..EE
        if (same_id(e, effect)) continue;                                         // 004E24F0..FC
        const U16 t = type_of(e);                                                 // 004E24FE
        if (t == 0x1c) {                                                          // 004E2502..21: fild, fmul, fadd, fstp dword
            const I v = static_cast<I>(gi(e, 0));
            factor = static_cast<float>(static_cast<double>(v) * step + static_cast<double>(factor));
        } else if (t == 0x1d) {                                                   // 004E2523..42: fild, fmul, fsubr, fstp dword
            const I v = static_cast<I>(gi(e, 0));
            factor = static_cast<float>(static_cast<double>(factor) - static_cast<double>(v) * step);
        }
    }
    kotor_fastcall_004ec3b0(creature, nullptr, factor);                           // 004E2551..58
    return 1;
}

// =====================================================================================================================
// 004E2570 CSWSEffectListHandler::OnRemoveForceResistanceIncrease(object, effect), RET 8
//   value = max(21h integer 0) - (last 22h integer 0), low byte only (sub bl, byte)
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e2570(void*, void*, void* object, void* effect) {
    if (as_creature(object) == nullptr) return 1;                                 // 004E2572..7F
    void* stats = stats_of(as_creature(object));                                  // 004E2585..94 [esp+C]
    U best = 0;                                                                   // 004E25A0 (EBX)
    U last22 = 0;                                                                 // 004E25A4 [esp+14]
    if (count_of(object) > 0) {                                                   // 004E2598..A8
        for (I i = 0; i < count_of(object); ++i) {                                // 004E25B0, 004E260C..15
            void* e = effect_at(object, i);                                       // 004E25B0..B6
            if (same_id(e, effect)) continue;                                     // 004E25B9..C6
            const U16 t = type_of(e);                                             // 004E25C8
            if (t == 0x21) {                                                      // 004E25CC..D0 -> 004E25DC..EF
                if (static_cast<I>(gi(e, 0)) > static_cast<I>(best)) best = gi(effect_at(object, i), 0); // 004E25EF..0A
            } else if (t == 0x22) {                                               // 004E25D2..D6
                last22 = gi(e, 0);                                                // 004E25E2..E9
            } else if (t > 0x22) {
                break;                                                            // 004E25D8
            }
        }
    }
    if (as_creature(object) != nullptr) at<U>(as_creature(object), 0x344) = 1;    // 004E2618..2A
    const U value = (best & 0xffffff00u) | static_cast<U8>(static_cast<U8>(best) - static_cast<U8>(last22)); // 004E2634
    kotor_fastcall_005afac0(stats, nullptr, value);                               // 004E2638..3D
    return 1;
}

// =====================================================================================================================
// 004E2650 CSWSEffectListHandler::OnRemoveForceResistanceDecrease(object, effect), RET 8
//   quirk kept: 22h entries call GetInteger(0) and discard it; the value is the max over 21h entries (pointer skip)
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e2650(void*, void*, void* object, void* effect) {
    if (as_creature(object) == nullptr) return 1;                                 // 004E2651..5E
    void* stats = stats_of(as_creature(object));                                  // 004E2664..73 [esp+10]
    U best = 0;                                                                   // 004E267D (EBX)
    if (count_of(object) > 0) {                                                   // 004E2677..83
        for (I i = 0; i < count_of(object); ++i) {                                // 004E2690, 004E26DB..E4
            void* e = effect_at(object, i);
            const U16 t = type_of(e);
            if (t == 0x21 || t == 0x22) {                                         // 004E269D..A7
                if (e == effect) continue;                                        // 004E26AD..AF
                if (t == 0x22) { (void)gi(e, 0); continue; }                      // 004E26B1..BE
                if (static_cast<I>(gi(e, 0)) > static_cast<I>(best)) best = gi(effect_at(object, i), 0); // 004E26C0..D9
            } else if (t > 0x22) {
                break;                                                            // 004E26A9
            }
        }
    }
    if (as_creature(object) != nullptr) at<U>(as_creature(object), 0x344) = 1;    // 004E26E7..F9
    kotor_fastcall_005afae0(stats, nullptr, best);                                // 004E2703..08
    return 1;
}

// =====================================================================================================================
// 004E2720 CSWSEffectListHandler::OnApplyDisease(object, effect, load_game), RET 0Ch
//   quirk kept: the disease.2da "Name" integer is read into a local that is never used
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e2720(void*, void*, void* object, void* effect, unsigned load_game) {
    if (as_creature(object) == nullptr) return 1;                                 // 004E273B..49
    if (load_game == 0) {                                                         // 004E274F..59 (else -> 004E2A28)
        const U16 subtype = at<U16>(effect, 0xa);                                 // 004E275F..61
        if (static_cast<U16>(static_cast<U8>(subtype) & 7) == k_duration_instant_007473b8) // 004E2765..73
            at<U16>(effect, 0xa) = static_cast<U16>((subtype & 0xfff8) | k_duration_permanent_007473c4); // 004E2775..81
        void* table = game_rules_table(0x94);                                     // 004E2785..91 (EBP) disease.2da
        void* timer = kotor_fastcall_004aede0(server_app(), nullptr);              // 004E2797..A8 [esp+4C -> arg1 slot]
        const U row = gi(effect, 0);                                              // 004E27A4..B5 [arg2 slot]
        if (at<U>(as_creature(object), 0xf8) != 0) return 1;                      // 004E27B1..C4
        void* creator = kotor_fastcall_004ae770(server_app(), nullptr, at<U>(effect, 0x18)); // 004E27CA..E0 (EBX)
        if (recovered_stats_effect_query_005a6960(stats_of(as_creature(object)), nullptr, 3, creator) != 0) { // 004E27DC..F7
            immune_feedback(object, creator, 0x7d, nullptr);                      // 004E27FD..883
            return 1;
        }
        for (I i = 0; i < count_of(object); ++i)                                  // 004E28A2..C3
            if (type_of(effect_at(object, i)) == 5) return 1;                     // 004E28B4..BB
        ExoString column;
        I first_save = 0;                                                         // [esp+10]
        exo_literal(&column, "First_Save");                                       // 004E28C5..CE (0746AE4)
        try { (void)c2da_current_get_int_00413660(table, nullptr, row, &column, &first_save); } // 004E28D3..EC
        catch (...) { exo_destroy(&column); throw; }
        exo_destroy(&column);                                                     // 004E28F1..FD
        if (static_cast<U8>(kotor_fastcall_005b92b0(as_creature(object), nullptr, 1, static_cast<U>(first_save), 5,
                at<U>(effect, 0x18), 1, 0, 0)) != 0) return 1;                     // 004E2902..25
        I name = 0;                                                               // [esp+28] (unused)
        exo_literal(&column, "Name");                                             // 004E292B..34 (0745EEC)
        try { (void)c2da_current_get_int_00413660(table, nullptr, row, &column, &name); } // 004E2939..4E
        catch (...) { exo_destroy(&column); throw; }
        exo_destroy(&column);                                                     // 004E2953..5F
        (void)name;
        if (as_creature(object) != nullptr) kotor_fastcall_004ef560(as_creature(object), nullptr, 0); // 004E2964..7A
        U day = 0, time = 0;                                                      // [esp+1C], [esp+18]
        frame_GetWorldTime_004ade40(timer, nullptr, &day, &time);                 // 004E297F..8D
        I hours = 0;                                                              // [esp+14]
        exo_literal(&column, "Incu_Hours");                                       // 004E2992..9B (0746AD8)
        try { (void)c2da_current_get_int_00413660(table, nullptr, row, &column, &hours); } // 004E29A0..B5
        catch (...) { exo_destroy(&column); throw; }
        exo_destroy(&column);                                                     // 004E29BA..C6
        void* module = server_get_module_004ae6b0(server_app(), nullptr);          // 004E29CB..D4
        const U delta = static_cast<U>(at<U8>(module, 0x1a0)) * k_ms_per_minute_00744178 * static_cast<U>(hours); // 004E29D9..E7 (imul)
        U out_day = 0, out_time = 0;                                              // [esp+20], [esp+24]
        (void)kotor_fastcall_004adf50(timer, nullptr, day, time, 0, delta, &out_day, &out_time); // 004E29EC..A07
        si(effect, 3, out_day);                                                   // 004E2A0C..15
        si(effect, 4, out_time);                                                  // 004E2A1A..23
    }
    // 004E2A28: apply a copy typed 1Eh with integer 0 = 33h (visual effect)
    void* copy = new_effect_copy(effect);                                         // 004E2A28..51 (EDI)
    at<U16>(copy, 8) = 0x1e;                                                      // 004E2A61
    si(copy, 0, 0x33);                                                            // 004E2A53..67
    recovered_effect_apply_004d14f0(object, nullptr, copy, load_game, 0);         // 004E2A6C..76
    return 0;                                                                     // 004E2A7B
}

// =====================================================================================================================
// 004E2A90 CSWSEffectListHandler::OnApplyHasteOrSlow(object, effect, load_game), RET 0Ch
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e2a90(void*, void*, void* object, void* effect, unsigned load_game) {
    if (as_creature(object) == nullptr) return 0;                                 // 004E2AA8..B8
    const U haste = type_of(effect) == 1 ? 1u : 0u;                               // 004E2ABE..D9 (EBX)
    void* creator = kotor_fastcall_004ae770(server_app(), nullptr, at<U>(effect, 0x18)); // 004E2ACC..E5 (ESI)
    if (haste == 0 &&                                                             // 004E2AE3..E7
        recovered_stats_effect_query_005a6960(stats_of(as_creature(object)), nullptr, 9, creator) != 0) { // 004E2AED..07
        immune_feedback(object, creator, 0x92, nullptr);                          // 004E2B0D..9B
        return 1;                                                                 // 004E2BA0
    }
    const I n = count_of(object);                                                 // 004E2BAA (EDI, cached)
    I net = 0;                                                                    // ECX
    for (I i = 0; i < n; ++i) {                                                   // 004E2BB2..DE
        const U t = type_of(effect_at(object, i));
        if (t == 1) ++net;                                                        // 004E2BC6..CC
        else if (t == 3) --net;                                                   // 004E2BCE..D4
        else if (static_cast<I>(t) > 3) break;                                    // 004E2BD6 (jg)
    }
    I after = (haste != 0 ? 1 : -1) + net;                                        // 004E2BE0..F0 (ESI)
    if (after >= 1) after = 1; else if (after <= -1) after = -1;                  // 004E2BF2..FB, 004E2C4F
    if (net >= 1) net = 1; else if (net <= -1) net = -1;                          // 004E2BFD..06, 004E2C56
    if (net == after) return 0;                                                   // 004E2C08..0A
    if (net > 0) {                                                                // 004E2C10..12: drop the haste marker
        for (I i = 0; i < n; ++i) {                                               // 004E2C14..48
            void* e = effect_at(object, i);
            const U16 t = type_of(e);
            if (t == 0x29) { (void)recovered_object_remove_effect_004d06c0(object, nullptr, at<U>(e, 0), at<U>(e, 4)); break; } // 004E2C5D..69
            if (t > 0x29) break;                                                  // 004E2C3C
        }
        return 0;
    }
    if (net < 0) {                                                                // 004E2C73: drop the slow marker
        for (I i = 0; i < n; ++i) {                                               // 004E2C75..A8
            void* e = effect_at(object, i);
            const U16 t = type_of(e);
            if (t == 0x2a) { (void)recovered_object_remove_effect_004d06c0(object, nullptr, at<U>(e, 0), at<U>(e, 4)); break; }
            if (t > 0x2a) break;                                                  // 004E2C9C
        }
        return 0;
    }
    void* marker = new_effect(1);                                                 // 004E2CAC..D4 (EAX)
    const U16 subtype = at<U16>(marker, 0xa);                                     // 004E2CDA..DC
    at<U16>(marker, 0xa) = static_cast<U16>((((subtype & 0xfff8) | k_duration_innate_007473c8) & 0xffef) | 8); // 004E2CE6..FC
    at<U>(marker, 0x1c) = at<U>(effect, 0x1c);                                    // 004E2D00..0A
    at<U>(marker, 0x20) = 0;                                                      // 004E2D17
    at<U16>(marker, 8) = static_cast<U16>(0x29 + (after <= 0 ? 1 : 0));           // 004E2D03..21
    recovered_effect_apply_004d14f0(object, nullptr, marker, load_game, 0);       // 004E2CE0..D25
    return 0;                                                                     // 004E2D2A
}

// =====================================================================================================================
// 004E2D50 CSWSEffectListHandler::OnRemoveHasteOrSlow(object, effect), RET 8
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e2d50(void*, void*, void* object, void* effect) {
    if (as_creature(object) == nullptr) return 1;                                 // 004E2D68..76
    at<U>(as_creature(object), 0x8cc) = 0;                                        // 004E2D7C..85
    at<U>(as_creature(object), 0x8d0) = 0;                                        // 004E2D8B..96
    const U haste = type_of(effect) == 1 ? 1u : 0u;                               // 004E2D92..B1 (EBP)
    const I n = count_of(object);                                                 // 004E2DA0 (EBX, cached)
    I net = 0;
    for (I i = 0; i < n; ++i) {                                                   // 004E2DB5..DE
        const U t = type_of(effect_at(object, i));
        if (t == 1) ++net;
        else if (t == 3) --net;
        else if (static_cast<I>(t) > 3) break;                                    // 004E2DD6 (jg)
    }
    I after = (haste == 0 ? 1 : -1) + net;                                        // 004E2DE0..EB
    if (after >= 1) after = 1; else if (after <= -1) after = -1;                  // 004E2DEE..08 (EDI)
    if (net >= 1) net = 1; else if (net <= -1) net = -1;                          // 004E2E0B..15, 004E2E5F
    if (net == after) return 1;                                                   // 004E2E18..1A
    if (net > 0) {                                                                // 004E2E22..24
        for (I i = 0; i < n; ++i) {                                               // 004E2E26..58
            void* e = effect_at(object, i);
            const U16 t = type_of(e);
            if (t == 0x29) { (void)recovered_object_remove_effect_004d06c0(object, nullptr, at<U>(e, 0), at<U>(e, 4)); break; } // 004E2E9C..A8
            if (t > 0x29) break;
        }
        return 1;
    }
    if (net < 0) {                                                                // 004E2E66
        for (I i = 0; i < n; ++i) {                                               // 004E2E68..98
            void* e = effect_at(object, i);
            const U16 t = type_of(e);
            if (t == 0x2a) { (void)recovered_object_remove_effect_004d06c0(object, nullptr, at<U>(e, 0), at<U>(e, 4)); break; }
            if (t > 0x2a) break;
        }
        return 1;
    }
    void* marker = new_effect(1);                                                 // 004E2EAF..D3
    const U16 subtype = at<U16>(marker, 0xa);                                     // 004E2ED5..D7
    at<U>(marker, 0x20) = 0;                                                      // 004E2EF5
    at<U16>(marker, 0xa) = static_cast<U16>((((subtype & 0xfff8) | k_duration_innate_007473c8) & 0xffef) | 8); // 004E2EE8..F06
    at<U16>(marker, 8) = static_cast<U16>(0x29 + (after <= 0 ? 1 : 0));           // 004E2F01..0F
    recovered_effect_apply_004d14f0(object, nullptr, marker, 0, 0);               // 004E2EDD..F13
    return 1;
}

// =====================================================================================================================
// 004E2F40 CSWSEffectListHandler::OnApplyVision(object, effect, load_game), RET 0Ch
//   original behaviour kept: builds a 1Eh copy and destroys it again without applying it
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e2f40(void*, void*, void* object, void* effect, unsigned) {
    if (as_creature(object) == nullptr) return 1;                                 // 004E2F46..61
    (void)gi(effect, 0);                                                          // 004E2F63..6B
    void* copy = new_effect_copy(effect);                                         // 004E2F70..99
    if (copy != nullptr) {                                                        // 004E2F9B..A5
        at<U16>(copy, 8) = 0x1e;                                                  // 004E2FA9
        recovered_effect_destroy_00503590(copy, nullptr);                         // 004E2FA7..AF
        live_free(copy);                                                          // 004E2FB4..BA
    }
    return 1;
}

// =====================================================================================================================
// 004E2FE0 CSWSEffectListHandler::OnRemoveLimitMovementSpeed(object, effect), RET 8
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e2fe0(void*, void*, void* object, void* effect) {
    if (as_creature(object) == nullptr) return 1;                                 // 004E2FE1..EE
    void* creature = as_creature(object);                                         // 004E2FF0..F6
    const I n = count_of(creature);                                               // 004E2FF9 (ESI, cached)
    U limited = 0;                                                                // 004E3004 (EBP)
    if (n > 0) {
        void** list = at<void**>(creature, 0x124);                                // 004E2FFF, 004E3011
        for (I i = 0; i < n; ++i) {                                               // 004E300B..2D
            void* e = list[i];
            const U16 t = type_of(e);
            if (t > 0x3b) break;                                                  // 004E3019..1D
            if (t == 0x3b && e != effect) { limited = 1; break; }                 // 004E301F..31
        }
    }
    at<U>(as_creature(object), 0x8e8) = limited;                                  // 004E3037..3F
    return 1;
}

// =====================================================================================================================
// 004E3050 CSWSEffectListHandler::OnRemoveSpecialWalkAnimation(object, effect), RET 8
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e3050(void*, void*, void* object, void* effect) {
    if (object == nullptr) return 1;                                              // 004E3055..57
    if (as_creature(object) == nullptr) return 1;                                 // 004E3059..62
    void* creature = as_creature(object);                                         // 004E3064..71 [arg1 slot]
    U8 walk = 0;                                                                  // 004E3075 (AL)
    // 004E306B..79: &creature->+124 is never null
    const I n = count_of(creature);                                               // 004E307C (EBX, cached)
    if (n > 0) {
        void** list = at<void**>(creature, 0x124);                                // 004E308B
        for (I i = 0; i < n; ++i) {                                               // 004E3085..A9
            void* e = list[i];
            if (e == effect) continue;                                            // 004E3093..95
            const U16 t = type_of(e);
            if (t == 0x3a) { walk = static_cast<U8>(gi(e, 0)); break; }           // 004E309B..AF
            if (t > 0x3a) break;                                                  // 004E30A1
        }
    }
    at<U8>(creature, 0xa10) = walk;                                               // 004E30B7..BB
    return 1;
}

// =====================================================================================================================
// 004E30D0 CSWSEffectListHandler::OnRemoveEffectIcon(object, effect), RET 8   (Endar)
//   quirk kept: the icon search walks down from the last entry with a 16-bit index and no lower bound
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e30d0(void*, void*, void* object, void* effect) {
    if (as_creature(object) == nullptr) return 1;                                 // 004E30D1..DF
    unsigned char* icons = static_cast<unsigned char*>(as_creature(object)) + 0x8f4; // 004E30E5..F4 (EDI) CExoArrayList<icon*>
    U16 index = at<U16>(stats_of(as_creature(object)), 0x13c);                    // 004E30F0..103 (BP)
    const U8 icon = static_cast<U8>(gi(effect, 0));                               // 004E310A..1B (BL)
    U other = 0;                                                                  // 004E3122 [arg1 slot]
    for (; static_cast<I>(index) < count_of(object); ++index) {                   // 004E311D..2A, 004E315C..68
        void* e = effect_at(object, index);                                       // 004E3130..36
        if (type_of(e) != 0x43) break;                                            // 004E3139..3E
        if (e == effect) continue;                                                // 004E3140..44
        if (gi(e, 0) == static_cast<U>(icon)) other = 1;                          // 004E3146..54
    }
    void** data = at<void**>(icons, 0);                                           // 004E316E
    U16 slot = static_cast<U16>(at<U16>(icons, 4) - 1);                           // 004E316A..74
    while (static_cast<I>(*static_cast<std::int16_t*>(data[slot])) != static_cast<I>(icon)) // 004E3177..82 (movsx)
        slot = static_cast<U16>(slot - 1);                                        // 004E3184
    if (other != 0) return 1;                                                     // 004E318B..91
    I i = slot;                                                                   // 004E3193
    live_free(data[i]);                                                           // 004E3196..9A
    const I remaining = at<I>(icons, 4) - 1;                                      // 004E319F..A5
    at<I>(icons, 4) = remaining;                                                  // 004E31AC
    for (; i < at<I>(icons, 4); ++i)                                              // 004E31AA..C2
        at<void**>(icons, 0)[i] = at<void**>(icons, 0)[i + 1];                    // 004E31B1..BA
    return 1;
}

// =====================================================================================================================
// 004E31D0 CSWSEffectListHandler::OnApplyDisguise(object, effect, load_game), RET 0Ch
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e31d0(void*, void*, void* object, void* effect, unsigned load_game) {
    void* creature = as_creature(object);                                         // 004E31D0..DE (EBX)
    if (creature == nullptr) return 1;                                            // 004E31E2
    if (load_game == 0 && vcall<U>(creature, 0x94) != 0) return 1;                // 004E31E4..F8
    unsigned char* list = static_cast<unsigned char*>(creature) + 0x124;          // 004E3204..0C (never null)
    const I n = at<I>(list, 4);                                                   // 004E3218 (EDX, cached)
    for (I i = 0; i < n; ++i) {                                                   // 004E3221..54
        void* e = at<void**>(list, 0)[i];                                         // 004E3230
        if (type_of(e) != 0x3e) continue;                                         // 004E3232..37
        if (same_id(e, effect)) continue;                                         // 004E3239..48
        si(at<void**>(list, 0)[i], 1, 1);                                         // 004E3256..5F
        void* again = at<void**>(list, 0)[i];                                     // 004E3264..66
        (void)recovered_object_remove_effect_004d06c0(creature, nullptr, at<U>(again, 0), at<U>(again, 4)); // 004E3269..76
        break;
    }
    const U appearance = gi(effect, 0);                                           // 004E327B..7F
    (void)kotor_fastcall_004ee010(creature, nullptr, appearance, effect, load_game); // 004E3284..8D
    return 0;                                                                     // 004E3295
}

// =====================================================================================================================
// 004E32A0 CSWSEffectListHandler::OnRemoveSeeInvisible(object, effect), RET 8
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e32a0(void*, void*, void* object, void* effect) {
    if (as_creature(object) == nullptr) return 1;                                 // 004E32A1..AE
    void* creature = as_creature(object);                                         // 004E32B0..B6
    U flags = at<U8>(creature, 0x8ec) & 0xfffffffeu;                              // 004E32BF..D3
    const I n = count_of(creature);                                               // 004E32CD (EBP, cached)
    for (I i = at<U16>(stats_of(creature), 0x146); i < n; ++i) {                  // 004E32C6..D8, 004E32FB..301
        void* e = effect_at(creature, i);
        const U16 t = type_of(e);
        if (t > 0x46) break;                                                      // 004E32E6..EE
        if (t == 0x46 && e != effect) flags |= 1;                                 // 004E32F0..F8
    }
    at<U8>(creature, 0x8ec) = static_cast<U8>(flags);                             // 004E3305
    return 1;
}

// =====================================================================================================================
// 004E3320 CSWSEffectListHandler::OnApplyUltraVision(object, effect, load_game), RET 0Ch
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e3320(void*, void*, void* object, void* effect, unsigned load_game) {
    if (as_creature(object) == nullptr) return 0;                                 // 004E3338..46
    void* creature = as_creature(object);                                         // 004E334C..58 (ESI)
    void* copy = new_effect_copy(effect);                                         // 004E3353..82 (EDI)
    at<U16>(copy, 8) = 0x45;                                                      // 004E3392
    si(copy, 0, 3);                                                               // 004E3384..98
    recovered_effect_apply_004d14f0(creature, nullptr, copy, load_game, 0);       // 004E339D..A7
    at<U8>(creature, 0x8ec) = static_cast<U8>(at<U8>(creature, 0x8ec) | 2);       // 004E33AC..BB
    for (I i = at<U16>(stats_of(creature), 0x14e); i < count_of(creature); ++i) { // 004E33B2..CE, 004E3453..5C
        void* e = effect_at(creature, i);                                         // 004E33E0..E6 (EDI)
        const U16 t = type_of(e);
        if (t > 0x49) break;                                                      // 004E33E9..F2
        if (t != 0x49) continue;                                                  // 004E33F4
        if (gi(e, 0) != 8) continue;                                              // 004E33F6..402
        for (I j = 0; j < count_of(creature); ) {                                 // 004E3404..0E, 004E344A..51
            void* other = effect_at(creature, j);                                 // 004E3410..16
            if (type_of(other) > 0x4b) break;                                     // 004E3419..1D
            if (same_id(other, e) && other != e) {                                // 004E341F..2F
                recovered_object_remove_effect_004d05c0(creature, nullptr, other); // 004E3431..34
                i = at<U16>(stats_of(creature), 0x14e);                           // 004E3439..3F
                j = 0;                                                            // 004E3446
            } else {
                ++j;                                                              // 004E344A
            }
        }
    }
    return 0;                                                                     // 004E3465
}

// =====================================================================================================================
// 004E3480 CSWSEffectListHandler::OnRemoveUltraVision(object, effect), RET 8
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e3480(void*, void*, void* object, void* effect) {
    if (as_creature(object) == nullptr) return 1;                                 // 004E3498..A6
    void* creature = as_creature(object);                                         // 004E34AC..B3 (ESI)
    U flags = at<U8>(creature, 0x8ec) & 0xfffffffdu;                              // 004E34BB..CF
    const I n = count_of(creature);                                               // 004E34C9 (EBP, cached)
    for (I i = at<U16>(stats_of(creature), 0x148); i < n; ++i) {                  // 004E34C2..D4, 004E34F7..FD
        void* e = effect_at(creature, i);
        const U16 t = type_of(e);
        if (t > 0x47) break;                                                      // 004E34E2..EA
        if (t == 0x47 && e != effect) flags |= 2;                                 // 004E34EC..F4
    }
    at<U8>(creature, 0x8ec) = static_cast<U8>(flags);                             // 004E3505
    for (I i = at<U16>(stats_of(creature), 0x14e); i < count_of(creature); ++i) { // 004E34FF..18, 004E3582..8B
        void* e = effect_at(creature, i);                                         // 004E3520..26 (EDI)
        const U16 t = type_of(e);
        if (t > 0x49) break;                                                      // 004E3529..32
        if (t != 0x49) continue;                                                  // 004E3534
        void* again = new_effect(1);                                              // 004E3536..5A (EBX)
        recovered_effect_copy_00504090(again, nullptr, e, 0);                     // 004E355C..69
        recovered_object_remove_effect_004d05c0(creature, nullptr, e);            // 004E356E..71
        recovered_effect_apply_004d14f0(creature, nullptr, again, 0, 0);          // 004E3576..7D
    }
    return 1;
}

// =====================================================================================================================
// 004E35B0 CSWSEffectListHandler::OnApplyTrueSeeing(object, effect, load_game), RET 0Ch
//   quirk kept: on a duplicate id it removes the outer (49h) entry at the current outer index, resets the outer index
//   but continues the inner scan
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e35b0(void*, void*, void* object, void*, unsigned) {
    if (as_creature(object) == nullptr) return 0;                                 // 004E35B3..C1
    void* creature = as_creature(object);                                         // 004E35C7..CE (ESI)
    at<U8>(creature, 0x8ec) = static_cast<U8>(at<U8>(creature, 0x8ec) | 4);       // 004E35D0..DF
    for (I i = at<U16>(stats_of(creature), 0x14e); i < count_of(creature); ++i) { // 004E35E5..F2, 004E3675..7E
        void* e = effect_at(creature, i);                                         // 004E3600..06 (EDI)
        const U16 t = type_of(e);
        if (t > 0x49) break;                                                      // 004E3609..12
        if (t != 0x49) continue;                                                  // 004E3614
        if (gi(e, 0) != 8) continue;                                              // 004E3616..22
        for (I j = 0; j < count_of(creature); ++j) {                              // 004E3624..2E, 004E366A..73
            void** list = at<void**>(creature, 0x124);                            // 004E3630
            void* other = list[j];                                                // 004E3636
            if (type_of(other) > 0x4b) break;                                     // 004E3639..3E
            if (same_id(other, e) && other != e) {                                // 004E3640..50
                recovered_object_remove_effect_004d05c0(creature, nullptr, list[i]); // 004E3652..58
                i = at<U16>(stats_of(creature), 0x14e);                           // 004E365D..63
            }
        }
    }
    return 0;                                                                     // 004E3683
}

// =====================================================================================================================
// 004E3690 CSWSEffectListHandler::OnRemoveTrueSeeing(object, effect), RET 8
//   original bug kept: a remaining 48h effect sets bit 2 (the ultravision bit) instead of bit 4
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e3690(void*, void*, void* object, void* effect) {
    if (as_creature(object) == nullptr) return 1;                                 // 004E3693..A1
    void* creature = as_creature(object);                                         // 004E36A7..AE (ESI)
    U flags = at<U8>(creature, 0x8ec) & 0xfffffffbu;                              // 004E36B6..CA
    const I n = count_of(creature);                                               // 004E36C4 (EBP, cached)
    for (I i = at<U16>(stats_of(creature), 0x148); i < n; ++i) {                  // 004E36BD..CF, 004E36F7..FD
        void* e = effect_at(creature, i);
        const U16 t = type_of(e);
        if (t > 0x48) break;                                                      // 004E36E2..EA
        if (t == 0x48 && e != effect) flags |= 2;                                 // 004E36EC..F4
    }
    at<U8>(creature, 0x8ec) = static_cast<U8>(flags);                             // 004E3705
    for (I i = at<U16>(stats_of(creature), 0x14e); i < count_of(creature); ++i) { // 004E36FF..18, 004E3745..4E
        void* e = effect_at(creature, i);                                         // 004E3720..26
        const U16 t = type_of(e);
        if (t > 0x49) break;                                                      // 004E3729..2D
        if (t != 0x49) continue;                                                  // 004E372F
        recovered_object_remove_effect_004d05c0(creature, nullptr, e);            // 004E3731..34
        recovered_effect_apply_004d14f0(creature, nullptr, e, 0, 0);              // 004E3739..40
    }
    return 1;
}

// =====================================================================================================================
// 004E3760 CSWSEffectListHandler::OnRemoveBlindness(object, effect), RET 8
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e3760(void*, void*, void* object, void* effect) {
    if (as_creature(object) == nullptr) return 1;                                 // 004E3761..6E
    void* creature = as_creature(object);                                         // 004E3774..82 (ESI)
    const U8 old = at<U8>(creature, 0x8ec);                                       // 004E3784..8E
    U flags = ~gi(effect, 0) & static_cast<U>(old);                               // 004E378A..B3
    for (I i = at<U16>(stats_of(creature), 0x14e); i < count_of(creature); ++i) { // 004E3797..B7, 004E37E2..EB
        void* e = effect_at(creature, i);
        const U16 t = type_of(e);
        if (t > 0x49) break;                                                      // 004E37C9..D1
        if (t == 0x49 && e != effect) flags |= gi(e, 0);                          // 004E37D3..E0
    }
    at<U8>(creature, 0x8ec) = static_cast<U8>(flags);                             // 004E37EF
    return 1;
}

// =====================================================================================================================
// 004E3800 CSWSEffectListHandler::OnApplyForcePush(object, effect, load_game), RET 0Ch
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e3800(void*, void*, void* object, void* effect, unsigned) {
    void* creature = as_creature(object);                                         // 004E3806..24 (ESI)
    if (creature == nullptr) return 0;                                            // 004E3826..28
    void* source = kotor_fastcall_004ae750(server_app(), nullptr, at<U>(effect, 0x18)); // 004E382E..47 (EDI)
    if (source == nullptr) return 0;                                              // 004E3849..4B
    if (vcall<void*>(source, 0x10) == nullptr) return 0;                          // 004E3851..5A
    Vector point;                                                                 // [esp+24]
    U direct;                                                                     // EBX
    if (gi(effect, 0) != 0) {                                                     // 004E3861..6E
        point.x = kotor_fastcall_005036c0(effect, nullptr, 0);                    // 004E3870..78
        point.y = kotor_fastcall_005036c0(effect, nullptr, 1);                    // 004E387C..85
        point.z = kotor_fastcall_005036c0(effect, nullptr, 2);                    // 004E3889..92
        direct = gi(effect, 1);                                                   // 004E3896..9F
    } else {
        direct = 0;                                                               // 004E386A
        copy_vector(&point, static_cast<unsigned char*>(vcall<void*>(source, 0x10)) + 0x90); // 004E38A3..BF
    }
    float distance = kotor_fastcall_005036c0(effect, nullptr, 3);                 // 004E38C3..CC [esp+10]
    if (distance == flt_of(k_zero_0073d700)) distance = flt_of(0x40a00000u);      // 004E38D0..DD (equal -> 5.0f)
    Vector* position = &at<Vector>(creature, 0x90);                               // 004E38EB (EDI)
    Vector delta;                                                                 // [esp+24] reused
    delta.z = static_cast<float>(static_cast<double>(position->z) - static_cast<double>(point.z)); // 004E38E5..F1, 3914
    delta.y = static_cast<float>(static_cast<double>(position->y) - static_cast<double>(point.y)); // 004E38F9..FD, 3910
    delta.x = static_cast<float>(static_cast<double>(position->x) - static_cast<double>(point.x)); // 004E3905..08, 390C
    Vector direction;                                                             // [esp+30]
    (void)normalize_004ab180(&direction, &delta);                                 // 004E38F5..918
    const double dz = static_cast<double>(direction.z) * static_cast<double>(distance);            // 004E391D..24 (kept in ST)
    const float dy = static_cast<float>(static_cast<double>(direction.y) * static_cast<double>(distance)); // 004E392C..37
    const float dx = static_cast<float>(static_cast<double>(direction.x) * static_cast<double>(distance)); // 004E393B..43
    Vector target;                                                                // [esp+18]
    target.z = static_cast<float>(dz + static_cast<double>(position->z));         // 004E3947, 395F
    target.y = static_cast<float>(static_cast<double>(dy) + static_cast<double>(position->y)); // 004E394A..4E, 395B
    target.x = static_cast<float>(static_cast<double>(dx) + static_cast<double>(position->x)); // 004E3951..57
    Vector facing;                                                                // [esp+24] reused
    facing.x = -direction.x; facing.y = -direction.y; facing.z = -direction.z;    // 004E3963..7D (fchs)
    w1x_object_set_orientation_004cd170(creature, nullptr, &facing);              // 004E3928..81
    if (direct == 0) {                                                            // 004E3986..88
        WalkHit hit;                                                              // [esp+3C]
        (void)w5_walk_hit_construct_004b14b0(&hit, nullptr);                      // 004E398E..92
        void* pathfind = at<void*>(creature, 0x340);                              // 004E3997
        float radius = at<float>(pathfind, 4);                                    // 004E399D..A0 [esp+10]
        const float band = at<float>(pathfind, 0x14);                             // 004E39A4..A9 [esp+14]
        void* area = recovered_object_area_004cb120(creature, nullptr);           // 004E39A7..AD
        const I result = w5_server_test_direct_line_004aee80(server_app(), nullptr, at<U>(creature, 4),
            ptr(word(area) + 0x10c), position, &target, &radius, band, 1, &hit);  // 004E39B2..DF
        if (result != 1) {                                                        // 004E39E4..E7
            if (result == 0) copy_vector(&target, position);                      // 004E39ED..F7
            else copy_vector(&target, &hit.point_4);                              // 004E39F9..A01
        }
    }
    U* path = static_cast<U*>(live_operator_new(0x10));                           // 004E3A11..18 (EBX, unchecked)
    path[0] = at<U>(creature, 0x90);                                              // 004E3A1A..1C
    path[1] = at<U>(creature, 0x94);                                              // 004E3A1E..24
    path[2] = bits(target.x);                                                     // 004E3A27..2B
    path[3] = bits(target.y);                                                     // 004E3A2E..32
    void* pathfind = at<void*>(creature, 0x340);                                  // 004E3A35
    if (at<U>(pathfind, 0x8c) != 0) {                                             // 004E3A3B..48
        void* old = at<void*>(pathfind, 0x90);                                    // 004E3A4A
        at<void*>(pathfind, 0x90) = nullptr;                                      // 004E3A51
        live_free(old);                                                           // 004E3A50..57
    }
    at<U>(at<void*>(creature, 0x340), 0x8c) = 4;                                  // 004E3A5F..6A
    at<U*>(at<void*>(creature, 0x340), 0x90) = path;                              // 004E3A70..76
    at<U>(at<void*>(creature, 0x340), 0x9c) = 0;                                  // 004E3A7C..82
    at<U>(creature, 0xa8c) = 4;                                                   // 004E3A88
    (void)kotor_fastcall_0051d9c0(creature, nullptr, 1);                          // 004E3A8E..92
    void* copy = new_effect_copy(effect);                                         // 004E3A97..BE (EDI)
    const U16 subtype = at<U16>(copy, 0xa);                                       // 004E3ABE..C0
    at<U16>(copy, 8) = 8;                                                         // 004E3AC4
    at<U16>(copy, 0xa) = static_cast<U16>((subtype & 0xfff8) | k_duration_temporary_007473c0); // 004E3AD9..E6
    si(copy, 0, 9);                                                               // 004E3ACA..EC
    recovered_effect_apply_004d14f0(creature, nullptr, copy, 0, 0);               // 004E3AF1..F8
    return 0;                                                                     // 004E3B06
}

// =====================================================================================================================
// 004E3BE0 CSWSEffectListHandler::OnApplyInvisibility(object, effect, load_game), RET 0Ch
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e3be0(void*, void*, void* object, void* effect, unsigned) {
    if (as_creature(object) == nullptr) return 0;                                 // 004E3BE1..EE
    void* creature = as_creature(object);                                         // 004E3BF0..F7
    if (effect == nullptr) return 0;                                              // 004E3BF9..FF
    void* module = server_get_module_004ae6b0(server_app(), nullptr);              // 004E3C01..09
    w1x_unique_append_006449d0(static_cast<unsigned char*>(module) + 0x13c, nullptr, at<U>(creature, 4)); // 004E3C0E..18
    return 0;
}

// =====================================================================================================================
// 004E3C30 CSWSEffectListHandler::OnRemoveInvisibility(object, effect), RET 8
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e3c30(void*, void*, void* object, void* effect) {
    U other = 0;                                                                  // 004E3C3A [esp+4]
    if (as_creature(object) == nullptr) return 1;                                 // 004E3C32..47
    void* creature = as_creature(object);                                         // 004E3C4D..56 (ESI)
    const I n = count_of(creature);                                               // 004E3C65 (EBX, cached)
    I i = at<U16>(stats_of(creature), 0x140);                                     // 004E3C58..5E
    if (i < n) {                                                                  // 004E3C6B..6D
        for (; i < n; ++i) {                                                      // 004E3C80..B5
            void* e = effect_at(creature, i);
            const U16 t = type_of(e);
            if (t > 0x3f) break;                                                  // 004E3C86..8A
            if (t != 0x2f && t != 0x3f) continue;                                 // 004E3C8C..96
            if (!same_id(effect, e)) other = 1;                                   // 004E3C98..A7
        }
        if (other != 0) return 1;                                                 // 004E3CB7..BE
    }
    void* module = server_get_module_004ae6b0(server_app(), nullptr);              // 004E3CC0..C9
    const U id = at<U>(creature, 4);                                              // 004E3CCE
    unsigned char* list = static_cast<unsigned char*>(module) + 0x13c;            // 004E3CD1
    I found = 0;                                                                  // EDI
    for (I k = at<I>(list, 4), j = 0; k > 0; --k, ++j)                            // 004E3CD7..EB (inline Contains)
        if (at<U*>(list, 0)[j] == id) ++found;
    if (found != 0) (void)recovered_int_list_remove_0066dcc0(list, nullptr, id);   // 004E3CED..F2
    return 1;
}

// =====================================================================================================================
// 004E3D10 CSWSEffectListHandler::OnApplySanctuary(object, effect, load_game), RET 0Ch
//   original behaviour kept: an empty local CExoArrayList is destroyed at the end (free(NULL))
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e3d10(void*, void*, void* object, void* effect, unsigned) {
    if (as_creature(object) == nullptr) return 0;                                 // 004E3D29..36
    DwordList unused{nullptr, 0, 0};                                              // 004E3D3F..49 [esp+10..18]
    void* creature = as_creature(object);                                         // 004E3D4D..58 (EBP)
    void* area = recovered_object_area_004cb120(creature, nullptr);               // 004E3D5A..61 (EBX)
    if (area != nullptr) {                                                        // 004E3D63..65
        const U8 dc = static_cast<U8>(gi(effect, 4));                             // 004E3D6B..7C [arg1 slot]
        for (I i = 0; i < at<I>(area, 0x194); ++i) {                              // 004E3D76..88, 004E3E09..1A
            void* other = kotor_fastcall_004ae770(server_app(), nullptr, at<U*>(area, 0x190)[i]); // 004E3D90..A8
            if (other == nullptr) continue;                                       // 004E3DAA..AC
            if (at<U>(other, 4) == at<U>(creature, 4)) continue;                  // 004E3DAE..B6
            unsigned char* seen = static_cast<unsigned char*>(
                recovered_creature_visible_element_00517a20(other, nullptr, at<U>(creature, 4))); // 004E3DB8..C4
            if (seen == nullptr) continue;
            // 004E3DC6: movzx cx, byte -> the pushed dword's upper half is stale ECX; SavingThrowRoll reads DC & FFFFh only
            if (static_cast<U8>(kotor_fastcall_005b92b0(other, nullptr, 3, static_cast<U>(dc), 0, at<U>(creature, 4), 1, 0, 0)) == 0) { // 004E3DCC..E4
                recovered_creature_pacify_004fda20(creature, nullptr, at<U>(other, 4)); // 004E3DE6..EC
                seen[4] = static_cast<U8>((seen[4] & 0xf7) | 4);                  // 004E3DF1..FA
            } else {
                seen[4] = static_cast<U8>((seen[4] & 0xfb) | 8);                  // 004E3DFF..06
            }
        }
        void* module = server_get_module_004ae6b0(server_app(), nullptr);          // 004E3E20..29
        w1x_unique_append_006449d0(static_cast<unsigned char*>(module) + 0x13c, nullptr, at<U>(creature, 4)); // 004E3E2E..38
    }
    live_free(unused.data);                                                       // 004E3E3D..48 (NULL)
    return 0;
}

// =====================================================================================================================
// 004E3E70 CSWSEffectListHandler::OnRemoveSanctuary(object, effect), RET 8
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e3e70(void*, void*, void* object, void* effect) {
    if (as_creature(object) == nullptr) return 1;                                 // 004E3E8B..99
    U other_effect = 0;                                                           // 004E3EA5 [esp+10]
    U other_sanctuary = 0;                                                        // 004E3EA9 [esp+18]
    void* creature = as_creature(object);                                         // 004E3E9F..B4 (EBP) [esp+14]
    void* area = recovered_object_area_004cb120(creature, nullptr);               // 004E3EB2..BD (EDI)
    if (area == nullptr) return 1;                                                // 004E3EBF..C1
    DwordList pacified{nullptr, 0, 0};                                            // 004E3EC7..CF [esp+20..28]
    for (I i = 0; i < at<I>(area, 0x194); ++i) {                                  // 004E3ED3..DF, 004E3F37..40
        void* other = kotor_fastcall_004ae770(server_app(), nullptr, at<U*>(area, 0x190)[i]); // 004E3EE1..F8
        if (other == nullptr) continue;
        unsigned char* seen = static_cast<unsigned char*>(
            recovered_creature_visible_element_00517a20(other, nullptr, at<U>(creature, 4))); // 004E3EFE..09
        if (seen == nullptr) continue;
        const U8 b = seen[4];                                                     // 004E3F0D
        const U8 state = static_cast<U8>((b >> 2) & 3);                           // 004E3F10..15
        if (state == 1) {                                                         // 004E3F18..1B
            try { gui_load_array_add_00671c00(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(&pacified), nullptr, at<U>(other, 4)); } // 004E3F1D..25
            catch (...) { live_free(pacified.data); throw; }
        } else if (state == 2) {                                                  // 004E3F2C..2F
            seen[4] = static_cast<U8>(b & 0xf3);                                  // 004E3F31..34
        }
    }
    const I n = count_of(creature);                                               // 004E3F4F..57 [esp+1C] (cached)
    for (I i = at<U16>(stats_of(creature), 0x140); i < n; ++i) {                  // 004E3F42..5B, 004E3FB2..C4
        void* e = effect_at(creature, i);                                         // 004E3F70..74
        const U16 t = type_of(e);                                                 // 004E3F76
        if (t > 0x3f) break;                                                      // 004E3F7A..7E
        if (t != 0x2f && t != 0x3f) continue;                                     // 004E3F80..8A
        if (same_id(e, effect)) continue;                                         // 004E3F8C..9D
        other_effect = 1;                                                         // 004E3FA3..A8
        if (t == 0x3f) other_sanctuary = 1;                                       // 004E3F9F..AE
    }
    if (other_effect == 0) {                                                      // 004E3FC6..D0
        void* module = server_get_module_004ae6b0(server_app(), nullptr);          // 004E3FD6..DF (EAX)
        const U id = at<U>(creature, 4);                                          // 004E3FEA
        I found = 0;                                                              // ESI
        for (I k = at<I>(module, 0x140), j = 0; k > 0; --k, ++j)                  // 004E3FE4..09 (inline Contains)
            if (at<U*>(module, 0x13c)[j] == id) ++found;
        if (found != 0) {                                                         // 004E400B..0D: inline CExoArrayList<ulong>::Remove
            const I old_count = at<I>(module, 0x140);                             // 004E400F (ESI)
            I j = 0;                                                              // 004E4015
            if (old_count > 0) {                                                  // 004E4017..19
                while (at<U*>(module, 0x13c)[j] != id) {                          // 004E4021..23
                    ++j;                                                          // 004E402B
                    if (j >= at<I>(module, 0x140)) break;                         // 004E4025..31
                }
            }
            at<I>(module, 0x140) = old_count - 1;                                 // 004E4033..38
            for (; j < at<I>(module, 0x140); ++j)                                 // 004E4036..3E, 004E404F..58
                at<U*>(module, 0x13c)[j] = at<U*>(module, 0x13c)[j + 1];          // 004E4040..4D
        }
    }
    if (other_sanctuary == 0) {                                                   // 004E405A..60
        const I m = pacified.count;                                               // 004E4062 (EDI, cached)
        for (I i = 0; i < m; ++i) {                                               // 004E4066..97
            void* other = kotor_fastcall_004ae770(server_app(), nullptr, pacified.data[i]); // 004E4070..85
            if (other != nullptr) recovered_creature_remove_visible_00517960(other, nullptr, at<U>(creature, 4)); // 004E4087..8F
        }
    }
    live_free(pacified.data);                                                     // 004E4099..A6
    return 1;
}

// =====================================================================================================================
// 004E40D0 CSWSEffectListHandler::OnApplyDispelAllMagic(object, effect, load_game), RET 0Ch
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e40d0(void*, void*, void* object, void* effect, unsigned) {
    void* array = recovered_object_array_004aed70(server_app(), nullptr);          // 004E40E8..F2 [esp+1C]
    void* caster = nullptr;                                                       // [esp+C]
    if (static_cast<U8>(kotor_fastcall_004d8230(array, nullptr, at<U>(effect, 0x18), &caster)) != 0) return 1; // 004E40F7..19
    if (caster == nullptr) return 1;                                              // 004E411F..27
    if (as_creature(caster) == nullptr) return 1;                                 // 004E412D..34
    const U caster_level = recovered_stats_level_005a5fd0(stats_of(as_creature(caster)), nullptr, 0) & 0xff; // 004E413A..55 [esp+20]
    LargeList seen{nullptr, 0, 0};                                                // 004E4159..61 [esp+30..38]
    void* message = nullptr;
    try {
        message = new_message_data();                                             // 004E4165..93 (EBX) [arg1 slot]
        recovered_event_object_004d7780(message, nullptr, 0, at<U>(object, 4));   // 004E4195..A2
        I dispelled = 0;                                                          // 004E41B1 [esp+10]
        for (I i = 0; i < count_of(object); ++i) {                                // 004E41A7..B9, 004E42E0..ED [esp+1C]
            void* e = effect_at(object, i);                                       // 004E41C0..C6 (ESI)
            const U16 subtype = at<U16>(e, 0xa);                                  // 004E41C9..CB
            if ((static_cast<U8>(subtype) & 0x18) != 8) continue;                 // 004E41CF..D7 (magical)
            const U16 duration = static_cast<U16>(subtype & 7);                   // 004E41DD
            if (duration != k_duration_temporary_007473c0 && duration != k_duration_permanent_007473c4) continue; // 004E41E0..F0
            const U lo = at<U>(e, 0), hi = at<U>(e, 4);                           // 004E41F6, 41FC [esp+30]
            const U spell = at<U>(e, 0x1c);                                       // 004E41F9 [esp+28]
            if (kotor_fastcall_004df510(&seen, nullptr, lo, hi) != 0) continue;   // 004E41FE..13
            void* owner = nullptr;                                                // [esp+18]
            U owner_level;                                                        // EDI
            if (static_cast<U8>(kotor_fastcall_004d8230(array, nullptr, at<U>(e, 0x18), &owner)) == 0 && // 004E4219..31
                as_creature(owner) != nullptr)                                    // 004E4233..3E
                owner_level = recovered_stats_level_005a5fd0(stats_of(as_creature(owner)), nullptr, 0) & 0xff; // 004E4240..58
            else
                owner_level = 0;                                                  // 004E425D
            const U roll = recovered_rules_roll_dice_00550b60(rules(), nullptr, 1, 0x14) & 0xffff; // 004E425F..72
            if (static_cast<I>(roll + caster_level) > static_cast<I>(owner_level + 0xb)) { // 004E4275..7C (jle)
                recovered_event_enqueue_004b08d0(recovered_server_ai_master_004aed80(server_app(), nullptr), nullptr,
                    0, 0, at<U>(effect, 0x18), at<U>(object, 4), k_event_remove_effect_007441ec, e); // 004E427E..A6
                if (at<I>(e, 0x1c) != -1) {                                       // 004E42AB..AF
                    ++dispelled;                                                  // 004E42BD
                    recovered_event_integer_004d76b0(message, nullptr, static_cast<U>(dispelled), spell); // 004E42B1..C4
                }
            }
            kotor_fastcall_004e3b70(&seen, nullptr, lo, hi);                      // 004E42C9..D3
        }
        recovered_event_integer_004d76b0(message, nullptr, 0, static_cast<U>(dispelled)); // 004E42F9..FD
        if (dispelled > 0) {                                                      // 004E4302..05
            void* client = kotor_fastcall_004ae910(server_app(), nullptr, at<U>(object, 4)); // 004E4307..14
            if (client != nullptr)                                                // 004E4319..1B
                (void)recovered_cc_message_00568570(recovered_server_sws_message_004aedb0(server_app(), nullptr), nullptr,
                    at<U>(client, 4), 0xf, message, 0);                           // 004E431D..35
            const U caster_id = at<U>(caster, 4);                                 // 004E433A..3E
            if (caster_id != at<U>(object, 4)) {                                  // 004E4341..44
                void* caster_client = kotor_fastcall_004ae910(server_app(), nullptr, caster_id); // 004E4346..50
                if (caster_client != nullptr)                                     // 004E4355..57
                    (void)recovered_cc_message_00568570(recovered_server_sws_message_004aedb0(server_app(), nullptr), nullptr,
                        at<U>(caster_client, 4), 0xf, message, 0);                // 004E4359..71
            }
        }
    } catch (...) { live_free(seen.data); throw; }
    if (message != nullptr) {                                                     // 004E4376..78
        recovered_message_data_destroy_004d6a40(message, nullptr);                // 004E437A..7C
        live_free(message);                                                       // 004E4381..87
    }
    live_free(seen.data);                                                         // 004E438A..97
    return 1;
}

// =====================================================================================================================
// 004E43C0 CSWSEffectListHandler::OnApplyDispelBestMagic(object, effect, load_game), RET 0Ch
//   note: the original reuses the "done" local [esp+1C] as the operator-new temporary of the feedback message; it is
//   overwritten with 1 before it is read, so it has no observable effect and is not modelled.
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e43c0(void*, void*, void* object, void* effect, unsigned) {
    void* array = recovered_object_array_004aed70(server_app(), nullptr);          // 004E43CE..E2
    void* caster = nullptr;                                                       // [esp+C]
    if (static_cast<U8>(kotor_fastcall_004d8230(array, nullptr, at<U>(effect, 0x18), &caster)) != 0) return 1; // 004E43E7..405
    if (caster == nullptr) return 1;                                              // 004E440B..13
    if (as_creature(caster) == nullptr) return 1;                                 // 004E4419..20
    const U caster_level = recovered_stats_level_005a5fd0(stats_of(as_creature(caster)), nullptr, 0) & 0xff; // 004E4426..40 [esp+18]
    LargeList skipped{nullptr, 0, 0};                                             // 004E4444..4C [esp+1C..24]
    U done = 0;                                                                   // 004E4455 [esp+20 after pushes]
    I best = 0;                                                                   // [esp+18 after pushes] (written before use)
    try {
        for (;;) {                                                                // 004E4460
            U found = 0;                                                          // 004E4466 [esp+10]
            if (count_of(object) <= 0) break;                                     // 004E4460..6A
            for (I i = 0; i < count_of(object); ++i) {                            // 004E4470..53C (EBP)
                void* e = effect_at(object, i);                                   // 004E447C..82
                const U16 subtype = at<U16>(e, 0xa);                              // 004E4485..87
                if ((static_cast<U8>(subtype) & 0x18) != 8) continue;             // 004E448B..93
                const U16 duration = static_cast<U16>(subtype & 7);               // 004E4499
                if (duration != k_duration_temporary_007473c0 && duration != k_duration_permanent_007473c4) continue; // 004E449C..AC
                const U lo = at<U>(e, 0), hi = at<U>(e, 4);                       // 004E44B6..B8
                if (skipped.count > 0) {                                          // 004E44BF..C1
                    I matches = 0;                                                // EDI
                    for (I j = 0; j < skipped.count; ++j)                         // 004E44C3..E2
                        if (skipped.data[2 * j] == lo && skipped.data[2 * j + 1] == hi) ++matches;
                    if (matches != 0) continue;                                   // 004E44E4..E6
                }
                void* owner = kotor_fastcall_004ae770(server_app(), nullptr, at<U>(e, 0x18)); // 004E44E8..FA
                if (owner != nullptr) {                                           // 004E44FC
                    if (static_cast<U8>(recovered_stats_level_005a5fd0(stats_of(owner), nullptr, 0)) != 0) { // 004E44FE..13 (jbe)
                        best = i; found = 1;                                      // 004E4515..19
                        break;
                    }
                } else {
                    best = i; found = 1;                                          // 004E4523..27
                    break;
                }
            }
            if (found == 0) break;                                                // 004E4542..48
            void* e = effect_at(object, best);                                    // 004E454E..5E (EDI)
            const U roll = recovered_rules_roll_dice_00550b60(rules(), nullptr, 1, 0x14) & 0xffff; // 004E4558..6E
            if (static_cast<I>(roll + caster_level) > 0xb) {                      // 004E4571..76 (jle)
                if (at<I>(e, 0x1c) != -1) {                                       // 004E457C..80
                    void* message = new_message_data();                           // 004E4586..AA (ESI)
                    recovered_event_object_004d7780(message, nullptr, 0, at<U>(object, 4)); // 004E45AA..B7
                    recovered_event_integer_004d76b0(message, nullptr, 0, 1);     // 004E45BC..C2
                    recovered_event_integer_004d76b0(message, nullptr, 1, at<U>(e, 0x1c)); // 004E45C7..CF
                    void* client = kotor_fastcall_004ae910(server_app(), nullptr, at<U>(object, 4)); // 004E45D4..E1
                    if (client != nullptr)                                        // 004E45E6..E8
                        (void)recovered_cc_message_00568570(recovered_server_sws_message_004aedb0(server_app(), nullptr),
                            nullptr, at<U>(client, 4), 0xf, message, 0);          // 004E45EA..603
                    if (message != nullptr) {                                     // 004E4608..0A
                        recovered_message_data_destroy_004d6a40(message, nullptr); // 004E460C..0E
                        live_free(message);                                       // 004E4613..19
                    }
                }
                recovered_event_enqueue_004b08d0(recovered_server_ai_master_004aed80(server_app(), nullptr), nullptr,
                    0, 0, at<U>(effect, 0x18), at<U>(object, 4), k_event_remove_effect_007441ec, e); // 004E461C..44
                done = 1;                                                         // 004E4649
            } else {                                                              // 004E4653: inline CExoArrayList<LARGE_INTEGER>::Add
                const U lo = at<U>(e, 0), hi = at<U>(e, 4);                       // 004E465D..5F
                if (skipped.count == skipped.alloc)                               // 004E4653..62
                    directory_list::AllocateTimes004c4f50(reinterpret_cast<directory_list::TimeList*>(&skipped), nullptr,
                        skipped.alloc != 0 ? skipped.alloc * 2 : 0x10);           // 004E4664..75
                skipped.data[2 * skipped.count] = lo;                             // 004E467A..82
                skipped.data[2 * skipped.count + 1] = hi;                         // 004E4685
                ++skipped.count;                                                  // 004E4689..8A
            }
            if (done != 0) break;                                                 // 004E468E..9C
        }
    } catch (...) { live_free(skipped.data); throw; }
    live_free(skipped.data);                                                      // 004E46A2..AF
    return 1;
}

// =====================================================================================================================
// 004E46E0 CSWSEffectListHandler::OnApplyBonusFeat(object, effect, load_game), RET 0Ch
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e46e0(void*, void*, void* object, void* effect, unsigned) {
    if (as_creature(object) == nullptr) return 1;                                 // 004E46E1..F6
    const U feat = gi(effect, 0);                                                 // 004E46F9..09 (EDI)
    unsigned char* feats = static_cast<unsigned char*>(stats_of(as_creature(object))) + 0x18; // 004E4705..1A CExoArrayList<ushort>
    if (at<U>(feats, 4) == at<U>(feats, 8)) {                                     // 004E4714..1F
        const U alloc = at<U>(feats, 8);
        kotor_m15_array_u16_allocate_004d2fa0(feats, nullptr, alloc != 0 ? alloc + alloc : 0x10); // 004E4721..31
    }
    at<U16*>(feats, 0)[at<U>(feats, 4)] = static_cast<U16>(feat);                 // 004E4736..3B
    ++at<U>(feats, 4);                                                            // 004E473F
    return 0;
}

// =====================================================================================================================
// 004E4750 CSWSEffectListHandler::OnRemoveBonusFeat(object, effect), RET 8
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e4750(void*, void*, void* object, void* effect) {
    if (as_creature(object) == nullptr) return 1;                                 // 004E4751..5E
    const U feat = gi(effect, 0);                                                 // 004E4760..70
    recovered_u16_list_remove_004e3b20(static_cast<unsigned char*>(stats_of(as_creature(object))) + 0x18, nullptr, feat); // 004E476C..7F
    return 1;
}

// =====================================================================================================================
// 004E4790 CSWSEffectListHandler::OnApplyEffectIcon(object, effect, load_game), RET 0Ch   (Endar)
//   icon entry (operator new 20h): +0 u16 icon id, +2 CResRef IconResRef, +14 Good != 0, +18 u16 Priority,
//   +1C the return value of the "Good" GetINTEntry (original behaviour kept)
// =====================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004e4790(void*, void*, void* object, void* effect, unsigned) {
    if (object == nullptr) return 1;                                              // 004E47AB..B2
    if (as_creature(object) == nullptr) return 1;                                 // 004E47B8..C1
    const U icon_id = gi(effect, 0);                                              // 004E47C7..DB (EBX) [arg2 slot]
    if (static_cast<U16>(at<U8>(effect, 0xa) & 7) == k_duration_instant_007473b8) return 1; // 004E47D4..E9
    unsigned char* icons = static_cast<unsigned char*>(as_creature(object)) + 0x8f4; // 004E47EF..F6 (EBP)
    // 004E47FC: &creature->+8F4 is never null
    for (I i = 0; i < at<I>(icons, 4); ++i)                                       // 004E4804..21
        if (*static_cast<U16*>(at<void**>(icons, 0)[i]) == static_cast<U16>(icon_id)) return 0; // 004E4810..15
    unsigned char* entry = static_cast<unsigned char*>(live_operator_new(0x20)); // 004E4823..2F (ESI)
    if (entry != nullptr) {
        try { (void)c2da_default_ref_00405ed0(entry + 2, nullptr); }              // 004E483F..42
        catch (...) { live_free(entry); throw; }
    }
    if (entry == nullptr) return 1;                                               // 004E484B..55
    ExoString resref_name;                                                        // [esp+24]
    (void)kotor_movie::string_element_construct_005b3190(&resref_name, nullptr);  // 004E485B..5F
    void* table = nullptr;
    const I row = static_cast<std::int16_t>(icon_id);                             // 004E4894 (movsx)
    try {
        ExoString column;                                                         // [esp+14]
        exo_literal(&column, "IconResRef");                                       // 004E4864..75 (0746B04)
        table = game_rules_table(0x120);                                          // 004E487A..85 effecticon.2da
        I ok;
        try { ok = c2da_current_get_string_00413270(table, nullptr, row, &column, &resref_name); } // 004E488B..A0
        catch (...) { exo_destroy(&column); throw; }
        exo_destroy(&column);                                                     // 004E48AB..B6
        bool failed = ok == 0;                                                    // 004E48A5..AF
        I priority = 0;                                                           // [esp+3C] (arg1 slot)
        if (!failed) {
            exo_literal(&column, "Priority");                                     // 004E48BF..C8 (0746AF8)
            table = game_rules_table(0x120);                                      // 004E48CD..D8
            U got;
            try { got = c2da_current_get_int_00413660(table, nullptr, static_cast<U>(row), &column, &priority); } // 004E48DE..F0
            catch (...) { exo_destroy(&column); throw; }
            exo_destroy(&column);                                                 // 004E48FB..906
            failed = got == 0;                                                    // 004E48F5..FF
        }
        if (failed) {                                                             // 004E48BB..BD, 004E490B..0D
            live_free(entry);                                                     // 004E490F..15
            exo_destroy(&resref_name);                                            // 004E4918..24
            return 1;
        }
        I good = 0;                                                               // [esp+10]
        exo_literal(&column, "Good");                                             // 004E4943..4C (0746AF0)
        table = game_rules_table(0x120);                                          // 004E4951..5C
        U good_status;
        try { good_status = c2da_current_get_int_00413660(table, nullptr, static_cast<U>(row), &column, &good); } // 004E4962..74
        catch (...) { exo_destroy(&column); throw; }
        at<U>(entry, 0x1c) = good_status;                                         // 004E497D
        exo_destroy(&column);                                                     // 004E4979..85
        at<U16>(entry, 0) = static_cast<U16>(icon_id);                            // 004E498A, 499C
        alignas(4) unsigned char resref_result[0x10];                             // [esp+14] (CResRef by-value return slot)
        (void)async_ref_assign_string_004061f0(entry + 2, nullptr, resref_result, &resref_name); // 004E498F..9F
        at<U>(entry, 0x14) = good != 0 ? 1u : 0u;                                 // 004E49A4..AF
        at<U16>(entry, 0x18) = static_cast<U16>(priority);                        // 004E49B2..B7
        I position = 0;                                                           // 004E49BE
        for (; position < at<I>(icons, 4); ++position)                            // 004E49BB..E0
            if (static_cast<I>(at<U16>(at<void**>(icons, 0)[position], 0x18)) > priority) break; // 004E49D0..D8 (jg)
        w1x_pointer_list_insert_00616940(icons, nullptr, word(entry), static_cast<U>(position)); // 004E49E2..E6
    } catch (...) { exo_destroy(&resref_name); throw; }
    exo_destroy(&resref_name);                                                    // 004E49EB..F7
    return 0;                                                                     // 004E49FC
}

// =====================================================================================================================
// Shared callees (absent from the c27 map; other effect groups may need them too)
// =====================================================================================================================
// 005036C0 CGameEffect::GetFloat(index): fld dword [this + index*4 + 38h], RET 4
extern "C" float __fastcall kotor_fastcall_005036c0(void* effect, void*, I index) {
    return reinterpret_cast<float*>(static_cast<unsigned char*>(effect) + 0x38)[index];
}
// 004DF510 CExoArrayList<LARGE_INTEGER>::Count(lo, hi): RET 8
extern "C" I __fastcall kotor_fastcall_004df510(void* list, void*, U lo, U hi) {
    I found = 0;
    const I n = at<I>(list, 4);                                                   // 004DF510
    if (n <= 0) return 0;                                                         // 004DF515..17
    const U* p = at<U*>(list, 0);                                                 // 004DF519
    for (I k = n; k != 0; --k, p += 2)                                            // 004DF526..38
        if (p[0] == lo && p[1] == hi) ++found;
    return found;
}
// 004E3B70 CExoArrayList<LARGE_INTEGER>::AddUnique(lo, hi): RET 8
extern "C" void __fastcall kotor_fastcall_004e3b70(void* list, void*, U lo, U hi) {
    LargeList* l = static_cast<LargeList*>(list);
    const I n = l->count;                                                         // 004E3B73
    if (n > 0) {                                                                  // 004E3B79..7B
        I found = 0;
        const U* p = l->data;
        for (I k = n; k != 0; --k, p += 2)                                        // 004E3B82..98
            if (p[0] == lo && p[1] == hi) ++found;
        if (found != 0) return;                                                   // 004E3B9A..9D
    }
    if (n == l->alloc)                                                            // 004E3B9F..A4
        directory_list::AllocateTimes004c4f50(reinterpret_cast<directory_list::TimeList*>(l), nullptr,
            l->alloc != 0 ? l->alloc + l->alloc : 0x10);                          // 004E3BA6..B6
    l->data[2 * l->count] = lo;                                                   // 004E3BBB..C4
    l->data[2 * l->count + 1] = hi;                                               // 004E3BC7..CB
    ++l->count;                                                                   // 004E3BCF
}
// 004EE010 CSWSCreature::Disguise(appearance, effect, load_game): RET 0Ch, EAX 0 (already disguised) / 1
extern "C" U __fastcall kotor_fastcall_004ee010(void* creature, void*, U appearance, void*, U load_game) {
    ExoString unused;                                                             // [esp+4]
    (void)kotor_movie::string_element_construct_005b3190(&unused, nullptr);       // 004EE02B..2F
    try {
        if (at<U>(creature, 0xa48) != 0) {                                        // 004EE034..44
            exo_destroy(&unused);                                                 // 004EE046..52
            return 0;
        }
        void* stats = stats_of(creature);                                         // 004EE06B
        at<U16>(creature, 0xa4c) = at<U16>(stats, 0x186);                        // 004EE071..78
        at<U16>(stats, 0x186) = static_cast<U16>(appearance);                     // 004EE07F..83
        at<U16>(creature, 0xa60) = static_cast<U16>(appearance);                  // 004EE08A
        kotor_fastcall_004ed6e0(creature, nullptr);                               // 004EE091..93 UpdatePersonalSpace
        kotor_fastcall_004ed850(creature, nullptr, 1);         // 004EE098..9C UpdateAppearanceDependantInfo
        at<U>(creature, 0xa48) = 1;                                               // 004EE0A7
        if (load_game == 0) {                                                     // 004EE0A1..B1
            void* area = recovered_object_area_004cb120(creature, nullptr);       // 004EE0B3..B5
            if (area != nullptr) {                                                // 004EE0BA..BC
                Vector safe{0.0f, 0.0f, 0.0f};                                    // 004EE0DB..EB [esp+10]
                Vector* position = &at<Vector>(creature, 0x90);                   // 004EE0D2 (EDI)
                if (w1x_area_safe_location_004be860(area, nullptr, word(position), 0x41a00000u,
                        at<U>(creature, 0x340), 1, word(&safe)) != 0)             // 004EE0BE..FA (20.0f)
                    recovered_object_position_004cd1a0(creature, nullptr, &safe, 1, 1, 0); // 004EE0FC..09
                else
                    copy_vector(&safe, position);                                 // 004EE110..20 (dead store kept)
            }
        }
    } catch (...) { exo_destroy(&unused); throw; }
    exo_destroy(&unused);                                                         // 004EE125..31
    return 1;
}
