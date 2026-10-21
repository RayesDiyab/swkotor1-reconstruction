#include "496dab8016-async_load.h"
#include "10145c43b0-ref_string_joins.h"
#include "e2a1c02a6b-lowercase.h"
#include <cstring>
using namespace async_load;

// Original004061F0: ECXreceiver, hidden16-byte output thenString onstack;
// full output pointer inEAX, RET8. Only disjoint live String backing storage
// is accepted; clearing the receiver precedes the CStr call/copy.
extern "C" __declspec(noinline) Ref* __fastcall
async_ref_assign_string_004061f0(Ref* self,void*,Ref* output,String* source) {
    const Word length=async_string_length_005e5790(source,nullptr);
    std::memset(self->bytes,0,sizeof(self->bytes));
    if(length) {
        const char* text=async_string_cstr_005e5670(source,nullptr);



        const Word count=length>16u?16u:length;

        std::memcpy(self->bytes,text,count);
        for(Word i=0;i<count;++i) {



            const int input=self->bytes[i];

            self->bytes[i]=static_cast<unsigned char>(kotor_lower_006fb766(input));
        }
    }
    // Each original DWORD read precedes its corresponding output write.
    // Do not replace these with an unconditional pre-read snapshot.
    for(unsigned offset=0;offset<16;offset+=4) {
        Word word;std::memcpy(&word,self->bytes+offset,4);
        std::memcpy(output->bytes+offset,&word,4);
    }
    return output;
}

// Original00406D60 consumes a private16-byte output and returnsreceiver.
extern "C" __declspec(noinline) Ref* __fastcall
async_ref_construct_string_00406d60(Ref* self,void*,String* source) {
    Ref discarded;
    (void)async_ref_assign_string_004061f0(self,nullptr,&discarded,source);
    return self;
}
