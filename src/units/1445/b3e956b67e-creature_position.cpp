// Original 00611F50, read-only GOG SHA256 in evidence/disassemble.py.
// This unit implements only Position; dependency bodies live in creature_position_lowers.cpp.
#include "939845f340-creature_methods.h"
#include "24fdd5d329-creature_x87.h"
#include <cstring>
namespace {
using U = std::uint32_t;
template<class T> T get(const void* p, U offset) { T v; std::memcpy(&v, static_cast<const unsigned char*>(p)+offset,sizeof v); return v; }
template<class T> void put(void* p, U offset, T v) { std::memcpy(static_cast<unsigned char*>(p)+offset,&v,sizeof v); }
void* ptr(const void* p,U o) { return get<void*>(p,o); }
void* at(void* p,U o) { return static_cast<unsigned char*>(p)+o; }
U bits(float f) { U v;std::memcpy(&v,&f,4);return v; }
const KotorM15eAnimBaseVtbl* av(void* p) { return get<const KotorM15eAnimBaseVtbl*>(p,0); }
const KotorM15ObjectMethods_007516c8* ov(void* p) { return get<const KotorM15ObjectMethods_007516c8*>(p,0); }
void table_rate(void* self,U column,float* out) {
    void* const table=ptr(ptr(kotor_cell_rules_007a3a28,0xB8),0x0C);
    (void)kotor_m15b_c2da_get_float_entry_00413350(table,nullptr,get<std::uint16_t>(ptr(self,0x21C),0x18),static_cast<std::int32_t>(column),out);
}
}
extern "C" void __fastcall recovered_creature_sync_position_00611f50(void* self,void*,const float* position,U animation,float delta) {
    // 00611F60..00611F77 raw three-word copy, followed by the real movement provider.
    put<U>(self,0x1DC,get<U>(position,0));
    put<U>(self,0x1E0,get<U>(position,4));
    put<U>(self,0x1E4,get<U>(position,8));
    kotor_fastcall_0060bdf0(self,nullptr,reinterpret_cast<W4Vector*>(const_cast<float*>(position)));
    float ground[3]={0.0f,0.0f,creature_x87::load(position[2])};
    ov(self)->s10c_set_ground_normal(self,nullptr,ground);
    if(kotor_m15a_object_animation_stationary_0063c170(self,nullptr,animation)!=0) return;
    float table=0.0f; // 00611FBA actually initializes the +10 slot, not the +14 rate slot.
    // The neither-walk-nor-run path consumes an unwritten original frame slot.
    // Keep real retained stack storage and read it only on that path, without
    // inventing a deterministic value or exposing an indeterminate C++ float.
    unsigned char retained_rate[4];
    float rate;
    if(recovered_object_animation_walk_0063c240(self,nullptr,animation)!=0) {
        std::memcpy(&rate,at(self,0x20C),4);
        table_rate(self,kotor_rules_cell32_007a212c,&table);
    } else if(recovered_object_animation_run_0063c2a0(self,nullptr,animation)!=0) {
        std::memcpy(&rate,at(self,0x210),4);
        const double awful=recovered_client_appearance_get_run_looks_awful_rate_006975d0(ptr(self,0x21C),nullptr);
        const U awful_flag=(creature_x87::compare(awful,rate)&0x41u)==0?1u:0u;
        put<U>(gob_get_property_0x68_0063c070(self,nullptr),0xCC,awful_flag);
        table_rate(self,kotor_rules_cell32_007a2120,&table);
    } else {
        U retained;
        __asm {
            lea eax,retained_rate
            mov eax,dword ptr[eax]
            mov retained,eax
        }
        std::memcpy(&rate,&retained,4);
    }
    table=creature_x87::times(table,1000.0f);
    const double scale_wide=recovered_creature_compute_movement_scale_0060bf60(self,nullptr,rate);
    const float scale=creature_x87::store_m32(scale_wide);
    put<float>(self,0x1F8,scale);
    const U current=kotor_m15a_object_get_current_animation_0063c060(self,nullptr);
    float length=kotor_fastcall_0063c0e0(self,nullptr,current);
    float movement=creature_x87::ratio(rate,table,length);
    if((creature_x87::compare(delta,0.5f)&5u)==1u && animation==0x2714u) {
        void* model=ptr(self,0x68);
        length=av(model)->s038(model,nullptr,get<std::uint16_t>(self,0xBA));
        table_rate(self,kotor_rules_cell32_007a212c,&table);
        table=creature_x87::times(table,1000.0f);
        movement=creature_x87::ratio(rate,table,length,true);
        model=ptr(self,0x68);
        const auto* const time_methods=av(model);
        const U actual=kotor_m15a_object_get_current_animation_0063c060(self,nullptr);
        model=ptr(self,0x68);
        rate=time_methods->s03c(model,nullptr,actual);
        if((creature_x87::compare(rate,0.0f)&0x41u)==0) {
            model=ptr(self,0x68);
            const auto* const methods=av(model);
            const U again=kotor_m15a_object_get_current_animation_0063c060(self,nullptr);
            const double denominator=methods->s038(model,nullptr,again);
            const float final_time=creature_x87::special_time(rate,denominator,length);
            model=ptr(self,0x68);
            (void)av(model)->s054(model,nullptr,get<std::uint16_t>(self,0xBA),bits(creature_x87::times(movement,scale)),0,bits(final_time));
        } else {
            model=ptr(self,0x68);
            (void)av(model)->s054(model,nullptr,get<std::uint16_t>(self,0xBA),bits(creature_x87::times(movement,scale)),0,0);
        }
    } else {
        void* model=ptr(self,0x68);
        if(get<U>(model,0xCC)==1) {
            length=av(model)->s038(model,nullptr,get<std::uint16_t>(self,0xBA));
            table_rate(self,kotor_rules_cell32_007a212c,&table);
            table=creature_x87::times(table,1000.0f);
            movement=creature_x87::ratio(rate,table,length,true);
        }
        model=ptr(self,0x68);
        const float elapsed=av(model)->s03c(model,nullptr,get<std::uint16_t>(self,0xBA));
        if((creature_x87::compare(elapsed,0.0f)&0x41u)==0) {
            model=ptr(self,0x68);
            rate=av(model)->s038(model,nullptr,get<std::uint16_t>(self,0xBA));
            model=ptr(self,0x68);
            const auto* const methods=av(model);
            const U again=kotor_m15a_object_get_current_animation_0063c060(self,nullptr);
            const double actual_length=methods->s038(model,nullptr,again);
            const float final_time=creature_x87::normal_time(actual_length,elapsed,rate);
            model=ptr(self,0x68);
            const auto* const setter=av(model);
            const float speed=creature_x87::times(movement,scale);
            const U final_animation=kotor_m15a_object_get_current_animation_0063c060(self,nullptr);
            model=ptr(self,0x68);
            (void)setter->s054(model,nullptr,final_animation,bits(speed),0,bits(final_time));
        } else {
            model=ptr(self,0x68);
            const auto* const setter=av(model);
            const float speed=creature_x87::times(movement,scale);
            const U final_animation=kotor_m15a_object_get_current_animation_0063c060(self,nullptr);
            model=ptr(self,0x68);
            (void)setter->s054(model,nullptr,final_animation,bits(speed),0,0);
        }
    }
    if(void* sound=ptr(self,0x17C)) kotor_m15_sound_source_set_position_005d59e0(sound,nullptr,position,1.5f);
    if(void* sound=ptr(self,0xCC)) kotor_m15_stream_source_set_position_005d62c0(sound,nullptr,position);
    const bool stationary=kotor_m15a_object_animation_stationary_0063c170(self,nullptr,animation)!=0;
    kotor_fastcall_00611850(self,nullptr,static_cast<W4Vector*>(at(self,stationary?0x1D0:0x1C4)));
}
