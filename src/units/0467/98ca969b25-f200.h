#pragma once
#include "15444fa152-addkey_bodies.h"
#include "873954807b-providers.h"
#include "d838e3c0da-table_contract.h"
#include <cstddef>
namespace f200_ports {
using Word=menu_resources::Word;using String=menu_resources::String;using Ref=menu_resources::Ref;
// Private measured local descriptor, NOT a claim of original external class.
// Main must reconcile sole nominal owner when the enumeration producer returns.
struct Enumeration {String* entries;Word count,capacity;};
static_assert(sizeof(Enumeration)==12 && offsetof(Enumeration,count)==4 && offsetof(Enumeration,capacity)==8,"observed three words");
void __fastcall Enumerate005e6640(void*,void*,void*,String*,Word,Word,Word);
void* __fastcall Allocate0040eb80(void*,void*,Word);
Word __fastcall Type00406650(menu_resources::Manager*,void*,String*);
void __fastcall Ref004065a0(menu_resources::Manager*,void*,Ref*,String*);
void __fastcall Insert0040e990(void*,void*,Ref*,Word,Word,Word);
}
