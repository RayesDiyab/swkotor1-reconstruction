#pragma once
#include <cstddef>
#include <cstdint>
namespace kotor_vertex_program {
struct VertexProgram {
    std::uint32_t untouched_word0;
    char* original_source;                       // +0x0004
    char program_text[0x2000];                    // +0x0008; no constructor direct writes
    std::uint32_t* parameter_words;              // +0x2008
    std::uint32_t program_id;                    // +0x200c
    std::int32_t allowed_lights;                 // +0x2010
    std::int32_t generation;                     // +0x2014
    std::uint32_t enabled;                       // +0x2018
    // This explicit method preserves the original constructor's EAX=this result.
    // It has no C++ member initializers, vtable assignment or implicit clearing.
    VertexProgram* __thiscall Construct_004a27d0(char* source, std::uint32_t* parameters);
    // Assigned separate body. The first word is used as a pointer at+0x40;
    // the second is consumed as an unnormalized low byte in the original.
    void __thiscall Enable_004a24d0(const void* lighting_input, std::uint8_t lighting);
    // Unresolved real dependency declaration, not an authored body or stub.
    void __thiscall FindNumberOfAllowedLights_004a2320();
};
// Unresolved manager dependency. This module owns no array/global storage.
void __cdecl AddVertexProgramToManager_004a2770(VertexProgram* program);
static_assert(sizeof(void*) == 4 && sizeof(VertexProgram) == 0x201c, "observed x86 receiver extent");
static_assert(offsetof(VertexProgram, original_source) == 4 && offsetof(VertexProgram, program_text) == 8, "source/buffer offsets");
static_assert(offsetof(VertexProgram, parameter_words) == 0x2008 && offsetof(VertexProgram, program_id) == 0x200c, "parameter/program words");
static_assert(offsetof(VertexProgram, allowed_lights) == 0x2010 && offsetof(VertexProgram, generation) == 0x2014 && offsetof(VertexProgram, enabled) == 0x2018, "tail offsets");
}
