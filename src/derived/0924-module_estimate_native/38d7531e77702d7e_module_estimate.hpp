#pragma once
#include <cstdint>
namespace recovered_module_estimate {
struct String{void*data;std::uint32_t length;};struct Array{void*data;std::uint32_t count,capacity;};
struct Ports{virtual ~Ports()=default;virtual void empty(String*)=0;virtual void literal(String*,const char*)=0;virtual void destroy(String*)=0;virtual void enumerate(Array*,String*,unsigned,unsigned,unsigned)=0;virtual String*element(Array*,unsigned)=0;virtual void destroy_array(Array*)=0;virtual void assign(String*,String*)=0;virtual unsigned length(String*)=0;virtual String*left(String*,String*,unsigned)=0;virtual String*right(String*,String*,unsigned)=0;virtual String*concat(String*,String*,String*)=0;virtual unsigned extension_type(String*)=0;virtual void file_construct(void**,String*,unsigned,String*)=0;virtual unsigned file_opened(void**)=0;virtual unsigned file_size(void**)=0;virtual void file_destroy(void**)=0;};
void estimate_004b5f90(void*,const String*,unsigned,Ports&);
}
