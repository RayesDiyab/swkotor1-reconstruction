// claude_encounter.cpp (lane "encounter", core): CSWSEncounter object lifetime, area attach, AI update / events,
// activation, difficulty, spawn-pool arithmetic and the NWScript SetEncounterData command, transcribed from the
// original swkotor.exe (read-only, capstone). Loaders: claude_encounter_load.cpp; spawn chain: claude_encounter_spawn.cpp.
//
//   0058FF80 CEncounterListEntry::CEncounterListEntry()          RET 0    @kotor_fastcall_0058ff80@8
//   0058FFC0 EncounterSpawnPoint::EncounterSpawnPoint()          RET 0    @kotor_fastcall_0058ffc0@8
//   00590250 CEncounterListEntry::CEncounterListEntry(const&)    RET 4    @kotor_fastcall_00590250@12
//   00590290 CSWSEncounter::ActivateAreaListContains(id)         RET 4    @kotor_fastcall_00590290@12
//   005902D0 CSWSEncounter::AddToActivateAreaList(id)            RET 4    @kotor_fastcall_005902d0@12
//   005903B0 CSWSEncounter::RemoveFromActivateAreaList(id)       RET 4    @kotor_fastcall_005903b0@12
//   00590A60 CSWSEncounter::SetDifficulty(int)                   RET 4    @kotor_fastcall_00590a60@12
//   00592210 CSWSEncounter::RemoveFromArea()                     RET 0    @kotor_fastcall_00592210@8
//   005931D0 CSWSEncounter::CalculateSpawnPool(float cr)         RET 4    @kotor_fastcall_005931d0@12
//   00593270 CSWSEncounter::LoadFractionalCRData()               RET 0    @kotor_fastcall_00593270@8
//   00593380 CSWSEncounter::~CSWSEncounter                       RET 0    @kotor_fastcall_00593380@8
//   00593510 CSWSEncounter::GetFirstName()                       RET 0    @kotor_fastcall_00593510@8
//   00593520 CSWSEncounter::AddToArea(area, x, y, z, flag)       RET 14h  @kotor_fastcall_00593520@28
//   00593BE0 CSWSEncounter::SpawnIfAppropriate(id, float cr)     RET 8    @kotor_fastcall_00593be0@16
//   00593C50 CSWSEncounter::SetActive(int)                       RET 4    @kotor_fastcall_00593c50@12
//   00593C70 CSWSEncounter::CSWSEncounter(id)                    RET 4    @kotor_fastcall_00593c70@12
//   00593F90 CSWSEncounter scalar deleting destructor            RET 4    @kotor_fastcall_00593f90@12
//   00593FB0 CSWSEncounter::AIUpdate()                           RET 0    @kotor_fastcall_00593fb0@8
//   00594220 CSWSEncounter::EventHandler(type, caller, event, a, b) RET 14h @kotor_fastcall_00594220@28
//   00542760 CSWVirtualMachineCommands::ExecuteCommandSetEncounterData(routine, argc) RET 8 @kotor_fastcall_00542760@16
//            (routines 0x115 SetEncounterActive 277, 0x117 SetEncounterSpawnsMax 279, 0x119 SetEncounterSpawnsCurrent 281,
//             0x128 SetEncounterDifficulty 296)
//   00749D98 CSWSEncounter vtable (56 slots, verified slot by slot against the original .rdata) _kotor_vtable_00749d98
//
// Quirks kept (documented in place): the constructor appends 51 more powers of 1.5 to the global CR->points table every
// time an encounter is constructed (the table only grows; 00832860 data / 00832864 size / 00832868 capacity);
// AddToActivateAreaList's growth copy reads one element past the old buffer (loop bound <= size); LoadFractionalCRData
// leaves +310 unset when the fractional-CR 2DA is absent and reuses the previous row's float when a GetFLOATEntry
// fails; the EventHandler ignores every non-script event (no destroy handling) and destroys only script events;
// AIUpdate's heartbeat event dereferences a failed allocation; SetDifficulty leaves +248 unchanged for an out-of-range
// row but writes 0 when the difficulty 2DA is missing.
#include "d8bbdade43-claude_encounter_common.hpp"

extern "C" {
// ---- additional existing providers (c40 map) ----
U __fastcall recovered_vm_outer_pop_integer_005d1000(void* vm, void*, I* out);                  // StackPopInteger [vm_actions_t1]
U __fastcall recovered_vm_outer_pop_object_005d10c0(void* vm, void*, U* out);                   // StackPopObject [1196]

// ---- generic slot bodies (as the CSWSStore table 0074BDB8 in claude_aoe_store) ----
void __fastcall slot_void_return_4_0060e760_a1(void*, void*, U);
void __fastcall kotor_m15e_slot008_void8_005b5e90(void*, void*, U, U);
U __fastcall kotor_m15e_slot010_zero_0063e7f0(void*, void*);
void* __fastcall kotor_m15e_slot00c_self_00641db0(void*, void*);
U __fastcall kotor_m15e_slot0d4_true_0063e7a0(void*, void*);
U __fastcall kotor_fastcall_004cc390(void*, void*, void*, U);
void __fastcall kotor_fastcall_004ccf60(void*, void*, U);
void* __fastcall kotor_fastcall_004d0170(void*, void*, void*);
U8 __fastcall kotor_fastcall_004cb190(void*, void*);
void* __fastcall kotor_fastcall_004d0190(void*, void*);
U __fastcall kotor_fastcall_004cb810(void*, void*);
U16 __fastcall kotor_fastcall_004d01a0(void*, void*, U);
U __fastcall kotor_fastcall_004caec0(void*, void*, U);
int __fastcall kotor_fastcall_004caee0(void*, void*, U);
int __fastcall kotor_fastcall_004caf70(void*, void*, U);
U __fastcall kotor_fastcall_004d01b0(void*, void*);
void* __fastcall kotor_fastcall_004d01c0(void*, void*, void*);
void __fastcall kotor_fastcall_004ccf80(void* object, void*, I damage);
int __fastcall kotor_fastcall_004cf160(void*, void*, void*, int, U, U, U);
int __fastcall kotor_fastcall_004d09e0(void*, void*, void*, int, U, U, U);
int __fastcall kotor_fastcall_004d0e40(void*, void*, void*, int, U, U, U, U);
void __fastcall recovered_object_portrait_ref_004d01f0(void*, void*, void*);
U __fastcall recovered_creature_portrait_004d0210(void*, void*);
void __fastcall recovered_object_portrait_004cad20(void*, void*, U);
U __stdcall c2da_resource_constant_word_00406490();
}

