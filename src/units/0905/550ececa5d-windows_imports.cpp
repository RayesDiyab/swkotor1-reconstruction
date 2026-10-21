#include "2513c38919-runtime_reporter.h"
namespace kotor_runtime_reporter_dependencies {
DWORD WINAPI module_file_name_0073d18c(HMODULE module,char* buffer,DWORD size) {
    return GetModuleFileNameA(module,buffer,size);
}
HANDLE WINAPI standard_handle_0073d104(DWORD kind) { return GetStdHandle(kind); }
BOOL WINAPI write_file_0073d204(HANDLE handle,const void* buffer,DWORD size,DWORD* count,OVERLAPPED* overlap) {
    return WriteFile(handle,buffer,size,count,overlap);
}
}

