// x87 arithmetic/status glue only: all provider calls, geometry iteration and state live in C++ bodies.
#include "d8257ab55f-w3_trigger_numeric.h"
using namespace trigger_private;
namespace trigger_numeric {
std::uint16_t compare(const void* lhs,const void* rhs){std::uint16_t status;
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
void float_copy(const void* source,void* destination){
 __asm{
  mov ecx,source
  mov edx,destination
  fld dword ptr [ecx]
  fstp dword ptr [edx]
 }
}
void negative_forward(const void* quaternion,Direction* direction){
 const U one_bits=0x3f800000;const U* one_pointer=&one_bits;float z_component;float* z_pointer=&z_component;
 float* x_destination=&direction->x;float* z_destination=&direction->z;unsigned char* y_destination=direction->y.bytes;
 __asm{
  mov ecx,quaternion
  fld dword ptr [ecx+8]
  fmul dword ptr [ecx]
  fld dword ptr [ecx+4]
  fmul dword ptr [ecx+12]
  faddp st(1),st(0)
  fadd st(0),st(0)
  fld dword ptr [ecx+8]
  fmul dword ptr [ecx+12]
  fld dword ptr [ecx]
  fmul dword ptr [ecx+4]
  fsubp st(1),st(0)
  fadd st(0),st(0)
  fld dword ptr [ecx+8]
  fld dword ptr [ecx+4]
  fld st(1)
  fmul st(0),st(2)
  fld st(1)
  fmul st(0),st(2)
  faddp st(1),st(0)
  fadd st(0),st(0)
  mov edx,one_pointer
  fsubr dword ptr [edx]
  mov edx,z_pointer
  fstp dword ptr [edx]
  fstp st(0)
  fstp st(0)
  fxch st(1)
  fchs
  mov edx,x_destination
  fstp dword ptr [edx]
  fchs
  mov edx,y_destination
  fstp tbyte ptr [edx]
  mov edx,z_pointer
  fld dword ptr [edx]
  fchs
  mov edx,z_destination
  fstp dword ptr [edx]
 }
}
void make_center(const U* minimum,const U* maximum,Center* result){
 const U half_bits=0x3f000000;const U* half_pointer=&half_bits;float z_sum;float* z_sum_pointer=&z_sum;
 float* destination=result->xyz;unsigned char* extended_z=result->z.bytes;
 __asm{
  mov ecx,maximum
  mov edx,minimum
  fld dword ptr [ecx]
  fadd dword ptr [edx]
  fld dword ptr [ecx+4]
  fadd dword ptr [edx+4]
  fld dword ptr [ecx+8]
  fadd dword ptr [edx+8]
  mov ecx,z_sum_pointer
  fstp dword ptr [ecx]
  fxch st(1)
  mov ecx,half_pointer
  fmul dword ptr [ecx]
  mov edx,destination
  fstp dword ptr [edx]
  fmul dword ptr [ecx]
  fstp dword ptr [edx+4]
  mov ecx,z_sum_pointer
  fld dword ptr [ecx]
  mov ecx,half_pointer
  fmul dword ptr [ecx]
  fst dword ptr [edx+8]
  mov ecx,extended_z
  fstp tbyte ptr [ecx]
 }
}
std::uint8_t center_side(const Direction* direction,const Center* center,const void* camera){
 const void* y_direction=direction->y.bytes;const void* z_center=center->z.bytes;const float* x_direction=&direction->x;const float* z_direction=&direction->z;const float* xyz=center->xyz;std::uint16_t status;
 __asm{
  mov ecx,y_direction
  fld tbyte ptr [ecx]
  mov ecx,z_direction
  fld dword ptr [ecx]
  mov ecx,z_center
  fld tbyte ptr [ecx]
  fmul st(0),st(1)
  mov ecx,xyz
  fld dword ptr [ecx+4]
  fmul st(0),st(3)
  faddp st(1),st(0)
  fld dword ptr [ecx]
  mov ecx,x_direction
  fmul dword ptr [ecx]
  faddp st(1),st(0)
  mov edx,camera
  fld dword ptr [edx+8]
  fmul st(0),st(2)
  fld dword ptr [edx+4]
  fmul st(0),st(4)
  faddp st(1),st(0)
  fld dword ptr [edx]
  fmul dword ptr [ecx]
  faddp st(1),st(0)
  fcompp
  fnstsw ax
  fstp st(0)
  fstp st(0)
  mov status,ax
 }
 const U mask=(status>>8)&5u;return (mask==1u||mask==4u)?1u:2u;
}
void negative_vector(const void* source,void* destination){
 __asm{
  mov ecx,source
  mov edx,destination
  fld dword ptr [ecx]
  fchs
  fstp dword ptr [edx]
  fld dword ptr [ecx+4]
  fchs
  fstp dword ptr [edx+4]
  fld dword ptr [ecx+8]
  fchs
  fstp dword ptr [edx+8]
 }
}
std::uint8_t point_side(const void* direction,const void* point,const void* camera){std::uint16_t status;
 __asm{
  mov ecx,direction
  mov edx,point
  fld dword ptr [ecx+8]
  fmul dword ptr [edx+8]
  fld dword ptr [ecx]
  fmul dword ptr [edx]
  faddp st(1),st(0)
  fld dword ptr [ecx+4]
  fmul dword ptr [edx+4]
  faddp st(1),st(0)
  mov edx,camera
  fld dword ptr [ecx+8]
  fmul dword ptr [edx+8]
  fld dword ptr [ecx+4]
  fmul dword ptr [edx+4]
  faddp st(1),st(0)
  fld dword ptr [ecx]
  fmul dword ptr [edx]
  faddp st(1),st(0)
  fcompp
  fnstsw ax
  mov status,ax
 }
 const U mask=(status>>8)&5u;return (mask==1u||mask==4u)?1u:2u;
}
void scale_coordinate(I extent,const void* source,void* destination){const I* extent_pointer=&extent;
 __asm{
  mov ecx,extent_pointer
  mov edx,source
  fild dword ptr [ecx]
  fmul dword ptr [edx]
  mov ecx,destination
  fstp dword ptr [ecx]
 }
}
}
