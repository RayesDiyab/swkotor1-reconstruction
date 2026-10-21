#include "aaf9b2970f-texture_join.h"
namespace texture_join {
Ref* __fastcall referenced_004205c0(Ref* self,void*) {
    Ref* const result=self->reference.target;return result?result:self;
}
__declspec(naked) Ref* __fastcall call_slot08(Ref*,void*) {
    __asm { mov eax,[ecx] }
    __asm { jmp dword ptr [eax+8] }
}
static std::int32_t signed_bits(Word x){return x&0x80000000u?-1-static_cast<std::int32_t>(~x):static_cast<std::int32_t>(x);}
void __fastcall add_unique_0046bd10(List* self,void*,Ref* value) {
    const std::int32_t size=self->size;
    Word matches=0;
    if(size>0) {
        const Word data=self->data;
        for(std::int32_t i=0;i<size;++i)
            if(*reinterpret_cast<const volatile Word*>(data+static_cast<Word>(i)*4u)==reinterpret_cast<Word>(value))++matches;
        if(matches)return;
    }
    const Word capacity=static_cast<Word>(self->capacity);
    if(static_cast<Word>(size)==capacity)
        menu_list_growth::allocate_005e03c0(self,nullptr,capacity?capacity*2u:8u);
    const Word count=static_cast<Word>(self->size),data=self->data;
    *reinterpret_cast<volatile Word*>(data+count*4u)=reinterpret_cast<Word>(value);
    self->size=signed_bits(static_cast<Word>(self->size)+1u);
}
__declspec(naked) void __fastcall set_environment_00421480(Ref*,void*,Word) {
    __asm {
        mov eax,[ecx]
        call dword ptr [eax+8]
        mov edx,[eax]
        mov ecx,eax
        call dword ptr [edx+4]
        test eax,eax
        je done
        mov cl,byte ptr [esp+4]
        test cl,cl
        mov byte ptr [eax+0dah],cl
        je done
        mov dword ptr [esp+4],eax
        mov ecx,canonical_environment_007a4770
        jmp add_unique_0046bd10
    done:
        ret 4
    }
}
}
