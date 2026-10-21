// Original00619E10 dust/splash/leaves continuation, with real fixed providers.
#include "5aec12c3b5-m15_dust.h"
#include "b96846784b-m15_dust_models.h"
#include "02d88fb64f-m15_dust_numeric.h"
#include "62420f93e8-allocator_live.h"
#include "cca80f7c35-m15e.h"
#include "916798d1e4-resources.h"
#include "b6ebbd7b5e-startup_string.h"
#include "7dff9333b0-string_priority.h"
#include "1a441fc83c-vtables_gen.h"
#include <cstddef>
#include <cstring>
extern "C" {
void* __fastcall area_vfx_construct_0068fe70(void*,void*,std::uint32_t);
void __fastcall area_vfx_play_00690510(void*,void*,std::uint32_t);
void* __fastcall async_ref_construct_string_00406d60(void*,void*,const void*);
float* __cdecl kotor_cdecl_00423bc0(float*,const float*,const float*);
kotor_scene3d::Vector* __cdecl vector_component_mult_00423bf0(kotor_scene3d::Vector*,const kotor_scene3d::Vector*,float);
extern volatile std::uint32_t scene3d_hit_trimeshes_007fbf4c;
}
namespace kotor_menu_window_cells {extern void* volatile manager_007a39fc;}
namespace {
using U=std::uint32_t;using B=unsigned char;using W=std::uint16_t;
using V=kotor_scene3d::Vector;
template<class T> T load(const void* p,U off){T value;std::memcpy(&value,static_cast<const B*>(p)+off,sizeof value);return value;}
template<class T> void store(void* p,U off,T value){std::memcpy(static_cast<B*>(p)+off,&value,sizeof value);}
template<class T> T* at(void* p,U off){return reinterpret_cast<T*>(static_cast<B*>(p)+off);}
const KotorM15ObjectMethods_007516c8* methods(void* self){return load<const KotorM15ObjectMethods_007516c8*>(self,0);}
struct AllocationGuard {void* memory;~AllocationGuard(){if(memory)live_free(memory);}};
// Actual ignored-model ArrayMutations is a constructed trivial subobject at+8;
// all other CHitInfo bytes retain the real constructor/destructor's ownership.
struct HitInfo {
    U prefix[2];kotor_reconstruction::ArrayMutations ignored;B tail[0x50];
    HitInfo(){aurora_hit_info_construct_004578f0(this,nullptr,0,0xbfffffffu,0);}
    ~HitInfo() noexcept(false){chit_info_destruct_00456b30(this,nullptr);}
};
static_assert(sizeof(HitInfo)==0x64 && offsetof(HitInfo,ignored)==8,"Original stack HitInfo carrier");
bool moving(W animation){
    switch(animation){case 0x2712:case 0x2713:case 0x2714:case 0x275e:case 0x275f:
    case 0x2764:case 0x2765:case 0x2766:case 0x2767:return true;default:return false;}
}
void query_height(void* actor,const V& ray_start){
    HitInfo hit;
    V point{0.0f,0.0f,0.0f};
    hit.prefix[0]=1;
    void* const client=load<void*>(kotor_menu_window_cells::manager_007a39fc,4);
    m15_dust_collect_client_005ed9a0(client,nullptr,load<U>(actor,4),&hit.ignored);
    const U saved=scene3d_hit_trimeshes_007fbf4c;
    scene3d_hit_trimeshes_007fbf4c=1;
    // Original does not restore this global when HitCheck throws. Its EH state
    // still destroys HitInfo; do not add a new exception-time restoration.
    V destination;std::memcpy(&destination,at<void>(actor,0x24),sizeof destination);
    void* const scene=load<void*>(load<void*>(actor,0x20),0x184);
    const auto* const table=load<const kotor_scene3d::SceneVTable*>(scene,0);
    const int found=table->s038_scene_hit_check(scene,nullptr,ray_start,destination,&point,&hit);
    m15_dust_numeric::add32(at<float>(actor,0x304),found?&point.z:at<float>(actor,0x2c),found?0x3c23d70au:0x3eb33333u);
    scene3d_hit_trimeshes_007fbf4c=saved;
}
}
extern "C" void __fastcall m15_dust_emit_00619e10(void* self,void*,U kind_word,U refresh_height){
    void* const memory=live_operator_new(0x130);
    void* effect=nullptr;
    if(memory!=nullptr){
        AllocationGuard guard{memory};
        effect=area_vfx_construct_0068fe70(memory,nullptr,1);
        guard.memory=nullptr;
    }
    if(effect==nullptr)return;
    kotor_reconstruction::StartupString name;
    const B kind=static_cast<B>(kind_word);
    switch(kind){
    case 0:GuiAssign005e5140(&name,nullptr,"fx_step_splash");break;
    case 1:GuiAssign005e5140(&name,nullptr,"fx_step_dust");break;
    case 2:GuiAssign005e5140(&name,nullptr,"fx_step_leaf");break;
    default:return;
    }
    menu_resources::Ref reference;
    async_ref_construct_string_00406d60(&reference,nullptr,&name);
    if(methods(effect)->s078_load_model(effect,nullptr,&reference,0xff,8)==0 || load<void*>(self,0x20)==nullptr)return;
    V point{0.0f,0.0f,0.0f};
    if(!moving(load<W>(self,0x48))){
        std::memcpy(&point,at<void>(self,0x24),sizeof point);
    }else if(methods(self)->s098_get_model(self,nullptr,0xff,1)!=nullptr){
        V step;
        const bool backwards=load<W>(self,0x48)==0x2713;
        vector_component_mult_00423bf0(&step,at<V>(self,0x30),kind==0?0.5f:0.1f);
        if(backwards)kotor_cdecl_00423bc0(&point.x,at<float>(self,0x24),&step.x);
        else vector_sum_00423b90(&point,at<V>(self,0x24),&step);
        V ray_start;std::memcpy(&ray_start,at<void>(self,0x24),sizeof ray_start);
        m15_dust_numeric::add32(&ray_start.z,at<float>(self,0x2c),0x3f800000u);
        if((refresh_height!=0 || m15_dust_numeric::refresh_height(at<float>(self,0x2c),at<float>(self,0x304))) &&
            load<void*>(load<void*>(self,0x20),0x184)!=nullptr)query_height(self,ray_start);
        if(kind==0){
            const V up{0.0f,0.0f,1.0f};V cross;
            vector_cross_004ab210(&cross,&up,at<V>(self,0x30));
            if(m15_dust_numeric::cross_is_large(&cross)){
                V normalized;normalize_004ab180(&normalized,&cross);
                const bool add=crt_rand_006fc447()%2!=0;
                const int remainder=crt_rand_006fc447()%20;
                float rate;m15_dust_numeric::random_rate(&rate,remainder);
                V jitter;vector_component_mult_00423bf0(&jitter,&normalized,rate);
                V result;
                if(add)vector_sum_00423b90(&result,&point,&jitter);
                else kotor_cdecl_00423bc0(&result.x,&point.x,&jitter.x);
                std::memcpy(&point,&result,sizeof point);
            }
            std::memcpy(&point.z,at<void>(self,0x304),4);
        }
    }
    methods(effect)->s094_attach_to_scene(effect,nullptr,load<void*>(load<void*>(self,0x20),0x184));
    const auto* const position_methods=methods(effect);
    store(effect,0x11c,1u);store(effect,0x118,1u);
    position_methods->s08c_set_position(effect,nullptr,&point.x);
    methods(effect)->s088_set_orientation(effect,nullptr,at<float>(self,0x30));
    area_vfx_play_00690510(effect,nullptr,1);
}
