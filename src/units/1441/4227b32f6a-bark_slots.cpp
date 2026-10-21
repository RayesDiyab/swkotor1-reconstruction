#include "e7bb1dbf1b-object_projectile_bark.h"
#include <cstdint>
#include <cstring>
using U=std::uint32_t;
using B=std::uint8_t;
namespace gui_load { struct Object; struct Extent; struct Panel; }
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
extern "C" {
void __fastcall gui_load_panel_set_extent_0040a5a0(gui_load::Object*,void*,const gui_load::Extent*);
void __fastcall gui_load_panel_on_added_0040b870(gui_load::Panel*,void*);
std::uint64_t __fastcall kotor_fastcall_004adc70(const void*,void*);
}
namespace {
template<class T>T rd(const void*p,U o){T v;std::memcpy(&v,static_cast<const B*>(p)+o,sizeof(v));return v;}
template<class T>void wr(void*p,U o,T v){std::memcpy(static_cast<B*>(p)+o,&v,sizeof(v));}
void* ptr(const void*p,U o){return rd<void*>(p,o);}
template<class F>F slot(void*p,U o){return rd<F>(ptr(p,0),o);}
}
extern "C" {
// Original006A9FD0,74bytes. Keep the existing W4 table's DWORD pointer transport.
void __fastcall kotor_fastcall_006a9fd0(void*self,void*,U extent_word){
    const void*extent=reinterpret_cast<const void*>(static_cast<std::uintptr_t>(extent_word));
    U child_extent[4]={5,5,rd<U>(extent,8)-10u,rd<U>(extent,0xc)-10u};
    void*child=static_cast<B*>(self)+0x64;
    slot<decltype(&gui_load_panel_set_extent_0040a5a0)>(child,4)(
        static_cast<gui_load::Object*>(child),nullptr,reinterpret_cast<const gui_load::Extent*>(child_extent));
    gui_load_panel_set_extent_0040a5a0(static_cast<gui_load::Object*>(self),nullptr,
                                     reinterpret_cast<const gui_load::Extent*>(extent));
}
// Original006A9F90,50bytes through its tailJMP0040B870.
void __fastcall kotor_fastcall_006a9f90(void*self,void*){
    void*client=ptr(kotor_menu_window_cells::manager_007a39fc,4);
    void*timer=ptr(ptr(client,4),0x2c);
    std::uint64_t ticks=kotor_fastcall_004adc70(timer,nullptr);
    std::int64_t signed_ticks;
    std::memcpy(&signed_ticks,&ticks,sizeof(signed_ticks));
    // Original006FC660 is signed64 divide; constant denominator cannot overflow.
    wr<U>(self,0x1bc,static_cast<U>(signed_ticks/1000));
    gui_load_panel_on_added_0040b870(static_cast<gui_load::Panel*>(self),nullptr);
}
}
