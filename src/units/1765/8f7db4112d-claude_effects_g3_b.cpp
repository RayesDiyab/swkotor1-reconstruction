// effects-g3 part B: CSWSEffectListHandler callbacks 004DE400..004DFA40 (0854 ai_resolve, effect dispatcher).
//   004DE400 OnRemoveForcePush          RET 8   -> kotor_fastcall_004de400@16 (object, effect)
//   004DE4D0 OnApplyDamageForcePoints   RET 0Ch -> kotor_fastcall_004de4d0@20 (object, effect, load_game)
//   004DE540 OnApplyHealForcePoints     RET 0Ch -> kotor_fastcall_004de540@20
//   004DE5B0 OnApplyBodyFuel            RET 0Ch -> kotor_fastcall_004de5b0@20
//   004DE5D0 OnRemoveBodyFuel           RET 8   -> kotor_fastcall_004de5d0@16
//   004DE5F0 OnApplyEffectNull          RET 0Ch -> kotor_fastcall_004de5f0@20
//   004DE600 OnApplyLightsaberThrow     RET 0Ch -> kotor_fastcall_004de600@20
//   004DECE0 OnRemoveLightsaberThrow    RET 8   -> kotor_fastcall_004dece0@16
//   004DED00 OnApplyForceResisted       RET 0Ch -> kotor_fastcall_004ded00@20
//   004DEDE0 OnApplyForceFizzle         RET 0Ch -> kotor_fastcall_004dede0@20
//   004DEE90 OnApplyPureGoodPowers      RET 0Ch -> kotor_fastcall_004dee90@20
//   004DF160 OnApplyPureEvilPowers      RET 0Ch -> kotor_fastcall_004df160@20
//   004DF410 OnRemoveStatChange1        RET 8   -> kotor_fastcall_004df410@16
//   004DF540 OnApplyForceShield         RET 0Ch -> kotor_fastcall_004df540@20
// All are thiscall on CSWSEffectListHandler (ECX unused by every body). Transcribed from the original
// swkotor.exe (read-only); addresses in comments are original instructions. Object vtable slots used:
// +10h AsSWSObject-style accessor (position at +90h), +30h AsSWSCreature, +7Ch (int) notifier, +98h/+9Ch
// max / current hit points, +A8h/+ACh damage immunity / resistance filters.
// CGameEffect: +08 type word, +0A subtype word (low 3 bits duration kind), +0C duration float, +18 creator.
// `new CGameEffect` failure leaves a null effect that is used anyway (original quirk, kept).
#include "842d6b1618-w5_t3.hpp"

using namespace w5;

