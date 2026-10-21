// Lane vmC, part C (creature / faction queries): NWScript engine-command handlers
// CSWVirtualMachineCommands::ExecuteCommand*, reconstructed from the original swkotor.exe (read-only, capstone
// linear sweep).  Addresses in comments are original instruction ranges.
//  * Every handler: original int __thiscall (int routine, int argc), RET 8 (verified) ->
//    extern "C" int __fastcall kotor_fastcall_<addr>(void* self, void*, int routine, unsigned argc)  (@16).
//  * Return literals: 0, -2001 (0xFFFFF82F pop failure), -2000 (neg/sbb/and 7D0h/add F830h push failure).
//  * .rdata literals used: [0074666C] byte 0 ("found"), [00746633] byte 4 (object type area), [007466F4] word 1Eh
//    (effect type visual), [007473B8] word 0 (duration bits), [00746828] 6Ch / [0074682C] 6Dh (pure good / evil
//    power effect types), [007473C8] word 4 (innate duration), [00747E98] 2.5f, [00741BE0] 2.0f, [00740C84] 3.0f,
//    strings [0074868C] "StrRef", [00748680] "BAD STRREF", [00748738] "BACKUPHEAD", [007454E4] "NORMALHEAD".
//  * Globals by symbol only: [007A39FC] manager (w5::server_app/client_app), [007A3A00] VM, [007A3A28] rules,
//    [007A3A08] TLK.  Memory: live_operator_new / live_free only.
//  * Deliberate deviations (UB in C++; documented for issue #23): two original reads of never-written stack slots
//    are given the value 0 here: GetReflexAdjustedDamage's DC on the argc==4 path (unreachable from compiled
//    NWScript, which always pushes all 5 arguments), and DuplicateHeadAppearance's C2DA result when GetINTEntry
//    fails (bad row / missing column).
//  * Extra callees reconstructed here (no provider in the c31 map): 005BEBC0 CSWSFaction::GetStrongestMember,
//    005BF480 CSWSFaction::GetBestAC, 005BF090 CSWSFaction::GetAverageGoodEvilAlignment,
//    005AE610 CSWSCreatureStats::ModifyAlignment, 00501D90 CSWSCreature::UpdatePureGoodEvilPowers (shared: part B
//    imports it), 004F77E0 RemovePureGoodPowers, 004F7830 RemovePureEvilPowers, 004FDBD0 AddPureGoodPowers,
//    004FDC80 AddPureEvilPowers.
#include "1e78b2f608-claude_vm_vmc_common.hpp"

namespace kotor_menu_10_19 { struct String; struct Tlk { String* __thiscall simple_0041e8f0(String* out, unsigned id); }; } // CTlkTable::GetSimpleString [0037]

