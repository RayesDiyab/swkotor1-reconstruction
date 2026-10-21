#include "89d338e0f3-arb_error.h"

namespace kotor_renderer_init {
void __cdecl ReportARBError_0047a2c0(const char* current_program) {
    (void)current_program;
    // Original reserves 0x28 stack bytes. The GL query writes into that local
    // storage; no position or returned string escapes this call.
    std::int32_t position[10];
    const char* error = glGetString_0073d290(0x8874u);
    // The original unconditionally dereferences the GL-owned pointer.
    if (*error != 0) {
        glGetIntegerv_0073d244(0x864bu, position);
    }
}
}
