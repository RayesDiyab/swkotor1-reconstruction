#pragma once
#include "d2e9f84747-base_ini.h"
#include <cstdint>
namespace base_resource_table {
using String=kotor_reconstruction::StartupString;
using Internal=base_ini_reconstruction::BaseInternal;
using Construct=String*(__fastcall*)(String*,void*);
using Destroy=void(__fastcall*)(String*,void*);
struct ResourcePair {std::uint16_t id;const char* literal;};
// Logical fixed pairs: Main6 owns one immutable support descriptor and actual
// canonical borrowed literal producers. Generated original data stays ignored.
extern const ResourcePair (&resource_pairs)[88];
String* __fastcall ConstructString(String*,void*);
void __fastcall DestroyString(String*,void*);
// Exact accepted normal iterator loop with canonical String lifetime carriers.
// Support projection only; original006FB55F EH cleanup remains held.
void __stdcall ConstructStringVectorNormal(String*,std::uint32_t,std::int32_t,Construct,Destroy);
void __fastcall CreateResourceExtensionTable005e6d20(Internal*,void*);
Internal* __fastcall ConstructBaseInternal005e8ba0(Internal*,void*);
}
