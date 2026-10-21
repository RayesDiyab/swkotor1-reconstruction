#include "c6ca4ec170-layers.h"
#include <cstring>
namespace {
using namespace aurora_gui;
I32 signed32(U32 value){I32 result;std::memcpy(&result,&value,4);return result;}
I32 signed16(U16 value){return value<0x8000u?static_cast<I32>(value):static_cast<I32>(value)-65536;}
bool positive(U32 value){return value!=0&&(value&0x80000000u)==0;}
volatile U16* frame(I32 index){return cells().frames5_007b9468+index*5;}
void x87_store_alpha(U32 bits,volatile float* cell){
    // One original FLD input-bits and one FSTP directly to the actual float cell.
    // No U32/float pointer alias, SSE conversion, CW/status reset or partial store.
    __asm {fld bits}
    __asm {mov eax,cell}
    __asm {fstp dword ptr [eax]}
}
void store_alpha_one(U32 bits,volatile float* cell){
    // StartLayer originally stores constant bits without touching x87 status.
    // Preserve that32-bit store into the already-live float object.
    __asm {mov eax,cell}
    __asm {mov ecx,bits}
    __asm {mov dword ptr [eax],ecx}
}
U32 quantize(const void* address){
    float value;std::memcpy(&value,address,4);const float scale=255.0f;float rounded;U32 result;
    __asm {fld value} __asm {fmul scale} __asm {fstp rounded}
    __asm {fld rounded}



    __asm {frndint} __asm {fistp result}

    return result;
}
}
extern "C" void __stdcall kotor_AurGUIStartLayer_004591B0(){
    using namespace aurora_gui;I32 viewport_values[4];get_integer(0xba2,viewport_values);
    auto* base=frame(0);base[1]=static_cast<U16>(viewport_values[1]);base[0]=static_cast<U16>(viewport_values[0]);
    base[2]=static_cast<U16>(viewport_values[2]);base[3]=static_cast<U16>(viewport_values[3]);base[4]=0;
    store_alpha_one(0x3f800000u,cells().alpha_007b9248);*cells().index_007b9460=0;
    kotor_renderer_init::glDisable_0073d22c(*cells().lighting_0078d45c);kotor_renderer_init::glDisable_0073d22c(*cells().depth_0078d474);
    matrix_mode(0x1701);push_matrix();load_identity();ortho(0,1,0,1,-1,1);
    matrix_mode(0x1700);load_identity();kotor_renderer_init::glEnable_0073d390(*cells().blend_0078d448);blend_func(0x302,0x303);
}
extern "C" void __stdcall kotor_AurGUIStopLayer_004592B0(){
    using namespace aurora_gui;*cells().index_007b9460=-1;
    matrix_mode(0x1701);pop_matrix();matrix_mode(0x1700);
    kotor_renderer_init::glEnable_0073d390(*cells().lighting_0078d45c);kotor_renderer_init::glEnable_0073d390(*cells().depth_0078d474);
}
extern "C" void __cdecl aurora_viewport_00459170(std::uint32_t x,std::uint32_t y,std::uint32_t w,std::uint32_t h){aurora_gui::viewport(signed32(x),signed32(y),signed32(w),signed32(h));}
extern "C" void __cdecl aurora_color_00459190(std::uint32_t rgba){aurora_gui::color4ubv(reinterpret_cast<const unsigned char*>(&rgba));}
extern "C" std::uint32_t __cdecl aurora_vector_not_equal_0043e1f0(const void* a,const void* b){return aurora_vector_equal_004aa980(a,b)==0?1u:0u;}
extern "C" std::uint32_t __cdecl kotor_AurGUISetupViewport_004592F0(
    std::uint32_t x,std::uint32_t y,std::uint32_t width,std::uint32_t height,void* color,std::uint32_t depth,std::uint32_t alpha){
    using namespace aurora_gui;
    if(*cells().gate_007bb4d0)return 0;
    if(((x+width)|(y+height))&0x80000000u)return 0;
    if(!positive(width)||!positive(height))return 0;



    if(signed32(x)>=signed32(*cells().width_0078d1d4)||signed32(y)>=signed32(*cells().height_0078d1d8))return 0;

    const U16 old_index=static_cast<U16>(*cells().index_007b9460);auto* previous=frame(signed16(old_index));
    const U16 offset_x=static_cast<U16>(previous[0]+static_cast<U16>(x));
    const U16 offset_y=static_cast<U16>(previous[1]+static_cast<U16>(y));
    const U16 new_index=static_cast<U16>(old_index+1u);const I32 next=signed16(new_index);auto* current=frame(next);
    current[0]=offset_x;x87_store_alpha(alpha,&cells().alpha_007b9248[next]);current[1]=offset_y;current[2]=static_cast<U16>(width);
    *cells().index_007b9460=static_cast<short>(signed16(new_index));current[3]=static_cast<U16>(height);



    current[4]=static_cast<U16>(static_cast<unsigned char>(depth));

    const U32 viewport_y=static_cast<U32>(signed16(frame(0)[3]))-static_cast<U32>(signed16(offset_y))-height;
    aurora_viewport_00459170(static_cast<U32>(signed16(offset_x)),viewport_y,width,height);
    const auto* bytes=static_cast<const unsigned char*>(color);
    const U32 red=quantize(bytes),green=quantize(bytes+4),blue=quantize(bytes+8),a=quantize(&alpha);
    const U32 packed=((((a<<8)|blue)<<8)|green)<<8|red;
    const float sentinel[3]={-1,-1,-1};
    if(aurora_vector_not_equal_0043e1f0(color,sentinel)){
        kotor_renderer_init::glDisable_0073d22c(*cells().texture_0078d458);kotor_renderer_init::glDisable_0073d22c(*cells().lighting_0078d45c);
        begin(7);aurora_color_00459190(packed);
        vertex4f(0,0,0,1);vertex4f(1,0,0,1);vertex4f(1,1,0,1);vertex4f(0,1,0,1);end();
        kotor_renderer_init::glEnable_0073d390(*cells().texture_0078d458);kotor_renderer_init::glEnable_0073d390(*cells().lighting_0078d45c);
    }
    if(static_cast<unsigned char>(depth)){
        clear(0x500);kotor_renderer_init::glEnable_0073d390(*cells().depth_0078d474);
    }



    return 1;

}
extern "C" void __stdcall kotor_AurGUICloseViewport_00459580(){
    using namespace aurora_gui;if(*cells().gate_007bb4d0)return;
    const U16 index=static_cast<U16>(static_cast<U16>(*cells().index_007b9460)-1u);*cells().index_007b9460=static_cast<short>(signed16(index));
    auto* previous=frame(signed16(index));const I32 h=signed16(previous[3]),w=signed16(previous[2]);
    const I32 y=signed16(frame(0)[3])-signed16(previous[1])-h;const I32 x=signed16(previous[0]);viewport(x,y,w,h);



    previous=frame(signed16(static_cast<U16>(*cells().index_007b9460)));

    if(previous[4])kotor_renderer_init::glEnable_0073d390(*cells().depth_0078d474);else kotor_renderer_init::glDisable_0073d22c(*cells().depth_0078d474);
}
