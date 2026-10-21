// Lane endar-actions, group "queries": NWScript engine-command handlers
// (CSWVirtualMachineCommands::ExecuteCommand*) reconstructed from the original swkotor.exe (read-only, capstone).
// Every handler: original int __thiscall (int routine, int argc), RET 8 -> extern "C" __fastcall @16.
// Addresses in comments are original instruction ranges.  Conventions: claude_vm_actions_common.hpp.
// .rdata facts used as literals: [0074666C] byte 0 = CGameObjectArray "found" status, [00746633] byte 4 =
// object type area (types above it are CSWSObjects), [00746644] 7F000000h = OBJECT_INVALID,
// [00742EF8] float 57.29578 (deg/rad), [0073D700] float 0.0, [00741868] float 360.0.
#include "061872cb05-claude_vm_actions_common.hpp"

namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); }  // CExoString() [menu20_bindings]

extern "C" {
// ---- existing providers (c17 map; object in brackets) ----
unsigned __fastcall w1x_creature_skill_rank_005aa570(void* stats, void*, U skill, U versus, U base_only); // CSWSCreatureStats::GetSkillRank (AL) [0886]
unsigned __fastcall kt_feat_remaining_uses_005a6680(void* stats, void*, U feat);                         // CSWSCreatureStats::GetFeatRemainingUses (AL) [provider_adapters]
void __fastcall w2x_stats_set_experience_005af480(void* stats, void*, unsigned xp, unsigned flag);       // CSWSCreatureStats::SetExperience [0886]
U __fastcall kotor_m15a_creature_get_equipped_item_id_0060ca80(void* creature, void*, U slot);          // CSWCCreature::GetEquippedItemID [0816]
void* __fastcall kotor_m15_client_get_item_by_id_005ed5a0(void* client, void*, U id);                   // CClientExoApp::GetItemByGameObjectID [0813]
void __fastcall kotor_m15a_item_power_item_00647130(void* item, void*, U powered, U a, U transition, U b); // CSWCItem::PowerItem, RET 10h [0817]
// ---- reconstructed in this file (section at the end) ----
void* __fastcall kotor_fastcall_004ca7a0(void* location, void*);
void* __fastcall kotor_fastcall_004cb7b0(void* object, void*, void* out);
I __fastcall kotor_fastcall_004cafe0(void* object, void*, U flags);
void __fastcall kotor_fastcall_0060fb80(void* creature, void*, U override_state, U powered, U transition);
}

namespace {
using namespace vmx;
constexpr U k_type_area_00746633 = 4;
constexpr float k_deg_per_rad_00742ef8 = 57.29578f;               // .rdata 00742EF8 (42652EE1h)
inline U type_of(void* o) { return at<U8>(o, 8); }
inline void* as_object(void* o) { return vcall<void*>(o, 0x10); }      // vtbl+10 AsSWSObject
inline void* as_door(void* o) { return vcall<void*>(o, 0x18); }        // vtbl+18 AsSWSDoor
inline void* as_module(void* o) { return vcall<void*>(o, 0x20); }      // vtbl+20 AsSWSModule
inline void* as_area(void* o) { return vcall<void*>(o, 0x28); }        // vtbl+28 AsSWSArea
inline void* as_creature(void* o) { return vcall<void*>(o, 0x30); }    // vtbl+30 AsSWSCreature
inline void* as_trigger(void* o) { return vcall<void*>(o, 0x40); }     // vtbl+40 AsSWSTrigger
inline void* as_placeable(void* o) { return vcall<void*>(o, 0x48); }   // vtbl+48 AsSWSPlaceable
inline void* as_encounter(void* o) { return vcall<void*>(o, 0x50); }   // vtbl+50 (type 11)
inline void* as_type13(void* o) { return vcall<void*>(o, 0x5c); }      // vtbl+5C (type 13)
// neg/sbb/and 7D0h/add F830h
inline I push_status(U pushed) { return pushed ? 0 : k_push_failed; }
inline U pop_object(U* id) { return recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, id); }
inline U pop_int(I* v) { return recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, v); }
inline I push_object(U id) { return push_status(recovered_vm_outer_push_object_005d10d0(vm(), nullptr, id)); }
inline I push_int(I v) { return push_status(recovered_vm_outer_push_integer_005d1010(vm(), nullptr, v)); }
inline I push_float(float v) { return push_status(recovered_vm_outer_push_float_005d1030(vm(), nullptr, v)); }
inline void* creature_by_id(U id) { return kotor_fastcall_004ae770(server_app(), nullptr, id); }
inline float fsub(float a, float b) { return static_cast<float>(static_cast<double>(a) - static_cast<double>(b)); }
}  // namespace

