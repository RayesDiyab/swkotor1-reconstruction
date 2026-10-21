#include "6b477b16ff-w3_trigger_render_internal.h"
#include "d8257ab55f-w3_trigger_numeric.h"
using namespace trigger_private;using namespace trigger_render;
namespace {
void negate_float(const void* source,void* destination){
 __asm{
  mov ecx,source
  fld dword ptr [ecx]
  fchs
  mov edx,destination
  fstp dword ptr [edx]
 }
}
void add_height(const void* source,const void* height,void* destination){
 __asm{
  mov ecx,source
  fld dword ptr [ecx]
  mov edx,height
  fadd dword ptr [edx]
  mov ecx,destination
  fstp dword ptr [ecx]
 }
}
void lower_wall(const void* upper,const void* height,void* destination){
 __asm{
  mov ecx,height
  fld dword ptr [ecx]
  fadd st(0),st(0)
  mov edx,upper
  fsubr dword ptr [edx]
  mov ecx,destination
  fstp dword ptr [ecx]
 }
}
void lower_vertex(const void* vertex,const void* height,U* output){
 output[0]=ld<U>(vertex,0);
 __asm{
  mov ecx,height
  fld dword ptr [ecx]
  fchs
  mov edx,vertex
  fld dword ptr [edx+4]
  mov ecx,output
  fstp dword ptr [ecx+4]
  fadd dword ptr [edx+8]
  fstp dword ptr [ecx+8]
 }
}
}
extern "C" void __fastcall w3_trigger_part_highlight_0049dc10(void* self,void*,U on,float width,U flags){
 const U zero=0;const U mask=(trigger_numeric::compare(&width,&zero)>>8)&5u;
 st<std::uint8_t>(self,0xb0,static_cast<std::uint8_t>(flags));st<std::uint8_t>(self,0xa8,static_cast<std::uint8_t>(on));
 if(mask==1u||mask==4u)negate_float(&width,at(self,0xac));else trigger_numeric::float_copy(&width,at(self,0xac));
 if(ld<I>(self,0x78)<2||(on&0xffu)==0)return;
 st<U>(self,0x84,0);st<U>(self,0x90,0);st<U>(self,0x9c,0);
 for(I index=0;index<ld<I>(self,0x78);++index){
  void* const vertex=at(ld<void*>(self,0x74),static_cast<U>(index)*12u);
  U point[3]={ld<U>(vertex,0),ld<U>(vertex,4),0};add_height(at(vertex,8),at(self,0xac),&point[2]);
  append_vector(list(self,0x80),point);append_vector(list(self,0x8c),point);
  lower_wall(&point[2],at(self,0xac),&point[2]);append_vector(list(self,0x80),point);
 }
 const U count=ld<U>(self,0x78);U offset=count*12u;
 for(U remaining=count;remaining!=0;--remaining){
  offset-=12;U point[3];lower_vertex(at(ld<void*>(self,0x74),offset),at(self,0xac),point);append_vector(list(self,0x98),point);
 }
 U point[3];for(U d=0;d<3;++d)point[d]=ld<U>(ld<void*>(self,0x80),d*4u);append_vector(list(self,0x80),point);
 for(U d=0;d<3;++d)point[d]=ld<U>(ld<void*>(self,0x80),12u+d*4u);append_vector(list(self,0x80),point);
 kotor_scene3d::Vector minimum,maximum;
 const auto* result=w3_trigger_vector_list_min_004ab060(&minimum,list(self,0x80));for(U d=0;d<3;++d)st<U>(self,0x5c+d*4u,ld<U>(result,d*4u));
 result=w3_trigger_vector_list_max_004aafc0(&maximum,list(self,0x80));for(U d=0;d<3;++d)st<U>(self,0x68+d*4u,ld<U>(result,d*4u));
}
