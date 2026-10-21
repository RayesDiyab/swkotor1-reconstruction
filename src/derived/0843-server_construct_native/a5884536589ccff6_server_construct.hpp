#pragma once
#include <cstdint>
namespace recovered_server_create {
struct Providers {
 void (*string_construct)(void*);
 void (*string_assign)(void*, const char*);
 void (*embedded_construct_0052b170)(void*); // 0052B170
 void (*embedded_construct_00563d20)(void*); // 00563D20
 void* (*allocate)(std::uint32_t);
 void* (*list_construct)(void*); // 004B25D0
 void (*publish_internal)(void*); // original 00831FDC cell
 std::uint32_t invalid_object; // exact original 00746644
 std::uint32_t invalid_player; // exact original 0074C524
};
// Ordinary construction body. Exceptional unwinding is not yet admitted.
void* construct_internal_004b55e0(void*, const Providers&);
}
namespace recovered_server_create {
void* construct_list_internal_005e9aa0(void*);
void* construct_list_outer_004b25d0(void*, void* (*allocate)(std::uint32_t));
}
namespace recovered_server_create {
void* construct_embedded_0052b170(void*, void (*string_construct)(void*));
}
namespace recovered_server_create {
void* construct_embedded_00563d20(void*);
}
