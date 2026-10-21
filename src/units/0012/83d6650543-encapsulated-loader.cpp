#include "f7362fe553-resource_dispatch.h"
#include "d06e8b44c9-manager_demand.h"
#include <cstdint>

// Source proposal only. Main6 owns Manager, archive/resource objects, virtual
// method tables and lifetime. Selected Main177 owns the reused list accessors.
// No numeric original-address callbacks or object/data providers are introduced.
namespace {
using Word=std::uint32_t;
template<class T> T read(void* p,Word off) {
    return *reinterpret_cast<T volatile*>(static_cast<unsigned char*>(p)+off);
}
template<class T> void write(void* p,Word off,T value) {
    *reinterpret_cast<T volatile*>(static_cast<unsigned char*>(p)+off)=value;
}
template<class Fn> Fn method(void* p,Word slot) {
    return read<Fn>(read<void*>(p,0),slot);
}
using Effect=void(__thiscall*)(void*);
using Sized=Word(__thiscall*)(void*,Word);
using Token=void*(__thiscall*)(void*);
using Transfer=Word(__thiscall*)(void*,Word,void*,Word,Word);
void effect(void* p,Word slot) { method<Effect>(p,slot)(p); }
void* receiver(void* entry) { return read<void*>(read<void*>(entry,0x30),0); }
void* find_entry(menu_resources::Manager* manager,void* resource,Word list_offset);
}

// Existing selected opaque DWORD GetAt carrier; Next is in manager_demand.h.
extern "C" Word __fastcall unresolved_list_at_pos_005e9b60(void*,void*,void*);
// Observed Manager receiver in ECX, ignored EDX, one resource stack argument;
// complete EAX is tested. This unresolved import does not provide an allocator.
extern "C" Word __fastcall c2da_manager_004077b0(
    menu_resources::Manager*,void*,void*);

namespace {
void* find_entry(menu_resources::Manager* manager,void* resource,Word list_offset) {
    void* list=read<void*>(manager,list_offset);
    void* position=read<void*>(list,0);
    if (!position) return nullptr;
    void* entry=reinterpret_cast<void*>(unresolved_list_at_pos_005e9b60(
        list,nullptr,position));
    while (position) {
        if (entry) {
            const Word key=(read<Word>(resource,8)>>14)&0x3fu;
            if (((key^read<Word>(entry,0x28))&0x0fffffffu)==0) break;
        }
        entry=reinterpret_cast<void*>(unresolved_list_next_005e9b90(
            read<void*>(manager,list_offset),nullptr,&position));
    }
    return position?entry:nullptr;
}
}

// Complete original body 00407BD0..00407D45 (374 bytes, terminal RET8).
extern "C" Word __fastcall c2da_manager_00407bd0(
    menu_resources::Manager* manager,void*,void* resource,Word mode) {
    if (!resource || (read<unsigned char>(resource,0x0c)&4u)!=0) return 0;
    void* entry=find_entry(manager,resource,0x18);
    if (!entry) return 0;
    void* archive=receiver(entry);
    if (mode) {
        effect(archive,0x08);
        archive=receiver(entry);
        const Word size=method<Sized>(archive,0x24)(
            archive,read<Word>(resource,8)&0x3fffu);
        write(resource,0x18,size);
        if (!c2da_manager_004077b0(manager,nullptr,resource)) {
            effect(receiver(entry),0x18);
            return 0;
        }
        archive=receiver(entry);
        write(manager,0x2c,method<Token>(archive,0x20)(archive));
        archive=receiver(entry);
        void* data=read<void*>(resource,0x10);
        const Word key=read<Word>(resource,8);
        (void)method<Transfer>(archive,0x38)(archive,key,data,size,0u);
        return 1;
    }
    effect(archive,0x04);
    archive=receiver(entry);
    const Word size=method<Sized>(archive,0x24)(
        archive,read<Word>(resource,8)&0x3fffu);
    write(resource,0x18,size);
    if (!c2da_manager_004077b0(manager,nullptr,resource)) {
        effect(receiver(entry),0x14);
        return 0;
    }
    archive=receiver(entry);
    void* data=read<void*>(resource,0x10);
    const Word key=read<Word>(resource,8);
    if (method<Transfer>(archive,0x34)(archive,key,data,size,0u)!=size) {
        effect(receiver(entry),0x14);
        return 0;
    }
    effect(receiver(entry),0x14);
    const Word result=c2da_r29_dispatch::serviced(resource);
    write(resource,0x0c,(read<Word>(resource,0x0c)&~4u)|(result?4u:0u));
    return result;
}
