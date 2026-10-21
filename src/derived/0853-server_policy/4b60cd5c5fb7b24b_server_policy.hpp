#pragma once
#include <cstdint>
namespace recovered_server_policy {
struct String {std::uint32_t data,length;};
struct Ports {virtual ~Ports()=default;virtual void ini_construct(void*)=0;virtual void ini_destroy(void*)=0;virtual void string_default(String*)=0;virtual void string_literal(String*,const char*)=0;virtual void string_destroy(String*)=0;virtual void string_assign(void*,const void*)=0;virtual void string_empty(void*)=0;};
unsigned net_server_005d4fb0(void*,String*,unsigned,Ports&);
void difficulty_0054ea70(void*,std::int32_t);
void initialize_004b1b00(void*,Ports&);
}
