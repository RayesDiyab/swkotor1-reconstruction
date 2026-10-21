// Lane vmA, group "pending": CreateObject (pending on the CSWSStore class, owned by lane "aoe"); Endar-used NWScript engine-command handlers (CSWVirtualMachineCommands::ExecuteCommand*)
// reconstructed from the original swkotor.exe (read-only, capstone linear sweep).  Addresses in comments are the
// original instructions.  Conventions: claude_vm_vmA_common.hpp.
// .rdata literals: [00746644] 7F000000h OBJECT_INVALID, [0074666C] 0 "found", [00746633] 4 type area,
// [007467C0] word 51h effect type Appear, [007415F8] float 0.017453292 (3C8EFA35h, deg->rad),
// [0074C0AC] "UTW ", [0074C084] "Waypoint template %s doesn't exist.\n".
#include "520143efd3-claude_vm_vma_common.hpp"

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
// ---- vmA 0886 exports (claude_vm_vmA_0886_exports.inc, included into the 0886 unit) ----
unsigned __fastcall kotor_fastcall_00587a70(void* placeable, void*, void* resref, unsigned flag); // CSWSPlaceable::LoadFromTemplate RET 8
unsigned __fastcall kotor_fastcall_005c7f30(void* waypoint, void*, void* gff, void* root);        // CSWSWaypoint::LoadWaypoint RET 8
// ---- not implemented anywhere: CSWSStore class (see report; CreateObject is "pending":"store") ----
void* __fastcall kotor_fastcall_005c6ab0(void* memory, void*, U id);                              // CSWSStore::CSWSStore RET 4
unsigned __fastcall kotor_fastcall_005c7760(void* store, void*, void* resref, unsigned flag);     // CSWSStore::LoadFromTemplate RET 8
void __fastcall kotor_fastcall_005c6e80(void* store, void*, void* area, U x, U y, U z, U flag);   // CSWSStore::AddToArea RET 14h
// ---- reconstructed in this file ----
U __fastcall kotor_fastcall_005c83b0(void* waypoint, void*, void* resref);
}

using namespace vma;

namespace {
// 004015A0 is a bare `ret` (CScriptLocation trivial dtor); written as a file-local no-op as in the endar-actions lane.
inline void location_dtor_004015a0(void*) {}
inline void* area_view(void* area) { return vcall<void*>(static_cast<unsigned char*>(area) + 0x11c, 0x28); }  // [area+11Ch] vtbl+28h
const U k_deg_to_rad_007415f8 = 0x3c8efa35u;
}  // namespace