extern "C" {
// ---------------- imports: existing providers (c27 candidate map) ----------------
U __fastcall recovered_effect_integer_00503690(void* effect, void*, U index);                    // CGameEffect::GetInteger [W1 b7]
void __fastcall recovered_effect_integer_005036a0(void* effect, void*, U index, U value);        // CGameEffect::SetInteger [0886]
void __fastcall recovered_effect_set_object_id_005036f0(void* effect, void*, U index, U id);     // CGameEffect::SetObjectID [1194u]
void __fastcall recovered_effect_creator_00503a00(void* effect, void*, U id);                    // CGameEffect::SetCreator [0886]
void* __fastcall recovered_effect_construct_00503e40(void* self, void*, U a);                    // CGameEffect::CGameEffect(int) [0886]
void* __fastcall kotor_fastcall_00503f70(void* self, void*, void* source);                       // CGameEffect::CGameEffect(copy) [claude_w1_route_bodies]
void __fastcall recovered_effect_resize_00503630(void* effect, void*, U count);                  // CGameEffect::SetNumIntegers [1091]
void __fastcall recovered_effect_apply_004d14f0(void* object, void*, void* effect, U a, U b);    // CSWSObject::ApplyEffect [0886]
U __fastcall recovered_object_remove_effect_004d06c0(void* object, void*, U lo, U hi);           // CSWSObject::RemoveEffectById [W1 b7]
void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void* spells, void*, U id);           // CSWSpellArray::GetSpell [0820]
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);                       // CServerExoApp::GetServerAIMaster
U8 __fastcall recovered_stats_level_005a5fd0(void* stats, void*, U flag);                        // CSWSCreatureStats::GetLevel (AL) [0963]
U __fastcall recovered_rules_roll_dice_00550b60(void* rules, void*, U count, U sides);           // CSWRules::RollDice (AX) [W1 b7]
U16 __fastcall w1x_creature_max_fp_004fd490(void* creature, void*);                              // CSWSCreature::GetMaxForcePoints (AX) [0886]
U __fastcall kt_get_class_005a4e90(void* stats, void*, U index);                                 // CSWSCreatureStats::GetClass (AL)
void __fastcall w1x_object_set_hp_004cc280(void* object, void*, U hp);                           // CSWSObject::SetCurrentHitPoints [0886]
void* __fastcall recovered_creature_client_creature_004efb20(void* creature, void*);             // CSWSCreature::GetClientCreature [W1 b7]
U __fastcall kotor_animation_sound_damage_effect_00616890(void* client_creature, void*);         // CSWCCreature::GetForceShield
void* __fastcall kotor_fastcall_005e5bc0(void* self, void*, I value);                            // CExoString::CExoString(int) [0266]
void* __fastcall frame_LogAssign_005e5140(void* self, void*, const char* text);                  // CExoString::operator=(char*) [0459]
I __fastcall kotor_fastcall_00414110(void* table, void*, void* row, void* column, I* out);       // C2DA::GetINTEntry(CExoString,CExoString,int*) [0594]
// declared by w5_t2/w5_t3/w5_server_env: kotor_fastcall_004ae750 GetGameObject, kotor_fastcall_004ae910
// GetClientObjectByObjectId, recovered_event_enqueue_004b08d0 AddEventDeltaTime, recovered_equipment_item_005a4c20
// GetItemInSlot, w5_creature_set_lock_orientation_to_object_004f34a0, kotor_cdecl_004ab100 vector_magnitude (ST0),
// kotor_cdecl_00423bc0 Vector operator-, frame_LogStringCtor_005e5a90 / frame_LogStringDtor_005e5c20, kotor_cell_rules_007a3a28.
// ---------------- sibling sub-worker A (effects-g3) ----------------
U __fastcall kotor_fastcall_005036e0(void* effect, void*, U index);                              // CGameEffect::GetObjectID
// ---------------- exports ----------------
U __fastcall kotor_fastcall_004de400(void* self, void*, void* object, void* effect);
U __fastcall kotor_fastcall_004de4d0(void* self, void*, void* object, void* effect, U load_game);
U __fastcall kotor_fastcall_004de540(void* self, void*, void* object, void* effect, U load_game);
U __fastcall kotor_fastcall_004de5b0(void* self, void*, void* object, void* effect, U load_game);
U __fastcall kotor_fastcall_004de5d0(void* self, void*, void* object, void* effect);
U __fastcall kotor_fastcall_004de5f0(void* self, void*, void* object, void* effect, U load_game);
U __fastcall kotor_fastcall_004de600(void* self, void*, void* object, void* effect, U load_game);
U __fastcall kotor_fastcall_004dece0(void* self, void*, void* object, void* effect);
U __fastcall kotor_fastcall_004ded00(void* self, void*, void* object, void* effect, U load_game);
U __fastcall kotor_fastcall_004dede0(void* self, void*, void* object, void* effect, U load_game);
U __fastcall kotor_fastcall_004dee90(void* self, void*, void* object, void* effect, U load_game);
U __fastcall kotor_fastcall_004df160(void* self, void*, void* object, void* effect, U load_game);
U __fastcall kotor_fastcall_004df410(void* self, void*, void* object, void* effect);
U __fastcall kotor_fastcall_004df540(void* self, void*, void* object, void* effect, U load_game);
}
namespace w2 { unsigned __cdecl ftol2_006fae8c(double value); }                                  // __ftol2 low dword [w2_ftol_native]

namespace {
using I16 = std::int16_t;
constexpr U16 k_duration_instant_007473b8 = 0;       // .rdata 007473B8 word
constexpr U16 k_duration_temporary_007473c0 = 1;     // .rdata 007473C0 word
constexpr U k_float_zero_0073d700 = 0x00000000u;     // .rdata 0073D700 (0.0f)
constexpr U k_float_2000_007469e4 = 0x44fa0000u;     // .rdata 007469E4 (2000.0f)
constexpr U k_float_half_0073e9ac = 0x3f000000u;     // .rdata 0073E9AC (0.5f)
constexpr U k_event_apply_effect_007441c8 = 5;       // .rdata 007441C8 dword
constexpr U8 k_class_a_0074b31c = 3;                 // .rdata 0074B31C byte
constexpr U8 k_class_b_0074b31d = 4;                 // .rdata 0074B31D byte
constexpr U8 k_class_c_0074b31e = 5;                 // .rdata 0074B31E byte
const char k_visual_effect_def_00746aac[] = "VisualEffectDef";
const char k_appearance_01_00746a9c[] = "Appearance_01";
const char k_visual_effect_01_00746a8c[] = "VisualEffect_01";
const char k_appearance_02_00746a7c[] = "Appearance_02";
const char k_visual_effect_02_00746a6c[] = "VisualEffect_02";
const char k_appearance_03_00746a5c[] = "Appearance_03";
const char k_visual_effect_03_00746a4c[] = "VisualEffect_03";
const char k_appearance_04_00746a3c[] = "Appearance_04";
const char k_visual_effect_04_00746a2c[] = "VisualEffect_04";
const char k_damage_flags_00746a20[] = "DamageFlags";
const char k_vulner_flags_00746a14[] = "VulnerFlags";
const char k_resistance_00746a08[] = "Resistance";
const char k_amount_00746a00[] = "Amount";
const char k_permanent_007469f4[] = "Permanent";

inline void* rules() { return kotor_cell_rules_007a3a28; }
inline void* as_creature(void* object) { return vcall<void*>(object, 0x30); }
inline U get_int(void* e, U index) { return recovered_effect_integer_00503690(e, nullptr, index); }
inline void set_int(void* e, U index, U value) { recovered_effect_integer_005036a0(e, nullptr, index, value); }
inline void apply(void* object, void* e, U a, U b) { recovered_effect_apply_004d14f0(object, nullptr, e, a, b); }
inline void set_duration_kind(void* e, U16 kind) { at<U16>(e, 0xa) = static_cast<U16>((at<U16>(e, 0xa) & 0xfff8u) | kind); }

// operator new(8Ch) + CGameEffect(1) / CGameEffect(const CGameEffect*); a throwing constructor frees the block
// (the original EH state), a failed allocation yields a null effect.
void* new_effect() {
    void* memory = live_operator_new(0x8c);
    if (!memory) return nullptr;
    try { return recovered_effect_construct_00503e40(memory, nullptr, 1); }
    catch (...) { live_free(memory); throw; }
}
void* new_effect_copy(void* source) {
    void* memory = live_operator_new(0x8c);
    if (!memory) return nullptr;
    try { return kotor_fastcall_00503f70(memory, nullptr, source); }
    catch (...) { live_free(memory); throw; }
}

// The inline distance blocks of OnApplyLightsaberThrow: B->vfunc10 first, then A->vfunc10; the three components
// A-B are stored as floats (z, y, x order of the fstp) and passed to vector_magnitude (ST0, unrounded).
double distance(void* a_object, void* b_object) {
    void* b = vcall<void*>(b_object, 0x10);
    void* a = vcall<void*>(a_object, 0x10);
    Vector v;
    v.z = at<float>(a, 0x98) - at<float>(b, 0x98);
    v.y = at<float>(a, 0x94) - at<float>(b, 0x94);
    v.x = at<float>(a, 0x90) - at<float>(b, 0x90);
    return kotor_cdecl_004ab100(&v);
}

// 8-byte CExoString on the stack with its destructor (EH order as the original frame).
struct Str {
    void* text = nullptr; U length = 0;
    Str() = default;
    Str(const Str&) = delete;
    Str& operator=(const Str&) = delete;
    ~Str() { frame_LogStringDtor_005e5c20(this, nullptr); }
};
}  // namespace

