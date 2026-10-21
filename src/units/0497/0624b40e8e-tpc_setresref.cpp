#include "a6c0baad11-tpc.hpp"
// 0070F800 CResHelper<CResTPC,0x0BBF>::SetResRef (complete, ECX this, two
// stack DWORDs, RET 8). Same template algorithm as the existing C2DA donor
// (Oct2 task-10 c2da-resource-binding-00413b40-r1 binding.cpp), instantiated
// with type 0x0BBF, a 0x48-byte CResTPC and constructor 00712EA0. Unlike that
// donor it also preserves the original allocation unwind (FuncInfo 0078A790,
// funclet 0073A9D0 -> 006FA390) around the CResTPC constructor.
using namespace tpc_lower;
using font_callback_read::ProcessedTexture;

namespace {
struct ResourceAllocation {
    void* raw;
    ~ResourceAllocation() noexcept {if(raw)live_free(raw);}
};
}

extern "C" __declspec(noinline) void __fastcall aurora_processed_set_ref_0070f800(
    ProcessedTexture* self,void*,menu_resources::Ref* incoming,Signed flag) noexcept(false) {
    if(resource_equal_004060b0(incoming,nullptr,&self->reference))return;   // F824-F832
    if(self->resource &&                                                     // F838-F83D
       c2da_not_equal_text_00406060(&self->reference,nullptr,c2da_empty_0073d71c())) { // F83F-F84D
        if(self->requested)c2da_cancel_00409b00(self->resource,nullptr);    // F84F-F859 (flag not cleared)
        if(!c2da_release_00409cf0(c2da_manager_007a39e8(),nullptr,self->resource)) { // F85E-F86F
            if(void* resource=self->resource)call_slot0(resource,1u);       // F871-F87C
            self->resource=nullptr;                                          // F87E
        }
    }
    if(c2da_equal_text_004060d0(incoming,nullptr,c2da_empty_0073d71c())) {  // F885-F893
        self->resource=nullptr;                                              // F8A1
        menu_resources::Ref discarded;
        (void)unresolved_ref_assign_00406290(&self->reference,nullptr,&discarded,c2da_empty_0073d71c()); // F895-F8A8
        return;
    }
    void* selected=c2da_lookup_004074d0(c2da_manager_007a39e8(),nullptr,incoming,0x0bbfu); // F8C1-F8CD
    if(!selected) {                                                          // F8D4
        void* const raw=live_operator_new(0x48);                             // F8D8-F8DA 006FA7E6
        {
            ResourceAllocation guard{raw};                                   // state 0 at F8E8
            selected=raw?aurora_tpc_resource_ctor_00712ea0(raw,nullptr):nullptr; // F8E6-F8F9
            guard.raw=nullptr;                                               // state -1 at F908
        }
        c2da_register_00407680(c2da_manager_007a39e8(),nullptr,incoming,0x0bbfu,selected); // F8FB-F910
        if(!selected)return;                                                 // F915-F917
    }
    self->resource=selected;                                                 // F91F
    if(flag) {                                                               // F919-F922
        self->requested=1;                                                   // F926
        c2da_request_00408620(selected,nullptr);                             // F924-F92D
    }
    menu_resources::Ref discarded;
    (void)c2da_copy_00406120(&self->reference,nullptr,&discarded,incoming);  // F932-F93C
}
