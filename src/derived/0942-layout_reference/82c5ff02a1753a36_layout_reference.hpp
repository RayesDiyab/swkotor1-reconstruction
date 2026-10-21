#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct LayoutReferencePorts {
 virtual ~LayoutReferencePorts()=default;
 virtual Word same_004060b0(Word,Word)=0;
 virtual Word nonempty_00406060(Word,Word)=0;
 virtual void release_00409b00(Word)=0;
 virtual Word remove_00409cf0(Word,Word)=0;
 virtual void destroy_resource(Word,Word)=0;
 virtual Word empty_004060d0(Word,Word)=0;
 virtual void assign_literal_00406290(Word,Word,Word)=0;
 virtual Word lookup_004074d0(Word,Word,Word)=0;
 virtual Word allocate(Word)=0;
 virtual Word construct_005df3a0(Word)=0;
 virtual void insert_00407680(Word,Word,Word,Word)=0;
 virtual void demand_00408620(Word)=0;
 virtual void assign_00406120(Word,Word,Word)=0;
};
void layout_reference_005de5f0(Memory&,LayoutReferencePorts&,Word,Word,Word,Word);
}
