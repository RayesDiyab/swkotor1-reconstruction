#include "7ab3df37696e930d_ai_event_cleanup.hpp"
#include "31535007a6dac6ab_event_action.hpp"
#include <cstdlib>
namespace recovered_ai_event_cleanup {
namespace {
struct MemoryBridge:kotor::character_load::Memory16 {Ports&p;explicit MemoryBridge(Ports&v):p(v){}U read(U a)const override{return p.read(a);}void write(U a,U b)override{p.write(a,b);}std::uint8_t read_byte(U)const override{std::abort();}void write_byte(U,std::uint8_t)override{std::abort();}void write_short(U,std::uint16_t)override{std::abort();}std::uint16_t read_short(U)const override{std::abort();}};
struct ServicesBridge:kotor::scene_event::Services {Ports&p;explicit ServicesBridge(Ports&v):p(v){}void release(U a)override{p.release(a);}void destruct_string_array_006fa852(U a,U stride,U count,U destructor)override{if(stride!=8||destructor!=0x5e5c20)std::abort();p.strings(a,count);}void allocate_list(U,U)override{std::abort();}U allocate_006fa7e6(U)override{std::abort();}void allocate_string_list_00405080(U,U)override{std::abort();}void assign_string_005e5c50(U,U)override{std::abort();}U construct_effect_00503e40(U,U)override{std::abort();}void copy_effect_00504090(U,U,U)override{std::abort();}U construct_location_004ca7a0(U)override{std::abort();}void copy_location_004ca7c0(U,U)override{std::abort();}U construct_talent_004ca9e0(U)override{std::abort();}void copy_talent_004caa10(U,U)override{std::abort();}};
}
void cleanup(U kind,U payload,Ports&p){if(!payload)return;switch(kind){case 1:p.script_release(p.read(0x7a3a00),payload);return;case 8:case 19:p.destroy(0x4af150,payload);break;case 10:case 24:case 26:p.destroy(0x4d7590,payload);break;case 21:p.destroy(0x4d4480,payload);break;case 22:p.destroy(0x4d6a40,payload);break;default:return;}p.release(payload);}
void event_destroy(U self,Ports&p){MemoryBridge m(p);ServicesBridge s(p);kotor::scene_event::destroy_event_004d7590(m,s,self);}
}
