#pragma once
#include <cstdint>
namespace kotor_renderer_init {
using I32 = std::int32_t;
// Borrowed canonical live words. This package does not allocate replacement globals.
struct AntiAliasingModeBindings {
    I32* current_007a6888;
    I32* previous_0078d440;
};
extern AntiAliasingModeBindings& aa_mode;
extern I32 __cdecl QueryAntiAliasingMode_0044f140(I32 value);
I32 __cdecl SetAntiAliasingMode_0044f2f0(I32 value);
}
