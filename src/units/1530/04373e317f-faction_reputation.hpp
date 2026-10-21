#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct ReputationPorts{virtual ~ReputationPorts()=default;virtual Word allocate(Word)=0;virtual void free(Word)=0;virtual Word table_int_00413510(Word,Word,Word,Word)=0;virtual Word count_00411940(Word,Word)=0;virtual void element_00411990(Word,Word,Word,Word)=0;virtual Word dword_00411c20(Word,Word,Word,Word,Word)=0;};
Word reputation_defaults_0052b9e0(Memory&,ReputationPorts&,Word,Word);
void reputation_load_0052bbe0(Memory&,ReputationPorts&,Word,Word,Word,Word);
}
