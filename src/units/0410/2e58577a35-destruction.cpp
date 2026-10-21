#include "a96315eb76-destruction.h"
#include "6542d604d6-allocator_live.h"
#include <cstring>

extern "C" void __fastcall c2da_resource_destroy_0041d760(void* self,void*) {
    const void* methods=c2da_resource_methods_0073ec9c();
    std::memcpy(self,&methods,sizeof(methods));
    c2da_resource_base_destroy_00409d40(self,nullptr);
}

extern "C" void* __fastcall c2da_resource_deleting_destroy_0041d7f0(
    void* self,void*,c2da_resource_methods::Word flags) {
    c2da_resource_destroy_0041d760(self,nullptr);
    // Original006FA390 is a cdecl JMP to006FB7B2. Reuse the selected live_free
    // owner and its live allocator state; never substitute std::free/new/delete.
    if((flags&1u)!=0u)live_free(self);
    return self;
}
