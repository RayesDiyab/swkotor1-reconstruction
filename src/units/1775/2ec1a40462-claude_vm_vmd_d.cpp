// Lane VM package vmD, part D: NWScript engine-command handlers (CSWVirtualMachineCommands::ExecuteCommand*)
// reconstructed from the original swkotor.exe (read-only, capstone linear sweep).  Addresses in comments are
// original instruction ranges.  Conventions: claude_vm_actions_common.hpp (not modified).
//  * Every handler: original int __thiscall (int routine, int argc), RET 8 (verified for all 18)
//    -> extern "C" int __fastcall kotor_fastcall_<addr>(self, edx, routine, argc), decorated @...@16.
//  * Return literals: 0, -2001 (0xFFFFF82F pop failure), push status neg/sbb/and 7D0h/add F830h (0 / -2000).
//  * .rdata facts used as literals: [0074666C] byte 0 = CGameObjectArray "found"; [00746634] byte 5 = creature,
//    [00746638] byte 9 = placeable, [00746639] byte 0Ah = door; [00748694] float 0.3048 (3E9C0EBFh),
//    [00748698] float 0.9144 (3F6A161Eh); jump table 0053AAAC (6 entries) decoded below.
//  * CSWVirtualMachineCommands +8 = OBJECT_SELF id of the running script.
#include "061872cb05-claude_vm_actions_common.hpp"

namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); }  // CExoString() [menu20_bindings]
namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } }                  // [007A39E0] CExoBase* [0068]

extern "C" {
// ---- existing providers (c31 map; object in brackets; arity checked against the original RET) ----
U __fastcall kotor_fastcall_004aea40(void* server, void*);                                   // CServerExoApp::GetPlayerCreatureId [0595]
void* __fastcall kotor_fastcall_004ae780(void* server, void*, U id);                        // GetAreaByGameObjectID RET 4 [0530]
void __fastcall recovered_item_set_identified_00554170(void* item, void*, U flag);          // CSWSItem::SetIdentified RET 4 [w2_party_native]
U __fastcall recovered_item_cost_00554000(void* item, void*);                               // CSWSItem::GetCost [w2_g07_native]
U __fastcall recovered_object_get_reputation_0057cb80(void* object, void*, U target, U arg2); // CSWSObject::GetReputation RET 8 [w1_native_b7]
void __fastcall recovered_debug_timestamp_005e9410(void* debug, void*, void* out);           // CExoDebug::GetCurrentTimestamp RET 4 [0854-slot509]
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);                 // GetSWSMessage [creature_destructor_r2]
void* __fastcall recovered_server_player_list_004aed90(void* server, void*);                 // GetPlayerList [w1_native_b7]
void* __fastcall kotor_fastcall_00561080(void* player, void*);                               // CSWSPlayer::GetGameObject [0595]
void __fastcall kotor_fastcall_004c8230(void* module, void*, U strref);                      // CSWSModule::DeleteWorldJournalEntryStrref RET 4 [claude_p2s_b_journal]
U __fastcall kotor_fastcall_0056a370(void* msg, void*, void* player, U strref);              // SendServerToPlayerJournalDeleteWorldStrref RET 8 [claude_p2s_b_journal]
U __fastcall kotor_fastcall_005ab880(void* stats, void*, U flag);                            // CSWSCreatureStats::GetWillSavingThrow RET 4 (AL) [claude_combat_attack_rolls]
void* __fastcall kotor_m15_menu_active_property_00553960(void* item, void*, I index);        // CSWSItem::GetActiveProperty RET 4 [menu_active_property]
void* __fastcall kotor_fastcall_00553990(void* item, void*, I index);                        // CSWSItem::GetPassiveProperty RET 4 [claude_combat_attack_damage]
}

