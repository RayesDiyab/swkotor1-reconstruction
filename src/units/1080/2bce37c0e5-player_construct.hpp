#pragma once
#include <cstdint>
namespace recovered_connection_player {
struct Ports {
 virtual ~Ports()=default;
 virtual std::uint32_t vtable(std::uint32_t)=0;
 virtual std::uint32_t invalid_object()=0;
 virtual void* allocate(std::uint32_t)=0;
 virtual void string_construct(void*)=0;
 virtual void string_assign_empty(void*)=0;
 virtual void ref_construct(void*)=0;
 virtual void ref_assign_empty(void*,void*)=0;
 virtual void* list_construct_005611f0(void*)=0;
 virtual void* last_update_00560b30(void*)=0;
 virtual void* update_wrapper_00560c20(void*)=0;
};
void* base_005cda10(void*,std::uint32_t,Ports&);
void* short_player_005802f0(void*,std::uint32_t,Ports&);
void* player_00562c70(void*,std::uint32_t,Ports&);
}
