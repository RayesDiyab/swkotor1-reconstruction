// lane-resource-lowers: typed reuse forwards for the five f3c0_ports CExoFile
// ports declared by task-2 f3c0-r1/f3c0.h. No second file body is authored.
// Each port names the SAME original address as the selected M40 provider:
//   005E68D0 CExoFile::CExoFile  -> ::c2da_file_construct_005e68d0 (m39-directory-ports.obj)
//   005E6A10 opened test         -> tlk_file::file_opened_005e6a10 (resource-successor-file-destructor-file_destroy.obj)
//   005E69E0 CExoFile::~CExoFile -> tlk_file::file_destroy_005e69e0 (same obj)
//   005E6960 CExoFile::Read      -> file_internal::exofile_read_005e6960 (m39-operations.obj)
//   005E6AA0 CExoFile::Seek      -> file_internal::exofile_seek_005e6aa0 (m39-size-seek.obj)
// Original ABI evidence (swkotor.exe):
//   005E6A10: mov eax,[ecx]; mov edx,[eax]; setne -> ret (no stack args)        => fastcall(void*,void*)
//   005E69E0: esi=[edi]; call 005EB900; push esi; call 006FA390; [edi]=0; ret   => fastcall(void*,void*)
//   005E6960: mov ecx,[ecx]; jmp 005EBA40 (internal read, RET 0Ch)                => fastcall(void*,void*,void*,W,W)
//   005E6AA0: ecx=[ecx]; eax=[ecx]; je ->xor eax,eax/ret 8; fseek(stream,[esp+4],[esp+8]);
//             neg/sbb/inc => result==0; ret 8                                     => fastcall(void*,void*,W,W)
// The receiver is the four-byte CExoFile whose lifetime Construct005e68d0 began
// with ::new(storage) tlk_file::File (directory_ports.cpp), so the static_cast
// below recovers that same object, as M40 directory_ports.cpp already does.
#include "48d4671e3a-f3c0.h"
#include "a481b9bd58-size_seek.h"
#include <type_traits>
// Existing M40 definition (directory_ports.cpp:9); C++ linkage, so the decorated
// name encodes this full type and any mismatch fails at link.
void* __fastcall c2da_file_construct_005e68d0(void*,void*,kotor_reconstruction::StartupString*,std::uint32_t,kotor_reconstruction::StartupString*);
static_assert(std::is_same<f3c0_ports::S,kotor_reconstruction::StartupString>::value,"one String epoch");
static_assert(std::is_same<f3c0_ports::S,tlk_file::String>::value,"file owner String epoch");
static_assert(std::is_same<f3c0_ports::W,tlk_file::Word>::value&&sizeof(f3c0_ports::W)==4,"one DWORD epoch");
static_assert(std::is_same<decltype(&f3c0_ports::Construct005e68d0),decltype(&::c2da_file_construct_005e68d0)>::value,"identical construct type: ECX raw storage, RET 0Ch");
static_assert(sizeof(tlk_file::File)==4,"opaque measured four-byte receiver is CExoFile4");
namespace f3c0_ports {
void* __fastcall Construct005e68d0(void* storage,void* edx,S* name,W type,S* mode){
 return ::c2da_file_construct_005e68d0(storage,edx,name,type,mode);
}
W __fastcall Opened005e6a10(void* file,void* edx){
 return tlk_file::file_opened_005e6a10(static_cast<tlk_file::File*>(file),edx);
}
void __fastcall Destroy005e69e0(void* file,void* edx){
 tlk_file::file_destroy_005e69e0(static_cast<tlk_file::File*>(file),edx);
}
W __fastcall Read005e6960(void* file,void* edx,void* out,W size,W count){
 return file_internal::exofile_read_005e6960(static_cast<tlk_file::File*>(file),edx,out,size,count);
}
W __fastcall Seek005e6aa0(void* file,void* edx,W offset,W origin){
 return file_internal::exofile_seek_005e6aa0(static_cast<tlk_file::File*>(file),edx,offset,origin);
}
}
