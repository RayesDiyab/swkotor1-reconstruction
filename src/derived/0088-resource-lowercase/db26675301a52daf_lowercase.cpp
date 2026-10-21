#include "226b37fa4c529bd3_lowercase.h"
using namespace kotor_lowercase;
extern "C" std::int32_t __cdecl kotor_lower_l_006fb69e(Locale* locale,std::int32_t character){
 const Locale* actual=locale;
 if(actual->ctype_locale==0||(actual->ascii_compatible!=0&&static_cast<Word>(character)<=127u)){
  return character>=65&&character<=90?character+32:character;
 }
 if(static_cast<Word>(character)<256u){
  std::int32_t uppercase;
  if(actual->mb_max>1){uppercase=dependencies.classify_007018cb(locale,character,1);}
  else{const auto* table=reinterpret_cast<const volatile unsigned char*>(actual->ctype);uppercase=table[static_cast<Word>(character)*2u]&1;}



  if(!uppercase)return character;

 }
 // The original packs input bytes into its incoming Locale-pointer argument
 // slot while retaining the actual Locale address separately in ESI.
 auto* input=reinterpret_cast<unsigned char*>(&locale);
 const Word high=(static_cast<Word>(character)>>8)&255u;
 const auto* table=reinterpret_cast<const volatile unsigned char*>(actual->ctype);
 int count;
 if(table[high*2u+1]&0x80){input[0]=static_cast<unsigned char>(high);input[1]=static_cast<unsigned char>(character);input[2]=0;count=2;}
 else{input[0]=static_cast<unsigned char>(character);input[1]=0;count=1;}
 Word output;
 const std::int32_t result=dependencies.map_0070272d(actual->ctype_locale,0x100,reinterpret_cast<const char*>(input),count,reinterpret_cast<char*>(&output),3,actual->codepage,1);
 if(!result)return character;
 const auto* bytes=reinterpret_cast<const unsigned char*>(&output);
 if(result==1)return bytes[0];
 return (static_cast<Word>(bytes[0])<<8)|bytes[1];
}
extern "C" std::int32_t __cdecl kotor_lower_006fb766(std::int32_t character){
 Locale* locale=dependencies.getptd_006feedf()->locale;

 if(locale!=*dependencies.global_locale_007a2824)locale=dependencies.update_006fdd2a();




 return kotor_lower_l_006fb69e(locale,character);
}
