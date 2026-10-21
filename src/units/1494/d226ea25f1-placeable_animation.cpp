#include "5b8ffa03d2-placeable_internal.h"
#include "d16a21bd22-m15_l0.h"
using namespace placeable_private;
using PlaceableVector=KotorM15VectorWords;
extern "C" {
U __fastcall kotor_fastcall_0063c380(void*,void*,U);
U __fastcall kotor_m15a_object_animation_looping_0063c300(void*,void*,U);
std::uint16_t __fastcall kotor_m15a_object_get_looping_animation_0063c100(void*,void*);
U __fastcall kotor_fastcall_0063c610(void*,void*,std::uint16_t*,float*,U*,U*);
U __fastcall kotor_fastcall_0063c470(void*,void*,U);
void __fastcall kotor_m15a_object_abort_fire_forget_animation_0063cad0(void*,void*,U);
void __fastcall kotor_fastcall_0063c9c0(void*,void*);
U __fastcall recovered_client_area_parent_corpse_exists_006057b0(void*,void*,U);
void* __fastcall async_ref_construct_string_00406d60(void*,void*,const void*);
}
namespace {
// Reused from the independently audited trigger next-animation caller. The original
// WORD output leaves two stack bytes untouched; later MOV32 transports all four.
struct alignas(4) AnimationSlot {std::uint16_t low;unsigned char upper[2];};
U raw_animation(const AnimationSlot* animation){U value;
 __asm {
  mov ecx,animation
  mov eax,dword ptr [ecx]
  mov value,eax
 }
 return value;
}
void* table(U offset){return ptr(ptr(kotor_cell_rules_007a3a28,0xB8),offset);}
}
extern "C" void __fastcall w3_placeable_animation_update_006832a0(void* self,void*){
 if(kotor_fastcall_0063c380(self,nullptr,rd<std::uint16_t>(self,0x48))==1){
  const U remaining=rd<U>(self,0x64)-rd<U>(self,0x44);wr<U>(self,0x64,remaining);
  if(remaining&0x80000000u){
   using SetAnimation=void(__fastcall*)(void*,void*,U,U,U);
   void* const methods=ptr(self,0);
   const U loop=kotor_m15a_object_get_looping_animation_0063c100(self,nullptr);
   rd<SetAnimation>(methods,0x9C)(self,nullptr,(remaining&0xFFFF0000u)|loop,0,0);
  }
 }
 if(kotor_m15a_object_animation_looping_0063c300(self,nullptr,rd<std::uint16_t>(self,0x48))!=1)return;
 AnimationSlot animation;float speed;U duration,data;
 if(kotor_fastcall_0063c610(self,nullptr,&animation.low,&speed,&duration,&data)!=1)return;
 wr<std::uint16_t>(self,0x48,animation.low);
 if(rd<U>(self,0x50)){
  const auto position=rd<PlaceableVector>(self,0x24),orientation=rd<PlaceableVector>(self,0x30);
  wr<U>(self,0x50,0);
  void* model=ptr(self,0x68);slot<Pointer>(model,0x68)(model,nullptr,&position);
  model=ptr(self,0x68);slot<Pointer>(model,0x64)(model,nullptr,&orientation);
  model=ptr(self,0x68);slot<One>(model,0x6C)(model,nullptr,0);
  model=ptr(self,0x68);slot<One>(model,0x70)(model,nullptr,0);
  kotor_m15a_object_abort_fire_forget_animation_0063cad0(self,nullptr,0);
 }
 if(kotor_fastcall_0063c470(self,nullptr,raw_animation(&animation))){
  wr<U>(self,0x50,1);
  const PlaceableVector zero{0,0,0};const float quaternion[4]={1,0,0,0};
  void* model=ptr(self,0x68);slot<Pointer>(model,0x68)(model,nullptr,&zero);
  model=ptr(self,0x68);slot<Pointer>(model,0x60)(model,nullptr,quaternion);
  model=ptr(self,0x68);slot<One>(model,0x6C)(model,nullptr,1);
  model=ptr(self,0x68);slot<One>(model,0x70)(model,nullptr,1);
  wr<U>(self,0x54,1);
 }
 U speed_bits;std::memcpy(&speed_bits,&speed,4);
 using Play=void(__fastcall*)(void*,void*,U,U,U,U);
 void* const model=ptr(self,0x68);
 slot<Play>(model,0x54)(model,nullptr,raw_animation(&animation),speed_bits,1,0);
 wr<U>(self,0x64,duration);kotor_fastcall_0063c9c0(self,nullptr);
 const auto id=animation.low;
 if(id!=0x138&&id!=0x139&&id!=0x132&&id!=0x13B&&id!=0x13A)return;
 String sound;I row;I found;
 {String column("SoundAppType");found=c2da_current_get_int_00413660(table(0x2C),nullptr,rd<B>(self,0x10C),&column,&row);}
 void* server=nullptr;
 if(kotor_m15a_object_get_server_object_0063d4b0(self,nullptr)){
  void* const owner=kotor_m15a_object_get_server_object_0063d4b0(self,nullptr);
  server=slot<Ptr>(owner,0x48)(owner,nullptr);
 }
 void* const area=ptr(self,0x20);
 if(server&&ptr(server,0x440)&&!rd<U>(server,0x444)&&area&&recovered_client_area_parent_corpse_exists_006057b0(area,nullptr,rd<U>(self,4)))row=0x35;
 else if(!found)return;
 if(id==0x138)return;
 {String column(id==0x139?"Closed":id==0x132?"Destroyed":"Used");
  found=c2da_current_get_string_00413270(table(0x70),nullptr,row,&column,&sound);}
 if(found){
  alignas(4) B reference[16];async_ref_construct_string_00406d60(reference,nullptr,&sound);
  const auto position=rd<PlaceableVector>(self,0x24);
  kotor_m15_sound_play_3d_one_shot_005d5e10(kotor_cell_sound_007a39ec,nullptr,reference,position,0.0f,0x16,0,0,0.0f,0.0f);
 }
}
