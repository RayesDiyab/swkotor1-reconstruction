#include "74ed618f22-resource_methods.h"
#include <cstring>

namespace {
using Word=c2da_resource_methods::Word;
template<class T> T load(const void* object,unsigned offset) {
    T value;
    std::memcpy(&value,static_cast<const unsigned char*>(object)+offset,sizeof(value));
    return value;
}
template<class T> void store(void* object,unsigned offset,T value) {
    std::memcpy(static_cast<unsigned char*>(object)+offset,&value,sizeof(value));
}

// Source-port domain: the live descriptor is a Methods0073ec9c object whose
// +0C entry has ResourceCallback type. The table and its selected M36 caller
// establish that join. This is not a cast between function-pointer types.
// Retain the original live virtual dispatch; do not replace it with a direct
// call to c2da_resource_freed_0041d770.
void notify_freed(void* object) {
    const void* address=load<const void*>(object,0);
    const auto* methods=static_cast<const c2da_resource_methods::Methods0073ec9c*>(address);
    (void)methods->freed_0041d770(object,nullptr);
}
}

extern "C" c2da_resource_methods::Word __fastcall
c2da_resource_freed_0041d770(void* self,void*) {
    store(self,0x28,Word{0});
    store(self,0x2c,Word{0});
    store(self,0x30,Word{0});
    store(self,0x34,Word{0});
    store(self,0x38,Word{0});
    return 1u;
}

extern "C" c2da_resource_methods::Word __fastcall
c2da_resource_serviced_0041d790(void* self,void*) {
    void* data=load<void*>(self,0x10);
    if(!data)return 0u;
    if(load<Word>(self,0x28)!=0u)return 1u;

    // The original subtracts with DWORD wrap and does not check size first.
    store(self,0x2c,load<Word>(self,0x18)-Word{8});
    store(self,0x34,data);
    if(load<Word>(data,0)!=0x20414432u) {
        notify_freed(self);
        return 0u;
    }
    const Word version=load<Word>(data,4);
    if(version==0x622e3256u) {
        store(self,0x38,Word{1});
    } else if(version!=0x302e3256u) {
        notify_freed(self);
        return 0u;
    }
    // V2.0 deliberately preserves the existing +38 word.
    store(self,0x30,static_cast<unsigned char*>(data)+8);
    store(self,0x28,Word{1});
    return 1u;
}
