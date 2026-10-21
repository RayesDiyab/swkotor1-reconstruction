#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct AreaEncounterPorts{virtual ~AreaEncounterPorts()=default;virtual Word list(Word,Word,Word,Word)=0;virtual Word count(Word,Word)=0;virtual Word nonempty_005050ba(Word,Word,Word,Word,Word,Word)=0;};
Word area_encounters_00505060(Memory&,AreaEncounterPorts&,Word area,Word gff,Word root,Word saved,Word templates,Word scratch);
}
