#pragma once
// lane-tpc stage header (single owner of ProcessedTexture and of the method
// tables 0075FB64 / 0075FB68 / 0075FC5C, per integrator decision).
// Self-contained: only stage headers "resources.h" (task-36 resource-r1) and
// "allocator_live.h" (task-23 sbh-slice2 live). Type aliases below are
// identical redeclarations of font_callback_read (task-9 read_callback.hpp /
// task-33 contract_types.hpp), so this header may share a TU with
// task-4 processed_raster.hpp.
#include "c53472623c7d6765_resources.h"
#include "20e68b036c30980c_allocator_live.h"   // live_operator_new = 006FA7E6, live_free = 006FA390 (JMP 006FB7B2)
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>

// Identical alias redeclarations of task-33 contract_types.hpp (font_reader_contract).
namespace font_reader_contract {
using Word=std::uint32_t;
using Signed=std::int32_t;
using ReadSource=char* (__cdecl*)(void*,const char*,Signed*);
using ReleaseSource=void (__cdecl*)(void*,Signed);
}
namespace font_callback_read {
using Word=std::uint32_t;
using Signed=std::int32_t;
using String=kotor_reconstruction::StartupString;
// Observed CResHelper<CResTPC,0x0BBF> layout (0070F980 F9A5/F99C/F9BC/F9C8;
// 0070E4F0 E50A operator new(0x1C)). Explicit ABI fields, not a C++ vptr.
struct ProcessedTexture {
    const void* volatile methods;     // +0  0075FB64 (base) / 0075FB68 (derived)
    volatile Word requested;          // +4
    void* volatile resource;          // +8  CResTPC* (0x48 bytes, 00712EA0)
    menu_resources::Ref reference;    // +C  CResRef[16]
};
static_assert(std::is_standard_layout<ProcessedTexture>::value && sizeof(ProcessedTexture)==0x1c &&
    offsetof(ProcessedTexture,requested)==4 && offsetof(ProcessedTexture,resource)==8 &&
    offsetof(ProcessedTexture,reference)==12,"observed CResHelper<CResTPC> ABI");
// The opaque Txi identity of read_callback.hpp, completed with the observed
// CResHelper<CResTXI,0x07E6> layout (0070E5E0 new(0x1C); 0070FD10).
struct Txi {
    const void* volatile methods;     // +0  0075FB6C (base) / 0075FB70 (derived)
    volatile Word requested;          // +4
    void* volatile resource;          // +8  CResTXI* (0x34 bytes, 00710DB0)
    menu_resources::Ref reference;    // +C
};
static_assert(std::is_standard_layout<Txi>::value && sizeof(Txi)==0x1c &&
    offsetof(Txi,resource)==8 && offsetof(Txi,reference)==12,"observed CResHelper<CResTXI> ABI");
extern "C" {
// Same spellings/types as task-4 processed_raster.hpp and Oct4 getters.hpp.
ProcessedTexture* __cdecl aurora_processed_target_0070e4f0(String*) noexcept(false);
Signed __fastcall aurora_get_demands_004064b0(void*,void*) noexcept(false);
char* __fastcall aurora_processed_pointer_00712f90(void*,void*) noexcept(false);
Word __fastcall aurora_txi_size_00712fc0(void*,void*);
Signed __fastcall processed_attrib_lower_00712ef0(void*,void*,Signed*,Signed*,char*,Signed*,Signed*,Signed*,float*,char*) noexcept(false);
}
}

namespace tpc_lower {
using Word=font_callback_read::Word;
using Signed=font_callback_read::Signed;
using Ref=menu_resources::Ref;
using Manager=menu_resources::Manager;
using ProcessedTexture=font_callback_read::ProcessedTexture;
// Original slot0 is thiscall(int flags) RET4; fastcall + unused EDX is the
// project transport (M40 resource_methods.h, decision D1).
using DeletingDestroy=void* (__fastcall*)(void*,void*,Word);
using ConstantWord=Word (__stdcall*)();
using ResourceCallback=Word (__fastcall*)(void*,void*);
// 0075FB64 = [0070F960], 0075FB68 = [0070FAA0]; one cell each.
struct HelperMethods { DeletingDestroy deleting_destroy; };
static_assert(sizeof(HelperMethods)==4,"one-slot table");
// 0075FC5C = [00713170, 00406490, 0063E7F0, 00712FD0, 00712FF0].
struct ResourceMethods {
    DeletingDestroy deleting_destroy_00713170;
    ConstantWord constant_word_00406490;
    ConstantWord constant_word_0063e7f0;
    ResourceCallback freed_00712fd0;
    ResourceCallback serviced_00712ff0;
};
static_assert(sizeof(ResourceMethods)==20 && offsetof(ResourceMethods,serviced_00712ff0)==16,"five cells");
template<class T> inline T load(const void* p,unsigned offset) {
    T value;std::memcpy(&value,static_cast<const unsigned char*>(p)+offset,sizeof(value));return value;
}
template<class T> inline void store(void* p,unsigned offset,T value) {
    std::memcpy(static_cast<unsigned char*>(p)+offset,&value,sizeof(value));
}
// `mov reg,[obj]; push 1; mov ecx,obj; call [reg]`; bits read via memcpy.
inline void* call_slot0(void* object,Word flags) {
    const void* methods=load<const void*>(object,0);
    DeletingDestroy slot;std::memcpy(&slot,methods,sizeof(slot));
    return slot(object,nullptr,flags);
}
using font_callback_read::aurora_get_demands_004064b0;
using font_callback_read::aurora_processed_pointer_00712f90;
using font_callback_read::aurora_txi_size_00712fc0;
using font_callback_read::processed_attrib_lower_00712ef0;
// (x+3)/4 with C signed division, as emitted at 0070F522-0070F53B etc.
inline Signed blocks4(Signed x) {return (x+3)/4;}
}

