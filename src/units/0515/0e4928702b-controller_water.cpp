// controller_water.cpp - WaterTextureController (vtable 00741D2C, 0x6C) and the lowpass helpers it calls:
// ImageLowpassLowQualityBlur 0045E8B0, ImageLowpassLowQualityBlur2 0045EA70, ImageLowpassBlur 0045ECB0.
// Evidence: build/ctl/0045FD70.txt, build/full.disasm.txt.
#include "49aa053254-controller_math.hpp"
#include "5825b1bc62-controller_rdata.h"   // D13 local generated header: 0078E450 kernel cold values, 0078E474 shift table
#include "59377d41d2-texture_cells.hpp"
#include <cstdlib>
using namespace controller_math;
using texture_controller::TextureController;
extern "C" int __cdecl crt_rand_006fc447();   // game CRT rand 006FC447 (shared holdrand stream; 0479-3d-model-model-lifetime)
extern I volatile enable_random_impulses_0078e554;   // controller_perlin_arturo.cpp

// Water/lowpass globals (.data/.bss; Ghidra labels).
U volatile low_quality_blur_007fb870=0;  // lowQualityBlur (BSS)
U volatile filter_water_0078e558=1;      // filterWater (.data 1)
U volatile low_quality_blur2_0078e55c=1; // lowQualityBlur2 (.data 1)
// Constant-initialized (no .CRT$XCU): hex-float literals bit-equal to the D13 generated cold values (asserted below).
float lowpass_kernel_0078e450[9]={0x0p+0f,0x1.99999ap-4f,0x0p+0f,0x1.99999ap-4f,0x1p-1f,0x1.99999ap-4f,0x0p+0f,0x1.99999ap-4f,0x0p+0f}; // .data
static_assert(lowpass_kernel_0078e450_bits[0]==0u&&lowpass_kernel_0078e450_bits[1]==0x3DCCCCCDu&&lowpass_kernel_0078e450_bits[2]==0u&&
              lowpass_kernel_0078e450_bits[3]==0x3DCCCCCDu&&lowpass_kernel_0078e450_bits[4]==0x3F000000u&&lowpass_kernel_0078e450_bits[5]==0x3DCCCCCDu&&
              lowpass_kernel_0078e450_bits[6]==0u&&lowpass_kernel_0078e450_bits[7]==0x3DCCCCCDu&&lowpass_kernel_0078e450_bits[8]==0u,
              "literal bits 0x0p+0f=00000000h, 0x1.99999ap-4f=3DCCCCCDh, 0x1p-1f=3F000000h must match 0078E450 cold values");
std::uint8_t lowpass_shift_0078e474[9]={lowpass_shift_0078e474_bytes[0],lowpass_shift_0078e474_bytes[1],lowpass_shift_0078e474_bytes[2],
 lowpass_shift_0078e474_bytes[3],lowpass_shift_0078e474_bytes[4],lowpass_shift_0078e474_bytes[5],lowpass_shift_0078e474_bytes[6],
 lowpass_shift_0078e474_bytes[7],lowpass_shift_0078e474_bytes[8]}; // .data (00 03 00 / 03 01 03 / 00 03 00 shift counts)
// ImageLowpassLowQualityBlur2 function-local statics (Ghidra labels 007BB514..007BB520).
U volatile blur2_old_width_007bb514=0,blur2_old_height_007bb518=0,blur2_new_image_007bb51c=0,blur2_guards_007bb520=0;

namespace {
struct WaterTextureController {
 TextureController base;
 I width;             // +38 "waterwidth"  (ctor 1Fh)
 I height;            // +3C "waterheight" (ctor 1Fh)
 U volatile grid;     // +40 float height grid
 U volatile next;     // +44 float grid
 U volatile velocity; // +48 float grid
 float force_phase;   // +4C
 float angle_phase;   // +50
 float force_speed;   // +54 "forcecyclespeed" (ctor 42652EE1h)
 float angle_speed;   // +58 "anglecyclespeed" (ctor 403758B4h)
 U volatile image;    // +5C byte image
 U volatile image2;   // +60 byte image (low quality blur source)
 I counter;           // +64
 I phase;             // +68 (ctor 2)
};
static_assert(sizeof(WaterTextureController)==0x6c,"Water layout (0042240E NewDerived 6Ch)");
WaterTextureController* self_of(void* p){return static_cast<WaterTextureController*>(p);}
float cell(U base,I index){float f;std::memcpy(&f,reinterpret_cast<const void*>(base+static_cast<U>(index)*4u),4);return f;}
void set_cell(U base,I index,float v){std::memcpy(reinterpret_cast<void*>(base+static_cast<U>(index)*4u),&v,4);}
}

