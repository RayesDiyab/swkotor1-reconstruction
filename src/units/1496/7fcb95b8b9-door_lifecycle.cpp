#include "2d1a6b5349-door_contracts.h"
namespace {
struct BaseCleanup {void* self;~BaseCleanup()noexcept(false){kotor_m15e_object_destruct_0063e800(self,nullptr);}};
struct StringCleanup {void* self;~StringCleanup()noexcept(false){c2da_current_string_destroy_005e5c20(static_cast<kotor_reconstruction::StartupString*>(self),nullptr);}};
}
extern "C" {
void __fastcall w3_door_destroy_00683f90(void* self,void*){
    using namespace door;
    put<const void*>(self,0,recovered_client_door_methods_00753960);
    BaseCleanup base{self};StringCleanup name{at(self,0xfc)},transition{at(self,0x130)},description{at(self,0x13c)};
    if(void* model=ptr(self,0x68)){
        (void)animation_table(model)->s078(model,nullptr);
        if(void* current=ptr(self,0x68))deleting(current,1);
        put<void*>(self,0x68,nullptr);
    }
}
void* __fastcall recovered_client_door_destructor_006843f0(void* self,void*,std::uint32_t flags){
    w3_door_destroy_00683f90(self,nullptr);
    if(flags&1u)live_free(self);
    return self;
}
void* __fastcall recovered_client_door_get_name_006823c0(void* self,void*,void* output){
    (void)r8_copy_construct_005e5b00(static_cast<kotor_reconstruction::StartupString*>(output),nullptr,
        static_cast<const kotor_reconstruction::StartupString*>(door::at(self,0xfc)));
    return output;
}
void __fastcall recovered_client_door_ai_update_00684cd0(void* self,void*){
    using namespace door;
    U day,tick;
    frame_GetWorldTime_004ade40(frame_GetWorldTimer_005ed4f0(client(),nullptr),nullptr,&day,&tick);
    // Original pushes previous words before invoking the second timer getter.
    const U previous_tick=get<U>(self,0x40),previous_day=get<U>(self,0x3c);
    U elapsed_day,elapsed_tick;
    (void)recovered_world_time_difference_004ae460(frame_GetWorldTimer_005ed4f0(client(),nullptr),nullptr,
        day,tick,previous_day,previous_tick,&elapsed_day,&elapsed_tick);
    const U countdown=get<U>(self,0xf0);
    put<U>(self,0x44,elapsed_tick);
    if(countdown!=0)put<U>(self,0xf0,countdown>elapsed_tick?countdown-elapsed_tick:0u);
    object_table(self)->s0e8_update_pop_up_bubble(self,nullptr);
    w3_door_animation_00684030(self,nullptr);
    object_table(self)->s0c4_update_visual_effects_list(self,nullptr);
    kotor_fastcall_004015a0(self,nullptr);
    w3_door_interaction_00684660(self,nullptr);
    if(kotor_fastcall_0063df60(self,nullptr)){
        kotor_m15e_client_ai_list_remove_object_00604810(at(kotor_m15e_client_get_client_ai_master_005ed6b0(client(),nullptr),4),nullptr,self);
        const U id=get<U>(self,4);
        void* removed;
        if(recovered_object_array_remove_004d81a0(kotor_fastcall_005ed570(client(),nullptr),nullptr,id,&removed)==0&&removed){
            deleting(self,1);
            return;
        }
    }
    put<U>(self,0x3c,day);put<U>(self,0x40,tick);
}
}
