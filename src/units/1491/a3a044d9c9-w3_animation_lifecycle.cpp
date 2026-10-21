// New bodies only for the assigned original W3 animation lifecycle entries.
#include "0b7d27aef1-w3_animation.h"
#include "62420f93e8-allocator_live.h"
#include "9492a26633-w3_client_door_placeable.h"
#include "a1bf7fc0cb-w3_client_t3.h"
#include <cstring>

extern "C" {
std::uint32_t __fastcall kotor_m15e_anim_base_unload_model_0069d740(void*,void*) noexcept(false);
void __fastcall kotor_m15e_anim_base_destruct_0069e140(void*,void*) noexcept(false);
}
namespace {
using U=std::uint32_t;
using B=unsigned char;
template<class T> T load(const void* p,U offset){T value;std::memcpy(&value,static_cast<const B*>(p)+offset,sizeof value);return value;}
template<class T> void store(void* p,U offset,T value){std::memcpy(static_cast<B*>(p)+offset,&value,sizeof value);}

// The three originals arm unwind state0 before UnloadModel. Their EH cleanup
// calls the real base destructor if unloading throws; normal exit calls it once.
struct BaseDestruction {
    void* self;
    ~BaseDestruction() noexcept(false){kotor_m15e_anim_base_destruct_0069e140(self,nullptr);}
};
void destruct(void* self,const void* table){
    store(self,0,table);
    BaseDestruction cleanup{self};
    kotor_m15e_anim_base_unload_model_0069d740(self,nullptr);
}
using ScalarDelete=void*(__fastcall*)(void*,void*,U);
}
extern "C" {
void __fastcall w3_anim_door_destruct_006e56c0(void* self,void*){
    destruct(self,recovered_anim_base_door_methods_00758a08);
}
void __fastcall w3_anim_placeable_destruct_006e4e70(void* self,void*){
    destruct(self,recovered_anim_base_placeable_methods_007588b0);
}
void __fastcall w3_anim_trigger_destruct_006e5f60(void* self,void*){
    destruct(self,recovered_anim_base_trigger_methods_00758c70);
}
void* __fastcall w3_anim_door_scalar_006e59a0(void* self,void*,U flags){
    w3_anim_door_destruct_006e56c0(self,nullptr);
    if((flags&1u)!=0)live_free(self);
    return self;
}
void* __fastcall w3_anim_placeable_scalar_006e5180(void* self,void*,U flags){
    w3_anim_placeable_destruct_006e4e70(self,nullptr);
    if((flags&1u)!=0)live_free(self);
    return self;
}
void* __fastcall w3_anim_trigger_scalar_006e6020(void* self,void*,U flags){
    w3_anim_trigger_destruct_006e5f60(self,nullptr);
    if((flags&1u)!=0)live_free(self);
    return self;
}
void* __fastcall w3_area_position_0062ec20(void* self,void*,void* output){
    const void* const server=load<void*>(self,0x94);
    if(server==nullptr){
        store(output,0,0u);store(output,4,0u);store(output,8,0u);
    }else{
        // Original copies three DWORDs in order. Keep its overlap behavior.
        store(output,0,load<U>(server,0x424));
        store(output,4,load<U>(server,0x428));
        store(output,8,load<U>(server,0x42c));
    }
    return output;
}
void __cdecl w3_bullet_die_00670b20(void* bullet){
    if(bullet!=nullptr){
        const void* const table=load<const void*>(bullet,0);
        const ScalarDelete destroy=load<ScalarDelete>(table,0);
        destroy(bullet,nullptr,1);
    }
}
}
