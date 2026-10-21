#pragma once
#include "cbf96b7af2-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct LocalizedGetterPorts {virtual ~LocalizedGetterPorts()=default;virtual Word next(Word,Word)=0;virtual void assign(Word,Word)=0;virtual Word length(Word)=0;virtual void byte(Word,Word)=0;};
Word localized_get_index_005e9eb0(Memory&,LocalizedGetterPorts&,Word,Word,Word,Word,Word,Word);
Word localized_length_index_005e9f00(Memory&,LocalizedGetterPorts&,Word,Word,Word);
}
