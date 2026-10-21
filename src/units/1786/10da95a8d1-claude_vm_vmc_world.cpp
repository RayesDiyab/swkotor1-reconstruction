// Lane vmC, part D ("world"): NWScript engine-command handlers CSWVirtualMachineCommands::ExecuteCommand*
// for world / area / item / timer / debug-print commands, reconstructed from the original swkotor.exe
// (read-only, capstone linear sweep).  Addresses in the comments are original instructions.
//  * Every handler: int __thiscall (int routine, int argc) RET 8 (verified) ->
//    extern "C" int __fastcall kotor_fastcall_<addr>(void* self, void*, int routine, unsigned argc)   (@16).
//  * Return codes: 0, -2001 (pop failure), -2000 (push failure) exactly as the original literals.
//  * .rdata literals: [0074666C] byte 0 = CGameObjectArray "found"; [00746633] byte 4 = area object type;
//    [00746644] 7F000000h OBJECT_INVALID; [007A3A28] g_pRules.
//  * Original quirks kept (documented at each site): unchecked nulls, leaks of popped locations /
//    allocated items, reuse of argument slots as scratch.
//  * 00541280 PlayVisualAreaEffect (pending lane "aoe") lives in claude_vm_vmC_pending.cpp.
#include "1e78b2f608-claude_vm_vmc_common.hpp"

namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); }  // CExoString() [menu20_bindings]
namespace menu_resources { struct Ref { Ref* __thiscall construct_00406d80(const char* text); }; } // CResRef(char*) [0030]
extern void* kotor_cell_rules_007a3a28;                                                              // g_pRules 007A3A28 [0421]

extern "C" {
// ---- existing providers (c31 map; object in brackets) ----
void* __fastcall server_get_module_004ae6b0(void* server, void*);                                   // GetModule [0530]
U __fastcall kotor_fastcall_004aea40(void* server, void*);                                           // GetPlayerCreatureId [0595]
void* __fastcall kotor_fastcall_004aede0(void* server, void*);                                       // GetWorldTimer [0457]
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, U* day, U* time);                   // CWorldTimer::GetWorldTime [0457]
void __fastcall frame_R4_004adbd0(void* timer, void*) noexcept(false);                               // CWorldTimer::SetSnapshotTime [0457]
void __fastcall recovered_timer_calendar_day_004add50(void* timer, void*, U day, void* year, void* month, void* dom); // ConvertFromCalendarDay [w1_b7]
I __fastcall recovered_event_time_compare_004adfa0(void* timer, void*, U d1, U t1, U d2, U t2);     // CompareWorldTimes [1017]
void __fastcall recovered_module_update_time_004c6210(void* module, void*, U day, U ms, U elapsed);  // CSWSModule::UpdateTime [w1_b7]
void* __fastcall kotor_fastcall_004ae800(void* server, void*, U id);                                 // GetSoundObjectByGameObjectID [claude_vm_actions_game]
void* __fastcall recovered_client_get_sound_object_005ed620(void* client, void*, U id);              // CClientExoApp::GetSoundObjectByGameObjectID [1204-01]
void __fastcall kotor_fastcall_005d5980(void* source, void*, float variance);                        // CExoSoundSource::SetPitchVariance [claude_client_sound_object]
void* __fastcall w5_server_get_door_by_id_004ae7c0(void* server, void*, U id);                       // GetDoorByGameObjectID [pf_w5_drive]
void __fastcall recovered_door_state_00589600(void* door, void*, U state, U notify);                 // CSWSDoor::SetOpenState [0886]
void __fastcall recovered_area_set_stealth_xp_00506aa0(void* area, void*, U value);                  // CSWSArea::SetCurrentStealthXP [w1_b7]
U __fastcall kotor_fastcall_004eb620(void* creature, void*, U item_id, U a2, U a3);                 // CSWSCreature::AddPickUpItemActions [claude_p2s_a_inv]
void __fastcall recovered_area_transition_bmp_00560cd0(void* player, void*, U value, void* text);    // CSWSPlayer::SetAreaTransitionBMP [w2_entry_native]
void* __fastcall lower_construct_005e6080(void* s, void*, void* out);                                // CExoString::LowerCase [0310]
U __fastcall recovered_area_first_object_005071b0(void* area, void*, U* out);                       // CSWSArea::GetFirstObjectInArea [w2_update_native]
U __fastcall recovered_area_next_object_005071e0(void* area, void*, U* out);                        // CSWSArea::GetNextObjectInArea [w2_update_native]
void* __fastcall recovered_item_lookup_004ae760(void* server, void*, U id);                          // GetItemByGameObjectID [1025]
unsigned __fastcall recovered_creature_acquire_item_005158e0(void* creature, void*, void** item, U a, U b, U c); // CSWSCreature::AcquireItem [1194v]
unsigned __fastcall recovered_placeable_acquire_item_00584b10(void* placeable, void*, void* item_cell, U container, U flag); // CSWSPlaceable::AcquireItem [1194v]
U __fastcall w5_item_acquire_item_0055dca0(void* item, void*, void* item_cell, U container, U flag);  // CSWSItem::AcquireItem [claude_w5_object_actions]
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* array, void*, U base);      // CSWBaseItemArray::GetBaseItem [0820]
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void* item, void*);                          // CSWItem::GetBaseItem [0820]
void* __fastcall recovered_item_construct_005530a0(void* item, void*, U id);                         // CSWSItem::CSWSItem [0963-r2]
U __fastcall recovered_item_template_005608b0(void* item, void*, void* resref);                     // CSWSItem::LoadFromTemplate [0995]
void* __fastcall recovered_message_data_construct_004d69f0(void* data, void*);                       // CSWCCMessageData::CSWCCMessageData [w1_b7]
void __fastcall recovered_event_string_set_004d6ae0(void* data, void*, U index, void* text);         // CSWCCMessageData::SetString [0886]
void __fastcall recovered_creature_feedback_004ede10(void* creature, void*, U message, void* data);  // CSWSCreature::SendFeedbackMessage [w1_b7]
U __fastcall exo_array_list_count_004af240(void* list, void*, U value);                             // CExoArrayList<ulong>::Count(value) [0567]
U __fastcall recovered_party_index_006349c0(void* party, void*, U id);                              // CSWParty::GetIndex [w1_b7]
unsigned __fastcall recovered_object_add_action_004cea20(void* object, void*, unsigned type, unsigned group,
    unsigned t0, void* v0, unsigned t1, void* v1, unsigned t2, void* v2, unsigned t3, void* v3, unsigned t4, void* v4, unsigned t5, void* v5,
    unsigned t6, void* v6, unsigned t7, void* v7, unsigned t8, void* v8, unsigned t9, void* v9, unsigned t10, void* v10, unsigned t11, void* v11,
    unsigned t12, void* v12);                                                                        // CSWSObject::AddAction RET 70h [w1_b7]
