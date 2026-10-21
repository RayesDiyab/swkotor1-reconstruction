#include "77d07bad29444d41_stack_cleanup.h"
namespace kotor::vm_stack_cleanup {
namespace {
using kotor::vm_leaf::Internal;
Internal& state(void* p) { return *static_cast<Internal*>(p); }
I32 depth(void* p) { return state(p).depth; }
void set_depth(void* p,I32 n) { state(p).depth=n; }
std::uint8_t tag(void* p,I32 i) { return state(p).tags[i]; }
U32 word(void* p,I32 i) { return state(p).words[i]; }
void* implementer(void* p) { return state(p).stack_owner->command_implementer; }
}
View view(kotor::vm_leaf::Internal& s) noexcept {
    return {&s,depth,set_depth,tag,word,implementer};
}
void set_stack_pointer_005d21f0(const View& stack,I32 requested,const Dependencies& d) {
    const I32 old_depth=stack.depth(stack.context);
    if(requested>=old_depth) return; // signed; a request to grow changes nothing
    for(I32 index=old_depth-1;index>=requested;--index) {
        const U32 byte=stack.tag(stack.context,index);
        const I32 type=byte<128u ? static_cast<I32>(byte) : static_cast<I32>(byte)-256;
        if(type==5) {
            const U32 handle=stack.word(stack.context,index);
            if(handle!=0) {
                d.destroy_string_005e5c20(d.context,handle);
                d.free_006fa390(d.context,handle); // original cached handle
            }
        } else if(type>=16 && type<=25) {
            const U32 handle=stack.word(stack.context,index);
            void* const owner_implementer=stack.implementer(stack.context);
            d.destroy_engine_slot6(d.context,owner_implementer,type-16,handle);
        }
    }
    stack.set_depth(stack.context,requested);
}
}
