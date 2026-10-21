#pragma once
#include <cstdint>
namespace recovered_net_connect {
struct String{std::uint32_t data,length;};
struct Ports{virtual ~Ports()=default;virtual unsigned length(const String*)=0;virtual const char*text(const String*)=0;virtual void*allocate(unsigned)=0;virtual void free(void*)=0;virtual void send(void*,unsigned,const void*,unsigned,unsigned)=0;virtual void assign(void*,const String*)=0;};
unsigned start_005d5040(void*,unsigned,const String*,const String*,unsigned,unsigned,Ports&);
}
