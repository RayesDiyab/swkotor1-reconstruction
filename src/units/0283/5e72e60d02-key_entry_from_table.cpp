#include "f5954e6ac4-key_entry_from_table.h"
#include <string.h>
namespace {
uint32_t word(const void* p,unsigned offset) {
    uint32_t v;memcpy(&v,static_cast<const unsigned char*>(p)+offset,4);return v;
}
void* pointer(const void* p,unsigned offset) {
    return reinterpret_cast<void*>(word(p,offset));
}
}
extern "C" __declspec(noinline) uint32_t __fastcall unresolved_key_entry_from_table_004071a0(
    menu_resources::Manager*,void*,void* member,menu_resources::Ref* resource,
    uint32_t type,uint32_t* output1,uint32_t* output2,uint32_t kind) {
    void* internal=pointer(member,0);
    void* position=pointer(internal,0);
    if(!position)return 0;
    uint32_t table=unresolved_list_at_pos_005e9b60(internal,nullptr,position);
    while(position) {
        void* current=reinterpret_cast<void*>(table);
        // Original reads+8 before its later null check when kind is nonzero.
#ifdef WRONG_IGNORE_KIND
        (void)kind;
        const bool kind_matches=true;
#else
        const bool kind_matches=!kind||word(current,8)==kind;
#endif
        if(kind_matches&&current) {
#ifdef WRONG_IGNORE_ACTIVE
            const bool available=true;
#else
            const bool available=word(current,4)==0;
#endif
            if(available) {
#ifdef WRONG_TRUNCATE_TYPE
                const uint32_t result=unresolved_find_key_0040ec50(current,nullptr,resource,static_cast<uint16_t>(type));
#else
                const uint32_t result=unresolved_find_key_0040ec50(current,nullptr,resource,type);
#endif
                if(result) {
#ifdef WRONG_SWAP_OUTPUTS
                    *output1=result;*output2=table;
#else
                    *output1=table;*output2=result;
#endif
#ifdef WRONG_RETURN_POINTER
                    return result;
#else
                    return 1;
#endif
                }
            }
        }
#ifdef WRONG_RETAIN_INITIAL_INTERNAL
        table=unresolved_list_next_005e9b90(internal,nullptr,&position);
#else
        // Original re-reads the wrapper's internal pointer after each miss.
        table=unresolved_list_next_005e9b90(pointer(member,0),nullptr,&position);
#endif
    }
#ifdef WRONG_CLEAR_FAILURE
    *output1=0;*output2=0;
#endif
    return 0;
}
