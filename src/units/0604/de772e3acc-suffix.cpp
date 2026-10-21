#include "12ed6df888-suffix.h"
#include <cstring>
namespace frame_control {
using namespace menu_continuation;
static U32 suffix_word(const void* p,U32 off){U32 v;std::memcpy(&v,static_cast<const unsigned char*>(p)+off,4);return v;}
static unsigned char suffix_byte(const void* p,U32 off){unsigned char v;std::memcpy(&v,static_cast<const unsigned char*>(p)+off,1);return v;}
static void suffix_store(void* p,U32 off,U32 v){std::memcpy(static_cast<unsigned char*>(p)+off,&v,4);}
static void* suffix_pointer(const void* p,U32 off){return reinterpret_cast<void*>(suffix_word(p,off));}
static bool suffix_positive(void* p,U32 off){
 bool yes=false;const U32 zero=0;
 __asm{mov ecx,p}__asm{add ecx,off}__asm{fld dword ptr[ecx]}
 __asm{fcomp zero}__asm{fnstsw ax}__asm{test ah,41h}__asm{jnz done}__asm{mov yes,1}
 __asm{done:}return yes;
}
static bool suffix_subtract_expired(void* p,U32 off){
 bool expired=false;const U32 zero=0;
 __asm{mov ecx,p}__asm{add ecx,off}__asm{fld dword ptr[ecx]}
 __asm{fsub dword ptr frame_delta_0078e574}



 __asm{fst dword ptr[ecx]}

 __asm{fcomp zero}__asm{fnstsw ax}__asm{test ah,41h}__asm{jp done}__asm{mov expired,1}
 __asm{done:}return expired;
}
static void suffix_subtract(void* p,U32 off){
 __asm{mov ecx,p}__asm{add ecx,off}__asm{fld dword ptr[ecx]}
 __asm{fsub dword ptr frame_delta_0078e574}__asm{fstp dword ptr[ecx]}
}
U32 __fastcall ContinueSuffix_00603db8(Client* self,void*,SuffixState* state){
 state->return_ready=0;
 const U32 timeout=suffix_word(self,0x3e0);
 if(timeout!=0){
  U32 milliseconds;
  __asm{fld dword ptr frame_delta_0078e574}
  __asm{fmul dword ptr milliseconds_0073d6fc}
  __asm{call unresolved_ftol2_006fae8c}
  __asm{mov milliseconds,eax}




  suffix_store(self,0x3e0,timeout<=milliseconds?0u:timeout-milliseconds);

 }
 if(sound_manager_007a39ec)frame_UpdateSound_005f5370(self,nullptr);
 const I64 ticks=exo_base_007a39e0->timers->GetHighResolutionTimer_005e9260();
 const U32 low=static_cast<U32>(ticks),raw_difference=low-high_resolution_low_008328f0;
 end_timer_low_008328e8=low;end_timer_high_008328ec=static_cast<U32>(static_cast<std::uint64_t>(ticks)>>32);




 __asm{fild raw_difference}

 __asm{fmul dword ptr profile_scale_0073d708}__asm{fstp dword ptr sound_elapsed_00832918}
 if(suffix_word(self,0x150)&&snapshot_enabled_007a2284){
  const bool include_gui=(suffix_byte(suffix_pointer(self,4),0x5c)&1u)!=0||suffix_byte(self->gui,0x71)!=0;
  // Original writes only the low byte of the reused profiling local.



  const U32 argument=(raw_difference&0xffffff00u)|(include_gui?1u:0u);

  frame_AurSnapshot_004211a0(static_cast<unsigned char*>(static_cast<void*>(self))+0x158,argument);
  suffix_store(self,0x150,0);
 }
 if(state->stack20_00603e74)suffix_store(self,0x288,0);
 if(suffix_positive(self,0x390)){
  if(suffix_subtract_expired(self,0x390)&&suffix_word(self,0x320)==0){



   frame_SetAutoPaused_005f3f10(self,nullptr,1,suffix_byte(self,0x398));
   static_cast<unsigned char*>(static_cast<void*>(self))[0x398]=0xff;
  }
 }else if(suffix_positive(self,0x38c))suffix_subtract(self,0x38c);
 if(suffix_positive(self,0x39c))suffix_subtract(self,0x39c);
 if(suffix_byte(self,0x37c)&4u){
  void* module=suffix_pointer(self,0x18);void* area=module?suffix_pointer(module,0x48):nullptr;
  void* server_area=area?frame_GetServerArea_00605640(area,nullptr):nullptr;
  if(!server_area||suffix_word(server_area,0x2c4)==0){
   void* server=suffix_pointer(app_manager_007a39fc,8);
   const U32 first=server?frame_GetPauseState_004ae980(server,nullptr,2):0;
   if(first!=suffix_word(self,0x380))frame_TogglePauseRequest_00677800(suffix_pointer(self,0x14c),nullptr);



   server=suffix_pointer(app_manager_007a39fc,8);
   const U32 second=server?frame_GetPauseState_004ae980(server,nullptr,2):0;

   frame_SetPaused_0062def0(self->in_game,nullptr,second,suffix_byte(self,0x388));
  }
  suffix_store(self,0x37c,suffix_word(self,0x37c)&0xfffffffbu);
 }
 const unsigned char tutorial=suffix_byte(self,0x3b8);
 if(tutorial!=0xff){
  // Genuine callee consumes one byte at stack+4; upper padding is not an input.
  frame_ShowTutorial_0062f4a0(self->in_game,nullptr,tutorial,suffix_word(self,0x3ac),suffix_word(self,0x3b0),suffix_word(self,0x3b4));
  static_cast<unsigned char*>(static_cast<void*>(self))[0x3b8]=0xff;
 }
 if(frame_LogIsEmpty_005e58a0(log_request_008328d8,nullptr)==0)return 0x00603fe3;
 // Stop at the genuine return-value assignment, before original entry/SEH
 // epilogue. The new stage is never exported as original MainLoop's entry ABI.
 state->mainloop_eax=suffix_word(self,0x250)!=0&&suffix_word(self,0x254)!=0?1u:0u;
 state->return_ready=1;
 return state->mainloop_eax?0x006040f4u:0x0060410cu;
}
U32 __fastcall ContinueAfterR2ThroughSuffix(Client* self,void*,Carry* carry,SuffixState* state){
 const U32 frontier=ContinueAfterR2_00603b6d(self,nullptr,carry);
 if(frontier!=0x00603db8u)return frontier;
 return ContinueSuffix_00603db8(self,nullptr,state);
}
}
