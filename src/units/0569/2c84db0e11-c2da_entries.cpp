// lane-startup-misc: C2DA (swkotor.exe GOG K1, base 0x400000) column lookup and
// FLOAT/INT entry readers, the three CRes2DA lowers the reused task-10 donors import,
// the Load2DArray literal accessors, and the D1 options_video ports required by
// lane-menu-tables d1_table_options_video.cpp (texture_bindings slots 00413CC0..00413660).
//
// Canonical bodies use the task-10 donor epoch (c2da_current::Table = Table84 with the
// actual Helper first subobject, StartupString cells). The D1 ports project the
// options_video 0x54/8-byte views onto those objects (the Table object is created in
// the caller's 0x54 storage by the constructor below; later calls reach it through
// std::launder), the same placement/launder convention as lane-menu-tables' ports.
#include "e76b1c7603-getstring.h"        // donor: c2da_current::Table/String, column/index imports
#include "dd2c538ad1-load_imports.h"     // donor: Load imports (literal accessors declared here)
#include "b8324a80af-resource_lower.h"   // donor: CRes2DA lower declarations (0041D730 ctor, 00406460 base ctor, 0073EC9C table)
#include "c7e191db10-native_strings.h"   // WindowCStr005e5670 (M41 window-native-strings.obj)
#include "66a5366142-menu_targets.h"     // kotor_menu20::String::as_int_005e5630 (M41 service-menu20-4.obj)
#include "866a226af7-texture.h"          // options_video::{Table,String,ResourceRef,I32} (D1 epoch)
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <new>
#include <type_traits>

using c2da_current::String;
using c2da_current::Table;
using c2da_current::Word;

// ---- genuine providers owned elsewhere (identical declarations) ----
extern "C" {
// lane-resource-lowers base_extension_type.cpp (005E6450 CExoString::CompareNoCase, RET 4).
__declspec(noinline) std::int32_t __fastcall exostring_compare_no_case_005e6450(String*,void*,String*);
// M41 string-compare-existing.obj (0070ACAF _stricmp, cdecl).
__declspec(noinline) std::int32_t __cdecl kotor_stricmp_0070acaf(const char*,const char*);
// lane-font txi_readers.cpp (006FAE08 sscanf, cdecl varargs; UCRT-backed, lane-font sscanf_analysis).
int __cdecl kotor_sscanf_006fae08(const char*,const char*,...);
}
// lane-sound audio_retyped.cpp (005E5650 CExoString::AsFLOAT: ECX String*, result ST0).
namespace kotor_sound_path { float __fastcall StringAsFloat005e5650(kotor_reconstruction::StartupString*,void*); }

namespace {
static_assert(std::is_same<String,kotor_reconstruction::StartupString>::value,"canonical 8-byte CExoString");
inline unsigned char* bytes_at(Word address){return reinterpret_cast<unsigned char*>(static_cast<std::uintptr_t>(address));}
inline Word word_at(Word address){Word v;std::memcpy(&v,bytes_at(address),4);return v;}
inline std::uint16_t half_at(Word address){std::uint16_t v;std::memcpy(&v,bytes_at(address),2);return v;}
inline std::int32_t signed_word(const Table* t,unsigned offset){const Word w=c2da_current::load_word(t,offset);std::int32_t v;std::memcpy(&v,&w,4);return v;}
// [[this+38h]+row*4]+column*8: re-read at every use (00413703/00413715/00413727/0041373A/00413751/00413778).
inline String* cell(const Table* t,Word row,Word column){
    return std::launder(reinterpret_cast<String*>(bytes_at(word_at(c2da_current::load_word(t,0x38)+row*4u)+column*8u)));
}
// The same 8-byte CExoString object seen through the M41 AsINT owner's receiver type.
inline std::int32_t as_int(String* s){return std::launder(static_cast<kotor_menu20::String*>(static_cast<void*>(s)))->as_int_005e5630();}
// Original immutable literals (pushed as immediates by the instructions cited at each use).
const char empty_0073d71c[]="";
const char float_format_0073e2e4[]="%f";
const char hex_format_0073e2e8[]="%x";
const char int_format_0073e2ec[]="%i";
const char default_colon_0073e30c[]="DEFAULT:";   // 9 compared bytes (donor load.cpp)
const char default_0073e304[]="DEFAULT";          // 8 compared bytes
const char missing_0073e2fc[]="****";             // 5 compared bytes
static_assert(sizeof(default_colon_0073e30c)==9&&sizeof(default_0073e304)==8&&sizeof(missing_0073e2fc)==5,"compared extents");
}

