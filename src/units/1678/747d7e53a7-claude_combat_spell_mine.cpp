// W5 queued-action body, lane worker combat-spell:
//   00519E30 CSWSCreature::AIActionSetMine   (export w5_action_set_mine_00519e30)
// plus its callees without a provider in the c07b candidate map: CSWSTrigger::CreateNewGeometry 0058D210,
// CSWSTrigger::AddToArea 0058F030, CSWSTrigger::RemoveFromArea 0058EE30, CSWSArea::RemoveObjectFromArea 0050DC40,
// CSWSArea::DecreaseAILevelPriority 00507340, CSWSObject::SetArea 004CB160, CSWSCreature::ClearActivities 004EB1B0,
// CSWSCreature::RemoveCombatInvisibilityEffects 004F5F70.
// Transcribed from the original swkotor.exe (read-only); addresses in the comments are original instructions.
// Conventions (W5 t1..t3): thiscall -> extern "C" __fastcall(self, unused_edx, ...); x87 at 53-bit precision ->
// double intermediates, float exactly where the original stores a dword; live_operator_new / live_free only.
#include "842d6b1618-w5_t3.hpp"
#include "5889fde5f1-w5_action_bodies.hpp"

using namespace w5;

namespace kotor_sound_math {   // 004AA980 operator==(const Vector&, const Vector&) [0483]
unsigned __cdecl Equal004aa980(const float* a, const float* b);
}

extern "C" {
// ---------------- imports: existing providers (c07b candidate map; object in brackets) ----------------
void __fastcall recovered_creature_cancel_action_004ef4d0(void* creature, void*);                // StopGuiTimingBar [clear9]
U __fastcall kotor_fastcall_005089d0(void* area, void*);                                         // CSWSArea::GetPlayerCanSetMines [trap_limit]
void* __fastcall kotor_m15_menu_active_property_00553960(void* item, void*, I index);            // CSWSItem::GetActiveProperty
unsigned __fastcall w1x_creature_skill_rank_005aa570(void* stats, void*, U skill, U versus, U base_only); // GetSkillRank (AL) [0886]
int __cdecl crt_rand_006fc447();                                                                 // _rand [0479]
I __fastcall c2da_current_get_string_00413270(void* table, void*, I row, void* column, void* out); // C2DA::GetCExoStringEntry [0379]
void* __fastcall recovered_trigger_construct_0058eae0(void* trigger, void*, U id);               // CSWSTrigger::CSWSTrigger [0963]
void __fastcall recovered_trigger_creator_0058d700(void* trigger, void*, U id);                  // CSWSTrigger::SetCreator [1124]
void __fastcall recovered_trigger_bounds_0058cc60(void* trigger, void*, void* a, void* b, void* c, void* d); // ComputeBoundingBox [1128]
unsigned __fastcall recovered_area_insert_0050dfd0(void* area, void*, unsigned id, unsigned flag); // CSWSArea::AddObjectToArea [1065]
U __fastcall recovered_int_list_remove_0066dcc0(void* list, void*, U value);                     // CExoArrayList<int>::remove [W1 b7]
U __fastcall recovered_ai_level_004b08a0(void* ai, void*, void* object, U level);                // CServerAIMaster::SetAILevel [1070]
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);                       // GetServerAIMaster
void* __fastcall recovered_message_data_construct_004d69f0(void* data, void*);                   // CSWCCMessageData() [W1 b7]
void __fastcall recovered_event_integer_004d76b0(void* data, void*, U index, U value);           // CSWCCMessageData::SetInteger [1017]
unsigned __fastcall recovered_item_in_repository_00555f40(void* repository, void*, void* item, unsigned recurse); // [w2_party]
unsigned __fastcall recovered_creature_remove_item_00510ef0(void* creature, void*, void* item, unsigned a2, unsigned a3,
    void* a4);                                                                                   // CSWSCreature::RemoveItem [w2_party]
unsigned __fastcall recovered_item_update_notify_00569fc0(void* message, void*, void* player, unsigned id); // ..ItemUpdate_DestroyItem
void* __cdecl combat_reuse_message_004aedb0();                                                   // GetSWSMessage [W1 b7]
U __fastcall recovered_effect_integer_00503690(void* effect, void*, U index);                    // CGameEffect::GetInteger [W1 b7]
unsigned __fastcall recovered_object_remove_effect_004d06c0(void* object, void*, unsigned lo, unsigned hi); // RemoveEffectById [W1 b7]
void __stdcall vector_constructor_iterator_00401130(void* base, U element_size, U count,
    void* (__fastcall* construct)(void*, void*));                                                // `eh vector constructor iterator' [0526]
void* __fastcall vector_default_construct_00409e70(void* vector, void*);                         // Vector::Vector [0526]
// ---------------- imports: callees owned by other claude_* files (declared only) ----------------
void __fastcall kotor_fastcall_004ef480(void* creature, void*, U time, U type);                  // StartGuiTimingBar [claude_w5_object_actions]
void __fastcall kotor_fastcall_004ec700(void* creature, void*, void* data);                      // BroadcastSkillData [claude_w5_creature_actions]
void __fastcall w5_creature_broadcast_voice_chat_004f2520(void* creature, void*, U voice);       // BroadcastVoiceChat [claude_w5_object_actions]

