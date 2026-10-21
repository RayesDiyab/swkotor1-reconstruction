#pragma once
#include "8fbfcf9c73-locale.h"
#include <type_traits>
namespace kotor_lowercase {
using Word=kotor_locale::Word;
using Locale=kotor_locale::Locale;
using Ptd=kotor_locale::Ptd;
struct Dependencies {
 Ptd*(__cdecl* getptd_006feedf)();
 Locale*(__cdecl* update_006fdd2a)();
 std::int32_t(__cdecl* classify_007018cb)(Locale*,std::int32_t,Word);
 std::int32_t(__cdecl* map_0070272d)(Word,Word,const char*,int,char*,int,Word,int);
 Locale* volatile* global_locale_007a2824;
};
extern const Dependencies dependencies;
static_assert(std::is_same<Locale,kotor_locale::Locale>::value&&std::is_same<Ptd,kotor_locale::Ptd>::value,"one actual type owner");
}
extern "C" __declspec(noinline) std::int32_t __cdecl kotor_lower_l_006fb69e(kotor_lowercase::Locale*,std::int32_t);
extern "C" __declspec(noinline) std::int32_t __cdecl kotor_lower_006fb766(std::int32_t);
