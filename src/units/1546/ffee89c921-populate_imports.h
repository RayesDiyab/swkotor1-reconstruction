#pragma once
#include "723b658e51-resources.h"
#include "423117f9b6-m15e.h"
#include <cstdint>

namespace frame_log { struct String; }
namespace menu_continuation {
struct AppManager;
extern AppManager* app_manager_007a39fc;
}
extern "C" {
void* __fastcall kotor_fastcall_005ed8b0(void*, void*);
void* __fastcall kotor_fastcall_00635460(void*, void*);
void* __fastcall kotor_fastcall_005ed580(void*, void*, std::uint32_t);
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void*, void*);
std::uint32_t __fastcall recovered_creature_incapacitated_004ef890(void*, void*);
void __fastcall kotor_fastcall_0063c000(void*, void*);
void __fastcall kotor_fastcall_00686e20(void*, void*);
std::uint32_t __fastcall gui_load_resref_not_equal_00406dc0(
    const menu_resources::Ref*, void*, const menu_resources::Ref*);
void __fastcall gui_load_border_params_set_fill_00414c00(
    void*, void*, const menu_resources::Ref*, std::int32_t) noexcept(false);
void __fastcall frame_LogStringCtor_005e5a90(
    frame_log::String*, void*, const char*) noexcept(false);
void __fastcall frame_LogStringDtor_005e5c20(frame_log::String*, void*);
}
