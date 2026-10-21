#pragma once
#include "84a5500766-startup_string.h"
#include <cstdint>
extern "C" {
std::uint32_t __fastcall async_string_length_005e5790(kotor_reconstruction::StartupString*,void*);
int __fastcall async_string_equal_005e5200(kotor_reconstruction::StartupString*,void*,kotor_reconstruction::StartupString*);
char* __fastcall async_string_cstr_005e5670(kotor_reconstruction::StartupString*,void*);
// Direct genuine Task19 body import; no new provider/table here.
std::int32_t __cdecl kotor_lower_006fb766(std::int32_t);
}