namespace {
using namespace enc;
constexpr I k_pop_failed = static_cast<I>(0xfffff82fu);   // -2001 (VM stack pop failure)
const double k_points_base_00749d78 = 1.5;                // .rdata 00749D78
const double k_quarter_00749d88 = 0.25;                   // .rdata 00749D88
const double k_half_0073f3d0 = 0.5;                       // .rdata 0073F3D0
const float k_minus_one_00740cbc = -1.0f;                 // .rdata 00740CBC
const float k_zero_0073d700 = 0.0f;                       // .rdata 0073D700
const char k_value_00749bc0[] = "VALUE";                  // .rdata 00749BC0
const char k_min_00749d90[] = "Min";                      // .rdata 00749D90
inline float minus_one() { return k_minus_one_00740cbc; }
// fldln2; fld v; fyl2x  (natural log exactly as the x87 sequence of 0059321A..28; no CALL)
__declspec(noinline) double x87_ln(double v) {
    double r;
    __asm {
        fldln2
        fld qword ptr v
        fyl2x
        fstp qword ptr r
    }
    return r;
}
struct FloatList { float* data; I size; I capacity; };   // CExoArrayList<float>
}

extern "C" {
// .bss 00832864 / 00832868: size and capacity of the CR->points table whose data pointer (00832860) is
// w5_cell_encounter_points_table_00832860 in claude_w5_creature_actions.
I w5_cell_encounter_points_size_00832864 = 0;
I w5_cell_encounter_points_capacity_00832868 = 0;

// 0058FF80 CEncounterListEntry::CEncounterListEntry(): RET 0.
void* __fastcall kotor_fastcall_0058ff80(void* self, void*) {
    kotor_fastcall_00405ed0(self, nullptr);                                                      // 0058FF84..86
    ResRef temp;                                                                                 // [esp+4]
    unresolved_ref_assign_00406290(self, nullptr, &temp, "");                                    // 0058FF8B..97 ([0073D71C])
    at<U>(self, 0x10) = 0; at<U>(self, 0x14) = 0; at<U>(self, 0x20) = 0;                         // 0058FF9C..A4
    at<U>(self, 0x18) = 0; at<U>(self, 0x1c) = 0;                                                // 0058FFA7..AA
    return self;
}

// 0058FFC0 EncounterSpawnPoint::EncounterSpawnPoint(): RET 0, four dwords zeroed.
void* __fastcall kotor_fastcall_0058ffc0(void* self, void*) {
    at<U>(self, 0) = 0; at<U>(self, 4) = 0; at<U>(self, 8) = 0; at<U>(self, 0xc) = 0;            // 0058FFC0..CC
    return self;
}

// 00590250 CEncounterListEntry::CEncounterListEntry(CEncounterListEntry const&): RET 4.
void* __fastcall kotor_fastcall_00590250(void* self, void*, const void* other) {
    ResRef temp;                                                                                 // [esp+8]
    ref_assign_copy_00406120(self, nullptr, &temp, other);                                       // 00590255..61
    void* o = const_cast<void*>(other);
    at<U>(self, 0x10) = at<U>(o, 0x10); at<U>(self, 0x14) = at<U>(o, 0x14);                      // 00590266..6F
    at<U>(self, 0x18) = at<U>(o, 0x18); at<U>(self, 0x1c) = at<U>(o, 0x1c);                      // 00590272..7B
    at<U>(self, 0x20) = at<U>(o, 0x20);                                                          // 0059027E..82
    return self;
}

// 00590290 CSWSEncounter::ActivateAreaListContains(id): RET 4, EAX 0/1.
U __fastcall kotor_fastcall_00590290(void* self, void*, U id) {
    const I size = at<I>(self, 0x288);                                                           // 00590290 (signed)
    const U* data = at<U*>(self, 0x284);
    for (I i = 0; i < size; ++i) if (data[i] == id) return 1;                                    // 005902A7..B1
    return 0;
}

// 005902D0 CSWSEncounter::AddToActivateAreaList(id): RET 4. Appends when absent; grows by 16.
void __fastcall kotor_fastcall_005902d0(void* self, void*, U id) {
    const I size = at<I>(self, 0x288);                                                           // 005902D4 (edx, cached)
    for (I i = 0; i < at<I>(self, 0x288); ++i)                                                   // 005902F0..04 (bound re-read)
        if (at<U*>(self, 0x284)[i] == id) return;
    const I capacity = at<I>(self, 0x280);
    if (size < capacity) {                                                                       // 00590306..0E (signed)
        at<U*>(self, 0x284)[size] = id;                                                          // 00590310..16
        at<I>(self, 0x288) = at<I>(self, 0x288) + 1;                                             // 00590319..21
        return;
    }
    U* old = at<U*>(self, 0x284);                                                                // 0059032C
    at<I>(self, 0x280) = capacity + 0x10;                                                        // 00590332..3D
    U* grown = static_cast<U*>(live_operator_new(static_cast<U>(capacity + 0x10) * 4));          // 00590343
    // 00590351..71: copies indices 0..size INCLUSIVE (jle) - reads one dword past the old buffer (original quirk)
    if (at<I>(self, 0x288) >= 0)
        for (I i = 0; i <= at<I>(self, 0x288); ++i) grown[i] = old[i];
    grown[at<I>(self, 0x288)] = id;                                                              // 00590378..7E
    at<I>(self, 0x288) = at<I>(self, 0x288) + 1;                                                 // 00590381..89
    at<U*>(self, 0x284) = grown;                                                                 // 0059038F
    live_free(old);                                                                              // 00590388..95
}

// 005903B0 CSWSEncounter::RemoveFromActivateAreaList(id): RET 4. Compacts every occurrence.
void __fastcall kotor_fastcall_005903b0(void* self, void*, U id) {
    I removed = 0;                                                                               // edi
    for (I i = 0; i < at<I>(self, 0x288); ++i) {                                                 // 005903D0..F4 (re-read)
        U* data = at<U*>(self, 0x284);
        const U v = data[i];
        if (v == id) ++removed;                                                                  // 005903D9..DD
        else if (removed != 0) data[i - removed] = v;                                            // 005903E0..E8
    }
    at<I>(self, 0x288) -= removed;                                                               // 005903F9
}

// 00590A60 CSWSEncounter::SetDifficulty(int index): RET 4. +248 = (int)EncDifficulty[index]."VALUE".
void __fastcall kotor_fastcall_00590a60(void* self, void*, I index) {
    float value = 0.0f;                                                                          // [esp+C] 00590A7E
    void* table = at<void*>(two_da(), 0xa8);                                                     // 00590A78..8E
    if (table != nullptr) {                                                                      // 00590A94..98
        if (index >= at<I>(table, 0x24) || index < 0) return;                                    // 00590AA0..A9 (signed)
        ExoString column; frame_LogStringCtor_005e5a90(&column, nullptr, k_value_00749bc0);      // 00590AAB..B4
        (void)c2da_current_get_float_00413430(table, nullptr, index, &column, &value);           // 00590AB9..CE
        frame_LogStringDtor_005e5c20(&column, nullptr);                                          // 00590AD3..DF
    }
    at<U>(self, 0x248) = ftol2_low(static_cast<double>(value));                                  // 00590AE4..ED
}

// 00592210 CSWSEncounter::RemoveFromArea(): RET 0.
void __fastcall kotor_fastcall_00592210(void* self, void*) {
    if (recovered_object_area_004cb120(self, nullptr) != nullptr) {                              // 00592214..1D
        (void)kotor_fastcall_0050dc40(recovered_object_area_004cb120(self, nullptr), nullptr, at<U>(self, 4)); // 0059221F..2C
        recovered_int_list_remove_0066dcc0(bytes(recovered_object_area_004cb120(self, nullptr)) + 0x1a0, nullptr,
                                           at<U>(self, 4));                                      // 00592231..42
    }
    kotor_fastcall_004cb160(self, nullptr, nullptr);                                             // 00592247..4B
    const Vector origin = {0.0f, 0.0f, 0.0f};                                                    // [esp+4] 0059225D..6D
    recovered_object_position_004cd1a0(self, nullptr, &origin, 1, 1, 0);                         // 00592250..75
}

// 005931D0 CSWSEncounter::CalculateSpawnPool(float cr): RET 4.
void __fastcall kotor_fastcall_005931d0(void* self, void*, float cr) {
    if (cr == k_minus_one_00740cbc) {                                                            // 005931D0..E2 (jp: unequal/NaN)
        kotor_fastcall_00591af0(self, nullptr);                                                  // 005931E4 TallyEnemyRadiusPoints
    } else {
        at<float>(self, 0x2d4) = static_cast<float>(kotor_fastcall_005919e0(self, nullptr, cr)); // 005931EB..F5
    }
    const float points = at<float>(self, 0x2d4);                                                 // 005931FB
    if (points == k_zero_0073d700) return;                                                       // 00593201..0C (jnp: equal)
    // 0059320E..32: ftol(ln(points * 0.25) / ln(1.5) + 0.5); each x87 step rounds to 53 bits (PC=53)
    const double scaled = static_cast<double>(at<float>(self, 0x2d4)) * k_quarter_00749d88;
    const double level = x87_ln(scaled) / x87_ln(k_points_base_00749d78) + k_half_0073f3d0;
    const U spawn_level = ftol2_low(level) + at<U>(self, 0x248);                                 // 00593232..3D
    at<U>(self, 0x2c0) = spawn_level;                                                            // 0059323F
    const float cr_below = static_cast<float>(static_cast<I>(spawn_level - 1));                  // 00593245..51 (fild; fstp dword)
    at<float>(self, 0x2d8) = static_cast<float>(kotor_fastcall_005919e0(self, nullptr, cr_below) +
                                                static_cast<double>(at<float>(self, 0x2d8)));    // 00593254..5F
}

// 00593270 CSWSEncounter::LoadFractionalCRData(): RET 0. +310 = new CExoArrayList<float> of FractionalCR[0..4]."Min".
void __fastcall kotor_fastcall_00593270(void* self, void*) {
    void* table = at<void*>(two_da(), 0x9c);                                                     // 0059328E..9B
    if (table == nullptr) return;                                                                // 005932A5..A9 (+310 left unset)
    FloatList* list = static_cast<FloatList*>(live_operator_new(0xc));                           // 005932AF..B1
    if (list != nullptr) { list->size = 0; list->capacity = 0; list->data = nullptr; }           // 005932BD..C3
    at<FloatList*>(self, 0x310) = list;                                                          // 005932D1
    float value = 0.0f;   // [esp+10]: never initialised by the original; a failed read reuses the previous row's value
    for (I row = 0; row < 5; ++row) {                                                            // 005932E0..5D
        ExoString column; frame_LogStringCtor_005e5a90(&column, nullptr, k_min_00749d90);         // 005932E0..E9
        (void)c2da_current_get_float_00413430(table, nullptr, row, &column, &value);             // 005932EE..303
        frame_LogStringDtor_005e5c20(&column, nullptr);                                          // 00593308..14
        FloatList* l = at<FloatList*>(self, 0x310);                                              // 0059331D (null deref on failed new)
        if (l->size == l->capacity) {                                                            // 00593323..2F
            menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(l), nullptr,
                static_cast<unsigned>(l->capacity != 0 ? l->capacity * 2 : 0x10));              // 00593331..41
        }
        l->data[l->size] = value;                                                                // 00593346..4F
        ++l->size;                                                                               // 00593352..5A
    }
}

