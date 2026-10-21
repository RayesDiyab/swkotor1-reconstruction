// Missing dependency bodies of 00611F50. No original instructions are executed.
#include "939845f340-creature_methods.h"
#include "24fdd5d329-creature_x87.h"
#include <cstring>
namespace kotor_scene3d { struct Vector; }
extern "C" double __cdecl vector_magnitude_004ab100(const kotor_scene3d::Vector*);
namespace {
using U=std::uint32_t;
template<class T>T get(const void* p,U o){T v;std::memcpy(&v,static_cast<const unsigned char*>(p)+o,sizeof v);return v;}
void* ptr(const void* p,U o){return get<void*>(p,o);}
const KotorM15eAnimBaseVtbl* vt(void* p){return get<const KotorM15eAnimBaseVtbl*>(p,0);}
U animation_flag(void* self,U animation,const U* column){
    std::int32_t value=0; // 0063C246/0063C2A6 initialize the actual output slot.
    if(void* model=ptr(self,0x68)) animation=vt(model)->s0e0(model,nullptr,animation);
    void* table=ptr(ptr(kotor_cell_rules_007a3a28,0xB8),0x3C);
    if(c2da_current_get_int_entry_00413510(static_cast<c2da_current::Table*>(table),nullptr,static_cast<std::uint16_t>(animation),static_cast<std::int32_t>(*column),&value)==0) return 0;
    return value!=0?1u:0u;
}
}
extern "C" U __fastcall recovered_object_animation_walk_0063c240(void* self,void*,U animation){return animation_flag(self,animation,&kotor_rules_cell32_007a2160);}
extern "C" U __fastcall recovered_object_animation_run_0063c2a0(void* self,void*,U animation){return animation_flag(self,animation,&kotor_rules_cell32_007a2164);}

// Reuse of W3 t3v/src/w3_client_t3_update.cpp::006975D0, with the two original-fidelity
// corrections: PUSH ECX initializes the float output to pointer bits; ST0 stays wide.
extern "C" double __fastcall recovered_client_appearance_get_run_looks_awful_rate_006975d0(void* appearance,void*) {
    const U seed=static_cast<U>(reinterpret_cast<std::uintptr_t>(appearance));
    float value;std::memcpy(&value,&seed,4);
    void* table=ptr(ptr(kotor_cell_rules_007a3a28,0xB8),0x0C);
    (void)kotor_m15b_c2da_get_float_entry_00413350(table,nullptr,get<std::uint16_t>(appearance,0x18),static_cast<std::int32_t>(kotor_rules_cell32_007a2120),&value);
    const float thousand=1000.0f,awful=0.3700000047683716f;
    double result;
    __asm { fld value
        fmul thousand
        fmul awful
        fstp result
    }
    return result;
}

extern "C" double __fastcall recovered_creature_compute_movement_scale_0060bf60(void* self,void*,float rate) {
    U waypoint=get<U>(self,0x1C0);
    if(waypoint==0) return 0.0;
    const float ramp_ms=get<U>(self,0x1F4)==1?500.0f:1000.0f;
    float distance=0.0f;
    float previous[3];std::memcpy(previous,static_cast<unsigned char*>(self)+0x1DC,12);
    do {
        const void* point=reinterpret_cast<const void*>(static_cast<std::uintptr_t>(waypoint));
        float next[3];std::memcpy(next,point,12);
        float difference[3]={creature_x87::difference(next[0],previous[0]),
                             creature_x87::difference(next[1],previous[1]),0.0f};
        const double length=vector_magnitude_004ab100(reinterpret_cast<const kotor_scene3d::Vector*>(difference));
        distance=creature_x87::add_length(length,distance);
        const U end=get<U>(self,0x1BC)+static_cast<U>(get<std::uint16_t>(self,0x1EC))*12u;
        waypoint+=12u;
        std::memcpy(previous,next,12);
        if(waypoint>=end) break; // 0060C029 CMP / JB, unsigned raw addresses.
    } while(true);
    float old;std::memcpy(&old,static_cast<unsigned char*>(self)+0x1F8,4);
    const U delta=get<U>(self,0x44);
    const float thousand=1000.0f,stopping_scale=4.999999987376214e-7f;
    const float one=1.0f,hundredth=0.009999999776482582f,tenth=0.10000000149011612f;
    const float millisecond=0.0010000000474974513f,unsigned_bias=4294967296.0f;
    float temporary;double result;
    // 0060C035..0060C0F2, preserving every branch and memory width.
    __asm {
        fld rate
        fmul thousand
        fmul stopping_scale
        fstp temporary
        fld one
        fld distance
        fcomp temporary
        fnstsw ax
        test ah,41h
        jp movement_ramp
        fstp st(0)
        fld distance
        fdiv temporary
        fcom hundredth
        fnstsw ax
        test ah,1
        jne movement_tenth
        fsqrt
        jmp movement_subtract
    movement_tenth:
        fstp st(0)
        fld tenth
    movement_subtract:
        fild delta
        test delta,80000000h
        jz movement_signed_delta
        fadd unsigned_bias
    movement_signed_delta:
        fmul millisecond
        fsubp st(1),st(0)
    movement_ramp:
        fld old
        fcomp one
        fnstsw ax
        test ah,41h
        jp movement_clamp
        fild delta
        test delta,80000000h
        jz movement_signed_ramp
        fadd unsigned_bias
    movement_signed_ramp:
        fdiv ramp_ms
        fadd old
        fstp temporary
        fcom temporary
        fnstsw ax
        test ah,1
        jne movement_clamp
        fstp st(0)
        fld temporary
    movement_clamp:
        fcom tenth
        fnstsw ax
        test ah,41h
        jp movement_return
        fstp st(0)
        fld tenth
    movement_return:
        fstp result
    }
    return result;
}
