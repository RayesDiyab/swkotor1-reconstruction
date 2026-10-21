#include "a43a781a46-w3_trigger_internal.h"
#include "cca80f7c35-m15e.h"
#include "cc436f6baa-m15e_animbase.h"
using namespace trigger_private;
namespace menu_continuation {struct WorldTimer;}
namespace kotor_menu_window_cells {extern void* volatile manager_007a39fc;}
extern "C" {
menu_continuation::WorldTimer* __fastcall frame_GetWorldTimer_005ed4f0(void*,void*);
void __fastcall frame_GetWorldTime_004ade40(menu_continuation::WorldTimer*,void*,U*,U*);
U __fastcall recovered_world_time_difference_004ae460(void*,void*,U,U,U,U,U*,U*);
void* __fastcall kotor_fastcall_005ed570(void*,void*);
std::uint8_t __fastcall recovered_object_array_remove_004d81a0(void*,void*,U,void**);
U __fastcall kotor_fastcall_0063c380(void*,void*,U);
U __fastcall kotor_m15a_object_animation_looping_0063c300(void*,void*,U);
U __fastcall kotor_fastcall_0063c610(void*,void*,std::uint16_t*,float*,U*,U*);
void __fastcall kotor_fastcall_0063c9c0(void*,void*);
void __cdecl live_free(void*);
}
namespace {
void* client(){return ld<void*>(const_cast<void*>(kotor_menu_window_cells::manager_007a39fc),4);}
struct alignas(4) AnimationSlot {std::uint16_t low;unsigned char upper[2];};
// 0063C610 writes only the low16; original MOV32 deliberately transports the remaining stackword.
U raw_animation(const AnimationSlot* slot){U result;
 __asm{
  mov ecx,slot
  mov eax,dword ptr [ecx]
  mov result,eax
 }
 return result;
}
}
extern "C" void __fastcall w3_trigger_00691970(void* self,void*) noexcept(false){
 const U animation=ld<std::uint16_t>(self,0x48);
 if(kotor_fastcall_0063c380(self,nullptr,animation)==1){
  const U remaining=ld<U>(self,0x64)-ld<U>(self,0x44);st<U>(self,0x64,remaining);
  if(remaining&0x80000000u){st<U>(self,0xe4,1);st<U>(self,0x13c,500);st<U>(self,0x64,0);return;}
 }
 if(kotor_m15a_object_animation_looping_0063c300(self,nullptr,ld<std::uint16_t>(self,0x48))==1){
  AnimationSlot next_animation;float speed;U duration,data;
  if(kotor_fastcall_0063c610(self,nullptr,&next_animation.low,&speed,&duration,&data)==1){
   const U full_animation=raw_animation(&next_animation);U speed_bits;std::memcpy(&speed_bits,&speed,4);
   st<std::uint16_t>(self,0x48,static_cast<std::uint16_t>(full_animation));
   void* const model=ld<void*>(self,0x68);
   ld<const KotorM15eAnimBaseVtbl*>(model,0)->s054(model,nullptr,full_animation,speed_bits,1,0);
   st<U>(self,0x64,duration);kotor_fastcall_0063c9c0(self,nullptr);
  }
  if(data)live_free(ptr(data));
 }
}
extern "C" void __fastcall recovered_client_trigger_ai_update_00692410(void* self,void*) noexcept(false){
 U day,tick,elapsed_day,elapsed_tick;
 frame_GetWorldTime_004ade40(frame_GetWorldTimer_005ed4f0(client(),nullptr),nullptr,&day,&tick);
 const U previous_tick=ld<U>(self,0x40);
 const U previous_day=ld<U>(self,0x3c);
 auto* const difference_timer=frame_GetWorldTimer_005ed4f0(client(),nullptr);
 recovered_world_time_difference_004ae460(difference_timer,nullptr,day,tick,previous_day,previous_tick,&elapsed_day,&elapsed_tick);
 const U countdown=ld<U>(self,0xf0);
 if(countdown){const U prior_delta=ld<U>(self,0x44);st<U>(self,0xf0,countdown>prior_delta?countdown-prior_delta:0);}
 st<U>(self,0x44,elapsed_tick);
 if(ld<U>(self,0xe4)){
  const U deletion_time=ld<U>(self,0x13c);
  if(deletion_time>elapsed_tick){st<U>(self,0x13c,deletion_time-elapsed_tick);return;}
  void* removed;
  void* const object_array=kotor_fastcall_005ed570(client(),nullptr);
  const U object_id=ld<U>(self,4);
  if(recovered_object_array_remove_004d81a0(object_array,nullptr,object_id,&removed)==0 && removed){
   ld<const KotorM15ObjectMethods_007516c8*>(self,0)->s000_scalar_deleting_destructor(self,nullptr,1);return;
  }
  return;
 }
 w3_trigger_00691560(self,nullptr);
 ld<const KotorM15ObjectMethods_007516c8*>(self,0)->s0e8_update_pop_up_bubble(self,nullptr);
 w3_trigger_006920b0(self,nullptr);w3_trigger_00691970(self,nullptr);
 frame_GetWorldTime_004ade40(frame_GetWorldTimer_005ed4f0(client(),nullptr),nullptr,&day,&tick);
 st<U>(self,0x3c,day);st<U>(self,0x40,tick);
}
