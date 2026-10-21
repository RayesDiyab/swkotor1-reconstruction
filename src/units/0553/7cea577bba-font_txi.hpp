#pragma once
// lane-font packets 5/6: texture TXI parse, FontInfo parse/getter/dtor, TXI readers, sscanf
// provider, image math, hash, ordered insertion.
// header_roots (stage-relative, no absolute includes here):
//   lane-font/source                                   (this header)
//   lane-texture-epoch/source/texture-epoch/include    (resource.hpp, texture_vtable.hpp, texture.hpp; epoch E1)
//   <M40 stage>/viewport-integration/dependencies/list-growth (growth.h, menu_list_growth::allocate_005e03c0)
#include "9ebac881e9-resource.hpp"
#include "d3ac7e1daf-texture_vtable.hpp"
#include "21e45d1112-growth.h"
#include <cstdint>
#include <cstddef>
#include <type_traits>

namespace font_txi {
using U=texture::U;using I=texture::I;using Basic=texture::Basic;using FontInfo=texture::FontInfo;using List=texture::List;
static_assert(std::is_same<List,menu_list_growth::List>::value,"one ArrayMutations nominal for 005E03C0");
static_assert(offsetof(Basic,f18)==0x18&&offsetof(Basic,f34)==0x34&&offsetof(Basic,info)==0x38&&offsetof(Basic,f4c)==0x4c&&
              offsetof(Basic,b8)==0xb8&&offsetof(Basic,ba)==0xba&&offsetof(Basic,bc)==0xbc&&offsetof(Basic,c0)==0xc0&&
              offsetof(Basic,c2)==0xc2&&offsetof(Basic,c4)==0xc4&&offsetof(Basic,c6)==0xc6&&offsetof(Basic,c8)==0xc8&&
              offsetof(Basic,ca)==0xca&&offsetof(Basic,hash)==0xd0&&offsetof(Basic,d2)==0xd2&&offsetof(Basic,d3)==0xd3&&
              offsetof(Basic,d4)==0xd4&&offsetof(Basic,flags)==0xd8,"ParseField 00422390 / AddToOrderedLists 00421560 field offsets");
static_assert(sizeof(FontInfo)==48&&sizeof(List)==12,"FontInfo: +00 numchars, +04..+14 five floats, +18/+24 Vector lists");

// Typed views (inferred from reader formats). Work for both the E1 U fields and a future float/int epoch.
template<class T> volatile float* AsFloat(T& field){
 if constexpr(std::is_same<std::remove_cv_t<T>,float>::value)return &field;
 else return reinterpret_cast<volatile float*>(&field);
}
template<class T> volatile std::int32_t* AsInt(T& field){
 if constexpr(std::is_same<std::remove_cv_t<T>,std::int32_t>::value)return &field;
 else return reinterpret_cast<volatile std::int32_t*>(&field);
}
inline volatile bool* AsBool(volatile std::uint8_t& field){return reinterpret_cast<volatile bool*>(&field);}
// E1 edit LF1: FontInfo is typed (texture.hpp); +18 / +24 Vector lists by member, no word-array cast.
inline List* InfoList(FontInfo* info,unsigned word){return word==6?&info->upper_left_coords:&info->lower_right_coords;}

// Texture receiver slots +00..+AC: typed through texture_vtable::head() (lane-texture-epoch E-D2 Head view).

// Typed table of the +34 procedure object (classes 004630B0/00462A70/00463320/00463260/00463470, vtables
// 0073ED54/0073ED70/0073ED90/0073EDAC). Only the two slots 00422390 calls are typed (inferred from the call
// sites: thiscall RET4); +04/+08 are layout only and never called here.
struct ProcedureObject;
struct ProcedureVTable {
 void (__fastcall* s00)(ProcedureObject*,void*,U);          // deleting destructor, push 1 (004223FF..00422403)
 const void* s04;
 const void* s08;
 U (__fastcall* s0c)(ProcedureObject*,void*,const char*);   // per-line parse, push line (00422ABE..00422AC1); raw EAX
};
struct ProcedureObject {const ProcedureVTable* volatile vtable;};

// Procedural-texture providers used only by 'proceduretype' (absent from fnt_console TXI).
struct ProcedureBindings {
 void** life_vtable_0073ed54;
 void** perlin_vtable_0073ed70;
 void** random_vtable_0073ed90;
 void** ringtexdistort_vtable_0073edac;
};
extern const ProcedureBindings& procedure_bindings;
}

