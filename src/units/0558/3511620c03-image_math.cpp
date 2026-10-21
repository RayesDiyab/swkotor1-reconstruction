// 0045DD90 ImageGammaCorrect, 0045E790 ImageGetAlphaMean (lane-font packet 5).
// Evidence: lane-font/disasm/0045dd90.asm.txt, 0045e790.asm.txt. Constants are the original
// read-only cell values: 0073D764 1.0f, 0073EDE0 float 1/255 (0x3B808081), 007414E8 double 255.0,
// 0073D700 0.0f. Arithmetic is kept on the x87 stack (TBYTE spills) so precision-control
// behaviour matches the original instead of SSE2 double/float rounding.
#include "43e0af9c1e-font_txi.hpp"
#include <cstring>
namespace font_txi {
namespace {
const float kOne=1.0f;                         // 0073D764
const float kInverse255=0.003921568859368563f; // 0073EDE0 (bits 3B808081)
const double k255=255.0;                       // 007414E8
const float kZero=0.0f;                        // 0073D700
struct Extended {std::uint8_t bytes[10];};
I SignedBits(U v){return v&0x80000000u?-1-static_cast<I>(~v):static_cast<I>(v);}
static_assert(sizeof(kInverse255)==4,"binary32 cell");
}
}
using namespace font_txi;
extern "C" void __cdecl scheduler_gamma_words_0045dd90(I width,I height,U components,void* image,U gamma_bits){
 float gamma;std::memcpy(&gamma,&gamma_bits,4);
 if(gamma==kOne)return;                        // 0045DD91..0045DDA0 fcomp/test ah,44h/jnp (NaN proceeds)
 I count=SignedBits(static_cast<U>(width)*static_cast<U>(height)*components); // imul,imul (0045DDA8..0045DDAD)
 if(count<=0)return;                           // 0045DDB6 jle
 float one=kOne,scale=kInverse255;double full=k255;
 float inverse;                                // 0045DDB8..0045DDC7 fld 1.0 / fdiv gamma / fstp dword
 __asm {
  fld dword ptr one
  fdiv dword ptr gamma
  fstp dword ptr inverse
 }
 auto pow_target=&genuine_x87_pow_006fc1f0;auto ftol_target=&genuine_x87_ftol_006fae8c;
 unsigned char* bytes=static_cast<unsigned char*>(image);
 for(I i=0;i<count;++i){                       // 0045DDD0..0045DDFC
  I value=bytes[i];U result;
  __asm {
   fild dword ptr value                         // 0045DDD8
   fmul dword ptr scale                         // 0045DDDC
   fld dword ptr inverse                        // 0045DDE2 (ST1=base, ST0=exponent)
   call pow_target                              // 0045DDE6 call 006FC1F0
   fmul qword ptr full                          // 0045DDEB
   call ftol_target                             // 0045DDF1 call 006FAE8C (EDX:EAX)
   mov result,eax
  }
  bytes[i]=static_cast<unsigned char>(result);  // 0045DDF6 mov [esi+ebx],al
 }
}
extern "C" float __cdecl scheduler_alpha_mean_0045e790(const void* image,I width,I height,U components){
 if(SignedBits(components)<4)return kOne;      // 0045E797 cmp edx,4 / jge (signed); fld 1.0
 I count=SignedBits(static_cast<U>(width)*static_cast<U>(height)); // 0045E7B2..0045E7B6
 Extended sum;float zero=kZero,scale=kInverse255;
 __asm {
  fld dword ptr zero                           // 0045E7A6
  fstp tbyte ptr sum
 }
 // The original unrolls by four (0045E7F4..0045E864) then finishes singly (0045E880..0045E896);
 // the per-pixel order of additions is sequential in both, reproduced here directly.
 U address=reinterpret_cast<U>(image)+3u;     // alpha byte of pixel 0 (lea eax,[ebp+3])
 for(I i=0;i<count;++i,address+=components){
  I alpha=*reinterpret_cast<const volatile unsigned char*>(address);
  __asm {
   fld tbyte ptr sum
   fild dword ptr alpha
   fmul dword ptr scale
   faddp st(1),st
   fstp tbyte ptr sum
  }
 }
 float result;
 __asm {
  fld tbyte ptr sum
  fidiv dword ptr count                         // 0045E898 fidiv w*h
  fstp dword ptr result                         // consumer (scheduler alpha_mean_bits) stores ST0 as binary32
 }
 return result;
}
