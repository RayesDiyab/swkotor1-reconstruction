#pragma once
#include "b1c9e15e4b3a62bf_locale.h"
namespace kotor_locale_cleaners {
struct Dependencies {
 void(__cdecl* free_006fb7b2)(void*);
 kotor_locale::MoneyLocale* volatile* current_money_007a328c;
 kotor_locale::MoneyLocale* initial_money_007a325c;
};
extern const Dependencies dependencies;
}
extern "C" __declspec(noinline) void __cdecl kotor_cleanup_0070631b(void*);
extern "C" __declspec(noinline) void __cdecl kotor_cleanup_00706547(void*);
extern "C" __declspec(noinline) void __cdecl kotor_cleanup_0070612c(kotor_locale::TimeLocale*);
