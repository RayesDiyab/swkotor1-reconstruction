#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct ModuleFinishPorts {virtual ~ModuleFinishPorts()=default;virtual Word list_004118c0(Word,Word,Word,Word)=0;virtual Word count_00411940(Word,Word)=0;virtual Word element_00411990(Word,Word,Word,Word)=0;virtual Word type_004111c0(Word,Word)=0;virtual Word word_00411c20(Word,Word,Word,Word,Word)=0;virtual Word allocate(Word)=0;virtual Word creature_004f7a10(Word,Word,Word)=0;virtual Word load_00500350(Word,Word,Word,Word)=0;virtual void destroy(Word,Word)=0;virtual Word float_bits_00411d00(Word,Word,Word,Word,Word)=0;virtual void position_004cd170(Word,Word)=0;virtual void attach_004c83b0(Word,Word)=0;};
Word module_finish_004c8c70(Memory&,ModuleFinishPorts&,Word,Word,Word,Word,Word);
}
