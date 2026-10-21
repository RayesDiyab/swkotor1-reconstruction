#pragma once
#include "4064827f19-door_contracts.h"
#include "4bfb9d8c74-door_area_extension.h"
#include "3c66185f25-w5_types.hpp"
extern "C" {
std::uint32_t __fastcall recovered_area_index_00506f20(void*,void*,std::uint32_t*,std::uint32_t);
void* __fastcall kotor_fastcall_004ae770(void*,void*,std::uint32_t);
std::uint32_t __fastcall w2x_area_safe_point_004be5e0(void*,void*,void*,std::uint32_t);
void* __fastcall w5_server_get_door_by_id_004ae7c0(void*,void*,std::uint32_t);
w5::Vector* __fastcall w5_door_get_nearest_action_point_00589240(void*,void*,w5::Vector*,const w5::Vector*,std::uint32_t);
void* __fastcall recovered_placeable_lookup_004ae7a0(void*,void*,std::uint32_t);
std::uint32_t __fastcall w1x_area_safe_location_004be860(void*,void*,std::uint32_t,std::uint32_t,
    std::uint32_t,std::uint32_t,std::uint32_t);
void __fastcall recovered_object_position_004cd1a0(void*,void*,void*,std::uint32_t,std::uint32_t,std::uint32_t);
kotor_scene3d::Vector* __fastcall collision_mesh_local_to_world_00596aa0(void*,void*,
    kotor_scene3d::Vector*,const kotor_scene3d::Vector*);
}