// ---------------- exports reconstructed in this file ----------------
void __fastcall kotor_fastcall_004cb160(void* object, void*, void* area);
void __fastcall kotor_fastcall_00507340(void* area, void*);
U __fastcall kotor_fastcall_0050dc40(void* area, void*, U id);
void __fastcall kotor_fastcall_0058ee30(void* trigger, void*);
void __fastcall kotor_fastcall_0058f030(void* trigger, void*, void* area, float x, float y, float z, U flag);
void __fastcall kotor_fastcall_0058d210(void* trigger, void*, float size, const Vector* unused, void* area);
void __fastcall kotor_fastcall_004eb1b0(void* creature, void*, U mask);
void __fastcall kotor_fastcall_004f5f70(void* creature, void*);
}

namespace {
constexpr U k_complete_00745dc0 = 2;      // .rdata 00745DC0
constexpr U k_failed_00745dc4 = 3;        // .rdata 00745DC4
constexpr unsigned char k_lookup_ok_0074666c = 0;     // .rdata 0074666C (byte)
constexpr unsigned char k_object_type_bound_00746633 = 4;   // .rdata 00746633 (byte)
constexpr unsigned char k_type_placeable_00746638 = 9;      // .rdata 00746638 (byte)
constexpr unsigned char k_type_door_00746639 = 10;          // .rdata 00746639 (byte)
constexpr U k_event_script_007441dc = 10;       // .rdata 007441DC
constexpr U k_event_destroy_007441e0 = 11;      // .rdata 007441E0
constexpr U k_effect_invisibility_00746738 = 0x2f;    // .rdata 00746738
constexpr U k_effect_last_combat_00746778 = 0x3f;     // .rdata 00746778
constexpr U k_float_1_0 = 0x3f800000u;          // 1.0 immediate
constexpr U k_float_2_0 = 0x40000000u;          // 2.0 immediate
constexpr U k_float_0_5 = 0x3f000000u;          // 0.5 immediate
const float k_set_range_00747d7c = 2.25f;       // .rdata 00747D7C (squared 1.5 m)
const float k_zero_0073d700 = 0.0f;             // .rdata 0073D700
const char k_set_dc_00747d74[] = "SetDC";             // .rdata 00747D74
const char k_detect_dc_00747d68[] = "DetectDCMod";    // .rdata 00747D68
const char k_disarm_dc_00747d5c[] = "DisarmDCMod";    // .rdata 00747D5C
const char k_trap_script_00747d50[] = "TrapScript";   // .rdata 00747D50
const char k_trap_name_00747d44[] = "TrapName";       // .rdata 00747D44

inline U addr(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline Vector vec_of_bits(U x, U y, U z) { Vector v; std::memcpy(&v.x, &x, 4); std::memcpy(&v.y, &y, 4); std::memcpy(&v.z, &z, 4); return v; }
inline void* stats_of(void* c) { return at<void*>(c, 0xa74); }
inline unsigned char* bytes(void* p) { return static_cast<unsigned char*>(p); }
inline void* traps_table() { return at<void*>(at<void*>(kotor_cell_rules_007a3a28, 0xb8), 0x8c); }   // rules +B8 -> +8C (traps 2DA)
inline unsigned char lookup(U id, void** out) { return static_cast<unsigned char>(kotor_fastcall_004d8230(object_array(), nullptr, id, out)); }
void add_front(void* o, U type, U group, const U* pairs) { w5_add_action_to_front_body(o, type, group, pairs); }
void enqueue(U caller, U target, U type, void* event) {   // AddEventDeltaTime(0, 0, caller, target, type, event)
    recovered_event_enqueue_004b08d0(recovered_server_ai_master_004aed80(server_app(), nullptr), nullptr, 0, 0,
                                     caller, target, type, event);
}
void* new_script_event(U16 type) {            // operator new(34h) + CScriptEvent; the type word is written unchecked
    void* memory = live_operator_new(0x34);
    void* event = memory != nullptr ? recovered_event_construct_004d7540(memory, nullptr) : nullptr;
    at<U16>(event, 0) = type;
    return event;
}
// traps 2DA int column (CExoString column temp + C2DA::GetINTEntry); the out value is left as the caller set it
// when the row/column is missing
void table_int(U row, const char* name, I* out) {
    alignas(4) unsigned char column[8];
    frame_LogStringCtor_005e5a90(column, nullptr, name);
    c2da_current_get_int_00413660(traps_table(), nullptr, row, column, out);
    frame_LogStringDtor_005e5c20(column, nullptr);
}
// append to a CExoArrayList<ulong> (+0 data, +4 size, +8 allocated) with the inline growth of 004FBB70 / 0058F0CE
void list_append(unsigned char* list, U value) {
    if (at<U>(list, 4) == at<U>(list, 8)) {
        const U grow = at<U>(list, 8) != 0 ? at<U>(list, 8) * 2 : 0x10;
        menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(list), nullptr, grow);
    }
    at<U*>(list, 0)[at<U>(list, 4)] = value;
    at<U>(list, 4) += 1;
}
I count_of(const unsigned char* list, U value) {   // inline occurrence count of 0050DC6D / 0058EE7D
    I n = 0;
    const I size = *reinterpret_cast<const I*>(list + 4);
    const U* data = *reinterpret_cast<U* const*>(list);
    for (I i = 0; i < size; ++i) if (data[i] == value) ++n;
    return n;
}
// CSWSArea::ComputeHeight(point, 1, 0) and the "height 0 -> keep the trigger z" fix-up of 0058D300..2A
void vertex_height(void* area, void* trigger, float* vertex) {
    const U h = w1x_area_height_bits_004bc380(area, nullptr, vertex, 1, nullptr);
    vertex[2] = flt(h);
    if (vertex[2] == k_zero_0073d700) std::memcpy(&vertex[2], bytes(trigger) + 0x98, 4);   // test ah,44h / jp: ordered equal
}
}  // namespace