extern "C" {
// ---- existing providers (c31 map; object in brackets) ----
extern void* kotor_cell_rules_007a3a28;                                                       // [007A3A28] CSWRules* [0421]
extern void* kotor_cell_tlk_007a3a08;                                                         // [007A3A08] CTlkTable* [0384]
void* __fastcall frame_LogStringCtor_005e5a90(void* string, void*, const char* text);         // CExoString(char const*) [0459]
U __fastcall kotor_fastcall_005eda50(void* client, void*, U id);                              // ServerToClientObjectId [0453]
void* __fastcall kotor_fastcall_005ed580(void* client, void*, U id);                          // CClientExoApp::GetGameObject [0701]
void __fastcall feedback_set_info_0063d2d0(void* object, void*, void* text);                  // CSWCObject::SetFeedbackInfo RET 4 [feedback_lowers]
int __fastcall kotor_fastcall_004ae950(void* server, void*, U id, U* out);                    // CServerExoApp::GetFactionOfObject [claude_vm_actions_world]
U __fastcall kotor_fastcall_005b92b0(void* creature, void*, U type, U dc, U subtype, U versus_id, U a5, U a6, U store); // SavingThrowRoll (AL) [claude_combat_attack_rolls]
void* __fastcall recovered_effect_construct_00503e40(void* effect, void*, U flag);            // CGameEffect::CGameEffect RET 4 [0886]
void __fastcall recovered_effect_creator_00503a00(void* effect, void*, U id);                 // CGameEffect::SetCreator RET 4 [0886]
void __fastcall recovered_effect_integer_005036a0(void* effect, void*, U index, U value);     // CGameEffect::SetInteger RET 8 [0886]
void __fastcall recovered_effect_apply_004d14f0(void* object, void*, void* effect, U a, U b); // CSWSObject::ApplyEffect RET 0Ch [0886]
unsigned __fastcall w2x_creature_gold_004edd60(void* creature, void*);                        // CSWSCreature::GetGold [0886]
void* __fastcall recovered_placeable_lookup_004ae7a0(void* server, void*, U id);              // GetPlaceableByGameObjectID [0886]
U __fastcall recovered_find_item_by_base_item_00555e00(void* repository, void*, U base, U flag); // CItemRepository::FindItemWithBaseItemType RET 8 [inventory_00555e00]
void* __fastcall recovered_item_lookup_004ae760(void* server, void*, U id);                   // GetItemByGameObjectID [1025]
U __fastcall kotor_fastcall_005a5110(void* stats, void*);                                     // CSWSCreatureStats::GetSimpleAlignmentGoodEvil (AL) [claude_combat_attack_effectbonus]
void* __fastcall kotor_fastcall_0052b3b0(void* manager, void*, I faction);                    // CFactionManager::GetFaction [claude_vm_actions_world]
U __fastcall recovered_stats_level_005a5fd0(void* stats, void*, U flag);                      // CSWSCreatureStats::GetLevel (AL) [0963]
int __fastcall area_clear_line_of_sight_0050c330(void* area, void*, void* a, void* b, void* c, unsigned* d,
    unsigned caller, unsigned target, int flag, int e);                                       // CSWSArea::ClearLineOfSight RET 20h [0526]
U __fastcall kotor_fastcall_005ad0e0(void* stats, void*, void* attacker, U flag);             // CSWSCreatureStats::GetArmorClassVersus [claude_combat_attack_attackstats]
void* __fastcall combat_reuse_faction_00513fc0(void* creature, void*);                        // CSWSCreature::GetFaction [w1_native_b7]
unsigned __fastcall recovered_faction_leader_005bf5c0(void* faction, void*);                  // CSWSFaction::GetLeader [w2_g07_native]
unsigned __fastcall recovered_object_add_action_004cea20(void* object, void*, unsigned type, unsigned group,
    unsigned t0, void* v0, unsigned t1, void* v1, unsigned t2, void* v2, unsigned t3, void* v3, unsigned t4, void* v4, unsigned t5, void* v5,
    unsigned t6, void* v6, unsigned t7, void* v7, unsigned t8, void* v8, unsigned t9, void* v9, unsigned t10, void* v10, unsigned t11, void* v11,
    unsigned t12, void* v12);                                                                 // CSWSObject::AddAction RET 70h [w1_native_b7]
void __fastcall kotor_fastcall_00518990(void* creature, void*, U keep_own);                   // CSWSCreature::SurrenderToEnemies [claude_w5_creature_actions]
U __fastcall recovered_object_remove_effect_004d06c0(void* object, void*, U lo, U hi);        // CSWSObject::RemoveEffectById RET 8 [w1_native_b7]
void __fastcall recovered_stats_refresh_005addc0(void* stats, void*);                         // CSWSCreatureStats::UpdateCombatInformation [1046]
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* array, void*, U id); // CSWBaseItemArray::GetBaseItem [0820]
void* __fastcall recovered_server_info_004aec90(void* server, void*);                         // CServerExoApp::GetServerInfo [w1_native_b7]
// ---- reconstructed in this file (section at the end) ----
U __fastcall kotor_fastcall_005bebc0(void* faction, void*, U object_id, U visible);
U __fastcall kotor_fastcall_005bf480(void* faction, void*, U object_id, U visible);
I __fastcall kotor_fastcall_005bf090(void* faction, void*);
void __fastcall kotor_fastcall_005ae610(void* stats, void*, U delta);
void __fastcall kotor_fastcall_00501d90(void* creature, void*);
U __fastcall kotor_fastcall_004f77e0(void* creature, void*);
U __fastcall kotor_fastcall_004f7830(void* creature, void*);
void __fastcall kotor_fastcall_004fdbd0(void* creature, void*);
void __fastcall kotor_fastcall_004fdc80(void* creature, void*);
}

namespace {
using namespace vmc;
constexpr U k_type_area_00746633 = 4;
constexpr U k_effect_visual_007466f4 = 0x1e;
constexpr U k_duration_instant_007473b8 = 0;
constexpr U k_effect_pure_good_00746828 = 0x6c;
constexpr U k_effect_pure_evil_0074682c = 0x6d;
constexpr U k_duration_innate_007473c8 = 4;
inline void* rules() { return kotor_cell_rules_007a3a28; }
inline void* two_da(U offset) { return at<void*>(at<void*>(rules(), 0xb8), offset); }   // [[[7A3A28]+B8]+offset]
inline void* faction_manager_or_null() {                                                // [[server+4]+10054h] (server+4 tested)
    void* internal = at<void*>(server_app(), 4);
    return internal ? at<void*>(internal, 0x10054) : nullptr;
}
}  // namespace

extern "C" {

// ===================================================================================================== 00530170
// DisplayFeedBackText(object oCreature, int nTextConstant) [routine 366]
int __fastcall kotor_fastcall_00530170(void*, void*, int, unsigned) {
    U id = 0;                                                                            // [esp+00]
    I row = 0;                                                                           // [esp+08]
    if (!pop_object(&id)) return k_pop_failed;                                           // 0053018E..9A
    id = kotor_fastcall_005eda50(client_app(), nullptr, id);                             // 0053019C..B5
    if (!pop_int(&row)) return k_pop_failed;                                             // 005301B9..C7
    void* object = kotor_fastcall_005ed580(client_app(), nullptr, id);                   // 005301DD..F1
    I strref = 0;                                                                        // 005301F5 [esp+04]
    if (object == nullptr || vcall<void*>(object, 0xc) == nullptr) return 0;             // 005301F3..0C
    ExoString column;
    frame_LogStringCtor_005e5a90(&column, nullptr, "StrRef");                            // 00530212..1B ([0074868C])
    c2da_current_get_int_00413660(two_da(0x108), nullptr, static_cast<U>(row), &column, &strref);        // 00530220..4B
    exo_destroy(&column);                                                                // 00530250..5C
    if (strref != 0) {                                                                   // 00530261..67
        ExoString text;                                                                  // [esp+10] (reused slot)
        reinterpret_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08)->simple_0041e8f0(
            reinterpret_cast<kotor_menu_10_19::String*>(&text), static_cast<unsigned>(strref)); // 00530269..75
        feedback_set_info_0063d2d0(vcall<void*>(object, 0xc), nullptr, &text);           // 0053027A..90
        exo_destroy(&text);                                                              // 00530295..D0
    } else {
        ExoString text;                                                                  // [esp+18]
        frame_LogStringCtor_005e5a90(&text, nullptr, "BAD STRREF");                      // 0053029B..A4 ([00748680])
        feedback_set_info_0063d2d0(vcall<void*>(object, 0xc), nullptr, &text);           // 005302A9..BF
        exo_destroy(&text);                                                              // 005302C4..D0
    }
    return 0;                                                                            // 005302D5..E6
}

