#include "a43a781a46-w3_trigger_internal.h"
#include "6f9c13e73d-w3_trigger_render.h"
#include "d8257ab55f-w3_trigger_numeric.h"
#include "c5812aaca1-model_instance.hpp"
#include <initializer_list>
using namespace trigger_private;
extern "C" {
struct W3TriggerPartMethods;
extern const W3TriggerPartMethods w3_trigger_part_methods_007436d0;
void* __fastcall mdl_node_construct_00445210(void*,void*,const char*);
}
namespace gui_viewport_list {void __fastcall add_00444fe0(kotor_reconstruction::ArrayMutations*,void*,int);}
namespace {
struct BoundsMethods {U preceding[0x54/4];void* (__fastcall* bounds)(void*,void*);};
static_assert(offsetof(BoundsMethods,bounds)==0x54,"original Part bounds slot");
void midpoint_xy(const void* bounds,void* output){const float half=0.5f;const float* factor=&half;
 __asm{
  mov ecx,bounds
  fld dword ptr [ecx+0x5c]
  fadd dword ptr [ecx+0x68]
  fld dword ptr [ecx+0x60]
  fadd dword ptr [ecx+0x6c]
  fxch st(1)
  mov edx,factor
  fmul dword ptr [edx]
  mov ecx,output
  fstp dword ptr [ecx]
  fmul dword ptr [edx]
  fstp dword ptr [ecx+4]
 }
}
}
extern "C" void* __fastcall recovered_mdl_node_trigger_construct_0049d7b0(void* self,void*,const char* name){
 mdl_node_construct_00445210(self,nullptr,name);st<std::uint16_t>(self,0,aurora_model::kNodeTrigger);return self;
}
extern "C" void* __fastcall w3_trigger_part_construct_0049db30(void* self,void*,aurora_model::MdlNode* node){
 aurora_part_construct_004457a0(static_cast<aurora_model::Part*>(self),nullptr,node);
 st<const void*>(self,0,&w3_trigger_part_methods_007436d0);
 for(U offset:{0x5cu,0x60u,0x64u,0x68u,0x6cu,0x70u,0x78u,0x7cu,0x74u,0x84u,0x88u,0x80u,0x90u,0x94u,0x8cu,0x9cu,0xa0u,0x98u,0xd4u,0xd8u,0xd0u})st<U>(self,offset,0);
 for(U offset:{0xb4u,0xb8u,0xbcu,0xc8u})st<U>(self,offset,0x3f800000);
 for(U offset:{0xa8u,0xb0u,0xc4u,0xccu})st<std::uint8_t>(self,offset,0);
 st<U>(self,0xc0,0x3f000000);return self;
}
extern "C" void __cdecl recovered_aur_part_trigger_set_highlight_params_0049d9b0(void* self,void* color,U alpha,U first,U second,U third){
 st<U>(self,0xb4,ld<U>(color,0));st<U>(self,0xb8,ld<U>(color,4));st<U>(self,0xbc,ld<U>(color,8));
 st<U>(self,0xc0,alpha);st<std::uint8_t>(self,0xc4,static_cast<std::uint8_t>(first));st<U>(self,0xc8,second);st<std::uint8_t>(self,0xcc,static_cast<std::uint8_t>(third));
}
extern "C" void __cdecl recovered_aur_part_trigger_set_highlight_0049e610(void* self,U on,float width,U flags){w3_trigger_part_highlight_0049dc10(self,nullptr,on,width,flags);}
extern "C" void __cdecl w3_trigger_part_add_scene_0046d130(void* scene,void* part){
 void* const bounds=ld<const BoundsMethods*>(part,0)->bounds(part,nullptr);
 for(I index=0;index<ld<I>(scene,0xcc);++index){
  void* const cell=ld<void*>(ld<void*>(scene,0xc8),static_cast<U>(index)*4u);
  if(!bounds)continue;
  U center[2];midpoint_xy(bounds,center);
  if((trigger_numeric::compare(&center[0],at(cell,0x1c))>>8)&1u)continue;
  if((trigger_numeric::compare(&center[1],at(cell,0x20))>>8)&1u)continue;
  const U x=(trigger_numeric::compare(&center[0],at(cell,0x28))>>8)&0x41u;
  if(x==0||x==0x41u)continue;
  const U y=(trigger_numeric::compare(&center[1],at(cell,0x2c))>>8)&0x41u;
  if(y==0||y==0x41u)continue;
  I bounds_word;const U raw=word(bounds);std::memcpy(&bounds_word,&raw,4);
  gui_viewport_list::add_00444fe0(static_cast<kotor_reconstruction::ArrayMutations*>(at(ld<void*>(cell,0x58),0x54)),nullptr,bounds_word);
 }
}
