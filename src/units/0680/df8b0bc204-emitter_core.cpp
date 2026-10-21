// lane-3d-emitter emitter_core.cpp: lane cells, scene/room/wind glue and PartEmitter lifetime slots.
// Evidence: dumpbin /disasm of swkotor.exe (GOG K1, image base 00400000); instruction addresses cited per behaviour.
// D1: thiscall bodies as __fastcall(self, unused_edx, ...). D30-FINAL: double for x87 intermediates, float at
// `fstp dword`. Unwind states (00717AE0 PartEmitter dtor) are cited, not emitted: the only throwing-declared callee
// reached is material_owner::Material::~Material (noexcept(false) declaration of a nonthrowing body, task-17).
#include "4ac9e30747-emitter.h"
#include "b22a9fca42-material.h"

// LOCAL-DECLS merged into emitter.h (lane header).

// ---------------------------------------------------------------- cells owned by this area (pinned image values)
extern "C" {
volatile kotor_scene3d::U emitter_bucket_enabled_0078e3d8 = 1;                 // .data 0078E3D8 = 1
volatile kotor_scene3d::U emitter_frame_threshold_00798b94 = 0x40A00000u;      // .data 00798B94 = 5.0f
volatile kotor_scene3d::U emitter_stencil_mode_00798b98 = 1;                   // .data 00798B98 = 1
volatile kotor_scene3d::I emitter_standard_pool_capacity_00798ba4 = 1000;      // .data 00798BA4 = 3E8h
volatile kotor_scene3d::I emitter_linked_pool_capacity_00798bac = 1000;        // .data 00798BAC = 3E8h
volatile kotor_scene3d::I particle_count_0083046c = 0;                         // BSS
volatile kotor_scene3d::I emitter_particles_drawn_00830474 = 0;                // BSS
volatile kotor_scene3d::U emitter_bss_00830488 = 0;                            // BSS
volatile kotor_scene3d::U emitter_bss_0083048c = 0;                            // BSS
volatile kotor_scene3d::U emitter_bss_00830490 = 0;                            // BSS
volatile kotor_scene3d::U emitter_last_spawn_00830494 = 0;                     // BSS
volatile kotor_scene3d::U emitter_debug_spheres_00830498 = 0;                  // BSS
volatile kotor_scene3d::U emitter_bss_0083049c = 0;                            // BSS
volatile kotor_scene3d::U emitter_cell_00798ba0 = 1;                           // .data 00798BA0 = 1
void* volatile emitter_standard_pool_008304a0 = nullptr;                       // BSS
volatile kotor_scene3d::U emitter_standard_pool_guard_008304a4 = 0;            // BSS
void* volatile emitter_linked_pool_008304b0 = nullptr;                         // BSS
volatile kotor_scene3d::U emitter_linked_pool_guard_008304b4 = 0;              // BSS
}

namespace {
using namespace kotor_emitter;
using kotor_reconstruction::ArrayMutations;
inline ArrayMutations* list_at(void* object, U offset) { return static_cast<ArrayMutations*>(at(object, offset)); }
inline void* element(const ArrayMutations* list, I index) {
    return as_object(*reinterpret_cast<const volatile U*>(static_cast<std::uintptr_t>(list->data) + static_cast<U>(index) * 4u));
}
inline void set_element(ArrayMutations* list, I index, U value) {
    *reinterpret_cast<volatile U*>(static_cast<std::uintptr_t>(list->data) + static_cast<U>(index) * 4u) = value;
}
// Inline CExoArrayList::Add with doubling growth (first allocation 8), as at 0046BDBD..0046BDE3 / 0049B2AB..0049B2D5.
inline void list_add(ArrayMutations* list, U value) {
    if (list->size == list->capacity) {
        const I capacity = list->capacity;
        menu_list_growth::allocate_005e03c0(list, nullptr, static_cast<U>(capacity != 0 ? capacity + capacity : 8));
    }
    set_element(list, list->size, value);
    list->size = list->size + 1;
}
// Inline "remove element at index (count already decremented) and shift down" used by the destructor
// (0049D010..0049D048): the count is re-read on every shift step.
inline void* take_reverse(ArrayMutations* list, I index) {
    void* const item = element(list, index);
    const I count = list->size - 1;
    list->size = count;
    for (I k = index; k < list->size; ++k) {
        const U next = *reinterpret_cast<const volatile U*>(static_cast<std::uintptr_t>(list->data) + static_cast<U>(k + 1) * 4u);
        set_element(list, k, next);
    }
    return item;
}
constexpr U kOneBits = 0x3F800000u;
constexpr U kTenThousandBits = 0x461C4000u;    // 10000.0f immediate (0048E1CA)
constexpr U kGravity_00743304 = 0xC11CF5C3u;   // -9.81f
constexpr U kHalf_0073e9ac = 0x3F000000u;      // 0.5f
constexpr U kHundredth_0073dfec = 0x3C23D70Au; // 0.01f
constexpr U kRadiusScale_00743300 = 0x3F933333u; // 1.15f
constexpr U kTenth = 0x3DCCCCCDu;              // 0.1f immediate (00451240 / 00451290)
}

