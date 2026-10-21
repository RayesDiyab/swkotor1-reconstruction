#pragma once
#include <cstdint>
namespace kotor_w3 { struct Vector3; }
struct ObjectVfxPosition;
extern "C" {
unsigned char __fastcall kotor_fastcall_0063d560(void*,void*,void*);
unsigned char __fastcall kotor_fastcall_0060fd80(void*,void*,void*);
double __fastcall kotor_fastcall_0063d780(void*,void*);
double __fastcall kotor_fastcall_00616fe0(void*,void*);
std::uint32_t __fastcall object_vfx_0063d970(void*,void*,std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t,const ObjectVfxPosition*);
std::uint32_t __fastcall object_vfx_0063f400(void*,void*,std::uint32_t);
std::uint32_t __fastcall object_vfx_0063db20(void*,void*,std::uint32_t);
std::uint32_t __fastcall object_vfx_0063ddf0(void*,void*,std::uint32_t);
void __fastcall object_vfx_0063f490(void*,void*);
void __fastcall object_vfx_0063db90(void*,void*,std::uint32_t,std::uint32_t);
void __fastcall area_vfx_base_set_position_0063f340(void*,void*,const kotor_w3::Vector3*);
std::uint32_t __fastcall area_vfx_base_hilite_0063cb00(void*,void*,const kotor_w3::Vector3*,std::uint32_t,std::uint32_t,std::uint32_t);
std::uint32_t __fastcall area_vfx_base_unhilite_0063cb30(void*,void*,std::uint32_t);
}