// ---- 00413100 C2DA::GetColumnIndex(CExoString*): ECX table, RET 4, EAX index or -1 ----
extern "C" __declspec(noinline) std::int32_t __fastcall
c2da_current_column_index_00413100(Table* self,void*,String* column){
    Word index=0;                                                            // 00413108 xor edi,edi
    if(c2da_current::load_word(self,0x3c)!=0){                               // 00413104/0041310A packed form
        if(signed_word(self,0x28)<=0)return -1;                              // 00413111/00413113 -> 00413149
        do{
            const char* text=WindowCStr005e5670(column,nullptr);             // 00413120..00413122 (every pass)
            const Word label=c2da_current::load_word(self,0x48)+half_at(c2da_current::load_word(self,0x50)+index*2u); // 00413127..00413132
            if(kotor_stricmp_0070acaf(reinterpret_cast<const char*>(bytes_at(label)),text)==0) // 00413135..0041313F
                return static_cast<std::int32_t>(index);                     // 00413181
            ++index;                                                         // 00413144
        }while(static_cast<std::int32_t>(index)<signed_word(self,0x28));     // 00413141..00413147 live reload
        return -1;                                                           // 0041314B
    }
    if(signed_word(self,0x28)<=0)return -1;                                  // 00413152/00413154
    do{
        String* label=std::launder(reinterpret_cast<String*>(bytes_at(c2da_current::load_word(self,0x34)+index*8u))); // 00413160..00413164
        if(exostring_compare_no_case_005e6450(label,nullptr,column)!=0)       // 00413167..0041316E
            return static_cast<std::int32_t>(index);                         // 00413181
        ++index;                                                             // 00413173
    }while(static_cast<std::int32_t>(index)<signed_word(self,0x28));         // 00413170..00413176 live reload
    return -1;                                                               // 0041317A
}

// ---- 00413430 C2DA::GetFLOATEntry(int row, CExoString* column, float* out): RET 0Ch, EAX 0/1 ----
extern "C" __declspec(noinline) std::int32_t __fastcall
c2da_current_get_float_00413430(Table* self,void*,std::int32_t row,String* column,float* out){
    const std::int32_t index=c2da_current_column_index_00413100(self,nullptr,column);    // 00413437..0041343A first
    if(row<0||row>=signed_word(self,0x24)||index==-1){                                     // 00413443..00413457
        *out=kotor_sound_path::StringAsFloat005e5650(c2da_current::default_string(self),nullptr); // 004134F1..004134FE (+1C)
        return 0;                                                                          // 00413501
    }
    const Word row_bits=static_cast<Word>(row),index_bits=static_cast<Word>(index);
    if(c2da_current::load_word(self,0x3c)!=0){                                             // 0041345D..00413462
        const Word at=c2da_current::load_word(self,0x28)*row_bits+index_bits;              // 00413464..0041346D (DWORD wrap)
        const Word offset=half_at(c2da_current::load_word(self,0x44)+at*2u);               // 00413473 movzx
        const Word zero=0;std::memcpy(out,&zero,4);                                        // 00413478 dword store before the scan
        (void)kotor_sscanf_006fae08(reinterpret_cast<const char*>(bytes_at(c2da_current::load_word(self,0x40)+offset)),
                                    float_format_0073e2e4,out);                            // 0041347E..00413489 result ignored
        return *bytes_at(c2da_current::load_word(self,0x40)+offset)!=0?1:0;                // 0041348E..0041349D [+40] reloaded
    }
    if(cell(self,row_bits,index_bits)->BorrowedEquals005e5290(empty_0073d71c)!=0){         // 004134A4..004134BF
        const Word zero=0;std::memcpy(out,&zero,4);                                        // 004134C7
        return 0;                                                                          // 004134CD
    }
    *out=kotor_sound_path::StringAsFloat005e5650(cell(self,row_bits,index_bits),nullptr);  // 004134D3..004134E5 fstp dword
    return 1;                                                                              // 004134E8
}

