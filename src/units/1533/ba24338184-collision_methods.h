#pragma once
#include <cstdint>
namespace kotor_scene3d {struct Vector;}
extern "C" {
std::uint32_t __fastcall w3_area_collision_contact_004bba40(void*,void*,void*,const kotor_scene3d::Vector*,std::uint32_t,const kotor_scene3d::Vector*,float,std::uint32_t,std::int32_t,std::uint32_t*);
std::int32_t __fastcall w3_area_collision_notify_004bc0e0(void*,void*,const kotor_scene3d::Vector*,float,const kotor_scene3d::Vector*,const kotor_scene3d::Vector*,void*);
}
