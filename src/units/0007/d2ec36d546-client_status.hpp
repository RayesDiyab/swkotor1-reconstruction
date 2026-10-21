#pragma once
namespace recovered_client_status {
struct String{void*data;unsigned length;};struct Ref{unsigned char bytes[16];};
struct Ports{virtual ~Ports()=default;virtual void empty(String*)=0;virtual void destroy(String*)=0;virtual unsigned overrun(void*,unsigned)=0;virtual unsigned underrun(void*,unsigned)=0;virtual void*app()=0;virtual void*gui(void*)=0;virtual void state(void*,unsigned)=0;virtual unsigned downloaded(void*)=0;virtual void set_downloaded(void*,unsigned)=0;virtual unsigned has_character(void*)=0;virtual String*character(void*)=0;virtual void ref_from_string(Ref*,String*)=0;virtual void send_character(void*,Ref*,unsigned)=0;virtual void format_run(String*,unsigned,const char*,const char*)=0;virtual unsigned length(String*)=0;virtual const char*text(String*)=0;virtual unsigned recipient()=0;virtual void*network(void*)=0;virtual void send(void*,unsigned,const char*,unsigned,unsigned)=0;};
unsigned status_0064f280(void*,unsigned,Ports&);
}
