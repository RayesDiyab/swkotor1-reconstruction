// Seven bounded missing thunks/leaves. Canonical animation/render storage reused.
#include "f904d42941-resources.h"
#include <cstdint>
#include <cstring>
using U=std::uint32_t;
using Ref=menu_resources::Ref;
extern "C" {
extern volatile std::uint8_t fb_speed_blur_007a6880;
extern volatile float fb_speed_blur_ratio_0078d43c;
}
namespace {
template<class T> T read(const void* p,U o) { T v;std::memcpy(&v,static_cast<const unsigned char*>(p)+o,sizeof v);return v; }
template<class F> F slot(void* p,U o) {return read<F>(read<void*>(p,0),o);}
using Getter=U (__fastcall*)(void*,void*);
using Bumped=U (__fastcall*)(void*,void*,const void*,float);
}
extern "C" {
// 0063D440 RET14h: original reference is passed as16-byte value, never pointer.
U __fastcall object_vfx_lower_0063d440(void* self,void*,Ref name,float thickness) {
    void* anim=read<void*>(self,0x68);if(!anim)return 0;
    return slot<Bumped>(anim,0x28)(anim,nullptr,&name,thickness);
}
U __fastcall object_vfx_lower_0063d460(void* self,void*) {
    void* anim=read<void*>(self,0x68);return anim?slot<Getter>(anim,0x2c)(anim,nullptr):0;
}
U __fastcall object_vfx_lower_0063ce10(void* self,void*) {
    void* anim=read<void*>(self,0x68);return anim?slot<Getter>(anim,0xa0)(anim,nullptr):0;
}
U __fastcall object_vfx_lower_0063ce30(void* self,void*) {
    void* anim=read<void*>(self,0x68);return anim?slot<Getter>(anim,0x24)(anim,nullptr):0;
}
void __cdecl object_vfx_lower_0044f0a0() {fb_speed_blur_007a6880=1;}
void __cdecl object_vfx_lower_0044f0b0() {fb_speed_blur_007a6880=0;}
// MOV-based bit transfer, including NaN payloads; no float load/round operation.
void __cdecl object_vfx_lower_0044f130(float ratio) {
    __asm {
        mov eax,ratio
        mov dword ptr fb_speed_blur_ratio_0078d43c,eax
    }
}
}