extern "C" {
// =========================================================================================================
// 004CB160 CSWSObject::SetArea(CSWSArea*): RET 4. +8C = area id (+120) or 7F000000.
void __fastcall kotor_fastcall_004cb160(void* self, void*, void* area) {
    if (area != nullptr) at<U>(self, 0x8c) = at<U>(area, 0x120);                    // 004CB160..74
    else at<U>(self, 0x8c) = k_object_invalid;                                      // 004CB177..83
}

// 00507340 CSWSArea::DecreaseAILevelPriority: RET 0. Every creature object of the area at AI level 1 drops to 0.
void __fastcall kotor_fastcall_00507340(void* area, void*) {
    void* array = object_array();                                                   // 0050734F (EBX)
    void* ai = recovered_server_ai_master_004aed80(server_app(), nullptr);          // 00507354..64 (EBP)
    for (I i = 0; i < at<I>(area, 0x194); ++i) {                                    // 00507366..C5 (count re-read)
        void* object = nullptr;                                                     // [esp+10]
        if (static_cast<unsigned char>(kotor_fastcall_004d8230(array, nullptr, at<U*>(area, 0x190)[i], &object))
            != k_lookup_ok_0074666c) continue;                                      // 00507372..8E
        if (at<U8>(object, 8) <= k_object_type_bound_00746633) continue;            // 00507390..9D (jbe)
        if (vcall<void*>(object, 0x30) == nullptr) continue;                        // 0050739F..A6 vtbl+30 AsSWSCreature
        if (at<U>(object, 0x78) != 1) continue;                                     // 005073A8..B0
        recovered_ai_level_004b08a0(ai, nullptr, object, 0);                        // 005073B2..B7
    }
}

// 0050DC40 CSWSArea::RemoveObjectFromArea(id): RET 4, EAX 1. A removed creature with a client lowers the area's
// player count (+128, AI priority drop at 0) and the area gets script event 0Dh (exit) from the creature.
U __fastcall kotor_fastcall_0050dc40(void* area, void*, U id) {
    unsigned char* objects = bytes(area) + 0x190;                                   // 0050DC59..64
    if (at<I>(objects, 4) > 0 && count_of(objects, id) != 0)                        // 0050DC6D..80
        recovered_int_list_remove_0066dcc0(objects, nullptr, id);                   // 0050DC82..83
    void* object = nullptr;                                                         // the argument slot [esp+20]
    if (lookup(id, &object) != k_lookup_ok_0074666c || object == nullptr) return 1; // 0050DC88..B4
    if (at<U8>(object, 8) > k_object_type_bound_00746633 && vcall<U>(object, 0x94) != 0) return 1; // 0050DCBA..CF
    if (vcall<void*>(object, 0x30) == nullptr) return 1;                            // 0050DCD9..E0
    if (kotor_fastcall_004ae910(server_app(), nullptr, id) != nullptr) {            // 0050DCE2..F2
        at<I>(area, 0x128) -= 1;                                                    // 0050DCF4
        if (at<I>(area, 0x128) == 0) kotor_fastcall_00507340(area, nullptr);        // 0050DCFA..FE
    }
    void* event = new_script_event(0xd);                                            // 0050DD03..29
    enqueue(id, at<U>(area, 0x120), k_event_script_007441dc, event);                // 0050DD2E..58
    return 1;                                                                       // 0050DD63
}

// 0058EE30 CSWSTrigger::RemoveFromArea: RET 0
void __fastcall kotor_fastcall_0058ee30(void* trigger, void*) {
    if (recovered_object_area_004cb120(trigger, nullptr) != nullptr) {              // 0058EE36..3D
        kotor_fastcall_0050dc40(recovered_object_area_004cb120(trigger, nullptr), nullptr, at<U>(trigger, 4)); // 0058EE3F..4D
        recovered_int_list_remove_0066dcc0(bytes(recovered_object_area_004cb120(trigger, nullptr)) + 0x1a0, nullptr,
                                           at<U>(trigger, 4));                      // 0058EE52..63 (area trigger list)
        const U id = at<U>(trigger, 4);                                             // 0058EE68
        unsigned char* traps = bytes(recovered_object_area_004cb120(trigger, nullptr)) + 0x12c; // 0058EE6B..78
        if (at<I>(traps, 4) > 0 && count_of(traps, id) == 1)                        // 0058EE7D..93
            recovered_int_list_remove_0066dcc0(bytes(recovered_object_area_004cb120(trigger, nullptr)) + 0x12c,
                                               nullptr, at<U>(trigger, 4));         // 0058EE95..A6
    }
    kotor_fastcall_004cb160(trigger, nullptr, nullptr);                             // 0058EEAC..B0
    const Vector zero = vec_of_bits(0, 0, 0);                                       // [esp+4..C]
    recovered_object_position_004cd1a0(trigger, nullptr, &zero, 1, 1, 0);           // 0058EEB5..DA
}

// 0058F030 CSWSTrigger::AddToArea(area, x, y, z, flag): RET 14h
void __fastcall kotor_fastcall_0058f030(void* trigger, void*, void* area, float x, float y, float z, U flag) {
    kotor_fastcall_0058ee30(trigger, nullptr);                                      // 0058F04A..4C
    if (area == nullptr) return;                                                    // 0058F051..57
    alignas(4) unsigned char unused_text[8];                                        // CExoString [esp+C]
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(unused_text), nullptr); // 0058F05E..62
    kotor_fastcall_004cb160(trigger, nullptr, area);                                // 0058F067..72
    const Vector position = {x, y, z};                                              // [esp+14..1C]
    recovered_object_position_004cd1a0(trigger, nullptr, &position, 0, 1, 0);       // 0058F077..9C
    recovered_area_insert_0050dfd0(recovered_object_area_004cb120(trigger, nullptr), nullptr, at<U>(trigger, 4), flag); // 0058F0A1..B3
    list_append(bytes(recovered_object_area_004cb120(trigger, nullptr)) + 0x1a0, at<U>(trigger, 4)); // 0058F0B8..FB
    // the bounding box is written over the argument slots (z, y, x, area), as the original passes their addresses
    U slot_area = addr(area), slot_x = bits(x), slot_y = bits(y), slot_z = bits(z);
    recovered_trigger_bounds_0058cc60(trigger, nullptr, &slot_z, &slot_y, &slot_x, &slot_area); // 0058F0F6..0F
    frame_LogStringDtor_005e5c20(unused_text, nullptr);                             // 0058F114..20
}

