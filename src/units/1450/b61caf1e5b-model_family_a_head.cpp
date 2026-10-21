// /EHs (without EHc): the real C-linkage provider calls can throw across the live StartupString.
#include "98e20b8cb4-model_family_a_internal.h"
#include "b6ebbd7b5e-startup_string.h"
using namespace model_family_a;
extern "C" void __fastcall model_family_a_00699ae0(void* self,void*) noexcept(false){
    kotor_reconstruction::StartupString unused; // Original005B3190/005E5C20, including exception cleanup.
    void* item=kotor_m15_client_get_item_by_id_005ed5a0(client(),nullptr,ld<U>(self,vb(self,-8,2)+0x40));
    if(item){
        void* object=off(item,0x10);
        void* model=ld<const KotorM15ObjectMethods_007516c8*>(object,0)->s098_get_model(object,nullptr,0xff,1);
        if(model){
            gvt(model)->s050_gob_attach_to_object(model,nullptr,nullptr,nullptr,0);
            gvt(model)->attach_to_scene(gob(model),nullptr,nullptr);
        }
    }
    st<U>(self,vb(self,-8,3)+0x44,1);
}
