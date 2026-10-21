#pragma once
#include "74272cb0e4-table_contract.h"

// Existing genuine lower algorithms are imported under the sole current
// Table/StartupString types. No fake provider or foreign class cast is supplied.
extern "C" {
std::int32_t __fastcall c2da_current_column_index_00413100(
    c2da_current::Table*,void*,c2da_current::String*);
char __fastcall c2da_current_string_index_005e5600(
    c2da_current::String*,void*,std::int32_t);
c2da_current::String* __fastcall GuiAssign005e5140(
    c2da_current::String*,void*,const char*);

// ECX canonical Table, unused EDX; row/column/output onstack, RET12; full EAX0/1.
std::int32_t __fastcall c2da_current_get_string_00413270(
    c2da_current::Table*,void*,std::int32_t,
    c2da_current::String*,c2da_current::String*);
}
