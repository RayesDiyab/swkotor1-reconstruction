#include "e5c8c0ea63-resource_methods.h"
#include "d06e8b44c9-manager_demand.h"
#include "572d069072-base_cancel_service.h"
#include "92d14e6be8-packed_file.h"   // lane-packed-file: kotor_packed_file::ImageMethods (0073E240), D1 fastcall slots
#include <cstdint>
#include <cstring>

// Original evidence owner: Task10 static demand capsule. New source proposal:
// r25_resource_loaders. Manager is the current canonical imported type; all
// resource/key-entry/method receivers remain opaque existing objects.
// D1 replacement (lane-resman, review XC-1): method-table cells are read with the
// exact __fastcall types they are stored with; no __thiscall pointer types and no
// function-pointer casts remain.
namespace {
using Word=std::uint32_t;
template<class T> T read(void* p,Word off) {
    return *reinterpret_cast<T volatile*>(static_cast<unsigned char*>(p)+off);
}
template<class T> void write(void* p,Word off,T value) {
    *reinterpret_cast<T volatile*>(static_cast<unsigned char*>(p)+off)=value;
}
// Archive receiver table (CExoResourceImageFile 0073E240): object-pointer read only.
const kotor_packed_file::ImageMethods* image_methods(void* receiver) {
    const kotor_packed_file::ImageMethods* table;
    std::memcpy(&table,receiver,sizeof(table));
    return table;
}
// CRes method table slot +10 (Serviced). Every CRes table (0073EC9C C2DA, 0073E2D0 GFF,
// 0075FC5C TPC, ...) stores a c2da_resource_methods::ResourceCallback there (D1); the
// cell is copied out with its own type.
c2da_resource_methods::ResourceCallback serviced_slot(void* resource) {
    const unsigned char* table=read<const unsigned char*>(resource,0);
    c2da_resource_methods::ResourceCallback callback;
    std::memcpy(&callback,table+16,sizeof(callback));
    return callback;
}
}

// Selected Main177 resource-dependencies/key-chain/list_exports.cpp is the
// existing GetAt owner; reuse its exact full-DWORD opaque carrier once.
extern "C" Word __fastcall unresolved_list_at_pos_005e9b60(void*,void*,void*);

extern "C" void __fastcall c2da_manager_00408b90(
    menu_resources::Manager* manager,void*,void* resource,Word) {
    if (!resource) return;
    if (read<std::uint16_t>(resource,4)>0) {
        write(resource,12,read<Word>(resource,12)|0x100u);
        return;
    }
    if ((read<unsigned char>(resource,12)&0x14u)!=0 &&
        read<void*>(resource,16)!=nullptr) {
        c2da_manager_00407020(manager,nullptr,resource);
    }
}

// 00407D50 CExoResMan::ServiceFromImage(CRes*, int) RET8.
extern "C" Word __fastcall c2da_manager_00407d50(
    menu_resources::Manager* manager,void*,void* resource,Word) {
    if (!resource || (read<unsigned char>(resource,12)&4u)!=0) return 0;      // 00407D56-00407D66
    void* list=read<void*>(manager,28);                                       // 00407D69 manager+1C
    void* position=read<void*>(list,0);
    if (!position) return 0;
    void* entry=reinterpret_cast<void*>(unresolved_list_at_pos_005e9b60(
        list,nullptr,position));                                              // 00407D78
    while (position) {
        if (entry) {
            const Word key=(read<Word>(resource,8)>>14)&0x3fu;                // 00407D8B-00407D94
            if (((key^read<Word>(entry,40))&0x0fffffffu)==0) break;           // 00407D97-00407D9E
        }
        entry=reinterpret_cast<void*>(unresolved_list_next_005e9b90(
            read<void*>(manager,28),nullptr,&position));                      // 00407DA8
    }
    if (!position) return 0;                                                  // 00407DB7
    const Word index=read<Word>(resource,8)&0x3fffu;                          // 00407DBF-00407DC9
    void* receiver=read<void*>(read<void*>(entry,48),0);                      // 00407DC2-00407DC5
    const Word size=image_methods(receiver)->packed.get_resource_size(receiver,nullptr,index); // 00407DD0 [+24]
    write(resource,24,size);                                                  // 00407DD3
    receiver=read<void*>(read<void*>(entry,48),0);                            // 00407DD6-00407DD9 (reloaded)
    void* data=image_methods(receiver)->get_resource(receiver,nullptr,index);// 00407DDE [+44]
    write(resource,16,data);                                                  // 00407DE1
    write(resource,12,read<Word>(resource,12)|4u);                            // 00407DE4
    return serviced_slot(resource)(resource,nullptr);                         // 00407DE8-00407DEC [+10]
}
