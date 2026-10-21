#include "a6c0baad11-tpc.hpp"
// CResHelper<CResTXI,0x07E6> ("Txi") lifetime and CResTXI (0x34 bytes):
// tail 0070EA90, getter 0070E5E0, ctor 0070FD10, Load 0070FAC0, metadata
// 0070FAF0, base dtor 0070FB30, SetResRef 0070FB90, base scalar 0070FCF0,
// derived dtor 0070FDA0, derived scalar 0070FDB0, tables 0075FB6C/0075FB70;
// CResTXI ctor 00710DB0, dtor 00710DE0, scalar 00710DF0, freed 00710E60,
// generic serviced 005D4C60, table 0075FBBC. Same template as the TPC chain
// (tpc_helper.cpp / tpc_setresref.cpp); evidence: lane evidence/0x70E5E0.txt,
// 0x70FAC0.txt, 0x710DB0.txt, 0x5D4C60.txt; tables read from .rdata.
using namespace tpc_lower;
using font_callback_read::String;

using font_callback_read::Txi;   // complete type defined once in tpc.hpp

extern "C" {
Txi* __cdecl aurora_txi_target_0070e5e0(String*) noexcept(false);
Txi* __fastcall aurora_txi_construct_0070fd10(Txi*,void*,menu_resources::Ref,Signed) noexcept(false);
Signed __fastcall aurora_txi_load_0070fac0(Txi*,void*) noexcept(false);
void __fastcall aurora_txi_base_destroy_0070fb30(Txi*,void*) noexcept;
void __fastcall aurora_txi_set_ref_0070fb90(Txi*,void*,menu_resources::Ref*,Signed) noexcept(false);
void* __fastcall aurora_txi_base_deleting_destroy_0070fcf0(void*,void*,Word) noexcept;
void __fastcall aurora_txi_destroy_0070fda0(Txi*,void*) noexcept;
void* __fastcall aurora_txi_deleting_destroy_0070fdb0(void*,void*,Word) noexcept;
void* __fastcall aurora_txi_resource_ctor_00710db0(void*,void*);
void __fastcall aurora_txi_resource_destroy_00710de0(void*,void*) noexcept;
void* __fastcall aurora_txi_resource_deleting_destroy_00710df0(void*,void*,Word) noexcept;
Word __fastcall aurora_txi_resource_freed_00710e60(void*,void*);
Word __fastcall aurora_generic_resource_serviced_005d4c60(void*,void*);
}

extern "C" const HelperMethods aurora_txi_helper_methods_0075fb6c{&aurora_txi_base_deleting_destroy_0070fcf0};
extern "C" const HelperMethods aurora_txi_methods_0075fb70{&aurora_txi_deleting_destroy_0070fdb0};
extern "C" const ResourceMethods aurora_txi_resource_methods_0075fbbc{
    &aurora_txi_resource_deleting_destroy_00710df0,
    &c2da_resource_constant_word_00406490,
    &c2da_resource_constant_word_0063e7f0,
    &aurora_txi_resource_freed_00710e60,
    &aurora_generic_resource_serviced_005d4c60
};

namespace {
struct BaseCleanup {
    Txi* object;
    ~BaseCleanup() noexcept {if(object)aurora_txi_base_destroy_0070fb30(object,nullptr);}
};
struct Allocation {
    void* raw;
    ~Allocation() noexcept {if(raw)live_free(raw);}
};
}

// 0070EA90 (complete): five-byte JMP 0070E5E0 (cdecl one pointer, caller pops).
extern "C" __declspec(naked) Txi* __cdecl txi_0070ea90(String*) {
    __asm { jmp aurora_txi_target_0070e5e0 }
}