// 0058D210 CSWSTrigger::CreateNewGeometry(float half size, Vector* (unused), CSWSArea*): RET 0Ch. A square of four
// vertices (+288, Vector[4]) around the trigger position, heights from the area walkmesh, index list +298 {0,1,2,3}.
void __fastcall kotor_fastcall_0058d210(void* trigger, void*, float size, const Vector*, void* area) {
    at<U>(trigger, 0x294) = 4;                                                      // 0058D229..2E
    at<U>(trigger, 0x284) = 4;                                                      // 0058D234
    if (at<void*>(trigger, 0x288) != nullptr) {                                     // 0058D23A..43
        live_free(at<void*>(trigger, 0x288));                                       // 0058D245..4B
        at<void*>(trigger, 0x288) = nullptr;                                        // 0058D24E
    }
    if (at<void*>(trigger, 0x298) != nullptr) {                                     // 0058D258..60
        live_free(at<void*>(trigger, 0x298));                                       // 0058D262..68
        at<void*>(trigger, 0x298) = nullptr;                                        // 0058D26B
    }
    if (area == nullptr) return;                                                    // 0058D275..7B
    void* memory = live_operator_new(0x30);                                         // 0058D281..8A
    void* vertices = nullptr;
    if (memory != nullptr) {                                                        // 0058D291..9B
        vector_constructor_iterator_00401130(memory, 0xc, 4, &vector_default_construct_00409e70); // 0058D29D..A7
        vertices = memory;
    }
    at<void*>(trigger, 0x288) = vertices;                                           // 0058D2BA
    at<void*>(trigger, 0x298) = live_operator_new(0x10);                            // 0058D2B0..CF
    const double tx = at<float>(trigger, 0x90), ty = at<float>(trigger, 0x94);
    float* v = at<float*>(trigger, 0x288);
    v[0] = static_cast<float>(tx - size);                                           // 0058D2C5..DE
    at<float*>(trigger, 0x288)[1] = static_cast<float>(static_cast<double>(size) + ty); // 0058D2E0..F4
    vertex_height(area, trigger, at<float*>(trigger, 0x288));                       // 0058D2F7..2A
    at<U*>(trigger, 0x298)[0] = 0;                                                  // 0058D32D..33
    at<float*>(trigger, 0x288)[3] = static_cast<float>(tx - size);                  // 0058D339..4D
    at<float*>(trigger, 0x288)[4] = static_cast<float>(ty - size);                  // 0058D350..62
    vertex_height(area, trigger, at<float*>(trigger, 0x288) + 3);                   // 0058D365..99
    at<U*>(trigger, 0x298)[1] = 1;                                                  // 0058D39C..A6
    at<float*>(trigger, 0x288)[6] = static_cast<float>(static_cast<double>(size) + tx); // 0058D3A2..BB
    at<float*>(trigger, 0x288)[7] = static_cast<float>(ty - size);                  // 0058D3BE..D2
    vertex_height(area, trigger, at<float*>(trigger, 0x288) + 6);                   // 0058D3D5..09
    at<U*>(trigger, 0x298)[2] = 2;                                                  // 0058D40C..16
    at<float*>(trigger, 0x288)[9] = static_cast<float>(static_cast<double>(size) + tx); // 0058D412..2B
    at<float*>(trigger, 0x288)[10] = static_cast<float>(static_cast<double>(size) + ty); // 0058D42E..42
    vertex_height(area, trigger, at<float*>(trigger, 0x288) + 9);                   // 0058D445..79
    at<U*>(trigger, 0x298)[3] = 3;                                                  // 0058D47C..82
}

// 004EB1B0 CSWSCreature::ClearActivities(mask): RET 4. Every requested active (+9FC), unlocked (+A00) activity bit
// except 2 is switched off through SetActivity(bit, 0, 0).
void __fastcall kotor_fastcall_004eb1b0(void* self, void*, U mask) {
    static const U k_bits[] = {1, 4, 8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000}; // 004EB1B5..96
    for (U bit : k_bits) {
        if ((mask & bit) == 0) continue;                                            // test al,bl / test eax,ebx (bit 80h: sign of BL)
        if ((at<U>(self, 0x9fc) & bit) == 0) continue;
        if ((at<U>(self, 0xa00) & bit) != 0) continue;
        recovered_set_activity_004f00a0(self, nullptr, bit, 0, 0);
    }
}

