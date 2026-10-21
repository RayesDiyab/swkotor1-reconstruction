#include "59f0da86f5-reporter_strings.h"
#include <cstdint>

namespace {
using U32=std::uint32_t;
static_assert(sizeof(void*)==4,"Original x86 access contract");
U32 load32(U32 address) {
    U32 value;
    __asm { mov eax,address
            mov eax,[eax]
            mov value,eax }
    return value;
}
void store32(U32 address,U32 value) {
    __asm { mov eax,address
            mov edx,value
            mov [eax],edx }
}
unsigned char load8(U32 address) { return *reinterpret_cast<volatile const unsigned char*>(address); }
void store8(U32 address,unsigned char value) { *reinterpret_cast<volatile unsigned char*>(address)=value; }
U32 zero_candidate(U32 word) { return (~word^(word+0x7efefeffu))&0x81010100u; }
void zero_tail(U32 out,U32 count) {
    while(count && (out&3u)) { store8(out++,0);--count; }
    while(count>=4) { store32(out,0);out+=4;count-=4; }
    while(count) { store8(out++,0);--count; }
}
}

namespace kotor_runtime_reporter_dependencies {
char* __cdecl string_concat_00705920(char* destination,const char* source) {
    U32 cursor=reinterpret_cast<U32>(destination),end=0;
    bool found=false;
    while(cursor&3u) {
        const auto byte=load8(cursor++);
        if(!byte) { end=cursor-1;found=true;break; }
    }
    while(!found) {
        const U32 first=load32(cursor);
        const U32 candidate=zero_candidate(first);
        cursor+=4;
        if(!candidate)continue;
        // The strcat scan rereads only a candidate word, unlike the copy tail.
        const U32 word=load32(cursor-4);
        for(unsigned lane=0;lane<4;++lane) {
            if(((word>>(lane*8))&0xffu)==0) {end=cursor-4+lane;found=true;break;}
        }
    }
    auto* append=reinterpret_cast<char*>(end);
    //00705973/78/7D/82 join the shared copy tail00705985. Reuse the owner's
    // exact full strcpy entry for that copy, then return the original base.
    reconstructed_strcpy_00705910(append,source);
    return destination;
}

static char* __cdecl copy_n_body(char* destination,const char* source,std::size_t count) {
    U32 in=reinterpret_cast<U32>(source),out=reinterpret_cast<U32>(destination);
    U32 remaining=static_cast<U32>(count);
    if(!remaining)return destination;
    while(in&3u) {
        const auto byte=load8(in++);store8(out++,byte);--remaining;
        if(!remaining)return destination;
        if(!byte) {zero_tail(out,remaining);return destination;}
    }
    while(remaining>=4) {
        const U32 first=load32(in);
        const U32 candidate=zero_candidate(first);
        U32 word=load32(in); // Original rereads every aligned source DWORD.
        in+=4;
        bool terminated=false;
        if(candidate) {
            for(unsigned lane=0;lane<4;++lane) {
                if(((word>>(lane*8))&0xffu)==0) {
                    // Zero from the first NUL through the entire copied DWORD.
                    word=lane==0?0:word&((1u<<(lane*8))-1u);
                    terminated=true;break;
                }
            }
        }
        store32(out,word);out+=4;remaining-=4;
        if(terminated) {
            // Unlike unaligned-byte termination, the original keeps DWORD
            // stores unaligned here; do not realign this padding phase.
            while(remaining>=4) {store32(out,0);out+=4;remaining-=4;}
            while(remaining) {store8(out++,0);--remaining;}
            return destination;
        }
    }
    while(remaining) {
        const auto byte=load8(in++);store8(out++,byte);--remaining;
        if(!byte) {
            while(remaining) {store8(out++,0);--remaining;}
            return destination;
        }
    }
    return destination;
}
// The compiler may use copy_n_body's private argument copies as scratch.
// This entry itself only reads the original three incoming slots (RS-1).
__declspec(naked) char* __cdecl string_copy_n_006fab80(char*,const char*,std::size_t) {
    __asm {
        push dword ptr [esp+12]
        push dword ptr [esp+12]
        push dword ptr [esp+12]
        call copy_n_body
        add esp,12
        ret
    }
}
}
