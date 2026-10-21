#include "6a3a57a963-composition.h"
#include <cstring>
using U32=menu_continuation::U32;using I32=menu_continuation::I32;
static U32 load(U32 p,U32 off){U32 v;std::memcpy(&v,reinterpret_cast<const void*>(p+off),4);return v;}
static void store(U32 p,U32 off,U32 v){std::memcpy(reinterpret_cast<void*>(p+off),&v,4);}
static unsigned char byte_at(U32 p,U32 off){unsigned char v;std::memcpy(&v,reinterpret_cast<void*>(p+off),1);return v;}
static short short_at(U32 p,U32 off){short v;std::memcpy(&v,reinterpret_cast<void*>(p+off),2);return v;}
static I32 signed_word(U32 bits){I32 v;std::memcpy(&v,&bits,4);return v;}
static U32 target(U32 r,U32 slot){return load(load(r,0),slot);}
// Only callable source-owned tables/receivers are valid. Inert original-address
// vtable tokens are outside the contract; this file defines no such table.
static void draw_virtual(U32 r,U32 slot,float d){reinterpret_cast<void(__thiscall*)(void*,float)>(target(r,slot))(reinterpret_cast<void*>(r),d);}
static void drag_virtual(U32 r,U32 x,U32 y){reinterpret_cast<void(__thiscall*)(void*,I32,I32)>(target(r,0x94))(reinterpret_cast<void*>(r),signed_word(x),signed_word(y));}
static U32 query_virtual(U32 r,U32 slot){return reinterpret_cast<U32(__thiscall*)(void*)>(target(r,slot))(reinterpret_cast<void*>(r));}
static void delete_virtual(U32 r,U32 flag){reinterpret_cast<void*(__thiscall*)(void*,U32)>(target(r,0))(reinterpret_cast<void*>(r),flag);}
static bool advance_fade(U32 manager,float delta){
 bool expired=false;const float zero=0.0f;
 __asm {
  mov ecx,manager
  fld dword ptr [ecx+44h]
  fcomp zero
  fnstsw ax
  test ah,1
  jnz done
  fld delta
  fadd dword ptr [ecx+44h]




  fst dword ptr [ecx+44h]

  fcomp dword ptr [ecx+48h]
  fnstsw ax
  test ah,41h
  jnz done
  mov expired,1
 done:
 }
 return expired;
}
void __thiscall menu_continuation::GuiManager::Draw_0040cc50(float delta){
 const U32 self=reinterpret_cast<U32>(this);
 const I32 first_count=signed_word(load(self,0x8c));



 frame_RenderBoundary_0044ed90();frame_SceneUpdate_00418300(delta);frame_SceneUpdate_00414a50(delta);

 if(byte_at(self,0x70)!=0)frame_DrawCursor_0040ae80(this,nullptr,delta);
 for(I32 i=0;i<first_count;++i){
  const U32 panel=load(load(self,0x88),static_cast<U32>(i)*4u);
  if(!panel){frame_RemoveArray_006857a0(reinterpret_cast<void*>(self+0x88),nullptr,i);--i;continue;}
  if((byte_at(panel,0x44)&4u)!=0 && frame_FindPopup_0040ab70(this,nullptr,reinterpret_cast<void*>(panel))==0xffffffffu)draw_virtual(panel,0x34,delta);
 }
 const I32 second_count=signed_word(load(self,0x98));
 for(I32 i=0;i<second_count;++i){
  const U32 panel=load(load(self,0x94),static_cast<U32>(i)*4u);
  if(!panel){frame_RemoveArray_006857a0(reinterpret_cast<void*>(self+0x94),nullptr,i);--i;continue;}
  if((byte_at(panel,0x44)&4u)!=0)draw_virtual(panel,0x34,delta);
 }
 if(load(self,0x54)){
  frame_PushRender_004591b0();
  const I32 width=short_at(self,0x6c);
  const I32 height=short_at(self,0x6e);
  float position[3]={-1.0f,-1.0f,-1.0f};
  if(frame_Viewport_004592f0(0,0,width,height,position,0.0f,1.0f)){
   drag_virtual(load(self,0x54),load(self,0),load(self,4));frame_DrawFinish_00459580();
  }
  frame_PopRender_004592b0();
 }
 const U32 client=load(reinterpret_cast<U32>(menu_continuation::app_manager_007a39fc),4);
 if(frame_UnknownClientFlag_005ee230(reinterpret_cast<void*>(client),nullptr)==0){
  if(byte_at(self,0x1c)&8u)draw_virtual(load(self,0x3c),0x34,delta);
  if(advance_fade(self,delta)){
   const U32 selected=load(self,8);
   if(selected && query_virtual(selected,0x90)!=0)store(self,0x40,load(self,8));
   store(self,0x44,0xbf800000u);
  }
 }
 if(load(self,0x20)){
  frame_PushRender_004591b0();draw_virtual(load(self,0x20),0x0c,delta);frame_PopRender_004592b0();
 }
 for(I32 i=signed_word(load(self,0x8c)-1u);i>=0;--i){
  const U32 panel=load(load(self,0x88),static_cast<U32>(i)*4u),flags=load(panel,0x44),action=(flags>>9)&3u;
  if(action==2u){frame_RemovePanel_0040c830(this,nullptr,reinterpret_cast<void*>(panel));delete_virtual(panel,1);}
  else if(action!=0u){



   store(panel,0x44,flags&0xfffff8ffu);

   frame_RemovePanel_0040c830(this,nullptr,reinterpret_cast<void*>(panel));}
 }
 if(frame_control::draw_tail_007a3d4c)frame_DrawTail_0040bec0(this,nullptr,delta);
}