// 00593380 CSWSEncounter::~CSWSEncounter: RET 0.
void __fastcall kotor_fastcall_00593380(void* self, void*) {
    at<const void*>(self, 0) = kotor_vtable_00749d98;                                            // 0059339F
    kotor_fastcall_00592210(self, nullptr);                                                      // 005933AD RemoveFromArea
    (void)recovered_ai_remove_004af3d0(ai(), nullptr, self);                                     // 005933B2..C2
    void* module = server_get_module_004ae6b0(server(), nullptr);                                // 005933C7..D0
    if (module != nullptr)
        (void)recovered_module_remove_tag_004c5ef0(module, nullptr, bytes(self) + 0x18, at<U>(self, 4)); // 005933D7..E5
    unsigned char* creatures = at<unsigned char*>(self, 0x2b0);                                  // 005933EA
    if (creatures != nullptr) {                                                                  // delete[] with count cookie
        c2da_destroy_array_006fa852(creatures, 0x24, at<I>(creatures - 4, 0),
                                    reinterpret_cast<void*>(&kotor_m15e_slot090_void0_004015a0)); // 005933F4..403
        live_free(creatures - 4);                                                                // 00593408..09
        at<void*>(self, 0x2b0) = nullptr;                                                        // 00593411
    }
    if (at<void*>(self, 0x284) != nullptr) { live_free(at<void*>(self, 0x284)); at<void*>(self, 0x284) = nullptr; } // 00593417..2A
    if (at<void*>(self, 0x290) != nullptr) { live_free(at<void*>(self, 0x290)); at<void*>(self, 0x290) = nullptr; } // 00593430..43
    if (at<void*>(self, 0x298) != nullptr) { live_free(at<void*>(self, 0x298)); at<void*>(self, 0x298) = nullptr; } // 00593449..5C
    if (at<void*>(self, 0x2b8) != nullptr) { live_free(at<void*>(self, 0x2b8)); at<void*>(self, 0x2b8) = nullptr; } // 00593462..75
    kotor_fastcall_005921a0(self, nullptr);                                                      // 0059347B..7D ClearSpawnList
    FloatList* fractional = at<FloatList*>(self, 0x310);                                         // 00593482
    if (fractional != nullptr) {                                                                 // 00593488..8A
        live_free(fractional->data);                                                             // 0059348C..8F
        fractional->data = nullptr;                                                              // 00593495
        live_free(fractional);                                                                   // 00593494..97 (+310 not cleared)
    }
    c2da_destroy_array_006fa852(bytes(self) + 0x2e8, 8, 5, reinterpret_cast<void*>(&frame_LogStringDtor_005e5c20)); // 0059349F..B4
    live_free(at<void*>(self, 0x258));                                                           // 005934B9..C5
    at<void*>(self, 0x258) = nullptr;                                                            // 005934D3
    exo_locstring_destroy_005ea000(bytes(self) + 0x230, nullptr);                                // 005934CD..DD
    recovered_object_destroy_004d0220(self, nullptr);                                            // 005934E2..EC
}

