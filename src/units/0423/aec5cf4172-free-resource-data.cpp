#include "d5add4ffcc-free_resource_data.h"
#include "57ef6354a2-allocator_live.h"
#include <cstdint>
#include <type_traits>

// Sole body author: /root/r25_resource_loaders. Main6 owns integration and the
// canonical allocator. Authenticated original 00406540..00406594, RET4.
namespace {
using FreeWord=std::uint32_t;
template<class T> T free_read(void* p,FreeWord offset) {
    return *reinterpret_cast<T volatile*>(static_cast<unsigned char*>(p)+offset);
}
template<class T> void free_write(void* p,FreeWord offset,T value) {
    *reinterpret_cast<T volatile*>(static_cast<unsigned char*>(p)+offset)=value;
}
}
extern "C" void __fastcall c2da_manager_00406540(
    menu_resources::Manager* manager,void*,void* resource) {
    if(!resource) return;
    void* data=free_read<void*>(resource,0x10);
    if(!data) return;
    if((free_read<FreeWord>(resource,8)&0xc0000000u)!=0x40000000u) {
        if(free_read<FreeWord>(resource,0x20)!=0) {
            const FreeWord pointer=static_cast<FreeWord>(reinterpret_cast<std::uintptr_t>(data));
            data=reinterpret_cast<void*>(static_cast<std::uintptr_t>(pointer-6u));
        }
        live_free(data);
        // Both reads follow free, in original order; wrapping ADD is observed.
        const FreeWord size=free_read<FreeWord>(resource,0x18);
        const FreeWord pool=free_read<FreeWord>(manager,8);
        free_write(manager,8,pool+size);
    }
    free_write(resource,0x18,FreeWord(0));
    free_write(resource,0x10,FreeWord(0));
}
using FreeContract=void(__fastcall*)(menu_resources::Manager*,void*,void*);
static_assert(std::is_same<decltype(&c2da_manager_00406540),FreeContract>::value,"ECX Manager, EDX unused, resource stack RET4");
static_assert(sizeof(void*)==4 && sizeof(FreeWord)==4,"x86 pointer/DWORD widths");
