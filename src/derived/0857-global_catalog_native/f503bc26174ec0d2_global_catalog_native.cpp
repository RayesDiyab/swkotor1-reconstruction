#include "14fdd219482b44da_global_catalog.hpp"
#include "903d071617827bcc_table_contract.h"
#include "08e70ea18f1fcd38_load.h"
#include "177a467f7ffe42b2_ini_comparison.h"
#include <cstring>
#include <cctype>
#if !defined(_M_IX86)
#error Catalog native requires x86
#endif
namespace directory_list {char __fastcall At005e5600(kotor_reconstruction::StartupString*,void*,std::int32_t);}
namespace kotor_reconstruction {void __cdecl StartupFormat005e5680(StartupString*,const char*,...);}
extern "C" {
void __fastcall frame_LogStringCtor_005e5a90(void*,void*,const char*);
void __fastcall frame_LogStringDtor_005e5c20(void*,void*);
const char* __fastcall frame_LogCStr_005e5670(void*,void*);
unsigned __fastcall async_string_length_005e5790(void*,void*);
int __fastcall c2da_current_get_string_00413270(void*,void*,int,void*,void*);
}
namespace {
using namespace recovered_global_catalog;
struct Native:LoadPorts{
 int length(String*s)override{return static_cast<int>(async_string_length_005e5790(s,nullptr));}
 signed char character(String*s,int i)override{return static_cast<signed char>(directory_list::At005e5600(reinterpret_cast<kotor_reconstruction::StartupString*>(s),nullptr,i));}
 int uppercase(int c)override{return std::toupper(c);}
 const char*text(String*s)override{return frame_LogCStr_005e5670(s,nullptr);}
 int equal_nc(String*s,const char*t)override{return ini_comparison::CompareNC005e58c0(reinterpret_cast<kotor_reconstruction::StartupString*>(s),nullptr,t);}
 void table_construct(void*t,const void*r,unsigned flag)override{c2da_current::Ref ref;std::memcpy(&ref,r,16);c2da_current_construct_00413cc0(t,nullptr,ref,flag);}
 int table_load(void*t)override{return c2da_current_load_004143b0(static_cast<c2da_current::Table*>(t),nullptr);}
 void table_destroy(void*t)override{c2da_current_destroy_00413d80(static_cast<c2da_current::Table*>(t),nullptr);}
 int table_get(void*t,int row,String*c,String*out)override{return c2da_current_get_string_00413270(t,nullptr,row,c,out);}
 void string_default(String*s)override{std::memset(s,0,8);}
 void string_literal(String*s,const char*t)override{frame_LogStringCtor_005e5a90(s,nullptr,t);}
 void string_destroy(String*s)override{frame_LogStringDtor_005e5c20(s,nullptr);}
 void diagnostic(String*s,const char*f,const char*a,const char*b,int limit)override{auto*out=reinterpret_cast<kotor_reconstruction::StartupString*>(s);if(b)kotor_reconstruction::StartupFormat005e5680(out,f,a,b);else if(limit)kotor_reconstruction::StartupFormat005e5680(out,f,a,limit);else kotor_reconstruction::StartupFormat005e5680(out,f,a);}
};
}
extern "C" unsigned __fastcall recovered_globalcat_initialize_004b1650(void*self,void*){Native p;c2da_current::Ref ref{};ref.construct_00406d80("globalcat");recovered_global_catalog::load_0052ae50(static_cast<char*>(self)+0x100fc,&ref,p);return 1;}