// 00593510 CSWSEncounter::GetFirstName(): RET 0, EAX = &LocalizedName (+230).
void* __fastcall kotor_fastcall_00593510(void* self, void*) { return bytes(self) + 0x230; }

// 00593520 CSWSEncounter::AddToArea(CSWSArea*, x, y, z, int flag): RET 14h.
void __fastcall kotor_fastcall_00593520(void* self, void*, void* area, float x, float y, float z, U flag) {
    kotor_fastcall_00592210(self, nullptr);                                                      // 00593526 RemoveFromArea
    if (area == nullptr) return;                                                                 // 0059352B..31
    kotor_fastcall_004cb160(self, nullptr, area);                                                // 00593538..3B
    const Vector pos = {x, y, z};                                                                // [esp+C] 00593540..61
    recovered_object_position_004cd1a0(self, nullptr, &pos, 0, 1, 0);                           // 0059354C..65
    (void)recovered_area_insert_0050dfd0(recovered_object_area_004cb120(self, nullptr), nullptr, at<U>(self, 4), flag); // 0059356A..7C
    // 00593581..BB: area +1A0 CExoArrayList<ulong> encounter ids .Add(id)
    struct IdList { U* data; I size; I capacity; };
    IdList* list = reinterpret_cast<IdList*>(bytes(recovered_object_area_004cb120(self, nullptr)) + 0x1a0);
    const U id = at<U>(self, 4);
    if (list->size == list->capacity) {                                                          // 00593597..9C
        menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(list), nullptr,
            static_cast<unsigned>(list->capacity != 0 ? list->capacity * 2 : 0x10));             // 0059359E..AE
    }
    list->data[list->size] = id;                                                                 // 005935B3..B8
    ++list->size;                                                                                // 005935BB
}

