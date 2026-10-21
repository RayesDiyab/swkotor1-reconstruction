#pragma once
#include <cstdint>
namespace kotor_scene3d {struct Vector;}
// Original005086D0: eight stack words, RET20h. One shared owner for Door/Trigger.
extern "C" std::uint32_t __fastcall recovered_area_region_005086d0(
    void*,void*,kotor_scene3d::Vector*,kotor_scene3d::Vector*,float,float,float,
    std::uint32_t,void*,std::int32_t);
