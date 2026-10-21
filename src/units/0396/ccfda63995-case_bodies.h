#pragma once
#include "0d5f762f70-lowercase.h"
#include "57ef6354a2-allocator_live.h"
#include "fe8ea4e64c-strcpy.h"
// Authentic unbound lower edge, called with zero arguments and ignored return.
// No reset provider/storage or generic CRT replacement is defined here.
extern "C" void __cdecl c2da_case_stack_recover_00708b4f();
extern "C" __declspec(noinline) char* __cdecl c2da_current_strlwr_007135df(char*);
extern "C" __declspec(noinline) char* __cdecl c2da_current_strupr_007136f3(char*);
