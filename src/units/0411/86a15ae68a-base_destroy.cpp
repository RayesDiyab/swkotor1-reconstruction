#include "a96315eb76-destruction.h"
#include "6e414386b5-base_cancel_service.h"
#include <cstring>

namespace {
template<class T> T load(const void* object,unsigned offset) {
    T value;
    std::memcpy(&value,static_cast<const unsigned char*>(object)+offset,sizeof(value));
    return value;
}
template<class T> void store(void* object,unsigned offset,T value) {
    std::memcpy(static_cast<unsigned char*>(object)+offset,&value,sizeof(value));
}
}

extern "C" void __fastcall c2da_resource_base_destroy_00409d40(void* self,void*) {
    // Original CMP precedes the unconditional base-table store.
    const std::uint16_t demands=load<std::uint16_t>(self,4);
    const void* methods=c2da_resource_base_methods_0073d86c();
    store(self,0,methods);
    if(demands>0u) {
        store(self,0x0c,load<std::uint32_t>(self,0x0c)|0x100u);
        return;
    }
    if((load<unsigned char>(self,0x0c)&0x14u)!=0u &&
       load<void*>(self,0x10)!=nullptr) {
        c2da_manager_00407020(c2da_manager_007a39e8(),nullptr,self);
    }
}
