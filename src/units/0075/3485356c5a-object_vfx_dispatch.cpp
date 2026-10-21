#include "244de166b8-object_vfx_core.h"
#include "04f793c7f8-object_vfx_handlers.h"
#include <cstdint>
#include <cstring>
using U=std::uint32_t;
using H=std::uint16_t;
namespace {
U rd(void*p,U off){U v;std::memcpy(&v,static_cast<unsigned char*>(p)+off,4);return v;}
void wr(void*p,U off,U v){std::memcpy(static_cast<unsigned char*>(p)+off,&v,4);}
}
extern "C" {
void __fastcall object_vfx_006a54d0(void*self,void*,U kind){
    if(!rd(self,0xa8)||rd(self,0xf4))return;
    const H n=static_cast<H>(kind);
    if(n<200)object_vfx_006a3660(self,nullptr,kind);
    else if(n<300)object_vfx_006a3740(self,nullptr,kind);
    else if(n<400)object_vfx_006a38e0(self,nullptr,kind);
    else if(n<500)object_vfx_006a3e80(self,nullptr,kind);
    else if(n<600)object_vfx_006a04c0(self,nullptr,kind);
    else if(n<700)object_vfx_006a3fd0(self,nullptr,kind);
    else if(n<800)object_vfx_006a4000(self,nullptr,kind);
    else if(n<900)object_vfx_006a4060(self,nullptr,kind);
    else if(n<1000)object_vfx_006a4240(self,nullptr,kind);
    else if(n<1100)object_vfx_006a34b0(self,nullptr,kind);
    else if(n<1200)object_vfx_006a2af0(self,nullptr,kind);
    else if(n<1300)object_vfx_006a2e10(self,nullptr,kind);
    else if(n<1400)object_vfx_006a30e0(self,nullptr,kind);
    else if(n<1500)object_vfx_006a1220(self,nullptr,kind);
    else if(n<1600)object_vfx_006a4580(self,nullptr,kind);
    else if(n<1700)object_vfx_006a14e0(self,nullptr,kind);
    else if(n<1800)object_vfx_006a4680(self,nullptr,kind);
    else if(n<1900)object_vfx_006a15d0(self,nullptr,kind);
    wr(self,0xf4,1); // original also marks unknown selector as applied
}
void __fastcall object_vfx_006a56f0(void*self,void*,U kind){
    if(!rd(self,0xa8)||!rd(self,0xf4))return;
    const H n=static_cast<H>(kind);
    if(n>=100&&n<200)object_vfx_006a47b0(self,nullptr,kind);
    else if(n>=200&&n<300)object_vfx_006a47f0(self,nullptr,kind);
    else if(n>=300&&n<400)object_vfx_006a4830(self,nullptr,kind);
    else if(n>=400&&n<500)object_vfx_006a4920(self,nullptr,kind);
    else if(n>=500&&n<600)object_vfx_006a47f0(self,nullptr,kind);
    else if(n>=600&&n<700)object_vfx_006a05b0(self,nullptr,kind);
    else if(n>=1000&&n<1100)object_vfx_006a4720(self,nullptr,kind);
    else if(n>=1400&&n<1500)object_vfx_006a1470(self,nullptr);
    else if(n>=1500&&n<1600)object_vfx_006a49a0(self,nullptr,kind,0);
    else if(n>=1600&&n<1700)object_vfx_006a1570(self,nullptr);
    else if(n>=1800&&n<1900)object_vfx_006a1610(self,nullptr,kind);
    wr(self,0xf4,0);
}
U __fastcall object_vfx_006a5890(void*self,void*,U kind){
    const H n=static_cast<H>(kind);
    if(n>=300&&n<400)return object_vfx_006a4fa0(self,nullptr,kind);
    if(n>=400&&n<500){
        const U index=static_cast<H>(kind-400u);
        if(index==0||(index>1&&index<=3))wr(self,0xd0,0x3f000000);
        return 1;
    }
    if(n>=600&&n<700)return object_vfx_006a0680(self,nullptr,kind);
    if(n>=1200&&n<1300)return object_vfx_006a0800(self,nullptr,kind);
    if(n>=1700&&n<1800)return object_vfx_006a0860(self,nullptr,kind);
    return 1;
}
void __fastcall object_vfx_006a69e0(void*self,void*,U kind,U delta){
    const H n=static_cast<H>(kind);
    if(n>=400&&n<500)object_vfx_006a4c00(self,nullptr,delta);
    else if(n>=500&&n<600)object_vfx_006a4a80(self,nullptr,delta);
    else if(n>=1500&&n<1600)object_vfx_006a4e70(self,nullptr,kind,delta);
    else if(n>=1600&&n<1700)object_vfx_006a5920(self,nullptr,delta);
}
}
