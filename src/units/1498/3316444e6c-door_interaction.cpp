#include "2d1a6b5349-door_contracts.h"
#include "ac7aea9252-door_x87.h"
#include "9ddb018d05-world_los.h"
namespace {
using namespace door;
struct CameraMethods {unsigned char prefix[0x24];U(__fastcall* orientation)(void*,void*,U);};
static_assert(offsetof(CameraMethods,orientation)==0x24,"Original camera orientation slot");
struct HitInfo {
    alignas(4) B storage[0x64];
    HitInfo(){(void)aurora_hit_info_construct_004578f0(storage,nullptr,0,0xbfffffffu,0);}
    ~HitInfo()noexcept(false){chit_info_destruct_00456b30(storage,nullptr);}
};
void difference(V& output,const V* a,const V* b){(void)kotor_cdecl_00423bc0(&output.x,&a->x,&b->x);}
V* gob_position(void* model,V& output){
    const U result=gob_table(model)->s064_gob_get_position(gob(model),nullptr,word(&output));
    return static_cast<V*>(pointer(result));
}
}
extern "C" void __fastcall w3_door_interaction_00684660(void* self,void*){
    using namespace door;
    void* gui=pointer(frame_R4_005ed690(client(),nullptr));
    void* player=client_get_player_creature_005ed540(client(),nullptr);
    if(!gui||!player||get<U>(self,0x12c)==0)return;
    const B state=get<B>(self,0x11c);
    if(state!=1&&state!=2)return;
    V direction{0,0,0};
    if(get<U>(gui,0xc1c)){
        void* camera=kotor_fastcall_005ed560(client(),nullptr);
        Q orientation;V axis;
        const U result=get<const CameraMethods*>(camera,0)->orientation(camera,nullptr,word(&orientation));
        // The earlier original push is QuaternionYDir's output, not a second
        // camera argument. Camera+24 consumes only its quaternion output word.
        V* output=quaternion_ydir_0044f420(static_cast<const Q*>(pointer(result)),nullptr,&axis);
        copy3(&direction,output);put<U>(&direction,8,0);
        V normalized;
        copy3(&direction,normalize_004ab180(&normalized,&direction));
    }else copy3(&direction,at(player,0x30));
    V* position=vector(self,0x24);
    V* player_position=vector(player,0x24);
    float rounded_dot;
    if(!door_x87::forward_dot(position,player_position,&direction,&rounded_dot))return;
    if(object_table(self)->s098_get_model(self,nullptr,0xff,1)){
        V minimum{0,0,0},maximum{0,0,0};
        Q rotation{1,0,0,0};
        void* model=object_table(self)->s098_get_model(self,nullptr,0xff,1);
        (void)gob_table(model)->s0ec_gob_get_bounding_box(gob(model),nullptr,word(&minimum),word(&maximum));
        const V facing{0,1,0};
        Q arc;
        // RotationArc takes two complete vectors by value in the original.
        rotation=*rotation_arc_004ab630(&arc,facing,*vector(self,0x30));
        V temporary;
        copy3(&minimum,quaternion_rotate_004a9bb0(&temporary,&rotation,&minimum));
        copy3(&maximum,quaternion_rotate_004a9bb0(&temporary,&rotation,&maximum));
        copy3(&minimum,vector_sum_00423b90(&temporary,&minimum,position));
        copy3(&maximum,vector_sum_00423b90(&temporary,&maximum,position));
        V scaled;
        door_x87::scale_direction(&direction,&scaled);
        V start;copy3(&start,player_position);
        V end;
        (void)vector_sum_00423b90(&end,player_position,&scaled);
        V intersection{0,0,0};
        put<U>(&end,8,0);put<U>(&start,8,0);
        if(!intersect_line_segments_2d_004ac340(&start,&end,&minimum,&maximum,&intersection))return;
        void* head=object_table(player)->s098_get_model(player,nullptr,0xfe,1);
        V head_position;
        const V* height_source=head?gob_position(head,head_position):vector(player,0x24);
        void* area=ptr(self,0x20);
        door_x87::height(height_source,&start,&intersection,head==nullptr);
        if(area){
            bool allowed=true;
            {
                HitInfo hit;
                void* scene=ptr(area,0x184);
                void* server_area=frame_GetServerArea_00605640(area,nullptr);
                put<U>(hit.storage,0,1);
                const auto* methods=get<const kotor_scene3d::SceneVTable*>(scene,0);
                if(methods->s038_scene_hit_check(scene,nullptr,start,intersection,nullptr,hit.storage)!=0)
                    allowed=false;
                else if(server_area){
                    float height;std::memcpy(&height,&start.z,4);
                    if(recovered_area_region_005086d0(server_area,nullptr,&start,&intersection,height,height,0.0f,0x7f000000u,nullptr,1)==0)
                        allowed=false;
                }
            }
            if(!allowed)return;
        }
        V previous,target;
        const auto* result=static_cast<const V*>(w3_area_position_0062ec20(gui,nullptr,&target));
        difference(previous,result,player_position);
        const U previous_id=get<U>(gui,0xc18);
        if(!door_x87::model_target(&start,&intersection,&previous,previous_id))return;
    }else{
        V separation;
        difference(separation,position,player_position);
        float rounded_squared;
        if(!door_x87::fallback_distance(&separation,&rounded_dot,&rounded_squared))return;
        V target,previous;
        const auto* result=static_cast<const V*>(w3_area_position_0062ec20(gui,nullptr,&target));
        difference(previous,result,player_position);
        if(!door_x87::fallback_target(&previous,&rounded_squared,get<U>(gui,0xc18)))return;
    }
    frame_R4_0062b020(gui,nullptr,get<U>(self,4),reinterpret_cast<U*>(position));
}

