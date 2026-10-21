#include "dd80e0b2e910b2a5_packet_dispatch.hpp"
#include <cstring>
#include <cstdio>

#if !defined(_M_IX86)
#error Server packet bridge requires x86
#endif
extern "C" unsigned __fastcall recovered_server_system_00528380(void*,void*,unsigned,const void*,unsigned);
extern "C" unsigned __fastcall recovered_player_message_00527b20(void*,void*,unsigned,void*,unsigned);
extern "C" __declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long,const char*);
namespace {struct Native:recovered_server_packets::Ports{void*pointer(void*s,unsigned o)override{void*p;std::memcpy(&p,static_cast<unsigned char*>(s)+o,4);return p;}void*connection(void*s,unsigned)override{return static_cast<unsigned char*>(s)+0x20014;}void dispatch_00528380(void*message,unsigned id,void*data,unsigned length)override{(void)recovered_server_system_00528380(message,nullptr,id,data,length);}void dispatch_00527b20(void*s,unsigned id,void*p,unsigned n)override{(void)recovered_player_message_00527b20(s,nullptr,id,p,n);}};}
extern "C" unsigned __fastcall recovered_server_packet_004b15d0(void*s,void*,unsigned id,void*packet,unsigned length,unsigned flags){Native p;return recovered_server_packets::dispatch_004b15d0(s,id,packet,length,flags,p);}
