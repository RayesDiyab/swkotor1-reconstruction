#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct ItemStackMergePorts{virtual ~ItemStackMergePorts()=default;virtual Word compatible(Word,Word)=0;virtual Word base(Word,Word)=0;};
Word item_stack_merge_00553f90(Memory&,ItemStackMergePorts&,Word,Word);
}
