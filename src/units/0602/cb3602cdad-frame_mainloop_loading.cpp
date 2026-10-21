#include "4ff7034f3a-mainloop.h"
// lane-m47-core rev2 ADAPTED COPY of the M46 stage unit frame-r4-mainloop (frame-gui/source/mainloop.cpp, base sha256
// 780de428c2a35898da409fed5ad9485bcf8dcd1d4f235656a60f493e23dba1fe).
// D76 FIX: the BindingLease that published `frame_menu::bindings` / `menu_bindings` (typed_dispatch / typed_wide address
// router) around each frame is removed together with those tables (rev2 driver.cpp / menu_r2.cpp call the providers
// directly). The bounded-slice diagnostic is the unit-owned frame_menu::diagnostic, reset at entry exactly as the local
// Diagnostic was value-initialised before (D2: same zero state per frame).
#include "100cc1add4-frame_menu_direct.h"
namespace frame_menu {extern U32 area_entry_stack20;} // lane W4: original [esp+20h] (frame_driver_loading.cpp)
namespace callable_frame {
U32 __thiscall Client::MainLoop_00602eb0(){
 // Original 00602EDB initializes stack20 to zero. The typed prefix router
 // rejects paths which may overwrite it outside the recovered menu domain.
 frame_control::SuffixState state={0,0,0};menu_continuation::Carry carry={};
 frame_menu::diagnostic={};
 const int prefix=reinterpret_cast<frame_menu::MenuClient*>(this)->MainLoopMenuIncrement(&carry);
 state.stack20_00603e74=frame_menu::area_entry_stack20; // 00603248 write consumed at 00603E74 (lane W4)
 if(!frame_menu::diagnostic.outside_contract && (prefix==0 || prefix==1))return static_cast<U32>(prefix);
 if(prefix!=frame_menu::blocked_result||frame_menu::diagnostic.blocked_at!=0x00603b6du)
  throw OutsideContract(frame_menu::diagnostic.blocked_at?frame_menu::diagnostic.blocked_at:0x006032feu);
 const U32 frontier=frame_control::ContinueAfterR2ThroughSuffix(this,nullptr,&carry,&state);
 if(frontier==0x00603fe3u){
  // Use the already-observed nonempty result; do not repeat IsEmpty/producers.
  active_log_00603fe3();state.mainloop_eax=(frame_menu::load(reinterpret_cast<U32>(this),0x250)!=0&&frame_menu::load(reinterpret_cast<U32>(this),0x254)!=0)?1u:0u;state.return_ready=1;
 }else if(frontier!=0x006040f4u&&frontier!=0x0060410cu)throw OutsideContract(frontier);
 if(!state.return_ready)throw OutsideContract(frontier);



 return state.mainloop_eax;

}
U32 __thiscall PublicClient::MainLoop_005ed7d0(){
 // Genuine wrapper loads the existing internal pointer at +4; no new object.
 return internal->MainLoop_00602eb0();
}
}

