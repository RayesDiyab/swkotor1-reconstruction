#include "0afc54c933-unload.h"
#include <cstring>
namespace {
using Word=menu_resources::Word;using Half=std::uint16_t;
template<class T>T Load(const void* p,unsigned offset){T v;std::memcpy(&v,static_cast<const unsigned char*>(p)+offset,sizeof(v));return v;}
template<class T>void Store(void* p,unsigned offset,T v){std::memcpy(static_cast<unsigned char*>(p)+offset,&v,sizeof(v));}
void Flags(void* resource,Word bits){Store(resource,0x0c,Load<Word>(resource,0x0c)|bits);}
}
// Complete0040D2E0 ordinary nonthrowing algorithm; historical FS/EH unclaimed.
extern "C" __declspec(noinline) void __fastcall
async_unload_key_0040d2e0(void* self,void*) {
    const Word initial_count=Load<Word>(self,0x0c);
    key_unload::QueryLocals scratch{0,0,0};
    Store(self,4,Word{1});
    if(!initial_count)return;
    Word offset=0;
    auto entry=[&]()->unsigned char* {return Load<unsigned char*>(self,0x10)+offset;};
    for(;;) {
        unsigned char* current=entry();
        void* resource=Load<void*>(current,0x10);
        if(resource) {
            if((Load<Word>(current,0x14)&0xc0000000u)==0x40000000u) {
                scratch.result2=0;scratch.result1=0;
            } else {
                const Word type=Load<Half>(current,0x1a);
                (void)unresolved_key_entry_00407230(rim_current_manager_007a39e8(),nullptr,
                    reinterpret_cast<menu_resources::Ref*>(current),type,&scratch.result1,&scratch.result2);
            }
            if(!scratch.result2) {
                resource=Load<void*>(entry(),0x10);
                if(Load<Half>(resource,4)>0u) {
                    menu_resources::String name;
                    c2da_ref_to_string_00405f70(reinterpret_cast<menu_resources::Ref*>(entry()),nullptr,&name);
                    char* cstr=WindowCStr005e5670(&name,nullptr);
                    char message[260];
                    // Keep the observed sprintf call even though text is unused.
                    (void)base_ini_reconstruction::bindings.sprintf_006fadb0(
                        message,key_unload::destroy_format_0073e0d8,cstr);
                    Flags(Load<void*>(entry(),0x10),0x200u);
                    // Actual canonical String destruction occurs after flag update.
                } else {
                    c2da_resource_serviced_00409b40(resource,nullptr,0u);
                    Store(Load<void*>(entry(),0x10),0x14,Word{0});
                    Store(Load<void*>(entry(),0x10),8,Word{0xffffffffu});
                }
            } else {
                void* target=reinterpret_cast<void*>(scratch.result2);
                Store(target,0x10,Load<void*>(entry(),0x10));
                target=reinterpret_cast<void*>(scratch.result2);
                void* moved=Load<void*>(target,0x10);
                Store(moved,8,Load<Word>(target,0x14));
                Store(Load<void*>(entry(),0x10),0x14,reinterpret_cast<void*>(scratch.result2));
                Store(entry(),0x10,Word{0});
                moved=Load<void*>(reinterpret_cast<void*>(scratch.result2),0x10);
                if(Load<Half>(moved,4)>0u)Flags(moved,0x100u);
                else c2da_resource_serviced_00409b40(moved,nullptr,0u);
            }
        }
        // Original loads capacity before updating its writable index cell.
        const Word old_index=scratch.index;
        const Word count=Load<Word>(self,0x0c);
        const Word next_index=old_index+1u;
        offset+=28u;scratch.index=next_index;
        if(next_index>=count)break;
    }
}
