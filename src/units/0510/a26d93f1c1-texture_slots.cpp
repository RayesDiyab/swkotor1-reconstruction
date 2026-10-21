// texture_slots.cpp - lane-texture-epoch PROPOSAL (review E-D1/E-D2): CAurTexture virtual slot bodies on the
// fnt_console path, shared by tables 0073F160/0073F0A8. Evidence: task-3 claude-texture-core-review-r1/disasm/41EA00.txt.
// Common prologue of every body (e.g. 0041EC30..0041EC3E): mov eax,[ecx]; call [eax+8] (GetReferencedTexture);
// mov edx,[eax]; mov ecx,eax; call [edx+4] (Basic: self_return 00641DB0; CAurTexture: return_zero 0063E7F0);
// test eax,eax; je <null tail>. Both calls go through the typed tables (texture_vtable::head).
#include "8945d3b71e-texture_vtable.hpp"
#include <cstring>

namespace {
using texture::Basic; using texture::U; using texture::I; using Ref=font::Texture;
// The Basic object begins with its Ref prefix (standard layout, first member): pointer-interconvertible.
Basic* resolve(void* self){
 Ref* const object=static_cast<Ref*>(self);
 Ref* const referenced=texture_vtable::head(object)->common.s08(object,nullptr);   // call [eax+8]
 Ref* const target=texture_vtable::head(referenced)->s04(referenced,nullptr);      // call [edx+4]
 return reinterpret_cast<Basic*>(target);
}
U address(const void* p){return reinterpret_cast<U>(p);}
// 0041ECFF/0041ED09: rep movsd (count>>2) then rep movsb (count&3).
void copy_words_then_bytes(void* destination,const void* source,U count){
 __asm {
  mov edi,destination
  mov esi,source
  mov ecx,count
  mov eax,ecx
  shr ecx,2
  rep movsd
  mov ecx,eax
  and ecx,3
  rep movsb
 }
}
}