// ---- owned by lane "aoe" (declared only; pending) ----
// ---- reconstructed in this file (section at the end) ----
void __fastcall kotor_fastcall_00506a80(void* area, void*, U value);
void __fastcall kotor_fastcall_004ae260(void* timer, void*, I hour, I minute, I second, I ms);
U __fastcall kotor_fastcall_004adca0(void* timer, void*, U year, U month, U day);
U __fastcall kotor_fastcall_004adcf0(void* timer, void*, U hour, U minute, U second, U ms);
void __fastcall kotor_fastcall_004adde0(void* timer, void*, U day, U time, U snapshot);
void __fastcall kotor_fastcall_005c8d50(void* sound, void*, float variation);
void __fastcall kotor_fastcall_0066ad40(void* sound, void*, float variation);
}

namespace {
using namespace vmc;
constexpr U8 k_type_area_00746633 = 4;
inline void exo_construct(ExoString* s) { (void)kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(s), nullptr); }
inline void* game_obj(U id) { return kotor_fastcall_004ae750(server_app(), nullptr, id); }
// a read the original performs through a possibly-null pointer (kept: faults exactly like the original)
inline U read_through(const void* p, U offset) {
    return *reinterpret_cast<volatile const U*>(reinterpret_cast<std::uintptr_t>(p) + offset);
}
inline float fmuls(float a, float b) { return a * b; }   // fld/fmul dword then fstp dword: exact single rounding
inline float fadds(float a, float b) { return a + b; }   // fld/fadd dword then fstp dword
}  // namespace