// ---- 00413660 C2DA::GetINTEntry(int row, CExoString* column, int* out): RET 0Ch, EAX 0/1 ----
extern "C" __declspec(noinline) std::int32_t __fastcall
c2da_current_get_int_00413660(Table* self,void*,std::int32_t row,String* column,std::int32_t* out){
    const std::int32_t index=c2da_current_column_index_00413100(self,nullptr,column);    // 00413667..0041366A first
    if(row<0||row>=signed_word(self,0x24)||index==-1){                                     // 00413673..00413687
        *out=as_int(c2da_current::default_string(self));                                   // 00413796..004137A4 (+1C)
        return 0;                                                                          // 004137A6
    }
    const Word row_bits=static_cast<Word>(row),index_bits=static_cast<Word>(index);
    if(c2da_current::load_word(self,0x3c)!=0){                                             // 0041368D..00413692
        const Word at=c2da_current::load_word(self,0x28)*row_bits+index_bits;              // 00413694..0041369D
        const Word offset=half_at(c2da_current::load_word(self,0x44)+at*2u);               // 004136A3
        *out=0;                                                                            // 004136A8
        (void)kotor_sscanf_006fae08(reinterpret_cast<const char*>(bytes_at(c2da_current::load_word(self,0x40)+offset)),
                                    int_format_0073e2ec,out);                              // 004136AE..004136B9 "%i"
        return *bytes_at(c2da_current::load_word(self,0x40)+offset)!=0?1:0;                // 004136BE..004136CD
    }
    if(cell(self,row_bits,index_bits)->BorrowedEquals005e5290(empty_0073d71c)!=0){         // 004136D4..004136EF
        *out=0;                                                                            // 004136F7
        return 0;                                                                          // 004136FD
    }
    if(cell(self,row_bits,index_bits)->BorrowedLength005e5790()>2){                        // 00413703..00413713 signed
        if(WindowCStr005e5670(cell(self,row_bits,index_bits),nullptr)[0]=='0'&&            // 00413715..00413725
           (WindowCStr005e5670(cell(self,row_bits,index_bits),nullptr)[1]=='x'||           // 00413727..00413738
            WindowCStr005e5670(cell(self,row_bits,index_bits),nullptr)[1]=='X')){          // 0041373A..0041374B
            (void)kotor_sscanf_006fae08(WindowCStr005e5670(cell(self,row_bits,index_bits),nullptr),
                                        hex_format_0073e2e8,out);                          // 0041374D..00413765 "%x", no pre-store
            return 1;                                                                      // 0041376F
        }
    }
    *out=as_int(cell(self,row_bits,index_bits));                                           // 00413778..0041378B
    return 1;                                                                              // 0041378D
}

// ---- 0041D730 CRes2DA::CRes2DA, imported by the donor SetResRef 00413B40 (binding.cpp): verbatim
// single-function extraction from 2026-10-04 task-4 service-composition-r4 source/resource_lower.cpp
// (= 2026-10-02 task-10 c2da-current-resource-lower-r1), re-verified against the original below. The other
// functions of that donor file have single owners elsewhere (D9 00405ED0, D18 lane-resman
// 00409B00/00409B10/00409B80/00409CF0/00406060, lane-tpc stage tpc_lowers.cpp 00404DD0/00710E50).
namespace {
template<class T>void store(void* p,unsigned offset,T value){std::memcpy(static_cast<unsigned char*>(p)+offset,&value,sizeof(value));}
}
// 0041D730: 00406460 base ctor, zero +28..+38, vtable 0073EC9C (CRes2DA), EAX=this.
extern "C" __declspec(noinline) void* __fastcall c2da_resource_ctor_0041d730(void* storage,void*){
    (void)c2da_resource_base_ctor_00406460(storage,nullptr);                               // 0041D733
    for(unsigned offset:{0x28u,0x2cu,0x30u,0x34u,0x38u})store(storage,offset,Word{0});     // 0041D738..0041D746
    store(storage,0,c2da_resource_methods_0073ec9c());                                     // 0041D749
    return storage;                                                                        // 0041D74F
}

// ---- Load2DArray literal accessors (donor load_imports.h contract; bytes read from the pinned exe .rdata) ----
extern "C" const char* __cdecl c2da_load_literal_0073e30c(){return default_colon_0073e30c;}
extern "C" const char* __cdecl c2da_load_literal_0073e304(){return default_0073e304;}
extern "C" const char* __cdecl c2da_load_literal_0073e2fc(){return missing_0073e2fc;}

