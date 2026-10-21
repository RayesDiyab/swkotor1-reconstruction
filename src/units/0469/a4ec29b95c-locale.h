#pragma once
#include <cstdint>
#include <cstddef>
#if !defined(_MSC_VER)||!defined(_M_IX86)
#error MSVC x86 required
#endif
namespace kotor_locale {
using Word=std::uint32_t;
struct RefCount {volatile Word value;};
struct TimeLocale {const char* strings[43];Word locale,calendar;volatile Word references;};
struct MoneyLocale {char* strings[10];unsigned char flags[8];};
struct Locale {
 volatile Word references,codepage;unsigned char opaque8[12];volatile Word ctype_locale;
 unsigned char opaque18[12];volatile Word ascii_compatible;volatile std::int32_t mb_max;
 RefCount* volatile ref2c;RefCount* volatile ref30;RefCount* volatile ref34;void* volatile money38;
 unsigned char opaque3c[4];RefCount* volatile ref40;unsigned char opaque44[4];const std::uint16_t* volatile ctype;TimeLocale* volatile time;TimeLocale* volatile cleanup_time50;
};
struct Ptd {unsigned char opaque[0x64];Locale* volatile locale;};
static_assert(sizeof(Locale)==0x54&&offsetof(Locale,ref2c)==0x2c&&offsetof(Locale,ref30)==0x30&&offsetof(Locale,ref34)==0x34&&offsetof(Locale,ref40)==0x40&&offsetof(Locale,time)==0x4c&&offsetof(Locale,cleanup_time50)==0x50&&offsetof(Locale,ctype)==0x48&&offsetof(Ptd,locale)==0x64,"observed x86 views");
struct Dependencies {
 Ptd*(__cdecl* getptd_006feedf)();
 void(__cdecl* release_006fda5a)(Locale*);
 void(__cdecl* lock_00701a74)(int);
 void(__cdecl* unlock_007019e0)(int);
 int(__cdecl* nls_classify_007073f8)(Word,const char*,int,std::uint16_t*,Word,Word,int);
 Locale* volatile* global_locale_007a2824;
 Locale* initial_locale_007a27d0;
};
extern const Dependencies dependencies;
extern Locale initial_locale_007a27d0;
extern Locale* volatile global_locale_007a2824;
extern const std::uint16_t ctype_with_eof_0075e530[257];
extern TimeLocale initial_time_007a3198;
extern MoneyLocale initial_money_007a325c;
extern char decimal_point_007a3258[4];
extern char empty_00833fe4[4];
static_assert(sizeof(TimeLocale)==0xb8&&offsetof(TimeLocale,references)==0xb4&&sizeof(MoneyLocale)==0x30,"initial time/money layout");
}
extern "C" __declspec(noinline) kotor_locale::Locale* __cdecl kotor_sync_locale_006fdb24();
extern "C" __declspec(noinline) kotor_locale::Locale* __cdecl kotor_refresh_locale_006fdd2a();
extern "C" __declspec(noinline) void __cdecl kotor_unlock12_006fdd5c();
extern "C" __declspec(noinline) kotor_locale::Word __cdecl kotor_classify_007018cb(kotor_locale::Locale*,std::int32_t,kotor_locale::Word);
