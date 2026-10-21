#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct ModuleHelperPorts { virtual ~ModuleHelperPorts()=default;virtual void assign_005e5c50(Word,Word)=0;virtual Word equals_005e5290(Word,Word)=0;virtual void write_byte(Word,Word)=0;virtual Word lower_005e6080(Word,Word)=0;virtual void destroy_005e5c20(Word)=0;virtual Word lookup_004076e0(Word,Word)=0;virtual Word resource_available_00406720(Word,Word)=0; };
void module_identifier_0054eaa0(Memory&,ModuleHelperPorts&,Word,Word);
void timer_scale_004adba0(Memory&,ModuleHelperPorts&,Word,Word);
void module_tag_004c3060(ModuleHelperPorts&,Word,Word,Word);
Word resource_type_00406720(Word);
Word resource_count_0040d1a0(Memory&,ModuleHelperPorts&,Word,Word);
Word resource_path_00407780(Memory&,ModuleHelperPorts&,Word,Word,Word);
}
