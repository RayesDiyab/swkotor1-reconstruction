#pragma once
#include "177aadaf45-scene3d.h"
#include <cstdint>
namespace m15_dust_numeric {
// Bounded original instruction continuations only; fixed vector/CRT algorithms
// stay in their selected providers. Pointer operands avoid float return loads.
inline void add32(float* destination,const float* input,std::uint32_t constant_bits){
    __asm {
        mov eax,input
        fld dword ptr [eax]
        fadd dword ptr constant_bits
        mov eax,destination
        fstp dword ptr [eax]
    }
}
inline bool refresh_height(const float* height,const float* cached){
    const double threshold=1.0;
    unsigned short status;
    __asm {
        mov eax,height
        fld dword ptr [eax]
        mov eax,cached
        fsub dword ptr [eax]
        fabs
        fcomp qword ptr threshold
        fnstsw status
    }
    return (status&0x4100u)==0;
}
inline bool cross_is_large(const kotor_scene3d::Vector* cross){
    const float threshold=0.00001f;
    unsigned short status;
    __asm {
        mov eax,cross
        fld dword ptr [eax+8]
        fmul dword ptr [eax+8]
        fld dword ptr [eax+4]
        fmul dword ptr [eax+4]
        faddp st(1),st(0)
        fld dword ptr [eax]
        fmul dword ptr [eax]
        faddp st(1),st(0)
        fcomp dword ptr threshold
        fnstsw status
    }
    return (status&0x4100u)==0;
}
inline void random_rate(float* destination,std::int32_t remainder){
    const float hundredth=0.01f;
    __asm {
        fild dword ptr remainder
        fmul dword ptr hundredth
        mov eax,destination
        fstp dword ptr [eax]
    }
}
}