namespace {
using namespace vmx;
constexpr U8 k_type_creature_00746634 = 5;
constexpr U8 k_type_placeable_00746638 = 9;
constexpr U8 k_type_door_00746639 = 0xa;
constexpr float k_feet_to_meters_00748694 = 0.3048f;   // .rdata 00748694 (3E9C0EBFh)
constexpr float k_yards_to_meters_00748698 = 0.9144f;  // .rdata 00748698 (3F6A161Eh)
static_assert(sizeof(float) == 4, "float");
// [0083282C]: .bss dword read only by PrintString / PrintFloat / WriteTimestampedLogEntry (00541604, 00541715,
// 0054184B, 0054187B, 005418D2); the original image contains no store to it, so it keeps its zero initial value.
// The strings lane keeps an identical file-local cell (anonymous namespace, no external symbol); this file keeps
// its own never-written zero cell (identical observable behaviour; coordinator may merge them).
U g_script_log_suppressed_0083282c = 0;

inline I push_status(U pushed) { return pushed ? 0 : k_push_failed; }
inline U pop_object(U* id) { return recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, id); }
inline U pop_int(I* v) { return recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, v); }
inline I push_object(U id) { return push_status(recovered_vm_outer_push_object_005d10d0(vm(), nullptr, id)); }
inline I push_int(I v) { return push_status(recovered_vm_outer_push_integer_005d1010(vm(), nullptr, v)); }
inline I push_float(float v) { return push_status(recovered_vm_outer_push_float_005d1030(vm(), nullptr, v)); }
inline U type_of(void* o) { return at<U8>(o, 8); }
inline void* rules() { return kotor_cell_rules_007a3a28; }                                     // [007A3A28]
inline void* debug_object() { return at<void*>(kotor_menu_10_19::dependencies::base_007a39e0, 8); }   // [[7A39E0]+8]
}  // namespace

