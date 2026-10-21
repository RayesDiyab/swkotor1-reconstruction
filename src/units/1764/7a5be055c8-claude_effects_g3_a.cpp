// Effects lane, group 3 part A (worker "effects-g3", sub-worker A): CSWSEffectListHandler callbacks dispatched by the
// server effect dispatcher (0886 NativeEffectDispatch::invoke -> 0854 ai_resolve), 004DCFF0..004DE0A0, plus the
// callees that have no directly callable provider in the c27 candidate map. Transcribed from the original
// swkotor.exe (read-only); addresses in the comments are the original instructions.
//
// ABI: OnApplyX is thiscall(CSWSObject* object, CGameEffect* effect, int load_game) RET 0Ch ->
//   extern "C" unsigned __fastcall kotor_fastcall_<addr>(void* self, void*, void* object, void* effect, unsigned) @20.
// OnRemoveX is thiscall(CSWSObject* object, CGameEffect* effect) RET 8 (verified for every remove handler here) ->
//   extern "C" unsigned __fastcall kotor_fastcall_<addr>(void* self, void*, void* object, void* effect) @16.
// The handler `this` (CSWSEffectListHandler*) is never read by these bodies.
// Memory: live_operator_new / live_free only; operator new + constructor keep the original EH state as a catch(...)
// that frees the block and rethrows. A failed allocation leaves the original's null object (later dereferenced as in
// the original). x87: 53-bit precision -> double intermediates, float exactly where the original stores a dword.
#include "842d6b1618-w5_t3.hpp"

using namespace w5;

namespace kotor_movie { void* __fastcall string_element_construct_005b3190(void* string, void*); }   // CExoString() [0603]
namespace kotor_reconstruction {
struct StartupString { void* data; U length; int __thiscall BorrowedEquals005e5290(const char* text); };  // CExoString::operator==(char const*) [0077]
}

extern "C" {
// ---------------- existing providers (c27 map; exact decorated names) ----------------
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);                         // CServerExoApp::GetServerAIMaster
void* __fastcall kotor_fastcall_004ae780(void* server, void*, U id);                               // GetAreaByGameObjectID [0530]
void __fastcall recovered_object_set_last_hostile_actor_004cc0c0(void* object, void*, U id, U flag); // SetLastHostileActor [1194w]
void __fastcall w5_object_clear_all_actions_004ccd80(void* object, void*, U clear_combat);      // ClearAllActions [1206-07 w5_drive_control]
void __fastcall w1x_object_set_hp_004cc280(void* object, void*, U hp);                             // SetCurrentHitPoints [0886]
void __fastcall kotor_fastcall_004d28f0(void* round, void*, U paused, U id);                       // SetRoundPaused [claude_w5_combat_actions]
void __fastcall kotor_fastcall_004d2920(void* round, void*, U time, U flag);                       // SetPauseTimer [claude_w5_combat_actions]
void __fastcall recovered_effect_apply_004d14f0(void* object, void*, void* effect, U a, U b);      // CSWSObject::ApplyEffect [0886]
void* __fastcall recovered_message_data_construct_004d69f0(void* data, void*);                    // CSWCCMessageData() [W1 b7]
void __fastcall kotor_fastcall_004d6ee0(void* self, void*, void* out);                             // CSWCCMessageData::CopyTo [claude_combat_attack_round]
void __fastcall recovered_creature_set_combat_state_004f2610(void* creature, void*, U state, U flag); // SetCombatState [W1 b7]
void __fastcall recovered_creature_clear_hostile_004f61d0(void* creature, void*, void* other);     // ClearHostileActionsVersus [W1 b7]
void __fastcall recovered_creature_placement_004fa100(void* creature, void*, U area, U x, U y, U z, U saved, U flag); // CSWSCreature::AddToArea [1060]
U __fastcall recovered_effect_integer_00503690(void* effect, void*, U index);                      // CGameEffect::GetInteger [W1 b7]
void __fastcall recovered_effect_integer_005036a0(void* effect, void*, U index, U value);          // CGameEffect::SetInteger [0886]
void __fastcall kotor_fastcall_005036d0(void* effect, void*, U index, U value_bits);               // CGameEffect::SetFloat [claude_w1_chains_damage]
void __fastcall recovered_effect_set_object_id_005036f0(void* effect, void*, U index, U id);       // CGameEffect::SetObjectID [1194u]
void __fastcall recovered_effect_creator_00503a00(void* effect, void*, U id);                      // CGameEffect::SetCreator [0886]
void* __fastcall recovered_effect_construct_00503e40(void* effect, void*, U flag);                 // CGameEffect(int) [0886]
void* __fastcall kotor_fastcall_00503f70(void* self, void*, void* source);                         // CGameEffect(CGameEffect*) [claude_w1_route_bodies]
U __fastcall kotor_fastcall_0051d9c0(void* creature, void*, U reset);                              // WalkUpdateLocation_QuickWalk [claude_w5_pathfind_walk_quick]
U __fastcall recovered_stats_class_level_005a5090(void* stats, void*, U index, U flag);            // CSWSCreatureStats::GetClassLevel (AL) [w2_g07_native]
U __fastcall recovered_stats_level_005a5fd0(void* stats, void*, U flag);                           // CSWSCreatureStats::GetLevel (AL) [0963]
U __fastcall recovered_stats_effect_query_005a6960(void* stats, void*, U kind, void* other);       // GetEffectImmunity [1094]
I __fastcall c2da_current_get_string_00413270(void* table, void*, I row, void* column, void* out); // C2DA::GetCExoStringEntry [0379]
U __fastcall recovered_test_safe_location_004aeb50(void* server, void*, U id, Vector* out);       // CServerExoApp::TestSafeLocation [w2_party_native]
double __fastcall kotor_fastcall_004f1310(void* creature, void*, U target, U flag);               // CSWSCreature::DesiredAttackRange [claude_combat_attack]
void __fastcall frame_LogStringDtor_005e5c20(void* string, void*);                                 // ~CExoString [0459]

// ---------------- absent from the c27 map as a directly callable symbol (owner: 0886 coordinator) ----------------
// 00587170 CSWSPlaceable::AddToArea(CSWSArea*, Vector* position, int) RET 0Ch. Its only body is the 1165 port
// placeable_placement_00587170, reached through the file-local 0886 glue native_placeable_placement(self, area, pos, flag).
void __fastcall kotor_fastcall_00587170(void* placeable, void*, void* area, Vector* position, U flag);

