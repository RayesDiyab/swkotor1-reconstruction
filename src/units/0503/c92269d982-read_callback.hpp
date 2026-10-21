#pragma once
// Stage copy of Oct4 task-9 font-import-joins-r2/source/read_callback.hpp.
// Changes: the two absolute includes are replaced by the stage header tpc.hpp;
// review T-D1 renames/retypes read_0070c920 to aurora_get_resource_0070c920.
#include "a6c0baad11-tpc.hpp"
#include <type_traits>

namespace font_callback_read {
using Word=font_reader_contract::Word;
using Signed=font_reader_contract::Signed;
using String=kotor_reconstruction::StartupString;

// These remain distinct opaque producer identities. No class layout, common
// base, vtable, field projection, or derived/base cast is asserted here.
struct Model;
struct ModelX;
struct Txi;
struct Texture;
struct ProcessedTexture;
struct Visibility;

// Genuine lower-entry declarations only. Their original owners must provide
// one coherent nominal String epoch and these exact physical x86 adapters.
// ECX receiver, unused EDX, and original remaining stack arguments/RET purge.
extern "C" {
 String* __fastcall assign_text_005e5140(String*,void*,const char*);
 // 005E6080 constructs a real String in raw destination storage and returns
 // its result pointer; the caller separately destroys that destination.
 String* __fastcall lower_construct_005e6080(String*,void*,void* destination);
 String* __fastcall assign_string_005e5c50(String*,void*,String*);
 int __cdecl format_006fadb0(char*,const char*,...);
 char* __cdecl uppercase_007136f3(char*);
 Model* __cdecl model_0070e080(String*,Signed,void*,void*) noexcept(false);
 ModelX* __cdecl modelx_0070eb50(String*) noexcept(false);
 Txi* __cdecl txi_0070ea90(String*) noexcept(false);
 Texture* __cdecl texture_0070e8b0(String*) noexcept(false);
 ProcessedTexture* __cdecl processed_0070ea30(String*) noexcept(false);
 // Saved GZF metadata calls 0070EAF0 stdcall/void, but its JMP target and
 // this caller prove cdecl one argument, pointer EAX, caller ADD ESP,4.
 Visibility* __cdecl visibility_0070eaf0(String*) noexcept(false);
 Word __fastcall data_size_007100a0(void*,void*);
 char* __fastcall data_pointer_007107b0(void*,void*);
 // Raw receiver adapters preserve machine pointer bits. In particular the
 // TGA branch passes Texture* to these entries without a C++ class downcast.
 // Any nominal class/base interpretation must be admitted by their owners.
 Word __fastcall txi_size_raw_0070f410(void*,void*);
 char* __fastcall txi_pointer_raw_0070f3e0(void*,void*);
 Word __fastcall processed_size_0070f3b0(ProcessedTexture*,void*);
 char* __fastcall processed_pointer_0070f380(ProcessedTexture*,void*);

 // Review T-D1: the 007B8FC4 installer stores this name with the cell type
 // aurres::GetCallback = void*(__cdecl*)(const char*,const char*,int32_t*).
 void* __cdecl aurora_get_resource_0070c920(const char* filename,const char* extension,Signed* raw_size) noexcept(false);
}
static_assert(std::is_same<decltype(&aurora_get_resource_0070c920),void* (__cdecl*)(const char*,const char*,std::int32_t*) noexcept(false)>::value,
              "Same installed source callback ABI; three cdecl arguments");
}
