// CAurFontInfo::ParseField 00422210, ReadVectorList 00421FF0, Vector-list Allocate 00421310,
// CAurTexture FontInfo getter 0041EFA0 (lane-font packet 6).
// Evidence: lane-gui-draw/exe-text.asm 00422210..00422357, 00421FF0..004221B8, 00421310..00421395,
// 0041EFA0..0041EFB6; strings 0073EE5C..0073EEC4, 0073EE54 "%d %c", 0073EE4C "endlist".
// FontInfo words: +00 numchars(int), +04 fontheight, +08 baselineheight, +0C texturewidth, +10 spacingR,
// +14 spacingB (float), +18 upperleft Vector list {data,size,capacity}, +24 lowerright list (12-byte Vectors).
#include "43e0af9c1e-font_txi.hpp"
#include <cstring>
namespace font_txi {
namespace {
I SignedBits(U v){return v&0x80000000u?-1-static_cast<I>(~v):static_cast<I>(v);}
U Load(U address){return *reinterpret_cast<volatile U*>(address);}
void Store(U address,U value){*reinterpret_cast<volatile U*>(address)=value;}
U Compare(const char* keyword,char* line){return static_cast<U>(kotor_stricmp_0070acaf(keyword,firstword_00463530(line)));}
// E1 edit LF2: typed FontInfo float members (+04..+14), no word-array cast.
volatile float* InfoFloat(FontInfo* info,unsigned word){
 switch(word){case 1:return &info->fontheight;case 2:return &info->baselineheight;case 3:return &info->texturewidth;
  case 4:return &info->spacing_r;default:return &info->spacing_b;}
}
}
}
using namespace font_txi;

// 00421310: ECX list, one stack word (n), RET4. Capacity stored first; new block zeroed; old elements
// (size, reloaded per iteration) copied 12 bytes each; old block freed. A failed allocation leaves data 0
// and the copy faults exactly as the original would.
extern "C" void __fastcall vector_list_allocate_00421310(List* list,void*,I n){
 U old=list->data;                                                    // 0042131D ebx
 list->capacity=n;                                                    // 00421323
 void* memory=live_operator_new(static_cast<U>(n)*12u);              // 00421326 006FA7E6
 U data=0;
 if(memory){
  data=reinterpret_cast<U>(memory);
  if(SignedBits(static_cast<U>(n)-1u)>=0){                             // 00421334..00421339 jl
   U remaining=static_cast<U>(n);U cell=data;
   do {Store(cell,0);Store(cell+4,0);Store(cell+8,0);cell+=12;}while(--remaining); // 00421340..0042134C
  }
 }
 list->data=data;                                                     // 00421352
 for(I i=0;i<list->size;++i){                                         // 00421354..00421380 (size reloaded)
  U from=old+static_cast<U>(i)*12u,to=list->data+static_cast<U>(i)*12u;
  Store(to,Load(from));Store(to+4,Load(from+4));Store(to+8,Load(from+8));
 }
 if(old)live_free(reinterpret_cast<void*>(old));                       // 00421389 006FA390
}

