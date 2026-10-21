#include "654318f17e-memmove.h"
extern "C" __declspec(noinline) void* __cdecl reconstructed_memmove(void* destination,const void* source,uint32_t count) {
    if(count==0) return destination;
    // Integer comparisons avoid unrelated-object pointer ordering in C++.
    const uintptr_t d=reinterpret_cast<uintptr_t>(destination);
    const uintptr_t s=reinterpret_cast<uintptr_t>(source);
    auto* out=static_cast<volatile unsigned char*>(destination);
    auto* in=static_cast<const volatile unsigned char*>(source);
    // Volatile byte accesses keep this body independent of compiler CRT copy substitutions.
    if(d>s && d<s+count) {
        for(uint32_t i=count;i!=0;--i) out[i-1]=in[i-1];
    } else {
        for(uint32_t i=0;i<count;++i) out[i]=in[i];
    }
    return destination;
}
