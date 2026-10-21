// controller_life_random_ring.cpp - Life (0073ED54, 0x3C), Random (0073ED90, 0x40), RingDistort (0073EDAC, 0x40).
// Created by 00422390 via base ctor 00462A70 + table store (no derived ctor). Evidence build/ctl/*.txt.
#include "49aa053254-controller_math.hpp"
#include "59377d41d2-texture_cells.hpp"
#include <cstdlib>
using namespace controller_math;
using texture_controller::TextureController;

// Ring distortion tuning cells (.data, no writer in the image): amplitude/frequency/phaseScale/phaseSpeed.
float ring_amplitude_0078e560=0.0299999993f;   // .data 3CF5C28F
float ring_frequency_0078e564=60.0f;           // .data 42700000
float ring_phase_scale_0078e568=25.0f;         // .data 41C80000
float ring_phase_speed_0078e56c=1.0f;          // .data 3F800000

extern "C" int __cdecl crt_rand_006fc447();   // game CRT rand 006FC447 (shared holdrand stream; 0479-3d-model-model-lifetime)
namespace {
struct LifeTextureController {TextureController base; U volatile cells;};                      // +38 state image
struct RandomTextureController {TextureController base; U volatile noise; U volatile channels;}; // +38 noise, +3C channel image
struct RingDistortTextureController {TextureController base; U volatile image; float phase;};    // +38 image, +3C phase
static_assert(sizeof(LifeTextureController)==0x3c&&sizeof(RandomTextureController)==0x40&&sizeof(RingDistortTextureController)==0x40,"sizes (00422453/004225C0/0042262A NewBase)");
template<class T> T* as(void* p){return static_cast<T*>(p);}
// Owner glImage (+AC) with (update, image) stack words (RET8).
void owner_gl_image(TextureController* c,U update,U image){font::Texture* const o=c->owner;(void)texture_vtable::head(o)->common.sac(o,nullptr,update,image);}
// CRT rand 006FC447 (holdrand*214013+2531011 >> 16 & 7FFF, per thread). Modern CRT rand: same algorithm/state model.
I crt_rand(){return crt_rand_006fc447();}
}

