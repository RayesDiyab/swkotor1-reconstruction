// lane-upload: faithful bodies for the texture upload/bind chain (swkotor.exe GOG K1).
// Evidence: dumpbin /disasm of the original (lane evidence/*.txt); instruction addresses cited per behavior.
// GL: every opengl32/glu32 call goes through the single IAT adapter owner (kotor_renderer_init::*_0073dxxx,
// lane-renderer-bodies), matching the original `call dword ptr ds:[IAT]` / `call ebx` with ebx=[IAT];
// glCompressedTexImage2DARB is re-read from extension cell 007BB774 at each call site through
// aurora_context_adoption::live_extensions (D11).
#include <cstdint>
#include <cstring>
#include "b195664429-upload.h"

namespace {
using namespace texture_upload;
namespace gl=kotor_renderer_init;

static_assert(sizeof(void*)==4 && sizeof(Basic)==0xec,"x86 Basic (E1)");

using Ref=font::Texture;
// U-D1: typed vtable access (lane-texture-epoch texture_vtable.hpp). The receiver's table pointer is
// reloaded from the volatile Ref prefix at every call (original: mov reg,[obj]; call [reg+slot]).
// Both tables (Basic 0073F160 / Reference 0073F0A8) hold a real texture_vtable::CommonSlots object at +08,
// so the data-pointer view below designates that member; slots are called with their declared types.
inline Ref* as_ref(void* object){return static_cast<Ref*>(object);}           // Ref24 is the +00 member
inline Basic* as_basic(Ref* object){return reinterpret_cast<Basic*>(object);} // standard-layout: Basic::prefix at +00
inline const texture_vtable::CommonSlots& table(const void* object){
 void** const vtable=static_cast<const Ref*>(object)->vtable;                  // volatile reload
 return *reinterpret_cast<const texture_vtable::CommonSlots*>(reinterpret_cast<const char*>(vtable)+8);
}
// Getter prologue: this->+08 (GetReferencedTexture) then ->+04 (Basic: 00641DB0; Reference: 0063E7F0).
inline Basic* referenced_basic(void* self){
 Ref* referenced=table(self).s08(as_ref(self),nullptr);
 return as_basic(upload_call_query_04(referenced));
}
inline U8 flag(const Basic* b,unsigned offset){return b->flags[offset-0xd8];}
inline U* ids(const Basic* b){return reinterpret_cast<U*>(static_cast<std::uintptr_t>(b->f40));}
inline void* as_pointer(U word){return reinterpret_cast<void*>(static_cast<std::uintptr_t>(word));}
inline I quarter_up(I value){return (value+3)/4;}     // lea +3; cdq; and edx,3; add; sar 2
inline I block_bytes(I bpp){return (bpp==4?1:0)*8+8;} // sete; lea [eax*8+8]
inline U8 low_byte(U value){return static_cast<U8>(value);} // `test al,al` on a full-EAX result
inline I field(const I& value){return texture::field_load(value);}

inline aurora_context_adoption::CompressedTexImage2D compressed_tex_image_2d(){
 return *aurora_context_adoption::live_extensions.compressed_tex_image_2d_007bb774;  // ds:[007BB774] per call
}
inline U format_entry(I index){return cells.gl_format_table_0073f36c[index];}       // [eax*4+0073F36C]

// GLRender 2D texture-parameter tail: 00424BF5..00424D50 (CreateTexture) / 004251B3..0042523F (Processed).
void texture_parameters(Basic* t,U mip){
 const I clamp=table(t).s60(t,nullptr);                       // edi = GetClamp
 const U filter=table(t).s70(t,nullptr);                     // movzx esi,al = GetFilter
 const U bits=static_cast<U>(clamp);
 gl::glTexParameteri_0073d23c(0x0de1,0x2802,static_cast<I>((~bits&1u)|0x2900u));      // WRAP_S
 gl::glTexParameteri_0073d23c(0x0de1,0x2803,static_cast<I>((~(bits>>1)&1u)|0x2900u)); // WRAP_T (shr)
 const U magnify=(filter!=0?1u:0u)+0x2600u;                          // setne; add 2600h
 gl::glTexParameteri_0073d23c(0x0de1,0x2800,static_cast<I>(magnify));
 const U minify=mip!=0?((filter!=0?3u:0u)+0x2700u):magnify;          // neg/sbb/and 3/add 2700h : edi
 gl::glTexParameteri_0073d23c(0x0de1,0x2801,static_cast<I>(minify));
}
// Cube texture-parameter tail: 0042418A..0042421D (CreateCubeTexture) / 004244E0..00424573 (Processed).
void cube_parameters(Basic* t){
 gl::glTexParameteri_0073d23c(0x8513,0x2802,0x812f);                  // CLAMP_TO_EDGE S
 gl::glTexParameteri_0073d23c(0x8513,0x2803,0x812f);                  // T
 gl::glTexParameteri_0073d23c(0x8513,0x8072,0x812f);                  // R
 const U8 filter=table(t).s70(t,nullptr);                    // neg al; sbb; neg
 gl::glTexParameteri_0073d23c(0x8513,0x2800,static_cast<I>((filter!=0?1u:0u)+0x2600u));
 U minify;
 if(table(t).s6c(t,nullptr)!=0){                             // GetMipMap (AL)
  const U8 again=table(t).s70(t,nullptr);                    // GetFilter called again
  minify=(again!=0?3u:0u)+0x2700u;
 }else{
  const U8 again=table(t).s70(t,nullptr);
  minify=(again!=0?1u:0u)+0x2600u;
 }
 gl::glTexParameteri_0073d23c(0x8513,0x2801,static_cast<I>(minify));
}
inline bool power_of_two_or_zero(I value){return (value&(value-1))==0;}
}

