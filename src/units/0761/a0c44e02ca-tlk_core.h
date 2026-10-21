#pragma once
// lane-tlk: CTlkTable / CTlkFile subsystem of swkotor.exe (GOG K1, image base 0x400000).
// Nominal model (see lane-tlk/PACKET.json "nominal_decision"):
//  * kotor_menu_10_19::{String,ResRef,StringResource,Tlk} stay the single M40 byte-view
//    definitions (startup_views.h 1b04c683, unchanged); consumers keep their names.
//  * TLK bodies operate on the canonical owners kotor_reconstruction::StartupString and
//    menu_resources::Ref through tlk_core::StringResource (distinct name, same 28-byte
//    layout). The M40 member ?fetch_0041e1a0@Tlk... reaches FetchInternal0041e1a0 by one
//    ABI-identical /alternatename row (thiscall RET0Ch == fastcall ECX,EDX-unused,RET0Ch).
//  * Every original thiscall is exposed as __fastcall(receiver, unused EDX, ...) (D1).
#include "4b90ecc798-startup_string.h"
#include "db0cf476b8-startup_views.h"
#include "8f3ecc31f0-resources.h"
#include "cdb0979b94-files.h"
#include "c36ec7ee7b-table_contract.h"     // c2da_current::Table (task-10 donor epoch, staged by lane-startup-misc)
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <type_traits>
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error Pinned MSVC x86 reconstruction.
#endif

namespace tlk_core {
using Word=std::uint32_t;
using String=kotor_reconstruction::StartupString;
using Ref=menu_resources::Ref;
using Tlk=kotor_menu_10_19::Tlk;            // 0x64 bytes (0041D8D0 ctor, M40 retained-11)
using TlkFile=tlk_file::TlkFile;            // CExoFile4 + header[5] (0041D810 / alloc 18h at 0041D9B3)

// STR_RES: 0041E8F0 local (+0 CExoString 005B3190 at 0041E918, +8 CResRef 00405ED0 at
// 0041E929), 005EA130 local (sub esp,1Ch at 005EA149), FetchInternal writes +18h
// (0041E344/0041E350). Layout identical to kotor_menu_10_19::StringResource.
struct StringResource { String text; Ref sound; Word duration_bits; };
static_assert(sizeof(StringResource)==28 && offsetof(StringResource,text)==0 &&
              offsetof(StringResource,sound)==8 && offsetof(StringResource,duration_bits)==24,
              "observed STR_RES layout");
static_assert(sizeof(StringResource)==sizeof(kotor_menu_10_19::StringResource) &&
              offsetof(kotor_menu_10_19::StringResource,sound)==offsetof(StringResource,sound) &&
              offsetof(kotor_menu_10_19::StringResource,duration_bits)==offsetof(StringResource,duration_bits),
              "same physical STR_RES as the M40 nominal view joined at link time");

// CTlkTable +40h token row, stride 24h (0041E66B lea edx,[eax+eax*8]; *4+4 cookie).
// Element ctor 0041D910 constructs only the CExoString at +4 (0041D913..0041D916);
// element dtor 0059EA20 destroys only that CExoString (add ecx,4; jmp 005E5C20).
// Array storage uses the original allocator family (006FA7E6 / 006FA390) with the
// 4-byte element-count cookie written at 0041E6A4 and read at 0041E5DD.
struct Token {
    Word hash;            // +00 00410560(text,len)       0041E77A
    String text;          // +04 C2DA column 0            0041E6DF
    Word column1;         // +0C C2DA column 1            0041E703
    Word columns3[4];     // +10 C2DA columns 3..6        0041E743
    Word column2;         // +20 C2DA column 2            0041E71B
    Token() {}            // 0041D910: only +4 is constructed; other words indeterminate
    static void* operator new[](std::size_t bytes) noexcept;     // 006FA7E6 (0041E67B)
    static void operator delete[](void* cookie) noexcept;        // 006FA390 (0041E5F2)
};
static_assert(sizeof(Token)==0x24 && offsetof(Token,text)==4 && offsetof(Token,column1)==0x0c &&
              offsetof(Token,columns3)==0x10 && offsetof(Token,column2)==0x20,"observed token row");

template<class T> inline T get(const void* object,unsigned offset) {
    T value; std::memcpy(&value,static_cast<const unsigned char*>(object)+offset,sizeof(T)); return value;
}
template<class T> inline void put(void* object,unsigned offset,T value) {
    std::memcpy(static_cast<unsigned char*>(object)+offset,&value,sizeof(T));
}

// ---- lane-tlk exports (C++ linkage: provider exceptions stay inside the /EHsc model) ----
// 0041E1A0 CTlkTable::FetchInternal(strref, STR_RES*, int parse) RET0Ch
int __fastcall FetchInternal0041e1a0(Tlk* self,void* unused_edx,Word strref,StringResource* output,int parse);
// 0041E5A0 CTlkTable::OpenFile(CExoString const&) RET4
Word __fastcall OpenFile0041e5a0(Tlk* self,void* unused_edx,String* name);
// 0041D920 CTlkTable::AddFile(CExoString const&, int slot) RET8
Word __fastcall AddFile0041d920(Tlk* self,void* unused_edx,String* name,int slot);
// 0041D810 CTlkFile::CTlkFile(CExoString const&) RET4 ; 0041D890 CTlkFile::ReadHeader() RET
TlkFile* __fastcall ConstructFile0041d810(TlkFile* self,void* unused_edx,String* name);
Word __fastcall ReadHeader0041d890(TlkFile* self,void* unused_edx);
// 00410560 hash(char const*, unsigned) cdecl (CRC table 007A4178)
Word __cdecl Hash00410560(const char* text,Word length);
// 005F4180 CClientExoAppInternal::AddDownloadedResources() RET
void __fastcall AddDownloadedResources005f4180(void* self,void* unused_edx);
// 0041DD30 CTlkTable::ParseStr(CExoString&) RET4 (tlk_parse.cpp)
void __fastcall ParseStr0041dd30(Tlk* self,void* unused_edx,String* text);

// ---- CTlkTable / CSWTlkTable virtual tables (D1 slot form, D21 typed constant data) ----
// Each table has exactly one slot: CTlkTable 0073ECDC = {005B5E90} (next word 0073ECE0 is the
// "F" literal); CSWTlkTable 0074E700 = {00674B90} (next word 0074E704 is literal text).
// Slot0 is the token-value hook called by ParseStr at 0041E006 (push &value; push token; call [edx]).
struct TlkMethods { void (__fastcall* token_value)(Tlk* self,void* unused_edx,Token* token,String* value); };
static_assert(sizeof(TlkMethods)==4,"single observed slot");
// 005B5E90 `ret 8` (Ghidra void_return; COMDAT-folded empty virtual) in the CTlkTable slot.
void __fastcall TokenValueBase005b5e90(Tlk* self,void* unused_edx,Token* token,String* value);
// 00674B90 CSWTlkTable::GetTokenValue (game-object token switch, 0x940 bytes) -- not authored here.
void __fastcall SwTokenValue00674b90(Tlk* self,void* unused_edx,Token* token,String* value);
// Typed dispatch: the receiver's vptr word is either a typed table address (lane-tlk tables) or
// the original vptr value written by the M40 sources (retained-11 0041D8D0: 0073ECDC; start-body
// 005F899B: 0074E700). Anything else is a contract violation (no Word->function-pointer cast).
const TlkMethods* MethodsOf(const Tlk* self);
}