extern "C" {

// ===================================================================================================== 00539DE0
// GetIsNight() [406]: module +1BC byte == 2
int __fastcall kotor_fastcall_00539de0(void*, void*, int, unsigned) {
    I result = 0;                                                                       // 00539DE9
    void* module = server_get_module_004ae6b0(server_app(), nullptr);                   // 00539DE0..EB
    if (module != nullptr && at<U8>(module, 0x1bc) == 2) result = 1;                    // 00539DF0..FD
    return push_int(result);                                                            // 00539E02..1D
}

// ===================================================================================================== 00541640
// PrintInteger(int nInteger, int bPrintToFile?) [4]: pops only, prints nothing in the retail build
int __fastcall kotor_fastcall_00541640(void*, void*, int, unsigned argc) {
    I value = 0, flag = 0;
    if (!pop_int(&value)) return k_pop_failed;                                          // 00541649..55
    if (static_cast<I>(argc) >= 2 && !pop_int(&flag)) return k_pop_failed;              // 00541657..70 (jl)
    return 0;                                                                           // 0054167D
}

// ===================================================================================================== 00541770
// PrintObject(object oObject, int) [5]
int __fastcall kotor_fastcall_00541770(void*, void*, int, unsigned argc) {
    U id = 0; I flag = 0;
    if (!pop_object(&id)) return k_pop_failed;                                          // 00541779..85
    if (static_cast<I>(argc) >= 2 && !pop_int(&flag)) return k_pop_failed;              // 00541787..A0
    return 0;                                                                           // 005417AD
}

// ===================================================================================================== 00541910
// PrintVector(vector vVector, int) [141]
int __fastcall kotor_fastcall_00541910(void*, void*, int, unsigned argc) {
    Vector v = {0.0f, 0.0f, 0.0f};                                                      // 0054191E..2E
    I flag = 0;
    if (!pop_vector(&v)) return k_pop_failed;                                           // 00541919..3D
    if (static_cast<I>(argc) >= 2 && !pop_int(&flag)) return k_pop_failed;              // 0054193F..58
    return 0;                                                                           // 00541965
}

// ===================================================================================================== 00546F30
// SetStealthXPDecrement(int) [499]: player creature's area +2BC
int __fastcall kotor_fastcall_00546f30(void*, void*, int, unsigned) {
    void* server = server_app();
    void* creature = creature_by_id(kotor_fastcall_004aea40(server, nullptr));          // 00546F37..44 (looked up before the pop)
    I value = 0;
    if (!pop_int(&value)) return k_pop_failed;                                          // 00546F49..66
    if (creature != nullptr) {                                                          // 00546F69
        void* area = recovered_object_area_004cb120(creature, nullptr);                 // 00546F6D..6F
        if (area != nullptr) at<I>(area, 0x2bc) = value;                                // 00546F74..7C
    }
    return 0;
}

// ===================================================================================================== 00546C20
// SetCurrentStealthXP(int) [478]
int __fastcall kotor_fastcall_00546c20(void*, void*, int, unsigned) {
    void* server = server_app();
    void* creature = creature_by_id(kotor_fastcall_004aea40(server, nullptr));          // 00546C27..34
    I value = 0;
    if (!pop_int(&value)) return k_pop_failed;                                          // 00546C39..56
    if (creature != nullptr) {                                                          // 00546C59
        void* area = recovered_object_area_004cb120(creature, nullptr);                 // 00546C5D..5F
        if (area != nullptr) recovered_area_set_stealth_xp_00506aa0(area, nullptr, static_cast<U>(value)); // 00546C64..6F
    }
    return 0;
}

// ===================================================================================================== 00546B70
// SetMaxStealthXP(int) [468]
int __fastcall kotor_fastcall_00546b70(void*, void*, int, unsigned) {
    void* server = server_app();
    void* creature = creature_by_id(kotor_fastcall_004aea40(server, nullptr));          // 00546B77..84
    I value = 0;
    if (!pop_int(&value)) return k_pop_failed;                                          // 00546B89..A6
    if (creature != nullptr) {                                                          // 00546BA9
        void* area = recovered_object_area_004cb120(creature, nullptr);                 // 00546BAD..AF
        if (area != nullptr) kotor_fastcall_00506a80(area, nullptr, static_cast<U>(value)); // 00546BB4..BF
    }
    return 0;
}

// ===================================================================================================== 00548310
// YavinHackCloseDoor(object oDoor) [771]: SetOpenState(0, 0)
int __fastcall kotor_fastcall_00548310(void*, void*, int, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                          // 00548311..2B
    void* door = w5_server_get_door_by_id_004ae7c0(server_app(), nullptr, id);          // 0054832E..3C
    if (door != nullptr) recovered_door_state_00589600(door, nullptr, 0, 0);            // 00548341..4B
    return 0;
}

// ===================================================================================================== 0053CA60
// GetPlotFlag(object oTarget) [455]: AsSWSObject +F8 != 0
int __fastcall kotor_fastcall_0053ca60(void*, void*, int, unsigned) {
    U id = 0; I result = 0;                                                             // 0053CA6D
    if (!pop_object(&id)) return k_pop_failed;                                          // 0053CA61..7F
    void* object = game_obj(id);                                                        // 0053CA82..91
    if (object != nullptr && as_object(object) != nullptr &&                            // 0053CA98..A5
        at<U>(as_object(object), 0xf8) != 0)                                            // 0053CAA7..B6
        result = 1;                                                                     // 0053CAB8
    return push_int(result);                                                            // 0053CABD..DA
}

// ===================================================================================================== 0053BE30
// GetModuleItemAcquired / AcquiredFrom / Lost / LostBy [282, 283, 292, 293]: module +154/+158/+15C/+160.
// Jump table 0053BEA0 indexed by the byte table 0053BEB4 (routine - 11Ah: 0,1,4*8,2,3; index 4 = default).
int __fastcall kotor_fastcall_0053be30(void*, void*, int routine, unsigned) {
    U result = k_invalid;                                                               // 0053BE39
    void* module = server_get_module_004ae6b0(server_app(), nullptr);                   // 0053BE30..3E
    if (module != nullptr) {                                                            // 0053BE43
        switch (static_cast<U>(routine) - 0x11au) {                                     // 0053BE47..5D (unsigned ja 0Bh)
        case 0: result = at<U>(module, 0x154); break;                                   // 0053BE64 (282)
        case 1: result = at<U>(module, 0x158); break;                                   // 0053BE6C (283)
        case 10: result = at<U>(module, 0x15c); break;                                  // 0053BE74 (292)
        case 11: result = at<U>(module, 0x160); break;                                  // 0053BE7C (293)
        default: break;                                                                 // 0053BE82
        }
    }
    return push_object(result);                                                         // 0053BE82..9D
}

// ===================================================================================================== 005365A0
// GetAreaOfEffectCreator(object oAreaOfEffectObject) [264]: vtbl+50 AsSWSAreaOfEffectObject, +248 creator
int __fastcall kotor_fastcall_005365a0(void*, void*, int, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                          // 005365A0..BF
    U result = k_invalid;                                                               // 005365D6
    void* object = nullptr;
    if (lookup(id, &object) && vcall<void*>(object, 0x50) != nullptr)                   // 005365C2..FA (no null check of object)
        result = at<U>(vcall<void*>(object, 0x50), 0x248);                              // 005365FC..05
    return push_object(result);                                                         // 0053660B..29
}

// ===================================================================================================== 005404F0
// ActionPickUpItem(object oItem) [34]: OBJECT_SELF creature AddPickUpItemActions(item, OBJECT_INVALID, FFh)
int __fastcall kotor_fastcall_005404f0(void* self, void*, int, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                          // 005404F2..0F
    void* creature = creature_by_id(self_id(self));                                     // 00540512..1F
    if (creature != nullptr) kotor_fastcall_004eb620(creature, nullptr, id, k_invalid, 0xff); // 00540524..39
    return 0;
}

// ===================================================================================================== 00543670
// SetTime(int nHour, int nMinute, int nSecond, int nMillisecond) [12]
int __fastcall kotor_fastcall_00543670(void*, void*, int, unsigned) {
    I hour = 0, minute = 0, second = 0, ms = 0;                                         // [esp+C], [esp+8], [esp+4], [esp]
    if (!pop_int(&hour) || !pop_int(&minute) || !pop_int(&second) || !pop_int(&ms))     // 00543679..C1
        return k_pop_failed;                                                            // 005436C3
    kotor_fastcall_004ae260(kotor_fastcall_004aede0(server_app(), nullptr), nullptr, hour, minute, second, ms); // 005436CE..F2
    U day = 0, time = 0;                                                                // [esp+14], [esp+10]
    frame_GetWorldTime_004ade40(kotor_fastcall_004aede0(server_app(), nullptr), nullptr, &day, &time); // 005436F7..11
    recovered_module_update_time_004c6210(server_get_module_004ae6b0(server_app(), nullptr), nullptr, day, time, 0); // 00543716..32
    return 0;
}

// ===================================================================================================== 00543CA0
// SoundObjectSetPitchVariation(object oSound, float fVariation) [690]
int __fastcall kotor_fastcall_00543ca0(void*, void*, int, unsigned) {
    U id = 0; float variation = 0.0f;
    if (!pop_object(&id) || !pop_float(&variation)) return k_pop_failed;                // 00543CA6..D3
    void* sound = kotor_fastcall_004ae800(server_app(), nullptr, id);                   // 00543CD6..E4
    if (sound != nullptr) kotor_fastcall_005c8d50(sound, nullptr, variation);           // 00543CE9..F4
    return 0;
}

// ===================================================================================================== 00542410
// SetAreaTransitionBMP(int nPredefinedAreaTransition, string sCustomAreaTransitionBMP) [203]
int __fastcall kotor_fastcall_00542410(void* self, void*, int, unsigned) {
    ExoString text; exo_construct(&text);                                               // 0054242B..2F
    I value = 0;
    if (!pop_int(&value) || !pop_string(&text)) {                                       // 00542434..62
        exo_destroy(&text);                                                             // 00542464..70
        return k_pop_failed;
    }
    void* player = kotor_fastcall_004ae910(server_app(), nullptr, self_id(self));       // 0054248C..99 GetClientObjectByObjectId
    if (player != nullptr) recovered_area_transition_bmp_00560cd0(player, nullptr, static_cast<U>(value), &text); // 0054249E..AE
    exo_destroy(&text);                                                                 // 005424B3..BF
    return 0;
}

// ===================================================================================================== 0053D920
// GetStringLowerCase(string sString) [61]
int __fastcall kotor_fastcall_0053d920(void*, void*, int, unsigned) {
    ExoString input; exo_construct(&input);                                             // 0053D938..3C [esp+8]
    ExoString result; exo_construct(&result);                                           // 0053D941..4D [esp]
    if (!pop_string(&input)) {                                                          // 0053D952..69
        exo_destroy(&result);                                                           // 0053D96B..73
        exo_destroy(&input);                                                            // 0053D978..84
        return k_pop_failed;
    }
    ExoString lowered;                                                                  // [esp+10] (constructed by LowerCase)
    void* tmp = lower_construct_005e6080(&input, nullptr, &lowered);                    // 0053D99F..A8
    async_assign_string_005e5c50(&result, nullptr, tmp);                                // 0053D9AD..B7
    exo_destroy(&lowered);                                                              // 0053D9BC..C5
    const U pushed = recovered_vm_outer_push_string_005d1090(vm(), nullptr, &result);   // 0053D9CA..D5
    exo_destroy(&result);                                                               // 0053D9E7 / 0053DA13
    exo_destroy(&input);                                                                // 0053D9EC..F8 / 0053DA18..24
    return pushed ? 0 : k_push_failed;
}

// ===================================================================================================== 00542300
// SendMessageToPC(object oPlayer, string szMessage) [374]: argc must be exactly 2
int __fastcall kotor_fastcall_00542300(void*, void*, int, unsigned argc) {
    ExoString text; exo_construct(&text);                                               // 00542318..1C [esp+4]
    U id = 0;                                                                           // [esp]
    if (argc != 2 || !pop_object(&id) || !pop_string(&text)) {                          // 00542321..56
        exo_destroy(&text);                                                             // 00542358..64
        return k_pop_failed;
    }
    void* creature = creature_by_id(id);                                                // 0054237F..8E
    if (creature != nullptr) {                                                          // 00542393..97
        void* memory = live_operator_new(0x34);                                         // 0054239A..A4
        void* data = memory ? recovered_message_data_construct_004d69f0(memory, nullptr) : nullptr; // 005423A8..BC
        recovered_event_string_set_004d6ae0(data, nullptr, 0, &text);                   // 005423BE..CC (null data not checked)
        recovered_creature_feedback_004ede10(creature, nullptr, 0xcc, data);            // 005423D1..D9 (data handed over)
    }
    exo_destroy(&text);                                                                 // 005423DF..EB
    return 0;
}

// ===================================================================================================== 0053E830
// GiveItem(object oItem, object oGiveTo) [271]
int __fastcall kotor_fastcall_0053e830(void*, void*, int, unsigned) {
    U item_id = 0, target_id = 0;                                                       // [esp+4], [esp+8]
    if (!pop_object(&item_id) || !pop_object(&target_id)) return k_pop_failed;          // 0053E836..63
    void* item = recovered_item_lookup_004ae760(server_app(), nullptr, item_id);        // 0053E866..84 (stored in [esp])
    void* target = game_obj(target_id);                                                 // 0053E87A..91
    if (item == nullptr || target == nullptr) return 0;                                 // 0053E893..A1
    const U8 type = type_of(target);                                                    // 0053E8A7
    if (type == 5) {                                                                    // 0053E8AA creature
        recovered_creature_acquire_item_005158e0(as_creature(target), nullptr, &item, at<U>(item, 0x268), k_invalid, 0); // 0053E8AF..CB
        return 0;
    }
    if (type == 9) {                                                                    // 0053E8D9 placeable
        recovered_placeable_acquire_item_00584b10(as_placeable(target), nullptr, &item, at<U>(item, 0x268), 0); // 0053E8DE..F5
        return 0;
    }
    if (type == 6) {                                                                    // 0053E903 item (container)
        void* target_item = vcall<void*>(target, 0x38);                                 // 0053E908..0C AsSWSItem
        void* base = kotor_m15b_base_item_array_get_base_item_005b31a0(at<void*>(kotor_cell_rules_007a3a28, 0x34), nullptr,
                                                                       at<U>(target_item, 0xc)); // 0053E90F..1C
        if (at<U>(base, 0x70) != 0)                                                     // 0053E921..26
            w5_item_acquire_item_0055dca0(vcall<void*>(target, 0x38), nullptr, &item, at<U>(item, 0x268), 0); // 0053E928..43
    }
    return 0;                                                                           // 0053E948
}

// ===================================================================================================== 00547970
// ChangeItemCost(string sItem, float fCostMultiplier) [747]: base item +C4 = multiplier.
// Quirk: the template item built to reach the base item is never destroyed (leaked).
int __fastcall kotor_fastcall_00547970(void*, void*, int, unsigned) {
    float multiplier = 1.0f;                                                            // 0054798C [esp]
    ExoString tag; exo_construct(&tag);                                                 // 00547988..94 [esp+8]
    if (!pop_string(&tag)) { exo_destroy(&tag); return k_pop_failed; }                  // 00547999..D9
    if (!pop_float(&multiplier)) { exo_destroy(&tag); return k_pop_failed; }            // 005479DC..14
    alignas(4) unsigned char resref_bytes[16];                                          // [esp+10] CResRef (16 bytes)
    reinterpret_cast<menu_resources::Ref*>(resref_bytes)->construct_00406d80(WindowCStr005e5670(&tag, nullptr)); // 00547A17..22
    void* memory = live_operator_new(0x29c);                                            // 00547A27..34
    void* item = memory ? recovered_item_construct_005530a0(memory, nullptr, k_invalid) : nullptr; // 00547A38..51
    if (item != nullptr && recovered_item_template_005608b0(item, nullptr, resref_bytes) != 0) { // 00547A53..6A
        void* base = kotor_m15b_item_get_base_item_005b4790(item, nullptr);             // 00547A6C..76
        at<float>(base, 0xc4) = multiplier;                                             // 00547A7B..7F
    }
    exo_destroy(&tag);                                                                  // 00547A85..91
    return 0;
}

// ===================================================================================================== 0054A090
// GetNearestMine(object oTarget?, int bDetectedOnly) [488]: closest trap object (area list +12C) to the object,
// by squared distance below 1.0e8; with the flag == 1 only traps whose detected-by list contains the object id.
int __fastcall kotor_fastcall_0054a090(void*, void*, int, unsigned) {
    U id = 0;                                                                           // [esp+4]
    float best = 1.0e8f;                                                                // 0054A0A1 [esp+8] = 4CBEBC20h
    I flag = 0;                                                                         // [esp+10]
    if (!pop_object(&id)) return k_pop_failed;                                          // 0054A093..BB
    if (!pop_int(&flag)) return k_pop_failed;                                           // 0054A0BE..D0
    void* list = nullptr;                                                               // edi
    void* origin = nullptr;                                                             // ebp
    void* object = game_obj(id);                                                        // 0054A0D2..E7
    if (object != nullptr && as_object(object) != nullptr) {                            // 0054A0E9..F6
        origin = as_object(object);                                                     // 0054A0F8..FF
        if (recovered_object_area_004cb120(origin, nullptr) != nullptr)                 // 0054A101..0A
            list = static_cast<unsigned char*>(recovered_object_area_004cb120(as_object(object), nullptr)) + 0x12c; // 0054A10C..1C
    }
    // 0054A124: otherwise ebp = the popped flag value (unused: edi = 0 skips the loop)
    U result = k_invalid;                                                               // 0054A12A [esp+0C]
    if (list != nullptr) {                                                              // 0054A128..32
        for (I i = 0; i < at<I>(list, 4); ++i) {                                        // 0054A138..2C (count re-read)
            void* trap = game_obj(at<U*>(list, 0)[i]);                                  // 0054A146..59
            if (trap == nullptr) continue;                                              // 0054A15B..5D
            if (flag == 1) {                                                            // 0054A163..68
                void* detected;
                if (vcall<void*>(trap, 0x40) != nullptr)                                // 0054A16A..75 AsSWSTrigger
                    detected = static_cast<unsigned char*>(vcall<void*>(trap, 0x40)) + 0x2a8;  // 0054A177..81
                else if (vcall<void*>(trap, 0x18) != nullptr)                           // 0054A189..92 AsSWSDoor
                    detected = static_cast<unsigned char*>(vcall<void*>(trap, 0x18)) + 0x2dc;  // 0054A194..9E
                else if (vcall<void*>(trap, 0x48) != nullptr)                           // 0054A1A6..AD AsSWSPlaceable
                    detected = static_cast<unsigned char*>(vcall<void*>(trap, 0x48)) + 0x318;  // 0054A1AF..BB
                else
                    continue;
                if (exo_array_list_count_004af240(detected, nullptr, id) == 0) continue;  // 0054A1C1..C8
            }
            void* t = as_object(trap);                                                  // 0054A1CA..CE
            // 0054A1D1..08: 53-bit x87: dx, dy, dz; (dx*dx + dz*dz) + dy*dy
            const double dx = static_cast<double>(at<float>(origin, 0x90)) - static_cast<double>(at<float>(t, 0x90));
            const double dy = static_cast<double>(at<float>(origin, 0x94)) - static_cast<double>(at<float>(t, 0x94));
            const double dz = static_cast<double>(at<float>(origin, 0x98)) - static_cast<double>(at<float>(t, 0x98));
            const double sum = (dx * dx + dz * dz) + dy * dy;
            if (sum < static_cast<double>(best)) {                                      // 0054A20A..13 (unordered/>= skip)
                best = static_cast<float>(sum);                                         // 0054A217
                result = at<U*>(list, 0)[i];                                            // 0054A215..1E
            }
        }
    }
    return push_object(result);                                                         // 0054A233..57
}

// ===================================================================================================== 0053C390
// GetFirstObjectInArea / GetNextObjectInArea(object oArea = OBJECT_INVALID, int nObjectFilter = 7FFFh) [93, 94].
// Filter bits -> object types: 1:5 2:6 4:7 8:A 10:B 20:C 40:9 80:E 100:D 200:10.
int __fastcall kotor_fastcall_0053c390(void* self, void*, int routine, unsigned argc) {
    ExoString unused; exo_construct(&unused);                                           // 0053C3AE..B4 [esp+20]
    void* objects = object_array();                                                     // 0053C3B9..C5
    U result = k_invalid;                                                               // 0053C3D1..D9 [esp+10]
    void* area = nullptr;                                                               // ebx
    void* found = nullptr;                                                              // [esp+14]
    I filter = 0;                                                                       // [esp+18]
    if (static_cast<I>(argc) >= 1) {                                                    // 0053C3CA..DD (jl)
        U area_id = 0;                                                                  // [esp+1C]
        if (!pop_object(&area_id)) { exo_destroy(&unused); return k_pop_failed; }       // 0053C3DF..F1 -> 0053C434
        if (static_cast<U8>(kotor_fastcall_004d8230(objects, nullptr, area_id, &found)) == 0 && found != nullptr) // 0053C3F3..12
            area = vcall<void*>(found, 0x28);                                           // 0053C414..19 AsSWSArea
    }
    if (static_cast<I>(argc) >= 2) {                                                    // 0053C41B..1E (jl)
        if (!pop_int(&filter)) { exo_destroy(&unused); return k_pop_failed; }           // 0053C420..4A
    } else {
        filter = 0x7fff;                                                                // 0053C44F
    }
    bool have = area != nullptr;
    if (!have) {                                                                        // 0053C457..59
        if (static_cast<U8>(kotor_fastcall_004d8230(objects, nullptr, self_id(self), &found)) == 0 && found != nullptr) { // 0053C45B..7D
            const U8 type = type_of(found);                                             // 0053C483..8C
            if (type > k_type_area_00746633) {                                          // 0053C48E (jbe: unsigned)
                area = recovered_object_area_004cb120(found, nullptr);                  // 0053C490
                have = area != nullptr;
            } else if (type == k_type_area_00746633) {                                  // 0053C497
                area = vcall<void*>(found, 0x28);                                       // 0053C49D..9F
                have = area != nullptr;
            }
        }
    }
    if (have) {                                                                         // 0053C4A4..A6
        U ok;
        if (routine == 0x5d) ok = recovered_area_first_object_005071b0(area, nullptr, &result); // 0053C4AC..BA
        else ok = recovered_area_next_object_005071e0(area, nullptr, &result);          // 0053C4CF..D6
        if (ok == 0) result = k_invalid;                                                // 0053C4BF..E5
        void* object = nullptr;                                                         // [esp+3C] (argc slot reused)
        if (static_cast<U8>(kotor_fastcall_004d8230(objects, nullptr, result, &object)) == 0) { // 0053C4E9..00
            for (;;) {
                const U type = at<U8>(object, 8);                                       // 0053C506..0A (no null check)
                if (type == 0) break;                                                   // 0053C50E..10
                const U f = static_cast<U>(filter);                                     // 0053C516
                if ((f & 1) && type == 5) break;                                        // 0053C51A..22
                if ((f & 2) && type == 6) break;                                        // 0053C528..30
                if ((f & 4) && type == 7) break;                                        // 0053C536..3E
                if ((f & 8) && type == 0xa) break;                                      // 0053C540..48
                if ((f & 0x10) && type == 0xb) break;                                   // 0053C54A..52
                if ((f & 0x20) && type == 0xc) break;                                   // 0053C554..5C
                if ((f & 0x40) && type == 9) break;                                     // 0053C55E..66
                if ((f & 0x80) && type == 0xe) break;                                   // 0053C568..6F (test cl,cl; jns)
                if ((f & 0x100) && type == 0xd) break;                                  // 0053C571..79
                if ((f & 0x200) && type == 0x10) break;                                 // 0053C57B..83
                if (recovered_area_next_object_005071e0(area, nullptr, &result) == 0) result = k_invalid; // 0053C585..9A
                if (static_cast<U8>(kotor_fastcall_004d8230(objects, nullptr, result, &object)) != 0) break; // 0053C59E..B5
            }
        }
    }
    const U pushed = recovered_vm_outer_push_object_005d10d0(vm(), nullptr, result);    // 0053C5BB..C6
    exo_destroy(&unused);                                                               // 0053C5DB / 0053C5E7
    return pushed ? 0 : k_push_failed;
}

// ===================================================================================================== 0052C960
// ActionFollowObject(object oFollow, float fFollowDistance = 0.0) [167]: queues action 37h (follow) and 3Ah on
// the OBJECT_SELF creature with the follow target, a point fFollowDistance ahead of it along its orientation
// (or the target's party formation slot, shifted -spacing on Y, when the target is a client party member),
// and the creature's own position.
// Quirks: the target object and the OBJECT_SELF creature are not null-checked (0052C9E2 / 0052CB29 reads).
int __fastcall kotor_fastcall_0052c960(void* self, void*, int, unsigned) {
    U target_id = 0;                                                                    // [ESP0+8]
    float distance = 0.0f;                                                              // 0052C979 [ESP0+4]
    I one = 1;                                                                          // 0052C971 [ESP0+0C]
    if (!pop_object(&target_id) || !pop_float(&distance)) return k_pop_failed;          // 0052C96A..A7
    if (target_id == k_invalid) return 0;                                               // 0052C9AA..B2
    void* creature = creature_by_id(self_id(self));                                     // 0052C9B8..C6 edi
    void* target = game_obj(target_id);                                                 // 0052C9CB..DB
    void* t = as_object(target);                                                        // 0052C9E0..E6
    Vector offset;                                                                      // [ESP0+20]
    offset.x = fmuls(distance, at<float>(t, 0x9c));                                     // 0052C9E9..F7
    offset.y = fmuls(distance, at<float>(t, 0xa0));                                     // 0052C9FB..05
    offset.z = fmuls(distance, at<float>(t, 0xa4));                                     // 0052CA09..13
    void* t2 = as_object(target);                                                       // 0052CA17
    Vector point;                                                                       // [ESP0+14]
    point.z = fadds(offset.z, at<float>(t2, 0x98));                                     // 0052CA1A..1E
    point.y = fadds(offset.y, at<float>(t2, 0x94));                                     // 0052CA2B..2F
    point.x = fadds(offset.x, at<float>(t2, 0x90));                                     // 0052CA32..40
    if (creature != nullptr) {                                                          // 0052CA29 / 0052CA44
        void* client = client_app();
        const U client_id = kotor_fastcall_005eda50(client, nullptr, at<U>(creature, 4)); // 0052CA4A..57
        void* client_creature = reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_R4_005ed5b0(client, nullptr, client_id))); // 0052CA5C..66
        if (client_creature != nullptr && at<U>(client_creature, 0x3a4) != 0) {         // 0052CA6B..7B
            const I index = static_cast<I>(recovered_party_index_006349c0(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr,
                                                                          at<U>(client_creature, 4))); // 0052CA81..95
            if (index >= 0) {                                                           // 0052CA9C..9E
                void* party = kotor_fastcall_005ed8b0(client_app(), nullptr);           // 0052CAA0..A9
                copy_vector(&point, static_cast<unsigned char*>(party) + static_cast<U>(index) * 0x88 + 0x5c); // 0052CAAE..CE
                void* party2 = kotor_fastcall_005ed8b0(client_app(), nullptr);          // 0052CAC1..D5
                const U slot = at<U>(party2, 0x1bc);                                    // 0052CADA
                Vector shift;                                                           // [ESP0+20]
                shift.x = 0.0f;                                                         // 0052CAF6
                shift.y = -at<float>(party2, 0x234 + slot * 4);                         // 0052CAE4..F2 (fchs)
                shift.z = 0.0f;                                                         // 0052CAFE
                vector_add_assign_004a9740(&point, nullptr, &shift);                    // 0052CAE0..06
            }
        }
    }
    Vector own;                                                                         // [ESP0+2C]
    own.x = w5::flt(read_through(creature, 0x90));                                      // 0052CB23..2B (null creature faults)
    own.y = w5::flt(read_through(creature, 0x94));                                      // 0052CB2F..39
    own.z = w5::flt(read_through(creature, 0x98));                                      // 0052CB3D..47
    I zero = 0;                                                                         // 0052CB77 [ESP0+10]
    recovered_object_add_action_004cea20(creature, nullptr, 0x37, 0xffff,               // 0052CB0B..82
        3, &target_id, 1, &one, 2, &point.x, 2, &point.y, 1, &zero, 2, &own.x, 2, &own.y,
        0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr);
    recovered_object_add_action_004cea20(creature, nullptr, 0x3a, 0xffff,               // 0052CB87..D0
        3, &target_id, 1, &one, 2, &point.x, 2, &point.y,
        0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr);
    return 0;
}

