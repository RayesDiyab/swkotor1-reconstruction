#pragma once
#include "e0867b18fc-door_area_contracts.h"
namespace door_area_x87 {
using door::U;using door::V;
inline U index_bits(const V* origin,const V* second){
    const float margin=3.0f;U result;
    __asm {mov ecx,origin
        mov edx,second
        fld dword ptr[ecx]
        fsub dword ptr[edx]
        fsub margin
        fstp result
    }
    return result;
}
// 0=skip creature,1=consider creature,2=finish sorted-list traversal.
inline U bounds_action(const V* second,const V* third,const V* current){
    const float margin=3.0f;U result;
    __asm {mov ecx,third
        mov edx,current
        fld dword ptr[ecx]
        fadd margin
        fcomp dword ptr[edx]
        fnstsw ax
        test ah,41h
        jnp area_bounds_finish
        mov ecx,second
        fld dword ptr[ecx+4]
        fsub margin
        fcomp dword ptr[edx+4]
        fnstsw ax
        test ah,1
        je area_bounds_skip
        mov ecx,third
        fld dword ptr[ecx+4]
        fadd margin
        fcomp dword ptr[edx+4]
        fnstsw ax
        test ah,41h
        jnp area_bounds_skip
        mov result,1
        jmp area_bounds_done
    area_bounds_finish:
        mov result,2
        jmp area_bounds_done
    area_bounds_skip:
        mov result,0
    area_bounds_done:
    }
    return result;
}
inline void difference(V* result,const V* left,const V* right){
    __asm {mov ecx,left
        mov edx,right
        mov eax,result
        fld dword ptr[ecx]
        fsub dword ptr[edx]
        fstp dword ptr[eax]
        fld dword ptr[ecx+4]
        fsub dword ptr[edx+4]
        fstp dword ptr[eax+4]
        fld dword ptr[ecx+8]
        fsub dword ptr[edx+8]
        fstp dword ptr[eax+8]
    }
}
inline void add(V* result,const V* left,const V* right){
    __asm {mov ecx,left
        mov edx,right
        mov eax,result
        fld dword ptr[ecx]
        fadd dword ptr[edx]
        fstp dword ptr[eax]
        fld dword ptr[ecx+4]
        fadd dword ptr[edx+4]
        fstp dword ptr[eax+4]
        fld dword ptr[ecx+8]
        fadd dword ptr[edx+8]
        fstp dword ptr[eax+8]
    }
}
inline bool ordered_zero(const V* point){
    const float zero=0.0f;U result;
    __asm {mov ecx,point
        fld dword ptr[ecx]
        fcomp zero
        fnstsw ax
        test ah,44h
        jp action_not_zero
        fld dword ptr[ecx+4]
        fcomp zero
        fnstsw ax
        test ah,44h
        jp action_not_zero
        fld dword ptr[ecx+8]
        fcomp zero
        fnstsw ax
        test ah,44h
        jp action_not_zero
        mov result,1
        jmp action_zero_done
    action_not_zero:
        mov result,0
    action_zero_done:
    }
    return result!=0;
}
inline bool second_farther(const V* first,const V* second){
    unsigned short status;
    __asm {mov ecx,first
        mov edx,second
        fld dword ptr[ecx+8]
        fmul dword ptr[ecx+8]
        fld dword ptr[ecx+4]
        fmul dword ptr[ecx+4]
        faddp st(1),st(0)
        fld dword ptr[ecx]
        fmul dword ptr[ecx]
        faddp st(1),st(0)
        fld dword ptr[edx+8]
        fmul dword ptr[edx+8]
        fld dword ptr[edx+4]
        fmul dword ptr[edx+4]
        faddp st(1),st(0)
        fld dword ptr[edx]
        fmul dword ptr[edx]
        faddp st(1),st(0)
        fcompp
        fnstsw ax
        mov status,ax
    }
    return (status&0x4100u)==0;
}
}
