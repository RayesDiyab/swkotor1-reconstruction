#include "42bcb3815d-directory_ref_boundary.hpp"
#include "85f96753c0-resource_dispatch.h"
#include "d06e8b44c9-manager_demand.h"
#include "572d069072-base_cancel_service.h"
#include "9ac2fa9d10-startup_string.h"
#include "dec5cccce1-string_priority.h"
#include "5072eb8c01-directory_data.h"
#include <cstdint>
#include <type_traits>

using DirectoryWord=std::uint32_t;
using DirectoryString=kotor_reconstruction::StartupString;

// These unresolved ports retain the observed physical arguments. Main6 owns
// their eventual donor selection and genuine data/receiver/lifetime bindings.
extern "C" {
// Ref declaration supplied by directory_ref_boundary.hpp.
DirectoryWord __fastcall c2da_file_005e6a10(void*,void*);
DirectoryWord __fastcall c2da_file_005e6950(void*,void*);
DirectoryWord __fastcall c2da_manager_004077b0(menu_resources::Manager*,void*,void*);
DirectoryWord __fastcall c2da_file_005e6960(void*,void*,void*,DirectoryWord,DirectoryWord);
void __fastcall c2da_file_005e6980(void*,void*,void*,DirectoryWord,DirectoryWord);
DirectoryWord __fastcall unresolved_list_at_pos_005e9b60(void*,void*,void*);
}
// Potentially throwing constructor: C++ linkage retains the existing /EHsc
// exception transport, while fastcall still supplies ECX and RET12 layout.
void* __fastcall c2da_file_construct_005e68d0(
    void*,void*,DirectoryString*,DirectoryWord,DirectoryString*);

namespace {
template<class T> T directory_read(void* receiver,DirectoryWord offset) {
    return *reinterpret_cast<T volatile*>(static_cast<unsigned char*>(receiver)+offset);
}
template<class T> void directory_write(void* receiver,DirectoryWord offset,T value) {
    *reinterpret_cast<T volatile*>(static_cast<unsigned char*>(receiver)+offset)=value;
}
// Support-only lifetime bookkeeping for genuine Concat output storage. It does
// not construct a new nominal string or duplicate any string algorithm.
struct DirectoryTemporary {
    alignas(DirectoryString) unsigned char storage[sizeof(DirectoryString)];
    bool constructed=false;
    DirectoryString* address() {return reinterpret_cast<DirectoryString*>(storage);}
    ~DirectoryTemporary() {if(constructed)address()->~StartupString();}
};
}

static DirectoryWord __fastcall directory_algorithm_004078f0(
    menu_resources::Manager* manager,void*,void* resource,DirectoryWord mode) {
    DirectoryString path;
    DirectoryString resource_name;
    if(!resource || (directory_read<unsigned char>(resource,12)&4u)!=0 ||
       !directory_read<void*>(resource,20)) return 0;
    void* list=directory_read<void*>(manager,20);
    void* position=directory_read<void*>(list,0);
    if(!position) return 0;
    void* entry=reinterpret_cast<void*>(unresolved_list_at_pos_005e9b60(list,nullptr,position));
    while(position) {
        if(entry) {
            const DirectoryWord key=(directory_read<DirectoryWord>(resource,8)>>14)&0x3fu;
            if(((key^directory_read<DirectoryWord>(entry,40))&0x0fffffffu)==0) break;
        }
        entry=reinterpret_cast<void*>(unresolved_list_next_005e9b90(
            directory_read<void*>(manager,20),nullptr,&position));
    }
    if(!position) return 0;
    c2da_directory_ref_string_00405f70(
        static_cast<menu_resources::Ref*>(directory_read<void*>(resource,20)),nullptr,&resource_name);
    DirectoryString* directory=reinterpret_cast<DirectoryString*>(static_cast<unsigned char*>(entry)+32);
    {
        DirectoryString slash(c2da_directory_literal_0073d8f4);
        DirectoryTemporary directory_slash;
        DirectoryString* first=GuiConcat005e5d10(directory,nullptr,directory_slash.address(),&slash);
        directory_slash.constructed=true;
        DirectoryTemporary directory_file;
        DirectoryString* second=GuiConcat005e5d10(first,nullptr,directory_file.address(),&resource_name);
        directory_file.constructed=true;
        path.Assign005e5c50(second);
    }
    directory->BorrowedEquals005e5290(c2da_directory_literal_0073d8e8);
    void* allocation=c2da_directory_new_006fa7e6(4);
    void* file=nullptr;
    if(allocation) {
        try {
            DirectoryString read_mode(c2da_directory_literal_0073d5b0);
            const DirectoryWord type=directory_read<std::uint16_t>(directory_read<void*>(resource,20),26);
            file=c2da_file_construct_005e68d0(allocation,nullptr,&path,type,&read_mode);
        } catch(...) {live_free(allocation);throw;}
    }
    if(!file) return 0;
    if(c2da_file_005e6a10(file,nullptr)==0) {
        c2da_005e69e0(file,nullptr);live_free(file);return 0;
    }
    const DirectoryWord size=c2da_file_005e6950(file,nullptr);
    directory_write(resource,24,size);
    if(size==0 || c2da_manager_004077b0(manager,nullptr,resource)==0) {
        c2da_005e69e0(file,nullptr);live_free(file);return 0;
    }
    if(mode!=0) {
        directory_write(manager,44,file);
        c2da_file_005e6980(file,nullptr,directory_read<void*>(resource,16),size,1);
        return 1;
    }
    c2da_file_005e6960(file,nullptr,directory_read<void*>(resource,16),size,1);
    c2da_005e69e0(file,nullptr);live_free(file);
    const DirectoryWord serviced=c2da_r29_dispatch::serviced(resource);
    directory_write(resource,12,(directory_read<DirectoryWord>(resource,12)&~4u)|(serviced!=0?4u:0u));
    return serviced;
}

// The single throwing algorithm has C++ linkage under the fixed /EHsc.
// This exact existing Cfastcall transport carries resource/mode and full EAX.
extern "C" DirectoryWord __fastcall c2da_manager_004078f0(
    menu_resources::Manager* manager,void* edx,void* resource,DirectoryWord mode) {
    return directory_algorithm_004078f0(manager,edx,resource,mode);
}

using DirectoryLoaderContract=DirectoryWord(__fastcall*)(menu_resources::Manager*,void*,void*,DirectoryWord);
static_assert(std::is_same<decltype(&c2da_manager_004078f0),DirectoryLoaderContract>::value,
              "Directory ECX Manager, unused EDX, resource/mode stack, DWORD EAX");
static_assert(sizeof(void*)==4 && sizeof(DirectoryWord)==4,"original x86 physical widths");
