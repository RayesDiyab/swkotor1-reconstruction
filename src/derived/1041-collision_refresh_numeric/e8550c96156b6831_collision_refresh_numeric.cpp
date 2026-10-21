#include "7ccfafa4292faca2_collision_refresh_numeric.hpp"
#include <cstring>
#include <cmath>
namespace kotor::server_mainloop {
bool collision_refresh_numeric(Word divisorBits,Word numeratorBits,Word&out){
 if((divisorBits&0x7f800000)==0x7f800000||(numeratorBits&0x7f800000)==0x7f800000)return false;
 float divisor,numerator;std::memcpy(&divisor,&divisorBits,4);std::memcpy(&numerator,&numeratorBits,4);
 unsigned char extended[10];double approximate;unsigned short oldControl,truncControl;std::int64_t integer;
#if defined(_MSC_VER) && defined(_M_IX86)
 if((divisorBits&0x7fffffff)!=0){__asm {fld numerator
 fdiv divisor
 fst approximate
 fstp tbyte ptr extended}}else{__asm {fld numerator
 fst approximate
 fstp tbyte ptr extended}}
#else
 __asm__ volatile("flds %2; fstl %0; fstpt %1":"=m"(approximate),"=m"(extended):"m"(numerator):"st");
 if((divisorBits&0x7fffffff)!=0){__asm__ volatile("flds %2; fdivs %3; fstl %0; fstpt %1":"=m"(approximate),"=m"(extended):"m"(numerator),"m"(divisor):"st");}
#endif
 // A deliberately conservative finite signed32 domain; conversion uses the preserved80-bit value.
 if(!std::isfinite(approximate)||approximate<=-2147483648.0||approximate>=2147483648.0)return false;
#if defined(_MSC_VER) && defined(_M_IX86)
 __asm {fnstcw oldControl}
 truncControl=static_cast<unsigned short>(oldControl|0xc00);
 __asm {fldcw truncControl
 fld tbyte ptr extended
 fistp qword ptr integer
 fldcw oldControl}
#else
 __asm__ volatile("fnstcw %0":"=m"(oldControl));truncControl=static_cast<unsigned short>(oldControl|0xc00);
 __asm__ volatile("fldcw %2; fldt %3; fistpq %0; fldcw %1":"=m"(integer):"m"(oldControl),"m"(truncControl),"m"(extended):"st");
#endif
 out=static_cast<Word>(integer);return true;
}
}
