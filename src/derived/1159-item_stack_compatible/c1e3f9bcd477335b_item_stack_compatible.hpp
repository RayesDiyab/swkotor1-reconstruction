#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct ItemStackCompatiblePorts{virtual ~ItemStackCompatiblePorts()=default;virtual Word string_different(Word,Word)=0;virtual Word localized_different(Word,Word)=0;virtual Word property(Word,Word,Word)=0;};
Word item_stack_property(Memory&,Word,Word,Word);
Word item_stack_compatible_00553cf0(Memory&,ItemStackCompatiblePorts&,Word,Word);
}
