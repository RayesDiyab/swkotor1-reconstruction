#pragma once
#include "36a720fede-scene3d.h"
#include <cstdint>
extern "C" {
void __fastcall w3_area_door_position_update_00508180(void*,void*,
    const kotor_scene3d::Vector*,const kotor_scene3d::Vector*,const kotor_scene3d::Vector*,
    std::uint32_t,std::int32_t);
kotor_scene3d::Vector* __fastcall w3_placeable_nearest_action_point_00584c20(void*,void*,
    kotor_scene3d::Vector*,const kotor_scene3d::Vector*);
}
