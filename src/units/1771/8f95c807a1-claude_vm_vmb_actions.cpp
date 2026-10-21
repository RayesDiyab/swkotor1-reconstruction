// Lane vmB, file "actions": effect constructors and action-queue NWScript engine-command handlers of package vmB
// (CSWVirtualMachineCommands::ExecuteCommand*), reconstructed from the original swkotor.exe (read-only, capstone).
// Every handler: original int __thiscall (int routine, int argc), RET 8 (verified) ->
//   extern "C" int __fastcall kotor_fastcall_<addr>(self, edx, routine, argc) @16.
// Addresses in comments are original instruction ranges.  Conventions: claude_vm_vmB_common.hpp.
// .rdata literals: [0074667C] 0 invalid effect, [00746738] 2Fh invisibility, [007467F8] 60h heal force points,
// [00746780] 41h spell level absorption, [007467E0] 5Ah force drain, [007467AC] 4Ch miss chance,
// [00746990] 0 miss-chance vs-type, [00746794] 46h see invisible, [0074675C] 38h skill decrease,
// [00746B10] 40.0f move-away default range, [00742EF8] 57.29578f (deg/rad), [00746644] 7F000000h OBJECT_INVALID.
#include "54db204e3b-claude_vm_vmb_common.hpp"
#include <initializer_list>

extern "C" {
// ---- existing providers (c31 map; object in brackets) ----
unsigned __fastcall recovered_object_add_action_004cea20(void* object, void*, unsigned type, unsigned group,
    unsigned t0, void* v0, unsigned t1, void* v1, unsigned t2, void* v2, unsigned t3, void* v3, unsigned t4, void* v4,
    unsigned t5, void* v5, unsigned t6, void* v6, unsigned t7, void* v7, unsigned t8, void* v8, unsigned t9, void* v9,
    unsigned t10, void* v10, unsigned t11, void* v11, unsigned t12, void* v12);           // CSWSObject::AddAction RET 70h [w1_native_b7_0056cbf0]
U __fastcall recovered_creature_add_move_to_point_action_004f8b60(void* creature, void*, U group, const void* point, U area_id,
    U target_id, U run, U range, U a6, U a7, U a8, U a9, U a10, U a11, U a12, const void* a13, U a14, U a15, U a16); // RET 44h [vm_actions_t5]
Vector* __fastcall kotor_fastcall_004bdf80(void* area, void*, Vector* out, const Vector* from, const Vector* target,
    float dist, void* pf);                                                               // CSWSArea::ComputeAwayVector RET 14h [claude_w5_movement_actions]
U __fastcall kotor_fastcall_004f0840(void* creature, void*, U item_id, Vector* position, U flag); // AddDropItemActions RET 0Ch [claude_p2s_a_inv]
void* __fastcall frame_R4_005ed690(void* client, void*);                                 // CClientExoApp::GetInGameGui [0457-successor]
void __fastcall kotor_fastcall_004d1d50(void* action, void*);                            // CSWSCombatRoundAction::ClearData [round_queue]
void __fastcall kotor_fastcall_004d3660(void* round, void*, void* action, I index);      // CSWSCombatRound::AddAction RET 8 [round_queue]
void* __fastcall kotor_fastcall_004cc630(void* object, void*, U type);                   // CSWSObject::GetNodeById RET 4 [producer_helpers]
void __fastcall w5_object_clear_all_actions_004ccd80(void* object, void*, U clear_combat); // CSWSObject::ClearAllActions RET 4 [1206-07]
void __fastcall recovered_unstealth_party_00563c60(void* party, void*, U flag);         // CSWPartyTable::UnstealthParty RET 4 [w2_party_native]
extern void* kotor_cell_rules_007a3a28;                                                  // g_pRules 007A3A28 [0421]
// ---- reconstructed in this file (section at the end) ----
U __fastcall kotor_fastcall_004d3b30(void* round, void*, U target, Vector position, U speed);
U __fastcall kotor_fastcall_004fa050(void* creature, void*, U target, const Vector* position, U speed);
void __fastcall kotor_fastcall_00506b90(void* area, void*, U mode);
}
namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void* server, void*); } // CServerExoApp::GetPartyTable [0575]