extern "C" {

// ================================================================== 005366A0 GetArea(object) [routine 24]
int __fastcall recovered_vm_get_area_005366a0(void*, void*, int, unsigned) {
    ExoString unused;                                                                  // 005366B9..BD local CExoString (unused)
    (void)kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(&unused), nullptr);
    U result = k_invalid;                                                              // 005366C8 esi = [746644]
    U id = 0;
    if (!pop_object(&id)) { exo_destroy(&unused); return k_pop_failed; }               // 005366CE..F5
    void* object = nullptr;
    if (lookup(id, &object) && object) {                                               // 0053670C..39
        if (type_of(object) > k_type_area_00746633) {                                  // 0053673B..46 (jbe: unsigned)
            void* area = recovered_object_area_004cb120(object, nullptr);              // 00536748 CSWSObject::GetArea
            if (area) result = at<U>(area, 0x120);                                     // 0053674D..51
        } else if (type_of(object) == k_type_area_00746633) {                          // 00536759
            result = at<U>(object, 4);                                                 // 0053675B area object id
        }
    }
    const I status = push_object(result);                                              // 0053675E..65
    exo_destroy(&unused);                                                              // 0053677A / 00536796
    return status;
}

// ================================================================== 0053B640 GetExitingObject() [routine 26]
// Uses the commands object's run-script caller: +4 = caller valid flag (== 1), +8 = caller object id.
int __fastcall recovered_vm_get_exiting_object_0053b640(void* self, void*, int, unsigned) {
    U result = k_invalid;                                                              // 0053B648
    void* object = nullptr;
    if (at<U>(self, 4) == 1 && lookup(at<U>(self, 8), &object)) {                      // 0053B643..78
        switch (type_of(object)) {                                                     // 0053B67E..91 byte table 0053B724
        case 3: { void* m = as_module(object); if (m) result = at<U>(m, 0x150); break; }      // 0053B6BA..C3
        case 4: { void* a = as_area(object); if (a) result = at<U>(a, 0x1d8); break; }        // 0053B6CB..D4
        case 7: { void* t = as_trigger(object); if (t) result = at<U>(t, 0x2a0); break; }     // 0053B698..A1
        case 11: { void* e = as_encounter(object); if (e) result = at<U>(e, 0x254); break; }  // 0053B6A9..B2
        case 13: { void* x = as_type13(object); if (x) result = at<U>(x, 0x2e4); break; }     // 0053B6DC..E5
        default: break;                                                                // table entry 5 -> 0053B6EB
        }
    }
    return push_object(result);                                                        // 0053B6EB..07
}

// ================================================================== 00537FE0 GetFacing(object) [routine 28]
// Quirk (original): the heading is atan2 of the raw orientation (x, y) while the 360-degree wrap tests the
// y of the normalized copy; z of the copy is zeroed before normalizing.
int __fastcall recovered_vm_get_facing_00537fe0(void*, void*, int, unsigned) {
    float result = -1.0f;                                                              // 00537FEE 0BF800000h
    Vector in = {0.0f, 0.0f, 0.0f};                                                    // 00537FF6..06
    Vector out;
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 0053800E..1F
    void* object = nullptr;
    if (lookup(id, &object) && object && type_of(object) > k_type_area_00746633) {     // 00538022..5A
        copy_vector(&in, static_cast<unsigned char*>(object) + 0x9c);                  // 0053805C..7A orientation
        in.z = 0.0f;                                                                   // 0053807F
        normalize_004ab180(&out, &in);                                                 // 00538087
        const float y = in.y, x = in.x, deg = k_deg_per_rad_00742ef8;
        __asm {
            fld dword ptr [y]                                                          // 0053808C
            fld dword ptr [x]                                                          // 00538090
            fpatan                                                                     // 00538097 atan2(y, x)
            fmul dword ptr [deg]                                                       // 00538099
            fstp dword ptr [result]                                                    // 0053809F
        }
        if (out.y < 0.0f)                                                              // 005380A3..B2 (C0 only; NaN skips)
            result = static_cast<float>(static_cast<double>(result) + 360.0);          // 005380B4..BE
    }
    return push_float(result);                                                         // 005380C2..E3
}

