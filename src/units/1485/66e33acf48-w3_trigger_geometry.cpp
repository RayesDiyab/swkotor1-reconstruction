#include "a43a781a46-w3_trigger_internal.h"
using namespace trigger_private;
extern "C" U __fastcall w5_area_intersect_line_segments_0058f840(void*,void*,float,float,float,float,float,float,float,float,float*,float*);
namespace {
// FCOM status must retain original ordered/unordered masks; neither operand is evaluated by host C++.
std::uint16_t comparison(const void* lhs,const void* rhs){std::uint16_t status;
 __asm{
  mov ecx,lhs
  mov edx,rhs
  fld dword ptr [ecx]
  fcomp dword ptr [edx]
  fnstsw ax
  mov status,ax
 }
 return status;
}
void center(const U* minimum,const U* maximum,void* output){
 const U half_bits=0x3f000000;const U* half_pointer=&half_bits;float z_difference,x_half;
 float* z_difference_pointer=&z_difference;float* x_half_pointer=&x_half;
 __asm{
  mov ecx,minimum
  mov edx,maximum
  mov eax,output
  fld dword ptr [edx]
  fsub dword ptr [ecx]
  fld dword ptr [edx+4]
  fsub dword ptr [ecx+4]
  fld dword ptr [edx+8]
  fsub dword ptr [ecx+8]
  mov edx,z_difference_pointer
  fstp dword ptr [edx]
  fxch st(1)
  mov edx,half_pointer
  fmul dword ptr [edx]
  mov edx,x_half_pointer
  fstp dword ptr [edx]
  mov edx,half_pointer
  fmul dword ptr [edx]
  mov edx,z_difference_pointer
  fld dword ptr [edx]
  mov edx,half_pointer
  fmul dword ptr [edx]
  mov edx,x_half_pointer
  fld dword ptr [edx]
  fadd dword ptr [ecx]
  fstp dword ptr [eax]
  fxch st(1)
  fadd dword ptr [ecx+4]
  fstp dword ptr [eax+4]
  fadd dword ptr [ecx+8]
  fstp dword ptr [eax+8]
 }
}
}
extern "C" void __fastcall w3_trigger_00691560(void* self,void*) noexcept(false){
 void* vertices=ld<void*>(self,0x134);U minimum[3],maximum[3];
 // Original reads vertex0 even when the signed count is zero or negative.
 for(U d=0;d<3;++d){minimum[d]=ld<U>(vertices,d*4);maximum[d]=ld<U>(vertices,d*4);}
 const I count=ld<I>(self,0x130);
 for(I i=1;i<count;++i){
  for(U d=0;d<3;++d){
   void* value=at(vertices,static_cast<U>(i)*12+d*4);
   const U status=(comparison(value,&minimum[d])>>8)&5u;
   if(status==1u||status==4u)minimum[d]=ld<U>(value,0); // parity odd of AH&5
  }
  for(U d=0;d<3;++d){
   void* value=at(vertices,static_cast<U>(i)*12+d*4);
   const U status=(comparison(value,&maximum[d])>>8)&0x41u;
   if(status==0)maximum[d]=ld<U>(value,0);
  }
 }
 center(minimum,maximum,at(self,0x24));
}
extern "C" U __fastcall w3_trigger_00691a30(void* self,void*,Vec* from,Vec* to,Vec* output) noexcept(false){
 void* const area=ld<void*>(self,0x20);
 if(!area)return 0;
 U index=0;U byte_offset=0;I signed_count=ld<I>(self,0x130);
 while(static_cast<I>(index)<signed_count){
  const U count=static_cast<U>(signed_count);const U following=index==count-1u?0u:index+1u;
  void* const vertices=ld<void*>(self,0x134);
  float current_x,current_y,next_x,next_y;std::memcpy(&current_x,at(vertices,byte_offset),4);std::memcpy(&current_y,at(vertices,byte_offset+4),4);std::memcpy(&next_x,at(vertices,following*12u),4);std::memcpy(&next_y,at(vertices,following*12u+4),4);
  float hit_x,hit_y;
  if(w5_area_intersect_line_segments_0058f840(area,nullptr,from->x,from->y,to->x,to->y,current_x,current_y,next_x,next_y,&hit_x,&hit_y)==1){
   // Original x passes through FLD/FSTP (including NaN quieting); y uses MOV; z is literal0.
   const float* hit_x_pointer=&hit_x;float* output_pointer=&output->x;
   __asm{
    mov ecx,hit_x_pointer
    mov edx,output_pointer
    fld dword ptr [ecx]
    fstp dword ptr [edx]
   }
   std::memcpy(&output->y,&hit_y,4);st<U>(output,8,0);return 1;
  }
  ++index;byte_offset+=12;signed_count=ld<I>(self,0x130);
 }
 return 0;
}
