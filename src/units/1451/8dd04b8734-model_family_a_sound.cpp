// /EHs (without EHc), to retain original constructor-only allocation cleanup/rethrow.
#include "98e20b8cb4-model_family_a_internal.h"
#include "916798d1e4-resources.h"
using namespace model_family_a;
namespace kotor_sound_rev3 {
void* __fastcall SourceConstruct005d60e0(void*,void*,const void*);
void __fastcall SourceSetPriorityGroup005d5900(void*,void*,unsigned int);
}
extern "C" {
void* __cdecl live_operator_new(U);
void __fastcall kotor_m15_sound_source_set_3d_005d5910(void*,void*,U);
void __fastcall kotor_m15_sound_source_set_looping_005d59d0(void*,void*,U);
void __fastcall kotor_m15_sound_source_set_position_005d59e0(void*,void*,const void*,float);
void __fastcall kotor_fastcall_005d5950(void*,void*,U);
U __fastcall kotor_fastcall_005d5930(void*,void*);
}
namespace {
const char torch_sound_00754d08[]="al_cv_firetorch1";
struct SoundSourceMethods {void* (__fastcall* destroy)(void*,void*,U);};
void set_position(void* self,void* model,kotor_scene3d::Vector& storage){
    const U result=gvt(model)->s064_gob_get_position(gob(model),nullptr,word(&storage));
    // The outstanding PUSH0 is the second SetPosition parameter; Gob::GetPosition pops only its out-pointer.
    kotor_m15_sound_source_set_position_005d59e0(ld<void*>(self,-8),nullptr,reinterpret_cast<const void*>(static_cast<std::uintptr_t>(result)),0.0f);
}
}
extern "C" void __fastcall model_family_a_0069a190(void* self,void*,U on) noexcept(false){
    if(on && ld<U>(self,-0x0c)!=0){
        if(!ld<void*>(self,-8)){
            void* memory=live_operator_new(8);
            void* source=nullptr;
            if(memory){
                try{
                    menu_resources::Ref reference;
                    reference.construct_00406d80(torch_sound_00754d08);
                    source=kotor_sound_rev3::SourceConstruct005d60e0(memory,nullptr,&reference);
                }catch(...){live_free(memory);throw;}
            }
            st<void*>(self,-8,source);
            if(!source)return;
            kotor_m15_sound_source_set_3d_005d5910(source,nullptr,1);
            kotor_sound_rev3::SourceSetPriorityGroup005d5900(ld<void*>(self,-8),nullptr,6);
            kotor_fastcall_005d5950(ld<void*>(self,-8),nullptr,10);
            kotor_m15_sound_source_set_looping_005d59d0(ld<void*>(self,-8),nullptr,1);
            (void)kotor_fastcall_005d5930(ld<void*>(self,-8),nullptr);
            if(!ld<void*>(self,-8))return;
        }
        void* item=kotor_m15_client_get_item_by_id_005ed5a0(client(),nullptr,ld<U>(self,-0x20));
        if(item){
            void* object=off(item,0x10);
            auto* table=ld<const KotorM15ObjectMethods_007516c8*>(object,0);
            if(table->s098_get_model(object,nullptr,0xff,1)){
                // Original deliberately calls GetModel again and dereferences its second result without another null guard.
                table=ld<const KotorM15ObjectMethods_007516c8*>(object,0);
                void* model=table->s098_get_model(object,nullptr,0xff,1);
                kotor_scene3d::Vector position;
                set_position(self,model,position);
                return;
            }
        }
        const U displacement=vb(self,-0x5c,1);
        if(ld<void*>(self,displacement+0x5c)){
            void* model=ld<void*>(self,displacement+0x5c);
            kotor_scene3d::Vector position;
            set_position(self,model,position);
        }
        return;
    }
    void* source=ld<void*>(self,-8);
    if(source){ld<const SoundSourceMethods*>(source,0)->destroy(source,nullptr,1);st<void*>(self,-8,nullptr);}
}
