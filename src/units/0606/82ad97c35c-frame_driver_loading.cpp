#include "6818a8b662-driver.h"
// lane-m47-core rev2 ADAPTED COPY of the M46 stage unit frame-r4-driver (frame-gui/dependencies/r2/driver.cpp, base sha256
// 651589f6934dfeeecddc62e4698dc516c0a9684d9f8682c0fdea98ff57ea8c20).
// D76 FIX: `frame_menu::bindings` (dispatch-by-address seam, filled only while mainloop.cpp's BindingLease is alive, via the
// integer->function-pointer router callable_frame::typed_dispatch) is removed. CaptureMenuSlice calls the genuine providers
// directly (frame_R4_* joins of lane-r4-lowers, same physical shapes the router used), reads the real cell
// menu_continuation::app_manager_007a39fc, and records its bounded-slice stop in the unit-owned `frame_menu::diagnostic`.
//   * 00602C90: the stage router threw OutsideContract(0x006032FE) for this in-game-only call (typed_dispatch case
//     0x00602c90); preserved unchanged (bounded menu slice), so the unreachable tail after it is not spelled.
//   * GuiManager::Draw (0040CC50 dispatch copy) had no caller in the stage (the frame uses draw.cpp's
//     menu_continuation::GuiManager::Draw_0040cc50) and is not defined here any more.
#include "100cc1add4-frame_menu_direct.h"
#include "723a50ae7e-loading_frame.hpp"
extern "C" void __fastcall client_internal_shut_down_to_main_menu_005fcbd0(void*,void*);
extern "C" void __cdecl kotor_cdecl_00401c10(float,unsigned,unsigned);
// Lane W4 (G12): in-game area-entry continuation 00602F51..00603253 (lane-w4 src/frame/frame_mainloop_ingame.cpp).
extern "C" void __fastcall w4_mainloop_area_entry_00602f51(void* client,void*,unsigned* stack20);
#include <cstring>
namespace frame_menu {
Diagnostic diagnostic{};
// Original MainLoop stack local [esp+20h]: zeroed at 00602EDB, set to 1 at 00603248 by the area-entry block, read by
// the suffix at 00603E74. Unit-owned per frame; frame_mainloop_loading.cpp copies it into SuffixState (lane W4).
U32 area_entry_stack20=0;
U32 load(U32 address,U32 offset){U32 v;std::memcpy(&v,reinterpret_cast<const void*>(address+offset),4);return v;}
void store(U32 address,U32 offset,U32 value){std::memcpy(reinterpret_cast<void*>(address+offset),&value,4);}
static unsigned char byte_at(U32 address,U32 offset){unsigned char v;std::memcpy(&v,reinterpret_cast<const void*>(address+offset),1);return v;}
static void* ptr(U32 value){return reinterpret_cast<void*>(value);}
static U32 app_manager(){return reinterpret_cast<U32>(menu_continuation::app_manager_007a39fc);}  // [007A39FC]
static void block(U32 address){diagnostic.outside_contract=true;diagnostic.blocked_at=address;}
__declspec(naked) U32 __fastcall object_slot_98(void*, void*, U32, U32) { __asm { mov eax, [ecx] } __asm { jmp dword ptr [eax + 98h] } }
struct LoadingFramePorts final:recovered_loading_frame::Ports {
 U32 app()override{return app_manager();}U32 read(U32 a,U32 o)override{return load(a,o);}void write(U32 a,U32 o,U32 v)override{store(a,o,v);}
 U32 connected(U32 a)override{return frame_R4_005ed480(ptr(a),nullptr);}void receive(U32 a,U32 v)override{frame_R4_005d57d0(ptr(a),nullptr,v);}void status(U32 a,U32 v)override{frame_R4_005d5650(ptr(a),nullptr,v);}
 // Original0074C538 is immutable .rdata DWORD00400000; evidence accompanies this source.
 U32 timeout()override{return 0x400000u;}void shutdown(U32 a)override{client_internal_shut_down_to_main_menu_005fcbd0(ptr(a),nullptr);}
 void pump(U32 raw,U32 a,U32 b)override{float f;std::memcpy(&f,&raw,4);kotor_cdecl_00401c10(f,a,b);}
 void input_delta(U32 self,U32 timer)override{const U32 low=static_cast<U32>(reinterpret_cast<menu_continuation::WorldTimer*>(timer)->GetSnapshotTimeDifference_004adc80());const float scale=menu_continuation::scale_007439bc;float value;
 __asm { fild low } __asm { fmul scale } __asm { fstp value }
 menu_continuation::frame_delta_0078e574=value;frame_R4_006227e0(ptr(self),nullptr,menu_continuation::frame_delta_0078e574);}
};
int __thiscall ClientInternal::CaptureMenuSlice(){
 const U32 self=reinterpret_cast<U32>(this);
 area_entry_stack20=0;                                  // 00602EDB
 if(const U32 gui=load(self,0x40)){unsigned zero[3]{};frame_R4_0062b020(ptr(gui),nullptr,0x7f000000u,zero);}
 // 00602EFC..00602F12: area-not-ready (+288) with a module (+18) enters 00602F18.
 // 00602F18..00602F49 resolves the cached player exactly as the original; a null
 // player returns to the common path 00603254 (00602F4B). Every non-null-player
 // continuation (00602F51..) remains excluded at its first instruction.
 if(load(self,0x288)!=0 && load(self,0x18)!=0){
  const U32 id=load(self,0x20);                        // 00602F18
  U32 player;
  if(load(self,0x3bc)==id)player=load(self,0x3c0);     // 00602F1B..00602F23
  else {
   player=frame_R4_005ed5b0(ptr(load(app_manager(),4)),nullptr,id); // 00602F2B..00602F35
   store(self,0x3c0,player);store(self,0x3bc,load(self,0x20));       // 00602F3A..00602F43 (reload +20)
  }
  // 00602F49/00602F4B; lane W4: 00602F51..00603253, then the common path 00603254 below.
  if(player)w4_mainloop_area_entry_00602f51(ptr(self),nullptr,&area_entry_stack20);
 }
 if((byte_at(self,0x1cc)&3u)==0)frame_R4_004adbd0(ptr(load(self,0x24)),nullptr);
 if((byte_at(self,0x1cc)&2u)==0)frame_R4_004adbd0(ptr(load(self,0x28)),nullptr);
 frame_R4_004adbd0(ptr(load(self,0x2c)),nullptr);
 const U32 manager=app_manager(),state=load(manager,0x14);
 if(load(state,0)!=1u){block(0x00603355);return blocked_result;}
 LoadingFramePorts ports;const int result=recovered_loading_frame::advance(self,ports);
 if(result<0){block(0x00603355);return blocked_result;}
 return result;
}
}
