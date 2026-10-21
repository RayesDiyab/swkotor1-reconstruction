#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct ModuleGffPorts {virtual ~ModuleGffPorts()=default;virtual void base_00406460(Word)=0;virtual Word vtable(Word)=0;virtual void write_byte(Word,Word)=0;};
Word gff_default_004105a0(Memory&,ModuleGffPorts&,Word);
Word module_gff_004c30c0(Memory&,ModuleGffPorts&,Word);
}
