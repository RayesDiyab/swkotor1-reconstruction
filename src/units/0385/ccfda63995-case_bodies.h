#pragma once
#include "e3c315b03e-lowercase.h"
#include "53783c93b9-allocator_live.h"
#include "7057f8ca5e-strcpy.h"
// Authentic unbound lower edge, called with zero arguments and ignored return.
// No reset provider/storage or generic CRT replacement is defined here.
extern "C" void __cdecl c2da_case_stack_recover_00708b4f();
extern "C" __declspec(noinline) char* __cdecl c2da_current_strlwr_007135df(char*);
extern "C" __declspec(noinline) char* __cdecl c2da_current_strupr_007136f3(char*);
