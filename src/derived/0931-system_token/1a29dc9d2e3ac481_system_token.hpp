#pragma once
namespace recovered_client_token {
struct String{void*data;unsigned length;};struct Ports{virtual ~Ports()=default;virtual void clear(String*)=0;virtual void counted(String*,const char*,unsigned)=0;virtual void assign(String*,String*)=0;virtual void destroy(String*)=0;};
unsigned parse_006754d0(const char*,unsigned,String*,String*,String*,Ports&);
}
