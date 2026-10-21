#include "dfcfb7fa81-validator_cells.h"
#include "ea46a70e8e-enable_bindings.h"

namespace kotor_vertex_program {
U8 __cdecl ValidateNonRadeon_0045f5e0() {
    using namespace kotor_renderer_validation_required;
    U32 value = cache_0078e508;
    if (value == 0xffffffffu) {
        const U32 required = mask_0078e4d0;
        const U32 available = capabilities_007bb85c;
        const U32 control = control_0078e4fc;
        const U32 included = (available & required) == required ? 1u : 0u;
        value = 1;
        if ((included & control) != 0 && (available & mask_0078e4f4) == 0)
            value = 0;
        cache_0078e508 = value;
    }
    // The machine contract is AL only; higher EAX bits are not a bool result.
    return value == 1 ? U8(1) : U8(0);
}
}
