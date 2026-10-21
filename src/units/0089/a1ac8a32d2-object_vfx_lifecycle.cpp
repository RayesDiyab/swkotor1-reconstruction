// Original objectVFX lifecycle. Dependencies retain their existing address owners.
#include "5d6494c40d-startup_string.h"
#include "085b9d5b72-object_vfx_core.h"
#include "5290d9ecdb-object_vfx_handlers.h"
#include "61291d3f9e-object_vfx_lifecycle.h"
#include <cstdint>
#include <cstring>
#include <new>
using U=std::uint32_t;using I=std::int32_t;using H=std::uint16_t;using B=std::uint8_t;
using String=kotor_reconstruction::StartupString;
namespace aurora_model {struct Gob;struct Scene;}
namespace kotor_menu_window_cells {extern void* volatile manager_007a39fc;}
namespace kotor_sound_rev3 {
void* __fastcall SourceConstruct005d60e0(void*,void*,const void*);
void __fastcall SourceSetPriorityGroup005d5900(void*,void*,U);
}
String* __fastcall GuiConcat005e5d10(String*,void*,String*,const String*);
struct VectorWords {U x,y,z;};
struct Ref {B bytes[16];};
static_assert(sizeof(VectorWords)==12&&sizeof(Ref)==16&&sizeof(String)==8,"Original output carriers");
extern "C" {
extern void* kotor_cell_rules_007a3a28;
extern void* kotor_cell_sound_007a39ec;
extern U kotor_rules_cell32_007a21d4,kotor_rules_cell32_007a21d8,kotor_rules_cell32_007a21dc;
extern U kotor_rules_cell32_007a21e0,kotor_rules_cell32_007a21e4;
void* __fastcall kotor_fastcall_005ed580(void*,void*,U);
void* __fastcall kotor_fastcall_005ed530(void*,void*);
void* __fastcall kotor_fastcall_005ed570(void*,void*);
B __fastcall kotor_fastcall_004d8230(void*,void*,U,void**);
U __fastcall unresolved_list_next_005e9b90(void*,void*,void**);
U __fastcall gob_get_property_0x68_0063c070(aurora_model::Gob*,void*);
String* __fastcall GuiAssign005e5140(String*,void*,const char*);
char* __fastcall WindowCStr005e5670(String*,void*);
String* __fastcall resman_string_from_int_005e5bc0(String*,void*,I);
void* __fastcall async_ref_construct_string_00406d60(void*,void*,void*);
I __fastcall object_vfx_table_00414260(void*,void*,String*,I,I*);
I __fastcall object_vfx_table_00414080(void*,void*,String*,I,float*);
I __fastcall object_vfx_table_00413ec0(void*,void*,String*,I,String*);
I __fastcall kotor_m15_c2da_get_string_entry_00413de0(void*,void*,void*,void*,void*) noexcept(false);
U __fastcall kotor_m15_sound_play_3d_one_shot_005d5e10(void*,void*,void*,VectorWords,float,U,U,U,float,float);
void __fastcall kotor_m15_sound_source_set_3d_005d5910(void*,void*,I);
void __fastcall kotor_m15_sound_source_set_looping_005d59d0(void*,void*,U);
void __fastcall kotor_fastcall_005d5930(void*,void*);
void __fastcall kotor_m15_sound_source_stop_005d5a20(void*,void*);
void* __cdecl live_operator_new(U);void __cdecl live_free(void*);
void __cdecl genuine_x87_ftol_006fae8c();
void __fastcall object_vfx_006a1650(void*,void*,void*,void*,String*);
void __fastcall object_vfx_006a0d30(void*,void*);
U __fastcall object_vfx_006a0620(void*,void*,U);
U __fastcall object_vfx_0063cb50(void*,void*,U,U);
void __fastcall area_vfx_client_announce_005edf60(void*,void*,U,U,const void*);
}
namespace {
template<class T>T rd(const void*p,U o){T v;std::memcpy(&v,static_cast<const B*>(p)+o,sizeof(v));return v;}
template<class T>void wr(void*p,U o,T v){std::memcpy(static_cast<B*>(p)+o,&v,sizeof(v));}
void* ptr(const void*p,U o){return rd<void*>(p,o);}
U word(const void*p){return static_cast<U>(reinterpret_cast<std::uintptr_t>(p));}
U output_word(void*p){return static_cast<U>(reinterpret_cast<std::uintptr_t>(p));}
void* from_word(U value){return reinterpret_cast<void*>(static_cast<std::uintptr_t>(value));}
template<class F>F slot(void*p,U o){return rd<F>(ptr(p,0),o);}
// Preserve original unwind edges through extern-C providers under /EHsc.
template<class F,class...A>auto invoke(F f,A...a){F volatile target=f;return target(a...);}
using Getter=void*(__fastcall*)(void*,void*);
using WordGetter=U(__fastcall*)(void*,void*);
using GobDelete=U(__fastcall*)(aurora_model::Gob*,void*,U);
using SoundDelete=void*(__fastcall*)(void*,void*,U);
using Zero=I(__fastcall*)(aurora_model::Gob*,void*,aurora_model::Scene*);
using Disconnect=void*(__fastcall*)(void*,void*,void*,const char*,I);
using Animate=U(__fastcall*)(aurora_model::Gob*,void*,U,U,U,U);
using Timing=U(__fastcall*)(aurora_model::Gob*,void*,U,U,U);
using Target=void*(__fastcall*)(void*,void*,U,U);
void* client(){return ptr(kotor_menu_window_cells::manager_007a39fc,4);}
void* table(){return ptr(ptr(kotor_cell_rules_007a3a28,0xb8),0x88);}
void* view(void*self){void*o=invoke(kotor_fastcall_005ed580,client(),nullptr,rd<U>(self,0x40));return o?slot<Getter>(o,0x0c)(o,nullptr):nullptr;}
bool range(H n,H low,H high){return n>=low&&n<high;}
// Exact FLD dword/FMUL dword -> existing ST0 __ftol2 provider, without a store.
// claude-build: noinline so callers treat the __ftol2 CALL as an ordinary call (ECX/EDX clobbered).
__declspec(noinline) U scaled(float value,float factor){U result;__asm {
 fld dword ptr value
 fmul dword ptr factor
 call genuine_x87_ftol_006fae8c
 mov result,eax
 }return result;}
struct TemporaryString {
 alignas(String) B storage[sizeof(String)];bool active=false;
 String*get(){return reinterpret_cast<String*>(storage);}
 void destroy(){if(active){active=false;get()->~StartupString();}}
 ~TemporaryString(){destroy();}
};
void number(TemporaryString&s,H n){invoke(resman_string_from_int_005e5bc0,s.get(),nullptr,static_cast<I>(n));s.active=true;}
void remove_model(void*self){
 void*m=ptr(self,0x8c);slot<Disconnect>(m,0x50)(m,nullptr,nullptr,nullptr,0);
 m=ptr(self,0x8c);slot<Zero>(m,0x4c)(static_cast<aurora_model::Gob*>(m),nullptr,nullptr);
 m=ptr(self,0x8c);if(m)slot<GobDelete>(m,0)(static_cast<aurora_model::Gob*>(m),nullptr,1);wr<void*>(self,0x8c,nullptr);
}
void attach_name(String&name,void*placeable,void*door,const char*place,const char*door_suffix,const char*fallback){
 if(!placeable&&!door){invoke(GuiAssign005e5140,&name,nullptr,fallback);return;}
 TemporaryString suffix,joined;
 ::new(static_cast<void*>(suffix.get()))String(placeable?place:door_suffix);suffix.active=true;
 String*base=reinterpret_cast<String*>(static_cast<B*>(placeable?placeable:door)+(placeable?0x144:0x130));
 invoke(GuiConcat005e5d10,base,nullptr,joined.get(),suffix.get());joined.active=true;
 name.Assign005e5c50(joined.get());joined.destroy();suffix.destroy();
}
void one_shot(String&sound,void*target){
 Ref ref;invoke(async_ref_construct_string_00406d60,&ref,nullptr,&sound);
 invoke(kotor_m15_sound_play_3d_one_shot_005d5e10,kotor_cell_sound_007a39ec,nullptr,&ref,
 rd<VectorWords>(target,0x24),1.5f,13u,0u,0u,0.0f,0.0f);
}
}
extern "C" {
void __fastcall object_vfx_camera_006416f0(void*self,void*,U amplitude,U duration){
 if(!duration||static_cast<I>(duration)<rd<I>(self,0xdc))return;
 wr<U>(self,0xdc,duration);wr<U>(self,0xe8,duration);wr<U>(self,0xe4,amplitude);
 wr<U>(self,0xe0,70);wr<U>(self,0xec,1);
}
void __fastcall object_vfx_camera_00641730(void*self,void*,U duration){
 if(rd<U>(self,0xf0))return;wr<U>(self,0xf0,50);wr<U>(self,0xf4,duration);wr<U>(self,0xf8,1);
}
void __fastcall object_vfx_006a5000(void*self,void*){
 String name;
 void*v=view(self);if(!v)return;
 void*t=slot<Target>(v,0x98)(v,nullptr,255,1);if(!t)return;
 wr<U>(self,0xe0,1);
 void*placeable=from_word(slot<WordGetter>(v,0x4c)(v,nullptr));
 void*door=from_word(slot<WordGetter>(v,0x14)(v,nullptr));
 if(ptr(self,0x74)){
  attach_name(name,placeable,door,"_ground","grnd","root");
  invoke(object_vfx_006a1650,self,nullptr,ptr(self,0x74),t,&name);
 }
 if(ptr(self,0x78)){
  attach_name(name,placeable,door,"_impact","impc","impact");
  invoke(object_vfx_006a1650,self,nullptr,ptr(self,0x78),t,&name);
 }
 if(ptr(self,0x7c)){
  attach_name(name,placeable,door,"_head_hit","hhit","talkdummy");
  void*p=from_word(invoke(gob_get_property_0x68_0063c070,static_cast<aurora_model::Gob*>(v),nullptr));
  void*head=t;
  if(p&&rd<B>(from_word(invoke(gob_get_property_0x68_0063c070,static_cast<aurora_model::Gob*>(v),nullptr)),0xc4)==3)
   head=slot<Target>(v,0x98)(v,nullptr,254,1);
  invoke(object_vfx_006a1650,self,nullptr,ptr(self,0x7c),head,&name);
 }
 H kind=65535;const B phase=rd<B>(self,0xc6);
 if(phase==1)kind=rd<H>(self,0xa0);else if(phase==2)kind=rd<H>(self,0xa2);else if(phase==3)kind=rd<H>(self,0xa4);
 void*m=ptr(self,0x8c);
 if(m){
  if(range(kind,600,700))invoke(object_vfx_006a0d30,self,nullptr);
  else if(range(kind,300,400))invoke(object_vfx_006a0df0,self,nullptr);
  else if(range(kind,1200,1300))invoke(object_vfx_006a2e10,self,nullptr,static_cast<U>(kind));
  else{
   using Connect=void*(__fastcall*)(void*,void*,void*,const char*,I);
   Connect f=slot<Connect>(m,0x50);
   char*s=invoke(WindowCStr005e5670,reinterpret_cast<String*>(static_cast<B*>(self)+0x2c),nullptr);
   f(ptr(self,0x8c),nullptr,t,s,0);
  }
 }
 if(range(kind,100,200))invoke(object_vfx_006a3660,self,nullptr,static_cast<U>(kind));
 else if(range(kind,200,300))invoke(object_vfx_006a3740,self,nullptr,static_cast<U>(kind));
 else if(range(kind,400,500)){
  if(kind==401||kind==404)invoke(object_vfx_0063cb50,v,nullptr,rd<U>(self,0xcc),255u);
  else invoke(object_vfx_006a3e80,self,nullptr,static_cast<U>(kind));
 }else if(range(kind,500,600)){
  using Material=U(__fastcall*)(void*,void*,U,U,U,U);
  slot<Material>(v,0xa4)(v,nullptr,word(static_cast<B*>(self)+0x18),3,0,1);
 }else if(range(kind,1000,1100))invoke(object_vfx_006a34b0,self,nullptr,static_cast<U>(kind));
 else if(range(kind,1400,1500))invoke(object_vfx_006a1220,self,nullptr,static_cast<U>(kind));
 else if(range(kind,1800,1900))invoke(object_vfx_006a15d0,self,nullptr,static_cast<U>(kind));
}
void __fastcall object_vfx_006a5fc0(void*self,void*){
 if(!rd<U>(self,0xbc)||rd<U>(self,0xc0))return;
 U empty=1;
 for(U i=0;i<3;++i){
  const U off=0x74+i*4;void*m=ptr(self,off);wr<U>(self,off-0x28,0);
  if(!m)continue;empty=0;
  if(slot<Animate>(m,0x18)(static_cast<aurora_model::Gob*>(m),nullptr,word("impact"),0x3f800000u,1,0)){
   float length,unused;wr<H>(self,0x58+i*2,1);
   m=ptr(self,off);slot<Timing>(m,0x1c)(static_cast<aurora_model::Gob*>(m),nullptr,word("impact"),output_word(&unused),output_word(&length));
   wr<U>(self,off-0x28,scaled(length,1000.0f));
  }else if(rd<B>(self,0xc8)==1){
   wr<H>(self,0x58+i*2,2);m=ptr(self,off);slot<Animate>(m,0x18)(static_cast<aurora_model::Gob*>(m),nullptr,word("duration"),0x3f800000u,0,0);
  }else{wr<H>(self,0x58+i*2,1);wr<U>(self,off-0x28,1000);}
 }
 wr<B>(self,0xc6,0);
 if(rd<H>(self,0xa0)!=65535){
  if(ptr(self,0x8c))remove_model(self);
  invoke(object_vfx_006a5890,self,nullptr,static_cast<U>(rd<H>(self,0xa0)));
  invoke(object_vfx_006a54d0,self,nullptr,static_cast<U>(rd<H>(self,0xa0)));
  U k=rd<H>(self,0xa0);wr<B>(self,0xc6,1);wr<B>(self,0xd9,invoke(object_vfx_006a0930,self,nullptr,k));
  wr<U>(self,0x9c,invoke(object_vfx_006a0620,self,nullptr,k));empty=0;
 }else if(rd<H>(self,0xa2)!=65535){
  invoke(object_vfx_006a08d0,self,nullptr);invoke(object_vfx_006a5890,self,nullptr,static_cast<U>(rd<H>(self,0xa2)));
  invoke(object_vfx_006a54d0,self,nullptr,static_cast<U>(rd<H>(self,0xa2)));
  U k=rd<H>(self,0xa2);wr<B>(self,0xc6,2);wr<B>(self,0xd9,invoke(object_vfx_006a0930,self,nullptr,k));empty=0;
 }else if(rd<H>(self,0xa4)!=65535){
  invoke(object_vfx_006a08d0,self,nullptr);invoke(object_vfx_006a5890,self,nullptr,static_cast<U>(rd<H>(self,0xa4)));
  invoke(object_vfx_006a54d0,self,nullptr,static_cast<U>(rd<H>(self,0xa4)));
  U k=rd<H>(self,0xa4);wr<B>(self,0xc6,3);wr<B>(self,0xd9,invoke(object_vfx_006a0930,self,nullptr,k));
  wr<U>(self,0x9c,k==65535?0:1000);empty=0;
 }
 if(!rd<B>(self,0xd8)){
  TemporaryString row;number(row,rd<H>(self,0xc4));I kind;float amplitude,duration;
  if(!invoke(object_vfx_table_00414260,table(),nullptr,row.get(),static_cast<I>(kotor_rules_cell32_007a21d4),&kind))kind=0;
  if(!invoke(object_vfx_table_00414080,table(),nullptr,row.get(),static_cast<I>(kotor_rules_cell32_007a21d8),&amplitude))amplitude=0;
  if(!invoke(object_vfx_table_00414080,table(),nullptr,row.get(),static_cast<I>(kotor_rules_cell32_007a21dc),&duration))duration=0;
  if(kind==1){void*c=client();U d=scaled(duration,1000.0f);void*module=invoke(kotor_fastcall_005ed530,c,nullptr);invoke(object_vfx_camera_00641730,module,nullptr,d);}
  else if(kind==2){void*c=client();U a=scaled(amplitude,1000.0f);U d=scaled(duration,1000.0f);void*module=invoke(kotor_fastcall_005ed530,c,nullptr);invoke(object_vfx_camera_006416f0,module,nullptr,d,a);}
  row.destroy();
 }
 String impact;String looping;void*v=view(self);TemporaryString row;number(row,rd<H>(self,0xc4));
 if(!invoke(object_vfx_table_00413ec0,table(),nullptr,row.get(),static_cast<I>(kotor_rules_cell32_007a21e0),&impact))invoke(GuiAssign005e5140,&impact,nullptr,"");
 if(!invoke(object_vfx_table_00413ec0,table(),nullptr,row.get(),static_cast<I>(kotor_rules_cell32_007a21e4),&looping))invoke(GuiAssign005e5140,&looping,nullptr,"");
 if(v&&impact.BorrowedLength005e5790()>0)one_shot(impact,v);
 if(looping.BorrowedLength005e5790()>0){
  void*raw=invoke(live_operator_new,8u);void*s=nullptr;
  try{if(raw){Ref ref;invoke(async_ref_construct_string_00406d60,&ref,nullptr,&looping);s=invoke(kotor_sound_rev3::SourceConstruct005d60e0,raw,nullptr,&ref);}}
  catch(...){invoke(live_free,raw);throw;}
  wr<void*>(self,0xec,s);
  if(s){invoke(kotor_m15_sound_source_set_3d_005d5910,s,nullptr,1);
   invoke(kotor_sound_rev3::SourceSetPriorityGroup005d5900,ptr(self,0xec),nullptr,13u);
   invoke(kotor_m15_sound_source_set_looping_005d59d0,ptr(self,0xec),nullptr,1u);
   invoke(kotor_fastcall_005d5930,ptr(self,0xec),nullptr);}
 }
 wr<U>(self,0xbc,0);if(empty)wr<U>(self,0xc0,1);
 if(!rd<U>(self,0xf0)){
  v=view(self);if(v)invoke(area_vfx_client_announce_005edf60,client(),nullptr,static_cast<U>(rd<H>(self,0xc4)),1u,static_cast<const void*>(static_cast<B*>(v)+0x24));
  wr<U>(self,0xf0,1);
 }
 row.destroy(); // original order: row, looping, impact
}
void __fastcall object_vfx_006a6590(void*self,void*,U stop){
 if(rd<U>(self,0xbc)){wr<U>(self,0xc0,stop);return;}
 wr<U>(self,0xbc,1);wr<U>(self,0xc0,stop);bool played=false;
 for(U i=0;i<3;++i){
  const U off=0x74+i*4;void*m=ptr(self,off);wr<U>(self,off-0x28,0);if(!m)continue;
  float length,unused;slot<Timing>(m,0x1c)(static_cast<aurora_model::Gob*>(m),nullptr,word("cessation"),output_word(&unused),output_word(&length));
  m=ptr(self,off);
  if(slot<Animate>(m,0x18)(static_cast<aurora_model::Gob*>(m),nullptr,word("cessation"),0x3f800000u,1,0)){
   wr<H>(self,0x58+i*2,3);wr<U>(self,off-0x28,scaled(length,1000.0f));
   wr<U>(self,0x90,3000u-scaled(length,-1000.0f));played=true;
  }else wr<U>(self,0xac,1);
 }
 if(stop&&!played)wr<U>(self,0xb8,1);
 B phase=rd<B>(self,0xc6);
 if(phase==2){
  H old=rd<H>(self,0xa2),next=rd<H>(self,0xa4);
  if(!(range(old,300,400)&&range(next,300,400))){
   invoke(object_vfx_006a56f0,self,nullptr,static_cast<U>(old));if(ptr(self,0x8c))remove_model(self);
  }
  next=rd<H>(self,0xa4);
  if(next==65535)wr<B>(self,0xc6,0);
  else{wr<B>(self,0xc6,3);wr<B>(self,0xd9,invoke(object_vfx_006a0930,self,nullptr,static_cast<U>(next)));wr<U>(self,0x9c,1000);}
 }else if(phase==1){
  invoke(object_vfx_006a56f0,self,nullptr,static_cast<U>(rd<H>(self,0xa0)));if(ptr(self,0x8c))remove_model(self);
 }
 if(rd<H>(self,0xa4)!=65535){
  if(!(range(rd<H>(self,0xa2),300,400)&&range(rd<H>(self,0xa4),300,400)))
   invoke(object_vfx_006a5890,self,nullptr,static_cast<U>(rd<H>(self,0xa4)));
  invoke(object_vfx_006a54d0,self,nullptr,static_cast<U>(rd<H>(self,0xa4)));
 }
 String sound;void*v=view(self);TemporaryString row;number(row,rd<H>(self,0xc4));
 TemporaryString column;::new(static_cast<void*>(column.get()))String("SoundCessation");column.active=true;
 I ok=invoke(kotor_m15_c2da_get_string_entry_00413de0,table(),nullptr,static_cast<void*>(row.get()),static_cast<void*>(column.get()),static_cast<void*>(&sound));column.destroy();
 if(!ok)invoke(GuiAssign005e5140,&sound,nullptr,"");
 if(v&&sound.BorrowedLength005e5790()>0)one_shot(sound,v);
 void*s=ptr(self,0xec);
 if(s){invoke(kotor_m15_sound_source_stop_005d5a20,s,nullptr);s=ptr(self,0xec);if(s)slot<SoundDelete>(s,0)(s,nullptr,1);wr<void*>(self,0xec,nullptr);}
 if(!rd<U>(self,0xe0))invoke(object_vfx_006a0c70,self,nullptr);
 row.destroy(); // original row then sound
}
// 0063DB20..0063DB85: input word is ignored, even though RET4 consumes it.
U __fastcall object_vfx_0063db20(void*self,void*,U){
 void*list=ptr(self,0x6c);void*head=ptr(list,0);
 void*effect=head?ptr(head,8):nullptr;
 void*position=ptr(list,0); // original separately reloads the head
 while(position){
  if(!rd<U>(effect,0xc0)&&rd<U>(effect,0xa8)){
   invoke(object_vfx_006a5f00,effect,nullptr);
   invoke(object_vfx_006a5a40,effect,nullptr,0u);
  }
  effect=reinterpret_cast<void*>(static_cast<std::uintptr_t>(invoke(unresolved_list_next_005e9b90,ptr(self,0x6c),nullptr,&position)));
 }
 return 1;
}
void __fastcall object_vfx_006a5f00(void*self,void*){
 if(!rd<U>(self,0xa8))return;
 for(U i=0;i<3;++i){
  U off=0x74+i*4;void*m=ptr(self,off);if(!m)continue;
  slot<Disconnect>(m,0x50)(m,nullptr,nullptr,nullptr,0);
  m=ptr(self,off);slot<Zero>(m,0x4c)(static_cast<aurora_model::Gob*>(m),nullptr,nullptr);
 }
 void*m=ptr(self,0x8c);
 if(m){slot<Disconnect>(m,0x50)(m,nullptr,nullptr,nullptr,0);
  m=ptr(self,0x8c);slot<Zero>(m,0x4c)(static_cast<aurora_model::Gob*>(m),nullptr,nullptr);}
 H kind=65535;B phase=rd<B>(self,0xc6);
 if(phase==1)kind=rd<H>(self,0xa0);else if(phase==2)kind=rd<H>(self,0xa2);else if(phase==3)kind=rd<H>(self,0xa4);
 if(kind!=65535)invoke(object_vfx_006a56f0,self,nullptr,static_cast<U>(kind));
 wr<U>(self,0xa8,0);wr<U>(self,0xe0,0);
}
U __fastcall object_vfx_006a5a40(void*self,void*,U force){
 if(!force&&rd<U>(self,0xa8))return 1;
 void*array=invoke(kotor_fastcall_005ed570,client(),nullptr);
 if(!array)return 0;
 void*object;
 if(invoke(kotor_fastcall_004d8230,array,nullptr,rd<U>(self,0x40),&object)!=0)return 0; // byte0074666C ==0
 void*v=slot<Getter>(object,0x0c)(object,nullptr);
 void*target=slot<Target>(v,0x98)(v,nullptr,255,1);
 String name;
 void*placeable=from_word(slot<WordGetter>(v,0x4c)(v,nullptr));
 void*door=from_word(slot<WordGetter>(v,0x14)(v,nullptr));
 if(ptr(self,0x74)){
  attach_name(name,placeable,door,"_ground","grnd","root");
  invoke(object_vfx_006a1650,self,nullptr,ptr(self,0x74),target,&name);
 }
 if(ptr(self,0x78)){
  attach_name(name,placeable,door,"_impact","impc","impact");
  invoke(object_vfx_006a1650,self,nullptr,ptr(self,0x78),target,&name);
 }
 if(ptr(self,0x7c)){
  attach_name(name,placeable,door,"_head_hit","hhit","talkdummy");
  void*p=from_word(invoke(gob_get_property_0x68_0063c070,static_cast<aurora_model::Gob*>(v),nullptr));void*head=target;
  if(p&&rd<B>(from_word(invoke(gob_get_property_0x68_0063c070,static_cast<aurora_model::Gob*>(v),nullptr)),0xc4)==3)
   head=slot<Target>(v,0x98)(v,nullptr,254,1);
  invoke(object_vfx_006a1650,self,nullptr,ptr(self,0x7c),head,&name);
 }
 if(ptr(self,0x8c)){
  // The original asks for the placeable view again here, rather than reusing
  // the earlier result, while retaining the saved door view.
  placeable=from_word(slot<WordGetter>(v,0x4c)(v,nullptr));
  attach_name(name,placeable,door,"_ground","grnd","root");
  void*m=ptr(self,0x8c);Disconnect f=slot<Disconnect>(m,0x50);
  char*text=invoke(WindowCStr005e5670,&name,nullptr);
  f(ptr(self,0x8c),nullptr,target,text,0);
 }
 B phase=rd<B>(self,0xc6);wr<U>(self,0xa8,1);wr<U>(self,0xe0,1);
 H kind=65535;
 if(phase==1)kind=rd<H>(self,0xa0);else if(phase==2)kind=rd<H>(self,0xa2);else if(phase==3)kind=rd<H>(self,0xa4);
 if(kind!=65535)invoke(object_vfx_006a54d0,self,nullptr,static_cast<U>(kind));
 return 1;
}
}
