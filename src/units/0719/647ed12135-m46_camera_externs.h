#pragma once
// lane-m46-3dcore fork D: declarations of callees that the M45 stage headers do not declare.
// Existing M45 providers are declared with their EXACT defined spelling (symbols-m45.json defined_by);
// names marked NEW have no provider yet (undefined externals of this fork, see parts/D.json).
// Pointer parameter types are opaque (void*) where the provider header lives outside this unit's roots;
// extern "C" __fastcall/__cdecl mangling depends only on the name and the stack byte count.
#include <cstdint>

namespace kotor_m46 {
using U = std::uint32_t;
using I = std::int32_t;
}

extern "C" {
// ---- existing M45 providers (exact spellings)
// texture-epoch-texture_x86: CExoArrayList<T*>::Remove 00445080, thiscall RET 4
void __fastcall texture_list_remove_00445080(void* list, void* unused_edx, void* value);
// 3d-model-model-material: SafePointer<CAurObject>::operator= 00447FA0, thiscall RET 4, EAX = this
void* __fastcall assign_00447fa0(void* safe_pointer, void* unused_edx, void* object);
// 3d-model-model-texture-lists: CExoArrayList<SafePointer<CAurTexture>>::allocate 0043DE00, thiscall RET 4
void __fastcall aurora_texture_ptr_list_allocate_0043de00(void* self, void* unused_edx, kotor_m46::I count);
// texture-epoch-controller_base: cosineLUT 004AADF0 / sineLUT 004AAE30 (cdecl, float in ST0)
float __cdecl cosine_lut_004aadf0(float);
float __cdecl sine_lut_004aae30(float);
// renderer-x87-ftol-alias: genuine __ftol2 006FAE8C (input ST0, result EDX:EAX); called only from inline asm
void __cdecl genuine_x87_ftol_006fae8c();

// ---- NEW (no provider in M45 or any lane source; exact ABI from the original)
// CAurBehaviorCameraFollow::CAurBehaviorCameraFollow 0049FE80: thiscall(this, gob, target) RET 8 (call site 0045C106)
void* __fastcall behavior_camera_follow_construct_0049fe80(void* self, void* unused_edx, void* gob, void* target);
// WindManager::pointSourceWindVector 004A5C80: thiscall(this, out, Vector point by value) RET 10h (call site 004A645F)
void* __fastcall wind_manager_point_source_wind_vector_004a5c80(void* wind, void* unused_edx, void* out,
                                                                float x, float y, float z);
// CExoArrayList<CAurTriangleListElem>::allocate 004A73F0 is reached only through 004A77C0 (not called here).
}