extern "C" {

// ================================================================== 0053A1D0 GetItemHasItemProperty(item, nProperty) [398]
int __fastcall kotor_fastcall_0053a1d0(void*, void*, int, unsigned) {
    U item_id = 0; I property = 0;
    if (!pop_object(&item_id)) return k_pop_failed;                                    // 0053A1D0..E5
    if (!pop_int(&property)) return k_pop_failed;                                      // 0053A1F2..04
    I result = 0;                                                                      // 0053A216 ebx
    void* item = recovered_item_lookup_004ae760(server_app(), nullptr, item_id);       // 0053A206..1D
    if (item) {                                                                        // 0053A21F
        for (I i = 0; i < at<I>(item, 0x248); ++i) {                                   // 0053A223..2E, 0053A25C..65 (jle / jl signed)
            void* p = kotor_m15_menu_active_property_00553960(item, nullptr, i);       // 0053A230..33
            if (!p) continue;                                                          // 0053A238
            const U8 bit = at<U8>(p, 0x11);                                            // 0053A23C
            if (bit != 0xff && (at<U>(item, 0x294) & (1u << (bit & 31u))) == 0) continue; // 0053A23F..51 (shl edx, cl)
            if (static_cast<I>(at<U16>(p, 0)) == property) { result = 1; goto done; }  // 0053A253..5A
        }
        for (I i = 0; i < at<I>(item, 0x24c); ++i) {                                   // 0053A267..71, 0053A288..91
            void* p = kotor_fastcall_00553990(item, nullptr, i);                       // 0053A273..76
            if (p && static_cast<I>(at<U16>(p, 0)) == property) { result = 1; break; } // 0053A27B..86
        }
    }
done:
    return push_int(result);                                                           // 0053A29B..BA
}

// ================================================================== 0053A9E0 GetLastAttackMode(oCreature) [318]
// Quirk (original): the found object pointer is not null-checked before its AsSWSCreature call.
int __fastcall kotor_fastcall_0053a9e0(void*, void*, int, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 0053A9E0..FF
    I result = static_cast<I>(k_invalid);                                              // 0053AA16 esi = 7F000000h
    void* object = nullptr;
    if (lookup(id, &object) && vcall<void*>(object, 0x30) != nullptr) {                // 0053AA02..3A
        void* creature = vcall<void*>(object, 0x30);                                   // 0053AA3C..42
        if (at<U>(creature, 0x4e0) != 0) {                                             // 0053AA45..4D
            // movzx eax, byte [c+16E]; dec eax; cmp 5; ja -> 0; table 0053AAAC
            switch (at<U8>(creature, 0x16e)) {
            case 1: result = 1; break;                                                 // -> 0053AA63
            case 2: result = 2; break;                                                 // -> 0053AA6A
            case 3: result = 3; break;                                                 // -> 0053AA71
            case 5: result = 4; break;                                                 // -> 0053AA78
            case 6: result = 5; break;                                                 // -> 0053AA7F
            default: result = 0; break;                                                // entry 3 / ja -> 0053AA86
            }
        }
    }
    return push_int(result);                                                           // 0053AA88..A6
}

// ================================================================== 0053E540 GetWillSavingThrow(oTarget) [492]
int __fastcall kotor_fastcall_0053e540(void*, void*, int, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 0053E540..5B
    void* object = kotor_fastcall_004ae750(server_app(), nullptr, id);                 // 0053E55E..72
    I result = 0;                                                                      // 0053E574
    if (object) {
        const U8 t = static_cast<U8>(type_of(object));                                 // 0053E57A
        if (t == k_type_creature_00746634) {                                           // 0053E57D..83
            void* creature = vcall<void*>(object, 0x30);                               // 0053E585..87
            if (creature) {
                void* stats = at<void*>(creature, 0xa74);                              // 0053E58E
                if (stats)                                                             // 0053E594
                    result = static_cast<signed char>(static_cast<U8>(kotor_fastcall_005ab880(stats, nullptr, 0))); // 0053E598..9E movsx
            }
        } else if (t == k_type_door_00746639) {                                        // 0053E5A3..A9
            void* door = vcall<void*>(object, 0x18);                                   // 0053E5AB..AD
            if (door) result = at<U8>(door, 0x2be);                                    // 0053E5B0..B4
        } else if (t == k_type_placeable_00746638) {                                   // 0053E5BD..C3
            void* placeable = vcall<void*>(object, 0x48);                              // 0053E5C5..C7
            if (placeable) result = at<U8>(placeable, 0x315);                          // 0053E5CA..CE
        }
    }
    return push_int(result);                                                           // 0053E5D5..F1
}

// ================================================================== 00548BE0 DeleteJournalWorldEntryStrref(nStrRef) [674]
// GetSWSMessage and GetModule are fetched before the pop (as in the original).
int __fastcall kotor_fastcall_00548be0(void*, void*, int, unsigned) {
    void* msg = recovered_server_sws_message_004aedb0(server_app(), nullptr);          // 00548BE0..ED ebx
    void* module = server_get_module_004ae6b0(server_app(), nullptr);                  // 00548BF2..FD esi
    I strref = 0;
    if (!pop_int(&strref)) return k_pop_failed;                                        // 00548C02..22
    if (module) kotor_fastcall_004c8230(module, nullptr, static_cast<U>(strref));      // 00548C25..30
    void* players = recovered_server_player_list_004aed90(server_app(), nullptr);      // 00548C35..3F
    if (players) {                                                                     // 00548C46
        void* pos = at<void*>(at<void*>(players, 0), 0);                               // 00548C4A..50 head
        while (pos) {                                                                  // 00548C54 / 00548C90
            void* player = unresolved_list_at_pos_005e9b60(at<void*>(players, 0), nullptr, pos); // 00548C56..59
            if (player && kotor_fastcall_00561080(player, nullptr) && msg)             // 00548C60..71
                (void)kotor_fastcall_0056a370(msg, nullptr, player, static_cast<U>(strref)); // 00548C73..7B
            (void)unresolved_list_next_005e9b90(at<void*>(players, 0), nullptr, &pos); // 00548C80..8C
        }
    }
    return 0;                                                                          // 00548C94..9C
}

// ================================================================== 0053ADB0 GetLastDisarmed() [347]
// Quirk (original): when OBJECT_SELF does not resolve to a CSWSObject, the fallback "object" is the stack slot
// created by `push ecx` at 0053ADB0, i.e. the CSWVirtualMachineCommands `this` pointer itself; its byte +8 (low
// byte of the OBJECT_SELF id) is then tested as an object type and, for 7/9/0Ah, its vtable is called.  Reproduced
// as written (original-game bug, issue #23).  The As* results are not null-checked.
int __fastcall kotor_fastcall_0053adb0(void* self, void*, int, unsigned) {
    U result = k_invalid;                                                              // 0053ADC2 edi
    void* object = kotor_fastcall_004ae750(server_app(), nullptr, at<U>(self, 8));     // 0053ADB1..C7
    void* target;
    if (object && vcall<void*>(object, 0x10) != nullptr)                               // 0053ADCE..DB
        target = vcall<void*>(object, 0x10);                                           // 0053ADDD..E4
    else
        target = self;                                                                 // 0053ADE8 [esp+8] = pushed ecx
    if (target) {                                                                      // 0053ADEC
        const U8 t = at<U8>(target, 8);                                                // 0053ADF0
        if (t == 9) result = at<U>(vcall<void*>(target, 0x48), 0x360);                 // 0053ADF3..FC placeable
        else if (t == 7) result = at<U>(vcall<void*>(target, 0x40), 0x2a4);            // 0053AE04..0D trigger
        else if (t == 0xa) result = at<U>(vcall<void*>(target, 0x18), 0x328);          // 0053AE15..1E door
    }
    return push_object(result);                                                        // 0053AE24..41
}

// ================================================================== 00541690 WriteTimestampedLogEntry(sLogEntry) [560]
// The log write itself is compiled out of the retail image: with [0083282C] == 0 only the two CExoString::CStr
// results are fetched and discarded (00541722 / 0054172B).
int __fastcall kotor_fastcall_00541690(void*, void*, int, unsigned) {
    ExoString entry;
    (void)kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(&entry), nullptr); // 005416A8..AC
    if (!recovered_vm_outer_pop_string_005d1080(vm(), nullptr, &entry)) {              // 005416B1..CB
        exo_destroy(&entry);                                                           // 005416CD..D9
        return k_pop_failed;                                                           // 005416DE
    }
    ExoString stamp;
    (void)kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(&stamp), nullptr); // 005416F4..F8
    recovered_debug_timestamp_005e9410(debug_object(), nullptr, &stamp);               // 005416FD..10
    if (g_script_log_suppressed_0083282c == 0) {                                       // 00541715..1C
        (void)WindowCStr005e5670(&stamp, nullptr);                                     // 0054171E..22
        (void)WindowCStr005e5670(&entry, nullptr);                                     // 00541727..2B
    }
    exo_destroy(&stamp);                                                               // 00541730..39
    exo_destroy(&entry);                                                               // 0054173E..4A
    return 0;                                                                          // 00541753
}

