#pragma once
#include <cstdint>
namespace recovered_connection_player {
struct LowerPorts {
 virtual ~LowerPorts()=default;
 virtual void*allocate(std::uint32_t)=0;
 virtual void*list_internal(void*)=0;
 virtual void*combat_information(void*)=0;
 virtual std::uint32_t invalid_object()=0;
 virtual unsigned char skill_count()=0;
 virtual void*pointer(void*,unsigned)=0;
 virtual void*remove_head(void*)=0;
 virtual void free(void*)=0;
};
void*list_wrapper_005611f0(void*,LowerPorts&);
void*last_update_005caf10(void*,LowerPorts&);
void*last_wrapper_00560b30(void*,LowerPorts&);
void*equipment_005ca7f0(void*,LowerPorts&);
void*equipment_wrapper_00560c20(void*,LowerPorts&);
}
