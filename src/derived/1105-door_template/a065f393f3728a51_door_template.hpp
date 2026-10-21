#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct DoorTemplatePorts{virtual ~DoorTemplatePorts()=default;virtual Word allocate(Word)=0;virtual Word construct_gff(Word,Word,Word,Word)=0;virtual void root(Word,Word)=0;virtual Word fields(Word,Word,Word,Word)=0;virtual void destroy(Word)=0;};
Word door_template_0058b3d0(Memory&,DoorTemplatePorts&,Word,Word,Word);
}
