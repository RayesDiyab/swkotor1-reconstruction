#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct CopyPorts {virtual ~CopyPorts()=default;virtual Word cstr_005e5670(Word)=0;virtual Word copy_file(Word,Word,Word)=0;virtual Word attributes(Word)=0;virtual Word set_attributes(Word,Word)=0;};
Word copy_module_004b1680(CopyPorts&,Word,Word);
}
