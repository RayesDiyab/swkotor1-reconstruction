#include "905c21d6b5-owner.hpp"
#include "8945d3b71e-texture_vtable.hpp" // E1: typed vtable dispatch (E-D2)



namespace texture_owner {
void** ref_table=nullptr;
static I signed_bits(U v){return v&0x80000000U?-1-static_cast<I>(~v):static_cast<I>(v);}
// ABI transport only: direct assembly call avoids invoking incompatible C++ pointer types.
__declspec(naked) U __cdecl dispatch_delete(void*,Ref*,U){
 __asm {mov eax,[esp+4]}
 __asm {mov ecx,[esp+8]}
 __asm {push dword ptr [esp+12]}
 __asm {call eax}
 __asm {ret}
}
// live_free has a void C++ contract; this transport preserves its machine EAX.
__declspec(naked) static U __cdecl free_raw(void*){__asm {jmp live_free}}
U __fastcall list_destroy_00422d60(List* self,void*){
 I index=signed_bits(static_cast<U>(self->size)-1U);
 while(index>=0){
  U data=self->data;
  Safe* slot=reinterpret_cast<Safe*>(*reinterpret_cast<volatile U*>(data+static_cast<U>(index)*4U));
  Ref* target=slot->target;
  if(target)target->registrations.RemoveReverse32(reinterpret_cast<U>(slot));
  index=signed_bits(static_cast<U>(index)-1U);

  slot->target=nullptr;

 }
 U saved=self->data;
 if(saved){live_free(reinterpret_cast<void*>(saved));self->data=0;}
 U again=self->data;
 self->capacity=0;self->size=0;
 return again?free_raw(reinterpret_cast<void*>(again)):0;
}
U __fastcall base_destroy_00422e90(Ref* self,void*){
 self->vtable=ref_table;
 Ref* old=self->reference.target;
 if(old)old->registrations.RemoveReverse32(reinterpret_cast<U>(&self->reference));
 self->reference.target=nullptr;
 if(old&&static_cast<U>(old->registrations.size)==0){
  Ref* first=texture_vtable::head(old)->s04(old,nullptr); // E1 edit O2: typed slot +04
  if(first){



   Ref* second=texture_vtable::head(old)->s04(old,nullptr); // E1 edit O2

   if(second)(void)texture_vtable::head(second)->s00(second,nullptr,1); // E1 edit O3: typed slot +00
  }else (void)texture_vtable::head(old)->s00(old,nullptr,1); // E1 edit O3
 }
 Ref* rebound=self->reference.target;

 if(rebound)rebound->registrations.RemoveReverse32(reinterpret_cast<U>(&self->reference));



 return list_destroy_00422d60(&self->registrations,nullptr);
}






void* __fastcall scalar_delete_004233c0(void* object,void*,U flags){ // E1 edit O1
 Basic* self=static_cast<Basic*>(object);
 texture_destruction::destroy_00422f40(self,nullptr);
 self->~Basic();



 if(flags&1U)live_free(self);

 return self;
}

Ref* __fastcall basic_query_00641db0(Ref* self,void*){return self;}
}
