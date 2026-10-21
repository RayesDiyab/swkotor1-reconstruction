// controller_wave.cpp - WaveTextureController (vtable 00741D80, 0x40). Evidence build/ctl/0045FD70.txt.
#include "49aa053254-controller_math.hpp"
#include "59377d41d2-texture_cells.hpp"
#include <cstdlib>
using namespace controller_math;
using texture_controller::TextureController;
extern "C" int __cdecl crt_rand_006fc447();   // game CRT rand 006FC447 (shared holdrand stream; 0479-3d-model-model-lifetime)
extern I volatile enable_random_impulses_0078e554;   // controller_perlin_arturo.cpp (.data 1)
namespace {
struct WaveTextureController {TextureController base; U volatile current; U volatile next;}; // +38/+3C float grids
static_assert(sizeof(WaveTextureController)==0x40,"Wave layout (00422532 NewDerived 40h)");
WaveTextureController* self_of(void* p){return static_cast<WaveTextureController*>(p);}
float cell(U base,I index){float f;std::memcpy(&f,reinterpret_cast<const void*>(base+static_cast<U>(index)*4u),4);return f;}
void set_cell(U base,I index,float v){std::memcpy(reinterpret_cast<void*>(base+static_cast<U>(index)*4u),&v,4);}
}
extern "C" {
void* __fastcall wave_controller_ctor_00463260(void* p,void*,font::Texture* owner){
 texture_controller_ctor_00462a70(p,nullptr,owner);                             // 00463268
 WaveTextureController* const c=self_of(p);
 c->current=0;c->next=0;                                                        // 0046326F/00463272
 c->base.vtable=&texture_controller::wave_vtable_00741d80;                      // 00463275
 return c;
}
void __fastcall wave_controller_dtor_004632b0(void* p,void*){
 WaveTextureController* const c=self_of(p);
 c->base.vtable=&texture_controller::wave_vtable_00741d80;                      // 004632CD
 live_free(reinterpret_cast<void*>(static_cast<U>(c->current)));               // 004632D3
 live_free(reinterpret_cast<void*>(static_cast<U>(c->next)));                  // 004632E4
 texture_controller_dtor_00460cf0(p,nullptr);                                   // 004632FA
}
void* __fastcall wave_controller_delete_00463290(void* p,void*,texture::U flags){
 wave_controller_dtor_004632b0(p,nullptr);if(flags&1u)live_free(p);return p;
}
// 00460000 Init: grids W*H floats; +3C=127.0f, +38=rand()%256 (the image-derived value is overwritten).
void __fastcall wave_controller_init_00460000(void* p,void*){
 WaveTextureController* const c=self_of(p);
 const I width=owner_width(&c->base);                                           // 0046000E
 const I height=owner_height(&c->base);                                         // 0046001C
 const U count=static_cast<U>(height)*static_cast<U>(width);                    // 00460024..00460026
 if(c->base.f0c!=0)texture_controller::table(&c->base)->s18(&c->base,nullptr); // 00460021..00460031
 c->current=address(live_operator_new(count<<2));                               // 00460034..0046003E
 c->next=address(live_operator_new(count<<2));                                  // 00460041..00460049
 const U image=address(owner_image(&c->base));                                  // 00460046..00460051
 I index=0;                                                                     // ebx
 for(I row=0;row<height;++row){                                                 // 00460054..00460110
  for(I col=0;col<width;++col){                                                 // 00460070..004600E6
   set_cell(c->next,index,127.0f);                                              // 00460082..00460085 42FE0000h
   if(image){
    const I bpp=owner_bpp(&c->base);                                            // 00460093
    set_cell(c->current,index,static_cast<float>(bytes(image)[static_cast<U>(bpp)*static_cast<U>(index)])); // 00460096..004600A8
   }else set_cell(c->current,index,127.0f);                                     // 004600AD..004600B0
   const I r=crt_rand_006fc447()%256;                                                   // 004600B7..004600C9
   set_cell(c->current,index,static_cast<float>(r));                            // 004600CA..004600D9
   ++index;
  }
 }
}
// 00462030 Control: one wave-equation step (3x3 kernel), swap, impulse, clamp, quantize, distort/H2C, upload.
void __fastcall wave_controller_control_00462030(void* p,void*){
 WaveTextureController* const c=self_of(p);
 const U frame=texture_cells::texture_current_frame_007a46f4;                   // 00462030
 if(static_cast<U>(c->base.f08)==frame)return;                                  // 0046203B
 c->base.f08=static_cast<I>(frame);                                             // 00462049
 const I width=owner_width(&c->base);                                           // 0046204F
 const I height=owner_height(&c->base);                                         // 00462059
 const double limit=static_cast<double>(as_float(0x3D23D70Au));                 // [00741C40] 0.04
 const double delta=static_cast<double>(texture_cells::delta_t_0078e574);
 const double dt=(delta<limit)?delta:limit;                                     // 0046205C..00462083 (NaN -> limit)
 const double scaled=dt*1.0;                                                    // 00462083 fmul [00741BFC]=1.0f
 const double c2=scaled*scaled;                                                 // 00462095..00462097
 float kernel[9];
 kernel[0]=0.0f;kernel[2]=0.0f;kernel[6]=0.0f;kernel[8]=0.0f;                   // 0046208D..004620A9
 kernel[3]=static_cast<float>(c2);kernel[1]=static_cast<float>(c2);kernel[7]=static_cast<float>(c2);kernel[5]=static_cast<float>(c2); // 004620B1..004620C1
 kernel[4]=static_cast<float>(2.0-c2*4.0);                                      // 004620C5..004620D1 [00741BE0]-c*[0073F404]
 for(I y=0;y<height;++y){                                                       // 004620D5..004621E0
  I column=height+1;                                                            // [esp+14h] (ebx+1 with ebx=height)
  U offset=static_cast<U>(y)*static_cast<U>(width)*4u;                          // [esp+10h]
  for(I col=0;col<width;++col){                                                 // 004620E0..004621BA
   const U target=c->next+offset;                                               // 00462101..0046210E
   float old;std::memcpy(&old,reinterpret_cast<const void*>(target),4);         // 00462110 [esp+38h]
   const I c0=(column-2)%width,c1=(column-1)%width,c2i=column%width;            // 0046211A..00462140
   double sum=0.0;                                                              // 00462104 fld [0073D700]
   I r=width+y-1;                                                               // 00462146 (esi+[esp+18h]-1)
   for(I k=0;k<3;++k,++r){                                                      // [esp+24h]=3
    const I base_index=(r%height)*width;                                        // 00462150..0046215E
    sum+=static_cast<double>(cell(c->current,c0+base_index))*static_cast<double>(kernel[3*k]);   // 00462163..0046216F
    sum+=static_cast<double>(cell(c->current,c1+base_index))*static_cast<double>(kernel[3*k+1]); // 00462171..0046217E
    sum+=static_cast<double>(cell(c->current,c2i+base_index))*static_cast<double>(kernel[3*k+2]); // 00462180..0046218F
   }
   const float value=static_cast<float>(sum-static_cast<double>(old));          // 00462193..004621A3
   std::memcpy(reinterpret_cast<void*>(target),&value,4);
   offset+=4;++column;                                                          // 004621A9..004621AC
  }
 }
 {const U t=c->current;c->current=c->next;c->next=t;}                           // 004621E6..004621EF swap
 if(enable_random_impulses_0078e554!=0){                                        // 004621F2..004621F9
  const I ry=crt_rand_006fc447()%height;                                                // 004621FB..00462203
  const I rx=crt_rand_006fc447()%width;                                                 // 00462205..0046220D
  const I r=crt_rand_006fc447()%32;                                                     // 00462211..00462221
  const I index=ry*width+rx;                                                    // 00462222..00462229
  set_cell(c->next,index,static_cast<float>(static_cast<double>(r)+static_cast<double>(cell(c->next,index)))); // 0046222D..00462239
 }
 const double low=static_cast<double>(as_float(0x42200000u));                   // [00741BF4] 40.0f
 const double high=255.0-low;                                                   // 0046225C..00462267 / 00462341..00462347
 for(I y=0;y<height;++y){                                                       // 0046223B..004623AF
  for(I x=0;x<width;++x){                                                       // groups of 4 (0046226D..00462331) + tail (00462356..00462387)
   const I index=y*width+x;
   double v=static_cast<double>(cell(c->current,index));
   if(v<low)v=low;                                                              // fcom low; test ah,5; jp (>= or NaN keeps)
   if(v>high)v=high;                                                            // fcom st(1); test ah,41h; jne (<=, NaN keeps)
   set_cell(c->current,index,static_cast<float>(v));
  }
 }
 std::uint8_t* const out=static_cast<std::uint8_t*>(live_operator_new(static_cast<U>(height)*static_cast<U>(width))); // 004623B9..004623C4 (never freed)
 for(I y=0;y<height;++y)                                                        // 004623CB..004624C8
  for(I x=0;x<width;++x)out[y*width+x]=static_cast<std::uint8_t>(ftol(static_cast<double>(cell(c->current,y*width+x)))); // 00462407..00462492
 font::Texture* const owner=c->base.owner;
 (void)texture_vtable::head(owner)->common.s1c(owner,nullptr);                  // 004624D2..004624D9 (result unused)
 bool owned=false;
 std::uint8_t* result;
 if(c->base.f0c!=0)result=texture_controller::table(&c->base)->s14(&c->base,nullptr,out); // 004624DC..004624E9
 else{owned=true;result=texture_controller_height2channel_00461060(&c->base,nullptr,out,nullptr);} // 004624EE..004624F3
 {font::Texture* const o=c->base.owner;(void)texture_vtable::head(o)->common.sac(o,nullptr,1,address(result));} // 004624F8..00462505
 if(owned)live_free(result);                                                    // 0046250B..00462510
}
}
namespace texture_controller {
const TextureControllerVTable wave_vtable_00741d80={
 &wave_controller_delete_00463290,&wave_controller_control_00462030,&wave_controller_init_00460000,
 &texture_controller_parse_field_00462b00,&texture_controller_is_linear_0041f250,&texture_controller_distort_00460d70,
 &texture_controller_set_image_0045fd70};
}
