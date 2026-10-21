#pragma once
#include "0fffd40974-startup_string.h"
#include <cstdint>
using AliasString=kotor_reconstruction::StartupString;
bool InstallAliasStringProviders(const char* empty_0073d71c);
namespace alias_modern {
// Explicit modern functional backend, not reconstructed0070AD18 or a locale
// fallback. Historical PTD/locale/CRT closure remains a separate adoption gate.
int __cdecl bounded_compare(const char* query,const char* key,std::uint32_t count);
}
