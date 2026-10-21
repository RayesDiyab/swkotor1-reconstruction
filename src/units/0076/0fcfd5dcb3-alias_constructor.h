#pragma once
#include "f16170cfb8-alias_path.h"
namespace alias_path {
// Reuse task8's actual declarations unchanged, including existing methods.
static_assert(sizeof(InternalView)==12,"original Alias internal allocation12");
__declspec(noinline) AliasListView* __fastcall ConstructAlias005e6810(AliasListView*,void*);
__declspec(noinline) InternalView* __fastcall ConstructAliasInternal005eb5b0(InternalView*,void*);
}
