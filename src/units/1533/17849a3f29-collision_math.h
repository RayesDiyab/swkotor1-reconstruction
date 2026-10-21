#pragma once
#include "ba24338184-collision_methods.h"
namespace collision {
using U=std::uint32_t;using I=std::int32_t;using V=kotor_scene3d::Vector;
// Debug path original raw reads are interleaved with the first FLD/FSUB.
inline void debug_difference(V* delta,V* first_copy,V* second_copy,const V* first,const V* second){__asm{
 mov esi,first
 mov edi,second
 mov ebx,first_copy
 mov ecx,second_copy
 mov eax,dword ptr[esi]
 mov edx,dword ptr[esi+4]
 mov dword ptr[ebx],eax
 fld dword ptr[ebx]
 mov eax,dword ptr[esi+8]
 mov dword ptr[ebx+4],edx
 mov edx,dword ptr[edi]
 mov dword ptr[ebx+8],eax
 mov eax,dword ptr[edi+4]
 mov esi,dword ptr[edi+8]
 mov dword ptr[ecx],edx
 fsub dword ptr[ecx]
 mov dword ptr[ecx+4],eax
 mov dword ptr[ecx+8],esi
 mov edx,delta
 fstp dword ptr[edx]
 fld dword ptr[ebx+4]
 fsub dword ptr[ecx+4]
 fstp dword ptr[edx+4]
 fld dword ptr[ebx+8]
 fsub dword ptr[ecx+8]
 fstp dword ptr[edx+8]
}}
// Only m32 inputs and original x87 operations. No intermediate C++ float values.
inline void difference(V* destination,const V* left,const V* right){__asm{
 mov eax,left
 mov ecx,right
 mov edx,destination
 fld dword ptr[eax]
 fsub dword ptr[ecx]
 fstp dword ptr[edx]
 fld dword ptr[eax+4]
 fsub dword ptr[ecx+4]
 fstp dword ptr[edx+4]
 fld dword ptr[eax+8]
 fsub dword ptr[ecx+8]
 fstp dword ptr[edx+8]
}}
inline bool outside_or_unordered(const V* position,const V* from,const float* radius){U fp_status;__asm{
 mov eax,position
 mov ecx,from
 fld dword ptr[eax]
 fsub dword ptr[ecx]
 fld dword ptr[eax+4]
 fsub dword ptr[ecx+4]
 fld dword ptr[eax+8]
 fsub dword ptr[ecx+8]
 fld st(0)
 fmul st(0),st(1)
 fld st(2)
 fmul st(0),st(3)
 faddp st(1),st(0)
 fld st(3)
 fmul st(0),st(4)
 faddp st(1),st(0)
 fstp st(3)
 fstp st(0)
 fstp st(0)
 mov edx,radius
 fld dword ptr[edx]
 fmul dword ptr[edx]
 fxch st(1)
 fcompp
 fnstsw ax
 movzx eax,ax
 mov fp_status,eax
}return (fp_status&0x4100)==0||(fp_status&0x4100)==0x4100;}
inline bool overlapping_xy(const V* delta,void* current,void* other){U fp_status;__asm{
 mov ecx,current
 mov eax,dword ptr[ecx+340h]
 fld dword ptr[eax+8]
 mov ecx,other
 mov ecx,dword ptr[ecx+340h]
 fadd dword ptr[ecx+8]
 mov ecx,delta
 fld dword ptr[ecx+4]
 fmul dword ptr[ecx+4]
 fld dword ptr[ecx]
 fmul dword ptr[ecx]
 faddp st(1),st(0)
 fld st(1)
 fmul st(0),st(2)
 fcompp
 fnstsw ax
 fstp st(0)
 movzx eax,ax
 mov fp_status,eax
}return (fp_status&0x4100)==0;}
inline bool second_is_nearer(const V* first,const V* second,const V* start){U fp_status;__asm{
 mov eax,first
 mov ecx,start
 fld dword ptr[eax]
 fsub dword ptr[ecx]
 fld dword ptr[eax+4]
 fsub dword ptr[ecx+4]
 fld dword ptr[eax+8]
 fsub dword ptr[ecx+8]
 fld st(0)
 fmul st(0),st(1)
 fld st(2)
 fmul st(0),st(3)
 faddp st(1),st(0)
 fld st(3)
 fmul st(0),st(4)
 faddp st(1),st(0)
 fstp st(3)
 fstp st(0)
 fstp st(0)
 mov eax,second
 fld dword ptr[eax]
 fsub dword ptr[ecx]
 fld dword ptr[eax+4]
 fsub dword ptr[ecx+4]
 fld dword ptr[eax+8]
 fsub dword ptr[ecx+8]
 fld st(0)
 fmul st(0),st(1)
 fld st(2)
 fmul st(0),st(3)
 faddp st(1),st(0)
 fld st(3)
 fmul st(0),st(4)
 faddp st(1),st(0)
 fcomp st(4)
 fstp st(0)
 fstp st(0)
 fnstsw ax
 fstp st(0)
 fstp st(0)
 movzx eax,ax
 mov fp_status,eax
}return (fp_status&0x0500)==0x0100;}
inline void perpendicular(V* plus,V* minus,const V* point,const V* normal){__asm{
 mov eax,normal
 mov ecx,point
 mov edx,plus
 fld dword ptr[eax+4]
 fchs
 fld dword ptr[eax]
 fld st(1)
 fadd dword ptr[ecx]
 fstp dword ptr[edx]
 fld dword ptr[ecx+4]
 fadd st(0),st(1)
 fstp dword ptr[edx+4]
 fld dword ptr[eax+8]
 fadd dword ptr[ecx+8]
 fstp dword ptr[edx+8]
 mov edx,minus
 fld dword ptr[ecx]
 fsub st(0),st(2)
 fstp dword ptr[edx]
 fld dword ptr[ecx+4]
 fsub st(0),st(1)
 fstp dword ptr[edx+4]
 fstp st(0)
 fstp st(0)
 fld dword ptr[ecx+8]
 fsub dword ptr[eax+8]
 fstp dword ptr[edx+8]
}}
}
