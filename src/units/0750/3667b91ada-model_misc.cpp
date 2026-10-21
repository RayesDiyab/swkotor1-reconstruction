// model_misc.cpp - lane-3d-model rev2: small helpers still open in this lane's closure: Gob::ProcessGob 00444840,
// PointSourceWind 004A58C0 / CExoArrayList<PointSourceWind>::Allocate 004A5F80 / ::Add 004A64C0,
// WindManager::AddPointSourceWind 004A6510, Quaternion operator+ 004A9D00, CExoArrayList<T*>::Count 004AF240.
// Spellings follow the sibling-lane declarations (anim3d.h, bsp.h). EH unwind state of 004A5F80 cited.
#include "a7e200e217-model_instance.hpp"

using namespace aurora_model;
using kotor_scene3d::Vector;
using kotor_scene3d::Quaternion;
using kotor_scene3d::PointerList;

namespace {
struct PointSourceWind { U w[6]; };                                                // 24 bytes (rep movsd, 6)
static_assert(sizeof(PointSourceWind) == 24, "PointSourceWind");
using ElementCtor = void* (__fastcall*)(void* self, void* unused_edx);
using ElementDtor = void (__fastcall*)(void* self, void* unused_edx);
}

extern "C" {
void __stdcall crt_eh_vector_constructor_006fb55f(void* first, U size, I count, ElementCtor construct, ElementDtor destroy); // model_texture_lists.cpp
void __stdcall c2da_destroy_array_006fa852(void* first, U size, I count, ElementDtor destroy);                           // strict-link
void __fastcall behavior_void_return_004015a0(void* self, void* unused_edx);       // lane-3d-construct (shared `ret` body 004015A0)
U8 __cdecl aurora_tree_has_single_texture_00443b30(Part* part);
U8 __cdecl aurora_tree_has_transparency_00443d70(Part* part);
U8 __cdecl aurora_tree_has_geometry_004440d0(Part* part);
U8 __cdecl aurora_tree_set_geometry_flags_004443e0(Part* part);
void __fastcall gob_process_00444840(void* gob, void* unused_edx);
void* __fastcall point_source_wind_construct_004a58c0(void* self, void* unused_edx);
void __fastcall point_source_wind_list_allocate_004a5f80(List* self, void* unused_edx, I count);
void __fastcall point_source_wind_list_add_004a64c0(List* self, void* unused_edx, PointSourceWind value);
void __fastcall wind_manager_add_point_source_wind_004a6510(void* wind, void* unused_edx, Vector position, U raw_a, U raw_b, U raw_c);
Quaternion* __cdecl quaternion_add_004a9d00(Quaternion* out, const Quaternion* a, const Quaternion* b);
I __fastcall exo_array_list_count_004af240(const PointerList* list, void* unused_edx, void* value);
}

// 00444840 Gob::ProcessGob: when the dirty byte +167 is set: single-texture test of the part tree (+6C) with
// +168 reset first (+164 = result; +168 cleared on failure; +164 cleared when +168 stays 0), +166 = TreeHasGeometry,
// TreeSetGeometryFlags, +17F = TreeHasTransparency, vtbl+188 (GetMinimumSphere), then +167 = 0.
void __fastcall gob_process_00444840(void* gob, void*) {
    if (get<U8>(gob, 0x167) == 0) return;                                           // 00444844..0044484E
    put<U>(gob, 0x168, 0);                                                          // 00444854
    const U8 single = aurora_tree_has_single_texture_00443b30(get<Part*>(gob, 0x6C)); // 00444850..0044485A
    put<U8>(gob, 0x164, single);                                                    // 00444864
    if (single == 0) put<U>(gob, 0x168, 0);                                         // 00444862..0044486C
    if (get<U>(gob, 0x168) == 0) put<U8>(gob, 0x164, 0);                            // 00444872..0044487A
    put<U8>(gob, 0x166, aurora_tree_has_geometry_004440d0(get<Part*>(gob, 0x6C)));  // 00444880..0044488D
    aurora_tree_set_geometry_flags_004443e0(get<Part*>(gob, 0x6C));                 // 00444889..00444893
    put<U8>(gob, 0x17F, aurora_tree_has_transparency_00443d70(get<Part*>(gob, 0x6C))); // 00444898..004448A8
    Gob* const g = static_cast<Gob*>(gob);
    get<const GobVTable*>(gob, 0)->s188_gob_get_minimum_sphere(g, nullptr);          // 004448A1..004448AE
    put<U8>(gob, 0x167, 0);                                                         // 004448B4
}

// 004A58C0 PointSourceWind::PointSourceWind: six dwords zero; EAX = this.
void* __fastcall point_source_wind_construct_004a58c0(void* self, void*) {
    PointSourceWind* const w = static_cast<PointSourceWind*>(self);
    for (U i = 0; i < 6; ++i) w->w[i] = 0;                                          // 004A58CA..004A58EE
    return self;
}

