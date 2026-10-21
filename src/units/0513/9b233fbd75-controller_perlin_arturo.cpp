// controller_perlin_arturo.cpp - Perlin (0073ED70, 0x44) and Arturo (00741D9C, 0x54) controllers plus their image
// helpers ImageBuildPerlin 0045E6B0, ImageGetPerlinPixel 0045E710, ImageInterpolate 0045E5B0 and the Arturo grid
// sampler 004608A0. Evidence build/ctl/0045FD70.txt and build/full.disasm.txt.
#include "49aa053254-controller_math.hpp"
#include "59377d41d2-texture_cells.hpp"
#include <cstdlib>
using namespace controller_math;
using texture_controller::TextureController;

// Arturo globals (.data; only Arturo reads/writes them).
float texture_delta_t_007fb874=0.0f;     // textureDeltaT (BSS)
I volatile framemod_0078e570=-1;         // framemod (.data FFFFFFFF)
I volatile enable_random_impulses_0078e554=1; // enableRandomImpulses (.data 1)

extern "C" int __cdecl crt_rand_006fc447();   // game CRT rand 006FC447 (shared holdrand stream; 0479-3d-model-model-lifetime)
namespace {
struct PerlinTextureController {TextureController base; U volatile current; U volatile next; float blend;}; // +38,+3C,+40
struct ArturoTextureController {
 TextureController base;
 I grid_width;        // +38 "arturowidth"  (ctor 0Fh)
 I grid_height;       // +3C "arturoheight" (ctor 0Fh)
 U volatile current;  // +40 float grid
 U volatile previous; // +44 float grid
 U volatile older;    // +48 float grid
 float time;          // +4C
 U volatile image;    // +50 byte image W*H
};
static_assert(sizeof(PerlinTextureController)==0x44&&sizeof(ArturoTextureController)==0x54,"sizes (004224A0/004224ED)");
template<class T> T* as(void* p){return static_cast<T*>(p);}
I crt_rand(){return crt_rand_006fc447();}
void owner_gl_image(TextureController* c,U update,U image){font::Texture* const o=c->owner;(void)texture_vtable::head(o)->common.sac(o,nullptr,update,image);}
float grid(U base,I index){float f;std::memcpy(&f,reinterpret_cast<const void*>(base+static_cast<U>(index)*4u),4);return f;}
void set_grid(U base,I index,float v){std::memcpy(reinterpret_cast<void*>(base+static_cast<U>(index)*4u),&v,4);}

// 0045E6B0 ImageBuildPerlin(size, amplitude): size*size bytes of ftol((rand()%128-64)*amplitude+127).
std::uint8_t* image_build_perlin_0045e6b0(I size,float amplitude){
 const I count=size*size;                                                       // 0045E6B2..0045E6B6
 std::uint8_t* const out=static_cast<std::uint8_t*>(live_operator_new(static_cast<U>(count))); // 0045E6BB
 for(I i=0;i<count;++i){                                                        // 0045E6C5..0045E701
  const I r=crt_rand()%128-0x40;                                                // 0045E6D0..0045E6E1
  out[i]=static_cast<std::uint8_t>(ftol(static_cast<double>(r)*static_cast<double>(amplitude)+127.0)); // 0045E6E8..0045E6FB [0073EDDC]
 }
 return out;
}
// 0045E5B0 ImageInterpolate(image, x, y, w, h): bilinear byte sample, rint to AL.
std::uint8_t image_interpolate_0045e5b0(const std::uint8_t* image,float x,float y,I w,I h){
 const I ix=ftol(std::floor(static_cast<double>(x)));                           // 0045E5B3..0045E5C6 floor 006FC750 + _ftol
 const I iy=ftol(std::floor(static_cast<double>(y)));                           // 0045E5CB..0045E5E0
 const double fx=static_cast<double>(x)-static_cast<double>(ix);                // 0045E5E5..0045E5F2
 const I ix1=(ix+1)%w;                                                          // 0045E5EF..0045E5F7
 const double fy=static_cast<double>(y)-static_cast<double>(iy);                // 0045E600..0045E604
 const double gx=1.0-fx;                                                        // 0045E608..0045E60E
 const I iy1=(iy+1)%h;                                                          // 0045E5F9/0045E613
 const double p00=image[iy*w+ix],p01=image[iy*w+ix1],p10=image[iy1*w+ix],p11=image[iy1*w+ix1]; // 0045E617..0045E651
 const double a=p00*gx+p01*fx;                                                  // 0045E636..0045E65B
 const double b=p10*gx+p11*fx;                                                  // 0045E667..0045E67B
 const float v=static_cast<float>(a*(1.0-fy)+b*fy);                             // 0045E65D..0045E681
 return static_cast<std::uint8_t>(rint_i32(v));                                 // 0045E68B..0045E697
}
// 0045E710 ImageGetPerlinPixel(levels, row, col, size, count, total): sum of (sample-7F) over octaves, +7F.
I image_get_perlin_pixel_0045e710(std::uint8_t* const* levels,I row,I col,I size,I count,I total){
 const float rf=static_cast<float>(static_cast<double>(row)/static_cast<double>(total)); // 0045E710..0045E730
 const float cf=static_cast<float>(static_cast<double>(col)/static_cast<double>(total)); // 0045E734..0045E73A
 U sum=0;                                                                       // ebx
 I s=size;
 for(I i=0;i<count;++i){                                                        // 0045E72A..0045E77C
  const float y=static_cast<float>(static_cast<double>(cf)*static_cast<double>(s)); // 0045E742..0045E758 (arg3)
  const float x=static_cast<float>(static_cast<double>(s)*static_cast<double>(rf)); // 0045E75C..0045E760 (arg2)
  const std::uint8_t sample=image_interpolate_0045e5b0(levels[i],x,y,s,s);      // 0045E764
  s+=s;                                                                         // 0045E769
  sum=sum+sample-0x7fu;                                                         // 0045E774 lea ebx,[ebx+edx-7Fh]
 }
 return static_cast<I>(static_cast<std::uint8_t>(sum))+0x7f;                    // 0045E77E..0045E786 (full EAX)
}
// 004608A0 float-grid bilinear sample(grid, x, y, w, h) -> ST0 (also used by Water 00461EAD).
double arturo_sample_impl_004608a0(U grid_base,float x,float y,I w,I h){
 const float xs=static_cast<float>(static_cast<double>(x)-0.5);                 // 004608A3..004608B4 (arg cell)
 const I ix=rint_i32(xs);                                                       // 004608BD..004608C7
 const float ys=static_cast<float>(static_cast<double>(y)-0.5);                 // 004608C9..004608DB
 const I iy=rint_i32(ys);                                                       // 004608DF..004608E9
 const double fx=static_cast<double>(xs)-static_cast<double>(ix);               // 004608EF..004608FB
 const I ix1=(ix+1)%w;                                                          // 004608F7..004608FF
 const double fy=static_cast<double>(ys)-static_cast<double>(iy);               // 00460901..00460905
 const double gx=1.0-fx;                                                        // 00460909..0046090F
 const I iy1=(iy+1)%h;                                                          // 00460917..0046091D
 const double a=gx*static_cast<double>(grid(grid_base,iy*w+ix))+fx*static_cast<double>(grid(grid_base,iy*w+ix1)); // 00460934..00460943
 const double b=gx*static_cast<double>(grid(grid_base,iy1*w+ix))+fx*static_cast<double>(grid(grid_base,iy1*w+ix1)); // 00460951..0046095B
 return a*(1.0-fy)+b*fy;                                                        // 00460946..00460961
}
}
extern "C" double __cdecl texture_controller_grid_sample_004608a0(const void* grid,float x,float y,texture::I w,texture::I h){return arturo_sample_impl_004608a0(address(grid),x,y,w,h);}
namespace {
// 00460BA3.. style clamp (shared with RingDistort/Arturo): v<0 -> 0, v>255 -> 255, NaN kept.
double clamp255(double v){const double k=255.0;if(!(k<v)&&v<0.0)return 0.0;if(k<v)return k;return v;}
}

