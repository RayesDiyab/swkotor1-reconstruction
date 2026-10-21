#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct AreaCreaturePorts{virtual ~AreaCreaturePorts()=default;virtual Word list(Word,Word,Word,Word)=0;virtual Word count(Word,Word)=0;virtual Word element(Word,Word,Word,Word)=0;virtual Word type(Word,Word)=0;virtual Word allocate(Word)=0;virtual Word construct(Word,Word,Word)=0;virtual void ref_literal(Word,Word)=0;virtual void gff_ref(Word,Word,Word,Word,Word,Word)=0;virtual Word load_template(Word,Word,Word)=0;virtual Word gff_dword(Word,Word,Word,Word,Word)=0;virtual Word load_gff(Word,Word,Word,Word)=0;virtual void destroy(Word,Word)=0;virtual Word gff_float(Word,Word,Word,Word,Word)=0;virtual Word safe_position(Word,Word,Word,Word,Word,Word)=0;virtual void place(Word,Word,Word,Word,Word,Word,Word)=0;virtual void orient(Word,Word)=0;};
Word area_creatures_00504a70(Memory&,AreaCreaturePorts&,Word,Word,Word,Word,Word,Word);
}
