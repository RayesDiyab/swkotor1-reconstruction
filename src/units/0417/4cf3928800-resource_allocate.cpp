#include "708fbab234-manager_demand.h"
#include "6542d604d6-allocator_live.h"
#include <cstdint>
#include <type_traits>

// Original CExoResMan::Malloc, 004077B0..00407825. This remains an isolated
// source proposal. All Manager/resource lifetimes and heap ownership are Main's.
namespace {
using Word=std::uint32_t;
template<class T> T read(void* p,Word offset) {
    return *reinterpret_cast<T volatile*>(static_cast<unsigned char*>(p)+offset);
}
template<class T> void write(void* p,Word offset,T value) {
    *reinterpret_cast<T volatile*>(static_cast<unsigned char*>(p)+offset)=value;
}
}
extern "C" Word __fastcall c2da_manager_004070a0(menu_resources::Manager*,void*);

extern "C" Word __fastcall c2da_manager_004077b0(
    menu_resources::Manager* manager,void*,void* resource) {
    // JLE is signed. Both words are reread after each successful eviction.
    while(read<std::int32_t>(resource,0x18)>read<std::int32_t>(manager,8)) {
        if(c2da_manager_004070a0(manager,nullptr)==0)break;
    }
    const Word size=read<Word>(resource,0x18);
    write(manager,8,read<Word>(manager,8)-size);
    void* data;
    if(read<Word>(resource,0x20)!=0) {
        void* allocation=live_operator_new(read<Word>(resource,0x18)+Word{6});
        // Preserve the original wrapping ADD even when allocation is null.
        const Word address=static_cast<Word>(reinterpret_cast<std::uintptr_t>(allocation));
        data=reinterpret_cast<void*>(static_cast<std::uintptr_t>(address+Word{6}));
    } else {
        const bool extra=read<Word>(resource,0x24)!=0;
        data=live_operator_new(read<Word>(resource,0x18)+(extra?Word{10}:Word{0}));
    }
    write(resource,0x10,data);
    if(data==nullptr) {
        const Word restore=read<Word>(resource,0x18);
        write(manager,8,read<Word>(manager,8)+restore);
        return 0;
    }
    return 1;
}
using Allocate=Word(__fastcall*)(menu_resources::Manager*,void*,void*);
static_assert(std::is_same<decltype(&c2da_manager_004077b0),Allocate>::value,"full EAX and RET4 carrier");
static_assert(sizeof(void*)==4,"x86 pointer arithmetic");
