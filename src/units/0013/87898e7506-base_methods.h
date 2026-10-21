#pragma once
#include "71329cf9aa-resource_methods.h"

// Source projection of the five observed cells at0073D86C. Its two callback
// cells point to a genuine no-argument full-EAX leaf, not ECX-taking functions.
namespace cres_base_methods {
using Word=c2da_resource_methods::Word;
using DeletingDestroy=c2da_resource_methods::DeletingDestroy;
using ConstantWord=c2da_resource_methods::ConstantWord;
struct Methods0073d86c {
    DeletingDestroy deleting_destroy_00409d70;
    ConstantWord constant_word_00406490;
    ConstantWord constant_word_0063e7f0;
    ConstantWord freed_0063e7a0;
    ConstantWord serviced_0063e7a0;
};
static_assert(std::is_standard_layout<Methods0073d86c>::value,"table offsets");
static_assert(sizeof(Methods0073d86c)==20 && alignof(Methods0073d86c)==4,"five x86 cells");
static_assert(offsetof(Methods0073d86c,deleting_destroy_00409d70)==0,"slot0");
static_assert(offsetof(Methods0073d86c,constant_word_00406490)==4,"slot1");
static_assert(offsetof(Methods0073d86c,constant_word_0063e7f0)==8,"slot2");
static_assert(offsetof(Methods0073d86c,freed_0063e7a0)==12,"slot3");
static_assert(offsetof(Methods0073d86c,serviced_0063e7a0)==16,"slot4");
}
extern "C" {
void* __fastcall cres_base_deleting_destroy_00409d70(void* self,void* unused_edx,cres_base_methods::Word flags);
cres_base_methods::Word __stdcall cres_base_constant_word_0063e7a0();
extern const cres_base_methods::Methods0073d86c cres_base_method_table_0073d86c;
const void* __cdecl c2da_resource_base_methods_0073d86c();
}