// 00593BE0 CSWSEncounter::SpawnIfAppropriate(id, float cr): RET 8.
void __fastcall kotor_fastcall_00593be0(void* self, void*, U id, float cr) {
    if (at<U>(self, 0x238) == 0) return;                                                         // 00593BE3..EB
    if (at<U>(self, 0x278) == 1) return;                                                         // 00593BED..F4
    at<U>(self, 0x28c) = 0;                                                                      // 00593BFB
    kotor_fastcall_00590da0(self, nullptr, id);                                                  // 00593BF6..05
    if (at<U>(self, 0x28c) != 1) return;                                                         // 00593C0A..11
    kotor_fastcall_005931d0(self, nullptr, cr);                                                  // 00593C13..1A
    if (!(at<float>(self, 0x2d8) > k_zero_0073d700)) return;                                     // 00593C1F..30 (test ah,41h)
    kotor_fastcall_005935d0(self, nullptr);                                                      // 00593C32..34 BuildSpawnList
    at<U>(self, 0x278) = 1;                                                                      // 00593C39
}

// 00593C50 CSWSEncounter::SetActive(int): RET 4.
void __fastcall kotor_fastcall_00593c50(void* self, void*, U active) {
    at<U>(self, 0x238) = active;                                                                 // 00593C56
    if (active != 0) kotor_fastcall_00593be0(self, nullptr, k_invalid_00746644, minus_one());    // 00593C5C..68
}

