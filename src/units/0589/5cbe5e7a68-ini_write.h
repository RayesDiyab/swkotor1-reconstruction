#pragma once
#include "b85de36c60-base_ini.h"
namespace base_ini_reconstruction {
// No added storage or shadow globals. Single inheritance retains the submitted
// twelve-byte Ini layout and existing String/list object lifetime views.
struct WritableIni:Ini {
 __declspec(noinline) int __thiscall write_005eab00(String*,String*,String*,String*);
};
static_assert(sizeof(WritableIni)==12 && sizeof(PublicIni)==4,"original storage unchanged");
struct WriteBindings {
 String*(__fastcall *concat_005e5d10)(String*,void*,String*,const String*);
 Node*(__fastcall *add_head_005e9d70)(List*,void*,void*);
 int(__cdecl *print_006faf01)(void*,const char*,...);
};
extern WriteBindings write_bindings;
}
