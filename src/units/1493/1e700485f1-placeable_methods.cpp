#include "5b8ffa03d2-placeable_internal.h"
using namespace placeable_private;
namespace menu_continuation {struct WorldTimer;}
extern "C" {
menu_continuation::WorldTimer* __fastcall frame_GetWorldTimer_005ed4f0(void*,void*);
void __fastcall frame_GetWorldTime_004ade40(menu_continuation::WorldTimer*,void*,U*,U*);
U __fastcall recovered_world_time_difference_004ae460(void*,void*,U,U,U,U,U*,U*);
void* __fastcall kotor_m15e_client_get_client_ai_master_005ed6b0(void*,void*);
void __fastcall kotor_m15e_client_ai_list_remove_object_00604810(void*,void*,void*);
void* __fastcall kotor_fastcall_005ed570(void*,void*);
B __fastcall recovered_object_array_remove_004d81a0(void*,void*,U,void**);
U __fastcall kotor_fastcall_0063df60(void*,void*);
void __fastcall kotor_m15e_slot090_void0_004015a0(void*,void*);
}
namespace {
void* table(U offset){return ptr(ptr(kotor_cell_rules_007a3a28,0xB8),offset);}
struct ExistingCleanup {
 void* value;Void destroy;
 ~ExistingCleanup() noexcept(false){if(value)destroy(value,nullptr);}
 void finish(){void* const ending=value;value=nullptr;destroy(ending,nullptr);}
};
}
extern "C" void __fastcall recovered_client_placeable_unload_light_00682be0(void* self,void*){
 if(kotor_fastcall_005ed530(client(),nullptr)){
  void* const area=ptr(kotor_fastcall_005ed530(client(),nullptr),0x48);
  if(area)recovered_client_area_remove_placeable_object_light_00605b50(area,nullptr,rd<U>(self,4),0);
 }
 void* light=ptr(self,0x14C);
 if(light){
  slot<Three>(light,0x50)(light,nullptr,0,0,0);
  light=ptr(self,0x14C);slot<One>(light,0x4C)(light,nullptr,0);
  light=ptr(self,0x14C);if(light)slot<Delete>(light,0)(light,nullptr,1);
  wr<void*>(self,0x14C,nullptr);
 }
}
extern "C" void __fastcall w3_placeable_destroy_00683200(void* self,void*){
 wr<const void*>(self,0,recovered_client_placeable_methods_007537d0);
 ExistingCleanup base{self,&kotor_m15e_object_destruct_0063e800};
 ExistingCleanup first{at(self,0xFC),&frame_LogStringDtor_005e5c20};
 ExistingCleanup second{at(self,0x104),&frame_LogStringDtor_005e5c20};
 ExistingCleanup third{at(self,0x144),&frame_LogStringDtor_005e5c20};
 recovered_client_placeable_unload_light_00682be0(self,nullptr);
 void* model=ptr(self,0x68);
 if(model){
  slot<Void>(model,0x78)(model,nullptr);
  model=ptr(self,0x68);if(model)slot<Delete>(model,0)(model,nullptr,1);
  wr<void*>(self,0x68,nullptr);
 }
 third.finish();second.finish();first.finish();base.finish();
}
extern "C" void* __fastcall recovered_client_placeable_destructor_006836a0(void* self,void*,U flags){
 w3_placeable_destroy_00683200(self,nullptr);if(flags&1)live_free(self);return self;
}
extern "C" U __fastcall recovered_client_placeable_is_hostile_006831b0(void* self,void*){
 if(kotor_m15a_object_get_server_object_0063d4b0(self,nullptr)){
  void* owner=kotor_m15a_object_get_server_object_0063d4b0(self,nullptr);
  if(slot<Ptr>(owner,0x48)(owner,nullptr)){
   void* server=nullptr;
   if(kotor_m15a_object_get_server_object_0063d4b0(self,nullptr)){
    owner=kotor_m15a_object_get_server_object_0063d4b0(self,nullptr);
    server=slot<Ptr>(owner,0x48)(owner,nullptr);
   }
   return rd<U>(server,0x340);
  }
 }
 return 0;
}
extern "C" U __fastcall recovered_client_placeable_apply_shadow_blob_00682c60(void* self,void*){
 String shadow;
 if(!ptr(self,0x68))return 0;
 I size;I found;
 {String column("ShadowSize");found=c2da_current_get_int_00413660(table(0x2C),nullptr,rd<B>(self,0x10C),&column,&size);}
 if(!found)return 1;
 GuiAssign005e5140(&shadow,nullptr,size==0?"Fx_Shadow_S":size==2?"Fx_Shadow_L":"Fx_Shadow_M");
 void* model=ptr(self,0x68);
 if(rd<U>(self,0xD4))slot<Void>(model,0xBC)(model,nullptr);
 model=ptr(self,0x68);
 using Shadow=U(__fastcall*)(void*,void*,const String*);
 const U result=slot<Shadow>(model,0xA4)(model,nullptr,&shadow);
 if(!rd<U>(self,0xD4))slot<Void>(self,0x108)(self,nullptr);
 return result;
}
extern "C" float __fastcall recovered_client_placeable_get_can_see_height_00682f20(void* self,void*){
 void* const placeables=table(0x2C);float height=1.5f;
 if(kotor_m15a_object_get_server_object_0063d4b0(self,nullptr)){
  void* const owner=kotor_m15a_object_get_server_object_0063d4b0(self,nullptr);
  void* const server=slot<Ptr>(owner,0x48)(owner,nullptr);
  if(server){
   I found;
   {String column("CanSeeHeight");found=c2da_current_get_float_00413430(placeables,nullptr,rd<B>(server,0x230),&column,&height);}
   if(!found)return 1.5f;
  }
 }
 return height;
}
extern "C" void __fastcall recovered_client_placeable_ai_update_006836c0(void* self,void*){
 U day,tick,elapsed_day,elapsed_tick;
 frame_GetWorldTime_004ade40(frame_GetWorldTimer_005ed4f0(client(),nullptr),nullptr,&day,&tick);
 recovered_world_time_difference_004ae460(frame_GetWorldTimer_005ed4f0(client(),nullptr),nullptr,day,tick,rd<U>(self,0x3C),rd<U>(self,0x40),&elapsed_day,&elapsed_tick);
 const U countdown=rd<U>(self,0xF0);wr<U>(self,0x44,elapsed_tick);
 if(countdown)wr<U>(self,0xF0,countdown>elapsed_tick?countdown-elapsed_tick:0);
 slot<Void>(self,0xE8)(self,nullptr);w3_placeable_animation_update_006832a0(self,nullptr);
 slot<Void>(self,0xC4)(self,nullptr);kotor_m15e_slot090_void0_004015a0(self,nullptr);
 if(kotor_fastcall_0063df60(self,nullptr)){
  void* const ai=kotor_m15e_client_get_client_ai_master_005ed6b0(client(),nullptr);
  kotor_m15e_client_ai_list_remove_object_00604810(at(ai,4),nullptr,self);
  void* removed;
  if(recovered_object_array_remove_004d81a0(kotor_fastcall_005ed570(client(),nullptr),nullptr,rd<U>(self,4),&removed)==0&&removed){
   slot<Delete>(self,0)(self,nullptr,1);return;
  }
 }
 wr<U>(self,0x3C,day);wr<U>(self,0x40,tick);
}
