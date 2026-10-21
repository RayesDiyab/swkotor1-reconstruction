#pragma once
#include <cstdint>
namespace recovered_server_create {
struct OuterPorts {
 virtual ~OuterPorts()=default;
 virtual void* allocate(std::uint32_t)=0;
 virtual void free(void*)=0;
 virtual const void* vtable()=0;
 virtual void* internal_construct_004b55e0(void*)=0;
 virtual void shutdown_005ed310(void*)=0;
 virtual void stop_004ae900(void*)=0;
 virtual void destroy_004aec70(void*)=0;
 virtual void cleanup_00401440()=0;
 virtual void set_options_004ae8f0(void*,std::uint32_t)=0;
 virtual std::uint32_t initialize_004ae850(void*)=0;
};
void* construct_outer_004aec00(void*,OuterPorts&);
std::uint32_t create_server_00401380(void*,std::uint32_t,OuterPorts&);
}
