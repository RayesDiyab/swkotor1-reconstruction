#include "713022adac-ini_c_primitives.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>
namespace kotor_functional {
int __cdecl modern_ini_sprintf(char* output,const char* format,...) {
    va_list arguments;
    va_start(arguments,format);
    const int result=std::vsprintf(output,format,arguments);
    va_end(arguments);
    return result;
}
char* __cdecl modern_ini_strchr(const char* text,int character) {
    return const_cast<char*>(std::strchr(text,character));
}
}