// ---------------- exports written here ----------------
U __fastcall kotor_fastcall_005036e0(void* effect, void*, U index);                                // CGameEffect::GetObjectID
void __fastcall kotor_fastcall_004cc790(void* object, void*, U group, U value);                    // CSWSObject::SetGroupInterruptable
U __fastcall kotor_fastcall_004ecff0(void* creature, void*);                                       // CSWSCreature::AddAppearActions
U __fastcall kotor_fastcall_004ed060(void* creature, void*, U flag);                               // CSWSCreature::AddDisappearActions
void __fastcall kotor_fastcall_004fd960(void* creature, void*);                                    // CSWSCreature::PacifyCreature
unsigned char __fastcall kotor_fastcall_005a50e0(void* stats, void*, U index);                     // GetClassNegativeLevels
void __fastcall kotor_fastcall_005a5440(void* stats, void*, U index, U value);                     // SetClassNegativeLevels
U __fastcall kotor_fastcall_004dcff0(void* self, void*, void* object, void* effect, U load_game);
U __fastcall kotor_fastcall_004dd100(void* self, void*, void* object, void* effect, U load_game);
U __fastcall kotor_fastcall_004dd140(void* self, void*, void* object, void* effect, U load_game);
U __fastcall kotor_fastcall_004dd270(void* self, void*, void* object, void* effect);
U __fastcall kotor_fastcall_004dd470(void* self, void*, void* object, void* effect, U load_game);
U __fastcall kotor_fastcall_004dd4f0(void* self, void*, void* object, void* effect, U load_game);
U __fastcall kotor_fastcall_004dd520(void* self, void*, void* object, void* effect, U load_game);
U __fastcall kotor_fastcall_004dd970(void* self, void*, void* object, void* effect);
U __fastcall kotor_fastcall_004dd9d0(void* self, void*, void* object, void* effect, U load_game);
U __fastcall kotor_fastcall_004ddac0(void* self, void*, void* object, void* effect);
U __fastcall kotor_fastcall_004ddd50(void* self, void*, void* object, void* effect, U load_game);
U __fastcall kotor_fastcall_004dde40(void* self, void*, void* object, void* effect, U load_game);
U __fastcall kotor_fastcall_004dde80(void* self, void*, void* object, void* effect);
U __fastcall kotor_fastcall_004ddec0(void* self, void*, void* object, void* effect, U load_game);
U __fastcall kotor_fastcall_004ddf20(void* self, void*, void* object, void* effect);
U __fastcall kotor_fastcall_004ddf40(void* self, void*, void* object, void* effect, U load_game);
U __fastcall kotor_fastcall_004de0a0(void* self, void*, void* object, void* effect, U load_game);
}

