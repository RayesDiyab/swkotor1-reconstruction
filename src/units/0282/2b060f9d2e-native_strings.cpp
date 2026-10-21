#include "0ec67db7c4-native_strings.h"
#include "7deb550b61-allocator_live.h"
char mapped_empty_008328a4[1]={0};
using kotor_reconstruction::StartupString;
extern "C" StartupString* __fastcall WindowCopy005e5b00(StartupString* self,void*,const StartupString* volatile source) {
    const StartupString* saved_source=source;
    const volatile unsigned char* initial=reinterpret_cast<const volatile unsigned char*>(saved_source->data);
    if(initial){const volatile unsigned char* cursor=initial;while(*cursor)++cursor;
        const std::uint32_t length=static_cast<std::uint32_t>(cursor-initial);
        // Preserve the observed overwrite of the incoming stack argument word.
        source=reinterpret_cast<const StartupString*>(length);
        if(length){cursor=initial;while(*cursor)++cursor;const std::uint32_t count=static_cast<std::uint32_t>(cursor-initial)+1;



            self->capacity=count;

            self->data=static_cast<char*>(live_operator_new(count));



            const volatile unsigned char* input=reinterpret_cast<const volatile unsigned char*>(saved_source->data);

            volatile unsigned char* output=reinterpret_cast<volatile unsigned char*>(self->data);unsigned char byte;
            do{byte=*input++;*output++=byte;}while(byte);
            return self;
        }
    }
    self->data=nullptr;self->capacity=0;return self;
}
extern "C" char* __fastcall WindowCStr005e5670(StartupString* self,void*) {
    char* value=self->data;if(value)return value;



    return mapped_empty_008328a4;

}
