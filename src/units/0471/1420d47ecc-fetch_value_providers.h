#pragma once
#include "b55d7d7357-resource_lifetime.h"
#include "0e26ab1a48-reference.h"
namespace tlk_values {
using Word=std::uint32_t;using String=kotor_reconstruction::StartupString;using Ref=tlk_live::SoundRef;
struct NumericBindings {
 int (__cdecl* sprintf_006fadb0)(char*,const char*,...);
 void* (__cdecl* allocate_006fa7e6)(Word);
 const char* signed_decimal_0073e2ec;
};
extern const NumericBindings numeric_bindings;
}
extern "C" __declspec(noinline) tlk_values::Ref* __fastcall SoundAssign00406170(tlk_values::Ref*,void*,tlk_values::Ref*,const tlk_values::Ref*);
// Link alias to the actual new signed-int constructor, same ECX/RET4/EAX ABI.
// Receiver must be fresh suitably aligned storage; this starts String lifetime.
extern "C" tlk_values::String* __fastcall NumericConstruct005e5bc0(tlk_values::String*,void*,tlk_values::Word);