using namespace vmb;

namespace {
inline U rules_byte(U offset) { return at<U8>(kotor_cell_rules_007a3a28, offset); }
struct Pair { U type; void* value; };
// CSWSObject::AddAction(type, group, 13 (parameter type, parameter pointer) pairs); unused pairs are (0, 0).
U add_action(void* object, U type, U group, std::initializer_list<Pair> list) {
    Pair p[13] = {};
    U i = 0;
    for (const Pair& x : list) p[i++] = x;
    return recovered_object_add_action_004cea20(object, nullptr, type, group, p[0].type, p[0].value, p[1].type, p[1].value,
        p[2].type, p[2].value, p[3].type, p[3].value, p[4].type, p[4].value, p[5].type, p[5].value, p[6].type, p[6].value,
        p[7].type, p[7].value, p[8].type, p[8].value, p[9].type, p[9].value, p[10].type, p[10].value, p[11].type, p[11].value,
        p[12].type, p[12].value);
}
}  // namespace

extern "C" {

// ===================================================================================================== 00532E40
// EffectInvisibility(int nInvisibilityType) [routine 457].  Types 1, 2, 4 are valid; anything else yields the
// invalid effect (no creator, flags untouched).  Integer 1 = rules byte +AAh.
int __fastcall kotor_fastcall_00532e40(void* self, void*, int, unsigned) {
    I kind = 0;
    if (!pop_int(&kind)) return k_pop_failed;                                            // 00532E59..83
    void* e = new_effect();                                                              // 00532E87..B1
    if (kind == 1 || kind == 2 || kind == 4) {                                           // 00532EB3..CC
        recovered_effect_creator_00503a00(e, nullptr, self_id(self));                    // 00532EDB..E1
        set_effect_type(e, 0x2f);                                                        // 00532EE6..F2 [00746738]
        set_subtype_magical(e);                                                          // 00532EEC..FF
        recovered_effect_integer_005036a0(e, nullptr, 0, static_cast<U>(kind));          // 00532F03..0C
        recovered_effect_integer_005036a0(e, nullptr, 1, rules_byte(0xaa));              // 00532F11..22
    } else {
        set_effect_type(e, 0);                                                           // 00532ECE..D5 [0074667C]
    }
    return push_effect(e);                                                               // 00532F27..73
}

// ===================================================================================================== 00535020
// EffectHealForcePoints(int nHeal) [routine 373]: type 60h, integer 0 = amount (no validation, no creator).
int __fastcall kotor_fastcall_00535020(void*, void*, int, unsigned) {
    I amount = 0;
    if (!pop_int(&amount)) return k_pop_failed;                                          // 0053503E..5F
    void* e = new_effect();                                                              // 00535062..8D
    set_effect_type(e, 0x60);                                                            // 0053508F..96 [007467F8]
    recovered_effect_integer_005036a0(e, nullptr, 0, static_cast<U>(amount));            // 0053509A..AB
    return push_effect(e);                                                               // 005350B0..FA
}

// ===================================================================================================== 00533FA0
// EffectSpellLevelAbsorption(int nMaxSpellLevelAbsorbed, int nTotalSpellLevelsAbsorbed = 0, int nSpellSchool = 0)
// [routine 472].  All three are popped unconditionally.  Valid when -1 <= max <= 9; integer 3 = (total == 0) is
// set on both paths.
int __fastcall kotor_fastcall_00533fa0(void* self, void*, int, unsigned) {
    I max_level = 0, total = 0, school = 0;
    if (!pop_int(&max_level)) return k_pop_failed;                                       // 00533FB9..CD
    if (!pop_int(&total)) return k_pop_failed;                                           // 00533FCF..E1
    if (!pop_int(&school)) return k_pop_failed;                                          // 00533FE3..0B
    void* e = new_effect();                                                              // 0053400E..39
    if (max_level <= 9 && max_level >= -1) {                                             // 0053403B..4F (signed)
        set_effect_type(e, 0x41);                                                        // 00534051..5D [00746780]
        set_subtype_magical(e);                                                          // 00534057..6A
        recovered_effect_creator_00503a00(e, nullptr, self_id(self));                    // 0053406E..74
        recovered_effect_integer_005036a0(e, nullptr, 0, static_cast<U>(max_level));     // 00534079..82
        recovered_effect_integer_005036a0(e, nullptr, 1, static_cast<U>(total));         // 00534087..90
        recovered_effect_integer_005036a0(e, nullptr, 2, static_cast<U>(school));        // 00534095..9E
    } else {
        set_effect_type(e, 0);                                                           // 005340A5..AB [0074667C]
    }
    recovered_effect_integer_005036a0(e, nullptr, 3, total == 0 ? 1u : 0u);              // 005340AF..C1
    return push_effect(e);                                                               // 005340C6..12
}

// ===================================================================================================== 00532920
// EffectForceDrain(int nDamage) [routine 675]: magical; amount > 0 -> type 5Ah with one integer, else invalid.
// Creator = OBJECT_SELF when it resolves to any game object.
int __fastcall kotor_fastcall_00532920(void* self, void*, int, unsigned) {
    I amount = 0;
    if (!pop_int(&amount)) return k_pop_failed;                                          // 00532939..63
    void* e = new_effect();                                                              // 00532966..91
    set_subtype_magical(e);                                                              // 00532993..AA
    if (amount > 0) {                                                                    // 005329AE..B4 (jle)
        set_effect_type(e, 0x5a);                                                        // 005329B6..C1 [007467E0]
        recovered_effect_resize_00503630(e, nullptr, 1);                                 // 005329BD..C5
        recovered_effect_integer_005036a0(e, nullptr, 0, static_cast<U>(amount));        // 005329CA..D3
    } else {
        set_effect_type(e, 0);                                                           // 005329DA..E1 [0074667C]
    }
    void* object = nullptr;
    if (lookup(self_id(self), &object) && object != nullptr)                             // 005329E5..11
        recovered_effect_creator_00503a00(e, nullptr, self_id(self));                    // 00532A13..19
    return push_effect(e);                                                               // 00532A1E..6A
}

// ===================================================================================================== 00533090
// EffectMissChance(int nPercentage) [routine 477]: 1..100 -> type 4Ch, integers (percentage, 0); else invalid.
int __fastcall kotor_fastcall_00533090(void* self, void*, int, unsigned) {
    I percent = 0;
    if (!pop_int(&percent)) return k_pop_failed;                                         // 005330A9..D3
    void* e = new_effect();                                                              // 005330D6..01
    if (percent >= 1 && percent <= 0x64) {                                               // 00533103..17 (signed)
        set_subtype_magical(e);                                                          // 00533119..28
        recovered_effect_creator_00503a00(e, nullptr, self_id(self));                    // 0053312C..32
        set_effect_type(e, 0x4c);                                                        // 00533137..3D [007467AC]
        recovered_effect_integer_005036a0(e, nullptr, 0, static_cast<U>(percent));       // 00533141..4A
        recovered_effect_integer_005036a0(e, nullptr, 1, 0);                             // 0053314F..5A [00746990]
    } else {
        set_effect_type(e, 0);                                                           // 00533161..67 [0074667C]
    }
    return push_effect(e);                                                               // 0053316B..B7
}

// ===================================================================================================== 00533AC0
// EffectSeeInvisible() [routine 466]: type 46h, creator OBJECT_SELF, magical.
int __fastcall kotor_fastcall_00533ac0(void* self, void*, int, unsigned) {
    void* e = new_effect();                                                              // 00533AD8..04
    set_effect_type(e, 0x46);                                                            // 00533B06..0C [00746794]
    recovered_effect_creator_00503a00(e, nullptr, self_id(self));                        // 00533B10..1E
    set_subtype_magical(e);                                                              // 00533B23..35
    return push_effect(e);                                                               // 00533B39..82
}

// ===================================================================================================== 00533B90
// EffectSkillDecrease(int nSkill, int nValue) [routine 453].  Valid when 0 < skill < rules byte +ABh (skill count)
// or skill == FFh; integers (skill, value, rules byte +AAh).
// Quirks kept: skill 0 is rejected (jle); on a successful push the effect is NOT deleted (leak; the original
// returns through neg/sbb without the ~CGameEffect/free tail its siblings have).
int __fastcall kotor_fastcall_00533b90(void* self, void*, int, unsigned) {
    I skill = 0, value = 0;
    if (!pop_int(&skill)) return k_pop_failed;                                           // 00533BA9..D3
    if (!pop_int(&value)) return k_pop_failed;                                           // 00533BD6..E8
    void* e = new_effect();                                                              // 00533BEA..15
    set_effect_type(e, 0x38);                                                            // 00533C17..24 [0074675C]
    set_subtype_magical(e);                                                              // 00533C1E..38
    if ((skill > 0 && skill < static_cast<I>(rules_byte(0xab))) || skill == 0xff) {      // 00533C3C..5A
        recovered_effect_integer_005036a0(e, nullptr, 0, static_cast<U>(skill));         // 00533C5C..61
        recovered_effect_integer_005036a0(e, nullptr, 1, static_cast<U>(value));         // 00533C66..6F
        recovered_effect_integer_005036a0(e, nullptr, 2, rules_byte(0xaa));              // 00533C74..86
    } else {
        set_effect_type(e, 0);                                                           // 00533C8D..93 [0074667C]
    }
    void* object = nullptr;
    if (lookup(self_id(self), &object) && object != nullptr)                             // 00533C97..C3
        recovered_effect_creator_00503a00(e, nullptr, self_id(self));                    // 00533CC5..CB
    return push_status(push_engine_raw(0, e));                                           // 00533CD0..FC (effect leaked)
}

// ===================================================================================================== 0052D090
// ActionMoveAwayFromLocation(location lMoveAwayFrom, int bRun = FALSE, float fMoveAwayRange = 40.0f) [routine 360]
// When OBJECT_SELF is a creature (+E8h == 1) in an area and within range of the location, queues
// AddMoveToPointAction to the computed away point and an action 2Ch (x, y, z, run, range, 10).
// Quirks kept: the popped location leaks on success and when OBJECT_SELF is already out of range (or the
// distance compare is unordered); the argc argument slot is reused as the integer 10 parameter.
int __fastcall kotor_fastcall_0052d090(void* self, void*, int, unsigned argc) {
    float range = flt(0x42200000u);                                                      // 0052D093 [00746B10] 40.0f [esp+C]
    I run = 0;                                                                           // 0052D0A8 [esp+14]
    void* location = nullptr;                                                            // [esp+10]
    if (!pop_engine(2, &location)) return k_pop_failed;                                  // 0052D09B..C6
    const auto fail = [&]() -> int {                                                     // 0052D0E7..109
        if (location != nullptr) { location_dtor_004015a0(location); live_free(location); }
        return k_pop_failed;
    };
    if (static_cast<I>(argc) >= 2 && !pop_int(&run)) return fail();                      // 0052D0CA..E5 (jl signed)
    if (static_cast<I>(argc) >= 3 && !pop_float(&range)) return fail();                  // 0052D10C..23
    void* object = nullptr;                                                              // [esp+8]
    void* area = nullptr;
    if (self_valid(self) == 1 && lookup(self_id(self), &object) && as_creature(object) != nullptr &&
        at<U>(as_creature(object), 0xe8) == 1) {                                         // 0052D125..7A
        Vector target;                                                                   // [esp+24]
        std::memcpy(&target, location, 12);                                              // 0052D180..94
        area = recovered_object_area_004cb120(as_creature(object), nullptr);             // 0052D198..AC
        unsigned char resref[0x10];                                                      // [esp+3C] (unused CResRef)
        c2da_default_ref_00405ed0(resref, nullptr);                                      // 0052D1A8..AE
        if (area != nullptr) {                                                           // 0052D1B3..B5
            Vector d;                                                                    // [esp+18]
            kotor_cdecl_00423bc0(&d, &at<Vector>(object, 0x90), &target);                // 0052D1BB..D0
            // 0052D1D5..FC: fld z; fmul z; fld y; fmul y; faddp; fld x; fmul x; faddp; fld r; fmul r; fcompp
            const double sum = (static_cast<double>(d.z) * d.z + static_cast<double>(d.y) * d.y) +
                               static_cast<double>(d.x) * d.x;
            const double r2 = static_cast<double>(range) * range;
            if (!(r2 >= sum)) return 0;                                                  // 0052D1FE..03 (C0: r2 < sum or NaN; location leaked)
            Vector away;                                                                 // [esp+3C]
            kotor_fastcall_004bdf80(area, nullptr, &away, &at<Vector>(object, 0x90), &target, range,
                                    at<void*>(as_creature(object), 0x340));              // 0052D209..33
            float zero[3];                                                               // [esp+18] reused
            zero[0] = 0.0f; zero[1] = 0.0f; zero[2] = 0.0f;                              // 0052D27A..8A
            U ten = 0xa;                                                                 // 0052D26F argc slot := 0Ah
            (void)recovered_creature_add_move_to_point_action_004f8b60(as_creature(object), nullptr, 0xffff, &away,
                at<U>(area, 0x120), k_invalid, static_cast<U>(run), 0, 0, 0, 0, 0, 0, 1, 0, zero, 0, 0, 0); // 0052D238..9E
            float* loc = static_cast<float*>(location);
            (void)add_action(object, 0x2c, 0xfffe, {{2, &loc[0]}, {2, &loc[1]}, {2, &loc[2]}, {1, &run},
                                                     {2, &range}, {1, &ten}});           // 0052D2A3..F5
            return 0;                                                                    // 0052D2FA..301 (location leaked)
        }
    }
    if (location != nullptr) { location_dtor_004015a0(location); live_free(location); }  // 0052D304..19
    return 0;                                                                            // 0052D31C..23
}

// ===================================================================================================== 0052E950
// CutsceneMove(object oTarget, vector vPosition, int nRun) [routine 507] on the OBJECT_SELF creature.
int __fastcall kotor_fastcall_0052e950(void* self, void*, int, unsigned) {
    U target = 0; Vector position = {0.0f, 0.0f, 0.0f}; I run = 0;                       // 0052E961..71
    if (!pop_object(&target)) return k_pop_failed;                                       // 0052E954..80
    if (!pop_vector(&position)) return k_pop_failed;                                     // 0052E982..94
    if (!pop_int(&run)) return k_pop_failed;                                             // 0052E996..B3
    void* creature = creature_by_id(self_id(self));                                      // 0052E9B6..C7
    if (creature != nullptr)                                                             // 0052E9C9
        (void)kotor_fastcall_004fa050(creature, nullptr, target, &position, static_cast<U>(run)); // 0052E9CB..DC
    return 0;                                                                            // 0052E9E1
}

// ===================================================================================================== 0052C6A0
// ActionBarkString(int strRef) [routine 700]: only while the in-game GUI's +B4h is 0; action 3Eh (strref).
int __fastcall kotor_fastcall_0052c6a0(void* self, void*, int, unsigned) {
    I strref = 0;                                                                        // [esp+4]
    if (!pop_int(&strref)) return k_pop_failed;                                          // 0052C6A2..BF
    void* gui = frame_R4_005ed690(client_app(), nullptr);                                // 0052C6C2..CB
    if (at<U>(gui, 0xb4) == 0) {                                                         // 0052C6D0..D8
        void* creature = creature_by_id(self_id(self));                                  // 0052C6DA..E7
        if (creature != nullptr)                                                         // 0052C6EC
            (void)add_action(creature, 0x3e, 0xffff, {{1, &strref}});                    // 0052C6F0..30
    }
    return 0;                                                                            // 0052C735
}

// ===================================================================================================== 00544270
// ActionSpeakStringByStrRef(int nStrRef, int nTalkVolume = TALKVOLUME_TALK) [routine 240]: action 21h
// (strref, mapped volume: 1 -> 0Ah, 2 -> 9, otherwise 8) on an OBJECT_SELF object whose +E8h is set.
int __fastcall kotor_fastcall_00544270(void* self, void*, int, unsigned) {
    I strref = 0, volume = 0;                                                            // [esp+10] / [esp+C]
    if (!pop_int(&strref)) return k_pop_failed;                                          // 00544274..88
    if (!pop_int(&volume)) return k_pop_failed;                                          // 0054428A..A7
    if (self_valid(self) != 1) return 0;                                                 // 005442AA..AE
    void* object = nullptr;                                                              // [esp+8]
    if (!lookup(self_id(self), &object)) return 0;                                       // 005442B4..E0
    U mapped = 8;                                                                        // 005442ED [esp+4]
    if (volume == 1) mapped = 0xa;                                                       // 005442EA..FF
    else if (volume == 2) mapped = 9;                                                    // 00544301..06
    if (as_object(object) == nullptr) return 0;                                          // 0054430E..19
    if (at<U>(as_object(object), 0xe8) == 0) return 0;                                   // 0054431B..2C
    (void)add_action(as_object(object), 0x21, 0xffff, {{1, &strref}, {1, &mapped}});     // 0054432E..7A
    return 0;                                                                            // 0054437F
}

// ===================================================================================================== 00542530
// SetCameraFacing(float fDirection) [routine 45]: action 16h (1, direction in degrees, 0.0, 0.0, 0) on the
// OBJECT_SELF creature whose +E8h is set.
int __fastcall kotor_fastcall_00542530(void* self, void*, int, unsigned) {
    float direction = 0.0f;                                                              // [esp+4]
    if (!pop_float(&direction)) return k_pop_failed;                                     // 00542534..53
    const float k_deg = flt(0x42652ee1u);                                                // [00742EF8] 57.29578f
    direction = static_cast<float>(static_cast<double>(direction) * static_cast<double>(k_deg)); // 00542556..76 fld/fmul/fstp dword
    U one = 1;                                                                           // [esp+18]
    float zero_a = 0.0f, zero_b = 0.0f;                                                  // [esp+14] / [esp+10]
    U zero_c = 0;                                                                        // [esp+C]
    if (self_valid(self) != 1) return 0;                                                 // 0054255A..8A
    void* object = nullptr;                                                              // [esp+8]
    if (!lookup(self_id(self), &object)) return 0;                                       // 00542590..B4
    if (as_creature(object) == nullptr) return 0;                                        // 005425B6..C1
    if (at<U>(as_creature(object), 0xe8) == 0) return 0;                                 // 005425C3..D4
    (void)add_action(as_creature(object), 0x16, 0xffff, {{1, &one}, {2, &direction}, {2, &zero_a}, {2, &zero_b},
                                                          {1, &zero_c}});                // 005425D6..2B
    return 0;                                                                            // 00542630
}

// ===================================================================================================== 00541970
// ActionPutDownItem(object oItem) [routine 35]: AddDropItemActions(item, &position, 0) on the OBJECT_SELF creature.
int __fastcall kotor_fastcall_00541970(void* self, void*, int, unsigned) {
    U item = 0;
    if (!pop_object(&item)) return k_pop_failed;                                         // 00541972..8F
    void* creature = creature_by_id(self_id(self));                                      // 00541992..9F
    if (creature != nullptr)                                                             // 005419A4..A6
        (void)kotor_fastcall_004f0840(creature, nullptr, item, &at<Vector>(creature, 0x90), 0); // 005419A8..B8
    return 0;                                                                            // 005419BD
}

// ===================================================================================================== 00543450
// SetRestrictMode(int nRestrict) [routine 58]: CSWSArea::SetPlayerRestrictMode on the module's current area.
int __fastcall kotor_fastcall_00543450(void*, void*, int, unsigned) {
    I mode = 0;
    if (!pop_int(&mode)) return k_pop_failed;                                            // 00543451..6B
    if (server_get_module_004ae6b0(server_app(), nullptr) != nullptr &&                  // 0054346E..7E
        module_get_area_004c30f0(server_get_module_004ae6b0(server_app(), nullptr), nullptr) != nullptr) // 00543480..97
        kotor_fastcall_00506b90(module_get_area_004c30f0(server_get_module_004ae6b0(server_app(), nullptr), nullptr),
                                nullptr, static_cast<U>(mode));                          // 00543499..B4
    return 0;                                                                            // 005434B9
}

// =====================================================================================================
// Reconstructed callees without a callable provider in the c31 map (exact original ABI).
// =====================================================================================================

// 004D3B30 CSWSCombatRound::AddCutsceneMoveActions(target, Vector position (by value), speed): thiscall RET 14h.
// new(88h) CSWSCombatRoundAction (inline ctor: +38/+3C/+40 = 0, ClearData); a failed allocation writes through
// null exactly like the original.  Kind 0Ch, +14 target, +38 position, +54 speed; AddAction(action, -1).
U __fastcall kotor_fastcall_004d3b30(void* round, void*, U target, Vector position, U speed) {
    void* memory = live_operator_new(0x88);                                              // 004D3B31..41
    unsigned char* action = static_cast<unsigned char*>(memory);
    if (memory != nullptr) {                                                             // 004D3B44..46
        at<U>(action, 0x38) = 0; at<U>(action, 0x3c) = 0; at<U>(action, 0x40) = 0;       // 004D3B48..4E
        kotor_fastcall_004d1d50(action, nullptr);                                        // 004D3B51
    } else {
        action = nullptr;                                                                // 004D3B58
    }
    at<U>(action, 0x54) = speed;                                                         // 004D3B5A..62
    at<U>(action, 0x14) = target;                                                        // 004D3B5E..65
    std::memcpy(action + 0x38, &position, 12);                                           // 004D3B68..85
    *reinterpret_cast<volatile U8*>(action + 0x10) = 0xc;                                // 004D3B7E
    kotor_fastcall_004d3660(round, nullptr, action, -1);                                 // 004D3B7C..88
    return word(action);                                                                 // EAX not used by callers
}

// 004FA050 CSWSCreature::AddCutsceneMoveActions(target, const Vector* position, speed): thiscall RET 0Ch, EAX 1.
// Quirk kept: the speed argument slot is overwritten with 1 and passed as action 3Fh's integer parameter.
U __fastcall kotor_fastcall_004fa050(void* creature, void*, U target, const Vector* position, U speed) {
    (void)kotor_fastcall_004d3b30(at<void*>(creature, 0x9c8), nullptr, target, *position, speed); // 004FA050..7C
    if (at<U>(creature, 0x4e0) == 0 && kotor_fastcall_004ae910(server_app(), nullptr, at<U>(creature, 4)) != nullptr) { // 004FA081..9E
        recovered_creature_compute_ai_state_004f1460(creature, nullptr);                // 004FA0A0..A2
        w5_object_clear_all_actions_004ccd80(creature, nullptr, 1);                      // 004FA0A7..AB
    }
    if (kotor_fastcall_004cc630(creature, nullptr, 0x3f) == nullptr) {                   // 004FA0B0..BB
        speed = 1;                                                                       // 004FA0E5 argument slot := 1
        (void)add_action(creature, 0x3f, 0xffff, {{1, &speed}});                         // 004FA0BD..F0
    }
    return 1;                                                                            // 004FA0F5
}

// 00506B90 CSWSArea::SetPlayerRestrictMode(int mode): thiscall RET 4.  Entering a new nonzero mode unstealths
// the party; +2B0h = mode always.
void __fastcall kotor_fastcall_00506b90(void* area, void*, U mode) {
    if (mode != 0 && at<U>(area, 0x2b0) != mode)                                         // 00506B92..A2
        recovered_unstealth_party_00563c60(lane_config_required::GetPartyTable_004aee70(server_app(), nullptr),
                                           nullptr, 1);                                  // 00506BA4..B5
    at<U>(area, 0x2b0) = mode;                                                           // 00506BBA
}

}  // extern "C"
