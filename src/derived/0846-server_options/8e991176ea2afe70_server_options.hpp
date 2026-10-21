#pragma once
#include <cstdint>
namespace recovered_server_options {
struct String {std::uint32_t data,length;};
struct Ports {
 virtual ~Ports()=default;
 virtual void* allocate(unsigned)=0;
 virtual void string_default(String*)=0;
 virtual void string_literal(String*,const char*)=0;
 virtual void string_destroy(String*)=0;
 virtual const char* string_data(String*)=0;
 virtual void debug_copy_name(String*)=0;
 virtual unsigned char mode()=0;
 virtual void* construct_0052b8e0(void*)=0;
 virtual void* construct_004ae4c0(void*)=0;
 virtual void* construct_004d7d80(void*,unsigned)=0;
 virtual void* construct_00409bf0(void*)=0;
 virtual void* construct_005d54a0(void*)=0;
 virtual void* construct_0054eb10(void*)=0;
 virtual void* construct_00520980(void*)=0;
 virtual void* construct_004b0780(void*)=0;
 virtual void set_app_00604790(void*,void*)=0;
 virtual void* resource_manager()=0;
 virtual void publish_resource_manager(void*)=0;
 virtual void directory_00408800(void*,String*)=0;
 virtual void fixed_004087e0(void*,String*)=0;
 virtual void remove_file_00406b20(void*,String*,unsigned)=0;
 virtual void create_directory_004068c0(void*,String*)=0;
 virtual void* talk_table()=0;
 virtual void publish_talk_table(void*)=0;
 virtual void* construct_0041d8d0(void*)=0;
 virtual void open_talk_0041e5a0(void*,String*)=0;
 virtual void* construct_005d0f40(void*)=0;
 virtual void publish_event_manager(void*)=0;
 virtual void* event_manager()=0;
 virtual const void* event_vtable_007450e4()=0;
 virtual void register_event_005d0ff0(void*,void*)=0;
 virtual void open_logs_005e93f0(String*)=0;
 virtual void* application_server()=0;
 virtual void initialize_net_005d5550(void*,void*)=0;
 virtual void initialize_004b1b00(void*)=0;
 virtual void* construct_0057a1f0(void*)=0;
 virtual void publish_007a3a28(void*)=0;
 virtual void initialize_004b1c40(void*)=0;
 virtual std::uint64_t high_resolution_005e9260()=0;
};
void set_options_004b63e0(void*,unsigned,Ports&);
}
