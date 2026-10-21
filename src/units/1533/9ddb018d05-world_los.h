#pragma once
// lane-world-los: shared world line-of-sight closure of CSWSArea::ClearLineOfSight 0050C330
// plus server accessors 004AE6B0 / 004AE750 / 004C30F0 (swkotor.exe GOG K1, image base 00400000).
//
// Conventions (DECISIONS.md D1 + D1-amendment, D21, D30-FINAL):
//  * original __thiscall entries are __fastcall(Self*, void* unused_edx, ...); callers pass nullptr
//    for EDX; callees never read EDX. No slot here returns a struct by value; none is variadic.
//  * virtual calls reload the receiver's vtable at each call through typed vtable views (D41 style);
//    no function-pointer casts.
//  * layouts are byte-offset views: every offset is cited from an observed instruction.
//  * x87: wide intermediates are double; float only at the original fstp dword stores (D30-FINAL).
//  * extern "C" names <snake>_<addr8 lowercase>; 004AE750 keeps the consumer-required spelling
//    kotor_fastcall_004ae750 (lane-r4-lowers r4_lowers.h:102).
#include <cstdint>
#include <cstring>
#include "36a720fede-scene3d.h"   // lane-3d-render canonical 3D header: Vector, Quaternion, PointerList, live_free,
                       // vector_cross_004ab210 / vector_magnitude_004ab100 / quaternion_rotate_004a9bb0 /
                       // quaternion_negate_w_004a9b50 / vector_sum_00423b90 (core-authored)

namespace kotor_world_los {
using U = std::uint32_t;
using I = std::int32_t;
using U8 = std::uint8_t;
using Vector = kotor_scene3d::Vector;
using Quaternion = kotor_scene3d::Quaternion;
using List = kotor_reconstruction::ArrayMutations;   // CExoArrayList {data,size,capacity}

template<class T> inline T& view(void* object, U offset) {
    return *reinterpret_cast<T*>(static_cast<unsigned char*>(object) + offset);
}
template<class T> inline const T& cview(const void* object, U offset) {
    return *reinterpret_cast<const T*>(static_cast<const unsigned char*>(object) + offset);
}
inline void* offset_ptr(void* object, U offset) { return static_cast<unsigned char*>(object) + offset; }
inline float bits_to_float(U bits) { float f; std::memcpy(&f, &bits, 4); return f; }
inline U float_to_bits(float f) { U u; std::memcpy(&u, &f, 4); return u; }
// Raw 12-byte vector copy as the original's three DWORD moves (no FPU canonicalisation).
inline void copy_vector(void* dst, const void* src) { std::memcpy(dst, src, 12); }

// .rdata constants (values read from swkotor.exe; cited at each use)
constexpr U8 k_object_found_0074666c = 0x00;   // CGameObjectArray::GetGameObject success code
constexpr U8 k_type_area_00746633 = 0x04;     // object type byte [obj+8] compared at 004B1832
// (.rdata 00746635 = 06 is compared at 004B1771, outside this closure)
constexpr float k_zero_0073d700 = 0.0f;         // .rdata 0073D700 = 00000000
constexpr float k_one_0073d764 = 1.0f;          // .rdata 0073D764 = 3F800000
constexpr double k_epsilon_007440f0 = 1.0e-9;   // .rdata 007440F0 = 3E112E0BE826D695 (qword)
constexpr U k_invalid_object_7f000000 = 0x7F000000u; // OBJECT_INVALID literal (0050C37C, 004B153D, 004C3138)

// ------------------------------------------------------------------ typed vtable views
// CGameObject-family vtable: only slots observed in this closure are typed; the rest are opaque words.
struct GameObject;
struct GameObjectVtbl {
    void* slot_00[4];
    I (__fastcall* slot_10)(GameObject* self, void* unused_edx);                  // 0050C3E9 / 0050C43F: nonzero -> exclusion list
    void* slot_14;
    void* (__fastcall* as_door_18)(GameObject* self, void* unused_edx);           // 004BBE4B (type 10 -> CSWSDoor*), 00506714
    void* slot_1c;
    void* (__fastcall* as_module_20)(GameObject* self, void* unused_edx);         // 004B1531
    void* slot_24;
    void* (__fastcall* as_area_28)(GameObject* self, void* unused_edx);           // 004B1842
    void* slot_2c[7];
    void* (__fastcall* as_placeable_48)(GameObject* self, void* unused_edx);      // 004BBE96 (type 9 -> CSWSPlaceable*)
};
static_assert(offsetof(GameObjectVtbl, slot_10) == 0x10 && offsetof(GameObjectVtbl, as_door_18) == 0x18 &&
    offsetof(GameObjectVtbl, as_module_20) == 0x20 && offsetof(GameObjectVtbl, as_area_28) == 0x28 &&
    offsetof(GameObjectVtbl, as_placeable_48) == 0x48, "observed slots");
struct GameObject {
    GameObjectVtbl* vtbl;   // [obj+0]
    // [obj+8] byte: object type (004BBE38 movzx; 004B182F)
};
inline U8 object_type(GameObject* object) { return view<U8>(object, 8); }

// CSWSArea vtable: slots +4 and +18 are called by NoNonWalkPolysDetailed (0050683D / 005068BF).
struct Area;
struct AreaVtbl {
    void* slot_00;
    I (__fastcall* slot_04)(Area* self, void* unused_edx, U object_id, List* list);         // 0050683D, RET 8 inferred from 2 pushes
    void* slot_08[4];
    // 005068BF: 10 pushes in order (a, b, f3, f4, f5, id, out_id, flag8, list, 1) - the caller's own arguments 1..9
    // forwarded unchanged plus literal 1 (Ghidra's 0050CA10 typing lists param 8/9 the other way round; unverified
    // which function the slot holds - observed caller order is used).
    I (__fastcall* no_nonwalk_polys_in_doors_18)(Area* self, void* unused_edx, Vector* a, Vector* b,
        float f3, float f4, float f5, U object_id, U* out_id, I flag8, List* list, I one);
};
static_assert(offsetof(AreaVtbl, slot_04) == 4 && offsetof(AreaVtbl, no_nonwalk_polys_in_doors_18) == 0x18, "area slots");
struct Area { AreaVtbl* vtbl; };

// ------------------------------------------------------------------ hit-info records
// CWalkHitInfo (004BB570 Initialize): +0 int, +4 Vector, +10 Vector, +1C Vector, +28 Vector, +34 Vector, +40 id, +44 int.
// CSWRoomSurfaceMeshHitInfo (00581320 ctor / 00580780 ClearAll): 0x20 bytes:
//   +0 mask, +4, +8 hit flag, +C face (-1), +10 Vector, +1C word.
struct SurfaceHitInfo { U mask_0; U field_4; U hit_8; I face_c; Vector point_10; U bit_1c; };
static_assert(sizeof(SurfaceHitInfo) == 0x20, "0x20 bytes (005797A0 sub esp,20h holds it)");

// ------------------------------------------------------------------ cells
// 007A693C GLRender drawable-square list (CExoArrayList<ulong>, .data zero-initialised; lane-world-los storage).
extern List drawable_squares_007a693c;
// 007A19B0 .data Vector (0,0,1) pushed by 005972B0 (005989D3, 00598ABE, 00598BA6, 005991CE, 00599275).
extern Vector up_007a19b0;
}

