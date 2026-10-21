#pragma once
#include <windows.h>
#include <cstddef>
#include <cstdint>

namespace kotor_runtime_reporter_dependencies {
struct ErrorRecord { int code; const char* text; };
static_assert(sizeof(void*)==4 && sizeof(ErrorRecord)==8, "Original x86 table");
static_assert(offsetof(ErrorRecord,text)==4, "Original text field");
// Required view: 18 records at007A2EE8 plus the adjacent eight bytes at007A2F78.
// Its physical shared-data alias/lifetime must be provided by the data owner.
// The initial adjacent record is codeC0000005 / pointer0000000B. Do not invoke
// that invalid-pointer route as a normal test or sanitize the source branch.
extern const volatile ErrorRecord* error_records_007a2ee8;
extern const volatile int* error_mode_00833c34;
extern const volatile int* application_type_007a2764;
DWORD WINAPI module_file_name_0073d18c(HMODULE,char*,DWORD);
HANDLE WINAPI standard_handle_0073d104(DWORD);
BOOL WINAPI write_file_0073d204(HANDLE,const void*,DWORD,DWORD*,OVERLAPPED*);
std::size_t __cdecl string_length_00701300(const char*);
char* __cdecl string_copy_00705910(char*,const char*);
char* __cdecl string_concat_00705920(char*,const char*);
char* __cdecl string_copy_n_006fab80(char*,const char*,std::size_t);
int __cdecl message_box_007092de(const char*,const char*,unsigned);
}
namespace kotor_selected_termination_dependencies {
void __cdecl report_runtime_error_00701d50(int);
}

