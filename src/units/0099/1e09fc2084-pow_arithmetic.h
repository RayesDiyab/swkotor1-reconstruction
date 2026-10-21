#pragma once
#include "e6297d9e4a-pow_raise_port.h"
// Assembly-call interfaces. ST/EDX/EAX and caller scratch contracts are recorded in REPORT.md.
extern "C" void __cdecl task8_pow_internal_006fc24d();
extern "C" void __cdecl task8_pow_sse2_internal_00704439();
extern "C" void __cdecl task8_math_exp2_00704200();
extern "C" void __cdecl task8_math_load_cw_00704215();
extern "C" void __cdecl task8_math_fload_00704245();
extern "C" void __cdecl task8_math_integer_test_006fc412();
extern "C" void __cdecl task8_math_error_pair_007041a0();
extern "C" void __cdecl task8_math_error_single_007041b7();
extern "C" void __cdecl task8_math_pow_special_007051e1();
extern "C" void __cdecl task8_math_classify_integer_00705177();
extern "C" void __cdecl task8_math_sse_error_007053de();
extern "C" void __cdecl task8_math_x87_error_007096b2();
extern "C" void __cdecl task8_math_port_006fe52c();
extern "C" void __cdecl task8_math_port_006ffe78();
extern "C" void __cdecl task8_math_port_00705d37();
extern "C" void __cdecl task8_math_port_00706a22();
extern "C" void __cdecl task8_math_port_007079e4();
extern "C" void __cdecl task8_math_port_00707c06();
extern "C" void __cdecl task8_math_port_00707e25();
extern "C" void __cdecl task8_math_port_0070a53b();
extern "C" unsigned task8_crt_fast_math_enabled_00833bdc;
extern "C" unsigned task8_crt_security_cookie_007a3180;
extern "C" int (__cdecl* task8_crt_matherr_callback_007a316c)(void*);
extern "C" unsigned task8_crt_matherr_user_007a36d0;
extern "C" const unsigned char task8_pow_const_0073ede8[16];
extern "C" const unsigned char task8_pow_const_0073edf8[16];
extern "C" const unsigned char task8_pow_const_00741df8[16];
extern "C" const unsigned char task8_pow_const_0074dc58[32];
extern "C" const unsigned char task8_pow_const_0075a960[15008];
extern "C" const unsigned char task8_pow_const_007a2780[16];
extern "C" const unsigned char task8_pow_const_007a2788[16];
extern "C" const unsigned char task8_pow_const_007a30e0[20];
extern "C" const unsigned char task8_pow_const_007a3110[16];
extern "C" const unsigned char task8_pow_const_007a32b0[16];
extern "C" const unsigned char task8_pow_const_007a32b8[16];
extern "C" const unsigned char task8_pow_const_007a32d0[16];
