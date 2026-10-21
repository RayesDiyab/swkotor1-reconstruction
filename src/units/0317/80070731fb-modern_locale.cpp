#include "69b775ba61-modern_locale.h"
#include "6ab21ebe52-lowercase.h"
#include "f4f2f2f883-reference.h"
#include "3652d2cc58-release.h"
#include "0de999f0f1-cleaners.h"
#include "934c4cb640-cells.h"
#include "7deb550b61-allocator_live.h"
#include "b5ad5e439a-nls_helpers.h"
#include "ec3073db47-ansi_providers.h"
#include <cstdlib>
#include <cstring>
#include <exception>
namespace kotor_functional {
namespace {
LocaleScope* active=nullptr;
struct ThreadLocale {
 kotor_locale::Ptd ptd{}; // Actual constructed canonical type, no raw PTD cast.
 LocaleScope* scope=nullptr;
 ThreadLocale* next=nullptr;
 DWORD id=0;
 ~ThreadLocale(){if(scope)scope->detach(this);}
};
thread_local ThreadLocale local;
[[noreturn]] void broken(){std::terminate();}
void drop(kotor_locale::Locale* p) {



 if(!p)return;
 p->references=p->references-1u;
 if(p->ref2c)p->ref2c->value=p->ref2c->value-1u;
 if(p->ref34)p->ref34->value=p->ref34->value-1u;
 if(p->ref30)p->ref30->value=p->ref30->value-1u;
 if(p->ref40)p->ref40->value=p->ref40->value-1u;
 p->time->references=p->time->references-1u;
 if(!p->references&&p!=&kotor_locale::initial_locale_007a27d0)kotor_release_locale_006fda5a(p);

}
unsigned __cdecl length(const char* p){return static_cast<unsigned>(std::strlen(p));}
// Supported modern heap bridge, zero original-address credit. NLS uses small
// checked counts; retain original low DWORD multiplication/zero working size.
void* __cdecl zero_allocate(unsigned count,unsigned size){const unsigned n=count*size;void* p=live_malloc(n?n:1);if(p)std::memset(p,0,n?n:1);return p;}
kotor_locale::Ptd* __cdecl get(){if(!active)broken();const DWORD error=GetLastError();auto* p=active->thread();SetLastError(error);return p;}
void __cdecl lock(int index){if(!active)broken();active->enter(index);}
void __cdecl unlock(int index){if(!active)broken();active->leave(index);}
int __cdecl classify(kotor_locale::Locale* p,int c,unsigned mask){return static_cast<int>(kotor_classify_007018cb(p,c,mask));}
}
bool LocaleScope::identity() const noexcept {
 return claimed_&&active==this&&source_heap_identity()&&
 nls_globals.classification_mode_00834034==&classification_mode_&&nls_globals.mapping_mode_00833ec4==&mapping_mode_&&
 nls_globals.locale_codepage_0083402c==&codepage_&&nls_globals.locale_id_0083401c==&locale_id_&&
 nls_bindings.type_w==GetStringTypeW&&nls_bindings.type_a==GetStringTypeA&&nls_bindings.map_w==LCMapStringW&&nls_bindings.map_a==LCMapStringA&&
 nls_bindings.to_wide==MultiByteToWideChar&&nls_bindings.to_narrow==WideCharToMultiByte&&nls_bindings.last_error==GetLastError&&
 nls_bindings.locale_cp_00709419==ansi_locale_cp_00709419&&nls_bindings.transcode_00709462==ansi_transcode_00709462&&nls_bindings.free_006fb7b2==live_free&&
 ansi_bindings.locale_info==GetLocaleInfoA&&ansi_bindings.cp_info==GetCPInfo&&ansi_bindings.to_wide==MultiByteToWideChar&&ansi_bindings.to_narrow==WideCharToMultiByte&&
 ansi_bindings.atoi_006faf5f==std::atoi&&ansi_bindings.strlen_00701300==length&&ansi_bindings.calloc_006fb823==zero_allocate&&ansi_bindings.free_006fb7b2==live_free;
}
bool locale_identity() noexcept{return active&&active->identity();}
bool LocaleScope::claim() noexcept {
 if(claimed_||active||!source_heap_identity()||
 kotor_locale::global_locale_007a2824!=&kotor_locale::initial_locale_007a27d0||
 kotor_locale::initial_locale_007a27d0.references!=1||kotor_locale::initial_time_007a3198.references!=0||
 nls_globals.classification_mode_00834034||nls_globals.mapping_mode_00833ec4||nls_globals.locale_codepage_0083402c||nls_globals.locale_id_0083401c||
 nls_bindings.type_w||nls_bindings.type_a||nls_bindings.map_w||nls_bindings.map_a||nls_bindings.to_wide||nls_bindings.to_narrow||nls_bindings.last_error||nls_bindings.locale_cp_00709419||nls_bindings.transcode_00709462||nls_bindings.free_006fb7b2||
 ansi_bindings.locale_info||ansi_bindings.cp_info||ansi_bindings.to_wide||ansi_bindings.to_narrow||ansi_bindings.atoi_006faf5f||ansi_bindings.strlen_00701300||ansi_bindings.calloc_006fb823||ansi_bindings.free_006fb7b2)return false;
 if(!InitializeCriticalSectionEx(&lock_,4000,0))return false;
 owner_=GetCurrentThreadId();threads_=nullptr;classification_mode_=mapping_mode_=codepage_=locale_id_=0;
 nls_globals={&classification_mode_,&mapping_mode_,&codepage_,&locale_id_};
 nls_bindings={GetStringTypeW,GetStringTypeA,LCMapStringW,LCMapStringA,MultiByteToWideChar,WideCharToMultiByte,GetLastError,ansi_locale_cp_00709419,ansi_transcode_00709462,live_free};
 ansi_bindings={GetLocaleInfoA,GetCPInfo,MultiByteToWideChar,WideCharToMultiByte,std::atoi,length,zero_allocate,live_free};
 claimed_=true;active=this;return true;
}
void LocaleScope::enter(int index){if(index!=12||!identity())broken();EnterCriticalSection(&lock_);}
void LocaleScope::leave(int index){if(index!=12||!identity())broken();LeaveCriticalSection(&lock_);}
kotor_locale::Ptd* LocaleScope::thread(){
 if(!identity())broken();
 EnterCriticalSection(&lock_);
 if(!local.scope){local.ptd={};local.scope=this;local.id=GetCurrentThreadId();local.next=static_cast<ThreadLocale*>(threads_);threads_=&local;}
 if(local.scope!=this)broken();
 LeaveCriticalSection(&lock_);return &local.ptd;
}
void LocaleScope::detach(void* pointer) noexcept {
 auto* t=static_cast<ThreadLocale*>(pointer);if(!identity()||t->scope!=this)broken();
 EnterCriticalSection(&lock_);auto* head=static_cast<ThreadLocale*>(threads_);ThreadLocale** link=&head;
 while(*link&&*link!=t)link=&(*link)->next;
 if(!*link)broken();*link=t->next;threads_=head;
 drop(t->ptd.locale);
 t->ptd.locale=nullptr;t->next=nullptr;t->scope=nullptr;t->id=0;LeaveCriticalSection(&lock_);
}
bool LocaleScope::close() noexcept {
 if(!claimed_)return true;
 if(!identity()||owner_!=GetCurrentThreadId())return false;
 EnterCriticalSection(&lock_);
 for(auto* t=static_cast<ThreadLocale*>(threads_);t;t=t->next)if(t->id!=owner_){LeaveCriticalSection(&lock_);return false;}
 if(local.scope==this)detach(&local);
 if(threads_)broken();
 active=nullptr;claimed_=false;nls_globals={};nls_bindings={};ansi_bindings={};
 LeaveCriticalSection(&lock_);DeleteCriticalSection(&lock_);owner_=0;return true;
}
LocaleScope::~LocaleScope(){if(!close())broken();}
}
namespace kotor_locale_release {extern const Dependencies dependencies={live_free,kotor_cleanup_00706547,kotor_cleanup_0070631b,kotor_cleanup_0070612c,&kotor_locale_shared_cells::money_00833fec,&kotor_locale_shared_cells::ref34_00834308,&kotor_locale_shared_cells::ref30_00834310,&kotor_locale_shared_cells::ref40_00834304,&kotor_locale_shared_cells::time_00833fe8};}
namespace kotor_locale_cleaners {extern const Dependencies dependencies={live_free,&kotor_locale_shared_cells::current_money_007a328c,&kotor_locale::initial_money_007a325c};}
namespace kotor_locale {extern const Dependencies dependencies={kotor_functional::get,kotor_release_locale_006fda5a,kotor_functional::lock,kotor_functional::unlock,nls_classify_007073f8,&global_locale_007a2824,&initial_locale_007a27d0};}
namespace kotor_lowercase {extern const Dependencies dependencies={kotor_functional::get,kotor_refresh_locale_006fdd2a,kotor_functional::classify,nls_map_0070272d,&kotor_locale::global_locale_007a2824};}
namespace gui_reference {Providers& reference_providers(){static Providers p={kotor_lower_006fb766};return p;}}
extern "C" __declspec(noreturn) void __cdecl nls_outside_contract(const char*){std::terminate();}
extern "C" __declspec(noreturn) void __cdecl ansi_outside_contract(const char*){std::terminate();}