extern "C" {

// ================================================================== 0052F820 CreateObject(int type, string template, location, int appear=FALSE) [routine 243]
// Switch on type-1 through byte table 0052FDA8 / dword table 0052FD90: 1 creature, 2 item, 32 waypoint, 64 placeable,
// 128 store, anything else -> OBJECT_INVALID.  Original quirks kept:
//  * the popped location leaks when a later pop fails; failed allocations are not checked (null objects are used);
//  * item/placeable/store/waypoint template-load results are ignored; a waypoint is placed but OBJECT_INVALID is
//    returned (esi is never set on that path);
//  * a creature that fails LoadFromTemplate is deleted (vtbl+0, flag 1) and the location is freed before the push.
int __fastcall kotor_fastcall_0052f820(void* self, void*, int, unsigned argc) {
    U result = k_invalid;                                                              // 0052F83B esi = [746644]
    ExoString name;
    (void)kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(&name), nullptr); // 0052F846..4A
    I type = 0; I appear = 0; void* location = nullptr;                                // 0052F859..60
    if (!pop_int(&type) || !pop_string(&name) || !pop_struct(2, &location) ||          // 0052F84F..95
        (static_cast<I>(argc) >= 4 && !pop_int(&appear))) {                            // 0052F897..B0 (jl)
        exo_destroy(&name);                                                            // 0052F8B2..BE
        return k_pop_failed;                                                           // 0052F8C3
    }
    Vector* loc = static_cast<Vector*>(location);                                      // +0 position, +0C orientation
    void* module = server_get_module_004ae6b0(server_app(), nullptr);                  // 0052F8CD..D6
    void* area = module ? module_get_area_004c30f0(module, nullptr) : nullptr;         // 0052F8DB..EE
    if (area) {
        alignas(4) unsigned char resref[16];                                           // [E-2Ch] CResRef
        reinterpret_cast<menu_resources::Ref*>(resref)->construct_00406d80(WindowCStr005e5670(&name, nullptr)); // 0052F8F4..02
        switch (type) {                                                                // 0052F907..1C
        case 2: {                                                                      // 0052F923 item
            void* memory = live_operator_new(0x29c);
            void* item = memory ? recovered_item_construct_005530a0(memory, nullptr, 0x7f000000u) : nullptr; // 0052F934..4D
            (void)recovered_item_template_005608b0(item, nullptr, resref);             // 0052F94F..5A
            void* object = static_cast<unsigned char*>(item) + 0x10;                   // 0052F968 CSWSObject base
            recovered_object_position_004cd1a0(object, nullptr, loc, 1, 1, 0);         // 0052F95F..6E
            w1x_object_set_orientation_004cd170(object, nullptr, loc + 1);             // 0052F973..7D
            recovered_item_add_to_area_00555720(item, nullptr, area, loc->x, loc->y, loc->z, 1); // 0052F982..96
            result = at<U>(item, 0x14);                                                // 0052F99B
            break;
        }
        case 1: {                                                                      // 0052F9A3 creature
            Vector safe = {0.0f, 0.0f, 0.0f};                                          // 0052F9A8..B8 [E-38h]
            void* memory = live_operator_new(0xac8);
            void* creature = memory ? recovered_server_creature_construct_004f7a10(memory, nullptr, 0x7f000000u, 0) : nullptr; // 0052F9C8..E6
            if (!w2x_creature_template_005026d0(creature, nullptr, resref, 0)) {        // 0052F9E8..FB
                if (creature) vcall<void*>(creature, 0, 1u);                           // 0052F9FD..07 scalar deleting dtor
                if (location) { location_dtor_004015a0(location); live_free(location); } // 0052FA09..1E
                const U pushed = recovered_vm_outer_push_object_005d10d0(vm(), nullptr, result); // 0052FA21..2D
                exo_destroy(&name);                                                    // 0052FA41 / 0052FD73
                return pushed ? 0 : k_push_failed;                                     // 0052FA46 / 0052FD78
            }
            w1x_object_set_orientation_004cd170(creature, nullptr, loc + 1);           // 0052FA50..5A
            safe = *loc;                                                               // 0052FA5F..7A
            if (w1x_area_safe_location_004be860(area, nullptr, word(loc), 0x41a00000u, at<U>(creature, 0x340), 1,
                                                word(&safe)))                          // 0052FA75..96 (20.0f)
                *loc = safe;                                                           // 0052FA98..AD
            safe.z = flt(w1x_area_height_bits_004bc380(area_view(area), nullptr, &safe, 1, nullptr)); // 0052FAB0..CE fstp dword
            if (appear != 0) {                                                         // 0052FAD2..D6
                void* em = live_operator_new(0x8c);                                    // 0052FAD8..DD
                void* effect = em ? recovered_effect_construct_00503e40(em, nullptr, 1) : nullptr; // 0052FAE5..FF
                at<U16>(effect, 8) = 0x51;                                             // 0052FB01..0C [7467C0]
                recovered_effect_creator_00503a00(effect, nullptr, self_id(self));     // 0052FB08..1A
                recovered_effect_apply_004d14f0(creature, nullptr, effect, 0, 0);      // 0052FB1F..24
            }
            recovered_creature_placement_004fa100(creature, nullptr, word(area), bits(safe.x), bits(safe.y), bits(safe.z), 1, 1); // 0052FB29..3F
            result = at<U>(creature, 4);                                               // 0052FB44
            break;
        }
        case 64: {                                                                     // 0052FB4C placeable
            void* memory = live_operator_new(0x454);
            void* placeable = memory ? recovered_placeable_construct_005877e0(memory, nullptr, 0x7f000000u) : nullptr; // 0052FB59..76
            (void)kotor_fastcall_00587a70(placeable, nullptr, resref, 0);              // 0052FB78..84
            recovered_object_position_004cd1a0(placeable, nullptr, loc, 1, 1, 0);      // 0052FB89..95
            w1x_object_set_orientation_004cd170(placeable, nullptr, loc + 1);          // 0052FB9A..A4
            const Vector axis = {0.0f, 0.0f, 1.0f};                                    // 0052FBB1..C1
            const double yaw = kotor_m15a_yaw_004aa0f0(loc + 1);                       // 0052FBA9..C9 (ST0)
            const float angle = static_cast<float>(yaw * static_cast<double>(flt(k_deg_to_rad_007415f8))); // 0052FBCE..DC fmul dword; fstp dword
            Quaternion quaternion;                                                     // [E-1Ch]
            (void)quaternion_construct_004ac960(&quaternion, nullptr, axis, angle);     // 0052FBDF..F4
            recovered_placeable_rotation_005853c0(placeable, nullptr, &quaternion);     // 0052FBF9..00
            kotor_fastcall_00587170(placeable, nullptr, area, loc, 1);                 // 0052FC05..0F
            result = at<U>(placeable, 4);                                              // 0052FC14
            break;
        }
        case 128: {                                                                    // 0052FC1C store
            void* memory = live_operator_new(0x258);
            void* store = memory ? kotor_fastcall_005c6ab0(memory, nullptr, 0x7f000000u) : nullptr; // 0052FC29..46
            (void)kotor_fastcall_005c7760(store, nullptr, resref, 0);                  // 0052FC48..54
            recovered_object_position_004cd1a0(store, nullptr, loc, 1, 1, 0);          // 0052FC59..65
            w1x_object_set_orientation_004cd170(store, nullptr, loc + 1);              // 0052FC6A..74
            kotor_fastcall_005c6e80(store, nullptr, area, bits(loc->x), bits(loc->y), bits(loc->z), 1); // 0052FC79..8D
            result = at<U>(store, 4);                                                  // 0052FC92
            break;
        }
        case 32: {                                                                     // 0052FC97 waypoint
            void* memory = live_operator_new(0x240);
            void* waypoint = memory ? recovered_waypoint_construct_005c7e70(memory, nullptr, 0x7f000000u) : nullptr; // 0052FCA4..C1
            (void)kotor_fastcall_005c83b0(waypoint, nullptr, resref);                  // 0052FCC3..CE
            recovered_object_position_004cd1a0(waypoint, nullptr, loc, 1, 1, 0);       // 0052FCD3..DF
            w1x_object_set_orientation_004cd170(waypoint, nullptr, loc + 1);           // 0052FCE4..EE
            recovered_waypoint_place_005c8580(waypoint, nullptr, area, bits(loc->x), bits(loc->y), bits(loc->z), 0); // 0052FCF3..06
            break;                                                                     // result stays OBJECT_INVALID
        }
        default: break;                                                                // table index 5 -> 0052FD0B
        }
    }
    const U pushed = recovered_vm_outer_push_object_005d10d0(vm(), nullptr, result);   // 0052FD0B..17
    if (location) { location_dtor_004015a0(location); live_free(location); }           // 0052FD1F..33 / 0052FD4F..63
    exo_destroy(&name);                                                                // 0052FD37..43 / 0052FD6F..73
    return pushed ? 0 : k_push_failed;                                                 // 0052FD48 / 0052FD78
}

