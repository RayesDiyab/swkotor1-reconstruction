#include "37fa053dee-list_key_ports.h"
#include <cstring>
namespace {
const char remove_diagnostic_0074e324[] = "CExoLinkedListInternal::Remove() Position paramter is NULL";
}
// Newly recovered complete normal Remove body; no original EAX result claim.
// Node and Internal are the actual selected owners; only the node is freed.
namespace add_key_lower {
void __fastcall Remove005e9c10(List* self,void*,void* position) {
    using namespace base_ini_reconstruction;
    if(!position) {
        warning_005e9400(static_cast<kotor_menu20::Debug*>(current_root_base()->debug),
                        nullptr,307,diagnostic_source_0074e208,remove_diagnostic_0074e324);
        return;
    }
    auto* actual=static_cast<Node*>(position);
    volatile Node* node=actual;
    Node* previous=node->prev;
    Node* next=node->next;
    void* payload=node->data;
    (void)payload; // original full DWORD read before unlink/free; consumed EAX unclaimed
    if(previous)static_cast<volatile Node*>(previous)->next=next;
    if(next)static_cast<volatile Node*>(next)->prev=previous;
    volatile List* list=self;
    if(list->tail==position)list->tail=node->prev;
    if(list->head==position)list->head=node->next;
    actual->~Node(); // trivial support lifetime end; no added original address
    live_free(actual); // selected matching family, same as canonical RemoveHead
    const std::int32_t current=list->count; // original reload after release
    const std::uint32_t bits=static_cast<std::uint32_t>(current)-1u;
    std::int32_t decremented;
    std::memcpy(&decremented,&bits,sizeof(decremented));
    list->count=decremented;
}
}
extern "C" __declspec(noinline) void __fastcall c2da_005e9c10(
    void* list,void*,void* position) {
    add_key_lower::Remove005e9c10(static_cast<base_ini_reconstruction::List*>(list),nullptr,position);
}
