#include "20bf4c9f83-next_step.h"
namespace list_accessor_adoption {
void* __cdecl NextStep005e9b90(Node* current,void* slot,PublishCursor publish) {
    using namespace base_ini_reconstruction;
    if(!current) {
        warning_005e9400(static_cast<kotor_menu20::Debug*>(current_root_base()->debug),
                        nullptr,255,diagnostic_source_0074e208,diagnostic_position_0074e2a4);
        return nullptr;
    }
    Node* next=current->next;



    publish(slot,next); // exact original store completes before new payload read



    return next?next->data:nullptr;

}
}