// ================================================================== 0053B840 GetLocation(object) [routine 213]
int __fastcall recovered_vm_get_location_0053b840(void*, void*, int, unsigned) {
    unsigned char location[0x18];
    kotor_fastcall_004ca7a0(location, nullptr);                                        // 0053B858..5C CScriptLocation()
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 0053B861..A1 (dtor 004015A0 is a no-op)
    void* object = nullptr;
    if (lookup(id, &object) && object && type_of(object) > k_type_area_00746633) {     // 0053B8A4..DC
        unsigned char temp[0x18];
        void* got = kotor_fastcall_004cb7b0(object, nullptr, temp);                    // 0053B8E0..E5 GetScriptLocation
        std::memcpy(location, got, 0x18);                                              // 0053B8EA..F5 rep movsd 6
    }
    return push_status(recovered_vm_outer_push_engine_structure_005d10b0(vm(), nullptr, 2, location)); // 0053B902..54 type 2
}

// ================================================================== 00537370 GetDistanceBetween(a, b) [routine 151]
// Quirk (original): found objects are not null-checked (status-only test).
int __fastcall recovered_vm_get_distance_between_00537370(void*, void*, int, unsigned) {
    float result = 0.0f;                                                               // 0053737C
    void* array = object_array();                                                      // 00537373..94 (fetched before the pops)
    U a = 0, b = 0;
    if (!pop_object(&a)) return k_pop_failed;                                          // 00537389..9D
    if (!pop_object(&b)) return k_pop_failed;                                          // 005373A3..B5
    void* oa = nullptr; void* ob = nullptr;
    if (static_cast<unsigned char>(kotor_fastcall_004d8230(array, nullptr, a, &oa)) == 0 &&   // 005373BB..D2
        static_cast<unsigned char>(kotor_fastcall_004d8230(array, nullptr, b, &ob)) == 0) {   // 005373D4..EB
        Vector d;
        d.z = fsub(at<float>(oa, 0x98), at<float>(ob, 0x98));                          // 005373F1..FB
        d.y = fsub(at<float>(oa, 0x94), at<float>(ob, 0x94));                          // 00537401..07
        d.x = fsub(at<float>(oa, 0x90), at<float>(ob, 0x90));                          // 0053740D..26
        result = static_cast<float>(kotor_cdecl_004ab100(&d));                         // 0053742A..2F vector_magnitude
    }
    return push_float(result);                                                         // 00537436..58
}

// ================================================================== 00537470 GetDistanceBetween2D(a, b) [routine 319]
int __fastcall recovered_vm_get_distance_between_2d_00537470(void*, void*, int, unsigned) {
    float result = 0.0f;                                                               // 0053747C
    void* array = object_array();                                                      // 00537473..94
    U a = 0, b = 0;
    if (!pop_object(&a)) return k_pop_failed;                                          // 00537489..9D
    if (!pop_object(&b)) return k_pop_failed;                                          // 005374A3..B5
    void* oa = nullptr; void* ob = nullptr;
    if (static_cast<unsigned char>(kotor_fastcall_004d8230(array, nullptr, a, &oa)) == 0 &&   // 005374BB..D2
        static_cast<unsigned char>(kotor_fastcall_004d8230(array, nullptr, b, &ob)) == 0) {   // 005374D8..EF
        Vector pa, pb;
        copy_vector(&pa, static_cast<unsigned char*>(oa) + 0x90);                      // 005374F1..23
        copy_vector(&pb, static_cast<unsigned char*>(ob) + 0x90);                      // 0053750B..2A
        Vector d;
        d.x = fsub(pa.x, pb.x);                                                        // 00537507..2D
        d.y = fsub(pa.y, pb.y);                                                        // 00537531..46
        d.z = 0.0f;                                                                    // 0053754A
        result = static_cast<float>(kotor_cdecl_004ab100(&d));                         // 00537552..57
    }
    return push_float(result);                                                         // 0053755E..80
}

