#include "82b6d197bfb15cf9_area_map_numeric.hpp"
#include <cmath>
#include <cstring>
namespace kotor::server_mainloop {
void area_fog_store(Word input_bits,Word* destination){
 float value;const float zero=0.0f;unsigned short status;std::memcpy(&value,&input_bits,4);
#if defined(_MSC_VER) && defined(_M_IX86)
 __asm {mov ecx,destination
        fld value
        fst dword ptr [ecx]
        fcomp zero
        fnstsw status}
#elif defined(__i386__) || defined(__x86_64__)
 __asm__ volatile("flds %2; fsts %0; fcomps %3; fnstsw %1":"=m"(*destination),"=m"(status):"m"(value),"m"(zero):"st");
#else
 #error Exact x87 fog comparison requires an x86 target.
#endif
 const unsigned comparison=(status>>8)&5;
 if(comparison==1||comparison==4)*destination=0;
}
bool area_map_coordinate(Word input_bits,Word factor_bits,Word& output){
 if((input_bits&0x7f800000)==0x7f800000)return false;
 if(factor_bits!=0x43dc0000&&factor_bits!=0x43800000)return false;
 float input,multiplier;const float bias=0.5f;double rounded;
 std::memcpy(&input,&input_bits,4);std::memcpy(&multiplier,&factor_bits,4);
#if defined(_MSC_VER) && defined(_M_IX86)
 __asm {fld input
        fmul multiplier
        fadd bias
        fstp qword ptr rounded}
#elif defined(__i386__) || defined(__x86_64__)
 __asm__ volatile("flds %1; fmuls %2; fadds %3; fstpl %0":"=m"(rounded):"m"(input),"m"(multiplier),"m"(bias):"st");
#else
 #error Exact x87 map-coordinate implementation requires an x86 target.
#endif
 const double integral=std::floor(rounded);
 if(integral < -2147483648.0 || integral > 2147483647.0)return false;
 output=static_cast<Word>(static_cast<std::int32_t>(integral));return true;
}
}
