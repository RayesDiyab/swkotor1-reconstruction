#pragma once
namespace kotor::server_mainloop {
// Exact x87 sequence, including binary32 spill and active control-word rounding.
inline void effect_rank_logarithm(unsigned value,unsigned*output){
 unsigned factor=0x40549a78;float spill;
#if defined(_MSC_VER)
 __asm {
  fldlg2
  fild dword ptr [value]
  fyl2x
  fmul dword ptr [factor]
  fstp dword ptr [spill]
  fld dword ptr [spill]
  frndint
  mov edx,output
  fistp dword ptr [edx]
 }
#else
 __asm__ volatile("fldlg2; fildl %2; fyl2x; fmuls %3; fstps %1; flds %1; frndint; fistpl %0"
 : "=m"(*output),"=m"(spill):"m"(value),"m"(factor):"st");
#endif
}
}
