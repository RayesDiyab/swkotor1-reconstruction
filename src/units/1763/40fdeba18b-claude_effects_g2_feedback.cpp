// Lane effects-g2 (feedback subset): CSWSEffectListHandler poison / disease / curse / silence / blindness /
// hit-point-change-when-dying callbacks, reconstructed from the original swkotor.exe (GOG, read-only).
//  * OnApplyX: thiscall(CSWSObject* object, CGameEffect* effect, int load_game), RET 0Ch
//      -> extern "C" unsigned __fastcall kotor_fastcall_<addr>(void* self, void*, void* object, void* effect, unsigned load_game) @20
//  * OnRemoveX: thiscall(CSWSObject* object, CGameEffect* effect), RET 8 (verified for every remove below)
//      -> extern "C" unsigned __fastcall kotor_fastcall_<addr>(void* self, void*, void* object, void* effect) @16
//  * Extra missing callee: CSWSCreature::BroadcastPoisoned 004ECD60, thiscall(CSWCCMessageData*), RET 4
//      -> extern "C" void __fastcall kotor_fastcall_004ecd60(void* creature, void*, void* data) @12
//  * vtable slots (CSWSCreature vtable 007470C8): +30h AsSWSCreature (00641DB0 self_return on creatures),
//    +94h GetDead (004EF820), +9Ch GetCurrentHitPoints(int) (004CAEC0, returns short in AX).
//  * Constants proven from .rdata: [007441EC]=0Eh (AI event "remove effect"), [007441C8]=5 (AI event "apply effect"),
//    [007473B8]=0 / [007473C0]=1 (duration-type words), [0073D6FC]=1000.0f, [0073E798]=0.1f, [007455C0]=6.0f,
//    [0073D700]=0.0f, [007455BC]=900.0f; 2DA column names "DC_SAVE","Name","DURATION","PERIOD","BLOODCOLR",
//    colour codes "R","G","Y".
//  * Memory: live_operator_new / live_free only. No original address is dereferenced.
//  * The original EH frames only guard the operator-new'd objects/locals during their constructors (delete on throw)
//    and the CExoString locals; none of the bound constructors throw, so the normal-path order is reproduced.
#include "842d6b1618-w5_t3.hpp"
#include <cstdint>
#include <cstring>
#include <cmath>

namespace kotor_reconstruction {
struct StartupString { void* data; w5::U length; int __thiscall BorrowedEquals005e5290(const char* text); }; // 005E5290 [0077]
}

