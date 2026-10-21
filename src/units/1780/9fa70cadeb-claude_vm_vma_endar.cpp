// Lane vmA, group "endar": Endar-used NWScript engine-command handlers (CSWVirtualMachineCommands::ExecuteCommand*)
// reconstructed from the original swkotor.exe (read-only, capstone linear sweep).  Addresses in comments are the
// original instructions.  Conventions: claude_vm_vmA_common.hpp.
// .rdata literals: [00746644] 7F000000h OBJECT_INVALID, [0074666C] 0 "found", [00746633] 4 type area,
// [007467C0] word 51h effect type Appear, [007415F8] float 0.017453292 (3C8EFA35h, deg->rad),
// [0074C0AC] "UTW ", [0074C084] "Waypoint template %s doesn't exist.\n".
#include "520143efd3-claude_vm_vma_common.hpp"
// CreateObject 0052F820 lives in claude_vm_vmA_pending.cpp (pending on the CSWSStore class, lane "aoe").

namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); }  // CExoString() [menu20_bindings]
namespace menu_resources { struct Ref { Ref* __thiscall construct_00406d80(const char* text); }; }     // CResRef(char*) [0030]
namespace kotor_reconstruction { struct StartupString; void __cdecl StartupFormat005e5680(StartupString*, const char*, ...); } // CExoString::Format [0125]

extern "C" {
// ---- existing providers (c31 map; object in brackets; RET checked against the original) ----
void* __fastcall server_get_module_004ae6b0(void* server, void*);                                 // CServerExoApp::GetModule [0530]
void* __fastcall module_get_area_004c30f0(void* module, void*);                                   // CSWSModule::GetArea RET 0 [0530]
void* __fastcall recovered_item_construct_005530a0(void* self, void*, U id);                      // CSWSItem::CSWSItem RET 4 [0963]
U __fastcall recovered_item_template_005608b0(void* item, void*, void* resref);                   // CSWSItem::LoadFromTemplate RET 4 [0995]
void __fastcall recovered_item_add_to_area_00555720(void* item, void*, void* area, float x, float y, float z, U flag); // CSWSItem::AddToArea RET 14h [W1 b7]
void* __fastcall recovered_server_creature_construct_004f7a10(void* memory, void*, U id, U flag); // CSWSCreature() RET 8 [0963]
unsigned __fastcall w2x_creature_template_005026d0(void* creature, void*, void* ref, unsigned flag); // CSWSCreature::LoadFromTemplate RET 8 [0886]
unsigned __fastcall w1x_area_safe_location_004be860(void* area, void*, unsigned a, unsigned b, unsigned c, unsigned d, unsigned e); // ComputeSafeLocation RET 14h [0886]
void __fastcall recovered_effect_apply_004d14f0(void* object, void*, void* effect, U a, U b);     // CSWSObject::ApplyEffect RET 0Ch [0886]
void __fastcall recovered_creature_placement_004fa100(void* creature, void*, U area, U x, U y, U z, U a, U b); // CSWSCreature::AddToArea RET 18h [1060]
void* __fastcall recovered_placeable_construct_005877e0(void* self, void*, unsigned id);          // CSWSPlaceable() RET 4 [0963]
void __fastcall recovered_placeable_rotation_005853c0(void* self, void*, void* quaternion);       // CSWSPlaceable::SetOrientation RET 4 [0886]
void __fastcall kotor_fastcall_00587170(void* placeable, void*, void* area, void* position, U flag); // CSWSPlaceable::AddToArea RET 0Ch [0886 claude exports]
void* __fastcall recovered_waypoint_construct_005c7e70(void* self, void*, unsigned id);           // CSWSWaypoint() RET 4 [0963]
void __fastcall recovered_waypoint_place_005c8580(void* wp, void*, void* area, U x, U y, U z, U flag); // CSWSWaypoint::AddToArea RET 14h [1134]
unsigned __fastcall recovered_stats_level_005a5fd0(void* stats, void*, U flag);                   // CSWSCreatureStats::GetLevel (AL) RET 4 [0963]
U __fastcall recovered_stats_has_feat_005a6630(void* stats, void*, U feat);                       // CSWSCreatureStats::HasFeat RET 4 [0963]
U __fastcall kotor_fastcall_005a5330(void* stats, void*, U flag);                                 // CSWSCreatureStats::GetDEXMod (AL) RET 4 [claude_combat_attack_attackstats]
void __fastcall recovered_event_integer_004d76b0(void* event, void*, U index, U value);           // CScriptEvent::SetInteger RET 8 [1017]
// GFF
void* __fastcall gui_load_gff_construct_00410630(void* gff, void*, unsigned type, const char* ext, const void* ref); // CResGFF(type, ext, ref) RET 0Ch [0289]
U __fastcall kotor_fastcall_00411240(void* gff, void*, void* out_struct);                         // CResGFF::GetTopLevelStruct RET 4 [claude_dialogue_start]
void __fastcall c2da_ref_to_string_00405f70(void* r, void*, void* out_string);                    // CResRef::CopyToString RET 4 [0204]
// ---- reconstructed in this file ----
I __fastcall kotor_fastcall_004ef6e0(void* creature, void*, U spell);
}

