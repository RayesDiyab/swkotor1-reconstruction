#include "91ebd12484-providers.h"
namespace gui_viewport_list {
static int Signed(Word w){return w&0x80000000u?-1-static_cast<int>(~w):static_cast<int>(w);}
void __fastcall add_00444fe0(List* self,void*,int value){
 const Word capacity=static_cast<Word>(self->capacity);
 if(static_cast<Word>(self->size)==capacity)
  menu_list_growth::allocate_005e03c0(self,nullptr,capacity?capacity+capacity:8u);
 const Word index=static_cast<Word>(self->size);
 const Word address=self->data+(index<<2);
 *reinterpret_cast<volatile Word*>(address)=static_cast<Word>(value);
 // INC reads live size after the element write, including an aliased size word.
 self->size=Signed(static_cast<Word>(self->size)+1u);
}
}
