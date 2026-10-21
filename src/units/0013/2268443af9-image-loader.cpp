#include "35cee25c6e-resource_dispatch.h"
#include "d06e8b44c9-manager_demand.h"
#include "2d6a840a27-ref_string_joins.h"
#include <cstdint>

// Original 00407E00..00407FD1, 466 bytes. Main6 owns this real cell, all
// nominal objects, method tables and lifetime. These are declarations only.
namespace kotor_menu_10_19 { namespace dependencies {
extern void* base_007a39e0;
}}
extern "C" std::uint32_t __fastcall unresolved_list_at_pos_005e9b60(void*,void*,void*);
extern "C" std::uint32_t __fastcall c2da_manager_004077b0(
    menu_resources::Manager*,void*,void*);
#include "b472198264-shared_boundary.hpp"
extern "C" kotor_reconstruction::StartupString* __fastcall c2da_base_005e6660(
    void*,void*,std::uint32_t);

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
void* receiver(void* entry,Word bif) {
    return read<void*>(read<void*>(entry,0x30),bif*4u);
}
}

extern "C" Word __fastcall c2da_manager_00407e00(
    menu_resources::Manager* manager,void*,void* resource,Word mode) {
    if (!resource || (read<unsigned char>(resource,0x0c)&4u)!=0) return 0;
    void* list=read<void*>(manager,0x10);
    void* position=read<void*>(list,0);
    // Captured before GetAt or any callback; subsequent resource key changes
    // do not change this physical archive receiver array index.
    const Word bif=(read<Word>(resource,8)>>20)&0x3ffu;
    if (!position) return 0;
    void* entry=reinterpret_cast<void*>(unresolved_list_at_pos_005e9b60(
        list,nullptr,position));
    while (position) {
        if (entry) {
            const Word key=(read<Word>(resource,8)>>14)&0x3fu;
            if (((key^read<Word>(entry,0x28))&0x0fffffffu)==0) break;
        }
        entry=reinterpret_cast<void*>(unresolved_list_next_005e9b90(
            read<void*>(manager,0x10),nullptr,&position));
    }
    if (!position) return 0;
    void* archive=receiver(entry,bif);
    if (mode) {
        effect(archive,0x08);
        archive=receiver(entry,bif);
        const Word size=method<Sized>(archive,0x24)(
            archive,read<Word>(resource,8)&0x3fffu);
        write(resource,0x18,size);
        if (!c2da_manager_004077b0(manager,nullptr,resource)) {
            effect(receiver(entry,bif),0x18);
            return 0;
        }
        archive=receiver(entry,bif);
        write(manager,0x2c,method<Token>(archive,0x20)(archive));
        archive=receiver(entry,bif);
        void* data=read<void*>(resource,0x10);
        const Word key=read<Word>(resource,8);
        (void)method<Transfer>(archive,0x38)(archive,key,data,size,0u);
        return 1;
    }
    effect(archive,0x04);
    archive=receiver(entry,bif);
    const Word size=method<Sized>(archive,0x24)(
        archive,read<Word>(resource,8)&0x3fffu);
    write(resource,0x18,size);
    if (!c2da_manager_004077b0(manager,nullptr,resource)) {
        effect(receiver(entry,bif),0x14);
        return 0;
    }
    archive=receiver(entry,bif);
    void* data=read<void*>(resource,0x10);
    const Word key=read<Word>(resource,8);
    if (method<Transfer>(archive,0x34)(archive,key,data,size,0u)!=size) {
        effect(receiver(entry,bif),0x14);
        return 0;
    }
    effect(receiver(entry,bif),0x14);
    const Word result=c2da_r29_dispatch::serviced(resource);
    if (result) {
        c2da_ref_00405fe0(read<void*>(resource,0x14),nullptr);
        void* key_entry=read<void*>(resource,0x14);
        const Word type=static_cast<Word>(read<std::uint16_t>(key_entry,0x1a));
        auto* extension=c2da_base_005e6660(
            kotor_menu_10_19::dependencies::base_007a39e0,nullptr,type);
        (void)async_string_cstr_005e5670(extension,nullptr);
        archive=receiver(entry,bif);
        (void)async_string_cstr_005e5670(
            reinterpret_cast<kotor_reconstruction::StartupString*>(
                static_cast<unsigned char*>(archive)+4),nullptr);
        c2da_ref_00405fe0(read<void*>(resource,0x14),nullptr);
    }
    write(resource,0x0c,(read<Word>(resource,0x0c)&~4u)|(result?4u:0u));
    return result;
}
