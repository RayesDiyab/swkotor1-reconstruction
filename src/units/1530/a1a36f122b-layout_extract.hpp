#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct LayoutExtractPorts {
 virtual ~LayoutExtractPorts()=default;
 virtual Word count(Word)=0;virtual Word allocate(Word)=0;
 virtual void construct(Word)=0;
 virtual Word position(Word,Word,Word)=0;virtual Word reference(Word,Word,Word)=0;
 virtual void assign(Word,Word,Word)=0;virtual void load(Word,Word)=0;virtual void bounds(Word)=0;
 virtual void overlap(Word,Word,Word)=0;virtual void connect(Word,Word,Word,Word)=0;virtual void finish(Word)=0;
};
Word layout_extract_00504870(Memory&,LayoutExtractPorts&,Word,Word);
Word layout_reference_at_005de470(Memory&,Word,Word,Word);
Word layout_position_at_005de4a0(Memory&,Word,Word,Word);
}