// 00710DB0 (complete): base 00406460, zero +28/+2C/+30, table 0075FBBC, +20=1
// (selects the allocator's size+6 path 004077D1: tag lives 6 bytes before data).
extern "C" __declspec(noinline) void* __fastcall aurora_txi_resource_ctor_00710db0(void* self,void*) {
    (void)c2da_resource_base_ctor_00406460(self,nullptr);              // 0DB3
    store(self,0x28,Word{0});store(self,0x2c,Word{0});store(self,0x30,Word{0}); // 0DBA-0DC0
    store<const void*>(self,0,&aurora_txi_resource_methods_0075fbbc);  // 0DC3
    store(self,0x20,Word{1});                                          // 0DC9
    return self;
}
// 00710DE0 (complete): table store, JMP 00409D40.
extern "C" __declspec(noinline) void __fastcall aurora_txi_resource_destroy_00710de0(void* self,void*) noexcept {
    store<const void*>(self,0,&aurora_txi_resource_methods_0075fbbc);
    c2da_resource_base_destroy_00409d40(self,nullptr);
}
// 00710DF0 (complete): slot0 of 0075FBBC.
extern "C" __declspec(noinline) void* __fastcall aurora_txi_resource_deleting_destroy_00710df0(
    void* self,void*,Word flags) noexcept {
    aurora_txi_resource_destroy_00710de0(self,nullptr);
    if((flags&1u)!=0u)live_free(self);                                 // 0E00 006FA390
    return self;
}
// 00710E60 (complete): zero +28/+2C/+30, return 1.
extern "C" __declspec(noinline) Word __fastcall aurora_txi_resource_freed_00710e60(void* self,void*) {
    store(self,0x28,Word{0});store(self,0x2c,Word{0});store(self,0x30,Word{0});
    return 1u;
}
// 005D4C60 (complete): generic OnResourceServiced. EAX=[+10] when null.
extern "C" __declspec(noinline) Word __fastcall aurora_generic_resource_serviced_005d4c60(void* self,void*) {
    void* const data=load<void*>(self,0x10);                          // 4C60
    if(!data)return 0;                                                 // 4C67 (EAX = 0)
    if(load<Word>(self,0x28)!=0)return 1;                              // 4C68-4C74
    store<void*>(self,0x30,data);                                      // 4C78
    store(self,0x2c,load<Word>(self,0x18));                            // 4C75/4C80
    store(self,0x28,Word{1});                                          // 4C83
    return 1;
}

// 0070FB30 (complete): base destructor, table 0075FB6C (same algorithm as 0070F7A0).
extern "C" __declspec(noinline) void __fastcall aurora_txi_base_destroy_0070fb30(Txi* self,void*) noexcept {
    void* const initial=self->resource;                                // FB33
    self->methods=&aurora_txi_helper_methods_0075fb6c;                 // FB38
    if(!initial)return;                                                // FB3E
    if(!c2da_not_equal_text_00406060(&self->reference,nullptr,c2da_empty_0073d71c()))return; // FB40-FB4F
    if(self->requested) {                                              // FB51
        c2da_cancel_00409b00(self->resource,nullptr);                  // FB58-FB5B
        self->requested=0;                                             // FB60
    }
    if(c2da_release_00409cf0(c2da_manager_007a39e8(),nullptr,self->resource))return; // FB67-FB78
    if(void* resource=self->resource)call_slot0(resource,1u);          // FB7A-FB85
    self->resource=nullptr;                                            // FB87
}
// 0070FDA0 (complete): derived table then JMP 0070FB30.
extern "C" __declspec(noinline) void __fastcall aurora_txi_destroy_0070fda0(Txi* self,void*) noexcept {
    self->methods=&aurora_txi_methods_0075fb70;
    aurora_txi_base_destroy_0070fb30(self,nullptr);
}
// 0070FCF0 (complete): slot0 of 0075FB6C.
extern "C" __declspec(noinline) void* __fastcall aurora_txi_base_deleting_destroy_0070fcf0(void* self,void*,Word flags) noexcept {
    aurora_txi_base_destroy_0070fb30(static_cast<Txi*>(self),nullptr);
    if((flags&1u)!=0u)live_free(self);                                 // FD00
    return self;
}
// 0070FDB0 (complete): slot0 of 0075FB70.
extern "C" __declspec(noinline) void* __fastcall aurora_txi_deleting_destroy_0070fdb0(void* self,void*,Word flags) noexcept {
    aurora_txi_destroy_0070fda0(static_cast<Txi*>(self),nullptr);      // FDB3
    if((flags&1u)!=0u)live_free(self);                                 // FDC0
    return self;
}