// =====================================================================================================
// Extra callees reconstructed here (no callable provider in the c31 image)
// =====================================================================================================

// 00506A80 CSWSArea::SetMaxStealthXP(ulong) RET 4: max = v; current clamped down (unsigned jbe)
void __fastcall kotor_fastcall_00506a80(void* area, void*, U value) {
    const bool clamp = at<U>(area, 0x2b8) > value;                                      // 00506A84 (cmp before the store)
    at<U>(area, 0x2b4) = value;                                                         // 00506A8A
    if (clamp) at<U>(area, 0x2b8) = value;                                              // 00506A90..92
}

// 004ADCA0 CWorldTimer::ConvertToCalendarDay(year, month, day) RET 0Ch (unsigned range checks)
U __fastcall kotor_fastcall_004adca0(void*, void*, U year, U month, U day) {
    U result = year <= 0x7d00 ? year * 0x150 : 0x6dec0u;                                // 004ADCA0..B3
    if (month >= 1 && month <= 0xc) result += (month - 1) * 0x1c;                       // 004ADCB8..CC
    else result += 0x8c;                                                                // 004ADCCE
    if (day >= 1 && day <= 0x1c) result += day - 1;                                     // 004ADCD3..E1
    return result;
}

// 004ADCF0 CWorldTimer::ConvertHourMinSecMsToMs(h, m, s, ms) RET 10h; +38 byte = minutes per hour.
// Quirk kept: an out-of-range second adds 0DAC0h (56000), not 59000.
U __fastcall kotor_fastcall_004adcf0(void* timer, void*, U hour, U minute, U second, U ms) {
    const U per_hour = at<U8>(timer, 0x38);                                             // 004ADCF7
    U result = hour <= 0x17 ? per_hour * hour * 0xea60u : per_hour * 0x150ea0u;         // 004ADCF4..08
    result += minute <= 0x3b ? minute * 0xea60u : 0x360420u;                            // 004ADD0E..21
    if (second <= 0x3b) return result + second * 0x3e8u + ms;                           // 004ADD26..3D
    return result + 0xdac0u + ms;                                                       // 004ADD40..4B
}