// 004A5F80 CExoArrayList<PointSourceWind>::Allocate(count), RET4: new[] (ctor 004A58C0, dtor 004015A0), copy the
// old elements (rep movsd), delete[] the old array (free 006FA390).
void __fastcall point_source_wind_list_allocate_004a5f80(List* self, void*, I count) {
    const U old = self->data;                                                       // 004A5F9E
    self->capacity = count;                                                         // 004A5FAC
    unsigned char* const block = static_cast<unsigned char*>(live_operator_new(static_cast<U>(count) * 24 + 4)); // 004A5FA0..004A5FAF
    unsigned char* first = nullptr;                                                 // 004A5FBB
    if (block != nullptr) {                                                         // 004A5FBD..004A5FC3
        put<I>(block, 0, count);                                                    // 004A5FD6 (array cookie)
        first = block + 4;                                                          // 004A5FD0
        crt_eh_vector_constructor_006fb55f(first, 24, count, &point_source_wind_construct_004a58c0, &behavior_void_return_004015a0); // 004A5FC5..004A5FD8
    }
    self->data = addr(first);                                                       // 004A5FEC
    for (I i = 0; i < self->size; ++i)                                              // 004A5FE0..004A6009
        ptr<PointSourceWind>(self->data)[i] = ptr<PointSourceWind>(old)[i];
    if (old != 0) {                                                                 // 004A600B..004A600D
        c2da_destroy_array_006fa852(ptr<void>(old), 24, get<I>(ptr<void>(old - 4), 0), &behavior_void_return_004015a0); // 004A600F..004A601E
        live_free(ptr<void>(old - 4));                                              // 004A6023..004A6024
    }
}

// 004A64C0 CExoArrayList<PointSourceWind>::Add(value), RET18h: growth cap ? 2*cap : 8 via 004A5F80.
void __fastcall point_source_wind_list_add_004a64c0(List* self, void*, PointSourceWind value) {
    if (self->size == self->capacity)                                               // 004A64C3..004A64CB
        point_source_wind_list_allocate_004a5f80(self, nullptr, self->capacity ? self->capacity * 2 : 8); // 004A64CD..004A64DD
    ptr<PointSourceWind>(self->data)[self->size] = value;                           // 004A64E2..004A64F6
    self->size = self->size + 1;                                                    // 004A64F8..004A64FE
}

// 004A6510 WindManager::AddPointSourceWind(position, a, b, c), RET18h: Add the six dwords to the list at +0C.
void __fastcall wind_manager_add_point_source_wind_004a6510(void* wind, void*, Vector position, U raw_a, U raw_b, U raw_c) {
    PointSourceWind w{};
    std::memcpy(&w.w[0], &position, 12);                                            // 004A6513..004A6526
    w.w[3] = raw_a; w.w[4] = raw_b; w.w[5] = raw_c;                                 // 004A652A..004A6548
    point_source_wind_list_add_004a64c0(reinterpret_cast<List*>(at(wind, 0x0C)), nullptr, w); // 004A6543..004A655A
}

// 004A9D00 operator+(out, a, b) for quaternions, cdecl -> out: componentwise a + b, rounded to float.
Quaternion* __cdecl quaternion_add_004a9d00(Quaternion* out, const Quaternion* a, const Quaternion* b) {
    const float s3 = static_cast<float>(static_cast<double>(get<float>(a, 0xC)) + get<float>(b, 0xC)); // 004A9D04..004A9D0B
    const float s2 = static_cast<float>(static_cast<double>(get<float>(a, 0x8)) + get<float>(b, 0x8)); // 004A9D0E..004A9D11
    const float s1 = static_cast<float>(static_cast<double>(get<float>(a, 0x4)) + get<float>(b, 0x4)); // 004A9D14..004A9D17
    const float s0 = static_cast<float>(static_cast<double>(get<float>(a, 0x0)) + get<float>(b, 0x0)); // 004A9D1A..004A9D20
    put<float>(out, 0x0, s0); put<float>(out, 0x4, s1);                             // 004A9D22..004A9D24
    put<float>(out, 0x8, s2); put<float>(out, 0xC, s3);                             // 004A9D27..004A9D2A
    return out;
}

// 004AF240 CExoArrayList<T*>::Count(value), RET4: number of elements equal to value.
I __fastcall exo_array_list_count_004af240(const PointerList* list, void*, void* value) {
    I n = 0;                                                                        // 004AF243
    for (I i = 0; i < list->size; ++i)                                              // 004AF240..004AF259
        if (ptr<U>(list->data)[i] == addr(value)) ++n;
    return n;
}
