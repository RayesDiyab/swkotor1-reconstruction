#include "3432d647fb-resource.hpp"
#include <string.h>



namespace texture_resource {
// E1 edit F7: `bindings` is defined (bound) in texture_cells.cpp.
static I signed_bits(U v){return v&0x80000000U?-1-static_cast<I>(~v):static_cast<I>(v);}
static U sar(U v,unsigned n){return (v>>n)|((v&0x80000000U)?(~0U<<(32-n)):0U);}
static U divide4_round_zero(U v){U bias=(v&0x80000000U)?3U:0U;return sar(v+bias,2);}
__declspec(naked) U __cdecl free_raw(void*){__asm {jmp live_free}}
// Current MSVC x86 single-inheritance member-pointer representation is checked.
void* __cdecl remove_method_address(){auto method=&List::RemoveReverse32;static_assert(sizeof(method)==sizeof(void*),"x86 direct member address required");void* result;memcpy(&result,&method,sizeof(result));return result;}
__declspec(naked) U __cdecl remove_raw(void*,List*,U){
 __asm {mov eax,[esp+4]}
 __asm {mov ecx,[esp+8]}
 __asm {push dword ptr [esp+12]}
 __asm {call eax}
 __asm {ret}
}
// Raw cdecl8 transport; no callback implementation is supplied by this packet.
__declspec(naked) U __cdecl image_call(acquisition::ReleaseRaster,U,U,U,U,U,U,U,U){ // E1 edit F5: typed target, same cdecl9 transport
 __asm {mov edx,esp}
 __asm {push dword ptr [edx+36]}
 __asm {push dword ptr [edx+32]}
 __asm {push dword ptr [edx+28]}
 __asm {push dword ptr [edx+24]}
 __asm {push dword ptr [edx+20]}
 __asm {push dword ptr [edx+16]}
 __asm {push dword ptr [edx+12]}
 __asm {push dword ptr [edx+8]}
 __asm {call dword ptr [edx+4]}
 __asm {add esp,32}
 __asm {ret}
}
U __cdecl image_amount(Basic* self,U format){
 // Other formats have an uninitialized original stack word on a positive mip walk.
 // Contract excludes that case; these bits are used only when format3/4 was established.
 U block=0;if(format==3)block=8;else if(format==4)block=16; // D2 DEVIATION (E-D5): original [esp+8] is uninitialised for other formats (writers only 0041F594/0041F5A3; read 0041F611)
 I height=texture::field_load(self->f6c);I width=texture::field_load(self->f68); // E1: Signed fields, ordered loads
 I ratio=height/width;U faces=1;
 if(ratio==6){faces=6;height/=6;}
 U amount=static_cast<U>(texture::field_load(self->f5c));
 if(amount){
  I initialCount=static_cast<I>(self->ce)-1;
  if(initialCount>0){U count=static_cast<U>(static_cast<I>(self->ce))-1U;do{
   height=signed_bits(sar(static_cast<U>(height),1));width=signed_bits(sar(static_cast<U>(width),1));
   U rows=divide4_round_zero(static_cast<U>(height)+3U);U cols=divide4_round_zero(static_cast<U>(width)+3U);
   amount+=rows*cols*block;--count;
  }while(count!=0);}
  if(faces==6)amount*=6U;
 }
 return amount;
}



U __fastcall free_image_0041f570(Basic* self,void*){
 U initial=self->f44;if(!initial)return 0;
 U format=static_cast<U>(texture::field_load(self->f70));U amount=image_amount(self,format);

 self->flags[14]=0;

 U result;
 if(self->flags[11]){
  U f64=static_cast<U>(texture::field_load(self->f64)),f60=static_cast<U>(texture::field_load(self->f60));U flag=self->flags[13];U liveFormat=static_cast<U>(texture::field_load(self->f70));
  U liveHeight=static_cast<U>(texture::field_load(self->f6c)),liveWidth=static_cast<U>(texture::field_load(self->f68)),liveImage=self->f44;
  result=image_call(*bindings.image_callback,liveImage,liveWidth,liveHeight,liveFormat,amount,f60,f64,flag);
  self->flags[11]=0;
 }else{
  result=free_raw(reinterpret_cast<void*>(self->f44));
 }

 self->f44=0;

 return result;
}

U __fastcall remove_ordered_00421650(Basic* self,void*){
 I size=bindings.scan->size;U matches=0;
 if(size>0){U data=bindings.scan->data;for(I i=0;i<size;++i)if(*reinterpret_cast<volatile U*>(data+static_cast<U>(i)*4U)==reinterpret_cast<U>(self))++matches;}
 void* method=remove_method_address();
 if(matches){remove_raw(method,bindings.scan,reinterpret_cast<U>(self));



 }
 List* selected=self->d3?bindings.d3_on:bindings.d3_off;



 return remove_raw(method,selected,reinterpret_cast<U>(self));
}
}
