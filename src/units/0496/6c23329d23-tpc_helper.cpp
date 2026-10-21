#include "a6c0baad11-tpc.hpp"
// CResHelper<CResTPC,0x0BBF> lifetime: getter 0070E4F0, constructor 0070F980,
// Load 0070F440, base destructor 0070F7A0, derived destructor 0070FA10,
// scalar deleting destructors 0070F960 (base) / 0070FAA0 (derived), and the
// one-slot tables 0075FB64 / 0075FB68. Allocation family: 006FA7E6 operator
// new (-> _nh_malloc(size,1)) = M40 live_operator_new; 006FA390 (JMP 006FB7B2
// free) = M40 live_free. No CRT/global new/delete, no MDL cleanup reused.
using namespace tpc_lower;
using font_callback_read::ProcessedTexture;
using font_callback_read::String;

extern "C" const HelperMethods aurora_processed_helper_methods_0075fb64{
    &aurora_processed_base_deleting_destroy_0070f960};
extern "C" const HelperMethods aurora_processed_methods_0075fb68{
    &aurora_processed_deleting_destroy_0070faa0};

namespace {
// Constructor FuncInfo 0078A7B4: one state; unwind funclet 0073A9F0 loads the
// saved this ([ebp-20h]) and JMPs base destructor 0070F7A0.
struct BaseCleanup {
    ProcessedTexture* object;
    ~BaseCleanup() noexcept {if(object)aurora_processed_base_destroy_0070f7a0(object,nullptr);}
};
// Getter FuncInfo 0078A5E0: one state; funclet 0073A930 pushes the saved raw
// allocation ([ebp-14h]) and calls 006FA390.
struct OuterAllocation {
    void* raw;
    ~OuterAllocation() noexcept {if(raw)live_free(raw);}
};
}

// 0070F7A0 (complete, RET0). Shares the CResHelper template algorithm with
// MDL 00710120 but owns its own base table 0075FB64 and is a distinct body.
extern "C" __declspec(noinline) void __fastcall aurora_processed_base_destroy_0070f7a0(
    ProcessedTexture* self,void*) noexcept {
    void* const initial=self->resource;                       // F7A3
    self->methods=&aurora_processed_helper_methods_0075fb64;  // F7A8
    if(!initial)return;                                       // F7AE
    if(!c2da_not_equal_text_00406060(&self->reference,nullptr,c2da_empty_0073d71c()))return; // F7B0-F7BF
    if(self->requested) {                                     // F7C1
        c2da_cancel_00409b00(self->resource,nullptr);         // F7C8-F7CB
        self->requested=0;                                    // F7D0
    }
    if(c2da_release_00409cf0(c2da_manager_007a39e8(),nullptr,self->resource))return; // F7D7-F7E8
    if(void* resource=self->resource)call_slot0(resource,1u); // F7EA-F7F5
    self->resource=nullptr;                                   // F7F7
}

// 0070FA10 (complete): derived table store then tail JMP 0070F7A0.
extern "C" __declspec(noinline) void __fastcall aurora_processed_destroy_0070fa10(
    ProcessedTexture* self,void*) noexcept {
    self->methods=&aurora_processed_methods_0075fb68;
    aurora_processed_base_destroy_0070f7a0(self,nullptr);
}

// 0070F960 (complete): slot0 of 0075FB64.
extern "C" __declspec(noinline) void* __fastcall aurora_processed_base_deleting_destroy_0070f960(
    void* self,void*,Word flags) noexcept {
    aurora_processed_base_destroy_0070f7a0(static_cast<ProcessedTexture*>(self),nullptr);
    if((flags&1u)!=0u)live_free(self);                        // F968-F975
    return self;
}

// 0070FAA0 (complete): slot0 of 0075FB68.
extern "C" __declspec(noinline) void* __fastcall aurora_processed_deleting_destroy_0070faa0(
    void* self,void*,Word flags) noexcept {
    aurora_processed_destroy_0070fa10(static_cast<ProcessedTexture*>(self),nullptr); // FAA3
    if((flags&1u)!=0u)live_free(self);                        // FAA8-FAB5
    return self;                                              // FAB8, RET4
}

// 0070F980 (complete, RET 14h). ECX this, 16-byte Ref by value, signed mode.
// SetResRef receives the address of the incoming by-value Ref (F9D4 lea
// [esp+30h] = arg slot), so the parameter is used in place, never copied.
// safebuffers: /GS otherwise copies this array-bearing by-value parameter to a
// protected local, and SetResRef would see the copy's address, not the slot.
extern "C" __declspec(noinline) __declspec(safebuffers) ProcessedTexture* __fastcall aurora_processed_construct_0070f980(
    ProcessedTexture* self,void*,menu_resources::Ref incoming,Signed mode) noexcept(false) {
    self->methods=&aurora_processed_helper_methods_0075fb64;  // F9A5
    (void)c2da_default_ref_00405ed0(&self->reference,nullptr);// F9AB
    self->resource=nullptr;                                   // F9BC
    menu_resources::Ref discarded;                            // 16-byte temp [esp+10h]
    (void)unresolved_ref_assign_00406290(&self->reference,nullptr,&discarded,c2da_empty_0073d71c()); // F9B0-F9C3
    self->requested=0;                                        // F9C8
    BaseCleanup cleanup{self};                                // state 0 at F9DB
    self->methods=&aurora_processed_methods_0075fb68;         // F9E3
    aurora_processed_set_ref_0070f800(self,nullptr,&incoming,mode); // F9E9
    cleanup.object=nullptr;
    return self;                                              // F9F3
}

