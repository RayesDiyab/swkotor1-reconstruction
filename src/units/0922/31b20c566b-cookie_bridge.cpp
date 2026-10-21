#include "274d20d3c9-cookie.h"
#include <type_traits>
static_assert(std::is_same<decltype(&legacy_check_cookie), void (__fastcall*)(std::uint32_t)>::value, "ECX donor contract");
// Assembly-only import: no argument conversion, stack adjustment, or extra return.
extern "C" __declspec(naked) void __cdecl task8_math_port_00705d37() {
    __asm { jmp legacy_check_cookie }
}