namespace texture {
// ---------------------------------------------------------------- 00420970 CAurTextureBasic::glImage
U __fastcall basic_gl_image_00420970(Basic* self,void*,U update){
 if(gl::ATIValidate_0045f640())gl::glFinish_0073d254();             // 00420973 / IAT 0073D254
 if(self->f44==0){self->f58=0;return 0;}                             // 00420989
 if(self->f58==0 || self->f40==0){                                   // 00420996..004209A2
  if(self->f40!=0)live_free(ids(self));                              // 006FA390 operator delete
  const I bytes=(static_cast<I>(self->c4)*static_cast<I>(self->c2))<<2;
  void* storage=live_operator_new(static_cast<std::uint32_t>(bytes)); // 006FA7E6
  self->f40=static_cast<U>(reinterpret_cast<std::uintptr_t>(storage)); // 004209D9 (before count reload)
  const I count=static_cast<I>(self->c4)*static_cast<I>(self->c2);
  gl::glGenTextures_0073d24c(count,static_cast<U*>(storage));        // IAT 0073D24C, pushes the new pointer
  self->f58=ids(self)[0];                                            // 004209ED reloads [esi+40h]
 }
 for(I index=0;index<static_cast<I>(self->c4)*static_cast<I>(self->c2);++index){ // movsx/imul at each test
  aur_texture_bind_tid_004204f0(ids(self)[index],flag(self,0xdb));   // 00420A26 (movzx DB)
  if(flag(self,0xdb)!=0){                                            // reload DB 00420A2B
   if(texture_call_processed_30_al(self)!=0)gl_create_processed_cube_texture_00424230(self,update,index); // 00420A46
   else gl_create_cube_texture_00423f90(self,update,index);          // 00420A4D
  }else if(texture_call_processed_30_al(self)!=0){                   // 00420A56
   gl_create_processed_texture_00424dd0(self,update,index);          // 00420A60
  }else{
   const U controller=self->f34;                                     // 00420A67 mov ecx,[esi+34h]
   if(controller!=0)upload_controller_call_10(controller);           // 00420A6E call [edx+10h]
   gl_create_texture_00424980(self,update,index);                    // 00420A76
  }
 }
 if(flag(self,0xdb)!=0 && gl::CubeMapAvailable_0045f8b0()!=0)        // 00420A95..00420AA7
  gl::glDisable_0073d22c(*cells.gl_texture_cube_map_0078d478);       // 00420AB0
 return self->f58;                                                   // 00420AB6
}
}

