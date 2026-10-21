#include "968af5075b-path.h"
#include "e87f8473e7-warning_runtime.h"
#include <type_traits>
namespace kotor_menu_10_19::dependencies {extern void* base_007a39e0;}
namespace path_resolution {
Bindings ForActualOwners(const char* empty,const char* with_extension,const char* without_extension){
 return {&kotor_menu_10_19::dependencies::base_007a39e0,
         &kotor_reconstruction::StartupFormat005e5680,empty,with_extension,without_extension};
}
String* __fastcall ResolveActual(void* aliases,void*,String* out,String* name,Word type){
 return resolve_005e68a0(static_cast<alias_path::AliasListView*>(aliases),nullptr,out,name,type);
}
}
static_assert(std::is_same<path_resolution::Base,base_ini_reconstruction::Base>::value,"one actual nominal Base");
static_assert(offsetof(path_resolution::Base,aliases)==12&&offsetof(path_resolution::Base,internal)==20,"actual publication fields");
static_assert(std::is_same<decltype(kotor_menu_10_19::dependencies::base_007a39e0),void*>::value,"borrow sole existing Base cell");
// Main retains ownership of descriptor/cells/literals and admission/drain. This
// source defines no Base, Alias, extension, engine, FILE or publication storage.