extern "C" {
// ---- this lane ------------------------------------------------------------
font_callback_read::ProcessedTexture* __fastcall aurora_processed_construct_0070f980(
    font_callback_read::ProcessedTexture*,void*,menu_resources::Ref,font_callback_read::Signed) noexcept(false);
font_callback_read::Signed __fastcall aurora_processed_load_0070f440(font_callback_read::ProcessedTexture*,void*) noexcept(false);
void __fastcall aurora_processed_set_ref_0070f800(font_callback_read::ProcessedTexture*,void*,
    menu_resources::Ref*,font_callback_read::Signed) noexcept(false);
void __fastcall aurora_processed_base_destroy_0070f7a0(font_callback_read::ProcessedTexture*,void*) noexcept;
void __fastcall aurora_processed_destroy_0070fa10(font_callback_read::ProcessedTexture*,void*) noexcept;
void* __fastcall aurora_processed_base_deleting_destroy_0070f960(void*,void*,tpc_lower::Word) noexcept;
void* __fastcall aurora_processed_deleting_destroy_0070faa0(void*,void*,tpc_lower::Word) noexcept;
void* __fastcall aurora_processed_info_0070fa20(void*,void*);
tpc_lower::Word __fastcall aurora_processed_size_0070f690(void*,void*);
extern const tpc_lower::HelperMethods aurora_processed_helper_methods_0075fb64;
extern const tpc_lower::HelperMethods aurora_processed_methods_0075fb68;
void* __fastcall aurora_tpc_resource_ctor_00712ea0(void*,void*);
void __fastcall aurora_tpc_resource_destroy_00712ee0(void*,void*) noexcept;
void* __fastcall aurora_tpc_resource_deleting_destroy_00713170(void*,void*,tpc_lower::Word) noexcept;
tpc_lower::Word __fastcall aurora_tpc_resource_freed_00712fd0(void*,void*);
tpc_lower::Word __fastcall aurora_tpc_resource_serviced_00712ff0(void*,void*);
extern const tpc_lower::ResourceMethods aurora_tpc_resource_methods_0075fc5c;
// ---- TXI / Aurora providers (this lane) ------------------------------------
void* __fastcall aurora_helper_info_0070faf0(void*,void*);
void __fastcall aurora_unload_00710100(void*,void*);
char* __fastcall aurora_txi_pointer_00712fa0(void*,void*);
tpc_lower::Word __fastcall aurora_processed_size_00712fb0(void*,void*);
void* __fastcall c2da_current_resource_data_00404dd0(void*,void*);
tpc_lower::Word __fastcall c2da_current_resource_size_00710e50(void*,void*);
// ---- stage-provided (M40/M41 strict-link objects) ------------------------
menu_resources::Ref* __fastcall async_ref_construct_string_00406d60(menu_resources::Ref*,void*,menu_resources::String*);
menu_resources::Ref* __fastcall unresolved_ref_assign_00406290(menu_resources::Ref*,void*,menu_resources::Ref*,const char*);
int __fastcall resource_equal_004060b0(menu_resources::Ref*,void*,menu_resources::Ref*);
int __fastcall c2da_equal_text_004060d0(menu_resources::Ref*,void*,const char*);
const char* __cdecl c2da_empty_0073d71c();
menu_resources::Manager* __cdecl c2da_manager_007a39e8();
void* __fastcall c2da_resource_base_ctor_00406460(void*,void*);
void __fastcall c2da_resource_base_destroy_00409d40(void*,void*);
tpc_lower::Word __stdcall c2da_resource_constant_word_00406490();
tpc_lower::Word __stdcall c2da_resource_constant_word_0063e7f0();
void __fastcall c2da_manager_cancel_004088f0(menu_resources::Manager*,void*,void*);
std::uint32_t __fastcall c2da_manager_demand_004089f0(menu_resources::Manager*,void*,void*);
void __fastcall c2da_manager_release_demand_00408c90(menu_resources::Manager*,void*,void*);
void __fastcall c2da_resource_serviced_00409b40(void*,void*,std::uint32_t);
// ---- lane-m42-menu-joins (single owner of 00405ED0) -----------------------
menu_resources::Ref* __fastcall c2da_default_ref_00405ed0(menu_resources::Ref*,void*);
// ---- donor lowers staged by this lane ------------------------------------
int __fastcall c2da_not_equal_text_00406060(menu_resources::Ref*,void*,const char*);
void __fastcall c2da_cancel_00409b00(void*,void*);
std::uint32_t __fastcall c2da_current_resource_demand_00409b10(void*,void*);
void __fastcall c2da_release_demand_00409b80(void*,void*);
int __fastcall c2da_release_00409cf0(menu_resources::Manager*,void*,void*);
void* __fastcall c2da_lookup_004074d0(menu_resources::Manager*,void*,menu_resources::Ref*,menu_resources::Word);
void __fastcall c2da_register_00407680(menu_resources::Manager*,void*,menu_resources::Ref*,menu_resources::Word,void*);
void __fastcall c2da_request_00408620(void*,void*);
menu_resources::Ref* __fastcall c2da_copy_00406120(menu_resources::Ref*,void*,menu_resources::Ref*,menu_resources::Ref*);
}
