#pragma once
#include <cstdint>

namespace kotor_reconstruction { struct StartupString; }
namespace kotor_scene3d { struct Vector; }

extern "C" {
std::uint8_t __fastcall kotor_fastcall_0060ff30(
    void* self, void* unused_edx, kotor_scene3d::Vector* output);
void __fastcall kotor_fastcall_0063d300(
    void* self, void* unused_edx, kotor_reconstruction::StartupString* output,
    std::uint32_t* tag);
std::uint32_t __fastcall kotor_fastcall_00685840(void* self, void* unused_edx);
std::uint32_t __fastcall kotor_fastcall_004efa80(void* self, void* unused_edx);
void __fastcall kotor_fastcall_0064af10(
    void* self, void* unused_edx, void* output_list, std::uint32_t kind,
    std::int32_t hit_points, std::uint32_t target_filter,
    std::uint32_t equipment_mask, float range);
void __fastcall kotor_fastcall_0064a870(
    void* self, void* unused_edx, void* output_list, std::uint32_t kind,
    std::int32_t hit_points, std::int32_t signed_word80,
    std::uint32_t target_filter, std::uint32_t equipment_mask, float range);
std::int32_t __fastcall kotor_fastcall_006467e0(
    void* self, void* unused_edx, std::int32_t index);
// All original result loads are FLD DWORD. The recovered float declaration
// is an inference from those loads; the caller must preserve live ST0 precision.
float __fastcall kotor_fastcall_00647dd0(
    void* self, void* unused_edx, const kotor_reconstruction::StartupString* tag);
}
