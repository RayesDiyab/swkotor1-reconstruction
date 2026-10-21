#include "1a29dc9d2e3ac481_system_token.hpp"
#include <cstring>
#if !defined(_M_IX86)
#error Client token bridge requires x86
#endif
extern "C"{void __fastcall GuiAssign005e5140(void*,void*,const char*);void* __fastcall resman_string_counted_005e5b70(void*,void*,const char*,int);void* __fastcall async_assign_string_005e5c50(void*,void*,void*);void __fastcall frame_LogStringDtor_005e5c20(void*,void*);}
namespace{using namespace recovered_client_token;static_assert(sizeof(String)==8,"Original String8");struct Native:Ports{void clear(String*s)override{GuiAssign005e5140(s,nullptr,"");}void counted(String*s,const char*t,unsigned n)override{resman_string_counted_005e5b70(s,nullptr,t,static_cast<int>(n));}void assign(String*a,String*b)override{async_assign_string_005e5c50(a,nullptr,b);}void destroy(String*s)override{frame_LogStringDtor_005e5c20(s,nullptr);}};}
extern "C" unsigned __fastcall recovered_client_token_006754d0(void*,void*,const char*data,unsigned length,recovered_client_token::String*a,recovered_client_token::String*b,recovered_client_token::String*c){Native p;return recovered_client_token::parse_006754d0(data,length,a,b,c,p);}
extern "C" void* __fastcall recovered_client_serverinfo_005ed710(void*client,void*){void*inner,*out;std::memcpy(&inner,static_cast<char*>(client)+4,4);std::memcpy(&out,static_cast<char*>(inner)+8,4);return out;}
