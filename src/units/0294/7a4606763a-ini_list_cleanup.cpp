#include "40141ae87f-ini_cleanup.h"
namespace base_ini_reconstruction {
void __fastcall destroy_list_005e9ab0(List* list,void*) {
    Node* current=static_cast<Node*>(list->head);
    while(current) {
        Node* saved=current;
        current=current->next; // original caches next before the free call
        live_free(saved);
    }
    // Original leaves all three receiver fields unchanged and frees no payload.
}
void* __fastcall remove_head_005e9c80(List* list,void*) {
    Node* head=static_cast<Node*>(list->head);
    if(!head) {
        warning_005e9400(static_cast<kotor_menu20::Debug*>(current_root_base()->debug),nullptr,357,
                        diagnostic_source_0074e208,diagnostic_empty_0074e360);
        return nullptr;
    }
    Node* next=head->next;
    void* payload=head->data;
    list->head=next;
    if(!next)list->tail=nullptr;
    else next->prev=nullptr;
    live_free(head);
    // 005E9CC7 reloads count AFTER free; preserve the 32-bit decrement.
    list->count=static_cast<std::int32_t>(static_cast<std::uint32_t>(list->count)-1u);
    return payload;
}
}