extern "C" {
// 0063E7F0 return_zero (33 C0 C3) as reference-table slot +04 (D24/E-D2): ECX ignored, EAX=0, RET0.
font::Texture* __fastcall texture_reference_query_0063e7f0(font::Texture*,void*){return nullptr;}

// +0C IsCubeMap 0041EBF0: cl=[basic+DB]; test; setne al (0041EC00..0041EC08); null -> xor al,al (0041EC0C).
std::uint8_t __fastcall texture_is_cube_map_0041ebf0(void* self,void*){
 Basic* const b=resolve(self);
 if(!b)return 0;
 return b->flags[3]!=0?1:0;
}
// +28 IsFullyLoaded 0041EC10: al=[basic+E0] (0041EC20); null -> xor al,al.
std::uint8_t __fastcall texture_is_fully_loaded_0041ec10(void* self,void*){
 Basic* const b=resolve(self);
 return b?b->flags[8]:static_cast<std::uint8_t>(0);
}
// +30 IsProcessed 0041EC30: al=[basic+E5] (0041EC40); null -> xor al,al (0041EC47).
std::uint8_t __fastcall texture_is_processed_0041ec30(void* self,void*){
 Basic* const b=resolve(self);
 return b?b->flags[13]:static_cast<std::uint8_t>(0);
}
// +34 IsTxiLoaded 0041EC50: al=[basic+E6] (0041EC60); null -> xor al,al (0041EC67).
std::uint8_t __fastcall texture_is_txi_loaded_0041ec50(void* self,void*){
 Basic* const b=resolve(self);
 return b?b->flags[14]:static_cast<std::uint8_t>(0);
}
// +A4 SetTxiLoaded 0041EC70 (RET4): if basic, [basic+E6]=low byte of arg (0041EC80/0041EC84); EAX = basic or 0.
U __fastcall texture_set_txi_loaded_0041ec70(void* self,void*,U loaded){
 Basic* const b=resolve(self);
 if(b)b->flags[14]=static_cast<std::uint8_t>(loaded);
 return address(b);
}
// +94 GetTxiData 0041EC90: eax=[basic+48] (0041ECA0); null -> 0.
void* __fastcall texture_get_txi_data_0041ec90(void* self,void*){
 Basic* const b=resolve(self);
 return b?reinterpret_cast<void*>(static_cast<U>(b->f48)):nullptr;
}
// +A0 SetTxiData 0041ECB0 (RET8). Raw EAX (D12): null basic -> 0; data==0 or length<=0 -> basic; copy -> length.
U __fastcall texture_set_txi_data_0041ecb0(void* self,void*,void* data,I length){
 Basic* const b=resolve(self);                                      // 0041ECBD ebx=basic
 if(!b)return 0;                                                    // 0041ECC1
 if(!data||length<=0)return address(b);                             // 0041ECC9/0041ECD3 (EAX still basic)
 const U old=b->f48;                                                // 0041ECD5
 if(old){live_free(reinterpret_cast<void*>(old));b->f48=0;}         // 0041ECDC..0041ECE5
 void* const memory=live_operator_new(static_cast<U>(length));      // 0041ECEE operator new 006FA7E6
 b->f48=address(memory);                                            // 0041ECF3
 copy_words_then_bytes(memory,data,static_cast<U>(length));         // 0041ECF6..0041ED09 (no allocation-failure branch)
 texture::field_store(b->f64,length);                               // 0041ED0B [+64]=length
 return static_cast<U>(length);                                     // EAX = count (0041ECFA mov eax,ecx)
}
// +90 GetTxiSize 0041ED20: eax=[basic+64] (0041ED30); null -> 0.
U __fastcall texture_get_txi_size_0041ed20(void* self,void*){
 Basic* const b=resolve(self);
 return b?static_cast<U>(texture::field_load(b->f64)):0u;
}
// +40 GetName 0041EFE0: eax=basic+78h (0041EFF0); null -> 0.
char* __fastcall texture_get_name_0041efe0(void* self,void*){
 Basic* const b=resolve(self);
 return b?b->name:nullptr;
}
// +44 GetFileRange 0041F000: movsx eax,word [basic+C6] (0041F010); null -> 0.
I __fastcall texture_get_file_range_0041f000(void* self,void*){
 Basic* const b=resolve(self);
 return b?static_cast<I>(b->c6):0;
}
// +AC glImage 00421430 (RET8): null -> 0 (0042143F); image!=0 -> [+44]=image, 00420970(update), [+44]=0;
// else 00420970(update). EAX = 00420970 result.
U __fastcall texture_gl_image_00421430(void* self,void*,U update,U image){
 Basic* const b=resolve(self);
 if(!b)return 0;
 if(image!=0){
  b->f44=image;                                                     // 0042144D
  const U result=texture::basic_gl_image_00420970(b,nullptr,update); // 00421457
  b->f44=0;                                                         // 0042145C
  return result;
 }
 return texture::basic_gl_image_00420970(b,nullptr,update);         // 0042146E
}

// ---- remaining slots (complete tables) ----
// +10 IsInvalid 004205D0: basic -> al=[basic+E1] (004205E3); else setne(self != [007A477C] nullTexture) (004205EB).
std::uint8_t __fastcall texture_is_invalid_004205d0(void* self,void*){
 Basic* const b=resolve(self);
 if(b)return b->flags[9];
 return static_cast<Ref*>(self)!=texture_cells::null_texture_007a477c.target?1:0;
}
// +14 IsDiffuseBumpMap 0041EE70: basic && [vtbl+1C] IsBumpMap(AL) && [basic+D8] -> EAX=1 else 0.
texture::U __fastcall texture_is_diffuse_bump_map_0041ee70(void* self,void*){
 Basic* const b=resolve(self);
 if(!b)return 0;
 if(static_cast<std::uint8_t>(texture_vtable::head(&b->prefix)->common.s1c(b,nullptr))==0)return 0;   // 0041EE87 test al,al
 return b->flags[0]!=0?1u:0u;                                                                          // 0041EE8E
}
// +18 IsSpecularBumpMap 0041EEB0: same with [basic+D9].
texture::U __fastcall texture_is_specular_bump_map_0041eeb0(void* self,void*){
 Basic* const b=resolve(self);
 if(!b)return 0;
 if(static_cast<std::uint8_t>(texture_vtable::head(&b->prefix)->common.s1c(b,nullptr))==0)return 0;   // 0041EEC7
 return b->flags[1]!=0?1u:0u;                                                                          // 0041EECE
}
// +24 IsUsingGlobalAlpha 0041EF80: al=[basic+DE].
std::uint8_t __fastcall texture_is_using_global_alpha_0041ef80(void* self,void*){
 Basic* const b=resolve(self);
 return b?b->flags[6]:static_cast<std::uint8_t>(0);
}
// +2C IsEnvironmentMapped 0041ED40: al=[basic+DA].
std::uint8_t __fastcall texture_is_environment_mapped_0041ed40(void* self,void*){
 Basic* const b=resolve(self);
 return b?b->flags[2]:static_cast<std::uint8_t>(0);
}
// +68 GetEnvMapAlpha 0041F180: fld [basic+28] | fld [0073D764]=1.0f.
float __fastcall texture_get_env_map_alpha_0041f180(void* self,void*){
 Basic* const b=resolve(self);
 return b?b->f28:1.0f;
}
// +74 GetMapTexelsToPixels 0041F120: al=[basic+DF].
std::uint8_t __fastcall texture_get_map_texels_to_pixels_0041f120(void* self,void*){
 Basic* const b=resolve(self);
 return b?b->flags[7]:static_cast<std::uint8_t>(0);
}
// +7C GetBumpIntensity 0041EDD0: fld [basic+1C] | fld [0073D700]=0.0f.
float __fastcall texture_get_bump_intensity_0041edd0(void* self,void*){
 Basic* const b=resolve(self);
 return b?b->f1c:0.0f;
}
// +80 GetSpecularBumpIntensity 0041EDF0: basic && [vtbl+18](AL) -> fld [basic+30]; else 0.0f (0073D700).
float __fastcall texture_get_specular_bump_intensity_0041edf0(void* self,void*){
 Basic* const b=resolve(self);
 if(!b)return 0.0f;
 if(static_cast<std::uint8_t>(texture_vtable::head(&b->prefix)->common.s18(b,nullptr))==0)return 0.0f; // 0041EE0F/0041EE12
 return b->f30;                                                                                         // 0041EE16
}
// +84 GetDiffuseBumpIntensity 0041EE30: basic && [vtbl+14](AL) -> fld [basic+2C]; else 0.0f.
float __fastcall texture_get_diffuse_bump_intensity_0041ee30(void* self,void*){
 Basic* const b=resolve(self);
 if(!b)return 0.0f;
 if(static_cast<std::uint8_t>(texture_vtable::head(&b->prefix)->common.s14(b,nullptr))==0)return 0.0f; // 0041EE4F/0041EE52
 return b->f2c;                                                                                         // 0041EE56
}
// +88 GetSpecularColor 0041ED60 (RET4, hidden Vector* result; EAX=result): basic -> copy [+4C],[+50],[+54] as
// DWORDs (0041ED95..0041EDA5); else copy a zeroed local Vector (0041ED71..0041ED81, 0041EDAE..0041EDC5).
void* __fastcall texture_get_specular_color_0041ed60(void* self,void*,void* result){
 Basic* const b=resolve(self);
 U words[3]={0,0,0};
 if(b){words[0]=texture::float_bits(b->f4c);words[1]=texture::float_bits(b->f50);words[2]=texture::float_bits(b->f54);}
 std::memcpy(result,words,sizeof words);
 return result;
}
// +98 GetTextureFormat 0041F1F0: null -> 7; if [+5C]==0 && [+74]==7: [+70]==4 -> [+74]=0; ==3 -> 1; else
// [+74]=(c!=2)+2 then reread; otherwise return [+74].
texture::U __fastcall texture_get_texture_format_0041f1f0(void* self,void*){
 Basic* const b=resolve(self);
 if(!b)return 7;                                                                         // 0041F241
 if(texture::field_load(b->f5c)==0&&texture::field_load(b->f74)==7){                    // 0041F200..0041F20B
  const I c=texture::field_load(b->f70);                                                 // 0041F20D
  if(c==4){texture::field_store(b->f74,0);return 0;}                                     // 0041F215..0041F21C
  if(c==3){texture::field_store(b->f74,1);return 1;}                                     // 0041F222..0041F22C
  texture::field_store(b->f74,c!=2?3:2);                                                 // 0041F22D..0041F23A
 }
 return static_cast<U>(texture::field_load(b->f74));                                     // 0041F23D
}
// +9C SetTextureFormat 0041F260 (RET4): basic && [+5C]==0 -> [+74]=arg.
void __fastcall texture_set_texture_format_0041f260(void* self,void*,U format){
 Basic* const b=resolve(self);
 if(b&&texture::field_load(b->f5c)==0)texture::field_store(b->f74,static_cast<I>(format)); // 0041F270..0041F27B
}
// +A8 IncrementTIDIndex 0041F3B0 (RET4): basic -> [+58]=([+40])[index].
void __fastcall texture_increment_tid_index_0041f3b0(void* self,void*,U index){
 Basic* const b=resolve(self);
 if(!b)return;
 const U ids=b->f40;                                                                     // 0041F3C0
 U id;std::memcpy(&id,reinterpret_cast<const void*>(ids+index*4u),sizeof id);           // 0041F3C7
 b->f58=id;                                                                              // 0041F3CA
}
// reference +B0 CAurTexture::FreeImage 0041F310: basic -> tail jump to the Basic's own [vtable+B0] (0041F324),
// EAX from it; else EAX=0 (query result).
texture::U __fastcall texture_free_image_0041f310(void* self,void*){
 Basic* const b=resolve(self);
 if(!b)return 0;
 return texture::texture_call_free_b0_eax(b);
}
// reference +00 ~CAurTexture scalar deleting 00423130 (RET4): 00422E90 base dtor; flags&1 -> free; EAX=this.
void* __fastcall texture_reference_scalar_delete_00423130(void* self,void*,U flags){
 Ref* const object=static_cast<Ref*>(self);
 (void)texture_owner::base_destroy_00422e90(object,nullptr);                             // 00423133
 if(flags&1u)live_free(object);                                                          // 00423138..00423140
 return object;                                                                          // 00423148
}
}
