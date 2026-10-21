#pragma once
#include "c30aaa70eb-resources.h"
namespace gui_reference {
// One canonical CRT/locale provider, supplied by its owner. No ASCII fallback.
struct Providers {int (__cdecl* lowercase_006fb766)(int);};
Providers& reference_providers();
}
// Exact existing task36 contract: ECX receiver, unused EDX, two stack words,
// EAX output-reference pointer and RET8. This is the only new definition.
extern "C" menu_resources::Ref* __fastcall unresolved_ref_assign_00406290(menu_resources::Ref*,void*,menu_resources::Ref*,const char*);
