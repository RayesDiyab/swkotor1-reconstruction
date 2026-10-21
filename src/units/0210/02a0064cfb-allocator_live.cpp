#include "b0cfb063de-allocator_live.h"
LiveAllocatorGlobals live_globals{};
LiveAllocatorDependencies live_dependencies{};
extern "C" __declspec(noinline) void* __cdecl live_heap_alloc(std::uint32_t size){
    if(*live_globals.mode==3&&size<=*live_globals.threshold){
        live_dependencies.lock(4);void* p=live_dependencies.sbh_alloc(size);live_dependencies.unlock(4);if(p)return p;
    }
    if(!size)size=1;if(*live_globals.mode!=1)size=(size+15u)&0xfffffff0u;
    return live_dependencies.heap_alloc(*live_globals.heap,0,size);
}
extern "C" __declspec(noinline) int __cdecl live_callnewh(std::uint32_t size){const auto handler=*live_globals.handler;return handler&&handler(size)!=0?1:0;}
extern "C" __declspec(noinline) void* __cdecl live_nh_malloc(std::uint32_t size,int flag){if(size>0xffffffe0u)return nullptr;for(;;){void* p=live_heap_alloc(size);if(p)return p;if(!flag||!live_callnewh(size))return nullptr;}}
extern "C" __declspec(noinline) void* __cdecl live_operator_new(std::uint32_t size){return live_nh_malloc(size,1);}
extern "C" __declspec(noinline) void* __cdecl live_malloc(std::uint32_t size){return live_nh_malloc(size,static_cast<int>(*live_globals.new_mode));}
extern "C" __declspec(noinline) void* __cdecl live_sbh_find(void* p){
    const auto address=static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(p));
    auto header=*live_globals.header_base;const auto end=header+*live_globals.header_count*20u;
    while(header<end){const auto base=*reinterpret_cast<const std::uint32_t*>(header+12u);if(address-base<0x100000u)return reinterpret_cast<void*>(header);header+=20u;}return nullptr;
}
extern "C" __declspec(noinline) void __cdecl live_free(void* p){if(!p)return;if(*live_globals.mode==3){live_dependencies.lock(4);void* header=live_sbh_find(p);if(header)live_dependencies.sbh_free(header,p);live_dependencies.unlock(4);if(header)return;}live_dependencies.heap_free(*live_globals.heap,0,p);}
