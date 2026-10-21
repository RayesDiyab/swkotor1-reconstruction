#include "4b60cd5c5fb7b24b_server_policy.hpp"
#include <cstring>
namespace recovered_server_policy {
namespace {unsigned read(void*p,unsigned o){unsigned x;std::memcpy(&x,(char*)p+o,4);return x;}void*get(void*p,unsigned o){return reinterpret_cast<void*>(static_cast<std::uintptr_t>(read(p,o)));}void word(void*p,unsigned o,unsigned x){std::memcpy((char*)p+o,&x,4);}}
unsigned net_server_005d4fb0(void*self,String*name,unsigned unused,Ports&p){(void)unused;if(read(self,0x2003c)==1)return 0;word(self,0x2003c,1);p.string_assign((char*)self+0x20040,name);p.string_empty((char*)self+0x20048);p.string_empty((char*)self+0x20050);p.string_empty((char*)self+0x20058);return 1;}
void difficulty_0054ea70(void*self,std::int32_t level){if(level>4)level=4;else if(level<0)level=0;word(self,0x108,static_cast<unsigned>(level));}
void initialize_004b1b00(void*self,Ports&p){
 std::uint32_t ini;p.ini_construct(&ini);String a,b,c,name;p.string_default(&a);p.string_default(&b);p.string_default(&c);p.string_literal(&name,"Server");net_server_005d4fb0(get(self,0x10040),&name,16,p);p.string_destroy(&name);
 word(get(self,0x10004),0x80,16);difficulty_0054ea70(get(self,0x10004),2);word(get(self,0x10004),0xc4,1);word(get(self,0x10004),0xe0,0);word(get(self,0x10004),0xec,0);
 void*info=get(self,0x10004);word(self,0x1b930,0);word(info,0x98,1);word(get(self,0x10004),0xc8,1);
 p.string_destroy(&c);p.string_destroy(&b);p.string_destroy(&a);p.ini_destroy(&ini);
}
}
