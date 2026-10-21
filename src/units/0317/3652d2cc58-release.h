#pragma once
#include "99aa702e48-locale.h"
namespace kotor_locale_release {
using kotor_locale::Locale;using kotor_locale::RefCount;using kotor_locale::TimeLocale;
struct Dependencies {
 void (__cdecl* free_006fb7b2)(void*);
 void (__cdecl* clean_00706547)(void*);
 void (__cdecl* clean_0070631b)(void*);
 void (__cdecl* clean_0070612c)(TimeLocale*);
 void* volatile* money_cell_00833fec;
 RefCount* volatile* ref34_cell_00834308;
 RefCount* volatile* ref30_cell_00834310;
 RefCount* volatile* ref40_cell_00834304;
 TimeLocale* volatile* time_cell_00833fe8;
};
extern const Dependencies dependencies;
}
extern "C" __declspec(noinline) void __cdecl kotor_release_locale_006fda5a(kotor_locale::Locale*);
