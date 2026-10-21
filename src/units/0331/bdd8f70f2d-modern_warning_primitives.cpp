#include "a399bd4f41-modern_warning_primitives.h"
#include <cstdio>
#include <cstring>
#if !defined(_MSC_VER) || !defined(_M_IX86) || !defined(_MT) || defined(_DLL)
#error One supported MSVC x86 static CRT is required.
#endif
namespace kotor_functional {
const char* __cdecl modern_ini_strrchr(const char* text,int value) {
    return std::strrchr(text,value);
}
std::int32_t __cdecl modern_warning_vsnprintf(char* output,std::uint32_t capacity,const char* format,va_list arguments) {
    // Deliberate legacy truncation contract (-1), not C99 required-size return.
    return ::_vsnprintf(output,capacity,format,arguments);
}
}
