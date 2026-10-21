#pragma once
#include <cstdint>
namespace kotor_scene3d {struct Vector;}
namespace kotor_reconstruction {struct StartupString;}
extern "C" {
std::uint32_t __fastcall object_projectile_0060d730(void*,void*,std::uint32_t,
 void* attack,const kotor_scene3d::Vector*,const kotor_reconstruction::StartupString*);
std::uint32_t __fastcall object_projectile_0060d070(void*,void*,std::uint32_t hand);
}
