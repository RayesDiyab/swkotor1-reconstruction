#pragma once
#include <cstdint>
namespace recovered_server_packets {
struct Ports {virtual ~Ports()=default;virtual void*pointer(void*,unsigned)=0;virtual void*connection(void*,std::uint32_t)=0;virtual void dispatch_00528380(void*,std::uint32_t,void*,std::uint32_t)=0;virtual void dispatch_00527b20(void*,std::uint32_t,void*,std::uint32_t)=0;};
unsigned dispatch_004b15d0(void*,std::uint32_t,void*,std::uint32_t,std::uint32_t,Ports&);
}