// 00421FF0: cdecl(line, list). Returns the raw EAX of each original exit.
extern "C" U __cdecl read_vector_list_00421ff0(char* line,List* list){
 I count=0;char tail;                                                 // 0042200B [esp+14h]=0; [esp+13h]
 const I scanned=kotor_sscanf_006fae08(line,"%d %c",&count,&tail);   // 00422013 (0073EE54)
 if(scanned>0){                                                       // 0042201F jle
  U eax=static_cast<U>(count);
  if(count==0){                                                       // 00422031
   U data=list->data;
   if(data){live_free(reinterpret_cast<void*>(data));eax=static_cast<U>(count);list->data=static_cast<U>(count);} // 00422039..00422046
   list->capacity=0;                                                  // 00422048
  }else{
   vector_list_allocate_00421310(list,nullptr,count);                 // 00422054
   eax=static_cast<U>(count);                                         // 00422059
  }
  list->size=count;                                                   // 00422060
  if(scanned==1){                                                     // 0042205D
   if(count<=0)return eax;                                            // 00422067 jle -> 004221B1
   I index=0;U offset=0;
   for(;;){
    char* next=resource_next_0044bfa0();                              // 00422071
    if(!next)return 0;                                                // 00422078 (EAX=0)
    parse_vector_00463570(next,reinterpret_cast<volatile float*>(list->data+offset)); // 0042207E..00422084
    eax=static_cast<U>(list->size);++index;offset+=12u;               // 00422089..00422090
    if(index>=SignedBits(eax))return eax;                             // 00422093 jl
   }
  }
  const U bytes=static_cast<U>(count)*12u;                            // 0042209F..004220A2
  const void* source=resource_read_0044bfd0(bytes);                   // 004220A6 (binary block)
  std::memcpy(reinterpret_cast<void*>(list->data),source,bytes);       // 004220AB..004220C0 rep movs
  *line=0;                                                            // 004220C5 mov byte [ebx],0
  return bytes;                                                       // EAX = byte count
 }
 // Unknown count: read Vectors until "endlist" (strncmp, 7 bytes, case-sensitive) or end of stream.
 char* next=resource_next_0044bfa0();                                 // 004220CD
 while(next&&std::strncmp(skip_blanks_00463510(next),"endlist",7)!=0){ // 004220D4/004220E4..004220FF
  const I size=list->size;                                            // 00422105
  if(size==0){                                                        // 0042210A
   vector_list_allocate_00421310(list,nullptr,1);list->size=1;        // 0042210C..00422115
  }else if(size==count){                                              // 0042211E..00422124
   const U last=list->data+static_cast<U>(count)*12u-12u;             // 00422126..0042212B
   const U x=Load(last);const I capacity=list->capacity;const U y=Load(last+4),z=Load(last+8); // 0042212F..00422139
   if(size==capacity)vector_list_allocate_00421310(list,nullptr,capacity?SignedBits(static_cast<U>(capacity)*2u):8); // 0042213C..0042214E
   const U at=list->data+static_cast<U>(list->size)*12u;              // 00422153..0042215B
   Store(at,x);Store(at+4,y);Store(at+8,z);list->size=list->size+1;  // 0042215E..0042216A
  }
  parse_vector_00463570(next,reinterpret_cast<volatile float*>(list->data+static_cast<U>(count)*12u)); // 00422171..0042217B
  ++count;                                                            // 00422187
  next=resource_next_0044bfa0();                                      // 0042218C
 }
 // 0042219F: skip(skip(line)+7); a null line passes 7 and faults in 00463510, as in the original.
 const U skipped=reinterpret_cast<U>(skip_blanks_00463510(next));
 return reinterpret_cast<U>(skip_blanks_00463510(reinterpret_cast<char*>(skipped+7u)));
}

// 00422210: ECX FontInfo (not null-checked), one stack word, RET4. Each block: EAX=_stricmp result,
// replaced by the reader's EAX on a match; the final value is the raw return.
extern "C" U __fastcall font_info_parse_00422210(FontInfo* self,void*,const char* text){
 char* line=const_cast<char*>(text);
 if(Compare("numchars",line)==0)parse_int_004635e0(line+8,&self->numchars); /* E1 edit LF3 */   // 00422216
 if(Compare("fontheight",line)==0)parse_float_004635c0(line+10,InfoFloat(self,1));     // 0042223D
 if(Compare("baselineheight",line)==0)parse_float_004635c0(line+14,InfoFloat(self,2)); // 00422265
 if(Compare("texturewidth",line)==0)parse_float_004635c0(line+12,InfoFloat(self,3));   // 0042228D
 if(Compare("spacingR",line)==0)parse_float_004635c0(line+8,InfoFloat(self,4));        // 004222B5
 if(Compare("spacingB",line)==0)parse_float_004635c0(line+8,InfoFloat(self,5));        // 004222DD
 if(Compare("upperleftcoords",line)==0)read_vector_list_00421ff0(line+15,InfoList(self,6));  // 00422305
 U eax=Compare("lowerrightcoords",line);                                                // 0042232D
 if(eax==0)eax=read_vector_list_00421ff0(line+16,InfoList(self,9));                      // 00422345
 return eax;
}

// 0041EFA0: vtable slot +38 in both CAurTextureBasic (0073F160) and the 24-byte reference table (0073F0A8).
// ECX this, RET0: target = this->slot08(); basic = target->slot04(); return basic ? basic->+38 : 0.
extern "C" FontInfo* __fastcall texture_font_info_0041efa0(void* self,void*){
 texture::Ref* receiver=static_cast<texture::Ref*>(self);
 texture::Ref* target=texture_vtable::head(receiver)->common.s08(receiver,nullptr); // 0041EFA0/0041EFA2 call [eax+8]
 texture::Ref* basic=texture_vtable::head(target)->s04(target,nullptr);           // 0041EFA5..0041EFA9 call [edx+4]
 return basic?reinterpret_cast<Basic*>(basic)->info:nullptr;           // 0041EFB0 [eax+38h] / 0041EFB4
}
