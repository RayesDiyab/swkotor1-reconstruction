// M47 composer (D79b): typed adapter for the original CRT setlocale 006FE34D, bound to the modern UCRT setlocale.
// Only original caller: 005F932D (StartServices), reached only when the language id read at 005F931C is 5 (Polish);
// unreachable on the English install. The original CRT locale internals (__interpretLocale 006FE16E and ~78 related
// functions, 14 CRT .data locale cells) are NOT reconstructed (open item for non-English support, D79b).
//
// Equivalence note (recorded per D79b): category numbering is identical (LC_ALL 0 .. LC_TIME 5 in both CRTs); the
// original returns 0 for category < 0 or > 5 (006FE35E..006FE369) - kept explicitly here because the UCRT would raise
// its invalid-parameter handler instead; a null name queries the current locale in both. Differences: the returned
// locale string is produced by the UCRT (Windows locale-name format) and the per-thread/global locale state lives in
// the UCRT rather than in the original CRT cells; no other staged code reads those original cells on the menu path.
#include <clocale>
#include <cstdint>
namespace services_tail5 {
using U32 = std::uint32_t; using I32 = std::int32_t;
U32 __cdecl Locale(I32 category, U32 name);   // same declaration as task-29 startservices-tail5 providers.h
U32 __cdecl Locale(I32 category, U32 name) {
    if (category < 0 || category > 5) return 0;                                   // 006FE35E..006FE369
    return static_cast<U32>(reinterpret_cast<std::uintptr_t>(std::setlocale(category, reinterpret_cast<const char*>(static_cast<std::uintptr_t>(name)))));
}
}
