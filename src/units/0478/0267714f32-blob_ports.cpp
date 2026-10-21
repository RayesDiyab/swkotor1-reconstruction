// lane-resource-lowers: fb80_ports blob allocation ports (task-2 fb80-r1/fb80.h:8-9).
// Original evidence (swkotor.exe):
//   005E9350: E9 A6 45 11 00  jmp 006FD8FB      ; Ghidra "_malloc" thunk
//   006FD8FB: push [00833ECC] ; push [esp+8] ; call 006FD8CF (_nh_malloc) ; pop ecx ; pop ecx ; ret
//             => malloc(size) == _nh_malloc(size, _newmode); cdecl, caller pops
//   005E9360: E9 4D 24 11 00  jmp 006FB7B2      ; Ghidra "_free" thunk
//   006FB7B2: CRT free (SEH frame 0075A060, small-block-heap lookup, HeapFree)
// Both thunks forward to the SAME game Win32-heap family that M40 binds as
// live_malloc (= live_nh_malloc(size,*new_mode)) and live_free (live-policy.obj,
// allocator_live.cpp). FB80 allocates the KEY file-name blob with 005E9350 and
// releases it only with 005E9360 (fb80.cpp Core/late_failure), so the family is
// closed: no CRT pointer crosses into live_free and vice versa.
#include "06b965e51a-fb80.h"
#include "d2c5c47a35-allocator_live.h"
#include <type_traits>
static_assert(std::is_same<fb80_ports::W,std::uint32_t>::value,"size is the full cdecl stack DWORD");
static_assert(std::is_same<decltype(&live_malloc),void*(__cdecl*)(std::uint32_t)>::value,"malloc family provider");
static_assert(std::is_same<decltype(&live_free),void(__cdecl*)(void*)>::value,"free family provider");
namespace fb80_ports {
void* __cdecl AllocateBlob005e9350(W size){return live_malloc(size);}
void __cdecl ReleaseBlob005e9360(void* blob){live_free(blob);}
}
