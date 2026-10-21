#include "7bb1cb5c27-aa_query.h"
#include <cstring>
namespace kotor_renderer_init {
static I32 SignedWord(const U16* word) {
    std::int16_t value;std::memcpy(&value,word,2);return value;
}
I32 __cdecl QueryAntiAliasingMode_0044f140(I32 requested) {
    if(requested<0)return 0;
    if(requested==0)return 1;
    if(AntiAliasingAvailable_0046ed70()==0)return 0;
    void* dc=GetDC_0073d408(*aa_query.window_007a39d8);
    const I32 attributes[20]={0x2001,1,0x2003,0x2027,0x2014,
        SignedWord(aa_query.color_007b9220),0x201b,8,0x2022,
        SignedWord(aa_query.depth_007b9224),0x2023,8,0x2011,1,
        0x2041,1,0x2042,requested,0,0};
    const float floats[2]={0.0f,0.0f};I32 format;U32 count;
    if((*aa_query.choose_007bb730)(dc,attributes,floats,1,&format,&count)!=0 && count!=0) {
        const I32 attribute=0x2042;I32 actual;
        // Original ignores this provider result and reads its output word.
        (*aa_query.attributes_007bb63c)(dc,format,0,1,&attribute,&actual);
        ReleaseDC_0073d404(*aa_query.window_007a39d8,dc);
        return actual==requested?1:0;
    }
    ReleaseDC_0073d404(*aa_query.window_007a39d8,dc);
    return 0;
}
}