// ================================================================== 00539ED0 GetIsPlayableRacialType(oCreature) [312]
// Race index: creature -> stats(+A74) word +DC; found non-creature -> byte [rules+0AAh] (as written); not found ->
// low word of the GetGameObject out slot, which 004D8230 zeroes on failure (race 0).  Quirk (original): the found
// pointer and the AsSWSCreature result are not null-checked.
int __fastcall kotor_fastcall_00539ed0(void*, void*, int, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 00539ED0..F3
    I result = 0;                                                                      // 00539EDF esi
    void* object = nullptr;
    U race;
    if (lookup(id, &object)) {                                                         // 00539EF6..1B
        if (at<U8>(object, 8) == 5)                                                    // 00539F1D..25
            race = at<U16>(at<void*>(vcall<void*>(object, 0x30), 0xa74), 0xdc);       // 00539F27..32
        else
            race = at<U8>(rules(), 0xaa);                                              // 00539F3B..41
    } else {
        race = static_cast<U>(reinterpret_cast<std::uintptr_t>(object));               // 00539F4A (out slot)
    }
    const U row = race & 0xffffu;                                                      // 00539F4E movzx edx, ax
    unsigned char* table = at<unsigned char*>(rules(), 0xb0);                          // 00539F51..59
    if (*reinterpret_cast<U*>(table + row * 0x34 + 0x20) != 0) result = 1;             // 00539F56..67
    return push_int(result);                                                           // 00539F6C..8A
}

