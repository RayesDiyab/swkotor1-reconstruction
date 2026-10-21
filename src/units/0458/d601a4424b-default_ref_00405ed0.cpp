// Extracted verbatim from task-10/c2da-current-resource-lower-r1/source/resource_lower.cpp (sha256 f305d237...): c2da_default_ref_00405ed0 + store helper only.
#include "cfc9b5ff41-resource_lower.h"
#include <cstring>
#include <initializer_list>

using namespace c2da_resource_lower;
namespace {
template<class T>void store(void* p,unsigned offset,T value) {
    std::memcpy(static_cast<unsigned char*>(p)+offset,&value,sizeof(value));
}
}

extern "C" __declspec(noinline) Ref* __fastcall c2da_default_ref_00405ed0(Ref* self,void*) {
    for(unsigned offset:{0u,4u,8u,12u})store(self,offset,Word{0});
    return self;
}