extern "C" {
// ---- TXI readers (names from task-17 material-init-r1 ports.h; bodies authored in txi_readers.cpp) ----
 char* __cdecl firstword_00463530(char*);
 void __cdecl parse_cstr_00463590(char*,char*);
 void __cdecl parse_float_004635c0(char*,volatile float*);
 void __cdecl parse_int_004635e0(char*,volatile std::int32_t*);
 void __cdecl parse_bool_00463680(char*,volatile bool*);
 void __cdecl parse_vector_00463570(char*,volatile float*);        // "%f %f %f"
 void __cdecl parse_short_00463650(char*,volatile std::int16_t*);  // "%d" -> word store (naked)
 char* __cdecl skip_blanks_00463510(char*);
 int __cdecl kotor_sscanf_006fae08(const char*,const char*,...);   // UCRT-backed provider (see PACKET sscanf_analysis)
 extern char kotor_firstword_buffer_007fbaf8[0x400];
// ---- resource text stream (names from task-17 ports.h; owner: resource layer, not this packet) ----
 char* __cdecl resource_next_0044bfa0();
 void* __cdecl resource_read_0044bfd0(font_txi::U);
// ---- M40 providers (identical declarations) ----
 __declspec(noinline) std::int32_t __cdecl kotor_stricmp_0070acaf(const char*,const char*);
 __declspec(noinline) std::int32_t __cdecl kotor_lower_006fb766(std::int32_t);
 void __cdecl genuine_x87_ftol_006fae8c();
 void __cdecl genuine_x87_pow_006fc1f0();     // task-8 x87-runtime-providers-r1 pow_entry.cpp
// ---- procedural texture constructors (thiscall, one stack word, EAX = object) ----
 void* __fastcall procedure_water_ctor_004630b0(void*,void*,font_txi::Basic*);
 void* __fastcall procedure_base_ctor_00462a70(void*,void*,font_txi::Basic*);
 void* __fastcall procedure_arturo_ctor_00463320(void*,void*,font_txi::Basic*);
 void* __fastcall procedure_wave_ctor_00463260(void*,void*,font_txi::Basic*);
 void* __fastcall procedure_cycle_ctor_00463470(void*,void*,font_txi::Basic*);

// ---- bodies authored by this lane ----
 font_txi::U __fastcall texture_parse_field_00422390(font_txi::Basic*,void*,const char*);      // RET4; Providers::parse_line
 font_txi::U __fastcall font_info_parse_00422210(font_txi::FontInfo*,void*,const char*);       // RET4; raw EAX
 font_txi::U __cdecl read_vector_list_00421ff0(char*,font_txi::List*);                         // raw EAX
 void __fastcall vector_list_allocate_00421310(font_txi::List*,void*,font_txi::I);             // RET4
 font_txi::FontInfo* __fastcall texture_font_info_0041efa0(void*,void*);                       // vtable slot +38 (0073F160/0073F0A8)
 font_txi::U __fastcall font_info_destroy_00420610(font_txi::FontInfo*,void*);                 // Providers::info_dtor
 font_txi::U __cdecl texture_hash_0041f4d0(const char*,font_txi::I,const font_txi::U*);        // Providers::hash
 font_txi::U __fastcall texture_add_ordered_00421560(font_txi::Basic*,void*);                  // Providers::add_ordered
 font_txi::U __fastcall list_insert_00445020(font_txi::List*,void*,font_txi::U,font_txi::I);   // RET8
 void __cdecl scheduler_gamma_words_0045dd90(font_txi::I,font_txi::I,font_txi::U,void*,font_txi::U);
 float __cdecl scheduler_alpha_mean_0045e790(const void*,font_txi::I,font_txi::I,font_txi::U); // Physical ST0 result.
}
