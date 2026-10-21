#include "8db4076b44-contracts.h"
#include "3a4300bdd7-allocator_live.h"
static void store(void* p,unsigned offset,std::uint32_t value){
    *reinterpret_cast<std::uint32_t volatile*>(static_cast<unsigned char*>(p)+offset)=value;
}
extern "C" void* __cdecl reconstructed_factory_0044ecd0(){
    const auto& b=*kotor_aurora_factory::views;
    void* p=*b.singleton_007b921c;



    if(!p){
        // Existing task23 malloc body006FA7E6; never a host-CRT substitute.
        p=live_malloc(0xb8);
        if(p){
            store(p,0xa8,0);store(p,0xac,0);store(p,0xb0,0x3f800000u);
            *b.state_007b9220=0;
            store(p,0xb4,0);
            store(p,0,static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(b.vtable_00741518)));



            const auto next=*b.counter_007b9218+1u;

            *b.singleton_007b921c=p;
            *b.counter_007b9218=next;
            return p;
        }



        *b.singleton_007b921c=nullptr;



    }
    *b.counter_007b9218=*b.counter_007b9218+1u;
    return p;
}
