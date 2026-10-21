// Lane vmB, file "endar": Endar-used NWScript engine-command handlers of package vmB
// (CSWVirtualMachineCommands::ExecuteCommand*), reconstructed from the original swkotor.exe (read-only, capstone).
// Every handler: original int __thiscall (int routine, int argc), RET 8 (verified) ->
//   extern "C" int __fastcall kotor_fastcall_<addr>(self, edx, routine, argc) @16.
// Addresses in comments are original instruction ranges.  Conventions: claude_vm_vmB_common.hpp.
#include "54db204e3b-claude_vm_vmb_common.hpp"

extern "C" {
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);                // GetServerAIMaster [creature_destructor_r2]
void* __fastcall kotor_fastcall_004ca7a0(void* location, void*);                          // CScriptLocation() [claude_vm_actions_queries]
int __fastcall kotor_fastcall_004ae950(void* server, void*, U id, U* out);                // GetFactionOfObject RET 8 [0530/0595]
void* __fastcall kotor_fastcall_004ae910(void* server, void*, U id);                      // GetClientObjectByObjectId RET 4 [0595]
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);              // GetSWSMessage [creature_destructor_r2]
U __fastcall recovered_cc_message_00568570(void* message, void*, U player, U type, void* data, U a4); // SendServerToPlayerCCMessage RET 10h
void* __fastcall recovered_message_data_construct_004d69f0(void* data, void*);            // CSWCCMessageData() [w1]
void __fastcall recovered_message_data_destroy_004d6a40(void* data, void*);               // ~CSWCCMessageData [w1]
void __fastcall recovered_event_integer_004d76b0(void* data, void*, U index, U value);    // CSWCCMessageData::SetInteger RET 8 [1017]
void __fastcall recovered_event_object_004d7780(void* data, void*, U index, U id);        // CSWCCMessageData::SetObjectID RET 8 [1017]
U __fastcall kotor_fastcall_004d6ee0(void* data, void*, void* other);                     // CSWCCMessageData::CopyTo RET 4 [claude_combat_attack_round]
void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void* spells, void*, int id);  // CSWSpellArray::GetSpell RET 4 [0820]
U __fastcall kotor_fastcall_0059b650(void* spell, void*, U cls);                          // CSWSpell::GetSpellLevel (AL) RET 4 [claude_combat_spell]
U __fastcall kt_get_class_005a4e90(void* stats, void*, U index);                          // CSWSCreatureStats::GetClass (AL) RET 4 [provider_adapters]
unsigned __fastcall recovered_stats_level_005a5fd0(void* stats, void*, U flag);           // CSWSCreatureStats::GetLevel (AL) RET 4
I __fastcall kotor_fastcall_005a5a60(void* stats, void*);                                 // CSWSCreatureStats::GetForceResistance (AL) RET 0
U __fastcall recovered_rules_roll_dice_00550b60(void* rules, void*, U count, U sides);    // CSWRules::RollDice RET 8
U __fastcall recovered_effect_integer_00503690(void* effect, void*, U index);             // CGameEffect::GetInteger RET 4 [w1 b7]
U __fastcall recovered_faction_is_npc_0052b280(void* manager, void*, U faction);          // CFactionManager::GetIsNPCFaction RET 4 [w1 b7]
I __fastcall recovered_faction_npc_reputation_0052b420(void* manager, void*, U own, U other); // GetNPCFactionReputation RET 8 [w1 b7]
// ---- reconstructed in this file (section at the end) ----
I __fastcall kotor_fastcall_004d13a0(void* object, void*, void* caster);                  // CSWSObject::DoSpellLevelAbsorption RET 4
I __fastcall kotor_fastcall_004ccfc0(void* object, void*, void* caster);                  // CSWSObject::DoSpellImmunity RET 4
void __fastcall kotor_fastcall_00514070(void* creature, void*, U faction, I delta);       // CSWSCreature::AdjustReputation RET 8
void __fastcall kotor_fastcall_0052b3d0(void* manager, void*, U own, U other, I value);   // CFactionManager::SetNPCFactionReputation RET 0Ch
}

using namespace vmb;

