#pragma once
#include <cstdint>
namespace recovered_loading_lookup {
struct String{std::uint32_t data,length;};struct Ref{char data[16];};
struct Ports{virtual ~Ports()=default;virtual void*pointer(void*,unsigned)=0;virtual void*rules()=0;virtual void*manager()=0;virtual void*allocate(unsigned)=0;virtual unsigned length(const String*)=0;virtual void string_default(String*)=0;virtual void string_literal(String*,const char*)=0;virtual void string_assign(String*,const char*)=0;virtual void string_copy_construct(String*,String*)=0;virtual void string_destroy(String*)=0;virtual void format_load(String*,const String*)=0;virtual void ref_from_string(Ref*,String*)=0;virtual void ref_literal(Ref*,const char*)=0;virtual int resource_exists(Ref*,unsigned,unsigned*)=0;virtual int table_named_string(void*,const String*,String*,String*)=0;virtual void*table_construct(void*,Ref,unsigned)=0;virtual int table_load(void*)=0;virtual void table_unload(void*)=0;virtual void table_delete(void*,unsigned)=0;virtual int table_int(void*,unsigned,String*,unsigned*)=0;virtual unsigned server_module_query(void*,const String*)=0;};
String* name_005f3650(String*,const String*,Ports&);
unsigned hint_005f4760(void*,const String*,unsigned,Ports&);
}