// ===================================================================================================== 00538450
// GetFactionEqual(object oFirstObject, object oSecondObject=OBJECT_SELF) [routine 172]
int __fastcall kotor_fastcall_00538450(void*, void*, int, unsigned) {
    U first = 0, second = 0, faction_a = 0, faction_b = 0;
    I result = 0;                                                                        // 0053845F esi
    if (!pop_object(&first)) return k_pop_failed;                                        // 0053845A..6A
    if (!pop_object(&second)) return k_pop_failed;                                       // 00538476..88
    if (kotor_fastcall_004ae950(server_app(), nullptr, first, &faction_a) == 1 &&        // 0053848A..A5
        kotor_fastcall_004ae950(server_app(), nullptr, second, &faction_b) == 1 &&       // 005384A7..C1
        faction_a == faction_b)                                                          // 005384C3..CB
        result = 1;                                                                      // 005384CD
    return push_int(result);                                                             // 005384D2..F0
}

// ===================================================================================================== 00536630
// GetAppearanceType(object oCreature) [routine 524]
int __fastcall kotor_fastcall_00536630(void*, void*, int, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                           // 00536631..4B
    I result = 0;                                                                        // 0053665D
    void* creature = creature_by_id(id);                                                 // 0053664E..5F
    if (creature) result = at<U16>(at<void*>(creature, 0xa74), 0x186);                   // 00536664..6E
    return push_int(result);                                                             // 00536675..91
}

// ===================================================================================================== 00536F80
// GetCreatureSize(object oCreature) [routine 479]
int __fastcall kotor_fastcall_00536f80(void*, void*, int, unsigned) {
    U id = 0;
    I result = 0;                                                                        // 00536F8D
    if (!pop_object(&id)) return k_pop_failed;                                           // 00536F88..9F
    void* creature = creature_by_id(id);                                                 // 00536FA2..B0
    if (creature) result = at<I>(creature, 0x4f8);                                       // 00536FB5..B9
    return push_int(result);                                                             // 00536FBF..DB
}

// ===================================================================================================== 0053CC30
// GetReflexAdjustedDamage(int nDamage, object oTarget, int nDC, int nSaveType=0, object oSaveVersus=OBJECT_SELF)
// [routine 299].  argc == 4 pops a different order (object, int, int, object) and leaves the DC slot unwritten
// (see the deviation note at the top: DC = 0 here).
int __fastcall kotor_fastcall_0053cc30(void*, void*, int, unsigned argc) {
    U versus = 0;       // [esp+00]
    U target = 0;       // [esp+04]
    I save_type = 0;    // [esp+08]
    I dc = 0;           // [esp+0C] (never written on the argc==4 path in the original)
    I damage = 0;       // argc slot [esp+28]
    if (argc == 4) {                                                                     // 0053CC3E..4F
        if (!pop_object(&target)) return k_pop_failed;                                   // 0053CC51..63
        if (!pop_int(&damage)) return k_pop_failed;                                      // 0053CC65..77
        if (!pop_int(&save_type)) return k_pop_failed;                                   // 0053CC79..8B
    } else {
        if (!pop_int(&damage)) return k_pop_failed;                                      // 0053CCB7..C9
        if (!pop_object(&target)) return k_pop_failed;                                  // 0053CCCB..DD
        if (!pop_int(&dc)) return k_pop_failed;                                          // 0053CCDF..F1
        if (!pop_int(&save_type)) return k_pop_failed;                                   // 0053CCF3..05
    }
    if (!pop_object(&versus)) return k_pop_failed;                                       // 0053CC8D..9F / 0053CD07..0C
    void* creature = creature_by_id(target);                                             // 0053CD0E..1C
    if (creature != nullptr &&                                                           // 0053CD21..25
        static_cast<U8>(kotor_fastcall_005b92b0(creature, nullptr, 2, static_cast<U>(dc), static_cast<U>(save_type),
                                                versus, 1, 0, 0)) != 0) {                // 0053CD2B..4B
        damage = damage / 2;                                                             // 0053CD51..60 cdq/sub/sar
        void* memory = live_operator_new(0x8c);                                          // 0053CD5B..69
        void* e = memory ? recovered_effect_construct_00503e40(memory, nullptr, 1) : nullptr; // 0053CD6C..89
        at<U16>(e, 0xa) = static_cast<U16>((at<U16>(e, 0xa) & 0xfff8u) | k_duration_instant_007473b8); // 0053CD8B..A5
        recovered_effect_creator_00503a00(e, nullptr, versus);                           // 0053CDA9..B0
        at<U16>(e, 8) = static_cast<U16>(k_effect_visual_007466f4);                      // 0053CDB5..C5
        recovered_effect_integer_005036a0(e, nullptr, 0, 0xfa0);                         // 0053CDBC..C9
        recovered_effect_apply_004d14f0(creature, nullptr, e, 0, 0);                     // 0053CDCE..D5
    }
    return push_int(damage);                                                             // 0053CDDB..08
}

