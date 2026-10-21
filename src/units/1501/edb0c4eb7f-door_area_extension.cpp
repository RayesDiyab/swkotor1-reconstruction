#include "e0867b18fc-door_area_contracts.h"
#include "9db2eb65b9-door_area_x87.h"
namespace {
using namespace door;
V* return_vector(V* output,const void* source){
    // Original loads all three DWORDs before its first output write.
    const U x=get<U>(source,0),y=get<U>(source,4),z=get<U>(source,8);
    put<U>(output,0,x);put<U>(output,4,y);put<U>(output,8,z);return output;
}
void transform_action(void* mesh,U offset,V& output){
    (void)collision_mesh_local_to_world_00596aa0(mesh,nullptr,&output,vector(mesh,offset));
}
void* server(){return ptr(menu_continuation::app_manager_007a39fc,8);}
}
extern "C" kotor_scene3d::Vector* __fastcall w3_placeable_nearest_action_point_00584c20(
    void* self,void*,kotor_scene3d::Vector* output,const kotor_scene3d::Vector* query){
    using namespace door;
    void* mesh=at(self,0x3a0);
    V first;
    transform_action(mesh,0x70,first);
    if(door_area_x87::ordered_zero(&first))return return_vector(output,at(self,0x90));
    transform_action(mesh,0x70,first);
    V first_difference;
    door_area_x87::difference(&first_difference,query,&first);
    V second;
    transform_action(mesh,0x7c,second);
    V second_difference;
    door_area_x87::difference(&second_difference,query,&second);
    transform_action(mesh,0x7c,second);
    if(!door_area_x87::ordered_zero(&second)&&
        !door_area_x87::second_farther(&first_difference,&second_difference)){
        transform_action(mesh,0x7c,second);
        return return_vector(output,&second);
    }
    transform_action(mesh,0x70,second);
    return return_vector(output,&second);
}
extern "C" void __fastcall w3_area_door_position_update_00508180(void* self,void*,
    const kotor_scene3d::Vector* origin,const kotor_scene3d::Vector* second,
    const kotor_scene3d::Vector* third,std::uint32_t object_id,std::int32_t action_points){
    using namespace door;
    U index=0;
    (void)recovered_area_index_00506f20(self,nullptr,&index,door_area_x87::index_bits(origin,second));
    void* list=at(self,0x190);
    V current{0,0,0},target{0,0,0};
    if(!list||get<I>(list,4)<=0||static_cast<I>(index)>=get<I>(list,4))return;
    do{
        const U creature_id=get<U>(ptr(list,0),index*4u);
        void* creature=kotor_fastcall_004ae770(server(),nullptr,creature_id);
        ++index;
        if(creature){
            copy3(&current,at(creature,0x90));
            const U bounds=door_area_x87::bounds_action(second,third,&current);
            if(bounds==2)return;
            if(bounds==1&&w2x_area_safe_point_004be5e0(self,nullptr,&current,get<U>(creature,0x340))==0){
                if(action_points){
                    if(void* door_object=w5_server_get_door_by_id_004ae7c0(server(),nullptr,object_id)){
                        // This selected provider owns a distinct nominal Vector.
                        w5::Vector from,result;
                        copy3(&from,&current);
                        const w5::Vector* returned=w5_door_get_nearest_action_point_00589240(door_object,nullptr,&result,&from,0);
                        copy3(&target,returned);
                    }else if(void* placeable=recovered_placeable_lookup_004ae7a0(server(),nullptr,object_id)){
                        V result;
                        const V* returned=w3_placeable_nearest_action_point_00584c20(placeable,nullptr,&result,&current);
                        copy3(&target,returned);
                    }
                    // Original retains target across iterations if both lookups fail.
                }else{
                    V separation,normalized;
                    door_area_x87::difference(&separation,&current,origin);
                    const V* result=normalize_004ab180(&normalized,&separation);
                    door_area_x87::add(&target,&current,result);
                }
                if(w1x_area_safe_location_004be860(self,nullptr,word(&current),0x41200000u,
                    get<U>(creature,0x340),0,word(&target))!=0)
                    recovered_object_position_004cd1a0(creature,nullptr,&target,1,1,0);
            }
        }
    }while(static_cast<I>(index)<get<I>(list,4));
}
