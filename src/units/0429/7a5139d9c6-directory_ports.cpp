#include "6add3cafee-size_seek.h"
#include <new>
#include <type_traits>
using String=kotor_reconstruction::StartupString;
using Word=std::uint32_t;
using File=tlk_file::File;
// C++ linkage preserves the directory caller's potentially throwing constructor
// transport. Start the actual four-byte aggregate lifetime in its raw allocation.
void* __fastcall c2da_file_construct_005e68d0(void* storage,void*,String* name,Word type,String* mode){
 auto* self=::new(storage) File;
 return file_internal::exofile_construct_005e68d0(self,nullptr,name,type,mode);
}
extern "C" Word __fastcall c2da_file_005e6a10(void* self,void*){
 return tlk_file::file_opened_005e6a10(static_cast<File*>(self),nullptr);
}
extern "C" Word __fastcall c2da_file_005e6950(void* self,void*){
 return file_internal::exofile_size_005e6950(static_cast<File*>(self),nullptr);
}
extern "C" Word __fastcall c2da_file_005e6960(void* self,void*,void* out,Word size,Word count){
 return file_internal::exofile_read_005e6960(static_cast<File*>(self),nullptr,out,size,count);
}
using Construct=void*(__fastcall*)(void*,void*,String*,Word,String*);
static_assert(std::is_same<decltype(&c2da_file_construct_005e68d0),Construct>::value,"C++ fastcall, ECX raw four-byte storage, RET12, full pointer EAX");
static_assert(std::is_same<decltype(&c2da_file_005e6960),Word(__fastcall*)(void*,void*,void*,Word,Word)>::value,"read RET12, wrapping byte count");
