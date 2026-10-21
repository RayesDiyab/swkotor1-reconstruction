#include "efe658bcced16cbc_player_lowers.hpp"
#include <cstring>
#include <initializer_list>
namespace recovered_connection_player {
namespace {void w(void*s,unsigned o,std::uint32_t v){std::memcpy(static_cast<unsigned char*>(s)+o,&v,4);}void h(void*s,unsigned o,std::uint16_t v){std::memcpy(static_cast<unsigned char*>(s)+o,&v,2);}void b(void*s,unsigned o,unsigned char v){static_cast<unsigned char*>(s)[o]=v;}void*at(void*s,unsigned o){return static_cast<unsigned char*>(s)+o;}void wp(void*s,unsigned o,void*p){w(s,o,static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(p)));}unsigned r(void*s,unsigned o){unsigned v;std::memcpy(&v,static_cast<unsigned char*>(s)+o,4);return v;}}
void*list_wrapper_005611f0(void*s,LowerPorts&p){void*x=p.allocate(12);wp(s,0,x?p.list_internal(x):nullptr);return s;}
void*last_update_005caf10(void*s,LowerPorts&p){
 for(unsigned o:{4u,8u,0u,0x10u,0x14u,0xcu,0x24u,0x28u,0x20u})w(s,o,0);
 w(s,0x48,0xffffffecu);h(s,0x42,0xffec);h(s,0x44,0xffec);
 for(unsigned o:{0x32u,0x33u,0x34u,0x35u,0x36u,0x37u,0x2cu,0x2du,0x2eu,0x2fu,0x30u,0x31u,0x3eu})b(s,o,0xec);
 h(s,0x52,0xffec);for(unsigned o:{0x38u,0x39u,0x3au,0x3bu,0x3cu,0x3du,0x3fu,0x40u,0x41u})b(s,o,0xec);
 w(s,0x58,0);w(s,0x4c,0);void*x=p.allocate(0x48);wp(s,0x54,x?p.combat_information(x):nullptr);
 const unsigned char count=p.skill_count();if(count){x=p.allocate(count);wp(s,0x58,x);const auto live=p.skill_count();std::memset(x,0xec,live);}return s;
}
void*last_wrapper_00560b30(void*s,LowerPorts&p){w(s,0,0x7f000000);void*x=p.allocate(0x5c);wp(s,8,x?last_update_005caf10(x,p):nullptr);b(s,4,0xff);w(s,12,1);return s;}
void*equipment_005ca7f0(void*s,LowerPorts&p){
 list_wrapper_005611f0(at(s,0x90),p);list_wrapper_005611f0(at(s,0x98),p);list_wrapper_005611f0(at(s,0x9c),p);
 for(unsigned i=0;i<18;++i){w(s,i*4,p.invalid_object());w(s,0x48+i*4,0xffffffffu);}
 for(unsigned o:{0x90u,0x98u,0x9cu})while(r(p.pointer(s,o),0)!=0){void*head=p.remove_head(p.pointer(s,o));p.free(head);}
 w(s,0x94,p.invalid_object());return s;
}
void*equipment_wrapper_00560c20(void*s,LowerPorts&p){w(s,4,0);void*x=p.allocate(0xa0);wp(s,0,x?equipment_005ca7f0(x,p):nullptr);return s;}
}