extern "C" {

// ===================================================================================================== 0054AE40
// RemoveEffect(object oCreature, effect eEffect) [routine 87]
// Marks every applied effect whose 64-bit id equals the popped effect's id (+20h = 0) and queues AI event 0Eh
// (remove effect, [007441EC]) for the last match.  The popped effect copy is deleted on every non-pop-failure path.
int __fastcall kotor_fastcall_0054ae40(void* self, void*, int, unsigned) {
    U id = 0; void* effect = nullptr;                                                    // 0054AE52 effect = 0
    if (!pop_object(&id)) return k_pop_failed;                                           // 0054AE43..61
    if (!pop_engine(0, &effect)) return k_pop_failed;                                    // 0054AE63..77
    void* object = game_object(id);                                                      // 0054AE84..99
    if (object != nullptr && as_object(object) != nullptr) {                             // 0054AE9B..AC
        unsigned char* list = static_cast<unsigned char*>(as_object(object)) + 0x124;    // 0054AEB2..B9 (second call)
        void* match = nullptr;                                                           // 0054AEBE
        if (list != nullptr && at<I>(list, 4) > 0) {                                     // 0054AEC3..CA (jle signed)
            for (U i = 0; i < at<U>(list, 4); ++i) {                                     // 0054AECC..FD (jb unsigned)
                void* applied = at<void**>(list, 0)[i];                                  // 0054AED7..D9
                if (at<U>(applied, 0) == at<U>(effect, 0) && at<U>(applied, 4) == at<U>(effect, 4)) { // 0054AEDC..E8
                    at<U>(applied, 0x20) = 0;                                            // 0054AEEA
                    match = applied;                                                     // 0054AEF5
                }
            }
            if (match != nullptr)                                                        // 0054AEFF..02
                recovered_event_enqueue_004b08d0(recovered_server_ai_master_004aed80(server_app(), nullptr), nullptr,
                    0, 0, self_id(self), id, 0xe, match);                                // 0054AF04..2C [007441EC]=0Eh
        }
    }
    if (effect != nullptr) delete_effect(effect);                                        // 0054AF31..46
    return 0;                                                                            // 0054AF4A
}

// ===================================================================================================== 00534E90
// EffectHorrified() [routine 471]: choke-family effect (type 8, [0074669C]) with integer 0 = 8.  Flags untouched,
// no creator.
int __fastcall kotor_fastcall_00534e90(void*, void*, int, unsigned) {
    void* e = new_effect();                                                              // 00534EA7..D1
    set_effect_type(e, 8);                                                               // 00534ED3..E7 [0074669C]
    recovered_effect_integer_005036a0(e, nullptr, 0, 8);                                 // 00534ED9..EB
    return push_effect(e);                                                               // 00534EF0..3A
}

// ===================================================================================================== 0053D400
// GetSpellTargetLocation() [routine 222]: location of OBJECT_SELF's spell target (+1B0h position, +9Ch orientation)
// when OBJECT_SELF is a creature or placeable; otherwise the zero location.
int __fastcall kotor_fastcall_0053d400(void* self, void*, int, unsigned) {
    U location[6];                                                                       // [esp+8] CScriptLocation
    kotor_fastcall_004ca7a0(location, nullptr);                                          // 0053D41B..1F
    if (self_valid(self) == 1) {                                                         // 0053D424..3D (GetObjectArray first)
        void* object = nullptr;
        if (lookup(self_id(self), &object)) {                                            // 0053D43F..55
            void* o = as_object(object);                                                 // 0053D457..5D
            if (o != nullptr && (as_creature(o) != nullptr || as_placeable(o) != nullptr)) { // 0053D460..7A
                std::memcpy(&location[0], static_cast<unsigned char*>(o) + 0x1b0, 12);   // 0053D47C..92
                std::memcpy(&location[3], static_cast<unsigned char*>(o) + 0x9c, 12);    // 0053D496..AC
            }
        }
    }
    const U pushed = push_engine_raw(2, location);                                       // 0053D4B0..C4
    location_dtor_004015a0(location);                                                    // 0053D4D3 / 0053D4EE
    return pushed ? 0 : k_push_failed;                                                   // 0053D4D8 / 0053D4F7
}


// ===================================================================================================== 005307F0
// EffectAbilityIncrease(int nAbilityToIncrease, int nModifyBy) [routine 80].  No argc check.
int __fastcall kotor_fastcall_005307f0(void* self, void*, int, unsigned) {
    I ability = 0, amount = 0;
    if (!pop_int(&ability)) return k_pop_failed;                                         // 00530809..1D [esp+4]
    if (!pop_int(&amount)) return k_pop_failed;                                          // 0053081F..31 [esp+8]
    if (ability < 0 || ability > 5) ability = 0;                                         // 0053084A..60 [0074684C]=5, [00746838]=0
    void* e = new_effect();                                                              // 00530865..8F
    set_effect_type(e, 0x24);                                                            // 00530891..9D [0074670C]
    set_subtype_magical(e);                                                              // 00530897..B2
    recovered_effect_integer_005036a0(e, nullptr, 0, static_cast<U>(ability));           // 005308B6..BF
    recovered_effect_integer_005036a0(e, nullptr, 1, static_cast<U>(amount));            // 005308C4..CD
    void* object = nullptr;
    if (lookup(self_id(self), &object) && object != nullptr)                             // 005308D2..FE
        recovered_effect_creator_00503a00(e, nullptr, self_id(self));                    // 00530900..06
    return push_effect(e);                                                               // 0053090B..57
}

// ===================================================================================================== 00533420
// EffectMovementSpeedIncrease(int nNewSpeedPercent) [routine 165].  argc must be exactly 1.
int __fastcall kotor_fastcall_00533420(void* self, void*, int, unsigned argc) {
    I percent = 0;                                                                       // popped into the routine slot [esp+20]
    if (argc != 1) return k_pop_failed;                                                  // 0053343C..42
    if (!pop_int(&percent)) return k_pop_failed;                                         // 00533444..56
    void* e = new_effect();                                                              // 00533470..9A
    set_effect_type(e, 0x1c);                                                            // 0053349C..A9 [007466EC]
    set_subtype_magical(e);                                                              // 005334A3..C0
    recovered_effect_integer_005036a0(e, nullptr, 0, static_cast<U>(percent));           // 005334C4..CB
    void* object = nullptr;
    if (lookup(self_id(self), &object) && object != nullptr)                             // 005334D0..FC
        recovered_effect_creator_00503a00(e, nullptr, self_id(self));                    // 005334FE..04
    return push_effect(e);                                                               // 00533509..55
}

// ===================================================================================================== 00547C50
// GetIsPoisoned(object oObject) [routine 751]: creature +9E0h, else 0.
int __fastcall kotor_fastcall_00547c50(void*, void*, int, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                           // 00547C50..6B
    I result = 0;                                                                        // 00547C7E edi = 0
    void* object = game_object(id);                                                      // 00547C6E..85
    if (object != nullptr && as_object(object) != nullptr) {                             // 00547C87..94
        void* o = as_object(object);                                                     // 00547C96..9A (second call)
        if (type_of(o) == 5)                                                             // 00547C9D..A6 [00746634] byte 5
            result = at<I>(as_creature(o), 0x9e0);                                       // 00547CA8..AF
    }
    return push_int(result);                                                             // 00547CB5..D2
}

// ===================================================================================================== 00533760
// EffectSavingThrowDecrease(int nSave, int nValue, int nSaveType=SAVING_THROW_TYPE_ALL) [routine 452]
int __fastcall kotor_fastcall_00533760(void* self, void*, int, unsigned argc) {
    I save = 0, value = 0, save_type = 0;                                                // 00533786 save_type = 0
    if (!pop_int(&save)) return k_pop_failed;                                            // 00533779..95 [esp+4]
    if (save < 0 || save >= 4) save = 0;                                                 // 00533797..A4
    if (!pop_int(&value)) return k_pop_failed;                                           // 005337AC..BE [esp+C]
    if (static_cast<I>(argc) > 2 && !pop_int(&save_type)) return k_pop_failed;           // 005337C0..D9 (jle signed) [esp+8]
    void* e = new_effect();                                                              // 005337F3..1D
    set_effect_type(e, 0x1b);                                                            // 0053381F..2B [007466E8]
    set_subtype_magical(e);                                                              // 00533825..40
    recovered_effect_integer_005036a0(e, nullptr, 0, static_cast<U>(value));             // 00533844..4D
    recovered_effect_integer_005036a0(e, nullptr, 1, static_cast<U>(save));              // 00533852..5B
    recovered_effect_integer_005036a0(e, nullptr, 2, static_cast<U>(save_type));         // 00533860..69
    recovered_effect_integer_005036a0(e, nullptr, 3, at<U8>(kotor_cell_rules_007a3a28, 0xaa)); // 0053386E..80 rules byte +AAh
    void* object = nullptr;
    if (lookup(self_id(self), &object) && object != nullptr)                             // 00533885..B1
        recovered_effect_creator_00503a00(e, nullptr, self_id(self));                    // 005338B3..B9
    return push_effect(e);                                                               // 005338BE..0A
}

// ===================================================================================================== 00534230
// EffectForceResistanceIncrease(int nValue) [routine 212]: creator set unconditionally, flags set last.
int __fastcall kotor_fastcall_00534230(void* self, void*, int, unsigned) {
    I value = 0;
    if (!pop_int(&value)) return k_pop_failed;                                           // 00534249..5D
    void* e = new_effect();                                                              // 00534277..A1
    set_effect_type(e, 0x21);                                                            // 005342A3..AA [00746700]
    recovered_effect_creator_00503a00(e, nullptr, self_id(self));                        // 005342AE..BC
    recovered_effect_integer_005036a0(e, nullptr, 0, static_cast<U>(value));             // 005342C1..CA
    set_subtype_magical(e);                                                              // 005342CF..E1
    return push_effect(e);                                                               // 005342E5..2E
}

// ===================================================================================================== 00534F40
// EffectDamageForcePoints(int nDamage) [routine 372]: type 5Fh, flags untouched, no creator.
int __fastcall kotor_fastcall_00534f40(void*, void*, int, unsigned) {
    I damage = 0;
    if (!pop_int(&damage)) return k_pop_failed;                                          // 00534F5E..6A
    void* e = new_effect();                                                              // 00534F83..AD
    set_effect_type(e, 0x5f);                                                            // 00534FAF..B6 [007467F4]
    recovered_effect_integer_005036a0(e, nullptr, 0, static_cast<U>(damage));            // 00534FBA..CB
    return push_effect(e);                                                               // 00534FD0..1A
}

// ===================================================================================================== 00537B90
// GetEffectSpellId(effect eSpellEffect) [routine 305]: effect +1Ch.  A failed push leaks the effect (quirk).
int __fastcall kotor_fastcall_00537b90(void*, void*, int, unsigned) {
    void* e = nullptr;                                                                   // 00537B9E
    if (!pop_engine(0, &e)) return k_pop_failed;                                         // 00537B91..B5
    if (!push_int_raw(at<I>(e, 0x1c))) return k_push_failed;                             // 00537BB8..D5
    if (e != nullptr) delete_effect(e);                                                  // 00537BD8..EE
    return 0;                                                                            // 00537BF1
}

// ===================================================================================================== 0053EFA0
// GetCreatureMovmentType(object oidCreature) [routine 566]: stats(+A74h)->+194h, default 1.
int __fastcall kotor_fastcall_0053efa0(void*, void*, int, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                           // 0053EFA0..BB
    I result = 1;                                                                        // 0053EFCD
    void* creature = creature_by_id(id);                                                 // 0053EFBE..D2
    if (creature != nullptr) result = at<I>(at<void*>(creature, 0xa74), 0x194);          // 0053EFD7..E1
    return push_int(result);                                                             // 0053EFE7..03
}

// ===================================================================================================== 0052E1A0
// AdjustReputation(object oTarget, object oSourceFactionMember, int nAdjustment) [routine 209]
int __fastcall kotor_fastcall_0052e1a0(void*, void*, int, unsigned) {
    U target = k_invalid, member = k_invalid;                                            // 0052E1A9..B2
    I adjustment = 0;
    if (!pop_object(&target)) return k_pop_failed;                                       // 0052E1B6..C2 [esp+4]
    if (!pop_object(&member)) return k_pop_failed;                                       // 0052E1C4..D6 [esp]
    if (!pop_int(&adjustment)) return k_pop_failed;                                      // 0052E1D8..EA [esp+8]
    void* source = creature_by_id(member);                                               // 0052E1F7..09
    if (source != nullptr && at<U>(source, 0xf8) != 0) return 0;                         // 0052E20B..15 (source +F8h set: skip)
    void* creature = creature_by_id(target);                                             // 0052E217..2A
    if (creature != nullptr) {                                                           // 0052E22C..2E
        U faction = 0;                                                                   // [esp+10] (uninitialised in the original; written by the callee on success)
        if (kotor_fastcall_004ae950(server_app(), nullptr, member, &faction) == 1)       // 0052E230..4A ([esp+4] after push esi = member; REVIEW-VM-PACKAGES fix)
            kotor_fastcall_00514070(creature, nullptr, faction, adjustment);             // 0052E24C..58
    }
    return 0;                                                                            // 0052E25E
}

// ===================================================================================================== 00541CC0
// ResistSpell(object oCaster, object oTarget) [routine 169].
// Result: 0 = no resist / invalid, 1 = resisted (force resistance roll or immunity/absorption result 1),
// otherwise the DoSpellLevelAbsorption / DoSpellImmunity result itself (e.g. -1 or 2).
// Quirks kept: on the immunity path the first message integer is the CSWSpell pointer (the slot is reused for the
// roll only on the force-resistance path); message objects from a failed operator new are dereferenced.
int __fastcall kotor_fastcall_00541cc0(void* self, void*, int, unsigned) {
    U caster_id = 0, target_id = 0;
    if (!pop_object(&caster_id)) return k_pop_failed;                                    // 00541CD9..ED [esp+C]
    if (!pop_object(&target_id)) return k_pop_failed;                                    // 00541D06..18 [esp+8]
    I result;                                                                            // [esp+0C] after the pushes (L0)
    void* caster = game_object(caster_id);                                               // 00541D1A..2F esi
    void* target = game_object(target_id);                                               // 00541D2F..44 ebx
    if (caster == nullptr || target == nullptr) return push_int(0);                      // 00541D44..50 -> 00541EB3, 00542071..A0
    void* spell = kotor_m15b_spell_array_get_spell_0059b6d0(at<void*>(kotor_cell_rules_007a3a28, 0x8c), nullptr,
        at<I>(as_object(caster), 0x1c0));                                                // 00541D56..76
    U slot_roll = word(spell);                                                           // 00541D7A [esp+1C] = spell (L3)
    if (spell == nullptr) return push_int(0);                                            // 00541D7E / 00541EA6
    I caster_level = 0;                                                                  // ebp
    void* self_object = game_object(self_id(self));                                      // 00541D84..96
    if (self_object != nullptr && vcall<void*>(self_object, 0x50) != nullptr) {          // 00541D98..A5
        caster_level = at<I>(vcall<void*>(self_object, 0x50), 0x25c);                    // 00541DA7..B4
    } else {
        bool fallback = true;
        if (type_of(caster) == 5) {                                                      // 00541DB6..BF [00746634]
            void* c = as_creature(caster);                                               // 00541DC1..C5
            if (at<I>(c, 0x964) == 1) {                                                  // 00541DC8..CF
                caster_level = at<I>(c, 0x968); fallback = false;                        // 00541DD1
            } else if (at<U8>(c, 0x1d4) != 0xff && at<U8>(c, 0x1d4) != 0xfe) {          // 00541DD9..E7
                caster_level = static_cast<I>(recovered_stats_level_005a5fd0(at<void*>(c, 0xa74), nullptr, 0) & 0xffu); // 00541DE9..F8
                fallback = false;
            }
        }
        if (fallback) caster_level = static_cast<I>(at<U8>(spell, 0x3f)) * 2 - 1;      // 00541DFD..01
    }
    if (type_of(target) != 5) return push_int(0);                                       // 00541E05..0D -> 00541EA6
    void* target_creature = as_creature(target);                                         // 00541E13..1A edi
    result = kotor_fastcall_004d13a0(target_creature, nullptr, as_object(caster));       // 00541E1C..2E
    if (result == -1)                                                                    // 00541E2B..32
        result = kotor_fastcall_004ccfc0(target_creature, nullptr, as_object(caster));   // 00541E34..43
    I resistance = 0;                                                                    // 00541E4B ebx = 0
    if (result == -1) {                                                                  // 00541E4D..50
        if (at<U8>(spell, 0x140) != 1) return push_int(result);                          // 00541E52..5D
        resistance = static_cast<I>(static_cast<signed char>(kotor_fastcall_005a5a60(at<void*>(target_creature, 0xa74), nullptr))); // 00541E63..6E movsx
        if (resistance <= 0) return push_int(0);                                         // 00541E73..75 -> 00541E9D
        const U roll = recovered_rules_roll_dice_00550b60(kotor_cell_rules_007a3a28, nullptr, 1, 0x14) & 0xffffu; // 00541E77..86
        slot_roll = roll;                                                                // 00541E89
        if (static_cast<I>(roll) + caster_level >= resistance) return push_int(0);       // 00541E8D..91 (jge)
        result = 1;                                                                      // 00541E93
    } else if (result != 1) {                                                            // 00541EC0..C3
        return push_int(result);
    }
    // ---- feedback: message 0Ah to caster and target players, then 11h with the roll breakdown ----
    void* memory = live_operator_new(0x34);                                              // 00541ECB..D2
    void* data = memory ? recovered_message_data_construct_004d69f0(memory, nullptr) : nullptr; // 00541ED9..EC
    recovered_event_object_004d7780(data, nullptr, 0, target_id);                        // 00541EEE..FE
    recovered_event_integer_004d76b0(data, nullptr, 0, 1);                               // 00541F03..08
    void* client = kotor_fastcall_004ae910(server_app(), nullptr, caster_id);            // 00541F0D..1B
    if (client != nullptr)                                                               // 00541F20..22
        (void)recovered_cc_message_00568570(recovered_server_sws_message_004aedb0(server_app(), nullptr), nullptr,
            at<U>(client, 4), 0xa, data, 0);                                             // 00541F24..3C
    client = kotor_fastcall_004ae910(server_app(), nullptr, target_id);                  // 00541F41..4F
    if (client != nullptr)                                                               // 00541F54..56
        (void)recovered_cc_message_00568570(recovered_server_sws_message_004aedb0(server_app(), nullptr), nullptr,
            at<U>(client, 4), 0xa, data, 0);                                             // 00541F58..70
    if (data != nullptr) { recovered_message_data_destroy_004d6a40(data, nullptr); live_free(data); } // 00541F75..86
    memory = live_operator_new(0x34);                                                    // 00541F89..90
    data = memory ? recovered_message_data_construct_004d69f0(memory, nullptr) : nullptr; // 00541F97..AE
    recovered_event_object_004d7780(data, nullptr, 0, target_id);                        // 00541FB0..C0
    recovered_event_object_004d7780(data, nullptr, 1, caster_id);                        // 00541FC5..CE
    recovered_event_integer_004d76b0(data, nullptr, 0, slot_roll);                       // 00541FD3..DB
    recovered_event_integer_004d76b0(data, nullptr, 1, static_cast<U>(caster_level));    // 00541FE0..E5
    recovered_event_integer_004d76b0(data, nullptr, 2, static_cast<U>(resistance));      // 00541FEA..EF
    client = kotor_fastcall_004ae910(server_app(), nullptr, caster_id);                  // 00541FF4..02
    if (client != nullptr)                                                               // 00542007..09
        (void)recovered_cc_message_00568570(recovered_server_sws_message_004aedb0(server_app(), nullptr), nullptr,
            at<U>(client, 4), 0x11, data, 0);                                            // 0054200B..23
    client = kotor_fastcall_004ae910(server_app(), nullptr, target_id);                  // 00542028..36
    if (client != nullptr)                                                               // 0054203B..3D
        (void)recovered_cc_message_00568570(recovered_server_sws_message_004aedb0(server_app(), nullptr), nullptr,
            at<U>(client, 4), 0x11, data, 0);                                            // 0054203F..57
    if (data != nullptr) { recovered_message_data_destroy_004d6a40(data, nullptr); live_free(data); } // 0054205C..6D
    return push_int(result);                                                             // 00542070..A0
}

// =====================================================================================================
// Reconstructed callees without a callable provider in the c31 map (exact original ABI).
// =====================================================================================================

// 004D13A0 CSWSObject::DoSpellLevelAbsorption(CSWSObject* caster): thiscall, RET 4.  Always returns -1 in this
// build: it looks up the spell and its level and scans the applied effects from stats word +152h while the effect
// type is <= 41h ([00746780]), but never acts on the result (dead body kept as in the original, reads included).
I __fastcall kotor_fastcall_004d13a0(void* object, void*, void* caster) {
    if (caster == nullptr || as_creature(caster) == nullptr) return -1;                  // 004D13A5..BA
    if (at<I>(object, 0x128) <= 0) return -1;                                            // 004D13C0..C8
    void* spell = kotor_m15b_spell_array_get_spell_0059b6d0(at<void*>(kotor_cell_rules_007a3a28, 0x8c), nullptr,
        at<I>(caster, 0x1c0));                                                           // 004D13CE..E1
    if (spell == nullptr) return -1;                                                     // 004D13E6..EA
    if (at<I>(as_creature(caster), 0x964) != 1) {                                        // 004D13F0..00
        const U8 class_byte = at<U8>(caster, 0x1d4);                                     // 004D1402..0C into the arg slot
        void* stats = at<void*>(as_creature(caster), 0xa74);                             // 004D1408..17
        const U arg = (word(caster) & 0xffffff00u) | class_byte;                         // 004D1413 dword = caster ptr with AL replaced
        const U cls = kt_get_class_005a4e90(stats, nullptr, arg) & 0xffu;                // 004D141D..25
        if (cls != 0xff) (void)kotor_fastcall_0059b650(spell, nullptr, cls);             // 004D1428..32 (result unused)
    }
    if (at<U8>(spell, 0x140) != 1) return -1;                                            // 004D1437..48
    for (int pass = 0; pass < 2; ++pass) {                                               // 004D144B..91 / 004D1492..D0 (two identical scans)
        I i = static_cast<I>(at<U16>(at<void*>(as_creature(object), 0xa74), 0x152));
        const I count = at<I>(object, 0x128);
        for (; i < count; ++i)
            if (static_cast<I>(at<U16>(at<void**>(object, 0x124)[i], 8)) > 0x41) break;
    }
    if (as_creature(object) != nullptr) (void)as_creature(caster);                       // 004D14D2..E2
    return -1;                                                                           // 004D14E5..EB
}

// 004CCFC0 CSWSObject::DoSpellImmunity(CSWSObject* caster): thiscall, RET 4.  Scans the applied effects from stats
// word +154h while type <= 32h ([00746744]); an effect of type 32h whose integer 0 is the caster's spell (+1C0h) or
// -1 makes the object immune: feedback message 44h (integer 1 = spell, object 0 = this id) is sent to this creature
// and a copy to the caster creature (each deleted instead when not a creature); returns 2.  Otherwise -1.
I __fastcall kotor_fastcall_004ccfc0(void* object, void*, void* caster) {
    const U spell = at<U>(caster, 0x1c0);                                                // 004CCFD6..DC ebp
    if (as_creature(object) == nullptr) return -1;                                       // 004CCFE5..ED
    if (at<I>(object, 0x128) <= 0) return -1;                                            // 004CCFEF..F7
    I i = static_cast<I>(at<U16>(at<void*>(as_creature(object), 0xa74), 0x154));        // 004CCFF9..06
    bool immune = false;
    for (; i < at<I>(object, 0x128); ++i) {                                              // 004CD00D..62 (signed)
        void* e = at<void**>(object, 0x124)[i];                                          // 004CD020..26
        if (static_cast<I>(at<U16>(e, 8)) > 0x32) break;                                 // 004CD029..2F (jg)
        if (at<U16>(e, 8) == 0x32) {                                                     // 004CD031..37
            if (recovered_effect_integer_00503690(e, nullptr, 0) == spell ||             // 004CD039..44
                recovered_effect_integer_00503690(e, nullptr, 0) == 0xffffffffu) { immune = true; break; } // 004CD046..52
        }
    }
    if (!immune) return -1;                                                              // 004CD064
    void* memory = live_operator_new(0x34);                                              // 004CD07C..83
    void* mine = memory ? recovered_message_data_construct_004d69f0(memory, nullptr) : nullptr; // 004CD08C..9F edi
    memory = live_operator_new(0x34);                                                    // 004CD0A1..B0
    void* copy = memory ? recovered_message_data_construct_004d69f0(memory, nullptr) : nullptr; // 004CD0B7..CA ebx
    recovered_event_integer_004d76b0(mine, nullptr, 1, spell);                           // 004CD0CC..D9
    recovered_event_object_004d7780(mine, nullptr, 0, at<U>(object, 4));                 // 004CD0DE..E6
    (void)kotor_fastcall_004d6ee0(mine, nullptr, copy);                                  // 004CD0EB..EE CopyTo
    if (as_creature(object) != nullptr)                                                  // 004CD0F3..FC
        recovered_creature_feedback_004ede10(as_creature(object), nullptr, 0x44, mine);  // 004CD0FE..0A
    else if (mine != nullptr) { recovered_message_data_destroy_004d6a40(mine, nullptr); live_free(mine); } // 004CD111..22
    if (as_creature(caster) != nullptr) {                                                // 004CD125..32
        recovered_creature_feedback_004ede10(as_creature(caster), nullptr, 0x44, copy);  // 004CD134..40
        return 2;                                                                        // 004CD145
    }
    if (copy != nullptr) { recovered_message_data_destroy_004d6a40(copy, nullptr); live_free(copy); } // 004CD14F..60
    return 2;                                                                            // 004CD163
}

// 00514070 CSWSCreature::AdjustReputation(int faction, int delta): thiscall, RET 8.  For an NPC faction other than
// the creature's own (stats +78h): reputation(own -> faction) += delta through the faction manager [[server+4]+10054h].
void __fastcall kotor_fastcall_00514070(void* creature, void*, U faction, I delta) {
    void* manager = at<void*>(at<void*>(server_app(), 4), 0x10054);                      // 00514070..84
    if (recovered_faction_is_npc_0052b280(manager, nullptr, faction) == 0) return;       // 0051408A..94
    const I reputation = recovered_faction_npc_reputation_0052b420(manager, nullptr,
        at<U>(at<void*>(creature, 0xa74), 0x78), faction);                               // 00514096..A3
    const U own = at<U>(at<void*>(creature, 0xa74), 0x78);                               // 005140A8..AE (re-read)
    if (own == faction) return;                                                          // 005140B1..B3
    kotor_fastcall_0052b3d0(manager, nullptr, own, faction, reputation + delta);         // 005140B5..C0
}

// 0052B3D0 CFactionManager::SetNPCFactionReputation(int own, int other, int value): thiscall, RET 0Ch.
// other in [1, count] and own in [0, count] (count +1Ch, signed); value clamped to [0, 100]; stored at
// table(+20h)[count*own + other - 1] when that index is below the table size (signed).
void __fastcall kotor_fastcall_0052b3d0(void* manager, void*, U own, U other, I value) {
    const I o = static_cast<I>(other);                                                   // 0052B3D1 edi
    const I count = at<I>(manager, 0x1c);                                                // 0052B3DA
    if (o < 1 || o > count) return;                                                      // 0052B3D5..DF
    const I s = static_cast<I>(own);                                                     // 0052B3E2 esi
    if (s < 0 || s > count) return;                                                      // 0052B3E6..EC
    if (value > 0x64) value = 0x64; else if (value < 0) value = 0;                       // 0052B3EE..02
    void* table = at<void*>(manager, 0x20);                                              // 0052B404
    const I index = static_cast<I>(static_cast<U>(count) * static_cast<U>(s) + static_cast<U>(o) - 1u); // 0052B407..0D
    if (index < at<I>(table, 4)) at<I*>(table, 0)[index] = value;                        // 0052B40A..17
}

}  // extern "C"
