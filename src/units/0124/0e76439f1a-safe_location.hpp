#pragma once
#include "93d73ae698-w1_ports.hpp"
// Tier 7: walkability / safe-location queries reached by W5's movement and door code.
namespace w1::safe_location {
// CSWSArea::NoNonWalkPolysInDoors(a*, b*, zlo, zhi, radius, object id, out id*, list*, flag9, flag10), thiscall ret 0x28.
U no_nonwalk_polys_in_doors_0050ca10(U area, U a, U b, U zlo, U zhi, U radius, U object_id, U out_id, U list, U flag9, U flag10, Ports& p);
// CSWSArea::ComputeSafeLocationInDirection(start*, direction*, max distance, creature*, check doors, out*), ret 0x18.
// Frame 0xA4 (the original's 0x78 locals plus its argument slots, and +0x98 for the ComputeHeight ST0 result).
U compute_safe_location_in_direction_004c0fa0(U area, U start, U dir, U max_dist, U creature, U check_doors, U out, U frame, Ports& p);
// CSWSArea::PositionWalkable(Vector by value), ret 0xC. Frame 0x2C.
U position_walkable_00506400(U area, U x, U y, U z, U frame, Ports& p);
// CSWRoomSurfaceMesh::GetSurfaceMaterialWalkCheckOnly(Vector by value), ret 0xC. Frame 0x58.
U surface_material_walk_check_00581a10(U mesh, U x, U y, U z, U frame, Ports& p);
}
