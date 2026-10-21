// lane-audit-r6allow unit xi-007043c1-sse2-available (item (a); NOT admissible by m43_initializer_audit_r1 - see NOTES).
// Original (dumpbin, swkotor.exe sha256 9c10e045...): 007043C1 __sse2_available_init is a C initializer in the
// ORIGINAL __xi table (__xi_a 0078D194 .. __xi_z 0078D1B0, slot 0078D1A4, 4th entry), walked by _cinit's
// _initterm_e loop at 006FB0E7..006FB10A BEFORE the C++ __xc table (006FB11A). It is NOT in __xc_a..__xc_z.
//   body (owner: stage unit crt-pow-math_cells, canonical_math_cells.cpp crt_sse2_available_init_007043c1):
//   pushfd/EFLAGS.ID toggle (007043CA..007043D9); CPUID leaf 1 (007043DD..007043E2); [00834318]=0, [00834314]=0
//   (007043E7/007043EE); if EDX bit 26 and OS probe 0070438C (SEH-framed movapd xmm0,xmm1) != 0 -> both = 1
//   (00704404..0070440C); return 0 (00704411) - int result, 0 = success for _initterm_e.
// Readers of 00834314 (__use_sse2_mathfcns): _CIpow 006FC1F2, pow 006FC1B2, floor 006FC752, ceil 006FC882.
// 00834318 (__sse2_available) has no reader outside its writer (byte scan of .text).
// Modern equivalent of an __xi entry: one int(__cdecl*)(void) pointer in a .CRT$XIC contribution (between the
// VCRuntime's .CRT$XIA/__xi_a and .CRT$XIZ/__xi_z), run by _initterm_e before any .CRT$XCU entry - the same phase
// and the same before-all-C++-initializers order as the original. No body here (single owner kept).
extern "C" int __cdecl crt_sse2_available_init_007043c1();   // crt-pow-math_cells (canonical_math_cells.cpp)
using kotor_pifv = int(__cdecl*)();
#pragma section(".CRT$XIC", long, read)
extern "C" __declspec(allocate(".CRT$XIC")) const kotor_pifv kotor_xi_0078d1a4_007043c1 = &crt_sse2_available_init_007043c1;
