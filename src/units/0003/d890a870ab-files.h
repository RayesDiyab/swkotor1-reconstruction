#pragma once
#include "34de21ea97-file_types.h"
#include <cstddef>
namespace tlk_file {
using Word=std::uint32_t;using String=kotor_reconstruction::StartupString;
// Proposed actual four-byte CExoFile owner. This is never a File24 view.
struct File {file_internal::Internal* internal;};
// Separate actual TLK aggregate, for future Main reconciliation only.
struct TlkFile {File file;Word header[5];};
static_assert(sizeof(File)==4&&offsetof(File,internal)==0,"CExoFile4");
static_assert(sizeof(TlkFile)==24&&offsetof(TlkFile,file)==0&&offsetof(TlkFile,header)==4,"TLK containment");
struct Bindings {
 File* (__fastcall* construct_exofile_005e68d0)(File*,void*,String*,Word,String*);
 Word (__fastcall* seek_beginning_005e6ad0)(File*,void*);
 Word (__fastcall* read_005e6960)(File*,void*,void*,Word,Word);
 void (__fastcall* destroy_internal_005eb900)(file_internal::Internal*,void*);
 void (__cdecl* free_006fa390)(void*);
 const char* binary_read_mode_0073d5b0;
};
static_assert(sizeof(Bindings)==24&&offsetof(Bindings,destroy_internal_005eb900)==12&&offsetof(Bindings,free_006fa390)==16,"same physical six slots, changed typed epoch");
// Main alone owns/publishes this cell and the descriptor lifetime.
extern const Bindings* bindings;
extern "C" Word __fastcall file_opened_005e6a10(File*,void*);
extern "C" void __fastcall file_destroy_005e69e0(File*,void*);
}
