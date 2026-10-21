#include "0f48c8b46c-check_extension.h"
namespace kotor_renderer_init {
U8 __cdecl CheckExtension_0045f980(const char* wanted,const char* text) {
    U32 wanted_length=0;while(wanted[wanted_length]!=0)++wanted_length;
    U32 text_length=0;while(text[text_length]!=0)++text_length;
    U32 position=0;
    while(position<text_length) {
        const U32 token_size=StringCSpan_006fcb40(text+position,token_delimiter_0073d5d8);
        if(wanted_length==token_size && StringNCompare_006faff0(wanted,text+position,token_size)==0) return 1;
        position+=token_size+1;
    }
    return 0;
}
}
