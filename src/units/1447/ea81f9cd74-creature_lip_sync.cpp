// 00616310: resource-backed viseme setup, with original ownership/unwind behavior.
#include "939845f340-creature_methods.h"
#include "24fdd5d329-creature_x87.h"
#include "51e94608b6-lip_resource.h"
#include "e880d614d4-resources.h"
#include <cstring>
extern "C" {
char* __fastcall c2da_ref_00405fe0(void*,void*) noexcept;
void* __cdecl live_operator_new(std::uint32_t);
void __cdecl live_free(void*);
std::uint32_t __fastcall recovered_bridge_layout_room_count_005a4c50(void*,void*);
void __fastcall recovered_bridge_layout_unload_005de450(void*,void*);
}
namespace {
using U=std::uint32_t;
template<class T>T get(const void* p,U o){T v;std::memcpy(&v,static_cast<const unsigned char*>(p)+o,sizeof v);return v;}
void* ptr(const void* p,U o){return get<void*>(p,o);}
U word(const void* p){return static_cast<U>(reinterpret_cast<std::uintptr_t>(p));}
U bits(float value){U r;std::memcpy(&r,&value,4);return r;}
const KotorM15eAnimBaseVtbl* av(void* p){return get<const KotorM15eAnimBaseVtbl*>(p,0);}
struct DeleteTable {void*(__fastcall* deleting)(void*,void*,U);};
void deleting(void* p){get<const DeleteTable*>(p,0)->deleting(p,nullptr,1);}
float reciprocal(float duration) {const float one=1.0f;float result;
    __asm {fld one
        fdiv duration
        fstp result
    }
    return result;
}
float viseme(U id){const float bias=4294967296.0f,one=1.0f,factor=0.0625f;float result;
    __asm {fild id
        test id,80000000h
        jz viseme_nonnegative
        fadd bias
    viseme_nonnegative:
        fadd one
        fmul factor
        fstp result
    }
    return result;
}
}
extern "C" U __fastcall recovered_creature_lip_sync_00616310(void* self,void*,U reference_word) {
    void* reference=reinterpret_cast<void*>(static_cast<std::uintptr_t>(reference_word));
    kotor_reconstruction::StartupString text(c2da_ref_00405fe0(reference,nullptr));
    void* allocation=live_operator_new(0x30);
    void* lip;
    // Original unwind state1 covers only the unfinished allocation/constructor.
    try {lip=allocation!=nullptr?lip_construct_0070c4d0(allocation,nullptr):nullptr;}
    catch(...) {live_free(allocation);throw;}
    if(lip==nullptr) return 0;
    if(lip_load_0070c590(lip,nullptr,text)==0) {deleting(lip);return 0;}
    const U count=recovered_bridge_layout_room_count_005a4c50(lip,nullptr);
    const float duration=lip_duration_0070c280(lip,nullptr);
    auto* times=static_cast<float*>(live_operator_new(count*4u));
    auto* visemes=static_cast<float*>(live_operator_new(count*4u));
    U handle=0;
    if(static_cast<std::int32_t>(count)>0) {
        const float inverse=reciprocal(duration);
        for(U i=0;static_cast<std::int32_t>(i)<static_cast<std::int32_t>(count);++i) {
            const float time=lip_frame_time_0070c2c0(lip,nullptr,i);
            times[i]=creature_x87::times(time,inverse);
            visemes[i]=viseme(lip_frame_id_0070c290(lip,nullptr,i));
        }
    }
    // The original reads both endpoints even for zero/negative count; preserve
    // its real unchecked memory behavior, without a substitute guard or trap.
    const unsigned first=creature_x87::compare(times[0],0.0f);
    if((first&0x44u)==0x40u) {
        const unsigned upper=creature_x87::compare(times[count-1u],1.0000100135803223f);
        if((upper&1u)!=0) {
            const unsigned lower=creature_x87::compare(times[count-1u],0.9999899864196777f)&0x41u;
            if(lower==0 || lower==0x41u) {
                times[count-1u]=1.0f;
                void* model=ptr(self,0x68);
                (void)av(model)->s05c(model,nullptr,0,0,0,0);
                model=ptr(self,0x68);
                handle=av(model)->s05c(model,nullptr,word(times),word(visemes),count,bits(duration));
            }
        }
    }
    recovered_bridge_layout_unload_005de450(lip,nullptr);
    deleting(lip);
    live_free(times);
    if(visemes!=nullptr) live_free(visemes);
    return handle;
}
