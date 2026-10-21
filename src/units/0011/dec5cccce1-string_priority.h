#pragma once
#include "9ac2fa9d10-startup_string.h"
// Native thiscall-compatible veneers: ECX receiver, ignored EDX; remaining
// words on stack. These bodies add no string layout or heap storage owner.
extern "C" kotor_reconstruction::StartupString* __fastcall GuiAssign005e5140(
    kotor_reconstruction::StartupString*,void*,const char*);
kotor_reconstruction::StartupString* __fastcall GuiConcat005e5d10(
    kotor_reconstruction::StartupString*,void*,kotor_reconstruction::StartupString*,
    const kotor_reconstruction::StartupString*);

// C++ linkage permits provider exceptions under fixed /EHsc; machine ABI is
// still ECX receiver, ignored EDX, two stack words and callee RET8.
