#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct LoadbarPorts{virtual ~LoadbarPorts()=default;virtual Word overrun(Word,Word)=0;virtual Word underrun(Word,Word)=0;virtual Word read_word(Word,Word)=0;virtual void empty_literal(Word)=0;virtual void destroy(Word)=0;virtual void event(Word,Word,Word,Word)=0;virtual void begin(Word,Word)=0;};
Word client_loadbar_0064edd0(Memory&,LoadbarPorts&,Word,Word,Word);
Word message_read_word_004d6780(Memory&,Word,Word);
void begin_loading_004b13e0(Memory&,Word,Word);
}
