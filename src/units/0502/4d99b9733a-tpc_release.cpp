#include "a6c0baad11-tpc.hpp"
// Aurora release provider 0070CBD0 (cell 007B8FC8, writer 0070D9D1), TPC
// release 0070EA40, TXI release 0070EAA0, unload 00710100, and the guarded
// helper getters 007100A0 / 007107B0 / 0070F3B0 / 0070F3E0 / 0070F410.
// Algorithms match the existing donors (task-33 font-callback-providers-r1
// release.cpp naked 0070CBD0; Oct4 task-9 font-import-joins-r2 aurora.cpp
// naked EA40/EAA0 and getters.cpp); re-expressed in C++ against this stage.
// Evidence: lane evidence/0x70CBC0.txt, 0x70EAA0.txt, 0x7100A0.txt,
// 0x7107B0.txt, 0x70F3B0.txt; task-4 processed-acquisition-r1 0070ea40.txt, 00710100.txt.
using namespace tpc_lower;
using font_callback_read::ProcessedTexture;

extern "C" {
// Names of the existing release epoch (task-33 release.hpp).
void __cdecl borrowed_release_model_0070e0e0(void*);
void __cdecl borrowed_release_processed_texture_0070ea40(void*);
void __cdecl borrowed_release_txi_0070eaa0(void*);
void __cdecl borrowed_release_visibility_0070eb00(void*);
void __cdecl borrowed_release_model_x_0070eb60(void*);
void __cdecl aurora_release_resource_0070cbd0(void*,Signed);
}
static_assert(std::is_same<decltype(&aurora_release_resource_0070cbd0),font_reader_contract::ReleaseSource>::value,
              "007B8FC8 cdecl(void*, signed DWORD)");

// 00710100 (complete, RET0): if demanded, tail JMP Release 00409B80.
extern "C" __declspec(noinline) void __fastcall aurora_unload_00710100(void* self,void*) {
    void* const resource=load<void*>(self,8);                          // 0103
    if(!resource||aurora_get_demands_004064b0(resource,nullptr)<1)return; // 0106-0112
    c2da_release_demand_00409b80(load<void*>(self,8),nullptr);         // 0114-0118
}

namespace {
// Shared release shape of 0070EA40 / 0070EAA0: resource+8 is passed to
// 004064B0 without a null test (EA45/EAA5), exactly as the original.
template<class Info>
void release_tagged(void* self,std::uint16_t type,Info info) {
    if(aurora_get_demands_004064b0(load<void*>(self,8),nullptr)<1)return; // EA45-EA50
    const void* tag=info(self,nullptr);                                // EA52-EA54
    if(!tag)return;                                                    // EA59
    if(load<std::uint16_t>(tag,0)!=type)return;                       // EA5D
    if(load<void*>(tag,2)!=self)return;                                // EA64
    aurora_unload_00710100(self,nullptr);                              // EA69-EA6B
    if(aurora_get_demands_004064b0(load<void*>(self,8),nullptr)>=1)return; // EA70-EA7B
    call_slot0(self,1u);                                               // EA7D-EA83
}
template<class Result,class Tail>
Result guarded(void* self,Tail tail) {
    void* const resource=load<void*>(self,8);
    if(!resource||aurora_get_demands_004064b0(resource,nullptr)<1)return Result{}; // XOR EAX,EAX
    return tail(load<void*>(self,8),nullptr);                          // reload, tail JMP
}
}

// 0070EA40 (complete, cdecl one pointer, RET0).
extern "C" __declspec(noinline) void __cdecl borrowed_release_processed_texture_0070ea40(void* self) {
    release_tagged(self,0x0bbf,aurora_processed_info_0070fa20);
}
// 0070EAA0 (complete, cdecl one pointer, RET0).
extern "C" __declspec(noinline) void __cdecl borrowed_release_txi_0070eaa0(void* self) {
    release_tagged(self,0x07e6,aurora_helper_info_0070faf0);
}

// 0070CBD0 (complete, cdecl(base,size), RET0). size>=0: tag WORD at base-6,
// owner at base-4. size<0 (TPC embedded TXI, size=-1-TXI): tag at
// base-size+3 (= data+TXI+4), owner at +2, accepted for 0BBE..0BBF.
extern "C" __declspec(noinline) void __cdecl aurora_release_resource_0070cbd0(void* base,Signed size) {
    const Word address=static_cast<Word>(reinterpret_cast<std::uintptr_t>(base));
    if(size>=0) {                                                      // CBD4-CBDA (JL)
        const unsigned char* const tag=reinterpret_cast<const unsigned char*>(static_cast<std::uintptr_t>(address-6u)); // CBDC-CBE0
        const Word type=load<std::uint16_t>(tag,0);                    // CBE3
        void* const owner=load<void*>(tag,2);                          // CBE6 (+2)
        if(type>0x0bb9u) {                                             // CBEF
            if(type==0x0bc0u)borrowed_release_model_x_0070eb60(owner); // CC24-CC2F
            return;
        }
        if(type==0x0bb9u){borrowed_release_visibility_0070eb00(owner);return;} // CC18-CC1B
        if(type==0x07d2u){borrowed_release_model_0070e0e0(owner);return;}      // CBF3-CC0F
        if(type==0x07e6u)borrowed_release_txi_0070eaa0(owner);                 // CBFB-CC03
        return;
    }
    const unsigned char* const tag=reinterpret_cast<const unsigned char*>(
        static_cast<std::uintptr_t>(address-static_cast<Word>(size)+3u)); // CC38-CC3E
    const Word type=load<std::uint16_t>(tag,0);                        // CC3A
    if(type<0x0bbeu||type>0x0bbfu)return;                              // CC41-CC4F
    borrowed_release_processed_texture_0070ea40(load<void*>(tag,2));   // CC51-CC55
}

// Guarded helper getters (each: [this+8], demands>=1, reload, tail).
extern "C" __declspec(noinline) Word __fastcall data_size_007100a0(void* self,void*) {
    return guarded<Word>(self,c2da_current_resource_size_00710e50);    // 007100B8 -> 00710E50
}
extern "C" __declspec(noinline) char* __fastcall data_pointer_007107b0(void* self,void*) {
    return static_cast<char*>(guarded<void*>(self,c2da_current_resource_data_00404dd0)); // 007107C8 -> 00404DD0
}
extern "C" __declspec(noinline) Word __fastcall txi_size_raw_0070f410(void* self,void*) {
    return guarded<Word>(self,aurora_txi_size_00712fc0);              // 0070F428 -> 00712FC0
}
extern "C" __declspec(noinline) char* __fastcall txi_pointer_raw_0070f3e0(void* self,void*) {
    return guarded<char*>(self,aurora_txi_pointer_00712fa0);          // 0070F3F8 -> 00712FA0
}
extern "C" __declspec(noinline) Word __fastcall processed_size_0070f3b0(ProcessedTexture* self,void*) {
    return guarded<Word>(self,aurora_processed_size_00712fb0);        // 0070F3C8 -> 00712FB0
}
