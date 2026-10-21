#pragma once
#include "44d4f9bb7c-alias_path.h"
#include "f166f203af-base_ini.h"
#include <cstddef>
namespace alias_add {
using String=kotor_reconstruction::StartupString;
using Node=alias_path::NodeView;
using Internal=alias_path::InternalView;
using List=alias_path::AliasListView;
using Base=base_ini_reconstruction::Base;
using BaseInternal=base_ini_reconstruction::BaseInternal;
using PublicIni=base_ini_reconstruction::PublicIni;
// A genuine owner supplies these opaque original dependencies and the actual
// existing global cell address. This packet defines none of those state owners.
struct Dependencies {
 void* const volatile* base_007a39e0;
 int(__fastcall* read_005e67c0)(PublicIni*,void*,String*,String*,String*,String*);
 const char* section_0074ddb0;
 const char* empty_0073d71c;
 const char* slash_0073d8f4;
};
extern const Dependencies dependencies;
__declspec(noinline) String* __fastcall Upper005e6360(String*,void*,String*);
__declspec(noinline) int __fastcall Greater005e5490(String*,void*,String*);
__declspec(noinline) Node* __fastcall ConstructNode005eb600(Node*,void*);
__declspec(noinline) void __fastcall AddInternal005ebf30(Internal*,void*,String*,String*);
__declspec(noinline) void __fastcall Add005e6880(List*,void*,String*,String*);
// Original RET16 consumes four words; fourth is unreferenced in the whole body.
__declspec(noinline) void __fastcall AddAlias005e7760(BaseInternal*,void*,String*,String*,String*,String*);
static_assert(sizeof(Node)==24&&offsetof(Node,previous)==20&&offsetof(Node,next)==16,"genuine complete Alias allocation/link offsets");
static_assert(sizeof(Internal)==12&&offsetof(Internal,head)==8&&offsetof(Base,aliases)==12,"existing canonical prefix identities");
}