// ===================================================================================================== 00539030
// GetGold(object oTarget=OBJECT_SELF) [routine 418]
int __fastcall kotor_fastcall_00539030(void*, void*, int, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                           // 00539031..4B
    I result = 0;                                                                        // 0053905D
    void* creature = creature_by_id(id);                                                 // 0053904E..5F
    if (creature) {
        result = static_cast<I>(w2x_creature_gold_004edd60(creature, nullptr));          // 00539068..6F
    } else {
        void* placeable = recovered_placeable_lookup_004ae7a0(server_app(), nullptr, id); // 00539073..80
        if (placeable && at<void*>(placeable, 0x36c)) {                                  // 00539085..91
            const U item_id = recovered_find_item_by_base_item_00555e00(at<void*>(placeable, 0x36c), nullptr, 0x17, 0); // 00539093..97
            void* item = recovered_item_lookup_004ae760(server_app(), nullptr, item_id); // 0053909C..A6
            if (item) result = at<I>(item, 0x28c);                                       // 005390AB..AF
        }
    }
    return push_int(result);                                                             // 005390B5..D1
}

// ===================================================================================================== 005364F0
// GetGoodEvilValue(object) [routine 125] / GetAlignmentGoodEvil(object) [routine 127]
int __fastcall kotor_fastcall_005364f0(void*, void*, int routine, unsigned) {
    U id = 0;
    I result = -1;                                                                       // 005364FF
    if (!pop_object(&id)) return k_pop_failed;                                           // 005364FA..14
    void* object = nullptr;
    if (lookup(id, &object) && type_of(object) > k_type_area_00746633) {                 // 00536517..4B (jbe unsigned)
        void* stats = at<void*>(as_creature(object), 0xa74);                             // 0053654D..52 (cast unchecked)
        if (routine == 0x7d)                                                             // 00536558..5F
            result = static_cast<int16_t>(at<U16>(stats, 0x17e));                        // 00536570 movsx
        else if (routine == 0x7f)                                                        // 00536561..64
            result = static_cast<U8>(kotor_fastcall_005a5110(stats, nullptr));          // 00536566..6B movzx al
    }
    return push_int(result);                                                             // 00536577..95
}

// ===================================================================================================== 00538970
// GetFactionStrongestMember(object oFactionMember=OBJECT_SELF, int bMustBeVisible=TRUE) [routine 182]
int __fastcall kotor_fastcall_00538970(void*, void*, int, unsigned argc) {
    U result = k_invalid;                                                                // 0053897E esi
    U id = k_invalid;                                                                    // 00538984
    I visible = 0;
    if (!pop_object(&id)) return k_pop_failed;                                           // 0053897A..9A
    visible = 1;                                                                         // 005389A2
    if (argc == 2 && !pop_int(&visible)) return k_pop_failed;                            // 0053899D..BE
    void* creature = creature_by_id(id);                                                 // 005389C0..CE
    if (creature) {
        const I faction_id = at<I>(at<void*>(creature, 0xa74), 0x78);                    // 005389D7..EB
        void* manager = faction_manager_or_null();                                       // 005389DD..F8
        if (manager) {
            void* faction = kotor_fastcall_0052b3b0(manager, nullptr, faction_id);       // 005389FA..07
            result = kotor_fastcall_005bebc0(faction, nullptr, id, static_cast<U>(visible)); // 00538A0C..13 (null unchecked)
        }
    }
    return push_status(recovered_vm_outer_push_object_005d10d0(vm(), nullptr, result)); // 00538A15..33
}

// ===================================================================================================== 00538380
// GetFactionBestAC(object oFactionMember=OBJECT_SELF, int bMustBeVisible=TRUE) [routine 193]
int __fastcall kotor_fastcall_00538380(void*, void*, int, unsigned argc) {
    U result = k_invalid;                                                                // 0053838E
    U id = k_invalid;                                                                    // 00538394
    I visible = 0;
    if (!pop_object(&id)) return k_pop_failed;                                           // 0053838A..AA
    visible = 1;                                                                         // 005383B2
    if (argc == 2 && !pop_int(&visible)) return k_pop_failed;                            // 005383AD..CE
    void* creature = creature_by_id(id);                                                 // 005383D0..DE
    if (creature) {
        const I faction_id = at<I>(at<void*>(creature, 0xa74), 0x78);                    // 005383E7..FB
        void* manager = faction_manager_or_null();                                       // 005383ED..08
        if (manager) {
            void* faction = kotor_fastcall_0052b3b0(manager, nullptr, faction_id);       // 0053840A..17
            result = kotor_fastcall_005bf480(faction, nullptr, id, static_cast<U>(visible)); // 0053841C..23
        }
    }
    return push_object(result);                                                          // 00538425..43
}

// ===================================================================================================== 005380F0
// GetFactionAverageGoodEvilAlignment(object oFactionMember=OBJECT_SELF) [routine 187]
int __fastcall kotor_fastcall_005380f0(void*, void*, int, unsigned) {
    I result = -1;                                                                       // 005380FD
    U id = k_invalid;                                                                    // 00538100
    if (!pop_object(&id)) return k_pop_failed;                                           // 005380F8..18
    void* creature = creature_by_id(id);                                                 // 0053811B..29
    if (creature) {
        const I faction_id = at<I>(at<void*>(creature, 0xa74), 0x78);                    // 00538132..38
        void* manager = faction_manager_or_null();                                       // 0053813B..53
        if (manager)
            result = kotor_fastcall_005bf090(kotor_fastcall_0052b3b0(manager, nullptr, faction_id), nullptr); // 00538155..64
    }
    return push_int(result);                                                             // 00538166..82
}

