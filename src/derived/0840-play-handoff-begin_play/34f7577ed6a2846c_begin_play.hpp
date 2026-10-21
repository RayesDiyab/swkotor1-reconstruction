#pragma once
#include <cstdint>
namespace recovered_begin_play {
struct String { std::uint32_t data,length; };
struct Ports {
 virtual ~Ports()=default;
 virtual void* manager()=0;
 virtual void remove_subpanels_006ea830(void*)=0;
 virtual void movie_cleanup_005ee120(void*)=0;
 virtual void* getter_005ed690(void*)=0;
 virtual void unpauseable_0062b410(void*,std::uint32_t)=0;
 virtual void string_construct_005b3190(String*)=0;
 virtual void string_literal_005e5a90(String*,const char*)=0;
 virtual void string_destroy_005e5c20(String*)=0;
 virtual void create_server_00401380(void*,std::uint32_t)=0;
 virtual void save_character_006123e0(void*)=0;
 virtual String* getter_string_005ed490(void*,String*)=0;
 virtual void* net_layer_slot10(void*)=0;
 virtual void net_start_005d5040(void*,std::uint32_t,const String*,const String*,std::uint32_t,std::uint32_t)=0;
 virtual void* getter_005ed6f0(void*)=0;
 virtual void ui_begin_006775e0(void*)=0;
 virtual void format_005e5680(String*,const char*,std::uint32_t,const char*,const char*,std::uint32_t,std::uint32_t)=0;
 virtual std::uint32_t length_005e5790(String*)=0;
 virtual const char* cstr_005e5670(String*)=0;
 virtual void net_send_005d4cb0(void*,std::uint32_t,const char*,std::uint32_t,std::uint32_t)=0;
 virtual void set_temp_005edaa0(void*,const String*)=0;
 virtual void set_flags_005eda60(void*,std::uint32_t,std::uint32_t)=0;
 virtual void set_module_005edcf0(void*,const String*)=0;
 virtual String* get_module_005edd00(void*,String*,const String*)=0;
 virtual void use_module_005edd30(void*,const String*)=0;
 virtual void connect_flags_005eda80(void*,std::uint32_t,std::uint32_t,std::uint32_t)=0;
 virtual std::uint32_t get_connect_005ee090(void*,const String*,std::uint32_t)=0;
 virtual void connect_005edf00(void*,std::uint32_t)=0;
};
void class_begin_006dbdf0(void*,Ports&);
void main_begin_006eb320(void*,Ports&);
}