// 00593C70 CSWSEncounter::CSWSEncounter(ulong id): RET 4.
void* __fastcall kotor_fastcall_00593c70(void* self, void*, U id) {
    kotor_fastcall_004cfcb0(self, nullptr, k_encounter_type_0074663c, id, 0);                    // 00593C8C..9C (0Dh)
    at<const void*>(self, 0) = kotor_vtable_00749d98;                                            // 00593CAB
    exo_locstring_construct_005e9e10(bytes(self) + 0x230, nullptr);                              // 00593CA1..B1
    at<U>(self, 0x25c) = 0; at<U>(self, 0x260) = 0; at<U>(self, 0x258) = 0;                      // 00593CB6..C2
    crt_eh_vector_constructor_006fb55f(bytes(self) + 0x2e8, 8, 5,
        reinterpret_cast<void*>(&kotor_movie::string_element_construct_005b3190),
        reinterpret_cast<void*>(&frame_LogStringDtor_005e5c20));                                 // 00593CC8..E2
    (void)kotor_fastcall_004b0850(ai(), nullptr, self, 0);                                       // 00593CE7..FE
    U day = 0, time = 0;                                                                         // [esp+10], [esp+14]
    frame_GetWorldTime_004ade40(world_timer(), nullptr, &day, &time);                            // 00593D03..1C
    at<U>(self, 0xa8) = day; at<U>(self, 0xac) = time;                                           // 00593D21..31
    at<U>(self, 0x270) = 0; at<U>(self, 0x274) = 0; at<U>(self, 0x26c) = 0; at<U>(self, 0x268) = 0; // 00593D37..49
    at<U>(self, 0x2b4) = 0; at<U>(self, 0x2b0) = 0;                                              // 00593D4F..55
    at<U>(self, 0x280) = 0x10;                                                                   // 00593D5B
    at<void*>(self, 0x284) = live_operator_new(0x40);                                            // 00593D29/65..6A
    at<U>(self, 0x288) = 0; at<U>(self, 0x28c) = 0; at<U>(self, 0x290) = 0;                      // 00593D78..84
    at<U>(self, 0x298) = 0; at<U>(self, 0x294) = 0; at<U>(self, 0x29c) = 0;                      // 00593D8A..96
    at<U>(self, 0x2e0) = k_invalid_00746644;                                                     // 00593D9C
    at<U>(self, 0x2c0) = 0; at<U>(self, 0x2bc) = 0; at<U>(self, 0x2c8) = 0; at<U>(self, 0x264) = 0; // 00593DA2..B4
    at<U>(self, 0x2d8) = 0; at<U>(self, 0x2dc) = 0; at<U>(self, 0x2cc) = 0; at<U>(self, 0x2d0) = 0; // 00593DBA..CC
    at<U>(self, 0x2e4) = k_invalid_00746644;                                                     // 00593DD2
    at<U>(self, 0x2b8) = 0; at<U>(self, 0x278) = 0;                                              // 00593DD8..E0
    // 00593DE6..9C: append (float)pow(1.5, i) for i = 0..50 to the global table (every construction - quirk kept)
    for (I i = 0; i <= 0x32; ++i) {
        const float points = static_cast<float>(x87_pow(k_points_base_00749d78, static_cast<double>(i))); // 00593DF0..FF
        if (w5_cell_encounter_points_size_00832864 == w5_cell_encounter_points_capacity_00832868) { // 00593E03..0E
            const I capacity = w5_cell_encounter_points_capacity_00832868 != 0
                                   ? w5_cell_encounter_points_capacity_00832868 * 2 : 0x10;      // 00593E10..18
            float* old = w5_cell_encounter_points_table_00832860;                                // 00593E1D
            w5_cell_encounter_points_capacity_00832868 = capacity;                               // 00593E23
            w5_cell_encounter_points_table_00832860 =
                static_cast<float*>(live_operator_new(static_cast<U>(capacity) * 4));            // 00593E28..37
            for (I k = 0; k < w5_cell_encounter_points_size_00832864; ++k)                       // 00593E3F..66 (re-read)
                w5_cell_encounter_points_table_00832860[k] = old[k];
            if (old != nullptr) live_free(old);                                                  // 00593E68..6D
        }
        w5_cell_encounter_points_table_00832860[w5_cell_encounter_points_size_00832864] = points; // 00593E75..84
        ++w5_cell_encounter_points_size_00832864;                                                // 00593E87..92
    }
    kotor_fastcall_00593270(self, nullptr);                                                      // 00593EA2..A4 LoadFractionalCRData
    const U spawned = at<U>(self, 0x278);                                                        // 00593EA9 (read before +238)
    at<U>(self, 0x238) = 1;                                                                      // 00593EB6
    if (spawned != 1) {                                                                          // 00593EB4..BC
        at<U>(self, 0x28c) = 0;                                                                  // 00593EC5
        kotor_fastcall_00590da0(self, nullptr, k_invalid_00746644);                              // 00593EBE..CB
        if (at<U>(self, 0x28c) == 1) {                                                           // 00593ED0..D6
            kotor_fastcall_005931d0(self, nullptr, minus_one());                                 // 00593ED8..DF
            if (at<float>(self, 0x2d8) > k_zero_0073d700) {                                      // 00593EE4..F5
                kotor_fastcall_005935d0(self, nullptr);                                          // 00593EF7..F9
                at<U>(self, 0x278) = 1;                                                          // 00593EFE
            }
        }
    }
    at<U>(self, 0x244) = 0; at<U>(self, 0x23c) = 0; at<U>(self, 0x240) = 0x3c;                   // 00593F0E..1A
    at<U>(self, 0x22c) = 1; at<U>(self, 0x248) = 0;                                              // 00593F24..2A
    const Vector origin = {0.0f, 0.0f, 0.0f};                                                    // [esp+1C] 00593F30..40
    recovered_object_position_004cd1a0(self, nullptr, &origin, 1, 1, 0);                         // 00593F04..48
    at<U>(self, 0x2c4) = 0; at<U>(self, 0x318) = 0;                                              // 00593F51..57
    at<U>(self, 0x254) = 8; at<U>(self, 0x250) = 2;                                              // 00593F5E..68
    return self;
}

// 00593F90 scalar deleting destructor: RET 4.
void* __fastcall kotor_fastcall_00593f90(void* self, void*, U flags) {
    kotor_fastcall_00593380(self, nullptr);                                                      // 00593F91..93
    if (flags & 1) live_free(self);                                                              // 00593F98..A5
    return self;
}

