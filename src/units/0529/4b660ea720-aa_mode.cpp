#include "6cb781896f-aa_mode.h"
namespace kotor_renderer_init {
I32 __cdecl SetAntiAliasingMode_0044f2f0(I32 value) {
    if (QueryAntiAliasingMode_0044f140(value) == 0) return 0;
    *aa_mode.previous_0078d440 = *aa_mode.current_007a6888;
    *aa_mode.current_007a6888 = value;
    return 1;
}
}
