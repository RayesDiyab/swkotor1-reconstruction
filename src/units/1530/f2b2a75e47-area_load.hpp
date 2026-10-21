#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct AreaLoadPorts {virtual ~AreaLoadPorts()=default;
virtual Word demand_00409b10(Word)=0;virtual void root_00411240(Word,Word)=0;
virtual void properties_00508c50(Word,Word)=0;virtual void tiles_005073d0(Word,Word)=0;
virtual void objects_0050dd80(Word,Word)=0;virtual void path_00508400(Word,Word)=0;
virtual void release_00409b80(Word)=0;virtual Word module_004ae6b0(Word)=0;
virtual void register_tag_004c7de0(Word,Word,Word)=0;
// Exact x87 signed product -> multiply float at73F400 -> CRT6FC880 -> FTOI6FAE8C remains explicit.
virtual Word numeric_tail(Word,Word)=0;virtual void byte(Word,Word)=0;
};
Word area_load_0050e190(Memory&,AreaLoadPorts&,Word,Word,Word);
}
