#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct AreaEffectObjectPorts{virtual ~AreaEffectObjectPorts()=default;virtual Word list(Word,Word,Word,Word)=0;virtual Word count(Word,Word)=0;virtual Word nonempty_00505b4a(Word,Word,Word,Word,Word,Word)=0;};
Word area_effect_objects_00505af0(Memory&,AreaEffectObjectPorts&,Word area,Word gff,Word root,Word saved,Word templates,Word scratch);
}