// ================================================================== 0053CED0 GetReputation(oSource, oTarget) [208]
// The first popped object is passed to GetReputation of the second (found) object.  Quirk (original): neither the
// found pointer nor the AsSWSObject result is null-checked.
int __fastcall kotor_fastcall_0053ced0(void*, void*, int, unsigned) {
    U first = 0, second = 0;
    I result = -1;                                                                     // 0053CEDF or esi, -1
    if (!pop_object(&first)) return k_pop_failed;                                      // 0053CEDA..F4
    if (!pop_object(&second)) return k_pop_failed;                                     // 0053CEF7..09
    void* object = nullptr;
    if (lookup(second, &object)) {                                                     // 0053CF0B..30
        void* as_object = vcall<void*>(object, 0x10);                                  // 0053CF36..3F
        result = static_cast<I>(recovered_object_get_reputation_0057cb80(as_object, nullptr, first, 0)); // 0053CF3C..49
    }
    return push_int(result);                                                           // 0053CF4B..69
}

// ================================================================== 005390E0 GetGoldPieceValue(oItem) [311]
// Quirk (original): the found pointer is not null-checked before AsSWSItem.
int __fastcall kotor_fastcall_005390e0(void*, void*, int, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 005390E0..03
    I result = 0;                                                                      // 005390EF
    void* object = nullptr;
    if (lookup(id, &object) && vcall<void*>(object, 0x38) != nullptr)                  // 00539106..38
        result = static_cast<I>(recovered_item_cost_00554000(vcall<void*>(object, 0x38), nullptr)); // 0053913A..4A
    return push_int(result);                                                           // 0053914C..6A
}

// ================================================================== 00537300 MusicBackgroundGetDayTrack(oArea) [558]
int __fastcall kotor_fastcall_00537300(void*, void*, int, unsigned) {
    U id = 0;
    I result = 0;                                                                      // 0053730D
    if (!pop_object(&id)) return k_pop_failed;                                         // 00537301..1F
    void* area = kotor_fastcall_004ae780(server_app(), nullptr, id);                   // 00537322..35
    if (area) result = at<I>(at<void*>(area, 0x208), 0xc);                             // 00537337..3F
    return push_int(result);                                                           // 00537342..5E
}

// ================================================================== 00542D50 SetIdentified(oItem, bIdentified) [333]
int __fastcall kotor_fastcall_00542d50(void*, void*, int, unsigned) {
    U id = 0; I flag = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 00542D50..65
    if (!pop_int(&flag)) return k_pop_failed;                                          // 00542D67..79
    void* item = recovered_item_lookup_004ae760(server_app(), nullptr, id);            // 00542D86..99
    if (item) recovered_item_set_identified_00554170(item, nullptr, static_cast<U>(flag)); // 00542D9B..A4
    return 0;                                                                          // 00542DA9..AE
}

// ================================================================== 0053AC60 GetLastClosedBy() [260]
int __fastcall kotor_fastcall_0053ac60(void* self, void*, int, unsigned) {
    U result = k_invalid;                                                              // 0053AC7A edi
    void* object = kotor_fastcall_004ae750(server_app(), nullptr, at<U>(self, 8));     // 0053AC60..71
    if (object) {                                                                      // 0053AC78
        if (vcall<void*>(object, 0x48) != nullptr)                                     // 0053AC81..8C AsSWSPlaceable
            result = at<U>(vcall<void*>(object, 0x48), 0x354);                         // 0053AC8E..93
        else if (vcall<void*>(object, 0x18) != nullptr)                                // 0053AC9B..A2 AsSWSDoor
            result = at<U>(vcall<void*>(object, 0x18), 0x320);                         // 0053ACA4..AB
    }
    return push_object(result);                                                        // 0053ACB1..CD
}

