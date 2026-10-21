#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct LayoutResourcePorts {virtual ~LayoutResourcePorts()=default;virtual void base(Word)=0;virtual Word vtable()=0;};
Word layout_resource_005df3a0(Memory&,LayoutResourcePorts&,Word);
Word layout_resource_freed_00710e60(Memory&,Word);
Word layout_resource_serviced_005d4c60(Memory&,Word);
}