// 004F5F70 CSWSCreature::RemoveCombatInvisibilityEffects: RET 0. Walks the effect list (+124/+128) from the
// stats' first combat-effect index (+140 word) while the type is <= 3Fh, removing invisibility effects (type 2Fh
// with integer 0 == 1) and type 3Fh effects.
void __fastcall kotor_fastcall_004f5f70(void* self, void*) {
    I i = at<U16>(stats_of(self), 0x140);                                           // 004F5F74..7A
    while (i < at<I>(self, 0x128)) {                                                // 004F5F81..87 / 004F5FEC..F2 (jl: signed)
        void* effect = at<void**>(self, 0x124)[i];                                  // 004F5F90..96
        const I type = at<U16>(effect, 8);
        if (type > static_cast<I>(k_effect_last_combat_00746778)) return;           // 004F5F99..A3 (jg)
        if (static_cast<U>(type) == k_effect_invisibility_00746738 &&
            recovered_effect_integer_00503690(effect, nullptr, 0) == 1) {           // 004F5FA5..BD
            recovered_object_remove_effect_004d06c0(self, nullptr, at<U>(effect, 0), at<U>(effect, 4)); // 004F5FBF..C8
            continue;
        }
        if (static_cast<U>(at<U16>(effect, 8)) == k_effect_last_combat_00746778) {  // 004F5FCF..D9
            recovered_object_remove_effect_004d06c0(self, nullptr, at<U>(effect, 0), at<U>(effect, 4)); // 004F5FDB..E4
            continue;
        }
        ++i;                                                                        // 004F5FEB
    }
}