extern "C" {
// ---------------- Life ----------------
// 0041F2B0 dtor: table; free +38; base dtor.
void __fastcall life_controller_dtor_0041f2b0(void* p,void*){
 LifeTextureController* const c=as<LifeTextureController>(p);
 c->base.vtable=&texture_controller::life_vtable_0073ed54;                      // 0041F2CD
 live_free(reinterpret_cast<void*>(static_cast<U>(c->cells)));                 // 0041F2D3..0041F2DF (no null test)
 texture_controller_dtor_00460cf0(p,nullptr);                                   // 0041F2F1
}
void* __fastcall life_controller_delete_0041f290(void* p,void*,texture::U flags){
 life_controller_dtor_0041f2b0(p,nullptr);if(flags&1u)live_free(p);return p;    // 0041F293..0041F2A8
}
// 00460120 Init: random live cells written to the owner image and to +38.
void __fastcall life_controller_init_00460120(void* p,void*){
 LifeTextureController* const c=as<LifeTextureController>(p);
 std::uint8_t pixel[4]={0,0,0,0xff};                                            // 00460132..00460141
 const I width=owner_width(&c->base);                                           // 00460146
 const I rows=owner_height(&c->base);                                           // 00460154
 const I bpp=owner_bpp(&c->base);                                               // 0046015E
 const U image=address(owner_image(&c->base));                                  // 00460168
 c->cells=address(live_operator_new(static_cast<U>(bpp)*static_cast<U>(rows)*static_cast<U>(width))); // 0046016F..00460182
 if(rows<=0)return;
 U row_offset=0;                                                                // [esp+14h]
 for(I row=rows;row!=0;--row){                                                  // [esp+1Ch] countdown
  if(width>0){
   U offset=row_offset;
   for(I col=width;col!=0;--col){                                               // [esp+18h] countdown
    pixel[1]=0;pixel[2]=0;pixel[3]=0xff;pixel[0]=0;                             // 004601AC..004601C0
    if(crt_rand()%100>60)pixel[0]=0xff;                                         // 004601C5..004601D7
    rep_copy(image+offset,address(pixel),static_cast<U>(bpp));                  // 004601DC..004601F9
    rep_copy(c->cells+offset,address(pixel),static_cast<U>(bpp));               // 004601FB..00460217
    offset+=static_cast<U>(bpp);                                                // 00460214
   }
  }
  row_offset+=static_cast<U>(bpp)*static_cast<U>(width);                         // 0046022F
 }
}
// 00460250 Control: every 5 frames, one Life generation on +38 (toroidal 3x3), upload, FreeImage.
void __fastcall life_controller_control_00460250(void* p,void*){
 LifeTextureController* const c=as<LifeTextureController>(p);
 const U frame=texture_cells::texture_current_frame_007a46f4;                   // 00460253
 if(c->base.f08>static_cast<I>(frame-5u))return;                                // 0046025B..00460267 (signed)
 c->base.f08=static_cast<I>(frame);                                             // 00460272
 std::uint8_t pixel[4]={0,0,0,0xff};                                            // 00460278..00460287 (persists across cells)
 const I width=owner_width(&c->base);                                           // 0046028C
 const I rows=owner_height(&c->base);                                           // 00460296
 const I bpp=owner_bpp(&c->base);                                               // 004602A2
 const U size=static_cast<U>(bpp)*static_cast<U>(rows)*static_cast<U>(width);   // 004602A7..004602AE
 const U old=address(live_operator_new(size));                                  // 004602B6
 rep_copy(old,c->cells,size);                                                   // 004602BB..004602D2
 U row_offset=0;                                                                // [esp+38h]
 for(I y=0;y<rows;++y){                                                         // 004602DD / 004604F0
  if(width>0){
   U out=row_offset;                                                            // [esp+2Ch]
   I x2=width+2;                                                                // [esp+1Ch] = col+width+2
   for(I remaining=width;remaining!=0;--remaining){                             // [esp+3Ch]
    I count=0,alive=0,dy=-1;                                                    // 00460311..0046031B
    for(;dy<=1;++dy){                                                           // 004603F1..004603FD
     const I row_wrap=(dy+y+rows)%rows;                                         // 00460330..0046033D idiv
     const I row_base=row_wrap*width;                                           // 00460344
     for(I dx=-1;dx<=1;++dx){                                                   // unrolled 00460347..004603EB
      const I col_wrap=(x2+dx-2)%width;                                         // 0046034B..00460350 / 00460385 / 004603B8
      const U index=static_cast<U>(col_wrap+row_base)*static_cast<U>(bpp);      // 00460352..00460354
      if(dx==0&&dy==0)alive=bytes(old)[index];                                  // 00460357..0046036B
      else if(bytes(old)[index]>0)++count;                                      // 00460375..0046037D
     }
    }
    const U current=c->cells;                                                   // [esp+30h]->+38 (reloaded)
    if((alive!=0&&count==2)||count==3){                                         // 00460403..0046041D
     I v=0;
     if(bpp>2){v=bytes(current)[out+static_cast<U>(dy)]+4;if(v>0xff)v=0xff;}   // 00460421..00460443 ([esp+18h]=dy=2)
     pixel[0]=0xff;pixel[1]=static_cast<std::uint8_t>(v);pixel[2]=static_cast<std::uint8_t>(v); // 00460448..00460490
    }else{
     const U at=current+out;                                                    // 0046045E
     pixel[0]=0;pixel[1]=0;                                                     // 00460464/00460469
     if(bytes(at)[0]!=0)pixel[2]=0x80;                                          // 00460461..00460470
     else if(bpp>2){const I v=bytes(at)[2];pixel[2]=static_cast<std::uint8_t>(v-4<0?0:v-4);} // 00460477..00460490
    }
    rep_copy(c->cells+out,address(pixel),static_cast<U>(bpp));                  // 00460494..004604B5
    out+=static_cast<U>(bpp);                                                   // 004604B3
    ++x2;                                                                       // 004604C7
   }
  }
  row_offset+=static_cast<U>(bpp)*static_cast<U>(width);                         // 004604E4
 }
 (void)owner_image(&c->base);                                                   // 004604FF slot +3C (result unused)
 const U copy=address(live_operator_new(size));                                 // 00460507 (never freed: original leak)
 rep_copy(copy,c->cells,size);                                                  // 0046050C..00460522
 owner_gl_image(&c->base,1,0);                                                  // 00460529..0046052D glImage(1,0)
 // 00460533..00460538 owner [vtable+B0] FreeImage; the owner of a controller is the Basic whose ParseField
 // created it (00422390), whose Ref prefix is its first member (pointer-interconvertible).
 texture::texture_call_free_b0(reinterpret_cast<texture::Basic*>(static_cast<font::Texture*>(c->base.owner)));
 live_free(reinterpret_cast<void*>(old));                                       // 0046053E..00460543
}

// ---------------- Random ----------------
void __fastcall random_controller_dtor_0041f3f0(void* p,void*){
 RandomTextureController* const c=as<RandomTextureController>(p);
 c->base.vtable=&texture_controller::random_vtable_0073ed90;                    // 0041F40D
 live_free(reinterpret_cast<void*>(static_cast<U>(c->noise)));                 // 0041F413..0041F41F
 live_free(reinterpret_cast<void*>(static_cast<U>(c->channels)));              // 0041F424..0041F428
 texture_controller_dtor_00460cf0(p,nullptr);                                   // 0041F43A
}
void* __fastcall random_controller_delete_0041f3d0(void* p,void*,texture::U flags){
 random_controller_dtor_0041f3f0(p,nullptr);if(flags&1u)live_free(p);return p;
}
// 004609F0 Init: optional SetImage (distort), +38 = new(W*H), +3C = new(BPP*H*W).
void __fastcall random_controller_init_004609f0(void* p,void*){
 RandomTextureController* const c=as<RandomTextureController>(p);
 const I width=owner_width(&c->base);                                           // 004609FB
 const I rows=owner_height(&c->base);                                           // 00460A05
 const I bpp=owner_bpp(&c->base);                                               // 00460A0F
 if(c->base.f0c!=0)texture_controller::table(&c->base)->s18(&c->base,nullptr);  // 00460A14..00460A1F own +18
 void* const noise=live_operator_new(static_cast<U>(rows)*static_cast<U>(width)); // 00460A22..00460A28
 const U channels_size=static_cast<U>(bpp)*static_cast<U>(rows)*static_cast<U>(width); // 00460A2D..00460A30
 c->noise=address(noise);                                                       // 00460A34
 c->channels=address(live_operator_new(channels_size));                         // 00460A37..00460A40
}
// 00462800 Control: once per frame, fill +38 with rand()%256, then distort or Height2Channel, upload.
void __fastcall random_controller_control_00462800(void* p,void*){
 RandomTextureController* const c=as<RandomTextureController>(p);
 const U frame=texture_cells::texture_current_frame_007a46f4;                   // 00462801
 if(static_cast<U>(c->base.f08)==frame)return;                                  // 00462809
 c->base.f08=static_cast<I>(frame);                                             // 00462812
 if(c->channels==0)return;                                                      // 00462815..0046281A
 const I width=owner_width(&c->base);                                           // 00462826
 const I rows=owner_height(&c->base);                                           // 00462830
 if(rows>0){
  U row_offset=0;
  for(I row=rows;row!=0;--row){                                                 // [esp+10h] countdown
   for(I col=0;col<width;++col){                                                // 00462842..00462864
    const I r=crt_rand()%256;                                                   // 00462846..00462858 (and 800000FFh; signed fix-up)
    bytes(c->noise+row_offset)[col]=static_cast<std::uint8_t>(r);               // 00462859..0046285E (+38 reloaded)
   }
   row_offset+=static_cast<U>(width);                                           // 0046286A
  }
 }
 if(c->base.f0c!=0){                                                            // 00462875..0046287B
  std::uint8_t* const distorted=texture_controller::table(&c->base)->s14(&c->base,nullptr,bytes(c->noise)); // 0046287D..00462885
  owner_gl_image(&c->base,1,address(distorted));                                // 00462888..00462890
  return;
 }
 std::uint8_t* const result=texture_controller_height2channel_00461060(&c->base,nullptr,bytes(c->noise),bytes(c->channels)); // 00462899..004628A3
 c->channels=address(result);                                                   // 004628AC
 owner_gl_image(&c->base,1,address(result));                                    // 004628A8..004628B3
}

// ---------------- RingDistort ----------------
void __fastcall ring_controller_dtor_0041f470(void* p,void*){
 RingDistortTextureController* const c=as<RingDistortTextureController>(p);
 c->base.vtable=&texture_controller::ring_distort_vtable_0073edac;              // 0041F48D
 live_free(reinterpret_cast<void*>(static_cast<U>(c->image)));                 // 0041F493..0041F49F
 texture_controller_dtor_00460cf0(p,nullptr);                                   // 0041F4A7..
}
void* __fastcall ring_controller_delete_0041f450(void* p,void*,texture::U flags){
 ring_controller_dtor_0041f470(p,nullptr);if(flags&1u)live_free(p);return p;
}
// 00460A50 Init: phase=0, +38 = new(BPP*H*W).
void __fastcall ring_controller_init_00460a50(void* p,void*){
 RingDistortTextureController* const c=as<RingDistortTextureController>(p);
 const I width=owner_width(&c->base);                                           // 00460A5A
 const I rows=owner_height(&c->base);                                           // 00460A64
 const I bpp=owner_bpp(&c->base);                                               // 00460A6E
 std::memset(&c->phase,0,4);                                                    // 00460A78
 c->image=address(live_operator_new(static_cast<U>(bpp)*static_cast<U>(rows)*static_cast<U>(width))); // 00460A71..00460A88
}
namespace {
// 00460BA3..00460C3D clamp of v*255 to [0,255] (NaN passes through), result left for _ftol.
double clamp255(double v){
 const double k=static_cast<double>(as_float(0x437F0000u));
 if(!(k<v)){                                                                    // fcomp 255 vs v; test ah,5; jnp
  if(v<0.0)return 0.0;                                                          // 0 vs v; test ah,41h (0<v or 0==v or NaN keeps)
 }
 if(k<v)return k;                                                               // second 255 test; jp keeps (<=255 or NaN)
 return v;
}
}
// 00460A90 Control: once per frame, radial cosine rings in R,G (B=A=FF), upload; phase += deltaT*speed mod 2*pi.
void __fastcall ring_controller_control_00460a90(void* p,void*){
 RingDistortTextureController* const c=as<RingDistortTextureController>(p);
 const U frame=texture_cells::texture_current_frame_007a46f4;                   // 00460A90
 if(static_cast<U>(c->base.f08)==frame)return;                                  // 00460A9B
 if(c->image==0)return;                                                         // 00460AA4..00460AA9
 c->base.f08=static_cast<I>(frame);                                             // 00460AB4
 const I width=owner_width(&c->base);                                           // 00460ABA
 const I rows=owner_height(&c->base);                                           // 00460AC8
 (void)owner_bpp(&c->base);                                                     // 00460AD6 (result unused)
 const float inverse=static_cast<float>(1.0/static_cast<double>(width));        // 00460AD9..00460AEB fdivr [0073D764]
 U out=0;                                                                       // edi
 for(I y=0;y<rows;++y){                                                         // 00460AEF / 00460C87
  const double yc=(static_cast<double>(y)+1.0)*static_cast<double>(inverse);    // 00460AF5..00460B07
  if(width<=0)continue;                                                         // 00460B0B
  const float yy=static_cast<float>((yc-0.5)*(yc-0.5));                         // 00460B11..00460B1B
  for(I x=0;x<width;++x){                                                       // 00460B21 / 00460C6E
   const double xc=(static_cast<double>(x)+1.0)*static_cast<double>(inverse)-0.5; // 00460B21..00460B30
   const float r=static_cast<float>(std::sqrt(xc*xc+static_cast<double>(yy)));  // 00460B36..00460B40 fsqrt
   auto ring=[&](){
    const float arg=static_cast<float>(static_cast<double>(ring_phase_scale_0078e568)*static_cast<double>(c->phase)+
                                       static_cast<double>(ring_frequency_0078e564)*static_cast<double>(r)); // 00460B46..00460B5B
    return static_cast<float>(static_cast<double>(cosine_lut_004aadf0(arg))*static_cast<double>(ring_amplitude_0078e560)+0.5); // 00460B5E..00460B6F
   };
   const float a=ring();                                                        // [esp+20h]
   const float b=ring();                                                        // 00460B73..00460B9F (col slot reused)
   const double va=clamp255(static_cast<double>(a)*static_cast<double>(as_float(0x437F0000u))); // 00460BA3..00460BEA
   const double vb=clamp255(static_cast<double>(b)*static_cast<double>(as_float(0x437F0000u))); // 00460BF0..00460C37
   bytes(c->image)[out+2]=0xff;                                                 // 00460C40
   bytes(c->image)[out+1]=static_cast<std::uint8_t>(ftol(vb));                  // 00460C45..00460C4D
   bytes(c->image)[out+0]=static_cast<std::uint8_t>(ftol(va));                  // 00460C51..00460C59
   bytes(c->image)[out+3]=0xff;                                                 // 00460C5F
   out+=4;
  }
 }
 owner_gl_image(&c->base,1,c->image);                                           // 00460C8D..00460C98
 const double advanced=static_cast<double>(texture_cells::delta_t_0078e574)*static_cast<double>(ring_phase_speed_0078e56c)+static_cast<double>(c->phase); // 00460C9E..00460CAD
 c->phase=static_cast<float>(advanced);                                         // 00460CB0 fst
 const double two_pi=static_cast<double>(as_float(0x40C90FDBu));                // [00740C88]
 if(!(advanced>two_pi))return;                                                  // 00460CB3..00460CBE
 double phase=static_cast<double>(c->phase);                                    // 00460CC0 fld (rounded)
 do{phase-=two_pi;}while(phase>two_pi);                                         // 00460CD0..00460CE1
 c->phase=static_cast<float>(phase);                                            // 00460CE3
}
}

namespace texture_controller {
const TextureControllerVTable life_vtable_0073ed54={
 &life_controller_delete_0041f290,&life_controller_control_00460250,&life_controller_init_00460120,
 &texture_controller_parse_field_00462b00,&texture_controller_is_linear_0041f250,&texture_controller_distort_00460d70,
 &texture_controller_set_image_0045fd70};
const TextureControllerVTable random_vtable_0073ed90={
 &random_controller_delete_0041f3d0,&random_controller_control_00462800,&random_controller_init_004609f0,
 &texture_controller_parse_field_00462b00,&texture_controller_is_linear_0041f250,&texture_controller_distort_00460d70,
 &texture_controller_set_image_0045fd70};
const TextureControllerVTable ring_distort_vtable_0073edac={
 &ring_controller_delete_0041f450,&ring_controller_control_00460a90,&ring_controller_init_00460a50,
 &texture_controller_parse_field_00462b00,&texture_controller_return_true_005a4cf0,&texture_controller_distort_00460d70,
 &texture_controller_set_image_0045fd70};
}
