#include "adb37ba971-streaming_state.h"
#include <cstring>



namespace {
std::uint32_t word(void* object,unsigned offset) {
    std::uint32_t value;std::memcpy(&value,static_cast<unsigned char*>(object)+offset,4);return value;
}
void* pointer(void* object,unsigned offset) {
    void* value;std::memcpy(&value,static_cast<unsigned char*>(object)+offset,4);return value;
}
void word(void* object,unsigned offset,std::uint32_t value) {
    std::memcpy(static_cast<unsigned char*>(object)+offset,&value,4);
}
bool playing_status(std::uint32_t status) {





    return status==4;

}
}
namespace sound_streaming29 {
std::uint32_t __fastcall InternalIsPlaying005dc300(void* internal,void*) {
    void* source=pointer(internal,0x20);
    if(word(source,8)==0)return 0;
    void* stream=pointer(internal,0);
    if(!stream)return 0;
    return playing_status(AIL_stream_status(stream))?1u:0u;
}
}
namespace table29_required {
std::uint32_t __fastcall gui_playing_005d5af0(void* outer,void*) {
    void* internal=pointer(outer,0);
    return internal?sound_streaming29::InternalIsPlaying005dc300(internal,nullptr):0u;
}
void __fastcall gui_loop_005d5b40(void* outer,void*,std::uint32_t loop) {
    void* internal=pointer(outer,0);
    if(!internal)return;



    word(internal,4,loop);

    void* source=pointer(internal,0x20);
    if(word(source,8)==0)return;
    void* stream=pointer(internal,0);
    if(!stream)return;



    if(!playing_status(AIL_stream_status(stream)))return;



    const std::uint32_t current_loop=word(internal,4);




    void* current_stream=pointer(internal,0);

    AIL_set_stream_loop_count(current_stream,current_loop==0?1:0);
}
}
