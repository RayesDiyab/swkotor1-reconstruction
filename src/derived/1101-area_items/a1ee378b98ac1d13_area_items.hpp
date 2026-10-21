#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct AreaItemPorts {virtual ~AreaItemPorts()=default;
virtual Word list(Word,Word,Word)=0;virtual Word count(Word,Word)=0;virtual Word element(Word,Word,Word,Word)=0;virtual Word type(Word,Word)=0;
virtual Word dword(Word,Word,Word)=0;virtual Word allocate(Word)=0;virtual Word construct(Word,Word)=0;virtual void ref_empty(Word)=0;virtual void ref_read(Word,Word,Word,Word,Word)=0;virtual Word load_template(Word,Word)=0;virtual Word load_gff(Word,Word,Word)=0;virtual void destroy(Word)=0;virtual void common(Word,Word,Word)=0;virtual Word floating(Word,Word,Word,Word)=0;virtual void place(Word,Word,Word,Word,Word,Word)=0;virtual void orient(Word,Word)=0;};
Word area_items_00504de0(Memory&,AreaItemPorts&,Word area,Word gff,Word root,Word saved,Word templates,Word scratch);
}