extern "C" {
// ---- lane-tlk owned cells / original data ----
extern void* kotor_cell_tlk_007a3a08;                      // .data CTlkTable* g_pTlkTable
extern const char* kotor_cell_tlk_version_0078d3ec;        // .data -> "V3.0" (0073ECD4)
extern const unsigned char kotor_rdata_live_count_0073ecd2;// .rdata byte 07h
// 0041E550 CTlkTable::Fetch(strref, STR_RES*, u8 gender) RET0Ch (lane-resman consumer name)
tlk_core::Word __fastcall tlk_fetch_0041e550(tlk_core::Tlk* self,void* unused_edx,tlk_core::Word strref,
                                             tlk_core::StringResource* output,tlk_core::Word gender);
// StartServices router entries (cdecl, client_services_start::Call*).
// Declared in service_router.cpp / bridges.h; defined in tlk_open.cpp / tlk_download.cpp.

// ---- providers owned elsewhere, not declared by a stage header ----
// lane-resman (D18): 005E5B70 CExoString(char const*,int) RET8 / 005E5BC0 CExoString(int) RET4.
kotor_reconstruction::StartupString* __fastcall resman_string_counted_005e5b70(
    kotor_reconstruction::StartupString* self,void* unused_edx,const char* text,int length);
kotor_reconstruction::StartupString* __fastcall resman_string_from_int_005e5bc0(
    kotor_reconstruction::StartupString* self,void* unused_edx,int value);
// 00406170 CResRef::operator=(CResRef const&) by value, RET8 (donor task-2 refcopy6170-r1,
// staged by this lane as ref_copy_00406170.cpp).
menu_resources::Ref* __fastcall SoundAssign00406170(menu_resources::Ref* self,void* unused_edx,
    menu_resources::Ref* out,const menu_resources::Ref* input);
// C2DA: 00413CC0 / 004143B0 / 00413D80 are declared by table_contract.h / load.h (task-10 donors,
// staged by lane-startup-misc). The two (row,column) getters are authored here (c2da_rowcol.cpp).
int __fastcall c2da_current_load_004143b0(c2da_current::Table*,void*);                       // load.h, RET
std::int32_t __fastcall c2da_current_get_string_entry_00413190(c2da_current::Table* self,void* unused_edx,
    std::int32_t row,std::int32_t column,kotor_reconstruction::StartupString* out);           // RET0Ch
std::int32_t __fastcall c2da_current_get_int_entry_00413510(c2da_current::Table* self,void* unused_edx,
    std::int32_t row,std::int32_t column,std::int32_t* out);                                  // RET0Ch
// 006FAFF0 CRT strncmp (cdecl; returns -1/0/+1), authored in tlk_parse.cpp.
int __cdecl kotor_strncmp_006faff0(const char* first,const char* second,tlk_core::Word count);
// lane-tlk typed vtables.
extern const tlk_core::TlkMethods kotor_vtable_ctlktable_0073ecdc;
extern const tlk_core::TlkMethods kotor_vtable_cswtlktable_0074e700;
// CRC table owned by M41 canonical-crc-storage.obj, filled by 00410520 (CExoResMan ctor 00409C79).
extern std::uint32_t kotor_gff_crc_table_007a4178[256];
}