extern "C" {

// ---------------------------------------------------------------- 004204F0 AurTextureBindTID
void __cdecl aur_texture_bind_tid_004204f0(U id,U8 cube){
 bool cube_enabled=false;                                            // xor bl,bl
 if(gl::CubeMapAvailable_0045f8b0()!=0){                             // 004204F4
  if(cube!=0){                                                       // byte [esp+10h]
   cube_enabled=true;                                                // mov bl,1
   gl::glEnable_0073d390(*cells.gl_texture_cube_map_0078d478);       // 0042050D
  }else{
   gl::glDisable_0073d22c(*cells.gl_texture_cube_map_0078d478);      // 0042051C
  }
 }
 if(id>1u && *cells.bind_gate_007a470c<0)return;                     // cmp esi,1; jbe / test; jl 004205B0
 if(cube_enabled){
  gl::glBindTexture_0073d240(*cells.gl_texture_cube_map_0078d478,id); // 00420540
  gl::glTexParameteri_0073d23c(*cells.gl_texture_cube_map_0078d478,0x84fe,*cells.anisotropy_007a685c); // 00420558
  auto& system=renderer_system::bindings();
  const U stage=*system.zero_007a6898;                               // mov edx,[007A6898]
  const U count=*cells.texture_binds_count_007a4710;                 // mov eax,[007A4710]
  system.cache6_007a683c[stage]=id;                                  // [edx*4+007A683C]=esi
  *cells.texture_binds_count_007a4710=count+1u;                      // inc eax; store
  return;
 }
 gl::glBindTexture_0073d240(*cells.gl_texture_2d_0078d458,id);       // 0042057F
 gl::glTexParameteri_0073d23c(*cells.gl_texture_2d_0078d458,0x84fe,*cells.anisotropy_007a685c); // 00420598
 auto& system=renderer_system::bindings();
 system.cache6_007a683c[*system.zero_007a6898]=id;                   // 004205A3
 *cells.texture_binds_count_007a4710=*cells.texture_binds_count_007a4710+1u; // inc dword [007A4710]
}

// ---------------------------------------------------------------- 00420BF0 AurTextureGetTID
U __cdecl aur_texture_get_tid_00420bf0(void* texture){
 if(texture==nullptr)return 0;                                       // 00420BF7
 if(table(texture).s08(as_ref(texture),nullptr)==static_cast<void*>(cells.null_texture_007a477c->target))return 0; // 00420C06
 Ref* referenced=table(texture).s08(as_ref(texture),nullptr);        // 00420C0C (second call)
 Basic* basic=as_basic(upload_call_query_04(referenced));
 return basic->f58;                                                  // mov eax,[eax+58h]
}

// ---------------------------------------------------------------- 004214B0 CAurTexture::Bind
void __fastcall texture_bind_004214b0(void* self,void*){
 if(*cells.bind_zero_007a4714!=0 ||                                  // 004214BA
    table(self).s08(as_ref(self),nullptr)==static_cast<void*>(cells.null_texture_007a477c->target)){ // 004214C7
  aur_texture_bind_tid_004204f0(0,0);                                // push 0; push 0
  return;
 }
 Basic* basic=as_basic(upload_call_query_04(as_ref(self))); // 004214DC
 if(basic==nullptr){
  Ref* referenced=table(self).s08(as_ref(self),nullptr);
  basic=as_basic(upload_call_query_04(referenced)); // 004214F0
  if(basic==nullptr){aur_texture_bind_tid_004204f0(0,0);return;}     // 0042154B
 }
 *cells.current_tid_007a47b8=aur_texture_get_tid_00420bf0(basic);   // 004214FF
 const U8 cube=texture::texture_call_flag_0c_al(basic);              // IsCubeMap (AL), pushed as eax
 aur_texture_bind_tid_004204f0(*cells.current_tid_007a47b8,cube);   // reloads 007A47B8 (0042150E)
 Ref* referenced=table(basic).s08(&basic->prefix,nullptr);            // 00421522
 Basic* owner=as_basic(upload_call_query_04(referenced));
 const U controller=owner->f34;                                      // 0042152C mov ecx,[eax+34h]
 if(controller!=0 && *cells.enable_texture_controllers_0078d430!=0)
  upload_controller_call_04(controller);                             // 0042153E call [eax+4]
 basic->flags[0xe7-0xd8]=1;                                          // mov byte [esi+E7h],1
}

// ---------------------------------------------------------------- 00424D70 GetCompressedContactSheetOffset
I __cdecl get_compressed_contact_sheet_offset_00424d70(I index,I size,I bpp){
 const I block=block_bytes(bpp);
 I total=quarter_up(size)*quarter_up(size)*block;
 while(size>1){size>>=1;total+=quarter_up(size)*quarter_up(size)*block;} // sar esi,1 loop
 return total*index;
}

// ---------------------------------------------------------------- 0045F000 ImageGetSubImage
void* __cdecl image_get_sub_image_0045f000(const void* image,I width,I,I bpp,I x,I y,I w,I h){
 const I row=bpp*w;                                                  // esi (0045F010)
 unsigned char* result=static_cast<unsigned char*>(live_operator_new(static_cast<std::uint32_t>(row*h))); // 006FA7E6
 if(h<=0)return result;                                              // 0045F030
 const I stride=width*bpp;                                           // [esp+1Ch]
 const unsigned char* source=static_cast<const unsigned char*>(image)+(width*y+x)*bpp; // 0045F03C..0045F04B
 unsigned char* target=result;                                       // [esp+10h]
 for(I rows=h;rows!=0;--rows){                                       // dec edi; jne
  if(w>0){
   const unsigned char* s=source;unsigned char* d=target;
   for(I columns=w;columns!=0;--columns){std::memcpy(d,s,static_cast<std::size_t>(static_cast<U>(bpp)));s+=bpp;d+=bpp;} // rep movsd/movsb
  }
  source+=stride;target+=row;                                        // 0045F0AB / 0045F0AD
 }
 return result;
}

// ---------------------------------------------------------------- 00420EA0 BuildTextureNormalMap
U __cdecl build_texture_normal_map_00420ea0(void* data,float scale,I* width,I* height,I,I zero,U normal){
 const I original_width=*width;                                      // eax
 bool failed=false;                                                  // ebx
 if(!power_of_two_or_zero(original_width)){
  if(power_of_two_or_zero(original_width-1))*width=original_width-1; // 00420EC4
  else failed=true;                                                  // 00420EBD
 }
 const I original_height=*height;                                    // ecx
 if(!power_of_two_or_zero(original_height)){
  if(!power_of_two_or_zero(original_height-1))return 0;              // 00420EDA -> 00420EE2
  *height=original_height-1;                                         // 00420EDC
 }
 if(failed)return 0;                                                 // 00420EDE
 convert_height_field_and_load_normal_map_004260a0(data,*width,*height,original_width,original_height,scale,zero,normal); // 00420F04
 return 1;
}

// ---------------------------------------------------------------- 00424580 AurBuild2DMipMaps
void __cdecl aur_build_2d_mip_maps_00424580(I w,I h,I bpp,const void* buffer,U update){
 I width=w,height=h;                                                 // ebx/[esp+14h], [esp+50h]
 unsigned char* source;                                              // ebp / [esp+28h]
 if(w==h){
  const I bytes=w*h*bpp;
  source=static_cast<unsigned char*>(live_operator_new(static_cast<std::uint32_t>(bytes))); // 006FC0D2 -> 006FA7E6
  std::memcpy(source,buffer,static_cast<std::size_t>(static_cast<U>(bytes)));             // rep movs
 }else{
  const I side=w>h?w:h;                                              // 0042459D..004245A1
  source=static_cast<unsigned char*>(image_scale_0045dad0(buffer,w,h,bpp,side,side)); // 004245B1
  width=side;height=side;
 }
 I level=0;                                                          // [esp+38h]
 const U type=(static_cast<U>((bpp!=2?1u:0u)-1u)&0x6c32u)+0x1401u;   // 1401 or 8033 (bpp 2)
 U format;
 if(bpp==4)format=0x1908;else if(bpp==3)format=0x1907;else format=(bpp!=2?1u:0u)+0x1908u;
 if(low_byte(update)==0)gl::glTexImage2D_0073d280(0x0de1,0,bpp,width,height,0,format,type,source);    // 00424668
 else gl::glTexSubImage2D_0073d294(0x0de1,0,0,0,width,height,format,type,source);                       // 00424686
 while(height*width>1){                                              // 00424695 / 00424922
  const I half_height=height/2,half_width=width/2;                   // cdq; sub; sar
  unsigned char* target=static_cast<unsigned char*>(live_operator_new(static_cast<std::uint32_t>(half_width*half_height*bpp))); // 006FC0D2
  if(half_height>0){
   I a_half=0;                                                       // [esp+1Ch]: row A pixel index / 2
   I row_b=width;                                                    // [esp+20h]: row B pixel index
   I out_row=0;                                                      // [esp+18h]
   for(I rows=half_height;rows!=0;--rows){
    for(I column=0;column<half_width;++column){
     const I a0=2*(a_half+column),b0=row_b+2*column;
     if(bpp==2){                                                     // 00424702: 4444 nibble average
      const unsigned char* pa=source+a0*2;const unsigned char* pb=source+b0*2;
      unsigned char* pd=target+(out_row+column)*2;
      for(int byte=0;byte<2;++byte){
       const I s0=pb[byte+2],s1=pb[byte],s2=pa[byte+2],s3=pa[byte];
       const I high=((s0>>4)+(s1>>4)+(s2>>4)+(s3>>4))/4;
       const I low=((s0&15)+(s1&15)+(s2&15)+(s3&15))/4;
       pd[byte]=static_cast<unsigned char>(static_cast<unsigned char>(high<<4)|static_cast<unsigned char>(low));
      }
     }else{                                                          // 00424805: per-channel box filter
      const unsigned char* pb0=source+b0*bpp;const unsigned char* pb1=source+(b0+1)*bpp;
      const unsigned char* pa0=source+a0*bpp;unsigned char* pd=target+(out_row+column)*bpp;
      for(I c=0;c<bpp;++c)pd[c]=static_cast<unsigned char>((pa0[c+bpp]+pb0[c]+pb1[c]+pa0[c])/4);
     }
    }
    a_half+=width;row_b+=2*width;out_row+=half_width;                // 0042488C..004248A9
   }
  }
  live_free(source);                                                 // 004248C0
  width>>=1;height>>=1;++level;                                      // sar ebx / sar edi / inc
  source=target;                                                     // [esp+28h]
  if(low_byte(update)==0)gl::glTexImage2D_0073d280(0x0de1,level,bpp,width,height,0,format,type,source); // 00424903
  else gl::glTexSubImage2D_0073d294(0x0de1,level,0,0,width,height,format,type,source);                  // 00424917
 }
 live_free(source);                                                  // 0042492C
}

// ---------------------------------------------------------------- 00424980 GLRender::CreateTexture
void __cdecl gl_create_texture_00424980(Basic* t,U update,I index){
 I height=table(t).s4c(t,nullptr);                            // edi
 const I bpp=table(t).s50(t,nullptr);                         // [esp+1Ch]
 void* const image=table(t).s3c(t,nullptr);                    // [esp+18h]
 const I num_x=table(t).s58(t,nullptr);                       // ebp
 const I num_y=table(t).s5c(t,nullptr);                       // [esp+2Ch]
 I width=table(t).s48(t,nullptr);                             // ebx
 I w=width/num_x;                                                    // [esp+10h]
 I h=height/num_y;                                                   // [esp+14h]
 const I x=(index%num_x)*w;
 const I y=(index/num_x)*h;
 void* buffer;                                                       // [esp+2Ch]
 if(num_x==1 && num_y==1)buffer=image;                               // 004249F6
 else buffer=image_get_sub_image_0045f000(image,width,height,bpp,x,y,w,h); // 00424A14
 if(low_byte(table(t).s1c(t,nullptr))!=0){                    // IsBumpMap 00424A24
  void* result=buffer;
  if(gl::BumpMapAvailable_0045fb10()!=0){                            // 00424A2B
   const U normal=table(t).s20(t,nullptr);                    // IsNormalMap: raw EAX pushed (D12)
   const float scale=table(t).s78(t,nullptr);             // GetBumpMapScaling (ST0)
   build_texture_normal_map_00420ea0(buffer,scale,&w,&h,bpp,0,normal); // 00424A59
   result=buffer;                                                    // esi=[esp+40h]
  }
  if(result!=image)live_free(result);                                // 00424A67..00424A72
  return;
 }
 const U mip=table(t).s6c(t,nullptr);                        // movzx [esp+20h]
 if(table(t).s64(t,nullptr)>0){                               // compressed 00424A94
  I level=0;                                                         // [esp+34h]
  const I block=block_bytes(bpp);                                    // [esp+24h]
  const char* cursor=static_cast<const char*>(table(t).s3c(t,nullptr)); // [esp+1Ch]
  const void* data=table(t).s3c(t,nullptr);
  const I size0=table(t).s64(t,nullptr);
  const U format=format_entry(table(t).s8c(t,nullptr));
  compressed_tex_image_2d()(0x0de1,0,format,width,height,0,size0,data); // 00424AF2
  if(table(t).s6c(t,nullptr)!=0){                            // 00424AFC
   I previous=table(t).s64(t,nullptr);                        // ebp
   for(;;){
    width>>=1;height>>=1;                                            // sar ebx / sar edi
    if(width==0){if(height==0)break;width=1;}
    if(height==0)height=1;
    ++level;
    cursor+=previous;                                                // [esp+1Ch]+ebp
    previous=quarter_up(height)*quarter_up(width)*block;
    const U level_format=format_entry(table(t).s8c(t,nullptr));
    compressed_tex_image_2d()(0x0de1,level,level_format,width,height,0,previous,cursor); // 00424B85
   }
  }
 }else{
  U format;                                                          // [esp+34h]
  U type=0x1401;                                                     // edi
  I internal=bpp;                                                    // ebx
  if(bpp==4)format=0x1908;
  else if(bpp==3)format=0x1907;
  else{
   format=(bpp!=2?1u:0u)+0x1908u;
   if(bpp==2){type=0x8033;internal=4;}                               // 00424CA9
  }
  if(mip!=0){                                                        // 00424BA5
   if(gl::SGIMipmapAvailable_0045f910()!=0){
    gl::glTexParameteri_0073d23c(0x0de1,0x8191,1);                   // GENERATE_MIPMAP_SGIS
    gl::glTexImage2D_0073d280(0x0de1,0,internal,w,h,0,format,type,buffer); // 00424BEF
   }else if(low_byte(update)!=0 || bpp==2){                          // 00424CBC / 00424CC3
    aur_build_2d_mip_maps_00424580(w,h,bpp,buffer,update);           // 00424CFB (full update word)
   }else{
    gl::gluBuild2DMipmaps_0073d030(0x0de1,internal,w,h,format,type,buffer); // 0070C120 -> [0073D030]
   }
  }else if(low_byte(update)==0){
   gl::glTexImage2D_0073d280(0x0de1,0,internal,w,h,0,format,type,buffer); // 00424D0E -> 00424BD0
  }else{
   gl::glTexSubImage2D_0073d294(0x0de1,0,0,0,w,h,format,0x1401,buffer); // 00424D38 (literal type)
  }
 }
 texture_parameters(t,mip);                                          // 00424BF5
 if(buffer!=image)live_free(buffer);                                 // 00424D52..00424D5D
}

// ---------------------------------------------------------------- 00424DD0 GLRender::CreateProcessedTexture
void __cdecl gl_create_processed_texture_00424dd0(Basic* t,U update,I index){
 const I height=table(t).s4c(t,nullptr);                      // [esp+24h]
 const I bpp=table(t).s50(t,nullptr);                         // edi, [esp+28h]
 const char* const image=static_cast<const char*>(table(t).s3c(t,nullptr)); // [esp+34h]
 const I num_x=table(t).s58(t,nullptr);                       // ebx
 const I num_y=table(t).s5c(t,nullptr);                       // ebp
 I w=table(t).s48(t,nullptr)/num_x;                           // [esp+14h]
 I h=height/num_y;                                                   // [esp+10h]
 const char* data;                                                   // ebx
 if(num_x==1 && num_y==num_x)data=image;                             // 00424E34
 else data=image+get_compressed_contact_sheet_offset_00424d70(index,w,bpp); // 00424E41
 const U mip=table(t).s6c(t,nullptr);                        // movzx ebp; [esp+2Ch]
 if(low_byte(table(t).s1c(t,nullptr))!=0 && bpp==1){          // 00424E63..00424E6D
  if(gl::BumpMapAvailable_0045fb10()!=0){
   const U normal=table(t).s20(t,nullptr);                    // raw EAX pushed (D12)
   const float scale=table(t).s78(t,nullptr);
   build_texture_normal_map_00420ea0(const_cast<char*>(data),scale,&w,&h,bpp,0,normal); // 00424E99
  }
  if(data!=image)live_free(const_cast<char*>(data));                 // 00424EA1..00424EAC (observed quirk)
  return;
 }
 if(table(t).s64(t,nullptr)>0){                               // compressed 00424EC0
  I level=0;                                                         // [esp+34h]
  const char* cursor=data;                                           // [esp+3Ch]
  const I block=block_bytes(bpp);                                    // [esp+18h]
  I size=quarter_up(h)*quarter_up(w)*block;                          // edi
  const U format=format_entry(table(t).s8c(t,nullptr));
  compressed_tex_image_2d()(0x0de1,0,format,w,h,0,size,data);        // 00424F33
  if(table(t).s6c(t,nullptr)!=0){                            // 00424F3D
   I lw=w,lh=h;                                                      // ebp / ebx
   for(;;){
    lw>>=1;lh>>=1;
    if(lw==0){if(lh==0)break;lw=1;}
    if(lh==0)lh=1;
    ++level;
    cursor+=size;                                                    // [esp+3Ch]+edi
    size=quarter_up(lh)*quarter_up(lw)*block;
    const U level_format=format_entry(table(t).s8c(t,nullptr));
    compressed_tex_image_2d()(0x0de1,level,level_format,lw,lh,0,size,cursor); // 00424FC5
   }
  }
 }else{
  U format;                                                          // [esp+34h]
  U type=0x1401;                                                     // [esp+3Ch]
  I internal=bpp;                                                    // [esp+18h]
  if(bpp==4)format=0x1908;
  else if(bpp==3)format=0x1907;
  else{
   format=(bpp!=2?1u:0u)+0x1908u;
   if(bpp==2){type=0x8033;internal=4;}                               // 00425054
  }
  if(mip!=0){                                                        // 00424FE6
   I level=0;                                                        // [esp+1Ch]
   const char* pixels=static_cast<const char*>(table(t).s3c(t,nullptr)); // ebp = GetImage (not data)
   if(low_byte(update)!=0)gl::glTexSubImage2D_0073d294(0x0de1,0,0,0,w,h,format,0x1401,pixels); // 00425027
   else gl::glTexImage2D_0073d280(0x0de1,0,internal,w,h,0,format,type,pixels);                  // 00425088
   if(table(t).s6c(t,nullptr)!=0){                           // 00425092
    I lh=h,lw=w;                                                     // ebx / edi
    I size=lh*lw*bpp;                                                // [esp+20h]
    lw>>=1;lh>>=1;
    I remaining=table(t).s54(t,nullptr)-1;                    // GetNumMipMaps-1, [esp+24h]
    if(remaining>0){
     do{
      if(lw==0)lw=1;
      if(lh==0)lh=1;
      pixels+=size;                                                  // ebp+=[esp+20h]
      size=lh*lw*bpp;                                                // ebx*edi*[esp+28h]
      ++level;
      if(low_byte(update)!=0)gl::glTexSubImage2D_0073d294(0x0de1,level,0,0,lw,lh,format,type,pixels); // 0042511E
      else gl::glTexImage2D_0073d280(0x0de1,level,internal,lw,lh,0,format,type,pixels);             // 00425135
      lw>>=1;lh>>=1;
     }while(--remaining!=0);                                         // dec; jne 004250D0
    }
   }
  }else if(low_byte(update)!=0){
   gl::glTexSubImage2D_0073d294(0x0de1,0,0,0,w,h,format,0x1401,data); // 00425174 (literal type)
  }else{
   gl::glTexImage2D_0073d280(0x0de1,0,internal,w,h,0,format,type,data); // 0042519E
  }
 }
 if(low_byte(table(t).s1c(t,nullptr))!=0)return;              // 004251A8 IsBumpMap -> skip params
 texture_parameters(t,mip);                                          // 004251B3..0042523F
}

// ---------------------------------------------------------------- 00423F90 GLRender::CreateCubeTexture
void __cdecl gl_create_cube_texture_00423f90(Basic* t,U update,I index){
 for(I face=0;face<6;++face){                                        // [esp+14h]
  const I width=table(t).s48(t,nullptr);                      // ebx
  const I height=table(t).s4c(t,nullptr);
  const I bpp=table(t).s50(t,nullptr);                        // [esp+2Ch]
  const I face_height=height/6;                                      // imul 2AAAAAABh; +sign (00423FC4)
  const U target=cells.cube_face_target_0078db50[face]+0x8515u;      // [esp+18h]
  const unsigned char* face_image=static_cast<const unsigned char*>(table(t).s3c(t,nullptr))
                                  +face_height*bpp*width*face;        // [esp+20h]
  const I w=width/table(t).s58(t,nullptr);                    // edi
  const I h=face_height/table(t).s5c(t,nullptr);              // ebp
  const I x=(index%table(t).s58(t,nullptr))*w;                // [esp+10h]
  const I y=(index/table(t).s58(t,nullptr))*h;                // [esp+1Ch]
  const void* buffer;                                                // [esp+1Ch]
  if(table(t).s58(t,nullptr)==1 && table(t).s5c(t,nullptr)==1)buffer=face_image; // 00424054..00424066
  else buffer=image_get_sub_image_0045f000(face_image,width,face_height,bpp,x,y,w,h); // 0042408E
  const U rotation=cells.cube_face_rotation_0078db38[face];          // [ecx*4+0078DB38]
  void* rotated=image_quarter_rotated_0041fbd0(buffer,w,h,table(t).s50(t,nullptr),rotation); // 004240B5
  const I components=table(t).s50(t,nullptr);                 // ebx
  U format;
  if(components==4)format=0x1908;
  else if(components==3)format=0x1907;
  else format=(components==2?1u:0u)+0x1909u;                         // LUMINANCE / LUMINANCE_ALPHA
  if(table(t).s6c(t,nullptr)!=0){                            // 004240FD
   gl::gluBuild2DMipmaps_0073d030(target,components,w,h,format,0x1401,rotated);         // 0042411B -> 0070C120
  }else if(low_byte(update)!=0){
   gl::glTexSubImage2D_0073d294(target,0,0,0,w,h,format,0x1401,rotated);               // 0042413E
  }else{
   gl::glTexImage2D_0073d280(target,0,components,w,h,0,format,0x1401,rotated);         // 0042414E
  }
  if(rotated!=buffer)live_free(rotated);                             // 0042415C..00424161
  if(buffer!=face_image)live_free(const_cast<void*>(buffer));        // 00424169..00424170
 }
 cube_parameters(t);                                                 // 0042418A..0042421D
}

// ---------------------------------------------------------------- 00424230 GLRender::CreateProcessedCubeTexture
void __cdecl gl_create_processed_cube_texture_00424230(Basic* t,U update,I){
 const I width=table(t).s48(t,nullptr);                       // ebx, [esp+2Ch]
 const I face_height=table(t).s4c(t,nullptr)/6;               // ebp, [esp+18h]
 const I bpp=table(t).s50(t,nullptr);                         // [esp+10h]
 const U mip=table(t).s6c(t,nullptr);                        // movzx [esp+1Ch]
 const char* cursor=static_cast<const char*>(table(t).s3c(t,nullptr)); // esi
 U target=0x8515;                                                    // [esp+38h]
 for(I faces=6;faces!=0;--faces,++target){                           // [esp+30h]
  if(table(t).s64(t,nullptr)>0){                              // 0042429B
   I level=0;                                                        // [esp+14h]
   const I block=block_bytes(bpp);                                   // [esp+28h]
   const I size0=table(t).s64(t,nullptr);
   const U format=format_entry(table(t).s8c(t,nullptr));
   compressed_tex_image_2d()(target,0,format,width,face_height,0,size0,cursor); // 004242EB
   if(mip!=0){                                                       // 004242F1 (else: cursor NOT advanced, observed)
    I previous=table(t).s64(t,nullptr);                       // eax
    I lw=width,lh=face_height;                                       // ebx / ebp
    for(;;){
     lw>>=1;lh>>=1;
     if(lw==0){if(lh==0)break;lw=1;}
     if(lh==0)lh=1;
     ++level;
     cursor+=previous;                                               // add esi,eax
     previous=quarter_up(lh)*quarter_up(lw)*block;                   // [esp+24h]
     const U level_format=format_entry(table(t).s8c(t,nullptr));
     compressed_tex_image_2d()(target,level,level_format,lw,lh,0,previous,cursor); // 00424377
    }
    cursor+=previous;                                                // 00424499 add esi,eax
   }
  }else{
   const U format=(bpp==4?1u:0u)+0x1907u;                            // 0042438D (RGB/RGBA only)
   if(mip!=0){                                                       // 004243A8
    I level=0;                                                       // [esp+20h]
    if(low_byte(update)!=0)gl::glTexSubImage2D_0073d294(target,0,0,0,width,face_height,format,0x1401,cursor); // 004243C7
    else gl::glTexImage2D_0073d280(target,0,bpp,width,face_height,0,format,0x1401,cursor);                  // 004243DB
    I size=bpp*face_height*width;                                    // [esp+14h]
    I lw=width>>1,lh=face_height>>1;                                 // ebp / ebx
    I remaining=table(t).s54(t,nullptr)-1;                    // 004243FD
    if(remaining>0){
     do{
      if(lw==0)lw=1;
      if(lh==0)lh=1;
      cursor+=size;                                                  // add esi,ecx
      size=lh*lw*bpp;
      ++level;
      if(low_byte(update)!=0)gl::glTexSubImage2D_0073d294(target,level,0,0,lw,lh,format,0x1401,cursor); // 00424460
      else gl::glTexImage2D_0073d280(target,level,bpp,lw,lh,0,format,0x1401,cursor);                  // 00424478
      lw>>=1;lh>>=1;
     }while(--remaining!=0);
    }
    cursor+=size;                                                    // 0042448D..00424499
   }else if(low_byte(update)!=0){                                    // 0042449D (cursor NOT advanced, observed)
    gl::glTexSubImage2D_0073d294(target,0,0,0,width,face_height,format,0x1401,cursor); // 004244AE
   }else{
    gl::glTexImage2D_0073d280(target,0,bpp,width,face_height,0,format,0x1401,cursor);  // 004244C2
   }
  }
 }
 cube_parameters(t);                                                 // 004244E0..00424573
}

// ---------------------------------------------------------------- Basic vtable 0073F160 getters
U __fastcall texture_is_bump_map_0041eef0(void* self,void*){
 Basic* b=referenced_basic(self);
 if(b==nullptr)return 0;                                             // eax already 0; xor al,al
 if(field(b->d4)!=0 && texture::texture_call_processed_30_al(b)!=0)return 1; // 0041EF11
 const I kind=field(b->d4);                                          // reload 0041EF18
 if(kind==1 && field(b->f70)==kind)return 1;                         // cmp [esi+70h],eax
 if(kind==2 && field(b->f70)==4)return 1;
 return 0;
}
U __fastcall texture_is_normal_map_0041ef50(void* self,void*){
 Basic* b=referenced_basic(self);
 if(b==nullptr)return 0;
 return (field(b->d4)==2 && field(b->f70)==4)?1u:0u;                 // 0041EF60/0041EF69
}
void* __fastcall texture_get_image_0041efc0(void* self,void*){Basic* b=referenced_basic(self);return b?as_pointer(b->f44):nullptr;}
I __fastcall texture_get_width_0041f020(void* self,void*){Basic* b=referenced_basic(self);return b?field(b->f68):0;}
I __fastcall texture_get_height_0041f040(void* self,void*){Basic* b=referenced_basic(self);return b?field(b->f6c):0;}
I __fastcall texture_get_bpp_0041f060(void* self,void*){Basic* b=referenced_basic(self);return b?field(b->f70):0;}
I __fastcall texture_get_num_mip_maps_0041f080(void* self,void*){Basic* b=referenced_basic(self);return b?static_cast<I>(b->ce):0;}
I __fastcall texture_get_num_x_0041f0a0(void* self,void*){Basic* b=referenced_basic(self);return b?static_cast<I>(b->c2):0;}
I __fastcall texture_get_num_y_0041f0c0(void* self,void*){Basic* b=referenced_basic(self);return b?static_cast<I>(b->c4):0;}
I __fastcall texture_get_clamp_0041f140(void* self,void*){Basic* b=referenced_basic(self);return b?static_cast<I>(b->c0):0;}
I __fastcall texture_get_compressed_size_0041f1a0(void* self,void*){Basic* b=referenced_basic(self);return b?field(b->f5c):0;}
U8 __fastcall texture_get_mip_map_0041f0e0(void* self,void*){Basic* b=referenced_basic(self);return b?flag(b,0xdc):U8(0);}
U8 __fastcall texture_get_filter_0041f100(void* self,void*){Basic* b=referenced_basic(self);return b?flag(b,0xdd):U8(0);}
float __fastcall texture_get_bump_map_scaling_0041f160(void* self,void*){
 Basic* b=referenced_basic(self);
 return b?static_cast<const volatile float&>(b->f18):0.0f;           // fld [eax+18h] / fld [0073D700]=0.0f
}
I __fastcall texture_get_compression_format_0041f1c0(void* self,void*){
 Basic* b=referenced_basic(self);
 if(b!=nullptr && field(b->f5c)!=0)return field(b->f74);             // 0041F1D0..0041F1D7
 return 7;                                                           // mov eax,7
}
}