extern "C" {
using w5::U; using w5::I; using w5::U16; using w5::U8;
// ---- CGameEffect ----
U __fastcall recovered_effect_integer_00503690(void* effect, void*, U index);                // GetInteger [w1_native_b7]
void __fastcall recovered_effect_integer_005036a0(void* effect, void*, U index, U value);    // SetInteger [0886]
void __fastcall kotor_fastcall_005036d0(void* effect, void*, U index, U value_bits);         // SetFloat [claude_w1_chains_damage]
void __fastcall recovered_effect_set_object_id_005036f0(void* effect, void*, U index, U id); // SetObjectID [1194u]
void __fastcall recovered_effect_creator_00503a00(void* effect, void*, U id);                // SetCreator [0886]
void* __fastcall recovered_effect_construct_00503e40(void* effect, void*, U flag);           // CGameEffect(int) [0886]
void* __fastcall kotor_fastcall_00503f70(void* effect, void*, void* source);                 // CGameEffect(CGameEffect*) [claude_w1_route_bodies]
void __fastcall recovered_effect_apply_004d14f0(void* object, void*, void* effect, U a, U b); // CSWSObject::ApplyEffect [0886]
// ---- CSWCCMessageData ----
void* __fastcall recovered_message_data_construct_004d69f0(void* data, void*);               // ctor [w1_native_b7]
void __fastcall recovered_message_data_destroy_004d6a40(void* data, void*);                  // dtor [w1_native_b7]
void __fastcall recovered_event_integer_004d76b0(void* data, void*, U index, U value);       // SetInteger [1017]
U __fastcall kotor_fastcall_004d6ee0(void* data, void*, void* other);                        // CopyTo [claude_combat_attack_round]
// ---- creature / stats / server ----
U __fastcall recovered_stats_effect_query_005a6960(void* stats, void*, U kind, void* other); // GetEffectImmunity [1094]
U __fastcall kotor_fastcall_005b92b0(void* creature, void*, U a1, U a2, U a3, U a4, U a5, U a6, U a7); // SavingThrowRoll (AL)
U __fastcall kotor_fastcall_004ee770(void* creature, void*, I row, void* poison, U a3, float strength); // ApplyPoisonDamage
void __fastcall w5_creature_broadcast_voice_chat_004f2520(void* creature, void*, U voice);   // BroadcastVoiceChat
void __fastcall w1x_object_set_hp_004cc280(void* object, void*, U hp);                       // SetCurrentHitPoints [0886]
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);                   // GetServerAIMaster [creature_destructor_r2]
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);                 // GetSWSMessage [creature_destructor_r2]
void* __fastcall kotor_fastcall_004aede0(void* server, void*);                               // GetWorldTimer [0457]
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, U* day, U* time);            // GetWorldTime [0457]
void* __fastcall kotor_fastcall_00561080(void* player, void*);                               // CSWSPlayer::GetGameObject [0595]
void* __fastcall combat_reuse_faction_00513fc0(void* creature, void*);                       // CSWSCreature::GetFaction [w1_native_b7]
U __fastcall combat_reuse_members_005be780(void* faction, void*, void** output);             // CSWSFaction::GetMemberList [w1_native_b7]
U __fastcall recovered_cc_message_00568570(void* message, void*, U player, U kind, void* data, U flag); // SendServerToPlayerCCMessage
I __fastcall c2da_current_get_string_00413270(void* table, void*, I row, void* column, void* out); // C2DA::GetCExoStringEntry [0376]

