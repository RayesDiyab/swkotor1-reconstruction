#include "20bf4c9f83-next_step.h"
namespace {
void __cdecl PublishTyped(void* slot,base_ini_reconstruction::Node* next) {
    // slot originated as Node**; this cast returns to that actual object type.
    *static_cast<base_ini_reconstruction::Node**>(slot)=next;
}
}
namespace base_ini_reconstruction {
void* __fastcall next_005e9b90(List*,void*,Node** position) {
    return list_accessor_adoption::NextStep005e9b90(*position,position,&PublishTyped);
}
}