// 0070FB90 (complete, RET 8): SetResRef, type 0x07E6, 0x34-byte CResTXI via
// 00710DB0; allocation unwind (handler 0073AA1B) frees through 006FA390.
extern "C" __declspec(noinline) void __fastcall aurora_txi_set_ref_0070fb90(
    Txi* self,void*,menu_resources::Ref* incoming,Signed flag) noexcept(false) {
    if(resource_equal_004060b0(incoming,nullptr,&self->reference))return;   // FBB4-FBC2
    if(self->resource &&                                                     // FBC8-FBCD
       c2da_not_equal_text_00406060(&self->reference,nullptr,c2da_empty_0073d71c())) { // FBCF-FBDD
        if(self->requested)c2da_cancel_00409b00(self->resource,nullptr);    // FBDF-FBE9 (flag kept)
        if(!c2da_release_00409cf0(c2da_manager_007a39e8(),nullptr,self->resource)) { // FBEE-FBFF
            if(void* resource=self->resource)call_slot0(resource,1u);       // FC01-FC0C
            self->resource=nullptr;                                          // FC0E
        }
    }
    if(c2da_equal_text_004060d0(incoming,nullptr,c2da_empty_0073d71c())) {  // FC15-FC23
        self->resource=nullptr;                                              // FC31
        menu_resources::Ref discarded;
        (void)unresolved_ref_assign_00406290(&self->reference,nullptr,&discarded,c2da_empty_0073d71c()); // FC25-FC38
        return;
    }
    void* selected=c2da_lookup_004074d0(c2da_manager_007a39e8(),nullptr,incoming,0x07e6u); // FC51-FC5D
    if(!selected) {
        void* const raw=live_operator_new(0x34);                             // FC68-FC6A
        {
            Allocation guard{raw};                                           // state 0 FC78
            selected=raw?aurora_txi_resource_ctor_00710db0(raw,nullptr):nullptr; // FC76-FC89
            guard.raw=nullptr;                                               // state -1 FC98
        }
        c2da_register_00407680(c2da_manager_007a39e8(),nullptr,incoming,0x07e6u,selected); // FC8B-FCA0
        if(!selected)return;                                                 // FCA5-FCA7
    }
    self->resource=selected;                                                 // FCAF
    if(flag) {
        self->requested=1;                                                   // FCB6
        c2da_request_00408620(selected,nullptr);                             // FCBD
    }
    menu_resources::Ref discarded;
    (void)c2da_copy_00406120(&self->reference,nullptr,&discarded,incoming);  // FCC2-FCCC
}

// 0070FD10 (complete, RET 14h); unwind (handler 0073AA38) -> base destructor 0070FB30.
extern "C" __declspec(noinline) __declspec(safebuffers) Txi* __fastcall aurora_txi_construct_0070fd10(
    Txi* self,void*,menu_resources::Ref incoming,Signed mode) noexcept(false) {
    self->methods=&aurora_txi_helper_methods_0075fb6c;                 // FD35
    (void)c2da_default_ref_00405ed0(&self->reference,nullptr);         // FD3B
    self->resource=nullptr;                                            // FD4C
    menu_resources::Ref discarded;
    (void)unresolved_ref_assign_00406290(&self->reference,nullptr,&discarded,c2da_empty_0073d71c()); // FD40-FD53
    self->requested=0;                                                 // FD58
    BaseCleanup cleanup{self};                                         // state 0 FD6B
    self->methods=&aurora_txi_methods_0075fb70;                        // FD73
    aurora_txi_set_ref_0070fb90(self,nullptr,&incoming,mode);          // FD64/FD79 (&incoming arg slot)
    cleanup.object=nullptr;
    return self;
}

