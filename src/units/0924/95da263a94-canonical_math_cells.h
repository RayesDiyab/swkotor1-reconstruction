#pragma once
// Proposed Main6-owned storage. These are the original x86 cell representations.
extern "C" unsigned kotor_crt_pow_sse2_enabled_00834314;
extern "C" unsigned task8_crt_fast_math_enabled_00833bdc;
extern "C" int (__cdecl* task8_crt_matherr_callback_007a316c)(void*);
extern "C" unsigned task8_crt_matherr_user_007a36d0;
// Assembly-only default callback port: argument stack word ignored; EAX32 zero.
extern "C" void __cdecl task8_math_port_00706a22();
