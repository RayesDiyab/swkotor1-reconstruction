#include "38d7531e77702d7e_module_estimate.hpp"
#include <cstring>
#include <new>
#if !defined(_M_IX86)
#error Estimate bridge requires x86
#endif
namespace kotor_reconstruction{struct StartupString;}
namespace kotor_menu_10_19::dependencies{extern void* base_007a39e0;}
namespace f200_ports{void __fastcall Enumerate005e6640(void*,void*,void*,kotor_reconstruction::StartupString*,unsigned,unsigned,unsigned);}
namespace base_ini_reconstruction{kotor_reconstruction::StartupString* __fastcall left_005e5f70(kotor_reconstruction::StartupString*,void*,kotor_reconstruction::StartupString*,int);kotor_reconstruction::StartupString* __fastcall right_005e6170(kotor_reconstruction::StartupString*,void*,kotor_reconstruction::StartupString*,int);}
kotor_reconstruction::StartupString* __fastcall GuiConcat005e5d10(kotor_reconstruction::StartupString*,void*,kotor_reconstruction::StartupString*,const kotor_reconstruction::StartupString*);
extern "C" {
void* __fastcall frame_LogStringCtor_005e5a90(void*,void*,const char*);void __fastcall frame_LogStringDtor_005e5c20(void*,void*);void* __fastcall async_assign_string_005e5c50(void*,void*,void*);unsigned __fastcall async_string_length_005e5790(void*,void*);
unsigned __fastcall c2da_base_005e6670(void*,void*,void*);
void* __fastcall frame_LogFileCtor_005e68d0(void*,void*,void*,unsigned,void*);void __fastcall frame_LogFileDtor_005e69e0(void*,void*);unsigned __fastcall frame_LogFileOpened_005e6a10(void*,void*);unsigned __fastcall frame_LogFileSize_005e6950(void*,void*);
}
namespace{using namespace recovered_module_estimate;using S=kotor_reconstruction::StartupString;static_assert(sizeof(String)==8&&sizeof(Array)==12,"Original string and list layout");
struct Native:Ports{
 void empty(String*s)override{std::memset(s,0,8);}void literal(String*s,const char*t)override{frame_LogStringCtor_005e5a90(s,nullptr,t);}void destroy(String*s)override{frame_LogStringDtor_005e5c20(s,nullptr);}
 void enumerate(Array*a,String*d,unsigned t,unsigned x,unsigned y)override{f200_ports::Enumerate005e6640(kotor_menu_10_19::dependencies::base_007a39e0,nullptr,a,reinterpret_cast<S*>(d),t,x,y);}String*element(Array*a,unsigned i)override{return static_cast<String*>(a->data)+i;}void destroy_array(Array*a)override{unsigned count;std::memcpy(&count,static_cast<char*>(a->data)-4,4);auto*strings=static_cast<String*>(a->data);for(;;){--count;if(static_cast<int>(count)<0)break;destroy(&strings[count]);}::operator delete(static_cast<char*>(a->data)-4);}
 void assign(String*a,String*b)override{async_assign_string_005e5c50(a,nullptr,b);}unsigned length(String*s)override{return async_string_length_005e5790(s,nullptr);}String*left(String*a,String*b,unsigned n)override{return reinterpret_cast<String*>(base_ini_reconstruction::left_005e5f70(reinterpret_cast<S*>(a),nullptr,reinterpret_cast<S*>(b),static_cast<int>(n)));}String*right(String*a,String*b,unsigned n)override{return reinterpret_cast<String*>(base_ini_reconstruction::right_005e6170(reinterpret_cast<S*>(a),nullptr,reinterpret_cast<S*>(b),static_cast<int>(n)));}String*concat(String*a,String*b,String*c)override{return reinterpret_cast<String*>(GuiConcat005e5d10(reinterpret_cast<S*>(a),nullptr,reinterpret_cast<S*>(b),reinterpret_cast<const S*>(c)));}unsigned extension_type(String*s)override{return c2da_base_005e6670(kotor_menu_10_19::dependencies::base_007a39e0,nullptr,s);}
 void file_construct(void**f,String*n,unsigned t,String*m)override{frame_LogFileCtor_005e68d0(f,nullptr,n,t,m);}unsigned file_opened(void**f)override{return frame_LogFileOpened_005e6a10(f,nullptr);}unsigned file_size(void**f)override{return frame_LogFileSize_005e6950(f,nullptr);}void file_destroy(void**f)override{frame_LogFileDtor_005e69e0(f,nullptr);}
};}
extern "C" void __fastcall recovered_estimate_004b5f90(void*self,void*,const recovered_module_estimate::String*name,unsigned type){Native p;recovered_module_estimate::estimate_004b5f90(self,name,type,p);}
