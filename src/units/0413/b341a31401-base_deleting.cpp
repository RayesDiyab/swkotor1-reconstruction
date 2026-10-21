#include "ce93ee50d7-base_methods.h"
#include "a96315eb76-destruction.h"
#include "6542d604d6-allocator_live.h"

extern "C" void* __fastcall cres_base_deleting_destroy_00409d70(
    void* self,void*,cres_base_methods::Word flags) {
    // 00409D70 inlines the same base operation recovered at00409D40.
    c2da_resource_base_destroy_00409d40(self,nullptr);
    if((flags&1u)!=0u)live_free(self);
    return self;
}
