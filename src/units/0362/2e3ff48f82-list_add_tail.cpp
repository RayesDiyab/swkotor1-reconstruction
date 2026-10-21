#include "ef7355cd52-ini_section.h"
#include <cstring>
namespace ini_section {
using namespace base_ini_reconstruction;
Node* __fastcall AddTail005e9dc0(List* self,void*,void* payload) {
    void* allocation=bindings.allocate_006fa7e6(12);
    Node* node=static_cast<Node*>(allocation);
    if(node) {
        ::new(node) Node; // actual trivial Node, no value initialization
        volatile Node* fresh=node;
        fresh->data=nullptr;
        fresh->next=nullptr;
        fresh->prev=nullptr;
    }
    volatile List* list=self;
    volatile Node* fresh=node;
    Node* saved_tail=static_cast<Node*>(list->tail);
    fresh->prev=saved_tail;
    fresh->next=nullptr;
    Node* live_tail=static_cast<Node*>(list->tail);
    if(live_tail) static_cast<volatile Node*>(live_tail)->next=node;
    list->tail=node;
    fresh->data=payload;
    if(!list->head) list->head=node;
    // Original INC wraps its dword. Preserve the signed canonical field's bits
    // without signed overflow or an aliasing load through a different type.
    const std::int32_t live_count=list->count;
    const std::uint32_t incremented=static_cast<std::uint32_t>(live_count)+1u;
    std::int32_t result;
    std::memcpy(&result,&incremented,sizeof(result));
    list->count=result;
    return node;
}
}
