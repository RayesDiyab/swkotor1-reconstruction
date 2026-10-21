// CSWCObject VFX list Add/Delete/Has. Canonical storage/providers stay owned.
#include "ab1862128b-object_vfx_list.h"
#include "e798fd6ecb-object_vfx_initializer.h"
#include <cstdint>
#include <cstring>
using U=std::uint32_t;
using H=std::uint16_t;
using B=std::uint8_t;
namespace kotor_list { struct Internal; }
extern "C" {
void* __cdecl live_operator_new(U);
void __cdecl live_free(void*);
U __fastcall unresolved_list_next_005e9b90(void*,void*,void**);
U __fastcall kotor_fastcall_005e9bd0(void*,void*,void*);
void __fastcall resource_append_005e9dc0(kotor_list::Internal*,void*,void*);
B __fastcall kotor_fastcall_006a0920(void*,void*);
void __fastcall object_vfx_0063dd50(void*,void*,void*);
void __fastcall object_vfx_006a6590(void*,void*,U);
void __fastcall object_vfx_006a1810(void*,void*);
void __fastcall object_vfx_006a5fc0(void*,void*);
U __fastcall object_vfx_006a5a40(void*,void*,U);
void __fastcall object_vfx_006a0c70(void*,void*);
}
namespace {
template<class T>T read(const void*p,U o){T v;std::memcpy(&v,static_cast<const B*>(p)+o,sizeof(v));return v;}
void* ptr(const void*p,U o){return read<void*>(p,o);}
template<class F>F slot(void*p,U o){return read<F>(ptr(p,0),o);}
template<class F,class...A>auto invoke(F f,A...a){F volatile target=f;return target(a...);}
using Has=U(__fastcall*)(void*,void*,U);
using Activate=void(__fastcall*)(void*,void*,U,U);
// Original Add unwind state0 protects only operator-new storage while the
// constructor runs. Successful constructor return disarms it, even if later
// Load/List/Show calls throw: do not invent whole-effect exception cleanup.
struct ConstructorAllocation {
    void* allocation;
    ~ConstructorAllocation(){if(allocation)live_free(allocation);}
    void release(){allocation=nullptr;}
};
}
extern "C" {
U __fastcall object_vfx_0063f400(void*self,void*,U effect_word){
    void* list=ptr(self,0x6c);
    void* position=ptr(list,0);
    void* head=ptr(list,0); // original second head read precedes first payload read
    void* effect=head?ptr(head,8):nullptr;
    while(position){
        if(read<H>(effect,0xc4)==static_cast<H>(effect_word)&&!read<U>(effect,0xc0)){
            invoke(object_vfx_0063dd50,self,nullptr,effect);
            invoke(object_vfx_006a6590,effect,nullptr,U(1));
            auto activate=slot<Activate>(self,0xdc); // cache before class getter
            U effect_class=invoke(kotor_fastcall_006a0920,effect,nullptr);
            activate(self,nullptr,effect_class,0);
            return 1;
        }
        effect=reinterpret_cast<void*>(static_cast<std::uintptr_t>(invoke(unresolved_list_next_005e9b90,ptr(self,0x6c),nullptr,&position)));
    }
    return 0;
}
U __fastcall object_vfx_0063ddf0(void*self,void*,U effect_word){
    void* list=ptr(self,0x6c);
    void* position=ptr(list,4);
    void* tail=ptr(list,4);
    void* effect=tail?ptr(tail,8):nullptr;
    while(position){
        // Original zero-extends the WORD stored on the effect and compares it
        // to the entire DWORD query. Unlike Delete, do not truncate the query.
        if(static_cast<U>(read<H>(effect,0xc4))==effect_word)return 1;
        effect=reinterpret_cast<void*>(static_cast<std::uintptr_t>(invoke(kotor_fastcall_005e9bd0,ptr(self,0x6c),nullptr,&position)));
    }
    return 0;
}
U __fastcall object_vfx_0063d970(void*self,void*,U effect_word,U arg1,U arg2,
    U arg3,U arg4,U arg5,const ObjectVfxPosition* position_arg){
    if(static_cast<H>(effect_word)==0x3fc && slot<Has>(self,0xcc)(self,nullptr,0x3fc))return 1;
    void* allocation=invoke(live_operator_new,U(0xf8));
    void* effect=nullptr;
    {
        ConstructorAllocation constructor{allocation};
        if(allocation)effect=invoke(object_vfx_006a09e0,allocation,nullptr);
        constructor.release();
    }
    if(!effect)return 0;
    const ObjectVfxPosition position=read<ObjectVfxPosition>(position_arg,0);
    if(!invoke(object_vfx_006a1880,effect,nullptr,effect_word,arg1,read<U>(self,4),arg2,arg3,arg4,arg5,position)){
        invoke(object_vfx_006a1810,effect,nullptr);
        invoke(live_free,effect);
        return 0;
    }
    invoke(resource_append_005e9dc0,reinterpret_cast<kotor_list::Internal*>(ptr(self,0x6c)),nullptr,effect);
    invoke(object_vfx_006a5fc0,effect,nullptr);
    (void)invoke(object_vfx_006a5a40,effect,nullptr,U(0));
    invoke(object_vfx_006a0c70,effect,nullptr);
    auto activate=slot<Activate>(self,0xdc); // original caches vtable before getter
    U effect_class=invoke(kotor_fastcall_006a0920,effect,nullptr);
    activate(self,nullptr,effect_class,static_cast<U>(reinterpret_cast<std::uintptr_t>(effect)));
    return 1;
}
}
