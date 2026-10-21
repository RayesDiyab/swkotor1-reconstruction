#pragma once
#include <cstdint>
#include "b8aebe2b27-camera_behavior_base.h"
namespace kotor_scene3d { struct Vector; }
namespace kotor_scene3d_construct { struct BehaviorBaseSlots; }
namespace camera_restore { extern const kotor_scene3d_construct::BehaviorBaseSlots camera_combat_vtable_00757bdc; }
extern "C" {
void* __fastcall kotor_fastcall_0063ad60(void*, void*, void* settings);
void* __fastcall kotor_fastcall_006d12b0(void*, void*, void* owner);
void __fastcall kotor_fastcall_006d22c0(void*, void*);
std::uint32_t __fastcall kotor_fastcall_006d14d0(void*, void*);
void* __fastcall kotor_fastcall_006d18c0(void*, void*, std::uint32_t flags);
std::uint32_t __fastcall kotor_fastcall_006d1fc0(void*, void*, std::uint32_t raw_delta);
kotor_scene3d::Vector* __fastcall kotor_fastcall_006d14e0(void*, void*, kotor_scene3d::Vector* out, void* character, void* target, float delta, float* yaw);
double __fastcall kotor_fastcall_006d16f0(void*, void*, const kotor_scene3d::Vector*, float yaw, float delta, std::uint32_t* flag);
kotor_scene3d::Vector* __fastcall kotor_fastcall_006d1760(void*, void*, kotor_scene3d::Vector* out, const kotor_scene3d::Vector* desired, float duration, float delta);
void __fastcall kotor_fastcall_006d1870(void*, void*, std::uint32_t raw_turn);
}
