#pragma once
#include "83a9f72867-w1_ports.hpp"
// W4 review list: 004FC4C0 CSWSCreature::GetNearestObjects and the leaves it owns.
namespace w1::nearest {
// CSWSCreature::ValidNearestObjectType(object*, int* hostile), ret 8.
U valid_nearest_object_type_004f2c30(U self, U object, U out, Ports& p);
// CExoArrayList<float>::Insert(value, index), ret 8.
void float_list_insert_004f78c0(U list, U value, U index, Ports& p);
// CSWSCreature::GetNearestObjects(float range_hostile, float range_other, list* cone, list* all), ret 0x10.
// `e` models the original ESP-relative frame (0xC0 bytes; +0xB0..+0xB9 holds the ST0 results).
void get_nearest_objects_004fc4c0(U self, U range_flagged, U range_other, U cone, U all, U e, Ports& p);
}
