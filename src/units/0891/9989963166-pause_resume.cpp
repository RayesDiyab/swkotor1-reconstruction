#include "aefc91cc19-pause_resume.h"
#include "345cf51dbe-lower_ports.h"
#include "fd59383644-list_port.h"
#include <cstring>



namespace {
using U=std::uint32_t;
U word(void* p,unsigned n){U v;std::memcpy(&v,static_cast<unsigned char*>(p)+n,4);return v;}
void* pointer(void* p,unsigned n){void* v;std::memcpy(&v,static_cast<unsigned char*>(p)+n,4);return v;}
unsigned char byte(void* p,unsigned n){unsigned char v;std::memcpy(&v,static_cast<unsigned char*>(p)+n,1);return v;}
bool exempt(void* internal,void* payload,unsigned offset) {
    using namespace sound_mode29_required;
    const U mode=word(internal,0x118+word(internal,0x140)*4);
    if(mode==*word_0074c5f4) {
        const auto kind=byte(payload,offset);



        return kind==1 || kind==2 || kind==4 || kind==11;

    }
    if(mode==*word_0074c5fc) {
        const auto kind=byte(payload,offset);
        return kind==1 || kind==2 || kind==11;
    }
    return false;
}
}
namespace sound_mode29 {
std::uint32_t __fastcall PauseAll005d82c0(void* p,void*,U all) {
    using namespace sound_pause29_required;

    *word_00832890=1;

    void* list=pointer(p,0x44);void* cursor=pointer(list,0);void* payload=cursor?pointer(cursor,8):nullptr;
    while(cursor) {
        if(all!=0 || !exempt(p,payload,8))stream_pause_005dc3c0(payload,nullptr);

        list=pointer(p,0x44);

        payload=reinterpret_cast<void*>(unresolved_list_next_005e9b90(list,nullptr,&cursor));
    }
    list=pointer(p,0x40);cursor=pointer(list,0);payload=cursor?pointer(cursor,8):nullptr;
    while(cursor) {
        const U veto=zero_0063e7f0();



        if(veto==0 && !exempt(p,payload,0x35))actor_pause_005db150(payload,nullptr);


        list=pointer(p,0x40);

        payload=reinterpret_cast<void*>(unresolved_list_next_005e9b90(list,nullptr,&cursor));
    }



    return 1;
}
std::uint32_t __fastcall ResumeAll005d83f0(void* p,void*) {
    using namespace sound_pause29_required;

    *word_00832890=0;

    void* list=pointer(p,0x44);void* cursor=pointer(list,0);void* payload=cursor?pointer(cursor,8):nullptr;
    while(cursor) {
        stream_resume_005dcdb0(payload,nullptr);

        list=pointer(p,0x44);

        payload=reinterpret_cast<void*>(unresolved_list_next_005e9b90(list,nullptr,&cursor));
    }
    list=pointer(p,0x40);cursor=pointer(list,0);payload=cursor?pointer(cursor,8):nullptr;
    while(cursor) {
        actor_resume_005db1c0(payload,nullptr);

        list=pointer(p,0x40);

        payload=reinterpret_cast<void*>(unresolved_list_next_005e9b90(list,nullptr,&cursor));
    }



    return 1;
}
}

