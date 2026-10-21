// controller_base.cpp - lane-texture-epoch PROPOSAL: TextureController base (vtable 00741C08) and shared slots.
// Evidence: build/ctl/0045FD70.txt (dumpbin of 0045FD70..00463510), build/ctl/0041F250.txt; addresses cited inline.
#include "c3a194d97e-controller_math.hpp"
#include "25d8ff45ea-controller_rdata.h"   // D13 local generated header (tools/gen_controller_rdata.py): LUT tables

using namespace controller_math;
using texture_controller::TextureController;
namespace {
TextureController* self_of(void* p){return static_cast<TextureController*>(p);}
}  // namespace
// M48 composer (single owner 004628C0): this body is the sole provider; external C linkage under the name bound by
// lane-m48-asciimdl (mdl_parse_helpers/mdl_parse_fields, asciimdl.hpp:96; same List type kotor_reconstruction::ArrayMutations).
// CExoArrayList<float> read "%d %c" header + values (004628C0, cdecl(text,list), RET0).
extern "C" void __cdecl mdl_parse_float_list_004628c0(char* text,texture::List* list);
extern "C" void __cdecl mdl_parse_float_list_004628c0(char* text,texture::List* list){
 I count=0;char separator;                                                     // [esp+18h]=0 at 004628DB
 const int fields=kotor_sscanf_006fae08(text,"%d %c",&count,&separator);       // 004628D5 fmt 0073EE54
 if(fields>0){
  const I n=count;                                                              // 004628F5
  if(n==0){                                                                     // 00462901
   const U data=list->data;
   if(data){live_free(reinterpret_cast<void*>(data));list->data=static_cast<U>(n);} // 00462909..00462916 (stores 0)
   list->capacity=0;                                                            // 00462918
  }else{
   kotor_renderer_gl::allocate_00437d60(list,nullptr,static_cast<U>(n));        // 00462924
  }
  list->size=n;                                                                 // 00462930
  if(fields==1){                                                                // 0046292D: one value per following line
   for(I i=0;i<n;){                                                             // 00462937 jle / 00462962 jl (live size)
    char* const line=resource_next_0044bfa0();                                  // 00462940
    if(!line)return;                                                            // 00462947
    parse_float_004635c0(line,reinterpret_cast<volatile float*>(list->data+static_cast<U>(i)*4u)); // 0046294D..00462954
    ++i;
    if(!(i<list->size))return;                                                  // 00462959..00462962
   }
   return;
  }
  // "%d %c" matched both: binary payload of count*4 bytes follows (0046296C..0046298E); separator byte cleared.
  const U bytes_needed=static_cast<U>(n)*4u;
  const void* const source=resource_read_0044bfd0(bytes_needed);                // 00462974
  rep_copy(list->data,address(source),bytes_needed);                            // 00462979..0046298E
  *text=0;                                                                      // 00462993 mov byte [ebx],0
  return;
 }
 // No count: lines until "endlist" (0046299B..00462A63), growing by doubling.
 char* line=resource_next_0044bfa0();                                           // 0046299B
 I index=0;                                                                     // [esp+14h]
 while(line){
  if(std::strncmp(skip_blanks_00463510(line),"endlist",7)==0)break;            // 004629B3..004629CE (strncmp 006FAFF0)
  const I size=list->size;                                                      // 004629D4
  if(size==0){
   kotor_renderer_gl::allocate_00437d60(list,nullptr,1);                        // 004629DB
   list->size=1;                                                                // 004629E3
  }else if(size==index){                                                        // 004629E8..004629EE
   const float last=list_float(*list,index-1);                                  // 004629F2 fld [data+eax*4-4]
   const I capacity=list->capacity;                                             // 004629F6
   if(size==capacity)                                                           // 004629F9
    kotor_renderer_gl::allocate_00437d60(list,nullptr,capacity?static_cast<U>(capacity+capacity):16u); // 00462A01..00462A11
   set_list_bits(*list,list->size,bits_of(last));                               // 00462A16..00462A1F
   list->size=list->size+1;                                                     // 00462A22..00462A26
  }
  parse_float_004635c0(line,reinterpret_cast<volatile float*>(list->data+static_cast<U>(index)*4u)); // 00462A29..00462A34
  ++index;                                                                      // 00462A40
  line=resource_next_0044bfa0();                                                // 00462A45
 }
 (void)skip_blanks_00463510(skip_blanks_00463510(line)+7);                      // 00462A54..00462A5E (result unused)
}
namespace {
// "channelscaleN" / "channeltranslateN" common store (00462C2C..00462C7A pattern): value >= 0 only.
void store_channel(texture::List& list,I index,float value,bool inline_growth){
 if(!(value>=0.0f))return;                                                      // fcomp 0.0; test ah,1 (C0: less or unordered)
 if(list.size==0){                                                              // 00462C47 / 00462E26
  if(inline_growth){                                                            // channeltranslate: inlined Allocate (00462E2D..00462E6B)
   const U old=list.data;
   list.capacity=4;                                                             // 00462E32
   void* const fresh=live_operator_new(16);                                     // 00462E35
   list.data=address(fresh);                                                    // 00462E3D
   for(I i=0;i<list.size;++i){U w;std::memcpy(&w,reinterpret_cast<const void*>(old+static_cast<U>(i)*4u),4);set_list_bits(list,i,w);} // 00462E43..00462E5F
   if(old)live_free(reinterpret_cast<void*>(old));                              // 00462E61..00462E66
  }else{
   kotor_renderer_gl::allocate_00437d60(&list,nullptr,4);                       // 00462C54
  }
  list.size=4;                                                                  // 00462C59 / 00462E72
  for(I i=0;i<4;++i)set_list_bits(list,i,0x3f800000u);                          // 00462C5C..00462C70 (1.0f raw, both lists)
 }
 set_list_bits(list,index,bits_of(value));                                      // 00462C73..00462C7A
}
}