extern "C" {
// ---------------- Perlin ----------------
void __fastcall perlin_controller_dtor_0041f350(void* p,void*){
 PerlinTextureController* const c=as<PerlinTextureController>(p);
 c->base.vtable=&texture_controller::perlin_vtable_0073ed70.base;               // 0041F36D
 live_free(reinterpret_cast<void*>(static_cast<U>(c->current)));               // 0041F373..0041F37F
 live_free(reinterpret_cast<void*>(static_cast<U>(c->next)));                  // 0041F384..0041F388
 texture_controller_dtor_00460cf0(p,nullptr);                                   // 0041F39A
}
void* __fastcall perlin_controller_delete_0041f330(void* p,void*,texture::U flags){
 perlin_controller_dtor_0041f350(p,nullptr);if(flags&1u)live_free(p);return p;
}
// 0045FE50 BuildPerlinImage(image, size) RET8.
void __fastcall perlin_controller_build_0045fe50(void*,void*,std::uint8_t* image,texture::I size){
 I s=4;float amplitude=1.0f;I levels_count=1;                                   // 0045FE5B..0045FE68
 if(size>=8){do{++levels_count;}while((4<<levels_count)<=size);}                // 0045FE6D..0045FE7C
 std::uint8_t** const levels=static_cast<std::uint8_t**>(live_operator_new(static_cast<U>(levels_count)*4u)); // 0045FE7E..0045FE87 (arg cell reused)
 for(I i=0;i<levels_count;++i){                                                 // 0045FE91..0045FEC8
  levels[i]=image_build_perlin_0045e6b0(s,amplitude);                           // 0045FEA0..0045FEBC
  amplitude=static_cast<float>(static_cast<double>(amplitude)*0.5);             // 0045FEAB..0045FEC0
  s+=s;                                                                         // 0045FEC4
 }
 std::uint8_t* row=image;
 for(I r=0;r<size;++r){                                                         // 0045FECC..0045FF08
  for(I col=0;col<size;++col)                                                   // 0045FED8..0045FEFD
   row[col]=static_cast<std::uint8_t>(image_get_perlin_pixel_0045e710(levels,r,col,4,levels_count,size)); // 0045FEE0..0045FEF7
  row+=size;                                                                    // 0045FF00
 }
 for(I i=0;i<levels_count;++i)live_free(levels[i]);                             // 0045FF0A..0045FF24
 live_free(levels);                                                             // 0045FF26..0045FF2B
}
// 0045FDD0 Init: optional SetImage; +40=0; +38/+3C = new(H*W); Build each with width.
void __fastcall perlin_controller_init_0045fdd0(void* p,void*){
 PerlinTextureController* const c=as<PerlinTextureController>(p);
 if(c->base.f0c!=0)texture_controller::table(&c->base)->s18(&c->base,nullptr); // 0045FDD5..0045FDDF
 font::Texture* const owner=c->base.owner;                                      // 0045FDE2
 std::memset(&c->blend,0,4);                                                    // 0045FDE7
 I size=texture_vtable::head(owner)->common.s4c(owner,nullptr);                 // 0045FDF4
 size*=texture_vtable::head(owner)->common.s48(owner,nullptr);                  // 0045FDFD
 c->current=address(live_operator_new(static_cast<U>(size)));                   // 0045FE03..0045FE0A
 c->next=address(live_operator_new(static_cast<U>(size)));                      // 0045FE0D..0045FE17
 {const texture_controller::PerlinVTable* const t=texture_controller::perlin_table(&c->base); // 0045FE15
  t->s1c(&c->base,nullptr,bytes(c->current),owner_width(&c->base));}            // 0045FE12..0045FE29
 {const texture_controller::PerlinVTable* const t=texture_controller::perlin_table(&c->base); // 0045FE31
  t->s1c(&c->base,nullptr,bytes(c->next),owner_width(&c->base));}               // 0045FE2C..0045FE3D
}
// 004615A0 Control: blend current/next by +40 (rate 4*deltaT), regenerate on wrap, distort or Height2Channel, upload.
void __fastcall perlin_controller_control_004615a0(void* p,void*){
 PerlinTextureController* const c=as<PerlinTextureController>(p);
 const U frame=texture_cells::texture_current_frame_007a46f4;                   // 004615A0
 if(static_cast<U>(c->base.f08)==frame)return;                                  // 004615AB
 c->base.f08=static_cast<I>(frame);                                             // 004615B5
 font::Texture* const owner=c->base.owner;                                      // 004615B8
 I count=texture_vtable::head(owner)->common.s48(owner,nullptr);                // 004615C7
 count*=texture_vtable::head(owner)->common.s48(owner,nullptr);                 // 004615D0..004615D9 (width*width)
 const double advanced=static_cast<double>(as_float(0x40800000u))*static_cast<double>(texture_cells::delta_t_0078e574)+static_cast<double>(c->blend); // 004615D3..004615E2 [00741BE4]
 c->blend=static_cast<float>(advanced);                                         // 004615E5 fst
 if(advanced>1.0){                                                              // 004615E8..004615F3
  rep_copy(c->current,c->next,static_cast<U>(count));                           // 004615F5..00461609
  const texture_controller::PerlinVTable* const t=texture_controller::perlin_table(&c->base); // 00461610
  t->s1c(&c->base,nullptr,bytes(c->next),owner_width(&c->base));                // 0046160B..0046161D
  if(static_cast<double>(c->blend)>=1.0){                                       // 00461620..0046162E (test ah,1: <1 or NaN skips)
   double b=static_cast<double>(c->blend);                                      // 00461630
   do{b-=1.0;}while(b>=1.0);                                                    // 00461640..00461651 (NaN exits)
   c->blend=static_cast<float>(b);                                              // 00461653
  }
 }
 std::uint8_t* const mixed=static_cast<std::uint8_t*>(live_operator_new(static_cast<U>(count))); // 00461656..00461664
 auto mix=[&](I i){
  const double a=bytes(c->current)[i];const double b=bytes(c->next)[i];
  return static_cast<std::uint8_t>(ftol(a*(1.0-static_cast<double>(c->blend))+b*static_cast<double>(c->blend))); // 0046167C..004616AA
 };
 I i=0;
 if(count>=4){                                                                  // 00461661..00461668
  do{
   mixed[i]=mix(i);mixed[i+1]=mix(i+1);mixed[i+2]=mix(i+2);mixed[i+3]=mix(i+3); // 0046167C..00461756
   i+=4;
  }while(i+3<count);                                                            // 00461759..00461767
 }
 for(;i<count;++i)mixed[i]=mix(i);                                              // 0046176D..004617AE
 (void)owner_image(&c->base);                                                   // 004617B0..004617B5 (result unused)
 bool owned=false;                                                              // bl
 std::uint8_t* result;
 if(c->base.f0c!=0)result=texture_controller::table(&c->base)->s14(&c->base,nullptr,mixed); // 004617B8..004617CB
 else{owned=true;result=texture_controller_height2channel_00461060(&c->base,nullptr,mixed,nullptr);} // 004617D0..004617DB
 live_free(mixed);                                                              // 004617E0..004617E7
 owner_gl_image(&c->base,1,address(result));                                    // 004617EC..004617FA (owner reloaded)
 if(owned)live_free(result);                                                    // 00461800..00461805
}

// ---------------- Arturo ----------------
void* __fastcall arturo_controller_ctor_00463320(void* p,void*,font::Texture* owner){
 texture_controller_ctor_00462a70(p,nullptr,owner);                             // 00463328
 ArturoTextureController* const c=as<ArturoTextureController>(p);
 c->grid_width=0x0f;c->grid_height=0x0f;                                        // 0046332D..00463335
 c->image=0;c->current=0;c->previous=0;c->older=0;                              // 0046333A..00463343
 c->base.vtable=&texture_controller::arturo_vtable_00741d9c.base;               // 00463346
 c->base.f08=-1;                                                                // 0046334C
 return c;
}
void __fastcall arturo_controller_dtor_00463380(void* p,void*){
 ArturoTextureController* const c=as<ArturoTextureController>(p);
 c->base.vtable=&texture_controller::arturo_vtable_00741d9c.base;               // 0046339D
 live_free(reinterpret_cast<void*>(static_cast<U>(c->previous)));              // 004633A3
 live_free(reinterpret_cast<void*>(static_cast<U>(c->older)));                 // 004633B4
 live_free(reinterpret_cast<void*>(static_cast<U>(c->current)));               // 004633BD
 live_free(reinterpret_cast<void*>(static_cast<U>(c->image)));                 // 004633C6
 texture_controller_dtor_00460cf0(p,nullptr);                                   // 004633DC
}
void* __fastcall arturo_controller_delete_00463360(void* p,void*,texture::U flags){
 arturo_controller_dtor_00463380(p,nullptr);if(flags&1u)live_free(p);return p;
}
// 00463400 ParseField: base first, then "arturowidth"/"arturoheight".
void __fastcall arturo_controller_parse_field_00463400(void* p,void*,const char* text){
 ArturoTextureController* const c=as<ArturoTextureController>(p);
 char* const line=const_cast<char*>(text);
 texture_controller_parse_field_00462b00(p,nullptr,text);                       // 00463409
 if(kotor_stricmp_0070acaf("arturowidth",firstword_00463530(line))==0)parse_int_004635e0(line+0x0b,&c->grid_width);   // 0046340E..0046342E
 if(kotor_stricmp_0070acaf("arturoheight",firstword_00463530(line))==0)parse_int_004635e0(line+0x0c,&c->grid_height); // 00463436..00463456
}
// 00460560 Init: grids/image, radial sine seed in +44, then BuildArturoImage twice (call + tail jump).
void __fastcall arturo_controller_init_00460560(void* p,void*){
 ArturoTextureController* const c=as<ArturoTextureController>(p);
 const I width=owner_width(&c->base);                                           // 0046056D
 const I pixels=owner_height(&c->base)*width;                                   // 00460577..0046057A
 const I cells=c->grid_height*c->grid_width;                                    // 0046057D..00460580
 std::memset(&c->time,0,4);                                                     // 0046058B
 if(c->base.f0c!=0)texture_controller::table(&c->base)->s18(&c->base,nullptr); // 00460586..00460598
 const U grid_bytes=static_cast<U>(cells)<<2;                                   // 0046059B
 c->previous=address(live_operator_new(grid_bytes));                            // 0046059E..004605A5
 c->older=address(live_operator_new(grid_bytes));                               // 004605A8..004605AE
 c->current=address(live_operator_new(grid_bytes));                             // 004605B1..004605B7
 c->image=address(live_operator_new(static_cast<U>(pixels)));                   // 004605BA..004605BF
 for(I y=0;y<c->grid_height;++y){                                               // 004605C2..0046066D
  if(!(c->grid_width>0))continue;
  const double yd=static_cast<double>(y);                                       // 004605E7 fild
  for(I x=0;x<c->grid_width;){                                                  // 004605F0..0046065F
   const I gw=c->grid_width;
   const double u=static_cast<double>(x)/static_cast<double>(gw)*2.0-1.0;       // 004605F3..00460609 (fadd st,st)
   set_grid(c->older,gw*y+x,0.0f);                                              // 00460612 [+48]=0
   const I index=c->grid_width*y+x;                                             // 00460619..00460624
   const double v=yd/static_cast<double>(c->grid_height)*2.0-1.0;               // 0046060F..0046062D
   ++x;
   const double r=std::sqrt(v*v+u*u);                                           // 00460633..0046063D
   const double argument=(1.0-r)*static_cast<double>(as_float(0x40490FDBu));    // 00460643..00460649 [00741BBC] pi
   set_grid(c->previous,index,x87_sin_mul_store(argument,127.0f));              // 0046064F..00460657 fsin, *[0073EDDC]
  }
 }
 const texture_controller::ArturoVTable* t=texture_controller::arturo_table(&c->base);
 t->s1c(&c->base,nullptr);                                                      // 00460673..00460677
 t=texture_controller::arturo_table(&c->base);
 t->s1c(&c->base,nullptr);                                                      // 0046067A..00460684 jmp [eax+1Ch]
}
// 00460690 BuildArturoImage: wave step on the float grids with an optional random impulse.
void __fastcall arturo_controller_build_00460690(void* p,void*){
 ArturoTextureController* const c=as<ArturoTextureController>(p);
 const I gh=c->grid_height,gw=c->grid_width;                                    // 00460697/0046069B
 const I cells=gh*gw;                                                           // 0046069F..004606A1
 const I rx=crt_rand()%gw;                                                      // 004606B0..004606B8
 const I ry=crt_rand()%gh;                                                      // 004606BC..004606CE
 const double k=static_cast<double>(as_float(0x00000000u));                     // [00741C04] (0.0f)
 for(I y=0;y<gh;++y){                                                           // 004606C8..0046085B
  if(gw<=0)continue;                                                            // 004606E4..004606EA
  const I below=((y+1)%gh)*gw;                                                  // 004606F0..00460708 (ebp)
  const I above=((gh+y+1-2)%gh)*gw;                                             // 004606F8..00460710 (ebx)
  const I row=y*gw;                                                             // [esp+14h]
  for(I x=0;x<gw;){                                                             // 00460713..00460823
   const I x1=x+1;
   double v=static_cast<double>(grid(c->previous,(gw+x1-2)%gw+row));            // 0046071C..00460730
   v+=static_cast<double>(grid(c->previous,x1%gw+row));                         // 00460733..0046073E
   v+=static_cast<double>(grid(c->previous,above+x));                           // 00460748
   v+=static_cast<double>(grid(c->previous,below+x));                           // 0046074B
   v=v*0.5-static_cast<double>(grid(c->older,row+x));                           // 0046074E..00460754
   v=v*(1.0-k)+static_cast<double>(grid(c->previous,row+x))*k;                  // 00460757..00460775
   const float stored=static_cast<float>(v);                                    // 00460777 fst [esp+30h]
   if(enable_random_impulses_0078e554!=0&&y==ry&&x==rx){                        // 0046075D..0046078F
    const I r=crt_rand()%256-0x80;                                              // 00460793..004607A6
    v=static_cast<double>(r)+static_cast<double>(stored);                       // 004607AF..004607B3
   }
   v=clamp255(v+127.0)-127.0;                                                   // 004607B7..00460804
   set_grid(c->current,row+x,static_cast<float>(v));                            // 0046080A..00460811
   x=x1;
  }
 }
 const U grid_bytes=static_cast<U>(cells)*4u;                                   // 00460864
 rep_copy(c->older,c->previous,grid_bytes);                                     // 00460861..0046087C
 rep_copy(c->previous,c->current,grid_bytes);                                   // 0046087E..00460890
}
// 00462520 Control: framed by textureDeltaT/framemod; three-phase resample of the grids into +50, upload.
void __fastcall arturo_controller_control_00462520(void* p,void*){
 ArturoTextureController* const c=as<ArturoTextureController>(p);
 const double period=static_cast<double>(as_float(0x3CF5C28Fu));                // [00741C00] 0.03
 const double accumulated=static_cast<double>(texture_cells::delta_t_0078e574)+static_cast<double>(texture_delta_t_007fb874); // 00462523..0046252D
 texture_delta_t_007fb874=static_cast<float>(accumulated);                      // 00462533 fst
 if(!(accumulated>period))return;                                               // 00462539..00462544
 const double stored=static_cast<double>(texture_delta_t_007fb874);             // 0046254A
 const double whole=std::floor(stored/period)*period;                           // 00462553..00462561
 const U frame=texture_cells::texture_current_frame_007a46f4;                   // 0046256B
 texture_delta_t_007fb874=static_cast<float>(stored-whole);                     // 00462574..0046257F
 if(static_cast<U>(c->base.f08)==frame)return;                                  // 0046257D..00462585
 c->base.f08=static_cast<I>(frame);                                             // 0046258B
 const I phase=(framemod_0078e570+1)%3;                                         // 0046258E..0046259A
 framemod_0078e570=phase;                                                       // 004625A0
 const I width=owner_width(&c->base);                                           // 004625AF
 const I height=owner_height(&c->base);                                         // 004625C1
 I current_phase=framemod_0078e570;                                             // 004625C4
 if(current_phase==0){
  c->time=static_cast<float>(static_cast<double>(texture_cells::delta_t_0078e574)*static_cast<double>(c->base.f18)*1.5+static_cast<double>(c->time)); // 004625D8..004625EA [0073F414]
  if(static_cast<double>(c->time)>1.0){                                         // 004625F1..004625FF
   texture_controller::arturo_table(&c->base)->s1c(&c->base,nullptr);           // 00462601..00462603
   while(static_cast<double>(c->time)>1.0)c->time=static_cast<float>(static_cast<double>(c->time)-1.0); // 0046260A..0046263E
   current_phase=framemod_0078e570;                                             // 00462640
  }
 }
 if(c->base.f0c!=0&&!(current_phase>=2&&current_phase!=0)){                     // 0046264C..0046265E
  const float gx=static_cast<float>(static_cast<double>(c->grid_width)/static_cast<double>(width));   // 00462664..00462674
  const I half=height/2;                                                        // 00462667..00462670
  const I first=half*current_phase;                                             // 00462672..00462678
  const float gy=static_cast<float>(static_cast<double>(c->grid_height)/static_cast<double>(height)); // 0046267B..00462689
  const I last=current_phase==1?height:(current_phase+1)*half;                  // 00462682..00462696
  float v=static_cast<float>(static_cast<double>(first)*static_cast<double>(gy)); // 0046269B..004626AB
  if(first<last){                                                               // 004626AF
   U out=static_cast<U>(first)*static_cast<U>(width);                           // 004626B5..004626B7
   for(I rows=last-first;rows!=0;--rows){                                       // [esp+28h]
    float u=0.0f;                                                               // [esp+14h]
    for(I col=0;col<width;++col){                                               // 004626C2..00462760
     const float a=static_cast<float>(texture_controller_grid_sample_004608a0(bytes(c->older),u,v,c->grid_width,c->grid_height)); // 004626D6..004626ED
     const double b=texture_controller_grid_sample_004608a0(bytes(c->previous),u,v,c->grid_width,c->grid_height); // 004626F1..00462703
     const float mixed=static_cast<float>((1.0-static_cast<double>(c->time))*static_cast<double>(a)+b*static_cast<double>(c->time)+127.0); // 00462708..00462729
     bytes(c->image)[out+static_cast<U>(col)]=static_cast<std::uint8_t>(rint_i32(mixed)); // 0046272D..0046274E
     u=static_cast<float>(static_cast<double>(u)+static_cast<double>(gx));      // 0046273D..00462751
    }
    v=static_cast<float>(static_cast<double>(v)+static_cast<double>(gy));       // 0046276E..00462785
    out+=static_cast<U>(width);                                                 // 0046277A
   }
  }
  current_phase=framemod_0078e570;                                              // 00462766
 }
 if(current_phase!=2)return;                                                    // 00462793..00462796
 U result=c->image;                                                             // 00462798
 bool owned=false;
 font::Texture* const owner=c->base.owner;                                      // 0046279B
 if(static_cast<std::uint8_t>(texture_vtable::head(owner)->common.s1c(owner,nullptr))!=1){ // 004627A2..004627A7 IsBumpMap AL
  if(c->base.f0c!=0)result=address(texture_controller::table(&c->base)->s14(&c->base,nullptr,bytes(c->image))); // 004627AD..004627BA
  else{owned=true;result=address(texture_controller_height2channel_00461060(&c->base,nullptr,bytes(c->image),nullptr));} // 004627BF..004627C7
 }
 owner_gl_image(&c->base,1,result);                                             // 004627CE..004627DA
 if(owned)live_free(reinterpret_cast<void*>(result));                           // 004627E0..004627E5
}
}

namespace texture_controller {
const PerlinVTable perlin_vtable_0073ed70={{
 &perlin_controller_delete_0041f330,&perlin_controller_control_004615a0,&perlin_controller_init_0045fdd0,
 &texture_controller_parse_field_00462b00,&texture_controller_is_linear_0041f250,&texture_controller_distort_00460d70,
 &texture_controller_set_image_0045fd70},&perlin_controller_build_0045fe50};
const ArturoVTable arturo_vtable_00741d9c={{
 &arturo_controller_delete_00463360,&arturo_controller_control_00462520,&arturo_controller_init_00460560,
 &arturo_controller_parse_field_00463400,&texture_controller_is_linear_0041f250,&texture_controller_distort_00460d70,
 &texture_controller_set_image_0045fd70},&arturo_controller_build_00460690};
}