extern "C" {

// ================================================================ scene / rooms / wind
// 004511F0 Scene::UpdateEmitters: active scene = this; every room's emitter list and the scene's own emitter
// list are advanced 100 times by 0.1 s (pre-warm).
void __fastcall scene_update_emitters_004511f0(void* scene, void*) {
    *kotor_scene3d::cells.active_scene_007b92a0 = scene;                               // 004511F8
    for (I i = 0; i < field<I>(scene, 0xCC); ++i) {                                     // 004511FE..00451211 / 0045125F..0045126C
        void* const room = as_object(*reinterpret_cast<const volatile U*>(field<U>(scene, 0xC8) + static_cast<U>(i) * 4u)); // 00451213..00451219
        void* const node = field<void*>(room, 0x58);                                    // 0045121C
        for (I j = 0; j < field<I>(node, 0x28); ++j) {                                  // 0045121F..00451226 / 0045124F..00451255
            void* const emitter = as_object(*reinterpret_cast<const volatile U*>(field<U>(node, 0x24) + static_cast<U>(j) * 4u)); // 00451230..00451233
            for (int k = 100; k != 0; --k)                                              // 00451236 / 0045124C..0045124D
                part_emitter_animate_particles_00494da0(emitter, nullptr, F(kTenth));  // 00451240..00451247
        }
    }
    for (I k = 0; k < field<I>(scene, 0x98); ++k) {                                     // 0045126E..00451278 / 0045129F..004512A8
        void* const emitter = as_object(*reinterpret_cast<const volatile U*>(field<U>(scene, 0x94) + static_cast<U>(k) * 4u)); // 00451280..00451286
        for (int n = 100; n != 0; --n)                                                  // 00451289 / 0045129C..0045129D
            part_emitter_animate_particles_00494da0(emitter, nullptr, F(kTenth));      // 00451290..00451297
    }
    *kotor_scene3d::cells.active_scene_007b92a0 = nullptr;                             // 004512AD / 004512BE
}

// 0046BDB0 nodedata::AddEmitter(emitter): CExoArrayList at +8C (data, size +90, capacity +94).
void __fastcall nodedata_add_emitter_0046bdb0(void* node, void*, void* emitter) {
    list_add(list_at(node, 0x8C), word_of(emitter));                                    // 0046BDB0..0046BDE3
}

// 0046CD70 AddEmitterToRooms(scene, emitter), cdecl.
void __cdecl add_emitter_to_rooms_0046cd70(void* scene, void* emitter) {
    void* const gob = field<void*>(emitter, 0x40);                                      // 0046CD76
    if (gob == nullptr) return;                                                         // 0046CD7B..0046CD7D
    if (aurora_vector_equal_004aa980(at(gob, 0x78), at(gob, 0x94)) != 0 &&              // 0046CD83..0046CD98
        field<I>(gob, 0x24) > 0) {                                                      // 0046CD9A..0046CDA0
        void* const first = as_object(*reinterpret_cast<const volatile U*>(field<U>(gob, 0x20))); // 0046CDA2..0046CDA5
        nodedata_add_emitter_0046bdb0(first, nullptr, emitter);                         // 0046CDA7..0046CDA8
        return;
    }
    const float* const position = static_cast<const float*>(at(field<void*>(emitter, 0x40), 0x78)); // 0046CDB1 / 0046CDBF
    I added = 0;                                                                        // 0046CDC4
    for (I i = 0; i < field<I>(scene, 0xCC); ++i) {                                     // 0046CDB9..0046CDC8 / 0046CE18..0046CE21
        void* const room = as_object(*reinterpret_cast<const volatile U*>(field<U>(scene, 0xC8) + static_cast<U>(i) * 4u)); // 0046CDD0..0046CDD8
        if (x87_less(W(position[0]), W(field<float>(room, 0x1C)))) continue;           // 0046CDD6..0046CDE3 (test ah,1)
        if (x87_less(W(position[1]), W(field<float>(room, 0x20)))) continue;           // 0046CDE5..0046CDF0
        if (!(W(position[0]) <= W(field<float>(room, 0x28)))) continue;                // 0046CDF2..0046CDFC (test ah,41h; jp)
        if (!(W(position[1]) <= W(field<float>(room, 0x2C)))) continue;                // 0046CDFE..0046CE09
        nodedata_add_emitter_0046bdb0(field<void*>(room, 0x58), nullptr, emitter);     // 0046CE0B..0046CE0F
        ++added;                                                                        // 0046CE14
    }
    if (added != 0) return;                                                             // 0046CE23..0046CE29
    for (I i = 0; i < field<I>(scene, 0xCC); ++i) {                                     // 0046CE2B..0046CE5B
        void* const room = as_object(*reinterpret_cast<const volatile U*>(field<U>(scene, 0xC8) + static_cast<U>(i) * 4u)); // 0046CE40..0046CE46
        nodedata_add_emitter_0046bdb0(field<void*>(room, 0x58), nullptr, emitter);     // 0046CE49..0046CE4D
    }
}

// 0048D780 MdlNodeEmitter::IsAffectedByWind: AL = bit 2 of [node+12C] (only AL is defined by the original).
bool __fastcall emitter_node_is_affected_by_wind_0048d780(void* node, void*) {
    return ((field<U>(node, 0x12C) >> 2) & 1u) != 0;                                  // 0048D780..0048D789 (and al,1)
}

// 004A6470 WindManager::RegisterWindyEmitter(emitter): tail jmp AddUnique on the list at +84C.
void __fastcall wind_manager_register_windy_emitter_004a6470(void* wind, void*, void* emitter) {
    exo_array_list_add_unique_0046bd10(static_cast<PointerList*>(at(wind, 0x84C)), nullptr, word_of(emitter)); // 004A6470..004A6476
}

// 004A6480 WindManager::DeregisterWindyEmitter(emitter): count matches in the list at +84C; when any,
// CExoArrayList::Remove 00445080 (RemoveReverse32).
void __fastcall wind_manager_deregister_windy_emitter_004a6480(void* wind, void*, void* emitter) {
    ArrayMutations* const list = list_at(wind, 0x84C);                                  // 004A6486
    I count = field<I>(wind, 0x850);                                                    // 004A6480
    I matches = 0;                                                                      // 004A648D
    if (count <= 0) return;                                                             // 004A648F..004A6491
    U cursor = list->data;                                                              // 004A6493
    do {
        if (*reinterpret_cast<const volatile U*>(static_cast<std::uintptr_t>(cursor)) == word_of(emitter)) ++matches; // 004A64A0..004A64A4
        cursor += 4u;                                                                   // 004A64A5
    } while (--count != 0);                                                             // 004A64A8..004A64A9
    if (matches != 0) list->RemoveReverse32(word_of(emitter));                          // 004A64AB..004A64B0
}

// ================================================================ MdlNodeEmitter / PartEmitter
// 00485590 MdlNodeEmitter::InternalControl(part, a1, a2, a3), RET 10h: emitter = part->vtbl[34]() (taken first),
// then MdlNode::InternalControl(part, a1, a2, a3), then PartEmitter::CalculateRadius(emitter).
void __fastcall node_emitter_internal_control_00485590(void* node, void*, void* part, float t0, float t1, float weight) {
    void* const emitter = as_object(part_table(part)->s034(part, nullptr));           // 00485596..0048559D / 004855A9
    node_internal_control_004854d0(node, nullptr, part, t0, t1, weight);          // 004855A0..004855B4 (stack dwords re-pushed)
    part_emitter_calculate_radius_0048e1b0(emitter, nullptr);                         // 004855B9..004855BB
}

// 0048DCE0 (PartEmitter vtbl+68): jmp 00484260 Part::Animate_derived.
U __fastcall part_animate_derived_0048dce0(void* self, void*, U a0) {
    return part_animate_derived_00484260(self, nullptr, a0);                          // 0048DCE0
}

// 0048E1B0 PartEmitter::CalculateRadius.
void __fastcall part_emitter_calculate_radius_0048e1b0(void* emitter, void*) {
    void* const gob = field<void*>(emitter, 0x40);                                      // 0048E1B6
    if (gob != nullptr) {                                                               // 0048E1B9..0048E1BB
        void* const holder = field<void*>(gob, 0x58);                                   // 0048E1BD
        if (holder != nullptr && (field<U8>(holder, 0x50) & 1u) != 0) {                 // 0048E1C0..0048E1C8
            field<U>(emitter, 0xD0) = kTenThousandBits;                                 // 0048E1CA (10000.0f)
            return;
        }
    }
    float world_position[3] = {0.0f, 0.0f, 0.0f};                                       // 0048E1E7..0048E1F7 ([esp+8..10])
    float world_orientation[4] = {F(kOneBits), 0.0f, 0.0f, 0.0f};                       // 0048E1FF..0048E217 ([esp+2C]=1.0, +30..+38=0)
    part_table(emitter)->s00c(emitter, nullptr, word_of(world_position), word_of(world_orientation)); // 0048E1D9..0048E21F GetWorld
    void* const target = field<void*>(emitter, 0x1E4);                                  // 0048E222
    if (target != nullptr) {                                                            // 0048E228..0048E22A
        float target_position[3];
        const float* const p = static_cast<const float*>(as_object(
            gob_table(target)->s064_gob_get_position(as_gob(target), nullptr, word_of(target_position)))); // 0048E22C..0048E233
        Vector delta;
        delta.x = S(W(p[0]) - W(world_position[0]));                                    // 0048E244..0048E24F
        delta.y = S(W(p[1]) - W(world_position[1]));                                    // 0048E23D..0048E253
        delta.z = S(W(p[2]) - W(world_position[2]));                                    // 0048E236..0048E257
        field<float>(emitter, 0xD0) = S(vector_magnitude_004ab100(&delta));             // 0048E25B..0048E263
        return;
    }
    double drop = W(field<float>(emitter, 0x7C)) * W(F(kGravity_00743304));             // 0048E26E..0048E271
    if (drop < 0.0) drop = -drop;                                                       // 0048E277..0048E284 (fcom 0; C0 only -> fchs)
    field<float>(emitter, 0xD4) = S(drop);                                              // 0048E28C
    double spread = 0.0;                                                                // 0048E294 fld [0073D700]
    void* const owner = field<void*>(emitter, 0x1C4);                                   // 0048E286
    if (owner != nullptr) {                                                             // 0048E292..0048E29A
        Vector wind;
        std::memcpy(&wind, at(owner, 0x18), sizeof(wind));                              // 0048E29C..0048E2B6 (DWORD copies)
        spread = vector_magnitude_004ab100(&wind);                                      // 0048E2BA
    }
    const double speed = W(field<float>(emitter, 0x78));
    double sum = W(field<float>(emitter, 0xA8)) + W(field<float>(emitter, 0x8C));       // 0048E2C2..0048E2C8
    sum = spread + sum;                                                                 // 0048E2CE faddp
    sum = sum + speed * W(field<float>(emitter, 0xD4)) * W(F(kHalf_0073e9ac));          // 0048E2D0..0048E2DF
    const float travel = S(sum * speed);                                                // 0048E2E1..0048E2E4 fst [esp+4]
    field<float>(emitter, 0xD0) = travel;                                               // 0048E2E8
    // x extent: max(+90, +94) (ties/unordered -> +94), then max with +E8 (ties/unordered -> +E8), * 0.5
    double size_x = (W(field<float>(emitter, 0x90)) > W(field<float>(emitter, 0x94)))   // 0048E2EE..0048E2FF
        ? W(field<float>(emitter, 0x90)) : W(field<float>(emitter, 0x94));
    if (!(size_x > W(field<float>(emitter, 0xE8)))) size_x = W(field<float>(emitter, 0xE8)); // 0048E30F..0048E31E
    size_x = size_x * W(F(kHalf_0073e9ac));                                             // 0048E324
    double size_y = (W(field<float>(emitter, 0x98)) > W(field<float>(emitter, 0x9C)))   // 0048E32A..0048E33B
        ? W(field<float>(emitter, 0x98)) : W(field<float>(emitter, 0x9C));
    if (!(size_y > W(field<float>(emitter, 0xEC)))) size_y = W(field<float>(emitter, 0xEC)); // 0048E34B..0048E35A
    size_y = size_y * W(F(kHalf_0073e9ac));                                             // 0048E360
    if (size_y <= 0.0) size_y = size_x;                                                 // 0048E366..0048E375 (ordered <= 0 -> fld st(0))
    const double extent_y = W(field<float>(emitter, 0xB0)) * W(F(kHundredth_0073dfec)); // 0048E377..0048E383
    const double extent_x = W(field<float>(emitter, 0xAC)) * W(F(kHundredth_0073dfec)); // 0048E38A..0048E394
    const double birth = std::sqrt(extent_x * extent_x + extent_y * extent_y);          // 0048E39A..0048E3A4
    const double size = std::sqrt(size_y * size_y + size_x * size_x);                   // 0048E3AA..0048E3B4
    field<float>(emitter, 0xD0) = S((size + birth + W(travel)) * W(F(kRadiusScale_00743300))); // 0048E3B6..0048E3C6
    const I count = field<I>(emitter, 0x1CC);                                           // 0048E37D (read before the store)
    if (count > 0) {                                                                    // 0048E392 / 0048E3CE
        for (I i = 0; ; ) {
            void* const particle = as_object(*reinterpret_cast<const volatile U*>(field<U>(emitter, 0x1C8) + static_cast<U>(i) * 4u)); // 0048E3D1..0048E3D7
            void* const chunky = as_object(particle_table(particle)->s04_as_chunky(particle, nullptr)); // 0048E3DA..0048E3DC
            if (chunky == nullptr) break;                                               // 0048E3DF..0048E3E1
            void* const chunk = field<void*>(chunky, 0x74);                             // 0048E3E3
            if (chunk != nullptr) {                                                     // 0048E3E6..0048E3E8
                Vector delta;
                delta.x = S(W(field<float>(chunk, 0x78)) - W(world_position[0]));      // 0048E3EA..0048E3F6
                delta.y = S(W(field<float>(chunk, 0x7C)) - W(world_position[1]));      // 0048E3FA..0048E401
                delta.z = S(W(field<float>(chunk, 0x80)) - W(world_position[2]));      // 0048E405..0048E40F
                double reach = vector_magnitude_004ab100(&delta) + W(field<float>(chunk, 0x138)); // 0048E413..0048E418
                if (reach < W(field<float>(emitter, 0xD0))) reach = W(field<float>(emitter, 0xD0)); // 0048E421..0048E430 (C0 only)
                field<float>(emitter, 0xD0) = S(reach);                                 // 0048E436
            }
            ++i;                                                                        // 0048E442
            if (!(i < field<I>(emitter, 0x1CC))) break;                                 // 0048E43C..0048E445
        }
    }
    emitter_last_spawn_00830494 = field<U>(emitter, 0x58);                              // 0048E448..0048E44C
}

// 0049B240 SetEmitterTarget (Gob::RegisterCallBack target for "_EmitterTarget"), cdecl(gob, event, part).
void __cdecl set_emitter_target_0049b240(void*, void*, aurora_model::Part* part_typed) {
    void* const emitter = part_typed;                                                   // 0049B246 ([esp+4Ch] = 3rd arg)
    void* reference = nullptr;
    for (I i = 0; i < field<I>(emitter, 0x34); ++i) {                                   // 0049B24A..0049B26A
        void* const child = as_object(*reinterpret_cast<const volatile U*>(field<U>(emitter, 0x30) + static_cast<U>(i) * 4u)); // 0049B253..0049B256
        if (part_table(child)->s038(child, nullptr) != 0) { reference = child; break; } // 0049B259..0049B262
    }
    if (reference == nullptr) {
        void* const gob = field<void*>(emitter, 0x40);                                  // 0049B26C
        gob_table(gob)->s02c_gob_deregister_callback(as_gob(gob), nullptr, field<U>(emitter, 0x1F0)); // 0049B26F..0049B278
        return;
    }
    void* const ref_part = as_object(part_table(reference)->s038(reference, nullptr));  // 0049B282..0049B286
    void* const target = field<void*>(ref_part, 0x4C);                                  // 0049B289
    U* const slot = &field<U>(emitter, 0x1E4);                                          // 0049B294
    if (*slot != 0) list_at(as_object(*slot), 4)->RemoveReverse32(word_of(slot));      // 0049B28C..0049B2A0 (00445080)
    *slot = word_of(target);                                                            // 0049B2A5..0049B2A7
    if (target != nullptr) list_add(list_at(target, 4), word_of(slot));                 // 0049B2A9..0049B2D5
    float world_position[3] = {0.0f, 0.0f, 0.0f};                                       // 0049B2E6..0049B2F6
    float world_orientation[4] = {F(kOneBits), 0.0f, 0.0f, 0.0f};                       // 0049B2FE..0049B316
    part_table(emitter)->s00c(emitter, nullptr, word_of(world_position), word_of(world_orientation)); // 0049B2D8..0049B31E
    void* const current = as_object(*slot);                                             // 0049B321
    if (current == nullptr) { field<U>(emitter, 0xD0) = 0; return; }                    // 0049B323..0049B36B
    float target_position[3];
    const float* const p = static_cast<const float*>(as_object(
        gob_table(current)->s064_gob_get_position(as_gob(current), nullptr, word_of(target_position)))); // 0049B327..0049B32E
    Vector delta;
    delta.x = S(W(p[0]) - W(world_position[0]));                                        // 0049B33F..0049B34A
    delta.y = S(W(p[1]) - W(world_position[1]));                                        // 0049B338..0049B34E
    delta.z = S(W(p[2]) - W(world_position[2]));                                        // 0049B331..0049B352
    field<float>(emitter, 0xD0) = S(vector_magnitude_004ab100(&delta));                 // 0049B356..0049B35E
}

// 0049CFA0 PartEmitter::~PartEmitter (body).
void __fastcall part_emitter_destruct_0049cfa0(void* self, void*) {
    field<const void*>(self, 0) = &aurora_vtable_part_emitter_00743340;                 // 0049CFC0
    void* const hit = field<void*>(self, 0x1E8);                                        // 0049CFC6
    if (hit != nullptr) {                                                               // 0049CFCC..0049CFD6
        U* const hit_ref = &field<U>(hit, 0x38);                                        // 0049CFDD
        if (*hit_ref != 0) list_at(as_object(*hit_ref), 4)->RemoveReverse32(word_of(hit_ref)); // 0049CFD8..0049CFE6 (00445080)
        if (field<void*>(hit, 0x8) != nullptr) live_free(field<void*>(hit, 0x8));     // 0049CFEB..0049CFF3
        live_free(hit);                                                                 // 0049CFFB..0049CFFC
    }
    for (I i = field<I>(self, 0x1CC) - 1; i >= 0; --i) {                                // 0049D004..0049D00B / 0049D054..0049D055
        void* const particle = take_reverse(list_at(self, 0x1C8), i);                   // 0049D010..0049D048
        if (particle != nullptr) particle_table(particle)->s00_deleting_destructor(particle, nullptr, 1); // 0049D04A..0049D052
    }
    for (I i = field<I>(self, 0x1D8) - 1; i >= 0; --i) {                                // 0049D057..0049D05E / 0049D0A4..0049D0A5
        void* const particle = take_reverse(list_at(self, 0x1D4), i);                   // 0049D060..0049D098
        if (particle != nullptr) particle_table(particle)->s00_deleting_destructor(particle, nullptr, 1); // 0049D09A..0049D0A2
    }
    if (field<void*>(self, 0x1C4) != nullptr)                                           // 0049D0A7..0049D0B5
        wind_manager_deregister_windy_emitter_004a6480(field<void*>(self, 0x1C4), nullptr, self); // 0049D0B7..0049D0B8
    if (field<void*>(self, 0x40) != nullptr) scene_remove_from_scene_manager_004554b0(self); // 0049D0BD..0049D0CA
    void* const material = field<void*>(self, 0x1E0);                                   // 0049D0CD
    if (material != nullptr) {                                                          // 0049D0D3..0049D0D5
        static_cast<material_owner::Material*>(material)->~Material();                 // 0049D0D7..0049D0D9 (0047B980)
        live_free(material);                                                            // 0049D0DE..0049D0DF
    }
    aurora_emitter_count_00830470 = aurora_emitter_count_00830470 - 1;                  // 0049D0E7..0049D0F4
    U* const target_ref = &field<U>(self, 0x1E4);                                       // 0049D0EE
    if (*target_ref != 0) list_at(as_object(*target_ref), 4)->RemoveReverse32(word_of(target_ref)); // 0049D0FA..0049D137 (inline)
    if (field<void*>(self, 0x1D4) != nullptr) live_free(field<void*>(self, 0x1D4));     // 0049D139..0049D149 (state 2)
    if (field<void*>(self, 0x1C8) != nullptr) live_free(field<void*>(self, 0x1C8));     // 0049D151..0049D161 (state 1)
    U* const wind_ref = &field<U>(self, 0x1C4);                                         // 0049D0AF (ebp)
    if (*wind_ref != 0) list_at(as_object(*wind_ref), 0)->RemoveReverse32(word_of(wind_ref)); // 0049D169..0049D1A5 (inline)
    part_destruct_00440140(self, nullptr);                                              // 0049D1A7..0049D1B1 Part::~Part
}

// 0049D780 PartEmitter::~PartEmitter (subtype entry): jmp 0049CFA0.
void __fastcall part_emitter_destruct_0049d780(void* self, void*) {
    part_emitter_destruct_0049cfa0(self, nullptr);                                      // 0049D780
}

// 0049D740 PartEmitter scalar deleting destructor (table 00743340 slot 0), RET 4, EAX = this.
U __fastcall part_emitter_deleting_destructor_0049d740(void* self, void*, U flags) {
    part_emitter_destruct_0049cfa0(self, nullptr);                                      // 0049D743
    if ((flags & 1u) != 0) live_free(self);                                             // 0049D748..0049D750
    return word_of(self);                                                               // 0049D758
}

// 0049D760 Fountain/Explosion/Single scalar deleting destructor (tables 00743478/00743500/00743588 slot 0).
U __fastcall part_emitter_deleting_destructor_0049d760(void* self, void*, U flags) {
    part_emitter_destruct_0049d780(self, nullptr);                                      // 0049D763
    if ((flags & 1u) != 0) live_free(self);                                             // 0049D768..0049D770
    return word_of(self);                                                               // 0049D778
}

} // extern "C"