extern "C" {

// 004DE400 OnRemoveForcePush(object, effect): RET 8, EAX 1.
U __fastcall kotor_fastcall_004de400(void*, void*, void* object, void*) {
    void* creature = as_creature(object);                                           // 004DE419..20
    if (creature) {
        w5_creature_set_lock_orientation_to_object_004f34a0(creature, nullptr, k_object_invalid, 0); // 004DE42A..33
        void* e = new_effect();                                                     // 004DE438..62
        const U creator = at<U>(e, 0x18);                                           // 004DE464
        at<U>(e, 0xc) = 0x40233333u;                                                // 004DE483 duration 2.55f
        set_duration_kind(e, k_duration_temporary_007473c0);                        // 004DE467..8A
        recovered_effect_creator_00503a00(e, nullptr, creator);                     // 004DE48E..90 (its own +18)
        at<U16>(e, 8) = 0x17;                                                       // 004DE49E
        set_int(e, 0, 0xfffffea1u);                                                 // 004DE495..A4
        apply(creature, e, 0, 0);                                                   // 004DE4A9..B0
    }
    return 1;                                                                       // 004DE4BA
}

// 004DE4D0 OnApplyDamageForcePoints: RET 0Ch, EAX 1.
U __fastcall kotor_fastcall_004de4d0(void*, void*, void* object, void* effect, U) {
    void* creature = as_creature(object);                                           // 004DE4D4..DD
    if (creature && at<void*>(creature, 0xa74)) {                                   // 004DE4DF..EB
        const I amount = static_cast<I>(get_int(effect, 0));                        // 004DE4ED..F2
        void* stats = at<void*>(creature, 0xa74);                                   // 004DE4F7
        U16 value = 0;                                                              // EDI = 0
        const I total = static_cast<I16>(static_cast<U16>(at<U16>(stats, 0x126) + at<U16>(stats, 0x124))); // 004DE4FD..0B
        if (total > amount)                                                         // 004DE50E..10 jle (signed)
            value = static_cast<U16>(static_cast<I16>(static_cast<U16>(at<U16>(stats, 0x126) + at<U16>(stats, 0x124))) - amount); // 004DE512..23
        at<U16>(stats, 0x124) = value;                                              // 004DE525 (base FP = total - amount, quirk)
    }
    return 1;
}

// 004DE540 OnApplyHealForcePoints: RET 0Ch, EAX 1.
U __fastcall kotor_fastcall_004de540(void*, void*, void* object, void* effect, U) {
    void* creature = as_creature(object);                                           // 004DE540..4A
    if (creature && at<void*>(creature, 0xa74)) {                                   // 004DE54C..58
        const I amount = static_cast<I>(get_int(effect, 0));                        // 004DE55A..61
        void* stats = at<void*>(creature, 0xa74);
        I value = static_cast<I16>(static_cast<U16>(at<U16>(stats, 0x126) + at<U16>(stats, 0x124))) + amount; // 004DE566..7F
        if (value > static_cast<I16>(w1x_creature_max_fp_004fd490(creature, nullptr)))  // 004DE57A..8B jle (signed)
            value = static_cast<I16>(w1x_creature_max_fp_004fd490(creature, nullptr));  // 004DE58D..94
        at<U16>(at<void*>(creature, 0xa74), 0x124) = static_cast<U16>(value);       // 004DE597..9D
    }
    return 1;
}

// 004DE5B0 OnApplyBodyFuel: RET 0Ch, EAX 0.
U __fastcall kotor_fastcall_004de5b0(void*, void*, void* object, void*, U) {
    void* creature = as_creature(object);
    if (creature) at<U>(creature, 0xa9c) = 1;                                       // 004DE5BD
    return 0;
}

// 004DE5D0 OnRemoveBodyFuel: RET 8, EAX 1.
U __fastcall kotor_fastcall_004de5d0(void*, void*, void* object, void*) {
    void* creature = as_creature(object);
    if (creature) at<U>(creature, 0xa9c) = 0;                                       // 004DE5DD
    return 1;
}

// 004DE5F0 OnApplyEffectNull: RET 0Ch, EAX 0 (the AsSWSCreature call is kept).
U __fastcall kotor_fastcall_004de5f0(void*, void*, void* object, void*, U) {
    (void)as_creature(object);                                                      // 004DE5F4..F6
    return 0;
}

// 004DE600 OnApplyLightsaberThrow: RET 0Ch, EAX 0.
// Frame mirror: ids[0..3] = [esp+50..5C] (thrower id, GetObjectID 0/1/2), objs[0..3] = [esp+40..4C]. The loop's
// continuation read [esp+i*4+40] reaches ids[0] for i = 4 (out-of-array, original layout), and its ids[i] read
// reaches the saved SEH link for i = 4; both only feed decisions that are 0 / continue there anyway.
U __fastcall kotor_fastcall_004de600(void*, void*, void* object, void* effect, U) {
    void* creature = as_creature(object);                                           // 004DE61F..24
    if (!creature) return 0;                                                        // 004DE62D
    at<U>(creature, 0xa04) = 1;                                                     // 004DE633
    if (!recovered_equipment_item_005a4c20(at<void*>(creature, 0xa2c), nullptr, 0x10)) return 0; // 004DE63D..4E
    U ids[4];
    ids[0] = at<U>(object, 4);                                                      // 004DE654
    ids[1] = kotor_fastcall_005036e0(effect, nullptr, 0);                           // 004DE65B..63
    ids[2] = kotor_fastcall_005036e0(effect, nullptr, 1);                           // 004DE66A..72
    ids[3] = kotor_fastcall_005036e0(effect, nullptr, 2);                           // 004DE677..7F
    void* spells = at<void*>(rules(), 0x8c);
    void* spell = get_int(effect, 0) != 0                                            // 004DE684..93
        ? kotor_m15b_spell_array_get_spell_0059b6d0(spells, nullptr, 4)             // 004DE695..A3
        : kotor_m15b_spell_array_get_spell_0059b6d0(spells, nullptr, 0x31);         // 004DE6A5..B3
    const I speed = at<I>(spell, 0xa8);                                             // 004DE6B8 -> [esp+74]
    void* objs[4] = {nullptr, nullptr, nullptr, nullptr};
    // every lookup is gated on the thrower id (EBP), not on the looked-up id (original)
    if (ids[0] != k_object_invalid) objs[0] = game_object(ids[0]);                  // 004DE6C0..E1
    if (ids[0] != k_object_invalid) objs[1] = game_object(ids[1]);                  // 004DE6E7..02
    if (ids[0] != k_object_invalid) objs[2] = game_object(ids[2]);                  // 004DE706..26
    if (ids[0] != k_object_invalid) objs[3] = game_object(ids[3]);                  // 004DE72C..4C
    // path length (x87: ST0 double, [esp+70] float accumulator)
    double st = static_cast<double>(flt(k_float_zero_0073d700));                    // 004DE754 fld 0.0
    float acc = 0.0f;                                                               // 004DE75C [esp+70] = 0
    bool tail = false;
    if (objs[1]) {                                                                  // 004DE75A..64
        st = distance(objs[0], objs[1]);                                            // 004DE76A..AF
        acc = static_cast<float>(st);                                               // 004DE7B7 fst
        if (!objs[2]) {
            tail = true;                                                            // 004DE7BB..BD -> 004DE91D
        } else {
            st = distance(objs[1], objs[2]);                                        // 004DE7C3..0C
            acc = static_cast<float>(st + static_cast<double>(acc));                // 004DE811..1C
            st = distance(objs[0], objs[2]);                                        // 004DE815..61
        }
    }
    if (!tail && objs[2] && objs[3]) {                                              // 004DE869..73
        st = distance(objs[2], objs[3]);                                            // 004DE879..C0
        acc = static_cast<float>(st + static_cast<double>(acc));                    // 004DE8C5..D0
        st = distance(objs[0], objs[3]);                                            // 004DE8C9..15
    }
    st = st + static_cast<double>(acc);                                             // 004DE91D fadd [esp+70]
    U delay_sum = 0;                                                                // 004DE932 [esp+70] = 0
    const float travel = static_cast<float>(st / static_cast<double>(speed) * static_cast<double>(flt(k_float_2000_007469e4))); // 004DE92E..40
    at<float>(creature, 0xab4) = travel;                                            // 004DE944
    void* ai = recovered_server_ai_master_004aed80(server_app(), nullptr);          // 004DE94A..59 -> [esp+74]
    if (!objs[0]) return 0;                                                         // 004DE957..5D
    for (U i = 1; i <= 4;) {                                                        // 004DE963..66 jg
        // EBP: ids[i] == invalid -> 0, else (i > 3 ? 0 : i)
        const U t = (i <= 3 && ids[i] != k_object_invalid) ? i : 0;                 // 004DE96C..8E
        void* prev = objs[i - 1];                                                   // 004DE975
        void* target_object = objs[t];                                              // 004DE992
        if (prev && vcall<void*>(prev, 0x10) && target_object && vcall<void*>(target_object, 0x10)) { // 004DE990..BA
            const U caller = ids[i - 1];
            void* e1 = new_effect();                                                // 004DE9C0..E4
            at<U16>(e1, 8) = 0x1e;                                                  // 004DE9EC
            set_duration_kind(e1, k_duration_temporary_007473c0);                   // 004DE9E6..0F
            set_int(e1, 0, 0x1770);                                                 // 004DE9F2..13
            set_int(e1, 2, 0);                                                      // 004DEA18..1E
            recovered_effect_set_object_id_005036f0(e1, nullptr, 0, caller);        // 004DEA23..2B
            recovered_effect_set_object_id_005036f0(e1, nullptr, 1, at<U>(creature, 4)); // 004DEA30..3C
            void* to = vcall<void*>(target_object, 0x10);                           // 004DEA41..45
            void* from = vcall<void*>(objs[i - 1], 0x10);                           // 004DEA4C..56
            Vector d;
            kotor_cdecl_00423bc0(&d, reinterpret_cast<const Vector*>(static_cast<U8*>(from) + 0x90),
                                 reinterpret_cast<const Vector*>(static_cast<U8*>(to) + 0x90)); // 004DEA59..6F
            const U delay = w2::ftol2_006fae8c(kotor_cdecl_004ab100(&d) / static_cast<double>(travel)
                                               * static_cast<double>(flt(k_float_2000_007469e4))); // 004DEA74..87
            const U target = ids[t];                                                // 004DEAA0 -> [esp+24]
            recovered_event_enqueue_004b08d0(ai, nullptr, 0, delay_sum, caller, target,
                                             k_event_apply_effect_007441c8, e1);    // 004DEA8C..AD
            if (t != 0) {                                                           // 004DEAB2..B4
                void* e2 = new_effect();                                            // 004DEABA..E4
                at<U16>(e2, 8) = 0x26;                                              // 004DEAF0
                set_duration_kind(e2, k_duration_temporary_007473c0);               // 004DEAEA..0D
                recovered_effect_creator_00503a00(e2, nullptr, ids[0]);             // 004DEAE6..11
                recovered_effect_resize_00503630(e2, nullptr, 0x15);                // 004DEB16..1A
                for (U k = 0; k < 0xf; ++k) set_int(e2, k, 0xffffffffu);            // 004DEB1F..2F
                const U8 level = recovered_stats_level_005a5fd0(at<void*>(creature, 0xa74), nullptr, 0); // 004DEB31..44
                const I lvl = level;                                                // [esp+24]
                const U count = w2::ftol2_006fae8c(static_cast<double>(lvl) * static_cast<double>(flt(k_float_half_0073e9ac))); // 004DEB4B..55
                U damage = static_cast<U16>(recovered_rules_roll_dice_00550b60(rules(), nullptr, count, 6)); // 004DEB5A..6A
                if (as_creature(target_object)) {                                   // 004DEB66..72
                    void* tc = as_creature(target_object);                          // 004DEB74..78
                    const U filtered = vcall<U>(tc, 0xa8, creature, damage, 0x1000u, 0u, 0u, 1u); // 004DEB7B..92
                    damage = vcall<U>(tc, 0xac, creature, filtered, 0x1000u, 0u, 0u);  // 004DEB98..AD
                }
                set_int(e2, 0xc, damage);                                           // 004DEBAF..B4
                set_int(e2, 0xe, damage);                                           // 004DEBB9..BE
                set_int(e2, 0x10, 0);                                               // 004DEBC3..C9
                set_int(e2, 0x13, 1);                                               // 004DEBCE..D4
                set_int(e2, 0x14, 1);                                               // 004DEBD9..DF
                void* e3 = new_effect();                                            // 004DEBE4..0E
                at<U16>(e3, 8) = 0x1e;                                              // 004DEC16
                set_duration_kind(e3, k_duration_instant_007473b8);                 // 004DEC10..39
                set_int(e3, 0, 0x1771);                                             // 004DEC1C..3D
                set_int(e3, 2, 0);                                                  // 004DEC42..48
                recovered_effect_set_object_id_005036f0(e3, nullptr, 0, at<U>(creature, 4)); // 004DEC4D..59
                const U when = delay + delay_sum;                                   // 004DEC73
                recovered_event_enqueue_004b08d0(ai, nullptr, 0, when, caller, target,
                                                 k_event_apply_effect_007441c8, e2); // 004DEC5E..81
                recovered_event_enqueue_004b08d0(ai, nullptr, 0, when, caller, target,
                                                 k_event_apply_effect_007441c8, e3); // 004DEC86..96
            }
            delay_sum += delay;                                                     // 004DEC9B..A9
        }
        // 004DECAD..B8: continue while the dword at [esp+i*4+40] is nonzero (i = 4 reads the ids[0] slot)
        const bool more = i < 4 ? objs[i] != nullptr : ids[0] != 0;
        ++i;
        if (!more) break;
    }
    return 0;                                                                       // 004DECC5
}

// 004DECE0 OnRemoveLightsaberThrow: RET 8, EAX 1.
U __fastcall kotor_fastcall_004dece0(void*, void*, void* object, void*) {
    void* creature = as_creature(object);
    if (creature) at<U>(creature, 0xa04) = 0;                                       // 004DECED
    return 1;
}

// 004DED00 OnApplyForceResisted: RET 0Ch, EAX 1.
U __fastcall kotor_fastcall_004ded00(void*, void*, void* object, void* effect, U load_game) {
    void* e = new_effect();                                                         // 004DED17..41
    if (e) {                                                                        // 004DED43..4D
        at<U16>(e, 8) = 0x1e;                                                       // 004DED55
        set_duration_kind(e, k_duration_instant_007473b8);                          // 004DED4F..6C
        recovered_effect_creator_00503a00(e, nullptr, at<U>(object, 4));            // 004DED70..76
        set_int(e, 0, 0xfc5);                                                       // 004DED7B..84
        recovered_effect_set_object_id_005036f0(e, nullptr, 0, kotor_fastcall_005036e0(effect, nullptr, 0)); // 004DED89..99
        apply(object, e, load_game, 0);                                             // 004DED9E..A8
        vcall<void>(object, 0x7c, 0x27a1u);                                         // 004DEDAD..B6
    }
    return 1;
}

// 004DEDE0 OnApplyForceFizzle: RET 0Ch, EAX 1.
U __fastcall kotor_fastcall_004dede0(void*, void*, void* object, void*, U load_game) {
    void* e = new_effect();                                                         // 004DEDF7..21
    if (e) {                                                                        // 004DEE23..2D
        at<U16>(e, 8) = 0x1e;                                                       // 004DEE36
        set_duration_kind(e, k_duration_instant_007473b8);                          // 004DEE2F..51
        set_int(e, 0, 0xfc4);                                                       // 004DEE3C..55
        recovered_effect_creator_00503a00(e, nullptr, at<U>(object, 4));            // 004DEE5A..64
        apply(object, e, load_game, 0);                                             // 004DEE69..73
    }
    return 1;
}

// 004DEE90 OnApplyPureGoodPowers: RET 0Ch, EAX 1 (no creature / stats) or 0.
// The class index lives in the byte of the object argument slot; the stats class count (+89) is re-read.
U __fastcall kotor_fastcall_004dee90(void*, void*, void* object, void* effect, U) {
    void* creature = as_creature(object);                                           // 004DEEA6..B0
    if (!creature) return 1;                                                        // 004DEEB4..CA
    void* stats = at<void*>(creature, 0xa74);                                       // 004DEECE
    if (!stats) return 1;                                                           // 004DEED4..ED
    void* client = kotor_fastcall_004ae910(server_app(), nullptr, at<U>(creature, 4)); // 004DEEF0..FE
    if (client && at<U>(client, 0x38) == at<U>(creature, 4) && at<U8>(stats, 0x89) != 0) { // 004DEF03..24
        U8 i = 0;                                                                   // 004DEF1F
        do {
            const U8 cls = static_cast<U8>(kt_get_class_005a4e90(stats, nullptr, i)); // 004DEF30..37
            if (cls == k_class_b_0074b31d) {                                        // 004DEF3C..42
                const I before = static_cast<I16>(w1x_creature_max_fp_004fd490(creature, nullptr)); // 004DEF44..50
                void* e = new_effect_copy(effect);                                  // 004DEF4B..75
                at<U16>(e, 8) = 0x24;                                               // 004DEF85
                set_int(e, 0, 5);                                                   // 004DEF77..8B
                set_int(e, 1, 3);                                                   // 004DEF90..96
                apply(creature, e, 0, 0);                                           // 004DEF9B..A2
                const I after = static_cast<I16>(w1x_creature_max_fp_004fd490(creature, nullptr)); // 004DEFA7..AE
                at<U16>(stats, 0x124) = static_cast<U16>(at<U16>(stats, 0x124) + static_cast<U16>(after - before)); // 004DEFB1..B3
            } else if (cls == k_class_a_0074b31c) {                                 // 004DEFBF..C5
                void* e = new_effect_copy(effect);                                  // 004DEFC7..F4
                at<U16>(e, 8) = 0x24;                                               // 004DF004
                set_int(e, 0, 0);                                                   // 004DEFF6..0A
                set_int(e, 1, 3);                                                   // 004DF00F..15
                apply(creature, e, 0, 0);                                           // 004DF01A..21
            } else if (cls == k_class_c_0074b31e) {                                 // 004DF02B..31
                const U before = vcall<U>(creature, 0x98, 0u);                      // 004DF037..48 (full EAX)
                void* e = new_effect_copy(effect);                                  // 004DF043..72
                at<U16>(e, 8) = 0x24;                                               // 004DF082
                set_int(e, 0, 2);                                                   // 004DF074..88
                set_int(e, 1, 3);                                                   // 004DF08D..93
                apply(creature, e, 0, 0);                                           // 004DF098..9F
                const U after = static_cast<U16>(vcall<U>(creature, 0x98, 0u));     // 004DF0A4..B2 (movzx-like: DI = AX)
                const U gain = after - before;                                      // 004DF0BB
                const I current = static_cast<I16>(static_cast<U16>(vcall<U>(creature, 0x9c, 1u))); // 004DF0B5..C3
                w1x_object_set_hp_004cc280(creature, nullptr,
                                           static_cast<U>(current + static_cast<I16>(static_cast<U16>(gain)))); // 004DF0C6..CE
            }
            ++i;                                                                    // 004DF0D3..DD
        } while (i < at<U8>(stats, 0x89));                                          // 004DF0D7..E5 jb
    }
    void* e = new_effect_copy(effect);                                              // 004DF0EC..19
    at<U16>(e, 8) = 0x43;                                                           // 004DF129
    set_int(e, 0, 0x3c);                                                            // 004DF11B..2F
    apply(creature, e, 0, 0);                                                       // 004DF134..3B
    return 0;                                                                       // 004DF146
}

// 004DF160 OnApplyPureEvilPowers: RET 0Ch, EAX 1 (no creature / stats) or 0.
U __fastcall kotor_fastcall_004df160(void*, void*, void* object, void* effect, U) {
    void* creature = as_creature(object);                                           // 004DF166..82
    if (!creature) return 1;                                                        // 004DF184..9C
    void* stats = at<void*>(creature, 0xa74);                                       // 004DF1A0 -> [esp+8]
    if (!stats) return 1;                                                           // 004DF1A6..C3
    void* client = kotor_fastcall_004ae910(server_app(), nullptr, at<U>(creature, 4)); // 004DF1C6..D4
    if (client && at<U>(client, 0x38) == at<U>(creature, 4) && at<U8>(stats, 0x89) != 0) { // 004DF1DC..FD
        U8 i = 0;                                                                   // 004DF1F8
        do {
            const U8 cls = static_cast<U8>(kt_get_class_005a4e90(stats, nullptr, i)); // 004DF210..17
            if (cls == k_class_b_0074b31d) {                                        // 004DF21C..22
                at<U16>(stats, 0x124) = static_cast<U16>(at<U16>(stats, 0x124) + 0x32); // 004DF224
            } else if (cls == k_class_a_0074b31c) {                                 // 004DF231..37
                void* e = new_effect_copy(effect);                                  // 004DF23D..64
                at<U16>(e, 8) = 0xd;                                                // 004DF270 (BX = 0Dh)
                set_int(e, 0, 8);                                                   // 004DF266..74
                set_int(e, 1, 3);                                                   // 004DF279..7F
                set_int(e, 2, at<U8>(rules(), 0xaa));                               // 004DF284..96
                set_int(e, 5, 1);                                                   // 004DF29B..A1
                apply(creature, e, 0, 0);                                           // 004DF2A6..AD
                void* e2 = new_effect_copy(effect);                                 // 004DF2B2..DF
                at<U16>(e2, 8) = 0xd;                                               // 004DF2EB
                set_int(e2, 0, 8);                                                  // 004DF2E1..EF
                set_int(e2, 1, 3);                                                  // 004DF2F4..FA
                set_int(e2, 2, at<U8>(rules(), 0xaa));                              // 004DF2FF..11
                set_int(e2, 5, 2);                                                  // 004DF316..1A -> 004DF378
                apply(creature, e2, 0, 0);                                          // 004DF37F..86
            } else if (cls == k_class_c_0074b31e) {                                 // 004DF31C..22
                void* e = new_effect_copy(effect);                                  // 004DF324..51
                at<U16>(e, 8) = 0x16;                                               // 004DF35D
                set_int(e, 0, 2);                                                   // 004DF353..63
                set_int(e, 1, at<U8>(rules(), 0xaa));                               // 004DF368..7A
                apply(creature, e, 0, 0);                                           // 004DF37F..86
            }
            ++i;                                                                    // 004DF38F..9D
        } while (i < at<U8>(stats, 0x89));                                          // 004DF393..A1 jb
    }
    void* e = new_effect_copy(effect);                                              // 004DF3A8..D5
    at<U16>(e, 8) = 0x43;                                                           // 004DF3E1
    set_int(e, 0, 0x3d);                                                            // 004DF3D7..E7
    apply(creature, e, 0, 0);                                                       // 004DF3EC..F3
    return 0;                                                                       // 004DF3FE
}

// 004DF410 OnRemoveStatChange1: RET 8, EAX 1.
U __fastcall kotor_fastcall_004df410(void*, void*, void*, void*) {
    return 1;
}

// 004DF540 OnApplyForceShield: RET 0Ch, EAX 0.
U __fastcall kotor_fastcall_004df540(void*, void*, void* object, void* effect, U) {
    if (!object) return 0;                                                          // 004DF564..68
    if (!as_creature(object)) return 0;                                             // 004DF56E..77
    if (!effect) return 0;                                                          // 004DF57D..80
    void* creature = as_creature(object);                                           // 004DF586..8D
    const U16 appearance = at<U16>(creature, 0xa60);                                // 004DF58F..98 [esp+14]
    if (recovered_creature_client_creature_004efb20(creature, nullptr) &&           // 004DF596..A4
        kotor_animation_sound_damage_effect_00616890(recovered_creature_client_creature_004efb20(creature, nullptr), nullptr)) { // 004DF5A6..B6
        // the existing force-shield effect (type 6Bh) in the type-sorted list; when absent the null entry is
        // dereferenced exactly as the original does (004DF5F1..F4 with EDI = 0).
        void* found = nullptr;
        const I count = at<I>(creature, 0x128);                                     // 004DF5B8
        if (count > 0) {
            void** list = at<void**>(creature, 0x124);                              // 004DF5C4
            for (I n = 0; n < count; ++n) {
                const U16 type = at<U16>(list[n], 8);                               // 004DF5D0..D2
                if (type == 0x6b) { found = at<void**>(creature, 0x124)[n]; break; } // 004DF5D6..EE
                if (type > 0x6b) break;                                             // 004DF5DC ja
            }
        }
        volatile U* id = static_cast<volatile U*>(found);
        const U hi = id[1];                                                         // 004DF5F1
        const U lo = id[0];                                                         // 004DF5F4
        recovered_object_remove_effect_004d06c0(creature, nullptr, lo, hi);         // 004DF5F6..FA
    }
    Str row;
    kotor_fastcall_005e5bc0(&row, nullptr, static_cast<I>(get_int(effect, 0)));     // 004DF601..0F
    void* table = at<void*>(at<void*>(rules(), 0xb8), 0x10c);                       // 004DF614..20
    Str column;
    frame_LogStringCtor_005e5a90(&column, nullptr, k_visual_effect_def_00746aac);   // 004DF626..3B
    I matched_value = 0;                                                            // [esp+10]
    I visual = 0;                                                                   // [esp+18]
    const char* const appearance_columns[4] = {k_appearance_01_00746a9c, k_appearance_02_00746a7c,
                                               k_appearance_03_00746a5c, k_appearance_04_00746a3c};
    const char* const visual_columns[4] = {k_visual_effect_01_00746a8c, k_visual_effect_02_00746a6c,
                                           k_visual_effect_03_00746a4c, k_visual_effect_04_00746a2c};
    for (U k = 0; k < 4; ++k) {                                                     // 004DF645..7CE (unrolled x4)
        bool match;
        {
            Str key;
            frame_LogStringCtor_005e5a90(&key, nullptr, appearance_columns[k]);
            match = kotor_fastcall_00414110(table, nullptr, &row, &key, &matched_value) != 0 &&
                    static_cast<I>(appearance) == matched_value;                    // movzx word / cmp / sete
        }                                                                           // ~CExoString
        if (match) { frame_LogAssign_005e5140(&column, nullptr, visual_columns[k]); break; } // 004DF7D5..D9
    }
    kotor_fastcall_00414110(table, nullptr, &row, &column, &visual);                // 004DF7DE..EF (result ignored)
    I damage_flags = 0, vulner_flags = 0, resistance = 0, amount = 0, permanent = 0; // 004DF7FD..0D
    struct Entry { const char* name; I* out; };
    const Entry entries[5] = {{k_damage_flags_00746a20, &damage_flags}, {k_vulner_flags_00746a14, &vulner_flags},
                              {k_resistance_00746a08, &resistance}, {k_amount_00746a00, &amount},
                              {k_permanent_007469f4, &permanent}};
    for (const Entry& entry : entries) {                                            // 004DF7F4..911
        Str key;
        frame_LogStringCtor_005e5a90(&key, nullptr, entry.name);
        kotor_fastcall_00414110(table, nullptr, &row, &key, entry.out);
    }
    (void)permanent;                                                                // read from the 2DA, never used
    void* visual_effect = new_effect_copy(effect);                                  // 004DF916..3B
    recovered_effect_creator_00503a00(visual_effect, nullptr, at<U>(object, 4));    // 004DF93D..4A
    at<U16>(visual_effect, 8) = 0x1e;                                               // 004DF94F
    set_int(visual_effect, 0, static_cast<U>(visual));                              // 004DF955..5E
    void* shield = new_effect_copy(effect);                                         // 004DF963..8C
    recovered_effect_creator_00503a00(shield, nullptr, at<U>(object, 4));           // 004DF98E..9B
    at<U16>(shield, 8) = 2;                                                         // 004DF9A6
    at<U16>(shield, 0xa) = static_cast<U16>((at<U16>(shield, 0xa) & 0xffefu) | 8u); // 004DF9A0..B4
    set_int(shield, 0, static_cast<U>(damage_flags));                               // 004DF9B8..C1
    set_int(shield, 1, static_cast<U>(resistance));                                 // 004DF9C6..CE
    set_int(shield, 2, static_cast<U>(amount));                                     // 004DF9D3..DC
    set_int(shield, 3, static_cast<U>(vulner_flags));                               // 004DF9E1..EA
    apply(object, visual_effect, 0, 0);                                             // 004DF9EF..F9
    apply(object, shield, 0, 0);                                                    // 004DF9FE..05
    return 0;                                                                       // 004DFA0A..2F (~column, ~row)
}

}  // extern "C"
