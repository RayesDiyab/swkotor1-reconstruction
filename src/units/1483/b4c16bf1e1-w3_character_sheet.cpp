#include "18caa2625f-w3_character_sheet.h"
#include <cstring>
namespace {
using U=std::uint32_t;using B=std::uint8_t;using H=std::uint16_t;
template<class T> T load(const void* p,U off){T v;std::memcpy(&v,static_cast<const B*>(p)+off,sizeof v);return v;}
template<class T> void store(void* p,U off,T v){std::memcpy(static_cast<B*>(p)+off,&v,sizeof v);}
void* at(void* p,U off){return static_cast<B*>(p)+off;}
B byte(void* m,U bits=8){return kotor_fastcall_004d6720(m,nullptr,bits);}
U boolean(void* m){return kotor_fastcall_004d66f0(m,nullptr);}
bool overflow(void* m){return recovered_player_message::overrun_004d6230(m)!=0;}
B rule(U off){return load<B>(kotor_cell_rules_007a3a28,off);}
void* combat_info(){
 void* p=live_operator_new(0x48);if(p==nullptr)return nullptr;
 try{return kotor_m15_combat_information_construct_00552280(p,nullptr);}catch(...){live_free(p);throw;}
}
void* modifier(){
 void* p=live_operator_new(5);if(p==nullptr)return nullptr;
 try{return recovered_combat_modifier_construct_0054f260(p,nullptr);}catch(...){live_free(p);throw;}
}
void append(void* list,void* p){gui_load_array_add_00671c00(static_cast<kotor_reconstruction::ArrayMutations*>(list),nullptr,reinterpret_cast<U>(p));}
}
// 00647560 RET8. Original truncates the argument to WORD before the unsigned bound test.
extern "C" void __fastcall recovered_client_stats_set_skill_rank_00647560(void* stats,void*,U skill,U rank){
 H index=static_cast<H>(skill);if(index<rule(0xab))store<B>(load<void*>(stats,0x84),index,static_cast<B>(rank));
}
// 00647A90 RET4. Replacement deletes the old combat information before installing the new pointer.
extern "C" void __fastcall recovered_client_stats_set_combat_information_00647a90(void* stats,void*,void* info){
 void* old=load<void*>(stats,0x68);
 if(old!=nullptr){kotor_m15b_combat_information_destruct_00550e00(old,nullptr);live_free(old);}
 if(load<U>(stats,0x6c)==0)store<U>(stats,0x6c,1);
 store(stats,0x68,info);
}
// 00667CC0..006684E7 RET. Partial reads retain original prior writes and allocation leaks on overflow.
extern "C" U __fastcall recovered_client_handle_character_sheet_00667cc0(void* message,void*){
 const U id=kotor_fastcall_00692630(message,nullptr);
 void* object; // original out-slot is not preinitialized; the selected lookup writes it on both outcomes.
 void* app=load<void*>(menu_continuation::app_manager_007a39fc,4);
 if(kotor_fastcall_004d8230(kotor_fastcall_005ed570(app,nullptr),nullptr,id,&object)!=0)return 0;
 using AsCreature=void* (__fastcall*)(void*,void*);
 void* creature=load<AsCreature>(load<void*>(object,0),0x2c)(object,nullptr);
 void* stats=load<void*>(creature,0x2f8);
 const U flags=kotor_fastcall_004d6780(message,nullptr,32);if(overflow(message))return 0;
 if(flags&1){
  B v[19];for(U i=0;i<19;++i)v[i]=byte(message);
  if(overflow(message))return 0;
  constexpr U offsets[19]={0x34,0x35,0x36,0x37,0x38,0x39,0x41,0x42,0x43,0x44,0x45,0x46,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x40};
  for(U i=1;i<16;++i)store(stats,offsets[i],v[i]);
  store(stats,offsets[0],v[0]);for(U i=16;i<19;++i)store(stats,offsets[i],v[i]);
 }
 if(flags&2){B v=byte(message);if(overflow(message))return 0;store(stats,0x5c,v);}
 if(flags&4){B v=byte(message);if(overflow(message))return 0;store(stats,0x5d,v);}
 if(flags&8){B v=byte(message);if(overflow(message))return 0;store(stats,0x5e,v);}
 if(flags&0x10){U v=kotor_fastcall_004d6780(message,nullptr,32);if(overflow(message))return 0;store(stats,0x58,v);}
 if(flags&0x20){B v=byte(message);U b=boolean(message);if(overflow(message))return 0;store(stats,0x64,v);store(stats,0x60,b);}
 if(flags&0x40){
  void* info=combat_info();
  B attack=byte(message,3),attack_a=byte(message),attack_b=byte(message),ac=byte(message);
  (void)byte(message,7); // original consumes this field and discards its value.
  B attacks=byte(message,5),attacks_a=byte(message,5),attacks_b=byte(message,5),type=byte(message,4),style=byte(message,3);
  if(overflow(message))return 0;
  store(info,0,attack);store(info,1,attack_a);store(info,2,attack_b);store(info,5,ac);
  store(info,7,attacks);store(info,8,attacks_a);store(info,9,attacks_b);store(info,0xa,type);store(info,0xb,style);
  U dual=boolean(message);if(overflow(message))return 0;store(info,0xc,dual);
  if(dual==1){B a=byte(message),b=byte(message),c=byte(message,4),d=byte(message,3);if(overflow(message))return 0;store(info,3,a);store(info,4,b);store(info,0x10,c);store(info,0x11,d);}
  U count=byte(message);
  for(U i=0;i<count;++i){
   B kind=byte(message),type_=byte(message,3);
   B versus=boolean(message)==1?byte(message):rule(0xaa);
   B subtype=boolean(message)==1?byte(message,3):B{0};
   if(overflow(message))return 0;
   void* mod=modifier();store(mod,0,kind);store(mod,4,versus);store(mod,2,type_);store(mod,3,subtype);append(at(info,0x14),mod);
  }
  count=byte(message);
  for(U i=0;i<count;++i){
   B kind=byte(message),amount=byte(message,4),type_=byte(message,3);
   B versus=boolean(message)==1?byte(message):rule(0xaa);
   B subtype=boolean(message)==1?byte(message,3):B{0};
   if(overflow(message))return 0;
   void* mod=modifier();store(mod,0,kind);store(mod,1,amount);store(mod,2,type_);store(mod,4,versus);store(mod,3,subtype);append(at(info,0x20),mod);
  }
  recovered_client_stats_set_combat_information_00647a90(stats,nullptr,info);
 }
 if(flags&0x400){H value=kotor_fastcall_004d6750(message,nullptr,16);store(stats,0x50,value);} // no overflow check here.
 if(flags&0x80){
  U bits=kotor_fastcall_004d6780(message,nullptr,rule(0xab));if(overflow(message))return 0;
  for(U skill=0;static_cast<H>(skill)<rule(0xab);++skill){
   if(bits&(1u<<(skill&31u))){
    B rank=byte(message);U update=boolean(message);if(overflow(message))return 0;
    recovered_client_stats_set_skill_rank_00647560(stats,nullptr,skill,rank);
    recovered_client_set_gui_skill_rank_0064ce90(message,nullptr,creature,1,update,skill);
   }
  }
 }
 if(flags&0x200){
  kotor_m15b_creature_stats_clear_feats_00648320(stats,nullptr);
  H count=kotor_fastcall_004d6750(message,nullptr,16);if(overflow(message))return 0;
  for(U i=0;i<count;++i){H value=kotor_fastcall_004d6750(message,nullptr,16);if(overflow(message))return 0;kotor_m15_array_u16_add_unique_005a7670(at(stats,0xb0),nullptr,value);}
  count=kotor_fastcall_004d6750(message,nullptr,16);if(overflow(message))return 0;
  for(U i=0;i<count;++i){H value=kotor_fastcall_004d6750(message,nullptr,16);if(overflow(message))return 0;kotor_m15_array_u16_add_unique_005a7670(at(stats,0xbc),nullptr,value);}
 }
 return 1;
}

