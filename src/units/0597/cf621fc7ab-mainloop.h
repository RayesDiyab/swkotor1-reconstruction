#pragma once
#include "4d5188663d-suffix.h"
#include "41bcc2cb50-menu_r2.h"
#include <exception>
namespace callable_frame {
using U32=menu_continuation::U32;
class OutsideContract final:public std::exception {
public: const U32 frontier;explicit OutsideContract(U32 pc):frontier(pc){}
 const char* what() const noexcept override{return "MainLoop path requires an unimplemented provider or prefix";}
};
struct Client:menu_continuation::Client {U32 __thiscall MainLoop_00602eb0();};
struct PublicClient {U32 word0;Client* internal;U32 __thiscall MainLoop_005ed7d0();};
static_assert(sizeof(Client)==0x4d4&&sizeof(PublicClient)==8&&offsetof(PublicClient,internal)==4,"caller views");
U32 __cdecl typed_dispatch(const frame_menu::Invocation*);
frame_menu::U64 __cdecl typed_wide(const frame_menu::Invocation*);
void active_log_00603fe3();
}
namespace frame_control {extern U32 resource_manager_007a39e8,message_manager_007a3a00,quiet_mode_0083291c;}
namespace frame_log {
struct String {unsigned data,length;};struct File {unsigned internal;};
static_assert(sizeof(String)==8&&sizeof(File)==4,"inferred source-owned local views");
extern const char empty_0073d5d4[],filename_0074edcc[],header_0074ed88[],header_0074ed30[],format_0074ed28[],error_0074ed00[],clear_0073d71c[];
}
extern "C" {
void __fastcall frame_LogStringCtor_005e5a90(frame_log::String*,void*,const char*) noexcept(false);
void __fastcall frame_LogStringDtor_005e5c20(frame_log::String*,void*);
void __fastcall frame_LogFileCtor_005e68d0(frame_log::File*,void*,frame_log::String*,unsigned,frame_log::String*) noexcept(false);
void __fastcall frame_LogFileDtor_005e69e0(frame_log::File*,void*);
unsigned __fastcall frame_LogFileOpened_005e6a10(frame_log::File*,void*) noexcept(false);
unsigned __fastcall frame_LogFileSize_005e6950(frame_log::File*,void*) noexcept(false);
void __fastcall frame_LogWriteText_005e6a40(frame_log::File*,void*,const char*) noexcept(false);
void __fastcall frame_LogWriteString_005e6a70(frame_log::File*,void*,frame_log::String*) noexcept(false);
char* __cdecl frame_LogDate_006fe4a7(char*) noexcept(false);
const char* __fastcall frame_LogCStr_005e5670(frame_log::String*,void*) noexcept(false);
void __cdecl frame_LogFormat_005e5680(frame_log::String*,const char*,...) noexcept(false);
void __fastcall frame_LogAssign_005e5140(frame_log::String*,void*,const char*) noexcept(false);
}