// 004ADDE0 CWorldTimer::SetWorldTime(day, time, snapshot) RET 0Ch
void __fastcall kotor_fastcall_004adde0(void* timer, void*, U day, U time, U snapshot) {
    if (snapshot == 1) frame_R4_004adbd0(timer, nullptr);                               // 004ADDE7..F1
    const unsigned long long elapsed = at<unsigned long long>(timer, 0xc) / 1000ull;    // 004ADDF6..05 __aulldiv
    const unsigned long long per_day = at<U>(timer, 0x3c);                              // 004ADE0A..0F
    const unsigned long long days = elapsed / per_day;                                  // 004ADE13 __aulldvrm (quotient)
    const unsigned long long rest = elapsed % per_day;                                  //            (remainder)
    at<U>(timer, 0x30) = day - static_cast<U>(days);                                    // 004ADE20..2D
    at<U>(timer, 0x34) = time - static_cast<U>(rest);                                   // 004ADE26..30
}

// 004AE260 CWorldTimer::AdvanceToTime(hour, minute, second, ms) RET 10h
void __fastcall kotor_fastcall_004ae260(void* timer, void*, I hour, I minute, I second, I ms) {
    if (hour < 0 || minute < 0 || second < 0 || ms < 0) return;                         // 004AE264..93 (jl)
    const U s_total = static_cast<U>(second) + static_cast<U>(ms) / 1000u;              // 004AE299..A3
    const I ms_rest = ms % 1000;                                                        // 004AE2A5..B4 (idiv)
    const U m_total = static_cast<U>(minute) + s_total / 60u;                           // 004AE2AF..BB
    U sec = static_cast<U>(static_cast<I>(s_total) % 60);                               // 004AE2BD..CC [esp+28]
    const U h_total = static_cast<U>(hour) + m_total / 60u;                             // 004AE2C7..D5
    U min = static_cast<U>(static_cast<I>(m_total) % 60);                               // 004AE2D7..E6 [esp+24]
    U days = h_total / 24u;                                                             // 004AE2DC..F9 esi
    U hr = static_cast<U>(static_cast<I>(h_total) % 24);                                // 004AE2EE..FC [esp+20]
    U day = 0, now = 0;                                                                 // [esp+2C] (ms slot), [esp+18]
    frame_GetWorldTime_004ade40(timer, nullptr, &day, &now);                            // 004AE2F3..06
    const U current_day = day;                                                          // 004AE30B ebx
    U year = 0, dom = 0;                                                                // [esp+14], [esp+10]
    U month = 0;                                                                        // [esp+2C] reused
    recovered_timer_calendar_day_004add50(timer, nullptr, current_day, &year, &month, &dom); // 004AE30F..21
    U out_dom, out_year;                                                                // ebx, esi
    bool advance = false;
    if (days == 0) {                                                                    // 004AE326..28
        const U target = kotor_fastcall_004adcf0(timer, nullptr, hr, min, sec, static_cast<U>(ms_rest)); // 004AE32A..3C
        if (recovered_event_time_compare_004adfa0(timer, nullptr, current_day, now, current_day, target) == 1) { // 004AE341..53
            days = 1;                                                                   // 004AE355
            advance = true;
        }
    } else {
        advance = true;                                                                 // 004AE359 (jbe never taken: esi != 0)
    }
    if (advance) {
        const U d = dom + days;                                                         // 004AE35B..5F
        out_dom = d % 0x1du;                                                            // 004AE361..74
        const U m = month + d / 0x1du;                                                  // 004AE36A..72
        out_year = year + m / 0xdu;                                                     // 004AE36E..81
        month = m % 0xdu;                                                               // 004AE389
        if (out_year > 0x7d00) return;                                                  // 004AE383..8D (ja)
    } else {
        out_dom = dom;                                                                  // 004AE391
        out_year = year;                                                                // 004AE395
    }
    const U time = kotor_fastcall_004adcf0(timer, nullptr, hr, min, sec, static_cast<U>(ms_rest)); // 004AE399..AD
    const U calendar = kotor_fastcall_004adca0(timer, nullptr, out_year, month, out_dom); // 004AE3B2..BC
    kotor_fastcall_004adde0(timer, nullptr, calendar, time, 0);                         // 004AE3A5 push 0, 004AE3C1..C4
}