// ================================================================== 005302F0 FeetToMeters / YardsToMeters [218, 219]
// 218 -> f * [00748694]; 219 -> f * [00748698]; any other routine pushes the routine argument's own bits as the
// float (the result slot is the routine argument slot [esp+8]; quirk kept, unreachable from the command table).
// x87: fld dword; fmul dword; fstp dword -- the double product of two floats is exact, so one rounding to float.
int __fastcall kotor_fastcall_005302f0(void*, void*, int routine, unsigned) {
    float value = 0.0f;
    if (!recovered_vm_outer_pop_float_005d1020(vm(), nullptr, &value)) return k_pop_failed; // 005302F1..0B
    U slot = static_cast<U>(routine);                                                  // [esp+8] routine argument
    if (routine == 0xda)                                                               // 00530312..17
        slot = bits(static_cast<float>(static_cast<double>(value) * static_cast<double>(k_feet_to_meters_00748694)));   // 00530328..32
    else if (routine == 0xdb)                                                          // 00530319..1A
        slot = bits(static_cast<float>(static_cast<double>(value) * static_cast<double>(k_yards_to_meters_00748698)));  // 0053031C..32
    return push_float(flt(slot));                                                      // 00530336..55
}

// ================================================================== 0053BCC0 GetMatchedStringsCount() [179]
int __fastcall kotor_fastcall_0053bcc0(void* self, void*, int, unsigned) {
    I result = 0;                                                                      // 0053BCD6
    void* object = nullptr;
    if (lookup(at<U>(self, 8), &object) && object) result = at<I>(object, 0x1a4);      // 0053BCC2..F4
    return push_int(result);                                                           // 0053BCFA..16
}

// ================================================================== 0053B5E0 GetLastWeaponUsed(oCreature) [328]
int __fastcall kotor_fastcall_0053b5e0(void*, void*, int, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 0053B5E0..FB
    void* creature = kotor_fastcall_004ae770(server_app(), nullptr, id);               // 0053B5FE..11
    const U result = creature ? at<U>(creature, 0x170) : k_invalid;                    // 0053B613..1A
    return push_object(result);                                                        // 0053B620..3B
}

// ================================================================== 00546B20 GetMaxStealthXP() [464]
int __fastcall kotor_fastcall_00546b20(void*, void*, int, unsigned) {
    void* server = server_app();
    void* creature = kotor_fastcall_004ae770(server, nullptr, kotor_fastcall_004aea40(server, nullptr)); // 00546B20..33
    I result = 0;                                                                      // 00546B38
    if (creature) {
        void* area = recovered_object_area_004cb120(creature, nullptr);                // 00546B3E..40
        if (area) result = at<I>(area, 0x2b4);                                         // 00546B45..49
    }
    return push_int(result);                                                           // 00546B4F..6A
}

// ================================================================== 00546CE0 GetStealthXPEnabled() [481]
int __fastcall kotor_fastcall_00546ce0(void*, void*, int, unsigned) {
    void* server = server_app();
    void* creature = kotor_fastcall_004ae770(server, nullptr, kotor_fastcall_004aea40(server, nullptr)); // 00546CE0..F3
    I result = 0;                                                                      // 00546CF8
    if (creature) {
        void* area = recovered_object_area_004cb120(creature, nullptr);                // 00546CFE..00
        if (area) result = at<I>(area, 0x2c0);                                         // 00546D05..09
    }
    return push_int(result);                                                           // 00546D0F..2A
}

// ================================================================== 005398C0 GetIsDawn() [407]
int __fastcall kotor_fastcall_005398c0(void*, void*, int, unsigned) {
    I result = 0;                                                                      // 005398C9
    void* module = server_get_module_004ae6b0(server_app(), nullptr);                  // 005398C0..CB
    if (module && at<U8>(module, 0x1bc) == 3) result = 1;                              // 005398D0..DD
    return push_int(result);                                                           // 005398E2..FD
}

}  // extern "C"
