#pragma once
#include "6dab7adcb5-menu_cohort.h"
#include "ffee89c921-populate_imports.h"
#include "3df78882af-startup_string.h"
#include "56f214f303-menu_projection.h"

// Existing providers retain their selected public callable declarations.
extern "C" {
void* __fastcall kotor_m15a_object_get_server_object_0063d4b0(void*, void*) noexcept(false);
void* __fastcall recovered_object_area_004cb120(void*, void*);
void* __fastcall recovered_creature_inventory_004ef770(void*, void*, std::uint32_t);
void* __fastcall w2x_inventory_item_005560b0(void*, void*, std::uint32_t);
void* __fastcall kotor_m15closure_00646c90(void*, void*);
std::uint32_t __fastcall recovered_object_get_reputation_0057cb80(
    void*, void*, std::uint32_t, std::uint32_t);

void* __fastcall kotor_m15_client_get_item_by_id_005ed5a0(void*, void*, std::uint32_t);
std::uint32_t __fastcall kotor_fastcall_005ee0f0(void*, void*);

// Missing lower implementations are explicit imports, not substitutes.
// The menu continuation owner will compose their genuine providers separately.
void __fastcall kotor_fastcall_0060f330(void*, void*, std::uint32_t, void*);
void __fastcall kotor_fastcall_006193a0(void*, void*, void*, void*,
    std::uint32_t, std::uint32_t);
std::uint32_t __fastcall kotor_fastcall_0060fcc0(void*, void*, void*);
void* __fastcall kotor_fastcall_00682e00(void*, void*);
void __fastcall kotor_fastcall_006837d0(void*, void*, void*, std::uint32_t, void*);
void __fastcall kotor_fastcall_00684410(void*, void*, void*, std::uint32_t, void*);
void __fastcall kotor_fastcall_00691f00(void*, void*, void*, std::uint32_t, void*);
void __fastcall kotor_fastcall_006191f0(void*, void*, void*, void*);
void __fastcall kotor_fastcall_006198e0(void*, void*, void*, void*);
void __fastcall kotor_fastcall_00619950(void*, void*, void*, void*);
void __fastcall kotor_fastcall_00619b10(void*, void*, void*, std::uint32_t, void*);
}

extern "C" {
void __fastcall kotor_fastcall_00685af0(void*, void*, void*);
void* __fastcall kotor_fastcall_005edeb0(void*, void*, void*, std::uint32_t);
void* __fastcall kotor_fastcall_005eda40(void*, void*);
void __fastcall gui_m48_progress_set_max_00417f90(void*, void*, std::uint32_t);
unsigned __int64 __cdecl unresolved_ftol2_006fae8c();
char* __fastcall WindowCStr005e5670(kotor_reconstruction::StartupString*, void*);
}
namespace kotor_original { void __fastcall kotor_fastcall_00417fb0(void*, void*, std::int32_t); }
namespace kotor_reconstruction { void __cdecl StartupFormat005e5680(StartupString*, const char*, ...); }
