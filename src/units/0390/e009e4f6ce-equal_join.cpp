// Source-only AddKey transport to the existing Task10 canonical equality owner.
#include "bb1c4af6af-add_key.h"
#include "f3b6b6404a-ref_string_joins.h"
#include <type_traits>
namespace add_key_lower {
using EqualCarrier = int (__fastcall*)(String*, void*, String*);
static_assert(std::is_same<String, kotor_reconstruction::StartupString>::value,
              "one actual canonical StartupString");
static_assert(std::is_same<decltype(&Equal005e5200), EqualCarrier>::value,
              "existing AddKey full int fastcall receiver and one stack pointer");
static_assert(std::is_same<decltype(&async_string_equal_005e5200), EqualCarrier>::value,
              "unchanged Task10 canonical donor transport");
__declspec(noinline) int __fastcall Equal005e5200(
    String* self, void* unused_edx, String* compare) {
    return async_string_equal_005e5200(self, unused_edx, compare);
}
}
