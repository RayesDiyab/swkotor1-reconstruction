#include "6249da84027a5d84_cleaners.h"
extern "C" void __cdecl kotor_cleanup_0070631b(void* block) {
 if(!block)return;
 auto* fields=static_cast<kotor_locale::MoneyLocale*>(block);



 for(unsigned i=0;i<3;++i) {
  char* p=fields->strings[i];



  auto* current=*kotor_locale_cleaners::dependencies.current_money_007a328c;

  if(p==current->strings[i])continue;

  if(p==kotor_locale_cleaners::dependencies.initial_money_007a325c->strings[i])continue;

  kotor_locale_cleaners::dependencies.free_006fb7b2(p);
 }
}
extern "C" void __cdecl kotor_cleanup_00706547(void* block) {
 if(!block)return;
 auto* fields=static_cast<kotor_locale::MoneyLocale*>(block);
 for(unsigned i=3;i<10;++i) {
  char* p=fields->strings[i];
  auto* current=*kotor_locale_cleaners::dependencies.current_money_007a328c;
  if(p==current->strings[i])continue;
  if(p==kotor_locale_cleaners::dependencies.initial_money_007a325c->strings[i])continue;
  kotor_locale_cleaners::dependencies.free_006fb7b2(p);
 }
}
extern "C" void __cdecl kotor_cleanup_0070612c(kotor_locale::TimeLocale* block) {
 if(!block)return;



 for(unsigned i=1;i<7;++i)kotor_locale_cleaners::dependencies.free_006fb7b2(const_cast<char*>(block->strings[i]));
 kotor_locale_cleaners::dependencies.free_006fb7b2(const_cast<char*>(block->strings[0]));
 for(unsigned i=8;i<14;++i)kotor_locale_cleaners::dependencies.free_006fb7b2(const_cast<char*>(block->strings[i]));
 kotor_locale_cleaners::dependencies.free_006fb7b2(const_cast<char*>(block->strings[7]));
 for(unsigned i=14;i<43;++i)kotor_locale_cleaners::dependencies.free_006fb7b2(const_cast<char*>(block->strings[i]));




}