// ===================================================================================================== 00538670
// GetFactionLeader(object oMemberOfFaction) [routine 562]
int __fastcall kotor_fastcall_00538670(void*, void*, int, unsigned) {
    U id = 0;
    U result = k_invalid;                                                                // 0053867D edi
    if (!pop_object(&id)) return k_pop_failed;                                           // 00538678..92
    void* creature = creature_by_id(id);                                                 // 00538695..A4
    if (creature && combat_reuse_faction_00513fc0(creature, nullptr) != nullptr)         // 005386AB..B8
        result = recovered_faction_leader_005bf5c0(combat_reuse_faction_00513fc0(creature, nullptr), nullptr); // 005386BA..C8
    return push_object(result);                                                          // 005386CA..E7
}

// ===================================================================================================== 00547BE0
// SetGoodEvilValue(object oCreature, int nAlignment) [routine 750]
int __fastcall kotor_fastcall_00547be0(void*, void*, int, unsigned) {
    U id = 0; I value = 0;
    if (!pop_object(&id)) return k_pop_failed;                                           // 00547BE9..F5
    if (!pop_int(&value)) return k_pop_failed;                                           // 00547BF7..13
    void* creature = creature_by_id(id);                                                 // 00547C16..24
    if (creature) {
        void* stats = at<void*>(creature, 0xa74);                                        // 00547C2D
        const U delta = static_cast<U16>(static_cast<U16>(value) - at<U16>(stats, 0x17e)); // 00547C33..41 (zero-extended word)
        kotor_fastcall_005ae610(stats, nullptr, delta);                                  // 00547C42
    }
    return 0;                                                                            // 00547C47..4C
}

// ===================================================================================================== 00546F90
// DuplicateHeadAppearance(object oidCreatureToChange, object oidCreatureToMatch) [routine 500]
int __fastcall kotor_fastcall_00546f90(void*, void*, int, unsigned) {
    U change = 0, match = 0;                                                             // [esp+04] / [esp+08]
    if (!pop_object(&change)) return k_pop_failed;                                       // 00546FAE..BA
    if (!pop_object(&match)) return k_pop_failed;                                        // 00546FBC..CE
    void* target = creature_by_id(change);                                               // 00546FE6..F6 edi
    void* source = creature_by_id(match);                                                // 00546FFB..0B esi
    if (target == nullptr || source == nullptr) return 0;                                // 00547010..1C
    I head = 0;                                                                          // [esp+00] (unwritten in the original; see top)
    ExoString column;
    frame_LogStringCtor_005e5a90(&column, nullptr, at<U>(source, 0xa6c) != 0 ? "BACKUPHEAD" : "NORMALHEAD"); // 00547022..35 / 0054706F..78
    c2da_current_get_int_00413660(two_da(0xc), nullptr, at<U16>(source, 0xa60), &column, &head); // 0054703A..64 / 0054707D..A8
    exo_destroy(&column);                                                                // 00547069 / 005470AD..B9
    at<U8>(target, 0xa70) = static_cast<U8>(head);                                       // 005470BE..C2
    return 0;                                                                            // 005470C8..DA
}

// ===================================================================================================== 00544830
// SurrenderToEnemies() [379] / SurrenderRetainBuffs() [476] / SurrenderByFaction-style (keep own) [762]
int __fastcall kotor_fastcall_00544830(void* self, void*, int routine, unsigned) {
    void* array = object_array();                                                        // 00544831..3C (called first)
    if (!self_valid(self)) return 0;                                                     // 00544841..45
    void* object = nullptr;
    if (static_cast<U8>(kotor_fastcall_004d8230(array, nullptr, self_id(self), &object)) != 0) return 0; // 0054484B..61
    void* creature = as_creature(object);                                                // 00544867..74
    if (creature == nullptr) return 0;
    if (at<U>(at<void*>(creature, 0xa74), 0x6c) != 0) return 0;                          // 0054487A..85
    if (routine == 0x17b) {                                                              // 00544887..90
        recovered_object_add_action_004cea20(creature, nullptr, 0x41, 0xffff,            // 00544892..CD
            0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr,
            0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr);
    } else if (routine == 0x1dc) {                                                       // 005448D9..DE
        kotor_fastcall_00518990(creature, nullptr, 0);                                   // 005448E0..E2
    } else if (routine == 0x2fa) {                                                       // 005448EE..F3
        kotor_fastcall_00518990(creature, nullptr, 1);                                   // 005448F5..F7
    }
    return 0;                                                                            // 005448FC..00
}

// ===================================================================================================== 0053B080
// GetLastKiller() [routine 437]: OBJECT_SELF (+8) looked up without the +4 validity test
int __fastcall kotor_fastcall_0053b080(void* self, void*, int, unsigned) {
    U result = k_invalid;                                                                // 0053B091
    void* object = game_object(self_id(self));                                           // 0053B080..96
    if (object && as_object(object)) {                                                   // 0053B09B..AA
        void* sws = as_object(object);                                                   // 0053B0AC..B3
        if (sws) result = at<U>(sws, 0x154);                                             // 0053B0B5..B7
    }
    return push_object(result);                                                          // 0053B0BD..D9
}

