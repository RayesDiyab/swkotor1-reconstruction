#pragma once
#include "2d1a6b5349-door_contracts.h"
// Bounded original arithmetic sequences; C++ retains the surrounding control
// flow. No qword transport or extra m32 narrowing is introduced in these tests.
namespace door_x87 {
using door::V;using door::U;
inline bool forward_dot(const V* position,const V* player,const V* direction,float* rounded){
    const float zero=0.0f;unsigned short status;
    __asm {mov ecx,position
        mov edx,player
        fld dword ptr[ecx+8]
        fsub dword ptr[edx+8]
        fld dword ptr[ecx+4]
        fsub dword ptr[edx+4]
        fld dword ptr[ecx]
        fsub dword ptr[edx]
        mov ecx,direction
        fld dword ptr[ecx+4]
        fmul st(0),st(2)
        fxch st(1)
        fmul dword ptr[ecx]
        faddp st(1),st(0)
        fxch st(2)
        fmul dword ptr[ecx+8]
        faddp st(2),st(0)
        fxch st(1)
        mov edx,rounded
        fst dword ptr[edx]
        fstp st(1)
        fcomp zero
        fnstsw ax
        mov status,ax
    }
    const unsigned mask=status&0x4100u;
    return mask==0||mask==0x4100u; // Original positive or unordered only.
}
inline void scale_direction(const V* direction,V* scaled){
    const float eight=8.0f;
    __asm {mov ecx,direction
        mov edx,scaled
        fld dword ptr[ecx+8]
        fmul eight
        fld dword ptr[ecx+4]
        fmul eight
        fld dword ptr[ecx]
        fmul eight
        fstp dword ptr[edx]
        fstp dword ptr[edx+4]
        fstp dword ptr[edx+8]
    }
}
inline void height(const V* source,V* start,V* hit,bool add_offset){
    const float height_offset=1.0f;
    __asm {mov ecx,source
        fld dword ptr[ecx+8]
        cmp add_offset,0
        je door_height_ready
        fadd height_offset
    door_height_ready:
        mov ecx,start
        mov edx,hit
        fst dword ptr[ecx+8]
        fstp dword ptr[edx+8]
    }
}
inline bool fallback_distance(const V* difference,const float* rounded_dot,float* rounded_squared){
    const float sixty_four=64.0f;
    const unsigned long long cosine_bits=0x3febb645a1cac083ull;
    U result;
    __asm {mov ecx,difference
        fld dword ptr[ecx+8]
        fmul dword ptr[ecx+8]
        fld dword ptr[ecx+4]
        fmul dword ptr[ecx+4]
        faddp st(1),st(0)
        fld dword ptr[ecx]
        fmul dword ptr[ecx]
        faddp st(1),st(0)
        mov edx,rounded_squared
        fst dword ptr[edx]
        fcomp sixty_four
        fnstsw ax
        test ah,41h
        jp door_distance_no
        mov ecx,rounded_dot
        fld dword ptr[ecx]
        fcomp qword ptr cosine_bits
        fnstsw ax
        test ah,1
        jne door_distance_no
        mov result,1
        jmp door_distance_done
    door_distance_no:
        mov result,0
    door_distance_done:
    }
    return result!=0;
}
inline bool model_target(const V* start,const V* hit,const V* previous,U previous_id){
    U result;
    __asm {mov ecx,start
        mov edx,hit
        fld dword ptr[ecx]
        fsub dword ptr[edx]
        fld dword ptr[ecx+4]
        fsub dword ptr[edx+4]
        fld dword ptr[ecx+8]
        fsub dword ptr[edx+8]
        cmp previous_id,7f000000h
        je door_model_empty
        mov ecx,previous
        fld dword ptr[ecx+8]
        fmul dword ptr[ecx+8]
        fld dword ptr[ecx+4]
        fmul dword ptr[ecx+4]
        faddp st(1),st(0)
        fld dword ptr[ecx]
        fmul dword ptr[ecx]
        faddp st(1),st(0)
        fld st(1)
        fmul st(0),st(2)
        fld st(3)
        fmul st(0),st(4)
        faddp st(1),st(0)
        fld st(4)
        fmul st(0),st(5)
        faddp st(1),st(0)
        fcompp
        fstp st(0)
        fnstsw ax
        fstp st(0)
        test ah,41h
        fstp st(0)
        jne door_model_no
        mov result,1
        jmp door_model_done
    door_model_empty:
        fstp st(0)
        fstp st(0)
        fstp st(0)
        mov result,1
        jmp door_model_done
    door_model_no:
        mov result,0
    door_model_done:
    }
    return result!=0;
}
inline bool fallback_target(const V* previous,const float* rounded_squared,U previous_id){
    if(previous_id==0x7f000000u)return true;
    unsigned short status;
    __asm {mov ecx,previous
        fld dword ptr[ecx+8]
        fmul dword ptr[ecx+8]
        fld dword ptr[ecx+4]
        fmul dword ptr[ecx+4]
        faddp st(1),st(0)
        fld dword ptr[ecx]
        fmul dword ptr[ecx]
        faddp st(1),st(0)
        mov edx,rounded_squared
        fcomp dword ptr[edx]
        fnstsw ax
        mov status,ax
    }
    return (status&0x4100u)==0;
}
}
