#pragma once
#include "7102fe53f2-startup_string.h"
// Native x86 ABI veneers: ECX receiver, unused EDX, optional stack word.
// The actual owned object type is the unchanged shared StartupString.
extern char mapped_empty_008328a4[1];
extern "C" kotor_reconstruction::StartupString* __fastcall WindowCopy005e5b00(kotor_reconstruction::StartupString*,void*,const kotor_reconstruction::StartupString* volatile);
extern "C" char* __fastcall WindowCStr005e5670(kotor_reconstruction::StartupString*,void*);
