#include "e4edc3af3f-object_vfx_area.h"
#include <cstdint>
#include <cstring>
using U=std::uint32_t;
using B=std::uint8_t;
namespace kotor_menu_window_cells{extern void*volatile manager_007a39fc;}
extern "C" {
// Unique writable initialized.data owners, authorized by root. Initializers
// encode original3EB33333/3F000000; native storage/initializer gate is pending.
float object_vfx_dark_cell_007a22ac=0.35f;
float object_vfx_dark_cell_007a22b0=0.5f;
void* __fastcall client_get_player_creature_005ed540(void*,void*);
U __fastcall unresolved_list_next_005e9b90(void*,void*,void**);
B __fastcall kotor_fastcall_006a0920(void*,void*);
void* __fastcall object_vfx_00614aa0(void*,void*,U);
}
namespace {
template<class T>T rd(const void*p,U off){T v;std::memcpy(&v,static_cast<const B*>(p)+off,sizeof(v));return v;}
void wr(void*p,U off,U v){std::memcpy(static_cast<B*>(p)+off,&v,4);}
void*ptr(const void*p,U off){return rd<void*>(p,off);}
void*client(){return ptr(kotor_menu_window_cells::manager_007a39fc,4);}
// A raw encoded-reference scan does not exclude computed access, so retain the
// writable global cells. Only original rdata literals below are immutable.
const U three=0x40400000;
const std::uint64_t sqrt_three=0x3ffbb645a1cac083ULL;
std::uint16_t compare_square(bool high){
    const float*threshold=high?&object_vfx_dark_cell_007a22b0:&object_vfx_dark_cell_007a22ac;
    std::uint16_t status;
    __asm {
        mov eax, threshold
        fld dword ptr [eax]
        fmul qword ptr sqrt_three
        fld st(0)
        fmul st(1)
        fld dword ptr three
        fcompp
        fnstsw ax
        fstp st(0)
        mov status, ax
    }
    return status;
}
}
extern "C" {
// Original126bytes; RET4/@12, returns model or0. Selected W3 stub was void.
void* __fastcall object_vfx_00614aa0(void*self,void*,U shadowing){
    void*position=ptr(ptr(self,0x6c),0);
    void*effect=position?ptr(position,8):nullptr;
    while(position){
        if(effect&&kotor_fastcall_006a0920(effect,nullptr)==4&&!rd<U>(effect,0xc0)&&rd<U>(effect,0xe0)){
            if(!shadowing||rd<U>(effect,0xd4))return ptr(effect,0x8c);
        }
        effect=reinterpret_cast<void*>(static_cast<std::uintptr_t>(unresolved_list_next_005e9b90(ptr(self,0x6c),nullptr,&position)));
    }
    return nullptr;
}
void __fastcall object_vfx_00605410(void*self,void*){
    if(!ptr(self,0x188))return;
    void*p=client_get_player_creature_005ed540(client(),nullptr);
    if(p)(void)object_vfx_00614aa0(p,nullptr,1);
}
U __fastcall object_vfx_00605440(void*self,void*){
    if(!ptr(self,0x184))return 0;
    void*p=client_get_player_creature_005ed540(client(),nullptr);
    if(p)(void)object_vfx_00614aa0(p,nullptr,0);
    const U masked=compare_square(false)&0x4100u;
    // TEST AH,41h followed by JP: even parity (00 or41) takes second arm.
    if(masked==0x0100u||masked==0x4000u){wr(self,0x234,1);return 1;}
    if(!(compare_square(true)&0x0100u)){wr(self,0x234,0);return 0;}
    const U retained=rd<U>(self,0x234);wr(self,0x234,retained);return retained;
}
}
