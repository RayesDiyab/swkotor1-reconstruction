#include "5408f4c951-compare.h"
using namespace kotor_lowercase;
extern "C" std::int32_t __cdecl kotor_ascii_stricmp_0070ab30(const char* first,const char* second){
 const volatile unsigned char* a=reinterpret_cast<const volatile unsigned char*>(first);
 const volatile unsigned char* b=reinterpret_cast<const volatile unsigned char*>(second);
 for(;;){
  // The assembly loads the second byte before the first and skips folding
  // raw-equal bytes; both pointers advance before the loop condition.
  unsigned right=*b++;unsigned left=*a++;
  if(left==right){if(right==0)return 0;continue;}
  if(right>=65u&&right<=90u)right+=32u;
  if(left>=65u&&left<=90u)left+=32u;
  if(left==right)continue;



  return left<right?-1:1;

 }
}
extern "C" std::int32_t __cdecl kotor_stricmp_0070acaf(const char* first,const char* second){
 Locale* locale=dependencies.getptd_006feedf()->locale;

 if(locale!=*dependencies.global_locale_007a2824)locale=dependencies.update_006fdd2a();

 if(locale->ctype_locale==0)return kotor_ascii_stricmp_0070ab30(first,second);
 const volatile unsigned char* a=reinterpret_cast<const volatile unsigned char*>(first);
 const volatile unsigned char* b=reinterpret_cast<const volatile unsigned char*>(second);
 std::int32_t left,right;
 do{




  left=kotor_lower_l_006fb69e(locale,*a);
  right=kotor_lower_l_006fb69e(locale,*b);

  ++a;++b;
 }while(left!=0&&left==right);



 // SUB EAX,ECX is a 32-bit subtraction. MSVC's documented integer conversion
 // retains the two's-complement bit pattern without C++ signed overflow.
 return static_cast<std::int32_t>(static_cast<Word>(left)-static_cast<Word>(right));

}
