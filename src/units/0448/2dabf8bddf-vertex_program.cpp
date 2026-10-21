#include "0ae0362481-vertex_program.h"
namespace kotor_vertex_program {
VertexProgram* __thiscall VertexProgram::Construct_004a27d0(char* source, std::uint32_t* parameters) {
    parameter_words = parameters;
    original_source = source;
    program_id = 0;
    allowed_lights = 0;
    generation = -1;
    FindNumberOfAllowedLights_004a2320();
    allowed_lights = 3;
    enabled = 0;
    AddVertexProgramToManager_004a2770(this);
    return this;
}
}