// 0070FAC0 (complete, RET0): Demand; WORD 07E6 + owner DWORD at data-6.
extern "C" __declspec(noinline) Signed __fastcall aurora_txi_load_0070fac0(Txi* self,void*) noexcept(false) {
    if(!c2da_current_resource_demand_00409b10(self->resource,nullptr))return 0; // FAC3-FAD0
    unsigned char* const tag=static_cast<unsigned char*>(c2da_current_resource_data_00404dd0(self->resource,nullptr))-6; // FAD1-FAD9
    store<Txi*>(tag,2,self);                                           // FADC
    store<std::uint16_t>(tag,0,0x07e6);                                // FADF
    return 1;
}

// 0070FAF0 (complete): data-6, or 0, or literal -6 when the second demand check
// fails (FB22-FB24). Same algorithm as the MDL-recovery naked donor
// metadata.cpp; Main must select exactly one definition.
extern "C" __declspec(noinline) void* __fastcall aurora_helper_info_0070faf0(void* raw_self,void*) {
    void* resource=load<void*>(raw_self,8);                            // FAF3
    if(!resource||aurora_get_demands_004064b0(resource,nullptr)<1)return nullptr; // FAF6-FB02, FB2B
    resource=load<void*>(raw_self,8);                                  // FB04
    if(!resource||aurora_get_demands_004064b0(resource,nullptr)<1)     // FB07-FB13
        return reinterpret_cast<void*>(static_cast<std::uintptr_t>(0xfffffffau));
    const Word data=static_cast<Word>(reinterpret_cast<std::uintptr_t>(
        c2da_current_resource_data_00404dd0(load<void*>(raw_self,8),nullptr))); // FB15-FB18
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(data-6u)); // FB1D
}

// 0070E5E0 (complete, cdecl one pointer): allocating TXI getter, tag 0x07E6.
// Outer unwind shares handler 0073A93B / funclet 0073A930 (006FA390).
extern "C" __declspec(noinline) Txi* __cdecl aurora_txi_target_0070e5e0(String* name) noexcept(false) {
    Txi* object=nullptr;
    void* const raw=live_operator_new(0x1c);                           // E5FA-E5FC
    if(raw) {
        Allocation guard{raw};                                         // state 0 E60C
        menu_resources::Ref outgoing;
        (void)async_ref_construct_string_00406d60(&outgoing,nullptr,name); // E616-E626
        object=aurora_txi_construct_0070fd10(static_cast<Txi*>(raw),nullptr,outgoing,0); // E62B-E62D
        guard.raw=nullptr;                                             // state -1 E63A
    }
    if(!object)return nullptr;                                         // E638-E642
    if(void* resource=object->resource) {                              // E644-E649
        if(aurora_get_demands_004064b0(resource,nullptr)>0) {          // E64B-E652
            const void* info=aurora_helper_info_0070faf0(object,nullptr); // E654-E656
            if(info&&load<std::uint16_t>(info,0)==0x07e6) {            // E65B-E664
                void* const owner=load<void*>(info,2);                 // E666
                if(owner) {
                    call_slot0(object,1u);                             // E66D-E673
                    object=static_cast<Txi*>(owner);                   // E675
                }
            }
        }
    }
    if(!object)return nullptr;                                         // E677-E679
    if(aurora_txi_load_0070fac0(object,nullptr))return object;         // E67B-E684, E6B2
    void* const resource=object->resource;                             // E686
    if(resource&&aurora_get_demands_004064b0(resource,nullptr)>=1)return nullptr; // E689-E695
    call_slot0(object,1u);                                             // E697-E69D
    return nullptr;
}