// ---- D1 ports: lane-menu-tables texture_bindings slots (options_video views) ----
namespace menu_tables_required {
namespace {
static_assert(sizeof(options_video::Table)==sizeof(Table)&&alignof(Table)<=alignof(options_video::Table),"0x54 storage hosts Table84");
static_assert(sizeof(options_video::String)==sizeof(String)&&sizeof(options_video::ResourceRef)==sizeof(c2da_current::Ref),"8-byte String, 16-byte CResRef");
inline Table* T(options_video::Table* t){return std::launder(static_cast<Table*>(static_cast<void*>(t)));}
inline String* S(options_video::String* s){return std::launder(static_cast<String*>(static_cast<void*>(s)));}
}
// 00413CC0: ECX storage, CResRef by value in [esp+4..+13], flag [esp+14], RET 14h, EAX=this.
// Tail jump keeps the caller's by-value CResRef stack slot (SetResRef 00413B40 receives its address, 00413D43).
__declspec(naked) options_video::Table* __fastcall C2daConstruct_00413cc0(options_video::Table*,void*,options_video::ResourceRef,options_video::I32){
    __asm jmp c2da_current_construct_00413cc0
}
options_video::I32 __fastcall C2daLoad_004143b0(options_video::Table* t,void*){return c2da_current_load_004143b0(T(t),nullptr);}
void __fastcall C2daUnload_004139e0(options_video::Table* t,void*){c2da_current_unload_004139e0(T(t),nullptr);}
options_video::I32 __fastcall C2daStringEntry_00413270(options_video::Table* t,void*,options_video::I32 row,options_video::String* column,options_video::String* out){
    return c2da_current_get_string_00413270(T(t),nullptr,row,S(column),S(out));
}
options_video::I32 __fastcall C2daFloatEntry_00413430(options_video::Table* t,void*,options_video::I32 row,options_video::String* column,float* out){
    return c2da_current_get_float_00413430(T(t),nullptr,row,S(column),out);
}
options_video::I32 __fastcall C2daIntEntry_00413660(options_video::Table* t,void*,options_video::I32 row,options_video::String* column,options_video::I32* out){
    return c2da_current_get_int_00413660(T(t),nullptr,row,S(column),out);
}
}
static_assert(std::is_same<decltype(&menu_tables_required::C2daConstruct_00413cc0),decltype(options_video::TextureBindings::construct_table_00413cc0)>::value,"+18 slot type");
static_assert(std::is_same<decltype(&menu_tables_required::C2daLoad_004143b0),decltype(options_video::TextureBindings::load_table_004143b0)>::value,"slot type");
static_assert(std::is_same<decltype(&menu_tables_required::C2daUnload_004139e0),decltype(options_video::TextureBindings::unload_table_004139e0)>::value,"slot type");
static_assert(std::is_same<decltype(&menu_tables_required::C2daStringEntry_00413270),decltype(options_video::TextureBindings::string_entry_00413270)>::value,"slot type");
static_assert(std::is_same<decltype(&menu_tables_required::C2daFloatEntry_00413430),decltype(options_video::TextureBindings::float_entry_00413430)>::value,"slot type");
static_assert(std::is_same<decltype(&menu_tables_required::C2daIntEntry_00413660),decltype(options_video::TextureBindings::int_entry_00413660)>::value,"slot type");

// ---- link-name joins (D9 precedent: ABI-identical existing owners, no second body) ----
// 004060B0 CResRef::operator== (ECX Ref*, one Ref* stack word, RET 4): M41 resource-ref-equality-join.obj.
#pragma comment(linker,"/alternatename:@c2da_equal_ref_004060b0@12=@resource_equal_004060b0@12")
// 005E5600 CExoString::operator[] (ECX String*, int, RET 4, AL): lane-resource-lowers directory_list.cpp.
#pragma comment(linker,"/alternatename:@c2da_current_string_index_005e5600@12=?At005e5600@directory_list@@YIDPAUStartupString@kotor_reconstruction@@PAXH@Z")
// 00406120 CResRef::operator= (ECX Ref*, out Ref*, source Ref*, RET 8): task-2 assign6120-r3 (lane-tpc stage assign6120).
#pragma comment(linker,"/alternatename:@c2da_copy_00406120@16=?assign_00406120@resource_ref_assignment@@YIPAURef@menu_resources@@PAU23@PAX0PBU23@@Z")
