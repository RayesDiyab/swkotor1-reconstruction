#pragma once
#include "6082f76680-destruction.hpp" // E1 epoch chain
namespace texture_owner {
using U=texture_destruction::U;using I=texture_destruction::I;
using Ref=texture_destruction::Ref;using Basic=texture_destruction::Basic;
using List=texture_destruction::List;using Safe=font::Safe;
extern void** ref_table;
extern "C" {
 U __fastcall list_destroy_00422d60(List*,void*);
 U __fastcall base_destroy_00422e90(Ref*,void*);
 void* __fastcall scalar_delete_004233c0(void*,void*,U); // E1 edit O1: vtable +00 shared type (E-D2)
 Ref* __fastcall basic_query_00641db0(Ref*,void*);
 U __cdecl dispatch_delete(void*,Ref*,U);
}
}
