#include "d828e87991-object_vfx_core.h"
#include <cstdint>
#include <cstring>
using U=std::uint32_t;using B=std::uint8_t;using H=std::uint16_t;
static_assert(sizeof(void*)==4,"original x86 pointer carrier");
namespace kotor_menu_window_cells{extern void*volatile manager_007a39fc;}
extern "C"{
void* __fastcall client_get_player_creature_005ed540(void*,void*);
void* __fastcall kotor_fastcall_005ed580(void*,void*,U);
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void*,void*);
U __fastcall kotor_fastcall_005b4880(void*,void*);
void __cdecl object_vfx_lower_0044f130(float); // unique sol_vfx_render provider
}
namespace{
template<class T>T rd(const void*p,U o){T v;std::memcpy(&v,static_cast<const B*>(p)+o,sizeof(v));return v;}
void*ptr(const void*p,U o){return rd<void*>(p,o);}
using Getter=void*(__fastcall*)(void*,void*);
Getter slot(void*p,U o){return rd<Getter>(ptr(p,0),o);}
void*client(){return ptr(kotor_menu_window_cells::manager_007a39fc,4);}
void speed_ratio(void*player){
    const B*p=static_cast<const B*>(player);void*stats=ptr(player,0x21c);
    const U one=0x3f800000,multiplier=0x3f400000;U bits;
    __asm{
        mov ecx,p
        fld dword ptr [ecx+3b0h]
        mov ecx,stats
        fdiv dword ptr [ecx+5ch]
        fcom dword ptr one
        fnstsw ax
        test ah,41h
        jne keep_ratio
        fstp st(0)
        fld dword ptr one
    keep_ratio:
        fmul dword ptr multiplier
        fstp dword ptr bits
        push bits
        call object_vfx_lower_0044f130
        add esp,4
    }
}
}
extern "C" void __fastcall object_vfx_006a5920(void*self,void*,U){
    void*player=client_get_player_creature_005ed540(client(),nullptr);if(!player)return;
    // Original PUSH ECX scratch initializes full selector with this pointer;
    // phase arms overwrite only its low WORD, preserving the high16 carrier.
    U kind=static_cast<U>(reinterpret_cast<std::uintptr_t>(self));
    const B phase=rd<B>(self,0xc6);
    if(phase==1)kind=(kind&0xffff0000u)|rd<H>(self,0xa0);
    else if(phase==3)kind=(kind&0xffff0000u)|rd<H>(self,0xa4);
    else if(phase==2)kind=(kind&0xffff0000u)|rd<H>(self,0xa2);
    void*o=kotor_fastcall_005ed580(client(),nullptr,rd<U>(self,0x40));
    if(o){void*view=slot(o,0x0c)(o,nullptr);
        if(view){
            const bool other=rd<U>(view,4)!=rd<U>(player,4);
            void*creature=slot(view,0x2c)(view,nullptr); // also called for other IDs
            bool remove=other;
            if(!other&&creature){
                void*server=kotor_m15a_creature_get_server_creature_0060fb20(creature,nullptr);
                if(server&&kotor_fastcall_005b4880(server,nullptr)&&rd<U>(view,4)==rd<U>(player,4))remove=true;
            }
            if(remove){object_vfx_006a56f0(self,nullptr,kind);return;}
        }
    }
    object_vfx_006a54d0(self,nullptr,kind);
    speed_ratio(player);
}
