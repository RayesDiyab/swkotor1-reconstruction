#pragma once
#include <cstdint>
namespace kotor_reconstruction { struct StartupString; }
namespace menu_resources { struct Ref; }
extern "C" {
void __fastcall object_projectile_base_0063daa0(void*,void*);
void* __fastcall object_projectile_base_0063e760(void*,void*);
void __fastcall object_projectile_base_0063cfd0(void*,void*,void* resref);
void* __fastcall object_projectile_base_0063e770(void*,void*,void* out_resref,std::uint32_t variant);
void __fastcall object_projectile_base_0063d140(void*,void*,std::uint32_t id);
std::uint16_t __fastcall object_projectile_base_0063e790(void*,void*);
void __fastcall object_projectile_base_0063d0a0(void*,void*,const float* normal);
std::uint32_t __fastcall object_projectile_base_0063e7b0(void*,void*,std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t);
std::uint32_t __fastcall object_projectile_base_0060f560(void*,void*,std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t);
void __fastcall object_projectile_base_0063e7d0(void*,void*,const float* position,std::uint32_t,float);
void __fastcall object_projectile_base_0063e7e0(void*,void*,const float* orientation);
void __fastcall object_projectile_base_0063d3f0(void*,void*,std::uint32_t animation,std::uint32_t value);
void __fastcall object_projectile_bark_0062ab30(void*,void*,std::uint32_t,kotor_reconstruction::StartupString*,menu_resources::Ref*,std::uint32_t);
}