// ================================================================== 0053B170 GetLastOpenedBy() [routine 376]
int __fastcall recovered_vm_get_last_opened_by_0053b170(void* self, void*, int, unsigned) {
    void* object = game_object(at<U>(self, 8));                                        // 0053B170..81 caller id
    U result = k_invalid;                                                              // 0053B18A
    if (object) {
        if (as_placeable(object)) result = at<U>(as_placeable(object), 0x350);         // 0053B191..A3
        else if (as_door(object)) result = at<U>(as_door(object), 0x31c);              // 0053B1AB..BB
    }
    return push_object(result);                                                        // 0053B1C1..DD
}

// ================================================================== 0053B570 GetLastUsedBy() [routine 330]
// Quirk (original): the caller's game object is not null-checked before its vtable is read.
int __fastcall recovered_vm_get_last_used_by_0053b570(void* self, void*, int, unsigned) {
    void* object = game_object(at<U>(self, 8));                                        // 0053B570..81
    U result = k_invalid;                                                              // 0053B58C
    if (as_placeable(object)) result = at<U>(as_placeable(object), 0x358);             // 0053B588..9F
    else if (as_door(object)) result = at<U>(as_door(object), 0x31c);                  // 0053B5A7..B7
    return push_object(result);                                                        // 0053B5BD..D9
}

// ================================================================== 00539E20 GetIsOpen(object) [routine 443]
int __fastcall recovered_vm_get_is_open_00539e20(void*, void*, int, unsigned) {
    U id = k_invalid;                                                                  // 00539E2D
    I result = 0;                                                                      // 00539E35
    if (!pop_object(&id)) return k_pop_failed;                                         // 00539E37..47
    void* object = game_object(id);                                                    // 00539E4A..59
    if (object) {
        if (as_door(object)) result = at<U8>(as_door(object), 0x2cc) != 0 ? 1 : 0;     // 00539E64..83
        else if (as_placeable(object))
            result = at<U>(as_placeable(object), 0xd4) == 0x275b ? 1 : 0;              // 00539E85..A4
    }
    return push_int(result);                                                           // 00539EA6..C3
}

// ================================================================== 0053EEC0 GetIsInConversation(object) [routine 445]
int __fastcall recovered_vm_get_is_in_conversation_0053eec0(void*, void*, int, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 0053EEC0..DB
    I result = 0;                                                                      // 0053EF42
    void* object = game_object(id);                                                    // 0053EEDE..EE
    if (object && as_object(object)) {                                                 // 0053EEF5..02
        const U partner = at<U>(as_object(object), 0x54);                              // 0053EF04..0B conversation object
        void* other = game_object(partner);                                            // 0053EF0E..17
        if (other && as_object(other))                                                 // 0053EF1E..2B
            result = at<U>(as_object(other), 0x54) == partner ? 1 : 0;                 // 0053EF2D..3E
    }
    return push_int(result);                                                           // 0053EF44..61
}

// ================================================================== 00539D80 GetIsInCombat(object) [routine 320]
int __fastcall recovered_vm_get_is_in_combat_00539d80(void*, void*, int, unsigned) {
    I result = 0;                                                                      // 00539D8D
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 00539D8F..9F
    void* creature = creature_by_id(id);                                               // 00539DA2..B0
    if (creature) result = at<I>(creature, 0x4e0);                                     // 00539DB5..B9
    return push_int(result);                                                           // 00539DBF..DB
}

// ================================================================== 0053D5A0 GetGender(object) [routine 358]
int __fastcall recovered_vm_get_gender_0053d5a0(void*, void*, int routine, unsigned) {
    I result = -1;                                                                     // 0053D5AD
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 0053D5B0..C0
    void* creature = creature_by_id(id);                                               // 0053D5C3..D1
    if (creature && routine == 0x166)                                                  // 0053D5D6..E2
        result = at<U8>(at<void*>(creature, 0xa74), 0x64);                             // 0053D5E4..EA stats gender
    return push_int(result);                                                           // 0053D5EE..0A
}

