#pragma once
#include "3f38d05b1d-menu_callbacks.h"
#include "3df78882af-startup_string.h"
#include "974daf4bf1-table_contract.h"
#include "99e00ff93c-m15b_stats.h"
#include "b0e1e980f2-m15_l0.h"
#include "03e67ad358-window_shared_cells.h"
#include <cstdint>

// Exact existing selected interfaces; no replacement bodies or generic ports.
extern "C" {
extern void* kotor_cell_rules_007a3a28;
void* __fastcall kotor_fastcall_005ed6f0(void*, void*);
void __fastcall kotor_fastcall_00610a10(void*, void*, std::uint32_t);
std::uint32_t __fastcall kotor_fastcall_00677a90(void*, void*, std::uint32_t, std::uint32_t);
void* __fastcall recovered_item_lookup_004ae760(void*, void*, std::uint32_t);
std::int32_t __fastcall c2da_current_get_int_00413660(c2da_current::Table*, void*,
    std::int32_t, c2da_current::String*, std::int32_t*);
void __fastcall kotor_fastcall_004d68a0(void*, void*, std::uint32_t, std::uint32_t, std::uint32_t);
void __fastcall kotor_fastcall_004d6340(void*, void*, std::uint32_t, std::uint32_t);
void __fastcall kotor_fastcall_004d62c0(void*, void*, std::uint32_t, std::uint32_t);
void __fastcall kotor_fastcall_004d68e0(void*, void*, std::uint32_t);
void __fastcall kotor_fastcall_004d6920(void*, void*, std::uint32_t, std::uint32_t, std::uint32_t);
std::uint32_t __fastcall kotor_fastcall_004d6440(void*, void*, void**, std::uint32_t*);
std::uint32_t __fastcall recovered_client_send_00677410(void*, void*, std::uint32_t,
    std::uint32_t, void*, std::uint32_t);
std::uint32_t __cdecl aurora_vector_equal_004aa980(const void*, const void*);
}
namespace kotor_rules {
void* __fastcall kotor_fastcall_00550c00(void*, void*, std::uint16_t);
}
