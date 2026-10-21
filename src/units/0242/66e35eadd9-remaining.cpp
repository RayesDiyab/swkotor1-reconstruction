#include "2e9ef02315-providers.h"
// lane-m48-b D81(4) REPLACEMENT COPY (only change: noexcept(false) on the 005E5A90 declaration) of
// lane-m47-core rev2 ADAPTED COPY of the M46 stage unit service-tail-22 (base sha256
// 785929e62806739c91282cb7e6ce7062f2330eff31973bc04e970790d2e86d95).
// D76 FIX: both bodies reached their callees through `services_tail5::bindings.dispatch` (service-tail-8's table, never
// filled: m47-compose D76-CHECK.json) and `locale_bindings` (never filled; the adapter 006FE34D raised a contract
// violation on a null boundary). Every address call is replaced by a direct typed call of the genuine provider and
// `locale_bindings` is filled statically with the real cells (object-pointer conversions only, no casts of functions).
//
// CSoundSetList::CSoundSetList 00678C10 (StartServices 005F939A, FIRST FRAME). Providers:
//   006FA7E6 live_operator_new          00406D80 menu_resources::Ref::construct_00406d80 (service-resource-13)
//   00413CC0 c2da_current_construct_00413cc0, 004143B0 c2da_current_load_004143b0, 00413270 c2da_current_get_string_00413270,
//   00413660 c2da_current_get_int_00413660 (lane-startup-misc donor-c2da, D38)
//   00401130 vector_constructor_iterator_00401130 (world-los) with element ctor kotor_item_drop_sounds::ref_default_element_00405ed0
//            (M46 alternatename join -> @c2da_default_ref_00405ed0@8)
//   005B3190 kotor_menu20::ConstructString005b3190, 005E5A90 frame_LogStringCtor_005e5a90, 005E5C20 StartupString::~StartupString
//   004061F0 async_ref_assign_string_004061f0 (next-ref-bodies), table slot 0 = c2da_current::TableSlot0::deleting.
//   .rdata 00752CC0 "SoundSet", 00752CB0 "SoundSetType", 00752CA8 "RESREF", 0073E3C4 "STRREF", 0074B76C "GENDER".
//
// setlocale 006FE34D (only caller 005F932D, reached only when the language id at 005F931C == 5, Polish): faithful body,
// providers = the staged locale family's typed table kotor_locale::dependencies (lock/unlock 00701A74/007019E0 index 12,
// release 006FDA5A), live_malloc 006FD8FB, live_free 006FB7B2, kotor_sync_locale_006fdb24 and the locale-shared-cells. OPEN (not authored here):
// __interpretLocale 006FE16E and the 14 CRT locale cells without an owner (kotor_crt_locale namespace below) -
// see PACKET-REV2.json open_gates (CRT locale family, ~78 fns).
#include "fb7fbde23c-table_contract.h"
#include "714f96481d-load.h"
#include "ab6dbc52ea-getstring.h"
#include "163d10ec4d-resources.h"
#include "84a5500766-startup_string.h"
#include "fba01a6b7c-cells.h"
#include <cstring>
#include <initializer_list>
namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); }
namespace kotor_item_drop_sounds { void* __fastcall ref_default_element_00405ed0(void* element, void*); }
extern "C" {
void* __cdecl live_operator_new(std::uint32_t);
void* __cdecl live_malloc(std::uint32_t);
void __cdecl live_free(void*);
void __stdcall vector_constructor_iterator_00401130(void* base, std::uint32_t element_size, std::uint32_t count,
                                                    void* (__fastcall* construct)(void*, void*));
void __fastcall frame_LogStringCtor_005e5a90(kotor_reconstruction::StartupString*, void*, const char*) noexcept(false); // D81(4): definition (r4_log_joins.cpp:36) is noexcept(false); extern "C" under /EHsc is otherwise assumed nothrow
menu_resources::Ref* __fastcall async_ref_assign_string_004061f0(menu_resources::Ref*, void*, menu_resources::Ref*,
                                                                 menu_resources::String*);
__declspec(noinline) std::int32_t __fastcall c2da_current_get_int_00413660(c2da_current::Table*, void*, std::int32_t,
                                                                         c2da_current::String*, std::int32_t*);
}
namespace services_tail5 {
// Boundary record the 006FE34D adapter inspects before and after the call (never set: the body has no held branch).
static Boundary locale_boundary{};
// [M47 compose D79b] setlocale 006FE34D is the UCRT adapter m47-joins/setlocale_006fe34d.cpp; the 20 original CRT
// locale snapshot words are no longer read by any staged body (only the router reads `boundary`).
LocaleBindings locale_bindings{{},
 reinterpret_cast<U32*>(const_cast<kotor_locale::Locale**>(&kotor_locale::global_locale_007a2824)),
 reinterpret_cast<U32>(&kotor_locale::initial_locale_007a27d0),
 &locale_boundary};

using c2da_current::Table;
using kotor_reconstruction::StartupString;
static U32 addr(const void* p){return reinterpret_cast<U32>(p);}
static Table* construct_table(const char* name){
 void* const allocation=live_operator_new(0x54);if(!allocation)return nullptr;                 // 00678C36..58
 menu_resources::Ref ref;ref.construct_00406d80(name);                                           // 00678C5A..69
 return c2da_current_construct_00413cc0(allocation,nullptr,ref,0);                               // 00678C6E..70
}
static void destroy_table(Table* object){                                                         // mov eax,[t]; push 1; call [eax]
 const c2da_current::TableSlot0* methods;std::memcpy(&methods,object,4);methods->deleting(object,nullptr,1);
}
static std::uint16_t count_at(const void* self){std::uint16_t n;std::memcpy(&n,self,2);return n;}
static void count_store(void* self,std::uint16_t n){std::memcpy(self,&n,2);}
static void text(StartupString* s,const char* t){frame_LogStringCtor_005e5a90(s,nullptr,t);}
SoundSetList* __thiscall SoundSetList::Construct(){
 words[1]=words[2]=words[3]=0;count_store(this,0); // word+2 padding untouched                    // 00678C38..41
 Table* const first=construct_table("SoundSet");                                                  // 00752CC0
 if(!first||!c2da_current_load_004143b0(first,nullptr))return this; // observed no cleanup on this branch
 Table* const second=construct_table("SoundSetType");                                             // 00752CB0
 if(!second){destroy_table(first);return this;}                                                   // 00678CE9..F0
 if(!c2da_current_load_004143b0(second,nullptr)){destroy_table(first);return this;} // second not deleted (00678D02..E86)



 U32 n=load(addr(first),0x24)&0xffffu;                                                            // 00678D0C..0F

 count_store(this,static_cast<std::uint16_t>(n));                                                 // 00678D12
 void* const refs=live_operator_new(n<<4);U32 scratch=addr(refs);                                 // 00678D15..25
 if(refs)vector_constructor_iterator_00401130(refs,16,n,&kotor_item_drop_sounds::ref_default_element_00405ed0); // 00678D35..3E
 words[1]=addr(refs);                                                                             // 00678D53
 words[2]=addr(live_operator_new(count_at(this)));                                                // 00678D47..62
 words[3]=addr(live_operator_new(static_cast<U32>(count_at(this))<<2));                           // 00678D5B..72
 // Nonzero-count contract requires valid output arrays, as the original has
 // unconditional stores after these allocations. No invented null fallback.
 for(U32 i=0;static_cast<std::uint16_t>(i)<count_at(this);++i){
  alignas(4) unsigned char value_bytes[8],column_ref[8],column_str[8],column_gender[8],out_ref[16];
  StartupString* const value=reinterpret_cast<StartupString*>(value_bytes);
  kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(value_bytes),nullptr);  // 00678D80..84
  const std::int32_t row=static_cast<std::uint16_t>(i);
  text(reinterpret_cast<StartupString*>(column_ref),"RESREF");                                    // 00678D89..9A
  c2da_current_get_string_00413270(first,nullptr,row,reinterpret_cast<StartupString*>(column_ref),value); // 00678D9F..B4
  reinterpret_cast<StartupString*>(column_ref)->~StartupString();                                  // 00678DB9..C2
  async_ref_assign_string_004061f0(reinterpret_cast<menu_resources::Ref*>(load(addr(this),4)+(static_cast<U32>(row)<<4)),
      nullptr,reinterpret_cast<menu_resources::Ref*>(out_ref),value);                              // 00678DC7..DB ([edi+4]+row*16)
  text(reinterpret_cast<StartupString*>(column_str),"STRREF");                                    // 00678DE0..E9
  std::int32_t value_int;std::memcpy(&value_int,&scratch,4);
  c2da_current_get_int_00413660(first,nullptr,row,reinterpret_cast<StartupString*>(column_str),&value_int); // 00678DEE..E00
  std::memcpy(&scratch,&value_int,4);
  reinterpret_cast<StartupString*>(column_str)->~StartupString();                                  // 00678E05..0E
  store(load(addr(this),0x0c),static_cast<U32>(row)*4,scratch);                                   // 00678E13..1A
  text(reinterpret_cast<StartupString*>(column_gender),"GENDER");                                 // 00678E1D..26
  std::memcpy(&value_int,&scratch,4);
  c2da_current_get_int_00413660(first,nullptr,row,reinterpret_cast<StartupString*>(column_gender),&value_int); // 00678E2B..3D
  std::memcpy(&scratch,&value_int,4);
  reinterpret_cast<StartupString*>(column_gender)->~StartupString();                               // 00678E42..4B
  unsigned char gender=static_cast<unsigned char>(scratch);
  std::memcpy(reinterpret_cast<void*>(load(addr(this),8)+static_cast<U32>(row)),&gender,1);        // 00678E50..57
  value->~StartupString();                                                                         // 00678E5A..66
 }
 destroy_table(first);destroy_table(second);return this;                                          // 00678E75..86
}

// [M47 compose D79b] Locale (setlocale 006FE34D) body removed: single body = UCRT typed adapter m47-joins/setlocale_006fe34d.cpp
}