// ================================================================== 0053D050 GetSkillRank(int skill, object) [routine 315]
// Quirk (original): a found object is not null-checked before its vtable is read.
int __fastcall recovered_vm_get_skill_rank_0053d050(void*, void*, int, unsigned) {
    I skill = 0; U id = 0;
    if (!pop_int(&skill)) return k_pop_failed;                                         // 0053D050..6F
    if (!pop_object(&id)) return k_pop_failed;                                         // 0053D072..84
    I result = -1;                                                                     // 0053D09A
    void* object = nullptr;
    if (lookup(id, &object) && as_creature(object)) {                                  // 0053D086..BC
        void* stats = at<void*>(as_creature(object), 0xa74);                           // 0053D0BE..CB
        result = static_cast<signed char>(w1x_creature_skill_rank_005aa570(stats, nullptr, static_cast<U>(skill), 0, 0)); // 0053D0D1..DD movsx
    }
    return push_int(result);                                                           // 0053D0E0..FE
}

// ================================================================== 00539170 GetHasFeat(int feat, object) [routine 285]
// Quirk (original): a found object is not null-checked before its vtable is read.
int __fastcall recovered_vm_get_has_feat_00539170(void*, void*, int, unsigned) {
    I result = 0;                                                                      // 0053917F
    I feat = 0; U id = 0;
    if (!pop_int(&feat)) return k_pop_failed;                                          // 00539181..93
    if (!pop_object(&id)) return k_pop_failed;                                         // 00539196..A8
    void* object = nullptr;
    if (lookup(id, &object) && as_creature(object)) {                                  // 005391AA..DC
        void* stats = at<void*>(as_creature(object), 0xa74);                           // 005391DE..EB
        const U8 uses = static_cast<U8>(kt_feat_remaining_uses_005a6680(stats, nullptr, static_cast<U>(feat))); // 005391F1..F4
        result = uses > 0 ? 1 : 0;                                                     // 005391F9..FF (unsigned 0 < al)
    }
    return push_int(result);                                                           // 00539201..1F
}

// ================================================================== 0053E600 GetXP(object) [routine 395]
int __fastcall recovered_vm_get_xp_0053e600(void*, void*, int, unsigned) {
    I result = 0;                                                                      // 0053E60F
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 0053E611..23
    void* object = nullptr;
    if (lookup(id, &object) && object) {                                               // 0053E626..53
        void* creature = as_creature(object);                                          // 0053E655..57
        if (creature) result = at<I>(at<void*>(creature, 0xa74), 0x68);                // 0053E65A..64
    }
    return push_int(result);                                                           // 0053E667..85
}

// ================================================================== 005437B0 SetXP(object, int xp) [routine 394]
int __fastcall recovered_vm_set_xp_005437b0(void*, void*, int, unsigned) {
    U id = 0; I xp = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 005437B0..C5
    if (!pop_int(&xp)) return k_pop_failed;                                            // 005437C7..E3
    if (xp >= 0) {                                                                     // 005437E6..EC (jl: signed)
        void* object = nullptr;
        if (lookup(id, &object) && object) {                                           // 005437EE..1E
            void* creature = as_creature(object);                                      // 00543820..22
            if (creature)
                w2x_stats_set_experience_005af480(at<void*>(creature, 0xa74), nullptr, static_cast<unsigned>(xp), 1); // 00543829..32
        }
    }
    return 0;                                                                          // 00543838
}