// =====================================================================================================
// Reconstructed callees used only by CreateObject.
// =====================================================================================================

// 005C83B0 CSWSWaypoint::LoadFromTemplate(CResRef): thiscall RET 4 -> 1 loaded, 0 missing template.
// Quirk (original): a failed allocation leaves the GFF null and +94h is read through it.
U __fastcall kotor_fastcall_005c83b0(void* waypoint, void*, void* resref) {
    void* memory = live_operator_new(0xa0);                                            // 005C83CB..D2
    void* gff = memory ? gui_load_gff_construct_00410630(memory, nullptr, 0x80a, "UTW ", resref) : nullptr; // 005C83DA..FE
    if (at<U>(gff, 0x94) == 0) {                                                       // 005C8400..12
        vcall<void*>(gff, 0, 1u);                                                      // 005C8418..1C scalar deleting dtor
        ExoString message, name;
        (void)kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(&message), nullptr); // 005C841E..22 [esp+18h]
        (void)kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(&name), nullptr);    // 005C8427..33 [esp+10h]
        c2da_ref_to_string_00405f70(resref, nullptr, &name);                           // 005C8438..44
        kotor_reconstruction::StartupFormat005e5680(reinterpret_cast<kotor_reconstruction::StartupString*>(&message),
            "Waypoint template %s doesn't exist.\n", WindowCStr005e5670(&name, nullptr)); // 005C8449..62 [74C084]
        exo_destroy(&name);                                                            // 005C8465..6E
        exo_destroy(&message);                                                         // 005C8473..7F
        return 0;                                                                      // 005C8486
    }
    U root = 0;
    (void)kotor_fastcall_00411240(gff, nullptr, &root);                                // 005C849A..9F
    (void)kotor_fastcall_005c7f30(waypoint, nullptr, gff, &root);                      // 005C84A4..AC
    vcall<void*>(gff, 0, 1u);                                                          // 005C84B1..B7
    return 1;                                                                          // 005C84BF
}

}  // extern "C"
