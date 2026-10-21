#pragma once
#include <cstdint>
namespace recovered_player_message {
struct String{unsigned char bytes[8];};
struct Route{std::uint32_t target,label;bool constant_one;};
Route route(unsigned char);
struct Ports {virtual ~Ports()=default;virtual void string_construct(String*)=0;virtual void string_destroy(String*)=0;virtual void initialize_read(void*,void*,unsigned,unsigned,unsigned)=0;virtual unsigned overrun(void*,unsigned)=0;virtual unsigned underrun(void*,unsigned)=0;virtual void*player(unsigned,unsigned)=0;virtual void format_unknown(String*,unsigned)=0;virtual void assign_label(String*,unsigned)=0;virtual unsigned handler(unsigned,void*,void*,unsigned)=0;virtual const char*text(String*)=0;virtual void format_problem(String*,bool,const char*,unsigned)=0;};
unsigned handle_00527b20(void*,unsigned,void*,unsigned,Ports&);
void initialize_read_004d66b0(void*,void*,unsigned,unsigned,unsigned);
unsigned overrun_004d6230(void*);
unsigned underrun_004d6240(void*);
}
