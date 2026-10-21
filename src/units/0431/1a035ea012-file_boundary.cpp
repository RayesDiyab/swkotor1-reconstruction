#include "6383a1b512-modern_boundaries.h"
#include <cstdio>
#include <exception>
#include <windows.h>
namespace {
std::FILE* __cdecl open_file(const char* name,const char* mode){std::FILE* result=nullptr;return fopen_s(&result,name,mode)==0?result:nullptr;}
std::uint32_t __cdecl read_file(void* out,std::uint32_t size,std::uint32_t count,std::FILE* file){return static_cast<std::uint32_t>(std::fread(out,size,count,file));}
void __stdcall delay_file(std::uint32_t duration){Sleep(duration);}
// The selected /MT cohort statically initializes all eight function pointers.
// /MD may require a CRT initializer for imported CRT addresses. Publication
// remains with Main6 after ordinary supported CRT startup.
const file_internal::FileOps file_boundary={open_file,std::fclose,std::fseek,std::ftell,read_file,std::ferror,delay_file,std::fflush};
static_assert(sizeof(file_internal::FileOps)==32&&offsetof(file_internal::FileOps,flush)==28,"FileOps32 flush offset28");
}
namespace tlk_host {
bool FileBoundaryReady() noexcept {
 return file_boundary.open==&open_file&&file_boundary.close==&std::fclose&&
        file_boundary.seek==&std::fseek&&file_boundary.tell==&std::ftell&&
        file_boundary.read==&read_file&&file_boundary.error==&std::ferror&&
        file_boundary.delay==&delay_file&&file_boundary.flush==&std::fflush;
}
const file_internal::FileOps& FileBoundary(){
 if(!FileBoundaryReady())std::terminate();
 return file_boundary;
}
}

namespace file_internal {
// Sole canonical pointer cell borrows the existing genuine /MT FileOps table.
// Its static lifetime exceeds every FILE consumer; no runtime initializer.
const FileOps* file_ops=&file_boundary;
}
