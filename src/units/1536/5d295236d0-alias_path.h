#pragma once
#include "158a6c4f90-startup_string.h"
#include <cstdint>
#include <cstddef>
namespace alias_path {
using String=kotor_reconstruction::StartupString;
// Only observed prefix views, not complete canonical alias classes. All String
// lifetimes and linked node storage must already exist; this lookup owns none.
struct NodeView {String key;String path;NodeView* next;NodeView* previous;};
struct InternalView {
 String fallback;
 NodeView* head;
 // Source counterpart of the second range of005E6890; no separate address credit.
 __declspec(noinline) String* __thiscall lookup(String*,std::int32_t);
};
struct AliasListView {
 InternalView* internal;
 __declspec(noinline) String* __thiscall GetAliasPath(String*,std::int32_t);
};
struct Providers {
 int(String::*equals_005e5290)(const char*);
 std::int32_t(String::*length_005e5790)();
 int(String::*prefix_005e5900)(String*,std::int32_t);
 const char* empty_0073d71c;
};
extern Providers providers; // No production installation or fallback here.
static_assert(sizeof(void*)==4 && sizeof(String)==8 && sizeof(AliasListView)==4,"observed x86 prefix sizes");
static_assert(offsetof(NodeView,key)==0 && offsetof(NodeView,path)==8 && offsetof(NodeView,next)==16 && offsetof(InternalView,head)==8,"observed node and internal offsets");
}

static_assert(sizeof(alias_path::NodeView)==24 && offsetof(alias_path::NodeView,previous)==20,"original complete Alias node24 and backlink20");
