#include "34158f4d61-ini_file_primitives.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>
namespace kotor_functional {
static_assert(sizeof(int)==4 && sizeof(long)==4 && sizeof(void*)==4,"exact x86 CRT widths");
void* __cdecl modern_ini_open(const char* path,const char* mode) {



    return std::fopen(path,mode);

}
char* __cdecl modern_ini_gets(char* output,int count,void* stream) {



    return std::fgets(output,count,static_cast<std::FILE*>(stream));

}
int __cdecl modern_ini_seek(void* stream,std::int32_t offset,int origin) {



    return std::fseek(static_cast<std::FILE*>(stream),static_cast<long>(offset),origin);

}
int __cdecl modern_ini_close(void* stream) {
    const int result=std::fclose(static_cast<std::FILE*>(stream));



    return result;

}
int __cdecl modern_ini_print(void* stream,const char* format,...) {
    va_list arguments;
    va_start(arguments,format);
    const int result=std::vfprintf(static_cast<std::FILE*>(stream),format,arguments);
    va_end(arguments);



    return result;

}
}
