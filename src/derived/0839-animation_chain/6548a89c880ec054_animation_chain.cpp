// Restored SetAnimation chain, original KOTOR1 entry addresses documented per body.
// All five-word internal calls preserve raw float argument bits and original ordering.
#include "5a8101d0a32e3df0_m15e_animbase_internal.h"
using namespace kotor_m15e_ab;
extern "C" void* __fastcall kotor_m15_client_get_item_by_id_005ed5a0(void*,void*,U);
extern "C" void* __fastcall kotor_m15b_item_get_base_item_005b4790(void*,void*);
namespace {
struct AnimationName {
 alignas(StartupString) unsigned char bytes[sizeof(StartupString)]; bool live=false;
 StartupString* get(){return reinterpret_cast<StartupString*>(bytes);}
 void construct(void* self,U id){avt(self)->s040(self,nullptr,get(),id);live=true;}
 const char* text(){return WindowCStr005e5670(get(),nullptr);}
 ~AnimationName(){if(live)get()->~StartupString();}
};
U normalized_rate(U rate){return bitsf(rate)==0.0f?fbits(1.0f):rate;}
int animation_kind(U animation){
 switch(animation&0xffffu){
 case 10003:case 10081:case 10083:case 10212:case 10238:case 10256:case 10258:case 10271:return 1;
 case 10136:case 10029:case 10030:case 10034:case 10038:case 10039:case 10040:case 10041:case 10042:case 10070:case 10071:case 10129:case 10130:case 10154:case 10155:case 10302:return 2;
 default:return 0;
 }
}
U play(void* model,const char* name,U rate,U blend,U flags){return gvt(model)->s018_gob_play_animation(gob(model),nullptr,wd(name),rate,blend,flags);}
void* model_part(void* object,U part){using Fn=void*(__fastcall*)(void*,void*,U,U);auto table=ld<const unsigned char*>(object,0);Fn f;std::memcpy(&f,table+0x98,sizeof(f));return f(object,nullptr,part,1);}
}
extern "C" {
// 0069E8D0: name lookup always occurs; a null model returns0. Original switch tables choose overlay or reverse-rate.
U __fastcall recovered_anim_base_internal_0069e8d0(void* self,void*,U model_word,U animation,U rate,U blend,U flags){
 AnimationName name;name.construct(self,animation);void* model=reinterpret_cast<void*>(model_word);if(!model)return 0;
 rate=normalized_rate(rate);const int kind=animation_kind(animation);
 if(kind==2){kotor_m15e_anim_base_set_overlay_animation_0069e420(self,nullptr,model,animation,1);return 1;}
 if(kind==1&&bitsf(rate)>=0.0f)rate=fbits(-bitsf(rate));
 play(model,name.text(),rate,blend,flags);return 1;
}
// 0069C940: tail/wings receive named animation with default fallback, then the requested model is handled by base.
U __fastcall recovered_anim_tw_internal_0069c940(void* self,void*,U model,U animation,U rate,U blend,U flags){
 rate=normalized_rate(rate);
 if(pv(self,0x16c)||pv(self,0x170)){
  AnimationName name;name.construct(self,animation);
  for(unsigned off:{0x16cu,0x170u}){void* extra=pv(self,off);if(extra&&play(extra,name.text(),rate,blend,flags)==0)play(pv(self,off),"default",rate,blend,flags);}
 }
 return recovered_anim_base_internal_0069e8d0(self,nullptr,model,animation,rate,blend,flags);
}
// 0069A3E0: wield this is +5Ch from its vbptr. Animate right weapon unless baseitem1Dh, then all left parts.
U __fastcall recovered_anim_wield_internal_0069a3e0(void* self,void*,U model,U animation,U rate,U blend,U flags){
 rate=normalized_rate(rate);
 void* right=kotor_m15_client_get_item_by_id_005ed5a0(client_app(),nullptr,ld<U>(self,-0x20));
 void* left=kotor_m15_client_get_item_by_id_005ed5a0(client_app(),nullptr,ld<U>(self,-0x1c));
 if(right||left){
  void* tw=at(self,vbe(self,-0x5c,1)-0x5c);AnimationName name;name.construct(tw,animation);
  if(right&&ld<U8>(kotor_m15b_item_get_base_item_005b4790(right,nullptr),0xac)!=0x1d){
   void* object=at(right,0x10);for(U part:{0xffu,0xfeu,0xfdu}){void* g=model_part(object,part);if(g)play(g,name.text(),rate,blend,flags);}
  }
  if(left){void* object=at(left,0x10);for(U part:{0xffu,0xfeu,0xfdu}){void* g=model_part(object,part);if(g)play(g,name.text(),rate,blend,flags);}}
 }
 return recovered_anim_tw_internal_0069c940(at(self,vbe(self,-0x5c,1)-0x5c),nullptr,model,animation,rate,blend,flags);
}
// 00699960 thunk + 006993B0 body: wield/body must succeed before head; vbtable entries reloaded after call.
U __fastcall recovered_anim_hw_set_00699960(void* vbase,void*,U animation,U rate,U blend,U flags){
 void* self=at(vbase,-static_cast<std::ptrdiff_t>(ld<I>(vbase,-4)));
 void* body=pv(self,vbe(self,-8,1)+0xb0);void* wield=at(self,vbe(self,-8,3)+0x54);
 if(!recovered_anim_wield_internal_0069a3e0(wield,nullptr,wd(body),animation,rate,blend,flags))return 0;
 void* head=pv(self,vbe(self,-8,2)+0x3c);void* tw=at(self,vbe(self,-8,1)-8);
 return recovered_anim_tw_internal_0069c940(tw,nullptr,wd(head),animation,rate,blend,flags)?1u:0u;
}
}
