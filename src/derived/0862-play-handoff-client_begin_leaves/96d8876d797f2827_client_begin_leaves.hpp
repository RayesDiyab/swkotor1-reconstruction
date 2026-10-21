#pragma once
#include <cstdint>
namespace recovered_client_begin {
struct String{std::uint32_t data,length;};
struct Ports{virtual ~Ports()=default;virtual void*pointer(void*,unsigned)=0;virtual void*manager()=0;virtual void copy_construct(String*,void*)=0;virtual void assign(void*,const String*)=0;virtual void*allocate(unsigned)=0;virtual void free(void*)=0;virtual void*network(void*)=0;virtual unsigned send(void*,unsigned,const void*,unsigned,unsigned)=0;};
String* name_005ed490(void*,String*,Ports&);void* message_005ed6f0(void*,Ports&);void temp_005edaa0(void*,const String*,Ports&);void screen_005edd30(void*,const String*,Ports&);unsigned begin_006775e0(Ports&);
}