// ===================================================================================================== 0053B4E0
// GetLastUnlocked() [routine 350].  Quirk: when OBJECT_SELF is not a CSWSObject, the original falls back to the
// slot holding its own ECX (the CSWVirtualMachineCommands this pointer) and type-tests / casts that object.
int __fastcall kotor_fastcall_0053b4e0(void* self, void*, int, unsigned) {
    U result = k_invalid;                                                                // 0053B4F2
    void* object = game_object(self_id(self));                                           // 0053B4E1..F7
    void* target = nullptr;
    if (object && as_object(object)) target = as_object(object);                         // 0053B4FC..14
    else target = self;                                                                  // 0053B518 [esp+8] = pushed ECX
    if (target) {                                                                        // 0053B51C..1E
        const U8 type = type_of(target);                                                 // 0053B520
        if (type == 0xa) result = at<U>(as_door(target), 0x318);                         // 0053B523..2C
        else if (type == 9) result = at<U>(as_placeable(target), 0x368);                 // 0053B534..3D
    }
    return push_object(result);                                                          // 0053B543..60
}

// ===================================================================================================== 0053E4C0
// GetWeaponRanged(object oItem) [routine 511]
int __fastcall kotor_fastcall_0053e4c0(void*, void*, int, unsigned) {
    U id = 0;
    I result = 0;                                                                        // 0053E4CD
    if (!pop_object(&id)) return k_pop_failed;                                           // 0053E4C8..DF
    void* item = recovered_item_lookup_004ae760(server_app(), nullptr, id);              // 0053E4E2..F0
    if (item) {
        void* base = kotor_m15b_base_item_array_get_base_item_005b31a0(at<void*>(rules(), 0x34), nullptr, at<U>(item, 0xc)); // 0053E4F9..05
        result = at<U8>(base, 0x1a) != 0 ? 1 : 0;                                        // 0053E50A..13 (0 < byte, unsigned)
    }
    return push_int(result);                                                             // 0053E515..31
}

// ===================================================================================================== 0053F080
// GetIsListening(object oObject) [routine 174]
int __fastcall kotor_fastcall_0053f080(void*, void*, int, unsigned) {
    U id = 0;
    I result = 0;                                                                        // 0053F08F
    if (!pop_object(&id)) return k_pop_failed;                                           // 0053F08A..A3
    void* object = nullptr;
    if (lookup(id, &object) && object) result = at<I>(object, 0x19c);                    // 0053F0A6..D5
    return push_int(result);                                                             // 0053F0DB..F9
}

// ===================================================================================================== 00539870
// GetInventoryDisturbType() [routine 352]: OBJECT_SELF (+8) without the +4 validity test
int __fastcall kotor_fastcall_00539870(void* self, void*, int, unsigned) {
    I result = 0;                                                                        // 00539880
    void* object = game_object(self_id(self));                                           // 00539870..82
    if (object) {
        void* sws = as_object(object);                                                   // 0053988B..92
        if (sws) result = at<I>(sws, 0x178);                                             // 00539896
    }
    return push_int(result);                                                             // 0053989C..B7
}

// ===================================================================================================== 0053B480
// GetLastTrapDetected(object oTarget=OBJECT_SELF) [routine 486]
int __fastcall kotor_fastcall_0053b480(void*, void*, int, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                           // 0053B481..9B
    U result = k_invalid;                                                                // 0053B4AD
    void* creature = creature_by_id(id);                                                 // 0053B49E..B2
    if (creature) result = at<U>(creature, 0x370);                                       // 0053B4B7..BB
    return push_object(result);                                                          // 0053B4C1..DD
}

// ===================================================================================================== 00538CA0
// GetGameDifficulty() [routine 513]
int __fastcall kotor_fastcall_00538ca0(void*, void*, int, unsigned) {
    void* info = recovered_server_info_004aec90(server_app(), nullptr);                  // 00538CA0..A8
    return push_int(at<I>(info, 0x108));                                                 // 00538CAD..CD
}

// =====================================================================================================
// Extra callees (no provider in the c31 map)
// =====================================================================================================

// 005BEBC0 CSWSFaction::GetStrongestMember(ulong oidVisibleTo, int bMustBeVisible), thiscall RET 8.
// Faction: +0 ulong* member ids, +4 count.  Highest GetLevel(0) wins (strictly greater, signed compare);
// with a valid visible-to object and bMustBeVisible, a member also needs ClearLineOfSight == 1 between the
// member's and the observer's positions (+90h), both creatures and the member's area present.
U __fastcall kotor_fastcall_005bebc0(void* faction, void*, U object_id, U visible) {
    I best = 0;                                                                          // 005BEBD3 [esp+14]
    U result = k_invalid;                                                                // 005BEBD7 [esp+18]
    U hit_object = 0;                                                                    // [esp+1C] out of ClearLineOfSight (never initialised)
    for (I i = 0; i < at<I>(faction, 4); ++i) {                                          // 005BEBDB..DF / 005BECC8..D2
        void* member = creature_by_id(at<U*>(faction, 0)[i]);                            // 005BEBF0..FE
        if (member == nullptr) continue;                                                 // 005BEC05..07
        const I level = static_cast<U8>(recovered_stats_level_005a5fd0(at<void*>(member, 0xa74), nullptr, 0)); // 005BEC0D..1A
        if (level <= best) continue;                                                     // 005BEC1D..21 (jle)
        if (object_id == k_invalid || visible == 0) {                                    // 005BEC27..3C
            best = level; result = at<U*>(faction, 0)[i];                                // 005BECE6..F3
            continue;
        }
        Vector hit = {0.0f, 0.0f, 0.0f};                                                 // 005BEC48..58 [esp+20..28]
        void* observer = creature_by_id(object_id);                                      // 005BEC42..64
        if (observer == nullptr) continue;                                               // 005BEC69..6D
        if (recovered_object_area_004cb120(member, nullptr) == nullptr) continue;        // 005BEC6F..78
        if (area_clear_line_of_sight_0050c330(recovered_object_area_004cb120(member, nullptr), nullptr, // 005BEC7A..A9
                static_cast<unsigned char*>(member) + 0x90, static_cast<unsigned char*>(observer) + 0x90,
                &hit, &hit_object, k_invalid, k_invalid, 0, 0) == 1) {                    // 005BECAE..B1
            best = level; result = at<U*>(faction, 0)[i];                                // 005BECB3..C0
        }
    }
    return result;                                                                       // 005BECD8..E3
}

