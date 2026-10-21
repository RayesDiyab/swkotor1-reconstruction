// lane-audit-r7 r5 unit xc-0073afc0-part-list (authored by lane-audit-r7 for the R7 initializer allowlist; requested by lane-m46-3dcore (D69)).
// The original CRT runs this initializer from its __xc table; here the compiler-emitted .CRT$XCU record of the
// named-namespace root below is the modern _initterm entry for the same role (one record in this object).
// Original (dumpbin, swkotor.exe sha256 9c10e045...): __xc slot 0078D07C -> 0073AFC0, walked by _cinit 006FB11A..006FB138.
//   initializer: 0073AFC0 mov ecx,827F40h / 0073AFC5 call 005C5A30 / 0073AFCA push 73BA40h / 0073AFCF call 006FB332 / 0073AFD4 pop ecx / 0073AFD5 ret
//   atexit disposer: 0073BA40 mov ecx,827F40h / 0073BA45 jmp 0047ABC0
#include "cb0b2681a6-m46_cells_storage.h"   // lane-m46-3dcore storage owner (constant zero / constructed image)
#include <cstdlib>
#include <cstdint>
extern "C" __declspec(noinline) void __cdecl kotor_xc_dispose_0073ba40() {
    exo_array_list_free_0047abc0(&scene3d_part_list_00827f40, nullptr);   // 0073BA40 mov ecx,00827F40h / jmp 0047ABC0
}
extern "C" __declspec(noinline) int __cdecl kotor_xc_init_0073afc0() {
    scene3d_part_list_00827f40.size = 0;       // 0073AFC0 mov ecx,00827F40h / call 005C5A30: [eax+4] = 0 (005C5A34)
    scene3d_part_list_00827f40.capacity = 0;   // [eax+8] = 0 (005C5A37)
    scene3d_part_list_00827f40.data = 0;       // [eax] = 0 (005C5A3A)
    return std::atexit(&kotor_xc_dispose_0073ba40);   // push 0073BA40h / call 006FB332 / pop ecx / ret
}
namespace kotor_xc_0078d07c {
extern const int root_0073afc0;
const int root_0073afc0 = kotor_xc_init_0073afc0();   // .CRT$XCU record
}