// 0070F440 (complete, RET0). Writes the 0x0BBF owner tag after the selected
// payload: data + size + TXI bytes + 4 (word tag, then owner DWORD at +2).
extern "C" __declspec(noinline) Signed __fastcall aurora_processed_load_0070f440(
    ProcessedTexture* self,void*) noexcept(false) {
    Signed faces=1;                                           // F44D [E-24h]
    if(!c2da_current_resource_demand_00409b10(self->resource,nullptr))return 0; // F446-F45C, no release
    // Decision D2: the original leaves these stack outputs unwritten when
    // 00712EF0 returns early (null header, 2EF7); they are value-initialized
    // here as a documented deviation (original slots E-1Ch..E-29h).
    Signed width=0,height=0,compressed=0,image=0,txi=0;float alpha=0.0f;char encoding=0,mips=0;
    (void)processed_attrib_lower_00712ef0(self->resource,nullptr,&width,&height,&encoding,
        &compressed,&image,&txi,&alpha,&mips);                // F45E-F489, EAX ignored
    char* data=nullptr;
    if(encoding==1||encoding==3||encoding==4)                // F48E-F49C
        data=aurora_processed_pointer_00712f90(self->resource,nullptr); // F49E-F4A8
    if(!data) {
        c2da_release_demand_00409b80(self->resource,nullptr); // F4AE-F4B1
        return 0;
    }
    Word size;
    if(compressed!=0) {                                       // F4BE-F4C4
        // Decision D2: original reads stack slot [E-28h], written only for
        // encodings 3 (8, F4D2) and 4 (16, F4E0). Encoding 1 with compressed
        // bytes is an indeterminate original read; 0 is used (deviation).
        Word block=0;
        if(encoding==3)block=8;else if(encoding==4)block=16;  // F4CA-F4E0
        Signed h=height,w=width;
        const Signed ratio=h/w;                               // F4E8-F4F5 signed idiv
        if(ratio==6) {faces=ratio;h=h/6;}                     // F4F7-F50C
        size=static_cast<Word>(compressed);
        for(Signed n=static_cast<Signed>(static_cast<signed char>(mips))-1;n>0;--n) { // F50E-F549
            h>>=1;w>>=1;                                      // F520/F52D SAR
            size+=static_cast<Word>(blocks4(h))*static_cast<Word>(blocks4(w))*block; // IMUL low bits
        }
        if(faces==6)size*=6u;                                 // F54F-F55B
    } else {
        size=static_cast<Word>(image);                        // F55F
    }
    // F563-F577: TXI size re-queried from the live resource cell.
    char* tag=data+(aurora_txi_size_00712fc0(self->resource,nullptr)+size+4u);
    store<ProcessedTexture*>(tag,2,self);                     // F573 owner DWORD
    store<std::uint16_t>(tag,0,0x0bbf);                       // F577 tag WORD
    return 1;
}

// 0070E4F0 (complete, cdecl one pointer, full EAX). Allocating getter.
extern "C" __declspec(noinline) ProcessedTexture* __cdecl aurora_processed_target_0070e4f0(
    String* name) noexcept(false) {
    ProcessedTexture* object=nullptr;
    void* const raw=live_operator_new(0x1c);                  // E50A-E50C 006FA7E6
    if(raw) {                                                 // E51A
        OuterAllocation guard{raw};                           // state 0 at E51C
        menu_resources::Ref outgoing;                         // outgoing by-value Ref
        (void)async_ref_construct_string_00406d60(&outgoing,nullptr,name); // E52A-E536
        object=aurora_processed_construct_0070f980(static_cast<ProcessedTexture*>(raw),nullptr,outgoing,0); // E53B-E53D
        guard.raw=nullptr;                                    // state -1 at E54A
    }
    if(!object)return nullptr;                                // E548-E552
    if(void* resource=object->resource) {                     // E554-E559
        if(aurora_get_demands_004064b0(resource,nullptr)>0) { // E55B-E562 (JLE)
            const void* info=aurora_processed_info_0070fa20(object,nullptr); // E564-E566
            if(info&&load<std::uint16_t>(info,0)==0x0bbf) {   // E56B-E574
                void* const owner=load<void*>(info,2);        // E576
                if(owner) {                                   // E579
                    call_slot0(object,1u);                    // E57D-E583 delete temporary
                    object=static_cast<ProcessedTexture*>(owner); // E585
                }
            }
        }
    }
    if(!object)return nullptr;                                // E587-E589
    if(aurora_processed_load_0070f440(object,nullptr))return object; // E58B-E594, E5C2
    void* const resource=object->resource;                    // E596
    if(resource&&aurora_get_demands_004064b0(resource,nullptr)>=1)return nullptr; // E59B-E5A5
    call_slot0(object,1u);                                    // E5A7-E5AD
    return nullptr;                                           // E5AF
}
