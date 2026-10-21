// Draft only: raw asin/provider protocol and storage gates remain open.
#include "ef0f364c1a-pointto.hpp"
#include "901ebaee8b-scene3d.h"
#include <cstddef>
#include <cstring>
#if !defined(_M_IX86)
#error "PointTo requires x86"
#endif
extern "C" {
using Vector=kotor_scene3d::Vector;
using Quaternion=kotor_scene3d::Quaternion;
Vector* __cdecl normalize_004ab180(Vector*,const Vector*);
Vector* __cdecl vector_cross_004ab210(Vector*,const Vector*,const Vector*);
double __cdecl vector_magnitude_004ab100(const Vector*);
Quaternion* __fastcall quaternion_construct_004ac960(Quaternion*,void*,Vector,float);
extern Vector x_axis_007a1998;
// ASM-only boundary, proposed pending independent raw-ST0/CW/storage gate.
// Input ST0, return ST0; no ordinary C++ zero-argument invocation.
void __cdecl pointto_fasin_006fcff0();
}
namespace kotor_pointto_cells {
// Original .data virtual zero tail [00831FB4,00831FD0), sole PointTo owner.
// Keep shared mutable storage and first-use guard bits; no thread-local cache.
struct State {Vector cross;Vector normalized;volatile kotor_scene3d::U guard;};
static_assert(sizeof(State)==28&&offsetof(State,normalized)==12&&offsetof(State,guard)==24,"original contiguous28-byte mutable state");
State state_00831fb4{};
}
namespace {
using U=kotor_scene3d::U;
using namespace kotor_pointto_cells;
constexpr U zero_0073d700=0u,one_0073d764=0x3f800000u;
constexpr U minus_one_00740cbc=0xbf800000u,pi_00741bbc=0x40490fdbu;
struct Frame {float angle;Vector axis;Vector scratch;};
static_assert(sizeof(Frame)==0x1c&&offsetof(Frame,axis)==4&&offsetof(Frame,scratch)==0x10,"original28-byte locals and exact12-byte callee outputs");
void copy_vector_bits(Vector* out,const Vector* in){
    std::memcpy(&out->x,&in->x,4);std::memcpy(&out->y,&in->y,4);std::memcpy(&out->z,&in->z,4);
}
void word(float* out,U value){std::memcpy(out,&value,4);}
bool magnitude_not_greater_one(float* angle){
    const Vector* v=&state_00831fb4.cross;unsigned char result;
    __asm {
        push v
        call vector_magnitude_004ab100
        add esp,4
        mov ecx,angle
        fst dword ptr [ecx]
        fcomp dword ptr one_0073d764
        fnstsw ax
        test ah,41h
        setnz result
    }
    return result!=0;
}
bool angle_less_minus_one(const float* angle){
    unsigned char result;
    __asm {
        mov ecx,angle
        fld dword ptr [ecx]
        fcomp dword ptr minus_one_00740cbc
        fnstsw ax
        test ah,5
        setnp result
    }
    return result!=0;
}
bool angle_not_equal_zero(const float* angle){
    unsigned char result;
    __asm {
        mov ecx,angle
        fld dword ptr [ecx]
        fcomp dword ptr zero_0073d700
        fnstsw ax
        test ah,44h
        setp result
    }
    return result!=0;
}
bool sum_less_zero(){
    const Vector* v=&state_00831fb4.normalized;unsigned char result;
    __asm {
        mov ecx,v
        fld dword ptr [ecx+4]
        fadd dword ptr [ecx]
        fmul dword ptr zero_0073d700
        fadd dword ptr [ecx+8]
        fcomp dword ptr zero_0073d700
        fnstsw ax
        test ah,5
        setnp result
    }
    return result!=0;
}
bool z_greater_zero(){
    const Vector* v=&state_00831fb4.normalized;unsigned char result;
    __asm {
        mov ecx,v
        fld dword ptr [ecx+8]
        fcomp dword ptr zero_0073d700
        fnstsw ax
        test ah,41h
        setz result
    }
    return result!=0;
}
void asin_and_store(float* angle){
    __asm {
        mov ecx,angle
        fld dword ptr [ecx]
        call pointto_fasin_006fcff0
        mov ecx,angle
        fstp dword ptr [ecx]
    }
}
void subtract_from_pi(float* angle){
    __asm {
        mov ecx,angle
        fld dword ptr pi_00741bbc
        fsub dword ptr [ecx]
        fstp dword ptr [ecx]
    }
}
}
extern "C" void* __cdecl pointto_004acca0(void* output,const void* direction){
    U guard=state_00831fb4.guard;
    if((guard&1u)==0){guard|=1u;state_00831fb4.guard=guard;state_00831fb4.normalized={0.0f,0.0f,0.0f};}
    if((guard&2u)==0){guard|=2u;state_00831fb4.guard=guard;state_00831fb4.cross={0.0f,0.0f,0.0f};}
    Frame f;
    const Vector* normalized=normalize_004ab180(&f.axis,static_cast<const Vector*>(direction));
    copy_vector_bits(&state_00831fb4.normalized,normalized);
    f.axis={0.0f,0.0f,1.0f};
    const Vector* crossed=vector_cross_004ab210(&f.scratch,&f.axis,&state_00831fb4.normalized);
    copy_vector_bits(&state_00831fb4.cross,crossed);
    bool use_asin=true;
    if(!magnitude_not_greater_one(&f.angle))word(&f.angle,one_0073d764);
    else if(angle_less_minus_one(&f.angle))word(&f.angle,minus_one_00740cbc);
    else if(!angle_not_equal_zero(&f.angle))use_asin=false;
    if(use_asin){
        asin_and_store(&f.angle);
        if(sum_less_zero())subtract_from_pi(&f.angle);
        const Vector* axis=normalize_004ab180(&f.scratch,&state_00831fb4.cross);
        const Vector by_value=*axis;
        quaternion_construct_004ac960(static_cast<Quaternion*>(output),nullptr,by_value,f.angle);
    }else{
        word(&f.angle,z_greater_zero()?zero_0073d700:pi_00741bbc);
        const Vector by_value=x_axis_007a1998;
        quaternion_construct_004ac960(static_cast<Quaternion*>(output),nullptr,by_value,f.angle);
    }
    return output;
}
