// CC-03 private reuse forward: fb80_ports::Find005e5f20 -> sole M40 donor
// path_resolution::find_005e5f20 (original 005E5F20). No second Find body, no casts.
#include "06b965e51a-fb80.h"
#include "634f113adb-path.h"
#include <type_traits>
// Join holds only if FB80 already names the actual donor types; otherwise fail closed.
static_assert(std::is_same<fb80_ports::S,kotor_reconstruction::StartupString>::value,"FB80 String must be the actual StartupString");
static_assert(std::is_same<fb80_ports::S,path_resolution::String>::value,"FB80 and donor share one String type");
static_assert(std::is_same<fb80_ports::W,path_resolution::Word>::value&&sizeof(fb80_ports::W)==4,"full character stack DWORD");
static_assert(std::is_same<decltype(&fb80_ports::Find005e5f20),decltype(&path_resolution::find_005e5f20)>::value,"identical int32 fastcall type: ECX self, EDX unused, two stack DWORDs, RET 8");
namespace fb80_ports {
std::int32_t __fastcall Find005e5f20(S* self,void* edx,W character,std::int32_t start){
 return path_resolution::find_005e5f20(self,edx,character,start);
}
}
