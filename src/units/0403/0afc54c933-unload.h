#pragma once
#include "868f97a4f5-resources.h"
#include "fe6ad04615-joins.h"
#include "c75b097fab-native_strings.h"
#include "4544498f05-base_ini.h"
#include "77c58f98ec-native_string_deps.h"
#include <cstddef>
namespace key_unload {
using Word=menu_resources::Word;
struct QueryLocals {Word result2,index,result1;};
static_assert(sizeof(QueryLocals)==12&&offsetof(QueryLocals,index)==4&&offsetof(QueryLocals,result1)==8,"actual output words eight bytes apart, loop index between");
extern const char destroy_format_0073e0d8[];
}
// Existing Task10 service owner, same actual opaque CRes allocation and RET4.
extern "C" void __fastcall c2da_resource_serviced_00409b40(void*,void*,menu_resources::Word);
// Same opaque key allocation returned by Main's sole0040D030 constructor.
extern "C" __declspec(noinline) void __fastcall async_unload_key_0040d2e0(void*,void*);
