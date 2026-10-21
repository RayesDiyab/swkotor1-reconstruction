#include "4064827f19-door_contracts.h"
#include "1c0610ca13-door_state_extension.h"
#include "4bfb9d8c74-door_area_extension.h"
extern "C" {
int __cdecl kotor_stricmp_0070acaf(const char*,const char*);
void* __fastcall kotor_m15a_object_get_server_object_0063d4b0(void*,void*);
void* __fastcall recovered_object_area_004cb120(void*,void*);
}
namespace {
struct ServerDoorView {
    const void* earlier[6];
    void*(__fastcall* door)(void*,void*);
};
static_assert(offsetof(ServerDoorView,door)==0x18,"Original server Door cast slot");
}
extern "C" void __cdecl recovered_door_move_next_door_state_00683f00(void*,const char* event,void* self){
    // Same admitted C-locale UCRT _strnicmp backend as 0063D340. Original
    // 0070AD18 selects its ASCII path under this process locale contract.
    if(kotor_stricmp_0070acaf(event,"door")!=0){
        if(_strnicmp(event,"done",4)!=0)return;
        event+=4;
        if(_strnicmp(event,"opening",7)!=0&&_strnicmp(event,"closing",7)!=0)return;
    }
    if(kotor_m15a_object_get_server_object_0063d4b0(self,nullptr)){
        void* server=kotor_m15a_object_get_server_object_0063d4b0(self,nullptr);
        void* server_door=door::get<const ServerDoorView*>(server,0)->door(server,nullptr);
        if(server_door)w3_server_door_advance_00589680(server_door,nullptr);
    }
}
extern "C" void __fastcall w3_server_door_advance_00589680(void* self,void*){
    using namespace door;
    const B previous=get<B>(self,0x2cc),next=get<B>(self,0x2cd);
    put<B>(self,0x2cc,next);
    if(previous==next||next==3)return;
    if(void* area=recovered_object_area_004cb120(self,nullptr)){
        const I closed=get<B>(self,0x2cc)==0?1:0;
        const U id=get<U>(self,4);
        w3_area_door_position_update_00508180(area,nullptr,vector(self,0x90),
            vector(self,0x338),vector(self,0x344),id,closed);
    }
}
