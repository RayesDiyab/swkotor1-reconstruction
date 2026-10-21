#pragma once
#include <cstdint>
#include <cstdarg>
namespace kotor_functional {
const char* __cdecl modern_ini_strrchr(const char*,int);
std::int32_t __cdecl modern_warning_vsnprintf(char*,std::uint32_t,const char*,va_list);
}
