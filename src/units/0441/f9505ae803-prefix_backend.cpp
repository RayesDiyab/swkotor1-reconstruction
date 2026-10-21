#include "525a467aa1-adoption.h"
#include <cstring>
#include <climits>
namespace alias_modern {
int __cdecl bounded_compare(const char* query,const char* key,std::uint32_t count) {
 // Explicitly selected modern backend for stable ASCII/C-locale Cstrings,
 // each shorter than INT_MAX. Historical count values above INT_MAX reach
 // their terminators in that domain. UCRT rejects those counts, so use its
 // genuine full comparator only for that range; all other counts remain
 // bounded exactly. This is not the task18 unbounded0070ACAF provider.
 if(count>static_cast<std::uint32_t>(INT_MAX))return _stricmp(query,key);
 return _strnicmp(query,key,static_cast<std::size_t>(count));
}
}
