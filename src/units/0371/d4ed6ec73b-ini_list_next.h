#pragma once
#include "40141ae87f-ini_cleanup.h"

namespace base_ini_reconstruction {
// Existing canonical List/Node/String types and root/Warning owners only.
// Parent supplies this exact literal identity in its one ignored data module.
extern const char diagnostic_position_0074e2a4[];

// Task5's owned005E9B90 projected onto actual canonical list/cursor types.
// ECX is ignored, EDX is unused, one physical stack word, original RET4.
__declspec(noinline) void* __fastcall next_005e9b90(List*, void*, Node** position);
}
