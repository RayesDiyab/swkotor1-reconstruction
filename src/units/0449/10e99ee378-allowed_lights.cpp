#include "0ae0362481-vertex_program.h"
namespace kotor_vertex_program {
void VertexProgram::FindNumberOfAllowedLights_004a2320() {
    const volatile unsigned char* const source = reinterpret_cast<unsigned char*>(original_source);
    std::uint32_t semicolons = 0, index = 0, length = 0;
    const volatile unsigned char* scan = source;
    while (*scan++) ++length;
    if (static_cast<std::int32_t>(length) > 0) {
        const volatile unsigned char* const rescan = reinterpret_cast<unsigned char*>(original_source);
        do {
            if (source[index] == 0x3b) ++semicolons;
            ++index;
            scan = rescan; length = 0;
            while (*scan++) ++length;
        } while (static_cast<std::int32_t>(index) < static_cast<std::int32_t>(length));
    }
    // Selected I386 MSVC conversion retains the wrapped two's-complement word.
    const std::int32_t numerator = static_cast<std::int32_t>(114u - semicolons);
    const std::int32_t quotient = numerator / 14;
    volatile std::int32_t* output = &allowed_lights;
    *output = quotient;
    if (quotient > 3) *output = 3;
    else *output = quotient;
}
}
