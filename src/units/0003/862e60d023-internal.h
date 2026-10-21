#pragma once
#include "d890a870ab-files.h"
namespace file_internal {
using Word=tlk_file::Word;using String=tlk_file::String;using File=tlk_file::File;
static_assert(sizeof(Internal)==20&&offsetof(Internal,name)==4&&offsetof(Internal,mode)==12,"observed internal storage");
struct EngineBindings {
 void* (__cdecl* allocate_006fa7e6)(Word);
 String* (__fastcall* assign_005e5c50)(String*,void*,const String*);
 const char* (__fastcall* text_005e5670)(String*,void*);
 String* (__fastcall* resolve_005e68a0)(void*,void*,String*,String*,Word);
 void* volatile* exo_base_007a39e0;
 String* retry_log_008328d8;
 void (__cdecl* format_005e5680)(String*,const char*,...);
 const char* retry_format;const char* null_format;const char* reopen_format;const char* short_format;const char* success_format;
};
// Modern opaque FILE boundary. No historical CRT internals or original code.
struct FileOps {
 std::FILE* (__cdecl* open)(const char*,const char*);
 int (__cdecl* close)(std::FILE*);
 int (__cdecl* seek)(std::FILE*,long,int);
 long (__cdecl* tell)(std::FILE*);
 Word (__cdecl* read)(void*,Word,Word,std::FILE*);
 int (__cdecl* error)(std::FILE*);
 void (__stdcall* delay)(Word);
 int (__cdecl* flush)(std::FILE*);
};
extern const EngineBindings* engine;
extern const FileOps* file_ops;
__declspec(noinline) File* __fastcall exofile_construct_005e68d0(File*,void*,String*,Word,String*);
extern "C" __declspec(noinline) Word __fastcall exofile_read_005e6960(File*,void*,void*,Word,Word);
extern "C" __declspec(noinline) Word __fastcall exofile_seek_beginning_005e6ad0(File*,void*);
extern "C" __declspec(noinline) void __fastcall internal_destroy_005eb900(Internal*,void*);
__declspec(noinline) Internal* __fastcall internal_construct_005eb840(Internal*,void*,String*,Word,String*);
extern "C" __declspec(noinline) Word __fastcall internal_read_005eba40(Internal*,void*,void*,Word,Word);
}
