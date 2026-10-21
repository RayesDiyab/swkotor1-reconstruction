#pragma once
// CSWCameraNavigate table 007515F4 and the five R1 bodies.
// Source-only contract: see ../PLAN.md, ../STATE-ABI.md, ../PROVIDERS.md.
// The existing BehaviorBaseSlots type comes from the canonical construct3d.h; no new type is defined.
#include <cstdint>
#include "f58c269829-scene3d.h"
#include "289d120a9a-construct3d.h"

extern "C" {
// 006386B0 / 006386D0 / 006387E0 / 006386E0 / 006386A0 (D1 thiscall-as-fastcall veneers).
void* __fastcall kotor_fastcall_006386b0(void* self, void*, std::uint32_t flags);   // RET 4
void __fastcall kotor_fastcall_006386d0(void* self, void*);                          // RET 0
std::uint32_t __fastcall kotor_fastcall_006387e0(void* self, void*, std::uint32_t raw_delta); // RET 4
void __fastcall kotor_fastcall_006386e0(void* self, void*, char* line);              // RET 4
std::uint32_t __fastcall kotor_fastcall_006386a0(void* self, void*);                 // RET 0, 0x1071

// Selected cdecl providers (baseline 1568 map; declared only, never reimplemented here).
kotor_scene3d::Quaternion* __cdecl yaw_pitch_roll_004acac0(kotor_scene3d::Quaternion*, float, float, float);
kotor_scene3d::Quaternion* __cdecl quaternion_normalize_004a9d60(kotor_scene3d::Quaternion*,
                                                                 kotor_scene3d::Quaternion);
double __cdecl quaternion_yaw_004a9f40(const kotor_scene3d::Quaternion*);
double __cdecl quaternion_pitch_004aa020(const kotor_scene3d::Quaternion*);
double __cdecl kotor_cdecl_004acb90(kotor_scene3d::Quaternion);
std::int32_t __cdecl kotor_stricmp_0070acaf(const char*, const char*);
char* __cdecl firstword_00463530(char*);
void __cdecl parse_float_004635c0(char*, volatile float*);
}

// A owns the 008338E8 storage/producer; this TU imports the canonical volatile int32 symbol only.
namespace kotor_input_cells_required {
extern volatile std::int32_t camera_toggle_008338e8;
}

namespace w4cam {
// One external typed definition; the Navigate constructor imports this exact symbol.
extern const kotor_scene3d_construct::BehaviorBaseSlots camera_navigate_vtable_007515f4;
}
