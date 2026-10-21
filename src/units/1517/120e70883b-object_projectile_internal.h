#pragma once
#include "3625856b76-object_projectile.h"
namespace kotor_scene3d { struct Vector; }
#include "ead2dcbdd7-m15e.h"
namespace object_projectile {
extern const KotorM15ObjectMethods_007516c8 table_00757cc8;
}
extern "C" {
// Foundation lifecycle/table entries. Original stack cleanup is unchanged.
void* __fastcall object_projectile_006d5af0(void*,void*,std::uint32_t flags);
void __fastcall object_projectile_006d7bc0(void*,void*);
std::uint32_t __fastcall object_projectile_006d2c50(void*,void*,void* resref,std::uint32_t part,std::uint32_t type);
std::uint32_t __fastcall object_projectile_006d2db0(void*,void*,std::uint32_t part);
void __fastcall object_projectile_006d2d30(void*,void*);
void* __fastcall object_projectile_006d44b0(void* list_holder,void*);
void __fastcall object_projectile_006d5a40(void*,void*);
// Mode builders consume only ECX; all callers discard incidental EAX.
void __fastcall object_projectile_006d4520(void*,void*);
void __fastcall object_projectile_006d4550(void*,void*);
void __fastcall object_projectile_006d4640(void*,void*);
void __fastcall object_projectile_006d4730(void*,void*);
void __fastcall object_projectile_006d4930(void*,void*);
void __fastcall object_projectile_006d4a20(void*,void*);
void __fastcall object_projectile_006d4b10(void*,void*);
void __fastcall object_projectile_006d4c00(void*,void*);
void __fastcall object_projectile_006d4fe0(void*,void*);
void __fastcall object_projectile_006d5500(void*,void*);
void __fastcall object_projectile_006d55f0(void*,void*);
void __fastcall object_projectile_006d50f0(void*,void*);
void __fastcall object_projectile_006d5250(void*,void*);
void __fastcall object_projectile_006d53a0(void*,void*);
// Position/motion and update builders; no by-value FP argument transports.
void __fastcall object_projectile_006d33c0(void*,void*,kotor_scene3d::Vector* output);
void __fastcall object_projectile_006d7590(void*,void*);
void __fastcall object_projectile_006d2e00(void*,void*);
void __fastcall object_projectile_006d30c0(void*,void*);
void __fastcall object_projectile_006d64f0(void*,void*);
void __fastcall object_projectile_006d5b10(void*,void*,std::uint32_t variant);
void __fastcall object_projectile_006d6a90(void*,void*);
std::uint32_t __fastcall object_projectile_006d70d0(void*,void*);
}
