#pragma once
#include <stdint.h>
using KotorMoveFn=void* (__cdecl *)(void*,const void*,uint32_t);
extern "C" void* __cdecl reconstructed_memmove(void* destination,const void* source,uint32_t count);
