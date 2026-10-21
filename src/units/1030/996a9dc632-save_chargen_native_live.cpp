#include "70b1f87b78-save_chargen.hpp"
#include "826a1be1ca-stats_read.hpp"
#include "77fb10ec4f-startup_views.h"
#include "723b658e51-resources.h"
#include <cstring>
#include <new>
#include <cstdint>
#if !defined(_M_IX86)
#error SaveCharGen native bridge requires x86
#endif
using recovered_save_chargen::String;using recovered_save_chargen::Struct;using recovered_save_chargen::List;using recovered_save_chargen::Ref;using recovered_save_chargen::Localized;
extern "C" {
void* __cdecl live_operator_new(std::uint32_t); void __cdecl live_free(void*); // game CRT heap: _live_operator_new 006FA7E6 / _live_free 006FA390 (decls as allocator_live.h)
extern void* kotor_cell_rules_007a3a28;
void* __fastcall exo_locstring_construct_005e9e10(void*,void*);void __fastcall exo_locstring_destroy_005ea000(void*,void*);unsigned __fastcall exo_locstring_add_005ea340(void*,void*,unsigned,void*,unsigned);
void* __fastcall recovered_gff_construct_004105a0(void*,void*);void __fastcall gff_resource_deleting_destroy_00413010(void*,void*,unsigned);
void __fastcall recovered_gff_00411260(void*,void*,Struct*,String*,String*);
void __fastcall recovered_gff_00412450(void*,void*,List*,Struct*,const char*);void __fastcall recovered_gff_004124e0(void*,void*,Struct*,List*,unsigned);
#define SCALAR(a) void __fastcall recovered_gff_##a(void*,void*,Struct*,unsigned,const char*);
SCALAR(00412620) SCALAR(00412670) SCALAR(004126c0) SCALAR(00412710) SCALAR(00412760) SCALAR(004127b0)
#undef SCALAR
void __fastcall recovered_gff_004128c0(void*,void*,Struct*,Ref*,const char*);void __fastcall recovered_gff_00412970(void*,void*,Struct*,void*,const char*);void __fastcall recovered_gff_00412a10(void*,void*,Struct*,Localized*,const char*);void __fastcall recovered_gff_00413030(void*,void*,String*,unsigned);
void __fastcall frame_LogStringCtor_005e5a90(void*,void*,const char*);void __fastcall frame_LogStringDtor_005e5c20(void*,void*);const char* __fastcall frame_LogCStr_005e5670(void*,void*);void __fastcall GuiAssign005e5140(void*,void*,const char*);void* __fastcall async_assign_string_005e5c50(void*,void*,void*);void* __fastcall async_ref_construct_string_00406d60(void*,void*,void*);
unsigned __fastcall kotor_m15b_creature_stats_get_armor_class_00647720(void*,void*,unsigned);unsigned __fastcall kotor_m15b_creature_stats_get_class_00647730(void*,void*,unsigned);unsigned __fastcall kotor_m15b_creature_stats_get_class_level_00647750(void*,void*,unsigned);unsigned __fastcall kotor_m15b_creature_stats_get_feat_006486e0(void*,void*,unsigned);Ref* __fastcall kotor_m15e_creature_stats_get_portrait_00647e80(void*,void*,Ref*,unsigned);
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long,const char*);
}
kotor_reconstruction::StartupString* __fastcall GuiConcat005e5d10(kotor_reconstruction::StartupString*,void*,kotor_reconstruction::StartupString*,const kotor_reconstruction::StartupString*);
namespace kotor_reconstruction{void __cdecl StartupFormat005e5680(StartupString*,const char*,...);}
namespace {
void* ptr(void*s,unsigned o){void*p;std::memcpy(&p,(char*)s+o,4);return p;}
struct Native:recovered_save_chargen::Ports{
void*pointer(void*s,unsigned o)override{return ptr(s,o);}void*rules()override{return kotor_cell_rules_007a3a28;}void*allocate(unsigned n)override{return live_operator_new(n);}void free(void*p)override{live_free(p);}
void localized_construct(Localized*p)override{exo_locstring_construct_005e9e10(p,nullptr);}void localized_add(Localized*p,unsigned lang,void*s,unsigned gender)override{exo_locstring_add_005ea340(p,nullptr,lang,s,gender);}void localized_destroy(Localized*p)override{exo_locstring_destroy_005ea000(p,nullptr);}
void*gff_construct(void*p)override{return recovered_gff_construct_004105a0(p,nullptr);}void gff_initialize(void*g,Struct*r,String*t,String*v)override{recovered_gff_00411260(g,nullptr,r,t,v);}void gff_destroy(void*g,unsigned flag)override{gff_resource_deleting_destroy_00413010(g,nullptr,flag);}
void write_scalar(void*g,Struct*r,unsigned v,const char*l,unsigned kind)override{char label[16]{};std::strncpy(label,l,16);switch(kind){
#define CASE(a) case 0x##a:recovered_gff_##a(g,nullptr,r,v,label);return;
CASE(00412620) CASE(00412670) CASE(004126c0) CASE(00412710) CASE(00412760) CASE(004127b0)
#undef CASE
 default:kotor_unreachable_trap(kind,"Invalid SaveCharGen scalar binding");}}
void write_string(void*g,Struct*r,void*s,const char*l)override{char label[16]{};std::strncpy(label,l,16);recovered_gff_00412970(g,nullptr,r,s,label);}void write_ref(void*g,Struct*r,Ref*s,const char*l)override{char label[16]{};std::strncpy(label,l,16);recovered_gff_004128c0(g,nullptr,r,s,label);}void write_localized(void*g,Struct*r,Localized*s,const char*l)override{char label[16]{};std::strncpy(label,l,16);recovered_gff_00412a10(g,nullptr,r,s,label);}
void add_list(void*g,List*out,Struct*r,const char*l)override{char label[16]{};std::strncpy(label,l,16);recovered_gff_00412450(g,nullptr,out,r,label);}void add_struct(void*g,Struct*out,List*l,unsigned t)override{recovered_gff_004124e0(g,nullptr,out,l,t);}void write_file(void*g,String*p,unsigned type)override{recovered_gff_00413030(g,nullptr,p,type);}
void string_default(String*s)override{std::memset(s,0,8);}void string_literal(String*s,const char*t)override{frame_LogStringCtor_005e5a90(s,nullptr,t);}void string_assign(String*s,const char*t)override{GuiAssign005e5140(s,nullptr,t);}void string_copy(String*a,String*b)override{async_assign_string_005e5c50(a,nullptr,b);}void string_destroy(String*s)override{frame_LogStringDtor_005e5c20(s,nullptr);}const char*text(String*s)override{return frame_LogCStr_005e5670(s,nullptr);}void string_concat(String*out,String*a,String*b)override{GuiConcat005e5d10(reinterpret_cast<kotor_reconstruction::StartupString*>(a),nullptr,reinterpret_cast<kotor_reconstruction::StartupString*>(out),reinterpret_cast<kotor_reconstruction::StartupString*>(b));}void string_format_index(String*out,const char*f,const char*t,unsigned i)override{kotor_reconstruction::StartupFormat005e5680(reinterpret_cast<kotor_reconstruction::StartupString*>(out),f,t,i);}void ref_from_string(Ref*r,String*s)override{async_ref_construct_string_00406d60(r,nullptr,s);}
unsigned armor(void*s,unsigned i)override{return kotor_m15b_creature_stats_get_armor_class_00647720(s,nullptr,i);}unsigned class_level(void*s,unsigned i)override{return kotor_m15b_creature_stats_get_class_level_00647750(s,nullptr,i);}unsigned class_id(void*s,unsigned i)override{return kotor_m15b_creature_stats_get_class_00647730(s,nullptr,i);}unsigned spell_count(void*s,unsigned c,unsigned l)override{return recovered_save_stats::spell_count_006487d0(s,c,l);}unsigned spell(void*s,unsigned c,unsigned l,unsigned i)override{return recovered_save_stats::spell_006495a0(s,c,l,i,ptr);}unsigned feat(void*s,unsigned i)override{return kotor_m15b_creature_stats_get_feat_006486e0(s,nullptr,i);}unsigned max_hp(void*s)override{return recovered_save_stats::maximum_hp_00647a80(s);}Ref*portrait(void*s,Ref*r,unsigned i)override{return kotor_m15e_creature_stats_get_portrait_00647e80(s,nullptr,r,i);}
};}
extern "C" void __fastcall recovered_save_chargen_006123e0(void*self,void*){Native p;recovered_save_chargen::save_006123e0(self,p);}
