#include "5fa1c8f7a9-item_ai_recharge.hpp"
#include <cstring>
namespace kotor_menu_window_cells {extern void* volatile manager_007a39fc;}
namespace menu_continuation {struct WorldTimer;}
extern "C" void* __fastcall kotor_fastcall_004aede0(void*,void*);
extern "C" void __fastcall frame_GetWorldTime_004ade40(menu_continuation::WorldTimer*,void*,unsigned*,unsigned*);
extern "C" unsigned __fastcall recovered_item_property_has_00555c90(void*,void*,unsigned,unsigned);
namespace {
using namespace recovered_item_ai_recharge;
static_assert(sizeof(void*)==4,"native x86 address ports");
U word(void*p){return reinterpret_cast<U>(p);} void* ptr(U a){return reinterpret_cast<void*>(a);}
struct Native final:Ports{
 U read(U a)override{if(a==0x7a39fc)return word(kotor_menu_window_cells::manager_007a39fc);U v;std::memcpy(&v,ptr(a),4);return v;}
 U read16(U a)override{std::uint16_t v;std::memcpy(&v,ptr(a),2);return v;}
 U read8(U a)override{std::uint8_t v;std::memcpy(&v,ptr(a),1);return v;}
 void write(U a,U v)override{std::memcpy(ptr(a),&v,4);}
 U has(U a,U b,U c)override{return recovered_item_property_has_00555c90(ptr(a),nullptr,b,c);}
 U timer(U a)override{return word(kotor_fastcall_004aede0(ptr(a),nullptr));}
 void time(U a,U b,U c)override{frame_GetWorldTime_004ade40(static_cast<menu_continuation::WorldTimer*>(ptr(a)),nullptr,static_cast<unsigned*>(ptr(b)),static_cast<unsigned*>(ptr(c)));}
 void difference(U a,U b,U c,U d,U e,U f,U g)override{(void)recovered_item_ai_recharge::difference(a,b,c,d,e,f,g,*this);}
};
}
extern "C" void __fastcall recovered_item_ai_update_0055cb60(void*self,void*){
 Native p;alignas(4) unsigned char scratch[0x20];
 static_assert(sizeof(scratch)>=0x1c,"last elapsed-day DWORD ends at1C");
 recovered_item_ai_recharge::update(word(self),word(scratch),p);
}
extern "C" unsigned __fastcall recovered_world_time_difference_004ae460(void*self,void*,unsigned day1,unsigned tick1,unsigned day2,unsigned tick2,unsigned*outDay,unsigned*outTick){
 Native p;return recovered_item_ai_recharge::difference(word(self),day1,tick1,day2,tick2,word(outDay),word(outTick),p);
}
