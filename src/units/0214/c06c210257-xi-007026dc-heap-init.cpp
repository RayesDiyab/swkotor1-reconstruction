// lane-m50-heap unit xi-007026dc-heap-init (G-HEAP fix; needs an initializer-audit successor before staging).
// Original (swkotor.exe sha256 9c10e045...): WinMainCRTStartup 006FB38D calls _heap_init 007026DC directly at
// 006FB456 (push 1 / call 007026DC), BEFORE _mtinit 006FB468 and BEFORE _cinit 006FB4C9 walks __xi (006FB0E7) and
// __xc (006FB11A). It is not an __xi/__xc table entry; it is the CRT's own pre-initializer heap creation.
// Modern equivalent: one int(__cdecl*)(void) pointer in .CRT$XIB, run by the modern CRT's _initterm_e over
// __xi_a..__xi_z after its own startup (.CRT$XIAA/.CRT$XIAC) and BEFORE every .CRT$XIC (incl. the r6 SSE2 entry
// 007043C1) and every .CRT$XCU entry - the same "before every C and C++ initializer" position as the original.
// No body here: the body is kotor_crt_heap_init_007026dc_phase in unit crt-heap-cells (single owner).
extern "C" int __cdecl kotor_crt_heap_init_007026dc_phase();   // crt-heap-cells (crt_heap_cells.cpp)
using kotor_pifv = int(__cdecl*)();
#pragma section(".CRT$XIB", long, read)
extern "C" __declspec(allocate(".CRT$XIB")) const kotor_pifv kotor_xi_heap_init_007026dc = &kotor_crt_heap_init_007026dc_phase;
