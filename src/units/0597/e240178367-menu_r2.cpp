#include "41bcc2cb50-menu_r2.h"
// lane-m47-core rev2 ADAPTED COPY of the M46 stage unit frame-r4-menu_r2 (frame-gui/dependencies/r2/menu_r2.cpp, base sha256
// 1f618ea7d4a9380e34785803386f36d83d47e6c8747e1ebb366af3d0364522e4).
// D76 FIX: `frame_menu::menu_bindings` / `bindings` (address-dispatch seam leased by mainloop.cpp, integer->function-pointer
// router) are removed. MainLoopMenuIncrement calls every genuine provider directly with the physical shape the stage
// router (entry_router.cpp typed_dispatch / typed_wide) already used, reads the real cells (menu_continuation::
// frame_delta_0078e574 / app_manager_007a39fc, frame_control:: resource/sound/message manager and quiet-mode cells), and the
// player slot +98h is the D21 veneer frame_menu::object_slot_98 (driver.cpp). The typed virtual adapters panel_draw ..
// player_query (integer->function-pointer casts, no caller in the stage) are not defined any more.
#include "ac2f572f78-frame_menu_direct.h"
// Lane W4 (G12) in-game continuations (lane-w4 src/frame/frame_mainloop_ingame.cpp); each resumes here.
extern "C" {
void __fastcall w4_mainloop_anim_queue_006035a6(void* client,void*);
void __fastcall w4_mainloop_world_tick_006038d6(void* client,void*,void* character,void* area);
void __fastcall w4_mainloop_server_dialog_00603ac5(void* client,void*,unsigned dialog_id);
}
#include <cstring>
namespace frame_menu {
static U32 addr(const void* p){return reinterpret_cast<U32>(p);}
static void* ptr(U32 value){return reinterpret_cast<void*>(value);}
static U32 app_manager(){return addr(menu_continuation::app_manager_007a39fc);}                     // [007A39FC]
static float delta(){return menu_continuation::frame_delta_0078e574;}
static int stop(U32 pc){diagnostic.outside_contract=true;diagnostic.blocked_at=pc;return blocked_result;}
static bool positive(U32 raw_value){
 bool yes=false;const U32 zero=0;
 __asm {
  fld raw_value
  fcomp zero
  fnstsw ax
  test ah,41h
  jnz done
  mov yes,1
 done:
 }
 return yes;
}
static U32 delta_word(U64 timer){
 const U32 scale=0x358637bdu;U32 result=0;







 U32 low_ticks=static_cast<U32>(timer);
 __asm {
  fild low_ticks
  fmul scale
  fstp result
 }

 return result;
}
static void subtract_delta(U32 self){
 float* const cell=&menu_continuation::frame_delta_0078e574;
 __asm {
  mov ecx,self
  mov edx,cell
  fld dword ptr[ecx+374h]
  fsub dword ptr[edx]
  fstp dword ptr[ecx+374h]
 }
}
static menu_continuation::Client* client_view(U32 self){return reinterpret_cast<menu_continuation::Client*>(self);}
int __thiscall MenuClient::MainLoopMenuIncrement(menu_continuation::Carry* carry){
 int first=CaptureMenuSlice();
 if(first!=blocked_result || diagnostic.blocked_at!=0x00603355u)return first;
 diagnostic.outside_contract=false;diagnostic.blocked_at=0;
 const U32 self=addr(this);
 const U64 ticks=static_cast<U64>(reinterpret_cast<menu_continuation::WorldTimer*>(load(self,0x24))->GetSnapshotTimeDifference_004adc80());
 const U32 new_delta=delta_word(ticks);std::memcpy(&menu_continuation::frame_delta_0078e574,&new_delta,4);
 frame_R4_00408d40(ptr(frame_control::resource_manager_007a39e8),nullptr,0);
 const U32 party=load(self,0x270);
 if(party){const U32 module=load(self,0x18);if(module&&load(module,0x48))frame_R4_00637050(ptr(party),nullptr,delta());}
 frame_RenderBoundary_0044ed90();
 frame_R4_006227e0(this,nullptr,delta());
 frame_R4_005f7330(this,nullptr);
 if(load(self,0x1a8)){
  U32 gui=frame_R4_005ed690(ptr(load(app_manager(),4)),nullptr);
  if(!load(gui,0xdc)){
   if(positive(load(self,0x374))){
    if(!frame_ContainsPanel_0040bd70(reinterpret_cast<menu_continuation::GuiManager*>(load(self,0x274)),nullptr,
                                     ptr(load(load(self,0x40),0x98)))&&load(self,0x1ac))
     frame_R4_00627260(ptr(load(load(self,0x40),0x98)),nullptr);
    else subtract_delta(self);
   }else if(!load(self,0x2c0)){
    frame_R4_0062cba0(ptr(load(self,0x40)),nullptr,0);
    U32 server=load(app_manager(),8);if(server)frame_R4_004ae8a0(ptr(server),nullptr);
    frame_R4_00401280(ptr(app_manager()),nullptr);
    frame_R4_005fca30(this,nullptr,0); // machine code PUSH0 and callee RET4
    frame_R4_0062c310(ptr(load(self,0x40)),nullptr);
    store(self,0x1a8,0);store(self,0x374,0);store(self,0x1ac,1);
    frame_R4_005d5e80(ptr(frame_control::sound_manager_007a39ec),nullptr,0); // pinned 0074C5EC word



    store(self,0x1ec,1);frame_R4_005f9af0(this,nullptr,1,127,2);

    frame_R4_0062aa00(ptr(load(self,0x40)),nullptr,2,1);
   }
  }
 }
 const U32 volume=load(self,0x1b0)||frame_control::quiet_mode_0083291c?0x3e800000u:0x3f800000u;
 float volume_value;std::memcpy(&volume_value,&volume,4);
 frame_R4_005f2f60(this,nullptr,volume_value);
 if(!frame_R4_005ed480(ptr(load(app_manager(),4)),nullptr)){
  frame_R4_005d57d0(ptr(load(self,0x10)),nullptr,1);frame_R4_005d5650(ptr(load(self,0x10)),nullptr,0x00400000u); //pinned 0074C538 word
 }
 if(load(self,0x32c)){
  frame_R4_005d0fc0(ptr(frame_control::message_manager_007a3a00),nullptr,reinterpret_cast<unsigned*>(self+0x330),0,0);store(self,0x32c,0);
 }
 U32 id=load(self,0x20),player=load(self,0x3c0);
 if(load(self,0x3bc)!=id){
  player=frame_R4_005ed5b0(ptr(load(app_manager(),4)),nullptr,id);



  id=load(self,0x20);store(self,0x3c0,player);store(self,0x3bc,id);

 }
 if(player){
 // The original repeats the cache check before the virtual call. The owned,
 // single-threaded contract preserves its reloads without claiming races.
 if(load(self,0x3bc)==id)player=load(self,0x3c0);
 else {player=frame_R4_005ed5b0(ptr(load(app_manager(),4)),nullptr,id);id=load(self,0x20);store(self,0x3c0,player);store(self,0x3bc,id);}
 U32 result=object_slot_98(ptr(player),nullptr,255,1);
 if(result)w4_mainloop_anim_queue_006035a6(this,nullptr); // 006035A6..00603892 (lane W4)
 }
 carry->saved_ebp_006038ab=addr(frame_GetCharacter_006346c0(ptr(load(self,0x270)),nullptr,0));
 const U32 saved_module=load(self,0x18);
 unsigned local28=0,local2c=0;




 // The two pointers are pushed before the zero-argument getter, remain on the
 // original stack across its RET0, and are consumed by GetWorldTime's RET8.
 menu_continuation::WorldTimer* world_timer=frame_GetWorldTimer_005ed4f0(ptr(load(app_manager(),4)),nullptr);
 frame_GetWorldTime_004ade40(world_timer,nullptr,&local2c,&local28);
 carry->world_time_local_28=local28;carry->world_time_local_2c=local2c;





 const U32 checked_module=saved_module;

 if(checked_module&&load(checked_module,0x48))              // 006038C3..006038D0
  w4_mainloop_world_tick_006038d6(this,nullptr,ptr(carry->saved_ebp_006038ab),ptr(load(checked_module,0x48))); // 006038D6..00603AA3 (lane W4)
 U32 gui=load(self,0x40);
 if(gui&&load(gui,0x188)!=0x7f000000u&&load(app_manager(),8))
  w4_mainloop_server_dialog_00603ac5(this,nullptr,load(gui,0x188)); // 00603AC5..00603B15 (lane W4)
 gui=load(self,0x40);                                       // 00603B16 reloads [esi+40h]
 if(gui){frame_GuiDispatch_006339c0(ptr(gui),nullptr);frame_GuiDispatch_00632180(ptr(load(self,0x40)),nullptr);}
 U32 input=load(self,0x9c);
 if((input==0||input==4)&&load(self,0x288)==0&&
    !reinterpret_cast<menu_continuation::InGame*>(load(self,0x40))->IsGlobalFading_0062ac60()&&
    !reinterpret_cast<menu_continuation::InGame*>(load(self,0x40))->IsGlobalFadeObscuring_0062ded0()){
  frame_DialogUpdate_005fa5a0(client_view(self),nullptr,delta());frame_DialogCheck_005f3ad0(client_view(self),nullptr);
 }
 return stop(0x00603b6d);
}
}
