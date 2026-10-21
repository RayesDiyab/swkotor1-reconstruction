#pragma once
// lane-world-los collision-mesh declarations (included by world_los.h only). Owner: fork MESH.
namespace kotor_world_los {
// 0083286C .data dword = 0: debug-draw switch read (== 1) by 005972B0 at 00597813, 00598658, 00598DD1,
// 005994C4, 00599898, 005999FD. No code in the image writes it (raw-byte search: only these six reads).
extern volatile U debug_walk_draw_0083286c;
// 007A2070 .data dword = 1: read once at 00597F96 (nonzero -> material mask from vtable slot +14h).
// No writer in the image.
extern volatile U use_walk_check_mask_007a2070;
// 007A2074..007A20BC .data: 19 drawable-square handles, each initialised FFFFFFFF; index = (addr-7A2074)/4.
// Each is read (cmp -1 -> AurRemoveDrawableSquare) and rewritten with AurAddDrawableSquare's EAX by 005972B0 only.
extern U debug_squares_007a2074[19];
}

extern "C" {
using kotor_world_los::U; using kotor_world_los::I; using kotor_world_los::Vector; using kotor_world_los::Quaternion;
// collision mesh (collision_mesh.cpp). All __thiscall in the original -> D1 __fastcall(mesh, unused EDX, ...).
// 00596A00 WorldToLocal(out, world): RET 8, EAX = out
Vector* __fastcall collision_mesh_world_to_local_00596a00(void* mesh, void*, Vector* out, const Vector* world);
// 00596AA0 LocalToWorld(out, local): RET 8, EAX = out
Vector* __fastcall collision_mesh_local_to_world_00596aa0(void* mesh, void*, Vector* out, const Vector* local);
// 00596B30 (Ghidra FUN_00596b30) rotate v by orientation +38 unless [mesh+4] != 0: RET 8, EAX = out
Vector* __fastcall collision_mesh_rotate_00596b30(void* mesh, void*, Vector* out, const Vector* v);
// 005972B0 CSWCollisionMesh::NoNonWalkPolys(a, b, f3 z-low, f4 z-high, f5 radius, CWalkHitInfo*): RET 18h,
// EAX 1 = clear, 0 = blocked
I __fastcall collision_mesh_no_nonwalk_polys_005972b0(void* mesh, void*, Vector* a, Vector* b, float f3, float f4,
    float f5, void* walk_hit);
}
