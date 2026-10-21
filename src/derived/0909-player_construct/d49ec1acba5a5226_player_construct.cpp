#include "f8656154f1460669_player_construct.hpp"
#include <cstring>
namespace recovered_connection_player {
namespace {void w(void*s,unsigned o,std::uint32_t v){std::memcpy(static_cast<unsigned char*>(s)+o,&v,4);}void wp(void*s,unsigned o,void*p){w(s,o,static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(p)));}void b(void*s,unsigned o,unsigned char v){static_cast<unsigned char*>(s)[o]=v;}void* at(void*s,unsigned o){return static_cast<unsigned char*>(s)+o;}}
void* base_005cda10(void*s,std::uint32_t id,Ports&p){w(s,0,p.vtable(0x0074c324));w(s,4,id);w(s,8,0);return s;}
void* short_player_005802f0(void*s,std::uint32_t id,Ports&p){base_005cda10(s,id,p);w(s,0,p.vtable(0x00749438));return s;}
void* player_00562c70(void*s,std::uint32_t id,Ports&p){
 base_005cda10(s,id,p);w(s,0,p.vtable(0x007490ac));p.string_construct(at(s,0x18));p.ref_construct(at(s,0x65));
 w(s,0x20,0);b(s,0x24,0);w(s,0x28,p.invalid_object());w(s,0x2c,0);w(s,0x30,0);w(s,0x34,p.invalid_object());
 void*x=p.allocate(4);wp(s,0xc,x?p.list_construct_005611f0(x):nullptr);
 x=p.allocate(4);wp(s,0x10,x?p.list_construct_005611f0(x):nullptr);
 x=p.allocate(16);wp(s,0x50,x?p.last_update_00560b30(x):nullptr);
 w(s,0x14,0xffffffffu);p.string_assign_empty(at(s,0x18));
 x=p.allocate(20);if(x){w(x,8,0);w(x,12,0);w(x,4,0);w(x,16,1);w(x,0,0);}wp(s,0x4c,x);
 x=p.allocate(8);wp(s,0x54,x?p.update_wrapper_00560c20(x):nullptr);
 x=p.allocate(8);if(x){w(x,4,0);w(x,0,p.invalid_object());}wp(s,0x58,x);
 w(s,0x3c,1);w(s,0x40,0);w(s,0x44,0xffffffffu);w(s,0x60,1);b(s,0x64,0);
 unsigned char result[16];p.ref_assign_empty(at(s,0x65),result);
 w(s,0x7c,0);w(s,0x38,p.invalid_object());w(s,0x5c,0);w(s,0x48,0);w(s,0x80,0);return s;
}
}
