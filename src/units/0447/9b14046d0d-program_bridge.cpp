#include "00dc47def0-renderer_initialize.h"
#include "0ae0362481-vertex_program.h"
#include <new>
extern "C" void* __fastcall proposed_construct_program(void* p,void*,char* source,std::uint32_t* parameters){
    // Fresh successful 201C allocation is an admission requirement. Trivial
    // default initialization establishes lifetime and writes no old bytes.
    auto* object=::new(p) kotor_vertex_program::VertexProgram;
    return object->Construct_004a27d0(source,parameters);
}
extern "C" void __fastcall proposed_enable_program(void* p,void*,const void* input,std::uint8_t lighting){
    static_cast<kotor_vertex_program::VertexProgram*>(p)->Enable_004a24d0(input,lighting);
}
namespace kotor_renderer_init {
const ConstructProgram construct_program_004a27d0=&proposed_construct_program;
const EnableProgram enable_program_004a24d0=&proposed_enable_program;
}