// 00593FB0 CSWSEncounter::AIUpdate(): RET 0.
void __fastcall kotor_fastcall_00593fb0(void* self, void*) {
    kotor_fastcall_00591ca0(self, nullptr);                                                      // 00593FCB..CD SpawnCreatureFromSpawnList
    const unsigned long long hr = recovered_bridge_exo_timers_high_resolution_005e9260(
        at<void*>(kotor_menu_10_19::dependencies::base_007a39e0, 4), nullptr);                   // 00593FD2..DA -> [esp+30/34]
    const U hr_low = static_cast<U>(hr), hr_high = static_cast<U>(hr >> 32);
    U day = 0, time = 0;                                                                         // [esp+10], [esp+0C]
    frame_GetWorldTime_004ade40(world_timer(), nullptr, &day, &time);                            // 00593FDF..4001
    U since_day = 0, since_time = 0;                                                             // [esp+24], [esp+14]
    (void)recovered_world_time_difference_004ae460(world_timer(), nullptr, day, time, at<U>(self, 0xa8), at<U>(self, 0xac),
                                                   &since_day, &since_time);                     // 00594006..38
    U beat_day = 0, beat_time = 0;                                                               // [esp+28], [esp+20]
    (void)recovered_world_time_difference_004ae460(world_timer(), nullptr, day, time, at<U>(self, 0x268), at<U>(self, 0x26c),
                                                   &beat_day, &beat_time);                       // 0059403D..6F
    at<U>(self, 0xcc) = since_time;                                                              // 00594074..80
    at<U>(self, 0xa8) = day; at<U>(self, 0xac) = time;                                           // 00594095..9B
    if (at<U>(self, 0x244) == 1 && at<U>(self, 0x23c) == 1 &&                                    // 00594086..AD SpawnOption, Reset
        at<U>(self, 0x270) != 0 && at<U>(self, 0x274) != 0) {                                    // 005940B3..C9
        U reset_day = 0, reset_time = 0;                                                         // [esp+1C], [esp+18]
        (void)recovered_world_time_difference_004ae460(world_timer(), nullptr, day, time, at<U>(self, 0x270),
                                                       at<U>(self, 0x274), &reset_day, &reset_time); // 005940CF..F6
        if (reset_time >= at<U>(self, 0x240) * 0x3e8u || reset_day >= 1) {                       // 005940FB..14 (unsigned)
            const I current = at<I>(self, 0x2c8), maximum = at<I>(self, 0x2c4);
            if (current < maximum || maximum == -1) {                                            // 00594116..29 (signed)
                at<I>(self, 0x2c8) = current + 1;                                                // 00594130..38
                at<U>(self, 0x270) = 0; at<U>(self, 0x274) = 0;                                  // 0059413E..44
                at<U>(self, 0x238) = 1;                                                          // 0059414A
                kotor_fastcall_00593be0(self, nullptr, k_invalid_00746644, minus_one());         // 0059412B..50
                at<U>(self, 0x2d8) = 0; at<U>(self, 0x2dc) = 0;                                  // 00594155..5B
            }
        }
    }
    if (at<U>(self, 0x238) == 0) return;                                                         // 00594161..67
    if (beat_time >= 0x1770u) {                                                                  // 0059416D..75 (6 s, unsigned)
        if (at<U>(self, 0x26c) != 0) {                                                           // 00594177..7D
            void* memory = live_operator_new(0x34);                                              // 0059417F..81
            void* event = memory != nullptr ? recovered_event_construct_004d7540(memory, nullptr) : nullptr; // 0059418D..A0
            at<U16>(event, 0) = 0;                                                               // 005941A2 OnHeartbeat (null deref kept)
            recovered_event_object_004d7780(event, nullptr, 0, at<U>(self, 4));                  // 005941A5..B4
            (void)recovered_event_enqueue_004b08d0(ai(), nullptr, 0, 0, at<U>(self, 4), at<U>(self, 4),
                                                   k_event_script_007441dc, event);              // 005941B9..D8
        }
        at<U>(self, 0x26c) = time; at<U>(self, 0x268) = day;                                     // 005941DD..EB
    }
    (void)recovered_object_run_actions_0057f4a0(self, nullptr, day, time, hr_low, hr_high);     // 005941F1..207
}

// 00594220 CSWSEncounter::EventHandler(type, caller, event, a, b): RET 14h.
void __fastcall kotor_fastcall_00594220(void* self, void*, U type, U, void* event, U, U) {
    if (type != k_event_script_007441dc) return;                                                 // 00594220..2D
    void* script = nullptr;
    const U16 kind = at<U16>(event, 0);                                                          // 00594238
    if (kind == 0xc) {                                                                           // 0059423B OnEntered
        at<U>(self, 0x2e0) = recovered_message_get_object_004d64c0(event, nullptr, 0);           // 00594241..53
        kotor_fastcall_00593be0(self, nullptr, recovered_message_get_object_004d64c0(event, nullptr, 0), minus_one()); // 0059424A..61
        script = bytes(self) + 0x2e8;                                                            // 0059426C
    } else if (kind == 0xd) {                                                                    // 00594274 OnExit
        at<U>(self, 0x2e4) = recovered_message_get_object_004d64c0(event, nullptr, 0);           // 0059427A..8A
        kotor_fastcall_00590da0(self, nullptr, k_invalid_00746644);                              // 00594283..90
        script = bytes(self) + 0x2f0;                                                            // 00594298
    } else if (kind == 0) {                                                                      // 005942A0 OnHeartbeat
        script = bytes(self) + 0x2f8;                                                            // 005942AB
    } else if (kind == 0x15) {                                                                   // 005942B3 OnExhausted
        script = bytes(self) + 0x300;                                                            // 005942BF
    } else if (kind == 0xb) {                                                                    // 005942C7 OnUserDefined
        at<I>(self, 0x314) = recovered_message_get_integer_004d6480(event, nullptr, 0);          // 005942CD..D9
        script = bytes(self) + 0x308;                                                            // 005942DF
    }
    if (script != nullptr) frame_R4_005d0fc0(vm(), nullptr, script, at<U>(self, 4), 1);          // 005942E5..EF
    recovered_event_destroy_004d7590(event, nullptr);                                            // 005942F4..F6
    live_free(event);                                                                            // 005942FB..FC
}

// 00542760 ExecuteCommandSetEncounterData(routine, argc): RET 8. Pops the int, then (argc > 1) the encounter object.
int __fastcall kotor_fastcall_00542760(void*, void*, int routine, unsigned argc) {
    I value = 0;                                                                                 // [esp]
    U id = k_invalid_00746644;                                                                   // [esp+4] 0054276E
    if (recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &value) == 0) return k_pop_failed; // 00542763..7D
    if (static_cast<I>(argc) > 1 && recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &id) == 0) // 0054277F..98 (jle signed)
        return k_pop_failed;
    void* encounter = kotor_fastcall_004ae7f0(server(), nullptr, id);                            // 005427A5..B3
    if (encounter == nullptr) return 0;                                                          // 005427B8..BC
    if (routine == 0x115) { kotor_fastcall_00593c50(encounter, nullptr, static_cast<U>(value)); return 0; } // 005427BE..D8
    if (routine == 0x128) { kotor_fastcall_00590a60(encounter, nullptr, value); return 0; }     // 005427DB..F1
    if (routine == 0x117) { at<I>(encounter, 0x2c4) = value; return 0; }                        // 005427F4..0A
    if (routine == 0x119) at<I>(encounter, 0x2c8) = value;                                      // 0054280D..18
    return 0;
}

