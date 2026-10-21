#include "e15fa4ba01-binding.h"
#include "7b8c81fcff-allocator_live.h"
#include <cstring>
using namespace c2da_binding;
void Helper::SetResRef(Ref* incoming,Word flag) {
    if(c2da_equal_ref_004060b0(incoming,nullptr,&reference))return;
    if(resource && c2da_not_equal_text_00406060(&reference,nullptr,c2da_empty_0073d71c())) {
        if(requested)c2da_cancel_00409b00(resource,nullptr);
        if(!c2da_release_00409cf0(c2da_manager_007a39e8(),nullptr,resource)) {
            void* current=resource;
            if(current) {
                using Delete=void(__thiscall*)(void*,Word);
                const unsigned char* resource_methods=nullptr;std::memcpy(&resource_methods,current,4);
                Delete deleting=nullptr;std::memcpy(&deleting,resource_methods,4);
                deleting(current,1);
            }
            resource=nullptr;
        }
    }
    if(c2da_equal_text_004060d0(incoming,nullptr,c2da_empty_0073d71c())) {
        resource=nullptr;
        Ref result;
        unresolved_ref_assign_00406290(&reference,nullptr,&result,c2da_empty_0073d71c());
        return;
    }
    void* selected=c2da_lookup_004074d0(c2da_manager_007a39e8(),nullptr,incoming,0x7e1);
    if(!selected) {
        void* allocation=live_operator_new(0x3c);
        selected=allocation?c2da_resource_ctor_0041d730(allocation,nullptr):nullptr;



        c2da_register_00407680(c2da_manager_007a39e8(),nullptr,incoming,0x7e1,selected);
        if(!selected)return;
    }
    resource=selected;
    if(flag) {

        requested=1;

        c2da_request_00408620(selected,nullptr);
    }
    Ref result;
    c2da_copy_00406120(&reference,nullptr,&result,incoming);
}
