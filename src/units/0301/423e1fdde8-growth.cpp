#include "af64fb3b38-growth.h"
namespace menu_list_growth {
static std::int32_t signed_bits(Word bits){return bits&0x80000000u?-1-static_cast<std::int32_t>(~bits):static_cast<std::int32_t>(bits);}
void __fastcall allocate_005e03c0(List* self,void*,Word capacity){
 self->capacity=signed_bits(capacity);
 const Word old=self->data;



 void* memory=bindings.allocate_006fa7e6(capacity<<2);

 auto count=self->size;
 self->data=static_cast<Word>(reinterpret_cast<std::uintptr_t>(memory));
 std::int32_t index=0;
 if(count>0)do{
  auto* destination=reinterpret_cast<volatile Word*>(self->data);
  auto* source=reinterpret_cast<const volatile Word*>(old);
  const Word value=source[index];destination[index]=value;

  count=self->size;

  ++index;
 }while(index<count);



 if(old)bindings.free_006fa390(reinterpret_cast<void*>(old));

}
}
