#include "45e4b03bb5-client_message.hpp"
#include <cstring>
#include <cstdio>
#if !defined(_M_IX86)
#error Client message bridge requires x86
#endif
namespace kotor_menu_window_cells{extern void*volatile manager_007a39fc;}
namespace kotor_reconstruction{struct StartupString;void __cdecl StartupFormat005e5680(StartupString*,const char*,...);}
extern "C" {unsigned __fastcall recovered_client_player_0066a640(void*,void*,const void*,unsigned);unsigned __fastcall recovered_client_system_00675710(void*,void*,const void*,unsigned);unsigned __fastcall kotor_fastcall_005ed470(void*,void*);void* __fastcall kotor_fastcall_005ed460(void*,void*);void* __fastcall frame_GetWorldTimer_005ed4f0(void*,void*);void __fastcall frame_LogStringDtor_005e5c20(void*,void*);void __fastcall frame_LogWriteString_005e6a70(void*,void*,void*);unsigned __fastcall recovered_file_write_005e69a0(void*,void*,const void*,unsigned,unsigned);}
namespace{using namespace recovered_client_message;void*ptr(void*p,unsigned o){void*v;std::memcpy(&v,static_cast<char*>(p)+o,4);return v;}unsigned rd(void*p,unsigned o){unsigned v;std::memcpy(&v,static_cast<char*>(p)+o,4);return v;}
struct Native:Ports{
 void*client_app()override{return ptr(kotor_menu_window_cells::manager_007a39fc,4);}unsigned logging(void*p)override{return kotor_fastcall_005ed470(p,nullptr);}void*log_file(void*p)override{return kotor_fastcall_005ed460(p,nullptr);}void*timer(void*p)override{return frame_GetWorldTimer_005ed4f0(p,nullptr);}std::uint64_t ticks(void*p)override{std::uint64_t v;std::memcpy(&v,static_cast<char*>(p)+12,8);return v;}
 void empty(String*s)override{std::memset(s,0,8);}void destroy(String*s)override{frame_LogStringDtor_005e5c20(s,nullptr);}void header(String*s,unsigned t,unsigned c,unsigned id,unsigned ignore,unsigned n)override{kotor_reconstruction::StartupFormat005e5680(reinterpret_cast<kotor_reconstruction::StartupString*>(s),"%d %c %d %d %d ",t,c,id,ignore,n);}void newline(String*s)override{kotor_reconstruction::StartupFormat005e5680(reinterpret_cast<kotor_reconstruction::StartupString*>(s),"\n");}void write_string(void*f,String*s)override{frame_LogWriteString_005e6a70(f,nullptr,s);}void write_bytes(void*f,const void*d,unsigned size,unsigned count)override{(void)recovered_file_write_005e69a0(f,nullptr,d,size,count);} /* 005E69A0 CExoFile::Write thiscall RET 0Ch */void*pointer(void*p,unsigned o)override{return ptr(p,o);}void player_message(void*s,unsigned char*d,unsigned n)override{(void)recovered_client_player_0066a640(s,nullptr,d,n);}void system_message(void*s,unsigned char*d,unsigned n)override{(void)recovered_client_system_00675710(s,nullptr,d,n);}
};}
extern "C" unsigned __fastcall recovered_client_message_005ee700(void*self,void*,unsigned id,unsigned char*packet,unsigned length,unsigned ignore){Native p;return recovered_client_message::handle_005ee700(self,id,packet,length,ignore,p);}
