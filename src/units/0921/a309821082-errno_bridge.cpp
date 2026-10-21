// lane-m48-b crt: the task-23 descriptor_io.h include (which drags io_locked.h and the excluded task-35 helpers.h) is
// replaced by the one declaration this unit uses (same signature as descriptor_io.h:5).
namespace kotor_descriptor_support { int* __cdecl errno_006ffe78(); }
#include <type_traits>
static_assert(sizeof(void*) == 4 && sizeof(int) == 4, "original EAX pointer ABI");
static_assert(std::is_same<decltype(&kotor_descriptor_support::errno_006ffe78), int* (__cdecl*)()>::value, "donor contract");
using kotor_descriptor_support::errno_006ffe78;
// Assembly-only import: a tail jump preserves the donor's complete EAX result.
extern "C" __declspec(naked) void __cdecl task8_math_port_006ffe78() {
    __asm { jmp errno_006ffe78 }
}
