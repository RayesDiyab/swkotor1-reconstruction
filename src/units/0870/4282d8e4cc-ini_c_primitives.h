#pragma once
#if !defined(_MSC_VER) || !defined(_M_IX86) || !defined(_MT) || defined(_DLL)
#error Modern INI primitives require one supported MSVC x86 static CRT (/MT).
#endif
namespace kotor_functional {
// Explicit modern CRT boundary support, not reconstructed original CRT entries.
// INI's admitted domain retains valid terminated strings and sufficient output
// capacity. These are real CRT calls, not success providers or original credit.
int __cdecl modern_ini_sprintf(char*,const char*,...);
char* __cdecl modern_ini_strchr(const char*,int);
}
