#pragma once
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error Approved MSVC x86 compiler required
#endif
// Custom no-stack-argument ABI: consume ST(0), return signed64 bits in EDX:EAX.
// This declaration is an assembly boundary, not an ordinary C++ numeric call.
// Task3's existing custom boundary uses this cdecl spelling. With no stack
// arguments its RET/ESP contract is identical to the saved stdcall(0) label.
extern "C" unsigned __int64 __cdecl unresolved_ftol2_006fae8c();