namespace {
constexpr U8 k_type_creature_00746634 = 5;        // .rdata 00746634 (byte)
constexpr U8 k_type_placeable_00746638 = 9;       // .rdata 00746638 (byte)
constexpr U k_ai_event_appear_007441c4 = 4;       // .rdata 007441C4
constexpr U k_ai_event_jump_007441c8 = 5;         // .rdata 007441C8
constexpr U k_ai_event_destroy_007441e0 = 0xb;    // .rdata 007441E0
constexpr U16 k_subtype_magical_007473b8 = 0;     // .rdata 007473B8 (word)
constexpr U16 k_subtype_supernatural_007473c0 = 1; // .rdata 007473C0 (word)
constexpr U16 k_subtype_extraordinary_007473c4 = 2; // .rdata 007473C4 (word)
constexpr U k_bits_twenty = 0x41a00000u;          // push 41A00000h (20.0f)
constexpr U k_bits_six = 0x40c00000u;             // mov dword [esi+0Ch], 40C00000h (6.0f)
const char k_bloodcolr_007469a8[] = "BLOODCOLR";  // .rdata 007469A8
const char k_r_007469a4[] = "R";                  // .rdata 007469A4
const char k_g_007469a0[] = "G";                  // .rdata 007469A0
const char k_y_0074699c[] = "Y";                  // .rdata 0074699C

inline U addr(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
template<class T = void> inline T* ptr(U v) { return reinterpret_cast<T*>(static_cast<std::uintptr_t>(v)); }
inline void* server() { return server_app(); }
inline void* ai() { return recovered_server_ai_master_004aed80(server(), nullptr); }
inline void* rules() { return kotor_cell_rules_007a3a28; }
inline void* as_creature(void* object) { return vcall<void*>(object, 0x30); }   // vtbl+30 AsSWSCreature
inline void* round_of(void* creature) { return at<void*>(creature, 0x9c8); }
inline void* stats_of(void* creature) { return at<void*>(creature, 0xa74); }
inline U eff_int(void* effect, U index) { return recovered_effect_integer_00503690(effect, nullptr, index); }
inline void eff_set(void* effect, U index, U value) { recovered_effect_integer_005036a0(effect, nullptr, index, value); }
inline float eff_float(void* effect, U index) { return at<float>(effect, 0x38 + index * 4); }   // inline view of 005036C0
inline U racial_all() { return at<U8>(rules(), 0xaa); }                                         // movzx [[7A3A28]+AA]
// word [effect+0A]: (& FFF8h) | subtype
inline void set_subtype(void* effect, U16 subtype) {
    at<U16>(effect, 0xa) = static_cast<U16>((static_cast<U>(at<U16>(effect, 0xa)) & 0xfff8u) | subtype);
}
template<class F> void* construct(U size, F ctor) {
    void* memory = live_operator_new(size);
    if (memory == nullptr) return nullptr;
    try { return ctor(memory); } catch (...) { live_free(memory); throw; }
}
void* new_effect() { return construct(0x8c, [](void* m) { return recovered_effect_construct_00503e40(m, nullptr, 1); }); }
void* copy_effect(void* source) { return construct(0x8c, [source](void* m) { return kotor_fastcall_00503f70(m, nullptr, source); }); }
void* new_message_data() { return construct(0x34, [](void* m) { return recovered_message_data_construct_004d69f0(m, nullptr); }); }
inline void enqueue(U ms, U caller, U target, U type, void* event) {
    recovered_event_enqueue_004b08d0(ai(), nullptr, 0, ms, caller, target, type, event);
}
// AddAction(type, 0FFFFh, 13 empty parameter pairs) (004ED001..3E / 004ED073..B0)
void add_empty_action(void* creature, U type) {
    recovered_object_add_action_004cea20(creature, nullptr, type, 0xffff, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr,
        0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr);
}
}  // namespace

extern "C" {

// =========================================================================================================
// Missing callees
// =========================================================================================================

// 005036E0 CGameEffect::GetObjectID(int index): mov eax,[ecx+index*4+78h]; RET 4.
U __fastcall kotor_fastcall_005036e0(void* effect, void*, U index) {
    return at<U>(effect, 0x78 + index * 4);                                                  // 005036E0..E8
}

// 004CC790 CSWSObject::SetGroupInterruptable(unsigned short group, int value): RET 8. Every queued action node of the
// object's action list (+FC) whose word +6C equals `group` gets +70 = value.
void __fastcall kotor_fastcall_004cc790(void* object, void*, U group, U value) {
    void* list = at<void*>(object, 0xfc);                                                    // 004CC794
    void* position = at<void*>(list, 0);                                                     // 004CC79A..9C [esp+4]
    void* node = at<void*>(list, 0);                                                         // 004CC7A0
    if (node != nullptr) node = at<void*>(node, 8);                                          // 004CC7A2..A6
    if (node == nullptr) return;                                                             // 004CC7A9..AB
    const U16 g = static_cast<U16>(group);                                                   // 004CC7AE bx
    do {
        if (at<U16>(node, 0x6c) == g) at<U>(node, 0x70) = value;                             // 004CC7B8..BE
        node = ptr(unresolved_list_next_005e9b90(at<void*>(object, 0xfc), nullptr, &position)); // 004CC7C1..CC
    } while (node != nullptr);                                                               // 004CC7D1..D3
}

// 004ECFF0 CSWSCreature::AddAppearActions(): RET 0. Returns 0 when +E8 is clear, else queues action 34h and returns 1.
U __fastcall kotor_fastcall_004ecff0(void* creature, void*) {
    if (at<U>(creature, 0xe8) == 0) return 0;                                                // 004ECFF3..FF
    add_empty_action(creature, 0x34);                                                        // 004ED001..3E
    kotor_fastcall_004cc790(creature, nullptr, at<U16>(creature, 0x16), 0);                  // 004ED043..4E
    return 1;
}

// 004ED060 CSWSCreature::AddDisappearActions(int flag): RET 4. Queues action 35h and an AI event (2000 ms) to itself:
// type 0Bh when flag == 0, else type 4. Quirk kept: the event-data argument is the flag value itself (push eax).
U __fastcall kotor_fastcall_004ed060(void* creature, void*, U flag) {
    if (at<U>(creature, 0xe8) == 0) return 0;                                                // 004ED063..70
    add_empty_action(creature, 0x35);                                                        // 004ED073..B0
    kotor_fastcall_004cc790(creature, nullptr, at<U16>(creature, 0x16), 0);                  // 004ED0B5..C0
    const U id = at<U>(creature, 4);                                                         // 004ED0D1
    const U type = flag == 0 ? k_ai_event_destroy_007441e0 : k_ai_event_appear_007441c4;     // 004ED0C5..E2
    enqueue(0x7d0, id, id, type, ptr(flag));                                                 // 004ED0DF..FA
    return 1;
}

// 004FD960 CSWSCreature::PacifyCreature(): RET 0. Every creature of the area that has this creature as attack target /
// attempted target / last hostile actor (+50C/+504/+524/+528/+158) leaves combat and drops its hostility.
void __fastcall kotor_fastcall_004fd960(void* creature, void*) {
    void* area = recovered_object_area_004cb120(creature, nullptr);                          // 004FD964
    if (area == nullptr) return;                                                             // 004FD96B..6D
    for (I i = 0; i < at<I>(area, 0x194); ++i) {                                             // 004FD973..7E / 004FDA04..0D
        void* other = kotor_fastcall_004ae770(server(), nullptr, at<U*>(area, 0x190)[i]);    // 004FD990..A2
        if (other == nullptr) continue;                                                      // 004FD9A9..AB
        const U id = at<U>(creature, 4);                                                     // 004FD9AD
        if (at<U>(other, 0x50c) != id && at<U>(other, 0x504) != id && at<U>(other, 0x524) != id &&
            at<U>(other, 0x528) != id && at<U>(other, 0x158) != id) continue;                // 004FD9B0..D6
        recovered_creature_set_combat_state_004f2610(other, nullptr, 0, 1);                  // 004FD9D8..DE
        if (at<U>(other, 0x158) == at<U>(creature, 4))                                       // 004FD9E3..EC
            recovered_object_set_last_hostile_actor_004cc0c0(other, nullptr, k_object_invalid, 1); // 004FD9EE..F7
        recovered_creature_clear_hostile_004f61d0(other, nullptr, creature);                 // 004FD9FC..FF
    }
}

// 005A50E0 CSWSCreatureStats::GetClassNegativeLevels(unsigned char class_index): RET 4, AL.
unsigned char __fastcall kotor_fastcall_005a50e0(void* stats, void*, U index) {
    const U8 i = static_cast<U8>(index);                                                     // 005A50E0
    if (i >= at<U8>(stats, 0x89)) return 0;                                                  // 005A50E4..EE (jb)
    return at<U8>(stats, static_cast<U>(i) * 40 + 0xa9);                                     // 005A50F1..FE
}

// 005A5440 CSWSCreatureStats::SetClassNegativeLevels(unsigned char class_index, unsigned char value): RET 8.
void __fastcall kotor_fastcall_005a5440(void* stats, void*, U index, U value) {
    const U8 i = static_cast<U8>(index);                                                     // 005A5440
    if (i >= at<U8>(stats, 0x89)) return;                                                    // 005A5444..4A (jae)
    at<U8>(stats, static_cast<U>(i) * 40 + 0xa9) = static_cast<U8>(value);                   // 005A544C..56
}

// =========================================================================================================
// Handlers
// =========================================================================================================

// 004DCFF0 OnApplyDarkness: RET 0Ch, returns 0. Creatures get two copies of the effect: type 2Fh (int0 = 2,
// int1 = racial "all") and type 49h (int0 = 8).
U __fastcall kotor_fastcall_004dcff0(void*, void*, void* object, void* effect, U load_game) {
    if (as_creature(object) == nullptr) return 0;                                            // 004DD00B..15
    void* creature = as_creature(object);                                                    // 004DD01B..1F (EDI)
    void* first = copy_effect(effect);                                                       // 004DD022..4F
    at<U16>(first, 8) = 0x2f;                                                                // 004DD061
    eff_set(first, 0, 2);                                                                    // 004DD053..67
    eff_set(first, 1, racial_all());                                                         // 004DD06C..7E
    recovered_effect_apply_004d14f0(creature, nullptr, first, load_game, 0);                 // 004DD083..8D
    void* second = copy_effect(effect);                                                      // 004DD092..BD
    at<U16>(second, 8) = 0x49;                                                               // 004DD0CF
    eff_set(second, 0, 8);                                                                   // 004DD0C1..D5
    recovered_effect_apply_004d14f0(creature, nullptr, second, load_game, 0);                // 004DD0DA..E0
    return 0;
}

// 004DD100 OnApplyTurnResistance: RET 0Ch. 0 only for a creature whose stats +6C is 0; 1 otherwise (null object too).
U __fastcall kotor_fastcall_004dd100(void*, void*, void* object, void*, U) {
    if (object == nullptr) return 1;                                                         // 004DD105..07
    if (as_creature(object) == nullptr) return 1;                                            // 004DD109..12
    if (at<U>(stats_of(as_creature(object)), 0x6c) != 0) return 1;                           // 004DD114..26
    return 0;                                                                                // 004DD128
}

// 004DD140 OnApplyDisappearAppear: RET 0Ch. int-id 0 = destination area; floats 0..2 = destination (read, unused).
U __fastcall kotor_fastcall_004dd140(void*, void*, void* object, void* effect, U) {
    (void)eff_float(effect, 2);                                                              // 004DD146..4F (fstp st0)
    (void)eff_float(effect, 1);                                                              // 004DD151..5A
    (void)eff_float(effect, 0);                                                              // 004DD15C..65
    const U area_id = kotor_fastcall_005036e0(effect, nullptr, 0);                           // 004DD167..70 (EBX)
    if (kotor_fastcall_004ae780(server(), nullptr, area_id) == nullptr) return 1;            // 004DD172..8B
    const U8 type = at<U8>(object, 8);                                                       // 004DD199
    if (type == k_type_creature_00746634) {                                                  // 004DD18E..9E
        void* creature = as_creature(object);                                                // 004DD1A0..A7 (ESI)
        w5_object_clear_all_actions_004ccd80(creature, nullptr, 1);                          // 004DD1A9..AD
        kotor_fastcall_004fd960(creature, nullptr);                                          // 004DD1B2..B4
        at<U>(creature, 0x310) = area_id;                                                    // 004DD1B9
        copy_vector(&at<U>(creature, 0x314), &at<U>(as_creature(object), 0x90));             // 004DD1BF..E0
        at<U>(creature, 0x320) = 0;                                                          // 004DD1EB
        kotor_fastcall_004d2920(round_of(creature), nullptr, 0, 1);                          // 004DD1DE..F5
        kotor_fastcall_004d28f0(round_of(creature), nullptr, 1, at<U>(object, 4));           // 004DD1FA..206
        kotor_fastcall_004ed060(creature, nullptr, 1);                                       // 004DD20B..0F
        return 0;
    }
    if (type == k_type_placeable_00746638) {                                                 // 004DD21C..22
        enqueue(0x7d0, at<U>(effect, 0x18), at<U>(object, 4), k_ai_event_appear_007441c4, nullptr); // 004DD224..4B
        return 0;
    }
    return 1;                                                                                // 004DD258..60
}

// 004DD270 OnRemoveDisappearAppear: RET 8 (two arguments). Returns 1. Frame: P = (x, y, z) floats 0..2 at
// [esp0-18h], Q = 0 vector at [esp0-24h]; the float reads are stored through the argument slots as in the original.
U __fastcall kotor_fastcall_004dd270(void*, void*, void* object, void* effect) {
    const float z = eff_float(effect, 2);                                                    // 004DD27A..83
    const float y = eff_float(effect, 1);                                                    // 004DD287..90
    const float x = eff_float(effect, 0);                                                    // 004DD294..9D
    Vector p = {x, y, z};                                                                    // 004DD2A1..B1
    Vector q = {0.0f, 0.0f, 0.0f};                                                           // 004DD2B5..C5
    void* area = kotor_fastcall_004ae780(server(), nullptr, kotor_fastcall_005036e0(effect, nullptr, 0)); // 004DD2AD..E1 (EBX)
    const U8 type = at<U8>(object, 8);                                                       // 004DD2ED
    if (type == k_type_creature_00746634) {                                                  // 004DD2E5..F2
        void* creature = as_creature(object);                                                // 004DD2F8..FF (EDI)
        w5_object_clear_all_actions_004ccd80(creature, nullptr, 1);                          // 004DD301..05
        kotor_fastcall_004d28f0(round_of(creature), nullptr, 0, k_object_invalid);           // 004DD30A..17
        kotor_fastcall_004d2920(round_of(creature), nullptr, 0x7d0, 0);                      // 004DD31C..29
        kotor_fastcall_004d28f0(round_of(creature), nullptr, 1, k_object_invalid);           // 004DD32E..3B
        kotor_fastcall_004ecff0(creature, nullptr);                                          // 004DD340..42
        if (w1x_area_safe_location_004be860(area, nullptr, addr(&p), k_bits_twenty, at<U>(creature, 0x340), 0,
                                             addr(&q)) != 0) {                               // 004DD347..68
            q.z = flt(w1x_area_height_bits_004bc380(area, nullptr, &q, 0, nullptr));         // 004DD36E..7A
            recovered_creature_placement_004fa100(creature, nullptr, addr(area), bits(q.x), bits(q.y), bits(q.z), 0, 0); // 004DD37E..94
            return 1;
        }
        const float h = flt(w1x_area_height_bits_004bc380(area, nullptr, &p, 0, nullptr));   // 004DD3A7..B4
        kotor_fastcall_005036d0(effect, nullptr, 2, bits(h));                                // 004DD3B3..BB
        const float nz = eff_float(effect, 2);                                               // 004DD3C4..CE
        const float ny = eff_float(effect, 1);                                               // 004DD3D1..DB
        const float nx = eff_float(effect, 0);                                               // 004DD3DE..E8
        recovered_creature_placement_004fa100(creature, nullptr, addr(area), bits(nx), bits(ny), bits(nz), 0, 0); // 004DD3C0..EE
        return 1;
    }
    if (type == k_type_placeable_00746638) {                                                 // 004DD401..07
        const float rz = eff_float(effect, 2);                                               // 004DD409..12
        const float ry = eff_float(effect, 1);                                               // 004DD416..1F
        const float rx = eff_float(effect, 0);                                               // 004DD423..2C
        Vector r = {rx, ry, rz};                                                             // 004DD430..48 [esp0-0Ch]
        void* placeable = vcall<void*>(object, 0x48);                                        // 004DD443..4C AsSWSPlaceable
        kotor_fastcall_00587170(placeable, nullptr, area, &r, 0);                            // 004DD438..51
    }
    return 1;                                                                                // 004DD456..61
}

// 004DD470 OnApplyDisappear: RET 0Ch, returns 1.
U __fastcall kotor_fastcall_004dd470(void*, void*, void* object, void* effect, U) {
    const U8 type = at<U8>(object, 8);                                                       // 004DD474
    if (type == k_type_creature_00746634) {                                                  // 004DD477..7E
        void* creature = as_creature(object);                                                // 004DD480..85
        w5_object_clear_all_actions_004ccd80(creature, nullptr, 1);                          // 004DD487..8B
        kotor_fastcall_004ed060(creature, nullptr, 0);                                       // 004DD490..94
        return 1;
    }
    if (type == k_type_placeable_00746638)                                                   // 004DD4A2..A8
        enqueue(0x7d0, at<U>(effect, 0x18), at<U>(object, 4), k_ai_event_destroy_007441e0, nullptr); // 004DD4AA..D6
    return 1;
}

// 004DD4F0 OnApplyAppear: RET 0Ch, returns 1.
U __fastcall kotor_fastcall_004dd4f0(void*, void*, void* object, void*, U) {
    if (at<U8>(object, 8) == k_type_creature_00746634) {                                     // 004DD4F4..FD
        void* creature = as_creature(object);                                                // 004DD4FF..05
        w5_object_clear_all_actions_004ccd80(creature, nullptr, 1);                          // 004DD507..0B
        kotor_fastcall_004ecff0(creature, nullptr);                                          // 004DD510..12
    }
    return 1;
}

// 004DD520 OnApplyNegativeLevel: RET 0Ch. Quirks kept: the creator creature is written into the object argument slot
// ([esp0+4]) before the immunity test and is the target of the trailing temporary-HP effect; the effect argument
// slot ([esp0+8]) holds the highest class level / negative-level count scratch.
U __fastcall kotor_fastcall_004dd520(void*, void*, void* object, void* effect, U load_game) {
    if (static_cast<I>(eff_int(effect, 0)) > 100) return 1;                                  // 004DD53D..49 (jg)
    if (as_creature(object) == nullptr) return 0;                                            // 004DD54F..5C
    void* creature = as_creature(object);                                                    // 004DD562..66 (ESI)
    void* creator = kotor_fastcall_004ae770(server(), nullptr, at<U>(effect, 0x18));         // 004DD569..78 (EBX, [esp0+4])
    if (recovered_stats_effect_query_005a6960(stats_of(creature), nullptr, 0x1d, creator) != 0) { // 004DD57D..93
        void* data = new_message_data();                                                     // 004DD599..B8 (EBP)
        recovered_event_object_004d7780(data, nullptr, 0, at<U>(object, 4));                 // 004DD5BA..CA
        if (creator != nullptr) {                                                            // 004DD5CF..D1
            void* copy = new_message_data();                                                 // 004DD5D3..F8
            kotor_fastcall_004d6ee0(data, nullptr, copy);                                    // 004DD5FA..605
            recovered_creature_feedback_004ede10(creator, nullptr, 0x83, copy);              // 004DD60A..12
        }
        recovered_creature_feedback_004ede10(creature, nullptr, 0x83, data);                 // 004DD617..1F
        return 1;
    }
    void* e = copy_effect(effect);                                                           // 004DD62E..55
    at<U16>(e, 8) = 0xb;                                                                     // 004DD665
    eff_set(e, 0, eff_int(effect, 0));                                                       // 004DD659..75
    eff_set(e, 1, 0);                                                                        // 004DD67A..80
    eff_set(e, 2, racial_all());                                                             // 004DD685..97
    recovered_effect_apply_004d14f0(creature, nullptr, e, load_game, 0);                     // 004DD69C..A6
    e = copy_effect(effect);                                                                 // 004DD6AB..D2
    at<U16>(e, 8) = 0x1b;                                                                    // 004DD6E2
    eff_set(e, 0, eff_int(effect, 0));                                                       // 004DD6D6..F2
    eff_set(e, 1, 0);                                                                        // 004DD6F7..FD
    eff_set(e, 3, racial_all());                                                             // 004DD702..14
    recovered_effect_apply_004d14f0(creature, nullptr, e, load_game, 0);                     // 004DD719..1F
    e = copy_effect(effect);                                                                 // 004DD724..4B
    at<U16>(e, 8) = 0x38;                                                                    // 004DD760
    eff_set(e, 0, 0xff);                                                                     // 004DD74F..66
    eff_set(e, 1, eff_int(effect, 0));                                                       // 004DD76B..79
    eff_set(e, 2, racial_all());                                                             // 004DD77E..8F
    recovered_effect_apply_004d14f0(creature, nullptr, e, load_game, 0);                     // 004DD794..9A
    // the class with the highest level (first one on ties) takes the negative levels
    U best = 0;                                                                              // EBX
    I best_level = 0;                                                                        // [esp0+8]
    if (at<U8>(stats_of(creature), 0x89) != 0) {                                             // 004DD79F..B5
        for (U i = 0; static_cast<I>(i) < static_cast<I>(at<U8>(stats_of(creature), 0x89)); ++i) { // 004DD7E8..F8
            const I level = static_cast<U8>(recovered_stats_class_level_005a5090(stats_of(creature), nullptr, i, 0)); // 004DD7C0..C8
            if (level > best_level) {                                                        // 004DD7CB..CF (jle)
                best = i;                                                                    // 004DD7DA
                best_level = static_cast<U8>(recovered_stats_class_level_005a5090(stats_of(creature), nullptr, i, 0)); // 004DD7D1..E4
            }
        }
        if (best_level > 0) {                                                                // 004DD7FA..800
            const U8 negative = kotor_fastcall_005a50e0(stats_of(creature), nullptr, best);  // 004DD806..19
            void* stats = stats_of(creature);                                                // 004DD80C (EDI)
            const U8 total = static_cast<U8>(static_cast<U8>(eff_int(effect, 0)) + negative); // 004DD815..22 (add al)
            kotor_fastcall_005a5440(stats, nullptr, best, total);                            // 004DD826..2A
            eff_set(effect, 1, best);                                                        // 004DD82F..34
            if (static_cast<U8>(recovered_stats_level_005a5fd0(stats_of(creature), nullptr, 1)) == 0) { // 004DD839..48 (ja)
                void* death = new_effect();                                                  // 004DD84E..76
                at<U16>(death, 8) = 0x13;                                                    // 004DD880
                set_subtype(death, k_subtype_magical_007473b8);                              // 004DD87A..9F
                eff_set(death, 0, 1);                                                        // 004DD886..A5
                eff_set(death, 1, 1);                                                        // 004DD8AA..B0
                recovered_effect_creator_00503a00(death, nullptr, at<U>(effect, 0x18));      // 004DD8B5..BB
                recovered_effect_apply_004d14f0(creature, nullptr, death, load_game, 0);     // 004DD8C0..CA
            }
        }
    }
    if (creator == nullptr) return 0;                                                        // 004DD8CF..D5
    void* bonus = new_effect();                                                              // 004DD8D7..FF
    set_subtype(bonus, k_subtype_extraordinary_007473c4);                                    // 004DD903..27
    at<U16>(bonus, 8) = 0xf;                                                                 // 004DD921
    eff_set(bonus, 0, eff_int(effect, 0) * 5);                                               // 004DD909..38 (lea [eax+eax*4])
    recovered_effect_apply_004d14f0(creator, nullptr, bonus, load_game, 0);                  // 004DD93D..47
    return 0;
}

// 004DD970 OnRemoveNegativeLevel: RET 8, returns 1. int1 = class index, int0 = level count (byte arithmetic).
U __fastcall kotor_fastcall_004dd970(void*, void*, void* object, void* effect) {
    if (as_creature(object) == nullptr) return 1;                                            // 004DD975..7E
    void* creature = as_creature(object);                                                    // 004DD980..87
    const U index = eff_int(effect, 1);                                                      // 004DD98E..94 (EDI)
    const U8 negative = kotor_fastcall_005a50e0(stats_of(creature), nullptr, index);         // 004DD999..A2
    void* stats = stats_of(creature);                                                        // 004DD9A7
    const U8 left = static_cast<U8>(negative - static_cast<U8>(eff_int(effect, 0)));         // 004DD9AD..B9 (sub bl,al)
    kotor_fastcall_005a5440(stats, nullptr, index, left);                                    // 004DD9BB..BF
    return 1;
}

// 004DD9D0 OnApplyWounding: RET 0Ch. Magical-subtype wounding on a creature schedules a 6 s supernatural type 54h
// effect (created by the wounding's creator) on the object; 0 then, 1 otherwise.
U __fastcall kotor_fastcall_004dd9d0(void*, void*, void* object, void* effect, U load_game) {
    if (as_creature(object) == nullptr) return 1;                                            // 004DD9EC..F5
    if (static_cast<U16>(at<U8>(effect, 0xa) & 7) != k_subtype_magical_007473b8) return 1;   // 004DD9FB..0E
    void* e = new_effect();                                                                  // 004DDA14..3E
    recovered_effect_creator_00503a00(e, nullptr, at<U>(effect, 0x18));                      // 004DDA40..4E
    at<U16>(e, 8) = 0x54;                                                                    // 004DDA5F
    at<U>(e, 0xc) = k_bits_six;                                                              // 004DDA75
    set_subtype(e, k_subtype_supernatural_007473c0);                                         // 004DDA57..7C
    recovered_effect_apply_004d14f0(object, nullptr, e, load_game, 0);                       // 004DDA5D..80
    return 0;
}

// 004DDAC0 OnRemoveWounding: RET 8, returns 1. One hit point of bleeding (vtbl+9C(1) - 1), a blood-colour visual,
// then either a death effect (the creature just died) or the next 6 s wounding tick.
U __fastcall kotor_fastcall_004ddac0(void*, void*, void* object, void* effect) {
    void* creature = as_creature(object);                                                    // 004DDAC6..E3 (ESI)
    if (creature == nullptr) return 1;                                                       // 004DDAE7..E9
    (void)kotor_m15e_slot010_zero_0063e7f0(creature, nullptr);                               // 004DDAF1..F3 (result unused)
    const U was_dead = vcall<U>(creature, 0x94);                                             // 004DDAF8..04 (EBP)
    const U was_dead_temp = recovered_creature_incapacitated_004ef890(creature, nullptr);    // 004DDB02..0B [esp+10]
    const I hp = static_cast<int16_t>(vcall<U>(creature, 0x9c, 1u));                         // 004DDB0F..1B (movsx)
    w1x_object_set_hp_004cc280(creature, nullptr, static_cast<U>(hp - 1));                   // 004DDB1E..22
    struct ExoString { U data, length; } colour;                                             // [esp+18]
    kotor_movie::string_element_construct_005b3190(&colour, nullptr);                        // 004DDB27..2B
    struct Guard { ExoString* s; ~Guard() { frame_LogStringDtor_005e5c20(s, nullptr); } } guard{&colour}; // EH state 0 .. 004DDD30
    {
        ExoString column;                                                                    // [esp+20]
        frame_LogStringCtor_005e5a90(&column, nullptr, k_bloodcolr_007469a8);                // 004DDB30..3D
        struct ColumnGuard { ExoString* s; ~ColumnGuard() { frame_LogStringDtor_005e5c20(s, nullptr); } } column_guard{&column};
        void* appearance = at<void*>(at<void*>(rules(), 0xb8), 0xc);                         // 004DDB42..4E
        c2da_current_get_string_00413270(appearance, nullptr, at<U16>(creature, 0xa60), &column, &colour); // 004DDB51..6A
    }                                                                                        // 004DDB6F..77
    void* visual = new_effect();                                                             // 004DDB7C..A3 (EDI)
    at<U16>(visual, 8) = 0x1e;                                                               // 004DDBAB
    set_subtype(visual, k_subtype_magical_007473b8);                                         // 004DDBA5..CB
    auto* text = reinterpret_cast<kotor_reconstruction::StartupString*>(&colour);
    if (text->BorrowedEquals005e5290(k_r_007469a4) != 0) eff_set(visual, 0, 0x9e);           // 004DDBB1..DD / 004DDC0F..12
    else if (text->BorrowedEquals005e5290(k_g_007469a0) != 0) eff_set(visual, 0, 0x9f);      // 004DDBDF..F6
    else if (text->BorrowedEquals005e5290(k_y_0074699c) != 0) eff_set(visual, 0, 0xa0);      // 004DDBF8..0F
    recovered_effect_apply_004d14f0(creature, nullptr, visual, 0, 0);                        // 004DDC17..1C
    bool died = false;
    if (was_dead == 0 && vcall<U>(creature, 0x94) != 0) died = true;                         // 004DDC21..31
    else if (was_dead_temp == 0 && recovered_creature_incapacitated_004ef890(creature, nullptr) != 0) died = true; // 004DDC33..42
    if (died) {
        void* death = new_effect();                                                          // 004DDC44..69
        at<U16>(death, 8) = 0x13;                                                            // 004DDC77
        set_subtype(death, k_subtype_magical_007473b8);                                      // 004DDC71..8E
        recovered_effect_creator_00503a00(death, nullptr, at<U>(effect, 0x18));              // 004DDC92..98
        eff_set(death, 0, 0);                                                                // 004DDC9D..A1
        eff_set(death, 1, 1);                                                                // 004DDCA6..AC
        recovered_effect_apply_004d14f0(creature, nullptr, death, 0, 0);                     // 004DDCB1..B6 / 004DDD1F
    } else {
        void* tick = new_effect();                                                           // 004DDCB8..DD (ESI)
        recovered_effect_creator_00503a00(tick, nullptr, at<U>(effect, 0x18));               // 004DDCE1..EF
        at<U16>(tick, 8) = 0x54;                                                             // 004DDCFA
        set_subtype(tick, k_subtype_supernatural_007473c0);                                  // 004DDCF4..17
        at<U>(tick, 0xc) = k_bits_six;                                                       // 004DDD10
        recovered_effect_apply_004d14f0(object, nullptr, tick, 0, 0);                        // 004DDD00..1F
    }
    return 1;                                                                                // 004DDD24..4C (string destroyed)
}

// 004DDD50 OnApplyVampiricRegeneration: RET 0Ch, returns 1. The creator heals min(int0, int1) (signed).
U __fastcall kotor_fastcall_004ddd50(void*, void*, void*, void* effect, U load_game) {
    void* creator = kotor_fastcall_004ae770(server(), nullptr, at<U>(effect, 0x18));         // 004DDD66..79 ([esp0+8])
    if (creator == nullptr) return 1;                                                        // 004DDD7E..86
    I amount = static_cast<I>(eff_int(effect, 0));                                           // 004DDD8E..91 (EBX)
    const I cap = static_cast<I>(eff_int(effect, 1));                                        // 004DDD96..9C (EBP)
    void* heal = new_effect();                                                               // 004DDDA1..C5
    at<U16>(heal, 8) = 0x27;                                                                 // 004DDDCD
    set_subtype(heal, k_subtype_magical_007473b8);                                           // 004DDDC7..EA
    recovered_effect_creator_00503a00(heal, nullptr, at<U>(effect, 0x18));                   // 004DDDEE..F2
    if (cap < amount) amount = cap;                                                          // 004DDDF7..FB (jge)
    eff_set(heal, 0, static_cast<U>(amount));                                                // 004DDDFD..02
    recovered_effect_apply_004d14f0(creator, nullptr, heal, load_game, 0);                   // 004DDE07..13
    return 1;
}

// 004DDE40 OnApplyTemporaryForce: RET 0Ch, returns 0. Stats word +126 (temporary force points) += int0.
U __fastcall kotor_fastcall_004dde40(void*, void*, void* object, void* effect, U) {
    void* creature = as_creature(object);                                                    // 004DDE40..47
    if (creature == nullptr) return 0;                                                       // 004DDE4A..4C
    void* stats = stats_of(creature);                                                        // 004DDE4E
    if (stats == nullptr) return 0;                                                          // 004DDE54..56
    const U16 old = at<U16>(stats, 0x126);                                                   // 004DDE5D
    at<U16>(stats, 0x126) = static_cast<U16>(eff_int(effect, 0) + old);                      // 004DDE64..6D
    return 0;
}

// 004DDE80 OnRemoveTemporaryForce: RET 8, returns 1. Stats word +126 -= int0.
U __fastcall kotor_fastcall_004dde80(void*, void*, void* object, void* effect) {
    void* creature = as_creature(object);                                                    // 004DDE80..87
    if (creature == nullptr) return 1;                                                       // 004DDE8A..8C
    void* stats = stats_of(creature);                                                        // 004DDE8E
    if (stats == nullptr) return 1;                                                          // 004DDE94..96
    const U16 old = at<U16>(stats, 0x126);                                                   // 004DDE9D
    at<U16>(stats, 0x126) = static_cast<U16>(old - eff_int(effect, 0));                      // 004DDEA4..AD
    return 1;
}

// 004DDEC0 OnApplyForceDrain: RET 0Ch, returns 1. Stats word +124 = max(0, (short)(+126 + +124) - int0) using the
// sign of the 32-bit subtraction (sets/dec/and).
U __fastcall kotor_fastcall_004ddec0(void*, void*, void* object, void* effect, U) {
    const U drain = eff_int(effect, 0);                                                      // 004DDEC0..C8 (EDI)
    if (as_creature(object) == nullptr) return 1;                                            // 004DDECD..DC
    void* stats = stats_of(as_creature(object));                                             // 004DDEDE..E5
    const U16 sum = static_cast<U16>(at<U16>(stats, 0x126) + at<U16>(stats, 0x124));         // 004DDEEB..F2
    U value = static_cast<U>(static_cast<I>(static_cast<int16_t>(sum))) - drain;             // 004DDEFE..01
    if ((value >> 31) != 0) value = 0;                                                       // 004DDF03..07
    at<U16>(stats, 0x124) = static_cast<U16>(value);                                         // 004DDF09
    return 1;
}

// 004DDF20 OnRemoveEffectNull: RET 8, returns 1 (the AsSWSCreature call is made, its result unused).
U __fastcall kotor_fastcall_004ddf20(void*, void*, void* object, void*) {
    (void)as_creature(object);                                                               // 004DDF20..26
    return 1;
}

// 004DDF40 OnApplyForceJump: RET 0Ch, returns 0. The caster faces the target (object id 0), a copy of the effect as
// type 67h (supernatural, object id 0 = target) is queued to the caster as AI event 5 after 300 ms, and the caster
// gets a visual effect 3EAh.
U __fastcall kotor_fastcall_004ddf40(void*, void*, void* object, void* effect, U) {
    void* caster = as_creature(object);                                                      // 004DDF46..62 (EBX)
    if (caster == nullptr) return 0;                                                         // 004DDF64..66
    void* target = kotor_fastcall_004ae770(server(), nullptr, kotor_fastcall_005036e0(effect, nullptr, 0)); // 004DDF6C..85 (EDI)
    if (target == nullptr) return 0;                                                         // 004DDF8A..90
    w5_creature_set_lock_orientation_to_object_004f34a0(caster, nullptr, at<U>(target, 4), 0); // 004DDF96..9D
    void* delayed = copy_effect(effect);                                                     // 004DDFA2..C3 (ESI)
    at<U16>(delayed, 8) = 0x67;                                                              // 004DDFCB
    set_subtype(delayed, k_subtype_supernatural_007473c0);                                   // 004DDFC5..E7
    recovered_effect_set_object_id_005036f0(delayed, nullptr, 0, at<U>(target, 4));          // 004DDFEB..F1
    const U caster_id = at<U>(caster, 4);                                                    // 004DDFFC
    enqueue(0x12c, caster_id, caster_id, k_ai_event_jump_007441c8, delayed);                 // 004DDFF6..E01A
    void* visual = copy_effect(effect);                                                      // 004DE01F..48
    at<U16>(visual, 8) = 0x1e;                                                               // 004DE054
    set_subtype(visual, k_subtype_magical_007473b8);                                         // 004DE04E..74
    eff_set(visual, 0, 0x3ea);                                                               // 004DE05A..78
    recovered_effect_apply_004d14f0(caster, nullptr, visual, 0, 0);                          // 004DE07D..84
    return 0;
}

// 004DE0A0 OnApplyForceJumpDelayed: RET 0Ch, returns 0. The caster leaps toward the target (object id 0):
//   N = normalize(target - caster); P = target - N * DesiredAttackRange(target, 1) (floats rounded per component).
//   P safe (TestSafeLocation != 0): the caster quick-walks to P.
//   P unsafe: the target is pushed back to G = target + N * DesiredAttackRange(target, round +9B8) (move-to-point
//   action + quick walk + type 8 effect int0 = 0Dh) and the caster quick-walks to the target position.
// The caster/target pathfinder (+340) gets a 16-byte two-point path (+8C = 4 points, +90 block, +9C = 0) and +A8C mode.
U __fastcall kotor_fastcall_004de0a0(void*, void*, void* object, void* effect, U) {
    void* caster = as_creature(object);                                                      // 004DE0A6..C4 (EBX)
    if (caster == nullptr) return 0;                                                         // 004DE0C6..CA
    void* target = kotor_fastcall_004ae770(server(), nullptr, kotor_fastcall_005036e0(effect, nullptr, 0)); // 004DE0D0..E6 (ESI)
    if (target == nullptr) return 0;                                                         // 004DE0EB..EF
    w5_creature_set_lock_orientation_to_object_004f34a0(caster, nullptr, at<U>(target, 4), 0); // 004DE0F5..FD
    Vector& tpos = at<Vector>(target, 0x90);                                                 // EDI = target + 90h
    Vector d;                                                                                // [esp0-24h]
    d.x = static_cast<float>(static_cast<double>(tpos.x) - at<float>(caster, 0x90));         // 004DE122..2F
    d.y = static_cast<float>(static_cast<double>(tpos.y) - at<float>(caster, 0x94));         // 004DE114..1B / 004DE133
    d.z = static_cast<float>(static_cast<double>(at<float>(target, 0x98)) - at<float>(caster, 0x98)); // 004DE102..08 / 004DE137
    Vector n;                                                                                // [esp0-3Ch]
    normalize_004ab180(&n, &d);                                                              // 004DE13B..43
    Vector p;                                                                                // [esp0-48h]
    float zr;                                                                                // [esp0+4] (object argument slot)
    {
        const double r = kotor_fastcall_004f1310(caster, nullptr, at<U>(target, 4), 1);      // 004DE140..4B
        zr = static_cast<float>(static_cast<double>(n.z) * r);                               // 004DE150..63
        const float yr = static_cast<float>(static_cast<double>(n.y) * r);                   // 004DE168..6F [esp0-4Ch]
        const float xr = static_cast<float>(static_cast<double>(n.x) * r);                   // 004DE173..79 [esp0-24h]
        const float pz = static_cast<float>(static_cast<double>(tpos.z) - zr);               // 004DE17F..82
        const float py = static_cast<float>(static_cast<double>(tpos.y) - yr);               // 004DE186..89
        const float px = static_cast<float>(static_cast<double>(tpos.x) - xr);               // 004DE18D..8F
        p.x = px; p.y = py; p.z = pz;                                                        // 004DE193..9B
    }
    if (recovered_test_safe_location_004aeb50(server(), nullptr, at<U>(target, 4), &p) == 0) { // 004DE19F..A9
        const double r = kotor_fastcall_004f1310(caster, nullptr, at<U>(target, 4),
                                                  at<U>(round_of(caster), 0x9b8));           // 004DE1AF..C2
        zr = static_cast<float>(static_cast<double>(n.z) * r);                               // 004DE1C7..D4
        p.x = tpos.x; p.y = tpos.y; p.z = tpos.z;                                            // 004DE1CD..EF (three dword moves)
        const float yr = static_cast<float>(static_cast<double>(n.y) * r);                   // 004DE1DB..F3 [esp0-4Ch]
        const float xr = static_cast<float>(r * static_cast<double>(n.x));                   // 004DE1F7..FB [esp0-18h]
        Vector g;                                                                            // [esp0-30h]
        const float gz = static_cast<float>(static_cast<double>(zr) + tpos.z);               // 004DE1FF..03
        const float gy = static_cast<float>(static_cast<double>(yr) + tpos.y);               // 004DE206..0A
        const float gx = static_cast<float>(static_cast<double>(xr) + tpos.x);               // 004DE20D..11
        g.x = gx; g.y = gy; g.z = gz;                                                        // 004DE213..1B
        void* tpf = at<void*>(target, 0x340);                                                // 004DE1E5
        if (at<U>(tpf, 0x8c) != 0) {                                                         // 004DE21F..27
            void* old = at<void*>(tpf, 0x90);                                                // 004DE229
            at<U>(tpf, 0x90) = 0;                                                            // 004DE230
            live_free(old);                                                                  // 004DE22F..3B
        }
        U* path = static_cast<U*>(live_operator_new(0x10));                                  // 004DE23E..4F (EBP)
        path[0] = bits(tpos.x);                                                              // 004DE245 / 004DE251
        path[1] = at<U>(target, 0x94);                                                       // 004DE254..5A
        path[2] = bits(g.x);                                                                 // 004DE25D / 004DE26E
        path[3] = bits(g.y);                                                                 // 004DE271 / 004DE276
        Vector zero = {0.0f, 0.0f, 0.0f};                                                    // [esp0-18h] 004DE291..A7
        w5_creature_add_move_to_point_action_to_front_004f8a50(target, nullptr, 0xffff, &g, at<U>(target, 0x8c),
            k_object_invalid, 0, 0.0f, 0.0f, 0, 0, 0, 0, 0, 0, &zero, 0, 0, 0);              // 004DE24A..B2
        at<U>(target, 0xa8c) = 6;                                                            // 004DE2BD
        at<U>(at<void*>(target, 0x340), 0x8c) = 4;                                           // 004DE2B7 / 004DE2C7
        at<U*>(at<void*>(target, 0x340), 0x90) = path;                                       // 004DE2D1..D7
        at<U>(at<void*>(target, 0x340), 0x9c) = 0;                                           // 004DE2DD..E7
        kotor_fastcall_0051d9c0(target, nullptr, 1);                                         // 004DE2E3..ED
        void* knock = copy_effect(effect);                                                   // 004DE2F2..19 (EBP)
        at<U16>(knock, 8) = 8;                                                               // 004DE323
        set_subtype(knock, k_subtype_supernatural_007473c0);                                 // 004DE31D..43
        eff_set(knock, 0, 0xd);                                                              // 004DE329..47
        recovered_effect_apply_004d14f0(target, nullptr, knock, 0, 0);                       // 004DE34C..51
    }
    void* cpf = at<void*>(caster, 0x340);                                                    // 004DE358
    if (at<U>(cpf, 0x8c) != 0) {                                                             // 004DE35E..65
        void* old = at<void*>(cpf, 0x90);                                                    // 004DE367
        at<U>(cpf, 0x90) = 0;                                                                // 004DE36E
        live_free(old);                                                                      // 004DE36D..79
    }
    U* path = static_cast<U*>(live_operator_new(0x10));                                      // 004DE37C..7E
    path[0] = at<U>(caster, 0x90);                                                           // 004DE383..89
    path[1] = at<U>(caster, 0x94);                                                           // 004DE38B..91
    path[2] = bits(p.x);                                                                     // 004DE394..98
    path[3] = bits(p.y);                                                                     // 004DE39B..9F
    at<U>(at<void*>(caster, 0x340), 0x8c) = 4;                                               // 004DE3A2..A8
    at<U*>(at<void*>(caster, 0x340), 0x90) = path;                                           // 004DE3B2..B8
    at<U>(at<void*>(caster, 0x340), 0x9c) = 0;                                               // 004DE3BE..C7
    at<U>(caster, 0xa8c) = 5;                                                                // 004DE3D1
    kotor_fastcall_0051d9c0(caster, nullptr, 1);                                             // 004DE3CD..DB
    return 0;
}

}  // extern "C"
