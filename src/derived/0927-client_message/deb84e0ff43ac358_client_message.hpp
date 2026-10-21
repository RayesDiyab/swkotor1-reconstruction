#pragma once
#include <cstdint>
namespace recovered_client_message {
struct String{void*data;unsigned length;};
struct Ports{virtual ~Ports()=default;virtual void*client_app()=0;virtual unsigned logging(void*)=0;virtual void*log_file(void*)=0;virtual void*timer(void*)=0;virtual std::uint64_t ticks(void*)=0;virtual void empty(String*)=0;virtual void destroy(String*)=0;virtual void header(String*,unsigned,unsigned,unsigned,unsigned,unsigned)=0;virtual void newline(String*)=0;virtual void write_string(void*,String*)=0;virtual void write_bytes(void*,const void*,unsigned,unsigned)=0;virtual void*pointer(void*,unsigned)=0;virtual void player_message(void*,unsigned char*,unsigned)=0;virtual void system_message(void*,unsigned char*,unsigned)=0;};
unsigned handle_005ee700(void*,unsigned,unsigned char*,unsigned,unsigned,Ports&);
}
