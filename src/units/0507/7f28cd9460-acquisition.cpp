// acquisition.cpp - lane-texture-epoch PROPOSAL, authored against texture.hpp epoch E1.
// Bodies: LoadImageA 0041F6A0 (CAurTextureBasic::LoadImageA) and the typed core of
// AurGetImageWrapper 0041EA20 (entry thunk in texture_x86.asm).
// Evidence: S/task-6/image-acquisition-r1/artifacts/original/{0041f6a0.txt,0041ea20.txt} (EVIDENCE.json pins,
// listing == pinned PE bytes). Address comments cite the instruction that establishes each behavior.
#include "f31f0dbce3-texture.hpp"
#include "f7b45a84e2-ini_c_primitives.h" // header_root = M40 modern-ini-support

namespace acquisition {
namespace {
using texture::Basic; using texture::U; using texture::I;
using texture::field_load; using texture::field_store;

template<class T> T cell_load(T& cell){return static_cast<volatile T&>(cell);}
template<class T> void cell_store(T& cell,T value){static_cast<volatile T&>(cell)=value;}
void put0(Signed* p){*static_cast<volatile Signed*>(p)=0;}
// Low-byte view of a caller argument cell (char may alias any object); MSVC char is signed, so the
// signed char conversion is the original MOVSX sign extension.
Signed low_byte(const void* cell){return static_cast<Signed>(static_cast<signed char>(*static_cast<const volatile char*>(cell)));}

// 0041F849/0041F850: rep movsd (count>>2) then rep movsb (count&3), forward, no zero-count access.
void copy_words_then_bytes(U destination,const char* source,U count){
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
char* pointer(U address){return reinterpret_cast<char*>(address);}
U address(const void* p){return reinterpret_cast<U>(p);}
}

// ---------------------------------------------------------------------------------------------
// 0041EA20 core. caller_cells is the caller's own argument area (see texture_x86.asm).
extern "C" char* __cdecl get_image_wrapper_core_0041ea20(WrapperCells* c){
 // 0041EA20..0041EA29: force the caller's request cell to zero in place.
 if(*cells.force_zero_request_007a4700!=0)cell_store(c->request,0);
 // 0041EA31..0041EA49: capture output pointers BEFORE any cell overwrite (ecx=p7, eax=p6, ebp=compressed,
 // ebx=name, esi=height, edi=width); 0041EA52 alpha cell read.
 Signed* const p7=cell_load(c->p7);
 Signed* const p6=cell_load(c->p6);
 Signed* const compressed=cell_load(c->compressed);
 const char* const name=cell_load(c->name);
 Signed* const height=cell_load(c->height);
 Signed* const width=cell_load(c->width);
 float* const alpha=cell_load(c->alpha);
 // 0041EA59, 0041EA60, 0041EA6F: zero *compressed, *p6, *p7 in that order.
 put0(compressed);put0(p6);put0(p7);
 // 0041EA75: processed callback first and unconditionally; encoding byte = low byte of incoming arg5 cell
 // (0041EA67), mip byte = low byte of incoming arg6 cell (0041EA4D).
 char* const processed=(*cells.processed_0078d420)(name,width,height,reinterpret_cast<char*>(&c->compressed),
                                                     compressed,p6,p7,alpha,reinterpret_cast<char*>(&c->p6));
 if(processed){
  // 0041EA86..0041EAA2: components32 <- movsx encoding; mip16 <- movsx mip byte; processed byte <- 1.
  const Signed encoding=low_byte(&c->compressed);
  Signed* const components=cell_load(c->components);
  std::int16_t volatile* const mip16=cell_load(c->mip16);
  *static_cast<volatile Signed*>(components)=encoding;
  *mip16=static_cast<std::int16_t>(low_byte(&c->p6));
  *cell_load(c->processed)=1;
  // 0041EAA5 reread *compressed, 0041EAA9 reread *components.
  const Signed compressed_bytes=*static_cast<volatile Signed*>(compressed);
  const Signed kind=*static_cast<volatile Signed*>(components);
  if(compressed_bytes!=0){
   if(kind==3)*cell_load(c->format)=7;         // 0041EB42..0041EB49
   else if(kind==4)*cell_load(c->format)=8;    // 0041EABD..0041EAC4
  }else{
   if(kind==1)*cell_load(c->format)=3;         // 0041EAF9..0041EB00
   else if(kind==3)*cell_load(c->format)=1;    // 0041EAEA..0041EAF1
   else if(kind==4)*cell_load(c->format)=0;    // 0041EADB..0041EAE2
  }
  return processed;
 }
 // 0041EB08: request cell reread (after the possible forced zero).
 Signed* components;
 if(cell_load(c->request)!=0&&acquisition_compression_available_0045f950()!=0){ // 0041EB10..0041EB17
  float* const compressed_alpha=cell_load(c->alpha);                            // 0041EB19
  components=cell_load(c->components);                                          // 0041EB1F
  char* const image=(*cells.compressed_0078d40c)(name,width,height,components,compressed,compressed_alpha); // 0041EB27
  if(image){
   const Signed kind=*static_cast<volatile Signed*>(components);                // 0041EB34
   if(kind==3)*cell_load(c->format)=7;                                          // 0041EB42
   else if(kind==4)*cell_load(c->format)=8;                                     // 0041EABD (via 0041EB3D)
   return image;
  }
 }else{
  components=cell_load(c->components);                                          // 0041EB51
 }
 // 0041EB55..0041EB62: format cell, then alpha cell; 16-bit raster callback.
 Signed* const format=cell_load(c->format);
 float* const raster_alpha=cell_load(c->alpha);
 char* const image16=(*cells.raster16_0078d414)(name,width,height,raster_alpha,format);
 *static_cast<volatile Signed*>(components)=2;                                  // 0041EB6D, unconditional
 if(image16){
  (void)*cells.raster16_0078d414;                                               // 0041EB76 dead cell reload (observed)
  return image16;
 }
 // 0041EB81..0041EB85: raw raster callback; unsigned outputs through the corresponding-unsigned rule.
 char* const raster=(*cells.raster_0078d404)(name,as_corresponding_unsigned(width),as_corresponding_unsigned(height),
                                             as_corresponding_unsigned(components));
 if(!raster)return raster;                                                      // 0041EB90 (EAX==0)
 const Signed kind=*static_cast<volatile Signed*>(components);                  // 0041EB92
 if(kind==1)*cell_load(c->format)=3;                                            // 0041EBBE..0041EBC2
 else if(kind==3)*cell_load(c->format)=1;                                       // 0041EBAF..0041EBB6
 else if(kind==4)*cell_load(c->format)=0;                                       // 0041EBA0..0041EBA7
 return raster;
}

// ---------------------------------------------------------------------------------------------
// 0041F6A0 LoadImageA.
extern "C" char* __fastcall load_image_a_0041f6a0(Basic* self,void*){
 texture::texture_call_free_b0(self);                                           // 0041F6A9..0041F6AD vtable+B0
 // 0041F6B3..0041F6D0: empty derived -> byte copy of name (+78) to derived (+98) including NUL; the
 // original addresses both through one object base, so the copy is expressed on Basic's bytes.
 char volatile* const bytes=reinterpret_cast<char volatile*>(self);
 if(bytes[offsetof(Basic,derived)]==0){
  U i=0;char ch;
  do{ch=bytes[offsetof(Basic,name)+i];bytes[offsetof(Basic,derived)+i]=ch;++i;}while(ch!=0);
 }
 const std::int16_t count=self->c6;                                             // 0041F6D2
 if(count!=0){
  // 0041F6E4..0041F6ED: pointer slots, sign-extended C6 << 2 in the 32-bit domain, operator new 006FA7E6.
  char** const slots=static_cast<char**>(live_operator_new(static_cast<U>(static_cast<I>(count))<<2));
  const std::int16_t first=self->c6;                                            // 0041F6F5
  field_store(self->f6c,0);                                                     // 0041F6FE
  if(first>0){
   // Frame locals of the original ([esp+10] width, [esp+18] height, [esp+1C] components, [esp+14] compressed,
   // [esp+20..9F] name). DOCUMENTED DEVIATION (integrator decision D2): the original has no initializing store
   // for width/height before the first 0041EA20 call (0041F710..0041F761) and commits them at 0041F7C5/0041F7CD;
   // if no callback writes them it would commit stack garbage. They are value-initialized to 0 here to avoid
   // reading an indeterminate value. components is always written by 0041EA20 (0041EA93 or 0041EB6D);
   // compressed is zeroed by 0041EA20 at 0041EA59.
   Signed width=0,height=0,components,compressed;char name[0x80];
   I index=0;
   do{
    slots[index]=nullptr;                                                       // 0041F722
    kotor_functional::modern_ini_sprintf(name,"%s%d",self->derived,index);      // 0041F729, literal 0073EDD0
    char* image=get_image_wrapper_0041ea20(name,&width,&height,&components,&compressed,&self->f60,&self->f64,
                                           &self->f74,&self->f20,&self->flags[13],&self->ce,0); // 0041F72E..0041F761
    slots[index]=image;                                                         // 0041F76B
    if(!image){
     kotor_functional::modern_ini_sprintf(name,"%s0%d",self->derived,index);    // 0041F782, literal 0073EDC8
     image=get_image_wrapper_0041ea20(name,&width,&height,&components,&compressed,&self->f60,&self->f64,
                                      &self->f74,&self->f20,&self->flags[13],&self->ce,0); // 0041F787..0041F7BA
     slots[index]=image;                                                        // 0041F7C2
    }
    // 0041F7C5..0041F7D7: commit locals width/components/height to +68/+70/+6C.
    const Signed w=cell_load(width),k=cell_load(components),h=cell_load(height);
    field_store(self->f68,w);field_store(self->f70,k);field_store(self->f6c,h);
    ++index;
   }while(index<static_cast<I>(self->c6));                                      // 0041F7DA..0041F7E4 movsx reload, JL
  }
  // 0041F7EA..0041F7FE: wrapped DWORD product movsx(C6)*[+70]*[+6C]*[+68]; operator new.
  U size=static_cast<U>(static_cast<I>(self->c6));
  size*=static_cast<U>(field_load(self->f70));
  size*=static_cast<U>(field_load(self->f6c));
  size*=static_cast<U>(field_load(self->f68));
  void* const combined=live_operator_new(size);
  const std::int16_t copies=self->c6;                                           // 0041F806
  self->f44=address(combined);                                                  // 0041F80E
  if(copies>0){
   U destination=address(combined);
   I index=0;
   do{
    U bytes_now=static_cast<U>(field_load(self->f70));                         // 0041F830..0041F837
    bytes_now*=static_cast<U>(field_load(self->f6c));
    bytes_now*=static_cast<U>(field_load(self->f68));
    copy_words_then_bytes(destination,slots[index],bytes_now);                  // 0041F83F..0041F850
    const Signed w=field_load(self->f68),h=field_load(self->f6c),k=field_load(self->f70); // 0041F852..0041F858
    destination+=static_cast<U>(w)*static_cast<U>(h)*static_cast<U>(k);         // 0041F85B..0041F865
    const ReleaseRaster release=*cells.release_007a46f8;                        // 0041F869
    char* const image=slots[index];                                             // 0041F879..0041F87D
    if(release!=nullptr){
     const Word processed_byte=self->flags[13];                                 // 0041F884 zero-extended E5
     const Signed p7=field_load(self->f64),p6=field_load(self->f60);            // 0041F88B, 0041F88F
     (*cells.release_007a46f8)(image,w,h,k,0,p6,p7,processed_byte);             // 0041F899 cell reloaded at call
    }else{
     live_free(image);                                                          // 0041F8A5 free 006FA390
    }
    ++index;
   }while(index<static_cast<I>(self->c6));                                      // 0041F8B1..0041F8BF
  }
  if(texture::texture_call_flag_0c_al(self)==0)self->c4=self->c6;              // 0041F8C5..0041F8D8
  self->flags[11]=0;                                                            // 0041F8E0 E3=0
  live_free(slots);                                                             // 0041F8E7
  const I live_count=self->c6;                                                  // 0041F8EC
  const U result=self->f44;                                                     // 0041F8F3
  const U rows=static_cast<U>(live_count)*static_cast<U>(field_load(self->f6c)); // 0041F8F9
  field_store(self->f6c,static_cast<I>(rows));                                  // 0041F900
  return pointer(result);
 }
 // Single image 0041F90B..0041F932: request = !DB && C4*C2==1 && !+34.
 Signed request=0;
 if(self->flags[3]==0){
  const I rows=self->c4,columns=self->c2;                                       // 0041F917, 0041F91E
  if(static_cast<U>(rows)*static_cast<U>(columns)==1u&&self->f34==0)request=1;
 }
 // 0041F937..0041F96D: actual Basic field destinations.
 char* const image=get_image_wrapper_0041ea20(self->derived,&self->f68,&self->f6c,&self->f70,&self->f5c,&self->f60,
                                              &self->f64,&self->f74,&self->f20,&self->flags[13],&self->ce,request);
 self->f44=address(image);                                                      // 0041F972
 if(*cells.release_007a46f8!=nullptr&&image!=nullptr){                          // 0041F975..0041F984
  self->flags[11]=1;                                                            // 0041F987 E3=1
  return image;                                                                 // EAX from the call
 }
 const U again=self->f44;                                                       // 0041F996 reread
 self->flags[11]=0;                                                             // 0041F99A
 return pointer(again);
}
}
