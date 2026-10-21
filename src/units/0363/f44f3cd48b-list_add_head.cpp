#include "c1d8cf9ceb-ini_addhead.h"
#include <cstring>
namespace ini_addhead {
using namespace base_ini_reconstruction;
Node* __fastcall AddHead005e9d70(List* self,void*,void* payload) {
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
    Node* saved_head=static_cast<Node*>(list->head);
    fresh->next=saved_head;
    fresh->prev=nullptr;
    Node* live_head=static_cast<Node*>(list->head);
    if(live_head) static_cast<volatile Node*>(live_head)->prev=node;
    list->head=node;
    fresh->data=payload;
    if(!list->tail) list->tail=node;
    // Preserve the original dword INC through the canonical signed field.
    const std::int32_t live_count=list->count;
    const std::uint32_t incremented=static_cast<std::uint32_t>(live_count)+1u;
    std::int32_t result;
    std::memcpy(&result,&incremented,sizeof(result));
    list->count=result;
    return node;
}
}