// 005BF480 CSWSFaction::GetBestAC(ulong oidVisibleTo, int bMustBeVisible), thiscall RET 8.  Same shape as
// GetStrongestMember with GetArmorClassVersus(0, 0); the compare is 16-bit signed (cmp di, word [best]) while the
// stored best is the full returned dword.
U __fastcall kotor_fastcall_005bf480(void* faction, void*, U object_id, U visible) {
    U best = 0;                                                                          // 005BF493 [esp+14]
    U result = k_invalid;                                                                // 005BF497 [esp+18]
    U hit_object = 0;                                                                    // [esp+1C] (never initialised)
    for (I i = 0; i < at<I>(faction, 4); ++i) {                                          // 005BF49B..9F / 005BF58A..94
        void* member = creature_by_id(at<U*>(faction, 0)[i]);                            // 005BF4B0..BE
        if (member == nullptr) continue;                                                 // 005BF4C5..C7
        const U ac = kotor_fastcall_005ad0e0(at<void*>(member, 0xa74), nullptr, nullptr, 0); // 005BF4CD..DC
        if (static_cast<int16_t>(ac) <= static_cast<int16_t>(best)) continue;            // 005BF4DE..E3 (jle, word)
        if (object_id == k_invalid || visible == 0) {                                    // 005BF4E9..FE
            best = ac; result = at<U*>(faction, 0)[i];                                   // 005BF5A8..B5
            continue;
        }
        Vector hit = {0.0f, 0.0f, 0.0f};                                                 // 005BF50A..1A
        void* observer = creature_by_id(object_id);                                      // 005BF504..26
        if (observer == nullptr) continue;                                               // 005BF52B..2F
        if (recovered_object_area_004cb120(member, nullptr) == nullptr) continue;        // 005BF531..3A
        if (area_clear_line_of_sight_0050c330(recovered_object_area_004cb120(member, nullptr), nullptr, // 005BF53C..6B
                static_cast<unsigned char*>(member) + 0x90, static_cast<unsigned char*>(observer) + 0x90,
                &hit, &hit_object, k_invalid, k_invalid, 0, 0) == 1) {                    // 005BF570..73
            best = ac; result = at<U*>(faction, 0)[i];                                   // 005BF575..82
        }
    }
    return result;                                                                       // 005BF59A..A5
}

// 005BF090 CSWSFaction::GetAverageGoodEvilAlignment(), thiscall plain RET.  Sums GetSimpleAlignmentGoodEvil
// (movzx AL) over the members found.  Quirks: an empty faction returns 1; a one-member faction returns the raw sum;
// otherwise the integer average is classified against 2.5 (equal -> 1) and then by |avg-3| <= |avg-2| -> 3, else 2.
I __fastcall kotor_fastcall_005bf090(void* faction, void*) {
    I sum = 0;                                                                           // 005BF099 ebx
    for (I i = 0; i < at<I>(faction, 4); ++i) {                                          // 005BF09B..CE
        void* member = creature_by_id(at<U*>(faction, 0)[i]);                            // 005BF0A1..AF
        if (member) sum += static_cast<U8>(kotor_fastcall_005a5110(at<void*>(member, 0xa74), nullptr)); // 005BF0B4..C6
    }
    const I count = at<I>(faction, 4);                                                   // 005BF0D0
    if (count <= 0) return 1;                                                            // 005BF0D3..D5 -> 005BF0F8
    if (count == 1) return sum;                                                          // 005BF0D7..DC
    const I average = sum / count;                                                       // 005BF0DE..E1 idiv
    const double avg = static_cast<double>(average);                                     // 005BF0E5 fild
    if (avg == static_cast<double>(2.5f)) return 1;                                      // 005BF0E9..F8 (fcom [747E98]; ZF path)
    const double to_two = avg - static_cast<double>(2.0f);                               // 005BF102..08 fsubr
    const double d_two = to_two < 0 ? -to_two : to_two;                                  // 005BF10A fabs
    const double to_three = avg - static_cast<double>(3.0f);                             // 005BF10E fsub [740C84]
    const double d_three = to_three < 0 ? -to_three : to_three;                          // 005BF114 fabs
    if (d_three < d_two || d_three == d_two) return 3;                                   // 005BF116..1D (C0|C3 -> 3)
    return 2;                                                                            // 005BF11F..28
}

