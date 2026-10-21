#include "e8a5e6befc-rand_constructor.h"
#include "bce7a91e4c-rand_getstring.h"
#include "5c06c8e15d-source_substring.h"
#include "51b0d83089-native_strings.h"
#include "dcaf3f57f3-allocator_live.h"
#include <cstring>
#include <new>
namespace startup_rand {
using String=kotor_reconstruction::StartupString;
// Support-only C++ lifetime bridge. Loads actual backing into initialized words
// without typed C++ reads of indeterminate fields. Fresh exclusive writable
// storage and no concurrent/aliased observer are required. The eight later
// same-bit assignments establish typed inputs for unchanged Task17 Rand.
static __declspec(naked) void __fastcall CaptureUnseededWords(const void*,std::uint32_t*) {
    __asm {
        mov eax,[ecx+10h]
        mov [edx],eax
        mov eax,[ecx+14h]
        mov [edx+4],eax
        mov eax,[ecx+18h]
        mov [edx+8],eax
        mov eax,[ecx+1ch]
        mov [edx+0ch],eax
        mov eax,[ecx+20h]
        mov [edx+10h],eax
        mov eax,[ecx+24h]
        mov [edx+14h],eax
        mov eax,[ecx+28h]
        mov [edx+18h],eax
        mov eax,[ecx+2ch]
        mov [edx+1ch],eax
        ret
    }
}
// Original constructor ignores BOOL and reads all8 physical counter bytes.
// This raw source-only observation preserves no-write/partial-write failure
// backing without undefined typed C++ reads or an invented zero default.
static __declspec(naked) std::uint64_t __cdecl ObserveFinalCounter() {
    __asm {
        push ebp
        mov ebp,esp
        and esp,0fffffff8h
        sub esp,8
        lea eax,[esp]
        push eax
        call dword ptr [rand_imports]
        mov eax,[esp]
        mov edx,[esp+4]
        mov esp,ebp
        pop ebp
        ret
    }
}
Internal* __fastcall ConstructRandInternal005ed0e0(Internal* storage,void*) {
    std::uint32_t backing[8]={};
    CaptureUnseededWords(storage,backing);
    auto* self=::new(static_cast<void*>(storage)) Internal;
    for(unsigned i=0;i<4;++i){self->masks[i]=backing[i];self->right_shifts[i]=backing[i+4];}
    // The bridge adds same-bit lifetime writes, not original instruction parity.
    // All unknown bytes and6A/6B remain unchanged; no guessed seed values.
    String generated;
    self->seeded=0;
    self->state_vars[0]=0x1745d12d;self->state_vars[1]=0x249248ed;
    self->state_vars[2]=0x333332f9;self->state_vars[3]=0x383ef9fa;
    self->total_shifts[0]=31;self->total_shifts[1]=29;self->total_shifts[2]=28;self->total_shifts[3]=25;
    self->left_shifts[0]=6;self->left_shifts[1]=2;self->left_shifts[2]=13;self->left_shifts[3]=3;
    self->additional_left_shifts[0]=20;self->additional_left_shifts[1]=12;self->additional_left_shifts[2]=7;self->additional_left_shifts[3]=15;
    self->pool_position=0;
    self->random_pool=static_cast<unsigned char*>(live_operator_new(128));
    rand_imports.gen_seed(self,nullptr);
    alignas(String) unsigned char output_storage[8];
    String* output=GetString005ecea0(self,nullptr,reinterpret_cast<String*>(output_storage),16);
    generated.Assign005e5c50(output);
    output->~String();
    std::uint32_t seeds[4];
    for(unsigned i=0;i<4;++i){
        alignas(String) unsigned char slice_storage[8];
        String* slice=SubString005e6270(&generated,nullptr,reinterpret_cast<String*>(slice_storage),static_cast<std::int32_t>(i*4),4);
        const char* text=WindowCStr005e5670(slice,nullptr);
        std::memcpy(&seeds[i],text,4);
        slice->~String();
    }
    self->ReSeed(seeds[0],seeds[1],seeds[2],seeds[3]);
    const std::uint64_t counter=ObserveFinalCounter();
    for(unsigned i=0;i<4;++i)std::memcpy(self->untouched_tail+2+i*8,&counter,8);
    const DWORD ticks=rand_imports.tick_count();
    std::memcpy(self->untouched_tail+34,&ticks,4);
    return self; // generated actual String is destroyed before entry returns
}
}