// ================================================================== 0053E120 GetTotalDamageDealt() [routine 345]
// Original bug (kept): when the caller id resolves to no CSWSObject, ebx is loaded from the stack slot that
// holds the entry ECX (the commands object), so the per-flag damage table is read from `self + 168h`.
int __fastcall recovered_vm_get_total_damage_dealt_0053e120(void* self, void*, int, unsigned) {
    I total = 0;                                                                       // 0053E133
    void* object = game_object(at<U>(self, 8));                                        // 0053E123..35
    void* target = (object && as_object(object)) ? as_object(object) : self;           // 0053E13A..56
    if (target) {                                                                      // 0053E15A
        for (U i = 0; i < 0xf; ++i) {                                                  // 0053E15F..84
            const U flag = 1u << i;                                                    // 0053E161..68
            if (kotor_fastcall_004cafe0(target, nullptr, flag) > 0)                    // 0053E16A..74 (jle: signed)
                total += kotor_fastcall_004cafe0(target, nullptr, flag);               // 0053E176..7E
        }
    }
    return push_int(total);                                                            // 0053E187..A5
}

// ================================================================== 0053AAD0 GetLastAttackResult(object) [routine 725]
int __fastcall recovered_vm_get_last_attack_result_0053aad0(void*, void*, int, unsigned) {
    U id = k_invalid; I result = 0;                                                    // 0053AADD..E5
    if (!pop_object(&id)) return k_pop_failed;                                         // 0053AAE7..F7
    void* creature = creature_by_id(id);                                               // 0053AAFA..08
    if (creature) result = at<I>(creature, 0x55c);                                     // 0053AB0D..11
    return push_int(result);                                                           // 0053AB17..33
}

// ================================================================== 0053AB40 GetWasForcePowerSuccessful(object) [routine 726]
int __fastcall recovered_vm_get_was_force_power_successful_0053ab40(void*, void*, int, unsigned) {
    U id = k_invalid; I result = 0;                                                    // 0053AB4D..55
    if (!pop_object(&id)) return k_pop_failed;                                         // 0053AB57..67
    void* creature = creature_by_id(id);                                               // 0053AB6A..78
    if (creature) result = at<I>(creature, 0x564);                                     // 0053AB7D..81
    return push_int(result);                                                           // 0053AB87..A3
}

// ================================================================== 00546E00 GetBlockingCreature(object) [routine 490]
int __fastcall recovered_vm_get_blocking_creature_00546e00(void*, void*, int, unsigned) {
    U result = k_invalid;                                                              // 00546E0D
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 00546E12..22
    void* creature = creature_by_id(id);                                               // 00546E25..33
    if (creature) result = at<U>(at<void*>(creature, 0x340), 0x254);                   // 00546E38..42
    return push_object(result);                                                        // 00546E48..64
}

// ================================================================== 00546E70 GetFoundEnemyCreature(object) [routine 495]
int __fastcall recovered_vm_get_found_enemy_creature_00546e70(void*, void*, int, unsigned) {
    U result = k_invalid;                                                              // 00546E7D
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 00546E82..92
    void* creature = creature_by_id(id);                                               // 00546E95..A3
    if (creature) result = at<U>(at<void*>(creature, 0x340), 0x264);                   // 00546EA8..B2
    return push_object(result);                                                        // 00546EB8..D4
}

// ================================================================== 00536950 GetSpellTarget(object) [routine 752]
int __fastcall recovered_vm_get_spell_target_00536950(void*, void*, int, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 00536950..6B
    U result = k_invalid;                                                              // 0053697D
    void* creature = creature_by_id(id);                                               // 0053696E..82
    if (creature) result = at<U>(creature, 0x528);                                     // 00536987..8B
    return push_object(result);                                                        // 00536991..AD
}

// ================================================================== 005433D0 SetPlotFlag(object, int flag) [routine 456]
int __fastcall recovered_vm_set_plot_flag_005433d0(void*, void*, int, unsigned) {
    U id = 0; I flag = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 005433D0..E5
    if (!pop_int(&flag)) return k_pop_failed;                                          // 005433E7..03
    void* object = game_object(id);                                                    // 00543406..15
    if (object && as_object(object)) at<I>(as_object(object), 0xf8) = flag;            // 0054341A..36
    return 0;                                                                          // 0054343C
}