using namespace vma;

extern "C" {

// ================================================================== 0053D2B0 GetSpellId() [routine 248]
// Caller object (self+8) -> AsSWSCreature -> +1C0h (current spell id); default -1.
// The original calls AsSWSCreature twice (test, then read); kept.
int __fastcall kotor_fastcall_0053d2b0(void* self, void*, int, unsigned) {
    I result = -1;                                                                     // 0053D2C6 or esi,-1
    void* object = nullptr;
    if (static_cast<unsigned char>(kotor_fastcall_004d8230(object_array(), nullptr, self_id(self), &object)) == 0 && // 0053D2B2..DB
        as_creature(object) != nullptr)                                                // 0053D2DD..E8
        result = at<I>(as_creature(object), 0x1c0);                                    // 0053D2EA..F3
    return push_int(result);                                                           // 0053D2F9..15
}

// ================================================================== 0053C610 GetObjectType(object) [routine 106]
// Result 0 when not found or type <= area; type byte 5..16 via jump table 0053C6EC, else 7FFFh.
int __fastcall kotor_fastcall_0053c610(void*, void*, int, unsigned) {
    U id = 0;
    I result = 0;                                                                      // 0053C61F xor esi,esi
    if (!pop_object(&id)) return k_pop_failed;                                         // 0053C61A..33
    void* object = nullptr;
    if (lookup(id, &object)) {                                                         // 0053C636..5B
        const U type = at<U8>(object, 8);                                              // 0053C65D..61 (no null check)
        if (type > k_type_area_00746633) {                                             // 0053C664..6A (jbe)
            switch (type) {                                                            // 0053C66C..77 table 0053C6EC
            case 5: result = 1; break;                                                 // creature
            case 6: result = 2; break;                                                 // item
            case 7: result = 4; break;                                                 // trigger
            case 9: result = 0x40; break;                                              // placeable
            case 10: result = 8; break;                                                // door
            case 11: result = 0x10; break;                                             // area of effect
            case 12: result = 0x20; break;                                             // waypoint
            case 13: result = 0x100; break;                                            // encounter
            case 14: result = 0x80; break;                                             // store
            case 16: result = 0x200; break;                                            // sound
            default: result = 0x7fff; break;                                           // 8, 15, > 16 -> 0053C6C4
            }
        }
    }
    return push_int(result);                                                           // 0053C6C9..E7
}

// ================================================================== 00536310 GetAbilityModifier(int ability, object) [routine 331]
// Pops ability then creature; result 0 unless the creature exists and ability is 0..5 (unsigned ja).
int __fastcall kotor_fastcall_00536310(void*, void*, int, unsigned) {
    I ability = 0; U id = 0;
    if (!pop_int(&ability)) return k_pop_failed;                                       // 00536319..2F
    if (!pop_object(&id)) return k_pop_failed;                                         // 00536332..44
    I result = 0;                                                                      // 00536355
    void* creature = kotor_fastcall_004ae770(server_app(), nullptr, id);               // 00536346..57
    if (creature) {                                                                    // 0053635C..5E
        void* stats = at<void*>(creature, 0xa74);
        switch (static_cast<U>(ability)) {                                             // 00536360..69 table 005363F0
        case 0: result = at<signed char>(stats, 0xea); break;                          // 00536370..76 STR mod
        case 1: result = static_cast<signed char>(static_cast<U8>(kotor_fastcall_005a5330(stats, nullptr, 0))); break; // 0053637F..8E DEX mod
        case 2: result = at<signed char>(stats, 0xee); break;                          // 00536393..99 CON mod
        case 3: result = at<signed char>(stats, 0xf0); break;                          // 005363A2..A8 INT mod
        case 4: result = at<signed char>(stats, 0xf2); break;                          // 005363B1..B7 WIS mod
        case 5: result = at<signed char>(stats, 0xf4); break;                          // 005363C0..C6 CHA mod
        default: break;
        }
    }
    return push_int(result);                                                           // 005363CD..EB
}

// ================================================================== 0053D320 GetSpellSaveDC() [routine 111]
// Caller creature: CalculateSpellSaveDC(creature+1C0h spell).  Caller area-of-effect object (vtbl+50h): its
// creator creature (+248h) with the AOE's vtbl+B8h spell id; if the creator is gone the AOE's stored DC (+258h).
// Default 14.  Quirk (original): the AOE's vtbl+B8h is called even when the creator is gone (result unused).
int __fastcall kotor_fastcall_0053d320(void* self, void*, int, unsigned) {
    I result = 0xe;                                                                    // 0053D336 edi = 0Eh
    void* object = nullptr;
    if (static_cast<unsigned char>(kotor_fastcall_004d8230(object_array(), nullptr, self_id(self), &object)) == 0) { // 0053D322..4D
        if (as_creature(object) != nullptr) {                                          // 0053D353..63
            void* creature = as_creature(object);                                      // 0053D365..6A
            if (creature) result = kotor_fastcall_004ef6e0(creature, nullptr, at<U>(creature, 0x1c0)); // 0053D36C..D3
        } else if (vcall<void*>(object, 0x50) != nullptr) {                            // 0053D378..7F
            void* creature = kotor_fastcall_004ae770(server_app(), nullptr, at<U>(vcall<void*>(object, 0x50), 0x248)); // 0053D381..9F
            if (!creature) result = at<I>(vcall<void*>(object, 0x50), 0x258);           // 0053D3A1..AE
            void* aoe = vcall<void*>(object, 0x50);                                    // 0053D3B4..BA
            const U spell = vcall<U>(aoe, 0xb8);                                       // 0053D3BD..C1
            if (creature) result = kotor_fastcall_004ef6e0(creature, nullptr, spell);  // 0053D3C7..D3
        }
    }
    return push_int(result);                                                           // 0053D3D6..F2
}

// ================================================================== 00535960 EventSpellCastAt(object caster, int spell, int harmful=TRUE) [routine 244]
// CScriptEvent type 2: int 0 = spell, int 1 = harmful, object 0 = caster.  Quirks (original): the event is allocated
// before the pops and leaks on a pop failure or a failed push; a failed allocation is dereferenced.
int __fastcall kotor_fastcall_00535960(void*, void*, int, unsigned argc) {
    void* memory = live_operator_new(0x34);                                            // 00535979..80
    void* event = memory ? recovered_event_construct_004d7540(memory, nullptr) : nullptr; // 00535987..9E
    U caster = 0; I spell = 0; I harmful = 1;                                          // 005359B3 [E-1Ch] = 1
    if (!pop_object(&caster)) return k_pop_failed;                                     // 005359A0..C2
    if (!pop_int(&spell)) return k_pop_failed;                                         // 005359C4..D6
    if (static_cast<I>(argc) > 2 && !pop_int(&harmful)) return k_pop_failed;           // 005359D8..F1 (jle)
    at<U16>(event, 0) = 2;                                                             // 00535A0A
    recovered_event_integer_004d76b0(event, nullptr, 0, static_cast<U>(spell));        // 00535A0F..18
    recovered_event_integer_004d76b0(event, nullptr, 1, static_cast<U>(harmful));      // 00535A1D..26
    recovered_event_object_004d7780(event, nullptr, 0, caster);                        // 00535A2B..34
    if (!recovered_vm_outer_push_engine_structure_005d10b0(vm(), nullptr, 1, event)) return k_push_failed; // 00535A39..4B
    recovered_event_destroy_004d7590(event, nullptr);                                  // 00535A62..64
    live_free(event);                                                                  // 00535A69..6A
    return 0;
}

// =====================================================================================================
// Reconstructed callees without a callable provider in the c31 map.
// =====================================================================================================

// 004EF6E0 CSWSCreature::CalculateSpellSaveDC(int spell): thiscall RET 4.  The spell id is unused.
// 5 + WIS mod (byte +F2h) + CHA mod (byte +F4h) + GetLevel(1) (AL, unsigned); feats 5Ah/59h/58h add 4/2/1 (first hit).
I __fastcall kotor_fastcall_004ef6e0(void* creature, void*, U) {
    void* stats = at<void*>(creature, 0xa74);                                          // 004EF6E5
    const signed char wis = at<signed char>(stats, 0xf2);                              // 004EF6EB
    const signed char cha = at<signed char>(stats, 0xf4);                              // 004EF6F1
    const U level = static_cast<U8>(recovered_stats_level_005a5fd0(stats, nullptr, 1)); // 004EF6F8..FE
    I dc = static_cast<I>(level) + wis + cha + 5;                                      // 004EF703..10
    if (recovered_stats_has_feat_005a6630(at<void*>(creature, 0xa74), nullptr, 0x5a)) return dc + 4; // 004EF714..2E
    if (recovered_stats_has_feat_005a6630(at<void*>(creature, 0xa74), nullptr, 0x59)) return dc + 2; // 004EF731..4B
    if (recovered_stats_has_feat_005a6630(at<void*>(creature, 0xa74), nullptr, 0x58)) ++dc;          // 004EF74E..5F
    return dc;                                                                         // 004EF760..66
}

}  // extern "C"
