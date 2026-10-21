#include "d14b72eb41-model_family_b.h"
#include "628decf93e-m15e_animbase_internal.h"
using namespace kotor_m15e_ab;

extern "C" {
void __fastcall model_family_b_00699ad0(void* self,void*,U item) noexcept(false) {
    st<U>(self,-8,item);                                                 // 00699AD4
}
// Head destructor's input is head+50h; the allocation starts50h earlier.
void* __fastcall model_family_b_0069bc80(void* self,void*,U flags) noexcept(false) {
    void* const allocation=at(self,-0x50);
    kotor_m15e_anim_base_head_destruct_0069b030(self,nullptr);
    kotor_m15e_anim_base_tw_destruct_0069cd10(self,nullptr);              // same original ECX, not a vbtable lookup
    if ((flags&1u)!=0) live_free(allocation);
    return allocation;
}
void* __fastcall model_family_b_0069cd60(void* self,void*,U flags) {
    kotor_m15e_anim_base_tw_destruct_0069cd10(self,nullptr);
    if ((flags&1u)!=0) live_free(self);
    return self;
}
void* __fastcall model_family_b_006a03a0(void* self,void*,U flags) {
    kotor_m15e_anim_base_destruct_0069e140(self,nullptr);
    if ((flags&1u)!=0) live_free(self);
    return self;
}
}