std::uint8_t texture_controller_new_image_007bb870[0x10000];                   // newImage (BSS byte[65536]); Distort output

extern "C" {
// 00462A70 TextureController::TextureController(owner), thiscall RET4, EAX=this.
void* __fastcall texture_controller_ctor_00462a70(void* p,void*,font::Texture* owner){
 TextureController* const c=self_of(p);
 c->vtable=&texture_controller::base_vtable_00741c08;                          // 00462A89
 c->channel_scale.size=0;c->channel_scale.capacity=0;c->channel_scale.data=0;  // 00462A8F..00462A95 (+20,+24,+1C)
 c->channel_translate.size=0;c->channel_translate.capacity=0;c->channel_translate.data=0; // 00462A9C..00462AA2 (+2C,+30,+28)
 c->image=0;c->f08=0;c->f0c=0;                                                  // 00462AA5..00462AAB
 c->owner=owner;                                                                // 00462AB2
 c->f10=1;                                                                      // 00462AB5
 std::memcpy(&c->f14,"\x00\x00\xa0\x40",4);                                     // 00462ABC 40A00000h (5.0f)
 std::memcpy(&c->f18,"\x00\x00\x80\x3f",4);                                     // 00462AC3 3F800000h (1.0f)
 return c;
}
// 00460CF0 ~TextureController: table, then free +34, +28, +1C.
void __fastcall texture_controller_dtor_00460cf0(void* p,void*){
 TextureController* const c=self_of(p);
 c->vtable=&texture_controller::base_vtable_00741c08;                          // 00460D0D
 if(const U image=c->image)live_free(reinterpret_cast<void*>(image));          // 00460D13..00460D23
 if(const U data=c->channel_translate.data)live_free(reinterpret_cast<void*>(data)); // 00460D2B..00460D38
 if(const U data=c->channel_scale.data)live_free(reinterpret_cast<void*>(data)); // 00460D40..00460D50
}
// 00462AE0 scalar deleting destructor (RET4).
void* __fastcall texture_controller_delete_00462ae0(void* p,void*,texture::U flags){
 texture_controller_dtor_00460cf0(p,nullptr);                                   // 00462AE3
 if(flags&1u)live_free(p);                                                      // 00462AE8..00462AF0
 return p;
}
// 0041F250 IsLinear: xor al,al; ret.
std::uint8_t __fastcall texture_controller_is_linear_0041f250(void*,void*){return 0;}
// 005A4CF0 return_true (RingDistort +10): mov al,1; ret.
std::uint8_t __fastcall texture_controller_return_true_005a4cf0(void*,void*){return 1;}
// 004015A0 void_return (CycleTID +08): ret.
void __fastcall texture_controller_void_return_004015a0(void*,void*){}
// 006FB8DE _purecall (push 19h; call _amsg_exit 006FB344): base table +04/+08. Modern CRT _purecall (same R6025
// pure-virtual termination intent; message/exit path is the modern CRT's).
void __fastcall texture_controller_purecall_006fb8de(void*,void*){_purecall();}
// 0045FD70 SetImage: [+34] = copy of owner image, size BPP*Height*Width.
void __fastcall texture_controller_set_image_0045fd70(void* p,void*){
 TextureController* const c=self_of(p);
 font::Texture* const owner=c->owner;                                           // 0045FD76
 U size=static_cast<U>(texture_vtable::head(owner)->common.s50(owner,nullptr)); // 0045FD83 BPP
 size*=static_cast<U>(texture_vtable::head(owner)->common.s4c(owner,nullptr));  // 0045FD8C Height
 size*=static_cast<U>(texture_vtable::head(owner)->common.s48(owner,nullptr));  // 0045FD96 Width
 c->image=address(live_operator_new(size));                                     // 0045FD9D..0045FDA5
 const void* const source=owner_image(c);                                       // 0045FDA2..0045FDAD (owner reloaded)
 rep_copy(c->image,address(source),size);                                       // 0045FDB0..0045FDC3
}
// 004AADF0 cosineLUT / 004AAE30 sineLUT: idx = rint((float)(x*5.09295797f)) & 31; return table[idx].
float __cdecl cosine_lut_004aadf0(float x){
 const float scaled=static_cast<float>(static_cast<double>(x)*static_cast<double>(as_float(0x40A2F983u))); // 004AADFB/004AAE05 fmul [00744068]
 const U index=static_cast<U>(rint_i32(scaled))&31u;                            // 004AAE11..004AAE19
 return as_float(cosine_lut_00743fe8_bits[index]);                              // 004AAE1C fld [ecx*4+00743FE8]
}
float __cdecl sine_lut_004aae30(float x){
 const float scaled=static_cast<float>(static_cast<double>(x)*static_cast<double>(as_float(0x40A2F983u))); // 004AAE3B/004AAE45
 const U index=static_cast<U>(rint_i32(scaled))&31u;                            // 004AAE51..004AAE59
 return as_float(sine_lut_00744070_bits[index]);                                // 004AAE5C fld [ecx*4+00744070]
}
// 00460D70 Distort(height) RET4: offsets each pixel by LUT vectors scaled by +14 into newImage 007BB870.
std::uint8_t* __fastcall texture_controller_distort_00460d70(void* p,void*,const std::uint8_t* height){
 TextureController* const c=self_of(p);
 const I bpp=owner_bpp(c);                                                      // 00460D80 slot +50
 const I width=owner_width(c);                                                  // 00460D8E slot +48
 const I rows=owner_height(c);                                                  // 00460D9C slot +4C
 const U map=address(height);
 if(rows<=0)return texture_controller_new_image_007bb870;                       // 00460DBC / 00460F21
 const U row_stride=static_cast<U>(width)*static_cast<U>(bpp);                  // 00460DC2..00460DC4
 U row_destination=address(texture_controller_new_image_007bb870);              // 00460DCF / 00460F34
 U map_row=0;                                                                   // [esp+2Ch]
 if(c->f10!=0){                                                                 // 00460DA3..00460DB4: "distortangle"
  for(I row=0;row<rows;++row){
   if(width>0){
    U destination=row_destination;                                              // [esp+28h]
    for(I col=0;col<width;++col){
     const I v=bytes(map)[map_row+static_cast<U>(col)];                         // 00460E1E movzx
     const float angle=static_cast<float>(static_cast<double>(v)*static_cast<double>(as_float(0x3CC90FDBu))); // 00460E26..00460E30 [00741C28]
     const float fx=static_cast<float>(static_cast<double>(cosine_lut_004aadf0(angle))*static_cast<double>(c->f14)-0.5); // 00460E39..00460E4E
     const I dx=rint_i32(fx);                                                   // 00460E52..00460E5C
     const float fy=static_cast<float>(static_cast<double>(sine_lut_004aae30(angle))*static_cast<double>(c->f14)-0.5);   // 00460E5F..00460E74
     const I dy=rint_i32(fy);                                                   // 00460E78..00460E82
     U offset=(static_cast<U>(row+rows)+static_cast<U>(dx))&static_cast<U>(rows-1); // 00460E84..00460E98
     offset*=static_cast<U>(width);                                             // 00460E9A
     offset+=(static_cast<U>(col)+static_cast<U>(dy)+static_cast<U>(width))&static_cast<U>(width-1); // 00460EA1..00460EAE
     offset*=static_cast<U>(bpp);                                               // 00460EB3
     rep_copy(destination,c->image+offset,static_cast<U>(bpp));                 // 00460EB0..00460ED3
     destination+=static_cast<U>(bpp);                                          // 00460ECF
    }
   }
   map_row+=static_cast<U>(width);                                              // 00460EF8
   row_destination+=row_stride;                                                 // 00460EFA
  }
  return texture_controller_new_image_007bb870;                                 // 00460F13
 }
 for(I row=0;row<rows;++row){                                                   // 00460F1F.. (no angle)
  if(width>0){
   U destination=row_destination;                                               // [esp+30h]
   for(I col=0;col<width;++col){
    const I v=static_cast<I>(bytes(map)[map_row+static_cast<U>(col)])-0x7f;     // 00460F6C..00460F74
    const double scaled=static_cast<double>(v)*static_cast<double>(as_float(0x3C000000u)); // 00460F7B..00460F7F [00741C24]
    const float stored=static_cast<float>(scaled);                              // 00460F85 fst [esp+20h]
    const I dx=rint_i32(static_cast<float>(scaled*static_cast<double>(c->f14)-0.5)); // 00460F89..00460FA0 (ST kept)
    const I dy=rint_i32(static_cast<float>(static_cast<double>(stored)*static_cast<double>(c->f14)-0.5)); // 00460FA2..00460FC1
    U offset=(static_cast<U>(row)+static_cast<U>(dx))&static_cast<U>(rows-1);   // 00460FC3..00460FD0
    offset*=static_cast<U>(width);                                              // 00460FD6
    offset+=(static_cast<U>(dy)+static_cast<U>(col))&static_cast<U>(width-1);   // 00460FD9..00460FE5
    offset*=static_cast<U>(bpp);                                                // 00460FEA
    rep_copy(destination,c->image+offset,static_cast<U>(bpp));                  // 00460FE7..0046100A
    destination+=static_cast<U>(bpp);                                           // 00461006
   }
  }
  map_row+=static_cast<U>(width);                                               // 0046102B
  row_destination+=row_stride;                                                  // 0046102D
 }
 return texture_controller_new_image_007bb870;                                  // 00461046
}

// 00461060 Height2Channel(height, out) RET8: per pixel, channel i = ftol(h*scale[i]) (+ ftol(translate[i]*255)).
// bpp==4: four byte channels; otherwise packed nibbles (2 channels per byte). out==0 -> allocate BPP*W*H.
std::uint8_t* __fastcall texture_controller_height2channel_00461060(void* p,void*,const std::uint8_t* height,std::uint8_t* out){
 TextureController* const c=self_of(p);
 const I rows=owner_height(c);                                                  // 0046106E slot +4C
 const I width=owner_width(c);                                                  // 00461078 slot +48
 const I bpp=owner_bpp(c);                                                      // 00461086 slot +50
 U result=address(out);
 if(!out)result=address(live_operator_new(static_cast<U>(bpp)*static_cast<U>(width)*static_cast<U>(rows))); // 00461097..004610A0
 if(rows<=0)return bytes(result);                                               // 004610AE
 const U row_stride=static_cast<U>(bpp)*static_cast<U>(width);                  // 004610B8..004610BA [esp+48h]
 U row_out=result;                                                              // [esp+20h]
 U row_out_plus1=result+1;                                                      // [esp+2Ch]
 U row_offset4=0;                                                               // [esp+28h]
 U row_source=address(height);                                                  // [esp+24h]
 auto scale_of=[&](I i){return c->channel_scale.size>i?list_float(c->channel_scale,i):1.0f;};          // [esp+50h]=1.0 default
 auto translate_of=[&](I i){return c->channel_translate.size>i?list_float(c->channel_translate,i):0.0f;}; // ST=0.0 default
 const double k255=static_cast<double>(as_float(0x437F0000u));                  // [0073EDE4] 255.0f
 const double k15=static_cast<double>(as_float(0x41700000u));                   // [00741C2C] 15.0f
 for(I row=0;row<rows;++row){
  if(width>0){
   U pixel_plus1=row_out_plus1;                                                 // [esp+1Ch]
   U pixel_offset4=row_offset4;                                                 // [esp+18h]
   U pixel_out=row_out;                                                         // [esp+30h]
   U source=row_source;                                                         // [esp+34h]
   for(I col=0;col<width;++col){
    if(bpp==4){                                                                 // 00461110
     const std::uint8_t h=bytes(source)[0];                                     // 00461119
     I ch=0;
     for(;ch<4;++ch){                                                           // unrolled 0046112A..0046131A (single pass)
      const float scale=scale_of(ch);const float translate=translate_of(ch);
      std::uint8_t& target=bytes(pixel_plus1)[ch-1];
      target=0;                                                                 // 0046115A
      if(scale>0.0f)target=static_cast<std::uint8_t>(ftol(static_cast<double>(h)*static_cast<double>(scale))); // 0046116B..00461181
      if(translate>0.0f)target=static_cast<std::uint8_t>(target+static_cast<std::uint8_t>(ftol(static_cast<double>(translate)*k255))); // 00461191..0046119C
     }
     // 0046132F: per-channel tail loop runs only when ch<4; unreachable after the unrolled pass (ch==4).
    }else{
     const std::uint8_t h=static_cast<std::uint8_t>(bytes(source)[0]>>4);       // 004613CA..004613CC
     U target=pixel_out;                                                        // edi = [esp+30h]
     for(I k=0;k<bpp;++k){                                                      // 004613D1..004614F8
      const I lo=2*k,hi=2*k+1;
      float scale=c->channel_scale.size>lo?list_float(c->channel_scale,lo):1.0f;     // 004613E5..00461400
      float translate=c->channel_translate.size>lo?list_float(c->channel_translate,lo):0.0f; // 00461404..0046140E
      std::uint8_t& byte=bytes(target)[0];
      byte=0;                                                                   // 00461415
      if(scale>0.0f)byte=static_cast<std::uint8_t>(ftol(static_cast<double>(h)*static_cast<double>(scale))&0x0f); // 00461425..0046143B
      if(translate>0.0f)byte=static_cast<std::uint8_t>((byte-static_cast<std::uint8_t>(ftol(static_cast<double>(translate)*k15)))&0x0f); // 0046144A..0046145C
      scale=c->channel_scale.size>hi?list_float(c->channel_scale,hi):1.0f;      // 00461462..00461481
      translate=c->channel_translate.size>hi?list_float(c->channel_translate,hi):0.0f; // 00461485..0046148F
      if(scale>0.0f)byte=static_cast<std::uint8_t>((static_cast<std::uint8_t>(ftol(static_cast<double>(h)*static_cast<double>(scale))<<4)+byte)&0xf0); // 004614A4..004614C3
      if(translate>0.0f)byte=static_cast<std::uint8_t>((byte-static_cast<std::uint8_t>(ftol(static_cast<double>(translate)*k15)<<4))&0xf0); // 004614D2..004614E7
      ++target;                                                                 // 004614F2
     }
    }
    pixel_plus1+=4;pixel_offset4+=4;                                            // 00461513..00461515
    pixel_out+=static_cast<U>(bpp);                                             // 00461523
    ++source;                                                                   // 00461525
   }
   (void)pixel_offset4;
  }
  row_out+=row_stride;                                                          // 00461551
  row_source+=static_cast<U>(width);                                            // 00461553
  row_offset4+=static_cast<U>(width)*4u;                                        // 00461568
  row_out_plus1+=static_cast<U>(width)*4u;                                      // 0046156A
 }
 return bytes(result);                                                          // 00461583
}

// 00462B00 TextureController::ParseField(line) RET4 (also 00463310 = jmp 00462B00).
void __fastcall texture_controller_parse_field_00462b00(void* p,void*,const char* text){
 TextureController* const c=self_of(p);
 char* const line=const_cast<char*>(text);
 auto key=[&](const char* k){return kotor_stricmp_0070acaf(k,firstword_00463530(line))==0;};
 if(key("channelscale"))mdl_parse_float_list_004628c0(line+0x0c,&c->channel_scale);           // 00462B12..00462B2B
 if(key("channeltranslate"))mdl_parse_float_list_004628c0(line+0x10,&c->channel_translate);   // 00462B3A..00462B53
 if(key("distort"))parse_int_004635e0(line+7,&c->f0c);                                   // 00462B62..00462B7B
 if(key("distortangle"))parse_int_004635e0(line+0x0c,&c->f10);                           // 00462B8A..00462BA3
 if(key("distortionamplitude"))parse_float_004635c0(line+0x13,&c->f14);                  // 00462BB2..00462BCB
 if(key("speed"))parse_float_004635c0(line+5,&c->f18);                                   // 00462BDA..00462BF3
 static const char* const scale_keys[4]={"channelscale0","channelscale1","channelscale2","channelscale3"};
 static const char* const translate_keys[4]={"channeltranslate0","channeltranslate1","channeltranslate2","channeltranslate3"};
 for(I i=0;i<4;++i){                                                                     // 00462BFC..00462DE1
  float value=as_float(0xbf800000u);                                                     // -1.0f before each test
  if(key(scale_keys[i]))parse_float_004635c0(line+0x0d,&value);
  store_channel(c->channel_scale,i,value,false);
 }
 for(I i=0;i<4;++i){                                                                     // 00462DF3..0046309F
  // channeltranslate3 keeps its -1.0f in the incoming argument cell [esp+18h] (00462FF7); not observable.
  float value=as_float(0xbf800000u);
  if(key(translate_keys[i]))parse_float_004635c0(line+0x11,&value);
  store_channel(c->channel_translate,i,value,true);
 }
}
}

namespace texture_controller {
// 00741C08 TextureController (abstract): +04/+08 = _purecall 006FB8DE.
const TextureControllerVTable base_vtable_00741c08={
 &texture_controller_delete_00462ae0,&texture_controller_purecall_006fb8de,&texture_controller_purecall_006fb8de,
 &texture_controller_parse_field_00462b00,&texture_controller_is_linear_0041f250,&texture_controller_distort_00460d70,
 &texture_controller_set_image_0045fd70};
}
