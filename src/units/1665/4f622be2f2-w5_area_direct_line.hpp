#pragma once
// W5 G16: CSWSArea::TestDirectLine and its bound dependencies.
#include "58f963adf7-w5_types.hpp"

extern "C" {
using w5::U; using w5::I; using w5::Vector;
// 004BCB70 CSWSArea::TestDirectLine(from*, to*, radius*, heightBand, unused, CWalkHitInfo*): thiscall RET 18h.
// EAX: 1 reachable; 0 no start room / no adjacent room / adjacent room flagged (+38); -1 walkmesh or room
// non-walk polys; -2 static objects (NoNonWalkPolysDetailed); -3 no mover at area+1AC, radius <= 0, or creatures.
I __fastcall w5_area_test_direct_line_004bcb70(void* area, void*, const Vector* from, const Vector* to,
    const float* radius, float height_band, U unused5, void* walk_hit);
extern U w5_direct_line_debug_008327c4;
extern U w5_direct_line_heights_008327c8;

// existing native provider (world-los 0526): CSWSArea::NoNonWalkPolysDetailed, thiscall RET 30h
U __fastcall area_no_nonwalk_polys_detailed_00506650(void* area, void*, Vector* a, Vector* b, float f3, float f4,
    float f5, U object_id, U* out_id, I flag8, void* list, I flag10, void* walk_hit, I check_doors);
U __cdecl aurora_vector_equal_004aa980(const void* left, const void* right);
// existing native provider (world-los 0527): CSWCollisionMesh::NoNonWalkPolys, thiscall RET 18h
I __fastcall collision_mesh_no_nonwalk_polys_005972b0(void* mesh, void*, Vector* a, Vector* b, float f3, float f4,
    float f5, void* walk_hit);

// W5 support adapters over existing host-Memory bodies (no address credit):
//   w5_walkmesh_adapters.cpp (self-contained):
I __fastcall w5_adapt_room_no_nonwalk_polys_00579680(void* room, void*, Vector* a, Vector* b, const float* low,
    const float* high, const float* radius, void* walk_hit);              // 00579680 room_sweep, RET 18h
I __fastcall w5_adapt_room_adjacent_00579420(void* room, void*, U edge);   // 00579420 room_adjacent, RET 4
// 0886 TU bindings: 004BB600 GetRoom is W2's w2x_area_room_004bb600 (t2b ruling; its last argument is the
// int* out room index that the original pre-sets to -1 at 004BB651, typed int* in W2 t4); 004BC590 is W5's
// w5x binding (tiers/t2/source/w5_0886_exports.inc).
unsigned __fastcall w2x_area_room_004bb600(void* area, void*, void* position, int* out_room_index); // RET 8
I __fastcall w5x_area_no_creatures_on_line_004bc590(void* area, void*, Vector* a, Vector* b, void* creature,
    I p4, I p5, I p6, I p7, void* walk_hit);                                                         // RET 20h
}
namespace w5 {
inline void* area_get_room(void* area, Vector* point, U* out) {      // CSWSArea::GetRoom 004BB600 via W2's binding
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(
        w2x_area_room_004bb600(area, nullptr, point, reinterpret_cast<int*>(out))));
}
}