// 005C8D50 CSWSSoundObject::ChangePitchVariation(float) RET 4: store +240 and forward to the client object
void __fastcall kotor_fastcall_005c8d50(void* sound, void*, float variation) {
    if (variation == at<float>(sound, 0x240)) return;                                   // 005C8D50..5F (equal skips; NaN updates)
    at<float>(sound, 0x240) = variation;                                                // 005C8D61..65
    void* client = client_app();
    const U client_id = kotor_fastcall_005eda50(client, nullptr, at<U>(sound, 4));      // 005C8D6B..78
    void* client_sound = recovered_client_get_sound_object_005ed620(client_app(), nullptr, client_id); // 005C8D7D..87
    if (client_sound != nullptr) kotor_fastcall_0066ad40(client_sound, nullptr, variation); // 005C8D8C..92 (tail jump)
}

// 0066AD40 CSWCSoundObject::ChangePitchVariation(float) RET 4
void __fastcall kotor_fastcall_0066ad40(void* sound, void*, float variation) {
    if (at<U>(sound, 0x168) == 0)                                                       // 0066AD43..4B
        kotor_fastcall_005d5980(at<void*>(sound, 0xfc), nullptr, variation);            // 0066AD4D..58
    at<float>(sound, 0x11c) = variation;                                                // 0066AD5D..61 / 0066AD6B..6F
}

}  // extern "C"