// M41 cell owner (window-shared-cells.obj)
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }

// Other-lane owners (bound, not authored here)
namespace kotor_renderer_gl {   // lane-renderer-bodies render_pool.h (D32)
void __fastcall allocate_00437d60(kotor_reconstruction::ArrayMutations* self, void* unused_edx, std::uint32_t capacity);
}
extern "C" {
// lane-r4-lowers r4_lowers.h:67 (CGameObjectArray::GetGameObject, RET 8, AL status)
unsigned char __fastcall kotor_fastcall_004d8230(void* array, void*, std::uint32_t id, void** out);
// lane-3d-model model_load.hpp:156-158 (GLRender vertex-pool cdecl wrappers)
std::uint32_t __cdecl aurora_pool_request_00423ee0(std::uint32_t a1, std::uint32_t a2, std::uint32_t a3, std::uint32_t unused4, std::uint32_t* out);
void* __cdecl aurora_pool_lock_00423f10(std::uint32_t handle, std::uint32_t unused2, std::uint32_t unused3, std::uint32_t a4);
std::uint32_t __cdecl aurora_pool_unlock_00423f30(std::uint32_t handle);
// lane-texture-epoch texture_x86.cpp:49 (CExoArrayList::Remove 00445080, RET 4)
void __fastcall texture_list_remove_00445080(kotor_reconstruction::ArrayMutations*, void*, std::uint32_t);
// lane-gui-draw (task-17 vector_equal.cpp reused unchanged): Vector operator== 004AA980, EAX 0/1
std::uint32_t __cdecl aurora_vector_equal_004aa980(const void* left, const void* right);
}
// M41 owner of CExoArrayList::Allocate 005E03C0: menu_list_growth::allocate_005e03c0 (growth.h via scene3d.h)

