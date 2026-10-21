#include "6c7cb94f2d0c3986_release.h"
#include <cstring>
using namespace kotor_locale_release;
// Read the R2 opaque fields without changing its sealed type proposal.
template<class T> static T Field(const Locale* locale,unsigned offset){T value;std::memcpy(&value,reinterpret_cast<const unsigned char*>(locale)+offset,sizeof(value));return value;}
extern "C" void __cdecl kotor_release_locale_006fda5a(Locale* locale){
 void* money=Field<void*>(locale,0x3c);
 if(money!=*dependencies.money_cell_00833fec&&money){
  if(locale->ref2c->value==0){
   RefCount* ref=locale->ref34;



   if(ref&&ref->value==0&&ref!=*dependencies.ref34_cell_00834308){

    dependencies.free_006fb7b2(ref);
    dependencies.clean_00706547(Field<void*>(locale,0x3c));
   }
   ref=locale->ref30;
   if(ref&&ref->value==0&&ref!=*dependencies.ref30_cell_00834310){
    dependencies.free_006fb7b2(ref);



    dependencies.clean_0070631b(Field<void*>(locale,0x3c));

   }
   dependencies.free_006fb7b2(locale->ref2c);
   dependencies.free_006fb7b2(Field<void*>(locale,0x3c));
  }
 }
 RefCount* ref=locale->ref40;
 if(ref!=*dependencies.ref40_cell_00834304&&ref&&ref->value==0){
  dependencies.free_006fb7b2(ref);
  dependencies.free_006fb7b2(Field<void*>(locale,0x44));
 }
 TimeLocale* time=locale->cleanup_time50;
 if(time!=*dependencies.time_cell_00833fe8&&time&&time->references==0){



  dependencies.clean_0070612c(time);

  dependencies.free_006fb7b2(locale->cleanup_time50);
 }

 dependencies.free_006fb7b2(locale);

}
