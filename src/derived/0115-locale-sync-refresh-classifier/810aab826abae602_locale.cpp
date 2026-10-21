#include "b1c9e15e4b3a62bf_locale.h"
using namespace kotor_locale;
extern "C" Locale* __cdecl kotor_sync_locale_006fdb24(){
 Ptd* ptd=dependencies.getptd_006feedf();Locale* old=ptd->locale;
 if(old!=*dependencies.global_locale_007a2824){
  if(old){
   RefCount* p=old->ref2c;old->references=old->references-1u;if(p)p->value=p->value-1u;
   p=old->ref34;if(p)p->value=p->value-1u;
   p=old->ref30;if(p)p->value=p->value-1u;
   p=old->ref40;if(p)p->value=p->value-1u;
   TimeLocale* t=old->time;t->references=t->references-1u;
  }



  ptd->locale=*dependencies.global_locale_007a2824;

  Locale* current=*dependencies.global_locale_007a2824;current->references=current->references+1u;
  RefCount* p=(*dependencies.global_locale_007a2824)->ref2c;if(p)p->value=p->value+1u;
  p=(*dependencies.global_locale_007a2824)->ref34;if(p)p->value=p->value+1u;
  p=(*dependencies.global_locale_007a2824)->ref30;if(p)p->value=p->value+1u;
  p=(*dependencies.global_locale_007a2824)->ref40;if(p)p->value=p->value+1u;
  TimeLocale* t=(*dependencies.global_locale_007a2824)->time;t->references=t->references+1u;
  if(old&&old->references==0){

   if(old!=dependencies.initial_locale_007a27d0)

    dependencies.release_006fda5a(old);
  }
 }
 return ptd->locale;
}
extern "C" void __cdecl kotor_unlock12_006fdd5c(){dependencies.unlock_007019e0(12);}
extern "C" Locale* __cdecl kotor_refresh_locale_006fdd2a(){
 dependencies.lock_00701a74(12);
 __try {return kotor_sync_locale_006fdb24();}
 __finally {

  kotor_unlock12_006fdd5c();

 }
}
extern "C" Word __cdecl kotor_classify_007018cb(Locale* locale,std::int32_t character,Word mask){



 if(static_cast<Word>(character)+1u<=256u)

  return static_cast<Word>(locale->ctype[character])&mask;
 unsigned char input[4];const Word high=(static_cast<Word>(character)>>8)&255u;int count;
 if(reinterpret_cast<const volatile unsigned char*>(locale->ctype)[high*2u+1]&0x80){input[0]=static_cast<unsigned char>(high);input[1]=static_cast<unsigned char>(character);input[2]=0;count=2;}
 else{input[0]=static_cast<unsigned char>(character);input[1]=0;count=1;}
 auto* output=reinterpret_cast<std::uint16_t*>(reinterpret_cast<unsigned char*>(&character)+2);
 const int result=dependencies.nls_classify_007073f8(1,reinterpret_cast<const char*>(input),count,output,locale->codepage,locale->ctype_locale,1);
 if(!result)return 0;
 return static_cast<Word>(*output)&mask;
}