// ================================================================== 00542F10 SetLightsaberPowered(object, override, powered, transition) [routine 421]
int __fastcall recovered_vm_set_lightsaber_powered_00542f10(void*, void*, int, unsigned) {
    U id = 0; I override_state = 0, powered = 0, transition = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 00542F19..25
    if (!pop_int(&override_state)) return k_pop_failed;                                // 00542F27..39 [esp+C]
    if (!pop_int(&powered)) return k_pop_failed;                                       // 00542F3B..4D [esp+8]
    if (!pop_int(&transition)) return k_pop_failed;                                    // 00542F4F..61 [esp+4]
    void* client = client_app();                                                       // 00542F6E..79
    const U client_id = kotor_fastcall_005eda50(client, nullptr, id);                  // 00542F7C..7F ServerToClientObjectId
    void* creature = reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_R4_005ed5b0(client, nullptr, client_id))); // 00542F84..87
    if (creature)                                                                      // 00542F8C..8F
        kotor_fastcall_0060fb80(creature, nullptr, static_cast<U>(override_state), static_cast<U>(powered), static_cast<U>(transition)); // 00542F91..A2
    return 0;                                                                          // 00542FA7
}

// =====================================================================================================
// Reconstructed callees without a callable provider in the c17 map (shared: exact original ABI).
// =====================================================================================================

// 004CA7A0 CScriptLocation::CScriptLocation(): thiscall, RET; zeroes the six dwords, returns this
void* __fastcall kotor_fastcall_004ca7a0(void* location, void*) {
    U* p = static_cast<U*>(location);
    p[0] = 0; p[1] = 0; p[2] = 0; p[3] = 0; p[4] = 0; p[5] = 0;                         // 004CA7A2..B2
    return location;                                                                   // 004CA7A0 eax = this
}

// 004CB7B0 CSWSObject::GetScriptLocation(CScriptLocation* out): thiscall, RET 4; returns out.
// Location = position (+90h) then orientation (+9Ch), copied through a stack temporary.
void* __fastcall kotor_fastcall_004cb7b0(void* object, void*, void* out) {
    U temp[6];
    std::memcpy(&temp[0], static_cast<unsigned char*>(object) + 0x90, 12);             // 004CB7B3..C2
    std::memcpy(&temp[3], static_cast<unsigned char*>(object) + 0x9c, 12);             // 004CB7C5..F4
    std::memcpy(out, temp, 0x18);                                                      // 004CB7E9..F8 rep movsd 6
    return out;
}

// 004CAFE0 CSWSObject::GetLastDamageAmountByFlags(int flags): thiscall, RET 4.  Finds the bit index
// i in [0,15) with (1 << i) == flags; returns ((int*)[this+168h])[i], else 0.
I __fastcall kotor_fastcall_004cafe0(void* object, void*, U flags) {
    for (U i = 0; i < 0xf; ++i)                                                        // 004CAFE6..FF
        if ((1u << i) == flags)                                                        // 004CAFF0..F9
            return at<I>(at<void*>(object, 0x168), i * 4);                             // 004CB007..0D
    return 0;                                                                          // 004CB001
}

// 0060FB80 CSWCCreature::SetOverrideLightsaberState(override, powered, transition): thiscall, RET 0Ch.
// When override is set, powers the items equipped in slots 10h and 20h; +3D8h = override always.
void __fastcall kotor_fastcall_0060fb80(void* creature, void*, U override_state, U powered, U transition) {
    if (override_state != 0) {                                                         // 0060FB82..8C
        void* client = client_app();                                                   // 0060FB8E..93
        void* item = kotor_m15_client_get_item_by_id_005ed5a0(client, nullptr,
            kotor_m15a_creature_get_equipped_item_id_0060ca80(creature, nullptr, 0x10)); // 0060FB96..A0
        if (item) kotor_m15a_item_power_item_00647130(item, nullptr, powered, 1, transition, 0); // 0060FBA5..B9
        client = client_app();                                                         // 0060FBBE..C4
        item = kotor_m15_client_get_item_by_id_005ed5a0(client, nullptr,
            kotor_m15a_creature_get_equipped_item_id_0060ca80(creature, nullptr, 0x20)); // 0060FBC7..D3
        if (item) kotor_m15a_item_power_item_00647130(item, nullptr, powered, 1, transition, 0); // 0060FBD8..E4
    }
    at<U>(creature, 0x3d8) = override_state;                                           // 0060FBEA
}

}  // extern "C"
