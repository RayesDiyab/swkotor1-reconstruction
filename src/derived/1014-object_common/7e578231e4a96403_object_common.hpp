#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct ObjectCommonPorts {virtual ~ObjectCommonPorts()=default;
 virtual Word list(Word,Word,Word,Word)=0; virtual Word count(Word,Word)=0;
 virtual void effects_nonempty(Word,Word,Word,Word)=0; virtual void actions_nonempty(Word,Word,Word,Word)=0;
 virtual void variables(Word,Word,Word)=0; virtual void builtins(Word,Word,Word)=0;
 virtual Word byte_read(Word,Word,Word,Word,Word)=0;
};
void object_effects_004d1be0(ObjectCommonPorts&,Word,Word,Word,Word);
void object_actions_004cecb0(ObjectCommonPorts&,Word,Word,Word,Word);
void object_common_004d1cf0(Memory&,ObjectCommonPorts&,Word,Word,Word,Word);
}