// =========================================================================================================
// 00519E30 CSWSCreature::AIActionSetMine(node): RET 4.
//   node +38 mine item, +3C target object (door / placeable) or 7F000000, +40..48 location, +6C action group.
//   Out of range: SetMine(1Dh) + ChangeFacing(31h point / 13h object) + MoveToPoint in front. First in-range
//   visit: PlayAnimation(6) with a 2 s GUI timing bar (+97C = 1). Second visit: Demolitions (skill 1) against the
//   traps 2DA SetDC; success / critical failure arm the mine (door / placeable trap fields, or a new 2 m CSWSTrigger),
//   consume the item, a critical failure on an existing object fires it, and the result is broadcast.
U __fastcall w5_action_set_mine_00519e30(void* self, void*, void* node) {
    alignas(4) unsigned char script[8];                                             // CExoString [esp+5C]
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(script), nullptr); // 00519E4E..52
    Vector move_point = vec_of_bits(0, 0, 0);                                       // [esp+7C..84] (00519E70..7E)
    w5_creature_activity_manager_004f87d0(self, nullptr, 2);                        // 00519E59..85
    U item_id = at<U>(node, 0x38);                                                  // [esp+38]
    U target = at<U>(node, 0x3c);                                                   // [esp+10]
    U lx = at<U>(node, 0x40), ly = at<U>(node, 0x44), lz = at<U>(node, 0x48);       // [esp+28], [esp+24], [esp+20]
    Vector work = vec_of_bits(lx, ly, lz);                                          // [esp+64..6C]
    Vector location = vec_of_bits(lx, ly, lz);                                      // [esp+70..78]
    U result = k_failed_00745dc4;
    const U group = at<U16>(node, 0x6c);
    void* item = recovered_item_lookup_004ae760(server_app(), nullptr, item_id);    // 00519EDC..F7 ([esp+4C], EDI)
    if (item == nullptr) {                                                          // 00519EF5..FB
        recovered_creature_cancel_action_004ef4d0(self, nullptr);                   // 00519EFD..FF
        at<U>(self, 0x97c) = 0;                                                     // 00519F04
        goto done;                                                                  // 00519F0A
    }
    {
    void* object = kotor_fastcall_004ae750(server_app(), nullptr, target);          // 00519F15..2C ([esp+48], EBP)
    if (object == nullptr) {                                                        // 00519F2A..30
        work = vec_of_bits(0, 0, 0);                                                // 00519F3C..44
        if (kotor_sound_math::Equal004aa980(&location.x, &work.x) != 0) {           // 00519F32..52
            at<U>(self, 0x97c) = 0;                                                 // 00519F54 -> 00519F9B
            goto done;
        }
    } else {
        copy_vector(&location, bytes(vcall<void*>(object, 0x10)) + 0x90);           // 00519F56..73 vtbl+10 AsSWSObject
    }
    if (recovered_object_area_004cb120(self, nullptr) != nullptr &&                 // 00519F77..80
        kotor_fastcall_005089d0(recovered_object_area_004cb120(self, nullptr), nullptr) == 0) { // 00519F82..92
        recovered_creature_cancel_action_004ef4d0(self, nullptr);                   // 00519F94..96
        at<U>(self, 0x97c) = 0;                                                     // 00519F9B
        goto done;
    }
    bool in_range;
    float use_range = 0.0f;                                                         // [esp+44] (GetUseRange output)
    if (object == nullptr) {                                                        // 00519FB0..B2
        Vector away;                                                                // [esp+64]
        away.x = static_cast<float>(static_cast<double>(at<float>(self, 0x90)) - location.x); // 00519FD6..E0
        away.y = static_cast<float>(static_cast<double>(at<float>(self, 0x94)) - location.y); // 00519FC7..E4
        away.z = static_cast<float>(static_cast<double>(at<float>(self, 0x98)) - location.z); // 00519FB8..E8
        Vector unit;                                                                // [esp+50]
        const Vector* n = normalize_004ab180(&unit, &away);                         // 00519FEC..F1
        const double px = static_cast<double>(location.x) - n->x;                   // 0051A005..09
        const double py = static_cast<double>(location.y) - n->y;                   // 00519FFE..02
        const double pz = static_cast<double>(location.z) - n->z;                   // 00519FF1..FB
        work.x = static_cast<float>(px); move_point.x = work.x;                     // 0051A00B..13
        work.y = static_cast<float>(py); move_point.y = work.y;                     // 0051A017..21
        work.z = static_cast<float>(pz); move_point.z = work.z;                     // 0051A01F..36
        const float dz = static_cast<float>(static_cast<double>(at<float>(self, 0x98)) - pz); // 0051A030..3F
        const double dy = static_cast<double>(at<float>(self, 0x94)) - py;          // 0051A043..4D
        const double dx = static_cast<double>(at<float>(self, 0x90)) - work.x;      // 0051A04F..55
        const double distance2 = (dx * dx + dy * dy) + static_cast<double>(dz) * dz; // 0051A059..6B
        in_range = distance2 < k_set_range_00747d7c;                                // 0051A06D..7C (test ah,5 / jp)
    } else {
        w5_creature_get_use_range_004ee440(self, nullptr, target, &move_point, &use_range, 0); // 0051A0C3..D9
        in_range = w5_creature_get_is_in_use_range_004f6000(self, nullptr, target, flt(k_float_0_5), 0) != 0; // 0051A0DE..F3
    }
    const U set_pairs[26] = {3, addr(&item_id), 3, addr(&target), 2, addr(&lx), 2, addr(&ly), 2, addr(&lz)};
    if (!in_range) {                                                                // 0051A0F5
        add_front(self, 0x1d, group, set_pairs);                                    // 0051A0F5..46
        const Vector zero = vec_of_bits(0, 0, 0);                                   // [esp+64] zeroed after its address is pushed
        if (object == nullptr) {                                                    // 0051A14B..75
            const U face[26] = {2, addr(&lx), 2, addr(&ly), 2, addr(&lz)};
            add_front(self, 0x31, group, face);                                     // 0051A177..9A
            w5_creature_add_move_to_point_action_to_front_004f8a50(self, nullptr, group, &move_point,
                at<U>(self, 0x8c), k_object_invalid, 1, flt(k_float_1_0), 0.0f, 0, 0, 0, 0, 0, 0, &zero, 0, 0, 0); // 0051A19F..239
        } else {
            const U face[26] = {3, addr(&target)};
            add_front(self, 0x13, group, face);                                     // 0051A1BC..D6
            w5_creature_add_move_to_point_action_to_front_004f8a50(self, nullptr, group, &move_point,
                at<U>(self, 0x8c), target, 1, use_range, 0.0f, 0, 0, 0, 0, 0, 0, &zero, 0, 0, 0); // 0051A1DB..239
        }
        result = k_complete_00745dc0;                                               // 0051AA20
        goto done;
    }
    if (at<U>(self, 0x97c) == 0) {                                                  // 0051A07E..86
        at<U>(self, 0x97c) = 1;                                                     // 0051A093
        U animation = 0x279c;                                                       // [esp+2C] (0051A243)
        if (object != nullptr &&
            (at<U8>(object, 8) == k_type_door_00746639 || at<U8>(object, 8) == k_type_placeable_00746638))
            animation = 0x274c;                                                     // 0051A09F..B6
        U one = 1, speed = k_float_1_0, duration = k_float_2_0;                     // [esp+40], [esp+14], [esp+18]
        add_front(self, 0x1d, group, set_pairs);                                    // 0051A24B..B9
        const U play[26] = {1, addr(&animation), 2, addr(&speed), 2, addr(&duration), 1, addr(&one)};
        add_front(self, 6, group, play);                                            // 0051A2BE..0D
        kotor_fastcall_004ef480(self, nullptr, 0x7d0, 5);                           // 0051A312..1B
        result = k_complete_00745dc0;
        goto done;
    }
    // second visit: the roll
    const U row = at<U16>(kotor_m15_menu_active_property_00553960(item, nullptr, 0), 2); // 0051A325..2E (EBX)
    I dc = 0;   // [esp+1C]: the original does not initialise it before C2DA::GetINTEntry (assumption: row present)
    table_int(row, k_set_dc_00747d74, &dc);                                         // 0051A332..77
    if (dc < 1) dc = 1;                                                             // 0051A37C..89 (jge: signed)
    I rank = static_cast<signed char>(static_cast<U8>(
        w1x_creature_skill_rank_005aa570(stats_of(self), nullptr, 1, 0, 0)));     // 0051A38D..A4 ([esp+14])
    U take20 = 0;                                                                   // [esp+2C]
    if (static_cast<signed char>(static_cast<U8>(
            w1x_creature_skill_rank_005aa570(stats_of(self), nullptr, 1, 0, 1))) >= 5) // 0051A39D..BD (jl: signed byte)
        rank += 2;                                                                  // 0051A3BF
    I roll;                                                                         // [esp+18]
    if (at<U>(self, 0x4e0) != 0) roll = crt_rand_006fc447() % 0x14 + 1;            // 0051A3C4..DC
    else { roll = 0x14; take20 = 1; }                                               // 0051A3E2..EE
    const I total = roll + rank;                                                    // 0051A3F2..FF ([esp+40])
    bool arm = true;
    if (total < dc && (take20 != 0 || total >= dc - 0xa)) arm = false;              // 0051A3FD..16 -> 0051A916
    if (arm) {
        I detect = 0, disarm = 0;   // [esp+30], [esp+34]: same assumption as SetDC
        table_int(row, k_detect_dc_00747d68, &detect);                              // 0051A41C..61
        table_int(row, k_disarm_dc_00747d5c, &disarm);                              // 0051A466..AB
        {
            alignas(4) unsigned char column[8];                                     // CExoString [esp+50]
            frame_LogStringCtor_005e5a90(column, nullptr, k_trap_script_00747d50);  // 0051A4B0..B9
            c2da_current_get_string_00413270(traps_table(), nullptr, static_cast<I>(row), column, script); // 0051A4BE..E4
            frame_LogStringDtor_005e5c20(column, nullptr);                          // 0051A4E9..F5
        }
        const U8 type = object != nullptr ? at<U8>(object, 8) : 0;
        if (object != nullptr && type == k_type_door_00746639) {                    // 0051A4FA..0B
            void* door = vcall<void*>(object, 0x18);                                // 0051A511..16 vtbl+18 AsSWSDoor
            at<U>(door, 0x2e8) = 1;                                                 // 0051A526
            at<U8>(door, 0x2f8) = static_cast<U8>(row);                             // 0051A530
            assign_string_005e5c50(bytes(door) + 0x270, nullptr, script);           // 0051A51B..36
            at<U8>(door, 0x2ed) = static_cast<U8>(static_cast<U8>(rank) + static_cast<U8>(roll) + static_cast<U8>(detect)); // 0051A53B..4D
            at<U8>(door, 0x2ec) = static_cast<U8>(static_cast<U8>(disarm) + static_cast<U8>(rank) + static_cast<U8>(roll)); // 0051A553..5B
            at<U>(door, 0x300) = 1; at<U>(door, 0x2fc) = 1; at<U>(door, 0x304) = 1; // 0051A561..72
            gui_load_array_add_00671c00(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(
                bytes(recovered_object_area_004cb120(self, nullptr)) + 0x12c), nullptr, at<U>(door, 4)); // 0051A578..89
        } else if (object != nullptr && type == k_type_placeable_00746638) {        // 0051A593..99
            void* placeable = vcall<void*>(object, 0x48);                           // 0051A59B..A0 vtbl+48 AsSWSPlaceable
            at<U>(placeable, 0x278) = 1;                                            // 0051A5B0
            at<U8>(placeable, 0x290) = static_cast<U8>(row);                        // 0051A5BA
            assign_string_005e5c50(bytes(placeable) + 0x2e4, nullptr, script);      // 0051A5A5..C0
            at<U8>(placeable, 0x276) = static_cast<U8>(static_cast<U8>(rank) + static_cast<U8>(roll) + static_cast<U8>(detect)); // 0051A5C5..D7
            at<U8>(placeable, 0x27c) = static_cast<U8>(static_cast<U8>(disarm) + static_cast<U8>(rank) + static_cast<U8>(roll)); // 0051A5DD..E5
            at<U>(placeable, 0x284) = 1; at<U>(placeable, 0x280) = 1; at<U>(placeable, 0x288) = 1; // 0051A5EB..FC
            gui_load_array_add_00671c00(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(
                bytes(recovered_object_area_004cb120(self, nullptr)) + 0x12c), nullptr, at<U>(placeable, 4)); // 0051A602 -> 0051A578..89
        } else {
            void* memory = live_operator_new(0x318);                                // 0051A607..11
            void* trigger = memory != nullptr ? recovered_trigger_construct_0058eae0(memory, nullptr, k_object_invalid) : nullptr; // 0051A618..34
            recovered_trigger_creator_0058d700(trigger, nullptr, at<U>(self, 4));   // 0051A636..44
            at<U8>(trigger, 0x2d4) = static_cast<U8>(row);                          // 0051A654
            assign_string_005e5c50(bytes(trigger) + 0x264, nullptr, script);        // 0051A649..5A
            at<I>(trigger, 0x2d8) = detect + roll + rank;                           // 0051A65F..70
            at<I>(trigger, 0x2dc) = disarm + roll + rank;                           // 0051A676..80
            at<U>(trigger, 0x2bc) = 1; at<U>(trigger, 0x2c0) = 1; at<U>(trigger, 0x2c4) = 1; // 0051A686..A2
            recovered_object_position_004cd1a0(trigger, nullptr,
                reinterpret_cast<const Vector*>(bytes(self) + 0x90), 1, 1, 0);      // 0051A67E..A8
            at<U>(trigger, 0x2b8) = at<U>(stats_of(self), 0x78);                    // 0051A6AD..B6
            I name = 0;                                                             // [esp+3C] (0051A6C5)
            table_int(row, k_trap_name_00747d44, &name);                            // 0051A6BC..0A
            at<I>(trigger, 0x22c) = name;                                           // 0051A70F..15
            kotor_fastcall_0058d210(trigger, nullptr, flt(k_float_2_0), &location,
                                    recovered_object_area_004cb120(self, nullptr)); // 0051A71B..2D
            kotor_fastcall_0058f030(trigger, nullptr, recovered_object_area_004cb120(self, nullptr),
                at<float>(self, 0x90), at<float>(self, 0x94), at<float>(self, 0x98), 1); // 0051A732..50
            gui_load_array_add_00671c00(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(
                bytes(recovered_object_area_004cb120(self, nullptr)) + 0x12c), nullptr, at<U>(trigger, 4)); // 0051A755..66
        }
        // consume the mine item
        void* repository = recovered_creature_inventory_004ef770(self, nullptr, 1);  // 0051A773..7A
        if (recovered_item_in_repository_00555f40(repository, nullptr, item, 1) != 0) { // 0051A77F..88
            if (at<I>(item, 0x28c) > 1) {                                           // 0051A78E..97 (jle: signed)
                at<I>(item, 0x28c) -= 1;                                            // 0051A799..9A
            } else if (recovered_creature_remove_item_00510ef0(self, nullptr, item, 1, 1, nullptr) == 1) { // 0051A7A2..B3
                void* player = kotor_fastcall_004ae910(server_app(), nullptr, at<U>(self, 4)); // 0051A7B5..C2
                if (player != nullptr)                                              // 0051A7C7..C9
                    recovered_item_update_notify_00569fc0(combat_reuse_message_004aedb0(), nullptr, player,
                                                          at<U>(item, 0x14));       // 0051A7CB..E0
                enqueue(at<U>(self, 4), at<U>(item, 0x14), k_event_destroy_007441e0, nullptr); // 0051A7E5..0A
            }
        }
        // a critical failure on an existing trigger / door / placeable sets it off at the user
        if (take20 == 0 && total < dc - 0xa && object != nullptr) {                 // 0051A80F..35
            void* event = nullptr;
            bool fire = true;
            if (vcall<void*>(object, 0x40) != nullptr) {                            // 0051A83B..47 vtbl+40 AsSWSTrigger
                at<U>(vcall<void*>(object, 0x40), 0x2bc) = 1;                       // 0051A849..51
                event = new_script_event(0xc);                                      // 0051A84F..7C
                recovered_event_object_004d7780(event, nullptr, 0, at<U>(self, 4)); // 0051A881..91
            } else if (vcall<void*>(object, 0x48) != nullptr || vcall<void*>(object, 0x18) != nullptr) { // 0051A898..AC
                event = new_script_event(0x1a);                                     // 0051A8AE..DD
            } else {
                fire = false;                                                       // 0051A8AC -> 0051A916
            }
            if (fire) {
                recovered_event_integer_004d76b0(event, nullptr, 0, 1);             // 0051A8E2..E7
                enqueue(at<U>(self, 4), target, k_event_script_007441dc, event);    // 0051A8EC..11
            }
        }
    }
    // the result message (CC skill data 5FAh) and voice chat
    {
        void* memory = live_operator_new(0x34);                                     // 0051A916..1D
        void* data = memory != nullptr ? recovered_message_data_construct_004d69f0(memory, nullptr) : nullptr; // 0051A924..3B
        recovered_event_object_004d7780(data, nullptr, 0, at<U>(self, 4));          // 0051A93D..4D
        recovered_event_integer_004d76b0(data, nullptr, 0, 0x5fa);                  // 0051A952..5B
        recovered_event_integer_004d76b0(data, nullptr, 1, static_cast<U>(roll));   // 0051A960..69
        recovered_event_integer_004d76b0(data, nullptr, 2, static_cast<U>(rank));   // 0051A96E..77
        recovered_event_integer_004d76b0(data, nullptr, 3, static_cast<U>(dc));     // 0051A97C..85
        recovered_event_integer_004d76b0(data, nullptr, 4, take20);                 // 0051A98A..93
        U success = 0;                                                              // EBX
        if (total >= dc) {                                                          // 0051A998..A4 (jl: signed)
            recovered_event_integer_004d76b0(data, nullptr, 5, 1);                  // 0051A9A6..AC
            success = 1;
        } else if (take20 != 0) {
            recovered_event_integer_004d76b0(data, nullptr, 5, 3);                  // 0051A9B8..BE
        } else if (total < dc - 0xa) {
            recovered_event_integer_004d76b0(data, nullptr, 5, 2);                  // 0051A9C0..C9
        } else {
            recovered_event_integer_004d76b0(data, nullptr, 5, 0);                  // 0051A9CB
        }
        recovered_event_integer_004d76b0(data, nullptr, 6, 0x144);                  // 0051A9D6..DF
        kotor_fastcall_004ec700(self, nullptr, data);                               // 0051A9E4..E7
        w5_creature_broadcast_voice_chat_004f2520(self, nullptr, success != 0 ? 0x13u : 0x18u); // 0051A9EC..FA
        kotor_fastcall_004eb1b0(self, nullptr, 0xe);                                // 0051A9FF..03
        kotor_fastcall_004f5f70(self, nullptr);                                     // 0051AA08..0A
        recovered_creature_cancel_action_004ef4d0(self, nullptr);                   // 0051AA0F..11
        at<U>(self, 0x97c) = 0;                                                     // 0051AA16
        result = k_complete_00745dc0;                                               // 0051AA20
    }
    }
done:
    frame_LogStringDtor_005e5c20(script, nullptr);                                  // 0051AA26..35
    return result;
}
}  // extern "C"