// ------------------------------------------------------------------ authored in this lane
extern "C" {
using kotor_world_los::U; using kotor_world_los::I; using kotor_world_los::Vector;
using kotor_world_los::Quaternion; using kotor_world_los::List; using kotor_world_los::SurfaceHitInfo;

// server accessors (server_accessors.cpp)
void* __fastcall server_get_module_004ae6b0(void* server, void*);                                   // RET 0
void* __fastcall kotor_fastcall_004ae750(void* server, void*, U object_id);                         // RET 4
void* __fastcall kotor_fastcall_004ae780(void* server, void*, U object_id);           // RET 4
void* __fastcall server_internal_get_module_004b14f0(void* internal, void*);                        // RET 0
void* __fastcall server_internal_get_game_object_004b1700(void* internal, void*, U object_id);      // RET 4
void* __fastcall server_internal_get_area_by_object_id_004b17e0(void* internal, void*, U object_id);// RET 4
void* __fastcall module_get_area_004c30f0(void* module, void*);                                     // RET 0

// area / room / surface mesh (area_los.cpp)
I __fastcall area_clear_line_of_sight_0050c330(void* area, void*, Vector* from, Vector* to, Vector* hit_out,
    U* blocker_out, U source_id, U target_id, I param7, I param8);                                   // RET 20h
U __fastcall area_no_nonwalk_polys_detailed_00506650(void* area, void*, Vector* a, Vector* b, float f3, float f4,
    float f5, U object_id, U* out_id, I flag8, List* list, I flag10, void* walk_hit, I check_doors); // RET 30h
I __stdcall area_no_nonwalk_polys_in_static_object_004bbe20(U object_id, Vector* a, Vector* b, float f4, float f5,
    float f6, U unused7, I unused8, void* walk_hit);                                                 // RET 24h, ECX ignored
I __fastcall placeable_no_nonwalk_polys_00585350(void* placeable, void*, Vector* a, Vector* b, float* f3,
    float* f4, float* f5, void* walk_hit);                                                           // RET 18h (tail jmp 005972B0)
I __fastcall door_no_nonwalk_polys_in_door_005890f0(void* door, void*, Vector* a, Vector* b, float* f3,
    float* f4, float* f5, void* walk_hit);                                                           // RET 18h (tail jmp 005972B0)
void __fastcall walk_hit_info_initialize_004bb570(void* info, void*, I p1, const Vector* v2, const Vector* v3,
    const Vector* v4, const Vector* v5, U p6, const Vector* v7);                                     // RET 1Ch
I __fastcall room_clear_line_of_sight_005797a0(void* room, void*, Vector from, Vector to, Vector* hit_out, U* mask); // RET 20h
void __fastcall surface_hit_info_clear_all_00580780(SurfaceHitInfo* info, void*, U mask);             // RET 4
SurfaceHitInfo* __fastcall surface_hit_info_construct_00581320(SurfaceHitInfo* info, void*, U mask);  // RET 4
I __fastcall surface_mesh_check_aabb_node_00580920(void* mesh, void*, I node, Vector* box_min, Vector* box_max,
    SurfaceHitInfo* hit);                                                                            // RET 10h
U __fastcall surface_mesh_check_aabb_line_of_sight_005814d0(void* mesh, void*, Vector from, Vector to,
    SurfaceHitInfo* hit);                                                                            // RET 1Ch
// compiler `eh vector constructor iterator` 00401130 and Vector::Vector 00409E70
void __stdcall vector_constructor_iterator_00401130(void* base, U element_size, U count,
    void* (__fastcall* construct)(void*, void*));                                                    // RET 10h
void* __fastcall vector_default_construct_00409e70(void* vector, void*);                            // RET 0, EAX=this

// drawable squares (squares.cpp)
void __cdecl gl_render_add_square_00435b40(U handle);
void __cdecl gl_render_remove_square_00435b90(U handle);
U __cdecl aur_add_drawable_square_0044efb0(const float* corners12, U color);
void __cdecl aur_remove_drawable_square_0044f050(U handle);

}
#include "3ce5de6a0a-world_los_geometry.h"   // fork-owned: geometry.cpp + geometry_core_handover.cpp
#include "83761db7c7-world_los_mesh.h"       // fork-owned: collision_mesh.cpp (+ squares used by it)