// ---- exports ----
unsigned __fastcall kotor_fastcall_004db320(void* self, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004db810(void* self, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004db870(void* self, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004dbe60(void* self, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004dc200(void* self, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004dc380(void* self, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004dcd30(void* self, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004dc660(void* self, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004dc7a0(void* self, void*, void* object, void* effect);
void __fastcall kotor_fastcall_004ecd60(void* creature, void*, void* data);
}

namespace {
using namespace w5;
struct ExoString { char* text; U length; };
static_assert(sizeof(ExoString) == 8, "CExoString is 8 bytes");

constexpr U k_event_remove_effect_007441ec = 0x0e;  // .rdata 007441EC
constexpr U k_event_apply_effect_007441c8 = 0x05;   // .rdata 007441C8
constexpr U16 k_duration_instant_007473b8 = 0;      // .rdata 007473B8 (word)
constexpr U16 k_duration_temporary_007473c0 = 1;    // .rdata 007473C0 (word)

inline void* as_creature(void* object) { return vcall<void*>(object, 0x30); }          // AsSWSCreature
inline void* stats(void* creature) { return at<void*>(creature, 0xa74); }
inline void* rules() { return kotor_cell_rules_007a3a28; }
inline U effect_int(void* e, U i) { return recovered_effect_integer_00503690(e, nullptr, i); }
inline void effect_set_int(void* e, U i, U v) { recovered_effect_integer_005036a0(e, nullptr, i, v); }
// inline view of 005036C0 CGameEffect::GetFloat: fld dword [ecx+eax*4+38h]; ret 4
inline float effect_float(void* e, U i) { return at<float>(e, 0x38 + i * 4); }
inline void effect_set_float(void* e, U i, float v) { kotor_fastcall_005036d0(e, nullptr, i, bits(v)); }
inline void set_duration_type(void* e, U16 type) { at<U16>(e, 0xa) = static_cast<U16>((at<U16>(e, 0xa) & 0xfff8u) | type); }
inline void apply(void* object, void* e, U a) { recovered_effect_apply_004d14f0(object, nullptr, e, a, 0); }
inline void* new_message_data() {
    void* memory = live_operator_new(0x34);
    return memory ? recovered_message_data_construct_004d69f0(memory, nullptr) : nullptr;
}
inline void* new_effect_copy(void* source) {
    void* memory = live_operator_new(0x8c);
    return memory ? kotor_fastcall_00503f70(memory, nullptr, source) : nullptr;
}
inline void* new_effect(U flag) {
    void* memory = live_operator_new(0x8c);
    return memory ? recovered_effect_construct_00503e40(memory, nullptr, flag) : nullptr;
}
inline void queue_event(U caller, U target, U type, void* event) {
    recovered_event_enqueue_004b08d0(recovered_server_ai_master_004aed80(server_app(), nullptr), nullptr, 0, 0,
                                     caller, target, type, event);
}
inline void feedback(void* creature, U message, void* data) { recovered_creature_feedback_004ede10(creature, nullptr, message, data); }
inline void* creature_by_id(U id) { return kotor_fastcall_004ae770(server_app(), nullptr, id); }
// 2DA int lookup with a temporary CExoString column (construct / GetINTEntry / destroy)
inline void int_entry(void* table, U row, const char* column, I* out) {
    ExoString name;
    frame_LogStringCtor_005e5a90(&name, nullptr, column);
    (void)c2da_current_get_int_00413660(table, nullptr, row, &name, out);
    frame_LogStringDtor_005e5c20(&name, nullptr);
}
// "immune" feedback: message to the target (and a copy to the creator when it is a creature)
inline void immunity_feedback(void* object, void* creator, U message, bool target_via_vcall, void* target_creature) {
    void* data = new_message_data();
    recovered_event_object_004d7780(data, nullptr, 0, at<U>(object, 4));
    if (creator) {
        void* copy = new_message_data();
        (void)kotor_fastcall_004d6ee0(data, nullptr, copy);
        feedback(creator, message, copy);
    }
    feedback(target_via_vcall ? as_creature(object) : target_creature, message, data);
}
}  // namespace

// ============================================================================================================
// 004DB320 CSWSEffectListHandler::OnApplyPoison(object, effect, load_game), RET 0Ch, EAX always 0
extern "C" unsigned __fastcall kotor_fastcall_004db320(void*, void*, void* object, void* effect, unsigned load_game) {
    void* creature = as_creature(object);                                                   // 004DB339..42
    if (!creature) return 0;                                                                // 004DB349..4B
    if (at<U>(creature, 0xf8) != 0 || vcall<U>(creature, 0x94) != 0 ||                     // 004DB351..69 GetDead
        recovered_creature_incapacitated_004ef890(creature, nullptr) != 0 ||                // 004DB36F..78 GetDeadTemp
        at<U>(creature, 0x9e0) == 1) {                                                      // 004DB37E..87
        queue_event(at<U>(effect, 0x18), at<U>(creature, 4), k_event_remove_effect_007441ec, effect); // 004DB7C1..E6
        return 0;
    }
    if (load_game == 0) {                                                                   // 004DB38D..95
        void* creator = creature_by_id(at<U>(effect, 0x18));                                // 004DB39B..B3
        if (recovered_stats_effect_query_005a6960(stats(creature), nullptr, 2, creator) != 0) { // 004DB3AD..BF
            void* data = new_message_data();                                                // 004DB3C5..E2
            recovered_event_object_004d7780(data, nullptr, 0, at<U>(object, 4));            // 004DB3E4..F8
            recovered_event_integer_004d76b0(data, nullptr, 0, 0);                          // 004DB3FD..403
            recovered_event_integer_004d76b0(data, nullptr, 1, 0);                          // 004DB408..40E
            recovered_event_integer_004d76b0(data, nullptr, 2, 0x2000);                     // 004DB413..41C
            if (creator) {                                                                  // 004DB421..423
                void* copy = new_message_data();                                            // 004DB425..452
                (void)kotor_fastcall_004d6ee0(data, nullptr, copy);                         // 004DB454..463
                feedback(creator, 0x3e, copy);                                              // 004DB468..471
            }
            feedback(creature, 0x3e, data);                                                 // 004DB476..47B
            queue_event(at<U>(effect, 0x18), at<U>(creature, 4), k_event_remove_effect_007441ec, effect); // 004DB480..4E6
            return 0;
        }
        U day, time;
        frame_GetWorldTime_004ade40(kotor_fastcall_004aede0(server_app(), nullptr), nullptr, &day, &time); // 004DB4A2..BB
        effect_set_int(effect, 1, day);                                                     // 004DB4C0..C9
        effect_set_int(effect, 2, time);                                                    // 004DB4CE..D7
        void* poison_2da = at<void*>(at<void*>(rules(), 0xb8), 0x90);                       // 004DB4DC..E7
        if (!poison_2da) {                                                                  // 004DB4ED..EF
            queue_event(at<U>(effect, 0x18), at<U>(creature, 4), k_event_remove_effect_007441ec, effect); // 004DB553..
            return 0;
        }
        const U row = effect_int(effect, 0);                                                // 004DB4F1..503
        I dc;
        int_entry(poison_2da, row, "DC_SAVE", &dc);                                         // 004DB4FA..530 ([007469DC])
        const U saved = static_cast<U8>(kotor_fastcall_005b92b0(creature, nullptr, 1, static_cast<U>(dc), 0x0c,
                                            at<U>(effect, 0x18), 1, 0, 0));                 // 004DB535..551
        if (saved) {
            queue_event(at<U>(effect, 0x18), at<U>(creature, 4), k_event_remove_effect_007441ec, effect); // 004DB553..
            return 0;
        }
        I name_strref;                                                                      // (original reuses the load_game slot)
        int_entry(poison_2da, row, "Name", &name_strref);                                   // 004DB56C..5A0 ([00745EEC])
        void* data = new_message_data();                                                    // 004DB5A5..5D4
        recovered_event_integer_004d76b0(data, nullptr, 0, static_cast<U>(name_strref));    // 004DB5D4..5E7
        recovered_event_object_004d7780(data, nullptr, 0, at<U>(creature, 4));              // 004DB5EC..5F6
        kotor_fastcall_004ecd60(creature, nullptr, data);                                   // 004DB5FB..602 BroadcastPoisoned
        if (data) {                                                                         // 004DB607..60D
            recovered_message_data_destroy_004d6a40(data, nullptr);                         // 004DB60F
            live_free(data);                                                                // 004DB614..61E
        }
        w5_creature_broadcast_voice_chat_004f2520(creature, nullptr, 0x1c);                 // 004DB621..625
        effect_set_float(effect, 0, 2.0f);                                                  // 004DB62A..633 (40000000h)
        (void)kotor_fastcall_004ee770(creature, nullptr, static_cast<I>(row), effect, 0, 1.0f); // 004DB638..643
        effect_set_int(effect, 3, day);                                                     // 004DB648..651
        effect_set_int(effect, 4, time);                                                    // 004DB656..65F
        I duration;
        int_entry(poison_2da, row, "DURATION", &duration);                                  // 004DB664..698 ([007469D0])
        effect_set_int(effect, 5, static_cast<U>(duration));                                // 004DB69D..6A6
        const double scaled = static_cast<double>(duration) * 1000.0;                       // 004DB6AB..6B5 fild*[0073D6FC]
        at<float>(effect, 0xc) = static_cast<float>(scaled);                                // 004DB6CD fstp dword
        set_duration_type(effect, k_duration_temporary_007473c0);                           // 004DB6AF..6D0
        I period;
        int_entry(poison_2da, row, "PERIOD", &period);                                      // 004DB6BB..703 ([007469C8])
        effect_set_int(effect, 6, static_cast<U>(period));                                  // 004DB708..711
        effect_set_int(effect, 7, 0);                                                       // 004DB716..71C
    }
    void* marker = new_effect(1);                                                           // 004DB721..74B
    at<U16>(marker, 8) = 0x1e;                                                              // 004DB753
    set_duration_type(marker, k_duration_instant_007473b8);                                 // 004DB74D..777
    effect_set_int(marker, 0, 0x3eb);                                                       // 004DB759..77B
    effect_set_int(marker, 2, 0);                                                           // 004DB780..786
    recovered_effect_set_object_id_005036f0(marker, nullptr, 0, at<U>(effect, 0x18));      // 004DB78B..793
    apply(creature, marker, 0);                                                             // 004DB798..79F
    at<U>(creature, 0x9e0) = 1;                                                             // 004DB7A9
    at<U>(creature, 0x9e8) = at<U>(effect, 0);                                              // 004DB7A4..7B3 effect id lo
    at<U>(creature, 0x9ec) = at<U>(effect, 4);                                              // 004DB7A6..7B9 effect id hi
    return 0;
}

// 004DB810 CSWSEffectListHandler::OnRemovePoison(object, effect), RET 8, EAX 1
extern "C" unsigned __fastcall kotor_fastcall_004db810(void*, void*, void* object, void* effect) {
    if (object && as_creature(object)) {                                                    // 004DB815..822
        void* creature = as_creature(object);                                               // 004DB824..828
        if (at<U>(creature, 0x9e0) == 1 && at<U>(creature, 0x9e8) == at<U>(effect, 0) &&
            at<U>(creature, 0x9ec) == at<U>(effect, 4)) {                                   // 004DB82B..84B
            at<U>(creature, 0x9e0) = 0;                                                     // 004DB850
            at<U>(creature, 0x9e8) = 0xffffffffu;                                           // 004DB85A
            at<U>(creature, 0x9ec) = 0xffffffffu;                                           // 004DB860
        }
    }
    return 1;
}

// 004DB870 CSWSEffectListHandler::OnRemoveDisease(object, effect), RET 8, EAX 1
extern "C" unsigned __fastcall kotor_fastcall_004db870(void*, void*, void* object, void*) {
    if (object && as_creature(object)) at<U>(as_creature(object), 0x9e4) = 0;              // 004DB875..88B
    return 1;
}

// 004DBE60 CSWSEffectListHandler::OnApplyCurse(object, effect, load_game), RET 0Ch
extern "C" unsigned __fastcall kotor_fastcall_004dbe60(void*, void*, void* object, void* effect, unsigned load_game) {
    void* creature = as_creature(object);                                                   // 004DBE7A..84
    if (!creature) return 0;                                                                // 004DBE86..88
    if (at<U>(creature, 0xf8) != 0) return 1;                                               // 004DBE8E..96
    void* creator = creature_by_id(at<U>(effect, 0x18));                                    // 004DBEA0..B8
    // quirk: the creator pointer is stored over the object argument slot ([esp+20h]) and re-read below
    if (recovered_stats_effect_query_005a6960(stats(creature), nullptr, 0x11, creator) != 0) { // 004DBEB2..C8
        immunity_feedback(object, creator, 0x8d, true, nullptr);                            // 004DBECE..F5A
        return 1;                                                                           // 004DBF5F
    }
    // six ability-decrease children (type 25h): child[0] = ability index k, child[1] = curse value k
    for (U k = 0; k < 6; ++k) {                                                             // 004DBF69..C1D6
        void* child = new_effect_copy(effect);
        effect_set_int(child, 0, k);
        at<U16>(child, 8) = 0x25;
        effect_set_int(child, 1, effect_int(effect, k));
        if (k == 2) {
            // quirk: the third child is not applied directly; it is queued as an AI "apply effect" event from the
            // creator (unchecked creator dereference) to the object                        // 004DC088..0B0
            queue_event(at<U>(creator, 4), at<U>(object, 4), k_event_apply_effect_007441c8, child);
        } else {
            apply(object, child, load_game);
        }
    }
    return 0;                                                                               // 004DC1DB
}

// 004DC200 CSWSEffectListHandler::OnApplySilence(object, effect, load_game), RET 0Ch
extern "C" unsigned __fastcall kotor_fastcall_004dc200(void*, void*, void* object, void* effect, unsigned load_game) {
    void* creature = as_creature(object);                                                   // 004DC21B..225
    if (!creature) return 0;                                                                // 004DC227..229
    void* creator = creature_by_id(at<U>(effect, 0x18));                                    // 004DC22F..24B
    if (recovered_stats_effect_query_005a6960(stats(creature), nullptr, 0x0b, creator) != 0) { // 004DC245..257
        immunity_feedback(object, creator, 0x91, true, nullptr);                            // 004DC25D..2EA
        return 1;                                                                           // 004DC2EF
    }
    if (at<U>(creature, 0xf8) != 0) return 1;                                               // 004DC2F6..2FE
    void* child = new_effect_copy(effect);                                                  // 004DC300..329
    at<U16>(child, 8) = 0x17;                                                               // 004DC339
    effect_set_int(child, 0, static_cast<U>(-9));                                           // 004DC32B..33F
    apply(creature, child, load_game);                                                      // 004DC344..34E
    at<U>(creature, 0x8c8) = 1;                                                             // 004DC353
    return 0;
}

// 004DC380 CSWSEffectListHandler::OnRemoveSilence(object, effect), RET 8, EAX 1 (object not null-checked)
extern "C" unsigned __fastcall kotor_fastcall_004dc380(void*, void*, void* object, void*) {
    if (as_creature(object)) at<U>(as_creature(object), 0x8c8) = 0;                        // 004DC385..397
    return 1;
}

// 004DCD30 CSWSEffectListHandler::OnApplyBlindness(object, effect, load_game), RET 0Ch
extern "C" unsigned __fastcall kotor_fastcall_004dcd30(void*, void*, void* object, void* effect, unsigned load_game) {
    if (!as_creature(object)) return 0;                                                     // 004DCD4B..57
    void* creature = as_creature(object);                                                   // 004DCD5D..61
    const U kind = effect_int(effect, 0);                                                   // 004DCD64..75 (kept in the object slot)
    if (!creature || (kind != 0x10 && kind != 8)) return 1;                                 // 004DCD73..87
    void* creator = creature_by_id(at<U>(effect, 0x18));                                    // 004DCD8D..9F
    if (kind == 0x10 &&
        recovered_stats_effect_query_005a6960(stats(creature), nullptr, 7, creator) != 0) { // 004DCDA5..BE
        immunity_feedback(object, creator, 0x8b, true, nullptr);                            // 004DCDC4..E51
        return 1;                                                                           // 004DCE56
    }
    if (at<U>(creature, 0xf8) != 0) return 1;                                               // 004DCE64..6C
    U deafness_flag;
    if (kind == 8) {                                                                        // 004DCE6E..71
        const U8 flags = at<U8>(creature, 0x8ec);                                           // 004DCE73
        if ((flags & 2) != 0 || (flags & 4) != 0) return 0;                                 // 004DCE79..88
        deafness_flag = 1;                                                                  // 004DCE7B
    } else {
        deafness_flag = 0;                                                                  // 004DCEB7..BC (kind 10h)
    }
    void* first = new_effect_copy(effect);                                                  // 004DCE8E..EC3
    at<U16>(first, 8) = 0x4b;                                                               // 004DCED2
    effect_set_int(first, 0, 0x32);                                                         // 004DCEC5..ED8
    effect_set_int(first, 1, deafness_flag);                                                // 004DCEDD..EE2
    apply(creature, first, load_game);                                                      // 004DCEE7..EF1
    void* second = new_effect_copy(effect);                                                 // 004DCEF6..F23
    at<U16>(second, 8) = 0x45;                                                              // 004DCF2F
    effect_set_int(second, 0, 4);                                                           // 004DCF25..F35
    apply(creature, second, load_game);                                                     // 004DCF3A..F40
    void* third = new_effect_copy(effect);                                                  // 004DCF45..F72
    at<U16>(third, 8) = 0x1e;                                                               // 004DCF81
    effect_set_int(third, 0, 0x138a);                                                       // 004DCF74..F87
    apply(creature, third, load_game);                                                      // 004DCF8C..F92
    at<U8>(creature, 0x8ec) = static_cast<U8>(at<U8>(creature, 0x8ec) | static_cast<U8>(kind)); // 004DCF97..FA3
    return 0;
}

// 004DC660 CSWSEffectListHandler::OnApplyHitPointChangeWhenDying(object, effect, load_game), RET 0Ch
extern "C" unsigned __fastcall kotor_fastcall_004dc660(void*, void*, void* object, void* effect, unsigned load_game) {
    if (!as_creature(object)) return 1;                                                     // 004DC676..685
    if (at<U>(stats(as_creature(object)), 0x6c) == 0) return 1;                             // 004DC68B..69D
    if (static_cast<U16>(at<U8>(effect, 0xa) & 7u) != k_duration_instant_007473b8) return 0; // 004DC6A7..6B6
    // fcomp [0073E798]; test ah,1: C0 (below) or unordered -> exit
    const double amount = effect_float(effect, 0);                                          // 004DC6BC..6C0
    if (!(amount >= static_cast<double>(0.1f))) return 0;                                   // 004DC6C5..6D0
    void* child = new_effect(1);                                                            // 004DC6D6..700
    recovered_effect_creator_00503a00(child, nullptr, at<U>(effect, 0x18));                 // 004DC702..710
    at<U16>(child, 8) = 0x39;                                                               // 004DC71B
    set_duration_type(child, k_duration_temporary_007473c0);                                // 004DC715..732
    effect_set_float(child, 0, effect_float(effect, 0));                                    // 004DC721..743
    const double rate = static_cast<double>(6.0f) / static_cast<double>(effect_float(effect, 0)); // 004DC748..751 fdivr
    at<float>(child, 0xc) = static_cast<float>(std::fabs(rate)); // 004DC761..763 fabs; fstp
    apply(object, child, load_game);                                                        // 004DC757..766
    return 0;
}

// 004DC7A0 CSWSEffectListHandler::OnRemoveHitPointChangeWhenDying(object, effect), RET 8, EAX 1
extern "C" unsigned __fastcall kotor_fastcall_004dc7a0(void*, void*, void* object, void* effect) {
    // quirk: a non-creature object reaches the GetDead vcall below with a null creature (access violation)
    void* volatile creature = as_creature(object);                                          // 004DC7B9..7CB
    if (creature && at<U>(stats(creature), 0x6c) != 0) {                                    // 004DC7CF..7E0
        const double amount = effect_float(effect, 0);                                      // 004DC7E6..7E9
        if (amount > 0.0) {                                                                 // 004DC7EE..7FF (test ah,41h)
            const I hp = static_cast<std::int16_t>(vcall<U>(creature, 0x9c, 1u));          // 004DC801..807
            w1x_object_set_hp_004cc280(creature, nullptr, static_cast<U>(hp + 1));          // 004DC80A..80E
        } else {
            const I hp = static_cast<std::int16_t>(vcall<U>(creature, 0x9c, 1u));          // 004DC818..81E
            w1x_object_set_hp_004cc280(creature, nullptr, static_cast<U>(hp - 1));          // 004DC821..825
            ExoString blood;
            kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(&blood), nullptr); // 004DC82A..82E
            ExoString column;
            frame_LogStringCtor_005e5a90(&column, nullptr, "BLOODCOLR");                    // 004DC833..840 ([007469A8])
            (void)c2da_current_get_string_00413270(at<void*>(at<void*>(rules(), 0xb8), 0xc), nullptr,
                                                    static_cast<I>(at<U16>(creature, 0xa60)), &column, &blood); // 004DC845..86D
            frame_LogStringDtor_005e5c20(&column, nullptr);                                 // 004DC872..87B
            void* child = new_effect(1);                                                    // 004DC880..8A3 (null if allocation fails)
            at<U16>(child, 8) = 0x1e;                                                       // 004DC8AB
            set_duration_type(child, k_duration_instant_007473b8);                          // 004DC8A5..8CB
            auto* text = reinterpret_cast<kotor_reconstruction::StartupString*>(&blood);
            if (text->BorrowedEquals005e5290("R") != 0) effect_set_int(child, 0, 0x9e);      // 004DC8B1..8D8 ([007469A4])
            else if (text->BorrowedEquals005e5290("G") != 0) effect_set_int(child, 0, 0x9f); // 004DC8DF..8F1 ([007469A0])
            else if (text->BorrowedEquals005e5290("Y") != 0) effect_set_int(child, 0, 0xa0); // 004DC8F8..90A ([0074699C])
            apply(creature, child, 0);                                                      // 004DC918..91F
            frame_LogStringDtor_005e5c20(&blood, nullptr);                                  // 004DC924..930
        }
    }
    if (kotor_m15e_slot010_zero_0063e7f0(creature, nullptr) != 0) {                        // 004DC935..93E return_zero
        void* child = new_effect(1);                                                        // 004DC944..96E
        recovered_effect_creator_00503a00(child, nullptr, at<U>(effect, 0x18));             // 004DC970..97E
        at<U16>(child, 8) = 0x39;                                                           // 004DC989
        set_duration_type(child, k_duration_temporary_007473c0);                            // 004DC983..9A0
        U duration = at<U>(effect, 0xc);                                                    // 004DC9A4 fld dword
        if ((duration & 0x7f800000u) == 0x7f800000u && (duration & 0x007fffffu) != 0) duration |= 0x00400000u; // x87 load quiets SNaN
        at<U>(child, 0xc) = duration;                                                       // 004DC9A7 fstp dword
        effect_set_float(child, 0, effect_float(effect, 0));                                // 004DC98F..9B7
        apply(object, child, 0);                                                            // 004DC9BC..9C3 -> 004DCA5A
        return 1;
    }
    if (vcall<U>(creature, 0x94) != 0 ||                                                    // 004DC9C8..9D4 GetDead
        recovered_creature_incapacitated_004ef890(creature, nullptr) != 0) {                // 004DC9D6..9DF GetDeadTemp
        void* child = new_effect(1);                                                        // 004DC9E1..A0B
        at<U16>(child, 8) = 0x13;                                                           // 004DCA13
        set_duration_type(child, k_duration_instant_007473b8);                              // 004DCA0D..A2E
        recovered_effect_creator_00503a00(child, nullptr, at<U>(effect, 0x18));             // 004DCA32..A38
        effect_set_int(child, 0, 0);                                                        // 004DCA3D..A43
        effect_set_int(child, 1, 1);                                                        // 004DCA48..A4E
        apply(creature, child, 0);                                                          // 004DCA53..A5A
    }
    return 1;
}

// ============================================================================================================
// 004ECD60 CSWSCreature::BroadcastPoisoned(CSWCCMessageData*), RET 4: CC message 1Bh to every faction member's
// player within 30 m (squared distance <= 900) in the same area
extern "C" void __fastcall kotor_fastcall_004ecd60(void* creature, void*, void* data) {
    void* faction = combat_reuse_faction_00513fc0(creature, nullptr);                      // 004ECD65..70
    if (!faction) return;
    void* list = nullptr;                                                                   // 004ECD7D
    const I count = static_cast<I>(combat_reuse_members_005be780(faction, nullptr, &list)); // 004ECD76..8C
    if (count <= 0) return;                                                                 // jle
    for (I i = 0; i < count; ++i) {                                                         // 004ECDA0..E41
        void* player = kotor_fastcall_004ae910(server_app(), nullptr, static_cast<U*>(list)[i]); // 004ECDA0..B6
        if (!player) continue;
        void* member = kotor_fastcall_00561080(player, nullptr);                            // 004ECDBC..C5
        if (!member) continue;
        if (at<U>(creature, 0x8c) != at<U>(member, 0x8c)) continue;                         // 004ECDC7..D3 area id
        const double dx = static_cast<double>(at<float>(member, 0x90)) - static_cast<double>(at<float>(creature, 0x90));
        const double dy = static_cast<double>(at<float>(member, 0x94)) - static_cast<double>(at<float>(creature, 0x94));
        const double dz = static_cast<double>(at<float>(member, 0x98)) - static_cast<double>(at<float>(creature, 0x98));
        const double distance2 = (dz * dz + dy * dy) + dx * dx;                             // 004ECDF9..E07
        if (!(distance2 <= static_cast<double>(900.0f))) continue;                          // 004ECE09..E1A (jp: above/unordered skip)
        (void)recovered_cc_message_00568570(recovered_server_sws_message_004aedb0(server_app(), nullptr), nullptr,
                                            at<U>(player, 4), 0x1b, data, 0);               // 004ECE1C..E35
    }
}
