// 0041F4D0 AurTextureCalculateHashID, 00421560 CAurTextureBasic::AddToOrderedLists,
// 00445020 CExoArrayList<ptr>::Insert (lane-font packet 5).
// Evidence: lane-font/disasm/0041f4d0.asm.txt, 00421560.asm.txt; 00445020..00445078 from
// lane-gui-draw/exe-text.asm. List cells are the reused texture_resource::bindings views
// (scan=List007A4798, d3_on=List007A4780, d3_off=List007A47A4; MINIMAL-ADOPTION.md).
#include "43e0af9c1e-font_txi.hpp"
namespace font_txi {
namespace {
I SignedBits(U v){return v&0x80000000u?-1-static_cast<I>(~v):static_cast<I>(v);}
U Load(U address){return *reinterpret_cast<volatile U*>(address);}
void Store(U address,U value){*reinterpret_cast<volatile U*>(address)=value;}
Basic* Element(U data,I index){return reinterpret_cast<Basic*>(Load(data+static_cast<U>(index)*4u));}
// cdq; idiv 0FE4Dh: quotient -> EAX, remainder -> new hash (0041F514..0041F51F, 0041F54A..0041F555).
void Reduce(U dividend,U& eax,U& hash){I a=SignedBits(dividend);eax=static_cast<U>(a/0xfe4d);hash=static_cast<U>(a%0xfe4d);}
// 004215C8..00421607 / 00421609..00421648: append with 005E03C0 growth; returns new count.
U Append(List* list,Basic* self){
 I capacity=list->capacity;                                   // mov eax,[cap]
 if(list->size==capacity)menu_list_growth::allocate_005e03c0(list,nullptr,capacity?static_cast<U>(capacity)+static_cast<U>(capacity):8u);
 U data=list->data;I size=list->size;                          // reload after growth
 Store(data+static_cast<U>(size)*4u,reinterpret_cast<U>(self));
 U count=static_cast<U>(list->size)+1u;list->size=SignedBits(count);
 return count;                                                 // EAX = new count
}
}
}
using namespace font_txi;
U __cdecl texture_hash_0041f4d0(const char* text,I count,const U* vector){
 U hash=0;                                                     // esi (0041F4DA)
 U length=0;while(text[length])++length;                       // 0041F4DC..0041F4E7
 U eax=length;I limit=SignedBits(length);
 for(I i=0;i<limit;++i){                                       // 0041F4EF jle / 0041F521 jl
  U lower=static_cast<U>(kotor_lower_006fb766(static_cast<std::int32_t>(static_cast<signed char>(text[i])))); // movsx + call 006FB766
  Reduce(hash?lower*hash:lower,eax,hash);                      // esi==0 -> no multiply (0041F4F1)
 }
 std::int16_t words=static_cast<std::int16_t>(static_cast<U>(count)); // mov ax,word [esp+18h]
 eax=(eax&0xffff0000u)|static_cast<std::uint16_t>(words);
 for(I j=0;j<words;++j){                                       // 0041F528 jle / 0041F557 jl
  U value=vector[j];                                           // mov eax,[ebx+ecx*4]
  Reduce(hash?value*hash:value,eax,hash);
 }
 return (eax&0xffff0000u)|(hash&0xffffu);                      // 0041F55A mov ax,si
}
U __fastcall list_insert_00445020(List* list,void*,U value,I index){
 I capacity=list->capacity;I old=list->size;                   // 00445023 cap, 00445027 size (edi)
 if(old==capacity)menu_list_growth::allocate_005e03c0(list,nullptr,capacity?static_cast<U>(capacity)+static_cast<U>(capacity):8u); // 0044503E
 I size=list->size;U data0=list->data;                         // 00445043 size, 00445046 data
 Store(data0+static_cast<U>(size)*4u,value);                   // 0044504C
 list->size=SignedBits(static_cast<U>(list->size)+1u);         // 0044504F inc [esi+4]
 if(old>index){                                                // 00445056 jle (signed)
  I j=old;
  do {U data=list->data;Store(data+static_cast<U>(j)*4u,Load(data+static_cast<U>(j)*4u-4u));--j;}while(j>index); // 00445060..0044506E
 }
 U data=list->data;Store(data+static_cast<U>(index)*4u,value); // 00445071..00445074
 return data;                                                  // EAX = data
}
U __fastcall texture_add_ordered_00421560(Basic* self,void*){
 List* scan=texture_resource::bindings.scan;
 I size=scan->size;                                            // 00421561 esi=[007A479C] (read once)
 I index=0;
 if(size>0){                                                   // 0042156E
  std::uint16_t hash=self->hash;U data=scan->data;            // 00421570 cx, 00421577 edx (read once)
  for(;;){
   if(hash<=Element(data,index)->hash){                       // 00421583 jbe (unsigned)
    while(index<size&&Element(data,index)->hash==hash)++index; // 00421593..004215AF
    break;
   }
   if(++index>=size)break;                                     // 0042158C..00421591
  }
 }
 list_insert_00445020(scan,nullptr,reinterpret_cast<U>(self),index); // 004215B2..004215B9
 return Append(self->d3?texture_resource::bindings.d3_on:texture_resource::bindings.d3_off,self); // 004215BE test [+D3]
}