// 00749D98 CSWSEncounter vtable (56 slots, original order; verified against .rdata 00749D98..00749E74): the CSWSObject
// table with +00 deleting dtor, +5C AsSWSEncounter (self_return), +70 AIUpdate, +78 EventHandler, +8C GetFirstName.
const void* const kotor_vtable_00749d98[56] = {
    reinterpret_cast<const void*>(&kotor_fastcall_00593f90),                   // +00 00593F90 deleting dtor
    reinterpret_cast<const void*>(&slot_void_return_4_0060e760_a1),            // +04 0060E760 void_return
    reinterpret_cast<const void*>(&kotor_m15e_slot008_void8_005b5e90),         // +08 005B5E90 void_return
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +0C 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot00c_self_00641db0),          // +10 00641DB0 self_return (AsSWSObject)
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +14
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +18
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +1C
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +20
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +24
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +28
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +2C
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +30 AsSWSCreature
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +34
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +38
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +3C
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +40
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +44
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +48
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +4C
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +50
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +54
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +58
    reinterpret_cast<const void*>(&kotor_m15e_slot00c_self_00641db0),          // +5C 00641DB0 self_return (AsSWSEncounter)
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +60
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +64 (AsSWSStore: zero)
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +68
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +6C
    reinterpret_cast<const void*>(&kotor_fastcall_00593fb0),                   // +70 00593FB0 AIUpdate
    reinterpret_cast<const void*>(&kotor_fastcall_004cc390),                   // +74 004CC390 ClearAction
    reinterpret_cast<const void*>(&kotor_fastcall_00594220),                   // +78 00594220 EventHandler
    reinterpret_cast<const void*>(&kotor_fastcall_004ccf60),                   // +7C 004CCF60 SetAnimation
    reinterpret_cast<const void*>(&kotor_fastcall_004d0170),                   // +80 004D0170 GetDialogResref
    reinterpret_cast<const void*>(&kotor_m15e_slot0d4_true_0063e7a0),          // +84 0063E7A0 return_true
    reinterpret_cast<const void*>(&kotor_fastcall_004cb190),                   // +88 004CB190 GetGender
    reinterpret_cast<const void*>(&kotor_fastcall_00593510),                   // +8C 00593510 GetFirstName
    reinterpret_cast<const void*>(&kotor_fastcall_004d0190),                   // +90 004D0190 GetLastName
    reinterpret_cast<const void*>(&kotor_fastcall_004cb810),                   // +94 004CB810 GetDead
    reinterpret_cast<const void*>(&kotor_fastcall_004d01a0),                   // +98 004D01A0 GetMaxHitPoints
    reinterpret_cast<const void*>(&kotor_fastcall_004caec0),                   // +9C 004CAEC0 GetCurrentHitPoints
    reinterpret_cast<const void*>(&kotor_fastcall_004ccf80),                   // +A0 004CCF80 DoDamage
    reinterpret_cast<const void*>(&kotor_fastcall_004d09e0),                   // +A4 004D09E0 DoDamageReduction
    reinterpret_cast<const void*>(&kotor_fastcall_004d0e40),                   // +A8 004D0E40 DoDamageResistance
    reinterpret_cast<const void*>(&kotor_fastcall_004cf160),                   // +AC 004CF160 DoDamageImmunity
    reinterpret_cast<const void*>(&kotor_fastcall_004caee0),                   // +B0 004CAEE0 GetDamageImmunity
    reinterpret_cast<const void*>(&kotor_fastcall_004caf70),                   // +B4 004CAF70 GetDamageImmunityByFlags
    reinterpret_cast<const void*>(&c2da_resource_constant_word_00406490),      // +B8 00406490 return_minus_one
    reinterpret_cast<const void*>(&slot_void_return_4_0060e760_a1),            // +BC 0060E760 void_return
    reinterpret_cast<const void*>(&slot_void_return_4_0060e760_a1),            // +C0 0060E760 void_return
    reinterpret_cast<const void*>(&kotor_fastcall_004d01b0),                   // +C4 004D01B0 GetLockOrientationToObject
    reinterpret_cast<const void*>(&kotor_fastcall_004d01c0),                   // +C8 004D01C0 GetPortrait
    reinterpret_cast<const void*>(&recovered_object_portrait_ref_004d01f0),    // +CC 004D01F0 SetPortrait
    reinterpret_cast<const void*>(&recovered_creature_portrait_004d0210),      // +D0 004D0210 GetPortraitId
    reinterpret_cast<const void*>(&recovered_object_portrait_004cad20),        // +D4 004CAD20 SetPortraitId
    reinterpret_cast<const void*>(&kotor_m15e_slot090_void0_004015a0),         // +D8 004015A0 void_return
    reinterpret_cast<const void*>(&slot_void_return_4_0060e760_a1),            // +DC 0060E760 void_return
};
}
