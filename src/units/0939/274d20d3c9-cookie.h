#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <cstdint>
struct CookieEntropyApi {
 void (WINAPI* time)(LPFILETIME);
 DWORD (WINAPI* process)();
 DWORD (WINAPI* thread)();
 DWORD (WINAPI* tick)();
 BOOL (WINAPI* counter)(LARGE_INTEGER*);
};
struct CookieFailureApi {
 void (__cdecl* report)(int,int); // original0070A9DE; owner must supply real body
 void (WINAPI* terminate)(UINT);
};
static_assert(sizeof(void*)==4&&sizeof(CookieEntropyApi)==20,"original x86 API ABI");
extern CookieEntropyApi cookie_entropy;
extern CookieFailureApi cookie_failure_api;
extern "C" {
extern std::uint32_t legacy_security_cookie; // original007A3180, not modern GS storage
std::uint32_t __cdecl legacy_initialize_cookie(); //0070A988, EAX counter-word XOR
void __fastcall legacy_check_cookie(std::uint32_t); //00705D37, ECX input
void __cdecl legacy_cookie_failure(); //00705D06 contract, report dependency gated
}