extern "C" {
// 0045E8B0 ImageLowpassLowQualityBlur(out, image, w, h, phase): rows [h*phase/3, h*(phase+1)/3) accumulate a
// plus-shaped kernel (0078E450) into out (out is not cleared here).
void __cdecl image_lowpass_low_quality_blur_0045e8b0(std::uint8_t* out,const std::uint8_t* image,texture::I w,texture::I h,texture::I phase){
 const I first=(h*phase)/3;                                                     // 0045E8BD..0045E8D0 (magic 55555556h, truncating)
 const I last=((phase+1)*h)/3;                                                  // 0045E8D2..0045E8E4
 if(first>=last)return;                                                         // 0045E8E6
 I row_offset=first*w;                                                          // [esp+14h]
 I row_counter=first+h-1;                                                       // [esp+1Ch]
 for(I rows=last-first;rows!=0;--rows){                                         // [esp+20h]
  if(w>0){
   I column=w+1;                                                                // arg cell reused (0045E920)
   for(I x=0;x<w;++x){
    I r_counter=row_counter;
    for(I ky=0;ky<3;++ky){                                                      // esi 0078E454 step 0Ch
     const I r=r_counter%h;                                                     // 0045E940..0045E945
     std::uint8_t& target=out[row_offset+x];
     if(ky==1){
      const I c0=(column-2)%w;                                                  // 0045E96C..0045E974
      target=static_cast<std::uint8_t>(target+static_cast<std::uint8_t>(ftol(static_cast<double>(image[r*w+c0])*static_cast<double>(lowpass_kernel_0078e450[ky*3])))); // 0045E97A..0045E995
     }
     const I c1=(column-1)%w;                                                   // 0045E9AD..0045E9B3
     target=static_cast<std::uint8_t>(target+static_cast<std::uint8_t>(ftol(static_cast<double>(image[r*w+c1])*static_cast<double>(lowpass_kernel_0078e450[ky*3+1])))); // 0045E9B9..0045E9D9
     if(ky==1){
      const I c2=column%w;                                                      // 0045E9E6..0045E9EB
      target=static_cast<std::uint8_t>(target+static_cast<std::uint8_t>(ftol(static_cast<double>(image[r*w+c2])*static_cast<double>(lowpass_kernel_0078e450[ky*3+2])))); // 0045E9ED..0045EA0A
     }
     ++r_counter;
    }
    ++column;
   }
  }
  ++row_counter;row_offset+=w;                                                  // 0045EA3F..0045EA53
 }
}
// 0045EA70 ImageLowpassLowQualityBlur2(image, w, h): plus kernel of right shifts (0078E474) via a cached buffer.
void __cdecl image_lowpass_low_quality_blur2_0045ea70(std::uint8_t* image,texture::I w,texture::I h){
 U guards=blur2_guards_007bb520;                                                // 0045EA7E
 if(!(guards&1u)){
  guards|=1u;blur2_guards_007bb520=guards;                                      // 0045EA95..0045EA98
  blur2_new_image_007bb51c=address(live_operator_new(static_cast<U>(w)*static_cast<U>(h))); // 0045EAA5..0045EAB8
  guards=blur2_guards_007bb520;                                                 // 0045EABD
 }
 if(!(guards&2u)){guards|=2u;blur2_guards_007bb520=guards;blur2_old_height_007bb518=static_cast<U>(h);} // 0045EAD7..0045EAE3
 if(!(guards&4u)){guards|=4u;blur2_guards_007bb520=guards;blur2_old_width_007bb514=static_cast<U>(w);}  // 0045EAE9..0045EAF5
 if(blur2_old_height_007bb518!=static_cast<U>(h)||blur2_old_width_007bb514!=static_cast<U>(w)){ // 0045EAFB..0045EB09
  live_free(reinterpret_cast<void*>(static_cast<U>(blur2_new_image_007bb51c)));  // 0045EB0B..0045EB12
  blur2_new_image_007bb51c=address(live_operator_new(static_cast<U>(w)*static_cast<U>(h))); // 0045EB17..0045EB25
  blur2_old_height_007bb518=static_cast<U>(h);blur2_old_width_007bb514=static_cast<U>(w); // 0045EB2A..0045EB30
 }
 const U size=static_cast<U>(w)*static_cast<U>(h);
 std::memset(reinterpret_cast<void*>(static_cast<U>(blur2_new_image_007bb51c)),0,size); // 0045EB36..0045EB51 rep stos
 U row=blur2_new_image_007bb51c;                                                // ebx
 for(I y=0;y<h;++y){
  if(w>0){
   const I row_counter=h+y-1;                                                   // 0045EB7E
   I column=w+1;                                                                // [esp+10h]
   for(I x=0;x<w;++x){
    I r_counter=row_counter;
    for(I ky=0;ky<3;++ky){                                                      // ebp 0078E475 step 3
     const I r=r_counter%h;
     std::uint8_t& target=bytes(row)[x];
     const std::uint8_t* shift=&lowpass_shift_0078e474[ky*3+1];
     if(ky==1){const I c0=(column-2)%w;target=static_cast<std::uint8_t>(target+static_cast<std::uint8_t>(image[r*w+c0]>>shift[-1]));} // 0045EBBB..0045EBE1
     {const I c1=(column-1)%w;target=static_cast<std::uint8_t>(target+static_cast<std::uint8_t>(image[r*w+c1]>>shift[0]));}            // 0045EBE4..0045EC0C
     if(ky==1){const I c2=column%w;target=static_cast<std::uint8_t>(static_cast<std::uint8_t>(image[r*w+c2]>>shift[1])+target);}       // 0045EC19..0045EC35
     ++r_counter;
    }
    ++column;
   }
  }
  row+=static_cast<U>(w);                                                       // 0045EC75
 }
 rep_copy(address(image),blur2_new_image_007bb51c,size);                        // 0045EC83..0045EC9B
}
// 0045ECB0 ImageLowpassBlur(image, w, h, bpp): 3x3 weighted blur (local kernel 0.025/0.1/0.5) per channel.
void __cdecl image_lowpass_blur_0045ecb0(std::uint8_t* image,texture::I w,texture::I h,texture::I bpp){
 static const U kernel_bits[9]={0x3CCCCCCDu,0x3DCCCCCDu,0x3CCCCCCDu,0x3DCCCCCDu,0x3F000000u,0x3DCCCCCDu,0x3CCCCCCDu,0x3DCCCCCDu,0x3CCCCCCDu}; // 0045ECD0..0045ED10 immediates
 const U size=static_cast<U>(w)*static_cast<U>(h)*static_cast<U>(bpp);         // 0045ECC8..0045ECCB
 std::uint8_t* const buffer=static_cast<std::uint8_t*>(live_operator_new(size)); // 0045ED1C
 std::memset(buffer,0,size);                                                    // 0045ED23..0045ED37
 I row_offset=0;                                                                // arg cell [esp+8Ch] reused
 for(I y=0;y<h;++y){
  for(I x=0;x<w;++x){
   for(I ky=0;ky<3;++ky){
    const I r=(h+y-1+ky)%h;                                                     // 0045EDA0..0045EDA5
    for(I kx=0;kx<3;++kx){
     const I c=(w+x-1+kx)%w;                                                    // 0045EDC0..0045EDC5
     const double weight=static_cast<double>(as_float(kernel_bits[ky*3+kx]));   // fld [esp+edi*4+58h]
     const U source=static_cast<U>(r*w+c)*static_cast<U>(bpp);
     const U target=static_cast<U>(x+row_offset)*static_cast<U>(bpp);
     for(I ch=0;ch<bpp;++ch)                                                    // groups of 4 (0045EE42..0045EECE) + tail (0045EF30..0045EF4B)
      buffer[target+static_cast<U>(ch)]=static_cast<std::uint8_t>(buffer[target+static_cast<U>(ch)]+static_cast<std::uint8_t>(ftol(static_cast<double>(image[source+static_cast<U>(ch)])*weight)));
    }
   }
  }
  row_offset+=w;                                                                // 0045EFBD
 }
 rep_copy(address(image),address(buffer),size);                                 // 0045EFD2..0045EFEE
 live_free(buffer);                                                             // 0045EFF0
}

// ---------------- Water ----------------
void* __fastcall water_controller_ctor_004630b0(void* p,void*,font::Texture* owner){
 texture_controller_ctor_00462a70(p,nullptr,owner);                             // 004630B8
 WaterTextureController* const c=self_of(p);
 c->grid=0;c->velocity=0;c->next=0;c->image=0;c->image2=0;                     // 004630C4..004630D0
 c->base.vtable=&texture_controller::water_vtable_00741d2c;                     // 004630D3
 std::memcpy(&c->force_speed,"\xe1\x2e\x65\x42",4);                             // 004630D9 42652EE1h
 std::memcpy(&c->angle_speed,"\xb4\x58\x37\x40",4);                             // 004630E0 403758B4h
 c->width=0x1f;c->height=0x1f;                                                  // 004630E7/004630EA
 c->phase=2;                                                                    // 004630ED
 return c;
}
void __fastcall water_controller_dtor_00463120(void* p,void*){
 WaterTextureController* const c=self_of(p);
 c->base.vtable=&texture_controller::water_vtable_00741d2c;                     // 0046313D
 live_free(reinterpret_cast<void*>(static_cast<U>(c->grid)));                  // 00463143
 live_free(reinterpret_cast<void*>(static_cast<U>(c->velocity)));              // 00463154
 live_free(reinterpret_cast<void*>(static_cast<U>(c->next)));                  // 0046315D
 live_free(reinterpret_cast<void*>(static_cast<U>(c->image)));                 // 00463166
 live_free(reinterpret_cast<void*>(static_cast<U>(c->image2)));                // 0046316F
 texture_controller_dtor_00460cf0(p,nullptr);                                   // 00463185
}
void* __fastcall water_controller_delete_00463100(void* p,void*,texture::U flags){
 water_controller_dtor_00463120(p,nullptr);if(flags&1u)live_free(p);return p;
}
// 004631A0 ParseField: base first, then four keywords.
void __fastcall water_controller_parse_field_004631a0(void* p,void*,const char* text){
 WaterTextureController* const c=self_of(p);
 char* const line=const_cast<char*>(text);
 texture_controller_parse_field_00462b00(p,nullptr,text);                       // 004631A9
 if(kotor_stricmp_0070acaf("forcecyclespeed",firstword_00463530(line))==0)parse_float_004635c0(line+0x0f,&c->force_speed); // 004631AE..004631CE
 if(kotor_stricmp_0070acaf("anglecyclespeed",firstword_00463530(line))==0)parse_float_004635c0(line+0x0f,&c->angle_speed); // 004631D6..004631F6
 if(kotor_stricmp_0070acaf("waterwidth",firstword_00463530(line))==0)parse_int_004635e0(line+0x0a,&c->width);             // 004631FE..0046321E
 if(kotor_stricmp_0070acaf("waterheight",firstword_00463530(line))==0)parse_int_004635e0(line+0x0b,&c->height);           // 00463226..00463246
}
// 0045FF40 Init.
void __fastcall water_controller_init_0045ff40(void* p,void*){
 WaterTextureController* const c=self_of(p);
 const I gw=c->width,gh=c->height;                                              // 0045FF45/0045FF4C
 const U cells=static_cast<U>(gw)*static_cast<U>(gh);                           // 0045FF4F..0045FF51
 c->counter=0;                                                                  // 0045FF56
 if(c->base.f0c!=0)texture_controller::table(&c->base)->s18(&c->base,nullptr); // 0045FF48..0045FF61
 c->velocity=address(live_operator_new(cells<<2));                              // 0045FF64..0045FF6E
 c->grid=address(live_operator_new(cells<<2));                                  // 0045FF71..0045FF77
 c->next=address(live_operator_new(cells<<2));                                  // 0045FF7A..0045FF84
 I index=0;
 for(I row=gh;row>0;--row)                                                      // 0045FF82..0045FFC4
  for(I col=0;col<gw;++col,++index){set_cell(c->velocity,index,0.0f);set_cell(c->grid,index,127.0f);} // 0045FFA0..0045FFB4
 const I width=owner_width(&c->base);                                           // 0045FFCB
 const I height=owner_height(&c->base);                                         // 0045FFD5
 const U pixels=static_cast<U>(height)*static_cast<U>(width);                   // 0045FFD8..0045FFDA
 c->image=address(live_operator_new(pixels));                                   // 0045FFDD..0045FFE4
 c->image2=address(live_operator_new(pixels));                                  // 0045FFE7..0045FFEF
 std::memset(&c->force_phase,0,4);std::memset(&c->angle_phase,0,4);             // 0045FFF2..0045FFF8
}
// 00461820 Control.
void __fastcall water_controller_control_00461820(void* p,void*){
 WaterTextureController* const c=self_of(p);
 const U frame=texture_cells::texture_current_frame_007a46f4;                   // 00461823
 if(static_cast<U>(c->base.f08)==frame)return;                                  // 0046182B
 c->base.f08=static_cast<I>(frame);                                             // 00461838
 if(low_quality_blur_007fb870==0){                                              // 0046183B..00461842
  if(c->counter%2!=0){c->counter=c->counter+1;return;}                          // 00461844..00461856 / 004618A0..004618A8
  c->counter=0;                                                                 // 00461858
 }
 c->phase=(c->phase+1)%3;                                                       // 0046185F..0046186F
 if(!(c->phase!=0&&low_quality_blur_007fb870!=0)){                              // 00461872..0046187B
  const I n=c->width;                                                           // esi (also the row count and modulus)
  const double limit=static_cast<double>(as_float(0x3D23D70Au));                // [00741C40]
  const double delta=static_cast<double>(texture_cells::delta_t_0078e574);
  const double dt=(delta<limit)?delta:limit;                                    // 00461885..004618AF
  const float step=static_cast<float>(dt*static_cast<double>(c->base.f18)*1.0); // 004618AF..004618BB [00741BF8]
  const double two_pi=static_cast<double>(as_float(0x40C90FDBu));               // [00740C88]
  const double k5f8=static_cast<double>(as_float(0x3C8EFA35u));                 // [007415F8] pi/180
  {const double v=static_cast<double>(step)*static_cast<double>(c->force_speed)*k5f8+static_cast<double>(c->force_phase); // 004618BF..004618CC
   c->force_phase=static_cast<float>(v);                                        // 004618D5 fst
   if(v>two_pi)c->force_phase=static_cast<float>(v-two_pi);}                    // 004618CF..004618E5
  {const double v=static_cast<double>(step)*static_cast<double>(c->angle_speed)*k5f8+static_cast<double>(c->angle_phase); // 004618EC..004618F9
   c->angle_phase=static_cast<float>(v);                                        // 00461902 fst
   if(v>two_pi)c->angle_phase=static_cast<float>(v-two_pi);}                    // 004618FC..00461912
  const float force=static_cast<float>(static_cast<double>(cosine_lut_004aadf0(c->force_phase))*0.25+static_cast<double>(as_float(0x3E4CCCCDu))); // 0046191E..00461933 [0073EDF0],[0073E46C]
  const float angle=static_cast<float>(static_cast<double>(cosine_lut_004aadf0(c->angle_phase))+0.0); // 00461937..00461955 [00741BF0]
  float weight[9];
  weight[1]=1.0f;weight[3]=1.0f;weight[4]=0.0f;weight[5]=1.0f;weight[7]=1.0f;   // 00461945..00461973
  auto corner=[&](U offset_bits){
   const float argument=static_cast<float>(static_cast<double>(angle)-static_cast<double>(as_float(offset_bits)));
   return static_cast<float>(static_cast<double>(cosine_lut_004aadf0(argument))*static_cast<double>(force)+0.5);
  };
  weight[0]=corner(0x3F490FDBu);                                                // 00461961..0046198D [00741C3C]
  weight[2]=corner(0x4016CBE4u);                                                // 00461991..004619AD [00741C38]
  weight[8]=corner(0x407B53D1u);                                                // 004619B1..004619CD [00741C34]
  weight[6]=corner(0x40AFEDDFu);                                                // 004619D1..004619F5 [00741C30]
  const I image_width=owner_width(&c->base);                                    // 004619F9 s48 -> [esp+38h]
  const float height_ratio=static_cast<float>(static_cast<double>(static_cast<float>(c->height))/static_cast<double>(owner_height(&c->base))); // 004619FC..00461A20
  const float width_ratio=static_cast<float>(static_cast<double>(static_cast<float>(c->width))/static_cast<double>(owner_width(&c->base)));    // 00461A24..00461A44
  for(I y=0;y<n;++y){                                                           // 00461A48..00461C06 (bound = waterwidth)
   for(I x=0;x<n;++x){                                                          // 00461A56..00461BE4
    const I index=y*n+x;
    float old;std::memcpy(&old,reinterpret_cast<const void*>(c->velocity+static_cast<U>(index)*4u),4); // 00461A7E..00461A89
    const float center=static_cast<float>(static_cast<double>(cell(c->grid,index))-127.0); // 00461A8D..00461A9F
    double sum=0.0;
    for(I r=0;r<3;++r){
     const I rr=(y+r+n-1)%n;                                                    // 00461AA3..00461AB3
     for(I k=0;k<3;++k){
      const I cc=(x+n+1+k-2)%n;                                                 // 00461AB7..00461B3B
      double v;
      if(r==1&&k==1)v=static_cast<double>(center);                              // 00461ABE..00461AC7 / 00461B05..00461B11
      else v=-((static_cast<double>(cell(c->grid,rr*n+cc))-127.0)-static_cast<double>(center)); // fsub 127; fsub center; fchs
      sum=sum-v*static_cast<double>(weight[r*3+k])*1.0;                         // fmul w; fmul [00741BE8]; fsubp
     }
    }
    const double damped=sum-static_cast<double>(as_float(0x3CCCCCCDu))*static_cast<double>(old); // 00461B95..00461BA7 [00741BEC]
    const double a=damped*static_cast<double>(step);                            // 00461BA9
    const double b=(0.5*a+static_cast<double>(old))*static_cast<double>(step)+static_cast<double>(center); // 00461BAD..00461BBD
    set_cell(c->velocity,index,static_cast<float>(a+static_cast<double>(old))); // 00461BC1..00461BC7
    set_cell(c->next,index,static_cast<float>(b));                              // 00461BC9..00461BD0
   }
  }
  if(enable_random_impulses_0078e554!=0){                                       // 00461C0C..00461C13
   const I ry=crt_rand_006fc447()%n;const I rx=crt_rand_006fc447()%n;const I r=crt_rand_006fc447()%127;  // 00461C15..00461C37
   const I index=ry*n+rx;
   set_cell(c->velocity,index,static_cast<float>(static_cast<double>(r)+static_cast<double>(cell(c->velocity,index)))); // 00461C3F..00461C4F
  }
  const double low=static_cast<double>(as_float(0x42200000u)),high=255.0-low;   // [00741BF4], 0073EDE4-00741BF4
  for(I y=0;y<n;++y)                                                            // 00461C51..00461E54
   for(I x=0;x<n;++x){
    const I index=y*n+x;
    double v=static_cast<double>(cell(c->next,index))+127.0;                    // 00461C96..00461C9C
    if(v<low){v=low;set_cell(c->velocity,index,0.0f);}                          // 00461CA2..00461CBA (integer 0 store)
    if(v>high){v=high;set_cell(c->velocity,index,0.0f);}                        // 00461CBD..00461CCD
    set_cell(c->grid,index,static_cast<float>(v));                              // 00461CD0..00461CD3
   }
  U row_offset=0;                                                               // ebx
  for(I y=0;y<owner_height(&c->base);++y){                                      // 00461E69 / 00461EF3 (height re-read per row)
   const float vy=static_cast<float>(static_cast<double>(y)*static_cast<double>(height_ratio)); // 00461E78..00461E88
   for(I x=0;x<image_width;++x){                                                // 00461E7E..00461EE1
    const float vx=static_cast<float>(static_cast<double>(x)*static_cast<double>(width_ratio)); // 00461E93..00461EA9
    const double s=texture_controller_grid_sample_004608a0(bytes(c->grid),vx,vy,c->width,c->height); // 00461EAD
    const U t=low_quality_blur_007fb870!=0?c->image2:c->image;                  // 00461EB2..00461ED2 (reloaded)
    bytes(t)[row_offset+static_cast<U>(x)]=static_cast<std::uint8_t>(ftol(s));  // 00461EBE..00461ED7
   }
   row_offset+=static_cast<U>(image_width);                                     // 00461EF1
  }
  if(low_quality_blur_007fb870!=0)                                              // 00461EFE..00461F25
   std::memset(bytes(c->image),0,static_cast<U>(owner_height(&c->base))*static_cast<U>(image_width));
 }
 // 00461F28: filtering.
 if(filter_water_0078e558!=0){
  font::Texture* const owner=c->base.owner;                                     // 00461F40
  if(low_quality_blur_007fb870!=0){                                             // 00461F39..00461F43
   const I h=texture_vtable::head(owner)->common.s4c(owner,nullptr);            // 00461F53
   const I w=texture_vtable::head(owner)->common.s48(owner,nullptr);            // 00461F5B
   image_lowpass_low_quality_blur_0045e8b0(bytes(c->image),bytes(c->image2),w,h,c->phase); // 00461F5E..00461F67
  }else if(low_quality_blur2_0078e55c!=0){                                      // 00461F71..00461F7A
   const I h=texture_vtable::head(owner)->common.s4c(owner,nullptr);            // 00461F85
   const I w=texture_vtable::head(owner)->common.s48(owner,nullptr);            // 00461F8D
   image_lowpass_low_quality_blur2_0045ea70(bytes(c->image),w,h);               // 00461F90..00461F95
  }else{
   const I h=texture_vtable::head(owner)->common.s4c(owner,nullptr);            // 00461FAA
   const I w=texture_vtable::head(owner)->common.s48(owner,nullptr);            // 00461FB2
   image_lowpass_blur_0045ecb0(bytes(c->image),w,h,1);                          // 00461FB5..00461FBA
  }
 }
 if(!(c->phase==2||low_quality_blur_007fb870==0))return;                        // 00461FC2..00461FCF
 U result=c->image;                                                             // 00461FD6
 bool owned=false;
 if(owner_bpp(&c->base)!=1){                                                    // 00461FDB..00461FE1 (full EAX)
  if(c->base.f0c!=0)result=address(texture_controller::table(&c->base)->s14(&c->base,nullptr,bytes(c->image))); // 00461FE3..00461FF2
  else{result=address(texture_controller_height2channel_00461060(&c->base,nullptr,bytes(c->image),nullptr));owned=true;} // 00461FF7..00462004
 }
 {font::Texture* const o=c->base.owner;(void)texture_vtable::head(o)->common.sac(o,nullptr,1,result);} // 00462008..00462010
 if(owned)live_free(reinterpret_cast<void*>(result));                           // 00462016..0046201B
 c->counter=c->counter+1;                                                       // 00462023
}
}
namespace texture_controller {
const TextureControllerVTable water_vtable_00741d2c={
 &water_controller_delete_00463100,&water_controller_control_00461820,&water_controller_init_0045ff40,
 &water_controller_parse_field_004631a0,&texture_controller_is_linear_0041f250,&texture_controller_distort_00460d70,
 &texture_controller_set_image_0045fd70};
}
