#include "49d96e5f57-soft_shadow.h"
namespace {
using namespace kotor_soft_shadow;
U32 read(const volatile U32* p){return *p;}
// Revision: one volatile U32 read, then the x86 handle-value conversion (as kotor_pbuffer).
void* handle(const volatile U32* p){return reinterpret_cast<void*>(static_cast<std::uintptr_t>(*p));}
void* read(void** p){return *static_cast<void* volatile*>(p);}
void put(void** p,void* x){*static_cast<void* volatile*>(p)=x;}
void params(U32 target){
    glTexParameteri_0073d23c(target,0x2802,0x2900);
    glTexParameteri_0073d23c(target,0x2803,0x2900);
    glTexParameteri_0073d23c(target,0x2800,0x2601);
    glTexParameteri_0073d23c(target,0x2801,0x2601);
}
void copy(U32 target,const volatile U32* width,const volatile U32* height){
    const U32 h=read(height),w=read(width);
    glCopyTexImage2D_0073d2d8(target,0,0x8058,0,0,w,h,0);
}
void buffer(void** out,void** dc,void** context,U32* texture,bool ati){
    const auto& b=bindings;
    void* p=CreatePBuffer_00426560(512,512);put(out,p);
    void* d=(*b.get_dc_007bb6b8)(p);put(dc,d);
    if(ati){
        void* c=handle(b.main_context_007a6854);put(context,c);
        (*b.make_current_007bb818)(d,d,c);
    }else{
        void* c=wglCreateContext_0073d2b8(d);put(context,c);
        wglShareLists_0073d2b4(handle(b.main_context_007a6854),c);
        void* saved_c=read(context);void* saved_d=read(dc);
        wglMakeCurrent_0073d2b0(saved_d,saved_c);
    }
    glGenTextures_0073d24c(1,texture);glBindTexture_0073d240(0xde1,read(texture));params(0xde1);
}
void restoreATI(){const auto& b=bindings;void* c=handle(b.main_context_007a6854);void* d=handle(b.main_dc_007a47e4);(*b.make_current_007bb818)(d,d,c);}
}
namespace kotor_renderer_init {
void __cdecl InitializeSoftShadows_00426cc0(){
    using namespace kotor_soft_shadow;const auto& b=bindings;
    if(SoftAvailable_0045f7b0()!=0 && *static_cast<volatile U8*>(b.enabled_0078dae9)!=0 && read(b.requested_0078e420)!=0){
        if(read(b.scene_007a687c)==0){glGenTextures_0073d24c(1,b.scene_007a687c);glBindTexture_0073d240(0x84f5,read(b.scene_007a687c));copy(0x84f5,b.width_0078d1d4,b.height_0078d1d8);glBindTexture_0073d240(0x84f5,0);}
        glGenTextures_0073d24c(1,b.scratch_007a68f8);glBindTexture_0073d240(0x84f5,read(b.scratch_007a68f8));copy(0x84f5,b.width_0078d1d4,b.height_0078d1d8);params(0x84f5);
        buffer(b.buffer1_007a68e8,b.dc1_007a68ec,b.context1_007a68f0,b.texture1_007a68f4,false);
        buffer(b.buffer2_007a68fc,b.dc2_007a6900,b.context2_007a6904,b.texture2_007a6908,false);
        void* c=handle(b.main_context_007a6854);void* d=handle(b.main_dc_007a47e4);wglMakeCurrent_0073d2b0(d,c);return;
    }
    if(ATISoftAvailable_0045f7e0()==0 || *static_cast<volatile U8*>(b.enabled_0078dae9)==0 || read(b.requested_0078e420)==0)return;
    const U32 h=read(b.height_0078d1d8),w=read(b.width_0078d1d4);PowerExtent_00427450(w,h,b.power_width_007a6890,b.power_height_007a6894);
    if(read(b.scene_007a687c)==0){glGenTextures_0073d24c(1,b.scene_007a687c);glBindTexture_0073d240(0xde1,read(b.scene_007a687c));params(0xde1);copy(0xde1,b.power_width_007a6890,b.power_height_007a6894);glBindTexture_0073d240(0xde1,0);}
    glGenTextures_0073d24c(1,b.scratch_007a68f8);glBindTexture_0073d240(0xde1,read(b.scratch_007a68f8));copy(0xde1,b.power_width_007a6890,b.power_height_007a6894);params(0xde1);
    buffer(b.buffer1_007a68e8,b.dc1_007a68ec,b.context1_007a68f0,b.texture1_007a68f4,true);restoreATI();
    buffer(b.buffer2_007a68fc,b.dc2_007a6900,b.context2_007a6904,b.texture2_007a6908,true);restoreATI();
    glEnable_0073d390(0x8920);
    *static_cast<volatile U32*>(b.shader_007a6928)=(*b.gen_shader_007bb754)(1);
    (*b.bind_shader_007bb6cc)(read(b.shader_007a6928));(*b.begin_007bb7f4)();
    (*b.sample_007bb620)(0x8921,0x84c0,0x8976);
    (*b.sample_007bb620)(0x8922,0x84c1,0x8976);
    (*b.sample_007bb620)(0x8923,0x84c2,0x8976);
    (*b.sample_007bb620)(0x8924,0x84c3,0x8976);
    (*b.color2_007bb840)(0x8964,0x8925,0,0,0x8921,0,0,0x8941,0,0);
    (*b.alpha2_007bb5e8)(0x8964,0x8925,0,0x8921,0,0,0x8941,0,0);
    (*b.color3_007bb7fc)(0x8968,0x8925,0,0,0x8922,0,0,0x8942,0,0,0x8925,0,0);
    (*b.alpha3_007bb680)(0x8968,0x8925,0,0x8922,0,0,0x8942,0,0,0x8925,0,0);
    (*b.color3_007bb7fc)(0x8968,0x8922,0,0,0x8923,0,0,0x8943,0,0,0x8925,0,0);
    (*b.alpha3_007bb680)(0x8968,0x8922,0,0x8923,0,0,0x8943,0,0,0x8925,0,0);
    (*b.color3_007bb7fc)(0x8968,0x8921,0,0,0x8924,0,0,0x8944,0,0,0x8922,0,0);
    (*b.alpha3_007bb680)(0x8968,0x8921,0,0x8924,0,0,0x8944,0,0,0x8922,0,0);
    (*b.end_007bb554)();glDisable_0073d22c(0x8920);
}
}
