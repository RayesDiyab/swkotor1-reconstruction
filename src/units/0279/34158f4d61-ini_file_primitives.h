#pragma once
#include <cstdint>
#if !defined(_MSC_VER) || !defined(_M_IX86) || !defined(_MT) || defined(_DLL)
#error Modern INI FILE boundaries require one supported MSVC x86 static CRT (/MT).
#endif
namespace kotor_functional {
// Opaque values are FILE* from this same linked /MT CRT. They are never a
// historical IoStream, descriptor, HANDLE or game-heap allocation. Valid CRT
// inputs, sufficient buffers and one close per successful open are required.
void* __cdecl modern_ini_open(const char*,const char*);
char* __cdecl modern_ini_gets(char*,int,void*);
int __cdecl modern_ini_seek(void*,std::int32_t,int);
int __cdecl modern_ini_close(void*);
int __cdecl modern_ini_print(void*,const char*,...);
}