// 005AE610 CSWSCreatureStats::ModifyAlignment(short nDelta), thiscall RET 4.  Clamps (short)+0x17E + delta to
// [0,100], stores the word, refreshes pure good/evil powers of the owning creature (+24h) when present, then
// UpdateCombatInformation.
void __fastcall kotor_fastcall_005ae610(void* stats, void*, U delta) {
    I value = static_cast<int16_t>(static_cast<U16>(delta)) + static_cast<int16_t>(at<U16>(stats, 0x17e)); // 005AE610..1F
    if (value > 0x64) value = 0x64;                                                      // 005AE621..33
    else if (value < 0) value = 0;                                                       // 005AE626..2A
    // 005AE63A..57: second clamp on (short)AX - a no-op for the already clamped value
    const I as_short = static_cast<int16_t>(static_cast<U16>(value));
    if (as_short > 0x64) value = 0x64;
    else if (as_short < 0) value = 0;
    else value = as_short;
    at<U16>(stats, 0x17e) = static_cast<U16>(value);                                     // 005AE65E
    void* creature = at<void*>(stats, 0x24);                                             // 005AE659..65
    if (creature) kotor_fastcall_00501d90(creature, nullptr);                            // 005AE667
    recovered_stats_refresh_005addc0(stats, nullptr);                                    // 005AE66C..6E
}

// 00501D90 CSWSCreature::UpdatePureGoodEvilPowers(), thiscall plain RET (shared callee: part B imports it).
// Good/evil 0 -> remove pure good, add pure evil; 100 -> remove pure evil, add pure good; else remove both.
void __fastcall kotor_fastcall_00501d90(void* creature, void*) {
    void* stats = at<void*>(creature, 0xa74);                                            // 00501D93..9B
    if (stats == nullptr) return;                                                        // 00501DD6
    const U16 value = at<U16>(stats, 0x17e);                                             // 00501D9D
    if (value == 0) {                                                                    // 00501DA4..A7
        kotor_fastcall_004f77e0(creature, nullptr);                                      // 00501DA9
        kotor_fastcall_004fdc80(creature, nullptr);                                      // 00501DB1 (tail jmp)
    } else if (value == 0x64) {                                                          // 00501DB6..BA
        kotor_fastcall_004f7830(creature, nullptr);                                      // 00501DBC
        kotor_fastcall_004fdbd0(creature, nullptr);                                      // 00501DC4 (tail jmp)
    } else {
        kotor_fastcall_004f77e0(creature, nullptr);                                      // 00501DC9
        kotor_fastcall_004f7830(creature, nullptr);                                      // 00501DD1 (tail jmp)
    }
}

namespace {
// 004F77E0 / 004F7830 body: first applied effect (+124h CGameEffect* array, +128h count) whose WORD type (+8)
// equals `type` is removed by its 64-bit id (+0 lo, +4 hi).  EAX: count when none matches, else the callee's EAX.
U remove_first_effect_of_type(void* creature, U type) {
    const I count = at<I>(creature, 0x128);                                              // 004F77E1
    if (count <= 0) return 0;                                                            // 004F77E7..EB
    void** effects = at<void**>(creature, 0x124);                                        // 004F77F6
    I i = 0;
    for (; i < count; ++i)                                                               // 004F7800..10
        if (at<U16>(effects[i], 8) == type) break;
    if (i == count) return static_cast<U>(count);                                        // 004F7812..16
    void* e = effects[i];                                                                // 004F7817
    return recovered_object_remove_effect_004d06c0(creature, nullptr, at<U>(e, 0), at<U>(e, 4)); // 004F781A..21
}
// 004FDBD0 / 004FDC80 body: remove the old power, then apply a new innate (duration 4) effect of `type`, creator
// = the creature's own id (+4), sub-type bits 3/4 cleared, +1C = -1.
void add_pure_power(void* creature, U type) {
    void* memory = live_operator_new(0x8c);                                              // 004FDBEF..F9
    void* e = memory ? recovered_effect_construct_00503e40(memory, nullptr, 1) : nullptr; // 004FDBFC..19
    at<U16>(e, 8) = static_cast<U16>(type);                                              // 004FDC1B..27
    at<U16>(e, 0xa) = static_cast<U16>((at<U16>(e, 0xa) & 0xfff8u) | k_duration_innate_007473c8); // 004FDC21..40
    recovered_effect_creator_00503a00(e, nullptr, at<U>(creature, 4));                   // 004FDC44..4A
    at<U8>(e, 0xa) = static_cast<U8>(at<U8>(e, 0xa) & 0xe7u);                            // 004FDC4F
    at<U>(e, 0x1c) = 0xffffffffu;                                                        // 004FDC5A
    recovered_effect_apply_004d14f0(creature, nullptr, e, 0, 0);                         // 004FDC53..61
}
}  // namespace

// 004F77E0 CSWSCreature::RemovePureGoodPowers(), thiscall plain RET
U __fastcall kotor_fastcall_004f77e0(void* creature, void*) {
    return remove_first_effect_of_type(creature, k_effect_pure_good_00746828);          // [00746828] 6Ch
}
// 004F7830 CSWSCreature::RemovePureEvilPowers(), thiscall plain RET
U __fastcall kotor_fastcall_004f7830(void* creature, void*) {
    return remove_first_effect_of_type(creature, k_effect_pure_evil_0074682c);          // [0074682C] 6Dh
}
// 004FDBD0 CSWSCreature::AddPureGoodPowers(), thiscall plain RET
void __fastcall kotor_fastcall_004fdbd0(void* creature, void*) {
    kotor_fastcall_004f77e0(creature, nullptr);                                          // 004FDBEA
    add_pure_power(creature, k_effect_pure_good_00746828);                               // 004FDBEF..66
}
// 004FDC80 CSWSCreature::AddPureEvilPowers(), thiscall plain RET
void __fastcall kotor_fastcall_004fdc80(void* creature, void*) {
    kotor_fastcall_004f7830(creature, nullptr);                                          // 004FDC9A
    add_pure_power(creature, k_effect_pure_evil_0074682c);                               // 004FDC9F..16
}

}  // extern "C"
